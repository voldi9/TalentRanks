#include "main.hpp"

string int_to_string(int x)
{
	string s = "";
	while(x)
	{
		s = s + char(48 + x%10);
		x/=10;
	}
	return s;
}

bool exists(int id)
{
	return !((map_solvers.find(id) == map_solvers.end()) &&
			(map_rounds.find(id) == map_rounds.end()) &&
			(map_contests.find(id) == map_contests.end()) &&
			(map_stages.find(id) == map_stages.end()) &&
			(map_tasks.find(id) == map_tasks.end()) &&
			(map_submits.find(id) == map_submits.end()));
}

void update_submit(submit * sub)
{
	cout << sub->id << "\n" << ctime(&sub->created) << "\n";
	return;
}

void add_solver(int id, solver * s)
{
	//add and use constructor!
	map_solvers.insert(make_pair(id, s));
	return;
}

void add_problem(int id, task * t)
{
	//add and use constructor!
	map_tasks.insert(make_pair(id, t));
	return;
}

int add_contest(int id, contest * c)
{
	pqxx::result results;
	try{
		pqxx::nontransaction read(*database);
		results = read.exec("SELECT ord, id FROM contest_rounds\n"
							"WHERE contest_id = " + 
							int_to_string(id) + ";");
		vector <pair<int, int>> stages;
		stages.clear();
		//sorting rounds in stage according to database tables
		for(pqxx::result::const_iterator row = results.begin();
     		row != results.end();
     		row++)
     		stages.push_back(make_pair(row[0].as<int>(), row[1].as<int>()));
     	sort(stages.begin(), stages.end());
     	for(int i=0; i<stages.size(); i++)
     		c->stage_ids.push_back(stages[i].second);
	}
	catch(const exception e){
		cerr << e.what();
		return 1;
	}
	map_contests.insert(make_pair(id, c));
	return 0;
}

int add_stage(int id, stage * s)
{
	pqxx::result results;
	try{
		pqxx::nontransaction read(*database);
		results = read.exec("SELECT ord, id FROM contest_rounds\n"
							"WHERE contest_stage_id = " + 
							int_to_string(id) + ";");
		vector <pair<int, int>> rounds;
		rounds.clear();
		//sorting rounds in stage according to database tables
		for(pqxx::result::const_iterator row = results.begin();
     		row != results.end();
     		row++)
     		rounds.push_back(make_pair(row[0].as<int>(), row[1].as<int>()));
     	sort(rounds.begin(), rounds.end());
     	for(int i=0; i<rounds.size(); i++)
     		s->round_ids.push_back(rounds[i].second);
	}
	catch(const exception e){
		cerr << e.what();
		return 1;
	}
	try{ //detecting stages contest
		pqxx::nontransaction read(*database);
		results = read.exec("SELECT contest_id FROM contest_stages\n"
							"WHERE id = " + 
							int_to_string(id) + ";");
		if(results.size())
			if(exists(results[0][0].as<int>()))
			{
				//dodalismy nowy etap pod istniejacy konkurs

			}
			else
			{
				s->c = new contest();
				add_contest(results[0][0].as<int>(), s->c);
			}
	}
	catch(const exception e){
		cerr << e.what();
		return 1;
	}
	map_stages.insert(make_pair(id, s));
	return 0;
}

int add_round(int id, round_ * r)
{
	//add and use constructor!
	pqxx::result results;
	try{ //detecting all problems in this round
		pqxx::nontransaction read(*database);
		results = read.exec("SELECT prefix_order, problem_id FROM contest_round_problems\n"
							"WHERE contest_round_id = " + 
							int_to_string(id) + ";");
		vector <pair<int, int>> problems;
		problems.clear();
		//sorting problems in round according to database tables
		for(pqxx::result::const_iterator row = results.begin();
     		row != results.end();
     		row++)
     		problems.push_back(make_pair(row[0].as<int>(), row[1].as<int>()));
     	sort(problems.begin(), problems.end());
     	for(int i=0; i<problems.size(); i++)
     		r->problem_ids.push_back(problems[i].second);
	}
	catch(const exception e){
		cerr << e.what();
		return 1;
	}

	try{ //detecting this round's stage
		pqxx::nontransaction read(*database);
		results = read.exec("SELECT contest_stage_id FROM contest_rounds\n"
							"WHERE id = " + 
							int_to_string(id) + ";");
		if(results.size())
			if(exists(results[0][0].as<int>()))
			{
				//dodalismy nowa runde pod istniejacy etap

			}
			else
			{
				r->s = new stage();
				add_stage(results[0][0].as<int>(), r->s);
			}
	}
	catch(const exception e){
		cerr << e.what();
		return 1;
	}

	map_rounds.insert(make_pair(id, r));
	return 0;
}
int zz = 2;

//function keeps monitoring the changes in the submits database table
int monitor()
{
	while(true)
	{
		pqxx::result results;
		//check for fresh submits
		try{
			pqxx::nontransaction read(*database);
			results = read.exec("SELECT * FROM new_submits;");
			read.commit();
		}
		catch(const exception e){
			cerr << e.what();
			return 1;
		}

		//process all of them
		if(results.size())
		{
			for(pqxx::result::const_iterator row = results.begin();
     			row != results.end();
     			row++)
     		{
     			//TODO: dodac written_test
     			printf("waj???!!!\n");
     			submit * sub = new submit(row);
     			printf("noł!\n");
     			map_submits.insert(make_pair(sub->id, sub));
     			if(	row[SUBMIT_STATUS].is_null() ||
     				row[SUBMIT_STATUS].as<string>() == CHECKER_UNKN_ERROR ||
     				row[SUBMIT_STATUS].as<string>() == CHECKER_PLAGIARISM ||
     				row[SUBMIT_STATUS].as<string>() == CHECKER_COMP_ERROR ||
     				row[SUBMIT_STATUS].as<string>() == CHECKER_CRIT_COMP_ERROR)	
     			{//zero pkt
     				//delete sub->solved;
     				delete sub;
     				printf("omgomg\n");
     				if(!zz--)
     					return 0;
     				//while(1);
     			}
     			else if(row[SUBMIT_STATUS].as<string>() == CHECKER_CHECKED ||
     					row[SUBMIT_STATUS].as<string>() == CHECKER_WRITTEN_TEST){
     				printf("but wai??\n");
     				update_submit(sub);}
     			else{printf("waj???!!!\n");
     				//nie bierz pod uwage - nieznany status
     				continue;  		}	

     			try{
					pqxx::nontransaction write(*database);
     				write.exec("DELETE FROM new_submits WHERE id = " + 
           						row[SUBMIT_ID].as<string>() + 
           						";"
								);
     				printf("%s\n", row[SUBMIT_ID].as<string>().c_str());
     				write.commit();
     			}
     			catch(const exception e){
					cerr << e.what();
					return 1;
				}

				printf("success!\n\n");
				usleep(SUC);
			}
			//break;
		}
		usleep(CHECK_INTERVAL);
		//puts("checked!\n");
	}
	database->disconnect();
	return 0;
}

int main()
{
	//trying to establish connection to the database
	try{
		database = new pqxx::connection(dbcommand);
		int monitor_success = monitor();
		delete database;
		return monitor_success;
	}
	catch(const exception e){
		cerr << e.what();
		return 1;
	}

	return 0;
}