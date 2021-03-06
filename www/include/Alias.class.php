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
		$email_id = ($email_id == null)? "NULL":intval ($email_id);

		$sql = "INSERT INTO ".$this::TABLE." (name, user_id, email_id) VALUES (
						'".$this->db->escapeString ($alias)."',
						".intval ($user_id).",
						NULL
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

	public function owns ($alias, $user_id)
	{
		$sql = "SELECT id FROM ".$this::TABLE." WHERE name = '".$this->db->escapeString ($alias)."' AND user_id = ".intval ($user_id).";";

		$res = $this->db->query ($sql);

		if ( $res === false )
			return false;

		$data = $res->fetchArray (SQLITE3_NUM);

		return ($data === false)? false:true;
	}

	public function get_for_uid ($user_id)
	{
		$sql = "SELECT id, name, email_id, created FROM ".$this::TABLE." WHERE user_id = ".intval ($user_id).";";

		$res = $this->db->query ($sql);

		if ( $res === false )
			return false;

		$rows = array ();

		while ( ($row = $res->fetchArray (SQLITE3_ASSOC)) )
			$rows[] = $row;

		return $rows;
	}

	public function get_for_email_id ($email_id)
	{
		$sql = "SELECT id, name, created FROM ".$this::TABLE." WHERE email_id = ".intval ($email_id).";";

		$res = $this->db->query ($sql);

		if ( $res === false )
			return false;

		$rows = array ();

		while ( ($row = $res->fetchArray (SQLITE3_ASSOC)) )
			$rows[] = $row;

		return $rows;
	}

	public function set_email_id ($alias, $email_id)
	{
		$email_id = ($email_id == null)? "NULL":intval ($email_id);
		$sql = "UPDATE ".$this::TABLE." SET email_id = ".$email_id." WHERE name = '".$this->db->escapeString ($alias)."';";

		return $this->db->exec ($sql);
	}

	public function unset_email_id ($email_id)
	{
		$sql = "UPDATE ".$this::TABLE." SET email_id = NULL WHERE email_id = ".intval ($email_id).";";

		return $this->db->exec ($sql);
	}

	public function delete ($alias, $user_id)
	{
		$sql = "DELETE FROM ".$this::TABLE." WHERE name = '".$this->db->escapeString ($alias)."' AND user_id = ".intval ($user_id).";";

		return $this->db->exec ($sql);
	}
};

?>

