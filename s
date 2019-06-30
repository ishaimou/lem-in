make re -C ./libft && make clean -C ./libft
gcc -g *.c -L ./libft -lft -I ./libft/includes/ -o lemin
