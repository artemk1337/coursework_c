#include "lemin.h"
#include "init_class.h"

Lemin *LeminObj;

t_room	*InitClass::init_room(char *s)
{
	t_room	*room = new t_room();
	room->min_w = INT_MAX / 2;
	room->name = strdup(s);
	return (room);
}

t_tmp	*InitClass::init_tmp(t_tmp *tmp, int a, char *s)
{
	t_tmp	*start;
	int		i;

	if (!(i = 0) && !tmp)
	{
		if (!(tmp = (t_tmp *)malloc(sizeof(t_tmp))))
			exit(1);
		start = tmp;
	}
	else
	{
		start = tmp;
		while (tmp->next)
			tmp = tmp->next;
		if (!(tmp->next = (t_tmp *)malloc(sizeof(t_tmp))))
			exit(1);
		tmp = tmp->next;
	}
	tmp->next = NULL;
	while (s[i] && s[i] != ' ')
		i++;
	s[i] = '\0';
	tmp->room = init_room(s);
	if (a == 0 && !(tmp->room->min_w = 0))
	{
		if (!LeminObj->start)
			LeminObj->start = tmp->room;
		else
			error_exit();
	}
	else if (a == -1)
	{
		if (!LeminObj->finish)
			LeminObj->finish = tmp->room;
		else
			error_exit();
	}
	return (start);
}

void		InitClass::init_class_lemin(void)
{
	LeminObj = new Lemin();
}
