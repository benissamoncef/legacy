#
#  FICHIER: @(#)Makefile	1.25         REL: 1.25        DATE: 07/09/07
#
#  Ce fichier est dependant des variables d'environnement :
#  $HOME, $COMPILDIR, $II_SYSTEM $RTHOME
#
###################################################################
#
# Inhibition de l'echo des commandes
#
.SILENT:
#
# Ajout du suffixe des fichiers PRO*C ORACLE
#
#.SUFFIXES: .sc 
#
##################################################################
#
# Historique
#	Modification :	
#	Mercier		Suppresion -g 				V 1.10
#	Guilhou		ajout compil dedi  			V 1.11
#	Gaborit		ajout cible tdpac.x			V 1.13
#	Thomas		ajout include xzao123.h			V 1.15
#	Thomas		suppression de la copie de ax_prof3	V 1.16
#	Niepcron	ajout de la copie en ax_prof4		V 1.17
#	Niepceron	ajout xd.a				V 1.18
#	Guilhou		ajout dapc_caltdp			V 1.19
#	Niepceron	modification pour rtworks3.5		V 1.20
#	??		ajout de +O3				V 1.21
#	??		??					V 1.22 et 1.23
#	JMG		ajout de drol_pal			V 1.24
# 	VR		Ajout de PRV	(DEM/1016)   V 1.25
# 	GGY		01/12/23 : Ajout drol_pic et drol_pic_util (DEM483)
##################################################################
#
# definitions de variables du makefile 
#
#
# nom du sous systeme
#
SSYST = SAIDEC
PROJHOME  = $(HOME)/migrazur
#
# definitions de variables decrivant les differents chemins d'acces
#

DIRSRC = $(PROJHOME)/$(SSYST)/src
DIREXE = $(PROJHOME)/$(SSYST)/bin
DIRLIB = $(PROJHOME)/$(SSYST)/lib
DIRINC = $(PROJHOME)/$(SSYST)/inc

# Paths  communs
# A decommenter une fois livre
#ICGINC = $(PROJECTDIR)/valid/ICG/inc
#ICGLIB = $(PROJECTDIR)/valid/ICG/lib
ICGINC = $(PROJHOME)/XDMICG/inc
ICGLIB = $(PROJHOME)/XDMICG/lib

DIRRTWORKS = $(RTHOME)/include
#PATCHIE = /produits/PatchIE/src/ie/*.o
PATCHIE_DIR = $(HOME)/PatchIE
PATCHIE = $(PATCHIE_DIR)/parse.o $(PATCHIE_DIR)/pattern.o $(PATCHIE_DIR)/slot.o
DIRSYBASE = $(SYBASE)/include


# *******************************************************************

# -- Modification Intervention SADDI + L.CESAR (19/07/95)
#    Compilation avec l'objet ipc_conn.o  cree a partir du path
#       dans le repertoire rtworks_patch + le patch ut_socket.o

OBJSADDI = /produits/rtworks30/ipc/ipc_conn.o \
	/produits/rtworks30/util/ut_socket.o

# ******************************************************************


# Definition des flags de compilation et de correcteur syntaxique
#
# rtlink
RTLINK = $(RTHOME)/bin/rtlink
RTLINKIE = $(RTHOME)/bin/rtlinkie
RTLINKROL = $(RTHOME)/bin/rtlink

# compilateur
#CC = purify cc
#CC = cc
CC = $(RTLINKIE)
CCROL = $(RTLINKROL)

CFLAGS = -D_HPUX_SOURCE -Aa\
	-I$(DIRRTWORKS)\
	-I$(DIRSYBASE)\
	-I$(ICGINC)\
	-I$(DIRINC)\
	+O3

#LDFLAGS         = -xcg92 \
#                  -L/opt/SUNWspro/SC2.0.1/cg92
# lint 
OPTLINT=no


# Definition des flags du lint
LFLAGS=	-u			\
	-I$(DIRRTWORKS)		\
	-I$(DIRSYBASE)		\
        -I$(ICGINC)		\
	-I$(DIRINC)


# librairies communes applicatives utilisees  

