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

function JSRootShellLogin(rpcurl,id,shell,logging)
{
   rpcurl =rpcurl || "http://localhost/rootrpcshell";
   id = id || "JSRootShellLogin";
   logging = logging || true;
   var sessionid;
   var username;
   var status=false;
   
   //form elements
   var logindivstatus;
   var loginbutton;
   var logindiv;
   var loginpassword;
   var loginusername;
   var dialog;
 
   
   this.GetSessionID = function(){
     return sessionid;
  }
   this.GetUserName = function(){
     return username;
  }
  this.IsLogged = function()
  {
      return status;
  }
   
   this.Init = function() {
      logindiv = document.createElement('div');
      logindiv.setAttribute('class', 'JSRootShellLogin');
      logindiv.setAttribute('id', id);


      var loginusernamelabel =  document.createElement('label');
      loginusernamelabel.setAttribute('for', 'JSRootShellLoginUserName'+id);
      loginusernamelabel.innerHTML += 'Username\n';
      logindiv.appendChild(loginusernamelabel);

      loginusername =  document.createElement('input');
      loginusername.setAttribute('id', 'JSRootShellLoginUserName'+id);
      loginusername.setAttribute('class', 'JSRootShellLoginUserName');
      loginusername.setAttribute('width', '100%');
      loginusername.setAttribute('value', 'omazapa@localhost');
      logindiv.appendChild(loginusername);

      var loginpasswordlabel =  document.createElement('label');
      loginpasswordlabel.setAttribute('for', 'JSRootShellLoginPassword'+id);
      loginpasswordlabel.innerHTML += '\nPassword\n';
      logindiv.appendChild(loginpasswordlabel);
      
      loginpassword =  document.createElement('input');
      loginpassword.setAttribute('id', 'JSRootShellLoginPassword'+id);
      loginpassword.setAttribute('class', 'JSRootShellLoginPassword');
      loginpassword.setAttribute('type', 'password');
      loginpassword.setAttribute('width', '100%');
      logindiv.appendChild(loginpassword);

      loginbutton =  document.createElement('input');
      loginbutton.setAttribute('id', 'JSRootShellLoginSubmit'+id);
      loginbutton.setAttribute('class', 'JSRootShellLoginSubmit');
      loginbutton.setAttribute('type', 'submit')
      loginbutton.setAttribute('value', 'Connect');;
      loginbutton.addEventListener('click', this.sendRequest, true);
      logindiv.appendChild(loginbutton);
      
      logindivstatus = document.createElement('img');
      logindivstatus.setAttribute('class', 'JSRootShellLoginStatus'+id);
      logindivstatus.setAttribute('id', id);
      logindivstatus.setAttribute('src', 'img/loader.gif');
      logindivstatus.setAttribute('style', 'padding-top:10px');
      
      document.body.appendChild(logindiv);
      
      
      dialog = $("#"+id).dialog({closeOnEscape: false,title: "Login",resizable: false,dialogClass: id});
      dialog.context.getElementsByClassName("ui-dialog-titlebar-close")[0].setAttribute("style","visibility: hidden");
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

   //this request is a request to authenticate
   this.sendRequest = function() {
          
      username = loginusername.value;
      var password = loginpassword.value;
      console.log(password)
      logindiv.removeChild(loginbutton);
      logindiv.appendChild(logindivstatus);
      
      
      
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
       paramstag.appendChild(getXmlParam(msg_dom,"string","Login",null));
       paramstag.appendChild(getXmlParam(msg_dom,"string",username,null));
       paramstag.appendChild(getXmlParam(msg_dom,"string",password,null));
       
       methodcalltag.appendChild(paramstag);

      var msgxmltext = new XMLSerializer().serializeToString(msg_dom);
      xmlhttp.open("POST", rpcurl, true);
      xmlhttp.setRequestHeader("Content-Type","application/x-www-form-urlencoded");
//       xmlhttp.setRequestHeader("Content-Type","text/xml; charset=utf-8");
      xmlhttp.send(msgxmltext);
      if(logging) console.log("Send XmlHttpMessage: "+msgxmltext);
      xmlhttp.ontimeout = function() {
	  console.log("The request timed out.");
	}
      xmlhttp.onreadystatechange = function(event) {
         if (xmlhttp.readyState == 4) {
            if (xmlhttp.status == 200) {
           	var xmlrep =  xmlhttp.responseXML;
                 if(logging){
                   console.log(xmlrep);  
                  }
                    status = xmlrep.getElementsByTagName("member")[1].childNodes[2].textContent;
                    sessionid  = xmlrep.getElementsByTagName("member")[0].childNodes[2].textContent;
		    if(parseInt(status) == false)
		    {
		           status = false
		           logindiv.removeChild(logindivstatus);
                           logindiv.appendChild(loginbutton);
		    }else
		    {
		      if(shell != null)
		      {
		       dialog.remove();
		       status=true;
		       shell.Init(username,sessionid);			
		      }else
		      {
 		        status=false;
			alert("Error Login: JSRootShell's object is not set to init.");
		      }
		    }
                 }
                 
           if(xmlhttp.status == 503){
		  status=false;
                  alert("ROOT XmlRpc Shell Engine is not running\nUrl:"+rpcurl);
                  logindiv.removeChild(logindivstatus);
                  logindiv.appendChild(loginbutton);
                }    

            
         } else {
//	   console.log("statusText: " + xmlhttp.statusText + "\nHTTP status code: " + xmlhttp.status);
         };
      };
   };
};
