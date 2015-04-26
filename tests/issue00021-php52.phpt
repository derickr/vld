--TEST--
Test for issue #21
--INI--
vld.active=1
--SKIPIF--
<?php if (PHP_VERSION_ID >= 50300) { echo "skip PHP 5.2 required\n"; } ?> 
--FILE--
<?php
for($i=0;$i<=2;$i++)
    echo $i;
?>
--EXPECTF--
Finding entry points
Branch analysis from position: 0
Jump found. Position 1 = 8, Position 2 = 6
Branch analysis from position: 8
Jump found. Position 1 = -2
Branch analysis from position: 6
Jump found. Position 1 = 3
Branch analysis from position: 3
Jump found. Position 1 = 1
Branch analysis from position: 1
filename:       %sissue00021-php52.php
function name:  (null)
number of ops:  10
compiled vars:  !0 = $i
line     #* E I O op                           fetch          ext  return  operands
-------------------------------------------------------------------------------------
   2     0  E >   ASSIGN                                                   !0, 0
         1    >   IS_SMALLER_OR_EQUAL                              ~1      !0, 2
         2      > JMPZNZ                                        6          ~1, ->8
         3    >   POST_INC                                         ~2      !0
         4        FREE                                                     ~2
         5      > JMP                                                      ->1
   3     6    >   ECHO                                                     !0
         7      > JMP                                                      ->3
   5     8    > > RETURN                                                   1
         9*     > HANDLE_EXCEPTION                                         

0branch: #  0; line:     2-    2; sop:     0; eop:     0; out1:   1
branch: #  1; line:     2-    2; sop:     1; eop:     2; out1:   8; out2:   6
branch: #  3; line:     2-    2; sop:     3; eop:     5; out1:   1
branch: #  6; line:     3-    3; sop:     6; eop:     7; out1:   3
branch: #  8; line:     5-    5; sop:     8; eop:     9
path #1: 0, 1, 8, 
path #2: 0, 1, 6, 3, 1, 8, 
12
