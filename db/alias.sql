create if not exists table alias (
	id bigint unsigned primary key auto_increment,
	name varchar (254) not null,
	email_id bigint unsinged not null,
	user_id bigint unsinged not null,
	created timestamp not null
);

