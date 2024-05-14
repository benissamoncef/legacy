/*E*/
/*Fichier : $Id: csva_sbd.c,v 1.4 1997/05/15 10:22:30 gaborit Exp $      Release : $Revision: 1.4 $        Date : $Date: 1997/05/15 10:22:30 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  GECONF
------------------------------------------------------
* MODULE SBD * FICHIER csva_sbd.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*	Module de TCSVA permettant d effectuer automatiquement
* les sauvegardes base CFG et HISTO.
*
********
*	csbd_sauvebd_cb est declenchee sur reception du message RTWks XDM_CSV_SBD.
*	Ce CallBack va effectuer une sauvegarde des bases sur bande et vide le journal
* des transactions,
* previent l administrateur de l etat de la sauvegarde.
*	
********
*	csbd_ProgramTimer_Hebdo fonction qui permet de reprogrammer
* le timer pour le message declenchant la sauvegarde hebdomadaire
* globale de la base CFG et incrementale de la base histo.
*
------------------------------------------------------
* HISTORIQUE :
*
* mercier	06 Jan 1995	: Creation
* niepceron	06 Mar 1995	: correction du calcul de la date de svg 1.2
------------------------------------------------------*/

/* fichiers inclus */

#include "csva_sbd.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: csva_sbd.c,v 1.4 1997/05/15 10:22:30 gaborit Exp $ : csva_sbd" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	csbd_sauvebd_cb est declenchee sur reception du message RTWks XDM_CSV_SBD.
*	Ce CallBack va effectuer une sauvegarde des bases sur bande et vide le journal
* des transactions,
* previent l administrateur de l etat de la sauvegarde.
*	
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void csbd_sauvebd_cb (	T_IPC_CONN 			va_Cnx,
        		T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        		T_CB_ARG                        pa_ARG) 
/*
* ARGUMENTS EN ENTREE :
*
*   	Arguments d'une callback RTServer.
*
* ARGUMENTS EN SORTIE :   aucun
* CODE RETOUR :  aucun
* CONDITION D'UTILISATION
*
*
* FONCTION 
*	Fonction declenchee sur reception de message XDM_CSV_SBD.	
* 	
------------------------------------------------------*/
{
	static char *version = "$Id: csva_sbd.c,v 1.4 1997/05/15 10:22:30 gaborit Exp $ : csbd_sauvebd_cb" ;
	
	/* Variables locales pour messages RTWks */
	
	XDY_Entier 		vl_Mois 	= 0;		/* type RTwks : int4*/
	XDY_Entier 		vl_Annee	= 0;		/* type RTwks : int4*/
	XDY_Entier 		vl_TapeID	= 0;
	int			vl_retcode    	= XDC_OK;	/* code de retour 	*/
	XDY_NomMachine  	vl_NomMachine 	= "";
	char			vl_Msg[500] 	= "";	
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN : csbd_sauvebd_cb : Debut d execution");
	sprintf(vl_Msg,"La Demande de Sauvegarde hebdomadaire de CFG et de HIST sur DAT est en cours.");
	XZIA08_AffMsgAdm(CSVAV_NOMMACHINE,vl_Msg);

	if ((vl_retcode = XZAG08_Sauvegarder_Base(	CSBDC_SITE_SAUVEGARDE,
							CSBDC_DEVICE_SAUVEGARDE,
							CSBDC_NOM_VOLUME_HEBDO,
							CSBDC_SVG_GLOB,
							CSBDC_BASE_HEBDO,
							CSBDC_FIC_TRACE_HEBDO,
							CSBDC_CAPACITE_DAT,
							CSBDC_NOMFIC_DAT,
							vl_NomMachine ))!=XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING ,"TCSVA : csbd_sauvebd_cb : XZAG08_Sauvegarder_Base de CFG a echoue");	
		XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : csbd_sauvebd_cb :  Sortie");
		sprintf(vl_Msg,"La Sauvegarde hebdomadaire de CFG et de HIST sur DAT a échouée.");
		XZIA08_AffMsgAdm(CSVAV_NOMMACHINE,vl_Msg);
		return;
	}
	else
	{
		/*XZST_10ArchiverMsgSyst(XZSTC_INFO,"La Sauvegarde hebdomadaire de CFG et de HIST sur DAT s est bien deroulee");*/
		XZST_03EcritureTrace(XZSTC_INFO ,"TCSVA : La Sauvegarde hebdomadaire de CFG et de HIST sur DAT s est bien deroulee");
		sprintf(vl_Msg,"La Sauvegarde hebdomadaire de CFG et de HIST sur DAT s'est bien deroulée");
		XZIA08_AffMsgAdm(CSVAV_NOMMACHINE,vl_Msg);
	}
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : csbd_sauvebd_cb : Fin d execution");
	return;			
}
/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	csbd_ProgramTimer_Hebdo fonction qui permet de reprogrammer
* le timer pour le message declenchant l ouverture d une fenetre 
* d information pour l administrateur, lui rappelant qu il doit effectuer
* chaque mois l archivage de la base hist.
*	
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int csbd_ProgramTimer_Hebdo () 
/*
* ARGUMENTS EN ENTREE :
*
* ARGUMENTS EN SORTIE :   aucun
* CODE RETOUR :  aucun
* CONDITION D'UTILISATION
*
*
* FONCTION 
*	
------------------------------------------------------*/
{
	static char *version = "$Id: csva_sbd.c,v 1.4 1997/05/15 10:22:30 gaborit Exp $ : csbd_ProgramTimer_Hebdo" ;
	
	XZSMT_Horodate 		vl_horodate;
	XDY_Horodate		vl_Nb_Secondes	= 0.0;	/* le timer est a declenchement cyclique */
	int			vl_JourSemaine=0;
	XDY_Horodate		vl_HorodateSec=0;
	XZSMT_Horodate  	vl_DATE_CHAR;
	char			vl_MoisChar[3]="";
	int			vl_Mois = 0;
	char			vl_AnneeChar[5]="";
	int			vl_Annee = 0;
	XDY_Datagroup   	vl_datagroup;
        XDY_NomSite     	pl_NomSite;
	int			vl_retcode    = XDC_OK;		/* code de retour 	*/
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN : csbd_ProgramTimer_Hebdo : Debut d execution");
	/*A 
	** Recuperation de l horodate courante
	*/
	if ((vl_retcode = XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME,&vl_JourSemaine,&vl_HorodateSec))!=XZSMC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"csbd_ProgramTimer_Hebdo : XZSM_07LireHorodate a echoue");
		csva_sortir();
	}
	/*A
	** Traduction en chaine pour recuperer le mois courant ( "12/01/1995 17:35:41" )
	** On utilise le jour de la semaine pour declencher le mardi a 2h00.
	*/
	XZSM_11ConversionHorodate ( vl_HorodateSec, &vl_DATE_CHAR );
	
	strcpy(&(vl_DATE_CHAR[11] ),CSBDC_HEURE_IHM_SAUVE);
	
	/*A
	** Conversion en secondes de la date de declenchement du timer reconstituee.
	*/
	XZSM_13ConversionHorodateSec ( vl_DATE_CHAR, &vl_HorodateSec);
	
	/*
	** Rajoue du nbre de secondes jusqu au prochain mardi
	*/
	
