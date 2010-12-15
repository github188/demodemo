﻿/*
 * ************************************************************************
 *                            SDK
 *                      大华网络SDK(C#版)
 * 
 * (c) Copyright 2007, ZheJiang Dahua Technology Stock Co.Ltd.
 *                      All Rights Reserved
 * 版 本 号:0.01
 * 文件名称:DaHuaSDK.cs
 * 功能说明:在现有的SDK(C++版)上再一次封装，针对C#应用开发
 * 作    者:李德明
 * 作成日期:2007/11/21
 * 修改日志:    日期        版本号      作者        变更事由
 *              2007/11/21  1.0         李德明      新建作成
 * 
 * ************************************************************************
 */

using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;
using System.ComponentModel;
using System.Net;

namespace DHNetSDK.CS
{
    /// <summary>
    /// 本类是在大华网络SDK(C++版)的基础上针对C#应用程序开发的开发包，使用此开
    /// 发包时需要将原有的SDK的动态链接库文件和本SDK文件放置在同一个目录下
    /// </summary>
    public class Client
    {
        #region << 类定义代码 >>

        #region << 常量、变量定义 >>

        #region << 变量>>

        #region << 临时使用 >>

        ///// <summary>
        ///// **临时**实时监视句柄,后面将整合到Channel信息中
        ///// </summary>
        //private int pRealHandle;
        ///// <summary>
        ///// **临时**网络回放句柄,后面将整合到Channel信息中
        ///// </summary>
        //private long pPlayBackHandle;

        #endregion

        /// <summary>
        /// 初始化操作完成标志
        /// </summary>
        private static bool initialized;

        /// <summary>
        /// SDK版本号使用的私有变量
        /// </summary>
        private string pSDKVersion;

        /// <summary>
        /// 设备用户基本信息使用的私有变量
        /// </summary>
        private CLIENT_INFO pClientInfo;
        
        /// <summary>
        /// 设备用户登录标志
        /// </summary>
        private bool pLogined=false;

        /// <summary>
        /// SDK使用最后一次错误信息
        /// </summary>
        private ErrInfo pLastError;

        #endregion

        #endregion

        #region << 属性 >>

        /// <summary>
        /// SDK版本号(只读属性)
        /// </summary>
        public string SDKVersion
        {
            get
            {
                return pSDKVersion;
            }
            //set
            //{
            //    pSDKVersion = value;
            //}
        }

        /// <summary>
        /// SDK使用最后一次错误信息
        /// </summary>
        public ErrInfo LastError
        {
            get
            {
                return pLastError;
            }
            //set
            //{
            //    pLastError = value;
            //}
        }

        /// <summary>
        /// 设备用户信息(只写属性)
        /// </summary>
        public CLIENT_INFO ClientInfo
        {
            //get
            //{
            //    return pClientInfo;
            //}
            set
            {
                pClientInfo = value;
            }
        }

        /// <summary>
        /// 设备用户登录标志:true己登录;false未登录
        /// </summary>
        public bool Logined
        {
            get
            {
                return pLogined;
            }
        }

        #endregion

        #region << 构造函数 >>

        /// <summary>
        /// 构造函数:
        /// ·对开发包的版本号的取得;
        /// ·初始化数字录像机;
        /// </summary>
        public Client()
        {
            pSDKVersion = "1.0";//**临时**使用1.0代替，正确使用是由现有的SDK的接口取得
            pClientInfo = new CLIENT_INFO();
            Init();//初始化数字录像机
        }

        #endregion

        #region << 方法 >>

        #region << 错误处理函数 >>

        /// <summary>
        /// 大华SDK调用失败时时抛出异常。
        /// </summary>
        /// <exception cref="Win32Exception">数字录像机原生异常,当SDK调用失败时触发。</exception>
        private  void ThrowLastError()
        {
            Int32 errorCode = CLIENT_GetLastError();
            if (errorCode != 0)
            {
                pLastError.errCode = errorCode.ToString();
                pLastError.errMessage = GetLastErrorName((uint)errorCode);
                throw new Win32Exception(errorCode, pLastError.errMessage);
            }
            else
            {
                pLastError.errCode = "0";
                pLastError.errMessage = "最近操作没有异常发生";
            }
        }

        /// <summary>
        /// 错误代码转换成标准备的文字信息
        /// </summary>
        /// <param name="errorCode">错误代码</param>
        /// <returns></returns>
        private static string GetLastErrorName(uint errorCode)
        {
            switch (errorCode)
            {
                case 0x80000000 | 1:
                    return "Windows系统出错";
                case 0x80000000 | 2:
                    return "网络错误，可能是因为网络超时";
                case 0x80000000 | 3:
                    return "设备协议不匹配";
                case 0x80000000 | 4:
                    return "句柄无效";
                case 0x80000000 | 5:
                    return "打开通道失败";
                case 0x80000000 | 6:
                    return "关闭通道失败";
                case 0x80000000 | 7:
                    return "用户参数不合法";
                case 0x80000000 | 8:
                    return "SDK初始化出错";
                case 0x80000000 | 9:
                    return "SDK清理出错";
                case 0x80000000 | 10:
                    return "申请render资源出错";
                case 0x80000000 | 11:
                    return "打开解码库出错";
                case 0x800000 | 12:
                    return "关闭解码库出错";
                case 0x80000000 | 13:
                    return "多画面预览中检测到通道数为0";
                case 0x80000000 | 14:
                    return "录音库初始化失败";
                case 0x80000000 | 15:
                    return "录音库未经初始化";
                case 0x80000000 | 16:
                    return "发送音频数据出错";
                case 0x80000000 | 17:
                    return "实时数据已经处于保存状态";
                case 0x80000000 | 18:
                    return "未保存实时数据";
                case 0x80000000 | 19:
                    return "打开文件出错";
                case 0x80000000 | 20:
                    return "启动云台控制定时器失败";
                case 0x80000000 | 21:
                    return "对返回数据的校验出错";
                case 0x80000000 | 22:
                    return "没有足够的缓存";
                case 0x80000000 | 23:
                    return "当前SDK未支持该功能";
                case 0x80000000 | 24:
                    return "查询不到录像";
                case 0x80000000 | 25:
                    return "无操作权限";
                case 0x80000000 | 26:
                    return "暂时无法执行";
                case 0x80000000 | 27:
                    return "未发现对讲通道";
                case 0x80000000 | 28:
                    return "未发现音频";
                case 0x80000000 | 29:
                    return "CLientSDK未经初始化";
                case 0x80000000 | 30:
                    return "下载已结束";
                case 0x80000000 | 31:
                    return "查询结果为空";
                case 0x80000000 | 32:
                    return "获取配置失败位置：系统属性";
                case 0x80000000 | 33:
                    return "获取配置失败位置：序列号";
                case 0x80000000 | 34:
                    return "获取配置失败位置：常规属性";
                case 0x80000000 | 35:
                    return "获取配置失败位置：DSP能力描述";
                case 0x80000000 | 36:
                    return "获取配置失败位置：网络属性";
                case 0x80000000 | 37:
                    return "获取配置失败位置：通道名称";
                case 0x80000000 | 38:
                    return "获取配置失败位置：视频属性";
                case 0x80000000 | 39:
                    return "获取配置失败位置：录像定时配置";
                case 0x80000000 | 40:
                    return "获取配置失败位置：解码器协议名称";
                case 0x80000000 | 41:
                    return "获取配置失败位置：232串口功能名称";
                case 0x80000000 | 42:
                    return "获取配置失败位置：解码器属性";
                case 0x80000000 | 43:
                    return "获取配置失败位置：232串口属性";
                case 0x80000000 | 44:
                    return "取配置失败位置：外部报警输入属性";
                case 0x80000000 | 45:
                    return "获取配置失败位置：图像检测报警属性";
                case 0x80000000 | 46:
                    return "获取配置失败位置：设备时间";
                case 0x80000000 | 47:
                    return "获取配置失败位置：预览参数";
                case 0x80000000 | 48:
                    return "获取配置失败位置：自动维护配置";
                case 0x80000000 | 49:
                    return "获取配置失败位置：视频矩阵配置";
                case 0x80000000 | 55:
                    return "设置配置失败位置：常规属性";
                case 0x80000000 | 56:
                    return "设置配置失败位置：网络属性";
                case 0x80000000 | 57:
                    return "设置配置失败位置：通道名称";
                case 0x80000000 | 58:
                    return "设置配置失败位置：视频属性";
                case 0x80000000 | 59:
                    return "设置配置失败位置：录像定时配置";
                case 0x80000000 | 60:
                    return "设置配置失败位置：解码器属性";
                case 0x80000000 | 61:
                    return "设置配置失败位置：232串口属性";
                case 0x80000000 | 62:
                    return "设置配置失败位置：外部报警输入属性";
                case 0x80000000 | 63:
                    return "设置配置失败位置：图像检测报警属性";
                case 0x80000000 | 64:
                    return "设置配置失败位置：设备时间";
                case 0x80000000 | 65:
                    return "设置配置失败位置：预览参数";
                case 0x80000000 | 66:
                    return "设置配置失败位置：自动维护配置";
                case 0x80000000 | 67:
                    return "设置配置失败位置：视频矩阵配置";
                case 0x80000000 | 70:
                    return "音频编码接口没有成功初始化";
                case 0x80000000 | 71:
                    return "数据过长";
                case 0x80000000 | 72:
                    return "设备不支持该操作";
                case 0x80000000 | 73:
                    return "设备资源不足";
                case 0x80000000 | 74:
                    return "服务器已经启动";
                case 0x80000000 | 75:
                    return "服务器尚未成功启动";
                case 0x80000000 | 80:
                    return "输入序列号有误";
                case 0x80000000 | 100:
                    return "密码不正确";
                case 0x80000000 | 101:
                    return "帐户不存在";
                case 0x80000000 | 102:
                    return "等待登录返回超时";
                case 0x80000000 | 103:
                    return "帐号已登录";
                case 0x80000000 | 104:
                    return "帐号已被锁定";
                case 0x80000000 | 105:
                    return "帐号已被列为黑名单";
                case 0x80000000 | 106:
                    return "资源不足，系统忙";
                case 0x80000000 | 107:
                    return "连接主机失败";
                case 0x80000000 | 108:
                    return "网络连接失败";
                case 0x80000000 | 120:
                    return "Render库打开音频出错";
                case 0x80000000 | 121:
                    return "Render库关闭音频出错";
                case 0x80000000 | 122:
                    return "Render库控制音量出错";
                case 0x80000000 | 123:
                    return "Render库设置画面参数出错";
                case 0x80000000 | 124:
                    return "Render库暂停播放出错";
                case 0x80000000 | 125:
                    return "Render库抓图出错";
                case 0x80000000 | 126:
                    return "Render库步进出错";
                case 0x80000000 | 127:
                    return "Render库设置帧率出错";
                case 0x80000000 | 999:
                    return "暂时无法设置";
                case 0x80000000 | 1000:
                    return "配置数据不合法";
                default:
                    return "未知错误";
            }
        }

        #endregion

        /// <summary>
        /// 初始化数字录像机
        /// </summary>
        public  void Init()
        {
            if (initialized==false)
            {
                CLIENT_Init(null, IntPtr.Zero);
                ThrowLastError();
                initialized = true;
            }
        }

        /// <summary>
        /// 设备用户登录
        /// </summary>
        /// <param name="ip">ip地址。</param>
        /// <param name="port">端口号。</param>        
        /// <returns>登录Id,失败返回0,否则返回Id。</returns>
        public bool Login(string ip, int port)
        {
            pClientInfo.DeviceIP = ip;
            pClientInfo.DevicePort = port;
            return Login();
        }

        /// <summary>
        /// 设备用户登录
        /// </summary>
        /// <param name="userName">设备用户名</param>
        /// <param name="userPassword">设备用户密码</param> 
        /// <param name="ip">ip地址。</param>
        /// <param name="port">端口号。</param>        
        /// <returns>登录Id,失败返回0,否则返回Id。</returns>
        public bool Login(string userName,string userPassword, string ip, int port)
        {
            pClientInfo.Name = userName;
            pClientInfo.Password = userPassword;
            pClientInfo.DeviceIP = ip;
            pClientInfo.DevicePort = port;
            return Login();
        }

        /// <summary>
        /// 设备用户登录
        /// </summary>
        public bool Login()
        {
            bool returnValue = false;
            if (initialized == true && pLogined==false)//己初始化但未登录过
            {
                NET_DEVICEINFO sdkInfo;                
                int error;
                int result = CLIENT_Login(pClientInfo.DeviceIP, (ushort)pClientInfo.DevicePort, pClientInfo.Name, pClientInfo.Password, out sdkInfo, out error);
                ThrowLastError();
                if (result != 0)
                {
                    pLogined = true;
                    pClientInfo.ID = result;
                    pClientInfo.DeviceAlarmInPortCount = sdkInfo.byAlarmInPortNum;
                    pClientInfo.DeviceAlarmOutPortCount = sdkInfo.byAlarmOutPortNum;
                    pClientInfo.DeviceChannelCount = sdkInfo.byChanNum;
                    pClientInfo.DeviceDiskCount = sdkInfo.byDiskNum;
                    pClientInfo.DeviceType = sdkInfo.byDVRType;
                    returnValue = true;
                    if (pClientInfo.ChannelInit() == false)
                    {
                        pClientInfo.Channel = null;
                    }
                    //else
                    //{
                    //    for (int i = 0; i < sdkInfo.byChanNum; i++)
                    //    {

                    //    }

                    //}
                }
                else
                {
                    pLogined = false;
                    pClientInfo.ID = 0;
                    pClientInfo.DeviceAlarmInPortCount = 0;
                    pClientInfo.DeviceAlarmOutPortCount = 0;
                    pClientInfo.DeviceChannelCount = 0;
                    pClientInfo.DeviceDiskCount = 0;
                    pClientInfo.DeviceType = 0;
                    returnValue = false;
                }
            }
            return returnValue;
        }
        
        /// <summary>
        /// 注销用户
        /// </summary>        
        public void Logout()
        {
            if (pLogined == true)//己经登录过的设备用户
            {
                CLIENT_Logout(pClientInfo.ID);
                pLogined = false;
                pClientInfo = new CLIENT_INFO();
                ThrowLastError();
            }
        }

        /// <summary>
        /// 清理SDK环境,当程序结束前执行。
        /// </summary>
        public void Cleanup()
        {
            CLIENT_Cleanup();
            ThrowLastError();
        }

        #region << 实时监视 >>

        /// <summary>
        /// 实时监视开始
        /// </summary>
        /// <param name="intChannelID">通道ID</param>
        /// <param name="hWnd">播放容器句柄</param>
        /// <returns></returns>
        public int RealPlay(int intChannelID, IntPtr hWnd)
        {
            int returnValue;
            if (intChannelID >= pClientInfo.DeviceChannelCount)
            {
                returnValue = 0;
            }
            else
            {
                returnValue = CLIENT_RealPlay(pClientInfo.ID, intChannelID, hWnd);
                ThrowLastError();
                pClientInfo.Channel[intChannelID].RealPlayHandle = returnValue;//通道对应的实时监视句柄保存
            }
            if (returnValue != 0)
            {
                pClientInfo.Channel[intChannelID].blnRealPlay = true;
            }
            else
            {
                pClientInfo.Channel[intChannelID].blnRealPlay = false;
            }
            return returnValue;
        }

        /// <summary>
        /// 结束实时监控
        /// </summary>
        /// <param name="intChannelID">通道ID</param>
        /// <returns>True:成功;False:失败</returns>
        public bool StopRealPlay(int intChannelID)
        {
            bool returnValue = true;
            if (pLogined == true)//判断是否己经登录
            {
                //returnValue = CLIENT_StopRealPlay(pRealHandle);//**临时**实时监视句柄,后面将整合到Channel信息中
                if (pClientInfo.DeviceChannelCount >= intChannelID + 1)//判断是否是有效通道
                {
                    if (pClientInfo.Channel[intChannelID].blnRealPlay == true)//判断该通道是否实时监视着
                    {
                        returnValue = CLIENT_StopRealPlay(pClientInfo.Channel[intChannelID].RealPlayHandle);
                        ThrowLastError();
                    }
                }
            }
            return returnValue;
        }

        #endregion

        #region << 回放和下载 >>

        /// <summary>
        /// 查询录像文件
        /// </summary>
        /// <param name="nChannelId">通道ID</param>
        /// <param name="nRecordFileType">录像文像类型:0.所有录像文件;1.外部报警;2.动态检测报警;3.所有报警;4.卡号查询;5.组合条件查询;</param>
        /// <param name="tmStart">录像开始时间</param>
        /// <param name="tmEnd">录像结束时间</param>
        /// <param name="pchCardid">卡号,只针对卡号查询有效，其他情况下可以填NULL</param>
        /// <param name="nriFileinfo">返回的录像文件信息，是一个NET_RECORDFILE_INFO结构数组</param>
        /// <param name="maxlen">nriFileinfo缓冲的最大长度;（单位字节，建议在100-200*sizeof(NET_RECORDFILE_INFO)之间）</param>
        /// <param name="filecount">返回的文件个数,属于输出参数最大只能查到缓冲满为止的录像记录</param>        
        /// <param name="waittime">等待时间</param>
        /// <param name="bTime">是否按时间查(目前无效)</param>
        /// <returns>成功返回TRUE，失败返回FALSE</returns>
        public bool QueryRecordFile(int nChannelId, int nRecordFileType, DateTime tmStart, DateTime tmEnd, string pchCardid, out NET_RECORDFILE_INFO nriFileinfo, int maxlen,out int filecount, int waittime, bool bTime)
        {
            bool returnValue = false;
            nriFileinfo = new NET_RECORDFILE_INFO();
            filecount = 0;
            if (tmStart.Date >= tmEnd.Date)
            {
                return returnValue;
            }
            NET_TIME netStartTime = ToNetTime(tmStart);
            NET_TIME netEndTime = ToNetTime(tmEnd);
            returnValue = CLIENT_QueryRecordFile(pClientInfo.ID, nChannelId, nRecordFileType, ref netStartTime, ref netEndTime, pchCardid, ref nriFileinfo, maxlen, ref  filecount, waittime, bTime);
            ThrowLastError();
            return returnValue;
        }

        /// <summary>
        /// 网络回放[注意:用户登录一台设备后，每通道同一时间只能播放一则录像,不能同时播放同一通道的多条记录]
        /// </summary>
        /// <param name="lpRecordFile">录像文件信息, 当按时间播放是只需填充起始时间和结束时间, 其他项填0</param>
        /// <param name="hWnd">回放容器名柄</param>
        /// <param name="cbDownLoadPos">进度回调函数</param>
        /// <param name="dwUserData">用户自定义数据</param>
        /// <returns>成功返回网络回放ID，失败返回0</returns>
        public long PlayBackByRecordFile(NET_RECORDFILE_INFO lpRecordFile, IntPtr hWnd, fDownLoadPosCallBack cbDownLoadPos, IntPtr dwUserData)
        {
            long returnValue;
            returnValue = CLIENT_PlayBackByRecordFile(pClientInfo.ID, ref lpRecordFile, hWnd, cbDownLoadPos, dwUserData);
            ThrowLastError();
            return returnValue;
        }

        /// <summary>
        /// 按时间范围网络回放[注意:用户登录一台设备后，每通道同一时间只能播放一则录像,不能同时播放同一通道的多条记录]
        /// </summary>
        /// <param name="nChannelID">通道</param>
        /// <param name="tmStart">播放开始时间</param>
        /// <param name="tmEnd">播放结束时间</param>
        /// <param name="hWnd">回放容器句柄</param>
        /// <param name="cbDownLoadPos">进度回调函数</param>
        /// <param name="dwUserData">用户自定义数据</param>
        /// <returns>成功返回网络回放ID，失败返回0</returns>
        public bool PlayBackByTime(int nChannelID, DateTime tmStart, DateTime tmEnd, IntPtr hWnd, fDownLoadPosCallBack cbDownLoadPos, IntPtr dwUserData)
        {
            bool returnValue=false;
            NET_TIME netStartTime = ToNetTime(tmStart);
            NET_TIME netEndTime = ToNetTime(tmEnd);
            int playHandel = CLIENT_PlayBackByTime(pClientInfo.ID,nChannelID,ref netStartTime,ref netEndTime,hWnd,cbDownLoadPos, dwUserData);
            if (playHandel != 0)
            {
                pClientInfo.Channel[nChannelID].blnPlayBack = true;
                pClientInfo.Channel[nChannelID].PlayBackHandle = playHandel;
                returnValue = true;
            }
            ThrowLastError();
            return returnValue;
        }

        ///// <summary>
        ///// 按时间回放。
        ///// </summary>        
        ///// <param name="channelId">通道号。</param>
        ///// <param name="startTime">起始时间。</param>
        ///// <param name="endTime">结束时间。</param>
        ///// <param name="hWnd">窗口句柄。</param>
        ///// <param name="downLoadPositonCallback">播放进度回调。</param>
        ///// <returns>返回playHandle。</returns>
        //public bool PlayBackByTime( int channelId, DateTime startTime,
        //                                 DateTime endTime, IntPtr hWnd, fDownLoadPosCallBack downLoadPositonCallback)
        //{

        //    NET_TIME netStartTime = ToNetTime(startTime);
        //    NET_TIME netEndTime = ToNetTime(endTime);
        //    int result =
        //        CLIENT_PlayBackByTime(pClientInfo.ID, channelId, ref netStartTime, ref netEndTime, hWnd,
        //                              downLoadPositonCallback, IntPtr.Zero);
        //    ThrowLastError();
        //    return result;
        //}

        /// <summary>
        /// 网络回放控制
        /// </summary>
        /// <param name="nChannelID">通道</param>
        /// <param name="playControl">播放控制命令</param>
        /// <returns>成功返回TRUE，失败返回FALSE</returns>
        public bool PlayBackControl(int nChannelID, PlayCon playControl)
        {
            bool returnValue = false;
            switch (playControl)
            {
                case PlayCon.Play://播放
                    returnValue=CLIENT_PausePlayBack(pClientInfo.Channel[nChannelID].PlayBackHandle, 0);
                    break;
                case PlayCon.Pause://暂停
                    returnValue=CLIENT_PausePlayBack(pClientInfo.Channel[nChannelID].PlayBackHandle, 1);
                    break;
                case PlayCon.Slow://慢放
                    returnValue = CLIENT_SlowPlayBack(pClientInfo.Channel[nChannelID].PlayBackHandle);
                    break;
                case PlayCon.Fast://快进
                    returnValue = CLIENT_FastPlayBack(pClientInfo.Channel[nChannelID].PlayBackHandle);
                    break;
                case PlayCon.StepPlay://单步播放开始
                    returnValue = CLIENT_StepPlayBack(pClientInfo.Channel[nChannelID].PlayBackHandle, false);
                    break;
                case PlayCon.StepStop://单步播放停止
                    returnValue = CLIENT_StepPlayBack(pClientInfo.Channel[nChannelID].PlayBackHandle, true);
                    break;
                case PlayCon.Stop://停止播放
                    returnValue = CLIENT_StopPlayBack(pClientInfo.Channel[nChannelID].PlayBackHandle);
                    break;
                default:
                    returnValue = false;
                    break;
            }
            return returnValue;
        }

        /// <summary>
        /// 拖动播放操作
        /// </summary>
        /// <param name="nChannelID">通道</param>
        /// <param name="playControl">播放控制命令</param>
        /// <param name="seekNum">相对文件开始处的偏移值</param>
        /// <returns></returns>
        public bool PlayBackControl(int nChannelID, PlayCon playControl,uint seekNum)
        { 
            bool returnValue = false;
            uint uTemp = 0xffffffff;
            switch (playControl)
            {  
                case PlayCon.SeekByTime://拖动播放[按时间偏移]
                    returnValue = CLIENT_SeekPlayBack(pClientInfo.Channel[nChannelID].PlayBackHandle, seekNum, uTemp);
                    break;
                case PlayCon.SeekByBit://拖动播放[按字节偏移]
                    returnValue = CLIENT_SeekPlayBack(pClientInfo.Channel[nChannelID].PlayBackHandle, uTemp, seekNum);
                    break;
                default:
                    returnValue = false;
                    break;
            }
            return returnValue;
        }

        #endregion

        #region << 云台控制 >>

        /// <summary>
        /// 云台控制
        /// </summary>
        /// <param name="nChannelID">通道ID</param>
        /// <param name="dwPTZCommand">云台控制命令</param>
        /// <param name="param1">参数1</param>
        /// <param name="param2">参数2</param>
        /// <param name="param3">参数3</param>
        /// <param name="dwStop">是否停止</param>
        /// <returns>是否停止</returns>
        public bool PTZControl(int nChannelID, PTZControlType dwPTZCommand,char param1, char param2, char param3, bool dwStop)
        { 
            bool returnValue=false;
            switch (dwPTZCommand)
            {
                case PTZControlType.EXTPTZ_FASTGOTO://快速定位
                    CLIENT_DHPTZControl(pClientInfo.ID, nChannelID,(int) dwPTZCommand, param1, param2, param3, dwStop);
                    break;
            }
            return returnValue;
        }

        /// <summary>
        /// 云台控制
        /// </summary>
        /// <param name="nChannelID">通道ID</param>
        /// <param name="dwPTZCommand">云台控制命令</param>
        /// <param name="param1">参数1</param>
        /// <param name="param2">参数2</param>
        /// <param name="dwStop">是否停止</param>
        /// <returns>是否停止</returns>
        public bool PTZControl(int nChannelID, PTZControlType dwPTZCommand, char param1, char param2, bool dwStop)
        {
            bool returnValue = false;
            switch (dwPTZCommand)
            {
                case PTZControlType.EXTPTZ_LEFTTOP://左上
                case PTZControlType.EXTPTZ_RIGHTTOP://右上
                case PTZControlType.EXTPTZ_LEFTDOWN://左下
                case PTZControlType.EXTPTZ_RIGHTDOWN://右下
                case PTZControlType.EXTPTZ_ADDTOLOOP://加入预置点到巡航
                case PTZControlType.EXTPTZ_DELFROMLOOP://删除巡航中预置点
                case PTZControlType.EXTPTZ_AUXIOPEN://x34辅助开关开
                case PTZControlType.EXTPTZ_AUXICLOSE://0x35辅助开关关
                    CLIENT_DHPTZControl(pClientInfo.ID, nChannelID,(int) dwPTZCommand, param1, param2, ' ', dwStop);
                    break;
            }
            return returnValue;
        }

        /// <summary>
        /// 云台控制
        /// </summary>
        /// <param name="nChannelID">通道ID</param>
        /// <param name="dwPTZCommand">云台控制命令</param>
        /// <param name="param1">参数1</param>
        /// <param name="dwStop">是否停止</param>
        /// <returns>是否停止</returns>
        public bool PTZControl(int nChannelID, PTZControlType dwPTZCommand, char param1, bool dwStop)
        {
            bool returnValue = false;
            return returnValue;
        }

        #endregion

        #endregion

        #endregion

        #region << 原有SDK(C++版)调用代码 >>

        [DllImport("dhnetsdk.dll")]
        private static extern Int32 CLIENT_Init(fDisConnect cbDisConnect, IntPtr dwUser);

        [DllImport("dhnetsdk.dll")]
        private static extern Int32 CLIENT_Login(string pchDVRIP, ushort wDVRPort, string pchUserName,
                                                 string pchPassword, out NET_DEVICEINFO lpDeviceInfo, out int error);

        [DllImport("dhnetsdk.dll")]
        private static extern Int32 CLIENT_StartListen(Int32 lLoginID);

        [DllImport("dhnetsdk.dll")]
        private static extern Int32 CLIENT_StopListen(Int32 lLoginID);

        [DllImport("dhnetsdk.dll")]
        private static extern Int32 CLIENT_Logout(Int32 lLoginID);

        [DllImport("dhnetsdk.dll")]
        private static extern int CLIENT_DownloadByTime(int lLoginID, int nChannelId, int nRecordFileType,
                                                        ref NET_TIME tmStart,
                                                        ref NET_TIME tmEnd, string sSavedFileName,
                                                        fTimeDownLoadPosCallBack cbTimeDownLoadPos, IntPtr dwUserData);

        [DllImport("dhnetsdk.dll")]
        private static extern int CLIENT_StopDownload(int lFileHandle);



        [DllImport("dhnetsdk.dll")]
        private static extern int CLIENT_PlayBackByTimeEx(int lLoginID, int nChannelID, ref NET_TIME lpStartTime,
                                                          ref NET_TIME lpStopTime, IntPtr hWnd,
                                                          fDownLoadPosCallBack cbDownLoadPos, int dwPosUser,
                                                          fDataCallBack fDownLoadDataCallBack, IntPtr dwDataUser);

        [DllImport("dhnetsdk.dll")]
        private static extern int CLIENT_OpenSound(int hPlayHandle);

        #region << 实时监视接口 >>

        /// <summary>
        /// 启动实时监视
        /// </summary>
        /// <param name="lLoginID">设备用户登录句柄</param>
        /// <param name="nChannelID">通道ID</param>
        /// <param name="hWnd">监视播放容器句柄</param>
        /// <returns>失败返回0，成功返回实时监视ID(实时监视句柄)，将作为相关函数的参数</returns>
        [DllImport("dhnetsdk.dll")]
        private static extern int CLIENT_RealPlay(int lLoginID, int nChannelID, IntPtr hWnd);        

        /// <summary>
        /// 停止实时监视
        /// </summary>
        /// <param name="lRealHandle">实时监视句柄</param>
        /// <returns>成功返回TRUE，失败返回FALSE</returns>
        [DllImport("dhnetsdk.dll")]
        private static extern bool CLIENT_StopRealPlay(int lRealHandle);
        
        #endregion


        #region << 回放和下载接口 >>
        
        /// <summary>
        /// 查询录像文件
        /// </summary>
        /// <param name="lLoginID">设备用户登录句柄</param>
        /// <param name="nChannelId">通道ID</param>
        /// <param name="nRecordFileType">录像文件类型 </param>
        /// <param name="tmStart">录像开始时间</param>
        /// <param name="tmEnd">录像结束时间</param>
        /// <param name="pchCardid">卡号,只针对卡号查询有效，其他情况下可以填NULL</param>
        /// <param name="nriFileinfo">返回的录像文件信息，是一个NET_RECORDFILE_INFO结构数组</param>
        /// <param name="maxlen">nriFileinfo缓冲的最大长度;（单位字节，建议在100-200*sizeof(NET_RECORDFILE_INFO)之间）</param>
        /// <param name="filecount">返回的文件个数,属于输出参数最大只能查到缓冲满为止的录像记录</param>
        /// <param name="waittime">等待时间</param>
        /// <param name="bTime">是否按时间查(目前无效)</param>
        /// <returns>成功返回TRUE，失败返回FALSE</returns>
        [DllImport("dhnetsdk.dll")]
        private static extern bool CLIENT_QueryRecordFile(int lLoginID, int nChannelId, int nRecordFileType, ref NET_TIME tmStart,ref NET_TIME tmEnd, string pchCardid, ref NET_RECORDFILE_INFO nriFileinfo, int maxlen, ref  int filecount, int waittime, bool bTime);
                                   
        /// <summary>
        /// 网络回放[注意:用户登录一台设备后，每通道同一时间只能播放一则录像,不能同时播放同一通道的多条记录]
        /// </summary>
        /// <param name="lLoginID">设备用户登录句柄</param>
        /// <param name="lpRecordFile">录像文件信息, 当按时间播放是只需填充起始时间和结束时间, 其他项填0</param>
        /// <param name="hWnd">回放容器句柄</param>
        /// <param name="cbDownLoadPos">进度回调函数</param>
        /// <param name="dwUserData">用户自定义数据</param>
        /// <returns>成功返回网络回放ID，失败返回0</returns>
        [DllImport("dhnetsdk.dll")]
        private static extern long CLIENT_PlayBackByRecordFile(int lLoginID,ref NET_RECORDFILE_INFO lpRecordFile, IntPtr hWnd, fDownLoadPosCallBack cbDownLoadPos, IntPtr dwUserData);

        /// <summary>
        /// 网络按时间回放
        /// </summary>
        /// <param name="lLoginID">设备用户登录句柄</param>
        /// <param name="nChannelID">通道ID</param>
        /// <param name="lpStartTime">录像开始时间</param>
        /// <param name="lpStopTime">录像结束时间</param>
        /// <param name="hWnd">录像回放容器句柄</param>
        /// <param name="cbDownLoadPos">进度回调函数</param>
        /// <param name="dwUserData">用户自定义数据</param>
        /// <returns>成功返回网络回放ID，失败返回0</returns>
        [DllImport("dhnetsdk.dll")]
        private static extern int CLIENT_PlayBackByTime(int lLoginID, int nChannelID, ref NET_TIME lpStartTime,
                                                        ref NET_TIME lpStopTime, IntPtr hWnd,
                                                        fDownLoadPosCallBack cbDownLoadPos,
                                                        IntPtr dwUserData);
        /// <summary>
        /// 网络回放停止
        /// </summary>
        /// <param name="lPlayHandle">回放句柄</param>
        /// <returns>成功返回TRUE，失败返回FALSE</returns>
        [DllImport("dhnetsdk.dll")]
        private static extern bool CLIENT_StopPlayBack(int lPlayHandle);

        /// <summary>
        /// 网络回放暂停与恢复播放
        /// </summary>
        /// <param name="lPlayHandle">播放句柄</param>
        /// <param name="bPause">1:暂停;0:恢复</param>
        /// <returns>成功返回TRUE，失败返回FALSE</returns>
        [DllImport("dhnetsdk.dll")]
        private static extern bool CLIENT_PausePlayBack(int lPlayHandle, int bPause);

        /// <summary>
        /// 改变位置播放[即拖动播放，只有打开图像的函数参数hWnd有效时该函数获取的参数才有效，否则无意义]
        /// </summary>
        /// <param name="lPlayHandle">播放句柄</param>
        /// <param name="offsettime">相对文件开始处偏移时间，单位为秒 .当其值为0xffffffff时,该参数无效.</param>
        /// <param name="offsetbyte">相对文件开始处偏移字节, 当其值为0xffffffff时, 该参数无效；当offsettime有效的时候,此参数无意义</param>
        /// <returns>成功返回TRUE，失败返回FALSE</returns>
        [DllImport("dhnetsdk.dll")]
        private static extern bool CLIENT_SeekPlayBack(int lPlayHandle, uint offsettime, uint offsetbyte);
        
        

        [DllImport("dhnetsdk.dll")]
        private static extern bool CLIENT_StepPlayBack(int lPlayHandle, bool bStop);

        [DllImport("dhnetsdk.dll")]
        private static extern bool CLIENT_FastPlayBack(int lPlayHandle);

        [DllImport("dhnetsdk.dll")]
        private static extern bool CLIENT_SlowPlayBack(int lPlayHandle);

          
        #endregion

        #region << 云台控制 >>

        /// <summary>
        /// 扩展云台控制
        /// </summary>
        /// <param name="lLoginID">设备用户登录句柄</param>
        /// <param name="nChannelID">通道号</param>
        /// <param name="dwPTZCommand">球机控制命令</param>
        /// <param name="param1">参数1</param>
        /// <param name="param2">参数2</param>
        /// <param name="param3">参数3</param>
        /// <param name="dwStop">是否停止</param>
        /// <returns>成功返回TRUE，失败返回FALSE</returns>
        [DllImport("dhnetsdk.dll")]
        private static extern bool CLIENT_DHPTZControl(int lLoginID, int nChannelID, int dwPTZCommand, char param1, char param2, char param3, bool dwStop);

        #endregion
        [DllImport("dhnetsdk.dll")]
        private static extern int CLIENT_CloseSound();

        [DllImport("dhnetsdk.dll")]
        private static extern int CLIENT_SetVolume(int lPlayHandle, int nVolume);

        
        [DllImport("dhnetsdk.dll")]
        private static extern void CLIENT_Cleanup();

        [DllImport("dhnetsdk.dll")]
        private static extern Int32 CLIENT_GetLastError();

        [DllImport("dhnetsdk.dll")]
        private static extern Int32 CLIENT_SetupDeviceTime(int lLoginID, ref NET_TIME pDeviceTime);

        #endregion

        #region << 委托 >>

        public delegate void fDisConnect(
            Int32 lLoginID, StringBuilder pchDVRIP, Int32 nDVRPort, IntPtr dwUser);

        public delegate void fDownLoadPosCallBack(int lPlayHandle, int dwTotalSize, int dwDownLoadSize, IntPtr dwUser);

        public delegate int fDataCallBack(int lRealHandle, int dwDataType, ref byte pBuffer, int dwBufSize, IntPtr dwUser);

        public delegate void fTimeDownLoadPosCallBack(
            int lPlayHandle, int dwTotalSize, int dwDownLoadSize, int index, NET_RECORDFILE_INFO recordfileinfo, IntPtr dwUser);

        #endregion

        #region << 公共函数定义 >>

        /// <summary>
        /// Windows系统标准时间格式转为自定义格式
        /// </summary>
        /// <param name="dateTime">系统时间对象</param>
        /// <returns>自定义时间格式的时间数据</returns>
        private static NET_TIME ToNetTime(DateTime dateTime)
        {
            NET_TIME result = new NET_TIME();
            result.dwYear = dateTime.Year;
            result.dwMonth = dateTime.Month;
            result.dwDay = dateTime.Day;
            result.dwHour = dateTime.Hour;
            result.dwMinute = dateTime.Minute;
            result.dwSecond = dateTime.Second;
            return result;
        }

        #endregion
    }

    #region << 结构 >>
    /// <summary>
    /// 设备用户信息
    /// </summary>
    public struct CLIENT_INFO
    {
        public int ID;
        public string Name;
        public string Password;
        public string DeviceName;
        public string DeviceIP;
        public byte DeviceAlarmInPortCount;
        public int DevicePort;
        public byte DeviceAlarmOutPortCount;
        public byte DeviceChannelCount;
        public byte DeviceType;
        public byte DeviceDiskCount;
        /// <summary>
        /// 通道信息
        /// </summary>
        public DEVCHANNEL_INFO[] Channel;
        /// <summary>
        /// 通道初期化
        /// </summary>
        /// <returns>true:成功;false:失败</returns>
        public bool ChannelInit()
        {
            try
            {
                Channel = new DEVCHANNEL_INFO[DeviceChannelCount];
            }
            catch
            {
                return false;
            }
            return true;
        }
    }

