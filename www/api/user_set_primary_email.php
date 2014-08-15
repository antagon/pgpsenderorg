<?php

require_once ("../include/ApiResponse.class.php");
require_once ("../include/Config.class.php");
require_once ("../include/User.class.php");
require_once ("../include/Email.class.php");

if ( ! isset ($_POST["api_key"]) || empty ($_POST["api_key"]) )
	ApiResponse::json_exit (ApiResponse::E_EMPTY, "API key is empty");

if ( ! isset ($_POST["address"]) || empty ($_POST["address"]) )
	ApiResponse::json_exit (ApiResponse::E_EMPTY, "Address is empty");

if ( filter_var($_POST["address"], FILTER_VALIDATE_EMAIL) === false )
	ApiResponse::json_exit (ApiResponse::E_FORMAT, "Invalid address format");

$user_db = new User (Config::SQLITE3_DB_FILE);

// Authenticate with api key
$user_data = $user_db->authenticate_key ($_POST["api_key"]);

if ( $user_data === false )
	ApiResponse::json_exit (ApiResponse::E_UNAUTHORIZED, "Unauthorized");

$email_db = new Email (Config::SQLITE3_DB_FILE);

$email_id = $email_db->owns ($_POST["address"], $user_data["id"]);

if ( $email_id === false )
	ApiResponse::json_exit (ApiResponse::E_NEXISTS, "Address does not exists");

if ( $user_db->set_primary_email_id ($user_data["name"], $email_id) === false )
	ApiResponse::json_exit (ApiResponse::E_INTERNAL);

ApiResponse::json_exit (ApiResponse::E_OK);
?>

