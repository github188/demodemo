#pragma once

#include "util.h"

class BTSInfo{
private:
	void buildClass(const CString &info);

public:
	CString uuid;
	CString devType; //1: ÊÓÆµ¼à¿Ø £¬2: Í¼Æ¬¼à¿Ø£¬ 3: µØÇø
	CString route;
	CString status;
	CString parentuuid;
	CString name;
	CString channelInfo;
	int idevType;
	static const char splitch='$';
	CStringArray ch_list;
	CString sPlace;

	HTREEITEM posInTree;

public:
	BTSInfo(const CString &info)
	{
		buildClass(info);
	}

	BTSInfo()
	{
	}

	HTREEITEM insertRootIntoTree(CTreeCtrl *tree)
	{
		int nImage=0;
		if(idevType==3)
		{
			nImage=0;
		}
		else if(idevType==2)
		{
			nImage=1;
		}
		else if(idevType==1)
		{
			nImage=2;
		}
		posInTree=tree->InsertItem(name, nImage, nImage);
		return posInTree;
	}

	HTREEITEM insertIntoTree(CTreeCtrl *tree, HTREEITEM parentItem)
	{
		int nImage=0;
		if(idevType==3)
		{
			nImage=0;
		}
		else if(idevType==2)
		{
			nImage=1;
		}
		else if(idevType==1)
		{
			nImage=2;
		}
		
		BOOL bIsBTS = (!channelInfo.IsEmpty());
		CString sBtsName;
		if (bIsBTS)
			sBtsName = uuid + "_" + name;
		else
			sBtsName = name;

		posInTree=tree->InsertItem(/*name*/sBtsName, nImage, nImage, parentItem);

		sPlace = name;
		HTREEITEM hTempItem = posInTree;
		while (hTempItem = tree->GetParentItem(hTempItem))
		{
			sPlace = tree->GetItemText(hTempItem) + sPlace;
		}

		//liang add for channel
		if ( !channelInfo.IsEmpty() )
		{
			HTREEITEM hItem;
			nImage = 1;
			int i=0;
			for(;i< ch_list.GetSize(); i++)
			{
				hItem= tree->InsertItem(ch_list.GetAt(i), nImage, nImage, posInTree);
				//tree->SetItemData(hItem,(DWORD)posInTree);
			}

			if (hItem)
				posInTree = hItem;
		}

		
		return posInTree;
	}

	void setPosInTree(HTREEITEM pos)
	{
		posInTree=pos;
	}

	HTREEITEM getPosInTree()
	{
		return posInTree;
	}
	
	void printitself()
	{
		_tprintf("uuid %s, devType %s, route %s, status %s parentuuid %s name %s\n",
				uuid, devType, route, status, parentuuid, name);
		if(devType!="3")
		{
			_tprintf("channelInfo %s\n",channelInfo);
		}
	}

};