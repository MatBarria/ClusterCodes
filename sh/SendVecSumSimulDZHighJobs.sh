MAINDIR=/work/mbarrial/ClusterCodes
BINDIR=${MAINDIR}/bin
SHDIR=${MAINDIR}/sh

mkdir -p ${BINDIR}

cd ${MAINDIR}

g++ -Wall -fPIC  `root-config --cflags` ${MAINDIR}/VecSumSimulDZHigh.cpp -o ${BINDIR}/VecSumSimul  `root-config --glibs`

bash VecSumSimulJob.sh D
bash VecSumSimulJob.sh C
bash VecSumSimulJob.sh Fe
bash VecSumSimulJob.sh Pb

echo "All jobs sumitted"
