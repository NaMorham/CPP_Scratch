#! /bin/bash

normal="${ansi_normal:-\e[0m}"
bold="${ansi_bold:-\e[1m}"
italic="${ansi_italic:-\e[3m}"
underline="${ansi_underline:-\e[4m}"
black="${ansi_black:-\e[30m}"
red="${ansi_red:-\e[31m}"
green="${ansi_green:-\e[32m}"
yellow="${ansi_yellow:-\e[33m}"
blue="${ansi_blue:-\e[34m}"
magenta="${ansi_magenta:-\e[35m}"
cyan="${ansi_cyan:-\e[36m}"
white="${ansi_white:-\e[37m}"

# >&2 echo -e "${black}black, ${red}red, ${green}green, ${yellow}yellow, ${blue}blue, ${magenta}magenta, ${cyan}cyan, ${white}white${normal}"
# >&2 echo -e "${bold}${black}black, ${red}red, ${green}green, ${yellow}yellow, ${blue}blue, ${magenta}magenta, ${cyan}cyan, ${white}white${normal}"

if [[ ! -t 0 ]]; then
    normal=""
    bold=""
    italic=""
    underline=""
    black=""
    red=""
    green=""
    yellow=""
    blue=""
    magenta=""
    cyan=""
    white=""
fi

# ---------------------------------------------
function errEcho()
{
    >&2 echo -e $*
}

function die()
{
    local _retCode; _retCode=$?
    errEcho "${bold}${red}[ERROR] Failure in:${normal} $@" 1>&2
    if [[ $_retCode == 0 ]] ; then
        _retCode=1
    fi
    exit $_retCode
}

# ---------------------------------------------
case "$(tr '[:upper:]' '[:lower:]' <<< "${DO_SCRIPT_DEBUG:-false}")" in
	false|off|no|0)
		SCRIPT_DBG=0
		;;
	true|on|yes|1)
		SCRIPT_DBG=1
		;;
	*)
		errEcho "Unknown setting for script debugging \$DO_SCRIPT_DEBUG = [${DO_SCRIPT_DEBUG:=}]"
		SCRIPT_DBG=0
		;;
esac

