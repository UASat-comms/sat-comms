echo "Starting RF send test script."
for count in {1..50}
do
sleep 2
echo "$count"
sudo ./send
done
exit 0
