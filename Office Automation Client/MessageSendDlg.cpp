// MessageSendDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Office Automation Client.h"
#include "MessageSendDlg.h"
#include "afxdialogex.h"
#include "PacketType.h"

// CMessageSendDlg 对话框

IMPLEMENT_DYNAMIC(CMessageSendDlg, CDialogEx)

CMessageSendDlg::CMessageSendDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMessageSendDlg::IDD, pParent)
	, m_edit(_T(""))
{

}

CMessageSendDlg::~CMessageSendDlg()
{
}

void CMessageSendDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_tree);
	DDX_Text(pDX, IDC_SENDMESSAGE_EDTCONTENT, m_edit);
	DDX_Control(pDX, IDC_SENDMESSAGE_LSTSENDTO, m_listBox);
}


BEGIN_MESSAGE_MAP(CMessageSendDlg, CDialogEx)
	ON_BN_CLICKED(IDC_SENDMESSAGE_BTNCANCEL, &CMessageSendDlg::OnBnClickedSendmessageBtncancel)
	ON_BN_CLICKED(IDC_SENDMESSAGE_BTNADD, &CMessageSendDlg::OnBnClickedSendmessageBtnadd)
	ON_BN_CLICKED(IDC_SENDMESSAGE_BTNDELETE, &CMessageSendDlg::OnBnClickedSendmessageBtndelete)
	ON_BN_CLICKED(IDC_SENDMESSAGE_BTNOK, &CMessageSendDlg::OnBnClickedSendmessageBtnok)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &CMessageSendDlg::OnTvnSelchangedTree1)
END_MESSAGE_MAP()


// CMessageSendDlg 消息处理程序
BOOL CMessageSendDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	//获取数据。
	RakNet::BitStream out;
	out.Write( (RakNet::MessageID)RH_GETNAMELIST);
	out.Write( theApp.m_ID);//权限相关标识
	theApp.m_peer->Send( &out, HIGH_PRIORITY, RELIABLE_ORDERED, 0, theApp.m_serverAddress, false);

	RakNet::Packet* packet;
	int max = 100;
	for( packet = theApp.m_peer->Receive(); !packet; packet = theApp.m_peer->Receive())
	{
		Sleep(100);
		if(max--){
			MessageBox("出错","提示",MB_OK);
			//exit(0);
			CDialog::OnCancel();
			return FALSE;
		}
	}
		
	//返回
	if( packet->data[0] == ID_DISCONNECTION_NOTIFICATION ||
	packet->data[0] == ID_CONNECTION_LOST)
	{
		MessageBox("与服务器失去连接。", "错误", MB_OK);
		exit(0);
	}
	if( packet->data[0] == RH_GETNAMELIST)
	{
		RakNet::BitStream bsIn( packet->data, packet->length, false);
		bsIn.IgnoreBytes( sizeof(RakNet::MessageID));
		int result;
		bsIn.Read( result);
		if( result == LR_SUCCESS)
		{
			bsIn.Read( ncount);
			for(int n=0;n<ncount;n++)
			{
				bsIn.Read( nlist[n].id);
				bsIn.Read( nlist[n].name);
				bsIn.Read( nlist[n].department);
			}

			bsIn.Read( dcount);
			for(int n=0;n<dcount;n++)
			{
				bsIn.Read( dlist[n].id);
				bsIn.Read( dlist[n].name);
				bsIn.Read( dlist[n].mas_ID);
			}

			theApp.m_peer->DeallocatePacket( packet);
			_beginthreadex( NULL, 0, COfficeAutomationClientApp::NetRecieveThread, &theApp, 0, NULL);
			//处理方法

		}
		else if( result == LR_ERROR)
		{
		  MessageBox("获取错误。", "错误", MB_OK);
		  theApp.m_peer->DeallocatePacket( packet);
		}
		else
		{
		  MessageBox("服务器出错。", "错误", MB_OK);
		  theApp.m_peer->DeallocatePacket( packet);
		}
	}

	HICON hIcon[3];//图标句柄数组
	HTREEITEM hRoot; //根节点
	HTREEITEM hCataItem; //表示任一分类节点
	HTREEITEM hArtItem;  //表示任一成员节点
	// 加载三个图标，并将它们的句柄保存到数组 
	hIcon[0] = AfxGetApp()->LoadIcon(IDI_ICON1);
	hIcon[1] = AfxGetApp()->LoadIcon(IDI_ICON1);
	hIcon[2] = AfxGetApp()->LoadIcon(IDI_ICON1);
	//创建图像序列CImageList对象
	m_imageList.Create(16,16,ILC_COLOR32,3,3);
	//添加图标到图像序列
	for(int n=0;n<3;n++)
	{
		m_imageList.Add(hIcon[n]);
	}
	m_tree.SetImageList(&m_imageList,TVSIL_NORMAL);//为树形控件设置图像序列

	hRoot = m_tree.InsertItem("OA",0,0);//插入根节点
	


	for(int n=0;n<dcount;n++)
	{
		hCataItem = m_tree.InsertItem(dlist[n].name,1,1,hRoot,TVI_LAST);//插入子节点
		m_tree.SetItemData(hCataItem,0);//附加编号数据

		for(int m=0;m<ncount;m++)
		{
			if(nlist[m].department == dlist[n].id)
			{
				hArtItem = m_tree.InsertItem(nlist[m].name,2,2,hCataItem,TVI_LAST);
				m_tree.SetItemData(hArtItem,nlist[m].id);
			}
		}
	}
	m_listBox.ModifyStyle(LVS_SORTDESCENDING | LVS_SORTASCENDING,0);
	m_listBox.SetCurSel(0);
	acount = 0;

	return TRUE;  
}

