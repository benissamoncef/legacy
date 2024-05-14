/*E*/
/*Fichier : $Id: xzas06.c,v 1.9 1998/09/22 13:14:48 gaborit Exp $      Release : $Revision: 1.9 $        Date : $Date: 1998/09/22 13:14:48 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAS06 * FICHIER xzas06.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Ce module appartient a TACLI. Il permet d animer les courbes des synoptiques
*
------------------------------------------------------
* HISTORIQUE :
*
* mercier	09 Nov 1994	: Creation
* niepceron     11 Jan 1995     : Cast sur toutes les var du msg envoye 1.3
* MERCIER	17 Fev 1995	: Correction de l envoie de Msg RtWks	1.4
* on n utilise pas TipcMsgCreate
* niepceron     29 Aou 1996     : Ajout de XZAS06_Fonction_De_T_CI (RADT)  1.5
* niepceron     02 Jul 1998     : separation EXP/HIS (dem/1699)  1.9
------------------------------------------------------*/

/* fichiers inclus */

#include "xzas06.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: xzas06.c,v 1.9 1998/09/22 13:14:48 gaborit Exp $ : xzas06" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	XZAS06_Fonction_De_T permet de demander l envoie des mesures Traffic
*	6MINS  afin d animer les courbes des Synoptiques.
*	Algo : Constitue un msg RTwks et
* 	envoie le message RTWks XDM_AS_FdeT.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAS06_Fonction_De_T (	XDY_Datagroup   va_datagroup_specif,
				XDY_Eqt		va_NumStation,
				XDY_Sens	va_Sens,
				XDY_Voie	va_Voie,
				XDY_Horodate	va_HorodateDebut,
				XDY_Octet	va_TypeDeDonnees) 
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
	static char *	version = "$Id: xzas06.c,v 1.9 1998/09/22 13:14:48 gaborit Exp $ : XZAS06_Fonction_De_T" ;
	
	int			vl_JourSemaine=0;
	double			vl_HorodateSec=0;
	XDY_Datagroup   vl_datagroup;
	XDY_NomSite 	pl_NomSite;
        int		vl_retcode = XDC_NOK;
        
	XZST_03EcritureTrace(XZSTC_INTERFACE,"IN  : XZAS06_Fonction_De_T : Debut d execution");
	
	/*A
	** Recuperation du site de la machine 
	*/
	if ( (vl_retcode = XZSC_04NomSite(pl_NomSite)) != XDC_OK)
	{
		XZST_03EcritureTrace( XZSTC_WARNING, "XZAS06 : Recuperation du Nom du site impossible .");
		return( XDC_NOK);
	} 	
	
	XZST_03EcritureTrace( XZSTC_DEBUG1, "XZAS06 : Recuperation du Nom du site %s.",pl_NomSite);
	
 	if ((vl_retcode = XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME,&vl_JourSemaine,&vl_HorodateSec))!=XZSMC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"acli_axtimer_XZAG05_Purge_Base_Exploitation n a pu recuperer l horodate");
	}

	/*A
	** Constitution du nom Data Group a partir du nom du site
	*/
	if (va_HorodateDebut <= vl_HorodateSec-((XDC_OFFSET_EN_JOURS_PURGE_BASE-1)*24.*3600.))
		XDG_EncodeDG2(vl_datagroup,XDG_AS_COURBE,XDC_NOM_SITE_HIS);
	else
		XDG_EncodeDG2(vl_datagroup,XDG_AS_COURBE,pl_NomSite);
	
	XZST_03EcritureTrace ( XZSTC_DEBUG1, "XZAS06 : Data group a joindre : %s",vl_datagroup);

	/*A
	** Envoi du message XDM_AS_FdeT. 
	*/ 
	if (TipcSrvMsgWrite(	vl_datagroup,
				XDM_IdentMsg(XDM_AS_FdeT) ,
				XDM_FLG_SRVMSGWRITE,
				T_IPC_FT_STR,	(T_STR) va_datagroup_specif,
				T_IPC_FT_INT2,	(T_INT2) va_NumStation,
				T_IPC_FT_CHAR,	(T_CHAR) va_Sens,
				T_IPC_FT_CHAR,	(T_CHAR) va_Voie,
				T_IPC_FT_REAL8,	(T_REAL8) va_HorodateDebut,
				T_IPC_FT_CHAR,	(T_CHAR) va_TypeDeDonnees,
				NULL))
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1,"XZAS06 a reussi a envoyer le msg XDM_AS_FdeT." );	
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_WARNING ,"XZAS06_Fonction_De_T : TipcSrvMsgWrite a echoue");	
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAS06_Fonction_De_T a echoue : Sortie");
		return(XDC_NOK);	 
	}
	/*A
	** Fin de l interface XZAS06
	*/
	XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT : XZAS06_Fonction_De_T : Fin d execution");
	return(XDC_OK);
}


