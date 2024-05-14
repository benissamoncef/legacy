/*E*/
/*X  Fichier : @(#)xzac905.h	1.1      Release : 1.1        Date : 04/12/20
------------------------------------------------------
*  * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAC * FICHIER xzac905.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Interface Actions >
*
------------------------------------------------------
* HISTORIQUE :
*
* ABE	09/04/20	: creation RAU SONO 1.1 DEM-SAE244
------------------------------------------------------ */

#ifndef xzac905
#define xzac905

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZACC_XZAC905_RPC_NAME "XZAC905"

#define XZACC_XZAC905_NB_PARM_RETURN 1

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */


extern int XZAC905_Commande_SONO ( XDY_Entier    	va_NumEvt_in,
                                 XDY_Octet     		va_CleEvt_in,
                                 XDY_Operateur 		va_Operateur_in,
                                 XDY_Eqt       		va_NumEqt_in,
                                 XDY_Horodate  		va_HeureLancement_in,
                                 XDY_Octet      	va_Site_in,
				 XDY_MessageSONO	*va_MessageSono_in,
				 XDY_Autoroute		*va_Autoroute_in,
				 XDY_Sens		va_Sens_in,
				 XDY_PR			va_PR_debut_in,
				 XDY_PR			va_PR_fin_in,
                                 XDY_Octet		va_Priorite_in,
                                 XDY_Entier    		va_DistanceEvt_in,
                                 XDY_Octet      	va_site_origine_in,
                                 XDY_Entier		*va_NumeroAction_out);

#endif	
