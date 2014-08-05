function register_submit ()
{
	var pgpsender = new PGPSender ();
	var username = $("[name=username]").val ();
	var password = $("[name=password]").val ();
	var password_confirm = $("[name=password_confirm]").val ();

	if ( password != password_confirm ){
		show_alert ("error", "Passwords do not match.");
		return;
	}

	if ( password.length < 8 ){
		show_alert ("error", "Password is too short, use at least 8 characters.");
		return;
	}

	pgpsender.user_register (username, password, function (data){

		if ( data.status != 0 ){
			show_alert ("error", data.message+".");
			return;
		}

		show_alert ("success", "New account was successfully created, please log in.");
		$("[name=username]").val ("");
		$("[name=password]").val ("");
		$("[name=password_confirm]").val ("");
	});
}


$(document).ready (function (){
	$("[name=submit]").click (register_submit);

	$("[name=username]").focus ();
});

