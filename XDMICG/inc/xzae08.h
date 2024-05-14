/*E*/
/*Fichier : $Id: xzae08.h,v 1.2 2019/01/29 11:27:20 pc2dpdy Exp $     Release : $Revision: 1.2 $        Date : $Date: 2019/01/29 11:27:20 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAE08 * FICHIER XZAE08.h
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
* JMG	13/06/18 ajout num alerte 1.1
------------------------------------------------------ */

#ifndef XZAE08
#define XZAE08

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
#define XZAE08C_RPC_NAME "XZAE08"

#define XZAE08C_NB_PARM_RETURN 3


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

extern int XZAE08_Creer_Fiche (	XDY_Octet	va_TypeEvenement_in ,
					XDY_Octet	va_MachineOperateur_in,
					XDY_Booleen	va_Prevision_ATraiter_in,
					XDY_Horodate	va_HoroDebut_in,
					XDY_Horodate	va_HoroDebutPrevu_in,
					XDY_Octet	va_Site_creation_in,
					XDY_Octet	va_Degrade_in,
					XDY_Entier	*va_NumEvenement_out,
					XDY_Octet	*va_CleEvenement_out,
					XDY_Entier	*va_NumFicheAppel_out,
					int		va_NumAlerte_in,
					XDY_Octet	va_SiteAlerte_in);

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
