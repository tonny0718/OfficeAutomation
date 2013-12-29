#pragma once
#include "atlcomtime.h"
#include "afxwin.h"
#include <set>
using std::set;

// CAddWorkDlg �Ի���

class CAddWorkDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAddWorkDlg)

public:
	CAddWorkDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAddWorkDlg();

// �Ի�������
	enum { IDD = IDD_ADDWORK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnBnClickedAddworkBtncancel();
  virtual BOOL OnInitDialog();
//  int m_cbHour;
//  int m_cbMinute;
  COleDateTime m_date;
  CString m_intent;
  BOOL m_forSelf;
  CButton m_isForOther;
  CButton m_isForSelf;
  CListBox m_lstAddTo;
  CListBox m_lstMates;
  set<int> m_idAddTo;
  afx_msg void OnBnClickedAddworkBtnadd();
  afx_msg void OnBnClickedAddworkLstdelete();
  afx_msg void OnBnClickedAddworkBtnok();
  CString m_cbHour;
  CString m_cbMinute;
};
