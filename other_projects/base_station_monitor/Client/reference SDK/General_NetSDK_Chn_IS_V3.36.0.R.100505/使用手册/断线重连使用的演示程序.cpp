#include <Windows.h>
#include <iostream.h>
#include <conio.h>
#include "dhnetsdk.h"

void CALLBACK DisConnectFunc(LONG lLoginID, char *pchDVRIP, LONG nDVRPort, DWORD dwUser)
{
	cout<<"����."<<endl;
	return;
}

void CALLBACK AutoConnectFunc(LONG lLoginID,char *pchDVRIP,LONG nDVRPort,DWORD dwUser)
{
	cout<<"�����ɹ�."<<endl;
	return;
}
int main(void)
{
	/*
	 *	�����������豸���û�����ͨ�������߻�������ʽʹ�豸���ߣ�Ȼ���ٻָ����硣
	 */
	NET_DEVICEINFO deviceInfo ={0};
	unsigned long lLogin;
	CLIENT_Init(DisConnectFunc,0);//��ʼ��sdk�����ö��߻ص�����
	CLIENT_SetAutoReconnect(AutoConnectFunc,0);//���ö��������ɹ��Ļص������������ô˽ӿڣ�sdk�������ж���������
	lLogin = CLIENT_Login("10.7.4.24",37777,"3","",&deviceInfo);//��½�豸���û�����ѡ�豸��
	if(lLogin <= 0)
	{
		cout<<"��½�豸ʧ��"<<endl;
	}
	else
	{
		cout<<"��½�豸�ɹ�����������˳�����"<<endl;
		cout<<"�������1�������û�лָ���sdk����Ϊ���ߣ������ĵȴ�"<<endl;
		_getch();
	}
	CLIENT_Logout(lLogin);
	CLIENT_Cleanup();
	return 0;
}
