#!/bin/bash
#SBATCH --job-name=ejercicio3
#SBATCH --output=resultEjercicio3.txt
#SBATCH --partition=nukwa
#SBATCH --ntasks=1
#SBATCH --time=00:10:00

srun ./ejercicio3
