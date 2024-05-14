#!/bin/ksh
# Fichier : $Id: xzao_make.ksh,v 1.80 2020/09/22 08:04:55 pc2dpdy Exp $        $Revision: 1.80 $        $Date: 2020/09/22 08:04:55 $
##################################################################################
# STERIA # PROJET MIGRAZUR
##################################################################################
# SOUS-SYSTEME BASEDD
##################################################################################
# MODULE XZAO # FICHIER xzao_make.sh
##################################################################################
# DESCRIPTION DU MODULE :
#  Compile les procedures stockees du groupe XZAO sur le serveur d'un site donne.
#
##################################################################################
# HISTORIQUE DES MODIFICATIONS :
# F.VOLCIC	13/08/94	: Creation
# B.GABORIT     07/11/94        : ajout xzao01,15,16,17 (1.3)
#                               : + compil par site
# C.TORREGROSSA 22/03/95        : ajout xzao307,308,309 (1.6)
# C.TORREGROSSA 07/06/95        : ajout xzao310 (1.7)
# C.TORREGROSSA 24/10/95        : suppression xzao04 (1.12)
# C.TORREGROSSA 01/12/95        : ajout xzao190, xzao320, xzao321 (1.13)
# C.TORREGROSSA 23/01/96        : ajout xzao199 (1.14)
# C.TORREGROSSA 25/03/96        : correction nom fichier xzao190 (1.15)
# D.MISMER	06/11/96	: Ajout xzao187 188 189 205 v1.18 (DEM/1232)
# D.MISMER	12/02/97	: Ajout xzao400 401 402 403 405 407 409 v1.19 (DEM/1395)
# D.MISMER	27/02/97	: Ajout xzao410 et xzao411 v1.20 (DEM/1395 + 1417) 
# JM. GUILHOU	10/12/97	: ajout XZAO413 (dem/1534) 1.21
# GUILHOU	09/01/98	: gestion multi-sites (dem/1536) 1.23
# C.TORREGROSSA 22/01/98        : Ajout xzao418, xzao421, xzao422 (1545) 1.24
# C.TORREGROSSA 06/02/98        : Ajout xzao222 (dem/1534 1.25
# C.TORREGROSSA 23/02/98        : Suppression xzao222 (dem/1534 1.26
# P. NIEPCERON  26/06/98        : separation EXP/HIS 1.27
# GUILHOU	20/09/00	: Ajout xzao419 420 423 424 425 426   1.28
# HILMARCHER    24/03/04	: ajout 'use PRC' car base master par defaut 1.29
# JMG		27/04/40	: ajout xzao500 1.30
# HILMARCHER    01/03/05        : modif proc xzao420, a ne pas compiler sur HIS 1.32
# P.NIEPCERON   09/03/05        : ajout xzao428 1.33
# HILMARCHER    28/04/05        : ajout xzao429 v1.34
# HILMARCHER    28/04/05        : ajout xzao430, xzao431 v1.35
# HILMARCHER    10/06/05        : ajout xzao432 v1.36
# HILMARCHER    10/06/05        : modif xzao115 JMG v1.37
# HILMARCHER    13/07/05        : ajout xzao433, xzao434, xzao435 au CI seulement v1.38
# JMG		28/09/05	: ajout xzao250 xzao316 xzao451 xzao440 xzao452 xzao453
#				  xzao720 xzao511 xzao780 xzao454
# JPL		20/02/07	: ajout xzao223 (DEM 614) 1.40
# PNI		08/03/07	: ajout xzao437 (DEM602) 1.41
# JPL		20/06/07	: ajout xzao220 (DEM 647) 1.42
# JPL		20/06/07	: ajout xzao224, xzao225, xzao226 (DEM 657) 1.43
# JPL		25/06/07	: corrections: syntaxe 'then'; noms fichiers 'sp'; xzao437 pas compilee sur HIS 1.44
# JPL		26/06/07	: correction: compilation XZAO442 et non deux fois XZAO441 1.45
# JPL		26/07/07	: Ajout xzao811 (DEM 672) 1.46
# PNI		21/08/07	: Ajout xzao501 (DEM687) 1.47
# JPL		23/10/07	: Ajout xzao209 (pour DEM 706) 1.48
# JMG		13/11/07	: ajout xzao450 1.49
# GTIE		13/02/08	: ajout xzao;0166, xzao317, xzao2031,
#				        xzao600-607,   xzao620-624,   xzao978-1014 numeros pairs  1.50
# JPL		05/11/08	: Restauration modifs. versions 1.46 a 1.49 (ecrasees par la v1.50)  1.51
#
# JPL		05/11/08	: Ajout xzao02 (DEM 809)  1.52
# JMG		25/11/09	: ajout xzao350 et xzao351 1.53
# PNI           05/01/10        : XZA8801 a compiler dans les secteurs aussi 1.54
# JPL		01/10/10	: Ajout xzao150 (DEM 930)  1.55
# JPL		11/10/10	: Ajout xzao193 (DEM 948)  1.56
# JPL		29/10/10	: Ajout xzao18 (DEM 949)  1.57
# JPL		16/11/10	: Suppression xzao14 (lie a DEM 948 et 949)  1.58
# JPL		03/01/10	: Ajout xzao231 (DEM 970)  1.59
# VR		16/04/12	: Ajout xzao251 xzao256 xzao257 xzao611 xzao870 xzao318 xzao4271 (DEM/1016)  1.6
#								xzao550 xzao551 xzao552 xzao553 xzao554 xzao555 xzao556
#								xzao557 xzao558 xzao559 xzao578 xzao560 xzao561 xzao562
#								xzao563 xzao564 xzao565 xzao566 xzao567 xzao568 xzao569 xzao570
#								xzao573 xzao574 xzao575 xzao576 xzao577
# JPL		06/08/12	: Suppression xzao311, xzao312 (DEM 1033)  1.61
# JPL		19/10/12	: Ajout xzao215 (DEM 1032)  1.62
# JPL		12/08/15	: Ajout xzao111 (DEM 1138)  1.63
# JMG		28/04/16 	: ajout xzao910 (BASC RAU) 1.64
# LCL		14/11/16	: Ajout xzao650 xzao651 (VIADUCS LOT23) 1.65
# JMG		12/12/16	: correction compis xzao911 xzao912  1.66
# JPL		13/02/17	: Ajout xzao444 (DEM 1193)  1.67
# JPL		13/02/17	:  ... et xzao464 avant elle car elle en depend  1.68
# JPL		15/03/17	:  Ajout xzao465 manquante  1.69
# JPL		19/04/17	:  Ajout xzao913 (DEM 1225)  1.70
# JPL		15/03/17	: xzao400 devient xzao4001 (pour DEM 1236); ajout xzao4350, xzao461, xzao466 manquants  1.71
# JPL		11/01/17	:  Ajout xzao262 (DEM 1266)  1.72
# JPL		07/06/18	: Suppression instructions 'drop' superflues  1.73
# JPL		07/06/18	: Migration Linux: phase de precompilation; fonction de compilation unitaire  1.74
# JPL		08/06/18	: Distinction du groupe XZAO, compile en premier lieu  1.75
# JPL		11/10/18	: Suppression "drop" et compil. superflues; inclusion systematique en-tetes  1.76
# JPL		22/10/18	: Suppression xzao554; ajout xzao730; plusieurs procedures degroupees  1.77
# JPL		19/11/18	: Procedures de configuration du SAE (tcgcd) separees  1.78
# JMG 1.79	11/05/19 	:	ajout xzao145
# GRO       05/08/20    : ajout xzao960, 961 TDP_FCD DEM_SAE152 1.80
# GGY  		15/09/2023  : Adaptation pour CNA (DEM-483)
##################################################################################

