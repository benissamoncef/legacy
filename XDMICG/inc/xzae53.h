/*E*/
/*Fichier : $Id: xzae53.h,v 1.2 2017/03/31 17:22:47 pc2dpdy Exp $     Release : $Revision: 1.2 $        Date : $Date: 2017/03/31 17:22:47 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAE53 * FICHIER XZAE53.h
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

#ifndef XZAE53
#define XZAE53

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
#define XZAE53C_RPC_NAME "XZAE53"

#define XZAE53C_NB_PARM_RETURN 1


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

extern int XZAE53_Ecrire_Fiche_Accident (	XDY_Entier	va_numero_in ,
					XDY_Octet	va_cle_in,
					XDY_Octet	va_VL_in,
					XDY_Octet	va_PL_in,
					XDY_Octet	va_Remorque_in,
					XDY_Octet	va_Car_in,
					XDY_Octet	va_Moto_in,
					XDY_Octet	va_BlesseLeg_in,
					XDY_Octet	va_BlesseGra_in,
					XDY_Octet	va_Mort_in	,
					XDY_Booleen	va_VehEscota_in,
					XDY_Booleen	va_MatDang_in,
					XDY_Booleen	va_HomEscota_in,
					XDY_Booleen	va_Personalite_in,
					XDY_Booleen	va_DegatsDom_in,
					XDY_Entier	va_PosteOperateur_in,
					XDY_Horodate	va_Validation_in,
					XDY_Mot	va_Cond_Meteo_in,
					XDY_Mot	va_Cond_Visibilite_in,
					XDY_Mot	va_Cond_Etat_Chau_in,
					XDY_Mot	va_Cond_Chantier_in);

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
