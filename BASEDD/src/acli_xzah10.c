/*E*/
/*Fichier : $Id: acli_xzah10.c,v 1.13 2020/11/17 15:43:02 pc2dpdy Exp $      Release : $Revision: 1.13 $        Date : $Date: 2020/11/17 15:43:02 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE ACLI_XZAH10 * FICHIER acli_xzah10.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* 	CallBack appele sur reception de message XDM_A_TIMER_RAFRAI_IHM,
*	appartenant a TACLI.x. Il permet le rafraichissement de l IHM. 
*	Il s effectue tous les 5secondes environ (constante de acli_dir.h).
***********************************
*	axtimer_Ihm_cb recoit le message RTWks XDM_A_TIMER_RAFRAI_IHM,  
* appelle XZAH10_Animer_IHM.
*********************************************************************
*
------------------------------------------------------
* HISTORIQUE :
*
* mercier	02 Nov 1994	: Creation
* T.millevilel	11 Jul 1995	: Suppression reprogrammation Timer
*	car ajout argument cyclique a l'init V1.4
* niepceron	27 Aou 1996	: Ajout de axtimer_Ihm_bloc_cb	1.5
* niepceron     09 Sep 1996	: Modification de l'interface XZIS20 v1.6
* niepceron 	05 Mai 1998	: ajout axtimer_FMC_locales_cb v1.7
* niepceron	21 Aou 1998	: Ajout PC3 DY et DP (dem/1700) v1.8
* claudel       09 Jul 2007     : Suppression DY v1.9
* JMG           02/10/09        : secto DEM887 1.10
* JMG		06/10/20	: Modif animation synoptiques PRA DEM-SAE93 1.11
* LCL		27/09/23 : Adaptations CNA DEM-483	
------------------------------------------------------*/

/* fichiers inclus */

#include "acli_xzah10.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: acli_xzah10.c,v 1.13 2020/11/17 15:43:02 pc2dpdy Exp $ : acli_xzah10" ;
static int  		vm_nb_lignes=0;
static XZIST_FMC_LOC	*pm_FMC_Locales = NULL;
extern int vg_PRA;
extern XDY_NomSite     	vg_NomSite;
/* declaration de fonctions internes */
int acli_FMC_locales();

