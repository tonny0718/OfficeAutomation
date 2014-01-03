#pragma once
#include "afxcmn.h"


// CMailListDlg �Ի���

class CMailListDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMailListDlg)

public:
	CMailListDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMailListDlg();

// �Ի�������
	enum { IDD = IDD_MAILLIST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedMaillistBtnreturn();
	afx_msg void OnBnClickedMaillistBtnnew();
	virtual BOOL OnInitDialog();
	CListCtrl m_list;

	afx_msg void OnNMDblclkMaillistList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedMaillistAll();
	afx_msg void OnBnClickedMaillistBtnrecieve();
	afx_msg void OnBnClickedMaillistBtnsend();
};
