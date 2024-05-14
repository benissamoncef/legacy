#
#  FICHIER: $Id: makefile,v 1.4 2011/12/21 09:19:07 gesconf Exp $        REL: $Revision: 1.4 $        DATE: $Date: 2011/12/21 09:19:07 $
#
#  Ce fichier permet de compiler, de linker les fichiers concernant
#   les tests du sous-systeme SUPERV.
#
###################################################################
# Inhibition de l'echo des commandes
#
.SILENT:
#
# Ajout du suffixe des fichiers PRO*C ORACLE
#
.SUFFIXES: .sc 
#
##################################################################
#
# definitions de variables du makefile 
#
#

# nom du sous systeme
SSYST = SUPERV
PROJHOME  = $(HOME)/migrazur

#
# definitions de variables decrivant les differents chemins d'acces
#

DIRINC = $(PROJHOME)/$(SSYST)/inc
DIRSRC = $(PROJHOME)/$(SSYST)/src
DIRLIB = $(PROJHOME)/$(SSYST)/lib
DIREXE = $(PROJHOME)/$(SSYST)/bin
DIRTEST = $(PROJHOME)/$(SSYST)/tests

DIRRTWORKS = $(RTHOME)/include

# Paths  communs
#
XDMICGINC = $(PROJHOME)/XDMICG/inc
XDMICGLIB = $(PROJHOME)/XDMICG/lib

RTLINK = $(RTHOME)/bin/rtlink


# compilateur

CC = cc
#CC = purify cc

CFLAGS = -g -D_HPUX_SOURCE -Aa +e\
	-I$(DIRRTWORKS)\
	-I$(DIRINC)\
	-I$(XDMICGINC)


# lint
OPTLINT=no
#LINTDIR=/opt/SUNWspro/bin
#LINT=$(LINTDIR)/lint

# Definition des flags du lint
LFLAGS=	-u			\
	-I$(DIRRTWORKS)		\
        -I$(DIRINC)		\
	-I$(XDMICGINC)


# librairies communes applicatives utilisees  

LIB =   $(XDMICGLIB)/xzs.a\
	$(XDMICGLIB)/xzc.a\
	$(XDMICGLIB)/xd.a	


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

INCICG = $(XDMICGINC)/xdc.h 		\
	 $(XDMICGINC)/xdy.h		\
	 $(XDMICGINC)/xdg.h		\
	 $(XDMICGINC)/xdl.h		\
	 $(XDMICGINC)/xdm.h		\
	 $(XDMICGINC)/xdf.h


#
# Sources 
#

SRCTEST_TSHOR = $(DIRTEST)/tests_shor.c
SRCTEST_TSTIM = $(DIRTEST)/tests_stim.c
SRCTEST_TSPNG = $(DIRTEST)/tests_spng.c
SRCTEST_XZSS = $(DIRTEST)/tests_xzss.c
SRCTEST_XZSC = $(DIRTEST)/tests_xzsc.c
SRCTEST_XZSA = $(DIRTEST)/tests_xzsa.c

#
# librairie d'un executable
#

OBJTEST_TSHOR = $(DIRTEST)/tests_shor.o
OBJTEST_TSTIM = $(DIRTEST)/tests_stim.o
OBJTEST_TSPNG = $(DIRTEST)/tests_spng.o
OBJTEST_XZSS = $(DIRTEST)/tests_xzss.o
OBJTEST_XZSA = $(DIRTEST)/tests_xzsa.o
OBJTEST_XZSC = $(DIRTEST)/tests_xzsc.o

#
# DEFINITION DU NOM DES EXECUTABLES 
#
# nom des executables
#

EXETEST_TSHOR = $(DIRTEST)/tests_shor.x
EXETEST_TSTIM = $(DIRTEST)/tests_stim.x
EXETEST_TSPNG = $(DIRTEST)/tests_spng.x
EXETEST_XZSS = $(DIRTEST)/tests_xzss.x
EXETEST_XZSA = $(DIRTEST)/tests_xzsa.x
EXETEST_XZSC = $(DIRTEST)/tests_xzsc.x

EXE = $(EXETEST_TSHOR) $(EXETEST_TSTIM) $(EXETEST_TSPNG) $(EXETEST_XZSS) $(EXETEST_XZSA) $(EXETEST_XZSC)

