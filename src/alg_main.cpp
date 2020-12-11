#include "lem-in.h"


// Основной алгоритм
void	alg(void)
{
	int	expect_ways, ways;

	ways = 0;
	for (expect_ways=1; 1; expect_ways++)
	{
		g_lemin.clearTmpSol();
		ways = 0;
		repair_all_rooms();
		update_min_weight_and_prev_room();
		// print_rooms();
		// print_neighs();
		while (ways != expect_ways)
		{
			update_min_weight_and_prev_room();
			BellmanFord();
			// если путей больше не существует, останавливаем алгоритм
			if (g_lemin.getRoom(g_lemin.getIdxEnd()).getPrevRoomIdx() == -1)
			{
				// cout << "No ways anymore" << endl;
				return;
			}
			if (!create_solution_and_split_rooms())
				ways++;
			else
			{
				g_lemin.clearTmpSol();
				ways = 0;
				repair_all_rooms();
				update_min_weight_and_prev_room();
			}
		}
		// сравнение путей; если новые пути лучше, используем их вместо старых
		compare_ways();
		// prints_sol(1, 1);
	}
}
