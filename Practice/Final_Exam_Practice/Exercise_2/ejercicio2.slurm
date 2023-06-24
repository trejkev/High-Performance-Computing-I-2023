#!/bin/bash
#SBATCH --job-name=ejercicio2
#SBATCH --output=resultEjercicio2.txt
#SBATCH --partition=nu
#SBATCH --ntasks=1
#SBATCH --time=00:10:00

srun ./ejercicio2 80