    /// <summary>
    /// 设备通道信息
    /// </summary>
    public struct DEVCHANNEL_INFO
    {
        /// <summary>
        /// 通道名
        /// </summary>
        public string Name;
        /// <summary>
        /// 通道实时监视标志
        /// </summary>
        public bool blnRealPlay;
        /// <summary>
        /// 实时监视句柄
        /// </summary>
        public int RealPlayHandle;
        /// <summary>
        /// 通道回放标志
        /// </summary>
        public bool blnPlayBack;
        /// <summary>
        /// 通道回放句柄
        /// </summary>
        public int PlayBackHandle;
        /// <summary>
        /// 亮度
        /// </summary>
        public int Bright;
        /// <summary>
        /// 对比度
        /// </summary>
        public int Contrast;
        /// <summary>
        /// 湿度
        /// </summary>
        public int Humidity;

    }

    /// <summary>
    /// 网络设备信息
    /// </summary>
    [StructLayout(LayoutKind.Sequential)]
    public struct NET_DEVICEINFO
    {
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 48)]
        public byte[] sSerialNumber; //序列号
        public byte byAlarmInPortNum; //DVR报警输入个数
        public byte byAlarmOutPortNum; //DVR报警输出个数
        public byte byDiskNum; //DVR硬盘个数
        public byte byDVRType; //DVR类型, 
        public byte byChanNum; //DVR通道个数
    }

    /// <summary>
    /// 网络时间
    /// </summary>
    [StructLayout(LayoutKind.Sequential)]
    public struct NET_TIME
    {
        /// <summary>
        /// 年
        /// </summary>
        public int dwYear;
        /// <summary>
        /// 月
        /// </summary>
        public int dwMonth;
        /// <summary>
        /// 日
        /// </summary>
        public int dwDay;
        /// <summary>
        /// 时
        /// </summary>
        public int dwHour;
        /// <summary>
        /// 分
        /// </summary>
        public int dwMinute;
        /// <summary>
        /// 秒
        /// </summary>
        public int dwSecond;
    }

    ///// <summary>
    ///// 录像文件信息
    ///// </summary>
    //public struct NET_RECORDFILE_INFO
    //{
    //    /// <summary>
    //    /// 通道号
    //    /// </summary>
    //    public uint ch;
    //    /// <summary>
    //    /// 文件名
    //    /// </summary>
    //    [MarshalAs(UnmanagedType.ByValArray,ArraySubType= UnmanagedType.ByValTStr,SizeConst=128)]
    //    public char[] filename;
    //    /// <summary>
    //    /// 文件长度
    //    /// </summary>
    //    public uint size;
    //    /// <summary>
    //    /// 开始时间
    //    /// </summary>
    //    public NET_TIME starttime; 
    //    /// <summary>
    //    /// 结束时间
    //    /// </summary>
    //    public NET_TIME endtime; 
    //    /// <summary>
    //    /// 磁盘号
    //    /// </summary>
    //    public uint driveno; 
    //    /// <summary>
    //    /// 起始簇号
    //    /// </summary>
    //    public uint startcluster;
    //}

    /// <summary>
    /// 错误内容
    /// </summary>
    public struct ErrInfo
    {
        /// <summary>
        /// 错误代码
        /// </summary>
        public string errCode;
        /// <summary>
        /// 错误信息
        /// </summary>
        public string errMessage;
    }
    #endregion

    #region << 枚举定义 >>

    /// <summary>
    /// 播放控制命令
    /// </summary>
    public enum PlayCon
    {
        /// <summary>
        /// 播放
        /// </summary>
        Play,
        /// <summary>
        /// 暂停
        /// </summary>
        Pause,
        /// <summary>
        /// 停止
        /// </summary>
        Stop,
        /// <summary>
        /// 拖动播放[按时间偏移]
        /// </summary>
        SeekByTime,
        /// <summary>
        /// 拖动播放[按字节偏移]
        /// </summary>
        SeekByBit,
        /// <summary>
        /// 单步播放开始[调用一次播放一帧]
        /// </summary>
        StepPlay,
        /// <summary>
        /// 单步播放停止
        /// </summary>
        StepStop,
        /// <summary>
        /// 快放
        /// </summary>
        Fast,
        /// <summary>
        /// 慢放
        /// </summary>
        Slow
    }

    /// <summary>
    /// 云台控制命令
    /// </summary>
    public enum PTZControlType
    {        
        /// <summary>
        /// 上
        /// </summary>
        PTZ_UP_CONTROL = 0,
        /// <summary>
        /// 下
        /// </summary>
        PTZ_DOWN_CONTROL,
        /// <summary>
        /// 左
        /// </summary>
        PTZ_LEFT_CONTROL,
        /// <summary>
        /// 右
        /// </summary>
        PTZ_RIGHT_CONTROL,
        /// <summary>
        /// 变倍+
        /// </summary>
        PTZ_ZOOM_ADD_CONTROL,
        /// <summary>
        /// 变倍-
        /// </summary>
        PTZ_ZOOM_DEC_CONTROL,
        /// <summary>
        /// 调焦+
        /// </summary>
        PTZ_FOCUS_ADD_CONTROL,
        /// <summary>
        /// 调焦- 
        /// </summary>
        PTZ_FOCUS_DEC_CONTROL,
        /// <summary>
        /// 光圈+ 
        /// </summary>
        PTZ_APERTURE_ADD_CONTROL,
        /// <summary>
        /// 光圈- 
        /// </summary>
        PTZ_APERTURE_DEC_CONTROL,
        /// <summary>
        /// 转至预置点 
        /// </summary>
        PTZ_POINT_MOVE_CONTROL,
        /// <summary>
        /// 设置 
        /// </summary>
        PTZ_POINT_SET_CONTROL,
        /// <summary>
        /// 删除 
        /// </summary>
        PTZ_POINT_DEL_CONTROL,
        /// <summary>
        /// 点间轮循 
        /// </summary>
        PTZ_POINT_LOOP_CONTROL,
        /// <summary>
        /// 灯光雨刷	
        /// </summary>
        PTZ_LAMP_CONTROL,
        /// <summary>
        /// 左上:p1水平速度, p2垂直速度
        /// </summary>
        EXTPTZ_LEFTTOP = 0X20,
        /// <summary>
        /// 右上:p1水平速度, p2垂直速度
        /// </summary>
        EXTPTZ_RIGHTTOP,
        /// <summary>
        /// 左下:p1水平速度, p2垂直速度
        /// </summary>
        EXTPTZ_LEFTDOWN,
        /// <summary>
        /// 右下:p1水平速度, p2垂直速度
        /// </summary>
        EXTPTZ_RIGHTDOWN,
        /// <summary>
        /// 加入预置点到巡航:p1巡航线路	p2预置点值
        /// </summary>
        EXTPTZ_ADDTOLOOP,
        /// <summary>
        /// 删除巡航中预置点:p1巡航线路	p2预置点值
        /// </summary>
        EXTPTZ_DELFROMLOOP,				
        /// <summary>
        /// 清除巡航:p1巡航线路	
        /// </summary>
        EXTPTZ_CLOSELOOP,
        /// <summary>
        /// 开始水平旋转
        /// </summary>
        EXTPTZ_STARTPANCRUISE,
        /// <summary>
        /// 停止水平旋转	
        /// </summary>
        EXTPTZ_STOPPANCRUISE,
        /// <summary>
        /// 设置左边界
        /// </summary>
        EXTPTZ_SETLEFTBORDER,
        /// <summary>
        /// 设置右边界		
        /// </summary>
        EXTPTZ_RIGHTBORDER,
        /// <summary>
        /// 开始线扫
        /// </summary>
        EXTPTZ_STARTLINESCAN,
        /// <summary>
        /// 停止线扫
        /// </summary>
        EXTPTZ_CLOSELINESCAN,
        /// <summary>
        /// 设置模式开始:模式线路
        /// </summary>
        EXTPTZ_SETMODESTART,
        /// <summary>
        /// 设置模式结束:模式线路
        /// </summary>
        EXTPTZ_SETMODESTOP,
		/// <summary>
        /// 运行模式:p1模式线路
		/// </summary>		
        EXTPTZ_RUNMODE,
        /// <summary>
        /// 停止模式:p1模式线路
        /// </summary>
        EXTPTZ_STOPMODE,
        /// <summary>
        /// 清除模式:p1模式线路
        /// </summary>
        EXTPTZ_DELETEMODE,
        /// <summary>
        /// 翻转命令
        /// </summary>
        EXTPTZ_REVERSECOMM,
        /// <summary>
        /// 快速定位  p1水平坐标 p2垂直坐标	 p3变倍
        /// </summary>
        EXTPTZ_FASTGOTO,
        /// <summary>
        /// x34:辅助开关开	 p1辅助点		
        /// </summary>
        EXTPTZ_AUXIOPEN,
        /// <summary>
        /// 0x35:辅助开关关	p1辅助点
        /// </summary>
        EXTPTZ_AUXICLOSE,
        /// <summary>
        /// 打开球机菜单
        /// </summary>
        EXTPTZ_OPENMENU = 0X36,
        /// <summary>
        /// 关闭菜单
        /// </summary>
        EXTPTZ_CLOSEMENU,
        /// <summary>
        /// 菜单确定
        /// </summary>
        EXTPTZ_MENUOK,
        /// <summary>
        /// 菜单取消
        /// </summary>
        EXTPTZ_MENUCANCEL,
        /// <summary>
        /// 菜单上
        /// </summary>
        EXTPTZ_MENUUP,
        /// <summary>
        /// 菜单下
        /// </summary>
        EXTPTZ_MENUDOWN,
        /// <summary>
        /// 菜单左
        /// </summary>
        EXTPTZ_MENULEFT,
        /// <summary>
        /// 菜单右
        /// </summary>
        EXTPTZ_MENURIGHT,
        /// <summary>
        /// 最大命令值
        /// </summary>
        EXTPTZ_TOTAL

    }
    #endregion
}

/*
 * ************************************************************************
 *                            SDK
 *                      大华网络SDK(C#版)
 * 
 * (c) Copyright 2007, ZheJiang Dahua Technology Stock Co.Ltd.
 *                      All Rights Reserved
 * 版 本 号:0.01
 * 文件名称:DaHuaSDK.cs
 * 功能说明:原始封装[在现有的SDK(C++版)上再一次封装,基本与原C++接口对应]
 * 作    者:李德明
 * 作成日期:2007/11/26
 * 修改日志:    日期        版本号      作者        变更事由
 *              2007/11/26  0.01        李德明      新建作成
 * 
 * ************************************************************************
 */
namespace DHNetSDK
{
    #region << DHClient类定义 >>

    /// <summary>
    /// 本开发包是在大华网络SDK(C++版)的基础上封装是对原SDK中C++接口转成C#风格的接口的开发包，
    /// 以便于应用程序开发使用。使用此开发包发布应用程序时需要将原有的SDK的动态链接库文件和本
    /// SDK文件放置在同一个目录下
    /// </summary>
    public sealed class DHClient
    {
        private static bool initialized = false;

        /// <summary>
        /// 是否将错误抛出[默认不抛出，只将错误信息返回给属性LastOperationInfo]
        /// </summary>
        private static bool pShowException = false;

        #region << 属性 >>

        /// <summary>
        /// 最后操作信息
        /// </summary>
        private static OPERATION_INFO pErrInfo;

        /// <summary>
        /// 最后操作信息[最后操作错误和成功,该属性只读]
        /// </summary>
        public static OPERATION_INFO LastOperationInfo
        {
            get
            {
                return pErrInfo;
            }
        }

        /// <summary>
        /// 字符编码格式[默认:gb2312 → 936]
        /// </summary>
        private static Encoding pEncoding=Encoding.GetEncoding(936);

        /// <summary>
        /// 字符编码格式
        /// </summary>
        public static Encoding EncodingInfo
        {
            get
            {
                return pEncoding;
            }
            set
            {
                pEncoding = value;
            }
        }
        #endregion

        #region << 类方法 >>

        #region << 私有方法 >>

        /// <summary>
        /// SDK调用失败时抛出异常,成功时返回无异常信息,并把操作信息赋给LastOperationInfo
        /// </summary>
        /// <exception cref="Win32Exception">数字录像机原生异常,当SDK调用失败时触发</exception>
        private static void DHThrowLastError()
        {
            Int32 errorCode = CLIENT_GetLastError();
            if (errorCode != 0)
            {
                pErrInfo.errCode = errorCode.ToString();
                pErrInfo.errMessage = DHGetLastErrorName((uint)errorCode);
                if (pShowException == true)
                {
                    throw new Win32Exception(errorCode, pErrInfo.errMessage);
                }
            }
            else
            {
                pErrInfo.errCode = "0";
                pErrInfo.errMessage = "最近操作无异常发生";
            }
        }

        /// <summary>
        /// 错误代码转换为标准备的错误信息描述
        /// </summary>
        /// <param name="errorCode">错误代码</param>
        /// <returns>标准备错误信息描述</returns>
        private static string DHGetLastErrorName(uint errorCode)
        {
            switch (errorCode)
            {
                case 0x80000000 | 1:
                    return "Windows系统出错";
                case 0x80000000 | 2:
                    return "网络错误，可能是因为网络超时";
                case 0x80000000 | 3:
                    return "设备协议不匹配";
                case 0x80000000 | 4:
                    return "句柄无效";
                case 0x80000000 | 5:
                    return "打开通道失败";
                case 0x80000000 | 6:
                    return "关闭通道失败";
                case 0x80000000 | 7:
                    return "用户参数不合法";
                case 0x80000000 | 8:
                    return "SDK初始化出错";
                case 0x80000000 | 9:
                    return "SDK清理出错";
                case 0x80000000 | 10:
                    return "申请render资源出错";
                case 0x80000000 | 11:
                    return "打开解码库出错";
                case 0x800000 | 12:
                    return "关闭解码库出错";
                case 0x80000000 | 13:
                    return "多画面预览中检测到通道数为0";
                case 0x80000000 | 14:
                    return "录音库初始化失败";
                case 0x80000000 | 15:
                    return "录音库未经初始化";
                case 0x80000000 | 16:
                    return "发送音频数据出错";
                case 0x80000000 | 17:
                    return "实时数据已经处于保存状态";
                case 0x80000000 | 18:
                    return "未保存实时数据";
                case 0x80000000 | 19:
                    return "打开文件出错";
                case 0x80000000 | 20:
                    return "启动云台控制定时器失败";
                case 0x80000000 | 21:
                    return "对返回数据的校验出错";
                case 0x80000000 | 22:
                    return "没有足够的缓存";
                case 0x80000000 | 23:
                    return "当前SDK未支持该功能";
                case 0x80000000 | 24:
                    return "查询不到录像";
                case 0x80000000 | 25:
                    return "无操作权限";
                case 0x80000000 | 26:
                    return "暂时无法执行";
                case 0x80000000 | 27:
                    return "未发现对讲通道";
                case 0x80000000 | 28:
                    return "未发现音频";
                case 0x80000000 | 29:
                    return "CLientSDK未经初始化";
                case 0x80000000 | 30:
                    return "下载已结束";
                case 0x80000000 | 31:
                    return "查询结果为空";
                case 0x80000000 | 32:
                    return "获取配置失败位置：系统属性";
                case 0x80000000 | 33:
                    return "获取配置失败位置：序列号";
                case 0x80000000 | 34:
                    return "获取配置失败位置：常规属性";
                case 0x80000000 | 35:
                    return "获取配置失败位置：DSP能力描述";
                case 0x80000000 | 36:
                    return "获取配置失败位置：网络属性";
                case 0x80000000 | 37:
                    return "获取配置失败位置：通道名称";
                case 0x80000000 | 38:
                    return "获取配置失败位置：视频属性";
                case 0x80000000 | 39:
                    return "获取配置失败位置：录像定时配置";
                case 0x80000000 | 40:
                    return "获取配置失败位置：解码器协议名称";
                case 0x80000000 | 41:
                    return "获取配置失败位置：232串口功能名称";
                case 0x80000000 | 42:
                    return "获取配置失败位置：解码器属性";
                case 0x80000000 | 43:
                    return "获取配置失败位置：232串口属性";
                case 0x80000000 | 44:
                    return "取配置失败位置：外部报警输入属性";
                case 0x80000000 | 45:
                    return "获取配置失败位置：图像检测报警属性";
                case 0x80000000 | 46:
                    return "获取配置失败位置：设备时间";
                case 0x80000000 | 47:
                    return "获取配置失败位置：预览参数";
                case 0x80000000 | 48:
                    return "获取配置失败位置：自动维护配置";
                case 0x80000000 | 49:
                    return "获取配置失败位置：视频矩阵配置";
                case 0x80000000 | 55:
                    return "设置配置失败位置：常规属性";
                case 0x80000000 | 56:
                    return "设置配置失败位置：网络属性";
                case 0x80000000 | 57:
                    return "设置配置失败位置：通道名称";
                case 0x80000000 | 58:
                    return "设置配置失败位置：视频属性";
                case 0x80000000 | 59:
                    return "设置配置失败位置：录像定时配置";
                case 0x80000000 | 60:
                    return "设置配置失败位置：解码器属性";
                case 0x80000000 | 61:
                    return "设置配置失败位置：232串口属性";
                case 0x80000000 | 62:
                    return "设置配置失败位置：外部报警输入属性";
                case 0x80000000 | 63:
                    return "设置配置失败位置：图像检测报警属性";
                case 0x80000000 | 64:
                    return "设置配置失败位置：设备时间";
                case 0x80000000 | 65:
                    return "设置配置失败位置：预览参数";
                case 0x80000000 | 66:
                    return "设置配置失败位置：自动维护配置";
                case 0x80000000 | 67:
                    return "设置配置失败位置：视频矩阵配置";
                case 0x80000000 | 70:
                    return "音频编码接口没有成功初始化";
                case 0x80000000 | 71:
                    return "数据过长";
                case 0x80000000 | 72:
                    return "设备不支持该操作";
                case 0x80000000 | 73:
                    return "设备资源不足";
                case 0x80000000 | 74:
                    return "服务器已经启动";
                case 0x80000000 | 75:
                    return "服务器尚未成功启动";
                case 0x80000000 | 80:
                    return "输入序列号有误";
                case 0x80000000 | 100:
                    return "密码不正确";
                case 0x80000000 | 101:
                    return "帐户不存在";
                case 0x80000000 | 102:
                    return "等待登录返回超时";
                case 0x80000000 | 103:
                    return "帐号已登录";
                case 0x80000000 | 104:
                    return "帐号已被锁定";
                case 0x80000000 | 105:
                    return "帐号已被列为黑名单";
                case 0x80000000 | 106:
                    return "资源不足，系统忙";
                case 0x80000000 | 107:
                    return "连接主机失败";
                case 0x80000000 | 108:
                    return "网络连接失败";
                case 0x80000000 | 120:
                    return "Render库打开音频出错";
                case 0x80000000 | 121:
                    return "Render库关闭音频出错";
                case 0x80000000 | 122:
                    return "Render库控制音量出错";
                case 0x80000000 | 123:
                    return "Render库设置画面参数出错";
                case 0x80000000 | 124:
                    return "Render库暂停播放出错";
                case 0x80000000 | 125:
                    return "Render库抓图出错";
                case 0x80000000 | 126:
                    return "Render库步进出错";
                case 0x80000000 | 127:
                    return "Render库设置帧率出错";
                case 0x80000000 | 999:
                    return "暂时无法设置";
                case 0x80000000 | 1000:
                    return "配置数据不合法";
                default:
                    return "未知错误";
            }
        }

        private static void DHThrowLastError(int returnValue)
        {
            if (returnValue == 0)
            {
                DHThrowLastError();
            }
            else
            {
                pErrInfo.errCode = "0";
                pErrInfo.errMessage = "最近操作无异常发生";
            }
        }

        private static void DHThrowLastError(bool returnValue)
        {
            if (returnValue == false)
            {
                DHThrowLastError();
            }
            else
            {
                pErrInfo.errCode = "0";
                pErrInfo.errMessage = "最近操作无异常发生";
            }
        }

        /// <summary>
        /// SDK调用失败时抛出异常
        /// </summary>
        /// <param name="e"></param>
        private static void DHThrowLastError(Exception e)
        {

            pErrInfo.errCode = e.ToString();
            pErrInfo.errMessage = e.Message;
            if (pShowException == true)
            {
                throw e;
            }
        }

        #endregion

        #region << 公用方法 >>

        #region << 设置抛错方式 >>

        /// <summary>
        /// 设置使用本类的程序中的是否将错误信息抛出[默认不抛出，只将错误信息返回给属性LastOperationInfo]
        /// </summary>
        /// <param name="blnShowException">true:抛出错误信息,并将错误信息返回给属性LastOperationInfo,由客户自行处理;不抛出错误信息,将错误信息返回给属性LastOperationInfo,不作任何处理,用户可根据方法的返回值或LastOperationInfo判断是否有错误发生,然后再作相应处理</param>
        public static void DHSetShowException(bool blnShowException)
        {
            pShowException = blnShowException;
        }

        #endregion

        #region << 字符编码 >>

        /// <summary>
        /// 设置字符编码格式[默认为:gb2312,其他字符编码请通过本函数设置]
        /// </summary>
        /// <param name="encodingNum">字符编码整数</param>
        public static void DHSetEncoding(int encodingNum)
        {
            pEncoding = Encoding.GetEncoding(encodingNum);
        }

        /// <summary>
        /// 设备字符编码格式[默认为:gb2312,其他字符编码请通过本函数设置]
        /// </summary>
        /// <param name="encodingString">字符编码字符串，参见MSDN中的Encoding</param>
        public static void DHSetEncoding(string encodingString)
        {
            pEncoding = Encoding.GetEncoding(encodingString);
        }

        /// <summary>
        /// 设备字符编码格式[默认为:gb2312,其他字符编码请通过本函数设置]
        /// </summary>
        /// <param name="encoding">字符编码枚举,参见LANGUAGE_ENCODING枚举类型</param>
        public static void DHSetEncoding(LANGUAGE_ENCODING encoding)
        {
            pEncoding =  Encoding.GetEncoding((int)encoding);
        }

        /// <summary>
        /// 设备字符编码格式[默认为:gb2312,其他字符编码请通过本函数设置]
        /// </summary>
        /// <param name="encoding">字符编码</param>
        public static void DHSetEncoding(Encoding encoding)
        {
            pEncoding = encoding;
        }

        #endregion

        #region << SDK初期化和信息获取 >>

        /// <summary>
        /// 初始化SDK
        /// </summary>
        /// <param name="cbDisConnect">
        /// 断线回调函数,参见委托:<seealso cref="fDisConnect"/>
        /// </param>
        /// <param name="dwUser">用户数据,没有数据请使用IntPtr.Zero</param>
        /// <returns>true:成功;false:失败</returns>
        public static bool DHInit(fDisConnect cbDisConnect, IntPtr dwUser)
        {
            bool returnValue = false;
            if (initialized == false)
            {
                returnValue = CLIENT_Init(cbDisConnect, dwUser);
                DHThrowLastError(returnValue);
                initialized = true;
            }
            return returnValue;
        }

        /// <summary>
        /// 清空SDK, 释放占用的资源，在所有的SDK函数之后调用
        /// </summary>
        public static void DHCleanup()
        {
            CLIENT_Cleanup();
            //DHThrowLastError();
        }

        /// <summary>
        /// 设置与设备的连接等待时间
        /// </summary>
        /// <param name="nWaitTime">连接等待时间[单位:毫秒]</param>
        /// <param name="nTryTimes">连接次数</param>
        public static void DHSetConnectTime(int nWaitTime, int nTryTimes)
        {
            CLIENT_SetConnectTime(nWaitTime, nTryTimes);
            //DHThrowLastError();
        }

        /// <summary>
        /// 得到SDK的版本号
        /// </summary>
        /// <param name="formatStyle">格式[S:00.00.00.00;D:0.0.0.0;S3:00.00.00;D3:0.0.0;X:十六进制表示版本号信息]</param>
        /// <returns>版本号</returns>
        public static string DHGetSDKVersion(string formatStyle)
        {
            UInt32 returnValue = CLIENT_GetSDKVersion();
            //转换为16进制表示值       
            string returnString = returnValue.ToString("X");
            //格式化版本号
            string strTemp = "00000000";
            strTemp = strTemp.Remove(0, returnString.Length) + returnString;
            string strVerPart1 = strTemp.Substring(0, 2);
            string strVerPart2 = strTemp.Substring(2, 2);
            string strVerPart3 = strTemp.Substring(4, 2);
            string strVerPart4 = strTemp.Substring(6, 2);
            switch (formatStyle.ToUpper())
            {
                case "S":
                    returnString = strVerPart1 + "." + strVerPart2 + "." + strVerPart3 + "." + strVerPart4;
                    break;
                case "D":
                    returnString = int.Parse(strVerPart1).ToString() + "." + int.Parse(strVerPart2).ToString() + "." + int.Parse(strVerPart3).ToString() + "." + int.Parse(strVerPart4).ToString();
                    break;
                case "S3":
                    returnString = strVerPart1 + "." + int.Parse(strVerPart2).ToString() +int.Parse(strVerPart3).ToString() + "." + strVerPart4;
                    break;
                case "D3":
                    returnString = int.Parse(strVerPart1).ToString() + "." + int.Parse(strVerPart2).ToString() + int.Parse(strVerPart3).ToString() + "." + int.Parse(strVerPart4).ToString();
                    break;
                case "X":
                    returnString = returnValue.ToString("X");
                    break;
                default:
                    returnString = strVerPart1 + "." + strVerPart2 + "." + strVerPart3;
                    break;
            }
            DHThrowLastError(int.Parse(returnValue.ToString()));
            return returnString;
        }

        /// <summary>
        /// 得到SDK的版本号(格式为S3)
        /// </summary>
        /// <returns>版本号</returns>
        public static string DHGetSDKVersion()
        {
            return DHGetSDKVersion("S3");
        }

        #endregion

        #region << 字符叠加 >>

        /// <summary>
        /// 用户自定义画图, 在打开图像之前调用此函数,否则无效,必须在所有窗口未显示之前调用, 可以用来对画面进行字符叠加
        /// </summary>
        /// <param name="cbDraw">画图回调，当设置为0时表示禁止回调</param>
        /// <param name="dwUser">用户数据,没有数据请使用IntPtr.Zero</param>
        public static void DHRigisterDrawFun(fDrawCallBack cbDraw, IntPtr dwUser)
        {   
            CLIENT_RigisterDrawFun(cbDraw,dwUser);
            //DHThrowLastError();
        }

        #endregion

        #region << 参数配置 >>

        #region << 获取参数 >>

        /// <summary>
        ///  获取设备配置
        /// </summary>
        /// <param name="lLoginID">设备用户登录ID:CLIENT_Login的返回值</param>
        /// <param name="dwCommand">配置类型</param>
        /// <param name="lChannel">通道号，如果不是通道参数，lChannel不用,置为-1即可</param>
        /// <param name="lpOutBuffer">存放输出参数的缓冲区, 根据不同的类型, 输出不同的配置结构, 具体见数据结构定义</param>
        /// <param name="dwOutBufferSize">输入缓冲区的大小, (单位字节).</param>
        /// <param name="lpBytesReturned">实际返回的缓冲区大小,对应配置结构的大小, (单位字节)</param>
        /// <param name="waittime">等待时间</param>
        /// <returns>true:成功;false:失败</returns>
        public static bool DHGetDevConfig(int lLoginID, CONFIG_COMMAND dwCommand, int lChannel, IntPtr lpOutBuffer, UInt32 dwOutBufferSize, ref UInt32 lpBytesReturned, int waittime)
        {
            bool returnValue = false;
            returnValue = CLIENT_GetDevConfig(lLoginID, (UInt32)dwCommand, lChannel, lpOutBuffer, dwOutBufferSize, ref lpBytesReturned, waittime);
            DHThrowLastError(returnValue);
            return returnValue;
        }

        /// <summary>
        ///  获取设备配置
        /// </summary>
        /// <param name="lLoginID">设备用户登录ID:CLIENT_Login的返回值</param>
        /// <param name="dwCommand">配置类型</param>
        /// <param name="lChannel">通道号，如果不是通道参数，lChannel不用,置为-1即可</param>
        /// <param name="lpOutBuffer">存放输出参数的缓冲区, 根据不同的类型, 输出不同的配置结构, 具体见数据结构定义</param>
        /// <param name="dwOutBufferSize">输入缓冲区的大小, (单位字节).</param>
        /// <param name="lpBytesReturned">实际返回的缓冲区大小,对应配置结构的大小, (单位字节)</param>
        /// <returns>true:成功;false:失败</returns>
        public static bool DHGetDevConfig(int lLoginID, CONFIG_COMMAND dwCommand, int lChannel, IntPtr lpOutBuffer, UInt32 dwOutBufferSize, ref UInt32 lpBytesReturned)
        {
            return DHGetDevConfig(lLoginID, dwCommand, lChannel, lpOutBuffer, dwOutBufferSize, ref lpBytesReturned, 3000);            
        }

        /// <summary>
        /// 内部使用的取设备信息命令函数
        /// </summary>
        /// <param name="lLoginID">设备用户登录ID:CLIENT_Login的返回值</param>
        /// <param name="lChannel">通道号</param>
        /// <param name="dwCommand">配制命令</param>
        /// <param name="obj">object对象</param>
        /// <param name="typeName">类型名称</param>
        /// <param name="waittime">等待时间</param>
        /// <returns>true:成功;false:失败</returns>
        private static bool PGetDevConfig(int lLoginID, int lChannel, CONFIG_COMMAND dwCommand, ref object obj, Type typeName, int waittime)
        {
            bool returnValue = false;
            IntPtr pBoxInfo = IntPtr.Zero;
            try
            {                
                UInt32 returnBuffSize = 0;
                pBoxInfo = Marshal.AllocHGlobal(Marshal.SizeOf(typeName));//分配固定的指定大小的内存空间
                if (pBoxInfo != IntPtr.Zero)
                {
                    returnValue = DHGetDevConfig(lLoginID, dwCommand, lChannel, pBoxInfo, (UInt32)Marshal.SizeOf(typeName), ref returnBuffSize, waittime);
                    if (returnValue == true)
                    {
                        obj = Marshal.PtrToStructure((IntPtr)((UInt32)pBoxInfo), typeName);
                    }
                }
                DHThrowLastError(returnValue);
            }
            catch (Exception e)
            {
                DHThrowLastError(e);
                returnValue= false;
            }
            finally
            {
                Marshal.FreeHGlobal(pBoxInfo);//释放固定内存分配
                pBoxInfo = IntPtr.Zero;                
            }
            return returnValue;
        }

        /// <summary>
        /// 内部使用的取设备所有信息命令函数
        /// </summary>
        /// <param name="lLoginID">设备用户登录ID:CLIENT_Login的返回值</param>
        /// <param name="lChannel">通道号</param>
        /// <param name="dwCommand">配制命令</param>
        /// <param name="obj">object对象</param>
        /// <param name="typeName">类型名称</param>
        /// <param name="waittime">等待时间</param>
        /// <returns>true:成功;false:失败</returns>
        private static bool PGetDevConfig(int lLoginID, CONFIG_COMMAND dwCommand, ref object[] obj, Type typeName, int waittime)
        {
            bool returnValue = false;
            IntPtr pBoxInfo = IntPtr.Zero;
            try
            {
                UInt32 returnBuffSize = 0;
                pBoxInfo = Marshal.AllocHGlobal(obj.Length * Marshal.SizeOf(typeName));//分配固定的指定大小的内存空间
                if (pBoxInfo != IntPtr.Zero)
                {
                    returnValue = DHGetDevConfig(lLoginID, dwCommand, -1, pBoxInfo, (uint)obj.Length * (UInt32)Marshal.SizeOf(typeName), ref returnBuffSize, waittime);
                    if (returnValue == true)
                    {
                        for (int loop = 0; loop < obj.Length; loop++)
                        {
                            obj[loop] = Marshal.PtrToStructure((IntPtr)((UInt32)pBoxInfo + (UInt32)Marshal.SizeOf(typeName) * loop), typeName);
                        }
                    }
                }
                DHThrowLastError(returnValue);
            }
            catch (Exception e)
            {
                DHThrowLastError(e);
                returnValue= false;
            }
            finally
            {
                Marshal.FreeHGlobal(pBoxInfo);//释放固定内存分配
                pBoxInfo = IntPtr.Zero;
            }
            return returnValue;            
        }

        /// <summary>
        ///  获取设备配置[设备信息配置]
        /// </summary>
        /// <param name="lLoginID">设备用户登录ID:CLIENT_Login的返回值</param>
        /// <param name="systemAttrConfig">设备信息</param>
        /// <param name="waittime">等待时间</param>
        /// <returns>true:成功;false:失败</returns>
        public static bool DHGetDevConfig(int lLoginID, ref DHDEV_SYSTEM_ATTR_CFG systemAttrConfig, int waittime)
        {
            object result = new object();
            bool returnValue = false;
            returnValue = PGetDevConfig(lLoginID, 0, CONFIG_COMMAND.DH_DEV_DEVICECFG, ref result, typeof(DHDEV_SYSTEM_ATTR_CFG),waittime);
            systemAttrConfig = (DHDEV_SYSTEM_ATTR_CFG)result;
            return returnValue;
        }
        
        /// <summary>
        ///  获取设备配置[设备信息配置默认等待时间3000]
        /// </summary>
        /// <param name="lLoginID">设备用户登录ID:CLIENT_Login的返回值</param>
        /// <param name="systemAttrConfig">设备信息</param>
        /// <returns>true:成功;false:失败</returns>
        public static bool DHGetDevConfig(int lLoginID, ref DHDEV_SYSTEM_ATTR_CFG systemAttrConfig)
        {
            return DHGetDevConfig(lLoginID, ref systemAttrConfig, 3000);
        }

        /// <summary>
        ///  获取网络参数
        /// </summary>
        /// <param name="lLoginID">设备用户登录ID:CLIENT_Login的返回值</param>
        /// <param name="netConfig">网络参数</param>
        /// <param name="waittime">等待时间</param>
        /// <returns>true:成功;false:失败</returns>
        public static bool DHGetDevConfig(int lLoginID, ref DHDEV_NET_CFG netConfig, int waittime)
        {
            object result = new object();
            bool returnValue = false;
            returnValue = PGetDevConfig(lLoginID, 0, CONFIG_COMMAND.DH_DEV_NETCFG, ref result, typeof(DHDEV_NET_CFG), waittime);
            netConfig = (DHDEV_NET_CFG)result;
            return returnValue;

        }

        /// <summary>
        ///  获取网络参数[默认等待时间3000]
        /// </summary>
        /// <param name="lLoginID">设备用户登录ID:CLIENT_Login的返回值</param>
        /// <param name="netConfig">网络参数</param>
        /// <returns>true:成功;false:失败</returns>
        public static bool DHGetDevConfig(int lLoginID, ref DHDEV_NET_CFG netConfig)
        {
            return DHGetDevConfig(lLoginID, ref netConfig, 3000);
        }

        /// <summary>
        ///  获取指定通道配置
        /// </summary>
        /// <param name="lLoginID">设备用户登录ID:CLIENT_Login的返回值</param>
        /// <param name="lChannel">通道号[此值不能为-1]</param>
        /// <param name="netConfig">通道配置</param>
        /// <param name="waittime">等待时间</param>
        /// <returns>true:成功;false:失败</returns>
        public static bool DHGetDevConfig(int lLoginID, int lChannel, ref DHDEV_CHANNEL_CFG channelConfig, int waittime)
        {
            object result = new object();
            bool returnValue = false;
            returnValue = PGetDevConfig(lLoginID, lChannel, CONFIG_COMMAND.DH_DEV_CHANNELCFG, ref result, typeof(DHDEV_CHANNEL_CFG), waittime);
            channelConfig = (DHDEV_CHANNEL_CFG)result;
            return returnValue;
        }

        /// <summary>
        ///  获取指定通道配置[默认等待时间3000]
        /// </summary>
        /// <param name="lLoginID">设备用户登录ID:CLIENT_Login的返回值</param>
        /// <param name="lChannel">通道号[此值不能为-1]</param>
        /// <param name="netConfig">通道配置</param>
        /// <returns>true:成功;false:失败</returns>
        public static bool DHGetDevConfig(int lLoginID, int lChannel, ref DHDEV_CHANNEL_CFG channelConfig)
        {
            return DHGetDevConfig(lLoginID, lChannel, ref channelConfig);
        }

        /// <summary>
        ///  获取所有通道配置
        /// </summary>
        /// <param name="lLoginID">设备用户登录ID:CLIENT_Login的返回值</param>
        /// <param name="lChannel">通道号</param>
        /// <param name="netConfig">通道配置</param>
        /// <param name="waittime">等待时间</param>
        /// <returns>true:成功;false:失败</returns>
        public static bool DHGetDevConfig(int lLoginID, ref DHDEV_CHANNEL_CFG[] channelConfig, int waittime)
        {
            object[] result = new object[channelConfig.Length];
            bool returnValue = false;
            returnValue = PGetDevConfig(lLoginID, CONFIG_COMMAND.DH_DEV_CHANNELCFG, ref result, typeof(DHDEV_CHANNEL_CFG), waittime);
            for(int i=0;i<channelConfig.Length;i++)
            {
                channelConfig[i] = (DHDEV_CHANNEL_CFG)result[i];
            }            
            return returnValue;
        }

        /// <summary>
        ///  获取所有通道配置[默认等待时间3000]
        /// </summary>
        /// <param name="lLoginID">设备用户登录ID:CLIENT_Login的返回值</param>
        /// <param name="lChannel">通道号</param>
        /// <param name="netConfig">通道配置</param>
        /// <returns>true:成功;false:失败</returns>
        public static bool DHGetDevConfig(int lLoginID, ref DHDEV_CHANNEL_CFG[] channelConfig)
        {
            return DHGetDevConfig(lLoginID, ref channelConfig, 3000);
        }

        /// <summary>
        ///  获取指定通道的录像配置
        /// </summary>
        /// <param name="lLoginID">设备用户登录ID:CLIENT_Login的返回值</param>
        /// <param name="lChannel">通道号</param>
        /// <param name="netConfig">录像配置</param>
        /// <param name="waittime">等待时间</param>
        /// <returns>true:成功;false:失败</returns>
        public static bool DHGetDevConfig(int lLoginID, int lChannel, ref DHDEV_RECORD_CFG recordConfig, int waittime)
        {
            object result = new object();
            bool returnValue = false;
            returnValue = PGetDevConfig(lLoginID, lChannel, CONFIG_COMMAND.DH_DEV_RECORDCFG, ref result, typeof(DHDEV_RECORD_CFG), waittime);
            recordConfig = (DHDEV_RECORD_CFG)result;
            return returnValue;
        }

