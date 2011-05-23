<?php
/**
 * SAE数据抓取服务
 *
 * @author  zhiyong
 * @version $Id$
 * @package sae
 *
 */

/**
 * SAE数据抓取class
 *
 * 在SAE下不能直接使用file_get_contents或者curl抓取外部数据.<br />
 * 而SaeFetchurl允许你抓取外部数据.支持的协议为http/https.
 *
 * 默认超时时间：
 *  - 连接超时： 5秒
 *  - 发送数据超时： 30秒
 *  - 接收数据超时： 40秒
 *
 * 抓取页面
 * <code>
 * $f = new SaeFetchurl();
 * $content = $f->fetch('http://sina.cn');
 * </code>
 *
 * 发起POST请求
 * <code>
 * $f = new SaeFetchurl();
 * $f->setMethod('post');
 * $f->setPostData( array('name'=> 'easychen' , 'email' => 'easychen@gmail.com' , 'file' => '文件的二进制内容') );
 * $f->fetch('http://photo.sinaapp.com/save.php');
 * </code>
 *
 * @author  zhiyong
 * @version $Id$
 * @package sae
 *
 */
class SaeFetchurl extends SaeObject
{
    function __construct( $akey = NULL , $skey = NULL )
    {
        if( $akey === NULL )
            $akey = SAE_ACCESSKEY;

        if( $skey === NULL )
            $skey = SAE_SECRETKEY;

        $this->impl_ = new FetchUrl($akey, $skey);
        $this->method_ = "get";
        $this->cookies_ = array();
        $this->opt_ = array();
        $this->headers_ = array();
    }

    /**
     * 设置acccesskey和secretkey
     *
     * 使用当前的应用的key时,不需要调用此方法
     *
     * @param string $akey
     * @param string $skey
     * @return void
     * @author zhiyong
     */
    public function setAuth( $akey , $skey )
    {
        $this->impl_->setAccesskey($akey);
        $this->impl_->setSecretkey($skey);
    }

    /**
     * @ignore
     */
    public function setAccesskey( $akey )
    {
        $this->impl_->setAccesskey($akey);
    }

    /**
     * @ignore
     */
    public function setSecretkey( $skey )
    {
        $this->impl_->setSecretkey($skey);
    }

    /**
     * 设置请求的方法(POST/GET/PUT... )
     *
     * @param string $method
     * @return void
     * @author zhiyong
     */
    public function setMethod( $method )
    {
        $this->method_ = trim($method);
    }

    /**
     * 设置POST方法的数据
     *
     * @param array $post_data , key为变量名称,value为变量值;value可以为二进制数据,如图片内容
     * @param bool $multipart , value是否为二进制数据,发送图片等大数据时必须为真.
     * @return bool
     * @author zhiyong
     */
    public function setPostData( $post_data , $multipart = false )
    {
        if ($this->method_ == "post")
        {
            if( is_array($post_data) && !$multipart )
            {
                $vec = array();
                foreach ($post_data as $k => $v)
                {
                    $v = urlencode($v);
                    array_push($vec, "$k=$v");
                }
                $this->opt_["post"] = join("&", $vec);
            }
            else
            {
                $this->opt_["post"] = $post_data;
            }

            return true;
        }
        return false;
    }

    /**
     * 在发起的请求中,添加请求头
     *
     * 不可以使用此方法设定的头：
     *  - Content-Length
     *  - Host
     *  - Referer
     *  - Vary
     *  - Via
     *  - X-Forwarded-For
     *  - FetchUrl
     *  - AccessKey
     *  - TimeStamp
     *  - Signature
     *  - AllowTruncated    //可使用setAllowTrunc方法来进行设定
     *  - ConnectTimeout    //可使用setConnectTimeout方法来进行设定
     *  - SendTimeout        //可使用setSendTimeout方法来进行设定
     *  - ReadTimeout        //可使用setReadTimeout方法来进行设定
     *
     *
     * @param string $name
     * @param string $value
     * @return bool
     * @author zhiyong
     */
    public function setHeader( $name , $value )
    {
        $name = trim($name);
        if (!in_array(strtolower($name), FetchUrl::$disabledHeaders)) {
            $this->headers_[$name] = $value;
            return true;
        } else {
            trigger_error("Disabled FetchUrl Header:" . $name, E_USER_NOTICE);
            return false;
        }
    }

