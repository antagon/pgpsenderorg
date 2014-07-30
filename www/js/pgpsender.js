/*
 * Copyright (c) 2013, PGPSender.org
 */

//
// Check if the string is valid PGP key
// This function is a workaround as openpgp.read_publickey fails to validate the string passed into it.
//
function is_pubkey (key_str)
{
	var key_str = $.trim (key_str);
	var b = key_str.split ("-----");
	
	if ( b.length == 1 )
		return false;
	
	if ( ! b[1].match (/BEGIN PGP PUBLIC KEY BLOCK/) )
		return false;
	
	return true;
}

//
// Return key object
//
function read_pubkey (key_str)
{
	if ( ! is_pubkey (key_str) )
		return null;
	
	var public_key = openpgp.read_publicKey ($.trim (key_str));
	
	return public_key;
}

//
// Return encrypted message
//
function encrypt (key, text)
{
	return openpgp.write_encrypted_message (key, text);
}

//
// Reference: http://stackoverflow.com/questions/2855865/jquery-regex-validation-of-e-mail-address
//
function is_email (emailAddress) {
    var pattern = new RegExp (/^((([a-z]|\d|[!#\$%&'\*\+\-\/=\?\^_`{\|}~]|[\u00A0-\uD7FF\uF900-\uFDCF\uFDF0-\uFFEF])+(\.([a-z]|\d|[!#\$%&'\*\+\-\/=\?\^_`{\|}~]|[\u00A0-\uD7FF\uF900-\uFDCF\uFDF0-\uFFEF])+)*)|((\x22)((((\x20|\x09)*(\x0d\x0a))?(\x20|\x09)+)?(([\x01-\x08\x0b\x0c\x0e-\x1f\x7f]|\x21|[\x23-\x5b]|[\x5d-\x7e]|[\u00A0-\uD7FF\uF900-\uFDCF\uFDF0-\uFFEF])|(\\([\x01-\x09\x0b\x0c\x0d-\x7f]|[\u00A0-\uD7FF\uF900-\uFDCF\uFDF0-\uFFEF]))))*(((\x20|\x09)*(\x0d\x0a))?(\x20|\x09)+)?(\x22)))@((([a-z]|\d|[\u00A0-\uD7FF\uF900-\uFDCF\uFDF0-\uFFEF])|(([a-z]|\d|[\u00A0-\uD7FF\uF900-\uFDCF\uFDF0-\uFFEF])([a-z]|\d|-|\.|_|~|[\u00A0-\uD7FF\uF900-\uFDCF\uFDF0-\uFFEF])*([a-z]|\d|[\u00A0-\uD7FF\uF900-\uFDCF\uFDF0-\uFFEF])))\.)+(([a-z]|[\u00A0-\uD7FF\uF900-\uFDCF\uFDF0-\uFFEF])|(([a-z]|[\u00A0-\uD7FF\uF900-\uFDCF\uFDF0-\uFFEF])([a-z]|\d|-|\.|_|~|[\u00A0-\uD7FF\uF900-\uFDCF\uFDF0-\uFFEF])*([a-z]|[\u00A0-\uD7FF\uF900-\uFDCF\uFDF0-\uFFEF])))\.?$/i);
    return pattern.test (emailAddress);
}

function fetch_key ()
{
	var recipient = $("#recipient").val ();
	var sender = $("#sender").val ();
	
	if ( is_email (recipient) == false || is_email (sender) == false )
		return;
	
	$.post ("fetch_key.php", { "recipient": recipient, "sender": sender }, function (response){
		try {
			response = $.parseJSON (response);
		} catch (error){
			alert (response);
		}
		
		if ( response.status == "ok" ){
			$("#recipient_pub_key").val (response.message);
			$("#ntf_key_found").show ();
		} else {
			$("#recipient_pub_key").val ("");
			$("#ntf_key_found").hide ();
		}
	});
}

//
// Show preview
//
$("#send").click (function (){
	var recipient = $("#recipient").val ().toLowerCase ();
	var sender = $("#sender").val ().toLowerCase ();
	var subject = $("#subject").val ();
	var error = false;
	var public_key = null;
	
	$(".error").hide ();
	
	if ( is_email (recipient) == false ){
		$("#err_recipient").show ();
		error = true;
	}
	
	if ( is_email (sender) == false ){
		$("#err_sender").show ();
		error = true;
	}
	
	if ( $("#message").val ().length == 0 ){
		$("#err_message").show ();
		error = true;
	}
	
	if ( subject.length == 0 )
		subject = "Blank";

	if ( error )
		return;
	
	$("#mailsender_modal").hide ();
	$("#preview_modal").show ();
	$("#preview_recipient").text (recipient);
	$("#preview_sender").text (sender);
	$("#preview_subject").text (subject);
	$("#preview_modal_store_key").prop ("checked", false);
	
	if ( $("#recipient_pub_key").val ().length > 0 )
		public_key = read_pubkey ($("#recipient_pub_key").val ());
	
	if ( public_key != null ){
		$("#preview_store_key_option").show ();
		$("#preview_message").text (encrypt (public_key, $("#message").val ()));
	} else {
		$("#preview_store_key_option").hide ();
		$("#preview_message").text ($("#message").val ());
	}
});

$("#sender_modal_raise").click (function (){
	$("#mailsender_modal").hide ();
	$("#sender_pub_key_modal").show ();
	$("#sender_pub_key").select ();
});

$("#recipient_modal_raise").click (function (){
	$("#mailsender_modal").hide ();
	$("#recipient_pub_key_modal").show ();
	$("#recipient_pub_key").select ();
});

$("#sender_modal_ok").click (function (){
	$("#sender_pub_key_modal").hide ();
	$("#mailsender_modal").show ();
});

$("#recipient_modal_ok").click (function (){
	$("#recipient_pub_key_modal").hide ();
	$("#mailsender_modal").show ();
});

$("#preview_modal_cancel").click (function (){
	$("#preview_modal").hide ();
	$("#mailsender_modal").show ();
});

$("#success_modal_another").click (function (){
	$("#success_modal").hide ();
	$("#mailsender_modal").show ();
	$("#recipient").select ();
});

$("#error_modal_resend").click (function (){
	$("#preview_modal_ok").trigger ("click");
});

// Actual send button
$("#preview_modal_ok").click (function (){
	var recipient = $("#recipient").val ().toLowerCase ();
	var sender = $("#sender").val ().toLowerCase ();
	var subject = $("#subject").val ();
	var message = $("#message").val ();
	var store_key = ( $("#preview_modal_store_key").prop ("checked") && is_pubkey ($("#recipient_pub_key").val ()) );
	var token = $("#token").val ();
	var public_key = null;
	
	$(".notice").hide ();
	
	if ( $("#recipient_pub_key").val ().length > 0 )
		public_key = read_pubkey ($("#recipient_pub_key").val ());
	
	if ( public_key != null )
		message = encrypt (public_key, message);
	
	$.post ("queuemail.php", { "recipient": recipient, "sender": sender, "subject": subject, "message": message, "token": token, "pub_key": ((store_key)? $("#recipient_pub_key").val ():"") }, function (response){
		try {
			response = $.parseJSON (response);
		} catch (error){
			alert (response);
		}
		
		if ( response.status == "ok" ){
			$("#preview_modal").hide ();
			$("#success_modal").show ();
			$(".maildata").val ("");
		} else {
			$("#preview_modal").hide ();
			$("#error_modal").show ();
		}
	});
});

$(document).ready (function (){
	if ( ! window.crypto.getRandomValues )
		alert ("Sorry, your browser does not support OpenPGP.js!");

	openpgp.init ();
	openpgp.config.commentstring = "http://pgpsender.org";
	
	if ( $("#recipient").val ().length == 0 )
		$("#recipient").select ();
	else if ( $("#sender").val ().length == 0 )
		$("#sender").select ();
	else if ( $("#subject").val ().length == 0 )
		$("#subject").select ();
	else
		$("#message").select ();
	
	fetch_key ();
	
	$("#recipient").change (function (){ fetch_key (); });
	$("#sender").change (function (){ fetch_key (); });
	$("#recipient_pub_key").change (function (){ $("#ntf_key_found").hide (); });
	
	$(window).keypress (function (event){
		if ( event.keyCode == 27 ){
			$(".modal").hide ();
			$("#mailsender_modal").show ();
		}
	});
});
