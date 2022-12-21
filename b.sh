#script for showing most frequent words in a text file
awk '{ if (++words[$0] > max) { max = words[$0]; maxW=$0 } } END { print maxW }' $1