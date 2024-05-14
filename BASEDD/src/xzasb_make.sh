#!/bin/ksh
# Fichier : $Id: xzasb_make.sh,v 1.10 2009/03/18 17:43:01 gesconf Exp $      Release : $Revision: 1.10 $        Date : $Date: 2009/03/18 17:43:01 $
##################################################################################
# STERIA # PROJET MIGRAZUR
##################################################################################
# SOUS-SYSTEME BASEDD
##################################################################################
# MODULE XZAS # FICHIER xzasb_make.sh
##################################################################################
# DESCRIPTION DU MODULE :
#  compile les procedures stock휿 de XZAS10 a XZAS12 sur SQL_CI ou SQL_DN ou SQL_DM
#  ou SQL_DC ou SQL_DS
#
##################################################################################
# HISTORIQUE DES MODIFICATIONS :
# C.TORREGROSSA	27/01/95	: Creation
# C.TORREGROSSA	04/09/96	: Ajout compilation sur les serveurs DC et DS (1.2)
# P.niepceron   09/10/96	: aJout porc 50 51 52 (dem/1227) 1.3
# P.niepceron   10/10/96	: aJout proc 53 (dem/1232) 1.4
# P.niepceron   19/02/97	: Ajout proc 13 14 (dem/tdp) 1.5
# Guilhou	28/05/97	: ajout proc 15 (dem/tdp) 1.6
# Guilhou	02/12/97	: ajout XZAS54 et XZAS55 (strada) 1.7
# Guilhou	02/12/97	: ajout XZAS56 et XZAS57 (strada) 1.8
# Guilhou	09/01/98	: gestion multi-sites (dem/1536) 1.9
# JPL		18/03/09	: Drop dans PRC; XZAS15 sur HIS seulement 1.10
##################################################################################

# repertoire des includes
includes=$HOME

