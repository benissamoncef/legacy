/*E*/
/*X  Fichier : $Id: xzac182.c,v 1.6 2017/03/09 17:51:11 pc2dpdy Exp $      Release : $Revision: 1.6 $        Date : $Date: 2017/03/09 17:51:11 $
------------------------------------------------------
*  *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAC * FICHIER xzac182.c
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
* VR	08/06/12	:	Ajout TDP PMVA (DEM/1014 PMA)
* PNI	27/08/2012	: correction type des destinations et ajout flash DEM1074 v1.5
* JMG   09/03/17        : REGIONALISATION - DEM1220 1.6
* LCL	27/09/23	: Adaptations CNA DEM-483
------------------------------------------------------*/

/* fichiers inclus */

#include "xzac.h"
#include "xzac182.h"



/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Appelle XZAC182SP via tacli
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAC182_Commande_PMVA_TACLI ( XDY_Entier    va_NumEvt_in,
                                 XDY_Octet     va_CleEvt_in,
                                 XDY_Operateur va_Operateur_in,
                                 XDY_Eqt       va_NumEqt_in,
                                 XDY_Horodate  va_HeureLancement_in,
                                 XDY_Octet      va_Site_in,
                                 char      	va_Ligne1_in[20],
                                 char      	va_Ligne2_in[20],
                                 char      	va_Ligne3_in[20],
                                 char      	va_Ligne4_in[20],
                                 char      	va_Ligne5_in[20],
                                 char      	va_Alternat1_in[20],
                                 char      	va_Alternat2_in[20],
                                 char      	va_Alternat3_in[20],
                                 char      	va_Alternat4_in[20],
                                 char      	va_Alternat5_in[20],
                                 XDY_Octet     	va_Clignotement1_in,
                                 XDY_Octet     	va_Clignotement2_in,
                                 XDY_Octet     	va_Clignotement3_in,
                                 XDY_Octet     	va_Clignotement4_in,
                                 XDY_Octet     	va_Clignotement5_in,
                                 XDY_Octet     	va_Flash_in,
                                 XDY_Octet     	va_Priorite_in,
                                 XDY_Entier    	va_DistanceEvt_in,
                                 XDY_Octet      va_site_origine_in,
                				 XDY_Mot		va_Dest_un_proche_in,
                				 XDY_Mot		va_Dest_un_loin_in,
                				 XDY_Mot		va_Dest_deux_proche_in,
                				 XDY_Mot		va_Dest_deux_loin_in)

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
     case (XDC_CI) : XDG_EncodeDG2(vl_dg,XDG_EPMVA_AUTO,XDC_NOM_SITE_CI);
                     break;
     case (XDC_VC) : XDG_EncodeDG2(vl_dg,XDG_EPMVA_AUTO,XDC_NOM_SITE_VC);
                     break;
     case (XDC_DP) : XDG_EncodeDG2(vl_dg,XDG_EPMVA_AUTO,XDC_NOM_SITE_DP);
                     break;
     case (XDC_CA) : XDG_EncodeDG2(vl_dg,XDG_EPMVA_AUTO,XDC_NOM_SITE_CA);
                     break;
    }
    if (TipcSrvMsgWrite(vl_dg,XDM_IdentMsg(XDM_EPMVA_AUTO) ,
                                XDM_FLG_SRVMSGWRITE,
                        T_IPC_FT_INT4, va_NumEvt_in,
                        T_IPC_FT_INT2, va_CleEvt_in,
                        T_IPC_FT_INT2, va_Operateur_in,
                        T_IPC_FT_INT2, va_NumEqt_in,
                        T_IPC_FT_REAL8, va_HeureLancement_in,
                        T_IPC_FT_INT2, va_Site_in,
                        T_IPC_FT_STR, va_Ligne1_in,
                        T_IPC_FT_STR, va_Ligne2_in,
                        T_IPC_FT_STR, va_Ligne3_in,
                        T_IPC_FT_STR, va_Ligne4_in,
                        T_IPC_FT_STR, va_Ligne5_in,
                        T_IPC_FT_STR, va_Alternat1_in,
                        T_IPC_FT_STR, va_Alternat2_in,
                        T_IPC_FT_STR, va_Alternat3_in,
                        T_IPC_FT_STR, va_Alternat4_in,
                        T_IPC_FT_STR, va_Alternat5_in,
                        T_IPC_FT_INT2, va_Clignotement1_in,
                        T_IPC_FT_INT2, va_Clignotement2_in,
                        T_IPC_FT_INT2, va_Clignotement3_in,
                        T_IPC_FT_INT2, va_Clignotement4_in,
                        T_IPC_FT_INT2, va_Clignotement5_in,
                        T_IPC_FT_INT2, va_Flash_in,
                        T_IPC_FT_INT2, va_Priorite_in,
                        T_IPC_FT_INT4, va_DistanceEvt_in,
                        T_IPC_FT_INT2, 0,
                        T_IPC_FT_INT2, va_site_origine_in,
                        T_IPC_FT_INT2, va_Dest_un_proche_in,
                        T_IPC_FT_INT2, va_Dest_un_loin_in,
                        T_IPC_FT_INT2, va_Dest_deux_proche_in,
                        T_IPC_FT_INT2, va_Dest_deux_loin_in,
                                NULL)) {
        XZST_03EcritureTrace(XZSTC_FONCTION,
                "XZAC182 a reussi a envoyer le msg XDM_EPMVA_AUTO sur %s",vl_dg );
        return (XDC_OK);
      }
      else
        XZST_03EcritureTrace(XZSTC_WARNING ,
                        "XZAC182 : TipcSrvMsgWrite XDM_EPMVA_AUTO a echoue");

    return (XDC_NOK);
}



