
/**
*
*@private
*/
function TracDialog(E){
	this.E = E;
	this._tcname = $("#cd_tcname", E);
	this._category = $("#cd_category", E);
	this._bugid = $("#cd_bugid", E);
	this._user = $("#cd_user", E);
	this._comments = $("#cd_comments", E);
}

TracDialog.prototype = {
	tcname: function(){return this._tcname.html.apply(this._tcname, arguments);},
	category: function(){return this._category.html.apply(this._category, arguments);},
	bugid: function(){return this._bugid.val.apply(this._bugid, arguments);},
	user: function(){return this._user.val.apply(this._user, arguments);},
	comments: function(){return this._comments.val.apply(this._comments, arguments);}
}

/**
*
*/
function build_testcase_comments_dialog(submit_callback){

	var dialog_element = $(
		'<div id="comments_dialog" title="testcase comments">' +
		'<table border="0"><tr><th align="left">TestCase:</th><td><span id="cd_tcname">xxxxxxxx</span></td></tr>' +
		'<tr><th align="left">Trac Category:</th><td><span id="cd_category">xxxxxxxx</span></td></tr>' +
		'<tr><th align="left">BUG ID:</th><td><input type="text" name="bug" id="cd_bugid" size="40"/></td></tr>' +
		'<tr><th align="left">Comment user:</th><td><input type="text" name="user" id="cd_user" size="40"/></td></tr>' +
		'<tr><th align="left">Comment:</th><td><textarea name="comments" id="cd_comments" rows="6" cols="40"></textarea></td></tr>' +
		"</table></div>");
	
	var comments = dialog_element.dialog({
									modal:true,
									autoOpen: false,
									width: 480,
									height: 300,
									buttons: {
										"Submit": function() {
											submit_callback();
											$(this).dialog("close"); 
										}, 
										"Cancel": function() {
											$(this).dialog("close"); 
										}
									}
								});
								
	//$("#comments_dialog").css({display:'block'});
	comments.model = new TracDialog(comments);	
	return comments;
}

//function 

