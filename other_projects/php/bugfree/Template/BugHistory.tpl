        {foreach from=$BugInfo.ActionList key=StepNum item=ActionInfo}
        <dl style="text-align:left;margin:6px 3px;padding:0;">
          <!--<dt style="float:left;width:20px;font-weight:normal;margin:15px 0 3px 0;">{$StepNum}.</dt>-->
          <dd style="float:left;margin:5px 0 0 4px;">{$ActionInfo.ActionDate}{if $ActionInfo.ActionType eq 'Resolved'}
            <strong>{$ActionInfo.ActionType}</strong> as <strong>{$ActionInfo.Resolution}</strong> by <strong>{$ActionInfo.ActionUserName}</strong>{else}
            <strong>{$ActionInfo.ActionType}</strong> by <strong>{$ActionInfo.ActionUserName}</strong>{/if}
          </dd>{if $ActionInfo.AFCount neq 0 || $ActionInfo.ActionNote neq ''}
          <dd style="float:left;clear:both;width:460px;clear:both;background-color:#F0F0F0;border:1px solid #E4E4E4;margin:0 0 0 4px;padding:5px;">
            <ul style="list-style:none;margin:0;padding:0;color:#333333;">
              {foreach from=$ActionInfo.HistoryList key=Key item=HistoryInfo}
              {if $HistoryInfo.ActionField eq 'ReproSteps' or $HistoryInfo.ActionField eq 'BugTitle'}
              <li>Changed <strong>{$HistoryInfo.ActionField}</strong> from <strong onmouseover="return overlib('{$HistoryInfo.OldValue|overlib_bbcode2html}',ABOVE,WIDTH,{$Lang.OLWIDTH},BGCOLOR,'{$Lang.OLBGCOLOR}',FGCOLOR,'{$Lang.OLFGCOLOR}');" onmouseout="return nd();">"<a href="javascript:void(0);" onclick="return false;">...</a>"</strong> to <strong onmouseover="return overlib('{$HistoryInfo.NewValue|overlib_bbcode2html}',ABOVE,WIDTH,{$Lang.OLWIDTH},BGCOLOR,'{$Lang.OLBGCOLOR}',FGCOLOR,'{$Lang.OLFGCOLOR}');" onmouseout="return nd();">"<a href="javascript:void(0);" onclick="return false;">...</a>"</strong></li>
              {elseif ($ActionInfo.ActionType neq 'Resolved' or $HistoryInfo.ActionField neq 'Resolution') and $HistoryInfo.ActionField neq 'ProjectID' and $HistoryInfo.ActionField neq 'ModuleID'}
              <li>Changed <strong>{$HistoryInfo.ActionField}</strong> from <strong>"{$HistoryInfo.OldValue}"</strong> to <strong>"{$HistoryInfo.NewValue}"</strong></li>{/if}{/foreach}
              {foreach from=$ActionInfo.FileList key=Key item=FileInfo}
              <li>Added file <strong{if $FileInfo.IsDroped eq '1'} style="text-decoration:line-through;"{/if}>{$FileInfo.FileTitle}</strong></li>{/foreach}
            </ul>
            <blockquote style="margin:10px 1px 10px 1px;">{$ActionInfo.ActionNote|replace:" ":"&nbsp;"|bbcode2html}</blockquote>
          </dd>{/if}
        </dl>
        {/foreach}
