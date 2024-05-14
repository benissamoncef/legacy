/*E*/
/*X  Fichier : $Id: xzac193.c,v 1.4 2017/03/09 17:53:10 pc2dpdy Exp $      Release : $Revision: 1.4 $        Date : $Date: 2017/03/09 17:53:10 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAC * FICHIER xzac193.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   < Module Interface Actions >
*
------------------------------------------------------
* HISTORIQUE :
*
*
* JMG	23/08/06	creation
* LCL   09/07/07        suppression DY
* JMG   29/09/09        SECTO : DEM 887
* JMG   09/03/17        : REGIONALISATION - DEM1220 1.4
* LCL	  27/09/23	: Adaptations CNA DEM-483
------------------------------------------------------*/

/* fichiers inclus */

#include "xzac193.h"


/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle XZAC193SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAC193_Commande_BAD_TACLI ( XDY_Entier    va_NumEvt_in,
                                 XDY_Octet     va_CleEvt_in,
                                 XDY_Operateur va_Operateur_in,
                                 XDY_Eqt       va_NumEqt_in,
                                 XDY_Horodate  va_HeureLancement_in,
                                 XDY_Booleen   va_Manuelle_in,
                                 XDY_Octet     va_Priorite_in,
                                 XDY_Entier    va_DistanceEvt_in,
                                 char      va_Scenario_in[20],
                                 XDY_Octet      va_Site_in,
                                 XDY_Octet      va_site_origine_in)

/*X*/
/*-----------------------------------------------------
* Service rendu
* commande de BAD
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
     case (XDC_CI) : XDG_EncodeDG2(vl_dg,XDG_EBAD_AUTO,XDC_NOM_SITE_CI);
                     break;
     case (XDC_VC) : XDG_EncodeDG2(vl_dg,XDG_EBAD_AUTO,XDC_NOM_SITE_VC);
                     break;
     case (XDC_DP) : XDG_EncodeDG2(vl_dg,XDG_EBAD_AUTO,XDC_NOM_SITE_DP);
                     break;
     case (XDC_CA) : XDG_EncodeDG2(vl_dg,XDG_EBAD_AUTO,XDC_NOM_SITE_CA);
                     break;
    }


    if (TipcSrvMsgWrite(vl_dg,XDM_IdentMsg(XDM_EBAD_AUTO) ,
                                XDM_FLG_SRVMSGWRITE,
                        va_NumEvt_in,
                        va_CleEvt_in,
                        va_Operateur_in,
                        va_NumEqt_in,
                        va_HeureLancement_in,
                        va_Manuelle_in,
                        va_Priorite_in,
                        va_DistanceEvt_in,
                        va_Scenario_in,
                        va_Site_in,
                        va_site_origine_in,
                                NULL)) {
        XZST_03EcritureTrace(XZSTC_FONCTION,
                "XZAC193 a reussi a envoyer le msg XDM_EBAD_AUTO sur %s",vl_dg );
        return (XDC_OK);
      }
      else
        XZST_03EcritureTrace(XZSTC_WARNING ,
                        "XZAC193 : TipcSrvMsgWrite XDM_EBAD_AUTO a echoue");
      return (XDC_NOK);


}




