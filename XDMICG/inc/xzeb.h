/*E*/
/* Fichier : $Id: xzeb.h,v 1.4 2004/05/06 14:25:07 gesconf Exp $     Release : $Revision: 1.4 $      Date :  $Date: 2004/05/06 14:25:07 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE xzeb  * FICHIER xzeb.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Structure des donnees du fichier xzeb.c >
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	15 Jan 1998	: Creation
* Mismer.D	version 1.2	27 Jan 1998	: Modif pour commande PAL (DEM/1539)
* JMG		1.3	27/04/04	PAL par scenario
------------------------------------------------------ */

#ifndef xzeb
#define xzeb

/* fichiers inclus */
#include "xdm.h"
#include "xdy.h"
#include "xdg.h"
#include "xzst.h"
#include "xzex.h"

/* definitions de constantes exportees */

/* definitions de types exportes */

/* Commande module PAL */
typedef struct {
        XDY_Eqt         Numero;                 	/* Numero d'equipement */
        char 		Scenario[7];    /* nom du scenario */
} XZEBT_CMD_PAL;			/* Commande module PAL */

		
/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande de commande PAL
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern 	int XZEB01_Commande_PAL (	XZEBT_CMD_PAL	va_Commande_PAL,
					int		va_NoCMD,
					XDY_NomMachine	pa_NomMachine );	

/*
* ARGUMENTS EN ENTREE :
*   va_Commande_PAL	Commande a effectuee
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
*   Doit etre invoquee sur validation de la fenetre de pilotage PAL. 
*
* FONCTION 
*   Construction du message XDM_ECMD_PAL en specifiant les valeurs recues.
*   Envoi du message sur le DataGroup XDG_ECMD_PAL_<NomMachine> du site concerne
*
------------------------------------------------------*/



#endif
