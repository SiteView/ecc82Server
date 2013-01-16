/*
 * Copyright (C) 2005 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */
#include <iostream>

#include <process.h>

#include "WApplication"
#include "WSignal_"
#include "WSignalInstance_"
#include "WSlotInstance_"
#include "WConnection_"
#include <Algorithm>

#include <windows.h>

int WSignalInstance_::m_nCount = 0;

extern void WriteLogFile(const char *pszFile, const char* pszMsg);

WSignalInstance_::WSignalInstance_(WObject *object, WSignal_ *signal)
  : signal_(signal),
    object_(object),
    exposed_(false),
    staticCodeChanged_(false),
    connectivityChanged_(false)
{
	nJavaSignal=-1;
    m_nCount ++;
}

void WSignalInstance_::SetJsVal(const char * strJs,WObject::ConnectionType type )
{
	nJavaSignal= type;
	connectivityChanged_= true;
	strJsFun=strJs;
}
WSignalInstance_::~WSignalInstance_()
{
	
  while (connections_.size() > 0)
  {
	  //char tmpchar[200];
	  //sprintf(tmpchar,"trigger  this : %08x  ",this);
    delete connections_[0];
  }
  m_nCount --;
	/*
	for(int i=0;i <connections_.size();i++)
	{
		OutputDebugString("~WSignalInstance_\n");
		delete connections_[i];
		connections_[i] =NULL;


	}*/
}

bool WSignalInstance_::connect(WSlotInstance_ *slot,
			       WObject::ConnectionType type)
{
  
	  for (unsigned i = 0; i < connections_.size(); ++i) {
		  if (connections_[i]->slot() == slot)
			  return false;
	  }
	  
	  new WConnection_(this, slot, type);
	  return true;		  

  
  
}

void WSignalInstance_::addConnection(WConnection_ *c)
{
    char buf[128];
  if (connections_.size() == 0) {
    connectivityChanged_ = true;
    object_->signalConnectionsChanged();

    if (signal_->exposable()) {
      exposed_ = true;
        //sprintf(buf, "add exposed WSignalInstance_ s%08x\n", reinterpret_cast<unsigned>(this));

        //char filename[64] = {0};
        //sprintf(filename, "e:\\treeview\\exposedSignal\\%d.log", getpid());
        //WriteLogFile(filename, buf);
      wApp->addExposedSignal(this);
    }
  }

        //sprintf(buf, "add WSignalInstance_ s%08x\n", reinterpret_cast<unsigned>(this));

        //char filename[64] = {0};
        //sprintf(filename, "e:\\treeview\\addSignal\\%d.log", getpid());
        //WriteLogFile(filename, buf);
  connections_.push_back(c);
}

void WSignalInstance_::setStaticCodeChanged()
{
  staticCodeChanged_ = true;
  object_->signalConnectionsChanged();
}

void WSignalInstance_::removeConnection(WConnection_ *c)
{
  std::vector<WConnection_ *>::iterator i
	  = std::find(connections_.begin(), connections_.end(), c);
  if (i == connections_.end()) {
    std::cerr << "Inconsitency in signal connections: could not find"
      " connection" << std::endl;
    exit(1);
  } else {
	    //char buf[128];
     //   sprintf(buf, "Remove WSignalInstance_ s%08x\n", reinterpret_cast<unsigned>(this));

     //   char filename[64] = {0};
     //   sprintf(filename, "e:\\treeview\\removeSignal\\%d.log", getpid());
     //   WriteLogFile(filename, buf);
	  //connections_[i]=NULL;
		//OutputDebugString(" removeConnection\n ");
		connections_.erase(i);
//		delete *i;

    if (connections_.size() == 0) {
      connectivityChanged_ = true;
      object_->signalConnectionsChanged();

      if (exposed_ && wApp)
	wApp->removeExposedSignal(this);
    }
  }
}

void WSignalInstance_::connectivityOk()
{
  connectivityChanged_ = false;
}

const std::string WSignalInstance_::encodeCmd() const
{
  char buf[50];
  sprintf(buf, "s%08x", reinterpret_cast<unsigned>(this));

  return std::string(buf);
}

WSignalInstance_ *WSignalInstance_::decodeSignal(const std::string cmd)
{
  unsigned signalInt;

  sscanf(cmd.c_str(), "s%x", &signalInt);
  WSignalInstance_ *result = reinterpret_cast<WSignalInstance_ *>(signalInt);


  if (wApp->isExposedSignal(result))
	  return result;
  else {
	  /*
    std::cerr << "Error: decodeSignal did not yield an exposed signal."
	      << std::endl;
		  */  
      

	  OutputDebugString("Error: decodeSignal did not yield an exposed signal.\n");
	  
	  //OutputDebugString(result->signal_->name().c_str());
	  //OutputDebugString(cmd.c_str());
      //OutputDebugString("\n");
    //exit(1);
	return NULL;
  }
}

void WSignalInstance_::trigger(WObject::ConnectionType type, void **args) const
{
	  //char tmpchar[200];
	  //sprintf(tmpchar,"trigger  this : %08x  ",this);
	  //OutputDebugString(tmpchar);  
	  //
	  //sprintf(tmpchar,"  size : %d  \n",connections_.size());
	  //OutputDebugString(tmpchar);  
	  //
        unsigned i = 0;
        try
		{
			//int nSize = connections_.size()
			for (i = 0; i < connections_.size(); ++i)
			{
				//OutputDebugString("aaaA\n");
				if (connections_[i]->type() == type)
                {
                    //if(connections_[i]->trigger(args)) 
                      //  return;
					connections_[i]->trigger(args);

                }
			}
		}
		catch(...)
		{
			char tmpchar[200];
            sprintf(tmpchar,"WSignalInstance_ trigger error(ConnectionType : %d)\n", type); 
			OutputDebugString(tmpchar);  
		}
}

void WSignalInstance_::getStaticCode(std::string& code) const
{
  for (unsigned i = 0; i < connections_.size(); ++i)
    code += connections_[i]->getStaticCode();

  staticCodeChanged_ = false;
}

WSlotInstance_ *WSignalInstance_::findConnectedNeedTriggerStaticSlot() const
{
  for (unsigned i = 0; i < connections_.size(); ++i) 
  {
    if (connections_[i]->slot()->needTriggerStatic())
      return connections_[i]->slot();
  }

  return 0;
}
