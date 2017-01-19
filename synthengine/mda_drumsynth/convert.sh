#!/bin/bash
#set -x 
mkdir tmp
cd tmp
unzip ../drumsynth_2_0_0.zip
mv bin/ ..
cd ..
cd bin
unzip library.zip
cd ..
mkdir wav
IFS=$'\n' ; for i in $(find bin/|grep ds$) ; do name=$( echo $i| sed -e 's@ @_@g' -e 's@/@_@g' -e 's@.ds@.wav@g' ) ;  echo "Generating $name" ; ./a.out $i wav/$name ; done ; unset IFS;