    /**
     * 设置FetchUrl参数
     *
     * 参数列表：
     *  - truncated        布尔        是否截断
     *  - redirect            布尔        是否支持重定向
     *  - username            字符串        http认证用户名
     *  - password            字符串        http认证密码
     *  - useragent        字符串        自定义UA
     *
     * @param string $name
     * @param string $value
     * @return void
     * @author Elmer Zhang
     * @ignore
     */
    public function setOpt( $name , $value )
    {
        $name = trim($name);
        $this->opt_[$name] = $value;
    }

    /**
     * 在发起的请求中,添加cookie数据,此函数可多次调用,添加多个cookie
     *
     * @param string $name
     * @param string $value
     * @return void
     * @author zhiyong
     */
    public function setCookie( $name , $value )
    {
        $name = trim($name);
        array_push($this->cookies_, "$name=$value");
    }

    /**
     * 是否允许截断
     *
     * 如果设置为true,当发送数据超过允许大小时,自动截取符合大小的部分;<br />
     * 如果设置为false,当发送数据超过允许大小时,直接返回false;
     *
     * @param bool $allow
     * @return void
     * @author zhiyong
     */
    public function setAllowTrunc($allow) {
        $this->opt_["truncated"] = $allow;
    }

    /**
     * 设置连接超时时间,此时间必须小于SAE系统设置的时间,否则以SAE系统设置为准
     *
     * @param int $ms 毫秒
     * @return void
     * @author zhiyong
     */
    public function setConnectTimeout($ms) {
        $this->opt_["connecttimeout"] = $ms;
    }

    /**
     * 设置发送超时时间,此时间必须小于SAE系统设置的时间,否则以SAE系统设置为准
     *
     * @param int $ms 毫秒
     * @return void
     * @author zhiyong
     */
    public function setSendTimeout($ms) {
        $this->opt_["sendtimeout"] = $ms;
    }

    /**
     * 设置读取超时时间,此时间必须小于SAE系统设置的时间,否则以SAE系统设置为准
     *
     * @param int $ms 毫秒
     * @return void
     * @author zhiyong
     */
    public function setReadTimeout($ms) {
        $this->opt_["ReadTimeout"] = $ms;
    }

    /**
     * 当请求页面是转向页时,是否允许跳转,SAE最大支持5次跳转
     *
     * @param bool $allow
     * @return void
     * @author zhiyong
     */
    public function setAllowRedirect($allow) {
        $this->opt_["redirect"] = $allow;
    }

    /**
     * 设置HTTP认证用户名密码
     *
     * @param string $username HTTP认证用户名
     * @param string $password HTTP认证密码
     * @return void
     * @author zhiyong
     */
    public function setHttpAuth($username, $password) {
        $this->opt_["username"] = $username;
        $this->opt_["password"] = $password;
    }

    /**
     * 发起请求
     *
     * @param string $url
     * @param array $opt 请求参数
     * @return mixed 成功时读取到的内容，否则返回false
     * @author zhiyong
     */
    public function fetch( $url, $opt = NULL )
    {
        if (count($this->cookies_) != 0) {
            $this->opt_["cookie"] = join("; ", $this->cookies_);
        }
        $opt = ($opt) ?  array_merge($this->opt_, $opt) : $this->opt_;
        return $this->impl_->fetch($url, $opt, $this->headers_);
    }

    /**
     * 返回数据的header信息
     *
     * @param bool $parse 是否解析header，默认为true。
     * @return array
     * @author zhiyong
     */
    public function responseHeaders($parse = true)
    {
        $items = explode("\r\n", $this->impl_->headerContent());
        if (!$parse) {
            return $items;
        }
        array_shift($items);
        $headers = array();
        foreach ($items as $_) {
            $item = explode(":", $_);
            $key = trim($item[0]);
            if ($key == "Set-Cookie") {
                if (array_key_exists($key, $headers)) {
                    array_push($headers[$key], trim($item[1]));
                } else {
                    $headers[$key] = array(trim($item[1]));
                }
            } else {
                $headers[$key] = trim($item[1]);
            }
        }
        return $headers;
    }

