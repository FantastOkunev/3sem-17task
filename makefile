a.exe:test.o
	wg++ -g -fno-elide-constructors -std=c++11 test.o
test.o:test.cpp
	wg++ -g -fno-elide-constructors -std=c++11 test.cpp   -c
