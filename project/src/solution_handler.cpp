#include "lemin.h"
#include "solution_handler_class.h"

extern Lemin *LeminObj;


struct s_bandwidth	*SolutionHandlerClass::count_solution_bandwidth(t_solution *solution)
{
	int					prev_path_len;
	struct s_bandwidth *bandwidth;

	bandwidth = new struct s_bandwidth();
	bandwidth->bandwidth = 1;
	prev_path_len = 0;
	while (solution)
	{
		if (solution->hide)
		{
			solution = solution->next;
			continue;
		}
		if (bandwidth->path_num)
			bandwidth->bandwidth += bandwidth->path_num * (solution->path_len -
					prev_path_len) + 1;
		prev_path_len = solution->path_len;
		bandwidth->path_num++;
		bandwidth->max_len = solution->path_len > bandwidth->max_len ?
				solution->path_len : bandwidth->max_len;
		solution = solution->next;
	}
	return (bandwidth);
}

/*
** Function check_solutions
** ------------------------
** 	prev_solution: solution from previous step
**	current_solution: solution from current step
**
**	return 0 if current solution better, else 1
*/
int		SolutionHandlerClass::check_solutions(t_solution **prev_solution, t_solution **current_solution)
{
	struct s_bandwidth	*prev_bandwidth;
	struct s_bandwidth	*curr_bandwidth;
	unsigned int		counter;

	if (!prev_solution)
		return (0);
	sort_solutions(prev_solution);
	sort_solutions(current_solution);
	prev_bandwidth = count_solution_bandwidth(*prev_solution);
	if (prev_bandwidth->bandwidth >= LeminObj->count)
		return (1);
	curr_bandwidth = count_solution_bandwidth(*current_solution);
	counter = curr_bandwidth->max_len - prev_bandwidth->max_len;
	while (counter--)
	{
		prev_bandwidth->bandwidth += prev_bandwidth->path_num;
		if (prev_bandwidth->bandwidth > LeminObj->count)
			return (1);
	}
	delete (prev_bandwidth);
	delete (curr_bandwidth);
	return (0);
}

t_solution *SolutionHandlerClass::copy_solution(t_solution *source_solutions)
{
	t_solution		*solution_iter;
	unsigned int	room_iterator;
	t_solution		*new_solution;

	if (!LeminObj->solution)
		return (NULL);
	new_solution = new t_solution();
	solution_iter = new_solution;
	while (source_solutions)
	{
		room_iterator = -1;
		solution_iter->arr = (t_room **)malloc(sizeof(t_room *) *
				(source_solutions->path_len + 1));
		solution_iter->path_len = source_solutions->path_len;
		while (++room_iterator < source_solutions->path_len)
			solution_iter->arr[room_iterator] =
					source_solutions->arr[room_iterator];
		solution_iter->arr[room_iterator] = NULL;
		if (source_solutions->next)
			solution_iter->next = new t_solution();
		else
			solution_iter->next = NULL;
		solution_iter = solution_iter->next;
		source_solutions = source_solutions->next;
	}
	return (new_solution);
}

void	SolutionHandlerClass::destroy_solutions(t_solution **solution)
{
	t_solution *temp;

	while (*solution)
	{
		free((*solution)->arr);
		temp = (*solution)->next;
		delete (*solution);
		*solution = temp;
	}
}

void	SolutionHandlerClass::remove_sol(t_solution *sol)
{
	t_solution	*t_s;
	t_solution	*t_s_pr;

	t_s_pr = NULL;
	t_s = LeminObj->solution;
	while (t_s != sol)
	{
		t_s_pr = t_s;
		t_s = t_s->next;
	}
	if (!t_s_pr)
		LeminObj->solution = t_s->next;
	else
		t_s_pr->next = t_s->next;
	free(sol->arr);
	delete(sol);
}

