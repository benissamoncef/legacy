/*E*/
/* Fichier : $Id: acli_xzac303_cb.c,v 1.1 2012/07/04 11:39:12 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2012/07/04 11:39:12 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE ACLI_XZAN34CB * FICHIER acli_xzac303_cb.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale
*   ou reference au fichier acli_xzac303_cb.h >
*
------------------------------------------------------
* HISTORIQUE :
*
* VR	02/05/12	: Creation (DEM/1016)
* LCL		20/12/21	: MOVIS P2 ems SAE-312
------------------------------------------------------*/

/* fichiers inclus */

#include "acli_xzac_cb.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *	version = "@(#)acli_xzac303_cb.c	1.1 12/05/06 : acli_xzac303_cb" ;


/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X*/
/*----------------------------------------------------
* SERVICE RENDU :
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void acli_xzac303_cb 	(T_IPC_CONN 	va_Cnx,
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
*	Fonction declenchee sur reception de message XDM_EPRV_AUTO_FIN.
*
------------------------------------------------------*/
{
#ifndef _TIBCO_EMS
	T_INT4 vl_numero,vl_distance, vl_numaction_out;
	T_INT2	vl_cle,vl_poste,vl_identifiant,vl_priorite,vl_district_prv,vl_district, vl_seuil;
	T_REAL8	vl_horodate;
	T_STR	vl_picto,vl_bandeau, vl_vitesse;
#else
	XDY_Entier vl_numero,vl_distance, vl_numaction_out;
	XDY_Mot	vl_cle,vl_poste,vl_identifiant,vl_priorite,vl_district_prv,vl_district, vl_seuil;
	XDY_Horodate	vl_horodate;
	XDY_Ligne_PMV	vl_picto,vl_bandeau, vl_vitesse;
#endif

	XZST_03EcritureTrace(XZSTC_WARNING,"IN : acli_xzac303_cb : debut d execution ");

        if ( !TipcMsgRead(pa_Data -> msg,
			T_IPC_FT_INT4, &vl_numero,
			T_IPC_FT_INT2, &vl_cle,
			T_IPC_FT_INT2, &vl_poste,
			T_IPC_FT_INT2, &vl_identifiant,
			T_IPC_FT_REAL8, &vl_horodate,
			T_IPC_FT_INT2, &vl_district_prv,
			T_IPC_FT_STR, &vl_bandeau,
			T_IPC_FT_STR, &vl_vitesse,
			T_IPC_FT_INT2, &vl_priorite,
			T_IPC_FT_INT4, &vl_distance,
			T_IPC_FT_INT2, &vl_district,
			NULL))
        {
	  XZST_03EcritureTrace ( XZSTC_WARNING," Impossible de lire le MSG XDM_EPRV_AUTO_FIN");
	  return;
        }

	XZST_03EcritureTrace(XZSTC_WARNING,"IN : acli_xzac303_cb : evt(%d/%d), poste(%d), prv (%d), site (%d), site prv(%d)",
			vl_numero,
			vl_cle,
			vl_poste,
			vl_identifiant,
			vl_district_prv,
			vl_district);

        if (XZAC303_Commande_Fin_PRV(vl_numero, /*num evt*/
                              vl_cle,    /*cle evt*/
                              vl_poste,            /*poste operateur*/
                              vl_identifiant,      /*numero equipement*/
                              vl_horodate ,            /*horodate lancement*/
                              vl_district_prv, /*site du pmv*/
                              vl_bandeau,
                              vl_vitesse,
							  vl_priorite,
							  vl_distance,
                              vl_district)             /*site d'origine de l'action*/
                               != XDC_OK)
        XZST_03EcritureTrace(XZSTC_FONCTION,"acli_xzac_303_cb : pb appel a XZAC303_Commande_PRV");


	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : acli_xzac_303_cb : Fin d execution ");

}

