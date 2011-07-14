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
if(!defined('IN_Read8')) exit('Access Denied');


/*
	使用实例:
	$p = new Page(2000, 20, 1, 'page');	//建立对象 & 设置基本值，分别为'总记录数'、'页面大小'、'当前页(默认为空则自动读取GET变量)'、'页码参数，默认为page'
	$p->rewrite = false;				//设置重写与否(false/true)，默认false不重写
	$p->styleMode = 0;					//设置前后翻页样式(0/1)，默认0为带文字样式(需要全局设定过变量$displayArr)
	$p->setVar(array(
		'b' => '2',
	));									//设置要传递的参数 --> 重写激活时无效
	$p->output();						//输出，默认直接输出,带参为1则否则返回一个字符串
	echo $p->limit();					//输出Limit子句。在sql语句中用法为 "SELECT * FROM TABLE {$p->limit()}";

 */

class Page {
	var $rewrite = false;//重写与否(默认不重写,重写时setVar函数将失效)
	var $styleMode = 0;//前后翻页样式(暂提供两种:一种为带文字格式,一种为不带文字)


	function Page($total, $pagesize = 15, $current = false, $pvar = 'page'){//分页初始化
		$this->total = $total;
		$this->pvar = $pvar;
		$this->pageMax = ceil($total / $pagesize);//总页数
		if(!$current) $current = empty($_GET[$this->pvar]) ? 1 : intval($_GET[$this->pvar]);
		if($current > $this->pageMax) $current = $this->pageMax;
		if($current < 1) $current = 1;

		$this->curr = $current;//当前页面
		$this->psize = $pagesize;//页面大小

		$this->varstr = '';
		$this->output = '';
	}

	function setVar($data){//除$_GET外追加变量的设置
		foreach($data as $k => $v) {
			$this->varstr .= (!empty($k) ? $k.'=' : '').$v.'&amp;';
		}
	}

	function output($return = false){
		if(empty($GLOBALS['displayArr']) || !is_array($GLOBALS['displayArr']))
			$this->display = array('prev10' => '&lt;&lt;', 'prev' => '&lt;', 'next' => '&gt;', 'next10' => '&gt;&gt;');
		elseif(array_key_exists($this->styleMode, $GLOBALS['displayArr']))
			$this->display = $GLOBALS['displayArr'][$this->styleMode];
		else
			$this->display = $GLOBALS['displayArr'][0];

		$this->OriginalUrl = $this->GetUrl();

		if($this->pageMax > 1) {
			$this->output .= '<div class="pages">';
			if($this->curr > 10) $this->output .= $this->HTMLTagA($this->FormatUrl($this->curr - 10), $this->display['prev10']);
			if($this->curr > 1) $this->output .= $this->HTMLTagA($this->FormatUrl($this->curr - 1), $this->display['prev']);

			$start = floor($this->curr / 10) * 10;
			$end = $start + 9;
			if($start < 1) $start = 1;
			if($end > $this->pageMax) $end = $this->pageMax;

			for($i = $start; $i <= $end; $i++) {
				if($this->curr == $i) $this->output .= '<span class="current">'.$i.'</span>';
				else $this->output .= $this->HTMLTagA($this->FormatUrl($i), $i, 'other');
			}

			if($this->curr < $this->pageMax)
				$this->output .= $this->HTMLTagA($this->FormatUrl($this->curr + 1), $this->display['next']);

			if($this->pageMax > 10 && $this->pageMax - $this->curr >= 10)
				$this->output .= $this->HTMLTagA($this->FormatUrl($this->curr + 10), $this->display['next10']);

			$this->output .= '</div>';
		}

		if($return)
			return $this->output;
		else
			echo $this->output;
	}

	function limit(){
		return ' LIMIT '.(($this->curr - 1) * $this->psize).', '.$this->psize.' ';
	}

	function between(){
		return ' BETWEEN '.($this->total - $this->curr * $this->psize + 1).' AND '.($this->total - ($this->curr - 1) * $this->psize).' ';
	}

	function GetUrl(){//获得合法的URL地址@Private
		if(empty($_SERVER['REDIRECT_URL']) || substr($_SERVER['REDIRECT_URL'], -4) == '.php') $this->rewrite = false;
		$BrowserUrl= !empty($_SERVER['HTTP_X_REWRITE_URL']) ? $_SERVER['HTTP_X_REWRITE_URL'] : $_SERVER['REQUEST_URI'];// IIS: HTTP_X_REWRITE_URL
		if($this->rewrite) {//是否重写(注意: 要求文件为.html结尾且重写规则中页码需要在最后。setVar无效,直接替换地址栏url)
			$pos = strpos($BrowserUrl, '-'.$this->pvar.'_'.$this->curr.'.html');//重写的用REQUEST_URI判断
			$pos2 = strpos($BrowserUrl, '-'.$this->pvar.'_'.$this->curr.'-') ;

			if(false === $pos && false === $pos2) {
				return 'http://'.$_SERVER['SERVER_NAME'].str_replace('.html', '-'.$this->pvar.'_ZPageChar.html', $BrowserUrl);
			}else {
				$diffchar = $pos ? '.html' : '-';
				return 'http://'.$_SERVER['SERVER_NAME'].str_replace('-'.$this->pvar.'_'.$this->curr.$diffchar, '-'.$this->pvar.'_ZPageChar'.$diffchar, $BrowserUrl);
			}
		}else {//未重写的用PHP_SELF+QUERY_STRING判断
			parse_str($_SERVER['QUERY_STRING'], $QUERY_STRING);
			$this->setVar($QUERY_STRING);

			$WholeUrl = 'http://'.$_SERVER['SERVER_NAME'].$_SERVER['PHP_SELF'].'?'.$this->varstr;

			$pos = strpos($WholeUrl, $this->pvar.'='.$this->curr.'&amp;');//检查是否已含分页

			if(false === $pos) $OriginalUrl = $WholeUrl.$this->pvar.'=ZPageChar&amp;';
			else $OriginalUrl = str_replace($this->pvar.'='.$this->curr.'&amp;', $this->pvar.'=ZPageChar&amp;', $WholeUrl);

			return substr($OriginalUrl, 0 ,-5);//去掉末尾&amp;
		}
	}

	function FormatUrl($num){//格式化URL@Private
		return str_replace('ZPageChar', $num, $this->OriginalUrl);
	}

	function HTMLTagA($href, $text, $style = ''){
		return '<a href="'.$href.($style ? '" class="'.$style : '').'">'.$text.'</a>';
	}
}


/*

//默认分页样式
.pages {
	padding: 1px;
	margin: 2px 0;
	clear: left;
	font-size: 100%;
}
.pages a, .pages span {
	display: block;
	float: left;
	line-height: 18px;
	height: 18px;
	padding: 0.1em 0.5em;
	margin-right: 0.1em;
	border: 1px solid White;
	background: White;
}
.pages a.other {
	color: #2E6AB1;
}

.pages span.current {
	border: 1px solid #2E6AB1;
	font-weight: bold;
	background: #698CC3;
	color: White;
}

.pages a {
	border: 1px solid #9AAFE5;
	color: #698CC3;
	text-decoration: none;
}

.pages a:hover {
	border-color: #2E6AB1;
	text-decoration: none;
}

.pages a.nextprev {
	font-weight: bold;
}

.pages span.nextprev {
	color: White;
	background: #698CC3;
	border: 1px solid #9AAFE5;
}
*/
