#ifndef INIT_CLASS_H
# define INIT_CLASS_H

# include "lemin.h"


class InitClass
{
	public:
	t_room		*init_room(char *s);
	void		init_class_lemin(void);
	t_tmp		*init_tmp(t_tmp *tmp, int a, char *s);
};

#endif
