#
#  FICHIER: modele_rtworks.make
#
#  Ce fichier est un modele de make permettant 
# de se connecter a un RTSERVER 
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
#
SSYST = SUPERV
PROJHOME  = $(HOME)/migrazur
#
# definitions de variables decrivant les differents chemins d'acces
#

DIRSRC = $(PROJHOME)/$(SSYST)/src
DIRTEST = $(PROJHOME)/$(SSYST)/tests
DIREXE = $(PROJHOME)/$(SSYST)/bin
DIRLIB = $(PROJHOME)/$(SSYST)/lib
DIRINC = $(PROJHOME)/$(SSYST)/inc

# Paths  communs
# A decommenter une fois livre
ICGINC = $(PROJHOME)/XDMICG/inc
ICGLIB = $(PROJHOME)/XDMICG/lib

# Definition des flags de compilation et de correcteur syntaxique
#
DIRRTWORKS = $(RTHOME)/include

# rtlink
RTLINK = $(RTHOME)/bin/rtlink

# compilateur
CC = cc
#CC = purify cc
CFLAGS = -g -D_HPUX_SOURCE -Aa\
	-I$(DIRRTWORKS)\
	-I$(ICGINC)\
	-I$(DIRINC)\
        -I/users/milleville/migrazur/BASEDD/inc\
        -I/users/milleville/migrazur/EQUEXT/inc\
        -I/users/milleville/migrazur/GECONF/inc\
        -I/users/milleville/migrazur/GESIHM/inc\
        -I/users/milleville/migrazur/MAINTENANCE/inc\
        -I/users/milleville/migrazur/SAIDEC/inc\
        -I/users/milleville/migrazur/SUPERV/inc\
        -I/users/milleville/migrazur/XDMICG/inc

OPTLINT=no



# Definition des flags du lint
LFLAGS=	-u			\
	-I$(DIRRTWORKS)		\
        -I$(ICGINC)		\
	-I$(DIRINC)


# librairies communes applicatives utilisees  

LIB =   $(ICGLIB)/xzs.a\
	$(ICGLIB)/xzg.a\
	$(ICGLIB)/xzc.a\
	$(ICGLIB)/xd.a	


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
INCICG = $(ICGINC)/xdc.h 		\
	 $(ICGINC)/xdy.h		\
	 $(ICGINC)/xdg.h		\
	 $(ICGINC)/xdl.h		\
	 $(ICGINC)/xdm.h		\
	 $(ICGINC)/xdf.h


#
# Sources 
#


SRCTEST = $(DIRTEST)/modele_rtworks.c

#
# librairie d'un executable
#

OBJTEST = $(DIRTEST)/modele_rtworks.o

#
# DEFINITION DU NOM DES EXECUTABLES 
#
# nom des executables
#

EXETEST = $(DIRTEST)/modele_rtworks.x

#
# DEFINITION DES REGLES DE GENERATION 
#
# Premiere regle il faut tout faire : les communs et les executables

tout : $(EXETEST) 
	echo
	@echo LE SOUS-SYSTEME $(SSYST) EST A JOUR. 
	echo


# -- Regles utiles simplement pour lancer independamment les
#  compilations. Exemple make tests_shor.x

modele_rtworks.x : $(EXETEST)
	echo



#
# GENERATION  D'UN EXECUTABLE 
#
# Regle de generation d'un executable a partir 
# de ses fichiers sources, de ses objets et des librairies communes
#
# Phase edition de lien
#



$(EXETEST) : $(SRCTEST) $(OBJTEST) $(LIB)
	echo
	echo "PHASE EDITION DE LIEN :"$@
	$(RTLINK)  $(OBJTEST) $(LIB) $(LDFLAGS)  -o $@
	echo
	echo "TAILLE DE L'EXECUTABLE :"$@
	@size $@


# 
# Regle liant les sources aux fichiers include communs et au makefile  
# 


$(SRCTEST) :$(INCICG) 
	touch $@

########### Fin de zone a modifier par le developpeur ################
#
# phase de menage du makefile
#
clean:
	rm -f $(DIRLIB)/*.a
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

