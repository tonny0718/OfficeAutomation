// MailListDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Office Automation Client.h"
#include "MailListDlg.h"
#include "afxdialogex.h"
#include "SendMailDlg.h"
#include "ReadMail.h"


// CMailListDlg �Ի���

IMPLEMENT_DYNAMIC(CMailListDlg, CDialogEx)

CMailListDlg::CMailListDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMailListDlg::IDD, pParent)
{

}

CMailListDlg::~CMailListDlg()
{
}

void CMailListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MAILLIST_LIST, m_list);
}


BEGIN_MESSAGE_MAP(CMailListDlg, CDialogEx)
	ON_BN_CLICKED(IDC_MAILLIST_BTNRETURN, &CMailListDlg::OnBnClickedMaillistBtnreturn)
	ON_BN_CLICKED(IDC_MAILLIST_BTNNEW, &CMailListDlg::OnBnClickedMaillistBtnnew)
	ON_NOTIFY(NM_DBLCLK, IDC_MAILLIST_LIST, &CMailListDlg::OnNMDblclkMaillistList)
	ON_BN_CLICKED(IDC_MAILLIST_ALL, &CMailListDlg::OnBnClickedMaillistAll)
	ON_BN_CLICKED(IDC_MAILLIST_BTNRECIEVE, &CMailListDlg::OnBnClickedMaillistBtnrecieve)
	ON_BN_CLICKED(IDC_MAILLIST_BTNSEND, &CMailListDlg::OnBnClickedMaillistBtnsend)
END_MESSAGE_MAP()


// CMailListDlg ��Ϣ�������


void CMailListDlg::OnBnClickedMaillistBtnreturn()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialog::OnCancel();
}


void CMailListDlg::OnBnClickedMaillistBtnnew()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//CSendMailDlg sendMailDlg;
	//sendMailDlg.DoModal();
	char* strmsg;
	unsigned ret;
	_beginthreadex( NULL, 0, NewMailThread, strmsg, 0, &ret);
}

BOOL CMailListDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	//��ʼ���б�
	LONG lStyle; 
	lStyle = GetWindowLong(m_list.m_hWnd, GWL_STYLE);// ��ȡ��ǰ����style 
	lStyle &= ~LVS_TYPEMASK; // �����ʾ��ʽλ 
	lStyle |= LVS_REPORT; // ����style 
	SetWindowLong(m_list.m_hWnd, GWL_STYLE, lStyle);// ����style 
	DWORD dwStyle = m_list.GetExtendedStyle(); 
	dwStyle |= LVS_EX_FULLROWSELECT;// ѡ��ĳ��ʹ���и�����ֻ������report ����listctrl �� 
	dwStyle |= LVS_EX_GRIDLINES;// �����ߣ�ֻ������report ����listctrl �� 
	m_list.SetExtendedStyle(dwStyle); // ������չ��� 
	
	m_list.InsertColumn( 0, "���", LVCFMT_LEFT, 40);
	m_list.InsertColumn( 1, "����", LVCFMT_LEFT, 135);
	m_list.InsertColumn( 2, "�����", LVCFMT_LEFT, 100);
	m_list.InsertColumn( 3, "����", LVCFMT_LEFT, 70);
	m_list.InsertColumn( 4, "״̬", LVCFMT_LEFT, 70);

	//��������
	for(int n=theApp.mailCount-1;n>=0;n--)
	{
		string tmp;
		CString c_n;
		c_n.Format("%d",n+1);
		int nRow = m_list.InsertItem( 0, c_n);//���

		m_list.SetItemText(nRow, 1, theApp.mailList[n].title.C_String());//���ı���

		CString c_fromuser;
		map<int, pair<string, int> >::iterator iter;
		iter = theApp.m_mates.find( theApp.mailList[n].fromUser);
		if( iter != theApp.m_mates.end())
		{
			tmp = theApp.m_departmentName[iter->second.second] + " " + iter->second.first;
		}
		c_fromuser.Format("%s", tmp.c_str());
		m_list.SetItemText(nRow, 2, c_fromuser);//�����
		
		if(theApp.mailList[n].type == 0)   m_list.SetItemText(nRow, 3, "��˽׶�");
		else if(theApp.mailList[n].type == 1)    m_list.SetItemText(nRow, 3, "����Ч");
		else m_list.SetItemText(nRow, 3, "δ֪");

		if(theApp.mailList[n].isRead == 1)   m_list.SetItemText(nRow, 4, "������");
		else if(theApp.mailList[n].isRead == 2)   m_list.SetItemText(nRow, 4, "����ͨ��");
		else if(theApp.mailList[n].isRead == 3)   m_list.SetItemText(nRow, 4, "����δͨ��");
		else if(theApp.mailList[n].isRead == 4)   m_list.SetItemText(nRow, 4, "δ��");
		else if(theApp.mailList[n].isRead == 5)   m_list.SetItemText(nRow, 4, "�Ѷ�");
		else if(theApp.mailList[n].isRead == 6)   m_list.SetItemText(nRow, 4, "����ͨ��(�Ѷ�)");
		else if(theApp.mailList[n].isRead == 7)   m_list.SetItemText(nRow, 4, "����δͨ��(�Ѷ�)");
		else m_list.SetItemText(nRow, 4, "δ֪");
		
	}

	return TRUE;
}



