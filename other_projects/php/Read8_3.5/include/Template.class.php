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
//**********************************************************//
//                          模版类                          //
//   说明:                                                  //
//       ·模版路径为当前子栏根目录下的template目录;        //
//       ·路径若有不同, 自行使用“../”来定位;             //
//                                                          //
//**********************************************************//
if(!defined('IN_Read8')) {
	exit('Access Denied (AOKOTpl.class.php)');
}

/*
	function list

	void AOKOTpl( string template_filename )
	void assign( mixed name [, mixed value] )
	mixed output( [bool return] )
	int mtime( string filename )

	void TemplateParser( string template_filename )
	mixed compile( [string compiled_template_path [, string compiled_template_filename]] )
	array var_name( string tag )
	array cmd_name( string tag )
*/

class AOKOTpl{
	var $single = 0;// 单/多模版

	function AOKOTpl($template_filename, $real_template = ''){
		global $_SYSTEM, $_SaveHTML;

		$this->data = array();
		$this->temp_dir = ROOT.'data/cache/template/';

		if(!$this->single) {
			$this->tpl_file = 'template/'.$_SYSTEM['DISPLAY']['template'].'/'.$template_filename.'.html';
			if(is_file(ROOT.$this->tpl_file)) {
				$this->cpl_file = 'template_'.preg_replace('/[:\/.\\\\]/', '_', str_replace('../', '', $_SYSTEM['DISPLAY']['template']).'_'.$template_filename).'.php';
			}else {
				$this->tpl_file = 'template/basic/'.$template_filename.'.html';
				$this->cpl_file = 'template_basic_'.preg_replace('/[:\/.\\\\]/', '_', $template_filename).'.php';
			}
		}else{
			$this->tpl_file = 'template/'.$template_filename.'.html';
			$this->cpl_file = 'template_'.preg_replace('/[:\/.\\\\]/', '_', $template_filename).'.php';
		}

		if($real_template) {
			if(!$this->single) {
				$this->tpl_file = 'template/'.$_SYSTEM['DISPLAY']['template'].'/'.$real_template.'.html';
				if(!is_file(ROOT.$this->tpl_file))
					$this->tpl_file = 'template/basic/'.$real_template.'.html';
			}else {
				$this->tpl_file = 'template/'.$real_template.'.html';
			}
		}

		if(isset($GLOBALS['_assign']))
			$this->assign($GLOBALS['_assign']);
	}
	function assign($name, $value = ''){
		if(is_array($name))
			foreach($name as $k => $v)
				$this->data[$k] = $v;
		else
			$this->data[$name] = $value;
	}
	function output($return = false){
		$_top = $this->data;
		$_obj = &$_top;
		$_stack_cnt = 0;
		$_stack[$_stack_cnt++] = $_obj;
		// Check if template is already compiled

		$compile_template = true;
		if(is_file($this->temp_dir.$this->cpl_file))
			if($this->mtime($this->temp_dir.$this->cpl_file) > $this->mtime(ROOT.$this->tpl_file))
				$compile_template = false;

		if($compile_template) {
			$this->parser = new AOKOTplParser($this->tpl_file);
			if(!$this->parser->compile($this->temp_dir, $this->cpl_file))
				trigger_error('Template Parse Error: '.$this->parser->error, E_USER_WARNING);
		}
		if($return) {
			ob_start();
			include $this->temp_dir.$this->cpl_file;
			return ob_get_clean();
		}else {
			include $this->temp_dir.$this->cpl_file;
		}
	}
	function mtime($filename){
		if(is_file($filename))
			return filemtime($filename);
	}
}

