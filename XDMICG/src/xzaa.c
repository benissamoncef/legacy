/*E*/
/*Fichier : $Id: xzaa.c,v 1.10 2017/03/09 17:47:28 pc2dpdy Exp $      Release : $Revision: 1.10 $        Date : $Date: 2017/03/09 17:47:28 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAA * FICHIER xzaa.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale
*   ou reference au fichier xzaa.h >
*
------------------------------------------------------
* HISTORIQUE :
*
* mercier	24 Oct 1994	: Creation
* gaborit	26 jan 1995	: ajout xzaa10_Equation (1.3)
* gaborit	27 jan 1995	: ajout horodate a xzaa10_Equation (1.4)
* mercier	17 Fev 1995	: Correction des envoies de msg		V 1.5
*     : on n utilise pas le TipcMsgCreate.
* JMG   31/10/07        : ajout site de gestion 1.6
* JMG           29/09/09        : SECTO : DEM 887
* JMG	26/06/01 : linux DEM 934
* JMG   09/03/17        : REGIONALISATION - DEM1220 1.10
* LCL	27/09/23	: Adaptations CNA DEM-483
------------------------------------------------------*/

/* fichiers inclus */

#include "xzaa.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: xzaa.c,v 1.10 2017/03/09 17:47:28 pc2dpdy Exp $ : xzaa" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	xzaa01_Creer_Alerte permet de mettre le compte rendu
*	d une action en base.
*	Algo : Recoit les parametres, 
*	constitue un msg RTwks et
* 	envoie le message RTWks XDM_AA_alerte.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int xzaa01_Creer_Alerte (	XDY_Horodate 	va_Horodate_double_in,
				XDY_TypeEqt	va_TypeEqt_in,
				XDY_Eqt		va_Eqt_in,
				XDY_TypeAlerte	va_TypeAlerte_in,
				XDY_TexteAlerte	va_TexteAlerte_in,
				XDY_District	va_SiteGestion_in) 
