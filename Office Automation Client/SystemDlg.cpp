// SystemDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Office Automation Client.h"
#include "SystemDlg.h"
#include "afxdialogex.h"
#include "PacketType.h"
#include "RakNet\BitStream.h"


// CSystemDlg 对话框

IMPLEMENT_DYNAMIC(CSystemDlg, CDialogEx)

CSystemDlg::CSystemDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSystemDlg::IDD, pParent)
  , m_strDepartmentName(_T(""))
  , m_strName(_T(""))
  , m_strPassword1(_T(""))
  , m_strPassword2(_T(""))
{

}

CSystemDlg::~CSystemDlg()
{
}

void CSystemDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialogEx::DoDataExchange(pDX);
  DDX_Text(pDX, IDC_SYSTEM_EDTDEPARTMENTNAME, m_strDepartmentName);
  DDX_Text(pDX, IDC_SYSTEM_EDTNAME, m_strName);
  DDX_Text(pDX, IDC_SYSTEM_EDTPASSWORD1, m_strPassword1);
  DDV_MaxChars(pDX, m_strPassword1, 20);
  DDX_Text(pDX, IDC_SYSTEM_EDTPASSWORD2, m_strPassword2);
  DDX_Control(pDX, IDC_SYSTEM_LSTDEPARTMENT, m_lstDepartment);
  DDX_Control(pDX, IDC_SYSTEM_LSTHUMEN, m_lstHuman);
  DDX_Control(pDX, IDC_SYSTEM_CBDEPARTMENT, m_cbDepartment);
  DDX_Control(pDX, IDC_SYSTEM_CBDEPARTMENTMEMBERR, m_cbDepartmentMember);
  DDX_Control(pDX, IDC_SYSTEM_BTNADDHUMAN, m_btnAddHuman);
}


BEGIN_MESSAGE_MAP(CSystemDlg, CDialogEx)
  ON_BN_CLICKED(IDC_SYSTEM_BTNRETURN, &CSystemDlg::OnBnClickedSystemBtnreturn)
  ON_BN_CLICKED(IDC_SYSTEM_BTNADDDEPARTMENT, &CSystemDlg::OnBnClickedSystemBtnadddepartment)
//  ON_REGISTERED_MESSAGE(WM_UPDATE, &CSystemDlg::OnUpdate)
//ON_REGISTERED_MESSAGE(WM_UPDATE, &CSystemDlg::OnUpdate)
ON_MESSAGE(WM_UPDATE, &CSystemDlg::OnUpdate)
ON_BN_CLICKED(IDC_SYSTEM_BTNCANCELCHOOSE, &CSystemDlg::OnBnClickedSystemBtncancelchoose)
ON_LBN_SELCHANGE(IDC_SYSTEM_LSTDEPARTMENT, &CSystemDlg::OnLbnSelchangeSystemLstdepartment)
ON_BN_CLICKED(IDC_SYSTEM_BTNSETMASTER, &CSystemDlg::OnBnClickedSystemBtnsetmaster)
ON_BN_CLICKED(IDC_SYSTEM_BTNDELETEDEPARTMENT, &CSystemDlg::OnBnClickedSystemBtndeletedepartment)
ON_LBN_SELCHANGE(IDC_SYSTEM_LSTHUMEN, &CSystemDlg::OnLbnSelchangeSystemLsthumen)
ON_BN_CLICKED(IDC_SYSTEM_BTNADDHUMAN, &CSystemDlg::OnBnClickedSystemBtnaddhuman)
ON_BN_CLICKED(IDC_SYSTEM_BTNDELETEHUMAN, &CSystemDlg::OnBnClickedSystemBtndeletehuman)
END_MESSAGE_MAP()


// CSystemDlg 消息处理程序

BOOL CSystemDlg::OnInitDialog()
{
  CDialogEx::OnInitDialog();

  ResetList();

  return TRUE;  // return TRUE unless you set the focus to a control

  // 异常: OCX 属性页应返回 FALSE
}


