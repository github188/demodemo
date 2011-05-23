<?php
/**
 * SAE邮件服务 
 *
 * @package sae 
 * @version $Id$
 * @author lijun
 */



/**
 * SAE邮件class , 目前只支持SMTP
 *
 * <code>
 * $mail = new SaeMail();
 * $mail->setAttach( array( 'my_photo' => '照片的二进制数据' ) );
 * $mail->quickSend( 'to@sina.cn' , '邮件标题' , '邮件内容' , 'smtpaccount@gmail.com' , 'password' );
 *
 * $mail->clean(); // 重用此对象
 * $mail->quickSend( 'to@sina.cn' , '邮件标题' , '邮件内容' , 'smtpaccount@unknown.com' , 'password' , 'smtp.unknown.com' , 25 ); // 指定smtp和端口
 *
 * </code>
 *
 * @package sae
 * @author lijun
 *
 */
class SaeMail extends SaeObject
{
    private static $_accesskey = "";    
    private static $_secretkey = "";
    private static $_errno=SAE_Success;
    private static $_errmsg="OK";
    private static $_count = 0;
    private static $_post = array();
    private static $_allowedAttachType = array("bmp","css","csv","gif","htm","html","jpeg","jpg","jpe","pdf","png","rss","text","txt","asc","diff","pot","tiff","tif","wbmp","ics","vcf");
    /**
     * @ignore
     */
    const DISPOSITION_ATTACH = 'A';
    /**
     * @ignore
     */
    const DISPOSITION_INLINE = 'I';
    private static $_disposition = array(  "bmp"=>self::DISPOSITION_INLINE, "css"=>self::DISPOSITION_ATTACH,
        "csv"=>self::DISPOSITION_ATTACH, "gif"=>self::DISPOSITION_INLINE,
        "htm"=>self::DISPOSITION_INLINE,"html"=>self::DISPOSITION_INLINE,
        "jpeg"=>self::DISPOSITION_INLINE,"jpg"=>self::DISPOSITION_INLINE,
        "jpe"=>self::DISPOSITION_INLINE, "pdf"=>self::DISPOSITION_ATTACH,
        "png"=>self::DISPOSITION_INLINE, "rss"=>self::DISPOSITION_INLINE,
        "text"=>self::DISPOSITION_ATTACH,"txt"=>self::DISPOSITION_ATTACH,
        "asc"=>self::DISPOSITION_ATTACH,"diff"=>self::DISPOSITION_ATTACH,
        "pot"=>self::DISPOSITION_ATTACH,"tiff"=>self::DISPOSITION_ATTACH,
        "tif"=>self::DISPOSITION_ATTACH,"wbmp"=>self::DISPOSITION_INLINE,
        "ics"=>self::DISPOSITION_INLINE,"vcf"=>self::DISPOSITION_INLINE,);
    private static $msp = array("sina.com"    => array("smtp.sina.com",25,0),
        "163.com"        => array("smtp.163.com",25,0),
        "263.com"        => array("smtp.263.com",25,0),
        "gmail.com"    => array("smtp.gmail.com",587,1),
        "sohu.com"    => array("smtp.sohu.com",25,0),
        "qq.com"        => array("smtp.qq.com",25,0),
        "vip.qq.com"    => array("smtp.qq.com",25,0),
        "126.com"        => array("smtp.126.com",25,0),
    );

    /**
     * @ignore
     */
    const baseurl = "http://mail.sae.sina.com.cn/index.php";
    /**
     * @ignore
     */
    const mail_limitsize = 1048576;
    /**
     * @ignore
     */
    const subject_limitsize = 256;

    /**
     * 构造对象，此处options选项的设置和函数setOpt相同
     *
     * @param array $options 邮件发送参数，详细参数请参考SaeMail::setOpt($options)
     */
    function __construct($options = array()) {
        self::$_accesskey = SAE_ACCESSKEY;
        self::$_secretkey = SAE_SECRETKEY;

        if (isset($options['subject']) && strlen($options['subject']) > self::subject_limitsize) {
            self::$_errmsg = "subject cannot larger than ".self::subject_limitsize." bytes";
            Throw new Exception(self::$_errmsg);
        }
        if(isset($options['content'])) self::$_count += strlen($options['content']);
        if(self::$_count > self::mail_limitsize) {
            self::$_errmsg = "mail size cannot larger than ".self::subject_limitsize." bytes";
            Throw new Exception(self::$_errmsg);
        }
    }

