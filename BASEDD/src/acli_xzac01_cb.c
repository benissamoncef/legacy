/*E*/
/*Fichier : @(#)acli_xzac01_cb.c	1.1      Release : 1.1        Date : 12/05/06
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE ACLI_XZAN34CB * FICHIER acli_xzac01_cb.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale
*   ou reference au fichier acli_xzac01_cb.h >
*
------------------------------------------------------
* HISTORIQUE :
*
* PNI	25/08/06	: Creation
* VR	07/11/11	: Ajout Destination Noeud (DEM/1014)
* PNI	28/08/13	: Correction de la lecture du msg DEM1074 1.3
* LCL		20/12/21	: MOVIS P2 ems SAE-312
------------------------------------------------------*/

/* fichiers inclus */

#include "acli_xzac_cb.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *	version = "@(#)acli_xzac01_cb.c	1.1 12/05/06 : acli_xzac01_cb" ;


/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void acli_xzac01_cb 	(T_IPC_CONN 	va_Cnx,
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
*	Fonction declenchee sur reception de message XDM_EPMV_AUTO.
* 	
------------------------------------------------------*/
{
#ifndef _TIBCO_EMS
	T_INT4 vl_numero,vl_distance,vl_dest,vl_destalt, vl_DestinationNoeud;
	T_INT2	vl_cle,vl_poste,vl_identifiant,vl_manuelle,vl_priorite,vl_district_pmv,vl_district, vl_clig1,vl_clig2,vl_clig3,vl_flash,vl_seuil,vl_cligpicto;
	T_REAL8	vl_horodate;
	T_STR	vl_Ligne1,vl_Ligne2, vl_Ligne3, vl_Alter1,vl_Alter2,vl_Alter3,vl_picto,vl_bandeau,vl_Ligne4,vl_Alter4	= "";
#else
	XDY_Entier vl_numero,vl_distance,vl_dest,vl_destalt, vl_DestinationNoeud;
	XDY_Mot	vl_cle,vl_poste,vl_identifiant,vl_manuelle,vl_priorite,vl_district_pmv,vl_district, vl_clig1,vl_clig2,vl_clig3,vl_flash,vl_seuil,vl_cligpicto;
	XDY_Horodate	vl_horodate;
	XDY_Ligne_PMV	vl_Ligne1,vl_Ligne2, vl_Ligne3, vl_Alter1,vl_Alter2,vl_Alter3,vl_picto,vl_bandeau,vl_Ligne4,vl_Alter4	= "";
#endif

	XZST_03EcritureTrace(XZSTC_WARNING,"IN : acli_xzac_01_cb : debut d execution ");
	XZST_03EcritureTrace(XZSTC_WARNING,"IN : acli_xzac_01_cb avant : evt(%d/%d), poste(%d), pmv (%d), site (%d), site pmv(%d) c1(%d) c2(%d) c3(%d) flash(%d)",
			vl_numero,
			vl_cle,
			vl_poste,
			vl_identifiant,
			vl_district_pmv,
			vl_district,vl_clig1,
			     vl_clig2,
			     vl_clig3, vl_flash);

        if ( !TipcMsgRead(pa_Data -> msg,
			T_IPC_FT_INT4, &vl_numero,
			T_IPC_FT_INT2, &vl_cle,
			T_IPC_FT_INT2, &vl_poste,
			T_IPC_FT_INT2, &vl_identifiant,
			T_IPC_FT_REAL8, &vl_horodate,
			T_IPC_FT_INT2, &vl_district_pmv,
			T_IPC_FT_STR, &vl_Ligne1,
			T_IPC_FT_STR, &vl_Ligne2,
			T_IPC_FT_STR, &vl_Ligne3,
			T_IPC_FT_STR, &vl_Alter1,
			T_IPC_FT_STR, &vl_Alter2,
			T_IPC_FT_STR, &vl_Alter3,
			T_IPC_FT_INT2, &vl_clig1,
			T_IPC_FT_INT2, &vl_clig2,
			T_IPC_FT_INT2, &vl_clig3,
			T_IPC_FT_INT2, &vl_flash,
			T_IPC_FT_STR, &vl_picto,
			T_IPC_FT_STR, &vl_bandeau,
			T_IPC_FT_INT2, &vl_cligpicto,
			T_IPC_FT_INT2, &vl_priorite,
			T_IPC_FT_INT4, &vl_distance,
			T_IPC_FT_STR, &vl_Ligne4,
			T_IPC_FT_STR, &vl_Alter4,
			T_IPC_FT_INT2, &vl_seuil,
			T_IPC_FT_INT4, &vl_dest,
			T_IPC_FT_INT4, &vl_destalt,
			T_IPC_FT_INT2, &vl_district,
			T_IPC_FT_INT4, &vl_DestinationNoeud,
			NULL))
        {
	  XZST_03EcritureTrace ( XZSTC_WARNING," Impossible de lire le MSG XDM_EPMV_AUTO");
	  return;
        }

	XZST_03EcritureTrace(XZSTC_WARNING,"IN : acli_xzac_01_cb : evt(%d/%d), poste(%d), pmv (%d), site (%d), site pmv(%d) c1(%d) c2(%d) c3(%d)",
			vl_numero,
			vl_cle,
			vl_poste,
			vl_identifiant,
			vl_district_pmv,
			vl_district,vl_clig1,
			     vl_clig2,
			     vl_clig3);

        if (XZAC01_Commande_PMV(vl_numero, /*num evt*/
                              vl_cle,    /*cle evt*/
                              vl_poste,            /*poste operateur*/
                              vl_identifiant,      /*numero equipement*/
                              vl_horodate ,            /*horodate lancement*/
                              vl_district_pmv,         /*site du pmv*/
			     vl_Ligne1,
			     vl_Ligne2,
			     vl_Ligne3,
			     vl_Alter1,
			     vl_Alter2,
			     vl_Alter3,
			     vl_clig1,
			     vl_clig2,
			     vl_clig3,
			     vl_flash,
			     vl_picto,
			     vl_bandeau,
			     vl_cligpicto,
			     vl_priorite,
			     vl_distance,
			     vl_Ligne4,
			     vl_Alter4,
			     vl_dest,
			     vl_destalt,
			     vl_seuil,
                              vl_district,
							  vl_DestinationNoeud)             /*site d'origine de l'action*/
                               != XDC_OK)
        XZST_03EcritureTrace(XZSTC_FONCTION,"acli_xzac_01_cb : pb appel a XZAC01_Commande_PMV");


	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : acli_xzac_01_cb : Fin d execution ");
        
}

