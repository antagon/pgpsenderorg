<?php
/*
 * Copyright (c) 2014, PGPSender.org
 */

require_once ("Config.class.php");

class User
{
	
	const TABLE = "user";
	
	private $db;
	
	public function __construct ($file)
	{
		/*$sql = "CREATE TABLE IF NOT EXISTS ".$this::TABLE." (
						id bigint unsigned primary key auto_increment,
						name varchar (32) unique not null,
						password varchar (60) not null,
						primary_email_id bigint unsigned not null,
						created timestamp not null);";*/

		$this->db = new Sqlite3 (Config::SQLITE3_DB_FILE);
		//$this->db->exec ($sql);
	}
	
	public function __destruct ()
	{
		$this->db->close ();
	}

	private function hash_password ($password)
	{
		return hash ("sha1", $password);
	}

	public function create ($username, $password)
	{
		$sql = "INSERT INTO ".$this::TABLE." (name, password) VALUES (
						'".$this->db->escapeString ($username)."',
						'".$this->hash_password ($password)."'
		);";

		return $this->db->exec ($sql);
	}

	public function passwd ($username, $password)
	{
		$sql = "UPDATE ".$this::TABLE." SET
					password = '".$this->hash_password ($password)."'
				WHERE name = '".$this->db->escapeString ($username)."';";

		return $this->db->exec ($sql);
	}

	public function authenticate ($username, $password)
	{
		$sql = "SELECT * FROM ".$this::TABLE." WHERE name = '".$this->db->escapeString ($username)."' AND password = '".$this->hash_password ($password)."'";

		$res = $this->db->query ($sql);

		if ( $res === false )
			return false;

		return $res->fetchArray (SQLITE3_ASSOC);
	}

	public function exists ($username)
	{
		$sql = "SELECT id FROM ".$this::TABLE." WHERE name = '".$this->db->escapeString ($username)."';";

		$res = $this->db->query ($sql);

		if ( $res === false )
			return false;

		$data = $res->fetchArray (SQLITE3_NUM);

		return ($data === false)? false:true;
	}
	
	public function pull ($limit)
	{
		$sql = "SELECT rowid, recipient, sender, subject, message FROM ".$this::TABLE." LIMIT ".intval ($limit).";";
		$res = $this->db->query ($sql);
		
		if ( $res === false )
			return false;
		
		$rows = array ();
		while ( ($row = $res->fetchArray (SQLITE3_ASSOC) ) )
			$rows[] = $row;
		
		return $rows;
	}
};

?>
