//
//  JSONWriter.h
//  cppreflect
//
//  Created by Phillip Voyle on 20/06/15.
//  Copyright (c) 2015 PV. All rights reserved.
//

#ifndef cppreflect_JSONWriter_h
#define cppreflect_JSONWriter_h

template<typename TStream>
class JSONWriter
{
  TStream& m_stream;
  bool m_first;
  
public:
  JSONWriter(TStream& stream):m_stream(stream)
  {
      m_first = false;
  }
  void BeginObject(const char* name)
  {
      m_stream << "{";
      m_first = true;
  }
  void EndObject()
  {
      m_stream << "}";
  }
  
  void BeginProperty(const char* name)
  {
      if(m_first)
      {
          m_first = false;
      }
      else
      {
          m_stream << ",";
      }
      
      m_stream << "\"" << name << "\":";
  }
  void EndProperty()
  {
  }
  
  void WriteValue(int value)
  {
      m_stream << value;
  }
  
  void WriteValue(float value)
  {
      m_stream << value;
  }
  
  void WriteValue(const std::string& str)
  {
      m_stream << "\"";
      for(char c :str)
      {
         switch(c)
         {
         case '\t':
            m_stream << "\\t";
            break;
         case '\f':
            m_stream << "\\f";
            break;
         case '\r':
            m_stream << "\\r";
            break;
         case '\n':
            m_stream << "\\n";
            break;
         case '\\':
            m_stream << "\\\\";
         case '"':
            m_stream << "\\\"";
            break;
         case '\b':
            m_stream << "\\b";
            break;
         default:
            m_stream << c;
            break;
         }
      }
      m_stream << "\"";
  }
  
  void WriteValue(bool b)
  {
      if(b)
      {
          m_stream << "true";
      }
      else
      {
          m_stream << "false";
      }
  }
  
  void BeginArray()
  {
      m_stream << "[";
      m_first = true;
  }
  
  void EndArray()
  {
      m_stream << "]";
  }
  
  void BeginItem()
  {
      if(m_first)
      {
          m_first = false;
      }
      else
      {
          m_stream << ",";
      }
  }
  
  void EndItem()
  {
  }
};

template<typename TStream>
JSONWriter<TStream> GetJSONWriter(TStream& stream)
{
  return JSONWriter<TStream>(stream);
}


#endif