function dbgList
{
	if [[ $SCRIPT_DBG -ne 0 ]]; then
		local _listName; _listName="${1:-List}"
		if [[ $# > 0 ]]; then
			shift
		fi
		local _msgtxt; _msgtxt="${@:- EMPTY}"
		errEcho "${blue}[DBG ]: $_listName = [${black}${bold}"
		errEcho "$_msgtxt"
		errEcho "${normal}${blue}]${normal}"
	fi
}

function dbgEcho
{
	if [[ $SCRIPT_DBG -ne 0 ]]; then
		local _msgtxt; _msgtxt="${@:-}"
		errEcho "${blue}[DBG ]:${black}${bold} $_msgtxt${normal}"
	fi
}

function dbgVar()
{
	if [[ $SCRIPT_DBG -ne 0 ]]; then
		local _line; _line=""
		local _entry; _entry=""
		while [[ $# -ge 1 ]]; do
			if [[ -n $1 ]]; then
				eval _val=\$$1
				_entry="$(printf "$1 = [${_val:-NOT SET}]")"
				if [[ $# -gt 1 ]]; then
					_entry="${_entry}, "
				fi
			fi
			_line="${_line}${_entry}"
			shift
		done
		dbgEcho "${_line}${normal}"
	fi
}

function traceEcho
{
	if [[ $SCRIPT_DBG -ne 0 ]]; then
		local _msgtxt;_msgtxt="${@:-}"
		errEcho "${magenta}[TRC ]:${normal} $_msgtxt${normal}"
	fi
}

function infoEcho
{
	if [[ $SCRIPT_DBG -ne 0 ]]; then
		local _msgtxt;_msgtxt="${@:-}"
		errEcho "${yellow}[INFO]:${normal} $_msgtxt${normal}"
	fi
}

function warnEcho
{
	if [[ $SCRIPT_DBG -ne 0 ]]; then
		local _msgtxt;_msgtxt="${@:-}"
		errEcho "${red}[WARN]:${normal} $_msgtxt${normal}"
	fi
}

function errorEcho
{
	# ignore $SCRIPT_DBG always show errors
    local _msgtxt;_msgtxt="${@:-}"
    errEcho "${bold}${red}[ERR ]:${normal} $_msgtxt${normal}"
}

# ---------------------------------------------
function preserveFile()
{
    local _fname; _fname="$1"
    if [[ -n $_fname && -f $_fname ]]; then
        # Get the last accessed time - trim off the fractional seconds?
        local _accessTime; _accessTime="$(find $(dirname "$_fname") -name "$_fname" -printf "%AY%Am%Ad.%AH%AM%AS")"
        if [[ -z $_accessTime ]]; then
            _accessTime="UNKNOWN"
        fi
        infoEcho "Moving existing ${bold}[$_fname]${normal}"
        mv -fv "$_fname" "${_fname}.${_accessTime}.old" || die "Cannot move existing file"
    fi
}

# ---------------------------------------------
set -u -o pipefail

function usage()
{
    errEcho "${bold}Usage:${normal} $(basename "${BASH_SOURCE[0]}") <project name>"
}

localDir=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )

if [[ $# -lt 1 ]]; then
    usage
    exit 1
fi

# always log - uncomment if needed
if [[ -n ${DO_SCRIPT_LOGGING:-} ]]; then
    logfileprefix="$(basename "${BASH_SOURCE[0]}" | sed -r "s:\..*$::").$(date "+%Y%m%d%H%M")"
    dbgVar logfileprefix
    if [[ -f ${logfileprefix}.err.log || -f ${logfileprefix}.out.log ]]; then
        logfileprefix="${logfileprefix}.$$"
        dbgEcho "Rebuilt logfileprefix [$logfileprefix]"
    fi
    infoEcho "Logging to ${logfileprefix}.err.log and ${logfileprefix}.out.log" 
    exec 2> >(tee -a ${logfileprefix}.err.log 1>&2)
    exec > >(tee -a ${logfileprefix}.out.log)
else
    dbgEcho "Not logging - DO_SCRIPT_LOGGING is set"
fi

#set -x
# ---------------------------------------------

traceEcho "Process args"

dryrun=""
if [[ $@ =~ --dryrun ]]; then
	dryrun="echo '(DRYRUN)'"
fi
dbgVar dryrun

if [[ ${1:-} == --dryrun ]]; then
	shift
fi

projectName="${1:-}"

traceEcho "Validate args"

if [[ ${projectName:-} =~ ^[a-zA-Z][a-zA-Z0-9_]+[a-zA-Z0-9]$ ]]; then
	dbgVar projectName
elif [[ ${projectName:-} =~ ^[^a-zA-Z] ]]; then
	die "Project name must start with a letter"
elif [[ ${projectName:-} =~ [^a-zA-Z0-9_] ]]; then
	die "Project name can only start with a letter and must only contain alpha numerics or '_'"
elif [[ ${projectName:-} =~ [^a-zA-Z0-9]$ ]]; then
	die "Project name must end with an alpha numeric"
elif [[ ${#projectName} -lt 3 ]]; then
	die "Project name must be at least 3 characters long"
fi

if [[ ${2:-} == --dryrun ]]; then
	shift
fi

projectCName="${2:-}"
if [[ $# -ge 2 ]]; then
	if [[ ${projectCName:-} =~ ^[a-zA-Z][a-zA-Z0-9_]+[a-zA-Z0-9]$ ]]; then
		dbgVar projectCName
	elif [[ ${projectCName:-} =~ ^[^a-zA-Z] ]]; then
		die "Project files name must start with a letter"
	elif [[ ${projectCName:-} =~ [^a-zA-Z0-9_] ]]; then
		die "Project files name can only start with a letter and must only contain alpha numerics or '_'"
	elif [[ ${projectCName:-} =~ [^a-zA-Z0-9]$ ]]; then
		die "Project files name must end with an alpha numeric"
	elif [[ ${#projectCName} -lt 3 ]]; then
		die "Project files name must be at least 3 characters long"
	fi
else
	projectCName="$( sed -E "s/^([A-Z])/\L\1/" <<< "${projectName}" )"
	dbgVar projectCName
fi

traceEcho "Validate input"

projects_path="../Projects"
new_project_path="${projects_path}/${projectName}"

if [[ -d ../Projects/${projectName} ]]; then
	die "A project folder for ${projectName} already exists"
elif [[ -f ../Projects/${projectName} ]]; then
	die "A file with the same name (${projectName}) as the project already exists"	
fi

traceEcho "Process"

infoEcho "Create new project folder [${new_project_path}]"
${dryrun} mkdir "${new_project_path}" || die "Creating new project directory"

in_path="${projects_path}/template"
out_path="${projects_path}/${projectName}"

infoEcho "Process template files"
for fl in README.md.tmpl projectTemplateVer.h.in.tmpl projectTemplate.cpp.tmpl CMakeLists.txt.tmpl; do
	fl_out="$(sed "s/projectTemplate/"${projectCName}"/" <<< "${fl/\.tmpl/}")"
	dbgVar fl fl_out

	in_file="${in_path}/${fl}"
	out_file="${out_path}/${fl_out}"
	dbgVar in_file out_file

	if [[ -z ${dryrun} ]]; then
		sed $"s/\[\[\[PROJECT_NAME\]\]\]/"${projectName}"/g; s/\[\[\[PROJECT_CNAME\]\]\]/"${projectCName}"/g" "${in_file}" > "${out_file}" || die "Parsing template file [${out_file}]"
	else
		${dryrun} sed \$\"s/\[\[\[PROJECT_NAME\]\]\]/"${projectName}"/g\; s/\[\[\[PROJECT_CNAME\]\]\]/"${projectCName}"/g\" \"${in_file}\" \> \"${out_file}\"
	fi

	errEcho

done

projects_cmake="${projects_path}/CMakeLists.txt"
quoted_projectName="\"${projectName}\""
infoEcho "Add new project to [${projects_cmake}]"
if [[ -f ${projects_cmake}.bak ]]; then
	dbgEcho "Delete previous project cmake backup"
	${dryrun} rm -v "${projects_cmake}.bak" || die "removing previous projects cmake backup"
fi
${dryrun} mv -v "${projects_cmake}" "${projects_cmake}.bak" || "backing up current projects cmake file"

if [[ -z ${dryrun} ]]; then
	awk -vnew_name=${quoted_projectName} $'BEGIN { lastWasAdd = 0; } { if ( /add_subdirectory/ ) { lastWasAdd = 1; } else { if ( lastWasAdd == 1 ) { print "add_subdirectory("new_name")"; }; lastWasAdd = 0; }; print $0; }' "${projects_cmake}.bak" > "${projects_cmake}" || die "Adding to project CMake file"
else
	${dryrun} awk -vnew_name=${quoted_projectName} \$'BEGIN { lastWasAdd = 0; } { if ( /add_subdirectory/ ) { lastWasAdd = 1; } else { if ( lastWasAdd == 1 ) { print "add_subdirectory("new_name")"; }; lastWasAdd = 0; }; print $0; }' \"${projects_cmake}.bak\" \> \"${projects_cmake}\"
fi

# we made it here, clean up temp files
if [[ -f ${projects_cmake}.bak ]]; then
	rm "${projects_cmake}.bak" || warnEcho "Could not delete temporary file [${projects_cmake}.bak]"
fi

traceEcho "${bold}${green}DONE.${normal}"

exit 0
