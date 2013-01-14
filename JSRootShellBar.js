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

function JSRootShellBar(rpcurl,id,logging)
{
   rpcurl = "http://localhost/rootrpcshell";
   id = id || "JSRootShellBar";
   logging = logging || true;   
 
   this.SetShell = function(_shell){
     shell=_shell;
    }//this method is a callback that was called when the user is ready logged
   
   
   
   //example of menu bar structure
   //    <ul id="xxx" class="JSRootMenuBar">
   // 	<li>
   // 		<a>Session</a>
   // 		<ul>
   // 			<li><a >Quit</a></li>
   // 		</ul>
   // 	</li>
   //    </ul>
   
   this.Init = function() {
      var bardiv = document.createElement('div');
      bardiv.setAttribute('class', 'JSRootShellBar');
      bardiv.setAttribute('id', id);

      //session items
      var barsession =  document.createElement('li');
      barsession.setAttribute('class','JSRootShellBarSubMenu');
      bardiv.appendChild(barsession);

      var barsessiontitle =  document.createElement('a');
      barsessiontitle.innerHTML = "Session";
      barsession.appendChild(barsessiontitle);
      
      var barsessioncontainer =  document.createElement('ul');

      var barsessionquitcontainer =  document.createElement('li');
      var barsessionquit =  document.createElement('a');
      barsessionquit.innerHTML = "Quit";
      
      barsessionquitcontainer.appendChild(barsessionquit);
      barsessioncontainer.appendChild(barsessionquitcontainer);
      barsession.appendChild(barsessioncontainer);

      
      //help items
      var barhelp =  document.createElement('li');
      barhelp.setAttribute('class','JSRootShellBarSubMenu');
      bardiv.appendChild(barhelp);

      var barhelptitle = document.createElement('a');
      barhelptitle.innerHTML = "Help";
      barhelp.appendChild(barhelptitle);
      
      var barhelpcontainer =  document.createElement('ul');
      
      var barhelpaboutcontainer =  document.createElement('li');
      var barhelpabout = document.createElement('a');
      barhelpabout.innerHTML = "About";
      barhelpaboutcontainer.addEventListener('click', this.About, true);
      
      barhelpaboutcontainer.appendChild(barhelpabout);
      barhelpcontainer.appendChild(barhelpaboutcontainer);
      barhelp.appendChild(barhelpcontainer);
      
      
      document.body.appendChild(bardiv);
      $("#"+id).menubar({autoExpand: true,
			menuIcon: true,
			buttons: true,});
   };
   
   var seed=0;
   this.About = function(){
     seed++;
     var aboutdiv = document.createElement('div');
     aboutdiv.setAttribute('id','JSRootShellBarAbout'+id+seed.toString());
     aboutdiv.setAttribute('class','JSRootShellBarAbout');
     aboutdiv.setAttribute('style','height: 250px;width: 500px;');
     
     var aboutimg = document.createElement('img');
     aboutimg.setAttribute('src','img/root.jpg');
     aboutimg.setAttribute('style','border:5px solid gray;border-radius: 5px;height: 269px;width: 458px;');
     var aboutmsg = document.createElement('div');
     
     var aboutmsgtext = "Copyright (C) 2013, Omar Andres Zapata Mesa<br>";
     aboutmsgtext    += "For the license see $ROOTSYS/README/LICENSE. <br>";
     aboutmsgtext    += "For the list of contributors see $ROOTSYS/README/CREDITS."
     aboutmsg.innerHTML = "<h5 style='color:#585858'>"+aboutmsgtext+"</h5>";
     
     aboutdiv.appendChild(aboutimg);
     aboutdiv.appendChild(aboutmsg);
     
     document.body.appendChild(aboutdiv);
     
     $("#"+'JSRootShellBarAbout'+id+seed.toString()).dialog({closeOnEscape: true,
							title: "About",resizable: false,
							dialogClass: 'JSRootShellBarAbout'+id, 
							height: 450, width: 500,
       close:function(){
	 this.remove();
      }
    }); 
  }
    
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
      xmlhttp.setRequestHeader("Content-Type","text/xml; charset=utf-8");
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
		           logindiv.removeChild(logindivstatus);
                           logindiv.appendChild(loginbutton);
		    }else
		    {
		      if(shell != null)
		      {
		       dialog.remove();
		       shell.Init(username,sessionid);			
		      }else
		      {
			alert("Error Login: JSRootShell's object is not set to init.");
		      }
		    }
                 }
                 
           if(xmlhttp.status == 503){
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
