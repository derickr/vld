--TEST--
Test for jump calculation with foreach()
--INI--
vld.active=1
--SKIPIF--
<?php if (PHP_VERSION_ID < 50400 || PHP_VERSION_ID >= 70000) { echo "skip PHP 5.[456] required\n"; } ?> 
--FILE--
<?php
class ComposerAutoloaderInit0d37b910670b3f7ddfa6b4516753af64
{
    public static function getLoader()
    {
		$map = [];

        foreach ($map as $namespace => $path) {
            echo $namespace, ': ', $path, "\n";
        }

        foreach ($map as $namespace => $path) {
            echo $namespace, ': ', $path, "\n";
        }

    }
}
?>
--EXPECTF--
Finding entry points
Branch analysis from position: 0
Jump found. Position 1 = -2
filename:       %sforeach-jumps-php54.php
function name:  (null)
number of ops:  2
compiled vars:  none
line     #* E I O op                           fetch          ext  return  operands
-------------------------------------------------------------------------------------
   3     0  E >   NOP                                                      
  19     1      > RETURN                                                   1

Class ComposerAutoloaderInit0d37b910670b3f7ddfa6b4516753af64:
Function getloader:
Finding entry points
Branch analysis from position: 0
Jump found. Position 1 = 3, Position 2 = 12
Branch analysis from position: 3
Jump found. Position 1 = 4, Position 2 = 12
Branch analysis from position: 4
Jump found. Position 1 = 3
Branch analysis from position: 3
Branch analysis from position: 12
Jump found. Position 1 = 14, Position 2 = 23
Branch analysis from position: 14
Jump found. Position 1 = 15, Position 2 = 23
Branch analysis from position: 15
Jump found. Position 1 = 14
Branch analysis from position: 14
Branch analysis from position: 23
Jump found. Position 1 = -2
Branch analysis from position: 23
Branch analysis from position: 12
filename:       %sforeach-jumps-php54.php
function name:  getLoader
number of ops:  25
compiled vars:  !0 = $map, !1 = $namespace, !2 = $path
line     #* E I O op                           fetch          ext  return  operands
-------------------------------------------------------------------------------------
   6     0  E >   INIT_ARRAY                                       ~0      
         1        ASSIGN                                                   !0, ~0
   8     2      > FE_RESET                                         $2      !0, ->12
         3    > > FE_FETCH                                         $3      $2, ->12
         4    >   OP_DATA                                          ~5      
         5        ASSIGN                                                   !2, $3
         6        ASSIGN                                                   !1, ~5
   9     7        ECHO                                                     !1
         8        ECHO                                                     '%3A+'
         9        ECHO                                                     !2
        10        ECHO                                                     '%0A'
  10    11      > JMP                                                      ->3
        12    >   SWITCH_FREE                                              $2
  12    13      > FE_RESET                                         $7      !0, ->23
        14    > > FE_FETCH                                         $8      $7, ->23
        15    >   OP_DATA                                          ~10     
        16        ASSIGN                                                   !2, $8
        17        ASSIGN                                                   !1, ~10
  13    18        ECHO                                                     !1
        19        ECHO                                                     '%3A+'
        20        ECHO                                                     !2
        21        ECHO                                                     '%0A'
  14    22      > JMP                                                      ->14
        23    >   SWITCH_FREE                                              $7
  16    24      > RETURN                                                   null

End of function getloader

End of class ComposerAutoloaderInit0d37b910670b3f7ddfa6b4516753af64.

branch: #  0; line:     3-   19; sop:     0; eop:     1; out1:  -2
path #1: 0, 
branch: #  0; line:     6-    8; sop:     0; eop:     2; out1:   3; out2:  12
branch: #  3; line:     8-    8; sop:     3; eop:     3; out1:   4; out2:  12
branch: #  4; line:     8-   10; sop:     4; eop:    11; out1:   3
branch: # 12; line:    10-   12; sop:    12; eop:    13; out1:  14; out2:  23
branch: # 14; line:    12-   12; sop:    14; eop:    14; out1:  15; out2:  23
branch: # 15; line:    12-   14; sop:    15; eop:    22; out1:  14
branch: # 23; line:    14-   16; sop:    23; eop:    24; out1:  -2
path #1: 0, 3, 4, 3, 12, 14, 15, 14, 23, 
path #2: 0, 3, 4, 3, 12, 14, 23, 
path #3: 0, 3, 4, 3, 12, 23, 
path #4: 0, 3, 12, 14, 15, 14, 23, 
path #5: 0, 3, 12, 14, 23, 
path #6: 0, 3, 12, 23, 
path #7: 0, 12, 14, 15, 14, 23, 
path #8: 0, 12, 14, 23, 
path #9: 0, 12, 23,
