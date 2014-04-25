DROP TABLE IF EXISTS new_submits CASCADE;

CREATE TABLE new_submits(
	id INTEGER NOT NULL,
	created TIMESTAMP NOT NULL,
	round_id INTEGER,
	user_id INTEGER,
	team_id INTEGER,
	problem_id INTEGER,
	status VARCHAR,
	total_points INTEGER
);

-- Function: submit_added()

-- DROP FUNCTION submit_added();

CREATE OR REPLACE FUNCTION submit_added()
  RETURNS trigger AS
$BODY$
BEGIN
	INSERT INTO new_submits VALUES
	(NEW.id, 
	 NEW.created,
	 NEW.round_id,
	 NEW.user_id,
	 NEW.team_id,
	 NEW.problem_id,
	 NEW.status,
	 NEW.total_points);
	RETURN NULL;
END;
$BODY$
  LANGUAGE plpgsql VOLATILE
  COST 100;
ALTER FUNCTION submit_added()
  OWNER TO oig2; 

DROP TRIGGER IF EXISTS submit_copy ON submits;

CREATE TRIGGER submit_copy
	AFTER INSERT OR UPDATE ON submits
	FOR EACH ROW
	EXECUTE PROCEDURE submit_added();

