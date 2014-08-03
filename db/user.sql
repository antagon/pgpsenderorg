create table if not exists user (
	id integer primary key autoincrement,
	name varchar (32) unique not null,
	password varchar (60) not null,
	primary_email_id integer,
	created timestamp default CURRENT_TIMESTAMP
);

