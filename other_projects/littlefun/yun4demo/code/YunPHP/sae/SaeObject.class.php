<?php

/**
 * Sae����
 * 
 * STDLib������class��Ӧ�ü̳б�class,��ʵ��SaeInterface�ӿ�  
 *
 * @author Easychen <easychen@gmail.com>
 * @version $Id$
 * @package sae
 */

/**
 * SaeObject
 *
 * @package sae
 */
abstract class SaeObject implements SaeInterface
{
    function __construct()
    {
        // 
    }
    
}

/**
 * SaeInterface , public interface of all sae client apis
 *
 * all sae client classes must implement these method for setting accesskey and secretkey , getting error infomation.
 * @package sae
 **/
interface SaeInterface
{
    public function errmsg();
    public function errno();
    public function setAuth( $akey , $skey );
}



?>