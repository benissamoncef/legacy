/*E*/
/*Fichier : $Id: cgcd_mSGBDty.c,v 1.9 2016/07/13 17:58:20 devgfi Exp $      Release : $Revision: 1.9 $        Date : $Date: 2016/07/13 17:58:20 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  GECONF
------------------------------------------------------
* MODULE MVF * FICHIER cgcd_mSGBDty.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* 	Fichier appartenant au module MVF contenant les fonctions
*  utilisateur de modificationSGBDtype.
*
*******************************
*	cmvf_FctUti_MST_XZAO101_70 : Fct utilisateur passee a xzao101_Lire_Config_Type_PMV,
*  qui appelle XZAO70_Ajouter_Type_PMV avec le nom de la base sur la quelle XZAO70_Ajouter_Type_PMV 
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*******************************
*	cmvf_FctUti_MST_XZAO102_71 : Fct utilisateur passee a XZAO102,
*  qui appelle XZAO71_Ajouter_Type_Picto avec le nom de la base sur la quelle XZAO71_Ajouter_Type_Picto 
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
*******************************
*	cmvf_FctUti_MST_XZAO154_155 : Fct utilisateur passee a XZAO154,
*  qui appelle XZAO155_Ajouter_Scenario avec le nom de la base sur la quelle XZAO155_Ajouter_Scenario 
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
*******************************
*	cmvf_FctUti_MSE_XZAO198_188 : Fct utilisateur passee a xzao198_Lire_Config_Alerte,
*  qui appelle XZAO188_Ajouter_Type_Alerte avec le nom de la base sur la quelle XZAO188_Ajouter_Type_Alerte 
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
*******************************
*	cmvf_FctUti_MSE_XZAO196_189 : Fct utilisateur passee a xzao196_Lire_Config_Alarme,
*  qui appelle XZAO189_Ajouter_Type_Alarme avec le nom de la base sur la quelle XZAO189_Ajouter_Type_Alarme 
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
*******************************

------------------------------------------------------
* HISTORIQUE :
*
************************
* mercier	19 Oct 1994	: Creation
************************
* Mercier	28 Nov 1994	: Ajout de commentaire	V 1.3
************************
* MERCIER	21 Dec 1994	: Modification de l appel V 1.4
*  de xzia_08
************************
* MERCIER	18 Jan 1995	: passage de cgcd_BilanConf_Trace	V 1.6
* a cuti_BilanConf_Trace
* NIEPCERON	07 Jun 1996	: Suppression de l'affichage des messages de trait. des lignes v1.7 (968)
* MISMER	06 Nov 1996	: Ajout de la fonction de lecture des alarme et alerte v1.9 (DEM/1232)
* JPL		13/07/16 : Migration architecture ia64 : compatibilite des types d'arguments  1.9
------------------------------------------------------*/

/* fichiers inclus */

