<?php
header('Content-type: text/json');
header('Content-type: application/json');
header('Cache-Control: no-cache');
header('Cache-Control: no-store' , false);




if (isset($_POST)) {
    $descriptorspec = array(
	  0 => array("pipe", "r"),  // stdin is a pipe that the child will read from
	  1 => array("pipe", "w"),  // stdout is a pipe that the child will write to
	  2 => array("pipe", "w")  // stderr is a file to write to
      );
      
$cwd = '.';


$shell = "./rootweb/jsrootshell";
$env = array();

$json_array = Array(); 

$process = proc_open($shell, $descriptorspec, $pipes, $cwd, $env);

if (is_resource($process)) {
    $json_array["proc_open"] = true;
    fclose($pipes[0]);
    
    while(!feof($pipes[1])) 
    {
    $json_array["stdout"] = stream_get_contents($pipes[1]);
    }    
    fclose($pipes[1]);
    
    
    while(!feof($pipes[2])) 
    {
    $json_array["stderr"] = stream_get_contents($pipes[2]);
    }
    fclose($pipes[2]);
    $json_array["proc_close"] = proc_close($process);
}else{
$json_array["proc_open"] = false;
}
echo json_encode($json_array)."\n";
}

?>