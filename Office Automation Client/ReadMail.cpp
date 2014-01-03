// ReadMail.cpp : 实现文件
//

#include "stdafx.h"
#include "Office Automation Client.h"
#include "ReadMail.h"
#include "afxdialogex.h"
#include "PacketType.h"


// ReadMail 对话框

IMPLEMENT_DYNAMIC(ReadMail, CDialogEx)

ReadMail::ReadMail(CWnd* pParent /*=NULL*/)
	: CDialogEx(ReadMail::IDD, pParent)
{

}

ReadMail::~ReadMail()
{
}

void ReadMail::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(ReadMail, CDialogEx)
	ON_BN_CLICKED(IDC_ONCANCLE, &ReadMail::OnBnClickedOncancle)
	ON_BN_CLICKED(IDC_MAIL2, &ReadMail::OnBnClickedMail2)
	ON_BN_CLICKED(IDC_MAIL3, &ReadMail::OnBnClickedMail3)
END_MESSAGE_MAP()


// ReadMail 消息处理程序


void ReadMail::OnBnClickedOncancle()
{
	// TODO: 在此添加控件通知处理程序代码
	if(theApp.mailList[MailNum].type == 0 && theApp.mailList[MailNum].fromUser == theApp.m_ID)
	{
		if(theApp.mailList[MailNum].isRead == 2)
		{
				theApp.mailList[MailNum].isRead = 6;
				RakNet::BitStream outMail;
				outMail.Write( (RakNet::MessageID)RH_SENDMAIL);//收公文提示消息
				outMail.Write( theApp.mailList[MailNum].fromUser);
				outMail.Write( theApp.mailList[MailNum].toDepartment);
				outMail.Write( theApp.mailList[MailNum].title);
				outMail.Write( theApp.mailList[MailNum].content);
				outMail.Write( theApp.mailList[MailNum].isRead);
				outMail.Write( theApp.mailList[MailNum].t);
				outMail.Write( theApp.mailList[MailNum].type);
				theApp.m_peer->Send( &outMail, HIGH_PRIORITY, RELIABLE_ORDERED, 0, theApp.m_serverAddress, false);
		}
		else if(theApp.mailList[MailNum].isRead == 3)
		{
				theApp.mailList[MailNum].isRead = 7;
				RakNet::BitStream outMail;
				outMail.Write( (RakNet::MessageID)RH_SENDMAIL);//收公文提示消息
				outMail.Write( theApp.mailList[MailNum].fromUser);
				outMail.Write( theApp.mailList[MailNum].toDepartment);
				outMail.Write( theApp.mailList[MailNum].title);
				outMail.Write( theApp.mailList[MailNum].content);
				outMail.Write( theApp.mailList[MailNum].isRead);
				outMail.Write( theApp.mailList[MailNum].t);
				outMail.Write( theApp.mailList[MailNum].type);
				theApp.m_peer->Send( &outMail, HIGH_PRIORITY, RELIABLE_ORDERED, 0, theApp.m_serverAddress, false);
		}
		//else return;
	}
	else if(theApp.mailList[MailNum].type == 1 && theApp.mailList[MailNum].toDepartment == theApp.m_ID)
	{
		if(theApp.mailList[MailNum].isRead == 4)
		{
			theApp.mailList[MailNum].isRead = 5;
			RakNet::BitStream outMail;
			outMail.Write( (RakNet::MessageID)RH_SENDMAIL);//收公文提示消息
			outMail.Write( theApp.mailList[MailNum].fromUser);
			outMail.Write( theApp.mailList[MailNum].toDepartment);
			outMail.Write( theApp.mailList[MailNum].title);
			outMail.Write( theApp.mailList[MailNum].content);
			outMail.Write( theApp.mailList[MailNum].isRead);
			outMail.Write( theApp.mailList[MailNum].t);
			outMail.Write( theApp.mailList[MailNum].type);
			theApp.m_peer->Send( &outMail, HIGH_PRIORITY, RELIABLE_ORDERED, 0, theApp.m_serverAddress, false);
		}
		//else return;
	}

	CDialog::OnCancel();
}

