INPUTARRAY=("$@")

TARNAME=${INPUTARRAY[0]}
Q2BIN=${INPUTARRAY[1]}
NUBIN=${INPUTARRAY[2]}

# set dirs
JOBDIR=/work/mbarrial/ClusterCodes/bin          # dir to store logs and job scripts
LOGSDIR=/work/mbarrial/ClusterCodes/logs

mkdir -p ${JOBDIR} ${LOGSDIR} # just in case

# setting jobname
jobname="Acc2_${TARNAME}_${Q2BIN}_${NUBIN}"
jobfile="${JOBDIR}/${jobname}.sh"

echo ${jobname}

echo "#!/bin/bash"                                                 > ${jobfile}
echo "#SBATCH -J ${jobname}"                                      >> ${jobfile}
echo "#SBATCH -o ${LOGSDIR}/${jobname}.out"                       >> ${jobfile}
echo "#SBATCH -e ${LOGSDIR}/${jobname}.err"                       >> ${jobfile}
echo "#SBATCH --time=4:00:00"                                     >> ${jobfile}
echo "#SBATCH --mem=1GB"                                          >> ${jobfile}
echo ""                                                           >> ${jobfile}
echo "source ${HOME}/.bashrc"                                     >> ${jobfile}
echo "cd ${JOBDIR}"                                               >> ${jobfile}
echo "../bin/AccCorrection ${TARNAME} ${Q2BIN} ${NUBIN}"                       >> ${jobfile}
echo "Submitting job: ${jobfile}"
sbatch ${jobfile} # submit job!
