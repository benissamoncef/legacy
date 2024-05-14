#
#  FICHIER: @(#)Makefile	$Id: Makefile,v 1.35 2013/02/12 12:47:46 devgfi Exp $      Release : $Revision: 1.35 $        Date : $Date: 2013/02/12 12:47:46 $
#
###################################################################
#
# Inhibition de l'echo des commandes
#
.SILENT:
#
#
##################################################################
##
# definitions de variables du makefile
#

HOMEPROJECTDIR = $(HOME)/migrazur
DIRRTWORKS = $(RTHOME)/include
DIRSYBASE = $(SYBASE)/include
ICGINC = $(HOMEPROJECTDIR)/XDMICG/inc
CONFINC = $(HOMEPROJECTDIR)/GECONF/inc
ICGLIB = $(HOMEPROJECTDIR)/XDMICG/lib

#Compilateur
#CC = purify cc
#CC=cc
CC=$(RTHOME)/bin/rtlink

#Flags du compilateur

CFLAGS = -g -D_HPUX_SOURCE -Aa +e\
	-I$(DIRRTWORKS)\
	-I$(DIRSYBASE)\
	-I$(CONFINC)\
	-I$(ICGINC)
#
##################################################################
# Definition des programmes de tests
##################################################################

#TEST xzag 
#	connexion 
#	deconnexion base
#
TEST1SRC = connect_deconnect.c
TEST1OBJ = connect_deconnect.o 
TEST1LIB = xza.a
TEST1EXE = connect_deconnect.x 

LIB = $(ICGLIB)/xza.a \
	$(ICGLIB)/xzaoa.a \
	$(ICGLIB)/xzaol.a \
	$(ICGLIB)/xzaoc.a \
	$(ICGLIB)/xzs.a \
	$(ICGLIB)/xzi.a \
	$(ICGLIB)/xzc.a \
	$(ICGLIB)/xd.a

#TEST xzag xzao15 
#		connexion
#		appel a xzao15
#          	deconnexion base 
TEST2SRC = con_sp015_decon.c
TEST2OBJ = con_sp015_decon.o
TEST2EXE = con_sp015_decon.x

#TEST xzag xzap01
#               connexion
#               appel a xzap01
#               deconnexion base
TEST3SRC = con_p01_decon.c
TEST3OBJ = con_p01_decon.o
TEST3EXE = con_p01_decon.x

#TEST xzag xzap02
#               connexion
#               appel a xzap02
#               deconnexion base
TEST4SRC = con_p02_decon.c
TEST4OBJ = con_p02_decon.o
TEST4EXE = con_p02_decon.x

#TEST xzag xzap03
#               connexion
#               appel a xzap03
#               deconnexion base
TEST5SRC = con_p03_decon.c
TEST5OBJ = con_p03_decon.o
TEST5EXE = con_p03_decon.x

#TEST xzag xzap04
#               connexion
#               appel a xzap04
#               deconnexion base
TEST6SRC = con_p04_decon.c
TEST6OBJ = con_p04_decon.o
TEST6EXE = con_p04_decon.x

#TEST xzag xzap05
#               connexion
#               appel a xzap05
#               deconnexion base
TEST7SRC = con_p05_decon.c
TEST7OBJ = con_p05_decon.o
TEST7EXE = con_p05_decon.x

#TEST xzag xzap06
#               connexion
#               appel a xzap06
#               deconnexion base
TEST8SRC = con_p06_decon.c
TEST8OBJ = con_p06_decon.o
TEST8EXE = con_p06_decon.x

#TEST xzag xzap07 xzap10
#               connexion
#               appel a xzap07 xzap10
#               deconnexion base
TEST9SRC = con_p07_decon.c
TEST9OBJ = con_p07_decon.o
TEST9EXE = con_p07_decon.x

#TEST xzag xzap08
#               connexion
#               appel a xzap08
#               deconnexion base
TEST10SRC = con_p08_decon.c
TEST10OBJ = con_p08_decon.o
TEST10EXE = con_p08_decon.x

#TEST xzag xzap09
#               connexion
#               appel a xzap09
#               deconnexion base
TEST11SRC = con_p09_decon.c
TEST11OBJ = con_p09_decon.o
TEST11EXE = con_p09_decon.x

#TEST xzag xzap22
#               connexion
#               appel a xzap22
#               deconnexion base
TEST12SRC = con_p22_decon.c
TEST12OBJ = con_p22_decon.o
TEST12EXE = con_p22_decon.x

#TEST xzag xzao16
#               connexion
#               appel a xzao16
#               deconnexion base
TEST13SRC = con_sp016_decon.c
TEST13OBJ = con_sp016_decon.o
TEST13EXE = con_sp016_decon.x

#TEST xzag xzao17
#               connexion
#               appel a xzao17
#               deconnexion base
TEST14SRC = con_sp017_decon.c
TEST14OBJ = con_sp017_decon.o
TEST14EXE = con_sp017_decon.x

#TEST xzag xzao19
#               connexion
#               appel a xzao19
#               deconnexion base
TEST15SRC = con_sp019_decon.c
TEST15OBJ = con_sp019_decon.o
TEST15EXE = con_sp019_decon.x

#TEST xzag xzae23
#               connexion
#               appel a xzae23
#               deconnexion base
TEST16SRC = con_e23_decon.c
TEST16OBJ = con_e23_decon.o
TEST16EXE = con_e23_decon.x

#TEST xzag xzae24
#               connexion
#               appel a xzae24
#               deconnexion base
TEST17SRC = con_e24_decon.c
TEST17OBJ = con_e24_decon.o
TEST17EXE = con_e24_decon.x

#TEST xzag xzae26
#               connexion
#               appel a xzae26
#               deconnexion base
TEST18SRC = con_e26_decon.c
TEST18OBJ = con_e26_decon.o
TEST18EXE = con_e26_decon.x

#TEST xzag xzae27
#               connexion
#               appel a xzae27
#               deconnexion base
TEST19SRC = con_e27_decon.c
TEST19OBJ = con_e27_decon.o
TEST19EXE = con_e27_decon.x

#TEST xzag xzae29
#               connexion
#               appel a xzae29
#               deconnexion base
TEST20SRC = con_e29_decon.c
TEST20OBJ = con_e29_decon.o
TEST20EXE = con_e29_decon.x

#TEST xzag xzae30
#               connexion
#               appel a xzae30
#               deconnexion base
TEST21SRC = con_e30_decon.c
TEST21OBJ = con_e30_decon.o
TEST21EXE = con_e30_decon.x

#TEST xzag xzae31
#               connexion
#               appel a xzae31
#               deconnexion base
TEST22SRC = con_e31_decon.c
TEST22OBJ = con_e31_decon.o
TEST22EXE = con_e31_decon.x

#TEST xzag xzae32
#               connexion
#               appel a xzae32
#               deconnexion base
TEST23SRC = con_e32_decon.c
TEST23OBJ = con_e32_decon.o
TEST23EXE = con_e32_decon.x

#TEST xzag xzae20
#               connexion
#               appel a xzae20
#               deconnexion base
TEST24SRC = con_e20_decon.c
TEST24OBJ = con_e20_decon.o
TEST24EXE = con_e20_decon.x

#TEST xzag xzae21
#               connexion
#               appel a xzae21
#               deconnexion base
TEST25SRC = con_e21_decon.c
TEST25OBJ = con_e21_decon.o
TEST25EXE = con_e21_decon.x

#TEST xzag xzae22
#               connexion
#               appel a xzae22
#               deconnexion base
TEST26SRC = con_e22_decon.c
TEST26OBJ = con_e22_decon.o
TEST26EXE = con_e22_decon.x

