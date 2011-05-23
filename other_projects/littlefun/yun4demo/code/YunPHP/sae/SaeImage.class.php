<?php
/**
 * SAEͼ�������
 *
 * @author lijun
 * @version $Id$
 * @package sae
 *
 */

/**
 * SAEͼ����class
 * �ɶ�2M���´�С��ͼ����д���
 *
 * <code>
 * $f = new SaeFetchurl();
 * $img_data = $f->fetch( 'http://ss7.sinaimg.cn/bmiddle/488efcbbt7b5c4ae51ca6&690' );
 * $img = new SaeImage();
 * $img->setData( $img_data );
 * $img->resize(200); // �ȱ����ŵ�200��
 * $img->flipH(); // ˮƽ��ת
 * $img->flipv(); // ��ֱ��ת
 * $new_data = $img->exec(); // ִ�д������ش����Ķ���������
 * // ���߿���ֱ�����
 * $img->exec( 'jpg' , true );
 * </code>
 *
 * @package sae
 * @author  lijun
 * 
 */
class SaeImage extends SaeObject 
{
    private static $_accesskey = "";
    private static $_secretkey = "";
    private static $_errno=SAE_Success;
    private static $_errmsg="OK";
    private static $_img_data;
    private static $_height = 0;
    private static $_width = 0;
    private static $_post = array();
    private static $_format;

    /**
     * @ignore
     */
    const baseimgurl = "http://image.sae.sina.com.cn/index.php";
    /**
     * @ignore
     */
    const image_limitsize = 2097152;

    /**
     * ����SaeImage����
     *
     * @param mix $img_data img_data��������Ϊ������ͼƬ���ݣ�Ҳ����������compositeͼƬ�ϳɵ����飬�����ʽ:
     * <pre>
     *              array(array('blob1',x1,y1,opacity1,anchor1),
     *                array('blob2',x2,y2,opacity2,anchor2),
     *                array('blob3',x3,y3,opacity3,anchor3)
     *                      );
     * </pre>
     * ���ϲ���composite��ͼƬʱ��$img_data�����趨
     */
    function __construct($img_data="") {

        self::$_accesskey = SAE_ACCESSKEY;
        self::$_secretkey = SAE_SECRETKEY;    

        if(is_array($img_data)) {
            $_size = 0;
            foreach($img_data as $i) {
                if(count($i) != 5) {
                    self::$_errno = SAE_ErrParameter;
                    self::$_errmsg = "image data array you supplied invalid";
                    return false;
                }
                $_size += strlen($i[0]);
            }
            if($_size > self::image_limitsize) {
                self::$_errno = SAE_ErrParameter;
                self::$_errmsg = "image datas length more than 2M";
                return false;
            }
        } else if(strlen($img_data) > self::image_limitsize) { 
            self::$_errno = SAE_ErrParameter;
            self::$_errmsg = "image data length more than 2M";
            return false;
        }
        self::$_img_data = $img_data;

    }

    /**
     * ���ش�����Ϣ
     *
     * @return string
     * @author Lijun
     */
    public function errmsg() {
        return self::$_errmsg;
    }

    /**
     * ���ش�����
     *
     * @return int
     * @author Lijun
     */
    public function errno() {
        return self::$_errno;
    }

    /**
     * ȡ��ͼ������
     *
     * @return array ����ʱ����false
     * @author Lijun
     */ 
    public function getImageAttr() {
        if(self::imageNull()) return false;
        $fn = tempnam(SAE_TMP_PATH, "SAE_IMAGE");
        if ($fn == false) {
            self::$_errmsg = "tempnam call failed when getImageAttr";
            return false;
        }
        if(!file_put_contents($fn, self::$_img_data)) {
            self::$_errmsg = "file_put_contents to SAETMP_PATH failed when getImageAttr";
            return false;
        }
        if(!($size = getimagesize($fn, $info))) {
            self::$_errmsg = "getimagesize failed when getImageAttr";
            return false;
        }
        foreach($info as $k=>$v) {
            $size[$k] = $v;
        }
        self::$_width = $size[0];
        self::$_height = $size[1];
        return $size;
    }

