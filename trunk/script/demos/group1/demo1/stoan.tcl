global stoanTCL
if {[info exists stoanTCL]} {
  return
}
set stoanTCL 1

# define vaiabiles
set m7ic "192.168.10.117"
set m7id "192.168.10.118"

set isg2ka "192.168.10.115"
set isg2kb "192.168.10.116"

set m7ic_ge030 "1.3.6.1.2.2.1.16.33"
set m7ic_ge031 "1.3.6.1.2.2.1.16.34"

set m7id_ge030 "1.3.6.1.2.2.1.16.78"
set m7id_ge031 "1.3.6.1.2.2.1.16.79"

set isg2kOID "a.b.c.d.e.f.g3"

# Notice:
# arguments for def are sequence sensitive

# define series
# name should not contain ","
# parameters after type are determined by type, now type can be snmp
def series -name series1 -description "ge-0/3/0" -xLabel "xLabel" -yLabel "yLabel" -yMax 100 -color blue -type snmp -ip $m7ic -community public -OID $m7ic_ge030 -poll 10
def series -name series2 -description "ge-0/3/1" -xLabel "xLabel" -yLabel "yLabel" -yMax 100 -color orange -type snmp -ip $m7ic -community public -OID $m7ic_ge031 -poll 10
def series -name series3 -description "ge-0/3/0" -xLabel "xLabel" -yLabel "yLabel" -yMax 100 -color blue -type snmp -ip $m7id -community public -OID $m7id_ge030 -poll 10
def series -name series4 -description "ge-0/3/1" -xLabel "xLabel" -yLabel "yLabel" -yMax 100 -color orange -type snmp -ip $m7id -community public -OID $m7id_ge031 -poll 10
def series -name series5 -description "ISG2000-A(Primary)" -xLabel "xLabel" -yLabel "yLabel" -yMax 100 -color blue -type snmp -ip $isg2ka -community public -OID $isg2kOID -poll 10
def series -name series6 -description "ISG2000-B(Backup)" -xLabel "xLabel" -yLabel "yLabel" -yMax 100 -color orange -type snmp -ip $isg2kb -community public -OID $isg2kOID -poll 10

# define graphs
# name should not contain "," or "|"
# parameters after type are determined by type, now type can be gta and pic
# parameters after style are determined by style, now style can be gsa and gsb
def graph -name graph1 -title "M7iDC-C Interfaces" -type gta -series "series1,series2" -style gsa
def graph -name graph2 -title "M7iDC-D Interfaces" -type gta -series "series3,series4" -style gsa
def graph -name graph3 -title "ISG Session Tables" -type gta -series "series5,series6" -style gsb
# def graph -name his -title "his" -type pic -picture "http://127.0.0.1/pic?node=abc&amp;type=his"

# define consoles
# name should not contain "," or "|"
# parameters after type are determined by type, now type can be normal (means control device via MS)
def console -name console1 -title "CLI Session - ISG2000-A" -type normal -device "nsos poc-isg2000-a"

# define actions
# graphs and consoles define associated graph pane and console pane
# depends defines dependency for certain action, action will not be enable unless its dependences have been executed
def action -name basic\
           -description "Here we can see that the primary eth1/1 infterface and the secondary interface eth1/2 both have an OSPF adjacency.\n\n\
                         \[Console] We can see that the learned default route is on the eth1/1 interface which connect to the M7iDC-C's ge-0/3/0."\
           -topology stoan1.jpg\
           -graphs "*"\
           -consoles "*"\
           -pre pre1\
           -func action1\
           -depends ""
def action -name failover\
           -description "Here we are going to fail interface eth1/1. This will force SPF to run and the new best path to be determined.\n\n\
                         \[Console] We disable the eth1/1 interface here this will force traffic to failover to the second interface eth1/2 which connects to M7iDC-D's ge-0/3/0.\n\n\
                         \[Graph Traffic] Traffic Starts Flowing through the eth1/2 interface connected to M7iDC-D's ge-0/3/0. Traffic Stops Flowing through M7iDC-C's ge-0/3/0 at the same time.\n\n\
                         \[Graph Session] The blue line here represents ISG2000-A's session table see that it is unchanged during the interface failover.\n\n\
                         \[Console] The default route points out the secondary interface is:\n\n\
                         \[Console] Here we show that the only OSPF neighbor active is via the eth1/2 interface:"\
           -topology stoan2.jpg\
           -graphs "*"\
           -consoles "*"\
           -pre pre2\
           -func action2\
           -depends "basic"
