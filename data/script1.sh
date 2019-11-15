#!/bin/bash

get_time () {
curl	'http://apps.mobilityware.com/Solitaire/getTime.php' \
	-H 'Host:apps.mobilityware.com' \
	-H 'Proxy-Connection:keep-alive' \
	-H 'Accept-Encoding:gzip, deflate' \
	-H 'Content-Type:application/x-www-form-urlencoded' \
	-H 'Content-Length:0' \
	-H 'Accept-Language:en-us' \
	-H 'Accept:*/*' \
	-H 'Pragma:no-cache' \
	-H 'Connection:keep-alive' \
	-H 'User-Agent:Solitaire/4.11.2.366 CFNetwork/672.1.15 Darwin/14.0.0' \
	-c cookiejar.txt \
	-o 1-getTime.php-response.txt
}

verified_one () {
today=`date --date="$1 days ago" +"%Y-%-m-%-d"`;
curl	'http://apps.mobilityware.com/Solitaire/Verified.php?g=0&c='$today'&statRows=100&d=1&z=181&v=0&statTabs=3' \
	-H 'Host:apps.mobilityware.com' \
	-H 'Proxy-Connection:keep-alive' \
	-H 'Accept-Encoding:gzip, deflate' \
	-H 'Content-Type:application/x-www-form-urlencoded' \
	-H 'Content-Length:0' \
	-H 'Accept-Language:en-us' \
	-H 'Accept:*/*' -H 'Pragma:no-cache' \
	-H 'Connection:keep-alive' \
	-H 'User-Agent:Solitaire/4.11.2.366 CFNetwork/672.1.15 Darwin/14.0.0' \
	-c cookiejar.txt \
	-b cookiejar.txt \
	-o "$today.txt"
}

get_time
for i in `seq 0 100`; do
    verified_one "$i"
done    


