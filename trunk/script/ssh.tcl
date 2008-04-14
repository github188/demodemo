global sshTCL
if {[info exists sshTCL]} {
  return
}
set sshTCL 1

source exp.tcl

::itcl::class Ssh {
  inherit Exp

  private variable _host ""
  private variable _user ""
  private variable _pass ""
  private variable _pmpt ""

  constructor {host user pass {pmpt "\\[.+?@.+?\\].\\s*$"}} {
    set _host $host
    set _user $user
    set _pass $pass
    set _pmpt $pmpt
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

  # open ssh session
  public method openSession {} {
    if {[getID] != ""} {
        closeSession
    }
    doLogDebug "Open ssh session, target: $_host"
    spawn ssh $_user@$_host
    setID $spawn_id
    expect {
      -i [getID]
      -timeout [getTimeout]
      -nocase
      -re "\\(yes/no\\)\\?\\s*$" {
        exp_send -i [getID] "yes\r"
        exp_continue
      }
      -re "password:\\s*$" {
        exp_send -i [getID] "$_pass\r"
        exp_continue
      }
      -re "$_pmpt\\s*$" {
        execCmds [list "ps [exp_pid -i [getID]]"]
        doLogDebug "Success to open ssh session, target: $_host pid: [exp_pid -i [getID]]"
        return true
      }
      default {
        setID
        doLogError "Can not expect \"$_pmpt\\s*$\", target: $_host pid: [exp_pid -i [getID]]"
        return false
      }
      timeout {
        setID
        doLogError "Timeout here while opening ssh session, target: $_host pid: [exp_pid -i [getID]]"
        return false
      }
    }
  }

  # close ssh session
  public method closeSession {} {
    if {[getID] != ""} {
      doLogDebug "Close ssh session, target: $_host pid: [exp_pid -i [getID]]"
      postCmd "exit"
      catch {close -i [getID]}
      catch {exp_wait -i [getID]}
      setID
    }
    return true
  }
}