#include "cgcd_mSGBDty.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: cgcd_mSGBDty.c,v 1.9 2016/07/13 17:58:20 devgfi Exp $ : cgcd_mSGBDty" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	cmvf_FctUti_MST_XZAO101_70 : Fct utilisateur passee a xzao101_Lire_Config_Type_PMV,
*  qui appelle XZAO70_Ajouter_Type_PMV avec le nom de la base sur la quelle XZAO70_Ajouter_Type_PMV 
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cmvf_FctUti_MST_XZAO101_70(XZAOT_ConfTypPMV        va_TypePMV_in, XDY_Entier     va_Resultat_out) 
/*
* ARGUMENTS EN ENTREE :  va_TypePMV_in contient le type de PMV a rajouter
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
*	Doit appeler XZAO70_Ajouter_Type_PMV
* 	
------------------------------------------------------*/
{
	static char *version 	= "$Id: cgcd_mSGBDty.c,v 1.9 2016/07/13 17:58:20 devgfi Exp $ : cmvf_FctUti_MST_XZAO101_70";
	char    vl_NomBase[4]	;
	int 	vl_retcode 	= XDC_OK;
	int 	vl_retcode_out 	= XDC_OK;
	char  	pl_Msg[500];			/* contient le mesg a tracer*/
	char  	pl_Msg2[500];			/* contient modification si vl_Resultat_out XDC_AJOUT,...*/
	char  	pl_NomProcs[80] ;
	
	XDY_ResConf vl_Resultat_out;
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MST_XZAO101_70");
	
	strcpy( vl_NomBase,CGCDG_Base_Courante);	
	if ((vl_retcode =XZAO70_Ajouter_Type_PMV(vl_NomBase,va_TypePMV_in, &vl_Resultat_out))!=XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MST_XZAO101_70 a echoue sur ligne %ld",(long)va_Resultat_out);
		vl_retcode_out = XDC_NOK;	
	}
	else
	/*A
	** Test sur parametres de sortie de XZAO70_Ajouter_Type_PMV sur coherence base
	** 4 valeurs de status d execution de la sp dans vl_Resultat_out
	** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
	*/
	{
		if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
		{
			strcpy(pl_NomProcs,"XZAO70_Ajouter_Type_PMV");
			XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld",pl_NomProcs,pl_Msg2,(long)va_Resultat_out);
			
			sprintf(pl_Msg,"%s %s sur la ligne %ld",XDF_CONF_TYPE_PMV,pl_Msg2,(long)va_Resultat_out);	
			cuti_BilanConf_Trace(pl_Msg);	/* Notification dans Bilan config 	*/
			
		}
		else
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MST_XZAO101_70 : cmvf_Trace_TYPE_Result a echoue");
			vl_retcode_out = XDC_NOK;
		}
	
		vl_retcode = vl_retcode_out;
	}	
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MST_XZAO101_70 : sortie avec %d",vl_retcode_out);
	return(vl_retcode_out);
}
/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	cmvf_FctUti_MST_XZAO102_71 : Fct utilisateur passee a XZAO102,
*  qui appelle XZAO71_Ajouter_Type_Picto avec le nom de la base sur la quelle XZAO71_Ajouter_Type_Picto 
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int cmvf_FctUti_MST_XZAO102_71(XZAOT_ConfTypPicto      va_TypePicto_in, XDY_Entier     va_Resultat_out) 
/*
* ARGUMENTS EN ENTREE :  va_TypePicto_in contient le type de Picto a rajouter
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
*	Doit appeler XZAO71_Ajouter_Type_Picto
* 	
------------------------------------------------------*/
{
	static char *version 	= "$Id: cgcd_mSGBDty.c,v 1.9 2016/07/13 17:58:20 devgfi Exp $ : cmvf_FctUti_MST_XZAO102_71";
	char    vl_NomBase[4]	;
	int 	vl_retcode 	= XDC_OK;
	int 	vl_retcode_out 	= XDC_OK;
	char  	pl_Msg[500];			/* contient le mesg a tracer*/
	char  	pl_Msg2[500];			/* contient modification si vl_Resultat_out XDC_AJOUT,...*/
	char  	pl_NomProcs[80] ;
	
	XDY_ResConf vl_Resultat_out;
	
	XZST_03EcritureTrace(XZSTC_WARNING,"IN  : cmvf_FctUti_MST_XZAO102_71");
	
	strcpy( vl_NomBase,CGCDG_Base_Courante);	
	if ((vl_retcode =XZAO71_Ajouter_Type_Picto(vl_NomBase,va_TypePicto_in, &vl_Resultat_out))!=XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MST_XZAO102_71 a echoue sur ligne %ld",(long)va_Resultat_out);		
		vl_retcode_out = XDC_NOK;	
	}
	else
	/*A
	** Test sur parametres de sortie de XZAO71_Ajouter_Type_Picto sur coherence base
	** 4 valeurs de status d execution de la sp dans vl_Resultat_out
	** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
	*/
	{
		if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
		{
			strcpy(pl_NomProcs,"XZAO71_Ajouter_Type_Picto");
			XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld",pl_NomProcs,pl_Msg2,(long)va_Resultat_out);
			
			sprintf(pl_Msg,"%s %s sur la ligne %ld",XDF_CONF_TYPE_PICTO,pl_Msg2,(long)va_Resultat_out);	
			cuti_BilanConf_Trace(pl_Msg);	/* Notification dans Bilan config 	*/
			
		}
		else
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MST_XZAO102_71 : cmvf_Trace_TYPE_Result a echoue");
			vl_retcode_out = XDC_NOK;
		}
	
		vl_retcode = vl_retcode_out;
	}	
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MST_XZAO102_71 : sortie avec %d",vl_retcode_out);
	return(vl_retcode_out);
}
/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	cmvf_FctUti_MST_XZAO154_155 : Fct utilisateur passee a XZAO154,
*  qui appelle XZAO155_Ajouter_Scenario avec le nom de la base sur la quelle XZAO155_Ajouter_Scenario 
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int cmvf_FctUti_MST_XZAO154_155(XZAOT_ConfScenario      va_Scenario_in, XDY_Entier     va_Resultat_out) 
/*
* ARGUMENTS EN ENTREE :  va_Scenario_in contient le scenario a rajouter
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
*	Doit appeler XZAO155_Ajouter_Scenario
* 	
------------------------------------------------------*/
{
	static char *version 	= "$Id: cgcd_mSGBDty.c,v 1.9 2016/07/13 17:58:20 devgfi Exp $ : cmvf_FctUti_MST_XZAO154_155";
	char    vl_NomBase[4]	;
	int 	vl_retcode 	= XDC_OK;
	int 	vl_retcode_out 	= XDC_OK;
	char  	pl_Msg[500];			/* contient le mesg a tracer*/
	char  	pl_Msg2[500];			/* contient modification si vl_Resultat_out XDC_AJOUT,...*/
	char  	pl_NomProcs[80] ;
	
	XDY_ResConf vl_Resultat_out;
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MST_XZAO154_155");
	
	strcpy( vl_NomBase,CGCDG_Base_Courante);	
	if ((vl_retcode =XZAO155_Ajouter_Scenario(vl_NomBase,va_Scenario_in, &vl_Resultat_out))!=XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MST_XZAO154_155 a echoue sur ligne %ld",(long)va_Resultat_out);		
		vl_retcode_out = XDC_NOK;	
	}
	else
	/*A
	** Test sur parametres de sortie de XZAO155_Ajouter_Scenario sur coherence base
	** 4 valeurs de status d execution de la sp dans vl_Resultat_out
	** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
	*/
	{
		if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
		{
			strcpy(pl_NomProcs,"XZAO155_Ajouter_Scenario");
			XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld",pl_NomProcs,pl_Msg2,(long)va_Resultat_out);
			
			sprintf(pl_Msg,"%s %s sur la ligne %ld",XDF_CONF_SCENARIO,pl_Msg2,(long)va_Resultat_out);	
			cuti_BilanConf_Trace(pl_Msg);	/* Notification dans Bilan config 	*/
			
		}
		else
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MST_XZAO154_155 : cmvf_Trace_TYPE_Result a echoue");
			vl_retcode_out = XDC_NOK;
		}
	
		vl_retcode = vl_retcode_out;
	}	
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MST_XZAO154_155 : sortie avec %d",vl_retcode_out);
	return(vl_retcode_out);
}
/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	cmvf_FctUti_MST_XZAN02_01 : Fct utilisateur passee a XZAO154,
*  qui appelle XZAN01_Creer_Type_Astreinte avec le nom de la base sur la quelle XZAN01_Creer_Type_Astreinte 
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int cmvf_FctUti_MST_XZAN02_01( 		XDY_Nom         va_NomTypeAstreinte_in,
                                        XDY_Octet       va_NumAstreinte_in, 
                                        XDY_Entier     	va_Resultat_out) 
