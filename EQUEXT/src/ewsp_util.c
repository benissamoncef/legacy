/*E*/
/*Fichier :  $Id: ewsp_util.c,v 1.6 2022/03/30 15:09:01 pc2dpdy Exp $      Release : $Revision: 1.0 $        Date : $Date: 2022/03/30 15:09:01 $ ------------------------------------------------------
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEREG * FICHIER ewsp_util.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module fonctions utilistaires tewsp >
*
------------------------------------------------------
* HISTORIQUE :
*
* LCL		30/03/22 creation 1.0
------------------------------------------------------*/

/* fichiers inclus */
#include "xzst.h"
#include "xzsm.h"
#include "xzit.h"
#include "xzia.h"
#include "xzdd.h"
#include "xzaac.h"
#include "xzaec.h"

#define C_DATE_NULLE -1


int genererReveil(int va_numero, int va_cle, char *va_texte, int va_site_reveil)
{
    XZITT_Destinataire vl_destinataire;
    XDY_Evt vl_fmc;
    int vl_indice;
    int vl_trouve;

    XZST_03EcritureTrace(XZSTC_FONCTION, "IN : genererReveil %d/%d : %s sur site %d",
                         va_numero, va_cle, va_texte, va_site_reveil);

    vl_fmc.numero = va_numero;
    vl_fmc.cle = va_cle;
    if (va_site_reveil == XDC_DP)
        strcpy(vl_destinataire.site, XDC_NOM_SITE_DP);
    else if (va_site_reveil == XDC_VC)
        strcpy(vl_destinataire.site, XDC_NOM_SITE_VC);
    else
        strcpy(vl_destinataire.site, XDC_NOM_SITE_CI);


    /* Fix reveil Fin FMC au CI */
    if (!strcmp(va_texte, XDC_REV_FMC_FIN))
    {
        strcpy(vl_destinataire.site, XDC_NOM_SITE_CI);
        XZIT08_Modif_Liste_FMC(vl_fmc, vl_destinataire, XDC_REV_FMC_MOD);
    }
    else
    {
        XZIT08_Modif_Liste_FMC(vl_fmc, vl_destinataire, va_texte);
    }
    return (XDC_OK);
}




