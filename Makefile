all:
	g++ -g main.cpp cesu2utf8.cpp -o cesu2utf8

clean:
	rm -f *.o
	rm -f cesu2utf8