        /// <summary>
        ///  获取指定通道的录像配置[默认等待时间3000]
        /// </summary>
        /// <param name="lLoginID">设备用户登录ID:CLIENT_Login的返回值</param>
        /// <param name="lChannel">通道号</param>
        /// <param name="netConfig">录像配置</param>
        /// <returns>true:成功;false:失败</returns>
        public static bool DHGetDevConfig(int lLoginID, int lChannel, ref DHDEV_RECORD_CFG recordConfig)
        {
            return DHGetDevConfig(lLoginID, lChannel, ref recordConfig, 3000);
        }

        /// <summary>
        ///  获取所有通道的录像配置
        /// </summary>
        /// <param name="lLoginID">设备用户登录ID:CLIENT_Login的返回值</param>
        /// <param name="netConfig">录像配置</param>
        /// <param name="waittime">等待时间</param>
        /// <returns>true:成功;false:失败</returns>
        public static bool DHGetDevConfig(int lLoginID, ref DHDEV_RECORD_CFG[] recordConfig, int waittime)
        {
            object[] result = new object[recordConfig.Length];
            bool returnValue = false;
            returnValue = PGetDevConfig(lLoginID, CONFIG_COMMAND.DH_DEV_RECORDCFG, ref result, typeof(DHDEV_RECORD_CFG), waittime);
            for (int i = 0; i < recordConfig.Length; i++)
            {
                recordConfig[i] = (DHDEV_RECORD_CFG)result[i];
            }
            return returnValue;
        }

        /// <summary>
        ///  获取所有通道的录像配置[默认等待时间3000]
        /// </summary>
        /// <param name="lLoginID">设备用户登录ID:CLIENT_Login的返回值</param>
        /// <param name="netConfig">录像配置</param>
        /// <returns>true:成功;false:失败</returns>
        public static bool DHGetDevConfig(int lLoginID, ref DHDEV_RECORD_CFG[] recordConfig)
        {
            return DHGetDevConfig(lLoginID, ref recordConfig, 3000);
        }

        /// <summary>
        ///  获取预览参数
        /// </summary>
        /// <param name="lLoginID">设备用户登录ID:CLIENT_Login的返回值</param>
        /// <param name="netConfig">预览参数</param>
        /// <param name="waittime">等待时间</param>
        /// <returns>true:成功;false:失败</returns>
        public static bool DHGetDevConfig(int lLoginID, ref DHDEV_PREVIEW_CFG previewConfig, int waittime)
        {
            object result = new object();
            bool returnValue = false;
            returnValue = PGetDevConfig(lLoginID, -1, CONFIG_COMMAND.DH_DEV_PREVIEWCFG, ref result, typeof(DHDEV_PREVIEW_CFG), waittime);
            previewConfig = (DHDEV_PREVIEW_CFG)result;
            return returnValue;
        }

        /// <summary>
        ///  获取预览参数[默认等待时间3000]
        /// </summary>
        /// <param name="lLoginID">设备用户登录ID:CLIENT_Login的返回值</param>
        /// <param name="netConfig">预览参数</param>
        /// <returns>true:成功;false:失败</returns>
        public static bool DHGetDevConfig(int lLoginID, ref DHDEV_PREVIEW_CFG previewConfig)
        {
            return DHGetDevConfig(lLoginID, ref previewConfig, 3000);
        }

        /// <summary>
        ///  获取串口参数
        /// </summary>
        /// <param name="lLoginID">设备用户登录ID:CLIENT_Login的返回值</param>
        /// <param name="netConfig">预览参数</param>
        /// <param name="waittime">等待时间</param>
        /// <returns>true:成功;false:失败</returns>
        public static bool DHGetDevConfig(int lLoginID, ref DHDEV_COMM_CFG commConfig, int waittime)
        {
            object result = new object();
            bool returnValue = false;
            returnValue = PGetDevConfig(lLoginID, -1, CONFIG_COMMAND.DH_DEV_COMMCFG, ref result, typeof(DHDEV_COMM_CFG), waittime);
            commConfig = (DHDEV_COMM_CFG)result;
            return returnValue;
        }

        /// <summary>
        ///  获取串口参数[默认等待时间3000]
        /// </summary>
        /// <param name="lLoginID">设备用户登录ID:CLIENT_Login的返回值</param>
        /// <param name="netConfig">预览参数</param>
        /// <returns>true:成功;false:失败</returns>
        public static bool DHGetDevConfig(int lLoginID, ref DHDEV_COMM_CFG commConfig)
        {
            return DHGetDevConfig(lLoginID, ref commConfig, 3000);
        }

        /// <summary>
        ///  获取报警参数
        /// </summary>
        /// <param name="lLoginID">设备用户登录ID:CLIENT_Login的返回值</param>
        /// <param name="alarmConfig">报警参数</param>
        /// <param name="waittime">等待时间</param>
        /// <returns>true:成功;false:失败</returns>
        public static bool DHGetDevConfig(int lLoginID, ref DHDEV_ALARM_SCHEDULE alarmConfig, int waittime)
        {
            object result = new object();
            bool returnValue = false;
            returnValue = PGetDevConfig(lLoginID, 0, CONFIG_COMMAND.DH_DEV_ALARMCFG, ref result, typeof(DHDEV_ALARM_SCHEDULE), waittime);
            alarmConfig = (DHDEV_ALARM_SCHEDULE)result;
            return returnValue;
        }

        /// <summary>
        ///  获取报警参数[默认等待时间3000]
        /// </summary>
        /// <param name="lLoginID">设备用户登录ID:CLIENT_Login的返回值</param>
        /// <param name="alarmConfig">报警参数</param>
        /// <returns>true:成功;false:失败</returns>
        public static bool DHGetDevConfig(int lLoginID, ref DHDEV_ALARM_SCHEDULE alarmConfig)
        {
            return DHGetDevConfig(lLoginID, ref alarmConfig, 3000);

        }

        /// <summary>
        ///  获取DVR时间
        /// </summary>
        /// <param name="lLoginID">设备用户登录ID:CLIENT_Login的返回值</param>
        /// <param name="dvrTime">时间参数</param>
        /// <param name="waittime">等待时间</param>
        /// <returns>true:成功;false:失败</returns>
        public static bool DHGetDevConfig(int lLoginID, ref NET_TIME dvrTime, int waittime)
        {
            object result = new object();
            bool returnValue = false;
            returnValue = PGetDevConfig(lLoginID, -1, CONFIG_COMMAND.DH_DEV_TIMECFG, ref result, typeof(NET_TIME), waittime);
            dvrTime = (NET_TIME)result;
            return returnValue;
        }

        /// <summary>
        ///  获取DVR时间[默认等待时间3000]
        /// </summary>
        /// <param name="lLoginID">设备用户登录ID:CLIENT_Login的返回值</param>
        /// <param name="dvrTime">时间参数</param>
        /// <returns>true:成功;false:失败</returns>
        public static bool DHGetDevConfig(int lLoginID, ref NET_TIME dvrTime)
        {
            return DHGetDevConfig(lLoginID, ref dvrTime, 3000);
        }

        #region << 对讲参数以后的信息因与机型有关故第一版SDK中暂不实现[暂时屏蔽] >>

        ///// <summary>
        /////  获取对讲参数
        ///// </summary>
        ///// <param name="lLoginID">设备用户登录ID:CLIENT_Login的返回值</param>
        ///// <param name="dvrTime">对讲参数</param>
        ///// <returns>true:成功;false:失败</returns>
        //public static bool DHGetDevConfig(int lLoginID, ref DHDEV_TALK_CFG talkConfig)
        //{
        //    object result = new object();
        //    bool returnValue = false;
        //    returnValue = PGetDevConfig(lLoginID, -1, CONFIG_COMMAND.DH_DEV_TALKCFG, ref result, typeof(DHDEV_TALK_CFG));
        //    talkConfig = (DHDEV_TALK_CFG)result;
        //    return returnValue;
        //}

        ///// <summary>
        /////  获取自动维护参数
        ///// </summary>
        ///// <param name="lLoginID">设备用户登录ID:CLIENT_Login的返回值</param>
        ///// <param name="dvrTime">自动维护参数</param>
        ///// <returns>true:成功;false:失败</returns>
        //public static bool DHGetDevConfig(int lLoginID, ref DHDEV_TALK_CFG autoConfig)
        //{
        //    object result = new object();
        //    bool returnValue = false;
        //    returnValue = PGetDevConfig(lLoginID, -1, CONFIG_COMMAND.DH_DEV_AUTOMTCFG, ref result, typeof(DHDEV_TALK_CFG));
        //    autoConfig = (DHDEV_TALK_CFG)result;
        //    return returnValue;
        //}

        #endregion

        #endregion

        #region << 设置参数 >>

        /// <summary>
        /// 设置设备配置
        /// </summary>
        /// <param name="lLoginID">设备用户登录ID:CLIENT_Login的返回值</param>
        /// <param name="dwCommand">配置类型</param>
        /// <param name="lChannel">通道号，如果设置全部通道数据为0xFFFFFFFF(-1)，如果命令不需要通道号，该参数无效</param>
        /// <param name="lpInBuffer">存放输出参数的缓冲区, 根据不同的类型, 输入不同的配置结构, 具体见数据结构定义</param>
        /// <param name="dwInBufferSize">输入缓冲区的大小, (单位字节).</param>
        /// <param name="waittime">等待时间</param>
        /// <returns>true:成功;false:失败</returns>
        public static bool DHSetDevConfig(int lLoginID, CONFIG_COMMAND dwCommand, int lChannel, IntPtr lpInBuffer, UInt32 dwInBufferSize, int waittime)
        {
            bool returnValue = false;
            returnValue = CLIENT_SetDevConfig(lLoginID, (UInt32)dwCommand, lChannel, lpInBuffer, dwInBufferSize, waittime);
            DHThrowLastError(returnValue);
            return returnValue;
        }

        /// <summary>
        /// 设置设备配置
        /// </summary>
        /// <param name="lLoginID">设备用户登录ID:CLIENT_Login的返回值</param>
        /// <param name="dwCommand">配置类型</param>
        /// <param name="lChannel">通道号，如果设置全部通道数据为0xFFFFFFFF(-1)，如果命令不需要通道号，该参数无效</param>
        /// <param name="lpOutBuffer">存放输出参数的缓冲区, 根据不同的类型, 输入不同的配置结构, 具体见数据结构定义</param>
        /// <param name="dwOutBufferSize">输入缓冲区的大小(单位字节).</param>
        /// <returns>true:成功;false:失败</returns>
        public static bool DHSetDevConfig(int lLoginID, CONFIG_COMMAND dwCommand, int lChannel, IntPtr lpInBuffer, UInt32 dwInBufferSize)
        {
            return DHSetDevConfig(lLoginID, dwCommand, lChannel, lpInBuffer, dwInBufferSize, 3000);
        }

        /// <summary>
        /// 内部使用的设置设备信息命令函数
        /// </summary>
        /// <param name="lLoginID">设备用户登录ID:CLIENT_Login的返回值</param>
        /// <param name="lChannel">通道号，如果设置全部通道数据为0xFFFFFFFF(-1)，如果命令不需要通道号，该参数无效</param>
        /// <param name="dwCommand">配制命令</param>
        /// <param name="obj">object对象</param>
        /// <param name="typeName">结构类型</param>
        /// <param name="waittime">等待时间</param>
        /// <returns>true:成功;false:失败</returns>
        private static bool PSetDevConfig(int lLoginID, int lChannel, CONFIG_COMMAND dwCommand, object obj, Type typeName, int waittime)
        {
            bool returnValue = false;
            IntPtr pBoxInfo = IntPtr.Zero;
            try
            {
                pBoxInfo = Marshal.AllocHGlobal(Marshal.SizeOf(typeName));//分配固定的指定大小的内存空间
                if (pBoxInfo != IntPtr.Zero)
                {
                    Marshal.StructureToPtr(obj, pBoxInfo, true);
                    returnValue = DHSetDevConfig(lLoginID, dwCommand, lChannel, pBoxInfo, (UInt32)Marshal.SizeOf(typeName), waittime);
                }
            }
            catch (Exception e)
            {
                DHThrowLastError(e);
                returnValue= false;
            }
            finally
            {
                Marshal.FreeHGlobal(pBoxInfo);//释放固定内存分配
                pBoxInfo = IntPtr.Zero;
            }
            return returnValue;
            
        }

        /// <summary>
        /// 内部使用的设置设备所有信息命令函数
        /// </summary>
        /// <param name="lLoginID">设备用户登录ID:CLIENT_Login的返回值</param>
        /// <param name="dwCommand">配制命令</param>
        /// <param name="obj">object对象</param>
        /// <param name="typeName">结构类型</param>
        /// <param name="waittime">等待时间</param>
        /// <returns>true:成功;false:失败</returns>
        private static bool PSetDevConfig(int lLoginID, CONFIG_COMMAND dwCommand,object[] obj, Type typeName, int waittime)
        {
            bool returnValue = false;
            IntPtr pBoxInfo = IntPtr.Zero;
            try
            {
                pBoxInfo = Marshal.AllocHGlobal(obj.Length * Marshal.SizeOf(typeName));//分配固定的指定大小的内存空间
                if (pBoxInfo != IntPtr.Zero)
                {
                    for (int loop = 0; loop < obj.Length; loop++)
                    {
                        Marshal.StructureToPtr(obj[loop], (IntPtr)((UInt32)pBoxInfo + (UInt32)Marshal.SizeOf(typeName) * loop), true);
                    }
                    returnValue = DHSetDevConfig(lLoginID, dwCommand, -1, pBoxInfo, (uint)obj.Length * (UInt32)Marshal.SizeOf(typeName), waittime);
                }
            }
            catch (Exception e)
            {
                DHThrowLastError(e);
                returnValue= false;
            }
            finally
            {
                Marshal.FreeHGlobal(pBoxInfo);//释放固定内存分配
                pBoxInfo = IntPtr.Zero;
                
            }
            return returnValue;
        }

        /// <summary>
        /// 设备属性设定
        /// </summary>
        /// <param name="lLoginID">设备用户登录ID:CLIENT_Login的返回值</param>
        /// <param name="systemAttrConfig">设备属性</param>
        /// <param name="waittime">等待时间</param>
        /// <returns>true:成功;false:失败</returns>
        public static bool DHSetDevConfig(int lLoginID,DHDEV_SYSTEM_ATTR_CFG systemAttrConfig, int waittime)
        {
            bool returnValue = false;
            returnValue = PSetDevConfig(lLoginID, 0, CONFIG_COMMAND.DH_DEV_DEVICECFG, (object)systemAttrConfig, typeof(DHDEV_SYSTEM_ATTR_CFG), waittime);
            return returnValue;
        }

        /// <summary>
        /// 设备属性设定[默认等待时间3000]
        /// </summary>
        /// <param name="lLoginID">设备用户登录ID:CLIENT_Login的返回值</param>
        /// <param name="systemAttrConfig">设备属性</param>
        /// <returns>true:成功;false:失败</returns>
        public static bool DHSetDevConfig(int lLoginID, DHDEV_SYSTEM_ATTR_CFG systemAttrConfig)
        {
            return DHSetDevConfig(lLoginID, systemAttrConfig, 3000);
        }

        /// <summary>
        /// 设定网络参数
        /// </summary>
        /// <param name="lLoginID">设备用户登录ID:CLIENT_Login的返回值</param>
        /// <param name="netConfig">网络参数</param>
        /// <param name="waittime">等待时间</param>
        /// <returns>true:成功;false:失败</returns>
        public static bool DHSetDevConfig(int lLoginID, DHDEV_NET_CFG netConfig, int waittime)
        {            
            bool returnValue = false;
            returnValue = PSetDevConfig(lLoginID, 0, CONFIG_COMMAND.DH_DEV_NETCFG, (object)netConfig, typeof(DHDEV_NET_CFG), waittime);           
            return returnValue;
        }

        /// <summary>
        /// 设定网络参数[默认等待时间3000]
        /// </summary>
        /// <param name="lLoginID">设备用户登录ID:CLIENT_Login的返回值</param>
        /// <param name="netConfig">网络参数</param>
        /// <returns>true:成功;false:失败</returns>
        public static bool DHSetDevConfig(int lLoginID, DHDEV_NET_CFG netConfig)
        {
            return DHSetDevConfig(lLoginID, netConfig, 3000);
        }

        /// <summary>
        /// 设定指定通道配置
        /// </summary>
        /// <param name="lLoginID">设备用户登录ID:CLIENT_Login的返回值</param>
        /// <param name="lChannel">通道号[此值不能为-1]</param>
        /// <param name="netConfig">通道配置</param>
        /// <param name="waittime">等待时间</param>
        /// <returns>true:成功;false:失败</returns>
        public static bool DHSetDevConfig(int lLoginID, int lChannel, DHDEV_CHANNEL_CFG channelConfig, int waittime)
        {            
            bool returnValue = false;
            returnValue = PSetDevConfig(lLoginID, lChannel, CONFIG_COMMAND.DH_DEV_CHANNELCFG,(object)channelConfig, typeof(DHDEV_CHANNEL_CFG), waittime);           
            return returnValue;
        }

        /// <summary>
        /// 设定指定通道配置[默认时间3000]
        /// </summary>
        /// <param name="lLoginID">设备用户登录ID:CLIENT_Login的返回值</param>
        /// <param name="lChannel">通道号[此值不能为-1]</param>
        /// <param name="netConfig">通道配置</param>
        /// <param name="waittime">等待时间</param>
        /// <returns>true:成功;false:失败</returns>
        public static bool DHSetDevConfig(int lLoginID, int lChannel, DHDEV_CHANNEL_CFG channelConfig)
        {
            return DHSetDevConfig(lLoginID, lChannel, channelConfig, 3000);
        }

        /// <summary>
        ///  设定所有通道配置
        /// </summary>
        /// <param name="lLoginID">设备用户登录ID:CLIENT_Login的返回值</param>
        /// <param name="lChannel">通道号</param>
        /// <param name="netConfig">通道配置</param>
        /// <param name="waittime">等待时间</param>
        /// <returns>true:成功;false:失败</returns>
        public static bool DHSetDevConfig(int lLoginID, DHDEV_CHANNEL_CFG[] channelConfig, int waittime)
        {
            object[] result = new object[channelConfig.Length];
            for (int i = 0; i < channelConfig.Length; i++)
            {
                result[i] = (object)channelConfig[i];
            }
            bool returnValue = false;
            returnValue = PSetDevConfig(lLoginID, CONFIG_COMMAND.DH_DEV_CHANNELCFG, result, typeof(DHDEV_CHANNEL_CFG), waittime);
            
            return returnValue;
        }

        /// <summary>
        /// 设定所有通道配置
        /// </summary>
        /// <param name="lLoginID">设备用户登录ID:CLIENT_Login的返回值</param>
        /// <param name="lChannel">通道号</param>
        /// <param name="netConfig">通道配置</param>
        /// <param name="waittime">等待时间</param>
        /// <returns>true:成功;false:失败</returns>
        public static bool DHSetDevConfig(int lLoginID, DHDEV_CHANNEL_CFG[] channelConfig)
        {
            return DHSetDevConfig(lLoginID, channelConfig, 3000);
        }

        /// <summary>
        /// 设定指定通道的录像配置
        /// </summary>
        /// <param name="lLoginID">设备用户登录ID:CLIENT_Login的返回值</param>
        /// <param name="lChannel">通道号</param>
        /// <param name="netConfig">录像配置</param>
        /// <param name="waittime">等待时间</param>
        /// <returns>true:成功;false:失败</returns>
        public static bool DHSetDevConfig(int lLoginID, int lChannel,DHDEV_RECORD_CFG recordConfig, int waittime)
        {
            bool returnValue = false;
            returnValue = PSetDevConfig(lLoginID, lChannel, CONFIG_COMMAND.DH_DEV_RECORDCFG, (object)recordConfig, typeof(DHDEV_RECORD_CFG), waittime);
            return returnValue;
        }

        /// <summary>
        /// 设定指定通道的录像配置[默认时间3000]
        /// </summary>
        /// <param name="lLoginID">设备用户登录ID:CLIENT_Login的返回值</param>
        /// <param name="lChannel">通道号</param>
        /// <param name="netConfig">录像配置</param>
        /// <param name="waittime">等待时间</param>
        /// <returns>true:成功;false:失败</returns>
        public static bool DHSetDevConfig(int lLoginID, int lChannel, DHDEV_RECORD_CFG recordConfig)
        {
            return DHSetDevConfig(lLoginID, lChannel, recordConfig, 600);
        }

        /// <summary>
        /// 设定所有通道的录像配置
        /// </summary>
        /// <param name="lLoginID">设备用户登录ID:CLIENT_Login的返回值</param>
        /// <param name="netConfig">录像配置</param>
        /// <param name="waittime">等待时间</param>
        /// <returns>true:成功;false:失败</returns>
        public static bool DHSetDevConfig(int lLoginID, DHDEV_RECORD_CFG[] recordConfig, int waittime)
        {
            object[] result = new object[recordConfig.Length];
            for (int i = 0; i < recordConfig.Length; i++)
            {
                result[i]=(object)recordConfig[i];
            }
            bool returnValue = false;
            returnValue = PSetDevConfig(lLoginID, CONFIG_COMMAND.DH_DEV_RECORDCFG, result, typeof(DHDEV_RECORD_CFG), waittime);
            
            return returnValue;
        }

        /// <summary>
        /// 设定所有通道的录像配置[默认等待时间3000]
        /// </summary>
        /// <param name="lLoginID">设备用户登录ID:CLIENT_Login的返回值</param>
        /// <param name="netConfig">录像配置</param>
        /// <returns>true:成功;false:失败</returns>
        public static bool DHSetDevConfig(int lLoginID, DHDEV_RECORD_CFG[] recordConfig)
        {
            return DHSetDevConfig(lLoginID, recordConfig, 3000);
        }


        /// <summary>
        /// 设定预览参数
        /// </summary>
        /// <param name="lLoginID">设备用户登录ID:CLIENT_Login的返回值</param>
        /// <param name="netConfig">预览参数</param>
        /// <param name="waittime">等待时间</param>
        /// <returns>true:成功;false:失败</returns>
        public static bool DHSetDevConfig(int lLoginID,DHDEV_PREVIEW_CFG previewConfig, int waittime)
        {
            bool returnValue = false;
            returnValue = PSetDevConfig(lLoginID, -1, CONFIG_COMMAND.DH_DEV_PREVIEWCFG, (object)previewConfig, typeof(DHDEV_PREVIEW_CFG), waittime);
            return returnValue;
        }

        /// <summary>
        /// 设定预览参数[默认等待时间3000]
        /// </summary>
        /// <param name="lLoginID">设备用户登录ID:CLIENT_Login的返回值</param>
        /// <param name="netConfig">预览参数</param>
        /// <returns>true:成功;false:失败</returns>
        public static bool DHSetDevConfig(int lLoginID,DHDEV_PREVIEW_CFG previewConfig)
        {
            return DHSetDevConfig(lLoginID,previewConfig, 3000);
        }

        /// <summary>
        /// 设定串口参数
        /// </summary>
        /// <param name="lLoginID">设备用户登录ID:CLIENT_Login的返回值</param>
        /// <param name="netConfig">预览参数</param>
        /// <param name="waittime">等待时间</param>
        /// <returns>true:成功;false:失败</returns>
        public static bool DHSetDevConfig(int lLoginID, DHDEV_COMM_CFG commConfig, int waittime)
        {
            bool returnValue = false;
            returnValue = PSetDevConfig(lLoginID, -1, CONFIG_COMMAND.DH_DEV_COMMCFG, (object)commConfig, typeof(DHDEV_COMM_CFG), waittime);
            return returnValue;
        }

        /// <summary>
        /// 设定串口参数[默认等待时间3000]
        /// </summary>
        /// <param name="lLoginID">设备用户登录ID:CLIENT_Login的返回值</param>
        /// <param name="netConfig">预览参数</param>
        /// <returns>true:成功;false:失败</returns>
        public static bool DHSetDevConfig(int lLoginID, DHDEV_COMM_CFG commConfig)
        {
            return DHSetDevConfig(lLoginID, commConfig, 3000);
        }

        /// <summary>
        /// 设定报警参数
        /// </summary>
        /// <param name="lLoginID">设备用户登录ID:CLIENT_Login的返回值</param>
        /// <param name="alarmConfig">报警参数</param>
        /// <param name="waittime">等待时间</param>
        /// <returns>true:成功;false:失败</returns>
        public static bool DHSetDevConfig(int lLoginID,DHDEV_ALARM_SCHEDULE alarmConfig, int waittime)
        {
            bool returnValue = false;
            returnValue = PSetDevConfig(lLoginID, 0, CONFIG_COMMAND.DH_DEV_ALARMCFG, (object)alarmConfig, typeof(DHDEV_ALARM_SCHEDULE), waittime);
            return returnValue;
        }

        /// <summary>
        /// 设定报警参数[默认等待时间3000]
        /// </summary>
        /// <param name="lLoginID">设备用户登录ID:CLIENT_Login的返回值</param>
        /// <param name="alarmConfig">报警参数</param>
        /// <returns>true:成功;false:失败</returns>
        public static bool DHSetDevConfig(int lLoginID,DHDEV_ALARM_SCHEDULE alarmConfig)
        {
            return DHSetDevConfig(lLoginID,alarmConfig, 3000);
        }

        /// <summary>
        /// 设定DVR时间
        /// </summary>
        /// <param name="lLoginID">设备用户登录ID:CLIENT_Login的返回值</param>
        /// <param name="dvrTime">时间参数</param>
        /// <param name="waittime">等待时间</param>
        /// <returns>true:成功;false:失败</returns>
        public static bool DHSetDevConfig(int lLoginID,NET_TIME dvrTime, int waittime)
        {
            
            bool returnValue = false;
            returnValue = PSetDevConfig(lLoginID, -1, CONFIG_COMMAND.DH_DEV_TIMECFG, (object)dvrTime, typeof(NET_TIME), waittime);
            return returnValue;
        }

        /// <summary>
        /// 设定DVR时间[默认等待时间3000]
        /// </summary>
        /// <param name="lLoginID">设备用户登录ID:CLIENT_Login的返回值</param>
        /// <param name="dvrTime">时间参数</param>
        /// <returns>true:成功;false:失败</returns>
        public static bool DHSetDevConfig(int lLoginID, NET_TIME dvrTime)
        {
            return DHSetDevConfig(lLoginID, dvrTime, 3000);
        }

        #endregion

        #endregion

        #region << 用户管理 >>

        /// <summary>
        /// 查询用户信息
        /// </summary>
        /// <param name="iLogin">DHLogin的返回值</param>
        /// <param name="userInfo">USER_MANAGE_INFO结构</param>
        /// <param name="waitTime">等待时间</param>
        /// <returns>true:成功;false:失败</returns>
        public static bool DHQueryUserInfo(int iLogin,ref USER_MANAGE_INFO userInfo,int waitTime)
        {
            bool retValue = false;
            IntPtr pBoxInfo = IntPtr.Zero;
            try
            {
                pBoxInfo = Marshal.AllocHGlobal(Marshal.SizeOf(typeof(USER_MANAGE_INFO)));//分配固定的指定大小的内存空间
                if (pBoxInfo != IntPtr.Zero)
                {
                    retValue = CLIENT_QueryUserInfo(iLogin, pBoxInfo, waitTime);
                    if (retValue == true)
                    {
                        userInfo = (USER_MANAGE_INFO)Marshal.PtrToStructure((IntPtr)((UInt32)pBoxInfo), typeof(USER_MANAGE_INFO));
                    }
                }
                DHThrowLastError(retValue);                
            }
            catch (Exception e)
            {
                DHThrowLastError(e);
                retValue = false;
            }
            finally
            {
                Marshal.FreeHGlobal(pBoxInfo);//释放固定内存分配
                pBoxInfo = IntPtr.Zero;
            } 
            return retValue;
                           
        }

        /// <summary>
        /// 增加用户组/删除用户组
        /// </summary>
        /// <param name="iLogin">DHLogin的返回值</param>
        /// <param name="grpInfo">用户组信息</param>
        /// <param name="waitTime">等待时间</param>
        /// <returns></returns>
        public static bool DHOperateUserInfo(int iLogin, USER_OPERATE operate,USER_GROUP_INFO grpInfo,int waitTime)
        {
            bool retValue = false;
            IntPtr pBoxInfo = IntPtr.Zero;
            try
            {
                pBoxInfo = Marshal.AllocHGlobal(Marshal.SizeOf(typeof(USER_GROUP_INFO)));//分配固定的指定大小的内存空间
                if (pBoxInfo != IntPtr.Zero)
                {
                    Marshal.StructureToPtr(grpInfo, pBoxInfo, true);
                    retValue = CLIENT_OperateUserInfo(iLogin, (int)operate, pBoxInfo, IntPtr.Zero, waitTime);
                }
                DHThrowLastError(retValue);
            }
            catch (Exception e)
            {
                DHThrowLastError(e);
                retValue = false;
            }
            finally
            {
                Marshal.FreeHGlobal(pBoxInfo);//释放固定内存分配
                pBoxInfo = IntPtr.Zero;                
            }
            return retValue;
        }

        /// <summary>
        /// 修改用户组
        /// </summary>
        /// <param name="iLogin"></param>
        /// <param name="grpInfo"></param>
        /// <param name="waitTime"></param>
        /// <returns></returns>
        public static bool DHOperateUserInfo(int iLogin, USER_OPERATE operate, USER_GROUP_INFO grpNewInfo, USER_GROUP_INFO grpOldInfo, int waitTime)
        {
            bool retValue = false;
            IntPtr pBoxInfo = IntPtr.Zero;
            IntPtr pBoxInfo1 = IntPtr.Zero;
            try
            {
                pBoxInfo = Marshal.AllocHGlobal(Marshal.SizeOf(typeof(USER_GROUP_INFO)));//分配固定的指定大小的内存空间                
                pBoxInfo1 = Marshal.AllocHGlobal(Marshal.SizeOf(typeof(USER_GROUP_INFO)));//分配固定的指定大小的内存空间
                if (pBoxInfo != IntPtr.Zero && pBoxInfo1!=IntPtr.Zero)
                {
                    Marshal.StructureToPtr(grpNewInfo, pBoxInfo, true);
                    Marshal.StructureToPtr(grpOldInfo, pBoxInfo1, true);
                    retValue = CLIENT_OperateUserInfo(iLogin, (int)operate, pBoxInfo, pBoxInfo1, waitTime);
                }
                DHThrowLastError(retValue);                
            }
            catch (Exception e)
            {
                DHThrowLastError(e);
                retValue= false;
            }
            finally
            {
                Marshal.FreeHGlobal(pBoxInfo);//释放固定内存分配
                pBoxInfo = IntPtr.Zero;
                Marshal.FreeHGlobal(pBoxInfo1);//释放固定内存分配
                pBoxInfo = IntPtr.Zero;                
            }
            return retValue;
            
        }

        /// <summary>
        /// 增加用户/删除用户
        /// </summary>
        /// <param name="iLogin">DHLogin返回值</param>
        /// <param name="usrInfo">新增用户信息</param>
        /// <param name="waitTime">等待时间</param>
        /// <returns></returns>
        public static bool DHOperateUserInfo(int iLogin, USER_OPERATE operate, USER_INFO usrInfo, int waitTime)
        {
            bool retValue = false;
            IntPtr pBoxInfo = IntPtr.Zero;
            try
            {
                pBoxInfo = Marshal.AllocHGlobal(Marshal.SizeOf(typeof(USER_INFO)));//分配固定的指定大小的内存空间
                if (pBoxInfo != IntPtr.Zero)
                {
                    Marshal.StructureToPtr(usrInfo, pBoxInfo, true);
                    retValue = CLIENT_OperateUserInfo(iLogin, (int)operate, pBoxInfo, IntPtr.Zero, waitTime);
                }
                DHThrowLastError(retValue);
            }
            catch (Exception e)
            {
                DHThrowLastError(e);
                retValue = false;
            }
            finally
            {
                Marshal.FreeHGlobal(pBoxInfo);//释放固定内存分配
                pBoxInfo = IntPtr.Zero;
            }
            return retValue;
        }

        /// <summary>
        /// 修改用户基本信息/修改用户密码
        /// </summary>
        /// <param name="iLogin">DHLogin返回值</param>
        /// <param name="userNewInfo">用户变更后信息</param>
        /// <param name="userOldInfo">用户变更前信息</param>
        /// <param name="waitTime">等待时间</param>
        /// <returns></returns>
        public static bool DHOperateUserInfo(int iLogin, USER_OPERATE operate, USER_INFO userNewInfo, USER_INFO userOldInfo, int waitTime)
        {
            bool retValue = false;
            IntPtr pBoxInfo = IntPtr.Zero;
            IntPtr pBoxInfo1 = IntPtr.Zero;
            try
            {                
                pBoxInfo = Marshal.AllocHGlobal(Marshal.SizeOf(typeof(USER_INFO)));//分配固定的指定大小的内存空间               
                pBoxInfo1 = Marshal.AllocHGlobal(Marshal.SizeOf(typeof(USER_INFO)));//分配固定的指定大小的内存空间
                if (pBoxInfo != IntPtr.Zero & pBoxInfo1!=IntPtr.Zero)
                {
                    Marshal.StructureToPtr(userNewInfo, pBoxInfo, true);
                    Marshal.StructureToPtr(userOldInfo, pBoxInfo1, true);
                    retValue = CLIENT_OperateUserInfo(iLogin, (int)operate, pBoxInfo, pBoxInfo1, waitTime);
                }
                DHThrowLastError(retValue);                
            }
            catch (Exception e)
            {
                DHThrowLastError(e);
                retValue = false;
            }
            finally
            {
                Marshal.FreeHGlobal(pBoxInfo);//释放固定内存分配
                pBoxInfo = IntPtr.Zero;
                Marshal.FreeHGlobal(pBoxInfo1);//释放固定内存分配
                pBoxInfo1 = IntPtr.Zero;                
            }
            return retValue;
        }
        
        #endregion

        #region << 状态侦听 >>

        /// <summary>
        /// 设置设备消息回调函数, 用来得到设备当前状态信息
        /// </summary>
        /// <param name="cbMessage">消息回调参数</param>
        /// <param name="dwUser">用户数据,没有数据请使用IntPtr.Zero</param>
        public static void DHSetDVRMessCallBack(fMessCallBack cbMessage, IntPtr dwUser)
        {
            CLIENT_SetDVRMessCallBack(cbMessage, dwUser);
            //DHThrowLastError();
        }

        /// <summary>
        /// 开始对某个设备订阅消息,用来设置是否需要对设备消息回调，得到的消息从<seealso cref="DHSetDVRMessCallBack"/>的设置值回调出来。
        /// </summary>
        /// <param name="lLoginID">设备用户登录ID:<seealso cref="DHLogin"/>的返回值</param>
        /// <returns>true:成功;false:失败</returns>
        public static bool DHStartListen(int lLoginID)
        {
            bool returnValue = false;
            returnValue = CLIENT_StartListen(lLoginID);
            DHThrowLastError(returnValue);
            return returnValue;
        }

        /// <summary>
        /// 停止对某个设备侦听消息
        /// </summary>
        /// <param name="lLoginID">设备用户登录ID:<seealso cref="CLIENT_Login"/>的返回值</param>
        /// <returns>true:成功;false:失败</returns>
        public static bool DHStopListen(int lLoginID)
        {
            bool returnValue = false;
            returnValue = CLIENT_StopListen(lLoginID);
            DHThrowLastError(returnValue);
            return returnValue;
        }

        /// <summary>
        /// 启动监听服务, 目前只实现了报警监听功能
        /// </summary>
        /// <param name="wPort">启动监听的端口</param>
        /// <param name="pIp">绑定的IP，为NULL时绑定本机所有合法IP</param>
        /// <param name="pfscb">服务器的消息回调接口</param>
        /// <param name="dwTimeOut">服务器维护连接的超时时间</param>
        /// <param name="dwUserData">用户回调的自定义数据</param>
        /// <returns>成功返回服务器句柄，失败返回0</returns>
        public static int DHStartService(ushort wPort, string pIp, fServiceCallBack pfscb, IntPtr dwTimeOut, IntPtr dwUserData)
        {
            int returnValue;
            returnValue = CLIENT_StartService(wPort, pIp, pfscb, dwTimeOut, dwUserData);
            DHThrowLastError(returnValue);
            return returnValue;
        }

        /// <summary>
        /// 停止端口监听服务
        /// </summary>
        /// <param name="lHandle">
        /// 要关闭的服务器的句柄:<seealso cref="CLIENT_StartService"/>的返回值</param>
        /// <returns>true:成功;false:失败</returns>
        public static bool DHStopService(int lHandle)
        {
            bool returnValue = false;
            returnValue = CLIENT_StopService(lHandle);
            DHThrowLastError(returnValue);
            return returnValue;
        }

        #endregion

        #region << 设备注册 >>

        /// <summary>
        /// 注册用户到设备，当设备端把用户设置为复用（设备默认的用户如admin,不能设置为复用），则使用该帐号可以多次向设备注册
        /// </summary>
        /// <param name="pchDVRIP">设备IP</param>
        /// <param name="wDVRPort">设备端口</param>
        /// <param name="pchUserName">用户名</param>
        /// <param name="pchPassword">用户密码</param>
        /// <param name="lpDeviceInfo">设备信息,属于输出参数</param>
        /// <param name="error">返回登录错误码</param>
        /// <returns>失败返回0，成功返回设备ID</returns>
        public static int DHLogin(string pchDVRIP, ushort wDVRPort, string pchUserName, string pchPassword, out NET_DEVICEINFO lpDeviceInfo, out int error)
        {
            int result = CLIENT_Login(pchDVRIP, wDVRPort, pchUserName, pchPassword, out lpDeviceInfo, out error);
            DHThrowLastError(result);
            return result;
        }

