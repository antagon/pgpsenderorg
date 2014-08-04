function PGPSender ()
{
	this.user_register = function (username, password, callback)
	{
		$.post ("/api/user_register.php", "username="+username+"&password="+password, function (data){
			callback (data);
		});
	};

	this.user_login = function (username, password, callback)
	{
		$.post ("/api/user_login.php", "username="+username+"&password="+password, function (data){
			callback (data);
		});
	};
};
