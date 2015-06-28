//
//  JSONLexer.h
//  cppreflect
//
//  Created by Phillip Voyle on 20/06/15.
//  Copyright (c) 2015 PV. All rights reserved.
//

#ifndef cppreflect_JSONLexer_h
#define cppreflect_JSONLexer_h

 class JSONLexicon
 {
 public:
     enum lexicon_type
     {
         SYMBOL,
         INTEGER,
         FLOAT,
         STRING,
         IDENTIFIER,
         END
     } type;
     std::string lexicon;
 };

 template<typename TStream>
 class JSONLexer
 {
   TStream& m_stream;
   JSONLexicon next;
   int nLine;

   char Next()
   {
      if(m_stream.eof())
      {
         throw std::runtime_error("end of file");
      }
      m_stream.get();
      return m_stream.peek();
   }

   void Read()
   {
      next.lexicon = "";
      if(m_stream.eof())
      {
         next.type = JSONLexicon::END;
      }
      
      char c = m_stream.peek();
      while(isspace(c))
      {
          if(c == '\n')
              nLine++;
          c = Next();
      }
      
      if(c == EOF)
      {
          next.type = JSONLexicon::END;
          return;
      }
      else if( (c == '{') || (c == ':') ||
          (c == '}') || (c == ',') ||
          (c == '[') || (c == ']'))
      {
          next.lexicon.push_back(c);
          next.type = JSONLexicon::SYMBOL;
          
          c = Next();
          return;
      }
      else if (isalpha(c))
      {
          next.type = JSONLexicon::IDENTIFIER;
          do
          {
              next.lexicon.push_back(c);
              c = Next();
          }
          while(isalnum(c));
          return;
      }
      else if (isdigit(c))
      {
          //todo: parse floating point and scientific format numbers
          do
          {
              next.lexicon.push_back(c);
              c = Next();
          }
          while(isdigit(c));
          if(c != '.')
          {
              next.type = JSONLexicon::INTEGER;
              return;
          }
          
          do
          {
              next.lexicon.push_back(c);
              c = Next();
          }
          while(isdigit(c));
          next.type = JSONLexicon::FLOAT;
          return;
      }
      else if(c == '"')
      {
         //todo: escape string
         next.type = JSONLexicon::STRING;
         for(;;)
         {
            c = Next();
          
            if(c == '"')
               break;
            if(c == '\\')
            {
               c = Next();
               switch(c)
               {
               case '"':
                  break;
               case '\\':
                  next.lexicon.push_back('\\');
                  break;
               case '/':
                  next.lexicon.push_back('/');
                  break;
               case 'b':
                  next.lexicon.push_back('\b');
                  break;
               case 'f':
                  next.lexicon.push_back('\f');
                  break;
               case 'r':
                  next.lexicon.push_back('\r');
                  break;
               case 't':
                  next.lexicon.push_back('\t');
                  break;
               case 'u':
                  throw std::runtime_error("unicode strings not supported");
               default:
                  throw std::runtime_error("unexpected escape sequence");
               }
            }
            else
            {
               next.lexicon.push_back(c);
            }
         }
         c = Next();
         return;
      }
      else
      {
          c = Next();
          throw std::runtime_error("unexpected character");
      }
   }

   public:
   JSONLexer(TStream& stream):m_stream(stream)
   {
      nLine = 1;
      Read();
   }

   const JSONLexicon& Get()
   {
      return next;
   }

   void Advance()
   {
      Read();
   }
};
 
 template<typename TStream>
 JSONLexer<TStream> GetJSONLexer(TStream& stream)
 {
     return JSONLexer<TStream>(stream);
 }

#endif
