#include "stdafx.h"
#include "VideoPlayControl.h"

void VideoPlayControl::real_play(CString &uuid)
{
	CString cmd="video>real?uuid=";
	cmd.Append(uuid);
	cmd.Append("\n");
	vedio_command_list.push_back(cmd);
}

void VideoPlayControl::replay(CString &uuid)
{
	CString cmd="video>replay?uuid=";
	cmd.Append(uuid);
	cmd.Append("\n");
	vedio_command_list.push_back(cmd);
}