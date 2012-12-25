/*************************************************************************
* Copyright (C) 2012,  Gfif Developers                                   *
* Grupo de Fenomenologia de Interacciones Fundamentales                  *
* http://gfif.udea.edu.co                                                *
* División de ciencias de la computación Gfifdev                         *
* http://gfifdev.udea.edu.co                                             *
* Class Created and Maintained By Omar Andres Zapata Mesa                *
* All rights reserved.                                                   *
*                                                                        *
*                                                                        *
* For the list of contributors see $ROOTSYS/README/CREDITS.              *
*************************************************************************/

#ifndef ROOT_TStdIOHandler
#define ROOT_TStdIOHandler
#include<string>
#include<sstream>
#include<iostream>
#include<fstream>

class TStdIOHandler
{
public:
  TStdIOHandler(){
  capturing=false;  
  }
  void InitCapture()
  {
    if(!capturing)
    {
      oldstderr = std::cerr.rdbuf(stderrbuffer.rdbuf());
      oldstdout = std::cout.rdbuf(stdoutbuffer.rdbuf());
      capturing = true;
    }
  }
  
  void EndCapture()
  {
    if(capturing)
    {
      std::cout.rdbuf(oldstdout);
      std::cerr.rdbuf(oldstderr);
      capturing = false;
    }
  }
  
  std::string getStdout()
  {
   return stdoutbuffer.str(); 
  }

  std::string getStderr()
  {
   return stderrbuffer.str(); 
  }

  void clear(){
    stdoutbuffer.str("");
    stderrbuffer.str("");
  }
  
private:
  bool capturing;
  
  std::stringstream stdoutbuffer;
  std::streambuf * oldstdout;
  
  std::stringstream stderrbuffer;
  std::streambuf * oldstderr;
};
#endif
