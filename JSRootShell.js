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


function JSRootShell(id,style) {
	id = id || "JSRootShell";
    style = style || "border-style:none;color:#000000;background:#DCDCDC;height:50%;width: 50%;";
    number = 0;
	this.updateStyle = function(newstyle)
	{
		document.getElementById(id).setAttribute("style", newstyle);
	};

	this.Init = function(){
		this.shelldiv = document.createElement('div');
		this.shelldiv.setAttribute('class','JSRootShell');
		this.shelldiv.setAttribute('id', id);
		this.shelldiv.setAttribute('style', style);

		this.currentPrompt = new JSRootPrompt(0,this);
		this.shelldiv.appendChild(this.currentPrompt.getElement());

		document.body.appendChild(this.shelldiv);
	};

    this.newPrompt= function (){
    	number++;
    	this.currentPrompt.setReadOnly();
		this.currentPrompt = new JSRootPrompt(number,this);
//		this.currentPrompt.setFocus();
		this.shelldiv.appendChild(this.currentPrompt.getElement());
    };

}
