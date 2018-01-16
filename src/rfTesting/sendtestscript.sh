echo "Starting RF send test script."
for k in `seq 1 10`;
do
    sleep 2
    echo "Test $k"
    sudo ./send
done
exit 0
