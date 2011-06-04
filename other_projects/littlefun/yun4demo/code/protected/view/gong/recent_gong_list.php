<ul>
	<?php 
		foreach($gong_list as $gong){
            $u = $gong['user_profile'];
		?>
		<li>
		<div class="test_per" style="width:100%;">
			<a href="http://weibo.com/<?=$u['id'] ?>" target="_blank">
    <img width="50" height="50" style="margin-right: 2px;" class="imgbox" src="<?=$u["profile_image_url"]?>"/></a>
    			<a href="/gong/u/<?=$u["id"] ?>/" target="_blank" style="color: #000;text-decoration: none;">
		                    <?=$u["name"] ?>
                        </a>
                        <br/><br/>
    			<a href="/gong/u/<?=$u["id"] ?>/" target="_blank" style="color: #000;">

		             查看后宫
                        </a>                        
                             
		</div>
		</li>
		<?php }?>
</ul>