/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	XZAS06_Fonction_De_T_CI permet de demander au CI l envoie des mesures Traffic
*	6MINS  afin d animer les courbes des Synoptiques.
*	Algo : Constitue un msg RTwks et
* 	envoie le message RTWks XDM_AS_FdeT.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAS06_Fonction_De_T_CI (	XDY_Datagroup   va_datagroup_specif,
				XDY_Eqt		va_NumStation,
				XDY_Sens	va_Sens,
				XDY_Voie	va_Voie,
				XDY_Horodate	va_HorodateDebut,
				XDY_Octet	va_TypeDeDonnees) 
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
	static char *	version = "$Id: xzas06.c,v 1.9 1998/09/22 13:14:48 gaborit Exp $ : XZAS06_Fonction_De_T_CI" ;
	
	int			vl_JourSemaine=0;
	double			vl_HorodateSec=0;
	XDY_Datagroup   vl_datagroup;
	XDY_NomSite 	pl_NomSite;
        int		vl_retcode = XDC_NOK;
        
	XZST_03EcritureTrace(XZSTC_INTERFACE,"IN  : XZAS06_Fonction_De_T_CI : Debut d execution");
	
	/*A
	** Recuperation du site de la machine 
	*/
	if ( (vl_retcode = XZSC_04NomSite(pl_NomSite)) != XDC_OK)
	{
		XZST_03EcritureTrace( XZSTC_WARNING, "XZAS06 : Recuperation du Nom du site impossible .");
		return( XDC_NOK);
	} 	
	
	XZST_03EcritureTrace( XZSTC_DEBUG1, "XZAS06 : Recuperation du Nom du site %s.",pl_NomSite);
	
 	if ((vl_retcode = XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME,&vl_JourSemaine,&vl_HorodateSec))!=XZSMC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"acli_axtimer_XZAG05_Purge_Base_Exploitation n a pu recuperer l horodate");
	}

	/*A
	** Constitution du nom Data Group a partir du nom du site
	*/
	if (va_HorodateDebut <= vl_HorodateSec-((XDC_OFFSET_EN_JOURS_PURGE_BASE-1)*24.*3600.))
		XDG_EncodeDG2(vl_datagroup,XDG_AS_COURBE,XDC_NOM_SITE_HIS);
	else
		XDG_EncodeDG2(vl_datagroup,XDG_AS_COURBE,XDC_NOM_SITE_CI);
	
	XZST_03EcritureTrace ( XZSTC_DEBUG1, "XZAS06 : Data group a joindre : %s",vl_datagroup);

	/*A
	** Envoi du message XDM_AS_FdeT. 
	*/ 
	if (TipcSrvMsgWrite(	vl_datagroup,
				XDM_IdentMsg(XDM_AS_FdeT) ,
				XDM_FLG_SRVMSGWRITE,
				T_IPC_FT_STR,	(T_STR) va_datagroup_specif,
				T_IPC_FT_INT2,	(T_INT2) va_NumStation,
				T_IPC_FT_CHAR,	(T_CHAR) va_Sens,
				T_IPC_FT_CHAR,	(T_CHAR) va_Voie,
				T_IPC_FT_REAL8,	(T_REAL8) va_HorodateDebut,
				T_IPC_FT_CHAR,	(T_CHAR) va_TypeDeDonnees,
				NULL))
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1,"XZAS06 a reussi a envoyer le msg XDM_AS_FdeT." );	
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_WARNING ,"XZAS06_Fonction_De_T_CI : TipcSrvMsgWrite a echoue");	
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAS06_Fonction_De_T_CI a echoue : Sortie");
		return(XDC_NOK);	 
	}
	/*A
	** Fin de l interface XZAS06
	*/
	XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT : XZAS06_Fonction_De_T_CI : Fin d execution");
	return(XDC_OK);
}