LIB =   $(ICGLIB)/xd.a 		\
	$(ICGLIB)/xzaoa.a       \
	$(ICGLIB)/xzi.a		\
	$(ICGLIB)/xzs.a		\
	$(ICGLIB)/xze.a		\
	$(ICGLIB)/xzaol.a       \
	$(ICGLIB)/xza.a         \
	$(ICGLIB)/xzaoa.a       \
	$(ICGLIB)/xd.a		\
	$(ICGLIB)/xzap.a	\
	$(ICGLIB)/xzad.a

LIBROL =   $(ICGLIB)/xd.a 		\
	$(ICGLIB)/xzaoa.a       \
	$(ICGLIB)/xzi.a		\
	$(ICGLIB)/xzs.a		\
	$(ICGLIB)/xze.a		\
	$(ICGLIB)/xzaol.a       \
	$(ICGLIB)/xza.a         \
	$(ICGLIB)/xzd.a         \
	$(ICGLIB)/xzaoa.a       \
	$(ICGLIB)/xzaoc.a       \
	$(ICGLIB)/xd.a		\
	$(ICGLIB)/xzap.a	\
	$(ICGLIB)/xzad.a



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
INCICG = $(ICGINC)/xdc.h 	\
	 $(ICGINC)/xdy.h	\
	 $(ICGINC)/xdg.h	\
	 $(ICGINC)/xdm.h	\
	 $(ICGINC)/xzac.h	\
	 $(ICGINC)/xzae.h	\
	 $(ICGINC)/xzae20.h	\
	 $(ICGINC)/xzae21.h	\
	 $(ICGINC)/xzae22.h	\
	 $(ICGINC)/xzae23.h	\
	 $(ICGINC)/xzae24.h	\
	 $(ICGINC)/xzae26.h	\
	 $(ICGINC)/xzae27.h	\
	 $(ICGINC)/xzae29.h	\
	 $(ICGINC)/xzae30.h	\
	 $(ICGINC)/xzae31.h	\
	 $(ICGINC)/xzae32.h	\
	 $(ICGINC)/xzae68.h	\
	 $(ICGINC)/xzae69.h	\
	 $(ICGINC)/xzag.h	\
	 $(ICGINC)/xzai07.h	\
	 $(ICGINC)/xzan37.h	\
	 $(ICGINC)/xzao15.h	\
	 $(ICGINC)/xzao16.h	\
	 $(ICGINC)/xzao19.h	\
	 $(ICGINC)/xzao20.h	\
	 $(ICGINC)/xzao21.h	\
	 $(ICGINC)/xzao123.h	\
	 $(ICGINC)/xzap01.h	\
	 $(ICGINC)/xzap02.h	\
	 $(ICGINC)/xzap03.h	\
	 $(ICGINC)/xzap04.h	\
	 $(ICGINC)/xzap05.h	\
	 $(ICGINC)/xzap06.h	\
	 $(ICGINC)/xzap07.h	\
	 $(ICGINC)/xzap08.h	\
	 $(ICGINC)/xzap09.h	\
	 $(ICGINC)/xzap10.h	\
	 $(ICGINC)/xzap22.h	\
	 $(ICGINC)/xzap23.h	\
	 $(ICGINC)/xzap25.h     \
	 $(ICGINC)/xzat.h	\
	 $(ICGINC)/xzia.h	\
	 $(ICGINC)/xzit.h	\
	 $(ICGINC)/xzsc.h	\
	 $(ICGINC)/xzsm.h	\
	 $(ICGINC)/xzss.h	\
	 $(ICGINC)/xzst.h	


INCIE = $(DIRINC)/dpac_ie.h	\
	$(DIRINC)/dpac_bdc.h	\
	$(DIRINC)/dpac_fctC.h	\
	$(DIRINC)/dpac_lin.h	\
	$(DIRINC)/dpac_pac.h	\
	$(DIRINC)/dpac_util.h	\
	$(DIRINC)/dpac_tra.h	\
	$(DIRINC)/dpac_arbre.h	\
	$(DIRINC)/dpac_app.h	\
	$(DIRINC)/dpac_fax.h	\
	$(DIRINC)/dpac_msg.h	\
	$(DIRINC)/dpac_eqt.h	\
	$(DIRINC)/dpac_pmv.h	\
	$(DIRINC)/dpac_nav.h	\
	$(DIRINC)/dpac_tun.h	\
	$(DIRINC)/dpac_ech.h	\
	$(DIRINC)/dpac_tdp.h	\
	$(DIRINC)/dpac_caltdp.h

