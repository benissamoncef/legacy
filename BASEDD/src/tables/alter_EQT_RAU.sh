#!/bin/ksh
# Verification sur quelle base on pointe
( ping -c 1 sdci1| grep SDCI ) > /dev/null 
if [ $? -eq 0 ]; then
# BDD Escota
echo "Escota"
# Modif table CFT
echo "
use CFT
go
alter table EQT_RAU add adresse_ip_esclave char(20) null,
					port_ip_esclave int default 0 null,
					adresse_ip_maitre char(20) null,
					port_ip_maitre int default 0 null
go" | isql -Usa -Pzigramur -SSQL_CI

# Modif table CFG
echo "
use CFG
go
alter table EQT_RAU add adresse_ip_esclave char(20) null,
					port_ip_esclave int default 0 null,
					adresse_ip_maitre char(20) null,
					port_ip_maitre int default 0 null
go" | isql -Usa -Pzigramur -SSQL_CI

echo "
use CFG
go
alter table EQT_RAU add adresse_ip_esclave char(20) null,
					port_ip_esclave int default 0 null,
					adresse_ip_maitre char(20) null,
					port_ip_maitre int default 0 null
go" | isql -Usa -Pzigramur -SSQL_DS

echo "
use CFG
go
alter table EQT_RAU add adresse_ip_esclave char(20) null,
					port_ip_esclave int default 0 null,
					adresse_ip_maitre char(20) null,
					port_ip_maitre int default 0 null
go" | isql -Usa -Pzigramur -SSQL_DM

else
# BDD CNA
echo "CNA"

# Modif table CFT
echo "
use CFT
go
alter table EQT_RAU add adresse_ip_esclave char(20) null,
					port_ip_esclave int default 0 null,
					adresse_ip_maitre char(20) null,
					port_ip_maitre int default 0 null
go" | isql -Usa -Pzigramur -SSQL_CA

# Modif table CFG
echo "
use CFG
go
alter table EQT_RAU add adresse_ip_esclave char(20) null,
					port_ip_esclave int default 0 null,
					adresse_ip_maitre char(20) null,
					port_ip_maitre int default 0 null
go" | isql -Usa -Pzigramur -SSQL_CA

fi