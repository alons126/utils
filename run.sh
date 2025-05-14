#!/bin/bash

export PyDir=/pnfs/genie/scratch/users/asportes/2N_Analysis_Samples/Ar40/G18_10a_00_000/402962MeV
export List_File="list.txt"
ls  PyDir/"master-routine_validation_01-eScattering >" + PyDir/List_File

echo "- Re-pulling repository -----------------------------------------------"
echo ""
#git reset --hard
git pull
echo ""
echo "- Lunching ------------------------------------------------------------"
echo ""
python Failed_jobs_finder.py
echo ""
echo "- Operation finished --------------------------------------------------"
