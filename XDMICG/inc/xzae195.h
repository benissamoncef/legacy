		/*E*/
/*Fichier : $Id: xzae195.h,v 1.2 2021/02/25 17:15:09 pc2dpdy Exp $      Release : $Revision: 1.2 $        Date : $Date: 2021/02/25 17:15:09 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAE195 * FICHIER XZAE195.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Appartient a BASEDD tache tada2
* permet de test si la FMC a été retypee
*
------------------------------------------------------
* HISTORIQUE :
*
* ABE	30/11/2020 :	Creation DEM130 V1.1
* ABE	05/02/2021 :    Ajout recuperation si route coupe sur version précedente de la fmc DEM-SAE130
------------------------------------------------------ */

#ifndef XZAE195
#define XZAE195

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
#define XZAE195C_RPC_NAME "XZAE195"

#define XZAE195C_NB_PARM_RETURN 4

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*  XZAE195_Lire_FMC_Retypee :permet de test si la FMC a été retypee
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int XZAE195_Lire_FMC_Retypee (XDY_Entier va_numero_in,
					XDY_Octet va_cle_in,
					XDY_Booleen *	va_retypage_out,
					XDY_Octet * va_type_anterieur_out,
					XDY_Booleen *	va_coupure_route_version_precedente,
					XDY_Booleen *	va_coupure_route_inverse_version_precedente);
/*
*
* PARAMETRES EN ENTREE :
*
*				
* PARAMETRES EN SORTIE :
* XDY_Eqt	va_numero_in
* XDY_sens	va_cle_in	

* VALEUR RENDUE :
* XDY_Booleen *	va_retypage_out
* XDY_MOT * va_type_anterieur_out
* 
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK 
*
* CONDITIONS D'UTILISATION :
*
*
*

* FONCTION :
*
* Permet de test si la FMC a été retypee
*
*
* MECANISMES :
*
------------------------------------------------------*/

#endif
