								/*E*/
/*Fichier :  $Id: xzat.c,v 1.20 2021/06/10 08:45:48 pc2dpdy Exp $      Release : $Revision: 1.20 $        Date : $Date: 2021/06/10 08:45:48 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAT * FICHIER xzat.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Contient les fonction d interfaces qui envoie un message RTwks
* afin de declencher dans un Open Client l 'execution d une Stored Procedure 
* sur le Sql_Serveur: Ces fonctions constituent apartir des parametres recus, le msg
* RTwks et l envoie au RTserveur.
**********************
*	xzat22_CompteRendu_Tube permet de mettre le compte rendu
*	d une action en base.
*	Algo : Recoit les parametres, 
*	constitue un msg RTwks et
* 	envoie le message RTWks XDM_AT_TUBE.
**********************
*	xzat23_CompteRendu_DemiEch permet de mettre le compte rendu
*	d une action en base.
*	Algo : Recoit les parametres, 
*	constitue un msg RTwks et
* 	envoie le message RTWks XDM_AT_echangeur.
***********************
*	xzat24_CompteRendu_Nav permet de mettre le compte rendu
*	d une action en base.
*	Algo : Recoit les parametres, 
*	constitue un msg RTwks et
* 	envoie le message RTWks XDM_AT_NAV.
***********************
*	xzat25_CompteRendu_PMV permet de mettre le compte rendu
*	d une action en base.
*	Algo : Recoit les parametres, 
*	constitue un msg RTwks et
* 	envoie le message RTWks XDM_AT_PMV.
***********************
*	xzat26_Init_Etats_Eqts permet de retourner l'etat des
*	equipements .
*	Algo : Recoit les parametres, 
*	constitue un msg RTwks et
* 	envoie le message RTWks XDM_AT_INI_EQT.
***********************
*	xzat27_Init_Etats_EqtsIhm permet de retourner l'etat des
*	equipements a l'ihm synoptique.
*	Algo : Recoit les parametres, 
*	constitue un msg RTwks et
* 	envoie le message RTWks XDM_AT_INI_IHM.
------------------------------------------------------
* HISTORIQUE :
*
* mercier	10 Oct 1994	: Creation
**********
* mercier	11 Oct 1994	: Ajout de xzat23_CompteRendu_DemiEch
					de xzat24_CompteRendu_Nav
					de xzat25_CompteRendu_PMV
**********
* Torregrossa	17 Fev 1995	: Ajout de xzat26_Init_Etats_Eqt
*                                          xzat27_Init_Etats_EqtIhm
**********
* Mercier	17 Fev 1995	: Correction de l envoie de Msg 	V 1.5
* RtWks : on n utilise pas TipcMsgCreate
**********
* Torregrossa	20 Fev 1995	: Modif XDM_AT_INI_EQT.
*                                       XDM_AT_INI_IHM.			V 1.6
* Guilhou	27 aut 1996 	: Modif XDM_AT_NAV, PMV, TUN, ECH (DEM/1172) v1.7
* Mismer	29 Jan 1998 	: Ajout compte rendu PAL (DEM/1539) v1.8
* JMG		13/09/05	: ajout xzat250 1.9
* JMG		28/09/05	: ajout xzat30 et xzat31 1.10
* JMG   13/11/07        : ajout site de gestion DEM715
* AAZ       30/08/2007  : DEM 662 ajout xzat40_CompteRendu_SIG
*                               xzat41_CompteRendu_ECL
*                               xzat42_CompteRendu_VEN
*                               xzat43_CompteRendu_ENR
*                               xzat44_CompteRendu_INC
*                               xzat45_CompteRendu_EXP
* JMG      29/09/09            : SECTO   DEM 887
* VR	   09/12/11 		   : Ajout PRV (DEM/1016)
* JMG		16/04/16 ajout SAGA 1.15
* JMG   09/03/17        : REGIONALISATION - DEM1220 1.15
* LCL	26/02/18	: Ajout CFE CTRL_FEUX 1.16 DEM1284
* JMG	12/01/20	: ajout IMU 1.17
* ABE	16/12/20	: Changemen nom fonction imu DEM-SAE155 
* ABE	02/04/21	: Ajout PAU SONO DEM-SAE244 1.20
* LCL	27/09/23	: Adaptations CNA DEM-483
* GGY	14/11/23	: Ajout PIC DEM-483
------------------------------------------------------*/

/* fichiers inclus */

#include "xzat.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: xzat.c,v 1.20 2021/06/10 08:45:48 pc2dpdy Exp $ : xzat" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	xzat22_CompteRendu_Tube permet de mettre le compte rendu
*	d une action en base.
*	Algo : Recoit les parametres, 
*	constitue un msg RTwks et
* 	envoie le message RTWks XDM_AT_TUBE.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int xzat22_CompteRendu_Tube (	XDY_Entier	va_Num_Action,
				XDY_Horodate 	va_Horodate_double,
				XDY_Booleen  	va_ResultatAction) 
/*
* ARGUMENTS EN ENTREE :
*
*   	Arguments d'une callback RTServer.
*
* ARGUMENTS EN SORTIE :
*
*   aucun
*
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
*	Fonction permettant d envoyer le message XDM_AT_TUBE.
*	Le message RTwks doit contenir un XDY_Mot,XDY_Horodate,XDY_Booleen.
*	Le message declenchera XZAT;22 une SP n effectuant pas de select et ne prenant pas
*	de parametre de retour.
* 	
------------------------------------------------------*/
{
	static char *	version = "$Id: xzat.c,v 1.20 2021/06/10 08:45:48 pc2dpdy Exp $ : xzat22_CompteRendu_Tube" ;
	
	XDY_Datagroup   vl_datagroup;
	XDY_NomSite 	pl_NomSite;
        T_BOOL		vl_EtatDg = FALSE;
        int		vl_retcode = XDC_NOK;
        
	XZST_03EcritureTrace(XZSTC_INTERFACE,"IN  : xzat22_CompteRendu_Tube : Debut d execution\n a recu Num_Action = %d\n Horodate = %lf\n ResultatAction = %d",va_Num_Action,va_Horodate_double,va_ResultatAction);
	
	/*A
	** Recuperation du site de la machine 
	*/
	if ( (vl_retcode = XZSC_04NomSite(pl_NomSite)) != XDC_OK)
	{
		XZST_03EcritureTrace( XZSTC_WARNING, "xzat22 : Recuperation du Nom du site impossible .");
		return( XDC_NOK);
	} 
	
	XZST_03EcritureTrace( XZSTC_DEBUG1, "xzat22 : Recuperation du Nom du site %s.",pl_NomSite);
	
	/*A
	** Constitution du nom Data Group a partir du nom du site
	*/
	XDG_EncodeDG2(vl_datagroup,XDG_AT,pl_NomSite); 
	
	XZST_03EcritureTrace ( XZSTC_DEBUG1, "xzat22 : Data group a joindre : %s",vl_datagroup);
	
	
	/*A
	** Envoi du message XDM_AT_TUBE 
	*/ 
	if (TipcSrvMsgWrite(vl_datagroup,XDM_IdentMsg(XDM_AT_TUBE) ,
				XDM_FLG_SRVMSGWRITE,
				T_IPC_FT_INT4, va_Num_Action, 
				T_IPC_FT_REAL8,va_Horodate_double, 
				T_IPC_FT_CHAR, va_ResultatAction, 
				NULL))
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1,"xzat22 a reussi a envoyer le msg XDM_AT_TUBE" );	
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_WARNING ,"xzat22_CompteRendu_Tube : TipcSrvMsgWrite a echoue");	
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : xzat22_CompteRendu_Tube a echoue : Sortie");
		return(XDC_NOK);	 
	}
	/*A
	** Fin de l interface xzat22
	*/
	XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT : xzat22_CompteRendu_Tube : Fin d execution");
	return(XDC_OK);
}
/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	xzat23_CompteRendu_DemiEch permet de mettre le compte rendu
*	d une action en base.
*	Algo : Recoit les parametres, 
*	constitue un msg RTwks et
* 	envoie le message RTWks XDM_AT_echangeur.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int xzat23_CompteRendu_DemiEch (	XDY_Entier	va_Num_Action,
				XDY_Horodate 	va_Horodate_double,
				XDY_Booleen  	va_ResultatAction
				) 
/*
* ARGUMENTS EN ENTREE :
*
*   	Arguments d'une callback RTServer.
*
* ARGUMENTS EN SORTIE :
*
*   aucun
*
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
*	Fonction permettant d envoyer le message XDM_AT_echangeur.
*	Le message RTwks doit contenir un XDY_Mot,XDY_Horodate,XDY_Booleen.
*	Le message declenchera XZAT;23 une SP n effectuant pas de select et ne prenant pas
*	de parametre de retour.
* 	
------------------------------------------------------*/
{
	static char *	version = "$Id: xzat.c,v 1.20 2021/06/10 08:45:48 pc2dpdy Exp $ : xzat23_CompteRendu_DemiEch" ;
	
	XDY_Datagroup   vl_datagroup;
	XDY_NomSite 	pl_NomSite;
        T_BOOL		vl_EtatDg = FALSE;
        int		vl_retcode = XDC_NOK;
        
	XZST_03EcritureTrace(XZSTC_INTERFACE,"IN  : xzat23_CompteRendu_DemiEch : Debut d execution\n a recu Num_Action = %d\n Horodate = %lf\n ResultatAction = %d",va_Num_Action,va_Horodate_double,va_ResultatAction);
	
	/*A
	** Recuperation du site de la machine 
	*/
	if ( (vl_retcode = XZSC_04NomSite(pl_NomSite)) != XDC_OK)
	{
		XZST_03EcritureTrace( XZSTC_WARNING, "xzat23 : Recuperation du Nom du site impossible .");
		return( XDC_NOK);
	} 
	
	XZST_03EcritureTrace( XZSTC_DEBUG1, "xzat23 : Recuperation du Nom du site %s.",pl_NomSite);
	
	/*A
	** Constitution du nom Data Group a partir du nom du site
	*/
	
	 XDG_EncodeDG2(vl_datagroup,XDG_AT,pl_NomSite);
	
	XZST_03EcritureTrace ( XZSTC_DEBUG1, "xzat23 : Data group a joindre : %s",vl_datagroup);
	
	
	/*A
	** Envoi du message XDM_AT_echangeur 
	*/ 
	if (TipcSrvMsgWrite(vl_datagroup,XDM_IdentMsg(XDM_AT_echangeur) ,
				XDM_FLG_SRVMSGWRITE,
				T_IPC_FT_INT4, va_Num_Action, 
				T_IPC_FT_REAL8,va_Horodate_double, 
				T_IPC_FT_CHAR, va_ResultatAction, 
				NULL))
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1,"xzat23 a reussi a envoyer le msg XDM_AT_echangeur" );	
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_WARNING ,"xzat23_CompteRendu_DemiEch : TipcSrvMsgWrite a echoue");	
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : xzat23_CompteRendu_DemiEch a echoue : Sortie");
		return(XDC_NOK);	 
	}
	/*A
	** Fin de l interface xzat23
	*/
	XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT : xzat23_CompteRendu_DemiEch : Fin d execution");
	return(XDC_OK);
}
/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	xzat24_CompteRendu_Nav permet de mettre le compte rendu
*	d une action en base.
*	Algo : Recoit les parametres, 
*	constitue un msg RTwks et
* 	envoie le message RTWks XDM_AT_NAV.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int xzat24_CompteRendu_Nav (	XDY_Entier	va_Num_Action,
				XDY_Horodate 	va_Horodate_double,
				XDY_Booleen  	va_ResultatAction,
				XDY_District va_SiteGestion) 
