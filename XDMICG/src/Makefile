#
#  Fichier : $Id: Makefile,v 1.174 2017/10/10 15:37:49 devgfi Exp $        Release : $Revision: 1.174 $        Date : $Date: 2017/10/10 15:37:49 $
#
#
#  Ce fichier est dependant des variables d'environnement :
#  $HOME, $PROJECTDIR, $RTHOME, $SYBASE
#
# T.Milleville 08 Mar 1995 	: Ajout fichier xzsi.c et xzsi.h   V1.82
# O.Mercier	09 Avri 1995	: Suppression du -g		V 1.87
# T.Milleviller	12 Avri 1995	: Suppression de l'option ADM_THIERRY V1.89
# O.Mercier	13 Avri 1995	: Suppression du -g		V 1.90
# C.Torregrossa	21 Juin 1995	: Ajout fichier xzag15.c        V 1.91
# C.Torregrossa	29 Juin 1995	: Ajout fichier xzao312.c       V 1.92
# C.Torregrossa	29 Juin 1995	: Changement de lib pour fichier xzao312.c       V 1.93
# C.Torregrossa	30 Octo 1995	: Changement de lib pour fichier xzao123.c       V 1.94
# C.Torregrossa	28 Nov Octo 1995	: Ajout de fichier xzao316.c       V 1.97
# B.Gaborit	27 Dec 1995	: Ajout du ss-syst�me MAINTE V1.97 aussi !
# C.Torregrossa	15 Mai 1996	: Correction de chmod de *ksh V1.100-V1.101
# JM. GUILHOU	22 Aou 1996 	: Ajout xzae97 (DEM 1123) V1.102
# P.Niepceron	27 Aou 1996	: Ajout xzae101 102 103 v1.103
# D.Mismer	16 Sep 1996	: Ajout xzea32			 	v1.104
# P.Niepceron   03 Oct 1996	: Ajout xzas50 51 52 (eqt10)		v1.105
# D.Mismer	16 Sep 1996	: Ajout xzea33 (Pesage RADT) (DEM/1227)	v1.106
# D.Mismer	06 Nov 1996	: Ajout xzao187 188 189 196 197 198  (DEM/1232)	
#				  Ajout xzea34   		     (DEM/1232) v1.107
# P.Niepceron   25 Nov 1996     : Ajout xzas53 (DEM/1232) v1.108
# P.Niepceron   16 Jan 1997	: Ajout xzao204 (DEM/1373) v1.109
# P.Niepceron   03 Fev 1997	: Ajout xzas13 (DEM/TDP) v1.110
# D.Mismer	10 Fev 1997	: Ajout xzao400 401 402 403 404 405 406 407 408 409 (DEM/1395)	
# D.Mismer	20 Fev 1997	: Ajout xzea35 36 37 (DEM/1396)	1.113
# D.Mismer	20 Fev 1997	: Ajout xzea35 36 37 (DEM/1396)	1.113
# P.Niepceron   20 Fev 1997     : Ajout xzadxx (DEM/1396...) 1.114
# D.Mismer	27 Fev 1997	: Ajout xzao410 (DEM/1395) 1.115	
# P.Niepceron   20 Fev 1997     : Ajout xzadxx (DEM/1396) 1.116
# D.Mismer	03 Mar 1997	: Ajout xzez (DEM/1396) 1.117	
# L.Vathelot	11 Avr 1997	: Ajout xzag100.c (DEM/1420) 1.118
# D.Mismer	24 Avr 1997	: Suppression ligne en double  1.119
# L.Vathelot	02 Mai 1997	: ajout xzas54.c (DEM/Strada) 1.119
# Guilhou	09 mai 1997	: ajout xzae123.c (strada) 1.119
# Guilhou	09 Mai 1997	: ajout xzae126.c (strada) 1.120
# Guilhou	09 mai 1997	: ajout xzas55.c (strada) 1.121
# L. Vathelot	13 Mai 1997	: ajout xzae127.c (strada) 1.122
# L. Vathelot	15 Mai 1997	: correction sur erreur de compil
# Guilhou	07 oct 1997	: ajout xzae129.c  a xzae140.c (strada) 1.126
# Penot		02 Dec 1997	: ajout xzae146.c 1.128
# Guilhou	10 dec 1997	: ajout xzao413.c et xzao22.c (dem/1534) 1.129
# Mismer	19 Jan 1998	: ajout xzea38.c et xzeb.c  (dem/1539) 1.130
# C.T.		21 Jan 1998	: ajout xzao418.c, xzao420.c, xzao421.c, xzao422.c  (dem/1545) 1.131
# P.Niepceron   25 Nov 1998     : Ajout xzap23,25 (DEM/1724) 1.132 1.133 1.134
# JMG		27 Sep 2000	: Ajout xzao419.c xzao424.c xzao425.c xzao426.c 1.135
# JMG		16 jan 2001	: ajout xzas80 xzae164 xzae160 xzas81 xzae165
# JMG		02 dec 2001	: ajout xzap26
# ETR		24 jui 2002	: ajout xzae175
# JMG		24 jui 2002	: ajout xzae166
# JMG		30 aou 2002	: ajout xzae173
# JMG		27/04/04	: ajout xzap27 xzat280 et xzao500 1.138
# JMG		20/10/04	: ajout XZAS82 		1.139
#				  ajout xzac66 et xzac67
# JMG		15/12/04	: ajout xzas83 1.140
# CHI		17/03/05	: ajout xzah02 1.141
# JMG		13/06/05	: ajout procs SAGA DATEX 1.142
# JMG		13/09/05	: ajout xzac86 xzao250 xzao427 xzao428 xzao440 1.143
#				  xzao430 xzao720 xzao780 xzap28 xzap33 xzat050 xzea080
# JMG		28/09/05	: ajout xzao429 1.144
#				  xzao451 xzao431 xzao432 xzao452 xzao453
#				  xzat35 xzat36 xzea40 xzea41 xzad17 xzao450
#				  xzac03 xzac62 xzap29
# PNI		15/12/05	: ajout xzas100
# JMG		15/12/05        : ajout xzao441
# PNI		25/08/06	: ajout xzac01,06,82,92,93,61,63,84,98,99
# JPL		28/06/07	: Ajout xzao225 (DEM 657) 1.147
# LCL           04/09/08        : ajout xzae40.c xzae554.c xzae555.c xzae556.c xzlg.c DEM 835
# JMG		25/05/09	: ajout xzao350 xzao351 xzao352 
# JMG		02/07/09	: ajout xzao801 xzae860 DEM895
# JPL		07/10/10	: Ajout xzao192, xzao193 (DEM 948)  1.156
# JPL		12/11/10	: Verification de la plateforme cible (DEM 961)  1.157
# JPL		08/12/10	: Bibliotheques cibles securisees par la declaration ".PRECIOUS" (DEM 961)  1.158
# JPL		14/02/11	: Renommage de xzao429 en xzao4290 (DEM 953)  1.159
# LCL		30/01/12	: Ajout xzao461/462/463/470 (DEM 1014) v1.160
# JMG		20/02/12	: Ajout xzae980 (DEM/1015)
# VR		29/05/12	ajout PRV DEM/1016
# MG		05/06/12	: ajout xzao468 et xzao469	(DEM/PMA)
# VR		05/06/12	: ajout xzao466 et xzao465	(DEM/PMA)
# JPL		11/02/13	: Suppression de xzao316, ajout xzao.c avec fonctions xzao2X sur autoroutes  1.165
# JPL		13/02/13	: Ajout xzad.c (fonctions XZAD10x de calcul des Temps de Parcours) (DEM 1061)  1.166
# JPL		22/02/13	: Ajout xzat01, xzac610, xzac840 (pour DEM 1061)  1.167
# JMG		26/04/16 : ajout SAGA 1.168
# JPL		31/05/16 : Ajout xdf.c, fonctions generales sur fichiers (pour DEM 1167)  1.169
# LCL		15/11/16	: Ajout xzao650 xzao650 xzao651 (LOT23 VIADUCS) 1.170
# JMG		10/01/16	: ajout xzao816 SAGA 1.171
# LCL 		27/06/17	: Ajout xzao952 xzao953
# JPL		06/10/17	: Remplacement de xzao400 par xzao4001 (pour DEM 1236)  1.174
# GGY		18/01/24	: Ajout XZAE861_Basculement_Tunnel (DEM-483)
# GGY		16/02/24	: Ajout XZAO510_Ajouter_Batiment (DEM-483)
###################################################################

#
# Inhibition de l'echo des commandes
#
.SILENT:
#
# Ajout du suffixe des fichiers PRO*C ORACLE
#
.SUFFIXES: .pc 
#
##################################################################
##
# definitions de variables du makefile 
#
#
# nom du sous systeme
#

CIBLE = 9000
SSYST = XDMICG
PROJHOME  = $(HOME)/migrazur

#
# definitions de variables decrivant les differents chemins d'acces
#

DIRINC = $(PROJHOME)/$(SSYST)/inc
DIRSRC = $(PROJHOME)/$(SSYST)/src
DIRLIB = $(PROJHOME)/$(SSYST)/lib
DIREXE = $(PROJHOME)/$(SSYST)/bin
DIREXE_SHELL = $(PROJHOME)/GECONF/bin

DIRRTWORKS = $(RTHOME)/include
DIRSYBASE = $(SYBASE)/include

# Paths  communs
#
#XDMICGINC = $(PROJECTDIR)/XDMICG/inc
#XDMICGLIB = $(PROJECTDIR)/XDMICG/lib
XDMICGINC = $(PROJHOME)/XDMICG/inc
XDMICGLIB = $(PROJHOME)/XDMICG/lib

RTLINK = $(RTHOME)/bin/rtlink


# compilateur

CC = cc
#CC = purify cc


CFLAGS = -g -D_HPUX_SOURCE -Aa +e\
	-I$(DIRRTWORKS)\
	-I$(DIRSYBASE)\
	-I$(DIRINC)\
	-I$(XDMICGINC)


# lint 
OPTLINT=no
LINTDIR= /opt/SUNWspro/bin
LINT = $(LINTDIR)/lint

# Definition des flags du lint
LFLAGS=	-u\
	-I$(DIRRTWORKS)\
	-I$(DIRINC)\
        -I$(XDMICGINC)


########### Zone a modifier par le developpeur ################
#
# DEFINITION DES SOURCES D'UNE  TACHE
#
# sources d'un executable
#
# x code du sous-systeme
# yyy nom du module
# 

#
# includes
#

#Sous systeme XDMICG : Module XZAO / XZAE / XZAP / XZAA / XZAH / XZAM / XZAT

