echo "=================================="
echo "---- Start of RF Receive Test ----"

for k in `seq 1 50`;
do
    echo "=====> Test iteration no.: $k"
    sudo ./recv
done

echo "----  End of RF Receive Test  ----"
echo "=================================="
exit 0
