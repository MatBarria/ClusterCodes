MAINDIR=/work/mbarrial/ClusterCodes
BINDIR=${MAINDIR}/bin

mkdir -p ${BINDIR}

g++ -Wall -fpic  `root-config --cflags` ../duplicateSimulTuple.cpp -o ../bin/duplicateSimulTuple `root-config --glibs`

bash DuplicateME.sh D
bash DuplicateME.sh C
bash DuplicateME.sh Fe
bash DuplicateME.sh Pb

echo "All jobs sumitted"
