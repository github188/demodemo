#if !defined(AFX_CONST_H)
#define AFX_CONST_H

#define WM_NOTIFY_MESSAGE		WM_USER + 1
#define WM_PLAYVIEW_SELECTED	WM_USER + 2
#define WM_RUNTIME_TIMER		WM_USER + 3
#define WM_WARNING_TIMER		WM_USER + 3

//WM_PLAYVIEW_SELECTED---------------------------------------------
#define MSG_SELECT_VIDEO		0x1	//Select the target play windows
#define MSG_FULL_WINDOW			0x2   //full open the windows
#define MSG_VV_1				0x3	
#define MSG_VV_4				0x4
#define MSG_VV_6				0x5
#define MSG_VV_8				0x6
#define MSG_VV_9				0x7
#define MSG_VV_16				0x8
#define MSG_VV_25				0x9
#define MSG_SELECT_CAMERA_DEVICE	0xa
#define MSG_UNSELECT_CAMERA_DEVICE	0xb
#define MSG_RESTORE_VIEW		0xc


#define ID_PLAYVIDEO_VIEW			2000			
#define ID_POPVIDEO_VIEW			2100	

//
#define ID_USER_CONTROL				2200			

//Warning Timer During...
#define WARNING_UPDATE_DURING		10000			

//Warning Ack...
typedef enum tagWARNING_STATUS
{
	WARNING_UNKNOWN,
	WARNING_UNACK,
	WARNING_ACK
}WARNING_STATUS;

typedef enum tagALARM_COMING_TYPE
{
	ALARM_NEW,
	ALARM_REFRESH
}ALARM_COMING_TYPE;

const int cnMAX_VV = 25;
const int cnMAX_POP_WINDOW = 8; //up to 8 pop window showing warning...

int play_video(int  sessionId, char * pBuffer, int  len);

const int cnPOP_VEDIO_INDEX = 25;
#endif //AFX_CONST_H