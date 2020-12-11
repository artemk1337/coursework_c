#ifndef LEMIN_H
# define LEMIN_H

# include <climits>
# include <iostream>
# include <fstream>
# include <vector>
# include <regex>

# define CROSS_WAY -1
# define OKAY_WAY 0

using namespace std;

class Room;
class Neigh;
class Solutions;
class Lemin;
class FileReader;

// utils.cpp
void	ErrorExit(string msg);
void	print_neighs(void);
void	print_rooms(void);
void	print_ants(void);
void	prints_sol(int tmp, int best);

vector<string>	spliter_(string s, char delim, vector<string> &elems);
vector<string>	spliter(string s, char delim);

void	update_links();
void	update_min_weight_and_prev_room();

// main_alg.cpp
void	alg();

// compare_ways_and_move_ants.cpp
void	compare_ways();
int		move_ants(vector<vector<int>> sol, int print, int show_step);
void	sort_ways();

// split_rooms_and_repair.cpp
void	repair_all_rooms();
int		create_solution_and_split_rooms();
void	split_room(int idx_curr, int idx_prev, int idx_next);

// bellman-ford.cpp
void	BellmanFord();

// read_file.cpp
void		createRoom(string s, int param);
void		createNeigh(string s);
FileReader	read_file(string filename);


# include "classes.h"

extern Lemin g_lemin;

#endif