        /// <summary>
        /// 注册用户到设备的扩展接口，支持一个用户指定设备支持的能力
        /// </summary>
        /// <param name="pchDVRIP">设备IP</param>
        /// <param name="wDVRPort">设备端口</param>
        /// <param name="pchUserName">用户名</param>
        /// <param name="pchPassword">用户密码</param>
        /// <param name="nSpecCap">设备支持的能力，值为2表示主动侦听模式下的用户登录。[车载dvr登录]</param>
        /// <param name="pCapParam">对nSpecCap 的补充参数, nSpecCap = 2时，pCapParam填充设备序列号字串。[车载dvr登录]</param>
        /// <param name="lpDeviceInfo">设备信息,属于输出参数</param>
        /// <param name="error">返回登录错误码</param>
        /// <returns>失败返回0，成功返回设备ID</returns>
        public static int DHLogin(string pchDVRIP, ushort wDVRPort, string pchUserName, string pchPassword, int nSpecCap, string pCapParam, out NET_DEVICEINFO lpDeviceInfo, out int error)
        {
            int result = CLIENT_LoginEx(pchDVRIP, wDVRPort, pchUserName, pchPassword, nSpecCap, pCapParam, out lpDeviceInfo, out error);
            DHThrowLastError(result);
            return result;
        }

        /// <summary>
        /// 注销设备用户
        /// </summary>
        /// <param name="lLoginID">设备用户登录ID:<seealso cref="CLIENT_Login"/>的返回值</param>
        /// <returns>true:成功;false:失败</returns>
        public static bool DHLogout(int lLoginID)
        {
            bool returnValue = false;
            returnValue = CLIENT_Logout(lLoginID);
            DHThrowLastError(returnValue);
            return returnValue;
        }

        #endregion

        #region << 设备控制 >>
        //暂时无
        #endregion

        #region << 实时监视 >>

        /// <summary>
        /// 启动实时监视
        /// </summary>
        /// <param name="lLoginID">设备用户登录ID:<seealso cref="CLIENT_Login"/>的返回值</param>
        /// <param name="nChannelID">通道ID</param>
        /// <param name="hWnd">显示容器窗口句柄</param>
        /// <returns>失败返回0，成功返回实时监视ID(实时监视句柄)</returns>
        public static int DHRealPlay(int lLoginID, int nChannelID, IntPtr hWnd)
        {
            int returnValue = CLIENT_RealPlay(lLoginID, nChannelID, hWnd);
            DHThrowLastError(returnValue);
            return returnValue;
        }

        /// <summary>
        /// 启动实时监视(增强版)
        /// </summary>
        /// <param name="lLoginID">设备用户登录ID:<seealso cref="CLIENT_Login"/>的返回值</param>
        /// <param name="nChannelID">通道ID</param>
        /// <param name="realPlayType">实时播放类型</param>
        /// <param name="hWnd">显示容器窗口句柄</param>
        /// <returns>失败返回0，成功返回实时监视ID(实时监视句柄)</returns>
        public static int DHRealPlayEx(int lLoginID, int nChannelID, REALPLAY_TYPE realPlayType,IntPtr hWnd)
        {
            int returnValue = CLIENT_RealPlayEx(lLoginID, nChannelID, hWnd,realPlayType);
            DHThrowLastError(returnValue);
            return returnValue;
        }

        /// <summary>
        /// 停止实时监视
        /// </summary>
        /// <param name="lRealHandle">实时监视句柄:<seealso cref="CLIENT_RealPlay"/>的返回值</param>
        /// <returns>true:成功;false:失败</returns>
        public static bool DHStopRealPlay(int lRealHandle)
        {
            bool returnValue = CLIENT_StopRealPlay(lRealHandle);
            DHThrowLastError(returnValue);
            return returnValue;
        }

        /// <summary>
        /// 停止实时监视(增强版)
        /// </summary>
        /// <param name="lRealHandle">实时监视句柄:<seealso cref="CLIENT_RealPlay"/>的返回值</param>
        /// <returns>true:成功;false:失败</returns>
        public static bool DHStopRealPlayEx(int lRealHandle)
        {
            bool returnValue = CLIENT_StopRealPlayEx(lRealHandle);
            DHThrowLastError(returnValue);
            return returnValue;
        }

        /// <summary>
        /// 开始保存实时监视数据,对前端设备监视的图像进行数据保存,形成录像文件,此数据是设备端传送过来的原始视频数据
        /// </summary>
        /// <param name="lRealHandle">CLIENT_RealPlay的返回值</param>
        /// <param name="pchFileName">实时监视保存文件名</param>
        /// <returns>true:成功;false:失败</returns>
        public static bool DHStartSaveRealData(int lRealHandle, string pchFileName)
        {
            bool returnValue = CLIENT_SaveRealData(lRealHandle, pchFileName);
            DHThrowLastError(returnValue);
            return returnValue;
        }

        /// <summary>
        /// 停止保存实时监视数据,关闭保存的文件
        /// </summary>
        /// <param name="lRealHandle">CLIENT_RealPlay的返回值</param>
        /// <returns>true:成功;false:失败</returns>
        public static bool DHStopSaveRealData(int lRealHandle)
        {
            bool returnValue = CLIENT_StopSaveRealData(lRealHandle);
            DHThrowLastError(returnValue);
            return returnValue;
        }

        /// <summary>
        /// 设置实时监视数据回调，给用户提供设备流出的数据，当   cbRealData为NULL时结束回调数据
        /// </summary>
        /// <param name="lRealHandle">CLIENT_RealPlay的返回值</param>
        /// <param name="cbRealData">回调函数,用于传出设备流出的实时数据</param>
        /// <param name="dwUser">用户数据</param>
        /// <returns>true:成功;false:失败</returns>
        public static bool DHSetRealDataCallBack(int lRealHandle, fRealDataCallBack cbRealData, IntPtr dwUser)
        {
            bool returnValue = CLIENT_SetRealDataCallBack(lRealHandle, cbRealData, dwUser);
            DHThrowLastError(returnValue);
            return returnValue;
        }

        /// <summary>
        /// 设置实时监视数据回调扩展接口 ,是对上一接口的补充, 增加一个回调数据类型标志dwFlag 参数, 可以选择性的回调出需要的数据, 对于没设置回调的数据类型就不回调出来了, 当设置为0x1f时与上一接口效果一样, 不过对回调函数也做了扩展
        /// </summary>
        /// <param name="lRealHandle">CLIENT_RealPlay的返回值</param>
        /// <param name="cbRealData">回调函数,用于传出多种类型的实时数据</param>
        /// <param name="dwFlag">
        /// 是按位来的, 可以组合, 为0x1f时五种数据类型都回调
        /// 0x00000001  等同原来的原始数据
        /// 0x00000002  是MPEG4/H264标准数据
        /// 0x00000004  YUV数据
        /// 0x00000008  PCM数据
        /// 0x00000010  原始音频数据</param>
        /// <param name="dwUser">用户数据</param>
        /// <returns>true:成功;false:失败</returns>
        public static bool DHSetRealDataCallBack(int lRealHandle, fRealDataCallBackEx cbRealData,UInt32 dwFlag, IntPtr dwUser)
        {
            bool returnValue = CLIENT_SetRealDataCallBackEx(lRealHandle, cbRealData, dwUser,dwFlag);
            DHThrowLastError(returnValue);
            return returnValue;
        }

        #endregion

        #region << 云台控制 >>

        /// <summary>
        /// 云台控制
        /// </summary>
        /// <param name="lLoginID">设备用户登录ID:<seealso cref="CLIENT_Login"/>的返回值</param>
        /// <param name="nChannelID">通道号</param>
        /// <param name="dwPTZCommand">云台控制命令<seealso cref="PTZControlType"/>[PTZ_****的命令]</param>
        /// <param name="dwStep">步进/速度</param>
        /// <param name="dwStop">是否停止</param>
        /// <returns>true:成功;false:失败</returns>
        public static bool DHPTZControl(int lLoginID, int nChannelID, PTZ_CONTROL dwPTZCommand, ushort dwStep, bool dwStop)
        {
            bool returnValue = false;
            switch (dwPTZCommand)
            {
                case PTZ_CONTROL.PTZ_APERTURE_ADD_CONTROL:
                case PTZ_CONTROL.PTZ_APERTURE_DEC_CONTROL:
                case PTZ_CONTROL.PTZ_DOWN_CONTROL:
                case PTZ_CONTROL.PTZ_FOCUS_ADD_CONTROL:
                case PTZ_CONTROL.PTZ_FOCUS_DEC_CONTROL:
                //case PTZ_CONTROL.PTZ_LAMP_CONTROL:
                case PTZ_CONTROL.PTZ_LEFT_CONTROL:
                //case PTZ_CONTROL.PTZ_POINT_DEL_CONTROL:
                case PTZ_CONTROL.PTZ_POINT_LOOP_CONTROL:
                //case PTZ_CONTROL.PTZ_POINT_MOVE_CONTROL:
                //case PTZ_CONTROL.PTZ_POINT_SET_CONTROL:
                case PTZ_CONTROL.PTZ_RIGHT_CONTROL:
                case PTZ_CONTROL.PTZ_UP_CONTROL:
                case PTZ_CONTROL.PTZ_ZOOM_ADD_CONTROL:
                case PTZ_CONTROL.PTZ_ZOOM_DEC_CONTROL:
                    returnValue = CLIENT_PTZControl(lLoginID, nChannelID, (ushort)dwPTZCommand, dwStep, dwStop);
                    break;
                case PTZ_CONTROL.PTZ_LAMP_CONTROL:
                    returnValue = CLIENT_DHPTZControlEx(lLoginID, nChannelID, (ushort)dwPTZCommand, dwStep, 0, 0, dwStop);
                    break;
                case PTZ_CONTROL.PTZ_POINT_DEL_CONTROL:                
                case PTZ_CONTROL.PTZ_POINT_MOVE_CONTROL:
                case PTZ_CONTROL.PTZ_POINT_SET_CONTROL:
                    returnValue = CLIENT_DHPTZControlEx(lLoginID, nChannelID, (ushort)dwPTZCommand, 0, dwStep, 0, dwStop);
                    break;

                //case PTZControlType.PTZ_POINT_LOOP_CONTROL:
                //    returnValue = CLIENT_DHPTZControl(lLoginID, nChannelID, (ushort)dwPTZCommand, "", "", dwStep, dwStop);
                //    break;
                case PTZ_CONTROL.EXTPTZ_SETMODESTART:
                case PTZ_CONTROL.EXTPTZ_SETMODESTOP:
                case PTZ_CONTROL.EXTPTZ_RUNMODE:
                case PTZ_CONTROL.EXTPTZ_STOPMODE:
                case PTZ_CONTROL.EXTPTZ_DELETEMODE:
                    returnValue = CLIENT_DHPTZControlEx(lLoginID, nChannelID, (ushort)dwPTZCommand, dwStep, 0, 0, dwStop);
                    break;
                case PTZ_CONTROL.EXTPTZ_STARTPANCRUISE:
                case PTZ_CONTROL.EXTPTZ_STOPPANCRUISE:
                case PTZ_CONTROL.EXTPTZ_SETLEFTBORDER:
                case PTZ_CONTROL.EXTPTZ_RIGHTBORDER:
                case PTZ_CONTROL.EXTPTZ_STARTLINESCAN:
                case PTZ_CONTROL.EXTPTZ_CLOSELINESCAN:
                case PTZ_CONTROL.EXTPTZ_REVERSECOMM:
                case PTZ_CONTROL.EXTPTZ_OPENMENU:
                case PTZ_CONTROL.EXTPTZ_CLOSEMENU:
                case PTZ_CONTROL.EXTPTZ_MENUOK:
                case PTZ_CONTROL.EXTPTZ_MENUCANCEL:
                case PTZ_CONTROL.EXTPTZ_MENUUP:
                case PTZ_CONTROL.EXTPTZ_MENUDOWN:
                case PTZ_CONTROL.EXTPTZ_MENULEFT:
                case PTZ_CONTROL.EXTPTZ_MENURIGHT:
                case PTZ_CONTROL.EXTPTZ_TOTAL:
                    returnValue = CLIENT_DHPTZControlEx(lLoginID, nChannelID, (ushort)dwPTZCommand, 0, 0, 0, dwStop);
                    break;
            }
            
            return returnValue;
        }

        /// <summary>
        /// 扩展云台控制, 对云台控制函数功能的增强控制
        /// </summary>
        /// <param name="lLoginID">设备用户登录ID:<seealso cref="CLIENT_Login"/>的返回值</param>
        /// <param name="nChannelID">通道号</param>
        /// <param name="dwPTZCommand">云台控制命令<seealso cref="PTZControlType"/>接受参数格式参见文档说明</param>
        /// <param name="param1">参数1</param>
        /// <param name="param2">参数2</param>
        /// <param name="param3">参数3</param>
        /// <param name="dwStop">是否停止</param>
        /// <returns>true:成功;false:失败</returns>
        public static bool DHPTZControl(int lLoginID, int nChannelID, PTZ_CONTROL dwPTZCommand, ushort param1, ushort param2, ushort param3, bool dwStop)
        {
            bool returnValue = false;
            switch (dwPTZCommand)
            {
                case PTZ_CONTROL.PTZ_APERTURE_ADD_CONTROL:
                case PTZ_CONTROL.PTZ_APERTURE_DEC_CONTROL:
                case PTZ_CONTROL.PTZ_DOWN_CONTROL:
                case PTZ_CONTROL.PTZ_FOCUS_ADD_CONTROL:
                case PTZ_CONTROL.PTZ_FOCUS_DEC_CONTROL:
                case PTZ_CONTROL.PTZ_LAMP_CONTROL:
                case PTZ_CONTROL.PTZ_LEFT_CONTROL:
                case PTZ_CONTROL.PTZ_POINT_DEL_CONTROL:
                case PTZ_CONTROL.PTZ_POINT_MOVE_CONTROL:
                case PTZ_CONTROL.PTZ_POINT_SET_CONTROL:
                case PTZ_CONTROL.PTZ_RIGHT_CONTROL:
                case PTZ_CONTROL.PTZ_UP_CONTROL:
                case PTZ_CONTROL.PTZ_ZOOM_ADD_CONTROL:
                case PTZ_CONTROL.PTZ_ZOOM_DEC_CONTROL:
                    returnValue = CLIENT_DHPTZControlEx(lLoginID, nChannelID, (ushort)dwPTZCommand, 0, param2, 0, dwStop);
                    break;
                case PTZ_CONTROL.PTZ_POINT_LOOP_CONTROL:
                    returnValue = CLIENT_DHPTZControlEx(lLoginID, nChannelID, (ushort)dwPTZCommand, 0, 0, param3, dwStop);
                    break;
                case PTZ_CONTROL.EXTPTZ_LEFTTOP:
                case PTZ_CONTROL.EXTPTZ_RIGHTTOP:
                case PTZ_CONTROL.EXTPTZ_LEFTDOWN:
                case PTZ_CONTROL.EXTPTZ_RIGHTDOWN:
                case PTZ_CONTROL.EXTPTZ_ADDTOLOOP:
                case PTZ_CONTROL.EXTPTZ_DELFROMLOOP:
                case PTZ_CONTROL.EXTPTZ_AUXIOPEN:
                case PTZ_CONTROL.EXTPTZ_AUXICLOSE:
                    returnValue = CLIENT_DHPTZControlEx(lLoginID, nChannelID, (ushort)dwPTZCommand, param1, param2, 0, dwStop);
                    break;
                case PTZ_CONTROL.EXTPTZ_SETMODESTART:
                case PTZ_CONTROL.EXTPTZ_SETMODESTOP:
                case PTZ_CONTROL.EXTPTZ_RUNMODE:
                case PTZ_CONTROL.EXTPTZ_STOPMODE:
                case PTZ_CONTROL.EXTPTZ_DELETEMODE:
                    returnValue = CLIENT_DHPTZControlEx(lLoginID, nChannelID, (ushort)dwPTZCommand, param1, 0, 0, dwStop);
                    break;
                //case PTZ_CONTROL.EXTPTZ_FASTGOTO:
                //    returnValue = CLIENT_DHPTZControl(lLoginID, nChannelID, (ushort)dwPTZCommand, param1, param2, param3, dwStop);
                //    break;
            }
            return returnValue;
        }

        /// <summary>
        /// 扩展云台控制, 对云台控制函数功能的增强控制
        /// </summary>
        /// <param name="lLoginID">设备用户登录ID:<seealso cref="CLIENT_Login"/>的返回值</param>
        /// <param name="nChannelID">通道号</param>
        /// <param name="dwPTZCommand">云台控制命令<seealso cref="PTZControlType"/>接受只有两个参数的命令</param>
        /// <param name="param1">参数1</param>
        /// <param name="param2">参数2</param>
        /// <param name="param3">参数3</param>
        /// <param name="dwStop">是否停止</param>
        /// <returns>true:成功;false:失败</returns>
        public static bool DHPTZControl(int lLoginID, int nChannelID, PTZ_CONTROL dwPTZCommand, ushort param1, ushort param2, bool dwStop)
        {
            bool returnValue = false;
            switch (dwPTZCommand)
            {
                case PTZ_CONTROL.EXTPTZ_LEFTTOP:
                case PTZ_CONTROL.EXTPTZ_RIGHTTOP:
                case PTZ_CONTROL.EXTPTZ_LEFTDOWN:
                case PTZ_CONTROL.EXTPTZ_RIGHTDOWN:
                case PTZ_CONTROL.EXTPTZ_ADDTOLOOP:
                case PTZ_CONTROL.EXTPTZ_DELFROMLOOP:
                case PTZ_CONTROL.EXTPTZ_AUXIOPEN:
                case PTZ_CONTROL.EXTPTZ_AUXICLOSE:
                    returnValue = CLIENT_DHPTZControl(lLoginID, nChannelID, (ushort)dwPTZCommand, param1, param2, 0, dwStop);
                    break;
            }
            return returnValue;
        }

        /// <summary>
        /// 扩展云台控制, 支持三维快速定位
        /// </summary>
        /// <param name="lLoginID">设备用户登录ID:<seealso cref="CLIENT_Login"/>的返回值</param>
        /// <param name="nChannelID">通道号</param>
        /// <param name="dwPTZCommand">云台控制命令<seealso cref="PTZControlType"/>接受只有两个参数的命令</param>
        /// <param name="param1">参数1</param>
        /// <param name="param2">参数2</param>
        /// <param name="param3">参数3</param>
        /// <param name="dwStop">是否停止</param>
        /// <returns>true:成功;false:失败</returns>
        public static bool DHPTZControl(int lLoginID, int nChannelID, bool blnFastGoto, int  param1, int param2,int param3, bool dwStop)
        {
            bool returnValue = false;
            if (blnFastGoto)
            {
                returnValue = CLIENT_DHPTZControlEx(lLoginID, nChannelID, (ushort)PTZ_CONTROL.EXTPTZ_FASTGOTO, param1, param2, param3, dwStop);
            }
            return returnValue;
        }

        #endregion

        #region << 回放下载 >>

        /// <summary>
        /// 查询录像文件
        /// </summary>
        /// <param name="lLoginID">设备用户登录句柄</param>
        /// <param name="nChannelId">通道ID</param>
        /// <param name="nRecordFileType">录像文件类型 </param>
        /// <param name="tmStart">录像开始时间</param>
        /// <param name="tmEnd">录像结束时间</param>
        /// <param name="pchCardid">卡号,只针对卡号查询有效，其他情况下可以填NULL</param>
        /// <param name="nriFileinfo">返回的录像文件信息，是一个NET_RECORDFILE_INFO结构数组[录像文件信息为指定条]</param>
        /// <param name="maxlen">nriFileinfo缓冲的最大长度;[单位字节，大小为结构数组维数*sizeof(NET_RECORDFILE_INFO),数组维为大小等于1，建议小于２００]</param>
        /// <param name="filecount">返回的文件个数,属于输出参数最大只能查到缓冲满为止的录像记录</param>
        /// <param name="waittime">等待时间</param>
        /// <param name="bTime">是否按时间查(目前无效)</param>
        /// <returns>true:成功;false:失败</returns>
        public static bool DHQueryRecordFile(int lLoginID, int nChannelId, RECORD_FILE_TYPE nRecordFileType, DateTime tmStart, DateTime tmEnd, string pchCardid, ref NET_RECORDFILE_INFO[] nriFileinfo, int maxlen, out  int filecount, int waittime, bool bTime)
        {
            bool returnValue = false;
            filecount = 0;
            IntPtr pBoxInfo = IntPtr.Zero;
            try
            {                
                NET_TIME timeStart = ToNetTime(tmStart);
                NET_TIME timeEnd = ToNetTime(tmEnd);
                pBoxInfo = Marshal.AllocHGlobal(maxlen);//分配固定的指定大小的内存空间
                int fileCountMin = 0;
                if (pBoxInfo != IntPtr.Zero)
                {
                    returnValue = CLIENT_QueryRecordFile(lLoginID, nChannelId, (int)nRecordFileType, ref timeStart, ref timeEnd, pchCardid, pBoxInfo, maxlen, out filecount, waittime, bTime);
                    fileCountMin = (filecount <= nriFileinfo.Length ? filecount : nriFileinfo.Length);
                    for (int dwLoop = 0; dwLoop < fileCountMin; dwLoop++)
                    {
                        //将指定内存空间的数据按指定格式复制到目的数组中
                        nriFileinfo[dwLoop] = (NET_RECORDFILE_INFO)Marshal.PtrToStructure((IntPtr)((UInt32)pBoxInfo + Marshal.SizeOf(typeof(NET_RECORDFILE_INFO)) * dwLoop), typeof(NET_RECORDFILE_INFO));
                    }
                }
                DHThrowLastError(returnValue);
               
            }
            catch (Exception e)
            {
                DHThrowLastError(e);
                returnValue= false;
            }
            finally
            {
                Marshal.FreeHGlobal(pBoxInfo);//释放固定内存分配
                pBoxInfo = IntPtr.Zero;                
            }
            return returnValue;           
        }

        /// <summary>
        /// 查询录像文件
        /// </summary>
        /// <param name="lLoginID">设备用户登录句柄</param>
        /// <param name="nChannelId">通道ID</param>
        /// <param name="nRecordFileType">录像文件类型 </param>
        /// <param name="tmStart">录像开始时间</param>
        /// <param name="tmEnd">录像结束时间</param>
        /// <param name="pchCardid">卡号,只针对卡号查询有效，其他情况下可以填NULL</param>
        /// <param name="nriFileinfo">返回的录像文件信息，是一个NET_RECORDFILE_INFO结构[录像文件信息只有一条]</param>
        /// <param name="maxlen">nriFileinfo缓冲的最大长度;[单位字节大小为sizeof(NET_RECORDFILE_INFO)]</param>
        /// <param name="filecount">返回的文件个数,属于输出参数最大只能查到缓冲满为止的录像记录</param>
        /// <param name="waittime">等待时间</param>
        /// <param name="bTime">是否按时间查(目前无效)</param>
        /// <returns>true:成功;false:失败</returns>
        public static bool DHQueryRecordFile(int lLoginID, int nChannelId, RECORD_FILE_TYPE nRecordFileType, DateTime tmStart, DateTime tmEnd, string pchCardid, ref NET_RECORDFILE_INFO nriFileinfo, int maxlen, out  int filecount, int waittime, bool bTime)
        {
            bool returnValue = false;
            filecount = 0;
            NET_RECORDFILE_INFO[] ntFileInfo = new NET_RECORDFILE_INFO[1];
            returnValue = DHQueryRecordFile(lLoginID, nChannelId, nRecordFileType, tmStart, tmEnd, pchCardid, ref ntFileInfo, maxlen, out filecount, waittime, bTime);
            nriFileinfo = ntFileInfo[0];
            DHThrowLastError(returnValue);
            return returnValue;
        }

        /// <summary>
        /// 网络回放[注意:用户登录一台设备后，每通道同一时间只能播放一则录像,不能同时播放同一通道的多条记录]
        /// </summary>
        /// <param name="lLoginID">设备用户登录句柄</param>
        /// <param name="lpRecordFile">录像文件信息, 当按时间播放是只需填充起始时间和结束时间, 其他项填0</param>
        /// <param name="hWnd">回放容器句柄</param>
        /// <param name="cbDownLoadPos">进度回调函数</param>
        /// <param name="dwUserData">用户自定义数据</param>
        /// <returns>成功返回网络回放ID，失败返回0</returns>
        public static int DHPlayBackByRecordFile(int lLoginID, ref NET_RECORDFILE_INFO lpRecordFile, IntPtr hWnd, fDownLoadPosCallBack cbDownLoadPos, IntPtr dwUserData)
        {
            int returnValue;
            returnValue = CLIENT_PlayBackByRecordFile(lLoginID, ref lpRecordFile, hWnd, cbDownLoadPos, dwUserData);
            DHThrowLastError(returnValue);
            return returnValue;
        }

        /// <summary>
        /// 带数据回调的按录像文件回放扩展接口,每通道同一时间只能播放一则录像,不能同时播放同一通道的多条记录。窗口参数（hWnd）有效时回调数据的返回值将被忽略，窗口参数 (hWnd)为0时，需要注意回调函数的返回值，具体见回调函数说明。
        /// </summary>
        /// <param name="lLoginID">CLIENT_Login的返回值</param>
        /// <param name="lpRecordFile">录像文件信息</param>
        /// <param name="hWnd">回放容器句柄</param>
        /// <param name="cbDownLoadPos">进度回调函数</param>
        /// <param name="dwPosUser">进度回调函数用户数据</param>
        /// <param name="fDownLoadDataCallBack">数据回调函数</param>
        /// <param name="dwDataUser">数据回调函数用户数据</param>
        /// <returns>成功返回网络回放ID，失败返回0</returns>
        public static int DHPlayBackByRecordFile(int lLoginID, ref NET_RECORDFILE_INFO lpRecordFile, IntPtr hWnd, fDownLoadPosCallBack cbDownLoadPos, IntPtr dwPosUser, fDataCallBack fDownLoadDataCallBack, IntPtr dwDataUser)
        {
            int returnValue;
            returnValue = CLIENT_PlayBackByRecordFileEx(lLoginID, ref lpRecordFile, hWnd, cbDownLoadPos, dwPosUser,fDownLoadDataCallBack,dwDataUser);
            DHThrowLastError(returnValue);
            return returnValue;
        }

        /// <summary>
        /// 网络按时间回放
        /// </summary>
        /// <param name="lLoginID">设备用户登录句柄</param>
        /// <param name="nChannelID">通道ID</param>
        /// <param name="lpStartTime">录像开始时间</param>
        /// <param name="lpStopTime">录像结束时间</param>
        /// <param name="hWnd">录像回放容器句柄</param>
        /// <param name="cbDownLoadPos">进度回调函数</param>
        /// <param name="dwUserData">用户自定义数据</param>
        /// <returns>成功返回网络回放ID，失败返回0</returns>
        public static int DHPlayBackByTime(int lLoginID, int nChannelID, DateTime tmStart, DateTime tmEnd, IntPtr hWnd, fDownLoadPosCallBack cbDownLoadPos, IntPtr dwUserData)
        {
            
            try
            {
                int returnValue = 0;
                NET_TIME timeStart = ToNetTime(tmStart);
                NET_TIME timeEnd = ToNetTime(tmEnd);
                returnValue = CLIENT_PlayBackByTime(lLoginID, nChannelID, ref timeStart, ref  timeEnd, hWnd, cbDownLoadPos, dwUserData);
                DHThrowLastError(returnValue);
                return returnValue;
            }
            catch (Exception e)
            {
                DHThrowLastError(e);
                return 0;
            }
            
        }

        /// <summary>
        /// 网络按时间回放
        /// </summary>
        /// <param name="lLoginID">设备用户登录句柄</param>
        /// <param name="nChannelID">通道ID</param>
        /// <param name="tmStart">录像开始时间</param>
        /// <param name="tmEnd">录像结束时间</param>
        /// <param name="hWnd">录像回放容器句柄</param>
        /// <param name="cbDownLoadPos">进度回调函数</param>
        /// <param name="dwPosUser">用户自定义数据[进度]</param>
        /// <param name="cbData">数据回调函数</param>
        /// <param name="dwDataUser">用户自定义数据[进度]</param>
        /// <returns>成功返回网络回放ID，失败返回0</returns>
        public static int DHPlayBackByTime(int lLoginID, int nChannelID, DateTime tmStart, DateTime tmEnd, IntPtr hWnd, fDownLoadPosCallBack cbDownLoadPos, IntPtr dwPosUser,fDataCallBack cbData,IntPtr dwDataUser)
        {
            try
            {
                int returnValue = 0;
                NET_TIME timeStart = ToNetTime(tmStart);
                NET_TIME timeEnd = ToNetTime(tmEnd);
                returnValue = CLIENT_PlayBackByTimeEx(lLoginID, nChannelID, ref timeStart, ref  timeEnd, hWnd, cbDownLoadPos, dwPosUser,cbData,dwDataUser);
                DHThrowLastError(returnValue);
                return returnValue;
            }
            catch (Exception e)
            {
                DHThrowLastError(e);
                return 0;
            }

        }

        /// <summary>
        /// 回放播放控制:播放，暂停，停止，单步播放，单步停止，慢放，快放
        /// </summary>
        /// <param name="lPlayHandle">播放句柄</param>
        /// <param name="pPlayCommand">控制命令:参见<seealso cref="PlayControlType"/></param>
        /// <returns>true:成功;false:失败</returns>
        public static bool DHPlayBackControl(int lPlayHandle, PLAY_CONTROL pPlayCommand)
        {
            bool returnValue = false;
            switch (pPlayCommand)
            {
                case PLAY_CONTROL.Play:
                    returnValue = CLIENT_PausePlayBack(lPlayHandle, 0);
                    break;
                case PLAY_CONTROL.Pause:
                    returnValue = CLIENT_PausePlayBack(lPlayHandle, 1);
                    break;
                case PLAY_CONTROL.Stop:
                    returnValue = CLIENT_StopPlayBack(lPlayHandle);
                    break;
                case PLAY_CONTROL.StepPlay:
                    returnValue = CLIENT_StepPlayBack(lPlayHandle, false);
                    break;
                case PLAY_CONTROL.StepStop:
                    returnValue = CLIENT_StepPlayBack(lPlayHandle, true);
                    break;
                case PLAY_CONTROL.Fast:
                    returnValue = CLIENT_FastPlayBack(lPlayHandle);
                    break;
                case PLAY_CONTROL.Slow:
                    returnValue = CLIENT_SlowPlayBack(lPlayHandle);
                    break;
            }
            DHThrowLastError(returnValue);
            return returnValue;
        }

        /// <summary>
        /// 回放播放控制:改变位置播放[即拖动播放，只有打开图像的函数参数hWnd有效时该函数获取的参数才有效，否则无意义]
        /// </summary>
        /// <param name="lPlayHandle">播放句柄</param>
        /// <param name="pPlayCommand">播放命令:SeekByBit,SeekByTime</param>
        /// <param name="offset">SeekByTime:相对文件开始处偏移时间,单位为秒;SeekByBit:相对文件开始处偏移字节;</param>
        /// <returns>true:成功;false:失败</returns>
        public static bool DHPlayBackControl(int lPlayHandle, PLAY_CONTROL pPlayCommand, uint offset)
        {
            bool returnValue = false;
            uint FalseValue = 0xFFFFFFFF;
            switch (pPlayCommand)
            {
                case PLAY_CONTROL.SeekByBit:
                    returnValue = CLIENT_SeekPlayBack(lPlayHandle, FalseValue, offset);
                    break;
                case PLAY_CONTROL.SeekByTime:
                    returnValue = CLIENT_SeekPlayBack(lPlayHandle, offset, FalseValue);
                    break;
            }
            DHThrowLastError(returnValue);
            return returnValue;
        }

        /// <summary>
        /// 按文件下载录像文件, 通过查询到的文件信息下载
        /// </summary>
        /// <param name="lLoginID">CLIENT_Login的返回值</param>
        /// <param name="lpRecordFile">录像文件信息</param>
        /// <param name="sSavedFileName">要保存的录像文件名，全路径</param>
        /// <param name="cbDownLoadPos">下载进度回调函数</param>
        /// <param name="dwUserData">下载进度回调用户自定义数据</param>
        /// <returns>成功返回下载ID，失败返回0</returns>
        public static int DHDownloadByRecordFile(int lLoginID, NET_RECORDFILE_INFO lpRecordFile,string sSavedFileName,  fDownLoadPosCallBack cbDownLoadPos, IntPtr dwUserData)
        {
            int intReturnValue = 0;
            intReturnValue=CLIENT_DownloadByRecordFile(lLoginID, ref  lpRecordFile,sSavedFileName, cbDownLoadPos, dwUserData);            
            DHThrowLastError();            
            return intReturnValue;
        }

        /// <summary>
        /// 按时间下载，直接输入指定通道起始时间和结束时间下载放录像
        /// </summary>
        /// <param name="lLoginID">CLIENT_Login的返回值</param>
        /// <param name="nChannelId">图像通道号，从0开始</param>
        /// <param name="nRecordFileType">保留</param>
        /// <param name="tmStart">开始时间</param>
        /// <param name="tmEnd">结束时间</param>
        /// <param name="sSavedFileName">要保存的录像文件名，全路径</param>
        /// <param name="cbTimeDownLoadPos">下载进度回调函数</param>
        /// <param name="dwUserData">下载进度回调用户自定义数据</param>
        /// <returns>成功返回下载ID，失败返回0</returns>
        public static int DHDownloadByTime(int lLoginID, int nChannelId, int nRecordFileType, DateTime tmStart, DateTime tmEnd, string sSavedFileName, fDownLoadPosCallBack cbTimeDownLoadPos, IntPtr dwUserData)
        {
            int intReturnValue = 0;
            NET_TIME ntStart = ToNetTime(tmStart);
            NET_TIME ntEnd = ToNetTime(tmEnd);
            intReturnValue = CLIENT_DownloadByTime(lLoginID, nChannelId, nRecordFileType, ref ntStart, ref ntEnd, sSavedFileName, cbTimeDownLoadPos, dwUserData);
            DHThrowLastError();
            return intReturnValue;
        }

        /// <summary>
        /// 停止下载录像文件
        /// </summary>
        /// <param name="lFileHandle">CLIENT_DownloadByRecordFile的返回值</param>
        /// <returns>true:成功;false:失败</returns>
        public static bool DHStopDownload(int lFileHandle)
        {
            bool blnReturnValue = false;
            blnReturnValue = CLIENT_StopDownload(lFileHandle);
            DHThrowLastError();
            return blnReturnValue;
        }

        /// <summary>
        /// 获得下载录像的当前位置,可以用于不需要实时显示下载进度的接口,与下载回调函数的功能类似
        /// </summary>
        /// <param name="lFileHandle">CLIENT_DownloadByRecordFile的返回值</param>
        /// <param name="nTotalSize">下载的总长度,单位KB</param>
        /// <param name="nDownLoadSize">已下载的长度,单位KB</param>
        /// <returns>true:成功;false:失败</returns>
        public static bool DHGetDownloadPos(int lFileHandle, out int nTotalSize, out int nDownLoadSize)
        {
            bool blnReturnValue = false;
            blnReturnValue = CLIENT_GetDownloadPos(lFileHandle, out nTotalSize, out  nDownLoadSize);
            DHThrowLastError();
            return blnReturnValue;
        }

        #endregion

        #region << 视频抓图 >>

        /// <summary>
        /// 保存图片,对显示图像进行瞬间抓图，只有打开图像的函数参数hWnd有效时该函数获取的参数才有效，否则无意义
        /// </summary>
        /// <param name="hPlayHandle">CLIENT_RealPlay的返回值</param>
        /// <param name="pchPicFileName">位图文件名，当前只支持BMP位图</param>
        /// <returns>true:成功;false:失败</returns>
        public static bool DHCapturePicture(int lPlayHandle, string phPicFileName)
        { 
            return CLIENT_CapturePicture(lPlayHandle,phPicFileName);
        }

        #endregion

        #endregion

        #endregion

        #region << 公共函数 >>

        /// <summary>
        /// Windows系统标准时间格式转为自定义格式
        /// </summary>
        /// <param name="dateTime">系统时间对象</param>
        /// <returns>自定义时间格式的时间数据</returns>
        private static NET_TIME ToNetTime(DateTime dateTime)
        {
            NET_TIME result = new NET_TIME();
            result.dwYear = dateTime.Year;
            result.dwMonth = dateTime.Month;
            result.dwDay = dateTime.Day;
            result.dwHour = dateTime.Hour;
            result.dwMinute = dateTime.Minute;
            result.dwSecond = dateTime.Second;
            return result;
        }

