#include "main.hpp"

#define START 0 //the row we start copying from (in case something went wrong and we dont want to
				//go through the whole table again)

//reads all the content of rank_caches table in the main database, then
//sets up and fills tables in the auxiliary database so that the main app
//could easier and faster access up-to-date rankings
int copy_ranks()
{
	pqxx::result results;
	try{
		pqxx::nontransaction read(*database);
		results = read.exec("SELECT * FROM rank_caches;");
		read.commit();	
		int num = results.size();
		int id, done = 0;
		for(pqxx::result::const_iterator row = results.begin() + START;
			row != results.end(); row++)
		{	

			if(row[ROUND_ID].is_null())
				continue;

			check_if_rank_in_base(row(ROUND_ID), ROUND);
			check_if_rank_in_base(row(STAGE_ID), STAGE);
			check_if_rank_in_base(row(CONTEST_ID), CONTEST);
			
			if(row[TEAM_ID].is_null()) //solver is an individual
				id = row(USER_ID);
			else //solver is a team
				id = row(TEAM_ID);			


			if(is_solver_in_rank(id, row(ROUND_ID))) //user/team with this id already in this ranking
				update_result(row, row(ROUND_ID), row(PROB_ID));
			else //adding user/team to ranking
				add_result(row, row(ROUND_ID), row(PROB_ID));
			
			if(is_solver_in_rank(id, row(STAGE_ID))) //user/team with this id already in this ranking
				update_result(row, row(STAGE_ID), row(ROUND_ID));
			else //adding user/team to ranking
				add_result(row, row(STAGE_ID), row(ROUND_ID));
			
			if(is_solver_in_rank(id, row(CONTEST_ID))) //user/team with this id already in this ranking
				update_result(row, row(CONTEST_ID), row(STAGE_ID));
			else //adding user/team to ranking
				add_result(row, row(CONTEST_ID), row(STAGE_ID));
			
			if(++done % 100 == 0)
				printf("copied %d out of %d\n", START + done, num);
		}
	}
	catch(const pqxx::sql_error e){
		cerr << e.what() << "\n";
		return -1;
	}
	catch(const exception e){
		cerr << e.what() << "\n";
		return -1;
	}
	return 0;
}

int main()
{
	//trying to establish connection to the database
	try{
		database = new pqxx::connection(dbparams);
		rankbase = new pqxx::connection(rankdbparams);
		pqxx::nontransaction write(*rankbase);
		write.exec("SET client_min_messages TO WARNING;"); //we dont want all the psql notices 
															//in the standard output
		write.commit();
		int copy_success = copy_ranks();
		clean_up();
		return copy_success;
	}
	catch(const pqxx::sql_error e){
		cerr << e.what() << "\n";
		return -1;
	}
	catch(const exception e){
		cerr << e.what() << "\n";
		return -1;
	}
	return 0;
}