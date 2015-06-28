//
//  VectorTypeDescriptor.h
//  cppreflect
//
//  Created by Phillip Voyle on 21/06/15.
//  Copyright (c) 2015 PV. All rights reserved.
//

#ifndef cppreflect_VectorTypeDescriptor_h
#define cppreflect_VectorTypeDescriptor_h

#include <vector>
#include "ArrayTypeDescriptor.h"

template<typename T>
class VectorInserter
{
   std::vector<T>& m_vec;
public:
   VectorInserter(std::vector<T>& vec):m_vec(vec)
   {
      vec.clear();
   }
   
   bool ok()
   {
      return true;
   }
   
   T& insert()
   {
      m_vec.emplace_back();
      return m_vec.back();
   }
};

template<typename T>
class ArrayTypeDescriptor<std::vector<T>>
{
public:
   VectorInserter<T> begin_insert(std::vector<T>& vec) const
   {
      return VectorInserter<T>(vec);
   }

   size_t get_size(const std::vector<T>& vec) const
   {
      return vec.size();
   }
};

template<typename T>
class ClassDescriptor<std::vector<T>>
{
public:
   typedef ArrayTypeDescriptor<std::vector<T>> descriptor_t;
};


#endif