    /**
     * ��������������³�ʼ��,���ڶ������һ��SaeImgae����
     *
     * <code>
     * $img = new SaeImage( $bin );
     * $img->resize(100);
     * $data1 = $img->exec();
     * $img->clean();
     * $img->setData( $bin2 );
     * $img->resize(300);
     * $data2 = $img->exec();
     * </code>
     *
     * @return void
     * @author Lijun
     */
    public function clean()
    {
        self::$_post = array();
        self::$_img_data = NULL;
    }

    /**
     * ����key.
     *
     * ֻ��ʹ������Ӧ�õ�keyʱ����Ҫ����
     *
     * @param string $accesskey 
     * @param string $secretkey 
     * @return bool
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
     * ����Ҫ�����ͼƬ���������ݻ����飬��ʽͬ���캯����img_data����
     *
     * @param string $img_data 
     * @return bool
     * @author Lijun
     */
    public function setData( $img_data ) {
        if(is_array($img_data)) {
            $_size = 0;
            foreach($img_data as $i) {
                if(count($i) != 5) {
                    self::$_errno = SAE_ErrParameter;
                    self::$_errmsg = "image data array you supported invalid";
                    return false;
                }
                $_size += strlen($i[0]);
            }
            if($_size > self::image_limitsize) {
                self::$_errno = SAE_ErrParameter;
                self::$_errmsg = "image datas length more than 2M";
                return false;
            }
        } else if(strlen($img_data) > self::image_limitsize) {
            self::$_errno = SAE_ErrParameter;
            self::$_errmsg = "image data length more than 2M";
            return false;
        }

        self::$_img_data = $img_data;
        return true;
    }

    /**
     * ����ͼƬ,ָֻ��width����heightʱ,�����еȱ�����
     *
     * @param int $width 
     * @param int $height 
     * @return bool
     * @author Lijun
     */
    public function resize($width=0, $height=0) {
        $width = intval($width);
        $height = intval($height);
        if(self::imageNull()) return false;
        array_push(self::$_post, array("act"=>"resize", "width"=>$width, "height"=>$height));
        return true;
    }

    /**
     * ����������.1Ϊԭ��С
     *
     * @param float $ratio 
     * @return bool
     * @author Lijun
     */
    public function resizeRatio($ratio=0.5) {
        $ratio = floatval($ratio);
        if(self::imageNull()) return false;
        if(self::$_width == 0) {
            $attr = $this->getImageAttr();
            if(! $attr) return false;
        }
        array_push(self::$_post, array("act"=>"resize", "width"=>self::$_width*$ratio, "height"=>self::$_height*$ratio));
        return true;
    }

    /**
     * ��ͼƬ���вü�
     *
     * @param float $lx x���(�ٷֱ�ģʽ,1Ϊԭͼ��С,��0.25) 
     * @param float $rx x�յ�(�ٷֱ�ģʽ,1Ϊԭͼ��С,��0.75) 
     * @param float $by y���(�ٷֱ�ģʽ,1Ϊԭͼ��С,��0.25) 
     * @param float $ty y�յ�(�ٷֱ�ģʽ,1Ϊԭͼ��С,��0.75) 
     * @return bool
     * @author Lijun
     */
    public function crop($lx=0.25, $rx=0.75, $by=0.25, $ty=0.75) {
        $lx = floatval($lx);
        $rx = floatval($rx);
        $by = floatval($by);
        $ty = floatval($ty);
        if(self::imageNull()) return false;
        array_push(self::$_post, array("act"=>"crop", "lx"=>$lx, "rx"=>$rx, "by"=>$by, "ty"=>$ty));
        return true;
    }

    /**
     * ��תͼƬ,����ֻ֧��90,180,270
     *
     * @param int $degree 
     * @return bool
     * @author Lijun
     */
    public function rotate($degree=90) {
        $degree = intval($degree);
        if(self::imageNull()) return false;
        array_push(self::$_post, array("act"=>"rotate", "degree"=>$degree));
        return true;
    }