INCOA =  $(DIRINC)/xzag.h\
	$(DIRINC)/xzag25.h\
	$(DIRINC)/xzag08.h\
	$(DIRINC)/xzag09.h\
	$(DIRINC)/xzag15.h\
	$(DIRINC)/asql_com.h\
	$(DIRINC)/xzaa.h\
	$(DIRINC)/xzaa30.h\
	$(DIRINC)/xzam.h\
	$(DIRINC)/xzat.h\
	$(DIRINC)/xzao123.h\
	$(DIRINC)/xzao25.h\
	$(DIRINC)/xzao26.h\
	$(DIRINC)/xzao27.h\
	$(DIRINC)/xzao28.h\
	$(DIRINC)/xzao29.h\
	$(DIRINC)/xzao51.h\
	$(DIRINC)/xzao53.h\
	$(DIRINC)/xzao55.h\
	$(DIRINC)/xzao57.h\
	$(DIRINC)/xzao59.h\
	$(DIRINC)/xzao61.h\
	$(DIRINC)/xzao63.h\
	$(DIRINC)/xzao64.h\
	$(DIRINC)/xzao65.h\
	$(DIRINC)/xzao66.h\
	$(DIRINC)/xzao67.h\
	$(DIRINC)/xzao68.h\
	$(DIRINC)/xzao69.h\
	$(DIRINC)/xzao70.h\
	$(DIRINC)/xzao71.h\
	$(DIRINC)/xzao72.h\
	$(DIRINC)/xzao73.h\
	$(DIRINC)/xzao80.h\
	$(DIRINC)/xzao81.h\
	$(DIRINC)/xzao84.h\
	$(DIRINC)/xzao86.h\
	$(DIRINC)/xzao88.h\
	$(DIRINC)/xzao90.h\
	$(DIRINC)/xzao92.h\
	$(DIRINC)/xzao94.h\
	$(DIRINC)/xzao119.h\
	$(DIRINC)/xzao121.h\
	$(DIRINC)/xzao137.h\
	$(DIRINC)/xzao138.h\
	$(DIRINC)/xzao139.h\
	$(DIRINC)/xzao140.h\
	$(DIRINC)/xzao141.h\
	$(DIRINC)/xzao142.h\
	$(DIRINC)/xzao143.h\
	$(DIRINC)/xzao144.h\
	$(DIRINC)/xzao146.h\
	$(DIRINC)/xzao151.h\
	$(DIRINC)/xzao155.h\
	$(DIRINC)/xzao157.h\
	$(DIRINC)/xzao159.h\
	$(DIRINC)/xzao161.h\
	$(DIRINC)/xzao350.h\
	$(DIRINC)/xzao351.h\
	$(DIRINC)/xzao352.h\
	$(DIRINC)/xzao444.h\
	$(DIRINC)/xzao450.h\
	$(DIRINC)/xzao451.h\
	$(DIRINC)/xzao166.h\
	$(DIRINC)/xzao0166.h\
	$(DIRINC)/xzao168.h\
	$(DIRINC)/xzao170.h\
	$(DIRINC)/xzao994.h\
	$(DIRINC)/xzao996.h\
	$(DIRINC)/xzao998.h\
	$(DIRINC)/xzao1012.h\
	$(DIRINC)/xzao1000.h\
	$(DIRINC)/xzao1001.h\
	$(DIRINC)/xzao1003.h\
	$(DIRINC)/xzao1005.h\
	$(DIRINC)/xzao1007.h\
	$(DIRINC)/xzao1009.h\
	$(DIRINC)/xzao982.h\
	$(DIRINC)/xzao984.h\
	$(DIRINC)/xzao986.h\
	$(DIRINC)/xzao988.h\
	$(DIRINC)/xzao1014.h\
	$(DIRINC)/xzao990.h\
	$(DIRINC)/xzao172.h\
	$(DIRINC)/xzao174.h\
	$(DIRINC)/xzao176.h\
	$(DIRINC)/xzao178.h\
	$(DIRINC)/xzao978.h\
	$(DIRINC)/xzao980.h\
	$(DIRINC)/xzao180.h\
	$(DIRINC)/xzao182.h\
	$(DIRINC)/xzao184.h\
	$(DIRINC)/xzao992.h\
	$(DIRINC)/xzao187.h\
	$(DIRINC)/xzao188.h\
	$(DIRINC)/xzao189.h\
	$(DIRINC)/xzao193.h\
	$(DIRINC)/xzao197.h\
	$(DIRINC)/xzao198.h\
	$(DIRINC)/xzao196.h\
	$(DIRINC)/xzao312.h\
	$(DIRINC)/xzao403.h\
	$(DIRINC)/xzao416.h\
	$(DIRINC)/xzao816.h\
	$(DIRINC)/xzao4290.h\
	$(DIRINC)/xzao830.h\
	$(DIRINC)/xzao435.h\
	$(DIRINC)/xzao436.h\
	$(DIRINC)/xzao404.h\
	$(DIRINC)/xzao405.h\
	$(DIRINC)/xzao406.h\
	$(DIRINC)/xzao407.h\
	$(DIRINC)/xzao408.h\
	$(DIRINC)/xzao417.h\
	$(DIRINC)/xzao418.h\
	$(DIRINC)/xzao420.h\
	$(DIRINC)/xzao421.h\
	$(DIRINC)/xzao510.h\
	$(DIRINC)/xzao511.h\
	$(DIRINC)/xzao551.h\
	$(DIRINC)/xzao553.h\
	$(DIRINC)/xzao554.h\
	$(DIRINC)/xzao556.h\
	$(DIRINC)/xzao557.h\
	$(DIRINC)/xzao558.h\
	$(DIRINC)/xzao561.h\
	$(DIRINC)/xzao563.h\
	$(DIRINC)/xzao600.h\
	$(DIRINC)/xzao601.h\
	$(DIRINC)/xzao602.h\
	$(DIRINC)/xzao603.h\
	$(DIRINC)/xzao604.h\
	$(DIRINC)/xzao605.h\
	$(DIRINC)/xzao606.h\
	$(DIRINC)/xzao607.h\
	$(DIRINC)/xzao649.h\
	$(DIRINC)/xzao650.h\
	$(DIRINC)/xzao651.h\
	$(DIRINC)/xzao720.h\
	$(DIRINC)/xzao730.h\
	$(DIRINC)/xzao257.h\
	$(DIRINC)/xzao780.h\
	$(DIRINC)/xzao870.h\
	$(DIRINC)/xzao801.h\
	$(DIRINC)/xzao422.h\
	$(DIRINC)/xzao462.h\
	$(DIRINC)/xzao461.h\
	$(DIRINC)/xzao462.h\
	$(DIRINC)/xzao463.h\
	$(DIRINC)/xzao574.h\
	$(DIRINC)/xzao910.h\
	$(DIRINC)/xzao952.h\
	$(DIRINC)/xzao953.h\
	$(DIRINC)/xzav.h

SRCOA =  $(DIRSRC)/asql_com.c\
	$(DIRSRC)/xzaa.c\
	$(DIRSRC)/xzaa30.c\
	$(DIRSRC)/xzam.c\
	$(DIRSRC)/xzat.c\
	$(DIRSRC)/xzao561.c\
	$(DIRSRC)/xzao563.c\
	$(DIRSRC)/xzao123.c\
	$(DIRSRC)/xzao.c\
	$(DIRSRC)/xzao51.c\
	$(DIRSRC)/xzao53.c\
	$(DIRSRC)/xzao55.c\
	$(DIRSRC)/xzao57.c\
	$(DIRSRC)/xzao59.c\
	$(DIRSRC)/xzao61.c\
	$(DIRSRC)/xzao63.c\
	$(DIRSRC)/xzao64.c\
	$(DIRSRC)/xzao65.c\
	$(DIRSRC)/xzao66.c\
	$(DIRSRC)/xzao67.c\
	$(DIRSRC)/xzao68.c\
	$(DIRSRC)/xzao69.c\
	$(DIRSRC)/xzao70.c\
	$(DIRSRC)/xzao71.c\
	$(DIRSRC)/xzao72.c\
	$(DIRSRC)/xzao73.c\
	$(DIRSRC)/xzao80.c\
	$(DIRSRC)/xzao81.c\
	$(DIRSRC)/xzao84.c\
	$(DIRSRC)/xzao86.c\
	$(DIRSRC)/xzao88.c\
	$(DIRSRC)/xzao90.c\
	$(DIRSRC)/xzao92.c\
	$(DIRSRC)/xzao94.c\
	$(DIRSRC)/xzao119.c\
	$(DIRSRC)/xzao121.c\
	$(DIRSRC)/xzao137.c\
	$(DIRSRC)/xzao138.c\
	$(DIRSRC)/xzao139.c\
	$(DIRSRC)/xzao140.c\
	$(DIRSRC)/xzao141.c\
	$(DIRSRC)/xzao142.c\
	$(DIRSRC)/xzao143.c\
	$(DIRSRC)/xzao144.c\
	$(DIRSRC)/xzao146.c\
	$(DIRSRC)/xzao151.c\
	$(DIRSRC)/xzao155.c\
	$(DIRSRC)/xzao157.c\
	$(DIRSRC)/xzao159.c\
	$(DIRSRC)/xzao161.c\
	$(DIRSRC)/xzao350.c\
	$(DIRSRC)/xzao351.c\
	$(DIRSRC)/xzao352.c\
	$(DIRSRC)/xzao444.c\
	$(DIRSRC)/xzao450.c\
	$(DIRSRC)/xzao451.c\
	$(DIRSRC)/xzao166.c\
	$(DIRSRC)/xzao0166.c\
	$(DIRSRC)/xzao168.c\
	$(DIRSRC)/xzao170.c\
	$(DIRSRC)/xzao994.c\
	$(DIRSRC)/xzao996.c\
	$(DIRSRC)/xzao998.c\
	$(DIRSRC)/xzao1012.c\
	$(DIRSRC)/xzao1000.c\
	$(DIRSRC)/xzao1002.c\
	$(DIRSRC)/xzao1004.c\
	$(DIRSRC)/xzao1006.c\
	$(DIRSRC)/xzao1008.c\
	$(DIRSRC)/xzao1009.c\
	$(DIRSRC)/xzao1010.c\
	$(DIRSRC)/xzao982.c\
	$(DIRSRC)/xzao984.c\
	$(DIRSRC)/xzao986.c\
	$(DIRSRC)/xzao988.c\
	$(DIRSRC)/xzao1014.c\
	$(DIRSRC)/xzao990.c\
	$(DIRSRC)/xzao172.c\
	$(DIRSRC)/xzao174.c\
	$(DIRSRC)/xzao176.c\
	$(DIRSRC)/xzao178.c\
	$(DIRSRC)/xzao978.c\
	$(DIRSRC)/xzao980.c\
	$(DIRSRC)/xzao180.c\
	$(DIRSRC)/xzao182.c\
	$(DIRSRC)/xzao184.c\
	$(DIRSRC)/xzao992.c\
	$(DIRSRC)/xzao187.c\
	$(DIRSRC)/xzao188.c\
	$(DIRSRC)/xzao189.c\
	$(DIRSRC)/xzao193.c\
	$(DIRSRC)/xzao196.c\
	$(DIRSRC)/xzao197.c\
	$(DIRSRC)/xzao198.c\
	$(DIRSRC)/xzao312.c\
	$(DIRSRC)/xzao403.c\
	$(DIRSRC)/xzao416.c\
	$(DIRSRC)/xzao816.c\
	$(DIRSRC)/xzao4290.c\
	$(DIRSRC)/xzao830.c\
	$(DIRSRC)/xzao435.c\
	$(DIRSRC)/xzao436.c\
	$(DIRSRC)/xzao404.c\
	$(DIRSRC)/xzao405.c\
	$(DIRSRC)/xzao406.c\
	$(DIRSRC)/xzao407.c\
	$(DIRSRC)/xzao408.c\
	$(DIRSRC)/xzao417.c\
	$(DIRSRC)/xzao418.c\
	$(DIRSRC)/xzao420.c\
	$(DIRSRC)/xzao421.c\
	$(DIRSRC)/xzao422.c\
	$(DIRSRC)/xzao510.c\
	$(DIRSRC)/xzao511.c\
	$(DIRSRC)/xzao551.c\
	$(DIRSRC)/xzao553.c\
	$(DIRSRC)/xzao554.c\
	$(DIRSRC)/xzao571.c\
	$(DIRSRC)/xzao572.c\
	$(DIRSRC)/xzao556.c\
	$(DIRSRC)/xzao557.c\
	$(DIRSRC)/xzao910.c\
	$(DIRSRC)/xzao790.c\
	$(DIRSRC)/xzao558.c\
	$(DIRSRC)/xzao570.c\
	$(DIRSRC)/xzao578.c\
	$(DIRSRC)/xzao600.c\
	$(DIRSRC)/xzao601.c\
	$(DIRSRC)/xzao602.c\
	$(DIRSRC)/xzao603.c\
	$(DIRSRC)/xzao604.c\
	$(DIRSRC)/xzao605.c\
	$(DIRSRC)/xzao606.c\
	$(DIRSRC)/xzao607.c\
	$(DIRSRC)/xzao649.c\
	$(DIRSRC)/xzao650.c\
	$(DIRSRC)/xzao651.c\
	$(DIRSRC)/xzao720.c\
	$(DIRSRC)/xzao730.c\
	$(DIRSRC)/xzao257.c\
	$(DIRSRC)/xzao780.c\
	$(DIRSRC)/xzao870.c\
	$(DIRSRC)/xzao801.c\
	$(DIRSRC)/xzag.c\
	$(DIRSRC)/xzag15.c\
	$(DIRSRC)/xzag25.c\
	$(DIRSRC)/xzag08.c\
	$(DIRSRC)/xzag09.c\
	$(DIRSRC)/xzag100.c\
	$(DIRSRC)/xzao462.c\
	$(DIRSRC)/xzao461.c\
	$(DIRSRC)/xzao463.c\
	$(DIRSRC)/xzao574.c\
	$(DIRSRC)/xzao952.c\
	$(DIRSRC)/xzao953.c\
	$(DIRSRC)/xzav01.c\
	$(DIRSRC)/xzav03.c

INCOC =  $(DIRINC)/xzao000.h\
	$(DIRINC)/xzao50.h\
	$(DIRINC)/xzao52.h\
	$(DIRINC)/xzao54.h\
	$(DIRINC)/xzao56.h\
	$(DIRINC)/xzao58.h\
	$(DIRINC)/xzao60.h\
	$(DIRINC)/xzao62.h\
	$(DIRINC)/xzao83.h\
	$(DIRINC)/xzao85.h\
	$(DIRINC)/xzao87.h\
	$(DIRINC)/xzao89.h\
	$(DIRINC)/xzao91.h\
	$(DIRINC)/xzao93.h\
	$(DIRINC)/xzao120.h\
	$(DIRINC)/xzao145.h\
	$(DIRINC)/xzao150.h\
	$(DIRINC)/xzao95.h\
	$(DIRINC)/xzao96.h\
	$(DIRINC)/xzao97.h\
	$(DIRINC)/xzao101.h\
	$(DIRINC)/xzao102.h\
	$(DIRINC)/xzao154.h\
	$(DIRINC)/xzao98.h\
	$(DIRINC)/xzao103.h\
	$(DIRINC)/xzao104.h\
	$(DIRINC)/xzao105.h\
	$(DIRINC)/xzao107.h\
	$(DIRINC)/xzao112.h\
	$(DIRINC)/xzao113.h\
	$(DIRINC)/xzao156.h\
	$(DIRINC)/xzao165.h\
	$(DIRINC)/xzao0165.h\
	$(DIRINC)/xzao167.h\
	$(DIRINC)/xzao173.h\
	$(DIRINC)/xzao99.h\
	$(DIRINC)/xzao106.h\
	$(DIRINC)/xzao108.h\
	$(DIRINC)/xzao110.h\
	$(DIRINC)/xzao111.h\
	$(DIRINC)/xzao114.h\
	$(DIRINC)/xzao158.h\
	$(DIRINC)/xzao175.h\
	$(DIRINC)/xzao177.h\
	$(DIRINC)/xzao977.h\
	$(DIRINC)/xzao979.h\
	$(DIRINC)/xzao100.h\
	$(DIRINC)/xzao109.h\
	$(DIRINC)/xzao160.h\
	$(DIRINC)/xzao431.h\
	$(DIRINC)/xzao432.h\
	$(DIRINC)/xzao169.h\
	$(DIRINC)/xzao993.h\
	$(DIRINC)/xzao995.h\
	$(DIRINC)/xzao997.h\
	$(DIRINC)/xzao999.h\
	$(DIRINC)/xzao1011.h\
	$(DIRINC)/xzao1001.h\
	$(DIRINC)/xzao1003.h\
	$(DIRINC)/xzao1005.h\
	$(DIRINC)/xzao1007.h\
	$(DIRINC)/xzao981.h\
	$(DIRINC)/xzao983.h\
	$(DIRINC)/xzao985.h\
	$(DIRINC)/xzao987.h\
	$(DIRINC)/xzao989.h\
	$(DIRINC)/xzao1013.h\
	$(DIRINC)/xzao171.h\
	$(DIRINC)/xzao179.h\
	$(DIRINC)/xzao181.h\
	$(DIRINC)/xzao183.h\
	$(DIRINC)/xzao991.h\
	$(DIRINC)/xzao191.h\
	$(DIRINC)/xzao192.h\
	$(DIRINC)/xzao430.h\
	$(DIRINC)/xzao480.h\
	$(DIRINC)/xzao255.h\
	$(DIRINC)/xzao118.h\
	$(DIRINC)/xzao640.h\
	$(DIRINC)/xzao641.h\
	$(DIRINC)/xzao642.h\
	$(DIRINC)/xzao643.h\
	$(DIRINC)/xzao644.h\
	$(DIRINC)/xzao645.h\
	$(DIRINC)/xzao646.h\
	$(DIRINC)/xzao647.h

