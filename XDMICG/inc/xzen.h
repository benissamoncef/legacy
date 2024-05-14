/*E*/
/* Fichier : $Id: xzen.h,v 1.12 2012/07/04 11:04:38 pc2dpdy Exp $      Release : $Revision: 1.12 $        Date : $Date: 2012/07/04 11:04:38 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE xzen  * FICHIER xzen.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Structure des donnees du fichier xzen.c >
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	25 Aug 1994	: Creation
* Mercier.O	version 1.2	21 Sep 1994	:
* Mercier.O	version 1.3	21 Sep 1994	:
* Mercier.O	version 1.4	22 Sep 1994	:
* Nagiel.E	version 1.5	16 Nov 1994	: maj des structures
* Nagiel.E	version 1.6	28 Nov 1994	:
* Nagiel.E	version 1.7	20 Dec 1994	:
* Mismer.D	version 1.8	23 Fev 1995	:
* Fontaine.C	version 1.9	24 Mai 1995	: Reprise entete, historique
* JMG		version 1.10	13/09/05	: ajout BAF BAD
* JMG		version 1.11	30/05/06	: definition type XZENT_CMD_SCEN_BAD
* VR		version 1.12	08/12/11 	: Ajout equipement PRV (DEM/1016)
------------------------------------------------------ */

#ifndef xzen
#define xzen

/* fichiers inclus */
#include "xdm.h"
#include "xdy.h"
#include "xdg.h"
#include "xzst.h"
/* definitions de constantes exportees */

#define XZENC_Lon_Matrice		XDC_Lon_Matrice
#define XZENC_Nb_Params_Matrice		XDC_Lon_Params_Matrice
#define XZENC_ACTIF   			'1'
#define XZENC_INACTIF 			'0'

/* definitions de types exportes */

/* Commande module NAV */
typedef struct	{
	XDY_Eqt		NoEqt;		/* Numero d'equipement  */
	XDY_Octet	Module;		/* Numero du module */
	char		Symb[4];	/* Symbole */
	XDY_Booleen	Flash;		/* Avec ou sans flash */
}	XZENT_CMD_NAV;			/* Commande module NAV */


/* Commande module NAV par scenario */
typedef struct	{
	XDY_Eqt		NoEqt;				/* Numero d'equipement  */
	char		Scenar[XDC_Lon_Scenario];	/* Nom du scenario */
}	XZENT_CMD_SCEN;					/* Commande module NAV par scenario */

/* Commande matrice Nav */
typedef struct	{
	XDY_Eqt			NoEqt;				/* Numero d'equipement  */
	char			Matrice[XZENC_Lon_Matrice];	/* Nom matrice */
	XDY_Mot			ListeParam[XZENC_Nb_Params_Matrice];
}	XZENT_CMD_MAT_NAV;

typedef struct	{
	XDY_Eqt		NoEqt;		/* Numero d'equipement  */
	XDY_Octet	Module;		/* Numero du module */
	XDY_Octet	Message;	/* message = 0 ou 1 */
	XDY_Booleen	Cligno;		/* Avec ou sans clignotement */
}	XZENT_CMD_BAF;			/* Commande module BAF */

typedef struct	{
	XDY_Eqt		NoEqt;		/* Numero d'equipement  */
	XDY_Octet	Module;		/* Numero du module */
	XDY_Octet	Message;	/* message = 0 ou 1 */
	XDY_Booleen	Cligno;		/* Avec ou sans clignotement */
}	XZENT_CMD_BAD;			/* Commande module BAF */

typedef struct	{
	XDY_Eqt		NoEqt;		/* Numero d'equipement  */
	XDY_VitesseRegul	Vitesse;	/* message = 0 ou 1 */
	XDY_Bandeau		Bandeau;
}	XZENT_CMD_PRV;			/* Commande module PRV */


/* Commande module BAD par scenario */
typedef struct	{
	XDY_Eqt		NoEqt;				/* Numero d'equipement  */
	char		Scenar[11];	/* Nom du scenario */
}	XZENT_CMD_SCEN_BAD;					/* Commande module NAV par scenario */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande de commande NAV
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern 	int XZEN01_Commande_NAV (	XZENT_CMD_NAV	va_Commande_NAV,
					int		va_NoCMD,
					XDY_NomMachine	pa_NomMachine );	

