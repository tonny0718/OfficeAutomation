// AddWorkDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Office Automation Client.h"
#include "AddWorkDlg.h"
#include "afxdialogex.h"
#include "PacketType.h"


// CAddWorkDlg �Ի���

IMPLEMENT_DYNAMIC(CAddWorkDlg, CDialogEx)

CAddWorkDlg::CAddWorkDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAddWorkDlg::IDD, pParent)
  , m_date(COleDateTime::GetCurrentTime())
  , m_intent(_T(""))
  , m_forSelf(FALSE)
  , m_cbHour(_T(""))
  , m_cbMinute(_T(""))
{

}

CAddWorkDlg::~CAddWorkDlg()
{
}

void CAddWorkDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialogEx::DoDataExchange(pDX);
  //  DDX_CBIndex(pDX, IDC_ADDWORK_CBHOUR, m_cbHour);
  //  DDV_MinMaxInt(pDX, m_cbHour, 0, 23);
  //  DDX_CBIndex(pDX, IDC_ADDWORK_CBMINUTE, m_cbMinute);
  //  DDV_MinMaxInt(pDX, m_cbMinute, 0, 59);
  DDX_DateTimeCtrl(pDX, IDC_ADDWORK_DATE, m_date);
  DDX_Text(pDX, IDC_ADDWORK_EDTINTENT, m_intent);
  DDX_Check(pDX, IDC_ADDWORK_FORSELF, m_forSelf);
  DDX_Control(pDX, IDC_ADDWORK_FOROTHER, m_isForOther);
  DDX_Control(pDX, IDC_ADDWORK_FORSELF, m_isForSelf);
  DDX_Control(pDX, IDC_ADDWORK_LSTADDTO, m_lstAddTo);
  DDX_Control(pDX, IDC_ADDWORK_LSTMATES, m_lstMates);
  DDX_CBString(pDX, IDC_ADDWORK_CBHOUR, m_cbHour);
  DDX_CBString(pDX, IDC_ADDWORK_CBMINUTE, m_cbMinute);
}


BEGIN_MESSAGE_MAP(CAddWorkDlg, CDialogEx)
  ON_BN_CLICKED(IDC_ADDWORK_BTNCANCEL, &CAddWorkDlg::OnBnClickedAddworkBtncancel)
  ON_BN_CLICKED(IDC_ADDWORK_BTNADD, &CAddWorkDlg::OnBnClickedAddworkBtnadd)
  ON_BN_CLICKED(IDC_ADDWORK_LSTDELETE, &CAddWorkDlg::OnBnClickedAddworkLstdelete)
  ON_BN_CLICKED(IDC_ADDWORK_BTNOK, &CAddWorkDlg::OnBnClickedAddworkBtnok)
END_MESSAGE_MAP()


// CAddWorkDlg ��Ϣ�������


void CAddWorkDlg::OnBnClickedAddworkBtncancel()
{
  // TODO: �ڴ���ӿؼ�֪ͨ����������
  CDialogEx::OnCancel();
}


BOOL CAddWorkDlg::OnInitDialog()
{
  CDialogEx::OnInitDialog();

  for( map<int, pair<string, int> >::iterator iter = theApp.m_mates.begin();
    iter != theApp.m_mates.end(); iter++)
  {
    string name = theApp.m_departmentName[iter->second.second] + " " + iter->second.first;
    m_lstMates.AddString( name.c_str());
  }

  m_isForSelf.SetCheck(1);

  return TRUE;
  // �쳣: OCX ����ҳӦ���� FALSE
}


