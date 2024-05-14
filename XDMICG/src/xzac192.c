/*E*/
/*X  Fichier : $Id: xzac192.c,v 1.4 2017/03/09 17:52:15 pc2dpdy Exp $      Release : $Revision: 1.4 $        Date : $Date: 2017/03/09 17:52:15 $
------------------------------------------------------
*  *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAC * FICHIER xzac192.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   < Module Interface Actions >
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	23/08/06	creation
* LCL   09/07/07        suppression DY
* JMG   29/09/09        SECTO : DEM 887
* JMG   09/03/17        : REGIONALISATION - DEM1220 1.4
* LCL	27/09/23	: Adaptations CNA DEM-483
------------------------------------------------------*/

/* fichiers inclus */

#include "xzac.h"
#include "xzac192.h"



/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle XZAC192SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAC192_Commande_BAF_TACLI ( XDY_Entier    va_NumEvt_in,
                                 XDY_Octet     va_CleEvt_in,
                                 XDY_Operateur va_Operateur_in,
                                 XDY_Eqt       va_NumEqt_in,
                                 XDY_Horodate  va_HeureLancement_in,
                                 XDY_Booleen   va_Manuelle_in,
                                 XDY_Octet     va_Priorite_in,
                                 XDY_Entier    va_DistanceEvt_in,
				 XDY_Octet	va_Module11_in,
				 XDY_Octet	va_Module12_in,
				 XDY_Octet	va_Module15_in,
				 XDY_Octet	va_Module21_in,
				 XDY_Octet	va_Module22_in,
				 XDY_Octet	va_Module25_in,
                                 XDY_Octet      va_Site_in,
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
T_INT2		tl_modules[6];

    switch (va_Site_in) {
     case (XDC_CI) : XDG_EncodeDG2(vl_dg,XDG_EBAF_AUTO,XDC_NOM_SITE_CI);
                     break;
     case (XDC_VC) : XDG_EncodeDG2(vl_dg,XDG_EBAF_AUTO,XDC_NOM_SITE_VC);
                     break;
     case (XDC_DP) : XDG_EncodeDG2(vl_dg,XDG_EBAF_AUTO,XDC_NOM_SITE_DP);
                     break;
     case (XDC_CA) : XDG_EncodeDG2(vl_dg,XDG_EBAF_AUTO,XDC_NOM_SITE_CA);
                     break;
    }

    tl_modules[0] = (T_INT2)va_Module11_in;
    tl_modules[1] = (T_INT2)va_Module12_in;
    tl_modules[2] = (T_INT2)va_Module15_in;
    tl_modules[3] = (T_INT2)va_Module21_in;
    tl_modules[4] = (T_INT2)va_Module22_in;
    tl_modules[5] = (T_INT2)va_Module25_in;

    if (TipcSrvMsgWrite(vl_dg,XDM_IdentMsg(XDM_EBAF_AUTO) ,
                                XDM_FLG_SRVMSGWRITE,
                        T_IPC_FT_INT4, &va_NumEvt_in,
                        T_IPC_FT_INT2, &va_CleEvt_in,
                        T_IPC_FT_INT2, &va_Operateur_in,
                        T_IPC_FT_INT2, &va_NumEqt_in,
                        T_IPC_FT_REAL8, &va_HeureLancement_in,
                        T_IPC_FT_INT2, &va_Manuelle_in,
                        T_IPC_FT_INT2, &va_Priorite_in,
                        T_IPC_FT_INT4, &va_DistanceEvt_in,
                        T_IPC_FT_INT2_ARRAY, tl_modules,6,
                        T_IPC_FT_INT2, &va_Site_in,
                        T_IPC_FT_INT2, &va_site_origine_in,
                                NULL)) {
        XZST_03EcritureTrace(XZSTC_FONCTION,
                "XZAC192 a reussi a envoyer le msg XDM_EBAF_AUTO sur %s",vl_dg );
        return (XDC_OK);
      }
      else
        XZST_03EcritureTrace(XZSTC_WARNING ,
                        "XZAC192 : TipcSrvMsgWrite XDM_EBAF_AUTO a echoue");

    return (XDC_NOK);

}
