<?php
/**
*
*  Copyright:  (c) 2004 - 2008 [AOKOSOFT] Read8
*  Web Site:   http://www.aokosoft.com http://www.read8.net
*  Powered by: [AOKOSOFT] Read8 Group
*  Support:    http://bbs.read8.net
*  License:    http://www.read8.net/license
*
*/
//¹ºÎï³µÀà

class Cart{

	var $arrCart = array();

	function Cart(){
		if(isset($_SESSION['SS_Cart']) && is_array($_SESSION['SS_Cart'])) $this->arrCart = $_SESSION['SS_Cart'];
	}

	function addToCart($buyID,$chapter = '',$price = ''){
		if(!array_key_exists($buyID,$this->arrCart)) {
			$nowArr = array($buyID => array('buyID' => $buyID,'chapter' => $chapter,'price' => $price,'time' => time()));
			$this->arrCart = $this->arrCart + $nowArr;
			$this->saveChange();
			return $this->arrCart;
		}
	}

	function rmFromCart($buyID){
		if(array_key_exists($buyID,$this->arrCart)) {
			$nowArr = array();
			foreach($this->arrCart as $key => $value)
				if($key != $buyID)
					$nowArr += array($key => $value);
			$this->arrCart = $nowArr;
			$this->saveChange();
			return $this->arrCart;
		}
	}

	function getCart($buyID = -1){
		$nowArr = $this->arrCart;
		if($buyID != -1) $nowArr = $nowArr[$buyID];
		if(is_array($nowArr) && count($nowArr)) return $nowArr;
		return array();
	}

	function reset(){
		unset($arrCart);
		unset($_SESSION['SS_Cart']);
	}

	function saveChange(){
		$_SESSION['SS_Cart'] = $this->arrCart;
	}

}
