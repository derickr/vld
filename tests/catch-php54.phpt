--TEST--
Test for jump calculation with catch()
--INI--
vld.active=1
--SKIPIF--
<?php if (PHP_VERSION_ID < 50400 || PHP_VERSION_ID >= 70000) { echo "skip PHP 5.[456] required\n"; } ?> 
--FILE--
<?php
function foo()
{
	$complicated = 'simple';
	echo "This is some {$complicated} text\n";
	try {
		echo "Time for exception throwing!!\n";
		throwException();
		echo "More text\n";
	} catch (OtherException $e) {
		echo "Caught in the other act!\n";
		var_dump($e->getMessage());
	} catch (Exception $e) {
		echo "Caught in the act!\n";
		var_dump($e->getMessage());
	}
	echo "And the end\n";
}
?>
--EXPECTF--
Finding entry points
Branch analysis from position: 0
Jump found. Position 1 = -2
filename:       %scatch-php54.php
function name:  (null)
number of ops:  2
compiled vars:  none
line     #* E I O op                           fetch          ext  return  operands
-------------------------------------------------------------------------------------
   2     0  E >   NOP                                                      
  20     1      > RETURN                                                   1

Function foo:
Finding entry points
Branch analysis from position: 0
Jump found. Position 1 = 23
Branch analysis from position: 23
Jump found. Position 1 = -2
Found catch point at position: 10
Branch analysis from position: 10
Jump found. Position 1 = 11, Position 2 = 17
Branch analysis from position: 11
Jump found. Position 1 = 23
Branch analysis from position: 23
Branch analysis from position: 17
Jump found. Position 1 = 18, Position 2 = -2
Branch analysis from position: 18
Jump found. Position 1 = -2
Found catch point at position: 17
Branch analysis from position: 17
filename:       %scatch-php54.php
function name:  foo
number of ops:  25
compiled vars:  !0 = $complicated, !1 = $e
line     #* E I O op                           fetch          ext  return  operands
-------------------------------------------------------------------------------------
   4     0  E >   ASSIGN                                                   !0, 'simple'
   5     1        ADD_STRING                                       ~1      'This+is+some+'
         2        ADD_VAR                                          ~1      ~1, !0
         3        ADD_STRING                                       ~1      ~1, '+text%0A'
         4        ECHO                                                     ~1
   7     5        ECHO                                                     'Time+for+exception+throwing%21%21%0A'
   8     6        INIT_FCALL_BY_NAME                                       'throwException'
         7        DO_FCALL_BY_NAME                              0          
   9     8        ECHO                                                     'More+text%0A'
  10     9      > JMP                                                      ->23
        10  E > > CATCH                                                    'OtherException', !1, ->17
  11    11    >   ECHO                                                     'Caught+in+the+other+act%21%0A'
  12    12        INIT_METHOD_CALL                                         !1, 'getMessage'
        13        DO_FCALL_BY_NAME                              0  $4      
        14        SEND_VAR_NO_REF                               6          $4
        15        DO_FCALL                                      1          'var_dump'
  13    16      > JMP                                                      ->23
        17  E > > CATCH                                                    'Exception', !1, ->23
  14    18    >   ECHO                                                     'Caught+in+the+act%21%0A'
  15    19        INIT_METHOD_CALL                                         !1, 'getMessage'
        20        DO_FCALL_BY_NAME                              0  $7      
        21        SEND_VAR_NO_REF                               6          $7
        22        DO_FCALL                                      1          'var_dump'
  17    23    >   ECHO                                                     'And+the+end%0A'
  18    24      > RETURN                                                   null

End of function foo

branch: #  0; line:     2-   20; sop:     0; eop:     1; out1:  -2
path #1: 0, 
branch: #  0; line:     4-   10; sop:     0; eop:     9; out1:  23
branch: # 10; line:    10-   10; sop:    10; eop:    10; out1:  11; out2:  17
branch: # 11; line:    11-   13; sop:    11; eop:    16; out1:  23
branch: # 17; line:    13-   13; sop:    17; eop:    17; out1:  18; out2:  -2
branch: # 18; line:    14-   17; sop:    18; eop:    22; out1:  23
branch: # 23; line:    17-   18; sop:    23; eop:    24; out1:  -2
path #1: 0, 23, 
path #2: 10, 11, 23, 
path #3: 10, 17, 18, 23,
