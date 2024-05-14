/*E*/
/*Fichier :  $Id: acli_xzac634_cb.c,v 1.1 2008/02/13 13:23:35 devgtie Exp $      Release : $Revision: 1.1 $        Date : $Date: 2008/02/13 13:23:35 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE ACLI_XZAN34CB * FICHIER acli_xzac634_cb.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale
*   ou reference au fichier acli_xzac634_cb.h >
*
------------------------------------------------------
* HISTORIQUE :
*
* J.B.	10/12/07	: Creation a partir xzac63 DEM 663
* LCL		20/12/21	: MOVIS P2 ems SAE-312
------------------------------------------------------*/

/* fichiers inclus */

#include "acli_xzac_cb.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */
static char *	version = "@(#)acli_xzac634_cb.c	1.1 03/26/07 : acli_xzac634_cb" ;


/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void acli_xzac634_cb 	(T_IPC_CONN 	va_Cnx,
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
*	Fonction declenchee sur reception de message XDM_ETUNNEL_AUTO.
* 	
------------------------------------------------------*/
{
#ifndef _TIBCO_EMS
        T_INT2  vl_identifiant,vl_manuelle,vl_district;
        T_REAL8 vl_horodate;
#else
        XDY_Mot vl_identifiant,vl_manuelle,vl_district;
        XDY_Horodate    vl_horodate;
#endif

	XZST_03EcritureTrace(XZSTC_WARNING,"IN : acli_xzac_634_cb : debut d execution ");

        if ( !TipcMsgRead(pa_Data -> msg,
			T_IPC_FT_INT2, &vl_identifiant,
			T_IPC_FT_INT2, &vl_district,
			T_IPC_FT_REAL8, &vl_horodate,
			T_IPC_FT_INT2, &vl_manuelle,
			NULL))
        {
	  XZST_03EcritureTrace ( XZSTC_WARNING," Impossible de lire le MSG XDM_ETUNNEL_AUTO_FIN");
	  return;
        }

	XZST_03EcritureTrace(XZSTC_WARNING,"IN : acli_xzac_634_cb : Tunnel (%d), site Tunnel(%d)",
			vl_identifiant,
			vl_district);

        if (XZAC634_Commande_Fin_Energie( vl_identifiant,      /*numero equipement*/
                              vl_district,         /*site du Tunnel*/
                              vl_horodate ,            /*horodate lancement*/
                              vl_manuelle )    
                               != XDC_OK)
        XZST_03EcritureTrace(XZSTC_FONCTION,"acli_xzac_634_cb : pb appel a XZAC634_Commande__Fin_Tunnel");


	XZST_03EcritureTrace(XZSTC_WARNING,"OUT : acli_xzac_634_cb : Fin d execution ");
        
}

