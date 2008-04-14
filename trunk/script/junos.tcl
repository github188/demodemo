global junosTCL
if {[info exists junosTCL]} {
  return
}
set junosTCL 1

source exp.tcl

::itcl::class JUNOS {
  inherit Exp

  private variable _host ""
  private variable _pmpt ""

  constructor {host} {
    set _host $host
    set _pmpt "\\S*@$host\[>#%\]\\s*\$"
  }

  public method defaultPmpt {} {
    return $_pmpt
  }

  public method setDefaultPmpt {pmpt} {
    set _pmpt $pmpt
  }

  public method getDefaultPmpt {} {
    return $_pmpt
  }

  # open screen os session
  public method openSession {} {
    if {[getID] != ""} {
        closeSession
    }
    doLogDebug "Open screen os session, target: $_host"
    spawn ssh -l admin magic tclsh /gamma/poc-connect.tcl ssh $_host
    setID $spawn_id
    expect {
      -i [getID]
      -timeout [getTimeout]
      -nocase
      -re "$_pmpt\\s*$" {
        doLogDebug "Success to open screen os session, target: $_host pid: [exp_pid -i [getID]]"
        return true
      }
      default {
        setID
        doLogError "Can not expect \"$_pmpt\\s*$\", target: $_host pid: [exp_pid -i [getID]]"
        return false
      }
      timeout {
        setID
        doLogError "Timeout here while opening screen os session, target: $_host pid: [exp_pid -i [getID]]"
        return false
      }
    }
  }

  # close screen os session
  public method closeSession {} {
    if {[getID] != ""} {
      doLogDebug "Close screen os session, target: $_host pid: [exp_pid -i [getID]]"
      postCmd "exit"
      catch {close -i [getID]}
      catch {exp_wait -i [getID]}
      setID
    }
    return true
  }
}