#
# DEFINITION DES REGLES DE GENERATION 
#
# Premiere regle il faut tout faire : les communs et les executables

tout : $(EXE)
	echo
	@echo LES TESTS DU SOUS-SYSTEME $(SSYST) SONT A JOUR. 
	echo


# -- Regles utiles simplement pour lancer independamment les
#  compilations. Exemple make tests_shor.x

tests_shor.x : $(EXETEST_TSHOR)
	echo

tests_spng.x : $(EXETEST_TSPNG)
	echo

tests_stim.x : $(EXETEST_TSTIM)
	echo

tests_xzss.x : $(EXETEST_XZSS)
	echo

tests_xzsa.x : $(EXETEST_XZSA)
	echo

tests_xzsc.x : $(EXETEST_XZSC)
	echo




#
# GENERATION  D'UN EXECUTABLE 
#
# Regle de generation d'un executable a partir 
# de ses fichiers sources, de ses objets et des librairies communes
#
# Phase edition de lien
#



$(EXETEST_TSHOR) : $(SRCTEST) $(OBJTEST_TSHOR) $(LIB)
	echo
	echo "PHASE EDITION DE LIEN :"$@
	$(RTLINK)  $(OBJTEST_TSHOR) $(LIB) $(LDFLAGS)  -o $@
	echo
	echo "TAILLE DE L'EXECUTABLE :"$@
	@size $@

$(EXETEST_TSTIM) : $(SRCTEST) $(OBJTEST_TSTIM) $(LIB)
	echo
	echo "PHASE EDITION DE LIEN :"$@
	$(RTLINK)  $(OBJTEST_TSTIM) $(LIB) $(LDFLAGS)  -o $@
	echo
	echo "TAILLE DE L'EXECUTABLE :"$@
	@size $@

$(EXETEST_TSPNG) : $(SRCTEST) $(OBJTEST_TSPNG) $(LIB)
	echo
	echo "PHASE EDITION DE LIEN :"$@
	$(RTLINK)  $(OBJTEST_TSPNG) $(LIB) $(LDFLAGS)  -o $@
	echo
	echo "TAILLE DE L'EXECUTABLE :"$@
	@size $@

$(EXETEST_XZSS) : $(SRCTEST) $(OBJTEST_XZSS) $(LIB)
	echo
	echo "PHASE EDITION DE LIEN :"$@
	$(RTLINK)  $(OBJTEST_XZSS) $(LIB) $(LDFLAGS)  -o $@
	echo
	echo "TAILLE DE L'EXECUTABLE :"$@
	@size $@

$(EXETEST_XZSA) : $(SRCTEST) $(OBJTEST_XZSA) $(LIB)
	echo
	echo "PHASE EDITION DE LIEN :"$@
	$(RTLINK)  $(OBJTEST_XZSA) $(LIB) $(LDFLAGS)  -o $@
	echo
	echo "TAILLE DE L'EXECUTABLE :"$@
	@size $@

$(EXETEST_XZSC) : $(SRCTEST) $(OBJTEST_XZSC) $(LIB)
	echo
	echo "PHASE EDITION DE LIEN :"$@
	$(RTLINK)  $(OBJTEST_XZSC) $(LIB) $(LDFLAGS)  -o $@
	echo
	echo "TAILLE DE L'EXECUTABLE :"$@
	@size $@

# 
# Regle liant les sources aux fichiers include communs et au makefile  
# 


$(SRCTEST_TSHOR) :$(INCICG) 
	touch $@
$(SRCTEST_TSTIM) :$(INCICG) 
	touch $@
$(SRCTEST_TSPNG) :$(INCICG) 
	touch $@
$(SRCTEST_XZSS) :$(INCICG) 
	touch $@
$(SRCTEST_XZSA) :$(INCICG) 
	touch $@
$(SRCTEST_XZSC) :$(INCICG) 
	touch $@


########### Fin de zone a modifier par le developpeur ################

#
# phase de menage du makefile
#
clean :
	rm -f ./*.o
	rm -f $(EXE)

#
#
# Redefinition de la regles de construction d'un .o a partir d'un .c
#
#
.c.o :
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

