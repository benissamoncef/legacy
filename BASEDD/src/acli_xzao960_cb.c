/*E*/
/*Fichier : @(#)acli_xzao960_cb.c	1.1      Release : 1.1        Date : 12/05/06
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE acli_xzao960CB * FICHIER acli_xzao960_cb.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < call back pour appel XZAO960 depuis un composant 
* non racordé à la base >
*
------------------------------------------------------
* HISTORIQUE :
*
* ABE 23/09/2020 : Creation DEM-SAE152 V1.1
* LCL		20/12/21	: MOVIS P2 ems SAE-312
------------------------------------------------------*/

/* fichiers inclus */

#include "acli_xzao_cb.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *	version = "@(#)acli_xzao960_cb.c	1.1 12/05/06 : acli_xzao960_cb" ;


/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void acli_xzao960_cb 	(T_IPC_CONN 	va_Cnx,
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
*	Fonction declenchee sur reception de message EMSG_TDP_FCD.
* 	
------------------------------------------------------*/
{
#ifndef _TIBCO_EMS
			T_INT4 vl_PR1_in,vl_PR2_in,vl_Tdp_in;
			T_STR vl_Autoroute_in="";
			T_INT2 vl_Sens_in,vl_Validite_in;
			T_REAL8	vl_Horodate_in;
#else
			XDY_Entier vl_PR1_in,vl_PR2_in,vl_Tdp_in;
			XDY_NomAuto vl_Autoroute_in="";
			XDY_Mot vl_Sens_in,vl_Validite_in;
			XDY_Horodate	vl_Horodate_in;
#endif

			int vl_ret=0;
	XZST_03EcritureTrace(XZSTC_WARNING,"IN : acli_xzao960_cb : debut d execution ");
		
        vl_ret = TipcMsgRead(pa_Data -> msg,
			T_IPC_FT_REAL8,&vl_Horodate_in,
			T_IPC_FT_INT4, &vl_PR1_in,
			T_IPC_FT_INT4, &vl_PR2_in,
			T_IPC_FT_INT2, &vl_Sens_in,
			T_IPC_FT_INT4, &vl_Tdp_in,      
			T_IPC_FT_INT2, &vl_Validite_in, 
			T_IPC_FT_STR,  &vl_Autoroute_in,
			NULL);
	if (!vl_ret)
        {
	XZST_03EcritureTrace(XZSTC_WARNING,"IN : acli_xzao960_cb : Horodate=%f,",vl_Horodate_in);
	XZST_03EcritureTrace(XZSTC_WARNING,"IN : acli_xzao960_cb : PR1=%d,",vl_PR1_in);
	XZST_03EcritureTrace(XZSTC_WARNING,"IN : acli_xzao960_cb : PR2=%d,",vl_PR2_in);
	XZST_03EcritureTrace(XZSTC_WARNING,"IN : acli_xzao960_cb : Sens=%d,",vl_Sens_in);
	XZST_03EcritureTrace(XZSTC_WARNING,"IN : acli_xzao960_cb : Tdp=%d,",vl_Tdp_in);
	XZST_03EcritureTrace(XZSTC_WARNING,"IN : acli_xzao960_cb : Validite=%d,",vl_Validite_in);
	XZST_03EcritureTrace(XZSTC_WARNING,"IN : acli_xzao960_cb : CharAutoroute=%s",vl_Autoroute_in);       
	  XZST_03EcritureTrace ( XZSTC_WARNING," Impossible de lire le MSG EMSG_TDP_FCD ret=%d",vl_ret);
	  return;
        }		

	XZST_03EcritureTrace(XZSTC_WARNING,"IN : acli_xzao960_cb : Horodate=%f, PR1=%d, PR2=%d, Sens=%d, Tdp=%d, Validite=%d, Autoroute=%s",
				    vl_Horodate_in,
				    vl_PR1_in, 		
				    vl_PR2_in,   
				    vl_Sens_in,
				    vl_Tdp_in,      
				    vl_Validite_in, 
				    vl_Autoroute_in);

        if ((XZAO960_insert_TDP_FCD ((XDY_Horodate)vl_Horodate_in,
				    (XDY_PR)vl_PR1_in, 		
				    (XDY_PR)vl_PR2_in,   
				    (XDY_Sens)vl_Sens_in,
				    (XDY_Entier)vl_Tdp_in,      
				    (XDY_Octet)vl_Validite_in, 
				    vl_Autoroute_in)  != XDC_OK))
        XZST_03EcritureTrace(XZSTC_FONCTION,"acli_xzao960_cb : pb appel a XZAO960_insert_TDP_FCD");


	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : acli_xzao960_cb : Fin d execution ");
        
}

