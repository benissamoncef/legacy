/*E*/
/*Fichier : $Id: cgcd_mSGBDma.c,v 1.7 2016/07/13 16:57:37 devgfi Exp $      Release : $Revision: 1.7 $        Date : $Date: 2016/07/13 16:57:37 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  GECONF
------------------------------------------------------
* MODULE MVF * FICHIER cgcd_mSGBDma.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  	Fichier appartenant au module MVF contenant les fonctions
*  modificationSGBDmachine. 
***************************
*
*	cmvf_FctUti_MSM_XZAO95_64 : Fct utilisateur passee a xzao95_Lire_Config_Machine,
*  qui appelle XZAO64_Ajouter_Machine avec le nom de la base sur la quelle XZAO64_Ajouter_Machine va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
*************************** 
*
*	cmvf_FctUti_MSM_XZAO96_65 : Fct utilisateur passee a XZAO96,
*  qui appelle XZAO65_Ajouter_Suppleant avec le nom de la base sur la quelle XZAO65_Ajouter_Suppleant va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
***************************
*	cmvf_FctUti_MSM_XZAO97_66 : Fct utilisateur passee a xzao97_Lire_Config_ServeurTerminaux,
*  qui appelle XZAO66_Ajouter_Serveur_Terminaux avec le nom de la base sur la quelle XZAO66_Ajouter_Serveur_Terminaux va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
***************************

------------------------------------------------------
* HISTORIQUE :
*
* mercier	19 Oct 1994	: Creation
*****************
* MERCIER	21 Dec 1994	: Modification de l appel V 1.3
*  de xzia_08
*****************
* MERCIER	18 Jan 1995	: passage de cgcd_BilanConf_Trace	V 1.5
* a cuti_BilanConf_Trace
* NIEPCERON	07 Jun 1996	: Suppression de l'affichage des messages de trait. des lignes v1.6 (968)
------------------------------------------------------*/

/* fichiers inclus */

#include "cgcd_mSGBDma.h"

