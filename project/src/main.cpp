#include "lemin.h"
#include "reading_class.h"
#include "output_class.h"
#include "init_class.h"
#include "moving_ants_class.h"
#include "solution_handler_class.h"
#include "hash_class.h"
#include "valid_class.h"

using namespace std;

// Алгоритм Беллмана-Форда
class Bellman_Ford
{
	protected:
	virtual int	bellman_ford(t_tmp *start, int counter)
	{
		t_tmp	*curr;
		t_room	*prev_r;
		t_next	*curr_n, *prev_n;

		for (curr = start; curr != NULL; curr = curr->next)
		{
			for (curr_n = curr->room->next; curr_n != NULL; curr_n = curr_n->next)
			{
				prev_r = curr_n->room;
				prev_n = prev_r->next;
				while (prev_n && prev_n->room != curr->room)
					prev_n = prev_n->next;
				if (prev_n->toggle && prev_r->min_w + prev_n->weight < curr->room->min_w
				&& prev_r != LeminObj->finish && !curr->room->superpos && !prev_r->superpos
				&& prev_r->min_w != (INT_MAX / 2) && prev_r->path == NULL)
				{
					counter++;
					curr->room->min_w = prev_r->min_w + prev_n->weight;
					curr->room->prev = prev_r;
				}
			}
		}
		return (counter);
	}
};

// Модифицированный Беллман-Форд
class AlgSupreposClass : private Bellman_Ford
{
	private:
	SolutionHandlerClass	SolutionHandlerClassObj;
	// Версия алгоритма Беллмана-Форда для пересекающихся путей.
	int			bellman_ford(t_tmp *start, int counter) override
	{
		t_tmp	*curr;
		t_room	*prev_r;
		t_next	*curr_n;
		t_next	*prev_n;

		for (curr = start; curr != NULL; curr = curr->next)
		{
			for (curr_n = curr->room->next; curr_n != NULL; curr_n = curr_n->next)
			{
				prev_r = curr_n->room;
				// Находим связь с предыдущей комнаты на текущую
				prev_n = prev_r->next;
				while (prev_n && prev_n->room != curr->room)
					prev_n = prev_n->next;
				// Отсюда начинается сам алгоритм
				/*
				prev_r - предыдущая комната\
				prev_n - соединение предыдущей комнаты с текущей\
				curr->room - текущая комната\
				*/
				if (prev_n->toggle && prev_r->min_w + prev_n->weight < curr->room->min_w
				&& prev_r != LeminObj->finish && !curr->room->superpos && !prev_r->superpos
				&& prev_r->min_w != (INT_MAX / 2)
				&& (prev_r->path == curr->room->path || (curr->room->path && !prev_r->path)))
				{
					counter++;
					curr->room->min_w = prev_r->min_w + prev_n->weight;
					curr->room->prev = prev_r;
				}
				// Этот кусок добавлен для выхода с пути, который уже существует.
				// Сложная штука.
				// Если новый путь пересекает пути, который распложены вплотную друг к другу,
				// то возникает проблема с сохранением предыдущей вершины.
				// Для этого используется дополнительная переменная prev1,
				// позволяющая хранить 2 предыдущие вершины.
				else if (prev_n->toggle && prev_r->path && prev_r->path[prev_r->idx + 1]
				&& curr->room->path != prev_r->path
				&& curr->room != LeminObj->start && prev_r != LeminObj->finish
				&& prev_r->path[prev_r->idx + 1] != LeminObj->finish
				&& prev_r->path[prev_r->idx + 1]->min_w != (INT_MAX / 2)
				&& prev_r->path[prev_r->idx + 1]->min_w - 1 + prev_n->weight < curr->room->min_w)
				{
					counter++;
					prev_r->min_w = prev_r->path[prev_r->idx + 1]->min_w - 1;
					curr->room->min_w = prev_r->min_w + prev_n->weight;
					curr->room->prev = prev_r;
					curr->room->prev1 = prev_r->path[prev_r->idx + 1];
					prev_r->prev = prev_r->path[prev_r->idx + 1];
				}
			}
		}
		return (counter);
	}

	// Удаляет решение из списка
	void	remove_sol_main(t_solution *sol)
	{
		t_solution	*t_s;
		t_solution	*t_s_pr;

		t_s_pr = NULL;
		for (t_s = LeminObj->solution; t_s != sol; t_s = t_s->next)
			t_s_pr = t_s;
		if (!t_s_pr)
			LeminObj->solution = t_s->next;
		else
			t_s_pr->next = t_s->next;
		free(sol->arr);
		free(sol);
	}

	// Страшная функция.
	// Коротко говоря, меняет пути в случае их наложения друг на друга.
	// Удаляет старые и заново выделяет память под новые, используя функцию save_tmp()
	void		remove_path_and_len(t_room** arr_prev, t_room** arr_target,
		int start, int finish)
	{
		t_solution	*tmp;
		int			i;

		i = -1;
		while (arr_target[++i])
		{
			arr_target[i]->idx = 0;
			arr_target[i]->path = NULL;
		}
		i = -1;
		while (arr_prev[++i])
		{
			if (arr_prev == arr_prev[i]->path)
			{
				arr_prev[i]->idx = 0;
				arr_prev[i]->path = NULL;
			}
		}
		i = -1;
		while (arr_prev[++i + 1] && i < start)
				arr_prev[i + 1]->prev = arr_prev[i];
		i = -1;
		while (arr_target[++i + 1] != arr_prev[start])
			;
		while (arr_target[++i + 1])
			arr_target[i + 1]->prev = arr_target[i];
		if (!SolutionHandlerClassObj.save_tmp())
			cout << "Problems in replacing ways 1";
		i = -1;
		while (arr_target[++i] != arr_prev[finish])
			arr_target[i + 1]->prev = arr_target[i];
		i = finish;
		while (arr_prev[++i])
			arr_prev[i]->prev = arr_prev[i - 1];
		if (!SolutionHandlerClassObj.save_tmp())
			cout << "Problems in replacing ways 2";
		tmp = LeminObj->solution;
		while (tmp)
		{
			if (tmp->arr == arr_target)
			{
				remove_sol_main(tmp);
				break ;
			}
			else
				tmp = tmp->next;
		}
		tmp = LeminObj->solution;
		while (tmp)
		{
			if (tmp->arr == arr_prev)
			{
				remove_sol_main(tmp);
				break ;
			}
			else
				tmp = tmp->next;
		}
	}

