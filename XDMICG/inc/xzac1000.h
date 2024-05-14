/*E*/
/*Fichier : $Id: xzac1000.h,v 1.1 2012/07/04 10:53:28 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2012/07/04 10:53:28 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAC * FICHIER xzac1000.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Interface Actions >
*
------------------------------------------------------
* HISTORIQUE :
*
* VR		21/12/11	: Creation (DEM/1016)
------------------------------------------------------ */

#ifndef xzac1000
#define xzac1000

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZACC_XZAC1000_RPC_NAME "XZAC1000"
#define XZACC_XZAC1000_NB_PARM_RETURN 0

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAC1000_Recap_Info_PRV ( 	XDY_Horodate,
					XDY_Evt,
					XDY_FonctionInt );



#endif
