#ifndef HASH_CLASS_H
# define HASH_CLASS_H

# include "lemin.h"


class HashClass
{
	friend int		main(void);
	private:
	unsigned long	hash(unsigned char *str);
	void			add_el_in_array(t_room *room, t_room **array);
	t_room			*find_element(char *name, t_room **room_array);
	protected:
	t_room			**create_array(t_tmp **list_begin);
};


#endif