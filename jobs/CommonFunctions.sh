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

# * Check if already being sourced * #
if [ "${CommonFunctionsScriptIsSourced}" == true ]; then
	return
fi
CommonFunctionsScriptIsSourced=true

# * ======================= * #
# * ------- GLOBALS ------- * #
# * ======================= * #

	gErrorColorCode="\e[91m"
	gSuccessColorCode="\e[92m"
	gInputColorCode="\e[93m"
	gColorCodeEnd="\e[0m"


# * =============================== * #
# * ------- PRINT FUNCTIONS ------- * #
# * =============================== * #

	function PrintErrorMessage()
	# Print a terminal message in the color used for an error message.
	{
		echo -e "\n${gErrorColorCode}${1}${gColorCodeEnd}"
	}

	function PrintSuccessMessage()
	# Print a terminal message in the color used for a success message.
	{
		echo -e "\n${gSuccessColorCode}${1}${gColorCodeEnd}"
	}

	function AskForInput()
	# Print a terminal message in the color used for a success message.
	{
		echo -e "\n${gInputColorCode}${1}${gColorCodeEnd}"
		read -p ""
	}



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

	function CheckIfFileExists()
	# Check if a file exists. Exit the script if it doesn't.
	{
		local fileToCheck="${1}"
		if [ ! -s "${fileToCheck}" ]; then
			PrintErrorMessage "ERROR: File \"${fileToCheck}\" does not exist"
			exit
		fi
	}

	function CreateOrEmptyDirectory()
	# Create a directory if necessary. If it already exists, remove the already existing files (with a certain pattern).
	{
		# * Import function arguments
		local path=${1}
		local subdir=${2}
		local analysis_type=${3}
		# * Main function: mkdir if necessary
		if [ ! -d "${path}/${2}" ]; then
			mkdir "${path}/${2}"
		else
			rm -rf "${path}/${2}/${2}_${analysis_type}_"*".sh"       # remove sumbit scripts
			rm -rf "${path}/${2}/${2}_${analysis_type}_"*".sh."*"."* # remove log files
			rm -rf "${path}/${2}/${2}_${analysis_type}_"*".txt"      # remove jobOptions
		fi
	}


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

	function FormatTextFileToCppVectorArguments()
	# Feed this function a text file, and it will prepend a `\t"` and append a `",` to each line. The comma is ommited for the last line.
	{
		# * Import function arguments
		local fileName=${1}
		# * Main function
		DeleteAllEmptyLines ${fileName}
		sed -i -e "s/.*/\t\"&\",/" ${fileName} # convert lines to C++ vector arguments
		sed -i "$ s/.$//"          ${fileName} # remove last comma
	}

	function ChangeLineEndingsFromWindowsToUnix()
	# Windows sometimes stores files with a different type of line endings. To make the file compatible again with Unix/Linux, use this function.
	{
		# * Import function arguments
		local fileName=${1}
		# * Main function: remove Windows style newline characters
		sed -i 's/\r$//' ${fileName}
	}


# * ====================================== * #
# * ------- LS INVENTORY FUNCTIONS ------- * #
# * ====================================== * #

	function CreateFilenameInventoryFromDirectory()
	# Feed this function a directory ($1) and it will list all files within that directory including their absolute paths. The output will be written to text files ($2), that with a maximum number of paths per file ($3). If you wish, you can only list files of a certain extension ($4).
	{
		# * Import function arguments
		local inputDirectory="${1}"
		local outputFile="${2}"
		local maxNPaths=10 # default value
		if [ $# -ge 3 ]; then
			maxNPaths=${3}
		fi
		local extension="*" # does not look for extensions by default
		if [ $# -ge 4 ]; then
			extension="${4}"
		fi
		# * Check arguments
		CheckIfFolderExists "${inputDirectory}"
		CheckIfFolderExists "$(dirname ${outputFile})"
		# * Get absolute path of the input directory so that `find` lists absolute paths as well
		cd "${inputDirectory}"
		inputDirectory="$(pwd)"
		cd - > /dev/null
		# * Make an inventory and write to file
		find "${inputDirectory}" -type f | grep ".*.${extension}$" > "${outputFile}"
		DeleteAllEmptyLines "${outputFile}"
	}
	export CreateFilenameInventoryFromDirectory

	function CreateFilenameInventoryFromFileOfDirectories()
	# Feed this function a filename of a file containing directories ($1) and it will list all files within those directories including their absolute paths. The output will be written to a file ($2). If you wish, you can only list files of a certain extension ($3).
	{
		# * Import function arguments
		local inputFile="${1}"
		local outputFile="${2}"
		local extension="*" # does not look for extensions by default
		if [ $# -ge 2 ]; then
			extension=${3}
		fi
		# * Check arguments
		CheckIfFileExists   "${inputFile}"
		CheckIfFolderExists "$(dirname ${outputFile})"
		DeleteAllEmptyLines ${inputFile}
		# * Make an inventory and write to file
		echo > ${outputFile} # empty output file
		while read dir; do
			echo > temp.txt # empty temporary write file
			if [ -d ${dir} ]; then
				echo "Reading directory \"${dir}\"..."
				CreateFilenameInventoryFromDirectory "${dir}" "temp.txt" "${extension}"
				cat "temp.txt" >> "${outputFile}"
			else
				PrintErrorMessage "WARNING: Directory \"${dir}\" does not exist"
			fi
		done < ${inputFile}
		PrintSuccessMessage "Successfully read all directories from file \"${inputFile}\""
		rm temp.txt # remove temporary write file
		DeleteAllEmptyLines ${outputFile}
	}


PrintSuccessMessage "Successfully loaded \"CommonFunctions.sh\"\n"