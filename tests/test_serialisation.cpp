#include <iostream>
#include <string>

#include "json/json.h"


int main(int argc, char** argv)
{
    std::string theString = "test string:\"testing\\ \t\n\"";
    std::string expectedString = R"("test string:\"testing\\ \t\n\"")";

    auto actualString = ToJSON(theString);

    if (actualString != expectedString)
    {
        std::cerr << "TEST FAIL" << std::endl;
        std::cerr << "actual: " << actualString << std::endl;
        std::cerr << "expected: " << expectedString << std::endl;
        return 1;
    }
    else
    {
        std::cout << "TEST PASS" << std::endl;
    }

	return 0;
}