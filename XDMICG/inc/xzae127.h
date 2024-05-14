/*E*/
/*Fichier : $Id: xzae127.h,v 1.8 2007/01/15 10:40:45 gesconf Exp $      Release : $Revision: 1.8 $        Date : $Date: 2007/01/15 10:40:45 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAE127 * FICHIER XZAE127.h
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
* Guilhou	02/12/97	: ajout zz en sortie 1.4
* Guilhou	02/12/97	: ajout sens en sortie 1.5
* Niepceron	23/12/98	: ajout niv de trafic et nb voies op. dem/1724 v1.6
* Niepceron     04/03/1999	: ajout gravite v1.7
* F.Lizot       07/05/2004      : Ajout de la reference datex (SAGA) v1.8
------------------------------------------------------ */

#ifndef XZAE127
#define XZAE127

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
#define XZAE127C_RPC_NAME "XZAE127"

#define XZAE127C_NB_PARM_RETURN 20


/* definitions de types exportes */



/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*  XZAE127_Infos_obligatoires : recupere les infos 
*  necessaires d un fichier evt strada
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int XZAE127_Infos_obligatoires (	XDY_Entier	va_numero_in ,
					XDY_Octet	va_cle_in,
					XDY_Horodate	*va_debut,
					XDY_Horodate	*va_fin,
					XDY_Nom		va_fausse_alerte_out,
					XDY_Entier	*va_duree_out,
					XDY_Nom		va_fin_evt_out,
					XDY_Horodate	*va_validation,
					XDY_Entier	*va_numero_cause_out,
					XDY_Octet	*va_cle_cause_out,
					XDY_Nom		va_type_cause_out,
					XDY_Octet	*va_veracite_out,
					XDY_Nom		va_prevision_out,
					XDY_Nom		va_localisation_out,
					XDY_Entier	*va_zz_out,
					XDY_Octet	*va_sens_out,
					XDY_Entier	*va_pr_out,
					XDY_Octet	*va_autoroute_out,
					XDY_Octet	*va_trafic_out,
					XDY_Octet       *va_gravite_out,
					XDY_Octet	*va_nbvoiesop_out,
					XDY_Nom         va_reference_out);

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
