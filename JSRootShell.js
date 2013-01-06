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


function JSRootShell(url,id, style,logging)
{
   url = url || "http://localhost/rootrpc";
   id = id || "JSRootShell";
   style = style || "";
   logging = logging || true;
   var number = 0;
    this.updateStyle = function(newstyle) {
      document.getElementById(id).setAttribute("style", newstyle);
   };
   
   var connecting=false;
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
      this.shelltable.setAttribute("width", "100%");
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
  /*
   * this function return a xml's element with the next structure
   * <param>
   *    <value><string>I am the param</string></value>
   * </param>
   * where type="string" and value="I am the param", 
   * iscdada is optional  arg, when iscdata is true
   * tha tag type is 
   * <string><![CDATA[Some <CDATA> I am the param]]></string>
   * show http://xmlrpc-c.sourceforge.net/doc/libxmlrpc.html#valuetype
   * for all supported types.
   */
  function getXmlParam(xmldoc,type,value,iscdata)
  {
       var paramtag  = xmldoc.createElement("param");
       var valuetag  = xmldoc.createElement("value");
       var typetag   = xmldoc.createElement(type);
       var datatag;
       if(iscdata)
        {
          datatag   = xmldoc.createCDATASection(value);
        }else
        {
          datatag  = xmldoc.createTextNode(value);
        }
        
       typetag.appendChild(datatag);
       valuetag.appendChild(typetag);
       paramtag.appendChild(valuetag);
       return paramtag;
  }

   this.sendRequest = function() {

//       var code = "code="+encodeURI(this.currentPrompt.getCode());

//      var code = "code="+encodeURIComponent(this.currentPrompt.getCode());
      var code = this.currentPrompt.getCode();
      var promptid = this.currentPrompt.getId();
      //initialization of xmlrequest object
      if (window.XMLHttpRequest) {
          xmlhttp = new XMLHttpRequest();
       } else {
          xmlhttp = new ActiveXObject("Microsoft.XMLHTTP");
       }
       //initialization of xml message
       //request types are processline, tabcompletion, history up/down
       var msg_dom = document.implementation.createDocument(null, "methodCall",null);
       
       var methodcalltag    = msg_dom.getElementsByTagName("methodCall")[0];
       
       var methodnametag    = msg_dom.createElement("methodName");
       methodnametag.appendChild(msg_dom.createTextNode("processLine"));
       methodcalltag.appendChild(methodnametag);
       
       
       var paramstag    = msg_dom.createElement("params");
       paramstag.appendChild(getXmlParam(msg_dom,"string",promptid,null));
       paramstag.appendChild(getXmlParam(msg_dom,"string",code,null));

       methodcalltag.appendChild(paramstag);

      var msgxmltext = new XMLSerializer().serializeToString(msg_dom);
      xmlhttp.open("POST", url, true);
      xmlhttp.setRequestHeader( "Content-Type", "text/xml; charset=utf-8" );
      xmlhttp.send(msgxmltext);
      if(logging) console.log("Send JSON's XmlHttpMessage: "+msgxmltext);
      xmlhttp.ontimeout = function() {
	  console.log("The request timed out.");
	}
      xmlhttp.onreadystatechange = function(event) {
         if (xmlhttp.readyState == 4) {
            if (xmlhttp.status == 200) {
           	var xmlrep =  xmlhttp.responseXML;
                 var canvas     = xmlrep.getElementsByTagName("member")[0].childNodes[2].textContent;
                 var canvasfile = xmlrep.getElementsByTagName("member")[1].childNodes[2].textContent;
                 var pid        = xmlrep.getElementsByTagName("member")[2].childNodes[2].textContent;
                 var stderr     = xmlrep.getElementsByTagName("member")[3].childNodes[2].textContent;
                 var stdout     = xmlrep.getElementsByTagName("member")[4].childNodes[2].textContent;
                 
                 var prompt = document.getElementById(promptid);
                 if(logging){
                   console.log("canvas:"+canvas);
                   console.log("canvasfile:"+canvasfile);
                   console.log("pid:"+pid);
                   console.log("stdout:"+stdout);
                   console.log("stderr:"+stderr);
                  }
                 
                 if(stdout){
                    if(stdout.trim().length != 0){
                        prompt.value += '\n'+stdout;
                     } 
                 }
                 
 	        if(stderr) {
                    if(stderr.trim().length != 0){
                    prompt.value += '\n'+stderr;
                     }
                 }

                prompt.style.height = prompt.scrollHeight + 'px';
            }
            if(xmlhttp.status == 503){
                connecting=confirm("The Server is not running, do you want start it?");
                if (connecting==true)
		{
		  xmlhttp.open("POST", "JSRootShellServer.php", true);
                  xmlhttp.setRequestHeader( "Content-Type", "text/xml; charset=utf-8" );
                  xmlhttp.send();
		  xmlhttp.onreadystatechange = function(){ }
                  alert("Server is starting...");
		}
                else
                {
                  alert("Server is starting...");                    
                }    
	    }
         } else {
//	   console.log("statusText: " + xmlhttp.statusText + "\nHTTP status code: " + xmlhttp.status);
         };
      };
   };
};
