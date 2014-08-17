<?php

require_once ("../include/ApiResponse.class.php");
require_once ("../include/Config.class.php");
require_once ("../include/User.class.php");
require_once ("../include/Alias.class.php");
require_once ("../include/Email.class.php");

if ( ! isset ($_POST["api_key"]) || empty ($_POST["api_key"]) )
	ApiResponse::json_exit (ApiResponse::E_EMPTY, "API key is empty");

if ( ! isset ($_POST["alias"]) || empty ($_POST["alias"]) )
	ApiResponse::json_exit (ApiResponse::E_EMPTY, "Address is empty");

if ( ! isset ($_POST["address"]) || empty ($_POST["address"]) )
	ApiResponse::json_exit (ApiResponse::E_EMPTY, "Address is empty");

$user_db = new User (Config::SQLITE3_DB_FILE);

// Authenticate with api key
$user_data = $user_db->authenticate_key ($_POST["api_key"]);

if ( $user_data === false )
	ApiResponse::json_exit (ApiResponse::E_UNAUTHORIZED, "Unauthorized");

$alias_db = new Alias (Config::SQLITE3_DB_FILE);

if ( $alias_db->owns ($_POST["alias"], $user_data["id"]) === false )
	ApiResponse::json_exit (ApiResponse::E_NEXISTS, "Alias does not exists");

$email_db = new Email (Config::SQLITE3_DB_FILE);

// If address is "unassigned", do not perform ownership check
if ( $_POST["address"] == "unassigned" ){
	$email_id = null;
} else {
	$email_id = $email_db->owns ($_POST["address"], $user_data["id"]);

	if ( $email_id === false )
		ApiResponse::json_exit (ApiResponse::E_NEXISTS, "Email does not exists");
}

if ( $alias_db->set_email_id ($_POST["alias"], $email_id) === false )
	ApiResponse::json_exit (ApiResponse::E_NEXISTS);

ApiResponse::json_exit (ApiResponse::E_OK);
?>