void CSystemDlg::OnBnClickedSystemBtnreturn()
{
  // TODO: 在此添加控件通知处理程序代码
  CDialogEx::OnCancel();
}


void CSystemDlg::OnBnClickedSystemBtnadddepartment()
{
  // TODO: 在此添加控件通知处理程序代码
  UpdateData( true);
  for( map<int, string>::iterator iter = theApp.m_departmentName.begin(); 
    iter != theApp.m_departmentName.end(); ++iter)
  {
    if( strcmp( iter->second.c_str(), m_strDepartmentName.GetBuffer()) == 0)
    {
      MessageBox("部门名称重复！", "错误", MB_OK);
      return;
    }
  }
  RakNet::BitStream bsOut;
  bsOut.Write( (RakNet::MessageID)RH_SET_DEPARTMENT);
  bsOut.Write( SDO_NEW);
  bsOut.Write( RakNet::RakString(m_strDepartmentName.GetBuffer()));
  theApp.m_peer->Send( &bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, theApp.m_serverAddress, false);
}

void CSystemDlg::ResetList()
{
  m_lstDepartment.ResetContent();
  m_cbDepartment.ResetContent();
  m_lstHuman.ResetContent();
  m_cbDepartmentMember.ResetContent();

  for( map<int, string>::iterator iter = theApp.m_departmentName.begin();
    iter != theApp.m_departmentName.end(); ++iter)
  {
    m_lstDepartment.AddString( iter->second.c_str());
    m_cbDepartment.AddString( iter->second.c_str());
  }

  for( map<int, pair<string, int> >::iterator iter = theApp.m_mates.begin();
    iter != theApp.m_mates.end(); ++iter)
  {
    m_lstHuman.AddString( (theApp.m_departmentName[iter->second.second] + " " + iter->second.first).c_str());
  }

  m_btnAddHuman.SetWindowTextA("添加");
}

afx_msg LRESULT CSystemDlg::OnUpdate(WPARAM wParam, LPARAM lParam)
{
  ResetList();
  return 0;
}



void CSystemDlg::OnBnClickedSystemBtncancelchoose()
{
  // TODO: 在此添加控件通知处理程序代码
  ResetList();
}


void CSystemDlg::OnLbnSelchangeSystemLstdepartment()
{
  // TODO: 在此添加控件通知处理程序代码
  m_cbDepartmentMember.ResetContent();
  int row = m_lstDepartment.GetCurSel();
  CString departmentName;
  m_lstDepartment.GetText( row, departmentName);
  for( map<int, string>::iterator iter = theApp.m_departmentName.begin();
    iter != theApp.m_departmentName.end(); ++iter)
  {
    if( strcmp( iter->second.c_str(), departmentName.GetBuffer()) == 0)
    {
      for( map<int, pair<string, int> >::iterator iter2 = theApp.m_mates.begin();
        iter2 != theApp.m_mates.end(); ++iter2)
      {
        if( iter2->second.second == iter->first)
        {
          m_cbDepartmentMember.AddString( iter2->second.first.c_str());
        }
      }
      int master = theApp.m_departmentMaster[iter->first];
      if( master != 0)
      {
        string masterName = theApp.m_mates[ master ].first;
        int index = m_cbDepartmentMember.FindString( 0, masterName.c_str());
        m_cbDepartmentMember.SetCurSel(index);
      }
      break;
    }
  }
}


