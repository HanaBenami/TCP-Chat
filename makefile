obj_files := chat_server.out chat_client.out
headers =  $(wildcard *.hpp)
port = 7777

include = -I../utils/ 
warnings = -pedantic-errors -Wall -Wextra
compile_cpp = g++ -std=c++11 $(warnings) $(include)
compile_c = gcc -ansi $(warnings)
vlg = valgrind --leak-check=yes --track-origins=yes 
# colors
title = \033[2;34m\033[47m
not_important = \033[90m
no_color = \033[0m

all: | clean $(obj_files)

server: chat_server.out 
	@echo "\n$(title) *** RUN SERVER *** $(no_color)"
#	sudo fuser -kn tcp 7777
#	netstat  | grep 7777
	$(vlg) ./chat_server.out

client: chat_client.out
	@echo "\n$(title) *** RUN CLIENT *** $(no_color)"
	$(vlg) ./chat_client.out

%.out: %.cpp $(headers)
	@echo "\n$(title) *** $@ *** $(no_color)"
	$(compile_cpp) $< -g -o $@ -pthread

clean:
	@echo "\n$(title) *** CLEAN *** $(no_color)"
	rm -f *.out
	rm -f *.o
	rm -f *core*