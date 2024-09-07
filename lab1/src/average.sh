i=1;
sum=0;
for number in "$@"
do
    i=$((i + 1))
    sum=$(($number + $sum))
done

n=$(($i - 1))
echo "cnt= $n"
echo "avg= $(echo "scale=2; $sum/$n" | bc)"
