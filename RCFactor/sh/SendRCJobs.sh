INPUTARRAY=("$@")

TARNAME=${INPUTARRAY[0]}
NPION=${INPUTARRAY[1]}

MAINDIR=/work/mbarrial/ClusterCodes/RCFactor
BINDIR=${MAINDIR}/GenRC
SHDIR=${MAINDIR}/sh
DATADIR=/work/mbarrial/Data/RCNominal

cd ${BINDIR}

rm newphihist.root
cp ${DATADIR}/${TARNAME}newphihist${NPION}.root ./newphihist.root

cd ${SHDIR}

for i in  0 1 2
do
  for j in  0 1 2 
  do
    bash RCJob.sh ${TARNAME} ${NPION} $i $j 
  done
done

