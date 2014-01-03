#include "RakNet\BitStream.h"
#include "Events.h"
#include "Global.h"

void WINAPI OnSetDepartment(PTP_CALLBACK_INSTANCE pInstance, void* packet)
{
  RakNet::Packet* pIn = (RakNet::Packet*)packet;
  RakNet::BitStream bsIn( pIn->data, pIn->length, false);
  bsIn.IgnoreBytes( sizeof(RakNet::MessageID));
  int setDepartmentOption;
  bsIn.Read( setDepartmentOption);

  if( setDepartmentOption == SDO_NEW)
  {
    RakNet::RakString name;
    bsIn.Read( name);
    char sqlCmd[128];
    sprintf( sqlCmd, "INSERT INTO department SET department_name='%s'", name.C_String());
    g_globalData->databaseLock.Lock();
    g_globalData->database->QueryWithoutResult(sqlCmd);
    g_globalData->databaseLock.Unlock();
    
    g_globalData->databaseLock.Lock();
    sprintf( sqlCmd, "SELECT id FROM department WHERE department_name='%s'", name.C_String());
    g_globalData->database->QueryWithResult(sqlCmd);
    g_globalData->database->FetchRow();
    int departmentID = g_globalData->database->GetResultInt(0);
    g_globalData->database->FreeResult();
    g_globalData->databaseLock.Unlock();

    RakNet::BitStream bsOut;
    bsOut.Write( (RakNet::MessageID)RH_SET_DEPARTMENT);
    bsOut.Write( SDO_NEW);
    bsOut.Write( departmentID);
    bsOut.Write( name);
    g_globalData->peer->Send( &bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, pIn->systemAddress, false);
    g_globalData->peer->Send( &bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, pIn->systemAddress, true);
  }
  else if( setDepartmentOption == SDO_DELETE)
  {
    int ID;
    bsIn.Read(ID);
    char sqlCmd[128];
    sprintf( sqlCmd, "DELETE FROM account WHERE department=%d", ID);
    g_globalData->databaseLock.Lock();
    g_globalData->database->QueryWithoutResult( sqlCmd);
    g_globalData->databaseLock.Unlock();

    sprintf( sqlCmd, "DELETE FROM department WHERE id=%d", ID);
    g_globalData->databaseLock.Lock();
    g_globalData->database->QueryWithoutResult( sqlCmd);
    g_globalData->databaseLock.Unlock();

    RakNet::BitStream bsOut;
    bsOut.Write( (RakNet::MessageID)RH_SET_DEPARTMENT);
    bsOut.Write( SDO_DELETE);
    bsOut.Write( ID);
    g_globalData->peer->Send( &bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, pIn->systemAddress, false);
    g_globalData->peer->Send( &bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, pIn->systemAddress, true);
  }
  else if( setDepartmentOption == SDO_SET_MASTER)
  {
    int departmentID, humanID;
    char sqlCmd[128];
    bsIn.Read( departmentID);
    bsIn.Read( humanID);
    sprintf( sqlCmd, "UPDATE account SET priority=1 WHERE department=%d AND priority=2", departmentID);
    g_globalData->databaseLock.Lock();
    g_globalData->database->QueryWithoutResult( sqlCmd);
    g_globalData->databaseLock.Unlock();
    sprintf( sqlCmd, "UPDATE account SET priority=2 WHERE Id=%d AND priority=1", humanID);
    g_globalData->databaseLock.Lock();
    g_globalData->database->QueryWithoutResult( sqlCmd);
    g_globalData->databaseLock.Unlock();
    sprintf( sqlCmd, "UPDATE department SET department_master=%d WHERE Id=%d", humanID, departmentID);
    g_globalData->databaseLock.Lock();
    g_globalData->database->QueryWithoutResult( sqlCmd);
    g_globalData->databaseLock.Unlock();

    RakNet::BitStream bsOut;
    bsOut.Write( (RakNet::MessageID)RH_SET_DEPARTMENT);
    bsOut.Write( SDO_SET_MASTER);
    bsOut.Write( departmentID);
    bsOut.Write( humanID);
    g_globalData->peer->Send( &bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, pIn->systemAddress, false);
    g_globalData->peer->Send( &bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, pIn->systemAddress, true);
  }

  g_globalData->peer->DeallocatePacket( pIn);
}