# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ishaimou <ishaimou@student.1337.ma>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/07/02 07:12:48 by ishaimou          #+#    #+#              #
#    Updated: 2019/07/26 01:12:49 by obelouch         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

BGREEN = \033[1;32m
BCYAN = \033[1;36m
RESET = \033[0m
BEER = 🍻
BOOKS = 📚i
CLEAN = ✅
FCLEAN = 😥

NAME = lem-in
LIB_NAME = libft.a
LIB = -L ./libft -lft
LIB_DIR = ./libft
H_LIB_DIR = ./libft/includes
H_DIR = ./includes
FLAGS = -Wall -Werror -Wextra

SRCS = 	main.c init.c tools.c hash_tools.c free.c manager.c\
		extended_bfs.c fill_bfs.c bfs.c algo_ishobe.c best_group.c\
		deleteme.c

SRCS_DIR = $(addprefix srcs/, $(SRCS))

all: $(NAME)

$(NAME):
	@make -C $(LIB_DIR)
	@echo "$(BOOKS) $(BGREEN)$(LIB_NAME) has been created successfully.$(RESET)"
	@gcc -o $(NAME) $(SRCS_DIR) -I $(H_DIR) -I $(H_LIB_DIR) $(LIB) $(FLAGS)
	@echo "$(BEER)  $(BGREEN)$(NAME) has been created successfully.$(RESET)"
	@echo ""
	@echo ""
	@echo "$(BGREEN)"
	@echo "              ,"
	@echo '     _,-`\   /|   .    .    /`.'
	@echo ' _,-`     \_/_|_  |\   |`. /   `._,--===--.__'
	@echo '        _/"/  " \ : \__|_ /.   ,"    :.  :. .`-._'
	@echo '       // ^   /7 t`""    "`-._/ ,"\   :   :  :  .`.'
	@echo '       Y      L/ )\         ]],`   \  :   :  :   : `.'
	@echo '       |        /  `.n_n_n,","\_    \ ;   ;  ;   ;  _>'
	@echo '       |__    ,`     |  \`-`    `-.__\_______.==---"'
	@echo '      //  `""\\      |   \            \'
	@echo '      \|     |/      /    \            \'
	@echo '                    /     |             `.'
	@echo '                   /      |               ^'
	@echo '                  ^       |'
	@echo "                          ^   $(BCYAN) ISHOBE$(RESET) Take care of ants"
	@echo "$(RESET)"

clean:
	@make clean -C $(LIB_DIR)
	@echo "$(CLEAN)  $(BCYAN)Object files has been cleaned successfully.$(RESET)"

fclean: clean
	@make fclean -C $(LIB_DIR)
	@rm -rf $(NAME)
	@echo "$(FCLEAN)  $(BCYAN)$(NAME) cleaned successfully.$(RESET)"

re: fclean all
