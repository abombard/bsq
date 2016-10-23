#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>

typedef struct	s_tab
{
	int8_t *tab;
	int len;
	int line;
	int col;
	int8_t v;
	int8_t o;
	int8_t p;
	int pass;
}				t_tab;

int ft_strlen(char *s)
{
	int x;

	x = 0;
	while (s[x])
		++x;
	return (x);
}

void ft_putchar(char c)
{
	write(1, &c, 1);
}

void ft_putnbr(int n)
{
	if (n < 0)
	{
		write(1, "-", 1);
		n = -n;
	}
	if (n > 9)
		ft_putnbr(n / 10);
	ft_putchar(n % 10 + '0'); 
}

void ft_putstr(char *s)
{
	if (!s)
		write(1, "(Null)", 6);
	else
		write(1, s, ft_strlen(s));
}

t_tab *get_file(char *s)
{
	t_tab *ret;
	int fd;

	ret = malloc(sizeof(t_tab));
	if (!ret)
		return (NULL);
	fd = open(s, O_RDONLY);
	if (fd == -1)
		return (NULL);
	ret->len = lseek(fd, 0, SEEK_END);
	lseek(fd, 0, SEEK_SET);
	ret->tab = malloc(sizeof(int8_t) * ret->len);
	if (!ret->tab)
		return (NULL);
	read(fd, ret->tab, ret->len);
	ret->tab[ret->len] = 0;
	ret->line = atoi(ret->tab);
	ret->col = (ret->len - 4) / ret->line;
	int i = 0;
	while (isdigit(ret->tab[i]))
		++i;
	ret->v = ret->tab[i];
	ret->o = ret->tab[i + 1];
	ret->p = ret->tab[i + 2];
	ret->pass = i + 4;
	return (ret);
}

void around(t_tab *s, int8_t *t, int i)
{
	int ret;

	if (s->tab[i] == s->o)
		t[i] = 0;
	else if (s->tab[i - 1] == s->o || s->tab[i - s->col] == s->o
			 || s->tab[i - s->col - 1] == s->o)
		t[i] = 1;
	else if (t[i - 1] == t[i - s->col - 1]
			 && t[i - 1] == t[i - s->col])
		t[i] = t[i - 1] + 1;
	else if (t[i - 1] >= t[i - s->col - 1]
			 && t[i - s->col] >= t[i - s->col - 1])
		t[i] = t[i - 1] > t[i - s->col] ? t[i - 1] : t[i - s->col];
	else
		t[i] = t[i - 1] > t[i - s->col] ? t[i - s->col] + 1: t[i - 1] + 1;
}

void solve(t_tab *s)
{
	int8_t *tab;
	int i, j;
	int ret;

	tab = malloc(sizeof(int8_t) * s->len);
	if (!tab)
		return ft_putstr("malloc failed");
	ret = 0;
	i = s->pass - 1;
	while (s->tab[++i])
	{
		if (s->tab[i] == '\n') {
			tab[i] = 0;
			continue ;
		}
		if (i > s->col + 4)
			around(s, tab, i);
		else
			tab[i] = s->tab[i] == s->v ? 1 : 0;
		if (tab[i] > ret)
			ret = tab[i];
	}

	int mem;
	i = s->pass;
	while (tab[i] != ret)
		++i;
	mem = i;
	i -= (ret - 1) * s->col + ret - 1;
	while (i < s->len)
	{
		j = 0;
		while (j < ret)
		{
			s->tab[i] = s->p;
			++i, ++j;
		}
		if (i >= mem)
			break;
		i += s->col - ret;
	}
	free(tab);
}

int main(int ac, char **av)
{
	t_tab *s;

	if (ac == 2)
	{
		s = get_file(av[1]);
		if (!s)
			return (-1);
		solve(s);
		if (s->tab)
		{
			ft_putstr(s->tab + s->pass);
			free(s->tab);
		}
		free(s);
	}
	ft_putstr("\n");
	return (0);
}
