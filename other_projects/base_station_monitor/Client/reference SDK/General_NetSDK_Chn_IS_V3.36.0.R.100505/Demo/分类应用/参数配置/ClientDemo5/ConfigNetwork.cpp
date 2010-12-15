// ConfigNetwork.cpp : implementation file
//

#include "stdafx.h"
#include "ClientDemo5.h"
#include "ConfigNetwork.h"
#include "ClientDemo5Dlg.h"
#include "MessageText.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConfigNetwork dialog


CConfigNetwork::CConfigNetwork(CWnd* pParent /*=NULL*/)
	: CDialog(CConfigNetwork::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConfigNetwork)
	m_bTransmit = FALSE;
	m_bValid = FALSE;
	m_bDHCP = FALSE;
	m_bDefault = FALSE;
	m_bMailEn = FALSE;
	m_bMailSSLEn = FALSE;
	//}}AFX_DATA_INIT
	m_nCurEthernet = -1;
	m_nCurRemoteHost = -1;
}


void CConfigNetwork::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfigNetwork)
	DDX_Control(pDX, IDC_IPADDRESS_MAILIP, m_ctlMailIP);
	DDX_Control(pDX, IDC_IPADDRESS_NETIPMASK, m_ctlNetIPMask);
	DDX_Control(pDX, IDC_IPADDRESS_NETIP, m_ctlNetIP);
	DDX_Control(pDX, IDC_IPADDRESS_NETGATEWAY, m_ctlNetGateWay);
	DDX_Control(pDX, IDC_IPADDRESS_HOSTIP, m_ctlHostIP);
	DDX_Control(pDX, IDC_CHECK_REMOTEHOST, m_ctlChkRemote);
	DDX_Control(pDX, IDC_COMBO_REMOTEHOST, m_ctlRemoteHost);
	DDX_Control(pDX, IDC_COMBO_ETHERNET, m_ctlEthernet);
	DDX_Control(pDX, IDC_COMBO_NETINTERFACE, m_ctlNetInterface);
	DDX_Check(pDX, IDC_CHECK_TRANS, m_bTransmit);
	DDX_Check(pDX, IDC_CHECK_VALID, m_bValid);
	DDX_Check(pDX, IDC_CHECK_DHCP, m_bDHCP);
	DDX_Check(pDX, IDC_CHECK_DEFAULT, m_bDefault);
	DDX_Check(pDX, IDC_CHECK_MAILEN, m_bMailEn);
	DDX_Check(pDX, IDC_CHECK_MAILSSLEN, m_bMailSSLEn);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConfigNetwork, CDialog)
	//{{AFX_MSG_MAP(CConfigNetwork)
	ON_BN_CLICKED(IDC_BUTTON_APPLY, OnButtonApply)
	ON_BN_CLICKED(IDC_BUTTON_READALL, OnButtonReadall)
	ON_CBN_SELCHANGE(IDC_COMBO_ETHERNET, OnSelchangeComboEthernet)
	ON_CBN_SELCHANGE(IDC_COMBO_REMOTEHOST, OnSelchangeComboRemotehost)
	ON_WM_CANCELMODE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfigNetwork message handlers

