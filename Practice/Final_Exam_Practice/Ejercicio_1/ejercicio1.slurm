#!/bin/bash
#SBATCH --job-name=ejercicio1
#SBATCH --output=resultEjercicio1.txt
#SBATCH --partition=nu
#SBATCH --ntasks=32
#SBATCH --time=00:10:00

module load mpich/4.1.1

srun ./ejercicio1 3