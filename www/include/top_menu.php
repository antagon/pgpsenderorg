	<a href="sender.php">Send email</a>
	<span class="delim">|</span>
<?php if ( isset ($_SESSION["user"]) ){ ?>
	<a href="user.php">@<?php echo htmlspecialchars ($_SESSION["user"]["name"]); ?></a> <span class="delim">|</span> <a href="logout.php">Logout</a>
<?php } else { ?>
	<a href="register.php">Register</a> <span class="delim">|</span> <a href="login.php">Login</a>
<?php } ?>