/*
* ARGUMENTS EN ENTREE :
*
*   	Arguments d'une callback RTServer.
*
* ARGUMENTS EN SORTIE :
*
*   aucun
*
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
*	Fonction permettant d envoyer le message XDM_AT_NAV.
*	Le message RTwks doit contenir un XDY_Mot,XDY_Horodate,XDY_Booleen.
*	Le message declenchera XZAT;23 une SP n effectuant pas de select et ne prenant pas
*	de parametre de retour.
* 	
------------------------------------------------------*/
{
	static char *	version = "$Id: xzat.c,v 1.20 2021/06/10 08:45:48 pc2dpdy Exp $ : xzat24_CompteRendu_Nav" ;
	
	XDY_Datagroup   vl_datagroup;
	XDY_NomSite 	pl_NomSite;
        T_BOOL		vl_EtatDg = FALSE;
        int		vl_retcode = XDC_NOK;
        
	XZST_03EcritureTrace(XZSTC_INTERFACE,"IN  : xzat24_CompteRendu_Nav : Debut d execution\n a recu Num_Action = %d\n Horodate = %lf\n ResultatAction = %d",va_Num_Action,va_Horodate_double,va_ResultatAction);
	
	/*A
	** Recuperation du site de la machine 
	*/
	if ( (vl_retcode = XZSC_04NomSite(pl_NomSite)) != XDC_OK)
	{
		XZST_03EcritureTrace( XZSTC_WARNING, "xzat24_CompteRendu_Nav : Recuperation du Nom du site impossible .");
		return( XDC_NOK);
	} 
	
	XZST_03EcritureTrace( XZSTC_DEBUG1, "xzat24_CompteRendu_Nav : Recuperation du Nom du site %s.",pl_NomSite);
	
	/*A
	** Constitution du nom Data Group a partir du nom du site
	*/
	
    switch (va_SiteGestion) 
	{
	 	case (XDC_CI) : XDG_EncodeDG2(vl_datagroup,XDG_AT,XDC_NOM_SITE_CI);
			      break;
	 	case (XDC_VC) : XDG_EncodeDG2(vl_datagroup,XDG_AT,XDC_NOM_SITE_VC);
			      break;
	 	case (XDC_DP) : XDG_EncodeDG2(vl_datagroup,XDG_AT,XDC_NOM_SITE_DP);
			      break;
	 	case (XDC_CA) : XDG_EncodeDG2(vl_datagroup,XDG_AT,XDC_NOM_SITE_CA);
			      break;
	 	default: XDG_EncodeDG2(vl_datagroup,XDG_AT,pl_NomSite);
				break;
	}
	XZST_03EcritureTrace ( XZSTC_DEBUG1, "xzat24_CompteRendu_Nav : Data group a joindre : %s",vl_datagroup);
	
	
	/*A
	** Envoi du message XDM_AT_NAV 
	*/ 
	if (TipcSrvMsgWrite(vl_datagroup,XDM_IdentMsg(XDM_AT_NAV) ,
				XDM_FLG_SRVMSGWRITE,
				T_IPC_FT_INT4, va_Num_Action, 
				T_IPC_FT_REAL8,va_Horodate_double, 
				T_IPC_FT_CHAR, va_ResultatAction, 
				NULL))
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1,"xzat24_CompteRendu_Nav a reussi a envoyer le msg XDM_AT_NAV" );	
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_WARNING ,"xzat24_CompteRendu_Nav : TipcSrvMsgWrite a echoue");	
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : xzat24_CompteRendu_Nav a echoue : Sortie");
		return(XDC_NOK);	 
	}
	/*A
	** Fin de l interface xzat24_CompteRendu_Nav
	*/
	XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT : xzat24_CompteRendu_Nav : Fin d execution");
	return(XDC_OK);
}
/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	xzat25_CompteRendu_PMV permet de mettre le compte rendu
*	d une action en base.
*	Algo : Recoit les parametres, 
*	constitue un msg RTwks et
* 	envoie le message RTWks XDM_AT_PMV.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int xzat25_CompteRendu_PMV (	XDY_Entier	va_Num_Action,
				XDY_Horodate 	va_Horodate_double,
				XDY_Booleen  	va_ResultatAction,
				XDY_District va_SiteGestion) 
/*
* ARGUMENTS EN ENTREE :
*
*   	Arguments d'une callback RTServer.
*
* ARGUMENTS EN SORTIE :
*
*   aucun
*
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
*	Fonction permettant d envoyer le message XDM_AT_PMV.
*	Le message RTwks doit contenir un XDY_Mot,XDY_Horodate,XDY_Booleen.
*	Le message declenchera XZAT;23 une SP n effectuant pas de select et ne prenant pas
*	de parametre de retour.
* 	
------------------------------------------------------*/
{
	static char *	version = "$Id: xzat.c,v 1.20 2021/06/10 08:45:48 pc2dpdy Exp $ : xzat25_CompteRendu_PMV" ;
	
	XDY_Datagroup   vl_datagroup;
	XDY_NomSite 	pl_NomSite;
        T_BOOL		vl_EtatDg = FALSE;
        int		vl_retcode = XDC_NOK;
        
	XZST_03EcritureTrace(XZSTC_WARNING,"IN  : xzat25_CompteRendu_PMV : Debut d execution\n a recu Num_Action = %d\n Horodate = %lf\n ResultatAction = %d",va_Num_Action,va_Horodate_double,va_ResultatAction);
	
	/*A
	** Recuperation du site de la machine 
	*/
	if ( (vl_retcode = XZSC_04NomSite(pl_NomSite)) != XDC_OK)
	{
		XZST_03EcritureTrace( XZSTC_WARNING, "xzat25_CompteRendu_PMV : Recuperation du Nom du site impossible .");
		return( XDC_NOK);
	} 

	XZST_03EcritureTrace( XZSTC_WARNING, "xzat25_CompteRendu_PMV : Recuperation du Nom du site %s.",pl_NomSite);
	
	/*A
	** Constitution du nom Data Group a partir du nom du site
	*/
    switch (va_SiteGestion) 
	{
	 	case (XDC_CI) : XDG_EncodeDG2(vl_datagroup,XDG_AT,XDC_NOM_SITE_CI);
			      break;
	 	case (XDC_VC) : XDG_EncodeDG2(vl_datagroup,XDG_AT,XDC_NOM_SITE_VC);
			      break;
	 	case (XDC_DP) : XDG_EncodeDG2(vl_datagroup,XDG_AT,XDC_NOM_SITE_DP);
			      break;
	 	case (XDC_CA) : XDG_EncodeDG2(vl_datagroup,XDG_AT,XDC_NOM_SITE_CA);
			      break;
	 	default: XDG_EncodeDG2(vl_datagroup,XDG_AT,pl_NomSite);
				break;
	}
	XZST_03EcritureTrace ( XZSTC_WARNING, "xzat25_CompteRendu_PMV : Data group a joindre : %s",vl_datagroup);
	

	/*A
	** Envoi du message XDM_AT_PMV 
	*/ 
	if (TipcSrvMsgWrite(vl_datagroup,XDM_IdentMsg(XDM_AT_PMV) ,
				XDM_FLG_SRVMSGWRITE,
				T_IPC_FT_INT4, va_Num_Action, 
				T_IPC_FT_REAL8,va_Horodate_double, 
				T_IPC_FT_CHAR, va_ResultatAction, 
				NULL))
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzat25_CompteRendu_PMV a reussi a envoyer le msg XDM_AT_PMV" );	
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_WARNING ,"xzat25_CompteRendu_PMV : TipcTipcSrvMsgWriteMsgWrite a echoue");	
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : xzat25_CompteRendu_PMV a echoue : Sortie");
		return(XDC_NOK);	 
	}
	/*A
	** Fin de l interface xzat25_CompteRendu_PMV
	*/
	XZST_03EcritureTrace(XZSTC_WARNING,"OUT : xzat25_CompteRendu_PMV : Fin d execution");
	return(XDC_OK);
}
/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	xzat26_Init_Etats_Eqts permet de retourner l'etat des
*	equipements.
*	Algo : Recoit les parametres, 
*	constitue un msg RTwks et
* 	envoie le message RTWks XDM_AT_INI_EQT.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int xzat26_Init_Etats_Eqts (	XDY_Octet	va_Type,
				XDY_Entier 	va_Etats,
				XDY_NomMachine 	va_LocalTechnique) 
/*
* ARGUMENTS EN ENTREE :
*
*   	Arguments d'une callback RTServer.
*
* ARGUMENTS EN SORTIE :
*
*   aucun
*
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
*	Fonction permettant d envoyer le message XDM_AT_INI_EQT.
*	Le message RTwks doit contenir un XDY_Octet,XDY_Entier,XDY_NomMachine.
*	Le message declenchera XZAT;26 une SP effectuant un select et ne prenant pas
*	de parametre de retour.
* 	
------------------------------------------------------*/
{
	static char *	version = "$Id: xzat.c,v 1.20 2021/06/10 08:45:48 pc2dpdy Exp $ : xzat26_Init_Etats_Eqts" ;
	
	XDY_Datagroup   vl_datagroup;
	XDY_NomSite 	pl_NomSite;
        T_BOOL		vl_EtatDg = FALSE;
        int		vl_retcode = XDC_NOK;
        
	if (va_LocalTechnique != NULL)
		XZST_03EcritureTrace(XZSTC_INTERFACE,"IN  : xzat27_Init_Etats_EqtsIhm : Debut d execution\n a recu Type = %d\n Etats = %d\n LocalTechnique = %s",va_Type,va_Etats,va_LocalTechnique);
	else
		XZST_03EcritureTrace(XZSTC_INTERFACE,"IN  : xzat27_Init_Etats_EqtsIhm : Debut d execution\n a recu Type = %d\n Etats = %d\n LocalTechnique = NULL",va_Type,va_Etats);

	/*A
	** Recuperation du site de la machine 
	*/
	if ( (vl_retcode = XZSC_04NomSite(pl_NomSite)) != XDC_OK)
	{
		XZST_03EcritureTrace( XZSTC_WARNING, "xzat26_Init_Etats_Eqts : Recuperation du Nom du site impossible .");
		return( XDC_NOK);
	} 
	
	XZST_03EcritureTrace( XZSTC_DEBUG1, "xzat26_Init_Etats_Eqts : Recuperation du Nom du site %s.",pl_NomSite);
	
	/*A
	** Constitution du nom Data Group a partir du nom du site
	*/
	XDG_EncodeDG2(vl_datagroup,XDG_AT,pl_NomSite);
	
	XZST_03EcritureTrace ( XZSTC_DEBUG1, "xzat26_Init_Etats_Eqts : Data group a joindre : %s",vl_datagroup);
	
	
	/*A
	** Envoi du message XDM_AT_INI_EQT 
	*/ 
	if (TipcSrvMsgWrite(vl_datagroup,XDM_IdentMsg(XDM_AT_INI_EQT) ,
				XDM_FLG_SRVMSGWRITE,
				T_IPC_FT_INT2, va_Type, 
				T_IPC_FT_INT4,va_Etats, 
				T_IPC_FT_STR, va_LocalTechnique, 
				NULL))
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1,"xzat26_Init_Etats_Eqts a reussi a envoyer le msg XDM_AT_INI_EQT" );	
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_WARNING ,"xzat26_Init_Etats_Eqts : TipcSrvMsgWrite a echoue");	
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : xzat26_Init_Etats_Eqts a echoue : Sortie");
		return(XDC_NOK);	 
	}
	/*A
	** Fin de l interface xzat26_Init_Etats_Eqts
	*/
	XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT : xzat26_Init_Etats_Eqts : Fin d execution");
	return(XDC_OK);
}
/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	xzat27_Init_Etats_EqtsIhm permet de retourner l'etat des
*	equipements a l'ihm synoptique.
*	Algo : Recoit les parametres, 
*	constitue un msg RTwks et
* 	envoie le message RTWks XDM_AT_INI_IHM.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int xzat27_Init_Etats_EqtsIhm (	XDY_Octet	va_Type,
				XDY_Entier 	va_Etats,
				XDY_Datagroup	va_DataGroupIhm) 