//Save
void CConfigNetwork::OnButtonApply() 
{
	// TODO: Add your control notification handler code here
	if(-1 != m_ctlEthernet.GetCurSel() && -1 != m_ctlRemoteHost.GetCurSel())
	{
		SaveNetWork(m_nCurEthernet);
		SaveRemoteHost(m_nCurRemoteHost);
		
		GetDlgItemText(IDC_EDIT_DEVNAME, m_NetIOInfo.sDevName,DH_MAX_NAME_LEN);
		if (GetDlgItemInt(IDC_EDIT_MAXTCP) > 0)
		{
			m_NetIOInfo.wTcpMaxConnectNum = GetDlgItemInt(IDC_EDIT_MAXTCP);
		}
		else
		{
			m_NetIOInfo.wTcpMaxConnectNum = 10;
		}
		
		if (GetDlgItemInt(IDC_EDIT_TCPPORT) < 65535 && GetDlgItemInt(IDC_EDIT_TCPPORT) > 0)
		{
			m_NetIOInfo.wTcpPort = GetDlgItemInt(IDC_EDIT_TCPPORT);
		}

		m_NetIOInfo.wUdpPort = GetDlgItemInt(IDC_EDIT_UDPPORT);
		m_NetIOInfo.wHttpPort = GetDlgItemInt(IDC_EDIT_HTTPPORT);
		m_NetIOInfo.wHttpsPort = GetDlgItemInt(IDC_EDIT_HTTPSPORT);
		m_NetIOInfo.wSslPort = GetDlgItemInt(IDC_EDIT_SSLPORT);
		
		GetDlgItemText(IDC_EDIT_MAILUSERNAME, m_MailInfo.sUserName, DH_MAX_MAIL_NAME_LEN);
		GetDlgItemText(IDC_EDIT_MAILUSERPSW, m_MailInfo.sUserPsw, DH_MAX_MAIL_NAME_LEN);
		GetDlgItemText(IDC_IPADDRESS_MAILIP, m_MailInfo.sMailIPAddr, DH_MAX_DOMAIN_NAME_LEN);
		m_MailInfo.wMailPort = GetDlgItemInt(IDC_EDIT_MAILPORT);
		GetDlgItemText(IDC_EDIT_MAILSEDADDR, m_MailInfo.sSenderAddr, DH_MAX_MAIL_ADDR_LEN);
		GetDlgItemText(IDC_EDIT_MAILDESADDR, m_MailInfo.sDestAddr, DH_MAX_MAIL_ADDR_LEN);
		GetDlgItemText(IDC_EDIT_MAILCCADDR, m_MailInfo.sCcAddr, DH_MAX_MAIL_ADDR_LEN);
		GetDlgItemText(IDC_EDIT_MAILBCCADDR, m_MailInfo.sBccAddr, DH_MAX_MAIL_ADDR_LEN);
		GetDlgItemText(IDC_EDIT_MAILSUBJECT, m_MailInfo.sSubject, DH_MAX_MAIL_SUBJECT_LEN);
		m_MailInfo.wSendInterval = GetDlgItemInt(IDC_EDIT_SENDINTERVAL);
		if (m_MailInfo.wSendInterval > 3600)
		{
			m_MailInfo.wSendInterval = 3600;
		}
		m_MailInfo.bEnable = m_bMailEn;
		m_MailInfo.bSSLEnable = m_bMailSSLEn;
		//Call father window  to save data
		((CClientDemo5Dlg*)AfxGetMainWnd())->SetNetWorkConfigInfo(&m_NetIOInfo, &m_MailInfo);
	}
	else
	{
		MessageBox(ConvertString("Input error!"), ConvertString("Prompt"));
	}
}

//Read and get 
void CConfigNetwork::OnButtonReadall() 
{
	// TODO: Add your control notification handler code here
	//First clear 
	CleanAll();
	//Call father widnow to get data 
	((CClientDemo5Dlg*)AfxGetMainWnd())->GetNetWorkConfigInfo();
}

BOOL CConfigNetwork::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	//Interface type
	m_ctlNetInterface.InsertString(0, ConvertString(NAME_NETCFG_NET1));
	m_ctlNetInterface.InsertString(1, ConvertString(NAME_NETCFG_NET2));
	m_ctlNetInterface.InsertString(2, ConvertString(NAME_NETCFG_NET3));
	m_ctlNetInterface.InsertString(3, ConvertString(NAME_NETCFG_NET4));
	m_ctlNetInterface.InsertString(4, ConvertString(NAME_NETCFG_NET5));
	
	//Remote host 
	m_ctlRemoteHost.InsertString(0, ConvertString(NAME_NETCFG_REMOTE_ALARM));
	m_ctlRemoteHost.InsertString(1, ConvertString(NAME_NETCFG_REMOTE_LOG));
	m_ctlRemoteHost.InsertString(2, ConvertString(NAME_NETCFG_REMOTE_SMTP));
	m_ctlRemoteHost.InsertString(3, ConvertString(NAME_NETCFG_REMOTE_MULTICAST));
	m_ctlRemoteHost.InsertString(4, ConvertString(NAME_NETCFG_REMOTE_NFS));
	m_ctlRemoteHost.InsertString(5, ConvertString(NAME_NETCFG_REMOTE_PPPOE));
	m_ctlRemoteHost.InsertString(6, ConvertString(NAME_NETCFG_REMOTE_DDNS));
	m_ctlRemoteHost.InsertString(7, ConvertString(NAME_NETCFG_REMOTE_DNS));

	g_SetWndStaticText(this);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/*
 *	Function£ºDisplay network configuration information
 *	Input parameter£º
 *	pNetIOInfo£ºNetwork configuration structure 
 *	nNetIOCount£ºNetwork port amount
 */
