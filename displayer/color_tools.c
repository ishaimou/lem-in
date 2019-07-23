#include "displayer.h"

SDL_Color		color_macros(int macros)
{
	if (macros == L_YELLOW)
		return (setcolor_sdl(255, 255, 0, 1));
	if (macros == L_WHITE)
		return (setcolor_sdl(255, 255, 255, 1));
	if (macros == L_BLACK)
		return (setcolor_sdl(0, 0, 0, 1));
	if (macros == L_GREEN)
		return (setcolor_sdl(0, 255, 0, 1));
	if (macros == L_RED)
		return (setcolor_sdl(255, 0, 0, 1));
	if (macros == L_BLUE)
		return (setcolor_sdl(0, 0, 255, 1));
	if (macros == L_ORANGE)
		return (setcolor_sdl(255, 70, 0, 1));
	if (macros == L_CYAN)
		return (setcolor_sdl(0, 255, 255, 1));
	if (macros == L_MAGENTA)
		return (setcolor_sdl(255, 0, 255, 1));
	return (setcolor_sdl(0, 0, 0, 1));
}

void		name_color(int macros)
{
	char	*str;

	if (macros == L_YELLOW)
		ft_printf("   color: %{yellow}Yellow%{eoc}\n");
	else if (macros == L_WHITE)
		ft_printf("   color: White\n");
	else if (macros == L_BLACK)
		ft_printf("   color: Black\n");
	else if (macros == L_GREEN)
		ft_printf("   color: %{green}Green%{eoc}\n");
	else if (macros == L_RED)
		ft_printf("   color: %{red}Red%{eoc}\n");
	else if (macros == L_BLUE)
		ft_printf("   color: %{blue}Blue%{eoc}\n");
	else if (macros == L_ORANGE)
		ft_printf("   color: Orange\n");
	else if (macros == L_CYAN)
		ft_printf("   color: %{cyan}Cyan%{eoc}\n");
	else if (macros == L_MAGENTA)
		ft_printf("   color: %{purple}Magenta%{eoc}\n");
	else
		ft_printf("   color: White\n");
}
