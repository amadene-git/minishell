NAME	=	minishell


SRCS	=	srcs/main.c\
			srcs/parser.c\
			srcs/built_in.c\
			srcs/built_in_export.c\
			srcs/exec.c\
			srcs/libft_utils.c\
			srcs/get_next_line.c\
			srcs/libft_utils_suite.c\


OBJS	=	${SRCS:.c=.o }

CFLAGS	=	-Wall -Wextra -Werror

CC		=	gcc

RM		=	rm -rf

INC		=	includes/

.c.o 	:
			${CC} -I includes -c $< -o ${<:.c=.o}


all		:	${NAME}

$(NAME)	: 	${OBJS}
			${CC} -o ${NAME} ${OBJS} -g

clean	:
			${RM} ${OBJS}

fclean	:	clean
			${RM} ${NAME}

re	:	fclean all

.PHONY	:	all clean fclean re

