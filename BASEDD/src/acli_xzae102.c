/*E*/
/*Fichier : $Id: acli_xzae102.c,v 1.2 2020/11/03 16:06:35 pc2dpdy Exp $      Release : $Revision: 1.2 $        Date : $Date: 2020/11/03 16:06:35 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE ACLI_XZAE102 * FICHIER acli_xzae102.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* 	CallBack appele sur reception de message XDM_A_TIMER_TRV_COURS,
*	appartenant a TACLI.x. Il permet le rafraichissement de l IHM. 
*	Il s effectue tous les 5secondes environ (constante de acli_dir.h).
***********************************
*	axtimer_Ihm_cb recoit le message RTWks XDM_A_TIMER_TRV_COURS,  
* appelle XZAE102_Travaux_a_mettre_en_cours.
*********************************************************************
*
------------------------------------------------------
* HISTORIQUE :
*
* niepceron	27 Aou 1996	: Creation
*
------------------------------------------------------*/

/* fichiers inclus */

#include "acli_xzae102.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: acli_xzae102.c,v 1.2 2020/11/03 16:06:35 pc2dpdy Exp $ : acli_xzae102" ;

extern XDY_NomSite vg_NomSite;

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	axtimer_Ihm_cb recoit le message RTWks XDM_A_TIMER_TRV_COURS,  
* appelle XZAE102_Travaux_a_mettre_en_cours.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void axtimer_trv_cours_cb 	(	T_IPC_CONN 			va_Cnx,
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
*
* FONCTION 
*	Fonction declenchee sur reception de message XDM_A_TIMER_TRV_COURS.
*	Le message RTwks contient une cle comme tout message provenant de timer.
*	On appele xzae102 afin d animer l IHM.
*	Puis on redemande une programmation du timer.
* 	
------------------------------------------------------*/
{
	static char *version = "$Id: acli_xzae102.c,v 1.2 2020/11/03 16:06:35 pc2dpdy Exp $ : acli_axtimer_Ihm_cb" ;
	
	XDY_Datagroup   	vl_datagroup;
        XDY_NomSite     	pl_NomSite;
	int			vl_JourSemaine=0;
	double			vl_HorodateSec=0;
	int			vl_Nb_Secondes=0;
	int			vl_ValRet = XDC_OK;
	
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN %s",version);
 	
        if( XZAE102_Travaux_a_mettre_en_cours () != XDC_OK)
        {
                XZST_03EcritureTrace( XZSTC_WARNING, "%s : XZAE102_Travaux_a_mettre_en_cours  a echoue",version);
        }
        else
        {
                XZST_03EcritureTrace( XZSTC_DEBUG1,"%s : XZAE102_Travaux_a_mettre_en_cours  a reussi",version);
        }
        /*A
        ** Reprogrammation du timer
        */
        /*A
	** Recuperation du site de la machine sinon sortie via acli_sortir();
	*/
	strcpy(pl_NomSite,vg_NomSite);
	
	/*A
        ** Constitution du nom Data Group a partir du nom du site
        */
        
        XDG_EncodeDG2(vl_datagroup,XDG_A_TIMER_TRV_COURS,pl_NomSite);
	
	/*A 
	** La CB de refraichissement IHM se declenchera 
	** dans AXZAE102C_TIMERDELAI_XZAE102 secondes apres programmation
	*/
	
	if ((vl_ValRet = XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME,&vl_JourSemaine,&vl_HorodateSec))!=XZSMC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"acli_axtimer_trv_cours_cb : XZSM_07LireHorodate a echoue");
		acli_sortir();
	}
	
	vl_Nb_Secondes = vl_HorodateSec + AXZAE102C_TIMERDELAI_XZAE102;	
	/*A  
	** Declenchement d'une programmation sur
    	** un datagroup  XDG_A_TIMER_TRV_COURS_NomSite avec le message
   	** XDM_A_TIMER_TRV_COURS ayant comme cle de AXZAE102C_TIMERCLE_XZAE102  ********
   	*/
        vl_ValRet = XZSM_01DemanderProgrammation(
                				vl_Nb_Secondes,
                				0,
                				vl_datagroup,
                				XDM_A_TIMER_TRV_COURS,
                				AXZAE102C_TIMERCLE_XZAE102,
                				XZSMC_SEC_VRAI );
      	if (vl_ValRet != XDC_OK)
   	{
     		XZST_03EcritureTrace(XZSTC_WARNING,"axtimer__cb : XZSM_01DemanderProgrammation : Probleme de programmation TIMER pour mettre les travaux en cours \n\n ");
   		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : axtimer__cb : Sortie avec %d",vl_ValRet);
		acli_sortir();
   	}
   	else
   	{
   		XZST_03EcritureTrace(XZSTC_DEBUG1,"axtimer_trv_cours_cb : XZSM_01DemanderProgrammation a reprogramm≈ avec SUCCES \n\t\tle d≈clenchement du timer sur axtimer_trv_cours_cb\n\n ");
   	}

	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT %s",version);
        
}
