global ctrlTCL
if {[info exists ctrlTCL]} {
  return
}
set ctrlTCL 1

source common.tcl

source nsos.tcl

proc errproc {msg} {
  if {[$::tmp getErrMsg] == "Session has been closed explicitly"} {
    $::tmp erase
    puts "Session has been closed explicitly"
    exit 0
  }
  return SKIP
}

proc loop {} {
  while 1 {
    set cmd [string trim [gets stdin]]
    if {[string first "::" $cmd] == 0} {s
      eval $::tmp [string range $cmd 2 end]
    } else {
      if {[string length $cmd] ==0} {
        set cmd " "
      }
      if {[regexp -nocase {exit|quit} $cmd]} {
        $::tmp erase
        puts "Session has been closed by user"
        exit 0
      } else {
        $::tmp execCmds [list $cmd]
      }
    }
  }
}

if {[llength $argv] == 2 && [regexp {nsos|junos} [lindex $argv 0]]} {
  if {[catch {
      set s [lindex $argv 0]
      set t [lindex $argv 1]
      source $s.tcl
      set cls [string toupper [lindex $argv 0]]
      set ::tmp [$cls $s\#auto $t]
      $::tmp setErrHandler ::errproc
      $::tmp openSession
    } ret]} {
    puts "Failed to create instance: $ret"
    exit -1
  } else {
    loop
  }
} else {
  puts "Wrong arguments: $argv"
  exit -1
}
