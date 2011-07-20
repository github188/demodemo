<?php 
if(!defined('IN_Read8') || !defined('IN_Read8_ACP') || !isset($PHP_SELF)) {
        exit('Access Denied (category.inc.php)');
}

$tpl = new AOKOTpl('income_error_fix');
$tpl->assign('records', $db->select(array(
        'from'  => __TAB_PAY__,
        'where' => array(
                'state' => 1,
        ),
)));
$tpl->output();


?>
