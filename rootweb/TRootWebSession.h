/*************************************************************************
 * Copyright (C) 2013,  Gfif Developers                                   *
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

#ifndef ROOT_TRootWebSession
#define ROOT_TRootWebSession
#include<string>

namespace ROOTWEB
{
  class TUserSession
  {
  private:
    std::string fSessionID;
    std::string fUserName;
    std::string fEngineUrl;
    int fPid;
  public:
    TUserSession(std::string user,std::string  sessionid,std::string  pid,std::string  engineurl):fUserName(user),fSessionID(sessionid),fPid(pid),fEngineUrl(engineurl){}
    ~TUserSession(){};
    
    void SetSessionID(std::string sessionid){fSessionID=sessionid;}
    void SetUserName(std::string user){fUserName=user;}
    void SetPID(int pid){fPid=pid;}
    void SetEngineURL(std::string engineurl){fEngineUrl=engineurl;}
    std::string GetSessionID(){return fSessionID;}
    std::string GetUserName(){return fUserName;}
    int GetPID(){return fPid;}
    std::string GetEngineUrl(){return fEngineUrl;}
    
    bool operator == (TUserSession &session)
    {
      if((fSessionID == session.fSessionID)&&(fUserName == session.fUserName)&&(fPid == session.fPid)&&(fEngineUrl == session.fEngineUrl))
      {
	return true;
      }else{
	return false;
      }
    }
  };  
}


#endif