/*	vl_HorodateSec = vl_HorodateSec +((24 * 3600) * abs(CSBDC_JOUR_SAUVE_HEBDO-vl_JourSemaine));*/
	vl_HorodateSec = vl_HorodateSec +((24 * 3600) * ((CSBDC_JOUR_SAUVE_HEBDO+6-vl_JourSemaine)%7 +1));
   
    	XZSM_11ConversionHorodate( vl_HorodateSec, &vl_horodate);
        XZST_03EcritureTrace(   XZSTC_DEBUG1 ,"Prog SVG pour : %s",vl_horodate);

	/*A
        ** Recuperation du site de la machine
        */
        if ( (vl_retcode = XZSC_04NomSite(pl_NomSite)) != XDC_OK)
        {
                XZST_03EcritureTrace( XZSTC_WARNING, "csbd_ProgramTimer_Hebdo : Recuperation du Nom du site impossible .");
                XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : csbd_ProgramTimer_Hebdo : Sortie");
                csva_sortir();
                return(XDC_NOK);
        }

	/*A
        ** Constitution du nom Data Group a partir du nom du site
        */
        
        XDG_EncodeDG2(vl_datagroup,XDG_CBDARCHIVE,pl_NomSite);
	if ((vl_retcode = XZSM_01DemanderProgrammation(	vl_HorodateSec,
							CSBDC_DELAI_HEBDO,
							vl_datagroup,
							XDM_CSV_SBD,
							CSBDC_CLE_TIMER_HEBDO_SBD,
							XZSMC_SEC_VRAI ))!= XDC_OK)
	{
     		XZST_03EcritureTrace(XZSTC_WARNING," Probleme de programmation TIMER hebdomadaire effectuant la sauvegarde automatique des bases CFG et HISTO \n\n ");
   		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : csbd_ProgramTimer_Hebdo : Sortie avec %d",vl_retcode);
		csva_sortir();
   	}
   	else XZST_03EcritureTrace(XZSTC_DEBUG1,"Programmation TIMER effectuant la sauvegarde automatique hebdomadaire des bases CFG et HISTO : OK\n\n");
   	
   	
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : csbd_ProgramTimer_Hebdo : Fin d execution");
	return(XDC_OK);			
}