if [[ $# != 1 && $# != 2 ]]
then
	echo "syntaxe : xzasb_make.sh NomSite TypeSite"
	echo "          NomSite : CI ou DN ou DM ou DC ou DS ou ..."
	exit 1
fi

# efface les procedures du module
echo " 
use PRC
go
drop procedure XZAS10
go
drop procedure XZAS11
go
drop procedure XZAS12
go
drop procedure XZAS13
go
drop procedure XZAS14
go
drop procedure XZAS15
go
drop procedure XZAS50 
go
drop procedure XZAS51 
go
drop procedure XZAS52
go
drop procedure XZAS53 
go
drop procedure XZAS54
go
drop procedure XZAS55
go
drop procedure XZAS56
go
drop procedure XZAS57
go" | isql -UMIGRAZUR -PMIGRAZUR -SSQL_${1}

# initialiser le fichier de trace des erreurs
echo "trace de la creation des procedures de XZAS" > xzasb$1.err
echo "" >> xzasb$1.err

# cr휿 XZAS10
echo "xzas10sp.prc" >> xzasb$1.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > xzasb$1.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzasc.h\"" >> xzasb$1.c
cat xzas10sp.prc >> xzasb$1.c
cc -P -C -D${1} xzasb$1.c
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${1} -i xzasb$1.i >> xzasb$1.err
rm xzasb$1.c xzasb$1.i


# cr휿 XZAS11
echo "xzas11sp.prc" >> xzasb$1.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > xzasb$1.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzasc.h\"" >> xzasb$1.c
cat xzas11sp.prc >> xzasb$1.c
cc -P -C -D${1} xzasb$1.c
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${1} -i xzasb$1.i >> xzasb$1.err
rm xzasb$1.c xzasb$1.i


# cr휿 XZAS12
echo "xzas12sp.prc" >> xzasb$1.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > xzasb$1.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzasc.h\"" >> xzasb$1.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> xzasb$1.c
cat xzas12sp.prc >> xzasb$1.c
cc -P -C -D${1} xzasb$1.c
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${1} -i xzasb$1.i >> xzasb$1.err
rm xzasb$1.c xzasb$1.i

# cr휿 XZAS13
echo "xzas13sp.prc" >> xzasb$1.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > xzasb$1.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzasc.h\"" >> xzasb$1.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> xzasb$1.c
cat xzas13sp.prc >> xzasb$1.c
cc -P -C -D${1} xzasb$1.c
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${1} -i xzasb$1.i >> xzasb$1.err
rm xzasb$1.c xzasb$1.i

# cr휿 XZAS14
echo "xzas14sp.prc" >> xzasb$1.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > xzasb$1.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzasc.h\"" >> xzasb$1.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> xzasb$1.c
cat xzas14sp.prc >> xzasb$1.c
cc -P -C -D${1} xzasb$1.c
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${1} -i xzasb$1.i >> xzasb$1.err
rm xzasb$1.c xzasb$1.i

if [[ $1 = "HIS" ]]
then
# cr휿 XZAS15
echo "xzas15sp.prc" >> xzasb$1.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > xzasb$1.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzasc.h\"" >> xzasb$1.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> xzasb$1.c
cat xzas15sp.prc >> xzasb$1.c
cc -P -C -D${1} xzasb$1.c
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${1} -i xzasb$1.i >> xzasb$1.err
rm xzasb$1.c xzasb$1.i
fi


# crEe XZAS50
echo "xzas50sp.prc" >> xzasb$1.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > xzasb$1.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzasc.h\"" >> xzasb$1.c
cat xzas50sp.prc >> xzasb$1.c
cc -P -C -D${1} xzasb$1.c
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${1} -i xzasb$1.i >> xzasb$1.err
rm xzasb$1.c xzasb$1.i

# crEe XZAS51
echo "xzas51sp.prc" >> xzasb$1.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > xzasb$1.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzasc.h\"" >> xzasb$1.c
cat xzas51sp.prc >> xzasb$1.c
cc -P -C -D${1} xzasb$1.c
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${1} -i xzasb$1.i >> xzasb$1.err
rm xzasb$1.c xzasb$1.i


# crEe XZAS52
echo "xzas52sp.prc" >> xzasb$1.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > xzasb$1.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzasc.h\"" >> xzasb$1.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> xzasb$1.c
cat xzas52sp.prc >> xzasb$1.c
cc -P -C -D${1} xzasb$1.c
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${1} -i xzasb$1.i >> xzasb$1.err
rm xzasb$1.c xzasb$1.i

# crEe XZAS53
echo "xzas53sp.prc" >> xzasb$1.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > xzasb$1.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzasc.h\"" >> xzasb$1.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> xzasb$1.c
cat xzas53sp.prc >> xzasb$1.c
cc -P -C -D${1} xzasb$1.c
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${1} -i xzasb$1.i >> xzasb$1.err
rm xzasb$1.c xzasb$1.i

# crEe XZAS54
echo "xzas54sp.prc" >> xzasb$1.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > xzasb$1.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzasc.h\"" >> xzasb$1.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> xzasb$1.c
cat xzas54sp.prc >> xzasb$1.c
cc -P -C -D${1} xzasb$1.c
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${1} -i xzasb$1.i >> xzasb$1.err
rm xzasb$1.c xzasb$1.i

# crEe XZAS55
echo "xzas55sp.prc" >> xzasb$1.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > xzasb$1.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzasc.h\"" >> xzasb$1.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> xzasb$1.c
cat xzas55sp.prc >> xzasb$1.c
cc -P -C -D${1} xzasb$1.c
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${1} -i xzasb$1.i >> xzasb$1.err
rm xzasb$1.c xzasb$1.i

# crEe XZAS56
echo "xzas56sp.prc" >> xzasb$1.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > xzasb$1.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzasc.h\"" >> xzasb$1.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> xzasb$1.c
cat xzas56sp.prc >> xzasb$1.c
cc -P -C -D${1} xzasb$1.c
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${1} -i xzasb$1.i >> xzasb$1.err
rm xzasb$1.c xzasb$1.i

# crEe XZAS57
echo "xzas57sp.prc" >> xzasb$1.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > xzasb$1.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzasc.h\"" >> xzasb$1.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> xzasb$1.c
cat xzas57sp.prc >> xzasb$1.c
cc -P -C -D${1} xzasb$1.c
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${1} -i xzasb$1.i >> xzasb$1.err
rm xzasb$1.c xzasb$1.i
