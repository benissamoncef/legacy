/*E*/
/*Fichier : $Id: xzep.h,v 1.18 2021/05/03 14:05:18 pc2dpdy Exp $      Release : $Revision: 1.18 $        Date : $Date: 2021/05/03 14:05:18 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE XZEP  * FICHIER xzep.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Structure des donnees du fichier xzep.c >
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	25 Aug 1994	: Creation
* Nagiel.E	version 1.2	04 Oct 1994	:
* Nagiel.E	version 1.3	04 Oct 1994	:
* Nagiel.E	version 1.4	04 Oct 1994	:
* Nagiel.E	version 1.5	04 Oct 1994	:
* Nagiel.E	version 1.6	04 Oct 1994	:
* Nagiel.E	version 1.7	10 Oct 1994	: Ajout XZEP02
* Nagiel.E	version 1.8	16 Nov 1994	:
* Mismer.D	version 1.9	14 Dec 1994	: Suppression XDF 
* Fontaine.C	version 1.10	24 Mai 1995	: Reprise entete, historique
* Mismer.D	version 1.11	28 Mar 1997	: Ajout destination TDP (DEM/1417) 
* jmg		version 1.12	13/09/05	: ajout PMVA 
* JPL		30/11/10	: Migration architecture HP ia64 (DEM 961) : prototype XZEP05 : declaration de type  1.13
* VR		08/12/11	: Ajout equipement PRV (DEM/1016)
* JMG		16/04/16	: ajout SAGA 1.15 DEM1191
* LCL		22/02/18	: ajout CTRL_FEUX 1.16 DEM1284
* CGR		07/12/20	: ajout IMU 1.17 DEM-SAE155
* ABE		02/04/21	: ajout PAU SONO 1.18 DEM-SAE244
* GGY		14/11/23	: Ajout PIC (DEM483)
------------------------------------------------------ */
#ifndef XZEP
#define XZEP

/* fichiers inclus  */
#include <xdy.h>
#include <xdc.h>
#include "xzst.h"
#include "xdm.h"

/* definitions de constantes exportees */
#define XZEPC_SURB_EQTALL	0

/* definitions de types exportes */
typedef	XDY_Etat_PMV	XZEPT_CMD_PMV;
typedef	XDY_Etat_Picto	XZEPT_CMD_PICTO;
typedef	XDY_Etat_PMVA	XZEPT_CMD_PMVA;
typedef	XDY_Etat_PRV	XZEPT_CMD_PRV;
typedef	XDY_Etat_Eqt_SAGA	XZEPT_CMD_EQT_SAGA;
typedef	XDY_Etat_CFE	XZEPT_CMD_CFE;
typedef XDY_Etat_IMU	XZEPT_CMD_IMU;
typedef XDY_Etat_SONO	XZEPT_CMD_SONO;
typedef	XDY_Etat_PIC	XZEPT_CMD_PIC;

/* definition de macro exportees */
/* declarations de donnees globales */
/* delaration de fonctions externes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande d'affichage sur PMV
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern 	int XZEP01_Commande_PMV (	XZEPT_CMD_PMV	va_Commande_PMV,
					int		va_NoCMD,
					XDY_NomMachine	pa_NomMachine );	

/*
* ARGUMENTS EN ENTREE :
*   va_Commande_PMV	Commande a effectuee
*   va_NoCMD		Numero de la commande
*   va_NomMachine	nom de la machine
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK		succes
*   XDC_NOK		echec 
*   XZEXC_ERR_EQT	Numero d'equipement invalide
*   XZEXC_ERR_CLIGN	Type de clignotement invalide
*
* CONDITION D'UTILISATION
*   Doit etre invoquee sur validation de la fenetre de pilotage PMV. 
*
* FONCTION 
*   Construction du message XDM_ECMD_PMV en specifiant les valeurs recues.
*   Envoi du message sur le DataGroup XDG_ECMD_PMV_<NomMachine> du site concerne
*
------------------------------------------------------*/


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande d'affichage sur Picto
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern 	int XZEP02_Commande_Picto (	XZEPT_CMD_PICTO	va_Commande_PICTO,
					int		va_NoCMD,
					XDY_NomMachine	pa_NomMachine );	

/*
* ARGUMENTS EN ENTREE :
*   va_Commande_Picto	Commande a effectuee
*   va_NoCMD		Numero de la commande
*   va_NomMachine	nom de la machine
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK		succes
*   XDC_NOK		echec 
*   XZEXC_ERR_EQT	Numero d'equipement invalide
*   XZEXC_ERR_CLIGN	Type de clignotement invalide
*   XZEXC_ERR_PICTO	Symbole picto invalide
*
* CONDITION D'UTILISATION
*   Doit etre invoquee sur validation de la fenetre de pilotage PMV. 
*
* FONCTION 
*   Construction du message XDM_ECMD_Picto en specifiant les valeurs recues.
*   Envoi du message sur le DataGroup XDG_ECMD_Picto_<NomMachine> du site concerne
*
------------------------------------------------------*/


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande rafraichissement affichage TDP sur PMV
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int XZEP03_Commande_Raf_TDP_PMV(	XZEPT_CMD_PMV	va_Commande_PMV,
					int		va_NoCMD,
					XDY_NomMachine	pa_NomMachine );	

/*
* ARGUMENTS EN ENTREE :
*   va_Commande_PMV	Commande a effectuee
*   va_NoCMD		Numero de la commande
*   va_NomMachine	nom de la machine
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK		succes
*   XDC_NOK		echec 
*   XZEXC_ERR_EQT	Numero d'equipement invalide
*   XZEXC_ERR_CLIGN	Type de clignotement invalide
*
* CONDITION D'UTILISATION
*   Doit etre invoquee sur changement duree afficha temps de parcours (TETDP). 
*
* FONCTION 
*   Construction du message XDM_ECMD_PMV en specifiant les valeurs recues.
*   Envoi du message sur le DataGroup XDG_ECMD_PMV_<NomMachine> du site concerne
*
------------------------------------------------------*/

extern  int XZEP04_Commande_PMVA (       XZEPT_CMD_PMVA   va_Commande_PMVA,
                                        	int             va_NoCMD,
                                        	XDY_NomMachine  pa_NomMachine );

extern int XZEP05_Commande_Raf_TDP_PMVA ( 	XZEPT_CMD_PMVA	va_Commande_PMV,
						int		va_NoCMD,
						XDY_NomMachine	pa_NomMachine )	;	

						
extern  int XZEP06_Commande_PRV (	XZEPT_CMD_PRV	va_Commande_PRV,
						int		va_NoCMD,
						XDY_NomMachine	pa_NomMachine);

extern  int XZEP104_Commande_Eqt_SAGA (       XZEPT_CMD_EQT_SAGA   va_Commande_Saga,
						int             va_NoCMD,
					XDY_NomMachine  pa_NomMachine );

extern int XZEP07_Commande_CFE ( 	XZEPT_CMD_CFE		va_Commande_CFE,
						int		va_NoCMD,
						XDY_NomMachine	pa_NomMachine );

extern int XZEP08_Commande_IMU (	XZEPT_CMD_IMU		va_Commande_IMU,
						int		va_NoCMD,
						XDY_NomMachine	pa_NomMachine );
								
extern int XZEP09_Commande_RAU_SONO (	XZEPT_CMD_SONO		va_Commande_SONO,
						int		va_NoCMD,
						XDY_NomMachine	pa_NomMachine );

extern  int XZEP10_Commande_PIC (	XZEPT_CMD_PIC	va_Commande_PIC,
						int		va_NoCMD,
						XDY_NomMachine	pa_NomMachine);

#endif
