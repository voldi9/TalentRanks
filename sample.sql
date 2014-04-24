delete from new_submits;
delete from submits where id >= 3232320 AND id < 3232330;
insert into submits (problem_id, user_id, id, total_points, status) values (22631, 90571, 3232320, 10, 'Sprawdzono');
--insert into submits (round_id, problem_id, user_id, id, total_points, status) values (22101, 22631, 90571, 3232320, 10, 'Sprawdzono');
insert into submits (round_id, problem_id, user_id, id, total_points, status) values (22101, 22631, 40471, 3232322, 30, 'PLAGIAT!');
insert into submits (round_id, problem_id, user_id, id, total_points, status) values (22101, 22631, 40771, 3232321, 20, 'Sprawdzono');
insert into submits (round_id, problem_id, user_id, id, total_points, status) values (22101, 22741, 90571, 3232323, 100, 'Sprawdzono');