/*
* ARGUMENTS EN ENTREE :
*
*   	Arguments d'une callback RTServer.
*
* ARGUMENTS EN SORTIE :
*
*   aucun
*
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
*	Fonction permettant d envoyer le message XDM_AT_INI_IHM.
*	Le message RTwks doit contenir un XDY_Octet,XDY_Entier,XDY_NomMachine.
*	Le message declenchera XZAT;26 une SP effectuant un select et ne prenant pas
*	de parametre de retour.
* 	
------------------------------------------------------*/
{
	static char *	version = "$Id: xzat.c,v 1.20 2021/06/10 08:45:48 pc2dpdy Exp $ : xzat27_Init_Etats_EqtsIhm" ;
	
	XDY_Datagroup   vl_datagroup;
	XDY_NomSite 	pl_NomSite;
        T_BOOL		vl_EtatDg = FALSE;
        int		vl_retcode = XDC_NOK;
        
	XZST_03EcritureTrace(XZSTC_INTERFACE,"IN  : xzat27_Init_Etats_EqtsIhm : Debut d execution\n a recu Type = %d\n Etats = %d\n DataGroupIhm = %s",va_Type,va_Etats,va_DataGroupIhm);

	/*A
	** Recuperation du site de la machine 
	*/
	if ( (vl_retcode = XZSC_04NomSite(pl_NomSite)) != XDC_OK)
	{
		XZST_03EcritureTrace( XZSTC_WARNING, "xzat27_Init_Etats_EqtsIhm : Recuperation du Nom du site impossible .");
		return( XDC_NOK);
	} 
	
	/* 
	** Affichage Trace 
	*/
	XZST_03EcritureTrace( XZSTC_DEBUG1, "xzat27_Init_Etats_EqtsIhm : Recuperation du Nom du site %s.",pl_NomSite);
	
	/*A
	** Constitution du nom Data Group a partir du nom du site
	*/
	XDG_EncodeDG2(vl_datagroup,XDG_AT,pl_NomSite);
	
	XZST_03EcritureTrace ( XZSTC_DEBUG1, "xzat27_Init_Etats_EqtsIhm : Data group a joindre : %s",vl_datagroup);
	
	
	/*A
	** Envoi du message XDM_AT_INI_IHM 
	*/ 
	if (TipcSrvMsgWrite(vl_datagroup,XDM_IdentMsg(XDM_AT_INI_IHM) ,
				XDM_FLG_SRVMSGWRITE,
				T_IPC_FT_INT2, va_Type, 
				T_IPC_FT_INT4, va_Etats, 
				T_IPC_FT_STR, va_DataGroupIhm, 
				NULL))
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1,"xzat27_Init_Etats_EqtsIhm a reussi a envoyer le msg XDM_AT_INI_IHM" );	
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_WARNING ,"xzat27_Init_Etats_EqtsIhm : TipcSrvMsgWrite a echoue");	
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : xzat27_Init_Etats_EqtsIhm a echoue : Sortie");
		return(XDC_NOK);	 
	}
	/*A
	** Fin de l interface xzat27_Init_Etats_EqtsIhm
	*/
	XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT : xzat27_Init_Etats_EqtsIhm : Fin d execution");
	return(XDC_OK);
}
/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	xzat28_CompteRendu_Pal permet de mettre le compte rendu
*	d une action en base.
*	Algo : Recoit les parametres, 
*	constitue un msg RTwks et
* 	envoie le message RTWks XDM_AT_PAL.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int xzat28_CompteRendu_Pal (	XDY_Entier	va_Num_Action,
				XDY_Horodate 	va_Horodate_double,
				XDY_Booleen  	va_ResultatAction,
				XDY_District va_SiteGestion) 
/*
* ARGUMENTS EN ENTREE :
*
*   	Arguments d'une callback RTServer.
*
* ARGUMENTS EN SORTIE :
*
*   aucun
*
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
*	Fonction permettant d envoyer le message XDM_AT_PAL.
*	Le message RTwks doit contenir un XDY_Mot,XDY_Horodate,XDY_Booleen.
*	Le message declenchera XZAT;27 une SP n effectuant pas de select et ne prenant pas
*	de parametre de retour.
* 	
------------------------------------------------------*/
{
	static char *	version = "$Id: xzat.c,v 1.20 2021/06/10 08:45:48 pc2dpdy Exp $ : xzat28_CompteRendu_Pal" ;
	
	XDY_Datagroup   vl_datagroup;
	XDY_NomSite 	pl_NomSite;
        T_BOOL		vl_EtatDg = FALSE;
        int		vl_retcode = XDC_NOK;
        
	XZST_03EcritureTrace(XZSTC_INTERFACE,"IN  : xzat28_CompteRendu_Pal : Debut d execution\n a recu Num_Action = %d\n Horodate = %lf\n ResultatAction = %d",va_Num_Action,va_Horodate_double,va_ResultatAction);
	
	/*A
	** Recuperation du site de la machine 
	*/
	if ( (vl_retcode = XZSC_04NomSite(pl_NomSite)) != XDC_OK)
	{
		XZST_03EcritureTrace( XZSTC_WARNING, "xzat28_CompteRendu_Pal : Recuperation du Nom du site impossible .");
		return( XDC_NOK);
	} 
	
	XZST_03EcritureTrace( XZSTC_DEBUG1, "xzat28_CompteRendu_Pal : Recuperation du Nom du site %s.",pl_NomSite);
	
	/*A
	** Constitution du nom Data Group a partir du nom du site
	*/
    switch (va_SiteGestion) 
	{
	 	case (XDC_CI) : XDG_EncodeDG2(vl_datagroup,XDG_AT,XDC_NOM_SITE_CI);
			      break;
	 	case (XDC_VC) : XDG_EncodeDG2(vl_datagroup,XDG_AT,XDC_NOM_SITE_VC);
			      break;
	 	case (XDC_DP) : XDG_EncodeDG2(vl_datagroup,XDG_AT,XDC_NOM_SITE_DP);
			      break;
	 	case (XDC_CA) : XDG_EncodeDG2(vl_datagroup,XDG_AT,XDC_NOM_SITE_CA);
			      break;
	 	default: XDG_EncodeDG2(vl_datagroup,XDG_AT,pl_NomSite);
				break;
	}
	XZST_03EcritureTrace ( XZSTC_DEBUG1, "xzat28_CompteRendu_Pal : Data group a joindre : %s",vl_datagroup);
	
	
	/*A
	** Envoi du message XDM_AT_PAL 
	*/ 
	if (TipcSrvMsgWrite(vl_datagroup,XDM_IdentMsg(XDM_AT_PAL) ,
				XDM_FLG_SRVMSGWRITE,
				T_IPC_FT_INT4, va_Num_Action, 
				T_IPC_FT_REAL8,va_Horodate_double, 
				T_IPC_FT_CHAR, va_ResultatAction, 
				NULL))
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1,"xzat28_CompteRendu_Pal a reussi a envoyer le msg XDM_AT_PAL" );	
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_WARNING ,"xzat28_CompteRendu_Pal : TipcSrvMsgWrite a echoue");	
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : xzat28_CompteRendu_Pal a echoue : Sortie");
		return(XDC_NOK);	 
	}
	/*A
	** Fin de l interface xzat28_CompteRendu_Pal
	*/
	XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT : xzat28_CompteRendu_Pal : Fin d execution");
	return(XDC_OK);
}




/*PMVA DEBUT*/
/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	xzat25_CompteRendu_PMV permet de mettre le compte rendu
*	d une action en base.
*	Algo : Recoit les parametres, 
*	constitue un msg RTwks et
* 	envoie le message RTWks XDM_AT_PMV.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int xzat250_CompteRendu_PMVA (	XDY_Entier	va_Num_Action,
				XDY_Horodate 	va_Horodate_double,
				XDY_Booleen  	va_ResultatAction,
				XDY_District va_SiteGestion) 
/*
* ARGUMENTS EN ENTREE :
*
*   	Arguments d'une callback RTServer.
*
* ARGUMENTS EN SORTIE :
*
*   aucun
*
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
*	Fonction permettant d envoyer le message XDM_AT_PMV.
*	Le message RTwks doit contenir un XDY_Mot,XDY_Horodate,XDY_Booleen.
*	Le message declenchera XZAT;23 une SP n effectuant pas de select et ne prenant pas
*	de parametre de retour.
* 	
------------------------------------------------------*/
{
	static char *	version = "@(#)xzat.c	1.8 01/29/98 : xzat250_CompteRendu_PMVA" ;
	
	XDY_Datagroup   vl_datagroup;
	XDY_NomSite 	pl_NomSite;
        T_BOOL		vl_EtatDg = FALSE;
        int		vl_retcode = XDC_NOK;
        
	XZST_03EcritureTrace(XZSTC_INTERFACE,"IN  : xzat250_CompteRendu_PMVA : Debut d execution\n a recu Num_Action = %d\n Horodate = %lf\n ResultatAction = %d",va_Num_Action,va_Horodate_double,va_ResultatAction);
	
	/*A
	** Recuperation du site de la machine 
	*/
	if ( (vl_retcode = XZSC_04NomSite(pl_NomSite)) != XDC_OK)
	{
		XZST_03EcritureTrace( XZSTC_WARNING, "xzat250_CompteRendu_PMVA : Recuperation du Nom du site impossible .");
		return( XDC_NOK);
	} 
	
	XZST_03EcritureTrace( XZSTC_DEBUG1, "xzat250_CompteRendu_PMVA : Recuperation du Nom du site %s.",pl_NomSite);
	
	/*A
	** Constitution du nom Data Group a partir du nom du site
	*/
    switch (va_SiteGestion) 
	{
	 	case (XDC_CI) : XDG_EncodeDG2(vl_datagroup,XDG_AT,XDC_NOM_SITE_CI);
			      break;
	 	case (XDC_VC) : XDG_EncodeDG2(vl_datagroup,XDG_AT,XDC_NOM_SITE_VC);
			      break;
	 	case (XDC_DP) : XDG_EncodeDG2(vl_datagroup,XDG_AT,XDC_NOM_SITE_DP);
			      break;
	 	case (XDC_CA) : XDG_EncodeDG2(vl_datagroup,XDG_AT,XDC_NOM_SITE_CA);
			      break;
	 	default: XDG_EncodeDG2(vl_datagroup,XDG_AT,pl_NomSite);
				break;
	}
	XZST_03EcritureTrace ( XZSTC_DEBUG1, "xzat250_CompteRendu_PMVA : Data group a joindre : %s",vl_datagroup);
	
	
	/*A
	** Envoi du message XDM_AT_PMV 
	*/ 
	if (TipcSrvMsgWrite(vl_datagroup,XDM_IdentMsg(XDM_AT_PMVA) ,
				XDM_FLG_SRVMSGWRITE,
				T_IPC_FT_INT4, va_Num_Action, 
				T_IPC_FT_REAL8,va_Horodate_double, 
				T_IPC_FT_CHAR, va_ResultatAction, 
				NULL))
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1,"xzat250_CompteRendu_PMVA a reussi a envoyer le msg XDM_AT_PMV" );	
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_WARNING ,"xzat250_CompteRendu_PMVA : TipcSrvMsgWrite a echoue");	
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : xzat250_CompteRendu_PMVA a echoue : Sortie");
		return(XDC_NOK);	 
	}
	/*A
	** Fin de l interface xzat25_CompteRendu_PMV
	*/
	XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT : xzat250_CompteRendu_PMVA : Fin d execution");
	return(XDC_OK);
}
/*PMVA FIN*/

