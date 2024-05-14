/*X  Fichier : $Id: fmc.bas,v 1.3 1996/11/28 11:52:13 gaborit Exp $      Release : $Revision: 1.3 $        Date : $Date: 1996/11/28 11:52:13 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER fmc.bas
------------------------------------------------------
* DESCRIPTION :
* renseigne les tables de CFG a partir des constantes
* d≈finies dans XDC (» faire au CI)
------------------------------------------------------
* HISTORIQUE :
*
* B.GABORIT	11/10/94	: Creation
* C.T.     	15/03/95	: Creation sous un autre fichier
* C.T.     	13/04/95	: Correction faute d'orthographe 
* C.T.     	28/11/96	: mise en minuscule de convoi militaire et excep.
*                                 et en operation
----------------------------------------------------*/

use CFG
go


/* cr≈ation des types d'≈v≈nement ou op≈ration */

delete TYP_FMC
insert TYP_FMC (numero,classe,nom,abreviation,operation,supprime)
values (XZAEC_FMC_Accident , XZAEC_ClasseAccident , "ACCIDENT","ACCIDENT", 0,0)
insert TYP_FMC (numero,classe,nom,abreviation,operation,supprime)
values (XZAEC_FMC_Delestage , XZAEC_ClasseDelestage , "D≈lestage","D≈lestage", 1,0)
insert TYP_FMC (numero,classe,nom,abreviation,operation,supprime)
values (XZAEC_FMC_Deviation , XZAEC_ClasseDelestage , "D≈viation","D≈viation", 1,0)
insert TYP_FMC (numero,classe,nom,abreviation,operation,supprime)
values (XZAEC_FMC_PosteOperateur , XZAEC_ClassePosteOper , "Poste Operateur","Operateur", 1,0)
insert TYP_FMC (numero,classe,nom,abreviation,operation,supprime)
values (XZAEC_FMC_Meteo , XZAEC_ClasseMeteo , "PERTURBATION METEO","METEO", 0,0)
insert TYP_FMC (numero,classe,nom,abreviation,operation,supprime)
values (XZAEC_FMC_Contresens , XZAEC_ClasseVehicule , "VEHICULE A CONTRESENS","CONTRESENS", 0,0)
insert TYP_FMC (numero,classe,nom,abreviation,operation,supprime)
values (XZAEC_FMC_VehArrete , XZAEC_ClasseVehicule , "VEHICULE ARRETE","VEH ARRETE", 0,0)
insert TYP_FMC (numero,classe,nom,abreviation,operation,supprime)
values (XZAEC_FMC_VehPanne , XZAEC_ClasseVehicule , "VEHICULE EN PANNE","PANNE VEH", 0,0)
insert TYP_FMC (numero,classe,nom,abreviation,operation,supprime)
values (XZAEC_FMC_VehFeu , XZAEC_ClasseVehicule , "VEHICULE EN FEU","VEH EN FEU", 0,0)
insert TYP_FMC (numero,classe,nom,abreviation,operation,supprime)
values (XZAEC_FMC_Cirque , XZAEC_ClasseVehLent , "CIRQUE EN CONVOI","CIRQUE", 0,0)
insert TYP_FMC (numero,classe,nom,abreviation,operation,supprime)
values (XZAEC_FMC_ConvoiExc , XZAEC_ClasseVehLent , "Convoi exceptionnel","CONVOI EXC", 1,0)
insert TYP_FMC (numero,classe,nom,abreviation,operation,supprime)
values (XZAEC_FMC_ConvoiMil , XZAEC_ClasseVehLent , "Convoi militaire","Convoi mil", 1,0)
insert TYP_FMC (numero,classe,nom,abreviation,operation,supprime)
values (XZAEC_FMC_VehLent , XZAEC_ClasseVehLent , "VEHICULE LENT","VEH LENT", 0,0)
insert TYP_FMC (numero,classe,nom,abreviation,operation,supprime)
values (XZAEC_FMC_Manifestation , XZAEC_ClasseManif , "MANIFESTATION","MANIF.", 0,0)
insert TYP_FMC (numero,classe,nom,abreviation,operation,supprime)
values (XZAEC_FMC_GreveESCOTA , XZAEC_ClasseManif , "GREVE PERSONNEL ESCOTA","GREVE ESC.", 0,0)
insert TYP_FMC (numero,classe,nom,abreviation,operation,supprime)
values (XZAEC_FMC_Escargot , XZAEC_ClasseManif , "OPERATION ESCARGOT","ESCARGOT", 0,0)
insert TYP_FMC (numero,classe,nom,abreviation,operation,supprime)
values (XZAEC_FMC_Basculement , XZAEC_ClasseBasculement , "Basculement","Basculement", 1,0)
insert TYP_FMC (numero,classe,nom,abreviation,operation,supprime)
values (XZAEC_FMC_Travaux , XZAEC_ClasseTravaux , "Travaux","Travaux", 1,0)
insert TYP_FMC (numero,classe,nom,abreviation,operation,supprime)
values (XZAEC_FMC_IncSsConces , XZAEC_ClasseSousConcess , "INCIDENT SS CONCESS.","SS CONCESS", 0,0)
insert TYP_FMC (numero,classe,nom,abreviation,operation,supprime)
values (XZAEC_FMC_QueueBouchon , XZAEC_ClasseBouchon , "BOUCHON","BOUCHON", 0,0)
insert TYP_FMC (numero,classe,nom,abreviation,operation,supprime)
values (XZAEC_FMC_FermetureEch , XZAEC_ClasseEchangeur , "Fermeture ≈changeur","Ferm. ≈ch.", 1,0)
insert TYP_FMC (numero,classe,nom,abreviation,operation,supprime)
values (XZAEC_FMC_Eboulement , XZAEC_ClasseGenerique , "EBOULEMENT","EBOULEMENT", 0,0)
insert TYP_FMC (numero,classe,nom,abreviation,operation,supprime)
values (XZAEC_FMC_Incendie , XZAEC_ClasseGenerique , "INCENDIE DE VEGETATION","INC. VEGE.", 0,0)
insert TYP_FMC (numero,classe,nom,abreviation,operation,supprime)
values (XZAEC_FMC_Inondation , XZAEC_ClasseGenerique , "INONDATION","INONDATION", 0,0)
insert TYP_FMC (numero,classe,nom,abreviation,operation,supprime)
values (XZAEC_FMC_Obstacle , XZAEC_ClasseGenerique , "OBSTACLE SUR CHAUSSEE","OBSTACLE", 0,0)
insert TYP_FMC (numero,classe,nom,abreviation,operation,supprime)
values (XZAEC_FMC_ChausseDeter , XZAEC_ClasseGenerique , "CHAUSSEE DETERIOREE","CHAUSSEE", 0,0)
insert TYP_FMC (numero,classe,nom,abreviation,operation,supprime)
values (XZAEC_FMC_FlaqueHuile , XZAEC_ClasseGenerique , "FLAQUE D'HUILE","HUILE", 0,0)
insert TYP_FMC (numero,classe,nom,abreviation,operation,supprime)
values (XZAEC_FMC_AnimalErrant , XZAEC_ClasseGenerique , "ANIMAL ERRANT","ANIMAL", 0,0)
insert TYP_FMC (numero,classe,nom,abreviation,operation,supprime)
values (XZAEC_FMC_FrontiereFermee , XZAEC_ClasseGenerique , "FRONTIERE FERMEE","FRONTIERE", 0,0)
insert TYP_FMC (numero,classe,nom,abreviation,operation,supprime)
values (XZAEC_FMC_AltercationUsager , XZAEC_ClasseGenerique , "ALTERCATION USAGERS","ALTERCATION", 0,0)
insert TYP_FMC (numero,classe,nom,abreviation,operation,supprime)
values (XZAEC_FMC_Agression , XZAEC_ClasseGenerique , "AGRESSION","AGRESSION", 0,0)
insert TYP_FMC (numero,classe,nom,abreviation,operation,supprime)
values (XZAEC_FMC_MalaiseUsager , XZAEC_ClasseGenerique , "MALAISE USAGER","MALAISE", 0,0)
insert TYP_FMC (numero,classe,nom,abreviation,operation,supprime)
values (XZAEC_FMC_EvtHorsReseau , XZAEC_ClasseGenerique , "EVENEMENT HORS RESEAU","HORS RES.", 0,0)
insert TYP_FMC (numero,classe,nom,abreviation,operation,supprime)
values (XZAEC_FMC_AireFermee , XZAEC_ClasseGenerique , "Aire ferm≈e","Aire ferm.", 1,0)
insert TYP_FMC (numero,classe,nom,abreviation,operation,supprime)
values (XZAEC_FMC_Salage , XZAEC_ClasseGenerique , "Salage","Salage", 1,0)
insert TYP_FMC (numero,classe,nom,abreviation,operation,supprime)
values (XZAEC_FMC_HoldUpVol , XZAEC_ClasseGenerique , "HOLD-UP / VOL","HOLDUP VOL", 0,0)
insert TYP_FMC (numero,classe,nom,abreviation,operation,supprime)
values (XZAEC_FMC_TeteBouchon , XZAEC_ClasseGenerique , "BOUCHON (t¡te)","BOUCHON T", 0,0)
insert TYP_FMC (numero,classe,nom,abreviation,operation,supprime)
values (XZAEC_FMC_PietonChaussee , XZAEC_ClasseGenerique , "PIETONS SUR LA CHAUSSEE","PIETONS", 0,0)
insert TYP_FMC (numero,classe,nom,abreviation,operation,supprime)
values (XZAEC_FMC_Autre , XZAEC_ClasseGenerique , "AUTRE EVENEMENT","AUTRE", 0,0)
go
