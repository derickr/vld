#!/bin/bash
export CFLAGS="-Wall -Wextra -Wdeclaration-after-statement -Wmissing-field-initializers -Wshadow -Wno-unused-parameter -ggdb3"
phpize
./configure
make all install
EXTENSIONDIR=`php -r 'echo ini_get("extension_dir");'`
echo "extension=vld.so" >> ~/.phpenv/versions/$(phpenv version-name)/etc/php.ini
echo "extension=vld.so" > /tmp/temp-php-config.ini
cat /tmp/temp-php-config.ini
