MAINDIR=/work/mbarrial/ClusterCodes
BINDIR=${MAINDIR}/bin

mkdir -p ${BINDIR}

g++ -Wall -fpic  `root-config --cflags` ../duplicateSimulTuple.cpp -o ../bin/duplicateSimulTuple `root-config --glibs`

bash DuplicateSimulJob.sh D
bash DuplicateSimulJob.sh C
bash DuplicateSimulJob.sh Fe
bash DuplicateSimulJob.sh Pb

echo "All jobs sumitted"
