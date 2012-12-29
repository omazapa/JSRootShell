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


function JSRootShell(id, style,logging)
{
   id = id || "JSRootShell";
   style = style || "border-style:none;color:#000000;background:#000000;height:50%;width: 50%;resize: both;";
   logging = logging || true;
   var number = 0;
    this.updateStyle = function(newstyle) {
      document.getElementById(id).setAttribute("style", newstyle);
   };
   
   function isJson(jmsg) {
    try {
       var json_obj =  JSON && JSON.parse(jmsg) || $.parseJSON(jmsg);
    } catch (e) {
       console.error("Error: parsing json message = "+e);
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
      this.shelltable.setAttribute('class', 'JSRootShellTable');
      this.shelldiv.appendChild(this.shelltable);

      this.currentPrompt = new JSRootPrompt(0, this);
      this.shelltable.appendChild(this.currentPrompt.getElement());

      document.body.appendChild(this.shelldiv);
   };

   this.newPrompt = function() {
      number++;
      this.currentPrompt.setReadOnly();
      this.currentPrompt = new JSRootPrompt(number, this);
      this.shelltable.appendChild(this.currentPrompt.getElement());
   };


   this.sendRequest = function() {

//       var code = "code="+encodeURI(this.currentPrompt.getCode());

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
      xmlhttp.setRequestHeader( "Content-Type", "application/json" );
      xmlhttp.send(msg);
      if(logging) console.log("Send JSON's XmlHttpMessage: "+msg);
      xmlhttp.ontimeout = function() {
	  console.log("The request timed out.");
	}
      xmlhttp.onreadystatechange = function(event) {
         if (xmlhttp.readyState == 4) {
            if (xmlhttp.status == 200) {
           	var output =  xmlhttp.responseText;
		if(isJson(output))
		{
                    var json_obj= JSON && JSON.parse(output) || $.parseJSON(output);
		    if(!json_obj.promptid)
		    {
		      alert("Error: shell can no get prompt id from xmlhttp response.");
		      return;
		    }
		    var prompt = document.getElementById(json_obj.promptid);
		    if(json_obj.proc_open)
		    {
		         if(json_obj.stdout) prompt.value += '\n'+json_obj.stdout;
			 if(json_obj.stderr) prompt.value += '\n'+json_obj.stderr;
			 prompt.style.height = prompt.scrollHeight + 'px';
		    }else
		    {
		     console.log("Error: the shell can not connect to server."); 
		    }
		}else
		{
		 alert("XmlHttp's Response is not valid JSON Message\nMSG = "+output); 
		}

            }
         } else {
	   console.log("statusText: " + xmlhttp.statusText + "\nHTTP status code: " + xmlhttp.status);
         };

      };
   };
};
