echo "Starting RF recv test script."
for count in {1..50}
do
echo "$count"
sudo ./recv
done
exit 0
