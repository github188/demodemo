                               {if $ActionType eq 'OpenBug' or $ActionType eq 'OpenCase' or $ActionType eq 'OpenResult' or $EditMode eq 'true'}
                            {foreach from=$FileList key=ActionID item=FileListInfo}
                              {foreach from=$FileListInfo item=FileInfo}
                                {if $FileInfo.IsDroped eq '0'}
{if $FileInfo.ForceDownload eq '1'}{assign var="target" value="_self"}{else}{assign var="target" value="_blank"}{/if}
                                <span id="TestFile{$FileInfo.FileID}">[<a href="FileInfo.php?FileID={$FileInfo.FileID}" target="{$target}" onclick="setNeedConfirm();NeedToConfirm=false;">{$FileInfo.FileTitle}</a>&nbsp;<a href="javascript:void(0);" id="TestFile{$FileInfo.FileID}Remove" style="color:#CC0000;text-decoration:none;font-weight:bold;" onclick="if(confirm('{$Lang.ConfirmDeleteTestFile}'))xajax_xDeleteTestFile('{$FileInfo.FileID}');"><img src="Image/deletefile.gif" /></a>]</span>
                              {/if}
                            {/foreach}
                          {/foreach}
                          {include file="AddFiles.tpl"}
                          {else}
                          {foreach from=$FileList key=ActionID item=FileListInfo}
                              {foreach from=$FileListInfo item=FileInfo}
                                  {if $FileInfo.IsDroped eq '0'}
{if $FileInfo.ForceDownload eq '1'}{assign var="target" value="_self"}{else}{assign var="target" value="_blank"}{/if}
                                    <span id="TestFile{$FileInfo.FileID}">[<a href="FileInfo.php?FileID={$FileInfo.FileID}" target="{$target}">{$FileInfo.FileTitle}</a>]</span>
                                {/if}{
                            /foreach}
                        {/foreach}
                    {/if}
