#!/bin/bash
for i in $(find bin/|grep ds$) ; do name=$( echo $i| sed -e 's@/@_@g' -e 's@.ds@.wav@g' ) ; ./a.out $i wav/$name ; done
