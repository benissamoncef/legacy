/*X  Fichier : $Id: alarmes.bas,v 1.6 1996/04/22 18:24:40 gaborit Exp $      Release : $Revision: 1.6 $        Date : $Date: 1996/04/22 18:24:40 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER alarmes.bas
------------------------------------------------------
* DESCRIPTION :
* renseigne les types d'alerme de CFG a partir des constantes
* d≈finies dans XDC (» faire au CI)
------------------------------------------------------
* HISTORIQUE :
*
* B.G.     	??/11/94	: Creation 
* C.T.     	15/03/95	: Creation avec entete (V 1.1)
* C.T.     	02/04/95	: Ajout de l'envoi (go) (V 1.2)
* B.G.          04/04/95        : mise en conformite avec xzamc 1.14 (V 1.3)
* Mismer   D.   21 Mar 1996	: Ajout de type d'alarme en reserve pour mini GTC (V 1.4)
* C.T.          22/04/96        : Ajout alarme fax (V 1.6)
----------------------------------------------------*/

use CFG
go
delete TYP_ALM
go
/* conforme » xzamc.h 1.14 */
insert TYP_ALM values ( 27 , "MANUELLE MINEURE" ,0,0,1)
insert TYP_ALM values ( 28 , "MANUELLE MAJEURE" ,0,1,0)
insert TYP_ALM values ( 29 , "MANUELLE CRITIQUE" ,1,0,0)
insert TYP_ALM values ( 1 , "NON REPONSE" ,0,1,0)
insert TYP_ALM values ( 2 , "ERREUR CHECKSUM" ,0,0,1)
insert TYP_ALM values ( 3 , "ERR NAK OU TRANS" ,0,0,1)
insert TYP_ALM values ( 4 , "ERR RGS" ,0,0,1)
insert TYP_ALM values ( 5 , "DEFAUT EDF" ,0,1,0)
insert TYP_ALM values ( 6 , "ERREUR INTERNE" ,0,1,0)
insert TYP_ALM values ( 7 , "ERREUR PORT SERIE NO 1" ,0,0,1)
insert TYP_ALM values ( 8 , "ERREUR PORT SERIE NO 2" ,0,0,1)
insert TYP_ALM values ( 9 , "PERTE DU MESSAGE" ,0,0,1)
insert TYP_ALM values ( 10 , "HORLOGE HORS SERVICE" ,0,0,1)
insert TYP_ALM values ( 11 , "PROBLEME MODULE" ,0,0,1)
insert TYP_ALM values ( 12 , "PROBLEME MODULE" ,0,1,0)
insert TYP_ALM values ( 13 , "PROBLEME LAMPE" ,0,0,1)
insert TYP_ALM values ( 14 , "PROBLEME LAMPE" ,0,1,0)
insert TYP_ALM values ( 15 , "PB. MESURE TEMPERATURE" ,0,0,1)
insert TYP_ALM values ( 16 , "PB. CELLULE LUMINOSITE" ,0,0,1)
insert TYP_ALM values ( 18 , "DEFAUT TRANSMISSION" ,0,0,1)
insert TYP_ALM values ( 19 , "DEFAUT ALIM. LAMPE" ,0,0,1)
insert TYP_ALM values ( 20 , "DEFAUT BATTERIE" ,0,0,1)
insert TYP_ALM values ( 21 , "PANNE BATTERIE" ,0,0,1)
insert TYP_ALM values ( 22 , "LAMPE HS" ,0,0,1)
insert TYP_ALM values ( 23 , "DEFAUT VIDEO" ,0,1,0)
insert TYP_ALM values ( 24 , "DEFAUT TRANSMISSION" ,0,0,1)
insert TYP_ALM values ( 25 , "DEFAUT ANALY. ARRETE" ,0,0,1)
insert TYP_ALM values ( 26 , "ERREUR CHECKSUM" ,0,0,1)
insert TYP_ALM values ( 30 , "ENERGIE" ,0,1,0)
insert TYP_ALM values ( 31 , "LAMPE" ,0,1,0)
insert TYP_ALM values ( 32 , "DISC SEQUENCE" ,0,0,1)
insert TYP_ALM values ( 33 , "DISC PANNEAU" ,0,0,1)
insert TYP_ALM values ( 34 , "MOTEUR HS" ,0,1,0)
insert TYP_ALM values ( 35 , "TRAMSMISSION HS" ,0,0,1)
insert TYP_ALM values ( 36 , "MODULE HS" ,0,0,1)
insert TYP_ALM values ( 37 , "ARMOIRE OUVERTE" ,0,0,1)
insert TYP_ALM values ( 38 , "DEFAUT FONCT. VERIN" ,0,0,1)
insert TYP_ALM values ( 39 , "BOUT. AZOTE VIDES" ,0,0,1)
insert TYP_ALM values ( 40 , "PROBLEME ANTENNE" ,0,0,1)
insert TYP_ALM values ( 41 , "DEFAUT PHONIE" ,0,1,0)
insert TYP_ALM values ( 42 , "DEFAUT PILE" ,0,1,0)
insert TYP_ALM values ( 43 , "TRANS. TELEPHONIE HS" ,0,1,0)
insert TYP_ALM values ( 44 , "ERREUR REPONSE" ,0,0,1)
insert TYP_ALM values ( 45 , "DEFAUT CYCLE SATIR" ,0,1,0)
insert TYP_ALM values ( 46 , "DEFAUT INTER GENERAL" ,0,1,0)
insert TYP_ALM values ( 47 , "ONDULEUR SUR BATTERIE" ,0,0,1)
insert TYP_ALM values ( 48 , "DEFAUT ONDULEUR" ,0,1,0)
insert TYP_ALM values ( 49 , "DEFAUT CLIMATISATION" ,0,0,1)
insert TYP_ALM values ( 50 , "DEFAUT BUS NORMAL EST" ,0,1,0)
insert TYP_ALM values ( 51 , "DEFAUT BUS NORMAL OUEST" ,0,1,0)
insert TYP_ALM values ( 52 , "DEFAUT BUS ONDULE EST" ,0,1,0)
insert TYP_ALM values ( 53 , "DEFAUT BUS ONDULE OUEST" ,0,1,0)
insert TYP_ALM values ( 54 , "DEFAUT BAIE VIDEO" ,0,1,0)
insert TYP_ALM values ( 55 , "DEFAUT BAIE DAI" ,0,1,0)
insert TYP_ALM values ( 56 , "DEFAUT BAIE INFORMATIQUE" ,0,1,0)
insert TYP_ALM values ( 57 , "DEFAUT PMV 1" ,0,1,0)
insert TYP_ALM values ( 58 , "DEFAUT PMV 2" ,0,1,0)
insert TYP_ALM values ( 59 , "COM. TACHE EQUIPT. HS" ,0,1,0)
insert TYP_ALM values ( 60 , "ALARME 1" ,0,0,1)
insert TYP_ALM values ( 61 , "ALARME 2" ,0,0,1)
insert TYP_ALM values ( 62 , "ALARME 3" ,0,0,1)
insert TYP_ALM values ( 63 , "ALARME 4" ,0,0,1)
insert TYP_ALM values ( 64 , "ALARME 5" ,0,0,1)
insert TYP_ALM values ( 65 , "NUMERO DE FAX BRULE" ,0,1,0)
go
