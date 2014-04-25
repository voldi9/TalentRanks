#include "utils.hpp"

string int_to_string(int x)
{
	if(x < 0)
		return "-" + int_to_string(-x);
	string s = "";
	while(x)
	{
		s = char(48 + x%10) + s;
		x /= 10;
	}
	return s;
}

void clean_up()
{
	database->disconnect();
	rankbase->disconnect();
	delete database;
	delete rankbase;
}
