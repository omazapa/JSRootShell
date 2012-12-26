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



$process = proc_open($shell, $descriptorspec, $pipes, $cwd, $env);
if (is_resource($process)) {
    fwrite($pipes[0],$code);
    fclose($pipes[0]);
    
    echo stream_get_contents($pipes[1]);
    fclose($pipes[1]);
    echo stream_get_contents($pipes[2]);
    fclose($pipes[2]);
    $return_value = proc_close($process);
   echo "command returned $return_value\n";
}
}

?>