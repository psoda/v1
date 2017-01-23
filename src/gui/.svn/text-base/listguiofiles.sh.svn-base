#!/bin/bash
echo $(for thisFile in $(grep -l "GUI" $(find ../ -name "*.h") $(find ../ -name "*.cpp")); do echo $(echo "../")$(echo $thisFile | grep -Eo "../[./]*[^.]*" | grep -Eo "[^./]*$")$(echo ".o"); done)
