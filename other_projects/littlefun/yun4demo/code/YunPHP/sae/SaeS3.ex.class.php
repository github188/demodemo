<?php
/**
 * SAE S3存储服务
 *
 * @author Elmer Zhang
 * @version $Id$
 * @package sae
 *
 */

/**
 * SaeS3 class
 *
 * 使用S3存储服务需要手工加载saes3.ex.class.php
 *
 * <code>
 * require_once('saes3.ex.class.php');
 *
 * $s = new SaeS3();
 * $url = $s->write( 'domain' , 'test/test.txt' , 'the content!' );
 * // will return 'http://domain.appname.s3.sinaapp.com/test/test.txt'
 *
 * echo $s->getUrl( 'example' , 'thebook' );
 * // will echo 'http://exampale.stor.sae.sina.com.cn/thebook';
 *
 * echo file_get_contents("http://domain.appname.s3.sinaapp.com/test/test.txt");
 * // will echo 'the content!';
 *
 *</code>
 *
 * @package sae
 * @author  Elmer Zhang
 * 
 */

class SaeS3 extends SaeObject 
{
    /**
     * 用户accessKey
     * @var string
     * @ignore
     */
    public $accessKey = '';
    /**
     * 用户secretKey
     * @var string
     * @ignore
     */
    public $secretKey = '';
    /**
     * 运行过程中的错误信息
     * @var string
     * @ignore
     */
    public $errMsg = 'success';
    /**
     * 运行过程中的错误代码
     * @var int
     * @ignore
     */
    public $errNum = 0;
    /**
     * 应用名
     * @var string
     * @ignore
     */
    public $appName = '';
    /**
     * @var string
     * @ignore
     */
    public $restUrl = '';
    /**
     * @var string
     */
    private $filePath= '';
    /**
     * 运行过程中的错误信息
     * @var string
     */
    private $basedomain = 's3.sinaapp.com';
    /**
     * 该类所支持的所有方法
     * @var array
     * @ignore
     */
    protected $_optUrlList = array(
        "writefile"=>'?act=writefile&ak=_AK_&sk=_SK_&dom=_DOMAIN_&destfile=_DESTFILE_',
        "getdomfilelist"=>'?act=getdomfilelist&ak=_AK_&sk=_SK_&dom=_DOMAIN_&prefix=_PREFIX_&limit=_LIMIT_',
        "getfileattr"=>'?act=getfileattr&ak=_AK_&sk=_SK_&dom=_DOMAIN_&filename=_FILENAME_&attrkey=_ATTRKEY_',
        "getfilecontent"=>'?act=getfilecontent&ak=_AK_&sk=_SK_&dom=_DOMAIN_&filename=_FILENAME_',
        "delfile"=>'?act=delfile&ak=_AK_&sk=_SK_&dom=_DOMAIN_&filename=_FILENAME_',
        "getdomcapacity"=>'?act=getdomcapacity&ak=_AK_&sk=_SK_&dom=_DOMAIN_',
    );
    /**
     * 构造函数
     * $_accessKey与$_secretKey可以为空，为空的情况下可以认为是公开读文件
     * @param string $_accessKey
     * @param string $_secretKey
     * @return void
     * @author Elmer Zhang
     */
    public function __construct( $_accessKey='', $_secretKey='' )
    {
        if( $_accessKey== '' ) $_accessKey = SAE_ACCESSKEY;
        if( $_secretKey== '' ) $_secretKey = SAE_SECRETKEY;

        $this->setAuth( $_accessKey, $_secretKey );
    }

    /**
     * 设置key
     *
     * 当需要访问其他APP的数据时使用
     *
     * @param string $akey 
     * @param string $skey 
     * @return void
     * @author Elmer Zhang
     */
    public function setAuth( $akey , $skey )
    {
        $this->initOptUrlList( $this->_optUrlList);
        $this->init( $akey, $skey );
    }

    /**
     * 返回运行过程中的错误信息
     *
     * @return string
     * @author Elmer Zhang
     */
    public function errmsg()
    {
        $ret = $this->errMsg."url(".$this->filePath.")";
        $this->restUrl = '';
        $this->errMsg = 'success!';
        return $ret;
    }

    /**
     * 返回运行过程中的错误代码
     *
     * @return int
     * @author Elmer Zhang
     */
    public function errno()
    {
        $ret = $this->errNum;
        $this->errNum = 0;
        return $ret;
    }

    /**
     * 取得访问存储文件的url
     *
     * @param string $domain 
     * @param string $filename 
     * @return string
     * @author Elmer Zhang
     */
    public function getUrl( $domain, $filename ) {

        // make it full domain
        $domain = $this->getDom($domain);
        $filename = trim($filename);

        $this->filePath = "http://".$domain.'.'.$this->basedomain . "/$filename";
        return $this->filePath;
    }

    /**
     * @ignore
     */
    protected function setUrl( $domain , $filename )
    {
        $domain = $this->getDom($domain);
        $filename = trim($filename);

        $this->filePath = "http://".$domain.'.'.$this->basedomain . "/$filename";
    }

