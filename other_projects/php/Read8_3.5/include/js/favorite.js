function favorite(bid, cid){
	if(typeof r_xmlhttp == 'object')
		return;

	if(!cid)
		cid = 0;

	r_ajax_request(G_siteurl+'/my/favorite.php?ajax=1&bid='+bid+'&cid='+cid, 'favorite');
}

function r_favorite_ajax_handler(){
	if(!r_ajax_handler())
		return;

	eval(r_xmlhttp.responseText);

	r_xmlhttp = false;
}

function rate(bid, score, story){
	if(typeof r_xmlhttp == 'object')
		return;

	r_ajax_request(G_siteurl+'/rate.php?ajax=1&bid='+bid+'&score='+score+'&story='+story, 'rate');
}

function r_rate_ajax_handler(){
	if(!r_ajax_handler())
		return;

	eval(r_xmlhttp.responseText);

	r_xmlhttp = false;
}

function vote(bid){
	if(typeof r_xmlhttp == 'object')
		return;

	r_ajax_request(G_siteurl+'/vote.php?ajax=1&bid='+bid, 'vote');
}

function r_vote_ajax_handler(){
	if(!r_ajax_handler())
		return;

	eval(r_xmlhttp.responseText);

	r_xmlhttp = false;
}

function digg(bid, digg, story){
	if(typeof r_xmlhttp == 'object')
		return;

	r_ajax_request(G_siteurl+'/digg.php?ajax=1&bid='+bid+'&digg='+digg+'&story='+story, 'digg');
}

function r_digg_ajax_handler(){
	if(!r_ajax_handler())
		return;

	eval(r_xmlhttp.responseText);

	r_xmlhttp = false;
}
