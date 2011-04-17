#pragma once

#include "BTSInfo.h"

class BTSManager
{
public:
	BTSManager(){};
	~BTSManager() //Free Memory. liang add.
	{
		POSITION pos = btsmap.GetStartPosition();
		int iKey;
		BTSInfo* pBTSInfo=NULL;
		while (pos != NULL)
		{
			btsmap.GetNextAssoc( pos, iKey, pBTSInfo );
			btsmap.RemoveKey( iKey );
			if (pBTSInfo)
			{
				delete pBTSInfo;
				pBTSInfo = NULL;
			}
			
		}
	}

public:
	CString GetCameraPlace(CString sBtsID);
	CString GetCameraUUID(CString sDevName);
	CStringArray& GetChannelsByUUID(CString sUUID);
	void buildbtsTree(CString &buffer, CTreeCtrl *tree);
	CMap<int, int, BTSInfo*, BTSInfo*> btsmap;

};
