#include "RakNet\BitStream.h"
#include "Events.h"
#include "Global.h"

void WINAPI OnSetHuman( PTP_CALLBACK_INSTANCE pInstance, void* packet)
{
  RakNet::Packet* pIn = (RakNet::Packet*)packet;
  RakNet::BitStream bsIn( pIn->data, pIn->length, false);
  bsIn.IgnoreBytes( sizeof(RakNet::MessageID));
  int option;
  bsIn.Read( option);

  if( option == SHO_NEW)
  {
    RakNet::RakString name, password;
    int departmentID;
    bsIn.Read( name);
    bsIn.Read( departmentID);
    bsIn.Read( password);
    char sqlCmd[256];
    sprintf( sqlCmd, "INSERT INTO account SET name='%s', password='%s', department=%d", name.C_String(), password.C_String(), departmentID);
    g_globalData->databaseLock.Lock();
    g_globalData->database->QueryWithoutResult(sqlCmd);
    g_globalData->databaseLock.Unlock();

    sprintf( sqlCmd, "SELECT Id FROM account WHERE name='%s' AND department=%d", name.C_String(), departmentID);
    g_globalData->databaseLock.Lock();
    g_globalData->database->QueryWithResult(sqlCmd);
    g_globalData->database->FetchRow();
    int ID = g_globalData->database->GetResultInt(0);
    g_globalData->database->FreeResult();
    g_globalData->databaseLock.Unlock();

    sprintf( sqlCmd, "INSERT INTO note SET Id=%d,note_intent=' '", ID);
    g_globalData->databaseLock.Lock();
    g_globalData->database->QueryWithoutResult(sqlCmd);
    g_globalData->databaseLock.Unlock();

    RakNet::BitStream bsOut;
    bsOut.Write( (RakNet::MessageID)RH_SET_HUMAN);
    bsOut.Write( SDO_NEW);
    bsOut.Write( ID);
    bsOut.Write( name);
    bsOut.Write( departmentID);
    g_globalData->peer->Send( &bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, pIn->systemAddress, false);
    g_globalData->peer->Send( &bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, pIn->systemAddress, true);
  }
  else if( option == SHO_DELETE)
  {
    int ID;
    bsIn.Read(ID);
    char sqlCmd[128];
    sprintf( sqlCmd, "DELETE FROM account WHERE Id=%d", ID);
    g_globalData->databaseLock.Lock();
    g_globalData->database->QueryWithoutResult(sqlCmd);
    g_globalData->databaseLock.Unlock();

    RakNet::BitStream bsOut;
    bsOut.Write( (RakNet::MessageID)RH_SET_HUMAN);
    bsOut.Write( SHO_DELETE);
    bsOut.Write( ID);
    g_globalData->peer->Send( &bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, pIn->systemAddress, false);
    g_globalData->peer->Send( &bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, pIn->systemAddress, true);
  }
  else if( option == SHO_CHANGE_NO_PASSWORD)
  {
    int ID,departmentID;
    RakNet::RakString name;
    bsIn.Read( ID);
    bsIn.Read( name);
    bsIn.Read( departmentID);
    char sqlCmd[256];
    sprintf( sqlCmd, "UPDATE account SET name='%s', department=%d WHERE Id=%d", name.C_String(), departmentID, ID);
    g_globalData->databaseLock.Lock();
    g_globalData->database->QueryWithoutResult( sqlCmd);
    g_globalData->databaseLock.Unlock();

    RakNet::BitStream bsOut;
    bsOut.Write( (RakNet::MessageID)RH_SET_HUMAN);
    bsOut.Write( SHO_CHANGE_NO_PASSWORD);
    bsOut.Write( ID);
    bsOut.Write( name);
    bsOut.Write( departmentID);
    g_globalData->peer->Send( &bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, pIn->systemAddress, false);
    g_globalData->peer->Send( &bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, pIn->systemAddress, true);
  }
  else if( option == SHO_CHANGE)
  {
    int ID,departmentID;
    RakNet::RakString name, password;
    bsIn.Read( ID);
    bsIn.Read( name);
    bsIn.Read( departmentID);
    bsIn.Read( password);
    char sqlCmd[256];
    sprintf( sqlCmd, "UPDATE account SET name='%s',password='%s',department=%d WHERE Id=%d", name.C_String(), password.C_String(), departmentID,ID);
    g_globalData->databaseLock.Lock();
    g_globalData->database->QueryWithoutResult(sqlCmd);
    g_globalData->databaseLock.Unlock();

    RakNet::BitStream bsOut;
    bsOut.Write( (RakNet::MessageID)RH_SET_HUMAN);
    bsOut.Write( SHO_CHANGE_NO_PASSWORD);
    bsOut.Write( ID);
    bsOut.Write( name);
    bsOut.Write( departmentID);
    g_globalData->peer->Send( &bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, pIn->systemAddress, false);
    g_globalData->peer->Send( &bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, pIn->systemAddress, true);
  }

  g_globalData->peer->DeallocatePacket( pIn);
}