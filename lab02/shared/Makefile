all:
	gcc -Wall -fPIC -c CollatzLibrary.c
	gcc -Wall -shared CollatzLibrary.o -o libCollatzLibrary.so 
	gcc -Wall main.c -L. -lCollatzLibrary -Wl,-rpath=. -o main_shared 

clean:
	rm CollatzLibrary.o libCollatzLibrary.so main_shared