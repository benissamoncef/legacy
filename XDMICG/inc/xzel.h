/*E*/
/*  Fichier : @(#)xzel.h	1.3      Release : 1.3        Date : 12/07/10
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE XZEL  * FICHIER xzel.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Structure des donnees du fichier xzel.c >
*
------------------------------------------------------
* HISTORIQUE :
*
* Nouaille.M	version 1.1	27 Oct 1994	: Creation
* Fontaine.C	version 1.2	24 Mai 1995	: Reprise entete
* JMG	22/11/10	version 1.3	ajout radio/tel
* SDU		10/02/12 : DEM 1015 -Communautaire
* JMG		14/04/12 : DEM 1016 regulation
* JMG	13/12/16 : ajout XZEL14 SAGA conf scenario  1.6 DEM1191
* JMG	19/06/18 : GABEB DEM1283
* JMG	12/06/19  : ajout XZEL23 ODI_C DEM1338 1.8
* JMG	12/01/20	: ajout IMU 1.9
------------------------------------------------------ */

#ifndef XZEL
#define XZEL

/* fichiers inclus  */

#include "xdc.h"
#include "xdm.h"
#include "xdy.h"
#include "xdg.h"
#include "xzec.h"

#include "xzst.h"

/* definitions de constantes exportees */

/* definitions de types exportes */
typedef XDY_Etat_IMU	XZELT_CMD_IMU;


/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */
/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande d'appel d'un intervenant par telephone 
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZEL01_Demande_Appel_Tel(XDY_NomSite 	va_nomSite,
                             XDY_Tel		va_NumTel,
                             XDY_Operateur	va_NumPO);
                             

/*
* ARGUMENTS EN ENTREE :
* va_nomSite : nom du site pour construire le datagroup
* va_NumTel : no de telephone a appeler
* va_NumPO : no du poste operateur demandeur
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK			succes
*   XDC_NOK			echec : Erreur envoi message datagroup
*   XDCC_ERR_POSTE_OP		echec : Numero poste oprateur invalide 
*
* CONDITION D'UTILISATION
*    
*
* FONCTION 
*   Demande d'appel d'un intervenant par telephone 
*
------------------------------------------------------*/


int XZEL02_Demande_Renvoi_Appel(XDY_NomSite 	va_nomSite,
                             XDY_Tel		va_NumRepondeur,
                             XDY_Operateur	va_NumPO);
/*
* ARGUMENTS EN ENTREE :
* va_nomSite : nom du site pour construire le datagroup
* va_NumRepondeur : no de repondeur a appeler
* va_NumPO : no du poste operateur demandeur
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK			succes
*   XDC_NOK			echec : Erreur envoi message datagroup
*   XDCC_ERR_POSTE_OP		echec : Numero poste operateur invalide 
*
* CONDITION D'UTILISATION
*    
*
* FONCTION 
*   Demande basculement sur repondeur
*
------------------------------------------------------*/  



int XZEL03_Annul_Renvoi(     XDY_NomSite 	va_nomSite,
                             XDY_Operateur	va_NumPO);
/*                             
* ARGUMENTS EN ENTREE :
* va_nomSite : nom du site pour construire le datagroup
* va_NumPO : no du poste operateur demandeur
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK			succes
*   XDC_NOK			echec : Erreur envoi message datagroup
*   XDCC_ERR_POSTE_OP		echec : Numero poste operateur invalide 
*
* CONDITION D'UTILISATION
*    
*
* FONCTION 
*   Demande annulation renvoi sur repondeur
*
------------------------------------------------------*/                                      


int Envoyer_Mess_Tel(	XDY_NomSite 	va_nomSite,
			int		va_codecmd,
                        XDY_Tel		va_NumTel,
                        XDY_Operateur	va_NumPO);
/*
* ARGUMENTS EN ENTREE :
* va_nomSite : nom du site 
* va_codecmd : no de commande
* va_NumTel :  no de telephone
* va_NumPO :   no du poste Operateur appelant
*
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK		succes
*   XDC_NOK		echec 
*
* CONDITION D'UTILISATION
*    
*
* FONCTION 
*   Envoie le message XDM_ECMD_AUTOTEL sur le 
*   datagroup XDG_ECMD_AUTOTEL_<nomSite>
*
------------------------------------------------------*/


int XZEL04_Mise_En_Garde (
       XDY_NomSite va_nomSite,
       XDY_Operateur va_NumPO);

int XZEL05_Reprise  (
       XDY_NomSite va_nomSite,
       XDY_Operateur va_NumPO);

int XZEL06_Demande_Appel_Radio (
       XDY_NomSite va_nomSite,
       XDY_TypeAppelRadio va_typeAppel, 
       XDY_GroupeRadio va_numeroGroupeRadio, 
       XDY_Radio va_numeroCibleAppel,
       XDY_Operateur va_NumPO );

int Envoyer_Mess_Rad (
       XDY_NomSite va_nomSite,
       XDY_TypeAppelRadio va_typeAppel, 
       XDY_GroupeRadio va_numeroGroupeRadio, 
       XDY_Radio va_numeroCibleAppel,
       XDY_Operateur va_NumPO) ;

int XZEL10_Acquitter_Communautaire (
       XDY_Entier idComm);

int XZEL11_Rapprocher_Communautaire (
       XDY_Entier idComm, 
       XDY_Evt idFMC,
       char *va_type);

int XZEL12_Ignorer_Communautaire (
       XDY_Entier idComm, 
       XDY_Octet Motif);
int XZEL13_Acquitter_Alerte_Regulation (
	char *va_Site,
	char	*va_Texte,
	int va_zone,
	int va_qualif, int va_numero);

int XZEL14_Acquitter_Alerte_Scenario_SAGA (
	char    *va_Texte,
	int va_qualif);
int XZEL15_Acquitter_Alerte_GABEB (
        char *va_Site,
        char    *va_Texte,
        int va_zone,
        int va_qualif, int va_numero);

int XZEL23_SMS(char *, XDY_Octet, char *, char *, char *);

int XZEL24_Commande_IMU(XZELT_CMD_IMU, int , XDY_NomMachine);
#endif
