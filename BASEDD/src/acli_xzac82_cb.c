/*E*/
/*Fichier : $Id: acli_xzac82_cb.c,v 1.3 2013/08/27 13:57:36 gesconf Exp $      Release : $Revision: 1.3 $        Date : $Date: 2013/08/27 13:57:36 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE ACLI_XZAN34CB * FICHIER acli_xzac82_cb.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale
*   ou reference au fichier acli_xzac82_cb.h >
*
------------------------------------------------------
* HISTORIQUE :
*
* PNI	25/08/06	: Creation
* VR	25/05/12	: Ajout DTP PMVA (DEM/1014 PMA)
* PNI	27/08/2013	: correction appel XZAC82_Commande_PMVA DEM1074 v1.3
* LCL		20/12/21	: MOVIS P2 ems SAE-312
------------------------------------------------------*/

/* fichiers inclus */

#include "acli_xzac_cb.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *	version = "$Id: acli_xzac82_cb.c,v 1.3 2013/08/27 13:57:36 gesconf Exp $ : acli_xzac82_cb" ;


/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void acli_xzac82_cb 	(T_IPC_CONN 	va_Cnx,
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
*	Fonction declenchee sur reception de message XDM_EPMVA_AUTO.
* 	
------------------------------------------------------*/
{
#ifndef _TIBCO_EMS
	T_INT4 vl_numero,vl_distance;
	T_INT2	vl_dest2lointaine, vl_dest2proche, vl_dest1lointaine, vl_dest1proche, vl_depasseSeuil, vl_cle,vl_poste,vl_identifiant,vl_manuelle,vl_priorite,vl_district_pmva,vl_district,vl_clig1,vl_clig2,vl_clig3,vl_clig4,vl_clig5,vl_flash;
	T_REAL8	vl_horodate;
	T_STR	vl_Ligne1,vl_Ligne2, vl_Ligne3,vl_Ligne4, vl_Ligne5, vl_Alter1,vl_Alter2,vl_Alter3,vl_Alter4,vl_Alter5	= "";
#else
	XDY_Entier vl_numero,vl_distance;
	XDY_Mot	vl_dest2lointaine, vl_dest2proche, vl_dest1lointaine, vl_dest1proche, vl_depasseSeuil, vl_cle,vl_poste,vl_identifiant,vl_manuelle,vl_priorite,vl_district_pmva,vl_district,vl_clig1,vl_clig2,vl_clig3,vl_clig4,vl_clig5,vl_flash;
	XDY_Horodate	vl_horodate;
	XDY_Ligne_PMV	vl_Ligne1,vl_Ligne2, vl_Ligne3,vl_Ligne4, vl_Ligne5, vl_Alter1,vl_Alter2,vl_Alter3,vl_Alter4,vl_Alter5	= "";
#endif
	T_INT4 vl_num_action = 0;

	XZST_03EcritureTrace(XZSTC_WARNING,"IN : acli_xzac_82_cb : debut d execution ");

        if ( !TipcMsgRead(pa_Data -> msg,
			T_IPC_FT_INT4, &vl_numero,
			T_IPC_FT_INT2, &vl_cle,
			T_IPC_FT_INT2, &vl_poste,
			T_IPC_FT_INT2, &vl_identifiant,
			T_IPC_FT_REAL8, &vl_horodate,
			T_IPC_FT_INT2, &vl_district_pmva,
			T_IPC_FT_STR, &vl_Ligne1,
			T_IPC_FT_STR, &vl_Ligne2,
			T_IPC_FT_STR, &vl_Ligne3,
			T_IPC_FT_STR, &vl_Ligne4,
			T_IPC_FT_STR, &vl_Ligne5,
			T_IPC_FT_STR, &vl_Alter1,
			T_IPC_FT_STR, &vl_Alter2,
			T_IPC_FT_STR, &vl_Alter3,
			T_IPC_FT_STR, &vl_Alter4,
			T_IPC_FT_STR, &vl_Alter5,
			T_IPC_FT_INT2, &vl_clig1,
			T_IPC_FT_INT2, &vl_clig2,
			T_IPC_FT_INT2, &vl_clig3,
			T_IPC_FT_INT2, &vl_clig4,
			T_IPC_FT_INT2, &vl_clig5,
			T_IPC_FT_INT2, &vl_flash,
			T_IPC_FT_INT2, &vl_priorite,
			T_IPC_FT_INT4, &vl_distance,
			T_IPC_FT_INT2, &vl_depasseSeuil,
			T_IPC_FT_INT2, &vl_district,
			T_IPC_FT_INT2, &vl_dest1proche,
			T_IPC_FT_INT2, &vl_dest1lointaine,
			T_IPC_FT_INT2, &vl_dest2proche,
			T_IPC_FT_INT2, &vl_dest2lointaine,
			NULL))
        {
	  XZST_03EcritureTrace ( XZSTC_WARNING," Impossible de lire le MSG XDM_EPMVA_AUTO");
	  return;
        }

	XZST_03EcritureTrace(XZSTC_WARNING,"IN : acli_xzac_82_cb : evt(%d/%d), poste(%d), pmva (%d), site pmva (%d), site origine(%d)",
			vl_numero,
			vl_cle,
			vl_poste,
			vl_identifiant,
			vl_district_pmva,
			vl_district);

        if (XZAC82_Commande_PMVA(vl_horodate ,         /*horodate lancement*/
				vl_numero, 		/*num evt*/
                              vl_cle,    			/*cle evt*/
                              vl_poste,             /*poste operateur*/
                              vl_identifiant,       /*numero equipement*/
                              vl_district_pmva,     /*site du pmva*/
			     			  vl_Ligne1,
			    			  vl_Ligne2,
			    			  vl_Ligne3,
			    			  vl_Ligne4,
			     			  vl_Ligne5,
			     			  vl_Alter1,
			      			  vl_Alter2,
			   				  vl_Alter3,
			  				  vl_Alter4,
			    			  vl_Alter5,
			   				  vl_clig1,
			  				  vl_clig2,
			   				  vl_clig3,
			     			  vl_clig4,
			     			  vl_clig5,
			     			  vl_flash,
			     		  	  vl_priorite,
			     			  vl_distance,
				 		  vl_depasseSeuil,
                 			          vl_district,             /*site d'origine de l'action*/
							  vl_dest1proche,
							  vl_dest1lointaine,
				 			  vl_dest2proche,
				 			  vl_dest2lointaine,
			    			  vl_num_action)
                  			  != XDC_OK)

        XZST_03EcritureTrace(XZSTC_FONCTION,"acli_xzac_82_cb : pb appel a XZAC82_Commande_PMVA");


	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : acli_xzac_82_cb : Fin d execution ");
        
}