/*PRV DEBUT*/
/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	xzat25_CompteRendu_PRV permet de mettre le compte rendu
*	d une action en base.
*	Algo : Recoit les parametres, 
*	constitue un msg RTwks et
* 	envoie le message RTWks XDM_AT_PRV.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int xzat520_CompteRendu_PRV (	XDY_Entier	va_Num_Action,
				XDY_Horodate 	va_Horodate_double,
				XDY_Booleen  	va_ResultatAction,
				XDY_District va_SiteGestion) 
/*
* ARGUMENTS EN ENTREE :
*
*   	Arguments d'une callback RTServer.
*
* ARGUMENTS EN SORTIE :
*
*   aucun
*
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
*	Fonction permettant d envoyer le message XDM_AT_PMV.
*	Le message RTwks doit contenir un XDY_Mot,XDY_Horodate,XDY_Booleen.
*	Le message declenchera XZAT;23 une SP n effectuant pas de select et ne prenant pas
*	de parametre de retour.
* 	
------------------------------------------------------*/
{
	static char *	version = "@(#)xzat.c	1.8 01/29/98 : xzat520_CompteRendu_PRV" ;
	
	XDY_Datagroup   vl_datagroup;
	XDY_NomSite 	pl_NomSite;
        T_BOOL		vl_EtatDg = FALSE;
        int		vl_retcode = XDC_NOK;
        
	XZST_03EcritureTrace(XZSTC_INTERFACE,"IN  : xzat520_CompteRendu_PRV : Debut d execution\n a recu Num_Action = %d\n Horodate = %lf\n ResultatAction = %d",va_Num_Action,va_Horodate_double,va_ResultatAction);
	
	/*A
	** Recuperation du site de la machine 
	*/
	if ( (vl_retcode = XZSC_04NomSite(pl_NomSite)) != XDC_OK)
	{
		XZST_03EcritureTrace( XZSTC_WARNING, "xzat520_CompteRendu_PRV : Recuperation du Nom du site impossible .");
		return( XDC_NOK);
	} 
	
	XZST_03EcritureTrace( XZSTC_DEBUG1, "xzat520_CompteRendu_PRV : Recuperation du Nom du site %s.",pl_NomSite);
	
	/*A
	** Constitution du nom Data Group a partir du nom du site
	*/
    switch (va_SiteGestion) 
	{
	 	case (XDC_CI) : XDG_EncodeDG2(vl_datagroup,XDG_AT,XDC_NOM_SITE_CI);
			      break;
	 	case (XDC_VC) : XDG_EncodeDG2(vl_datagroup,XDG_AT,XDC_NOM_SITE_VC);
			      break;
	 	case (XDC_DP) : XDG_EncodeDG2(vl_datagroup,XDG_AT,XDC_NOM_SITE_DP);
			      break;
	 	case (XDC_CA) : XDG_EncodeDG2(vl_datagroup,XDG_AT,XDC_NOM_SITE_CA);
			      break;
	 	default: XDG_EncodeDG2(vl_datagroup,XDG_AT,pl_NomSite);
				break;
	}
	XZST_03EcritureTrace ( XZSTC_DEBUG1, "xzat520_CompteRendu_PRV : Data group a joindre : %s",vl_datagroup);
	
	
	/*A
	** Envoi du message XDM_AT_PMV 
	*/ 
	if (TipcSrvMsgWrite(vl_datagroup,XDM_IdentMsg(XDM_AT_PRV) ,
				XDM_FLG_SRVMSGWRITE,
				T_IPC_FT_INT4, va_Num_Action, 
				T_IPC_FT_REAL8,va_Horodate_double, 
				T_IPC_FT_CHAR, va_ResultatAction, 
				NULL))
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzat520_CompteRendu_PRV a reussi a envoyer le msg XDM_AT_PRV" );
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_WARNING ,"xzat520_CompteRendu_PRV : TipcSrvMsgWrite a echoue");	
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : xzat520_CompteRendu_PRV a echoue : Sortie");
		return(XDC_NOK);	 
	}
	/*A
	** Fin de l interface xzat520_CompteRendu_PRV
	*/
	XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT : xzat520_CompteRendu_PRV : Fin d execution");
	return(XDC_OK);
}
/*PRV FIN*/

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	xzat30_CompteRendu_BAF permet de mettre le compte rendu
*	d une action en base.
*	Algo : Recoit les parametres, 
*	constitue un msg RTwks et
* 	envoie le message RTWks XDM_AT_BAF.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int xzat30_CompteRendu_BAF (	XDY_Entier	va_Num_Action,
				XDY_Horodate 	va_Horodate_double,
				XDY_Booleen  	va_ResultatAction,
				XDY_District va_SiteGestion) 
/*
* ARGUMENTS EN ENTREE :
*
*   	Arguments d'une callback RTServer.
*
* ARGUMENTS EN SORTIE :
*
*   aucun
*
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
*	Fonction permettant d envoyer le message XDM_AT_NAV.
*	Le message RTwks doit contenir un XDY_Mot,XDY_Horodate,XDY_Booleen.
*	Le message declenchera XZAT;23 une SP n effectuant pas de select et ne prenant pas
*	de parametre de retour.
* 	
------------------------------------------------------*/
{
	static char *	version = "@(#)xzat.c	1.8 01/29/98 : xzat30_CompteRendu_BAF" ;
	
	XDY_Datagroup   vl_datagroup;
	XDY_NomSite 	pl_NomSite;
        T_BOOL		vl_EtatDg = FALSE;
        int		vl_retcode = XDC_NOK;
        
	XZST_03EcritureTrace(XZSTC_WARNING,"IN  : xzat30_CompteRendu_BAF : Debut d execution\n a recu Num_Action = %d\n Horodate = %lf\n ResultatAction = %d",va_Num_Action,va_Horodate_double,va_ResultatAction);
	
	/*A
	** Recuperation du site de la machine 
	*/
	if ( (vl_retcode = XZSC_04NomSite(pl_NomSite)) != XDC_OK)
	{
		XZST_03EcritureTrace( XZSTC_WARNING, "xzat30_CompteRendu_BAF : Recuperation du Nom du site impossible .");
		return( XDC_NOK);
	} 
	
	XZST_03EcritureTrace( XZSTC_DEBUG1, "xzat30_CompteRendu_BAF : Recuperation du Nom du site %s.",pl_NomSite);
	
	/*A
	** Constitution du nom Data Group a partir du nom du site
	*/
    switch (va_SiteGestion) 
	{
	 	case (XDC_CI) : XDG_EncodeDG2(vl_datagroup,XDG_AT,XDC_NOM_SITE_CI);
			      break;
	 	case (XDC_VC) : XDG_EncodeDG2(vl_datagroup,XDG_AT,XDC_NOM_SITE_VC);
			      break;
	 	case (XDC_DP) : XDG_EncodeDG2(vl_datagroup,XDG_AT,XDC_NOM_SITE_DP);
			      break;
	 	case (XDC_CA) : XDG_EncodeDG2(vl_datagroup,XDG_AT,XDC_NOM_SITE_CA);
			      break;
	 	default: XDG_EncodeDG2(vl_datagroup,XDG_AT,pl_NomSite);
				break;
	}
	XZST_03EcritureTrace ( XZSTC_DEBUG1, "xzat30_CompteRendu_BAF : Data group a joindre : %s",vl_datagroup);
	
	
	/*A
	** Envoi du message XDM_AT_BAF 
	*/ 
	if (TipcSrvMsgWrite(vl_datagroup,XDM_IdentMsg(XDM_AT_BAF) ,
				XDM_FLG_SRVMSGWRITE,
				T_IPC_FT_INT4, va_Num_Action, 
				T_IPC_FT_REAL8,va_Horodate_double, 
				T_IPC_FT_CHAR, va_ResultatAction, 
				NULL))
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1,"xzat30_CompteRendu_BAF a reussi a envoyer le msg XDM_AT_BAF" );	
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_WARNING ,"xzat30_CompteRendu_BAF : TipcSrvMsgWrite a echoue");	
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : xzat30_CompteRendu_BAF a echoue : Sortie");
		return(XDC_NOK);	 
	}
	/*A
	** Fin de l interface xzat24_CompteRendu_Nav
	*/
	XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT : xzat30_CompteRendu_BAF : Fin d execution");
	return(XDC_OK);
}


/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	xzat30_CompteRendu_BAF permet de mettre le compte rendu
*	d une action en base.
*	Algo : Recoit les parametres, 
*	constitue un msg RTwks et
* 	envoie le message RTWks XDM_AT_BAD.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int xzat31_CompteRendu_BAD (	XDY_Entier	va_Num_Action,
				XDY_Horodate 	va_Horodate_double,
				XDY_Booleen  	va_ResultatAction,
				XDY_District va_SiteGestion) 
