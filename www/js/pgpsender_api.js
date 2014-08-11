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

	this.user_passwd = function (api_key, password, password_new, callback)
	{
		$.post ("/api/user_passwd.php", "api_key="+api_key+"&password="+password+"&password_new="+password_new, function (response){
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

	this.email_get = function (api_key, callback)
	{
		$.post ("/api/email_get.php", "api_key="+api_key, function (response){
			var data = null;

			try {
				data = $.parseJSON (response);
			} catch (e){
				console.log ("JSON parser failed - invalid data!");
				return;
			}

			callback (data);
		});
	};

	this.alias_get = function (api_key, callback)
	{
		$.post ("/api/alias_get.php", "api_key="+api_key, function (response){
			var data = null;

			try {
				data = $.parseJSON (response);
			} catch (e){
				console.log ("JSON parser failed - invalid data!");
				return;
			}

			callback (data);
		});
	};

	this.email_add = function (api_key, address, callback)
	{
		$.post ("/api/email_add.php", "api_key="+api_key+"&address="+address, function (response){
			var data = null;

			try {
				data = $.parseJSON (response);
			} catch (e){
				console.log ("JSON parser failed - invalid data!");
				return;
			}

			callback (data);
		});
	};

	this.alias_add = function (api_key, alias, hostname, callback)
	{
		$.post ("/api/alias_add.php", "api_key="+api_key+"&alias="+alias+"&hostname="+hostname, function (response){
			var data = null;

			try {
				data = $.parseJSON (response);
			} catch (e){
				console.log ("JSON parser failed - invalid data!");
				return;
			}

			callback (data);
		});
	};

	this.alias_delete = function (api_key, alias, callback)
	{
		$.post ("/api/alias_delete.php", "api_key="+api_key+"&alias="+alias, function (response){
			var data = null;

			try {
				data = $.parseJSON (response);
			} catch (e){
				console.log ("JSON parser failed - invalid data!");
				return;
			}

			callback (data);
		});
	};
};

