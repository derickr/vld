--TEST--
Test for issue #20
--INI--
vld.active=1
--SKIPIF--
<?php if (PHP_VERSION_ID < 70000) { echo "skip PHP 7 required\n"; } ?> 
--FILE--
<?php
$x = 2;
$z = "-${x}${x}" <=> 1;
?>
--EXPECTF--
Finding entry points
Branch analysis from position: 0
Jump found. Position 1 = -2
filename:       %sissue00020-02-php70.php
function name:  (null)
number of ops:  7
compiled vars:  !0 = $x, !1 = $z
line     #* E I O op                           fetch          ext  return  operands
-------------------------------------------------------------------------------------
   2     0  E >   ASSIGN                                                   !0, 2
   3     1        ROPE_INIT                                     0  ~3      45
         2        ROPE_END                                      0  ~3      ~3, !0
         3        ROPE_END                                      0  ~3      ~3, !0
         4        SPACESHIP                                        ~4      ~3, 1
         5        ASSIGN                                                   !1, ~4
         6      > RETURN                                                   1

branch: #  0; line:     2-    3; sop:     0; eop:     6; out1:  -2
path #1: 0,
