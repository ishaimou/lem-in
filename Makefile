
# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ishaimou <ishaimou@student.1337.ma>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/07/02 07:12:48 by ishaimou          #+#    #+#              #
#    Updated: 2019/07/02 08:32:12 by ishaimou         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

BGREEN = \033[1;32m
BCYAN = \033[1;36m
RESET = \033[0m
BEER = \xf0\x9f\x8d\xba

NAME = lem-in
LIB_NAME = libft.a
LIB = -L ./libft -lft
LIB_DIR = ./libft
H_LIB_DIR = ./libft/includes
H_DIR = ./includes
FLAGS = -Wall -Werror -Wextra

SRCS = 	main.c init.c tools.c hash_tools.c free.c\
		bfs.c algo_ishobe.c best_group.c manager.c \
		deleteme.c

SRCS_DIR = $(addprefix srcs/, $(SRCS))

all: $(NAME)

$(NAME):
	@make -C $(LIB_DIR)
	@echo "$(BGREEN)$(LIB_NAME) has been created successfully.$(RESET)"
	@gcc -o $(NAME) $(SRCS_DIR) -I $(H_DIR) -I $(H_LIB_DIR) $(LIB) $(FLAGS)
	@echo "$(BEER)  $(BGREEN)$(NAME) has been created successfully.$(RESET)"

clean:
	@make clean -C $(LIB_DIR)
	@echo "$(BCYAN)Object files has been cleaned successfully.$(RESET)"

fclean: clean
	@make fclean -C $(LIB_DIR)
	@rm -rf $(NAME)
	@echo "$(BCYAN)$(NAME) cleaned successfully.$(RESET)"

re: fclean all
