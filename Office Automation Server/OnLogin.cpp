#include "RakNet\BitStream.h"
#include "Events.h"
#include "Global.h"

void WINAPI OnLogin( PTP_CALLBACK_INSTANCE pInstance, void* packet)
{
  RakNet::Packet* pIn = (RakNet::Packet*)packet;
  RakNet::BitStream bsIn(pIn->data, pIn->length, false);
  RakNet::RakString username, password;
  bsIn.IgnoreBytes( sizeof(RakNet::MessageID));
  bsIn.Read( username);
  bsIn.Read( password);
  int ID;
  char sqlCmd[1024];
  sprintf( sqlCmd, "SELECT id,department,priority FROM account WHERE name='%s' AND password='%s'", username.C_String(), password.C_String());
  g_globalData->databaseLock.Lock();
  int count = g_globalData->database->QueryWithResult(sqlCmd);
  RakNet::BitStream bsOut;
  bsOut.Write( (RakNet::MessageID)RH_LOGIN);
  if( count == 0)
  {
    bsOut.Write( LR_ERROR);
    printf("%s登录失败\n", username.C_String());
  }
  else
  {
    g_globalData->database->FetchRow();
    bsOut.Write( LR_SUCCESS);
    ID = g_globalData->database->GetResultInt(0);
    bsOut.Write( ID);
    bsOut.Write( g_globalData->database->GetResultInt(1));//department
    bsOut.Write( g_globalData->database->GetResultInt(2));//priority
    printf("%s登录成功.\n", username.C_String());
    g_globalData->addressMap[ID] = pIn->systemAddress;
  }
  g_globalData->database->FreeResult();
  g_globalData->databaseLock.Unlock();
  g_globalData->peer->Send( &bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, pIn->systemAddress, false);
  g_globalData->peer->DeallocatePacket( pIn);

  if( count != 0)
  {
    //写入上班时间
    g_globalData->databaseLock.Lock();
    sprintf( sqlCmd, "INSERT INTO attendance SET user=%d,type=1", ID);
    g_globalData->database->QueryWithoutResult( sqlCmd);
    g_globalData->databaseLock.Unlock();

    //发送同事信息
    sprintf( sqlCmd, "SELECT ID,name,department FROM account");
    g_globalData->databaseLock.Lock();
    count = g_globalData->database->QueryWithResult( sqlCmd);
    RakNet::BitStream mates;
    mates.Write( (RakNet::MessageID)RH_MATES);
    mates.Write( count);
    for( int i=0; i<count; i++)
    {
      g_globalData->database->FetchRow();
      mates.Write( g_globalData->database->GetResultInt(0));//ID
      RakNet::RakString name(g_globalData->database->GetResult(1));
      mates.Write( name);
      mates.Write( g_globalData->database->GetResultInt(2));//department
    }
    g_globalData->peer->Send( &mates, HIGH_PRIORITY, RELIABLE_ORDERED, 0, pIn->systemAddress, false);
    g_globalData->database->FreeResult();
    g_globalData->databaseLock.Unlock();

    //发送各部门名称
    sprintf( sqlCmd, "SELECT ID,department_name,department_master FROM department");
    g_globalData->databaseLock.Lock();
    count = g_globalData->database->QueryWithResult( sqlCmd);
    RakNet::BitStream departmentInformation;
    departmentInformation.Write( (RakNet::MessageID)RH_DEPARTMENT_INFORMATION);
    departmentInformation.Write( count);
    for( int i=0; i<count; i++)
    {
      g_globalData->database->FetchRow();
      departmentInformation.Write( g_globalData->database->GetResultInt(0));//department_id
      RakNet::RakString departmentName( g_globalData->database->GetResult(1));
      departmentInformation.Write( departmentName);
      departmentInformation.Write( g_globalData->database->GetResultInt(2));
    }
    g_globalData->peer->Send( &departmentInformation, HIGH_PRIORITY, RELIABLE_ORDERED, 0, pIn->systemAddress, false);
    g_globalData->database->FreeResult();
    g_globalData->databaseLock.Unlock();

    //发送时间安排
    sprintf( sqlCmd, "SELECT time,intent,ID FROM schedule WHERE user_id=%d AND isDone=0", ID);
    g_globalData->databaseLock.Lock();
    count = g_globalData->database->QueryWithResult( sqlCmd);
    RakNet::BitStream schedule;
    schedule.Write( (RakNet::MessageID)RH_SCHEDULE);
    schedule.Write( SO_FIRST);
    schedule.Write( count);
    for( int i=0; i<count; i++)
    {
      g_globalData->database->FetchRow();
      schedule.Write( g_globalData->database->GetResultInt(2));//ID
      RakNet::RakString time( g_globalData->database->GetResult(0));
      RakNet::RakString intent( g_globalData->database->GetResult(1));
      schedule.Write( time);
      schedule.Write( intent);
    }
    g_globalData->peer->Send( &schedule, HIGH_PRIORITY, RELIABLE_ORDERED, 0, pIn->systemAddress, false);
    g_globalData->database->FreeResult();
    g_globalData->databaseLock.Unlock();

    

    //发送备忘录信息
    sprintf( sqlCmd, "SELECT note_intent FROM note WHERE Id=%d", ID);
    g_globalData->databaseLock.Lock();
    g_globalData->database->QueryWithResult( sqlCmd);
    RakNet::BitStream note;
    note.Write( (RakNet::MessageID)RH_NOTE);
    g_globalData->database->FetchRow();
    RakNet::RakString noteIntent( g_globalData->database->GetResult(0));
    note.Write( noteIntent);
    g_globalData->peer->Send( &note, HIGH_PRIORITY, RELIABLE_ORDERED, 0, pIn->systemAddress, false);
    g_globalData->database->FreeResult();
    g_globalData->databaseLock.Unlock();

    //初始化消息

	  char sqlCmd[1024];
	  sprintf( sqlCmd, "SELECT fromuser,message,t from message where isread=0 and touser=%d ",ID);
	  g_globalData->databaseLock.Lock();
	  int messageNum = g_globalData->database->QueryWithResult(sqlCmd);
	  if(messageNum == 0)
	  {
		  RakNet::BitStream bsOutTest;
		  bsOutTest.Write( (RakNet::MessageID)RH_NEWMESSAGE);
		  int from = 255;
		  RakNet::RakString ACCmessage = "登录成功";
		  bsOutTest.Write( from);
		  bsOutTest.Write( ACCmessage);
		  g_globalData->peer->Send( &bsOutTest, HIGH_PRIORITY, RELIABLE_ORDERED, 0, pIn->systemAddress, false);
	  }
	  else
	  {
		  for(int n=0;n<messageNum;n++)
		  {
			  g_globalData->database->FetchRow();
			  RakNet::BitStream bsOutTest;
			  bsOutTest.Write( (RakNet::MessageID)RH_NEWMESSAGE);
			  RakNet::RakString ACCmessage = g_globalData->database->GetResult(1);
			  bsOutTest.Write( g_globalData->database->GetResultInt(0));
			  bsOutTest.Write( ACCmessage);
			  bsOutTest.Write( g_globalData->database->GetResultInt(2));
			  g_globalData->peer->Send( &bsOutTest, HIGH_PRIORITY, RELIABLE_ORDERED, 0, pIn->systemAddress, false);
		  }
	  }
	  g_globalData->databaseLock.Unlock();




	  //初始化公文
	  RakNet::RakString title,content;
	  int mailNum;
	  char sqlMail[1024];
	  //下发未读，已读公文。
	  sprintf( sqlMail, "SELECT fromuser,todepartment,title,t,content,isread,type from mail where type=1 and todepartment=%d ",ID);
	  g_globalData->databaseLock.Lock();
	  mailNum = g_globalData->database->QueryWithResult(sqlMail);
	  if(mailNum != 0)
	  {
		  for(int n=0;n<mailNum;n++)
		  {
			  g_globalData->database->FetchRow();
			  RakNet::BitStream bsOutTest;
			  bsOutTest.Write( (RakNet::MessageID)RH_SENDMAIL);
			  bsOutTest.Write( g_globalData->database->GetResultInt(0));//fromuser
			  bsOutTest.Write( g_globalData->database->GetResultInt(1));//todepartment
			  title = g_globalData->database->GetResult(2);             //title
			  bsOutTest.Write( title);
			  content = g_globalData->database->GetResult(4);           //content
			  bsOutTest.Write( content);
			  bsOutTest.Write( g_globalData->database->GetResultInt(5));//isread
			  bsOutTest.Write( g_globalData->database->GetResultInt(3));//t
			  bsOutTest.Write( g_globalData->database->GetResultInt(6));//type
			  g_globalData->peer->Send( &bsOutTest, HIGH_PRIORITY, RELIABLE_ORDERED, 0, pIn->systemAddress, false);
		  }
	  }
	  g_globalData->databaseLock.Unlock();


	  //收取发送公文
	  sprintf( sqlMail, "SELECT fromuser,todepartment,title,t,content,isread,type from mail where type=0 and fromuser=%d ",ID);
	  g_globalData->databaseLock.Lock();
	  mailNum = g_globalData->database->QueryWithResult(sqlMail);
	  if(mailNum != 0)
	  {
		  for(int n=0;n<mailNum;n++)
		  {
			  g_globalData->database->FetchRow();
			  RakNet::BitStream bsOutTest;
			  bsOutTest.Write( (RakNet::MessageID)RH_SENDMAIL);
			  bsOutTest.Write( g_globalData->database->GetResultInt(0));//fromuser
			  bsOutTest.Write( g_globalData->database->GetResultInt(1));//todepartment
			  title = g_globalData->database->GetResult(2);             //title
			  bsOutTest.Write( title);
			  content = g_globalData->database->GetResult(4);           //content
			  bsOutTest.Write( content);
			  bsOutTest.Write( g_globalData->database->GetResultInt(5));//isread
			  bsOutTest.Write( g_globalData->database->GetResultInt(3));//t
			  bsOutTest.Write( g_globalData->database->GetResultInt(6));//type
			  g_globalData->peer->Send( &bsOutTest, HIGH_PRIORITY, RELIABLE_ORDERED, 0, pIn->systemAddress, false);
		  }
	  }
	  g_globalData->databaseLock.Unlock();

	  //下发待审批公文
	  sprintf( sqlMail, "SELECT id from department where department_master=%d ",ID);
	  g_globalData->databaseLock.Lock();
	  int isBoss = g_globalData->database->QueryWithResult(sqlMail);
	  if(isBoss != 0)
	  {
		  //printf("isBoss");
		  g_globalData->database->FetchRow();
		  int bossDepartmentId = g_globalData->database->GetResultInt(0);
		  //给boss发审批
		  sprintf( sqlMail, "SELECT fromuser,todepartment,title,t,content,isread,type from mail,account where fromuser=id and type=0 and department=%d ",bossDepartmentId);
		  int mailNum = g_globalData->database->QueryWithResult(sqlMail);
		  if(mailNum != 0)
		  {
			  for(int n=0;n<mailNum;n++)
			  {
				  g_globalData->database->FetchRow();
				  RakNet::BitStream bsOutTest;
				  bsOutTest.Write( (RakNet::MessageID)RH_SENDMAIL);
				  bsOutTest.Write( g_globalData->database->GetResultInt(0));//fromuser
				  bsOutTest.Write( g_globalData->database->GetResultInt(1));//todepartment
				  title = g_globalData->database->GetResult(2);             //title
				  bsOutTest.Write( title);
				  content = g_globalData->database->GetResult(4);           //content
				  bsOutTest.Write( content);
				  bsOutTest.Write( g_globalData->database->GetResultInt(5));//isread
				  bsOutTest.Write( g_globalData->database->GetResultInt(3));//t
				  bsOutTest.Write( g_globalData->database->GetResultInt(6));//type
				  g_globalData->peer->Send( &bsOutTest, HIGH_PRIORITY, RELIABLE_ORDERED, 0, pIn->systemAddress, false);
			  }
		  }
	  }
	  g_globalData->databaseLock.Unlock();
  }
}