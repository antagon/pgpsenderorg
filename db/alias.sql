create table if not exists alias (
	id integer primary key autoincrement,
	name varchar (254) not null,
	email_id integer not null,
	user_id integer not null,
	created timestamp not null
);

