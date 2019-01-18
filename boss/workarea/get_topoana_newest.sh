SourceRootPath="/besfs/users/zhouxy"
TargetPath="Analysis/TopoAna"
TopoAna="$(find ${SourceRootPath}/tools/topoana -type d | grep -E ".*v[0-9]+\.[0-9]+\.[0-9]+$" | sort | tail -n1)"
MctruthForTopoAnaAlg="$(find ${SourceRootPath}/workarea/workarea-*/Analysis/Physics/MctruthForTopoAnaAlg/ -type d | grep -E ".*-[0-9][0-9]-[0-9][0-9]-[0-9][0-9]$" | sort | tail -n1)"

# * Check input paths * #
	function CheckPath()
	{
		pathToCheck="${1}"
		if [ ! -d "${pathToCheck}" ]; then
			echo "ERROR: path \"${pathToCheck}\" does not exist"
			exit 0
		fi
	}
	CheckPath "${TopoAna}"
	CheckPath "${MctruthForTopoAnaAlg}"
	CheckPath "${TargetPath}"

# * Get base folder names * #
	TopoAna_base="$(basename ${TopoAna})"
	MctruthForTopoAnaAlg_base="$(basename ${MctruthForTopoAnaAlg})"

# * Ask user to continue * #
	echo "This will copy and OVERWRITE the latest version of the \"topoana\" package (${TopoAna_base}) and of \"${MctruthForTopoAnaAlg_base}\""
	echo
	read -p "  Press ENTER to continue..."


# * Empty output path * #
	rm -rf "${TargetPath}/${TopoAna_base}"
	rm -rf "${TargetPath}/${MctruthForTopoAnaAlg}"

# * Empty output path * #
	function CopyTheThing()
	{
		pathToCopy="${1}"
		echo "Copying \"$(basename ${pathToCopy})\" to \"${TargetPath}\" ..."
		cp -fR "${pathToCopy}" "${TargetPath}"
	}
	CopyTheThing "${TopoAna}"
	CopyTheThing "${MctruthForTopoAnaAlg}"