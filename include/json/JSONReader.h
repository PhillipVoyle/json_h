//
//  JSONReader.h
//  cppreflect
//
//  Created by Phillip Voyle on 20/06/15.
//  Copyright (c) 2015 PV. All rights reserved.
//

#ifndef cppreflect_JSONReader_h
#define cppreflect_JSONReader_h

#include "JSONLexer.h"

template<typename TStream>
class JSONReader
{
   JSONLexer<TStream> m_lexer;
   
   bool ReadString(std::string& result)
   {
      if(m_lexer.Get().type == JSONLexicon::STRING)
      {
          result = m_lexer.Get().lexicon;
          m_lexer.Advance();
          return true;
      }
      else
      {
         return false;
      }
  }
  
public:
   JSONReader(TStream& stream):m_lexer(stream)
   {
   }
  
   bool MatchSymbol(const std::string& symbol)
   {
      return (m_lexer.Get().type == JSONLexicon::SYMBOL) && (symbol == m_lexer.Get().lexicon);
   }

   bool IsBeginObject()
   {
      return MatchSymbol("{");
   }

   bool IsEndObject()
   {
      return MatchSymbol("}");
   }

   void EnterObject()
   {
      if(MatchSymbol("{"))
      {
          m_lexer.Advance();
      }
      else
      {
          throw std::runtime_error("expected '{'");
      }
   }

   void LeaveObject()
   {
      if(MatchSymbol("}"))
      {
          m_lexer.Advance();
      }
      else
      {
          throw std::runtime_error("expected '}'");
      }
   }

   bool IsBeginArray()
   {
      return MatchSymbol("[");
   }

   bool IsEndArray()
   {
      return MatchSymbol("]");
   }

   void EnterArray()
   {
      if(MatchSymbol("["))
      {
          m_lexer.Advance();
      }
      else
      {
          throw std::runtime_error("expected '['");
      }
   }

   void LeaveArray()
   {
      if(MatchSymbol("]"))
      {
          m_lexer.Advance();
      }
      else
      {
          throw std::runtime_error("expected ']'");
      }
   }

   void NextItem()
   {
      if(MatchSymbol(","))
      {
          m_lexer.Advance();
      }
      else
      {
          throw std::runtime_error("expected ','");
      }
   }

   void ReadProperty(std::string& property)
   {
      if(ReadString(property))
      {
          if(!MatchSymbol(":"))
          {
              throw std::runtime_error("expected ':'");
          }
          m_lexer.Advance();
      }
      else
      {
          throw std::runtime_error("expected property");
      }
   }

   void FirstProperty(std::string& property)
   {
      ReadProperty(property);
   }

   void NextProperty(std::string& property)
   {
      if(MatchSymbol(","))
      {
          m_lexer.Advance();
          ReadProperty(property);
      }
      else
      {
          throw std::runtime_error("expected ','");
      }
   }

   bool MatchIdentifier(const std::string& id)
   {
      return (m_lexer.Get().type == JSONLexicon::IDENTIFIER) && (id == m_lexer.Get().lexicon);
   }

   void ReadValue(bool& result)
   {
      if((m_lexer.Get().type == JSONLexicon::STRING) && (m_lexer.Get().lexicon == "true"))
      {
          result = true;
          m_lexer.Advance();
      }
      else if((m_lexer.Get().type == JSONLexicon::STRING) && (m_lexer.Get().lexicon == "false"))
      {
          result = false;
          m_lexer.Advance();
      }
      else
      {
         throw std::runtime_error("expected boolean");
      }
   }

   void ReadValue(std::string& result)
   {
      if(!ReadString(result))
      {
         throw std::runtime_error("expected string");
      }
   }

   void ReadValue(int& result)
   {
      if(m_lexer.Get().type == JSONLexicon::INTEGER)
      {
          std::stringstream ssInt(m_lexer.Get().lexicon);
          ssInt >> result;
          m_lexer.Advance();
      }
      else
      {
         throw std::runtime_error("expected integer");
      }
   }

   bool ReadValue(float& result)
   {
      if((m_lexer.Get().type == JSONLexicon::FLOAT) || (m_lexer.Get().type == JSONLexicon::INTEGER))
      {
          std::stringstream ssFloat(m_lexer.Get().lexicon);
          ssFloat >> result;
          m_lexer.Advance();
      }
      else
      {
         throw std::runtime_error("expected float");
      }
   }
};

template<typename TStream>
JSONReader<TStream> GetJSONReader(TStream& stream)
{
  return JSONReader<TStream>(stream);
}

#endif
