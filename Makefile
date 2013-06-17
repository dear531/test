src	:=$(wildcard $(spath)*.c)
obj	:=$(addprefix $(opath),$(notdir $(src:.c=.o)))

CFLAGS	:=-Wall -g -c
CC	:=cc

all:server client
server:sniffer_server.o 
	$(CC) $^ -o $@ 
client:sniffer_client.o
	$(CC) $^ -o $@
$(obj):%.o:%.c 
	$(CC) $(CFLAGS) $< -o $@
clean:
	-rm -rf $(obj) .*.swp  a.out
rmexe:
	-rm -rf server client

.PHONY:
	all clean rmexe
