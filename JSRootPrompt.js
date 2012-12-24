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

function JSRootPrompt(number, shell)
{
   // line number in prompt
   number = number || "0";
   number = number.toString();
   this.keypressEvent = function() {
      ///////////////////////////////////////////////////
      //adjust the promot in text area with every event//
      ///////////////////////////////////////////////////
      adjustPrompt();

      var key;
      if (window.event) {
         key = event.keyCode;
      } else {
         key = event.witch;
      }

      if (key == _enter) {
    	  //////////////////////
         //shell callback here//
    	 //////////////////////
         shell.sendRequest();
         shell.newPrompt();
      }
   };

   this.getId = function() {
      return "JSRootPrompt" + number;
   };

   var prompt = document.createElement('div');
   prompt .setAttribute('class', 'JSRootPrompt');
   prompt.setAttribute("style", "display: inline-block;vertical-align: top;");

   var preprompt = document.createTextNode("root [" + number.toString() + "] ");
   prompt.appendChild(preprompt);

   var style = 'border: 0 none white; overflow: hidden;padding: 0;outline: none;resize: none;width: 85%;';
   var promptInput = document.createElement('textarea');
   promptInput.setAttribute('value', '');
   promptInput.select();
   promptInput.setAttribute('class', 'JSRootPrompt');
   promptInput.setAttribute('style', style);
   promptInput.setAttribute("autofocus", "autofocus");
   promptInput.setAttribute('id', this.getId());
   promptInput.addEventListener('keypress', this.keypressEvent, false);
   prompt.appendChild(promptInput);

   function adjustPrompt() {
      promptInput.style.height = 'auto';
      promptInput.style.height = promptInput.scrollHeight + 'px';
   }

   this.getElement = function() {
      return prompt;
   };

   this.setReadOnly = function(status) {
         promptInput.setAttribute("readonly");
   };

   this.getCode = function() {
      return promptInput.value;
   };
}
