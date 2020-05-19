#ifndef MOVING_ANTS_CLASS_H
# define MOVING_ANTS_CLASS_H

# include "lemin.h"

class MovingAntsClass
{
	friend int	main(void);
	private:
	int			check_ant(t_solution *sol, int len_way);
	void		print_ant(int c, char *s);
	void		move_ant(t_solution *sol, int len_way);
	protected:
	void		algorithm(void);
};

#endif
