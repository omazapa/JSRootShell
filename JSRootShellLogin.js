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

function JSRootShellLogin(rpcurl,id,logging)
{
   rpcurl = "http://localhost/rootrpcshell";
   id = id || "JSRootShellLogin";
   logging = logging || true;

   this.Init = function() {
      this.logindiv = document.createElement('div');
      this.logindiv.setAttribute('class', 'JSRootShellLogin');
      this.logindiv.setAttribute('id', id);


      this.loginusernamelabel =  document.createElement('label');
      this.loginusernamelabel.setAttribute('for', 'JSRootShellLoginUserName');
      this.loginusernamelabel.innerHTML += "Username\n";
      this.logindiv.appendChild(this.loginusernamelabel);

      this.loginusername =  document.createElement('input');
      this.loginusername.setAttribute('id', 'JSRootShellLoginUserName');
      this.loginusername.setAttribute('class', 'JSRootShellLoginUserName');
      this.loginusername.setAttribute("width", "100%");
      this.logindiv.appendChild(this.loginusername);

      this.loginpasswordlabel =  document.createElement('label');
      this.loginpasswordlabel.setAttribute('for', 'JSRootShellLoginPassword');
      this.loginpasswordlabel.innerHTML += "\nPassword\n";
      this.logindiv.appendChild(this.loginpasswordlabel);
      
      this.loginpassword =  document.createElement('input');
      this.loginpassword.setAttribute('id', 'JSRootShellLoginPassword');
      this.loginpassword.setAttribute('class', 'JSRootShellLoginPassword');
      this.loginpassword.setAttribute('type', 'password');
      this.loginpassword.setAttribute("width", "100%");
      this.logindiv.appendChild(this.loginpassword);

      this.loginbutton =  document.createElement('input');
      this.loginbutton.setAttribute('id', 'JSRootShellLoginSubmit');
      this.loginbutton.setAttribute('class', 'JSRootShellLoginSubmit');
      this.loginbutton.setAttribute('type', 'submit')
      this.loginbutton.setAttribute('value', 'Connect');;
      this.loginbutton.setAttribute('style', 'border:5px solid gray;border-radius: 4px;');
      this.logindiv.appendChild(this.loginbutton);

      
      
      
      document.body.appendChild(this.logindiv);
      
      
      this.dialog = $("#"+id).dialog({closeOnEscape: false,title: "Login",resizable: false,dialogClass: id});
      this.dialog.context.getElementsByClassName("ui-dialog-titlebar-close")[0].setAttribute("style","visibility: hidden");
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
       paramstag.appendChild(getXmlParam(msg_dom,"string",promptid,null));
       paramstag.appendChild(getXmlParam(msg_dom,"string",code,null));

       methodcalltag.appendChild(paramstag);

      var msgxmltext = new XMLSerializer().serializeToString(msg_dom);
      xmlhttp.open("POST", rpcurl, true);
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
                 var canvases_names  = xmlrep.getElementsByTagName("member")[0];
                 var canvases_size   = xmlrep.getElementsByTagName("member")[1].childNodes[2].textContent;
                 var pid             = xmlrep.getElementsByTagName("member")[2].childNodes[2].textContent;
                 var stderr          = xmlrep.getElementsByTagName("member")[3].childNodes[2].textContent;
                 var stdout          = xmlrep.getElementsByTagName("member")[4].childNodes[2].textContent;
                 
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
