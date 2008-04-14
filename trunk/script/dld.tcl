global dldTCL
if {[info exists dldTCL]} {
  return
}
set dldTCL 1

package require cmdline

source common.tcl
source dm.tcl
source ds.tcl
source ms.tcl

global arrSeries
global arrGraph
global arrConsole
global arrAction
global arrProperty

global lstSeries
global lstGraph
global lstConsole
global lstAction

global xml

proc def {type args} {
  set types [list series graph console action demo]
  if {[lsearch $types $type] > -1} {
    eval $type $args
  } else {
    error "Wrong type with def command:\nValid type should be one of [join $types {, }]"
  }
}

proc series {args} {
  set options {
    {name.arg       }
    {description.arg}
    {xLabel.arg     }
    {yLabel.arg     }
    {yMax.arg       }
    {color.arg      }
    {type.arg       }
  }
  set usage ": def series \[options]\noptions:"
  if {[catch {::cmdline::getKnownOptions args $options $usage} ret]} {
    error "Wrong arguments for series definition\n$ret"
  } else {
    # add validation here
    array set params $ret
    if {[info exists ::arrSeries($params(name))]} {
      error "Reduplicate definition of series $params(name)"
    }
    set ::arrSeries($params(name)) "<ser\
                                    name=\"$params(name)\"\
                                    description=\"$params(description)\"\
                                    xLabel=\"$params(xLabel)\"\
                                    yLabel=\"$params(yLabel)\"\
                                    yMax=\"$params(yMax)\"\
                                    color=\"$params(color)\"\
                                    type=\"$params(type)\"\
                                    [eval series_$params(type) $args]\
                                    />"
    lappend ::lstSeries $params(name)
  }
}

proc series_snmp {args} {
  set options {
    {ip.arg       }
    {community.arg}
    {OID.arg      }
    {poll.arg     }
  }
  set usage ": def series_snmp \[options]\noptions:"
  if {[catch {::cmdline::getKnownOptions args $options $usage} ret]} {
    error "Wrong arguments for series snmp definition\n$ret"
  } else {
    # add validation here
    array set params $ret
    return "ip=\"$params(ip)\"\
            community=\"$params(community)\"\
            OID=\"$params(OID)\"\
            poll=\"$params(poll)\""
  }
}

proc graph {args} {
  set options {
    {name.arg }
    {title.arg}
    {type.arg }
  }
  set usage ": def graph \[options]\noptions:"
  if {[catch {::cmdline::getKnownOptions args $options $usage} ret]} {
    error "Wrong arguments for graph definition\n$ret"
  } else {
    # add validation here
    array set params $ret
    if {[info exists ::arrGraph($params(name))]} {
      error "Reduplicate definition of graph $params(name)"
    }
    set ::arrGraph($params(name)) "<graph\
                                   name=\"$params(name)\"\
                                   title=\"$params(title)\"\
                                   type=\"$params(type)\"\
                                   [eval graph_$params(type) $args]\
                                   />"
    lappend ::lstGraph $params(name)
  }
}

proc graph_gta {args} {
  set options {
    {series.arg}
    {style.arg }
  }
  set usage ": def graph_gta \[options]\noptions:"
  if {[catch {::cmdline::getKnownOptions args $options $usage} ret]} {
    error "Wrong arguments for graph gta definition\n$ret"
  } else {
    # add validation here
    array set params $ret
    return "series=\"$params(series)\"\
            style=\"$params(style)\"\
            [eval graph_$params(style) $args]"
  }
}

proc graph_gsa {args} {
  # add analysis here
  return ""
}

proc graph_gsb {args} {
  # add analysis here
  return ""
}

proc graph_pic {args} {
  set options {
    {picture.arg}
    {style.arg  }
  }
  set usage ": def graph_pic \[options]\noptions:"
  if {[catch {::cmdline::getKnownOptions args $options $usage} ret]} {
    error "Wrong arguments for graph pic definition\n$ret"
  } else {
    # add validation here
    array set params $ret
    return "picture=\"$params(picture)\""
  }
}

proc console {args} {
  set options {
    {name.arg }
    {title.arg}
    {type.arg }
  }
  set usage ": def console \[options]\noptions:"
  if {[catch {::cmdline::getKnownOptions args $options $usage} ret]} {
    error "Wrong arguments for console definition\n$ret"
  } else {
    # add validation here
    array set params $ret
    if {[info exists ::arrConsole($params(name))]} {
      error "Reduplicate definition of console $params(name)"
    }
    set ::arrConsole($params(name)) "<console\
                                     name=\"$params(name)\"\
                                     title=\"$params(title)\"\
                                     type=\"$params(type)\"\
                                     [eval console_$params(type) $args]\
                                     />"
    lappend ::lstConsole $params(name)
  }
}

