/*E*/
/*Fichier : $Id: xzas02.c,v 1.3 2020/11/03 17:57:24 pc2dpdy Exp $      Release : $Revision: 1.3 $        Date : $Date: 2020/11/03 17:57:24 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAS02 * FICHIER xzas02.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Ce module appartient a TACLI. Il permet d animer les courbes des synoptiques
*
------------------------------------------------------
* HISTORIQUE :
*
* gaborit	14/01/95	: Creation
* niepceron	23/09/96	: ajout de la duree de la plage (naa3) v1.2
* jMG		15/0�5/20 : EMS 1.3
------------------------------------------------------*/

/* fichiers inclus */

#include "xzas02.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: xzas02.c,v 1.3 2020/11/03 17:57:24 pc2dpdy Exp $ : xzas02" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	XZAS02_Inrets permet de demander l envoie des mesures Inrets
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAS02_Inrets (	XDY_Datagroup   va_datagroup_specif,
				XDY_Autoroute	va_Autoroute,
				XDY_PR		va_PR_Debut,
				XDY_PR		va_PR_Fin,
				XDY_Horodate	va_HorodateDebut,
				XDY_Octet	va_Plage) 
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
	static char *	version = "$Id: xzas02.c,v 1.3 2020/11/03 17:57:24 pc2dpdy Exp $ : XZAS02_Inrets" ;
	
	XDY_Datagroup   vl_datagroup;
	XDY_NomSite 	pl_NomSite;
        int		vl_retcode = XDC_NOK;
        
	XZST_03EcritureTrace(XZSTC_INTERFACE,"IN  : %s : Debut d execution sur DG %s ",
						version,va_datagroup_specif);
	/*A
	** Recuperation du site de la machine 
	*/
	if ( (vl_retcode = XZSC_04NomSite(pl_NomSite)) != XDC_OK)
	{
		/*B 
		** Ecriture Trace 
		*/
		XZST_03EcritureTrace( XZSTC_WARNING, "%s: Recuperation du Nom du site impossible .",version);
		return( XDC_NOK);
	} 	
	
	XZST_03EcritureTrace( XZSTC_DEBUG1, "%s: Nom du site %s.",version,pl_NomSite);
	
	/*A
	** Constitution du nom Data Group a partir du nom du site
	*/
	XDG_EncodeDG2(vl_datagroup,XDG_AS_COURBE,pl_NomSite);
	
	/*A
	** Envoi du message XDM_AS_INRETS. 
	*/ 
	if (TipcSrvMsgWrite(	vl_datagroup,
				XDM_IdentMsg(XDM_AS_INRETS) ,
				XDM_FLG_SRVMSGWRITE,
				T_IPC_FT_STR,	(T_STR) va_datagroup_specif,
				T_IPC_FT_CHAR,	(T_CHAR) va_Autoroute,
				T_IPC_FT_INT4,	(T_INT4) va_PR_Debut,
				T_IPC_FT_INT4,	(T_INT4) va_PR_Fin,
				T_IPC_FT_REAL8,	(T_REAL8) va_HorodateDebut,
				T_IPC_FT_INT2,  (T_INT2) va_Plage,
				NULL))
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1,"%s a reussi a envoyer le msg",version );	
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_WARNING ,"%s : TipcSrvMsgWrite a echoue",version);	
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : %s a echoue : Sortie",version);
		return(XDC_NOK);	 
	}
	/*A
	** Fin de l interface XZAS02
	*/
	XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT : %s : Fin d execution",version);
	return(XDC_OK);
}

