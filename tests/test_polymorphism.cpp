#include <iostream>
#include <string>

#include "json/json.h"

class IOperation
{
public:
   virtual ~IOperation() { }
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

void RWInterface()
{
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
   
   auto result =  test->Execute();
   std::cout << "test->Execute() = " << result << std::endl;
   if((sOut == sJSON) && (result == 176))
   {
      std::cout << "test pass" << std::endl;
   }
   else
   {
      std::cout << "test fail" << std::endl;
   }
}

int main(int argc, char** argv)
{
	RWInterface();
	return 0;
}