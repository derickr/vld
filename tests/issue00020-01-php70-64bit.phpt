--TEST--
Test for issue #20
--INI--
vld.active=1
--SKIPIF--
<?php
if (PHP_VERSION_ID < 70000) { echo "skip PHP 7 required\n"; }
if (PHP_INT_SIZE != 8) { echo "skip 64bit test\n"; }
?>
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
filename:       %sissue00020-01-php70-64bit.php
function name:  (null)
number of ops:  10
compiled vars:  !0 = $x, !1 = $y, !2 = $z
line     #* E I O op                           fetch          ext  return  operands
-------------------------------------------------------------------------------------
   2     0  E >   ASSIGN                                                   !0, 2
   3     1        ROPE_INIT                                     3  ~5      '-'
         2        ROPE_ADD                                      1  ~5      ~5, !0
         3        ROPE_END                                      2  ~4      ~5, !0
         4        ASSIGN                                                   !1, ~4
   4     5        ROPE_INIT                                     3  ~9      '-'
         6        ROPE_ADD                                      1  ~9      ~9, !0
         7        ROPE_END                                      2  ~8      ~9, !0
         8        ASSIGN                                                   !2, ~8
         9      > RETURN                                                   1

branch: #  0; line:     2-    4; sop:     0; eop:     9; out1:  -2
path #1: 0,
