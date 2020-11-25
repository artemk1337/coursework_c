#include "lem-in.h"


Lemin g_lemin;



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

// <===== Разделение на подстроки =====> //




// <===== Создание комнат и соседей =====> //

void	createRoom(string s, int param)
{
	vector<string>	words;

	// Split string line on words
	words = spliter(s, ' ');
	if (words.size() != 3) {ErrorExit("Not valid string line with ` `");}
	// Add new room
	g_lemin.addRoom(Room(words[0]), param);
}

void	createNeigh(string s)
{
	vector<string>	words;

	int				idx1;
	int				idx2;

	// Split string line on words
	words = spliter(s, '-');
	if (words.size() != 2) {ErrorExit("Not valid string line with `-`");}

	// Find idx rooms
	idx1 = g_lemin.getIdxRoomByName(words[0]);
	idx2 = g_lemin.getIdxRoomByName(words[1]);
	// Add new neighbours
	if (idx1 != -1 && idx2 != -1) {
		g_lemin.getAddrRoom(idx1).addNeigh(idx2);
		g_lemin.getAddrRoom(idx2).addNeigh(idx1);
	}
	else
		ErrorExit("Cant find exist rooms by name");
}

// <===== Создание комнат и соседей =====> //





// <========== Считывание файла ==========> //

FileReader	read_file(string filename)
{
	int i = 0;
	int k = 0;
	int start_finish = 0;
	FileReader fread(filename);

	g_lemin.setAnts(stoi(fread.getValue(0)));
	if (g_lemin.getAnts() < 1)
		ErrorExit("Ants must be > 0");
	i = 0;
	while (++i < fread.getSizeVec())
	{
		if (fread.getValue(i) == "##start")
			start_finish = 1;
		else if (fread.getValue(i) == "##end")
			start_finish = -1;
		else if (fread.getValue(i)[0] == '#')
			continue ;
		else
		{
			if (fread.getValue(i).find(" ") != string::npos)
				createRoom(fread.getValue(i), start_finish);
			else if (fread.getValue(i).find("-") != string::npos)
				createNeigh(fread.getValue(i));
			start_finish = 0;
		}
	}
	// cout << 111 << endl;
	return (fread);
}

// <========== Считывание файла ==========> //



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

// <========== Обновление весов и предыдущих комнат ==========> //



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

// <========== Обновление весов связей ==========> //





// <========== Беллман-Форд ==========> //

void	BellmanFord(void)
{
	int i, iisrc, n;
	Room	*roomsrc;
	Room	*roomdst;
	Neigh	*neigh;
	int chacnges;

	for (i=0; i<g_lemin.getSizeRooms();i++)
	{
		chacnges = 0;
		for (iisrc=0; iisrc<g_lemin.getSizeRooms();iisrc++)
		{
			roomsrc = &g_lemin.getAddrRoom(iisrc);
			for (n=0; n<roomsrc->getNeighsSize(); n++)
			{
				neigh = &roomsrc->getAddrNeigh(n);
				roomdst = &g_lemin.getAddrRoom(neigh->getIdxNextRoom());

				if (iisrc != g_lemin.getIdxEnd() &&
					neigh->getExist() &&
					neigh->getGlobalExist() &&
					roomsrc->getMinWeight() < INT_MAX / 2 &&
					roomsrc->getMinWeight() + neigh->getWeight() < roomdst->getMinWeight())
				{
					chacnges++;
					roomdst->setPrevRoomIdx(iisrc);
					roomdst->setMinWeight(roomsrc->getMinWeight() + neigh->getWeight());
				}
			}
		}
		if (chacnges == 0)
			return ;
	}
}

// <========== Беллман-Форд ==========> //



// <========== Раздваивание комнат с переносом связей ==========> //

