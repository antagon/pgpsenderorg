<?php

class ApiResponse
{
	const E_OK = 0;
	const E_EMPTY = 1;
	const E_EXISTS = 2;

	public static function json_exit ($status, $message = "")
	{
		echo json_encode (array ("status" => $status, "message" => $message));
		exit (0);
	}
};

?>

