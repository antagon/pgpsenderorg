function PGPSender ()
{
	this.user_register = function (username, password)
	{
		$.post ("/api/user_register.php", "username="+username+"&password="+password, function (){
			alert ("submitted");
		});
	};

	this.user_login = function ()
	{
		
	};
};
