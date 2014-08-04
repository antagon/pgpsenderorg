function login_submit ()
{
	var pgpsender = new PGPSender ();

	pgpsender.user_login ($("[name=username]").val (), $("[name=password]").val (), function (response){
		var data = $.parseJSON (response);

		if ( data.status != 0 ){
			$("[name=password]").val ("");
			show_alert ("error", data.message+".");
			return;
		}

		location.reload ();
	});
}

$(document).ready (function (){
	$("[name=submit]").click (login_submit);

	$("[name=username]").focus ();
});

