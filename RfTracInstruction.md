#Rftrac instruction and feature list

# Introduction #
[RfTrac](http://deonwu84.appspot.com/rf_trac/) is a web tool to track down test log of robot framework. it provide a central database to manage the logs. it's used a JS plug-in to collect comments from distributed local log, and supports to diff, statistics testcase log.

# Project purpose #
it's not a standalone project, but a sub application of personal website. it's only supported to host in google-app-engine.
  * learn JSI to manage large javascript project.
  * learn javascript to cross domain access data.
  * learn google-app-engine in python
  * learn open web API design pattern.
  * learn restful
  * cache page with HTTP header

# RfTrac feature #
  * **central database** -- all of test result is imported to one database. it's simple to diff and statistics the result.
  * **comments** -- it's a JS plug-in to sort and comment the test log. the category is defined for per project.
  * **diff log** -- To find the last test result with same SUT, version, test user.
  * **statistics** -- statistics result by testcase, project, tags, SUT, e.g.
  * ...