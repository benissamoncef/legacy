#!/bin/ksh
# Fichier : $Id: xzae_make.sh,v 1.75 2020/07/08 09:34:19 gesconf Exp $        $Revision: 1.75 $        $Date: 2020/07/08 09:34:19 $
##################################################################################
# STERIA # PROJET MIGRAZUR
##################################################################################
# SOUS-SYSTEME BASEDD
##################################################################################
# MODULE XZAE # FICHIER xzae_make.sh
##################################################################################
# DESCRIPTION DU MODULE :
#  compile les procedures stockee de XZAE sur SQL_${Nom_Site}
#
##################################################################################
# HISTORIQUE DES MODIFICATIONS :
# C.TORREGROSSA	13/08/94	: Creation
# C.TORREGROSSA	30/08/94	: Modif ajout 
# C.TORREGROSSA	10/10/94	: Modif nom include xzae$1.h en xzaec.h
# C.TORREGROSSA	25/10/94	: Modif utilisation du script
#                                 donner le nom du site sur lequel les procedures
#                                 seront stockees
# B.GABORIT     08/11/94        : Ajout xzae21sp.prc (1.6)
# C.TORREGROSSA	14/11/94	: Ajout la compilation conditionnelle CI DN DM
# B.GABORIT     19/12/94        : Ajout xzae16sp.prc (1.8)
# B.GABORIT     10/04/95        : Ajout xzae86sp.prc (1.9)
# C.TORREGROSSA	07/07/95	: Ajout xzae10sp.prc (1.12)
# C.TORREGROSSA	04/08/95	: Ajout xzae87sp.prc, xzae88sp.prc (1.13)
# C.TORREGROSSA	22/11/95	: Ajout xzae83sp.prc (1.14)
# C.TORREGROSSA	26/03/96	: Ajout xzae19sp.prc (1.15)
# P.VERDIER     20/06/96	: DEM 76 : Ajout xzae91sp.prc, xzae92sp.prc, xzae93sp.prc, 
#					  xzae94sp.prc, xzae95sp.prc, xzae96sp.prc (1.16)
# JM. GUILHOU   22/08/96	: DEM 1123 : ajout xzae97sp.prc (1.17)
# P. VERDIER	27/08/96	: DEM 76 : Ajout xzae98sp.prc (1.18)
# C.TORREGROSSA	02/09/96	: Ajout xzae101sp.prc, xzae102sp.prc et de la 
#                                 xzae105sp.prc xzae106sp.prc xzae107sp.prc
#                                 compilation conditionnelle PC simplifie  (RADT 1.19)
# JM. GUILHOU	23/09/96	: ajout XZAE;16 sur pc simplifie (RADT 1.21)
# JM. GUILHOU	24/09/96	; ajout XZAE;108	(1.22)
# P. NIEPCERON  26/09/96        ; ajout XZAE;109	(1.21)
# P. NIEPCERON  26/09/96        ; suppression XZAE;109        (1.22)
# P. NIEPCERON  26/09/96        ; ajout XZAE;109        (1.23)
# C.TORREGROSSA	09/10/96	: Ajout xzae110sp.prc (DEM 1165) (V1.24)
# P. NIEPCERON  31/10/96	: Ajout xzae111sp.prc (DEM 1248) et XZAE;112 (1106) (v1.25)
# GUILHOU	20/05/97	: ajout xzae30sp.prc (DEM/1394) 1.26
# GUILHOU	23/05/97	: ajout flag Compilation pour XZAE;17 1.27
# GUILHOU	04/11/97	: ajout XZAE119 A XZAE140 (strada) 1.28
# GUILHOU	05/11/97	: ajout XZAE141 (strada) 1.29
# GUILHOU	08/01/98	: gestion type du PC en argument du makefile (dem/1536) 1.30
#				  ajout XZAE147
# GUILHOU	14/01/98	: ajout XZAE148 (dem/1535) 1.31
# C.TORREGROSSA	26/02/98	: ajout XZAE75 (dem/1583) 1.32
# C.TORREGROSSA	28/04/98	: Suppression de XZAE12 (dem/1638) 1.36
# P. NIEPCERON  26/06/98	: modif nom trace 1.37
# P. NIEPCERON  26/06/98        : separation EXP/HIS 1.38
# P. NIEPCERON  10/02/99	: modif interges 1.29
# GUILHOU	16/05/00	: pas de XZAE62 dans HIS 
# GUILHOU	30/08/02	: ajout compil XZAE nomade et MI2 (170 -> 175)
# CLUCHAGUE     18/03/03	: modif car oubli des echo et des drop sur prc mi2 et web (1.47) 
# P. NIEPCERON  07/03/05	: ajout xae200 201 1.48
# HILMARCHER    16/03/05        : ajout xzae203, xzae204, xzae205, xzae206 v1.49
# JMG		16/03/05	: ajout xzae551,552,553	v1.50
# JMG		27/06/05	: correction SAGA v1.50 DEM473
# CHI           28/09/05        : ajout xzae207, xzae208 v1.51
# JPL 		06/04/07	: ajout xzae209 v1.52
# JPL 		03/05/07	: compilation xzae551 au CI seul et xzae208 pas sur HIS v1.53
# AAZ		Nov 2007 : DEM 663
# JMG           15/09/08        : ajout XZAE220 XZAE178 XZAE554 XZAE555 XZAE556 XZAE641 DEM/835
# JPL		17/02/09	: Ajout de XZAE210 et suppression de XZAE19, devenue inutile (DEM 848) 1.56
# JPL		17/02/09	: Correction: xzae161, appelee par xzae42, doit etre compilee sur secteurs 1.57
# PNI		10/03/09	: xzae200 a compiler sur tous les sites 1.58
# JPL		18/03/09	: Correction syntaxe du commentaire precedent 1.59
# PNI		22/12/09	: XZAE556 a compiler dans les secteurs aussi 1.60 1.61
# PNI		04/01/10	: XZAE860 a compiler dans les secteurs aussi 1.62
# JPL		01/09/10	: ... mais XZAE556 et XZAE860 ne sont pas a compiler sur HIS 1.63
# PNI		26/05/11	: ajout xzae1270
# JPL		07/11/11	: Ajout XZAE89 (libelle localisation avec pt car. dans tous les cas)  1.65
# JPL		07/03/12	: Ajout XZAE90 selection des commentaires d'une FMC (DEM 1024)  1.66
# VR		16/04/12	: Ajout xzae82 xzae562 xzae563 xzae560 xzae565 xzae564 xzae566 xzae200 (DEM/1016)
# JPL		12/09/12	: Ajout XZAE211, XZAE212 (DEM 1037)  1.68
# JPL		19/10/12	: Ajout XZAE76, XZAE77 (DEM 1032)  1.69
# JPL		06/06/18	: Suppression instructions 'drop' superflues  1.70
# JPL		06/06/18	: Migration Linux 64 bits (DEM 1274) : phase de precompilation  1.71
# JPL		20/06/18	: Syntaxe; ajout 'use PRC' pour drop procedures  1.72
# JMG		01/12/18	: xzae230 et xzae231 AVA 1.73
# JPL		22/03/19	: Ajout XZAE570, XZAE571 (GABEB)  1.74
# AMA		08/07/20	: retrait des prc xzae230 et xzae231 AVA (1.73): modifications abandonnees
# GGY		31/05/23	:	Ajout des procs chapeau - Correction PA Consignation Bouchon (DEM-475)
# GGY   	15/09/2023  : Adaptation pour CNA (DEM-483)
# GGY		18/01/24	: Ajout xzae861_Lire_Config_Tunnel (DEM-483)
##################################################################################