/*
* ARGUMENTS EN ENTREE :
*
*   	Arguments d'une callback RTServer.
*
* ARGUMENTS EN SORTIE :
*
*   aucun
*
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
*	Fonction permettant d envoyer le message XDM_AT_NAV.
*	Le message RTwks doit contenir un XDY_Mot,XDY_Horodate,XDY_Booleen.
*	Le message declenchera XZAT;23 une SP n effectuant pas de select et ne prenant pas
*	de parametre de retour.
* 	
------------------------------------------------------*/
{
	static char *	version = "@(#)xzat.c	1.8 01/29/98 : xzat31_CompteRendu_BAD" ;
	
	XDY_Datagroup   vl_datagroup;
	XDY_NomSite 	pl_NomSite;
        T_BOOL		vl_EtatDg = FALSE;
        int		vl_retcode = XDC_NOK;
        
	XZST_03EcritureTrace(XZSTC_INTERFACE,"IN  : xzat31_CompteRendu_BAD : Debut d execution\n a recu Num_Action = %d\n Horodate = %lf\n ResultatAction = %d",va_Num_Action,va_Horodate_double,va_ResultatAction);
	
	/*A
	** Recuperation du site de la machine 
	*/
	if ( (vl_retcode = XZSC_04NomSite(pl_NomSite)) != XDC_OK)
	{
		XZST_03EcritureTrace( XZSTC_WARNING, "xzat31_CompteRendu_BAD : Recuperation du Nom du site impossible .");
		return( XDC_NOK);
	} 
	
	XZST_03EcritureTrace( XZSTC_DEBUG1, "xzat31_CompteRendu_BAD : Recuperation du Nom du site %s.",pl_NomSite);
	
	/*A
	** Constitution du nom Data Group a partir du nom du site
	*/
    switch (va_SiteGestion) 
	{
	 	case (XDC_CI) : XDG_EncodeDG2(vl_datagroup,XDG_AT,XDC_NOM_SITE_CI);
			      break;
	 	case (XDC_VC) : XDG_EncodeDG2(vl_datagroup,XDG_AT,XDC_NOM_SITE_VC);
			      break;
	 	case (XDC_DP) : XDG_EncodeDG2(vl_datagroup,XDG_AT,XDC_NOM_SITE_DP);
			      break;
	 	case (XDC_CA) : XDG_EncodeDG2(vl_datagroup,XDG_AT,XDC_NOM_SITE_CA);
			      break;
	 	default: XDG_EncodeDG2(vl_datagroup,XDG_AT,pl_NomSite);
				break;
	}
	XZST_03EcritureTrace ( XZSTC_DEBUG1, "xzat31_CompteRendu_BAD : Data group a joindre : %s",vl_datagroup);
	
	
	/*A
	** Envoi du message XDM_AT_BAD 
	*/ 
	if (TipcSrvMsgWrite(vl_datagroup,XDM_IdentMsg(XDM_AT_BAD) ,
				XDM_FLG_SRVMSGWRITE,
				T_IPC_FT_INT4, va_Num_Action, 
				T_IPC_FT_REAL8,va_Horodate_double, 
				T_IPC_FT_CHAR, va_ResultatAction, 
				NULL))
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1,"xzat31_CompteRendu_BAD a reussi a envoyer le msg XDM_AT_BAF" );	
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_WARNING ,"xzat31_CompteRendu_BAD : TipcSrvMsgWrite a echoue");	
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : xzat30_CompteRendu_BAF a echoue : Sortie");
		return(XDC_NOK);	 
	}
	/*A
	** Fin de l interface xzat24_CompteRendu_Nav
	*/
	XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT : xzat31_CompteRendu_BAD : Fin d execution");
	return(XDC_OK);
}


/* AAZ Ajout xzat40, 41, 42, 43, 44 pour SIG, ECL, VEN, ENR, INC */

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	xzat40_CompteRendu_SIG permet de mettre le compte rendu
*	d une action en base.
*	Algo : Recoit les parametres, 
*	constitue un msg RTwks et
* 	envoie le message RTWks XDM_AT_SIG.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int xzat40_CompteRendu_SIG ( XDY_Entier	    va_Num_Action,
				             XDY_Horodate 	va_Horodate_double,
				             XDY_Booleen  	va_ResultatAction) 
/*
* ARGUMENTS EN ENTREE :
*
*   	Arguments d'une callback RTServer.
*
* ARGUMENTS EN SORTIE :
*
*   aucun
*
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
*	Fonction permettant d envoyer le message XDM_AT_SIG.
*	Le message RTwks doit contenir un XDY_Mot,XDY_Horodate,XDY_Booleen.
*	Le message declenchera XZAT;40 une SP n effectuant pas de select et ne prenant pas
*	de parametre de retour.
* 	
------------------------------------------------------*/
{
	static char *	version = "@(#)xzat.c	1.10 03/26/07 : xzat40_CompteRendu_SIG" ;
	
	XDY_Datagroup   vl_datagroup;
	XDY_NomSite 	pl_NomSite;
    T_BOOL		    vl_EtatDg = FALSE;
    int		        vl_retcode = XDC_NOK;
        
	XZST_03EcritureTrace(XZSTC_INTERFACE,"IN  : xzat40_CompteRendu_SIG : Debut d execution\n a recu Num_Action = %d\n Horodate = %lf\n ResultatAction = %d",va_Num_Action,va_Horodate_double,va_ResultatAction);
	
	/*A
	** Recuperation du site de la machine 
	*/
	if ( (vl_retcode = XZSC_04NomSite(pl_NomSite)) != XDC_OK)
	{
		XZST_03EcritureTrace( XZSTC_WARNING, "xzat40 : Recuperation du Nom du site impossible .");
		return( XDC_NOK);
	} 
	
	XZST_03EcritureTrace( XZSTC_DEBUG1, "xzat40 : Recuperation du Nom du site %s.",pl_NomSite);
	
	/*A
	** Constitution du nom Data Group a partir du nom du site
	*/
	XDG_EncodeDG2(vl_datagroup,XDG_AT,pl_NomSite);
	
	XZST_03EcritureTrace ( XZSTC_DEBUG1, "xzat40 : Data group a joindre : %s",vl_datagroup);
	
	
	/*A
	** Envoi du message XDM_AT_SIG 
	*/ 
	if (TipcSrvMsgWrite(vl_datagroup,XDM_IdentMsg(XDM_AT_SIG) ,
				XDM_FLG_SRVMSGWRITE,
				T_IPC_FT_INT4, va_Num_Action, 
				T_IPC_FT_REAL8,va_Horodate_double, 
				T_IPC_FT_CHAR, va_ResultatAction, 
				NULL))
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1,"xzat40 a reussi a envoyer le msg XDM_AT_SIG" );	
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_WARNING ,"xzat40_CompteRendu_SIG : TipcSrvMsgWrite a echoue");	
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : xzat40_CompteRendu_SIG a echoue : Sortie");
		return(XDC_NOK);	 
	}
	/*A
	** Fin de l interface xzat40
	*/
	XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT : xzat40_CompteRendu_SIG : Fin d execution");
	return(XDC_OK);
}

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	xzat41_CompteRendu_ECL permet de mettre le compte rendu
*	d une action en base.
*	Algo : Recoit les parametres, 
*	constitue un msg RTwks et
* 	envoie le message RTWks XDM_AT_ECL.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int xzat41_CompteRendu_ECL ( XDY_Entier	    va_Num_Action,
				             XDY_Horodate 	va_Horodate_double,
				             XDY_Booleen  	va_ResultatAction) 
/*
* ARGUMENTS EN ENTREE :
*
*   	Arguments d'une callback RTServer.
*
* ARGUMENTS EN SORTIE :
*
*   aucun
*
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
*	Fonction permettant d envoyer le message XDM_AT_ECL.
*	Le message RTwks doit contenir un XDY_Mot,XDY_Horodate,XDY_Booleen.
*	Le message declenchera XZAT;41 une SP n effectuant pas de select et ne prenant pas
*	de parametre de retour.
* 	
------------------------------------------------------*/
{
	static char *	version = "@(#)xzat.c	1.10 03/26/07 : xzat41_CompteRendu_ECL" ;
	
	XDY_Datagroup   vl_datagroup;
	XDY_NomSite 	pl_NomSite;
    T_BOOL		    vl_EtatDg = FALSE;
    int		        vl_retcode = XDC_NOK;
        
	XZST_03EcritureTrace(XZSTC_INTERFACE,"IN  : xzat41_CompteRendu_ECL : Debut d execution\n a recu Num_Action = %d\n Horodate = %lf\n ResultatAction = %d",va_Num_Action,va_Horodate_double,va_ResultatAction);
	
	/*A
	** Recuperation du site de la machine 
	*/
	if ( (vl_retcode = XZSC_04NomSite(pl_NomSite)) != XDC_OK)
	{
		XZST_03EcritureTrace( XZSTC_WARNING, "xzat41 : Recuperation du Nom du site impossible .");
		return( XDC_NOK);
	} 
	
	XZST_03EcritureTrace( XZSTC_DEBUG1, "xzat41 : Recuperation du Nom du site %s.",pl_NomSite);
	
	/*A
	** Constitution du nom Data Group a partir du nom du site
	*/
	XDG_EncodeDG2(vl_datagroup,XDG_AT,pl_NomSite);
	
	XZST_03EcritureTrace ( XZSTC_DEBUG1, "xzat41 : Data group a joindre : %s",vl_datagroup);
	
	
	/*A
	** Envoi du message XDM_AT_LTN 
	*/ 
	if (TipcSrvMsgWrite(vl_datagroup,XDM_IdentMsg(XDM_AT_ECL) ,
				XDM_FLG_SRVMSGWRITE,
				T_IPC_FT_INT4, va_Num_Action, 
				T_IPC_FT_REAL8,va_Horodate_double, 
				T_IPC_FT_CHAR, va_ResultatAction, 
				NULL))
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1,"xzat41 a reussi a envoyer le msg XDM_AT_ECL" );	
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_WARNING ,"xzat41_CompteRendu_ECL : TipcSrvMsgWrite a echoue");	
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : xzat41_CompteRendu_ECL a echoue : Sortie");
		return(XDC_NOK);	 
	}
	/*A
	** Fin de l interface xzat41
	*/
	XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT : xzat41_CompteRendu_ECL : Fin d execution");
	return(XDC_OK);
}


/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	xzat42_CompteRendu_VEN permet de mettre le compte rendu
*	d une action en base.
*	Algo : Recoit les parametres, 
*	constitue un msg RTwks et
* 	envoie le message RTWks XDM_AT_VEN.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int xzat42_CompteRendu_VEN ( XDY_Entier	    va_Num_Action,
				             XDY_Horodate 	va_Horodate_double,
				             XDY_Booleen  	va_ResultatAction) 
/*
* ARGUMENTS EN ENTREE :
*
*   	Arguments d'une callback RTServer.
*
* ARGUMENTS EN SORTIE :
*
*   aucun
*
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
*	Fonction permettant d envoyer le message XDM_AT_VEN.
*	Le message RTwks doit contenir un XDY_Mot,XDY_Horodate,XDY_Booleen.
*	Le message declenchera XZAT;42 une SP n effectuant pas de select et ne prenant pas
*	de parametre de retour.
* 	
------------------------------------------------------*/
{
	static char *	version = "@(#)xzat.c	1.10 03/26/07 : xzat42_CompteRendu_VEN" ;
	
	XDY_Datagroup   vl_datagroup;
	XDY_NomSite 	pl_NomSite;
    T_BOOL		    vl_EtatDg = FALSE;
    int		        vl_retcode = XDC_NOK;
        
	XZST_03EcritureTrace(XZSTC_INTERFACE,"IN  : xzat42_CompteRendu_VEN : Debut d execution\n a recu Num_Action = %d\n Horodate = %lf\n ResultatAction = %d",va_Num_Action,va_Horodate_double,va_ResultatAction);
	
	/*A
	** Recuperation du site de la machine 
	*/
	if ( (vl_retcode = XZSC_04NomSite(pl_NomSite)) != XDC_OK)
	{
		XZST_03EcritureTrace( XZSTC_WARNING, "xzat42 : Recuperation du Nom du site impossible .");
		return( XDC_NOK);
	} 
	
	XZST_03EcritureTrace( XZSTC_DEBUG1, "xzat42 : Recuperation du Nom du site %s.",pl_NomSite);
	
	/*A
	** Constitution du nom Data Group a partir du nom du site
	*/
	XDG_EncodeDG2(vl_datagroup,XDG_AT,pl_NomSite);
	
	XZST_03EcritureTrace ( XZSTC_DEBUG1, "xzat42 : Data group a joindre : %s",vl_datagroup);
	
	
	/*A
	** Envoi du message XDM_AT_VEN 
	*/ 
	if (TipcSrvMsgWrite(vl_datagroup,XDM_IdentMsg(XDM_AT_VEN) ,
				XDM_FLG_SRVMSGWRITE,
				T_IPC_FT_INT4, va_Num_Action, 
				T_IPC_FT_REAL8,va_Horodate_double, 
				T_IPC_FT_CHAR, va_ResultatAction, 
				NULL))
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1,"xzat42 a reussi a envoyer le msg XDM_AT_VEN" );	
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_WARNING ,"xzat42_CompteRendu_VEN : TipcSrvMsgWrite a echoue");	
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : xzat42_CompteRendu_VEN a echoue : Sortie");
		return(XDC_NOK);	 
	}
	/*A
	** Fin de l interface xzat42
	*/
	XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT : xzat42_CompteRendu_VEN : Fin d execution");
	return(XDC_OK);
}