#TEST xzag xzap10
#               connexion
#               appel a xzap10
#               deconnexion base
TEST27SRC = con_p10_decon.c
TEST27OBJ = con_p10_decon.o
TEST27EXE = con_p10_decon.x

#TEST xzat xzat22 xzat23 xzat24 xzat25
#               connexion au RtServeur
#               appel a xzat22 xzat23 xzat24 xzat25
#
TEST28SRC = xzat_test.c
TEST28OBJ = xzat_test.o
TEST28EXE = xzat_test.x

#TEST xzcg23 xzcg24 xzcg25
#               connexion au RtServeur
#               appel a xzcg23 xzcg24 xzcg25
#
TEST29SRC = xzcg_test.c
TEST29OBJ = xzcg_test.o
TEST29EXE = xzcg_test.x

#TEST xzaa01
#               connexion au RtServeur
#               appel a xzaa01
#
TEST30SRC = xzaa_test.c
TEST30OBJ = xzaa_test.o
TEST30EXE = xzaa_test.x

#TEST xzam01
#               connexion au RtServeur
#               appel a xzam01
#
TEST31SRC = xzam_test.c
TEST31OBJ = xzam_test.o
TEST31EXE = xzam_test.x

#TEST xzah11
#               connexion au RtServeur
#               appel a xzah01
#
TEST32SRC = xzah11_test.c
TEST32OBJ = xzah11_test.o
TEST32EXE = xzah11_test.x

#TEST xzah10
#               connexion au RtServeur
#               appel a xzah10 qui fait appel a xzah01 03 05 08 07
#
TEST33SRC = xzah10_test.c
TEST33OBJ = xzah10_test.o
TEST33EXE = xzah10_test.x

#TEST xzdd01 
#	connexion au noyau de supervision
#	appel a xzdd01
#
TDLIB = $(ICGLIB)/xza.a	\
	$(ICGLIB)/xzaoa.a \
	$(ICGLIB)/xzs.a	\
	$(ICGLIB)/xzd.a	\
	$(ICGLIB)/xd.a
TD1SRC = test_xzdd01.c 
TD1OBJ = test_xzdd01.o 
TD1EXE = test_xzdd01.x 

#TEST xzdd02 
#	connexion au noyau de supervision
#	appel a xzdd02
#
TD2SRC = test_xzdd02.c 
TD2OBJ = test_xzdd02.o 
TD2EXE = test_xzdd02.x 

#TEST xzac22 
#	connexion au noyau de supervision
#	appel a xzac22
#
TEST34SRC = xzac22tst.c 
TEST34OBJ = xzac22tst.o 
TEST34EXE = xzac22tst.x 

#TEST xzac23 
#	connexion au noyau de supervision
#	appel a xzac23
#
TEST35SRC = xzac23tst.c 
TEST35OBJ = xzac23tst.o 
TEST35EXE = xzac23tst.x 

#TEST xzac25 
#	connexion au noyau de supervision
#	appel a xzac25
#
TEST36SRC = xzac25tst.c 
TEST36OBJ = xzac25tst.o 
TEST36EXE = xzac25tst.x 

#TEST xzac26 
#	connexion au noyau de supervision
#	appel a xzac26
#
TEST37SRC = xzac26tst.c 
TEST37OBJ = xzac26tst.o 
TEST37EXE = xzac26tst.x 

#TEST xzag05
#               connexion a SQL Serveur
#               appel a xzag05 
#		deconnexion
TEST38SRC = xzag05tst.c
TEST38OBJ = xzag05tst.o
TEST38EXE = xzag05tst.x

#TEST xzat05
#               connexion a SQL Serveur
#               appel a xzat05 
#		deconnexion
TEST39SRC = xzat05tst.c
TEST39OBJ = xzat05tst.o
TEST39EXE = xzat05tst.x

#TEST xzat06
#               connexion a SQL Serveur
#               appel a xzat06 
#		deconnexion
TEST40SRC = xzat06tst.c
TEST40OBJ = xzat06tst.o
TEST40EXE = xzat06tst.x

#TEST xzat07
#               connexion a SQL Serveur
#               appel a xzat07
#		deconnexion
TEST41SRC = xzat07tst.c
TEST41OBJ = xzat07tst.o
TEST41EXE = xzat07tst.x

#TEST xzat08
#               connexion a SQL Serveur
#               appel a xzat08 
#		deconnexion
TEST42SRC = xzat08tst.c
TEST42OBJ = xzat08tst.o
TEST42EXE = xzat08tst.x

#TEST xzag06
#               connexion a SQL Serveur
#               appel a xzag06 
#		deconnexion
TEST43SRC = xzag06tst.c
TEST43OBJ = xzag06tst.o
TEST43EXE = xzag06tst.x

#TEST xzai07
#               connexion a SQL Serveur
#               appel a xzai07 
#		deconnexion
TEST44SRC = xzai07tst.c
TEST44OBJ = xzai07tst.o
TEST44EXE = xzai07tst.x

#TEST xzas10
#               connexion a SQL Serveur
#               appel a xzas10 
#		deconnexion
TEST45SRC = xzas10_test.c
TEST45OBJ = xzas10_test.o
TEST45EXE = xzas10_test.x

#TEST xzas11
#               connexion a SQL Serveur
#               appel a xzas11 
#		deconnexion
TEST46SRC = xzas11_test.c
TEST46OBJ = xzas11_test.o
TEST46EXE = xzas11_test.x

#TEST xzas12
#               connexion a SQL Serveur
#               appel a xzas12 
#		deconnexion
TEST47SRC = xzas12_test.c
TEST47OBJ = xzas12_test.o
TEST47EXE = xzas12_test.x

#TEST xzcg_testm
#               connexion a SQL Serveur
#               appel a tcgcd pour config machine 
#		deconnexion
TEST48SRC = xzcg_testm.c
TEST48OBJ = xzcg_testm.o
TEST48EXE = xzcg_testm.x

#TEST xzan01
#               connexion a SQL Serveur
#               appel a xzan01 
#		deconnexion
TEST49SRC = xzan01tst.c
TEST49OBJ = xzan01tst.o
TEST49EXE = xzan01tst.x

#TEST xzcg_testy
#               connexion a SQL Serveur
#               appel a tcgcd pour config type 
#		deconnexion
TEST50SRC = xzcg_testy.c
TEST50OBJ = xzcg_testy.o
TEST50EXE = xzcg_testy.x

#TEST xzcg_testeq
#               connexion a SQL Serveur
#               appel a tcgcd pour config equipement 
#		deconnexion
TEST51SRC = xzcg_testeq.c
TEST51OBJ = xzcg_testeq.o
TEST51EXE = xzcg_testeq.x

#TEST xzan37_test
#               connexion a SQL Serveur
#               appel a xzan37 sur le sql CI pour config equipement 
#		deconnexion
TEST52SRC = xzan37_test.c
TEST52OBJ = xzan37_test.o
TEST52EXE = xzan37_test.x

#TEST xzag08_test
#               appel a xzag08
TEST53SRC = xzag08_test.c
TEST53OBJ = xzag08_test.o
TEST53EXE = xzag08_test.x

#TEST xzag09_test
#               appel a xzag09
TEST54SRC = xzag09_test.c
TEST54OBJ = xzag09_test.o
TEST54EXE = xzag09_test.x

#TEST xzca_test
#               appel a xzag09
TEST55SRC = xzca_test.c
TEST55OBJ = xzca_test.o
TEST55EXE = xzca_test.x

#TEST xzaf10_test
#               appel a xzaf10
TEST56SRC = xzaf10_test.c
TEST56OBJ = xzaf10_test.o
TEST56EXE = xzaf10_test.x

