<?php

require_once ("../include/ApiResponse.class.php");
require_once ("../include/Config.class.php");
require_once ("../include/User.class.php");
require_once ("../include/Email.class.php");

if ( ! isset ($_POST["api_key"]) || empty ($_POST["api_key"]) )
	ApiResponse::json_exit (ApiResponse::E_EMPTY, "API key is empty");

$user_db = new User (Config::SQLITE3_DB_FILE);

// Authenticate with api key
$user_data = $user_db->authenticate_key ($_POST["api_key"]);

if ( $user_data === false )
	ApiResponse::json_exit (ApiResponse::E_UNAUTHORIZED, "Unauthorized");

$email_db = new Email (Config::SQLITE3_DB_FILE);

$emails = $email_db->get_for_uid ($user_data["id"]);

if ( $emails === false )
	ApiResponse::json_exit (ApiResponse::E_INTERNAL);

$data = array ();

foreach ( $emails as $email ){
	$data[] = array ("address" => $email["name"], "alias" => array ());
}

/*$data = array (
	array ("address" => "nobody@example.com", "alias" => array ("b88d40f311aa0591170a82e9147d3d9c@pgpsender.org", "2be852db88abf04941728fa41cf197ff@pgpsender.org")),
	array ("address" => "johndoe@example.com", "alias" => array ("3fe7c4bd69d4b53cffe0a9dbe1db6027@pgpsender.org"))
);*/

ApiResponse::json_exit_data (ApiResponse::E_OK, $data);
?>