/*
* ARGUMENTS EN ENTREE :
*	va_Horodate_double_in 
*   	va_TypeEqt_in
*	va_Eqt_in
*	va_Type_in
*	va_TexteAlerte_in
*	constituent le contenu du meessage a envoyer sur le DG XDG_AA_NOMSITE.
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
*	Fonction permettant d envoyer le message XDM_AA_alerte.
*	Le message RTwks doit contenir un XDY_Horodate XDY_TypeEqt XDY_Eqt XDY_TypeAlerte XDY_TexteAlerte.
*	Le message declenchera XZAA;01 une SP n effectuant pas de select et ne prenant pas
*	de parametre de retour.
* 	
------------------------------------------------------*/
{
	static char *	version = "@(#)xzaa.c	1.5 02/17/95 : xzaa01_Creer_Alerte" ;
	
	XDY_Datagroup   vl_datagroup;
	XDY_NomSite 	pl_NomSite;
        T_BOOL		vl_EtatDg = FALSE;
        int		vl_retcode = XDC_NOK;
        T_IPC_MSG 	pl_Msg;
        
#ifdef _HPUX_SOURCE
	XZST_03EcritureTrace(XZSTC_INTERFACE,"IN  : xzaa01_Creer_Alerte : Debut d execution\n a recu Horodate 	= %ld\n TypeEqt 	= %d\n Eqt 		= %d\nTypeAlerte 		= %d\nTexteAlerte = %s\n",va_Horodate_double_in,va_TypeEqt_in,va_Eqt_in,va_TypeAlerte_in,va_TexteAlerte_in);
#endif
	/*A
	** Recuperation du site de la machine 
	*/
	if ( (vl_retcode = XZSC_04NomSite(pl_NomSite)) != XDC_OK)
	{
		/*B 
		** Ecriture Trace 
		*/
		XZST_03EcritureTrace( XZSTC_WARNING, "xzaa01 : Recuperation du Nom du site impossible .");
		return( XDC_NOK);
	} 	
	/* 
	** Affichage Trace 
	*/
	XZST_03EcritureTrace( XZSTC_DEBUG1, "xzaa01 : Recuperation du Nom du site %s.",pl_NomSite);
	
	/*A
	** Constitution du nom Data Group a partir du nom du site
	*/
    switch (va_SiteGestion_in) {
     	case (XDC_CI) : XDG_EncodeDG2(vl_datagroup,XDG_AA,XDC_NOM_SITE_CI);
                     break;
     	case (XDC_VC) : XDG_EncodeDG2(vl_datagroup,XDG_AA,XDC_NOM_SITE_VC);
                     break;
     	case (XDC_DP) : XDG_EncodeDG2(vl_datagroup,XDG_AA,XDC_NOM_SITE_DP);
                     break;
     	case (XDC_CA) : XDG_EncodeDG2(vl_datagroup,XDG_AA,XDC_NOM_SITE_CA);
                     break;
        default : XDG_EncodeDG2(vl_datagroup,XDG_AA,pl_NomSite);
                        break;
    }
	
	XZST_03EcritureTrace ( XZSTC_DEBUG1, "xzaa01 : Data group a joindre : %s",vl_datagroup);
	
	/*A
	** Envoi du message XDM_AA_alerte 
	*/ 
	if (TipcSrvMsgWrite(vl_datagroup,XDM_IdentMsg(XDM_AA_alerte) ,
				XDM_FLG_SRVMSGWRITE,
				T_IPC_FT_REAL8, va_Horodate_double_in,
				T_IPC_FT_CHAR, va_TypeEqt_in,  
				T_IPC_FT_INT2, va_Eqt_in,
				T_IPC_FT_CHAR, va_TypeAlerte_in,
				T_IPC_FT_STR, va_TexteAlerte_in,
				NULL))
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1,"xzaa01 a reussi a envoyer le msg XDM_AA_alerte" );	
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_WARNING ,"xzaa01_Creer_Alerte : TipcSrvMsgWrite a echoue");	
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : xzaa01_Creer_Alerte a echoue : Sortie");
		return(XDC_NOK);	 
	}
	/*A
	** Fin de l interface xzaa01
	*/
	XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT : xzaa01_Creer_Alerte : Fin d execution");
	return(XDC_OK);
}
/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	xzaa10_Equation permet de lancer le calcul des equations evt
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int xzaa10_Equation (	XDY_Octet 	va_TypeFmc_in,
			XDY_Entier	va_VR_in,
			XDY_Entier	va_VM2_in,
			XDY_Entier	va_VM1_in,
			XDY_Entier	va_VL_in,
			XDY_Entier	va_BAU_in,
			XDY_Entier	va_VR_I_in,
			XDY_Entier	va_VM2_I_in,
			XDY_Entier	va_VM1_I_in,
			XDY_Entier	va_VL_I_in,
			XDY_Entier	va_BAU_I_in,
			float		va_Duree_in,
			float		va_Longueur_in,
			XDY_Horodate	va_Horodate_in,
			XDY_Eqt		va_Machine_in
			) 
