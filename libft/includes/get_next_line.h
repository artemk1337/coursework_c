#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# define BUFF_SIZE	1
# define FD_SIZE 1488

# include <cstring> //string.h

int		get_next_line(const int fd, char **line);

#endif
