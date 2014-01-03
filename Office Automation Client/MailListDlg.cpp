// MailListDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Office Automation Client.h"
#include "MailListDlg.h"
#include "afxdialogex.h"
#include "SendMailDlg.h"
#include "ReadMail.h"


// CMailListDlg 对话框

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


// CMailListDlg 消息处理程序


void CMailListDlg::OnBnClickedMaillistBtnreturn()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialog::OnCancel();
}


void CMailListDlg::OnBnClickedMaillistBtnnew()
{
	// TODO: 在此添加控件通知处理程序代码
	//CSendMailDlg sendMailDlg;
	//sendMailDlg.DoModal();
	char* strmsg;
	unsigned ret;
	_beginthreadex( NULL, 0, NewMailThread, strmsg, 0, &ret);
}

BOOL CMailListDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	//初始化列表
	LONG lStyle; 
	lStyle = GetWindowLong(m_list.m_hWnd, GWL_STYLE);// 获取当前窗口style 
	lStyle &= ~LVS_TYPEMASK; // 清除显示方式位 
	lStyle |= LVS_REPORT; // 设置style 
	SetWindowLong(m_list.m_hWnd, GWL_STYLE, lStyle);// 设置style 
	DWORD dwStyle = m_list.GetExtendedStyle(); 
	dwStyle |= LVS_EX_FULLROWSELECT;// 选中某行使整行高亮（只适用与report 风格的listctrl ） 
	dwStyle |= LVS_EX_GRIDLINES;// 网格线（只适用与report 风格的listctrl ） 
	m_list.SetExtendedStyle(dwStyle); // 设置扩展风格 
	
	m_list.InsertColumn( 0, "序号", LVCFMT_LEFT, 40);
	m_list.InsertColumn( 1, "标题", LVCFMT_LEFT, 135);
	m_list.InsertColumn( 2, "起草人", LVCFMT_LEFT, 100);
	m_list.InsertColumn( 3, "类型", LVCFMT_LEFT, 70);
	m_list.InsertColumn( 4, "状态", LVCFMT_LEFT, 70);

	//插入内容
	for(int n=theApp.mailCount-1;n>=0;n--)
	{
		string tmp;
		CString c_n;
		c_n.Format("%d",n+1);
		int nRow = m_list.InsertItem( 0, c_n);//序号

		m_list.SetItemText(nRow, 1, theApp.mailList[n].title.C_String());//公文标题

		CString c_fromuser;
		map<int, pair<string, int> >::iterator iter;
		iter = theApp.m_mates.find( theApp.mailList[n].fromUser);
		if( iter != theApp.m_mates.end())
		{
			tmp = theApp.m_departmentName[iter->second.second] + " " + iter->second.first;
		}
		c_fromuser.Format("%s", tmp.c_str());
		m_list.SetItemText(nRow, 2, c_fromuser);//起草人
		
		if(theApp.mailList[n].type == 0)   m_list.SetItemText(nRow, 3, "审核阶段");
		else if(theApp.mailList[n].type == 1)    m_list.SetItemText(nRow, 3, "已生效");
		else m_list.SetItemText(nRow, 3, "未知");

		if(theApp.mailList[n].isRead == 1)   m_list.SetItemText(nRow, 4, "待审批");
		else if(theApp.mailList[n].isRead == 2)   m_list.SetItemText(nRow, 4, "审批通过");
		else if(theApp.mailList[n].isRead == 3)   m_list.SetItemText(nRow, 4, "审批未通过");
		else if(theApp.mailList[n].isRead == 4)   m_list.SetItemText(nRow, 4, "未读");
		else if(theApp.mailList[n].isRead == 5)   m_list.SetItemText(nRow, 4, "已读");
		else if(theApp.mailList[n].isRead == 6)   m_list.SetItemText(nRow, 4, "审批通过(已读)");
		else if(theApp.mailList[n].isRead == 7)   m_list.SetItemText(nRow, 4, "审批未通过(已读)");
		else m_list.SetItemText(nRow, 4, "未知");
		
	}

	return TRUE;
}



void CMailListDlg::OnNMDblclkMaillistList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
		*pResult = 0;
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	int nId; 
	POSITION pos=m_list.GetFirstSelectedItemPosition();//得到点击的位置  
	if(pos==NULL) {return;}//空 不处理
	nId=(int)m_list.GetNextSelectedItem(pos);//得到行号，通过POSITION转化

	CString s=m_list.GetItemText(nId,0);
	//MessageBox(s);
	int num;
	num = _tstoi( LPCTSTR(s));

	ReadMail readMail;
	readMail.MailNum = num-1;
	readMail.DoModal();
}


