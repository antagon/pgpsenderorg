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

$addr_tokens = explode ("@", $_POST["address"]);

if ( in_array ($addr_tokens[1], Config::$hostname) )
	ApiResponse::json_exit (ApiResponse::E_ILEGAL, "Invalid hostname");

$email_db = new Email (Config::SQLITE3_DB_FILE);

if ( $email_db->exists ($_POST["address"]) )
	ApiResponse::json_exit (ApiResponse::E_EXISTS, "Email already exists");

if ( $email_db->create ($_POST["address"], $user_data["id"]) === false )
	ApiResponse::json_exit (ApiResponse::E_INTERNAL);

ApiResponse::json_exit (ApiResponse::E_OK);
?>

