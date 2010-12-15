unit dhnetsdk;

interface

uses
        SysUtils,WinTypes;

type
(************************************************************************
 ** �������Ͽ��ص�ԭ��
 ***********************************************************************)
        fDisConnect = procedure (lLoginID: LongInt; strDVRIP: Pchar; nDVRPort: LongInt;  dwUser: LongWord); stdcall;

(************************************************************************
 ** �ص�ԭ��
 ***********************************************************************)
        fMessCallBack = function (lCommand,lLoginID:LongInt; var pBuf;
			dwBufLen:LongWord; strDVRIP:Pchar; nDVRPort:LongInt; dwUser: LongWord ):LongBool;stdcall;

(************************************************************************
 ** ��Ļ���ӻص�ԭ��
 ***********************************************************************)
        fDrawCallBack = procedure (lLoginID,lPlayHandle:LongInt; hDC: THandle; dwUser:LongInt);stdcall;

(************************************************************************
 ** �豸��Ϣ
 ***********************************************************************)
        NET_DEVICEINFO=Record
	        sSerialNumber: array [0..47] of Byte;	//���к�
        	byAlarmInPortNum: byte;				//DVR�����������
	        byAlarmOutPortNum:BYTE;				//DVR�����������
        	byDiskNum:BYTE;						//DVR Ӳ�̸���
	        byDVRType:BYTE ;						//DVR����,
        	byChanNum:BYTE;						//DVR ͨ������
        end;

(************************************************************************
 ** SDK��ʼ��
 ***********************************************************************)
function  CLIENT_Init( cbDisConnect: fDisConnect; dwUser:LongWord): LongBool; stdcall; External 'dhnetsdk'

(************************************************************************
 ** SDK�˳�����
 ***********************************************************************)
procedure CLIENT_Cleanup() ; stdcall;External 'dhnetsdk'

(************************************************************************
 ** �������ӷ�������ʱʱ��ͳ��Դ���
 ***********************************************************************)
procedure CLIENT_SetConnectTime(nWaitTime: Integer; nTryTimes: Integer);stdcall; External 'dhnetsdk';


(************************************************************************
 ** ��ȡSDK�İ汾��Ϣ
 ***********************************************************************)
function CLIENT_GetSDKVersion(): LongWord;stdcall; External 'dhnetsdk';


(************************************************************************
 ** ���豸ע��
 ***********************************************************************)
function CLIENT_Login(strDVRIP: Pchar; wDVRPort: WORD; strUserName: Pchar; strPassword: Pchar; var netDeviceInfo: NET_DEVICEINFO; error:Pinteger):LongInt;stdcall; External 'dhnetsdk';


(************************************************************************
 ** ���豸ע��
 ***********************************************************************)
function CLIENT_Logout(lLoginID: LongInt):LongBool;stdcall; External 'dhnetsdk';



(************************************************************************
 **
 ***********************************************************************)
procedure CLIENT_SetDVRMessCallBack(cbMessage:fMessCallBack; dwUser:LongWord);stdcall; External 'dhnetsdk';


(************************************************************************
 ** ������Ļ���ӻص�
 ***********************************************************************)
procedure CLIENT_RigisterDrawFun(cbDraw:fDrawCallBack; dwUser:LongWord);stdcall; External 'dhnetsdk';


(************************************************************************
 ** ��ʼʵʱԤ��
 ***********************************************************************)
function CLIENT_RealPlay(lLoginID:LongInt; nChannelID:Integer; hWnd:HWND):LongInt;stdcall; External 'dhnetsdk';


(************************************************************************
 ** ֹͣʵʱԤ��
 ***********************************************************************)
function CLIENT_StopRealPlay(lRealHandle:LongInt):LongBool;stdcall; External 'dhnetsdk';


implementation

end.

