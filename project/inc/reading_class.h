#ifndef READING_CLASS_H
# define READING_CLASS_H

# include "lemin.h"


class ReadingMap
{
	private:
	void		put_way_dop(t_tmp *tmp, t_tmp *tmp_2, char *s2, int weight);
	void		put_way(char *s, t_tmp *tmp);
	t_tmp		*create_struct_(char *line, t_tmp *tmp);
	t_tmp		*create_struct_prefix(char **line, t_tmp *tmp);
	void		check_ants(char *line);
	public:
	t_tmp		*create_struct(void);
};

#endif
