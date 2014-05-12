#include "utils.hpp"

int check_if_rank_in_base(ranking * r)
{
	pqxx::result results;
	//printf("tu!\n");
	try{ //detecting all problems in this round
		pqxx::nontransaction write(*rankbase);
		string query = "CREATE TABLE IF NOT EXISTS \"" + int_to_string(r->id)  + "\"(\n" + 
						"user_id INTEGER NOT NULL,\n"
						"team_id INTEGER NOT NULL,\n"
						"points INTEGER NOT NULL DEFAULT 0";
		for(vector<int>::iterator it = r->lower_ids.begin(); it != r->lower_ids.end(); it++)
			query += ",\n\"" + int_to_string(*it) + "\" INTEGER NOT NULL DEFAULT 0";
		query += "\n);";
		//printf("%s\n", query.c_str());
		write.exec(query);
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
	return 0;
}


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
		printf("%d: last submit %s", it->first, ctime(&it->second->last_submit));
		it->second->print();
	}

	printf("\nstages:\n");
	for(map<int, stage*>::iterator it = map_stages.begin(); it!=map_stages.end(); it++)
	{
		printf("%d: last submit %s", it->first, ctime(&it->second->last_submit));
		it->second->print();
	}

	printf("\ncontests:\n");
	for(map<int, contest*>::iterator it = map_contests.begin(); it!=map_contests.end(); it++)
	{
		printf("%d: last submit %s", it->first, ctime(&it->second->last_submit));
		it->second->print();
	}

	puts("");
}


void clear()
{
	for(map<int, submit*>::iterator it = map_submits.begin(); it!=map_submits.end(); it++)
	{
		submit * sub = it->second;
		if(sub->from_round && sub->from_round->s && sub->from_round->s->c)
			while(!sub->from_round->s->c->rows.empty())
			{
				delete sub->from_round->s->c->rows.back();
				sub->from_round->s->c->rows.pop_back();
			}

		if(sub->from_round && sub->from_round->s)
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