# repertoire des includes
includes=$HOME/migrazur/XDMICG/inc

if [[ $# != 1 && $# != 2 ]]
then
        echo "Usage : xzao_make.ksh NomSite "
        echo "        NomSite : CI ou HIS  ou DP ou VC  ou DM ou DS ou DC ..."
        exit 1
fi

typeset -u Nom_Site=$1

Fichier_Compil="xzao${Nom_Site}"


# Directives de compilation selon le site
typeset -u Option_Site=$1
if [[ "$Option_Site" = "HIS" ]]
then
	Option_Site=HIST
fi


########################################################################################################
##
##  FONCTION     : Compiler_Procedure_XZAO
##
##  ARGUMENTS    :
##    $1 : Le nom de la procedure stockee a compiler (ex. XZAO123).
##
##  COMPTE RENDU :
##    Vrai (0) ou faux (1) selon que l'operation s'est correctement deroulee.
##
########################################################################################################

Compiler_Procedure_XZAO ()
{
	typeset -u Nom_Proc="$1"

	typeset -l Racine_Proc="$1"
	Fichier_Proc=${Racine_Proc}sp.prc

	[[ -r "${Fichier_Proc}" ]]  ||  {
		echo "   Fichier $Fichier_Proc introuvable."  >&2
		false
		return
	}

	echo "$Fichier_Proc" >> ${Fichier_Compil}.err

	echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
	echo "#include \"$includes/xzaec.h\"" >> ${Fichier_Compil}.c
	echo "#include \"$includes/xzaac.h\"" >> ${Fichier_Compil}.c

	cat ${Fichier_Proc} >> ${Fichier_Compil}.c

	cpp -E -P -C -D${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i  
	isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err

	rm ${Fichier_Compil}.c  ${Fichier_Compil}.i
}


########################################################################################################

# Effacer les procedures du module
echo " 
use PRC
go
drop procedure XZAO 
go" | isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site}


# initialiser le fichier de trace des erreurs
echo "Compilation des procedures de XZAO sur site ${Nom_Site}" > ${Fichier_Compil}.err
echo "" >> ${Fichier_Compil}.err


# Compiler les procedures du groupe XZAO
# ______________________________________

Compiler_Procedure_XZAO  XZAO05
Compiler_Procedure_XZAO  XZAO11
Compiler_Procedure_XZAO  XZAO12
Compiler_Procedure_XZAO  XZAO15
Compiler_Procedure_XZAO  XZAO16
Compiler_Procedure_XZAO  XZAO19
Compiler_Procedure_XZAO  XZAO20
Compiler_Procedure_XZAO  XZAO21
Compiler_Procedure_XZAO  XZAO22
Compiler_Procedure_XZAO  XZAO75
Compiler_Procedure_XZAO  XZAO76
Compiler_Procedure_XZAO  XZAO77
Compiler_Procedure_XZAO  XZAO79
Compiler_Procedure_XZAO  XZAO82
Compiler_Procedure_XZAO  XZAO116
Compiler_Procedure_XZAO  XZAO117
Compiler_Procedure_XZAO  XZAO122
Compiler_Procedure_XZAO  XZAO124
Compiler_Procedure_XZAO  XZAO125
Compiler_Procedure_XZAO  XZAO126
Compiler_Procedure_XZAO  XZAO127
Compiler_Procedure_XZAO  XZAO128
Compiler_Procedure_XZAO  XZAO129
Compiler_Procedure_XZAO  XZAO130
Compiler_Procedure_XZAO  XZAO131
Compiler_Procedure_XZAO  XZAO132
Compiler_Procedure_XZAO  XZAO133
Compiler_Procedure_XZAO  XZAO134
Compiler_Procedure_XZAO  XZAO135
Compiler_Procedure_XZAO  XZAO147
Compiler_Procedure_XZAO  XZAO148
Compiler_Procedure_XZAO  XZAO149
Compiler_Procedure_XZAO  XZAO152
Compiler_Procedure_XZAO  XZAO162
Compiler_Procedure_XZAO  XZAO163
Compiler_Procedure_XZAO  XZAO164
Compiler_Procedure_XZAO  XZAO185
Compiler_Procedure_XZAO  XZAO186
Compiler_Procedure_XZAO  XZAO199
Compiler_Procedure_XZAO  XZAO200
Compiler_Procedure_XZAO  XZAO201
Compiler_Procedure_XZAO  XZAO202
Compiler_Procedure_XZAO  XZAO203
Compiler_Procedure_XZAO  XZAO204
Compiler_Procedure_XZAO  XZAO205
Compiler_Procedure_XZAO  XZAO206
Compiler_Procedure_XZAO  XZAO208
Compiler_Procedure_XZAO  XZAO212
Compiler_Procedure_XZAO  XZAO214

Compiler_Procedure_XZAO  XZAO302
Compiler_Procedure_XZAO  XZAO303

if [[ "$Nom_Site" != "HIS" ]]
then
	Compiler_Procedure_XZAO  XZAO304
fi

Compiler_Procedure_XZAO  XZAO307
Compiler_Procedure_XZAO  XZAO308
Compiler_Procedure_XZAO  XZAO309
Compiler_Procedure_XZAO  XZAO317
Compiler_Procedure_XZAO  XZAO320
Compiler_Procedure_XZAO  XZAO321
Compiler_Procedure_XZAO  XZAO322
Compiler_Procedure_XZAO  XZAO410
Compiler_Procedure_XZAO  XZAO411
Compiler_Procedure_XZAO  XZAO415
Compiler_Procedure_XZAO  XZAO418
Compiler_Procedure_XZAO  XZAO422
Compiler_Procedure_XZAO  XZAO960
Compiler_Procedure_XZAO  XZAO961
Compiler_Procedure_XZAO  XZAO962

# ========================================================================================================================


# Compiler les procedures degroupees
# __________________________________

# Creer XZAO02 AVANT XZAO01 POUR CREATION TABLE PARTAGEE
Compiler_Procedure_XZAO  XZAO02

Compiler_Procedure_XZAO  XZAO01


Compiler_Procedure_XZAO  XZAO10

if [[ "$Nom_Site" != "HIS" ]]
then
	Compiler_Procedure_XZAO  XZAO13
fi

Compiler_Procedure_XZAO  XZAO17
Compiler_Procedure_XZAO  XZAO18
Compiler_Procedure_XZAO  XZAO30

Compiler_Procedure_XZAO  XZAO74
Compiler_Procedure_XZAO  XZAO78

if [[ "$Nom_Site" != "HIS" ]]
then
	Compiler_Procedure_XZAO  XZAO111
fi

{ echo "use PRC"; echo go; echo "drop procedure XZAO115"; echo go; } | isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site}
Compiler_Procedure_XZAO  XZAO115

Compiler_Procedure_XZAO  XZAO123
Compiler_Procedure_XZAO  XZAO136

Compiler_Procedure_XZAO  XZAO501

if [[ "$Nom_Site" != "HIS" ]]
then
	Compiler_Procedure_XZAO  XZAO150
fi

Compiler_Procedure_XZAO  XZAO452
Compiler_Procedure_XZAO  XZAO453
Compiler_Procedure_XZAO  XZAO454

Compiler_Procedure_XZAO  XZAO2031

if [[ "$Nom_Site" != "HIS" ]]
then
	Compiler_Procedure_XZAO  XZAO209
	Compiler_Procedure_XZAO  XZAO210
	Compiler_Procedure_XZAO  XZAO231
fi

if [[ "$Nom_Site" = "CI" ]] || [[ "$Nom_Site" = "CA" ]]
then
	Compiler_Procedure_XZAO  XZAO215
fi

Compiler_Procedure_XZAO  XZAO220
Compiler_Procedure_XZAO  XZAO223
Compiler_Procedure_XZAO  XZAO224
Compiler_Procedure_XZAO  XZAO225
Compiler_Procedure_XZAO  XZAO250
Compiler_Procedure_XZAO  XZAO251

Compiler_Procedure_XZAO  XZAO301
Compiler_Procedure_XZAO  XZAO306
Compiler_Procedure_XZAO  XZAO310
{ echo "use PRC"; echo go; echo "drop procedure XZAO316"; echo go; } | isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site}
Compiler_Procedure_XZAO  XZAO316
Compiler_Procedure_XZAO  XZAO318
Compiler_Procedure_XZAO  XZAO350

Compiler_Procedure_XZAO  XZAO4001
Compiler_Procedure_XZAO  XZAO401
Compiler_Procedure_XZAO  XZAO402

if [[ "$Nom_Site" != "HIS" ]]
then
	Compiler_Procedure_XZAO  XZAO409
fi

Compiler_Procedure_XZAO  XZAO412

{ echo "use PRC"; echo go; echo "drop procedure XZAO413"; echo go; } | isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site}
Compiler_Procedure_XZAO  XZAO413