/* definitions de constante */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: cgcd_mSGBDma.c,v 1.7 2016/07/13 16:57:37 devgfi Exp $ : cgcd_mSGBDma" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	cmvf_FctUti_MSM_XZAO95_64 : Fct utilisateur passee a xzao95_Lire_Config_Machine,
*  qui appelle XZAO64_Ajouter_Machine avec le nom de la base sur la quelle XZAO64_Ajouter_Machine va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cmvf_FctUti_MSM_XZAO95_64(XZAOT_ConfMachine       va_Machine_in, XDY_Entier     va_Resultat_out) 
/*
* ARGUMENTS EN ENTREE :  va_Machine_in contient la machine a rajouter
*			va_Resultat_out contient le numero de ligne du fichier qui est en cours 
*			de traitement.
*			 	
* ARGUMENTS EN SORTIE :  aucun
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
*
* CONDITION D'UTILISATION
*
* FONCTION 
*	Doit appeler XZAO64_Ajouter_Machine
* 	
------------------------------------------------------*/
{
	static char *version 	= "$Id: cgcd_mSGBDma.c,v 1.7 2016/07/13 16:57:37 devgfi Exp $ : cmvf_FctUti_MSM_XZAO95_64";
	char    vl_NomBase[4]	;
	int 	vl_retcode 	= XDC_OK;
	int 	vl_retcode_out 	= XDC_OK;
	char  	pl_Msg[500];			/* contient le mesg a tracer*/
	char  	pl_Msg2[500];			/* contient modification si vl_Resultat_out XDC_AJOUT,...*/
	char  	pl_NomProcs[80] ;
	
	XDY_ResConf vl_Resultat_out;
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MSM_XZAO95_64");
	
	strcpy( vl_NomBase,CGCDG_Base_Courante);	
	if ((vl_retcode =XZAO64_Ajouter_Machine(vl_NomBase,va_Machine_in, &vl_Resultat_out))!=XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSM_XZAO95_64 a echoue sur ligne %ld",(long)va_Resultat_out);
		vl_retcode_out = XDC_NOK;	
	}
	else
	/*A
	** Test sur parametres de sortie de XZAO64_Ajouter_Machine sur coherence base
	** 4 valeurs de status d execution de la sp dans vl_Resultat_out
	** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
	*/
	{
		if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
		{
			strcpy(pl_NomProcs,"XZAO64_Ajouter_Machine");
			XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld",pl_NomProcs,pl_Msg2,(long)va_Resultat_out);
			
			sprintf(pl_Msg,"%s %s sur la ligne %ld",XDF_CONF_MACHINE,pl_Msg2,(long)va_Resultat_out);
			cuti_BilanConf_Trace(pl_Msg);	/* Notification dans Bilan config 	*/
			
		}
		else
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSM_XZAO95_64 : cmvf_Trace_TYPE_Result a echoue");
			vl_retcode_out = XDC_NOK;
		}
	
		vl_retcode = vl_retcode_out;
	}	
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MSM_XZAO95_64 : sortie avec %d",vl_retcode_out);
	return(vl_retcode_out);
}

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	cmvf_FctUti_MSM_XZAO96_65 : Fct utilisateur passee a XZAO96,
*  qui appelle XZAO65_Ajouter_Suppleant avec le nom de la base sur la quelle XZAO65_Ajouter_Suppleant va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cmvf_FctUti_MSM_XZAO96_65(XZAOT_ConfSuppl         va_Suppleant_in, XDY_Entier     va_Resultat_out) 
/*
* ARGUMENTS EN ENTREE :  va_Suppleant_in contient le suppleant a rajouter
*			va_Resultat_out contient le numero de ligne du fichier qui est en cours 
*			de traitement.
*			 	
* ARGUMENTS EN SORTIE :  aucun
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
*
* CONDITION D'UTILISATION
*
* FONCTION 
*	Doit appeler XZAO65_Ajouter_Suppleant
* 	
------------------------------------------------------*/
{
	static char *version 	= "$Id: cgcd_mSGBDma.c,v 1.7 2016/07/13 16:57:37 devgfi Exp $ : cmvf_FctUti_MSM_XZAO96_65";
	char    vl_NomBase[4]	;
	int 	vl_retcode 	= XDC_OK;
	int 	vl_retcode_out 	= XDC_OK;
	char  	pl_Msg[500];			/* contient le mesg a tracer*/
	char  	pl_Msg2[500];			/* contient modification si vl_Resultat_out XDC_AJOUT,...*/
	char  	pl_NomProcs[80] ;
	
	XDY_ResConf vl_Resultat_out;
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MSM_XZAO96_65");
	
	strcpy( vl_NomBase,CGCDG_Base_Courante);	
	if ((vl_retcode =XZAO65_Ajouter_Suppleant(vl_NomBase,va_Suppleant_in, &vl_Resultat_out))!=XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSM_XZAO96_65 a echoue sur ligne %ld",(long)va_Resultat_out);
		vl_retcode_out = XDC_NOK;	
	}
	else
	/*A
	** Test sur parametres de sortie de XZAO65_Ajouter_Suppleant sur coherence base
	** 4 valeurs de status d execution de la sp dans vl_Resultat_out
	** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
	*/
	{
		if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
		{
			strcpy(pl_NomProcs,"XZAO65_Ajouter_Suppleant");
			XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld",pl_NomProcs,pl_Msg2,(long)va_Resultat_out);
			
			sprintf(pl_Msg,"%s %s sur la ligne %ld",XDF_CONF_SUPPLEANT,pl_Msg2,(long)va_Resultat_out);
			cuti_BilanConf_Trace(pl_Msg);	/* Notification dans Bilan config 	*/
			
		}
		else
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSM_XZAO96_65 : cmvf_Trace_TYPE_Result a echoue");
			vl_retcode_out = XDC_NOK;
		}
	
		vl_retcode = vl_retcode_out;
	}	
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MSM_XZAO96_65 : sortie avec %d",vl_retcode_out);
	return(vl_retcode_out);
}
/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	cmvf_FctUti_MSM_XZAO97_66 : Fct utilisateur passee a xzao97_Lire_Config_ServeurTerminaux,
*  qui appelle XZAO66_Ajouter_Serveur_Terminaux avec le nom de la base sur la quelle XZAO66_Ajouter_Serveur_Terminaux va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cmvf_FctUti_MSM_XZAO97_66(XZAOT_ConfST            va_Serveur_in, XDY_Entier     va_Resultat_out) 
/*
* ARGUMENTS EN ENTREE :  va_Serveur_in contient le serveur de terminaux a rajouter
*			va_Resultat_out contient le numero de ligne du fichier qui est en cours 
*			de traitement.
*			 	
* ARGUMENTS EN SORTIE :  aucun
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
*
* CONDITION D'UTILISATION
*
* FONCTION 
*	Doit appeler XZAO66_Ajouter_Serveur_Terminaux
* 	
------------------------------------------------------*/
{
	static char *version 	= "$Id: cgcd_mSGBDma.c,v 1.7 2016/07/13 16:57:37 devgfi Exp $ : cmvf_FctUti_MSM_XZAO97_66";
	char    vl_NomBase[4]	;
	int 	vl_retcode 	= XDC_OK;
	int 	vl_retcode_out 	= XDC_OK;
	char  	pl_Msg[500];			/* contient le mesg a tracer*/
	char  	pl_Msg2[500];			/* contient modification si vl_Resultat_out XDC_AJOUT,...*/
	char  	pl_NomProcs[80] ;
	
	XDY_ResConf vl_Resultat_out;
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MSM_XZAO97_66");
	
	strcpy( vl_NomBase,CGCDG_Base_Courante);	
	if ((vl_retcode =XZAO66_Ajouter_Serveur_Terminaux(vl_NomBase,va_Serveur_in, &vl_Resultat_out))!=XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSM_XZAO97_66 a echoue sur ligne %ld",(long)va_Resultat_out);
		vl_retcode_out = XDC_NOK;	
	}
	else
	/*A
	** Test sur parametres de sortie de XZAO66_Ajouter_Serveur_Terminaux sur coherence base
	** 4 valeurs de status d execution de la sp dans vl_Resultat_out
	** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
	*/
	{
		if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
		{
			strcpy(pl_NomProcs,"XZAO66_Ajouter_Serveur_Terminaux");
			XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld",pl_NomProcs,pl_Msg2,(long)va_Resultat_out);
			
			sprintf(pl_Msg,"%s %s sur la ligne %ld",XDF_CONF_SERV_TERM,pl_Msg2,(long)va_Resultat_out);
			cuti_BilanConf_Trace(pl_Msg);	/* Notification dans Bilan config 	*/
			
		}
		else
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSM_XZAO97_66 : cmvf_Trace_TYPE_Result a echoue");
			vl_retcode_out = XDC_NOK;
		}
	
		vl_retcode = vl_retcode_out;
	}	
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MSM_XZAO97_66 : sortie avec %d",vl_retcode_out);
	return(vl_retcode_out);
}
