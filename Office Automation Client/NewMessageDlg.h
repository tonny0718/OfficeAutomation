#pragma once


// NewMessageDlg �Ի���

class NewMessageDlg : public CDialogEx
{
	DECLARE_DYNAMIC(NewMessageDlg)

public:
	NewMessageDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~NewMessageDlg();
	BOOL OnInitDialog();
	CString messageText;

// �Ի�������
	enum { IDD = IDD_NEWMESSAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};

unsigned WINAPI NewMessageThread( void* message);