void CConfigNetwork::InitDlgInfo(DHDEV_NET_CFG *pNetIOInfo, BYTE nNetIOCount, DHDEV_MAIL_CFG *pMailInfo)
{
	if(pNetIOInfo != NULL)
	{
		m_NetIOInfo = *pNetIOInfo;
		m_MailInfo = *pMailInfo;
		//Set Ethernet network port dropdown menu 
		m_ctlEthernet.ResetContent();
		CString strNet;
		int i =0;
		int nIndex = 0;
		for (i = 0; i < nNetIOCount; i++)
		{
			strNet.Format(ConvertString("Ethernet network%d"), i+1);
			nIndex = m_ctlEthernet.AddString(strNet);
			m_ctlEthernet.SetItemData(nIndex,i);
		}
		
		SetDlgItemText(IDC_EDIT_DEVNAME, m_NetIOInfo.sDevName);
		SetDlgItemInt(IDC_EDIT_MAXTCP, m_NetIOInfo.wTcpMaxConnectNum);
		SetDlgItemInt(IDC_EDIT_TCPPORT, m_NetIOInfo.wTcpPort);
		SetDlgItemInt(IDC_EDIT_UDPPORT, m_NetIOInfo.wUdpPort);
		SetDlgItemInt(IDC_EDIT_HTTPPORT, m_NetIOInfo.wHttpPort);
		SetDlgItemInt(IDC_EDIT_HTTPSPORT, m_NetIOInfo.wHttpsPort);
		SetDlgItemInt(IDC_EDIT_SSLPORT, m_NetIOInfo.wSslPort);

		SetDlgItemText(IDC_EDIT_MAILUSERNAME, m_MailInfo.sUserName);
		SetDlgItemText(IDC_EDIT_MAILUSERPSW, m_MailInfo.sUserPsw);
		SetDlgItemText(IDC_IPADDRESS_MAILIP, m_MailInfo.sMailIPAddr);
		SetDlgItemInt(IDC_EDIT_MAILPORT, m_MailInfo.wMailPort);
		SetDlgItemText(IDC_EDIT_MAILSEDADDR, m_MailInfo.sSenderAddr);
		SetDlgItemText(IDC_EDIT_MAILDESADDR, m_MailInfo.sDestAddr);
		SetDlgItemText(IDC_EDIT_MAILCCADDR, m_MailInfo.sCcAddr);
		SetDlgItemText(IDC_EDIT_MAILBCCADDR, m_MailInfo.sBccAddr);
		SetDlgItemText(IDC_EDIT_MAILSUBJECT, m_MailInfo.sSubject);
		SetDlgItemInt(IDC_EDIT_SENDINTERVAL, m_MailInfo.wSendInterval);
		m_bMailEn = m_MailInfo.bEnable;
		m_bMailSSLEn = m_MailInfo.bSSLEnable;
		
		if (m_ctlEthernet.GetCount() > 0)
		{
			m_ctlEthernet.SetCurSel(0);
			OnSelchangeComboEthernet();
		}
		if (m_ctlRemoteHost.GetCount() > 0)
		{
			m_ctlRemoteHost.SetCurSel(0);
			OnSelchangeComboRemotehost();
		}
	}
}