/*
* ARGUMENTS EN ENTREE :
*	XDY_Octet 	va_TypeFmc_in,
*	XDY_Entier	va_VR_in,
*	XDY_Entier	va_VM2_in,
*	XDY_Entier	va_VM1_in,
*	XDY_Entier	va_VL_in,
*	XDY_Entier	va_BAU_in,
*	XDY_Entier	va_VR_I_in,
*	XDY_Entier	va_VM2_I_in,
*	XDY_Entier	va_VM1_I_in,
*	XDY_Entier	va_VL_I_in,
*	XDY_Entier	va_BAU_I_in,
*	float		va_Duree_in,  date de fin prevue - date courante en minutes (>0 si ok, 0 sinon) 
*	float		va_Longueur_in  longueur en kilometres si evt etendu, 0 sinon 
*	XDY_Horodate	va_Horodate_in,
*	XDY_Eqt		va_Machine_in
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
	static char *	version = "@(#)xzaa.c	1.5 02/17/95 : xzaa10_Equation" ;
	
	XDY_Datagroup   vl_datagroup;
	XDY_NomSite 	pl_NomSite;
        T_BOOL		vl_EtatDg = FALSE;
        int		vl_retcode = XDC_NOK;
        T_IPC_MSG 	pl_Msg;
        
	XZST_03EcritureTrace(XZSTC_INTERFACE,"IN  : %s : Debut d execution",version);
	
	/*A
	** Recuperation du site de la machine 
	*/
	if ( (vl_retcode = XZSC_04NomSite(pl_NomSite)) != XDC_OK)
	{
		XZST_03EcritureTrace( XZSTC_WARNING, "%s : Recuperation du Nom du site impossible .",version);
		return( XDC_NOK);
	} 	
	XZST_03EcritureTrace( XZSTC_DEBUG1, "%s : Recuperation du Nom du site %s.",version,pl_NomSite);
	
	/*A
	** Constitution du nom Data Group a partir du nom du site
	*/
	XDG_EncodeDG2(vl_datagroup,XDG_AEQA,pl_NomSite);
	
	XZST_03EcritureTrace ( XZSTC_DEBUG1, "%s : Data group a joindre : %s",version,vl_datagroup);
	
	
	/*A
	** Envoi du message XDM_AEQA 
	*/ 
	if (TipcSrvMsgWrite(vl_datagroup,XDM_IdentMsg(XDM_AEQA) ,
				XDM_FLG_SRVMSGWRITE,
				T_IPC_FT_CHAR,	va_TypeFmc_in,
				T_IPC_FT_INT4,	va_VR_in,
				T_IPC_FT_INT4,	va_VM2_in,
				T_IPC_FT_INT4,	va_VM1_in,
				T_IPC_FT_INT4,	va_VL_in,
				T_IPC_FT_INT4,	va_BAU_in,
				T_IPC_FT_INT4,	va_VR_I_in,
				T_IPC_FT_INT4,	va_VM2_I_in,
				T_IPC_FT_INT4,	va_VM1_I_in,
				T_IPC_FT_INT4,	va_VL_I_in,
				T_IPC_FT_INT4,	va_BAU_I_in,
				T_IPC_FT_REAL8,	va_Duree_in,
				T_IPC_FT_REAL8,	va_Longueur_in,
				T_IPC_FT_REAL8,	va_Horodate_in,
				T_IPC_FT_INT2,	va_Machine_in,
				NULL))
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1,"%s a reussi a envoyer le msg XDM_AEQA",version );	
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_WARNING ,"%s : TipcSrvMsgWrite a echoue",version);	
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : %s a echoue : Sortie",version);
		return(XDC_NOK);	 
	}
	/*A
	** Fin de l interface xzaa10
	*/
	XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT : %s : Fin d execution",version);
	return(XDC_OK);
}



