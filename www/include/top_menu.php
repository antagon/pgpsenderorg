<?php if ( isset ($_SESSION["user"]) ){ ?>
	<a href="sender.php">Sender</a> <span class="delim">|</span> <a href="user.php">@<?php echo $_SESSION["user"]["name"]; ?></a> <span class="delim">|</span> <a href="logout.php">Logout</a>
<?php } else { ?>
	<a href="sender.php">Sender</a> <span class="delim">|</span> <a href="register.php">Register</a> <span class="delim">|</span> <a href="login.php">Login</a>
<?php } ?>

