// NoteDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Office Automation Client.h"
#include "NoteDlg.h"
#include "afxdialogex.h"
#include "RakNet/BitStream.h"
#include "PacketType.h"


// CNoteDlg �Ի���

IMPLEMENT_DYNAMIC(CNoteDlg, CDialogEx)

CNoteDlg::CNoteDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CNoteDlg::IDD, pParent)
	, m_save(_T(""))
{

}

CNoteDlg::~CNoteDlg()
{
}

void CNoteDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_NOTES_EDTNOTE, m_save);
}


BEGIN_MESSAGE_MAP(CNoteDlg, CDialogEx)
	ON_BN_CLICKED(IDC_NOTES_BTNSAVE, &CNoteDlg::OnBnClickedNotesBtnsave)
	ON_BN_CLICKED(IDC_NOTES_BTNCANCEL, &CNoteDlg::OnBnClickedNotesBtncancel)
END_MESSAGE_MAP()


// CNoteDlg ��Ϣ�������


BOOL CNoteDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_save = theApp.m_note.c_str();
	UpdateData(false);

	return TRUE;  // return TRUE unless you set the focus to a control

	// �쳣: OCX ����ҳӦ���� FALSE
}


void CNoteDlg::OnBnClickedNotesBtnsave()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	  UpdateData(true);
	  RakNet::BitStream out;
	  out.Write ((RakNet::MessageID)RH_NOTE);
	  out.Write(theApp.m_ID);
	  out.Write(RakNet::RakString(m_save.GetBuffer()));
	  theApp.m_peer->Send(&out, HIGH_PRIORITY, RELIABLE_ORDERED, 0, theApp.m_serverAddress, false);
	  theApp.m_note = m_save ;	   
}


void CNoteDlg::OnBnClickedNotesBtncancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(MessageBox("������˹رհ�ť������δ����Ĳ�������ʧЧ���Ƿ�ȷ����","��ʾ",MB_OKCANCEL)==IDCANCEL)
	{
		return;
	}
	
	CDialogEx::OnCancel();
}