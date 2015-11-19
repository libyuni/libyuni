#!/usr/bin/env bash

root=$(dirname "${0}")

list=$(cd "${root}" && ls | grep '\.md' | sort -rV)


html=""
space="



"

for i in $list; do
	html="${html}$(markdown "${i}")${space}"
done


echo "${html}" \
	| sed 's|<h2>|<h3>|g' | sed 's|</h2>|</h3>|g' \
	| sed 's|<h1>|<h2>|g' | sed 's|</h1>|</h2>|g' \
	| sed 's|<h2>|\n\n\n\n\n<h2>|' \
	| sed 's|<li><p>|<li>|g' | sed 's|</p></li>|</li>|g'