/*
* ARGUMENTS EN ENTREE :  va_Scenario_in contient le scenario a rajouter
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
*	Doit appeler XZAN01_Creer_Type_Astreinte
* 	
------------------------------------------------------*/
{
	static char *version 	= "$Id: cgcd_mSGBDty.c,v 1.9 2016/07/13 17:58:20 devgfi Exp $ : cmvf_FctUti_MST_XZAN02_01";
	char    vl_NomBase[4]	;
	int 	vl_retcode 	= XDC_OK;
	int 	vl_retcode_out 	= XDC_OK;
	char  	pl_Msg[500];			/* contient le mesg a tracer*/
	char  	pl_Msg2[500];			/* contient modification si vl_Resultat_out XDC_AJOUT,...*/
	char  	pl_NomProcs[80] ;
	
	XDY_ResConf vl_Resultat_out;
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MST_XZAN02_01");
	
	strcpy( vl_NomBase,CGCDG_Base_Courante);	
	if ((vl_retcode =XZAN01_Creer_Type_Astreinte(	vl_NomBase,
							va_NomTypeAstreinte_in,
							va_NumAstreinte_in, 
							&vl_Resultat_out))!=XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MST_XZAN02_01 a echoue sur ligne %ld",(long)va_Resultat_out);		
		vl_retcode_out = XDC_NOK;	
	}
	else
	/*A
	** Test sur parametres de sortie de XZAN01_Creer_Type_Astreinte sur coherence base
	** 4 valeurs de status d execution de la sp dans vl_Resultat_out
	** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
	*/
	{
		if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
		{
			strcpy(pl_NomProcs,"XZAN01_Creer_Type_Astreinte");
			XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld",pl_NomProcs,pl_Msg2,(long)va_Resultat_out);
			
			sprintf(pl_Msg,"%s %s sur la ligne %ld",XDF_TYPE_ASTREINTE,pl_Msg2,(long)va_Resultat_out);	
			cuti_BilanConf_Trace(pl_Msg);	/* Notification dans Bilan config 	*/
			
		}
		else
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MST_XZAN02_01 : cmvf_Trace_TYPE_Result a echoue");
			vl_retcode_out = XDC_NOK;
		}
	
		vl_retcode = vl_retcode_out;
	}	
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MST_XZAN02_01 : sortie avec %d",vl_retcode_out);
	return(vl_retcode_out);
}

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	cmvf_FctUti_MSE_XZAO198_188 : Fct utilisateur passee a xzao198_Lire_Config_Alerte,
*  qui appelle XZAO188_Ajouter_Type_Alerte avec le nom de la base sur la quelle XZAO188_Ajouter_Type_Alerte 
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cmvf_FctUti_MSE_XZAO198_188(XZAOT_ConfTypeAlerte       va_TypeAlerte_in, XDY_Entier     va_Resultat_out) 
/*
* ARGUMENTS EN ENTREE :  va_TypeAlerte_in contient le type d'alerte a rajouter
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
*	Doit appeler XZAO188_Ajouter_Type_Alerte
* 	
------------------------------------------------------*/
{
	static char *version 	= "$Id: cgcd_mSGBDty.c,v 1.9 2016/07/13 17:58:20 devgfi Exp $ : cmvf_FctUti_MSE_XZAO198_188";
	char    vl_NomBase[4]	;
	int 	vl_retcode 	= XDC_OK;
	int 	vl_retcode_out 	= XDC_OK;
	char  	pl_Msg[500];			/* contient le mesg a tracer*/
	char  	pl_Msg2[500];			/* contient modification si vl_Resultat_out XDC_AJOUT,...*/
	char  	pl_NomProcs[80] ;
	
	XDY_ResConf vl_Resultat_out;
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MSE_XZAO198_188");
	
	strcpy( vl_NomBase,CGCDG_Base_Courante);	
	if ((vl_retcode =XZAO188_Ajouter_Type_Alerte(vl_NomBase,va_TypeAlerte_in, &vl_Resultat_out))!=XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO198_188 a echoue sur ligne %ld",(long)va_Resultat_out);		
		vl_retcode_out = XDC_NOK;	
	}
	else
	/*A
	** Test sur parametres de sortie de XZAO188_Ajouter_Type_Alerte sur coherence base
	** 4 valeurs de status d execution de la sp dans vl_Resultat_out
	** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
	*/
	{
		if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
		{
			strcpy(pl_NomProcs,"XZAO188_Ajouter_Type_Alerte");
			XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld",pl_NomProcs,pl_Msg2,(long)va_Resultat_out);
			
			sprintf(pl_Msg,"%s %s sur la ligne %ld",XDF_CONF_ALERTE,pl_Msg2,(long)va_Resultat_out);	
			cuti_BilanConf_Trace(pl_Msg);	/* Notification dans Bilan config 	*/
			
		}
		else
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO198_188 : cmvf_Trace_TYPE_Result a echoue");
			vl_retcode_out = XDC_NOK;
		}
	
		vl_retcode = vl_retcode_out;
	}	
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MSE_XZAO198_188 : sortie avec %d",vl_retcode_out);
	return(vl_retcode_out);
}
/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	cmvf_FctUti_MSE_XZAO196_189 : Fct utilisateur passee a xzao196_Lire_Config_Alarme,
*  qui appelle XZAO189_Ajouter_Type_Alarme avec le nom de la base sur la quelle XZAO189_Ajouter_Type_Alarme 
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cmvf_FctUti_MSE_XZAO196_189(XZAOT_ConfTypeAlarme       va_TypeAlarme_in, XDY_Entier     va_Resultat_out) 
/*
* ARGUMENTS EN ENTREE :  va_TypeAlarme_in contient le type d'alarme a rajouter
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
*	Doit appeler XZAO189_Ajouter_Type_Alarme
* 	
------------------------------------------------------*/
{
	static char *version 	= "$Id: cgcd_mSGBDty.c,v 1.9 2016/07/13 17:58:20 devgfi Exp $ : cmvf_FctUti_MSE_XZAO196_189";
	char    vl_NomBase[4]	;
	int 	vl_retcode 	= XDC_OK;
	int 	vl_retcode_out 	= XDC_OK;
	char  	pl_Msg[500];			/* contient le mesg a tracer*/
	char  	pl_Msg2[500];			/* contient modification si vl_Resultat_out XDC_AJOUT,...*/
	char  	pl_NomProcs[80] ;
	
	XDY_ResConf vl_Resultat_out;
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MSE_XZAO196_189");
	
	strcpy( vl_NomBase,CGCDG_Base_Courante);	
	if ((vl_retcode =XZAO189_Ajouter_Type_Alarme(vl_NomBase,va_TypeAlarme_in, &vl_Resultat_out))!=XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO196_189 a echoue sur ligne %ld",(long)va_Resultat_out);		
		vl_retcode_out = XDC_NOK;	
	}
	else
	/*A
	** Test sur parametres de sortie de XZAO189_Ajouter_Type_Alarme sur coherence base
	** 4 valeurs de status d execution de la sp dans vl_Resultat_out
	** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
	*/
	{
		if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
		{
			strcpy(pl_NomProcs,"XZAO189_Ajouter_Type_Alarme");
			XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld",pl_NomProcs,pl_Msg2,(long)va_Resultat_out);
			
			sprintf(pl_Msg,"%s %s sur la ligne %ld",XDF_CONF_ALARME,pl_Msg2,(long)va_Resultat_out);	
			cuti_BilanConf_Trace(pl_Msg);	/* Notification dans Bilan config 	*/
			
		}
		else
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO196_189 : cmvf_Trace_TYPE_Result a echoue");
			vl_retcode_out = XDC_NOK;
		}
	
		vl_retcode = vl_retcode_out;
	}	
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MSE_XZAO196_189 : sortie avec %d",vl_retcode_out);
	return(vl_retcode_out);
}
