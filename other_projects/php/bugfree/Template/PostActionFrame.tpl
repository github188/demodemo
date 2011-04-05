{if $CFG.DebugMode}{assign var="DebugWinLength" value="1100"}{else}{assign var="DebugWinLength" value="0"}{/if}
<iframe name="PostActionFrame" width="{$DebugWinLength}" height="{$DebugWinLength}" scrolling="auto" frameborder="0"></iframe>
