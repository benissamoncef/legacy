/*E*/
/*Fichier : $Id: cgcd_mvf_2.c,v 1.17 2020/05/19 14:52:50 gesconf Exp $      Release : $Revision: 1.17 $        Date : $Date: 2020/05/19 14:52:50 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  GECONF
------------------------------------------------------
* MODULE MMVF_SGBD_RESEAU * FICHIER cgcd_mvf_2.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
***********************
*	cmvf_ModifSGBD_Reseau : Pour chaque classe de reseau 
* (autoroute, district, portion, noeud, echangeur, aire de repos, peage, pente et descente,
* Ouverture du fichier de la classe reseau.
* Lecture   du fichier de la classe reseau.
* La fonction utilisateur passee en argument appelle la primitive respective afin de 
* modifier la base donnee concernant la classe du reseau.
***********************	
* 	cmvf_Modif_SGBD_Ihm : Pour chaque classe d ihm 
* Ouverture du fichier du type.
* Lecture   du fichier du type.
* La fonction utilisateur passee en argument appelle la primitive respective afin de 
* modifier la base donnee concernant la classe de ihm.
**********************
* 	cmvf_Trace_TYPE_Result : se trouve dans cgcd_mvf_util.c.
**********************
------------------------------------------------------
* HISTORIQUE :
*
* mercier	21 Oct 1994	: Creation
*************************
* Mercier O.	27 Oct 1994	: Modification	V 1.2
*				Correction appel Fct Utili des Villes
*************************
* MERCIER O.	14 Jan 1995	: Ajout erreur pour adminis	V 1.5
* NIEPCERON     28 Fev 1995     : Remplacement de repertoire de liv par celui de travail v1.6 
* NIEPCERON	07 Jun 1996	: remplacement du \n par en cours pour le msg Traitement du fichier ... v1.7
* MISMER	10 Fev 1997	: Ajout des fonction temps de parcours v1.7 (DEM/1395)
* VATHELOT	04 Avr 1997	: Ajout du PR fin pour chaque segment v1.8 (DEM/1420)
* VR		28 Nov 2011 : Ajout recherche_dest_pmv (DEM/1014)
* JPL		20/06/2012	: Suppression des directives ignorant deux sections  1.11
* VR		05 Juin 2012 : Ajout TDP pour PMVA (DEM/1014 PMA)
* JPL		13/07/2016	: Inclusion des declarations des fonctions XZAOC  1.13
* LCL		15/11/2016	: Ajout decodage fichier viaducs (LOT23) 1.14
* LCL		17//12016	: Correctif viaduc (LOT23) 1.15
* JPL		24/01/19	: Gestion des fichiers d'en-tete inclu(de)s  1.16
* PNI		19/05/20	:  Correctif : fermeture fichier en trop 1.17
* ABE 		02/08/22 	: Ajout NIChe et ISue de secour localisation tunnel DEM-SAE403
r-----------------------------------------------------*/

/* fichiers inclus */