Compiler_Procedure_XZAO  XZAO414
Compiler_Procedure_XZAO  XZAO419

if [[ "$Nom_Site" != "HIS" ]]
then
	{ echo "use PRC"; echo go; echo "drop procedure XZAO420"; echo go; } | isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site}
	Compiler_Procedure_XZAO  XZAO420
fi

Compiler_Procedure_XZAO  XZAO423
Compiler_Procedure_XZAO  XZAO424
{ echo "use PRC"; echo go; echo "drop procedure XZAO425"; echo go; } | isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site}
Compiler_Procedure_XZAO  XZAO425
Compiler_Procedure_XZAO  XZAO426
Compiler_Procedure_XZAO  XZAO427
Compiler_Procedure_XZAO  XZAO428
{ echo "use PRC"; echo go; echo "drop procedure XZAO429"; echo go; } | isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site}
Compiler_Procedure_XZAO  XZAO429
{ echo "use PRC"; echo go; echo "drop procedure XZAO430"; echo go; } | isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site}
Compiler_Procedure_XZAO  XZAO430
{ echo "use PRC"; echo go; echo "drop procedure XZAO431"; echo go; } | isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site}
Compiler_Procedure_XZAO  XZAO431
Compiler_Procedure_XZAO  XZAO432

if [[ "$Nom_Site" = "CI" ]] || [[ "$Nom_Site" = "CA" ]]
then
	Compiler_Procedure_XZAO  XZAO433
	Compiler_Procedure_XZAO  XZAO434
	Compiler_Procedure_XZAO  XZAO435
	Compiler_Procedure_XZAO  XZAO4350
