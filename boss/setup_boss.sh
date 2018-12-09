# ! Script parameters ! #
BOSSVERSION="7.0.4"
BOSSWORKAREA="/besfs/users/${USER}/BOSS_Afterburner/boss"
BOSSWORKAREAbash="/besfs/users/\${USER}/BOSS_Afterburner/boss"
CMTHOME="/afs/ihep.ac.cn/bes3/offline/Boss/cmthome/cmthome-${BOSSVERSION}"
CMTHOMENAME="cmthome"

# * Check paramteres * #
	if [ ! -d "${BOSSWORKAREA}" ]; then
		echo "ERROR: Folder \"${BOSSWORKAREA}\" (BOSSWORKAREA) does not exist"
		exit
	fi
	if [ ! -d "${CMTHOME}" ]; then
		echo "ERROR: Folder \"${CMTHOME}\" (CMTHOME) does not exist"
		exit
	fi
	echo "Loading BOSS version ${BOSSVERSION}..."

# * Main script * #
	# * Copy BOSS CMT files
	cmtPath="${BOSSWORKAREA}/${CMTHOMENAME}"
	mkdir -p "${cmtPath}" # create folder if necessary
	rm -rf "${cmtPath}/"* # empty folder if necessary
	cp -R "${CMTHOME}/"* "${cmtPath}" #! overwrite previous files!
	cd "${cmtPath}"

	# * Replace lines in `requirements`
	awk '{flag = 1}
		{sub(/#macro WorkArea "\/ihepbatch\/bes\/maqm\/workarea"/,"macro WorkArea \"'${BOSSWORKAREAbash}'/workarea\"")}
		{sub(/#path_remove CMTPATH  "\$\{WorkArea\}"/,"path_remove  CMTPATH \"${WorkArea}\"")}
		{sub(/#path_prepend CMTPATH "\$\{WorkArea\}"/,"path_prepend CMTPATH \"${WorkArea}\"")}
		{if(flag == 1) {print $0} else {next} }' \
	"${cmtPath}/requirements" > "${cmtPath}/requirements_new"
	mv  "${cmtPath}/requirements_new" "${cmtPath}/requirements"

	# * Setup BOSS environment
	echo -e "\n__________________"
	echo "source setupCMT.sh"
	source setupCMT.sh  # starts connection to the CMT
	echo -e "\n__________"
	echo "cmt config"
	cmt config          # initiates configuration
	echo -e "\n_______________"
	echo -e "source setup.sh"
	source setup.sh     # sets path variables