void	split_room(int idx_curr, int idx_prev, int idx_next)
{
	stringstream	name;
	int				i, ii, idx_neigh_room;
	Neigh			*neigh_from_curr, *neigh_from_neigh;

	name << g_lemin.getRoom(idx_curr).getName() << "_in";
	g_lemin.addRoom(Room(name.str()), 0);
	g_lemin.getAddrRoom(g_lemin.getSizeRooms()-1).setPrevRoomIdx(idx_prev);
	g_lemin.getAddrRoom(idx_curr).setIdxIn(g_lemin.getSizeRooms()-1);
	g_lemin.getAddrRoom(g_lemin.getSizeRooms()-1).setIdxOut(idx_curr);

	for (i=0; i<g_lemin.getRoom(idx_curr).getNeighsSize(); i++)
	{
		// cout << g_lemin.getRoom(idx_curr).getName() << endl;
		// прогоняем все связи из текущей комнаты
		neigh_from_curr = &g_lemin.getAddrRoom(idx_curr).getAddrNeigh(i);
		if (neigh_from_curr->getIdxNextRoom() != idx_next &&neigh_from_curr->getIdxNextRoom() != g_lemin.getRoom(idx_next).getIdxIn())
		{
			// ищем во всех связях соседа указатель на текущую комнату
			ii = 0;
			idx_neigh_room = neigh_from_curr->getIdxNextRoom();
			// если соседняя комната in, переводим указатель на out
			if (g_lemin.getRoom(idx_neigh_room).getIdxOut() != -1 )
				idx_neigh_room = g_lemin.getRoom(idx_neigh_room).getIdxOut();
			// ищем текущую комнату
			while (g_lemin.getRoom(idx_neigh_room).getNeigh(ii).getIdxNextRoom() != idx_curr)
				ii++;
			neigh_from_neigh = &g_lemin.getAddrRoom(idx_neigh_room).getAddrNeigh(ii);
			// если соседняя комната является предыдущей
			if (idx_neigh_room == idx_prev)
			{
				// зануляем связь
				neigh_from_neigh->setExist(0);
				// переносим связь с текущей на in комнату
				neigh_from_curr->setIdxNextRoom(g_lemin.getAddrRoom(idx_curr).getIdxIn());
				neigh_from_curr->setWeight(0);
			}
			neigh_from_neigh->setIdxNextRoom(g_lemin.getRoom(idx_curr).getIdxIn());
		}
		else
		{
			// ищем во всех связях соседа указатель на текущую комнату
			ii = 0;
			idx_neigh_room = neigh_from_curr->getIdxNextRoom();
			if (g_lemin.getRoom(idx_neigh_room).getIdxIn() != -1)
				idx_neigh_room = g_lemin.getRoom(idx_neigh_room).getIdxOut();
			while (g_lemin.getRoom(idx_neigh_room).getNeigh(ii).getIdxNextRoom() != idx_curr)
				ii++;
			neigh_from_neigh = &g_lemin.getAddrRoom(idx_neigh_room).getAddrNeigh(ii);
			neigh_from_curr->setExist(0);
			neigh_from_neigh->setWeight(-1);
		}
	}
	g_lemin.getAddrRoom(g_lemin.getSizeRooms()-1).addNeigh(idx_prev);
	g_lemin.getAddrRoom(g_lemin.getSizeRooms()-1).getAddrNeigh(g_lemin.getAddrRoom(g_lemin.getSizeRooms()-1).getNeighsSize()-1).setWeight(-1);
}

// <========== Раздваивание комнат с переносом связей ==========> //




// <========== Создание решение и прокладывание пути ==========> //

int		create_solution_and_split_rooms(void)
{
	int			idx;
	int			idx_next = 0;
	int			len_way = 0;
	int			id_in_room = -1;
	vector<int>	way;

	idx = g_lemin.getIdxEnd();
	while (idx != g_lemin.getIdxStart())
	{
		if (g_lemin.getRoom(idx).getIdxOut() > -1)
			id_in_room = idx;
		idx = g_lemin.getRoom(idx).getPrevRoomIdx();
	}
	if (id_in_room != -1)
	{
		g_lemin.getAddrRoom(id_in_room).getAddrNeigh(0).setGlobalExist(0);
		return (1);
	}

	idx = g_lemin.getIdxEnd();
	while (idx != g_lemin.getIdxStart()){
		if (idx != g_lemin.getIdxEnd())
			split_room(idx, g_lemin.getRoom(idx).getPrevRoomIdx(), idx_next);
		way.push_back(idx);
		idx_next = idx;
		idx = g_lemin.getRoom(idx).getPrevRoomIdx();
	}
	way.push_back(idx);
	reverse(way.begin(), way.end());
	g_lemin.addWayTmpSol(way);
	return (0);
}

// <========== Создание решение и прокладывание пути ==========> //





// <========== Восстановление комнат с переносом связей ==========> //

