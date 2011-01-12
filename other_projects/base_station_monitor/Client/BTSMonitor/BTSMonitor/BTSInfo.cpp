#include "stdafx.h"
#include "BTSInfo.h"

void BTSInfo::buildClass(const CString &info){
		int pos = 0;
		pos = util::split_next(info, uuid, '$', 0);
		pos = util::split_next(info, devType, '$', pos + 1);
		pos = util::split_next(info, route, '$', pos + 1);
		pos = util::split_next(info, status, '$', pos + 1);

		if(devType.GetLength() > 0){
			util::str2int(devType);
		}
}