# repertoire des includes
includes=$HOME

if [[ $# != 1 && $# != 2 ]]
then
	echo "syntaxe : xzae_make.sh NomSite Type"
	echo "          NomSite : CI ou HIS  ou DP ou VC  ou DM ou DS ou DC ..."
	echo "		Type : PC3 ou autre chose"
	exit 1
fi

typeset -u Nom_Site=$1

Fichier_Proc="xzae${Nom_Site}"


if [[ $# = 1 ]]
then
        type="xx"
else
        type=$2
fi


# Directives de compilation selon le site
if [[ $type != "PC3" ]]
then
	typeset -u Option_Site=$1
	if [[ "$Option_Site" = "HIS" ]]
	then
		Option_Site="HIST"
	fi
else
	Option_Site="PC_SIMPL"
fi


# Directives de compilation selon le systeme
if [[ "$SYSTEME" = "HP-UX" ]]
then
	PATH=$PATH:/usr/ccs/lbin
fi



# efface les procedures du module
echo " 
use PRC
go
drop procedure XZAE 
go" | isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site}

# initialiser le fichier de trace des erreurs
echo "trace de la creation des procedures de XZAE" > ${Fichier_Proc}.err
echo "" >> ${Fichier_Proc}.err

# cree XZAE;01
echo "xzae01sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae01sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

# cree XZAE;03
echo "xzae03sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae03sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

if [[ "$Nom_Site" != "HIS" ]]
then
# cree XZAE;08
echo "xzae08sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae08sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

# cree XZAE;10
echo "xzae10sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae10sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

# crie XZAE;30
echo "xzae30sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae30sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i
fi

# cree XZAE;66
echo "xzae66sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae66sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

# cree XZAE11
echo "xzae11sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae11sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

if [[ "$Nom_Site" != "HIS" ]]
then
# cree XZAE;16
echo "xzae16sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae16sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

# cree XZAE;14
echo "xzae14sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae14sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i
fi

# cree XZAE;40
echo "xzae40sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae40sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

# cree XZAE;43
echo "xzae43sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae43sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

# cree XZAE;49
echo "xzae49sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae49sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

if [[ "$Nom_Site" != "HIS" ]]
then
# cree XZAE;61
echo "xzae61sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae61sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

# cree XZAE;64
echo "xzae64sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae64sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i
fi

# cree XZAE;68
echo "xzae68sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae68sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i


# cree XZAE;70
echo "xzae70sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae70sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

# cree XZAE82
echo "xzae82sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae82sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

# cree XZAE;85
echo "xzae85sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae85sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

if [[ "$Nom_Site" != "HIS" ]]
then
# cree XZAE;87
echo "xzae87sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae87sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i
fi

# cree XZAE;88
echo "xzae88sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae88sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

# cree XZAE90
echo "xzae90sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae90sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

# cree XZAE;91
echo "xzae91sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae91sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

# cree XZAE;92
echo "xzae92sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae92sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

# cree XZAE;93
echo "xzae93sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae93sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

# cree XZAE;94
echo "xzae94sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae94sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i


# cree XZAE;102
echo "xzae102sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae102sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

if [[ "$Nom_Site" != "HIS" ]]
then
# cree XZAE;104
echo "xzae104sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae104sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i
fi

# cree XZAE;105
echo "xzae105sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae105sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

# cree XZAE;106
echo "xzae106sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae106sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

# cree XZAE;107
echo "xzae107sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae107sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i


# cree XZAE;108
echo "xzae108sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae108sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

# cree XZAE;111
echo "xzae111sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae111sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

if [[ "$Nom_Site" != "HIS" ]]
then
# cree XZAE;109
echo "xzae109sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae109sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i


# cree XZAE;110
echo "xzae110sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae110sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

# cree XZAE;112
echo "xzae112sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae112sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

# cree XZAE;113
echo "
use PRC
go
drop procedure XZAE113
go" | isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site}
echo "xzae113sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae113sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

# cree XZAE;67
echo "xzae67sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae67sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i
fi


# cree XZAE;80
echo "xzae80sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae80sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

# cree XZAE203
echo "xzae203sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae203sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

echo "
use PRC
go
drop procedure XZAE154
go" | isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site}
echo "xzae154sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae154sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

# cree XZAE;81
echo "xzae81sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae81sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i


# cree XZAE;82
echo "xzae82sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae82sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

# cree XZAE;02
echo "xzae02sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae02sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

# cree XZAE200
echo "xzae200sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae200sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

# cree XZAE205
echo "xzae205sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae205sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

# cree XZAE204
echo "xzae204sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae204sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

# cree XZAE206
echo "xzae206sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae206sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

# cree XZAE207
echo "xzae207sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae207sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

if [[ "$Nom_Site" != "HIS" ]]
then
# cree XZAE208
echo "
use PRC
go
drop procedure XZAE208
go" | isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site}
echo "xzae208sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae208sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i
fi

# cree XZAE;83
echo "xzae83sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae83sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

# cree XZAE89
echo "xzae89sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae89sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

if [[ "$Nom_Site" != "HIS" ]]
then
# cree XZAE;18
echo "xzae18sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae18sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

# cree XZAE;15
echo "xzae15sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae15sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

# cree XZAE;13
echo "xzae13sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae13sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

# cree XZAE;17
echo "xzae17sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae17sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

# cree XZAE;19
#echo "xzae19sp.prc" >> ${Fichier_Proc}.err
#echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
#echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
#cat xzae19sp.prc >> ${Fichier_Proc}.c
#cc -P -C ${Fichier_Proc}.c
#isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
#rm ${Fichier_Proc}.c ${Fichier_Proc}.i
fi

# cree XZAE;21
echo "xzae21sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae21sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

# cree XZAE;41
echo "xzae41sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae41sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i


# cree XZAE;42
echo "xzae42sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae42sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i



# cree XZAE;44
echo "xzae44sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae44sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

# cree XZAE;45
echo "xzae45sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae45sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i


# cree XZAE;46
echo "xzae46sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae46sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i


# cree XZAE;47
echo "xzae47sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae47sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i


# cree XZAE;48
echo "xzae48sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae48sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

# cree XZAE;50
echo "xzae50sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae50sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i


# cree XZAE;51
echo "xzae51sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae51sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i


# cree XZAE;52
echo "xzae52sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae52sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i


# cree XZAE76
echo "xzae76sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae76sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i


# cree XZAE211
echo "xzae211sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae211sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i


if [[ "$Nom_Site" != "HIS" ]]
then
# cree XZAE;53
echo "xzae53sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae53sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i


# cree XZAE;54
echo "xzae54sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae54sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

# cree XZAE;55
echo "xzae55sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae55sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i


# cree XZAE;56
echo "xzae56sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae56sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i


# cree XZAE;57
echo "xzae57sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae57sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i


# cree XZAE;58
echo "xzae58sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae58sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

# cree XZAE;60
echo "xzae60sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae60sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

# cree XZAE;62
echo "xzae62sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae62sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

# cree XZAE;63
echo "xzae63sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae63sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

# cree XZAE;65
echo "xzae65sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae65sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

# cree XZAE77
echo "xzae77sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae77sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

# cree XZAE212
echo "xzae212sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae212sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

fi


# cree XZAE;69
echo "xzae69sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae69sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

# cree XZAE;71
echo "xzae71sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae71sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

# cree XZAE;72
echo "xzae72sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae72sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

if [[ "$Nom_Site" != "HIS" ]]
then
# cree XZAE;86
echo "xzae86sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae86sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

# cree XZAE;96
echo "xzae96sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae96sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i
fi

# cree XZAE;95
echo "xzae95sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae95sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i


# cree XZAE;97
echo "xzae97sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae97sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

# cree XZAE;98
echo "xzae98sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae98sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

# cree XZAE;101
echo "xzae101sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae101sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

if [[ "$Nom_Site" != "HIS" ]]
then
# cree XZAE;147
echo "xzae147sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae147sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

# cree XZAE;148
echo "xzae148sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae148sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i
fi

if [[ "$Nom_Site" != "HIS" ]]
then
# cree XZAE209
echo "xzae209sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae209sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

# cree XZAE210
echo "xzae210sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae210sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i
fi


if [[ "$Nom_Site" != "HIS" ]]
then
echo "xzae200sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae200sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

echo "xzae161sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae161sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i
fi

# cree XZAE560
echo "xzae560sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae560sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

# cree XZAE564
echo "xzae564sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae564sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

# cree XZAE565
echo "xzae565sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae565sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

# cree XZAE566
echo "xzae566sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae566sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

# cree XZAE567
echo "xzae567sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae567sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i


# cree XZAE562
echo "xzae562sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae562sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

# cree XZAE563
echo "xzae563sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae563sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

#########################debut compilation sur CI uniquement ou CA ###########

if [[ "$Nom_Site" = "CI" ]] || [[ "$Nom_Site" = "CA" ]]
then
# cree XZAE75
echo "xzae75sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae75sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

echo "xzae160sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae160sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

echo "
use PRC
go
drop procedure XZAE162
go" | isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site}
echo "xzae162sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae162sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

echo "xzae163sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae163sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

echo "
use PRC
go
drop procedure XZAE164
go" | isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site}
echo "xzae164sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae164sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

echo "
use PRC
go
drop procedure XZAE165
go" | isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site}
echo "xzae165sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae165sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

echo "xzae166sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae166sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

echo "xzae167sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae167sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

echo "xzae168sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae168sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i



echo "
use PRC
go
drop procedure XZAE170
go" | isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site}
echo "xzae170sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae170sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i


echo "
use PRC
go
drop procedure XZAE171
go" | isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site}
echo "xzae171sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae171sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i


echo "xzae172sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae172sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

echo "xzae175sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae175sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

echo "
use PRC
go
drop procedure XZAE178
go" | isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site}
echo "xzae178sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae178sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i


echo "xzae185sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae185sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i


echo "xzae201sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae201sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

echo "xzae220sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae220sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i




fi


#########################################################################


#proc STRADA
if [[ "$Nom_Site" = "CI" ]] || [[ "$Nom_Site" = "CA" ]]
then
echo "
use PRC
go
drop procedure XZAE120
go" | isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site}
echo "xzae120sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae120sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

echo "xzae121sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae121sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

echo "
use PRC
go
drop procedure XZAE122
go" | isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site}
echo "xzae122sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae122sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

echo "
use PRC
go
drop procedure XZAE123
go" | isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site}
echo "xzae123sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae123sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

echo "
use PRC
go
drop procedure XZAE124
go" | isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site}
echo "xzae124sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae124sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

echo "
use PRC
go
drop procedure XZAE125
go" | isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site}
echo "xzae125sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae125sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

echo "xzae126sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae126sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

echo "xzae127sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae127sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

echo "
use PRC
go
drop procedure XZAE128
go" | isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site}
echo "xzae128sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae128sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

echo "xzae129sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae129sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

echo "
use PRC
go
drop procedure XZAE130
go" | isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site}
echo "xzae130sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae130sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

echo "
use PRC
go
drop procedure XZAE131
go" | isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site}
echo "xzae131sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae131sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

echo "
use PRC
go
drop procedure XZAE132
go" | isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site}
echo "xzae132sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae132sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

echo "
use PRC
go
drop procedure XZAE133
go" | isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site}
echo "xzae133sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae133sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

echo "
use PRC
go
drop procedure XZAE134
go" | isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site}
echo "xzae134sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae134sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

echo "
use PRC
go
drop procedure XZAE135
go" | isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site}
echo "xzae135sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae135sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

echo "xzae136sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae136sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

echo "xzae137sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae137sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

echo "xzae138sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae138sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

echo "
use PRC
go
drop procedure XZAE139
go" | isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site}
echo "xzae139sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae139sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

echo "xzae140sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae140sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

echo "xzae141sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae141sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

echo "
use PRC
go
drop procedure XZAE142
go" | isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site}
echo "xzae142sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae142sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

echo "
use PRC
go
drop procedure XZAE143
go" | isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site}
echo "xzae143sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae143sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

echo "
use PRC
go
drop procedure XZAE144
go" | isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site}
echo "xzae144sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae144sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

echo "
use PRC
go
drop procedure XZAE145
go" | isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site}
echo "xzae145sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae145sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

echo "xzae146sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae146sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

echo "
use PRC
go
drop procedure XZAE149
go" | isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site}
echo "xzae149sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae149sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

echo "
use PRC
go
drop procedure XZAE150
go" | isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site}
echo "xzae150sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae150sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

echo "xzae152sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae152sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

echo "
use PRC
go
drop procedure XZAE153
go" | isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site}
echo "xzae153sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae153sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

fi

echo "
use PRC
go
drop procedure XZAE155
go" | isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site}
echo "xzae155sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae155sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

## 				
## fonction depannage du SAE vers Assistance VA, abandon pour cause de cout 						
##								
## echo "
## use PRC
## go
## drop procedure XZAE230
## go" | isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site}
## echo "xzae230sp.prc" >> ${Fichier_Proc}.err
## echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
## echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
## cat xzae230sp.prc >> ${Fichier_Proc}.c
## cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
## isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
## rm ${Fichier_Proc}.c ${Fichier_Proc}.i

## echo "
## use PRC
## go
## drop procedure XZAE231
## go" | isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site}
## echo "xzae231sp.prc" >> ${Fichier_Proc}.err
## echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
## echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
## cat xzae231sp.prc >> ${Fichier_Proc}.c
## cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
## isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
## rm ${Fichier_Proc}.c ${Fichier_Proc}.i
## 


if [[ "$Nom_Site" = "HIS" ]]
then
echo "
use PRC
go
drop procedure XZAE145
go" | isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site}
echo "xzae145sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae145sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i
fi

if [[ "$Nom_Site" = "CI" ]] || [[ "$Nom_Site" = "CA" ]]
then
echo "
use PRC
go
drop procedure XZAE550
go" | isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site}
echo "xzae550sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae550sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i
fi

if [[ "$Nom_Site" = "CI" ]] || [[ "$Nom_Site" = "CA" ]]
then
echo "xzae551sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae551sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i
fi

if [[ "$Nom_Site" = "CI" ]] || [[ "$Nom_Site" = "CA" ]]
then
echo "xzae552sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae552sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i
fi

if [[ "$Nom_Site" = "CI" ]] || [[ "$Nom_Site" = "CA" ]]
then
echo "
use PRC
go
drop procedure XZAE553
go" | isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site}
echo "xzae553sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae553sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i
fi

if [[ "$Nom_Site" = "CI" ]] || [[ "$Nom_Site" = "CA" ]]
then
echo "
use PRC
go
drop procedure XZAE554
go" | isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site}
echo "xzae554sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae554sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i
fi

if [[ "$Nom_Site" = "CI" ]] || [[ "$Nom_Site" = "CA" ]]
then
echo "
use PRC
go
drop procedure XZAE555
go" | isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site}
echo "xzae555sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae555sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i
fi

if [[ "$Nom_Site" != "HIS" ]]
then
echo "
use PRC
go
drop procedure XZAE556
go" | isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site}
echo "xzae556sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae556sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

echo "xzae860sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae860sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

echo "xzae861sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae861sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i
fi

if [[ "$Nom_Site" = "CI" ]] || [[ "$Nom_Site" = "CA" ]]
then
echo "
use PRC
go
drop procedure XZAE641
go" | isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site}
echo "xzae641sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae641sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

echo "xzae1270sp.prc" >> ${Fichier_Proc}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
cat xzae1270sp.prc >> ${Fichier_Proc}.c
cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
rm ${Fichier_Proc}.c ${Fichier_Proc}.i

fi

if [[ "$Nom_Site" != "HIS" ]]
then
	echo "xzae570sp.prc" >> ${Fichier_Proc}.err
	echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
	echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
	cat xzae570sp.prc >> ${Fichier_Proc}.c
	cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
	isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
	rm ${Fichier_Proc}.c ${Fichier_Proc}.i

	echo "xzae571sp.prc" >> ${Fichier_Proc}.err
	echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
	echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
	cat xzae571sp.prc >> ${Fichier_Proc}.c
	cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
	isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
	rm ${Fichier_Proc}.c ${Fichier_Proc}.i

	# Procs Chapeau (DEM-475)
	echo "xzae008sp.prc" >> ${Fichier_Proc}.err
	echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
	echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
	cat xzae008sp.prc >> ${Fichier_Proc}.c
	cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
	isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
	rm ${Fichier_Proc}.c ${Fichier_Proc}.i

	echo "xzae011sp.prc" >> ${Fichier_Proc}.err
	echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
	echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
	cat xzae011sp.prc >> ${Fichier_Proc}.c
	cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
	isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
	rm ${Fichier_Proc}.c ${Fichier_Proc}.i

	echo "xzae015sp.prc" >> ${Fichier_Proc}.err
	echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
	echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
	cat xzae015sp.prc >> ${Fichier_Proc}.c
	cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
	isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
	rm ${Fichier_Proc}.c ${Fichier_Proc}.i

	echo "xzae060sp.prc" >> ${Fichier_Proc}.err
	echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
	echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
	cat xzae060sp.prc >> ${Fichier_Proc}.c
	cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
	isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
	rm ${Fichier_Proc}.c ${Fichier_Proc}.i

	echo "xzae0210sp.prc" >> ${Fichier_Proc}.err
	echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
	echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
	cat xzae0210sp.prc >> ${Fichier_Proc}.c
	cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
	isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
	rm ${Fichier_Proc}.c ${Fichier_Proc}.i

	echo "xzae040sp.prc" >> ${Fichier_Proc}.err
	echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
	echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
	cat xzae040sp.prc >> ${Fichier_Proc}.c
	cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
	isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
	rm ${Fichier_Proc}.c ${Fichier_Proc}.i

	echo "xzae051sp.prc" >> ${Fichier_Proc}.err
	echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Proc}.c
	echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Proc}.c
	cat xzae051sp.prc >> ${Fichier_Proc}.c
	cpp -E -P -C -D${Option_Site} ${Fichier_Proc}.c  > ${Fichier_Proc}.i  2>&-
	isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Proc}.i >> ${Fichier_Proc}.err
	rm ${Fichier_Proc}.c ${Fichier_Proc}.i

fi

