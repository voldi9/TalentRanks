#include "main.hpp"

//TODO: drop function
//TODO: autodrop unused rankings
//TODO: implement partial points coverage!

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
			row != results.end(); row++)
			if(row[0].is_null()) //assuming a non-ordered lower is before the ordered ones
				stages.push_back(make_pair(-1, row(1)));
			else
				stages.push_back(make_pair(row(0), row(1)));
		
		sort(stages.begin(), stages.end());
		for(int i=0; i<stages.size(); i++)
			c->lower_ids.push_back(stages[i].second);
	}
	catch(const pqxx::sql_error e)
	{
		cerr << e.what() << "\n";
		return -1;
	}
	catch(const exception e){
		cerr << e.what() << "\n";
		return -1;
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
			row != results.end(); row++)
			if(row[0].is_null()) //assuming a non-ordered lower is before the ordered ones
				rounds.push_back(make_pair(-1, row(1)));
			else
				rounds.push_back(make_pair(row(0), row(1)));
		

		sort(rounds.begin(), rounds.end());
		for(int i=0; i<rounds.size(); i++)
			s->lower_ids.push_back(rounds[i].second);
	}
	catch(const pqxx::sql_error e)
	{
		cerr << e.what() << "\n";
		return -1;
	}
	catch(const exception e){
		cerr << e.what() << "\n";
		return -1;
	}
	try{ //detecting stages contest
		pqxx::nontransaction read(*database);
		results = read.exec("SELECT contest_id FROM contest_stages\n"
							"WHERE id = " + 
							int_to_string(s->id) + ";");
		read.commit();
		if(results.size()) //assuming a stage only belongs to one contest
			if(map_contests.find(results[0][0].as<int>()) != map_contests.end())
			{
				//dodalismy nowy etap pod istniejacy konkurs

			}
			else
			{
				s->c = new contest(results[0][0].as<int>());
				add_contest(s->c);
			}
	}
	catch(const pqxx::sql_error e)
	{
		cerr << e.what() << "\n";
		return -1;
	}
	catch(const exception e){
		cerr << e.what() << "\n";
		return -1;
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
			row != results.end(); row++)
			if(row[0].is_null()) //assuming a non-ordered lower is before the ordered ones
				problems.push_back(make_pair(-1, row(1)));
			else
				problems.push_back(make_pair(row(0), row(1)));
		
		sort(problems.begin(), problems.end());
		for(int i=0; i<problems.size(); i++)
			r->lower_ids.push_back(problems[i].second);
	}
	catch(const pqxx::sql_error e)
	{
		cerr << e.what() << "\n";
		return -1;
	}
	catch(const exception e){
		cerr << e.what() << "\n";
		return -1;
	}
	
	try{ //detecting this round's stage
		pqxx::nontransaction read(*database);
		results = read.exec("SELECT contest_stage_id FROM contest_rounds\n"
							"WHERE id = " + 
							int_to_string(r->id) + ";");
		read.commit();

		if(results.size()) //assuming a round only belongs to one stage
			if(map_stages.find(results[0][0].as<int>()) != map_stages.end())
			{
				//dodalismy nowa runde pod istniejacy etap

			}
			else
			{
				r->s = new stage(results[0][0].as<int>());
				add_stage(r->s);
			}
	}
	catch(const pqxx::sql_error e)
	{
		cerr << e.what() << "\n";
		return -1;
	}
	catch(const exception e){
		cerr << e.what() << "\n";
		return -1;
	}

	map_rounds.insert(make_pair(r->id, r));
	printf("abc %d\n", r->id);
	return 0;
}

//int zz = 2;

