#include <climits>
#include <iostream>
#include <fstream>
#include <vector>
#include <regex>

#define CROSS_WAY -1
#define OKAY_WAY 0

using namespace std;

class Room;
class Neigh;
class Solutions;
class Lemin;
class FileReader;


extern Lemin g_lemin;

// utils.cpp
void	ErrorExit(string msg);
void	print_neighs(void);
void	print_rooms(void);
void	print_ants(void);
void	prints_sol(int tmp=0, int best=1);