/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	xzat43_CompteRendu_ENR permet de mettre le compte rendu
*	d une action en base.
*	Algo : Recoit les parametres, 
*	constitue un msg RTwks et
* 	envoie le message RTWks XDM_AT_ENR.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int xzat43_CompteRendu_ENR ( XDY_Entier	    va_Num_Action,
				             XDY_Horodate 	va_Horodate_double,
				             XDY_Booleen  	va_ResultatAction) 
/*
* ARGUMENTS EN ENTREE :
*
*   	Arguments d'une callback RTServer.
*
* ARGUMENTS EN SORTIE :
*
*   aucun
*
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
*	Fonction permettant d envoyer le message XDM_AT_ENR.
*	Le message RTwks doit contenir un XDY_Mot,XDY_Horodate,XDY_Booleen.
*	Le message declenchera XZAT;43 une SP n effectuant pas de select et ne prenant pas
*	de parametre de retour.
* 	
------------------------------------------------------*/
{
	static char *	version = "@(#)xzat.c	1.10 03/26/07 : xzat43_CompteRendu_ENR" ;
	
	XDY_Datagroup   vl_datagroup;
	XDY_NomSite 	pl_NomSite;
    T_BOOL		    vl_EtatDg = FALSE;
    int		        vl_retcode = XDC_NOK;
        
	XZST_03EcritureTrace(XZSTC_INTERFACE,"IN  : xzat43_CompteRendu_ENR : Debut d execution\n a recu Num_Action = %d\n Horodate = %lf\n ResultatAction = %d",va_Num_Action,va_Horodate_double,va_ResultatAction);
	
	/*A
	** Recuperation du site de la machine 
	*/
	if ( (vl_retcode = XZSC_04NomSite(pl_NomSite)) != XDC_OK)
	{
		XZST_03EcritureTrace( XZSTC_WARNING, "xzat43 : Recuperation du Nom du site impossible .");
		return( XDC_NOK);
	} 
	
	XZST_03EcritureTrace( XZSTC_DEBUG1, "xzat43 : Recuperation du Nom du site %s.",pl_NomSite);
	
	/*A
	** Constitution du nom Data Group a partir du nom du site
	*/
	XDG_EncodeDG2(vl_datagroup,XDG_AT,pl_NomSite);
	
	XZST_03EcritureTrace ( XZSTC_DEBUG1, "xzat43 : Data group a joindre : %s",vl_datagroup);
	
	
	/*A
	** Envoi du message XDM_AT_ENR 
	*/ 
	if (TipcSrvMsgWrite(vl_datagroup,XDM_IdentMsg(XDM_AT_ENR) ,
				XDM_FLG_SRVMSGWRITE,
				T_IPC_FT_INT4, va_Num_Action, 
				T_IPC_FT_REAL8,va_Horodate_double, 
				T_IPC_FT_CHAR, va_ResultatAction, 
				NULL))
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1,"xzat43 a reussi a envoyer le msg XDM_AT_ENR" );	
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_WARNING ,"xzat43_CompteRendu_ENR : TipcSrvMsgWrite a echoue");	
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : xzat43_CompteRendu_ENR a echoue : Sortie");
		return(XDC_NOK);	 
	}
	/*A
	** Fin de l interface xzat43
	*/
	XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT : xzat43_CompteRendu_ENR : Fin d execution");
	return(XDC_OK);
}


/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	xzat44_CompteRendu_INC permet de mettre le compte rendu
*	d une action en base.
*	Algo : Recoit les parametres, 
*	constitue un msg RTwks et
* 	envoie le message RTWks XDM_AT_INC.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int xzat44_CompteRendu_INC ( XDY_Entier	    va_Num_Action,
				             XDY_Horodate 	va_Horodate_double,
				             XDY_Booleen  	va_ResultatAction) 
/*
* ARGUMENTS EN ENTREE :
*
*   	Arguments d'une callback RTServer.
*
* ARGUMENTS EN SORTIE :
*
*   aucun
*
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
*	Fonction permettant d envoyer le message XDM_AT_INC.
*	Le message RTwks doit contenir un XDY_Mot,XDY_Horodate,XDY_Booleen.
*	Le message declenchera XZAT;43 une SP n effectuant pas de select et ne prenant pas
*	de parametre de retour.
* 	
------------------------------------------------------*/
{
	static char *	version = "@(#)xzat.c	1.10 03/26/07 : xzat44_CompteRendu_INC" ;
	
	XDY_Datagroup   vl_datagroup;
	XDY_NomSite 	pl_NomSite;
    T_BOOL		    vl_EtatDg = FALSE;
    int		        vl_retcode = XDC_NOK;
        
	XZST_03EcritureTrace(XZSTC_INTERFACE,"IN  : xzat44_CompteRendu_INC : Debut d execution\n a recu Num_Action = %d\n Horodate = %lf\n ResultatAction = %d",va_Num_Action,va_Horodate_double,va_ResultatAction);
	
	/*A
	** Recuperation du site de la machine 
	*/
	if ( (vl_retcode = XZSC_04NomSite(pl_NomSite)) != XDC_OK)
	{
		XZST_03EcritureTrace( XZSTC_WARNING, "xzat44 : Recuperation du Nom du site impossible .");
		return( XDC_NOK);
	} 
	
	XZST_03EcritureTrace( XZSTC_DEBUG1, "xzat44 : Recuperation du Nom du site %s.",pl_NomSite);
	
	/*A
	** Constitution du nom Data Group a partir du nom du site
	*/
	XDG_EncodeDG2(vl_datagroup,XDG_AT,pl_NomSite);
	
	XZST_03EcritureTrace ( XZSTC_DEBUG1, "xzat44 : Data group a joindre : %s",vl_datagroup);
	
	
	/*A
	** Envoi du message XDM_AT_INC 
	*/ 
	if (TipcSrvMsgWrite(vl_datagroup,XDM_IdentMsg(XDM_AT_INC) ,
				XDM_FLG_SRVMSGWRITE,
				T_IPC_FT_INT4, va_Num_Action, 
				T_IPC_FT_REAL8,va_Horodate_double, 
				T_IPC_FT_CHAR, va_ResultatAction, 
				NULL))
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1,"xzat44 a reussi a envoyer le msg XDM_AT_INC" );	
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_WARNING ,"xzat44_CompteRendu_INC : TipcSrvMsgWrite a echoue");	
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : xzat44_CompteRendu_INC a echoue : Sortie");
		return(XDC_NOK);	 
	}
	/*A
	** Fin de l interface xzat44
	*/
	XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT : xzat44_CompteRendu_INC : Fin d execution");
	return(XDC_OK);
}

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	xzat45_CompteRendu_EXP permet de mettre le compte rendu
*	d une action en base.
*	Algo : Recoit les parametres, 
*	constitue un msg RTwks et
* 	envoie le message RTWks XDM_AT_EXP.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int xzat45_CompteRendu_EXP ( XDY_Entier	    va_Num_Action,
				             XDY_Horodate 	va_Horodate_double,
				             XDY_Booleen  	va_ResultatAction) 
/*
* ARGUMENTS EN ENTREE :
*
*   	Arguments d'une callback RTServer.
*
* ARGUMENTS EN SORTIE :
*
*   aucun
*
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
*	Fonction permettant d envoyer le message XDM_AT_EXP.
*	Le message RTwks doit contenir un XDY_Mot,XDY_Horodate,XDY_Booleen.
*	Le message declenchera XZAT;43 une SP n effectuant pas de select et ne prenant pas
*	de parametre de retour.
* 	
------------------------------------------------------*/
{
	static char *	version = "@(#)xzat.c	1.10 03/26/07 : xzat45_CompteRendu_EXP" ;
	
	XDY_Datagroup   vl_datagroup;
	XDY_NomSite 	pl_NomSite;
    T_BOOL		    vl_EtatDg = FALSE;
    int		        vl_retcode = XDC_NOK;
        
	//XZST_03EcritureTrace(XZSTC_INTERFACE,"IN  : xzat45_CompteRendu_EXP : Debut d execution\n a recu Num_Action = %d\n Horodate = %lf\n ResultatAction = %d",va_Num_Action,va_Horodate_double,va_ResultatAction);
	XZST_03EcritureTrace(XZSTC_WARNING,"IN  : xzat45_CompteRendu_EXP : Debut d execution\n a recu Num_Action = %d\n Horodate = %lf\n ResultatAction = %d",va_Num_Action,va_Horodate_double,va_ResultatAction);
	
	/*A
	** Recuperation du site de la machine 
	*/
	if ( (vl_retcode = XZSC_04NomSite(pl_NomSite)) != XDC_OK)
	{
		XZST_03EcritureTrace( XZSTC_WARNING, "xzat45 : Recuperation du Nom du site impossible .");
		return( XDC_NOK);
	} 
	
	XZST_03EcritureTrace( XZSTC_DEBUG1, "xzat45 : Recuperation du Nom du site %s.",pl_NomSite);
	
	/*A
	** Constitution du nom Data Group a partir du nom du site
	*/
	XDG_EncodeDG2(vl_datagroup,XDG_AT,pl_NomSite);
	
	XZST_03EcritureTrace ( XZSTC_DEBUG1, "xzat45 : Data group a joindre : %s",vl_datagroup);
	
	
	/*A
	** Envoi du message XDM_AT_EXP 
	*/ 
	if (TipcSrvMsgWrite(vl_datagroup,XDM_IdentMsg(XDM_AT_EXP) ,
				XDM_FLG_SRVMSGWRITE,
				T_IPC_FT_INT4, va_Num_Action, 
				T_IPC_FT_REAL8,va_Horodate_double, 
				T_IPC_FT_CHAR, va_ResultatAction, 
				NULL))
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1,"xzat45 a reussi a envoyer le msg XDM_AT_EXP" );	
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_WARNING ,"xzat45_CompteRendu_EXP : TipcSrvMsgWrite a echoue");	
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : xzat45_CompteRendu_EXP a echoue : Sortie");
		return(XDC_NOK);	 
	}
	/*A
	** Fin de l interface xzat45
	*/
	XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT : xzat45_CompteRendu_EXP : Fin d execution");
	return(XDC_OK);
}




/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	xzat25_CompteRendu_PMV permet de mettre le compte rendu
*	d une action en base.
*	Algo : Recoit les parametres, 
*	constitue un msg RTwks et
* 	envoie le message RTWks XDM_AT_PMV.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int xzat251_CompteRendu_Eqt_SAGA (	XDY_Entier	va_Num_Action,
				XDY_Horodate 	va_Horodate_double,
				XDY_Booleen  	va_ResultatAction,
				XDY_District va_SiteGestion) 
