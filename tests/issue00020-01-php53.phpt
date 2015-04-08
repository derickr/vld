--TEST--
Test for issue #20
--INI--
vld.active=1
--SKIPIF--
<?php if (PHP_VERSION_ID < 50300 || PHP_VERSION_ID >= 70000) { echo "skip PHP 5.[3456] required\n"; } ?> 
--FILE--
<?php
$x = 2;
$y = "-{$x}{$x}";
$z = "-${x}${x}";
?>
--EXPECTF--
Finding entry points
Branch analysis from position: 0
Jump found. Position 1 = -2
filename:       %sissue00020-01-php53.php
function name:  (null)
number of ops:  10
compiled vars:  !0 = $x, !1 = $y, !2 = $z
line     #* E I O op                           fetch          ext  return  operands
-------------------------------------------------------------------------------------
   2     0  E >   ASSIGN                                                   !0, 2
   3     1        ADD_CHAR                                         ~1      45
         2        ADD_VAR                                          ~1      ~1, !0
         3        ADD_VAR                                          ~1      ~1, !0
         4        ASSIGN                                                   !1, ~1
   4     5        ADD_CHAR                                         ~3      45
         6        ADD_VAR                                          ~3      ~3, !0
         7        ADD_VAR                                          ~3      ~3, !0
         8        ASSIGN                                                   !2, ~3
   6     9      > RETURN                                                   1

branch: #  0; line:     2-    6; sop:     0; eop:     9; out1:  -2
path #1: 0,
