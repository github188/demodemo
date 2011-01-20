#pragma once

#include "util.h"

/*
<<<<< .mine
class BTSInfo{
private:
	void buildClass(const CString &info);

public:
	CString uuid;
	CString devType;
	CString route;
	CString status;
	int idevType;

public:
	BTSInfo(const CString &info)
	{
		buildClass(info);
	}
	
	void printitself()
	{
		_tprintf("uuid %s, devType %s, route %s, status %s\n",
				uuid, devType, route, status);
	}
*/

class BTSInfo{
private:
	void buildClass(const CString &info);

public:
	CString uuid;
	CString devType;
	CString route;
	CString status;
	CString parentuuid;
	CString name;
	CString channelInfo;
	int idevType;
	static const char splitch='$';

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
		posInTree=tree->InsertItem(name, nImage, nImage, parentItem);
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
//>>>>>>> .r455
};