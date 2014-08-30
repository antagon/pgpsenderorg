<?php
session_start ();

require_once ("../include/ApiResponse.class.php");
require_once ("../include/Config.class.php");
require_once ("../include/MailerFacility.class.php");

if ( ! isset ($_POST["recipient"]) || empty ($_POST["recipient"]) )
	ApiResponse::json_exit (ApiResponse::E_EMPTY, "Recipient is empty");

if ( ! isset ($_POST["sender"]) || empty ($_POST["sender"]) )
	ApiResponse::json_exit (ApiResponse::E_EMPTY, "Sender is empty");

if ( ! isset ($_POST["access_token"]) )
	ApiResponse::json_exit (ApiResponse::E_EMPTY, "Access token is empty");

if ( ! isset ($_POST["subject"]) )
	$_POST["subject"] = "No subject";

if ( ! isset ($_POST["message"]) || empty ($_POST["message"]) )
	$_POST["message"] = " ";

if ( ! isset ($_POST["pub_key"]) )
	$_POST["pub_key"] = "";

if ( ! isset ($_SESSION["access_token"]) )
	ApiResponse::json_exit (ApiResponse::E_ILEGAL, "access token");

//
// Check values
//
/*if ( ($_SESSION["hit_time"] + Config::HIT_SPEED_MAX) > time () )
	ApiResponse::json_exit (ApiResponse::E_ILEGAL, "time limit");*/

if ( $_SESSION["access_token"] != $_POST["access_token"] )
	ApiResponse::json_exit (ApiResponse::E_ILEGAL, "access token - ".$_SESSION["access_token"]);

if ( ! filter_var ($_POST["recipient"], FILTER_VALIDATE_EMAIL) )
	ApiResponse::json_exit (ApiResponse::E_ILEGAL, "recipient format");

if ( ! filter_var ($_POST["sender"], FILTER_VALIDATE_EMAIL) )
	ApiResponse::json_exit (ApiResponse::E_ILEGAL, "sender format");

//$_SESSION["hit_time"] = time ();

$email_facility = new MailerFacility ();

if ( ! $email_facility->compose_raw ($_POST["recipient"], Config::$mailer["address"], Config::$mailer["name"],
										$_POST["sender"], $_POST["subject"], $_POST["message"]) )
	ApiResponse::json_exit (ApiResponse::E_INTERNAL);

//
// Import public key
//
if ( ! empty ($_POST["pub_key"]) ){
	// TODO...
}

ApiResponse::json_exit (ApiResponse::E_OK);
?>

