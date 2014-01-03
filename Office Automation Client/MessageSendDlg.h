#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include <set>
using std::set;

// CMessageSendDlg �Ի���

class CMessageSendDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMessageSendDlg)

public:
	CMessageSendDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMessageSendDlg();

// �Ի�������
	enum { IDD = IDD_SENDMESSAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
