#ifndef ranking_hpp
#define ranking_hpp
//#include "ranking.cpp"
#include "main.hpp"

class ranking
{
	public:
		int id;
		int pointed; //how many pointers to this ranking are being used
		vector <result> rows;
		vector <int> lower_ids;
		map <int, result*> map_ids;
		int lower_num();
		//vector <int> ids;
		ranking();
		~ranking();
};

#endif