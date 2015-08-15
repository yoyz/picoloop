#!/bin/bash
find * -name "*.ds" | sed -e 's@/@ @g' > patchlist.cfg
