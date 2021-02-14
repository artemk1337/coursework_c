#include "lem-in.h"

// <===== Создание комнат и соседей =====> //

void    createRoom(string s, int param)
{
    vector<string>    words;

    // Разделить строку на слова
    words = spliter(s, ' ');
    if (words.size() != 3) {ErrorExit("Not valid string line with ` `");}
    // Добавить новую комнату
    g_lemin.addRoom(Room(words[0]), param);
}

void    createNeigh(string s)
{
    vector<string>    words;

    int                idx1;
    int                idx2;

    // Разделить строку на слова
    words = spliter(s, '-');
    if (words.size() != 2) {ErrorExit("Not valid string line with `-`");}

    // Поиск индекса комнаты по названию
    idx1 = g_lemin.getIdxRoomByName(words[0]);
    idx2 = g_lemin.getIdxRoomByName(words[1]);
    // Добавить нового соседа
    if (idx1 != -1 && idx2 != -1) {
        g_lemin.getAddrRoom(idx1).addNeigh(idx2);
        g_lemin.getAddrRoom(idx2).addNeigh(idx1);
    }
    else
        ErrorExit("Cant find exist rooms by name");
}


// <========== Считывание файла ==========> //

FileReader    read_file(string filename)
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