fi

if [[ "$Nom_Site" != "HIS" ]]
then
	Compiler_Procedure_XZAO  XZAO437
fi

Compiler_Procedure_XZAO  XZAO440
Compiler_Procedure_XZAO  XZAO441
{ echo "use PRC"; echo go; echo "drop procedure XZAO442"; echo go; } | isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site}
Compiler_Procedure_XZAO  XZAO442
Compiler_Procedure_XZAO  XZAO443

# Creer XZAO464  AVANT XZAO444
Compiler_Procedure_XZAO  XZAO464
Compiler_Procedure_XZAO  XZAO465

Compiler_Procedure_XZAO  XZAO444


Compiler_Procedure_XZAO  XZAO500

{ echo "use PRC"; echo go; echo "drop procedure XZAO511"; echo go; } | isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site}
Compiler_Procedure_XZAO  XZAO511

Compiler_Procedure_XZAO  XZAO550
Compiler_Procedure_XZAO  XZAO551
Compiler_Procedure_XZAO  XZAO555
Compiler_Procedure_XZAO  XZAO556
Compiler_Procedure_XZAO  XZAO557
Compiler_Procedure_XZAO  XZAO5571
Compiler_Procedure_XZAO  XZAO558
Compiler_Procedure_XZAO  XZAO561

