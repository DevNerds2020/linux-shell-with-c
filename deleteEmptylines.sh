# delete all empty lines from file
file=$1
sed -i '/^$/d' $file