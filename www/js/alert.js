function show_alert (type, message)
{
	var alert_class = null;

	switch (type){
		case "error":
			alert_class = "alert-error";
			break;

		case "info":
			alert_class = "alert-info";
			break;

		case "success":
			alert_class = "alert-success";
			break;

		default:
			alert_class = "alert-info";
			break;
	}

	$("#alert").text (message);
	$("#alert").addClass (alert_class);
	$("#alert").show ();
}

function hide_alert ()
{
	$("#alert").hide ();
	$("#alert").removeClass ("alert-error alert-info alert-success");
	$("#alert").text ("");
}

