/*E*/
/*Fichier :  $Id: cgcd_mvf.c,v 1.27 2021/05/03 13:46:24 pc2dpdy Exp $      Release : $Revision: 1.27 $        Date : $Date: 2021/05/03 13:46:24 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  GECONF
------------------------------------------------------
* MODULE MMVF * FICHIER cgcd_mvf.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*	cmvf_ModifSGBD_Reseau : se trouve dans cgcd_mvf_2.c.
********************** 
* 	cmvf_Trace_TYPE_Result : fonction permettant de construire le message de trace Bilan Config
* et de tester le mode d execution des Fct du type XZAO51 appelee par les fcts utilisateurs.
* Cette fonction n est appelee que par les fct utilisateurs.
**********************
*	cmvf_Modif_SGBD_Type : Pour chaque type 
* (evenement,astreinte,equipement.)
* Ouverture du fichier du type.
* Lecture   du fichier du type.
* La fonction utilisateur passee en argument appelle la primitive respective afin de 
* modifier la base donnee concernant la classe de machine.
*
**********************	
* 	cmvf_Modif_SGBD_Machine : Pour chaque classe de machine 
* (machine, suppleant, serveur de terminaux.)
* Ouverture du fichier de la classe machine.
* Lecture   du fichier de la classe machine.
* La fonction utilisateur passee en argument appelle la primitive respective afin de 
* modifier la base donnee concernant la classe de machine.
*  
**********************
*	cmvf_Modif_SGBD_Equipements : Pour chaque classe d equipement .
* Ouverture du fichier du type.
* Lecture   du fichier du type.
* La fonction utilisateur passee en argument appelle la primitive respective afin de 
* modifier la base donnee concernant la classe de machine.
**********************  
*	cmvf_Modif_SGBD_Ihm : se trouve dans cgcd_mvf_2.c.
**********************
* 	cmvf_Trace_TYPE_Result : se trouve dans cgcd_mvf_2.c.
**********************
------------------------------------------------------
* HISTORIQUE :
*
* mercier	13 Oct 1994	: Creation
************************
* Mercier	28 Nov 1994	: Ajout de XZAN02_01				V 1.4
* pour cmvf_Modif_SGBD_Type permettant de verifier 
* le type d astreinte.
************************
* Mercier	19 DEC 1994	: Modif	des path fichiers de donnees 		V 1.5
************************
* MERCIER O.	14 Jan 1995	: Ajout de message d erreur pour l admin 	V 1.7
* TORREGROSSA.	01 Dec 1995	: Ajout lecture du fichier de config des fax	V 1.8
* 
* NIEPCERON 28 Fev 1995     : Remplacement de repertoire de liv par celui de travail v1.9
* NIEPCERON	07 Jun 1996	: remplacement du \n par en cours pour le msg Traitement du fichier ... v1.10
* MISMER    06 Nov 1996     : Ajout de la fonction de lecture des equipements generique LCR v1.11 (DEM/1232)
* MISMER	10 Fev 1997	: Ajout des fonction temps de parcours v1.11 (DEM/1395)
* Guilhou	02 jan 1998	: ajout des zones d'influence PMV (dem/1534) 1.13
* C.T.	21 jan 1998	: ajout des panneaux d'alerte (dem/1545) 1.14
* JBL	Nov 2007	: DEM 666
* JMG	25/09/09	: ajout appel xzao352_Lire_Config_Zon_TUN  1.17
* JPL	08/10/10	: Traitement configuration des zones de couverture cameras (DEM 948)  1.18
* JMG	25/09/09	: ajout appel xzao352_Lire_Config_Zon_TUN 
* VR	15/12/11 : Ajout PRV (DEM/1016)
* ABE	27-04-2015 : projet suppression ancienne GTC (GTCA)
* JMG	26/04/16: ajout SAGA 1.21
* JMG	26/03/17: ajout SAGA 1.21 DEM1191
* LCL	26/06/17: Ajout zones urbaines 1.23 convergence
* JPL	10/01/17 : Traitement configuration des points de mesure de la DIT (DEM 1266)  1.24
* LCL	12/02/18 : Ajout controleurs de feux CTRL_FEUX 1.25 DEM1284
* LCL	28/05/18 : Ajout GABEB 1.26 DEM1283
* CGR	29/07/20 : Ajout IMU 1.27 DEM-SAE155
* CGR	06/10/20 : Ajout Zones et Messages pour IMU DEM-SAE155
* ABK	18/08/23 : Ajout nouveau type Picto (DEM/483)
* GGY	04/01/24 : Modification ordonacement tcgcd (DEM-483)
------------------------------------------------------*/

/* fichiers inclus */

