
#include <iostream>
#include <string>
#include <vector>

#include "json/json.h"

class TestSerializable
{
public:
	std::vector<int> values;
	std::vector<int> moreValues;
};

BEGIN_CLASS_DESCRIPTOR(TestSerializable)
CLASS_DESCRIPTOR_ENTRY(values)
CLASS_DESCRIPTOR_ENTRY(moreValues)
END_CLASS_DESCRIPTOR()

int main(int argc, char** argv)
{
	std::string sJSONIn = "{\"values\":[],\"moreValues\":[]}";

	TestSerializable tz;
	FromJSON(tz, sJSONIn);
	std::string sJSONOut = ToJSON(tz);

	if (sJSONIn == sJSONOut)
	{
		std::cout << "test pass" << std::endl;
	}
	else
	{
		std::cout << "test fail" << std::endl;
	}
	return 0;
}