#TEST xzaf02_test
#               appel a xzaf02
TEST57SRC = xzaf02_test.c
TEST57OBJ = xzaf02_test.o
TEST57EXE = xzaf02_test.x

#TEST xzaf03_test
#               appel a xzaf03
TEST58SRC = xzaf03_test.c
TEST58OBJ = xzaf03_test.o
TEST58EXE = xzaf03_test.x

#TEST xzaf05_test
#               appel a xzaf05
TEST59SRC = xzaf05_test.c
TEST59OBJ = xzaf05_test.o
TEST59EXE = xzaf05_test.x

#TEST xzaf06_test
#               appel a xzaf06
TEST60SRC = xzaf06_test.c
TEST60OBJ = xzaf06_test.o
TEST60EXE = xzaf06_test.x

#TEST xzaf08_test
#               appel a xzaf08
TEST61SRC = xzaf08_test.c
TEST61OBJ = xzaf08_test.o
TEST61EXE = xzaf08_test.x

#TEST xzaf11_test
#               appel a xzaf11
TEST62SRC = xzaf11_test.c
TEST62OBJ = xzaf11_test.o
TEST62EXE = xzaf11_test.x

#TEST xzaf14_test
#               appel a xzaf14
TEST63SRC = xzaf14_test.c
TEST63OBJ = xzaf14_test.o
TEST63EXE = xzaf14_test.x

#TEST xzaf15_test
#               appel a xzaf15
TEST64SRC = xzaf15_test.c
TEST64OBJ = xzaf15_test.o
TEST64EXE = xzaf15_test.x

#TEST xzaz01_test
#               appel a xzaz01
TEST65SRC = xzaz01_test.c
TEST65OBJ = xzaz01_test.o
TEST65EXE = xzaz01_test.x

#TEST conn_ascl06_decon.x
#               appel a ascl06
TEST66SRC = conn_ascl06_decon.c
TEST66OBJ = conn_ascl06_decon.o
TEST66EXE = conn_ascl06_decon.x

#TEST xzaf16_test
#               appel a xzaf16
TEST67SRC = xzaf16_test.c
TEST67OBJ = xzaf16_test.o
TEST67EXE = xzaf16_test.x

#TEST xzao
#               connexion a SQL Serveur
#               appel a xzao25, xzao26, xzao27, xzao28
#		deconnexion
TEST68SRC = ex_xzao.c
TEST68OBJ = ex_xzao.o
TEST68EXE = ex_xzao.x


all : $(TEST1EXE) $(TEST2EXE) $(TEST3EXE) $(TEST4EXE) $(TEST5EXE) $(TEST6EXE) $(TEST7EXE) $(TEST8EXE) $(TEST9EXE) $(TEST10EXE) $(TEST11EXE) $(TEST12EXE) $(TEST13EXE) $(TEST14EXE) $(TEST15EXE) $(TEST16EXE) $(TEST17EXE) $(TEST18EXE) $(TEST19EXE) $(TEST20EXE) $(TEST21EXE) $(TEST22EXE) $(TEST23EXE) $(TEST24EXE) $(TEST25EXE) $(TEST26EXE) $(TEST27EXE) $(TEST28EXE) $(TEST30EXE) $(TEST31EXE) $(TEST32EXE) $(TEST33EXE) $(TEST34EXE) $(TEST35EXE) $(TEST36EXE) $(TEST37EXE) $(TEST38EXE) $(TEST39EXE) $(TEST40EXE) $(TEST41EXE) $(TEST42EXE) $(TEST43EXE) $(TEST44EXE) $(TEST45EXE) $(TEST46EXE) $(TEST47EXE) $(TEST48EXE) $(TEST49EXE) $(TEST50EXE) $(TEST51EXE) $(TEST52EXE) $(TEST53EXE) $(TEST54EXE) $(TEST55EXE) $(TEST56EXE) $(TEST57EXE) $(TEST58EXE) $(TEST59EXE) $(TEST60EXE) $(TEST61EXE) $(TEST62EXE) $(TEST63EXE) $(TEST64EXE) $(TEST65EXE) $(TEST66EXE) $(TEST67EXE) $(TEST68EXE)

$(TEST1EXE) : $(TEST1SRC) $(LIB) $(TEST1OBJ)
	echo "PHASE DE COMPILATION             : " $(TEST1SRC) 
	$(CC) $(CFLAGS) $(TEST1OBJ) $(LIB) -L$(RTHOME)/lib/hp700_hpux_9_0 -lrtutil -lrtipc -L$(SYBASE)/lib -lblk -lcs -lct -lcomn -lintl -lsybdb -ltcl -linsck -lm  -o $@
	echo "EXECUTABLE DE TEST PRET          : " $(TEST1EXE)
	echo

$(TEST2EXE) : $(TEST2SRC) $(LIB) $(TEST2OBJ)
	echo "PHASE DE COMPILATION             : " $(TEST2SRC)
	$(CC) $(CFLAGS) $(TEST2OBJ) $(LIB) -L$(RTHOME)/lib/hp700_hpux_9_0 -lrtutil -lrtipc -lrtserver -L$(SYBASE)/lib -lblk -lcs -lct -lcomn -lintl -lsybdb -ltcl -linsck -lm  -o $@
	echo "EXECUTABLE DE TEST PRET          : " $(TEST2EXE)
	echo


$(TEST3EXE) : $(TEST3SRC) $(LIB) $(TEST3OBJ)
	echo "PHASE DE COMPILATION             : " $(TEST3SRC)
	$(CC) $(CFLAGS) $(TEST3OBJ) $(LIB) -L$(RTHOME)/lib/hp700_hpux_9_0 -lrtutil -lrtipc -lrtserver -L$(SYBASE)/lib -lblk -lcs -lct -lcomn -lintl -lsybdb -ltcl -linsck -lm  -o $@
	echo "EXECUTABLE DE TEST PRET          : " $(TEST3EXE)
	echo

$(TEST4EXE) : $(TEST4SRC) $(LIB) $(TEST4OBJ)
	echo "PHASE DE COMPILATION             : " $(TEST4SRC)
	$(CC) $(CFLAGS) $(TEST4OBJ) $(LIB) -L$(RTHOME)/lib/hp700_hpux_9_0 -lrtutil -lrtipc -lrtserver -L$(SYBASE)/lib -lblk -lcs -lct -lcomn -lintl -lsybdb -ltcl -linsck -lm  -o $@
	echo "EXECUTABLE DE TEST PRET          : " $(TEST4EXE)
	echo

$(TEST5EXE) : $(TEST5SRC) $(LIB) $(TEST5OBJ)
	echo "PHASE DE COMPILATION             : " $(TEST5SRC)
	$(CC) $(CFLAGS) $(TEST5OBJ) $(LIB) -L$(RTHOME)/lib/hp700_hpux_9_0 -lrtutil -lrtipc -lrtserver -L$(SYBASE)/lib -lblk -lcs -lct -lcomn -lintl -lsybdb -ltcl -linsck -lm  -o $@
	echo "EXECUTABLE DE TEST PRET          : " $(TEST5EXE)
	echo

$(TEST6EXE) : $(TEST6SRC) $(LIB) $(TEST6OBJ)
	echo "PHASE DE COMPILATION             : " $(TEST6SRC)
	$(CC) $(CFLAGS) $(TEST6OBJ) $(LIB) -L$(RTHOME)/lib/hp700_hpux_9_0 -lrtutil -lrtipc -lrtserver -L$(SYBASE)/lib -lblk -lcs -lct -lcomn -lintl -lsybdb -ltcl -linsck -lm  -o $@
	echo "EXECUTABLE DE TEST PRET          : " $(TEST6EXE)
	echo

