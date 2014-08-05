<?php

session_start ();

require_once ("../include/ApiResponse.class.php");
require_once ("../include/Config.class.php");
require_once ("../include/User.class.php");

if ( ! isset ($_POST["username"]) || empty ($_POST["username"]) )
	ApiResponse::json_exit (ApiResponse::E_EMPTY, "Username is empty");

if ( ! isset ($_POST["password"]) || empty ($_POST["password"]) )
	ApiResponse::json_exit (ApiResponse::E_EMPTY, "Password is empty");

$user_db = new User (Config::SQLITE3_DB_FILE);

$user_data = $user_db->authenticate ($_POST["username"], $_POST["password"]);

if ( $user_data === false )
	ApiResponse::json_exit (ApiResponse::E_EXISTS, "Authentication failed");

$_SESSION["user"] = $user_data;

ApiResponse::json_exit (ApiResponse::E_OK);
?>

