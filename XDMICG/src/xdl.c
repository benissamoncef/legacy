/*E*/
/* Fichier $Id: xdl.c,v 1.28 2017/04/25 15:31:09 devgfi Exp $			Release : $Revision: 1.28 $        Date : $Date: 2017/04/25 15:31:09 $
******************************************************
* STERIA *  PROJET MIGRAZUR		
******************************************************
* MODULE XDL  * FICHIER  xdl.c
******************************************************
* DESCRIPTION DU MODULE :
*
******************************************************
* HISTORIQUE :
*
*  T.MIlleville   27 Oct 94 	: Creation
*
*  T.MIlleville   13 Jan 95 	: Ajout XDL_SUP59_DSKNOK
*				  et XDL_SUP60_ALLDSKNOK V1.3
*
*  T.MIlleville   16 Jan 95 	: Ajout XDL_SUP61_DSKOK
*				  et XDL_SUP62_SYNCOK	V1.4
*
*  T.MIlleville   25 Jan 95 	: Ajout XDL_SUP63_SRVNOK V1.8
*  T.MIlleville   09 Mar 95 	: Modification des textes V1.12
*  T.MIlleville   09 Mar 95 	: Ajout d'un msg SUP_64 pour prevenir 
*		d'un pb de lecture horloge V1.13
*  T.MIlleville   19 Avr 95 	: Suppression du msg SUP48 declare 
*	2 fois V1.14
*  T.MIlleville   17 Mai 95 	: Modification des flags de diffusion
*	di msg SUP48  V1.15
*  T.MIlleville   18 Mai 95 	: Modification des flags de diffusion
*	di msg SUP48  V1.16
*  T.MIlleville   10 Aug 95 	: Ajout msg XDL_SUP39_SCHS  V1.17
*  T.MIlleville   12 Oct 95 	: Ajout commantaires et traces V1.18
*  P.Niepceron	  15 Fev 96	: Correction du libelle de XDL_SUP07_CMDLAPP v1.19
*  P.Niepceron    16 Fev 96     : Correction du libelle de XDL_SUP47_COMBEOK v1.20
*  P.Niepceron    1  Mar 96	: Correction des accents v1.21 1.22
* Guilhou	22 aut 96	: ajout EXTENSION OUESt v1.24
*  P.Niepceron    3 sep 96	: correction du site de diffusion de SUP47 v1.25
* JMG		26/06/10 	: linyx DEM/934
* JPL		21/03/17	: Changement du terme Secteur en Region (DEM 1173)  1.28
*******************************************************/
/******************************************************
* TESTS UNITAIRES  : RELECTURE CROISEE
*
*
*******************************************************/


/* fichiers inclus */

#include "xdy.h"
#include "xzst.h"
#include "xdl.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

