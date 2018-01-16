echo "=================================="
echo "----  Start of RF Send Test   ----"

for k in `seq 1 50`;
do
    sleep 2
    echo "=====> Test iteration no.: $k"
    sudo ./send
done

echo "----   End of RF Send Test    ----"
echo "=================================="

exit 0
