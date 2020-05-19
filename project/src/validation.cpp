#include "lemin.h"
#include "valid_class.h"

#define ULLONG_MAX_STR "18446744073709551615"

// TODO Decide function returning value type void or string


void	ValidClass::free_word_list(char ***words)
{
	int i;
	char **words_ar;

	i = -1;
	words_ar = *words;
	if (!*words_ar)
		return ;
	while (words_ar[++i])
		ft_strdel(&words_ar[i]);
	free(*words);
}

unsigned long int ValidClass::check_ant_num(char *str)
{
	if ((strlen(str) > 19) || !ft_isdigitstr(str))
		error_exit();
	return ft_atoul(str);
}

void ValidClass::check_node(char *str)
{
	char **words;
	int i = -1;

	words = ft_strsplit(str, ' ');
	while (words[++i])
		;
	if (str[0] == 'L' || i != 3)
		error_exit();
	while (--i != 1)
		if (!ft_isdigitstr(words[i]))
			error_exit();
	free_word_list(&words);
}

void ValidClass::check_edge(char *str)
{
	char **nodes;
	int i = 0;

	nodes = ft_strsplit(str, '-');
	while (nodes[++i])
		;
	if (str[0] == 'L' || i != 2)
		error_exit();
	free_word_list(&nodes);
}

void ValidClass::check_one_hash(t_room **rooms_array, int i)
{
	int max_threshold;
	int j;

	max_threshold = (i / LeminObj->rooms) * LeminObj->rooms + LeminObj->rooms;
	j = max_threshold - LeminObj->rooms;
	while (j < max_threshold && rooms_array[j])
	{
		if (ft_strequ(rooms_array[i]->name, rooms_array[j]->name)
		&& i != j)
			error_exit();
		j++;
	}
}

void	ValidClass::check_duplicate_nodes(t_room **rooms_array)
{
	int i;

	i = 0;
	if (!LeminObj->start || !LeminObj->finish)
		error_exit();
	while (i < (LeminObj->rooms * LeminObj->rooms))
	{
		if (!rooms_array[i])
			i = (i / LeminObj->rooms + 1) * LeminObj->rooms;
		else {
			check_one_hash(rooms_array, i);
			i++;
		}
	}
}