void CAddWorkDlg::OnBnClickedAddworkBtnadd()
{
  // TODO: �ڴ���ӿؼ�֪ͨ����������
  int index = m_lstMates.GetCurSel();
  if( index == LB_ERR)
    return;

  CString mate;
  m_lstMates.GetText( index, mate);
  char department[256], name[256];
  sscanf( mate.GetBuffer(), "%s %s", department, name);
  for( map<int, pair< string, int> >::iterator iter = theApp.m_mates.begin();
    iter != theApp.m_mates.end(); ++iter)
  {
    if( iter->second.first != name)
      continue;
    if( theApp.m_departmentName[ iter->second.second] != department)
      continue;
    m_lstAddTo.AddString( mate);
    m_lstMates.DeleteString( index);
    m_idAddTo.insert( iter->first);
    break;
  }
}


void CAddWorkDlg::OnBnClickedAddworkLstdelete()
{
  // TODO: �ڴ���ӿؼ�֪ͨ����������
  int index = m_lstAddTo.GetCurSel();
  if( index == LB_ERR)
    return;

  CString mate;
  m_lstAddTo.GetText( index, mate);
  char department[256], name[256];
  sscanf( mate.GetBuffer(), "%s %s", department, name);
  for( map<int, pair< string, int> >::iterator iter = theApp.m_mates.begin();
    iter != theApp.m_mates.end(); ++iter)
  {
    if( iter->second.first != name)
      continue;
    if( theApp.m_departmentName[ iter->second.second] != department)
      continue;
    m_lstMates.AddString( mate);
    m_lstAddTo.DeleteString( index);
    m_idAddTo.erase( iter->first);
    break;
  }
}


void CAddWorkDlg::OnBnClickedAddworkBtnok()
{
  // TODO: �ڴ���ӿؼ�֪ͨ����������
  UpdateData(true);
  set<int> realAddTo;
  if( m_isForOther.GetCheck() == 0 && m_isForSelf.GetCheck() == 0)
  {
    MessageBox("����δѡ�����κ�����ӹ������š�", "����", MB_OK);
    return;
  }
  if( m_isForOther.GetCheck() != 0)
  {
    if( theApp.m_priority == 2 && m_idAddTo.empty() == false)//��Ա
    {
      MessageBox("����Ȩ����������ӹ�������", "����", MB_OK);
      return;
    }
    if( theApp.m_priority == 1)
    {
      for( set<int>::iterator iter = m_idAddTo.begin(); iter != m_idAddTo.end(); iter++)
      {
        if( theApp.m_mates[*iter].second != theApp.m_department)
        {
          MessageBox("����Ȩ���������ŵ���Ա��ӹ�������", "����", MB_OK);
          return;
        }
      }
    }
    realAddTo = m_idAddTo;
  }
  if( m_isForSelf.GetCheck() != 0)
  {
    realAddTo.insert( theApp.m_ID);
  }

  COleDateTime now = COleDateTime::GetCurrentTime();
  int hour = atoi(m_cbHour.GetBuffer());
  int minute = atoi( m_cbMinute.GetBuffer());
  COleDateTime workTime( m_date.GetYear(), m_date.GetMonth(), m_date.GetDay(), hour, minute, 0);
  COleDateTimeSpan timeSpan = workTime - now;
  if( timeSpan.GetTotalMinutes() <= 3)
  {
    MessageBox("������ָ����ʱ���Ѳ���3���ӣ��޷�����µĹ�������.", "����", MB_OK);
    return;
  }

  for( set<int>::iterator iter = realAddTo.begin(); iter != realAddTo.end(); iter++)
  {
    RakNet::BitStream bsOut;
    bsOut.Write( (RakNet::MessageID)RH_SCHEDULE);
    bsOut.Write( SO_NEW);
    bsOut.Write( theApp.m_ID);
    bsOut.Write( *iter);
    RakNet::RakString time;
    time.Set( "%d-%d-%d %s:%s:00", m_date.GetYear(), m_date.GetMonth(), m_date.GetDay(), m_cbHour.GetBuffer(), m_cbMinute.GetBuffer());
    bsOut.Write(time);
    RakNet::RakString intent(m_intent.GetBuffer());
    bsOut.Write(intent);
    theApp.m_peer->Send( &bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, theApp.m_serverAddress, false);
  }
  CDialogEx::OnOK();
}
