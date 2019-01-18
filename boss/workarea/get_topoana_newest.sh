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
	echo


# * Empty output path * #
	mkdir -p "${TargetPath}/MctruthForTopoAnaAlg"
	rm -rf "${TargetPath}/${TopoAna_base}"
	rm -rf "${TargetPath}/MctruthForTopoAnaAlg/${MctruthForTopoAnaAlg}"

# * Empty output path * #
	echo "Copying \"topoana/$(basename ${TopoAna})\"..."
	cp -fR "${TopoAna}" "${TargetPath}"
	echo "Copying \"$(basename ${MctruthForTopoAnaAlg})\"..."
	cp -fR "${MctruthForTopoAnaAlg}" "${TargetPath}/MctruthForTopoAnaAlg/"