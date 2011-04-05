<?php
/**
 * BugFree is free software under the terms of the FreeBSD License.
 *
 * admin menu.
 *
 * @link        http://www.bugfree.org.cn
 * @package     BugFree
 */
/* Init BugFree system. */
require_once("../Include/SetupBug.inc.php");    // Include init file.
bugJudgeAdminUser();                            // Judge whether the user is an admin user.
require_once("TreeMenu.class.php");             // Include HTML_TreeMenu class file.

/**
 * Register current selected project id to session and get it's info.
 */
$_REQUEST["ProjectID"] > 0 ? $_SESSION["BugProjectID"] = $_REQUEST["ProjectID"] : "";
$ProjectInfo = bugGetProjectInfo($_SESSION["BugProjectID"]);

/**
 * Create module tree of current project.
 */

/* Set the link url. */
$LinkURL = "ManageModule.php";

/* Create an object of HTML_TreeMenu class whose name is TreeMenu defined by function bugGetModules(). */
$TreeMenu  = new HTML_TreeMenu();

/* Eval the HTML_TreeMenu code creaed by bugGetModules() to fill the nodes of module tree. */
eval (bugGetModules($ProjectInfo["ProjectID"],1,0,"Module",$LinkURL,"AdminMainFrame"));

/* Create an object of HTML_TreeMenu_DHTML class from $TreeMenu. */
$ModuleTree = &new HTML_TreeMenu_DHTML($TreeMenu, array('images' => '../Images/TreeMenu', 'defaultClass' => 'treeMenuDefault'));

/* Get the DHTML tree content and assign to template. */
$TPL->assign("ModuleTree",$ModuleTree->printMenu(false));

/* Assign project vars to template. */
$TPL->assign("ProjectList",  $MyHtml->select(bugGetProjects(),"ProjectID","Reverse","ProjectForm.submit();parent.AdminMainFrame.location='ManageProject.php?ProjectID='+document.ProjectForm.ProjectID.value"));
$TPL->assign("SelectProject",$MyJS->selectOption("document.ProjectForm.ProjectID",$ProjectInfo["ProjectID"]));
$TPL->assign("ProjectInfo",  $ProjectInfo);

/* Display the template file. */
$TPL->display("Admin/AdminMenu.tpl");
?>