class AOKOTplParser{
	function AOKOTplParser($template_filename){
		// Load Template
		$this->template = rfor(ROOT.$template_filename);
		$this->tpl_file = $template_filename;
	}
	function compile($compiled_template_path = '', $compiled_template_filename = ''){
		global $_SYSTEM;
		if(false === $this->template) {
			$this->error = "Read template file $this->tpl_file failed";
			return false;
		}

		//include ConfigFile('adverts');
		/*foreach($_CACHE['adverts']['cachead'] as $pos => $cachead) {
			$adstr = "<div><script type=\"text/javascript\" src=\"".$_SYSTEM['SYSTEM']['SITE_ADDR']."/dataquote.php?getad=$pos\"></script></div>";
			if($cachead) $adstr .= '<noscript><div>'.$_CACHE['adverts'][$pos].'</div></noscript>';
			$this->template = $this->template = str_replace("{CAD_$pos}", $adstr, $this->template);
		}*/

		$this->template = preg_replace('/(<\?|<\?php)/ise',"htmlspecialchars('\\1');", $this->template);

		$page = "<?php\nif(!defined('IN_Read8')) {\n\texit('Access Denied ($compiled_template_filename)');\n}\n?>\n".$this->template;
		// END, ELSE Blocks
		$page = preg_replace("/<!-- ENDIF.+?-->/", "<?php\n}\n?>", $page);
		$page = preg_replace("/<!-- END[ a-zA-Z0-9_.]* -->/", "<?php\n}\n\$_obj=\$_stack[--\$_stack_cnt];}\n?>", $page);
		$page = str_replace("<!-- ELSE -->", "<?php\n}else {\n?>", $page);
		// 'BEGIN - END' Blocks
		if(preg_match_all('/<!-- BEGIN ([a-zA-Z0-9_.]+) -->/', $page, $var)) {
			foreach($var[1] as $tag) {
				list($parent, $block) = $this->var_name($tag);
				$code =  "<?php\n"
						."\tif(!empty(\$$parent"."['$block'])) {\n"
						."\t\tif(!is_array(\$$parent"."['$block']))\n"
						."\t\t\t\$$parent"."['$block'] = array(array('$block' => \$$parent"."['$block']));\n"
						."\t\t\$_tmp_arr_keys = array_keys(\$$parent"."['$block']);\n"
						."\t\tif(\$_tmp_arr_keys[0] != '0')\n"
						."\t\t\t\$$parent"."['$block'] = array(0 => \$$parent"."['$block']);\n"
						."\t\t\$_stack[\$_stack_cnt++] = \$_obj;\n"
						."\t\tforeach(\$$parent"."['$block'] as \$rowcnt=>\$$block) {\n"
						."\t\t\t\$$block"."['I'] = \$rowcnt;\n"
						."\t\t\t\$$block"."['II'] = \$rowcnt+1;\n"
						."\t\t\t\$$block"."['III'] = \$rowcnt%7;\n";

				if(isset($GLOBALS['_assign']))
					foreach($GLOBALS['_assign'] as $key => $val)
						$code .= "\t\t\t\$$block"."['$key'] = \$GLOBALS['_assign']['$key'];\n";

				$code .= "\t\t\t\$$block"."['A'] = \$rowcnt % 2;\n"
						."\t\t\t\$$block"."['B'] = \$rowcnt % 2;\n"
						."\t\t\t\$_obj = &\$$block;\n?>";
				$page = str_replace("<!-- BEGIN $tag -->", $code, $page);
			}
		}
		// 'IF nnn="mmm"' Blocks
		if(preg_match_all('/<!-- (ELSE)?IF ([a-zA-Z0-9_.]+)([!=<>]+)"([^"]*)" -->/', $page, $var)) {
			foreach($var[2] as $cnt => $tag) {
				list($parent, $block) = $this->var_name($tag);
				$cmp  = $var[3][$cnt];
				$val  = $var[4][$cnt];
				$else = ($var[1][$cnt] == 'ELSE') ? '}else' : '';
				if($cmp == '=') $cmp = '==';
				$code = "<?php\n$else"."if(\$$parent"."['$block'] $cmp \"$val\") {\n?>";
				$page = str_replace($var[0][$cnt], $code, $page);
			}
		}
		// 'IF nnn' Blocks
		if(preg_match_all('/<!-- (ELSE)?IF ([a-zA-Z0-9_.]+) -->/', $page, $var)) {
			foreach($var[2] as $cnt => $tag) {
				$else = ($var[1][$cnt] == 'ELSE') ? '} else' : '';
				list($parent, $block) = $this->var_name($tag);
				$code = "<?php\n$else"."if(!empty(\$$parent"."['$block'])) {\n?>";
				$page = str_replace($var[0][$cnt], $code, $page);
			}
		}
		// Replace Scalars
		if(preg_match_all('/{([a-zA-Z0-9][a-zA-Z0-9_\-:. >]*)}/', $page, $var)) {
			foreach($var[1] as $fulltag) {
				// Determin Command (echo / $obj[n]=)
				list($cmd, $tag) = $this->cmd_name($fulltag);
				list($block, $skalar) = $this->var_name($tag);
				if(preg_match('/([\w]+)_DATE_(.*)/e',$skalar,$skalar2)){
					if(function_exists('rdate')){
						$code = "<?php\n$cmd rdate(\$$block"."['$skalar2[1]'],'$skalar2[2]');\n?>\n";
					}else{
						$code = "<?php\n$cmd date('$skalar2[2]',\$$block"."['$skalar2[1]']);\n?>\n";
					}
				}else{
					$code = "<?php\n$cmd \$$block"."['$skalar'];\n?>\n";
				}
				$page = str_replace('{'.$fulltag.'}', $code, $page);
			}
		}
		// Add Include Header
		if(isset($header) && $header) {
			$page = "<?php\n$header\n?>$page";
		}
		// Store Code to Temp Dir
		if($compiled_template_path && $compiled_template_filename) {
			rmkdir('data/cache/template');
			if(false === rfow($compiled_template_path.$compiled_template_filename, $page)) {
				$this->error = "No write permission at ./data/cache/template/ or $compiled_template_filename";
				return false;
			}else {
				return true;
			}
		}else {
			return $page;
		}
	}
	function var_name($tag){
		$parent_level = 0;
		while(substr($tag, 0, 7) == 'parent.') {
			$tag = substr($tag, 7);
			$parent_level++;
		}
		if(substr($tag, 0, 4) == 'top.') {
			$obj = '_stack[0]';
			$tag = substr($tag,4);
		}elseif($parent_level) {
			$obj = '_stack[$_stack_cnt-'.$parent_level.']';
		}else {
			$obj = '_obj';
		}
		while(is_int(strpos($tag, '.'))) {
			list($parent, $tag) = explode('.', $tag, 2);
			if(is_numeric($parent)) {
				$obj .= "[".$parent."]";
			}else {
				$obj .= "['".$parent."']";
			}
		}
		return array($obj, $tag);
	}
	function cmd_name($tag){
		if(preg_match('/^(.+) > ([a-zA-Z0-9_.]+)$/', $tag, $tagvar)) {
			$tag = $tagvar[1];
			list($newblock, $newskalar) = $this->var_name($tagvar[2]);
			$cmd = "\$$newblock"."['$newskalar'] = ";
		}else {
			$cmd = "echo";
		}
		return array($cmd, $tag);
	}
}