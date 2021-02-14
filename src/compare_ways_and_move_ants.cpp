#include "lem-in.h"


// сортировка путей
void    sort_ways(){
    if (g_lemin.getSizeTmpSol() > 1)
        sort(g_lemin.getAddrAllTmpWays().begin(), g_lemin.getAddrAllTmpWays().end(), [](const vector<int> & a, const vector<int> & b){ return a.size() < b.size(); });
    /*
    if (g_lemin.getSizeBestSol() > 1)
        sort(g_lemin.getAddrAllBestWays().begin(), g_lemin.getAddrAllBestWays().end(), [](const vector<int> & a, const vector<int> & b){ return a.size() < b.size(); });
    */
}

// перемещение муравьев по комнатам
int        move_ants(vector<vector<int>> sol, int print=1, int show_step=0){
    int    i, ii;
    int    steps = 0;
    int*    len_ways = new int[sol.size() + 1];
    int*    dist_ways = new int[sol.size() + 1];
    int    tmp = 0;

    if (sol.size() > 0)
        for (i=0; i<sol.size(); i++)
            len_ways[i] = int(sol[i].size());
    else
        return (0);

    for (i=0; i<sol.size(); i++)
    {
        dist_ways[i] = len_ways[i] * i - tmp;
        tmp += len_ways[i];
    }

    // перенес всех муравьев в стартовую комнату
    g_lemin.getAddrRoom(g_lemin.getIdxStart()).setNumAnt(g_lemin.getAnts());
    g_lemin.getAddrRoom(g_lemin.getIdxEnd()).setNumAnt(0);
    while (g_lemin.getAddrRoom(g_lemin.getIdxEnd()).getNumAnt() != g_lemin.getAnts()){
        // пробегаюсь по списку путей
        for (i=0; i<sol.size(); i++)
        {
            // пробегаюсь по пути от конца до начала и переношу муравьев
            for (ii=int(sol[i].size()) - 1; ii>0; ii--)
            {
                // проверка условия отправки муравья из старта
                if (ii == 1 && dist_ways[i] > g_lemin.getRoom(g_lemin.getIdxStart()).getNumAnt())
                    break ;
                if (g_lemin.getRoom(sol[i][static_cast<__int64>(ii) - 1]).getNumAnt() > 0)
                {
                    tmp = g_lemin.getRoom(sol[i][static_cast<__int64>(ii) - 1]).getNumAnt();
                    if (ii == 1)
                        g_lemin.getAddrRoom(sol[i][static_cast<__int64>(ii) - 1]).setNumAnt(g_lemin.getRoom(sol[i][static_cast<__int64>(ii) - 1]).getNumAnt() - 1);
                    else
                        g_lemin.getAddrRoom(sol[i][static_cast<__int64>(ii) - 1]).setNumAnt(0);
                    if (ii == sol[i].size() - 1)
                        g_lemin.getAddrRoom(sol[i][ii]).setNumAnt(g_lemin.getRoom(sol[i][ii]).getNumAnt() + 1);
                    else
                        g_lemin.getAddrRoom(sol[i][ii]).setNumAnt(tmp);
                    if (print)
                        cout << "L" << abs(tmp - (g_lemin.getAnts() + 1)) << "-" << g_lemin.getRoom(sol[i][ii]).getName() << " ";
                }
            }
        }
        steps++;
        if (show_step)
            cout << "\t| step " << steps;
        if (print)
            cout << endl;
    }
    g_lemin.getAddrRoom(g_lemin.getIdxStart()).setNumAnt(g_lemin.getAnts());
    g_lemin.getAddrRoom(g_lemin.getIdxEnd()).setNumAnt(0);
    return steps;
}

// сравнение путей
void    compare_ways()
{
    int    i;

    sort_ways();
    if (g_lemin.getSizeBestSol() == 0 ||
        move_ants(g_lemin.getAllBestWays(), 0) > move_ants(g_lemin.getAllTmpWays(), 0))
    {
        g_lemin.clearBestSol();
        for (i=0; i<g_lemin.getSizeTmpSol(); i++)
            g_lemin.addWayBestSol(g_lemin.getOneTmpWay(i));
    }
}
