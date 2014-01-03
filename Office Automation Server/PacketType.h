#pragma once

#include <string>
#include "RakNet\MessageIdentifiers.h"
#include "RakNet\RakNetTypes.h"
using std::string;

//const string serverIP = "115.28.59.149";
const string serverIP = "127.0.0.1";
const int serverPort = 35000;

enum RakNetHead
{
  RH_LOGIN = ID_USER_PACKET_ENUM + 1, //��¼
  RH_DEPARTMENT_INFORMATION, //�ɷ�������ͻ��˴��Ͳ�����Ϣ  RH + SUM(INT) + [ ID(INT) + NAME(STRING)]
  RH_SCHEDULE,
  RH_MATES,//�ɷ�������ͻ��˴���ͬ����Ϣ
  RH_NOTE,//�ɿͻ��˷����� RH_NOTE + ID(INT) + NOTE(STRING)
          //�ɷ����������� RH_NOTE + NOTE(STRING)  ֻ�ڵ�¼ʱ����һ��
  RH_SET_DEPARTMENT,//���ò�����Ϣ
  RH_SET_HUMAN,//������Ա��Ϣ
  RH_SENDMESSAGE,
  RH_NEWMESSAGE,
  RH_SENDMAIL,
};

//RH_LOGIN RESULT
const int LR_SUCCESS = 1;
const int LR_ERROR = 2;//�û����������
const int LR_FAIL = 3;//����������

//RH_SCHEDULE option
const int SO_FIRST = 1;//��¼ʱ��ͻ��˴���ʱ��� + SUM(INT) + [ TIME + INTENT]
const int SO_NEW = 2;//�¼���ʱ���
const int SO_DONE = 3;//ʱ������ + ID

//Set Department option
const int SDO_NEW = 1;//�ɿͻ�������������� + NAME(STRING)
                  //�ɷ�������ͻ��˷��� + ID(INT) + NAME(STRING) �㲥
const int SDO_DELETE = 2;//�ɿͻ�������������� + ID(INT)
                     //�ɷ�������ͻ��˷��� + ID(INT) �㲥
const int SDO_SET_MASTER = 3;//�����鳤 + DEPARTMENT_ID(INT) + HUMAN_ID(INT)
                         //�ɷ�������ͻ��˷��� + DEPARTMENT_ID(INT) + HUMAN_ID(INT) �㲥

//Set Human Option
const int SHO_NEW = 1;//�ɿͻ�������������� + NAME(STRING) + DEPARTMENT_ID(INT) + PASSWORD(STRING)
                      //�ɷ�������ͻ��˷��� + ID(INT) + NAME(STRING) + DEPARTMENT_ID(INT)  �㲥
const int SHO_DELETE = 2;//�ɿͻ�������������� + ID(INT)
                         //�ɷ�������ͻ��˷��� + ID(INT)  �㲥
const int SHO_CHANGE_NO_PASSWORD = 3;//�ɿͻ�������������� + ID(INT) + NAME(STRING) + DEPARTMENT_ID(INT)
                                     //�ɷ�������ͻ��˷��� + ID(INT) + NAME(STRING) + DEPARTMENT_ID(INT) �㲥
const int SHO_CHANGE = 4;//�ɿͻ�������������� + ID(INT) + NAME(STRING) + DEPARTMENT_ID(INT) + PASSWORD(STRING)

//RH_SENDMESSAGE option
const int SEND = 1;
const int RECEIVE = 2;

//RH_SENDMAIL��option
const int MAILCREATE = 1;//����¹���
const int MAILCHECKED = 2;//����ͨ��
const int MAILNOTHROW = 3;//����δͨ����������Ϣ��
const int MAILSEND = 4;//��ͨ�������Ĺ����·�����Ӧ����Ա��
const int DONE = 5;//Ա���Ѷ�

const int PASSISREAD = 6;//ͨ���ļ��Ѷ�
const int NOTPASSISREAD = 7;//δͨ���ļ��Ѷ