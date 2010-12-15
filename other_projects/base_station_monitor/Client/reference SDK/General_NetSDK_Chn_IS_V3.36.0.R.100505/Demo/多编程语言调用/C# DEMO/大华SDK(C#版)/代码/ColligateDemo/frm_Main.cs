using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using DHPlaySDK;
using DHNetSDK;

namespace ColligateDemo
{
    public partial class frm_Main : Form
    {
        private NET_DEVICEINFO deviceInfo;

        /// <summary>
        /// 设备用户登录ＩＤ
        /// </summary>
        private int pLoginID;

        /// <summary>
        /// 程序消息提示Title
        /// </summary>
        private const string pMsgTitle = "大华网络SDK Demo程序";

        /// <summary>
        /// 最后操作信息显示格式
        /// </summary>
        private const string pErrInfoFormatStyle = "代码:errcode;\n描述:errmSG.";

        /// <summary>
        /// 实时监视句柄
        /// </summary>
        private int uRealHandle;

        /// <summary>
        /// 实时监视数据回调
        /// </summary>
        private fRealDataCallBack cbRealData;

        /// <summary>
        /// 断开连接回调
        /// </summary>
        private fDisConnect disConnect;

        /// <summary>
        /// 是否己设置实时数据回调
        /// </summary>
        private bool blnSetRealDataCallBack = false;

        //private int aa = 0;
        public frm_Main()
        {
            InitializeComponent();
        }

        private void btnLogin_Click(object sender, EventArgs e)
        {
            frm_AddDevice fAddDev = new frm_AddDevice();
            fAddDev.ShowDialog();
            //DHClient.DHSetShowException(true);
            try
            {
                if (fAddDev.blnOKEnter == true)
                {
                    //设备用户信息获得
                    deviceInfo = new NET_DEVICEINFO();
                    int error = 0;
                    //设备用户登录
                    pLoginID = DHClient.DHLogin(fAddDev.cmbDevIP.Text.ToString(), ushort.Parse(fAddDev.txtDevProt.Text.ToString()), fAddDev.txtName.Text.ToString(), fAddDev.txtPassword.Text.ToString(), out deviceInfo, out error);
                    if (pLoginID != 0)
                    {

                        btnStartRealPlay.Enabled = true;
                        btnLogin.Enabled = false;
                        btnLogout.Enabled = true;
                    }
                    else
                    {
                        btnStartRealPlay.Enabled = false;
                        btnLogin.Enabled = true;
                        btnLogout.Enabled = false;
                        MessageBox.Show(DHClient.LastOperationInfo.ToString(pErrInfoFormatStyle), pMsgTitle);
                    }
                }
            }
            catch
            {
                //报最后一次操作的错误信息
                MessageBox.Show(DHClient.LastOperationInfo.ToString(pErrInfoFormatStyle), pMsgTitle);
            }
        }

        /// <summary>
        /// 实时监视数据回调数据处理
        /// </summary>
        /// <param name="lRealHandle"></param>
        /// <param name="dwDataType"></param>
        /// <param name="pBuffer"></param>
        /// <param name="dwBufSize"></param>
        /// <param name="dwUser"></param>
        private void cbRealDataFun(int lRealHandle, UInt32 dwDataType, IntPtr pBuffer, UInt32 dwBufSize, IntPtr dwUser)
        {
            DHPlay.DHPlayControl(PLAY_COMMAND.InputData, 0,pBuffer,dwBufSize);//此处第二个参数nPort参数要与网络实时监控数据的取得的RealHandle对应
        }

        private void frm_Main_Load(object sender, EventArgs e)
        {
            disConnect = new fDisConnect(DisConnectEvent);
            bool blnInit=  DHClient.DHInit(disConnect, IntPtr.Zero);
            //MessageBox.Show((blnInit == true ? "成功" : "失败"), pMsgTitle);
        }
        private void DisConnectEvent(int lLoginID, StringBuilder pchDVRIP, int nDVRPort, IntPtr dwUser)
        {
            MessageBox.Show("设备断开！", pMsgTitle);
        }

        private void btnStopRealPlay_Click(object sender, EventArgs e)
        {
            DHPlay.DHPlayControl(PLAY_COMMAND.Stop, 0);
            DHPlay.DHPlayControl(PLAY_COMMAND.CloseStream, 0);
            btnStartRealPlay.Enabled = true;
            btnStopRealPlay.Enabled = false;
            picPlayMain.Refresh();
        }

        private void btnStartRealPlay_Click(object sender, EventArgs e)
        {
            if (pLoginID != 0)
            {

                uRealHandle = DHClient.DHRealPlay(pLoginID, 0, IntPtr.Zero);//只取数据不播放
                cbRealData = new fRealDataCallBack(cbRealDataFun);
                if (DHPlay.DHPlayControl(PLAY_COMMAND.OpenStream, 0, IntPtr.Zero, 0, (UInt32)(900 * 1024)))
                {
                    // MessageBox.Show("打开流播放成功！", pMsgTitle);
                }
                else
                {
                    MessageBox.Show("打开流播放失败！", pMsgTitle);
                    btnStopRealPlay.Enabled = false;
                    return;
                }

                if (DHPlay.DHSetStreamOpenMode(0, PLAY_MODE.STREAME_REALTIME))//设置流播放模式
                {
                    //MessageBox.Show("设置流播放模式成功！", pMsgTitle);
                }
                else
                {
                    MessageBox.Show("设置流播放模式失败！", pMsgTitle); 
                    btnStopRealPlay.Enabled = false;
                    return;
                }
                if (DHPlay.DHPlayControl(PLAY_COMMAND.Start, 0, picPlayMain.Handle))
                {
                    // MessageBox.Show("流播放开始成功！", pMsgTitle);
                }
                else
                {
                    MessageBox.Show("流播放开始失败！", pMsgTitle);
                    btnStopRealPlay.Enabled = false;
                    return;
                }
                if (blnSetRealDataCallBack == true)
                {
                    btnStopRealPlay.Enabled = true;
                    btnStartRealPlay.Enabled = false;
                    return;
                }
                if (DHClient.DHSetRealDataCallBack(uRealHandle, cbRealData, IntPtr.Zero))//设置数据回调处理函数
                {
                    // MessageBox.Show("设置数据回调处理函数成功！", pMsgTitle);
                    blnSetRealDataCallBack = true;
                }
                else
                {
                    MessageBox.Show("设置数据回调处理函数失败！", pMsgTitle);
                    btnStopRealPlay.Enabled = false;
                    blnSetRealDataCallBack = false;
                    return;
                }
                btnStopRealPlay.Enabled = true;
                btnStartRealPlay.Enabled = false;
                //picPlayMain.Refresh();
            }            
        }

        private void btnVDC_Click(object sender, EventArgs e)
        {

            frm_VDCDemo fVDC = new frm_VDCDemo();
            fVDC.ShowDialog();
        }

        private void btnLogout_Click(object sender, EventArgs e)
        {
            blnSetRealDataCallBack = false;
            btnStartRealPlay.Enabled = false;
            btnStopRealPlay.Enabled = false;
            btnLogout.Enabled = false;
            btnLogin.Enabled = true;
            DHClient.DHLogout(pLoginID);
        }
    }
}