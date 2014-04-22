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
		//vector <int> ids;
		ranking();
		~ranking();
};

#endif