/*E*/
/*Fichier : $Id: xzae137.h,v 1.6 2019/04/23 11:18:51 pc2dpdy Exp $      Release : $Revision: 1.6 $        Date : $Date: 2019/04/23 11:18:51 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAE137 * FICHIER XZAE137.h
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
* Niepceron	16 MAr 1997	: recup des libelle de type dem/1741 v1.2
* Niepceron     17/01/2007	: Ajout arg pour PX-METACOR v1.4 DEM605
* JMG		08/03/18 : ajout vitesse 1.6
------------------------------------------------------ */

#ifndef XZAE137
#define XZAE137

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
#define XZAE137C_RPC_NAME "XZAE137"

#define XZAE137C_NB_PARM_RETURN 13


/* definitions de types exportes */



/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*  XZAE137_Infos_obligatoires : recupere les infos 
*  necessaires d un fichier evt strada
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int XZAE137_Lire_Fiche_Travaux_Pour_Strada (	XDY_Entier	va_numero_in ,
					XDY_Octet	va_cle_in,
					XDY_Booleen	*va_nature_out,
					char		*va_type_out,
					XDY_Booleen	*va_mobile_out,
					XDY_Entier	*va_longueur_out,
					XDY_Octet	*va_type_loca_out,
					char 		*va_nom_loca_out,
					int 		*va_num_loca_out,
					char		*va_ConfigVoies_out,
					char		*va_ConfigVoiesSuite_out);

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
