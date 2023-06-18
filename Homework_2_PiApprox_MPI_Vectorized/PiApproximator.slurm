#!/bin/bash
#SBATCH --job-name=PiApproximator
#SBATCH --output=result.txt
#SBATCH --partition=nu
#SBATCH --ntasks=1
#SBATCH --time=00:10:00

module load gcc/7.2.0

srun ./pi_threads 64 100000000000