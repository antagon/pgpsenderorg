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

	this.user_set_primary_email = function (api_key, email, callback)
	{
		$.post ("/api/user_set_primary_email.php", "api_key="+api_key+"&address="+email, function (response){
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

	this.email_set_gpgkey = function (api_key, address, gpg_key, callback)
	{
		$.post ("/api/email_set_gpgkey.php", "api_key="+api_key+"&address="+address+"&gpg_key="+gpg_key, function (response){
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

	this.email_send = function (access_token, recipient, sender, subject, message, callback)
	{
		$.post ("/api/email_send.php", "access_token="+access_token+"&recipient="+recipient+"&sender="+sender+"&subject="+subject+"&message="+message, function (response){
			var data = null;

			try {
				data = $.parseJSON (response);
			} catch (e){
				console.log ("JSON parser failed - invalid data!");	
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

	this.email_delete = function (api_key, email, callback)
	{
		$.post ("/api/email_delete.php", "api_key="+api_key+"&address="+email, function (response){
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

	this.alias_assign_address = function (api_key, alias, address, callback)
	{
		$.post ("/api/alias_assign_address.php", "api_key="+api_key+"&alias="+alias+"&address="+address, function (response){
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