        /// <summary>
        /// 特列UInt32类型值转换成指定的格式
        /// </summary>
        /// <param name="formatStyle">
        /// 格式参数:
        /// yyyy表示日期的年(4位);yy表示日期的年(2位);
        /// m表示日期的月(自动);mm表示日期的月(2位);
        /// d表示日期的天(自动);dd表示日期的天(2位);
        /// p1表示IP地址的第一部分;p2表示IP地址的第二部分;p3表示IP地址的第三部分;p4表示IP地址的第四部分;
        /// v1表示版本号的第一部分;v2表示版本号的第2部分;[版本号格式:主版本号.次版本号]
        /// vs1表示版本号的第一部分;vs2表示版本号的第2部分;vs3表示版本号的第3部分;vs4表示版本号的第4部分;[版本号格式:x.x.x.x]
        /// </param>
        /// <returns></returns>
        public static string DHUInt32ToString(UInt32 intValue,string formatStyle)
        {
            //转换为16进制表示值       
            string valueString = intValue.ToString("X");
            string strReturn = formatStyle.ToUpper();
            //格式化
            string strTemp = "00000000";
            strTemp = strTemp.Remove(0, valueString.Length) + valueString;
            string strPart1 = strTemp.Substring(0, 2);
            string strPart2 = strTemp.Substring(2, 2);
            string strPart3 = strTemp.Substring(4, 2);
            string strPart4 = strTemp.Substring(6, 2);
            //日期年的处理
            if (strReturn.IndexOf("YYYY") != -1)
            {
                strReturn = strReturn.Replace("YYYY", int.Parse(strPart1 + strPart2, System.Globalization.NumberStyles.AllowHexSpecifier).ToString());
            }
            if (strReturn.IndexOf("YY") != -1)
            {
                strReturn = strReturn.Replace("YY", (int.Parse(strPart1 + strPart2, System.Globalization.NumberStyles.AllowHexSpecifier) % 100).ToString());
            }
            //日期月的处理
            if (strReturn.IndexOf("MM") != -1)
            {
                int mm = int.Parse(strPart3, System.Globalization.NumberStyles.AllowHexSpecifier);
                strReturn = strReturn.Replace("MM", ((mm>10?mm.ToString():"0"+mm.ToString())));
            }
            if (strReturn.IndexOf("M") != -1)
            {
                strReturn = strReturn.Replace("M", int.Parse(strPart3, System.Globalization.NumberStyles.AllowHexSpecifier).ToString());
            }
            //日期日的处理
            if (strReturn.IndexOf("DD") != -1)
            {
                int dd = int.Parse(strPart4, System.Globalization.NumberStyles.AllowHexSpecifier);
                strReturn = strReturn.Replace("DD", ((dd > 10 ? dd.ToString() : "0" + dd.ToString())));
            }
            if (strReturn.IndexOf("D") != -1)
            {
                strReturn = strReturn.Replace("D", int.Parse(strPart4, System.Globalization.NumberStyles.AllowHexSpecifier).ToString());
            }
            //IP地址的处理
            if (strReturn.IndexOf("P1") != -1)
            {
                strReturn = strReturn.Replace("P1", int.Parse(strPart1, System.Globalization.NumberStyles.AllowHexSpecifier).ToString());
            }
            if (strReturn.IndexOf("P2") != -1)
            {
                strReturn = strReturn.Replace("P2", int.Parse(strPart2, System.Globalization.NumberStyles.AllowHexSpecifier).ToString());
            }
            if (strReturn.IndexOf("P3") != -1)
            {
                strReturn = strReturn.Replace("P3", int.Parse(strPart1, System.Globalization.NumberStyles.AllowHexSpecifier).ToString());
            }
            if (strReturn.IndexOf("P4") != -1)
            {
                strReturn = strReturn.Replace("P4", int.Parse(strPart1, System.Globalization.NumberStyles.AllowHexSpecifier).ToString());
            }
            //版本号的处理[主版本号.次版本号]
            if (strReturn.IndexOf("V1") != -1)
            {
                strReturn = strReturn.Replace("V1", int.Parse(strPart1 + strPart2, System.Globalization.NumberStyles.AllowHexSpecifier).ToString());
            }
            if (strReturn.IndexOf("V2") != -1)
            {
                strReturn = strReturn.Replace("V2", int.Parse(strPart3 + strPart4, System.Globalization.NumberStyles.AllowHexSpecifier).ToString());
            }
            //版本号的处理[X.X.X.X]
            if (strReturn.IndexOf("VS1") != -1)
            {
                strReturn = strReturn.Replace("VS1", int.Parse(strPart1, System.Globalization.NumberStyles.AllowHexSpecifier).ToString());
            }
            if (strReturn.IndexOf("VS2") != -1)
            {
                strReturn = strReturn.Replace("VS2", int.Parse(strPart2, System.Globalization.NumberStyles.AllowHexSpecifier).ToString());
            }
            if (strReturn.IndexOf("VS3") != -1)
            {
                strReturn = strReturn.Replace("VS3", int.Parse(strPart1, System.Globalization.NumberStyles.AllowHexSpecifier).ToString());
            }
            if (strReturn.IndexOf("VS4") != -1)
            {
                strReturn = strReturn.Replace("VS4", int.Parse(strPart1, System.Globalization.NumberStyles.AllowHexSpecifier).ToString());
            }
            return strReturn;
        }

        /// <summary>
        /// 字符数组转换为字符串
        /// </summary>
        /// <param name="charValue">字符数组</param>
        /// <returns>字符串</returns>
        public static string DHByteArrayToString(byte[] byteArray)
        {
            //string result = Encoding.GetEncoding("gb2312").GetString(byteArray);
            //string result = Encoding.GetEncoding(936).GetString(byteArray);
            string result = pEncoding.GetString(byteArray);
            return result;
        }

        /// <summary>
        /// 字符数组转换为字符串[仅适用于字符数据长度为16的字符数组对象]
        /// </summary>
        /// <param name="charValue">字符数组</param>
        /// <param name="formatStyle">字符串格式[不区分大小写] IP1:IP地址的第一部分;IP2:IP地址第二部分;IP3:IP地址的第三部分;IP4:IP地址的第四部分</param>
        /// <returns>字符串</returns>
        public static string DHByteArrayToString(byte[] byteArray,string formatStyle)
        {
            //string result = Encoding.GetEncoding("gb2312").GetString(byteArray);
            //string result = Encoding.GetEncoding(936).GetString(byteArray);
            string result = pEncoding.GetString(byteArray);
            if (result.Length == 16)
            {
                string sPart1 = result.Substring(0, 4);
                string sPart2 = result.Substring(4, 4);
                string sPart3 = result.Substring(8, 4);
                string sPart4 = result.Substring(12, 4);
                string strTemp = formatStyle.ToUpper();
                //IP地址格式处理
                if (strTemp.IndexOf("IP1")!=-1)
                {
                    strTemp = strTemp.Replace("IP1", int.Parse(sPart1).ToString());
                }
                if (strTemp.IndexOf("IP2") != -1)
                {
                    strTemp = strTemp.Replace("IP2", int.Parse(sPart2).ToString());
                }
                if (strTemp.IndexOf("IP3") != -1)
                {
                    strTemp = strTemp.Replace("IP3", int.Parse(sPart3).ToString());
                }
                if (strTemp.IndexOf("IP4") != -1)
                {
                    strTemp = strTemp.Replace("IP4", int.Parse(sPart4).ToString());
                }
                result = strTemp;
            }
            return result;
        }


        /// <summary>
        /// 字符串转换为字符数组
        /// </summary>
        /// <param name="strValue"></param>
        /// <returns></returns>
        public static bool DHStringToByteArry(string strValue,ref byte[] byteArry)
        {
            try
            {
                //byte[] byteTemp = Encoding.GetEncoding("gb2312").GetBytes(strValue);
                byte[] byteTemp = pEncoding.GetBytes(strValue);
                int maxLen=(byteTemp.Length>byteArry.Length?byteArry.Length:byteTemp.Length);
                for (int i = 0; i < byteArry.Length; i++)
                {
                    if (i < maxLen)
                    {
                        byteArry[i] = byteTemp[i];
                    }
                    else
                    {
                        byteArry[i] = new byte();
                    }
                }
                    return true;
            }
            catch
            {
                return false;
            }
        }

        #region **IP地址转换为字符数组功能代码[暂不用]**

        ///// <summary>
        ///// 将IP地址转为字符数组
        ///// </summary>
        ///// <param name="P1">IP地址的第一部分</param>
        ///// <param name="P2">IP地址的第二部分</param>
        ///// <param name="P3">IP地址的第三部分</param>
        ///// <param name="P4">IP地址的第四部分</param>
        ///// <returns></returns>
        //public static char[] DHIPAddToCharArry(string P1, string P2, string P3, string P4)
        //{
        //    char[] result = new char[16];
        //    string strResult = "";
        //    string strTemp = "0000";
        //    strTemp = strTemp.Remove(0, P1.ToString().Length) + P1.ToString();
        //    strResult+=strTemp;
        //    strTemp = "0000";
        //    strTemp = strTemp.Remove(0, P2.ToString().Length) + P2.ToString();
        //    strResult += strTemp;
        //    strTemp = "0000";
        //    strTemp = strTemp.Remove(0, P3.ToString().Length) + P3.ToString();
        //    strResult += strTemp;
        //    strTemp = "0000";
        //    strTemp = strTemp.Remove(0, P4.ToString().Length) + P4.ToString();
        //    strResult += strTemp;
        //    strResult.CopyTo(0, result, 0, 16);
        //    return result;
        //}

        ///// <summary>
        ///// 将IP地址的字符串转为字符数组
        ///// </summary>
        ///// <param name="IPAdd">IP地址的字符串</param>
        ///// <param name="strSprtr">分隔符</param>
        ///// <returns></returns>
        //public static char[] DHIPAddToCharArry(string IPAdd,char chrSprtr)
        //{
        //    string[] strPart = new string[4];
        //    strPart = IPAdd.Split(chrSprtr);
        //    if (strPart.Length ==4)
        //    {
        //        return DHIPAddToCharArry(strPart[0], strPart[1], strPart[2], strPart[3]);
        //    }
        //    else
        //    {
        //        return new char[16];
        //    }
        //}
        #endregion

        #endregion

        #region << 原SDK调用 >>

        /// <summary>
        /// 返回函数执行失败代码
        /// </summary>
        /// <returns>执行失败代码</returns>
        [DllImport("dhnetsdk.dll")]
        private static extern Int32 CLIENT_GetLastError();

        /// <summary>
        /// 初始化SDK
        /// </summary>
        /// <param name="cbDisConnect">
        /// 断线回调函数,参见委托<seealso cref="fDisConnect"/>
        /// </param>
        /// <param name="dwUser">用户信息</param>
        /// <returns>true:成功;false:失败</returns>
        [DllImport("dhnetsdk.dll")]
        private static extern bool CLIENT_Init(fDisConnect cbDisConnect, IntPtr dwUser);

        /// <summary>
        /// 清空SDK, 释放占用的资源，在所有的SDK函数之后调用
        /// </summary>
        [DllImport("dhnetsdk.dll")]
        private static extern void CLIENT_Cleanup();

        /// <summary>
        /// 设置与设备的连接等待时间
        /// </summary>
        /// <param name="nWaitTime">连接等待时间[单位:毫秒]</param>
        /// <param name="nTryTimes">连接次数</param>
        [DllImport("dhnetsdk.dll")]
        private static extern void CLIENT_SetConnectTime(int nWaitTime, int nTryTimes);

        /// <summary>
        /// 设置设备消息回调函数, 用来得到设备当前状态信息
        /// </summary>
        /// <param name="cbMessage">消息回调参数,参见委托<seealso cref="fMessCallBack"/></param>
        /// <param name="dwUser">用户数据</param>
        [DllImport("dhnetsdk.dll")]
        private static extern void CLIENT_SetDVRMessCallBack(fMessCallBack cbMessage, IntPtr dwUser);        

        /// <summary>
        /// 开始对某个设备订阅消息,用来设置是否需要对设备消息回调，得到的消息从<seealso cref="CLIENT_SetDVRMessCallBack"/>的设置值回调出来。
        /// </summary>
        /// <param name="lLoginID">设备用户登录ID:<seealso cref="CLIENT_Login"/>的返回值</param>
        /// <returns>true:成功;false:失败</returns>
        [DllImport("dhnetsdk.dll")]
        private static extern bool CLIENT_StartListenEx(int lLoginID);

        /// <summary>
        /// 开始对某个设备订阅消息,用来设置是否需要对设备消息回调，得到的消息从<seealso cref="CLIENT_SetDVRMessCallBack"/>的设置值回调出来。
        /// </summary>
        /// <param name="lLoginID">设备用户登录ID:<seealso cref="CLIENT_Login"/>的返回值</param>
        /// <returns>true:成功;false:失败</returns>
        [DllImport("dhnetsdk.dll")]
        private static extern bool CLIENT_StartListen(int lLoginID);

        /// <summary>
        /// 停止对某个设备侦听消息
        /// </summary>
        /// <param name="lLoginID">设备用户登录ID:<seealso cref="CLIENT_Login"/>的返回值</param>
        /// <returns>true:成功;false:失败</returns>
        [DllImport("dhnetsdk.dll")]
        private static extern bool CLIENT_StopListen(int lLoginID);

        /// <summary>
        /// 启动监听服务, 目前只实现了报警监听功能
        /// </summary>
        /// <param name="wPort">启动监听的端口</param>
        /// <param name="pIp">绑定的IP，为NULL时绑定本机所有合法IP</param>
        /// <param name="pfscb">服务器的消息回调接口</param>
        /// <param name="dwTimeOut">服务器维护连接的超时时间</param>
        /// <param name="dwUserData">用户回调的自定义数据</param>
        /// <returns>成功返回服务器句柄，失败返回0</returns>
        [DllImport("dhnetsdk.dll")]
        private static extern int CLIENT_StartService(ushort wPort, string pIp, fServiceCallBack pfscb, IntPtr dwTimeOut, IntPtr dwUserData);

        /// <summary>
        /// 停止端口监听服务
        /// </summary>
        /// <param name="lHandle">
        /// 要关闭的服务器的句柄:<seealso cref="CLIENT_StartService"/>的返回值</param>
        /// <returns>true:成功;false:失败</returns>
        [DllImport("dhnetsdk.dll")]
        private static extern bool CLIENT_StopService(int lHandle);

        /// <summary>
        /// 注册用户到设备，当设备端把用户设置为复用（设备默认的用户如admin,不能设置为复用），则使用该帐号可以多次向设备注册
        /// </summary>
        /// <param name="pchDVRIP">设备IP</param>
        /// <param name="wDVRPort">设备端口</param>
        /// <param name="pchUserName">用户名</param>
        /// <param name="pchPassword">用户密码</param>
        /// <param name="lpDeviceInfo">设备信息,属于输出参数</param>
        /// <param name="error">返回登录错误码</param>
        /// <returns>失败返回0，成功返回设备ID</returns>
        [DllImport("dhnetsdk.dll")]
        private static extern int CLIENT_Login(string pchDVRIP, ushort wDVRPort, string pchUserName, string pchPassword, out NET_DEVICEINFO lpDeviceInfo, out int error);

        /// <summary>
        /// 注册用户到设备的扩展接口，支持一个用户指定设备支持的能力
        /// </summary>
        /// <param name="pchDVRIP">设备IP</param>
        /// <param name="wDVRPort">设备端口</param>
        /// <param name="pchUserName">用户名</param>
        /// <param name="pchPassword">用户密码</param>
        /// <param name="nSpecCap">设备支持的能力，值为2表示主动侦听模式下的用户登录。[车载dvr登录]</param>
        /// <param name="pCapParam">对nSpecCap 的补充参数, nSpecCap = 2时，pCapParam填充设备序列号字串。[车载dvr登录]</param>
        /// <param name="lpDeviceInfo">设备信息,属于输出参数</param>
        /// <param name="error">返回登录错误码</param>
        /// <returns>失败返回0，成功返回设备ID</returns>
        [DllImport("dhnetsdk.dll")]
        private static extern int CLIENT_LoginEx(string pchDVRIP, ushort wDVRPort, string pchUserName, string pchPassword, int nSpecCap, string pCapParam, out NET_DEVICEINFO lpDeviceInfo, out int error);

        /// <summary>
        /// 注销设备用户
        /// </summary>
        /// <param name="lLoginID">设备用户登录ID:<seealso cref="CLIENT_Login"/>的返回值</param>
        /// <returns>true:成功;false:失败</returns>
        [DllImport("dhnetsdk.dll")]
        private static extern bool CLIENT_Logout(int lLoginID);

        /// <summary>
        /// 启动实时监视
        /// </summary>
        /// <param name="lLoginID">设备用户登录ID:<seealso cref="CLIENT_Login"/>的返回值</param>
        /// <param name="nChannelID">通道ID</param>
        /// <param name="hWnd">显示容器窗口句柄</param>
        /// <returns>失败返回0，成功返回实时监视ID(实时监视句柄)</returns>
        [DllImport("dhnetsdk.dll")]
        private static extern int CLIENT_RealPlay(int lLoginID, int nChannelID, IntPtr hWnd);

        /// <summary>
        /// 停止实时监视
        /// </summary>
        /// <param name="lRealHandle">实时监视句柄:<seealso cref="CLIENT_RealPlay"/>的返回值</param>
        /// <returns>true:成功;false:失败</returns>
        [DllImport("dhnetsdk.dll")]
        private static extern bool CLIENT_StopRealPlay(int lRealHandle);

        /// <summary>
        /// 停止实时监视
        /// </summary>
        /// <param name="lRealHandle">实时监视句柄:<seealso cref="CLIENT_RealPlay"/>的返回值</param>
        /// <returns>true:成功;false:失败</returns>
        [DllImport("dhnetsdk.dll")]
        private static extern bool CLIENT_StopRealPlayEx(int lRealHandle);

        /// <summary>
        /// 设置实时监视数据回调，给用户提供设备流出的数据，当   cbRealData为NULL时结束回调数据
        /// </summary>
        /// <param name="lRealHandle">CLIENT_RealPlay的返回值</param>
        /// <param name="cbRealData">回调函数,用于传出设备流出的实时数据</param>
        /// <param name="dwUser">用户数据</param>
        /// <returns>true:成功;false:失败</returns>
        [DllImport("dhnetsdk.dll")]
        private static extern bool CLIENT_SetRealDataCallBack(int lRealHandle, fRealDataCallBack cbRealData, IntPtr dwUser);

        /// <summary>
        /// 设置实时监视数据回调扩展接口,是对上一接口的补充, 增加一个回调数据类型标志dwFlag 参数, 
        /// 可以选择性的回调出需要的数据, 对于没设置回调的数据类型就不回调出来了, 当设置为0x1f时与
        /// 上一接口效果一样, 不过对回调函数也做了扩展。
        /// </summary>
        /// <param name="lRealHandle">CLIENT_RealPlay的返回值</param>
        /// <param name="cbRealData">回调函数,用于传出设备流出的实时数据</param>
        /// <param name="dwUser">用户数据</param>
        /// <param name="dwFlag">数据类型标志</param>
        /// <returns>true:成功;false:失败</returns>
        [DllImport("dhnetsdk.dll")]
        private static extern bool CLIENT_SetRealDataCallBackEx(int lRealHandle, fRealDataCallBackEx cbRealData, IntPtr dwUser, UInt32 dwFlag);

        /// <summary>
        /// 云台控制
        /// </summary>
        /// <param name="lLoginID">设备用户登录ID:<seealso cref="CLIENT_Login"/>的返回值</param>
        /// <param name="nChannelID">通道号</param>
        /// <param name="dwPTZCommand">云台控制命令<seealso cref="PTZControlType"/></param>
        /// <param name="dwStep">步进/速度</param>
        /// <param name="dwStop">是否停止</param>
        /// <returns>true:成功;false:失败</returns>
        [DllImport("dhnetsdk.dll")]
        private static extern bool CLIENT_PTZControl(int lLoginID, int nChannelID, ushort dwPTZCommand, ushort dwStep, bool dwStop);

        /// <summary>
        /// 扩展云台控制, 对云台控制函数功能的增强控制
        /// </summary>
        /// <param name="lLoginID">设备用户登录ID:<seealso cref="CLIENT_Login"/>的返回值</param>
        /// <param name="nChannelID">通道号</param>
        /// <param name="dwPTZCommand">云台控制命令</param>
        /// <param name="param1">参数1</param>
        /// <param name="param2">参数2</param>
        /// <param name="param3">参数3</param>
        /// <param name="dwStop">是否停止</param>
        /// <returns>true:成功;false:失败</returns>
        [DllImport("dhnetsdk.dll")]
        private static extern bool CLIENT_DHPTZControl(int lLoginID, int nChannelID, ushort dwPTZCommand, ushort param1, ushort param2, ushort param3, bool dwStop);

        /// <summary>
        /// 扩展云台控制, 支持三维快速定位
        /// </summary>
        /// <param name="lLoginID">设备用户登录ID:<seealso cref="CLIENT_Login"/>的返回值</param>
        /// <param name="nChannelID">通道号</param>
        /// <param name="dwPTZCommand">云台控制命令</param>
        /// <param name="param1">参数1</param>
        /// <param name="param2">参数2</param>
        /// <param name="param3">参数3</param>
        /// <param name="dwStop">是否停止</param>
        /// <returns>true:成功;false:失败</returns>
        [DllImport("dhnetsdk.dll")]
        private static extern bool CLIENT_DHPTZControlEx(int lLoginID, int nChannelID, ushort dwPTZCommand, int lParam1, int lParam2, int lParam3, bool dwStop);

        /// <summary>
        /// 查询录像文件
        /// </summary>
        /// <param name="lLoginID">设备用户登录句柄</param>
        /// <param name="nChannelId">通道ID</param>
        /// <param name="nRecordFileType">录像文件类型 </param>
        /// <param name="tmStart">录像开始时间</param>
        /// <param name="tmEnd">录像结束时间</param>
        /// <param name="pchCardid">卡号,只针对卡号查询有效，其他情况下可以填NULL</param>
        /// <param name="nriFileinfo">返回的录像文件信息，是一个NET_RECORDFILE_INFO结构数组</param>
        /// <param name="maxlen">nriFileinfo缓冲的最大长度;（单位字节，建议在100-200*sizeof(NET_RECORDFILE_INFO)之间）</param>
        /// <param name="filecount">返回的文件个数,属于输出参数最大只能查到缓冲满为止的录像记录</param>
        /// <param name="waittime">等待时间</param>
        /// <param name="bTime">是否按时间查(目前无效)</param>
        /// <returns>true:成功;false:失败</returns>
        [DllImport("dhnetsdk.dll")]
        private static extern bool CLIENT_QueryRecordFile(int lLoginID, int nChannelId, int nRecordFileType, ref NET_TIME tmStart, ref NET_TIME tmEnd, string pchCardid, IntPtr nriFileinfo, int maxlen, out  int filecount, int waittime, bool bTime);

        /// <summary>
        /// 网络回放[注意:用户登录一台设备后，每通道同一时间只能播放一则录像,不能同时播放同一通道的多条记录]
        /// </summary>
        /// <param name="lLoginID">设备用户登录句柄</param>
        /// <param name="lpRecordFile">录像文件信息, 当按时间播放是只需填充起始时间和结束时间, 其他项填0</param>
        /// <param name="hWnd">回放容器句柄</param>
        /// <param name="cbDownLoadPos">进度回调函数</param>
        /// <param name="dwUserData">用户自定义数据</param>
        /// <returns>成功返回网络回放ID，失败返回0</returns>
        [DllImport("dhnetsdk.dll")]
        private static extern int CLIENT_PlayBackByRecordFile(int lLoginID, ref NET_RECORDFILE_INFO lpRecordFile, IntPtr hWnd, fDownLoadPosCallBack cbDownLoadPos, IntPtr dwUserData);

        /// <summary>
        /// 带数据回调的按时间回放扩展接口。窗口参数（hWnd）有效时回调数据的返回值将被忽略，窗口参数 (hWnd)为0时，需要注意回调函数的返回值。
        /// </summary>
        /// <param name="lLoginID">CLIENT_Login的返回值</param>
        /// <param name="nChannelID">图像通道号，必须指定</param>
        /// <param name="lpStartTime">开始时间</param>
        /// <param name="lpStopTime">结束时间</param>
        /// <param name="hWnd">回放容器句柄</param>
        /// <param name="cbDownLoadPos">进度回调函数</param>
        /// <param name="dwPosUser">进度回调用户数据</param>
        /// <param name="fDownLoadDataCallBack">数据回调函数</param>
        /// <param name="dwDataUser">数据回调用户数据</param>
        /// <returns>成功返回网络回放ID，失败返回0</returns>
        [DllImport("dhnetsdk.dll")]
        private static extern int CLIENT_PlayBackByTimeEx(int lLoginID,int nChannelID, ref NET_TIME lpStartTime,
                                                          ref NET_TIME lpStopTime, IntPtr hWnd,
                                                          fDownLoadPosCallBack cbDownLoadPos, IntPtr dwPosUser,
                                                          fDataCallBack fDownLoadDataCallBack, IntPtr dwDataUser);

        /// <summary>
        /// 带数据回调的按录像文件回放扩展接口,每通道同一时间只能播放一则录像,不能同时播放同一通道的多条记录。窗口参数（hWnd）有效时回调数据的返回值将被忽略，窗口参数 (hWnd)为0时，需要注意回调函数的返回值，具体见回调函数说明
        /// </summary>
        /// <param name="lLoginID">设备用户登录句柄</param>
        /// <param name="lpRecordFile">录像文件信息</param>
        /// <param name="hWnd">回放容器句柄</param>
        /// <param name="cbDownLoadPos">进度回调函数</param>
        /// <param name="dwPosUser">进度回调函数用户数据</param>
        /// <param name="fDownLoadDataCallBack">数据回调函数</param>
        /// <param name="dwDataUser">数据回调函数用户数据</param>
        /// <returns>成功返回网络回放ID，失败返回0</returns>
        [DllImport("dhnetsdk.dll")]
        private static extern int CLIENT_PlayBackByRecordFileEx(int lLoginID, ref NET_RECORDFILE_INFO lpRecordFile, IntPtr hWnd, fDownLoadPosCallBack cbDownLoadPos, IntPtr dwPosUser,fDataCallBack fDownLoadDataCallBack, IntPtr dwDataUser);

        /// <summary>
        /// 网络按时间回放
        /// </summary>
        /// <param name="lLoginID">设备用户登录句柄</param>
        /// <param name="nChannelID">通道ID</param>
        /// <param name="lpStartTime">录像开始时间</param>
        /// <param name="lpStopTime">录像结束时间</param>
        /// <param name="hWnd">录像回放容器句柄</param>
        /// <param name="cbDownLoadPos">进度回调函数</param>
        /// <param name="dwUserData">用户自定义数据</param>
        /// <returns>成功返回网络回放ID，失败返回0</returns>
        [DllImport("dhnetsdk.dll")]
        private static extern int CLIENT_PlayBackByTime(int lLoginID, int nChannelID, ref NET_TIME lpStartTime,
                                                        ref NET_TIME lpStopTime, IntPtr hWnd,
                                                        fDownLoadPosCallBack cbDownLoadPos,
                                                        IntPtr dwUserData);
        /// <summary>
        /// 网络回放停止
        /// </summary>
        /// <param name="lPlayHandle">回放句柄</param>
        /// <returns>true:成功;false:失败</returns>
        [DllImport("dhnetsdk.dll")]
        private static extern bool CLIENT_StopPlayBack(int lPlayHandle);

        /// <summary>
        /// 网络回放暂停与恢复播放
        /// </summary>
        /// <param name="lPlayHandle">播放句柄</param>
        /// <param name="bPause">1:暂停;0:恢复</param>
        /// <returns>true:成功;false:失败</returns>
        [DllImport("dhnetsdk.dll")]
        private static extern bool CLIENT_PausePlayBack(int lPlayHandle, int bPause);

        /// <summary>
        /// 改变位置播放[即拖动播放，只有打开图像的函数参数hWnd有效时该函数获取的参数才有效，否则无意义]
        /// </summary>
        /// <param name="lPlayHandle">播放句柄</param>
        /// <param name="offsettime">相对文件开始处偏移时间，单位为秒 .当其值为0xffffffff时,该参数无效.</param>
        /// <param name="offsetbyte">相对文件开始处偏移字节, 当其值为0xffffffff时, 该参数无效；当offsettime有效的时候,此参数无意义</param>
        /// <returns>true:成功;false:失败</returns>
        [DllImport("dhnetsdk.dll")]
        private static extern bool CLIENT_SeekPlayBack(int lPlayHandle, uint offsettime, uint offsetbyte);

        /// <summary>
        /// 单步播放[调用一次播放一帧图像，只有打开图像的函数参数hWnd有效时该函数获取的参数才有效，否则无意义]
        /// </summary>
        /// <param name="lPlayHandle">回放句柄</param>
        /// <param name="bStop">是否停止单步播放, 在结束单步时调用</param>
        /// <returns>true:成功;false:失败</returns>
        [DllImport("dhnetsdk.dll")]
        private static extern bool CLIENT_StepPlayBack(int lPlayHandle, bool bStop);

        /// <summary>
        /// 快放[将当前帧率提高一倍,但是不能无限制的快放,目前最大200,大于时返回FALSE, 有音频的话不可以快放，慢放没有问题，只有打开图像的函数参数hWnd有效时该函数获取的参数才有效，否则无意义]
        /// </summary>
        /// <param name="lPlayHandle">回放句柄</param>
        /// <returns>true:成功;false:失败</returns>
        [DllImport("dhnetsdk.dll")]
        private static extern bool CLIENT_FastPlayBack(int lPlayHandle);

        /// <summary>
        /// 慢放[将当前帧率降低一倍,最慢为每秒一帧,小于1则返回FALSE，只有打开图像的函数参数hWnd有效时该函数获取的参数才有效，否则无意义]
        /// </summary>
        /// <param name="lPlayHandle">回放句柄</param>
        /// <returns>true:成功;false:失败</returns>
        [DllImport("dhnetsdk.dll")]
        private static extern bool CLIENT_SlowPlayBack(int lPlayHandle);

        /// <summary>
        /// 得到SDK的版本号
        /// </summary>
        /// <returns>版本号</returns>
        [DllImport("dhnetsdk.dll")]
        private static extern UInt32 CLIENT_GetSDKVersion();

        /// <summary>
        /// 用户自定义画图, 在打开图像之前调用此函数,否则无效,必须在所有窗口未显示之前调用, 可以用来对画面进行字符叠加
        /// </summary>
        /// <param name="cbDraw">画图回调，当设置为0时表示禁止回调</param>
        /// <param name="dwUser">用户数据</param>
        [DllImport("dhnetsdk.dll")]
        private static extern void CLIENT_RigisterDrawFun(fDrawCallBack cbDraw, IntPtr dwUser);

        /// <summary>
        ///  获取设备配置
        /// </summary>
        /// <param name="lLoginID">设备用户登录ID:CLIENT_Login的返回值</param>
        /// <param name="dwCommand">配置类型</param>
        /// <param name="lChannel">通道号，如果不是通道参数，lChannel不用,置为-1即可</param>
        /// <param name="lpOutBuffer">存放输出参数的缓冲区, 根据不同的类型, 输出不同的配置结构, 具体见数据结构定义</param>
        /// <param name="dwOutBufferSize">输入缓冲区的大小, (单位字节).</param>
        /// <param name="lpBytesReturned">实际返回的缓冲区大小,对应配置结构的大小, (单位字节)</param>
        /// <param name="waittime">等待时间</param>
        /// <returns>true:成功;false:失败</returns>
        [DllImport("dhnetsdk.dll")]
        private static extern bool CLIENT_GetDevConfig(int lLoginID, UInt32 dwCommand, int lChannel, IntPtr lpOutBuffer, UInt32 dwOutBufferSize, ref UInt32 lpBytesReturned, int waittime);

        /// <summary>
        /// 设置设备配置
        /// </summary>
        /// <param name="lLoginID">设备用户登录ID:CLIENT_Login的返回值</param>
        /// <param name="dwCommand">配置类型</param>
        /// <param name="lChannel">通道号，如果设置全部通道数据为0xFFFFFFFF，如果命令不需要通道号，该参数无效</param>
        /// <param name="lpInBuffer">存放输入参数的缓冲区, 根据不同的类型, 输入不同的配置结构, 具体见数据结构定义</param>
        /// <param name="dwInBufferSize">输入缓冲区的大小, (单位字节)</param>
        /// <returns>true:成功;false:失败</returns>
        [DllImport("dhnetsdk.dll")]
        private static extern bool CLIENT_SetDevConfig(int lLoginID, UInt32 dwCommand, int lChannel, IntPtr lpInBuffer, UInt32 dwInBufferSize, int waittime);

        /// <summary>
        /// 查询设备的通道名称
        /// </summary>
        /// <param name="lLoginID">设备用户登录ID:CLIENT_Login的返回值</param>
        /// <param name="pChannelName">通道名缓冲区（一般每个通道名是32字节长，这里需分配16*32字节长度的缓冲）</param>
        /// <param name="maxlen">缓冲区长度, (单位字节)</param>
        /// <param name="nChannelCount">总共通道数</param>
        /// <param name="waittime">等待时间</param>
        /// <returns>true:成功;false:失败</returns>
        [DllImport("dhnetsdk.dll")]
        private static extern bool CLIENT_QueryChannelName(int lLoginID, ref char pChannelName, int maxlen, ref int nChannelCount, int waittime);

        /// <summary>
        /// 查询串口协议与解码器协议, 属于配置信息的一部分,查询前端设备目前支持的可选控制协议
        /// </summary>
        /// <param name="lLoginID">设备用户登录ID:CLIENT_Login的返回值</param>
        /// <param name="nProtocolType">0是串口协议1是解码器协议(云台控制协议)</param>
        /// <param name="pProtocolBuffer">接收的协议缓冲区</param>
        /// <param name="maxlen">接收的协议缓冲区长度</param>
        /// <param name="nProtocollen">接收的总字节数,(单位字节).</param>
        /// <param name="waittime">等待时间</param>
        /// <returns>true:成功;false:失败</returns>
        [DllImport("dhnetsdk.dll")]
        private static extern bool CLIENT_QueryComProtocol(int lLoginID, int nProtocolType, ref char pProtocolBuffer, int maxlen, ref int nProtocollen, int waittime);

        /// <summary>
        /// 查询系统信息,不同的信息有不同的数据结构
        /// </summary>
        /// <param name="lLoginID">设备用户登录ID:CLIENT_Login的返回值</param>
        /// <param name="nSystemType">要查询的系统类型
        ///  0		一般信息
        ///  1		查询设备属性信息
        ///  2		查询硬盘信息
        ///  3		查询文件系统信息
        ///  4		查询视频属性信息
        ///  5		查询系统字符集属性信息
        ///  6		查询光存储设备信息
        ///  7		获取设备序列号
        /// </param>
        /// <param name="pSysInfoBuffer">接收的协议缓冲区</param>
        /// <param name="maxlen">接收的协议缓冲区长度,(单位字节)</param>
        /// <param name="nSysInfolen">接收的总字节数,(单位字节).</param>
        /// <param name="waittime">等待时间</param>
        /// <returns>true:成功;false:失败</returns>
        [DllImport("dhnetsdk.dll")]
        private static extern bool CLIENT_QuerySystemInfo(int lLoginID, int nSystemType, ref char pSysInfoBuffer, int maxlen, ref  int nSysInfolen, int waittime);

        /// <summary>
        /// 获取设备配置信息
        /// </summary>
        /// <param name="lLoginID">设备用户登录ID:CLIENT_Login的返回值</param>
        /// <param name="nConfigType">配置类型</param>
        /// <param name="pConfigbuf">配置的接收缓冲区, 根据不同的配置信息,构和数据长度也不一致</param>
        /// <param name="maxlen">配置的接收缓冲区最大长度,(单位字节)</param>
        /// <param name="nConfigbuflen">收到的配置包长度,(单位字节)</param>
        /// <param name="waittime">等待时间</param>
        /// <returns>true:成功;false:失败</returns>
        [DllImport("dhnetsdk.dll")]
        private static extern bool CLIENT_QueryConfig(int lLoginID, int nConfigType, ref  char pConfigbuf, int maxlen, ref int nConfigbuflen, int waittime);

        /// <summary>
        /// 设置配置
        /// </summary>
        /// <param name="lLoginID">设备用户登录ID:CLIENT_Login的返回值</param>
        /// <param name="nConfigType">配置类型</param>
        /// <param name="pConfigbuf">设置配置缓冲区</param>
        /// <param name="nConfigbuflen">设置配置包长度,(单位字节)</param>
        /// <param name="waittime">等待时间</param>
        /// <returns>true:成功;false:失败</returns>
        [DllImport("dhnetsdk.dll")]
        private static extern bool CLIENT_SetupConfig(int lLoginID, int nConfigType, ref char pConfigbuf, int nConfigbuflen, int waittime);

        /// <summary>
        /// 查询用户信息
        /// </summary>
        /// <param name="lLoginID">CLIENT_Login的返回值</param>
        /// <param name="info">用户信息缓存，应传进一个已分配好内存的USER_MANAGE_INFO结构指针; 返回各用户信息USER_INFO中的password字段是加过密的。</param>
        /// <param name="waittime">等待时间</param>
        /// <returns>true:成功;false:失败</returns>
        [DllImport("dhnetsdk.dll")]
        private static extern bool CLIENT_QueryUserInfo(int lLoginID, IntPtr info, int waittime);

        /// <summary>
        /// 设置用户信息
        /// </summary>
        /// <param name="lLoginID">CLIENT_Login的返回值</param>
        /// <param name="nOperateType">
        /// 设置类型
        /// 0		增加用户组－opParam为欲设置的USER_GROUP_INF结构指针
        /// 1		删除用户组－opParam为欲设置的USER_GROUP_INF结构指针其name成员是实际起作用的变量
        /// 2		修改用户组－opParam为新的USER_GROUP_INF结构指针，subParam为原始的USER_GROUP_INF结构指针，
        /// 3		增加用户 －opParam为欲增加的USER_INF结构指针其password成员传普通字符串即可，不用加密
        /// 4		删除用户 －opParam为欲删除的USER_INF结构指针其name成员是实际起作用的变量
        /// 5		修改用户 －opParam为新的USER_INF结构指针subParam为原始的USER_INF结构指针,其password成员必须是查询时返回的密码字段（加密过的）
        /// 6		修改用户密码－opParam为新的USER_INF结构指针subParam为原始的USER_INF结构指针,其password成员必须是普通字符串，不用加密
        /// </param>
        /// <param name="opParam">设置用户信息的输入缓冲，具体见nOperateType说明。</param>
        /// <param name="subParam">设置用户信息的输入缓冲，具体见nOperateType说明。</param>
        /// <param name="waittime">等待时间</param>
        /// <returns>true:成功;false:失败</returns>
        [DllImport("dhnetsdk.dll")]
        private static extern bool CLIENT_OperateUserInfo(int lLoginID, int nOperateType, IntPtr opParam, IntPtr subParam, int waittime);

        /// <summary>
        /// 保存图片,对显示图像进行瞬间抓图，只有打开图像的函数参数hWnd有效时该函数获取的参数才有效，否则无意义
        /// </summary>
        /// <param name="hPlayHandle">CLIENT_RealPlay的返回值</param>
        /// <param name="pchPicFileName">位图文件名，当前只支持BMP位图</param>
        /// <returns>true:成功;false:失败</returns>
        [DllImport("dhnetsdk.dll")]
        private static extern bool CLIENT_CapturePicture(int hPlayHandle, string pchPicFileName);

        
        /// <summary>
        /// 按文件下载录像文件, 通过查询到的文件信息下载
        /// </summary>
        /// <param name="lLoginID">CLIENT_Login的返回值</param>
        /// <param name="lpRecordFile">录像文件信息</param>
        /// <param name="sSavedFileName">要保存的录像文件名，全路径</param>
        /// <param name="cbDownLoadPos">下载进度回调函数</param>
        /// <param name="dwUserData">下载进度回调用户自定义数据</param>
        /// <returns>成功返回下载ID，失败返回0</returns>
        [DllImport("dhnetsdk.dll")]
        private static extern int CLIENT_DownloadByRecordFile(int lLoginID,ref NET_RECORDFILE_INFO lpRecordFile,string  sSavedFileName, fDownLoadPosCallBack cbDownLoadPos, IntPtr dwUserData);

