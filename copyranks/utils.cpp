#include "utils.hpp"

//the tables and columns are being named with numbers (ids of problems, rounds, stages and contests)
//need to be referenced to as "ID", so we put the '"' character on both sides of the number
inline string query_str(int x)
{
	return "\"" + int_to_string(x) + "\"";
}

string int_to_string(int x)
{
	if(x < 0)
		return "-" + int_to_string(-x);
	if(x == 0)
		return "0";
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

//flag indicates whether this ranking is a round, stage or contest
int check_if_rank_in_base(int id, int flag)
{
	pqxx::result results;
	set <int> columns;
	columns.clear();
	try{ //detecting all problems in this round
		pqxx::nontransaction read(*database);
		pqxx::nontransaction rank_read(*rankbase);

		results = rank_read.exec("SELECT EXISTS (SELECT * FROM information_schema.tables\n"
							"WHERE table_name = '" + int_to_string(id) + "');");
		rank_read.commit();
		if(results[0][0].as<string>() == "t") //if table exists, skip
			return 0;

		string query = "CREATE TABLE IF NOT EXISTS " + query_str(id) + "(\n" + 
						"id INTEGER NOT NULL DEFAULT 0 PRIMARY KEY,\n"
						"points INTEGER NOT NULL DEFAULT 0";

		switch(flag)
		{
			case ROUND:
				results = read.exec(round_query + int_to_string(id) + ";");
				break;
			case STAGE:
				results = read.exec(stage_query + int_to_string(id) + ";");
				break;
			case CONTEST:
				results = read.exec(contest_query + int_to_string(id) + ";");
				break;
			default:  //error
				return -1;
		}

		read.commit();
		vector <pair<int, int>> lowers;
		lowers.clear();

		//sorting lower levels structures in ranking (e.g. problems in a round) 
		//according to database tables
		for(pqxx::result::const_iterator row = results.begin();
			row != results.end(); row++)
		{
			if(columns.find(row(1)) == columns.end()) //same lower more than once in ranking definition
				continue;
			if(row[0].is_null()) //assuming a non-ordered lower is before the ordered ones
				lowers.push_back(make_pair((int) results.size() + 1, row(1)));
			else
				lowers.push_back(make_pair(row(0), row(1)));
			columns.insert(row(1));
		}

		sort(lowers.begin(), lowers.end());
		
		for(int i=0; i<lowers.size(); i++)
			query += ",\n" + query_str(lowers[i].second) + " INTEGER NOT NULL DEFAULT 0";
		
		query += "\n);";
		pqxx::nontransaction write(*rankbase);
		write.exec(query);
		write.commit();
	}
	catch(const pqxx::sql_error e){
		cerr << e.what() << "\n";
		return -1;
	}
	catch(const exception e){
		cerr << e.what() << "\n";
		while(1);
		return -1;
	}
	return 0;
}

//checks if this task wasnt deleted, but submited before that,
//means that e.g. according to contest_round_problems this problem is no longer
//in this round, but was before and some submits were made before problem
//was deleted. Adds the columns if its not there
int check_column_existance(int rank_id, int lower_id)
{
	pqxx::result columns;
	pqxx::nontransaction read(*rankbase);
	columns = read.exec("SELECT count(*) FROM information_schema.columns\n"
						"WHERE table_name = '" + int_to_string(rank_id) + "'\n" +
						"AND column_name = '" + int_to_string(lower_id) + "';");

	if(!columns[0][0].as<int>()) //if it doesnt exist, create it
		read.exec("ALTER TABLE " + query_str(rank_id) + "\n" +
					"ADD COLUMN " + query_str(lower_id) + " INTEGER NOT NULL DEFAULT 0;");

	read.commit();
	return 0;
}

//adds new row of results to specified ranking
int add_result(pqxx::result::tuple row, int rank_id, int lower_id)
{
	int id, partial, total;
	partial = total = 0; //if the values are NULL in database, then we assume its 0 points
	if(row[TEAM_ID].is_null()) //solver is an individual
		id = row(USER_ID);
	else //solver is a team
		id = row(TEAM_ID);

	if(!row[PARTIAL_POINTS].is_null()) 
		partial = row(PARTIAL_POINTS);

	if(!row[TOTAL_POINTS].is_null())
		total = row(TOTAL_POINTS);

	check_column_existance(rank_id, lower_id);

	string columns = "(id, points, " + query_str(lower_id) + ")";
	string values = "(" + int_to_string(id) + ", ";
	values += int_to_string(total + partial * MULTIPL) + ", "; //points for this problem
	values += int_to_string(total + partial * MULTIPL) + ")"; //and the sum
	
	pqxx::nontransaction write(*rankbase);
	write.exec("INSERT INTO " + query_str(rank_id) + " " + columns + "\n" +
				"VALUES " + values + ";");
	write.commit();
	return 0;
}

//updates points in a row of results of the specified ranking
int update_result(pqxx::result::tuple row, int rank_id, int lower_id)
{
	int id, total, partial;
	partial = total = 0; //if the values are NULL in database, then we assume its 0 points
	pqxx::result res;

	if(row[TEAM_ID].is_null()) //solver is an individual
		id = row(USER_ID);
	else //solver is a team
		id = row(TEAM_ID);

	check_column_existance(rank_id, lower_id);

	if(!row[PARTIAL_POINTS].is_null())
		partial = row(PARTIAL_POINTS);

	if(!row[TOTAL_POINTS].is_null())
		total = row(TOTAL_POINTS);

	string new_points = int_to_string(total + partial * MULTIPL);

	pqxx::nontransaction write(*rankbase);
	write.exec("UPDATE " + query_str(rank_id) + "\n" +
				"SET points = points + " + new_points + " - " + query_str(lower_id) + ",\n" +
				query_str(lower_id) + " = " + new_points +
				"WHERE id = " + int_to_string(id) + ";");
	write.commit();
	return 0;
}

//checks whether a user/team with specified id has his/its row of results in 
//specified ranking
int is_solver_in_rank(int solver_id, int rank_id)
{
	pqxx::result rank_results;
	pqxx::nontransaction rank_read(*rankbase);
	string query = "SELECT count(*) FROM " + query_str(rank_id) + "\n" +
									"WHERE id = " + int_to_string(solver_id) + ";";

	rank_results = rank_read.exec("SELECT count(*) FROM " + query_str(rank_id) + "\n" +
									"WHERE id = " + int_to_string(solver_id) + ";");
	rank_read.commit();
	return rank_results[0][0].as<int>();
}

