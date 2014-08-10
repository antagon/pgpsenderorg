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

foreach ( $emails as $key => $email )
	$emails[$key]["primary"] = ($email["id"] == $user_data["primary_email_id"])? true:false;

ApiResponse::json_exit_data (ApiResponse::E_OK, $emails);
?>

