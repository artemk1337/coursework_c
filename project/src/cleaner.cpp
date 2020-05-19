#include "lemin.h"

void			clean_tmp(t_tmp **tmp)
{
	t_tmp	*temp_ptr;

	while (*tmp)
	{
		temp_ptr = (*tmp)->next;
		free(*tmp);
		*tmp = temp_ptr;
	}
}

void			reset_struct(t_tmp *list)
{
	t_tmp *tmp;

	tmp = list;
	while (tmp)
	{
		tmp->room->prev = NULL;
		tmp->room->prev1 = NULL;
        if (tmp->room != LeminObj->start)
		    tmp->room->min_w = INT_MAX / 2;
		tmp = tmp->next;
	}
}