INCROL = $(DIRINC)/drol_dir.h	\
	$(DIRINC)/drol_tfm.h	\
	$(DIRINC)/drol_fax.h	\
	$(DIRINC)/drol_nav.h	\
	$(DIRINC)/drol_tun.h	\
	$(DIRINC)/drol_ech.h	\
	$(DIRINC)/drol_pmv.h	\
	$(DIRINC)/drol_pmv_util.h	\
	$(DIRINC)/drol_arbre.h	\
	$(DIRINC)/drol_pal.h	\
	$(DIRINC)/drol_baf.h	\
	$(DIRINC)/drol_bad.h	\
	$(DIRINC)/drol_caltdp.h	\
	$(DIRINC)/drol_pmva.h	\
	$(DIRINC)/drol_pmva_util.h	\
	$(DIRINC)/drol_prv.h	\
	$(DIRINC)/drol_prv_util.h	\
	$(DIRINC)/drol_pic.h	\
	$(DIRINC)/drol_pic_util.h	\
	$(DIRINC)/drol_tdp.h	\
	$(DIRINC)/drol_appel.h

#
# Sources 
#

SRCIE = $(DIRSRC)/dpac_ie.c	\
	$(DIRSRC)/dpac_bdc.c	\
	$(DIRSRC)/dpac_fctC.c	\
	$(DIRSRC)/dpac_lin.c	\
	$(DIRSRC)/dpac_pac.c	\
	$(DIRSRC)/dpac_util.c	\
	$(DIRSRC)/dpac_tra.c	\
	$(DIRSRC)/dpac_arbre.c	\
	$(DIRSRC)/dpac_app.c	\
	$(DIRSRC)/dpac_fax.c	\
	$(DIRSRC)/dpac_msg.c	\
	$(DIRSRC)/dpac_pmv.c	\
	$(DIRSRC)/dpac_nav.c	\
	$(DIRSRC)/dpac_tun.c	\
	$(DIRSRC)/dpac_ech.c	\
	$(DIRSRC)/dpac_tdp.c	\
	$(DIRSRC)/dpac_caltdp.c

SRCROL = $(DIRSRC)/drol_dir.c	\
	$(DIRSRC)/drol_tfm.c	\
	$(DIRSRC)/drol_fax.c	\
	$(DIRSRC)/drol_nav.c	\
	$(DIRSRC)/drol_tun.c	\
	$(DIRSRC)/drol_ech.c	\
	$(DIRSRC)/drol_pmv.c	\
	$(DIRSRC)/drol_pmv_util.c	\
	$(DIRSRC)/drol_arbre.c	\
	$(DIRSRC)/drol_pal.c	\
	$(DIRSRC)/drol_baf.c	\
	$(DIRSRC)/drol_prv.c	\
	$(DIRSRC)/drol_prv_util.c	\
	$(DIRSRC)/drol_pic.c	\
	$(DIRSRC)/drol_pic_util.c	\
	$(DIRSRC)/drol_bad.c	\
	$(DIRSRC)/drol_pmva.c	\
	$(DIRSRC)/drol_pmva_util.c	\
	$(DIRSRC)/drol_tdp.c	\
	$(DIRSRC)/drol_caltdp.c	\
	$(DIRSRC)/drol_appel.c


#
# librairie d'un executable
#

OBJIE  = $(DIRLIB)/tdpac.a

OBJROL = $(DIRLIB)/tdrol.a

#
# DEFINITION DU NOM DES EXECUTABLES 
#
# nom des executables
#

EXEIE  = $(DIREXE)/tdpac.x

EXEROL = $(DIREXE)/tdrol.x

#
# DEFINITION DES REGLES DE GENERATION 
#
# Premiere regle il faut tout faire : les communs et les executables

tout : $(EXEIE) dedi $(EXEROL) 
	echo
	@echo LE SOUS-SYSTEME $(SSYST) EST A JOUR. 
	echo

