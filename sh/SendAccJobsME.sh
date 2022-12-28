MAINDIR=/work/mbarrial/ClusterCodes
BINDIR=${MAINDIR}/bin
SHDIR=${MAINDIR}/sh

mkdir -p ${BINDIR}

cd ${MAINDIR}

g++ -Wall -fPIC -I./include `root-config --cflags` AccCorrectionMe.cpp -o ./bin/ACCME `root-config --glibs` ./include/Acc.h

cd ${SHDIR}

for i in  0 1 2
do
  for j in  0 1 2 
  do
    bash AccJobME.sh C $i $j 
    bash AccJobME.sh Fe $i $j 
    bash AccJobME.sh Pb $i $j 
    bash AccJobME.sh DC $i $j 
    bash AccJobME.sh DFe $i $j 
    bash AccJobME.sh DPb $i $j 
  done
done

