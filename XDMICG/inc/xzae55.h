/*E*/
/*Fichier : $Id: xzae55.h,v 1.2 2017/03/31 17:24:19 pc2dpdy Exp $     Release : $Revision: 1.2 $        Date : $Date: 2017/03/31 17:24:19 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAE55 * FICHIER XZAE55.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale >
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG   08/02/16 creation SAGA 1.0
------------------------------------------------------ */

#ifndef XZAE55
#define XZAE55

	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include <memory.h>
	
	/* Include Sybase */
	#include <ctpublic.h>
	#include <cstypes.h>

	/* Include applicatif Migrazur */
	#include "xdy.h"
	#include "xdc.h"
	#include "xzae.h"
	#include "xzag.h"
	#include "asql_com.h"

/* definitions de constantes exportees */
#define XZAE55C_RPC_NAME "XZAE;55"

#define XZAE55C_NB_PARM_RETURN 1


/* definitions de types exportes */



/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int XZAE55_Ecrire_Fiche_Vehicule_Lent (	XDY_Entier	va_numero_in ,
					XDY_Octet	va_cle_in,
					XDY_Horodate	va_Horodate_in,
					XDY_Mot		va_Longueur_in,
					XDY_Octet	va_Vitesse_in,
					XDY_Mot		va_Hauteur_in,
					XDY_Mot	va_Largeur_in,
					XDY_Mot		va_Poids_in,
					char		*va_Contenu_in,
					XDY_Mot	va_PosteOperateur_in,
					XDY_Octet		va_Prevision_Traiter_Clos_in);

/*
*
* PARAMETRES EN ENTREE :
*
* XDY_Eqt		va_numero_in
* XDY_Sens		va_cle_in
*				
* PARAMETRES EN SORTIE :
*
* VALEUR RENDUE :
*
* 
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK 
*
* CONDITIONS D'UTILISATION :
*

* FONCTION :
*
*
*
* MECANISMES :
*
------------------------------------------------------*/
#endif
