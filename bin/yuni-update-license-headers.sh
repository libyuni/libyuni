#!/bin/bash

argv0="${0}"

# current root folder
rootfolder="" # will be reset later

# source folder
srcfolder=""

# realpath bin
bin_realpath=$(which realpath)


die()
{
	>&2 echo "[!!] ${1}"
	exit 1
}



main()
{
	echo "Yuni License updater"
	[ ! "" = "${bin_realpath}" ] || die "the program 'realpath' is missing"

	rootfolder=$("${bin_realpath}" "$(dirname "${argv0}")/..")
	srcfolder=$("${bin_realpath}" "${rootfolder}/src/yuni")
	tmpfile=$(mktemp)
	license=$(cat "${rootfolder}/bin/__license-header.txt")

	echo " . root: ${rootfolder}"
	echo " . sources: ${srcfolder}"
	echo " . temp file: ${tmpfile}"
	echo

	file_headers=$(find "${srcfolder}" '(' -name '*.h' -or -name '*.hxx' ')' \
		-and '!' '(' -path '*/msinttypes/*' ')' \
		-and '!' '(' -path '*private*/sqlite/*' ')' \
		-and '!' '(' -path '*private*/glew/*' ')' \
		-and '!' '(' -path '*private*/peg/*' ')' \
		-and '!' '(' -name 'mongoose.h' ')' \
		)

	for filename in ${file_headers}; do
		echo " . updating ${filename}"
		dos2unix -q -S "${filename}"

		local isHeader=1
		local content="${license}"

		while IFS='' read -r line || [[ -n $line ]]; do
			if [ "${isHeader}" -ge 1 ]; then
				if [[ ! "${line}" == '/*'* ]] && [[ ! "${line}" == '**'* ]] && [[ ! "${line}" == '*/'* ]] ; then
					isHeader=0
					echo "${license}" > "${tmpfile}"
					echo "${line}" >> "${tmpfile}"
				fi
			else
				echo "${line}" >> "${tmpfile}"
			fi
			# echo "Text read from file: $line"
		done < "${filename}"

		cat "${tmpfile}" > "${filename}"
	done

	rm -f "${tmpfile}"
}



IFS="
"
main
