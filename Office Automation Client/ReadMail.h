#pragma once


// ReadMail �Ի���

class ReadMail : public CDialogEx
{
	DECLARE_DYNAMIC(ReadMail)

public:
	ReadMail(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~ReadMail();

// �Ի�������
	enum { IDD = IDD_READMAIL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOncancle();
	virtual BOOL OnInitDialog();
	int MailNum;
	afx_msg void OnBnClickedMail2();
	afx_msg void OnBnClickedMail3();
};
