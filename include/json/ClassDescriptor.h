//
//  ClassDescriptor.h
//  cppreflect
//
//  Created by Phillip Voyle on 21/06/15.
//  Copyright (c) 2015 PV. All rights reserved.
//

#ifndef cppreflect_ClassDescriptor_h
#define cppreflect_ClassDescriptor_h

template<typename T>
class PrimitiveTypeDescriptor
{
};

template<typename TClass>
class ClassDescriptor
{
public:
   typedef PrimitiveTypeDescriptor<TClass> descriptor_t;
};

#define BEGIN_CLASS_DESCRIPTOR(X) \
   template<> \
   class ClassDescriptor<X> \
   { \
   public: \
   typedef X class_t; \
      typedef ClassDescriptor<X> descriptor_t; \
      constexpr const char* get_name() const { return #X; } \
      template<typename TCallback> \
      void for_each_property(TCallback& callback) const {

#define CLASS_DESCRIPTOR_ENTRY(X) callback(#X, &class_t::X);

#define END_CLASS_DESCRIPTOR() }};

template<typename T>
typename ClassDescriptor<T>::descriptor_t GetTypeDescriptor(const T& t)
{
   return typename ClassDescriptor<T>::descriptor_t {};
}

template <typename TWriter, typename TClass>
class WriteObjectFunctor
{
   TWriter& m_writer;
   const TClass& m_t;
   bool m_first;
public:
   WriteObjectFunctor(TWriter& writer, const TClass& t):m_writer(writer), m_t(t)
   {
      m_first = true;
   }
   
   template<typename TPropertyType>
   void operator()(const char* szProperty, TPropertyType TClass::*pPropertyOffset)
   {
      m_writer.BeginProperty(szProperty);
      WriteObject(m_writer, m_t.*pPropertyOffset);
      m_writer.EndProperty();
   }
};


template<typename TReader, typename T>
void DispatchReadObject(const PrimitiveTypeDescriptor<T>& descriptor, TReader &reader, T& t)
{
   reader.ReadValue(t);
}

template<typename TWriter, typename T>
void DispatchWriteObject(const ClassDescriptor<T>& descriptor, TWriter &writer, const T &t)
{
   WriteObjectFunctor<TWriter, T> functor(writer, t);
   writer.BeginObject(descriptor.get_name());
   descriptor.for_each_property(functor);
   writer.EndObject();
}

template <typename TReader, typename TClass>
class ReadObjectFunctor
{
   TReader& m_reader;
   TClass& m_t;
   std::string m_sProperty;
public:
   bool m_bFound;

   ReadObjectFunctor(TReader& reader, TClass& t, std::string sProperty):m_reader(reader), m_t(t), m_sProperty(sProperty)
   {
      m_bFound = false;
   }
   
   template<typename TPropertyType>
   void operator()(const char* szProperty, TPropertyType TClass::*pPropertyOffset)
   {
      if(m_sProperty == szProperty)
      {
         ReadJSON(m_reader, m_t.*pPropertyOffset);
         m_bFound = true;
      }
   }
};


template<typename TReader, typename T>
void DispatchReadObject(const ClassDescriptor<T>& descriptor, TReader &reader, T &t)
{
   reader.EnterObject();
   if(!reader.IsEndObject())
   {
      std::string sProperty;
      reader.FirstProperty(sProperty);
      for(;;)
      {
         ReadObjectFunctor<TReader, T> functor {reader, t, sProperty};
         descriptor.for_each_property(functor);
         if(!functor.m_bFound)
         {
            throw std::runtime_error("could not find property");
         }
         
         if(reader.IsEndObject())
         {
            break;
         }
         reader.NextProperty(sProperty);
      }
   }
   reader.LeaveObject();
}

template<typename TReader, typename T>
void ReadObject(TReader&reader, T& t)
{
   DispatchReadObject(GetTypeDescriptor(t), reader, t);
}

template<typename TWriter, typename T>
void DispatchWriteObject(const PrimitiveTypeDescriptor<T>& descriptor, TWriter &writer, const T& t)
{
   writer.WriteValue(t);
}

template<typename TWriter, typename T>
void WriteObject(TWriter& writer, const T& t)
{
   DispatchWriteObject(GetTypeDescriptor(t), writer, t);
}

#endif