void CSystemDlg::OnBnClickedSystemBtnsetmaster()
{
  // TODO: 在此添加控件通知处理程序代码
  RakNet::BitStream bsOut;
  bsOut.Write( (RakNet::MessageID)RH_SET_DEPARTMENT);
  bsOut.Write( SDO_SET_MASTER);

  int row = m_lstDepartment.GetCurSel();
  CString departmentName;
  m_lstDepartment.GetText( row, departmentName);
  CString masterName;
  m_cbDepartmentMember.GetWindowTextA(masterName);
  for( map<int, string>::iterator iter = theApp.m_departmentName.begin();
    iter != theApp.m_departmentName.end(); ++iter)
  {
    if( strcmp( iter->second.c_str(), departmentName.GetBuffer()) == 0)
    {
      bsOut.Write( iter->first);

      for( map<int, pair<string, int> >::iterator iter2 = theApp.m_mates.begin();
        iter2 != theApp.m_mates.end(); ++iter2)
      {
        if( iter2->second.second == iter->first && strcmp(iter2->second.first.c_str(), masterName.GetBuffer()) == 0)
        {
          bsOut.Write( iter2->first);
        }
      }
      break;
    }
  }
  theApp.m_peer->Send( &bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, theApp.m_serverAddress, false);
}


void CSystemDlg::OnBnClickedSystemBtndeletedepartment()
{
  // TODO: 在此添加控件通知处理程序代码
  int nIndex = m_lstDepartment.GetCurSel();
  if( nIndex == LB_ERR)
    return;

  if( MessageBox("删除部门操作会导致在该部门下所有员工都会被删除，是否确定?", "提示", MB_OKCANCEL) == IDCANCEL)
    return;

  CString departmentName;
  m_lstDepartment.GetText( nIndex, departmentName);
  for( map<int, string>::iterator iter = theApp.m_departmentName.begin();
    iter != theApp.m_departmentName.end(); ++iter)
  {
    if( strcmp(departmentName.GetBuffer(), iter->second.c_str()) == 0)
    {
      RakNet::BitStream bsOut;
      bsOut.Write( (RakNet::MessageID)RH_SET_DEPARTMENT);
      bsOut.Write( SDO_DELETE);
      bsOut.Write( iter->first);
      theApp.m_peer->Send( &bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, theApp.m_serverAddress, false);
      break;
    }
  }
}


void CSystemDlg::OnLbnSelchangeSystemLsthumen()
{
  // TODO: 在此添加控件通知处理程序代码
  int row = m_lstHuman.GetCurSel();
  CString text;
  m_lstHuman.GetText( row, text);
  char name[256],department[256];
  sscanf( text.GetBuffer(), "%s %s", department, name);

  for( map<int, pair<string, int> >::iterator iter = theApp.m_mates.begin();
    iter != theApp.m_mates.end(); ++iter)
  {
    if( strcmp(iter->second.first.c_str(), name ) == 0)
    {
      m_strName = name;
      int index = m_cbDepartment.FindString(0, department );
      m_cbDepartment.SetCurSel(index);
      m_btnAddHuman.SetWindowTextA("保存");
      m_strPassword1="";
      m_strPassword2="";
    }
  }

  UpdateData(false);
}


