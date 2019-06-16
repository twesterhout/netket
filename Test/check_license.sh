# Copyright 2019 The Simons Foundation, Inc. - All Rights Reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#!/bin/bash

_USE_COLORS=true
if diff --color=always 2>&1 | grep -q "unrecognized option" ; then
	_USE_COLORS=false
fi

_diff() {
	if $_USE_COLORS ; then
		diff --color=always $@
	else
		diff $@
	fi
}

_get_year_specs() {
	declare -A exceptions
	# We do our best to guess the years correctly, but sometimes, git log
	# --follow gets stuff wrong.  If you're absolutely sure that you've
	# encountered such an example, add it to the exception list below
	exceptions["netket/__init__.py"]="2019"

	if [ $# -ne 1 ] ; then echo "expected one argument: filename"; return 1; fi
	if [ -n "${exceptions[$1]}" ] ; then echo ${exceptions["$1"]}; return; fi
	years_log=$(git log --follow --pretty=%ad --date=short "$1" | cut -d'-' -f 1 | sort -n)
	years_blame=$(git blame -p "$1" | grep -E "^author-time" | cut -d' ' -f 2 | sort -n)
	if [ -z "$years_log" ] && [ -z "$years_blame" ] ; then date +%Y; return; fi
	min_year=$(echo "$years_blame" | head -n 1)
	min_year=$(date +%Y -d @$min_year)
	if [ -n "$years_log" ] && [ $(echo "$years_log" | head -n 1) -lt $min_year ] ; then
		min_year=$(echo "$years_log" | head -n 1)
	fi
	max_year=$(echo "$years_blame" | tail -n 1)
	max_year=$(date +%Y -d @$max_year)
	# if [ -n "$years_log" ] && [ $(echo "$years_log" | tail -n 1) -gt $max_year ] ; then
	# 	max_year=$(echo "$years_log" | tail -n 1)
	# fi
	if [ $min_year -ne $max_year ] ; then
		echo "$min_year-$max_year"
	else
		echo "$min_year"
	fi
}

_expected_cxx_header() {
	if [ $# -ne 1 ] ; then echo "$0: expected one argument: filename"; return 1; fi
	_f=$1
	_c=
	case "${filename##*.}" in
		sh|py|txt) _c="#" ;;
		cc|hpp) _c="//" ;;
	esac
	cat <<-EOF
		$_c Copyright $(_get_year_specs "$_f") The Simons Foundation, Inc. - All Rights Reserved.
		$_c
		$_c Licensed under the Apache License, Version 2.0 (the "License");
		$_c you may not use this file except in compliance with the License.
		$_c You may obtain a copy of the License at
		$_c
		$_c    http://www.apache.org/licenses/LICENSE-2.0
		$_c
		$_c Unless required by applicable law or agreed to in writing, software
		$_c distributed under the License is distributed on an "AS IS" BASIS,
		$_c WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
		$_c See the License for the specific language governing permissions and
		$_c limitations under the License.
	EOF
}

_check_copyright() {
	filename=$1
	expected=$(_expected_cxx_header "$filename")
	output=$(_diff <(head -n $(echo "$expected" | wc -l) "$filename") <(echo "$expected"))
	if [ -n "$output" ] ; then
		echo -e "\e[1;37m$filename:\e[0m"
		echo -e "$output" | sed 's/^/    /'
		return 1
	fi
}

STATUS=0
for f in $(git ls-files); do
	case "$f" in
		*CMakeLists.txt) ;&
		*check_license.sh) ;&
		Examples/*.py|Examples/*.cc|Examples/*.hpp) ;&		
		# Sourses/*.py|Sources/*.cc|Sources/*.hpp) ;&		
		Test/*.py|Test/*.cc|Test/*.hpp) ;&		
		setup.py|netket/*.py|netket/*.cc|netket/*.hpp)
			_check_copyright "$f"
			_status=$?
			[ $_status -ne 0 ] && STATUS=$_status
		;;
	esac
done
exit $STATUS
