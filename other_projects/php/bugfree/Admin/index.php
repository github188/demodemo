<?php
/**
 * BugFree is free software under the terms of the FreeBSD License.
 *
 * admin index.
 *
 * @link        http://www.bugfree.org.cn
 * @package     BugFree
 */
/* Init BugFree system. */
require_once("../Include/Init.inc.php");

/**
 * Judge whether the user is an admin user.
 */
baseJudgeAdminUserLogin();

header("location:AdminProjectList.php");
?>
