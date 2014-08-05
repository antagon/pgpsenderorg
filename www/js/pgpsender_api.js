function PGPSender ()
{
	this.user_register = function (username, password, callback)
	{
		$.post ("/api/user_register.php", "username="+username+"&password="+password, function (response){
			var data = null;

			try {
				data = $.parseJSON (response);
			} catch (e) {
				console.log ("JSON parser failed - invalid data!");
				return;
			}

			callback (data);
		});
	};

	this.user_login = function (username, password, callback)
	{
		$.post ("/api/user_login.php", "username="+username+"&password="+password, function (response){
			var data = null;

			try {
				data = $.parseJSON (response);
			} catch (e) {
				console.log ("JSON parser failed - invalid data!");
				return;
			}

			callback (data);
		});
	};

	this.user_passwd = function (username, password, password_new, callback)
	{
		$.post ("/api/user_passwd.php", "username="+username+"&password="+password+"&password_new="+password_new, function (response){
			var data = null;

			try {
				data = $.parseJSON (response);
			} catch (e) {
				console.log ("JSON parser failed - invalid data!");
				return;
			}

			callback (data);
		});
	};
};