	// Проверка: если проложился путь, пересекающий другие пути
	int		check_valid_ways(void)
	{
		t_solution	*tmp;
		int			i;
		int			start;
		int			finish;

		tmp = LeminObj->solution;
		while (tmp->next)
			tmp = tmp->next;
		i = -1;
		while (tmp->arr[++i])
		{
			if (tmp->arr[i]->path != tmp->arr && tmp->arr[i]->path)
			{
				start = i;
				while (tmp->arr[i + 1]->path == tmp->arr[i]->path)
					i++;
				finish = i;
				remove_path_and_len(tmp->arr, tmp->arr[i]->path, start, finish);
				return (1);
			}
		}
		return (0);
	}

	// Переносим room->prev1 в room->prev->prev
	// Костыль, избавляющий от проблем в случае множества рядом-проложенных путей.
	// См. выше во втором Беллман-Форд
	void	fix_problem(void)
	{
		t_room	*curr_r;

		for (curr_r = LeminObj->finish; curr_r != LeminObj->start; curr_r = curr_r->prev)
		{
			if (curr_r->prev1)
			{
				curr_r->prev->prev = curr_r->prev1;
				curr_r->prev1 = NULL;	
			}
		}
	}

	// Если нашелся первый пересекающийся путь, то вызывается второй алгоритм
	public:
	int	algorithm(t_tmp *list)
	{
		int i;

		while (1)
		{
			for (i = 0; LeminObj->edge; ++i)
				if (!bellman_ford(list, 0))
					break ;
			if (!(LeminObj->finish->prev))
				return (1);
			fix_problem();
			if (LeminObj->prev_solution)
				SolutionHandlerClassObj.destroy_solutions(&(LeminObj->prev_solution));
			LeminObj->prev_solution = SolutionHandlerClassObj.copy_solution(LeminObj->solution);
			if (!SolutionHandlerClassObj.save_tmp())
				cout << "Error in find conflict!\n";
			while (check_valid_ways())
				;
			if ((SolutionHandlerClassObj.check_solutions(&LeminObj->prev_solution, &LeminObj->solution)))
			{
				SolutionHandlerClassObj.destroy_solutions(&LeminObj->solution);
				LeminObj->solution = LeminObj->prev_solution;
			}
			reset_struct(list);
		}
	}
};

class AlgClass : private Bellman_Ford
{
	private:
	AlgSupreposClass		BFS_modif;
	SolutionHandlerClass	SolutionHandlerClassObj;

	public:
	// Алгоритм
	void	algorithm(t_tmp *list)
	{
		int i;

		while (1)
		{
			// Сам алгоритм поиска кратчайшего пути
			for (i = 0; LeminObj->edge; ++i)
				if (!bellman_ford(list, 0))
					break ;
			// Если основной алгоритм не выполнился,
			// пробуем запустить второй для поиск пересекающхся путей
			if (!(LeminObj->finish->prev) && BFS_modif.algorithm(list))
			{
				SolutionHandlerClassObj.sort_solutions(&LeminObj->solution);
				return ;
			}
			// Удаляем предыдущеий набор путей и переприсваиваем
			if (LeminObj->prev_solution)
				SolutionHandlerClassObj.destroy_solutions(&(LeminObj->prev_solution));
			LeminObj->prev_solution = SolutionHandlerClassObj.copy_solution(LeminObj->solution);
			// Сохраняем путь
			if (!SolutionHandlerClassObj.save_tmp())
				break ;
			// Проверяем, лучше новый набор путей или хуже
			if ((SolutionHandlerClassObj.check_solutions(&LeminObj->prev_solution, &LeminObj->solution)))
			{
				SolutionHandlerClassObj.destroy_solutions(&LeminObj->solution);
				LeminObj->solution = LeminObj->prev_solution;
				return ;
			}
			reset_struct(list);
		}
	}
};

int		main(void)
{
	ReadingMap				ReadingMapObj;
	AlgClass				AlgClassObj;
	InitClass				InitClassObj;
	MovingAntsClass			MovingAntsClassObj;
	OutputClass				OutputClassObj;
	SolutionHandlerClass	SolutionHandlerClassObj;
	HashClass				HashClassObj;
	ValidClass				ValidClassObj;

	InitClassObj.init_class_lemin();
	LeminObj->list_ = ReadingMapObj.create_struct();
	LeminObj->arr = HashClassObj.create_array(&LeminObj->list_);
	ValidClassObj.check_duplicate_nodes(LeminObj->arr);
	AlgClassObj.algorithm(LeminObj->list_);
	SolutionHandlerClassObj.print_sol();
	if (!(LeminObj->solution))
		error_exit();
	OutputClassObj.show_input();
	MovingAntsClassObj.algorithm();
	return (0);
}
