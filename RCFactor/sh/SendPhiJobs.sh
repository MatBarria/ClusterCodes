MAINDIR=/work/mbarrial/Pt2Broadening_multi-pion/RCFactor
#MAINDIR=/home/matias/proyecto/Pt2Broadening_multi-pion/RCFactor
SHDIR=${MAINDIR}/sh
BINDIR=${MAINDIR}/bin

mkdir -p ${BINDIR}
cd ${BINDIR}

g++ -Wall -fPIC -I./include `root-config --cflags` PhiHist.cpp -o ./bin/PhiHist `root-config --glibs` ./include/Acc_Rc.h


cd ${SHDIR}

bash PhiJob.sh
