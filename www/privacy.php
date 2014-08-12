<?php
	session_start ();
?>

<!DOCTYPE html>
<html>
	<head>
	<title>Privacy - PGPSender</title>
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

			<h2>Privacy</h2>

			<p>This document lays down a key tenets of our privacy policy in respect to user and user's data.</p>

			<ol>
				<li>We <strong>shall not</strong> expose user's email addresses to anyone.</li>
				<li>We <strong>shall not</strong> expose relationship between user defined aliases and user's email addresses.</li>
				<li>We <strong>shall not</strong> prowl through content of user's emails.</li>
				<li>We <strong>shall not</strong> preserve undeliverable emails.</li>
				<li>We <strong>shall not</strong> preserve user's email addresses against his will.</li>
				<li>We <strong>shall not</strong> monitor user's activity and/or behavior.</li>
				<li>We <strong>shall not</strong> send unsolicited emails to user.</li>
				<li>We <strong>shall</strong> reveal publicly all attempts to break our vow to these tenets.</li>
			</ol>

		</div>
		
		<div id="footer">
<?php
	require_once ("include/footer.php");
?>
		</div>
	</body>
</html>

