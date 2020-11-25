#include "lem-in.h"

Lemin g_lemin;

int main(int argc, char** argv) {
	FileReader fread = read_file("../maps_example/subject-1.map");
	fread.print_input();
	alg();
	prints_sol(0, 1);
	move_ants(g_lemin.getAllBestWays(), 1, 1);
}
