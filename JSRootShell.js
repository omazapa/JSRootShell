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
   
   function isJson(value) {
    try {
        JSON && JSON.parse(value) || $.parseJSON(value);
    } catch (e) {
        return false;
    }
    return true;
   }

   this.Init = function() {
      this.shelldiv = document.createElement('div');
      this.shelldiv.setAttribute('class', 'JSRootShell');
      this.shelldiv.setAttribute('id', id);
      this.shelldiv.setAttribute('style', style);

      this.shelltable =  document.createElement('table');
      this.shelldiv.appendChild(this.shelltable);

      this.currentPrompt = new JSRootPrompt(0, this);
      this.shelltable.appendChild(this.currentPrompt.getElement());

      document.body.appendChild(this.shelldiv);
   };

   this.newPrompt = function() {
      number++;
      this.currentPrompt.setReadOnly();
      this.currentPrompt = new JSRootPrompt(number, this);
      this.shelltable.appendChild(document.createElement('br'));
      this.shelltable.appendChild(this.currentPrompt.getElement());
   };


   this.sendRequest = function() {

      var code = "code="+encodeURIComponent(this.currentPrompt.getCode());
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
           	var output =  xmlhttp.responseText;
		if(isJson())
		{
                    var json_obj= JSON && JSON.parse(output) || $.parseJSON(output);		  
                    var prompt = document.getElementById(json_obj.promptid);
                    prompt.value += '\n'+json_obj.output;
		}else
		{
		 alert("Message is not valid JSON string\nMSG = "+output); 
		}

            };
         } else {
            //alert("statusText: " + xmlhttp.statusText + "\nHTTP status code: " + xmlhttp.status);
         };

      };
   };
};