def action -name restore\
           -description "\[Console] The e1/1 interface is set to an up state:\n\n\
                         \[Console] We check and see that the default route still points out the eth1/2 interface:\n\n\
                         We timelaps the demo 30 seconds to show that after the OSPF neighbor relationship comes back up that the traffic begins to flow out the e1/1 interface.\n\n\
                         \[Graph Traffic] Traffic Starts Flowing out the eth1/1 interface. Traffic Stops Flowing out the eth1/2 interface\n\n\
                         \[Graph Session] No sessions were lost during this interface failback
                         "\
           -topology stoan1.jpg\
           -graphs "*"\
           -consoles "*"\
           -pre pre3\
           -func action3\
           -depends "failover"

# define static properties for demo
# series, graphs, consoles and actions will be all defined items if not specified
# preparation, healthy and provision are used to define corresponding functions and will be same name if not specified
def demo -brief "Interface Failure OSPF and NSRP"\
         -detail "This demo shows how OSPF can be used to reroute traffic around a failed interface.\n\n\
                  In this demo we fail an interface and watch OSPF reroute around the failure"\
         -topology stoan0.jpg\
         -series "*"\
         -graphs "*"\
         -consoles "*"\
         -preparation preparation\
         -healthy healthy\
         -actions "basic,failover,restore"\
         -provision provision

# define help-funtions
proc preparation {args} {
  set ret true
  ${::log}::info "Preparing"
  # add pre-configure here
  # ...
  ${::log}::info "Finish to prepare"
  if {$ret} {
    exit 0
  } else {
    exit -1
  }
}

proc healthy {args} {
  set ret true
  ${::log}::info "Checking health"
  # add health-check here
  # ...
  ${::log}::info "Finish to check health"
  if {$ret} {
    exit 0
  } else {
    exit -1
  }
}

proc provision {args} {
  set ret true
  ${::log}::info "Provisioning"
  # add provision here
  foreach device $args {
    # MS::provision $device
  }
  # ...
  ${::log}::info "Finish to provision"
  if {$ret} {
    exit 0
  } else {
    exit -1
  }
}

proc pre1 {args} {
  set ret true
  ${::log}::info "Preparing for action1"
  # add preparation here
  DM::activeGraph graph1
  DM::activeGraph graph2
  DM::activeGraph graph3
  DM::activeConsole console1
  # ...
  ${::log}::info "Finish to preparing for action1"
  if {$ret} {
    exit 0
  } else {
    exit -1
  }
}

proc action1 {args} {
  set ret true
  ${::log}::info "Start to run action1"
  # add business code here
  DM::popWin "graph1,graph3|graph2,console1"
  # ...
  ${::log}::info "Finish to run action1"
  if {$ret} {
    exit 0
  } else {
    exit -1
  }
}

proc pre2 {args} {
  set ret true
  ${::log}::info "Preparing for action2"
  # add preparation here
  # ...
  ${::log}::info "Finish to preparing for action2"
  if {$ret} {
    exit 0
  } else {
    exit -1
  }
}

proc action2 {} {
  set ret true
  ${::log}::info "Start to run action2"
  # add business code here
  # link down
  # ...
  ${::log}::info "Finish to run action2"
  if {$ret} {
    exit 0
  } else {
    exit -1
  }
}

proc pre3 {args} {
  set ret true
  ${::log}::info "Preparing for action3"
  # add preparation here
  # ...
  ${::log}::info "Finish to preparing for action3"
  if {$ret} {
    exit 0
  } else {
    exit -1
  }
}

proc action3 {} {
  set ret true
  ${::log}::info "Start to run action3"
  # add business code here
  # link up
  # ...
  ${::log}::info "Finish to run action3"
  if {$ret} {
    exit 0
  } else {
    exit -1
  }
}
