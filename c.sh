#delete space \t and then show the result
# sed -i 's/[ \t]*$//' $1 
# cat $1
result=$(cat $1 | tr -d " \t\n\r")
echo $result
echo $result > $1
