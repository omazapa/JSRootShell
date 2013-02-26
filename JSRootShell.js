/*************************************************************************
* Copyright (C) 2013,  Gfif Developers                                   *
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

var canvasurl = "";
var canvasformat = "";

function JSRootShell(rpcurl,id, style,logging)
{
   rpcurl = rpcurl || "http://localhost/rootrpc";
   id = id || "JSRootShell";
   style = style || "";
   logging = logging || true;
   
   //promptid
   var number = 0;
   
   //session information
   var username=null;
   var sessionid=null;
   var shell=this;
   
   var history;

   
   var login = new JSRootShellLogin(rpcurl,id+"Login",this,logging);
   var bar = new JSRootShellBar(rpcurl,id+"Bar",this,logging);
   
   this.updateStyle = function(newstyle) {
      document.getElementById(id).setAttribute("style", newstyle);
   };
   
   this.setCanvasUrl = function(url){
       canvasurl = url;
   }

   this.setCanvasFormat = function(format){
       canvasformat = format;
   }
   
   this.Logout = function(){
      login.sendLogoutRequest();
      bar.Close();
    }
   
   this.Init = function(_username,_sessionid) {
     
      if(!login.IsLogged()){
	login.Init();
	return;
      }
      bar.Init();
      username=_username;
      sessionid=_sessionid;
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
    
    //this should be changed for promptRquest to get real prompt number
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

  function error(){    
          alert("Error: can not connect to \n"+rpcurl+"\nRestart shell engine\nor fix you url for rpc server.");
	  return;
  }  

  this.sendRequestOutput = function(promptid) {
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
       methodnametag.appendChild(msg_dom.createTextNode("ShellEngine"));
       methodcalltag.appendChild(methodnametag);
       
       
       var paramstag    = msg_dom.createElement("params");
       paramstag.appendChild(getXmlParam(msg_dom,"string","Output",null));
       paramstag.appendChild(getXmlParam(msg_dom,"string",username,null));
       paramstag.appendChild(getXmlParam(msg_dom,"string",sessionid,null));

     
       methodcalltag.appendChild(paramstag);

      var msgxmltext = new XMLSerializer().serializeToString(msg_dom);
      xmlhttp.open("POST", rpcurl, true);
      xmlhttp.setRequestHeader("Content-Type","application/x-www-form-urlencoded");
      xmlhttp.upload.addEventListener("error", error, false);
      xmlhttp.send(msgxmltext);	
      xmlhttp.onreadystatechange = function(event) {
        if (xmlhttp.readyState == 4) {
            if (xmlhttp.status == 200) {
           	var xmlrep =  xmlhttp.responseXML;
		 var capturing          = xmlrep.getElementsByTagName("member")[0].childNodes[2].textContent;
		 var stderr             = xmlrep.getElementsByTagName("member")[1].childNodes[2].textContent;
                 var stdout             = xmlrep.getElementsByTagName("member")[2].childNodes[2].textContent;
		 if(logging){
                   console.log(xmlrep);  
                   console.log("capturing:"+capturing);                   
                   console.log("stdout:"+stdout);
                   console.log("stderr:"+stderr);
                  }
                 
		 var prompt = document.getElementById(promptid);
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
		  return capturing;
	    }
	}
      }
  }
   this.sendRequest = function() {

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
       methodnametag.appendChild(msg_dom.createTextNode("ShellEngine"));
       methodcalltag.appendChild(methodnametag);
       
       
       var paramstag    = msg_dom.createElement("params");
       paramstag.appendChild(getXmlParam(msg_dom,"string","ProcessLine",null));
       paramstag.appendChild(getXmlParam(msg_dom,"string",username,null));
       paramstag.appendChild(getXmlParam(msg_dom,"string",sessionid,null));
       paramstag.appendChild(getXmlParam(msg_dom,"string",promptid,null));
       paramstag.appendChild(getXmlParam(msg_dom,"string",code,null));

       methodcalltag.appendChild(paramstag);

      var msgxmltext = new XMLSerializer().serializeToString(msg_dom);
      xmlhttp.open("POST", rpcurl, false);
      xmlhttp.setRequestHeader("Content-Type","application/x-www-form-urlencoded");
      xmlhttp.upload.addEventListener("error", error, false);
      xmlhttp.send(msgxmltext);	

      if(logging) console.log("Send JSON's XmlHttpMessage: "+msgxmltext);
      xmlhttp.ontimeout = function() {
	  console.log("The request timed out.");
	}
      xmlhttp.onreadystatechange = function(event) {
         if (xmlhttp.readyState == 4) {
            if (xmlhttp.status == 200) {
           	var xmlrep =  xmlhttp.responseXML;
		 console.log(xmlrep);
		 try{
                 var canvases_names  = xmlrep.getElementsByTagName("member")[0];
                 var canvases_size   = xmlrep.getElementsByTagName("member")[1].childNodes[2].textContent;
		 var errorcode       = xmlrep.getElementsByTagName("member")[2].childNodes[2].textContent;
                 var pid             = xmlrep.getElementsByTagName("member")[3].childNodes[2].textContent;
                 var stderr          = xmlrep.getElementsByTagName("member")[4].childNodes[2].textContent;
                 var stdout          = xmlrep.getElementsByTagName("member")[5].childNodes[2].textContent;
		   
		}catch(e)
		{
		  console.log("Exception captured:\n"+e);
		  console.log("Xml Response:\n"+xmlrep);
		}
                 
                 var prompt = document.getElementById(promptid);
                 
                 canvases_size=parseInt(canvases_size);
                 var canvases_names_array=new Array();
                 for(i=0;i<canvases_size;i++)
                 {
                     var data=canvases_names.getElementsByTagName("array")[0];
                     canvases_names_array[i]=data.getElementsByTagName("value")[i].childNodes[0].textContent;
                 }
                 
                 
                 if(logging){
                   console.log(xmlrep);  
                   console.log("canvases_size:"+canvases_size);                   
                   console.log("canvases_names_array:"+canvases_names_array);
		   console.log("errorcode:"+errorcode);
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
                 
                 if(canvases_size>0)
                 {
                     for(i=0;i<canvases_size;i++)
                         {
                             var win = document.getElementById(canvases_names_array[i]);
                             var c = new JSRootCanvasWindow(canvases_names_array[i]);
                             if(win)
                             {
                             c.update();
                             }else{
                             c.setImg(canvasurl+"/"+canvases_names_array[i]+canvasformat);
                             c.show();                             
                             }
                             
                         }
                 }

                prompt.style.height = prompt.scrollHeight + 'px';
		return parseInt(errorcode);
            }
            if(xmlhttp.status == 503){
                  alert("Server is not running!!\nUrl: "+rpcurl);
		  return 3;
	    }
         } else {
//	   console.log("statusText: " + xmlhttp.statusText + "\nHTTP status code: " + xmlhttp.status);
         };
	 if (xmlhttp.readyState == 3) {
	   var xmlrep =  xmlhttp.responseXML;
           console.log("statusText: " + xmlhttp.statusText + "\nHTTP status code: " + xmlhttp.status);
	   console.log(xmlrep);
	 };
      };
   };
};
