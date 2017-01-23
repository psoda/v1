#!/bin/bash
for file in ./data/*.nex
do
echo "Running $file"
echo "------------------------------------------------------------------"
echo
./src/psoda $file
echo
echo "------------------------------------------------------------------"
echo "Finished $file"
echo
echo
echo
echo
echo

done
