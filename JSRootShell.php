<?php
header('Content-Type: text/xml');
header('Cache-Control: no-cache');
header('Cache-Control: no-store' , false);
$xmlStr = '<?xml version="1.0" encoding="UTF-8"?>'.
          '<data>'.
          '<promptid>'.$_POST["promptid"].'</promptid>'.
          '<output> '.'Output'. $_POST["code"].'</output>'.
          '</data>';
echo $xmlStr;

// //print_r($_POST);
// if (isset($_POST)) {
//   print("<script language=\"javascript\"> alert('called')</script>");
//    print("<h1 aling=center >CALLED</h1>");
// }else
// {
//   print("<script language=\"javascript\"> alert('called')</script>");
//   print("<h1 aling=center >NOT CALLED</h1>");
// }

//   $values = array();
//   foreach ($_POST as $key => $value) {
//             $values[] = "$key=$value";
//             print("<h1 aling=center >CALLED</h1>");
//             print("<script language=\"javascript\"> alert('called')</script>");
//           print("<script language=\"javascript\"> alert($key=$value)</script>");
//}
?>