#include "main.hpp"



int check_if_rank_in_base(int id, int flag)
{
	pqxx::result results;
	//printf("tu!\n");
	try{ //detecting all problems in this round
		pqxx::nontransaction read(*database);
		string query = "CREATE TABLE IF NOT EXISTS \"" + int_to_string(id)  + "\"(\n" + 
						"user_id INTEGER NOT NULL DEFAULT 0,\n"
						"team_id INTEGER NOT NULL DEFAULT 0,\n"
						"points INTEGER NOT NULL DEFAULT 0";

		//printf("before accessing:\n");
		switch(flag)
		{
			case ROUND:
				//printf("%s\n", (round_query + int_to_string(id) + ";").c_str());
				results = read.exec(round_query + int_to_string(id) + ";");
				break;
			case STAGE:
				//printf("%s\n", (stage_query + int_to_string(id) + ";").c_str());
				results = read.exec(stage_query + int_to_string(id) + ";");
				break;
			case CONTEST:
				//printf("%s\n", (contest_query + int_to_string(id) + ";").c_str());
				results = read.exec(contest_query + int_to_string(id) + ";");
				break;
			default:  //error
				return 1;
		}

		read.commit();
		vector <pair<int, int>> lowers;
		lowers.clear();
		//printf("jeszcze ok!!!\n");

		//sorting lower levels structures in ranking (e.g. problems in a round) 
		//according to database tables
		for(pqxx::result::const_iterator row = results.begin();
			row != results.end(); row++){
			if(row[0].is_null()) //assuming a non-ordered lower is before the ordered ones
				lowers.push_back(make_pair(-1, row(1)));
			else
				lowers.push_back(make_pair(row(0), row(1)));
		

		}
		sort(lowers.begin(), lowers.end());

		
		for(int i=0; i<lowers.size(); i++)
			query += ",\n\"" + int_to_string(lowers[i].second) + "\" INTEGER NOT NULL DEFAULT 0";
		query += "\n);";
		//printf("%s\n", query.c_str());
		pqxx::nontransaction write(*rankbase);
		write.exec(query);
		write.commit();
	}
	catch(const exception e){
		cerr << e.what() << "\n";
		while(1);
		return 1;
	}
	return 0;
}

int copy_ranks()
{
	pqxx::result results;
	try{
		pqxx::nontransaction read(*database);
		results = read.exec("SELECT * FROM rank_caches;");
		read.commit();	
		int num = results.size();
		int done = 0;
		for(pqxx::result::const_iterator row = results.begin();
			row != results.end(); row++)
		{
			if(row[ROUND_ID].is_null())
				continue;

			check_if_rank_in_base(row(ROUND_ID), ROUND);
			check_if_rank_in_base(row(STAGE_ID), STAGE);
			check_if_rank_in_base(row(CONTEST_ID), CONTEST);
			
			pqxx::result rank_results;
			pqxx::nontransaction rank_read(*database);

			if(row[USER_ID].is_null()) //solver is a team
				rank_results = rank_read.exec(
									"SELECT count(*) FROM rank_caches\n"
									"WHERE team_id = " + int_to_string(row(TEAM_ID)) + "\n" +
									"AND problem_id = " + int_to_string(row(PROB_ID)) + "\n" +
									"AND contest_round_id = " + int_to_string(row(ROUND_ID)) + ";");
			else //solver is an individual
				rank_results = rank_read.exec(
									"SELECT count(*) FROM rank_caches\n"
									"WHERE user_id = " + int_to_string(row(USER_ID)) + "\n" +
									"AND problem_id = " + int_to_string(row(PROB_ID)) + "\n" +
									"AND contest_round_id = " + int_to_string(row(ROUND_ID)) + ";");
			
			rank_read.commit();

			pqxx::nontransaction write(*database);

			if(rank_results.size()) //user/team with this id already in this ranking
			{

			}
			else //adding user/team to ranking
			{
				string columns = "(";
				string values = "(";

				if(row[USER_ID].is_null())
				{

				}
				else

				columns += ")";
				values += ")";
				write.exec("INSERT INTO \"" + int_to_string(row(ROUND_ID)) + "\" " +
							columns + "\n"
							"VALUES " + values + ";");
			}
			
			write.commit();
			
			if(++done % 100 == 0)
				printf("copied %d out of %d\n", done, num);
		}
	}
	catch(const exception e){
		cerr << e.what() << "\n";
		return 1;
	}
}


int main()
{
	//trying to establish connection to the database
	try{
		database = new pqxx::connection(dbparams);
		rankbase = new pqxx::connection(rankdbparams);
		pqxx::nontransaction write(*rankbase);
		write.exec("SET client_min_messages TO WARNING;");
		write.commit();
		int copy_success = copy_ranks();
		clean_up();
		return copy_success;
	}
	catch(const exception e){
		cerr << e.what() << "\n";
		return 1;
	}
	return 0;
}