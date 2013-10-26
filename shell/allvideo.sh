#!/bin/bash
USERAGENT="Mozilla/5.0"
baseurl='http://www.flvxz.com/getFlv.php?url='

while read line
do
	url=$(echo ${line} |awk '{print $1}')

	urlline=$(wget --user-agent="$USERAGENT" "${baseurl}${url}" -O - \
		-o /dev/stderr |sed 's=\(<\/\w\+>\)=\1\n=g' |\
		grep -v -e 'flvxz\.com' -e '^<[/a-z]*>$' |grep -A5 '单文件' | \
		grep -oP '\w+://.*(?=</a>)'
	)

	filename=$(echo -e ${urlline} |tr '/' ' ' |grep -oP '(?<=">).*')
	fileurl=$(echo ${urlline} |grep -oP '\w+://.*(?=">)')

	#wget -c -t 5 --user-agent="$USERAGENT" "${fileurl}" -O "${filename}"
	echo FILENAME:${filename}
	axel --user-agent="$USERAGENT" "${fileurl}" -o "${filename}"
done <url
