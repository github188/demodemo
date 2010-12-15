using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using DHNetSDK;
using System.Runtime.InteropServices;       

namespace SDKDownLoadFileDemo
{
    public partial class frm_Main : Form
    {
        /// <summary>
        /// 设备用户登录句柄
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
        /// 用户信息
        /// </summary>
        private USER_MANAGE_INFO userManageInfo;
        /// <summary>
        /// 常规报警信息
        /// </summary>
        private NET_CLIENT_STATE clientState;
        /// <summary>
        /// 按文件下载句柄
        /// </summary>
        private int pDownloadHandleByFile;
        /// <summary>
        /// 按时间下载句柄
        /// </summary>
        private int pDownloadHandleByTime;
        /// <summary>
        /// 下载句柄
        /// </summary>
        private int pDownloadHandle;
        /// <summary>
        /// 断开回调
        /// </summary>
        private fDisConnect disConnect;
        /// <summary>
        /// 下载回调
        /// </summary>
        private fDownLoadPosCallBack downLoadFun;
        /// <summary>
        /// 下载进度百分比
        /// </summary>
        private double dblDownLoadPos;
        public frm_Main()
        {
            InitializeComponent();
        }
        /// <summary>
        /// 画面加载初期化
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void frmMain_Load(object sender, EventArgs e)
        {
            disConnect = new fDisConnect(DisConnectEvent);
            DHClient.DHInit(disConnect, IntPtr.Zero);
            downLoadFun = new fDownLoadPosCallBack(DownLoadPosFun);
            grbMain.Enabled = false;
            btnDownLoad1.Tag = "";
            btnDownLoad2.Tag = "";
        }
        /// <summary>
        /// 设备断开连接处理
        /// </summary>
        /// <param name="lLoginID">登录ID</param>
        /// <param name="pchDVRIP">DVR设备IP</param>
        /// <param name="nDVRPort">DVR设备端口</param>
        /// <param name="dwUser">用户数据</param>
        private void DisConnectEvent(int lLoginID, StringBuilder pchDVRIP, int nDVRPort, IntPtr dwUser)
        {
            //设备断开连接处理            
            MessageBox.Show("设备用户断开连接", pMsgTitle);
        }
        /// <summary>
        /// 用户登录按钮按下处理
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnUserLogin_Click(object sender, EventArgs e)
        {
            switch (btnUserLogin.Text)
            {
                case "设备用户登录":
                    frm_AddDevice fLogin = new frm_AddDevice();
                    fLogin.ShowDialog();
                    if (fLogin.blnOKEnter == true)
                    {
                        //设备用户信息获得
                        NET_DEVICEINFO deviceInfo = new NET_DEVICEINFO();
                        int error = 0;
                        //设备用户登录
                        pLoginID = DHClient.DHLogin(fLogin.cmbDevIP.Text.ToString(), ushort.Parse(fLogin.txtDevProt.Text.ToString()), fLogin.txtName.Text.ToString(), fLogin.txtPassword.Text.ToString(), out deviceInfo, out error);
                        if (pLoginID != 0)
                        {
                            btnUserLogin.BackColor = Color.Yellow;
                            btnUserLogin.Text = "设备用户注销";
                            grbMain.Enabled = true;
                        }
                        else
                        {
                            //报最后一次操作的错误信息
                            MessageBox.Show(DHClient.LastOperationInfo.ToString(pErrInfoFormatStyle), pMsgTitle);
                            grbMain.Enabled = false;
                        }
                    }
                    break;
                case "设备用户注销":
                    bool result = DHClient.DHLogout(pLoginID);
                    if (result == false)
                    {
                        //报最后一次操作的错误信息
                        MessageBox.Show(DHClient.LastOperationInfo.ToString(pErrInfoFormatStyle), pMsgTitle);
                    }
                    btnUserLogin.BackColor = Color.Transparent;
                    btnUserLogin.Text = "设备用户登录";
                    grbMain.Enabled = false;
                    break;
            }
        }

