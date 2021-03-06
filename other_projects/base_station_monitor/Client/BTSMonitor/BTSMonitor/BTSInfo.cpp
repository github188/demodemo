#include "stdafx.h"
#include "BTSInfo.h"

void BTSInfo::buildClass(const CString &info){
		int pos = 0;
		pos = util::split_next(info, uuid, splitch, 0);
		pos = util::split_next(info, devType, splitch, pos + 1);
		pos = util::split_next(info, route, splitch, pos + 1);
		pos = util::split_next(info, status, splitch, pos + 1);
		pos = util::split_next(info, parentuuid, splitch, pos+1);
		pos = util::split_next(info, name, splitch, pos+1);
		if(devType!="3")
		{
			pos = util::split_next(info, channelInfo, splitch, pos+1);
			//liang add, parse channel detail info.
			if (channelInfo.GetLength() > 0)
			{
				int pos1=0;
				CString sChannel;
				char token = ',';
				pos1 = util::split_next(channelInfo, sChannel,token, 0);
				while( sChannel.GetLength()>0 )
				{
					ch_list.Add(sChannel);
					sChannel.Empty();
					pos1 = util::split_next(channelInfo, sChannel,token, pos1+1);
				}
			}
		}

		if(devType.GetLength() > 0){
			idevType=util::str2int(devType);
		}
}
