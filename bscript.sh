#script for showing most frequent words in a text file
#usage: bscript.sh filename
filename=$1
awk '{ if (++words[$0] > max) { max = words[$0]; maxW=$0 } } END { print maxW }' $filename