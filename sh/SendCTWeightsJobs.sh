#MAINDIR=/work/mbarrial/Pt2Broadening_multi-pion
MAINDIR=/work/mbarrial/ClusterCodes
BINDIR=${MAINDIR}/bin
SHDIR=${MAINDIR}/sh

mkdir -p ${BINDIR}

g++ -Wall -fPIC -I${MAINDIR}/include `root-config --cflags` ${MAINDIR}/CTWeights.cpp -o ${BINDIR}/CTWeights `root-config --glibs` ./include/Acc.h

for i in  0 1 2
do
  for j in  0 1 2 
  do
    bash CTWeightsJob.sh C $i $j 
    bash CTWeightsJob.sh Fe $i $j 
    bash CTWeightsJob.sh  Pb $i $j 
    bash CTWeightsJob.sh  DC $i $j 
    bash CTWeightsJob.sh  DFe $i $j 
    bash CTWeightsJob.sh  DPb $i $j 
  done
done