SRCOC =  $(DIRSRC)/xzao000.c\
	$(DIRSRC)/xzao50.c\
	$(DIRSRC)/xzao52.c\
	$(DIRSRC)/xzao54.c\
	$(DIRSRC)/xzao56.c\
	$(DIRSRC)/xzao58.c\
	$(DIRSRC)/xzao60.c\
	$(DIRSRC)/xzao62.c\
	$(DIRSRC)/xzao83.c\
	$(DIRSRC)/xzao85.c\
	$(DIRSRC)/xzao87.c\
	$(DIRSRC)/xzao89.c\
	$(DIRSRC)/xzao91.c\
	$(DIRSRC)/xzao93.c\
	$(DIRSRC)/xzao120.c\
	$(DIRSRC)/xzao145.c\
	$(DIRSRC)/xzao150.c\
	$(DIRSRC)/xzao95.c\
	$(DIRSRC)/xzao96.c\
	$(DIRSRC)/xzao97.c\
	$(DIRSRC)/xzao101.c\
	$(DIRSRC)/xzao102.c\
	$(DIRSRC)/xzao154.c\
	$(DIRSRC)/xzao98.c\
	$(DIRSRC)/xzao103.c\
	$(DIRSRC)/xzao255.c\
	$(DIRSRC)/xzao104.c\
	$(DIRSRC)/xzao105.c\
	$(DIRSRC)/xzao107.c\
	$(DIRSRC)/xzao112.c\
	$(DIRSRC)/xzao113.c\
	$(DIRSRC)/xzao156.c\
	$(DIRSRC)/xzao165.c\
	$(DIRSRC)/xzao0165.c\
	$(DIRSRC)/xzao167.c\
	$(DIRSRC)/xzao173.c\
	$(DIRSRC)/xzao99.c\
	$(DIRSRC)/xzao106.c\
	$(DIRSRC)/xzao108.c\
	$(DIRSRC)/xzao110.c\
	$(DIRSRC)/xzao111.c\
	$(DIRSRC)/xzao114.c\
	$(DIRSRC)/xzao158.c\
	$(DIRSRC)/xzao175.c\
	$(DIRSRC)/xzao177.c\
	$(DIRSRC)/xzao977.c\
	$(DIRSRC)/xzao979.c\
	$(DIRSRC)/xzao100.c\
	$(DIRSRC)/xzao109.c\
	$(DIRSRC)/xzao160.c\
	$(DIRSRC)/xzao430.c\
	$(DIRSRC)/xzao480.c\
	$(DIRSRC)/xzao431.c\
	$(DIRSRC)/xzao432.c\
	$(DIRSRC)/xzao169.c\
	$(DIRSRC)/xzao993.c\
	$(DIRSRC)/xzao995.c\
	$(DIRSRC)/xzao997.c\
	$(DIRSRC)/xzao999.c\
	$(DIRSRC)/xzao1011.c\
	$(DIRSRC)/xzao1001.c\
	$(DIRSRC)/xzao1003.c\
	$(DIRSRC)/xzao1005.c\
	$(DIRSRC)/xzao1007.c\
	$(DIRSRC)/xzao981.c\
	$(DIRSRC)/xzao983.c\
	$(DIRSRC)/xzao985.c\
	$(DIRSRC)/xzao987.c\
	$(DIRSRC)/xzao989.c\
	$(DIRSRC)/xzao1013.c\
	$(DIRSRC)/xzao171.c\
	$(DIRSRC)/xzao179.c\
	$(DIRSRC)/xzao181.c\
	$(DIRSRC)/xzao183.c\
	$(DIRSRC)/xzao991.c\
	$(DIRSRC)/xzao191.c\
	$(DIRSRC)/xzao192.c\
	$(DIRSRC)/xzao118.c\
	$(DIRSRC)/xzao640.c\
	$(DIRSRC)/xzao641.c\
	$(DIRSRC)/xzao642.c\
	$(DIRSRC)/xzao643.c\
	$(DIRSRC)/xzao644.c\
	$(DIRSRC)/xzao645.c\
	$(DIRSRC)/xzao646.c\
	$(DIRSRC)/xzao647.c

INCOL =  $(DIRINC)/xzao74.h\
	$(DIRINC)/xzao75.h\
	$(DIRINC)/xzao76.h\
	$(DIRINC)/xzao77.h\
	$(DIRINC)/xzao78.h\
	$(DIRINC)/xzao79.h\
	$(DIRINC)/xzao82.h\
	$(DIRINC)/xzao115.h\
	$(DIRINC)/xzao116.h\
	$(DIRINC)/xzao117.h\
	$(DIRINC)/xzao122.h\
	$(DIRINC)/xzao124.h\
	$(DIRINC)/xzao125.h\
	$(DIRINC)/xzao126.h\
	$(DIRINC)/xzao127.h\
	$(DIRINC)/xzao128.h\
	$(DIRINC)/xzao129.h\
	$(DIRINC)/xzao130.h\
	$(DIRINC)/xzao131.h\
	$(DIRINC)/xzao132.h\
	$(DIRINC)/xzao133.h\
	$(DIRINC)/xzao134.h\
	$(DIRINC)/xzao135.h\
	$(DIRINC)/xzao136.h\
	$(DIRINC)/xzao147.h\
	$(DIRINC)/xzao148.h\
	$(DIRINC)/xzao501.h\
	$(DIRINC)/xzao149.h\
	$(DIRINC)/xzao152.h\
	$(DIRINC)/xzao163.h\
	$(DIRINC)/xzao452.h\
	$(DIRINC)/xzao453.h\
	$(DIRINC)/xzao164.h\
	$(DIRINC)/xzao190.h\
	$(DIRINC)/xzao208.h\
	$(DIRINC)/xzao203.h\
	$(DIRINC)/xzao2031.h\
	$(DIRINC)/xzao204.h\
	$(DIRINC)/xzao205.h\
	$(DIRINC)/xzao214.h\
	$(DIRINC)/xzao225.h\
	$(DIRINC)/xzao250.h\
	$(DIRINC)/xzao251.h\
	$(DIRINC)/xzao306.h\
	$(DIRINC)/xzao4001.h\
	$(DIRINC)/xzao401.h\
	$(DIRINC)/xzao470.h\
	$(DIRINC)/xzao402.h\
	$(DIRINC)/xzao409.h\
	$(DIRINC)/xzao412.h\
	$(DIRINC)/xzao413.h\
	$(DIRINC)/xzao419.h\
	$(DIRINC)/xzao424.h\
	$(DIRINC)/xzao440.h\
	$(DIRINC)/xzao253.h\
	$(DIRINC)/xzao256.h\
	$(DIRINC)/xzao441.h\
	$(DIRINC)/xzao425.h\
	$(DIRINC)/xzao426.h\
	$(DIRINC)/xzao427.h\
	$(DIRINC)/xzao428.h\
	$(DIRINC)/xzao410.h\
	$(DIRINC)/xzao465.h\
	$(DIRINC)/xzao466.h\
	$(DIRINC)/xzao467.h\
	$(DIRINC)/xzao468.h\
	$(DIRINC)/xzao469.h

SRCOL =  $(DIRSRC)/xzao74.c\
	$(DIRSRC)/xzao75.c\
	$(DIRSRC)/xzao76.c\
	$(DIRSRC)/xzao77.c\
	$(DIRSRC)/xzao78.c\
	$(DIRSRC)/xzao79.c\
	$(DIRSRC)/xzao82.c\
	$(DIRSRC)/xzao115.c\
	$(DIRSRC)/xzao116.c\
	$(DIRSRC)/xzao117.c\
	$(DIRSRC)/xzao122.c\
	$(DIRSRC)/xzao124.c\
	$(DIRSRC)/xzao125.c\
	$(DIRSRC)/xzao126.c\
	$(DIRSRC)/xzao127.c\
	$(DIRSRC)/xzao128.c\
	$(DIRSRC)/xzao129.c\
	$(DIRSRC)/xzao130.c\
	$(DIRSRC)/xzao131.c\
	$(DIRSRC)/xzao132.c\
	$(DIRSRC)/xzao133.c\
	$(DIRSRC)/xzao134.c\
	$(DIRSRC)/xzao135.c\
	$(DIRSRC)/xzao136.c\
	$(DIRSRC)/xzao147.c\
	$(DIRSRC)/xzao148.c\
	$(DIRSRC)/xzao501.c\
	$(DIRSRC)/xzao149.c\
	$(DIRSRC)/xzao152.c\
	$(DIRSRC)/xzao163.c\
	$(DIRSRC)/xzao452.c\
	$(DIRSRC)/xzao453.c\
	$(DIRSRC)/xzao164.c\
	$(DIRSRC)/xzao190.c\
	$(DIRSRC)/xzao208.c\
	$(DIRSRC)/xzao203.c\
	$(DIRSRC)/xzao2031.c\
	$(DIRSRC)/xzao204.c\
	$(DIRSRC)/xzao205.c\
	$(DIRSRC)/xzao214.c\
	$(DIRSRC)/xzao225.c\
	$(DIRSRC)/xzao250.c\
	$(DIRSRC)/xzao251.c\
	$(DIRSRC)/xzao306.c\
	$(DIRSRC)/xzao4001.c\
	$(DIRSRC)/xzao401.c\
	$(DIRSRC)/xzao470.c\
	$(DIRSRC)/xzao412.c\
	$(DIRSRC)/xzao413.c\
	$(DIRSRC)/xzao419.c\
	$(DIRSRC)/xzao424.c\
	$(DIRSRC)/xzao440.c\
	$(DIRSRC)/xzao253.c\
	$(DIRSRC)/xzao256.c\
	$(DIRSRC)/xzao441.c\
	$(DIRSRC)/xzao425.c\
	$(DIRSRC)/xzao426.c\
	$(DIRSRC)/xzao427.c\
	$(DIRSRC)/xzao428.c\
	$(DIRSRC)/xzao402.c\
	$(DIRSRC)/xzao409.c\
	$(DIRSRC)/xzao410.c\
	$(DIRSRC)/xzao465.c\
	$(DIRSRC)/xzao466.c\
	$(DIRSRC)/xzao467.c\
	$(DIRSRC)/xzao468.c\
	$(DIRSRC)/xzao469.c

