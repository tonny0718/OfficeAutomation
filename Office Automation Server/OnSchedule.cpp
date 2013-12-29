#include "RakNet\BitStream.h"
#include "Global.h"
#include "Events.h"

void WINAPI OnSchedule( PTP_CALLBACK_INSTANCE pInstance, void* packet)
{
  RakNet::Packet* pIn = (RakNet::Packet*)packet;
  RakNet::BitStream bsIn(pIn->data, pIn->length, false);
  bsIn.IgnoreBytes( sizeof(RakNet::MessageID));
  int option;
  bsIn.Read( option);
  if( option == SO_DONE)
  {
    int ID;
    bsIn.Read(ID);
    char sqlCmd[1024];
    sprintf( sqlCmd, "UPDATE schedule SET isDone=1 WHERE id=%d", ID);
    g_globalData->databaseLock.Lock();
    g_globalData->database->QueryWithoutResult(sqlCmd);
    g_globalData->databaseLock.Unlock();
  }
  else if( option == SO_NEW)
  {
    int fromID, toID;
    bsIn.Read( fromID);
    bsIn.Read( toID);
    RakNet::RakString time, intent;
    bsIn.Read( time);
    bsIn.Read( intent);
    char sqlCmd[1024];
    sprintf( sqlCmd, "INSERT INTO schedule SET user_id=%d,time='%s',intent='%s'", toID, time.C_String(), intent.C_String());
    g_globalData->databaseLock.Lock();
    g_globalData->database->QueryWithoutResult(sqlCmd);
    g_globalData->databaseLock.Unlock();

    sprintf( sqlCmd, "SELECT ID FROM schedule WHERE time='%s' AND intent='%s'", time.C_String(), intent.C_String());
    g_globalData->databaseLock.Lock();
    g_globalData->database->QueryWithResult( sqlCmd);
    g_globalData->database->FetchRow();
    int ID = g_globalData->database->GetResultInt(0);
    g_globalData->database->FreeResult();
    g_globalData->databaseLock.Unlock();
    map<int, RakNet::SystemAddress>::iterator iter = g_globalData->addressMap.find( toID);
    if( iter != g_globalData->addressMap.end())
    {
      RakNet::BitStream bsOut;
      bsOut.Write( (RakNet::MessageID)RH_SCHEDULE);
      bsOut.Write(SO_NEW);
      bsOut.Write( fromID);
      bsOut.Write( toID);
      bsOut.Write( ID);
      bsOut.Write( time);
      bsOut.Write( intent);
      g_globalData->peer->Send( &bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, iter->second, false);
    }
  }
  g_globalData->peer->DeallocatePacket( pIn);
}