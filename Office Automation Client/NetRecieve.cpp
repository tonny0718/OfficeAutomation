#include "stdafx.h"
#include "Office Automation Client.h"
#include "PacketType.h"
#include "ScheduleDlg.h"
#include "NewMessageDlg.h"

unsigned WINAPI COfficeAutomationClientApp::NetRecieveThread( void* pOfficeAutomationClientApp)
{
  COfficeAutomationClientApp* app = (COfficeAutomationClientApp*)pOfficeAutomationClientApp;

  RakNet::Packet* packet;
  theApp.mailCount = 0;

  while(1)
  {
    for( packet = app->m_peer->Receive(); !packet; packet = app->m_peer->Receive() )
      Sleep(200);

    if( packet->data[0] == ID_CONNECTION_LOST || packet->data[0] == ID_DISCONNECTION_NOTIFICATION)
    {
      MessageBox( NULL, "与服务器失去连接", "错误", MB_OK);
      app->Exit();
    }
    else if(packet->data[0] == RH_DEPARTMENT_INFORMATION)
    {
      RakNet::BitStream bsIn(packet->data, packet->length, false);
      bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
      int count;
      bsIn.Read( count);
      for( int i=0; i<count; i++)
      {
        int departmentID,departmentMaster;
        RakNet::RakString departmentName;
        bsIn.Read(departmentID);
        bsIn.Read( departmentName);
        theApp.m_departmentName[departmentID] = departmentName.C_String();
        bsIn.Read( departmentMaster);
        theApp.m_departmentMaster[departmentID] = departmentMaster;
      }
    }
    else if( packet->data[0] == RH_SCHEDULE)
    {
      RakNet::BitStream bsIn( packet->data, packet->length, false);
      bsIn.IgnoreBytes( sizeof(RakNet::MessageID));
      int option;
      bsIn.Read(option);
      if( option == SO_FIRST)
      {
        int count;
        bsIn.Read( count);
        for( int i=1; i<=count; i++)
        {
          int ID;
          RakNet::RakString time;
          RakNet::RakString intent;
          bsIn.Read( ID);
          bsIn.Read( time);
          bsIn.Read( intent);

          theApp.m_schedule[time.C_String()] = std::make_pair( intent.C_String(), ID);
        }
      }
      else if( option == SO_NEW)
      {
        int fromID, toID,ID;
        bsIn.Read( fromID);
        bsIn.Read( toID);
        bsIn.Read( ID);
        RakNet::RakString time, intent;
        bsIn.Read( time);
        bsIn.Read( intent);
        theApp.m_schedule[time.C_String()] = std::make_pair( intent.C_String(), ID);
      }
      theApp.m_scheduleDlg.SendMessage( WM_UPDATELIST);
    }
    else if( packet->data[0] == RH_MATES)
    {
      RakNet::BitStream bsIn( packet->data, packet->length, false);
      bsIn.IgnoreBytes( sizeof(RakNet::MessageID));
      int count;
      bsIn.Read(count);
      for( int i=1; i<=count; i++)
      {
        int ID,department;
        RakNet::RakString name;
        bsIn.Read( ID);
        bsIn.Read(name);
        bsIn.Read(department);
        theApp.m_mates[ID] = std::make_pair( name.C_String(), department);
      }
    }
    else if( packet->data[0] == RH_NOTE)
	  {
		  RakNet::BitStream bsIn( packet->data, packet->length, false);
		  bsIn.IgnoreBytes( sizeof(RakNet::MessageID));
		  RakNet::RakString note;
		  bsIn .Read( note);
		  theApp.m_note = note;
	  }
    else if( packet->data[0] == RH_SET_DEPARTMENT)
    {
      RakNet::BitStream bsIn( packet->data, packet->length, false);
      bsIn.IgnoreBytes( sizeof(RakNet::MessageID));
      int option;
      bsIn.Read( option);
      if( option == SDO_NEW)
      {
        int departmentID;
        RakNet::RakString departmentName;
        bsIn.Read( departmentID);
        bsIn.Read( departmentName);
        theApp.m_departmentName[departmentID] = departmentName.C_String();
        theApp.m_systemDlg.SendMessage(WM_UPDATE);
      }
      else if( option == SDO_DELETE)
      {
        int departmentID;
        bsIn.Read(departmentID);
        theApp.m_departmentName.erase(departmentID);
        theApp.m_departmentMaster.erase( departmentID);
        for( map<int, pair<string, int> >::iterator iter = theApp.m_mates.begin();
          iter != theApp.m_mates.end(); )
        {
          if( iter->second.second == departmentID)
            theApp.m_mates.erase(iter++);
          else
            ++iter;
        }
        theApp.m_systemDlg.SendMessage( WM_UPDATE);
      }
      else if( option == SDO_SET_MASTER)
      {
        int departmentID,masterID;
        bsIn.Read(departmentID);
        bsIn.Read(masterID);
        theApp.m_departmentMaster[departmentID] = masterID;
        theApp.m_systemDlg.SendMessage(WM_UPDATE);
      }
    }
    else if( packet->data[0] == RH_SET_HUMAN)
    {
      RakNet::BitStream bsIn(packet->data, packet->length, false);
      bsIn.IgnoreBytes( sizeof(RakNet::MessageID));
      int option;
      bsIn.Read( option);
      if( option == SHO_NEW)
      {
        int ID, departmentID;
        RakNet::RakString name;
        bsIn.Read( ID);
        bsIn.Read( name);
        bsIn.Read( departmentID);
        theApp.m_mates[ID] = std::make_pair( string(name.C_String()), departmentID);
        theApp.m_systemDlg.SendMessage(WM_UPDATE);
      }
      else if( option == SHO_DELETE)
      {
        int ID;
        bsIn.Read( ID);
        theApp.m_mates.erase(ID);
        theApp.m_systemDlg.SendMessage(WM_UPDATE);
      }
      else if( option == SHO_CHANGE_NO_PASSWORD)
      {
        int ID, departmentID;
        RakNet::RakString name;
        bsIn.Read( ID);
        bsIn.Read( name);
        bsIn.Read( departmentID);
        theApp.m_mates[ID] = std::make_pair( string(name.C_String()), departmentID);
        theApp.m_systemDlg.SendMessage(WM_UPDATE);
      }
    }
    else if( packet->data[0] == RH_NEWMESSAGE)
	  {
		  RakNet::BitStream bsIn( packet->data, packet->length, false);
		  bsIn.IgnoreBytes( sizeof(RakNet::MessageID));
		  RakNet::RakString Message;
		  int t;
		  int FromUser;
		  bsIn.Read( FromUser);
		  bsIn.Read( Message);
		  bsIn.Read( t);
		  char* strmsg = new char[1000];
		  if(FromUser == 255)
		  {
			  sprintf(strmsg,"系统消息:\n\t%s",Message.C_String());
		  }
		  else
		  {
			  map<int, pair<string, int> >::iterator iter;
			  iter = theApp.m_mates.find(FromUser);
			  string name;
			  if( iter != theApp.m_mates.end())
			  {
				  name = theApp.m_departmentName[iter->second.second] + " " + iter->second.first;
			  }

			  sprintf(strmsg,"%s:\n\t%s",name.c_str(),Message.C_String());
		  }
		
		  //弹窗
      unsigned ret;
      _beginthreadex( NULL, 0, NewMessageThread, strmsg, 0, &ret);
		  if(FromUser != 255)
		  {
			  //返回收到消息
			  RakNet::BitStream out;
			  out.Write( (RakNet::MessageID)RH_SENDMESSAGE);
			  out.Write( RECEIVE);
			  out.Write( FromUser);
			  out.Write( theApp.m_ID);
			  out.Write( t);
			  theApp.m_peer->Send( &out, HIGH_PRIORITY, RELIABLE_ORDERED, 0, theApp.m_serverAddress, false);
		  }
    }
	else if( packet->data[0] == RH_SENDMAIL)//收到新公文
	{
		RakNet::BitStream bsIn( packet->data, packet->length, false);
		bsIn.IgnoreBytes( sizeof(RakNet::MessageID));
		RakNet::RakString title,content;
		int fromUser,toDepartment,isRead,t,type;
		bsIn.Read( fromUser);
		bsIn.Read( toDepartment);
		bsIn.Read( title);
		bsIn.Read( content);
		bsIn.Read( isRead);
		bsIn.Read( t);
		bsIn.Read( type);

		theApp.tmpmail.fromUser = fromUser;
		theApp.tmpmail.toDepartment = toDepartment;
		theApp.tmpmail.title = title;
		theApp.tmpmail.content = content;
		theApp.tmpmail.isRead = isRead;
		theApp.tmpmail.t = t;
		theApp.tmpmail.type = type;

		int flag = 0;
		for(int n=0;n<theApp.mailCount;n++)
		{
			if( theApp.mailList[n].fromUser == fromUser && theApp.mailList[n].toDepartment == toDepartment 
				&& theApp.mailList[n].t == t && theApp.mailList[n].type == type)
			{
				flag = 1;
				theApp.mailList[n].isRead = isRead;
				break;
			}
		}
		if(!flag)
		{
			theApp.mailList[theApp.mailCount] = theApp.tmpmail;
			theApp.mailCount++;
		}


		if(isRead ==2 || isRead == 3 || isRead == 4)
		{
			//弹窗
			char* strmsg = new char[1000];
			sprintf(strmsg,"公文(%d)",isRead);
			unsigned ret;
			_beginthreadex( NULL, 0, NewMessageThread, strmsg, 0, &ret);
		}
	}
    app->m_peer->DeallocatePacket( packet);
  }
  return 0;
}