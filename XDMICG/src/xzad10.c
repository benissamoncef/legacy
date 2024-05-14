
/* Fichier : $Id: xzad10.c,v 1.2 2010/11/30 16:07:58 gesconf Exp $	      Release : $Revision: 1.2 $        Date : $Date: 2010/11/30 16:07:58 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZADMIN  * FICHIER xzad.c
******************************************************
* DESCRIPTION DU MODULE :
* module de calcul des temps de parcours
*
******************************************************
* HISTORIQUE :
*
*  Niepceron  03 fev 1997      : Creation
*  JPL		30/11/10 : Migration architecture HP ia64 (DEM 961) : suppression erreurs mineures  1.2
*******************************************************/

/* fichiers inclus */
#include "xzad10.h"

/* declaration des donnees globales */

/*fonctions utilisateurs passees dans les xzao40x*/



/* declaration des fonctions externes */

/* Fichier : $Id: xzad10.c,v 1.2 2010/11/30 16:07:58 gesconf Exp $        Release : $Revision: 1.2 $        Date : $Date: 2010/11/30 16:07:58 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZAD_TDP
******************************************************
* DESCRIPTION DE LA FONCTION :
* abonnement au TDP pour les ZDP d'un site
*
******************************************************
******************************************************/

int	 XZAD10_Seuil_TDP(		XDY_Eqt		va_zdp,
					XDY_Mot		va_site,
					XDY_Octet	va_typefrn,
					XDY_Horodate	va_horodate)
{
static char *version = "$Id: xzad10.c,v 1.2 2010/11/30 16:07:58 gesconf Exp $ : XZAD10_Seuil_TDP" ;
XZSCT_Datagroup vl_Dg;
int vl_code_retour=XDC_OK;
XZSCT_NomSite    vl_NomSite;

  XZST_03EcritureTrace(XZSTC_FONCTION,"XZAD10_Seuil_TDP : IN");
  
/*A Controle des Parametres*/

/*A Construction et envoi du Message */
/*A Recuperation du nom du site */
if ( XZSC_04NomSite(vl_NomSite) ==     XZSCC_INI_INV )
   {
   XZST_03EcritureTrace(   XZSTC_WARNING, "Initialisation du nom du site non effectuee " );
   return XDC_NOK;
   }  
         
XDG_EncodeDG2( vl_Dg , XDG_ETDP_SEUIL , vl_NomSite );
if (!TipcSrvMsgWrite(   vl_Dg
                        ,XDM_IdentMsg(XDM_ETDP_SEUIL) 
                        ,XDM_FLG_SRVMSGWRITE
			,T_IPC_FT_INT2,(T_INT2) va_zdp
			,T_IPC_FT_CHAR,(T_CHAR) va_site
			,T_IPC_FT_CHAR,(T_CHAR) va_typefrn
			,T_IPC_FT_REAL8,(T_REAL8) va_horodate,
			NULL) )
   {
   /* Affichage Trace */
   XZST_03EcritureTrace(XZSTC_WARNING
                        ,"XZIS02_Raf_Etat_Eqt:TipcSrvMsgWrite(%s,%u,XDM_FLG_SRVMSGWRITE,%d,%d,%d,%fNULL)" 
			,vl_Dg,XDM_IdentMsg(XDM_ETDP_SEUIL), va_zdp,  va_site, va_typefrn, va_horodate);
   vl_code_retour = XDC_NOK;
   }

  

  XZST_03EcritureTrace(XZSTC_FONCTION,"XZAD10_Seuil_TDP :  OUT");
  return (XDC_OK);
}

