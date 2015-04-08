--TEST--
Test for issue #19
--INI--
vld.active=1
--SKIPIF--
<?php if (PHP_VERSION_ID >= 50300) { echo "skip PHP 5.2 required\n"; } ?> 
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
filename:       %sissue00020-01-php52.php
function name:  (null)
number of ops:  13
compiled vars:  !0 = $x, !1 = $y, !2 = $z
line     #* E I O op                           fetch          ext  return  operands
-------------------------------------------------------------------------------------
   2     0  E >   ASSIGN                                                   !0, 2
   3     1        INIT_STRING                                      ~1      
         2        ADD_CHAR                                         ~1      ~1, 45
         3        ADD_VAR                                          ~1      ~1, !0
         4        ADD_VAR                                          ~1      ~1, !0
         5        ASSIGN                                                   !1, ~1
   4     6        INIT_STRING                                      ~3      
         7        ADD_CHAR                                         ~3      ~3, 45
         8        ADD_VAR                                          ~3      ~3, !0
         9        ADD_VAR                                          ~3      ~3, !0
        10        ASSIGN                                                   !2, ~3
   6    11      > RETURN                                                   1
        12*     > HANDLE_EXCEPTION                                         

branch: #  0; line:     2-    6; sop:     0; eop:    12
path #1: 0,
