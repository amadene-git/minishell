NAME	=	minishell


SRCS	=	srcs/main.c\
			srcs/parser.c\
			srcs/built_in.c\
			srcs/built_in_export.c\
			srcs/exec.c\
			srcs/libft_utils.c\


OBJS	=	${SRCS:.c=.o }

CFLAGS	=	-Wall -Wextra -Werror

CC		=	gcc

RM		=	rm -rf

LIBFT	=	~/Documents/42-cursus/Libft/libft.a 


INC		=	includes/

.c.o 	:
			${CC} -I ~/Documents/42-cursus/Libft -I includes -c $< -o ${<:.c=.o}


all		:	${NAME}

$(NAME)	: 	${OBJS}
			${CC} -o ${NAME} ${OBJS} -g ${LIBFT}

clean	:
			${RM} ${OBJS}

fclean	:	clean
			${RM} ${NAME}

re	:	fclean all

.PHONY	:	all clean fclean re

