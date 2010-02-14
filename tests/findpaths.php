<?php
function readBranchFile( $fileName )
{
	$branches = array();

	foreach( file( $fileName ) as $line )
	{
		if ( preg_match( '@^branch: #\s*(\d+); line:\s+(\d+); sop:\s+(\d+); eop:\s+(\d+)(; out1:\s+(\d+))?(; out2:\s+(\d+))?@', $line, $m ) )
		{
			@list( $dummy, $nr, $line, $sop, $eop, $dummy, $out1, $dummy, $out2 ) = $m;
			$branches[$nr] = array(
				'nr' => $nr,
				'line' => $line,
				'sop' => $sop,
				'eop' => $eop,
				'out' => array( $out1, $out2 ),
			);
		}
	}
	return $branches;
}

function findLastElement( $path )
{
	$elems = array_reverse( explode( '-', $path ) );
	return $elems[0];
}

function findPaths( $nr, $branches, &$paths, $prevPath )
{
	$path = $prevPath . '-' . $nr;
	$out0 = $branches[$nr]['out'][0];
	$out1 = $branches[$nr]['out'][1];

	$last = findLastElement( $path );

	$found = false;
	if ( isset( $out0 ) && strpos( $path, "$last-$out0" ) === false )
	{
		findPaths( $out0, $branches, $paths, $path );
		$found = true;
	}
	if ( isset( $out1 ) && strpos( $path, "$last-$out1" ) === false )
	{
		findPaths( $out1, $branches, $paths, $path );
		$found = true;
	}
	if ( !$found )
	{
		$paths[] = $path;
	}
}

$paths = array();
$branches = readBranchFile( 'test2.branch' );
findPaths( 0, $branches, $paths, '' );
var_dump( $paths );
