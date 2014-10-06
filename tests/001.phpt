--TEST--
Check for vld presence
--SKIPIF--
<?php if (!extension_loaded("vld")) print "skip"; ?>
--POST--
--GET--
--FILE--
<?php 
echo "vld extension is available";
?>
--EXPECT--
vld extension is available
