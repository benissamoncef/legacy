rm /tmp/smsok.txt
echo $3 > /tmp/smsok.txt
cat /produits/migrazur/appliSD/fichiers/mod/sms.txt | sed -e "s/<nom>/$1/g" > /tmp/sms.tmp 
cat /tmp/sms.tmp | sed -e "s/<tel_depanneur>/$2/g" >> /tmp/smsok.txt
rm /tmp/sms.tmp
