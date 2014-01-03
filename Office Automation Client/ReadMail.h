#pragma once


// ReadMail 对话框

class ReadMail : public CDialogEx
{
	DECLARE_DYNAMIC(ReadMail)

public:
	ReadMail(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~ReadMail();

// 对话框数据
	enum { IDD = IDD_READMAIL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOncancle();
	virtual BOOL OnInitDialog();
	int MailNum;
	afx_msg void OnBnClickedMail2();
	afx_msg void OnBnClickedMail3();
};
