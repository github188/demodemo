<?php
/**
 * SAE数据存储服务
 *
 * @author quanjun
 * @version $Id$
 * @package sae
 *
 */

/**
 * SaeStorage class
 *
 * <code>
 * $s = new SaeStorage();
 * $s->write( 'example' , 'thebook' , 'bookcontent!' );
 * 
 * echo $s->read( 'example' , 'thebook') ; 
 * // will echo 'bookcontent!';
 *
 * echo $s->getUrl( 'example' , 'thebook' );
 * // will echo 'http://exampale.stor.sinaapp.com/thebook';
 *
 *</code>
 *
 * @package sae
 * @author  quanjun
 * 
 */

class SaeStorage extends SaeObject 
{
    /**
     * 用户accessKey
     * @var string
     */
    public $accessKey = '';
    /**
     * 用户secretKey
     * @var string
     */
    public $secretKey = '';
    /**
     * 运行过程中的错误信息
     * @var string
     */
    public $errMsg = 'success';
    /**
     * 运行过程中的错误代码
     * @var int
     */
    public $errNum = 0;
    /**
     * 应用名
     * @var string
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
    private $basedomain = 'stor.sinaapp.com';
    /**
     * 该类所支持的所有方法
     * @var array
     * @ignore
     */
    protected $optUrlList = array(
        "writefile"=>'?act=writefile&ak=_AK_&sk=_SK_&dom=_DOMAIN_&destfile=_DESTFILE_',
        "uploadfile"=>'?act=uploadfile&ak=_AK_&sk=_SK_&dom=_DOMAIN_&destfile=_DESTFILE_',
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
     * @author Quanjun
     */
    public function __construct( $_accessKey='', $_secretKey='' )
    {
        if( $_accessKey== '' ) $_accessKey = SAE_ACCESSKEY;
        if( $_secretKey== '' ) $_secretKey = SAE_SECRETKEY;

        $this->init( $_accessKey, $_secretKey );
    }

    /**
     * 设置key
     *
     * 当需要访问其他APP的数据时使用
     *
     * @param string $akey 
     * @param string $skey 
     * @return void
     * @author Quanjun
     */
    public function setAuth( $akey , $skey )
    {
        $this->init( $akey, $skey );
    }

    /**
     * 返回运行过程中的错误信息
     *
     * @return string
     * @author Quanjun
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
     * @author Quanjun
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
     * @author Quanjun
     */
    public function getUrl( $domain, $filename ) {

        // make it full domain
        $domain = trim($domain);
        $filename = trim($filename);
        if( isset($_SERVER['HTTP_APPNAME']) ) $domain = $_SERVER['HTTP_APPNAME'] .'-'. $domain;

        $this->filePath = "http://".$domain.'.'.$this->basedomain . "/$filename";
        return $this->filePath;
    }

    /**
     * @ignore
     */
    protected function setUrl( $domain , $filename )
    {
        $domain = trim($domain);
        $filename = trim($filename);

        $this->filePath = "http://".$domain.'.'.$this->basedomain . "/$filename";
    }

    /**
     * 将数据写入存储
     *
     * @param string $domain 存储域,在在线管理平台.storage页面可进行管理
     * @param string $destFile 文件名 
     * @param string $stringFileContent 文件内容,支持二进制数据 
     * @param int $size 写入长度,默认为不限制
     * @return string 写入成功时返回该文件的下载地址，否则返回false
     * @author Elmer Zhang
     */
    public function write( $domain, $destFile, $stringFileContent="", $size=-1 )
    {
        if ( Empty( $domain ) || Empty( $destFile ) || Empty( $stringFileContent ) )
        {
            $this->errMsg = 'the value of parameter (domain,destFile,stringFileContent) can not be empty!';
            $this->errNum = -101;
            return false;
        }

        if ( $size > -1 )
            $stringFileContent = substr( $stringFileContent, 0, $size );

        $srcFile = tempnam(SAE_TMP_PATH, 'SAE_STOR_UPLOAD');
        file_put_contents($srcFile, $stringFileContent);

        $re = $this->upload($domain, $destFile, $srcFile);
        unlink($srcFile);
        return $re;
    }

    /**
     * 将文件上传入存储
     *
     * @param string $domain 存储域,在在线管理平台.storage页面可进行管理
     * @param string $destFile 目标文件名
     * @param string $srcFile 源文件名
     * @return string 写入成功时返回该文件的下载地址，否则返回false
     * @author Elmer Zhang
     */
    public function upload( $domain, $destFile, $srcFile="" )
    {
        $domain = trim($domain);
        $destFile = trim($destFile);

        if ( Empty( $domain ) || Empty( $destFile ) || Empty( $srcFile ) )
        {
            $this->errMsg = 'the value of parameter (domain,destFile,srcFile) can not be empty!';
            $this->errNum = -101;
            return false;
        }

        // make it full domain
        if( isset($_SERVER['HTTP_APPNAME']) ) $domain = $_SERVER['HTTP_APPNAME'] .'-'. $domain;

        $this->setUrl( $domain, $destFile );

        $urlStr = $this->optUrlList['uploadfile'];
        $urlStr = str_replace( '_DOMAIN_', $domain , $urlStr );
        $urlStr = str_replace( '_DESTFILE_', $destFile, $urlStr );
        $ret = $this->parseRetData( $this->getJsonContentsAndDecode( $urlStr, $srcFile ) );
        if ( $ret !== false )
            return $this->filePath;
        else
            return false;
    }


    /**
     * 获取指定domain下的文件名列表
     *
     * @param string $domain 存储域,在在线管理平台.storage页面可进行管理
     * @param string $prefix 如 *,abc*,*.txt
     * @param int $limit 返回条数,最大100条,默认10条
     * @return array 执行成功时返回文件列表数组，否则返回false
     * @author Quanjun
     */
    public function getList( $domain, $prefix='*', $limit=10 )
    {
        $domain = trim($domain);

        //echo $prefix;
        if ( Empty( $domain ) )
        {
            //echo "f=".__FILE__.",l=".__LINE__."<br>";
            $this->errMsg = 'the value of parameter (domain,filename) can not be empty!';
            $this->errNum = -101;
            return false;
        }

        // add prefix
        if( isset($_SERVER['HTTP_APPNAME']) ) $domain = $_SERVER['HTTP_APPNAME'] .'-'. $domain;

        $urlStr = $this->optUrlList['getdomfilelist'];

        $urlStr = str_replace( '_DOMAIN_', $domain, $urlStr );

        $urlStr = str_replace( '_PREFIX_', $prefix, $urlStr );

        $urlStr = str_replace( '_LIMIT_', $limit, $urlStr );

        return $this->parseRetData( $this->getJsonContentsAndDecode( $urlStr ) );
    }

    /**
     * 获取文件属性
     *
     * @param string $domain 
     * @param string $filename 
     * @param array $attrKey 属性值,如 array("fileName", "length")
     * @return array
     * @author Quanjun
     */
    public function getAttr( $domain, $filename, $attrKey=array("fileName", "length") )
    {
        $domain = trim($domain);
        $filename = trim($filename);

        if ( Empty( $domain ) || Empty( $filename ) )
        {
            $this->errMsg = 'the value of parameter (domain,filename) can not be empty!';
            $this->errNum = -101;
            return false;
        }

        // make it full domain
        if( isset($_SERVER['HTTP_APPNAME']) ) $domain = $_SERVER['HTTP_APPNAME'] .'-'. $domain;

        $this->setUrl( $domain, $filename );

        $urlStr = $this->optUrlList['getfileattr'];
        $urlStr = str_replace( '_DOMAIN_', $domain, $urlStr );
        $urlStr = str_replace( '_FILENAME_', $filename, $urlStr );
        $urlStr = str_replace( '_ATTRKEY_', json_encode( $attrKey ), $urlStr );
        //print_r( $urlStr );
        $ret = $this->parseRetData( $this->getJsonContentsAndDecode( $urlStr ) );
        if ( is_object( $ret ) )
            return (array)$ret;
        else
            return $ret;
    }

    /**
     * 获取文件的内容
     *
     * @param string $domain 
     * @param string $filename 
     * @return mixxed 文件内容
     * @author Quanjun
     */
    public function read( $domain, $filename )
    {
        $domain = trim($domain);
        $filename = trim($filename);

        if ( Empty( $domain ) || Empty( $filename ) )
        {
            $this->errMsg = 'the value of parameter (domain,filename) can not be empty!';
            $this->errNum = -101;
            return false;
        }

        //echo $this->getUrl( $domain , $filename );

        // make it full domain
        if( isset($_SERVER['HTTP_APPNAME']) ) $domain = $_SERVER['HTTP_APPNAME'] .'-'. $domain;

        $this->setUrl( $domain, $filename );
        $urlStr = $this->optUrlList['getfilecontent'];
        $urlStr = str_replace( '_DOMAIN_', $domain, $urlStr );
        $urlStr = str_replace( '_FILENAME_', $filename, $urlStr );

        $ret =  $this->getJsonContentsAndDecode( $urlStr );
        if ( isset( $ret['errno'] ) )
        {
            $this->parseRetData( $ret );
            return false;            
        }
        if ( isset($ret[0]) && $ret[0] == 'HTTP/1.1 404' )
        {
            $this->parseRetData( array( 'errno'=>-18,'errmsg'=>'file is not exists!' ) );
            return false;
        }
        return $ret[0];
    }

    /**
     * 删除文件
     *
     * @param string $domain 
     * @param string $filename 
     * @return bool
     * @author Quanjun
     */
    public function delete( $domain, $filename )
    {
        $domain = trim($domain);
        $filename = trim($filename);

        if ( Empty( $domain ) || Empty( $filename ) )
        {
            $this->errMsg = 'the value of parameter (domain,filename) can not be empty!';
            $this->errNum = -101;
            return false;
        }

        // make it full domain
        if( isset($_SERVER['HTTP_APPNAME']) ) $domain = $_SERVER['HTTP_APPNAME'] .'-'. $domain;

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
    protected function initOptUrlList( $_optUrlList=array() )
    {
        $this->optUrlList = array();
        $this->optUrlList = $_optUrlList;


        $this->init( $this->accessKey, $this->secretKey );



    }
    /**
     * 构造函数运行时替换所有$this->optUrlList值里的accessKey与secretKey 
     * @param string $_accessKey
     * @param string $_secretKey
     * @return void
     * @ignore
     */ 
    protected function init( $_accessKey, $_secretKey )
    {
        $_accessKey = trim($_accessKey);
        $_secretKey = trim($_secretKey);

        $this->appName = $_SERVER[ 'HTTP_APPNAME' ];
        $this->accessKey = $_accessKey;
        $this->secretKey = $_secretKey;
        while ( current( $this->optUrlList ) !== false )
        {
            $this->optUrlList[ key( $this->optUrlList ) ] = str_replace( '_AK_', $this->accessKey, current( $this->optUrlList ) );
            $this->optUrlList[ key( $this->optUrlList ) ]= SAE_STOREHOST.str_replace( '_SK_', $this->secretKey, current( $this->optUrlList ) );
            //echo "l=".$this->optUrlList[ key( $this->optUrlList ) ] ."<br>";
            next( $this->optUrlList );
        }


        reset( $this->optUrlList );
    }

    /**
     * 最终调用server端方法的rest函数封装
     * @ignore
     */
    protected function getJsonContentsAndDecode( $url, $srcFile='' ) //获取对应URL的JSON格式数据并解码
    {
        if( empty( $url ) )
            return false;
        $this->restUrl = $url;
        // echo $url .'<hr/>';
        $ch=curl_init();

        curl_setopt( $ch, CURLOPT_URL, $url );
        curl_setopt( $ch, CURLOPT_HTTPGET, true );
        curl_setopt( $ch, CURLOPT_RETURNTRANSFER, true );


        if ( !Empty( $srcFile ) )
        {
            curl_setopt($ch, CURLOPT_POST, true );
            curl_setopt( $ch, CURLOPT_POSTFIELDS, array( 'srcFile'=>"@{$srcFile}" ) );
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
        //    print_r( $retData );
        if ( !isset( $retData['errno'] ) || !isset( $retData['errmsg'] ) )
        {
            //    print_r( $retData );
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
     * @author Quanjun
     * @ignore
     */
    public function getDomainCapacity( $domain='' )
    {
        $domain = trim($domain);

        if ( Empty( $domain ) )
        {
            $this->errMsg = 'the value of parameter \'$domain\' can not be empty!';
            $this->errNum = -101;
            return false;
        }

        // make it full domain
        if( isset($_SERVER['HTTP_APPNAME']) ) $domain = $_SERVER['HTTP_APPNAME'] .'-'. $domain;

        $urlStr = $this->optUrlList['getdomcapacity'];
        //print_r( $urlStr );
        $urlStr = str_replace( '_DOMAIN_', $domain, $urlStr );
        $ret = (array)$this->parseRetData( $this->getJsonContentsAndDecode( $urlStr ) );
        if ( $ret[ 'errno' ] == 0 )
            return $ret['data'];
        else
            return false;
    }
}