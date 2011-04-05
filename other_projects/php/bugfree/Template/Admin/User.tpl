{include file="Admin/AdminHeader.tpl"}
<body class="AdminBody">
{include file="Admin/AdminTopNav.tpl"}
  <div class="AdminNav">
    &lt;<a href="AdminUserList.php">{$Lang.BackToUserList}</a>&#124;
    <strong>{$UserInfo.UserName}</strong>
  </div>
  {include file="ActionMessage.tpl"}
  <form id="UserForm" action="" method="post" onsubmit="xajax_xAdmin{$ActionType}(xajax.getFormValues('UserForm'));return false;">
  <div id="UForm" class="CommonForm AdminForm">
    <h1>{if $ActionType eq 'AddUser'}{$Lang.AddUser}{else}{$Lang.EditUser}{/if}</h1>
    <dl>
      <dt><label>{$Lang.AuthModeName}</label></dt>
      <dd>
        {if $ActionType eq 'AddUser'}
        <select id="AuthMode" name="AuthMode" onchange="changeRPE();">
            <option value="DB">{$Lang.AuthMode.DB}</option>
        </select>
        {else}
        {$UserInfo.AuthModeName}
        <input type="hidden" id="AuthMode" name="AuthMode" value="{$UserInfo.AuthMode}" />
        {/if}
      </dd>
    </dl>

    <dl>
      <dt><label>{$Lang.UserName}</label></dt>
      <dd>
        {if $ActionType eq 'AddUser'}<input type="text" id="UserName" name="UserName" maxlength="30" value="" />{else}
        {$UserInfo.UserName}<input type="hidden" id="UserName" name="UserName" maxlength="20" value="{$UserInfo.UserName}" />
        {/if}
      </dd>
    </dl>
    <dl id="RealNameDL">
      <dt><label for="RealName">{$Lang.RealName}</label></dt>
      <dd><input type="text" id="RealName" name="RealName" value="{$UserInfo.RealName}" />{$Lang.RealNameNote}</dd>
    </dl>
    <dl id="UserPasswordDL">
      <dt><label for="UserPassword">{$Lang.UserPassword}</label></dt>
      <dd><input type="text" id="UserPassword" name="UserPassword" value="" />{if $ActionType eq 'AddUser'}{$Lang.RawUserPasswordNote}{else if $ActionType eq 'EditUser'}{$Lang.EditUserPasswordNote}{/if}</dd>
    </dl>
    <dl id="EmailDL">
      <dt><label for="Email">{$Lang.Email}</label></dt>
      <dd><input type="text" id="Email" name="Email" value="{$UserInfo.Email}" />{$Lang.EmailNote}</dd>
    </dl>
    <dl class="Line"></dl>
    <dl>
      <input type="submit" class="Btn" name="SaveUser" value="{$Lang.Save}" />
      <input type="hidden" id="UserID" name="UserID" value="{$UserInfo.UserID}" />
    </dl>
  </div>
  </form>
{literal}
<script type="text/javascript">
//<![CDATA[
// disable or enable RealName, PassWord, Email
function changeRPE()
{
    if(xajax.$('AuthMode').value == 'LDAP')
    {
        hiddenObj('RealNameDL,UserPasswordDL,EmailDL');
        xajax.$('RealName').disabled=true;
        xajax.$('UserPassword').disabled=true;
        xajax.$('Email').disabled=true;
        //xajax.$('RealName').value="";
        //xajax.$('UserPassword').value="";
        //xajax.$('Email').value="";

    }
    else
    {
        displayObj('RealNameDL,UserPasswordDL,EmailDL');
        xajax.$('RealName').disabled="";
        xajax.$('UserPassword').disabled="";
        xajax.$('Email').disabled="";
    }
}
changeRPE();
//]]>
</script>
{/literal}
</body>
</html>