/* definition de fonctions externes */

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	axtimer_Ihm_cb recoit le message RTWks XDM_A_TIMER_RAFRAI_IHM,  
* appelle XZAH10_Animer_IHM.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void axtimer_Ihm_cb 	(T_IPC_CONN 			va_Cnx,
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
*	Fonction declenchee sur reception de message XDM_A_TIMER_RAFRAI_IHM.
*	Le message RTwks contient une cle comme tout message provenant de timer.
*	On appele xzah10 afin d animer l IHM.
*	Puis on redemande une programmation du timer.
* 	
------------------------------------------------------*/
{
	static char *version = "$Id: acli_xzah10.c,v 1.13 2020/11/17 15:43:02 pc2dpdy Exp $ : acli_axtimer_Ihm_cb" ;
	
	XDY_Datagroup   	vl_datagroup;
        XDY_NomSite     	pl_NomSite;
        char*			vl_Cle_TIMER_RAFRAI_IHM = AXZAH10C_TIMERCLE_XZAH10;
	int			vl_JourSemaine=0;
	double			vl_HorodateSec=0;
	int			vl_Nb_Secondes=0;
	int			vl_ValRet = XDC_OK;
	
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN %s",version);
	
        /*A
	** Recuperation du site de la machine sinon sortie via acli_sortir();
	*/
	strcpy(pl_NomSite, vg_NomSite);
	if (vg_PRA==XDC_NOK) {
		XZST_03EcritureTrace(XZSTC_WARNING,"appel XZAH10 site %s", pl_NomSite);
        	if( XZAH10_Animer_IHM (pl_NomSite) != XDC_OK) 
                	XZST_03EcritureTrace( XZSTC_WARNING, "%s : XZAH10_Animer_IHM () a echoue",version);
        	else
                	XZST_03EcritureTrace( XZSTC_DEBUG1,"%s : XZAH10_Animer_IHM () a reussi",version);
        }
	else {
		XZST_03EcritureTrace(XZSTC_WARNING,"appel XZAH10 site NULL");
        	if( XZAH10_Animer_IHM (NULL) != XDC_OK) 
                	XZST_03EcritureTrace( XZSTC_WARNING, "%s : XZAH10_Animer_IHM () a echoue",version);
        	else
                	XZST_03EcritureTrace( XZSTC_DEBUG1,"%s : XZAH10_Animer_IHM () a reussi",version);



        }

        /*A
        ** Reprogrammation du timer
        */
	
	/*A
        ** Constitution du nom Data Group a partir du nom du site
        */
        
        XDG_EncodeDG2(vl_datagroup,XDG_A_TIMER_RAFRAI_IHM,pl_NomSite);
	
	/*A 
	** La CB de refraichissement IHM se declenchera 
	** dans AXZAH10C_TIMERDELAI_XZAH10 secondes apres programmation
	*/
	
	if ((vl_ValRet = XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME,&vl_JourSemaine,&vl_HorodateSec))!=XZSMC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"acli_axtimer_Ihm_cb : XZSM_07LireHorodate a echoue");
		acli_sortir();
	}
	
	vl_Nb_Secondes = vl_HorodateSec + AXZAH10C_TIMERDELAI_XZAH10;	
	/*A  
	** Declenchement d'une programmation sur
    	** un datagroup  XDG_A_TIMER_RAFRAI_IHM_NomSite avec le message
   	** XDM_A_TIMER_RAFRAI_IHM ayant comme cle de AXZAH10C_TIMERCLE_XZAH10  ********
   	*/
        vl_ValRet = XZSM_01DemanderProgrammation(
                				vl_Nb_Secondes,
                				0,
                				vl_datagroup,
                				XDM_A_TIMER_RAFRAI_IHM,
                				AXZAH10C_TIMERCLE_XZAH10,
                				XZSMC_SEC_VRAI );
      	if (vl_ValRet != XDC_OK)
   	{
     		XZST_03EcritureTrace(XZSTC_WARNING,"axtimer_Ihm_cb : XZSM_01DemanderProgrammation : Probleme de programmation TIMER pour rafraichissement IHM \n\n ");
   		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : axtimer_Ihm_cb : Sortie avec %d",vl_ValRet);
		acli_sortir();
   	}
   	else
   	{
   		XZST_03EcritureTrace(XZSTC_DEBUG1,"axtimer_Ihm_cb : XZSM_01DemanderProgrammation a reprogramm� avec SUCCES \n\t\tle d�clenchement du timer sur axtimer_Ihm_cb\n\n ");
   	}

	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT %s",version);
        
}
/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	axtimer_Ihm_cb recoit le message RTWks XDM_A_TIMER_RAFRAI_IHM,  
* appelle XZAH10_Animer_IHM.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void axtimer_Ihm_bloc_cb 	(T_IPC_CONN 			va_Cnx,
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
*	Fonction declenchee sur reception de message XDM_A_TIMER_RAFRAI_IHM.
*	Le message RTwks contient une cle comme tout message provenant de timer.
*	On appele xzah10 afin d animer l IHM.
*	Puis on redemande une programmation du timer.
* 	
------------------------------------------------------*/
{
	static char *version = "$Id: acli_xzah10.c,v 1.13 2020/11/17 15:43:02 pc2dpdy Exp $ : acli_axtimer_Ihm_bloc_cb" ;
	
	XDY_Datagroup   	vl_datagroup;
        XDY_NomSite     	pl_NomSite;
	int			vl_JourSemaine=0;
	double			vl_HorodateSec=0;
	int			vl_Nb_Secondes=0;
	int			vl_ValRet = XDC_OK;
	
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN %s",version);

 	
 	
        if( XZAH10_Animer_IHM (NULL) != XDC_OK)
        {
                XZST_03EcritureTrace( XZSTC_WARNING, "%s : XZAH10_Animer_IHM () a echoue",version);
        }
        else
        {
                XZST_03EcritureTrace( XZSTC_DEBUG1,"%s : XZAH10_Animer_IHM () a reussi",version);
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
        
        XDG_EncodeDG2(vl_datagroup,XDG_A_TIMER_RAF_IHM_BLOC,pl_NomSite);
	
	/*A 
	** La CB de refraichissement IHM se declenchera 
	** dans AXZAH10C_TIMERDELAI_XZAH10_BLOC secondes apres programmation
	*/
	
	if ((vl_ValRet = XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME,&vl_JourSemaine,&vl_HorodateSec))!=XZSMC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"acli_axtimer_Ihm_bloc_cb : XZSM_07LireHorodate a echoue");
		acli_sortir();
	}
	
	vl_Nb_Secondes = vl_HorodateSec + AXZAH10C_TIMERDELAI_XZAH10_BLOC;	
	/*A  
	** Declenchement d'une programmation sur
    	** un datagroup  XDG_A_TIMER_RAF_IHM_BLOC_NomSite avec le message
   	** XDM_A_TIMER_RAFRAI_IHM ayant comme cle de AXZAH10C_TIMERCLE_XZAH10_BLOC  ********
   	*/
        vl_ValRet = XZSM_01DemanderProgrammation(
                				vl_Nb_Secondes,
                				0,
                				vl_datagroup,
                				XDM_A_TIMER_RAFRAI_IHM,
                				AXZAH10C_TIMERCLE_XZAH10_BLOC,
                				XZSMC_SEC_VRAI );
      	if (vl_ValRet != XDC_OK)
   	{
     		XZST_03EcritureTrace(XZSTC_WARNING,"axtimer_Ihm_bloc_cb : XZSM_01DemanderProgrammation : Probleme de programmation TIMER pour rafraichissement IHM_BLOC \n\n ");
   		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : axtimer_Ihm_bloc_cb : Sortie avec %d",vl_ValRet);
		acli_sortir();
   	}
   	else
   	{
   		XZST_03EcritureTrace(XZSTC_DEBUG1,"axtimer_Ihm_bloc_cb : XZSM_01DemanderProgrammation a reprogramm� avec SUCCES \n\t\tle d�clenchement du timer sur axtimer_Ihm__BLOCcb\n\n ");
   	}

	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT %s",version);
        
}

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	axtimer_Ihm_cb recoit le message RTWks XDM_A_TIMER_RAFRAI_IHM,  
* appelle XZAH10_Animer_IHM.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void axtimer_FMC_locales_cb 	(T_IPC_CONN 			va_Cnx,
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
*	Fonction declenchee sur reception de message XDM_A_TIMER_FMC_LOC.
*	Le message RTwks contient une cle comme tout message provenant de timer.
*	On appele xzah10 afin d animer l IHM.
*	Puis on redemande une programmation du timer.
* 	
------------------------------------------------------*/
{
	static char *version = "$Id: acli_xzah10.c,v 1.13 2020/11/17 15:43:02 pc2dpdy Exp $ : acli_axtimer_FMC_locales_cb" ;
	
	XDY_Datagroup   	vl_datagroup;
        XDY_NomSite     	pl_NomSite;
	int			vl_JourSemaine=0;
	double			vl_HorodateSec=0;
	int			vl_Nb_Secondes=0;
	int			vl_ValRet = XDC_OK;
	
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN %s",version);

	/*A Recuperation de la liste de FMC locales de VC */
	vm_nb_lignes=0;
	pm_FMC_Locales=NULL;
	if ( XZAE101_Liste_FMC_Locales( XDC_NOM_SITE_VC , acli_FMC_locales )== XDC_NOK )
        {
                XZST_03EcritureTrace( XZSTC_WARNING, "%s : XZAE101_Liste_FMC_Locales (XDC_NOM_SITE_VC,fonct) a echoue",version);
        }
        else
        {
                XZST_03EcritureTrace( XZSTC_DEBUG1,"%s : XZAE101_Liste_FMC_Locales (XDC_NOM_SITE_VC,fonct) a reussi",version);
        }
	/*A Envoi de la liste de FMC locales de DC */
	if ( XZIS20_Envoyer_FMC_Locales( XDC_NOM_SITE_VC , pm_FMC_Locales , vm_nb_lignes)== XDC_NOK )
        {
                XZST_03EcritureTrace( XZSTC_WARNING, "%s :  XZIS20_Envoyer_FMC_Locales(XDC_NOM_SITE_VC) a echoue",version);
        }
        else
        {
                XZST_03EcritureTrace( XZSTC_DEBUG1,"%s : XZIS20_Envoyer_FMC_Locales (XDC_NOM_SITE_VC) a reussi",version);
        }
        free(pm_FMC_Locales); 
 	
	/*A Recuperation de la liste de FMC locales de DP */
	vm_nb_lignes=0;
	pm_FMC_Locales=NULL;
	if ( XZAE101_Liste_FMC_Locales( XDC_NOM_SITE_DP, acli_FMC_locales )== XDC_NOK )
	{
		XZST_03EcritureTrace( XZSTC_WARNING, "%s : XZAE101_Liste_FMC_Locales (XDC_NOM_SITE_DP,fonct) a echoue",version);
        }
	else
	{
		XZST_03EcritureTrace( XZSTC_DEBUG1,"%s : XZAE101_Liste_FMC_Locales (XDC_NOM_SITE_DP,fonct) a reussi",version);
        }
	/*A Envoi de la liste de FMC locales de DP */
	if ( XZIS20_Envoyer_FMC_Locales( XDC_NOM_SITE_DP , pm_FMC_Locales, vm_nb_lignes )== XDC_NOK )
	{
		XZST_03EcritureTrace( XZSTC_WARNING, "%s :  XZIS20_Envoyer_FMC_Locales(XDC_NOM_SITE_DP) a echoue",version);
        }
	else
	{
		XZST_03EcritureTrace( XZSTC_DEBUG1,"%s : XZIS20_Envoyer_FMC_Locales (XDC_NOM_SITE_DP) a reussi",version);
	}
	free(pm_FMC_Locales);

        /*A
        ** Reprogrammation du timer
        */
        /*A
	** Recuperation du site de la machine sinon sortie via acli_sortir();
	*/
	strcpy(pl_NomSite, vg_NomSite);
	
	/*A
        ** Constitution du nom Data Group a partir du nom du site
        */
        
        XDG_EncodeDG2(vl_datagroup,XDG_A_TIMER_FMC_LOC,pl_NomSite);
	
	/*A 
	** La CB de refraichissement IHM se declenchera 
	** dans AXZAE101C_TIMERDELAI_XZAE101 secondes apres programmation
	*/
	
	if ((vl_ValRet = XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME,&vl_JourSemaine,&vl_HorodateSec))!=XZSMC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"acli_axtimer_FMC_locales_cb : XZSM_07LireHorodate a echoue");
		acli_sortir();
	}
	
	vl_Nb_Secondes = vl_HorodateSec + AXZAE101C_TIMERDELAI_XZAE101;	
	/*A  
	** Declenchement d'une programmation sur
    	** un datagroup  XDG_A_TIMER_FMC_LOC_NomSite avec le message
   	** XDM_A_TIMER_FMC_LOC ayant comme cle de  AXZAE101C_TIMERCLE_XZAE101 ********
   	*/
        vl_ValRet = XZSM_01DemanderProgrammation(
                				vl_Nb_Secondes,
                				0,
                				vl_datagroup,
                				XDM_A_TIMER_FMC_LOC,
                				AXZAE101C_TIMERCLE_XZAE101,
                				XZSMC_SEC_VRAI );
      	if (vl_ValRet != XDC_OK)
   	{
     		XZST_03EcritureTrace(XZSTC_WARNING,"axtimer_FMC_locales_cb : XZSM_01DemanderProgrammation : Probleme de programmation TIMER pour rafraichissement IHM_BLOC \n\n ");
   		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : axtimer_FMC_locales_cb : Sortie avec %d",vl_ValRet);
		acli_sortir();
   	}
   	else
   	{
   		XZST_03EcritureTrace(XZSTC_DEBUG1,"axtimer_FMC_locales_cb : XZSM_01DemanderProgrammation a reprogramm� avec SUCCES \n\t\tle d�clenchement du timer sur axtimer_Ihm__BLOCcb\n\n ");
   	}

	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT %s",version);
        
}


/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	acli_FMC_locales recupere et memorise les FMC locales
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int acli_FMC_locales 	(XZIST_FMC_LOC va_ligne) 
/*
* ARGUMENTS EN ENTREE :
*
*   	va_ligne: Caracteriqtiques d'une FMC locales.
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
*	reallocation du tableau pm_FMC_Locales et ajout de la ligne passee
*	en argument
* 	
------------------------------------------------------*/
{
	static char *version = "$Id: acli_xzah10.c,v 1.13 2020/11/17 15:43:02 pc2dpdy Exp $ : acli_FMC_locales" ;
	
	
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN %s",version);
	
	/*A Reallocation du tableau pm_FMC_Locale */
	pm_FMC_Locales=realloc(pm_FMC_Locales, (vm_nb_lignes +1) * sizeof( XZIST_FMC_LOC ));
	
	/*A Ajout de la ligne va_ligne pm_FMC_Locale */
	pm_FMC_Locales[vm_nb_lignes]=va_ligne;
	vm_nb_lignes++;
	XZST_03EcritureTrace(XZSTC_DEBUG1,"%s: vm_nb_lignes:%d",version,vm_nb_lignes);
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT %s",version);
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN %s",version);
	return XDC_OK;
}
