#include "lem-in.h"

Lemin g_lemin;

int main(int argc, char** argv) {
    if (argc == 2) {
    FileReader fread = read_file(string(argv[1]));
    fread.print_input();
    alg();
    prints_sol(0, 1);
    move_ants(g_lemin.getAllBestWays(), 1, 1);
    }
}
