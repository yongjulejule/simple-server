######################### Color #########################
GREEN="\033[32m"
L_GREEN="\033[1;32m"
RED="\033[31m"
L_RED="\033[1;31m"
RESET="\033[0m"
BOLD="\033[1m"
L_PUPLE="\033[1;35m"
L_CYAN="\033[1;96m"
UP = "\033[A"
CUT = "\033[K"

########################## Rule ##########################

.PHONY		:	all
all				:
						@make -C ./server
						@make -C ./client

.PHONY			:	clean
clean			:
						@make -C ./server clean 
						@make -C ./client clean


.PHONY			:	fclean
fclean			:	clean
						@make -C ./server fclean 
						@make -C ./client fclean

.PHONY			:	re
re				:	fclean all

.PHONY			:	debug
debug			: 
						@make -C ./server debug 
						@make -C ./client debug

.PHONY			:	leaks
leaks			:
						@make -C ./server leaks 
						@make -C ./client leaks
