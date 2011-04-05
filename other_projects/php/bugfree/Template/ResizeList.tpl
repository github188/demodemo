{literal}
<script>
function resizeList()
{
var h1 = document.body.clientHeight;
var h2 = document.documentElement.clientHeight;
var h3 = (h2>h1)?h2:h1;
var hsub = 38;
(h2>h1&&h2!=0)?hsub=36:"";
(h2<h1&&h2!=0)?hsub=32:"";
xajax.$('ListSubTable').style.height = h3-hsub + 'px';
var w1 = document.body.clientWidth;
var w2 = document.documentElement.clientWidth;
xajax.$('ListSubTable').style.width = w1-12 + 'px';
}
</script>
{/literal}
