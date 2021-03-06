<?php

require_once ("include/Config.class.php");

session_start ();

if ( ! isset ($_SESSION["user"]) )
	header ("Location: /login.php");
	
?>

<!DOCTYPE html>
<html>
	<head>
	<title>PGPSender</title>
<?php
	require_once ("include/head.php");
?>
	</head>
	<body>
		<div id="content">
			<div id="lang_switcher">
<?php
	require_once ("include/top_menu.php");
?>
			</div>
			
			<h1> <a href="/">PGPSender</a> </h1>
			<div id="quote">
<?php
	require_once ("include/quote.php");
?>
			</div>

			<fieldset>
				<legend>Addresses</legend>

				<div>
					<button name="btn_address_new">Add address</button>
				</div>

				<div id="alert_address" class="alert hidden"></div>

				<div id="address_new" class="hidden">
					<form onsubmit="return false;">
						<input type="text" name="in_address_new" placeholder="New address">
						<button name="btn_address_new_submit">Submit</button>
					</form>
				</div>

				<div id="email_table" class="table"></div>
			</fieldset>

			<fieldset>
				<legend>Aliases</legend>

				<div>
					<button name="btn_alias_new">Add alias</button>
				</div>

				<div id="alert_alias" class="alert hidden"></div>

				<div id="alias_new" class="hidden">
					<form onsubmit="return false;">
						<input type="text" name="in_alias_new" placeholder="New alias">

						<select name="select_alias_hostname_new">
							<?php foreach ( Config::$hostname as $hostname ){ ?>
							<option value="<?php echo $hostname; ?>">@<?php echo $hostname; ?></option>
							<?php } ?>
						</select>

						<button name="btn_alias_new_submit">Submit</button>
					</form>
				</div>

				<div id="alias_table" class="table"></div>
			</fieldset>

			<fieldset>
				<legend>Change password</legend>

				<div id="alert_passwd" class="alert hidden"></div>

				<form onsubmit="return false;">
					<input type="password" name="passwd_password" placeholder="Password">
					<input type="password" name="passwd_password_new" placeholder="New password">

					<button name="passwd_submit">Submit</button>
				</form>
			</fieldset>

			<fieldset>
				<legend>Delete account</legend>

				<p class="alert-error">
					When deleting the account, your email addresses will be deleted from the database.
					Please note, that the aliases will be preserved to avoid being reused by another user.<br><br>
					This operation cannot be reversed, please confirm your decision by providing your password.
				</p>

				<form onsubmit="return false;">
					<input type="password" name="password" placeholder="Password">

					<button name="submit">Confirm</button>
				</form>
			</fieldset>

		</div>
		
		<div id="footer">
<?php
	require_once ("include/footer.php");
?>
		</div>

		<input type="hidden" name="api_key" value="<?php echo $_SESSION["user"]["api_key"]; ?>">

		<script type="text/javascript" src="js/user.js"></script>
	</body>
</html>