void CMailListDlg::OnBnClickedMaillistAll()//显示全部（刷新）
{
	// TODO: 在此添加控件通知处理程序代码
	m_list.DeleteAllItems();

	//插入内容
	for(int n=theApp.mailCount-1;n>=0;n--)
	{
		string tmp;
		CString c_n;
		c_n.Format("%d",n+1);
		int nRow = m_list.InsertItem( 0, c_n);//序号

		m_list.SetItemText(nRow, 1, theApp.mailList[n].title.C_String());//公文标题

		CString c_fromuser;
		map<int, pair<string, int> >::iterator iter;
		iter = theApp.m_mates.find( theApp.mailList[n].fromUser);
		if( iter != theApp.m_mates.end())
		{
			tmp = theApp.m_departmentName[iter->second.second] + " " + iter->second.first;
		}
		c_fromuser.Format("%s", tmp.c_str());
		m_list.SetItemText(nRow, 2, c_fromuser);//起草人
		
		if(theApp.mailList[n].type == 0)   m_list.SetItemText(nRow, 3, "审核阶段");
		else if(theApp.mailList[n].type == 1)    m_list.SetItemText(nRow, 3, "已生效");
		else m_list.SetItemText(nRow, 3, "未知");

		if(theApp.mailList[n].isRead == 1)   m_list.SetItemText(nRow, 4, "待审批");
		else if(theApp.mailList[n].isRead == 2)   m_list.SetItemText(nRow, 4, "审批通过");
		else if(theApp.mailList[n].isRead == 3)   m_list.SetItemText(nRow, 4, "审批未通过");
		else if(theApp.mailList[n].isRead == 4)   m_list.SetItemText(nRow, 4, "未读");
		else if(theApp.mailList[n].isRead == 5)   m_list.SetItemText(nRow, 4, "已读");
		else if(theApp.mailList[n].isRead == 6)   m_list.SetItemText(nRow, 4, "审批通过(已读)");
		else if(theApp.mailList[n].isRead == 7)   m_list.SetItemText(nRow, 4, "审批未通过(已读)");
		else m_list.SetItemText(nRow, 4, "未知");
		
	}
}


void CMailListDlg::OnBnClickedMaillistBtnrecieve()//收件箱
{
	// TODO: 在此添加控件通知处理程序代码
	m_list.DeleteAllItems();

	//插入内容
	for(int n=theApp.mailCount-1;n>=0;n--)
	{
		if(theApp.mailList[n].type ==0) continue;
		string tmp;
		CString c_n;
		c_n.Format("%d",n+1);
		int nRow = m_list.InsertItem( 0, c_n);//序号

		m_list.SetItemText(nRow, 1, theApp.mailList[n].title.C_String());//公文标题

		CString c_fromuser;
		map<int, pair<string, int> >::iterator iter;
		iter = theApp.m_mates.find( theApp.mailList[n].fromUser);
		if( iter != theApp.m_mates.end())
		{
			tmp = theApp.m_departmentName[iter->second.second] + " " + iter->second.first;
		}
		c_fromuser.Format("%s", tmp.c_str());
		m_list.SetItemText(nRow, 2, c_fromuser);//起草人
		
		if(theApp.mailList[n].type == 0)   m_list.SetItemText(nRow, 3, "审核阶段");
		else if(theApp.mailList[n].type == 1)    m_list.SetItemText(nRow, 3, "已生效");
		else m_list.SetItemText(nRow, 3, "未知");

		if(theApp.mailList[n].isRead == 1)   m_list.SetItemText(nRow, 4, "待审批");
		else if(theApp.mailList[n].isRead == 2)   m_list.SetItemText(nRow, 4, "审批通过");
		else if(theApp.mailList[n].isRead == 3)   m_list.SetItemText(nRow, 4, "审批未通过");
		else if(theApp.mailList[n].isRead == 4)   m_list.SetItemText(nRow, 4, "未读");
		else if(theApp.mailList[n].isRead == 5)   m_list.SetItemText(nRow, 4, "已读");
		else if(theApp.mailList[n].isRead == 6)   m_list.SetItemText(nRow, 4, "审批通过(已读)");
		else if(theApp.mailList[n].isRead == 7)   m_list.SetItemText(nRow, 4, "审批未通过(已读)");
		else m_list.SetItemText(nRow, 4, "未知");
		
	}
}


void CMailListDlg::OnBnClickedMaillistBtnsend()//发件箱
{
	// TODO: 在此添加控件通知处理程序代码
	m_list.DeleteAllItems();

	//插入内容
	for(int n=theApp.mailCount-1;n>=0;n--)
	{
		if(theApp.mailList[n].type == 1) continue;
		string tmp;
		CString c_n;
		c_n.Format("%d",n+1);
		int nRow = m_list.InsertItem( 0, c_n);//序号

		m_list.SetItemText(nRow, 1, theApp.mailList[n].title.C_String());//公文标题

		CString c_fromuser;
		map<int, pair<string, int> >::iterator iter;
		iter = theApp.m_mates.find( theApp.mailList[n].fromUser);
		if( iter != theApp.m_mates.end())
		{
			tmp = theApp.m_departmentName[iter->second.second] + " " + iter->second.first;
		}
		c_fromuser.Format("%s", tmp.c_str());
		m_list.SetItemText(nRow, 2, c_fromuser);//起草人
		
		if(theApp.mailList[n].type == 0)   m_list.SetItemText(nRow, 3, "审核阶段");
		else if(theApp.mailList[n].type == 1)    m_list.SetItemText(nRow, 3, "已生效");
		else m_list.SetItemText(nRow, 3, "未知");

		if(theApp.mailList[n].isRead == 1)   m_list.SetItemText(nRow, 4, "待审批");
		else if(theApp.mailList[n].isRead == 2)   m_list.SetItemText(nRow, 4, "审批通过");
		else if(theApp.mailList[n].isRead == 3)   m_list.SetItemText(nRow, 4, "审批未通过");
		else if(theApp.mailList[n].isRead == 4)   m_list.SetItemText(nRow, 4, "未读");
		else if(theApp.mailList[n].isRead == 5)   m_list.SetItemText(nRow, 4, "已读");
		else if(theApp.mailList[n].isRead == 6)   m_list.SetItemText(nRow, 4, "审批通过(已读)");
		else if(theApp.mailList[n].isRead == 7)   m_list.SetItemText(nRow, 4, "审批未通过(已读)");
		else m_list.SetItemText(nRow, 4, "未知");
		
	}
}
