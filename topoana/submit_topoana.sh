inputCard="${1}"
inputCard=${inputCard/.card}
echo ${inputCard}
inputCardFilename="$(basename ${inputCard})"
echo ${inputCardFilename}

echo "topoana.exe \"${inputCard}.card\" > ${inputCardFilename}.log" > "${inputCardFilename}.sh"
chmod +x "${inputCardFilename}.sh"
read -p "Submit \"${inputCardFilename}.sh\" to queue? (press ENTER to continue...) "