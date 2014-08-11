<?php

require_once ("../include/ApiResponse.class.php");
require_once ("../include/Config.class.php");
require_once ("../include/User.class.php");
require_once ("../include/Alias.class.php");

if ( ! isset ($_POST["api_key"]) || empty ($_POST["api_key"]) )
	ApiResponse::json_exit (ApiResponse::E_EMPTY, "API key is empty");

if ( ! isset ($_POST["alias"]) || empty ($_POST["alias"]) )
	ApiResponse::json_exit (ApiResponse::E_EMPTY, "Address is empty");

if ( ! isset ($_POST["hostname"]) || empty ($_POST["hostname"]) )
	ApiResponse::json_exit (ApiResponse::E_EMPTY, "Hostname is empty");

// Check if user provided the alias in email address format, probably out of confusion.
// We should not punish him for that, but rather fix it for him.
$addr_tokens = explode ("@", $_POST["alias"]);
$_POST["alias"] = $addr_tokens[0];

// Check if hostname exists in configuration
if ( in_array ($_POST["hostname"], Config::$hostname) === false )
	ApiResponse::json_exit (ApiResponse::E_NEXISTS, "Hostname does not exists");

$_POST["alias"] .= "@".$_POST["hostname"];

$user_db = new User (Config::SQLITE3_DB_FILE);

// Authenticate with api key
$user_data = $user_db->authenticate_key ($_POST["api_key"]);

if ( $user_data === false )
	ApiResponse::json_exit (ApiResponse::E_UNAUTHORIZED, "Unauthorized");

$alias_db = new Alias (Config::SQLITE3_DB_FILE);

if ( $alias_db->exists ($_POST["alias"]) )
	ApiResponse::json_exit (ApiResponse::E_EXISTS, "Alias already exists");

if ( $alias_db->create ($_POST["alias"], $user_data["id"]) === false )
	ApiResponse::json_exit (ApiResponse::E_INTERNAL);

ApiResponse::json_exit (ApiResponse::E_OK);
?>

