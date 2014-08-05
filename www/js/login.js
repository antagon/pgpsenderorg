function login_submit ()
{
	var pgpsender = new PGPSender ();

	pgpsender.user_login ($("[name=username]").val (), $("[name=password]").val (), function (data){

		if ( data.status != 0 ){
			$("[name=password]").val ("");
			show_alert ("alert", "error", data.message+".");
			return;
		}

		location.reload ();
	});
}

$(document).ready (function (){
	$("[name=submit]").click (login_submit);

	$("[name=username]").focus ();
});