        /// <summary>
        /// 按时间下载，直接输入指定通道起始时间和结束时间下载放录像
        /// </summary>
        /// <param name="lLoginID">CLIENT_Login的返回值</param>
        /// <param name="nChannelId">图像通道号，从0开始</param>
        /// <param name="nRecordFileType">保留</param>
        /// <param name="tmStart">开始时间</param>
        /// <param name="tmEnd">结束时间</param>
        /// <param name="sSavedFileName">要保存的录像文件名，全路径</param>
        /// <param name="cbTimeDownLoadPos">下载进度回调函数</param>
        /// <param name="dwUserData">下载进度回调用户自定义数据</param>
        /// <returns>成功返回下载ID，失败返回0</returns>
        [DllImport("dhnetsdk.dll")]
        private static extern int CLIENT_DownloadByTime(int lLoginID, int nChannelId, int nRecordFileType,ref  NET_TIME tmStart,ref  NET_TIME tmEnd, string sSavedFileName, fDownLoadPosCallBack cbTimeDownLoadPos, IntPtr dwUserData);

        /// <summary>
        /// 停止下载录像文件
        /// </summary>
        /// <param name="lFileHandle">CLIENT_DownloadByRecordFile的返回值</param>
        /// <returns>true:成功;false:失败</returns>
        [DllImport("dhnetsdk.dll")]
        private static extern bool CLIENT_StopDownload(int lFileHandle);

        /// <summary>
        /// 获得下载录像的当前位置,可以用于不需要实时显示下载进度的接口,与下载回调函数的功能类似
        /// </summary>
        /// <param name="lFileHandle">CLIENT_DownloadByRecordFile的返回值</param>
        /// <param name="nTotalSize">下载的总长度,单位KB</param>
        /// <param name="nDownLoadSize">已下载的长度,单位KB</param>
        /// <returns>true:成功;false:失败</returns>
        [DllImport("dhnetsdk.dll")]
        private static extern bool CLIENT_GetDownloadPos(int lFileHandle, out int nTotalSize, out int nDownLoadSize);

        /// <summary>
        /// 启动实时监视或多画面预览
        /// </summary>
        /// <param name="lLoginID">CLIENT_Login的返回值</param>
        /// <param name="nChannelID">实时监视通道号，如果rType为RType_Multiplay该参数保留。当rType为RType_Multiplay_1~RType_Multiplay_16时，nChannelID决定了预览的画面，如当RType_Multiplay_4时，通道为4或5或6或7表示预览第五到第7通道的四画面预览。</param>
        /// <param name="hWnd">窗口句柄，值为0对数据不解码、不显示图像。</param>
        /// <param name="rType">数据类型(参考REALPLAY_TYPE)</param>
        /// <returns>失败返回0，成功返回实时监视ID(实时监视句柄)，将作为相关函数的参数</returns>
        [DllImport("dhnetsdk.dll")]
        private static extern int CLIENT_RealPlayEx(int lLoginID, int nChannelID, IntPtr hWnd, REALPLAY_TYPE rType);

        /// <summary>
        /// 开始保存实时监视数据,对前端设备监视的图像进行数据保存,形成录像文件,此数据是设备端传送过来的原始视频数据
        /// </summary>
        /// <param name="lRealHandle">CLIENT_RealPlay的返回值</param>
        /// <param name="pchFileName">实时监视保存文件名</param>
        /// <returns>true:成功;false:失败</returns>
        [DllImport("dhnetsdk.dll")]
        private static extern bool CLIENT_SaveRealData(int lRealHandle, string pchFileName);

        /// <summary>
        /// 停止保存实时监视数据,关闭保存的文件
        /// </summary>
        /// <param name="lRealHandle">CLIENT_RealPlay的返回值</param>
        /// <returns>true:成功;false:失败</returns>
        [DllImport("dhnetsdk.dll")]
        private static extern bool CLIENT_StopSaveRealData(int lRealHandle);
        
        /// <summary>
        /// 设置解码库视频参数，只有打开图像的函数参数hWnd有效时该函数获取的参数才有效，否则无意义
        /// </summary>
        /// <param name="lPlayHandle">CLIENT_RealPlay的返回值</param>
        /// <param name="nBrightness">亮度(0-255)</param>
        /// <param name="nContrast">对比度(0-255)</param>
        /// <param name="nHue">色度(0-255)</param>
        /// <param name="nSaturation">饱和度(0-255)</param>
        /// <returns>true:成功;false:失败</returns>
        [DllImport("dhnetsdk.dll")]
        private static extern bool CLIENT_ClientSetVideoEffect(int lPlayHandle, byte nBrightness, byte nContrast, byte nHue, byte nSaturation);

        /// <summary>
        /// 获取解码库视频参数，只有打开图像的函数参数hWnd有效时该函数获取的参数才有效，否则无意义。
        /// </summary>
        /// <param name="lPlayHandle">CLIENT_RealPlay的返回值</param>
        /// <param name="nBrightness">返回亮度指针(0-255)</param>
        /// <param name="nContrast">返回对比度指针(0-255)</param>
        /// <param name="nHue">返回色度指针(0-255)</param>
        /// <param name="nSaturation">返回饱和度指针(0-255)</param>
        /// <returns>true:成功;false:失败</returns>
        [DllImport("dhnetsdk.dll")]
        private static extern bool CLIENT_ClientGetVideoEffect(int lPlayHandle, ref byte nBrightness, ref byte nContrast, ref byte nHue, ref byte nSaturation);

        #endregion

    }
    
    #endregion

    #region << 委托 >>

    /// <summary>
    /// 断线回调函数，回调出当前网络已经断开的设备, 对调用SDK的ClIENT_LogOut()函数主动断开的设备不回调
    /// </summary>
    /// <param name="lLoginID">设备用户登录句柄</param>
    /// <param name="pchDVRIP">DVR设备IP</param>
    /// <param name="nDVRPort">DVR设备连接端口</param>
    /// <param name="dwUser">用户数据</param>
    public delegate void fDisConnect(
        int lLoginID, StringBuilder pchDVRIP, int nDVRPort, IntPtr dwUser);

    /// <summary>
    /// 设备消息回调函数
    /// </summary>
    /// <param name="lCommand">回调类型</param>
    /// <param name="lLoginID">设备用户登录ID:<seealso cref="CLIENT_Login"/>返回值</param>
    /// <param name="pBuf">接收报警数据的缓存，根据调用的侦听接口和lCommand值不同，填充的数据不同</param>
    /// <param name="dwBufLen">pBuf的长度[单位字节]</param>
    /// <param name="pchDVRIP">设备IP</param>
    /// <param name="nDVRPort">设备端口</param>
    /// <param name="dwUser">回调的用户数据</param>
    /// <returns>true:成功;false:失败</returns>
    public delegate bool fMessCallBack(
        int lCommand, int lLoginID, IntPtr pBuf, UInt32 dwBufLen, IntPtr pchDVRIP, int nDVRPort, UInt32 dwUser);

    /// <summary>
    /// 监听回调函数
    /// </summary>
    /// <param name="lHandle">回调消息的服务器句柄</param>
    /// <param name="pIp">上传消息的设备Ip</param>
    /// <param name="wPort">上传消息的设备Port</param>
    /// <param name="lCommand">回调类型</param>
    /// <param name="pParam">接收报警数据的缓存，根据调用的侦听接口和lCommand值不同，填充的数据不同</param>
    /// <param name="dwParamLen">pParam的长度[单位字节]</param>
    /// <param name="dwUserData">用户回调的自定义数据</param>
    /// <returns></returns>
    public delegate int fServiceCallBack(
        int lHandle, string pIp, ushort wPort, int lCommand, string pParam, ushort dwParamLen, IntPtr dwUserData);

    /// <summary>
    /// 进度回调函数
    /// </summary>
    /// <param name="lPlayHandle">播放句柄:<seealso cref="CLIENT_PlayBackByRecordFile"/>返回值</param>
    /// <param name="dwTotalSize">指本次播放总大小，单位为KB</param>
    /// <param name="dwDownLoadSize">指已经播放的大小，单位为KB，当其值为-1时表示本次回防结束</param>
    /// <param name="dwUser">用户数据</param>
    public delegate void fDownLoadPosCallBack(int lPlayHandle, UInt32 dwTotalSize, UInt32 dwDownLoadSize, IntPtr dwUser);

    /// <summary>
    /// 画图回调
    /// </summary>
    /// <param name="lLoginID">设备用户登录句柄:<seealso cref="CLIENT_Login"/>的返回值，标识设备</param>
    /// <param name="lPlayHandle">实时监视播放句柄:<seealso cref="CLIENT_RealPlay"/>的返回值，标识通道。</param>
    /// <param name="hDC">是对整个显示画面区域的画板指针,根据用户的需要可以对任何位置叠加显示</param>
    /// <param name="dwUser">用户数据，就是上面输入的用户数据</param>
    public delegate void fDrawCallBack(int lLoginID, int lPlayHandle, IntPtr hDC, IntPtr dwUser);

    /// <summary>
    /// 实时监视数据回调
    /// </summary>
    /// <param name="lRealHandle">CLIENT_RealPlay的返回值</param>
    /// <param name="dwDataType">
    /// 标识回调出来的数据类型, 一旦设置回调,是以下4种数据都会同时回调出来,用户可以根据需要有选择的处理部分数据; 
    /// 0:原始数据(与SaveRealData保存的数据一致);
    /// 1:标准视频数据;
    /// 2:yuv数据;
    /// 3:pcm音频数据;
    /// </param>
    /// <param name="pBuffer">回调数据,根据数据类型的不同每次回调不同的长度的数据,除类型0, 其他数据类型都是按帧,每次回调一帧数据</param>
    /// <param name="dwBufSize">回调数据的长度, (单位字节)</param>
    /// <param name="dwUser">用户数据</param>
    public delegate void fRealDataCallBack(int lRealHandle, UInt32 dwDataType,IntPtr pBuffer, UInt32 dwBufSize, IntPtr dwUser);

    /// <summary>
    /// 实时监视数据回调(实时监视数据回调)
    /// </summary>
    /// <param name="lRealHandle">实时监视ID</param>
    /// <param name="dwDataType">
    /// 标识回调出来的数据类型, 只有dwFlag设置标识的数据才会回调出来：
    /// 0:原始数据(与SaveRealData保存的数据一致);
    /// 1:标准视频数据;
    /// 2:yuv数据
    /// 3:pcm音频数据
    /// </param>
    /// <param name="pBuffer">回调数据,根据数据类型的不同每次回调不同的长度的数据,除类型0, 其他数据类型都是按帧,每次回调一帧数据</param>
    /// <param name="dwBufSize">回调数据的长度, 根据不同的类型, 长度也不同, (单位字节)</param>
    /// <param name="param">回调数据参数结构体, 根据不同的类型, 参数结构也不一致, 当类型为0 (原始数据)和 2 (YUV数据) 时为0</param>
    /// <param name="dwUser">用户数据，就是上面输入的用户数据</param>
    public delegate void fRealDataCallBackEx(int lRealHandle, UInt32 dwDataType, IntPtr pBuffer, UInt32 dwBufSize, int param, IntPtr dwUser);

    /// <summary>实时监视数据回调(实时监视数据回调)</summary>
    /// <param name="lRealHandle">实时监视ID</param>
    /// <param name="dwDataType">
    /// 标识回调出来的数据类型, 只有dwFlag设置标识的数据才会回调出来：
    /// 0:原始数据(与SaveRealData保存的数据一致);
    /// 1:标准视频数据;
    /// 2:yuv数据
    /// 3:pcm音频数据
    /// </param>
    /// <param name="pBuffer">回调数据,根据数据类型的不同每次回调不同的长度的数据,除类型0, 其他数据类型都是按帧,每次回调一帧数据</param>
    /// <param name="dwBufSize">回调数据的长度, 根据不同的类型, 长度也不同, (单位字节)</param>
    /// <param name="param">回调数据参数结构体, 根据不同的类型, 参数结构也不一致, 当类型为0 (原始数据)和 2 (YUV数据) 时为0</param>
    /// <param name="dwUser">用户数据，就是上面输入的用户数据</param>
    /// <returns>
    /// 本回调函数的返回值会影响SDK的内部操作：
    /// 1:代表回调成功，没什么特殊情况都应该返回这个值
    /// 0:代表应用程序缓冲满，网络SDK将停顿一小会（ms级），然后回调同一数据，一般用于数据速度控制。
    /// -1:代表系统出错，将直接结束回放线程，慎用
    /// </returns>
    public delegate int fDataCallBack(int lRealHandle,UInt32 dwDataType, IntPtr pBuffer, UInt32 dwBufSize, UInt32 dwUser);

    #endregion

    #region << 结构定义 >>

