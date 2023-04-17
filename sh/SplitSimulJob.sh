TARNAME="$@"

# set dirs
MAINDIR=/work/mbarrial/ClusterCodes              # dir of the program
JOBDIR=/work/mbarrial/ClusterCodes/bin          # dir to store logs and job scripts
LOGSDIR=/work/mbarrial/ClusterCodes/bin          # dir to store logs and job scripts

mkdir -p ${JOBDIR} ${LOGSDIR} # just in case

# setting jobname
jobname="SplitSimul_${TARNAME}"
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
echo "./SplitSimul ${TARNAME}"                                    >> ${jobfile}
echo "Submitting job: ${jobfile}"
sbatch ${jobfile} # submit job!
