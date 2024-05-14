/*E*/
/* Fichier : $Id: xzac803.c,v 1.2 2017/03/09 17:54:21 pc2dpdy Exp $      Release : $Revision: 1.2 $        Date : $Date: 2017/03/09 17:54:21 $
------------------------------------------------------
*  *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAC * FICHIER xzac101.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   < Module Interface Actions >
*
------------------------------------------------------
* HISTORIQUE :
*
* VR	02/05/12		Creation (DEM 1016)
* JMG   09/03/17        : REGIONALISATION - DEM1220 1.2
* LCL	27/09/23	: Adaptations CNA DEM-483
------------------------------------------------------*/

/* fichiers inclus */

#include "xzac803.h"


/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU :
*
*  Appelle TACLI
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAC803_Commande_PRV_TACLI ( XDY_Entier    	va_NumEvt_in,
                                 XDY_Octet     	va_CleEvt_in,
                                 XDY_Operateur 	va_Operateur_in,
                                 XDY_Eqt       	va_NumEqt_in,
                                 XDY_Horodate  	va_HeureLancement_in,
                                 XDY_Octet      va_Site_in,
                                 XDY_Bandeau	va_Bandeau_in,
                                 char      		va_TextePicto_in[20],
                                 XDY_Octet     	va_Priorite_in,
                                 XDY_Entier    	va_DistanceEvt_in,
                                 XDY_Octet      va_site_origine_in)

/*X*/
/*-----------------------------------------------------
* Service rendu
* commande de BAF
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
     case (XDC_CI) : XDG_EncodeDG2(vl_dg,XDG_EPRV_AUTO,XDC_NOM_SITE_CI);
                     break;
     case (XDC_VC) : XDG_EncodeDG2(vl_dg,XDG_EPRV_AUTO,XDC_NOM_SITE_VC);
                     break;
     case (XDC_DP) : XDG_EncodeDG2(vl_dg,XDG_EPRV_AUTO,XDC_NOM_SITE_DP);
                     break;
     case (XDC_CA) : XDG_EncodeDG2(vl_dg,XDG_EPRV_AUTO,XDC_NOM_SITE_CA);
                     break;
    }

    if (TipcSrvMsgWrite(vl_dg,XDM_IdentMsg(XDM_EPRV_AUTO) ,
                                XDM_FLG_SRVMSGWRITE,
                        T_IPC_FT_INT4, va_NumEvt_in,
                        T_IPC_FT_INT2, va_CleEvt_in,
                        T_IPC_FT_INT2, va_Operateur_in,
                        T_IPC_FT_INT2, va_NumEqt_in,
                        T_IPC_FT_REAL8, va_HeureLancement_in,
                        T_IPC_FT_INT2, va_Site_in,
                        T_IPC_FT_STR, va_Bandeau_in,
                        T_IPC_FT_STR, va_TextePicto_in,
                        T_IPC_FT_INT2, va_Priorite_in,
                        T_IPC_FT_INT4, va_DistanceEvt_in,
                        T_IPC_FT_INT2, va_site_origine_in,
                                NULL)) {
        XZST_03EcritureTrace(XZSTC_FONCTION,
                "XZAC803 a reussi a envoyer le msg XDM_EPRV_AUTO sur %s",vl_dg );
        return (XDC_OK);
      }
      else
        XZST_03EcritureTrace(XZSTC_WARNING ,
                        "XZAC803 : TipcSrvMsgWrite XDM_EPRV_AUTO a echoue");
  return (XDC_NOK);

}