        /// <summary>
        /// 按文件下载按钮按下
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnDownLoad1_Click(object sender, EventArgs e)
        {
            frm_PlayBackByFileSet fpf = new frm_PlayBackByFileSet();
            int pPlayBackChannelID = 0;
            NET_RECORDFILE_INFO fileInfo;
            fpf.gLoginID = pLoginID;
            fpf.cmbChannelSelect.Items.Clear();
            for (int i = 0; i < 16; i++)
            {
                fpf.cmbChannelSelect.Items.Add(i.ToString());
            }
            fpf.ShowDialog(this);
            if (fpf.blnOKEnter == true)
            {
                pPlayBackChannelID = int.Parse(fpf.txtChannelID.Text.ToString());
                fileInfo = fpf.gFileInfo;
                //**********按文件下载**********
                if (txtDirPath1.Text.Trim().Length>0 && txtFileName1.Text.Trim().Length > 0)
                {
                    pDownloadHandleByFile = DHClient.DHDownloadByRecordFile(pLoginID, fileInfo, txtDirPath1.Text + @"\" + txtFileName1.Text, downLoadFun, IntPtr.Zero);
                    if (pDownloadHandleByFile != 0)
                    {
                        btnDownLoad1.Tag = "下载中";
                        pDownloadHandle = pDownloadHandleByFile;
                        MessageBox.Show("开始下载!", pMsgTitle);
                    }
                    else
                    {
                        MessageBox.Show(DHClient.LastOperationInfo.ToString(pErrInfoFormatStyle), pMsgTitle);
                    }
                }
                else
                {
                    MessageBox.Show("请输入有效的录像保存目录和文件名!", pMsgTitle);
                }
                
                //******************************                
            }
        }
        /// <summary>
        /// 下载回调
        /// </summary>
        /// <param name="lPlayHandle">播放句柄</param>
        /// <param name="dwTotalSize">累计大小</param>
        /// <param name="dwDownLoadSize">下载大小</param>
        /// <param name="dwUser">用户数据</param>
        private void DownLoadPosFun(int lPlayHandle, UInt32 dwTotalSize, UInt32 dwDownLoadSize, IntPtr dwUser)
        {
            dblDownLoadPos = ((double)(dwDownLoadSize / dwTotalSize)*100);
            psbMain.Maximum = (int)dwTotalSize;
            psbMain.Value = (int)(dwDownLoadSize != 0xFFFFFFFF && dwDownLoadSize!=0xFFFFFFFE && dwDownLoadSize <= dwTotalSize ? dwDownLoadSize : 0);

            if ((0xFFFFFFFF == dwDownLoadSize) /*|| (dwDownLoadSize == dwTotalSize)*/)
            {
                btnDownLoad1.Tag = "";
                psbMain.Value = 0;
                DHClient.DHStopDownload(pDownloadHandle);
                MessageBox.Show("下载结束！");
            }
            else if (0xFFFFFFFE == dwDownLoadSize)
            {
                MessageBox.Show("磁盘空间不足！");
                DHClient.DHStopDownload(pDownloadHandle);
            }
            
        }
        /// <summary>
        /// 按文件下载目录选择
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnDirSelect1_Click(object sender, EventArgs e)
        {
            if(fbdMain.ShowDialog()==DialogResult.OK)
            {
                txtDirPath1.Text = fbdMain.SelectedPath;
            }
        }
        /// <summary>
        /// 停止按文件下载按钮按下
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnStopDownLoad1_Click(object sender, EventArgs e)
        {
            if (btnDownLoad1.Tag.ToString().Equals("下载中"))
            {
                bool blnStopSucced = DHClient.DHStopDownload(pDownloadHandleByFile);
                if (blnStopSucced)
                {
                    MessageBox.Show("停止下载成功！", pMsgTitle);
                }
                else
                {
                    MessageBox.Show(DHClient.LastOperationInfo.ToString(pErrInfoFormatStyle), pMsgTitle);
                }
                btnDownLoad1.Tag = "";
                psbMain.Maximum = 100;
                psbMain.Value = 0;
            }
            else
            {
                MessageBox.Show("当前没有下载任务！", pMsgTitle);
            }
            
        }
        /// <summary>
        /// 获取按文件下载的下载进度
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnGetDownLoadPos1_Click(object sender, EventArgs e)
        {
            if (btnDownLoad1.Tag.ToString().Equals("下载中"))
            {
                int dwTotal=0;
                int dwSize=0;
                bool blnGetPosSucced = DHClient.DHGetDownloadPos(pDownloadHandleByFile, out dwTotal, out dwSize);
                if (blnGetPosSucced)
                {
                    MessageBox.Show("下载统计:\n\t总长度:"+ dwTotal.ToString()+"\n\t己下载:"+dwSize.ToString() ,pMsgTitle);
                }
                else
                {
                    MessageBox.Show(DHClient.LastOperationInfo.ToString(pErrInfoFormatStyle), pMsgTitle);
                }
            }
            else
            {
                MessageBox.Show("当前没有下载任务！", pMsgTitle);
            }
        }
        /// <summary>
        /// 按时间下载按钮按下
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnDownLoad2_Click(object sender, EventArgs e)
        {
            frm_PlayBackByTimeSet fPBSet = new frm_PlayBackByTimeSet();
            fPBSet.cmbChannelSelect.Items.Clear();
            for (int i = 0; i < 16; i++)
            {
                fPBSet.cmbChannelSelect.Items.Add(i.ToString());
            }
            NET_RECORDFILE_INFO            fileInfo = new NET_RECORDFILE_INFO();
            int fileCount = 0;
            bool blnQueryRecordFile = false;
            int pPlayBackChannelID=0;
            fPBSet.ShowDialog();
            if (fPBSet.blnOKEnter == true)
            {
                DateTime startTime = fPBSet.dtpStart.Value;
                DateTime endTime = fPBSet.dtpEnd.Value;
                if (startTime.Date >= endTime.Date)
                {
                    MessageBox.Show("开始日期不在结束日期设置前，请重新设置！", pMsgTitle);
                }
                else
                {
                    blnQueryRecordFile = DHClient.DHQueryRecordFile(pLoginID, int.Parse(fPBSet.txtChannelID.Text.ToString()), RECORD_FILE_TYPE.ALLRECORDFILE,
                                                                    startTime, endTime, null, ref fileInfo, Marshal.SizeOf(typeof(NET_RECORDFILE_INFO)), out  fileCount, 5000, false);//按时间回放
                    if (blnQueryRecordFile == true)
                    {
                        //**********按文件下载**********
                        pPlayBackChannelID = int.Parse(fPBSet.txtChannelID.Text.ToString());
                        if (txtDirPath2.Text.Trim().Length > 0 && txtFileName2.Text.Trim().Length > 0)
                        {
                            pDownloadHandleByTime = DHClient.DHDownloadByTime(pLoginID, pPlayBackChannelID, 0, startTime, endTime, txtDirPath1.Text + @"\" + txtFileName2.Text, downLoadFun, IntPtr.Zero);
                            if (pDownloadHandleByTime != 0)
                            {
                                btnDownLoad2.Tag = "下载中";
                                pDownloadHandle = pDownloadHandleByTime;
                                MessageBox.Show("开始下载！", pMsgTitle);
                            }
                            else
                            {
                                MessageBox.Show(DHClient.LastOperationInfo.ToString(pErrInfoFormatStyle), pMsgTitle);
                            }
                        }
                        else
                        {
                            MessageBox.Show("请输入有效的录像保存目录和文件名!", pMsgTitle);
                        }
                        //*******************************
                    }
                    else
                    {
                        MessageBox.Show("这个时间段里没有录像文件供下载!", pMsgTitle);
                    }
                }
                //MessageBox.Show(blnQueryRecordFile.ToString(),MsgTitle);
            }
        }
        /// <summary>
        /// 停止按时间下载按钮按下
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnStopDownLoad2_Click(object sender, EventArgs e)
        {
            if (btnDownLoad2.Tag.ToString().Equals("下载中"))
            {
                bool blnStopSucced = DHClient.DHStopDownload(pDownloadHandleByTime);
                if (blnStopSucced)
                {
                    MessageBox.Show("停止下载成功！", pMsgTitle);

                }
                else
                {
                    MessageBox.Show(DHClient.LastOperationInfo.ToString(pErrInfoFormatStyle), pMsgTitle);
                }
                btnDownLoad2.Tag = "";
                psbMain.Maximum = 100;
                psbMain.Value = 0;
            }
            else
            {
                MessageBox.Show("当前没有下载任务！", pMsgTitle);
            }
        }
        /// <summary>
        /// 获取按时间下载的下载进度按钮按下处理
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnGetDownLoadPos2_Click(object sender, EventArgs e)
        {
            if (btnDownLoad2.Tag.ToString().Equals("下载中"))
            {
                int dwTotal = 0;
                int dwSize = 0;
                bool blnGetPosSucced = DHClient.DHGetDownloadPos(pDownloadHandleByTime, out dwTotal, out dwSize);
                if (blnGetPosSucced)
                {
                    MessageBox.Show("下载统计:\n\t总长度:" + dwTotal.ToString() + "\n\t己下载:" + dwSize.ToString(), pMsgTitle);
                }
                else
                {
                    MessageBox.Show(DHClient.LastOperationInfo.ToString(pErrInfoFormatStyle), pMsgTitle);
                }
            }
            else
            {
                MessageBox.Show("当前没有下载任务！", pMsgTitle);
            }
        }
        /// <summary>
        /// 保存目录选择按钮按下
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnDirSelect_Click(object sender, EventArgs e)
        {
            if (fbdMain.ShowDialog() == DialogResult.OK)
            {
                txtDirPath2.Text = fbdMain.SelectedPath;
            }

        }
    }
}