    /**
     * 设置发送参数,此处设置的参数只有使用send()方法发送才有效;quickSend()时将忽略此设置.
     * 
     *
     * @param array $options, 支持的Key如下:
     * <pre>
     *    -----------------------------------------
     *    <b>KEY        VALUE</b>
     *    -----------------------------------------
     *    from        string (only one)
     *    -----------------------------------------
     *    to        string (多个用;分开)
     *    -----------------------------------------
     *    cc        string (多个用;分开)
     *    -----------------------------------------
     *    smtp_host    string
     *    -----------------------------------------
     *    smtp_port    port,default 25
     *    -----------------------------------------
     *    smtp_username    string
     *    -----------------------------------------
     *    smtp_password    string
     *    -----------------------------------------
     *    subject        string,最大长度256字节
     *    -----------------------------------------
     *    content        text
     *    -----------------------------------------
     *    content_type    "TEXT"|"HTML",default TEXT
     *    -----------------------------------------
     *    charset        default utf8
     *    -----------------------------------------
     *    tls        default false
     *    -----------------------------------------
     * </pre>
     *
     * @return bool
     * @author Lijun
     */
    public function setOpt($options) {
        if (isset($options['subject']) && strlen($options['subject']) > self::subject_limitsize) {
            self::$_errno = SAE_ErrParameter;
            self::$_errmsg = "subject cannot larger than ".self::subject_limitsize." bytes";
            return false;
        }
        if(isset($options['content'])) self::$_count += strlen($options['content']);
        if(self::$_count > self::mail_limitsize) {
            self::$_errno = SAE_ErrParameter;
            self::$_errmsg = "mail size cannot larger than ".self::subject_limitsize." bytes";
            return false;
        }

        self::$_post = array_merge(self::$_post, $options);

        return true;
    }

    /**
     * 用于重用实例化对象时，将上一次的相关数据清零
     *
     * @return void
     * @author Lijun
     */
    public function clean() {
        self::$_post = array();
        self::$_count = 0;
        return true;
    }

    /**
     * 快速发送邮件
     *
     * 由于采用邮件队列发送,本函数返回成功时,只意味着邮件成功送到发送队列,并不等效于邮件已经成功发送
     *
     * @param string $to 要发送到的邮件地址 
     * @param string $subject 邮件标题
     * @param string $msgbody 邮件内容
     * @param string $smtp_user smtp用户名
     * @param string $smtp_pass smtp用户密码
     * @param string $smtp_host smtp服务host,使用sina,gmail,163,265,netease,qq,sohu,yahoo的smtp时可不填
     * @param string $smtp_port smtp服务端口,使用sina,gmail,163,265,netease,qq,sohu,yahoo的smtp时可不填
     * @param string $smtp_tls smtp服务是否开启tls(如gmail),使用sina,gmail,163,265,netease,qq,sohu,yahoo的smtp时可不填
     * @return bool
     * @author Lijun
     */
    function quickSend($to, $subject, $msgbody, $smtp_user, $smtp_pass, $smtp_host='', $smtp_port=25, $smtp_tls=false)
    {
        $to = trim($to);
        $subject = trim($subject);
        $msgbody = trim($msgbody);
        $smtp_user = trim($smtp_user);
        $smtp_host = trim($smtp_host);
        $smtp_port = intval($smtp_port);

        self::$_count += strlen($msgbody);
        if(strlen($subject) > self::subject_limitsize) {
            $_errno = SAE_ErrParameter;
            self::$_errmsg = "subject cannot larger than ".self::subject_limitsize." bytes";
            return false;
        }
        if(self::$_count > self::mail_limitsize) {
            self::$_errno = SAE_ErrParameter;
            self::$_errmsg = "mail size cannot larger than ".self::subject_limitsize." bytes";
            return false;
        }

        //if(preg_match('/([a-zA-Z0-9_-]+)@([a-zA-Z0-9-]+\.[a-zA-Z0-9-]+\.?[a-zA-Z0-9-]*)/', $smtp_user, $match)) {
        if (filter_var($smtp_user, FILTER_VALIDATE_EMAIL)) {
            preg_match('/([^@]+)@(.*)/', $smtp_user, $match);
            $user = $match[1]; $host = $match[2];
            if(empty($smtp_host)) {
                //print_r($match);
                if(isset(self::$msp[$host])) { $smtp_host = self::$msp[$host][0]; }
                else {
                    self::$_errno = SAE_ErrParameter;
                    self::$_errmsg = "you can set smtp_host explicitly or choose msp from sina,gmail,163,265,netease,qq,sohu,yahoo";
                    return false;
                }
            }
            if($smtp_port == 25 and isset(self::$msp[$host])) {
                $smtp_port = self::$msp[$host][1];
            }
            if(!$smtp_tls and isset(self::$msp[$host])) {
                $smtp_tls = self::$msp[$host][2];
            }
            $smtp_tls = ($smtp_tls == true);
            $username = $user;
        } else {
            self::$_errno = SAE_ErrParameter;
            self::$_errmsg = "invalid email address";
            return false;
        }
        self::$_post = array_merge(self::$_post, array("from"=>$smtp_user, "smtp_username"=>$username, "smtp_password"=>$smtp_pass, "smtp_host"=>$smtp_host, "smtp_port"=>$smtp_port, 'to'=>$to,'subject'=>$subject,'content'=>$msgbody, 'tls'=>$smtp_tls));

        return $this->send();
    }

    /**
     * 取得错误码
     *
     * @return int
     * @author Lijun
     */
    public function errno() {
        return self::$_errno;
    }

    /**
     * 取得错误信息
     *
     * @return string
     * @author Lijun
     */
    public function errmsg() {
        return self::$_errmsg;
    }

