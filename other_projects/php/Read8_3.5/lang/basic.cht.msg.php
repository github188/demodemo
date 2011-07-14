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
$Lang = array(
	'api_not_open' => '尚未啟用通行證，請先通過後臺設置',
	'book_not_exists' => '沒有這本書',
	'hash_code_error' => '驗證碼錯誤',
	'user_not_exists' => '用戶不存在',

	'apply_no_writer_privileges' => '您沒有作家許可權',
	'apply_already_writer_privileges' => '您已經具有作家許可權',
	'apply_money_input_error' => '金額輸入錯誤',
	'apply_no_enough_money' => '賬上餘額不足',
	'apply_require_data' => '填寫不全',

	'apply_pay_success' => '續費申請成功',
	'apply_fee_success' => '兌換申請成功',
	'apply_author_success' => '作家申請成功',
	'apply_book_success' => '新書申請成功',

	'bookcase_limit' => '您的書架上收藏書籍數目超限',

	'bookcase_success' => '添加刪除書架成功',
	'bookcase_modify_success' => '修改書架成功',
	'bookcase_bookcase_update_success' => '書簽更新成功',

	'booked_no_manage_book_privileges' => '無作品管理許可權',
	'booked_no_manage_chapter_privileges' => '您沒有該書章節發佈和管理許可權',
	'booked_face_filetype_error' => '圖片類型錯誤，請使用JPG格式圖片',
	'booked_face_filesize_error' => '圖片大小超限',

	'booked_face_upload_success' => '封面上傳成功',
	'booked_volume_success' => '卷操作成功',

	'bookreview_no_delete_privileges' => '您沒有刪除該書評的許可權',
	'bookreview_no_post_privileges' => '您沒有發表書評的許可權',
	'bookreview_include_banned_string' => '內容含有被禁止字串',
	'bookreview_length_overload' => '書評超出限定值',

	'bookreview_delete_success' => '書評刪除成功',
	'bookreview_post_success' => '書評發表成功',

	'bookstat_not_in_bookcase' => '此書不在書架，請先收藏。',

	'bookstat_vote_success' => '投票成功，感謝您對本書的支持。',
	'bookstat_vote_repeat' => '您也太熱情了，請勿重複投票。',

	'chaptered_no_manage_book_privileges' => '您沒有該作品的管理許可權',
	'chaptered_no_manage_chapter_privileges' => '您沒有該作品的章節發佈和管理許可權',
	'chaptered_no_manage_chapter_privileges_delete' => '您沒有作品刪除許可權',
	'chaptered_upload_disabled' => '禁止上傳附件',
	'chaptered_upload_oversize' => '附件大小超限',
	'chaptered_upload_filetype_disabled' => '檔案類型被禁止',
	'chaptered_content_banned_string' => '文章標題或內容含有禁用詞',

	'chaptered_delete_chapter_success' => '章節刪除成功',
	'chaptered_delete_draft_success' => '草稿刪除成功',

	'charge_vip_expired' => '您的vip會員服務過期或尚未購買,請先購買vip會員服務',
	'charge_no_enough_money' => '賬上餘額不足',

	'charge_success' => '購買成功',

	'download_no_privileges' => '沒有下載許可權',
	'download_file_not_exists' => '無此文件',

	'drafted_no_manage_draft_privileges' => '您沒有該作品草稿的管理許可權',

	'groups_no_view_privileges' => '您沒有查看群組的許可權',
	'groups_no_manage_modrs' => '您沒有設置組長的許可權',
	'groups_no_manage_members' => '您沒有設置該組組員的許可權',
	'groups_only_in_one_group' => '此用戶已經是其他組成員。請通知其退出其他組先。',
	'groups_not_in_this_group' => '此用戶不是該組成員',

	'groups_exit_success' => '退出群組成功',
	'groups_set_modr_success' => '設置組長成功',
	'groups_set_members_success' => '設置組員成功',

	'login_username_password_lack' => '缺少用戶名或密碼',
	'login_password_error' => '密碼錯誤',

	'login_login_success' => '登錄成功',
	'login_logoff_success' => '退出成功',

	'mobilepay_failed' => '校驗失敗，請勿偽造支付內容。',
	'mobilepay_success' => '線上購買成功',
	'mobilepay_money_input_error' => '金額輸入錯誤',

	'profile_register_closed' => '目前註冊不可用',
	'profile_register_logined' => '請勿重複註冊',
	'profile_register_failed' => '註冊資料插入失敗',
	'profile_edit_failed' => '資料更新資料插入失敗',

	'profile_register_success' => '註冊成功',
	'profile_edit_success' => '修改成功',

	'pointconv_ins_bbs_credit_not_open' => '尚未設定積分轉換欄位，請先通過後臺設置',

	'pointconv_success' => '積分轉換成功',

	'shop_chapter_ordered' => '您已經購買過此章節了',

	'shop_add_success' => '添加成功',
	'shop_del_success' => '刪除成功',


	'worksed_no_manage_book_privileges' => '您沒有管理作品的許可權',
	'worksed_no_manage_chapter_privileges' => '您沒有管理該書章節的許可權',
	'worksed_no_directory_write_privileges' => '錯誤代碼: 00001\n移動靜態網頁檔失敗，請聯繫管理員',
	'worksed_intro_banned_string' => '書籍介紹含有禁用詞',

	'worksed_modify_booktype_success' => '修改書籍分類成功',
	'worksed_modify_book_attribute' => '書籍操作成功',

	'log_profile_change_root_pswd' => '修改站長密碼',

	'log_admin_adverts' => '修改廣告',

	'seo_about' => '關於我們',
	'seo_error' => '錯誤資訊',
	'seo_guide' => '使用者指南',
	'seo_contact' => '聯繫方法',
	'seo_apply' => '申請程式',
	'seo_authorinfo' => '作家資訊',
	'seo_worksed' => '書籍列表',
	'seo_rank' => '排行榜',
	'seo_rank_all' => '總排行',
	'seo_rank_week' => '本周排行',
	'seo_rank_month' => '本月排行',
	'seo_rank_newbook' => '新書點擊榜',
	'seo_rank_vote' => '網友投票排行',
	'seo_update_last' => '最後更新',
	'seo_new_recommend' => '最新推薦',
	'seo_new_book' => '最新入庫',
	'seo_typelist' => '分類列表',
	'seo_bookcase' => '個人書架',
	'seo_booked' => '章節清單程式',
	'seo_bookreview' => '書評',
	'seo_bookroom' => '公開書庫',
	'seo_chaptered' => '章節管理',
	'seo_charge' => '電子書收銀台',
	'seo_drafted' => '草稿箱',
	'seo_groups' => '組設置',
	'seo_login' => '登錄程式',
	'seo_mobilepay' => '線上支付程式',
	'seo_pointconv' => '論壇積分轉換',
	'seo_profile' => '註冊程式',
	'seo_profile_edit' => '資料修改查看',
	'seo_shop' => '電子書庫',

	'admin_file_error' => '暫不提供',
	'admin_login_success' => '登錄成功',
	'admin_no_privileges' => '無訪問後臺許可權',
	'admin_no_privileges_root' => '非站長勿動',

	'admin_no_privileges_verifybook' => '您沒有新書審核許可權',
	'admin_no_privileges_verify_pay' => '您沒有續費審核許可權',
	'admin_no_privileges_verify_fee' => '您沒有兌換審核許可權',
	'admin_no_privileges_verify_writer' => '您沒有作家審核許可權',

	'admin_no_privileges_manage_announce' => '您沒有公告管理許可權',
	'admin_no_privileges_manage_book_type' => '您沒有類別管理許可權',
	'admin_no_privileges_manage_book' => '您沒有作品管理許可權',
	'admin_no_privileges_manage_book_del' => '您沒有作品刪除許可權',
	'admin_no_privileges_manage_book_update' => '您沒有書籍更新管理許可權',
	'admin_no_privileges_manage_book_zip' => '您沒有打包下載管理許可權',
	'admin_no_privileges_manage_comm' => '您沒有書評管理許可權',
	'admin_no_privileges_manage_links' => '您沒有友情連結管理許可權',
	'admin_no_privileges_manage_banned' => '您沒有友情連結管理許可權',

	'admin_no_privileges_manage_templates' => '您沒有範本管理許可權',
	'admin_no_privileges_manage_styles' => '您沒有風格管理許可權',

	'admin_no_privileges_collect' => '您沒有採集許可權',
	'admin_no_privileges_batcollect' => '您沒有批量採集許可權',

	'admin_no_privileges_bookbackup' => '您沒有書籍備份許可權',

	'admin_no_privileges_logs' => '您沒有日誌&統計查看許可權',

	'admin_manage_admin_success_add' => '添加後臺用戶成功',
	'admin_manage_admin_success_edit' => '編輯後臺用戶成功',
	'admin_manage_admin_delete_self' => '不能刪除自己',
	'admin_manage_admin_delete_grant' => '不能刪除授予自己許可權的用戶',
	'admin_manage_admin_edit_self' => '不能編輯自己',
	'admin_manage_admin_edit_grant' => '不能編輯授予自己許可權的用戶',

	'admin_manage_adverts' => '修改廣告成功',

	'admin_manage_announce_success_edit' => '發佈/修改公告成功',
	'admin_manage_announce_success_del' => '刪除公告成功',

	'admin_manage_book_success_change_state' => '書籍狀態修改成功',
	'admin_manage_book_success_clear' => '書籍清零成功',
	'admin_manage_book_success_del' => '書籍刪除成功',

	'admin_manage_book_update_success_map' => '站內地圖生成成功',
	'admin_manage_book_update_success_page' => '書籍頁面更新成功',

	'admin_manage_book_zip_success_del' => '批量刪除打包下載成功',

	'admin_manage_comm_success_del' => '批量刪除書評成功',

	'admin_manage_log_success_backup' => '日誌立即備份成功',

	'admin_verify_book_verified' => '該書已審核',

	'admin_verify_book_success_deny' => '拒絕新書申請成功',
	'admin_verify_book_success_pass' => '通過新書申請成功',

	'admin_verify_fee_success_deny' => '拒絕兌換申請成功',
	'admin_verify_fee_success_deny_auto' => '尚未支付足夠金額，自動拒絕兌換申請',
	'admin_verify_fee_success_pass' => '通過兌換申請成功',

	'admin_verify_pay_success_deny' => '拒絕續費申請成功',
	'admin_verify_pay_success_pass' => '通過續費申請成功',

	'admin_verify_writer_verified' => '該申請已審核',

	'admin_verify_writer_success_deny' => '拒絕作家申請成功',
	'admin_verify_writer_success_pass' => '通過作家申請成功',

	'admin_manage_module_success_import' => '模組導入成功',
	'admin_manage_module_success_edit' => '修改模組設置成功',

	'admin_module_bookbase_error' => '參數不足，請返回重新設置',
	'admin_module_bookbase_file_not_exists' => '文件不存在: '.(empty($GLOBALS['datafile']) ? '' : $GLOBALS['datafile']),

	'admin_module_bookbase_success_backup' => '書庫備份成功',
	'admin_module_bookbase_success_recovery' => '恢復資料成功',
	'admin_module_bookbase_success_backup_del' => '刪除書庫備份成功',

	'admin_module_database_sqlrunclose' => '資料庫命令運行被禁止，開啟方法請參考運行頁面注釋。',
	'admin_module_database_error' => '參數不足，請返回重新設置',
	'admin_module_database_file_not_exists' => '文件不存在: '.(empty($GLOBALS['datafile']) ? '' : $GLOBALS['datafile']),

	'admin_module_database_success_backup' => '資料庫備份成功',
	'admin_module_database_success_recovery' => '恢復資料庫成功',
	'admin_module_database_success_backup_del' => '刪除資料庫備份成功',
	'admin_module_database_success_update' => 'SQL語句運行成功',
	'admin_module_database_success_optimize' => '資料庫優化成功',
	'admin_module_database_success_repair' => '資料庫修復成功',
	'admin_module_database_success_disable' => '禁止SQL運行成功',

	'admin_module_collect_cache_not_exists' => '類別緩存不存在，請重生成緩存',
	'admin_module_collect_get_book_name_failed' => '書籍名稱無法得到，請檢查書籍編號，並且確認您的伺服器配置正常，程式可以讀取遠端檔。',
	'admin_module_collect_get_author_failed' => '作者資訊無法得到，請確認規則正確，並且確認您的伺服器配置正常，程式可以讀取遠端檔。',
	'admin_module_collect_parse_chapter_error' => '解析書籍章節錯誤，採集規則出錯或者本書暫無章節',
	'admin_module_collect_parse_chapter_error_debug' => '解析書籍章節錯誤，採集規則出錯，或者本書暫無章節<br />調試資訊: '.(empty($GLOBALS['info']) ? '' : $GLOBALS['info']),
	'admin_module_collect_book_finished' => '此書已經全書完，如果需要採集請到作品管理中將其重新開啟',

	'admin_module_collect_select_task' => '尚未選擇任務',

	'admin_module_collect_success_single_import' => '規則導入成功',
	'admin_module_collect_success_single_delete' => '規則刪除成功',
	'admin_module_collect_success_single_edit' => '規則修改成功',
	'admin_module_collect_success_bat_import' => '規則導入成功',
	'admin_module_collect_success_bat_delete_preg' => '正則式批量採集任務刪除成功',
	'admin_module_collect_success_bat_delete_txt' => '文本式批量採集任務刪除成功',
	'admin_module_collect_success_bat_edit_preg' => '正則式批量採集任務修改成功',
	'admin_module_collect_success_bat_edit_txt' => '文本式批量採集任務修改成功',
	'admin_module_collect_success_' => '',
	'admin_module_collect_success_' => '',

	'admin_module_vip_service_success_manage' => '添加刪除服務成功',
	'admin_module_vip_service_success_edit' => '修改服務參數成功',
	'admin_module_vip_service_unknown_service_id' => '未知服務',

	'admin_login_pswd_error' => '後臺登錄密碼錯誤',
	'admin_login_success' => '登錄成功',
	'admin_login_failed' => '登錄失敗',
	'admin_user_not_granted' => '未知後臺用戶',
	'admin_change_pswd_error_repeat' => '兩次密碼不符',
	'admin_change_pswd_error_length' => '密碼長度不夠',
	'admin_change_pswd_error_history' => '不得使用曾使用過的密碼',
	'admin_change_pswd_success' => '修改密碼成功',

	'module_no_use_privilege' => '模組尚未開啟或您無權使用',

	'module_vip_sn' => '模組註冊碼過期',
	'module_vip_undefined_function' => 'VIP模組不存在此功能',

	'module_vip_my_service_exists' => '您已經訂購了該服務',
	'module_vip_my_register_failed' => '訂購服務失敗',
	'module_vip_my_register_success' => '訂購成功',

	'system_api_id_error' => '請勿偽造通行證ID',
	'system_book_id_error' => '書籍ID非數字',
	'system_book_type_id_error' => '類別ID非數字',
	'system_collect_id_error' => '非法的採集ID',
	'system_login_expired' => '登錄超時，請先登錄',
	'system_sn_error' => '型號限制，請檢查註冊碼是否過期及SN授權。',
	'system_pay_error' => '支付過程出錯，數額出錯或帳戶金額已經增加成功',
);
