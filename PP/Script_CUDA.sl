#!/bin/bash

#SBATCH --job-name=myjob_gpu
#SBATCH --nodes=1
#SBATCH --gres=gpu
#SBATCH --time=11:59:00

module load cuda
nvcc -g -G -O0 -lcublas -lcurand -o seq.bin seq.cu
./seq.bin
