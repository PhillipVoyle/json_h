//
//  JSON.h
//  cppreflect
//
//  Created by Phillip Voyle on 21/06/15.
//  Copyright (c) 2015 PV. All rights reserved.
//

#ifndef cppreflect_JSON_h
#define cppreflect_JSON_h

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <map>

#include "JSONReader.h"
#include "JSONWriter.h"

#include "ClassDescriptor.h"
#include "ArrayTypeDescriptor.h"
#include "VectorTypeDescriptor.h"
#include "MapTypeDescriptor.h"

template<typename TWriter, typename T>
void WriteObject(TWriter& writer, const T& t);

template<typename TReader, typename T>
void ReadObject(TReader& reader, T& t);

template<typename TStream, typename T>
void WriteJSON(JSONWriter<TStream> &writer, const T& t)
{
   WriteObject(writer, t);
}

template<typename TStream, typename T>
void WriteJSON(TStream &stream, const T& t)
{
   auto writer = GetJSONWriter(stream);
   WriteObject(writer, t);
}

template<typename T>
std::string ToJSON(const T& t)
{
   std::stringstream ssOut;
   WriteJSON(ssOut, t);
   return ssOut.str();
}

template<typename TStream, typename T>
void ReadJSON(JSONReader<TStream>& reader, T& t)
{
   ReadObject(reader, t);
}

template<typename TStream, typename T>
void ReadJSON(TStream& stream, T& t)
{
   auto jsonReader = GetJSONReader(stream);
   ReadObject(jsonReader, t);
}

template<typename T>
void FromJSON(T& t, const std::string& json)
{
   std::stringstream ssIn(json);
   ReadJSON(ssIn, t);
}

#endif
