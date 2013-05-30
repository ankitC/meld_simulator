CC = /usr/bin/gcc
CFLAGS =  -lpthread
FILES = blocks meld_sim 
blocks: csapp.c client_vm.c meld_server.c
	$(CC)  csapp.c client_vm.c -o blocks $(CFLAGS)
	$(CC)  csapp.c meld_server.c -o meld_sim $(CFLAGS)

# Clean up
clean:
	rm -f $(FILES) *.o *~