XDLT_SystemeAlarme tg_msgSysteme [] =
{
  /* Messages generales */
  { XDL_TEXT, XDLC_DAUCUN, XDLC_BAUCUN, "%s" },

  /* Messages et alarmes du Sous-Systeme SUPERV */

  { XDL_SUP01_CMDLTAC, XDLC_DAUCUN, XDLC_BAUCUN, "Administration : commande de lancement de la tache %s sur la machine %s." },
  { XDL_SUP02_CMDATAC, XDLC_DAUCUN, XDLC_BAUCUN, "Administration : commande d'arrêt de la tache %s sur la machine %s."},
  { XDL_SUP03_CMDRBOO, XDLC_DAUCUN, XDLC_BAUCUN, "Administration : commande de reboot de la machine %s."},
  { XDL_SUP04_CMDASSY, XDLC_DAUCUN, XDLC_BAUCUN, "Administration : commande d'arrêt du sous-systeme %s sur la machine %s."},
  { XDL_SUP05_CMDLSSY, XDLC_DAUCUN, XDLC_BAUCUN, "Administration : commande de lancement du sous-systeme %s sur la machine %s."},
  { XDL_SUP06_CMDAAPP, XDLC_DAUCUN, XDLC_BAUCUN, "Administration : commande d'arrêt de l'application sur la machine %s."},
  { XDL_SUP07_CMDLAPP, XDLC_DAUCUN, XDLC_BAUCUN, "Administration : commande de lancement de l'application sur la machine %s."},
  { XDL_SUP08_CMDAMAC, XDLC_DAUCUN, XDLC_BAUCUN, "Administration : commande d'arrêt de la machine %s."},
  { XDL_SUP09_DISPTAC, XDLC_DAUCUN, XDLC_BAUCUN, "Surveillance : disparition de la tache %s sur la machine %s."},
  { XDL_SUP10_RELATAC, XDLC_DAUCUN, XDLC_BAUCUN, "Surveillance : relance automatique de la tache %s sur la machine %s."},
  { XDL_SUP17_PRISLOG, XDLC_DAUCUN, XDLC_BAUCUN, "Administration : Prise en compte d'une nouvelle version logicielle."},
  { XDL_SUP20_MACETAT, XDLC_DAUCUN, XDLC_BAUCUN, "Administration : Machine %s est dans l'état %s ."},
  { XDL_SUP21_MACSSYS, XDLC_DAUCUN, XDLC_BAUCUN, "Administration : Sur la machine %s,le Sous-Systeme %s  est dans l'état %s ."},
  { XDL_SUP22_MACTACE, XDLC_DAUCUN, XDLC_BAUCUN, "Administration : Sur la machine %s, la tache %s est dans l'état %s ."},
  { XDL_SUP26_MACMODE, XDLC_DAUCUN, XDLC_BAUCUN, "Administration : La machine %s a comme état d'instruction %s et état de reconstruction %s ."},
  { XDL_SUP27_MACVISU, XDLC_DAUCUN, XDLC_BAUCUN, "Administration : Modification du Niveau de traces sur la machine  %s de la tache %s -- Niveau %s -- Affichage %s ."},
  { XDL_SUP30_SUPERHS, XDLC_DAUCUN, XDLC_BAUCUN, "Surveillance %s : Supervision du systeme MIGRAZUR du site de %s hors service."},
  { XDL_SUP31_COMSDHS, XDLC_SITEADM, XDLC_BAUCUN, "Surveillance %s : perte de communication avec le serveur de données %s."},
  { XDL_SUP32_COMSIHS, XDLC_SITEADM, XDLC_BAUCUN, "Surveillance %s : perte de communication avec le site du CI."},
  { XDL_SUP32_COMSIHS, XDLC_SITEADM, XDLC_BAUCUN, "Surveillance %s : perte de communication avec le site du CI."},
  { XDL_SUP33_COMCOHS, XDLC_SITEADM, XDLC_BAUCUN, "Surveillance : perte de communication avec le serveur de communication %s."},
  { XDL_SUP34_COMEXHS, XDLC_SITEADM, XDLC_BAUCUN, "Surveillance %s : perte de communication avec le site d'exploitation de %s."},
  { XDL_SUP35_BASCCOM, XDLC_SITEADM, XDLC_BAUCUN, "Surveillance : basculement sur le serveur de communication %s."},
  { XDL_SUP36_SVCOMRD, XDLC_SITEADM, XDLC_BAUCUN, "Surveillance : serveurs de communication du site d'exploitation %s redondants."},
  { XDL_SUP37_BASCUSD, XDLC_SITEADM, XDLC_BAUCUN, "Surveillance : basculement sur le serveur de données %s."},
  { XDL_SUP38_SVDONRD, XDLC_SITEADM, XDLC_BAUCUN, "Surveillance : serveurs de données redondants."},
  { XDL_SUP39_SCHS, XDLC_SITEADM, XDLC_BAUCUN, "Surveillance %s : perte de communication avec le serveur %s sur le site d'exploitation de %s."},
  { XDL_SUP45_PERIPHS, XDLC_DAUCUN, XDLC_BAUCUN, "Surveillance : Périphérique %s hors service."},
  { XDL_SUP47_COMBEOK, XDLC_SITEADM, XDLC_BAUCUN, "Surveillance %s : reprise de communication avec le site du CI."},
  { XDL_SUP48_COMEXOK, XDLC_SITEADM, XDLC_BAUCUN, "Surveillance %s : reprise de communication avec le site d'exploitation de %s."},
  { XDL_SUP50_COMSCOK, XDLC_SITEADM, XDLC_BAUCUN, "Surveillance %s : reprise de communication avec le serveur de communication %s."},
  { XDL_SUP51_COMSDOK, XDLC_SITEADM, XDLC_BAUCUN, "Surveillance %s : reprise de communication avec le serveur de données %s."},
  { XDL_SUP57_PERIPOK, XDLC_DAUCUN, XDLC_BAUCUN, "Surveillance : Périphérique %s en service."},
  { XDL_SUP58_SECNOK, XDLC_BAUCUN, XDLC_BAUCUN, "Serveur redondant %s indisponible."},
  { XDL_SUP59_DSKNOK, XDLC_GLOBAL, XDLC_BAUCUN, "Disque %s du site %s indisponible."},
  { XDL_SUP60_ALLDSKNOK, XDLC_GLOBAL, XDLC_BAUCUN, "Attention tous les disques du site %s indisponibles."},
  { XDL_SUP61_DSKOK, XDLC_GLOBAL, XDLC_BAUCUN, "Disque %s du site %s de nouveau disponible."},
  { XDL_SUP62_SYNCOK, XDLC_GLOBAL, XDLC_BAUCUN, "Synchronisation disques miroirs du site %s en cours."},
  { XDL_SUP63_SRVNOK, XDLC_GLOBAL, XDLC_BAUCUN, "SQL Server sur le Site %s non présent ."},
  { XDL_SUP64_HORNOK, XDLC_GLOBAL, XDLC_BAUCUN, "Problème de lecture de l'heure sur l'horloge du site %s."},

  /* Messages et alarmes du Sous-Systeme GESIHM */
  { XDL_GES01_GE505OK, 		XDLC_SITEADM, XDLC_BAUCUN, "Génération fichier C505 réussie, nom du fichier: %s"},
  { XDL_GES02_GE505HS, 		XDLC_SITEADM, XDLC_BAUCUN, "Génération fichier C505 a échoué"},
  { XDL_GES03_DATAERR, 		XDLC_SITEADM, XDLC_BAUCUN, "Donnée %s érronée. Recommencez la saisie, SVP"},
  { XDL_GES04_NUMINCI, 		XDLC_SITEADM, XDLC_BAUCUN, "Problème dans la recherche du numéro d'incident"},
  { XDL_GES05_XZCSERR, 		XDLC_SITEADM, XDLC_BAUCUN, "Problème dans la primitive %s"},
  { XDL_GES06_SQLCERR, 		XDLC_SITEADM, XDLC_BAUCUN, "Problème SQL dans la primitive %s"},
  { XDL_GES07_UTINCONNU, 	XDLC_SITEADM, XDLC_BAUCUN, "Utilisateur inconnu"},
  { XDL_GES08_PASSERRONE, 	XDLC_SITEADM, XDLC_BAUCUN, "Mot de passe erroné"},
  { XDL_GES09_LISTEVIDE, 	XDLC_SITEADM, XDLC_BAUCUN, "La liste est vide"},
  { XDL_GES10_SAISECT, 		XDLC_SITEADM, XDLC_BAUCUN, "Veuillez saisir la region, SVP"},
  { XDL_GES11_SAISPASS, 	XDLC_SITEADM, XDLC_BAUCUN, "Veuillez saisir le mot de passe, SVP"},
  { XDL_GES12_BP, 		XDLC_SITEADM, XDLC_BAUCUN, 
     "| Planification | BP | Information gare | Gare de correspondance | Categorie du BP | Coord X | Coord Y | Date de validite | Date de création | BP-CIS |"},
  { XDL_GES13_DP, 		XDLC_SITEADM, XDLC_BAUCUN, 
     "| Planification | DP | Genre | Presence ZSE | Coord X | Coord Y | Attribue au BP | Gare mère | Centre de telecommande | Date de validite | Date de création |"},
  { XDL_GES14_EP, 		XDLC_SITEADM, XDLC_BAUCUN, "| Planification | EP | Attribue au DP | Distance au DP | DABS du DP | au DP | Coord X | Coord Y | Genre | Etat | Direction | Date de validite | Date de création |"},
  { XDL_GES15_ABS, 		XDLC_SITEADM, XDLC_BAUCUN, "| Planification | ABS | Du BP | au BP | Nombre de voies | Date de validite | Date de création |"},
  { XDL_GES16_Rela, 		XDLC_SITEADM, XDLC_BAUCUN, "| Planification | ABS | DABS du DP | au DP | Sens DABS ABS | Verticale | Horizontale | Voie 1 | Sens normal | Sens inverse | Voie 2 | Sens normal | Sens inverse | Voie 3 | Sens normal | Sens inverse | Date de validite | Date de création |"},
  { XDL_GES17_DABS, 		XDLC_SITEADM, XDLC_BAUCUN, "| Planification | DABS du DP | au DP | Sens | Genre | Genre de circulation | Longueur | Nombre de voies | Troncon radio | Un | Deux | Trois | Quatre | Cinq | Six | HPLKI | HPLKI+10 | HPLKII | HPLKII-10 | Train D | Grand container | Reserve | Sensibilite d'ecart | Sensibilite repetition | Correction ZWL | Date de validite | Date de création | Sous-region  Priorite |"},
  { XDL_GES18_Pac, 		XDLC_SITEADM, XDLC_BAUCUN, "| Planification | Parcours | Du BP | au BP | Itineraire prioritaire | Date de validite | Date de création | Itineraire 1 | Itineraire 2 | Itineraire 3 |"},
  { XDL_GES19_InfoG, 		XDLC_SITEADM, XDLC_BAUCUN, "| Planification | Gare | Telephone CFF | Telephone ST | Delai d'attente | Delai de tolerance | Temps de rotation | Temps de tolerance | Vue de la gare | Numero de la liste | Description | Type d'enclenchement | Vehicule | Wagon | Citerne | TES | Date de validite | Date de création | Voie Aiguillage initial droit Aiguillage initial gauche Aiguillage final droit Aiguillage final gauche Longueur voie droite Longueur voie gauche | Fonction Telephone CFF PTT NATEL | Bus Texte Temps | Texte libre |"},
  { XDL_GES20_Secteur, 		XDLC_SITEADM, XDLC_BAUCUN, "| Planification | Region  | Type | Date de validite | Date de création |"},
  { XDL_GES21_SsSecteur, 	XDLC_SITEADM, XDLC_BAUCUN, "| Planification | Sous-region  | Type | Region  | Seuil inferieur | Ecart inferieur | Seuil superieur | Ecart superieur | Seuil intermediaire | Ecart intermediaire | Telephone CFF | Telephone ST | Ressource | Date de validite | Date de création |"},
  { XDL_GES22_Image, 		XDLC_SITEADM, XDLC_BAUCUN, "| Planification | Type de synoptique | Synoptique | Site | Commentaire | Date de validite | Date de création | Sous-region  Priorite |"},
  { XDL_GES23_Occup, 		XDLC_SITEADM, XDLC_BAUCUN, "| Planification | DABS du DP | au DP | Direction | Element de temps | Modus | Prevision | Voie MIGRAZUR | Temps | Date de validite | Date de création |"},
  { XDL_GES24_ConvBP, 		XDLC_SITEADM, XDLC_BAUCUN, "| Planification | DP | Direction | Modus | Numero de voie | Prevision | Temps | Date de validite | Date de création |"},
  { XDL_GES25_ConvEP, 		XDLC_SITEADM, XDLC_BAUCUN, "| Planification | EP | Direction | Modus | Prevision | Temps | Date de validite | Date de création |"},
  { XDL_GES26_CatPrev, 		XDLC_SITEADM, XDLC_BAUCUN, "| Planification | Prevision | Train | Vitesse maximale de | a | Rapport de freinage de | a | Charge remorquee de | a | Date de validite | Date de création | Genre de train | Sensibilite repetition Sensibilite d'ecart |"},
  { XDL_GES27_CatTrain, 	XDLC_SITEADM, XDLC_BAUCUN, "| Planification | Train | Description en francais | Description en allemand | Date de validite | Date de création |"},
  { XDL_GES28_GenreTrain, 	XDLC_SITEADM, XDLC_BAUCUN, "| Planification | Train | Filtre absence de train | Temps d'arrêt | Locomotive | Description en francais | Description en allemand | Date de validite | Date de création |"},
  { XDL_GES29_Utilisateur,	XDLC_SITEADM, XDLC_BAUCUN, "| Planification | Utilisateur | Classe | Langue | Nom | Prenom | Section | Telephone | Bureau | Lieu de travail | Date de validite | Date de création |"},
  { XDL_GES30_Classe, 		XDLC_SITEADM, XDLC_BAUCUN, "| Planification | Classe | Date de validite | Date de création | Fonction |"},
  { XDL_GES31_Fonction,		XDLC_SITEADM, XDLC_BAUCUN, "| Planification | Fonction MIGRAZUR | Fichier d'aide en francais | Fichier d'aide en allemand | Date de validite | Date de création |"},
  { XDL_GES32_Justif, 		XDLC_SITEADM, XDLC_BAUCUN, "| Planification | Justification | Libelle en francais | Libelle en allemand | Date de validite | Date de création |"},
  { XDL_GES33_Annuaire, 	XDLC_SITEADM, XDLC_BAUCUN, "| Planification | Fonction | Telephone CFF | Telephone PTT | Telephone NATEL | Date de validite | Date de création | BP |"},
  { XDL_GES34_MsgCode, 		XDLC_SITEADM, XDLC_BAUCUN, "| Planification | Message | Libelle en francais | Libelle en allemand | Date de validite | Date de création |"},
  { XDL_GES35_SecRadio, 	XDLC_SITEADM, XDLC_BAUCUN, "| Planification | Region  | Sous-region  | Date de validite | Date de création |"},
  { XDL_GES36_TronRadio, 	XDLC_SITEADM, XDLC_BAUCUN, "| Planification | Troncon | Region  | Rang | Priorite | Date de validite | Date de création | DABS du DP au DP |"},
  { XDL_GES37_ParamFen, 	XDLC_SITEADM, XDLC_BAUCUN, "| Planification | Region  | Genre de liste | Position en X | Position en Y | Largeur | Hauteur | Ecran d'apparition | Date de validite | Date de création |"},
  { XDL_GES38_SAINUMINC, 	XDLC_SITEADM, XDLC_BAUCUN, "Veuillez saisir le numéro d'incident ou le code de justification, SVP"},
  { XDL_GES39_LISTEIMPRIME,	XDLC_SITEADM, XDLC_BAUCUN},
  { XDL_GES40_FINLISTETDA,	XDLC_SITEADM, XDLC_BAUCUN, "Fin de la table des données à activer"},
  { XDL_GES41_TDA_DISPARU,      XDLC_SITEADM, XDLC_BAUCUN, "La planification a disparu de la table des données à activer"},
  { XDL_GES42_DEJA_TDA, XDLC_SITEADM, XDLC_BAUCUN, "La planification existe déjà en table des données à activer"},
  { XDL_GES43_DATE_INV, XDLC_SITEADM, XDLC_BAUCUN, "La date est invalide"},
  { XDL_GES44_BD_DISPARU,       XDLC_SITEADM, XDLC_BAUCUN, "La donnée a disparu de la base de données"},
  { XDL_GES45_DEJA_BD,		XDLC_SITEADM, XDLC_BAUCUN, "La donnée existe déjà dans la base de données"},
  { XDL_GES46_MAC_NON_EXIS,	XDLC_SITEADM, XDLC_BAUCUN, "Cette machine n'existe pas"},
  { XDL_GES47_SAIS_TOUS_CHAMPS, XDLC_SITEADM, XDLC_BAUCUN, "Saisissez tous les champs, SVP"},
  { XDL_GES48_BASC_SD_IMPOSS,	XDLC_SITEADM, XDLC_BAUCUN, "Basculement du serveur de données impossible"},
  { XDL_GES49_BASC_SC_IMPOSS,  XDLC_SITEADM, XDLC_BAUCUN, "Basculement du serveur de communication impossible"},
  { XDL_GES50_ARR_SSYS_IMPOSS, XDLC_SITEADM, XDLC_BAUCUN, "Arrêt du sous-systeme impossible"},
  { XDL_GES51_LAN_SSYS_IMPOSS, XDLC_SITEADM, XDLC_BAUCUN, "Lancement du sous-systeme impossible"},
  { XDL_GES52_ARR_MIGRAZUR_IMPOSS, XDLC_SITEADM, XDLC_BAUCUN, "Arrêt de MIGRAZUR impossible"},
  { XDL_GES53_LAN_MIGRAZUR_IMPOSS, XDLC_SITEADM, XDLC_BAUCUN, "Lancement de MIGRAZUR impossible"},
  { XDL_GES54_ARRMAC1_IMPOSS,	XDLC_SITEADM, XDLC_BAUCUN, "Arrêt de la machine avec relance impossible"},
  { XDL_GES55_ARRMAC2_IMPOSS,  XDLC_SITEADM, XDLC_BAUCUN, "Arrêt de la machine sans relance impossible"},
  { XDL_GES56_FONC_NON_EXEC,	XDLC_SITEADM, XDLC_BAUCUN, "Fonction non executée"},
  { XDL_GES57_FONC_PAS_IMPL, 	XDLC_SITEADM, XDLC_BAUCUN, "Fonction pas encore implementée"},
  { XDL_GES58_COMM_TRAC_IMPOSS,	XDLC_SITEADM, XDLC_BAUCUN, "Commande du niveau de trace impossible"},
  { XDL_GES62_CHAMP_OBLIG,	XDLC_SITEADM, XDLC_BAUCUN, "Les champs obligatoires du masque ne sont pas renseignés"},
  { XDL_GES63_INTERV_FAUX,	XDLC_SITEADM, XDLC_BAUCUN, "Cette valeur doit etre comprise entre %d et %d"},
  { XDL_GES64_ACHEV,	XDLC_BAUCUN, XDLC_BAUCUN, "Achevement de la fiche operateur du site %s par le CI"},

  /* Messages et alarmes du Sous-Systeme ARCHIV */
  { XDL_ARC01_INSRBAN, XDLC_LOCAL, XDLC_BAUCUN, "Inserer bande no %s dans périphérique %s."},
  { XDL_ARC02_ERRABAN, XDLC_SITEEXP, XDLC_BAUCUN, "Erreur acces bande no %s. Verifier le périphérique et la bande. Inserer bande no %s."},
  { XDL_ARC03_LANCARC, XDLC_SITEADM, XDLC_BAUCUN, "--%s --Lancement archivage hebdomadaire sur le long terme uniquemement au CI de Mandelieu. Accés refusé."},
  { XDL_ARC04_RESSOCC, XDLC_SITE, XDLC_BAUCUN, "-- %s --Ressources occupées. Archivage hebdomadaire momentanément impossible."},
  { XDL_ARC05_DEMAARC, XDLC_DAUCUN, XDLC_BAUCUN, "Demande d'archivage effectuée sur le long terme en date du %s. Prochaine demande dans %s jours."},
  { XDL_ARC06_BANDEHS, XDLC_DAUCUN, XDLC_BAUCUN, "La bande inserée dans le périphérique %s n'est pas la bonne. Verifiez et insérez la bande no %s."},
  { XDL_ARC07_BANDCAP, XDLC_DAUCUN, XDLC_BAUCUN, "Placer une bande de plus grande capacité. Modifier le fichier de formatage."},
  { XDL_ARC08_RESSOCC, XDLC_DAUCUN, XDLC_BAUCUN, "Ressources occupées. Désarchivage impossible. Refaite une demande ultérieurement."},
  { XDL_ARC09_GRAFNAR, XDLC_DAUCUN, XDLC_BAUCUN, "Le graphique horaire %s n'est pas archivé de %s à %s. Demande de reconstruction refusée."},
  { XDL_ARC10_GRAFHHS, XDLC_DAUCUN, XDLC_BAUCUN, "Le graphique horaire %s est inaccessible sur les bandes d'archivage de %s ä %s. Demande de reconstruction interrompue."},
  { XDL_ARC11_RECONHS, XDLC_DAUCUN, XDLC_BAUCUN, "Les données de la plage de %s au %s pour le graphique horaire %s ne peuvent pas être extraites. Demande de reconstruction interrompue."},
  { XDL_ARC12_CREAGRA, XDLC_DAUCUN, XDLC_BAUCUN, "Création des fichiers pour la reconstruction du graphique horaire %s sur la plage de %s à %s impossible. Demande de reconstruction interrompue."},

  /* Messages et alarmes du Sous-Systeme CONFIG */
  { XDL_CON01_SGBDAHS, XDLC_BASC, XDLC_BAUCUN, "SGBDR actif %s indisponible. Basculement demande."},
  { XDL_CON02_SGBDPHS, XDLC_DAUCUN, XDLC_BAUCUN, "SGBDR passif %s indisponible."},
  { XDL_CON03_TCIMP01, XDLC_DAUCUN, XDLC_BAUCUN, "Veuillez activer d'abord, la donnée\n    %s\n a été nouvellement modifiée."},
  { XDL_CON04_TCIMP02, XDLC_DAUCUN, XDLC_BAUCUN, "Une nouvelle mise en service de la donnée\n     %s\n avant la précédente n'est pas possible."},
  { XDL_CON05_TCIMP03, XDLC_DAUCUN, XDLC_BAUCUN, "Avertissement: l'element\n       %s\n est-il replanifié (Valider)\n ou a-t-on oublié son activation (Annulation)."},
  { XDL_CON06_TCIMP04, XDLC_DAUCUN, XDLC_BAUCUN, "Avertissement: Le numero de voie de EP\n     %s\n est incohérent."},
  { XDL_CON07_SAUVBD_OK, XDLC_DAUCUN, XDLC_BAUCUN, "La sauvegarde de la Base de Données s'est terminée normalement"},
  { XDL_CON08_PBTAR, XDLC_DAUCUN, XDLC_BAUCUN, "Problème de sauvegarde de la Base de Données sur bande; verifiez le périphérique, SVP"},
  { XDL_CON09_PBAUTRE, XDLC_DAUCUN, XDLC_BAUCUN, "Problème de sauvegarde de la base."},
  { XDL_CON20_TCIMP_DEB, XDLC_DAUCUN, XDLC_BAUCUN, "INFO : D\151but des importations DfA/ZLR"},
  { XDL_CON21_TCIMP_OK, XDLC_DAUCUN, XDLC_BAUCUN, "INFO : Fin correcte des importations DfA/ZLR"},
  { XDL_CON22_TCIMP_NOK, XDLC_DAUCUN, XDLC_BAUCUN, "ALERTE : Probl\150me grave - Abandon"},
  { XDL_CON23_TCIMP_SQL, XDLC_DAUCUN, XDLC_BAUCUN, "ALERTE : Probl\150me connection base - Abandon"},
  { XDL_CON24_TCIMP_EXISTE, XDLC_DAUCUN, XDLC_BAUCUN, "ALERTE : Importation d\151j\140 en cours - Abandon"},
  { XDL_CON30_TCACT_DEB, XDLC_DAUCUN, XDLC_BAUCUN, "INFO : Debut de l'activation des données"},
  { XDL_CON31_TCACT_FIN, XDLC_DAUCUN, XDLC_BAUCUN, "INFO : Fin correcte de l'activation des données"},
  { XDL_CON32_TCACT_NOK, XDLC_DAUCUN, XDLC_BAUCUN, "FATAL : Problème grave - Abandon"},
  { XDL_CON33_TCACT_NDB, XDLC_DAUCUN, XDLC_BAUCUN, "FATAL : Problème connection base - Abandon"},
  { XDL_CON34_TCACT_SQL, XDLC_DAUCUN, XDLC_BAUCUN, "FATAL : Problème SQL"},
  { XDL_CON35_TCACT_EXISTE, XDLC_DAUCUN, XDLC_BAUCUN, "FATAL : Activation deja en cours - Abandon"},
  { XDL_CON36_TCACT_TRFERR, XDLC_DAUCUN, XDLC_BAUCUN, "FATAL : Problème de transfert du fichier HISTORIQUE !"},
  { XDL_CON37_TCACT_FICERR, XDLC_DAUCUN, XDLC_BAUCUN, "FATAL : Problème d'ouverture du fichier HISTORIQUE !"},
  { XDL_CON38_TCACT_DAYERR, XDLC_DAUCUN, XDLC_BAUCUN, "FATAL : Problème grave"},
  { XDL_CON39_TCACT_CARERR, XDLC_DAUCUN, XDLC_BAUCUN, "FATAL : Problème de création ou de transfert du fichier CARTO !"},

  /* Messages et alarmes du Sous-Systeme ALGORI */
  { XDL_ALG01_Incorrect, XDLC_GLOBAL, XDLC_BAUCUN, "Réception de message ZSE incorrect. Texte reçu: %s"},
  { XDL_ALG02_Incoherent, XDLC_GLOBAL, XDLC_BAUCUN, "Réception de message %s incorrect. Champ %s incoherent."},
  { XDL_ALG03_Conversion, XDLC_GLOBAL, XDLC_BAUCUN, "Problème dans la conversion de %s. Données rejetées."},
  { XDL_ALG04_Valable, XDLC_GLOBAL, XDLC_BAUCUN, "La Donnée %s n'est pas valable dans la système."}

} ;

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Permet de recuperer le texte de l'alarme 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XDL_01SystemeAlarme (int 			va_Numero, 
			 XDLT_SystemeAlarme*	pa_SystemeAlarme)

