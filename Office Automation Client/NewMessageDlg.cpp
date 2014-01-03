// NewMessageDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Office Automation Client.h"
#include "NewMessageDlg.h"
#include "afxdialogex.h"


// NewMessageDlg 对话框

IMPLEMENT_DYNAMIC(NewMessageDlg, CDialogEx)

NewMessageDlg::NewMessageDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(NewMessageDlg::IDD, pParent)
{

}

NewMessageDlg::~NewMessageDlg()
{
}

void NewMessageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(NewMessageDlg, CDialogEx)
END_MESSAGE_MAP()


// NewMessageDlg 消息处理程序
BOOL NewMessageDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CRect rectWorkArea;  
	SystemParametersInfo(SPI_GETWORKAREA,0,&rectWorkArea,SPIF_SENDCHANGE);     
  
	// 获得对话框大小  
	CRect rectDlg;  
	GetWindowRect(&rectDlg);  
	int nW = rectDlg.Width();  
	int nH = rectDlg.Height();  
  
	// 将窗口设置到右下角  
	::SetWindowPos(GetSafeHwnd(),HWND_BOTTOM,rectWorkArea.right-nW-20,rectWorkArea.bottom-nH,nW,nH,SWP_SHOWWINDOW|SWP_NOZORDER);
  
	// 动画显示弹窗效果  
	//AnimateWindow(GetSafeHwnd(),1000,AW_HIDE|AW_BLEND);

	CStatic *pStatic=(CStatic*)GetDlgItem(IDC_MESSAGE);
	pStatic->SetWindowText(messageText);

	return TRUE;
}


unsigned WINAPI NewMessageThread( void* message)
{
  char* msg = (char*)message;
  NewMessageDlg newMessage;
	newMessage.messageText = msg;
	newMessage.DoModal();
  delete[] msg;
  return 0;
}