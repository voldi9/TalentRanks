#include "main.hpp"

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

void add_solver(solver * s)
{
	//add and use constructor!
	map_solvers.insert(make_pair(s->id, s));
	return;
}

void add_problem(task * t)
{
	//add and use constructor!
	map_tasks.insert(make_pair(t->id, t));
	return;
}

int add_contest(contest * c)
{
	pqxx::result results;
	try{
		pqxx::nontransaction read(*database);
		results = read.exec("SELECT ord, id FROM contest_stages\n"
							"WHERE contest_id = " + 
							int_to_string(c->id) + ";");
		read.commit();
		vector <pair<int, int>> stages;
		stages.clear();
		//sorting rounds in stage according to database tables
		for(pqxx::result::const_iterator row = results.begin();
     		row != results.end();
     		row++)
     		stages.push_back(make_pair(row[0].as<int>(), row[1].as<int>()));
     	sort(stages.begin(), stages.end());
     	for(int i=0; i<stages.size(); i++)
     		c->lower_ids.push_back(stages[i].second);
	}
	catch(const exception e){
		cerr << e.what();
		return 1;
	}
	map_contests.insert(make_pair(c->id, c));
	return 0;
}

int add_stage(stage * s)
{
	pqxx::result results;
	try{
		pqxx::nontransaction read(*database);
		results = read.exec("SELECT ord, id FROM contest_rounds\n"
							"WHERE contest_stage_id = " + 
							int_to_string(s->id) + ";");
		read.commit();
		vector <pair<int, int>> rounds;
		rounds.clear();
	
		//sorting rounds in stage according to database tables
		for(pqxx::result::const_iterator row = results.begin();
     		row != results.end();
     		row++)
     		rounds.push_back(make_pair(row[0].as<int>(), row[1].as<int>()));
     	sort(rounds.begin(), rounds.end());
     	for(int i=0; i<rounds.size(); i++)
     		s->lower_ids.push_back(rounds[i].second);
	}
	catch(const exception e){
		cerr << e.what();
		return 1;
	}
	try{ //detecting stages contest
		pqxx::nontransaction read(*database);
		results = read.exec("SELECT contest_id FROM contest_stages\n"
							"WHERE id = " + 
							int_to_string(s->id) + ";");
		read.commit();
		if(results.size()) //assuming a stage only belongs to one contest
			if(exists(results[0][0].as<int>()))
			{
				//dodalismy nowy etap pod istniejacy konkurs

			}
			else
			{
				s->c = new contest(results[0][0].as<int>());
				add_contest(s->c);
			}
	}
	catch(const exception e){
		cerr << e.what();
		return 1;
	}
	map_stages.insert(make_pair(s->id, s));
	return 0;
}

int add_round(round_ * r)
{
	//add and use constructor!
	pqxx::result results;
	try{ //detecting all problems in this round
		pqxx::nontransaction read(*database);
		results = read.exec("SELECT ord, problem_id FROM contest_round_problems\n"
							"WHERE contest_round_id = " + 
							int_to_string(r->id) + ";");
		read.commit();
		vector <pair<int, int>> problems;
		problems.clear();

		//sorting problems in round according to database tables
		for(pqxx::result::const_iterator row = results.begin();
     		row != results.end();
     		row++)
     		problems.push_back(make_pair(row[0].as<int>(), row[1].as<int>()));
     	sort(problems.begin(), problems.end());
     	for(int i=0; i<problems.size(); i++)
     		r->lower_ids.push_back(problems[i].second);
	}
	catch(const exception e){
		cerr << e.what();
		return 1;
	}
	
	try{ //detecting this round's stage
		pqxx::nontransaction read(*database);
		results = read.exec("SELECT contest_stage_id FROM contest_rounds\n"
							"WHERE id = " + 
							int_to_string(r->id) + ";");
		read.commit();

		if(results.size()) //assuming a round only belongs to one stage
			if(exists(results[0][0].as<int>()))
			{
				//dodalismy nowa runde pod istniejacy etap

			}
			else
			{
				r->s = new stage(results[0][0].as<int>());
				add_stage(r->s);
			}
	}
	catch(const exception e){
		cerr << e.what();
		return 1;
	}

	map_rounds.insert(make_pair(r->id, r));
	printf("abc %d\n", r->id);
	return 0;
}