$(TEST7EXE) : $(TEST7SRC) $(LIB) $(TEST7OBJ)
	echo "PHASE DE COMPILATION             : " $(TEST7SRC)
	$(CC) $(CFLAGS) $(TEST7OBJ) $(LIB) -L$(RTHOME)/lib/hp700_hpux_9_0 -lrtutil -lrtipc -lrtserver -L$(SYBASE)/lib -lblk -lcs -lct -lcomn -lintl -lsybdb -ltcl -linsck -lm  -o $@
	echo "EXECUTABLE DE TEST PRET          : " $(TEST7EXE)
	echo

$(TEST8EXE) : $(TEST8SRC) $(LIB) $(TEST8OBJ)
	echo "PHASE DE COMPILATION             : " $(TEST8SRC)
	$(CC) $(CFLAGS) $(TEST8OBJ) $(LIB) -L$(RTHOME)/lib/hp700_hpux_9_0 -lrtutil -lrtipc -lrtserver -L$(SYBASE)/lib -lblk -lcs -lct -lcomn -lintl -lsybdb -ltcl -linsck -lm  -o $@
	echo "EXECUTABLE DE TEST PRET          : " $(TEST8EXE)
	echo

$(TEST9EXE) : $(TEST9SRC) $(LIB) $(TEST9OBJ)
	echo "PHASE DE COMPILATION             : " $(TEST9SRC)
	$(CC) $(CFLAGS) $(TEST9OBJ) $(LIB) -L$(RTHOME)/lib/hp700_hpux_9_0 -lrtutil -lrtipc -lrtserver -L$(SYBASE)/lib -lblk -lcs -lct -lcomn -lintl -lsybdb -ltcl -linsck -lm  -o $@
	echo "EXECUTABLE DE TEST PRET          : " $(TEST9EXE)
	echo

$(TEST10EXE) : $(TEST10SRC) $(LIB) $(TEST10OBJ)
	echo "PHASE DE COMPILATION             : " $(TEST10SRC)
	$(CC) $(CFLAGS) $(TEST10OBJ) $(LIB) -L$(RTHOME)/lib/hp700_hpux_9_0 -lrtutil -lrtipc -lrtserver -L$(SYBASE)/lib -lblk -lcs -lct -lcomn -lintl -lsybdb -ltcl -linsck -lm  -o $@
	echo "EXECUTABLE DE TEST PRET : " $(TEST10EXE)
	echo

$(TEST11EXE) : $(TEST11SRC) $(LIB) $(TEST11OBJ)
	echo "PHASE DE COMPILATION             : " $(TEST11SRC)
	$(CC) $(CFLAGS) $(TEST11OBJ) $(LIB) -L$(RTHOME)/lib/hp700_hpux_9_0 -lrtutil -lrtipc -lrtserver -L$(SYBASE)/lib -lblk -lcs -lct -lcomn -lintl -lsybdb -ltcl -linsck -lm  -o $@
	echo "EXECUTABLE DE TEST PRET          : " $(TEST11EXE)
	echo

$(TEST12EXE) : $(TEST12SRC) $(LIB) $(TEST12OBJ)
	echo "PHASE DE COMPILATION             : " $(TEST12SRC)
	$(CC) $(CFLAGS) $(TEST12OBJ) $(LIB) -L$(RTHOME)/lib/hp700_hpux_9_0 -lrtutil -lrtipc -lrtserver -L$(SYBASE)/lib -lblk -lcs -lct -lcomn -lintl -lsybdb -ltcl -linsck -lm  -o $@
	echo "EXECUTABLE DE TEST PRET          : " $(TEST12EXE)
	echo

$(TEST13EXE) : $(TEST13SRC) $(LIB) $(TEST13OBJ)
	echo "PHASE DE COMPILATION             : " $(TEST13SRC)
	$(CC) $(CFLAGS) $(TEST13OBJ) $(LIB) -L$(RTHOME)/lib/hp700_hpux_9_0 -lrtutil -lrtipc -lrtserver -L$(SYBASE)/lib -lblk -lcs -lct -lcomn -lintl -lsybdb -ltcl -linsck -lm  -o $@
	echo "EXECUTABLE DE TEST PRET          : " $(TEST13EXE)
	echo


$(TEST14EXE) : $(TEST14SRC) $(LIB) $(TEST14OBJ)
	echo "PHASE DE COMPILATION             : " $(TEST14SRC)
	$(CC) $(CFLAGS) $(TEST14OBJ) $(LIB) -L$(RTHOME)/lib/hp700_hpux_9_0 -lrtutil -lrtipc -lrtserver -L$(SYBASE)/lib -lblk -lcs -lct -lcomn -lintl -lsybdb -ltcl -linsck -lm  -o $@
	echo "EXECUTABLE DE TEST PRET          : " $(TEST14EXE)
	echo

$(TEST15EXE) : $(TEST15SRC) $(LIB) $(TEST15OBJ)
	echo "PHASE DE COMPILATION             : " $(TEST15SRC)
	$(CC) $(CFLAGS) $(TEST15OBJ) $(LIB) -L$(RTHOME)/lib/hp700_hpux_9_0 -lrtutil -lrtipc -lrtserver -L$(SYBASE)/lib -lblk -lcs -lct -lcomn -lintl -lsybdb -ltcl -linsck -lm  -o $@
	echo "EXECUTABLE DE TEST PRET          : " $(TEST15EXE)
	echo

$(TEST16EXE) : $(TEST16SRC) $(LIB) $(TEST16OBJ)
	echo "PHASE DE COMPILATION             : " $(TEST16SRC)
	$(CC) $(CFLAGS) $(TEST16OBJ) $(LIB) -L$(RTHOME)/lib/hp700_hpux_9_0 -lrtutil -lrtipc -lrtserver -L$(SYBASE)/lib -lblk -lcs -lct -lcomn -lintl -lsybdb -ltcl -linsck -lm  -o $@
	echo "EXECUTABLE DE TEST PRET          : " $(TEST16EXE)
	echo
	
$(TEST17EXE) : $(TEST17SRC) $(LIB) $(TEST17OBJ)
	echo "PHASE DE COMPILATION             : " $(TEST17SRC)
	$(CC) $(CFLAGS) $(TEST17OBJ) $(LIB) -L$(RTHOME)/lib/hp700_hpux_9_0 -lrtutil -lrtipc -lrtserver -L$(SYBASE)/lib -lblk -lcs -lct -lcomn -lintl -lsybdb -ltcl -linsck -lm  -o $@
	echo "EXECUTABLE DE TEST PRET          : " $(TEST17EXE)
	echo

$(TEST18EXE) : $(TEST18SRC) $(LIB) $(TEST18OBJ)
	echo "PHASE DE COMPILATION             : " $(TEST18SRC)
	$(CC) $(CFLAGS) $(TEST18OBJ) $(LIB) -L$(RTHOME)/lib/hp700_hpux_9_0 -lrtutil -lrtipc -lrtserver -L$(SYBASE)/lib -lblk -lcs -lct -lcomn -lintl -lsybdb -ltcl -linsck -lm  -o $@
	echo "EXECUTABLE DE TEST PRET          : " $(TEST18EXE)
	echo

$(TEST19EXE) : $(TEST19SRC) $(LIB) $(TEST19OBJ)
	echo "PHASE DE COMPILATION             : " $(TEST19SRC)
	$(CC) $(CFLAGS) $(TEST19OBJ) $(LIB) -L$(RTHOME)/lib/hp700_hpux_9_0 -lrtutil -lrtipc -lrtserver -L$(SYBASE)/lib -lblk -lcs -lct -lcomn -lintl -lsybdb -ltcl -linsck -lm  -o $@
	echo "EXECUTABLE DE TEST PRET          : " $(TEST19EXE)
	echo

