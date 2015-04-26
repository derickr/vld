--TEST--
Test for issue #21
--INI--
vld.active=1
--SKIPIF--
<?php if (PHP_VERSION_ID < 70000) { echo "skip PHP 7 required\n"; } ?> 
--FILE--
<?php
for($i=0;$i<=2;$i++)
    echo $i;
?>
--EXPECTF--
Finding entry points
Branch analysis from position: 0
Jump found. Position 1 = 5
Branch analysis from position: 5
Jump found. Position 1 = 7, Position 2 = 2
Branch analysis from position: 7
Jump found. Position 1 = -2
Branch analysis from position: 2
Jump found. Position 1 = 7, Position 2 = 2
Branch analysis from position: 7
Branch analysis from position: 2
filename:       %sissue00021-php70.php
function name:  (null)
number of ops:  8
compiled vars:  !0 = $i
line     #* E I O op                           fetch          ext  return  operands
-------------------------------------------------------------------------------------
   2     0  E >   ASSIGN                                                   !0, 0
         1      > JMP                                                      ->5
   3     2    >   ECHO                                                     !0
   2     3        POST_INC                                         ~2      !0
         4        FREE                                                     ~2
         5    >   IS_SMALLER_OR_EQUAL                              ~3      !0, 2
         6      > JMPNZ                                                    ~3, ->2
         7    > > RETURN                                                   1

0branch: #  0; line:     2-    2; sop:     0; eop:     1; out1:   5
branch: #  2; line:     3-    2; sop:     2; eop:     4; out1:   5
branch: #  5; line:     2-    2; sop:     5; eop:     6; out1:   7; out2:   2
branch: #  7; line:     2-    2; sop:     7; eop:     7; out1:  -2
path #1: 0, 5, 7, 
path #2: 0, 5, 2, 5, 7, 
12
