#show lines that arent comment in file
file=$1
sed -i '/^#/d' $file