#include "cgcd_mvf.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: cgcd_mvf.c,v 1.27 2021/05/03 13:46:24 pc2dpdy Exp $ : cgcd_mvf" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	cmvf_Modif_SGBD_Machine : Pour chaque classe de machine 
* (machine, suppleant, serveur de terminaux.)
* Ouverture du fichier de la classe machine.
* Lecture   du fichier de la classe machine.
* La fonction utilisateur passee en argument appelle la primitive respective afin de 
* modifier la base donnee concernant la classe de machine.
*  
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cmvf_Modif_SGBD_Machine() 
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
	static char *	version	= "$Id: cgcd_mvf.c,v 1.27 2021/05/03 13:46:24 pc2dpdy Exp $ : cmvf_Modif_SGBD_Machine";
	int     	vl_retcode;
	char  		pl_NomFile[255];          /* 
					** recoit le nom complet des fichiers a ouvrir
					*/
	
	
	char  		pl_Msg[255];		/* contient le mesg a tracer*/
	XDY_Entier	pl_NumeroLigne;		/* va contenir le nbre ligne lues par les fonctions du type XZAO50 */
	
	FILE *		pl_FD;	/* File descripteur du fichier courant de travail */
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN : cmvf_Modif_SGBD_Machine : debut d execution");
	
	sprintf(pl_Msg,"************************************");
	cuti_BilanConf_Trace(pl_Msg); 
	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
	
	sprintf(pl_Msg,"--------Modification de la configuration des machines sur la base %s --------",CGCDG_Base_Courante);
	cuti_BilanConf_Trace(pl_Msg); 
	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
	
	
	/*A
	** Ouverture du fichier de configuration des machines
	*/
	sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_EQUI_TRAV,XDF_CONF_MACHINE);	
	if (!(pl_FD=fopen(pl_NomFile,"r")))
	{
		/*B 
		** En cas de prob d ouverture du fichier de conf, on logue le prob a l administrateur
		** et on le notifie dans le fichier Bilan Config
		*/
		sprintf(pl_Msg,"Erreur d'ouverture du fichier %s\n",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_Modif_SGBD_Machine : \n%s ",pl_Msg);		
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* On previent l IHM de l administrateur */
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
		fclose(pl_FD);		
		return(XDC_NOK);		
	}
	else
	{
		sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_Modif_SGBD_Machine : %s",pl_Msg);
		cuti_BilanConf_Trace("********************************************************");
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); 
	}
	/*A
	** Verification machine
	*/
	if ((vl_retcode = xzao95_Lire_Config_Machine(pl_FD,cmvf_FctUti_MSM_XZAO95_64,&pl_NumeroLigne))!=XDC_OK)
	{
		if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
		else sprintf(pl_Msg,"Erreur detectée dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
	 	XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb : xzao95_Lire_Config_Machine() a echoue");
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
	** Ouverture du fichier de configuration des suppleants
	*/
	sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_EQUI_TRAV,XDF_CONF_SUPPLEANT);	
	if (!(pl_FD=fopen(pl_NomFile,"r")))
	{
		/*B 
		** En cas de prob d ouverture du fichier de conf, on logue le prob a l administrateur
		** et on le notifie dans le fichier Bilan Config
		*/
		sprintf(pl_Msg,"Erreur d'ouverture du fichier %s\n",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_Modif_SGBD_Machine : \n%s ",pl_Msg);		
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* On previent l IHM de l administrateur */
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
		fclose(pl_FD);		
		return(XDC_NOK);		
	}
	else
	{
		sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_Modif_SGBD_Machine : %s",pl_Msg);
		cuti_BilanConf_Trace("********************************************************");
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
	}
	/*A
	** Verification suppleants
	*/
	if ((vl_retcode = xzao96_Lire_Config_Suppleant (pl_FD,cmvf_FctUti_MSM_XZAO96_65,&pl_NumeroLigne))!=XDC_OK)
	{
		if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
		else sprintf(pl_Msg,"Erreur detectée dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
	 	XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb : xzao96_Lire_Config_Suppleant () a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cvce_VerifModif_cb a echoue : Sortie");
		fclose(pl_FD);
		return(XDC_NOK);
	}
	else
	{
		sprintf(pl_Msg,"Traitement de %s OK.",pl_NomFile);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
		fclose(pl_FD);
	}	
	/*A
	** Ouverture du fichier de configuration des serveurs de terminaux
	*/
	sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_EQUI_TRAV,XDF_CONF_SERV_TERM);	
	if (!(pl_FD=fopen(pl_NomFile,"r")))
	{
		/*B 
		** En cas de prob d ouverture du fichier de conf, on logue le prob a l administrateur
		** et on le notifie dans le fichier Bilan Config
		*/
		sprintf(pl_Msg,"Erreur d'ouverture du fichier %s\n",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_Modif_SGBD_Machine : \n%s ",pl_Msg);		
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* On previent l IHM de l administrateur */
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
		fclose(pl_FD);		
		return(XDC_NOK);		
	}
	else
	{
		sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_Modif_SGBD_Machine : %s",pl_Msg);
		cuti_BilanConf_Trace("********************************************************");
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
	}
	/*A
	** Verification serveurs de terminaux
	*/
	if ((vl_retcode = xzao97_Lire_Config_ServeurTerminaux(pl_FD,cmvf_FctUti_MSM_XZAO97_66,&pl_NumeroLigne))!=XDC_OK)
	{
		if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
		else sprintf(pl_Msg,"Erreur detectée dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
	 	XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb : xzao97_Lire_Config_ServeurTerminaux() a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cvce_VerifModif_cb a echoue : Sortie");
		fclose(pl_FD);
		return(XDC_NOK);
	}
	else
	{
		sprintf(pl_Msg,"Traitement de %s OK.",pl_NomFile);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);	   
		fclose(pl_FD);
	}		
		
	sprintf(pl_Msg,"*."  /* ,pl_NomFile */);
	cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
		
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT: cmvf_Modif_SGBD_Machine : Fin d execution");
	return(XDC_OK);
}
/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	cmvf_Modif_SGBD_Type : Pour chaque type 
* (evenement,astreinte,equipement.)
* Ouverture du fichier du type.
* Lecture   du fichier du type.
* La fonction utilisateur passee en argument appelle la primitive respective afin de 
* modifier la base donnee concernant la classe de machine.
*  
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cmvf_Modif_SGBD_Type(	) 
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
	static char *	version	= "$Id: cgcd_mvf.c,v 1.27 2021/05/03 13:46:24 pc2dpdy Exp $ : cmvf_Modif_SGBD_Type";
	int     	vl_retcode;
	char  		pl_NomFile[255];          /* 
					** recoit le nom complet des fichiers a ouvrir
					*/
	
	
	char  		pl_Msg[255];		/* contient le mesg a tracer*/
	XDY_Entier	pl_NumeroLigne;		/* va contenir le nbre ligne lues par les fonctions du type XZAO50 */
	
	FILE *		pl_FD;	/* File descripteur du fichier courant de travail */
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN : cmvf_Modif_SGBD_Type : debut d execution");
	
	sprintf(pl_Msg,"************************************");
	cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
	sprintf(pl_Msg,"--------Modification de la configuration des types sur la base %s --------",CGCDG_Base_Courante);
	cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
	
	/*A
	** Ouverture du fichier de configuration des types PMV
	*/
	sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_EQUI_TRAV,XDF_CONF_TYPE_PMV);	
	if (!(pl_FD=fopen(pl_NomFile,"r")))
	{
		/*B 
		** En cas de prob d ouverture du fichier de conf, on logue le prob a l administrateur
		** et on le notifie dans le fichier Bilan Config
		*/
		sprintf(pl_Msg,"Erreur d'ouverture du fichier %s\n",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_Modif_SGBD_Type : \n%s ",pl_Msg);		
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* On previent l IHM de l administrateur */
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
		fclose(pl_FD);		
		return(XDC_NOK);		
	}
	else
	{
		sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_Modif_SGBD_Type : %s",pl_Msg);
		cuti_BilanConf_Trace("********************************************************");
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
	}
	/*A
	** Verification type PMV
	*/
	if ((vl_retcode = xzao101_Lire_Config_Type_PMV(pl_FD,cmvf_FctUti_MST_XZAO101_70,&pl_NumeroLigne))!=XDC_OK)
	{
		if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
		else sprintf(pl_Msg,"Erreur detectée dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
	 	XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb : xzao101_Lire_Config_Type_PMV() a echoue");
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
	** Ouverture du fichier de configuration des types Picto 
	*/
	sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_EQUI_TRAV,XDF_CONF_TYPE_PICTO);	
	if (!(pl_FD=fopen(pl_NomFile,"r")))
	{
		/*B 
		** En cas de prob d ouverture du fichier de conf, on logue le prob a l administrateur
		** et on le notifie dans le fichier Bilan Config
		*/
		sprintf(pl_Msg,"Erreur d'ouverture du fichier %s\n",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_Modif_SGBD_Type : \n%s ",pl_Msg);		
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* On previent l IHM de l administrateur */
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
		fclose(pl_FD);		
		return(XDC_NOK);		
	}
	else
	{
		sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_Modif_SGBD_Type : %s",pl_Msg);
		cuti_BilanConf_Trace("********************************************************");
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
	}
	/*A
	** Verification type Picto
	*/
	if ((vl_retcode = xzao102_Lire_Config_Type_Picto(pl_FD,cmvf_FctUti_MST_XZAO102_71,&pl_NumeroLigne))!=XDC_OK)
	{
		if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
		else sprintf(pl_Msg,"Erreur detectee dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
	 	XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb : xzao102_Lire_Config_Type_Picto () a echoue");
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
	** Ouverture du fichier de types d astreintes
	*/
	sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_DORA_TRAV,XDF_TYPE_ASTREINTE );	
	if (!(pl_FD=fopen(pl_NomFile,"r")))
	{
		/*B 
		** En cas de prob d ouverture du fichier de conf, on logue le prob a l administrateur
		** et on le notifie dans le fichier Bilan Config
		*/
		sprintf(pl_Msg,"Erreur d'ouverture du fichier %s\n",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_Modif_SGBD_Type : \n%s ",pl_Msg);		
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); 	/* On previent l IHM de l administrateur */
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); 	/* Notification dans Bilan config*/
		fclose(pl_FD);		
		return(XDC_NOK);		
	}
	else
	{
		sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_Modif_SGBD_Type : %s",pl_Msg);
		cuti_BilanConf_Trace("********************************************************");
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
	}
	/*A
	** Verification des types d astreintes
	*/
	if ((vl_retcode = XZAN02_Lire_Type_Astreinte (pl_FD,cmvf_FctUti_MST_XZAN02_01,&pl_NumeroLigne))!=XDC_OK)
	{
		if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
		else sprintf(pl_Msg,"Erreur detectée dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
	 	XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb : XZAN02_Lire_Type_Astreinte () a echoue");
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
	** Ouverture du fichier de configuration des Alertes
	*/
	sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_EQUI_TRAV,XDF_CONF_ALERTE );	
	if (!(pl_FD=fopen(pl_NomFile,"r")))
	{
		/*B 
		** En cas de prob d ouverture du fichier de conf, on logue le prob a l administrateur
		** et on le notifie dans le fichier Bilan Config
		*/
		sprintf(pl_Msg,"Erreur d'ouverture du fichier %s\n",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_Modif_SGBD_Type : \n%s ",pl_Msg);		
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* On previent l IHM de l administrateur */
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
		fclose(pl_FD);		
		return(XDC_NOK);		
	}
	else
	{
		sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_Modif_SGBD_Type : %s",pl_Msg);
		cuti_BilanConf_Trace("********************************************************");
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
	}
	/*A
	** Verification des Alertes
	*/
	if ((vl_retcode = xzao198_Lire_Config_Alerte (pl_FD, cmvf_FctUti_MSE_XZAO198_188  ,&pl_NumeroLigne))!=XDC_OK)
	{
		if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
		else sprintf(pl_Msg,"Erreur detectée dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
	 	XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb : xzao198_Lire_Config_Alerte () a echoue");
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
	** Ouverture du fichier de configuration des Alarmes
	*/
	sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_EQUI_TRAV,XDF_CONF_ALARME );	
	if (!(pl_FD=fopen(pl_NomFile,"r")))
	{
		/*B 
		** En cas de prob d ouverture du fichier de conf, on logue le prob a l administrateur
		** et on le notifie dans le fichier Bilan Config
		*/
		sprintf(pl_Msg,"Erreur d'ouverture du fichier %s\n",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_Modif_SGBD_Type : \n%s ",pl_Msg);		
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* On previent l IHM de l administrateur */
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
		fclose(pl_FD);		
		return(XDC_NOK);		
	}
	else
	{
		sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_Modif_SGBD_Type : %s",pl_Msg);
		cuti_BilanConf_Trace("********************************************************");
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
	}
	/*A
	** Verification des Alarmes
	*/
	if ((vl_retcode = xzao196_Lire_Config_Alarme (pl_FD, cmvf_FctUti_MSE_XZAO196_189  ,&pl_NumeroLigne))!=XDC_OK)
	{
		if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
		else sprintf(pl_Msg,"Erreur detectée dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
	 	XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb : xzao196_Lire_Config_Alarme () a echoue");
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

	
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT: cmvf_Modif_SGBD_Type : Fin d execution");
	return(XDC_OK);
}

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	cmvf_Modif_SGBD_Equipements : Pour chaque classe d equipement .
* 
* Ouverture du fichier du type.
* Lecture   du fichier du type.
* La fonction utilisateur passee en argument appelle la primitive respective afin de 
* modifier la base donnee concernant la classe de machine.
*  
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int cmvf_Modif_SGBD_Equipements(	) 
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
	static char *	version	= "$Id: cgcd_mvf.c,v 1.27 2021/05/03 13:46:24 pc2dpdy Exp $ : cmvf_Modif_SGBD_Equipements";
	int     	vl_retcode;
	char  		pl_NomFile[255];    	/* 
						** recoit le nom complet des fichiers a ouvrir
						*/
	
	
	char  		pl_Msg[255];		/* contient le mesg a tracer*/
	XDY_Entier	pl_NumeroLigne;		/* va contenir le nbre ligne lues par les fonctions du type XZAO50 */
	
	FILE *		pl_FD;	/* File descripteur du fichier courant de travail */
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN : cmvf_Modif_SGBD_Equipements : debut d execution");

	XZST_08ForceNivVisuTraceLocal(XZSTC_FATAL|XZSTC_WARNING|XZSTC_INFO|XZSTC_FONCTION|XZSTC_INTERFACE|XZSTC_MESSAGE|XZSTC_DEBUG1,XZSTC_NECRAN);	
	sprintf(pl_Msg,"************************************");
	cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
	sprintf(pl_Msg,"--------Modification de la configuration des equipements sur la base %s --------",CGCDG_Base_Courante);
	cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);


	/*A
	** Ouverture du fichier de configuration des PRV
	*/
	sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_EQUI_TRAV,XDF_CONF_PRV);	
	if (!(pl_FD=fopen(pl_NomFile,"r")))
	{
		/*B 
		** En cas de prob d ouverture du fichier de conf, on logue le prob a l administrateur
		** et on le notifie dans le fichier Bilan Config
		*/
		sprintf(pl_Msg,"Erreur d'ouverture du fichier %s\n",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_Modif_SGBD_Equipements : \n%s ",pl_Msg);		
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* On previent l IHM de l administrateur */
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
		fclose(pl_FD);		
		return(XDC_NOK);		
	}
	else
	{
		sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_Modif_SGBD_Equipements : %s",pl_Msg);
		cuti_BilanConf_Trace("********************************************************");
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
	}
	/*A
	** Verification PRV
	*/
	if ((vl_retcode = xzao255_Lire_Config_PRV(pl_FD,cmvf_FctUti_MSE_XZAO257_PRV,&pl_NumeroLigne))!=XDC_OK)
	{
		if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
		else sprintf(pl_Msg,"Erreur detectee dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
	 	XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb :xzao255_Lire_Config_PRV () a echoue");
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
	** Ouverture du fichier de configuration des PIC
	*/
	sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_EQUI_TRAV,XDF_CONF_PIC);	
	if (!(pl_FD=fopen(pl_NomFile,"r")))
	{
		/*B 
		** En cas de prob d ouverture du fichier de conf, on logue le prob a l administrateur
		** et on le notifie dans le fichier Bilan Config
		*/
		sprintf(pl_Msg,"Erreur d'ouverture du fichier %s\n",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_Modif_SGBD_Equipements : \n%s ",pl_Msg);		
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* On previent l IHM de l administrateur */
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
		fclose(pl_FD);		
		return(XDC_NOK);		
	}
	else
	{
		sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_Modif_SGBD_Equipements : %s",pl_Msg);
		cuti_BilanConf_Trace("********************************************************");
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
	}
	/*A
	** Verification PIC
	*/
	if ((vl_retcode = xzao2551_Lire_Config_PIC(pl_FD,cmvf_FctUti_MSE_XZAO2571_PIC,&pl_NumeroLigne))!=XDC_OK)
	{
		if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
		else sprintf(pl_Msg,"Erreur detectee dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
	 	XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb :xzao255_Lire_Config_PIC () a echoue");
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
	** Ouverture du fichier de configuration des DAI
	*/
	sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_EQUI_TRAV,XDF_CONF_DAI);	
	if (!(pl_FD=fopen(pl_NomFile,"r")))
	{
		/*B 
		** En cas de prob d ouverture du fichier de conf, on logue le prob a l administrateur
		** et on le notifie dans le fichier Bilan Config
		*/
		sprintf(pl_Msg,"Erreur d'ouverture du fichier %s\n",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_Modif_SGBD_Equipements : \n%s ",pl_Msg);		
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* On previent l IHM de l administrateur */
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
		fclose(pl_FD);		
		return(XDC_NOK);		
	}
	else
	{
		sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_Modif_SGBD_Equipements : %s",pl_Msg);
		cuti_BilanConf_Trace("********************************************************");
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
	}
	/*A
	** Verification DAI
	*/
	if ((vl_retcode = xzao105_Lire_Config_DAI (pl_FD,cmvf_FctUti_MSE_XZAO105_80,&pl_NumeroLigne))!=XDC_OK)
	{
		if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
		else sprintf(pl_Msg,"Erreur detectée dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
	 	XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb : xzao105_Lire_Config_DAI () a echoue");
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
        ** Ouverture du fichier de configuration des IMU
        */
        sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_EQUI_TRAV,XDF_CONF_IMU);
        if (!(pl_FD=fopen(pl_NomFile,"r")))
        {
                /*B 
                ** En cas de prob d ouverture du fichier de conf, on logue le prob a l administrateur
                ** et on le notifie dans le fichier Bilan Config
                */
                sprintf(pl_Msg,"Erreur d'ouverture du fichier %s\n",pl_NomFile);
                XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_Modif_SGBD_Equipements : \n%s ",pl_Msg);
                XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* On previent l IHM de l administrateur */
                cuti_BilanConf_Trace(pl_Msg);
                XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
                fclose(pl_FD);
                return(XDC_NOK);
        }
        else
        {
                sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
                XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_Modif_SGBD_Equipements : %s",pl_Msg);
                cuti_BilanConf_Trace("********************************************************");
                cuti_BilanConf_Trace(pl_Msg);
                XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
        }
        /*A
        ** Verification IMU
        */
        if ((vl_retcode = xzao493_Lire_Config_IMU(pl_FD,cmvf_FctUti_MSE_XZAO492_IMU,&pl_NumeroLigne))!=XDC_OK)
        {
                if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
                else sprintf(pl_Msg,"Erreur detectee dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
                cuti_BilanConf_Trace(pl_Msg);
                XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
                XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb :xzao493_Lire_Config_IMU () a echoue");
                XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cvce_VerifModif_cb a echoue : Sortie");
                fclose(pl_FD);
                return(XDC_NOK);
        }
        else
        {
                sprintf(pl_Msg,"Traitement de %s OK.",pl_NomFile);
                cuti_BilanConf_Trace(pl_Msg);
                XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);         /* Notification dans Bilan config */
                fclose(pl_FD);
        }


	/*A
        ** Ouverture du fichier de configuration des Zones IMU
        */
	sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_EQUI_TRAV,XDF_CONF_IMU_ZON);
	if (!(pl_FD=fopen(pl_NomFile,"r")))
	{
		/*B
		** En cas de prob d ouverture du fichier de conf, on logue le prob a l administrateur
		** et on le notifie dans le fichier Bilan Config
		*/
		sprintf(pl_Msg,"Erreur d'ouverture du fichier %s\n",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_Modif_SGBD_Equipements : \n%s ",pl_Msg);
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* On previent l IHM de l administrateur */
		cuti_BilanConf_Trace(pl_Msg);
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
		fclose(pl_FD);
		return(XDC_NOK);
	}
	
	else
	{
		sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_Modif_SGBD_Equipements : %s",pl_Msg);
		cuti_BilanConf_Trace("********************************************************");
		cuti_BilanConf_Trace(pl_Msg);
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
	}
	/*A
	** Verification Zones IMU
	*/
	if ((vl_retcode = xzao496_Lire_Config_Zon_IMU(pl_FD, cmvf_FctUti_MSE_XZAO495_Zon_IMU, &pl_NumeroLigne))!=XDC_OK)
	{
		if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
		else sprintf(pl_Msg,"Erreur detectee dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
		cuti_BilanConf_Trace(pl_Msg);
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
		XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb :xzao496_Lire_Config_Zon_IMU () a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cvce_VerifModif_cb a echoue : Sortie");
		fclose(pl_FD);
		return(XDC_NOK);
	}
	else
	{
		sprintf(pl_Msg,"Traitement de %s OK.",pl_NomFile);
		cuti_BilanConf_Trace(pl_Msg);
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);         /* Notification dans Bilan config */
		fclose(pl_FD);
	}


	/*A
        ** Ouverture du fichier de configuration des Messages IMU
	*/
	sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_EQUI_TRAV,XDF_CONF_IMU_MSG);
	if (!(pl_FD=fopen(pl_NomFile,"r")))
	{
		/*B
		** En cas de prob d ouverture du fichier de conf, on logue le prob a l administrateur
		** et on le notifie dans le fichier Bilan Config
		*/
		sprintf(pl_Msg,"Erreur d'ouverture du fichier %s\n",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_Modif_SGBD_Equipements : \n%s ",pl_Msg);
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* On previent l IHM de l administrateur */
		cuti_BilanConf_Trace(pl_Msg);
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
		fclose(pl_FD);
		return(XDC_NOK);
	}
	else
	{
		sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_Modif_SGBD_Equipements : %s",pl_Msg);
		cuti_BilanConf_Trace("********************************************************");
		cuti_BilanConf_Trace(pl_Msg);
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
	}
	/*A
        ** Verification Messages IMU
	*/
	if ((vl_retcode = xzao499_Lire_Config_Msg_IMU(pl_FD, cmvf_FctUti_MSE_XZAO498_Msg_IMU, &pl_NumeroLigne))!=XDC_OK)
	{
		if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
		else sprintf(pl_Msg,"Erreur detectee dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
		cuti_BilanConf_Trace(pl_Msg);
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
		XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb :xzao499_Lire_Config_Msg_IMU () a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cvce_VerifModif_cb a echoue : Sortie");
		fclose(pl_FD);
		return(XDC_NOK);
	}
	else
	{
		sprintf(pl_Msg,"Traitement de %s OK.",pl_NomFile);
		cuti_BilanConf_Trace(pl_Msg);
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);         /* Notification dans Bilan config */
		fclose(pl_FD);
	}



	/* TEST JBL */

    /* AAZ 29/11/2007 ajout ouvrages lit, ltn et sys*/

	/*A
	** Ouverture du fichier de configuration des ouvrages lits d'arret
	*/
	sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_EQUI_TRAV,XDF_CONF_OUV_LIT );	
	if (!(pl_FD=fopen(pl_NomFile,"r")))
	{
		/*B 
		** En cas de prob d ouverture du fichier de conf, on logue le prob a l administrateur
		** et on le notifie dans le fichier Bilan Config
		*/
		sprintf(pl_Msg,"Erreur d'ouverture du fichier %s\n",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_Modif_SGBD_Equipements : \n%s ",pl_Msg);		
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* On previent l IHM de l administrateur */
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
		fclose(pl_FD);		
		return(XDC_NOK);		
	}
	else
	{
		sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_Modif_SGBD_Equipements : %s",pl_Msg);
		cuti_BilanConf_Trace("********************************************************");
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
	}
	/*A
	** Verification des ouvrages lits d'arret
	*/
	if ((vl_retcode = xzao1003_Lire_Config_Lit (pl_FD,  cmvf_FctUti_MSE_XZAO1003_1004 ,&pl_NumeroLigne))!=XDC_OK)
	{
		if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
		else sprintf(pl_Msg,"Erreur detectée dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
	 	XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb : xzao1003_Lire_Config_Lit () a echoue");
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
	** Ouverture du fichier de configuration des ouvrages locaux techniques
	*/
	sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_EQUI_TRAV,XDF_CONF_OUV_LTN );	
	if (!(pl_FD=fopen(pl_NomFile,"r")))
	{
		/*B 
		** En cas de prob d ouverture du fichier de conf, on logue le prob a l administrateur
		** et on le notifie dans le fichier Bilan Config
		*/
		sprintf(pl_Msg,"Erreur d'ouverture du fichier %s\n",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_Modif_SGBD_Equipements : \n%s ",pl_Msg);		
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* On previent l IHM de l administrateur */
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
		fclose(pl_FD);		
		return(XDC_NOK);		
	}
	else
	{
		sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_Modif_SGBD_Equipements : %s",pl_Msg);
		cuti_BilanConf_Trace("********************************************************");
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
	}
	/*A
	** Verification des ouvrages locaux techniques
	*/
	if ((vl_retcode = xzao1005_Lire_Config_Ltn (pl_FD,  cmvf_FctUti_MSE_XZAO1005_1006 ,&pl_NumeroLigne))!=XDC_OK)
	{
		if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
		else sprintf(pl_Msg,"Erreur detectée dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
	 	XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb : xzao1005_Lire_Config_Ltn () a echoue");
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
	** Ouverture du fichier de configuration des ouvrages locaux techniques
	*/
	sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_EQUI_TRAV,XDF_CONF_EQT_SAGA );	
	if (!(pl_FD=fopen(pl_NomFile,"r")))
	{
		/*B 
		** En cas de prob d ouverture du fichier de conf, on logue le prob a l administrateur
		** et on le notifie dans le fichier Bilan Config
		*/
		sprintf(pl_Msg,"Erreur d'ouverture du fichier %s\n",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_Modif_SGBD_Equipements : \n%s ",pl_Msg);		
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* On previent l IHM de l administrateur */
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
		fclose(pl_FD);		
		return(XDC_NOK);		
	}
	else
	{
		sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_Modif_SGBD_Equipements : %s",pl_Msg);
		cuti_BilanConf_Trace("********************************************************");
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
	}
	/*A
	** Verification des eqts saga
	*/
	if ((vl_retcode = xzao480_Lire_Config_Eqt_SAGA (pl_FD,  cmvf_FctUti_MSE_XZAO422_730 ,&pl_NumeroLigne))!=XDC_OK)
	{
		if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
		else sprintf(pl_Msg,"Erreur detectée dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
	 	XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb : xzao480_Lire_Config_Eqt_SAGA () a echoue");
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
	** Ouverture du fichier de configuration des ouvrages EHT
	*/
	sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_EQUI_TRAV,XDF_CONF_OUV_EHT );	
	if (!(pl_FD=fopen(pl_NomFile,"r")))
	{
		/*B 
		** En cas de prob d ouverture du fichier de conf, on logue le prob a l administrateur
		** et on le notifie dans le fichier Bilan Config
		*/
		sprintf(pl_Msg,"Erreur d'ouverture du fichier %s\n",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_Modif_SGBD_Equipements : \n%s ",pl_Msg);		
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* On previent l IHM de l administrateur */
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
		fclose(pl_FD);		
		return(XDC_NOK);		
	}
	else
	{
		sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_Modif_SGBD_Equipements : %s",pl_Msg);
		cuti_BilanConf_Trace("********************************************************");
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
	}
	/*A
	** Verification des ouvrages EHT
	*/
	if ((vl_retcode = xzao1009_Lire_Config_Eht (pl_FD,  cmvf_FctUti_MSE_XZAO1009_1010 ,&pl_NumeroLigne))!=XDC_OK)
	{
		if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
		else sprintf(pl_Msg,"Erreur detectée dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
	 	XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb : xzao1009_Lire_Config_Eht () a echoue");
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

    /* AAZ 14/11/2007 */
	/*A
	** Ouverture du fichier de configuration de la  GTC Nice Renovee
	*/
	sprintf( pl_NomFile, "%s/%s", CLIVC_LIVR_EQUI_TRAV, XDF_CONF_GTC_NICE_RENOVEE );	
	if ( !( pl_FD = fopen( pl_NomFile, "r" ) ) )
	{
		/*B 
		** En cas de prob d ouverture du fichier de conf, on logue le prob a l administrateur
		** et on le notifie dans le fichier Bilan Config
		*/
		sprintf( pl_Msg, "Erreur d'ouverture du fichier %s\n", pl_NomFile);
		XZST_03EcritureTrace( XZSTC_WARNING, "TCGCD : cmvf_Modif_SGBD_Equipements : \n%s ", pl_Msg );		
		XZIA08_AffMsgAdm( CGCDV_NOMMACHINE, pl_Msg ); /* On previent l IHM de l administrateur */
		cuti_BilanConf_Trace( pl_Msg ); 
	 	XZIA08_AffMsgAdm( CGCDV_NOMMACHINE, pl_Msg ); /* Notification dans Bilan config*/
		fclose( pl_FD );		
		return( XDC_NOK );		
	}
	else
	{
		sprintf( pl_Msg, "Traitement du fichier %s en cours", pl_NomFile );
		XZST_03EcritureTrace( XZSTC_DEBUG1, "cmvf_Modif_SGBD_Equipements : %s", pl_Msg );
		cuti_BilanConf_Trace( "********************************************************" );
		cuti_BilanConf_Trace( pl_Msg ); 
	 	XZIA08_AffMsgAdm( CGCDV_NOMMACHINE, pl_Msg ); /* Notification dans Bilan config*/
	}

	   /*A 
    ** Verification GTC Nice Renovee
    */
    if ((vl_retcode = xzao0165_Lire_Config_GTC_NiceRENOVEE (pl_FD, cmvf_FctUti_MSE_XZAO0165_0166  ,&pl_NumeroLigne))!=XDC_OK)
    {
        if (pl_NumeroLigne == 0) 
            sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
        else
            sprintf(pl_Msg,"Erreur detect�e dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
        cuti_BilanConf_Trace( pl_Msg ); 
        XZIA08_AffMsgAdm( CGCDV_NOMMACHINE, pl_Msg );
        XZST_03EcritureTrace( XZSTC_WARNING , "TCGCD : cvce_VerifModif_cb : xzao0165_Lire_Config_GTC_NiceRENOVEE () a echoue" );
        XZST_03EcritureTrace( XZSTC_FONCTION, "OUT : cvce_VerifModif_cb a echoue : Sortie" );
        fclose( pl_FD );
        return( XDC_NOK );
    }
    else
    {
        sprintf( pl_Msg, "Traitement de %s OK.", pl_NomFile );
        cuti_BilanConf_Trace( pl_Msg ); 
        XZIA08_AffMsgAdm( CGCDV_NOMMACHINE, pl_Msg );       /* Notification dans Bilan config */
        fclose( pl_FD );
    }
	/* AAZ */
    /* AAZ 16/11/2007 */
    
    /* Sequences  du domaine SIG */
    
    /*A
    ** Ouverture du fichier de configuration des sequences  du domaine SIG
    */
    sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_EQUI_TRAV,XDF_CONF_SEQ_SIG );    
    if (!(pl_FD=fopen(pl_NomFile,"r")))
    {
        /*B 
        ** En cas de prob d ouverture du fichier de conf, on logue le prob a l administrateur
        ** et on le notifie dans le fichier Bilan Config
        */
        sprintf( pl_Msg, "Erreur d'ouverture du fichier %s\n", pl_NomFile );
        XZST_03EcritureTrace( XZSTC_WARNING, "TCGCD : cmvf_Modif_SGBD_Equipements : \n%s ", pl_Msg);        
        XZIA08_AffMsgAdm( CGCDV_NOMMACHINE, pl_Msg ); /* On previent l IHM de l administrateur */
        cuti_BilanConf_Trace( pl_Msg ); 
        XZIA08_AffMsgAdm( CGCDV_NOMMACHINE, pl_Msg ); /* Notification dans Bilan config*/
        fclose( pl_FD );        
        return( XDC_NOK );        
    }
    else
    {
        sprintf( pl_Msg, "Traitement du fichier %s en cours", pl_NomFile );
        XZST_03EcritureTrace( XZSTC_DEBUG1, "cmvf_Modif_SGBD_Equipements : %s", pl_Msg);
        cuti_BilanConf_Trace( "********************************************************" );
        cuti_BilanConf_Trace( pl_Msg ); 
         XZIA08_AffMsgAdm( CGCDV_NOMMACHINE, pl_Msg ); /* Notification dans Bilan config*/
    }


    /*A
    ** Verification des sequences  domaine SIG
    */
    if ((vl_retcode = xzao993_Lire_Config_Seq_SIG (pl_FD, cmvf_FctUti_MSE_XZAO993_994  ,&pl_NumeroLigne))!=XDC_OK)
    {
        if (pl_NumeroLigne == 0) 
            sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
        else
            sprintf(pl_Msg,"Erreur detectée dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
        cuti_BilanConf_Trace( pl_Msg ); 
        XZIA08_AffMsgAdm( CGCDV_NOMMACHINE, pl_Msg );
        XZST_03EcritureTrace( XZSTC_WARNING , "TCGCD : cvce_VerifModif_cb : xzao993_Lire_Config_Seq_SIG () a echoue" );
        XZST_03EcritureTrace( XZSTC_FONCTION, "OUT : cvce_VerifModif_cb a echoue : Sortie" );
        fclose( pl_FD );
        return( XDC_NOK );
    }
    else
    {
        sprintf( pl_Msg, "Traitement de %s OK.", pl_NomFile );
        cuti_BilanConf_Trace( pl_Msg ); 
        XZIA08_AffMsgAdm( CGCDV_NOMMACHINE, pl_Msg );       /* Notification dans Bilan config */
        fclose( pl_FD );
    }

    
    /* Sequences  du domaine ECL */
    
    /*A
    ** Ouverture du fichier de configuration des sequences  du domaine ECL
    */
    sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_EQUI_TRAV,XDF_CONF_SEQ_ECL );    
    if (!(pl_FD=fopen(pl_NomFile,"r")))
    {
        /*B 
        ** En cas de prob d ouverture du fichier de conf, on logue le prob a l administrateur
        ** et on le notifie dans le fichier Bilan Config
        */
        sprintf( pl_Msg, "Erreur d'ouverture du fichier %s\n", pl_NomFile );
        XZST_03EcritureTrace( XZSTC_WARNING, "TCGCD : cmvf_Modif_SGBD_Equipements : \n%s ", pl_Msg);        
        XZIA08_AffMsgAdm( CGCDV_NOMMACHINE, pl_Msg ); /* On previent l IHM de l administrateur */
        cuti_BilanConf_Trace( pl_Msg ); 
        XZIA08_AffMsgAdm( CGCDV_NOMMACHINE, pl_Msg ); /* Notification dans Bilan config*/
        fclose( pl_FD );        
        return( XDC_NOK );        
    }
    else
    {
        sprintf( pl_Msg, "Traitement du fichier %s en cours", pl_NomFile );
        XZST_03EcritureTrace( XZSTC_DEBUG1, "cmvf_Modif_SGBD_Equipements : %s", pl_Msg);
        cuti_BilanConf_Trace( "********************************************************" );
        cuti_BilanConf_Trace( pl_Msg ); 
         XZIA08_AffMsgAdm( CGCDV_NOMMACHINE, pl_Msg ); /* Notification dans Bilan config*/
    }


    /*A
    ** Verification des sequences  domaine ECL
    */
    if ((vl_retcode = xzao995_Lire_Config_Seq_ECL (pl_FD, cmvf_FctUti_MSE_XZAO995_996  ,&pl_NumeroLigne))!=XDC_OK)
    {
        if (pl_NumeroLigne == 0) 
            sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
        else
            sprintf(pl_Msg,"Erreur detectée dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
        cuti_BilanConf_Trace( pl_Msg ); 
        XZIA08_AffMsgAdm( CGCDV_NOMMACHINE, pl_Msg );
        XZST_03EcritureTrace( XZSTC_WARNING , "TCGCD : cvce_VerifModif_cb : xzao995_Lire_Config_Seq_ECL () a echoue" );
        XZST_03EcritureTrace( XZSTC_FONCTION, "OUT : cvce_VerifModif_cb a echoue : Sortie" );
        fclose( pl_FD );
        return( XDC_NOK );
    }
    else
    {
        sprintf( pl_Msg, "Traitement de %s OK.", pl_NomFile );
        cuti_BilanConf_Trace( pl_Msg ); 
        XZIA08_AffMsgAdm( CGCDV_NOMMACHINE, pl_Msg );       /* Notification dans Bilan config */
        fclose( pl_FD );
    }

    
    /* Sequences  du domaine VEN */
    
    /*A
    ** Ouverture du fichier de configuration des sequences  du domaine VEN
    */
    sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_EQUI_TRAV,XDF_CONF_SEQ_VEN );    
    if (!(pl_FD=fopen(pl_NomFile,"r")))
    {
        /*B 
        ** En cas de prob d ouverture du fichier de conf, on logue le prob a l administrateur
        ** et on le notifie dans le fichier Bilan Config
        */
        sprintf( pl_Msg, "Erreur d'ouverture du fichier %s\n", pl_NomFile );
        XZST_03EcritureTrace( XZSTC_WARNING, "TCGCD : cmvf_Modif_SGBD_Equipements : \n%s ", pl_Msg);        
        XZIA08_AffMsgAdm( CGCDV_NOMMACHINE, pl_Msg ); /* On previent l IHM de l administrateur */
        cuti_BilanConf_Trace( pl_Msg ); 
        XZIA08_AffMsgAdm( CGCDV_NOMMACHINE, pl_Msg ); /* Notification dans Bilan config*/
        fclose( pl_FD );        
        return( XDC_NOK );        
    }
    else
    {
        sprintf( pl_Msg, "Traitement du fichier %s en cours", pl_NomFile );
        XZST_03EcritureTrace( XZSTC_DEBUG1, "cmvf_Modif_SGBD_Equipements : %s", pl_Msg);
        cuti_BilanConf_Trace( "********************************************************" );
        cuti_BilanConf_Trace( pl_Msg ); 
         XZIA08_AffMsgAdm( CGCDV_NOMMACHINE, pl_Msg ); /* Notification dans Bilan config*/
    }


    /*A
    ** Verification des sequences  domaine VEN
    */
    if ((vl_retcode = xzao997_Lire_Config_Seq_VEN (pl_FD, cmvf_FctUti_MSE_XZAO997_998  ,&pl_NumeroLigne))!=XDC_OK)
    {
        if (pl_NumeroLigne == 0) 
            sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
        else
            sprintf(pl_Msg,"Erreur detectée dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
        cuti_BilanConf_Trace( pl_Msg ); 
        XZIA08_AffMsgAdm( CGCDV_NOMMACHINE, pl_Msg );
        XZST_03EcritureTrace( XZSTC_WARNING , "TCGCD : cvce_VerifModif_cb : xzao997_Lire_Config_Seq_VEN () a echoue" );
        XZST_03EcritureTrace( XZSTC_FONCTION, "OUT : cvce_VerifModif_cb a echoue : Sortie" );
        fclose( pl_FD );
        return( XDC_NOK );
    }
    else
    {
        sprintf( pl_Msg, "Traitement de %s OK.", pl_NomFile );
        cuti_BilanConf_Trace( pl_Msg ); 
        XZIA08_AffMsgAdm( CGCDV_NOMMACHINE, pl_Msg );       /* Notification dans Bilan config */
        fclose( pl_FD );
    }

    
    /* Sequences  du domaine ENR */
    
    /*A
    ** Ouverture du fichier de configuration des sequences  du domaine ENR
    */
    sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_EQUI_TRAV,XDF_CONF_SEQ_ENR );    
    if (!(pl_FD=fopen(pl_NomFile,"r")))
    {
        /*B 
        ** En cas de prob d ouverture du fichier de conf, on logue le prob a l administrateur
        ** et on le notifie dans le fichier Bilan Config
        */
        sprintf( pl_Msg, "Erreur d'ouverture du fichier %s\n", pl_NomFile );
        XZST_03EcritureTrace( XZSTC_WARNING, "TCGCD : cmvf_Modif_SGBD_Equipements : \n%s ", pl_Msg);        
        XZIA08_AffMsgAdm( CGCDV_NOMMACHINE, pl_Msg ); /* On previent l IHM de l administrateur */
        cuti_BilanConf_Trace( pl_Msg ); 
        XZIA08_AffMsgAdm( CGCDV_NOMMACHINE, pl_Msg ); /* Notification dans Bilan config*/
        fclose( pl_FD );        
        return( XDC_NOK );        
    }
    else
    {
        sprintf( pl_Msg, "Traitement du fichier %s en cours", pl_NomFile );
        XZST_03EcritureTrace( XZSTC_DEBUG1, "cmvf_Modif_SGBD_Equipements : %s", pl_Msg);
        cuti_BilanConf_Trace( "********************************************************" );
        cuti_BilanConf_Trace( pl_Msg ); 
         XZIA08_AffMsgAdm( CGCDV_NOMMACHINE, pl_Msg ); /* Notification dans Bilan config*/
    }


    /*A
    ** Verification des sequences  domaine ENR
    */
    if ((vl_retcode = xzao999_Lire_Config_Seq_ENR (pl_FD, cmvf_FctUti_MSE_XZAO999_1000  ,&pl_NumeroLigne))!=XDC_OK)
    {
        if (pl_NumeroLigne == 0) 
            sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
        else
            sprintf(pl_Msg,"Erreur detectée dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
        cuti_BilanConf_Trace( pl_Msg ); 
        XZIA08_AffMsgAdm( CGCDV_NOMMACHINE, pl_Msg );
        XZST_03EcritureTrace( XZSTC_WARNING , "TCGCD : cvce_VerifModif_cb : xzao999_Lire_Config_Seq_ENR () a echoue" );
        XZST_03EcritureTrace( XZSTC_FONCTION, "OUT : cvce_VerifModif_cb a echoue : Sortie" );
        fclose( pl_FD );
        return( XDC_NOK );
    }
    else
    {
        sprintf( pl_Msg, "Traitement de %s OK.", pl_NomFile );
        cuti_BilanConf_Trace( pl_Msg ); 
        XZIA08_AffMsgAdm( CGCDV_NOMMACHINE, pl_Msg );       /* Notification dans Bilan config */
        fclose( pl_FD );
    }
    
    /* Sequences  du domaine INC */
    
    /*A
    ** Ouverture du fichier de configuration des sequences  du domaine INC
    */
    sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_EQUI_TRAV,XDF_CONF_SEQ_INC );    
    if (!(pl_FD=fopen(pl_NomFile,"r")))
    {
        /*B 
        ** En cas de prob d ouverture du fichier de conf, on logue le prob a l administrateur
        ** et on le notifie dans le fichier Bilan Config
        */
        sprintf( pl_Msg, "Erreur d'ouverture du fichier %s\n", pl_NomFile );
        XZST_03EcritureTrace( XZSTC_WARNING, "TCGCD : cmvf_Modif_SGBD_Equipements : \n%s ", pl_Msg);        
        XZIA08_AffMsgAdm( CGCDV_NOMMACHINE, pl_Msg ); /* On previent l IHM de l administrateur */
        cuti_BilanConf_Trace( pl_Msg ); 
        XZIA08_AffMsgAdm( CGCDV_NOMMACHINE, pl_Msg ); /* Notification dans Bilan config*/
        fclose( pl_FD );        
        return( XDC_NOK );        
    }
    else
    {
        sprintf( pl_Msg, "Traitement du fichier %s en cours", pl_NomFile );
        XZST_03EcritureTrace( XZSTC_DEBUG1, "cmvf_Modif_SGBD_Equipements : %s", pl_Msg);
        cuti_BilanConf_Trace( "********************************************************" );
        cuti_BilanConf_Trace( pl_Msg ); 
         XZIA08_AffMsgAdm( CGCDV_NOMMACHINE, pl_Msg ); /* Notification dans Bilan config*/
    }


    /*A
    ** Verification des sequences  domaine INC
    */
    if ((vl_retcode = xzao1001_Lire_Config_Seq_INC (pl_FD, cmvf_FctUti_MSE_XZAO1001_1002  ,&pl_NumeroLigne))!=XDC_OK)
    {
        if (pl_NumeroLigne == 0) 
            sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
        else
            sprintf(pl_Msg,"Erreur detectée dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
        cuti_BilanConf_Trace( pl_Msg ); 
        XZIA08_AffMsgAdm( CGCDV_NOMMACHINE, pl_Msg );
        XZST_03EcritureTrace( XZSTC_WARNING , "TCGCD : cvce_VerifModif_cb : xzao1001_Lire_Config_Seq_INC () a echoue" );
        XZST_03EcritureTrace( XZSTC_FONCTION, "OUT : cvce_VerifModif_cb a echoue : Sortie" );
        fclose( pl_FD );
        return( XDC_NOK );
    }
    else
    {
        sprintf( pl_Msg, "Traitement de %s OK.", pl_NomFile );
        cuti_BilanConf_Trace( pl_Msg ); 
        XZIA08_AffMsgAdm( CGCDV_NOMMACHINE, pl_Msg );       /* Notification dans Bilan config */
        fclose( pl_FD );
    }
	
	/* USINE GTIE return XDC_OK; */
	/* FIN TEST JBL */		

        /*A
        ** Ouverture du fichier de configuration des CAP
        */
        sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_EQUI_TRAV,XDF_CONF_CAP);
        if (!(pl_FD=fopen(pl_NomFile,"r")))
        {
                /*B 
                ** En cas de prob d ouverture du fichier de conf, on logue le prob a l administrateur
                ** et on le notifie dans le fichier Bilan Config
                */
                sprintf(pl_Msg,"Erreur d'ouverture du fichier %s\n",pl_NomFile);
                XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_Modif_SGBD_Equipements : \n%s ",pl_Msg);
                XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* On previent l IHM de l administrateur */
                cuti_BilanConf_Trace(pl_Msg);
                XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
                fclose(pl_FD);
                return(XDC_NOK);
        }
        else
        {
                sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
                XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_Modif_SGBD_Equipements : %s",pl_Msg);
                cuti_BilanConf_Trace("********************************************************");
                cuti_BilanConf_Trace(pl_Msg);
                XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
        }
        /*A
        ** Verification CAP
        */
        if ((vl_retcode = xzao263_Lire_Config_CAP(pl_FD,cmvf_FctUti_MSE_XZAO265_CAP,&pl_NumeroLigne))!=XDC_OK)
        {
                if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
                else sprintf(pl_Msg,"Erreur detectee dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
                cuti_BilanConf_Trace(pl_Msg);
                XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
                XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb :xzao265_Lire_Config_CAP () a echoue");
                XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cvce_VerifModif_cb a echoue : Sortie");
                fclose(pl_FD);
                return(XDC_NOK);
        }
        else
        {
                sprintf(pl_Msg,"Traitement de %s OK.",pl_NomFile);
                cuti_BilanConf_Trace(pl_Msg);
                XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);         /* Notification dans Bilan config */
                fclose(pl_FD);
        }

	/*A
	** Ouverture du fichier de configuration des stations RADT
	*/
	sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_EQUI_TRAV,XDF_CONF_STATION_RADT);	
	if (!(pl_FD=fopen(pl_NomFile,"r")))
	{
		/*B 
		** En cas de prob d ouverture du fichier de conf, on logue le prob a l administrateur
		** et on le notifie dans le fichier Bilan Config
		*/
		sprintf(pl_Msg,"Erreur d'ouverture du fichier %s\n",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_Modif_SGBD_Equipements : \n%s ",pl_Msg);		
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* On previent l IHM de l administrateur */
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
		fclose(pl_FD);		
		return(XDC_NOK);		
	}
	else
	{
		sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_Modif_SGBD_Equipements : %s",pl_Msg);
		cuti_BilanConf_Trace("********************************************************");
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
	}
	/*A
	** Verification RADT
	*/
	if ((vl_retcode = xzao98_Lire_Config_RADT(pl_FD,cmvf_FctUti_MSE_XZAO98_67,&pl_NumeroLigne))!=XDC_OK)
	{
		if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
		else sprintf(pl_Msg,"Erreur detectée dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
	 	XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb : xzao98_Lire_Config_RADT() a echoue");
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
	** Ouverture du fichier de configuration des PMV pictos
	*/
	sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_EQUI_TRAV,XDF_CONF_PMV_PICTO);	
	if (!(pl_FD=fopen(pl_NomFile,"r")))
	{
		/*B 
		** En cas de prob d ouverture du fichier de conf, on logue le prob a l administrateur
		** et on le notifie dans le fichier Bilan Config
		*/
		sprintf(pl_Msg,"Erreur d'ouverture du fichier %s\n",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_Modif_SGBD_Equipements : \n%s ",pl_Msg);		
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* On previent l IHM de l administrateur */
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
		fclose(pl_FD);		
		return(XDC_NOK);		
	}
	else
	{
		sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_Modif_SGBD_Equipements : %s",pl_Msg);
		cuti_BilanConf_Trace("********************************************************");
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
	}
	/*A
	** Verification PMV pictos
	*/
	if ((vl_retcode = xzao103_Lire_Config_PMV_Picto(pl_FD,cmvf_FctUti_MSE_XZAO103_72,&pl_NumeroLigne))!=XDC_OK)
	{
		if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
		else sprintf(pl_Msg,"Erreur detectée dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
	 	XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb :xzao103_Lire_Config_PMV_Picto () a echoue");
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
	** Ouverture du fichier de configuration des zones d'influence PMV
	*/
	sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_EQUI_TRAV,XDF_CONF_ZON_PMV);	
	if (!(pl_FD=fopen(pl_NomFile,"r")))
	{
		/*B 
		** En cas de prob d ouverture du fichier de conf, on logue le prob a l administrateur
		** et on le notifie dans le fichier Bilan Config
		*/
		sprintf(pl_Msg,"Erreur d'ouverture du fichier %s\n",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_Modif_SGBD_Equipements : \n%s ",pl_Msg);		
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* On previent l IHM de l administrateur */
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
		fclose(pl_FD);		
		return(XDC_NOK);		
	}
	else
	{
		sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_Modif_SGBD_Equipements : %s",pl_Msg);
		cuti_BilanConf_Trace("********************************************************");
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
	}
	/*A
	** Verification des zones d'influence PMV
	*/
	if ((vl_retcode = xzao417_Lire_Config_Zon_PMV (pl_FD, cmvf_FctUti_MSE_XZAO417_416  ,&pl_NumeroLigne))!=XDC_OK)
	{
		if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
		else sprintf(pl_Msg,"Erreur detectée dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
	 	XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb : xzao417_Lire_Config_Zon_PMV () a echoue");
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
	** Ouverture du fichier de configuration des zones d'influence PMV_SAGA
	*/
	sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_EQUI_TRAV,XDF_CONF_ZON_PMV_SAGA);	
	if (!(pl_FD=fopen(pl_NomFile,"r")))
	{
		/*B 
		** En cas de prob d ouverture du fichier de conf, on logue le prob a l administrateur
		** et on le notifie dans le fichier Bilan Config
		*/
		sprintf(pl_Msg,"Erreur d'ouverture du fichier %s\n",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_Modif_SGBD_Equipements : \n%s ",pl_Msg);		
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* On previent l IHM de l administrateur */
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
		fclose(pl_FD);		
		return(XDC_NOK);		
	}
	else
	{
		sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_Modif_SGBD_Equipements : %s",pl_Msg);
		cuti_BilanConf_Trace("********************************************************");
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
	}
	/*A
	** Verification des zones d'influence PMV
	*/
	if ((vl_retcode = xzao417_Lire_Config_Zon_PMV (pl_FD, cmvf_FctUti_MSE_XZAO816  ,&pl_NumeroLigne))!=XDC_OK)
	{
		if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
		else sprintf(pl_Msg,"Erreur detectée dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
	 	XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb : xzao417_Lire_Config_Zon_PMV_SAGA () a echoue");
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
	** Ouverture du fichier de configuration des Mini GTC
	*/
	sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_EQUI_TRAV,XDF_CONF_MINI_GTC);	
	if (!(pl_FD=fopen(pl_NomFile,"r")))
	{
		/*B 
		** En cas de prob d ouverture du fichier de conf, on logue le prob a l administrateur
		** et on le notifie dans le fichier Bilan Config
		*/
		sprintf(pl_Msg,"Erreur d'ouverture du fichier %s\n",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_Modif_SGBD_Equipements : \n%s ",pl_Msg);		
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* On previent l IHM de l administrateur */
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
		fclose(pl_FD);		
		return(XDC_NOK);		
	}
	else
	{
		sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_Modif_SGBD_Equipements : %s",pl_Msg);
		cuti_BilanConf_Trace("********************************************************");
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
	}
	/*A
	** Verification Mini GTC 
	*/
	if ((vl_retcode = xzao104_Lire_Config_Mini_GTC (pl_FD,cmvf_FctUti_MSE_XZAO104_73,&pl_NumeroLigne))!=XDC_OK)
	{
		if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
		else sprintf(pl_Msg,"Erreur detectée dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
	 	XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb : xzao104_Lire_Config_Mini_GTC () a echoue");
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
	** Ouverture du fichier de configuration des SysVideo
	*/
	sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_EQUI_TRAV,XDF_CONF_SYS_VIDEO);	
	if (!(pl_FD=fopen(pl_NomFile,"r")))
	{
		/*B 
		** En cas de prob d ouverture du fichier de conf, on logue le prob a l administrateur
		** et on le notifie dans le fichier Bilan Config
		*/
		sprintf(pl_Msg,"Erreur d'ouverture du fichier %s\n",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_Modif_SGBD_Equipements : \n%s ",pl_Msg);		
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* On previent l IHM de l administrateur */
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
		fclose(pl_FD);		
		return(XDC_NOK);		
	}
	else
	{
		sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_Modif_SGBD_Equipements : %s",pl_Msg);
		cuti_BilanConf_Trace("********************************************************");
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
	}
	/*A
	** Verification SysVideo
	*/
	if ((vl_retcode = xzao107_Lire_Config_SystemeVideo (pl_FD,cmvf_FctUti_MSE_XZAO107_137,&pl_NumeroLigne))!=XDC_OK)
	{
		if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
		else sprintf(pl_Msg,"Erreur detectée dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
	 	XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb : xzao107_Lire_Config_SystemeVideo () a echoue");
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
	** Ouverture du fichier de configuration des fax
	*/
	sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_EQUI_TRAV,XDF_CONF_FAX);	
	if (!(pl_FD=fopen(pl_NomFile,"r")))
	{
		/*B 
		** En cas de prob d ouverture du fichier de conf, on logue le prob a l administrateur
		** et on le notifie dans le fichier Bilan Config
		*/
		sprintf(pl_Msg,"Erreur d'ouverture du fichier %s\n",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_Modif_SGBD_Fax : \n%s ",pl_Msg);		
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* On previent l IHM de l administrateur */
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
		fclose(pl_FD);		
		return(XDC_NOK);		
	}
	else
	{
		sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_Modif_SGBD_Fax : %s",pl_Msg);
		cuti_BilanConf_Trace("********************************************************");
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); 
	}
	/*A
	** Verification fax
	*/
	if ((vl_retcode = xzao191_Lire_Config_Fax(pl_FD,cmvf_FctUti_MSE_XZAO191_190,&pl_NumeroLigne))!=XDC_OK)
	{
		if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
		else sprintf(pl_Msg,"Erreur detectée dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
	 	XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb : xzao191_Lire_Config_Fax() a echoue");
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
	** Ouverture du fichier de configuration des MAA
	*/
	sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_EQUI_TRAV,XDF_CONF_MAA);	
	if (!(pl_FD=fopen(pl_NomFile,"r")))
	{
		/*B 
		** En cas de prob d ouverture du fichier de conf, on logue le prob a l administrateur
		** et on le notifie dans le fichier Bilan Config
		*/
		sprintf(pl_Msg,"Erreur d'ouverture du fichier %s\n",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_Modif_SGBD_Equipements : \n%s ",pl_Msg);		
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* On previent l IHM de l administrateur */
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
		fclose(pl_FD);		
		return(XDC_NOK);		
	}
	else
	{
		sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_Modif_SGBD_Equipements : %s",pl_Msg);
		cuti_BilanConf_Trace("********************************************************");
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
	}
	/*A
	** Verification MAA
	*/
	if ((vl_retcode = xzao112_Lire_Config_MAA (pl_FD,cmvf_FctUti_MSE_XZAO112_142,&pl_NumeroLigne))!=XDC_OK)
	{
		if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
		else sprintf(pl_Msg,"Erreur detectée dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
	 	XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb : xzao112_Lire_Config_MAA () a echoue");
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
	** Ouverture du fichier de configuration des PIRAU
	*/
	sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_EQUI_TRAV,XDF_CONF_PIRAU );	
	if (!(pl_FD=fopen(pl_NomFile,"r")))
	{
		/*B 
		** En cas de prob d ouverture du fichier de conf, on logue le prob a l administrateur
		** et on le notifie dans le fichier Bilan Config
		*/
		sprintf(pl_Msg,"Erreur d'ouverture du fichier %s\n",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_Modif_SGBD_Equipements : \n%s ",pl_Msg);		
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* On previent l IHM de l administrateur */
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
		fclose(pl_FD);		
		return(XDC_NOK);		
	}
	else
	{
		sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_Modif_SGBD_Equipements : %s",pl_Msg);
		cuti_BilanConf_Trace("********************************************************");
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
	}
	/*A
	** Verification PIRAU
	*/
	if ((vl_retcode =  xzao113_Lire_Config_PIRAU (pl_FD,cmvf_FctUti_MSE_XZAO113_143,&pl_NumeroLigne))!=XDC_OK)
	{
		if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
		else sprintf(pl_Msg,"Erreur detectée dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
	 	XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb : xzao113_Lire_Config_PIRAU  () a echoue");
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
	** Ouverture du fichier de configuration des Åquipement generique LCR
	*/
	sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_EQUI_TRAV,XDF_CONF_EQT_GEN_LCR );	
	if (!(pl_FD=fopen(pl_NomFile,"r")))
	{
		/*B 
		** En cas de prob d ouverture du fichier de conf, on logue le prob a l administrateur
		** et on le notifie dans le fichier Bilan Config
		*/
		sprintf(pl_Msg,"Erreur d'ouverture du fichier %s\n",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_Modif_SGBD_Equipements : \n%s ",pl_Msg);		
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* On previent l IHM de l administrateur */
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
		fclose(pl_FD);		
		return(XDC_NOK);		
	}
	else
	{
		sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_Modif_SGBD_Equipements : %s",pl_Msg);
		cuti_BilanConf_Trace("********************************************************");
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
	}
	/*A
	** Verification des Åquipement generique LCR
	*/
	if ((vl_retcode = xzao197_Lire_Config_Eqt_Generique_LCR (pl_FD, cmvf_FctUti_MSE_XZAO197_187  ,&pl_NumeroLigne))!=XDC_OK)
	{
		if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
		else sprintf(pl_Msg,"Erreur detectée dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
	 	XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb : xzao197_Lire_Config_Eqt_Generique_LCR () a echoue");
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
	** Ouverture du fichier de configuration des Åquipement UGTP (Temps De Parcours)
	*/
	sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_EQUI_TRAV,XDF_CONF_EQT_TDP );	
	if (!(pl_FD=fopen(pl_NomFile,"r")))
	{
		/*B 
		** En cas de prob d ouverture du fichier de conf, on logue le prob a l administrateur
		** et on le notifie dans le fichier Bilan Config
		*/
		sprintf(pl_Msg,"Erreur d'ouverture du fichier %s\n",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_Modif_SGBD_Equipements : \n%s ",pl_Msg);		
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* On previent l IHM de l administrateur */
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
		fclose(pl_FD);		
		return(XDC_NOK);		
	}
	else
	{
		sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_Modif_SGBD_Equipements : %s",pl_Msg);
		cuti_BilanConf_Trace("********************************************************");
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
	}
	/*A
	** Verification des Åquipement UGTP (Temps De Parcours)
	*/
	if ((vl_retcode = xzao408_Lire_Config_Eqt_TDP (pl_FD, cmvf_FctUti_MSE_XZAO407_408  ,&pl_NumeroLigne))!=XDC_OK)
	{
		if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
		else sprintf(pl_Msg,"Erreur detectée dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
	 	XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb : xzao408_Lire_Config_Eqt_TDP () a echoue");
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
	** Ouverture du fichier de configuration des Åquipement ZDP (Zone De Parcours)
	*/
	sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_EQUI_TRAV, XDF_CONF_EQT_ZDP );	
	if (!(pl_FD=fopen(pl_NomFile,"r")))
	{
		/*B 
		** En cas de prob d ouverture du fichier de conf, on logue le prob a l administrateur
		** et on le notifie dans le fichier Bilan Config
		*/
		sprintf(pl_Msg,"Erreur d'ouverture du fichier %s\n",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_Modif_SGBD_Equipements : \n%s ",pl_Msg);		
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* On previent l IHM de l administrateur */
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
		fclose(pl_FD);		
		return(XDC_NOK);		
	}
	else
	{
		sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_Modif_SGBD_Equipements : %s",pl_Msg);
		cuti_BilanConf_Trace("********************************************************");
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
	}
	/*A
	** Verification des Åquipement ZDP (Zone De Parcours)
	*/
	if ((vl_retcode = xzao404_Lire_Config_Eqt_ZDP (pl_FD, cmvf_FctUti_MSE_XZAO403_404  ,&pl_NumeroLigne))!=XDC_OK)
	{
		if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
		else sprintf(pl_Msg,"Erreur detectée dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
	 	XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb : xzao404_Lire_Config_Eqt_ZDP () a echoue");
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
	** Ouverture du fichier de configuration des  TypNAV
	*/
	sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_EQUI_TRAV,XDF_CONF_TYPE_NAV );	
	if (!(pl_FD=fopen(pl_NomFile,"r")))
	{
		/*B 
		** En cas de prob d ouverture du fichier de conf, on logue le prob a l administrateur
		** et on le notifie dans le fichier Bilan Config
		*/
		sprintf(pl_Msg,"Erreur d'ouverture du fichier %s\n",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_Modif_SGBD_Equipements : \n%s ",pl_Msg);		
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* On previent l IHM de l administrateur */
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
		fclose(pl_FD);		
		return(XDC_NOK);		
	}
	else
	{
		sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_Modif_SGBD_Equipements : %s",pl_Msg);
		cuti_BilanConf_Trace("********************************************************");
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
	}
	/*A
	** Verification  TypNAV
	*/
	if ((vl_retcode =  xzao156_Lire_Config_Type_NAV  (pl_FD,cmvf_FctUti_MSE_XZAO156_157,&pl_NumeroLigne))!=XDC_OK)
	{
		if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
		else sprintf(pl_Msg,"Erreur detectée dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
	 	XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb :  xzao156_Lire_Config_Type_NAV  () a echoue");
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
	
	/* ABE	27-04-2015 : projet suppression ancienne GTC (GTCA)	*/	
//	/*A
//	** Ouverture du fichier de configuration de la  GTC Nice 
//	*/
//	sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_EQUI_TRAV,XDF_CONF_GTC_NICE  );	
//	if (!(pl_FD=fopen(pl_NomFile,"r")))
//	{
//		/*B 
//		** En cas de prob d ouverture du fichier de conf, on logue le prob a l administrateur
//		** et on le notifie dans le fichier Bilan Config
//		*/
//		sprintf(pl_Msg,"Erreur d'ouverture du fichier %s\n",pl_NomFile);
//		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_Modif_SGBD_Equipements : \n%s ",pl_Msg);		
//		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* On previent l IHM de l administrateur */
//		cuti_BilanConf_Trace(pl_Msg); 
//	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
//		fclose(pl_FD);		
//		return(XDC_NOK);		
//	}
//	else
//	{
//		sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
//		XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_Modif_SGBD_Equipements : %s",pl_Msg);
//		cuti_BilanConf_Trace("********************************************************");
//		cuti_BilanConf_Trace(pl_Msg); 
//	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
//	}
//	/*A
//	** Verification   GTC Nice
//	*/
//	if ((vl_retcode =  xzao165_Lire_Config_GTC_Nice   (pl_FD,cmvf_FctUti_MSE_XZAO165_166,&pl_NumeroLigne))!=XDC_OK)
//	{
//		if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
//		else sprintf(pl_Msg,"Erreur detectée dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
//		cuti_BilanConf_Trace(pl_Msg); 
//	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
//	 	XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb :  xzao165_Lire_Config_GTC_Nice   () a echoue");
//		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cvce_VerifModif_cb a echoue : Sortie");
//		fclose(pl_FD);
//		return(XDC_NOK);
//	}
//	else
//	{
//		sprintf(pl_Msg,"Traitement de %s OK.",pl_NomFile);
//		cuti_BilanConf_Trace(pl_Msg); 
//	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);	   /* Notification dans Bilan config */
//		fclose(pl_FD);
//	}
	
	/* ABE	27-04-2015 : projet suppression ancienne GTC (GTCA)	*/
//	/*A
//	** Ouverture du fichier de configuration des sequences tubes  
//	*/
//	sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_EQUI_TRAV,XDF_CONF_SEQ_TUBE  );	
//	if (!(pl_FD=fopen(pl_NomFile,"r")))
//	{
//		/*B 
//		** En cas de prob d ouverture du fichier de conf, on logue le prob a l administrateur
//		** et on le notifie dans le fichier Bilan Config
//		*/
//		sprintf(pl_Msg,"Erreur d'ouverture du fichier %s\n",pl_NomFile);
//		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_Modif_SGBD_Equipements : \n%s ",pl_Msg);		
//		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* On previent l IHM de l administrateur */
//		cuti_BilanConf_Trace(pl_Msg); 
//	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
//		fclose(pl_FD);		
//		return(XDC_NOK);		
//	}
//	else
//	{
//		sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
//		XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_Modif_SGBD_Equipements : %s",pl_Msg);
//		cuti_BilanConf_Trace("********************************************************");
//		cuti_BilanConf_Trace(pl_Msg); 
//	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
//	}
//	/*A
//	** Verification des sequences tubes   
//	*/
//	if ((vl_retcode =   xzao167_Lire_Config_Sequences_Tube   (pl_FD,cmvf_FctUti_MSE_XZAO167_168,&pl_NumeroLigne))!=XDC_OK)
//	{
//		if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
//		else sprintf(pl_Msg,"Erreur detectée dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
//		cuti_BilanConf_Trace(pl_Msg); 
//	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
//	 	XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb :  xzao167_Lire_Config_Sequences_Tube    () a echoue");
//		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cvce_VerifModif_cb a echoue : Sortie");
//		fclose(pl_FD);
//		return(XDC_NOK);
//	}
//	else
//	{
//		sprintf(pl_Msg,"Traitement de %s OK.",pl_NomFile);
//		cuti_BilanConf_Trace(pl_Msg); 
//	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);	   /* Notification dans Bilan config */
//		fclose(pl_FD);
//	}

	/* ABE	27-04-2015 : projet suppression ancienne GTC (GTCA)	*/
//	/*A
//	** Ouverture du fichier de configuration des sequences  echangeur 
//	*/
//	sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_EQUI_TRAV,XDF_CONF_SEQ_ECHANGEUR  );	
//	if (!(pl_FD=fopen(pl_NomFile,"r")))
//	{
//		/*B 
//		** En cas de prob d ouverture du fichier de conf, on logue le prob a l administrateur
//		** et on le notifie dans le fichier Bilan Config
//		*/
//		sprintf(pl_Msg,"Erreur d'ouverture du fichier %s\n",pl_NomFile);
//		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_Modif_SGBD_Equipements : \n%s ",pl_Msg);		
//		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* On previent l IHM de l administrateur */
//		cuti_BilanConf_Trace(pl_Msg); 
//	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
//		fclose(pl_FD);		
//		return(XDC_NOK);		
//	}
//	else
//	{
//		sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
//		XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_Modif_SGBD_Equipements : %s",pl_Msg);
//		cuti_BilanConf_Trace("********************************************************");
//		cuti_BilanConf_Trace(pl_Msg); 
//	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
//	}
//	/*A
//	** Verification des sequences echangeur   
//	*/
//	if ((vl_retcode =  xzao173_Lire_Config_Sequences_Ech   (pl_FD,cmvf_FctUti_MSE_XZAO173_174,&pl_NumeroLigne))!=XDC_OK)
//	{
//		if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
//		else sprintf(pl_Msg,"Erreur detectée dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
//		cuti_BilanConf_Trace(pl_Msg); 
//	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
//	 	XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb : xzao173_Lire_Config_Sequences_Ech () a echoue");
//		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cvce_VerifModif_cb a echoue : Sortie");
//		fclose(pl_FD);
//		return(XDC_NOK);
//	}
//	else
//	{
//		sprintf(pl_Msg,"Traitement de %s OK.",pl_NomFile);
//		cuti_BilanConf_Trace(pl_Msg); 
//	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);	   /* Notification dans Bilan config */
//		fclose(pl_FD);
//	}
	/*A
	** Ouverture du fichier de configuration des chaussees
	*/
	sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_EQUI_TRAV,XDF_CONF_CHAUSSEE  );	
	if (!(pl_FD=fopen(pl_NomFile,"r")))
	{
		/*B 
		** En cas de prob d ouverture du fichier de conf, on logue le prob a l administrateur
		** et on le notifie dans le fichier Bilan Config
		*/
		sprintf(pl_Msg,"Erreur d'ouverture du fichier %s\n",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_Modif_SGBD_Equipements : \n%s ",pl_Msg);		
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* On previent l IHM de l administrateur */
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
		fclose(pl_FD);		
		return(XDC_NOK);		
	}
	else
	{
		sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_Modif_SGBD_Equipements : %s",pl_Msg);
		cuti_BilanConf_Trace("********************************************************");
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
	}
	/*A
	** Verification des chaussees   
	*/
	if ((vl_retcode =  xzao99_Lire_Config_Chaussee  (pl_FD,cmvf_FctUti_MSE_XZAO99_68,&pl_NumeroLigne))!=XDC_OK)
	{
		if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
		else sprintf(pl_Msg,"Erreur detectée dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
	 	XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb : xzao99_Lire_Config_Chaussee () a echoue");
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
	** Ouverture du fichier de configuration des analyseurs
	*/
	sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_EQUI_TRAV,XDF_CONF_ANALYSEUR  );	
	if (!(pl_FD=fopen(pl_NomFile,"r")))
	{
		/*B 
		** En cas de prob d ouverture du fichier de conf, on logue le prob a l administrateur
		** et on le notifie dans le fichier Bilan Config
		*/
		sprintf(pl_Msg,"Erreur d'ouverture du fichier %s\n",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_Modif_SGBD_Equipements : \n%s ",pl_Msg);		
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* On previent l IHM de l administrateur */
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
		fclose(pl_FD);		
		return(XDC_NOK);		
	}
	else
	{
		sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_Modif_SGBD_Equipements : %s",pl_Msg);
		cuti_BilanConf_Trace("********************************************************");
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
	}
	/*A
	** Verification des analyseurs  
	*/
	if ((vl_retcode =   xzao106_Lire_Config_Analyseur (pl_FD,cmvf_FctUti_MSE_XZAO106_81,&pl_NumeroLigne))!=XDC_OK)
	{
		if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
		else sprintf(pl_Msg,"Erreur detectée dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
	 	XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb : xzao106_Lire_Config_Analyseur () a echoue");
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
	** Ouverture du fichier de configuration des LT Video
	*/
	sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_EQUI_TRAV,XDF_CONF_LTVIDEO  );	
	if (!(pl_FD=fopen(pl_NomFile,"r")))
	{
		/*B 
		** En cas de prob d ouverture du fichier de conf, on logue le prob a l administrateur
		** et on le notifie dans le fichier Bilan Config
		*/
		sprintf(pl_Msg,"Erreur d'ouverture du fichier %s\n",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_Modif_SGBD_Equipements : \n%s ",pl_Msg);		
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* On previent l IHM de l administrateur */
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
		fclose(pl_FD);		
		return(XDC_NOK);		
	}
	else
	{
		sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_Modif_SGBD_Equipements : %s",pl_Msg);
		cuti_BilanConf_Trace("********************************************************");
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
	}
	/*A
	** Verification des LT Video 
	*/
	if ((vl_retcode =  xzao108_Lire_Config_LTVideo  (pl_FD, cmvf_FctUti_MSE_XZAO108_138 ,&pl_NumeroLigne))!=XDC_OK)
	{
		if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
		else sprintf(pl_Msg,"Erreur detectée dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
	 	XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb : xzao108_Lire_Config_LTVideo () a echoue");
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
	}/*A
	** Ouverture du fichier de configuration des moniteurs
	*/
	sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_EQUI_TRAV,XDF_CONF_MONITEUR  );	
	if (!(pl_FD=fopen(pl_NomFile,"r")))
	{
		/*B 
		** En cas de prob d ouverture du fichier de conf, on logue le prob a l administrateur
		** et on le notifie dans le fichier Bilan Config
		*/
		sprintf(pl_Msg,"Erreur d'ouverture du fichier %s\n",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_Modif_SGBD_Equipements : \n%s ",pl_Msg);		
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* On previent l IHM de l administrateur */
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
		fclose(pl_FD);		
		return(XDC_NOK);		
	}
	else
	{
		sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_Modif_SGBD_Equipements : %s",pl_Msg);
		cuti_BilanConf_Trace("********************************************************");
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
	}
	/*A
	** Verification des moniteurs 
	*/
	if ((vl_retcode =  xzao110_Lire_Config_Moniteur  (pl_FD, cmvf_FctUti_MSE_XZAO110_140 ,&pl_NumeroLigne))!=XDC_OK)
	{
		if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
		else sprintf(pl_Msg,"Erreur detectée dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
	 	XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb : xzao110_Lire_Config_Moniteur () a echoue");
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
	** Ouverture du fichier de configuration des magentoscopes
	*/
	sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_EQUI_TRAV,XDF_CONF_MAGNETOSCOPE  );	
	if (!(pl_FD=fopen(pl_NomFile,"r")))
	{
		/*B 
		** En cas de prob d ouverture du fichier de conf, on logue le prob a l administrateur
		** et on le notifie dans le fichier Bilan Config
		*/
		sprintf(pl_Msg,"Erreur d'ouverture du fichier %s\n",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_Modif_SGBD_Equipements : \n%s ",pl_Msg);		
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* On previent l IHM de l administrateur */
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
		fclose(pl_FD);		
		return(XDC_NOK);		
	}
	else
	{
		sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_Modif_SGBD_Equipements : %s",pl_Msg);
		cuti_BilanConf_Trace("********************************************************");
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
	}
	/*A
	** Verification des  magentoscopes
	*/
	if ((vl_retcode = xzao111_Lire_Config_Magnetoscope (pl_FD, cmvf_FctUti_MSE_XZAO111_141 ,&pl_NumeroLigne))!=XDC_OK)
	{
		if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
		else sprintf(pl_Msg,"Erreur detectée dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
	 	XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb : xzao111_Lire_Config_Magnetoscope () a echoue");
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
	** Ouverture du fichier de configuration des PAU
	*/
	sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_EQUI_TRAV,XDF_CONF_PAU );	
	if (!(pl_FD=fopen(pl_NomFile,"r")))
	{
		/*B 
		** En cas de prob d ouverture du fichier de conf, on logue le prob a l administrateur
		** et on le notifie dans le fichier Bilan Config
		*/
		sprintf(pl_Msg,"Erreur d'ouverture du fichier %s\n",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_Modif_SGBD_Equipements : \n%s ",pl_Msg);		
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* On previent l IHM de l administrateur */
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
		fclose(pl_FD);		
		return(XDC_NOK);		
	}
	else
	{
		sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_Modif_SGBD_Equipements : %s",pl_Msg);
		cuti_BilanConf_Trace("********************************************************");
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
	}
	/*A
	** Verification des PAU
	*/
	if ((vl_retcode = xzao114_Lire_Config_PAU (pl_FD, cmvf_FctUti_MSE_XZAO114_144 ,&pl_NumeroLigne))!=XDC_OK)
	{
		if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
		else sprintf(pl_Msg,"Erreur detectée dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
	 	XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb : xzao114_Lire_Config_PAU () a echoue");
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
	** Ouverture du fichier de scenari
	*/
	sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_EQUI_TRAV,XDF_CONF_SCENARIO );	
	if (!(pl_FD=fopen(pl_NomFile,"r")))
	{
		/*B 
		** En cas de prob d ouverture du fichier de conf, on logue le prob a l administrateur
		** et on le notifie dans le fichier Bilan Config
		*/
		sprintf(pl_Msg,"Erreur d'ouverture du fichier %s\n",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_Modif_SGBD_Type : \n%s ",pl_Msg);		
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* On previent l IHM de l administrateur */
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
		fclose(pl_FD);		
		return(XDC_NOK);		
	}
	else
	{
		sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_Modif_SGBD_Type : %s",pl_Msg);
		cuti_BilanConf_Trace("********************************************************");
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
	}
	/*A
	** Verification scenari
	*/
	if ((vl_retcode = xzao154_Lire_Config_Scenarios (pl_FD,cmvf_FctUti_MST_XZAO154_155,&pl_NumeroLigne))!=XDC_OK)
	{
		if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
		else sprintf(pl_Msg,"Erreur detectée dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
	 	XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb : xzao154_Lire_Config_Scenarios () a echoue");
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
	** Ouverture du fichier de configuration des scenari connus
	*/
	sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_EQUI_TRAV,XDF_CONF_SCENARIO_CONNU );	
	if (!(pl_FD=fopen(pl_NomFile,"r")))
	{
		/*B 
		** En cas de prob d ouverture du fichier de conf, on logue le prob a l administrateur
		** et on le notifie dans le fichier Bilan Config
		*/
		sprintf(pl_Msg,"Erreur d'ouverture du fichier %s\n",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_Modif_SGBD_Equipements : \n%s ",pl_Msg);		
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* On previent l IHM de l administrateur */
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
		fclose(pl_FD);		
		return(XDC_NOK);		
	}
	else
	{
		sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_Modif_SGBD_Equipements : %s",pl_Msg);
		cuti_BilanConf_Trace("********************************************************");
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
	}
	/*A
	** Verification des scenari connus
	*/
	if ((vl_retcode = xzao158_Lire_Config_Scen_Connus (pl_FD, cmvf_FctUti_MSE_XZAO158_159 ,&pl_NumeroLigne))!=XDC_OK)
	{
		if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
		else sprintf(pl_Msg,"Erreur detectée dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
	 	XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb : xzao158_Lire_Config_Scen_Connus () a echoue");
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
	** Ouverture du fichier de configuration des demi-echangeurs
	*/
	sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_EQUI_TRAV,XDF_CONF_DEMIECHANGEUR );	
	if (!(pl_FD=fopen(pl_NomFile,"r")))
	{
		/*B 
		** En cas de prob d ouverture du fichier de conf, on logue le prob a l administrateur
		** et on le notifie dans le fichier Bilan Config
		*/
		sprintf(pl_Msg,"Erreur d'ouverture du fichier %s\n",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_Modif_SGBD_Equipements : \n%s ",pl_Msg);		
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* On previent l IHM de l administrateur */
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
		fclose(pl_FD);		
		return(XDC_NOK);		
	}
	else
	{
		sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_Modif_SGBD_Equipements : %s",pl_Msg);
		cuti_BilanConf_Trace("********************************************************");
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
	}
	/*A
	** Verification des demi-echangeurs
	*/
	if ((vl_retcode = xzao175_Lire_Config_Demi_Ech (pl_FD, cmvf_FctUti_MSE_XZAO175_176 ,&pl_NumeroLigne))!=XDC_OK)
	{
		if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
		else sprintf(pl_Msg,"Erreur detectée dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
	 	XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb : xzao175_Lire_Config_Demi_Ech () a echoue");
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
	** Ouverture du fichier de configuration des tubes
	*/
	sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_EQUI_TRAV,XDF_CONF_TUBE );	
	if (!(pl_FD=fopen(pl_NomFile,"r")))
	{
		/*B 
		** En cas de prob d ouverture du fichier de conf, on logue le prob a l administrateur
		** et on le notifie dans le fichier Bilan Config
		*/
		sprintf(pl_Msg,"Erreur d'ouverture du fichier %s\n",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_Modif_SGBD_Equipements : \n%s ",pl_Msg);		
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* On previent l IHM de l administrateur */
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
		fclose(pl_FD);		
		return(XDC_NOK);		
	}
	else
	{
		sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_Modif_SGBD_Equipements : %s",pl_Msg);
		cuti_BilanConf_Trace("********************************************************");
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
	}
	
	/*A
	** Verification des tubes
	*/
	if ((vl_retcode = xzao177_Lire_Config_Tube (pl_FD, cmvf_FctUti_MSE_XZAO177_178 ,&pl_NumeroLigne))!=XDC_OK)
	{
		if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
		else sprintf(pl_Msg,"Erreur detectée dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
	 	XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb : xzao177_Lire_Config_Tube () a echoue");
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

    /* AAZ 15/11/2007 nouvelle configuration tube et echangeur */

	/*A
	** Ouverture du fichier de configuration des tubes (idem ancienne GTC)
	*/
	sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_EQUI_TRAV,XDF_CONF_TUBE );	
	if (!(pl_FD=fopen(pl_NomFile,"r")))
	{
		/*B 
		** En cas de prob d ouverture du fichier de conf, on logue le prob a l administrateur
		** et on le notifie dans le fichier Bilan Config
		*/
		sprintf(pl_Msg,"Erreur d'ouverture du fichier %s\n",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_Modif_SGBD_Equipements : \n%s ",pl_Msg);		
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* On previent l IHM de l administrateur */
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
		fclose(pl_FD);		
		return(XDC_NOK);		
	}
	else
	{
		sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_Modif_SGBD_Equipements : %s",pl_Msg);
		cuti_BilanConf_Trace("********************************************************");
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
	}
	
	/*A
	** Verification des tubes
	*/
	if ((vl_retcode = xzao977_Lire_Config_Tube_Renove (pl_FD, cmvf_FctUti_MSE_XZAO977_978 ,&pl_NumeroLigne))!=XDC_OK)
	{
		if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
		else sprintf(pl_Msg,"Erreur detectée dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
	 	XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb : xzao977_Lire_Config_Tube_Renove () a echoue");
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
	** Ouverture du fichier de configuration des demi-echangeurs (idem gtc ancienne)
	*/
	sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_EQUI_TRAV,XDF_CONF_DEMIECHANGEUR );	
	if (!(pl_FD=fopen(pl_NomFile,"r")))
	{
		/*B 
		** En cas de prob d ouverture du fichier de conf, on logue le prob a l administrateur
		** et on le notifie dans le fichier Bilan Config
		*/
		sprintf(pl_Msg,"Erreur d'ouverture du fichier %s\n",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_Modif_SGBD_Equipements : \n%s ",pl_Msg);		
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* On previent l IHM de l administrateur */
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
		fclose(pl_FD);		
		return(XDC_NOK);		
	}
	else
	{
		sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_Modif_SGBD_Equipements : %s",pl_Msg);
		cuti_BilanConf_Trace("********************************************************");
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
	}
	/*A
	** Verification des demi-echangeurs
	*/
	if ((vl_retcode = xzao979_Lire_Config_Demi_Ech_Renove (pl_FD, cmvf_FctUti_MSE_XZAO979_980 ,&pl_NumeroLigne))!=XDC_OK)
	{
		if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
		else sprintf(pl_Msg,"Erreur detectée dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
	 	XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb : xzao979_Lire_Config_Demi_Ech_Renove () a echoue");
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
	** Ouverture du fichier de configuration des ouvrages systeme
	*/
	sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_EQUI_TRAV,XDF_CONF_OUV_SYS );	
	if (!(pl_FD=fopen(pl_NomFile,"r")))
	{
		/*B 
		** En cas de prob d ouverture du fichier de conf, on logue le prob a l administrateur
		** et on le notifie dans le fichier Bilan Config
		*/
		sprintf(pl_Msg,"Erreur d'ouverture du fichier %s\n",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_Modif_SGBD_Equipements : \n%s ",pl_Msg);		
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* On previent l IHM de l administrateur */
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
		fclose(pl_FD);		
		return(XDC_NOK);		
	}
	else
	{
		sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_Modif_SGBD_Equipements : %s",pl_Msg);
		cuti_BilanConf_Trace("********************************************************");
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
	}
	/*A
	** Verification des ouvrages systeme
	*/
	if ((vl_retcode = xzao1007_Lire_Config_Sys (pl_FD,  cmvf_FctUti_MSE_XZAO1007_1008 ,&pl_NumeroLigne))!=XDC_OK)
	{
		if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
		else sprintf(pl_Msg,"Erreur detectée dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
	 	XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb : xzao1007_Lire_Config_Sys () a echoue");
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
	** Ouverture du fichier de configuration des EqtGTC
	*/
	/*****************************/
	/* SIGNALISATION             */
	/*****************************/
	sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_EQUI_TRAV,XDF_CONF_EQT_SIG );	
	if (!(pl_FD=fopen(pl_NomFile,"r")))
	{
		/*B 
		** En cas de prob d ouverture du fichier de conf, on logue le prob a l administrateur
		** et on le notifie dans le fichier Bilan Config
		*/
		sprintf(pl_Msg,"Erreur d'ouverture du fichier %s\n",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_Modif_SGBD_Equipements : \n%s ",pl_Msg);		
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* On previent l IHM de l administrateur */
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
		fclose(pl_FD);		
		return(XDC_NOK);		
	}
	else
	{
		sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_Modif_SGBD_Equipements : %s",pl_Msg);
		cuti_BilanConf_Trace("********************************************************");
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
	}
	/*A
	** Verification des EqtGTC
	*/
	if ((vl_retcode = xzao991_Lire_Config_Equipement (pl_FD,  cmvf_FctUti_MSE_XZAO991_992 ,&pl_NumeroLigne))!=XDC_OK)
	{
		if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
		else sprintf(pl_Msg,"Erreur detectée dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
	 	XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb : xzao991_Lire_Config_Equipement () a echoue");
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

	/*****************************/
	/* VENTILATION               */
	/*****************************/
	sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_EQUI_TRAV,XDF_CONF_EQT_VEN );	
	if (!(pl_FD=fopen(pl_NomFile,"r")))
	{
		/*B 
		** En cas de prob d ouverture du fichier de conf, on logue le prob a l administrateur
		** et on le notifie dans le fichier Bilan Config
		*/
		sprintf(pl_Msg,"Erreur d'ouverture du fichier %s\n",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_Modif_SGBD_Equipements : \n%s ",pl_Msg);		
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* On previent l IHM de l administrateur */
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
		fclose(pl_FD);		
		return(XDC_NOK);		
	}
	else
	{
		sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_Modif_SGBD_Equipements : %s",pl_Msg);
		cuti_BilanConf_Trace("********************************************************");
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
	}
	/*A
	** Verification des EqtGTC
	*/
	if ((vl_retcode = xzao991_Lire_Config_Equipement (pl_FD,  cmvf_FctUti_MSE_XZAO991_992 ,&pl_NumeroLigne))!=XDC_OK)
	{
		if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
		else sprintf(pl_Msg,"Erreur detectée dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
	 	XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb : xzao991_Lire_Config_Equipement () a echoue");
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
	
	/*****************************/
	/* ECLAIRAGE                 */
	/*****************************/
	sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_EQUI_TRAV,XDF_CONF_EQT_ECL );	
	if (!(pl_FD=fopen(pl_NomFile,"r")))
	{
		/*B 
		** En cas de prob d ouverture du fichier de conf, on logue le prob a l administrateur
		** et on le notifie dans le fichier Bilan Config
		*/
		sprintf(pl_Msg,"Erreur d'ouverture du fichier %s\n",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_Modif_SGBD_Equipements : \n%s ",pl_Msg);		
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* On previent l IHM de l administrateur */
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
		fclose(pl_FD);		
		return(XDC_NOK);		
	}
	else
	{
		sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_Modif_SGBD_Equipements : %s",pl_Msg);
		cuti_BilanConf_Trace("********************************************************");
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
	}
	/*A
	** Verification des EqtGTC
	*/
	if ((vl_retcode = xzao991_Lire_Config_Equipement (pl_FD,  cmvf_FctUti_MSE_XZAO991_992 ,&pl_NumeroLigne))!=XDC_OK)
	{
		if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
		else sprintf(pl_Msg,"Erreur detectée dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
	 	XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb : xzao991_Lire_Config_Equipement () a echoue");
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

	/*****************************/
	/* ENERGIE                   */
	/*****************************/
	sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_EQUI_TRAV,XDF_CONF_EQT_ENR );	
	if (!(pl_FD=fopen(pl_NomFile,"r")))
	{
		/*B 
		** En cas de prob d ouverture du fichier de conf, on logue le prob a l administrateur
		** et on le notifie dans le fichier Bilan Config
		*/
		sprintf(pl_Msg,"Erreur d'ouverture du fichier %s\n",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_Modif_SGBD_Equipements : \n%s ",pl_Msg);		
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* On previent l IHM de l administrateur */
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
		fclose(pl_FD);		
		return(XDC_NOK);		
	}
	else
	{
		sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_Modif_SGBD_Equipements : %s",pl_Msg);
		cuti_BilanConf_Trace("********************************************************");
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
	}
	/*A
	** Verification des EqtGTC
	*/
	if ((vl_retcode = xzao991_Lire_Config_Equipement (pl_FD,  cmvf_FctUti_MSE_XZAO991_992 ,&pl_NumeroLigne))!=XDC_OK)
	{
		if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
		else sprintf(pl_Msg,"Erreur detectée dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
	 	XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb : xzao991_Lire_Config_Equipement () a echoue");
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
	
	/*****************************/
	/* EXPLOITATION              */
	/*****************************/
	sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_EQUI_TRAV,XDF_CONF_EQT_EXP );	
	if (!(pl_FD=fopen(pl_NomFile,"r")))
	{
		/*B 
		** En cas de prob d ouverture du fichier de conf, on logue le prob a l administrateur
		** et on le notifie dans le fichier Bilan Config
		*/
		sprintf(pl_Msg,"Erreur d'ouverture du fichier %s\n",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_Modif_SGBD_Equipements : \n%s ",pl_Msg);		
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* On previent l IHM de l administrateur */
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
		fclose(pl_FD);		
		return(XDC_NOK);		
	}
	else
	{
		sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_Modif_SGBD_Equipements : %s",pl_Msg);
		cuti_BilanConf_Trace("********************************************************");
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
	}
	/*A
	** Verification des EqtGTC
	*/
	if ((vl_retcode = xzao991_Lire_Config_Equipement (pl_FD,  cmvf_FctUti_MSE_XZAO991_992 ,&pl_NumeroLigne))!=XDC_OK)
	{
		if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
		else sprintf(pl_Msg,"Erreur detectée dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
	 	XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb : xzao991_Lire_Config_Equipement () a echoue");
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
	
	/*****************************/
	/* INCENDIE                  */
	/*****************************/
	sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_EQUI_TRAV,XDF_CONF_EQT_INC );	
	if (!(pl_FD=fopen(pl_NomFile,"r")))
	{
		/*B 
		** En cas de prob d ouverture du fichier de conf, on logue le prob a l administrateur
		** et on le notifie dans le fichier Bilan Config
		*/
		sprintf(pl_Msg,"Erreur d'ouverture du fichier %s\n",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_Modif_SGBD_Equipements : \n%s ",pl_Msg);		
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* On previent l IHM de l administrateur */
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
		fclose(pl_FD);		
		return(XDC_NOK);		
	}
	else
	{
		sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_Modif_SGBD_Equipements : %s",pl_Msg);
		cuti_BilanConf_Trace("********************************************************");
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
	}
	/*A
	** Verification des EqtGTC
	*/
	if ((vl_retcode = xzao991_Lire_Config_Equipement (pl_FD,  cmvf_FctUti_MSE_XZAO991_992 ,&pl_NumeroLigne))!=XDC_OK)
	{
		if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
		else sprintf(pl_Msg,"Erreur detectée dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
	 	XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb : xzao991_Lire_Config_Equipement () a echoue");
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
	
	/*****************************/
	/* LTN                       */
	/*****************************/
	sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_EQUI_TRAV,XDF_CONF_EQT_LTN );	
	if (!(pl_FD=fopen(pl_NomFile,"r")))
	{
		/*B 
		** En cas de prob d ouverture du fichier de conf, on logue le prob a l administrateur
		** et on le notifie dans le fichier Bilan Config
		*/
		sprintf(pl_Msg,"Erreur d'ouverture du fichier %s\n",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_Modif_SGBD_Equipements : \n%s ",pl_Msg);		
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* On previent l IHM de l administrateur */
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
		fclose(pl_FD);		
		return(XDC_NOK);		
	}
	else
	{
		sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_Modif_SGBD_Equipements : %s",pl_Msg);
		cuti_BilanConf_Trace("********************************************************");
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
	}
	/*A
	** Verification des EqtGTC
	*/
	if ((vl_retcode = xzao991_Lire_Config_Equipement (pl_FD,  cmvf_FctUti_MSE_XZAO991_992 ,&pl_NumeroLigne))!=XDC_OK)
	{
		if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
		else sprintf(pl_Msg,"Erreur detectée dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
	 	XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb : xzao991_Lire_Config_Equipement () a echoue");
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
	
	/*****************************/
	/* SYSTEME                   */
	/*****************************/
	sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_EQUI_TRAV,XDF_CONF_EQT_SYS );	
	if (!(pl_FD=fopen(pl_NomFile,"r")))
	{
		/*B 
		** En cas de prob d ouverture du fichier de conf, on logue le prob a l administrateur
		** et on le notifie dans le fichier Bilan Config
		*/
		sprintf(pl_Msg,"Erreur d'ouverture du fichier %s\n",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_Modif_SGBD_Equipements : \n%s ",pl_Msg);		
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* On previent l IHM de l administrateur */
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
		fclose(pl_FD);		
		return(XDC_NOK);		
	}
	else
	{
		sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_Modif_SGBD_Equipements : %s",pl_Msg);
		cuti_BilanConf_Trace("********************************************************");
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
	}
	/*A
	** Verification des EqtGTC
	*/
	if ((vl_retcode = xzao991_Lire_Config_Equipement (pl_FD,  cmvf_FctUti_MSE_XZAO991_992 ,&pl_NumeroLigne))!=XDC_OK)
	{
		if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
		else sprintf(pl_Msg,"Erreur detectée dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
	 	XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb : xzao991_Lire_Config_Equipement () a echoue");
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
	
 /* Sequences autorisees du domaine SIG */
    
    /*A
    ** Ouverture du fichier de configuration des sequences autorisees du domaine SIG
    */
    sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_EQUI_TRAV,XDF_CONF_SEQ_AUT_SIG );    
    if (!(pl_FD=fopen(pl_NomFile,"r")))
    {
        /*B 
        ** En cas de prob d ouverture du fichier de conf, on logue le prob a l administrateur
        ** et on le notifie dans le fichier Bilan Config
        */
        sprintf( pl_Msg, "Erreur d'ouverture du fichier %s\n", pl_NomFile );
        XZST_03EcritureTrace( XZSTC_WARNING, "TCGCD : cmvf_Modif_SGBD_Equipements : \n%s ", pl_Msg);        
        XZIA08_AffMsgAdm( CGCDV_NOMMACHINE, pl_Msg ); /* On previent l IHM de l administrateur */
        cuti_BilanConf_Trace( pl_Msg ); 
        XZIA08_AffMsgAdm( CGCDV_NOMMACHINE, pl_Msg ); /* Notification dans Bilan config*/
        fclose( pl_FD );        
        return( XDC_NOK );        
    }
    else
    {
        sprintf( pl_Msg, "Traitement du fichier %s en cours", pl_NomFile );
        XZST_03EcritureTrace( XZSTC_DEBUG1, "cmvf_Modif_SGBD_Equipements : %s", pl_Msg);
        cuti_BilanConf_Trace( "********************************************************" );
        cuti_BilanConf_Trace( pl_Msg ); 
         XZIA08_AffMsgAdm( CGCDV_NOMMACHINE, pl_Msg ); /* Notification dans Bilan config*/
    }

    /*A
    ** Verification des sequences autorisees domaine SIG
    */
    if ((vl_retcode = xzao981_Lire_Config_SeqAut_SIG (pl_FD, cmvf_FctUti_MSE_XZAO981_982  ,&pl_NumeroLigne))!=XDC_OK)
    {
        if (pl_NumeroLigne == 0) 
            sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
        else
            sprintf(pl_Msg,"Erreur detectée dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
        cuti_BilanConf_Trace( pl_Msg ); 
        XZIA08_AffMsgAdm( CGCDV_NOMMACHINE, pl_Msg );
        XZST_03EcritureTrace( XZSTC_WARNING , "TCGCD : cvce_VerifModif_cb : xzao981_Lire_Config_SeqAut_SIG () a echoue" );
        XZST_03EcritureTrace( XZSTC_FONCTION, "OUT : cvce_VerifModif_cb a echoue : Sortie" );
        fclose( pl_FD );
        return( XDC_NOK );
    }
    else
    {
        sprintf( pl_Msg, "Traitement de %s OK.", pl_NomFile );
        cuti_BilanConf_Trace( pl_Msg ); 
        XZIA08_AffMsgAdm( CGCDV_NOMMACHINE, pl_Msg );       /* Notification dans Bilan config */
        fclose( pl_FD );
    }

    /*A
    ** Ouverture du fichier de configuration des sequences de retour a la normal du domaine SIG
    */
    sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_EQUI_TRAV,XDF_CONF_SEQ_RET_SIG );    
    if (!(pl_FD=fopen(pl_NomFile,"r")))
    {
        /*B 
        ** En cas de prob d ouverture du fichier de conf, on logue le prob a l administrateur
        ** et on le notifie dans le fichier Bilan Config
        */
        sprintf( pl_Msg, "Erreur d'ouverture du fichier %s\n", pl_NomFile );
        XZST_03EcritureTrace( XZSTC_WARNING, "TCGCD : cmvf_Modif_SGBD_Equipements : \n%s ", pl_Msg);        
        XZIA08_AffMsgAdm( CGCDV_NOMMACHINE, pl_Msg ); /* On previent l IHM de l administrateur */
        cuti_BilanConf_Trace( pl_Msg ); 
        XZIA08_AffMsgAdm( CGCDV_NOMMACHINE, pl_Msg ); /* Notification dans Bilan config*/
        fclose( pl_FD );        
        return( XDC_NOK );        
    }
    else
    {
        sprintf( pl_Msg, "Traitement du fichier %s en cours", pl_NomFile );
        XZST_03EcritureTrace( XZSTC_DEBUG1, "cmvf_Modif_SGBD_Equipements : %s", pl_Msg);
        cuti_BilanConf_Trace( "********************************************************" );
        cuti_BilanConf_Trace( pl_Msg ); 
         XZIA08_AffMsgAdm( CGCDV_NOMMACHINE, pl_Msg ); /* Notification dans Bilan config*/
    }

    /*A
    ** Verification des sequences de retour a la normal du domaine SIG
    */
    if ((vl_retcode = xzao891_Lire_Config_SeqRet_SIG (pl_FD, cmvf_FctUti_MSE_XZAO891_892  ,&pl_NumeroLigne))!=XDC_OK)
    {
        if (pl_NumeroLigne == 0) 
            sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
        else
            sprintf(pl_Msg,"Erreur detectée dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
        cuti_BilanConf_Trace( pl_Msg ); 
        XZIA08_AffMsgAdm( CGCDV_NOMMACHINE, pl_Msg );
        XZST_03EcritureTrace( XZSTC_WARNING , "TCGCD : cvce_VerifModif_cb : xzao891_Lire_Config_SeqRet_SIG () a echoue" );
        XZST_03EcritureTrace( XZSTC_FONCTION, "OUT : cvce_VerifModif_cb a echoue : Sortie" );
        fclose( pl_FD );
        return( XDC_NOK );
    }
    else
    {
        sprintf( pl_Msg, "Traitement de %s OK.", pl_NomFile );
        cuti_BilanConf_Trace( pl_Msg ); 
        XZIA08_AffMsgAdm( CGCDV_NOMMACHINE, pl_Msg );       /* Notification dans Bilan config */
        fclose( pl_FD );
    }

	/* Sequences  du domaine EXP */
    
    /*A
    ** Ouverture du fichier de configuration des sequences  du domaine EXP
    */
    sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_EQUI_TRAV,XDF_CONF_SEQ_EXP );    
    if (!(pl_FD=fopen(pl_NomFile,"r")))
    {
        /*B 
        ** En cas de prob d ouverture du fichier de conf, on logue le prob a l administrateur
        ** et on le notifie dans le fichier Bilan Config
        */
        sprintf( pl_Msg, "Erreur d'ouverture du fichier %s\n", pl_NomFile );
        XZST_03EcritureTrace( XZSTC_WARNING, "TCGCD : cmvf_Modif_SGBD_Equipements : \n%s ", pl_Msg);        
        XZIA08_AffMsgAdm( CGCDV_NOMMACHINE, pl_Msg ); /* On previent l IHM de l administrateur */
        cuti_BilanConf_Trace( pl_Msg ); 
        XZIA08_AffMsgAdm( CGCDV_NOMMACHINE, pl_Msg ); /* Notification dans Bilan config*/
        fclose( pl_FD );        
        return( XDC_NOK );        
    }
    else
    {
        sprintf( pl_Msg, "Traitement du fichier %s en cours", pl_NomFile );
        XZST_03EcritureTrace( XZSTC_DEBUG1, "cmvf_Modif_SGBD_Equipements : %s", pl_Msg);
        cuti_BilanConf_Trace( "********************************************************" );
        cuti_BilanConf_Trace( pl_Msg ); 
         XZIA08_AffMsgAdm( CGCDV_NOMMACHINE, pl_Msg ); /* Notification dans Bilan config*/
    }


    /*A
    ** Verification des sequences  domaine EXP
    */
    if ((vl_retcode = xzao1011_Lire_Config_Seq_EXP (pl_FD, cmvf_FctUti_MSE_XZAO1011_1012  ,&pl_NumeroLigne))!=XDC_OK)
    {
        if (pl_NumeroLigne == 0) 
            sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
        else
            sprintf(pl_Msg,"Erreur detectée dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
        cuti_BilanConf_Trace( pl_Msg ); 
        XZIA08_AffMsgAdm( CGCDV_NOMMACHINE, pl_Msg );
        XZST_03EcritureTrace( XZSTC_WARNING , "TCGCD : cvce_VerifModif_cb : xzao1001_Lire_Config_Seq_INC () a echoue" );
        XZST_03EcritureTrace( XZSTC_FONCTION, "OUT : cvce_VerifModif_cb a echoue : Sortie" );
        fclose( pl_FD );
        return( XDC_NOK );
    }
    else
    {
        sprintf( pl_Msg, "Traitement de %s OK.", pl_NomFile );
        cuti_BilanConf_Trace( pl_Msg ); 
        XZIA08_AffMsgAdm( CGCDV_NOMMACHINE, pl_Msg );       /* Notification dans Bilan config */
        fclose( pl_FD );
    }
    
    /* Sequences autorisees du domaine ECL */
    
    /*A
    ** Ouverture du fichier de configuration des sequences autorisees du domaine ECL
    */
    sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_EQUI_TRAV,XDF_CONF_SEQ_AUT_ECL );    
    if (!(pl_FD=fopen(pl_NomFile,"r")))
    {
        /*B 
        ** En cas de prob d ouverture du fichier de conf, on logue le prob a l administrateur
        ** et on le notifie dans le fichier Bilan Config
        */
        sprintf( pl_Msg, "Erreur d'ouverture du fichier %s\n", pl_NomFile );
        XZST_03EcritureTrace( XZSTC_WARNING, "TCGCD : cmvf_Modif_SGBD_Equipements : \n%s ", pl_Msg);        
        XZIA08_AffMsgAdm( CGCDV_NOMMACHINE, pl_Msg ); /* On previent l IHM de l administrateur */
        cuti_BilanConf_Trace( pl_Msg ); 
        XZIA08_AffMsgAdm( CGCDV_NOMMACHINE, pl_Msg ); /* Notification dans Bilan config*/
        fclose( pl_FD );        
        return( XDC_NOK );        
    }
    else
    {
        sprintf( pl_Msg, "Traitement du fichier %s en cours", pl_NomFile );
        XZST_03EcritureTrace( XZSTC_DEBUG1, "cmvf_Modif_SGBD_Equipements : %s", pl_Msg);
        cuti_BilanConf_Trace( "********************************************************" );
        cuti_BilanConf_Trace( pl_Msg ); 
         XZIA08_AffMsgAdm( CGCDV_NOMMACHINE, pl_Msg ); /* Notification dans Bilan config*/
    }



    /*A
    ** Verification des sequences autorisees domaine ECL
    */
    if ((vl_retcode = xzao983_Lire_Config_SeqAut_ECL (pl_FD, cmvf_FctUti_MSE_XZAO983_984  ,&pl_NumeroLigne))!=XDC_OK)
    {
        if (pl_NumeroLigne == 0) 
            sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
        else
            sprintf(pl_Msg,"Erreur detectée dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
        cuti_BilanConf_Trace( pl_Msg ); 
        XZIA08_AffMsgAdm( CGCDV_NOMMACHINE, pl_Msg );
        XZST_03EcritureTrace( XZSTC_WARNING , "TCGCD : cvce_VerifModif_cb : xzao983_Lire_Config_SeqAut_ECL () a echoue" );
        XZST_03EcritureTrace( XZSTC_FONCTION, "OUT : cvce_VerifModif_cb a echoue : Sortie" );
        fclose( pl_FD );
        return( XDC_NOK );
    }
    else
    {
        sprintf( pl_Msg, "Traitement de %s OK.", pl_NomFile );
        cuti_BilanConf_Trace( pl_Msg ); 
        XZIA08_AffMsgAdm( CGCDV_NOMMACHINE, pl_Msg );       /* Notification dans Bilan config */
        fclose( pl_FD );
    }

    
    /* Sequences autorisees du domaine VEN */
    
    /*A
    ** Ouverture du fichier de configuration des sequences autorisees du domaine VEN
    */
    sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_EQUI_TRAV,XDF_CONF_SEQ_AUT_VEN );    
    if (!(pl_FD=fopen(pl_NomFile,"r")))
    {
        /*B 
        ** En cas de prob d ouverture du fichier de conf, on logue le prob a l administrateur
        ** et on le notifie dans le fichier Bilan Config
        */
        sprintf( pl_Msg, "Erreur d'ouverture du fichier %s\n", pl_NomFile );
        XZST_03EcritureTrace( XZSTC_WARNING, "TCGCD : cmvf_Modif_SGBD_Equipements : \n%s ", pl_Msg);        
        XZIA08_AffMsgAdm( CGCDV_NOMMACHINE, pl_Msg ); /* On previent l IHM de l administrateur */
        cuti_BilanConf_Trace( pl_Msg ); 
        XZIA08_AffMsgAdm( CGCDV_NOMMACHINE, pl_Msg ); /* Notification dans Bilan config*/
        fclose( pl_FD );        
        return( XDC_NOK );        
    }
    else
    {
        sprintf( pl_Msg, "Traitement du fichier %s en cours", pl_NomFile );
        XZST_03EcritureTrace( XZSTC_DEBUG1, "cmvf_Modif_SGBD_Equipements : %s", pl_Msg);
        cuti_BilanConf_Trace( "********************************************************" );
        cuti_BilanConf_Trace( pl_Msg ); 
         XZIA08_AffMsgAdm( CGCDV_NOMMACHINE, pl_Msg ); /* Notification dans Bilan config*/
    }



    /*A
    ** Verification des sequences autorisees domaine VEN
    */
    if ((vl_retcode = xzao985_Lire_Config_SeqAut_VEN (pl_FD, cmvf_FctUti_MSE_XZAO985_986  ,&pl_NumeroLigne))!=XDC_OK)
    {
        if (pl_NumeroLigne == 0) 
            sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
        else
            sprintf(pl_Msg,"Erreur detectée dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
        cuti_BilanConf_Trace( pl_Msg ); 
        XZIA08_AffMsgAdm( CGCDV_NOMMACHINE, pl_Msg );
        XZST_03EcritureTrace( XZSTC_WARNING , "TCGCD : cvce_VerifModif_cb : xzao985_Lire_Config_SeqAut_VEN () a echoue" );
        XZST_03EcritureTrace( XZSTC_FONCTION, "OUT : cvce_VerifModif_cb a echoue : Sortie" );
        fclose( pl_FD );
        return( XDC_NOK );
    }
    else
    {
        sprintf( pl_Msg, "Traitement de %s OK.", pl_NomFile );
        cuti_BilanConf_Trace( pl_Msg ); 
        XZIA08_AffMsgAdm( CGCDV_NOMMACHINE, pl_Msg );       /* Notification dans Bilan config */
        fclose( pl_FD );
    }

    
    /* Sequences autorisees du domaine ENR */
    
    /*A
    ** Ouverture du fichier de configuration des sequences autorisees du domaine ENR
    */
    sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_EQUI_TRAV,XDF_CONF_SEQ_AUT_ENR );    
    if (!(pl_FD=fopen(pl_NomFile,"r")))
    {
        /*B 
        ** En cas de prob d ouverture du fichier de conf, on logue le prob a l administrateur
        ** et on le notifie dans le fichier Bilan Config
        */
        sprintf( pl_Msg, "Erreur d'ouverture du fichier %s\n", pl_NomFile );
        XZST_03EcritureTrace( XZSTC_WARNING, "TCGCD : cmvf_Modif_SGBD_Equipements : \n%s ", pl_Msg);        
        XZIA08_AffMsgAdm( CGCDV_NOMMACHINE, pl_Msg ); /* On previent l IHM de l administrateur */
        cuti_BilanConf_Trace( pl_Msg ); 
        XZIA08_AffMsgAdm( CGCDV_NOMMACHINE, pl_Msg ); /* Notification dans Bilan config*/
        fclose( pl_FD );        
        return( XDC_NOK );        
    }
    else
    {
        sprintf( pl_Msg, "Traitement du fichier %s en cours", pl_NomFile );
        XZST_03EcritureTrace( XZSTC_DEBUG1, "cmvf_Modif_SGBD_Equipements : %s", pl_Msg);
        cuti_BilanConf_Trace( "********************************************************" );
        cuti_BilanConf_Trace( pl_Msg ); 
         XZIA08_AffMsgAdm( CGCDV_NOMMACHINE, pl_Msg ); /* Notification dans Bilan config*/
    }



    /*A
    ** Verification des sequences autorisees domaine ENR
    */
    if ((vl_retcode = xzao987_Lire_Config_SeqAut_ENR (pl_FD, cmvf_FctUti_MSE_XZAO987_988  ,&pl_NumeroLigne))!=XDC_OK)
    {
        if (pl_NumeroLigne == 0) 
            sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
        else
            sprintf(pl_Msg,"Erreur detectée dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
        cuti_BilanConf_Trace( pl_Msg ); 
        XZIA08_AffMsgAdm( CGCDV_NOMMACHINE, pl_Msg );
        XZST_03EcritureTrace( XZSTC_WARNING , "TCGCD : cvce_VerifModif_cb : xzao987_Lire_Config_SeqAut_ENR () a echoue" );
        XZST_03EcritureTrace( XZSTC_FONCTION, "OUT : cvce_VerifModif_cb a echoue : Sortie" );
        fclose( pl_FD );
        return( XDC_NOK );
    }
    else
    {
        sprintf( pl_Msg, "Traitement de %s OK.", pl_NomFile );
        cuti_BilanConf_Trace( pl_Msg ); 
        XZIA08_AffMsgAdm( CGCDV_NOMMACHINE, pl_Msg );       /* Notification dans Bilan config */
        fclose( pl_FD );
    }

    
    /* Sequences autorisees du domaine INC */
    
    /*A
    ** Ouverture du fichier de configuration des sequences autorisees du domaine INC
    */
    sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_EQUI_TRAV,XDF_CONF_SEQ_AUT_INC );    
    if (!(pl_FD=fopen(pl_NomFile,"r")))
    {
        /*B 
        ** En cas de prob d ouverture du fichier de conf, on logue le prob a l administrateur
        ** et on le notifie dans le fichier Bilan Config
        */
        sprintf( pl_Msg, "Erreur d'ouverture du fichier %s\n", pl_NomFile );
        XZST_03EcritureTrace( XZSTC_WARNING, "TCGCD : cmvf_Modif_SGBD_Equipements : \n%s ", pl_Msg);        
        XZIA08_AffMsgAdm( CGCDV_NOMMACHINE, pl_Msg ); /* On previent l IHM de l administrateur */
        cuti_BilanConf_Trace( pl_Msg ); 
        XZIA08_AffMsgAdm( CGCDV_NOMMACHINE, pl_Msg ); /* Notification dans Bilan config*/
        fclose( pl_FD );        
        return( XDC_NOK );        
    }
    else
    {
        sprintf( pl_Msg, "Traitement du fichier %s en cours", pl_NomFile );
        XZST_03EcritureTrace( XZSTC_DEBUG1, "cmvf_Modif_SGBD_Equipements : %s", pl_Msg);
        cuti_BilanConf_Trace( "********************************************************" );
        cuti_BilanConf_Trace( pl_Msg ); 
         XZIA08_AffMsgAdm( CGCDV_NOMMACHINE, pl_Msg ); /* Notification dans Bilan config*/
    }



    /*A
    ** Verification des sequences autorisees domaine INC
    */
    if ((vl_retcode = xzao989_Lire_Config_SeqAut_INC (pl_FD, cmvf_FctUti_MSE_XZAO989_990  ,&pl_NumeroLigne))!=XDC_OK)
    {
        if (pl_NumeroLigne == 0) 
            sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
        else
            sprintf(pl_Msg,"Erreur detectée dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
        cuti_BilanConf_Trace( pl_Msg ); 
        XZIA08_AffMsgAdm( CGCDV_NOMMACHINE, pl_Msg );
        XZST_03EcritureTrace( XZSTC_WARNING , "TCGCD : cvce_VerifModif_cb : xzao989_Lire_Config_SeqAut_INC () a echoue" );
        XZST_03EcritureTrace( XZSTC_FONCTION, "OUT : cvce_VerifModif_cb a echoue : Sortie" );
        fclose( pl_FD );
        return( XDC_NOK );
    }
    else
    {
        sprintf( pl_Msg, "Traitement de %s OK.", pl_NomFile );
        cuti_BilanConf_Trace( pl_Msg ); 
        XZIA08_AffMsgAdm( CGCDV_NOMMACHINE, pl_Msg );       /* Notification dans Bilan config */
        fclose( pl_FD );
    }
    
    /* Sequences autorisees du domaine EXP */
    
    /*A
    ** Ouverture du fichier de configuration des sequences autorisees du domaine EXP
    */
    sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_EQUI_TRAV,XDF_CONF_SEQ_AUT_EXP );    
    if (!(pl_FD=fopen(pl_NomFile,"r")))
    {
        /*B 
        ** En cas de prob d ouverture du fichier de conf, on logue le prob a l administrateur
        ** et on le notifie dans le fichier Bilan Config
        */
        sprintf( pl_Msg, "Erreur d'ouverture du fichier %s\n", pl_NomFile );
        XZST_03EcritureTrace( XZSTC_WARNING, "TCGCD : cmvf_Modif_SGBD_Equipements : \n%s ", pl_Msg);        
        XZIA08_AffMsgAdm( CGCDV_NOMMACHINE, pl_Msg ); /* On previent l IHM de l administrateur */
        cuti_BilanConf_Trace( pl_Msg ); 
        XZIA08_AffMsgAdm( CGCDV_NOMMACHINE, pl_Msg ); /* Notification dans Bilan config*/
        fclose( pl_FD );        
        return( XDC_NOK );        
    }
    else
    {
        sprintf( pl_Msg, "Traitement du fichier %s en cours", pl_NomFile );
        XZST_03EcritureTrace( XZSTC_DEBUG1, "cmvf_Modif_SGBD_Equipements : %s", pl_Msg);
        cuti_BilanConf_Trace( "********************************************************" );
        cuti_BilanConf_Trace( pl_Msg ); 
         XZIA08_AffMsgAdm( CGCDV_NOMMACHINE, pl_Msg ); /* Notification dans Bilan config*/
    }



    /*A
    ** Verification des sequences autorisees domaine EXP
    */
    if ((vl_retcode = xzao1013_Lire_Config_SeqAut_EXP (pl_FD, cmvf_FctUti_MSE_XZAO1013_1014  ,&pl_NumeroLigne))!=XDC_OK)
    {
        if (pl_NumeroLigne == 0) 
            sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
        else
            sprintf(pl_Msg,"Erreur detectée dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
        cuti_BilanConf_Trace( pl_Msg ); 
        XZIA08_AffMsgAdm( CGCDV_NOMMACHINE, pl_Msg );
        XZST_03EcritureTrace( XZSTC_WARNING , "TCGCD : cvce_VerifModif_cb : xzao981_Lire_Config_SeqAut_SIG () a echoue" );
        XZST_03EcritureTrace( XZSTC_FONCTION, "OUT : cvce_VerifModif_cb a echoue : Sortie" );
        fclose( pl_FD );
        return( XDC_NOK );
    }
    else
    {
        sprintf( pl_Msg, "Traitement de %s OK.", pl_NomFile );
        cuti_BilanConf_Trace( pl_Msg ); 
        XZIA08_AffMsgAdm( CGCDV_NOMMACHINE, pl_Msg );       /* Notification dans Bilan config */
        fclose( pl_FD );
    }	
	
    /* AAZ */

    /* AAZ */

    /* AAZ 26/09/2007 */
	/*A
	** Ouverture du fichier de configuration des equipements du domaine signalisation
	*/
	/*
	sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_EQUI_TRAV,XDF_CONF_SIG );	
	printf("AZA %s/%s",CLIVC_LIVR_EQUI_TRAV,XDF_CONF_SIG );	
	if (!(pl_FD=fopen(pl_NomFile,"r")))
	{
		//B 
		// En cas de prob d ouverture du fichier de conf, on logue le prob a l administrateur
		// et on le notifie dans le fichier Bilan Config
		//
		sprintf(pl_Msg,"Erreur d'ouverture du fichier %s\n",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_Modif_SGBD_Equipements : \n%s ",pl_Msg);		
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); // On previent l IHM de l administrateur
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); // Notification dans Bilan config
		fclose(pl_FD);		
		return(XDC_NOK);		
	}
	else
	{
		sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_Modif_SGBD_Equipements : %s",pl_Msg);
		cuti_BilanConf_Trace("********************************************************");
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); // Notification dans Bilan config
	}
	//A
	// Verification des equipements du domaine signalisation
	//
	if ((vl_retcode = xzao640_Lire_Config_SIG (pl_FD, cmvf_FctUti_MSE_XZAO640_600 ,&pl_NumeroLigne))!=XDC_OK)
	{
		if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
		else sprintf(pl_Msg,"Erreur detectée dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
	 	XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb : xzao640_Lire_Config_SIG () a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cvce_VerifModif_cb a echoue : Sortie");
		fclose(pl_FD);
		return(XDC_NOK);
	}
	else
	{
		sprintf(pl_Msg,"Traitement de %s OK.",pl_NomFile);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);	   // Notification dans Bilan config
		fclose(pl_FD);
	}
    */
	/*A
	** Ouverture du fichier de configuration des equipements du domaine eclairage
	*/
	/*
	sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_EQUI_TRAV,XDF_CONF_ECL );	
	if (!(pl_FD=fopen(pl_NomFile,"r")))
	{
		//B 
		// En cas de prob d ouverture du fichier de conf, on logue le prob a l administrateur
		// et on le notifie dans le fichier Bilan Config
		//
		sprintf(pl_Msg,"Erreur d'ouverture du fichier %s\n",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_Modif_SGBD_Equipements : \n%s ",pl_Msg);		
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); // On previent l IHM de l administrateur
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); // Notification dans Bilan config
		fclose(pl_FD);		
		return(XDC_NOK);		
	}
	else
	{
		sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_Modif_SGBD_Equipements : %s",pl_Msg);
		cuti_BilanConf_Trace("********************************************************");
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); // Notification dans Bilan config
	}
	//A
	// Verification des equipements du domaine eclairage
	//
	if ((vl_retcode = xzao641_Lire_Config_ECL (pl_FD, cmvf_FctUti_MSE_XZAO641_601 ,&pl_NumeroLigne))!=XDC_OK)
	{
		if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
		else sprintf(pl_Msg,"Erreur detectée dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
	 	XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb : xzao641_Lire_Config_ECL () a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cvce_VerifModif_cb a echoue : Sortie");
		fclose(pl_FD);
		return(XDC_NOK);
	}
	else
	{
		sprintf(pl_Msg,"Traitement de %s OK.",pl_NomFile);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);	   // Notification dans Bilan config
		fclose(pl_FD);
	}
	*/

	/*A
	** Ouverture du fichier de configuration des equipements du domaine ventilation
	*/
	/*
	sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_EQUI_TRAV,XDF_CONF_VEN );	
	if (!(pl_FD=fopen(pl_NomFile,"r")))
	{
		//B 
		// En cas de prob d ouverture du fichier de conf, on logue le prob a l administrateur
		// et on le notifie dans le fichier Bilan Config
		//
		sprintf(pl_Msg,"Erreur d'ouverture du fichier %s\n",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_Modif_SGBD_Equipements : \n%s ",pl_Msg);		
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); // On previent l IHM de l administrateur
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); // Notification dans Bilan config
		fclose(pl_FD);		
		return(XDC_NOK);		
	}
	else
	{
		sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_Modif_SGBD_Equipements : %s",pl_Msg);
		cuti_BilanConf_Trace("********************************************************");
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); // Notification dans Bilan config
	}
	//A
	// Verification des equipements du domaine ventilation
	//
	if ((vl_retcode = xzao642_Lire_Config_VEN (pl_FD, cmvf_FctUti_MSE_XZAO642_602 ,&pl_NumeroLigne))!=XDC_OK)
	{
		if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
		else sprintf(pl_Msg,"Erreur detectée dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
	 	XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb : xzao642_Lire_Config_VEN () a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cvce_VerifModif_cb a echoue : Sortie");
		fclose(pl_FD);
		return(XDC_NOK);
	}
	else
	{
		sprintf(pl_Msg,"Traitement de %s OK.",pl_NomFile);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);	   // Notification dans Bilan config
		fclose(pl_FD);
	}
	*/
	/*A
	** Ouverture du fichier de configuration des equipements du domaine energie
	*/
    /*
	sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_EQUI_TRAV,XDF_CONF_ENR );	
	if (!(pl_FD=fopen(pl_NomFile,"r")))
	{
		//B 
		// En cas de prob d ouverture du fichier de conf, on logue le prob a l administrateur
		// et on le notifie dans le fichier Bilan Config
		//
		sprintf(pl_Msg,"Erreur d'ouverture du fichier %s\n",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_Modif_SGBD_Equipements : \n%s ",pl_Msg);		
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); // On previent l IHM de l administrateur
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); // Notification dans Bilan config
		fclose(pl_FD);		
		return(XDC_NOK);		
	}
	else
	{
		sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_Modif_SGBD_Equipements : %s",pl_Msg);
		cuti_BilanConf_Trace("********************************************************");
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); // Notification dans Bilan config
	}
	//A
	// Verification des equipements du domaine energie
	//
	if ((vl_retcode = xzao643_Lire_Config_ENR (pl_FD, cmvf_FctUti_MSE_XZAO643_603 ,&pl_NumeroLigne))!=XDC_OK)
	{
		if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
		else sprintf(pl_Msg,"Erreur detectée dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
	 	XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb : xzao643_Lire_Config_ENR () a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cvce_VerifModif_cb a echoue : Sortie");
		fclose(pl_FD);
		return(XDC_NOK);
	}
	else
	{
		sprintf(pl_Msg,"Traitement de %s OK.",pl_NomFile);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);	   // Notification dans Bilan config
		fclose(pl_FD);
	}
	*/
	/*A
	** Ouverture du fichier de configuration des equipements du domaine protection incendie
	*/
	/*
	sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_EQUI_TRAV,XDF_CONF_INC );	
	if (!(pl_FD=fopen(pl_NomFile,"r")))
	{
		//B 
		// En cas de prob d ouverture du fichier de conf, on logue le prob a l administrateur
		// et on le notifie dans le fichier Bilan Config
		//
		sprintf(pl_Msg,"Erreur d'ouverture du fichier %s\n",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_Modif_SGBD_Equipements : \n%s ",pl_Msg);		
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); // On previent l IHM de l administrateur
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); // Notification dans Bilan config
		fclose(pl_FD);		
		return(XDC_NOK);		
	}
	else
	{
		sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_Modif_SGBD_Equipements : %s",pl_Msg);
		cuti_BilanConf_Trace("********************************************************");
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); // Notification dans Bilan config
	}
	//A
	// Verification des equipements du domaine protection incendie
	//
	if ((vl_retcode = xzao644_Lire_Config_INC (pl_FD, cmvf_FctUti_MSE_XZAO644_604 ,&pl_NumeroLigne))!=XDC_OK)
	{
		if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
		else sprintf(pl_Msg,"Erreur detectée dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
	 	XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb : xzao644_Lire_Config_INC () a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cvce_VerifModif_cb a echoue : Sortie");
		fclose(pl_FD);
		return(XDC_NOK);
	}
	else
	{
		sprintf(pl_Msg,"Traitement de %s OK.",pl_NomFile);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);	   // Notification dans Bilan config
		fclose(pl_FD);
	}
	*/

	/*A
	** Ouverture du fichier de configuration des equipements du domaine exploitation
	*/
	/*
	sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_EQUI_TRAV,XDF_CONF_EXP );	
	if (!(pl_FD=fopen(pl_NomFile,"r")))
	{
		//B 
		// En cas de prob d ouverture du fichier de conf, on logue le prob a l administrateur
		// et on le notifie dans le fichier Bilan Config
		//
		sprintf(pl_Msg,"Erreur d'ouverture du fichier %s\n",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_Modif_SGBD_Equipements : \n%s ",pl_Msg);		
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); // On previent l IHM de l administrateur
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); // Notification dans Bilan config
		fclose(pl_FD);		
		return(XDC_NOK);		
	}
	else
	{
		sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_Modif_SGBD_Equipements : %s",pl_Msg);
		cuti_BilanConf_Trace("********************************************************");
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); // Notification dans Bilan config
	}
	//A
	// Verification des equipements du domaine exploitation
	//
	if ((vl_retcode = xzao645_Lire_Config_EXP (pl_FD, cmvf_FctUti_MSE_XZAO645_605 ,&pl_NumeroLigne))!=XDC_OK)
	{
		if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
		else sprintf(pl_Msg,"Erreur detectée dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
	 	XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb : xzao645_Lire_Config_EXP () a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cvce_VerifModif_cb a echoue : Sortie");
		fclose(pl_FD);
		return(XDC_NOK);
	}
	else
	{
		sprintf(pl_Msg,"Traitement de %s OK.",pl_NomFile);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);	   // Notification dans Bilan config
		fclose(pl_FD);
	}
	*/

	/*A
	** Ouverture du fichier de configuration des equipements du domaine systeme
	*/
	/*
	sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_EQUI_TRAV,XDF_CONF_SYS );	
	if (!(pl_FD=fopen(pl_NomFile,"r")))
	{
		//B 
		// En cas de prob d ouverture du fichier de conf, on logue le prob a l administrateur
		// et on le notifie dans le fichier Bilan Config
		//
		sprintf(pl_Msg,"Erreur d'ouverture du fichier %s\n",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_Modif_SGBD_Equipements : \n%s ",pl_Msg);		
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); // On previent l IHM de l administrateur
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); // Notification dans Bilan config
		fclose(pl_FD);		
		return(XDC_NOK);		
	}
	else
	{
		sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_Modif_SGBD_Equipements : %s",pl_Msg);
		cuti_BilanConf_Trace("********************************************************");
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); // Notification dans Bilan config
	}

	//A
	// Verification des equipements du domaine systeme
	//
	if ((vl_retcode = xzao646_Lire_Config_SYS (pl_FD, cmvf_FctUti_MSE_XZAO646_606 ,&pl_NumeroLigne))!=XDC_OK)
	{
		if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
		else sprintf(pl_Msg,"Erreur detectée dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
	 	XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb : xzao646_Lire_Config_SYS () a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cvce_VerifModif_cb a echoue : Sortie");
		fclose(pl_FD);
		return(XDC_NOK);
	}
	else
	{
		sprintf(pl_Msg,"Traitement de %s OK.",pl_NomFile);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);	   // Notification dans Bilan config
		fclose(pl_FD);
	}
	*/
	/*A
	** Ouverture du fichier de configuration des equipements du domaine locaux techniques
	*/
	/*
	sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_EQUI_TRAV,XDF_CONF_LTN );	
	if (!(pl_FD=fopen(pl_NomFile,"r")))
	{
		//B 
		// En cas de prob d ouverture du fichier de conf, on logue le prob a l administrateur
		// et on le notifie dans le fichier Bilan Config
		//
		sprintf(pl_Msg,"Erreur d'ouverture du fichier %s\n",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_Modif_SGBD_Equipements : \n%s ",pl_Msg);		
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); // On previent l IHM de l administrateur
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); // Notification dans Bilan config
		fclose(pl_FD);		
		return(XDC_NOK);		
	}
	else
	{
		sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_Modif_SGBD_Equipements : %s",pl_Msg);
		cuti_BilanConf_Trace("********************************************************");
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); // Notification dans Bilan config
	}
	//A
	// Verification des equipements du domaine locaux techniques
	//
	if ((vl_retcode = xzao647_Lire_Config_LTN (pl_FD, cmvf_FctUti_MSE_XZAO647_607 ,&pl_NumeroLigne))!=XDC_OK)
	{
		if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
		else sprintf(pl_Msg,"Erreur detectée dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
	 	XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb : xzao647_Lire_Config_LTN () a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cvce_VerifModif_cb a echoue : Sortie");
		fclose(pl_FD);
		return(XDC_NOK);
	}
	else
	{
		sprintf(pl_Msg,"Traitement de %s OK.",pl_NomFile);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);	   // Notification dans Bilan config
		fclose(pl_FD);
	}
	*/

    /* AAZ */

        /*A
        ** Ouverture du fichier de configuration des zones d'influence TUNNEL
        */
        sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_EQUI_TRAV,XDF_CONF_ZON_TUN);
        if (!(pl_FD=fopen(pl_NomFile,"r")))
        {
                /*B
                ** En cas de prob d ouverture du fichier de conf, on logue le prob a l administrateur
                ** et on le notifie dans le fichier Bilan Config);
                */
                sprintf(pl_Msg,"Erreur d'ouverture du fichier %s\n",pl_NomFile);
                XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_Modif_SGBD_Equipements : \n%s ",pl_Msg);
                XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* On previent l IHM de l administrateur */
                cuti_BilanConf_Trace(pl_Msg);
                XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
                fclose(pl_FD);
                return(XDC_NOK);
        }
        else
        {
                sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
                XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_Modif_SGBD_Equipements : %s",pl_Msg);
                cuti_BilanConf_Trace("********************************************************");
                cuti_BilanConf_Trace(pl_Msg);
                XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
        }

        /*A
        ** Verification des zones d'influence PMV
        */
	if ((vl_retcode = xzao352_Lire_Config_Zon_TUN (pl_FD, cmvf_FctUti_MSE_XZAO352_351  ,&pl_NumeroLigne))!=XDC_OK)
	{
                if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
                else sprintf(pl_Msg,"Erreur detectée dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
		cuti_BilanConf_Trace(pl_Msg);
                XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
                XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb : xzao352_Lire_Config_Zon_TUN () a echoue");
                XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cvce_VerifModif_cb a echoue : Sortie");
                fclose(pl_FD);
                return(XDC_NOK);
        }
	else
        {
                sprintf(pl_Msg,"Traitement de %s OK.",pl_NomFile);
                cuti_BilanConf_Trace(pl_Msg);
                XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);         /* Notification dans Bilan config */
                fclose(pl_FD);
        }

        /*A
        ** Ouverture du fichier de configuration des zones urbaines
        */
        sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_EQUI_TRAV,XDF_CONF_ZON_URB);
        if (!(pl_FD=fopen(pl_NomFile,"r")))
        {
                /*B
                ** En cas de prob d ouverture du fichier de conf, on logue le prob a l administrateur
                ** et on le notifie dans le fichier Bilan Config);
                */
                sprintf(pl_Msg,"Erreur d'ouverture du fichier %s\n",pl_NomFile);
                XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_Modif_SGBD_Equipements : \n%s ",pl_Msg);
                XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* On previent l IHM de l administrateur */
                cuti_BilanConf_Trace(pl_Msg);
                XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
                fclose(pl_FD);
                return(XDC_NOK);
        }
        else
        {
                sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
                XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_Modif_SGBD_Equipements : %s",pl_Msg);
                cuti_BilanConf_Trace("********************************************************");
                cuti_BilanConf_Trace(pl_Msg);
                XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
        }
        /*A
        ** Verification des zones urbaines
        */
	if ((vl_retcode = xzao952_Lire_Config_ZoneUrbaine (pl_FD, cmvf_FctUti_MSE_XZAO953_952  ,&pl_NumeroLigne))!=XDC_OK)
	{
                if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
                else sprintf(pl_Msg,"Erreur detectée dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
		cuti_BilanConf_Trace(pl_Msg);
                XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
                XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb : xzao952_Lire_Config_ZoneUrbaine () a echoue");
                XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cvce_VerifModif_cb a echoue : Sortie");
                fclose(pl_FD);
                return(XDC_NOK);
        }
	else
        {
                sprintf(pl_Msg,"Traitement de %s OK.",pl_NomFile);
                cuti_BilanConf_Trace(pl_Msg);
                XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);         /* Notification dans Bilan config */
                fclose(pl_FD);
	}


	/*A
	** Ouverture du fichier de configuration des points de mesures
	*/
	sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_EQUI_TRAV,XDF_CONF_PT_MESURE );	
	if (!(pl_FD=fopen(pl_NomFile,"r")))
	{
		/*B 
		** En cas de prob d ouverture du fichier de conf, on logue le prob a l administrateur
		** et on le notifie dans le fichier Bilan Config
		*/
		sprintf(pl_Msg,"Erreur d'ouverture du fichier %s\n",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_Modif_SGBD_Equipements : \n%s ",pl_Msg);		
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* On previent l IHM de l administrateur */
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
		fclose(pl_FD);		
		return(XDC_NOK);		
	}
	else
	{
		sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_Modif_SGBD_Equipements : %s",pl_Msg);
		cuti_BilanConf_Trace("********************************************************");
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
	}
	/*A
	** Verification des points de mesures
	*/
	if ((vl_retcode = xzao100_Lire_Config_Pt_Mesure (pl_FD, cmvf_FctUti_MSE_XZAO100_69 ,&pl_NumeroLigne))!=XDC_OK)
	{
		if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
		else sprintf(pl_Msg,"Erreur detectée dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
	 	XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb : xzao100_Lire_Config_Pt_Mesure () a echoue");
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
	** Ouverture du fichier de configuration des cameras
	*/
	sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_EQUI_TRAV,XDF_CONF_CAMERA );	
	if (!(pl_FD=fopen(pl_NomFile,"r")))
	{
		/*B 
		** En cas de prob d ouverture du fichier de conf, on logue le prob a l administrateur
		** et on le notifie dans le fichier Bilan Config
		*/
		sprintf(pl_Msg,"Erreur d'ouverture du fichier %s",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_Modif_SGBD_Equipements : %s\n",pl_Msg);		
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* On previent l IHM de l administrateur */
		cuti_BilanConf_Trace(pl_Msg); /* Notification dans Bilan config*/
		fclose(pl_FD);		
		return(XDC_NOK);		
	}
	else
	{
		sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_Modif_SGBD_Equipements : %s",pl_Msg);
		cuti_BilanConf_Trace("********************************************************");
		cuti_BilanConf_Trace(pl_Msg); /* Notification dans Bilan config*/
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
	}
	/*A
	** Verification des cameras
	*/
	if ((vl_retcode = xzao109_Lire_Config_Camera (pl_FD, cmvf_FctUti_MSE_XZAO109_139 ,&pl_NumeroLigne))!=XDC_OK)
	{
		if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
		else sprintf(pl_Msg,"Erreur detectée dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
	 	XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb : xzao109_Lire_Config_Camera () a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cvce_VerifModif_cb a echoue : Sortie");
		fclose(pl_FD);
		return(XDC_NOK);
	}
	else
	{
		sprintf(pl_Msg,"Traitement de %s OK.",pl_NomFile);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);	   /* On previent l IHM de l administrateur */
		fclose(pl_FD);
	}
	/*A
	** Ouverture du fichier de configuration des zones de couverture des cameras
	*/
	sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_EQUI_TRAV,XDF_CONF_ZON_CAM);
	if (!(pl_FD=fopen(pl_NomFile,"r")))
	{
		/*B 
		** En cas de prob d ouverture du fichier de conf, on logue le prob a l administrateur
		** et on le notifie dans le fichier Bilan Config
		*/
		sprintf(pl_Msg,"Erreur d'ouverture du fichier %s",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_Modif_SGBD_Equipements : %s\n",pl_Msg);		
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* On previent l IHM de l administrateur */
		cuti_BilanConf_Trace(pl_Msg); /* Notification dans Bilan config*/
		fclose(pl_FD);		
		return(XDC_NOK);		
	}
	else
	{
		sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_Modif_SGBD_Equipements : %s",pl_Msg);
		cuti_BilanConf_Trace("********************************************************");
		cuti_BilanConf_Trace(pl_Msg); /* Notification dans Bilan config*/
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* On previent l IHM de l administrateur */
	}
	/*A
	** Verification des zones de couverture des cameras
	*/
	if ((vl_retcode = xzao192_Lire_Config_Zon_Camera (pl_FD, cmvf_FctUti_MSE_XZAO192_193 ,&pl_NumeroLigne))!=XDC_OK)
	{
		if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
		else sprintf(pl_Msg,"Erreur detectée dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
	 	XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb : xzao192_Lire_Config_Zon_Camera () a echoue");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cvce_VerifModif_cb a echoue : Sortie");
		fclose(pl_FD);
		return(XDC_NOK);
	}
	else
	{
		sprintf(pl_Msg,"Traitement de %s OK.",pl_NomFile);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);	   /* On previent l IHM de l administrateur */
		fclose(pl_FD);
	}
	/*A
	** Ouverture du fichier de configuration des NAV
	*/
	sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_EQUI_TRAV,XDF_CONF_NAV );	
	if (!(pl_FD=fopen(pl_NomFile,"r")))
	{
		/*B 
		** En cas de prob d ouverture du fichier de conf, on logue le prob a l administrateur
		** et on le notifie dans le fichier Bilan Config
		*/
		sprintf(pl_Msg,"Erreur d'ouverture du fichier %s\n",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_Modif_SGBD_Equipements : \n%s ",pl_Msg);		
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* On previent l IHM de l administrateur */
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
		fclose(pl_FD);		
		return(XDC_NOK);		
	}
	else
	{
		sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_Modif_SGBD_Equipements : %s",pl_Msg);
		cuti_BilanConf_Trace("********************************************************");
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
	}
	/*A
	** Verification des NAV
	*/
	if ((vl_retcode = xzao160_Lire_Config_NAV (pl_FD, cmvf_FctUti_MSE_XZAO160_161 ,&pl_NumeroLigne))!=XDC_OK)
	{
		if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
		else sprintf(pl_Msg,"Erreur detectée dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
	 	XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb : xzao160_Lire_Config_NAV () a echoue");
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
	
	/* ABE	27-04-2015 : projet suppression ancienne GTC (GTCA)	*/	
//	/*A
//	** Ouverture du fichier de configuration dessequences autoroutieres de type tube 
//	*/
//	sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_EQUI_TRAV,XDF_CONF_SEQ_AUT_TUBE );	
//	if (!(pl_FD=fopen(pl_NomFile,"r")))
//	{
//		/*B 
//		** En cas de prob d ouverture du fichier de conf, on logue le prob a l administrateur
//		** et on le notifie dans le fichier Bilan Config
//		*/
//		sprintf(pl_Msg,"Erreur d'ouverture du fichier %s\n",pl_NomFile);
//		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_Modif_SGBD_Equipements : \n%s ",pl_Msg);		
//		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* On previent l IHM de l administrateur */
//		cuti_BilanConf_Trace(pl_Msg); 
//	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
//		fclose(pl_FD);		
//		return(XDC_NOK);		
//	}
//	else
//	{
//		sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
//		XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_Modif_SGBD_Equipements : %s",pl_Msg);
//		cuti_BilanConf_Trace("********************************************************");
//		cuti_BilanConf_Trace(pl_Msg); 
//	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
//	}
//	/*A
//	** Verification des sequences autoroutieres de type tube
//	*/
//	if ((vl_retcode = xzao169_Lire_Config_SeqAut_Tunnel (pl_FD, cmvf_FctUti_MSE_XZAO169_170  ,&pl_NumeroLigne))!=XDC_OK)
//	{
//		if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
//		else sprintf(pl_Msg,"Erreur detectée dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
//		cuti_BilanConf_Trace(pl_Msg); 
//	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
//	 	XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb : xzao169_Lire_Config_SeqAut_Tunnel () a echoue");
//		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cvce_VerifModif_cb a echoue : Sortie");
//		fclose(pl_FD);
//		return(XDC_NOK);
//	}
//	else
//	{
//		sprintf(pl_Msg,"Traitement de %s OK.",pl_NomFile);
//		cuti_BilanConf_Trace(pl_Msg); 
//	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);	   /* Notification dans Bilan config */
//		fclose(pl_FD);
//	}
	
	/* ABE	27-04-2015 : projet suppression ancienne GTC (GTCA)	*/	
//	/*A
//	** Ouverture du fichier de configuration des sequences autorisees des echangeurs
//	*/
//	sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_EQUI_TRAV,XDF_CONF_SEQ_AUT_ECHANGEUR );	
//	if (!(pl_FD=fopen(pl_NomFile,"r")))
//	{
//		/*B 
//		** En cas de prob d ouverture du fichier de conf, on logue le prob a l administrateur
//		** et on le notifie dans le fichier Bilan Config
//		*/
//		sprintf(pl_Msg,"Erreur d'ouverture du fichier %s\n",pl_NomFile);
//		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_Modif_SGBD_Equipements : \n%s ",pl_Msg);		
//		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* On previent l IHM de l administrateur */
//		cuti_BilanConf_Trace(pl_Msg); 
//	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
//		fclose(pl_FD);		
//		return(XDC_NOK);		
//	}
//	else
//	{
//		sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
//		XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_Modif_SGBD_Equipements : %s",pl_Msg);
//		cuti_BilanConf_Trace("********************************************************");
//		cuti_BilanConf_Trace(pl_Msg); 
//	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
//	}
//	/*A
//	** Verification des sequences autorisees des echangeurs
//	*/
//	if ((vl_retcode = xzao171_Lire_Config_SeqAut_Echangeur (pl_FD, cmvf_FctUti_MSE_XZAO171_172  ,&pl_NumeroLigne))!=XDC_OK)
//	{
//		if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
//		else sprintf(pl_Msg,"Erreur detectée dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
//		cuti_BilanConf_Trace(pl_Msg); 
//	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
//	 	XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb : xzao171_Lire_Config_SeqAut_Echangeur () a echoue");
//		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cvce_VerifModif_cb a echoue : Sortie");
//		fclose(pl_FD);
//		return(XDC_NOK);
//	}
//	else
//	{
//		sprintf(pl_Msg,"Traitement de %s OK.",pl_NomFile);
//		cuti_BilanConf_Trace(pl_Msg); 
//	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);	   /* Notification dans Bilan config */
//		fclose(pl_FD);
//	}

	/*A
	** Ouverture du fichier de configuration des capteurs tunnel
	*/
	sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_EQUI_TRAV,XDF_CONF_CAPT_TUNNEL );	
	if (!(pl_FD=fopen(pl_NomFile,"r")))
	{
		/*B 
		** En cas de prob d ouverture du fichier de conf, on logue le prob a l administrateur
		** et on le notifie dans le fichier Bilan Config
		*/
		sprintf(pl_Msg,"Erreur d'ouverture du fichier %s\n",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_Modif_SGBD_Equipements : \n%s ",pl_Msg);		
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* On previent l IHM de l administrateur */
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
		fclose(pl_FD);		
		return(XDC_NOK);		
	}
	else
	{
		sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_Modif_SGBD_Equipements : %s",pl_Msg);
		cuti_BilanConf_Trace("********************************************************");
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
	}
	/*A
	** Verification des capteurs tunnel
	*/
	if ((vl_retcode = xzao179_Lire_Config_Capteurs_Tunnel (pl_FD, cmvf_FctUti_MSE_XZAO179_180  ,&pl_NumeroLigne))!=XDC_OK)
	{
		if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
		else sprintf(pl_Msg,"Erreur detectée dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
	 	XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb : xzao179_Lire_Config_Capteurs_Tunnel () a echoue");
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
	** Ouverture du fichier de configuration des FAV
	*/
	sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_EQUI_TRAV,XDF_CONF_FAV );	
	if (!(pl_FD=fopen(pl_NomFile,"r")))
	{
		/*B 
		** En cas de prob d ouverture du fichier de conf, on logue le prob a l administrateur
		** et on le notifie dans le fichier Bilan Config
		*/
		sprintf(pl_Msg,"Erreur d'ouverture du fichier %s\n",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_Modif_SGBD_Equipements : \n%s ",pl_Msg);		
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* On previent l IHM de l administrateur */
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
		fclose(pl_FD);		
		return(XDC_NOK);		
	}
	else
	{
		sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_Modif_SGBD_Equipements : %s",pl_Msg);
		cuti_BilanConf_Trace("********************************************************");
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
	}
	/*A
	** Verification des FAV
	*/
	if ((vl_retcode = xzao181_Lire_Config_FAV (pl_FD, cmvf_FctUti_MSE_XZAO181_182  ,&pl_NumeroLigne))!=XDC_OK)
	{
		if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
		else sprintf(pl_Msg,"Erreur detectée dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
	 	XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb : xzao181_Lire_Config_FAV () a echoue");
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
	** Ouverture du fichier de configuration des PnGTC
	*/
	sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_EQUI_TRAV,XDF_CONF_PN_GTC );	
	if (!(pl_FD=fopen(pl_NomFile,"r")))
	{
		/*B 
		** En cas de prob d ouverture du fichier de conf, on logue le prob a l administrateur
		** et on le notifie dans le fichier Bilan Config
		*/
		sprintf(pl_Msg,"Erreur d'ouverture du fichier %s\n",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_Modif_SGBD_Equipements : \n%s ",pl_Msg);		
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* On previent l IHM de l administrateur */
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
		fclose(pl_FD);		
		return(XDC_NOK);		
	}
	else
	{
		sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_Modif_SGBD_Equipements : %s",pl_Msg);
		cuti_BilanConf_Trace("********************************************************");
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
	}
	/*A
	** Verification des PnGTC
	*/
	if ((vl_retcode = xzao183_Lire_Config_PnGTC (pl_FD,  cmvf_FctUti_MSE_XZAO183_184 ,&pl_NumeroLigne))!=XDC_OK)
	{
		if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
		else sprintf(pl_Msg,"Erreur detectée dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
	 	XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb : xzao183_Lire_Config_PnGTC () a echoue");
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
	** Ouverture du fichier de configuration des PAL
	*/
	sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_EQUI_TRAV,XDF_CONF_PAL );	
	if (!(pl_FD=fopen(pl_NomFile,"r")))
	{
		/*B 
		** En cas de prob d ouverture du fichier de conf, on logue le prob a l administrateur
		** et on le notifie dans le fichier Bilan Config
		*/
		sprintf(pl_Msg,"Erreur d'ouverture du fichier %s\n",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_Modif_SGBD_Equipements : \n%s ",pl_Msg);		
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* On previent l IHM de l administrateur */
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
		fclose(pl_FD);		
		return(XDC_NOK);		
	}
	else
	{
		sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_Modif_SGBD_Equipements : %s",pl_Msg);
		cuti_BilanConf_Trace("********************************************************");
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
	}
	/*A
	** Verification des PAL
	*/
	if ((vl_retcode = xzao420_Lire_Config_PAL (pl_FD, cmvf_FctUti_MSE_XZAO421_420 ,&pl_NumeroLigne))!=XDC_OK)
	{
		if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
		else sprintf(pl_Msg,"Erreur detectée dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
	 	XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb : xzao420_Lire_Config_PAL () a echoue");
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
	** Ouverture du fichier de configuration des PMVA
	*/
	sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_EQUI_TRAV,XDF_CONF_PMVA);
	if (!(pl_FD=fopen(pl_NomFile,"r")))
	{
		/*B 
		** En cas de prob d ouverture du fichier de conf, on logue le prob a l administrateur
		** et on le notifie dans le fichier Bilan Config
		*/
		sprintf(pl_Msg,"Erreur d'ouverture du fichier %s\n",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_Modif_SGBD_Equipements : \n%s ",pl_Msg);		
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* On previent l IHM de l administrateur */
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
		fclose(pl_FD);		
		return(XDC_NOK);		
	}
	else
	{
		sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_Modif_SGBD_Equipements : %s",pl_Msg);
		cuti_BilanConf_Trace("********************************************************");
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
	}
	/*A
	** Verification PMVA
	*/
	if ((vl_retcode = xzao430_Lire_Config_PMVA(pl_FD,cmvf_FctUti_MSE_XZAO422_720,&pl_NumeroLigne))!=XDC_OK)
	{
		if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
		else sprintf(pl_Msg,"Erreur detectée dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
	 	XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb :xzao103_Lire_Config_PMV_Picto () a echoue");
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
	** Ouverture du fichier de configuration des zones d'influence PMVA
	*/
	sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_EQUI_TRAV,XDF_CONF_ZON_PMVA);	
	if (!(pl_FD=fopen(pl_NomFile,"r")))
	{
		/*B 
		** En cas de prob d ouverture du fichier de conf, on logue le prob a l administrateur
		** et on le notifie dans le fichier Bilan Config
		*/
		sprintf(pl_Msg,"Erreur d'ouverture du fichier %s\n",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_Modif_SGBD_Equipements : \n%s ",pl_Msg);		
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* On previent l IHM de l administrateur */
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
		fclose(pl_FD);		
		return(XDC_NOK);		
	}
	else
	{
		sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_Modif_SGBD_Equipements : %s",pl_Msg);
		cuti_BilanConf_Trace("********************************************************");
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
	}
	/*A
	** Verification des zones d'influence PMVA
	*/
	if ((vl_retcode = xzao428_Lire_Config_Zon_PMVA (pl_FD, cmvf_FctUti_MSE_XZAO428_ZON_PMVA  ,&pl_NumeroLigne))!=XDC_OK)
	{
		if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
		else sprintf(pl_Msg,"Erreur detectée dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
	 	XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb : xzao428_Lire_Config_Zon_PMVA () a echoue");
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
	** Ouverture du fichier de configuration des zones d'influence PMVA SAGA
	*/
	sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_EQUI_TRAV,XDF_CONF_ZON_PMVA_SAGA);	
	if (!(pl_FD=fopen(pl_NomFile,"r")))
	{
		/*B 
		** En cas de prob d ouverture du fichier de conf, on logue le prob a l administrateur
		** et on le notifie dans le fichier Bilan Config
		*/
		sprintf(pl_Msg,"Erreur d'ouverture du fichier %s\n",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_Modif_SGBD_Equipements : \n%s ",pl_Msg);		
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* On previent l IHM de l administrateur */
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
		fclose(pl_FD);		
		return(XDC_NOK);		
	}
	else
	{
		sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_Modif_SGBD_Equipements : %s",pl_Msg);
		cuti_BilanConf_Trace("********************************************************");
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
	}
	/*A
	** Verification des zones d'influence PMVA
	*/
	if ((vl_retcode = xzao428_Lire_Config_Zon_PMVA (pl_FD, cmvf_FctUti_MSE_XZAO830  ,&pl_NumeroLigne))!=XDC_OK)
	{
		if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
		else sprintf(pl_Msg,"Erreur detectée dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
	 	XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb : xzao428_Lire_Config_Zon_PMVA () a echoue");
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
	** Ouverture du fichier de configuration des BAF
	*/
	sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_EQUI_TRAV,XDF_CONF_BAF);	
	if (!(pl_FD=fopen(pl_NomFile,"r")))
	{
		/*B 
		** En cas de prob d ouverture du fichier de conf, on logue le prob a l administrateur
		** et on le notifie dans le fichier Bilan Config
		*/
		sprintf(pl_Msg,"Erreur d'ouverture du fichier %s\n",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_Modif_SGBD_Equipements : \n%s ",pl_Msg);		
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* On previent l IHM de l administrateur */
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
		fclose(pl_FD);		
		return(XDC_NOK);		
	}
	else
	{
		sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_Modif_SGBD_Equipements : %s",pl_Msg);
		cuti_BilanConf_Trace("********************************************************");
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
	}
	/*A
	** Verification BAF
	*/
	if ((vl_retcode = xzao431_Lire_Config_BAF(pl_FD,cmvf_FctUti_MSE_XZAO450_BAF,&pl_NumeroLigne))!=XDC_OK)
	{
		if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
		else sprintf(pl_Msg,"Erreur detectée dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
	 	XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb :xzao103_Lire_Config_PMV_Picto () a echoue");
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
	** Ouverture du fichier de configuration des zones d'influence BAF
	*/
	sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_EQUI_TRAV,XDF_CONF_ZON_BAF);	
	if (!(pl_FD=fopen(pl_NomFile,"r")))
	{
		/*B 
		** En cas de prob d ouverture du fichier de conf, on logue le prob a l administrateur
		** et on le notifie dans le fichier Bilan Config
		*/
		sprintf(pl_Msg,"Erreur d'ouverture du fichier %s\n",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_Modif_SGBD_Equipements : \n%s ",pl_Msg);		
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* On previent l IHM de l administrateur */
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
		fclose(pl_FD);		
		return(XDC_NOK);		
	}
	else
	{
		sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_Modif_SGBD_Equipements : %s",pl_Msg);
		cuti_BilanConf_Trace("********************************************************");
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
	}
	/*A
	** Verification des zones d'influence BAF
	*/
	if ((vl_retcode = xzao433_Lire_Config_Zon_BAF (pl_FD, cmvf_FctUti_MSE_XZAO433_ZON_BAF  ,&pl_NumeroLigne))!=XDC_OK)
	{
		if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
		else sprintf(pl_Msg,"Erreur detectée dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
	 	XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb : xzao433_Lire_Config_Zon_BAF () a echoue");
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
	** Ouverture du fichier de configuration des BAD
	*/
	sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_EQUI_TRAV,XDF_CONF_BAD);	
	if (!(pl_FD=fopen(pl_NomFile,"r")))
	{
		/*B 
		** En cas de prob d ouverture du fichier de conf, on logue le prob a l administrateur
		** et on le notifie dans le fichier Bilan Config
		*/
		sprintf(pl_Msg,"Erreur d'ouverture du fichier %s\n",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_Modif_SGBD_Equipements : \n%s ",pl_Msg);		
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* On previent l IHM de l administrateur */
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
		fclose(pl_FD);		
		return(XDC_NOK);		
	}
	else
	{
		sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_Modif_SGBD_Equipements : %s",pl_Msg);
		cuti_BilanConf_Trace("********************************************************");
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
	}
	/*A
	** Verification BAD
	*/
	if ((vl_retcode = xzao432_Lire_Config_BAD(pl_FD,cmvf_FctUti_MSE_XZAO451_BAD,&pl_NumeroLigne))!=XDC_OK)
	{
		if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
		else sprintf(pl_Msg,"Erreur detectée dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
	 	XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb :xzao103_Lire_Config_PMV_Picto () a echoue");
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
	** Ouverture du fichier de configuration des zones d'influence BAD
	*/
	sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_EQUI_TRAV,XDF_CONF_ZON_BAD);	
	if (!(pl_FD=fopen(pl_NomFile,"r")))
	{
		/*B 
		** En cas de prob d ouverture du fichier de conf, on logue le prob a l administrateur
		** et on le notifie dans le fichier Bilan Config
		*/
		sprintf(pl_Msg,"Erreur d'ouverture du fichier %s\n",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_Modif_SGBD_Equipements : \n%s ",pl_Msg);		
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* On previent l IHM de l administrateur */
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
		fclose(pl_FD);		
		return(XDC_NOK);		
	}
	else
	{
		sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_Modif_SGBD_Equipements : %s",pl_Msg);
		cuti_BilanConf_Trace("********************************************************");
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
	}
	/*A
	** Verification des zones d'influence BAF
	*/
	if ((vl_retcode = xzao434_Lire_Config_Zon_BAD (pl_FD, cmvf_FctUti_MSE_XZAO434_ZON_BAD  ,&pl_NumeroLigne))!=XDC_OK)
	{
		if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
		else sprintf(pl_Msg,"Erreur detectée dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
	 	XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb : xzao434_Lire_Config_Zon_BAD () a echoue");
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
	** Ouverture du fichier de configuration des zones de regulation
	*/
	sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_EQUI_TRAV,XDF_CONF_ZON_REG);
	if (!(pl_FD=fopen(pl_NomFile,"r")))
	{
		/*B 
		** En cas de prob d ouverture du fichier de conf, on logue le prob a l administrateur
		** et on le notifie dans le fichier Bilan Config
		*/
		sprintf(pl_Msg,"Erreur d'ouverture du fichier %s\n",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_Modif_SGBD_Equipements : \n%s ",pl_Msg);		
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* On previent l IHM de l administrateur */
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
		fclose(pl_FD);		
		return(XDC_NOK);		
	}
	else
	{
		sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_Modif_SGBD_Equipements : %s",pl_Msg);
		cuti_BilanConf_Trace("********************************************************");
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
	}
	/*A
	** Verification des zones de regulation
	*/
	if ((vl_retcode = xzao253_Lire_Config_Zon_Regul (pl_FD, cmvf_FctUti_MSE_XZAO553_Zone_Regul  ,&pl_NumeroLigne))!=XDC_OK)
	{
		if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
		else sprintf(pl_Msg,"Erreur detectee dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
	 	XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb : xzao253_Lire_Config_Zon_Regul () a echoue");
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
	** Ouverture du fichier de configuration des stations (points de mesure) DIT
	*/
	/*!
	**    DEPEND DE LA CONFIGURATION DES STATIONS RADT, QUI DOIT ETRE FAITE AVANT !!!
	*/

	sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_EQUI_TRAV, XDF_CONF_STATION_DIT);
	if (!(pl_FD=fopen(pl_NomFile,"r")))
	{
		/*B 
		** En cas de prob d ouverture du fichier de conf, on logue le prob a l administrateur
		** et on le notifie dans le fichier Bilan Config
		*/
		sprintf(pl_Msg,"Erreur d'ouverture du fichier %s\n",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_Modif_SGBD_Equipements : \n%s ",pl_Msg);		
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* On previent l IHM de l administrateur */
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
		fclose(pl_FD);		
		return(XDC_NOK);		
	}
	else
	{
		sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_Modif_SGBD_Equipements : %s",pl_Msg);
		cuti_BilanConf_Trace("********************************************************");
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
	}
	/*A
	** Verification des stations (points de mesure) DIT
	*/
	if ((vl_retcode = xzao261_Lire_Config_Stations_DIT (pl_FD, cmvf_FctUti_MSE_XZAO261_262, &pl_NumeroLigne))!=XDC_OK)
	{
		if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
		else sprintf(pl_Msg,"Erreur detectée dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
	 	XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb : xzao261_Lire_Config_Stations_DIT () a echoue");
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
	** Ouverture du fichier de configuration des CFE
	*/
	sprintf(pl_NomFile,"%s/%s",CLIVC_LIVR_EQUI_TRAV,XDF_CONF_CFE);	
	if (!(pl_FD=fopen(pl_NomFile,"r")))
	{
		/*B 
		** En cas de prob d ouverture du fichier de conf, on logue le prob a l administrateur
		** et on le notifie dans le fichier Bilan Config
		*/
		sprintf(pl_Msg,"Erreur d'ouverture du fichier %s\n",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cmvf_Modif_SGBD_Equipements : \n%s ",pl_Msg);		
		XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* On previent l IHM de l administrateur */
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
		fclose(pl_FD);		
		return(XDC_NOK);		
	}
	else
	{
		sprintf(pl_Msg,"Traitement du fichier %s en cours",pl_NomFile);
		XZST_03EcritureTrace(XZSTC_DEBUG1,"cmvf_Modif_SGBD_Equipements : %s",pl_Msg);
		cuti_BilanConf_Trace("********************************************************");
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg); /* Notification dans Bilan config*/
	}
	/*A
	** Verification CFE
	*/
	if ((vl_retcode = xzao259_Lire_Config_CFE(pl_FD,cmvf_FctUti_MSE_XZAO258_CFE,&pl_NumeroLigne))!=XDC_OK)
	{
		if (pl_NumeroLigne == 0) sprintf(pl_Msg,"Impossible de traiter le fichier %s",pl_NomFile);
		else sprintf(pl_Msg,"Erreur detectee dans %s a la ligne %d",pl_NomFile,pl_NumeroLigne);
		cuti_BilanConf_Trace(pl_Msg); 
	 	XZIA08_AffMsgAdm(CGCDV_NOMMACHINE,pl_Msg);
	 	XZST_03EcritureTrace(XZSTC_WARNING ,"TCGCD : cvce_VerifModif_cb :xzao259_Lire_Config_CFE () a echoue");
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
	** Si toute les Verifications sont OK
	** On passe le flags pour verification equipement a OK
	*/
	if((vl_retcode = cliv_Modif_Tab_Checklist(XZCGC_TYPEF_EQUI,XDC_OK))==XDC_NOK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"cliv_Liv_Diff_Fichier : cliv_Modif_Tab_Checklist a echoue pour type de fichier %s a passer a %d",
					XZCGC_TYPEF_EQUI,XDC_NOK); 
	}
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT: cmvf_Modif_SGBD_Equipements : Fin d execution");
	return(XDC_OK);
}
