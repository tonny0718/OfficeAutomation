#pragma once
#include "afxwin.h"


// CSystemDlg 对话框

#define WM_UPDATE (WM_USER +1)

class CSystemDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSystemDlg)

public:
	CSystemDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSystemDlg();

// 对话框数据
	enum { IDD = IDD_SYSTEM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
  CString m_strDepartmentName;
  CString m_strName;
  CString m_strPassword1;
  CString m_strPassword2;
  CListBox m_lstDepartment;
  CListBox m_lstHuman;
  CComboBox m_cbDepartment;
  CComboBox m_cbDepartmentMember;
  virtual BOOL OnInitDialog();
  afx_msg void OnBnClickedSystemBtnreturn();
  afx_msg void OnBnClickedSystemBtnadddepartment();
protected:
  afx_msg LRESULT OnUpdate(WPARAM wParam, LPARAM lParam);

public:
  void ResetList();
  afx_msg void OnBnClickedSystemBtncancelchoose();
  afx_msg void OnLbnSelchangeSystemLstdepartment();
  afx_msg void OnBnClickedSystemBtnsetmaster();
  afx_msg void OnBnClickedSystemBtndeletedepartment();
  afx_msg void OnLbnSelchangeSystemLsthumen();
  CButton m_btnAddHuman;
  afx_msg void OnBnClickedSystemBtnaddhuman();
  afx_msg void OnBnClickedSystemBtndeletehuman();
};
