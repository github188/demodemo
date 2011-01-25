#if !defined(AFX_CONST_H)
#define AFX_CONST_H

#define WM_NOTIFY_MESSAGE		WM_USER + 1
#define WM_PLAYVIEW_SELECTED	WM_USER + 2
#define WM_RUNTIME_TIMER		WM_USER + 3

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


#define ID_PLAYVIDEO_VIEW			2000			
#define ID_POPVIDEO_VIEW			2100	

//Warning Ack...
typedef enum tagWARNING_STATUS
{
	WARNING_UNKNOWN,
	WARNING_UNACK,
	WARNING_ACK
}WARNING_STATUS;

const int cnMAX_POP_WINDOW = 8; //up to 8 pop window showing warning...
#endif //AFX_CONST_H