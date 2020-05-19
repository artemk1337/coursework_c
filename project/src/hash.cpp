#include "lemin.h"
#include "hash_class.h"

//c program hash

unsigned long	HashClass::hash(unsigned char *str)
{
	unsigned long	hash;
	int				c;

	hash = 5381;
	while ((c = (*str++)))
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
	return hash;
}

void			HashClass::add_el_in_array(t_room *room, t_room **array)
{
	unsigned long	tmp_hash;
	unsigned int	i;

	tmp_hash = hash((unsigned char*)room->name) % LeminObj->rooms;
	i = tmp_hash * LeminObj->rooms;
	while (array[i])
		i++;
	array[i] = room;
}

// Не чищу!!! 
t_room			**HashClass::create_array(t_tmp **list_begin)
{
	t_room	**rooms_array;
	t_tmp	*start; // Добавил
	int		i;
	t_tmp	*temp_ptr;

	rooms_array = (t_room	**)ft_memalloc(sizeof(t_room*) * LeminObj->rooms * LeminObj->rooms);
	i = -1;
	start = *list_begin; // Добавил
	while (++i < LeminObj->rooms)
	{
		temp_ptr = *list_begin;
		add_el_in_array(temp_ptr->room, rooms_array);
		temp_ptr = (*list_begin)->next;
		//free(*list_begin); // Добавил
		*list_begin = temp_ptr;
	}
	*list_begin = start; // Добавил
	return rooms_array;
}

t_room			*HashClass::find_element(char *name, t_room **room_array)
{
	unsigned int	element_hash;

	element_hash = hash((unsigned char*)name);
	while (strcmp(room_array[element_hash]->name, name))
		element_hash++;
	return (room_array[element_hash]);
}