/*
* ARGUMENTS EN ENTREE :
*   va_Commande_NAV	Commande a effectuee
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
*
* CONDITION D'UTILISATION
*   Doit etre invoquee sur validation de la fenetre de pilotage NAV. 
*
* FONCTION 
*   Construction du message XDM_ECMD_NAV en specifiant les valeurs recues.
*   Envoi du message sur le DataGroup XDG_ECMD_NAV_<NomMachine> du site concerne
*
------------------------------------------------------*/


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande commande NAV par scenario
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern 	int XZEN02_Commande_SCEN (	XZENT_CMD_SCEN	va_Commande_SCEN,
					int		va_NoCMD,
					XDY_NomMachine	pa_NomMachine );	

/*
* ARGUMENTS EN ENTREE :
*   va_Commande_SCEN	Commande a effectuee
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
*
* CONDITION D'UTILISATION
*   Doit etre invoquee sur validation de la fenetre de pilotage NAV. 
*
* FONCTION 
*   Construction du message XDM_ECMD_SCEN en specifiant les valeurs recues.
*   Envoi du message sur le DataGroup XDG_ECMD_SCEN_<NomMachine> du site concerne
*
------------------------------------------------------*/

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande commande NAV par fichier scenario
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern 	int XZEN03_Config_Matrice_Test (XZENT_CMD_MAT_NAV	va_Commande_MAT,
					int			va_NoCMD,
					XDY_NomMachine		pa_NomMachine )	;

/*
* ARGUMENTS EN ENTREE :
*   va_Commande_SCEN_FICH	Commande a effectuee
*   va_NoCMD			Numero de la commande
*   va_NomMachine		Nom de la machine
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK		succes
*   XDC_NOK		echec 
*   XZEXC_ERR_EQT	Numero d'equipement invalide
*
* CONDITION D'UTILISATION
*   Doit etre invoquee sur validation de la fenetre de pilotage NAV. 
*
* FONCTION 
*   Construction du message XDM_ECMD_SCEN_FICH en specifiant les valeurs recues.
*   Envoi du message sur le DataGroup XDG_ECMD_SCEN_FICH_<NomMachine> 
*   du site concerne
*
------------------------------------------------------*/
/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande commande NAV par scenario
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern 	int XZEN04_Arret_SCEN (	XDY_Eqt		va_NoEqt,
				int		va_NoCMD,
				XDY_NomMachine	pa_NomMachine )	;	

/*
* ARGUMENTS EN ENTREE :
*   va_Commande_SCEN	Commande a effectuee
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
*
* CONDITION D'UTILISATION
*   Doit etre invoquee sur validation de la fenetre de pilotage NAV. 
*
* FONCTION 
*   Construction du message XDM_ECMD_SCEN en specifiant les valeurs recues.
*   Envoi du message sur le DataGroup XDG_ECMD_SCEN_<NomMachine> du site concerne
*
------------------------------------------------------*/


extern 	int XZEN10_Commande_BAF (	XZENT_CMD_BAF	va_Commande_BAF,
					int		va_NoCMD,
					XDY_NomMachine	pa_NomMachine );	
extern 	int XZEN11_Commande_BAD (	XZENT_CMD_BAD	va_Commande_BAD,
					int		va_NoCMD,
					XDY_NomMachine	pa_NomMachine );		
extern 	int XZEN14_Commande_PRV (	XZENT_CMD_PRV	va_Commande_PRV,
					int		va_NoCMD,
					XDY_NomMachine	pa_NomMachine );	
extern 	int XZEN13_Commande_SCEN_BAD (	XZENT_CMD_SCEN_BAD	 va_Commande_SCEN,
					int		va_NoCMD,
					XDY_NomMachine	pa_NomMachine );	

extern 	int XZEN15_Arret_SCEN_BAD (	XDY_Eqt		va_NoEqt,
				int		va_NoCMD,
				XDY_NomMachine	pa_NomMachine )	;	


extern 	int XZEN16_Commande_BAF_gloable (	XZENT_CMD_BAF	va_Commande_BAF[6],
					int		va_NoCMD,
					XDY_NomMachine	pa_NomMachine );	
#endif
