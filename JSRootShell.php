<?php
header('Content-Type: text/xml');
header('Cache-Control: no-cache');
header('Cache-Control: no-store' , false);

if (isset($_POST)) {
    $descriptorspec = array(
	  0 => array("pipe", "r"),  // stdin is a pipe that the child will read from
	  1 => array("pipe", "w"),  // stdout is a pipe that the child will write to
	  2 => array("pipe", "w")  // stderr is a file to write to
      );
      
$cwd = '.';


$promptid = $_POST["promptid"];
$code = urldecode($_POST["code"]);
$shell = "./rootweb/rootshellclient";
$env = array('JSRPromptID' => $promptid );

$json_array = Array("promptid" => $promptid); 

$process = proc_open($shell, $descriptorspec, $pipes, $cwd, $env);
if (is_resource($process)) {
    $json_array["proc_open"] = true;
    fwrite($pipes[0],$code);
    fclose($pipes[0]);
    $json_array["stdout"] = stream_get_contents($pipes[1]);
    fclose($pipes[1]);
    
    $json_array["stderr"] = stream_get_contents($pipes[2]);
    fclose($pipes[2]);
    $json_array["proc_close"] = proc_close($process);
}else{
$json_array["proc_open"] = false;
}

echo json_encode($json_array);
}

?>