/*
* ARGUMENTS EN ENTREE :
*
*   	Arguments d'une callback RTServer.
*
* ARGUMENTS EN SORTIE :
*
*   aucun
*
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
*	Fonction permettant d envoyer le message XDM_AT_PMV.
*	Le message RTwks doit contenir un XDY_Mot,XDY_Horodate,XDY_Booleen.
*	Le message declenchera XZAT;23 une SP n effectuant pas de select et ne prenant pas
*	de parametre de retour.
* 	
------------------------------------------------------*/
{
	static char *	version = "@(#)xzat.c	1.8 01/29/98 : xzat250_CompteRendu_PMVA" ;
	
	XDY_Datagroup   vl_datagroup;
	XDY_NomSite 	pl_NomSite;
        T_BOOL		vl_EtatDg = FALSE;
        int		vl_retcode = XDC_NOK;
        
	XZST_03EcritureTrace(XZSTC_INTERFACE,"IN  : xzat251_CompteRendu_Eqt_SAGA : Debut d execution\n a recu Num_Action = %d\n Horodate = %lf\n ResultatAction = %d",va_Num_Action,va_Horodate_double,va_ResultatAction);
	
	/*A
	** Recuperation du site de la machine 
	*/
	if ( (vl_retcode = XZSC_04NomSite(pl_NomSite)) != XDC_OK)
	{
		XZST_03EcritureTrace( XZSTC_WARNING, "xzat250_CompteRendu_PMVA : Recuperation du Nom du site impossible .");
		return( XDC_NOK);
	} 
	
	XZST_03EcritureTrace( XZSTC_DEBUG1, "xzat250_CompteRendu_PMVA : Recuperation du Nom du site %s.",pl_NomSite);
	
	/*A
	** Constitution du nom Data Group a partir du nom du site
	*/
    switch (va_SiteGestion) 
	{
	 	case (XDC_CI) : XDG_EncodeDG2(vl_datagroup,XDG_AT,XDC_NOM_SITE_CI);
			      break;
	 	case (XDC_VC) : XDG_EncodeDG2(vl_datagroup,XDG_AT,XDC_NOM_SITE_VC);
			      break;
	 	case (XDC_DP) : XDG_EncodeDG2(vl_datagroup,XDG_AT,XDC_NOM_SITE_DP);
			      break;
	 	case (XDC_CA) : XDG_EncodeDG2(vl_datagroup,XDG_AT,XDC_NOM_SITE_CA);
			      break;
	 	default: XDG_EncodeDG2(vl_datagroup,XDG_AT,pl_NomSite);
				break;
	}
	XZST_03EcritureTrace ( XZSTC_DEBUG1, "xzat250_CompteRendu_PMVA : Data group a joindre : %s",vl_datagroup);
	
	
	/*A
	** Envoi du message XDM_AT_PMV 
	*/ 
	if (TipcSrvMsgWrite(vl_datagroup,XDM_IdentMsg(XDM_AT_EQT_SAGA) ,
				XDM_FLG_SRVMSGWRITE,
				T_IPC_FT_INT4, va_Num_Action, 
				T_IPC_FT_REAL8,va_Horodate_double, 
				T_IPC_FT_CHAR, va_ResultatAction, 
				NULL))
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1,"xzat251_CompteRendu_Eqt_SAGA a reussi a envoyer le msg XDM_AT_PMV" );	
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_WARNING ,"xzat251_CompteRendu_Eqt_SAGA : TipcSrvMsgWrite a echoue");	
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : xzat251_CompteRendu_Eqt_SAGA a echoue : Sortie");
		return(XDC_NOK);	 
	}
	/*A
	** Fin de l interface xzat25_CompteRendu_PMV
	*/
	XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT : xzat251_CompteRendu_Eqt_SAGA : Fin d execution");
	return(XDC_OK);
}

/*CFE DEBUT*/
/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*       xzat521_CompteRendu_CFE permet de mettre le compte rendu
*       d une action en base.
*       Algo : Recoit les parametres, 
*       constitue un msg RTwks et
*       envoie le message RTWks XDM_AT_CFE.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int xzat521_CompteRendu_CFE (   XDY_Entier      va_Num_Action,
                                XDY_Horodate    va_Horodate_double,
                                XDY_Booleen     va_ResultatAction,
                                XDY_District va_SiteGestion)
/*
* ARGUMENTS EN ENTREE :
*
*       Arguments d'une callback RTServer.
*
* ARGUMENTS EN SORTIE :
*
*   aucun
*
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon renvoie XDC_NOK.
*
* CONDITION D'UTILISATION
*
*       Il faut etre connecte au RTserveur local.
*       
*
*
* FONCTION 
*       Fonction permettant d envoyer le message XDM_AT_CFE.
*       Le message RTwks doit contenir un XDY_Mot,XDY_Horodate,XDY_Booleen.
*       Le message declenchera XZAT;23 une SP n effectuant pas de select et ne prenant pas
*       de parametre de retour.
*       
------------------------------------------------------*/
{
        static char *   version = "@(#)xzat.c   1.8 01/29/98 : xzat520_CompteRendu_CFE" ;

        XDY_Datagroup   vl_datagroup;
        XDY_NomSite     pl_NomSite;
        T_BOOL          vl_EtatDg = FALSE;
        int             vl_retcode = XDC_NOK;

        XZST_03EcritureTrace(XZSTC_INTERFACE,"IN  : xzat521_CompteRendu_CFE : Debut d execution\n a recu Num_Action = %d\n Horodate = %lf\n ResultatAction = %d",va_Num_Action,va_Horodate_double,va_ResultatAction);

        /*A
        ** Recuperation du site de la machine 
        */
        if ( (vl_retcode = XZSC_04NomSite(pl_NomSite)) != XDC_OK)
        {
                XZST_03EcritureTrace( XZSTC_WARNING, "xzat521_CompteRendu_CFE : Recuperation du Nom du site impossible .");
                return( XDC_NOK);
        }

        XZST_03EcritureTrace( XZSTC_DEBUG1, "xzat521_CompteRendu_CFE : Recuperation du Nom du site %s.",pl_NomSite);

        /*A
        ** Constitution du nom Data Group a partir du nom du site
        */
		switch (va_SiteGestion) 
		{
			case (XDC_CI) : XDG_EncodeDG2(vl_datagroup,XDG_AT,XDC_NOM_SITE_CI);
					break;
			case (XDC_VC) : XDG_EncodeDG2(vl_datagroup,XDG_AT,XDC_NOM_SITE_VC);
					break;
			case (XDC_DP) : XDG_EncodeDG2(vl_datagroup,XDG_AT,XDC_NOM_SITE_DP);
					break;
			case (XDC_CA) : XDG_EncodeDG2(vl_datagroup,XDG_AT,XDC_NOM_SITE_CA);
					break;
			default: XDG_EncodeDG2(vl_datagroup,XDG_AT,pl_NomSite);
					break;
		}
        XZST_03EcritureTrace ( XZSTC_DEBUG1, "xzat521_CompteRendu_CFE : Data group a joindre : %s",vl_datagroup);


        /*A
        ** Envoi du message XDM_AT_CFE 
        */
        if (TipcSrvMsgWrite(vl_datagroup,XDM_IdentMsg(XDM_AT_CFE) ,
                                XDM_FLG_SRVMSGWRITE,
                                T_IPC_FT_INT4, va_Num_Action,
                                T_IPC_FT_REAL8,va_Horodate_double,
                                T_IPC_FT_CHAR, va_ResultatAction,
                                NULL))
        {
                XZST_03EcritureTrace(XZSTC_WARNING,"xzat521_CompteRendu_CFE a reussi a envoyer le msg XDM_AT_CFE" );
        }
        else
        {
                XZST_03EcritureTrace(XZSTC_WARNING ,"xzat521_CompteRendu_CFE : TipcSrvMsgWrite a echoue");
                XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : xzat521_CompteRendu_CFE a echoue : Sortie");
                return(XDC_NOK);
        }
        /*A
        ** Fin de l interface xzat521_CompteRendu_CFE
        */
        XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT : xzat521_CompteRendu_CFE : Fin d execution");
        return(XDC_OK);
}
/*CFE FIN*/

/* IMU DEBUT */
/*X*/ 
/*---------------------------------------------------- 
* SERVICE RENDU :  
*       xzat253_CompteRendu_IMU permet de mettre le compte rendu 
*       d une action en base. 
*       Algo : Recoit les parametres,  
*       constitue un msg RTwks et 
*       envoie le message RTWks XDM_AT_IMU. 
------------------------------------------------------ 
* SEQUENCE D'APPEL : 
*/ 								
int xzat253_CompteRendu_IMU (   XDY_Entier      va_Num_Action, 
                                XDY_Horodate    va_Horodate_double, 
                                XDY_Booleen     va_ResultatAction, 
                                XDY_District va_SiteGestion) 
/* 
* ARGUMENTS EN ENTREE : 
* 
*       Arguments d'une callback RTServer. 
* 
* ARGUMENTS EN SORTIE : 
* 
*   aucun 
* 
* CODE RETOUR :  
*  XDC_OK si tout s est bien passe 
*  sinon renvoie XDC_NOK. 
* 
* CONDITION D'UTILISATION 
* 
*       Il faut etre connecte au RTserveur local. 
*        
* 
* 
* FONCTION  
*       Fonction permettant d envoyer le message XDM_AT_IMU. 
*       Le message RTwks doit contenir un XDY_Mot,XDY_Horodate,XDY_Booleen. 
*       Le message declenchera XZAT;23 une SP n effectuant pas de select et ne prenant pas 
*       de parametre de retour. 
*        
------------------------------------------------------*/ 
{ 
        static char *   version = "@(#)xzat.c   1.8 01/29/98 : xzat253_CompteRendu_IMU " ; 
 
        XDY_Datagroup   vl_datagroup; 
        XDY_NomSite     pl_NomSite; 
        T_BOOL          vl_EtatDg = FALSE; 
        int             vl_retcode = XDC_NOK; 
 
        XZST_03EcritureTrace(XZSTC_INTERFACE,"IN  : xzat253_CompteRendu_IMU : Debut d execution\n a recu Num_Action = %d\n Horodate = %lf\n ResultatAction = %d",va_Num_Action,va_Horodate_double,va_ResultatAction); 
 
        /*A 
        ** Recuperation du site de la machine  
        */ 
        if ( (vl_retcode = XZSC_04NomSite(pl_NomSite)) != XDC_OK) 
        { 
                XZST_03EcritureTrace( XZSTC_WARNING, "xzat253_CompteRendu_IMU: Recuperation du Nom du site impossible ."); 
                return( XDC_NOK); 
        } 
 
        XZST_03EcritureTrace( XZSTC_DEBUG1, "xzat253_CompteRendu_IMU : Recuperation du Nom du site %s.",pl_NomSite); 
 
        /*A 
        ** Constitution du nom Data Group a partir du nom du site 
        */ 
 
         switch (va_SiteGestion) { 
         case (XDC_CI) : XDG_EncodeDG2(vl_datagroup,XDG_AT,XDC_NOM_SITE_CI); 
                              break; 
         case (XDC_VC) : XDG_EncodeDG2(vl_datagroup,XDG_AT,XDC_NOM_SITE_VC); 
                              break; 
         case (XDC_DP) : XDG_EncodeDG2(vl_datagroup,XDG_AT,XDC_NOM_SITE_DP); 
                              break; 
         case (XDC_CA) : XDG_EncodeDG2(vl_datagroup,XDG_AT,XDC_NOM_SITE_CA); 
                              break; 
         default: XDG_EncodeDG2(vl_datagroup,XDG_AT,pl_NomSite); 
                                break; 
        } 
        XZST_03EcritureTrace ( XZSTC_DEBUG1, " xzat253_CompteRendu_IMU: Data group a joindre : %s",vl_datagroup); 
 
 
        /*A 
        ** Envoi du message XDM_AT_IMU  
        */ 
        if (TipcSrvMsgWrite(vl_datagroup,XDM_IdentMsg(XDM_AT_IMU) , 
                                XDM_FLG_SRVMSGWRITE, 
                                T_IPC_FT_INT4, va_Num_Action, 
                                T_IPC_FT_REAL8,va_Horodate_double, 
                                T_IPC_FT_CHAR, va_ResultatAction, 
                                NULL)) 
        { 
                XZST_03EcritureTrace(XZSTC_WARNING," xzat253_CompteRendu_IMU a reussi a envoyer le msg XDM_AT_IMU" ); 
        } 
        else 
        { 
                XZST_03EcritureTrace(XZSTC_WARNING ," xzat253_CompteRendu_IMU: TipcSrvMsgWrite a echoue"); 
                XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : xzat253_CompteRendu_IMU a echoue : Sortie"); 
                return(XDC_NOK); 
        } 
        /*A 
        ** Fin de l interface xzat253_CompteRendu_IMU
        */ 
        XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT : xzat253_CompteRendu_IMU: Fin d execution"); 
        return(XDC_OK); 
} 	
/* IMU FIN */