void CMessageSendDlg::OnBnClickedSendmessageBtncancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialog::OnCancel();
}


void CMessageSendDlg::OnBnClickedSendmessageBtnadd()
{
	// TODO: 在此添加控件通知处理程序代码
	if(sArr[0].id == 0)return;
	for(int n=1;n<acount+1;n++)
	{
		if(sArr[n].id == sArr[0].id)
			return;
	}
	acount++;
	sArr[acount] = sArr[0];
	m_listBox.AddString(sArr[acount].name);
}


void CMessageSendDlg::OnBnClickedSendmessageBtndelete()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_listBox.GetCurSel()>=0){
		int n = m_listBox.GetCurSel();
		m_listBox.DeleteString(n);
		for(int i=n+1;i<acount-2;i++)
		{
			sArr[i] = sArr[i+1];
		}
		acount--;
	}
	sArr[0].id = 0;
}


void CMessageSendDlg::OnBnClickedSendmessageBtnok()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if(acount<1)
	{
		MessageBox("请选择联系人","提示",MB_OK);
		return;
	}
	char str[1000];
	sprintf(str,"确认发送消息'%s'给%d人",m_edit,acount);

	if(MessageBox(str,"确认发送",MB_OKCANCEL)!=TRUE)return;
	//发送数据。
	//格式 fromID,message,toIDnumber,touser;
	RakNet::BitStream out;
	out.Write( (RakNet::MessageID)RH_SENDMESSAGE);
	out.Write( theApp.m_ID);
	RakNet::RakString message = m_edit;
	out.Write( message);
	out.Write( acount);
	for(int n=0;n<acount;n++)
	{
		out.Write(sArr[n+1].id);
	}
	theApp.m_peer->Send( &out, HIGH_PRIORITY, RELIABLE_ORDERED, 0, theApp.m_serverAddress, false);

	RakNet::Packet* packet;
	for( packet = theApp.m_peer->Receive(); !packet; packet = theApp.m_peer->Receive())
		Sleep(100);
	//返回
	if( packet->data[0] == ID_DISCONNECTION_NOTIFICATION ||
	packet->data[0] == ID_CONNECTION_LOST)
	{
		MessageBox("与服务器失去连接。", "错误", MB_OK);
		exit(0);
	}
	if( packet->data[0] == RH_SENDMESSAGE)
	{
		RakNet::BitStream bsIn( packet->data, packet->length, false);
		bsIn.IgnoreBytes( sizeof(RakNet::MessageID));
		int result;
		bsIn.Read( result);
		if( result == LR_SUCCESS)
		{
			MessageBox("发送成功。", "提示", MB_OK);
			theApp.m_peer->DeallocatePacket( packet);
			_beginthreadex( NULL, 0, COfficeAutomationClientApp::NetRecieveThread, &theApp, 0, NULL);
			//处理方法

		}
		else if( result == LR_ERROR)
		{
		  MessageBox("获取错误。", "错误", MB_OK);
		  theApp.m_peer->DeallocatePacket( packet);
		}
		else
		{
		  MessageBox("服务器出错。", "错误", MB_OK);
		  theApp.m_peer->DeallocatePacket( packet);
		}
	}

	return;
}


void CMessageSendDlg::OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
		*pResult = 0;
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	HTREEITEM hItem = m_tree.GetSelectedItem();
	if(m_tree.GetItemText(hItem) == "OA"){
		sArr[0].id = 0;
		return;
	}
	sArr[0].name = m_tree.GetItemText(hItem);
	sArr[0].id = m_tree.GetItemData(hItem);
}
