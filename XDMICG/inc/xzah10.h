/*E Fichier : $Id: xzah10.h,v 1.4 1996/09/09 09:14:18 gaborit Exp $      Release : $Revision: 1.4 $        Date : $Date: 1996/09/09 09:14:18 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAH10 * FICHIER XZAH10.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale >
*
------------------------------------------------------
* HISTORIQUE :
*
* mercier	27 Oct 1994	: Creation
******************************
* mercier	3  Nov 1994	: Ajout 		V 1.2
*	de xzah03 xzah04 xzah05
******************************
* Torregrossa	20 fev 1995	: Ajout constantes	V 1.3
* Niepceron	26 Aou 1996	: Ajout constante XZAH_TRAIT_BLOC v1.4
------------------------------------------------------ */

#ifndef XZAH10
#define XZAH10

/* fichiers inclus */
	/* Include systeme */
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
	#include "xzag.h"
	#include "asql_com.h"
	#include "xzis.h"
	#include "xzah01.h"
	#include "xzah07.h"
	#include "xzah08.h"

/* definitions de constantes exportees */

#define XZAH10C_XZAH30_RPC_NAME		"XZAH;30"
#define XZAH10C_XZAH30_NB_PARAM_RETURN	0

#define XZAH_TRAIT_BLOC 	XDC_TRAIT_BLOC

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

XDY_Booleen XZAH10G_Init;


/* delaration de fonctions externes */
/*------------------------------------------------------
* SERVICE RENDU : 
*	**  Si XZAH10G_Init reste a XDC_FAUX la BaseDD rafraichit que les delta sur les synoptiques.
	**  sinon c est un rafraichissement de type initialisation. 
	**  Pour que l animation synoptique soit de type initialisation
	**  il faut que XZAH11 soit appelee avant XZAH10 ou que XZAH10 soit 
	**  a l initialisation de migrazur.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int XZAH10_Animer_IHM( );

/*
*
* PARAMETRES EN ENTREE : aucun
* PARAMETRES EN SORTIE : aucun
* VALEUR RENDUE :
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK 
*
* CONDITIONS D'UTILISATION :
*	Rtserver en etat de marche, SQL Serveur en etat de marche avec les SP connus.
* INTERFACE :
*
* MECANISMES :
*
------------------------------------------------------*/


#endif
