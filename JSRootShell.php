<?php
header('Content-Type: text/xml');
header('Cache-Control: no-cache');
header('Cache-Control: no-store' , false);



if (isset($_POST)) {
$msg_arr = Array("promptid" => $_POST["promptid"],"output" => $_POST["code"]);
$json_msg = json_encode($msg_arr);
echo $json_msg;
}
?>