INCA = $(DIRINC)/xzau16.h\
	$(DIRINC)/xzae08.h\
	$(DIRINC)/xzae11.h\
	$(DIRINC)/xzae20.h\
	$(DIRINC)/xzae21.h\
	$(DIRINC)/xzae22.h\
	$(DIRINC)/xzae23.h\
	$(DIRINC)/xzae24.h\
	$(DIRINC)/xzae26.h\
	$(DIRINC)/xzae27.h\
	$(DIRINC)/xzae29.h\
	$(DIRINC)/xzae30.h\
	$(DIRINC)/xzae31.h\
	$(DIRINC)/xzae32.h\
        $(DIRINC)/xzae40.h\
        $(DIRINC)/xzae41.h\
        $(DIRINC)/xzae42.h\
        $(DIRINC)/xzae44.h\
        $(DIRINC)/xzae45.h\
        $(DIRINC)/xzae46.h\
        $(DIRINC)/xzae47.h\
        $(DIRINC)/xzae48.h\
        $(DIRINC)/xzae49.h\
        $(DIRINC)/xzae50.h\
        $(DIRINC)/xzae51.h\
        $(DIRINC)/xzae52.h\
	$(DIRINC)/xzae53.h\
        $(DIRINC)/xzae54.h\
        $(DIRINC)/xzae55.h\
        $(DIRINC)/xzae56.h\
        $(DIRINC)/xzae57.h\
        $(DIRINC)/xzae58.h\
        $(DIRINC)/xzae60.h\
        $(DIRINC)/xzae61.h\
        $(DIRINC)/xzae62.h\
        $(DIRINC)/xzae63.h\
        $(DIRINC)/xzae65.h\
        $(DIRINC)/xzae67.h\
	$(DIRINC)/xzae68.h\
	$(DIRINC)/xzae69.h\
	$(DIRINC)/xzae96.h\
	$(DIRINC)/xzae97.h\
	$(DIRINC)/xzae641.h\
	$(DIRINC)/xzae101.h\
	$(DIRINC)/xzae102.h\
	$(DIRINC)/xzae103.h\
	$(DIRINC)/xzae123.h\
	$(DIRINC)/xzae126.h\
	$(DIRINC)/xzae185.h\
	$(DIRINC)/xzae127.h\
	$(DIRINC)/xzae1270.h\
	$(DIRINC)/xzae129.h\
	$(DIRINC)/xzae131.h\
	$(DIRINC)/xzae132.h\
	$(DIRINC)/xzae133.h\
	$(DIRINC)/xzae134.h\
	$(DIRINC)/xzae135.h\
	$(DIRINC)/xzae136.h\
	$(DIRINC)/xzae137.h\
	$(DIRINC)/xzae138.h\
	$(DIRINC)/xzae139.h\
	$(DIRINC)/xzae140.h\
	$(DIRINC)/xzae146.h\
	$(DIRINC)/xzae150.h\
	$(DIRINC)/xzae153.h\
	$(DIRINC)/xzae155.h\
	$(DIRINC)/xzae160.h\
	$(DIRINC)/xzae162.h\
	$(DIRINC)/xzae163.h\
	$(DIRINC)/xzae164.h\
	$(DIRINC)/xzae165.h\
	$(DIRINC)/xzae166.h\
	$(DIRINC)/xzae170.h\
	$(DIRINC)/xzae171.h\
	$(DIRINC)/xzae172.h\
	$(DIRINC)/xzae173.h\
	$(DIRINC)/xzae175.h\
	$(DIRINC)/xzae178.h\
	$(DIRINC)/xzae179.h\
	$(DIRINC)/xzae182.h\
	$(DIRINC)/xzae200.h\
	$(DIRINC)/xzae208.h\
	$(DIRINC)/xzae212.h\
	$(DIRINC)/xzae220.h\
	$(DIRINC)/xzae201.h\
	$(DIRINC)/xzae207.h\
	$(DIRINC)/xzae550.h\
	$(DIRINC)/xzae552.h\
	$(DIRINC)/xzae553.h\
	$(DIRINC)/xzae980.h\
	$(DIRINC)/xzae981.h\
	$(DIRINC)/xzae554.h\
	$(DIRINC)/xzae555.h\
	$(DIRINC)/xzae556.h\
	$(DIRINC)/xzae560.h\
	$(DIRINC)/xzae562.h\
	$(DIRINC)/xzae563.h\
	$(DIRINC)/xzae564.h\
	$(DIRINC)/xzae565.h\
	$(DIRINC)/xzae566.h\
	$(DIRINC)/xzae567.h\
	$(DIRINC)/xzae568.h\
	$(DIRINC)/xzae860.h\
	$(DIRINC)/xzae861.h\
	$(DIRINC)/xzao980.h\
	$(DIRINC)/xzan01.h\
	$(DIRINC)/xzan02.h\
	$(DIRINC)/xzan34.h\
	$(DIRINC)/xzan36.h\
	$(DIRINC)/xzan37.h\
	$(DIRINC)/xzar03.h\
	$(DIRINC)/xzai07.h\
	$(DIRINC)/xzai500.h\
	$(DIRINC)/xzas02.h\
	$(DIRINC)/xzas03.h\
	$(DIRINC)/xzas06.h\
	$(DIRINC)/xzas91.h\
	$(DIRINC)/xzas10.h\
	$(DIRINC)/xzas11.h\
	$(DIRINC)/xzas12.h\
	$(DIRINC)/xzas13.h\
	$(DIRINC)/xzas50.h\
	$(DIRINC)/xzas51.h\
	$(DIRINC)/xzas52.h\
	$(DIRINC)/xzas53.h\
	$(DIRINC)/xzas54.h\
	$(DIRINC)/xzas540.h\
	$(DIRINC)/xzas55.h\
	$(DIRINC)/xzas62.h\
	$(DIRINC)/xzas70.h\
	$(DIRINC)/xzas80.h\
	$(DIRINC)/xzas81.h\
	$(DIRINC)/xzas82.h\
	$(DIRINC)/xzas83.h\
	$(DIRINC)/xzas87.h\
	$(DIRINC)/xzas88.h\
	$(DIRINC)/xzas100.h\
	$(DIRINC)/xzat01.h\
	$(DIRINC)/xzat05.h\
	$(DIRINC)/xzat050.h\
	$(DIRINC)/xzat060.h\
	$(DIRINC)/xzat06.h\
	$(DIRINC)/xzat35.h\
	$(DIRINC)/xzat36.h\
	$(DIRINC)/xzat51.h\
	$(DIRINC)/xzat511.h\
	$(DIRINC)/xzat07.h\
	$(DIRINC)/xzat08.h\
	$(DIRINC)/xzat280.h\
	$(DIRINC)/xzah_utils.h\
	$(DIRINC)/xzat100.h\
	$(DIRINC)/xzah01.h\
	$(DIRINC)/xzah03.h\
	$(DIRINC)/xzah02.h\
	$(DIRINC)/xzah04.h\
	$(DIRINC)/xzah05.h\
	$(DIRINC)/xzah07.h\
	$(DIRINC)/xzah08.h\
	$(DIRINC)/xzah10.h\
	$(DIRINC)/xzah11.h\
	$(DIRINC)/xzac22.h\
	$(DIRINC)/xzac23.h\
	$(DIRINC)/xzac25.h\
	$(DIRINC)/xzac26.h\
	$(DIRINC)/xzac70.h\
	$(DIRINC)/xzac71.h\
	$(DIRINC)/xzac73.h\
	$(DIRINC)/xzac75.h\
	$(DIRINC)/xzac66.h\
	$(DIRINC)/xzac03.h\
	$(DIRINC)/xzac103.h\
	$(DIRINC)/xzac01.h\
	$(DIRINC)/xzac101.h\
	$(DIRINC)/xzac803.h\
	$(DIRINC)/xzac100.h\
	$(DIRINC)/xzac108.h\
	$(DIRINC)/xzac102.h\
	$(DIRINC)/xzac109.h\
	$(DIRINC)/xzac104.h\
	$(DIRINC)/xzac105.h\
	$(DIRINC)/xzac550.h\
	$(DIRINC)/xzac67.h\
	$(DIRINC)/xzac62.h\
	$(DIRINC)/xzac80.h\
	$(DIRINC)/xzac81.h\
	$(DIRINC)/xzac82.h\
	$(DIRINC)/xzac802.h\
	$(DIRINC)/xzac8021.h\
	$(DIRINC)/xzac803.h\
	$(DIRINC)/xzac63.h\
	$(DIRINC)/xzac631.h\
	$(DIRINC)/xzac632.h\
	$(DIRINC)/xzac633.h\
	$(DIRINC)/xzac634.h\
	$(DIRINC)/xzac635.h\
	$(DIRINC)/xzac636.h\
	$(DIRINC)/xzac84.h\
	$(DIRINC)/xzac840.h\
	$(DIRINC)/xzac06.h\
	$(DIRINC)/xzac106.h\
	$(DIRINC)/xzac61.h\
	$(DIRINC)/xzac610.h\
	$(DIRINC)/xzac93.h\
	$(DIRINC)/xzac182.h\
	$(DIRINC)/xzac92.h\
	$(DIRINC)/xzac99.h\
	$(DIRINC)/xzac192.h\
	$(DIRINC)/xzac193.h\
	$(DIRINC)/xzac98.h\
	$(DIRINC)/xzac99.h\
	$(DIRINC)/xzac86.h\
	$(DIRINC)/xzac303.h\
	$(DIRINC)/xzac1000.h\
	$(DIRINC)/xzap21.h\
	$(DIRINC)/xzao15.h\
	$(DIRINC)/xzao16.h\
	$(DIRINC)/xzao17.h\
	$(DIRINC)/xzao19.h\
	$(DIRINC)/xzao20.h\
	$(DIRINC)/xzao21.h\
	$(DIRINC)/xzao22.h\
	$(DIRINC)/xzao500.h\
	$(DIRINC)/xzao576.h\
	$(DIRINC)/xzao5761.h\
	$(DIRINC)/xzao577.h\
	$(DIRINC)/xzao5771.h\
	$(DIRINC)/xzao630.h\
	$(DIRINC)/xzao631.h\
	$(DIRINC)/xzao632.h\
	$(DIRINC)/xzao633.h\
	$(DIRINC)/xzao634.h\
	$(DIRINC)/xzao635.h\
	$(DIRINC)/xzao636.h\
	$(DIRINC)/xzao637.h\
	$(DIRINC)/xzah21.h\
	$(DIRINC)/xzah26.h\
	$(DIRINC)/xzah27.h\
	$(DIRINC)/xzaz01.h\
	$(DIRINC)/xzaf02.h\
	$(DIRINC)/xzaf03.h\
	$(DIRINC)/xzaf05.h\
	$(DIRINC)/xzaf06.h\
	$(DIRINC)/xzaf08.h\
	$(DIRINC)/xzaf10.h\
	$(DIRINC)/xzaf14.h\
	$(DIRINC)/xzaf15.h\
	$(DIRINC)/xzaf16.h\
	$(DIRINC)/xzaf11.h\
	$(DIRINC)/xzaq.h

INCAP = $(DIRINC)/xzap01.h\
	$(DIRINC)/xzap02.h\
	$(DIRINC)/xzap03.h\
	$(DIRINC)/xzap04.h\
	$(DIRINC)/xzap05.h\
	$(DIRINC)/xzap06.h\
	$(DIRINC)/xzap07.h\
	$(DIRINC)/xzap08.h\
	$(DIRINC)/xzap09.h\
	$(DIRINC)/xzap10.h\
	$(DIRINC)/xzap22.h\
	$(DIRINC)/xzap23.h\
	$(DIRINC)/xzap26.h\
	$(DIRINC)/xzap27.h\
	$(DIRINC)/xzap28.h\
	$(DIRINC)/xzap82.h\
	$(DIRINC)/xzap821.h\
	$(DIRINC)/xzap29.h\
	$(DIRINC)/xzap33.h\
	$(DIRINC)/xzap25.h

