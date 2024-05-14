#
#
# COPIE DES FICHIERS DE TRACES 
#
#
rm -r /produits/migrazur/appliSD/traces/traces_photo
mkdir /produits/migrazur/appliSD/traces/traces_photo
cp /produits/migrazur/appliSD/traces/* /produits/migrazur/appliSD/traces/traces_photo/
#
#
# CLICHE DES TACHES DE LA MACHINE 
#
#
echo "========================" > /produits/migrazur/appliSD/traces/traces_photo/photo
echo "========= DATE =========" >> /produits/migrazur/appliSD/traces/traces_photo/photo
echo "========================" >> /produits/migrazur/appliSD/traces/traces_photo/photo
date >> /produits/migrazur/appliSD/traces/traces_photo/photo
echo "==========================" >> /produits/migrazur/appliSD/traces/traces_photo/photo
echo "========= ps -ef =========" >> /produits/migrazur/appliSD/traces/traces_photo/photo
echo "==========================" >> /produits/migrazur/appliSD/traces/traces_photo/photo
ps -ef >> /produits/migrazur/appliSD/traces/traces_photo/photo
echo "==================================" >> /produits/migrazur/appliSD/traces/traces_photo/photo
echo "========= ps -ef|grep ts =========" >> /produits/migrazur/appliSD/traces/traces_photo/photo
echo "==================================" >> /produits/migrazur/appliSD/traces/traces_photo/photo
ps -ef|grep ts >> /produits/migrazur/appliSD/traces/traces_photo/photo
echo "==================================" >> /produits/migrazur/appliSD/traces/traces_photo/photo
echo "========= ps -ef|grep te =========" >> /produits/migrazur/appliSD/traces/traces_photo/photo 
echo "==================================" >> /produits/migrazur/appliSD/traces/traces_photo/photo
ps -ef|grep te >> /produits/migrazur/appliSD/traces/traces_photo/photo
echo "==================================" >> /produits/migrazur/appliSD/traces/traces_photo/photo
echo "========= ps -ef|grep ti =========" >> /produits/migrazur/appliSD/traces/traces_photo/photo
echo "==================================" >> /produits/migrazur/appliSD/traces/traces_photo/photo
ps -ef|grep ti >> /produits/migrazur/appliSD/traces/traces_photo/photo
echo "=======================" >> /produits/migrazur/appliSD/traces/traces_photo/photo
echo "========= top =========" >> /produits/migrazur/appliSD/traces/traces_photo/photo
echo "=======================" >> /produits/migrazur/appliSD/traces/traces_photo/photo
top -d1 >> /produits/migrazur/appliSD/traces/traces_photo/photo
#
#
# SOCKETS 
#
#
echo "===========================" >> /produits/migrazur/appliSD/traces/traces_photo/photo
echo "========= netstat =========" >> /produits/migrazur/appliSD/traces/traces_photo/photo
echo "===========================" >> /produits/migrazur/appliSD/traces/traces_photo/photo
netstat >> /produits/migrazur/appliSD/traces/traces_photo/photo
