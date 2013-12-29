#include "RakNet\BitStream.h"
#include "Events.h"
#include "Global.h"


void WINAPI OnSendMessage( PTP_CALLBACK_INSTANCE pInstance, void* packet)
{
	printf("进入发送模块\n");
  RakNet::Packet* pIn = (RakNet::Packet*)packet;
  RakNet::BitStream bsIn(pIn->data, pIn->length, false);
  int fromUser,toUserNumber,toUser;
  RakNet::RakString message;
  bsIn.IgnoreBytes( sizeof(RakNet::MessageID));
  bsIn.Read( fromUser);//from。
  bsIn.Read( message);//接收人个数。
  bsIn.Read( toUserNumber);//接收人个数。
  for(int n=0;n<toUserNumber;n++)
  {
	bsIn.Read( toUser);
	//处理发送消息
	//发送人：fromUser ， 接收人：toUser ， 消息内容：message
	printf("%d-->%d:%s\n",fromUser,toUser,message.C_String());
  }
  RakNet::BitStream bsOut;
  bsOut.Write( (RakNet::MessageID)RH_SENDMESSAGE);
  if( FALSE)
  {
    bsOut.Write( LR_ERROR);
    printf("发送失败\n");
  }
  else
  {
    bsOut.Write( LR_SUCCESS);
    printf("发送成功.\n");
  }
  g_globalData->peer->Send( &bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, pIn->systemAddress, false);
  g_globalData->peer->DeallocatePacket( pIn);
}

void WINAPI OnGetNameList( PTP_CALLBACK_INSTANCE pInstance, void* packet)
{
  RakNet::Packet* pIn = (RakNet::Packet*)packet;
  RakNet::BitStream bsIn(pIn->data, pIn->length, false);
  int usermark;
  bsIn.IgnoreBytes( sizeof(RakNet::MessageID));
  bsIn.Read( usermark);//权限字段，验证略。

  char sqlCmd[1024];
  sprintf( sqlCmd, "SELECT id,name,department FROM account");
  g_globalData->databaseLock.Lock();
  int count = g_globalData->database->QueryWithResult(sqlCmd);
  RakNet::BitStream bsOut;
  bsOut.Write( (RakNet::MessageID)RH_GETNAMELIST);
  if( count == 0)
  {
    bsOut.Write( LR_ERROR);
    printf("%d拉取列表失败,没有内容\n", usermark);
  }
  else
  {
    bsOut.Write( LR_SUCCESS);
	bsOut.Write( count);
	for(int n=0;n<count;n++)
	{
		g_globalData->database->FetchRow();
		bsOut.Write(g_globalData->database->GetResultInt(0));
		RakNet::RakString name = g_globalData->database->GetResult(1);
		bsOut.Write(name);
		bsOut.Write(g_globalData->database->GetResultInt(2));
	}
	sprintf( sqlCmd, "SELECT Id,department_name,department_master FROM department");
	count = g_globalData->database->QueryWithResult(sqlCmd);
	bsOut.Write( count);
	for(int n=0;n<count;n++)
	{
		g_globalData->database->FetchRow();
		bsOut.Write(g_globalData->database->GetResultInt(0));
		RakNet::RakString name = g_globalData->database->GetResult(1);
		bsOut.Write(name);
		bsOut.Write(g_globalData->database->GetResultInt(2));
	}


    printf("%d获取列表.\n", usermark);
    //g_globalData->addressMap[ID] = pIn->systemAddress;
  }
  g_globalData->database->FreeResult();
  g_globalData->databaseLock.Unlock();
  g_globalData->peer->Send( &bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, pIn->systemAddress, false);
  g_globalData->peer->DeallocatePacket( pIn);

}