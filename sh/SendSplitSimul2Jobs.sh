MAINDIR=/work/mbarrial/ClusterCodes
BINDIR=${MAINDIR}/bin

mkdir -p ${BINDIR}

g++ -Wall -fPIC -I${MAINDIR}/include `root-config --cflags` ${MAINDIR}/SplitSimul2.cpp -o ${BINDIR}/SplitSimul `root-config --glibs` ${MAINDIR}/include/Binning.h

bash SplitSimulJob.sh D
bash SplitSimulJob.sh C
bash SplitSimulJob.sh Fe
bash SplitSimulJob.sh Pb

echo "All jobs sumitted"