void handle(submit * sub, string status)
{
	//sub->add();
	if(!sub->solved || !sub->problem || !sub->from_round) //unknown solver's, round's or problem's id
	{
		puts("OH YEAH!!\n");
		delete sub;
		return;
	}

	check_if_rank_in_base(sub->from_round->id, ROUND);
	if(sub->from_round->s)
		check_if_rank_in_base(sub->from_round->s->id, STAGE);
	if(sub->from_round->s && sub->from_round->s->c)
		check_if_rank_in_base(sub->from_round->s->c->id, CONTEST);

	map_submits.insert(make_pair(sub->id, sub));
	sub->from_round->add_user(sub->solved);

	if(status == "" ||
		status == CHECKER_UNKN_ERROR ||
		status == CHECKER_PLAGIARISM ||
		status == CHECKER_COMP_ERROR ||
		status == CHECKER_CRIT_COMP_ERROR)	
	{
		sub->total_points = 0;
		sub->add();
		update_submit(sub); //usun
	}
	else if(status == CHECKER_CHECKED ||
			status == CHECKER_WRITTEN_TEST)
	{
		sub->add();
		update_submit(sub);
	}
	else //nie bierz pod uwage - nieznany status
		return;  

	printf("success!\n\n");
	usleep(SUCCESS);
}
int zz = 0;
//function keeps monitoring the changes in the submits database table
int monitor()
{
	//---------------UNCOMMENT!!!!
	while(true)
	{
		zz++;
		semop(sem_id, &P, 1);
		pqxx::result results;
		//check for fresh submits
		try{
			pqxx::nontransaction read(*database);
			results = read.exec("SELECT * FROM new_submits;");
			read.commit();
		}
		catch(const pqxx::sql_error e)
		{
			cerr << e.what() << "\n";
			return -1;
		}
		catch(const exception e){
			cerr << e.what() << "\n";
			return -1;
		}

		//process all of them
		if(results.size())
		{
			for(pqxx::result::const_iterator row = results.begin();
				row != results.end(); row++)
			{
				//TODO: dodac written_test
				submit * sub;

				if(map_submits.find(row(SUBMIT_ID)) == map_submits.end())
					sub = new submit(row);
				else
					sub = map_submits[row(SUBMIT_ID)];

				try{
					pqxx::nontransaction write(*database);
					write.exec("DELETE FROM new_submits WHERE id = " + 
							row[SUBMIT_ID].as<string>() + 
							";");
					printf("%s\n", row[SUBMIT_ID].as<string>().c_str());
					write.commit();
				}
				catch(const pqxx::sql_error e)
				{
					cerr << e.what() << "\n";
					return -1;
				}
				catch(const exception e){
					cerr << e.what() << "\n";
					return -1;
				}

				if(row[SUBMIT_STATUS].is_null())
					handle(sub, "");
				else
					handle(sub, row[SUBMIT_STATUS].as<string>());
			}
		}
		semop(sem_id, &V, 1);
		printf("SUCCESS! %d\n", zz);
		usleep(CHECK_INTERVAL);
	}
	//database->disconnect();
	return 0;
}

#define RANK_ID 0
#define RANK_SUB_ID 1
#define RANK_ROUND_ID 2
#define RANK_USER_ID 3
#define RANK_TEAM_ID 4
#define RANK_PROB_ID 5
#define RANK_STATUS 6
#define RANK_POINTS 7
/*
	BEZSENS :::: !!!!!!!!!!!
*/
int pickup_ranking(ranking * r)
{
	/*
	pqxx::result results;
	try{
		printf("ok\n");
		pqxx::nontransaction read(*database);
		results = read.exec("SELECT submit_id FROM rank_caches\n"
							"WHERE contest_round_id = " + 
							int_to_string(r->id) + ";");
		read.commit();	
		printf("//\\ %d\n", results.size());
		for(pqxx::result::const_iterator row = results.begin();
			row != results.end(); row++)
		{

		}
	}
	catch(const pqxx::sql_error e)
	{
		cerr << e.what() << "\n";
		return -1;
	}
	catch(const exception e){
		printf("nie ok\n");
		cerr << e.what() << "\n";
		return -1;
	}*/
	return 0;
}

int pickup_round(int id)
{
	pqxx::result results;
	/*
	try{
		pqxx::nontransaction read(*database);
		read.exec("SELECT id FROM contest_rounds WHERE id = " + 
					int_to_string(id) + ";");
		read.commit();
		if(!results.size()) //no round with such an id
			return -1;
	}
	catch(const pqxx::sql_error e)
	{
		cerr << e.what() << "\n";
		return -1;
	}
	catch(const exception e){
		cerr << e.what() << "\n";
		return -1;
	}*/
	puts("dafuq?!\n");
	//this round may not exist at all!!!
	round_ * r = new round_(id);
	add_round(r);
	//uncomment below and r must be a ranking
	//pickup_ranking(r);
}


int main()
{
	//trying to establish connection to the database
	try{
		database = new pqxx::connection(dbparams);
		rankbase = new pqxx::connection(rankdbparams);
		sem_id = semget(SEM_KEY, 1, 0666 | IPC_CREAT);
		semctl(sem_id, 0, SETVAL, 1); //set the semaphore value to 1
		//printf("%d\n", sem_id);
		//printf("! %d\n", semctl(sem_id, 0, GETVAL));
		int monitor_success = monitor();
		//return monitor_success;
	}
	catch(const pqxx::sql_error e)
	{
		cerr << e.what() << "\n";
		return -1;
	}
	catch(const exception e){
		cerr << e.what() << "\n";
		return -1;
	}
	//pickup_round(22101);
	print_maps();
	clear();
	clean_up();
	return 0;
}