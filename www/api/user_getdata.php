<?php

require_once ("../include/ApiResponse.class.php");
require_once ("../include/Config.class.php");
require_once ("../include/User.class.php");
require_once ("../include/Email.class.php");
require_once ("../include/Alias.class.php");

if ( ! isset ($_POST["api_key"]) || empty ($_POST["api_key"]) )
	ApiResponse::json_exit (ApiResponse::E_EMPTY, "API key is empty");

$user_db = new User (Config::SQLITE3_DB_FILE);

// Authenticate with api key
$user_data = $user_db->authenticate_key ($_POST["api_key"]);

if ( $user_data === false )
	ApiResponse::json_exit (ApiResponse::E_UNAUTHORIZED, "Unauthorized");

$email_db = new Email (Config::SQLITE3_DB_FILE);
$alias_db = new Alias (Config::SQLITE3_DB_FILE);

$emails = $email_db->get_for_uid ($user_data["id"]);
$aliases = $alias_db->get_for_uid ($user_data["id"]);

if ( $emails === false )
	ApiResponse::json_exit (ApiResponse::E_INTERNAL);

if ( $aliases === false )
	ApiResponse::json_exit (ApiResponse::E_INTERNAL);

$json_array = array ("summary" => array ("address_count" => count ($emails),
									"alias_count" => count ($aliases)),
									"data" => array ());

foreach ( $emails as $email ){

	// Initialize temporary alias array
	$email["alias"] = array ();

	foreach ( $aliases as $alias ){
		if ( $emails["id"] == $alias["email_id"] )
			$email["alias"][] = $alias["name"];
	}

	$json_array["data"][] = array ("name" => $email["name"], "alias" => $email["alias"]);
}

ApiResponse::json_exit_data (ApiResponse::E_OK, $json_array);
?>

