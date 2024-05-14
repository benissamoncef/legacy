/*E*/
/*  Fichier : 	$Id: xzac103.c,v 1.5 2017/03/09 17:49:33 pc2dpdy Exp $  Release : $Revision: 1.5 $ Date : $Date: 2017/03/09 17:49:33 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAC * FICHIER xzac103.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   < Module Interface Actions >
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	12/11/04	creation
* LCL   09/07/07        suppression DY
* JMG   29/09/09        SECTO : DEM 887
* JMG   09/03/17        : REGIONALISATION - DEM1220 1.5
* LCL	27/09/23	: Adaptations CNA DEM-483
------------------------------------------------------*/

/* fichiers inclus */

#include "xzac103.h"


/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  envoie XDM_ENAV_AUTO 
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAC103_Commande_NAV_TACLI ( XDY_Entier    va_NumEvt_in,
                                 XDY_Octet     va_CleEvt_in,
                                 XDY_Operateur va_Operateur_in,
                                 XDY_Eqt       va_NumEqt_in,
                                 XDY_Horodate  va_HeureLancement_in,
                                 XDY_Booleen   va_Manuelle_in,
                                 XDY_Octet     va_Priorite_in,
                                 XDY_Entier    va_DistanceEvt_in,
                                 char      *va_Scenario_in,
                                 XDY_Octet      va_Site_in,
                                 XDY_Octet      va_site_origine_in)

/*X*/
/*-----------------------------------------------------
* Service rendu
* commande de NAV via TACLI
* 
* Sequence d'appel
* 
* Code retour
* XDC_OK
* XDC_NOK	: evenement non trouve
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* 
* Fonction
* 
------------------------------------------------*/
{
  char            vl_dg[50]="";

    switch (va_Site_in) {
     case (XDC_CI) : XDG_EncodeDG2(vl_dg,XDG_ENAV_AUTO,XDC_NOM_SITE_CI);
                     break;
     case (XDC_VC) : XDG_EncodeDG2(vl_dg,XDG_ENAV_AUTO,XDC_NOM_SITE_VC);
                     break;
     case (XDC_DP) : XDG_EncodeDG2(vl_dg,XDG_ENAV_AUTO,XDC_NOM_SITE_DP);
                     break;
     case (XDC_CA) : XDG_EncodeDG2(vl_dg,XDG_ENAV_AUTO,XDC_NOM_SITE_CA);
                     break;
    }

    if (TipcSrvMsgWrite(vl_dg,XDM_IdentMsg(XDM_ENAV_AUTO) ,
                                XDM_FLG_SRVMSGWRITE,
                                T_IPC_FT_INT4, va_NumEvt_in,
                                T_IPC_FT_INT2, va_CleEvt_in,
                                T_IPC_FT_INT2, va_Operateur_in,
                                T_IPC_FT_INT2, va_NumEqt_in,
                                T_IPC_FT_REAL8, va_HeureLancement_in,
                                T_IPC_FT_INT2, va_Manuelle_in,
                                T_IPC_FT_INT2, va_Priorite_in,
                                T_IPC_FT_INT4, va_DistanceEvt_in,
                                T_IPC_FT_STR, va_Scenario_in,
                                T_IPC_FT_INT2, va_Site_in,
                                T_IPC_FT_INT2, va_site_origine_in,
                                NULL)) {
        XZST_03EcritureTrace(XZSTC_FONCTION,
                "XZAC103 a reussi a envoyer le msg XDM_ENAV_AUTO sur %s",vl_dg );
        return (XDC_OK);
      }
      else
        XZST_03EcritureTrace(XZSTC_WARNING ,
                        "XZAC103 : TipcSrvMsgWrite XDM_ENAV_AUTO a echoue");
      return (XDC_NOK);
}




