#ifndef group_hpp
#define group_hpp
//#include "group.cpp"
#include "main.hpp"

class group
{
	public:
		int id;
		int pointed; //how many pointers to this group are being used
		time_t last_submit;
		vector <result*> rows;
		vector <int> lower_ids;
		map <int, result*> map_ids;
		void print();
		int lower_num();
		void update(int solver_id, int lower_id, int total_points);
		group();
		~group();
};

#endif