#ifndef CLASSES_H
# define CLASSES_H

# include "lem-in.h"

class Room;
class Neigh;
class Solutions;
class Lemin;
class FileReader;

class Neigh
{
	private:
		int weight = 1;
		int idx_next_room = 0;
		int exist = 1;
		int global_exist = 1;

	public:
		Neigh(int num_next_room);

		int		getIdxNextRoom(void);
		void	setIdxNextRoom(int idx_next_room);
		int		getWeight(void);
		void	setWeight(int weight);
		int		getExist(void);
		void	setExist(int exist);
		int		getGlobalExist(void);
		void	setGlobalExist(int global_exist);
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
		vector<vector<int>>	&getAddrAllBestWays() {return best_sol;}
		vector<vector<int>>	getAllBestWays() {return best_sol;}

		int			getSizeTmpSol(void) const {return tmp_sol.size();}
		vector<int>	getOneTmpWay(int idx) const {return tmp_sol[idx];}
		vector<vector<int>>	&getAddrAllTmpWays() {return tmp_sol;}
		vector<vector<int>>	getAllTmpWays() {return tmp_sol;}

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
		void	setAnts(int ants_);
		int		getAnts(void);

		Room	getRoom(int i);
		Room	&getAddrRoom(int i);
		int		getIdxStart(void);
		int		getIdxEnd(void);
		int		getSizeRooms(void);
		void	delRoom(int idx);

		void	addRoom(Room room, int param);

		int		getIdxRoomByName(string name);
};

class FileReader
{
	private:
		vector<string>	file_text;

	public:
		FileReader(string filename);

		string	getValue(int i);
		int		getSizeVec(void);
		void	print_input(void);
};


#endif
