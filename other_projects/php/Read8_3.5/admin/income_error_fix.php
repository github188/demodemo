<?php 
if(!defined('IN_Read8') || !defined('IN_Read8_ACP') || !isset($PHP_SELF)) {
        exit('Access Denied (category.inc.php)');
}

$pid = $_GET['id'];
if($pid && $_GET['action'] == 'ignore') {
        $db->single(__TAB_PAY__);
        $db->update(array(
                'state'  => 3,
        ), array(
                'id'      => $_GET['id'],
        ));
}else if($pid && $_GET['action'] == 'ok'){
    #echo "------------";
    $db->single(__TAB_PAY__);
    $trade = $db->selectOne(array(
            'field' => 'uid, price, vmoney, dateline',
            'where' => array(
                    'id'     => $pid,
                    'active' => 1,
            ),
    ));
    $db->update(array(
            'state'   => 2,
            'suctime' => TIMESTAMP,
    ), array(
            'id'      => $pid,
            'active'  => 1,
    ));
    $db->execUpBySql('UPDATE '.__TAB_USER__.' SET vmoney = vmoney + '.$trade['vmoney'].' WHERE uid = '.$trade['uid']);    
}
        

#        jumpage("?ptype=$ptype&pfile=$pfile", '删除成功');

$tpl = new AOKOTpl('income_error_fix');
$tpl->assign('records', $db->select(array(
        'from'  => __TAB_PAY__,
        'where' => array(
                'state' => 1,
        ),
)));
$tpl->output();


?>
