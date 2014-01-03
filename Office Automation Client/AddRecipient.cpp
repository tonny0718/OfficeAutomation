// AddRecipient.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Office Automation Client.h"
#include "AddRecipient.h"
#include "afxdialogex.h"
#include "PacketType.h"


// CAddRecipient �Ի���

IMPLEMENT_DYNAMIC(CAddRecipient, CDialogEx)

CAddRecipient::CAddRecipient(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAddRecipient::IDD, pParent)
{

}

CAddRecipient::~CAddRecipient()
{
}

void CAddRecipient::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ADDRECIPIENT_LSTADDTO, m_lstAddTo);
	DDX_Control(pDX, IDC_ADDRECIPIENT_LSTDEP, m_lstDep);
}


BEGIN_MESSAGE_MAP(CAddRecipient, CDialogEx)
	ON_BN_CLICKED(IDC_ADDRECIPIENT_BTNCANCEL, &CAddRecipient::OnBnClickedAddrecipientBtncancel)
	ON_BN_CLICKED(IDC_ADDRECIPIENT_BTNOK, &CAddRecipient::OnBnClickedAddrecipientBtnok)
	ON_BN_CLICKED(IDC_ADDRECIPIENT_BTNADD, &CAddRecipient::OnBnClickedAddrecipientBtnadd)
	ON_BN_CLICKED(IDC_ADDRECIPIENT_BTNDELETE, &CAddRecipient::OnBnClickedAddrecipientBtndelete)
END_MESSAGE_MAP()


// CAddRecipient ��Ϣ�������
BOOL CAddRecipient::OnInitDialog()
{
	CDialog::OnInitDialog();
	for( map<int, string>::iterator depiter = theApp.m_departmentName.begin();
	depiter != theApp.m_departmentName.end(); depiter++)
	{
		string name = depiter->second;
		m_lstDep.AddString( name.c_str());
	}
	return TRUE;  
}


void CAddRecipient::OnBnClickedAddrecipientBtncancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialog::OnCancel();
}


void CAddRecipient::OnBnClickedAddrecipientBtnok()//ȷ�� ���ص�����ҳ��
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	theApp.mailaccepteddepartments = m_depIdAddTo;
	CDialog::OnCancel();
}


void CAddRecipient::OnBnClickedAddrecipientBtnadd()//��Ӳ���
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int index = m_lstDep.GetCurSel();
	if( index == LB_ERR) return;

	CString dep;
	m_lstDep.GetText( index, dep);
	char department[50];
	sscanf( dep.GetBuffer(), "%s", department);
	for( map<int, string>::iterator depiter = theApp.m_departmentName.begin();depiter != theApp.m_departmentName.end(); ++depiter)
	{
		if( depiter->second != department)
		continue;
		m_lstAddTo.AddString( dep);
		m_lstDep.DeleteString( index);
		m_depIdAddTo.insert( depiter->first);
		break;
	}
}


void CAddRecipient::OnBnClickedAddrecipientBtndelete()//�Ƴ�����
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int index = m_lstAddTo.GetCurSel();
	if( index == LB_ERR) return;

	CString dep;
	m_lstAddTo.GetText( index, dep);
	char department[50];
	sscanf( dep.GetBuffer(), "%s", department);
	for( map<int, string>::iterator depiter = theApp.m_departmentName.begin();depiter != theApp.m_departmentName.end(); ++depiter)
	{
		if( depiter->second != department)
		continue;
		m_lstDep.AddString( dep);
		m_lstAddTo.DeleteString( index);
		m_depIdAddTo.erase( depiter->first);
		break;
	}
}
