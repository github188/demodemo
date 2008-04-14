global commonTCL
if {[info exists commonTCL]} {
  return
}
set commonTCL 1

package require logger::utils

set log [logger::init DL]
logger::utils::applyAppender -appender console\
                             -serviceCmd $log\
                             -appenderArgs {-conversionPattern {\[%p] %m \n\t}} ;#{\[%d] \[%c] \[%M] \[%p] %m}

proc arrayval {array {pattern *}} {
  upvar $array a
  if {![array exists a]} {
    error "\"$array\" isn't an array"
  }
  set lines [list]
  set max 0
  foreach name [array names a $pattern] {
    set len [string length $name]
    if {$len > $max} {
      set max $len
    }
  }
  set max [expr {$max + [string length $array] + 2}]
  foreach name [array names a $pattern] {
    set line [format %s(%s) $array $name]
    lappend lines [format "%-*s = %s" $max $line $a($name)]
  }
  return [join [lsort $lines] \n]
}
