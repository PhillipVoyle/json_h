all: test_boolean.exe test_polymorphism.exe

test_boolean.exe: test_boolean.cpp
	g++ test_boolean.cpp -o test_boolean.exe -I ../include -std=c++11

test_polymorphism.exe: test_polymorphism.cpp
	g++ test_polymorphism.cpp -o test_polymorphism.exe -I ../Include -std=c++11

clean:
	rm test_boolean.exe
	rm test_polymorphism.exe
