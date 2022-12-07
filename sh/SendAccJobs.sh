#MAINDIR=/work/mbarrial/Pt2Broadening_multi-pion
MAINDIR=/home/matias/proyecto/Pt2Broadening_multi-pion
BINDIR=${MAINDIR}/bin
SHDIR=${MAINDIR}/sh

mkdir -p ${BINDIR}

cd ${MAINDIR}

g++ -Wall -fPIC -I./include `root-config --cflags` AccCorrection3.cpp -o ./bin/AccCorrection3  `root-config --glibs` ./include/Acc.h


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