void		SolutionHandlerClass::init_sol_dop(t_solution *tmp_s, int size, t_solution *sol)
{
	int		i;
	t_next	*neigh;
    t_room  *room;

	i = 1;
    room = LeminObj->finish;
	while (room != LeminObj->start)
	{
        if (room != LeminObj->finish && !room->path)
        {
            room->path = tmp_s->arr;
            room->idx = size - i;
        }
		neigh = room->prev->next;
		while (neigh->room != room)
			neigh = neigh->next;
		tmp_s->arr[size - i] = neigh->room;
		room = room->prev;
		i++;
	}
	tmp_s->hide = 0;
	tmp_s->arr[size - i] = LeminObj->start;
	tmp_s->arr[size - i]->idx = size - i;
	LeminObj->solution = sol;
}

void			SolutionHandlerClass::init_sol(t_solution *sol, int size)
{
	t_solution	*tmp_s;

	if (!(sol))
	{
		if (!(sol = (t_solution *)ft_memalloc(sizeof(t_solution))))
			exit(1);
		tmp_s = sol;
	}
	else
	{
		tmp_s = sol;
		while (tmp_s->next)
			tmp_s = tmp_s->next;
		if (!(tmp_s->next = (t_solution *)ft_memalloc(sizeof(t_solution))))
			exit(1);
		tmp_s = tmp_s->next;
	}
	if (!(tmp_s->arr = (t_room **)malloc(sizeof(t_room *) * (size + 1))))
		exit(1);
	tmp_s->arr[size] = NULL;
	tmp_s->path_len = size;
	init_sol_dop(tmp_s, size, sol);
}

int				SolutionHandlerClass::save_tmp(void)
{
	t_room	*room;
	t_next	*neigh;
	t_next	*neigh_tmp;
	int		tmp_rooms;

	room = LeminObj->finish;
	tmp_rooms = 1;
	while (room != LeminObj->start)
	{
        if (!room)
            return (0);
		tmp_rooms++;
		neigh = room->prev->next;
		while (neigh->room != room)
			neigh = neigh->next;
		neigh->toggle = 0;
		// Change weight
		neigh_tmp = room->next;
		while (neigh_tmp->room != room->prev)
			neigh_tmp = neigh_tmp->next;
		neigh_tmp->weight = -1;
		room = room->prev;
	}
	init_sol(LeminObj->solution, tmp_rooms);
    return (1);
}

int				SolutionHandlerClass::len_sol(t_solution *solution)
{
	t_solution	*begin;
	int			i;

	begin = solution;
	i = 0;
	while (begin)
	{
		i++;
		begin = begin->next;
	}
	return (i);
}

void			SolutionHandlerClass::sort_solutions(t_solution **solution)
{
	t_solution	*prev;
	t_solution	*curr;
	t_solution	*start;
	int			i;
	int			max_i;

	if (!*solution || !(*solution)->next)
		return ;
	start = (*solution);
	curr = start;
	i = 0;
	max_i = len_sol(curr);
	while (i < max_i)
	{
		prev = NULL;
		curr = start;
		while (curr && curr->next)
		{
			if (curr->path_len > curr->next->path_len)
			{
				if (!prev)
				{
					start = curr->next;
					curr->next = start->next;
					start->next = curr;
				}
				else
				{
					prev->next = curr->next;
					curr->next = prev->next->next;
					prev->next->next = curr;
				}
			}
			prev = curr;
			curr = curr->next;
		}
		i++;
	}
	*solution = start;
}




























void	SolutionHandlerClass::print_sol(void)
{
	t_solution	*sol;
	int			i;

	if (!LeminObj->solution)
	{
		std::cout << ("NO SOLUTION\n");
		return ;	
	}
	std::cout << ("Ways:\n\n");
	sol = LeminObj->solution;
	while (sol)
	{
		i = 0;
		while (sol->arr[i])
		{
			std::cout << (sol->arr[i]->name);
			if (sol->arr[i + 1])
				std::cout << (" -> ");
			i++;
		}
		std::cout << ('\n');
		sol = sol->next;
	}
	std::cout << ('\n');
}
