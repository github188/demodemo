global msTCL
if {[info exists msTCL]} {
  return
}
set msTCL 1

# ms related
# needs dld to handle the output

namespace eval MS {
  namespace export initTestbed chkTestbed
  namespace export connect disconnect prompt execute powerOn powerOff reboot
  namespace export provision
  namespace export call
}

proc MS::initTestbed {name} {

}

proc MS::chkTestbed {name} {

}

proc MS::connect {device} {

}

proc MS::disconnect {device} {

}

proc MS::prompt {session} {

}

proc MS::execute {session command} {

}

proc MS::powerOn {device} {

}

proc MS::powerOff {device} {

}

proc MS::reboot {device} {

}

proc MS::provision {device} {

}

proc MS::call {args} {

}