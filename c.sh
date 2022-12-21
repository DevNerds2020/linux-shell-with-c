#delete all white spaces and \n from each line and show the result
sed -i 's/ //g' $1 | cat $1
# cat $1 | tr -d ' \t\n