<?php

require_once ("../include/ApiResponse.class.php");
require_once ("../include/Config.class.php");
require_once ("../include/User.class.php");

if ( ! isset ($_POST["username"]) || empty ($_POST["username"]) )
	ApiResponse::json_exit (ApiResponse::E_EMPTY, "Username is empty");

if ( ! isset ($_POST["password"]) || empty ($_POST["password"]) )
	ApiResponse::json_exit (ApiResponse::E_EMPTY, "Password is empty");

if ( ! isset ($_POST["password_new"]) || empty ($_POST["password_new"]) )
	ApiResponse::json_exit (ApiResponse::E_EMPTY, "New password is empty");

$user_db = new User (Config::SQLITE3_DB_FILE);

$user_data = $user_db->authenticate ($_POST["username"], $_POST["password"]);

if ( $user_data === false )
	ApiResponse::json_exit (ApiResponse::E_UNAUTHORIZED, "Unauthorized");

if ( ! $user_db->passwd ($_POST["username"], $_POST["password_new"]) )
	ApiResponse::json_exit (ApiResponse::E_INTERNAL);

ApiResponse::json_exit (ApiResponse::E_OK);
?>

