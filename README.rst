README
======

This extension is to show low level PHP structures. It is therefore very
sensitive to changes in the PHP API. If the PECL install doesn't work, please
try the latest version from GitHub::

	git clone https://github.com/derickr/vld.git
	cd vld
	phpize
	./configure
	make && make install

If it still doesn't compile, then VLD is not for you. Patches are welcome
through Pull Requests.

How does it work?
-----------------

There are a few options in the form of ``php.ini`` settings available.

In order for VLD to do anything, you need to set ``vld.active=1``.

If you want to prevent VLD from executing coe, set ``vld.execute=0``.

Other settings are also available, but not documented yet.

Please see the project page at http://derickrethans.nl/projects.html#vld for
some more information.

