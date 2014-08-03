create table if not exists email (
	id bigint unsigned primary key auto_increment,
	name varchar (254) not null,
	user_id bigint unsinged not null,
	created timestamp not null
);

