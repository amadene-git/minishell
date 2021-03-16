# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mbouzaie <mbouzaie@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/01/27 13:25:25 by mbouzaie          #+#    #+#              #
#    Updated: 2021/03/16 19:29:40 by mbouzaie         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

MINISHELL_H		= -I includes/
LIBFT_H			= -I $(LIB_DIR)includes/
LIBFT_A			= libft.a

OBJ_DIR			= objs/
SRC_DIR			= srcs/
LIB_DIR			= srcs/libft/
LIBS_DIR		= -L. -L$(LIB_DIR)


SRCS	=		main.c\
				parser.c\
				built_in_echo.c\
				built_in_cd.c\
				built_in_pwd.c\
				built_in_export.c\
				built_in_env.c\
				built_in_exit.c\
				dlist.c\
				dlist_suite.c\
				exec_bin.c\
				exec_builtin.c\
				utils.c\
				redirect.c\
				token.c\
				cmd.c\
				errors.c\
				absolute_path.c\
				lexer.c\

CFIND			= $(SRCS:%=$(SRC_DIR)%)
OFILE			= $(SRCS:%.c=%.o)
OBJS			= $(addprefix $(OBJ_DIR), $(OFILE))



CC				= gcc
RM				= rm -f
CFLAGS			= -Wall -Wextra -Werror -g -o
				 
LIBS			= $(CUB3D_H) $(LIBFT_H)

NAME			= libshell.a
EX_NAME			= minishell


all:			$(OBJ_DIR) $(EX_NAME)

$(OBJ_DIR):
				@mkdir -p $(OBJ_DIR)

$(EX_NAME) : $(NAME)
				@$(CC) $(CFLAGS) $(EX_NAME) $(LIBS_DIR) -lshell $(LIBS)
				@rm $(NAME)

$(NAME):		$(OBJS)
				@make -C $(LIB_DIR) bonus
				@cp $(LIB_DIR)$(LIBFT_A) .
				@mv $(LIBFT_A) $(NAME)
				@ar rc $(NAME) $(addprefix $(OBJ_DIR), $(OFILE))
				@ranlib $(NAME)
				@echo minishell is ready!


$(OBJS): $(CFIND)
			@make $(OFILE)

$(OFILE):
			@$(CC) $(LIBS) -c $(CFLAGS) $(OBJ_DIR)$@ $(SRC_DIR)$(@:%.o=%.c)

clean:
				@/bin/rm -rf $(OBJ_DIR)
				@make -C $(LIB_DIR) clean
				@echo object files cleaned

fclean:			clean
				$(RM) $(NAME) $(EX_NAME)
				@make -C $(LIB_DIR) fclean
				@echo cleaned $(EX_NAME) and object files

re:				fclean all

.PHONY:			all clean fclean re