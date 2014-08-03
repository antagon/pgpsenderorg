create if not exists table user (
	id bigint unsigned primary key auto_increment,
	name varchar (32) unique not null,
	password varchar (60) not null,
	primary_email_id bigint unsigned not null,
	created timestamp not null
);

