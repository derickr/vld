--TEST--
Test for issue #19
--SKIPIF--
<?php if (PHP_VERSION_ID < 70000) { echo "skip PHP 7.0 required\n"; } ?> 
--INI--
vld.active=1
--FILE--
<?php
function foo($x, $y) {
    return $x-$y;
}
?>
--EXPECTF--
Finding entry points
Branch analysis from position: 0
Jump found. Position 1 = -2
filename:       %sissue00019-php70.php
function name:  (null)
number of ops:  2
compiled vars:  none
line     #* E I O op                           fetch          ext  return  operands
-------------------------------------------------------------------------------------
   2     0  E >   NOP                                                      
   4     1      > RETURN                                                   1

Function foo:
Finding entry points
Branch analysis from position: 0
Jump found. Position 1 = -2
filename:       %sissue00019-php70.php
function name:  foo
number of ops:  5
compiled vars:  !0 = $x, !1 = $y
line     #* E I O op                           fetch          ext  return  operands
-------------------------------------------------------------------------------------
   2     0  E >   RECV                                             !0      
         1        RECV                                             !1      
   3     2        SUB                                              ~2      !0, !1
         3      > RETURN                                                   ~2
   4     4*     > RETURN                                                   null

End of function foo

branch: #  0; line:     2-    4; sop:     0; eop:     1; out1:  -2
path #1: 0, 
branch: #  0; line:     2-    4; sop:     0; eop:     4
path #1: 0,
