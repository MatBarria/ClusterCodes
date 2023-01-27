#!/bin/bash
#SBATCH -J MaxE_C
#SBATCH -o /eos/user/m/mbarrial/Pt2Broadening_multi-pion/bin/MaxEnSimul_C.out
#SBATCH -e /eos/user/m/mbarrial/Pt2Broadening_multi-pion/bin/MaxEnSimul_C.err
#SBATCH --time=4:30:00
#SBATCH --mem=1GB

./MoreEnergySimul C
