#include "lem-in.h"

void	ErrorExit(string msg)
{
	cout << "ERROR: " << msg << endl;
    exit (1);
}


// <========== TESTs ==========> //

void	print_neighs(void)
{
	int i, k;

	for (i=0; i<g_lemin.getSizeRooms(); i++)
	{
		//cout << g_lemin.getRoom(i).getName() << ":" << endl;
		for (k=0; k<g_lemin.getRoom(i).getNeighsSize(); k++)
		{
			cout << g_lemin.getRoom(i).getName();
			cout << "-" << g_lemin.getRoom(g_lemin.getRoom(i).getNeigh(k).getIdxNextRoom()).getName() << "\tWeight: " << g_lemin.getRoom(i).getNeigh(k).getWeight() << "\tGlExist: " << g_lemin.getRoom(i).getNeigh(k).getGlobalExist() << endl;
		}
	}
	cout << endl;
}

void	print_rooms(void)
{
	int i;

	for (i=0; i<g_lemin.getSizeRooms(); i++)
	{
		cout << g_lemin.getRoom(i).getName();
		if (g_lemin.getRoom(i).getPrevRoomIdx() > -1)
			cout << "\tPrev: "<< g_lemin.getRoom(g_lemin.getRoom(i).getPrevRoomIdx()).getName();
		else
			cout << "\tPrev: "<< "None";
		cout << "\tWeight: " << g_lemin.getRoom(i).getMinWeight();
		cout << "\tIn/Out: " << g_lemin.getRoom(i).getIdxIn() << "/" << g_lemin.getRoom(i).getIdxOut() << endl;
	}
	cout << "Start: " << g_lemin.getRoom(g_lemin.getIdxStart()).getName() << endl;
	cout << "End: " << g_lemin.getRoom(g_lemin.getIdxEnd()).getName() << endl;
	cout << endl;
}

void	print_ants(void){
	cout << "Ants: " << g_lemin.getAnts() << '\n' << endl;
}

void	prints_sol(int tmp=0, int best=1)
{
	int i, k;

	if (best){
		cout << "\nBest sol:" << endl;
		for (i=0; i<g_lemin.getSizeBestSol(); i++){
			for (k=0; k<g_lemin.getOneBestWay(i).size(); k++)
				cout << g_lemin.getRoom(g_lemin.getOneBestWay(i)[k]).getName() << '-';
			cout << endl;
		}
	}
	if (tmp){
		cout << "\nTmp sol:" << endl;
		for (i=0; i<g_lemin.getSizeTmpSol(); i++)
		{
			for (k=0; k<g_lemin.getOneTmpWay(i).size(); k++)
				cout << g_lemin.getRoom(g_lemin.getOneTmpWay(i)[k]).getName() << '-';
			cout << endl;
		}
	}
	cout << endl;
}