if [[ "$Nom_Site" = "CI" ]] || [[ "$Nom_Site" = "CA" ]]
then
	Compiler_Procedure_XZAO  XZAO562
fi

Compiler_Procedure_XZAO  XZAO563

if [[ "$Nom_Site" = "CI" ]] || [[ "$Nom_Site" = "CA" ]]
then
	Compiler_Procedure_XZAO  XZAO565
	Compiler_Procedure_XZAO  XZAO566
fi

Compiler_Procedure_XZAO  XZAO570
Compiler_Procedure_XZAO  XZAO573
Compiler_Procedure_XZAO  XZAO574
Compiler_Procedure_XZAO  XZAO575
Compiler_Procedure_XZAO  XZAO576
Compiler_Procedure_XZAO  XZAO5761
Compiler_Procedure_XZAO  XZAO577
Compiler_Procedure_XZAO  XZAO5771


Compiler_Procedure_XZAO  XZAO611


# LOT23 VIADUCS
Compiler_Procedure_XZAO  XZAO651


Compiler_Procedure_XZAO  XZAO780

{ echo "use PRC"; echo go; echo "drop procedure XZAO801"; echo go; } | isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site}
Compiler_Procedure_XZAO  XZAO801

if [[ "$Nom_Site" != "HIS" ]]
then
	Compiler_Procedure_XZAO  XZAO811
