/*E*/
/*Fichier : $Id: acli_xzac03_cb.c,v 1.2 2010/11/18 11:57:31 gesconf Exp $      Release : $Revision: 1.2 $        Date : $Date: 2010/11/18 11:57:31 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE ACLI_XZAN34CB * FICHIER acli_xzac03_cb.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale
*   ou reference au fichier acli_xzac03_cb.h >
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	14 Nov 2005	: Creation
* JPL	18/11/10 : Suppression warnings mineurs pour compilation sur architecture HP ia64 (DEM 961)  1.2
* LCL		20/12/21	: MOVIS P2 ems SAE-312
------------------------------------------------------*/

/* fichiers inclus */

#include "acli_xzac03_cb.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *	version = "$Id: acli_xzac03_cb.c,v 1.2 2010/11/18 11:57:31 gesconf Exp $ : acli_xzac03_cb" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void acli_xzac03_cb 	(T_IPC_CONN 	va_Cnx,
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
*	Fonction declenchee sur reception de message XDM_ENAV_AUTO.
* 	
------------------------------------------------------*/
{
#ifndef _TIBCO_EMS
	T_INT4 vl_numero,vl_distance;
	T_INT2	vl_cle,vl_poste,vl_identifiant,vl_manuelle,vl_priorite,vl_district_bra,vl_district;
	T_STR 	vl_scenario,vl_nom_scenario;
	T_REAL8	vl_horodate;
#else
	XDY_Entier vl_numero,vl_distance;
	XDY_Mot	vl_cle,vl_poste,vl_identifiant,vl_manuelle,vl_priorite,vl_district_bra,vl_district;
	XDY_Phrase	vl_scenario,vl_nom_scenario;
	T_REAL8	vl_horodate;
#endif
	
	XZST_03EcritureTrace(XZSTC_WARNING,"IN : acli_xzac_03_cb : debut d execution ");

        if ( !TipcMsgRead(pa_Data -> msg,
			T_IPC_FT_INT4, &vl_numero,
			T_IPC_FT_INT2, &vl_cle,
			T_IPC_FT_INT2, &vl_poste,
			T_IPC_FT_INT2, &vl_identifiant,
			T_IPC_FT_REAL8, &vl_horodate,
			T_IPC_FT_INT2, &vl_manuelle,
			T_IPC_FT_INT2, &vl_priorite,
			T_IPC_FT_INT4, &vl_distance,
			T_IPC_FT_STR, &vl_scenario,
			T_IPC_FT_INT2, &vl_district_bra,
			T_IPC_FT_INT2, &vl_district,
			NULL))
        {
	  XZST_03EcritureTrace ( XZSTC_WARNING," Impossible de lire le MSG XDM_EBRA_AUTO");
	  return;
        }

	XZST_03EcritureTrace(XZSTC_WARNING,"IN : acli_xzac_03_cb : evt(%d/%d), poste(%d), bra (%d), sce (%s), site (%d), site bra(%d)",
			vl_numero,
			vl_cle,
			vl_poste,
			vl_identifiant,
			vl_scenario,
			vl_district_bra,
			vl_district);

        if (XZAC03_Commande_NAV(vl_numero, /*num evt*/
                              vl_cle,    /*cle evt*/
                              vl_poste,            /*poste operateur*/
                              vl_identifiant,      /*numero equipement*/
                              vl_horodate ,            /*horodate lancement*/
                              vl_manuelle,                   /*manuelle*/
                              vl_priorite,         /*priorite*/
                              vl_distance,      /*distance evt*/
                              vl_scenario,         /*nom court du scenario*/
                              vl_district_bra,         /*site du bra*/
                              vl_district)             /*site d'origine de l'action*/
                               != XDC_OK)
        XZST_03EcritureTrace(XZSTC_FONCTION,"acli_xzac_03_cb : pb appel a XZAC03_Commande_NAV");


	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : acli_xzac_03_cb : Fin d execution ");
        
}

