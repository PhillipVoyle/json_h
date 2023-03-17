# json.h
A JSON Serialisation Project for C++

How to use this software
========================
Install the header files into your include path, and then you can include the
main header file in your source file.

``` cpp
#include <json/json.h>
```

Class Stubs
-----------
Each class you want to reflect will have publicly exposed properties that can
be set or read by the JSON reader/writer. To expose your classes to the JSON
reader and writer you use the BEGIN_CLASS_DESCRIPTOR, END_CLASS_DESCRIPTOR, and
CLASS_DESCRIPTOR_ENTRY preprocessor macros. 

``` cpp
#include <json/json.h>

class AClass
{
public:
   int aValue;
   int anotherValue;
   std::string thirdValue;
};

BEGIN_CLASS_DESCRIPTOR(AClass)
   CLASS_DESCRIPTOR_ENTRY(aValue)
   CLASS_DESCRIPTOR_ENTRY(anotherValue)
   CLASS_DESCRIPTOR_ENTRY(thirdValue)
END_CLASS_DESCRIPTOR()

```

Conversion to and from Strings
------------------------------
json.h allows C++ objects with descriptor stubs to be converted to and from
strings using the ToJSON and FromJSON functions.

``` cpp

void WriteRead()
{
	AClass srcObject = {1, 2, "three"};
	std::string jsonString = ToJSON(srcObject);
	
	// prints {"aValue":1,"anotherValue":2,"thirdValue":"three"}
	std::cout << jsonString << std::endl;
	
	AClass dstObject;
	//populates dstObject with 1, 2, "three"
	FromJSON(dstObject, jsonString);
}

```

Using Streams
-------------
It's also possible to use json.h with C++ Streams, such as std::istream, and
std::ostream. You can do so using ReadJSON and WriteJSON. You can even do so
using std::cin and std::cout.

``` cpp

void ReadWrite()
{
	std::ifstream readFile("in.json");
	
	AClass anObject;
	ReadJSON(readFile, anObject);
	
	std::ofstream writeFile("out.json");
	WriteJSON(writeFile, anObject);
}

```

Interface Stubs
---------------
To use polymorphic types you also need to supply an interface descriptor stub.
You can specify these using the BEGIN_INTERFACE_DESCRIPTOR,
END_INTERFACE_DESCRIPTOR, and INTERFACE_DESCRIPTOR_ENTRY preprocessor macros.

``` cpp

class IOperation
{
public:
   virtual float Execute() = 0;
};

class Add : public IOperation
{
public:
   std::shared_ptr<IOperation> operand1;
   std::shared_ptr<IOperation> operand2;
   Add()
   {
   }
   Add(std::shared_ptr<IOperation> o1, std::shared_ptr<IOperation> o2):operand1(o1), operand2(o2)
   {
   }
   
   float Execute()
   {
      return operand1->Execute() + operand2->Execute();
   }
};

class Multiply : public IOperation
{
public:
   std::shared_ptr<IOperation> operand1;
   std::shared_ptr<IOperation> operand2;
   Multiply()
   {
   }
   Multiply(std::shared_ptr<IOperation> o1, std::shared_ptr<IOperation> o2):operand1(o1), operand2(o2)
   {
   }
   
   float Execute()
   {
      return operand1->Execute() * operand2->Execute();
   }
};

class Value: public IOperation
{
public:
   float value;
   
   Value()
   {
      value = 0;
   }
   Value(float v): value(v)
   {
   }
   
   float Execute()
   {
      return value;
   }
};

BEGIN_CLASS_DESCRIPTOR(Value)
   CLASS_DESCRIPTOR_ENTRY(value)
END_CLASS_DESCRIPTOR();

BEGIN_CLASS_DESCRIPTOR(Add)
   CLASS_DESCRIPTOR_ENTRY(operand1)
   CLASS_DESCRIPTOR_ENTRY(operand2)
END_CLASS_DESCRIPTOR()

BEGIN_CLASS_DESCRIPTOR(Multiply)
   CLASS_DESCRIPTOR_ENTRY(operand1)
   CLASS_DESCRIPTOR_ENTRY(operand2)
END_CLASS_DESCRIPTOR()

BEGIN_INTERFACE_DESCRIPTOR(IOperation)
   INTERFACE_DESCRIPTOR_ENTRY(Value)
   INTERFACE_DESCRIPTOR_ENTRY(Add)
   INTERFACE_DESCRIPTOR_ENTRY(Multiply)
END_INTERFACE_DESCRIPTOR()
```
At this point you will be able to load and save shared_ptr smart pointers
containing your polymorphic objects.

``` cpp
   std::string sJSON = "{"
      "\"type\":\"Add\","
         "\"operand1\":{\"type\":\"Multiply\","
            "\"operand1\":{\"type\":\"Value\",\"value\":10},"
            "\"operand2\":{\"type\":\"Value\",\"value\":15}}"
         ",\"operand2\":{\"type\":\"Value\",\"value\":26}}";

   std::shared_ptr<IOperation> test;
   FromJSON(test, sJSON);
   
   std::string sOut = ToJSON(test);
   std::cout << sOut << std::endl;

```

Authoring New Descriptor Types
------------------------------
A descriptor can be thought of as a set of functions and specialisations that allows json.h to find the specific specialisation for the type, and a method for taking that matching classes and performing derialisation and deserialisation on them

A good example is in MapTypeDescriptor.h


```
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
```

Note the specialisation of `ClassDescriptor` for maps of `std::string` to type `T` which indicates the type of descriptor `MapTypeDescriptor`. Then there are template implementations for the `DispatchWriteObject` and `DispatchReadObject` which define how instances of the object type would be converted to and from JSON via the JsonReader and JsonWriter object types. Note that these are not explicitly mentioned here, and the implementor is free to replace the implementation of these objects if desired

