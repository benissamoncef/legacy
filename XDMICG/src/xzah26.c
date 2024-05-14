/*E*/
/*Fichier : $Id: xzah26.c,v 1.2 1995/02/17 18:08:35 mercier Exp $      Release : $Revision: 1.2 $        Date : $Date: 1995/02/17 18:08:35 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAH * FICHIER xzah26.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Ce module appartient a TACLI. Il permet de demander l'init voies RADT et DAI
*
------------------------------------------------------
* HISTORIQUE :
*
* gaborit	11 Jan 1995     : creation
*****
* Mercier	17 Fev 1995	: Correction de l envoie de msg RtWks	v 1.2
* on ne se sert pas de TipcMsgCreate
------------------------------------------------------*/

/* fichiers inclus */

#include "xzah26.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: xzah26.c,v 1.2 1995/02/17 18:08:35 mercier Exp $ : xzah26" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	XZAH26_InitRadtDai
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAH26_InitRadtDai (	XDY_NomMachine    va_NomMachine_in) 
/*
* ARGUMENTS EN ENTREE : XDY_Machine    va_NomMachine_in : nom de la station LT qui demande l init
*
* ARGUMENTS EN SORTIE : aucun
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon renvoie XDC_NOK.
*
* CONDITION D'UTILISATION
*
*	Il faut etre connecte au RTserveur local.
*	
*
* FONCTION 
* 	
------------------------------------------------------*/
{
	static char *	version = "$Id: xzah26.c,v 1.2 1995/02/17 18:08:35 mercier Exp $ : xzah26" ;
	
	XDY_Datagroup   vl_datagroup;
	XDY_NomSite 	pl_NomSite;
        int		vl_retcode = XDC_NOK;
        T_IPC_MSG 	pl_Msg;
        
	XZST_03EcritureTrace(XZSTC_INTERFACE,"IN  : %s : Debut d execution pour machine %s",version,va_NomMachine_in);
	
	/*A
	** Recuperation du site de la machine 
	*/
	if ( (vl_retcode = XZSC_04NomSite(pl_NomSite)) != XDC_OK)
	{
		/*B 
		** Ecriture Trace 
		*/
		XZST_03EcritureTrace( XZSTC_WARNING, "%s : Recuperation du Nom du site impossible .",version);
		return( XDC_NOK);
	} 	
	
	XZST_03EcritureTrace( XZSTC_DEBUG1, "%s : Recuperation du Nom du site %s.",version,pl_NomSite);
	
	/*A
	** Constitution du nom Data Group a partir du nom du site
	*/
	XDG_EncodeDG2(vl_datagroup,XDG_AH_INITLT,pl_NomSite);
	
	XZST_03EcritureTrace ( XZSTC_DEBUG1, "%s : Data group a joindre : %s",version,vl_datagroup);

	/*A
	** Envoi du message XDM_AH_INITLT. 
	*/ 
	if (TipcSrvMsgWrite(	vl_datagroup,
				XDM_IdentMsg(XDM_AH_INITLT) ,
				XDM_FLG_SRVMSGWRITE,
				T_IPC_FT_STR,	(T_STR) va_NomMachine_in,
				NULL))
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1,"%s a reussi a envoyer le msg XDM_AH_INITLT.",version );	
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_WARNING ,"%s : TipcSrvMsgWrite a echoue",version);	
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : %s a echoue : Sortie",version);
		return(XDC_NOK);	 
	}
	/*A
	** Fin de l interface XZAH26
	*/
	XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT : %s : Fin d execution",version);
	return(XDC_OK);
}

