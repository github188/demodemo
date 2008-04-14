global objTCL
if {[info exists objTCL]} {
  return
}
set objTCL 1

package require Itcl

::itcl::class Object {
  private variable _fullName   ""

  private variable _logger     ""

  private variable _errHandler ""
  private variable _errMsg     ""
  private variable _errDetail  ""

  constructor {} {
    set _fullName $this
  }

  destructor {
    clean
  }

  public method getFullName {} {
    return $_fullName
  }

  public method setLogger {logger} {
    set _logger $logger
  }

  public method getLogger {} {
    return $_logger
  }

  public method doLogDebug {msg} {
    if {[getLogger] != ""} {
      catch {eval [getLogger]::debug {$msg}}
    }
  }

  public method doLogError {msg} {
    if {[getLogger] != ""} {
      catch {eval [getLogger]::error {$msg}}
    }
  }

  # set the error handler which will be called when error occurs
  public method setErrHandler {errHandler} {
    set _errHandler $errHandler
  }

  public method getErrHandler {} {
    return $_errHandler
  }

  # set the error message
  public method setErrMsg {errMsg} {
    set _errMsg $errMsg
  }

  public method getErrMsg {} {
    return $_errMsg
  }

  # set the detail of the error message
  public method setErrDetail {errDetail} {
    set _errDetail $errDetail
  }

  public method getErrDetail {} {
    return $_errDetail
  }

  # be called when error occurs and error detail will be passed to error handler
  public method dispatchErr {} {
    if {[getErrHandler] == ""} {
	    doLogError "Fail to dispatch error - no error handler:\n\
                  msg - [getErrMsg]\n\
                  err - [getErrDetail]"
      return ABORT
    }
    set ret [eval [getErrHandler] {[getErrDetail]}]
    doLogDebug "Dispatch error [getErrHandler]\n\
                msg - [getErrMsg]\n\
                err - [getErrDetail]\n\
                return value is - $ret"
    switch -- $ret {
      REDO {
        return REDO
      }
      SKIP {
        return SKIP
      }
      ABORT {
        return ABORT
      }
      default {
        return ABORT
      }
    }
  }

  # clean up, sub-object which need clean should use "chain" before return
  public method clean {} {
    return true
  }

  # erase this
  public method erase {} {
    catch {::itcl::delete object [getFullName]}
    return true
  }

  # find object
  public proc exists {obj {wild 0}} {
    if {$wild} {
      return [llength [::itcl::find objects "$obj"]]
    } else {
      return [llength [::itcl::find objects "$obj" -isa Object]]
    }
  }

  public proc newInstance {class name args} {
    return [[eval $class $name $args] getFullName]
  }
}
