<?php
	
	$username='ws_user';
	$password='ws_password';
	
	$url = $_GET["ip"]."/".$_GET["action"];
	//echo $url."<br/>";

	$ch = curl_init();
	curl_setopt($ch, CURLOPT_URL, $url);
	curl_setopt($ch, CURLOPT_TIMEOUT, 30); //timeout after 30 seconds
	curl_setopt($ch, CURLOPT_RETURNTRANSFER,1);
	curl_setopt($ch, CURLOPT_HTTPAUTH, CURLAUTH_ANY);
	curl_setopt($ch, CURLOPT_USERPWD, "$username:$password");
	$result=curl_exec ($ch);
	print_r($result);
	$status_code = curl_getinfo($ch, CURLINFO_HTTP_CODE);   //get status code
	curl_close ($ch);

?>
