#!/bin/bash
#SBATCH --job-name=Fourier_Transform
#SBATCH --output=results.txt
#SBATCH --partition=nu
#SBATCH --ntasks=1
#SBATCH --time=00:10:00

module load openmpi/4.0.5
module load gcc/11.1.0

srun ./bin/fourier_transform square_50Hz 1 16 16384 dft
