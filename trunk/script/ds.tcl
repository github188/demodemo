global dsTCL
if {[info exists dsTCL]} {
  return
}
set dsTCL 1

# ds related
# APIs provided by ds

# proc login {user} {}
# proc demoInfo {demo} {}
# proc demoLock {demo} {}
# proc demoExec {action} {}
# proc demoProvision {device} {}
# proc demoRelease {} {}

namespace eval DS {
  namespace export lazyProvision
}

proc DS::lazyProvision {device} {
  ${::log}::alert "api call DS::demoProvision $device"
}
