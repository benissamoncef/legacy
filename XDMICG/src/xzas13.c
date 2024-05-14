/*E*/
/*Fichier : $Id: xzas13.c,v 1.1 1997/05/15 09:54:27 gaborit Exp $      Release : $Revision: 1.1 $        Date : $Date: 1997/05/15 09:54:27 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODTULE XZAS13 * FICHIER xzas13.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Ce module appartient a TACLI. Il permet d animer les courbes des synoptiques
*
------------------------------------------------------
* HISTORIQUE :
*
* Niepceron	03 Fev 1997	: Creation
------------------------------------------------------*/

/* fichiers inclus */

#include "xzas13.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: xzas13.c,v 1.1 1997/05/15 09:54:27 gaborit Exp $ : xzas13" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	XZAS13 _Init_TDP  permet de demander l envoie des mesures TDP
*	6MINS  afin d animer les courbes des Synoptiques.
*	Algo : Constitue un msg RTwks et
* 	envoie le message RTWks XDM_AS_TDP
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAS13_Init_TDP (	XDY_Datagroup   va_datagroup_specif,
				XDY_Eqt		va_NumZdp,
				XDY_Horodate	va_HorodateDebut) 
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
*	Fonction permettant d envoyer le message XDM_AS_FdeT.
*	Le message RTwks ne contient pas d element.
*	Le message declenchera AHCL11 une CB de TACLI qui positionera la globale XZAH11G_Init a XDC_VRAI.
* 	
------------------------------------------------------*/
{
	static char *	version = "$Id: xzas13.c,v 1.1 1997/05/15 09:54:27 gaborit Exp $ : XZAS13_TDP" ;
	
	XDY_Datagroup   vl_datagroup;
	XDY_NomSite 	pl_NomSite;
        int		vl_retcode = XDC_NOK;
        
	XZST_03EcritureTrace(XZSTC_INTERFACE,"IN  : XZAS13_TDP : Debut d execution");
	
	/*A
	** Recuperation du site de la machine 
	*/
	if ( (vl_retcode = XZSC_04NomSite(pl_NomSite)) != XDC_OK)
	{
		XZST_03EcritureTrace( XZSTC_WARNING, "XZAS13 : Recuperation du Nom du site impossible .");
		return( XDC_NOK);
	} 	
	
	XZST_03EcritureTrace( XZSTC_DEBUG1, "XZAS13 : Recuperation du Nom du site %s.",pl_NomSite);
	
	/*A
	** Constitution du nom Data Group a partir du nom du site
	*/
	XDG_EncodeDG2(vl_datagroup,XDG_AS_COURBE,pl_NomSite);
	
	XZST_03EcritureTrace ( XZSTC_DEBUG1, "XZAS13 : Data group a joindre : %s",vl_datagroup);

	/*A
	** Envoi du message XDM_AS_TDP. 
	*/ 
	if (TipcSrvMsgWrite(	vl_datagroup,
				XDM_IdentMsg(XDM_AS_TDP) ,
				XDM_FLG_SRVMSGWRITE,
				T_IPC_FT_STR,	(T_STR) va_datagroup_specif,
				T_IPC_FT_INT2,	(T_INT2) va_NumZdp,
				T_IPC_FT_REAL8,	(T_REAL8) va_HorodateDebut,
				NULL))
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1,"XZAS13 a reussi a envoyer le msg XDM_AS_FdeT." );	
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_WARNING ,"XZAS13_TDP : TipcSrvMsgWrite a echoue");	
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAS13_TDP a echoue : Sortie");
		return(XDC_NOK);	 
	}
	/*A
	** Fin de l interface XZAS13
	*/
	XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT : XZAS13_TDP : Fin d execution");
	return(XDC_OK);
}


/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	XZAS13 _Init_TDP  permet de demander l envoie des mesures TDP
*	6MINS  afin d animer les courbes des Synoptiques.
*	Algo : Constitue un msg RTwks et
* 	envoie le message RTWks XDM_AS_TDP
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAS13_Init_TDP_CI (	XDY_Datagroup   va_datagroup_specif,
				XDY_Eqt		va_NumZdp,
				XDY_Horodate	va_HorodateDebut) 
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
*	Fonction permettant d envoyer le message XDM_AS_FdeT.
*	Le message RTwks ne contient pas d element.
*	Le message declenchera AHCL11 une CB de TACLI qui positionera la globale XZAH11G_Init a XDC_VRAI.
* 	
------------------------------------------------------*/
{
	static char *	version = "$Id: xzas13.c,v 1.1 1997/05/15 09:54:27 gaborit Exp $ : XZAS13_TDP_CI" ;
	
	XDY_Datagroup   vl_datagroup;
	XDY_NomSite 	pl_NomSite;
        int		vl_retcode = XDC_NOK;
        
	XZST_03EcritureTrace(XZSTC_INTERFACE,"IN  : XZAS13_TDP_CI : Debut d execution");
	
	/*A
	** Recuperation du site de la machine 
	*/
	if ( (vl_retcode = XZSC_04NomSite(pl_NomSite)) != XDC_OK)
	{
		XZST_03EcritureTrace( XZSTC_WARNING, "XZAS13 : Recuperation du Nom du site impossible .");
		return( XDC_NOK);
	} 	
	
	XZST_03EcritureTrace( XZSTC_DEBUG1, "XZAS13 : Recuperation du Nom du site %s.",pl_NomSite);
	
	/*A
	** Constitution du nom Data Group a partir du nom du site
	*/
	XDG_EncodeDG2(vl_datagroup,XDG_AS_COURBE,XDC_NOM_SITE_CI);
	
	XZST_03EcritureTrace ( XZSTC_DEBUG1, "XZAS13 : Data group a joindre : %s",vl_datagroup);

	/*A
	** Envoi du message XDM_AS_TDP. 
	*/ 
	if (TipcSrvMsgWrite(	vl_datagroup,
				XDM_IdentMsg(XDM_AS_TDP) ,
				XDM_FLG_SRVMSGWRITE,
				T_IPC_FT_STR,	(T_STR) va_datagroup_specif,
				T_IPC_FT_INT2,	(T_INT2) va_NumZdp,
				T_IPC_FT_REAL8,	(T_REAL8) va_HorodateDebut,
				NULL))
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1,"XZAS13 a reussi a envoyer le msg XDM_AS_FdeT." );	
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_WARNING ,"XZAS13_TDP : TipcSrvMsgWrite a echoue");	
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAS13_TDP a echoue : Sortie");
		return(XDC_NOK);	 
	}
	/*A
	** Fin de l interface XZAS13
	*/
	XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT : XZAS13_TDP_CI : Fin d execution");
	return(XDC_OK);
}

