// SendMailDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Office Automation Client.h"
#include "SendMailDlg.h"
#include "afxdialogex.h"
#include "PacketType.h"
#include "ctime"
#include "AddRecipient.h"


// CSendMailDlg �Ի���

IMPLEMENT_DYNAMIC(CSendMailDlg, CDialogEx)

CSendMailDlg::CSendMailDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSendMailDlg::IDD, pParent)
	, m_title(_T(""))
	, m_content(_T(""))
	, m_departments(_T(""))
{

}

CSendMailDlg::~CSendMailDlg()
{
}

void CSendMailDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_SENDMAIL_EDTTITLE, m_title);
	DDX_Text(pDX, IDC_SENDMAIL_EDTCONTENT, m_content);
	DDX_Text(pDX, IDC_SENDMAIL_EDTRECIPIENT, m_departments);
}


BEGIN_MESSAGE_MAP(CSendMailDlg, CDialogEx)
	ON_BN_CLICKED(IDC_SENDMAIL_BTNCANCEL, &CSendMailDlg::OnBnClickedSendmailBtncancel)
	ON_BN_CLICKED(IDC_SENDMAIL_BTNSEND, &CSendMailDlg::OnBnClickedSendmailBtnsend)
	ON_BN_CLICKED(IDC_SENDMAIL_BTNCHOOSERECIPIENT, &CSendMailDlg::OnBnClickedSendmailBtnchooserecipient)
END_MESSAGE_MAP()


// CSendMailDlg ��Ϣ�������


void CSendMailDlg::OnBnClickedSendmailBtncancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialog::OnCancel();
}


BOOL CSendMailDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	set<int> a;
	//a.insert( 1);
	a.insert( 3);//ȱʡ����ִ�в�
	theApp.mailaccepteddepartments = a;

	CString strRe = "";
	CString c_tmp;
	string tmp;
	int flag1 = 1;

	set<int> realAddTo = theApp.mailaccepteddepartments;
	for( set<int>::iterator iter = realAddTo.begin(); iter != realAddTo.end(); iter++)
	{
		*iter;
		//��ȡ������
		
		map<int,string >::iterator iter2;
		iter2 = theApp.m_departmentName.find( *iter);
		if( iter2 != theApp.m_departmentName.end())
		{
			tmp = iter2->second;
		}
		c_tmp.Format("%s", tmp.c_str());
		//�ҵ�һ�������� ���ϼ���
		if(flag1)
		{
			strRe+=c_tmp;
			flag1=0;
		}
		else
		{
			strRe+=","+c_tmp;
		}
		
	}
	m_departments = strRe;
	UpdateData(FALSE);


	return TRUE;
}

void CSendMailDlg::OnBnClickedSendmailBtnsend()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);

	for( set<int>::iterator iter = theApp.mailaccepteddepartments.begin();
	iter != theApp.mailaccepteddepartments.end(); iter++)
	{

		RakNet::RakString title,content;
		title = m_title;
		content = m_content;
		int isRead = 1;
		time_t now_time = time(NULL);
		int t = now_time;
		int toDepartment = *iter /*m_departments*/;
		int type = 0;

		RakNet::BitStream outMail;
		outMail.Write( (RakNet::MessageID)RH_SENDMAIL);
		outMail.Write( theApp.m_ID);
		outMail.Write( toDepartment);
		outMail.Write( title);
		outMail.Write( content);
		outMail.Write( isRead);
		outMail.Write( t);
		outMail.Write( type);
		theApp.m_peer->Send( &outMail, HIGH_PRIORITY, RELIABLE_ORDERED, 0, theApp.m_serverAddress, false);

		theApp.tmpmail.fromUser = theApp.m_ID;
		theApp.tmpmail.toDepartment = toDepartment;
		theApp.tmpmail.title = title;
		theApp.tmpmail.content = content;
		theApp.tmpmail.isRead = isRead;
		theApp.tmpmail.t = t;
		theApp.tmpmail.type = type;

		theApp.mailList[theApp.mailCount] = theApp.tmpmail;
		theApp.mailCount++;
	}
	CDialog::OnCancel();
}


void CSendMailDlg::OnBnClickedSendmailBtnchooserecipient()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CAddRecipient addRecipient;
	addRecipient.DoModal();
	CString strRe;
	CString c_tmp;
	string tmp;
	int flag = 1;

	set<int> realAddTo = theApp.mailaccepteddepartments;
	for( set<int>::iterator iter = realAddTo.begin(); iter != realAddTo.end(); iter++)
	{
		*iter;
		//��ȡ������
		map<int,string >::iterator iter2;
		iter2 = theApp.m_departmentName.find( *iter);
		if( iter2 != theApp.m_departmentName.end())
		{
			tmp = iter2->second;
		}
		c_tmp.Format("%s", tmp.c_str());
		//�ҵ�һ�������� ���ϼ���
		if(flag)
		{
			strRe+=c_tmp;
			flag=0;
		}
		else
		{
			strRe+=","+c_tmp;
		}
	}
	m_departments = strRe;
	UpdateData(FALSE);

}

unsigned WINAPI NewMailThread( void* message)
{
	CSendMailDlg newMessage;
	newMessage.DoModal();
  return 0;
}