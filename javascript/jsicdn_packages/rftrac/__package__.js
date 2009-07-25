this.addScript('robot.js', ['setup_robot_trac', 'get_test_case_table']);
this.addScript('dialog.js', 'build_testcase_comments_dialog');

this.addDependence('robot.js', 'jQuery:jquery-ui.js');
this.addDependence('robot.js', 'rftrac:build_testcase_comments_dialog');
this.addDependence('robot.js', 'hash:To_md5');