//Ethernet port switch 
void CConfigNetwork::OnSelchangeComboEthernet() 
{
	// TODO: Add your control notification handler code here
	int nIndex = m_ctlEthernet.GetCurSel();
	if(CB_ERR != nIndex)
	{
		int nNum = m_ctlEthernet.GetItemData(nIndex);
		
		//First save Ethernet port information 
		SaveNetWork(m_nCurEthernet);
		
		//Display Ethernet port corresponding information 
		SetDlgItemText(IDC_IPADDRESS_NETIP, m_NetIOInfo.stEtherNet[nNum].sDevIPAddr);
		SetDlgItemText(IDC_IPADDRESS_NETIPMASK, m_NetIOInfo.stEtherNet[nNum].sDevIPMask);
		SetDlgItemText(IDC_IPADDRESS_NETGATEWAY, m_NetIOInfo.stEtherNet[nNum].sGatewayIP);
		m_ctlNetInterface.SetCurSel(m_NetIOInfo.stEtherNet[nNum].dwNetInterface-1);
		SetDlgItemText(IDC_EDIT_NETMAC, m_NetIOInfo.stEtherNet[nNum].byMACAddr);
		m_bDHCP = (m_NetIOInfo.stEtherNet[nNum].bValid & 0x2) != 0 ? TRUE : FALSE;
		m_bValid = (m_NetIOInfo.stEtherNet[nNum].bValid & 0x1) != 0 ? FALSE : TRUE;
		m_bTransmit = m_NetIOInfo.stEtherNet[nNum].bTranMedia == 0 ? TRUE : FALSE;
		m_bDefault = m_NetIOInfo.stEtherNet[nNum].bDefaultEth == 0 ? FALSE : TRUE;
		UpdateData(FALSE);
		
		m_nCurEthernet = nNum;
	}
}

//Remote host switch 
void CConfigNetwork::OnSelchangeComboRemotehost() 
{
	// TODO: Add your control notification handler code here
	int nIndex = m_ctlRemoteHost.GetCurSel();
	if(CB_ERR != nIndex && CB_ERR != m_ctlEthernet.GetCurSel())
	{
		//First save remote host information 
		SaveRemoteHost(m_nCurRemoteHost);
		
		//Display remote host corresponding information 
		DH_REMOTE_HOST remoteHostInfo = GetRemoteHostType(nIndex);
		m_ctlChkRemote.SetCheck(remoteHostInfo.byEnable?1:0);
		SetDlgItemText(IDC_IPADDRESS_HOSTIP, remoteHostInfo.sHostIPAddr);
		SetDlgItemInt(IDC_EDIT_HOSTPORT, remoteHostInfo.wHostPort);
		SetDlgItemText(IDC_EDIT_HOSTUSERNAME, remoteHostInfo.sHostUser);
		SetDlgItemText(IDC_EDIT_HOSTUSERPSW, remoteHostInfo.sHostPassword);
		UpdateData(FALSE);
		
		m_nCurRemoteHost = nIndex;
	}
}

//Save Ethernet port information 
void CConfigNetwork::SaveNetWork(int nNum)
{
	if(-1 < nNum)
	{
		GetDlgItemText(IDC_IPADDRESS_NETIP,m_NetIOInfo.stEtherNet[nNum].sDevIPAddr,DH_MAX_IPADDR_LEN);
		GetDlgItemText(IDC_IPADDRESS_NETIPMASK,m_NetIOInfo.stEtherNet[nNum].sDevIPMask,DH_MAX_IPADDR_LEN);
		GetDlgItemText(IDC_IPADDRESS_NETGATEWAY,m_NetIOInfo.stEtherNet[nNum].sGatewayIP,DH_MAX_IPADDR_LEN);
		m_NetIOInfo.stEtherNet[nNum].dwNetInterface = m_ctlNetInterface.GetCurSel() + 1;
		GetDlgItemText(IDC_EDIT_NETMAC,m_NetIOInfo.stEtherNet[nNum].byMACAddr,DH_MACADDR_LEN);
		UpdateData(TRUE);
		if (m_bDHCP)
		{
			m_NetIOInfo.stEtherNet[nNum].bValid |= 0x02;
		}
		else
		{
			m_NetIOInfo.stEtherNet[nNum].bValid &= 0xFD;
		}
		if (m_bValid)
		{
			m_NetIOInfo.stEtherNet[nNum].bValid &= 0xFE;
		}
		else
		{
			m_NetIOInfo.stEtherNet[nNum].bValid |= 0x01;
		}
		m_NetIOInfo.stEtherNet[nNum].bTranMedia = m_bTransmit ? 0 : 1;
		m_NetIOInfo.stEtherNet[nNum].bDefaultEth = m_bDefault ? 1 : 0;
	}
}