fi

Compiler_Procedure_XZAO  XZAO807
Compiler_Procedure_XZAO  XZAO870


Compiler_Procedure_XZAO  XZAO910
Compiler_Procedure_XZAO  XZAO911
Compiler_Procedure_XZAO  XZAO912

if [[ "$Nom_Site" = "HIS" ]]
then
	Compiler_Procedure_XZAO  XZAO913
fi


# ========================================================================================================================


# Compiler les procedures de configuration du SAE
# _______________________________________________


# Procedures groupees
# ___________________

Compiler_Procedure_XZAO  XZAO51
Compiler_Procedure_XZAO  XZAO53
Compiler_Procedure_XZAO  XZAO55
Compiler_Procedure_XZAO  XZAO57
Compiler_Procedure_XZAO  XZAO59
Compiler_Procedure_XZAO  XZAO61
Compiler_Procedure_XZAO  XZAO63
Compiler_Procedure_XZAO  XZAO64
Compiler_Procedure_XZAO  XZAO65
Compiler_Procedure_XZAO  XZAO66
Compiler_Procedure_XZAO  XZAO68
Compiler_Procedure_XZAO  XZAO69
Compiler_Procedure_XZAO  XZAO71
Compiler_Procedure_XZAO  XZAO86
Compiler_Procedure_XZAO  XZAO88
Compiler_Procedure_XZAO  XZAO90
Compiler_Procedure_XZAO  XZAO92
Compiler_Procedure_XZAO  XZAO94
Compiler_Procedure_XZAO  XZAO119
Compiler_Procedure_XZAO  XZAO121
Compiler_Procedure_XZAO  XZAO137
Compiler_Procedure_XZAO  XZAO138
Compiler_Procedure_XZAO  XZAO140
Compiler_Procedure_XZAO  XZAO141
Compiler_Procedure_XZAO  XZAO142
Compiler_Procedure_XZAO  XZAO146
Compiler_Procedure_XZAO  XZAO151
Compiler_Procedure_XZAO  XZAO155
Compiler_Procedure_XZAO  XZAO157
Compiler_Procedure_XZAO  XZAO159
Compiler_Procedure_XZAO  XZAO168
Compiler_Procedure_XZAO  XZAO170
Compiler_Procedure_XZAO  XZAO172
Compiler_Procedure_XZAO  XZAO174
Compiler_Procedure_XZAO  XZAO176
Compiler_Procedure_XZAO  XZAO178
Compiler_Procedure_XZAO  XZAO188
Compiler_Procedure_XZAO  XZAO189
Compiler_Procedure_XZAO  XZAO190

Compiler_Procedure_XZAO  XZAO405

Compiler_Procedure_XZAO  XZAO600
Compiler_Procedure_XZAO  XZAO601
Compiler_Procedure_XZAO  XZAO602
Compiler_Procedure_XZAO  XZAO603
Compiler_Procedure_XZAO  XZAO604
Compiler_Procedure_XZAO  XZAO605
Compiler_Procedure_XZAO  XZAO606
Compiler_Procedure_XZAO  XZAO607
Compiler_Procedure_XZAO  XZAO620
Compiler_Procedure_XZAO  XZAO621
Compiler_Procedure_XZAO  XZAO622
Compiler_Procedure_XZAO  XZAO623
Compiler_Procedure_XZAO  XZAO624