void CSystemDlg::OnBnClickedSystemBtnaddhuman()
{
  // TODO: 在此添加控件通知处理程序代码
  UpdateData(true);

  if( m_strName.IsEmpty() )
  {
    MessageBox( "用户名不能为空", "错误", MB_OK);
    return;
  }

  CString btnString;
  m_btnAddHuman.GetWindowTextA( btnString);
  if( strcmp(btnString.GetBuffer(), "添加") == 0)
  {
    int index = m_cbDepartment.GetCurSel();
    if( index == LB_ERR)
    {
      MessageBox("未选择部门", "错误", MB_OK);
      return;
    }
    if( m_strPassword1.IsEmpty())
    {
      MessageBox("密码不能为空", "错误", MB_OK);
      return;
    }
    if( strcmp(m_strPassword1.GetBuffer(), m_strPassword2.GetBuffer()) != 0)
    {
      MessageBox("两次密码输入不一致", "错误", MB_OK);
      return;
    }
    CString departmentName;
    m_cbDepartment.GetWindowTextA(departmentName);
    for( map<int, string>::iterator iter = theApp.m_departmentName.begin();
      iter != theApp.m_departmentName.end(); ++iter)
    {
      if( strcmp(iter->second.c_str(), departmentName.GetBuffer()) == 0)
      {
        RakNet::BitStream bsOut;
        bsOut.Write( (RakNet::MessageID)RH_SET_HUMAN);
        bsOut.Write( SHO_NEW);
        bsOut.Write( RakNet::RakString(m_strName.GetBuffer()));
        bsOut.Write( iter->first);
        bsOut.Write( RakNet::RakString(m_strPassword1.GetBuffer()));
        theApp.m_peer->Send( &bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, theApp.m_serverAddress, false);
        return;
      }
    }
  }
  else
  {
    if( strcmp(m_strPassword1.GetBuffer(), m_strPassword2.GetBuffer()) != 0)
    {
      MessageBox("两次密码输入不一致", "错误", MB_OK);
      return;
    }
    int row = m_lstHuman.GetCurSel();
    CString text;
    m_lstHuman.GetText( row, text);
    char name[256],department[256];
    sscanf( text.GetBuffer(), "%s %s", department, name);
    int ID = -1;
    for( map<int, pair<string, int> >::iterator iter = theApp.m_mates.begin();
      iter != theApp.m_mates.end(); ++iter)
    {
      if( strcmp( iter->second.first.c_str(), name) == 0)
      {
        ID = iter->first;
        break;
      }
    }
    if( ID == -1)
      return;
    int index = m_cbDepartment.GetCurSel();
    if( index == LB_ERR)
    {
      MessageBox("未选择部门", "错误", MB_OK);
      return;
    }
    CString departmentName;
    m_cbDepartment.GetWindowTextA(departmentName);
    for( map<int, string>::iterator iter = theApp.m_departmentName.begin();
      iter != theApp.m_departmentName.end(); ++iter)
    {
      if( strcmp(iter->second.c_str(), departmentName.GetBuffer()) == 0)
      {
        RakNet::BitStream bsOut;
        bsOut.Write( (RakNet::MessageID)RH_SET_HUMAN);
        if( m_strPassword1.IsEmpty())
          bsOut.Write( SHO_CHANGE_NO_PASSWORD);
        else
          bsOut.Write( SHO_CHANGE);
        bsOut.Write( ID);
        bsOut.Write( RakNet::RakString(m_strName.GetBuffer()));
        bsOut.Write( iter->first);
        if( m_strPassword1.IsEmpty() == false )
          bsOut.Write( RakNet::RakString(m_strPassword1.GetBuffer()));
        theApp.m_peer->Send( &bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, theApp.m_serverAddress, false);
        return;
      }
    }
  }
}


void CSystemDlg::OnBnClickedSystemBtndeletehuman()
{
  // TODO: 在此添加控件通知处理程序代码
  int row = m_lstHuman.GetCurSel();
  CString text;
  char department[256],name[256];
  m_lstHuman.GetText( row, text);
  sscanf( text.GetBuffer(), "%s %s", department, name);
  int ID = -1;
  for( map<int, pair<string, int> >::iterator iter = theApp.m_mates.begin();
    iter != theApp.m_mates.end(); ++iter)
  {
    if( strcmp( iter->second.first.c_str(), name) == 0)
    {
      ID = iter->first;
      break;
    }
  }
  if( ID == -1)
    return;

  for( map<int,int>::iterator iter = theApp.m_departmentMaster.begin();
    iter != theApp.m_departmentMaster.end(); ++iter)
  {
    if( iter->second == ID)
    {
      MessageBox("该员工现在是其所在部门的组长，无法删除.", "错误", MB_OK);
      return;
    }
  }

  RakNet::BitStream bsOut;
  bsOut.Write( (RakNet::MessageID)RH_SET_HUMAN);
  bsOut.Write( SHO_DELETE);
  bsOut.Write( ID);
  theApp.m_peer->Send( &bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, theApp.m_serverAddress, false);
}