    /**
     * 设置key
     *
     * 只有使用其他应用的key时才需要调用
     *
     * @param string $accesskey 
     * @param string $secretkey 
     * @return void
     * @author Lijun
     */
    public function setAuth( $accesskey, $secretkey) {
        $accesskey = trim($accesskey);
        $secretkey = trim($secretkey);
        self::$_accesskey = $accesskey;
        self::$_secretkey = $secretkey;
        return true;
    }

    /**
     * 添加附件
     *
     * @param array $attach , key为文件名称,附件类型由文件名后缀决定,value为文件内容;文件内容支持二进制<br>
     * 支持的文件后缀:bmp,css,csv,gif,htm,html,jpeg,jpg,jpe,pdf,png,rss,text,txt,asc,diff,pot,tiff,tif,wbmp,ics,vcf
     * @return bool
     * @author Lijun
     */
    public function setAttach($attach) {
        if(!is_array($attach)) {
            self::$_errmsg = "attach parameter must be an array!";
            self::$_errno = SAE_ErrParameter;
            return false;
        }
        foreach($attach as $fn=>$blob) {
            $suffix = end(explode(".", $fn));
            if(!in_array($suffix, self::$_allowedAttachType)) {
                self::$_errno = SAE_ErrParameter;
                self::$_errmsg = "Invalid attachment type";
                return false;
            }
            self::$_count += strlen($blob);
            if(self::$_count > self::mail_limitsize) {
                self::$_errno = SAE_ErrForbidden;
                self::$_errmsg = "mail size cannot larger than ".self::mail_limitsize." bytes";
                return false;
            }
            self::$_post = array_merge(self::$_post, array("attach:$fn:B:".self::$_disposition[$suffix] => base64_encode($blob)));
            //print_r(strlen(base64_encode($blob)));
        }
        return true;
    }

    /**
     * 发送邮件
     *
     * @return bool
     * @author Lijun
     */
    public function send() {
        if(self::$_count > self::mail_limitsize) {
            self::$_errno = SAE_ErrForbidden;
            self::$_errmsg = "mail size cannot larger than ".self::mail_limitsize." bytes";
            return false;
        }
        //print_r(self::$_post);
        $tobepost = json_encode(self::$_post);
        return self::postData(array("saemail"=>$tobepost));
    }


    private static function postData($post) {
        $url = self::baseurl;
        $s = curl_init();
        curl_setopt($s,CURLOPT_URL,$url);
        curl_setopt($s,CURLOPT_HTTP_VERSION,CURL_HTTP_VERSION_1_0);
        curl_setopt($s,CURLOPT_TIMEOUT,5);
        curl_setopt($s,CURLOPT_RETURNTRANSFER,true);
        curl_setopt($s,CURLOPT_HEADER, 1);
        curl_setopt($s,CURLINFO_HEADER_OUT, true);
        curl_setopt($s,CURLOPT_HTTPHEADER, self::genReqestHeader($post));
        curl_setopt($s,CURLOPT_POST,true);
        curl_setopt($s,CURLOPT_POSTFIELDS,$post); 
        $ret = curl_exec($s);
        // exception handle, if error happens, set errno/errmsg, and return false
        $info = curl_getinfo($s);
        //print_r($info);
        //echo 'abab';
        //print_r($ret);
        //echo 'abab';
        if(empty($info['http_code'])) {
            self::$_errno = SAE_ErrInternal;
            self::$_errmsg = "mail service segment fault";
            return false;
        } else if($info['http_code'] != 200) {
            self::$_errno = SAE_ErrInternal;
            self::$_errmsg = "mail service internal error";
            return false;
        } else {
            if($info['size_download'] == 0) { // get MailError header
                $header = substr($ret, 0, $info['header_size']);
                $mailheader = self::extractCustomHeader("MailError", $header);
                if($mailheader == false) { // not found MailError header
                    self::$_errno = SAE_ErrUnknown;
                    self::$_errmsg = "unknown error";
                    return false;
                }
                $err = explode(",", $mailheader, 2);
                self::$_errno = $err[0];
                self::$_errmsg = $err[1];
                return false;
            } else {
                $body = substr($ret, -$info['size_download']);
                $body = json_decode(trim($body), true);
                self::$_errno = $body['errno'];
                self::$_errmsg = $body['errmsg'];
                if ($body['errno'] != 0) {
                    return false;
                }
            }
        }
        return true;
    }

    private static function genSignature($content, $secretkey) {
        $sig = base64_encode(hash_hmac('sha256',$content,$secretkey,true));
        return $sig;
    }

    private static function genReqestHeader($post) {
        $timestamp = date('Y-m-d H:i:s');
        $cont1 = "ACCESSKEY".self::$_accesskey."TIMESTAMP".$timestamp;
        $reqhead = array("TimeStamp: $timestamp","AccessKey: ".self::$_accesskey, "Signature: " . self::genSignature($cont1, self::$_secretkey));
        //print_r($reqhead);
        return $reqhead;
    }

    private static function extractCustomHeader($key, $header) {
        $pattern = '/'.$key.'(.*?)'."\n/";
        if (preg_match($pattern, $header, $result)) {
            return $result[1];
        } else {
            return false;
        }
    }

}