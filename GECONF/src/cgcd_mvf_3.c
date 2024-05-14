/*E*/
/*Fichier : $Id: cgcd_mvf_3.c,v 1.10 2018/10/23 14:11:46 devgfi Exp $      Release : $Revision: 1.10 $        Date : $Date: 2018/10/23 14:11:46 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  GECONF
------------------------------------------------------
* MODULE MMVF_3 * FICHIER cgcd_mvf_3.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*	MMVF_3 appartient a MMVF
* SERVICE RENDU : 
*******************
*	cmvf_ModifSGBD_RSAD : lecture des fichiers du repertoire
* contenant les astreintes.
*  Ouverture des types d astreintes.
*  Lecture du fichier de types d astreintes et appel a XZDC01.
*  La fonction utilisateur passee en argument appelle la primitive XZAN36
*  pour tester l existence du type dans la base de donnees.
*******************
*	cmvf_Verif_DEFT : lecture des fichiers par defaut.
* Pour chaque fichier de conf :
* Ouverture du fichier du type.
* Lecture   du fichier du type.
*
*******************
*	cmvf_Verif_SCNT : lecture des fichiers du repertoire
* contenant les scenarios customizies.
* pour chaque fichier de conf :
* Ouverture du fichier du type.
* Lecture   du fichier du type.
*
*******************
*	cmvf_Verif_SNAV : lecture des fichiers du repertoire
* contenant les scenarios NAVs customizies.
* pour chaque fichier de conf :
* Ouverture du fichier du type.
* Lecture   du fichier du type.
*
*******************
*	cmvf_Verif_EQAL :lecture des fichiers du repertoire
* contenant les scenarios s Equations d alertes customizies.
* pour chaque fichier de conf :
* Ouverture du fichier du type.
* Lecture   du fichier du type.
*
******************** 
------------------------------------------------------
* HISTORIQUE :
*
* MERCIER O.	21 Oct 1994	: Creation
***************
* MERCIER O.	14 Jan 1994	: Integration verif RSAD	V 1.5
* NIEPCERON	28 Fev 1995	: Remplacement de repertoire de liv par celui de travail v1.6
* NIEPCERON	07 Jun 1996	: remplacement du \n par en cours pour le msg Traitement du fichier ... v1.7
* NIEPCERON	09 Sep 1996	: deplacement de seuils_trafic_dai_defaut de dft dans snt v1.8
* JPL		20/02/18	: Migration Linux 64 bits (DEM 1274) : Seuils de niveau trafic DAI: test supprime  1.9
* JPL		23/10/18	: Restauration tests des seuils de niveau trafic DAI et Equations d'alerte  1.10
------------------------------------------------------*/

/* fichiers inclus */

