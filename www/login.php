<!DOCTYPE html>
<html>
	<head>
	<title>Login - PGPSender</title>
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
			
			<h1> <a href="./">PGPSender</a> </h1>
			<div id="quote">
<?php
	require_once ("include/quote.php");
?>
			</div>

			<fieldset>
				<legend>Login</legend>

				<input type="text" name="username" placeholder="Username">
				<input type="password" name="password" placeholder="Password">

				<button name="submit">Submit</button>
			</fieldset>

		</div>
		
		<div id="footer">
<?php
	require_once ("include/footer.php");
?>
		</div>

		<script type="text/javascript" src="js/login.js"></script>
	</body>
</html>