    /**
     * 返回HTTP状态码
     *
     * @return int
     * @author Elmer Zhang
     */
    public function httpCode() {
        return $this->impl_->httpCode();
    }


    /**
     * 返回头里边的cookie信息
     *
     * @return array
     * @author zhiyong
     */
    public function responseCookies()
    {
        $header = $this->impl_->headerContent();
        $matchs = array();
        $cookies = array();
        if (preg_match_all('/Set-Cookie:\s([^\r\n]+)/', $header, $matchs)) {
            foreach ($matchs[1] as $match) {
                $cookie = array();
                $items = explode(";", $match);
                foreach ($items as $_) {
                    $item = explode("=", trim($_));
                    $cookie[$item[0]]= $item[1];
                }
                array_push($cookies, $cookie);
            }
        }
        return $cookies;
    }

    /**
     * 返回错误码
     *
     * @return int
     * @author zhiyong
     */
    public function errno()
    {
        if ($this->impl_->errno() != 0) {
            return $this->impl_->errno();
        } else {
            if ($this->impl_->httpCode() != 200) {
                return $this->impl_->httpCode();
            }
        }
        return 0;
    }

    /**
     * 返回错误信息
     *
     * @return string
     * @author zhiyong
     */
    public function errmsg()
    {
        if ($this->impl_->errno() != 0) {
            return $this->impl_->error();
        } else {
            if ($this->impl_->httpCode() != 200) {
                return $this->impl_->httpDesc();
            }
        }
        return "";
    }

    private $impl_;
    private $opt_;
    private $headers_;

}


/**
 * FetchUrl , the sub class of SaeFetchurl
 *
 *
 * @package sae
 * @subpackage fetchurl
 * @author  zhiyong
 * @ignore
 */
class FetchUrl {
    const end_         = "http://fetchurl.sae.sina.com.cn/";
    const maxRedirect_ = 5;
    public static $disabledHeaders = array(
        'content-length',
        'host',
        'referer',
        'vary',
        'via',
        'x-forwarded-for',
        'fetchurl',
        'accesskey',
        'timestamp',
        'signature',
        'allowtruncated',
        'connecttimeout',
        'sendtimeout',
        'readtimeout',
    );

    public function __construct($accesskey, $secretkey) {
        $accesskey = trim($accesskey);
        $secretkey = trim($secretkey);

        $this->accesskey_ = $accesskey;
        $this->secretkey_ = $secretkey;

        $this->contents_ = array(null, null);

        $this->errno_ = 0;
        $this->error_ = null;
        $this->debug_ = false;
    }

    public function __destruct() {
        // do nothing
    }

    public function setAccesskey($accesskey) {
        $accesskey = trim($accesskey);
        $this->accesskey_ = $accesskey;
    }

    public function setSecretkey($secretkey) {
        $secretkey = trim($secretkey);
        $this->secretkey_ = $secretkey;
    }

    public function setDebugOn() {
        $this->debug_ = true;
    }

    public function setDebugOff() {
        $this->debug_ = false;
    }

    public function fetch($url, $opt = null, $headers) {

        $maxRedirect = 1;
        if (is_array($opt) && array_key_exists('redirect',$opt) && $opt['redirect']) {
            $maxRedirect = FetchUrl::maxRedirect_;
        }
        for ($i = 0; $i < $maxRedirect; ++$i) {
            $this->dofetch($url, $opt, $headers);
            if ($this->errno_ == 0) {
                if ($this->httpCode_ == 301 || $this->httpCode_ == 302) {
                    $matchs = array();
                    if (preg_match('/Location:\s([^\r\n]+)/', $this->contents_[0], $matchs)) {
                        $newUrl = $matchs[1];
                        if ($newUrl[0] == '/') {
                            $url = preg_replace('/^((?:https?:\/\/)?[^\/]+)\/(.*)$/', '$1', $url) . $newUrl;
                        } else {
                            $url = $newUrl;
                        }

                        if ($this->debug_) {
                            echo "[debug] redirect to $url\n";
                        }
                        continue;
                    }
                }
            }
            break;
        }

        if ($this->errno_ == 0 && $this->httpCode_ == 200) {
            return $this->contents_[1];
        } else {
            return false;
        }
    }

    public function headerContent() {
        return $this->contents_[0];
    }

