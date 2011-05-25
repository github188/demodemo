<ul>
        <?php
                $page = 0;
                for($index=0;$index<count($friendList);){
        ?>
        <li style="height: 535px;width: 325px;" >
        <?php
                for ($block=0;$index<count($friendList)&&$block<12;$block++) {
                        $su = $friendList [$index];
//                      $su["profile_image_url"] = str_replace("/50/", "/180/", $su["profile_image_url"]);
        ?>
                <div class="gz_per">
                <table width="100%" border="0" cellspacing="0" cellpadding="0">
                        <tr>
                                <td width="70" rowspan="2">
                                        <a href="http://weibo.com/<?=$su["id"] ?>" target="_blank">
                                                <img src="<?=$su["profile_image_url"] ?>" width="60" height="60" class="imgbox" />
                                        </a>
                                </td>
                                <td height="35" valign="top" style="color: #666;"><a href="http://weibo.com/<?=$su["id"] ?>" target="_blank" style="color: #666;text-decoration: none;"><?=$su["screen_name"] ?></a></td>
                        </tr>
                        <tr>
                                <td valign="top"></td>
                        </tr>
                </table>
                </div>
        <?php
                        $index++;
                }
        ?>
        </li>
        <?php
                        $page++;
                }
        ?>
</ul>

