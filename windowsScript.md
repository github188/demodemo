## Variable Edit/Replace ##

Use the syntax below to edit and replace the characters assigned to a string variable.

Syntax
> %variable:StrToFind=NewStr%
> %~[param\_ext](param_ext.md)$variable:Param
Key
> StrToFind    : The characters we are looking for
> NewStr       : The chars to replace with (if any)
> variable     : The environment variable
> param\_ext    : Any filename Parameter Extension
> Param        : A command line parameter (e.g. 1)

This syntax can be used anywhere that you would otherwise refer to the whole %variable% such as ECHOing the variable to screen, setting one variable = another

"StrToFind" can begin with an asterisk, in which case it will replace all characters to the left of "StrToFind".
By leaving NewStr blank, you can delete characters.

Using both an asterisk and NewStr=null effectively provides a left$() or right$() function.

Examples:
The variable _test containing 12345abcabc is used for all the following examples:_

::Replace '12345' with 'Hello '
> SET _test=12345abcabc
> SET_result=%_test:12345=Hello %
> ECHO %_result%          =Hello abcabc

::Replace the character string 'ab' with 'xy'
> SET _test=12345abcabc
> SET_result=%_test:ab=xy%
> ECHO %_result%          =12345xycxyc

::Delete the character string 'ab'
> SET _test=12345abcabc
> SET_result=%_test:ab=%
> ECHO %_result%          =12345cc

::Delete the character string 'ab' and everything before it
> SET _test=12345abcabc
> SET_result=%**test:_ab=%
> ECHO %_result%          =cabc**

::Replace the character string 'ab' and everything before it with 'XY'
> SET _test=12345abcabc
> SET_result=%**test:_ab=XY%
> ECHO %_result%          =XYcabc**


:: To remove characters from the right hand side of a string is
:: a two step process and requires the use of a CALL statement
:: e.g.

> SET _test=The quick brown fox jumps over the lazy dog_

> :: To delete everything after the string 'brown'
> :: first delete 'brown' and everything before it
> SET _endbit=%_test:_brown=%
> Echo We dont want: [%**endbit%]**

> ::Now remove this from the original string
> CALL SET_result=%%_test:%_endbit%=%%
> echo %_result%_

## Remove spaces from a text string ##

To delete space characters use the same syntax as above:

SET _no\_spaces=%_some\_var: =%


Boolean Test "does string exist ?"

To test for the existence of a value we can replace the value with itself, placing the result in a second variable and then testing that variable with EQU

Example: test for the existence of the string "London" in a variable containing text (that could be in any order) "Aberdeen, London, Edinburgh"

Set _cities="Aberdeen, London, Edinburgh"_

:: Remove characters before 'London'
Set _endbit=%_cities:**London=London%**

:: Remove characters after 'London' (the text 'London' is 6 chars long)
SET _result=%_endbit:~0,6%
IF %_result% EQU London ECHO London was found_

## Finding items within the PATH environment variable ##

The %PATH% variable contains a list of folder names.

If you have a parameter containing a valid 'folder' this can be compared with the PATH variable.

This is done using the syntax: $variable:parameter

Example

%PATH% = C:\Windows\system32;C:\WINNT;C:\utils\jdk\bin
batch parameter %1 = C:\utils\jdk\bin

To get the drive and Path
ECHO %~dp$PATH:1
This will either return "C:\utils\jdk\bin" or a NULL if the item is not found in the %PATH%

If the batch parameter was supplied as %2 then this would be: ECHO %~dp$PATH:2

This syntax can be applied where:

  * he parameter is any valid parameter (%1 %2 %G) but it must contain a Full Path (not a pathname)
  * he variable is %PATH% or any other variable that contains one or more Paths or pathnames separated by semicolons ;
  * f nothing is found by the search, then this will return an empty string (NULL)

Be wary of using the syntax on this page to modify the PATH - the User path can be edited, but the System path remains read-only for most users.

"A phony smile will never replace honest integrity" - Bob Martinelli

Related:

## SUBSTRING of a variable :~ ##

PARAMETERS - Filename Parameter Extensions

PATHMAN - This Resource Kit utility allows quick modification of both the system and user paths. Pathman can resolve many problems such as duplicate characters, and can improve performance by removing duplicate paths. For details see Pathman.wri in the resource kit.
