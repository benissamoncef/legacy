/bin/cp -fp /etc/hosts_aub_nce /etc/hosts
scp -p -o StrictHostKeyChecking=no /etc/hosts sdvc1:/etc
scp -p -o StrictHostKeyChecking=no /etc/hosts spci:/etc
scp -p -o StrictHostKeyChecking=no /etc/hosts spci2:/etc
scp -p -o StrictHostKeyChecking=no /etc/hosts spdp:/etc
scp -p -o StrictHostKeyChecking=no /etc/hosts spvc:/etc
scp -p -o StrictHostKeyChecking=no /etc/hosts podfa2:/etc
scp -p -o StrictHostKeyChecking=no /etc/hosts podp3:/etc
#scp -p -o StrictHostKeyChecking=no /etc/hosts poci3:/etc

