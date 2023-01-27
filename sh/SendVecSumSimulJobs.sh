MAINDIR=/work/mbarrial/ClusterCodes
BINDIR=${MAINDIR}/bin
SHDIR=${MAINDIR}/sh

mkdir -p ${BINDIR}

g++ -Wall -fPIC  `root-config --cflags` ${MAINDIR}/VecSumSimul.cpp -o ${BINDIR}/VecSumSimul  `root-config --glibs`

bash VecSumSimulJob.sh D
bash VecSumSimulJob.sh C
bash VecSumSimulJob.sh Fe
bash VecSumSimulJob.sh Pb

echo "All jobs sumitted"