tdpac.x : $(EXEIE)
	echo

tdrol.x : $(EXEROL)
	echo

#
# GENERATION  D'UN EXECUTABLE 
#
# Regle de generation d'un executable a partir 
# de ses fichiers sources, de ses objets et des librairies communes
#
# Phase edition de lien
#

dedi :	
	cp $(HOME)/axhome/ax_prof3.ie $(HOME)/axhome/ax_prof4
	rm -f $(DIREXE)/*.d
	rm -f $(DIREXE)/*.elo
	cp dedi_make.sh $(DIREXE)
	chmod a+wx $(DIREXE)/dedi_make.sh
	cp dedi_makeref.sh $(DIREXE)
	chmod a+wx $(DIREXE)/dedi_makeref.sh
	cp dedi_getref.sh $(DIREXE)
	chmod a+wx $(DIREXE)/dedi_getref.sh
	cp *.d $(DIREXE)
	chmod ug+w $(DIREXE)/*.d
	DISPLAY=$${DISPLAY:=:0.0} ; export DISPLAY ; applix -macro compil

$(EXEIE) : $(SRCIE) $(OBJIE) $(LIB)
	echo
	echo "PHASE EDITION DE LIEN :"$@
#	$(RTLINKIE) $(CFLAGS) $(OBJIE) $(LIB) -L$(SYBASE)/lib -lblk -lcs -lct -lcomn -lintl -lsybdb -ltcl -linsck -lm $(LDFLAGS) -o $@
	echo $(CC) $(CFLAGS) $(OBJIE) $(LIB) -L$(RTHOME)/lib/hp700_hpux -lrtutil -lrtipc -lrtserver -L$(SYBASE)/lib -lblk -lcs -lct -lcomn -lintl -lsybdb -ltcl -linsck -lm -o $@
	$(CC) $(CFLAGS) $(OBJIE) $(LIB) -L$(RTHOME)/lib/hp700_hpux -lrtutil -lrtipc -lrtserver -L$(SYBASE)/lib -lblk -lcs -lct -lcomn -lintl -lsybdb -ltcl -linsck -lm -o $@
	
	echo
	echo "TAILLE DE L'EXECUTABLE :"$@
	@size $@
	@echo Executable $@ a jour. 

$(EXEROL) : $(SRCROL) $(OBJROL) $(LIBROL)
	echo
	echo "PHASE EDITION DE LIEN :"$@
	echo $(CCROL) $(CFLAGS) $(OBJROL) $(LIBROL) -L$(RTHOME)/lib/hp700_hpux -lrtutil -lrtipc -lrtserver -L$(SYBASE)/lib -lblk -lcs -lct -lcomn -lintl -lsybdb -ltcl -linsck -lm -o $@
	$(CCROL) $(CFLAGS) $(OBJROL) $(LIBROL) -L$(RTHOME)/lib/hp700_hpux -lrtutil -lrtipc -lrtserver -L$(SYBASE)/lib -lblk -lcs -lct -lcomn -lintl -lsybdb -ltcl -linsck -lm -o $@
	echo
	echo "TAILLE DE L'EXECUTABLE :"$@
	@size $@
	@echo Executable $@ a jour. 

# 
# Regle liant les sources aux fichiers include communs et au makefile  
# 
$(SRCIE) : $(INCICG) $(INCIE) 
	touch $@

$(SRCROL) : $(INCICG) $(INCROL) 
	touch $@

# 
# Regle de generation d'une librairie  a partir 
# de ses fichiers objet.
# 
$(OBJIE): $(OBJIE)(dpac_ie.o)	\
	$(OBJIE)(dpac_bdc.o)	\
	$(OBJIE)(dpac_fctC.o)	\
	$(OBJIE)(dpac_lin.o)	\
	$(OBJIE)(dpac_pac.o)	\
	$(OBJIE)(dpac_util.o)	\
	$(OBJIE)(dpac_tra.o)	\
	$(OBJIE)(dpac_arbre.o)	\
	$(OBJIE)(dpac_app.o)	\
	$(OBJIE)(dpac_fax.o)	\
	$(OBJIE)(dpac_msg.o)	\
	$(OBJIE)(dpac_pmv.o)	\
	$(OBJIE)(dpac_nav.o)	\
	$(OBJIE)(dpac_tun.o)	\
	$(OBJIE)(dpac_ech.o) 	\
	$(OBJIE)(dpac_tdp.o)	\
	$(OBJIE)(dpac_caltdp.o)

$(OBJROL) : $(OBJROL)(drol_dir.o)	\
	$(OBJROL)(drol_tfm.o)	\
	$(OBJROL)(drol_fax.o)	\
	$(OBJROL)(drol_nav.o)	\
	$(OBJROL)(drol_tun.o)	\
	$(OBJROL)(drol_ech.o)	\
	$(OBJROL)(drol_pmv.o)	\
	$(OBJROL)(drol_pmv_util.o)	\
	$(OBJROL)(drol_pal.o)	\
	$(OBJROL)(drol_arbre.o)	\
	$(OBJROL)(drol_baf.o)	\
	$(OBJROL)(drol_bad.o)	\
	$(OBJROL)(drol_pmva_util.o)	\
	$(OBJROL)(drol_prv_util.o)	\
	$(OBJROL)(drol_pic_util.o)	\
	$(OBJROL)(drol_caltdp.o)	\
	$(OBJROL)(drol_tdp.o)	\
	$(OBJROL)(drol_pmva.o)	\
	$(OBJROL)(drol_prv.o)	\
	$(OBJROL)(drol_pic.o)	\
	$(OBJROL)(drol_appel.o)


########### Fin de zone a modifier par le developpeur ################
#
# phase de menage du makefile
#
clean:
	rm -f $(DIRLIB)/*.a
	rm -f $(EXEIE)
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
	echo $(CCROL)   $(CFLAGS) -c $<
	$(CCROL)   $(CFLAGS) -c $<

#
# Redefinition de la regles de construction d'un .a a partir d'un .o
#
.o.a:
	echo "CREATION DE LIBRAIRIE "$@.a
	ar rv $@.a $<
	rm -f $<

#
# Redefinition de la regles de construction d'un .a a partir d'un .c
#
.c.a : 
	echo " "
	if test $(OPTLINT) = yes;\
          then echo "PHASE DE VERIFICATION SYNTAXIQUE : "   $*.c;\
               $(LINT) $(LFLAGS)  $*.c;\
          else echo  "PAS DE VERIFICATION SYNTAXIQUE " $*.c;\
        fi;
	echo
	echo "PHASE DE COMPILATION             : "   $*.c
	echo " "
	$(CCROL)   $(CFLAGS) -c $*.c
	echo "MISE A JOUR DE LIBRAIRIE :"$@
	ar r $@ $*.o
	rm -f $*.o

#
# Redefinition de la regles de construction d'un .o a partir d'un .pc
#
.sc.o :
	echo "PHASE DE PRECOMPILATION C : "   $<
	cp $*.pc $*.c
	cc -E $(CFLAGS) $*.c > _$*.pc
	chmod +w $*.c
	rm $*.c
	echo
	echo "PHASE DE PRECOMPILATION INGRES : "   $<
	$(PCC) $(PCCFLAGS) _$*.sc 
	echo
	echo "PHASE DE COMPILATION             : "   _$*.c
	echo
	$(CC)   $(CFLAGS) -c _$*.c
	mv _$*.o $*.o
	rm _$*.c _$*.l* _$*.pc

#
# Redefinition de la regle de construction d'un .a a partir d'un .pc
#
.pc.a :
	echo "PHASE DE PRECOMPILATION C : "   $<
	cp $*.pc $*.c
	cc -E $(CFLAGS) $*.c > _$*.sc
	chmod +w $*.c
	rm $*.c
	echo
	echo "PHASE DE PRECOMPILATION INGRES : "   $<
	$(PCC) $(PCCFLAGS) _$*.sc 
	echo
	echo "PHASE DE COMPILATION             : "   _$*.c
	echo
	$(CC)   $(CFLAGS) -c _$*.c
	mv _$*.o $*.o
	rm _$*.c _$*.l* _$*.pc
	echo "MISE A JOUR DE LIBRAIRIE :"$@
	ar r $@ $*.o
	rm -f $*.o

