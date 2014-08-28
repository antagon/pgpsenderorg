<?php
/*
 * Copyright (c) 2014, PGPSender.org
 */

require_once ("Config.class.php");
require_once ("phpmailer/PHPMailer.class.php");

class MailerFacility
{
	private $phpmailer;

	public function __construct ()
	{
		$this->phpmailer = new PHPMailer ();

		$this->phpmailer->CharSet = "UTF-8";
		//$this->phpmailer->isSMTP (); 
		//$this->phpmailer->SMTPAuth = true;
		//$this->phpmailer->SMTPSecure = "tls";
		//$this->phpmailer->Host = Config::SMTP_HOST;
		//$this->phpmailer->Port = Config::SMTP_PORT;
		//$this->phpmailer->Username = Config::SMTP_USER;
		//$this->phpmailer->Password = Config::SMTP_PASS;
		$this->phpmailer->XMailer = "PGPSender.org";
		$this->phpmailer->WordWrap = 80; 
		$this->phpmailer->isHTML (false);
	}

	public function __destruct ()
	{
	
	}

	private function write ($filename)
	{
		try {
			$this->phpmailer->preSend ();
		} catch (phpmailerException $ex){
			return false;
		}

		$fd = fopen ($filename, "w");

		if ( $fd === false )
			return false;

		fwrite ($fd, $this->phpmailer->getSentMIMEMessage ());
		fclose ($fd);

		return true;
	}

	public function compose_raw ($to, $from, $subject, $body)
	{
		$this->phpmailer->From = $from;
		$this->phpmailer->FromName = "";
		$this->phpmailer->addReplyTo ($from);
		$this->phpmailer->addAddress ($to);
		$this->phpmailer->Subject = $subject;
		$this->phpmailer->Body = $body;

		return $this->write (tempnam (Config::MAILQUEUE_PATH, "mail"));
	}
};

?>
