/*E*/
/*Fichier : $Id: xzae140.h,v 1.2 1997/12/03 16:39:56 verdier Exp $      Release : $Revision: 1.2 $        Date : $Date: 1997/12/03 16:39:56 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAE140 * FICHIER XZAE140.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale >
*
------------------------------------------------------
* HISTORIQUE :
*
* Vathelot	12 Mai 1997	: Creation (dem/strada) 1.1
------------------------------------------------------ */

#ifndef XZAE140
#define XZAE140

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
#define XZAE140C_RPC_NAME "XZAE140"

#define XZAE140C_NB_PARM_RETURN 8


/* definitions de types exportes */



/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*  XZAE140_Infos_obligatoires : recupere les infos 
*  necessaires d un fichier evt strada
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int XZAE140_Lire_Fiche_Vehicule_Pour_Strada (    XDY_Entier      va_numero_in ,
                                        XDY_Octet       va_cle_in,
                                        XDY_Octet       *va_type_out,
                                        XDY_Mot         *va_volume_out,
                                        char            *va_matiere_out,
                                        XDY_Mot         *va_bebes_out,
                                        XDY_Mot         *va_agees_out,
                                        XDY_Booleen     *va_animaux_out,
                                        XDY_Booleen     *va_escota_out,
                                        char            *va_observations_out);


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
