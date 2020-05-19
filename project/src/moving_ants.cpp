#include "lemin.h"
#include "moving_ants_class.h"

int			MovingAntsClass::check_ant(t_solution *sol, int len_way)
{
	t_solution		*sol_tmp;
	int				i;
	unsigned long	delta;

	sol_tmp = LeminObj->solution;
	delta = 0;
	i = 0;
	while (sol_tmp != sol)
	{
		delta += len_way - sol_tmp->path_len;
		sol_tmp = sol_tmp->next;
		i++;
	}
	if (delta <= LeminObj->start->ant)
		return (1);
	return (0);
}

// std::cout работает слишком долго! использую write
void		MovingAntsClass::print_ant(int c, char *s)
{
	ft_putchar('L');
	ft_putnbr(c);
	ft_putchar('-');
	ft_putstr(s);
	ft_putchar(' ');
}

void		MovingAntsClass::move_ant(t_solution *sol, int len_way)
{
	t_room	*curr_r;
	t_room	*pr_r;
	int		len_cpy;

	len_cpy = len_way;
	len_way--;
	while (len_way > 0)
	{
		if (len_way == 1 && !check_ant(sol, len_cpy))
			break ;
		curr_r = sol->arr[len_way];
		pr_r = sol->arr[len_way - 1];
		if (pr_r->ant)
		{
			curr_r->ant += 1;
			pr_r->ant -= 1;
			if (len_way > 1)
				curr_r->number_anc = pr_r->number_anc;
			else
				curr_r->number_anc = LeminObj->count - pr_r->ant;
			pr_r->number_anc = 0;
			print_ant(curr_r->number_anc, curr_r->name);
		}
		len_way--;
	}
}

void		MovingAntsClass::algorithm(void)
{
	t_solution	*sol;
	int			i;
	int			step;

	LeminObj->start->ant = LeminObj->count;
	LeminObj->finish->ant = 0;
	step = 1;
	while (LeminObj->finish->ant != LeminObj->count)
	{
		i = 0;
		sol = LeminObj->solution;
		while (sol)
		{
			move_ant(sol, sol->path_len);
			sol = sol->next;
			i++;
		}
		std::cout << ("\t| step - ") << (step++) << ("\n");
	}
}
