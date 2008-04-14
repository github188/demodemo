global expTCL
if {[info exists expTCL]} {
  return
}
set expTCL 1

package require Expect

source obj.tcl

::itcl::class Exp {
  inherit Object

  private variable _id        ""
  private variable _timeout   5
  private variable _sleeptime 0

  public method defaultPmpt {} {}
  public method openSession {} {}
  public method closeSession {} {}

  # set the spawn id returned by expect
  public method setID {{id ""}} {
    set _id $id
  }

  public method getID {} {
    return $_id
  }

  # set the timeout value for expect
  public method setTimeout {{timeout 5}} {
    set _timeout $timeout
  }

  public method getTimeout {} {
    return $_timeout
  }

  # set the sleep time value for lazy exp_send
  public method setSleeptime {{sleeptime 1}} {
    set _sleeptime $sleeptime
  }

  public method getSleeptime {} {
    return $_sleeptime
  }

  # post an command and return immediately
  public method postCmd {cmd} {
    if {[getID] == ""} {
      return false
    }
    set send_slow {10 .001}
    exp_send -i [getID] -s -- "$cmd\r"
    return true
  }

  # execute command
  public method execCmd {cmd pmpt {noerror 0}} {
    if {[getID] == ""} {
      return false
    }
    set send_slow {10 .001}
    exp_send -i [getID] -s -- "$cmd\r"
    expect {
      -i [getID]
      -timeout [getTimeout]
      -nocase
      -re $pmpt {
        setErrDetail $expect_out(buffer)
        return true
      }
      timeout {
        if {$noerror} {
          return true
        }
        doLogDebug "Can not expect \"$pmpt\" while sending \"$cmd\""
        setErrMsg "Can not expect \"$pmpt\" while sending \"$cmd\""
        expect {
          -i [getID]
          * {
            setErrDetail $expect_out(buffer)
          }
        }
        return false
      }
      eof {
        setID
        doLogDebug "Session has been closed explicitly"
        setErrMsg "Session has been closed explicitly"
        setErrDetail $expect_out(buffer)
        dispatchErr
        return true
      }
    }
  }

  # do a batch of executions
  public method execCmds {cmds} {
    if {[getID] == ""} {
      if {[openSession]} {
        set state 1
      } else  {
        return false
      }
    } else {
      set state 0
    }
    set round 0
    set ret true
    for {set ix 0} {$ix < [llength $cmds]} {incr ix} {
      if {![execCmd [lindex $cmds $ix] [defaultPmpt]]} {
        switch [dispatchErr] {
          REDO {
            if {$round} {
              set ret false
              break
            }
            incr ix -1
            incr round
          }
          SKIP {

          }
          ABORT {
            set ret false
            break
          }
        }
      }
    }
    if {$state} {
      if {![closeSession]} {
        set ret false
      }
    }
    return $ret
  }

  # same as execCmds but need specify the pair of command and prompt
  public method execCmdsEx {cmds} {
    if {[getID] == ""} {
      if {[openSession]} {
        set state 1
      } else  {
        return false
      }
    } else {
      set state 0
    }
    set round 0
    set ret true
    for {set ix 0} {$ix < [llength $cmds]} {incr ix} {
      if {![execCmd [lindex $cmds $ix] [lindex $cmds [incr ix]]]} {
        switch [dispatchErr] {
          REDO {
            if {$round} {
              set ret false
              break
            }
            incr ix -2
            incr round
          }
          SKIP {

          }
          ABORT {
            set ret false
            break
          }
        }
      }
    }
    if {$state} {
      if {![closeSession]} {
        set ret false
      }
    }
    return $ret
  }

  # clean up the session
  public method clean {} {
    closeSession
    chain
    return true
  }
}
