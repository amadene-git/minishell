NAME		= minishell

CC		= gcc -Wall -Wextra -Werror

SRCS		= srcs/main.c




all		: 
			${CC} ${SRCS}

clean		:
		

fclean		:	clean 
				rm ${NAME}
