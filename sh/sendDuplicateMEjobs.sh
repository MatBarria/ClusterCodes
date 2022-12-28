MAINDIR=/work/mbarrial/ClusterCodes
BINDIR=${MAINDIR}/bin

mkdir -p ${BINDIR}

g++ -Wall -fPIC  `root-config --cflags` ${MAINDIR}/MoreEnergySimul.cpp -o ${BINDIR}/MoreEnergySimul  `root-config --glibs`

bash DuplicateME.sh D
bash DuplicateME.sh C
bash DuplicateME.sh Fe
bash DuplicateME.sh Pb

echo "All jobs sumitted"