$(TEST20EXE) : $(TEST20SRC) $(LIB) $(TEST20OBJ)
	echo "PHASE DE COMPILATION             : " $(TEST20SRC)
	$(CC) $(CFLAGS) $(TEST20OBJ) $(LIB) -L$(RTHOME)/lib/hp700_hpux_9_0 -lrtutil -lrtipc -lrtserver -L$(SYBASE)/lib -lblk -lcs -lct -lcomn -lintl -lsybdb -ltcl -linsck -lm  -o $@
	echo "EXECUTABLE DE TEST PRET          : " $(TEST20EXE)
	echo

$(TEST21EXE) : $(TEST21SRC) $(LIB) $(TEST21OBJ)
	echo "PHASE DE COMPILATION             : " $(TEST21SRC)
	$(CC) $(CFLAGS) $(TEST21OBJ) $(LIB) -L$(RTHOME)/lib/hp700_hpux_9_0 -lrtutil -lrtipc -lrtserver -L$(SYBASE)/lib -lblk -lcs -lct -lcomn -lintl -lsybdb -ltcl -linsck -lm  -o $@
	echo "EXECUTABLE DE TEST PRET          : " $(TEST21EXE)
	echo

$(TEST22EXE) : $(TEST22SRC) $(LIB) $(TEST22OBJ)
	echo "PHASE DE COMPILATION             : " $(TEST22SRC)
	$(CC) $(CFLAGS) $(TEST22OBJ) $(LIB) -L$(RTHOME)/lib/hp700_hpux_9_0 -lrtutil -lrtipc -lrtserver -L$(SYBASE)/lib -lblk -lcs -lct -lcomn -lintl -lsybdb -ltcl -linsck -lm  -o $@
	echo "EXECUTABLE DE TEST PRET          : " $(TEST22EXE)
	echo

$(TEST23EXE) : $(TEST23SRC) $(LIB) $(TEST23OBJ)
	echo "PHASE DE COMPILATION             : " $(TEST23SRC)
	$(CC) $(CFLAGS) $(TEST23OBJ) $(LIB) -L$(RTHOME)/lib/hp700_hpux_9_0 -lrtutil -lrtipc -lrtserver -L$(SYBASE)/lib -lblk -lcs -lct -lcomn -lintl -lsybdb -ltcl -linsck -lm  -o $@
	echo "EXECUTABLE DE TEST PRET          : " $(TEST23EXE)
	echo

$(TEST24EXE) : $(TEST24SRC) $(LIB) $(TEST24OBJ)
	echo "PHASE DE COMPILATION             : " $(TEST24SRC)
	$(CC) $(CFLAGS) $(TEST24OBJ) $(LIB) -L$(RTHOME)/lib/hp700_hpux_9_0 -lrtutil -lrtipc -lrtserver -L$(SYBASE)/lib -lblk -lcs -lct -lcomn -lintl -lsybdb -ltcl -linsck -lm  -o $@
	echo "EXECUTABLE DE TEST PRET          : " $(TEST24EXE)
	echo

$(TEST25EXE) : $(TEST25SRC) $(LIB) $(TEST25OBJ)
	echo "PHASE DE COMPILATION             : " $(TEST25SRC)
	$(CC) $(CFLAGS) $(TEST25OBJ) $(LIB) -L$(RTHOME)/lib/hp700_hpux_9_0 -lrtutil -lrtipc -lrtserver -L$(SYBASE)/lib -lblk -lcs -lct -lcomn -lintl -lsybdb -ltcl -linsck -lm  -o $@
	echo "EXECUTABLE DE TEST PRET          : " $(TEST25EXE)
	echo

$(TEST26EXE) : $(TEST26SRC) $(LIB) $(TEST26OBJ)
	echo "PHASE DE COMPILATION             : " $(TEST26SRC)
	$(CC) $(CFLAGS) $(TEST26OBJ) $(LIB) -L$(RTHOME)/lib/hp700_hpux_9_0 -lrtutil -lrtipc -lrtserver -L$(SYBASE)/lib -lblk -lcs -lct -lcomn -lintl -lsybdb -ltcl -linsck -lm  -o $@
	echo "EXECUTABLE DE TEST PRET          : " $(TEST26EXE)
	echo

$(TEST27EXE) : $(TEST27SRC) $(LIB) $(TEST27OBJ)
	echo "PHASE DE COMPILATION             : " $(TEST27SRC)
	$(CC) $(CFLAGS) $(TEST27OBJ) $(LIB) -L$(RTHOME)/lib/hp700_hpux_9_0 -lrtutil -lrtipc -lrtserver -L$(SYBASE)/lib -lblk -lcs -lct -lcomn -lintl -lsybdb -ltcl -linsck -lm  -o $@
	echo "EXECUTABLE DE TEST PRET          : " $(TEST27EXE)
	echo


$(TEST28EXE) : $(TEST28SRC) $(LIB) $(TEST28OBJ)
	echo "PHASE DE COMPILATION             : " $(TEST28SRC)
	$(CC) $(CFLAGS) $(TEST28OBJ) $(LIB) -L$(RTHOME)/lib/hp700_hpux_9_0 -lrtutil -lrtipc -lrtserver -L$(SYBASE)/lib -lblk -lcs -lct -lcomn -lintl -lsybdb -ltcl -linsck -lm  -o $@
	echo "EXECUTABLE DE TEST PRET          : " $(TEST28EXE)
	echo
	
$(TEST29EXE) : $(TEST29SRC) $(LIB) $(TEST29OBJ)
	echo "PHASE DE COMPILATION             : " $(TEST29SRC)
	$(CC) $(CFLAGS) $(TEST29OBJ) $(LIB) -L$(RTHOME)/lib/hp700_hpux_9_0 -lrtutil -lrtipc -lrtserver -L$(SYBASE)/lib -lblk -lcs -lct -lcomn -lintl -lsybdb -ltcl -linsck -lm  -o $@
	echo "EXECUTABLE DE TEST PRET          : " $(TEST29EXE)
	echo
	
$(TEST30EXE) : $(TEST30SRC) $(LIB) $(TEST30OBJ)
	echo "PHASE DE COMPILATION             : " $(TEST30SRC)
	$(CC) $(CFLAGS) $(TEST30OBJ) $(LIB) -L$(RTHOME)/lib/hp700_hpux_9_0 -lrtutil -lrtipc -lrtserver -L$(SYBASE)/lib -lblk -lcs -lct -lcomn -lintl -lsybdb -ltcl -linsck -lm  -o $@
	echo "EXECUTABLE DE TEST PRET          : " $(TEST30EXE)
	echo

$(TEST31EXE) : $(TEST31SRC) $(LIB) $(TEST31OBJ)
	echo "PHASE DE COMPILATION             : " $(TEST31SRC)
	$(CC) $(CFLAGS) $(TEST31OBJ) $(LIB) -L$(RTHOME)/lib/hp700_hpux_9_0 -lrtutil -lrtipc -lrtserver -L$(SYBASE)/lib -lblk -lcs -lct -lcomn -lintl -lsybdb -ltcl -linsck -lm  -o $@
	echo "EXECUTABLE DE TEST PRET          : " $(TEST31EXE)
	echo	

$(TEST32EXE) : $(TEST32SRC) $(LIB) $(TEST32OBJ)
	echo "PHASE DE COMPILATION             : " $(TEST32SRC)
	$(CC) $(CFLAGS) $(TEST32OBJ) $(LIB) -L$(RTHOME)/lib/hp700_hpux_9_0 -lrtutil -lrtipc -lrtserver -L$(SYBASE)/lib -lblk -lcs -lct -lcomn -lintl -lsybdb -ltcl -linsck -lm  -o $@
	echo "EXECUTABLE DE TEST PRET          : " $(TEST32EXE)
	echo	

