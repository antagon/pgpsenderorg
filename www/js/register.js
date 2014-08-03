function register_submit ()
{
	var pgpsender = new PGPSender ();

	pgpsender.user_register ($("[name=username]").val (),
								$("[name=password]").val ());
}


$(document).ready (function (){
	$("[name=submit]").click (register_submit);

	$("[name=username]").focus ();
});

