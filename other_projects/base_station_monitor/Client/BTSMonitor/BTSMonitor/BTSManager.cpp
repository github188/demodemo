#include "stdafx.h"
#include "util.h"
#include "BTSManager.h"

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
		btsline=buffer.Mid(ileft, iright);
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
		}
	}
}
