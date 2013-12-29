#include "stdafx.h"
#include "Office Automation Client.h"
#include "PacketType.h"
#include "ScheduleDlg.h"

unsigned WINAPI COfficeAutomationClientApp::NetRecieveThread( void* pOfficeAutomationClientApp)
{
  COfficeAutomationClientApp* app = (COfficeAutomationClientApp*)pOfficeAutomationClientApp;

  RakNet::Packet* packet;

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
        int departmentID;
        RakNet::RakString departmentName;
        bsIn.Read(departmentID);
        bsIn.Read( departmentName);
        theApp.m_departmentName[departmentID] = departmentName.C_String();
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

    app->m_peer->DeallocatePacket( packet);
  }

  return 0;
}