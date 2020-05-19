#ifndef LEMIN_H
# define LEMIN_H

# define STDIN 0

# include <libft.h>
# include <climits> //limits.h
# include <iostream> // only for std::out
# include <vector>

using namespace std;

class AlgClass;
class AlgSupreposClass;
class Bellman_Ford;
class HashClass;
class InitClass;
class MovingAntsClass;
class ValidClass;
class OutputClass;
class ReadingMap;
class SolutionHandlerClass;


class t_next;
class t_room;
class t_tmp;
class t_solution;
class Lemin;
extern Lemin *LeminObj;



class t_next
{
	friend class SolutionHandlerClass;
	friend class Bellman_Ford;
	friend class AlgSupreposClass;
	friend class ReadingMap;
	protected:
	class t_room		*room;
	int					weight;
	int					toggle;
	class t_next		*next;
};

class t_room
{
	friend class SolutionHandlerClass;
	friend class Bellman_Ford;
	friend class AlgSupreposClass;
	friend class ReadingMap;
	friend void reset_struct(class t_tmp*);
	friend class HashClass;
	friend class InitClass;
	friend class AlgClass;
	friend class MovingAntsClass;
	friend class ValidClass;
	friend class OutputClass;
	protected:
	char				*name;
	t_next				*next;

	int					min_w;
	int					superpos;
	int					was_sup;
	class t_room		*prev;
	class t_room		*prev1;

	class t_room		**path;
	int					idx;

	// For ants
	unsigned long		ant;
	unsigned long		number_anc;
};

class t_tmp
{
	friend void clean_tmp(t_tmp**);
	friend void reset_struct(t_tmp*);
	friend class HashClass;
	friend class InitClass;
	friend class Bellman_Ford;
	friend class AlgSupreposClass;
	friend class ReadingMap;
	protected:
	t_room				*room;
	class t_tmp			*next;	
};

class t_solution
{
	friend class SolutionHandlerClass;
	friend class AlgSupreposClass;
	friend class MovingAntsClass;
	protected:
	t_room				**arr;
	unsigned int		path_len;
	int					hide;
	class t_solution	*next;
};

class Lemin
{
	public:
	vector <string>		ivector;
	unsigned long		count;

	int					rooms;
	t_tmp				*list_;
	int					edge;

	t_room				*start;
	t_room				*finish;
	t_room				**arr;
	t_solution			*solution;
	t_solution			*prev_solution;
};

void					clean_tmp(t_tmp **tmp);
void					reset_struct(t_tmp *list);

void					error_exit(void);

#endif
