#ifndef SOLUTION_HANDLER_CLASS_H
# define SOLUTION_HANDLER_CLASS_H

# include "lemin.h"

class AlgClass;
class AlgSupreposClass;
class SolutionHandlerClass;

struct s_bandwidth
{
	friend class SolutionHandlerClass;
	private:
	unsigned int	path_num;
	unsigned int	max_len;
	unsigned int	bandwidth; // max bandwidth for max_len moves
	t_solution		*solution;
};

class SolutionHandlerClass
{
	friend class AlgClass;
	friend class AlgSupreposClass;
	private:
	struct s_bandwidth	*count_solution_bandwidth(t_solution *solution);
	void				init_sol_dop(t_solution *tmp_s, int size, t_solution *sol);
	void				init_sol(t_solution *sol, int size);
	int					len_sol(t_solution *solution);
	protected:
	int					check_solutions(t_solution **prev_solution, t_solution **current_solution);
	t_solution			*copy_solution(t_solution *source_solutions);
	void				destroy_solutions(t_solution **solution);
	void				remove_sol(t_solution *sol);
	int					save_tmp(void);
	void				sort_solutions(t_solution **solution);
	public:
	void				print_sol(void);
};



#endif
