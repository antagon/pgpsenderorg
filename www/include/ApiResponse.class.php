<?php

class ApiResponse
{
	const E_OK = 0;
	const E_EMPTY = 1;
	const E_EXISTS = 2;
	const E_INTERNAL = 3;
	const E_UNAUTHORIZED = 4;

	public static function json_exit ($status, $message = "")
	{
		echo json_encode (array ("status" => $status, "message" => $message));
		exit (0);
	}

	public static function json_exit_data ($status, $data = array ())
	{
		echo json_encode (array ("status" => $status, "data" => $data));
	}
};

?>

