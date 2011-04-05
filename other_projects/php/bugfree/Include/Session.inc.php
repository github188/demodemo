<?php
/**
 * BugFree is free software under the terms of the FreeBSD License.
 *
 * session.
 *
 * @link        http://www.bugfree.org.cn
 * @package     BugFree
 */
$sess_save_path = ini_get('session.save_path');
$sess_prefix = str_replace(dirname($_CFG['RealRootPath']),'',$_CFG['RealRootPath']);
$sess_prefix = str_replace("/",'',$sess_prefix);
$sess_prefix = str_replace("\\",'',$sess_prefix);

function sess_open($save_path, $session_name)
{
  global $sess_save_path, $sess_session_name;

  $sess_save_path = $save_path;
  $sess_session_name = $session_name;
  return(true);
}

function sess_close()
{
  return(true);
}

function sess_read($id)
{
  global $sess_save_path, $sess_session_name, $sess_prefix;

  $sess_file = "$sess_save_path/sess_{$sess_prefix}{$id}";
  if ($fp = @fopen($sess_file, "r")) {
    $sess_data = @fread($fp, filesize($sess_file));
    return($sess_data);
  } else {
    return(""); // Must return "" here.
  }

}

function sess_write($id, $sess_data)
{
  global $sess_save_path, $sess_session_name, $sess_prefix;

  $sess_file = "$sess_save_path/sess_{$sess_prefix}{$id}";
  if ($fp = @fopen($sess_file, "w")) {
    return(@fwrite($fp, $sess_data));
  } else {
    return(false);
  }

}

function sess_destroy($id)
{
  global $sess_save_path, $sess_session_name, $sess_prefix;

  $sess_file = "$sess_save_path/sess_{$sess_prefix}{$id}";
  return(unlink($sess_file));
}

function sess_gc($maxlifetime)
{
  return true;
}

if($sess_save_path != '')
{
    session_set_save_handler("sess_open", "sess_close", "sess_read", "sess_write", "sess_destroy", "sess_gc");
}
?>