int zz = 3;


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
     			submit * sub = new submit(row);
     			//sub->add();
     			if(!sub->solved || !sub->problem || !sub->from_round) //unknown solver's, round's or problem's id
     			{
     				delete sub;
     				continue;
     			}
     			map_submits.insert(make_pair(sub->id, sub));
     			sub->from_round->add_user(sub->solved);

     			if(	row[SUBMIT_STATUS].is_null() ||
     				row[SUBMIT_STATUS].as<string>() == CHECKER_UNKN_ERROR ||
     				row[SUBMIT_STATUS].as<string>() == CHECKER_PLAGIARISM ||
     				row[SUBMIT_STATUS].as<string>() == CHECKER_COMP_ERROR ||
     				row[SUBMIT_STATUS].as<string>() == CHECKER_CRIT_COMP_ERROR)	
     			{
     				sub->total_points = 0;
     				sub->add();
     				update_submit(sub); //usun

     				if(!zz--)
     				{
     					sub->from_round->print();
     					return 0;
     				}
     			}
     			else if(row[SUBMIT_STATUS].as<string>() == CHECKER_CHECKED ||
     					row[SUBMIT_STATUS].as<string>() == CHECKER_WRITTEN_TEST)
     			{
     				sub->add();
     				if(!zz--)
     				{
     					sub->from_round->print();
     					return 0;
     				}
     				update_submit(sub);
     			}
     			else
     				//nie bierz pod uwage - nieznany status
     				continue;  

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
		}
		usleep(CHECK_INTERVAL);
	}
	//database->disconnect();
	return 0;
}

void print_maps()
{
	printf("\nsubmits:\n");
	for(map<int, submit*>::iterator it = map_submits.begin(); it!=map_submits.end(); it++)
	{
		printf("%d, ", it->first);
		//it->second->print();
	}

	printf("\nrounds:\n");
	for(map<int, round_*>::iterator it = map_rounds.begin(); it!=map_rounds.end(); it++)
	{
		printf("%d:\n", it->first);
		it->second->print();
	}

	printf("\nstages:\n");
	for(map<int, stage*>::iterator it = map_stages.begin(); it!=map_stages.end(); it++)
	{
		printf("%d:\n", it->first);
		it->second->print();
	}

	printf("\ncontests:\n");
	for(map<int, contest*>::iterator it = map_contests.begin(); it!=map_contests.end(); it++)
	{
		printf("%d:\n", it->first);
		it->second->print();
	}

	puts("");
}


void clear()
{
	for(map<int, submit*>::iterator it = map_submits.begin(); it!=map_submits.end(); it++)
	{
		submit * sub = it->second;
		if(sub->from_round->s->c)
			while(!sub->from_round->s->c->rows.empty())
			{
				delete sub->from_round->s->c->rows.back();
				sub->from_round->s->c->rows.pop_back();
			}

		if(sub->from_round->s)
			while(!sub->from_round->s->rows.empty())
			{
				delete sub->from_round->s->rows.back();
				sub->from_round->s->rows.pop_back();
			}


		if(sub->from_round)
			while(!sub->from_round->rows.empty())
			{
				delete sub->from_round->rows.back();
				sub->from_round->rows.pop_back();
			}
	}

	while(!map_solvers.empty())
		delete map_solvers.begin()->second;

	while(!map_tasks.empty())
		delete map_tasks.begin()->second;

	while(!map_rounds.empty())
		delete map_rounds.begin()->second;

	while(!map_stages.empty())
		delete map_stages.begin()->second;

	while(!map_contests.empty())
		delete map_contests.begin()->second;

	while(!map_submits.empty())
		delete map_submits.begin()->second;
}

/*
	BEZSENS :::: !!!!!!!!!!!
*/
int pickup_ranking(round_ * r)
{
	pqxx::result results;
	try{
		pqxx::nontransaction read(*database);
     	read.exec("SELECT id FROM contest_rounds WHERE id = " + 
					int_to_string(r->id) + ";");
     	read.commit();
     	if(!results.size()) //no round with such an id
     		return 1;
	}
	catch(const exception e){
		cerr << e.what();
		return 1;
	}
}

int pickup_round(int id)
{
	pqxx::result results;
	try{
		pqxx::nontransaction read(*database);
     	read.exec("SELECT id FROM contest_rounds WHERE id = " + 
					int_to_string(id) + ";");
     	read.commit();
     	if(!results.size()) //no round with such an id
     		return 1;
	}
	catch(const exception e){
		cerr << e.what();
		return 1;
	}
		round_ * r = new round_(id);
		add_round(r);
		pickup_ranking(r);
}

int main()
{
	//trying to establish connection to the database
	try{
		database = new pqxx::connection(dbcommand);
		int monitor_success = monitor();
		//return monitor_success;
	}
	catch(const exception e){
		cerr << e.what();
		return 1;
	}
	print_maps();
	clear();
	//pickup_round(-10);
	delete database; //wloz do try{} wyzej
	return 0;
}