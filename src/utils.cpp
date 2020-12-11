#include "lem-in.h"


// выход с ошибкой
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


// <===== Разделение на подстроки =====> //

vector<string>	spliter_(string s, char delim, vector<string> &elems) {
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) {
        if (item.length() > 0) {
            elems.push_back(item);  
        }
    }
    return elems;
}

vector<string>	spliter(string s, char delim) {
    vector<string> elems;
    spliter_(s, delim, elems);
    return elems;
}


// <========== Обновление весов и предыдущих комнат ==========> //

void	update_min_weight_and_prev_room(void)
{
	int i;
	Room *tmproom;
	for (i=0; i<g_lemin.getSizeRooms(); i++)
	{
		tmproom = &g_lemin.getAddrRoom(i);
		if (g_lemin.getIdxStart() != i)
			tmproom->setMinWeight(INT_MAX / 2);
		else
			tmproom->setMinWeight(0);
		tmproom->setPrevRoomIdx(-1);
	}
}


// <========== Обновление весов связей + существование ==========> //

void	update_links(void)
{
	int i, ii;

	for (i=0; i<g_lemin.getSizeRooms(); i++)
	{
		for (ii=0; ii<g_lemin.getRoom(i).getNeighsSize(); ii++)
		{
			g_lemin.getAddrRoom(i).getAddrNeigh(ii).setWeight(1);
			g_lemin.getAddrRoom(i).getAddrNeigh(ii).setExist(1);
		}
	}
}