    public function errno() {
        return $this->errno_;
    }

    public function error() {
        return $this->error_;
    }

    public function httpCode() {
        return $this->httpCode_;
    }

    public function httpDesc() {
        return $this->httpDesc_;
    }

    private function signature($url, $timestamp) {
        $content = "FetchUrl"  . $url .
            "TimeStamp" . $timestamp .
            "AccessKey" . $this->accesskey_;
        $signature = (base64_encode(hash_hmac('sha256',$content,$this->secretkey_,true)));
        if ($this->debug_) {
            echo "[debug] content: $content" . "\n";
            echo "[debug] signature: $signature" . "\n";
        }
        return $signature;
    }

    private function dofetch($url, $opt, $headers_) {

        $timestamp = date("Y-m-d H:i:s");
        $signature = $this->signature($url, $timestamp);
        $headers = array("FetchUrl: $url",
            "AccessKey: $this->accesskey_",
            "TimeStamp: $timestamp",
            "Signature: $signature"
        );

        $ch = curl_init();
        curl_setopt($ch, CURLOPT_HEADER, true);
        curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
        if ($this->debug_) {
            curl_setopt($ch, CURLOPT_VERBOSE, true);
        }

        if (is_array($opt) && !empty($opt)) {
            foreach( $opt as $k => $v) {
                switch(strtolower($k)) {
                case 'username':
                    if (array_key_exists("password",$opt)) {
                        curl_setopt($ch, CURLOPT_USERPWD, $v . ":" . $opt["password"]);
                    }
                    break;
                case 'password':
                    if (array_key_exists("username",$opt)) {
                        curl_setopt($ch, CURLOPT_USERPWD, $opt["username"] . ":" . $v);
                    }
                    break;
                case 'useragent':
                    curl_setopt($ch, CURLOPT_USERAGENT, $v);
                    break;
                case 'post':
                    curl_setopt($ch, CURLOPT_POST, true);
                    curl_setopt($ch, CURLOPT_POSTFIELDS, $v);
                    break;
                case 'cookie':
                    curl_setopt($ch, CURLOPT_COOKIESESSION, true);
                    curl_setopt($ch, CURLOPT_COOKIE, $v);
                    break;
                case 'truncated':
                    array_push($headers, "AllowTruncated:" . $v);
                    break;
                case 'connecttimeout':
                    array_push($headers, "ConnectTimeout:" . intval($v));
                    break;
                case 'sendtimeout':
                    array_push($headers, "SendTimeout:" . intval($v));
                    break;
                case 'readtimeout':
                    array_push($headers, "ReadTimeout:" . intval($v));
                    break;
                default:
                    break;

                }
            }
        }

        if (is_array($headers_) && !empty($headers_)) {
            foreach($headers_ as $k => $v) {
                if (!in_array(strtolower($k), FetchUrl::$disabledHeaders)) {
                    array_push($headers, "{$k}:" . $v);
                }
            }
        }

        curl_setopt($ch, CURLOPT_HTTPHEADER, $headers);
        curl_setopt($ch, CURLOPT_URL, FetchUrl::end_);

        $contents =  curl_exec($ch);
        $this->errno_ = curl_errno($ch);
        $this->error_ = curl_error($ch);

        if ($this->errno_ == 0) {
            $this->contents_ = explode("\r\n\r\n", $contents, 2);
            if ($this->debug_) {
                echo "[debug] header => " . $this->contents_[0] . "\n";
                echo "[debug] body   => " . $this->contents_[1] . "\n";
            }
            $matchs = array();
            if (preg_match('/^(?:[^\s]+)\s([^\s]+)\s([^\r\n]+)/', $this->contents_[0], $matchs)) {
                $this->httpCode_ = $matchs[1];
                $this->httpDesc_ = $matchs[2];
                if ($this->debug_) {
                    echo "[debug] httpCode = " . $this->httpCode_ . "  httpDesc = " . $this->httpDesc_ . "\n";
                }
            } else {
                $this->errno_ = -1;
                $this->error_ = "invalid response";
            }
        }
        curl_close($ch);
    }

    private $accesskey_;
    private $secretkey_;

    private $errno_;
    private $error_;

    private $httpCode_;
    private $httpDesc_;
    private $contents_;

    private $debug_;

}