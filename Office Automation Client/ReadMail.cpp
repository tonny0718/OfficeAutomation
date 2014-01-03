// ReadMail.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Office Automation Client.h"
#include "ReadMail.h"
#include "afxdialogex.h"
#include "PacketType.h"


// ReadMail �Ի���

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


// ReadMail ��Ϣ�������


void ReadMail::OnBnClickedOncancle()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(theApp.mailList[MailNum].type == 0 && theApp.mailList[MailNum].fromUser == theApp.m_ID)
	{
		if(theApp.mailList[MailNum].isRead == 2)
		{
				theApp.mailList[MailNum].isRead = 6;
				RakNet::BitStream outMail;
				outMail.Write( (RakNet::MessageID)RH_SENDMAIL);//�չ�����ʾ��Ϣ
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
				outMail.Write( (RakNet::MessageID)RH_SENDMAIL);//�չ�����ʾ��Ϣ
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
			outMail.Write( (RakNet::MessageID)RH_SENDMAIL);//�չ�����ʾ��Ϣ
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

	//MailNum��ӦtheApp.mailList[MailNum]Ϊѡ����
	
	CStatic *pStatic1=(CStatic*)GetDlgItem(IDC_MAILTITLE);
	pStatic1->SetWindowText(theApp.mailList[MailNum].title);

	CStatic *pStatic2=(CStatic*)GetDlgItem(IDC_MAILCONTENT);
	pStatic2->SetWindowText(theApp.mailList[MailNum].content);

	//��ȡ�����
	string tmp;
	CString c_fromuser,c_todepartment,c_time;
	map<int, pair<string, int> >::iterator iter;
	iter = theApp.m_mates.find( theApp.mailList[MailNum].fromUser);
	if( iter != theApp.m_mates.end())
	{
		tmp = theApp.m_departmentName[iter->second.second] + " " + iter->second.first;
	}
	c_fromuser.Format("%s", tmp.c_str());

	//��ȡ���ܲ���
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

	//ʱ�任��
	char s[100];
	time_t tick = theApp.mailList[MailNum].t;
	struct tm tm;
	tm = *localtime(&tick);
	strftime(s, sizeof(s), "%Y-%m-%d %H:%M:%S", &tm);
	c_time.Format("%s", s);

	if(theApp.mailList[MailNum].isRead == 4 || theApp.mailList[MailNum].isRead == 5)
	{
		c_todepartment = "������";
	}

	//�ϳ�info
	char str[1024];
	sprintf(str,"�����:%s ���ղ���:%s ���ʱ��:%s",c_fromuser,c_todepartment,c_time);
	CStatic *pStatic3=(CStatic*)GetDlgItem(IDC_MAILINFO);
	pStatic3->SetWindowText(str);

	if(theApp.mailList[MailNum].isRead != 1)
	{
		GetDlgItem(IDC_MAIL2)->EnableWindow(FALSE);
		GetDlgItem(IDC_MAIL3)->EnableWindow(FALSE);
	}
	else 
	{
		//find�鳤ID
		int boss_no;
		map<int, int>::iterator iter_boss;
		iter_boss = theApp.m_departmentMaster.find(theApp.m_department);
		if(iter_boss != theApp.m_departmentMaster.end())
		{
			boss_no = iter_boss->second;
		}
		if(theApp.m_ID != boss_no)//��������鳤
		{
			GetDlgItem(IDC_MAIL2)->EnableWindow(FALSE);
			GetDlgItem(IDC_MAIL3)->EnableWindow(FALSE);
		}
	}
	return TRUE;
}


void ReadMail::OnBnClickedMail2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(theApp.mailList[MailNum].isRead !=1) return;
	theApp.mailList[MailNum].isRead = 2;

	//ͨ�������ķ���
	RakNet::BitStream outMail;
	outMail.Write( (RakNet::MessageID)RH_SENDMAIL);//�չ�����ʾ��Ϣ
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(theApp.mailList[MailNum].isRead !=1) return;
	theApp.mailList[MailNum].isRead = 3;
	
	//δͨ�������ķ���
	RakNet::BitStream outMail;
	outMail.Write( (RakNet::MessageID)RH_SENDMAIL);//�չ�����ʾ��Ϣ
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
