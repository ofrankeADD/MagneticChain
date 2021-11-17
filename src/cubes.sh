#!/bin/bash -l

#$ -S /bin/bash

#$ -q klumpp2
#$ -N N3D4

#$ -M bahareh.kiani@mpikg.mpg.de
#$ -m bae

#$ -cwd
#$ -o $JOB_NAME.$JOB_ID.OUT
#$ -e $JOB_NAME.$JOB_ID.ERR

module purge
module add gcc/4.9.2
#module add intel/ps_xe_2015/compiler
#icc -03 magnChain.cpp magnChain


cd $SGE_O_WORKDIR

./magnChain

exit 0 
