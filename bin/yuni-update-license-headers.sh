#!/bin/bash

argv0="${0}"

# current root folder
rootfolder="" # will be reset later

# source folder
srcfolder=""

# realpath bin
bin_realpath=$(which realpath)


rootfolder=$("${bin_realpath}" "$(dirname "${argv0}")/..")

srcfolder=$("${bin_realpath}" "${rootfolder}/src/")





die()
{
	>&2 echo "[!!] ${1}"
	exit 1
}

tmpfile=$(mktemp yuni-tmp-license-updater-XXXXXX)
[ "$?" -eq 0 ] || die "failed to create temporary file"

license=$(cat "${rootfolder}/bin/__license-header.txt")
[ "$?" -eq 0 ] || die "failed to read the license file template"




update_file()
{
	local filename="$1"
	local isHeaderFile="$2"

	echo " . updating ${filename}"
	dos2unix -f -q -S "${filename}"

	local isHeader=1
	local content="${license}"

	while IFS='' read -r line || [[ -n $line ]]; do
		if [ "${isHeader}" -ge 1 ]; then
			if [[ ! "${line}" == '/*'* ]] && [[ ! "${line}" == '**'* ]] && [[ ! "${line}" == '*/'* ]] && [[ ! "${line}" == '' ]]; then

				if [ ! "${isHeaderFile}" = 0 ]; then
					if [[ ! "${line}" == '#pragma once' ]]; then
						>&2 echo " . invalid header guard: ${filename} (#pragma once expected)"
					fi
				fi

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
}



main()
{
	echo "Yuni License updater"
	[ ! "" = "${bin_realpath}" ] || die "the program 'realpath' is missing"

	echo " . root: ${rootfolder}"
	echo " . sources: ${srcfolder}"
	echo " . temp file: ${tmpfile}"
	echo

	echo
	echo " . processing .h,.hxx files..."
	file_headers=$(find "${srcfolder}" '(' -name '*.h' -or -name '*.hxx' ')' \
		-and '!' '(' -path '*/msinttypes/*' ')' \
		-and '!' '(' -path '*/bind/*' ')' \
		-and '!' '(' -path '*/yuni-docmake/*' ')' \
		-and '!' '(' -path '*private*/sqlite/*' ')' \
		-and '!' '(' -path '*private*/glew/*' ')' \
		-and '!' '(' -path '*private*/peg/*' ')' \
		-and '!' '(' -name 'mongoose.h' ')' \
		-and '!' '(' -name 'vs-banned.h' ')' \
		-and '!' '(' -name 'config.h' ')' \
		)

	for filename in ${file_headers}; do
		update_file "${filename}" 1
	done


	echo
	echo " . processing .cpp files..."
	file_headers=$(find "${srcfolder}" '(' -name '*.cpp' ')' \
		-and '!' '(' -path '*/msinttypes/*' ')' \
		-and '!' '(' -path '*/bind/*' ')' \
		-and '!' '(' -path '*/yuni-docmake/*' ')' \
		-and '!' '(' -path '*private*/sqlite/*' ')' \
		-and '!' '(' -path '*private*/glew/*' ')' \
		-and '!' '(' -path '*private*/peg/*' ')' \
		-and '!' '(' -name 'stb_image.cpp' ')' \
		)

	for filename in ${file_headers}; do
		update_file "${filename}" 0
	done


	rm -f "${tmpfile}"
}



IFS="
"
main
