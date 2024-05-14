/*E*/
/*Fichier : $Id: acli_xzag06_cb.c,v 1.8 2020/11/03 16:06:47 pc2dpdy Exp $      Release : $Revision: 1.8 $        Date : $Date: 2020/11/03 16:06:47 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE ACLI_XZAG06 * FICHIER acli_xzag06_cb.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* 	CallBack appele sur reception de message XDM_AG_PURGEHISTO,
*	appartenant a TACLI.x. Le rafraichissement s effectue tous les 24 H
* 	a une heure precise (constante de acli_dir.h).
*
------------------------------------------------------
* HISTORIQUE :
*
* mercier	07 Nov 1994	: Creation
* torregrossa	10 Aou 1995	: Decalage des purges de bases (delta 30mn) V 1.2
* torregrossa	10 Aou 1995	: Renommage fonction appelee V 1.3
* torregrossa	11 Aou 1995	: Suppression du decalage des purges de bases V 1.4
* niepceron	30 sep 1997	: Ajout de la remontee d'erreur dans le journal d'administration v1.5	
* niepceron     03 Jul 1998	:Correction de la remontee d'erreur dans le journal d'administration v1.6
* JPL		05/06/18	: Arguments complets pour appel XZST_10ArchiverMsgSyst (DEM 1274)  1.7
------------------------------------------------------*/

/* fichiers inclus */

#include "acli_xzag06_cb.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: acli_xzag06_cb.c,v 1.8 2020/11/03 16:06:47 pc2dpdy Exp $ : acli_xzag06_cb" ;

/* declaration de fonctions internes */

extern XDY_NomSite vg_NomSite;

/* definition de fonctions externes */

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	axtimer_XZAG06_Purge_Base_Historique_cb recoit le message RTWks XDM_AG_PURGEHISTO,  
* appelle XZAG_Purge_Base_Historique.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void axtimer_XZAG06_Purge_Base_Historique_cb 	(T_IPC_CONN va_Cnx,
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
*	Fonction declenchee sur reception de message XDM_AG_PURGEHISTO.
*	Le message RTwks contient une cle comme tout message provenant de timer.
*	On appele xzag06 afin de purger les donnees de la base d exploitation vieilles 
*	de plus d un mois.
*	Puis on redemande une programmation du timer.
* 	
------------------------------------------------------*/
{
	static char *version = "$Id: acli_xzag06_cb.c,v 1.8 2020/11/03 16:06:47 pc2dpdy Exp $ : acli_axtimer_XZAG06_Purge_Base_Historique_cb" ;
	
	XDY_Datagroup   	vl_datagroup;
        XDY_NomSite     	pl_NomSite;
        char*			vl_Cle_TIMER_PURGE_HISTORIQUE = AXZAG06C_TIMERCLE_XZAG06;
	int			vl_JourSemaine=0;
	double			vl_HorodateSec=0;
	int			vl_Nb_Secondes=0;
	int			vl_ValRet = XDC_OK;
	XZSMT_Date      	vl_Date;
        XZSMT_Heure     	vl_Heure;
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN : axtimer_XZAG06_Purge_Base_Historique_cb : debut d execution ");
 	
 	if ((vl_ValRet = XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME,&vl_JourSemaine,&vl_HorodateSec))!=XZSMC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"acli_axtimer_XZAG06_Purge_Base_Historique : XZSM_07LireHorodate a echoue");
	}
        /*A
	** Recuperation du site de la machine sinon sortie via acli_sortir();
	*/
	strcpy(pl_NomSite,vg_NomSite);
	/*A
	** Appel a XZAG_Purge_Base_Historique avec une horodate de -(5ans +1 jour).
	*/
        if( XZAG_Purge_Base_Historique (vl_HorodateSec-AXZAG06C_OFFSET_PURGE_XZAG06) != XDC_OK)
        {
                XZST_03EcritureTrace( XZSTC_WARNING, "acli_axtimer_XZAG06_Purge_Base_Historique_cb : XZAG_Purge_Base_Historique () a echoue");
		XZST_10ArchiverMsgSyst(0,"PURGE : ++++++++++++++++++++++++++++++++++++++++++++++++++",NULL);
		XZST_10ArchiverMsgSyst(0,"PURGE :  LA PURGE DE L'HISTORIQUE SUR LE SITE %s A ECHOUE ",pl_NomSite,NULL);
		XZST_10ArchiverMsgSyst(0,"PURGE : ++++++++++++++++++++++++++++++++++++++++++++++++++",NULL);
        }
        else
        {
                XZST_03EcritureTrace( XZSTC_DEBUG1,"acli_axtimer_XZAG06_Purge_Base_Historique_cb : XZAG_Purge_Base_Historique () a reussi");
        }
        /*A
        ** Reprogrammation du timer
        */
	
	/*A
        ** Constitution du nom Data Group a partir du nom du site
        */
        
        XDG_EncodeDG2(vl_datagroup,XDG_AG,pl_NomSite);
	
	/*A 
	** La CB de purge de la base historique se declenchera a
	** ACLIDC_HEURE_DECLENCHEMENT_XZAG06 le lendemain de la programmation
	*/
	
	strcpy(vl_Date,"");
	strcpy(vl_Heure,AXZAG06C_HEURE_DECLENCHEMENT_XZAG06);
	if ((vl_ValRet = XZSM_16ConvHeureNbSec(vl_Date,vl_Heure,&vl_HorodateSec))!=XZSMC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"acli_axtimer_XZAG06_Purge_Base_Historique_cb : XZSM_16ConvHeureNbSec a echoue");
		acli_sortir();
	}
	
	vl_Nb_Secondes = vl_HorodateSec + ACLIDC_OFFSET_24H;	
	/*A  
	** Declenchement d'une programmation sur
    	** un datagroup  XDG_AG_NomSite avec le message
   	** XDM_AG_PURGEHISTO ayant comme cle de AXZAG06C_TIMERCLE_XZAG06  ********
   	*/
        vl_ValRet = XZSM_01DemanderProgrammation(
                				vl_Nb_Secondes,
                				0,
                				vl_datagroup,
                				XDM_AG_PURGEHISTO,
                				AXZAG06C_TIMERCLE_XZAG06,
                				XZSMC_SEC_VRAI );
      	if (vl_ValRet != XDC_OK)
   	{
     		XZST_03EcritureTrace(XZSTC_WARNING,"axtimer_XZAG06_Purge_Base_Historique_cb : XZSM_01DemanderProgrammation : Probleme de programmation TIMER pour purge de la base d exploitation \n\n ");
   		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : axtimer_XZAG06_Purge_Base_Historique_cb : Sortie avec %d",vl_ValRet);
		acli_sortir();
   	}
   	else
   	{
   		XZST_03EcritureTrace(XZSTC_DEBUG1,"axtimer_XZAG06_Purge_Base_Historique_cb : XZSM_01DemanderProgrammation a reprogramm≈ avec SUCCES \n\t\tle d≈clenchement du timer sur axtimer_XZAG06_Purge_Base_Historique_cb\n\n ");
   	}

	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : axtimer_XZAG06_Purge_Base_Historique_cb : Fin d execution ");
        
}

