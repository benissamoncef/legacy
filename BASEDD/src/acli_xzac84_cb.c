/*E*/
/*Fichier : $Id: acli_xzac84_cb.c,v 1.1 2006/12/05 16:14:28 gesconf Exp $      Release : $Revision: 1.1 $        Date : $Date: 2006/12/05 16:14:28 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE ACLI_XZAN34CB * FICHIER acli_xzac61_cb.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale
*   ou reference au fichier acli_xzac61_cb.h >
*
------------------------------------------------------
* HISTORIQUE :
*
* mercier	07 Nov 1994	: Creation
* LCL		20/12/21	: MOVIS P2 ems SAE-312
------------------------------------------------------*/

/* fichiers inclus */

#include "acli_xzac_cb.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */
static char *	version = "$Id: acli_xzac84_cb.c,v 1.1 2006/12/05 16:14:28 gesconf Exp $ : acli_xzac84_cb" ;


/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void acli_xzac84_cb 	(T_IPC_CONN 	va_Cnx,
        		T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        		T_CB_ARG                        pa_ARG) 
/*
* ARGUMENTS EN ENTREE :
*
*   	Arguments d'une callback RTServer.
*
* ARGUMENTS EN SORTIE :
*
*   aucun
*
* CODE RETOUR : 
*  aucun
*
* CONDITION D'UTILISATION
*
* FONCTION 
*	Fonction declenchee sur reception de message XDM_EPMVA_AUTO_FIN.
* 	
------------------------------------------------------*/
{
#ifndef _TIBCO_EMS
        T_INT2  vl_identifiant,vl_neutre,vl_district;
        T_REAL8 vl_horodate;
#else
        XDY_Mot vl_identifiant,vl_neutre,vl_district;
        XDY_Horodate    vl_horodate;
#endif
	
	XZST_03EcritureTrace(XZSTC_WARNING,"IN : acli_xzac_84_cb : debut d execution ");

        if ( !TipcMsgRead(pa_Data -> msg,
			T_IPC_FT_INT2, &vl_identifiant,
			T_IPC_FT_INT2, &vl_district,
			T_IPC_FT_REAL8, &vl_horodate,
			T_IPC_FT_INT2, &vl_neutre,
			NULL))
        {
	  XZST_03EcritureTrace ( XZSTC_WARNING," Impossible de lire le MSG XDM_EPMVA_AUTO_FIN");
	  return;
        }

	XZST_03EcritureTrace(XZSTC_WARNING,"IN : acli_xzac_84_cb : pmva (%d), neutre (%d) site pmva(%d)",
			vl_identifiant,vl_neutre,
			vl_district);

        if (XZAC84_Commande_Fin_PMVA( vl_identifiant,      /*numero equipement*/
                              vl_district,         /*site du pmva*/
                              vl_horodate ,            /*horodate lancement*/
                              vl_neutre)   
                               != XDC_OK)
        XZST_03EcritureTrace(XZSTC_FONCTION,"acli_xzac_84_cb : pb appel a XZAC84_Commande__Fin_PMVA");


	XZST_03EcritureTrace(XZSTC_WARNING,"OUT : acli_xzac_84_cb : Fin d execution ");
        
}

