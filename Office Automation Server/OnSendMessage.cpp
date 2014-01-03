#include "RakNet\BitStream.h"
#include "Events.h"
#include "Global.h"
#include "ctime"


void WINAPI OnSendMessage( PTP_CALLBACK_INSTANCE pInstance, void* packet)
{
	//printf("���뷢��ģ��\n");
	time_t now_time;
	now_time = time(NULL);
	int t = now_time;
	RakNet::Packet* pIn = (RakNet::Packet*)packet;
	RakNet::BitStream bsIn(pIn->data, pIn->length, false);
	int fromUser,toUser;
	RakNet::RakString message;
	bsIn.IgnoreBytes( sizeof(RakNet::MessageID));
	int option;
	bsIn.Read( option);
	if(option == SEND)
	{
		bsIn.Read( fromUser);
		bsIn.Read( toUser);
		bsIn.Read( message);
		//printf("������Ϣ");
		//��������Ϣ
		//�����ˣ�fromUser �� �����ˣ�toUser �� ��Ϣ���ݣ�message
		char sqlCmd[1024];
		sprintf( sqlCmd, "INSERT INTO message (fromuser,touser,message,isread,t) values (%d,%d,'%s',0,%d)", fromUser, toUser, message.C_String(),t);
		//printf(sqlCmd);
		g_globalData->databaseLock.Lock();
		g_globalData->database->QueryWithoutResult(sqlCmd);
		g_globalData->databaseLock.Unlock();
		g_globalData->peer->DeallocatePacket( pIn);
		//�㲥��Ϣ

		map<int, RakNet::SystemAddress>::iterator iter;
		iter = g_globalData->addressMap.find( toUser);
		if( iter != g_globalData->addressMap.end())
		{
			RakNet::BitStream bsOutTest;
			bsOutTest.Write( (RakNet::MessageID)RH_NEWMESSAGE);
			bsOutTest.Write( fromUser);
			bsOutTest.Write( message);
			bsOutTest.Write( t);
			g_globalData->peer->Send( &bsOutTest, HIGH_PRIORITY, RELIABLE_ORDERED, 0, iter->second, false);
			printf("%d-->%d:%s  ���·�\n",fromUser,toUser,message.C_String());
		}
		else{
			printf("%d-->%d:%s  δ����\n",fromUser,toUser,message.C_String());
		}
	}
	else if(option == RECEIVE)
	{
		//���ջظ�
		printf("���ջظ�");
		bsIn.Read( fromUser);
		bsIn.Read( toUser);
		bsIn.Read( t);
		printf("%d-->%d:%d\n",fromUser,toUser,t);
		char sqlCmd[1024];
		sprintf( sqlCmd, "UPDATE message SET isread=1 where fromuser=%d and touser=%d and t=%d", fromUser, toUser,t);
		//printf(sqlCmd);
		g_globalData->databaseLock.Lock();
		g_globalData->database->QueryWithoutResult(sqlCmd);
		g_globalData->databaseLock.Unlock();
		g_globalData->peer->DeallocatePacket( pIn);
	}
}