global dmTCL
if {[info exists dmTCL]} {
  return
}
set dmTCL 1

# gui related
# needs dld to handle the output

namespace eval DM {
  namespace export popWeb popVNC popFlash
  namespace export activeGraph activeConsole
  namespace export switchTopo
}

# Notice:
# DM will take care all windows populated by popXXX and will not re-open certain window unless it has been closed by user

# pop-up a web page
proc DM::popWeb {url} {
  ${::log}::alert "api call DM::popup Page $url"
}

# pop-up a vnc window
proc DM::popVNC {target passwd} {
  ${::log}::alert "api call DM::popup VNC $target $passwd"
}

# pop-up a flash window
proc DM::popFlash {flash} {
  ${::log}::alert "api call DM::popup Flash $flash"
}

# pop-up a composite window (graph or console)
# para defines position, for example: g1,g2|c
# +-----------+
# +- g1 - g2 -+
# +-    c    -+
# +-----------+
# Notice:
# this requires graph and console have unique name
proc DM::popWin {para} {
  ${::log}::alert "api call DM::popup Win $para"
}

# let DS to retrieve data for certain graph
# cls means clear historical data or not
proc DM::activeGraph {graph {cls 0}} {
  ${::log}::alert "api call DM::active Graph $graph $cls"
}

# let DS to retrieve data for certain console
# cls means clear historical data or not
proc DM::activeConsole {console {cls 0}} {
  ${::log}::alert "api call DM::active Console $console $cls"
}

# change topology on demo page
proc DM::switchTopo {topo} {
  ${::log}::alert "api call DM::switch Topo $topo"
}
