all: test_boolean test_polymorphism

test_boolean: test_boolean.cpp
	g++ -I../include -std=c++11 test_boolean.cpp -o test_boolean

test_polymorphism: test_polymorphism.cpp
	g++ -I../include -std=c++11 test_polymorphism.cpp -o test_polymorphism

clean:
	rm -f test_boolean
	rm -f test_polymorphism
