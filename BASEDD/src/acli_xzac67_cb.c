/*E*/
/*Fichier : $Id: acli_xzac67_cb.c,v 1.1 2004/11/16 14:09:10 gesconf Exp $      Release : $Revision: 1.1 $        Date : $Date: 2004/11/16 14:09:10 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE ACLI_XZAN34CB * FICHIER acli_xzac66_cb.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale
*   ou reference au fichier acli_xzac66_cb.h >
*
------------------------------------------------------
* HISTORIQUE :
*
* mercier	07 Nov 1994	: Creation
* LCL		20/12/21	: MOVIS P2 ems SAE-312
------------------------------------------------------*/

/* fichiers inclus */

#include "acli_xzac67_cb.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: acli_xzac67_cb.c,v 1.1 2004/11/16 14:09:10 gesconf Exp $ : acli_xzac66_cb" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void acli_xzac67_cb 	(T_IPC_CONN 	va_Cnx,
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
*	Fonction declenchee sur reception de message XDM_EPAL_AUTO.
* 	
------------------------------------------------------*/
{
#ifndef _TIBCO_EMS
        T_INT2  vl_identifiant,vl_manuelle,vl_district;
        T_STR   vl_scenario,vl_nom_scenario;
        T_REAL8 vl_horodate;
#else
        XDY_Mot vl_identifiant,vl_manuelle,vl_district;
        XDY_Scenario    vl_scenario,vl_nom_scenario;
        XDY_Horodate    vl_horodate;
#endif

	XZST_03EcritureTrace(XZSTC_WARNING,"IN : acli_xzac_67_cb : debut d execution ");

        if ( !TipcMsgRead(pa_Data -> msg,
			T_IPC_FT_INT2, &vl_identifiant,
			T_IPC_FT_INT2, &vl_district,
			T_IPC_FT_REAL8, &vl_horodate,
			T_IPC_FT_INT2, &vl_manuelle,
			T_IPC_FT_STR, &vl_scenario,
			T_IPC_FT_STR, &vl_nom_scenario,
			NULL))
        {
	  XZST_03EcritureTrace ( XZSTC_WARNING," Impossible de lire le MSG XDM_EPAL_AUTO_FIN");
	  return;
        }

	XZST_03EcritureTrace(XZSTC_WARNING,"IN : acli_xzac_67_cb : pal (%d), sce (%s/%s), site pal(%d)",
			vl_identifiant,
			vl_scenario,
			vl_nom_scenario,
			vl_district);

        if (XZAC67_Commande_Fin_PAL( vl_identifiant,      /*numero equipement*/
                              vl_district,         /*site du pal*/
                              vl_horodate ,            /*horodate lancement*/
                              vl_manuelle,                   /*manuelle*/
                              vl_scenario,         /*nom court du scenario*/
                              vl_nom_scenario)     /*nom long du scenario*/
                               != XDC_OK)
        XZST_03EcritureTrace(XZSTC_FONCTION,"acli_xzac_67_cb : pb appel a XZAC67_Commande__Fin_PAL");


	XZST_03EcritureTrace(XZSTC_WARNING,"OUT : acli_xzac_67_cb : Fin d execution ");
        
}

