#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CMessageSendDlg �Ի���

class CMessageSendDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMessageSendDlg)

public:
	CMessageSendDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMessageSendDlg();
	virtual BOOL OnInitDialog();

// �Ի�������
	enum { IDD = IDD_SENDMESSAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CTreeCtrl m_tree;
	CString m_edit;
	CListBox m_listBox;
	CImageList m_imageList;

	struct nameList{int id;char name[20];int department;}nlist[105];
	struct depList{int id;char name[20];int mas_ID;}dlist[100];
	struct sentArr{int id;CString name;}sArr[100];

	int acount;
	int ncount;
	int dcount;

	afx_msg void OnBnClickedSendmessageBtncancel();
	afx_msg void OnBnClickedSendmessageBtnadd();
	afx_msg void OnBnClickedSendmessageBtndelete();
	afx_msg void OnBnClickedSendmessageBtnok();
	afx_msg void OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult);
};
