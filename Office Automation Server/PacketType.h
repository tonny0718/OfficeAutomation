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
  RH_LOGIN = ID_USER_PACKET_ENUM + 1, //登录
  RH_DEPARTMENT_INFORMATION, //由服务器向客户端传送部门信息  RH + SUM(INT) + [ ID(INT) + NAME(STRING)]
  RH_SCHEDULE,
  RH_MATES,//由服务器向客户端传送同事信息
  RH_NOTE,//由客户端发出： RH_NOTE + ID(INT) + NOTE(STRING)
          //由服务器发出： RH_NOTE + NOTE(STRING)  只在登录时发送一次
  RH_SET_DEPARTMENT,//设置部门信息
  RH_SET_HUMAN,//设置人员信息
  RH_SENDMESSAGE,
  RH_NEWMESSAGE,
  RH_SENDMAIL,
};

//RH_LOGIN RESULT
const int LR_SUCCESS = 1;
const int LR_ERROR = 2;//用户名密码错误
const int LR_FAIL = 3;//服务器出错

//RH_SCHEDULE option
const int SO_FIRST = 1;//登录时向客户端传送时间表 + SUM(INT) + [ TIME + INTENT]
const int SO_NEW = 2;//新加入时间表
const int SO_DONE = 3;//时间表过期 + ID

//Set Department option
const int SDO_NEW = 1;//由客户端向服务器发送 + NAME(STRING)
                  //由服务器向客户端发送 + ID(INT) + NAME(STRING) 广播
const int SDO_DELETE = 2;//由客户端向服务器发送 + ID(INT)
                     //由服务器向客户端发送 + ID(INT) 广播
const int SDO_SET_MASTER = 3;//设置组长 + DEPARTMENT_ID(INT) + HUMAN_ID(INT)
                         //由服务器向客户端发送 + DEPARTMENT_ID(INT) + HUMAN_ID(INT) 广播

//Set Human Option
const int SHO_NEW = 1;//由客户端向服务器发送 + NAME(STRING) + DEPARTMENT_ID(INT) + PASSWORD(STRING)
                      //由服务器向客户端发送 + ID(INT) + NAME(STRING) + DEPARTMENT_ID(INT)  广播
const int SHO_DELETE = 2;//由客户端向服务器发送 + ID(INT)
                         //由服务器向客户端发送 + ID(INT)  广播
const int SHO_CHANGE_NO_PASSWORD = 3;//由客户端向服务器发送 + ID(INT) + NAME(STRING) + DEPARTMENT_ID(INT)
                                     //由服务器向客户端发送 + ID(INT) + NAME(STRING) + DEPARTMENT_ID(INT) 广播
const int SHO_CHANGE = 4;//由客户端向服务器发送 + ID(INT) + NAME(STRING) + DEPARTMENT_ID(INT) + PASSWORD(STRING)

//RH_SENDMESSAGE option
const int SEND = 1;
const int RECEIVE = 2;

//RH_SENDMAIL　option
const int MAILCREATE = 1;//起草新公文
const int MAILCHECKED = 2;//审批通过
const int MAILNOTHROW = 3;//审批未通过，返回消息。
const int MAILSEND = 4;//将通过审批的公文下发至对应组内员工
const int DONE = 5;//员工已读

const int PASSISREAD = 6;//通过文件已读
const int NOTPASSISREAD = 7;//未通过文件已读