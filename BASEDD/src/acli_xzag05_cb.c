/*E*/
/*Fichier : $Id: acli_xzag05_cb.c,v 1.11 2020/11/03 15:44:56 pc2dpdy Exp $      Release : $Revision: 1.11 $        Date : $Date: 2020/11/03 15:44:56 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE ACLI_XZAG05_CB * FICHIER acli_xzag05_cb.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* 	CallBack appele sur reception de message XDM_AG_PURGE,
*	appartenant a TACLI.x. Le rafraichissement s effectue tous les 5secondes
* 	environ (constante de acli_dir.h).

*
------------------------------------------------------
* HISTORIQUE :
*
* mercier	07 Nov 1994	: Creation
* torregrossa	21 Jui 1994	: Changer le nom de la fonction de purge appelee
* torregrossa	10 Aou 1995	: Decalage des purges de bases (delta 30mn) V 1.3
* torregrossa	09 Sep 1996	: Ajout cas des purges sur pc simplifies DS et DC (RADT V1.4)
* niepceron	30 sep 1997	: Ajout de la remontee d'erreur dans le journal d'administration v1.5	
* JMG           02/10/09        : secto DEM887 1.6 1.7
* JPL		05/06/18	: Arguments complets pour appel XZST_10ArchiverMsgSyst (DEM 1274)  1.9
* niepceron	15/10/19	: suppression VE et replanification des purges EXP v1.10	
------------------------------------------------------*/

/* fichiers inclus */

#include "acli_xzag05_cb.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: acli_xzag05_cb.c,v 1.11 2020/11/03 15:44:56 pc2dpdy Exp $ : acli_xzag05_cb" ;

