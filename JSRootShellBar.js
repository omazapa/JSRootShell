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

function JSRootShellBar(rpcurl,id,shell,logging)
{
   rpcurl = "http://localhost/rootrpcshell";
   id = id || "JSRootShellBar";
   logging = logging || true;      
   var bardiv;
   
   //example of menu bar structure
   //    <ul id="xxx" class="JSRootMenuBar">
   // 	<li>
   // 		<a>Session</a>
   // 		<ul>
   // 			<li><a >Logout</a></li>
   // 		</ul>
   // 	</li>
   //    </ul>
   
   this.Init = function() {
      if(!shell) {alert("Error: JSRootMenuBar has not JSRootShell's object (shell) asigned."); return;}
      bardiv = document.createElement('div');
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
      barsessionquit.innerHTML = "Logout";
      barsessionquitcontainer.addEventListener('click', shell.Logout, true);

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
   
   this.Close = function(){
     document.body.removeChild(bardiv);
  }
   
   
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
     aboutmsgtext    += "Grupo de Fenomenologia de Interacciones Fundamentales <br>";
     aboutmsgtext    += "<a href='http://gfif.udea.edu.co/web/JSRootShell'  target='_blank'>http://gfif.udea.edu.co/web/JSRootShell</a><br>";
     aboutmsgtext    += "División de ciencias de la computación Gfifdev <br>";
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
};