void CMailListDlg::OnNMDblclkMaillistList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
		*pResult = 0;
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;

	int nId; 
	POSITION pos=m_list.GetFirstSelectedItemPosition();//�õ������λ��  
	if(pos==NULL) {return;}//�� ������
	nId=(int)m_list.GetNextSelectedItem(pos);//�õ��кţ�ͨ��POSITIONת��

	CString s=m_list.GetItemText(nId,0);
	//MessageBox(s);
	int num;
	num = _tstoi( LPCTSTR(s));

	ReadMail readMail;
	readMail.MailNum = num-1;
	readMail.DoModal();
}


void CMailListDlg::OnBnClickedMaillistAll()//��ʾȫ����ˢ�£�
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_list.DeleteAllItems();

	//��������
	for(int n=theApp.mailCount-1;n>=0;n--)
	{
		string tmp;
		CString c_n;
		c_n.Format("%d",n+1);
		int nRow = m_list.InsertItem( 0, c_n);//���

		m_list.SetItemText(nRow, 1, theApp.mailList[n].title.C_String());//���ı���

		CString c_fromuser;
		map<int, pair<string, int> >::iterator iter;
		iter = theApp.m_mates.find( theApp.mailList[n].fromUser);
		if( iter != theApp.m_mates.end())
		{
			tmp = theApp.m_departmentName[iter->second.second] + " " + iter->second.first;
		}
		c_fromuser.Format("%s", tmp.c_str());
		m_list.SetItemText(nRow, 2, c_fromuser);//�����
		
		if(theApp.mailList[n].type == 0)   m_list.SetItemText(nRow, 3, "��˽׶�");
		else if(theApp.mailList[n].type == 1)    m_list.SetItemText(nRow, 3, "����Ч");
		else m_list.SetItemText(nRow, 3, "δ֪");

		if(theApp.mailList[n].isRead == 1)   m_list.SetItemText(nRow, 4, "������");
		else if(theApp.mailList[n].isRead == 2)   m_list.SetItemText(nRow, 4, "����ͨ��");
		else if(theApp.mailList[n].isRead == 3)   m_list.SetItemText(nRow, 4, "����δͨ��");
		else if(theApp.mailList[n].isRead == 4)   m_list.SetItemText(nRow, 4, "δ��");
		else if(theApp.mailList[n].isRead == 5)   m_list.SetItemText(nRow, 4, "�Ѷ�");
		else if(theApp.mailList[n].isRead == 6)   m_list.SetItemText(nRow, 4, "����ͨ��(�Ѷ�)");
		else if(theApp.mailList[n].isRead == 7)   m_list.SetItemText(nRow, 4, "����δͨ��(�Ѷ�)");
		else m_list.SetItemText(nRow, 4, "δ֪");
		
	}
}


