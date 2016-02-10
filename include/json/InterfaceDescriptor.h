//
//  InterfaceDescriptor.h
//  cppreflect
//
//  Created by Phillip Voyle on 25/06/15.
//  Copyright (c) 2015 PV. All rights reserved.
//

#ifndef cppreflect_InterfaceDescriptor_h
#define cppreflect_InterfaceDescriptor_h

#include <memory>

template<typename T>
class InterfaceDescriptor
{
};

#define BEGIN_INTERFACE_DESCRIPTOR(X) \
template<> class InterfaceDescriptor<X>; \
template<> class ClassDescriptor<X> { public: typedef InterfaceDescriptor<X> descriptor_t;}; \
template<> \
class InterfaceDescriptor<X> { public: \
   template<typename TCallback>\
   void for_each_interface(TCallback callback) const\
   {

#define INTERFACE_DESCRIPTOR_ENTRY(X) callback(ClassDescriptor<X>::descriptor_t{});

#define END_INTERFACE_DESCRIPTOR() \
   } \
};

template<typename TInterface, typename TReader>
class TryReadObjectFunctor
{
   std::shared_ptr<TInterface>& m_t;
   TReader &m_reader;
   std::string m_type;
public:
   TryReadObjectFunctor(std::shared_ptr<TInterface>& t, TReader &reader, const std::string& type):m_t(t), m_reader(reader), m_type(type)
   {
   }

   template<typename TConcrete>
   void operator()(ClassDescriptor<TConcrete> descriptor) const
   {
      if(m_type != descriptor.get_name())
      {
         return;
      }
      
      auto t = std::make_shared<TConcrete>();
      m_t = t;
      
      std::string sProperty;
      while(!m_reader.IsEndObject())
      {
         m_reader.NextProperty(sProperty);
         
         ReadObjectFunctor<TReader, TConcrete> functor {m_reader, *t, sProperty};
         descriptor.for_each_property(functor);
         if(!functor.m_bFound)
         {
            throw std::runtime_error("could not find property");
         }
      }
   }
};

template<typename TReader, typename T>
std::shared_ptr<T> CreateAndRead(const InterfaceDescriptor<T> & descriptor, TReader &reader)
{
   std::shared_ptr<T> result;
   reader.EnterObject();
   if(!reader.IsEndObject())
   {
      std::string sProperty;
      reader.FirstProperty(sProperty);
      if(sProperty != "type")
      {
         throw std::runtime_error("expected type property");
      }
      std::string sType;
      ReadObject(reader, sType);
      
      TryReadObjectFunctor<T, TReader> functor {result, reader, sType};
      descriptor.for_each_interface(functor);
   }
   reader.LeaveObject();
   
   return result;
}

template<typename TInterface, typename TWriter>
class TryWriteObjectFunctor
{
   const TInterface& m_t;
   TWriter &m_writer;
public:
   TryWriteObjectFunctor(const TInterface& t, TWriter &writer):m_t(t), m_writer(writer)
   {
   }

   template<typename TConcrete>
   void operator()(ClassDescriptor<TConcrete> descriptor) const
   {
      try {
         const TConcrete& concrete = dynamic_cast<const TConcrete&>(m_t); //todo: prefer faster mechanism?
         
         m_writer.BeginObject(descriptor.get_name());
         m_writer.BeginProperty("type");
         
         WriteObjectFunctor<TWriter, TConcrete> functor(m_writer, concrete, false);
         WriteObject(m_writer, std::string(descriptor.get_name())); //fixme: (allow const char*)
         m_writer.EndProperty();
         descriptor.for_each_property(functor);
         m_writer.EndObject();
      }
      catch (std::bad_cast e)
      {
         //ignore it
      }
   }
};

template<typename TWriter, typename T>
void DispatchWriteObject(const InterfaceDescriptor<T> & descriptor, TWriter &writer, const T& t)
{
   TryWriteObjectFunctor<T, TWriter> functor {t, writer};
   descriptor.for_each_interface(functor);
}

#endif
