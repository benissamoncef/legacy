/*E*/
/*X  Fichier : @(#)xzac101.c	1.4      Release : 1.4        Date : 10/05/09
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
* JMG	23/08/06	creation
* LCL   09/07/07        Supression DY 
* JMG   29/09/09        SECTO : DEM 887
* VR	04/11/11		Ajout destination noeud (DEM 1014)
* PNI	26/08/13	correction T_IPC_FT_INT4, &vl_DestinationNoeud DEM1074
* JMG   09/03/17        : REGIONALISATION - DEM1220 1.7
* LCL	27/09/23	: Adaptations CNA DEM-483
------------------------------------------------------*/

/* fichiers inclus */

#include "xzac101.h"


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

int XZAC101_Commande_PMV_TACLI ( XDY_Entier    va_NumEvt_in,
                                 XDY_Octet     va_CleEvt_in,
                                 XDY_Operateur va_Operateur_in,
                                 XDY_Eqt       va_NumEqt_in,
                                 XDY_Horodate  va_HeureLancement_in,
                                 XDY_Octet      va_Site_in,
                                 char      	va_Ligne1_in[20],
                                 char      	va_Ligne2_in[20],
                                 char      	va_Ligne3_in[20],
                                 char      	va_Alternat1_in[20],
                                 char      	va_Alternat2_in[20],
                                 char      	va_Alternat3_in[20],
                                 XDY_Octet     	va_Clignotement1_in,
                                 XDY_Octet     	va_Clignotement2_in,
                                 XDY_Octet     	va_Clignotement3_in,
                                 XDY_Octet     	va_Flash_in,
                                 char      	va_TypePicto_in[20],
                                 char      	va_TextePicto_in[20],
                                 XDY_Octet     	va_ClignotementPicto_in,
                                 XDY_Octet     va_Priorite_in,
                                 XDY_Entier    va_DistanceEvt_in,
                                 char      	va_LigneDir_in[20],
                                 char      	va_AlternatDir_in[20],
				 XDY_Mot	va_Destination_in,
				 XDY_Mot	va_DestinationAlternat_in,
				 XDY_Octet	va_DepassSeuil_in,
                                 XDY_Octet      va_site_origine_in,
								 XDY_Mot	va_DestinationNoeud_in)

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
	XZST_03EcritureTrace(XZSTC_WARNING,"IN : xzac101 : evt(%d/%d), poste(%d), pmv (%d), site (%d), site pmv(%d) c1(%d) c2(%d) c3(%d) flash(%d)",
                 va_NumEqt_in,	va_NumEvt_in,
                 va_CleEvt_in,
                  va_Operateur_in, va_NumEqt_in,va_Site_in,va_site_origine_in,va_Clignotement1_in,va_Clignotement2_in,va_Clignotement3_in, va_Flash_in);

    switch (va_Site_in) {
     case (XDC_CI) : XDG_EncodeDG2(vl_dg,XDG_EPMV_AUTO,XDC_NOM_SITE_CI);
                     break;
     case (XDC_VC) : XDG_EncodeDG2(vl_dg,XDG_EPMV_AUTO,XDC_NOM_SITE_VC);
                     break;
     case (XDC_DP) : XDG_EncodeDG2(vl_dg,XDG_EPMV_AUTO,XDC_NOM_SITE_DP);
                     break;
     case (XDC_CA) : XDG_EncodeDG2(vl_dg,XDG_EPMV_AUTO,XDC_NOM_SITE_CA);
                     break;
    }

    if (TipcSrvMsgWrite(vl_dg,XDM_IdentMsg(XDM_EPMV_AUTO) ,
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
                        T_IPC_FT_STR, va_Alternat1_in,
                        T_IPC_FT_STR, va_Alternat2_in,
                        T_IPC_FT_STR, va_Alternat3_in,
                        T_IPC_FT_INT2, va_Clignotement1_in,
                        T_IPC_FT_INT2, va_Clignotement2_in,
                        T_IPC_FT_INT2, va_Clignotement3_in,
                        T_IPC_FT_INT2, va_Flash_in,
                        T_IPC_FT_STR, va_TypePicto_in,
                        T_IPC_FT_STR, va_TextePicto_in,
                        T_IPC_FT_INT2, va_ClignotementPicto_in,
                        T_IPC_FT_INT2, va_Priorite_in,
                        T_IPC_FT_INT4, va_DistanceEvt_in,
                        T_IPC_FT_STR, va_LigneDir_in,
                        T_IPC_FT_STR, va_AlternatDir_in,
                        T_IPC_FT_INT2, va_DepassSeuil_in,
                        T_IPC_FT_INT4, va_Destination_in,
                        T_IPC_FT_INT4, va_DestinationAlternat_in,
                        T_IPC_FT_INT2, va_site_origine_in,
			T_IPC_FT_INT4,	va_DestinationNoeud_in,
                                NULL)) {
        XZST_03EcritureTrace(XZSTC_FONCTION,
                "XZAC101 a reussi a envoyer le msg XDM_EPMV_AUTO sur %s",vl_dg );
        return (XDC_OK);
      }
      else
        XZST_03EcritureTrace(XZSTC_WARNING ,
                        "XZAC101 : TipcSrvMsgWrite XDM_EPMV_AUTO a echoue");
  return (XDC_NOK);

}




