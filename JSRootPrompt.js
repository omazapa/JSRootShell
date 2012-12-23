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

function JSRootPrompt(number,shell) {
	// line number in prompt
	number = number || "0";
	number = number.toString();
	this.keypressEvent = function(){
			var key;
			if (window.event) {
				key = event.keyCode;
			}else
			{
				key = event.witch;
			}

			if (key == _enter) {
			  //shell callback here
				shell.newPrompt();
//				alert(document.getElementById("JSRootPrompt" + number).value);
			}
	};

	var prompt = document.createElement('div');
	prompt.setAttribute('class','JSRootPrompt');
	prompt.innerHTML="root [" + number.toString() + "] ";

	var promptInput = document.createElement('input');
	promptInput.setAttribute('class','JSRootPrompt');
	promptInput.setAttribute('type','text');
	promptInput.setAttribute("autofocus","autofocus");
	promptInput.setAttribute('id', "JSRootPrompt" + number.toString());
	promptInput.addEventListener('keypress', this.keypressEvent, true);

	promptInput.focus();
	prompt.appendChild(promptInput);


	this.getElement = function() {
		return prompt;
	};

	this.setReadOnly = function(){
		promptInput.setAttribute("readonly");
    };

    this.setFocus = function(){
    	promptInput.focus();
    };
}


//function parseInputEvent() {
//	var key;
//	if (window.event) {
//		key = event.keyCode;
//	}
//
//	if (key == _enter) {
//		document.write(key);
//
////
////		var line = document.getElementById("JSRootPrompt"+ RootPrompt.number.toString());
////		line.setAttribute("readonly");
////		RootPrompt.history.push(line.value);
////
////		RootPrompt.number++;
////		document.getElementById("JSRootShell").innerHTML+="<br>"+RootPrompt.getPrompt();
////
////		RootPrompt.history_position = 0;
//	}
//
//}
//