//Save remote host information 
void CConfigNetwork::SaveRemoteHost(int nNum)
{
	if(-1 < nNum)
	{
		switch(nNum)
		{
		case 0:	/* Alarm server */
			GetDlgItemText(IDC_IPADDRESS_HOSTIP,m_NetIOInfo.struAlarmHost.sHostIPAddr,DH_MAX_IPADDR_LEN);
			m_NetIOInfo.struAlarmHost.wHostPort = GetDlgItemInt(IDC_EDIT_HOSTPORT);
			break;
		case 1:/* Log server */
			GetDlgItemText(IDC_IPADDRESS_HOSTIP,m_NetIOInfo.struLogHost.sHostIPAddr,DH_MAX_IPADDR_LEN);
			m_NetIOInfo.struLogHost.wHostPort = GetDlgItemInt(IDC_EDIT_HOSTPORT);
			break;
		case 2:/* SMTP server */
			GetDlgItemText(IDC_IPADDRESS_HOSTIP,m_NetIOInfo.struSmtpHost.sHostIPAddr,DH_MAX_IPADDR_LEN);
			m_NetIOInfo.struSmtpHost.wHostPort = GetDlgItemInt(IDC_EDIT_HOSTPORT);
			break;
		case 3:/* Multiplecast group */
			GetDlgItemText(IDC_IPADDRESS_HOSTIP,m_NetIOInfo.struMultiCast.sHostIPAddr,DH_MAX_IPADDR_LEN);
			m_NetIOInfo.struMultiCast.wHostPort = GetDlgItemInt(IDC_EDIT_HOSTPORT);
			break;
		case 4:/* NFS server */
			GetDlgItemText(IDC_IPADDRESS_HOSTIP,m_NetIOInfo.struNfs.sHostIPAddr,DH_MAX_IPADDR_LEN);
			m_NetIOInfo.struNfs.wHostPort = GetDlgItemInt(IDC_EDIT_HOSTPORT);
			m_NetIOInfo.struNfs.byEnable = m_ctlChkRemote.GetCheck();
			break;
		case 5:/* PPPoE server*/
			GetDlgItemText(IDC_EDIT_HOSTUSERNAME, m_NetIOInfo.struPppoe.sHostUser,DH_MAX_HOST_NAMELEN);
			GetDlgItemText(IDC_EDIT_HOSTUSERPSW, m_NetIOInfo.struPppoe.sHostPassword,DH_MAX_HOST_PSWLEN);
			m_NetIOInfo.struPppoe.byEnable = m_ctlChkRemote.GetCheck();
			GetDlgItemText(IDC_EDIT_HOSTRESERVED, m_NetIOInfo.sPppoeIP,DH_MAX_IPADDR_LEN);
			break;
		case 6:/* DDNS server */
			GetDlgItemText(IDC_IPADDRESS_HOSTIP,m_NetIOInfo.struDdns.sHostIPAddr,DH_MAX_IPADDR_LEN);
			m_NetIOInfo.struDdns.wHostPort = GetDlgItemInt(IDC_EDIT_HOSTPORT);
			m_NetIOInfo.struDdns.byEnable = m_ctlChkRemote.GetCheck();
			GetDlgItemText(IDC_EDIT_HOSTRESERVED, m_NetIOInfo.sDdnsHostName,DH_MAX_HOST_NAMELEN);
			break;
		case 7:/* DNS server*/
			GetDlgItemText(IDC_IPADDRESS_HOSTIP,m_NetIOInfo.struDns.sHostIPAddr,DH_MAX_IPADDR_LEN);
			break;
		default:
			break;
		}
	}
}

