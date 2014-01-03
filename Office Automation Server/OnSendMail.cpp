#include "RakNet\BitStream.h"
#include "Events.h"
#include "Global.h"
#include <stdio.h>
#include <string>

void WINAPI OnSendMail( PTP_CALLBACK_INSTANCE pInstance, void* packet)
{
	int t;
	int fromUser,toDepartment,isRead,type;
	char sqlCmd[1024];
	RakNet::Packet* pIn = (RakNet::Packet*)packet;
	RakNet::BitStream bsIn(pIn->data, pIn->length, false);
	RakNet::RakString title,content;
	bsIn.IgnoreBytes( sizeof(RakNet::MessageID));
	bsIn.Read( fromUser);
	bsIn.Read( toDepartment);
	bsIn.Read( title);
	bsIn.Read( content);
	bsIn.Read( isRead);
	bsIn.Read( t);
	bsIn.Read( type);
	int option = isRead;
	if(option == MAILCREATE)//��ݹ���
	{
		sprintf( sqlCmd, "INSERT INTO mail (fromuser,todepartment,title,t,content,isread) values (%d,%d,'%s',%d,'%s',1)", 
											fromUser, toDepartment, title.C_String(),t,content.C_String());
		g_globalData->databaseLock.Lock();
		g_globalData->database->QueryWithoutResult(sqlCmd);
		//��ȡ�鳤id
		sprintf( sqlCmd, "select department_master from department where id in (select department from account where id=%d)",fromUser);
		g_globalData->database->QueryWithResult(sqlCmd);
		g_globalData->database->FetchRow();
		int boss = g_globalData->database->GetResultInt(0);
		g_globalData->databaseLock.Unlock();
		
		//������Ϣ
		map<int, RakNet::SystemAddress>::iterator iter;
		iter = g_globalData->addressMap.find( boss);
		if( iter != g_globalData->addressMap.end())
		{
			RakNet::BitStream bsOutTest;
			bsOutTest.Write( (RakNet::MessageID)RH_SENDMAIL);
			bsOutTest.Write( fromUser);
			bsOutTest.Write( toDepartment);
			bsOutTest.Write( title);
			bsOutTest.Write( content);
			bsOutTest.Write( isRead);
			bsOutTest.Write( t);
			bsOutTest.Write( type);
			g_globalData->peer->Send( &bsOutTest, HIGH_PRIORITY, RELIABLE_ORDERED, 0, iter->second, false);
			printf("%d����(1)�·�\n",fromUser);
		}
		else{
			printf("%d����(1)δ����\n",fromUser);
		}
	}
	else if(option == MAILCHECKED)//ͨ������
	{
		sprintf( sqlCmd, "UPDATE mail SET isread=2 where fromuser=%d and t=%d and todepartment=%d and type=%d", fromUser,t,toDepartment,type);
		g_globalData->databaseLock.Lock();
		g_globalData->database->QueryWithoutResult(sqlCmd);
		sprintf( sqlCmd, "SELECT id from account where department=%d",toDepartment);
		int count = g_globalData->database->QueryWithResult(sqlCmd);
		char sqlAdd[1024][100];
		int toID[100];
		for(int n=0;n<count;n++)
		{
			g_globalData->database->FetchRow();
			toID[n] = g_globalData->database->GetResultInt(0);    
			sprintf( sqlAdd[n], "INSERT INTO mail (fromuser,todepartment,title,t,content,isread,type) values (%d,%d,'%s',%d,'%s',4,1)"
													,fromUser,toID[n],title.C_String(),t,content.C_String());
		}
		for(int n=0;n<count;n++)
		{
			g_globalData->database->QueryWithResult(sqlAdd[n]);
			//������Ϣ
			map<int, RakNet::SystemAddress>::iterator iter;
			iter = g_globalData->addressMap.find( toID[n]);
			if( iter != g_globalData->addressMap.end())
			{
				RakNet::BitStream bsOutTest;
				bsOutTest.Write( (RakNet::MessageID)RH_SENDMAIL);//�չ�����ʾ��Ϣ
				bsOutTest.Write( fromUser);
				bsOutTest.Write( toID[n]);
				bsOutTest.Write( title);
				bsOutTest.Write( content);
				bsOutTest.Write( 4);
				bsOutTest.Write( t);
				bsOutTest.Write( type);
				g_globalData->peer->Send( &bsOutTest, HIGH_PRIORITY, RELIABLE_ORDERED, 0, iter->second, false);
				printf("����(2)�·�\n");
			}
			else{
				printf("����(2)δ����\n");
			}
		}
		g_globalData->databaseLock.Unlock();
		g_globalData->peer->DeallocatePacket( pIn);
	}
	else if(option == MAILNOTHROW)//����δͨ��
	{
		sprintf( sqlCmd, "UPDATE mail SET isread=3 where fromuser=%d and t=%d and todepartment=%d and type=%d", fromUser,t,toDepartment,type);
		g_globalData->databaseLock.Lock();
		g_globalData->database->QueryWithoutResult(sqlCmd);
		g_globalData->databaseLock.Unlock();
		g_globalData->peer->DeallocatePacket( pIn);

		//֪ͨ����ˣ�
		map<int, RakNet::SystemAddress>::iterator iter;
		iter = g_globalData->addressMap.find( fromUser);
		if( iter != g_globalData->addressMap.end())
		{
			RakNet::BitStream bsOutTest;
			bsOutTest.Write( (RakNet::MessageID)RH_SENDMAIL);//�չ�����ʾ��Ϣ
			bsOutTest.Write( fromUser);
			bsOutTest.Write( toDepartment);
			bsOutTest.Write( title);
			bsOutTest.Write( content);
			bsOutTest.Write( 3);
			bsOutTest.Write( t);
			bsOutTest.Write( type);
			g_globalData->peer->Send( &bsOutTest, HIGH_PRIORITY, RELIABLE_ORDERED, 0, iter->second, false);
			printf("%d����(3)�·�\n",fromUser);
		}
		else{
			printf("%d����(3)δ����\n",fromUser);
		}

	}
	else if(option == MAILSEND)//��������δ��
	{
		int toUser = toDepartment;
		printf("����(4)\n");
		sprintf( sqlCmd, "UPDATE mail SET isread=4 where fromuser=%d and t=%d and todepartment=%d and type=%d", fromUser,t,toDepartment,type);
		g_globalData->databaseLock.Lock();
		g_globalData->database->QueryWithoutResult(sqlCmd);
		g_globalData->databaseLock.Unlock();
		g_globalData->peer->DeallocatePacket( pIn);
	}
	else if(option == DONE)//�����Ѷ�
	{
		int toUser = toDepartment;
		printf("����(5)\n");
		sprintf( sqlCmd, "UPDATE mail SET isread=5 where fromuser=%d and t=%d and todepartment=%d and type=%d", fromUser,t,toDepartment,type);
		g_globalData->databaseLock.Lock();
		g_globalData->database->QueryWithoutResult(sqlCmd);
		g_globalData->databaseLock.Unlock();
		g_globalData->peer->DeallocatePacket( pIn);
	}
	else if(option == PASSISREAD)//����ͨ�������Ѷ�
	{
		printf("����(6)\n");
		sprintf( sqlCmd, "UPDATE mail SET isread=6 where fromuser=%d and t=%d and todepartment=%d and type=%d", fromUser,t,toDepartment,type);
		g_globalData->databaseLock.Lock();
		g_globalData->database->QueryWithoutResult(sqlCmd);
		g_globalData->databaseLock.Unlock();
		g_globalData->peer->DeallocatePacket( pIn);
	}
	else if(option == NOTPASSISREAD)//δͨ�����������Ѷ�
	{
		printf("����(7)\n");
		sprintf( sqlCmd, "UPDATE mail SET isread=7 where fromuser=%d and t=%d and todepartment=%d and type=%d", fromUser,t,toDepartment,type);
		g_globalData->databaseLock.Lock();
		g_globalData->database->QueryWithoutResult(sqlCmd);
		g_globalData->databaseLock.Unlock();
		g_globalData->peer->DeallocatePacket( pIn);
	}
	
}