    /// <summary>
    /// 网络设备信息
    /// </summary>
    [StructLayout(LayoutKind.Sequential)]
    public struct NET_DEVICEINFO
    {
        /// <summary>
        /// 序列号[长度48]
        /// </summary>
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 48)]
        public byte[] sSerialNumber;

        /// <summary>
        /// DVR报警输入个数
        /// </summary>
        public byte byAlarmInPortNum;

        /// <summary>
        /// DVR报警输出个数
        /// </summary>
        public byte byAlarmOutPortNum;

        /// <summary>
        /// DVR硬盘个数
        /// </summary>
        public byte byDiskNum;

        /// <summary>
        /// DVR类型
        /// </summary>
        public byte byDVRType;

        /// <summary>
        /// DVR通道个数
        /// </summary>
        public byte byChanNum;

    }

    /// <summary>
    /// 网络时间
    /// </summary>
    [StructLayout(LayoutKind.Sequential)]
    public struct NET_TIME
    {
        /// <summary>
        /// 年
        /// </summary>
        public int dwYear;
        /// <summary>
        /// 月
        /// </summary>
        public int dwMonth;
        /// <summary>
        /// 日
        /// </summary>
        public int dwDay;
        /// <summary>
        /// 时
        /// </summary>
        public int dwHour;
        /// <summary>
        /// 分
        /// </summary>
        public int dwMinute;
        /// <summary>
        /// 秒
        /// </summary>
        public int dwSecond;

        /// <summary>
        /// 将日期按格式转换
        /// </summary>
        /// <param name="FormatStyle">日期格式字符串:yyyy为年的值格式[固定四位]；mm为月的值格式[固定两位]；dd为日的值格式[固定两位]；d为日的值格式[不限定位数]；m为月的格式[不限定位数]；yy为年的格式[固定两位]；y为年的格式[不限定位数]；hh为时的值格式[固定两位]；h为时的值格式[不限定位数]；MM为分的值格式[固定两位]；M为分的值格式[不限定位数]；ss为秒的值格式[固定两位]；s为秒的值格式[不限定位数]；</param>
        /// <returns></returns>
        public string ToString(string FormatStyle)
        {
            string returnValue = FormatStyle;
            //年的格式处理
            string strY = "";
            if (returnValue.IndexOf("yyyy") != -1)
            {
                strY = "0000".Remove(4 - dwYear.ToString().Length) + dwYear.ToString();
                returnValue = returnValue.Replace("yyyy", strY);
            }
            else if (returnValue.IndexOf("yy") != -1)
            {
                if (dwYear.ToString().Length > 2)
                {
                    strY = dwYear.ToString().Substring(dwYear.ToString().Length - 2);
                    returnValue = returnValue.Replace("yy", strY);
                }
                else
                {
                    strY = "00".Remove(2 - dwYear.ToString().Length) + dwYear.ToString();
                    returnValue = returnValue.Replace("yy", strY);
                }
            }
            else if (returnValue.IndexOf("y") != -1)
            {
                strY = dwYear.ToString();
                returnValue = returnValue.Replace("y", strY);
            }
            //月的格式处理
            string strM = "";
            if (returnValue.IndexOf("mm") != -1)
            {
                strM = "00".Remove(2 - dwMonth.ToString().Length) + dwMonth.ToString();
                returnValue = returnValue.Replace("mm", strM);
            }
            else if (returnValue.IndexOf("m") != -1)
            {
                strM = dwMonth.ToString();
                returnValue = returnValue.Replace("m", strM);
            }
            //日的格式处理
            string strD = "";
            if (returnValue.IndexOf("dd") != -1)
            {
                strD = "00".Remove(2 - dwDay.ToString().Length) + dwDay.ToString();
                returnValue = returnValue.Replace("dd", strD);
            }
            else if (returnValue.IndexOf("d") != -1)
            {
                strD = dwDay.ToString();
                returnValue = returnValue.Replace("d", strD);
            }
            //时的格式处理
            string strH = "";
            if (returnValue.IndexOf("hh") != -1)
            {
                strH = "00".Remove(2 - dwHour.ToString().Length) + dwHour.ToString();
                returnValue = returnValue.Replace("hh", strH);
            }
            else if (returnValue.IndexOf("h") != -1)
            {
                strH = dwHour.ToString();
                returnValue = returnValue.Replace("h", strH);
            }
            //分的格式处理
            string strMM = "";
            if (returnValue.IndexOf("MM") != -1)
            {
                strMM = "00".Remove(2 - dwMinute.ToString().Length) + dwMinute.ToString();
                returnValue = returnValue.Replace("MM", strMM);
            }
            else if (returnValue.IndexOf("M") != -1)
            {
                strMM = dwMinute.ToString();
                returnValue = returnValue.Replace("M", strMM);
            }
            //秒的格式处理
            string strS = "";
            if (returnValue.IndexOf("ss") != -1)
            {
                strS = "00".Remove(2 - dwSecond.ToString().Length) + dwSecond.ToString();
                returnValue = returnValue.Replace("ss", strS);
            }
            else if (returnValue.IndexOf("s") != -1)
            {
                strS = dwSecond.ToString();
                returnValue = returnValue.Replace("s", strS);
            }
            return returnValue;
        }

        /// <summary>
        /// 转为标准备的系统时间格式
        /// </summary>
        /// <returns></returns>
        public DateTime ToDateTime()
        {
            try
            {
                return new DateTime(dwYear, dwMonth, dwDay, dwHour, dwMinute, dwSecond);
            }
            catch
            {
                return DateTime.Now;
            }
        }

    }

    /// <summary>
    /// 录像文件信息
    /// </summary>
    public struct NET_RECORDFILE_INFO
    {
        /// <summary>
        /// 通道号
        /// </summary>
        public uint ch;
        /// <summary>
        /// 文件名
        /// </summary>
        //[MarshalAs(UnmanagedType.ByValArray, ArraySubType = UnmanagedType.ByValTStr, SizeConst = 128)]
        //public char[] filename;
        [MarshalAs(UnmanagedType.ByValArray,  SizeConst = 128)]
        public byte[] filename;
        //public string filename;
        /// <summary>
        /// 文件长度
        /// </summary>
        public uint size;
        /// <summary>
        /// 开始时间
        /// </summary>
        public NET_TIME starttime;
        /// <summary>
        /// 结束时间
        /// </summary>
        public NET_TIME endtime;
        /// <summary>
        /// 磁盘号
        /// </summary>
        public uint driveno;
        /// <summary>
        /// 起始簇号
        /// </summary>
        public uint startcluster;
        /// <summary>
        /// 录像文件类型  0：普通录像；1：报警录
        /// </summary>
        public int nRecordFileType;
    }

    /// <summary>
    /// 错误内容
    /// </summary>
    public struct OPERATION_INFO
    {
        /// <summary>
        /// 错误代码
        /// </summary>
        public string errCode;
        /// <summary>
        /// 错误描述
        /// </summary>
        public string errMessage;
        /// <summary>
        /// 按自定义格式返回错误内容字符串
        /// </summary>
        /// <param name="FormatStyle">错误内容字符串格式：errcode代替错误代码;errmsg代替错误描述</param>
        /// <returns></returns>
        public string ToString(string FormatStyle)
        {
            string returnValue = FormatStyle;
            if (returnValue.Length == 0)
            {
                returnValue = "errcode:errmsg!";
            }
            returnValue = returnValue.ToUpper();
            returnValue = returnValue.Replace("ERRCODE", errCode).Replace("ERRMSG", errMessage);
            return returnValue;

        }
    }

    /// <summary>
    /// 设备状态信息
    /// </summary>
    public struct NET_CLIENT_STATE
    {
        /// <summary>
        /// 通道数
        /// </summary>
        public int channelcount;

        /// <summary>
        /// 报警输入数
        /// </summary>
        public int alarminputcount;

        /// <summary>
        /// 外部报警
        /// </summary>
        //[MarshalAs(UnmanagedType.ByValArray, ArraySubType = UnmanagedType.ByValTStr, SizeConst = 16)]
        //public char[] alarm;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 16)]
        public byte[] alarm;

        /// <summary>
        /// 动态检测
        /// </summary>
        //[MarshalAs(UnmanagedType.ByValArray, ArraySubType = UnmanagedType.ByValTStr, SizeConst = 16)]
        //public char[] motiondection;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 16)]
        public byte[] motiondection;

        /// <summary>
        /// 视频丢失
        /// </summary>
        //[MarshalAs(UnmanagedType.ByValArray, ArraySubType = UnmanagedType.ByValTStr, SizeConst = 16)]
        //public char[] videolost;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 16)]
        public byte[] videolost;
    }

    /// <summary>
    /// 设备通道状态信息
    /// </summary>
    public struct NET_DEV_CHANNELSTATE
    {
        /// <summary>
        /// 通道是否在录像,0-不录像,1-录像
        /// </summary>
        public byte byRecordStatic;
        /// <summary>
        /// 连接的信号状态,0-正常,1-信号丢失
        /// </summary>
        public byte bySignalStatic;
        /// <summary>
        /// 通道硬件状态,0-正常,1-异常,例如DSP死掉
        /// </summary>
        public byte byHardwareStatic;
        /// <summary>
        /// 暂时无效
        /// </summary>
        char reserve;
        /// <summary>
        /// 实际码率,暂时无效
        /// </summary>
        UInt32 dwBitRate;
        /// <summary>
        /// 客户端连接的个数, 暂时无效
        /// </summary>
        UInt32 dwLinkNum;
        /// <summary>
        /// 客户端的IP地址,暂时无效
        /// </summary>
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 6)]
        UInt32[] dwClientIP;
    }

    /// <summary>
    /// 设备硬盘状态信息
    /// </summary>
    public struct NET_DEV_DISKSTATE
    {
        /// <summary>
        /// 硬盘的容量
        /// </summary>
        public UInt32 dwVolume;
        /// <summary>
        /// 硬盘的剩余空间
        /// </summary>
        public UInt32 dwFreeSpace;
        /// <summary>
        /// 硬盘的状态,休眠,活动,不正常等
        /// </summary>
        public UInt32 dwStatus;
    }

    /// <summary>
    /// 设备工作状态信息
    /// </summary>
    public struct NET_DEV_WORKSTATE
    {
        /// <summary>
        /// 设备状态0x00 正常,0x01 CPU占用过高, 0x02 硬件错误
        /// </summary>
        public UInt32 dwDeviceStatic;
        /// <summary>
        /// 设备暂时不支持
        /// </summary>
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 32)]
        public NET_DEV_DISKSTATE[] stHardDiskStatic;
        /// <summary>
        /// 通道的状态
        /// </summary>
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 16)]
        public NET_DEV_CHANNELSTATE[] stChanStatic;
        /// <summary>
        /// 报警端口的状态0-无报警,1-有报警
        /// </summary>
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 128)]
        public byte[] byAlarmInStatic;
        /// <summary>
        /// 报警输出端口的状态0-无输出,1-有输出
        /// </summary>
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 64)]
        public byte[] byAlarmOutStatic;
        /// <summary>
        /// 本地显示状态0-正常,1-不正常
        /// </summary>
        public UInt32 dwLocalDisplay;
    }

    /// <summary>
    /// 串口协议信息(232和485
    /// </summary>
    public struct PROTOCOL_INFO
    {
        /// <summary>
        /// 协议名
        /// </summary>
        //[MarshalAs(UnmanagedType.ByValArray, ArraySubType = UnmanagedType.ByValTStr, SizeConst = 12)]
        //public char[] protocolname;
        [MarshalAs(UnmanagedType.ByValArray,  SizeConst = 12)]
        public byte[] protocolname;
        /// <summary>
        /// 波特率
        /// </summary>
        public uint baudbase;
        /// <summary>
        /// 数据位
        /// </summary>
        public char databits;
        /// <summary>
        /// 停止位
        /// </summary>
        public char stopbits;
        /// <summary>
        /// 校验位
        /// </summary>
        public char parity;
        /// <summary>
        /// 暂不支持
        /// </summary>
        public char reserve;
    }

    /// <summary>
    /// 报警IO控制(报警输出和报警输入使能)
    /// </summary>
    public struct ALARM_CONTROL
    {
        /// <summary>
        /// 端口序号
        /// </summary>
        public ushort index;
        /// <summary>
        /// 端口状态
        /// </summary>
        public ushort state;
    }

    
    public struct OPR_RIGHT
    {
        public UInt32 dwID;
        //[MarshalAs(UnmanagedType.ByValArray, ArraySubType = UnmanagedType.ByValTStr, SizeConst = 32)]
        //public char[] name;
        [MarshalAs(UnmanagedType.ByValArray,  SizeConst = 32)]
        public byte[] name;
        //[MarshalAs(UnmanagedType.ByValArray, ArraySubType = UnmanagedType.ByValTStr, SizeConst = 32)]
        //public char[] memo;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 32)]
        public byte[] memo;
    }

    /// <summary>
    /// 用户信息
    /// </summary>
    public struct USER_INFO
    {
        public UInt32 dwID;
        public UInt32 dwGroupID;
        //[MarshalAs(UnmanagedType.ByValArray, ArraySubType = UnmanagedType.ByValTStr, SizeConst = 8)]
        //public char[] name;
        [MarshalAs(UnmanagedType.ByValArray,  SizeConst = 8)]
        public byte[] name;
        //[MarshalAs(UnmanagedType.ByValArray, ArraySubType = UnmanagedType.ByValTStr, SizeConst = 8)]
        //public char[] passWord;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 8)]
        public byte[] passWord;
        public UInt32 dwRightNum;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 100)]
        public UInt32[] rights;
        //[MarshalAs(UnmanagedType.ByValArray, ArraySubType = UnmanagedType.ByValTStr, SizeConst = 32)]
        //public char[] memo;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 32)]
        public byte[] memo;
        /// <summary>
        /// 本用户是否允许复用:1.复用;0:不复用;
        /// </summary>
        public UInt32 dwReusable;
        public USER_INFO(string st)
        {
            dwID = new UInt32();
            dwGroupID = new UInt32();
            dwRightNum = new UInt32();
            dwReusable = new UInt32();
            name = new byte[8];
            passWord = new byte[8];
            rights = new UInt32[100];
            memo = new byte[32];
        }
    }

    /// <summary>
    /// 用户组结构
    /// </summary>
    public struct USER_GROUP_INFO
    {
        public UInt32 dwID;
        //[MarshalAs(UnmanagedType.ByValArray, ArraySubType = UnmanagedType.ByValTStr, SizeConst = 8)]
        //public char[] name;
        [MarshalAs(UnmanagedType.ByValArray,SizeConst = 8)]
        public byte[] name;
        public UInt32 dwRightNum;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 100)]
        public UInt32[] rights;
        //[MarshalAs(UnmanagedType.ByValArray, ArraySubType = UnmanagedType.ByValTStr, SizeConst = 32)]
        //public char[] memo;
        [MarshalAs(UnmanagedType.ByValArray,  SizeConst = 32)]
        public byte[] memo;
        public USER_GROUP_INFO(string strP)
        {
            dwID = new UInt32();
            name = new byte[8];
            dwRightNum = new UInt32();
            rights = new UInt32[100];
            memo = new byte[32];
        }
    }


    /// <summary>
    /// 用户信息配置结构
    /// </summary>
    public struct USER_MANAGE_INFO
    {
        public UInt32 dwRightNum;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 100)]
        public OPR_RIGHT[] rightList;
        public UInt32 dwGroupNum;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 20)]
        public USER_GROUP_INFO[] groupList;
        public UInt32 dwUserNum;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 200)]
        public USER_INFO[] userList;
        /// <summary>
        /// 特殊信息:1.支持用户复用;0.不支持用户复用;
        /// </summary>
        public UInt32 dwSpecial;
    }

    /// <summary>
    /// 日志结构
    /// </summary>
    public struct DH_LOG_ITEM
    {
        /// <summary>
        /// 日期
        /// </summary>
        public NET_TIME time;
        /// <summary>
        /// 类型
        /// </summary>
        public UInt16 type;
        /// <summary>
        /// 保留
        /// </summary>
        public byte reserved;
        /// <summary>
        /// 数据
        /// </summary>
        public byte data;
        /// <summary>
        /// 内容
        /// </summary>
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 8)]
        public byte[] context;
    }


    /// <summary>
    /// 查询硬盘信息的返回数据结构
    /// </summary>
    public struct DH_HARDDISK_STATE
    {
        /// <summary>
        /// 硬盘个数
        /// </summary>
        public UInt32 dwDiskNum;
        /// <summary>
        /// 各硬盘信息
        /// </summary>
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 32)]
        public NET_DEV_DISKSTATE[] stDisks;
    }

    /// <summary>
    /// 音频数据的格式结构
    /// </summary>
    public struct DH_AUDIO_FORMAT
    {
        /// <summary>
        /// 编码类型，0-PCM
        /// </summary>
        public byte byFormatTag;
        /// <summary>
        /// 声道数
        /// </summary>
        public UInt16 nChannels;
        /// <summary>
        /// 采样深度
        /// </summary>
        public UInt16 wBitsPerSample;
        /// <summary>
        /// 采样率
        /// </summary>
        public UInt32 nSamplesPerSec;
    }

    /// <summary>
    /// 版本信息
    /// 关于时间的数据组织是：yyyymmdd
    /// </summary>
    public struct DH_VERSION_INFO
    {
        /// <summary>
        /// 版本号:高16位表示主版本号，低16位表示次版本号
        /// </summary>
        public UInt32 dwSoftwareVersion;
        /// <summary>
        /// 创建时间
        /// </summary>
        public UInt32 dwSoftwareBuildDate;
        /// <summary>
        /// DSP版本号
        /// </summary>
        public UInt32 dwDspSoftwareVersion;
        /// <summary>
        /// DSP版本创建时间
        /// </summary>
        public UInt32 dwDspSoftwareBuildDate;
        /// <summary>
        /// 面板版本
        /// </summary>
        public UInt32 dwPanelVersion;
        /// <summary>
        /// 面板软件创建日期
        /// </summary>
        public UInt32 dwPanelSoftwareBuildDate;
        /// <summary>
        /// 硬件版本
        /// </summary>
        public UInt32 dwHardwareVersion;
        /// <summary>
        /// 硬件制作日期
        /// </summary>
        public UInt32 dwHardwareDate;
        /// <summary>
        /// Web版本
        /// </summary>
        public UInt32 dwWebVersion;
        /// <summary>
        /// Web创建日期
        /// </summary>
        public UInt32 dwWebBuildDate;
    }



    /// <summary>
    /// DSP能力描述
    /// </summary>
    public struct DH_DSP_ENCODECAP
    {
        /// <summary>
        /// 视频制式掩码，按位表示设备能够支持的视频制式
        /// </summary>
        public UInt32 dwVideoStandardMask;
        /// <summary>
        /// 分辨率掩码，按位表示设备能够支持的分辨率设置
        /// </summary>
        public UInt32 dwImageSizeMask;
        /// <summary>
        /// 编码模式掩码，按位表示设备能够支持的编码模式设置
        /// </summary>
        public UInt32 dwEncodeModeMask;
        /// <summary>
        /// 按位表示设备支持的多媒体功能 第一位表示支持主码流,第二位表示支持辅码流1,第三位表示支持辅码流2,第四位表示支持jpg抓图
        /// </summary>
        public UInt32 dwStreamCap;
        /// <summary>
        /// 表示主码流为各分辨率时，支持的辅码流分辨率掩码
        /// </summary>
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 8)]
        public UInt32[] dwImageSizeMask_Assi;
        /// <summary>
        /// DSP 支持的最高编码能力
        /// </summary>
        public UInt32 dwMaxEncodePower;
        /// <summary>
        /// 每块 DSP 支持最多输入视频通道数 
        /// </summary>
        public UInt16 wMaxSupportChannel;
        /// <summary>
        /// DSP 每通道的最大编码设置是否同步 0-不同步, 1-同步
        /// </summary>
        public UInt16 wChannelMaxSetSync;
    }

    /// <summary>
    /// 设备信息配置
    /// </summary>
    public struct DHDEV_SYSTEM_ATTR_CFG
    {
        /// <summary>
        /// 
        /// </summary>
        public UInt32 dwSize;
        /*下面是设备的只读部分*/
        /// <summary>
        /// 版本
        /// </summary>
        public DH_VERSION_INFO stVersion;
        /// <summary>
        /// DSP能力描述
        /// </summary>
        public DH_DSP_ENCODECAP stDspEncodeCap;
        /// <summary>
        /// 序列号
        /// </summary>
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 48)]
        public byte[] szDevSerialNo;
        /// <summary>
        /// 序列号转成字符串
        /// </summary>
        /// <returns></returns>
        public string DevSerialNo()
        {
            string result = "";
            foreach (byte bt in szDevSerialNo)
            {
                result += bt.ToString("D");
            }
            return result;
        }
        /// <summary>
        /// 设备类型，见枚举NET_DEVICE_TYPE
        /// </summary>
        public byte byDevType;
        /// <summary>
        /// 显示标准备的设备类型描述
        /// </summary>
        /// <returns></returns>
        public string DevType()
        {
            string result = "";
            switch(int.Parse(byDevType.ToString()))
            {
                case (int)NET_DEVICE_TYPE.NET_DVR_DDNS:
                    result = "DDNS服务器";
                    break;
                case (int)NET_DEVICE_TYPE.NET_DVR_MEPG4_SH2:
                    result = "MPEG4视豪录像机";
                    break;
                case (int)NET_DEVICE_TYPE.NET_DVR_MEPG4_ST2:
                    result = "MPEG4视通录像机";
                    break;
                case (int)NET_DEVICE_TYPE.NET_DVR_MPEG1_2:
                    result = "MPEG1二路录像机";
                    break;
                case (int)NET_DEVICE_TYPE.NET_DVR_MPEG1_8:
                    result = "MPEG4八路录像机";
                    break;
                case (int)NET_DEVICE_TYPE.NET_DVR_MPEG4_16:
                    result = "MPEG4十六录像机";
                    break;
                case (int)NET_DEVICE_TYPE.NET_DVR_MPEG4_8:
                    result = "MPEG4八路录像机";
                    break;
                case (int)NET_DEVICE_TYPE.NET_DVR_MPEG4_GBE:
                    result = "MPEG4视通二代增强型录像机";
                    break;
                case (int)NET_DEVICE_TYPE.NET_DVR_MPEG4_NVSII:
                    result = "MPEG4网络视频服务器II代";
                    break;
                case (int)NET_DEVICE_TYPE.NET_DVR_MPEG4_SX2:
                    result = "MPEG4视新十六路录像机";
                    break;
                case (int)NET_DEVICE_TYPE.NET_DVR_NONREALTIME:
                    result = "非实时";
                    break;
                case (int)NET_DEVICE_TYPE.NET_DVR_NONREALTIME_MACE:
                    result = "非实时MACE";
                    break;
                case (int)NET_DEVICE_TYPE.NET_DVR_STD_NEW:
                    result = "新标准配置协议";
                    break;
                case (int)NET_DEVICE_TYPE.NET_NVS_MPEG1:
                    result = "网络视频服务器";
                    break;
                case (int)NET_DEVICE_TYPE.NET_PRODUCT_NONE:
                    result = "无";
                    break;
            }
            return result;
        }
        /// <summary>
        /// 设备详细型号，字符串格式，可能为空
        /// </summary>
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 32)]
        public byte[] szDevType;
        /// <summary>
        /// 视频口数量
        /// </summary>
        public byte byVideoCaptureNum;
        /// <summary>
        /// 音频口数量
        /// </summary>
        public byte byAudioCaptureNum;
        /// <summary>
        /// NSP
        /// </summary>
        public byte byTalkInChanNum;
        /// <summary>
        /// NSP
        /// </summary>
        public byte byTalkOutChanNum;
        /// <summary>
        /// NSP
        /// </summary>
        public byte byDecodeChanNum;
        /// <summary>
        /// 报警输入口数
        /// </summary>
        public byte byAlarmInNum;
        /// <summary>
        /// 报警输出口数
        /// </summary>
        public byte byAlarmOutNum;
        /// <summary>
        /// 网络口数
        /// </summary>
        public byte byNetIONum;
        /// <summary>
        /// USB口数量
        /// </summary>
        public byte byUsbIONum;
        /// <summary>
        /// IDE数量
        /// </summary>
        public byte byIdeIONum;
        /// <summary>
        /// 串口数量
        /// </summary>
        public byte byComIONum;
        /// <summary>
        /// 并口数量
        /// </summary>
        public byte byLPTIONum;
        /// <summary>
        /// NSP
        /// </summary>
        public byte byVgaIONum;
        /// <summary>
        /// NSP
        /// </summary>
        public byte byIdeControlNum;
        /// <summary>
        /// NSP
        /// </summary>
        public byte byIdeControlType;
        /// <summary>
        /// NSP，扩展描述
        /// </summary>
        public byte byCapability;
        /// <summary>
        /// 视频矩阵输出口数
        /// </summary>
        public byte byMatrixOutNum;
        /*下面是设备的可写部分*/
        /// <summary>
        /// 硬盘满处理方式（覆盖、停止）
        /// </summary>
        public byte byOverWrite;
        /// <summary>
        /// 录像打包长度
        /// </summary>
        public byte byRecordLen;
        /// <summary>
        /// NSP
        /// </summary>
        public byte byStartChanNo;
        /// <summary>
        /// 设备编号，用于遥控
        /// </summary>
        public UInt16 wDevNo;
        /// <summary>
        /// 视频制式
        /// </summary>
        public byte byVideoStandard;
        /// <summary>
        /// 日期格式
        /// </summary>
        public byte byDateFormat;
        /// <summary>
        /// 日期分割符(0-"."， 1-"-"， 2-"/")
        /// </summary>
        public byte byDateSprtr;
        /// <summary>
        /// 时间格式 (0-24小时，1－12小时)
        /// </summary>
        public byte byTimeFmt;
        /// <summary>
        /// 保留字
        /// </summary>
        public byte byReserved;
    }

    /// <summary>
    /// 时间段结构  
    /// </summary>
    public struct DH_TSECT
    {
        public bool bEnable;
        public int iBeginHour;
        public int iBeginMin;
        public int iBeginSec;
        public int iEndHour;
        public int iEndMin;
        public int iEndSec;
    }

    /// <summary>
    /// 时间段结构[长度6]
    /// </summary>
    public struct DH_REC_TSECT
    {
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 6)]
        public DH_TSECT[] sTSECT;
    }
    
    /// <summary>
    /// 区域:各边距按整长8192的比例 
    /// </summary>
    public struct DH_RECT
    {
        public int left;
        public int top;
        public int right;
        public int bottom;
    }

    /// <summary>
    /// OSD属性结构 
    /// </summary>
    public struct DH_ENCODE_WIDGET
    {
        /// <summary>
        /// 物件的前景RGB，和透明度  
        /// </summary>
        public UInt32 rgbaFrontground;
        /// <summary>
        /// 物件的后景RGB，和透明度 
        /// </summary>
        public UInt32 rgbaBackground;
        /// <summary>
        /// 位置  
        /// </summary>
        public DH_RECT rcRect;
        /// <summary>
        /// 物件显示
        /// </summary>
        public byte bShow;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
        public byte[] byReserved;
    }

    /// <summary>
    /// 通道音视频属性 
    /// </summary>
    public struct DH_VIDEOENC_OPT
    {
        //视频参数
        /// <summary>
        /// 视频使能:1－打开，0－关闭 
        /// </summary>
        public byte byVideoEnable;
        /// <summary>
        /// 码流控制,参照常量定义
        /// </summary>
        public byte byBitRateControl;
        /// <summary>
        /// 帧率
        /// </summary>
        public byte byFramesPerSec;
        /// <summary>
        /// 编码模式,参照常量定义 
        /// </summary>
        public byte byEncodeMode;
        /// <summary>
        /// 分辨率参,参照常量定义 
        /// </summary>
        public byte byImageSize;
        /// <summary>
        /// 档次1-6 
        /// </summary>
        public byte byImageQlty;
        /// <summary>
        /// 限码流参数, 范围：50~4*1024 (k)
        /// </summary>
        public UInt16 wLimitStream;

        //音频参数
        /// <summary>
        /// 音频使能:1－打开，0－关闭
        /// </summary>
        public byte byAudioEnable;
        /// <summary>
        /// 编码类型，如PCM
        /// </summary>
        public byte wFormatTag;
        /// <summary>
        /// 声道数
        /// </summary>
        public UInt16 nChannels;
        /// <summary>
        /// 采样深度	
        /// </summary>
        public UInt16 wBitsPerSample;
        /// <summary>
        /// 采样率
        /// </summary>
        public UInt32 nSamplesPerSec;
        /// <summary>
        /// 保留字
        /// </summary>
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 2)]
        public byte[] reserved;
    }

    /// <summary>
    /// 画面颜色属性
    /// </summary>
    public struct DH_COLOR_CFG
    {
        public DH_TSECT stSect;
        /// <summary>
        /// 亮度	0-100
        /// </summary>
        public byte byBrightness;
        /// <summary>
        /// 对比度	0-100
        /// </summary>
        public byte byContrast;
        /// <summary>
        /// 饱和度	0-100
        /// </summary>
        public byte bySaturation;
        /// <summary>
        /// 色度	0-100
        /// </summary>
        public byte byHue;
        /// <summary>
        /// 增益使能	  
        /// </summary>
        public byte byGainEn;
        /// <summary>
        /// 增益	0-100
        /// </summary>
        public byte byGain;

        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 2)]
        public byte[] byReserved;
    }



    /// <summary>
    /// 图像通道属性结构体
    /// </summary>
    public struct DHDEV_CHANNEL_CFG
    {
        public UInt32 dwSize;
        /// <summary>
        /// 
        /// </summary>
        //[MarshalAs(UnmanagedType.ByValArray, ArraySubType = UnmanagedType.ByValTStr, SizeConst = 32)]
        //public char[] szChannelName;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 32)]
        public byte[] szChannelName;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
        public DH_VIDEOENC_OPT[] stMainVideoEncOpt;
        /// <summary>
        ///  通常指网传码流	
        /// </summary>
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
        public DH_VIDEOENC_OPT[] stAssiVideoEncOpt;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 2)]
        public DH_COLOR_CFG[] stColorCfg;
        public DH_ENCODE_WIDGET stTimeOSD;
        public DH_ENCODE_WIDGET stChannelOSD;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 1)]
        public DH_ENCODE_WIDGET[] stBlindCover;

        /// <summary>
        ///  区域遮盖开关,0x00不使能遮盖，0x01仅遮盖,设备本地预览，0x10仅遮盖录像（及网络预览），0x11都遮盖
        ///  </summary>
        public byte byBlindEnable;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
        public byte[] byReserved;
    }

    /// <summary>
    /// 预览图像参数
    /// </summary>
    public struct DHDEV_PREVIEW_CFG
    {
        public UInt32 dwSize;
        public DH_VIDEOENC_OPT stPreView;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 2)]
        public DH_COLOR_CFG[] stColorCfg;
    }

    /// <summary>
    /// 语音对讲音频属性
    /// </summary>
    public struct DHDEV_TALK_CFG
    {
        #region << 音频输入参数 >>
        /// <summary>
        /// 编码类型，如PCM
        /// </summary>
        public byte byInFormatTag;
        /// <summary>
        /// 声道数
        /// </summary>
        public byte byInChannels;
        /// <summary>
        /// 采样深度	
        /// </summary>
        public UInt16 wInBitsPerSample;
        /// <summary>
        /// 采样率
        /// </summary>
        public UInt32 dwInSamplesPerSec;

        #endregion

        #region << 音频输出参数 >>
        /// <summary>
        /// 编码类型，如PCM
        /// </summary>
        public byte byOutFormatTag;
        /// <summary>
        /// 声道数
        /// </summary>
        public byte byOutChannels;
        /// <summary>
        /// 采样深度
        /// </summary>
        public UInt16 wOutBitsPerSample;
        /// <summary>
        /// 采样率
        /// </summary>
        public UInt32 dwOutSamplesPerSec;
        #endregion

    }

    /// <summary>
    /// 定时录像
    /// </summary>
    public struct DHDEV_RECORD_CFG
    {
        
        public UInt32 dwSize;
        /// <summary>
        /// 时间段结构
        /// </summary>
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 7)]
        public DH_REC_TSECT[] stSect;
        /// <summary>
        /// 预录时间,单位是s,0表示不预录 
        /// </summary>
        public byte byPreRecordLen;
        /// <summary>
        /// 录像冗余开关
        /// </summary>
        public byte byRedundancyEn;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 2)]
        public byte[] byReserved;
    }

    /// <summary>
    /// 报警配置
    /// </summary>
    public struct DH_PTZ_LINK
    {
        int iType;
        int iValue;
    }

    /// <summary>
    /// 消息触发配置
    /// 消息处理方式,可以同时多种处理方式,包括
    /// 0x00000001 - 网络:上传管理服务器
    /// 0x00000002 - 录像:触发
    /// 0x00000004 - 云台联动
    /// 0x00000008 - 发送邮件
    /// 0x00000010 - 设备本地报警轮巡
    /// 0x00000020 - 设备提示使能
    /// 0x00000040 - 设备报警输出使能
    /// 0x00000080 - Ftp上传使能
    /// 0x00000100 - 蜂鸣
    /// 0x00000200 - 语音提示
    /// 0x00000400 - 抓图使能
    /// </summary>
    public struct DH_MSG_HANDLE
    {
        /// <summary>
        /// 当前报警所支持的处理方式，按位掩码表示
        /// </summary>
        public UInt32 dwActionMask;
        /// <summary>
        /// 触发动作，按位掩码表示，具体动作所需要的参数在各自的配置中体现
        /// </summary>
        public UInt32 dwActionFlag;
        /// <summary>
        /// 报警触发的输出通道,为 1 表示触发该输出
        /// </summary>
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 16)]
        public byte[] byRelAlarmOut;
        /// <summary>
        /// 报警持续时间
        /// </summary>
        public UInt32 dwDuration;
        /// <summary>
        /// 联动录像	, 报警触发的录像通道,为1表示触发该通道	
        /// </summary>
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 16)]
        public byte[] byRecordChannel;
        /// <summary>
        /// 录像持续时间 
        /// </summary>
        public UInt32 dwRecLatch;
        /// <summary>
        /// 抓图通道
        /// </summary>
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 16)]
        public byte[] bySnap;
        /// <summary>
        /// 轮巡通道
        /// </summary>
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 16)]
        public byte[] byTour;
        /// <summary>
        /// 云台联动
        /// </summary>
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 16)]
        public DH_PTZ_LINK[] struPtzLink;
    }

    /// <summary>
    /// 外部报警
    /// </summary>
    public struct DH_ALARMIN_CFG
    {
        /// <summary>
        /// 报警器类型,0：常闭,1：常开  	
        /// </summary>
        public byte byAlarmType;
        /// <summary>
        /// 报警使能
        /// </summary>
        public byte byAlarmEn;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 2)]
        public byte[] byReserved;
        /// <summary>
        /// NSP
        /// </summary>
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 7)]
        public DH_REC_TSECT[] stSect;
        /// <summary>
        /// 处理方式
        /// </summary>
        public DH_MSG_HANDLE struHandle;
    }

    /// <summary>
    /// 检测区域[长度16]
    /// </summary>
    public struct DH_DETECT
    {
        [MarshalAs(UnmanagedType.ByValArray, SizeConst =32)]
        public byte[] Detected;
    }

    /// <summary>
    /// 动态检测
    /// </summary>
    public struct DH_MOTION_DETECT_CFG
    {
        /// <summary>
        /// 动态检测报警使能
        /// </summary>
        public byte byMotionEn;

        public byte byReserved;
        /// <summary>
        /// 灵敏度
        /// </summary>
        public UInt16 wSenseLevel;
        /// <summary>
        /// 动态检测区域的行数
        /// </summary>
        public UInt16 wMotionRow;
        /// <summary>
        /// 动态检测区域的列数
        /// </summary>
        public UInt16 wMotionCol;
        /// <summary>
        /// 检测区域，共32*32块区域
        /// </summary>
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 32)]
        public DH_DETECT[] byDetected;
        /// <summary>
        /// NSP
        /// </summary>
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 7)]
        public DH_REC_TSECT[] stSect;
        /// <summary>
        /// 处理方式
        /// </summary>
        public DH_MSG_HANDLE struHandle;
    }

    /// <summary>
    /// 视频丢失报警
    /// </summary>
    public struct DH_VIDEO_LOST_CFG
    {
        /// <summary>
        /// 视频丢失报警使能
        /// </summary>
        public byte byAlarmEn;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
        public byte[] byReserved;
        /// <summary>
        /// NSP
        /// </summary>
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 7)]
        public DH_REC_TSECT[] stSect;
        /// <summary>
        /// 处理方式
        /// </summary>
        public DH_MSG_HANDLE struHandle;
    }

    /// <summary>
    /// 图像遮挡报警
    /// </summary>
    public struct DH_BLIND_CFG
    {
        /// <summary>
        /// 使能
        /// </summary>
        public byte byBlindEnable;
        /// <summary>
        /// 灵敏度1-6 
        /// </summary>
        public byte byBlindLevel;

        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 2)]
        public byte[] byReserved;
        /// <summary>
        /// NSP
        /// </summary>
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 7)]
        public DH_REC_TSECT[] stSect;
        /// <summary>
        /// 处理方式
        /// </summary>
        public DH_MSG_HANDLE struHandle;
    }

    /// <summary>
    /// 硬盘消息(内部报警)
    /// </summary>
    public struct DH_DISK_ALARM_CFG
    {
        /// <summary>
        /// 无硬盘时报警
        /// </summary>
        public byte byNoDiskEn;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
        public byte[] byReserved_1;
        /// <summary>
        /// NSP
        /// </summary>
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 7)]
        public DH_REC_TSECT[] stNDSect;
        /// <summary>
        /// 处理方式 
        /// </summary>
        public DH_MSG_HANDLE struNDHandle;
        /// <summary>
        /// 硬盘低容量时报警
        /// </summary>
        public byte byLowCapEn;
        /// <summary>
        /// 容量阀值 0-99
        /// </summary>
        public byte byLowerLimit;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 2)]
        public byte[] byReserved_2;
        /// <summary>
        /// NSP
        /// </summary>
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 7)]
        public DH_REC_TSECT[] stLCSect;
        /// <summary>
        /// 处理方式  
        /// </summary>
        public DH_MSG_HANDLE struLCHandle;
        /// <summary>
        /// 硬盘故障报警
        /// </summary>
        public byte byDiskErrEn;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
        public byte[] byReserved_3;
        /// <summary>
        /// NSP
        /// </summary>
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 7)]
        public DH_REC_TSECT[] stEDSect;
        /// <summary>
        /// 处理方式 
        /// </summary>
        public DH_MSG_HANDLE struEDHandle;
    }

    public struct DH_NETBROKEN_ALARM_CFG
    {
        public byte byEnable;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
        public byte[] byReserved;
        public DH_MSG_HANDLE struHandle;
    }

    /// <summary>
    /// 报警布防
    /// </summary>
    public struct DHDEV_ALARM_SCHEDULE
    {
        public UInt32 dwSize;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 16)]
        public DH_ALARMIN_CFG[] struLocalAlmIn;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 16)]
        public DH_ALARMIN_CFG[] struNetAlmIn;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 16)]
        public DH_MOTION_DETECT_CFG[] struMotion;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 16)]
        public DH_VIDEO_LOST_CFG[] struVideoLost;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 16)]
        public DH_BLIND_CFG[] struBlind;
        public DH_DISK_ALARM_CFG struDiskAlarm;
        public DH_NETBROKEN_ALARM_CFG struNetBrokenAlarm;
    }

    /// <summary>
    /// 以太网配置
    /// </summary>
    public struct DH_ETHERNET
    {
        /// <summary>
        /// DVR IP 地址  
        /// </summary>
        //[MarshalAs(UnmanagedType.ByValArray, ArraySubType = UnmanagedType.ByValTStr, SizeConst = 16)]
        //public char[] sDevIPAddr;
        [MarshalAs(UnmanagedType.ByValArray,  SizeConst = 16)]
        public byte[] sDevIPAddr;
        /// <summary>
        /// DVR IP 地址掩码
        /// </summary>
        //[MarshalAs(UnmanagedType.ByValArray, ArraySubType = UnmanagedType.ByValTStr, SizeConst = 16)]
        //public char[] sDevIPMask;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 16)]
        public byte[] sDevIPMask;
        /// <summary>
        ///  网关地址   
        /// </summary>
        //[MarshalAs(UnmanagedType.ByValArray, ArraySubType = UnmanagedType.ByValTStr, SizeConst = 16)]
        //public char[] sGatewayIP;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 16)]
        public byte[] sGatewayIP;
        /// <summary>
        /// NSP
        /// 10M/100M  自适应,索引 
        /// 1-10MBase - T
        /// 2-10MBase-T 全双工 
        /// 3-100MBase - TX
        /// 4-100M 全双工
        /// 5-10M/100M  自适应 
        /// </summary>
        public UInt32 dwNetInterface;
        /// <summary>
        /// MAC地址，只读   
        /// </summary>
        //[MarshalAs(UnmanagedType.ByValArray, ArraySubType = UnmanagedType.ByValTStr, SizeConst = 40)]
        //public char[] byMACAddr;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 40)]
        public byte[] byMACAddr;
    }

    /// <summary>
    /// 远程主机配置
    /// </summary>
    public struct DH_REMOTE_HOST
    {
        /// <summary>
        /// 连接使能
        /// </summary>
        public byte byEnable;
        public byte byReserved;
        /// <summary>
        /// 远程主机端口
        /// </summary>
        public UInt16 wHostPort;
        /// <summary>
        /// 远程主机 IP 地址
        /// </summary>
        //[MarshalAs(UnmanagedType.ByValArray, ArraySubType = UnmanagedType.ByValTStr, SizeConst = 16)]
        //public char[] sHostIPAddr;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 16)]
        public byte[] sHostIPAddr;
        /// <summary>
        /// 远程主机 用户名
        /// </summary>
        //[MarshalAs(UnmanagedType.ByValArray, ArraySubType = UnmanagedType.ByValTStr, SizeConst = 64)]
        //public char[] sHostUser;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 64)]
        public byte[] sHostUser;
        /// <summary>
        /// 远程主机 密码
        /// </summary>
        //[MarshalAs(UnmanagedType.ByValArray, ArraySubType = UnmanagedType.ByValTStr, SizeConst = 32)]
        //public char[] sHostPassword;
        [MarshalAs(UnmanagedType.ByValArray,  SizeConst = 32)]
        public byte[] sHostPassword;
    }

    /// <summary>
    /// 邮件配置
    /// </summary>
    public struct DH_MAIL_CFG
    {
        /// <summary>
        /// 邮件服务器IP地址
        /// </summary>
        //[MarshalAs(UnmanagedType.ByValArray, ArraySubType = UnmanagedType.ByValTStr, SizeConst = 16)]
        //public char[] sMailIPAddr;
        [MarshalAs(UnmanagedType.ByValArray, ArraySubType = UnmanagedType.ByValTStr, SizeConst = 16)]
        public byte[] sMailIPAddr;

        /// <summary>
        /// 邮件服务器端口
        /// </summary>
        public UInt16 wMailPort;
        /// <summary>
        /// 保留
        /// </summary>
        public UInt16 wReserved;
        /// <summary>
        /// 发送地址
        /// </summary>
        //[MarshalAs(UnmanagedType.ByValArray, ArraySubType = UnmanagedType.ByValTStr, SizeConst = 128)]
        //public char[] sSenderAddr;
        [MarshalAs(UnmanagedType.ByValArray, ArraySubType = UnmanagedType.ByValTStr, SizeConst = 128)]
        public byte[] sSenderAddr;
        /// <summary>
        /// 用户名 
        /// </summary>
        //[MarshalAs(UnmanagedType.ByValArray, ArraySubType = UnmanagedType.ByValTStr, SizeConst = 16)]
        //public char[] sUserName;
        [MarshalAs(UnmanagedType.ByValArray,SizeConst = 16)]
        public byte[] sUserName;
        ///// <summary>
        ///// 用户名的字符串
        ///// </summary>
        ///// <returns></returns>
        //public string UserName()
        //{
        //    string result = "";
        //    foreach (char chr in sUserName)
        //    {
        //        if (chr != '\0')
        //        {
        //            result += chr.ToString();
        //        }
        //    }
        //    return result;
        //}
        /// <summary>
        /// 用户密码
        /// </summary>
        //[MarshalAs(UnmanagedType.ByValArray, ArraySubType = UnmanagedType.ByValTStr, SizeConst = 16)]
        //public char[] sUserPsw;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 16)]
        public byte[] sUserPsw;

        ///// <summary>
        ///// 用户密码的字符串
        ///// </summary>
        ///// <returns></returns>
        //public string UserPsw()
        //{
        //    string result = "";
        //    foreach (char chr in sUserPsw)
        //    {
        //        if (chr != '\0')
        //        {
        //            result += chr.ToString();
        //        }
        //    }
        //    return result;
        //}

        /// <summary>
        /// 目的地址
        /// </summary>
        //[MarshalAs(UnmanagedType.ByValArray, ArraySubType = UnmanagedType.ByValTStr, SizeConst = 128)]
        //public char[] sDestAddr;
        [MarshalAs(UnmanagedType.ByValArray,  SizeConst = 128)]
        public byte[] sDestAddr;
        /// <summary>
        /// 抄送地址
        /// </summary>
        //[MarshalAs(UnmanagedType.ByValArray, ArraySubType = UnmanagedType.ByValTStr, SizeConst = 128)]
        //public char[] sCcAddr;
        [MarshalAs(UnmanagedType.ByValArray,  SizeConst = 128)]
        public byte[] sCcAddr;
        /// <summary>
        /// 抄送地址的字符串
        /// </summary>
        /// <returns></returns>
        public string CcAddr()
        {
            string result = "";
            foreach (char chr in sCcAddr)
            {
                if (chr != '\0')
                {
                    result += chr.ToString();
                }
            }
            return result;
        }
        /// <summary>
        /// 暗抄地址
        /// </summary>
        //[MarshalAs(UnmanagedType.ByValArray, ArraySubType = UnmanagedType.ByValTStr, SizeConst = 128)]
        //public char[] sBccAddr;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 128)]
        public byte[] sBccAddr;
        /// <summary>
        /// 暗抄地址的字符串
        /// </summary>
        /// <returns></returns>
        public string BccAddr()
        {
            string result = "";
            foreach (char chr in sBccAddr)
            {
                if (chr != '\0')
                {
                    result += chr.ToString();
                }
            }
            return result;
        }
        /// <summary>
        /// 标题
        /// </summary>        
        //[MarshalAs(UnmanagedType.ByValArray, ArraySubType = UnmanagedType.ByValTStr, SizeConst = 64)]
        //public char[] sSubject;        
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 64)]
        public byte[] sSubject;       
        /// <summary>
        /// 标题的字符串
        /// </summary>
        /// <returns></returns>
        public string Subject()
        {
            string result = "";
            foreach (byte chr in sSubject)
            {
                if (chr != '\0')
                {
                    result += (char)chr;
                }
            }
            return result;
        }
    }

    /// <summary>
    /// 网络配置结构体
    /// </summary>
    public struct DHDEV_NET_CFG
    {
        public UInt32 dwSize;
        /// <summary>
        /// 设备主机名
        /// </summary>
        //[MarshalAs(UnmanagedType.ByValArray, ArraySubType = UnmanagedType.ByValTStr, SizeConst = 16)]
        //public char[] sDevName;
        [MarshalAs(UnmanagedType.ByValArray,  SizeConst = 16)]
        public byte[] sDevName;
        /// <summary>
        /// 设备主机名的字符串表达
        /// </summary>
        /// <returns>主机名的字符串</returns>
        public string DevName()
        {
            string result = "";
            foreach (char chr in sDevName)
            {
                if (chr != '\0')
                {
                    result += chr.ToString();
                }
            }
            return result;
        }
        /// <summary>
        /// TCP最大连接数(一般指视频数据请求数) 
        /// </summary>
        public UInt16 wTcpMaxConnectNum;
        /// <summary>
        /// TCP帧听端口
        /// </summary>
        public UInt16 wTcpPort;
        /// <summary>
        /// UDP侦听端口
        /// </summary>
        public UInt16 wUdpPort;
        /// <summary>
        /// HTTP端口号 
        /// </summary>
        public UInt16 wHttpPort;
        /// <summary>
        /// HTTPS端口号 
        /// </summary>
        public UInt16 wHttpsPort;
        /// <summary>
        /// SSL端口号
        /// </summary>
        public UInt16 wSslPort;
        /// <summary>
        /// 以太网口
        /// </summary>
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 2)]
        public DH_ETHERNET[] stEtherNet;
        /// <summary>
        /// 报警服务器
        /// </summary>
        public DH_REMOTE_HOST struAlarmHost;
        /// <summary>
        /// 日志服务器 
        /// </summary>
        public DH_REMOTE_HOST struLogHost;
        /// <summary>
        /// SMTP服务器
        /// </summary>
        public DH_REMOTE_HOST struSmtpHost;
        /// <summary>
        /// 多播组
        /// </summary>
        public DH_REMOTE_HOST struMultiCast;
        /// <summary>
        /// NFS服务器
        /// </summary>
        public DH_REMOTE_HOST struNfs;
        /// <summary>
        /// PPPoE服务器
        /// </summary>
        public DH_REMOTE_HOST struPppoe;
        /// <summary>
        /// PPPoE注册返回的IP
        /// </summary>
        //[MarshalAs(UnmanagedType.ByValArray, ArraySubType = UnmanagedType.ByValTStr, SizeConst = 16)]
        //public char[] sPppoeIP;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 16)]
        public byte[] sPppoeIP;
        /// <summary>
        /// DDNS服务器
        /// </summary>
        public DH_REMOTE_HOST struDdns;
        /// <summary>
        /// DDNS主机名
        /// </summary>
        //[MarshalAs(UnmanagedType.ByValArray, ArraySubType = UnmanagedType.ByValTStr, SizeConst = 64)]
        //public char[] sDdnsHostName;
        [MarshalAs(UnmanagedType.ByValArray,SizeConst = 64)]
        public byte[] sDdnsHostName;
        /// <summary>
        /// DNS服务器
        /// </summary>
        public DH_REMOTE_HOST struDns;
        /// <summary>
        /// 邮件配置
        /// </summary>
        public DH_MAIL_CFG struMail;
    }

    /// <summary>
    /// 串口基本属性
    /// </summary>
    public struct DH_COMM_PROP
    {
        /// <summary>
        /// 数据位 0:5;1:6;2:7;3-8;
        /// </summary>
        public byte byDataBit;
        /// <summary>
        /// 停止位 0:1位;1:1.5位; 2:2位;
        /// </summary>
        public byte byStopBit;
        /// <summary>
        /// 校验位 0:不校验;1:奇校验; 2:偶校验;
        /// </summary>
        public byte byParity;
        /// <summary>
        /// 波特率 0:300;1:600;2:1200;3:2400;4:4800;5:9600;6:19200;7:38400;8:57600;9:115200;
        /// </summary>
        public byte byBaudRate;
    }

    /// <summary>
    /// 485解码器配置
    /// </summary>
    public struct DH_485_CFG
    {
        public DH_COMM_PROP struComm;
        /// <summary>
        /// 协议类型 保存协议的下标，动态变化
        /// </summary>
        public UInt16 wProtocol;
        /// <summary>
        /// 解码器地址:0 - 255
        /// </summary>
        public UInt16 wDecoderAddress;
    }

    /// <summary>
    /// 232串口配置
    /// </summary>
    public struct DH_RS232_CFG
    {
        public DH_COMM_PROP struComm;
        public byte byFunction;			// 串口功能，对应串口配置取到的功能名列表 

        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)]
        public byte[] byReserved;
    }


    /// <summary>
    /// 协议名
    /// </summary>
    public struct DH_PROANDFUN_NAME
    {
        /// <summary>
        /// 协议名[长度16]
        /// </summary>
        //[MarshalAs(UnmanagedType.ByValArray, ArraySubType = UnmanagedType.ByValTStr, SizeConst = 16)]
        //public char[] ProName;
        [MarshalAs(UnmanagedType.ByValArray,  SizeConst = 16)]
        public byte[] ProName;
    }


    /// <summary>
    /// 串口配置结构体
    /// </summary>
    public struct DHDEV_COMM_CFG
    {
        /// <summary>
        /// 解码器协议
        /// </summary>
        public UInt32 dwSize;
        /// <summary>
        /// 协议个数
        /// </summary>
        public UInt32 dwDecProListNum;
        /// <summary>
        /// 协议名列表100
        /// </summary>
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 100)]
        public DH_PROANDFUN_NAME[] DecProName;
        /// <summary>
        /// 各解码器当前属性
        /// </summary>
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 16)]
        public DH_485_CFG[] stDecoder;
        /// <summary>
        /// 232功能个数
        /// </summary>
        public UInt32 dw232FuncNameNum;
        /// <summary>
        /// 功能名列表10*16
        /// </summary>
        [MarshalAs(UnmanagedType.ByValArray, ArraySubType = UnmanagedType.ByValTStr, SizeConst = 10)]
        public DH_PROANDFUN_NAME[] s232FuncName;
        /// <summary>
        /// 各232串口当前属性
        /// </summary>
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 2)]
        public DH_RS232_CFG[] st232;
    }

    /// <summary>
    /// 自动维护属性
    /// </summary>
    public struct DHDEV_AUTOMT_CFG
    {
        /// <summary>
        /// 自动重启
        /// </summary>
        public UInt32 dwSize;
        /// <summary>
        ///  自动重启日期设定 0:从不;1:每天;2:每星期日;3:每星期一;......
        /// </summary>
        public byte byAutoRebootDay;
        /// <summary>
        /// 自动重启时间设定 0:0:00;1:1:00;........23:23:00;
        /// </summary>
        public byte byAutoRebootTime;
        /// <summary>
        /// 自动删除文件 0:从不;1:24H;2:48H;3:72H;4:96H:5:一周;6:一月
        /// </summary>
        public byte byAutoDeleteFilesTime;
        /// <summary>
        /// 保留位
        /// </summary>
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 13)]
        public byte[] reserved;
    }

    /// <summary>
    /// 本机控制策略配置
    /// </summary>
    public struct DH_VIDEOGROUP_CFG
    {
        /// <summary>
        /// 视频输出 0:无效;1:设备通道数表示对应通道;设备通道数+1代表all;
        /// </summary>
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 6)]
        public byte[] byVideoOut;
        /// <summary>
        /// 轮巡间隔，单位秒, 5-300 
        /// </summary>
        public int iInterval;
        /// <summary>
        /// 是否轮巡 
        /// </summary>
        public byte byEnableTour;
        /// <summary>
        /// 联动报警通道 0:无效;1:报警通道数表示对应通道;报警通道数+1代表1-4;报警通道数+2代表5-8...;参考本地界面
        /// </summary>
        public byte byAlarmChannel;

        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 2)]
        public byte[] byReserved;
    }

    /// <summary>
    /// 本机矩阵控制策略配置
    /// </summary>
    public struct DHDEV_VIDEO_MATRIX_CFG
    {
        public UInt32 dwSize;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 4)]
        public DH_VIDEOGROUP_CFG[] struVideoGroup;
    }

    /// <summary>
    /// ddns配置
    /// </summary>
    public struct DH_DDNS_SERVER_CFG
    {
        /// <summary>
        /// ddns服务器id号
        /// </summary>
        public UInt32 dwId;
        /// <summary>
        /// 使能，同一时间只能有一个ddns服务器处于使能状态
        /// </summary>
        public bool bEnable;
        /// <summary>
        /// 服务器类型，希网..
        /// </summary>	
        //[MarshalAs(UnmanagedType.ByValArray, ArraySubType = UnmanagedType.ByValTStr, SizeConst = 32)]
        //public char[] szServerType;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 32)]
        public byte[] szServerType;
        /// <summary>
        /// 服务器ip
        /// </summary>
        //[MarshalAs(UnmanagedType.ByValArray, ArraySubType = UnmanagedType.ByValTStr, SizeConst = 16)]
        //public char[] szServerIp;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 16)]
        public byte[] szServerIp;
        /// <summary>
        /// 服务器端口
        /// </summary>
        public UInt32 dwServerPort;
        /// <summary>
        /// dvr域名如jecke.3322.org
        /// </summary>
        //[MarshalAs(UnmanagedType.ByValArray, ArraySubType = UnmanagedType.ByValTStr, SizeConst = 256)]
        //public char[] szDomainName;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 256)]
        public byte[] szDomainName;
        /// <summary>
        /// 用户名
        /// </summary>
        //[MarshalAs(UnmanagedType.ByValArray, ArraySubType = UnmanagedType.ByValTStr, SizeConst = 32)]
        //public char[] szUserName;
        [MarshalAs(UnmanagedType.ByValArray,  SizeConst = 32)]
        public byte[] szUserName;
        /// <summary>
        /// 密码
        /// </summary>
        //[MarshalAs(UnmanagedType.ByValArray, ArraySubType = UnmanagedType.ByValTStr, SizeConst = 32)]
        //public char[] szUserPsw;
        [MarshalAs(UnmanagedType.ByValArray,  SizeConst = 32)]
        public byte[] szUserPsw;
        /// <summary>
        /// 服务器别名，如"dahua ddns"
        /// </summary>
        //[MarshalAs(UnmanagedType.ByValArray, ArraySubType = UnmanagedType.ByValTStr, SizeConst = 32)]
        //public char[] szAlias;
        [MarshalAs(UnmanagedType.ByValArray,  SizeConst = 32)]
        public byte[] szAlias;
    }
    /// <summary>
    /// 多ddns配置
    /// </summary>
    public struct DHDEV_MULTI_DDNS_CFG
    {
        public UInt32 dwSize;
        public UInt32 dwDdnsServerNum;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 10)]
        public DH_DDNS_SERVER_CFG[] struDdnsServer;
    }

    /// <summary>
    /// 抓图功能配置
    /// </summary>
    public struct DHDEV_SNAP_CFG
    {
        public UInt32 dwSize;
        /// <summary>
        /// 定时抓图开关（报警抓图开关在各报警联动配置中体现）
        /// </summary>
        public bool bTimingEnable;
        /// <summary>
        /// 抓图编码配置，现支持其中的分辨率、画质、帧率设置，帧率在这里是负数，表示一秒抓图的次数。
        /// </summary>
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 32)]
        public DH_VIDEOENC_OPT[] struSnapEnc;
    }

    /// <summary>
    /// web路径配置
    /// </summary>
    public struct DHDEV_URL_CFG
    {
        public UInt32 dwSize;
        /// <summary>
        /// 是否抓图
        /// </summary>
        public bool bSnapEnable;
        /// <summary>
        /// 抓图周期
        /// </summary>
        public int iSnapInterval;
        /// <summary>
        /// HTTP主机IP
        /// </summary>
        //[MarshalAs(UnmanagedType.ByValArray, ArraySubType = UnmanagedType.ByValTStr, SizeConst = 16)]
        //public char[] szHostIp;
        [MarshalAs(UnmanagedType.ByValArray,SizeConst = 16)]
        public byte[] szHostIp;
        public UInt16 wHostPort;
        /// <summary>
        /// 状态消息发送间隔
        /// </summary>
        public int iMsgInterval;
        /// <summary>
        /// 状态消息上传URL
        /// </summary>
        //[MarshalAs(UnmanagedType.ByValArray, ArraySubType = UnmanagedType.ByValTStr, SizeConst = 128)]
        //public char[] szUrlState;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 128)]
        public byte[] szUrlState;
        /// <summary>
        /// 图片上传URL
        /// </summary>
        //[MarshalAs(UnmanagedType.ByValArray, ArraySubType = UnmanagedType.ByValTStr, SizeConst = 128)]
        //public char[] szUrlImage;
        [MarshalAs(UnmanagedType.ByValArray,  SizeConst = 128)]
        public byte[] szUrlImage;
        /// <summary>
        /// 机器的web编号
        /// </summary>
        //[MarshalAs(UnmanagedType.ByValArray, ArraySubType = UnmanagedType.ByValTStr, SizeConst = 48)]
        //public char[] szDevId;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 48)]
        public byte[] szDevId;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 2)]
        public byte byReserved;
    }


    /// <summary>
    /// 
    /// </summary>
    public struct struPeriod
    {
        /// <summary>
        /// 该时间段内的“使能”无效，可忽略
        /// </summary>
        public DH_TSECT struSect;
        /// <summary>
        /// 上传动态检测录像
        /// </summary>
        public bool bMdEn;
        /// <summary>
        /// 上传外部报警录像
        /// </summary>
        public bool bAlarmEn;
        /// <summary>
        /// 上传普通定时录像
        /// </summary>	
        public bool bTimerEn;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 4)]
        public UInt32[] dwRev;
    }
    /// <summary>
    /// FTP上传配置
    /// </summary>
    public struct DH_FTP_UPLOAD_CFG
    {

        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 2)]
        public struPeriod[] Period;
    }

    /// <summary>
    /// FTP上传配置
    /// </summary>
    public struct DHDEV_FTP_PROTO_CFG
    {
        public UInt32 dwSize;

        /// <summary>
        /// 是否启用
        /// </summary>
        public bool bEnable;
        /// <summary>
        /// 主机IP
        /// </summary>
        //[MarshalAs(UnmanagedType.ByValArray, ArraySubType = UnmanagedType.ByValTStr, SizeConst = 16)]
        //public char[] szHostIp;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 16)]
        public byte[] szHostIp;
        /// <summary>
        /// 主机端口
        /// </summary>
        public UInt32 wHostPort;
        /// <summary>
        /// FTP目录路径
        /// </summary>
        //[MarshalAs(UnmanagedType.ByValArray, ArraySubType = UnmanagedType.ByValTStr, SizeConst = 240)]
        //public char[] szDirName;
        [MarshalAs(UnmanagedType.ByValArray,  SizeConst = 240)]
        public byte[] szDirName;
        /// <summary>
        /// 用户名
        /// </summary>
        //[MarshalAs(UnmanagedType.ByValArray, ArraySubType = UnmanagedType.ByValTStr, SizeConst = 64)]
        //public char[] szUserName;
        [MarshalAs(UnmanagedType.ByValArray,SizeConst = 64)]
        public byte[] szUserName;
        /// <summary>
        /// 密码
        /// </summary>
        //[MarshalAs(UnmanagedType.ByValArray, ArraySubType = UnmanagedType.ByValTStr, SizeConst = 64)]
        //public char[] szPassword;
        [MarshalAs(UnmanagedType.ByValArray,  SizeConst = 64)]
        public byte[] szPassword;
        /// <summary>
        /// 文件长度
        /// </summary>
        public int iFileLen;
        /// <summary>
        /// 相邻文件时间间隔
        /// </summary>
        public int iInterval;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 42)]
        public DH_FTP_UPLOAD_CFG[][] struUploadCfg;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 130)]
        public byte[] reserved;
    }

    /// <summary>
    /// 平台接入配置 － U网通平台
    /// </summary>
    public struct DH_INTERVIDEO_UCOM_CHN_CFG
    {
        public bool bChnEn;
        //[MarshalAs(UnmanagedType.ByValArray, ArraySubType = UnmanagedType.ByValTStr, SizeConst = 32)]
        //public char[] szChnId;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 32)]
        public byte[] szChnId;
    }

    /// <summary>
    /// 平台接入配置 － U网通平台
    /// </summary>
    public struct DHDEV_INTERVIDEO_UCOM_CFG
    {
        public UInt32 dwSize;
        /// <summary>
        /// 接入功能使能与否 0:使能
        /// </summary>
        public bool bFuncEnable;
        /// <summary>
        /// 心跳使能与否
        /// </summary>
        public bool bAliveEnable;
        /// <summary>
        /// 心跳周期，单位秒，0-3600
        /// </summary>
        public UInt32 dwAlivePeriod;
        /// <summary>
        /// CMS的IP
        /// </summary>
        //[MarshalAs(UnmanagedType.ByValArray, ArraySubType = UnmanagedType.ByValTStr, SizeConst = 16)]
        //public char[] szServerIp;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 16)]
        public byte[] szServerIp;
        /// <summary>
        /// CMS的Port
        /// </summary>
        public UInt16 wServerPort;
        /// <summary>
        /// 注册密码
        /// </summary>
        //[MarshalAs(UnmanagedType.ByValArray, ArraySubType = UnmanagedType.ByValTStr, SizeConst = 16)]
        //public char[] szRegPwd;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 16)]
        public byte[] szRegPwd;
        /// <summary>
        /// 设备id
        /// </summary>
        //[MarshalAs(UnmanagedType.ByValArray, ArraySubType = UnmanagedType.ByValTStr, SizeConst = 32)]
        //public char[] szDeviceId;
        [MarshalAs(UnmanagedType.ByValArray,  SizeConst = 32)]
        public byte[] szDeviceId;
        //[MarshalAs(UnmanagedType.ByValArray, ArraySubType = UnmanagedType.ByValTStr, SizeConst = 32)]
        //public char[] szUserName;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 32)]
        public byte[] szUserName;
        //[MarshalAs(UnmanagedType.ByValArray, ArraySubType = UnmanagedType.ByValTStr, SizeConst = 32)]
        //public char[] szPassWord;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 32)]
        public byte[] szPassWord;
        /// <summary>
        /// 通道id,en
        /// </summary>
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 16)]
        public DH_INTERVIDEO_UCOM_CHN_CFG[] struChnInfo;
    }

    #endregion

    #region << 枚举定义>>

    /// <summary>
    /// 实时播放类型
    /// </summary>
    public enum REALPLAY_TYPE
    {
        DH_RType_RealPlay = 0,
        DH_RType_Multiplay,
        DH_RType_RealPlay_0,
        DH_RType_RealPlay_1,
        DH_RType_RealPlay_2,
        DH_RType_RealPlay_3,
        DH_RType_Multiplay_1,
        DH_RType_Multiplay_4,
        DH_RType_Multiplay_8,
        DH_RType_Multiplay_9,
        DH_RType_Multiplay_16,
    }

    /// <summary>
    /// 播放控制命令
    /// </summary>
    public enum PLAY_CONTROL
    {
        /// <summary>
        /// 播放
        /// </summary>
        Play,
        /// <summary>
        /// 暂停
        /// </summary>
        Pause,
        /// <summary>
        /// 停止
        /// </summary>
        Stop,
        /// <summary>
        /// 拖动播放[按时间偏移]
        /// </summary>
        SeekByTime,
        /// <summary>
        /// 拖动播放[按字节偏移]
        /// </summary>
        SeekByBit,
        /// <summary>
        /// 单步播放开始[调用一次播放一帧]
        /// </summary>
        StepPlay,
        /// <summary>
        /// 单步播放停止
        /// </summary>
        StepStop,
        /// <summary>
        /// 快放
        /// </summary>
        Fast,
        /// <summary>
        /// 慢放
        /// </summary>
        Slow
    }

    /// <summary>
    /// 云台控制命令
    /// </summary>
    public enum PTZ_CONTROL
    {
        /// <summary>
        /// 上
        /// </summary>
        PTZ_UP_CONTROL = 0,
        /// <summary>
        /// 下
        /// </summary>
        PTZ_DOWN_CONTROL,
        /// <summary>
        /// 左
        /// </summary>
        PTZ_LEFT_CONTROL,
        /// <summary>
        /// 右
        /// </summary>
        PTZ_RIGHT_CONTROL,
        /// <summary>
        /// 变倍+
        /// </summary>
        PTZ_ZOOM_ADD_CONTROL,
        /// <summary>
        /// 变倍-
        /// </summary>
        PTZ_ZOOM_DEC_CONTROL,
        /// <summary>
        /// 调焦+
        /// </summary>
        PTZ_FOCUS_ADD_CONTROL,
        /// <summary>
        /// 调焦- 
        /// </summary>
        PTZ_FOCUS_DEC_CONTROL,
        /// <summary>
        /// 光圈+ 
        /// </summary>
        PTZ_APERTURE_ADD_CONTROL,
        /// <summary>
        /// 光圈- 
        /// </summary>
        PTZ_APERTURE_DEC_CONTROL,
        /// <summary>
        /// 转至预置点 
        /// </summary>
        PTZ_POINT_MOVE_CONTROL,
        /// <summary>
        /// 设置 
        /// </summary>
        PTZ_POINT_SET_CONTROL,
        /// <summary>
        /// 删除 
        /// </summary>
        PTZ_POINT_DEL_CONTROL,
        /// <summary>
        /// 点间轮循 
        /// </summary>
        PTZ_POINT_LOOP_CONTROL,
        /// <summary>
        /// 灯光雨刷	
        /// </summary>
        PTZ_LAMP_CONTROL,
        /// <summary>
        /// 左上:p1水平速度, p2垂直速度
        /// </summary>
        EXTPTZ_LEFTTOP = 0X20,
        /// <summary>
        /// 右上:p1水平速度, p2垂直速度
        /// </summary>
        EXTPTZ_RIGHTTOP,
        /// <summary>
        /// 左下:p1水平速度, p2垂直速度
        /// </summary>
        EXTPTZ_LEFTDOWN,
        /// <summary>
        /// 右下:p1水平速度, p2垂直速度
        /// </summary>
        EXTPTZ_RIGHTDOWN,
        /// <summary>
        /// 加入预置点到巡航:p1巡航线路	p2预置点值
        /// </summary>
        EXTPTZ_ADDTOLOOP,
        /// <summary>
        /// 删除巡航中预置点:p1巡航线路	p2预置点值
        /// </summary>
        EXTPTZ_DELFROMLOOP,
        /// <summary>
        /// 清除巡航:p1巡航线路	
        /// </summary>
        EXTPTZ_CLOSELOOP,
        /// <summary>
        /// 开始水平旋转
        /// </summary>
        EXTPTZ_STARTPANCRUISE,
        /// <summary>
        /// 停止水平旋转	
        /// </summary>
        EXTPTZ_STOPPANCRUISE,
        /// <summary>
        /// 设置左边界
        /// </summary>
        EXTPTZ_SETLEFTBORDER,
        /// <summary>
        /// 设置右边界		
        /// </summary>
        EXTPTZ_RIGHTBORDER,
        /// <summary>
        /// 开始线扫
        /// </summary>
        EXTPTZ_STARTLINESCAN,
        /// <summary>
        /// 停止线扫
        /// </summary>
        EXTPTZ_CLOSELINESCAN,
        /// <summary>
        /// 设置模式开始:模式线路
        /// </summary>
        EXTPTZ_SETMODESTART,
        /// <summary>
        /// 设置模式结束:模式线路
        /// </summary>
        EXTPTZ_SETMODESTOP,
        /// <summary>
        /// 运行模式:p1模式线路
        /// </summary>		
        EXTPTZ_RUNMODE,
        /// <summary>
        /// 停止模式:p1模式线路
        /// </summary>
        EXTPTZ_STOPMODE,
        /// <summary>
        /// 清除模式:p1模式线路
        /// </summary>
        EXTPTZ_DELETEMODE,
        /// <summary>
        /// 翻转命令
        /// </summary>
        EXTPTZ_REVERSECOMM,
        /// <summary>
        /// 快速定位  p1水平坐标 p2垂直坐标	 p3变倍
        /// </summary>
        EXTPTZ_FASTGOTO,
        /// <summary>
        /// x34:辅助开关开	 p1辅助点		
        /// </summary>
        EXTPTZ_AUXIOPEN,
        /// <summary>
        /// 0x35:辅助开关关	p1辅助点
        /// </summary>
        EXTPTZ_AUXICLOSE,
        /// <summary>
        /// 打开球机菜单
        /// </summary>
        EXTPTZ_OPENMENU = 0X36,
        /// <summary>
        /// 关闭菜单
        /// </summary>
        EXTPTZ_CLOSEMENU,
        /// <summary>
        /// 菜单确定
        /// </summary>
        EXTPTZ_MENUOK,
        /// <summary>
        /// 菜单取消
        /// </summary>
        EXTPTZ_MENUCANCEL,
        /// <summary>
        /// 菜单上
        /// </summary>
        EXTPTZ_MENUUP,
        /// <summary>
        /// 菜单下
        /// </summary>
        EXTPTZ_MENUDOWN,
        /// <summary>
        /// 菜单左
        /// </summary>
        EXTPTZ_MENULEFT,
        /// <summary>
        /// 菜单右
        /// </summary>
        EXTPTZ_MENURIGHT,
        /// <summary>
        /// 最大命令值
        /// </summary>
        EXTPTZ_TOTAL
    }

    /// <summary>
    /// 录像文件类型
    /// </summary>
    public enum RECORD_FILE_TYPE
    {
        /// <summary>
        /// 0:所有录像文件
        /// </summary>
        ALLRECORDFILE,
        /// <summary>
        /// 1:外部报警   
        /// </summary>
        OUTALARM,
        /// <summary>
        /// 2:动态检测报警
        /// </summary>
        DYNAMICSCANALARM,
        /// <summary>
        /// 3:所有报警
        /// </summary>
        ALLALARM,
        /// <summary>
        /// 4:卡号查询
        /// </summary>
        CARDNOSEACH,
        /// <summary>
        /// 5:组合条件查询
        /// </summary>
        COMBINEDSEACH
    }

    /// <summary>
    /// 配制命令
    /// </summary>
    public enum CONFIG_COMMAND
    {
        /// <summary>
        /// 1:获取设备参数,参见DHDEV_SYSTEM_ATTR_CFG
        /// </summary>
        DH_DEV_DEVICECFG = 1,
        /// <summary>
        /// 2:获取网络参数,参见DHDEV_NET_CFG
        /// </summary>
        DH_DEV_NETCFG = 2,
        /// <summary>
        /// 3:获取通道配置－图像及压缩参数等,参见DHDEV_CHANNEL_CFG
        /// </summary>
        DH_DEV_CHANNELCFG = 3,
        /// <summary>
        /// 4:获取预览参数,参见DHDEV_PREVIEW_CFG
        /// </summary>
        DH_DEV_PREVIEWCFG = 4,
        /// <summary>
        /// 5:获取录像时间参数,参见DHDEV_RECORD_CFG
        /// </summary>
        DH_DEV_RECORDCFG = 5,
        /// <summary>
        /// 6:获取串口参数,参见DHDEV_COMM_CFG
        /// </summary>
        DH_DEV_COMMCFG = 6,
        /// <summary>
        /// 7:获取报警参数,参见DHDEV_ALARM_SCHEDULE
        /// </summary>
        DH_DEV_ALARMCFG = 7,
        /// <summary>
        /// 8:获取DVR时间,参见NET_TIME结构
        /// </summary>
        DH_DEV_TIMECFG = 8,
        /// <summary>
        /// 9:对讲参数,参见DHDEV_TALK_CFG
        /// </summary>
        DH_DEV_TALKCFG = 9,
        /// <summary>
        /// 10:自动维护配置
        /// </summary>
        DH_DEV_AUTOMTCFG = 10,
        /// <summary>
        /// 11:本机矩阵控制策略配置	
        /// </summary>
        DH_DEV_VEDIO_MARTIX = 11,
        /// <summary>
        /// 12:多ddns服务器配置
        /// </summary>
        DH_DEV_MULTI_DDNS = 12,
        /// <summary>
        /// 13:抓图相关配置
        /// </summary>
        DH_DEV_SNAP_CFG = 13,
        /// <summary>
        /// 14:HTTP路径配置
        /// </summary>
        DH_DEV_WEB_URL_CFG = 14,
        /// <summary>
        /// 15:FTP上传配置
        /// </summary>
        DH_DEV_FTP_PROTO_CFG = 15,
        /// <summary>
        /// 16:平台接入配置，此时channel参数代表平台类型，如 11 代表U网通
        /// </summary>
        DH_DEV_INTERVIDEO_CFG = 16
    }

    /// <summary>
    /// 设备型号(DVR类型)
    /// </summary>
    public enum NET_DEVICE_TYPE
    {
        NET_PRODUCT_NONE = 0,
        /// <summary>
        /// 非实时MACE  
        /// </summary>
        NET_DVR_NONREALTIME_MACE,
        /// <summary>
        /// 非实时 
        /// </summary>
        NET_DVR_NONREALTIME,
        /// <summary>
        /// 网络视频服务器
        /// </summary>
        NET_NVS_MPEG1,
        /// <summary>
        ///  MPEG1二路录像机  
        /// </summary>
        NET_DVR_MPEG1_2,
        /// <summary>
        /// MPEG1八路录像机  
        /// </summary>
        NET_DVR_MPEG1_8,
        /// <summary>
        /// MPEG4八路录像机
        /// </summary>
        NET_DVR_MPEG4_8,
        /// <summary>
        /// MPEG4十六路录像机  
        /// </summary>
        NET_DVR_MPEG4_16,
        /// <summary>
        /// MPEG4视新十六路录像机  
        /// </summary>
        NET_DVR_MPEG4_SX2,
        /// <summary>
        /// MPEG4视通录像机
        /// </summary>
        NET_DVR_MEPG4_ST2,
        /// <summary>
        /// MPEG4视豪录像机
        /// </summary>
        NET_DVR_MEPG4_SH2,
        /// <summary>
        /// MPEG4视通二代增强型录像机
        /// </summary>
        NET_DVR_MPEG4_GBE,
        /// <summary>
        /// MPEG4网络视频服务器II代
        /// </summary>
        NET_DVR_MPEG4_NVSII,
        /// <summary>
        /// 新标准配置协议
        /// </summary>
        NET_DVR_STD_NEW,
        /// <summary>
        /// DDNS服务器
        /// </summary>
        NET_DVR_DDNS
    };

    /// <summary>
    /// 查询日志扩展接口参数：设备日志类型
    /// </summary>
    public enum DH_LOG_QUERY_TYPE
    {
        /// <summary>
        /// 所有日志
        /// </summary>
        DHLOG_ALL = 0,
        /// <summary>
        /// 系统日志
        /// </summary>
        DHLOG_SYSTEM,
        /// <summary>
        /// 配置日志
        /// </summary>
        DHLOG_CONFIG,
        /// <summary>
        /// 存储相关
        /// </summary>
        DHLOG_STORAGE,
        /// <summary>
        /// 报警日志
        /// </summary>
        DHLOG_ALARM,
        /// <summary>
        /// 录像相关
        /// </summary>
        DHLOG_RECORD,
        /// <summary>
        /// 帐号相关
        /// </summary>
        DHLOG_ACCOUNT,
        /// <summary>
        /// 清除日志
        /// </summary>
        DHLOG_CLEAR,
        /// <summary>
        /// 回放相关
        /// </summary>
        DHLOG_PLAYBACK
    }

    /// <summary>
    /// 日志的类型
    /// </summary>
    public enum DH_LOG_TYPE
    {
        /// <summary>
        /// 重起应用程序
        /// </summary>
        DH_LOG_REBOOT = 0x0000,
        /// <summary>
        /// 关闭应用程序
        /// </summary>
        DH_LOG_SHUT,
        /// <summary>
        /// 保存配制
        /// </summary>
        DH_LOG_CONFSAVE = 0x0100,
        /// <summary>
        /// 读取配置
        /// </summary>
        DH_LOG_CONFLOAD,
        /// <summary>
        /// 文件系统错误
        /// </summary>
        DH_LOG_FSERROR = 0x0200,
        /// <summary>
        /// 硬盘写错误
        /// </summary>
        DH_LOG_HDD_WERR,
        /// <summary>
        /// 硬盘读错误
        /// </summary>
        DH_LOG_HDD_RERR,
        /// <summary>
        /// 设置硬盘类型
        /// </summary>
        DH_LOG_HDD_TYPE,
        /// <summary>
        /// 格式化硬盘
        /// </summary>
        DH_LOG_HDD_FORMAT,
        /// <summary>
        /// 外部报警输入
        /// </summary>
        DH_LOG_ALM_IN = 0x0300,
        /// <summary>
        /// 外部报警输入停止
        /// </summary>
        DH_LOG_ALM_END = 0x0302,
        /// <summary>
        /// 手动录像开
        /// </summary>
        DH_LOG_MANUAL_RECORD = 0x0401,
        /// <summary>
        /// 录像停止
        /// </summary>
        DH_LOG_CLOSED_RECORD,
        /// <summary>
        /// 用户管理:登录
        /// </summary>
        DH_LOG_LOGIN = 0x0500,
        /// <summary>
        /// 用户管理:注销
        /// </summary>
        DH_LOG_LOGOUT,
        /// <summary>
        /// 用户管理:添加用户
        /// </summary>
        DH_LOG_ADD_USER,
        /// <summary>
        /// 用户管理:删除用户
        /// </summary>
        DH_LOG_DELETE_USER,
        /// <summary>
        /// 用户管理:修改用户
        /// </summary>
        DH_LOG_MODIFY_USER,
        /// <summary>
        /// 用户管理:添加组
        /// </summary>
        DH_LOG_ADD_GROUP,
        /// <summary>
        /// 用户管理:删除组
        /// </summary>
        DH_LOG_DELETE_GROUP,
        /// <summary>
        /// 用户管理:修改组
        /// </summary>
        DH_LOG_MODIFY_GROUP,
        /// <summary>
        /// 日志清除
        /// </summary>
        DH_LOG_CLEAR = 0x0600,			//clear 
        /// <summary>
        /// 
        /// </summary>
        DH_LOG_TYPE_NR = 7,
    }

    /// <summary>
    /// 配制的类型
    /// </summary>
    /// C++中名称为CFG_INDEX
    public enum CFG_TYPE
    {
        /// <summary>
        /// 普通
        /// </summary>
        CFG_GENERAL = 0,
        /// <summary>
        /// 串口
        /// </summary>
        CFG_COMM,
        /// <summary>
        /// 网络
        /// </summary>
        CFG_NET,
        /// <summary>
        /// 录像
        /// </summary>
        CFG_RECORD,
        /// <summary>
        /// 视频通道
        /// </summary>
        CFG_CAPTURE,
        /// <summary>
        /// 云台
        /// </summary>
        CFG_PTZ,
        /// <summary>
        /// 动态检测
        /// </summary>
        CFG_DETECT,
        /// <summary>
        /// 报警
        /// </summary>
        CFG_ALARM,
        /// <summary>
        /// 显示
        /// </summary>
        CFG_DISPLAY,
        /// <summary>
        /// 通道标题
        /// </summary>
        CFG_TITLE = 10,
        /// <summary>
        /// 邮件
        /// </summary>
        CFG_MAIL,
        /// <summary>
        /// 预览图像配置
        /// </summary>
        CFG_EXCAPTURE,
        /// <summary>
        /// pppoe拨号配置
        /// </summary>
        CFG_PPPOE,
        /// <summary>
        /// ddns配置
        /// </summary>
        CFG_DDNS,
        /// <summary>
        /// 抓包配置
        /// </summary>
        CFG_SNIFFER,
        /// <summary>
        /// DSP编码能力信息
        /// </summary>
        CFG_DSPINFO,
        /// <summary>
        /// 色彩配置	
        /// </summary>
        CFG_COLOR = 126,
        /// <summary>
        /// 所有配置
        /// </summary>
        CFG_ALL,
    }

    /// <summary>
    /// 定时录像类型－定时、动态检测、报警
    /// </summary>
    public enum REC_TYPE
    {
        DH_REC_TYPE_TIM = 0,
        DH_REC_TYPE_MTD,
        DH_REC_TYPE_ALM,
        DH_REC_TYPE_NUM,
    }

    /// <summary>
    /// 用户管理操作命令
    /// </summary>
    public enum USER_OPERATE
    {
        /// <summary>
        /// 新增用户组
        /// </summary>
        DH_GROUP_ADD=0,
        /// <summary>
        /// 删除用户组
        /// </summary>
        DH_GROUP_DELETE,
        /// <summary>
        /// 修改用户组
        /// </summary>
        DH_GROUP_EDIT,
        /// <summary>
        /// 新增用户
        /// </summary>
        DH_USER_ADD,
        /// <summary>
        /// 删除用户
        /// </summary>
        DH_USER_DELETE,
        /// <summary>
        /// 修改用户
        /// </summary>
        DH_USER_EDIT,
        /// <summary>
        /// 修改用户密码
        /// </summary>
        DH_USER_CHANGEPASSWORD
        
    }

    /// <summary>
    /// 回调信息类型
    /// </summary>
    public enum CALLBACK_TYPE
    {        
        /// <summary>
        /// 常规报警信息
        /// </summary>
        DH_COMM_ALARM = 0x1100,
        /// <summary>
        /// 视频遮挡报警
        /// </summary>
        DH_SHELTER_ALARM = 0x1101,
        /// <summary>
        /// 硬盘满报警
        /// </summary>
        DH_DISK_FULL_ALARM = 0X1102,
        /// <summary>
        /// 硬盘故障报警
        /// </summary>
        DH_DISK_ERROR_ALARM = 0x1103,
        /// <summary>
        /// 音频检测报警
        /// </summary>
        DH_SOUND_DETECT_ALARM = 0x1104,
        /// <summary>
        /// 报警解码器报警
        /// </summary>
        DH_ALARM_DECODER_ALARM = 0x1105,

        /*以下为扩展回调报警信息类型，对应CLIENT_StartListenEx*/

        /// <summary>
        /// 外部报警
        /// </summary>
        DH_ALARM_ALARM_EX = 0x2101,
        /// <summary>
        /// 动态检测报警
        /// </summary>
        DH_MOTION_ALARM_EX = 0x2102,
        /// <summary>
        /// /视频丢失报警
        /// </summary>
        DH_VIDEOLOST_ALARM_EX = 0X2103,
        /// <summary>
        /// 遮挡报警
        /// </summary>
        DH_SHELTER_ALARM_EX = 0x2104,
        /// <summary>
        /// 音频检测报警
        /// </summary>
        DH_SOUND_DETECT_ALARM_EX = 0x2105,
        /// <summary>
        /// 硬盘满报警
        /// </summary>    
        DH_DISKFULL_ALARM_EX = 0x2106,
        /// <summary>
        /// 坏硬盘报警
        /// </summary>    
        DH_DISKERROR_ALARM_EX = 0x2107
    }

    /// <summary>
    /// 表示字符编码
    /// </summary>
    public enum LANGUAGE_ENCODING
    {
        /// <summary>
        /// IBM EBCDIC（美国 - 加拿大） 
        /// </summary>
        IBM037 = 37,
        /// <summary>
        /// OEM 美国 
        /// </summary>
        IBM437 = 437,
        /// <summary>
        /// IBM EBCDIC（国际） 
        /// </summary>
        IBM500 = 500,
        /// <summary>
        /// 阿拉伯字符 (ASMO 708) 
        /// </summary>
        ASMO_708 = 708,
        /// <summary>
        /// 阿拉伯字符 (DOS) 
        /// </summary>
        DOS_720 = 720,
        /// <summary>
        /// 希腊字符 (DOS) 
        /// </summary>
        ibm737 = 737,
        /// <summary>
        /// 波罗的海字符 (DOS) 
        /// </summary>
        ibm775 = 775,
        /// <summary>
        /// 西欧字符 (DOS) 
        /// </summary>
        ibm850 = 850,

        /// <summary>
        /// 中欧字符 (DOS) 
        /// </summary>
        ibm852 = 852,
        /// <summary>
        /// OEM 西里尔语 
        /// </summary>
        IBM855 = 855,
        /// <summary>
        /// 土耳其字符 (DOS) 
        /// </summary>
        ibm857 = 857,
        /// <summary>
        /// OEM 多语言拉丁语 I 
        /// </summary>
        IBM00858 = 858,
        /// <summary>
        /// 葡萄牙语 (DOS) 
        /// </summary>
        IBM860 = 860,
        /// <summary>
        /// 冰岛语 (DOS) 
        /// </summary>
        ibm861 = 861,
        /// <summary>
        /// 希伯来字符 (DOS) 
        /// </summary>
        DOS_862 = 862,

        /// <summary>
        /// 加拿大法语 (DOS) 
        /// </summary>
        IBM863 = 863,
        /// <summary>
        /// 阿拉伯字符 (864) 
        /// </summary>
        IBM864 = 864,
        /// <summary>
        /// 北欧字符 (DOS) 
        /// </summary>
        IBM865 = 865,
        /// <summary>
        /// 西里尔字符 (DOS) 
        /// </summary>
        cp866 = 866,
        /// <summary>
        /// 现代希腊字符 (DOS) 
        /// </summary>
        ibm869 = 869,
        /// <summary>
        /// IBM EBCDIC（多语言拉丁语 2） 
        /// </summary>
        IBM870 = 870,
        /// <summary>
        /// 泰语 (Windows) 
        /// </summary>
        windows_874 = 874,
        /// <summary>
        /// IBM EBCDIC（现代希腊语） 
        /// </summary>
        cp875 = 875,
        /// <summary>
        /// 日语 (Shift-JIS)
        /// </summary>
        shift_jis = 932,
        /// <summary>
        /// 简体中文 (GB2312) 
        /// </summary>
        gb2312 = 936,
        /// <summary>
        /// 朝鲜语 
        /// </summary>
        ks_c_5601_1987 = 949,
        /// <summary>
        /// 繁体中文 (Big5) 
        /// </summary>
        big5 = 950,
        /// <summary>
        /// IBM EBCDIC（土耳其拉丁语 5） 
        /// </summary>
        IBM1026 = 1026,
        /// <summary>
        /// IBM 拉丁语 1 
        /// </summary>
        IBM01047 = 1047,
        /// <summary>
        /// IBM EBCDIC（美国 - 加拿大 - 欧洲） 
        /// </summary>
        IBM01140 = 1140,
        /// <summary>
        /// IBM EBCDIC（德国 - 欧洲） 
        /// </summary>
        IBM01141 = 1141,
        /// <summary>
        /// IBM EBCDIC（丹麦 - 挪威 - 欧洲） 
        /// </summary>
        IBM01142 = 1142,
        /// <summary>
        /// IBM EBCDIC（芬兰 - 瑞典 - 欧洲） 
        /// </summary>
        IBM01143 = 1143,
        /// <summary>
        /// IBM EBCDIC（意大利 - 欧洲） 
        /// </summary>
        IBM01144 = 1144,
        /// <summary>
        /// IBM EBCDIC（西班牙 - 欧洲） 
        /// </summary>
        IBM01145 = 1145,
        /// <summary>
        /// IBM EBCDIC（英国 - 欧洲） 
        /// </summary>
        IBM01146 = 1146,
        /// <summary>
        /// IBM EBCDIC（法国 - 欧洲） 
        /// </summary>
        IBM01147 = 1147,
        /// <summary>
        /// IBM EBCDIC（国际 - 欧洲） 
        /// </summary>
        IBM01148 = 1148,
        /// <summary>
        /// IBM EBCDIC（冰岛语 - 欧洲） 
        /// </summary>
        IBM01149 = 1149,
        /// <summary>
        /// Unicode 
        /// </summary>
        utf_16 = 1200,
        /// <summary>
        /// Unicode (Big-Endian) 
        /// </summary>
        UnicodeFFFE = 1201,
        /// <summary>
        /// 中欧字符 (Windows) 
        /// </summary>
        windows_1250 = 1250,
        /// <summary>
        /// 西里尔字符 (Windows)
        /// </summary>
        windows_1251 = 1251,
        /// <summary>
        /// 西欧字符 (Windows) 
        /// </summary>
        Windows_1252 = 1252,
        /// <summary>
        /// 希腊字符 (Windows) 
        /// </summary>
        windows_1253 = 1253,
        /// <summary>
        /// 土耳其字符 (Windows) 
        /// </summary>
        windows_1254 = 1254,
        /// <summary>
        /// 希伯来字符 (Windows) 
        /// </summary>
        windows_1255 = 1255,
        /// <summary>
        /// 阿拉伯字符 (Windows) 
        /// </summary>
        windows_1256 = 1256,
        /// <summary>
        /// 波罗的海字符 (Windows) 
        /// </summary>
        windows_1257 = 1257,
        /// <summary>
        /// 越南字符 (Windows) 
        /// </summary>
        windows_1258 = 1258,
        /// <summary>
        /// 朝鲜语 (Johab) 
        /// </summary>
        Johab = 1361,
        /// <summary>
        /// 西欧字符 (Mac) 
        /// </summary>
        macintosh = 10000,
        /// <summary>
        /// 日语 (Mac) 
        /// </summary>
        x_mac_japanese = 10001,
        /// <summary>
        /// 繁体中文 (Mac) 
        /// </summary>
        x_mac_chinesetrad = 10002,
        /// <summary>
        /// 朝鲜语 (Mac) 
        /// </summary>
        x_mac_korean = 10003,
        /// <summary>
        /// 阿拉伯字符 (Mac) 
        /// </summary>
        x_mac_arabic = 10004,
        /// <summary>
        /// 希伯来字符 (Mac) 
        /// </summary>
        x_mac_hebrew = 10005,
        /// <summary>
        /// 希腊字符 (Mac) 
        /// </summary>
        x_mac_greek = 10006,
        /// <summary>
        /// 西里尔字符 (Mac) 
        /// </summary>
        x_mac_cyrillic = 10007,
        /// <summary>
        /// 简体中文 (Mac) 
        /// </summary>
        x_mac_chinesesimp = 10008,
        /// <summary>
        /// 罗马尼亚语 (Mac) 
        /// </summary>
        x_mac_romanian = 10010,
        /// <summary>
        /// 乌克兰语 (Mac)
        /// </summary>
        x_mac_ukrainian = 10017,
        /// <summary>
        /// 泰语 (Mac) 
        /// </summary>
        x_mac_thai = 10021,
        /// <summary>
        /// 中欧字符 (Mac) 
        /// </summary>
        x_mac_ce = 10029,
        /// <summary>
        /// 冰岛语 (Mac) 
        /// </summary>
        x_mac_icelandic = 10079,
        /// <summary>
        /// 土耳其字符 (Mac) 
        /// </summary>
        x_mac_turkish = 10081,
        /// <summary>
        /// 克罗地亚语 (Mac) 
        /// </summary>
        x_mac_croatian = 10082,
        /// <summary>
        /// 繁体中文 (CNS) 
        /// </summary>
        x_Chinese_CNS = 20000,
        /// <summary>
        /// TCA 台湾 
        /// </summary>
        x_cp20001 = 20001,
        /// <summary>
        /// 繁体中文 (Eten) 
        /// </summary>
        x_Chinese_Eten = 20002,
        /// <summary>
        /// IBM5550 台湾 
        /// </summary>
        x_cp20003 = 20003,
        /// <summary>
        /// TeleText 台湾 
        /// </summary>
        x_cp20004 = 20004,
        /// <summary>
        /// Wang 台湾 
        /// </summary>
        x_cp20005 = 20005,
        /// <summary>
        /// 西欧字符 (IA5)
        /// </summary>
        x_IA5 = 20105,
        /// <summary>
        /// 德语 (IA5) 
        /// </summary>
        x_IA5_German = 20106,
        /// <summary>
        /// 瑞典语 (IA5) 
        /// </summary>
        x_IA5_Swedish = 20107,
        /// <summary>
        /// 挪威语 (IA5) 
        /// </summary>
        x_IA5_Norwegian = 20108,
        /// <summary>
        /// US-ASCII 
        /// </summary>
        us_ascii = 20127,
        /// <summary>
        /// T.61 
        /// </summary>
        x_cp20261 = 20261,
        /// <summary>
        /// ISO-6937 
        /// </summary>
        x_cp20269 = 20269,
        /// <summary>
        /// IBM EBCDIC（德国） 
        /// </summary>
        IBM273 = 20273,
        /// <summary>
        /// IBM EBCDIC（丹麦 - 挪威） 
        /// </summary>
        IBM277 = 20277,
        /// <summary>
        ///  IBM EBCDIC（芬兰 - 瑞典） 
        /// </summary>
        IBM278 = 20278,
        /// <summary>
        /// IBM EBCDIC（意大利） 
        /// </summary>
        IBM280 = 20280,
        /// <summary>
        /// IBM EBCDIC（西班牙） 
        /// </summary>
        IBM284 = 20284,
        /// <summary>
        /// IBM EBCDIC（英国） 
        /// </summary>
        IBM285 = 20285,
        /// <summary>
        /// IBM EBCDIC（日语片假名） 
        /// </summary>
        IBM290 = 20290,
        /// <summary>
        /// IBM EBCDIC（法国） 
        /// </summary>
        IBM297 = 20297,
        /// <summary>
        /// IBM EBCDIC（阿拉伯语） 
        /// </summary>
        IBM420 = 20420,
        /// <summary>
        ///  IBM EBCDIC（希腊语） 
        /// </summary>
        IBM423 = 20423,
        /// <summary>
        /// IBM EBCDIC（希伯来语） 
        /// </summary>
        IBM424 = 20424,
        /// <summary>
        /// IBM EBCDIC（朝鲜语扩展） 
        /// </summary>
        x_EBCDIC_KoreanExtended = 20833,
        /// <summary>
        /// IBM EBCDIC（泰语） 
        /// </summary>
        IBM_Thai = 20838,
        /// <summary>
        /// 西里尔字符 (KOI8-R) 
        /// </summary>
        koi8_r = 20866,
        /// <summary>
        /// IBM EBCDIC（冰岛语） 
        /// </summary>
        IBM871 = 20871,
        /// <summary>
        /// IBM EBCDIC（西里尔俄语） 
        /// </summary>
        IBM880 = 20880,
        /// <summary>
        /// IBM EBCDIC（土耳其语） 
        /// </summary>
        IBM905 = 20905,
        /// <summary>
        /// IBM 拉丁语 1 
        /// </summary>
        IBM00924 = 20924,
        /// <summary>
        /// 日语（JIS 0208-1990 和 0212-1990） 
        /// </summary>
        EUC_JP = 20932,
        /// <summary>
        /// 简体中文 (GB2312-80) 
        /// </summary>
        x_cp20936 = 20936,
        /// <summary>
        /// 朝鲜语 Wansung 
        /// </summary>
        x_cp20949 = 20949,
        /// <summary>
        /// IBM EBCDIC（西里尔塞尔维亚 - 保加利亚语） 
        /// </summary>
        cp1025 = 21025,
        /// <summary>
        /// 西里尔字符 (KOI8-U) 
        /// </summary>
        koi8_u = 21866,
        /// <summary>
        /// 西欧字符 (ISO) 
        /// </summary>
        iso_8859_1 = 28591,
        /// <summary>
        /// 中欧字符 (ISO) 
        /// </summary>
        iso_8859_2 = 28592,
        /// <summary>
        /// 拉丁语 3 (ISO) 
        /// </summary>
        iso_8859_3 = 28593,
        /// <summary>
        /// 波罗的海字符 (ISO) 
        /// </summary>
        iso_8859_4 = 28594,
        /// <summary>
        /// 西里尔字符 (ISO) 
        /// </summary>
        iso_8859_5 = 28595,
        /// <summary>
        /// 阿拉伯字符 (ISO) 
        /// </summary>
        iso_8859_6 = 28596,
        /// <summary>
        /// 希腊字符 (ISO) 
        /// </summary>
        iso_8859_7 = 28597,
        /// <summary>
        /// 希伯来字符 (ISO-Visual) 
        /// </summary>
        iso_8859_8 = 28598,
        /// <summary>
        /// 土耳其字符 (ISO) 
        /// </summary>
        iso_8859_9 = 28599,
        /// <summary>
        /// 爱沙尼亚语 (ISO) 
        /// </summary>
        iso_8859_13 = 28603,
        /// <summary>
        /// 拉丁语 9 (ISO) 
        /// </summary>
        iso_8859_15 = 28605,
        /// <summary>
        /// 欧罗巴 
        /// </summary>
        x_Europa = 29001,
        /// <summary>
        /// 希伯来字符 (ISO-Logical) 
        /// </summary>
        iso_8859_8_i = 38598,
        /// <summary>
        /// 日语 (JIS) 
        /// </summary>
        iso_jp_JIS = 50220,
        /// <summary>
        /// 日语（JIS- 允许 1 字节假名） 
        /// </summary>
        csISO2022JP = 50221,
        /// <summary>
        /// 日语（JIS- 允许 1 字节假名 - SO/SI） 
        /// </summary>
        iso_2022_jp = 50222,
        /// <summary>
        /// 朝鲜语 (ISO) 
        /// </summary>
        iso_2022_kr = 50225,
        /// <summary>
        /// 简体中文 (ISO-2022) 
        /// </summary>
        x_cp50227 = 50227,
        /// <summary>
        /// 日语 (EUC) 
        /// </summary>
        euc_jp = 51932,
        /// <summary>
        /// 简体中文 (EUC) 
        /// </summary>
        EUC_CN = 51936,
        /// <summary>
        /// 朝鲜语 (EUC) 
        /// </summary>
        euc_kr = 51949,
        /// <summary>
        /// 简体中文 (HZ) 
        /// </summary>
        hz_gb_2312 = 52936,
        /// <summary>
        /// 简体中文 (GB18030) 
        /// </summary>
        GB18030 = 54936,
        /// <summary>
        /// ISCII 梵文 
        /// </summary>
        x_iscii_de = 57002,
        /// <summary>
        /// ISCII 孟加拉语 
        /// </summary>
        x_iscii_be = 57003,
        /// <summary>
        /// ISCII 泰米尔语 
        /// </summary>
        x_iscii_ta = 57004,
        /// <summary>
        /// ISCII 泰卢固语 
        /// </summary>
        x_iscii_te = 57005,
        /// <summary>
        /// ISCII 阿萨姆语 
        /// </summary>
        x_iscii_as = 57006,
        /// <summary>
        /// ISCII 奥里雅语 
        /// </summary>
        x_iscii_or = 57007,
        /// <summary>
        /// ISCII 卡纳达语 
        /// </summary>
        x_iscii_ka = 57008,
        /// <summary>
        /// ISCII 马拉雅拉姆语 
        /// </summary>
        x_iscii_ma = 57009,
        /// <summary>
        /// ISCII 古吉拉特语 
        /// </summary>
        x_iscii_gu = 57010,
        /// <summary>
        /// ISCII 旁遮普语 
        /// </summary>
        x_iscii_pa = 57011,
        /// <summary>
        /// Unicode (UTF-7) 
        /// </summary>
        utf_7 = 65000,
        /// <summary>
        /// Unicode (UTF-8) 
        /// </summary>
        utf_8 = 65001,
        /// <summary>
        /// Unicode (UTF-32) 
        /// </summary>
        utf_32 = 65005,
        /// <summary>
        /// Unicode (UTF-32 Big-Endian) 
        /// </summary>
        utf_32BE = 65006
    }

    #endregion

}