//Get current selected remote host type
DH_REMOTE_HOST CConfigNetwork::GetRemoteHostType(int nType)
{
	DH_REMOTE_HOST remoteHostInfo = {0};
	CString str;
	switch(nType)
	{
	case 0:	/* Alarm server */
		remoteHostInfo = m_NetIOInfo.struAlarmHost;
		break;
	case 1:/* Log server*/
		remoteHostInfo = m_NetIOInfo.struLogHost;
		break;
	case 2:/* SMTP server*/
		remoteHostInfo = m_NetIOInfo.struSmtpHost;
		break;
	case 3:/* Multiplecast group */
		remoteHostInfo = m_NetIOInfo.struMultiCast;
		break;
	case 4:/* NFS server */
		remoteHostInfo = m_NetIOInfo.struNfs;
		break;
	case 5:/* PPPoE server */
		remoteHostInfo = m_NetIOInfo.struPppoe;
		str = NAME_NETCFG_PPPOE_IP;
		str = str + ":";
		SetDlgItemText(IDC_HOSTRESERVED, str);
		SetDlgItemText(IDC_EDIT_HOSTRESERVED, m_NetIOInfo.sPppoeIP);
		break;
	case 6:/* DDNS server*/
		remoteHostInfo = m_NetIOInfo.struDdns;
		str = NAME_NETCFG_DDNS_HOSTNAME;
		str = str + ":";
		SetDlgItemText(IDC_HOSTRESERVED, str);
		SetDlgItemText(IDC_EDIT_HOSTRESERVED, m_NetIOInfo.sDdnsHostName);
		break;
	case 7:/* DNS server*/
		remoteHostInfo = m_NetIOInfo.struDns;
		break;
	default:
		break;
	}
	m_ctlChkRemote.EnableWindow(4 == nType || 5 == nType || 6 == nType);
	GetDlgItem(IDC_IPADDRESS_HOSTIP)->EnableWindow(5 != nType);
	GetDlgItem(IDC_EDIT_HOSTPORT)->EnableWindow(5 != nType && 7 != nType);
	GetDlgItem(IDC_EDIT_HOSTUSERNAME)->EnableWindow(4 == nType || 5 == nType);
	GetDlgItem(IDC_EDIT_HOSTUSERPSW)->EnableWindow(4 == nType || 5 == nType);
	GetDlgItem(IDC_HOSTRESERVED)->ShowWindow((5 == nType || 6 == nType)?SW_SHOW:SW_HIDE);
	GetDlgItem(IDC_EDIT_HOSTRESERVED)->ShowWindow((5 == nType || 6 == nType)?SW_SHOW:SW_HIDE);
	return remoteHostInfo;
}

//Clear
void CConfigNetwork::CleanAll()
{
	m_ctlNetInterface.SetCurSel(-1);
	m_ctlRemoteHost.SetCurSel(-1);
	m_ctlEthernet.ResetContent();
	
	SetDlgItemText(IDC_EDIT_DEVNAME, "");
	SetDlgItemText(IDC_EDIT_MAXTCP, "");
	SetDlgItemText(IDC_EDIT_TCPPORT, "");
	SetDlgItemText(IDC_EDIT_UDPPORT, "");
	SetDlgItemText(IDC_EDIT_HTTPPORT, "");
	SetDlgItemText(IDC_EDIT_HTTPSPORT, "");
	SetDlgItemText(IDC_EDIT_SSLPORT, "");
	
	SetDlgItemText(IDC_EDIT_MAILUSERNAME, "");
	SetDlgItemText(IDC_EDIT_MAILUSERPSW, "");
	SetDlgItemText(IDC_IPADDRESS_MAILIP,"");
	SetDlgItemText(IDC_EDIT_MAILPORT, "");
	SetDlgItemText(IDC_EDIT_MAILSEDADDR, "");
	SetDlgItemText(IDC_EDIT_MAILDESADDR, "");
	SetDlgItemText(IDC_EDIT_MAILCCADDR, "");
	SetDlgItemText(IDC_EDIT_MAILBCCADDR, "");
	SetDlgItemText(IDC_EDIT_MAILSUBJECT, "");
	SetDlgItemInt(IDC_EDIT_SENDINTERVAL, 0);
	m_bMailEn = FALSE;
	m_bMailSSLEn = FALSE;

	SetDlgItemText(IDC_IPADDRESS_NETIP, "");
	SetDlgItemText(IDC_IPADDRESS_NETIPMASK, "");
	SetDlgItemText(IDC_IPADDRESS_NETGATEWAY, "");
	SetDlgItemText(IDC_EDIT_NETMAC, "");

	m_ctlChkRemote.SetCheck(0);
	SetDlgItemText(IDC_IPADDRESS_HOSTIP, "");
	SetDlgItemText(IDC_EDIT_HOSTPORT, "");
	SetDlgItemText(IDC_EDIT_HOSTUSERNAME, "");
	SetDlgItemText(IDC_EDIT_HOSTUSERPSW, "");
	GetDlgItem(IDC_HOSTRESERVED)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_HOSTRESERVED)->ShowWindow(SW_HIDE);
	
	m_nCurEthernet = -1;
	m_nCurRemoteHost = -1;

	UpdateData(FALSE);
}

void CConfigNetwork::OnCancelMode() 
{
	CDialog::OnCancelMode();
	
	// TODO: Add your message handler code here
	
}








































































































































































































































































































































































































































































































































