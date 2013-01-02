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


extern "C"
{
#include<string.h>  
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include<fcntl.h>
}

#define MAX_LEN 40
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
      /* save stdout/stderr for display later */
      saved_stdout = dup(STDOUT_FILENO);  
      saved_stderr = dup(STDERR_FILENO);  
      if( pipe(stdout_pipe) != 0 ) {          /* make a pipe for stdout*/
         return;
      }
      if( pipe(stderr_pipe) != 0 ) {          /* make a pipe for stdout*/
         return;
      }

      long flags = fcntl(stdout_pipe[0], F_GETFL); 
      flags |= O_NONBLOCK; 
      fcntl(stdout_pipe[0], F_SETFL, flags);
      
      flags = fcntl(stderr_pipe[0], F_GETFL); 
      flags |= O_NONBLOCK; 
      fcntl(stderr_pipe[0], F_SETFL, flags);
      
      dup2(stdout_pipe[1], STDOUT_FILENO);   /* redirect stdout to the pipe */
      close(stdout_pipe[1]);
      
      dup2(stderr_pipe[1], STDERR_FILENO);   /* redirect stderr to the pipe */
      close(stderr_pipe[1]);
      
      
      capturing = true;
    }
  }
  
  void EndCapture()
  {
    if(capturing)
    {
      fflush(stdout);
      fflush(stderr);
      int buf_readed;
      
      while(true)/* read from pipe into buffer */
      {
	buf_readed = read(stdout_pipe[0], buffer, MAX_LEN);
	if(buf_readed<=0) break;
	stdoutpipe += buffer;
      }

      while(true)/* read from pipe into buffer */
      {
	buf_readed = read(stderr_pipe[0], buffer, MAX_LEN);
	if(buf_readed<=0) break;
	stderrpipe += buffer;
      }

      dup2(saved_stdout, STDOUT_FILENO);  /* reconnect stdout*/
      dup2(saved_stderr, STDERR_FILENO);  /* reconnect stderr*/
      capturing = false;
    }
  }
  
  std::string getStdout()
  {
      return stdoutpipe;
  }

  std::string getStderr()
  {
   return stderrpipe;
  }

  void clear(){
    stdoutpipe="";
    stderrpipe="";
  }
  
private:
  bool capturing;
  //this values are to capture stdout, stderr
  std::string    stdoutpipe;
  std::string    stderrpipe;
  char buffer[MAX_LEN+1];
  int stdout_pipe[2];
  int stderr_pipe[2];
  int saved_stderr;
  int saved_stdout;
};
#endif
