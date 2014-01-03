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
	if(option == MAILCREATE)//起草公文
	{
		sprintf( sqlCmd, "INSERT INTO mail (fromuser,todepartment,title,t,content,isread) values (%d,%d,'%s',%d,'%s',1)", 
											fromUser, toDepartment, title.C_String(),t,content.C_String());
		g_globalData->databaseLock.Lock();
		g_globalData->database->QueryWithoutResult(sqlCmd);
		//获取组长id
		sprintf( sqlCmd, "select department_master from department where id in (select department from account where id=%d)",fromUser);
		g_globalData->database->QueryWithResult(sqlCmd);
		g_globalData->database->FetchRow();
		int boss = g_globalData->database->GetResultInt(0);
		g_globalData->databaseLock.Unlock();
		
		//推送消息
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
			printf("%d公文(1)下发\n",fromUser);
		}
		else{
			printf("%d公文(1)未发送\n",fromUser);
		}
	}
	else if(option == MAILCHECKED)//通过审批
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
			//推送消息
			map<int, RakNet::SystemAddress>::iterator iter;
			iter = g_globalData->addressMap.find( toID[n]);
			if( iter != g_globalData->addressMap.end())
			{
				RakNet::BitStream bsOutTest;
				bsOutTest.Write( (RakNet::MessageID)RH_SENDMAIL);//收公文提示消息
				bsOutTest.Write( fromUser);
				bsOutTest.Write( toID[n]);
				bsOutTest.Write( title);
				bsOutTest.Write( content);
				bsOutTest.Write( 4);
				bsOutTest.Write( t);
				bsOutTest.Write( type);
				g_globalData->peer->Send( &bsOutTest, HIGH_PRIORITY, RELIABLE_ORDERED, 0, iter->second, false);
				printf("公文(2)下发\n");
			}
			else{
				printf("公文(2)未发送\n");
			}
		}
		g_globalData->databaseLock.Unlock();
		g_globalData->peer->DeallocatePacket( pIn);
	}
	else if(option == MAILNOTHROW)//审批未通过
	{
		sprintf( sqlCmd, "UPDATE mail SET isread=3 where fromuser=%d and t=%d and todepartment=%d and type=%d", fromUser,t,toDepartment,type);
		g_globalData->databaseLock.Lock();
		g_globalData->database->QueryWithoutResult(sqlCmd);
		g_globalData->databaseLock.Unlock();
		g_globalData->peer->DeallocatePacket( pIn);

		//通知起草人：
		map<int, RakNet::SystemAddress>::iterator iter;
		iter = g_globalData->addressMap.find( fromUser);
		if( iter != g_globalData->addressMap.end())
		{
			RakNet::BitStream bsOutTest;
			bsOutTest.Write( (RakNet::MessageID)RH_SENDMAIL);//收公文提示消息
			bsOutTest.Write( fromUser);
			bsOutTest.Write( toDepartment);
			bsOutTest.Write( title);
			bsOutTest.Write( content);
			bsOutTest.Write( 3);
			bsOutTest.Write( t);
			bsOutTest.Write( type);
			g_globalData->peer->Send( &bsOutTest, HIGH_PRIORITY, RELIABLE_ORDERED, 0, iter->second, false);
			printf("%d公文(3)下发\n",fromUser);
		}
		else{
			printf("%d公文(3)未发送\n",fromUser);
		}

	}
	else if(option == MAILSEND)//公文设置未读
	{
		int toUser = toDepartment;
		printf("公文(4)\n");
		sprintf( sqlCmd, "UPDATE mail SET isread=4 where fromuser=%d and t=%d and todepartment=%d and type=%d", fromUser,t,toDepartment,type);
		g_globalData->databaseLock.Lock();
		g_globalData->database->QueryWithoutResult(sqlCmd);
		g_globalData->databaseLock.Unlock();
		g_globalData->peer->DeallocatePacket( pIn);
	}
	else if(option == DONE)//公文已读
	{
		int toUser = toDepartment;
		printf("公文(5)\n");
		sprintf( sqlCmd, "UPDATE mail SET isread=5 where fromuser=%d and t=%d and todepartment=%d and type=%d", fromUser,t,toDepartment,type);
		g_globalData->databaseLock.Lock();
		g_globalData->database->QueryWithoutResult(sqlCmd);
		g_globalData->databaseLock.Unlock();
		g_globalData->peer->DeallocatePacket( pIn);
	}
	else if(option == PASSISREAD)//审批通过公文已读
	{
		printf("公文(6)\n");
		sprintf( sqlCmd, "UPDATE mail SET isread=6 where fromuser=%d and t=%d and todepartment=%d and type=%d", fromUser,t,toDepartment,type);
		g_globalData->databaseLock.Lock();
		g_globalData->database->QueryWithoutResult(sqlCmd);
		g_globalData->databaseLock.Unlock();
		g_globalData->peer->DeallocatePacket( pIn);
	}
	else if(option == NOTPASSISREAD)//未通过审批公文已读
	{
		printf("公文(7)\n");
		sprintf( sqlCmd, "UPDATE mail SET isread=7 where fromuser=%d and t=%d and todepartment=%d and type=%d", fromUser,t,toDepartment,type);
		g_globalData->databaseLock.Lock();
		g_globalData->database->QueryWithoutResult(sqlCmd);
		g_globalData->databaseLock.Unlock();
		g_globalData->peer->DeallocatePacket( pIn);
	}
	
}