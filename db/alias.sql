--
-- Copyright (c) 2014, PGPSender.org
--

create table if not exists alias (
	id integer primary key autoincrement,
	name varchar (254) unique not null,
	email_id integer,
	user_id integer not null,
	created timestamp default CURRENT_TIMESTAMP
);

