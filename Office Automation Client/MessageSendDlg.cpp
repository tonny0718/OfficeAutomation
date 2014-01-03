// MessageSendDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Office Automation Client.h"
#include "MessageSendDlg.h"
#include "afxdialogex.h"
#include "PacketType.h"

// CMessageSendDlg 对话框

IMPLEMENT_DYNAMIC(CMessageSendDlg, CDialogEx)

CMessageSendDlg::CMessageSendDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMessageSendDlg::IDD, pParent)
	, m_edit(_T(""))
{

}

CMessageSendDlg::~CMessageSendDlg()
{
}

void CMessageSendDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_TREE1, m_tree);
	DDX_Text(pDX, IDC_SENDMESSAGE_EDTCONTENT, m_edit);
	//  DDX_Control(pDX, IDC_SENDMESSAGE_LSTMATES, m_lstMates);
	//  DDX_Control(pDX, IDC_SENDMESSAGE_LSTADDTO, m_lstAddTo);
	//  DDX_Control(pDX, IDC_SENDMESSAGE_LSTADDTO, m_lstdep);
	//  DDX_Control(pDX, IDC_SENDMESSAGE_LSTMATES, m_lstAddTo);
	//  DDX_Control(pDX, IDC_SENDMESSAGE_LSTADDTO, m_lstMats);
	DDX_Control(pDX, IDC_SENDMESSAGE_LSTMATES, m_lstAddTo);
	DDX_Control(pDX, IDC_SENDMESSAGE_LSTADDTO, m_lstMates);
}


BEGIN_MESSAGE_MAP(CMessageSendDlg, CDialogEx)
	ON_BN_CLICKED(IDC_SENDMESSAGE_BTNCANCEL, &CMessageSendDlg::OnBnClickedSendmessageBtncancel)
	ON_BN_CLICKED(IDC_SENDMESSAGE_BTNADD, &CMessageSendDlg::OnBnClickedSendmessageBtnadd)
	ON_BN_CLICKED(IDC_SENDMESSAGE_BTNDELETE, &CMessageSendDlg::OnBnClickedSendmessageBtndelete)
	ON_BN_CLICKED(IDC_SENDMESSAGE_BTNOK, &CMessageSendDlg::OnBnClickedSendmessageBtnok)
	//ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &CMessageSendDlg::OnTvnSelchangedTree1)
END_MESSAGE_MAP()


// CMessageSendDlg 消息处理程序
BOOL CMessageSendDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	for( map<int, pair<string, int> >::iterator iter = theApp.m_mates.begin();
    iter != theApp.m_mates.end(); iter++)
	{
		string name = theApp.m_departmentName[iter->second.second] + " " + iter->second.first;
		m_lstMates.AddString( name.c_str());
	}
	return TRUE;  
}

void CMessageSendDlg::OnBnClickedSendmessageBtncancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialog::OnCancel();
}


void CMessageSendDlg::OnBnClickedSendmessageBtnadd()
{
	// TODO: 在此添加控件通知处理程序代码
	int index = m_lstMates.GetCurSel();
	if( index == LB_ERR)
		return;

  CString mate;
  m_lstMates.GetText( index, mate);
  char department[256], name[256];
  sscanf( mate.GetBuffer(), "%s %s", department, name);
  for( map<int, pair< string, int> >::iterator iter = theApp.m_mates.begin();
    iter != theApp.m_mates.end(); ++iter)
  {
    if( iter->second.first != name)
      continue;
    if( theApp.m_departmentName[ iter->second.second] != department)
      continue;
    m_lstAddTo.AddString( mate);
    m_lstMates.DeleteString( index);
    m_idAddTo.insert( iter->first);
    break;
  }
}


void CMessageSendDlg::OnBnClickedSendmessageBtndelete()
{
	// TODO: 在此添加控件通知处理程序代码
  int index = m_lstAddTo.GetCurSel();
  if( index == LB_ERR)
    return;

  CString mate;
  m_lstAddTo.GetText( index, mate);
  char department[256], name[256];
  sscanf( mate.GetBuffer(), "%s %s", department, name);
  for( map<int, pair< string, int> >::iterator iter = theApp.m_mates.begin();
    iter != theApp.m_mates.end(); ++iter)
  {
    if( iter->second.first != name)
      continue;
    if( theApp.m_departmentName[ iter->second.second] != department)
      continue;
    m_lstMates.AddString( mate);
    m_lstAddTo.DeleteString( index);
    m_idAddTo.erase( iter->first);
    break;
  }
}


void CMessageSendDlg::OnBnClickedSendmessageBtnok()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	char str[1000];
	sprintf(str,"确认发送消息'%s'?",m_edit);
	if(MessageBox(str,"确认发送",MB_OKCANCEL)!=TRUE)return;
	//发送数据。
	//格式 fromID,message,touser;
	UpdateData(true);
	set<int> realAddTo;
	realAddTo = m_idAddTo;
	for( set<int>::iterator iter = realAddTo.begin(); iter != realAddTo.end(); iter++)
	{
		RakNet::BitStream out;
		RakNet::RakString message = m_edit;
		out.Write( (RakNet::MessageID)RH_SENDMESSAGE);
		out.Write( SEND);
		out.Write( theApp.m_ID);
		out.Write( *iter);
		out.Write( message);
		theApp.m_peer->Send( &out, HIGH_PRIORITY, RELIABLE_ORDERED, 0, theApp.m_serverAddress, false);
	}
	return;
}