    /**
     * 将数据写入存储
     *
     * @param string $domain 存储域,在在线管理平台.storage页面可进行管理
     * @param string $destFile 文件名 
     * @param string $stringFileContent 文件内容,支持二进制数据 
     * @param string $size 写入长度,默认为不限制
     * @return void
     * @author Elmer Zhang
     */
    public function write( $domain, $destFile, $stringFileContent="", $size=-1 )
    {

        if ( Empty( $domain ) || Empty( $destFile ) || Empty( $stringFileContent ) )
        {
            $this->errMsg = 'the value of parameter (domain,destFile,srcFile) can not be empty!';
            $this->errNum = -101;
            return false;
        }

        // make it full domain
        $domain = $this->getDom($domain);
        $destFile = trim($destFile);

        if ( $size > -1 )
            $stringFileContent = substr( $stringFileContent, 0, $size );
        $this->setUrl( $domain, $destFile );

        $urlStr = $this->optUrlList['writefile'];
        $urlStr = str_replace( '_DOMAIN_', $domain , $urlStr );
        $urlStr = str_replace( '_DESTFILE_', $destFile, $urlStr );
        $ret = $this->parseRetData( $this->getJsonContentsAndDecode( $urlStr, $stringFileContent ) );
        if ( $ret !== false )
            return $this->filePath;
        else
            return false;
    }

    /**
     * 获取指定domain下的文件名列表
     *
     * @param string $domain 存储域,在在线管理平台.S3页面可进行管理
     * @param string $dir 目录，如 test, test/test2
     * @param string $limit 返回条数，默认1000条
     * @return array
     * @author Elmer Zhang
     */
    public function getList( $domain, $dir='', $limit=1000 )
    {
        //echo $dir;
        if ( Empty( $domain ) )
        {
            //echo "f=".__FILE__.",l=".__LINE__."<br>";
            $this->errMsg = 'the value of parameter (domain,filename) can not be empty!';
            $this->errNum = -101;
            return false;
        }

        // add dir
        $domain = $this->getDom($domain);
        $dir = trim($dir);

        $urlStr = $this->optUrlList['getdomfilelist'];

        $urlStr = str_replace( '_DOMAIN_', $domain, $urlStr );

        $urlStr = str_replace( '_PREFIX_', $dir, $urlStr );

        $urlStr = str_replace( '_LIMIT_', $limit, $urlStr );

        $ret = $this->parseRetData( $this->getJsonContentsAndDecode( $urlStr ) );
        $list = json_decode($ret);
        if (is_array($list)) {
            return $list;
        } else {
            return false;
        }
    }

    /**
     * 获取文件属性
     *
     * @param string $domain 
     * @param string $filename 
     * @param array $attrKey 属性值,如 array("fileName", "length")
     * @return array
     * @author Elmer Zhang
     */
    public function getAttr( $domain, $filename, $attrKey=array("fileName", "length") )
    {
        if ( Empty( $domain ) || Empty( $filename ) )
        {
            $this->errMsg = 'the value of parameter (domain,filename) can not be empty!';
            $this->errNum = -101;
            return false;
        }

        // make it full domain
        $domain = $this->getDom($domain);
        $filename = trim($filename);

        $this->setUrl( $domain, $filename );

        $urlStr = $this->optUrlList['getfileattr'];
        $urlStr = str_replace( '_DOMAIN_', $domain, $urlStr );
        $urlStr = str_replace( '_FILENAME_', $filename, $urlStr );
        $urlStr = str_replace( '_ATTRKEY_', json_encode( $attrKey ), $urlStr );
        //print_r( $urlStr );
        $ret = $this->parseRetData( $this->getJsonContentsAndDecode( $urlStr ) );
        $ret = json_decode($ret, true);
        if ( is_array( $ret ) )
            return $ret;
        else
            return false;
    }

    /**
     * 删除文件
     *
     * @param string $domain 
     * @param string $filename 
     * @return bool
     * @author Elmer Zhang
     */
    public function delete( $domain, $filename )
    {
        if ( Empty( $domain ) || Empty( $filename ) )
        {
            $this->errMsg = 'the value of parameter (domain,filename) can not be empty!';
            $this->errNum = -101;
            return false;
        }

        // make it full domain
        $domain = $this->getDom($domain);
        $filename = trim($filename);

        $this->setUrl( $domain, $filename );
        $urlStr = $this->optUrlList['delfile'];
        $urlStr = str_replace( '_DOMAIN_', $domain, $urlStr );
        $urlStr = str_replace( '_FILENAME_', $filename, $urlStr );
        $ret = $this->parseRetData( $this->getJsonContentsAndDecode( $urlStr ) );
        if ( $ret === false )
            return false;
        if ( $ret[ 'errno' ] == 0 )
            return true;
        else
            return false;
    }

    // =================================================================

    /**
     * @ignore
     */    
    protected function initOptUrlList( $_optUrlList=array() ) {
        $this->optUrlList = array();
        $this->optUrlList = $_optUrlList;


        while ( current( $this->optUrlList ) !== false ) {
            $this->optUrlList[ key( $this->optUrlList ) ] = SAE_S3HOST.current($this->optUrlList);
            next( $this->optUrlList );
        }

        reset( $this->optUrlList );

        //$this->init( $this->accessKey, $this->secretKey );

    }