/*
* ARGUMENTS EN ENTREE :
*   va_Numero : Numero du texte 
*
*
* ARGUMENTS EN SORTIE :
*   pa_SystemeAlarme : Structure de l'alarme 
*
*
* CODE RETOUR : 
*   XDC_NOK	: Problème recuperation alarme
*   XDC_OK      : Message d'alarme recupere
*
* CONDITION D'UTILISATION
*   Aucune
*
* FONCTION 
*   Verifie que le numero de l'alarme est connu , puis parcours
*	le tableau et recupe le libelle du message
*
------------------------------------------------------*/
{
	static char *version="$Id: xdl.c,v 1.28 2017/04/25 15:31:09 devgfi Exp $ : XDL_01SystemeAlarme " ;
  /* Variables Locales */
  register int 		vl_valeurRetour = XDC_NOK;
  register int		vl_indice = 0;

	XZST_03EcritureTrace (XZSTC_INTERFACE,"XDL_01SystemeAlarme : IN \
	-- Numero Alarme %d \n", va_Numero );

  /* si hors limites */
  if ((va_Numero < OFFSUP) || 
      (va_Numero > XDLC_MAXALRM))
  {
	XZST_03EcritureTrace (XZSTC_WARNING,
		" XDL_01   numero de texte hors limite %d \n",va_Numero);
      vl_valeurRetour = XDC_NOK;
  }
  /* Si NUL ... */
  else if (pa_SystemeAlarme == (XDLT_SystemeAlarme*) NULL)
  {
	XZST_03EcritureTrace (XZSTC_WARNING,
		" XDL_01   pointeur sur alarme == NULL \n");
  	vl_valeurRetour = XDC_NOK;
  }
  /* Renvoi de la structure concernee  (acces indirect) */
  else
  {
      for (vl_indice=0; 
	   vl_indice < ((sizeof (tg_msgSysteme)) / (sizeof (XDLT_SystemeAlarme))); 
	   vl_indice++)
      { 
	XZST_03EcritureTrace (XZSTC_DEBUG3,
		"Recherche Msg Alarme %d -- %d \n",
		va_Numero,tg_msgSysteme [vl_indice].Id);
	if (va_Numero == tg_msgSysteme [vl_indice].Id)
	{
	    *pa_SystemeAlarme = tg_msgSysteme [vl_indice];
	    vl_valeurRetour = XDC_OK;
	    break;
	}
      }
  }

	XZST_03EcritureTrace (XZSTC_INTERFACE,"XDL_01SystemeAlarme : OUT \
	-- Id %d  Ind Diff %d -- IndBasc %d -- \
	-- Format %s \n",pa_SystemeAlarme->Id,
	pa_SystemeAlarme->IndDiff,pa_SystemeAlarme->IndBasc,
	pa_SystemeAlarme->FormatFranc);

  	return (vl_valeurRetour);

}	/* Fin XDL_01... */
