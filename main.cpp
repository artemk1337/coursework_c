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
class Lemin;
class FileReader;



void ErrorExit(string msg)
{
	cout << "ERROR: " << msg << endl;
    exit (1);
}


class Neigh
{
	private:
		int weight = 1;
		int idx_next_room = 0;
		int exist = 1;
		int global_exist = 1;

	public:
		Neigh(int num_next_room) {this->idx_next_room = num_next_room;}

		int		getIdxNextRoom(void) const {return idx_next_room;}
		void	setIdxNextRoom(int idx_next_room) {this->idx_next_room = idx_next_room;}
		int		getWeight(void) const {return weight;}
		void	setWeight(int weight) {this->weight = weight;}
		int		getExist(void) const {return exist;}
		void	setExist(int exist) {this->exist = exist;}
		int		getGlobalExist(void) const {return global_exist;}
		void	setGlobalExist(int global_exist) {this->global_exist = global_exist;}
};


class Room
{
	private:
		vector<Neigh>	neighs;
		int				num_ant = 0;
		string			name;

		int				idx_out = -1;
		int				idx_in = -1;
		
		int				min_weight = 0;
		int				prev_room_idx = -1;

	public:
		string	getName(void) const {return name;}
		void	setName(string name) {this->name = name;}
		int		getNumAnt(void) const {return num_ant;}
		void	setNumAnt(int num_ant) {this->num_ant = num_ant;}

		int		getIdxOut(void) const {return idx_out;}
		void	setIdxOut(int idx_out) {this->idx_out = idx_out;}
		int		getIdxIn(void) const {return idx_in;}
		void	setIdxIn(int idx_in) {this->idx_in = idx_in;}

		int		getMinWeight(void) const {return min_weight;}
		void	setMinWeight(int min_weight) {this->min_weight = min_weight;}
		int		getPrevRoomIdx(void) const {return prev_room_idx;}
		void	setPrevRoomIdx(int prev_room_idx)
			{this->prev_room_idx = prev_room_idx;}

		Neigh	getNeigh(int i) {return neighs[i];}
		Neigh	&getAddrNeigh(int i) {return neighs[i];}
		int		getNeighsSize(void) const {return neighs.size();}

		Room(string name)
		{
			this->name = name;
		}

		void addNeigh(int num_next_room) {neighs.push_back(Neigh(num_next_room));}

};

class Solutions
{
	private:
		vector<vector<int>>	best_sol;
		vector<vector<int>>	tmp_sol;
	
	public:
		int			getSizeBestSol(void) const {return best_sol.size();}
		vector<int>	getOneBestWay(int idx) const {return best_sol[idx];}
		int			getSizeTmpSol(void) const {return tmp_sol.size();}
		vector<int>	getOneTmpWay(int idx) const {return tmp_sol[idx];}

		void		addWayBestSol(vector<int> vec) {best_sol.push_back(vec);}
		void		addWayTmpSol(vector<int> vec) {tmp_sol.push_back(vec);}

		void		clearBestSol(void) {best_sol.clear();}
		void		clearTmpSol(void) {tmp_sol.clear();}
};




class Lemin: public Solutions
{
	private:
		int				ants = 0;
		vector<Room>	rooms;
		int				start = -1; // index start room
		int				end = -1; // index end room
	
	public:
		void	setAnts(int ants_) {ants = ants_;}
		int		getAnts(void) {return ants;}

		Room	getRoom(int i) {return rooms[i];}
		Room	&getAddrRoom(int i) {return rooms[i];}
		int		getIdxStart(void) const {if (start == -1) ErrorExit("No START-room"); 
					return start;}
		int		getIdxEnd(void) const {if (end == -1) ErrorExit("No END-room"); 
					return end;}
		int		getSizeRooms(void) {return rooms.size();}
		void	delRoom(int idx) {this->rooms.erase(this->rooms.begin() + idx);}

		void	addRoom(Room room, int param) {
			rooms.push_back(room);
			if (param == 1)
				start = rooms.size() - 1;
			else if (param == -1)
				end = rooms.size() - 1;
			}

		int		getIdxRoomByName(string name)
		{
			int i;
			for (i=0; i<rooms.size(); i++)
			 	if (rooms[i].getName() == name)
			 		return i;
			return int(-1);
		}

};


Lemin g_lemin;









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

void	print_ants(void)
{
	cout << "Ants: " << g_lemin.getAnts() << '\n' << endl;
}

void	prints_sol(void)
{
	int i, k;

	cout << "Best sol:" << endl;
	for (i=0; i<g_lemin.getSizeBestSol(); i++)
	{
		for (k=0; k<g_lemin.getOneBestWay(i).size(); k++)
			cout << g_lemin.getOneBestWay(i)[k] << '-';
		cout << endl;
	}
	cout << endl;
	cout << "\nTmp sol:" << endl;
	for (i=0; i<g_lemin.getSizeTmpSol(); i++)
	{
		for (k=0; k<g_lemin.getOneTmpWay(i).size(); k++)
			cout << g_lemin.getRoom((g_lemin.getOneTmpWay(i)[k])).getName() << '-';
		cout << endl;
	}
	cout << endl;
}

// <========== TESTs ==========> //











// <===== Считывание из файла =====> //

class FileReader
{
	private:
		vector<string>	file_text;

	public:
		FileReader(string filename)
		{	
			string buffer;
			ifstream ReadFile(filename);
			while (getline (ReadFile, buffer)) {
				file_text.push_back(buffer);
			}
			ReadFile.close();
		}

		string	getValue(int i) {return (file_text[i]);}
		int		getSizeVec(void) {return (file_text.size());}
};

// <===== Считывание из файла =====> //



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

void	read_file(string filename)
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
		else
		{
			if (fread.getValue(i).find(" ") != string::npos)
				createRoom(fread.getValue(i), start_finish);
			else if (fread.getValue(i).find("-") != string::npos)
				createNeigh(fread.getValue(i));
			start_finish = 0;
		}
	}
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


// <========== Обновление весов связей ==========> //

void	update_weight_links(void)
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
				// переносим связи. Если связь не найдена, эта комната приналежит к одному пути и связи нет.
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
	update_weight_links();
}



void	repair_all_rooms2()
{
	int i, ii, iii, idx_out_room, idx_in_room, idx_prev_room;

	for (i=0; i<g_lemin.getSizeRooms(); i++)
	{
		if (g_lemin.getRoom(i).getIdxIn() != -1)
		{
			idx_out_room = i;
			idx_in_room = g_lemin.getRoom(i).getIdxIn();
			idx_prev_room = g_lemin.getRoom(idx_in_room).getNeigh(0).getIdxNextRoom();
			// cout << idx_out_room << idx_in_room << idx_prev_room << endl;

			// перенесем связи с in на out

		}
	}
}


// <========== Восстановление комнат с переносом связей ==========> //




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
				cout << "No ways anymore" << endl;
				return;
			}
			if (!create_solution_and_split_rooms())
				ways++;
			else
				break ;
		}
		// здесь будет сравнение путей
		// 
		prints_sol();
	}
}


int main(int argc, char** argv) {

	read_file("example");
	alg();
}


