<?php
/*
 * Copyright (c) 2014, PGPSender.org
 */

require_once ("Config.class.php");

class Email
{
	const TABLE = "email";

	private $db;

	public function __construct ($file)
	{
		$this->db = new Sqlite3 (CONFIG::SQLITE3_DB_FILE);
	}

	public function __destruct ()
	{
		$this->db->close ();
	}

	public function create ($address, $user_id)
	{
		$sql = "INSERT INTO ".$this::TABLE." (name, user_id) VALUES (
					'".$this->db->escapeString ($address)."',
					".intval ($user_id)."
		);";

		return $this->db->exec ($sql);
	}

	public function exists ($address)
	{
		$sql = "SELECT id FROM ".$this::TABLE." WHERE name = '".$this->db->escapeString ($address)."';";

		$res = $this->db->query ($sql);

		if ( $res === false )
			return false;

		$data = $res->fetchArray (SQLITE3_NUM);

		return ($data === false)? false:true;
	}

	public function owns ($address, $user_id)
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
		$sql = "SELECT id, name, created FROM ".$this::TABLE." WHERE user_id = '".intval ($user_id)."';";

		$res = $this->db->query ($sql);

		if ( $res === false )
			return false;

		$rows = array ();

		while ( ($row = $res->fetchArray (SQLITE3_ASSOC)) )
			$rows[] = $row;

		return $rows;
	}

	public function delete ($address, $user_id)
	{
		$sql = "DELETE FROM ".$this::TABLE." WHERE name = '".$this->db->escapeString ($alias)."' AND user_id = ".intval ($user_id).";";

		return $this->db->exec ($sql);
	}
};

?>