SRCA =  $(DIRSRC)/xzau16.c\
	$(DIRSRC)/xzao15.c\
	$(DIRSRC)/xzao16.c\
	$(DIRSRC)/xzao17.c\
	$(DIRSRC)/xzao19.c\
	$(DIRSRC)/xzao500.c\
	$(DIRSRC)/xzao576.c\
	$(DIRSRC)/xzao5761.c\
	$(DIRSRC)/xzao577.c\
	$(DIRSRC)/xzao5771.c\
	$(DIRSRC)/xzae08.c\
	$(DIRSRC)/xzae11.c\
	$(DIRSRC)/xzae20.c\
	$(DIRSRC)/xzae21.c\
	$(DIRSRC)/xzae22.c\
	$(DIRSRC)/xzae23.c\
	$(DIRSRC)/xzae24.c\
	$(DIRSRC)/xzae26.c\
	$(DIRSRC)/xzae27.c\
	$(DIRSRC)/xzae29.c\
	$(DIRSRC)/xzae30.c\
	$(DIRSRC)/xzae31.c\
	$(DIRSRC)/xzae32.c\
        $(DIRSRC)/xzae40.c\
        $(DIRSRC)/xzae41.c\
        $(DIRSRC)/xzae42.c\
        $(DIRSRC)/xzae44.c\
        $(DIRSRC)/xzae45.c\
        $(DIRSRC)/xzae46.c\
        $(DIRSRC)/xzae47.c\
        $(DIRSRC)/xzae48.c\
        $(DIRSRC)/xzae49.c\
        $(DIRSRC)/xzae50.c\
        $(DIRSRC)/xzae51.c\
        $(DIRSRC)/xzae52.c\
	$(DIRSRC)/xzae53.c\
        $(DIRSRC)/xzae54.c\
        $(DIRSRC)/xzae55.c\
        $(DIRSRC)/xzae56.c\
        $(DIRSRC)/xzae57.c\
        $(DIRSRC)/xzae58.c\
        $(DIRSRC)/xzae60.c\
        $(DIRSRC)/xzae61.c\
        $(DIRSRC)/xzae62.c\
        $(DIRSRC)/xzae63.c\
        $(DIRSRC)/xzae65.c\
        $(DIRSRC)/xzae67.c\
	$(DIRSRC)/xzae68.c\
	$(DIRSRC)/xzae69.c\
	$(DIRSRC)/xzae96.c\
	$(DIRSRC)/xzae97.c\
	$(DIRSRC)/xzae641.c\
	$(DIRSRC)/xzae101.c\
	$(DIRSRC)/xzae102.c\
	$(DIRSRC)/xzae103.c\
	$(DIRSRC)/xzae123.c\
	$(DIRSRC)/xzae126.c\
	$(DIRSRC)/xzae185.c\
	$(DIRSRC)/xzae127.c\
	$(DIRSRC)/xzae1270.c\
	$(DIRSRC)/xzae129.c\
	$(DIRSRC)/xzae153.c\
	$(DIRSRC)/xzae155.c\
	$(DIRSRC)/xzae150.c\
	$(DIRSRC)/xzae131.c\
	$(DIRSRC)/xzae132.c\
	$(DIRSRC)/xzae133.c\
	$(DIRSRC)/xzae134.c\
	$(DIRSRC)/xzae135.c\
	$(DIRSRC)/xzae136.c\
	$(DIRSRC)/xzae137.c\
	$(DIRSRC)/xzae138.c\
	$(DIRSRC)/xzae139.c\
	$(DIRSRC)/xzae140.c\
	$(DIRSRC)/xzae146.c\
	$(DIRSRC)/xzae160.c\
	$(DIRSRC)/xzae162.c\
	$(DIRSRC)/xzae163.c\
	$(DIRSRC)/xzae164.c\
	$(DIRSRC)/xzae165.c\
	$(DIRSRC)/xzae166.c\
	$(DIRSRC)/xzae170.c\
	$(DIRSRC)/xzae171.c\
	$(DIRSRC)/xzae172.c\
	$(DIRSRC)/xzae173.c\
	$(DIRSRC)/xzae175.c\
	$(DIRSRC)/xzae178.c\
	$(DIRSRC)/xzae179.c\
	$(DIRSRC)/xzae182.c\
	$(DIRSRC)/xzae200.c\
	$(DIRSRC)/xzae208.c\
	$(DIRSRC)/xzae212.c\
	$(DIRSRC)/xzae220.c\
	$(DIRSRC)/xzae201.c\
	$(DIRSRC)/xzae207.c\
	$(DIRSRC)/xzae550.c\
	$(DIRSRC)/xzae552.c\
	$(DIRSRC)/xzae553.c\
	$(DIRSRC)/xzae980.c\
	$(DIRSRC)/xzae981.c\
	$(DIRSRC)/xzae554.c\
	$(DIRSRC)/xzae555.c\
	$(DIRSRC)/xzae556.c\
	$(DIRSRC)/xzae562.c\
	$(DIRSRC)/xzae563.c\
	$(DIRSRC)/xzae560.c\
	$(DIRSRC)/xzae564.c\
	$(DIRSRC)/xzae565.c\
	$(DIRSRC)/xzae566.c\
	$(DIRSRC)/xzae567.c\
	$(DIRSRC)/xzae568.c\
	$(DIRSRC)/xzae860.c\
	$(DIRSRC)/xzae861.c\
	$(DIRSRC)/xzan01.c\
	$(DIRSRC)/xzan02.c\
	$(DIRSRC)/xzan34.c\
	$(DIRSRC)/xzan36.c\
	$(DIRSRC)/xzan37.c\
	$(DIRSRC)/xzar03.c\
	$(DIRSRC)/xzai07.c\
	$(DIRSRC)/xzai500.c\
	$(DIRSRC)/xzas02.c\
	$(DIRSRC)/xzas03.c\
	$(DIRSRC)/xzas06.c\
	$(DIRSRC)/xzas91.c\
	$(DIRSRC)/xzas10.c\
	$(DIRSRC)/xzas11.c\
	$(DIRSRC)/xzas12.c\
	$(DIRSRC)/xzas13.c\
	$(DIRSRC)/xzas54.c\
	$(DIRSRC)/xzas540.c\
	$(DIRSRC)/xzas55.c\
	$(DIRSRC)/xzas62.c\
	$(DIRSRC)/xzas70.c\
	$(DIRSRC)/xzas80.c\
	$(DIRSRC)/xzas81.c\
	$(DIRSRC)/xzas82.c\
	$(DIRSRC)/xzas83.c\
	$(DIRSRC)/xzas87.c\
	$(DIRSRC)/xzas88.c\
	$(DIRSRC)/xzas100.c\
	$(DIRSRC)/xzat01.c\
	$(DIRSRC)/xzat05.c\
	$(DIRSRC)/xzat050.c\
	$(DIRSRC)/xzat060.c\
	$(DIRSRC)/xzat06.c\
	$(DIRSRC)/xzat51.c\
	$(DIRSRC)/xzat511.c\
	$(DIRSRC)/xzat35.c\
	$(DIRSRC)/xzat36.c\
	$(DIRSRC)/xzat07.c\
	$(DIRSRC)/xzat08.c\
	$(DIRSRC)/xzat280.c\
	$(DIRSRC)/xzat100.c\
	$(DIRSRC)/xzah01.c\
	$(DIRSRC)/xzah03.c\
	$(DIRSRC)/xzah02.c\
	$(DIRSRC)/xzah04.c\
	$(DIRSRC)/xzah05.c\
	$(DIRSRC)/xzah07.c\
	$(DIRSRC)/xzah08.c\
	$(DIRSRC)/xzah10.c\
	$(DIRSRC)/xzah11.c\
	$(DIRSRC)/xzac22.c\
	$(DIRSRC)/xzac23.c\
	$(DIRSRC)/xzac25.c\
	$(DIRSRC)/xzac26.c\
	$(DIRSRC)/xzac70.c\
	$(DIRSRC)/xzac71.c\
	$(DIRSRC)/xzac73.c\
	$(DIRSRC)/xzac75.c\
	$(DIRSRC)/xzac66.c\
	$(DIRSRC)/xzac03.c\
	$(DIRSRC)/xzac103.c\
	$(DIRSRC)/xzac67.c\
	$(DIRSRC)/xzac62.c\
	$(DIRSRC)/xzac80.c\
	$(DIRSRC)/xzac81.c\
	$(DIRSRC)/xzac82.c\
	$(DIRSRC)/xzac802.c\
	$(DIRSRC)/xzac8021.c\
	$(DIRSRC)/xzac63.c\
	$(DIRSRC)/xzac631.c\
	$(DIRSRC)/xzac632.c\
	$(DIRSRC)/xzac633.c\
	$(DIRSRC)/xzac634.c\
	$(DIRSRC)/xzac635.c\
	$(DIRSRC)/xzac636.c\
	$(DIRSRC)/xzac84.c\
	$(DIRSRC)/xzac840.c\
	$(DIRSRC)/xzac06.c\
	$(DIRSRC)/xzac106.c\
	$(DIRSRC)/xzac01.c\
	$(DIRSRC)/xzac101.c\
	$(DIRSRC)/xzac803.c\
	$(DIRSRC)/xzac100.c\
	$(DIRSRC)/xzac108.c\
	$(DIRSRC)/xzac102.c\
	$(DIRSRC)/xzac109.c\
	$(DIRSRC)/xzac104.c\
	$(DIRSRC)/xzac105.c\
	$(DIRSRC)/xzac550.c\
	$(DIRSRC)/xzac61.c\
	$(DIRSRC)/xzac610.c\
	$(DIRSRC)/xzac182.c\
	$(DIRSRC)/xzac92.c\
	$(DIRSRC)/xzac93.c\
	$(DIRSRC)/xzac192.c\
	$(DIRSRC)/xzac193.c\
	$(DIRSRC)/xzac98.c\
	$(DIRSRC)/xzac99.c\
	$(DIRSRC)/xzac86.c\
	$(DIRSRC)/xzac303.c\
	$(DIRSRC)/xzac1000.c\
	$(DIRSRC)/xzap21.c\
	$(DIRSRC)/xzao20.c\
	$(DIRSRC)/xzao21.c\
	$(DIRSRC)/xzao22.c\
	$(DIRSRC)/xzao630.c\
	$(DIRSRC)/xzao631.c\
	$(DIRSRC)/xzao632.c\
	$(DIRSRC)/xzao633.c\
	$(DIRSRC)/xzao634.c\
	$(DIRSRC)/xzao635.c\
	$(DIRSRC)/xzao636.c\
	$(DIRSRC)/xzao637.c\
	$(DIRSRC)/xzah21.c\
	$(DIRSRC)/xzah26.c\
	$(DIRSRC)/xzah27.c\
	$(DIRSRC)/xzaz01.c\
	$(DIRSRC)/xzaf02.c\
	$(DIRSRC)/xzaf03.c\
	$(DIRSRC)/xzaf05.c\
	$(DIRSRC)/xzaf06.c\
	$(DIRSRC)/xzaf08.c\
	$(DIRSRC)/xzaf10.c\
	$(DIRSRC)/xzaf14.c\
	$(DIRSRC)/xzaf15.c\
	$(DIRSRC)/xzaf16.c\
	$(DIRSRC)/xzaf11.c\
	$(DIRSRC)/xzaq.c

SRCAP =	$(DIRSRC)/xzap01.c\
	$(DIRSRC)/xzap02.c\
	$(DIRSRC)/xzap03.c\
	$(DIRSRC)/xzap04.c\
	$(DIRSRC)/xzap05.c\
	$(DIRSRC)/xzap06.c\
	$(DIRSRC)/xzap07.c\
	$(DIRSRC)/xzap08.c\
	$(DIRSRC)/xzap09.c\
	$(DIRSRC)/xzap10.c\
	$(DIRSRC)/xzap22.c\
	$(DIRSRC)/xzap23.c\
	$(DIRSRC)/xzap26.c\
	$(DIRSRC)/xzap27.c\
	$(DIRSRC)/xzap28.c\
	$(DIRSRC)/xzap82.c\
	$(DIRSRC)/xzap821.c\
	$(DIRSRC)/xzap29.c\
	$(DIRSRC)/xzap33.c\
	$(DIRSRC)/xzap25.c

#  Sources Communes 

INCD =	$(DIRINC)/xdc.h\
	$(DIRINC)/xdf.h\
	$(DIRINC)/xdg.h\
	$(DIRINC)/xdm.h\
	$(DIRINC)/xdl.h\
	$(DIRINC)/xdy.h\
	$(DIRINC)/xdea_eqn_alt.h

#  Sources SUPERV 
INCS =	$(DIRINC)/xzsi.h\
	$(DIRINC)/xzsa.h\
	  $(DIRINC)/xzsc.h\
	  $(DIRINC)/xzse.h\
	  $(DIRINC)/xzsg.h\
	  $(DIRINC)/xzsm.h\
	  $(DIRINC)/xzsp.h\
	  $(DIRINC)/xzss.h\
	  $(DIRINC)/xzst.h\
	  $(DIRINC)/xzsx.h\
	  $(DIRINC)/xzlg.h

SRCS =	$(DIRSRC)/xzsi.c\
	 $(DIRSRC)/xzsa.c\
	 $(DIRSRC)/xzsc.c\
	 $(DIRSRC)/xzse.c\
	 $(DIRSRC)/xzsg.c\
	 $(DIRSRC)/xzsm.c\
	 $(DIRSRC)/xzsp.c\
	 $(DIRSRC)/xzss.c\
	 $(DIRSRC)/xzst.c\
	 $(DIRSRC)/xzsx.c\
	 $(DIRSRC)/xzlg.c

#  Sources EQUEXT 
INCE =	$(DIRINC)/xzea.h\
	 $(DIRINC)/xzea01.h\
	 $(DIRINC)/xzea02.h\
	 $(DIRINC)/xzea03.h\
	 $(DIRINC)/xzea04.h\
	 $(DIRINC)/xzea05.h\
	 $(DIRINC)/xzea06.h\
	 $(DIRINC)/xzea07.h\
	 $(DIRINC)/xzea08.h\
	 $(DIRINC)/xzea080.h\
	 $(DIRINC)/xzea180.h\
	 $(DIRINC)/xzea081.h\
	 $(DIRINC)/xzea09.h\
	 $(DIRINC)/xzea10.h\
	 $(DIRINC)/xzea40.h\
	 $(DIRINC)/xzea41.h\
	 $(DIRINC)/xzea11.h\
	 $(DIRINC)/xzea12.h\
	 $(DIRINC)/xzea13.h\
	 $(DIRINC)/xzea14.h\
	 $(DIRINC)/xzea15.h\
	 $(DIRINC)/xzea16.h\
	 $(DIRINC)/xzea18.h\
	 $(DIRINC)/xzea22.h\
	 $(DIRINC)/xzea23.h\
	 $(DIRINC)/xzea24.h\
	 $(DIRINC)/xzea25.h\
	 $(DIRINC)/xzea26.h\
	 $(DIRINC)/xzea27.h\
	 $(DIRINC)/xzea28.h\
	 $(DIRINC)/xzea29.h\
	 $(DIRINC)/xzea30.h\
	 $(DIRINC)/xzea31.h\
	 $(DIRINC)/xzea32.h\
	 $(DIRINC)/xzea33.h\
	 $(DIRINC)/xzea34.h\
	 $(DIRINC)/xzea35.h\
	 $(DIRINC)/xzea36.h\
	 $(DIRINC)/xzea37.h\
	 $(DIRINC)/xzea38.h\
	 $(DIRINC)/xzea42.h\
	 $(DIRINC)/xzea43.h\
	 $(DIRINC)/xzea44.h\
	 $(DIRINC)/xzea45.h\
	 $(DIRINC)/xzea46.h\
	 $(DIRINC)/xzea47.h\
	 $(DIRINC)/xzea48.h\
	 $(DIRINC)/xzea49.h\
	 $(DIRINC)/xzea50.h\
	 $(DIRINC)/xzea51.h\
	 $(DIRINC)/xzea52.h\
	 $(DIRINC)/xzea53.h\
	 $(DIRINC)/xzea54.h\
	 $(DIRINC)/xzea55.h\
	 $(DIRINC)/xzea56.h\
	 $(DIRINC)/xzec.h\
	 $(DIRINC)/xzed.h\
	 $(DIRINC)/xzeg01.h\
	 $(DIRINC)/xzel.h\
	 $(DIRINC)/xzen.h\
	 $(DIRINC)/xzeb.h\
	 $(DIRINC)/xzeo.h\
	 $(DIRINC)/xzep.h\
	 $(DIRINC)/xzes.h\
	 $(DIRINC)/xzet.h\
	 $(DIRINC)/xzev.h\
	 $(DIRINC)/xzex.h\
	 $(DIRINC)/xzez.h\
	 $(DIRINC)/xzeu.h\
	 $(DIRINC)/xzer.h

