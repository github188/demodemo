
// testPlayback.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CtestPlaybackApp:
// See testPlayback.cpp for the implementation of this class
//

class CtestPlaybackApp : public CWinAppEx
{
public:
	CtestPlaybackApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CtestPlaybackApp theApp;