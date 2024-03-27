all:
	gcc -Wall -fPIC -c CollatzLibrary.c
	gcc -Wall -shared CollatzLibrary.o -o libCollatzLibrary.so 
	gcc -Wall main.c -o main_dynamic_loaded -ldl

clean:
	rm CollatzLibrary.o libCollatzLibrary.so main_dynamic_loaded