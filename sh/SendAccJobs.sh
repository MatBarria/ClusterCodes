#MAINDIR=/work/mbarrial/Pt2Broadening_multi-pion
MAINDIR=/work/mbarrial/ClusterCodes
BINDIR=${MAINDIR}/bin
SHDIR=${MAINDIR}/sh

mkdir -p ${BINDIR}

g++ -Wall -fPIC -I${MAINDIR}/include `root-config --cflags` ${MAINDIR}/AccCorrection.cpp -o ${BINDIR}/AccCorrection  `root-config --glibs` ${MAINDIR}/Acc.h


cd ${SHDIR}

for i in  0 1 2
do
  for j in  0 1 2 
  do
    bash AccJob.sh C $i $j 
    bash AccJob.sh Fe $i $j 
    bash AccJob.sh Pb $i $j 
    bash AccJob.sh DC $i $j 
    bash AccJob.sh DFe $i $j 
    bash AccJob.sh DPb $i $j 
  done
done

