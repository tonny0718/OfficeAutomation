#pragma once


// CSendMailDlg 对话框

class CSendMailDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSendMailDlg)

public:
	CSendMailDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSendMailDlg();

// 对话框数据
	enum { IDD = IDD_SENDMAIL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedSendmailBtncancel();
	virtual BOOL OnInitDialog();
	CString m_title;
	CString m_content;
	CString m_departments;
	afx_msg void OnBnClickedSendmailBtnsend();
	afx_msg void OnBnClickedSendmailBtnchooserecipient();
};


unsigned WINAPI NewMailThread( void* message);