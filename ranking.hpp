#ifndef ranking_hpp
#define ranking_hpp
//#include "ranking.cpp"
#include "main.hpp"

class ranking
{
	public:
		int id;
		int pointed; //how many pointers to this ranking are being used
		vector <result*> rows;
		vector <int> lower_ids;
		map <int, result*> map_ids;
		void print();
		int lower_num();
		ranking();
		~ranking();
};

#endif