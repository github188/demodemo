<%args>
$data
$cols=>3
$attr=>undef
</%args>
<%init>

my ($count, $cur, $start_row, $end_row, $padding) = (0, 0, 1, 0, 0);
$count = @{$data};
$padding = $cols - $count % $cols if $count % $cols != 0;

#print "count=$count, padding=$padding";
</%init>

<table border="0" <% $attr %>>
% foreach (@{$data}) {
% print "<tr>" if $start_row;
<td> <% $m->content |n %></td>
% print "</tr>" if $end_row;
% $cur++;
% $start_row = $cur % $cols == 0;
% $end_row = ($cur + 1) % $cols == 0;
% }
% print "<td></td>" x $padding . "</tr>" if $padding;
</table>