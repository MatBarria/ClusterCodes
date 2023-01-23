MAINDIR=/work/mbarrial/ClusterCodes
BINDIR=${MAINDIR}/bin
SHDIR=${MAINDIR}/sh

mkdir -p ${BINDIR}

cd ${MAINDIR}

g++ -Wall -fPIC -I../include `root-config --cflags` AccCorrection2.cpp -o ./bin/AccCorrection2  `root-config --glibs` ./include/Acc.h

cd ${SHDIR}

for i in  0 1 2
do
  for j in  0 1 2 
  do
    bash AccJob2.sh C $i $j 
    bash AccJob2.sh Fe $i $j 
    bash AccJob2.sh Pb $i $j 
    bash AccJob2.sh DC $i $j 
    bash AccJob2.sh DFe $i $j 
    bash AccJob2.sh DPb $i $j 
  done
done

