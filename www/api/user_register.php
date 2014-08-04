<?php

require_once ("../include/ApiResponse.class.php");
require_once ("../include/Config.class.php");
require_once ("../include/User.class.php");

if ( ! isset ($_POST["username"]) || empty ($_POST["username"]) )
	ApiResponse::json_exit (ApiResponse::E_EMPTY, "Username is empty");

if ( ! isset ($_POST["password"]) || empty ($_POST["password"]) )
	ApiResponse::json_exit (ApiResponse::E_EMPTY, "Password is empty");

$user_db = new User (Config::SQLITE3_DB_FILE);

if ( $user_db->exists ($_POST["username"]) )
	ApiResponse::json_exit (ApiResponse::E_EXISTS, "Username already exists");

$user_db->create ($_POST["username"], $_POST["password"]);

ApiResponse::json_exit (ApiResponse::E_OK);
?>

