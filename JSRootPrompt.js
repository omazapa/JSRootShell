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

   function adjustPrompt() {
      promptInput.style.height = promptInput.scrollHeight + 'px';
   }

   this.keypressEvent = function(e) {
      ///////////////////////////////////////////////////
      //adjust the promot in text area with every event//
      ///////////////////////////////////////////////////
      adjustPrompt();

      var key;
      if (window.event) {
         key = event.keyCode;
      } else {
    	 //added tu support firefox
         key = e.charCode || e.keyCode || e.which;
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

   var prompt = document.createElement('tr');
   prompt .setAttribute('class', 'JSRootPrompt');
   prompt.setAttribute("style", "width: 100%;");

   var prepromptCell = document.createElement('td');
   prepromptCell.setAttribute('class', 'JSRootPrePrompt');
   prepromptCell.setAttribute("style", "display: inline-block;vertical-align: top;resize: none;width:auto;white-space: nowrap;");
   prepromptCell.appendChild(document.createTextNode("root [" + number.toString() + "] "));
   prompt.appendChild(prepromptCell);

   var style = 'border: 0 none white; overflow: hidden;padding: 1;outline: none;resize: none;';

   var promptInputCell = document.createElement('td');
   promptInputCell.setAttribute('class', 'JSRootPromptCell');
   promptInputCell.setAttribute('style', "display: inline-block;vertical-align: top;resize: none;width: 100%;");
   var promptInput = document.createElement('textarea');
   promptInput.setAttribute('value', '');
   promptInput.select();
   promptInput.setAttribute('class', 'JSRootPromptInput');
   promptInput.setAttribute('style', "display: inline-block;vertical-align: top;resize: none;width: 100%;overflow:hidden;outline: none;");
   promptInput.setAttribute("autofocus", "autofocus");
   promptInput.setAttribute('id', this.getId());
   promptInput.addEventListener('keypress', this.keypressEvent, true);
   promptInput.addEventListener('input', this.keypressEvent, true);
   promptInput.addEventListener('haschange', this.keypressEvent, true);

   promptInputCell.appendChild(promptInput);
   prompt.appendChild(promptInputCell);

   this.getElement = function() {
      return prompt;
   };

   this.setReadOnly = function() {
         promptInput.setAttribute("readonly","true");
   };

   this.getCode = function() {
      return promptInput.value;
   };
}
