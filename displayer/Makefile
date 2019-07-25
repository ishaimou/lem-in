# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ishaimou <ishaimou@student.1337.ma>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/07/22 23:50:43 by ishaimou          #+#    #+#              #
#    Updated: 2019/07/25 10:09:44 by ishaimou         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

BGREEN = \033[1;32m
BCYAN = \033[1;36m
RESET = \033[0m

NAME = displayer

LIB_NAME = libft.a

LIB_DIR = ../libft

LIBFT = -L ../libft -lft

LIBSDL = -L ./libsdl -lsdl

LIB_SDL2 = -L ~/.brew/Cellar/sdl2/2.0.9_1/lib/ -lSDL2

LIB_SDL2_IMG = -L ~/.brew/Cellar/sdl2_image/2.0.4/lib/ -lSDL2_image

LIB_SDL2_TTF = -L ~/.brew/Cellar/sdl2_ttf/2.0.14/lib/ -lSDL2_ttf

LIB_SDL2_MIXER = -L ~/.brew/Cellar/sdl2_mixer/2.0.4/lib -lSDL2_mixer

H_LIBFT = ../libft/includes

H_DIR = ./includes

H_LIBSDL = ./libsdl

FLAGS = -Wall -Werror -Wextra

SRCS = alloc_places.c take_cmds.c main.c tools.c ant_man.c color_tools.c\
	   draw_rooms.c event.c free.c init.c print_debug.c print_life_ants.c\
	   hash_tools.c deleteme.c

SRCS_DIR = $(addprefix srcs/, $(SRCS))

all: $(NAME)

$(NAME):
	@make -C $(LIB_DIR)
	@echo "$(BGREEN)$(LIB_NAME) has been created successfully.$(RESET)"
	@gcc -o $(NAME) $(SRCS_DIR) -I $(H_LIBFT) -I $(H_DIR) -I $(H_LIBSDL) $(LIBFT) $(LIBSDL)\
		$(LIB_SDL2) $(LIB_SDL2_IMG) $(LIB_SDL2_TTF) $(LIB_SDL2_MIXER) $(FLAGS)
	@echo "$(BGREEN)$(NAME) has been created successfully.$(RESET)"

clean:
	@make clean -C $(LIB_DIR)
	@echo "$(BCYAN)Object files has been cleaned successfully.$(RESET)"

fclean: clean
	@make fclean -C $(LIB_DIR)
	@rm -rf $(NAME)
	@echo "$(BCYAN)$(NAME) cleaned successfully.$(RESET)"

re: fclean all
