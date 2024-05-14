/*E*/
/*Fichier : $Id: acli_xzan34_cb.c,v 1.2 2020/11/03 15:45:38 pc2dpdy Exp $      Release : $Revision: 1.2 $        Date : $Date: 2020/11/03 15:45:38 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE ACLI_XZAN34CB * FICHIER acli_xzan34_cb.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale
*   ou reference au fichier acli_xzan34_cb.h >
*
------------------------------------------------------
* HISTORIQUE :
*
* mercier	07 Nov 1994	: Creation
------------------------------------------------------*/

/* fichiers inclus */

#include "acli_xzan34_cb.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: acli_xzan34_cb.c,v 1.2 2020/11/03 15:45:38 pc2dpdy Exp $ : acli_xzan34_cb" ;

/* declaration de fonctions internes */
extern XDY_NomSite vg_NomSite;

/* definition de fonctions externes */

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	axtimer_XZAN34_Epurer_Calendrier_Astreinte_cb recoit le message RTWks 
*  XDM_AN_PURGE sur XDG_AN_site,  
* appelle XZAN34_Epurer_Calendrier_Astreinte.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void axtimer_XZAN34_Epurer_Calendrier_Astreinte_cb 	(T_IPC_CONN 	va_Cnx,
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
*	On doit etre sur le site CI, le RTserveur doit tourner et pour que
*  XZAN34_Epurer_Calendrier_Astreinte (appelee par ce callb.) s execute correctement
*  le SQL_Serveur doit tourner.
*
* FONCTION 
*	Fonction declenchee sur reception de message XDM_AN_PURGE.
*	Le message RTwks contient une cle comme tout message provenant de timer.
*	On appele xzan34 afin de purger le calendrier des astreintes.
*	Puis on redemande une programmation du timer.
* 	
------------------------------------------------------*/
{
	static char *version = "$Id: acli_xzan34_cb.c,v 1.2 2020/11/03 15:45:38 pc2dpdy Exp $ : acli_axtimer_XZAN34_Epurer_Calendrier_Astreinte_cb" ;
	
	XDY_Datagroup   	vl_datagroup;
        XDY_NomSite     	pl_NomSite;
        char*			vl_Cle_TIMER_RAFRAI_IHM = AXZAN34C_TIMERCLE_XZAN34;
	int			vl_JourSemaine=0;
	double			vl_HorodateSec=0;
	int			vl_Nb_Secondes=0;
	int			vl_ValRet = XDC_OK;
	XZSMT_Date      	vl_Date;
        XZSMT_Heure     	vl_Heure;
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN : axtimer_XZAN34_Epurer_Calendrier_Astreinte_cb : debut d execution ");
 	/*A
	** Recuperation du site de la machine sinon sortie via acli_sortir();
	*/
	strcpy(pl_NomSite, vg_NomSite);
 	/*A
	** On teste si on n est bien sur le CI sinon on sort.
	*/
	if(!strcmp(pl_NomSite,XDC_NOM_SITE_CI))
	{
		XZST_03EcritureTrace( XZSTC_DEBUG1,"acli_axtimer_XZAN34_Epurer_Calendrier_Astreinte_cb : Declenchement sur le CI autorise");
	}
	else
	{
		XZST_03EcritureTrace( XZSTC_WARNING, "acli_axtimer_XZAN34_Epurer_Calendrier_Astreinte_cb : Erreur : ce CB ne doit etre arme que sur le site CI et non pas sur le site %s .",pl_NomSite);
		return;
	}
        if( XZAN34_Epurer_Calendrier_Astreinte () != XDC_OK)
        {
                XZST_03EcritureTrace( XZSTC_WARNING, "acli_axtimer_XZAN34_Epurer_Calendrier_Astreinte_cb : XZAN34_Epurer_Calendrier_Astreinte () a echoue");
        }
        else
        {
                XZST_03EcritureTrace( XZSTC_DEBUG1,"acli_axtimer_XZAN34_Epurer_Calendrier_Astreinte_cb : XZAN34_Epurer_Calendrier_Astreinte () a reussi");
        }
        /*A
        ** Reprogrammation du timer
        */
	/*A
        ** Constitution du nom Data Group a partir du nom du site
        */
        
        XDG_EncodeDG2(vl_datagroup,XDG_AN,pl_NomSite);
	
	/*A 
	** La CB de purge du calendrier des astreintes se declenchera a
	** ACLIDC_HEURE_DECLENCHEMENT_XZAG06 le lendemain de la programmation
	*/
	
	strcpy(vl_Date,"");
	strcpy(vl_Heure,AXZAN34C_HEURE_DECLENCHEMENT_XZAN34);
	if ((vl_ValRet = XZSM_16ConvHeureNbSec(vl_Date,vl_Heure,&vl_HorodateSec))!=XZSMC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"acli_axtimer_XZAN34_Epurer_Calendrier_Astreinte_cb : XZSM_07LireHorodate a echoue");
		acli_sortir();
	}
	
	vl_Nb_Secondes = vl_HorodateSec + ACLIDC_OFFSET_24H;	
	/*A  
	** Declenchement d'une programmation sur
    	** un datagroup  XDG_AN_NomSite avec le message
   	** XDM_AN_PURGE ayant comme cle de AXZAN34C_TIMERCLE_XZAN34  ********
   	*/
        vl_ValRet = XZSM_01DemanderProgrammation(
                				vl_Nb_Secondes,
                				0,
                				vl_datagroup,
                				XDM_AN_PURGE,
                				AXZAN34C_TIMERCLE_XZAN34,
                				XZSMC_SEC_VRAI );
      	if (vl_ValRet != XDC_OK)
   	{
     		XZST_03EcritureTrace(XZSTC_WARNING,"axtimer_XZAN34_Epurer_Calendrier_Astreinte_cb : XZSM_01DemanderProgrammation :\n Probleme de programmation TIMER pour rafraichissement IHM \n\n ");
   		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : axtimer_XZAN34_Epurer_Calendrier_Astreinte_cb : Sortie avec %d",vl_ValRet);
		acli_sortir();
   	}
   	else
   	{
   		XZST_03EcritureTrace(XZSTC_DEBUG1,"axtimer_XZAN34_Epurer_Calendrier_Astreinte_cb : XZSM_01DemanderProgrammation a reprogramm≈ avec SUCCES \n\t\tle d≈clenchement du timer sur axtimer_XZAN34_Epurer_Calendrier_Astreinte_cb\n\n ");
   	}

	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : axtimer_XZAN34_Epurer_Calendrier_Astreinte_cb : Fin d execution ");
        
}