/*X*/
/*----------------------------------------------------
* SERVICE RENDU :
*       xzaa01_Creer_Alerte permet de mettre le compte rendu
*       d une action en base.
*       Algo : Recoit les parametres,
*       constitue un msg RTwks et
*       envoie le message RTWks XDM_AA_alerte.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int xzaa01_Creer_Alerte_Localisee (     XDY_Horodate    va_Horodate_double_in,
                                XDY_TypeEqt     va_TypeEqt_in,
                                XDY_Eqt         va_Eqt_in,
                                XDY_TypeAlerte  va_TypeAlerte_in,
                                XDY_TexteAlerte va_TexteAlerte_in,
                                XDY_District    va_SiteGestion_in,
                                char    *va_autoroute_in,
                                XDY_PR va_pr_in,
                                int va_sens_in)
/*
* ARGUMENTS EN ENTREE :
*       va_Horodate_double_in
*       va_TypeEqt_in
*       va_Eqt_in
*       va_Type_in
*       va_TexteAlerte_in
*       constituent le contenu du meessage a envoyer sur le DG XDG_AA_NOMSITE.
*
* ARGUMENTS EN SORTIE : aucun
* CODE RETOUR :
*  XDC_OK si tout s est bien passe
*  sinon renvoie XDC_NOK.
*
* CONDITION D'UTILISATION
*
*       Il faut etre connecte au RTserveur local.
*
*
* FONCTION
*       Fonction permettant d envoyer le message XDM_AA_alerte.
*       Le message RTwks doit contenir un XDY_Horodate XDY_TypeEqt XDY_Eqt XDY_TypeAlerte XDY_TexteAlerte.
*       Le message declenchera XZAA;01 une SP n effectuant pas de select et ne prenant pas
*       de parametre de retour.
*
------------------------------------------------------*/
{
        static char *   version = "@(#)xzaa.c   1.5 02/17/95 : xzaa01_Creer_Alerte" ;

        XDY_Datagroup   vl_datagroup;
        XDY_NomSite     pl_NomSite;
        T_BOOL          vl_EtatDg = FALSE;
        int             vl_retcode = XDC_NOK;
        T_IPC_MSG       pl_Msg;

#ifdef _HPUX_SOURCE
        XZST_03EcritureTrace(XZSTC_INTERFACE,"IN  : xzaa01_Creer_Alerte_localisee : Debut d execution\n a recu Horodate   = %ld\n TypeEqt         = %d\n Eqt = %d\nTypeAlerte                = %d\nTexteAlerte = %s\n",va_Horodate_double_in,va_TypeEqt_in,va_Eqt_in,va_TypeAlerte_in,va_TexteAlerte_in);
#endif
        /*A
        ** Recuperation du site de la machine
        */
        if ( (vl_retcode = XZSC_04NomSite(pl_NomSite)) != XDC_OK)
        {
                /*B
                ** Ecriture Trace
                */
                XZST_03EcritureTrace( XZSTC_WARNING, "xzaa01 : Recuperation du Nom du site impossible .");
                return( XDC_NOK);
        }
        /*
        ** Affichage Trace
        */
        XZST_03EcritureTrace( XZSTC_DEBUG1, "xzaa01 : Recuperation du Nom du site %s.",pl_NomSite);
        /*A
        ** Constitution du nom Data Group a partir du nom du site
        */
    switch (va_SiteGestion_in) {
     	case (XDC_CI) : XDG_EncodeDG2(vl_datagroup,XDG_AA,XDC_NOM_SITE_CI);
                     break;
     	case (XDC_VC) : XDG_EncodeDG2(vl_datagroup,XDG_AA,XDC_NOM_SITE_VC);
                     break;
     	case (XDC_DP) : XDG_EncodeDG2(vl_datagroup,XDG_AA,XDC_NOM_SITE_DP);
                     break;
     	case (XDC_CA) : XDG_EncodeDG2(vl_datagroup,XDG_AA,XDC_NOM_SITE_CA);
                     break;
        default : XDG_EncodeDG2(vl_datagroup,XDG_AA,pl_NomSite);
                        break;
    }

        XZST_03EcritureTrace ( XZSTC_DEBUG1, "xzaa01_localise : Data group a joindre : %s",vl_datagroup);

        XZST_03EcritureTrace ( XZSTC_WARNING,"SENS %d",va_sens_in);
        /*A
        ** Envoi du message XDM_AA_alerte_localisee
        */
	if (TipcSrvMsgWrite(vl_datagroup,XDM_IdentMsg(XDM_AA_alerte_localisee) ,
                                XDM_FLG_SRVMSGWRITE,
                                T_IPC_FT_REAL8, va_Horodate_double_in,
                                T_IPC_FT_CHAR, va_TypeEqt_in,
                                T_IPC_FT_INT2, va_Eqt_in,
                                T_IPC_FT_CHAR, va_TypeAlerte_in,
                                T_IPC_FT_STR, va_TexteAlerte_in,
                                T_IPC_FT_STR, va_autoroute_in,
                                T_IPC_FT_INT4, va_pr_in,
                                T_IPC_FT_INT4, va_sens_in,
                                NULL))
        {
                XZST_03EcritureTrace(XZSTC_DEBUG1,"xzaa01_localise a reussi a envoyer le msg XDM_AA_alerte" );
        }
        else
        {
                XZST_03EcritureTrace(XZSTC_WARNING ,"xzaa01_Creer_Alerte_localise : TipcSrvMsgWrite a echoue");
                XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : xzaa01_Creer_Alerte_localise a echoue : Sortie");
                return(XDC_NOK);
        }
        /*A
        ** Fin de l interface xzaa01
        */
        XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT : xzaa01_Creer_Alerte_localise : Fin d execution");
        return(XDC_OK);
}


