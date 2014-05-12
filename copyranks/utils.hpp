#ifndef utils_hpp
#define utils_hpp
#include "main.hpp"

inline string query_str(int x);
string int_to_string(int x);
void clean_up();
int check_if_rank_in_base(int id, int flag);
int check_column_existance(int rank_id, int lower_id);
int add_result(pqxx::result::tuple row, int rank_id, int lower_id);
int update_result(pqxx::result::tuple row, int rank_id, int lower_id);
int is_solver_in_rank(int solver_id, int rank_id);
int copy_ranks();

#endif