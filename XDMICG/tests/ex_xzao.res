XZAG02_Connexion_Base retourne :0

** Test de la procedure XZAO25_Nom_Autoroute **


*****
CR=0 - XZAO25_Nom_Autoroute 4 = <A501>
*****


*****
CR=0 - XZAO25_Nom_Autoroute 10 = <N296>
*****


*****
CR=1 - XZAO25_Nom_Autoroute 15 = <>
*****


*****
CR=1 - XZAO25_Nom_Autoroute 0 = <>
*****


*****
CR=0 - XZAO25_Nom_Autoroute 1 = <A8>
*****

------------------------------------------------------------------------------

** Test de la procedure XZAO26_Numero_Autoroute **


*****
XZAO26_Numero_Autoroute A57 retourne le numero : 8
*****


*****
XZAO26_Numero_Autoroute A8 retourne le numero : 1
*****


*****
XZAO26_Numero_Autoroute  retourne le numero : 0
*****


*****
XZAO26_Numero_Autoroute NULL retourne le numero : 0
*****


*****
XZAO26_Numero_Autoroute A570 retourne le numero : 11
*****

------------------------------------------------------------------------------

** Test de la procedure XZAO27_Ordre_PR **


*****
XZAO27_Ordre_PR Autoroute 1 = 1
*****


*****
XZAO27_Ordre_PR Autoroute 13 = 1
*****


*****
XZAO27_Ordre_PR Autoroute 0 = 1
*****


*****
XZAO27_Ordre_PR Autoroute 8 = -1
*****


*****
XZAO27_Ordre_PR Autoroute 5 = 1
*****

------------------------------------------------------------------------------

** Test de la procedure XZAO28_Autoroute **


*****
CR=0 - Autoroute 1 <A8>, INV=1, Escota=1, Debut/Fin 0/224000, Concession 17900/224000
*****


*****
CR=0 - Autoroute 3 <A50>, INV=1, Escota=1, Debut/Fin 15362/72780, Concession 15362/67701
*****


*****
CR=1 - Autoroute 0 <>, INV=0, Escota=0, Debut/Fin 0/0, Concession 0/0
*****


*****
CR=0 - Autoroute 11 <A570>, INV=1, Escota=0, Debut/Fin 0/7160, Concession 0/0
*****
XZAG03_Deconnexion_Base retourne 0
