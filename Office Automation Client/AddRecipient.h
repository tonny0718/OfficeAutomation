#pragma once
#include "afxwin.h"
#include <set>
using std::set;


// CAddRecipient 对话框

class CAddRecipient : public CDialogEx
{
	DECLARE_DYNAMIC(CAddRecipient)

public:
	CAddRecipient(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAddRecipient();

	virtual BOOL OnInitDialog();

// 对话框数据
	enum { IDD = IDD_ADDRECIPIENT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedAddrecipientBtncancel();
	afx_msg void OnBnClickedAddrecipientBtnok();
	afx_msg void OnBnClickedAddrecipientBtnadd();
	afx_msg void OnBnClickedAddrecipientBtndelete();

	set<int> m_depIdAddTo;
	CListBox m_lstAddTo;
	CListBox m_lstDep;

	
};
