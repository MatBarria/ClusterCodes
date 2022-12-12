MAINDIR=/work/mbarrial/ClusterCodes
BINDIR=${MAINDIR}/bin

mkdir -p ${BINDIR}

g++ -Wall -fPIC  `root-config --cflags` ${MAINDIR}/MoreEnergySimul.cpp -o ${BINDIR}/MoreEnergySimul  `root-config --glibs`

bash MoreEnergySimulJob.sh D
bash MoreEnergySimulJob.sh C
bash MoreEnergySimulJob.sh Fe
bash MoreEnergySimulJob.sh Pb

echo "All jobs sumitted"
