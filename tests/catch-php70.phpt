--TEST--
Test for jump calculation with catch()
--INI--
vld.active=1
--SKIPIF--
<?php if (PHP_VERSION_ID < 70000) { echo "skip PHP 7 required\n"; } ?> 
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
filename:       %scatch-php70.php
function name:  (null)
number of ops:  2
compiled vars:  none
line     #* E I O op                           fetch          ext  return  operands
-------------------------------------------------------------------------------------
   2     0  E >   NOP                                                      
  18     1      > RETURN                                                   1

Function foo:
Finding entry points
Branch analysis from position: 0
Jump found. Position 1 = 25
Branch analysis from position: 25
Jump found. Position 1 = -2
Found catch point at position: 10
Branch analysis from position: 10
Jump found. Position 1 = 11, Position 2 = 18
Branch analysis from position: 11
Jump found. Position 1 = 25
Branch analysis from position: 25
Branch analysis from position: 18
Jump found. Position 1 = 19, Position 2 = -2
Branch analysis from position: 19
Jump found. Position 1 = -2
Found catch point at position: 18
Branch analysis from position: 18
filename:       %scatch-php70.php
function name:  foo
number of ops:  27
compiled vars:  !0 = $complicated, !1 = $e
line     #* E I O op                           fetch          ext  return  operands
-------------------------------------------------------------------------------------
   4     0  E >   ASSIGN                                                   !0, 'simple'
   5     1        ROPE_INIT                                     3  ~4      'This+is+some+'
         2        ROPE_ADD                                      1  ~4      ~4, !0
         3        ROPE_END                                      2  ~3      ~4, '+text%0A'
         4        ECHO                                                     ~3
   7     5        ECHO                                                     'Time+for+exception+throwing%21%21%0A'
   8     6        INIT_FCALL_BY_NAME                                       'throwException'
         7        DO_FCALL_BY_NAME                                         
   9     8        ECHO                                                     'More+text%0A'
         9      > JMP                                                      ->25
  10    10  E > > CATCH                                                    'OtherException', !1, ->18
  11    11    >   ECHO                                                     'Caught+in+the+other+act%21%0A'
  12    12        INIT_FCALL                                               'var_dump'
        13        INIT_METHOD_CALL                                         !1, 'getMessage'
        14        DO_FCALL                                      0  $7      
        15        SEND_VAR                                                 $7
        16        DO_ICALL                                                 
        17      > JMP                                                      ->25
  13    18  E > > CATCH                                                    'Exception', !1, ->25
  14    19    >   ECHO                                                     'Caught+in+the+act%21%0A'
  15    20        INIT_FCALL                                               'var_dump'
        21        INIT_METHOD_CALL                                         !1, 'getMessage'
        22        DO_FCALL                                      0  $9      
        23        SEND_VAR                                                 $9
        24        DO_ICALL                                                 
  17    25    >   ECHO                                                     'And+the+end%0A'
  18    26      > RETURN                                                   null

End of function foo

branch: #  0; line:     2-   18; sop:     0; eop:     1; out1:  -2
path #1: 0, 
branch: #  0; line:     4-    9; sop:     0; eop:     9; out1:  25
branch: # 10; line:    10-   10; sop:    10; eop:    10; out1:  11; out2:  18
branch: # 11; line:    11-   12; sop:    11; eop:    17; out1:  25
branch: # 18; line:    13-   13; sop:    18; eop:    18; out1:  19; out2:  -2
branch: # 19; line:    14-   17; sop:    19; eop:    24; out1:  25
branch: # 25; line:    17-   18; sop:    25; eop:    26; out1:  -2
path #1: 0, 25, 
path #2: 10, 11, 25, 
path #3: 10, 18, 19, 25,
