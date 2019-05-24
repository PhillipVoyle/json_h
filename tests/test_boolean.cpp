
#include <iostream>
#include <string>

#include "json/json.h"

class TestSerializable
{
public:
	int id = 1;
	int intField;
	bool boolField;
};
 
BEGIN_CLASS_DESCRIPTOR(TestSerializable)
   CLASS_DESCRIPTOR_ENTRY(id)
   CLASS_DESCRIPTOR_ENTRY(intField)
   CLASS_DESCRIPTOR_ENTRY(boolField)
END_CLASS_DESCRIPTOR()

int main(int argc, char** argv)
{
   std::string sJSONIn = "{\"id\":5,\"intField\":36,\"boolField\":true}";
   
   TestSerializable tz;
   FromJSON(tz, sJSONIn);
   std::string sJSONOut = ToJSON(tz);
   
   if(sJSONIn == sJSONOut)
   {
      std::cout << "test pass" << std::endl;
   }
   else
   {
      std::cout << "test fail" << std::endl;
   }
   return 0;
}