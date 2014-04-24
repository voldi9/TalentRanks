#ifndef ranking_hpp
#define ranking_hpp
//#include "ranking.cpp"
#include "main.hpp"

class ranking
{
	public:
		int id;
		int pointed; //how many pointers to this ranking are being used
		time_t last_submit;
		vector <result*> rows;
		vector <int> lower_ids;
		map <int, result*> map_ids;
		void print();
		int lower_num();
		void update(int solver_id, int lower_id, int total_points);
		ranking();
		~ranking();
};

#endif