    /**
     * ˮƽ��ת
     *
     * @return bool
     * @author Lijun
     */
    public function flipH() {
        if(self::imageNull()) return false;
        array_push(self::$_post, array("act"=>"flipH"));
        return true;
    }

    /**
     * ��ֱ��ת
     *
     * @return bool
     * @author Lijun
     */
    public function flipV() {
        if(self::imageNull()) return false;
        array_push(self::$_post, array("act"=>"flipV"));
        return true;
    }

    /**
     * �������ע�⣬����������ˮӡ
     *
     * @param string $txt ����Ϊutf8����
     * @param float $opacity ���ò�͸����
     * @param constant $gravity �������ְڷ�λ��, SAE_NorthWest,SAE_North,SAE_NorthEast,SAE_West,
     *                SAE_Center,SAE_East,SAE_SouthWest,SAE_South,SAE_SouthEast,SAE_Static
     * @param array $font �����������������������:
     * <pre>
     *    name,����,�������ƣ�֧�ֵ����壺SAE_SimSun(����,Ĭ��)��SAE_SimKai(����)...
     *    weight,������,int
     *    size�������С,int
     *    color,������ɫ,black(Ĭ��),red,blue...;
     *    raise,����Ч��0,1,2. 0�����ø���(Ĭ��)��1͹����2����
     *    stretch,����,��������: SAE_Normal(Ĭ��),SAE_UltraCondensed,SAE_ExtraCondensed,SAE_Condensed,
     *        SAE_SemiCondensed,SAE_SemiExpanded,SAE_Expanded,SAE_ExtraExpanded,SAE_UltraExpanded
     *    style,����,������SAE_Normal(Ĭ��),SAE_Italic,SAE_Oblique
     * </pre>
     *
     * @return bool
     * @author Lijun
     */
    public function annotate($txt, $opacity=0.5, $gravity=SAE_Static,
        $font = array("name"=>SAE_SimSun, "size"=>15, "weight"=>300,
        "stretch"=>SAE_Normal,"style"=>SAE_Normal, "raise"=>0, "color"=>"black")) {
            $opacity = floatval($opacity);
            if(self::imageNull()) return false;

            array_push(self::$_post, array("act"=>"annotate", "txt"=>$txt, "opacity"=>$opacity,
                "gravity"=>$gravity, "font"=>array("name"=>$font['name'],"size"=>$font["size"],
                "weight"=>$font["weight"],"color"=>$font["color"],"stretch"=>$font["stretch"],
                "style"=>$font["style"],"raise"=>$font["raise"])));

            return true;
        }

    /**
     * ȥ���,����ͼƬ������ͨ������exec֮ǰ
     *
     * @return bool
     * @author Lijun
     */
    public function improve() {
        if(self::imageNull()) return false;
        array_push(self::$_post, array("act"=>"improve"));
        return true;
    }

    /**
     * ����ͼƬ�������
     *
     * @param string $format ͼƬ��ʽ,֧��gif��jpg
     * @param bool $display �Ƿ�ֱ�������false:�������true:�����Ĭ��false
     * @return void
     * @author Lijun
     */
    public function exec($format="jpg", $display=false) {
        if(self::imageNull()) return false;
        if($format !== "jpg" and $format !== "gif") {
            self::$_errno = SAE_ErrParameter;
            self::$_errmsg = "format must be one of 'jpg' and 'gif'";
            return false;
        } else {
            self::$_format = $format;
        }
        if(self::$_post[0]["act"] == "composite" && is_array(self::$_img_data)) {
            foreach(self::$_img_data as $k=>$v) {
                self::$_img_data[$k][0] = base64_encode($v[0]); //unset($v[0]);
            }
            array_unshift(self::$_post, array("format"=>$format, "imagedata"=>self::$_img_data));
        } else {
            array_unshift(self::$_post, array("format"=>$format, "imagedata"=>base64_encode(self::$_img_data)));
        }

        if(self::$_post[1]["act"] == "composite" && !is_array(self::$_post[0]["imagedata"])) {
            self::$_errno = SAE_ErrParameter;
            self::$_errmsg = "composite imagedata must be an array, pls see doc:";
            return false;
        }
        if(self::$_post[1]["act"] != "composite" && is_array(self::$_post[0]["imagedata"])) {
            self::$_errno = SAE_ErrParameter;
            self::$_errmsg = "imagedata is array only when composite image and composite must be the first operation";
            return false;
        }

        foreach(self::$_post as $k=>$a) {
            if(isset($a["act"]) && $a["act"] == 'composite' && $k != 1) {
                self::$_errno = SAE_ErrParameter;
                self::$_errmsg = "composite operation must be the first operation!";
                return false;
            }
        }
        $tobepost = json_encode(self::$_post);
        $ret = self::postImgData(array("saeimg"=>$tobepost));
        if($ret && $display) {
            header("Content-Type: image/$format");
            echo $ret;
            return true;
        } else {
            return $ret;
        }
    }

