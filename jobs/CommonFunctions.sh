#!/bin/bash -
# * ============================================================================= * #
# *   DESCRIPTION: Shell script that contains definitions of functions that are   * #
# *                used in the bash scripts this folder.                          * #
# *        AUTHOR: Remco de Boer (@IHEP), EMAIL: remco.de.boer@ihep.ac.cn         * #
# *  ORGANIZATION: IHEP, CAS (Beijing, CHINA)                                     * #
# *       CREATED: 8 November 2018                                                * #
# *         USAGE: include in another bash script with this line:                 * #
# *                source CommonFunctions.sh                                      * #
# * ============================================================================= * #


# * ================================== * #
# * ------- SCRIPT INITIALISER ------- * #
# * ================================== * #

	# * Check if already being sourced * #
		if [ "${CommonFunctionsScriptIsSourced}" == true ]; then
			return
		fi

	# * Set identifier parameters for this script * #
		# * Parameter that blocks script from resourcing
		CommonFunctionsScriptIsSourced=true
		# * Get absolute path to script
		if [[ "${BASH_SOURCE[0]}" == /* ]]; then # if absolute already
			PathToCommonFunctionsScript="${BASH_SOURCE[0]}"
		else # if relative path
			cd "$(dirname "${BASH_SOURCE[0]}")" > /dev/null
			PathToCommonFunctionsScript="$(pwd)/${BASH_SOURCE[0]}"
			cd - > /dev/null
		fi



# * ======================= * #
# * ------- GLOBALS ------- * #
# * ======================= * #

	gErrorColorCode="\e[91m"
	gSuccessColorCode="\e[92m"
	gInputColorCode="\e[93m"
	gColorCodeEnd="\e[0m"



# * ==================================== * #
# * ------- NAVIGATION FUNCTIONS ------- * #
# * ==================================== * #

	function CreateBaseDir()
	# Creates a base directory for a file to which you want to write.
	{
		mkdir -p "$(dirname "${1}")"
	}
	export CreateBaseDir

	function CdToBaseDir()
	# Creates a base directory for a file to which you want to write.
	{
		gStoredDirectory="$(pwd)"
		cd "$(dirname "${1}")"
	}
	export CdToBaseDir

	function CreateOrEmptyDirectory()
	# Create a directory if necessary. If it already exists, remove the already existing files (with a certain pattern).
	{
		# * Import function arguments
		local path=${1}
		local subdir=${2}
		local analysis_type=${3}
		# * Main function: mkdir if necessary
		if [ ! -d "${path}/${2}" ]; then
			mkdir -p "${path}/${2}"
		else
			rm -rf "${path}/${2}/${2}_${analysis_type}_"*".sh"       # remove sumbit scripts
			rm -rf "${path}/${2}/${2}_${analysis_type}_"*".sh."*"."* # remove log files
			rm -rf "${path}/${2}/${2}_${analysis_type}_"*".txt"      # remove jobOptions
		fi
	}
	export CreateOrEmptyDirectory



# * ======================================= * #
# * ------- SCRIPT ACCESS FUNCTIONS ------- * #
# * ======================================= * #

	function ResourceCommonFunctions()
	# The parameter `CommonFunctionsScriptIsSourced` is set to `true` if this script is sourced. Use this function if you really want to bypass this safety measure and source again.
	{
		CommonFunctionsScriptIsSourced=false
		source "${PathToCommonFunctionsScript}"
	}
	export ResourceCommonFunctions



# * =============================== * #
# * ------- PRINT FUNCTIONS ------- * #
# * =============================== * #

	function PrintErrorMessage()
	# Print a terminal message in the color used for an error message.
	{
		echo -e "${gErrorColorCode}${1}${gColorCodeEnd}"
	}
	export PrintErrorMessage

	function PrintSuccessMessage()
	# Print a terminal message in the color used for a success message.
	{
		echo -e "${gSuccessColorCode}${1}${gColorCodeEnd}"
	}
	export PrintSuccessMessage

	function AskForInput()
	# Print a terminal message in the color used for a success message.
	{
		echo -e "${gInputColorCode}${1}${gColorCodeEnd}"
		read -p ""
	}
	export AskForInput



# * =============================== * #
# * ------- CHECK FUNCTIONS ------- * #
# * =============================== * #

	function CheckIfFolderExists()
	# Check if a folder exists. Exit the script if it doesn't.
	{
		local folderToCheck="${1}"
		if [ ! -d ${folderToCheck} ]; then
			PrintErrorMessage "ERROR: Folder \"${folderToCheck}\" does not exist"
			exit
		fi
	}
	export CheckIfFolderExists

	function CheckIfBaseDirExists()
	# Check if the path to a folder exists. Exit the script if it doesn't.
	{
		CheckIfFolderExists "$(dirname "${1}")"
	}
	export CheckIfBaseDirExists

	function CheckIfFileExists()
	# Check if a file exists. Exit the script if it doesn't.
	{
		local fileToCheck="${1}"
		if [ ! -s "${fileToCheck}" ]; then
			PrintErrorMessage "ERROR: File \"${fileToCheck}\" does not exist"
			exit
		fi
	}
	export CheckIfFileExists



# * ====================================== * #
# * ------- TEXT FILE MODIFICATION ------- * #
# * ====================================== * #

	function DeleteAllEmptyLines()
	# Delete all empty lines in a file, including files that only contain whitespace characters
	{
		# * Import function arguments
		local fileName=${1}
		# * Main function: delete all empty lines of the file
		sed -i '/^\s*$/d' ${fileName} # delete all empty lines of the file
	}
	export DeleteAllEmptyLines

	function FormatTextFileToCppVectorArguments()
	# Feed this function a text file, and it will prepend a `\t"` and append a `",` to each line. The comma is ommited for the last line.
	{
		# * Import function arguments
		local fileName=${1}
		# * Execute function if lines 
		local numCorrect=$(grep "^\s*\"[^\"]*\",\s*$" "${fileName}" | wc -l) # does not include last line
		local numLines=$(cat "${fileName}" | sed '/^\s*$/d' | wc -l)
		if [ $numLines != $(($numCorrect+1)) ]; then
			DeleteAllEmptyLines ${fileName}
			sed -i -e "s/.*/\t\"&\",/" ${fileName} # convert lines to C++ vector arguments
			sed -i "$ s/.$//"          ${fileName} # remove last comma
		fi
	}
	export FormatTextFileToCppVectorArguments

	function ChangeLineEndingsFromWindowsToUnix()
	# Windows sometimes stores files with a different type of line endings. To make the file compatible again with Unix/Linux, use this function.
	{
		# * Import function arguments
		local fileName=${1}
		# * Main function: remove Windows style newline characters
		sed -i 's/\r$//' "${fileName}"
	}
	export ChangeLineEndingsFromWindowsToUnix

	function SplitTextFile()
	# Feed this function a path to a text file ($1) and it will split up this file into separate files each with a maximum number of lines ($2 -- default value is 10).
	{
		# * Import function arguments
			local fileToSplit="${1}"
			local maxNLines=10 # default value
			if [ $# -ge 2 ]; then
				maxNLines=${2}
			fi
		# * Check arguments
		CheckIfFileExists "${fileToSplit}"
		# * Extract path, filename, and extension for prefix * #
			local path=$(dirname "${fileToSplit}")
			local filename=$(basename "${fileToSplit}")
			local extension="${filename/*.}"
			local filename="${filename%.*}"
			local prefix="${path}/${filename}_"
		# * Split input file * #
		split -d -a3 -l${maxNLines} "${fileToSplit}" "${prefix}"
		# * Append extension again (if original file has one) * #
		# shopt -s extglob # for regex
		for file in $(ls ${prefix}???); do #! number of ? should match the -a3 argument above
			DeleteAllEmptyLines "${file}"
			if [ "${filename}" != "${extension}" ]; then
				mv -f "${file}" "${file}.${extension}"
			fi
		done
		# ! REMOVE ORIGINAL FILE ! #
			rm "${fileToSplit}"
	}
	export SplitTextFile



# * ====================================== * #
# * ------- LS INVENTORY FUNCTIONS ------- * #
# * ====================================== * #

	function CreateFilenameInventoryFromDirectory()
	# Feed this function a path to a directory ($1) and it will list all files within that directory including their absolute paths. This list will be written to text files ($2) with a maximum number of paths per file ($3 -- default is 0, namely no max). If you wish, you can only list files of a certain extension ($4).
	{
		# * Import function arguments
			local inputDirectory="${1}"
			local outputFile="${inputDirectory//\//_}"
				if [ "${outputFile:0:1}" == "_" ]; then
					outputFile="${outputFile:1}"
				fi
				outputFile="filenames/${outputFile}.txt"
			if [ $# -ge 2 ]; then outputFile="${2}"; fi
			local maxNLines=8 # default value
			if [ $# -ge 3 ]; then maxNLines=$3; fi
			local extension="*" # does not look for extensions by default
			if [ $# -ge 4 ]; then extension="${4}"; fi
		# * Check arguments
			CheckIfFolderExists "${inputDirectory}"
			CreateBaseDir "${outputFile}"
		# * Get absolute path of the input directory so that `find` lists absolute paths as well
			cd "${inputDirectory}"
			inputDirectory="$(pwd)"
			cd - > /dev/null
		# * Make an inventory and write to file
			find "${inputDirectory}" -type f | grep ".*.${extension}$" > "${outputFile}"
			DeleteAllEmptyLines "${outputFile}"
		# * Split the output file if required
			if [ $maxNLines -gt 0 ]; then
				SplitTextFile "${outputFile}" ${maxNLines}
			fi
	}
	export CreateFilenameInventoryFromDirectory

	function CreateFilenameInventoryFromFile()
	# Feed this function a path ($1) to a file containing directories and/or file names and it will list all files within those directories including their absolute paths.  This list will be written to text files ($2) with a maximum number of paths per file ($3 -- default is 0, namely no max). If you wish, you can only list files of a certain extension ($4).
	{
		# * Import function arguments * #
			local inputFile="${1}"
			local outputFile="${2}"
			local maxNLines=0 # default value
			if [ $# -ge 3 ]; then maxNLines=${3}; fi
			local extension="*" # does not look for extensions by default
			if [ $# -ge 4 ]; then extension=${4}; fi
		# * Check arguments * #
			CheckIfFileExists   "${inputFile}"
			DeleteAllEmptyLines "${inputFile}"
			CreateBaseDir "${outputFile}"
			local startPath="$(pwd)"
		# * Get absolute path of input file * #
			CdToBaseDir "${inputFile}"
			inputFile="$(pwd)/$(basename "${inputFile}")"
			cd - > /dev/null
		# * Get absolute path of output file * #
			CdToBaseDir "${outputFile}"
			outputFile="$(pwd)/$(basename "${outputFile}")"
			cd - > /dev/null
		# * Make an inventory and write to file * #
			CdToBaseDir "${inputFile}" # in case of relative paths
			echo > "${outputFile}" # empty output file
			{ cat "${inputFile}"; echo; } | while read line; do
				# * Check if empty line *
				if [ "${line}" != "" ]; then
					# * If line is a file, just add it to the output file
					if [ -f "${line}" ]; then
						echo "Added file \"$(basename ${line}\")"
						echo "${line}" >> "${outputFile}"
					# * Otherwise, presume line is a directory and add its contents
					elif [ -d ${line} ]; then # check if directory exists
						CreateFilenameInventoryFromDirectory "${line}" "temp.txt" 0 "${extension}"
						cat "temp.txt" >> "${outputFile}"
						echo "Added directory \"$(basename ${line}\") ($(cat temp.txt | wc -l) files)"
						rm "temp.txt"
					# * Error moessage if nothing
					else
						PrintErrorMessage "WARNING: \"${line}\" does not exist"
					fi
				fi
			done
		# * Finalise output file * #
			DeleteAllEmptyLines "${outputFile}"
			PrintSuccessMessage "\nRead $(cat "${outputFile}" | wc -l) files and directories from file \"$(basename "${inputFile}")\""
			PrintSuccessMessage "--> output written to \"$(basename "${outputFile}")\"\n"
		# * Split the output file if required
			if [ $maxNLines -gt 0 ]; then
				SplitTextFile "${outputFile}" ${maxNLines}
			fi
		# * Go back to starting directory * #
			cd "${startPath}"
	}
	export CreateFilenameInventoryFromFile



PrintSuccessMessage "\nSuccessfully loaded \"CommonFunctions.sh\"\n"