    /**
     * 构造函数运行时替换所有$this->optUrlList值里的accessKey与secretKey 
     * @param string $_accessKey
     * @param string $_secretKey
     * @return void
     * @ignore
     */
    protected function init( $_accessKey, $_secretKey ) {
        $_accessKey = trim($_accessKey);
        $_secretKey = trim($_secretKey);

        $this->appName = $_SERVER[ 'HTTP_APPNAME' ];
        $this->accessKey = $_accessKey;
        $this->secretKey = $_secretKey;
        while ( current( $this->optUrlList ) !== false ) {
            $this->optUrlList[ key( $this->optUrlList ) ] = str_replace( '_AK_', $this->accessKey, current( $this->optUrlList ) );
            $this->optUrlList[ key( $this->optUrlList ) ] = str_replace( '_SK_', $this->secretKey, current( $this->optUrlList ) );
            next( $this->optUrlList );
        }

        reset( $this->optUrlList );
    }

    /**
     * 最终调用server端方法的rest函数封装
     * @ignore
     */
    protected function getJsonContentsAndDecode( $url, $stringFileContent='' ) //获取对应URL的JSON格式数据并解码
    {
        if( empty( $url ) )
            return false;
        $this->restUrl = $url;
        //echo $url ."\n";
        //file_put_contents('/data0/logs/s3url.log', $url."\n", FILE_APPEND);
        $ch=curl_init();

        curl_setopt( $ch, CURLOPT_URL, $url );
        curl_setopt( $ch, CURLOPT_HTTPGET, true );
        curl_setopt( $ch, CURLOPT_RETURNTRANSFER, true );


        if ( !Empty( $stringFileContent ) )
        {
            curl_setopt($ch, CURLOPT_POST, true );
            curl_setopt( $ch, CURLOPT_POSTFIELDS, array( 'fileContent'=>base64_encode($stringFileContent) ) );
        }


        curl_setopt( $ch, CURLOPT_USERAGENT, 'SAE Online Platform' );
        $content=curl_exec( $ch );
        curl_close($ch);
        if( false !== $content )
        {
            //print_r( $content );
            $tmp = json_decode( $content );
            if ( Empty( $tmp ) )//若非结构数据则直接抛出数据源
                return (array)$content;
            else
                return (array)$tmp;
        }
        else
            return array( 'errno'=>-102, 'errmsg'=>'bad request' );
    }

    /**
     * 解析并验证server端返回的数据结构
     * @ignore
     */
    public function parseRetData( $retData = array() )
    {
        //print_r( $retData );
        if ( !isset( $retData['errno'] ) || !isset( $retData['errmsg'] ) )
        {
            //file_put_contents('/data0/logs/s3ret.log', date('Y-m-d H:i:s') . "\n", FILE_APPEND);
            //file_put_contents('/data0/logs/s3ret.log', $this->restUrl . "\n", FILE_APPEND);
            //file_put_contents('/data0/logs/s3ret.log', json_encode($retData) ."\n\n", FILE_APPEND);
            //print_r( $retData );
            $this->errMsg = 'bad request';
            $this->errNum = -12;
            return false;
        }
        if ( $retData['errno'] !== 0 )
        {
            $this->errMsg = $retData[ 'errmsg' ];
            $this->errNum = $retData['errno'];
            return false;
        }
        if ( isset( $retData['data'] ) )
            return $retData['data'];
        return $retData;
    }

    /**
     * 获取domain所占存储的大小
     *
     * @param string $domain 
     * @return int
     * @author Elmer Zhang
     */
    public function getDomainCapacity( $domain='' )
    {
        if ( Empty( $domain ) )
        {
            $this->errMsg = 'the value of parameter \'domain\' can not be empty!';
            $this->errNum = -101;
            return false;
        }
        $domain = $this->getDom($domain);

        $urlStr = $this->optUrlList['getdomcapacity'];
        //print_r( $urlStr );
        $urlStr = str_replace( '_DOMAIN_', $domain, $urlStr );
        $ret = (array)$this->parseRetData( $this->getJsonContentsAndDecode( $urlStr ) );
        if ( $ret[ 'errno' ] == 0 )
            return $ret['data'];
        else
            return false;
    }

    /**
     * domain拼接
     * @param string $domain
     * @parma bool $concat
     * @return string
     * @author Elmer Zhang
     * @ignore
     */
    protected function getDom($domain, $concat = true) {
        $domain = trim($domain);

        if ($concat) {
            if( isset($_SERVER['HTTP_APPNAME']) && strpos($domain, '.') === false ) {
                $domain = $domain . '.' . $_SERVER['HTTP_APPNAME'];
            }
        } else {
            if ( ( $pos = strpos($domain, '.') ) !== false ) {
                $domain = substr($domain, 0, $pos);
            }
        }
        return $domain;
    }
}
