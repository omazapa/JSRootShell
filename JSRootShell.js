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
       var msg_dom = document.implementation.createDocument(null, 'methodCall','xml');
       
       var methodcalltag    = msg_dom.getElementsByTagName("methodCall")[0];
       
       var methodnametag    = msg_dom.createElement("methodName");
       methodnametag.appendChild(msg_dom.createTextNode("processLine"));
       methodcalltag.appendChild(methodnametag);
       
       
       var paramstag    = msg_dom.createElement("params");
       paramstag.appendChild(getXmlParam(msg_dom,"string",promptid,null));
       paramstag.appendChild(getXmlParam(msg_dom,"string",code,null));

       methodcalltag.appendChild(paramstag);

      var msgxmltext = new XMLSerializer().serializeToString(msg_dom);
      var url = "http://localhost/rootrpc";
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
                var pid    = xmlrep.getElementsByTagName("member")[0].childNodes[2].textContent;
                var stderr = xmlrep.getElementsByTagName("member")[1].childNodes[2];
                var stdout = xmlrep.getElementsByTagName("member")[2].childNodes[2];

                var prompt = document.getElementById(promptid);
                if(logging){
                   console.log("stdout:"+stdout.childNodes[0].textContent);
                   console.log("stderr:"+stderr.childNodes[0].textContent);
                    }
                
                if(stdout){
                    if(stdout.childNodes[0].textContent.trimRight().length != 0){
                        prompt.value += '\n'+stdout.childNodes[0].textContent;
                    } 
                }
                
	        if(stderr) {
                    if(stderr.childNodes[0].textContent.trimRight().length != 0){
                    prompt.value += '\n'+stderr.childNodes[0].textContent;
                    }
                }
                prompt.style.height = prompt.scrollHeight + 'px';
//                alert(xmlrep.getElementsByTagName("i4").nodeValue);
            }
         } else {
//	   console.log("statusText: " + xmlhttp.statusText + "\nHTTP status code: " + xmlhttp.status);
         };
      };
   };
};
