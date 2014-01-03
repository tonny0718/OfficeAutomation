#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include <set>
using std::set;

// CMessageSendDlg 对话框

class CMessageSendDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMessageSendDlg)

public:
	CMessageSendDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMessageSendDlg();

// 对话框数据
	enum { IDD = IDD_SENDMESSAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_edit;
    set<int> m_idAddTo;

	afx_msg void OnBnClickedSendmessageBtncancel();
	afx_msg void OnBnClickedSendmessageBtnadd();
	afx_msg void OnBnClickedSendmessageBtndelete();
	afx_msg void OnBnClickedSendmessageBtnok();

	virtual BOOL OnInitDialog();
	CListBox m_lstAddTo;
	CListBox m_lstMates;
};
