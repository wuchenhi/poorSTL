main:
	g++ test.cpp -g -std=c++11 -fno-elide-constructors -o test 

clean:
	rm test