$(TEST33EXE) : $(TEST33SRC) $(LIB) $(TEST33OBJ)
	echo "PHASE DE COMPILATION             : " $(TEST33SRC)
	$(CC) $(CFLAGS) $(TEST33OBJ) $(LIB) -L$(RTHOME)/lib/hp700_hpux_9_0 -lrtutil -lrtipc -lrtserver -L$(SYBASE)/lib -lblk -lcs -lct -lcomn -lintl -lsybdb -ltcl -linsck -lm  -o $@
	echo "EXECUTABLE DE TEST PRET          : " $(TEST33EXE)
	echo	

$(TD1EXE) : $(TD1SRC) $(TDLIB) $(TD1OBJ)
	echo "PHASE DE COMPILATION             : " $(TD1SRC) 
	$(CC) $(CFLAGS) $(TD1OBJ) $(TDLIB) -L$(RTHOME)/lib/hp700_hpux_9_0 -lrtutil -lrtipc -o $@
	echo "EXECUTABLE DE TEST PRET          : " $(TD1EXE)
	echo

$(TD2EXE) : $(TD2SRC) $(TDLIB) $(TD2OBJ)
	echo "PHASE DE COMPILATION             : " $(TD2SRC) 
	$(CC) $(CFLAGS) $(TD2OBJ) $(TDLIB) -L$(RTHOME)/lib/hp700_hpux_9_0 -lrtutil -lrtipc -o $@
	echo "EXECUTABLE DE TEST PRET          : " $(TD2EXE)
	echo

$(TEST34EXE) : $(TEST34SRC) $(LIB) $(TEST34OBJ)
	echo "PHASE DE COMPILATION             : " $(TEST34SRC)
	$(CC) $(CFLAGS) $(TEST34OBJ) $(LIB) -L$(RTHOME)/lib/hp700_hpux_9_0 -lrtutil -lrtipc -lrtserver -L$(SYBASE)/lib -lblk -lcs -lct -lcomn -lintl -lsybdb -ltcl -linsck -lm  -o $@
	echo "EXECUTABLE DE TEST PRET          : " $(TEST34EXE)
	echo	

$(TEST35EXE) : $(TEST35SRC) $(LIB) $(TEST35OBJ)
	echo "PHASE DE COMPILATION             : " $(TEST35SRC)
	$(CC) $(CFLAGS) $(TEST35OBJ) $(LIB) -L$(RTHOME)/lib/hp700_hpux_9_0 -lrtutil -lrtipc -lrtserver -L$(SYBASE)/lib -lblk -lcs -lct -lcomn -lintl -lsybdb -ltcl -linsck -lm  -o $@
	echo "EXECUTABLE DE TEST PRET          : " $(TEST35EXE)
	echo	

$(TEST36EXE) : $(TEST34SRC) $(LIB) $(TEST36OBJ)
	echo "PHASE DE COMPILATION             : " $(TEST36SRC)
	$(CC) $(CFLAGS) $(TEST36OBJ) $(LIB) -L$(RTHOME)/lib/hp700_hpux_9_0 -lrtutil -lrtipc -lrtserver -L$(SYBASE)/lib -lblk -lcs -lct -lcomn -lintl -lsybdb -ltcl -linsck -lm  -o $@
	echo "EXECUTABLE DE TEST PRET          : " $(TEST36EXE)
	echo	

$(TEST37EXE) : $(TEST37SRC) $(LIB) $(TEST37OBJ)
	echo "PHASE DE COMPILATION             : " $(TEST37SRC)
	$(CC) $(CFLAGS) $(TEST37OBJ) $(LIB) -L$(RTHOME)/lib/hp700_hpux_9_0 -lrtutil -lrtipc -lrtserver -L$(SYBASE)/lib -lblk -lcs -lct -lcomn -lintl -lsybdb -ltcl -linsck -lm  -o $@
	echo "EXECUTABLE DE TEST PRET          : " $(TEST37EXE)
	echo	

$(TEST38EXE) : $(TEST38SRC) $(LIB) $(TEST38OBJ)
	echo "PHASE DE COMPILATION             : " $(TEST38SRC)
	$(CC) $(CFLAGS) $(TEST38OBJ) $(LIB) -L$(RTHOME)/lib/hp700_hpux_9_0 -lrtutil -lrtipc -lrtserver -L$(SYBASE)/lib -lblk -lcs -lct -lcomn -lintl -lsybdb -ltcl -linsck -lm  -o $@
	echo "EXECUTABLE DE TEST PRET          : " $(TEST38EXE)
	echo	

$(TEST39EXE) : $(TEST39SRC) $(LIB) $(TEST39OBJ)
	echo "PHASE DE COMPILATION             : " $(TEST39SRC)
	$(CC) $(CFLAGS) $(TEST39OBJ) $(LIB) -L$(RTHOME)/lib/hp700_hpux_9_0 -lrtutil -lrtipc -lrtserver -L$(SYBASE)/lib -lblk -lcs -lct -lcomn -lintl -lsybdb -ltcl -linsck -lm  -o $@
	echo "EXECUTABLE DE TEST PRET          : " $(TEST39EXE)
	echo	

$(TEST40EXE) : $(TEST40SRC) $(LIB) $(TEST40OBJ)
	echo "PHASE DE COMPILATION             : " $(TEST40SRC)
	$(CC) $(CFLAGS) $(TEST40OBJ) $(LIB) -L$(RTHOME)/lib/hp700_hpux_9_0 -lrtutil -lrtipc -lrtserver -L$(SYBASE)/lib -lblk -lcs -lct -lcomn -lintl -lsybdb -ltcl -linsck -lm  -o $@
	echo "EXECUTABLE DE TEST PRET          : " $(TEST40EXE)
	echo	

$(TEST41EXE) : $(TEST41SRC) $(LIB) $(TEST41OBJ)
	echo "PHASE DE COMPILATION             : " $(TEST41SRC)
	$(CC) $(CFLAGS) $(TEST41OBJ) $(LIB) -L$(RTHOME)/lib/hp700_hpux_9_0 -lrtutil -lrtipc -lrtserver -L$(SYBASE)/lib -lblk -lcs -lct -lcomn -lintl -lsybdb -ltcl -linsck -lm  -o $@
	echo "EXECUTABLE DE TEST PRET          : " $(TEST41EXE)
	echo	
$(TEST42EXE) : $(TEST42SRC) $(LIB) $(TEST42OBJ)
	echo "PHASE DE COMPILATION             : " $(TEST42SRC)
	$(CC) $(CFLAGS) $(TEST42OBJ) $(LIB) -L$(RTHOME)/lib/hp700_hpux_9_0 -lrtutil -lrtipc -lrtserver -L$(SYBASE)/lib -lblk -lcs -lct -lcomn -lintl -lsybdb -ltcl -linsck -lm  -o $@
	echo "EXECUTABLE DE TEST PRET          : " $(TEST42EXE)
	echo	

$(TEST43EXE) : $(TEST43SRC) $(LIB) $(TEST43OBJ)
	echo "PHASE DE COMPILATION             : " $(TEST43SRC)
	$(CC) $(CFLAGS) $(TEST43OBJ) $(LIB) -L$(RTHOME)/lib/hp700_hpux_9_0 -lrtutil -lrtipc -lrtserver -L$(SYBASE)/lib -lblk -lcs -lct -lcomn -lintl -lsybdb -ltcl -linsck -lm  -o $@
	echo "EXECUTABLE DE TEST PRET          : " $(TEST43EXE)
	echo	

