# Makefile for lab 4 task 6
bin/program: obj/client.o obj/server.o
	gcc -o bin/program obj/client.o obj/server.o -l sqlite3
obj/client.o: src/client.c
	gcc -c src/client.c -o obj/client.o -l sqlite3
obj/server.o: src/server.c
	gcc -c src/server.c -o obj/server.o -l sqlite3
clean:
	rm -rf obj/*.o bin/program
check:
  shell: /usr/bin/bash -e {0}
distcheck:
  shell: /usr/bin/bash -e {0}
