#include "RakNet\BitStream.h"
#include "Global.h"
#include "Events.h"

void WINAPI OnNote( PTP_CALLBACK_INSTANCE pInstance, void* packet)
{
  RakNet::Packet* pIn = (RakNet::Packet*)packet;
  RakNet::BitStream bsIn( pIn->data, pIn->length, false);
  bsIn.IgnoreBytes( sizeof(RakNet::MessageID));

  int ID;
  RakNet::RakString note_intent;
  bsIn.Read( ID);
  bsIn.Read( note_intent);
  
  char sqlCmd[2048];
  sprintf( sqlCmd, "UPDATE note SET note_intent='%s' WHERE Id=%d", note_intent.C_String(), ID);
  g_globalData->databaseLock.Lock();
  g_globalData->database->QueryWithoutResult( sqlCmd);
  g_globalData->databaseLock.Unlock();

  g_globalData->peer->DeallocatePacket( pIn);
}