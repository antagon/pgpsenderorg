--
-- Copyright (c) 2014, PGPSender.org
--

create table if not exists email (
	id integer primary key autoincrement,
	name varchar (254) unique not null,
	user_id integer not null,
	created timestamp default CURRENT_TIMESTAMP
);

