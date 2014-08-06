function register_submit ()
{
	var pgpsender = new PGPSender ();
	var username = $("[name=username]").val ();
	var password = $("[name=password]").val ();
	var password_confirm = $("[name=password_confirm]").val ();

	if ( password != password_confirm ){
		show_alert ("alert", "error", "Passwords do not match.");
		$("[name=password_confirm]").val ("");
		return;
	}

	if ( password.length < 8 ){
		show_alert ("alert", "error", "Password is too short, use at least 8 characters.");
		$("[name=password]").val ("");
		$("[name=password_confirm]").val ("");
		return;
	}

	pgpsender.user_register (username, password, function (data){

		if ( data.status != 0 ){
			show_alert ("alert", "error", data.message+".");
			$("[name=password]").val ("");
			$("[name=password_confirm]").val ("");
			return;
		}

		show_alert ("alert", "success", "New account was successfully created, please log in.");
		$("[name=username]").val ("");
		$("[name=password]").val ("");
		$("[name=password_confirm]").val ("");
	});
}


$(document).ready (function (){
	$("[name=submit]").click (register_submit);

	$("[name=username]").focus ();
});