    /**
     * ͼƬ�ϳɣ����Խ��ж���ͼƬ�ĺϳɣ�Ҳ������ͼƬˮӡ��
     * <pre>
     * ע��composite�����ں�����ͼƬ������һ��ʹ��ʱ��composite�����һ��������
     * ��ͼƬ�ϳ�ʱ����ʼ�����󴫵�imageData������Ҫ��һ����ά���顣
     * </pre>
     * �����е�ÿ����ԱҲ�����飬��Ҫ����5��Ԫ�أ�
     * - ��ʾͼ�����ݵ�blob�ַ��� (blob string)
     * - ��ʾ�ڻ����Ϸ���ͼ��ʱ���ê��λ�õ� x ƫ�Ƶ�������������Ϊ����
     * - ��ʾ�ڻ����Ϸ���ͼ��ʱ���ê��λ�õ� y ƫ�Ƶ�������������Ϊ����
     * - ��ʾͼ��͸����(opacity)�ĸ��������� 0.0 �� 1.0 ֮�䣨���� 0.0 �� 1.0��0��ʾȫ͸����1��ʾ�͸����
     * - ������ê���λ�ã�������֮һ��
     * <pre>
     *     SAE_TOP_LEFT SAE_TOP_CENTER SAE_TOP_RIGHT SAE_CENTER_LEFT SAE_CENTER_CENTER 
     *     SAE_CENTER_RIGHT SAE_BOTTOM_LEFT SAE_BOTTOM_CENTER SAE_BOTTOM_RIGHT
     * </pre>
     * ע�������xƫ�ƺ�yƫ�ƣ���ê��λ���йأ����ѡ����ê��TOP_RIGHT,<br>
     * ��x��y��ƫ����ָ��ͼƬ��TOP_RIGHT(���Ͻ�)����ڻ��������Ͻǵ�ƫ�ƣ�<br>
     * �����ѡ��BOTTOM_LEFT(���½�)Ϊê�㣬��x/yƫ�ƾ���ָ��ͼƬ�����½�<br>
     * ����ڻ��������½ǵ�ƫ������<br>
     * <b>ƫ�Ƶ�����ͬ��ѧ�е����޹涨��</b><br>
     * ͼƬ�ķ���˳��ͬ�����г��ֵ�˳��
     *
     * <code>
     * //��������ץȡҪ�ϳɵĶ���ͼƬ
     * $img1 = file_get_contents('http://ss2.sinaimg.cn/bmiddle/53b05ae9t73817f6bf751&690');
     * $img2 = file_get_contents('http://timg.sjs.sinajs.cn/miniblog2style/images/common/logo.png');
     * $img3 = file_get_contents('http://i1.sinaimg.cn/home/deco/2009/0330/logo_home.gif');
     * 
     * //ʵ����SaeImage��ȡ�����һ��ͼƬ�Ĵ�С���Ժ������趨�ϳɺ�ͼƬ�Ļ�����С
     * $img = new SaeImage( $img1 );
     * $size = $img->getImageAttr();
     * 
     * //���$img����
     * $img->clean();
     * 
     * //�趨Ҫ���ںϳɵ�����ͼƬ������ص������ں����ͼƬ���ס����ǰ���ͼƬ��
     * $img->setData( array(
     * array( $img1, 0, 0, 1, SAE_TOP_LEFT ),
     * array( $img2, 0, 0, 0.5, SAE_BOTTOM_RIGHT ),
     * array( $img3, 0, 0, 1, SAE_BOTTOM_LEFT ),
     * ) );
     * 
     * //ִ�кϳ�
     * $img->composite($size[0], $size[1]);
     * 
     * //���ͼƬ
     * $img->exec('jpg', true);
     * </code>
     * @param int $width ���û������
     * @param int $height ���û����߶�
     * @param string $color ���û�����ɫ
     * @return bool
     * @author Lijun
     */
    public function composite($width, $height, $color="black") {
        $width = intval($width);
        $height = intval($height);
        if(self::imageNull()) return false;
        array_push(self::$_post, array("act"=>"composite", "width"=>$width, "height"=>$height, "color"=>$color));
        return true;
    }

