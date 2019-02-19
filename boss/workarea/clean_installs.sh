SCRIPTPATH="$(cd "$(dirname "$0")"; pwd -P)"

# ! Folders that contain any of the following strings will be removed ! #
	folderIdentifiers=(
		x86_64
		i386_
	)

# * Ask whether to continue * #
	echo "This script will remove everything in the InstallArea"
	echo "and all install folders containing any of the following strings:"
	for id in "${folderIdentifiers[@]}"; do
		echo "  \"${id}\""
	done
	echo "This can be useful for debugging purposes"
	echo
	echo "Press ENTER to continue..."
	read -p ""

# * Remove InstallArea * #
	echo "Removing \"InstallArea\" folder"
	rm -rf "${SCRIPTPATH}/InstallArea"

# * Remove install folders * #
	for id in "${folderIdentifiers[@]}"; do
		echo "Removing folders with identifier \"${id}\""
		for folder in "$(find "${SCRIPTPATH}" -type d | grep ${id})"; do
			rm -rf "${folder}"
		done
	done
