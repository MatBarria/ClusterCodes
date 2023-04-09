INPUTARRAY=("$@")

TARNAME=${INPUTARRAY[0]}
NPION=${INPUTARRAY[1]}

MAINDIR=/work/mbarrial/ClusterCodes/RCFactor
BINDIR=${MAINDIR}/GenRC
SHDIR=${MAINDIR}/sh
DATADIR=/work/mbarrial/Data/60/RCFactors

cd ${BINDIR}

rm newphihist.root
cp ${DATADIR}/${TARNAME}newphihist${NPION}.root ./newphihist.root

cd ${SHDIR}

for i in  0 1 2
do
  for j in  0 1 2 
  do
    for k in  0 1 2 3 4 5 6
    do
        #bash RCJob.sh ${TARNAME} ${NPION} $i $j $k
        echo "-.-"
    done
  done
done


bash RCJob.sh ${TARNAME} ${NPION} 0 2 $k
bash RCJob.sh ${TARNAME} ${NPION} 1 2 $k
