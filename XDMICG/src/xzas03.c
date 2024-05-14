/*E*/
/*Fichier : $Id: xzas03.c,v 1.3 2020/11/03 17:57:25 pc2dpdy Exp $      Release : $Revision: 1.3 $        Date : $Date: 2020/11/03 17:57:25 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAS03 * FICHIER xzas03.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Ce module appartient a TACLI. Il permet d animer les courbes des synoptiques
*
------------------------------------------------------
* HISTORIQUE :
*
* gaborit	14/01/95	: Creation
* niepceron	02 Jul 1998	: separation EXP/HIS (dem/1699) v1.2
* JMG		15/05/20 	: EMS 1.3
------------------------------------------------------*/

/* fichiers inclus */

#include "xzas03.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: xzas03.c,v 1.3 2020/11/03 17:57:25 pc2dpdy Exp $ : xzas03" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	XZAS03_Bouchon permet de demander l envoie des mesures Bouchon pour graphe Inrets
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAS03_Bouchon (    XDY_Datagroup   va_datagroup_specif,
                                XDY_Autoroute   va_Autoroute,
                                XDY_PR          va_PR_Debut,
                                XDY_PR          va_PR_Fin,
                                XDY_Horodate    va_HorodateDebut )
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
	static char *	version = "$Id: xzas03.c,v 1.3 2020/11/03 17:57:25 pc2dpdy Exp $ : XZAS03_Bouchon" ;
	
	int			vl_JourSemaine=0;
	double			vl_HorodateSec=0;
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
	
	/*A
	** Envoi du message XDM_AS_BOUCHON. 
	*/ 
	if (TipcSrvMsgWrite(	vl_datagroup,
				XDM_IdentMsg(XDM_AS_BOUCHON) ,
				XDM_FLG_SRVMSGWRITE,
				T_IPC_FT_STR,	(T_STR) va_datagroup_specif,
				T_IPC_FT_CHAR,	(T_CHAR) va_Autoroute,
				T_IPC_FT_INT4,	(T_INT4) va_PR_Debut,
				T_IPC_FT_INT4,	(T_INT4) va_PR_Fin,
				T_IPC_FT_REAL8,	(T_REAL8) va_HorodateDebut,
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
	** Fin de l interface XZAS03
	*/
	XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT : %s : Fin d execution",version);
	return(XDC_OK);
}

