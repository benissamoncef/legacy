/*E*/
/*Fichier : $Id: cgcd_mvf_util.c,v 1.6 2016/07/13 20:47:46 devgfi Exp $      Release : $Revision: 1.6 $        Date : $Date: 2016/07/13 20:47:46 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  GECONF
------------------------------------------------------
* MODULE MMVF_UTIL * FICHIER cgcd_mvf_util.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*	MMVF_UTIL : contient des utilitaires pour MMVF
***********************
* 	cmvf_Trace_TYPE_Result : fonction permettant de construire le message de trace 
* Bilan Config et de tester le mode d execution des Fct du type XZAO51 appelee 
* par les fcts utilisateurs.
* Cette fonction n est appelee que par les fct utilisateurs.
*********************** 
*
------------------------------------------------------
* HISTORIQUE :
*
* mercier	21 Oct 1994	: Creation
*******************
* mercier			: Rien			V1.2
*******************
* mercier	04 Nov	1994	: Ajout 		V1.3
* 		du cas defaut pour cmvf_Trace_TYPE_Result
* niepceron	01 Mar	1996	: correction accents v1.5
* JPL		13/07/16	: Nom du label "default" du "switch"  1.6
------------------------------------------------------*/

/* fichiers inclus */

#include "cgcd_mvf_util.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: cgcd_mvf_util.c,v 1.6 2016/07/13 20:47:46 devgfi Exp $ : cgcd_mvf_util" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */
/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
* cmvf_Trace_TYPE_Result : fonction permettant de construire le message de trace Bilan Config
* et de tester le mode d execution des Fct du type XZAO51 appelee par les fcts utilisateurs.
* Cette fonction n est appelee que par les fct utilisateurs.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
extern int cmvf_Trace_TYPE_Result(XDY_ResConf va_Resultat_in, char * pa_Msg_out, int *pa_retcode_out) 
/*
* ARGUMENTS EN ENTREE :  va_Resultat_in contient le status d execution de la SP.	
* ARGUMENTS EN SORTIE :  pa_Msg_out va contenir le message a trace 
*			 pa_retcode_out va contenir le code a renvoyer en fonction de va_Resultat_in
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
* CONDITION D'UTILISATION
*
* FONCTION
*	Si  va_Resultat_in == XDC_ECHEC, pa_retcode_out = XDC_NOK;
*	et le message construit stipulera l echec d execution de la SP.
*	sinon en fonction va_Resultat_in on construira un message parlant : on a effectue une modification,
*	un ajout ou une suppression.	
------------------------------------------------------*/
{
	static char *version 	= "$Id: cgcd_mvf_util.c,v 1.6 2016/07/13 20:47:46 devgfi Exp $ : cmvf_Trace_TYPE_Result";
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_Trace_TYPE_Result a recu %ld pour construire la trace",(long)va_Resultat_in);
	switch (va_Resultat_in)
	{
		case XDC_ECHEC : 
			*pa_retcode_out = XDC_NOK;
			sprintf(pa_Msg_out,"a echoue sur la tentative de mise dans la base %s",CGCDG_Base_Courante);
			break;
		case XDC_SUPPR : 
			*pa_retcode_out = XDC_OK;
			sprintf(pa_Msg_out,"a reussi la suppression dans la base %s",CGCDG_Base_Courante);
			break;
		case XDC_MODIF : 
			*pa_retcode_out = XDC_OK;
			sprintf(pa_Msg_out,"a reussi la modification dans la base %s",CGCDG_Base_Courante);
			break;	
		case XDC_AJOUT : 
			*pa_retcode_out = XDC_OK;
			sprintf(pa_Msg_out,"a reussi l'ajout dans la base %s",CGCDG_Base_Courante);
			break;	
		case XDC_LU : 
			*pa_retcode_out = XDC_OK;
			sprintf(pa_Msg_out,"n'a pas eu a effectuer de modification dans la base %s",CGCDG_Base_Courante);
			break;	
		default	:
			*pa_retcode_out = XDC_NOK;
			sprintf(pa_Msg_out,"a echoue sur la tentative de mise dans la base %s en renvoyant un type d'action %d inconnu",CGCDG_Base_Courante,va_Resultat_in);
			break;
	}/* Fin du switch */
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT  : cmvf_Trace_TYPE_Result : Fin d execution");

	return(XDC_OK);	
}


