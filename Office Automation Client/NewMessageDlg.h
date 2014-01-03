#pragma once


// NewMessageDlg 对话框

class NewMessageDlg : public CDialogEx
{
	DECLARE_DYNAMIC(NewMessageDlg)

public:
	NewMessageDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~NewMessageDlg();
	BOOL OnInitDialog();
	CString messageText;

// 对话框数据
	enum { IDD = IDD_NEWMESSAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};

unsigned WINAPI NewMessageThread( void* message);