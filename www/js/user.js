function change_password_submit ()
{
	var pgpsender = new PGPSender ();

	pgpsender.user_passwd ($("[name=username]").val (), $("[name=passwd_password]").val (), $("[name=passwd_password_new]").val (), function (data){

		if ( data.status != 0 ){
			$("[name=passwd_password]").val ("");
			$("[name=passwd_password_new]").val ("");
			show_alert ("alert_passwd", "error", data.message+".");
			return;
		}

		show_alert ("alert_passwd", "success", "Password changed successfully.");
	});
}

$(document).ready (function (){
	$("[name=passwd_submit]").click (change_password_submit);
});

