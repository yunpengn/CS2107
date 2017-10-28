<?php
$to = 'prof@cs2107.spro.ink';
$subject = 'Assignment Grades';
$message = 'ctf2107@gmail.com:A1234567Z:100/100';

$headers = array();
$headers[] = 'From: 2107ta@cs2107.spro.ink';
$headers[] = 'Date: Sat, 09 Sep 2017 00:45:02 +0800';
$headers[] = 'X-Mailer: Mac OS XIII Mail';

if(mail($to, $subject, $message, implode("\r\n", $headers))) {
	echo("Email Sent.");
} else {
	echo("Sending Failed.");
}
?>