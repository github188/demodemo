#include <Windows.h>
#include <iostream.h>
#include <conio.h>
#include "dhnetsdk.h"

void CALLBACK DisConnectFunc(LONG lLoginID, char *pchDVRIP, LONG nDVRPort, DWORD dwUser)
{
	cout<<"断线."<<endl;
	return;
}

void CALLBACK AutoConnectFunc(LONG lLoginID,char *pchDVRIP,LONG nDVRPort,DWORD dwUser)
{
	cout<<"重连成功."<<endl;
	return;
}
int main(void)
{
	/*
	 *	程序连接上设备后，用户可以通过拔网线或其他方式使设备断线，然后再恢复网络。
	 */
	NET_DEVICEINFO deviceInfo ={0};
	unsigned long lLogin;
	CLIENT_Init(DisConnectFunc,0);//初始化sdk，设置断线回调函数
	CLIENT_SetAutoReconnect(AutoConnectFunc,0);//设置断线重连成功的回调函数。不调用此接口，sdk将不进行断线重连。
	lLogin = CLIENT_Login("10.7.4.24",37777,"3","",&deviceInfo);//登陆设备，用户需另选设备。
	if(lLogin <= 0)
	{
		cout<<"登陆设备失败"<<endl;
	}
	else
	{
		cout<<"登陆设备成功，按任意键退出程序。"<<endl;
		cout<<"网络故障1分钟如果没有恢复，sdk会认为断线，请耐心等待"<<endl;
		_getch();
	}
	CLIENT_Logout(lLogin);
	CLIENT_Cleanup();
	return 0;
}
