#pragma once

#include "BTSInfo.h"

class BTSManager
{
public:
	void buildbtsTree(CString &buffer, const CTreeCtrl &tree);
	CMap<int, int, BTSInfo, const BTSInfo &> btsmap;
};
