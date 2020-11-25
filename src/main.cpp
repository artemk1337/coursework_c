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

int		create_solution_and_split_rooms()
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














int main(int argc, char** argv) {
	FileReader fread = read_file("../maps_example/subject-1.map");
	fread.print_input();
	alg();
	prints_sol(0, 1);
	move_ants(g_lemin.getAllBestWays(), 1, 1);
}
