<?php

session_start ();

if ( isset ($_SESSION["user"]) )
	header ("Location: /");

?>

<!DOCTYPE html>
<html>
	<head>
	<title>Register - PGPSender</title>
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

<?php
	require_once ("include/alert.php");
?>

			<fieldset>
				<legend>Register</legend>

				<input type="text" name="username" placeholder="Username">
				<input type="password" name="password" placeholder="Password">
				<input type="password" name="password_confirm" placeholder="Confirm password">

				<button name="submit">Submit</button>
			</fieldset>

		</div>
		
		<div id="footer">
<?php
	require_once ("include/footer.php");
?>
		</div>

		<script type="text/javascript" src="js/register.js"></script>
	</body>
</html>

