#ifndef VALID_CLASS_H
# define VALID_CLASS_H

# include "lemin.h"

class ValidClass
{
	public:
	void				free_word_list(char ***words);
	unsigned long int	check_ant_num(char *str);
	void				check_node(char *str);
	void				check_edge(char *str);
	void				check_one_hash(t_room **rooms_array, int i);
	void				check_duplicate_nodes(t_room **rooms_array);
};


#endif
