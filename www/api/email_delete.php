<?php

require_once ("../include/ApiResponse.class.php");
require_once ("../include/Config.class.php");
require_once ("../include/User.class.php");
require_once ("../include/Email.class.php");

if ( ! isset ($_POST["api_key"]) || empty ($_POST["api_key"]) )
	ApiResponse::json_exit (ApiResponse::E_EMPTY, "API key is empty");

if ( ! isset ($_POST["address"]) || empty ($_POST["address"]) )
	ApiResponse::json_exit (ApiResponse::E_EMPTY, "Address is empty");

$user_db = new User (Config::SQLITE3_DB_FILE);

// Authenticate with api key
$user_data = $user_db->authenticate_key ($_POST["api_key"]);

if ( $user_data === false )
	ApiResponse::json_exit (ApiResponse::E_UNAUTHORIZED, "Unauthorized");

$email_db = new Email (Config::SQLITE3_DB_FILE);

if ( $email_db->exists ($_POST["address"]) === false )
	ApiResponse::json_exit (ApiResponse::E_NEXISTS, "Email does not exists");

if ( $email_db->delete ($_POST["address"]) === false )
	ApiResponse::json_exit (ApiResponse::E_INTERNAL);

ApiResponse::json_exit (ApiResponse::E_OK);
?>