#include "cgcd_mvf_2.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: cgcd_mvf_2.c,v 1.17 2020/05/19 14:52:50 gesconf Exp $ : cgcd_mvf_2" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */
/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	cmvf_ModifSGBD_Reseau : Pour chaque classe de reseau 
* (autoroute, district, portion, noeud, echangeur, aire de repos, peage, pente et descente,
* Ouverture du fichier de la classe reseau.
* Lecture   du fichier de la classe reseau.
* La fonction utilisateur passee en argument appelle la primitive respective afin de 
* modifier la base donnee concernant la classe du reseau.
*  
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cmvf_ModifSGBD_Reseau(	) 
/*
* ARGUMENTS EN ENTREE :  aucun 	
* ARGUMENTS EN SORTIE :  aucun
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
*
* CONDITION D'UTILISATION
*	fichier de trace de bilan config doit etre ouvert
*	les fichiers de configuration doivent exister et dument remplis
*	dans les directories definies dans le xdc.h conformement au ppl.
*
* FONCTION 
	
* 	
------------------------------------------------------*/
{
	static char *	version = "$Id: cgcd_mvf_2.c,v 1.17 2020/05/19 14:52:50 gesconf Exp $ : cmvf_ModifSGBD_Reseau";
	int     	vl_retcode;
	char  		pl_NomFile[255];          /* 
					** recoit le nom complet des fichiers a ouvrir
					*/
	
	
	char  		pl_Msg[255];		/* contient le mesg a tracer*/
	XDY_Entier	pl_NumeroLigne;		/* va contenir le nbre ligne lues par les fonctions du type XZAO50 */
	
	FILE *		pl_FD;	/* File descripteur du fichier courant de travail */
	char		vl_NomBase[4];
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN : cmvf_ModifSGBD_Reseau : debut d execution");
	
	sprintf(pl_Msg,"************************************");
	cuti_BilanConf_Trace(pl_Msg); 
	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
	
	sprintf(pl_Msg,"--------Modification de la configuration du reseau sur la base %s --------",CGCDG_Base_Courante);
	cuti_BilanConf_Trace(pl_Msg); 
	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);

	/*A
	** Ouverture du fichier de configuration des District
	*/
	sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_DORA_TRAV,XDF_CONF_DISTRICT);	
	if (!(pl_FD=fopen(pl_NomFile,"r")))
	{
		sprintf(pl_Msg,"Erreur d'ouverture du fichier %s\n",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_ModifSGBD_Reseau : \n%s ",pl_Msg);		
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); 	/* On previent l IHM de l administrateur */
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); 	/* Notification dans Bilan config*/		
		fclose(pl_FD);
		return(XDC_NOK);		
	}
	else
	{
		sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_ModifSGBD_Reseau : %s",pl_Msg);
		cuti_BilanConf_Trace("********************************************************");
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
	}
	/*A
	** Verification district
	*/
	if ((vl_retcode = xzao52_Lire_Config_District(pl_FD,cmvf_FctUti_MSR_XZAO52_53,&pl_NumeroLigne))!=XDC_OK)
	{
		if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
		else sprintf(pl_Msg,"Erreur detectee dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
	 	XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb xzao52_Lire_Config_District() a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cvce_VerifModif_cb a echoue : Sortie");
		fclose(pl_FD);
		return(XDC_NOK);
	}
	else
	{
		sprintf(pl_Msg,"Traitement de %s OK.",pl_NomFile);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);	   /* Notification dans Bilan config */
		fclose(pl_FD);
	}
	/*A
	** Ouverture du fichier de configuration des autoroutes
	*/
	sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_DORA_TRAV,XDF_CONF_AUTOROUTE);	
	if (!(pl_FD=fopen(pl_NomFile,"r")))
	{
		/*B 
		** En cas de prob d ouverture du fichier de conf, on logue le prob a l administrateur
		** et on le notifie dans le fichier Bilan Config
		*/
		sprintf(pl_Msg,"Erreur d'ouverture du fichier %s\n",pl_NomFile);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_ModifSGBD_Reseau : \n%s ",pl_Msg);		
		fclose(pl_FD);		
		return(XDC_NOK);		
	}
	else
	{
		sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_ModifSGBD_Reseau : %s",pl_Msg);
		cuti_BilanConf_Trace("********************************************************");
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
	}
	/*A
	** Verification autoroute
	*/
	if ((vl_retcode = xzao50_Lire_Config_Autoroute(pl_FD,cmvf_FctUti_MSR_XZAO50_51,&pl_NumeroLigne))!=XDC_OK)
	{
		if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
		else sprintf(pl_Msg,"Erreur detectee dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
		XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb : xzao50_Lire_Config_Autoroute() a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cvce_VerifModif_cb a echoue : Sortie");
		fclose(pl_FD);
		return(XDC_NOK);
	}
	else
	{
		sprintf(pl_Msg,"Traitement de %s OK.",pl_NomFile);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);	   /* Notification dans Bilan config */
		fclose(pl_FD);
	}
	/*A
	** Ouverture du fichier de configuration des portions
	*/
	sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_DORA_TRAV,XDF_CONF_PORTION);	
	if (!(pl_FD=fopen(pl_NomFile,"r")))
	{
		sprintf(pl_Msg,"Erreur d'ouverture du fichier %s\n",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_ModifSGBD_Reseau : \n%s ",pl_Msg);		
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); 	/* On previent l IHM de l administrateur */
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); 	/* Notification dans Bilan config*/		
		fclose(pl_FD);
		return(XDC_NOK);		
	}
	else
	{
		sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_ModifSGBD_Reseau : %s",pl_Msg);
		cuti_BilanConf_Trace("********************************************************");
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
	}
	/*A
	** Verification portion
	*/
	if ((vl_retcode = xzao54_Lire_Config_Portion(pl_FD,cmvf_FctUti_MSR_XZAO54_55,&pl_NumeroLigne))!=XDC_OK)
	{
		if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
		else sprintf(pl_Msg,"Erreur detectee dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
	 	XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb () xzao54_Lire_Config_Portion a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cvce_VerifModif_cb a echoue : Sortie");
		fclose(pl_FD);
		return(XDC_NOK);
	}
	else
	{
		sprintf(pl_Msg,"Traitement de %s OK.",pl_NomFile);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);	   /* Notification dans Bilan config */
		fclose(pl_FD);
	}
	/*A
	** Ouverture du fichier de configuration des noeuds
	*/
	sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_DORA_TRAV,XDF_CONF_NOEUD);	
	if (!(pl_FD=fopen(pl_NomFile,"r")))
	{
		sprintf(pl_Msg,"Erreur d'ouverture du fichier %s\n",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_ModifSGBD_Reseau : \n%s ",pl_Msg);		
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); 	/* On previent l IHM de l administrateur */
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); 	/* Notification dans Bilan config*/		
		fclose(pl_FD);
		return(XDC_NOK);		
	}
	else
	{
		sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_ModifSGBD_Reseau : %s",pl_Msg);
		cuti_BilanConf_Trace("********************************************************");
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
	}
	/*A
	** Verification noeud
	*/
	if ((vl_retcode = xzao56_Lire_Config_Noeud(pl_FD,cmvf_FctUti_MSR_XZAO56_57,&pl_NumeroLigne))!=XDC_OK)
	{
		if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
		else sprintf(pl_Msg,"Erreur detectee dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
	 	XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb :  xzao56_Lire_Config_Noeud a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cvce_VerifModif_cb a echoue : Sortie");
		fclose(pl_FD);
		return(XDC_NOK);
	}
	else
	{
		sprintf(pl_Msg,"Traitement de %s OK.",pl_NomFile);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);	   /* Notification dans Bilan config */
		fclose(pl_FD);
	}
	/*A
	** Ouverture du fichier de configuration des segments
	*/
	sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_DORA_TRAV,XDF_CONF_SEGMENT);	
	if (!(pl_FD=fopen(pl_NomFile,"r")))
	{
		sprintf(pl_Msg,"Erreur d'ouverture du fichier %s\n",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_ModifSGBD_Reseau : \n%s ",pl_Msg);		
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); 	/* On previent l IHM de l administrateur */
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); 	/* Notification dans Bilan config*/		
		fclose(pl_FD);
		return(XDC_NOK);		
	}
	else
	{
		sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_ModifSGBD_Reseau : %s",pl_Msg);
		cuti_BilanConf_Trace("********************************************************");
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
	}
	/*A
	** Verification segments
	*/
	if ((vl_retcode = xzao58_Lire_Config_Segment(pl_FD,cmvf_FctUti_MSR_XZAO58_59,&pl_NumeroLigne))!=XDC_OK)
	{
		if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
		else sprintf(pl_Msg,"Erreur detectee dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
		XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb :  xzao58_Lire_Config_Segment a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cvce_VerifModif_cb a echoue : Sortie");
		fclose(pl_FD);
		return(XDC_NOK);
	}
	else
	{
		sprintf(pl_Msg,"Traitement de %s OK.",pl_NomFile);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);	   /* Notification dans Bilan config */
		fclose(pl_FD);
	}
		/*A
	** Ouverture du fichier de configuration des niche et issue de secour
	*/
	sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_DORA_TRAV,XDF_CONF_NIC_IS);	
	if (!(pl_FD=fopen(pl_NomFile,"r")))
	{
		/*B 
		** En cas de prob d ouverture du fichier de conf, on logue le prob a l administrateur
		** et on le notifie dans le fichier Bilan Config
		*/
		sprintf(pl_Msg,"Erreur d'ouverture du fichier %s\n",pl_NomFile);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_ModifSGBD_Reseau : \n%s ",pl_Msg);		
		fclose(pl_FD);		
		return(XDC_NOK);		
	}
	else
	{
		sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_ModifSGBD_Reseau : %s",pl_Msg);
		cuti_BilanConf_Trace("********************************************************");
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
	}
	/*A
	** Verification niche et issue de secour
	*/
	if ((vl_retcode = xzao506_Lire_Config_NIC_IS(pl_FD,cmvf_FctUti_MSR_XZAO506_507,&pl_NumeroLigne))!=XDC_OK)
	{
		if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
		else sprintf(pl_Msg,"Erreur detectee dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
		XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb : xzao506_Lire_Config_NIC_IS() a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cvce_VerifModif_cb a echoue : Sortie");
		fclose(pl_FD);
		return(XDC_NOK);
	}
	else
	{
		sprintf(pl_Msg,"Traitement de %s OK.",pl_NomFile);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);	   /* Notification dans Bilan config */
		fclose(pl_FD);
	}

