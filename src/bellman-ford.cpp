#include "lem-in.h"

// Алгоритм Беллмана-Форда
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
