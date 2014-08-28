// Reference: http://stackoverflow.com/questions/2855865/jquery-regex-validation-of-e-mail-address
function is_email (emailAddress) {
	var pattern = new RegExp (/^((([a-z]|\d|[!#\$%&'\*\+\-\/=\?\^_`{\|}~]|[\u00A0-\uD7FF\uF900-\uFDCF\uFDF0-\uFFEF])+(\.([a-z]|\d|[!#\$%&'\*\+\-\/=\?\^_`{\|}~]|[\u00A0-\uD7FF\uF900-\uFDCF\uFDF0-\uFFEF])+)*)|((\x22)((((\x20|\x09)*(\x0d\x0a))?(\x20|\x09)+)?(([\x01-\x08\x0b\x0c\x0e-\x1f\x7f]|\x21|[\x23-\x5b]|[\x5d-\x7e]|[\u00A0-\uD7FF\uF900-\uFDCF\uFDF0-\uFFEF])|(\\([\x01-\x09\x0b\x0c\x0d-\x7f]|[\u00A0-\uD7FF\uF900-\uFDCF\uFDF0-\uFFEF]))))*(((\x20|\x09)*(\x0d\x0a))?(\x20|\x09)+)?(\x22)))@((([a-z]|\d|[\u00A0-\uD7FF\uF900-\uFDCF\uFDF0-\uFFEF])|(([a-z]|\d|[\u00A0-\uD7FF\uF900-\uFDCF\uFDF0-\uFFEF])([a-z]|\d|-|\.|_|~|[\u00A0-\uD7FF\uF900-\uFDCF\uFDF0-\uFFEF])*([a-z]|\d|[\u00A0-\uD7FF\uF900-\uFDCF\uFDF0-\uFFEF])))\.)+(([a-z]|[\u00A0-\uD7FF\uF900-\uFDCF\uFDF0-\uFFEF])|(([a-z]|[\u00A0-\uD7FF\uF900-\uFDCF\uFDF0-\uFFEF])([a-z]|\d|-|\.|_|~|[\u00A0-\uD7FF\uF900-\uFDCF\uFDF0-\uFFEF])*([a-z]|[\u00A0-\uD7FF\uF900-\uFDCF\uFDF0-\uFFEF])))\.?$/i);

	return pattern.test (emailAddress);
}

function show_preview ()
{
	var recipient = $("[name=in_email_recipient]").val ();
	var sender = $("[name=in_email_sender]").val ();
	var subject = $("[name=in_email_subject]").val ();
	var body = $("[name=in_email_body]").val ();

	hide_alert ("alert_recipient");
	hide_alert ("alert_sender");

	if ( ! is_email (recipient) ){
		show_alert ("alert_recipient", "error", "* Invalid format");
		return;
	}

	if ( ! is_email (sender) ){
		show_alert ("alert_sender", "error", "* Invalid format");
		return;
	}

	$("#email_preview_recipient").text (recipient);
	$("#email_preview_sender").text (sender);
	$("#email_preview_subject").text (subject);
	$("#email_preview_body").text (body);

	$("#email_form").hide ();
	$("#email_paste_key").hide ();
	$("#email_preview").show ();
}

function show_form ()
{
	$("#email_preview").hide ();
	$("#email_paste_key").hide ();
	$("#email_form").show ();
}

function show_public_key ()
{
	$("#email_preview").hide ();
	$("#email_form").hide ();
	$("#email_paste_key").show ();
}

function queue_email ()
{
	var pgpsender = new PGPSender ();
	var access_token = $("[name=access_token]").val ();
	var recipient = $("[name=in_email_recipient]").val ();
	var sender = $("[name=in_email_sender]").val ();
	var subject = $("[name=in_email_subject]").val ();
	var body = $("[name=in_email_body]").val ();

	pgpsender.email_send (access_token, recipient, sender, subject, body, function (data){
		if ( data.status != 0 ){
			// TODO: show alert;
			return;
		}

		show_form ();
	});
}

$(document).ready (function (){
	$("[name=btn_email_preview]").click (show_preview);
	$("[name=btn_email_preview_cancel]").click (show_form);
	$("[name=btn_email_preview_send]").click (queue_email);
	$("[name=btn_email_paste_key]").click (show_public_key);
	$("[name=btn_email_paste_key_ok]").click (show_form);
});

