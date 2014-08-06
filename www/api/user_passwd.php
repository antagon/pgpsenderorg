<?php

require_once ("../include/ApiResponse.class.php");
require_once ("../include/Config.class.php");
require_once ("../include/User.class.php");

if ( ! isset ($_POST["api_key"]) || empty ($_POST["api_key"]) )
	ApiResponse::json_exit (ApiResponse::E_EMPTY, "Username is empty");

if ( ! isset ($_POST["password"]) || empty ($_POST["password"]) )
	ApiResponse::json_exit (ApiResponse::E_EMPTY, "Password is empty");

if ( ! isset ($_POST["password_new"]) || empty ($_POST["password_new"]) )
	ApiResponse::json_exit (ApiResponse::E_EMPTY, "New password is empty");

$user_db = new User (Config::SQLITE3_DB_FILE);

// Authenticate with api key
$user_data = $user_db->authenticate_key ($_POST["api_key"]);

if ( $user_data === false )
	ApiResponse::json_exit (ApiResponse::E_UNAUTHORIZED, "Unauthorized");

// FIXME: a second authentication query should be avoided. At this moment it is here
// only to verify current password in case of stolen api key.
if ( $user_db->authenticate ($user_data["name"], $_POST["password"]) === false )
	ApiResponse::json_exit (ApiResponse::E_UNAUTHORIZED, "Unauthorized");

if ( $user_db->passwd ($user_data["name"], $_POST["password_new"]) === false )
	ApiResponse::json_exit (ApiResponse::E_INTERNAL);

ApiResponse::json_exit (ApiResponse::E_OK);
?>

