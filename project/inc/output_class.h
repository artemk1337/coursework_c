#ifndef OUTPUT_CLASS_H
# define OUTPUT_CLASS_H

# include "lemin.h"


class OutputClass
{
	friend int	main(void);
	protected:
	void		test_way(void);
	public:
	void		show_input(void);
};


#endif