SRCE =	$(DIRSRC)/xzea01.c\
	 $(DIRSRC)/xzea02.c\
	 $(DIRSRC)/xzea03.c\
	 $(DIRSRC)/xzea04.c\
	 $(DIRSRC)/xzea05.c\
	 $(DIRSRC)/xzea06.c\
	 $(DIRSRC)/xzea07.c\
	 $(DIRSRC)/xzea08.c\
	 $(DIRSRC)/xzea080.c\
	 $(DIRSRC)/xzea180.c\
	 $(DIRSRC)/xzea081.c\
	 $(DIRSRC)/xzea09.c\
	 $(DIRSRC)/xzea10.c\
	 $(DIRSRC)/xzea40.c\
	 $(DIRSRC)/xzea41.c\
	 $(DIRSRC)/xzea11.c\
	 $(DIRSRC)/xzea12.c\
	 $(DIRSRC)/xzea13.c\
	 $(DIRSRC)/xzea14.c\
	 $(DIRSRC)/xzea15.c\
	 $(DIRSRC)/xzea16.c\
	 $(DIRSRC)/xzea18.c\
	 $(DIRSRC)/xzea22.c\
	 $(DIRSRC)/xzea23.c\
	 $(DIRSRC)/xzea24.c\
	 $(DIRSRC)/xzea25.c\
	 $(DIRSRC)/xzea26.c\
	 $(DIRSRC)/xzea27.c\
	 $(DIRSRC)/xzea28.c\
	 $(DIRSRC)/xzea29.c\
	 $(DIRSRC)/xzea30.c\
	 $(DIRSRC)/xzea31.c\
	 $(DIRSRC)/xzea32.c\
	 $(DIRSRC)/xzea33.c\
	 $(DIRSRC)/xzea34.c\
	 $(DIRSRC)/xzea35.c\
	 $(DIRSRC)/xzea36.c\
	 $(DIRSRC)/xzea37.c\
	 $(DIRSRC)/xzea38.c\
	 $(DIRSRC)/xzea42.c\
	 $(DIRSRC)/xzea43.c\
	 $(DIRSRC)/xzea44.c\
	 $(DIRSRC)/xzea45.c\
	 $(DIRSRC)/xzea46.c\
	 $(DIRSRC)/xzea47.c\
	 $(DIRSRC)/xzea48.c\
	 $(DIRSRC)/xzea49.c\
	 $(DIRSRC)/xzea50.c\
	 $(DIRSRC)/xzea51.c\
	 $(DIRSRC)/xzea52.c\
	 $(DIRSRC)/xzea53.c\
	 $(DIRSRC)/xzea54.c\
	 $(DIRSRC)/xzea55.c\
	 $(DIRSRC)/xzea56.c\
	 $(DIRSRC)/xzea70.c\
	 $(DIRSRC)/xzea71.c\
	 $(DIRSRC)/xzec.c\
	 $(DIRSRC)/xzed.c\
	 $(DIRSRC)/xzeg01.c\
	 $(DIRSRC)/xzel.c\
	 $(DIRSRC)/xzen.c\
	 $(DIRSRC)/xzeb.c\
	 $(DIRSRC)/xzeo.c\
	 $(DIRSRC)/xzep.c\
	 $(DIRSRC)/xzes.c\
	 $(DIRSRC)/xzet.c\
	 $(DIRSRC)/xzev.c\
	 $(DIRSRC)/xzex.c\
	 $(DIRSRC)/xzez.c\
	 $(DIRSRC)/xzeu.c\
	 $(DIRSRC)/xzer.c

INCEH =	$(DIRINC)/xzeh.h

SRCEH =	$(DIRSRC)/xzeh.c

INCAD = $(DIRINC)/xzad.h\
	$(DIRINC)/xzad10.h\
	$(DIRINC)/xzad07.h\
	$(DIRINC)/xzad17.h\
	$(DIRINC)/xzad08.h\
	$(DIRINC)/xzad02.h\
	$(DIRINC)/xzad03.h\
	$(DIRINC)/xzad04.h\
	$(DIRINC)/xzad05.h

SRCAD = $(DIRSRC)/xzad.c\
	$(DIRSRC)/xzad10.c\
	$(DIRSRC)/xzad07.c\
	$(DIRSRC)/xzad17.c\
	$(DIRSRC)/xzad08.c\
	$(DIRSRC)/xzad02.c\
	$(DIRSRC)/xzad03.c\
	$(DIRSRC)/xzad04.c\
	$(DIRSRC)/xzad05.c

#---------------------------------------------------------------------
#  Sources GESIHM

INCI = 	$(DIRINC)/xzis.h\
	$(DIRINC)/xzir.h\
	$(DIRINC)/xzia.h\
	$(DIRINC)/xzit.h


SRCI =   $(DIRSRC)/xzis.c\
	$(DIRSRC)/xzir.c\
	$(DIRSRC)/xzia.c\
	$(DIRSRC)/xzit.c


#  Fin declaration Sources GESIHM
#---------------------------------------------------------------------


#---------------------------------------------------------------------
#  Sources SAIDEC


INCP =	$(DIRINC)/xzdd.h\
	$(DIRINC)/xzdc.h


SRCP =	$(DIRSRC)/xzdd.c\
	$(DIRSRC)/xzdc.c


#  Fin declaration Sources SAIDEC
#---------------------------------------------------------------------


#  Sources CONFIG 

INCC =	 $(DIRINC)/xzca.h\
	$(DIRINC)/xzcg.h

SRCC =	 $(DIRSRC)/xzca.c\
	$(DIRSRC)/xzcg.c

#---------------------------------------------------------------------
#  Sources MAINTE


INCM = $(DIRINC)/xzma.h


SRCM = $(DIRSRC)/xzma.c

#  Fin declaration Sources MAINTE
#---------------------------------------------------------------------

# Sources communs

SRCD =	$(DIRSRC)/xdg.c \
	$(DIRSRC)/xdm.c \
	$(DIRSRC)/xdl.c \
	$(DIRSRC)/xdy.c \
	$(DIRSRC)/xdf.c \
	$(DIRSRC)/xdea_eqn_alt.c

SRC = $(SRCS) $(SRCC) $(SRCD) $(SRCOA) $(SRCOC) $(SRCOL) $(SRCA) $(SRCI) $(SRCP) $(SRCE) $(SRCEH) $(SRCM) $(SRCAD)

#
# librairie d'un executable
#

LIBS =  $(DIRLIB)/xzs.a

LIBC =  $(DIRLIB)/xzc.a

LIBD =  $(DIRLIB)/xd.a

LIBOC =  $(DIRLIB)/xzaoc.a

LIBOA =  $(DIRLIB)/xzaoa.a

LIBOL =  $(DIRLIB)/xzaol.a

LIBA =  $(DIRLIB)/xza.a

LIBAP =  $(DIRLIB)/xzap.a

LIBI =	$(DIRLIB)/xzi.a

LIBE =	$(DIRLIB)/xze.a

LIBEH =	$(DIRLIB)/xzeh.a

LIBP =	$(DIRLIB)/xzd.a

LIBM = $(DIRLIB)/xzm.a

LIBAD = $(DIRLIB)/xzad.a


LIB = $(LIBD) $(LIBS) $(LIBC) $(LIBOA) $(LIBOC) $(LIBOL) $(LIBA) $(LIBI) $(LIBP) $(LIBE) $(LIBEH) $(LIBM) $(LIBAD) $(LIBAP)

#
# DEFINITION DES REGLES DE GENERATION 
#
# Premiere regle il faut tout faire : les communs et les executables

