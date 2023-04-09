MAINDIR=/work/mbarrial/ClusterCodes/RCFactor
SHDIR=${MAINDIR}/sh
BINDIR=${MAINDIR}/bin

mkdir -p ${BINDIR}
cd ${MAINDIR}

g++ -Wall -fPIC -I./include `root-config --cflags` PhiHist.cpp -o ./bin/PhiHist `root-config --glibs` ./include/Acc_Rc.h


cd ${SHDIR}

bash PhiJob.sh
