<?php
// header('Content-Type: text/xml');
// header('Cache-Control: no-cache');
// header('Cache-Control: no-store' , false);



if (isset($_POST)) {
    $descriptorspec = array(
	  0 => array("pipe", "r"),  // stdin is a pipe that the child will read from
	  1 => array("pipe", "w"),  // stdout is a pipe that the child will write to
	  2 => array("pipe", "w") // stderr is a file to write to
      );
      
$cwd = '/tmp';
$env = array('some_option' => 'aeiou');


$promptid = $_POST["promptid"];
$code = $_POST["code"];
$shell = "rootshellclient "+(string)strlen($code);

$process = proc_open($shell, $descriptorspec, $pipes, $cwd, $env);

}

$msg_arr = Array("promptid" => $_POST["promptid"],"output" => $_POST["code"]);
$json_msg = json_encode($msg_arr);
echo $json_msg;

?>