BOOL ReadMail::OnInitDialog()
{
	CDialog::OnInitDialog();

	//MailNum对应theApp.mailList[MailNum]为选中项
	
	CStatic *pStatic1=(CStatic*)GetDlgItem(IDC_MAILTITLE);
	pStatic1->SetWindowText(theApp.mailList[MailNum].title);

	CStatic *pStatic2=(CStatic*)GetDlgItem(IDC_MAILCONTENT);
	pStatic2->SetWindowText(theApp.mailList[MailNum].content);

	//获取起草人
	string tmp;
	CString c_fromuser,c_todepartment,c_time;
	map<int, pair<string, int> >::iterator iter;
	iter = theApp.m_mates.find( theApp.mailList[MailNum].fromUser);
	if( iter != theApp.m_mates.end())
	{
		tmp = theApp.m_departmentName[iter->second.second] + " " + iter->second.first;
	}
	c_fromuser.Format("%s", tmp.c_str());

	//获取接受部门
	map<int, pair<string, int> >::iterator iter2;
	iter2 = theApp.m_mates.find( theApp.mailList[MailNum].fromUser);
	if( iter2 != theApp.m_mates.end())
	{
		tmp = theApp.m_departmentName[iter->second.second] + " " + iter->second.first;
	}
	c_fromuser.Format("%s", tmp.c_str());

	map<int, string>::iterator iter3;
	iter3 = theApp.m_departmentName.find( theApp.mailList[MailNum].toDepartment);
	if( iter3 != theApp.m_departmentName.end())
	{
		tmp = iter3->second;
	}
	c_todepartment.Format("%s",tmp.c_str());

	//时间换算
	char s[100];
	time_t tick = theApp.mailList[MailNum].t;
	struct tm tm;
	tm = *localtime(&tick);
	strftime(s, sizeof(s), "%Y-%m-%d %H:%M:%S", &tm);
	c_time.Format("%s", s);

	if(theApp.mailList[MailNum].isRead == 4 || theApp.mailList[MailNum].isRead == 5)
	{
		c_todepartment = "本部门";
	}

	//合成info
	char str[1024];
	sprintf(str,"起草人:%s 接收部门:%s 起草时间:%s",c_fromuser,c_todepartment,c_time);
	CStatic *pStatic3=(CStatic*)GetDlgItem(IDC_MAILINFO);
	pStatic3->SetWindowText(str);

	if(theApp.mailList[MailNum].isRead != 1)
	{
		GetDlgItem(IDC_MAIL2)->EnableWindow(FALSE);
		GetDlgItem(IDC_MAIL3)->EnableWindow(FALSE);
	}
	else 
	{
		//find组长ID
		int boss_no;
		map<int, int>::iterator iter_boss;
		iter_boss = theApp.m_departmentMaster.find(theApp.m_department);
		if(iter_boss != theApp.m_departmentMaster.end())
		{
			boss_no = iter_boss->second;
		}
		if(theApp.m_ID != boss_no)//如果不是组长
		{
			GetDlgItem(IDC_MAIL2)->EnableWindow(FALSE);
			GetDlgItem(IDC_MAIL3)->EnableWindow(FALSE);
		}
	}
	return TRUE;
}


void ReadMail::OnBnClickedMail2()
{
	// TODO: 在此添加控件通知处理程序代码
	if(theApp.mailList[MailNum].isRead !=1) return;
	theApp.mailList[MailNum].isRead = 2;

	//通过审批的方法
	RakNet::BitStream outMail;
	outMail.Write( (RakNet::MessageID)RH_SENDMAIL);//收公文提示消息
	outMail.Write( theApp.mailList[MailNum].fromUser);
	outMail.Write( theApp.mailList[MailNum].toDepartment);
	outMail.Write( theApp.mailList[MailNum].title);
	outMail.Write( theApp.mailList[MailNum].content);
	outMail.Write( theApp.mailList[MailNum].isRead);
	outMail.Write( theApp.mailList[MailNum].t);
	outMail.Write( theApp.mailList[MailNum].type);
	theApp.m_peer->Send( &outMail, HIGH_PRIORITY, RELIABLE_ORDERED, 0, theApp.m_serverAddress, false);

	CDialog::OnCancel();
}


void ReadMail::OnBnClickedMail3()
{
	// TODO: 在此添加控件通知处理程序代码
	if(theApp.mailList[MailNum].isRead !=1) return;
	theApp.mailList[MailNum].isRead = 3;
	
	//未通过审批的方法
	RakNet::BitStream outMail;
	outMail.Write( (RakNet::MessageID)RH_SENDMAIL);//收公文提示消息
	outMail.Write( theApp.mailList[MailNum].fromUser);
	outMail.Write( theApp.mailList[MailNum].toDepartment);
	outMail.Write( theApp.mailList[MailNum].title);
	outMail.Write( theApp.mailList[MailNum].content);
	outMail.Write( theApp.mailList[MailNum].isRead);
	outMail.Write( theApp.mailList[MailNum].t);
	outMail.Write( theApp.mailList[MailNum].type);
	theApp.m_peer->Send( &outMail, HIGH_PRIORITY, RELIABLE_ORDERED, 0, theApp.m_serverAddress, false);

	CDialog::OnCancel();
}
