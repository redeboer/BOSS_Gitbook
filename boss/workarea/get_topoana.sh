TopoAnaPath="/besfs/users/zhouxy/tools/topoana"
TargetPath="Analysis/TopoAna"
VersionToCopy="1.6.2"

echo "Which version of \"topoana\" do you want to load?"
echo "(if left empty, version ${VersionToCopy} will be used)"
read tempVersion

if [ "${tempVersion}" != "" ]; then
	VersionToCopy="${tempVersion}"
fi

PathToCopy="${TopoAnaPath}/v${VersionToCopy}"
if [ ! -d "${PathToCopy}" ]; then
	echo "ERROR: path \"${PathToCopy}\" does not exist"
	exit 0
fi
if [ ! -d "${TargetPath}" ]; then
	echo "ERROR: output path \"${TargetPath}\" does not exist"
	exit 0
fi

TargetPath="${TargetPath}/v${VersionToCopy}"
mkdir -p "${TargetPath}/v${VersionToCopy}"
rm -rf "${TargetPath}/v${VersionToCopy}/"*

echo "Copying \"${PathToCopy}\""
echo "     to \"${TargetPath}\" ..."
cp -fR "${PathToCopy}/"* "${TargetPath}/"