/*E*/
/*Fichier : $Id: csva_inf.c,v 1.2 1996/03/01 15:25:40 mercier Exp $      Release : $Revision: 1.2 $        Date : $Date: 1996/03/01 15:25:40 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  GECONF
------------------------------------------------------
* MODULE INF * FICHIER csva_inf.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*	cinf_ProgramTimer_Mois fonction qui permet de reprogrammer
* le timer pour le message declenchant l ouverture d une fenetre 
* d information pour l administrateur, lui rappelant qu il doit effectuer
* chaque mois l archivage de la base hist.
*	
*
------------------------------------------------------
* HISTORIQUE :
*
* mercier	06 Jan 1995	: Creation
* Niepceron	01 Mar 1996	: Correction des accents v1.2
------------------------------------------------------*/

/* fichiers inclus */

#include "csva_inf.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: csva_inf.c,v 1.2 1996/03/01 15:25:40 mercier Exp $ : csva_inf" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	cinf_sauvinf_cb est declenchee sur reception du message RTWks XDM_CSV_INF.
*	Ce CallBack va prevenir l administrateur qu il doit effectuer l archivage .
*	
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void cinf_sauvinf_cb (	T_IPC_CONN 			va_Cnx,
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
*	Fonction declenchee sur reception de message XDM_CSV_INF.	
* 	
------------------------------------------------------*/
{
	static char *version = "$Id: csva_inf.c,v 1.2 1996/03/01 15:25:40 mercier Exp $ : cinf_sauvinf_cb" ;
	
	/* Variables locales pour messages RTWks */
	
	int			vl_retcode    	= XDC_OK;	/* code de retour 	*/
	XDY_NomMachine  	vl_NomMachine 	= "";
	char			vl_Msg[500] 	= "";	
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN : cinf_sauvinf_cb : Debut d execution");
	
	sprintf(vl_Msg,"                 *");
	XZIA08_AffMsgAdm(CSVAV_NOMMACHINE,vl_Msg);
	
	sprintf(vl_Msg,"                * *");
	XZIA08_AffMsgAdm(CSVAV_NOMMACHINE,vl_Msg);
	
	sprintf(vl_Msg,"              ** ! **");
	XZIA08_AffMsgAdm(CSVAV_NOMMACHINE,vl_Msg);
	
	sprintf(vl_Msg,"             **  !  **");
	XZIA08_AffMsgAdm(CSVAV_NOMMACHINE,vl_Msg);
	
	sprintf(vl_Msg,"           **    !   **");
	XZIA08_AffMsgAdm(CSVAV_NOMMACHINE,vl_Msg);
	
	sprintf(vl_Msg,"          ***************");
	XZIA08_AffMsgAdm(CSVAV_NOMMACHINE,vl_Msg);
	
	sprintf(vl_Msg,"Message pour l'administrateur : Il faut penser à archiver le mois précédent.");
	XZIA08_AffMsgAdm(CSVAV_NOMMACHINE,vl_Msg);
	
	sprintf(vl_Msg,"_____________________________________________________________________________");
	XZIA08_AffMsgAdm(CSVAV_NOMMACHINE,vl_Msg);
	
	if((vl_retcode = cinf_ProgramTimer_Mois ()) != XDC_OK)
	{
		sprintf(vl_Msg,"La reprograamtion du message mensuel de sauvegarde a echouée");
		XZIA08_AffMsgAdm(CSVAV_NOMMACHINE,vl_Msg);
		XZST_03EcritureTrace(XZSTC_WARNING,"cinf_sauvinf_cb : %s", vl_Msg);
	}
	else
	{
		sprintf(vl_Msg,"La reprograamtion du message mensuel de sauvegarde : OK");
		XZIA08_AffMsgAdm(CSVAV_NOMMACHINE,vl_Msg);
		XZST_03EcritureTrace(XZSTC_INFO,"cinf_sauvinf_cb : %s", vl_Msg);
	
	}
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cinf_sauvinf_cb : Fin d execution");
}

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	cinf_ProgramTimer_Mois fonction qui permet de reprogrammer
* le timer pour le message declenchant l ouverture d une fenetre 
* d information pour l administrateur, lui rappelant qu il doit effectuer
* chaque mois l archivage de la base hist.
*	
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cinf_ProgramTimer_Mois () 
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
	static char *version = "$Id: csva_inf.c,v 1.2 1996/03/01 15:25:40 mercier Exp $ : cinf_ProgramTimer_Mois" ;
	
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
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN : cinf_ProgramTimer_Mois : Debut d execution");
	/*A 
	** Recuperation de l horodate courante
	*/
	if ((vl_retcode = XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME,&vl_JourSemaine,&vl_HorodateSec))!=XZSMC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"cinf_ProgramTimer_Mois : XZSM_07LireHorodate a echoue");
		csva_sortir();
	}
	/*A
	** Traduction en chaine pour recuperer le mois courant ( "12/01/1995 17:35:41" )
	*/
	XZSM_11ConversionHorodate ( vl_HorodateSec, &vl_DATE_CHAR );
	
	strncpy(vl_MoisChar,&(vl_DATE_CHAR[3]),2);
	if( vl_MoisChar[0] == '0' )
	{
		vl_MoisChar[0] = vl_MoisChar[1];
		vl_MoisChar[1] = '\0';
	}
	
	vl_MoisChar[2]='\0';
	
	vl_Mois = atoi(vl_MoisChar);
	
	if ( vl_Mois == 12) /* Incementation de l annee */
	{
		strcpy(vl_MoisChar,"01");
		strncpy(vl_AnneeChar,&(vl_DATE_CHAR[6]),4);
		vl_AnneeChar[4] = '\0';
		
		vl_Annee = atoi(vl_AnneeChar);
		/*sscanf(vl_AnneeChar,"%d",&vl_Annee);*/
		vl_Annee++;
		sprintf(vl_AnneeChar,"%d",vl_Annee);
		vl_DATE_CHAR[3] = vl_MoisChar[0];
		vl_DATE_CHAR[4] = vl_MoisChar[1];
		vl_DATE_CHAR[6] = vl_AnneeChar[0];
		vl_DATE_CHAR[7] = vl_AnneeChar[1];
		vl_DATE_CHAR[8] = vl_AnneeChar[2];
		vl_DATE_CHAR[9] = vl_AnneeChar[3];
	} 
	else
	{
		vl_Mois++;
		sprintf(vl_MoisChar,"%d",vl_Mois);
		if (vl_Mois <10)
		{
			vl_MoisChar[1] = vl_MoisChar[0];
			vl_MoisChar[0] = '0';
		}
		vl_DATE_CHAR[3] = vl_MoisChar[0];
		vl_DATE_CHAR[4] = vl_MoisChar[1];
		
	}
	strncpy(vl_DATE_CHAR,CINFC_JOUR_IHM_SAUVE,2);
	strcpy(&(vl_DATE_CHAR[11] ),CINFC_HEURE_IHM_SAUVE);
	
	/*A
	** Conversion en secondes de la date de declenchement du timer reconstituee.
	*/
	XZSM_13ConversionHorodateSec ( vl_DATE_CHAR, &vl_HorodateSec);
	
	/*A
        ** Recuperation du site de la machine
        */
        if ( (vl_retcode = XZSC_04NomSite(pl_NomSite)) != XDC_OK)
        {
                XZST_03EcritureTrace( XZSTC_WARNING, "cinf_ProgramTimer_Mois : Recuperation du Nom du site impossible .");
                XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cinf_ProgramTimer_Mois : Sortie");
                csva_sortir();
                return(XDC_NOK);
        }

	/*A
        ** Constitution du nom Data Group a partir du nom du site
        */
        
        XDG_EncodeDG2(vl_datagroup,XDG_CBDARCHIVE,pl_NomSite);
	if ((vl_retcode = XZSM_01DemanderProgrammation(	vl_HorodateSec,
							0,
							vl_datagroup,
							XDM_CSV_INF,
							CINFC_CLE_TIMER_MENSUEL_INF,
							XZSMC_SEC_VRAI ))!= XDC_OK)
	{
     		XZST_03EcritureTrace(XZSTC_WARNING," Probleme de programmation TIMER mensuel pour prevenir l administrateur qu il peut effectuer l archivage du dernier mois historise \n\n ");
   		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cinf_ProgramTimer_Mois : Sortie avec %d",vl_retcode);
		csva_sortir();
   	}
   	else XZST_03EcritureTrace(XZSTC_DEBUG1,"Programmation TIMER mensuel pour prevenir l administrateur qu il peut effectuer l archivage du dernier mois historise : OK\n\n ");
   	
   	
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cinf_ProgramTimer_Mois : Fin d execution");
	return(XDC_OK);			
}
