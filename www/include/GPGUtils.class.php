<?php
/*
 * Copyright (c) 2014, PGPSender.org
 */

require_once ("Config.class.php");

class GPGUtils
{
	public static function import ($key_str)
	{
		$fd = popen ("gpgimport.sh", "w");

		if ( $fd === false )
			return false;

		fwrite ($fd, $key_str);

		return pclose ($fd);
	}

	public static function get_key ($email, &$key_str)
	{
		$fd = popen ("gpggetkey.sh \"".escapeshellarg ($email)."\"", "r");

		if ( $fd === false )
			return false;

		fread ($fd, $key_str, 8192);

		return pclose ($fd);
	}

	public static function encrypt ($email, $message)
	{
		$fd = popen ("gpgencrypt.sh \"".escapeshellarg ($email)."\"", "rw");

		if ( $fd === false )
			return false;

		fwrite ($fd, $message);

		return pclose ($fd);
	}

	// TODO: delete key...
};

?>
