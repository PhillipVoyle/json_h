//
//  PointerTypeDescriptor.h
//  cppreflect
//
//  Created by Phillip Voyle on 22/06/15.
//  Copyright (c) 2015 PV. All rights reserved.
//

#ifndef cppreflect_PointerTypeDescriptor_h
#define cppreflect_PointerTypeDescriptor_h

#include <memory>

template<typename T, typename TReader>
std::shared_ptr<T> CreateAndRead(const ClassDescriptor<T>& descriptor, TReader& reader)
{
   auto result = std::make_shared<T>();
   ReadObject(reader, *result);
   return result;
}

template<typename T>
class ClassDescriptor;

template<typename T>
class PointerTypeDescriptor
{
};

template<typename T>
class ClassDescriptor<std::shared_ptr<T>>
{
public:
   typedef PointerTypeDescriptor<std::shared_ptr<T>> descriptor_t;
};

template<typename TReader, typename T>
void DispatchReadObject(const PointerTypeDescriptor<std::shared_ptr<T>> & descriptor, TReader &reader, std::shared_ptr<T>& t)
{
   typename ClassDescriptor<T>::descriptor_t desc;
   t = CreateAndRead(desc, reader);
}

template<typename TWriter, typename T>
void DispatchWriteObject(const PointerTypeDescriptor<std::shared_ptr<T>> & descriptor, TWriter &writer, const std::shared_ptr<T>& t)
{
   if(t == nullptr)
   {
      writer.WriteNull();
   }
   else
   {
      WriteObject(writer, *t.get());
   }
}


#endif
