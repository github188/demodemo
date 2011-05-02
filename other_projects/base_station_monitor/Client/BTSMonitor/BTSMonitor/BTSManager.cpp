#include "stdafx.h"
#include "util.h"
#include "BTSManager.h"

//#ifdef _DEBUG
//#define new DEBUG_NEW
//#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void BTSManager::buildbtsTree(CString &buffer, CTreeCtrl *tree)
{
	int ipos=0;
	int ileft=0, iright=0;
	CString btsline;
	while(ipos!=-1)
	{
		ipos=buffer.Find('\n', ileft);
		if(ipos==-1)
		{
			break;
		}
		iright=ipos;
		btsline=buffer.Mid(ileft, iright-ileft+1);
		ileft=ipos+1;

		BTSInfo *pinfo=new BTSInfo(btsline);
		if(pinfo->parentuuid=="0")
		{
			HTREEITEM htitem=pinfo->insertRootIntoTree(tree);
			//btsmap[pinfo->uuid]=pinfo;
			btsmap.SetAt(util::str2int(pinfo->uuid), pinfo);
		}
		else
		{
			//btsmap.Lookup(util::str2int(pinfo->parentuuid), &pInfo);
			BTSInfo *pParentInfo=btsmap[util::str2int(pinfo->parentuuid)];
			pinfo->insertIntoTree(tree, pParentInfo->getPosInTree());
			btsmap.SetAt(util::str2int(pinfo->uuid), pinfo);
		}

		//if (pinfo)
		//	delete pinfo;
	}
}

//Get Place by uuid.
CString BTSManager::GetCameraPlace(CString sBtsID)
{
	POSITION pos = btsmap.GetStartPosition();
	int iKey;
	BTSInfo* pBTSInfo=NULL;
	while (pos != NULL)
	{
		btsmap.GetNextAssoc( pos, iKey, pBTSInfo);
		if (pBTSInfo && pBTSInfo->uuid == sBtsID)
			return pBTSInfo->sPlace;
	}
	return CString("");
}

CString BTSManager::GetCameraUUID(CString sDevName)
{
	POSITION pos = btsmap.GetStartPosition();
	int iKey;
	BTSInfo* pBTSInfo=NULL;
	while (pos != NULL)
	{
		btsmap.GetNextAssoc( pos, iKey, pBTSInfo);
		if (pBTSInfo && pBTSInfo->name == sDevName)
			return pBTSInfo->uuid;
	}
	return CString("");
}
CStringArray& BTSManager::GetChannelsByUUID(CString sUUID)
{
	CStringArray sChannels;
	POSITION pos = btsmap.GetStartPosition();
	int iKey;
	BTSInfo* pBTSInfo=NULL;
	while (pos != NULL)
	{
		btsmap.GetNextAssoc( pos, iKey, pBTSInfo);
		if (pBTSInfo && pBTSInfo->uuid == sUUID)
			sChannels.Append(pBTSInfo->ch_list);
	}
	return sChannels;

}

CString BTSManager::GetRouteByUUID(CString sUUID)
{
	POSITION pos = btsmap.GetStartPosition();
	int iKey;
	BTSInfo* pBTSInfo=NULL;
	while (pos != NULL)
	{
		btsmap.GetNextAssoc( pos, iKey, pBTSInfo);
		if (pBTSInfo && pBTSInfo->uuid == sUUID)
			return pBTSInfo->route;
	}

	return CString("");

}

BTSInfo* BTSManager::GetBTSInfoByUUID(CString sUUID)
{
	POSITION pos = btsmap.GetStartPosition();
	int iKey;
	BTSInfo* pBTSInfo=NULL;
	while (pos != NULL)
	{
		btsmap.GetNextAssoc( pos, iKey, pBTSInfo);
		if (pBTSInfo && pBTSInfo->uuid == sUUID)
			return pBTSInfo;
	}

	return NULL;
}
