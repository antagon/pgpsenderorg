create table if not exists email (
	id integer primary key autoincrement,
	name varchar (254) not null,
	user_id integer not null,
	created timestamp not null
);

