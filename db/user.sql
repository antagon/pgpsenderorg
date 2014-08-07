--
-- Copyright (c) 2014, PGPSender.org
--

create table if not exists user (
	id integer primary key autoincrement,
	name varchar (32) unique not null,
	password varchar (60) not null,
	primary_email_id integer,
	api_key varchar (64) unique not null,
	created timestamp default CURRENT_TIMESTAMP
);

