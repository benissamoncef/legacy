/*E*/
/*Fichier : $Id: acli_xzac98_cb.c,v 1.1 2006/12/05 16:13:57 gesconf Exp $      Release : $Revision: 1.1 $        Date : $Date: 2006/12/05 16:13:57 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE ACLI_XZAN34CB * FICHIER acli_xzac98_cb.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale
*   ou reference au fichier acli_xzac98_cb.h >
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
static char *	version = "$Id: acli_xzac98_cb.c,v 1.1 2006/12/05 16:13:57 gesconf Exp $ : acli_xzac98_cb" ;


/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void acli_xzac98_cb 	(T_IPC_CONN 	va_Cnx,
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
*	Fonction declenchee sur reception de message XDM_EBAF_AUTO_FIN.
* 	
------------------------------------------------------*/
{
#ifndef _TIBCO_EMS
        T_INT2  vl_identifiant,vl_manuelle,vl_district;
        T_STR   vl_scenario,vl_nom_scenario;
        T_REAL8 vl_horodate;
	T_INT4 vl_taille;
	T_INT2 *tl_modules;
#else
        XDY_Mot vl_identifiant,vl_manuelle,vl_district;
        XDY_Scenario    vl_scenario,vl_nom_scenario;
        XDY_Horodate    vl_horodate;
	XDY_Entier vl_taille;
	XDY_Mot tl_modules[XDC_Lon_Nb_Mod_BAF];
#endif
	
	XZST_03EcritureTrace(XZSTC_WARNING,"IN : acli_xzac_98_cb : debut d execution ");

        if ( !TipcMsgRead(pa_Data -> msg,
			T_IPC_FT_INT2, &vl_identifiant,
			T_IPC_FT_INT2, &vl_district,
			T_IPC_FT_REAL8, &vl_horodate,
			T_IPC_FT_INT2, &vl_manuelle,
			T_IPC_FT_INT2_ARRAY, &tl_modules,&vl_taille,
			NULL))
        {
	  XZST_03EcritureTrace ( XZSTC_WARNING," Impossible de lire le MSG XDM_EBAF_AUTO_FIN");
	  return;
        }

	XZST_03EcritureTrace(XZSTC_WARNING,"IN : acli_xzac_98_cb : baf (%d,%d,%d) (%d,%d,%d), modules (%s), site baf(%d)",
			vl_identifiant,
			tl_modules[0],
			tl_modules[1],
			tl_modules[2],
			tl_modules[3],
			tl_modules[4],
			tl_modules[5],
			vl_district);

        if (XZAC98_Commande_Fin_BAF( vl_identifiant,      /*numero equipement*/
                              vl_district,         /*site du baf*/
                              vl_horodate ,            /*horodate lancement*/
                              vl_manuelle,                   /*manuelle*/
			      tl_modules[0],
			      tl_modules[1],
			      tl_modules[2],
			      tl_modules[3],
			      tl_modules[4],
			      tl_modules[5])   
                               != XDC_OK)
        XZST_03EcritureTrace(XZSTC_FONCTION,"acli_xzac_98_cb : pb appel a XZAC98_Commande__Fin_BAF");


	XZST_03EcritureTrace(XZSTC_WARNING,"OUT : acli_xzac_98_cb : Fin d execution ");
        
}

