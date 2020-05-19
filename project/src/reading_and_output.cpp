#include "lemin.h"
#include "reading_class.h"
#include "output_class.h"
#include "init_class.h"
#include "valid_class.h"

InitClass	InitClassObj;

void		ReadingMap::put_way_dop(t_tmp *tmp, t_tmp *tmp_2,
			char *s2, int weight)
{
	t_next *neigh;

	if (!tmp->room->next)
	{
		if (!(tmp->room->next = (t_next *)malloc(sizeof(t_next))))
			error_exit();
		neigh = tmp->room->next;
	}
	else
	{
		neigh = tmp->room->next;
		while (neigh->next)
		{
			if (!strcmp(neigh->room->name, s2))
				return ;
			neigh = neigh->next;
		}
		if (!(neigh->next = (t_next *)malloc(sizeof(t_next))))
			error_exit();
		neigh = neigh->next;
	}
	neigh->weight = weight;
	neigh->toggle = 1;
	neigh->next = NULL;
	while (tmp_2 && strcmp(tmp_2->room->name, s2))
		tmp_2 = tmp_2->next;
	if (tmp_2)
		neigh->room = tmp_2->room;
}

void		ReadingMap::put_way(char *s, t_tmp *tmp)
{
	t_tmp		*start;
	int			i;
	ValidClass	ValidClassObj;

	i = 0;
	(LeminObj->edge)++;
	ValidClassObj.check_edge(s);
	while (s[i] != '-')
		i++;
	s[i] = '\0';
	start = tmp;
	while (tmp && strcmp(tmp->room->name, s))
		tmp = tmp->next;
	if (tmp)
		put_way_dop(tmp, start, &(s[i + 1]), 1);
	tmp = start;
	while (tmp && strcmp(tmp->room->name, &(s[i + 1])))
		tmp = tmp->next;
	if (tmp)
		put_way_dop(tmp, start, s, 1);
	else
		error_exit();
}

t_tmp		*ReadingMap::create_struct_(char *line, t_tmp *tmp)
{
	ValidClass	ValidClassObj;

	if (strchr(line, ' '))
	{
		ValidClassObj.check_node(line);
		tmp = InitClassObj.init_tmp(tmp, 1, line);
		(LeminObj->rooms)++;
	}
	else if (strchr(line, '-'))
		put_way(line, tmp);
	return (tmp);
}

t_tmp		*ReadingMap::create_struct_prefix(char **line, t_tmp *tmp)
{
	ValidClass	ValidClassObj;

	if (!strcmp(&(line[0][2]), "start"))
	{
		ft_strdel(line);
		if ((get_next_line(STDIN, line)) < 1)
			error_exit();
		ValidClassObj.check_node(*line);
		LeminObj->ivector.push_back(string(*line));
		LeminObj->ivector.push_back("\n");
		tmp = InitClassObj.init_tmp(tmp, 0, *line);
		(LeminObj->rooms)++;
	}
	else if (!strcmp(&(line[0][2]), "end"))
	{
		ft_strdel(line);
		if ((get_next_line(STDIN, line)) < 1)
			error_exit();
		ValidClassObj.check_node(*line);
		LeminObj->ivector.push_back(string(*line));
		LeminObj->ivector.push_back("\n");
		tmp = InitClassObj.init_tmp(tmp, -1, *line);
		(LeminObj->rooms)++;
	}
	return (tmp);
}

void		ReadingMap::check_ants(char *line)
{
	ValidClass	ValidClassObj;
	LeminObj->count = ValidClassObj.check_ant_num(line);
	LeminObj->ivector.push_back(string(line));
	LeminObj->ivector.push_back("\n");
	ft_strdel(&line);
}

t_tmp		*ReadingMap::create_struct(void)
{
	char	*line;
	t_tmp	*tmp;

	tmp = NULL;
	if (get_next_line(STDIN, &line) > 0)
		check_ants(line);
	while (get_next_line(STDIN, &line) > 0)
	{
		if (*line == '\0')
		{
			ft_strdel(&line);
			break ;
		}
		LeminObj->ivector.push_back(string(line));
		LeminObj->ivector.push_back("\n");
		if (line[0] != '#')
			tmp = create_struct_(line, tmp);
		else if (line[0] == '#' && line[1] == '#')
			tmp = create_struct_prefix(&line, tmp);
		ft_strdel(&line);
	}
	if (!(tmp))
		error_exit();
	return (tmp);
}




void		OutputClass::show_input(void)
{
	long unsigned int i;

	for (i = 0; i < LeminObj->ivector.size(); i++)
		cout << LeminObj->ivector[i];
	cout << "\n";
}

void		OutputClass::test_way(void)
{
	t_room *room;

	room = LeminObj->finish;
	while (room != LeminObj->start)
	{
		std::cout << (room->name) << (" <- ");
		room = room->prev;
	}
	std::cout << (room->name) << ("\n");
}


