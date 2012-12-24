/*************************************************************************
* Copyright (C) 2012,  Gfif Developers                                   *
* Grupo de Fenomenologia de Interacciones Fundamentales                  *
* http://gfif.udea.edu.co                                                *
* División de ciencias de la computación Gfifdev                         *
* http://gfifdev.udea.edu.co                                             *
* Created and Maintained By Omar Andres Zapata Mesa                      *
* All rights reserved.                                                   *
*                                                                        *
*                                                                        *
* For the list of contributors see $ROOTSYS/README/CREDITS.              *
*************************************************************************/

var _tab = 9;
var _enter = 13;
var _delete = 46;
var _up = 38;
var _down = 40;


function JSRootShell(id, style)
{
   id = id || "JSRootShell";
   style = style || "border-style:none;color:#000000;background:#000000;height:50%;width: 50%;resize: both;";
   var number = 0;
    this.updateStyle = function(newstyle) {
      document.getElementById(id).setAttribute("style", newstyle);
   };

   this.Init = function() {
      this.shelldiv = document.createElement('div');
      this.shelldiv.setAttribute('class', 'JSRootShell');
      this.shelldiv.setAttribute('id', id);
      this.shelldiv.setAttribute('style', style);

      this.currentPrompt = new JSRootPrompt(0, this);
      this.shelldiv.appendChild(this.currentPrompt.getElement());

      document.body.appendChild(this.shelldiv);
   };

   this.newPrompt = function() {
      number++;
      this.currentPrompt.setReadOnly();
      this.currentPrompt = new JSRootPrompt(number, this);
      this.shelldiv.appendChild(document.createElement('br'));
      this.shelldiv.appendChild(this.currentPrompt.getElement());
   };


   this.sendRequest = function() {

      var code = "code="+this.currentPrompt.getCode();
//      alert(this.currentPrompt.getCode());
      var promptid = "promptid="+this.currentPrompt.getId();
      var msg  = promptid+'&'+code;
      if (window.XMLHttpRequest) {
          xmlhttp = new XMLHttpRequest();
       } else {
          xmlhttp = new ActiveXObject("Microsoft.XMLHTTP");
       }
      var url = "JSRootShell.php";
      xmlhttp.open("POST", url, true);
      xmlhttp.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
      xmlhttp.send(msg);
      xmlhttp.onreadystatechange = function() {

         if (xmlhttp.readyState == 4) {
            if (xmlhttp.status == 200) {
               var outputXml =  xmlhttp.responseXML;
               var promptid  =  outputXml.getElementsByTagName('promptid')[0].firstChild.nodeValue;
               var output    =  outputXml.getElementsByTagName('output')[0].firstChild.nodeValue;
//               alert(promptid);
//               alert(output);
               var prompt = document.getElementById(promptid);
               prompt.value += '\n'+output;
            }
         } else {
            //alert("statusText: " + xmlhttp.statusText + "\nHTTP status code: " + xmlhttp.status);
         }

      };
   };
}
