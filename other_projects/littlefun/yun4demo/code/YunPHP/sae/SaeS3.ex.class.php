<?php
/**
 * SAE S3�洢����
 *
 * @author Elmer Zhang
 * @version $Id$
 * @package sae
 *
 */

/**
 * SaeS3 class
 *
 * ʹ��S3�洢������Ҫ�ֹ�����saes3.ex.class.php
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
     * �û�accessKey
     * @var string
     * @ignore
     */
    public $accessKey = '';
    /**
     * �û�secretKey
     * @var string
     * @ignore
     */
    public $secretKey = '';
    /**
     * ���й����еĴ�����Ϣ
     * @var string
     * @ignore
     */
    public $errMsg = 'success';
    /**
     * ���й����еĴ������
     * @var int
     * @ignore
     */
    public $errNum = 0;
    /**
     * Ӧ����
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
     * ���й����еĴ�����Ϣ
     * @var string
     */
    private $basedomain = 's3.sinaapp.com';
    /**
     * ������֧�ֵ����з���
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
     * ���캯��
     * $_accessKey��$_secretKey����Ϊ�գ�Ϊ�յ�����¿�����Ϊ�ǹ������ļ�
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
     * ����key
     *
     * ����Ҫ��������APP������ʱʹ��
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
     * �������й����еĴ�����Ϣ
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
     * �������й����еĴ������
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
     * ȡ�÷��ʴ洢�ļ���url
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
     * ������д��洢
     *
     * @param string $domain �洢��,�����߹���ƽ̨.storageҳ��ɽ��й���
     * @param string $destFile �ļ��� 
     * @param string $stringFileContent �ļ�����,֧�ֶ��������� 
     * @param string $size д�볤��,Ĭ��Ϊ������
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
     * ��ȡָ��domain�µ��ļ����б�
     *
     * @param string $domain �洢��,�����߹���ƽ̨.S3ҳ��ɽ��й���
     * @param string $dir Ŀ¼���� test, test/test2
     * @param string $limit ����������Ĭ��1000��
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
     * ��ȡ�ļ�����
     *
     * @param string $domain 
     * @param string $filename 
     * @param array $attrKey ����ֵ,�� array("fileName", "length")
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
     * ɾ���ļ�
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
     * ���캯������ʱ�滻����$this->optUrlListֵ���accessKey��secretKey 
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
     * ���յ���server�˷�����rest������װ
     * @ignore
     */
    protected function getJsonContentsAndDecode( $url, $stringFileContent='' ) //��ȡ��ӦURL��JSON��ʽ���ݲ�����
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
            if ( Empty( $tmp ) )//���ǽṹ������ֱ���׳�����Դ
                return (array)$content;
            else
                return (array)$tmp;
        }
        else
            return array( 'errno'=>-102, 'errmsg'=>'bad request' );
    }

    /**
     * ��������֤server�˷��ص����ݽṹ
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
     * ��ȡdomain��ռ�洢�Ĵ�С
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
     * domainƴ��
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