/* RAU DEBUT */
/*X*/ 
/*---------------------------------------------------- 
* SERVICE RENDU :  
*       xzat551_CompteRendu_RAU_SONO permet de mettre le compte rendu 
*       d une action en base. 
*       Algo : Recoit les parametres,  
*       constitue un msg RTwks et 
*       envoie le message RTWks XDM_AT_SONO. 
------------------------------------------------------ 
* SEQUENCE D'APPEL : 
*/ 								
int xzat551_CompteRendu_RAU_SONO (   XDY_Entier      va_Num_Action, 
                                XDY_Horodate    va_Horodate_double, 
                                XDY_Booleen     va_ResultatAction, 
                                XDY_District va_SiteGestion,
				XDY_Mot	     va_Nb_PAUErreur, 
				XDY_Mot	     va_Nb_PAUPilotes) 
/* 
* ARGUMENTS EN ENTREE : 
* 
*       Arguments d'une callback RTServer. 
* 
* ARGUMENTS EN SORTIE : 
* 
*   aucun 
* 
* CODE RETOUR :  
*  XDC_OK si tout s est bien passe 
*  sinon renvoie XDC_NOK. 
* 
* CONDITION D'UTILISATION 
* 
*       Il faut etre connecte au RTserveur local. 
*        
* 
* 
* FONCTION  
*       Fonction permettant d envoyer le message XDM_AT_RAU_SONO. 
*       Le message RTwks doit contenir un XDY_Mot,XDY_Horodate,XDY_Booleen. 
*       Le message declenchera XZAT550 une SP n effectuant pas de select et ne prenant pas 
*       de parametre de retour. 
*        
------------------------------------------------------*/ 
{ 
        static char *   version = "@(#)xzat.c   1.8 01/29/98 : xzat551_CompteRendu_RAU_SONO " ; 
 
        XDY_Datagroup   vl_datagroup; 
        XDY_NomSite     pl_NomSite; 
        T_BOOL          vl_EtatDg = FALSE; 
        int             vl_retcode = XDC_NOK; 
 
        XZST_03EcritureTrace(XZSTC_INTERFACE,"IN  : xzat551_CompteRendu_RAU_SONO : Debut d execution\n a recu Num_Action = %d\n Horodate = %lf\n ResultatAction = %d",va_Num_Action,va_Horodate_double,va_ResultatAction); 
 
        /*A 
        ** Recuperation du site de la machine  
        */ 
        if ( (vl_retcode = XZSC_04NomSite(pl_NomSite)) != XDC_OK) 
        { 
                XZST_03EcritureTrace( XZSTC_WARNING, "xzat551_CompteRendu_RAU_SONO: Recuperation du Nom du site impossible ."); 
                return( XDC_NOK); 
        } 
 
        XZST_03EcritureTrace( XZSTC_DEBUG1, "xzat551_CompteRendu_RAU_SONO : Recuperation du Nom du site %s.",pl_NomSite); 
 
        /*A 
        ** Constitution du nom Data Group a partir du nom du site 
        */ 
 
         switch (va_SiteGestion) { 
         case (XDC_CI) : XDG_EncodeDG2(vl_datagroup,XDG_AT,XDC_NOM_SITE_CI); 
                              break; 
         case (XDC_VC) : XDG_EncodeDG2(vl_datagroup,XDG_AT,XDC_NOM_SITE_VC); 
                              break; 
         case (XDC_DP) : XDG_EncodeDG2(vl_datagroup,XDG_AT,XDC_NOM_SITE_DP); 
                              break; 
         case (XDC_CA) : XDG_EncodeDG2(vl_datagroup,XDG_AT,XDC_NOM_SITE_CA); 
                              break; 
         default: XDG_EncodeDG2(vl_datagroup,XDG_AT,pl_NomSite); 
                                break; 
        } 
        XZST_03EcritureTrace ( XZSTC_DEBUG1, " xzat551_CompteRendu_RAU_SONO: Data group a joindre : %s",vl_datagroup); 
 
 
        /*A 
        ** Envoi du message XDM_AT_RAU_SONO  
        */ 
        if (TipcSrvMsgWrite(vl_datagroup,XDM_IdentMsg(XDM_AT_SONO) , 
                                XDM_FLG_SRVMSGWRITE, 
                                T_IPC_FT_INT4, va_Num_Action, 
                                T_IPC_FT_REAL8,va_Horodate_double, 
                                T_IPC_FT_CHAR, va_ResultatAction,
				T_IPC_FT_INT2, va_Nb_PAUErreur, 
				T_IPC_FT_INT2, va_Nb_PAUPilotes,
                                NULL)) 
        { 
                XZST_03EcritureTrace(XZSTC_WARNING," xzat551_CompteRendu_RAU_SONO a reussi a envoyer le msg XDM_AT_RAU_SONO" ); 
        } 
        else 
        { 
                XZST_03EcritureTrace(XZSTC_WARNING ," xzat551_CompteRendu_RAU_SONO: TipcSrvMsgWrite a echoue"); 
                XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : xzat551_CompteRendu_RAU_SONO a echoue : Sortie"); 
                return(XDC_NOK); 
        } 
        /*A 
        ** Fin de l interface xzat551_CompteRendu_RAU
        */ 
        XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT : xzat551_CompteRendu_RAU_SONO: Fin d execution"); 
        return(XDC_OK); 
} 	
/* RAU FIN */									


/*PIC DEBUT*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	xzat25_CompteRendu_PIC permet de mettre le compte rendu
*	d une action en base.
*	Algo : Recoit les parametres, 
*	constitue un msg RTwks et
* 	envoie le message RTWks XDM_AT_PIC.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int xzat5201_CompteRendu_PIC (	XDY_Entier		va_Num_Action,
								XDY_Horodate 	va_Horodate_double,
								XDY_Booleen  	va_ResultatAction,
								XDY_District 	va_SiteGestion) 
/*
* ARGUMENTS EN ENTREE :
*
*	Arguments d'une callback RTServer.
*
* ARGUMENTS EN SORTIE :
*
*	aucun
*
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon renvoie XDC_NOK.
*
* CONDITION D'UTILISATION
*
*	Il faut etre connecte au RTserveur local.
*
* FONCTION 
*	Fonction permettant d envoyer le message XDM_AT_PMV.
*	Le message RTwks doit contenir un XDY_Mot,XDY_Horodate,XDY_Booleen.
*	Le message declenchera XZAT;23 une SP n effectuant pas de select et ne prenant pas
*	de parametre de retour.
* 	
------------------------------------------------------*/
{
	static char *	version = "@(#)xzat.c : xzat5201_CompteRendu_PIC" ;
	
	XDY_Datagroup   vl_datagroup;
	XDY_NomSite 	pl_NomSite;
	T_BOOL			vl_EtatDg = FALSE;
	int				vl_retcode = XDC_NOK;
        
	XZST_03EcritureTrace(XZSTC_INTERFACE,"IN : xzat5201_CompteRendu_PIC : Debut d execution\n a recu Num_Action = %d\n Horodate = %lf\n ResultatAction = %d",va_Num_Action,va_Horodate_double,va_ResultatAction);
	
	/* Recuperation du site de la machine */
	if ( (vl_retcode = XZSC_04NomSite(pl_NomSite)) != XDC_OK)
	{
		XZST_03EcritureTrace( XZSTC_WARNING, "xzat5201_CompteRendu_PIC : Recuperation du Nom du site impossible.");
		return( XDC_NOK);
	} 
	XZST_03EcritureTrace( XZSTC_DEBUG1, "xzat5201_CompteRendu_PIC : Recuperation du Nom du site %s.", pl_NomSite);
	
	/* Constitution du nom Data Group a partir du nom du site */
    switch (va_SiteGestion) 
	{
	 	case (XDC_CI) : XDG_EncodeDG2(vl_datagroup,XDG_AT,XDC_NOM_SITE_CI);
			break;
	 	case (XDC_VC) : XDG_EncodeDG2(vl_datagroup,XDG_AT,XDC_NOM_SITE_VC);
			break;
	 	case (XDC_DP) : XDG_EncodeDG2(vl_datagroup,XDG_AT,XDC_NOM_SITE_DP);
			break;
	 	case (XDC_CA) : XDG_EncodeDG2(vl_datagroup,XDG_AT,XDC_NOM_SITE_CA);
			break;
	 	default: XDG_EncodeDG2(vl_datagroup,XDG_AT,pl_NomSite);
			break;
	}
	XZST_03EcritureTrace ( XZSTC_DEBUG1, "xzat5201_CompteRendu_PIC : Data group a joindre : %s",vl_datagroup);
	
	/* Envoi du message XDM_AT_PMV */ 
	if (TipcSrvMsgWrite(vl_datagroup,XDM_IdentMsg(XDM_AT_PIC) ,
				XDM_FLG_SRVMSGWRITE,
				T_IPC_FT_INT4, va_Num_Action, 
				T_IPC_FT_REAL8,va_Horodate_double, 
				T_IPC_FT_CHAR, va_ResultatAction, 
				NULL))
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzat5201_CompteRendu_PIC a reussi a envoyer le msg XDM_AT_PIC" );
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_WARNING ,"xzat5201_CompteRendu_PIC : TipcSrvMsgWrite a echoue");	
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : xzat5201_CompteRendu_PIC a echoue : Sortie");
		return(XDC_NOK);	 
	}

	/* Fin de l interface xzat5201_CompteRendu_PIC */
	XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT : xzat5201_CompteRendu_PIC : Fin d execution");
	return(XDC_OK);
}
/*PIC FIN*/