<?php
/**
 * BugFree is free software under the terms of the FreeBSD License.
 *
 * logout.
 *
 * @link        http://www.bugfree.org.cn
 * @package     BugFree
 */
/* Init BugFree system. */
require("Include/Init.inc.php");

session_destroy();
@setcookie("BFUserPWD", '', time()-3600);
@setcookie("BFUserName", '', time()-3600);
jsGoto("Login.php","parent.parent");
?>