void CMailListDlg::OnBnClickedMaillistBtnrecieve()//�ռ���
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_list.DeleteAllItems();

	//��������
	for(int n=theApp.mailCount-1;n>=0;n--)
	{
		if(theApp.mailList[n].type ==0) continue;
		string tmp;
		CString c_n;
		c_n.Format("%d",n+1);
		int nRow = m_list.InsertItem( 0, c_n);//���

		m_list.SetItemText(nRow, 1, theApp.mailList[n].title.C_String());//���ı���

		CString c_fromuser;
		map<int, pair<string, int> >::iterator iter;
		iter = theApp.m_mates.find( theApp.mailList[n].fromUser);
		if( iter != theApp.m_mates.end())
		{
			tmp = theApp.m_departmentName[iter->second.second] + " " + iter->second.first;
		}
		c_fromuser.Format("%s", tmp.c_str());
		m_list.SetItemText(nRow, 2, c_fromuser);//�����
		
		if(theApp.mailList[n].type == 0)   m_list.SetItemText(nRow, 3, "��˽׶�");
		else if(theApp.mailList[n].type == 1)    m_list.SetItemText(nRow, 3, "����Ч");
		else m_list.SetItemText(nRow, 3, "δ֪");

		if(theApp.mailList[n].isRead == 1)   m_list.SetItemText(nRow, 4, "������");
		else if(theApp.mailList[n].isRead == 2)   m_list.SetItemText(nRow, 4, "����ͨ��");
		else if(theApp.mailList[n].isRead == 3)   m_list.SetItemText(nRow, 4, "����δͨ��");
		else if(theApp.mailList[n].isRead == 4)   m_list.SetItemText(nRow, 4, "δ��");
		else if(theApp.mailList[n].isRead == 5)   m_list.SetItemText(nRow, 4, "�Ѷ�");
		else if(theApp.mailList[n].isRead == 6)   m_list.SetItemText(nRow, 4, "����ͨ��(�Ѷ�)");
		else if(theApp.mailList[n].isRead == 7)   m_list.SetItemText(nRow, 4, "����δͨ��(�Ѷ�)");
		else m_list.SetItemText(nRow, 4, "δ֪");
		
	}
}


void CMailListDlg::OnBnClickedMaillistBtnsend()//������
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_list.DeleteAllItems();

	//��������
	for(int n=theApp.mailCount-1;n>=0;n--)
	{
		if(theApp.mailList[n].type == 1) continue;
		string tmp;
		CString c_n;
		c_n.Format("%d",n+1);
		int nRow = m_list.InsertItem( 0, c_n);//���

		m_list.SetItemText(nRow, 1, theApp.mailList[n].title.C_String());//���ı���

		CString c_fromuser;
		map<int, pair<string, int> >::iterator iter;
		iter = theApp.m_mates.find( theApp.mailList[n].fromUser);
		if( iter != theApp.m_mates.end())
		{
			tmp = theApp.m_departmentName[iter->second.second] + " " + iter->second.first;
		}
		c_fromuser.Format("%s", tmp.c_str());
		m_list.SetItemText(nRow, 2, c_fromuser);//�����
		
		if(theApp.mailList[n].type == 0)   m_list.SetItemText(nRow, 3, "��˽׶�");
		else if(theApp.mailList[n].type == 1)    m_list.SetItemText(nRow, 3, "����Ч");
		else m_list.SetItemText(nRow, 3, "δ֪");

		if(theApp.mailList[n].isRead == 1)   m_list.SetItemText(nRow, 4, "������");
		else if(theApp.mailList[n].isRead == 2)   m_list.SetItemText(nRow, 4, "����ͨ��");
		else if(theApp.mailList[n].isRead == 3)   m_list.SetItemText(nRow, 4, "����δͨ��");
		else if(theApp.mailList[n].isRead == 4)   m_list.SetItemText(nRow, 4, "δ��");
		else if(theApp.mailList[n].isRead == 5)   m_list.SetItemText(nRow, 4, "�Ѷ�");
		else if(theApp.mailList[n].isRead == 6)   m_list.SetItemText(nRow, 4, "����ͨ��(�Ѷ�)");
		else if(theApp.mailList[n].isRead == 7)   m_list.SetItemText(nRow, 4, "����δͨ��(�Ѷ�)");
		else m_list.SetItemText(nRow, 4, "δ֪");
		
	}
}
