/*E Fichier : $Id: csva_gca.h,v 1.1 1995/01/25 10:40:33 mercier Exp $      Release : $Revision: 1.1 $        Date : $Date: 1995/01/25 10:40:33 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME GECONF
------------------------------------------------------
* MODULE GCA * FICHIER GCA.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*
*	Module de TCSVA contenant les fonctions de gestions 
* du catalogue des archives.
* DCG-002/0.1
------------------------------------------------------
* HISTORIQUE :
*
* mercier	06 Jan 1995	: Creation
------------------------------------------------------ */

#ifndef GCA
#define GCA

/* fichiers inclus */
	#include        <stdio.h>

	/* includes Utilitaires */
	
		/* rtworks */
	#include        <rtworks/common.h>
	#include        <rtworks/ipc.h>
	#include        <rtworks/ipc_proto.h>
	
	/* includes applicatifs Migrazur */
		
	#include        "xdc.h"
	#include        "xdy.h"
	#include        "xdg.h"
	#include        "xdf.h"	
	#include        "xzsm.h"
	#include        "xzsc.h"
	#include        "xzia.h"
	#include        "xzca.h"
	
/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

XDY_Entier 	CGCAV_MOISBD;
XDY_Entier 	CGCAV_ANNEEBD;

/* delaration de fonctions externes */

/*  Fonction permettant d ecrire d un enregistrement dans un fichier du 
*  catalogue des archives. Cette enregistrement contient le numero logique
*  de la bande (constituer de 6 digits : 199401 pour le mois de janvier de
*  l annee 1994), avec le nombre de copie.
*/

extern int cgca_EcritCat( XZCAT_ComposantCAT );

/*  Fonction permettant de recuperer le mois et l annee d archivage present
*  dans la base de sauvegarde/restauration.
*/

extern int  cgca_LectMoisAnneeBD( XDY_Entier *,XDY_Entier *);

/*  Fonction permettant de sauvegarder le mois et l annee d archivage present
*  dans la base de sauvegarde/restauration.
*/

extern int  cgca_EcritMoisAnneeBD( XDY_Entier , XDY_Entier );

#endif
