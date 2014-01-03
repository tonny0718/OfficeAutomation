
// Office Automation Client.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#include "stdafx.h"
#include <map>
#include <string>
#include <set>
using std::set;
using std::map;
using std::string;
using std::pair;

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号

#include "ScheduleDlg.h"
#include "SystemDlg.h"

// COfficeAutomationClientApp:
// 有关此类的实现，请参阅 Office Automation Client.cpp
//

class COfficeAutomationClientApp : public CWinApp
{
public:
	COfficeAutomationClientApp();

// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()

public:
  RakNet::RakPeerInterface* m_peer;
  RakNet::SystemAddress m_serverAddress;
  int m_ID;
  int m_department;
  int m_priority;

  map<int, string> m_departmentName;
  map<int, int> m_departmentMaster;
  map<string, pair<string,int> > m_schedule;
  map<int, pair<string, int> > m_mates;
  string m_note;

  struct mail{
	  int fromUser;
	  int toDepartment;
	  RakNet::RakString title;
	  RakNet::RakString content;
	  int isRead;
	  int t;
	  int type;
  }mailList[100],tmpmail;
  int mailCount;
  set<int> mailaccepteddepartments;


  ScheduleDlg m_scheduleDlg;
  CSystemDlg m_systemDlg;

  static unsigned WINAPI NetRecieveThread( void* pOfficeAutomationClientApp);
  void Exit();
};

extern COfficeAutomationClientApp theApp;