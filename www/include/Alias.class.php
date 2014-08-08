<?php
/*
 * Copyright (c) 2014, PGPSender.org
 */

require_once ("Config.class.php");

class Alias
{
	
	const TABLE = "alias";
	
	private $db;
	
	public function __construct ($file)
	{
		$this->db = new Sqlite3 (Config::SQLITE3_DB_FILE);
	}
	
	public function __destruct ()
	{
		$this->db->close ();
	}

	public function create ($alias, $user_id, $email_id = null)
	{
		$email_id = ($email_id == null)? null:intval ($email_id);

		$sql = "INSERT INTO ".$this::TABLE." (name, user_id) VALUES (
						'".$this->db->escapeString ($alias)."',
						".intval ($user_id).",
						".$email_id."
		);";

		return $this->db->exec ($sql);
	}

	public function exists ($alias)
	{
		$sql = "SELECT id FROM ".$this::TABLE." WHERE name = '".$this->db->escapeString ($alias)."';";

		$res = $this->db->query ($sql);

		if ( $res === false )
			return false;

		$data = $res->fetchArray (SQLITE3_NUM);

		return ($data === false)? false:true;
	}

public function get_for_uid ($user_id)
	{
		$sql = "SELECT id, name, created FROM ".$this::TABLE." WHERE user_id = '".intval ($user_id)."';";

		$res = $this->db->query ($sql);

		if ( $res === false )
			return false;

		$rows = array ();

		while ( ($row = $res->fetchArray (SQLITE3_ASSOC)) )
			$rows[] = $row;

		return $rows;
	}

	public function delete ($alias)
	{
		$sql = "DELETE FROM ".$this::TABLE." WHERE name = '".$this->db->escapeString ($alias)."';";

		return $this->db->exec ($sql);
	}
};

?>

