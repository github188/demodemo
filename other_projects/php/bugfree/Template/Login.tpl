<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset={$Lang.Charset}" />
<meta http-equiv="Content-Language" content="{$Lang.Charset}" />
<link href="Css/Login.css" rel="stylesheet" type="text/css" />
{$xajax_javascript}
<script src="JS/Main.js" type="text/javascript"></script>
<title>{$HeaderTitle|default:$Lang.ProductName}</title>
<base target="{$BaseTarget|default:"RightBottomFrame"}">
</head>

<body>

<div id="LoginContainer">
    <img src="Image/login_bg_left.gif" class="loginBgImage"/>
    <div id="LoginMain">
        <div id="LoginLogo">
            <span id="Version">{$Lang.Version}</span>
        </div>
        <div id="LoginFormContainer">
            <form method="post" id="LoginForm" onsubmit="xajax_xCheckUserLogin(xajax.getFormValues('LoginForm'));return false;">
            <p>
                <label for="TestUserName"><strong id="ForTestUserName">{$Lang.TestUserName}</strong></label>
                <input type="text" name="TestUserName" class="TxtInput" id="TestUserName" value="{$TestUserName}" />
            </p>

            <p>
                <label for="TestUserPWD"><strong id="ForTestUserPWD">{$Lang.TestUserPWD}</strong></label>
                <input type="password" name="TestUserPWD" class="TxtInput" id="TestUserPWD" value="" />
            </p>
            <p>
                <label for="Language"><strong id="ForLanguage">{$Lang.SelectLanguage}</strong></label>
                {$LanguageList}
            </p>

            <p>
                <label>&nbsp;</label>
                <input type="checkbox" id="RememberLoginStatus" name="RememberLoginStatus" value="1" {if $RememberStatus eq 1}checked="checked"{/if} /><span id="ForRememberMe" style="margin:20px 0 0">{$Lang.RememberMe}</span>
            </p>
            <p>
                <input type="submit" id="SubmitLoginBTN" value="{$Lang.ButtonLogin}" accesskey="L" class="loginSubmit" />
                <input type="hidden" name="HttpRefer"  value="{$HttpRefer}">
            </p>

            <div id="ActionMessage" class="{$MessageType|default:"notice"}" style="display:none;"></div>
            </form>
        </div>
    </div>
    <img src="Image/login_bg_right.gif" class="loginBgImage"/>
    <br class="clear" />
    <div id="shadow">
      <img src="Image/login_shadow_left.gif" class="loginBgImage"/>
      <div id="ShadowCenter">
        <center>
        <br/>
        </center>
        <center>
        <br/>
        </center>
      </div>
      <img src="Image/login_shadow_right.gif" class="loginBgImage"/>
    </div>
</div>

<script defer type="text/javascript" src="JS/pngfix.js"></script>

{literal}
<script type="text/javascript">
//<![CDATA[
if(TestCookie.read('BFUserName') == '')
{
    xajax.$('TestUserName').focus();
}
else
{
    xajax.$('TestUserPWD').focus();
}
//]]>
</script>
{/literal}
</body>
</html>