--TEST--
Test for issue #20
--INI--
vld.active=1
--SKIPIF--
<?php if (PHP_VERSION_ID < 70000) { echo "skip PHP 7 required\n"; } ?> 
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
filename:       %sissue00020-01-php70.php
function name:  (null)
number of ops:  10
compiled vars:  !0 = $x, !1 = $y, !2 = $z
line     #* E I O op                           fetch          ext  return  operands
-------------------------------------------------------------------------------------
   2     0  E >   ASSIGN                                                   !0, 2
   3     1        ROPE_INIT                                     0  ~4      45
         2        ROPE_END                                      0  ~4      ~4, !0
         3        ROPE_END                                      0  ~4      ~4, !0
         4        ASSIGN                                                   !1, ~4
   4     5        ROPE_INIT                                     0  ~6      45
         6        ROPE_END                                      0  ~6      ~6, !0
         7        ROPE_END                                      0  ~6      ~6, !0
         8        ASSIGN                                                   !2, ~6
         9      > RETURN                                                   1

branch: #  0; line:     2-    4; sop:     0; eop:     9; out1:  -2
path #1: 0,