tout : verifier_plateforme $(SRC) $(LIB)  
	echo
	@echo LE SOUS-SYSTEME $(SSYST) EST A JOUR. 
	cp -f *.ksh $(DIREXE_SHELL)
	@chmod a+wx $(DIREXE_SHELL)/*.ksh
	echo

$(LIB) : verifier_plateforme

# -- Regles utiles simplement pour lancer independamment les
#  compilations. Exemple : make xzs.a

xzs.a : $(LIBS)
	echo
xd.a : $(LIBD)
	echo
xzc.a : $(LIBC)
	echo
xze.a : $(LIBE)
	echo
xzeh.a : $(LIBEH)
	echo
xzad.a : $(LIBAD)
	echo
xzaoc.a : $(LIBOC)
	echo
xzaoa.a : $(LIBOA)
	echo
xzaol.a : $(LIBOL)
	echo
xza.a : $(LIBA)
	echo
xzap.a : $(LIBAP)
	echo

xzi.a : $(LIBI)
	echo
xzd.a : $(LIBP)
	echo
xzm.a : $(LIBM)
	echo


# 
# Regle liant les sources aux fichiers include communs et au makefile  
# 
$(SRCS) : $(INCD) $(INCS)
	touch $@

$(SRCI) : $(INCI)
	touch $@

$(SRCP) : $(INCP)
	touch $@

$(SRCOC) : $(INCOC)
	touch $@

$(SRCOA) : $(INCOA)
	touch $@

$(SRCOL) : $(INCOL)
	touch $@

$(SRCE) : $(INCE)
	touch $@

$(SRCEH) : $(INCEH)
	touch $@

$(SRCAD) : $(INCAD)
	touch $@

$(SRCC) : $(INCC) $(INCD)
	touch $@

$(SRCD) : $(INCD)
	touch $@

$(SRCM) : $(INCM)
	touch $@

$(SRCA) : $(INCA)  $(INCD)
	touch $@
	
$(SRCAP) : $(INCAP) 
	touch $@

# 
# Regle de generation d'une librairie  a partir 
# de ses fichiers objet.
# 

$(LIBE): $(LIBE)(xzea01.o)\
	 $(LIBE)(xzea02.o)\
	 $(LIBE)(xzea03.o)\
	 $(LIBE)(xzea04.o)\
	 $(LIBE)(xzea05.o)\
	 $(LIBE)(xzea06.o)\
	 $(LIBE)(xzea07.o)\
	 $(LIBE)(xzea08.o)\
	 $(LIBE)(xzea080.o)\
	 $(LIBE)(xzea180.o)\
	 $(LIBE)(xzea081.o)\
	 $(LIBE)(xzea09.o)\
	 $(LIBE)(xzea10.o)\
	 $(LIBE)(xzea40.o)\
	 $(LIBE)(xzea41.o)\
	 $(LIBE)(xzea11.o)\
	 $(LIBE)(xzea12.o)\
	 $(LIBE)(xzea13.o)\
	 $(LIBE)(xzea14.o)\
	 $(LIBE)(xzea15.o)\
	 $(LIBE)(xzea16.o)\
	 $(LIBE)(xzea18.o)\
	 $(LIBE)(xzea22.o)\
	 $(LIBE)(xzea23.o)\
	 $(LIBE)(xzea24.o)\
	 $(LIBE)(xzea25.o)\
	 $(LIBE)(xzea26.o)\
	 $(LIBE)(xzea27.o)\
	 $(LIBE)(xzea28.o)\
	 $(LIBE)(xzea29.o)\
	 $(LIBE)(xzea30.o)\
	 $(LIBE)(xzea31.o)\
	 $(LIBE)(xzea32.o)\
	 $(LIBE)(xzea33.o)\
	 $(LIBE)(xzea34.o)\
	 $(LIBE)(xzea35.o)\
	 $(LIBE)(xzea36.o)\
	 $(LIBE)(xzea37.o)\
	 $(LIBE)(xzea38.o)\
	 $(LIBE)(xzea42.o)\
	 $(LIBE)(xzea43.o)\
	 $(LIBE)(xzea44.o)\
	 $(LIBE)(xzea45.o)\
	 $(LIBE)(xzea46.o)\
	 $(LIBE)(xzea47.o)\
	 $(LIBE)(xzea48.o)\
	 $(LIBE)(xzea49.o)\
	 $(LIBE)(xzea50.o)\
	 $(LIBE)(xzea51.o)\
	 $(LIBE)(xzea52.o)\
	 $(LIBE)(xzea53.o)\
	 $(LIBE)(xzea54.o)\
	 $(LIBE)(xzea55.o)\
	 $(LIBE)(xzea56.o)\
	 $(LIBE)(xzea70.o)\
	 $(LIBE)(xzea71.o)\
	 $(LIBE)(xzec.o)\
	 $(LIBE)(xzed.o)\
	 $(LIBE)(xzeg01.o)\
	 $(LIBE)(xzel.o)\
	 $(LIBE)(xzen.o)\
	 $(LIBE)(xzeb.o)\
	 $(LIBE)(xzeo.o)\
	 $(LIBE)(xzep.o)\
	 $(LIBE)(xzes.o)\
	 $(LIBE)(xzet.o)\
	 $(LIBE)(xzev.o)\
	 $(LIBE)(xzex.o)\
	 $(LIBE)(xzez.o)\
	 $(LIBE)(xzeu.o)\
	 $(LIBE)(xzer.o)	

$(LIBEH): $(LIBEH)(xzeh.o)	

$(LIBAD): $(LIBAD)(xzad.o)\
	$(LIBAD)(xzad10.o)\
	$(LIBAD)(xzad07.o)\
	$(LIBAD)(xzad17.o)\
	$(LIBAD)(xzad08.o)\
	$(LIBAD)(xzad02.o)\
	$(LIBAD)(xzad03.o)\
	$(LIBAD)(xzad04.o)\
	$(LIBAD)(xzad05.o)

$(LIBS): $(LIBS)(xzsi.o)\
	$(LIBS)(xzsa.o)\
	$(LIBS)(xzsc.o)\
	$(LIBS)(xzse.o)\
	$(LIBS)(xzsg.o)\
	$(LIBS)(xzsm.o)\
	$(LIBS)(xzsp.o)\
	$(LIBS)(xzss.o)\
	$(LIBS)(xzst.o)\
	$(LIBS)(xzsx.o)	\
	$(LIBS)(xzlg.o)

$(LIBC): $(LIBC)(xzca.o)\
	$(LIBC)(xzcg.o)

$(LIBD): $(LIBD)(xdg.o)\
	$(LIBD)(xdm.o)\
	$(LIBD)(xdl.o)\
	$(LIBD)(xdy.o)\
	$(LIBD)(xdf.o)\
	$(LIBD)(xdea_eqn_alt.o)

$(LIBOC): $(LIBOC)(xzao000.o)\
	$(LIBOC)(xzao50.o)\
	$(LIBOC)(xzao52.o)\
	$(LIBOC)(xzao54.o)\
	$(LIBOC)(xzao56.o)\
	$(LIBOC)(xzao58.o)\
	$(LIBOC)(xzao60.o)\
	$(LIBOC)(xzao62.o)\
	$(LIBOC)(xzao83.o)\
	$(LIBOC)(xzao85.o)\
	$(LIBOC)(xzao87.o)\
	$(LIBOC)(xzao89.o)\
	$(LIBOC)(xzao91.o)\
	$(LIBOC)(xzao93.o)\
	$(LIBOC)(xzao120.o)\
	$(LIBOC)(xzao145.o)\
	$(LIBOC)(xzao150.o)\
	$(LIBOC)(xzao95.o)\
	$(LIBOC)(xzao96.o)\
	$(LIBOC)(xzao97.o)\
	$(LIBOC)(xzao101.o)\
	$(LIBOC)(xzao102.o)\
	$(LIBOC)(xzao154.o)\
	$(LIBOC)(xzao98.o)\
	$(LIBOC)(xzao103.o)\
	$(LIBOC)(xzao255.o)\
	$(LIBOC)(xzao104.o)\
	$(LIBOC)(xzao105.o)\
	$(LIBOC)(xzao107.o)\
	$(LIBOC)(xzao112.o)\
	$(LIBOC)(xzao113.o)\
	$(LIBOC)(xzao156.o)\
	$(LIBOC)(xzao165.o)\
	$(LIBOC)(xzao0165.o)\
	$(LIBOC)(xzao167.o)\
	$(LIBOC)(xzao173.o)\
	$(LIBOC)(xzao99.o)\
	$(LIBOC)(xzao106.o)\
	$(LIBOC)(xzao108.o)\
	$(LIBOC)(xzao110.o)\
	$(LIBOC)(xzao111.o)\
	$(LIBOC)(xzao114.o)\
	$(LIBOC)(xzao158.o)\
	$(LIBOC)(xzao175.o)\
	$(LIBOC)(xzao177.o)\
	$(LIBOC)(xzao977.o)\
	$(LIBOC)(xzao979.o)\
	$(LIBOC)(xzao100.o)\
	$(LIBOC)(xzao109.o)\
	$(LIBOC)(xzao160.o)\
	$(LIBOC)(xzao431.o)\
	$(LIBOC)(xzao432.o)\
	$(LIBOC)(xzao169.o)\
	$(LIBOC)(xzao993.o)\
	$(LIBOC)(xzao995.o)\
	$(LIBOC)(xzao997.o)\
	$(LIBOC)(xzao999.o)\
	$(LIBOC)(xzao1011.o)\
	$(LIBOC)(xzao1001.o)\
	$(LIBOC)(xzao1003.o)\
	$(LIBOC)(xzao1005.o)\
	$(LIBOC)(xzao1007.o)\
	$(LIBOC)(xzao981.o)\
	$(LIBOC)(xzao983.o)\
	$(LIBOC)(xzao985.o)\
	$(LIBOC)(xzao987.o)\
	$(LIBOC)(xzao989.o)\
	$(LIBOC)(xzao1013.o)\
	$(LIBOC)(xzao171.o)\
	$(LIBOC)(xzao179.o)\
	$(LIBOC)(xzao181.o)\
	$(LIBOC)(xzao183.o)\
	$(LIBOC)(xzao991.o)\
	$(LIBOC)(xzao191.o)\
	$(LIBOC)(xzao192.o)\
	$(LIBOC)(xzao430.o)\
	$(LIBOC)(xzao480.o)\
	$(LIBOC)(xzao118.o)\
	$(LIBOC)(xzao640.o)\
	$(LIBOC)(xzao641.o)\
	$(LIBOC)(xzao642.o)\
	$(LIBOC)(xzao643.o)\
	$(LIBOC)(xzao644.o)\
	$(LIBOC)(xzao645.o)\
	$(LIBOC)(xzao646.o)\
	$(LIBOC)(xzao647.o)

$(LIBOA): $(LIBOA)(xzag25.o)\
	$(LIBOA)(xzag.o)\
	$(LIBOA)(xzag15.o)\
	$(LIBOA)(xzag08.o)\
	$(LIBOA)(xzag09.o)\
	$(LIBOA)(xzag100.o)\
	$(LIBOA)(asql_com.o)\
	$(LIBOA)(xzaa.o)\
	$(LIBOA)(xzaa30.o)\
	$(LIBOA)(xzam.o)\
	$(LIBOA)(xzat.o)\
	$(LIBOA)(xzao561.o)\
	$(LIBOA)(xzao563.o)\
	$(LIBOA)(xzao123.o)\
	$(LIBOA)(xzao.o)\
	$(LIBOA)(xzao51.o)\
	$(LIBOA)(xzao53.o)\
	$(LIBOA)(xzao55.o)\
	$(LIBOA)(xzao57.o)\
	$(LIBOA)(xzao59.o)\
	$(LIBOA)(xzao61.o)\
	$(LIBOA)(xzao63.o)\
	$(LIBOA)(xzao64.o)\
	$(LIBOA)(xzao65.o)\
	$(LIBOA)(xzao66.o)\
	$(LIBOA)(xzao67.o)\
	$(LIBOA)(xzao68.o)\
	$(LIBOA)(xzao69.o)\
	$(LIBOA)(xzao70.o)\
	$(LIBOA)(xzao71.o)\
	$(LIBOA)(xzao72.o)\
	$(LIBOA)(xzao73.o)\
	$(LIBOA)(xzao80.o)\
	$(LIBOA)(xzao81.o)\
	$(LIBOA)(xzao84.o)\
	$(LIBOA)(xzao86.o)\
	$(LIBOA)(xzao88.o)\
	$(LIBOA)(xzao90.o)\
	$(LIBOA)(xzao92.o)\
	$(LIBOA)(xzao94.o)\
	$(LIBOA)(xzao119.o)\
	$(LIBOA)(xzao121.o)\
	$(LIBOA)(xzao137.o)\
	$(LIBOA)(xzao138.o)\
	$(LIBOA)(xzao139.o)\
	$(LIBOA)(xzao140.o)\
	$(LIBOA)(xzao141.o)\
	$(LIBOA)(xzao142.o)\
	$(LIBOA)(xzao144.o)\
	$(LIBOA)(xzao143.o)\
	$(LIBOA)(xzao146.o)\
	$(LIBOA)(xzao151.o)\
	$(LIBOA)(xzao155.o)\
	$(LIBOA)(xzao157.o)\
	$(LIBOA)(xzao159.o)\
	$(LIBOA)(xzao161.o)\
	$(LIBOA)(xzao350.o)\
	$(LIBOA)(xzao351.o)\
	$(LIBOA)(xzao352.o)\
	$(LIBOA)(xzao444.o)\
	$(LIBOA)(xzao450.o)\
	$(LIBOA)(xzao451.o)\
	$(LIBOA)(xzao166.o)\
	$(LIBOA)(xzao0166.o)\
	$(LIBOA)(xzao168.o)\
	$(LIBOA)(xzao170.o)\
	$(LIBOA)(xzao994.o)\
	$(LIBOA)(xzao996.o)\
	$(LIBOA)(xzao998.o)\
	$(LIBOA)(xzao1012.o)\
	$(LIBOA)(xzao1000.o)\
	$(LIBOA)(xzao1002.o)\
	$(LIBOA)(xzao1004.o)\
	$(LIBOA)(xzao1006.o)\
	$(LIBOA)(xzao1008.o)\
	$(LIBOA)(xzao1009.o)\
	$(LIBOA)(xzao1010.o)\
	$(LIBOA)(xzao982.o)\
	$(LIBOA)(xzao984.o)\
	$(LIBOA)(xzao986.o)\
	$(LIBOA)(xzao988.o)\
	$(LIBOA)(xzao990.o)\
	$(LIBOA)(xzao1014.o)\
	$(LIBOA)(xzao172.o)\
	$(LIBOA)(xzao174.o)\
	$(LIBOA)(xzao176.o)\
	$(LIBOA)(xzao178.o)\
	$(LIBOA)(xzao978.o)\
	$(LIBOA)(xzao980.o)\
	$(LIBOA)(xzao180.o)\
	$(LIBOA)(xzao182.o)\
	$(LIBOA)(xzao184.o)\
	$(LIBOA)(xzao992.o)\
	$(LIBOA)(xzao187.o)\
	$(LIBOA)(xzao188.o)\
	$(LIBOA)(xzao189.o)\
	$(LIBOA)(xzao193.o)\
	$(LIBOA)(xzao196.o)\
	$(LIBOA)(xzao197.o)\
	$(LIBOA)(xzao198.o)\
	$(LIBOA)(xzao312.o)\
	$(LIBOA)(xzao403.o)\
	$(LIBOA)(xzao416.o)\
	$(LIBOA)(xzao816.o)\
	$(LIBOA)(xzao830.o)\
	$(LIBOA)(xzao4290.o)\
	$(LIBOA)(xzao435.o)\
	$(LIBOA)(xzao436.o)\
	$(LIBOA)(xzao420.o)\
	$(LIBOA)(xzao404.o)\
	$(LIBOA)(xzao405.o)\
	$(LIBOA)(xzao406.o)\
	$(LIBOA)(xzao407.o)\
	$(LIBOA)(xzao408.o)\
	$(LIBOA)(xzao417.o)\
	$(LIBOA)(xzao418.o)\
	$(LIBOA)(xzao421.o)\
	$(LIBOA)(xzao510.o)\
	$(LIBOA)(xzao511.o)\
	$(LIBOA)(xzao551.o)\
	$(LIBOA)(xzao553.o)\
	$(LIBOA)(xzao554.o)\
	$(LIBOA)(xzao571.o)\
	$(LIBOA)(xzao572.o)\
	$(LIBOA)(xzao556.o)\
	$(LIBOA)(xzao557.o)\
	$(LIBOA)(xzao910.o)\
	$(LIBOA)(xzao790.o)\
	$(LIBOA)(xzao558.o)\
	$(LIBOA)(xzao570.o)\
	$(LIBOA)(xzao578.o)\
	$(LIBOA)(xzao600.o)\
	$(LIBOA)(xzao601.o)\
	$(LIBOA)(xzao602.o)\
	$(LIBOA)(xzao603.o)\
	$(LIBOA)(xzao604.o)\
	$(LIBOA)(xzao605.o)\
	$(LIBOA)(xzao606.o)\
	$(LIBOA)(xzao607.o)\
	$(LIBOA)(xzao720.o)\
	$(LIBOA)(xzao649.o)\
	$(LIBOA)(xzao650.o)\
	$(LIBOA)(xzao651.o)\
	$(LIBOA)(xzao730.o)\
	$(LIBOA)(xzao257.o)\
	$(LIBOA)(xzao780.o)\
	$(LIBOA)(xzao870.o)\
	$(LIBOA)(xzao801.o)\
	$(LIBOA)(xzao422.o)\
	$(LIBOA)(xzao462.o)\
	$(LIBOA)(xzao461.o)\
	$(LIBOA)(xzao463.o)\
	$(LIBOA)(xzao574.o)\
	$(LIBOA)(xzao952.o)\
	$(LIBOA)(xzao953.o)\
	$(LIBOA)(xzav01.o)\
	$(LIBOA)(xzav03.o)

$(LIBOL): $(LIBOL)(xzao74.o)\
	$(LIBOL)(xzao75.o)\
	$(LIBOL)(xzao76.o)\
	$(LIBOL)(xzao77.o)\
	$(LIBOL)(xzao78.o)\
	$(LIBOL)(xzao79.o)\
	$(LIBOL)(xzao82.o)\
	$(LIBOL)(xzao115.o)\
	$(LIBOL)(xzao116.o)\
	$(LIBOL)(xzao117.o)\
	$(LIBOL)(xzao122.o)\
	$(LIBOL)(xzao124.o)\
	$(LIBOL)(xzao125.o)\
	$(LIBOL)(xzao126.o)\
	$(LIBOL)(xzao127.o)\
	$(LIBOL)(xzao128.o)\
	$(LIBOL)(xzao129.o)\
	$(LIBOL)(xzao130.o)\
	$(LIBOL)(xzao131.o)\
	$(LIBOL)(xzao132.o)\
	$(LIBOL)(xzao133.o)\
	$(LIBOL)(xzao134.o)\
	$(LIBOL)(xzao135.o)\
	$(LIBOL)(xzao136.o)\
	$(LIBOL)(xzao147.o)\
	$(LIBOL)(xzao148.o)\
	$(LIBOL)(xzao501.o)\
	$(LIBOL)(xzao149.o)\
	$(LIBOL)(xzao152.o)\
	$(LIBOL)(xzao163.o)\
	$(LIBOL)(xzao452.o)\
	$(LIBOL)(xzao453.o)\
	$(LIBOL)(xzao164.o)\
	$(LIBOL)(xzao190.o)\
	$(LIBOL)(xzao208.o)\
	$(LIBOL)(xzao203.o)\
	$(LIBOL)(xzao2031.o)\
	$(LIBOL)(xzao204.o)\
	$(LIBOL)(xzao205.o)\
	$(LIBOL)(xzao208.o)\
	$(LIBOL)(xzao214.o)\
	$(LIBOL)(xzao225.o)\
	$(LIBOL)(xzao250.o)\
	$(LIBOL)(xzao251.o)\
	$(LIBOL)(xzao253.o)\
	$(LIBOL)(xzao306.o)\
	$(LIBOL)(xzao4001.o)\
	$(LIBOL)(xzao401.o)\
	$(LIBOL)(xzao470.o)\
	$(LIBOL)(xzao412.o)\
	$(LIBOL)(xzao413.o)\
	$(LIBOL)(xzao419.o)\
	$(LIBOL)(xzao424.o)\
	$(LIBOL)(xzao425.o)\
	$(LIBOL)(xzao443.o)\
	$(LIBOL)(xzao426.o)\
	$(LIBOL)(xzao427.o)\
	$(LIBOL)(xzao428.o)\
	$(LIBOL)(xzao433.o)\
	$(LIBOL)(xzao434.o)\
	$(LIBOL)(xzao440.o)\
	$(LIBOL)(xzao256.o)\
	$(LIBOL)(xzao441.o)\
	$(LIBOL)(xzao442.o)\
	$(LIBOL)(xzao402.o)\
	$(LIBOL)(xzao409.o)\
	$(LIBOL)(xzao410.o)\
	$(LIBOL)(xzao465.o)\
	$(LIBOL)(xzao466.o)\
	$(LIBOL)(xzao467.o)\
	$(LIBOL)(xzao468.o)\
	$(LIBOL)(xzao469.o)

$(LIBA): $(LIBA)(xzau16.o)\
	$(LIBA)(xzae560.o)\
	$(LIBA)(xzae562.o)\
	$(LIBA)(xzae563.o)\
	$(LIBA)(xzae564.o)\
	$(LIBA)(xzae565.o)\
	$(LIBA)(xzae566.o)\
	$(LIBA)(xzae567.o)\
	$(LIBA)(xzae568.o)\
	$(LIBA)(xzae08.o)\
        $(LIBA)(xzae11.o)\
	$(LIBA)(xzae20.o)\
	$(LIBA)(xzae21.o)\
	$(LIBA)(xzae22.o)\
	$(LIBA)(xzae23.o)\
	$(LIBA)(xzae24.o)\
	$(LIBA)(xzae26.o)\
	$(LIBA)(xzae27.o)\
	$(LIBA)(xzae29.o)\
	$(LIBA)(xzae30.o)\
	$(LIBA)(xzae31.o)\
	$(LIBA)(xzae32.o)\
	$(LIBA)(xzae40.o)\
        $(LIBA)(xzae41.o)\
        $(LIBA)(xzae42.o)\
        $(LIBA)(xzae44.o)\
        $(LIBA)(xzae45.o)\
        $(LIBA)(xzae46.o)\
        $(LIBA)(xzae47.o)\
        $(LIBA)(xzae48.o)\
        $(LIBA)(xzae49.o)\
        $(LIBA)(xzae50.o)\
        $(LIBA)(xzae51.o)\
        $(LIBA)(xzae52.o)\
	$(LIBA)(xzae53.o)\
        $(LIBA)(xzae54.o)\
        $(LIBA)(xzae55.o)\
        $(LIBA)(xzae56.o)\
        $(LIBA)(xzae57.o)\
        $(LIBA)(xzae58.o)\
        $(LIBA)(xzae60.o)\
        $(LIBA)(xzae61.o)\
        $(LIBA)(xzae62.o)\
        $(LIBA)(xzae63.o)\
        $(LIBA)(xzae65.o)\
        $(LIBA)(xzae67.o)\
	$(LIBA)(xzae68.o)\
	$(LIBA)(xzae69.o)\
	$(LIBA)(xzae97.o)\
	$(LIBA)(xzae641.o)\
	$(LIBA)(xzae101.o)\
	$(LIBA)(xzae102.o)\
	$(LIBA)(xzae103.o)\
	$(LIBA)(xzae123.o)\
	$(LIBA)(xzae126.o)\
	$(LIBA)(xzae185.o)\
	$(LIBA)(xzae127.o)\
	$(LIBA)(xzae1270.o)\
	$(LIBA)(xzae129.o)\
	$(LIBA)(xzae153.o)\
	$(LIBA)(xzae155.o)\
	$(LIBA)(xzae150.o)\
	$(LIBA)(xzae131.o)\
	$(LIBA)(xzae132.o)\
	$(LIBA)(xzae133.o)\
	$(LIBA)(xzae134.o)\
	$(LIBA)(xzae135.o)\
	$(LIBA)(xzae136.o)\
	$(LIBA)(xzae137.o)\
	$(LIBA)(xzae138.o)\
	$(LIBA)(xzae139.o)\
	$(LIBA)(xzae140.o)\
	$(LIBA)(xzae146.o)\
	$(LIBA)(xzae160.o)\
	$(LIBA)(xzae162.o)\
	$(LIBA)(xzae163.o)\
	$(LIBA)(xzae164.o)\
	$(LIBA)(xzae165.o)\
	$(LIBA)(xzae166.o)\
	$(LIBA)(xzae170.o)\
	$(LIBA)(xzae171.o)\
	$(LIBA)(xzae172.o)\
	$(LIBA)(xzae173.o)\
	$(LIBA)(xzae175.o)\
	$(LIBA)(xzae178.o)\
	$(LIBA)(xzae179.o)\
	$(LIBA)(xzae182.o)\
	$(LIBA)(xzae200.o)\
	$(LIBA)(xzae220.o)\
	$(LIBA)(xzae201.o)\
	$(LIBA)(xzae207.o)\
	$(LIBA)(xzae550.o)\
	$(LIBA)(xzae552.o)\
	$(LIBA)(xzae553.o)\
	$(LIBA)(xzae980.o)\
	$(LIBA)(xzae981.o)\
	$(LIBA)(xzae554.o)\
	$(LIBA)(xzae555.o)\
	$(LIBA)(xzae556.o)\
	$(LIBA)(xzae860.o)\
	$(LIBA)(xzae861.o)\
	$(LIBA)(xzan01.o)\
	$(LIBA)(xzan02.o)\
	$(LIBA)(xzan34.o)\
	$(LIBA)(xzan36.o)\
	$(LIBA)(xzan37.o)\
	$(LIBA)(xzar03.o)\
	$(LIBA)(xzai07.o)\
	$(LIBA)(xzai500.o)\
	$(LIBA)(xzas02.o)\
	$(LIBA)(xzas03.o)\
	$(LIBA)(xzas91.o)\
	$(LIBA)(xzas06.o)\
	$(LIBA)(xzas10.o)\
	$(LIBA)(xzas11.o)\
	$(LIBA)(xzas12.o)\
	$(LIBA)(xzas13.o)\
	$(LIBA)(xzas50.o)\
	$(LIBA)(xzas51.o)\
	$(LIBA)(xzas52.o)\
	$(LIBA)(xzas53.o)\
	$(LIBA)(xzas54.o)\
	$(LIBA)(xzas540.o)\
	$(LIBA)(xzas55.o)\
	$(LIBA)(xzas62.o)\
	$(LIBA)(xzas70.o)\
	$(LIBA)(xzas80.o)\
	$(LIBA)(xzas81.o)\
	$(LIBA)(xzas82.o)\
	$(LIBA)(xzas83.o)\
	$(LIBA)(xzas87.o)\
	$(LIBA)(xzas88.o)\
	$(LIBA)(xzas100.o)\
	$(LIBA)(xzat01.o)\
	$(LIBA)(xzat05.o)\
	$(LIBA)(xzat050.o)\
	$(LIBA)(xzat060.o)\
	$(LIBA)(xzat06.o)\
	$(LIBA)(xzat51.o)\
	$(LIBA)(xzat511.o)\
	$(LIBA)(xzat35.o)\
	$(LIBA)(xzat36.o)\
	$(LIBA)(xzat07.o)\
	$(LIBA)(xzat08.o)\
	$(LIBA)(xzat280.o)\
	$(LIBA)(xzat100.o)\
	$(LIBA)(xzah01.o)\
	$(LIBA)(xzah03.o)\
	$(LIBA)(xzah02.o)\
	$(LIBA)(xzah04.o)\
	$(LIBA)(xzah05.o)\
	$(LIBA)(xzah07.o)\
	$(LIBA)(xzah08.o)\
	$(LIBA)(xzah10.o)\
	$(LIBA)(xzah11.o)\
	$(LIBA)(xzac22.o)\
	$(LIBA)(xzac23.o)\
	$(LIBA)(xzac25.o)\
	$(LIBA)(xzac26.o)\
	$(LIBA)(xzac70.o)\
	$(LIBA)(xzac71.o)\
	$(LIBA)(xzac73.o)\
	$(LIBA)(xzac75.o)\
	$(LIBA)(xzac66.o)\
	$(LIBA)(xzac03.o)\
	$(LIBA)(xzac103.o)\
	$(LIBA)(xzac01.o)\
	$(LIBA)(xzac101.o)\
	$(LIBA)(xzac803.o)\
	$(LIBA)(xzac06.o)\
	$(LIBA)(xzac106.o)\
	$(LIBA)(xzac61.o)\
	$(LIBA)(xzac610.o)\
	$(LIBA)(xzac63.o)\
	$(LIBA)(xzac82.o)\
	$(LIBA)(xzac802.o)\
	$(LIBA)(xzac8021.o)\
	$(LIBA)(xzac182.o)\
	$(LIBA)(xzac550.o)\
	$(LIBA)(xzac84.o)\
	$(LIBA)(xzac840.o)\
	$(LIBA)(xzac92.o)\
	$(LIBA)(xzac192.o)\
	$(LIBA)(xzac93.o)\
	$(LIBA)(xzac193.o)\
	$(LIBA)(xzac98.o)\
	$(LIBA)(xzac99.o)\
	$(LIBA)(xzac67.o)\
	$(LIBA)(xzac62.o)\
	$(LIBA)(xzac80.o)\
	$(LIBA)(xzac81.o)\
	$(LIBA)(xzac102.o)\
	$(LIBA)(xzac100.o)\
	$(LIBA)(xzac108.o)\
	$(LIBA)(xzac303.o)\
	$(LIBA)(xzac631.o)\
	$(LIBA)(xzac632.o)\
	$(LIBA)(xzac633.o)\
	$(LIBA)(xzac634.o)\
	$(LIBA)(xzac635.o)\
	$(LIBA)(xzac636.o)\
	$(LIBA)(xzac06.o)\
	$(LIBA)(xzac92.o)\
	$(LIBA)(xzac99.o)\
	$(LIBA)(xzac86.o)\
	$(LIBA)(xzac1000.o)\
	$(LIBA)(xzap21.o)\
	$(LIBA)(xzao15.o)\
	$(LIBA)(xzao16.o)\
	$(LIBA)(xzao17.o)\
	$(LIBA)(xzao19.o)\
	$(LIBA)(xzao20.o)\
	$(LIBA)(xzao21.o)\
	$(LIBA)(xzao22.o)\
	$(LIBA)(xzao500.o)\
	$(LIBA)(xzao576.o)\
	$(LIBA)(xzao5761.o)\
	$(LIBA)(xzao577.o)\
	$(LIBA)(xzao5771.o)\
	$(LIBA)(xzao630.o)\
	$(LIBA)(xzao631.o)\
	$(LIBA)(xzao632.o)\
	$(LIBA)(xzao633.o)\
	$(LIBA)(xzao634.o)\
	$(LIBA)(xzao635.o)\
	$(LIBA)(xzao636.o)\
	$(LIBA)(xzao637.o)\
	$(LIBA)(xzah21.o)\
	$(LIBA)(xzah26.o)\
	$(LIBA)(xzah27.o)\
	$(LIBA)(xzaz01.o)\
	$(LIBA)(xzaf02.o)\
	$(LIBA)(xzaf03.o)\
	$(LIBA)(xzaf05.o)\
	$(LIBA)(xzaf06.o)\
	$(LIBA)(xzaf08.o)\
	$(LIBA)(xzaf10.o)\
	$(LIBA)(xzaf14.o)\
	$(LIBA)(xzaf15.o)\
	$(LIBA)(xzaf16.o)\
	$(LIBA)(xzaf11.o)\
	$(LIBA)(xzaq.o)

$(LIBAP): $(LIBAP)(xzap01.o)\
	$(LIBAP)(xzap02.o)\
	$(LIBAP)(xzap03.o)\
	$(LIBAP)(xzap04.o)\
	$(LIBAP)(xzap05.o)\
	$(LIBAP)(xzap06.o)\
	$(LIBAP)(xzap07.o)\
	$(LIBAP)(xzap08.o)\
	$(LIBAP)(xzap09.o)\
	$(LIBAP)(xzap10.o)\
	$(LIBAP)(xzap22.o)\
	$(LIBAP)(xzap23.o)\
	$(LIBAP)(xzap26.o)\
	$(LIBAP)(xzap27.o)\
	$(LIBAP)(xzap28.o)\
	$(LIBAP)(xzap82.o)\
	$(LIBAP)(xzap821.o)\
	$(LIBAP)(xzap29.o)\
	$(LIBAP)(xzap33.o)\
	$(LIBAP)(xzap25.o)

$(LIBI): $(LIBI)(xzis.o)\
	$(LIBI)(xzir.o)\
	$(LIBI)(xzia.o)\
	$(LIBI)(xzit.o)

$(LIBP): $(LIBP)(xzdd.o)\
	$(LIBP)(xzdc.o)

$(LIBM): $(LIBM)(xzma.o)


########### Fin de zone a modifier par le developpeur ################

.PRECIOUS : $(LIB)


verifier_plateforme :
	if test "$(MODELE)" != "$(CIBLE)" ; \
	then	echo "Ce fichier Makefile N'EST PAS DESTINE A CETTE PLATEFORME. NE PAS GENERER DE BINAIRE !!!" ; \
		false ; \
	fi

#
# phase de menage du makefile
#

clean : verifier_plateforme
	echo "Menage"
	rm -f $(DIRLIB)/*.a
	rm -f $(EXE)
#
#
# Redefinition de la regles de construction d'un .o a partir d'un .c
#
#
.c.o : verifier_plateforme
	echo " "
	if test $(OPTLINT) = yes;\
          then echo "PHASE DE VERIFICATION SYNTAXIQUE : "   $<;\
               $(LINT) $(LFLAGS)  $<;\
          else echo $< "PAS DE VERIFICATION SYNTAXIQUE ";\
        fi;
	echo
	echo "PHASE DE COMPILATION             : "   $<
	echo " "
	$(CC)   $(CFLAGS) -c $<

#
# Redefinition de la regles de construction d'un .a a partir d'un .o
#
.o.a: verifier_plateforme
	echo "CREATION DE LIBRAIRIE "$@.a
	ar rv $@.a $<
	touch $@.a 
	rm -f $<

#
# Redefinition de la regles de construction d'un .a a partir d'un .c
#
.c.a : verifier_plateforme
	echo " "
	if test $(OPTLINT) = yes;\
          then echo "PHASE DE VERIFICATION SYNTAXIQUE : "   $*.c;\
               $(LINT) $(LFLAGS)  $*.c;\
          else echo  "PAS DE VERIFICATION SYNTAXIQUE " $*.c;\
        fi;
	echo
	echo "PHASE DE COMPILATION             : "   $*.c
	echo " "
	$(CC)   $(CFLAGS) -c $*.c
	echo "MISE A JOUR DE LIBRAIRIE :"$@
	ar rv $@ $*.o
	touch $@ 
	rm -f $*.o


