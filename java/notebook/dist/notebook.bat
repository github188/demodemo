@echo off
rem 
rem Copyright (C) 2008 DeonWu@gmail.com
rem
rem This file is part of Js-Shell. Js-Shell is a set of library for running
rem javascript in Rhino.
rem
rem Js-Shell is free software: you can redistribute it and/or modify
rem it under the terms of the GNU General Public License as published by
rem the Free Software Foundation, either version 3 of the License, or
rem (at your option) any later version.
rem
rem Js-Shell is distributed in the hope that it will be useful,
rem but WITHOUT ANY WARRANTY; without even the implied warranty of
rem MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
rem GNU General Public License for more details.
rem 
rem You should have received a copy of the GNU General Public License
rem along with Js-Shell.  If not, see <http://www.gnu.org/licenses/>.
rem
rem $ Name LastChangeRevision LastChangeDate LastChangeBy $
rem $Id: js.bat 65 2008-08-28 09:40:38Z DeonWu $
rem 


rem set JS_HOME=%~dp0
rem echo %JS_HOME%

set JS_HOME=%~dp0..
java -jar %~dp0notebook-0.1.jar