Compiler_Procedure_XZAO  XZAO166
Compiler_Procedure_XZAO  XZAO0166


# AAZ 16/11/2007

Compiler_Procedure_XZAO  XZAO978
Compiler_Procedure_XZAO  XZAO980
Compiler_Procedure_XZAO  XZAO994
Compiler_Procedure_XZAO  XZAO996
Compiler_Procedure_XZAO  XZAO998
Compiler_Procedure_XZAO  XZAO1000
Compiler_Procedure_XZAO  XZAO1002
Compiler_Procedure_XZAO  XZAO1004
Compiler_Procedure_XZAO  XZAO1006
Compiler_Procedure_XZAO  XZAO1008
Compiler_Procedure_XZAO  XZAO1010
Compiler_Procedure_XZAO  XZAO982
Compiler_Procedure_XZAO  XZAO984
Compiler_Procedure_XZAO  XZAO986
Compiler_Procedure_XZAO  XZAO988
Compiler_Procedure_XZAO  XZAO990
Compiler_Procedure_XZAO  XZAO1012
Compiler_Procedure_XZAO  XZAO1014


# Procedures degroupees
# _____________________


Compiler_Procedure_XZAO  XZAO67
Compiler_Procedure_XZAO  XZAO70
Compiler_Procedure_XZAO  XZAO72
Compiler_Procedure_XZAO  XZAO73

Compiler_Procedure_XZAO  XZAO80
Compiler_Procedure_XZAO  XZAO81
Compiler_Procedure_XZAO  XZAO84

Compiler_Procedure_XZAO  XZAO139

Compiler_Procedure_XZAO  XZAO143
Compiler_Procedure_XZAO  XZAO144
Compiler_Procedure_XZAO  XZAO145
Compiler_Procedure_XZAO  XZAO161
Compiler_Procedure_XZAO  XZAO180
Compiler_Procedure_XZAO  XZAO182
Compiler_Procedure_XZAO  XZAO184
Compiler_Procedure_XZAO  XZAO187

Compiler_Procedure_XZAO  XZAO450
Compiler_Procedure_XZAO  XZAO451


Compiler_Procedure_XZAO  XZAO992

if [[ "$Nom_Site" = "CI" ]] || [[ "$Nom_Site" = "CA" ]]
then
	Compiler_Procedure_XZAO  XZAO193
fi

Compiler_Procedure_XZAO  XZAO226
Compiler_Procedure_XZAO  XZAO257

if [[ "$Nom_Site" = "CI" ]] || [[ "$Nom_Site" = "CA" ]]
then
	Compiler_Procedure_XZAO  XZAO262
fi

Compiler_Procedure_XZAO  XZAO351

if [[ "$Nom_Site" = "CI" ]] || [[ "$Nom_Site" = "CA" ]]
then
	Compiler_Procedure_XZAO  XZAO403
fi

Compiler_Procedure_XZAO  XZAO407

Compiler_Procedure_XZAO  XZAO416
Compiler_Procedure_XZAO  XZAO421
Compiler_Procedure_XZAO  XZAO4290

Compiler_Procedure_XZAO  XZAO455
Compiler_Procedure_XZAO  XZAO456

if [[ "$Nom_Site" = "CI" ]] || [[ "$Nom_Site" = "CA" ]]
then
	Compiler_Procedure_XZAO  XZAO461
	Compiler_Procedure_XZAO  XZAO466
fi

Compiler_Procedure_XZAO  XZAO552
Compiler_Procedure_XZAO  XZAO553
Compiler_Procedure_XZAO  XZAO559
Compiler_Procedure_XZAO  XZAO564

Compiler_Procedure_XZAO  XZAO578


# LOT23 VIADUCS
Compiler_Procedure_XZAO  XZAO650


Compiler_Procedure_XZAO  XZAO720
Compiler_Procedure_XZAO  XZAO730

