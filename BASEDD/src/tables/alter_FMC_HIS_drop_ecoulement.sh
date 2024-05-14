##!/bin/ksh
#
#echo "
#use EXP
#go
#alter table FMC_HIS drop ecoulement
#go" | isql -Usa -Pzigramur -SSQL_DC
#
#echo "
#use HIS
#go
#alter table FMC_HIS drop ecoulement
#go" | isql -Usa -Pzigramur -SSQL_HIS
#
#echo "
#use EXP
#go
#alter table FMC_HIS drop ecoulement
#go" | isql -Usa -Pzigramur -SSQL_DS
#
#echo "
#use EXP
#go
#alter table FMC_HIS drop ecoulement
#go" | isql -Usa -Pzigramur -SSQL_DM
#
#echo "
#use EXP
#go
#alter table FMC_HIS drop ecoulement
#go" | isql -Usa -Pzigramur -SSQL_CI
