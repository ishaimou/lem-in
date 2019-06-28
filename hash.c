/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obelouch <OB-96@hotmail.com>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/28 03:34:30 by obelouch          #+#    #+#             */
/*   Updated: 2019/06/28 05:04:29 by obelouch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE	10

unsigned long		hash_str(char *str)
{
	unsigned long	hash;
	unsigned long	power;
	int				p;
	int				i;

	i = 0;
	p = 2;
	hash = 0;
	power = 1;
	while (str[i])
	{
		hash += (str[i] - 31) * power;
		power *= p;
		i++;
	}
	return (hash);
}

int					fill_hashtab(char **hash_tab, int size, char *str)
{
	unsigned long	ind;
	unsigned long	i;

	ind = hash_str(str) % size;
	i = ind;
	while (i < size)
	{
		if (!hash_tab[i])
		{
			hash_tab[i] = strdup(str);			//!!!!!!!!!!!!!!!!!!!!
			return (i);
		}
		i++;
	}
	i = 0;
	while (i < ind)
	{
		if (!hash_tab[i])
		{
			hash_tab[i] = strdup(str);			//!!!!!!!!!!!!!!!!!!!!!
			return (i);
		}
		i++;
	}
	return (-1);
}

int					main(int ac, char **av)
{
	char			**tab;
	int				i;

	if (ac == SIZE + 1)
	{
		tab = (char**)malloc(sizeof(char*) * (SIZE + 1));
		i = -1;
		while (++i < SIZE)
			tab[i] = NULL;
		i = -1;
		while (++i < SIZE)
			fill_hashtab(tab, SIZE, av[i + 1]);
		i = -1;
		while (++i < SIZE)
			printf("tab[%d]: %s\t| hash = %lu\n", i, tab[i], hash_str(tab[i]));
		//if (ac == 2)
		//	printf("hash of %s: %lu\n", av[1], hash_str(av[1]));
		i = -1;
		while (++i < SIZE)
			free(tab[i]);
	}
	return (0);
}
