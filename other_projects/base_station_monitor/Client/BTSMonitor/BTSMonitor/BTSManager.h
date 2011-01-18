#pragma once

#include "BTSInfo.h"

class BTSManager
{
public:
	void buildbtsTree(CString &buffer, CTreeCtrl *tree);
	CMap<int, int, BTSInfo*, BTSInfo*> btsmap;
};
