#delete space \t \n and then show the result
sed -i 's/[ \t]*$//' $1 
cat $1