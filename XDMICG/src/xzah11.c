/*E*/
/*Fichier : $Id: xzah11.c,v 1.2 1995/02/17 18:06:29 mercier Exp $      Release : $Revision: 1.2 $        Date : $Date: 1995/02/17 18:06:29 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAH11 * FICHIER xzah11.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*	Module appartenant a BASEDD et declenchant un Callback de TACLI.
********************************
*	XZAH11_Init_IHM permet de reinitialiser la globale XZAHG_Init a XDC_VRAI.
*	Algo : Constitue un msg RTwks et
* 	envoie le message RTWks XDM_A_INIT_IHM.
*	Fonction permettant d envoyer le message XDM_A_INIT_IHM.
*	Le message RTwks ne contient pas d element.
*	Le message declenchera AHCL01 une CB de TACLI qui positionera la globale XZAHG_Init a XDC_VRAI.
* 
* 
*
------------------------------------------------------
* HISTORIQUE :
*
* mercier	28 Oct 1994	: Creation
**************
* MERCIER	17 Fev 1995	: Correction envoie de msg RtWks	V 1.2
* On enleve le TipcMsgCreate dont on ne se sert pas.
------------------------------------------------------*/

/* fichiers inclus */

#include "xzah11.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: xzah11.c,v 1.2 1995/02/17 18:06:29 mercier Exp $ : xzah11" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */
/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	XZAH11_Init_IHM permet de reinitialiser la globale XZAHG_Init a XDC_VRAI.
*	Algo : Constitue un msg RTwks et
* 	envoie le message RTWks XDM_A_INIT_IHM.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAH11_Init_IHM () 
/*
* ARGUMENTS EN ENTREE : aucun
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
*	Fonction permettant d envoyer le message XDM_A_INIT_IHM.
*	Le message RTwks ne contient pas d element.
*	Le message declenchera AHCL11 une CB de TACLI qui positionera la globale XZAH11G_Init a XDC_VRAI.
* 	
------------------------------------------------------*/
{
	static char *	version = "$Id: xzah11.c,v 1.2 1995/02/17 18:06:29 mercier Exp $ : XZAH11_Init_IHM" ;
	
	XDY_Datagroup   vl_datagroup;
	XDY_NomSite 	pl_NomSite;
        T_BOOL		vl_EtatDg = FALSE;
        int		vl_retcode = XDC_NOK;
        T_IPC_MSG 	pl_Msg;
        
	XZST_03EcritureTrace(XZSTC_INTERFACE,"IN  : XZAH11_Init_IHM : Debut d execution");

	/*A
	** Recuperation du site de la machine 
	*/
	if ( (vl_retcode = XZSC_04NomSite(pl_NomSite)) != XDC_OK)
	{
		XZST_03EcritureTrace( XZSTC_WARNING, "xzah11 : Recuperation du Nom du site impossible .");
		return( XDC_NOK);
	} 	
	/* 
	** Affichage Trace 
	*/
	XZST_03EcritureTrace( XZSTC_DEBUG1, "xzah11 : Recuperation du Nom du site %s.",pl_NomSite);
	
	/*A
	** Constitution du nom Data Group a partir du nom du site
	*/
	XDG_EncodeDG2(vl_datagroup,XDG_A_IHM,pl_NomSite);
	
	XZST_03EcritureTrace ( XZSTC_DEBUG1, "xzah11 : Data group a joindre : %s",vl_datagroup);

	/*A
	** Envoi du message XDM_A_INIT_IHM. 
	*/ 
	if (TipcSrvMsgWrite(vl_datagroup,XDM_IdentMsg(XDM_A_INIT_IHM) ,XDM_FLG_SRVMSGWRITE,NULL))
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1,"xzah11 a reussi a envoyer le msg XDM_A_INIT_IHM." );	
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_WARNING ,"XZAH11_Init_IHM : TipcSrvMsgWrite a echoue");	
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAH11_Init_IHM a echoue : Sortie");
		return(XDC_NOK);	 
	}
	/*A
	** Fin de l interface xzah11
	*/
	XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT : XZAH11_Init_IHM : Fin d execution");
	return(XDC_OK);
}


