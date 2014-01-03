// NewMessageDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Office Automation Client.h"
#include "NewMessageDlg.h"
#include "afxdialogex.h"


// NewMessageDlg �Ի���

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


// NewMessageDlg ��Ϣ�������
BOOL NewMessageDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CRect rectWorkArea;  
	SystemParametersInfo(SPI_GETWORKAREA,0,&rectWorkArea,SPIF_SENDCHANGE);     
  
	// ��öԻ����С  
	CRect rectDlg;  
	GetWindowRect(&rectDlg);  
	int nW = rectDlg.Width();  
	int nH = rectDlg.Height();  
  
	// ���������õ����½�  
	::SetWindowPos(GetSafeHwnd(),HWND_BOTTOM,rectWorkArea.right-nW-20,rectWorkArea.bottom-nH,nW,nH,SWP_SHOWWINDOW|SWP_NOZORDER);
  
	// ������ʾ����Ч��  
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