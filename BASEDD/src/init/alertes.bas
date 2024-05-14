/*X  Fichier : $Id: alertes.bas,v 1.5 1996/04/22 18:24:53 gaborit Exp $      Release : $Revision: 1.5 $        Date : $Date: 1996/04/22 18:24:53 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER typesFMC.bas
------------------------------------------------------
* DESCRIPTION :
* renseigne les types d'alerte de CFG a partir des constantes
* d≈finies dans "xzaac" (» faire au CI)
------------------------------------------------------
* HISTORIQUE :
*
* B.G.     	??/11/95	: Creation 
* C.T.     	15/03/95	: Creation avec entete (V 1.1)
* C.T.     	02/04/95	: Ajout de l'envoi (go) (V 1.2)
* BG		04/04/95	: mise a jour v1.13 xzaac.h (V 1.3)
* D.M.  21/03/96        :  ajout d'un type d'alerte pour mini GTC (DEM/734) (1.4)
* C.T.     	22/04/96	: suppression alerte fax (V 1.6)
----------------------------------------------------*/

use CFG
go

delete TYP_ALT
go

/* conforme » xzaac.h 1.12 */
insert TYP_ALT values ( 1 , "RALENTISSEMENT" )
insert TYP_ALT values ( 2 , "VEHICULE ARRETE" )
insert TYP_ALT values ( 3 , "VEHICULE ARRETE + BOUCHON" )
insert TYP_ALT values ( 4 , "AFFICHAGE INCOHERENT" )
insert TYP_ALT values ( 5 , "GTC NICE EN MODE LOCAL" )
insert TYP_ALT values ( 6 , "LIT D'ARRET OCCUPE" )
insert TYP_ALT values ( 7 , "EXTINCTEUR DECROCHE" )
insert TYP_ALT values ( 8 , "SEUIL CO ATTEINT" )
insert TYP_ALT values ( 9 , "SEUIL OPACITE ATTEINT" )
insert TYP_ALT values ( 10 , "CONNEXION LOCALE LECTURE" )
insert TYP_ALT values ( 11 , "CONNEXION LOCALE ECRITURE" )
insert TYP_ALT values ( 12 , "CONNEXION LOCALE INCONNU" )
insert TYP_ALT values ( 13 , "COLLISION BARRIERE" )
insert TYP_ALT values ( 14 , "APPEL PAU" )
insert TYP_ALT values ( 15 , "GENDARME EN ECOUTE" )
insert TYP_ALT values ( 16 , "FIN GN EN ECOUTE" )
insert TYP_ALT values ( 17 , "GN DEMANDE LE SECRET" )
insert TYP_ALT values ( 18 , "GN DEMANDE REPRISE SAE" )
insert TYP_ALT values ( 19 , "PASSAGE JOUR-NUIT" )
insert TYP_ALT values ( 20 , "INTRUSION 1" )
insert TYP_ALT values ( 21 , "INTRUSION 2" )
insert TYP_ALT values ( 22 , "EQUATION D'ALERTE RADT" )
insert TYP_ALT values ( 23 , "LTV SATURE" )
insert TYP_ALT values ( 24 , "EQUATION D'ALERTE EVENT" )
insert TYP_ALT values ( 25 , "CHANGEMENT NIV DE SERVICE" )
insert TYP_ALT values ( 27 , "PROBLEME DE TEMPERATURE" )
insert TYP_ALT values ( 28 , "NIVEAU D'EAU MAX ATTEINT" )
insert TYP_ALT values ( 29 , "ALERTE 1" )
insert TYP_ALT values ( 30 , "ALERTE 2" )
insert TYP_ALT values ( 31 , "ALERTE 3" )
insert TYP_ALT values ( 32 , "ALERTE 4" )
insert TYP_ALT values ( 33 , "ALERTE 5" )
go
