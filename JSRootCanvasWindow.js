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


function JSRootCanvasWindow(id)
{
   var elem = document.getElementById(id);
   if(elem)
   {
      this.win    = elem;
      this.winbox = this.win.childNodes[0];
      this.url    = this.winbox.src;
   }else{
   this.win = document.createElement("div");
   this.win.setAttribute("id", id);
   this.win.setAttribute("title", id);
   this.win.setAttribute("display", "none");
   
   this.winbox = document.createElement("img");
   this.winbox.setAttribute("id", id+"-box");
   this.winbox.setAttribute("display", "none");
   this.win.appendChild(this.winbox);
   
   document.body.appendChild(this.win);
   this.dialog = $("#"+id).dialog({autoOpen: false,dialogClass:id}); 
   
   var widgetcode = this.dialog.context.getElementsByClassName("ui-dialog ui-widget ui-widget-content ui-corner-all "+id+" ui-draggable ui-resizable");
   
   var header = $(widgetcode[0]).children(".ui-dialog-titlebar.ui-widget-header.ui-corner-all.ui-helper-clearfix");
         
   var icon = document.createElement("div");
   icon.setAttribute("class", "ui-dialog-title");
   this.winbox.setAttribute("status", "show");
   icon.setAttribute("id", id+"accordion");
   icon.innerHTML = '<span class="ui-accordion-header-icon ui-icon ui-icon-triangle-1-s"></span>';
   header[0].insertBefore(icon,header[0].childNodes[0]);
   
   icon.onclick = function (){
             var status = $("#"+id+"-box").attr("status");
             if(status == "show")
             {
               $("#"+id+"-box").attr("status", "hide");
               icon.innerHTML = '<span class="ui-accordion-header-icon ui-icon ui-icon-triangle-1-e"></span>';
               $( "#"+id ).dialog( "option", "height", 50 );
               $( "#"+id ).dialog( "option", "width", 50);

             }else
             {
               $("#"+id+"-box").attr("status", "show");
               icon.innerHTML = '<span class="ui-accordion-header-icon ui-icon ui-icon-triangle-1-s"></span>';
               $( "#"+id ).dialog( "option", "height", $("#"+id+"-box").height()+50 );
               $( "#"+id ).dialog( "option", "width", $("#"+id+"-box").width() +50);
             }
            $("#"+id+"-box").slideToggle(0);

         };
   }
  this.setImg = function(url){
      this.url = url;
      this.winbox.setAttribute("src",url);
      this.winbox.onload = function(){
          $( "#"+id ).dialog( "option", "height", $("#"+id+"-box").height()+50 );
          $( "#"+id ).dialog( "option", "width", $("#"+id+"-box").width() +50);
      }
  }       
  this.show = function(){$("#"+id).dialog("open");}
  this.close = function(){$("#"+id).dialog("close");}
  this.update = function(){
      this.winbox.setAttribute("src",this.url);
      this.winbox.onload = function(){
          //trick to conserve the actual size of window
          //if the img size was change it will be 
          //adjusted.
          $("#"+id+"-box").slideToggle(0);
          $("#"+id+"-box").slideToggle(0);          
    }
  }
};