void	repair_all_rooms()
{
	int		idx, i, ii, idx_in, idx_prev, idx_neigh, exist;

	ii = -1;
	while (++ii<g_lemin.getSizeRooms() && g_lemin.getRoom(ii).getIdxOut() == -1)
	{
		exist = 1;
		idx_in = g_lemin.getRoom(ii).getIdxIn();
		// если есть раздвоение комнат
		if (idx_in > -1)
		{
			idx_prev = g_lemin.getRoom(idx_in).getNeigh(0).getIdxNextRoom();
			i = 0;
			// ищем связь на комнату in из out
			while (g_lemin.getRoom(ii).getNeigh(i).getIdxNextRoom() != idx_in)
				i++;
			if (!g_lemin.getRoom(idx_in).getNeigh(0).getGlobalExist())
				exist = 0;
			// если путь пересекался, изменил глобальное существование связи
			g_lemin.getAddrRoom(ii).getAddrNeigh(i).setGlobalExist(exist);
			// переносим связь с in на prev из out
			g_lemin.getAddrRoom(ii).getAddrNeigh(i).setIdxNextRoom(idx_prev);

			i = 0;
			// ищем связь на комнату in из prev
			while (g_lemin.getRoom(idx_prev).getNeigh(i).getIdxNextRoom() != idx_in)
				i++;
			g_lemin.getAddrRoom(idx_prev).getAddrNeigh(i).setGlobalExist(exist);
			g_lemin.getAddrRoom(idx_prev).getAddrNeigh(i).setIdxNextRoom(ii);

			g_lemin.getAddrRoom(ii).setIdxIn(-1);
			// связь удалим вместе с комнатой
		
			// перенос связий с комнаты in на out
			for (i=0; i<g_lemin.getRoom(ii).getNeighsSize(); i++)
			{
				idx_neigh = g_lemin.getRoom(ii).getNeigh(i).getIdxNextRoom();
				// если есть раздвоение комнаты
				if (g_lemin.getRoom(idx_neigh).getIdxOut() != -1)
					idx_neigh = g_lemin.getRoom(idx_neigh).getIdxOut();
				idx_prev = 0;
				// переносим связи. Если связь не найдено, эта комната приналежит к одному пути и связи нет.
				while (idx_prev < g_lemin.getRoom(idx_neigh).getNeighsSize() && g_lemin.getRoom(idx_neigh).getNeigh(idx_prev).getIdxNextRoom() != idx_in)
					idx_prev++;
				if (idx_prev < g_lemin.getRoom(idx_neigh).getNeighsSize())
					g_lemin.getAddrRoom(idx_neigh).getAddrNeigh(idx_prev).setIdxNextRoom(ii);
			}
		}
		
	}
	// удаляем все комнаты in
	while (ii < g_lemin.getSizeRooms())
		g_lemin.delRoom(ii);
	// восстанавливаем веса связей и существаование самих связей
	update_links();
}

// <========== Восстановление комнат с переносом связей ==========> //



// <========== Сравнение путей ==========> //

// сортировка путей
void	sort_ways(){
	if (g_lemin.getSizeTmpSol() > 1)
		sort(g_lemin.getAddrAllTmpWays().begin(), g_lemin.getAddrAllTmpWays().end(), [](const vector<int> & a, const vector<int> & b){ return a.size() < b.size(); });
	/*
	if (g_lemin.getSizeBestSol() > 1)
		sort(g_lemin.getAddrAllBestWays().begin(), g_lemin.getAddrAllBestWays().end(), [](const vector<int> & a, const vector<int> & b){ return a.size() < b.size(); });
	*/
}

int		move_ants(vector<vector<int>> sol, int print=1, int show_step=0){
	int	i, ii;
	int	steps = 0;
	int	len_ways[sol.size() + 1];
	int	dist_ways[sol.size() + 1];
	int	tmp = 0;
	int	ants;

	if (sol.size() > 0)
		for (i=0; i<sol.size(); i++)
			len_ways[i] = sol[i].size();
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
			for (ii=sol[i].size() - 1; ii>0; ii--)
			{
				// проверка условия отправки муравья из старта
				if (ii == 1 && dist_ways[i] > g_lemin.getRoom(g_lemin.getIdxStart()).getNumAnt())
					break ;
				if (g_lemin.getRoom(sol[i][ii - 1]).getNumAnt() > 0)
				{
					tmp = g_lemin.getRoom(sol[i][ii - 1]).getNumAnt();
					if (ii == 1)
						g_lemin.getAddrRoom(sol[i][ii - 1]).setNumAnt(g_lemin.getRoom(sol[i][ii - 1]).getNumAnt() - 1);
					else
						g_lemin.getAddrRoom(sol[i][ii - 1]).setNumAnt(0);
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

void	compare_ways(){
	int	i;
	int	ants;

	sort_ways();
	if (g_lemin.getSizeBestSol() == 0 ||
		move_ants(g_lemin.getAllBestWays(), 0) > move_ants(g_lemin.getAllTmpWays(), 0))
	{
		g_lemin.clearBestSol();
		for (i=0; i<g_lemin.getSizeTmpSol(); i++)
			g_lemin.addWayBestSol(g_lemin.getOneTmpWay(i));
	}
}

// <========== Сравнение путей ==========> //






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


int main(int argc, char** argv) {
	FileReader fread = read_file("../maps_example/subject-1.map");
	fread.print_input();
	alg();
	prints_sol(0, 1);
	move_ants(g_lemin.getAllBestWays(), 1, 1);
}