proc console_normal {args} {
  set options {
    {device.arg}
  }
  set usage ": def console_normal \[options]\noptions:"
  if {[catch {::cmdline::getKnownOptions args $options $usage} ret]} {
    error "Wrong arguments for console normal definition\n$ret"
  } else {
    # add validation here
    array set params $ret
    return "device=\"$params(device)\""
  }
}

proc action {args} {
  set options {
    {name.arg       }
    {description.arg}
    {topology.arg   }
    {graphs.arg     }
    {consoles.arg   }
    {pre.arg        }
    {func.arg       }
    {depends.arg    }
  }
  set usage ": def action \[options]\noptions:"
  if {[catch {::cmdline::getKnownOptions args $options $usage} ret]} {
    error "Wrong arguments for action definition\n$ret"
  } else {
    # add validation here
    array set params $ret
    if {[info exists ::arrAction($params(name))]} {
      error "Reduplicate definition of action $params(name)"
    }
    proc tmp {p1 p2} {
      if {$p1 == "*"} {
        return [eval  join \$::lst$p2 {,}]
      }
      return $p1
    }
    set ::arrAction($params(name)) "<action\
                                    name=\"$params(name)\"\
                                    topology=\"$params(topology)\"\
                                    graphs=\"[tmp $params(graphs) Graph]\"\
                                    consoles=\"[tmp $params(consoles) Console]\"\
                                    pre=\"$params(pre)\"\
                                    func=\"$params(func)\"\
                                    depends=\"$params(depends)\"\
                                    >\n\
                                    $params(description)\n\
                                    </action>"
    lappend ::lstAction $params(name)
  }
}

proc demo {args} {
  set options {
    {brief.arg      }
    {detail.arg     }
    {topology.arg   }
    {series.arg     }
    {graphs.arg     }
    {consoles.arg   }
    {preparation.arg}
    {healthy.arg    }
    {actions.arg    }
    {provision.arg  }
  }
  set usage ": def demo \[options]\noptions:"
  if {[catch {::cmdline::getKnownOptions args $options $usage} ret]} {
    error "Wrong arguments for demo definition\n$ret"
  } else {
    # add validation here
    array set ::arrProperty $ret
    proc tmp {p1 p2} {
      if {![info exists ::arrProperty($p1)] || $::arrProperty($p1) == "*" || [string length $::arrProperty($p1)] == 0} {
        eval set ::arrProperty($p1) [eval  join \$::lst$p2 {,}]
      }
    }
    tmp series Series
    tmp graphs Graph
    tmp consoles Console
    tmp actions Action
    foreach value {preparation healthy provision} {
      if {![info exists ::arrProperty($value)] || [string length $::arrProperty($value)] == 0} {
        set ::arrProperty($value) $value
      }
    }
    set ::arrProperty(name) [file rootname $::argv0]
  }
}

proc main {args} {
  if {[llength $args] < 2 || ![file isfile [lindex $args 0]]} {
    ${::log}::error "Wrong arguments, should be: dld script action\n\
                     \tscript - demo script\n\
                     \taction - action to be executed"
    exit -1
  } else {
    set script [lindex $args 0]
    set action [lindex $args 1]
    ${::log}::debug "Try to execute demo script $script with $action"
    if {[catch {source $script} ret]} {
      ${::log}::error "Fail to load demo $script\n$ret $::errorInfo"
      exit -1
    }
    ${::log}::debug "Success to load demo $script"
    if {$action == "xml"} {
      ${::log}::debug "series:\n[arrayval ::arrSeries]"
      ${::log}::debug "graphs:\n[arrayval ::arrGraph]"
      ${::log}::debug "consoles:\n[arrayval ::arrConsole]"
      ${::log}::debug "actions:\n[arrayval ::arrAction]"
      ${::log}::debug "properties:\n[arrayval ::arrProperty]"
      set ::xml ""
      append ::xml "<demo name=\"$::arrProperty(name)\" topology=\"$::arrProperty(topology)\">\n"
      proc tmp1 {p1} {
        append ::xml "<$p1>\n"
        append ::xml " $::arrProperty($p1)\n"
        append ::xml "</$p1>\n"
      }
      tmp1 brief
      tmp1 detail
      proc tmp2 {p1 p2 p3} {
        append ::xml "<$p2>\n"
        foreach value [split $::arrProperty($p2) {,}] {
          append ::xml [subst [subst { \$::arr$p3\($value)\n}]]
        }
        append ::xml "</$p2>\n"
      }
      tmp2 ser series Series
      tmp2 graph graphs Graph
      tmp2 console consoles Console
      tmp1 preparation
      tmp1 healthy
      tmp2 action actions Action
      tmp1 provision
      append ::xml "</demo>"
      ${::log}::alert "Below is static information in xml format:\n$::xml"
      exit 0
    }
    if {[catch {eval $action} ret]} {
      ${::log}::error "Fail to invoke $action\n $ret $::errorInfo"
      exit -1
    }
    ${::log}::debug "Success to invoke $action"
    exit 0
  }
}

eval main $argv
