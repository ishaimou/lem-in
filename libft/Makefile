NAME = libft.a

LIBFT  = ft_memset ft_bzero ft_memcpy ft_memccpy ft_memmove ft_memchr ft_printhex\
		 ft_memcmp ft_memalloc ft_memdel ft_strnew ft_strdel ft_strclr ft_putnbr\
		 ft_striter ft_striteri ft_strmap ft_strmapi ft_strequ ft_strnequ ft_utoa\
		 ft_strsub ft_strjoin ft_strtrim ft_strsplit ft_itoa ft_itoa_base ft_trimstr\
		 ft_utoa_base ft_putchar ft_putstr ft_putendl get_next_line copy_char2\
		 ft_putchar_fd ft_putstr_fd ft_putendl_fd ft_putnbr_fd ft_strlen len_tab\
		 ft_strcpy ft_strncpy ft_strcat ft_strncat ft_strlcat ft_strstr ft_strcombin\
		 ft_strnstr ft_strcmp ft_strncmp ft_strchr ft_strrchr ft_strpbrk free_tabstr\
		 ft_strdup ft_strndup ft_atoi ft_isalpha ft_isdigit ft_isalnum ft_intlen\
		 ft_isascii ft_isprint ft_toupper ft_tolower add_2_tab ft_prime ft_ultoa\
		 ft_strlowcase ft_strupcase ft_swap ft_strrev ft_isblank ft_isxdigit ft_sqrt\
		 ft_strnjoin ft_atoi_base ft_strswap ft_strsum ft_str_pushback ft_ulltoa\
		 ft_lltoa ft_ulltoa_base ft_strpower ft_strmult ft_strcnew ft_ltoa ft_maxmin\
		 ft_poslltoa copy_2_char ft_epurstr join_from_tab add_2_tab tabint_reset\
		 hash_str hash_filltab hash_findid hash_strtoind hash_putintab hash_freetab\
		 mxint_free\

LIBFT_SRC = $(addprefix srcs/, $(addsuffix .c, $(LIBFT)))
LIBFT_OBJ = $(addsuffix .o, $(LIBFT))



LIB_STRUCTS = qt_enqueue qt_dequeue qt_isempty qt_free qt_front qt_print qt_new_queue\
			  bt_apply_infix bt_apply_revinfix bt_apply_prefix bt_apply_suffix bt_free\
			  bt_create_node bt_insert_item bt_search_item bt_size_count bt_level_count\
		 	  ft_lstnew ft_lstnew_sm ft_lstdelone ft_lstdel ft_lstadd ft_lstiter\
			  ft_lstmap ft_lstprint ft_list_size ft_lstadd_last\
			  sk_push sk_pop sk_top sk_free sk_isempty ic_cmp ic_ncmp ic_copy ic_lstfind\
			  ic_addnode ic_pushnode ic_joinstr ic_free ic_print ic_lstdel ic_size\
			  chr_init chr_free chr_pushfront chr_addnode chr_revprint chr_print\
			  chr_len gnl_save_chr gnl_nsave_chr\
			  pt_new pt_swap pt_linterp pt_rot pt_diffteta\

LIB_STRUCTS_SRC = $(addprefix srcs/data_structs/, $(addsuffix .c, $(LIB_STRUCTS)))
LIB_STRUCTS_OBJ = $(addsuffix .o, $(LIB_STRUCTS))



LIB_FT_PRINTF = ft_printf check parse tools tools2 tools3 date noprint cast\
			flags flags2 engine convers_c convers_s precalc put_chr\
		   	convers_efgh convers_ee convers_xb convers_di colinvpourcent\
			ssnprintf convers_f vddnprintf convers_o convers_u convers_di_width\
			convers_u_flag_prec convers_p getcalcul_dld convers_lf gets\
			convers_ee_tools str_tools custom_expf int_expmantis conv_f_tools\
			convers_o_tools convers_xb_tools check_fill fill_tools

LIB_FT_PRINTF_SRC = $(addprefix srcs/printf/, $(addsuffix .c, $(LIB_FT_PRINTF)))
LIB_FT_PRINTF_OBJ = $(addsuffix .o, $(LIB_FT_PRINTF))



FLAGS = -Wall -Wextra -Werror

all : $(NAME)

$(NAME) :
		@gcc -c  $(FLAGS) -I ./includes $(LIB_FT_PRINTF_SRC) $(LIB_STRUCTS_SRC) $(LIBFT_SRC)
		@ar rc $(NAME) $(LIBFT_OBJ) $(LIB_FT_PRINTF_OBJ) $(LIB_STRUCTS_OBJ)
		@ranlib	$(NAME)

clean :
		@/bin/rm -rf $(LIB_FT_PRINTF_OBJ) $(LIBFT_OBJ) $(LIB_STRUCTS_OBJ)

fclean : clean
		@/bin/rm -rf $(NAME)

re : fclean all

.PHONY : all clean fclean re
