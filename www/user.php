<?php

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
					<button>Add address</button>
				</div>

				<table>
					<tbody>
						<tr>
							<td>foo@example.org</td>
							<td class="text-right">
								<button disabled="disabled">Primary</button> <button>GPG key</button>
							</td>
						</tr>
						<tr>
							<td>blablah@example.org</td>
							<td class="text-right">
								<button>Set primary</button> <button>GPG key</button>
							</td>
						</tr>
					</tbody>
				</table>
			</fieldset>

			<fieldset>
				<legend>Aliases</legend>

				<div>
					<button>Add alias</button>
				</div>

				<table>
					<tbody>
						<tr>
							<td>b88d40f311aa0591170a82e9147d3d9c@pgpsender.org</td>
							<td class="text-right">
								<select>
									<option value="null">unassigned</option>
								</select>
							</td>
						</tr>
						<tr>
							<td>2be852db88abf04941728fa41cf197ff@pgpsender.org</td>
							<td class="text-right">
								<select>
									<option value="null">unassigned</option>
								</select>
							</td>
						</tr>
						<tr>
							<td>3fe7c4bd69d4b53cffe0a9dbe1db6027@pgpsender.org</td>
							<td class="text-right">
								<select>
									<option value="null">unassigned</option>
								</select>
							</td>
						</tr>
					</tbody>
				</table>
			</fieldset>

			<fieldset>
				<legend>Change password</legend>

				<div id="alert_passwd" class="alert hidden"></div>

				<input type="password" name="passwd_password" placeholder="Password">
				<input type="password" name="passwd_password_new" placeholder="New password">

				<button name="passwd_submit">Submit</button>
			</fieldset>

			<fieldset>
				<legend>Delete account</legend>

				<p class="alert-error">
					Upon deleting your account, all data associated with the account will be deleted from our database!<br>
					As this operation cannot be reversed, please confirm your decision by providing your password.
				</p>

				<input type="password" name="password" placeholder="Password">

				<button name="submit">Confirm</button>
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

