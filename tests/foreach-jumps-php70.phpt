--TEST--
Test for jump calculation with foreach()
--INI--
vld.active=1
--SKIPIF--
<?php if (PHP_VERSION_ID < 70000) { echo "skip PHP 7 required\n"; } ?> 
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
filename:       %sforeach-jumps-php70.php
function name:  (null)
number of ops:  2
compiled vars:  none
line     #* E I O op                           fetch          ext  return  operands
-------------------------------------------------------------------------------------
   2     0  E >   NOP                                                      
  17     1      > RETURN                                                   1

Class ComposerAutoloaderInit0d37b910670b3f7ddfa6b4516753af64:
Function getloader:
Finding entry points
Branch analysis from position: 0
Jump found. Position 1 = 2, Position 2 = 9
Branch analysis from position: 2
Jump found. Position 1 = 3, Position 2 = 9
Branch analysis from position: 3
Jump found. Position 1 = 2
Branch analysis from position: 2
Branch analysis from position: 9
Jump found. Position 1 = 11, Position 2 = 18
Branch analysis from position: 11
Jump found. Position 1 = 12, Position 2 = 18
Branch analysis from position: 12
Jump found. Position 1 = 11
Branch analysis from position: 11
Branch analysis from position: 18
Jump found. Position 1 = -2
Branch analysis from position: 18
Branch analysis from position: 9
filename:       %sforeach-jumps-php70.php
function name:  getLoader
number of ops:  20
compiled vars:  !0 = $map, !1 = $path, !2 = $namespace
line     #* E I O op                           fetch          ext  return  operands
-------------------------------------------------------------------------------------
   6     0  E >   ASSIGN                                                   !0, <array>
   8     1      > FE_RESET_R                                       $4      !0, ->9
         2    > > FE_FETCH_R                                               $4, !1, ->9
         3    >   ASSIGN                                                   !2, ~5
   9     4        ECHO                                                     !2
         5        ECHO                                                     '%3A+'
         6        ECHO                                                     !1
         7        ECHO                                                     '%0A'
         8      > JMP                                                      ->2
         9    >   FE_FREE                                                  $4
  12    10      > FE_RESET_R                                       $7      !0, ->18
        11    > > FE_FETCH_R                                               $7, !1, ->18
        12    >   ASSIGN                                                   !2, ~8
  13    13        ECHO                                                     !2
        14        ECHO                                                     '%3A+'
        15        ECHO                                                     !1
        16        ECHO                                                     '%0A'
        17      > JMP                                                      ->11
        18    >   FE_FREE                                                  $7
  16    19      > RETURN                                                   null

End of function getloader

End of class ComposerAutoloaderInit0d37b910670b3f7ddfa6b4516753af64.

branch: #  0; line:     2-   17; sop:     0; eop:     1; out1:  -2
path #1: 0, 
branch: #  0; line:     6-    8; sop:     0; eop:     1; out1:   2; out2:   9
branch: #  2; line:     8-    8; sop:     2; eop:     2; out1:   3; out2:   9
branch: #  3; line:     8-    9; sop:     3; eop:     8; out1:   2
branch: #  9; line:     9-   12; sop:     9; eop:    10; out1:  11; out2:  18
branch: # 11; line:    12-   12; sop:    11; eop:    11; out1:  12; out2:  18
branch: # 12; line:    12-   13; sop:    12; eop:    17; out1:  11
branch: # 18; line:    13-   16; sop:    18; eop:    19; out1:  -2
path #1: 0, 2, 3, 2, 9, 11, 12, 11, 18, 
path #2: 0, 2, 3, 2, 9, 11, 18, 
path #3: 0, 2, 3, 2, 9, 18, 
path #4: 0, 2, 9, 11, 12, 11, 18, 
path #5: 0, 2, 9, 11, 18, 
path #6: 0, 2, 9, 18, 
path #7: 0, 9, 11, 12, 11, 18, 
path #8: 0, 9, 11, 18, 
path #9: 0, 9, 18,