/*A
	** Ouverture du fichier de configuration des pentes
	*/
	sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_DORA_TRAV,XDF_CONF_PENTE);	
	if (!(pl_FD=fopen(pl_NomFile,"r")))
	{
		sprintf(pl_Msg,"Erreur d'ouverture du fichier %s\n",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_ModifSGBD_Reseau : \n%s ",pl_Msg);		
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); 	/* On previent l IHM de l administrateur */
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); 	/* Notification dans Bilan config*/		
		fclose(pl_FD);
		return(XDC_NOK);		
	}
	else
	{
		sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_ModifSGBD_Reseau : %s",pl_Msg);
		cuti_BilanConf_Trace("********************************************************");
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
	}
	/*A
	** Verification pentes
	*/
	if ((vl_retcode = xzao60_Lire_Config_Pente(pl_FD,cmvf_FctUti_MSR_XZAO60_61,&pl_NumeroLigne))!=XDC_OK)
	{
		if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
		else sprintf(pl_Msg,"Erreur detectee dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
	 	XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb : xzao60_Lire_Config_Pente  a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cvce_VerifModif_cb a echoue : Sortie");
		fclose(pl_FD);
		return(XDC_NOK);
	}
	else
	{
		sprintf(pl_Msg,"Traitement de %s OK.",pl_NomFile);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);	   /* Notification dans Bilan config */
		fclose(pl_FD);
	}
	/*A
	** Ouverture du fichier de configuration des echangeurs 
	*/
	sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_DORA_TRAV,XDF_CONF_ECHANGEUR);	
	if (!(pl_FD=fopen(pl_NomFile,"r")))
	{
		sprintf(pl_Msg,"Erreur d'ouverture du fichier %s\n",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_ModifSGBD_Reseau : \n%s ",pl_Msg);		
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); 	/* On previent l IHM de l administrateur */
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); 	/* Notification dans Bilan config*/		
		fclose(pl_FD);
		return(XDC_NOK);		
	}
	else
	{
		sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_ModifSGBD_Reseau : %s",pl_Msg);
		cuti_BilanConf_Trace("********************************************************");
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
	}
	/*A
	** Verification echangeurs
	*/
	if ((vl_retcode = xzao83_Lire_Config_Echangeur(pl_FD,cmvf_FctUti_MSR_XZAO83_84,&pl_NumeroLigne))!=XDC_OK)
	{
		if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
		else sprintf(pl_Msg,"Erreur detectee dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
	 	XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb : xzao83_Lire_Config_Echangeur a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cvce_VerifModif_cb a echoue : Sortie");
		fclose(pl_FD);
		return(XDC_NOK);
	}
	else
	{
		sprintf(pl_Msg,"Traitement de %s OK.",pl_NomFile);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);	   /* Notification dans Bilan config */
		fclose(pl_FD);
	}
	/*A
	** Ouverture du fichier de configuration des aires 
	*/
	sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_DORA_TRAV,XDF_CONF_AIRE);	
	if (!(pl_FD=fopen(pl_NomFile,"r")))
	{
		sprintf(pl_Msg,"Erreur d'ouverture du fichier %s\n",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_ModifSGBD_Reseau : \n%s ",pl_Msg);		
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); 	/* On previent l IHM de l administrateur */
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); 	/* Notification dans Bilan config*/		
		fclose(pl_FD);
		return(XDC_NOK);		
	}
	else
	{
		sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_ModifSGBD_Reseau : %s",pl_Msg);
		cuti_BilanConf_Trace("********************************************************");
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
	}
	/*A
	** Verification aires
	*/
	if ((vl_retcode = xzao85_Lire_Config_Aire(pl_FD,cmvf_FctUti_MSR_XZAO85_86,&pl_NumeroLigne))!=XDC_OK)
	{
		if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
		else sprintf(pl_Msg,"Erreur detectee dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
	 	XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb : xzao85_Lire_Config_Aire a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cvce_VerifModif_cb a echoue : Sortie");
		fclose(pl_FD);
		return(XDC_NOK);
	}
	else
	{
		sprintf(pl_Msg,"Traitement de %s OK.",pl_NomFile);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);	   /* Notification dans Bilan config */
		fclose(pl_FD);
	}

	/* Ouverture du fichier de configuration des batiments */
	sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_DORA_TRAV,XDF_CONF_BATIMENT);	
	if (!(pl_FD=fopen(pl_NomFile,"r")))
	{
		sprintf (pl_Msg,"Erreur d'ouverture du fichier %s\n", pl_NomFile);
		XZST_03EcritureTrace (XZSTC_WARNING, "TCGCD : cmvf_ModifSGBD_Reseau : \n%s ", pl_Msg);		
		XZIA08_AffMsgAdm (CGCDV_NOMMACHINE, pl_Msg); 	/* On previent l IHM de l administrateur */
		cuti_BilanConf_Trace (pl_Msg); 
	 	XZIA08_AffMsgAdm (CGCDV_NOMMACHINE, pl_Msg); 	/* Notification dans Bilan config*/		
		fclose (pl_FD);
		return (XDC_NOK);		
	}
	else
	{
		sprintf (pl_Msg, "Traitement du fichier %s en cours", pl_NomFile);
		XZST_03EcritureTrace (XZSTC_DEBUG1,"cmvf_ModifSGBD_Reseau : %s", pl_Msg);
		cuti_BilanConf_Trace ("********************************************************");
		cuti_BilanConf_Trace (pl_Msg); 
	 	XZIA08_AffMsgAdm (CGCDV_NOMMACHINE, pl_Msg); /* Notification dans Bilan config*/
	}
	/* Verification batiments */
	if ((vl_retcode = xzao511_Lire_Config_Bat (pl_FD, cmvf_FctUti_MSR_XZAO511_510, &pl_NumeroLigne)) != XDC_OK)
	{
		if (pl_NumeroLigne == 0)
			sprintf(pl_Msg, "Impossible de traiter le fichier %s", pl_NomFile);
		else
			sprintf (pl_Msg,"Erreur detectee dans %s a la ligne %d", pl_NomFile, pl_NumeroLigne);
		cuti_BilanConf_Trace (pl_Msg); 
	 	XZIA08_AffMsgAdm (CGCDV_NOMMACHINE,pl_Msg);
	 	XZST_03EcritureTrace (XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb : xzao511_Lire_Config_Bat a echoue");
		XZST_03EcritureTrace (XZSTC_FONCTION, "OUT : cvce_VerifModif_cb a echoue : Sortie");
		fclose (pl_FD);
		return (XDC_NOK);
	}
	else
	{
		sprintf (pl_Msg,"Traitement de %s OK.", pl_NomFile);
		cuti_BilanConf_Trace (pl_Msg); 
	 	XZIA08_AffMsgAdm (CGCDV_NOMMACHINE, pl_Msg);	   /* Notification dans Bilan config */
		fclose (pl_FD);
	}

	/*A
	** Ouverture du fichier de configuration des viaducs 
	*/
	sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_DORA_TRAV,XDF_CONF_VIADUC);	
	if (!(pl_FD=fopen(pl_NomFile,"r")))
	{
		sprintf(pl_Msg,"Erreur d'ouverture du fichier %s\n",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_ModifSGBD_Reseau : \n%s ",pl_Msg);		
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); 	/* On previent l IHM de l administrateur */
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); 	/* Notification dans Bilan config*/		
		fclose(pl_FD);
		return(XDC_NOK);		
	}
	else
	{
		sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_ModifSGBD_Reseau : %s",pl_Msg);
		cuti_BilanConf_Trace("********************************************************");
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
	}
	/*A
	** Verification viaducs
	*/
	if ((vl_retcode = xzao649_Lire_Config_Viaduc(pl_FD,cmvf_FctUti_MSR_XZAO649_650,&pl_NumeroLigne))!=XDC_OK)
	{
		if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
		else sprintf(pl_Msg,"Erreur detectee dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
	 	XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb : xzao64_Lire_Config_Viaduc a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cvce_VerifModif_cb a echoue : Sortie");
		fclose(pl_FD);
		return(XDC_NOK);
	}
	else
	{
		sprintf(pl_Msg,"Traitement de %s OK.",pl_NomFile);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);	   /* Notification dans Bilan config */
		fclose(pl_FD);
	}
	
	/*A
	** Ouverture du fichier de configuration des peages 
	*/
	sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_DORA_TRAV,XDF_CONF_PEAGE);	
	if (!(pl_FD=fopen(pl_NomFile,"r")))
	{
		sprintf(pl_Msg,"Erreur d'ouverture du fichier %s\n",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_ModifSGBD_Reseau : \n%s ",pl_Msg);		
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); 	/* On previent l IHM de l administrateur */
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); 	/* Notification dans Bilan config*/		
		fclose(pl_FD);
		return(XDC_NOK);		
	}
	else
	{
		sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_ModifSGBD_Reseau : %s",pl_Msg);
		cuti_BilanConf_Trace("********************************************************");
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
	}
	/*A
	** Verification peages
	*/
	if ((vl_retcode = xzao87_Lire_Config_Peage(pl_FD,cmvf_FctUti_MSR_XZAO87_88,&pl_NumeroLigne))!=XDC_OK)
	{
		if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
		else sprintf(pl_Msg,"Erreur detectee dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
	 	XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb : xzao87_Lire_Config_Peage a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cvce_VerifModif_cb a echoue : Sortie");
		fclose(pl_FD);
		return(XDC_NOK);
	}
	else
	{
		sprintf(pl_Msg,"Traitement de %s OK.",pl_NomFile);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);	   /* Notification dans Bilan config */
		fclose(pl_FD);
	}
	/*A
	** Ouverture du fichier de configuration des passages superieurs 
	*/
	sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_DORA_TRAV,XDF_CONF_PS);	
	if (!(pl_FD=fopen(pl_NomFile,"r")))
	{
		sprintf(pl_Msg,"Erreur d'ouverture du fichier %s\n",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_ModifSGBD_Reseau : \n%s ",pl_Msg);		
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); 	/* On previent l IHM de l administrateur */
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); 	/* Notification dans Bilan config*/		
		fclose(pl_FD);
		return(XDC_NOK);		
	}
	else
	{
		sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_ModifSGBD_Reseau : %s",pl_Msg);
		cuti_BilanConf_Trace("********************************************************");
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
	}
	/*A
	** Verification passages superieurs 
	*/
	if ((vl_retcode = xzao89_Lire_Config_PS(pl_FD,cmvf_FctUti_MSR_XZAO89_90,&pl_NumeroLigne))!=XDC_OK)
	{
		if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
		else sprintf(pl_Msg,"Erreur detectee dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
	 	XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb : xzao89_Lire_Config_PS a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cvce_VerifModif_cb a echoue : Sortie");
		fclose(pl_FD);
		return(XDC_NOK);
	}
	else
	{
		sprintf(pl_Msg,"Traitement de %s OK.",pl_NomFile);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);	   /* Notification dans Bilan config */
		fclose(pl_FD);
	}
	/*A
	** Ouverture du fichier de configuration du reseau 
	*/
	sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_DORA_TRAV,XDF_CONF_RESEAU);	
	if (!(pl_FD=fopen(pl_NomFile,"r")))
	{
		sprintf(pl_Msg,"Erreur d'ouverture du fichier %s\n",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_ModifSGBD_Reseau : \n%s ",pl_Msg);		
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); 	/* On previent l IHM de l administrateur */
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); 	/* Notification dans Bilan config*/		
		fclose(pl_FD);
		return(XDC_NOK);		
	}
	else
	{
		sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_ModifSGBD_Reseau : %s",pl_Msg);
		cuti_BilanConf_Trace("********************************************************");
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
	}
	/*A
	** Verification du reseau
	*/
	if ((vl_retcode = xzao91_Lire_Config_Reseau(pl_FD,cmvf_FctUti_MSR_XZAO91_92,&pl_NumeroLigne))!=XDC_OK)
	{
		if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
		else sprintf(pl_Msg,"Erreur detectee dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
	 	XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb : xzao91_Lire_Config_Reseau a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cvce_VerifModif_cb a echoue : Sortie");
		fclose(pl_FD);
		return(XDC_NOK);
	}
	else
	{
		sprintf(pl_Msg,"Traitement de %s OK.",pl_NomFile);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);	   /* Notification dans Bilan config */
		fclose(pl_FD);
	}
	/*A
	** Ouverture du fichier de configuration des villes 
	*/
	sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_DORA_TRAV,XDF_CONF_VILLE);	
	if (!(pl_FD=fopen(pl_NomFile,"r")))
	{
		sprintf(pl_Msg,"Erreur d'ouverture du fichier %s\n",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_ModifSGBD_Reseau : \n%s ",pl_Msg);		
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); 	/* On previent l IHM de l administrateur */
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); 	/* Notification dans Bilan config*/		
		fclose(pl_FD);
		return(XDC_NOK);		
	}
	else
	{
		sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_ModifSGBD_Reseau : %s",pl_Msg);
		cuti_BilanConf_Trace("********************************************************");
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
	}
	/*A
	** Verification des villes
	*/
	if ((vl_retcode = xzao93_Lire_Config_Ville (pl_FD,cmvf_FctUti_MSR_XZAO93_94,&pl_NumeroLigne))!=XDC_OK)
	{
		if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
		else sprintf(pl_Msg,"Erreur detectee dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
	 	XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb : xzao93_Lire_Config_Ville  a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cvce_VerifModif_cb a echoue : Sortie");
		fclose(pl_FD);
		return(XDC_NOK);
	}
	else
	{
		sprintf(pl_Msg,"Traitement de %s OK.",pl_NomFile);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);	   /* Notification dans Bilan config */
		fclose(pl_FD);
	}
	/*A
	** Ouverture du fichier de configuration des panneaux de Police
	*/
	sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_DORA_TRAV,XDF_CONF_PAN_POLICE);	
	if (!(pl_FD=fopen(pl_NomFile,"r")))
	{
		sprintf(pl_Msg,"Erreur d'ouverture du fichier %s\n",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_ModifSGBD_Reseau : \n%s ",pl_Msg);		
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); 	/* On previent l IHM de l administrateur */
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); 	/* Notification dans Bilan config*/		
		fclose(pl_FD);
		return(XDC_NOK);		
	}
	else
	{
		sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_ModifSGBD_Reseau : %s",pl_Msg);
		cuti_BilanConf_Trace("********************************************************");
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
	}
	/*A
	** Verification des panneaux de Police 
	*/
	if ((vl_retcode =  xzao120_Lire_Config_Panneau_Police(pl_FD,cmvf_FctUti_MSR_XZAO120_121,&pl_NumeroLigne))!=XDC_OK)
	{
		if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
		else sprintf(pl_Msg,"Erreur detectee dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
	 	XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb :  xzao120_Lire_Config_Panneau_Policea echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cvce_VerifModif_cb a echoue : Sortie");
		fclose(pl_FD);
		return(XDC_NOK);
	}
	else
	{
		sprintf(pl_Msg,"Traitement de %s OK.",pl_NomFile);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);	   /* Notification dans Bilan config */
		fclose(pl_FD);
	}
	/*A
	** Ouverture du fichier de configuration des  interruptions de terre-plein central
	*/
	sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_DORA_TRAV,XDF_CONF_ITPC);	
	if (!(pl_FD=fopen(pl_NomFile,"r")))
	{
		sprintf(pl_Msg,"Erreur d'ouverture du fichier %s\n",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_ModifSGBD_Reseau : \n%s ",pl_Msg);		
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); 	/* On previent l IHM de l administrateur */
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); 	/* Notification dans Bilan config*/		
		fclose(pl_FD);
		return(XDC_NOK);		
	}
	else
	{
		sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_ModifSGBD_Reseau : %s",pl_Msg);
		cuti_BilanConf_Trace("********************************************************");
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
	}
	/*A
	** Verification des ITPC
	*/
	if ((vl_retcode = xzao145_Lire_Config_ITPC  (pl_FD,cmvf_FctUti_MSR_XZAO145_146,&pl_NumeroLigne))!=XDC_OK)
	{
		if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
		else sprintf(pl_Msg,"Erreur detectee dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
	 	XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb :  xzao145_Lire_Config_ITPC  a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cvce_VerifModif_cb a echoue : Sortie");
		fclose(pl_FD);
		return(XDC_NOK);
	}
	else
	{
		sprintf(pl_Msg,"Traitement de %s OK.",pl_NomFile);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);	   /* Notification dans Bilan config */
		fclose(pl_FD);
	}
	/*A
	** Ouverture du fichier de configuration des portails  
	*/
	sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_DORA_TRAV,XDF_CONF_PORTAIL );	
	if (!(pl_FD=fopen(pl_NomFile,"r")))
	{
		sprintf(pl_Msg,"Erreur d'ouverture du fichier %s\n",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_ModifSGBD_Reseau : \n%s ",pl_Msg);		
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); 	/* On previent l IHM de l administrateur */
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); 	/* Notification dans Bilan config*/		
		fclose(pl_FD);
		return(XDC_NOK);		
	}
	else
	{
		sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_ModifSGBD_Reseau : %s",pl_Msg);
		cuti_BilanConf_Trace("********************************************************");
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
	}
	/*A
	** Verification des portails 
	*/
	if ((vl_retcode = xzao150_Lire_Config_Portail (pl_FD,cmvf_FctUti_MSR_XZAO150_151,&pl_NumeroLigne))!=XDC_OK)
	{
		if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
		else sprintf(pl_Msg,"Erreur detectee dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
	 	XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb :  xzao150_Lire_Config_Portail a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cvce_VerifModif_cb a echoue : Sortie");
		fclose(pl_FD);
		return(XDC_NOK);
	}
	else
	{
		sprintf(pl_Msg,"Traitement de %s OK.",pl_NomFile);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);	   /* Notification dans Bilan config */
		fclose(pl_FD);
	}


	/*A
	** Ouverture du fichier de configuration des destinations temps de parcours  
	*/
	sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_DORA_TRAV,XDF_CONF_DTP );	
	if (!(pl_FD=fopen(pl_NomFile,"r")))
	{
		sprintf(pl_Msg,"Erreur d'ouverture du fichier %s\n",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_ModifSGBD_Reseau : \n%s ",pl_Msg);		
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); 	/* On previent l IHM de l administrateur */
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); 	/* Notification dans Bilan config*/		
		fclose(pl_FD);
		return(XDC_NOK);		
	}
	else
	{
		sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_ModifSGBD_Reseau : %s",pl_Msg);
		cuti_BilanConf_Trace("********************************************************");
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
	}

	/*A
	** Verification des destination temps de parcours 
	*/
	if ((vl_retcode = xzao406_Lire_Config_Destination_TDP (pl_FD,cmvf_FctUti_MSR_XZAO405_406,&pl_NumeroLigne))!=XDC_OK)
	{
		if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
		else sprintf(pl_Msg,"Erreur detectee dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
	 	XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb : xzao406_Lire_Config_Destination_TDP  a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cvce_VerifModif_cb a echoue : Sortie");
		fclose(pl_FD);
		return(XDC_NOK);
	}
	else
	{
		sprintf(pl_Msg,"Traitement de %s OK.",pl_NomFile);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);	   /* Notification dans Bilan config */
		fclose(pl_FD);
	}
	/*A
	** Ouverture du fichier de configuration des associations destinations temps de parcours PMV 
	*/
	sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_DORA_TRAV,XDF_CONF_DTP_PMV );	
	if (!(pl_FD=fopen(pl_NomFile,"r")))
	{
		sprintf(pl_Msg,"Erreur d'ouverture du fichier %s\n",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_ModifSGBD_Reseau : \n%s ",pl_Msg);		
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); 	/* On previent l IHM de l administrateur */
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); 	/* Notification dans Bilan config*/		
		fclose(pl_FD);
		return(XDC_NOK);		
	}
	else
	{
		sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_ModifSGBD_Reseau : %s",pl_Msg);
		cuti_BilanConf_Trace("********************************************************");
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
	}

	/*A
	** Lecture du fichier XDF_CONF_DTP_PMV
	*/
	if ((vl_retcode = xzao462_Lire_Config_Destination_TDP_PMV (pl_FD,cmvf_FctUti_MSR_XZAO461,&pl_NumeroLigne))!=XDC_OK)
	{
		if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
		else sprintf(pl_Msg,"Erreur detectee dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
	 	XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb : xzao462_Lire_Config_Destination_TDP_PMV  a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cvce_VerifModif_cb a echoue : Sortie");
		fclose(pl_FD);
		return(XDC_NOK);
	}
	else
	{
		sprintf(pl_Msg,"Traitement de %s OK.",pl_NomFile);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);	   /* Notification dans Bilan config */
		fclose(pl_FD);
	}

	/*A
	** Ouverture du fichier de configuration des associations destinations temps de parcours PMVA
	*/
	sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_DORA_TRAV,XDF_CONF_DTP_PMA );
	if (!(pl_FD=fopen(pl_NomFile,"r")))
	{
		sprintf(pl_Msg,"Erreur d'ouverture du fichier %s\n",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_ModifSGBD_Reseau : \n%s ",pl_Msg);
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); 	/* On previent l IHM de l administrateur */
		cuti_BilanConf_Trace(pl_Msg);
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); 	/* Notification dans Bilan config*/
		fclose(pl_FD);
		return(XDC_NOK);
	}
	else
	{
		sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_ModifSGBD_Reseau : %s",pl_Msg);
		cuti_BilanConf_Trace("********************************************************");
		cuti_BilanConf_Trace(pl_Msg);
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
	}

	/*A
	** Lecture du fichier XDF_CONF_DTP_PMA
	*/


	if ((vl_retcode = xzao465_Lire_Config_Destination_TDP_PMA (pl_FD,cmvf_FctUti_MSR_XZAO466,&pl_NumeroLigne))!=XDC_OK)
	{
		if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
		else sprintf(pl_Msg,"Erreur detectee dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
		cuti_BilanConf_Trace(pl_Msg);
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
	 	XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb : xzao465_Lire_Config_Destination_TDP_PMA  a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cvce_VerifModif_cb a echoue : Sortie");
		fclose(pl_FD);
		return(XDC_NOK);
	}
	else
	{
		sprintf(pl_Msg,"Traitement de %s OK.",pl_NomFile);
		cuti_BilanConf_Trace(pl_Msg);
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);	   /* Notification dans Bilan config*/
		fclose(pl_FD);
	}

	/*A
	** Verification des PR de fin de chaque segment 
	*/
	strcpy(vl_NomBase,CGCDG_Base_Courante);
	if ((vl_retcode = XZAG100_Calcul_RES_SEG_PR_Fin (vl_NomBase))!=XDC_OK)
	{
		sprintf(pl_Msg,"Erreur du remplissage de la colonne PR Fin de chaque segment");
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
	 	XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb : xzag100_Calcul_RES_SEG_PR_Fin  a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cvce_VerifModif_cb a echoue : Sortie");
		fclose(pl_FD);
		return(XDC_NOK);
	}
	else
	{
		sprintf(pl_Msg,"Traitement du remplissage du PR Fin de chaque segment ");
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);	   /* Notification dans Bilan config */
	}

	
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT: cmvf_ModifSGBD_Reseau : Fin d execution");
	return(XDC_OK);
}
/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	cmvf_Modif_SGBD_Ihm : Pour chaque classe d ihm 
* Ouverture du fichier du type.
* Lecture   du fichier du type.
* La fonction utilisateur passee en argument appelle la primitive respective afin de 
* modifier la base donnee concernant la classe de ihm.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int cmvf_Modif_SGBD_Ihm() 
/*
* ARGUMENTS EN ENTREE :  aucun 	
* ARGUMENTS EN SORTIE :  aucun
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
*
* CONDITION D'UTILISATION
*	fichier de trace de bilan config doit etre ouvert
*	les fichiers de configuration doivent exister et dument remplis
*	dans les directories definies dans le xdc.h conformement au ppl.
*
* FONCTION 	
* 	
------------------------------------------------------*/
{
	static char *	version = "$Id: cgcd_mvf_2.c,v 1.17 2020/05/19 14:52:50 gesconf Exp $ : cmvf_Modif_SGBD_Ihm";
	int     	vl_retcode;
	char  		pl_NomFile[255];          /* 
					** recoit le nom complet des fichiers a ouvrir
					*/
	
	
	char  		pl_Msg[255];		/* contient le mesg a tracer*/
	XDY_Entier	pl_NumeroLigne;		/* va contenir le nbre ligne lues par les fonctions du type XZAO50 */
	
	FILE *		pl_FD;	/* File descripteur du fichier courant de travail */
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN : cmvf_Modif_SGBD_Ihm : debut d execution");
	
	sprintf(pl_Msg,"************************************");
	cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
	sprintf(pl_Msg,"--------Modification de la configuration des machines sur la base %s --------",CGCDG_Base_Courante);
	cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
	
	/*A
	** Ouverture du fichier de configuration des influences stations
	*/
	sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_DORA_TRAV,XDF_CONF_INFLU_STATION);	
	if (!(pl_FD=fopen(pl_NomFile,"r")))
	{
		/*B 
		** En cas de prob d ouverture du fichier de conf, on logue le prob a l administrateur
		** et on le notifie dans le fichier Bilan Config
		*/
		sprintf(pl_Msg,"Erreur d'ouverture du fichier %s\n",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_Modif_SGBD_Ihm : \n%s ",pl_Msg);		
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* On previent l IHM de l administrateur */
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
		fclose(pl_FD);		
		return(XDC_NOK);		
	}
	else
	{
		sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_Modif_SGBD_Ihm : %s",pl_Msg);
		cuti_BilanConf_Trace("********************************************************");
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
	}
	/*A
	** Verification influence station
	*/
	if ((vl_retcode = xzao62_Lire_Config_Influence_Station(pl_FD,cmvf_FctUti_MSI_XZAO62_63,&pl_NumeroLigne))!=XDC_OK)
	{
		if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
		else sprintf(pl_Msg,"Erreur detectee dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
	 	XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb : xzao62_Lire_Config_Influence_Station() a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cvce_VerifModif_cb a echoue : Sortie");
		fclose(pl_FD);
		return(XDC_NOK);
	}
	else
	{
		sprintf(pl_Msg,"Traitement de %s OK.",pl_NomFile);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);	   /* Notification dans Bilan config */
		fclose(pl_FD);
	}
	/*A
	** Ouverture du fichier de configuration des troncons
	*/
	sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_DORA_TRAV,XDF_CONF_TRONCON);	
	if (!(pl_FD=fopen(pl_NomFile,"r")))
	{
		/*B 
		** En cas de prob d ouverture du fichier de conf, on logue le prob a l administrateur
		** et on le notifie dans le fichier Bilan Config
		*/
		sprintf(pl_Msg,"Erreur d'ouverture du fichier %s\n",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_Modif_SGBD_Ihm : \n%s ",pl_Msg);		
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* On previent l IHM de l administrateur */
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
		fclose(pl_FD);		
		return(XDC_NOK);		
	}
	else
	{
		sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_Modif_SGBD_Ihm : %s",pl_Msg);
		cuti_BilanConf_Trace("********************************************************");
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
	}
	/*A
	** Verification troncon
	*/
	if ((vl_retcode = xzao118_Lire_Config_Troncon(pl_FD,cmvf_FctUti_MSI_XZAO118_119,&pl_NumeroLigne))!=XDC_OK)
	{
		if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
		else sprintf(pl_Msg,"Erreur detectee dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
	 	XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb : xzao118_Lire_Config_Troncon() a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cvce_VerifModif_cb a echoue : Sortie");
		fclose(pl_FD);
		return(XDC_NOK);
	}
	else
	{
		sprintf(pl_Msg,"Traitement de %s OK.",pl_NomFile);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);	   /* Notification dans Bilan config */
		fclose(pl_FD);
	}
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT: cmvf_Modif_SGBD_Ihm : Fin d execution");
	return(XDC_OK);
}

