function add_email_row (id, address, is_primary, is_key)
{
	var buttons = "";

	if ( is_primary )
		buttons += "<button class=\"btn_address_primary\" disabled=\"disabled\" title=\"Primary address\"> <span style=\"color:#2D6FAC;\" class=\"fa fa-star\"></span> </button>";
	else
		buttons += "<button class=\"btn_address_primary\" data-address=\""+address+"\" title=\"Set as primary address\"> <span class=\"fa fa-star\"></span> </button>";

	// TODO: distinguish between the two (is set or not set)
	if ( is_key )
		buttons += "<button class=\"btn_address_key\" data-address=\""+address+"\" title=\"Set public key\"> <span class=\"fa fa-lock\"></span> </button>";
	else
		buttons += "<button class=\"btn_address_key\" data-address=\""+address+"\" title=\"Set public key\"> <span class=\"fa fa-unlock-alt\"></span> </button>";

	buttons += "<button class=\"btn_address_delete\" data-address=\""+address+"\" title=\"Delete\"> <span class=\"fa fa-trash-o\"></span> </button>";

	$("#"+id+" tbody").append ("<tr> <td>"+address+"</td> <td class=\"text-right\"> "+buttons+" </td> </tr>");

	$(".btn_address_primary").last ().click (set_primary_email_submit);
	$(".btn_address_key").last ().click (function (){ console.log ("TODO..."); });
	$(".btn_address_delete").last ().click (delete_address_submit);
}

function add_alias_row (id, alias, assigned_address, addresses)
{
	var buttons = "";

	buttons += "<select class=\"select_alias_target\" data-alias=\""+alias+"\">";
	buttons += "<option value=\"unassigned\">unassigned</option>";

	for ( var i = 0; i < addresses.length; i++ ){
		buttons += "<option value=\""+addresses[i]+"\" "+((addresses[i] == assigned_address)? "selected":"")+">"+addresses[i]+"</option>";
	}

	buttons += "</select>";

	buttons += "<button class=\"btn_alias_delete\" data-alias=\""+alias+"\" title=\"Delete\"> <span class=\"fa fa-trash-o\"></span> </button>";

	$("#"+id+" tbody").append ("<tr> <td>"+alias+"</td> <td class=\"text-right\">"+buttons+"</td> </tr>");

	$(".select_alias_target").last ().change (assign_alias_address_submit);
	$(".btn_alias_delete").last ().click (delete_alias_submit);
}

function get_userdata ()
{
	var pgpsender = new PGPSender ();

	pgpsender.email_get ($("[name=api_key]").val (), function (data){
		var emails = data.data;
		var addresses = new Array ();

		if ( data.status != 0 )
			return;

		$("#email_table tbody").html ("");

		for ( var i = 0; i < emails.length; i++ ){
			add_email_row ("email_table", emails[i].name, emails[i].primary, false);
			addresses.push (emails[i].name);
		}

		// Fetch aliases
		pgpsender.alias_get ($("[name=api_key]").val (), function (data){
			var aliases = data.data;

			if ( data.status != 0 )
				return;

			$("#alias_table tbody").html ("");

			for ( var i = 0; i < aliases.length; i++ ){
				var assigned_address = null;

				for ( var j = 0; j < emails.length; j++ ){
					if ( aliases[i].email_id == emails[j].id )
						assigned_address = emails[j].name;
				}

				add_alias_row ("alias_table", aliases[i].name, assigned_address, addresses);
			}
		});
	});
}

function change_password_submit ()
{
	var pgpsender = new PGPSender ();

	pgpsender.user_passwd ($("[name=api_key]").val (), $("[name=passwd_password]").val (), $("[name=passwd_password_new]").val (), function (data){

		$("[name=passwd_password]").val ("");
		$("[name=passwd_password_new]").val ("");

		if ( data.status != 0 ){
			show_alert ("alert_passwd", "error", data.message+".");
			return;
		}

		show_alert ("alert_passwd", "success", "Password changed successfully.");
	});
}

function set_primary_email_submit (event)
{
	var pgpsender = new PGPSender ();
	var address = $(event.target).data ("address");

	pgpsender.user_set_primary_email ($("[name=api_key]").val (), address, function (data){

		if ( data.status != 0 ){
			show_alert ("alert_address", "error", data.message+".");
			return;
		}

		get_userdata ();
	});
}

function assign_alias_address_submit (event)
{
	var pgpsender = new PGPSender ();
	var alias = $(event.target).data ("alias");
	var address = $(event.target).val ();

	pgpsender.alias_assign_address ($("[name=api_key]").val (), alias, address, function (data){

		if ( data.status != 0 ){
			show_alert ("alert_alias", "error", data.message+".");
			return;
		}

		get_userdata ();
	});
}

function add_address_submit ()
{
	var pgpsender = new PGPSender ();

	pgpsender.email_add ($("[name=api_key]").val (), $("[name=in_address_new]").val (), function (data){

		if ( data.status != 0 ){
			show_alert ("alert_address", "error", data.message+".");
			return;
		}

		get_userdata ();
	});
}

function add_alias_submit ()
{
	var pgpsender = new PGPSender ();

	pgpsender.alias_add ($("[name=api_key]").val (), $("[name=in_alias_new]").val (), $("[name=select_alias_hostname_new]").val (), function (data){

		if ( data.status != 0 ){
			show_alert ("alert_alias", "error", data.message+".");
			return;
		}

		get_userdata ();
	});
}

function delete_address_submit (event)
{
	var pgpsender = new PGPSender ();
	var address = $(event.target).data ("address");

	pgpsender.email_delete ($("[name=api_key]").val (), address, function (data){

		if ( data.status != 0 ){
			show_alert ("alert_address", "error", data.message+".");
			return;
		}

		get_userdata ();
	});
}

function delete_alias_submit (event)
{
	var pgpsender = new PGPSender ();
	var alias = $(event.target).data ("alias");

	pgpsender.alias_delete ($("[name=api_key]").val (), alias, function (data){

		if ( data.status != 0 ){
			show_alert ("alert_alias", "error", data.message+".");
			return;
		}

		get_userdata ();
	});
}

$(document).ready (function (){
	$("[name=btn_address_new]").click (function (){
		hide_alert ("alert_address");
		$("[name=in_address_new]").val ("");
		$("#address_new").show ();
		$("[name=in_address_new]").focus ();
	});

	$("[name=btn_address_new_submit]").click (function (){
		$("#address_new").hide ();
		add_address_submit ();
	});

	$("[name=btn_alias_new]").click (function (){
		hide_alert ("alert_alias");
		$("[name=in_alias_new]").val ("");
		$("#alias_new").show ();
		$("[name=in_alias_new]").focus ();
	});

	$("[name=btn_alias_new_submit]").click (function (){
		$("#alias_new").hide ();
		add_alias_submit ();
	});

	$("[name=passwd_submit]").click (change_password_submit);

	get_userdata ();
});

