//
//  ArrayTypeDescriptor.h
//  cppreflect
//
//  Created by Phillip Voyle on 21/06/15.
//  Copyright (c) 2015 PV. All rights reserved.
//

#ifndef cppreflect_ArrayTypeDescriptor_h
#define cppreflect_ArrayTypeDescriptor_h

template<typename T>
class ClassDescriptor;

template<typename T>
class ArrayTypeDescriptor;

template<typename T, int N>
class ArrayInserter
{
   T* m_curr;
   T* m_end;
public:
   ArrayInserter(T(&t)[N])
   {
      m_curr = t;
      m_end = t+N;
   }
   bool ok()
   {
      return m_curr != m_end;
   }
   
   T& insert()
   {
      return *(m_curr++);
   }
};


template<typename T, int N>
class ArrayTypeDescriptor<T[N]>
{
public:
   ArrayInserter<T,N> begin_insert(T(&t)[N]) const
   {
      return ArrayInserter<T, N>(t);
   }

   size_t get_size(const T(&t)[N]) const
   {
      return N;
   }
};

template<typename T, int N>
class ClassDescriptor<T[N]>
{
public:
   typedef ArrayTypeDescriptor<T[N]> descriptor_t;
};

template<typename TWriter, typename T>
void DispatchWriteObject(const ArrayTypeDescriptor<T>& descriptor, TWriter &writer, const T& t)
{
   writer.BeginArray();
   auto size = descriptor.get_size(t);
   for(int n = 0; n < size; n++)
   {
      writer.BeginItem();
      WriteJSON(writer, t[n]);
      writer.EndItem();
   }
   writer.EndArray();
}


template<typename TReader, typename T>
void DispatchReadObject(const ArrayTypeDescriptor<T>& descriptor, TReader &reader, T& t)
{
   reader.EnterArray();
   auto it = descriptor.begin_insert(t);
   if(it.ok() && !reader.IsEndArray())
   {
      do
      {
         ReadObject(reader, it.insert());
         if(reader.IsEndArray())
         {
            break;
         }
         reader.NextItem();
      }
      while(it.ok());
   }
   reader.LeaveArray();
}

#endif