    private static function postImgData($post) {
        $url = self::baseimgurl;
        $s = curl_init();
        curl_setopt($s,CURLOPT_URL,$url);
        curl_setopt($s,CURLOPT_HTTP_VERSION,CURL_HTTP_VERSION_1_0);
        curl_setopt($s,CURLOPT_TIMEOUT,15);
        curl_setopt($s,CURLOPT_RETURNTRANSFER,true);
        curl_setopt($s,CURLOPT_HEADER, 1);
        curl_setopt($s,CURLINFO_HEADER_OUT, true);
        curl_setopt($s,CURLOPT_HTTPHEADER, self::genReqestHeader($post));
        curl_setopt($s,CURLOPT_POST,true);
        curl_setopt($s,CURLOPT_POSTFIELDS,$post); 
        $ret = curl_exec($s);
        // exception handle, if error happens, set errno/errmsg, and return false
        $info = curl_getinfo($s);

        if(empty($info['http_code'])) {
            self::$_errno = SAE_ErrInternal;
            self::$_errmsg = "image service segment fault";
            return false;
        } else if($info['http_code'] != 200) {
            self::$_errno = SAE_ErrInternal;
            self::$_errmsg = "image service internal error";
            return false;
        } else {
            if($info['size_download'] == 0) { // get ImageError header
                $header = substr($ret, 0, $info['header_size']);
                $imageheader = self::extractCustomHeader("ImageError", $header);
                if($imageheader == false) { // not found ImageError header
                    self::$_errno = SAE_ErrUnknown;
                    self::$_errmsg = "unknown error";
                    return false;
                }
                $err = explode(",", $imageheader, 2);
                self::$_errno = $err[0];
                self::$_errmsg = $err[1];
                return false;
            } else {
                $body = substr($ret, -$info['size_download']);
                return $body;
            }
        }
        return $ret;
    }

    private static function genSignature($content, $secretkey) {
        $sig = base64_encode(hash_hmac('sha256',$content,$secretkey,true));
        return $sig;
    }

    private static function genReqestHeader($post) {
        $timestamp = date('Y-m-d H:i:s');
        $cont1 = "ACCESSKEY".self::$_accesskey."TIMESTAMP".$timestamp;
        $reqhead = array("TimeStamp: $timestamp","AccessKey: ".self::$_accesskey, "Signature: " . self::genSignature($cont1, self::$_secretkey));
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

    private static function imageNull() {
        if(is_array(self::$_img_data) && count(self::$_img_data) == 0) {
            self::$_errno = SAE_ErrParameter;
            self::$_errmsg = "image data cannot be empty";
            return true;
        }
        if(!is_array(self::$_img_data) && self::$_img_data == "") {
            self::$_errno = SAE_ErrParameter;
            self::$_errmsg = "image data cannot be empty";
            return true;
        }
        return false;
    }
}