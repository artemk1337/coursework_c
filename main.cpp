#include "lemin.h"


void ErrorExit(string msg)
{
	cout << "ERROR: " << msg << endl;
    exit (1);
}


class Neigh
{
	private:
		int weight = 0;
		int num_next_room = 0;
		int exist = 1;
		int global_exist = 1;
	public:
		Neigh(int num_next_room) {this->num_next_room = num_next_room;}

		int		getIdxNextRoom(void) const {return num_next_room;}
};


class Room
{
	private:
		vector<Neigh>	neighs;
		int				num_ant = 0;
		string			name;
		
		int				min_weight = 0;
		int				prev_room_idx = 0;

	public:
		string	getName(void) const {return name;}
		void	setName(string name) {this->name = name;}
		int		getNumAnt(void) const {return num_ant;}
		void	setNumAnt(int num_ant) {this->num_ant = num_ant;}

		Neigh	getNeighs(int i) {return neighs[i];}
		int		getNeighsSize(void) const {return neighs.size();}

		Room(string name)
		{
			this->name = name;
		}

		void addNeigh(int num_next_room) {neighs.push_back(Neigh(num_next_room));}

};





class Lemin
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
		Room	getRoomStart(void) {if (start == -1) ErrorExit("No START-room") ;
				return rooms[start - 1];}
		Room	getRoomEnd(void) {if (end == -1) ErrorExit("No END-room");
				return rooms[end - 1];}
		int		getSizeRooms(void) {return rooms.size();}

		void	addRoom(Room room, int param) {
			rooms.push_back(room);
			if (param == 1)
				start = rooms.size();
			else if (param == -1)
				end = rooms.size();
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


class FileReader: public Lemin
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


int main(int argc, char** argv) {
	int i = 0;
	int k = 0;
	int	start_finish = 0;
	FileReader	fread("example");

	g_lemin.setAnts(stoi(fread.getValue(0)));
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

	// print rooms
	for (i=0; i<g_lemin.getSizeRooms(); i++)
		cout << g_lemin.getRoom(i).getName() << endl;
	cout << "Start: " << g_lemin.getRoomStart().getName() << endl;
	cout << "End: " << g_lemin.getRoomEnd().getName() << endl;
	// prints neighs
	for (i=0; i<g_lemin.getSizeRooms(); i++)
	{
		cout << g_lemin.getRoom(i).getName() << ":" << endl;

		for (k=0; k<g_lemin.getRoom(i).getNeighsSize(); k++)
		{
			cout << g_lemin.getRoom(i).getName();
			cout << "-" << g_lemin.getRoom(g_lemin.getRoom(i).getNeighs(k).getIdxNextRoom()).getName() << endl;
		}
	}
}


