/*E*/
/*Fichier : $Id: acli_xzac06_cb.c,v 1.1 2007/03/26 13:55:46 gesconf Exp $      Release : $Revision: 1.1 $        Date : $Date: 2007/03/26 13:55:46 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE ACLI_XZAN34CB * FICHIER acli_xzac06_cb.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale
*   ou reference au fichier acli_xzac06_cb.h >
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	14 Nov 2005	: Creation
* LCL		20/12/21	: MOVIS P2 ems SAE-312
------------------------------------------------------*/

/* fichiers inclus */

#include "acli_xzac_cb.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *	version = "$Id: acli_xzac06_cb.c,v 1.1 2007/03/26 13:55:46 gesconf Exp $ : acli_xzac06_cb" ;


/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void acli_xzac06_cb 	(T_IPC_CONN 	va_Cnx,
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
*	Fonction declenchee sur reception de message XDM_ETunnel_AUTO.
* 	
------------------------------------------------------*/
{
#ifndef _TIBCO_EMS
	T_INT4 vl_numero,vl_distance;
	T_INT2	vl_cle,vl_poste,vl_identifiant,vl_manuelle,vl_priorite,vl_district_Tunnel,vl_district;
	T_INT2 	vl_scenario;
	T_REAL8	vl_horodate;
#else
	XDY_Entier vl_numero,vl_distance;
	XDY_Mot	vl_cle,vl_poste,vl_identifiant,vl_manuelle,vl_priorite,vl_district_Tunnel,vl_district;
	XDY_Mot 	vl_scenario;
	XDY_Horodate	vl_horodate;
#endif
	
	XZST_03EcritureTrace(XZSTC_WARNING,"IN : acli_xzac_06_cb : debut d execution ");

        if ( !TipcMsgRead(pa_Data -> msg,
			T_IPC_FT_INT4, &vl_numero,
			T_IPC_FT_INT2, &vl_cle,
			T_IPC_FT_INT2, &vl_poste,
			T_IPC_FT_INT2, &vl_identifiant,
			T_IPC_FT_REAL8, &vl_horodate,
			T_IPC_FT_INT2, &vl_manuelle,
			T_IPC_FT_INT2, &vl_priorite,
			T_IPC_FT_INT4, &vl_distance,
			T_IPC_FT_INT2, &vl_scenario,
			NULL))
        {
	  XZST_03EcritureTrace ( XZSTC_WARNING," Impossible de lire le MSG XDM_ETunnel_AUTO");
	  return;
        }

	XZST_03EcritureTrace(XZSTC_WARNING,"IN : acli_xzac_06_cb : evt(%d/%d), poste(%d), Tunnel (%d), sce (%d)",
			vl_numero,
			vl_cle,
			vl_poste,
			vl_identifiant,
			vl_scenario);

        if (XZAC06_Commande_Tunnel(vl_numero, /*num evt*/
                              vl_cle,    /*cle evt*/
                              vl_poste,            /*poste operateur*/
                              vl_identifiant,      /*numero equipement*/
                              vl_horodate ,            /*horodate lancement*/
                              vl_manuelle,                   /*manuelle*/
                              vl_priorite,         /*priorite*/
                              vl_distance,      /*distance evt*/
                              vl_scenario         /*nom court du scenario*/)  
                               != XDC_OK)
        XZST_03EcritureTrace(XZSTC_FONCTION,"acli_xzac_06_cb : pb appel a XZAC06_Commande_Tunnel");


	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : acli_xzac_06_cb : Fin d execution ");
        
}

