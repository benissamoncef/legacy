/*E*/
/*Fichier : $Id: cgcd_mSGBDihm.c,v 1.7 2016/07/13 18:16:44 devgfi Exp $      Release : $Revision: 1.7 $        Date : $Date: 2016/07/13 18:16:44 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  GECONF
------------------------------------------------------
* MODULE MVF * FICHIER cgcd_mSGBDihm.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* 	Fichier appartenant au module MVF contenant les fonctions
*  modificationSGBDihm.
*
*************************
*	cmvf_FctUti_MSI_XZAO62_63 : Fct utilisateur passee a xzao62_Lire_Config_Influence_Station,
*  qui appelle XZAO63_Ajouter_Influence_Station avec le nom de la base sur la quelle XZAO63_Ajouter_Influence_Station va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*************************
*	cmvf_FctUti_MSI_XZAO118_119 : Fct utilisateur passee a xzao118_Lire_Config_Troncon,
*  qui appelle XZAO119_Ajouter_Troncon avec le nom de la base sur la quelle XZAO119_Ajouter_Troncon va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
*************************
------------------------------------------------------
* HISTORIQUE :
*
* mercier	19 Oct 1994	: Creation
***********************
* MERCIER	21 Dec 1994	: Modification de l appel V 1.3
*  de xzia_08
***********************
* MERCIER	18 Jan  1995	: passage de cgcd_BilanConf_Trace	V 1.5
* a cuti_BilanConf_Trace
* NIEPCERON	07 Jun 1996	: Suppression de l'affichage des messages de trait. des lignes v1.6 (968)
* JPL		13/07/16	: Migration architecture ia64 : compatibilite des types d'arguments  1.7
------------------------------------------------------*/

/* fichiers inclus */

#include "cgcd_mvf_util.h"
#include "cgcd_util.h"

#include "cgcd_mSGBDihm.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: cgcd_mSGBDihm.c,v 1.7 2016/07/13 18:16:44 devgfi Exp $ : cgcd_mSGBDihm" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	cmvf_FctUti_MSI_XZAO62_63 : Fct utilisateur passee a xzao62_Lire_Config_Influence_Station,
*  qui appelle XZAO63_Ajouter_Influence_Station avec le nom de la base sur la quelle XZAO63_Ajouter_Influence_Station va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cmvf_FctUti_MSI_XZAO62_63( XZAOT_ConfInfluence va_Station_in, XDY_Entier     va_Resultat_out) 
/*
* ARGUMENTS EN ENTREE :  va_Station_in contient l influence station a rajouter
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
*	Doit appeler XZAO63_Ajouter_Influence_Station
* 	
------------------------------------------------------*/
{
	static char *version 	= "$Id: cgcd_mSGBDihm.c,v 1.7 2016/07/13 18:16:44 devgfi Exp $ : cmvf_FctUti_MSI_XZAO62_63";
	char    vl_NomBase[4]	;
	int 	vl_retcode 	= XDC_OK;
	int 	vl_retcode_out 	= XDC_OK;
	char  	pl_Msg[500];			/* contient le mesg a tracer*/
	char  	pl_Msg2[500];			/* contient modification si vl_Resultat_out XDC_AJOUT,...*/
	char  	pl_NomProcs[80] ;
	
	XDY_ResConf vl_Resultat_out;
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MSI_XZAO62_63");
	
	strcpy( vl_NomBase,CGCDG_Base_Courante);	
	if ((vl_retcode =XZAO63_Ajouter_Influence_Station(vl_NomBase,va_Station_in, &vl_Resultat_out))!=XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSI_XZAO62_63 a echoue sur ligne %ld",(long)va_Resultat_out);
		vl_retcode_out = XDC_NOK;	
	}
	else
	/*A
	** Test sur parametres de sortie de XZAO63_Ajouter_Influence_Station sur coherence base
	** 4 valeurs de status d execution de la sp dans vl_Resultat_out
	** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
	*/
	{
		if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
		{
			strcpy(pl_NomProcs,"XZAO63_Ajouter_Influence_Station");
			XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld",pl_NomProcs,pl_Msg2,(long)va_Resultat_out);
			
			sprintf(pl_Msg,"%s %s sur la ligne %ld",XDF_CONF_INFLU_STATION,pl_Msg2,(long)va_Resultat_out);
			cuti_BilanConf_Trace(pl_Msg);	/* Notification dans Bilan config 	*/
			
		}
		else
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSI_XZAO62_63 : cmvf_Trace_TYPE_Result a echoue");
			vl_retcode_out = XDC_NOK;
		}
	
		vl_retcode = vl_retcode_out;
	}	
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MSI_XZAO62_63 : sortie avec %d",vl_retcode_out);
	return(vl_retcode_out);
}


/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	cmvf_FctUti_MSI_XZAO118_119 : Fct utilisateur passee a xzao118_Lire_Config_Troncon,
*  qui appelle XZAO119_Ajouter_Troncon avec le nom de la base sur la quelle XZAO119_Ajouter_Troncon va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cmvf_FctUti_MSI_XZAO118_119(XZAOT_ConfTroncon       va_Troncon_in, XDY_Entier     va_Resultat_out) 
/*
* ARGUMENTS EN ENTREE :  va_Troncon_in contient le troncon a rajouter
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
*	Doit appeler XZAO119_Ajouter_Troncon
* 	
------------------------------------------------------*/
{
	static char *version 	= "$Id: cgcd_mSGBDihm.c,v 1.7 2016/07/13 18:16:44 devgfi Exp $ : cmvf_FctUti_MSI_XZAO118_119";
	char    vl_NomBase[4]	;
	int 	vl_retcode 	= XDC_OK;
	int 	vl_retcode_out 	= XDC_OK;
	char  	pl_Msg[500];			/* contient le mesg a tracer*/
	char  	pl_Msg2[500];			/* contient modification si vl_Resultat_out XDC_AJOUT,...*/
	char  	pl_NomProcs[80] ;
	
	XDY_ResConf vl_Resultat_out;
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MSI_XZAO118_119");
	
	strcpy( vl_NomBase,CGCDG_Base_Courante);	
	if ((vl_retcode =XZAO119_Ajouter_Troncon(vl_NomBase,va_Troncon_in, &vl_Resultat_out))!=XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSI_XZAO118_119 a echoue sur ligne %ld",(long)va_Resultat_out);
		vl_retcode_out = XDC_NOK;	
	}
	else
	/*A
	** Test sur parametres de sortie de XZAO119_Ajouter_Troncon sur coherence base
	** 4 valeurs de status d execution de la sp dans vl_Resultat_out
	** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
	*/
	{
		if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
		{
			strcpy(pl_NomProcs,"XZAO119_Ajouter_Troncon");
			XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld",pl_NomProcs,pl_Msg2,(long)va_Resultat_out);
			
			sprintf(pl_Msg,"%s %s sur la ligne %ld",XDF_CONF_TRONCON,pl_Msg2,(long)va_Resultat_out);	
			cuti_BilanConf_Trace(pl_Msg);	/* Notification dans Bilan config 	*/
			
		}
		else
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSI_XZAO118_119 : cmvf_Trace_TYPE_Result a echoue");
			vl_retcode_out = XDC_NOK;
		}
	
		vl_retcode = vl_retcode_out;
	}	
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MSI_XZAO118_119 : sortie avec %d",vl_retcode_out);
	return(vl_retcode_out);
}