extern XDY_NomSite vg_NomSite;

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	axtimer_XZAG05_Purge_Base_Exploitation_cb recoit le message RTWks XDM_AG_PURGE,  
* appelle XZAG_Purge_Base_Exploitation.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void axtimer_XZAG05_Purge_Base_Exploitation_cb 	(T_IPC_CONN va_Cnx,
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
*	Fonction declenchee sur reception de message XDM_AG_PURGE.
*	Le message RTwks contient une cle comme tout message provenant de timer.
*	On appele xzag05 afin de purger les donnees de la base d exploitation vieilles 
*	de plus d un mois.
*	Puis on redemande une programmation du timer.
* 	
------------------------------------------------------*/
{
	static char *version = "$Id: acli_xzag05_cb.c,v 1.11 2020/11/03 15:44:56 pc2dpdy Exp $ : acli_axtimer_XZAG05_Purge_Base_Exploitation_cb" ;
	
	XDY_Datagroup   	vl_datagroup;
        XDY_NomSite     	pl_NomSite;
        int		     	vl_TypeMachine=0;
        char*			vl_Cle_TIMER_PURGE_EXPLOIT = AXZAG05C_TIMERCLE_XZAG05;
	int			vl_JourSemaine=0;
	double			vl_HorodateSec=0;
	int			vl_Nb_Secondes=0;
	int			vl_ValRet = XDC_OK;
	XZSMT_Date      	vl_Date;
        XZSMT_Heure     	vl_Heure;
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN : axtimer_XZAG05_Purge_Base_Exploitation_cb : debut d execution ");
 	
 	if ((vl_ValRet = XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME,&vl_JourSemaine,&vl_HorodateSec))!=XZSMC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"acli_axtimer_XZAG05_Purge_Base_Exploitation n a pu recuperer l horodate");
	}
	/*A
	** Recherche type de pc 
	*/
	if ( (vl_ValRet =XZSC_05TypeMachine(&vl_TypeMachine)) != XDC_OK)
	{
		XZST_03EcritureTrace( XZSTC_FATAL, "acli_axtimer_XZAG05_Purge_Base_Exploitation : Recuperation du type machine impossible .");
		exit(1);
	}
	else
	{
		XZST_03EcritureTrace( XZSTC_DEBUG1, "acli_axtimer_XZAG05_Purge_Base_Exploitation : Recuperation du type machine %d.",vl_TypeMachine);
	}
        /*A
	** Recuperation du site de la machine sinon sortie via acli_sortir();
	*/
	strcpy(pl_NomSite,vg_NomSite);
	/*A
	** Appel a XZAG_Purge_Base_Exploitation avec une horodate de -32 jours.
	*/
        if( XZAG_Purge_Base_Exploitation (vl_HorodateSec-AXZAG05C_OFFSET_PURGE_XZAG05, vl_TypeMachine) != XDC_OK)
        {
                XZST_03EcritureTrace( XZSTC_WARNING, "acli_axtimer_XZAG05_Purge_Base_Exploitation_cb : XZAG_Purge_Base_Exploitation () a echoue");
        	XZST_10ArchiverMsgSyst(0,"PURGE : ++++++++++++++++++++++++++++++++++++++++++++++++++++",NULL);
        	XZST_10ArchiverMsgSyst(0,"PURGE :  LA PURGE DE L'EXPLOITATION SUR LE SITE %s A ECHOUE ",pl_NomSite,NULL);
        	XZST_10ArchiverMsgSyst(0,"PURGE : ++++++++++++++++++++++++++++++++++++++++++++++++++++",NULL);
        }
        else
        {
                XZST_03EcritureTrace( XZSTC_DEBUG1,"acli_axtimer_XZAG05_Purge_Base_Exploitation_cb : XZAG_Purge_Base_Exploitation () a reussi");
        }
        /*A
        ** Reprogrammation du timer
        */
	
	/*A
        ** Constitution du nom Data Group a partir du nom du site
        */
        
        XDG_EncodeDG2(vl_datagroup,XDG_AG,pl_NomSite);
	
	/*A 
	** La CB de purge de la base d exploitation se declenchera a
	** ACLIDC_HEURE_DECLENCHEMENT_XZAG05 le lendemain de la programmation
	*/
	
	strcpy(vl_Date,"");
	strcpy(vl_Heure,AXZAG05C_HEURE_DECLENCHEMENT_XZAG05);
	if ((vl_ValRet = XZSM_16ConvHeureNbSec(vl_Date,vl_Heure,&vl_HorodateSec))!=XZSMC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"acli_axtimer_XZAG05_Purge_Base_Exploitation_cb : XZSM_16ConvHeureNbSec a echoue");
		acli_sortir();
	}
	
	/*A
	** Decaler la purge de ACLIDC_DELTA_PURGE_INTER_SITE minutes pour Mandelieu
	** et de 2 * ACLIDC_DELTA_PURGE_INTER_SITE minutes pour le CI
	*/
   		if (!strcmp(pl_NomSite,XDC_NOM_SITE_VC))
			vl_HorodateSec = vl_HorodateSec ;
   		else if (!strcmp(pl_NomSite,XDC_NOM_SITE_DP))
			vl_HorodateSec = vl_HorodateSec + 1.5 * ACLIDC_DELTA_PURGE_INTER_SITE;
   		else if (!strcmp(pl_NomSite,XDC_NOM_SITE_CI))
			vl_HorodateSec = vl_HorodateSec + 3 * ACLIDC_DELTA_PURGE_INTER_SITE;

	vl_Nb_Secondes = vl_HorodateSec + ACLIDC_OFFSET_24H ;	
	/*A  
	** Declenchement d'une programmation sur
    	** un datagroup  XDG_AG_NomSite avec le message
   	** XDM_AG_PURGE ayant comme cle de AXZAG05C_TIMERCLE_XZAG05  ********
   	*/
        vl_ValRet = XZSM_01DemanderProgrammation(
                				vl_Nb_Secondes,
                				0,
                				vl_datagroup,
                				XDM_AG_PURGE,
                				AXZAG05C_TIMERCLE_XZAG05,
                				XZSMC_SEC_VRAI );
      	if (vl_ValRet != XDC_OK)
   	{
     		XZST_03EcritureTrace(XZSTC_WARNING,"axtimer_XZAG05_Purge_Base_Exploitation_cb : XZSM_01DemanderProgrammation : Probleme de programmation TIMER pour purge de la base d exploitation \n\n ");
   		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : axtimer_XZAG05_Purge_Base_Exploitation_cb : Sortie avec %d",vl_ValRet);
		acli_sortir();
   	}
   	else
   	{
   		XZST_03EcritureTrace(XZSTC_DEBUG1,"axtimer_XZAG05_Purge_Base_Exploitation_cb : XZSM_01DemanderProgrammation a reprogramm≈ avec SUCCES \n\t\tle d≈clenchement du timer sur axtimer_XZAG05_Purge_Base_Exploitation_cb\n\n ");
   	}

	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : axtimer_XZAG05_Purge_Base_Exploitation_cb : Fin d execution ");
        
}

