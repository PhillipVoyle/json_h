//
//  MapTypeDescriptor.h
//  cppreflect
//
//  Created by Phillip Voyle on 21/06/15.
//  Copyright (c) 2015 PV. All rights reserved.
//

#ifndef cppreflect_MapTypeDescriptor_h
#define cppreflect_MapTypeDescriptor_h

#include <map>

template<typename T>
class ClassDescriptor;

template<typename T>
class MapTypeDescriptor
{
};

template<typename T>
class ClassDescriptor<std::map<std::string, T>>
{
public:
   typedef MapTypeDescriptor<std::map<std::string, T>> descriptor_t;
};

template<typename TReader, typename T>
void DispatchReadObject(const MapTypeDescriptor<std::map<std::string, T>>& descriptor, TReader &reader, std::map<std::string, T>& t)
{
   reader.EnterObject();
   if(!reader.IsEndObject())
   {
      std::string sProperty;
      reader.FirstProperty(sProperty);
      for(;;)
      {
         ReadJSON(reader, t[sProperty]);
         if(reader.IsEndObject())
         {
            break;
         }
         reader.NextProperty(sProperty);
      }
   }
   reader.LeaveObject();
}


template<typename TWriter, typename T>
void DispatchWriteObject(const MapTypeDescriptor<std::map<std::string, T>>& descriptor, TWriter& writer, const std::map<std::string, T>& t)
{
   writer.BeginObject("map");
   for(auto it = t.begin(); it != t.end(); it++)
   {
      writer.BeginProperty(it->first.c_str());
      WriteJSON(writer, it->second);
      writer.EndProperty();
   }
   writer.EndObject();
}

#endif