$(TEST44EXE) : $(TEST44SRC) $(LIB) $(TEST44OBJ)
	echo "PHASE DE COMPILATION             : " $(TEST44SRC)
	$(CC) $(CFLAGS) $(TEST44OBJ) $(LIB) -L$(RTHOME)/lib/hp700_hpux_9_0 -lrtutil -lrtipc -lrtserver -L$(SYBASE)/lib -lblk -lcs -lct -lcomn -lintl -lsybdb -ltcl -linsck -lm  -o $@
	echo "EXECUTABLE DE TEST PRET          : " $(TEST44EXE)
	echo	

$(TEST45EXE) : $(TEST45SRC) $(LIB) $(TEST45OBJ)
	echo "PHASE DE COMPILATION             : " $(TEST45SRC)
	$(CC) $(CFLAGS) $(TEST45OBJ) $(LIB) -L$(RTHOME)/lib/hp700_hpux_9_0 -lrtutil -lrtipc -lrtserver -L$(SYBASE)/lib -lblk -lcs -lct -lcomn -lintl -lsybdb -ltcl -linsck -lm  -o $@
	echo "EXECUTABLE DE TEST PRET          : " $(TEST45EXE)
	echo	

$(TEST46EXE) : $(TEST45SRC) $(LIB) $(TEST46OBJ)
	echo "PHASE DE COMPILATION             : " $(TEST46SRC)
	$(CC) $(CFLAGS) $(TEST46OBJ) $(LIB) -L$(RTHOME)/lib/hp700_hpux_9_0 -lrtutil -lrtipc -lrtserver -L$(SYBASE)/lib -lblk -lcs -lct -lcomn -lintl -lsybdb -ltcl -linsck -lm  -o $@
	echo "EXECUTABLE DE TEST PRET          : " $(TEST46EXE)
	echo	

$(TEST47EXE) : $(TEST47SRC) $(LIB) $(TEST47OBJ)
	echo "PHASE DE COMPILATION             : " $(TEST47SRC)
	$(CC) $(CFLAGS) $(TEST47OBJ) $(LIB) -L$(RTHOME)/lib/hp700_hpux_9_0 -lrtutil -lrtipc -lrtserver -L$(SYBASE)/lib -lblk -lcs -lct -lcomn -lintl -lsybdb -ltcl -linsck -lm  -o $@
	echo "EXECUTABLE DE TEST PRET          : " $(TEST47EXE)
	echo	

$(TEST48EXE) : $(TEST48SRC) $(LIB) $(TEST48OBJ)
	echo "PHASE DE COMPILATION             : " $(TEST48SRC)
	$(CC) $(CFLAGS) $(TEST48OBJ) $(LIB) -L$(RTHOME)/lib/hp700_hpux_9_0 -lrtutil -lrtipc -lrtserver -L$(SYBASE)/lib -lblk -lcs -lct -lcomn -lintl -lsybdb -ltcl -linsck -lm  -o $@
	echo "EXECUTABLE DE TEST PRET          : " $(TEST48EXE)
	echo	

$(TEST49EXE) : $(TEST49SRC) $(LIB) $(TEST49OBJ)
	echo "PHASE DE COMPILATION             : " $(TEST49SRC)
	$(CC) $(CFLAGS) $(TEST49OBJ) $(LIB) -L$(RTHOME)/lib/hp700_hpux_9_0 -lrtutil -lrtipc -lrtserver -L$(SYBASE)/lib -lblk -lcs -lct -lcomn -lintl -lsybdb -ltcl -linsck -lm  -o $@
	echo "EXECUTABLE DE TEST PRET          : " $(TEST49EXE)
	echo	

$(TEST50EXE) : $(TEST50SRC) $(LIB) $(TEST50OBJ)
	echo "PHASE DE COMPILATION             : " $(TEST50SRC)
	$(CC) $(CFLAGS) $(TEST50OBJ) $(LIB) -L$(RTHOME)/lib/hp700_hpux_9_0 -lrtutil -lrtipc -lrtserver -L$(SYBASE)/lib -lblk -lcs -lct -lcomn -lintl -lsybdb -ltcl -linsck -lm  -o $@
	echo "EXECUTABLE DE TEST PRET          : " $(TEST50EXE)
	echo	

$(TEST51EXE) : $(TEST51SRC) $(LIB) $(TEST51OBJ)
	echo "PHASE DE COMPILATION             : " $(TEST51SRC)
	$(CC) $(CFLAGS) $(TEST51OBJ) $(LIB) -L$(RTHOME)/lib/hp700_hpux_9_0 -lrtutil -lrtipc -lrtserver -L$(SYBASE)/lib -lblk -lcs -lct -lcomn -lintl -lsybdb -ltcl -linsck -lm  -o $@
	echo "EXECUTABLE DE TEST PRET          : " $(TEST51EXE)
	echo	

$(TEST52EXE) : $(TEST52SRC) $(LIB) $(TEST52OBJ)
	echo "PHASE DE COMPILATION             : " $(TEST52SRC)
	$(CC) $(CFLAGS) $(TEST52OBJ) $(LIB) -L$(RTHOME)/lib/hp700_hpux_9_0 -lrtutil -lrtipc -lrtserver -L$(SYBASE)/lib -lblk -lcs -lct -lcomn -lintl -lsybdb -ltcl -linsck -lm  -o $@
	echo "EXECUTABLE DE TEST PRET          : " $(TEST52EXE)
	echo	

$(TEST53EXE) : $(TEST53SRC) $(LIB) $(TEST53OBJ)
	echo "PHASE DE COMPILATION             : " $(TEST53SRC)
	$(CC) $(CFLAGS) $(TEST53OBJ) $(LIB) -L$(RTHOME)/lib/hp700_hpux_9_0 -lrtutil -lrtipc -lrtserver -L$(SYBASE)/lib -lblk -lcs -lct -lcomn -lintl -lsybdb -ltcl -linsck -lm  -o $@
	echo "EXECUTABLE DE TEST PRET          : " $(TEST53EXE)
	echo	

$(TEST54EXE) : $(TEST54SRC) $(LIB) $(TEST54OBJ)
	echo "PHASE DE COMPILATION             : " $(TEST54SRC)
	$(CC) $(CFLAGS) $(TEST54OBJ) $(LIB) -L$(RTHOME)/lib/hp700_hpux_9_0 -lrtutil -lrtipc -lrtserver -L$(SYBASE)/lib -lblk -lcs -lct -lcomn -lintl -lsybdb -ltcl -linsck -lm  -o $@
	echo "EXECUTABLE DE TEST PRET          : " $(TEST54EXE)
	echo	


$(TEST55EXE) : $(TEST55SRC) $(LIB) $(TEST55OBJ)
	echo "PHASE DE COMPILATION             : " $(TEST55SRC)
	$(CC) $(CFLAGS) $(TEST55OBJ) $(LIB) -L$(RTHOME)/lib/hp700_hpux_9_0 -lrtutil -lrtipc -lrtserver -L$(SYBASE)/lib -lblk -lcs -lct -lcomn -lintl -lsybdb -ltcl -linsck -lm  -o $@
	echo "EXECUTABLE DE TEST PRET          : " $(TEST55EXE)
	echo	

$(TEST56EXE) : $(TEST56SRC) $(LIB) $(TEST56OBJ)
	echo "PHASE DE COMPILATION             : " $(TEST56SRC)
	$(CC) $(CFLAGS) $(TEST56OBJ) $(LIB) -L$(RTHOME)/lib/hp700_hpux_9_0 -lrtutil -lrtipc -lrtserver -L$(SYBASE)/lib -lblk -lcs -lct -lcomn -lintl -lsybdb -ltcl -linsck -lm  -o $@
	echo "EXECUTABLE DE TEST PRET          : " $(TEST56EXE)
	echo	

