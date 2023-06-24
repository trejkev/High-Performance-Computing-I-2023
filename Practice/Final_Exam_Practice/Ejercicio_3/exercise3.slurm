#!/bin/bash
#SBATCH --job-name=exercise3
#SBATCH --output=resultExercise3.txt
#SBATCH --partition=nukwa
#SBATCH --ntasks=1
#SBATCH --time=00:10:00

srun ./exercise3
