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

function JSRootPrompt() {
	// line number in prompt
	this.number = 0;
	this.print = function() {
		document.write(this.getPrompt());
	};
	this.getPrompt = function() {
		var prePrompt = "root [" + this.number + "] ";
		var inputPromptId = " id=\"JSRootPrompt" + this.number.toString() + "\" ";
		var inputPromptEvents = " onkeypress=\"parseInputEvent()\"  onkeyup=\"callHistoryUp()\" onkeydown=\"callHistoryDown()\" ";
		var inputPromptStyle = " style=\"border-style:none;\" ";
		var inputPromptFull = prePrompt + "<input type=\"text\"" + inputPromptId + inputPromptEvents + inputPromptStyle + ">";
		return inputPromptFull;
	};
	this.parseInput = parseInputEvent;
	this.history = [];
	this.history_position = 0;
}

var RootPrompt = new JSRootPrompt();

function callHistoryUp() {
	var key;
	if (window.event) {
		key = event.keyCode;
	} else {
		key = e.which;
	}

	if (key == _up) {
		var line = document.getElementById("JSRootPrompt"+ RootPrompt.number.toString());

		if ((RootPrompt.history.length > 0)
				&& (RootPrompt.history_position < RootPrompt.history.length)) {
			line.setAttribute("value",RootPrompt.history[RootPrompt.history_position]);
			RootPrompt.history_position++;
		}
	}

}

function callHistoryDown() {
	var key;
	if (window.event) {
		key = event.keyCode;
	} else {
		key = e.which;
	}

	if (key == _down) {
		var line = document.getElementById("JSRootPrompt"+ RootPrompt.number.toString());

		if ((RootPrompt.history.length > 0)
				&& (RootPrompt.history_position > 0)) {
			line.setAttribute("value",
					RootPrompt.history[RootPrompt.history_position]);
			RootPrompt.history_position--;
		}
	}
}

function parseInputEvent() {
	var key;
	if (window.event) {
		key = event.keyCode;
	} else {
		key = e.which;
	}

	if (key == _enter) {

		var line = document.getElementById("JSRootPrompt"+ RootPrompt.number.toString());
		line.setAttribute("readonly");
		RootPrompt.history.push(line.value);

		RootPrompt.number++;
		document.write("<br>");
		RootPrompt.print();
		RootPrompt.history_position = 0;
	}

}

function InitShell() {
	document.write(RootPrompt.getPrompt());
}