#include "cgcd_mvf_3.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: cgcd_mvf_3.c,v 1.10 2018/10/23 14:11:46 devgfi Exp $ : cgcd_mvf_3" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */
/*----------------------------------------------------
* SERVICE RENDU : 
*******************
*	cmvf_ModifSGBD_RSAD : Ouverture des types d astreintes.
*  Lecture du fichier de types d astreintes et appel a XZDC01.
*  La fonction utilisateur passee en argument appelle la primitive XZAN36
*  pour tester l existence du type dans la base de donnees.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cmvf_ModifSGBD_RSAD(	) 
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
	static char *	version = "$Id: cgcd_mvf_3.c,v 1.10 2018/10/23 14:11:46 devgfi Exp $ : cmvf_ModifSGBD_RSAD";
	int     	vl_retcode;
	char  		pl_NomFile[255];          /* 
					** recoit le nom complet des fichiers a ouvrir
					*/
	
	
	char  		pl_Msg[255];		/* contient le mesg a tracer*/
	XDY_Entier	pl_NumeroLigne;		/* va contenir le nbre ligne lues par les fonctions du type XZAO50 */
	
	FILE *		pl_FD;	/* File descripteur du fichier courant de travail */
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN : cmvf_ModifSGBD_RSAD : debut d execution");
	
	
	sprintf(pl_Msg,"************************************");
	cuti_BilanConf_Trace(pl_Msg); 
	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
	
	sprintf(pl_Msg,"--------Verification  de la base %s par rapport aux regles du SAD--------",CGCDG_Base_Courante);
	cuti_BilanConf_Trace(pl_Msg); 
	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
	
	
	/*A
	** Ouverture du fichier de configuration des donnees classe de Distances Inter Vehiculaire. 
	*/
	sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_RSAD_TRAV,XDF_ASTREINTES_BDC);	
	if (!(pl_FD=fopen(pl_NomFile,"r")))
	{
		/*B 
		** En cas de prob d ouverture du fichier de conf, on logue le prob a l administrateur
		** et on le notifie dans le fichier Bilan Config
		*/
		sprintf(pl_Msg,"Erreur d'ouverture du fichier %s\n",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_ModifSGBD_RSAD : \n%s ",pl_Msg);		
		cuti_BilanConf_Trace(pl_Msg); 
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* On previent l IHM de l administrateur */
		fclose(pl_FD);		
		return(XDC_NOK);		
	}
	else
	{
		sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_ModifSGBD_RSAD : %s",pl_Msg);
		cuti_BilanConf_Trace("********************************************************");
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
	}
	/*A
	** Verification des donnees de la base / aux fichiers du SAD.
	*/
	if ((vl_retcode = XZDC01_LireAstreintesSAD (pl_FD,&(cmvf_FctUtil_RSAD),&pl_NumeroLigne))!=XDC_OK)
	{
		if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
		else sprintf(pl_Msg,"Erreur detectee dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
		
		XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb : XZDC01_LireAstreintesSAD () a echoue");
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
	
	
	
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT: cmvf_ModifSGBD_RSAD : Fin d execution");
	return(XDC_OK);
}
/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	cmvf_Verif_DEFT : pour chaque fichier de conf :
* Ouverture du fichier du type.
* Lecture   du fichier du type.
*
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int cmvf_Verif_DEFT() 
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
	static char *	version = "$Id: cgcd_mvf_3.c,v 1.10 2018/10/23 14:11:46 devgfi Exp $ : cmvf_Verif_DEFT";
	int     	vl_retcode;
	char  		pl_NomFile[255];     	/* 
						** recoit le nom complet des fichiers a ouvrir
						*/
	
	
	char  		pl_Msg[255];		/* contient le mesg a tracer*/
	XDY_Entier	pl_NumeroLigne = 0;	/* va contenir le nbre ligne lues par les fonctions du type XZAO50 */
	
	XDY_Fichier	pl_FD;	/* File descripteur du fichier courant de travail */
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN : cmvf_Verif_DEFT : debut d execution");
	
	sprintf(pl_Msg,"************************************");
	cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
	sprintf(pl_Msg,"--------Modification de la configuration des machines sur la base %s --------",CGCDG_Base_Courante);
	cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
	
	/*A
	** Ouverture du fichier des coefficients de calcul des donnees 6 minutes glissantes. 
	*/
	sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_DEFT_TRAV,XDF_CONF_COEF_6_MIN_GLISS);	
	
	if (XZSS_12OuvrirFichier(XZSSC_FIC_RD,pl_NomFile,&pl_FD) != XDC_OK)
	{
		/*B 
		** En cas de prob d ouverture du fichier de conf, on logue le prob a l administrateur
		** et on le notifie dans le fichier Bilan Config
		*/
		sprintf(pl_Msg,"Erreur d'ouverture du fichier %s\n",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_Verif_DEFT : \n%s ",pl_Msg);		
		cuti_BilanConf_Trace(pl_Msg); 
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* On previent l IHM de l administrateur */
		close(pl_FD);		
		return(XDC_NOK);		
	}
	else
	{
		sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_Verif_DEFT : %s",pl_Msg);
		cuti_BilanConf_Trace("********************************************************");
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
	}
	/*A
	** Verification  coefficients de calcul des donnees 6 minutes.
	*/
	if ((vl_retcode = XZEC20_Verif_CDMG(pl_FD,&pl_NumeroLigne))!=XDC_OK)
	{
		if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
		else sprintf(pl_Msg,"Erreur detectee dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
		
		XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb : XZEC20_Verif_CDMG () a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cvce_VerifModif_cb a echoue : Sortie");
		close(pl_FD);
		return(XDC_NOK);
	}
	else
	{
		sprintf(pl_Msg,"Traitement de %s OK.",pl_NomFile);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
		close(pl_FD);
	}
	/*A
	** Ouverture du fichier de configuration des donnees classe de Distances Inter Vehiculaire. 
	*/
	sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_DEFT_TRAV,XDF_CONF_DIST_INTER_VEHI);	
	if (XZSS_12OuvrirFichier(XZSSC_FIC_RD,pl_NomFile,&pl_FD) != XDC_OK)
	{
		/*B 
		** En cas de prob d ouverture du fichier de conf, on logue le prob a l administrateur
		** et on le notifie dans le fichier Bilan Config
		*/
		sprintf(pl_Msg,"Erreur d'ouverture du fichier %s\n",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_Verif_DEFT : \n%s ",pl_Msg);		
		cuti_BilanConf_Trace(pl_Msg); 
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* On previent l IHM de l administrateur */
		close(pl_FD);		
		return(XDC_NOK);		
	}
	else
	{
		sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_Verif_DEFT : %s",pl_Msg);
		cuti_BilanConf_Trace("********************************************************");
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
	}
	/*A
	** Verification des donnees classe de Distances Inter Vehiculaire .
	*/
	if ((vl_retcode = XZEC21_Verif_Classe_DIV(pl_FD,&pl_NumeroLigne))!=XDC_OK)
	{
		if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
		else sprintf(pl_Msg,"Erreur detectee dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
		
		XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb :XZEC21_Verif_Classe_DIV () a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cvce_VerifModif_cb a echoue : Sortie");
		close(pl_FD);
		return(XDC_NOK);
	}
	else
	{
		sprintf(pl_Msg,"Traitement de %s OK.",pl_NomFile);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);	   /* Notification dans Bilan config */
		close(pl_FD);
	}
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT: cmvf_Verif_DEFT : Fin d execution");
	return(XDC_OK);
}

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	cmvf_Verif_SCNT : pour chaque fichier de conf :
* Ouverture du fichier du type.
* Lecture   du fichier du type.
*
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int cmvf_Verif_SCNT() 
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
	static char *	version = "$Id: cgcd_mvf_3.c,v 1.10 2018/10/23 14:11:46 devgfi Exp $ : cmvf_Verif_SCNT";
	int     	vl_retcode;
	char  		pl_NomFile[255];       	/* 
						** recoit le nom complet des fichiers a ouvrir
						*/
	
	
	char  		pl_Msg[255];		/* contient le mesg a tracer*/
	XDY_Entier	pl_NumeroLigne = 0;	/* va contenir le nbre ligne lues par les fonctions du type XZAO50 */
	
	XDY_Fichier	pl_FD;	/* File descripteur du fichier courant de travail */

	XZST_03EcritureTrace(XZSTC_FONCTION,"IN : cmvf_Verif_SCNT : debut d execution");
	/*A
	** Ouverture du fichier contenant des seuils trafic pour alerte DAI. 
	*/
	sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_SCNT_TRAV,XDF_CONF_SEUIL_TRAFIC_DAI_DEFAUT);	
	if (XZSS_12OuvrirFichier(XZSSC_FIC_RD,pl_NomFile,&pl_FD) != XDC_OK)
	{
		/*B 
		** En cas de prob d ouverture du fichier de conf, on logue le prob a l administrateur
		** et on le notifie dans le fichier Bilan Config
		*/
		sprintf(pl_Msg,"Erreur d'ouverture du fichier %s\n",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_Verif_SCNT : \n%s ",pl_Msg);		
		cuti_BilanConf_Trace(pl_Msg); 
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* On previent l IHM de l administrateur */
		close(pl_FD);		
		return(XDC_NOK);		
	}
	else
	{
		sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_Verif_SCNT : %s",pl_Msg);
		cuti_BilanConf_Trace("********************************************************");
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
	}
	/*A
	** Verification des seuils trafic pour alerte DAI.
	*/
	if ((vl_retcode = XZEC22_Seuil_Trafic(pl_FD,&pl_NumeroLigne))!=XDC_OK)
	{
		if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
		else sprintf(pl_Msg,"Erreur detectee dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
		
		XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb : XZEC22_Seuil_Trafic () a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cvce_VerifModif_cb a echoue : Sortie");
		close(pl_FD);
		return(XDC_NOK);
	}
	else
	{
		sprintf(pl_Msg,"Traitement de %s OK.",pl_NomFile);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);	   /* Notification dans Bilan config */
		close(pl_FD);
	}
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT: cmvf_Verif_SCNT : Fin d execution");
	return(XDC_OK);
}

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	cmvf_Verif_EQAL : pour chaque fichier de conf :
* Ouverture du fichier du type.
* Lecture   du fichier du type.
*
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int cmvf_Verif_EQAL() 
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
	static char *	version = "$Id: cgcd_mvf_3.c,v 1.10 2018/10/23 14:11:46 devgfi Exp $ : cmvf_Verif_EQAL";
	int     	vl_retcode;
	char  		pl_NomFile[255];          /* 
					** recoit le nom complet des fichiers a ouvrir
					*/
	
	
	char  		pl_Msg[255];		/* contient le mesg a tracer*/
	XDY_Entier	pl_NumeroLigne = 0;	/* va contenir le nbre ligne lues par les fonctions du type XZAO50 */
	
	XDY_Fichier	pl_FD;	/* File descripteur du fichier courant de travail */
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN : cmvf_Verif_EQAL : debut d execution");
	/*A
	** Ouverture du fichier de configuration des equations d'alerte. 
	*/
	sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_EQAL_TRAV,XDF_CONF_EQU_ALERTE);	
	if (XZSS_12OuvrirFichier(XZSSC_FIC_RD,pl_NomFile,&pl_FD) != XDC_OK)
	{
		
		sprintf(pl_Msg,"Erreur d'ouverture du fichier %s\n",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_Verif_EQAL : \n%s ",pl_Msg);		
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); 
		close(pl_FD);		
		return(XDC_NOK);		
	}
	else
	{
		sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_Verif_EQAL : %s",pl_Msg);
		cuti_BilanConf_Trace("********************************************************");
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); 
	}
	/*A
	** Verification des equations d'alerte.
	*/
	if ((vl_retcode = XZEC24_Equation_Alerte (pl_FD,&pl_NumeroLigne))!=XDC_OK)
	{
		if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
		else sprintf(pl_Msg,"Erreur detectee dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
		
		XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb : XZEC24_Equation_Alerte () a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cvce_VerifModif_cb a echoue : Sortie");
		close(pl_FD);
		return(XDC_NOK);
	}
	else
	{
		sprintf(pl_Msg,"Traitement de %s OK.",pl_NomFile);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);	   
		close(pl_FD);
	}
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT: cmvf_Verif_EQAL : Fin d execution");
	return(XDC_OK);
}

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	cmvf_FctUtil_RSAD : fonction utilisateur pour cmvf_ModifSGBD_RSAD
* XZDC01.
*
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int cmvf_FctUtil_RSAD (XDY_Nom         va_NomTypeAstreinte,XDY_Octet       va_NumTypeAstreinte) 
/*
* ARGUMENTS EN ENTREE :  
* ARGUMENTS EN SORTIE :  aucun
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
*
* CONDITION D'UTILISATION
*
* FONCTION 	
* 	
------------------------------------------------------*/
{
	static char *	version			=	"$Id: cgcd_mvf_3.c,v 1.10 2018/10/23 14:11:46 devgfi Exp $ : cmvf_Verif_EQAL";
	char    		vl_NomBase [4];
	int     		vl_retcode;
	XDY_Booleen		vl_Existence	=	TRUE;
	char			vl_Explication [255];
	
	XZST_03EcritureTrace (XZSTC_FONCTION,"IN: cmvf_FctUtil_RSAD : Debut d execution NomTypeAstreinte = %s, NumTypeAstreinte = %d",
				va_NomTypeAstreinte, va_NumTypeAstreinte);
	
	strcpy( vl_NomBase,CGCDG_Base_Courante);	
	vl_retcode = XZAN36_Existence_Astreinte(vl_NomBase, va_NumTypeAstreinte, &vl_Existence);
	
	/*A si le type est inconnu ou n'existe pas en base */
	if ( (vl_retcode != XDC_OK) || (vl_Existence != TRUE) )
	{
		XZST_03EcritureTrace(XZSTC_WARNING,
			"Le type d'astreinte %d (%s) n'existe pas dans la base de donnees %s",
			va_NumTypeAstreinte, va_NomTypeAstreinte, vl_NomBase);
		
		/*B La coherence n est pas bonne */
		
		sprintf(vl_Explication,
			"Le type d'astreinte %d (%s) est utilise dans la base de connaissances.\n",
			va_NumTypeAstreinte, va_NomTypeAstreinte);
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,vl_Explication);
		cuti_BilanConf_Trace(vl_Explication); 
		XZST_03EcritureTrace(XZSTC_FONCTION,"OUT: cmvf_FctUtil_RSAD : Fin d execution : Coherence Non OK");
		return(XDC_NOK);	
	}
				
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT: cmvf_FctUtil_RSAD : Fin d execution");
	return(XDC_OK);
}