$(TEST57EXE) : $(TEST57SRC) $(LIB) $(TEST57OBJ)
	echo "PHASE DE COMPILATION             : " $(TEST57SRC)
	$(CC) $(CFLAGS) $(TEST57OBJ) $(LIB) -L$(RTHOME)/lib/hp700_hpux_9_0 -lrtutil -lrtipc -lrtserver -L$(SYBASE)/lib -lblk -lcs -lct -lcomn -lintl -lsybdb -ltcl -linsck -lm  -o $@
	echo "EXECUTABLE DE TEST PRET          : " $(TEST57EXE)
	echo	

$(TEST58EXE) : $(TEST58SRC) $(LIB) $(TEST58OBJ)
	echo "PHASE DE COMPILATION             : " $(TEST58SRC)
	$(CC) $(CFLAGS) $(TEST58OBJ) $(LIB) -L$(RTHOME)/lib/hp700_hpux_9_0 -lrtutil -lrtipc -lrtserver -L$(SYBASE)/lib -lblk -lcs -lct -lcomn -lintl -lsybdb -ltcl -linsck -lm  -o $@
	echo "EXECUTABLE DE TEST PRET          : " $(TEST58EXE)
	echo	

$(TEST59EXE) : $(TEST59SRC) $(LIB) $(TEST59OBJ)
	echo "PHASE DE COMPILATION             : " $(TEST59SRC)
	$(CC) $(CFLAGS) $(TEST59OBJ) $(LIB) -L$(RTHOME)/lib/hp700_hpux_9_0 -lrtutil -lrtipc -lrtserver -L$(SYBASE)/lib -lblk -lcs -lct -lcomn -lintl -lsybdb -ltcl -linsck -lm  -o $@
	echo "EXECUTABLE DE TEST PRET          : " $(TEST59EXE)
	echo	

$(TEST60EXE) : $(TEST60SRC) $(LIB) $(TEST60OBJ)
	echo "PHASE DE COMPILATION             : " $(TEST60SRC)
	$(CC) $(CFLAGS) $(TEST60OBJ) $(LIB) -L$(RTHOME)/lib/hp700_hpux_9_0 -lrtutil -lrtipc -lrtserver -L$(SYBASE)/lib -lblk -lcs -lct -lcomn -lintl -lsybdb -ltcl -linsck -lm  -o $@
	echo "EXECUTABLE DE TEST PRET          : " $(TEST60EXE)
	echo	

$(TEST61EXE) : $(TEST61SRC) $(LIB) $(TEST61OBJ)
	echo "PHASE DE COMPILATION             : " $(TEST61SRC)
	$(CC) $(CFLAGS) $(TEST61OBJ) $(LIB) -L$(RTHOME)/lib/hp700_hpux_9_0 -lrtutil -lrtipc -lrtserver -L$(SYBASE)/lib -lblk -lcs -lct -lcomn -lintl -lsybdb -ltcl -linsck -lm  -o $@
	echo "EXECUTABLE DE TEST PRET          : " $(TEST61EXE)
	echo	

$(TEST62EXE) : $(TEST62SRC) $(LIB) $(TEST62OBJ)
	echo "PHASE DE COMPILATION             : " $(TEST62SRC)
	$(CC) $(CFLAGS) $(TEST62OBJ) $(LIB) -L$(RTHOME)/lib/hp700_hpux_9_0 -lrtutil -lrtipc -lrtserver -L$(SYBASE)/lib -lblk -lcs -lct -lcomn -lintl -lsybdb -ltcl -linsck -lm  -o $@
	echo "EXECUTABLE DE TEST PRET          : " $(TEST62EXE)
	echo	

$(TEST63EXE) : $(TEST63SRC) $(LIB) $(TEST63OBJ)
	echo "PHASE DE COMPILATION             : " $(TEST63SRC)
	$(CC) $(CFLAGS) $(TEST63OBJ) $(LIB) -L$(RTHOME)/lib/hp700_hpux_9_0 -lrtutil -lrtipc -lrtserver -L$(SYBASE)/lib -lblk -lcs -lct -lcomn -lintl -lsybdb -ltcl -linsck -lm  -o $@
	echo "EXECUTABLE DE TEST PRET          : " $(TEST63EXE)
	echo	

$(TEST64EXE) : $(TEST64SRC) $(LIB) $(TEST64OBJ)
	echo "PHASE DE COMPILATION             : " $(TEST64SRC)
	$(CC) $(CFLAGS) $(TEST64OBJ) $(LIB) -L$(RTHOME)/lib/hp700_hpux_9_0 -lrtutil -lrtipc -lrtserver -L$(SYBASE)/lib -lblk -lcs -lct -lcomn -lintl -lsybdb -ltcl -linsck -lm  -o $@
	echo "EXECUTABLE DE TEST PRET          : " $(TEST64EXE)
	echo	

$(TEST65EXE) : $(TEST65SRC) $(LIB) $(TEST65OBJ)
	echo "PHASE DE COMPILATION             : " $(TEST65SRC)
	$(CC) $(CFLAGS) $(TEST65OBJ) $(LIB) -L$(RTHOME)/lib/hp700_hpux_9_0 -lrtutil -lrtipc -lrtserver -L$(SYBASE)/lib -lblk -lcs -lct -lcomn -lintl -lsybdb -ltcl -linsck -lm  -o $@
	echo "EXECUTABLE DE TEST PRET          : " $(TEST65EXE)
	echo	

$(TEST66EXE) : $(TEST66SRC) $(LIB) $(TEST66OBJ)
	echo "PHASE DE COMPILATION             : " $(TEST66SRC)
	purify cc $(CFLAGS) $(TEST66OBJ) $(ICGLIB)/xzaoa.a $(ICGLIB)/xzs.a $(ICGLIB)/xd.a -L$(RTHOME)/lib/hp700_hpux_9_0 -lrtutil -lrtipc -lrtserver -L$(SYBASE)/lib -lblk -lcs -lct -lcomn -lintl -lsybdb -ltcl -linsck -lm  -o $@
	echo "EXECUTABLE DE TEST PRET          : " $(TEST66EXE)
	echo	

$(TEST67EXE) : $(TEST67SRC) $(LIB) $(TEST67OBJ)
	echo "PHASE DE COMPILATION             : " $(TEST67SRC)
	$(CC) $(CFLAGS) $(TEST67OBJ) $(LIB) -L$(RTHOME)/lib/hp700_hpux_9_0 -lrtutil -lrtipc -lrtserver -L$(SYBASE)/lib -lblk -lcs -lct -lcomn -lintl -lsybdb -ltcl -linsck -lm  -o $@
	echo "EXECUTABLE DE TEST PRET          : " $(TEST67EXE)
	echo	

$(TEST68EXE) : $(TEST68SRC) $(LIB) $(TEST68OBJ)
	echo "PHASE DE COMPILATION             : " $(TEST68SRC)
	$(CC) $(CFLAGS) $(TEST68OBJ) $(LIB) -L$(RTHOME)/lib/hp700_hpux -lrtutil -lrtipc -lrtserver -L$(SYBASE)/lib -lblk -lcs -lct -lcomn -lintl -lsybdb -ltcl -linsck -lm  -o $@
	echo "EXECUTABLE DE TEST PRET          : " $(TEST68EXE)
	echo	



.c.o:
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o *.ln *.x *.out
	echo "Menage effectue"


