#!/bin/bash
for i in * ; do name=$( echo $i| sed -e 's@ @_@g' ) ; mv "$i" $name ; done
