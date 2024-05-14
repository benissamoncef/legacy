/*E*/
/*Fichier : $Id: cgcd_mSGBDeq.c,v 1.33 2021/05/10 08:52:03 pc2dpdy Exp $      Release : $Revision: 1.33 $        Date : $Date: 2021/05/10 08:52:03 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  GECONF
------------------------------------------------------
* MODULE MVF * FICHIER cgcd_mSGBDeq.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*    Fichier appartenant au module MVF contenant les fonctions
*  modificationSGBDequipements.
*
******************
*    cmvf_FctUti_MSE_XZAO98_67 : Fct utilisateur passee a xzao95_Lire_Config_Machine,
*  qui appelle XZAO67_Ajouter_Station_RADT avec le nom de la base sur la quelle XZAO67_Ajouter_Station_RADT va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
******************
*    cmvf_FctUti_MSE_XZAO103_72 : Fct utilisateur passee a xzao103_Lire_Config_PMV_Picto,
*  qui appelle XZAO72_Ajouter_PMVPicto avec le nom de la base sur la quelle XZAO72_Ajouter_PMVPicto va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
******************
*    cmvf_FctUti_MSE_XZAO105_80 : Fct utilisateur passee a xzao105_Lire_Config_DAI,
*  qui appelle XZAO80_Ajouter_DAI avec le nom de la base sur la quelle XZAO80_Ajouter_DAI va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
******************
*    cmvf_FctUti_MSE_XZAO107_137: Fct utilisateur passee a xzao107_Lire_Config_SystemeVideo,
*  qui appelle XZAO137_Ajouter_SystemeVideo avec le nom de la base sur la quelle XZAO137_Ajouter_SystemeVideo va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
******************
*    cmvf_FctUti_MSE_XZAO112_142 : Fct utilisateur passee a XZAO112,
*  qui appelle XZAO142_Ajouter_MAA avec le nom de la base sur la quelle XZAO142_Ajouter_MAA va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
******************
*    cmvf_FctUti_MSE_XZAO113_143 : Fct utilisateur passee a xzao113_Lire_Config_PIRAU,
*  qui appelle XZAO143_Ajouter_PIRAU avec le nom de la base sur la quelle XZAO143_Ajouter_PIRAU va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
******************
*    cmvf_FctUti_MSE_XZAO156_157 : Fct utilisateur passee a XZAO156,
*  qui appelle XZAO166_Ajouter_GTC_NICE avec le nom de la base sur la quelle XZAO166_Ajouter_GTC_NICE va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
******************
*    cmvf_FctUti_MSE_XZAO165_166 : Fct utilisateur passee a XZAO165,
*  qui appelle XZAO166_Ajouter_GTC_NICE avec le nom de la base sur la quelle XZAO166_Ajouter_GTC_NICE va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
******************
*    cmvf_FctUti_MSE_XZAO167_168 : Fct utilisateur passee a XZAO167,
*  qui appelle XZAO168_Ajouter_Sequence_Tube avec le nom de la base sur la quelle XZAO168_Ajouter_Sequence_Tube va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
******************
*    cmvf_FctUti_MSE_XZAO173_174 : Fct utilisateur passee a xzao173_Lire_Config_Sequences_Ech,
*  qui appelle XZAO174_Ajouter_Sequence_Ech avec le nom de la base sur la quelle XZAO174_Ajouter_Sequence_Ech
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
******************
*    cmvf_FctUti_MSE_XZAO99_68 : Fct utilisateur passee a xzao99_Lire_Config_Chaussee,
*  qui appelle XZAO68_Ajouter_Chaussee avec le nom de la base sur la quelle XZAO68_Ajouter_Chaussee 
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
******************
*    cmvf_FctUti_MSE_XZAO106_81 : Fct utilisateur passee a xzao106_Lire_Config_Analyseur,
*  qui appelle XZAO81_Ajouter_Analyseur avec le nom de la base sur la quelle XZAO81_Ajouter_Analyseur 
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
******************
*    cmvf_FctUti_MSE_XZAO108_138 : Fct utilisateur passee a xzao108_Lire_Config_LTVideo,
*  qui appelle XZAO138_Ajouter_LTVideo avec le nom de la base sur la quelle XZAO138_Ajouter_LTVideo 
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
******************
*    cmvf_FctUti_MSE_XZAO111_141 : Fct utilisateur passee a xzao111_Lire_Config_Magnetoscope,
*  qui appelle XZAO141_Ajouter_Magnetoscope avec le nom de la base sur la quelle XZAO141_Ajouter_Magnetoscope 
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
******************
*    cmvf_FctUti_MSE_XZAO114_144 : Fct utilisateur passee a xzao114_Lire_Config_PAU,
*  qui appelle XZAO144_Ajouter_PAU avec le nom de la base sur la quelle XZAO144_Ajouter_PAU 
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
******************
*    cmvf_FctUti_MSE_XZAO158_159 : Fct utilisateur passee a xzao158_Lire_Config_Scen_Connus,
*  qui appelle XZAO159_Ajouter_Scen_Connu avec le nom de la base sur la quelle XZAO159_Ajouter_Scen_Connu 
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
******************
*    cmvf_FctUti_MSE_XZAO175_176 : Fct utilisateur passee a xzao175_Lire_Config_Demi_Ech,
*  qui appelle XZAO176_Ajouter_Demi_Ech avec le nom de la base sur la quelle XZAO176_Ajouter_Demi_Ech 
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
******************
*    cmvf_FctUti_MSE_XZAO177_178 : Fct utilisateur passee a xzao177_Lire_Config_Tube,
*  qui appelle XZAO178_Ajouter_Tube avec le nom de la base sur la quelle XZAO178_Ajouter_Tube 
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
******************
*    cmvf_FctUti_MSE_XZAO100_69 : Fct utilisateur passee a xzao100_Lire_Config_Pt_Mesure,
*  qui appelle XZAO69_Ajouter_Pt_Mesure avec le nom de la base sur la quelle XZAO69_Ajouter_Pt_Mesure 
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
******************
*    cmvf_FctUti_MSE_XZAO109_139 : Fct utilisateur passee a xzao109_Lire_Config_Camera,
*  qui appelle XZAO139_Ajouter_Camera avec le nom de la base sur la quelle XZAO139_Ajouter_Camera 
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
******************
*    cmvf_FctUti_MSE_XZAO192_193 : Fct utilisateur passee a xzao192_Lire_Config_Zon_Camera,
*  qui appelle XZAO193_Ajouter_Zon_Camera avec le nom de la base sur la quelle XZAO193_Ajouter_Zon_Camera 
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
******************
*    cmvf_FctUti_MSE_XZAO160_161 : Fct utilisateur passee a xzao160_Lire_Config_NAV,
*  qui appelle XZAO161_Ajouter_NAV avec le nom de la base sur la quelle XZAO161_Ajouter_NAV 
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
******************
*    cmvf_FctUti_MSE_XZAO169_170 : Fct utilisateur passee a xzao169_Lire_Config_SeqAut_Tunnel,
*  qui appelle XZAO170_Ajouter_SeqAut_Tunnel avec le nom de la base sur la quelle XZAO170_Ajouter_SeqAut_Tunnel 
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
******************
*    cmvf_FctUti_MSE_XZAO171_172 : Fct utilisateur passee a xzao171_Lire_Config_SeqAut_Echangeur,
*  qui appelle XZAO172_Ajouter_SeqAut_Echangeur avec le nom de la base sur la quelle XZAO172_Ajouter_SeqAut_Echangeur 
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
******************
*    cmvf_FctUti_MSE_XZAO179_180 : Fct utilisateur passee a xzao179_Lire_Config_Capteurs_Tunnel,
*  qui appelle XZAO180_Ajouter_Capteur_Tunnel avec le nom de la base sur la quelle XZAO180_Ajouter_Capteur_Tunnel 
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
******************
*    cmvf_FctUti_MSE_XZAO181_182 : Fct utilisateur passee a xzao181_Lire_Config_FAV,
*  qui appelle XZAO182_Ajouter_FAV avec le nom de la base sur la quelle XZAO182_Ajouter_FAV 
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
******************
*    cmvf_FctUti_MSE_XZAO183_184 : Fct utilisateur passee a xzao183_Lire_Config_PnGTC,
*  qui appelle XZAO184_Ajouter_PnGTC avec le nom de la base sur la quelle XZAO184_Ajouter_PnGTC 
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
******************
*    cmvf_FctUti_MSE_XZAO191_190 : Fct utilisateur passee a xzao191_Lire_Config_Fax,
*  qui appelle XZAO184_Ajouter_PnGTC avec le nom de la base sur la quelle XZAO190_Ajouter_Fax 
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
******************
*    cmvf_FctUti_MSE_XZAO197_187 : Fct utilisateur passee a xzao197_Lire_Config_Eqt_Generique_LCR,
*  qui appelle XZAO187_Ajouter_Eqt_generique_LCR avec le nom de la base sur la quelle XZAO187_Ajouter_Eqt_generique_LCR 
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
******************
*    cmvf_FctUti_MSE_XZAO403_404 : Fct utilisateur passee a xzao404_Lire_Config_Eqt_ZDP
*  qui appelle XZAO403_Ajouter_Eqt_ZDP avec le nom de la base sur la quelle XZAO403_Ajouter_Eqt_ZDP 
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
******************
*    cmvf_FctUti_MSE_XZAO407_408 : Fct utilisateur passee a xzao408_Lire_Config_Eqt_TDP
*  qui appelle XZAO407_Ajouter_Eqt_TDP avec le nom de la base sur la quelle XZAO407_Ajouter_Eqt_TDP 
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
******************
*    cmvf_FctUti_MSE_XZAO417_416 : Fct utilisateur passee a xzao417_Lire_Config_Zon_PMV
* qui appelle XZAO416_Ajouter_Zon_PMV avec le nom de la base sur la quelle XZAO416_Ajouter_Zon_PMV
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
******************
*    cmvf_FctUti_MSE_XZAO421_420 : Fct utilisateur passee a xzao421_Lire_Config_PAL
* qui appelle XZAO420_Ajouter_PAL avec le nom de la base sur laquelle XZAO421_Ajouter_PAL
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
******************
* AAZ 26/09/2007 
******************
*    cmvf_FctUti_MSE_XZAO640_600 : Fct utilisateur passee a xzao640_Lire_Config_SIG,
*  qui appelle XZAO600_Ajouter_SIG avec le nom de la base sur la quelle XZAO600_Ajouter_SIG 
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
******************
*    cmvf_FctUti_MSE_XZAO641_601 : Fct utilisateur passee a xzao641_Lire_Config_ECL,
*  qui appelle XZAO601_Ajouter_ECL avec le nom de la base sur la quelle XZAO601_Ajouter_ECL 
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
******************
*    cmvf_FctUti_MSE_XZAO642_602 : Fct utilisateur passee a xzao642_Lire_Config_VEN,
*  qui appelle XZAO602_Ajouter_ECL avec le nom de la base sur la quelle XZAO602_Ajouter_ECL 
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
******************
*    cmvf_FctUti_MSE_XZAO643_603 : Fct utilisateur passee a xzao643_Lire_Config_ENR,
*  qui appelle XZAO603_Ajouter_ENR avec le nom de la base sur la quelle XZAO603_Ajouter_ENR 
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
******************
*    cmvf_FctUti_MSE_XZAO644_604 : Fct utilisateur passee a xzao644_Lire_Config_INC,
*  qui appelle XZAO604_Ajouter_INC avec le nom de la base sur la quelle XZAO604_Ajouter_INC 
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
******************
*    cmvf_FctUti_MSE_XZAO645_605 : Fct utilisateur passee a xzao645_Lire_Config_EXP,
*  qui appelle XZAO605_Ajouter_EXP avec le nom de la base sur la quelle XZAO605_Ajouter_EXP 
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
******************
*    cmvf_FctUti_MSE_XZAO646_606 : Fct utilisateur passee a xzao646_Lire_Config_SYS,
*  qui appelle XZAO606_Ajouter_SYS avec le nom de la base sur la quelle XZAO606_Ajouter_SYS 
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
******************
*    cmvf_FctUti_MSE_XZAO647_607 : Fct utilisateur passee a xzao647_Lire_Config_LTN,
*  qui appelle XZAO607_Ajouter_LTN avec le nom de la base sur la quelle XZAO607_Ajouter_LTN 
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
******************
*    cmvf_FctUti_MSE_XZAO257_PRV: Fct utilisateur passee a xzao255_Lire_Config_PRV,
*  qui appelle XZAO257_Ajouter_PRV avec le nom de la base sur la quelle XZAO257_Ajouter_PRV va avoir une action.
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
******************
*    cmvf_FctUti_MSE_XZAO2571_PIC: Fct utilisateur passee a xzao2571_Lire_Config_PIC,
*  qui appelle XZAO2571_Ajouter_PIC avec le nom de la base sur la quelle XZAO2571_Ajouter_PIC va avoir une action.
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
******************

* AAZ 
******************
------------------------------------------------------
* HISTORIQUE :
*
* mercier    19 Oct 1994    : Creation                V 1.1
**************
* volcic    14 Nov 1994    : Modif XZAOT_ConfNAV => XZAOT_AjoutNAV    V 1.3
**************
* MERCIER    21 Dec 1994    : Modification de l appel         V 1.4
*  de xzia_08
**************
* TORREGROSSA    01 Dec 1995    : Ajout de la fonction de lecture des fax V 1.5
* NIEPCERON    07 Jun 1996    : Suppression de l'affichage des messages de trait. des lignes v1.6 (968)
* MISMER    06 Nov 1996    : Ajout de la fonction de lecture des equipements generique LCR v1.9 (DEM/1232)
* MISMER    10 Fev 1997    : Ajout des fonction temps de parcours v1.10 (DEM/1395)
* Guilhou    02 jan 1998     : ajout traitement des zones d'influence PMV 1.11 (dem/1534)
* TORREGROSSA    21 Jan 1998    : Ajout de la fonction de lecture des pal (1545) V 1.12
* Guilhou    20 sep 2000    : modif interface cmvf_FctUti_MSE_XZAO417_416 1.13
* JMG        28/09/05    : ajout PMVA BAF BAD 1.14
* JBL		Nov 2007	: DEM 666
* JMG		25/09/09 : ajout ZON_TUN 1.17
* JPL		07/10/10 : Gestion zones cameras par cmvf_FctUti_MSE_XZAO192_193 (DEM 948)  1.18
* JPL		15/02/11 : Ajout parametre distance a XZAO428_ZON_PMVA; appelle XZAO4290 (DEM 953)  1.19
* VR		15/12/11 : Ajout PRV (DEM/1016)
* VR		11/01/12 : Integration Regulation de vitesse (DEM/1016)
* JMG		26/04/16 : ajout SAGA 1.21 DEM1191
* JPL		19/07/16 : Migration architecture ia64 : compatibilite des types d'arguments  1.23
* LCL		26/06/17 : ajout zones urbaines 1.24 (convergence)
* JPL		10/01/17 : Gestion points de mesure de la DIT par cmvf_FctUti_MSE_XZAO261_262 (DEM 1266)  1.25
* LCL		06/02/18 : Gestion controleurs de feux cmvf_FctUti_MSE_XZAO258_CFE (CTRL_FEUX) 1.26 DEM1284
* LCL		28/05/18 : Gestion capteurs de bouchons cmvf_FctUti_MSE_XZAO265_CAP (GABEB) 1.27 DEM1283
* LCL		22/10/18 : Ajout identifiant LCR DEM1306 1.28
* JPL		24/01/19 : Gestion des fichiers d'en-t�te inclu(de)s  1.31
* CGR		30/07/20 : Ajout cmvf_FctUti_MSE_XZAO492_IMU 1.32 DEM-SAE155
* CGR		06/10/20 : Ajout cmvf_FctUti_MSE_XZAO495_Zon_IMU et cmvf_FctUti_MSE_XZAO498_Msg_IMU DEM-SAE155
* LCL		10/05/21 : Correction type XZAO_ConfMsgIMU 1.34 DEM-SAE155
* ABK		18/08/23 : Ajout du nouveau type Picto (DEM/483)
-----------------------------------------------*/

/* fichiers inclus */

#include "cgcd_mSGBDeq.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: cgcd_mSGBDeq.c,v 1.33 2021/05/10 08:52:03 pc2dpdy Exp $ : cgcd_mSGBDeq" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */
/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*    cmvf_FctUti_MSE_XZAO98_67 : Fct utilisateur passee a xzao95_Lire_Config_Machine,
*  qui appelle XZAO67_Ajouter_Station_RADT avec le nom de la base sur la quelle XZAO67_Ajouter_Station_RADT va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cmvf_FctUti_MSE_XZAO98_67(XZAOT_ConfRADT          va_Station_in, XDY_Entier     va_Resultat_out) 
/*
* ARGUMENTS EN ENTREE :  va_Station_in contient la station RADT a rajouter
*            va_Resultat_out contient le numero de ligne du fichier qui est en cours 
*            de traitement.
*                 
* ARGUMENTS EN SORTIE :  aucun
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
*
* CONDITION D'UTILISATION
*
* FONCTION 
*    Doit appeler XZAO67_Ajouter_Station_RADT
*     
------------------------------------------------------*/
{
    static char *version     = "$Id: cgcd_mSGBDeq.c,v 1.33 2021/05/10 08:52:03 pc2dpdy Exp $ : cmvf_FctUti_MSE_XZAO98_67";
    char    vl_NomBase[4]    ;
    int     vl_retcode     = XDC_OK;
    int     vl_retcode_out     = XDC_OK;
    char      pl_Msg[500];            /* contient le mesg a tracer*/
    char      pl_Msg2[500];            /* contient modification si vl_Resultat_out XDC_AJOUT,...*/
    char      pl_NomProcs[80] ;
    
    XDY_ResConf vl_Resultat_out;
    
    XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MSE_XZAO98_67");
    
    strcpy( vl_NomBase,CGCDG_Base_Courante);    
    if ((vl_retcode =XZAO67_Ajouter_Station_RADT(vl_NomBase,va_Station_in, &vl_Resultat_out))!=XDC_OK)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO98_67 a echoue sur ligne %ld",(long)va_Resultat_out);
        vl_retcode_out = XDC_NOK;    
    }
    else
    /*A
    ** Test sur parametres de sortie de XZAO67_Ajouter_Station_RADT sur coherence base
    ** 4 valeurs de status d execution de la sp dans vl_Resultat_out
    ** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
    */
    {
        if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
        {
            strcpy(pl_NomProcs,"XZAO67_Ajouter_Station_RADT");
            XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld",pl_NomProcs,pl_Msg2,(long)va_Resultat_out);
            
            sprintf(pl_Msg,"%s %s sur la ligne %ld",XDF_CONF_STATION_RADT,pl_Msg2,(long)va_Resultat_out);
            cuti_BilanConf_Trace(pl_Msg);    /* Notification dans Bilan config     */
            
        }
        else
        {
            XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO98_67 : cmvf_Trace_TYPE_Result a echoue");
            vl_retcode_out = XDC_NOK;
        }
    
        vl_retcode = vl_retcode_out;
    }    
    XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MSE_XZAO98_67 : sortie avec %d",vl_retcode_out);
    return(vl_retcode_out);
}

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*    cmvf_FctUti_MSE_XZAO103_72 : Fct utilisateur passee a xzao103_Lire_Config_PMV_Picto,
*  qui appelle XZAO72_Ajouter_PMVPicto avec le nom de la base sur la quelle XZAO72_Ajouter_PMVPicto va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cmvf_FctUti_MSE_XZAO103_72( XZAOT_ConfPMV           va_PMVPicto_in, XDY_Entier     va_Resultat_out) 
/*
* ARGUMENTS EN ENTREE :  va_PMVPicto_in contient le PMV picto a rajouter
*            va_Resultat_out contient le numero de ligne du fichier qui est en cours 
*            de traitement.
*                 
* ARGUMENTS EN SORTIE :  aucun
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
*
* CONDITION D'UTILISATION
*
* FONCTION 
*    Doit appeler XZAO72_Ajouter_PMVPicto
*     
------------------------------------------------------*/
{
    static char *version     = "$Id: cgcd_mSGBDeq.c,v 1.33 2021/05/10 08:52:03 pc2dpdy Exp $ : cmvf_FctUti_MSE_XZAO103_72";
    char    vl_NomBase[4]    ;
    int     vl_retcode     = XDC_OK;
    int     vl_retcode_out     = XDC_OK;
    char      pl_Msg[500];            /* contient le mesg a tracer*/
    char      pl_Msg2[500];            /* contient modification si vl_Resultat_out XDC_AJOUT,...*/
    char      pl_NomProcs[80] ;
    
    XDY_ResConf vl_Resultat_out;
    
    XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MSE_XZAO103_72");
    
    strcpy( vl_NomBase,CGCDG_Base_Courante);    
    if ((vl_retcode =XZAO72_Ajouter_PMVPicto(vl_NomBase,va_PMVPicto_in, &vl_Resultat_out))!=XDC_OK)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO103_72 a echoue sur ligne %ld",(long)va_Resultat_out);
        vl_retcode_out = XDC_NOK;    
    }
    else
    /*A
    ** Test sur parametres de sortie de XZAO72_Ajouter_PMVPicto sur coherence base
    ** 4 valeurs de status d execution de la sp dans vl_Resultat_out
    ** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
    */
    {
        if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
        {
            strcpy(pl_NomProcs,"XZAO72_Ajouter_PMVPicto");
            XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld",pl_NomProcs,pl_Msg2,(long)va_Resultat_out);
            
            sprintf(pl_Msg,"%s %s sur la ligne %ld",XDF_CONF_PMV_PICTO,pl_Msg2,(long)va_Resultat_out);
            cuti_BilanConf_Trace(pl_Msg);    /* Notification dans Bilan config     */
            
        }
        else
        {
            XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO103_72 : cmvf_Trace_TYPE_Result a echoue");
            vl_retcode_out = XDC_NOK;
        }
    
        vl_retcode = vl_retcode_out;
    }    
    XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MSE_XZAO103_72 : sortie avec %d",vl_retcode_out);
    return(vl_retcode_out);
}

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*    cmvf_FctUti_MSE_XZAO104_73 : Fct utilisateur passee a xzao104_Lire_Config_Mini_GTC,
*  qui appelle XZAO73_Ajouter_MiniGTC avec le nom de la base sur la quelle XZAO73_Ajouter_MiniGTC va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cmvf_FctUti_MSE_XZAO104_73(XZAOT_ConfMiniGTC       va_MiniGTC_in, XDY_Entier     va_Resultat_out) 
/*
* ARGUMENTS EN ENTREE :  va_MiniGTC_in contient la miniGTC a rajouter
*            va_Resultat_out contient le numero de ligne du fichier qui est en cours 
*            de traitement.
*                 
* ARGUMENTS EN SORTIE :  aucun
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
*
* CONDITION D'UTILISATION
*
* FONCTION 
*    Doit appeler XZAO73_Ajouter_MiniGTC
*     
------------------------------------------------------*/
{
    static char *version     = "$Id: cgcd_mSGBDeq.c,v 1.33 2021/05/10 08:52:03 pc2dpdy Exp $ : cmvf_FctUti_MSE_XZAO104_73";
    char    vl_NomBase[4]    ;
    int     vl_retcode     = XDC_OK;
    int     vl_retcode_out     = XDC_OK;
    char      pl_Msg[500];            /* contient le mesg a tracer*/
    char      pl_Msg2[500];            /* contient modification si vl_Resultat_out XDC_AJOUT,...*/
    char      pl_NomProcs[80] ;
    
    XDY_ResConf vl_Resultat_out;
    
    XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MSE_XZAO104_73");
    
    strcpy( vl_NomBase,CGCDG_Base_Courante);    
    if ((vl_retcode =XZAO73_Ajouter_MiniGTC(vl_NomBase,va_MiniGTC_in, &vl_Resultat_out))!=XDC_OK)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO104_73 a echoue sur ligne %ld",(long)va_Resultat_out);
        vl_retcode_out = XDC_NOK;    
    }
    else
    /*A
    ** Test sur parametres de sortie de XZAO73_Ajouter_MiniGTC sur coherence base
    ** 4 valeurs de status d execution de la sp dans vl_Resultat_out
    ** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
    */
    {
        if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
        {
            strcpy(pl_NomProcs,"XZAO73_Ajouter_MiniGTC");
            XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld",pl_NomProcs,pl_Msg2,(long)va_Resultat_out);
            
            sprintf(pl_Msg,"%s %s sur la ligne %ld",XDF_CONF_MINI_GTC,pl_Msg2,(long)va_Resultat_out);
            cuti_BilanConf_Trace(pl_Msg);    /* Notification dans Bilan config     */
            
        }
        else
        {
            XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO104_73 : cmvf_Trace_TYPE_Result a echoue");
            vl_retcode_out = XDC_NOK;
        }
    
        vl_retcode = vl_retcode_out;
    }    
    XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MSE_XZAO104_73 : sortie avec %d",vl_retcode_out);
    return(vl_retcode_out);
}
/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*    cmvf_FctUti_MSE_XZAO105_80 : Fct utilisateur passee a xzao105_Lire_Config_DAI,
*  qui appelle XZAO80_Ajouter_DAI avec le nom de la base sur la quelle XZAO80_Ajouter_DAI va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cmvf_FctUti_MSE_XZAO105_80(XZAOT_ConfDAI           va_DAI_in, XDY_Entier     va_Resultat_out) 
/*
* ARGUMENTS EN ENTREE :  va_DAI_in contient la DAI a rajouter
*            va_Resultat_out contient le numero de ligne du fichier qui est en cours 
*            de traitement.
*                 
* ARGUMENTS EN SORTIE :  aucun
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
*
* CONDITION D'UTILISATION
*
* FONCTION 
*    Doit appeler XZAO80_Ajouter_DAI
*     
------------------------------------------------------*/
{
    static char *version     = "$Id: cgcd_mSGBDeq.c,v 1.33 2021/05/10 08:52:03 pc2dpdy Exp $ : cmvf_FctUti_MSE_XZAO105_80";
    char    vl_NomBase[4]    ;
    int     vl_retcode     = XDC_OK;
    int     vl_retcode_out     = XDC_OK;
    char      pl_Msg[500];            /* contient le mesg a tracer*/
    char      pl_Msg2[500];            /* contient modification si vl_Resultat_out XDC_AJOUT,...*/
    char      pl_NomProcs[80] ;
    
    XDY_ResConf vl_Resultat_out;
    
    XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MSE_XZAO105_80");
    
    strcpy( vl_NomBase,CGCDG_Base_Courante);    
    if ((vl_retcode =XZAO80_Ajouter_DAI(vl_NomBase,va_DAI_in, &vl_Resultat_out))!=XDC_OK)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO105_80 a echoue sur ligne %ld",(long)va_Resultat_out);
        vl_retcode_out = XDC_NOK;    
    }
    else
    /*A
    ** Test sur parametres de sortie de XZAO80_Ajouter_DAI sur coherence base
    ** 4 valeurs de status d execution de la sp dans vl_Resultat_out
    ** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
    */
    {
        if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
        {
            strcpy(pl_NomProcs,"XZAO80_Ajouter_DAI");
            XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld",pl_NomProcs,pl_Msg2,(long)va_Resultat_out);
            
            sprintf(pl_Msg,"%s %s sur la ligne %ld",XDF_CONF_DAI,pl_Msg2,(long)va_Resultat_out);
            cuti_BilanConf_Trace(pl_Msg);    /* Notification dans Bilan config     */
            
        }
        else
        {
            XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO105_80 : cmvf_Trace_TYPE_Result a echoue");
            vl_retcode_out = XDC_NOK;
        }
    
        vl_retcode = vl_retcode_out;
    }    
    XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MSE_XZAO105_80 : sortie avec %d",vl_retcode_out);
    return(vl_retcode_out);
}
/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*    cmvf_FctUti_MSE_XZAO107_137: Fct utilisateur passee a xzao107_Lire_Config_SystemeVideo,
*  qui appelle XZAO137_Ajouter_SystemeVideo avec le nom de la base sur la quelle XZAO137_Ajouter_SystemeVideo va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cmvf_FctUti_MSE_XZAO107_137(XZAOT_ConfSyVideo       va_SystemeVideo_in, XDY_Entier     va_Resultat_out) 
/*
* ARGUMENTS EN ENTREE :  va_SystemeVideo_in contient le systeme video a rajouter
*            va_Resultat_out contient le numero de ligne du fichier qui est en cours 
*            de traitement.
*                 
* ARGUMENTS EN SORTIE :  aucun
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
*
* CONDITION D'UTILISATION
*
* FONCTION 
*    Doit appeler XZAO137_Ajouter_SystemeVideo
*     
------------------------------------------------------*/
{
    static char *version     = "$Id: cgcd_mSGBDeq.c,v 1.33 2021/05/10 08:52:03 pc2dpdy Exp $ : cmvf_FctUti_MSE_XZAO107_137";
    char    vl_NomBase[4]    ;
    int     vl_retcode     = XDC_OK;
    int     vl_retcode_out     = XDC_OK;
    char      pl_Msg[500];            /* contient le mesg a tracer*/
    char      pl_Msg2[500];            /* contient modification si vl_Resultat_out XDC_AJOUT,...*/
    char      pl_NomProcs[80] ;
    
    XDY_ResConf vl_Resultat_out;
    
    XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MSE_XZAO107_137");
    
    strcpy( vl_NomBase,CGCDG_Base_Courante);    
    if ((vl_retcode =XZAO137_Ajouter_SystemeVideo(vl_NomBase,va_SystemeVideo_in, &vl_Resultat_out))!=XDC_OK)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO107_137a echoue sur ligne %ld",(long)va_Resultat_out);
        vl_retcode_out = XDC_NOK;    
    }
    else
    /*A
    ** Test sur parametres de sortie de XZAO137_Ajouter_SystemeVideo sur coherence base
    ** 4 valeurs de status d execution de la sp dans vl_Resultat_out
    ** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
    */
    {
        if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
        {
            strcpy(pl_NomProcs,"XZAO137_Ajouter_SystemeVideo");
            XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld",pl_NomProcs,pl_Msg2,(long)va_Resultat_out);
            
            sprintf(pl_Msg,"%s %s sur la ligne %ld",XDF_CONF_SYS_VIDEO,pl_Msg2,(long)va_Resultat_out);
            cuti_BilanConf_Trace(pl_Msg);    /* Notification dans Bilan config     */
            
        }
        else
        {
            XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO107_137: cmvf_Trace_TYPE_Result a echoue");
            vl_retcode_out = XDC_NOK;
        }
    
        vl_retcode = vl_retcode_out;
    }    
    XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MSE_XZAO107_137: sortie avec %d",vl_retcode_out);
    return(vl_retcode_out);
}
/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*    cmvf_FctUti_MSE_XZAO112_142 : Fct utilisateur passee a XZAO112,
*  qui appelle XZAO142_Ajouter_MAA avec le nom de la base sur la quelle XZAO142_Ajouter_MAA va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cmvf_FctUti_MSE_XZAO112_142(XZAOT_ConfMAA           va_MAA_in, XDY_Entier     va_Resultat_out) 
/*
* ARGUMENTS EN ENTREE :  va_MAA_in contient la Machine automatique d appel (MAA) a rajouter
*            va_Resultat_out contient le numero de ligne du fichier qui est en cours 
*            de traitement.
*                 
* ARGUMENTS EN SORTIE :  aucun
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
*
* CONDITION D'UTILISATION
*
* FONCTION 
*    Doit appeler XZAO142_Ajouter_MAA
*     
------------------------------------------------------*/
{
    static char *version     = "$Id: cgcd_mSGBDeq.c,v 1.33 2021/05/10 08:52:03 pc2dpdy Exp $ : cmvf_FctUti_MSE_XZAO112_142";
    char    vl_NomBase[4]    ;
    int     vl_retcode     = XDC_OK;
    int     vl_retcode_out     = XDC_OK;
    char      pl_Msg[500];            /* contient le mesg a tracer*/
    char      pl_Msg2[500];            /* contient modification si vl_Resultat_out XDC_AJOUT,...*/
    char      pl_NomProcs[80] ;
    
    XDY_ResConf vl_Resultat_out;
    
    XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MSE_XZAO112_142");
    
    strcpy( vl_NomBase,CGCDG_Base_Courante);    
    if ((vl_retcode =XZAO142_Ajouter_MAA(vl_NomBase,va_MAA_in, &vl_Resultat_out))!=XDC_OK)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO112_142 a echoue sur ligne %ld",(long)va_Resultat_out);
        vl_retcode_out = XDC_NOK;    
    }
    else
    /*A
    ** Test sur parametres de sortie de XZAO142_Ajouter_MAA sur coherence base
    ** 4 valeurs de status d execution de la sp dans vl_Resultat_out
    ** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
    */
    {
        if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
        {
            strcpy(pl_NomProcs,"XZAO142_Ajouter_MAA");
            XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld",pl_NomProcs,pl_Msg2,(long)va_Resultat_out);
            
            sprintf(pl_Msg,"%s %s sur la ligne %ld",XDF_CONF_MAA,pl_Msg2,(long)va_Resultat_out);
            cuti_BilanConf_Trace(pl_Msg);    /* Notification dans Bilan config     */
            
        }
        else
        {
            XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO112_142 : cmvf_Trace_TYPE_Result a echoue");
            vl_retcode_out = XDC_NOK;
        }
    
        vl_retcode = vl_retcode_out;
    }    
    XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MSE_XZAO112_142 : sortie avec %d",vl_retcode_out);
    return(vl_retcode_out);
}
/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*    cmvf_FctUti_MSE_XZAO113_143 : Fct utilisateur passee a xzao113_Lire_Config_PIRAU,
*  qui appelle XZAO143_Ajouter_PIRAU avec le nom de la base sur la quelle XZAO143_Ajouter_PIRAU va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cmvf_FctUti_MSE_XZAO113_143(XZAOT_ConfPIRAU         va_PIRAU_in, XDY_Entier     va_Resultat_out) 
/*
* ARGUMENTS EN ENTREE :  va_PIRAU_in contient le PIRAU a rajouter
*            va_Resultat_out contient le numero de ligne du fichier qui est en cours 
*            de traitement.
*                 
* ARGUMENTS EN SORTIE :  aucun
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
*
* CONDITION D'UTILISATION
*
* FONCTION 
*    Doit appeler XZAO143_Ajouter_PIRAU
*     
------------------------------------------------------*/
{
    static char *version     = "$Id: cgcd_mSGBDeq.c,v 1.33 2021/05/10 08:52:03 pc2dpdy Exp $ : cmvf_FctUti_MSE_XZAO113_143";
    char    vl_NomBase[4]    ;
    int     vl_retcode     = XDC_OK;
    int     vl_retcode_out     = XDC_OK;
    char      pl_Msg[500];            /* contient le mesg a tracer*/
    char      pl_Msg2[500];            /* contient modification si vl_Resultat_out XDC_AJOUT,...*/
    char      pl_NomProcs[80] ;
    
    XDY_ResConf vl_Resultat_out;
    
    XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MSE_XZAO113_143");
    
    strcpy( vl_NomBase,CGCDG_Base_Courante);    
    if ((vl_retcode =XZAO143_Ajouter_PIRAU(vl_NomBase,va_PIRAU_in, &vl_Resultat_out))!=XDC_OK)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO113_143 a echoue sur ligne %ld",(long)va_Resultat_out);
        vl_retcode_out = XDC_NOK;    
    }
    else
    /*A
    ** Test sur parametres de sortie de XZAO143_Ajouter_PIRAU sur coherence base
    ** 4 valeurs de status d execution de la sp dans vl_Resultat_out
    ** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
    */
    {
        if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
        {
            strcpy(pl_NomProcs,"XZAO143_Ajouter_PIRAU");
            XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld",pl_NomProcs,pl_Msg2,(long)va_Resultat_out);
            
            sprintf(pl_Msg,"%s %s sur la ligne %ld",XDF_CONF_PIRAU,pl_Msg2,(long)va_Resultat_out);
            cuti_BilanConf_Trace(pl_Msg);    /* Notification dans Bilan config     */
            
        }
        else
        {
            XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO113_143 : cmvf_Trace_TYPE_Result a echoue");
            vl_retcode_out = XDC_NOK;
        }
    
        vl_retcode = vl_retcode_out;
    }    
    XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MSE_XZAO113_143 : sortie avec %d",vl_retcode_out);
    return(vl_retcode_out);
}
/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*    cmvf_FctUti_MSE_XZAO156_157 : Fct utilisateur passee a XZAO156,
*  qui appelle XZAO157_Ajouter_Type_NAV avec le nom de la base sur la quelle XZAO157_Ajouter_Type_NAV va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cmvf_FctUti_MSE_XZAO156_157(XZAOT_ConfTypNAV       va_Type_NAV_in, XDY_Entier     va_Resultat_out) 
/*
* ARGUMENTS EN ENTREE :  va_Type_NAV_in contient la type de la NAV a rajouter
*            va_Resultat_out contient le numero de ligne du fichier qui est en cours 
*            de traitement.
*                 
* ARGUMENTS EN SORTIE :  aucun
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
*
* CONDITION D'UTILISATION
*
* FONCTION 
*    Doit appeler XZAO157_Ajouter_Type_NAV
*     
------------------------------------------------------*/
{
    static char *version     = "$Id: cgcd_mSGBDeq.c,v 1.33 2021/05/10 08:52:03 pc2dpdy Exp $ : cmvf_FctUti_MSE_XZAO156_157";
    char    vl_NomBase[4]    ;
    int     vl_retcode     = XDC_OK;
    int     vl_retcode_out     = XDC_OK;
    char      pl_Msg[500];            /* contient le mesg a tracer*/
    char      pl_Msg2[500];            /* contient modification si vl_Resultat_out XDC_AJOUT,...*/
    char      pl_NomProcs[80] ;
    
    XDY_ResConf vl_Resultat_out;
    
    XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MSE_XZAO156_157");
    
    strcpy( vl_NomBase,CGCDG_Base_Courante);    
    if ((vl_retcode =XZAO157_Ajouter_Type_NAV(vl_NomBase,va_Type_NAV_in, &vl_Resultat_out))!=XDC_OK)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO156_157 a echoue sur ligne %ld",(long)va_Resultat_out);
        vl_retcode_out = XDC_NOK;    
    }
    else
    /*A
    ** Test sur parametres de sortie de XZAO157_Ajouter_Type_NAV sur coherence base
    ** 4 valeurs de status d execution de la sp dans vl_Resultat_out
    ** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
    */
    {
        if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
        {
            strcpy(pl_NomProcs,"XZAO157_Ajouter_Type_NAV");
            XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld",pl_NomProcs,pl_Msg2,(long)va_Resultat_out);
            
            sprintf(pl_Msg,"%s %s sur la ligne %ld",XDF_CONF_TYPE_NAV,pl_Msg2,(long)va_Resultat_out);
            cuti_BilanConf_Trace(pl_Msg);    /* Notification dans Bilan config     */
            
        }
        else
        {
            XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO156_157 : cmvf_Trace_TYPE_Result a echoue");
            vl_retcode_out = XDC_NOK;
        }
    
        vl_retcode = vl_retcode_out;
    }    
    XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MSE_XZAO156_157 : sortie avec %d",vl_retcode_out);
    return(vl_retcode_out);
}
/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*    cmvf_FctUti_MSE_XZAO165_166 : Fct utilisateur passee a XZAO165,
*  qui appelle XZAO166_Ajouter_GTC_NICE avec le nom de la base sur la quelle XZAO166_Ajouter_GTC_NICE va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cmvf_FctUti_MSE_XZAO165_166(XZAOT_ConfGTCNICE       va_GTC_NICE_in, XDY_Entier     va_Resultat_out) 
/*
* ARGUMENTS EN ENTREE :  va_GTC_NICE_in contient la GTC de Nice a rajouter
*            va_Resultat_out contient le numero de ligne du fichier qui est en cours 
*            de traitement.
*                 
* ARGUMENTS EN SORTIE :  aucun
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
*
* CONDITION D'UTILISATION
*
* FONCTION 
*    Doit appeler XZAO166_Ajouter_GTC_NICE
*     
------------------------------------------------------*/
{
    static char *version     = "$Id: cgcd_mSGBDeq.c,v 1.33 2021/05/10 08:52:03 pc2dpdy Exp $ : cmvf_FctUti_MSE_XZAO165_166";
    char    vl_NomBase[4]    ;
    int     vl_retcode     = XDC_OK;
    int     vl_retcode_out     = XDC_OK;
    char      pl_Msg[500];            /* contient le mesg a tracer*/
    char      pl_Msg2[500];            /* contient modification si vl_Resultat_out XDC_AJOUT,...*/
    char      pl_NomProcs[80] ;
    
    XDY_ResConf vl_Resultat_out;
    
    XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MSE_XZAO165_166");
    
    strcpy( vl_NomBase,CGCDG_Base_Courante);    
    if ((vl_retcode =XZAO166_Ajouter_GTC_NICE(vl_NomBase,va_GTC_NICE_in ,&vl_Resultat_out))!=XDC_OK)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO165_166 a echoue sur ligne %ld",(long)va_Resultat_out);
        vl_retcode_out = XDC_NOK;    
    }
    else
    /*A
    ** Test sur parametres de sortie de XZAO166_Ajouter_GTC_NICE sur coherence base
    ** 4 valeurs de status d execution de la sp dans vl_Resultat_out
    ** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
    */
    {
        if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
        {
            strcpy(pl_NomProcs,"XZAO166_Ajouter_GTC_NICE");
            XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld",pl_NomProcs,pl_Msg2,(long)va_Resultat_out);
            
            sprintf(pl_Msg,"%s %s sur la ligne %ld",XDF_CONF_GTC_NICE,pl_Msg2,(long)va_Resultat_out);
            cuti_BilanConf_Trace(pl_Msg);    /* Notification dans Bilan config     */
            
        }
        else
        {
            XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO165_166 : cmvf_Trace_TYPE_Result a echoue");
            vl_retcode_out = XDC_NOK;
        }
    
        vl_retcode = vl_retcode_out;
    }    
    XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MSE_XZAO165_166 : sortie avec %d",vl_retcode_out);
    return(vl_retcode_out);
}
/* AAZ 14/11/2007 */
/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*    cmvf_FctUti_MSE_XZAO0165_0166 : Fct utilisateur passee a XZAO0165,
*  qui appelle XZAO0166_Ajouter_GTC_NICE_RENOVEE 
*  avec le nom de la base sur laquelle XZAO0166_Ajouter_GTC_NICE_RENOVEE va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cmvf_FctUti_MSE_XZAO0165_0166 ( XZAOT_ConfGTCNICERENOVEE  va_GTC_NICE_in,
                                    XDY_Entier                va_Resultat_out) 
/*
* ARGUMENTS EN ENTREE :  va_GTC_NICE_in contient la GTC de Nice Renovee a rajouter
*            va_Resultat_out contient le numero de ligne du fichier qui est en cours 
*            de traitement.
*                 
* ARGUMENTS EN SORTIE :  aucun
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
*
* CONDITION D'UTILISATION
*
* FONCTION 
*    Doit appeler XZAO0166_Ajouter_GTC_NICE_RENOVEE
*     
------------------------------------------------------*/
{
    static char *version     = "$Id: cgcd_mSGBDeq.c,v 1.33 2021/05/10 08:52:03 pc2dpdy Exp $ : cmvf_FctUti_MSE_XZAO0165_0166";
    char        vl_NomBase[4]    ;
    int         vl_retcode     = XDC_OK;
    int         vl_retcode_out     = XDC_OK;
    char          pl_Msg[500];            /* contient le mesg a tracer*/
    char          pl_Msg2[500];            /* contient modification si vl_Resultat_out XDC_AJOUT,...*/
    char          pl_NomProcs[80] ;
    
    XDY_ResConf vl_Resultat_out;
    
    XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MSE_XZAO0165_0166");
    
    strcpy( vl_NomBase,CGCDG_Base_Courante);    
    if ((vl_retcode =XZAO0166_Ajouter_GTC_NICE_RENOVEE( vl_NomBase, va_GTC_NICE_in , &vl_Resultat_out))!=XDC_OK)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO0165_0166 a echoue sur ligne %ld", (long)va_Resultat_out);
        vl_retcode_out = XDC_NOK;    
    }
    else
    /*A
    ** Test sur parametres de sortie de XZAO0166_Ajouter_GTC_NICE_RENOVEE sur coherence base
    ** 4 valeurs de status d execution de la sp dans vl_Resultat_out
    ** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
    */
    {
        if ((vl_retcode = cmvf_Trace_TYPE_Result( vl_Resultat_out, pl_Msg2, &vl_retcode_out )) == XDC_OK)
        {
            strcpy( pl_NomProcs, "XZAO0166_Ajouter_GTC_NICE_RENOVEE" );
            XZST_03EcritureTrace(XZSTC_WARNING, "%s %s sur la ligne %ld", pl_NomProcs, pl_Msg2, (long)va_Resultat_out );
            
            sprintf( pl_Msg, "%s %s sur la ligne %ld", XDF_CONF_GTC_NICE_RENOVEE, pl_Msg2, (long)va_Resultat_out );
            cuti_BilanConf_Trace( pl_Msg );    /* Notification dans Bilan config     */
            
        }
        else
        {
            XZST_03EcritureTrace( XZSTC_WARNING, "cmvf_FctUti_MSE_XZAO0165_0166 : cmvf_Trace_TYPE_Result a echoue" );
            vl_retcode_out = XDC_NOK;
        }
    
        vl_retcode = vl_retcode_out;
    }    
    XZST_03EcritureTrace( XZSTC_FONCTION, "OUT : cmvf_FctUti_MSE_XZAO165_166 : sortie avec %d", vl_retcode_out );
    return( vl_retcode_out );
}

/* AAZ */

/* AAZ 16/11/2007 */



/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*    cmvf_FctUti_MSE_XZAO993_994 : Fct utilisateur passee a xzao993_Lire_Config_Seq_SIG,
*  qui appelle XZAO994_Ajouter_Seq_SIG avec le nom de la base sur la quelle XZAO994_Ajouter_Seq_SIG 
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cmvf_FctUti_MSE_XZAO993_994(XZAOT_ConfSeqSIG  va_Seq_SIG_in, XDY_Entier  va_Resultat_out) 
/*
* ARGUMENTS EN ENTREE :  va_Seq_SIG_in contient une sequence dans le domaine SIG a rajouter
*            va_Resultat_out contient le numero de ligne du fichier qui est en cours 
*            de traitement.
*                 
* ARGUMENTS EN SORTIE :  aucun
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
*
* CONDITION D'UTILISATION
*
* FONCTION 
*    Doit appeler XZAO994_Ajouter_Seq_SIG
*     
------------------------------------------------------*/
{
    static  char *version   = "$Id: cgcd_mSGBDeq.c,v 1.33 2021/05/10 08:52:03 pc2dpdy Exp $ : cmvf_FctUti_MSE_XZAO993_994";
    char    vl_NomBase[4]   ;
    int     vl_retcode      = XDC_OK;
    int     vl_retcode_out  = XDC_OK;
    char    pl_Msg[500];            /* contient le mesg a tracer*/
    char    pl_Msg2[500];           /* contient modification si vl_Resultat_out XDC_AJOUT,...*/
    char    pl_NomProcs[80] ;
    
    XDY_ResConf vl_Resultat_out;
    
    XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MSE_XZAO993_994");
    
    strcpy( vl_NomBase,CGCDG_Base_Courante);    
    if ((vl_retcode =XZAO994_Ajouter_Seq_SIG(vl_NomBase,va_Seq_SIG_in, &vl_Resultat_out))!=XDC_OK)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO993_994 a echoue sur ligne %ld",(long)va_Resultat_out);
        vl_retcode_out = XDC_NOK;    
    }
    else
    /*A
    ** Test sur parametres de sortie de XZAO994_Ajouter_Seq_SIG sur coherence base
    ** 4 valeurs de status d execution de la sp dans vl_Resultat_out
    ** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
    */
    {
        if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
        {
            strcpy(pl_NomProcs,"XZAO994_Ajouter_Seq_SIG");
            XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld",pl_NomProcs,pl_Msg2,(long)va_Resultat_out);
            
            sprintf(pl_Msg,"%s %s sur la ligne %ld",XDF_CONF_SEQ_AUT_SIG,pl_Msg2,(long)va_Resultat_out);
            cuti_BilanConf_Trace(pl_Msg);    /* Notification dans Bilan config     */
            
        }
        else
        {
            XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO993_994 : cmvf_Trace_TYPE_Result a echoue");
            vl_retcode_out = XDC_NOK;
        }
    
        vl_retcode = vl_retcode_out;
    }    
    XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MSE_XZAO993_994 : sortie avec %d",vl_retcode_out);
    return(vl_retcode_out);
}


/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*    cmvf_FctUti_MSE_XZAO1011_1012 : Fct utilisateur passee a xzao993_Lire_Config_Seq_EXP,
*  qui appelle XZAO1012_Ajouter_Seq_EXP avec le nom de la base sur la quelle XZAO1012_Ajouter_Seq_EXP 
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cmvf_FctUti_MSE_XZAO1011_1012(XZAOT_ConfSeqEXP  va_Seq_EXP_in, XDY_Entier  va_Resultat_out) 
/*
* ARGUMENTS EN ENTREE :  va_Seq_EXP_in contient une sequence dans le domaine EXP a rajouter
*            va_Resultat_out contient le numero de ligne du fichier qui est en cours 
*            de traitement.
*                 
* ARGUMENTS EN SORTIE :  aucun
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
*
* CONDITION D'UTILISATION
*
* FONCTION 
*    Doit appeler XZAO1012_Ajouter_Seq_EXP
*     
------------------------------------------------------*/
{
    static  char *version   = "$Id: cgcd_mSGBDeq.c,v 1.33 2021/05/10 08:52:03 pc2dpdy Exp $ : cmvf_FctUti_MSE_XZAO1011_1012";
    char    vl_NomBase[4]   ;
    int     vl_retcode      = XDC_OK;
    int     vl_retcode_out  = XDC_OK;
    char    pl_Msg[500];            /* contient le mesg a tracer*/
    char    pl_Msg2[500];           /* contient modification si vl_Resultat_out XDC_AJOUT,...*/
    char    pl_NomProcs[80] ;
    
    XDY_ResConf vl_Resultat_out;
    
    XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MSE_XZAO1011_1012");
    
    strcpy( vl_NomBase,CGCDG_Base_Courante);    
    if ((vl_retcode =XZAO1012_Ajouter_Seq_EXP(vl_NomBase,va_Seq_EXP_in, &vl_Resultat_out))!=XDC_OK)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO1011_1012 a echoue sur ligne %ld",(long)va_Resultat_out);
        vl_retcode_out = XDC_NOK;    
    }
    else
    /*A
    ** Test sur parametres de sortie de XZAO1012_Ajouter_Seq_EXP sur coherence base
    ** 4 valeurs de status d execution de la sp dans vl_Resultat_out
    ** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
    */
    {
        if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
        {
            strcpy(pl_NomProcs,"XZAO1012_Ajouter_Seq_EXP");
            XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld",pl_NomProcs,pl_Msg2,(long)va_Resultat_out);
            
            sprintf(pl_Msg,"%s %s sur la ligne %ld",XDF_CONF_SEQ_AUT_EXP,pl_Msg2,(long)va_Resultat_out);
            cuti_BilanConf_Trace(pl_Msg);    /* Notification dans Bilan config     */
            
        }
        else
        {
            XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO1011_1012 : cmvf_Trace_TYPE_Result a echoue");
            vl_retcode_out = XDC_NOK;
        }
    
        vl_retcode = vl_retcode_out;
    }    
    XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MSE_XZAO1011_1012 : sortie avec %d",vl_retcode_out);
    return(vl_retcode_out);
}



/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*    cmvf_FctUti_MSE_XZAO995_996 : Fct utilisateur passee a xzao995_Lire_Config_Seq_ECL,
*  qui appelle XZAO996_Ajouter_Seq_ECL avec le nom de la base sur la quelle XZAO996_Ajouter_Seq_ECL 
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cmvf_FctUti_MSE_XZAO995_996(XZAOT_ConfSeqECL  va_Seq_ECL_in, XDY_Entier  va_Resultat_out) 
/*
* ARGUMENTS EN ENTREE :  va_Seq_ECL_in contient une sequence dans le domaine ECL a rajouter
*            va_Resultat_out contient le numero de ligne du fichier qui est en cours 
*            de traitement.
*                 
* ARGUMENTS EN SORTIE :  aucun
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
*
* CONDITION D'UTILISATION
*
* FONCTION 
*    Doit appeler XZAO996_Ajouter_Seq_ECL
*     
------------------------------------------------------*/
{
    static  char *version   = "$Id: cgcd_mSGBDeq.c,v 1.33 2021/05/10 08:52:03 pc2dpdy Exp $ : cmvf_FctUti_MSE_XZAO995_996";
    char    vl_NomBase[4]   ;
    int     vl_retcode      = XDC_OK;
    int     vl_retcode_out  = XDC_OK;
    char    pl_Msg[500];            /* contient le mesg a tracer*/
    char    pl_Msg2[500];           /* contient modification si vl_Resultat_out XDC_AJOUT,...*/
    char    pl_NomProcs[80] ;
    
    XDY_ResConf vl_Resultat_out;
    
    XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MSE_XZAO995_996");
    
    strcpy( vl_NomBase,CGCDG_Base_Courante);    
    if ((vl_retcode =XZAO996_Ajouter_Seq_ECL(vl_NomBase,va_Seq_ECL_in, &vl_Resultat_out))!=XDC_OK)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO995_996 a echoue sur ligne %ld",(long)va_Resultat_out);
        vl_retcode_out = XDC_NOK;    
    }
    else
    /*A
    ** Test sur parametres de sortie de XZAO996_Ajouter_Seq_ECL sur coherence base
    ** 4 valeurs de status d execution de la sp dans vl_Resultat_out
    ** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
    */
    {
        if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
        {
            strcpy(pl_NomProcs,"XZAO996_Ajouter_Seq_ECL");
            XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld",pl_NomProcs,pl_Msg2,(long)va_Resultat_out);
            
            sprintf(pl_Msg,"%s %s sur la ligne %ld",XDF_CONF_SEQ_AUT_ECL,pl_Msg2,(long)va_Resultat_out);
            cuti_BilanConf_Trace(pl_Msg);    /* Notification dans Bilan config     */
            
        }
        else
        {
            XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO995_996 : cmvf_Trace_TYPE_Result a echoue");
            vl_retcode_out = XDC_NOK;
        }
    
        vl_retcode = vl_retcode_out;
    }    
    XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MSE_XZAO995_996 : sortie avec %d",vl_retcode_out);
    return(vl_retcode_out);
}



/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*    cmvf_FctUti_MSE_XZAO997_998 : Fct utilisateur passee a xzao997_Lire_Config_Seq_VEN,
*  qui appelle XZAO998_Ajouter_Seq_VEN avec le nom de la base sur la quelle XZAO998_Ajouter_Seq_VEN 
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cmvf_FctUti_MSE_XZAO997_998(XZAOT_ConfSeqVEN  va_Seq_VEN_in, XDY_Entier  va_Resultat_out) 
/*
* ARGUMENTS EN ENTREE :  va_Seq_VEN_in contient une sequence dans le domaine VEN a rajouter
*            va_Resultat_out contient le numero de ligne du fichier qui est en cours 
*            de traitement.
*                 
* ARGUMENTS EN SORTIE :  aucun
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
*
* CONDITION D'UTILISATION
*
* FONCTION 
*    Doit appeler XZAO998_Ajouter_Seq_VEN
*     
------------------------------------------------------*/
{
    static  char *version   = "$Id: cgcd_mSGBDeq.c,v 1.33 2021/05/10 08:52:03 pc2dpdy Exp $ : cmvf_FctUti_MSE_XZAO997_998";
    char    vl_NomBase[4]   ;
    int     vl_retcode      = XDC_OK;
    int     vl_retcode_out  = XDC_OK;
    char    pl_Msg[500];            /* contient le mesg a tracer*/
    char    pl_Msg2[500];           /* contient modification si vl_Resultat_out XDC_AJOUT,...*/
    char    pl_NomProcs[80] ;
    
    XDY_ResConf vl_Resultat_out;
    
    XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MSE_XZAO997_998");
    
    strcpy( vl_NomBase,CGCDG_Base_Courante);    
    if ((vl_retcode =XZAO998_Ajouter_Seq_VEN(vl_NomBase,va_Seq_VEN_in, &vl_Resultat_out))!=XDC_OK)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO997_998 a echoue sur ligne %ld",(long)va_Resultat_out);
        vl_retcode_out = XDC_NOK;    
    }
    else
    /*A
    ** Test sur parametres de sortie de XZAO998_Ajouter_Seq_VEN sur coherence base
    ** 4 valeurs de status d execution de la sp dans vl_Resultat_out
    ** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
    */
    {
        if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
        {
            strcpy(pl_NomProcs,"XZAO998_Ajouter_Seq_VEN");
            XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld",pl_NomProcs,pl_Msg2,(long)va_Resultat_out);
            
            sprintf(pl_Msg,"%s %s sur la ligne %ld",XDF_CONF_SEQ_AUT_VEN,pl_Msg2,(long)va_Resultat_out);
            cuti_BilanConf_Trace(pl_Msg);    /* Notification dans Bilan config     */
            
        }
        else
        {
            XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO997_998 : cmvf_Trace_TYPE_Result a echoue");
            vl_retcode_out = XDC_NOK;
        }
    
        vl_retcode = vl_retcode_out;
    }    
    XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MSE_XZAO997_998 : sortie avec %d",vl_retcode_out);
    return(vl_retcode_out);
}



/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*    cmvf_FctUti_MSE_XZAO999_1000 : Fct utilisateur passee a xzao999_Lire_Config_Seq_ENR,
*  qui appelle XZAO1000_Ajouter_Seq_ENR avec le nom de la base sur la quelle XZAO1000_Ajouter_Seq_ENR 
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cmvf_FctUti_MSE_XZAO999_1000(XZAOT_ConfSeqENR  va_Seq_ENR_in, XDY_Entier  va_Resultat_out) 
/*
* ARGUMENTS EN ENTREE :  va_Seq_ENR_in contient une sequence dans le domaine ENR a rajouter
*            va_Resultat_out contient le numero de ligne du fichier qui est en cours 
*            de traitement.
*                 
* ARGUMENTS EN SORTIE :  aucun
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
*
* CONDITION D'UTILISATION
*
* FONCTION 
*    Doit appeler XZAO1000_Ajouter_Seq_ENR
*     
------------------------------------------------------*/
{
    static  char *version   = "$Id: cgcd_mSGBDeq.c,v 1.33 2021/05/10 08:52:03 pc2dpdy Exp $ : cmvf_FctUti_MSE_XZAO999_1000";
    char    vl_NomBase[4]   ;
    int     vl_retcode      = XDC_OK;
    int     vl_retcode_out  = XDC_OK;
    char    pl_Msg[500];            /* contient le mesg a tracer*/
    char    pl_Msg2[500];           /* contient modification si vl_Resultat_out XDC_AJOUT,...*/
    char    pl_NomProcs[80] ;
    
    XDY_ResConf vl_Resultat_out;
    
    XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MSE_XZAO999_1000");
    
    strcpy( vl_NomBase,CGCDG_Base_Courante);    
    if ((vl_retcode =XZAO1000_Ajouter_Seq_ENR(vl_NomBase,va_Seq_ENR_in, &vl_Resultat_out))!=XDC_OK)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO999_1000 a echoue sur ligne %ld",(long)va_Resultat_out);
        vl_retcode_out = XDC_NOK;    
    }
    else
    /*A
    ** Test sur parametres de sortie de XZAO1000_Ajouter_Seq_ENR sur coherence base
    ** 4 valeurs de status d execution de la sp dans vl_Resultat_out
    ** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
    */
    {
        if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
        {
            strcpy(pl_NomProcs,"XZAO1000_Ajouter_Seq_ENR");
            XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld",pl_NomProcs,pl_Msg2,(long)va_Resultat_out);
            
            sprintf(pl_Msg,"%s %s sur la ligne %ld",XDF_CONF_SEQ_AUT_ENR,pl_Msg2,(long)va_Resultat_out);
            cuti_BilanConf_Trace(pl_Msg);    /* Notification dans Bilan config     */
            
        }
        else
        {
            XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO999_1000 : cmvf_Trace_TYPE_Result a echoue");
            vl_retcode_out = XDC_NOK;
        }
    
        vl_retcode = vl_retcode_out;
    }    
    XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MSE_XZAO999_1000 : sortie avec %d",vl_retcode_out);
    return(vl_retcode_out);
}



/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*    cmvf_FctUti_MSE_XZAO1001_1002 : Fct utilisateur passee a xzao1001_Lire_Config_Seq_INC,
*  qui appelle XZAO1002_Ajouter_Seq_INC avec le nom de la base sur la quelle XZAO1002_Ajouter_Seq_INC 
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cmvf_FctUti_MSE_XZAO1001_1002(XZAOT_ConfSeqINC  va_Seq_INC_in, XDY_Entier  va_Resultat_out) 
/*
* ARGUMENTS EN ENTREE :  va_Seq_INC_in contient une sequence dans le domaine INC a rajouter
*            va_Resultat_out contient le numero de ligne du fichier qui est en cours 
*            de traitement.
*                 
* ARGUMENTS EN SORTIE :  aucun
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
*
* CONDITION D'UTILISATION
*
* FONCTION 
*    Doit appeler XZAO1002_Ajouter_Seq_INC
*     
------------------------------------------------------*/
{
    static  char *version   = "$Id: cgcd_mSGBDeq.c,v 1.33 2021/05/10 08:52:03 pc2dpdy Exp $ : cmvf_FctUti_MSE_XZAO1001_1002";
    char    vl_NomBase[4]   ;
    int     vl_retcode      = XDC_OK;
    int     vl_retcode_out  = XDC_OK;
    char    pl_Msg[500];            /* contient le mesg a tracer*/
    char    pl_Msg2[500];           /* contient modification si vl_Resultat_out XDC_AJOUT,...*/
    char    pl_NomProcs[80] ;
    
    XDY_ResConf vl_Resultat_out;
    
    XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MSE_XZAO1001_1002");
    
    strcpy( vl_NomBase,CGCDG_Base_Courante);    
    if ((vl_retcode =XZAO1002_Ajouter_Seq_INC(vl_NomBase,va_Seq_INC_in, &vl_Resultat_out))!=XDC_OK)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO1001_1002 a echoue sur ligne %ld",(long)va_Resultat_out);
        vl_retcode_out = XDC_NOK;    
    }
    else
    /*A
    ** Test sur parametres de sortie de XZAO1002_Ajouter_Seq_INC sur coherence base
    ** 4 valeurs de status d execution de la sp dans vl_Resultat_out
    ** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
    */
    {
        if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
        {
            strcpy(pl_NomProcs,"XZAO1002_Ajouter_Seq_INC");
            XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld",pl_NomProcs,pl_Msg2,(long)va_Resultat_out);
            
            sprintf(pl_Msg,"%s %s sur la ligne %ld",XDF_CONF_SEQ_AUT_INC,pl_Msg2,(long)va_Resultat_out);
            cuti_BilanConf_Trace(pl_Msg);    /* Notification dans Bilan config     */
            
        }
        else
        {
            XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO1001_1002 : cmvf_Trace_TYPE_Result a echoue");
            vl_retcode_out = XDC_NOK;
        }
    
        vl_retcode = vl_retcode_out;
    }    
    XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MSE_XZAO1001_1002 : sortie avec %d",vl_retcode_out);
    return(vl_retcode_out);
}


/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*    cmvf_FctUti_MSE_XZAO981_982 : Fct utilisateur passee a xzao981_Lire_Config_SeqAut_SIG,
*  qui appelle XZAO982_Ajouter_SeqAut_SIG avec le nom de la base sur la quelle XZAO982_Ajouter_SeqAut_SIG 
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cmvf_FctUti_MSE_XZAO981_982(XZAOT_ConfSeqAutSIG  va_Seq_Aut_SIG_in, XDY_Entier  va_Resultat_out) 
/*
* ARGUMENTS EN ENTREE :  va_Seq_Aut_SIG_in contient une sequence autorise dans le domaine SIG a rajouter
*            va_Resultat_out contient le numero de ligne du fichier qui est en cours 
*            de traitement.
*                 
* ARGUMENTS EN SORTIE :  aucun
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
*
* CONDITION D'UTILISATION
*
* FONCTION 
*    Doit appeler XZAO982_Ajouter_SeqAut_SIG
*     
------------------------------------------------------*/
{
    static  char *version   = "$Id: cgcd_mSGBDeq.c,v 1.33 2021/05/10 08:52:03 pc2dpdy Exp $ : cmvf_FctUti_MSE_XZAO981_982";
    char    vl_NomBase[4]   ;
    int     vl_retcode      = XDC_OK;
    int     vl_retcode_out  = XDC_OK;
    char    pl_Msg[500];            /* contient le mesg a tracer*/
    char    pl_Msg2[500];           /* contient modification si vl_Resultat_out XDC_AJOUT,...*/
    char    pl_NomProcs[80] ;
    
    XDY_ResConf vl_Resultat_out;
    
    XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MSE_XZAO981_982");
    
    strcpy( vl_NomBase,CGCDG_Base_Courante);    
    if ((vl_retcode =XZAO982_Ajouter_SeqAut_SIG(vl_NomBase,va_Seq_Aut_SIG_in, &vl_Resultat_out))!=XDC_OK)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO981_982 a echoue sur ligne %ld",(long)va_Resultat_out);
        vl_retcode_out = XDC_NOK;    
    }
    else
    /*A
    ** Test sur parametres de sortie de XZAO982_Ajouter_SeqAut_SIG sur coherence base
    ** 4 valeurs de status d execution de la sp dans vl_Resultat_out
    ** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
    */
    {
        if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
        {
            strcpy(pl_NomProcs,"XZAO982_Ajouter_SeqAut_SIG");
            XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld",pl_NomProcs,pl_Msg2,(long)va_Resultat_out);
            
            sprintf(pl_Msg,"%s %s sur la ligne %ld",XDF_CONF_SEQ_AUT_SIG,pl_Msg2,(long)va_Resultat_out);
            cuti_BilanConf_Trace(pl_Msg);    /* Notification dans Bilan config     */
            
        }
        else
        {
            XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO981_982 : cmvf_Trace_TYPE_Result a echoue");
            vl_retcode_out = XDC_NOK;
        }
    
        vl_retcode = vl_retcode_out;
    }    
    XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MSE_XZAO981_982 : sortie avec %d",vl_retcode_out);
    return(vl_retcode_out);
}

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*    cmvf_FctUti_MSE_XZAO891_892 : Fct utilisateur passee a xzao891_Lire_Config_SeqRet_SIG,
*  qui appelle XZAO892_Ajouter_SeqRet_SIG avec le nom de la base sur la quelle XZAO892_Ajouter_SeqRet_SIG 
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cmvf_FctUti_MSE_XZAO891_892(XZAOT_ConfSeqRetSIG  va_Seq_Ret_SIG_in, XDY_Entier  va_Resultat_out) 
/*
* ARGUMENTS EN ENTREE :  va_Seq_Ret_SIG_in contient une correspondance entre les sequence sur le terrain et les sequence
*            de retour à la normal de celle-ci
*            va_Resultat_out contient le numero de ligne du fichier qui est en cours 
*            de traitement.
*                 
* ARGUMENTS EN SORTIE :  aucun
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
*
* CONDITION D'UTILISATION
*
* FONCTION 
*    Doit appeler XZAO892_Ajouter_SeqRet_SIG
*     
------------------------------------------------------*/
{
    static  char *version   = "$Id: cgcd_mSGBDeq.c,v 1.33 2021/05/10 08:52:03 pc2dpdy Exp $ : cmvf_FctUti_MSE_XZAO891_892";
    char    vl_NomBase[4]   ;
    int     vl_retcode      = XDC_OK;
    int     vl_retcode_out  = XDC_OK;
    char    pl_Msg[500];            /* contient le mesg a tracer*/
    char    pl_Msg2[500];           /* contient modification si vl_Resultat_out XDC_AJOUT,...*/
    char    pl_NomProcs[80] ;
    
    XDY_ResConf vl_Resultat_out;
    
    XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MSE_XZAO891_892");
    
    strcpy( vl_NomBase,CGCDG_Base_Courante);    
    if ((vl_retcode =XZAO892_Ajouter_SeqRet_SIG(vl_NomBase,va_Seq_Ret_SIG_in, &vl_Resultat_out))!=XDC_OK)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO891_892 a echoue sur ligne %ld",(long)va_Resultat_out);
        vl_retcode_out = XDC_NOK;    
    }
    else
    /*A
    ** Test sur parametres de sortie de XZAO892_Ajouter_SeqRet_SIG sur coherence base
    ** 4 valeurs de status d execution de la sp dans vl_Resultat_out
    ** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
    */
    {
        if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
        {
            strcpy(pl_NomProcs,"XZAO892_Ajouter_SeqRet_SIG");
            XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld",pl_NomProcs,pl_Msg2,(long)va_Resultat_out);
            
            sprintf(pl_Msg,"%s %s sur la ligne %ld",XDF_CONF_SEQ_AUT_SIG,pl_Msg2,(long)va_Resultat_out);
            cuti_BilanConf_Trace(pl_Msg);    /* Notification dans Bilan config     */
            
        }
        else
        {
            XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO891_892 : cmvf_Trace_TYPE_Result a echoue");
            vl_retcode_out = XDC_NOK;
        }
    
        vl_retcode = vl_retcode_out;
    }    
    XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MSE_XZAO891_892 : sortie avec %d",vl_retcode_out);
    return(vl_retcode_out);
}


/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*    cmvf_FctUti_MSE_XZAO981_982 : Fct utilisateur passee a xzao981_Lire_Config_SeqAut_SIG,
*  qui appelle XZAO982_Ajouter_SeqAut_SIG avec le nom de la base sur la quelle XZAO982_Ajouter_SeqAut_SIG 
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cmvf_FctUti_MSE_XZAO1013_1014(XZAOT_ConfSeqAutEXP  va_Seq_Aut_EXP_in, XDY_Entier  va_Resultat_out) 
/*
* ARGUMENTS EN ENTREE :  va_Seq_Aut_EXP_in contient une sequence autorise dans le domaine EXP a rajouter
*            va_Resultat_out contient le numero de ligne du fichier qui est en cours 
*            de traitement.
*                 
* ARGUMENTS EN SORTIE :  aucun
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
*
* CONDITION D'UTILISATION
*
* FONCTION 
*    Doit appeler XZAO1014_Ajouter_SeqAut_EXP
*     
------------------------------------------------------*/
{
    static  char *version   = "$Id: cgcd_mSGBDeq.c,v 1.33 2021/05/10 08:52:03 pc2dpdy Exp $ : cmvf_FctUti_MSE_XZAO1013_1014";
    char    vl_NomBase[4]   ;
    int     vl_retcode      = XDC_OK;
    int     vl_retcode_out  = XDC_OK;
    char    pl_Msg[500];            /* contient le mesg a tracer*/
    char    pl_Msg2[500];           /* contient modification si vl_Resultat_out XDC_AJOUT,...*/
    char    pl_NomProcs[80] ;
    
    XDY_ResConf vl_Resultat_out;
    
    XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MSE_XZAO1013_1014");
    
    strcpy( vl_NomBase,CGCDG_Base_Courante);    
    if ((vl_retcode =XZAO1014_Ajouter_SeqAut_EXP(vl_NomBase,va_Seq_Aut_EXP_in, &vl_Resultat_out))!=XDC_OK)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO1013_1014 a echoue sur ligne %ld",(long)va_Resultat_out);
        vl_retcode_out = XDC_NOK;    
    }
    else
    /*A
    ** Test sur parametres de sortie de XZAO1014_Ajouter_SeqAut_EXP sur coherence base
    ** 4 valeurs de status d execution de la sp dans vl_Resultat_out
    ** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
    */
    {
        if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
        {
            strcpy(pl_NomProcs,"XZAO1014_Ajouter_SeqAut_EXP");
            XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld",pl_NomProcs,pl_Msg2,(long)va_Resultat_out);
            
            sprintf(pl_Msg,"%s %s sur la ligne %ld",XDF_CONF_SEQ_AUT_EXP,pl_Msg2,(long)va_Resultat_out);
            cuti_BilanConf_Trace(pl_Msg);    /* Notification dans Bilan config     */
            
        }
        else
        {
            XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO1013_1014 : cmvf_Trace_TYPE_Result a echoue");
            vl_retcode_out = XDC_NOK;
        }
    
        vl_retcode = vl_retcode_out;
    }    
    XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MSE_XZAO1013_1014 : sortie avec %d",vl_retcode_out);
    return(vl_retcode_out);
}
/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*    cmvf_FctUti_MSE_XZAO983_984 : Fct utilisateur passee a xzao983_Lire_Config_SeqAut_ECL,
*  qui appelle XZAO984_Ajouter_SeqAut_ECL avec le nom de la base sur la quelle XZAO984_Ajouter_SeqAut_ECL 
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cmvf_FctUti_MSE_XZAO983_984(XZAOT_ConfSeqAutECL  va_Seq_Aut_ECL_in, XDY_Entier  va_Resultat_out) 
/*
* ARGUMENTS EN ENTREE :  va_Seq_Aut_ECL_in contient une sequence autorise dans le domaine ECL a rajouter
*            va_Resultat_out contient le numero de ligne du fichier qui est en cours 
*            de traitement.
*                 
* ARGUMENTS EN SORTIE :  aucun
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
*
* CONDITION D'UTILISATION
*
* FONCTION 
*    Doit appeler XZAO984_Ajouter_SeqAut_ECL
*     
------------------------------------------------------*/
{
    static  char *version   = "$Id: cgcd_mSGBDeq.c,v 1.33 2021/05/10 08:52:03 pc2dpdy Exp $ : cmvf_FctUti_MSE_XZAO983_984";
    char    vl_NomBase[4]   ;
    int     vl_retcode      = XDC_OK;
    int     vl_retcode_out  = XDC_OK;
    char    pl_Msg[500];            /* contient le mesg a tracer*/
    char    pl_Msg2[500];           /* contient modification si vl_Resultat_out XDC_AJOUT,...*/
    char    pl_NomProcs[80] ;
    
    XDY_ResConf vl_Resultat_out;
    
    XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MSE_XZAO983_984");
    
    strcpy( vl_NomBase,CGCDG_Base_Courante);    
    if ((vl_retcode =XZAO984_Ajouter_SeqAut_ECL(vl_NomBase,va_Seq_Aut_ECL_in, &vl_Resultat_out))!=XDC_OK)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO983_984 a echoue sur ligne %ld",(long)va_Resultat_out);
        vl_retcode_out = XDC_NOK;    
    }
    else
    /*A
    ** Test sur parametres de sortie de XZAO984_Ajouter_SeqAut_ECL sur coherence base
    ** 4 valeurs de status d execution de la sp dans vl_Resultat_out
    ** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
    */
    {
        if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
        {
            strcpy(pl_NomProcs,"XZAO984_Ajouter_SeqAut_ECL");
            XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld",pl_NomProcs,pl_Msg2,(long)va_Resultat_out);
            
            sprintf(pl_Msg,"%s %s sur la ligne %ld",XDF_CONF_SEQ_AUT_ECL,pl_Msg2,(long)va_Resultat_out);
            cuti_BilanConf_Trace(pl_Msg);    /* Notification dans Bilan config     */
            
        }
        else
        {
            XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO983_984 : cmvf_Trace_TYPE_Result a echoue");
            vl_retcode_out = XDC_NOK;
        }
    
        vl_retcode = vl_retcode_out;
    }    
    XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MSE_XZAO983_984 : sortie avec %d",vl_retcode_out);
    return(vl_retcode_out);
}

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*    cmvf_FctUti_MSE_XZAO985_986 : Fct utilisateur passee a xzao985_Lire_Config_SeqAut_VEN,
*  qui appelle XZAO986_Ajouter_SeqAut_VEN avec le nom de la base sur la quelle XZAO986_Ajouter_SeqAut_VEN 
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cmvf_FctUti_MSE_XZAO985_986(XZAOT_ConfSeqAutVEN  va_Seq_Aut_VEN_in, XDY_Entier  va_Resultat_out) 
/*
* ARGUMENTS EN ENTREE :  va_Seq_Aut_VEN_in contient une sequence autorise dans le domaine VEN a rajouter
*            va_Resultat_out contient le numero de ligne du fichier qui est en cours 
*            de traitement.
*                 
* ARGUMENTS EN SORTIE :  aucun
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
*
* CONDITION D'UTILISATION
*
* FONCTION 
*    Doit appeler XZAO986_Ajouter_SeqAut_VEN
*     
------------------------------------------------------*/
{
    static  char *version   = "$Id: cgcd_mSGBDeq.c,v 1.33 2021/05/10 08:52:03 pc2dpdy Exp $ : cmvf_FctUti_MSE_XZAO985_986";
    char    vl_NomBase[4]   ;
    int     vl_retcode      = XDC_OK;
    int     vl_retcode_out  = XDC_OK;
    char    pl_Msg[500];            /* contient le mesg a tracer*/
    char    pl_Msg2[500];           /* contient modification si vl_Resultat_out XDC_AJOUT,...*/
    char    pl_NomProcs[80] ;
    
    XDY_ResConf vl_Resultat_out;
    
    XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MSE_XZAO985_986");
    
    strcpy( vl_NomBase,CGCDG_Base_Courante);    
    if ((vl_retcode =XZAO986_Ajouter_SeqAut_VEN(vl_NomBase,va_Seq_Aut_VEN_in, &vl_Resultat_out))!=XDC_OK)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO985_986 a echoue sur ligne %ld",(long)va_Resultat_out);
        vl_retcode_out = XDC_NOK;    
    }
    else
    /*A
    ** Test sur parametres de sortie de XZAO986_Ajouter_SeqAut_VEN sur coherence base
    ** 4 valeurs de status d execution de la sp dans vl_Resultat_out
    ** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
    */
    {
        if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
        {
            strcpy(pl_NomProcs,"XZAO986_Ajouter_SeqAut_VEN");
            XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld",pl_NomProcs,pl_Msg2,(long)va_Resultat_out);
            
            sprintf(pl_Msg,"%s %s sur la ligne %ld",XDF_CONF_SEQ_AUT_VEN,pl_Msg2,(long)va_Resultat_out);
            cuti_BilanConf_Trace(pl_Msg);    /* Notification dans Bilan config     */
            
        }
        else
        {
            XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO985_986 : cmvf_Trace_TYPE_Result a echoue");
            vl_retcode_out = XDC_NOK;
        }
    
        vl_retcode = vl_retcode_out;
    }    
    XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MSE_XZAO985_986 : sortie avec %d",vl_retcode_out);
    return(vl_retcode_out);
}

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*    cmvf_FctUti_MSE_XZAO987_988 : Fct utilisateur passee a xzao987_Lire_Config_SeqAut_ENR,
*  qui appelle XZAO988_Ajouter_SeqAut_ENR avec le nom de la base sur la quelle XZAO988_Ajouter_SeqAut_ENR 
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cmvf_FctUti_MSE_XZAO987_988(XZAOT_ConfSeqAutENR  va_Seq_Aut_ENR_in, XDY_Entier  va_Resultat_out) 
/*
* ARGUMENTS EN ENTREE :  va_Seq_Aut_ENR_in contient une sequence autorise dans le domaine ENR a rajouter
*            va_Resultat_out contient le numero de ligne du fichier qui est en cours 
*            de traitement.
*                 
* ARGUMENTS EN SORTIE :  aucun
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
*
* CONDITION D'UTILISATION
*
* FONCTION 
*    Doit appeler XZAO988_Ajouter_SeqAut_ENR
*     
------------------------------------------------------*/
{
    static  char *version   = "$Id: cgcd_mSGBDeq.c,v 1.33 2021/05/10 08:52:03 pc2dpdy Exp $ : cmvf_FctUti_MSE_XZAO987_988";
    char    vl_NomBase[4]   ;
    int     vl_retcode      = XDC_OK;
    int     vl_retcode_out  = XDC_OK;
    char    pl_Msg[500];            /* contient le mesg a tracer*/
    char    pl_Msg2[500];           /* contient modification si vl_Resultat_out XDC_AJOUT,...*/
    char    pl_NomProcs[80] ;
    
    XDY_ResConf vl_Resultat_out;
    
    XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MSE_XZAO987_988");
    
    strcpy( vl_NomBase,CGCDG_Base_Courante);    
    if ((vl_retcode =XZAO988_Ajouter_SeqAut_ENR(vl_NomBase,va_Seq_Aut_ENR_in, &vl_Resultat_out))!=XDC_OK)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO987_988 a echoue sur ligne %ld",(long)va_Resultat_out);
        vl_retcode_out = XDC_NOK;    
    }
    else
    /*A
    ** Test sur parametres de sortie de XZAO988_Ajouter_SeqAut_ENR sur coherence base
    ** 4 valeurs de status d execution de la sp dans vl_Resultat_out
    ** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
    */
    {
        if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
        {
            strcpy(pl_NomProcs,"XZAO988_Ajouter_SeqAut_ENR");
            XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld",pl_NomProcs,pl_Msg2,(long)va_Resultat_out);
            
            sprintf(pl_Msg,"%s %s sur la ligne %ld",XDF_CONF_SEQ_AUT_ENR,pl_Msg2,(long)va_Resultat_out);
            cuti_BilanConf_Trace(pl_Msg);    /* Notification dans Bilan config     */
            
        }
        else
        {
            XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO987_988 : cmvf_Trace_TYPE_Result a echoue");
            vl_retcode_out = XDC_NOK;
        }
    
        vl_retcode = vl_retcode_out;
    }    
    XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MSE_XZAO987_988 : sortie avec %d",vl_retcode_out);
    return(vl_retcode_out);
}

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*    cmvf_FctUti_MSE_XZAO989_990 : Fct utilisateur passee a xzao989_Lire_Config_SeqAut_INC,
*  qui appelle XZAO990_Ajouter_SeqAut_INC avec le nom de la base sur la quelle XZAO990_Ajouter_SeqAut_INC 
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cmvf_FctUti_MSE_XZAO989_990(XZAOT_ConfSeqAutINC  va_Seq_Aut_INC_in, XDY_Entier  va_Resultat_out) 
/*
* ARGUMENTS EN ENTREE :  va_Seq_Aut_INC_in contient une sequence autorise dans le domaine INC a rajouter
*            va_Resultat_out contient le numero de ligne du fichier qui est en cours 
*            de traitement.
*                 
* ARGUMENTS EN SORTIE :  aucun
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
*
* CONDITION D'UTILISATION
*
* FONCTION 
*    Doit appeler XZAO990_Ajouter_SeqAut_INC
*     
------------------------------------------------------*/
{
    static  char *version   = "$Id: cgcd_mSGBDeq.c,v 1.33 2021/05/10 08:52:03 pc2dpdy Exp $ : cmvf_FctUti_MSE_XZAO989_990";
    char    vl_NomBase[4]   ;
    int     vl_retcode      = XDC_OK;
    int     vl_retcode_out  = XDC_OK;
    char    pl_Msg[500];            /* contient le mesg a tracer*/
    char    pl_Msg2[500];           /* contient modification si vl_Resultat_out XDC_AJOUT,...*/
    char    pl_NomProcs[80] ;
    
    XDY_ResConf vl_Resultat_out;
    
    XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MSE_XZAO989_990");
    
    strcpy( vl_NomBase,CGCDG_Base_Courante);    
    if ((vl_retcode =XZAO990_Ajouter_SeqAut_INC(vl_NomBase,va_Seq_Aut_INC_in, &vl_Resultat_out))!=XDC_OK)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO989_990 a echoue sur ligne %ld",(long)va_Resultat_out);
        vl_retcode_out = XDC_NOK;    
    }
    else
    /*A
    ** Test sur parametres de sortie de XZAO990_Ajouter_SeqAut_INC sur coherence base
    ** 4 valeurs de status d execution de la sp dans vl_Resultat_out
    ** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
    */
    {
        if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
        {
            strcpy(pl_NomProcs,"XZAO990_Ajouter_SeqAut_INC");
            XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld",pl_NomProcs,pl_Msg2,(long)va_Resultat_out);
            
            sprintf(pl_Msg,"%s %s sur la ligne %ld",XDF_CONF_SEQ_AUT_INC,pl_Msg2,(long)va_Resultat_out);
            cuti_BilanConf_Trace(pl_Msg);    /* Notification dans Bilan config     */
            
        }
        else
        {
            XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO989_990 : cmvf_Trace_TYPE_Result a echoue");
            vl_retcode_out = XDC_NOK;
        }
    
        vl_retcode = vl_retcode_out;
    }    
    XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MSE_XZAO989_990 : sortie avec %d",vl_retcode_out);
    return(vl_retcode_out);
}

/* AAZ */
/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*    cmvf_FctUti_MSE_XZAO167_168 : Fct utilisateur passee a XZAO167,
*  qui appelle XZAO168_Ajouter_Sequence_Tube avec le nom de la base sur la quelle XZAO168_Ajouter_Sequence_Tube va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cmvf_FctUti_MSE_XZAO167_168(XZAOT_ConfSeqTube       va_Sequence_Tube_in, XDY_Entier     va_Resultat_out) 
/*
* ARGUMENTS EN ENTREE :  va_Sequence_Tube_in contient la machine a rajouter
*            va_Resultat_out contient le numero de ligne du fichier qui est en cours 
*            de traitement.
*                 
* ARGUMENTS EN SORTIE :  aucun
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
*
* CONDITION D'UTILISATION
*
* FONCTION 
*    Doit appeler XZAO168_Ajouter_Sequence_Tube
*     
------------------------------------------------------*/
{
    static char *version     = "$Id: cgcd_mSGBDeq.c,v 1.33 2021/05/10 08:52:03 pc2dpdy Exp $ : cmvf_FctUti_MSE_XZAO167_168";
    char    vl_NomBase[4]    ;
    int     vl_retcode     = XDC_OK;
    int     vl_retcode_out     = XDC_OK;
    char      pl_Msg[500];            /* contient le mesg a tracer*/
    char      pl_Msg2[500];            /* contient modification si vl_Resultat_out XDC_AJOUT,...*/
    char      pl_NomProcs[80] ;
    
    XDY_ResConf vl_Resultat_out;
    
    XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MSE_XZAO167_168");
    
    strcpy( vl_NomBase,CGCDG_Base_Courante);    
    if ((vl_retcode =XZAO168_Ajouter_Sequence_Tube(vl_NomBase,va_Sequence_Tube_in, &vl_Resultat_out))!=XDC_OK)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO167_168 a echoue sur ligne %ld",(long)va_Resultat_out);
        vl_retcode_out = XDC_NOK;    
    }
    else
    /*A
    ** Test sur parametres de sortie de XZAO168_Ajouter_Sequence_Tube sur coherence base
    ** 4 valeurs de status d execution de la sp dans vl_Resultat_out
    ** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
    */
    {
        if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
        {
            strcpy(pl_NomProcs,"XZAO168_Ajouter_Sequence_Tube");
            XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld",pl_NomProcs,pl_Msg2,(long)va_Resultat_out);
            
            sprintf(pl_Msg,"%s %s sur la ligne %ld",XDF_CONF_SEQ_TUBE,pl_Msg2,(long)va_Resultat_out);
            cuti_BilanConf_Trace(pl_Msg);    /* Notification dans Bilan config     */
            
        }
        else
        {
            XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO167_168 : cmvf_Trace_TYPE_Result a echoue");
            vl_retcode_out = XDC_NOK;
        }
    
        vl_retcode = vl_retcode_out;
    }    
    XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MSE_XZAO167_168 : sortie avec %d",vl_retcode_out);
    return(vl_retcode_out);
}
/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*    cmvf_FctUti_MSE_XZAO173_174 : Fct utilisateur passee a xzao173_Lire_Config_Sequences_Ech,
*  qui appelle XZAO174_Ajouter_Sequence_Ech avec le nom de la base sur la quelle XZAO174_Ajouter_Sequence_Ech 
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cmvf_FctUti_MSE_XZAO173_174(XZAOT_ConfSeqEch        va_Sequence_Ech_in, XDY_Entier     va_Resultat_out) 
/*
* ARGUMENTS EN ENTREE :  va_Sequence_Ech_in contient la sequence echangeur a rajouter
*            va_Resultat_out contient le numero de ligne du fichier qui est en cours 
*            de traitement.
*                 
* ARGUMENTS EN SORTIE :  aucun
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
*
* CONDITION D'UTILISATION
*
* FONCTION 
*    Doit appeler XZAO174_Ajouter_Sequence_Ech
*     
------------------------------------------------------*/
{
    static char *version     = "$Id: cgcd_mSGBDeq.c,v 1.33 2021/05/10 08:52:03 pc2dpdy Exp $ : cmvf_FctUti_MSE_XZAO173_174";
    char    vl_NomBase[4]    ;
    int     vl_retcode     = XDC_OK;
    int     vl_retcode_out     = XDC_OK;
    char      pl_Msg[500];            /* contient le mesg a tracer*/
    char      pl_Msg2[500];            /* contient modification si vl_Resultat_out XDC_AJOUT,...*/
    char      pl_NomProcs[80] ;
    
    XDY_ResConf vl_Resultat_out;
    
    XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MSE_XZAO173_174");
    
    strcpy( vl_NomBase,CGCDG_Base_Courante);    
    if ((vl_retcode =XZAO174_Ajouter_Sequence_Ech(vl_NomBase,va_Sequence_Ech_in, &vl_Resultat_out))!=XDC_OK)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO173_174 a echoue sur ligne %ld",(long)va_Resultat_out);
        vl_retcode_out = XDC_NOK;    
    }
    else
    /*A
    ** Test sur parametres de sortie de XZAO174_Ajouter_Sequence_Ech sur coherence base
    ** 4 valeurs de status d execution de la sp dans vl_Resultat_out
    ** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
    */
    {
        if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
        {
            strcpy(pl_NomProcs,"XZAO174_Ajouter_Sequence_Ech");
            XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld",pl_NomProcs,pl_Msg2,(long)va_Resultat_out);
            
            sprintf(pl_Msg,"%s %s sur la ligne %ld",XDF_CONF_SEQ_ECHANGEUR,pl_Msg2,(long)va_Resultat_out);
            cuti_BilanConf_Trace(pl_Msg);    /* Notification dans Bilan config     */
            
        }
        else
        {
            XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO173_174 : cmvf_Trace_TYPE_Result a echoue");
            vl_retcode_out = XDC_NOK;
        }
    
        vl_retcode = vl_retcode_out;
    }    
    XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MSE_XZAO173_174 : sortie avec %d",vl_retcode_out);
    return(vl_retcode_out);
}    
/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*    cmvf_FctUti_MSE_XZAO99_68 : Fct utilisateur passee a xzao99_Lire_Config_Chaussee,
*  qui appelle XZAO68_Ajouter_Chaussee avec le nom de la base sur la quelle XZAO68_Ajouter_Chaussee 
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cmvf_FctUti_MSE_XZAO99_68( XZAOT_ConfChau          va_Chaussee_in , XDY_Entier     va_Resultat_out) 
/*
* ARGUMENTS EN ENTREE :  va_Chaussee_in contient la chaussee a rajouter
*            va_Resultat_out contient le numero de ligne du fichier qui est en cours 
*            de traitement.
*                 
* ARGUMENTS EN SORTIE :  aucun
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
*
* CONDITION D'UTILISATION
*
* FONCTION 
*    Doit appeler XZAO68_Ajouter_Chaussee
*     
------------------------------------------------------*/
{
    static char *version     = "$Id: cgcd_mSGBDeq.c,v 1.33 2021/05/10 08:52:03 pc2dpdy Exp $ : cmvf_FctUti_MSE_XZAO99_68";
    char    vl_NomBase[4]    ;
    int     vl_retcode     = XDC_OK;
    int     vl_retcode_out     = XDC_OK;
    char      pl_Msg[500];            /* contient le mesg a tracer*/
    char      pl_Msg2[500];            /* contient modification si vl_Resultat_out XDC_AJOUT,...*/
    char      pl_NomProcs[80] ;
    
    XDY_ResConf vl_Resultat_out;
    
    XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MSE_XZAO99_68");
    
    strcpy( vl_NomBase,CGCDG_Base_Courante);    
    if ((vl_retcode =XZAO68_Ajouter_Chaussee(vl_NomBase,va_Chaussee_in, &vl_Resultat_out))!=XDC_OK)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO99_68 a echoue sur ligne %ld",(long)va_Resultat_out);
        vl_retcode_out = XDC_NOK;    
    }
    else
    /*A
    ** Test sur parametres de sortie de XZAO68_Ajouter_Chaussee sur coherence base
    ** 4 valeurs de status d execution de la sp dans vl_Resultat_out
    ** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
    */
    {
        if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
        {
            strcpy(pl_NomProcs,"XZAO68_Ajouter_Chaussee");
            XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld",pl_NomProcs,pl_Msg2,(long)va_Resultat_out);
            
            sprintf(pl_Msg,"%s %s sur la ligne %ld",XDF_CONF_CHAUSSEE,pl_Msg2,(long)va_Resultat_out);
            cuti_BilanConf_Trace(pl_Msg);    /* Notification dans Bilan config     */
            
        }
        else
        {
            XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO99_68 : cmvf_Trace_TYPE_Result a echoue");
            vl_retcode_out = XDC_NOK;
        }
    
        vl_retcode = vl_retcode_out;
    }    
    XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MSE_XZAO99_68 : sortie avec %d",vl_retcode_out);
    return(vl_retcode_out);
}

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*    cmvf_FctUti_MSE_XZAO106_81 : Fct utilisateur passee a xzao106_Lire_Config_Analyseur,
*  qui appelle XZAO81_Ajouter_Analyseur avec le nom de la base sur la quelle XZAO81_Ajouter_Analyseur 
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cmvf_FctUti_MSE_XZAO106_81( XZAOT_ConfAnal          va_Analyseur_in, XDY_Entier     va_Resultat_out) 
/*
* ARGUMENTS EN ENTREE :  va_Analyseur_in contient l analyseur a rajouter
*            va_Resultat_out contient le numero de ligne du fichier qui est en cours 
*            de traitement.
*                 
* ARGUMENTS EN SORTIE :  aucun
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
*
* CONDITION D'UTILISATION
*
* FONCTION 
*    Doit appeler XZAO81_Ajouter_Analyseur
*     
------------------------------------------------------*/
{
    static char *version     = "$Id: cgcd_mSGBDeq.c,v 1.33 2021/05/10 08:52:03 pc2dpdy Exp $ : cmvf_FctUti_MSE_XZAO106_81";
    char    vl_NomBase[4]    ;
    int     vl_retcode     = XDC_OK;
    int     vl_retcode_out     = XDC_OK;
    char      pl_Msg[500];            /* contient le mesg a tracer*/
    char      pl_Msg2[500];            /* contient modification si vl_Resultat_out XDC_AJOUT,...*/
    char      pl_NomProcs[80] ;
    
    XDY_ResConf vl_Resultat_out;
    
    XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MSE_XZAO106_81");
    
    strcpy( vl_NomBase,CGCDG_Base_Courante);    
    if ((vl_retcode =XZAO81_Ajouter_Analyseur(vl_NomBase,va_Analyseur_in, &vl_Resultat_out))!=XDC_OK)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO106_81 a echoue sur ligne %ld",(long)va_Resultat_out);
        vl_retcode_out = XDC_NOK;    
    }
    else
    /*A
    ** Test sur parametres de sortie de XZAO81_Ajouter_Analyseur sur coherence base
    ** 4 valeurs de status d execution de la sp dans vl_Resultat_out
    ** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
    */
    {
        if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
        {
            strcpy(pl_NomProcs,"XZAO81_Ajouter_Analyseur");
            XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld",pl_NomProcs,pl_Msg2,(long)va_Resultat_out);
            
            sprintf(pl_Msg,"%s %s sur la ligne %ld",XDF_CONF_ANALYSEUR,pl_Msg2,(long)va_Resultat_out);
            cuti_BilanConf_Trace(pl_Msg);    /* Notification dans Bilan config     */
            
        }
        else
        {
            XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO106_81 : cmvf_Trace_TYPE_Result a echoue");
            vl_retcode_out = XDC_NOK;
        }
    
        vl_retcode = vl_retcode_out;
    }    
    XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MSE_XZAO106_81 : sortie avec %d",vl_retcode_out);
    return(vl_retcode_out);
}

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*    cmvf_FctUti_MSE_XZAO108_138 : Fct utilisateur passee a xzao108_Lire_Config_LTVideo,
*  qui appelle XZAO138_Ajouter_LTVideo avec le nom de la base sur la quelle XZAO138_Ajouter_LTVideo 
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cmvf_FctUti_MSE_XZAO108_138( XZAOT_ConfLTVideo       va_LTVideo_in , XDY_Entier     va_Resultat_out) 
/*
* ARGUMENTS EN ENTREE :  va_LTVideo_in contient les LTVideo a rajouter
*            va_Resultat_out contient le numero de ligne du fichier qui est en cours 
*            de traitement.
*                 
* ARGUMENTS EN SORTIE :  aucun
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
*
* CONDITION D'UTILISATION
*
* FONCTION 
*    Doit appeler XZAO138_Ajouter_LTVideo
*     
------------------------------------------------------*/
{
    static char *version     = "$Id: cgcd_mSGBDeq.c,v 1.33 2021/05/10 08:52:03 pc2dpdy Exp $ : cmvf_FctUti_MSE_XZAO108_138";
    char    vl_NomBase[4]    ;
    int     vl_retcode     = XDC_OK;
    int     vl_retcode_out     = XDC_OK;
    char      pl_Msg[500];            /* contient le mesg a tracer*/
    char      pl_Msg2[500];            /* contient modification si vl_Resultat_out XDC_AJOUT,...*/
    char      pl_NomProcs[80] ;
    
    XDY_ResConf vl_Resultat_out;
    
    XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MSE_XZAO108_138");
    
    strcpy( vl_NomBase,CGCDG_Base_Courante);    
    if ((vl_retcode =XZAO138_Ajouter_LTVideo(vl_NomBase,va_LTVideo_in, &vl_Resultat_out))!=XDC_OK)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO108_138 a echoue sur ligne %ld",(long)va_Resultat_out);
        vl_retcode_out = XDC_NOK;    
    }
    else
    /*A
    ** Test sur parametres de sortie de XZAO138_Ajouter_LTVideo sur coherence base
    ** 4 valeurs de status d execution de la sp dans vl_Resultat_out
    ** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
    */
    {
        if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
        {
            strcpy(pl_NomProcs,"XZAO138_Ajouter_LTVideo");
            XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld",pl_NomProcs,pl_Msg2,(long)va_Resultat_out);
            
            sprintf(pl_Msg,"%s %s sur la ligne %ld",XDF_CONF_LTVIDEO,pl_Msg2,(long)va_Resultat_out);
            cuti_BilanConf_Trace(pl_Msg);    /* Notification dans Bilan config     */
            
        }
        else
        {
            XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO108_138 : cmvf_Trace_TYPE_Result a echoue");
            vl_retcode_out = XDC_NOK;
        }
    
        vl_retcode = vl_retcode_out;
    }    
    XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MSE_XZAO108_138 : sortie avec %d",vl_retcode_out);
    return(vl_retcode_out);
}
/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*    cmvf_FctUti_MSE_XZAO110_140 : Fct utilisateur passee a xzao110_Lire_Config_Moniteur,
*  qui appelle XZAO140_Ajouter_Moniteur avec le nom de la base sur la quelle XZAO140_Ajouter_Moniteur 
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cmvf_FctUti_MSE_XZAO110_140( XZAOT_ConfMoniteur      va_Moniteur_in , XDY_Entier     va_Resultat_out) 
/*
* ARGUMENTS EN ENTREE :  va_Moniteur_in contient le moniteur a rajouter
*            va_Resultat_out contient le numero de ligne du fichier qui est en cours 
*            de traitement.
*                 
* ARGUMENTS EN SORTIE :  aucun
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
*
* CONDITION D'UTILISATION
*
* FONCTION 
*    Doit appeler XZAO140_Ajouter_Moniteur
*     
------------------------------------------------------*/
{
    static char *version     = "$Id: cgcd_mSGBDeq.c,v 1.33 2021/05/10 08:52:03 pc2dpdy Exp $ : cmvf_FctUti_MSE_XZAO110_140";
    char    vl_NomBase[4]    ;
    int     vl_retcode     = XDC_OK;
    int     vl_retcode_out     = XDC_OK;
    char      pl_Msg[500];            /* contient le mesg a tracer*/
    char      pl_Msg2[500];            /* contient modification si vl_Resultat_out XDC_AJOUT,...*/
    char      pl_NomProcs[80] ;
    
    XDY_ResConf vl_Resultat_out;
    
    XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MSE_XZAO110_140");
    
    strcpy( vl_NomBase,CGCDG_Base_Courante);    
    if ((vl_retcode =XZAO140_Ajouter_Moniteur(vl_NomBase,va_Moniteur_in, &vl_Resultat_out))!=XDC_OK)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO110_140 a echoue sur ligne %ld",(long)va_Resultat_out);
        vl_retcode_out = XDC_NOK;    
    }
    else
    /*A
    ** Test sur parametres de sortie de XZAO140_Ajouter_Moniteur sur coherence base
    ** 4 valeurs de status d execution de la sp dans vl_Resultat_out
    ** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
    */
    {
        if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
        {
            strcpy(pl_NomProcs,"XZAO140_Ajouter_Moniteur");
            XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld",pl_NomProcs,pl_Msg2,(long)va_Resultat_out);
            
            sprintf(pl_Msg,"%s %s sur la ligne %ld",XDF_CONF_MONITEUR,pl_Msg2,(long)va_Resultat_out);
            cuti_BilanConf_Trace(pl_Msg);    /* Notification dans Bilan config     */
            
        }
        else
        {
            XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO110_140 : cmvf_Trace_TYPE_Result a echoue");
            vl_retcode_out = XDC_NOK;
        }
    
        vl_retcode = vl_retcode_out;
    }    
    XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MSE_XZAO110_140 : sortie avec %d",vl_retcode_out);
    return(vl_retcode_out);
}

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*    cmvf_FctUti_MSE_XZAO111_141 : Fct utilisateur passee a xzao111_Lire_Config_Magnetoscope,
*  qui appelle XZAO141_Ajouter_Magnetoscope avec le nom de la base sur la quelle XZAO141_Ajouter_Magnetoscope 
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cmvf_FctUti_MSE_XZAO111_141( XZAOT_ConfMagneto       va_Magneto_in, XDY_Entier     va_Resultat_out) 
/*
* ARGUMENTS EN ENTREE :  va_Magneto_in contient le magnetoscope a rajouter
*            va_Resultat_out contient le numero de ligne du fichier qui est en cours 
*            de traitement.
*                 
* ARGUMENTS EN SORTIE :  aucun
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
*
* CONDITION D'UTILISATION
*
* FONCTION 
*    Doit appeler XZAO141_Ajouter_Magnetoscope
*     
------------------------------------------------------*/
{
    static char *version     = "$Id: cgcd_mSGBDeq.c,v 1.33 2021/05/10 08:52:03 pc2dpdy Exp $ : cmvf_FctUti_MSE_XZAO111_141";
    char    vl_NomBase[4]    ;
    int     vl_retcode     = XDC_OK;
    int     vl_retcode_out     = XDC_OK;
    char      pl_Msg[500];            /* contient le mesg a tracer*/
    char      pl_Msg2[500];            /* contient modification si vl_Resultat_out XDC_AJOUT,...*/
    char      pl_NomProcs[80] ;
    
    XDY_ResConf vl_Resultat_out;
    
    XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MSE_XZAO111_141");
    
    strcpy( vl_NomBase,CGCDG_Base_Courante);    
    if ((vl_retcode =XZAO141_Ajouter_Magnetoscope(vl_NomBase,va_Magneto_in, &vl_Resultat_out))!=XDC_OK)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO111_141 a echoue sur ligne %ld",(long)va_Resultat_out);
        vl_retcode_out = XDC_NOK;    
    }
    else
    /*A
    ** Test sur parametres de sortie de XZAO141_Ajouter_Magnetoscope sur coherence base
    ** 4 valeurs de status d execution de la sp dans vl_Resultat_out
    ** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
    */
    {
        if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
        {
            strcpy(pl_NomProcs,"XZAO141_Ajouter_Magnetoscope");
            XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld",pl_NomProcs,pl_Msg2,(long)va_Resultat_out);
            
            sprintf(pl_Msg,"%s %s sur la ligne %ld",XDF_CONF_MAGNETOSCOPE,pl_Msg2,(long)va_Resultat_out);
            cuti_BilanConf_Trace(pl_Msg);    /* Notification dans Bilan config     */
            
        }
        else
        {
            XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO111_141 : cmvf_Trace_TYPE_Result a echoue");
            vl_retcode_out = XDC_NOK;
        }
    
        vl_retcode = vl_retcode_out;
    }    
    XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MSE_XZAO111_141 : sortie avec %d",vl_retcode_out);
    return(vl_retcode_out);
}
/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*    cmvf_FctUti_MSE_XZAO114_144 : Fct utilisateur passee a xzao114_Lire_Config_PAU,
*  qui appelle XZAO144_Ajouter_PAU avec le nom de la base sur la quelle XZAO144_Ajouter_PAU 
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cmvf_FctUti_MSE_XZAO114_144( XZAOT_ConfPAU   va_PAU_in, XDY_Entier     va_Resultat_out) 
/*
* ARGUMENTS EN ENTREE :  va_PAU_in contient le pau a rajouter
*            va_Resultat_out contient le numero de ligne du fichier qui est en cours 
*            de traitement.
*                 
* ARGUMENTS EN SORTIE :  aucun
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
*
* CONDITION D'UTILISATION
*
* FONCTION 
*    Doit appeler XZAO144_Ajouter_PAU
*     
------------------------------------------------------*/
{
    static char *version     = "$Id: cgcd_mSGBDeq.c,v 1.33 2021/05/10 08:52:03 pc2dpdy Exp $ : cmvf_FctUti_MSE_XZAO114_144";
    char    vl_NomBase[4]    ;
    int     vl_retcode     = XDC_OK;
    int     vl_retcode_out     = XDC_OK;
    char      pl_Msg[500];            /* contient le mesg a tracer*/
    char      pl_Msg2[500];            /* contient modification si vl_Resultat_out XDC_AJOUT,...*/
    char      pl_NomProcs[80] ;
    
    XDY_ResConf vl_Resultat_out;
    
    XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MSE_XZAO114_144");
    
    strcpy( vl_NomBase,CGCDG_Base_Courante);    
    if ((vl_retcode =XZAO144_Ajouter_PAU(vl_NomBase,va_PAU_in, &vl_Resultat_out))!=XDC_OK)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO114_144 a echoue sur ligne %ld",(long)va_Resultat_out);
        vl_retcode_out = XDC_NOK;    
    }
    else
    /*A
    ** Test sur parametres de sortie de XZAO144_Ajouter_PAU sur coherence base
    ** 4 valeurs de status d execution de la sp dans vl_Resultat_out
    ** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
    */
    {
        if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
        {
            strcpy(pl_NomProcs,"XZAO144_Ajouter_PAU");
            XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld",pl_NomProcs,pl_Msg2,(long)va_Resultat_out);
            
            sprintf(pl_Msg,"%s %s sur la ligne %ld",XDF_CONF_PAU,pl_Msg2,(long)va_Resultat_out);
            cuti_BilanConf_Trace(pl_Msg);    /* Notification dans Bilan config     */
        }
        else
        {
            XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO114_144 : cmvf_Trace_TYPE_Result a echoue");
            vl_retcode_out = XDC_NOK;
        }
    
        vl_retcode = vl_retcode_out;
    }    
    XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MSE_XZAO114_144 : sortie avec %d",vl_retcode_out);
    return(vl_retcode_out);
}
/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*    cmvf_FctUti_MSE_XZAO158_159 : Fct utilisateur passee a xzao158_Lire_Config_Scen_Connus,
*  qui appelle XZAO159_Ajouter_Scen_Connu avec le nom de la base sur la quelle XZAO159_Ajouter_Scen_Connu 
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cmvf_FctUti_MSE_XZAO158_159( XZAOT_ConfScenCn        va_Scen_Connu_in, XDY_Entier     va_Resultat_out) 
/*
* ARGUMENTS EN ENTREE :  va_Scen_Connu_in contient le scenario connu a rajouter
*            va_Resultat_out contient le numero de ligne du fichier qui est en cours 
*            de traitement.
*                 
* ARGUMENTS EN SORTIE :  aucun
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
*
* CONDITION D'UTILISATION
*
* FONCTION 
*    Doit appeler XZAO159_Ajouter_Scen_Connu
*     
------------------------------------------------------*/
{
    static char *version     = "$Id: cgcd_mSGBDeq.c,v 1.33 2021/05/10 08:52:03 pc2dpdy Exp $ : cmvf_FctUti_MSE_XZAO158_159";
    char    vl_NomBase[4]    ;
    int     vl_retcode     = XDC_OK;
    int     vl_retcode_out     = XDC_OK;
    char      pl_Msg[500];            /* contient le mesg a tracer*/
    char      pl_Msg2[500];            /* contient modification si vl_Resultat_out XDC_AJOUT,...*/
    char      pl_NomProcs[80] ;
    
    XDY_ResConf vl_Resultat_out;
    
    XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MSE_XZAO158_159");
    
    strcpy( vl_NomBase,CGCDG_Base_Courante);    
    if ((vl_retcode =XZAO159_Ajouter_Scen_Connu(vl_NomBase,va_Scen_Connu_in, &vl_Resultat_out))!=XDC_OK)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO158_159 a echoue sur ligne %ld",(long)va_Resultat_out);
        vl_retcode_out = XDC_NOK;    
    }
    else
    /*A
    ** Test sur parametres de sortie de XZAO159_Ajouter_Scen_Connu sur coherence base
    ** 4 valeurs de status d execution de la sp dans vl_Resultat_out
    ** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
    */
    {
        if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
        {
            strcpy(pl_NomProcs,"XZAO159_Ajouter_Scen_Connu");
            XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld",pl_NomProcs,pl_Msg2,(long)va_Resultat_out);
            
            sprintf(pl_Msg,"%s %s sur la ligne %ld",XDF_CONF_SCENARIO_CONNU,pl_Msg2,(long)va_Resultat_out);
            cuti_BilanConf_Trace(pl_Msg);    /* Notification dans Bilan config     */
            
        }
        else
        {
            XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO158_159 : cmvf_Trace_TYPE_Result a echoue");
            vl_retcode_out = XDC_NOK;
        }
    
        vl_retcode = vl_retcode_out;
    }    
    XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MSE_XZAO158_159 : sortie avec %d",vl_retcode_out);
    return(vl_retcode_out);
}

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*    cmvf_FctUti_MSE_XZAO175_176 : Fct utilisateur passee a xzao175_Lire_Config_Demi_Ech,
*  qui appelle XZAO176_Ajouter_Demi_Ech avec le nom de la base sur la quelle XZAO176_Ajouter_Demi_Ech 
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cmvf_FctUti_MSE_XZAO175_176(XZAOT_ConfDemiEch       va_DemiEch_in , XDY_Entier     va_Resultat_out) 
/*
* ARGUMENTS EN ENTREE :  va_DemiEch_in contient le demi echangeur a rajouter
*            va_Resultat_out contient le numero de ligne du fichier qui est en cours 
*            de traitement.
*                 
* ARGUMENTS EN SORTIE :  aucun
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
*
* CONDITION D'UTILISATION
*
* FONCTION 
*    Doit appeler XZAO176_Ajouter_Demi_Ech
*     
------------------------------------------------------*/
{
    static char *version     = "$Id: cgcd_mSGBDeq.c,v 1.33 2021/05/10 08:52:03 pc2dpdy Exp $ : cmvf_FctUti_MSE_XZAO175_176";
    char    vl_NomBase[4]    ;
    int     vl_retcode     = XDC_OK;
    int     vl_retcode_out     = XDC_OK;
    char      pl_Msg[500];            /* contient le mesg a tracer*/
    char      pl_Msg2[500];            /* contient modification si vl_Resultat_out XDC_AJOUT,...*/
    char      pl_NomProcs[80] ;
    
    XDY_ResConf vl_Resultat_out;
    
    XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MSE_XZAO175_176");
    
    strcpy( vl_NomBase,CGCDG_Base_Courante);    
    if ((vl_retcode =XZAO176_Ajouter_Demi_Ech(vl_NomBase,va_DemiEch_in, &vl_Resultat_out))!=XDC_OK)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO175_176 a echoue sur ligne %ld",(long)va_Resultat_out);
        vl_retcode_out = XDC_NOK;    
    }
    else
    /*A
    ** Test sur parametres de sortie de XZAO176_Ajouter_Demi_Ech sur coherence base
    ** 4 valeurs de status d execution de la sp dans vl_Resultat_out
    ** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
    */
    {
        if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
        {
            strcpy(pl_NomProcs,"XZAO176_Ajouter_Demi_Ech");
            XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld",pl_NomProcs,pl_Msg2,(long)va_Resultat_out);
            
            sprintf(pl_Msg,"%s %s sur la ligne %ld",XDF_CONF_DEMIECHANGEUR,pl_Msg2,(long)va_Resultat_out);    
            cuti_BilanConf_Trace(pl_Msg);    /* Notification dans Bilan config     */
            
        }
        else
        {
            XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO175_176 : cmvf_Trace_TYPE_Result a echoue");
            vl_retcode_out = XDC_NOK;
        }
    
        vl_retcode = vl_retcode_out;
    }    
    XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MSE_XZAO175_176 : sortie avec %d",vl_retcode_out);
    return(vl_retcode_out);
}
/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*    cmvf_FctUti_MSE_XZAO177_178 : Fct utilisateur passee a xzao177_Lire_Config_Tube,
*  qui appelle XZAO178_Ajouter_Tube avec le nom de la base sur la quelle XZAO178_Ajouter_Tube 
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cmvf_FctUti_MSE_XZAO177_178(XZAOT_ConfTube          va_Tube_in, XDY_Entier     va_Resultat_out) 
/*
* ARGUMENTS EN ENTREE :  va_Tube_in contient le tube a rajouter
*            va_Resultat_out contient le numero de ligne du fichier qui est en cours 
*            de traitement.
*                 
* ARGUMENTS EN SORTIE :  aucun
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
*
* CONDITION D'UTILISATION
*
* FONCTION 
*    Doit appeler XZAO178_Ajouter_Tube
*     
------------------------------------------------------*/
{
    static char *version     = "$Id: cgcd_mSGBDeq.c,v 1.33 2021/05/10 08:52:03 pc2dpdy Exp $ : cmvf_FctUti_MSE_XZAO177_178";
    char    vl_NomBase[4]    ;
    int     vl_retcode     = XDC_OK;
    int     vl_retcode_out     = XDC_OK;
    char      pl_Msg[500];            /* contient le mesg a tracer*/
    char      pl_Msg2[500];            /* contient modification si vl_Resultat_out XDC_AJOUT,...*/
    char      pl_NomProcs[80] ;
    
    XDY_ResConf vl_Resultat_out;
    
    XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MSE_XZAO177_178");
    
    strcpy( vl_NomBase,CGCDG_Base_Courante);    
    if ((vl_retcode =XZAO178_Ajouter_Tube(vl_NomBase,va_Tube_in, &vl_Resultat_out))!=XDC_OK)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO177_178 a echoue sur ligne %ld",(long)va_Resultat_out);
        vl_retcode_out = XDC_NOK;    
    }
    else
    /*A
    ** Test sur parametres de sortie de XZAO178_Ajouter_Tube sur coherence base
    ** 4 valeurs de status d execution de la sp dans vl_Resultat_out
    ** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
    */
    {
        if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
        {
            strcpy(pl_NomProcs,"XZAO178_Ajouter_Tube");
            XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld",pl_NomProcs,pl_Msg2,(long)va_Resultat_out);
            
            sprintf(pl_Msg,"%s %s sur la ligne %ld",XDF_CONF_TUBE,pl_Msg2,(long)va_Resultat_out);
            cuti_BilanConf_Trace(pl_Msg);    /* Notification dans Bilan config     */
            
        }
        else
        {
            XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO177_178 : cmvf_Trace_TYPE_Result a echoue");
            vl_retcode_out = XDC_NOK;
        }
    
        vl_retcode = vl_retcode_out;
    }    
    XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MSE_XZAO177_178 : sortie avec %d",vl_retcode_out);
    return(vl_retcode_out);
}

/* AAZ 15/11/2007 Ajout tube et echangeur renoves */

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*  cmvf_FctUti_MSE_XZAO977_978 : Fct utilisateur passee a xzao977_Lire_Config_Tube_Renove,
*  qui appelle XZAO978_Ajouter_Tube_Renove avec le nom de la base sur la quelle XZAO978_Ajouter_Tube_Renove 
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cmvf_FctUti_MSE_XZAO977_978(XZAOT_ConfTubeRenove va_Tube_in, XDY_Entier va_Resultat_out) 
/*
* ARGUMENTS EN ENTREE :  va_Tube_in contient le tube a rajouter
*            va_Resultat_out contient le numero de ligne du fichier qui est en cours 
*            de traitement.
*                 
* ARGUMENTS EN SORTIE :  aucun
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
*
* CONDITION D'UTILISATION
*
* FONCTION 
*    Doit appeler XZAO978_Ajouter_Tube_Renove
*     
------------------------------------------------------*/
{
    static char *version     = "$Id: cgcd_mSGBDeq.c,v 1.33 2021/05/10 08:52:03 pc2dpdy Exp $ : cmvf_FctUti_MSE_XZAO977_978";
    char        vl_NomBase[4]    ;
    int         vl_retcode     = XDC_OK;
    int         vl_retcode_out = XDC_OK;
    char        pl_Msg[500];            /* contient le mesg a tracer*/
    char        pl_Msg2[500];           /* contient modification si vl_Resultat_out XDC_AJOUT,...*/
    char        pl_NomProcs[80] ;
    
    XDY_ResConf vl_Resultat_out;
    
    XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MSE_XZAO977_978");
    
    strcpy( vl_NomBase,CGCDG_Base_Courante);    
    if ((vl_retcode =XZAO978_Ajouter_Tube_Renove( vl_NomBase, va_Tube_in, &vl_Resultat_out))!=XDC_OK)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO977_978 a echoue sur ligne %ld",(long)va_Resultat_out);
        vl_retcode_out = XDC_NOK;    
    }
    else
    /*A
    ** Test sur parametres de sortie de XZAO978_Ajouter_Tube_Renove sur coherence base
    ** 4 valeurs de status d execution de la sp dans vl_Resultat_out
    ** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
    */
    {
        if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
        {
            strcpy(pl_NomProcs,"XZAO978_Ajouter_Tube_Renove");
            XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld",pl_NomProcs,pl_Msg2,(long)va_Resultat_out);
            
            sprintf(pl_Msg,"%s %s sur la ligne %ld",XDF_CONF_TUBE,pl_Msg2,(long)va_Resultat_out);
            cuti_BilanConf_Trace(pl_Msg);    /* Notification dans Bilan config     */
            
        }
        else
        {
            XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO977_978 : cmvf_Trace_TYPE_Result a echoue");
            vl_retcode_out = XDC_NOK;
        }
    
        vl_retcode = vl_retcode_out;
    }    
    XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MSE_XZAO977_978 : sortie avec %d",vl_retcode_out);
    return(vl_retcode_out);
}

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*  cmvf_FctUti_MSE_XZAO979_980 : Fct utilisateur passee a xzao979_Lire_Config_Demi_Ech_Renove,
*  qui appelle XZAO980_Ajouter_Demi_Ech_Renove avec le nom de la base sur la quelle XZAO980_Ajouter_Demi_Ech_Renove 
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cmvf_FctUti_MSE_XZAO979_980(XZAOT_ConfDemiEchRenove va_DemiEch_in , XDY_Entier va_Resultat_out) 
/*
* ARGUMENTS EN ENTREE :  va_DemiEch_in contient le demi echangeur a rajouter
*            va_Resultat_out contient le numero de ligne du fichier qui est en cours 
*            de traitement.
*                 
* ARGUMENTS EN SORTIE :  aucun
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
*
* CONDITION D'UTILISATION
*
* FONCTION 
*    Doit appeler XZAO980_Ajouter_Demi_Ech_Renove
*     
------------------------------------------------------*/
{
    static      char *version    = "$Id: cgcd_mSGBDeq.c,v 1.33 2021/05/10 08:52:03 pc2dpdy Exp $ : cmvf_FctUti_MSE_XZAO979_980";
    char        vl_NomBase[4]    ;
    int         vl_retcode       = XDC_OK;
    int         vl_retcode_out   = XDC_OK;
    char        pl_Msg[500];            /* contient le mesg a tracer*/
    char        pl_Msg2[500];           /* contient modification si vl_Resultat_out XDC_AJOUT,...*/
    char        pl_NomProcs[80] ;

    XDY_ResConf vl_Resultat_out;

    XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MSE_XZAO979_980");

    strcpy( vl_NomBase,CGCDG_Base_Courante);    
    if ((vl_retcode =XZAO980_Ajouter_Demi_Ech_Renove(vl_NomBase, va_DemiEch_in, &vl_Resultat_out))!=XDC_OK)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO979_980 a echoue sur ligne %ld",(long)va_Resultat_out);
        vl_retcode_out = XDC_NOK;    
    }
    else
    /*A
    ** Test sur parametres de sortie de XZAO980_Ajouter_Demi_Ech_Renove sur coherence base
    ** 4 valeurs de status d execution de la sp dans vl_Resultat_out
    ** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
    */
    {
        if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
        {
            strcpy(pl_NomProcs,"XZAO980_Ajouter_Demi_Ech_Renove");
            XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld",pl_NomProcs,pl_Msg2,(long)va_Resultat_out);
            
            sprintf(pl_Msg,"%s %s sur la ligne %ld",XDF_CONF_DEMIECHANGEUR,pl_Msg2,(long)va_Resultat_out);
            cuti_BilanConf_Trace(pl_Msg);    /* Notification dans Bilan config     */
            
        }
        else
        {
            XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO979_980 : cmvf_Trace_TYPE_Result a echoue");
            vl_retcode_out = XDC_NOK;
        }
    
        vl_retcode = vl_retcode_out;
    }    
    XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MSE_XZAO979_980 : sortie avec %d",vl_retcode_out);
    return(vl_retcode_out);
}

/* AAZ */

/* AAZ 29/11/2007 ajout ouvrage lits, locaux techniques et systeme*/

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*  cmvf_FctUti_MSE_XZAO1003_1004 : Fct utilisateur passee a xzao1003_Lire_Config_Lit,
*  qui appelle XZAO1004_Ajouter_LIT avec le nom de la base sur la quelle XZAO1004_Ajouter_LIT 
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cmvf_FctUti_MSE_XZAO1003_1004(XZAOT_ConfLitG va_LIT_in, XDY_Entier va_Resultat_out) 
/*
* ARGUMENTS EN ENTREE :  va_LIT_in contient l'ouvrage a rajouter
*            va_Resultat_out contient le numero de ligne du fichier qui est en cours 
*            de traitement.
*                 
* ARGUMENTS EN SORTIE :  aucun
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
*
* CONDITION D'UTILISATION
*
* FONCTION 
*    Doit appeler XZAO1004_Ajouter_LIT
*     
------------------------------------------------------*/
{
    static char *version     = "$Id: cgcd_mSGBDeq.c,v 1.33 2021/05/10 08:52:03 pc2dpdy Exp $ : cmvf_FctUti_MSE_XZAO1003_1004";
    char        vl_NomBase[4]    ;
    int         vl_retcode     = XDC_OK;
    int         vl_retcode_out = XDC_OK;
    char        pl_Msg[500];            /* contient le mesg a tracer*/
    char        pl_Msg2[500];           /* contient modification si vl_Resultat_out XDC_AJOUT,...*/
    char        pl_NomProcs[80] ;
    
    XDY_ResConf vl_Resultat_out;
    
    XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MSE_XZAO1003_1004");
    
    strcpy( vl_NomBase,CGCDG_Base_Courante);    
    if ((vl_retcode =XZAO1004_Ajouter_LIT( vl_NomBase, va_LIT_in, &vl_Resultat_out))!=XDC_OK)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO1003_1004 a echoue sur ligne %ld",(long)va_Resultat_out);
        vl_retcode_out = XDC_NOK;    
    }
    else
    /*A
    ** Test sur parametres de sortie de XZAO1004_Ajouter_LIT sur coherence base
    ** 4 valeurs de status d execution de la sp dans vl_Resultat_out
    ** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
    */
    {
        if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
        {
            strcpy(pl_NomProcs,"XZAO1004_Ajouter_LIT");
            XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld",pl_NomProcs,pl_Msg2,(long)va_Resultat_out);
            
            sprintf(pl_Msg,"%s %s sur la ligne %ld",XDF_CONF_OUV_LIT,pl_Msg2,(long)va_Resultat_out);
            cuti_BilanConf_Trace(pl_Msg);    /* Notification dans Bilan config     */
            
        }
        else
        {
            XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO1003_1004 : cmvf_Trace_TYPE_Result a echoue");
            vl_retcode_out = XDC_NOK;
        }
    
        vl_retcode = vl_retcode_out;
    }    
    XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MSE_XZAO1003_1004 : sortie avec %d",vl_retcode_out);
    return(vl_retcode_out);
}

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*  cmvf_FctUti_MSE_XZAO1005_1006 : Fct utilisateur passee a xzao1005_Lire_Config_Ltn,
*  qui appelle XZAO1006_Ajouter_LTN avec le nom de la base sur la quelle XZAO1006_Ajouter_LTN 
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cmvf_FctUti_MSE_XZAO1005_1006(XZAOT_ConfLTNG va_LTN_in, XDY_Entier va_Resultat_out) 
/*
* ARGUMENTS EN ENTREE :  va_LTN_in contient l'ouvrage a rajouter
*            va_Resultat_out contient le numero de ligne du fichier qui est en cours 
*            de traitement.
*                 
* ARGUMENTS EN SORTIE :  aucun
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
*
* CONDITION D'UTILISATION
*
* FONCTION 
*    Doit appeler XZAO1006_Ajouter_LTN
*     
------------------------------------------------------*/
{
    static char *version     = "$Id: cgcd_mSGBDeq.c,v 1.33 2021/05/10 08:52:03 pc2dpdy Exp $ : cmvf_FctUti_MSE_XZAO1005_1006";
    char        vl_NomBase[4]    ;
    int         vl_retcode     = XDC_OK;
    int         vl_retcode_out = XDC_OK;
    char        pl_Msg[500];            /* contient le mesg a tracer*/
    char        pl_Msg2[500];           /* contient modification si vl_Resultat_out XDC_AJOUT,...*/
    char        pl_NomProcs[80] ;
    
    XDY_ResConf vl_Resultat_out;
    
    XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MSE_XZAO1005_1006");
    
    strcpy( vl_NomBase,CGCDG_Base_Courante);    
    if ((vl_retcode =XZAO1006_Ajouter_LTN( vl_NomBase, va_LTN_in, &vl_Resultat_out))!=XDC_OK)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO1005_1006 a echoue sur ligne %ld",(long)va_Resultat_out);
        vl_retcode_out = XDC_NOK;    
    }
    else
    /*A
    ** Test sur parametres de sortie de XZAO1006_Ajouter_LTN sur coherence base
    ** 4 valeurs de status d execution de la sp dans vl_Resultat_out
    ** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
    */
    {
        if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
        {
            strcpy(pl_NomProcs,"XZAO1006_Ajouter_LTN");
            XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld",pl_NomProcs,pl_Msg2,(long)va_Resultat_out);
            
            sprintf(pl_Msg,"%s %s sur la ligne %ld",XDF_CONF_OUV_LTN,pl_Msg2,(long)va_Resultat_out);
            cuti_BilanConf_Trace(pl_Msg);    /* Notification dans Bilan config     */
            
        }
        else
        {
            XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO1005_1006 : cmvf_Trace_TYPE_Result a echoue");
            vl_retcode_out = XDC_NOK;
        }
    
        vl_retcode = vl_retcode_out;
    }    
    XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MSE_XZAO1005_1006 : sortie avec %d",vl_retcode_out);
    return(vl_retcode_out);
}


/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*  cmvf_FctUti_MSE_XZAO1007_1008 : Fct utilisateur passee a xzao1007_Lire_Config_Sys,
*  qui appelle XZAO1008_Ajouter_SYS avec le nom de la base sur la quelle XZAO1008_Ajouter_SYS 
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cmvf_FctUti_MSE_XZAO1007_1008(XZAOT_ConfSYSG va_SYS_in, XDY_Entier va_Resultat_out) 
/*
* ARGUMENTS EN ENTREE :  va_SYS_in contient l'ouvrage a rajouter
*            va_Resultat_out contient le numero de ligne du fichier qui est en cours 
*            de traitement.
*                 
* ARGUMENTS EN SORTIE :  aucun
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
*
* CONDITION D'UTILISATION
*
* FONCTION 
*    Doit appeler XZAO1008_Ajouter_SYS
*     
------------------------------------------------------*/
{
    static char *version     = "$Id: cgcd_mSGBDeq.c,v 1.33 2021/05/10 08:52:03 pc2dpdy Exp $ : cmvf_FctUti_MSE_XZAO1007_1008";
    char        vl_NomBase[4]    ;
    int         vl_retcode     = XDC_OK;
    int         vl_retcode_out = XDC_OK;
    char        pl_Msg[500];            /* contient le mesg a tracer*/
    char        pl_Msg2[500];           /* contient modification si vl_Resultat_out XDC_AJOUT,...*/
    char        pl_NomProcs[80] ;
    
    XDY_ResConf vl_Resultat_out;
    
    XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MSE_XZAO1007_1008");
    
    strcpy( vl_NomBase,CGCDG_Base_Courante);    
    if ((vl_retcode =XZAO1008_Ajouter_SYS( vl_NomBase, va_SYS_in, &vl_Resultat_out))!=XDC_OK)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO1007_1008 a echoue sur ligne %ld",(long)va_Resultat_out);
        vl_retcode_out = XDC_NOK;    
    }
    else
    /*A
    ** Test sur parametres de sortie de XZAO1008_Ajouter_SYS sur coherence base
    ** 4 valeurs de status d execution de la sp dans vl_Resultat_out
    ** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
    */
    {
        if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
        {
            strcpy(pl_NomProcs,"XZAO1008_Ajouter_SYS");
            XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld",pl_NomProcs,pl_Msg2,(long)va_Resultat_out);
            
            sprintf(pl_Msg,"%s %s sur la ligne %ld",XDF_CONF_OUV_SYS,pl_Msg2,(long)va_Resultat_out);
            cuti_BilanConf_Trace(pl_Msg);    /* Notification dans Bilan config     */
            
        }
        else
        {
            XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO1007_1008 : cmvf_Trace_TYPE_Result a echoue");
            vl_retcode_out = XDC_NOK;
        }
    
        vl_retcode = vl_retcode_out;
    }    
    XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MSE_XZAO1007_1008 : sortie avec %d",vl_retcode_out);
    return(vl_retcode_out);
}

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*  cmvf_FctUti_MSE_XZAO1009_1010 : Fct utilisateur passee a xzao1009_Lire_Config_EHT,
*  qui appelle XZAO1010_Ajouter_EHT avec le nom de la base sur la quelle XZAO1010_Ajouter_EHT 
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cmvf_FctUti_MSE_XZAO1009_1010(XZAOT_ConfEHTG va_EHT_in, XDY_Entier va_Resultat_out) 
/*
* ARGUMENTS EN ENTREE :  va_EHT_in contient l'ouvrage a rajouter
*            va_Resultat_out contient le numero de ligne du fichier qui est en cours 
*            de traitement.
*                 
* ARGUMENTS EN SORTIE :  aucun
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
*
* CONDITION D'UTILISATION
*
* FONCTION 
*    Doit appeler XZAO1010_Ajouter_EHT
*     
------------------------------------------------------*/
{
    static char *version     = "$Id: cgcd_mSGBDeq.c,v 1.33 2021/05/10 08:52:03 pc2dpdy Exp $ : cmvf_FctUti_MSE_XZAO1009_1010";
    char        vl_NomBase[4]    ;
    int         vl_retcode     = XDC_OK;
    int         vl_retcode_out = XDC_OK;
    char        pl_Msg[500];            /* contient le mesg a tracer*/
    char        pl_Msg2[500];           /* contient modification si vl_Resultat_out XDC_AJOUT,...*/
    char        pl_NomProcs[80] ;
    
    XDY_ResConf vl_Resultat_out;
    
    XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MSE_XZAO1009_1010");
    
    strcpy( vl_NomBase,CGCDG_Base_Courante);    
    if ((vl_retcode =XZAO1010_Ajouter_EHT( vl_NomBase, va_EHT_in, &vl_Resultat_out))!=XDC_OK)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO1009_1010 a echoue sur ligne %ld",(long)va_Resultat_out);
        vl_retcode_out = XDC_NOK;    
    }
    else
    /*A
    ** Test sur parametres de sortie de XZAO1010_Ajouter_EHT sur coherence base
    ** 4 valeurs de status d execution de la sp dans vl_Resultat_out
    ** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
    */
    {
        if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
        {
            strcpy(pl_NomProcs,"XZAO1010_Ajouter_EHT");
            XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld",pl_NomProcs,pl_Msg2,(long)va_Resultat_out);
            
            sprintf(pl_Msg,"%s %s sur la ligne %ld",XDF_CONF_OUV_EHT,pl_Msg2,(long)va_Resultat_out);
            cuti_BilanConf_Trace(pl_Msg);    /* Notification dans Bilan config     */
            
        }
        else
        {
            XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO1009_1010 : cmvf_Trace_TYPE_Result a echoue");
            vl_retcode_out = XDC_NOK;
        }
    
        vl_retcode = vl_retcode_out;
    }    
    XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MSE_XZAO1009_1010 : sortie avec %d",vl_retcode_out);
    return(vl_retcode_out);
}

/* AAZ */

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*    cmvf_FctUti_MSE_XZAO100_69 : Fct utilisateur passee a xzao100_Lire_Config_Pt_Mesure,
*  qui appelle XZAO69_Ajouter_Pt_Mesure avec le nom de la base sur la quelle XZAO69_Ajouter_Pt_Mesure 
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cmvf_FctUti_MSE_XZAO100_69(XZAOT_ConfPtMes         va_Station_in, XDY_Entier     va_Resultat_out) 
/*
* ARGUMENTS EN ENTREE :  va_Station_in contient le point de mesure a rajouter
*            va_Resultat_out contient le numero de ligne du fichier qui est en cours 
*            de traitement.
*                 
* ARGUMENTS EN SORTIE :  aucun
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
*
* CONDITION D'UTILISATION
*
* FONCTION 
*    Doit appeler XZAO69_Ajouter_Pt_Mesure
*     
------------------------------------------------------*/
{
    static char *version     = "$Id: cgcd_mSGBDeq.c,v 1.33 2021/05/10 08:52:03 pc2dpdy Exp $ : cmvf_FctUti_MSE_XZAO100_69";
    char    vl_NomBase[4]    ;
    int     vl_retcode     = XDC_OK;
    int     vl_retcode_out     = XDC_OK;
    char      pl_Msg[500];            /* contient le mesg a tracer*/
    char      pl_Msg2[500];            /* contient modification si vl_Resultat_out XDC_AJOUT,...*/
    char      pl_NomProcs[80] ;
    
    XDY_ResConf vl_Resultat_out;
    
    XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MSE_XZAO100_69");
    
    strcpy( vl_NomBase,CGCDG_Base_Courante);    
    if ((vl_retcode =XZAO69_Ajouter_Pt_Mesure(vl_NomBase,va_Station_in, &vl_Resultat_out))!=XDC_OK)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO100_69 a echoue sur ligne %ld",(long)va_Resultat_out);
        vl_retcode_out = XDC_NOK;    
    }
    else
    /*A
    ** Test sur parametres de sortie de XZAO69_Ajouter_Pt_Mesure sur coherence base
    ** 4 valeurs de status d execution de la sp dans vl_Resultat_out
    ** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
    */
    {
        if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
        {
            strcpy(pl_NomProcs,"XZAO69_Ajouter_Pt_Mesure");
            XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld",pl_NomProcs,pl_Msg2,(long)va_Resultat_out);
            
            sprintf(pl_Msg,"%s %s sur la ligne %ld",XDF_CONF_PT_MESURE,pl_Msg2,(long)va_Resultat_out);
            cuti_BilanConf_Trace(pl_Msg);    /* Notification dans Bilan config     */
            
        }
        else
        {
            XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO100_69 : cmvf_Trace_TYPE_Result a echoue");
            vl_retcode_out = XDC_NOK;
        }
    
        vl_retcode = vl_retcode_out;
    }    
    XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MSE_XZAO100_69 : sortie avec %d",vl_retcode_out);
    return(vl_retcode_out);
}
/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*    cmvf_FctUti_MSE_XZAO109_139 : Fct utilisateur passee a xzao109_Lire_Config_Camera,
*  qui appelle XZAO139_Ajouter_Camera avec le nom de la base sur la quelle XZAO139_Ajouter_Camera 
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cmvf_FctUti_MSE_XZAO109_139(XZAOT_ConfCamera        va_Camera_in, XDY_Entier     va_Resultat_out) 
/*
* ARGUMENTS EN ENTREE :  va_Camera_in contient la camera a rajouter
*            va_Resultat_out contient le numero de ligne du fichier qui est en cours 
*            de traitement.
*                 
* ARGUMENTS EN SORTIE :  aucun
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
*
* CONDITION D'UTILISATION
*
* FONCTION 
*    Doit appeler XZAO139_Ajouter_Camera
*     
------------------------------------------------------*/
{
    static char *version     = "$Id: cgcd_mSGBDeq.c,v 1.33 2021/05/10 08:52:03 pc2dpdy Exp $ : cmvf_FctUti_MSE_XZAO109_139";
    char    vl_NomBase[4]    ;
    int     vl_retcode     = XDC_OK;
    int     vl_retcode_out     = XDC_OK;
    char      pl_Msg[500];            /* contient le mesg a tracer*/
    char      pl_Msg2[500];            /* contient modification si vl_Resultat_out XDC_AJOUT,...*/
    char      pl_NomProcs[80] ;
    
    XDY_ResConf vl_Resultat_out;
    
    XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MSE_XZAO109_139");
    
    strcpy( vl_NomBase,CGCDG_Base_Courante);    
    if ((vl_retcode =XZAO139_Ajouter_Camera(vl_NomBase,va_Camera_in, &vl_Resultat_out))!=XDC_OK)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO109_139 a echoue sur ligne %ld",(long)va_Resultat_out);
        vl_retcode_out = XDC_NOK;    
    }
    else
    /*A
    ** Test sur parametres de sortie de XZAO139_Ajouter_Camera sur coherence base
    ** 4 valeurs de status d execution de la sp dans vl_Resultat_out
    ** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
    */
    {
        if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
        {
            strcpy(pl_NomProcs,"XZAO139_Ajouter_Camera");
            XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld",pl_NomProcs,pl_Msg2,(long)va_Resultat_out);
            
            sprintf(pl_Msg,"%s %s sur la ligne %ld",XDF_CONF_CAMERA,pl_Msg2,(long)va_Resultat_out);
            cuti_BilanConf_Trace(pl_Msg);    /* Notification dans Bilan config     */
            
        }
        else
        {
            XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO109_139 : cmvf_Trace_TYPE_Result a echoue");
            vl_retcode_out = XDC_NOK;
        }
    
        vl_retcode = vl_retcode_out;
    }    
    XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MSE_XZAO109_139 : sortie avec %d",vl_retcode_out);
    return(vl_retcode_out);
}

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*    cmvf_FctUti_MSE_XZAO192_193 : Fct utilisateur passee a xzao192_Lire_Config_Zon_Camera,
*  qui appelle XZAO193_Ajouter_Zon_Camera avec le nom de la base sur la quelle XZAO193_Ajouter_Zon_Camera 
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cmvf_FctUti_MSE_XZAO192_193(XZAOT_ConfZoneCamera        va_Zone_Camera_in, XDY_Entier     va_Resultat_out) 
/*
* ARGUMENTS EN ENTREE :
*            va_Zone_Camera_in contient la zone de couverture camera a rajouter
*            va_Resultat_out contient le numero de ligne du fichier qui est en cours 
*            de traitement.
*
* ARGUMENTS EN SORTIE :  aucun
*
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
*
* CONDITION D'UTILISATION
*
* FONCTION 
*    Doit appeler XZAO193_Ajouter_Zon_Camera
*
------------------------------------------------------*/
{
    static char *version     = "$Id: cgcd_mSGBDeq.c,v 1.33 2021/05/10 08:52:03 pc2dpdy Exp $ : cmvf_FctUti_MSE_XZAO192_193";
    char    vl_NomBase[4]    ;
    int     vl_retcode     = XDC_OK;
    int     vl_retcode_out     = XDC_OK;
    char      pl_Msg[500];            /* contient le mesg a tracer*/
    char      pl_Msg2[500];            /* contient modification si vl_Resultat_out XDC_AJOUT,...*/
    char      pl_NomProcs[80] ;
    
    XDY_ResConf vl_Resultat_out;
    
    XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MSE_XZAO192_193");
    
    strcpy( vl_NomBase,CGCDG_Base_Courante);    
    if ((vl_retcode =XZAO193_Ajouter_Zon_Camera(vl_NomBase,va_Zone_Camera_in, &vl_Resultat_out))!=XDC_OK)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO192_193 a echoue sur ligne %ld",(long)va_Resultat_out);
        vl_retcode_out = XDC_NOK;    
    }
    else
    /*A
    ** Test sur parametres de sortie de XZAO193_Ajouter_Zon_Camera sur coherence base
    ** 4 valeurs de status d execution de la sp dans vl_Resultat_out
    ** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
    */
    {
        if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
        {
            strcpy(pl_NomProcs,"XZAO193_Ajouter_Zon_Camera");
            XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld",pl_NomProcs,pl_Msg2,(long)va_Resultat_out);

            sprintf(pl_Msg,"%s %s sur la ligne %ld",XDF_CONF_ZON_CAM,pl_Msg2,(long)va_Resultat_out);
            cuti_BilanConf_Trace(pl_Msg);    /* Notification dans Bilan config     */
        }
        else
        {
            XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO192_193 : cmvf_Trace_TYPE_Result a echoue");
            vl_retcode_out = XDC_NOK;
        }
    
        vl_retcode = vl_retcode_out;
    }    
    XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MSE_XZAO192_193 : sortie avec %d",vl_retcode_out);
    return(vl_retcode_out);
}

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*    cmvf_FctUti_MSE_XZAO160_161 : Fct utilisateur passee a xzao160_Lire_Config_NAV,
*  qui appelle XZAO161_Ajouter_NAV avec le nom de la base sur la quelle XZAO161_Ajouter_NAV 
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cmvf_FctUti_MSE_XZAO160_161(XZAOT_AjoutNAV   va_NAV_in, XDY_Entier     va_Resultat_out) 
/*
* ARGUMENTS EN ENTREE :  va_NAV_in contient la NAV a rajouter
*            va_Resultat_out contient le numero de ligne du fichier qui est en cours 
*            de traitement.
*                 
* ARGUMENTS EN SORTIE :  aucun
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
*
* CONDITION D'UTILISATION
*
* FONCTION 
*    Doit appeler XZAO161_Ajouter_NAV
*     
------------------------------------------------------*/
{
    static char *version     = "$Id: cgcd_mSGBDeq.c,v 1.33 2021/05/10 08:52:03 pc2dpdy Exp $ : cmvf_FctUti_MSE_XZAO160_161";
    char    vl_NomBase[4]    ;
    int     vl_retcode     = XDC_OK;
    int     vl_retcode_out     = XDC_OK;
    char      pl_Msg[500];            /* contient le mesg a tracer*/
    char      pl_Msg2[500];            /* contient modification si vl_Resultat_out XDC_AJOUT,...*/
    char      pl_NomProcs[80] ;
    
    XDY_ResConf vl_Resultat_out;
    
    XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MSE_XZAO160_161");
    
    strcpy( vl_NomBase,CGCDG_Base_Courante);    
    if ((vl_retcode =XZAO161_Ajouter_NAV(vl_NomBase,va_NAV_in, &vl_Resultat_out))!=XDC_OK)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO160_161 a echoue sur ligne %ld",(long)va_Resultat_out);
        vl_retcode_out = XDC_NOK;    
    }
    else
    /*A
    ** Test sur parametres de sortie de XZAO161_Ajouter_NAV sur coherence base
    ** 4 valeurs de status d execution de la sp dans vl_Resultat_out
    ** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
    */
    {
        if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
        {
            strcpy(pl_NomProcs,"XZAO161_Ajouter_NAV");
            XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld",pl_NomProcs,pl_Msg2,(long)va_Resultat_out);
            
            sprintf(pl_Msg,"%s %s sur la ligne %ld",XDF_CONF_NAV,pl_Msg2,(long)va_Resultat_out);
            cuti_BilanConf_Trace(pl_Msg);    /* Notification dans Bilan config     */
            
        }
        else
        {
            XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO160_161 : cmvf_Trace_TYPE_Result a echoue");
            vl_retcode_out = XDC_NOK;
        }
    
        vl_retcode = vl_retcode_out;
    }    
    XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MSE_XZAO160_161 : sortie avec %d",vl_retcode_out);
    return(vl_retcode_out);
}

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*    cmvf_FctUti_MSE_XZAO169_170 : Fct utilisateur passee a xzao169_Lire_Config_SeqAut_Tunnel,
*  qui appelle XZAO170_Ajouter_SeqAut_Tunnel avec le nom de la base sur la quelle XZAO170_Ajouter_SeqAut_Tunnel 
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cmvf_FctUti_MSE_XZAO169_170(XZAOT_ConfSeqAutTun     va_Seq_Aut_Tube_in, XDY_Entier     va_Resultat_out) 
/*
* ARGUMENTS EN ENTREE :  va_Seq_Aut_Tube_in contient une sequence d autoroute tunnel a rajouter
*            va_Resultat_out contient le numero de ligne du fichier qui est en cours 
*            de traitement.
*                 
* ARGUMENTS EN SORTIE :  aucun
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
*
* CONDITION D'UTILISATION
*
* FONCTION 
*    Doit appeler XZAO170_Ajouter_SeqAut_Tunnel
*     
------------------------------------------------------*/
{
    static char *version     = "$Id: cgcd_mSGBDeq.c,v 1.33 2021/05/10 08:52:03 pc2dpdy Exp $ : cmvf_FctUti_MSE_XZAO169_170";
    char    vl_NomBase[4]    ;
    int     vl_retcode     = XDC_OK;
    int     vl_retcode_out     = XDC_OK;
    char      pl_Msg[500];            /* contient le mesg a tracer*/
    char      pl_Msg2[500];            /* contient modification si vl_Resultat_out XDC_AJOUT,...*/
    char      pl_NomProcs[80] ;
    
    XDY_ResConf vl_Resultat_out;
    
    XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MSE_XZAO169_170");
    
    strcpy( vl_NomBase,CGCDG_Base_Courante);    
    if ((vl_retcode =XZAO170_Ajouter_SeqAut_Tunnel(vl_NomBase,va_Seq_Aut_Tube_in, &vl_Resultat_out))!=XDC_OK)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO169_170 a echoue sur ligne %ld",(long)va_Resultat_out);
        vl_retcode_out = XDC_NOK;    
    }
    else
    /*A
    ** Test sur parametres de sortie de XZAO170_Ajouter_SeqAut_Tunnel sur coherence base
    ** 4 valeurs de status d execution de la sp dans vl_Resultat_out
    ** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
    */
    {
        if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
        {
            strcpy(pl_NomProcs,"XZAO170_Ajouter_SeqAut_Tunnel");
            XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld",pl_NomProcs,pl_Msg2,(long)va_Resultat_out);
            
            sprintf(pl_Msg,"%s %s sur la ligne %ld",XDF_CONF_SEQ_AUT_TUBE,pl_Msg2,(long)va_Resultat_out);
            cuti_BilanConf_Trace(pl_Msg);    /* Notification dans Bilan config     */
            
        }
        else
        {
            XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO169_170 : cmvf_Trace_TYPE_Result a echoue");
            vl_retcode_out = XDC_NOK;
        }
    
        vl_retcode = vl_retcode_out;
    }    
    XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MSE_XZAO169_170 : sortie avec %d",vl_retcode_out);
    return(vl_retcode_out);
}
/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*    cmvf_FctUti_MSE_XZAO171_172 : Fct utilisateur passee a xzao171_Lire_Config_SeqAut_Echangeur,
*  qui appelle XZAO172_Ajouter_SeqAut_Echangeur avec le nom de la base sur la quelle XZAO172_Ajouter_SeqAut_Echangeur 
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cmvf_FctUti_MSE_XZAO171_172(XZAOT_ConfSeqAutEch     va_SeqAut_Echangeur_in, XDY_Entier     va_Resultat_out) 
/*
* ARGUMENTS EN ENTREE :  va_SeqAut_Echangeur_in contient la configue d une sequence d autoroute tunnel a rajouter
*            va_Resultat_out contient le numero de ligne du fichier qui est en cours 
*            de traitement.
*                 
* ARGUMENTS EN SORTIE :  aucun
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
*
* CONDITION D'UTILISATION
*
* FONCTION 
*    Doit appeler XZAO172_Ajouter_SeqAut_Echangeur
*     
------------------------------------------------------*/
{
    static char *version     = "$Id: cgcd_mSGBDeq.c,v 1.33 2021/05/10 08:52:03 pc2dpdy Exp $ : cmvf_FctUti_MSE_XZAO171_172";
    char    vl_NomBase[4]    ;
    int     vl_retcode     = XDC_OK;
    int     vl_retcode_out     = XDC_OK;
    char      pl_Msg[500];            /* contient le mesg a tracer*/
    char      pl_Msg2[500];            /* contient modification si vl_Resultat_out XDC_AJOUT,...*/
    char      pl_NomProcs[80] ;
    
    XDY_ResConf vl_Resultat_out;
    
    XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MSE_XZAO171_172");
    
    strcpy( vl_NomBase,CGCDG_Base_Courante);    
    if ((vl_retcode =XZAO172_Ajouter_SeqAut_Echangeur(vl_NomBase,va_SeqAut_Echangeur_in, &vl_Resultat_out))!=XDC_OK)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO171_172 a echoue sur ligne %ld",(long)va_Resultat_out);
        vl_retcode_out = XDC_NOK;    
    }
    else
    /*A
    ** Test sur parametres de sortie de XZAO172_Ajouter_SeqAut_Echangeur sur coherence base
    ** 4 valeurs de status d execution de la sp dans vl_Resultat_out
    ** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
    */
    {
        if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
        {
            strcpy(pl_NomProcs,"XZAO172_Ajouter_SeqAut_Echangeur");
            XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld",pl_NomProcs,pl_Msg2,(long)va_Resultat_out);
            
            sprintf(pl_Msg,"%s %s sur la ligne %ld",XDF_CONF_SEQ_AUT_ECHANGEUR,pl_Msg2,(long)va_Resultat_out);
            cuti_BilanConf_Trace(pl_Msg);    /* Notification dans Bilan config     */
            
        }
        else
        {
            XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO171_172 : cmvf_Trace_TYPE_Result a echoue");
            vl_retcode_out = XDC_NOK;
        }
    
        vl_retcode = vl_retcode_out;
    }    
    XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MSE_XZAO171_172 : sortie avec %d",vl_retcode_out);
    return(vl_retcode_out);
}
/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*    cmvf_FctUti_MSE_XZAO179_180 : Fct utilisateur passee a xzao179_Lire_Config_Capteurs_Tunnel,
*  qui appelle XZAO180_Ajouter_Capteur_Tunnel avec le nom de la base sur la quelle XZAO180_Ajouter_Capteur_Tunnel 
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cmvf_FctUti_MSE_XZAO179_180(XZAOT_ConfCaptTun       va_CapteurTunnel_in, XDY_Entier     va_Resultat_out) 
/*
* ARGUMENTS EN ENTREE :  va_CapteurTunnel_in contient le capteur tunnel a rajouter
*            va_Resultat_out contient le numero de ligne du fichier qui est en cours 
*            de traitement.
*                 
* ARGUMENTS EN SORTIE :  aucun
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
*
* CONDITION D'UTILISATION
*
* FONCTION 
*    Doit appeler XZAO180_Ajouter_Capteur_Tunnel
*     
------------------------------------------------------*/
{
    static char *version     = "$Id: cgcd_mSGBDeq.c,v 1.33 2021/05/10 08:52:03 pc2dpdy Exp $ : cmvf_FctUti_MSE_XZAO179_180";
    char    vl_NomBase[4]    ;
    int     vl_retcode     = XDC_OK;
    int     vl_retcode_out     = XDC_OK;
    char      pl_Msg[500];            /* contient le mesg a tracer*/
    char      pl_Msg2[500];            /* contient modification si vl_Resultat_out XDC_AJOUT,...*/
    char      pl_NomProcs[80] ;
    
    XDY_ResConf vl_Resultat_out;
    
    XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MSE_XZAO179_180");
    
    strcpy( vl_NomBase,CGCDG_Base_Courante);    
    if ((vl_retcode =XZAO180_Ajouter_Capteur_Tunnel(vl_NomBase,va_CapteurTunnel_in, &vl_Resultat_out))!=XDC_OK)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO179_180 a echoue sur ligne %ld",(long)va_Resultat_out);
        vl_retcode_out = XDC_NOK;    
    }
    else
    /*A
    ** Test sur parametres de sortie de XZAO180_Ajouter_Capteur_Tunnel sur coherence base
    ** 4 valeurs de status d execution de la sp dans vl_Resultat_out
    ** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
    */
    {
        if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
        {
            strcpy(pl_NomProcs,"XZAO180_Ajouter_Capteur_Tunnel");
            XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld",pl_NomProcs,pl_Msg2,(long)va_Resultat_out);
            
            sprintf(pl_Msg,"%s %s sur la ligne %ld",XDF_CONF_CAPT_TUNNEL,pl_Msg2,(long)va_Resultat_out);
            cuti_BilanConf_Trace(pl_Msg);    /* Notification dans Bilan config     */
            
        }
        else
        {
            XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO179_180 : cmvf_Trace_TYPE_Result a echoue");
            vl_retcode_out = XDC_NOK;
        }
    
        vl_retcode = vl_retcode_out;
    }    
    XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MSE_XZAO179_180 : sortie avec %d",vl_retcode_out);
    return(vl_retcode_out);
}
/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*    cmvf_FctUti_MSE_XZAO181_182 : Fct utilisateur passee a xzao181_Lire_Config_FAV,
*  qui appelle XZAO182_Ajouter_FAV avec le nom de la base sur la quelle XZAO182_Ajouter_FAV 
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cmvf_FctUti_MSE_XZAO181_182(XZAOT_ConfFAV           va_FAV_in, XDY_Entier     va_Resultat_out) 
/*
* ARGUMENTS EN ENTREE :  va_FAV_in contient la FAV a rajouter
*            va_Resultat_out contient le numero de ligne du fichier qui est en cours 
*            de traitement.
*                 
* ARGUMENTS EN SORTIE :  aucun
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
*
* CONDITION D'UTILISATION
*
* FONCTION 
*    Doit appeler XZAO182_Ajouter_FAV
*     
------------------------------------------------------*/
{
    static char *version     = "$Id: cgcd_mSGBDeq.c,v 1.33 2021/05/10 08:52:03 pc2dpdy Exp $ : cmvf_FctUti_MSE_XZAO181_182";
    char    vl_NomBase[4]    ;
    int     vl_retcode     = XDC_OK;
    int     vl_retcode_out     = XDC_OK;
    char      pl_Msg[500];            /* contient le mesg a tracer*/
    char      pl_Msg2[500];            /* contient modification si vl_Resultat_out XDC_AJOUT,...*/
    char      pl_NomProcs[80] ;
    
    XDY_ResConf vl_Resultat_out;
    
    XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MSE_XZAO181_182");
    
    strcpy( vl_NomBase,CGCDG_Base_Courante);    
    if ((vl_retcode =XZAO182_Ajouter_FAV(vl_NomBase,va_FAV_in, &vl_Resultat_out))!=XDC_OK)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO181_182 a echoue sur ligne %ld",(long)va_Resultat_out);
        vl_retcode_out = XDC_NOK;    
    }
    else
    /*A
    ** Test sur parametres de sortie de XZAO182_Ajouter_FAV sur coherence base
    ** 4 valeurs de status d execution de la sp dans vl_Resultat_out
    ** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
    */
    {
        if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
        {
            strcpy(pl_NomProcs,"XZAO182_Ajouter_FAV");
            XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld",pl_NomProcs,pl_Msg2,(long)va_Resultat_out);
            
            sprintf(pl_Msg,"%s %s sur la ligne %ld",XDF_CONF_FAV,pl_Msg2,(long)va_Resultat_out);
            cuti_BilanConf_Trace(pl_Msg);    /* Notification dans Bilan config     */
            
        }
        else
        {
            XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO181_182 : cmvf_Trace_TYPE_Result a echoue");
            vl_retcode_out = XDC_NOK;
        }
    
        vl_retcode = vl_retcode_out;
    }    
    XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MSE_XZAO181_182 : sortie avec %d",vl_retcode_out);
    return(vl_retcode_out);
}

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*    cmvf_FctUti_MSE_XZAO183_184 : Fct utilisateur passee a xzao183_Lire_Config_PnGTC,
*  qui appelle XZAO184_Ajouter_PnGTC avec le nom de la base sur la quelle XZAO184_Ajouter_PnGTC 
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cmvf_FctUti_MSE_XZAO183_184(XZAOT_ConfPnGTC         va_PnGTC_in, XDY_Entier     va_Resultat_out) 
/*
* ARGUMENTS EN ENTREE :  va_PnGTC_in contient la PnGTC a rajouter
*            va_Resultat_out contient le numero de ligne du fichier qui est en cours 
*            de traitement.
*                 
* ARGUMENTS EN SORTIE :  aucun
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
*
* CONDITION D'UTILISATION
*
* FONCTION 
*    Doit appeler XZAO184_Ajouter_PnGTC
*     
------------------------------------------------------*/
{
    static char *version     = "$Id: cgcd_mSGBDeq.c,v 1.33 2021/05/10 08:52:03 pc2dpdy Exp $ : cmvf_FctUti_MSE_XZAO183_184";
    char    vl_NomBase[4]    ;
    int     vl_retcode     = XDC_OK;
    int     vl_retcode_out     = XDC_OK;
    char      pl_Msg[500];            /* contient le mesg a tracer*/
    char      pl_Msg2[500];            /* contient modification si vl_Resultat_out XDC_AJOUT,...*/
    char      pl_NomProcs[80] ;
    
    XDY_ResConf vl_Resultat_out;
    
    XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MSE_XZAO183_184");
    
    strcpy( vl_NomBase,CGCDG_Base_Courante);    
    if ((vl_retcode =XZAO184_Ajouter_PnGTC(vl_NomBase,va_PnGTC_in, &vl_Resultat_out))!=XDC_OK)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO183_184 a echoue sur ligne %ld",(long)va_Resultat_out);
        vl_retcode_out = XDC_NOK;    
    }
    else
    /*A
    ** Test sur parametres de sortie de XZAO184_Ajouter_PnGTC sur coherence base
    ** 4 valeurs de status d execution de la sp dans vl_Resultat_out
    ** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
    */
    {
        if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
        {
            strcpy(pl_NomProcs,"XZAO184_Ajouter_PnGTC");
            XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld",pl_NomProcs,pl_Msg2,(long)va_Resultat_out);
            
            sprintf(pl_Msg,"%s %s sur la ligne %ld",XDF_CONF_PN_GTC,pl_Msg2,(long)va_Resultat_out);
            cuti_BilanConf_Trace(pl_Msg);    /* Notification dans Bilan config     */
            
        }
        else
        {
            XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO183_184 : cmvf_Trace_TYPE_Result a echoue");
            vl_retcode_out = XDC_NOK;
        }
    
        vl_retcode = vl_retcode_out;
    }    
    XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MSE_XZAO183_184 : sortie avec %d",vl_retcode_out);
    return(vl_retcode_out);
}

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*    cmvf_FctUti_MSE_XZAO191_190 : Fct utilisateur passee a xzao191_Lire_Config_Fax,
*  qui appelle XZAO184_Ajouter_PnGTC avec le nom de la base sur la quelle XZAO190_Ajouter_Fax 
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cmvf_FctUti_MSE_XZAO191_190(XZAOT_ConfFax         va_Fax_in, XDY_Entier     va_Resultat_out) 
/*
* ARGUMENTS EN ENTREE :  va_Fax_in contient le Fax a rajouter
*            va_Resultat_out contient le numero de ligne du fichier qui est en cours 
*            de traitement.
*                 
* ARGUMENTS EN SORTIE :  aucun
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
*
* CONDITION D'UTILISATION
*
* FONCTION 
*    Doit appeler XZAO190_Ajouter_Fax
*     
------------------------------------------------------*/
{
    static char *version     = "$Id: cgcd_mSGBDeq.c,v 1.33 2021/05/10 08:52:03 pc2dpdy Exp $ : cmvf_FctUti_MSE_XZAO191_190";
    char    vl_NomBase[4]    ;
    int     vl_retcode     = XDC_OK;
    int     vl_retcode_out     = XDC_OK;
    char      pl_Msg[500];            /* contient le mesg a tracer*/
    char      pl_Msg2[500];            /* contient modification si vl_Resultat_out XDC_AJOUT,...*/
    char      pl_NomProcs[80] ;
    
    XDY_ResConf vl_Resultat_out;
    
    XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MSE_XZAO191_190");
    
    strcpy( vl_NomBase,CGCDG_Base_Courante);    
    if ((vl_retcode =XZAO190_Ajouter_Fax(vl_NomBase,va_Fax_in, &vl_Resultat_out))!=XDC_OK)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO191_190 a echoue sur ligne %ld",(long)va_Resultat_out);
        vl_retcode_out = XDC_NOK;    
    }
    else
    /*A
    ** Test sur parametres de sortie de XZAO190_Ajouter_Fax sur coherence base
    ** 4 valeurs de status d execution de la sp dans vl_Resultat_out
    ** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
    */
    {
        if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
        {
            strcpy(pl_NomProcs,"XZAO190_Ajouter_Fax");
            XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld",pl_NomProcs,pl_Msg2,(long)va_Resultat_out);
            
            sprintf(pl_Msg,"%s %s sur la ligne %ld",XDF_CONF_FAX,pl_Msg2,(long)va_Resultat_out);
            cuti_BilanConf_Trace(pl_Msg);    /* Notification dans Bilan config     */
            
        }
        else
        {
            XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO191_190 : cmvf_Trace_TYPE_Result a echoue");
            vl_retcode_out = XDC_NOK;
        }
    
        vl_retcode = vl_retcode_out;
    }    
    XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MSE_XZAO191_190 : sortie avec %d",vl_retcode_out);
    return(vl_retcode_out);
}
/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*    cmvf_FctUti_MSE_XZAO197_187 : Fct utilisateur passee a xzao197_Lire_Config_Eqt_Generique_LCR,
*  qui appelle XZAO187_Ajouter_Eqt_generique_LCR avec le nom de la base sur la quelle XZAO187_Ajouter_Eqt_generique_LCR 
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cmvf_FctUti_MSE_XZAO197_187(XZAOT_ConfEqtGenLCR       va_EqtGeneriqueLCR_in, XDY_Entier     va_Resultat_out) 
/*
* ARGUMENTS EN ENTREE :  va_EqtGeneriqueLCR_in contient l'Åquipement generique LCR a rajouter
*            va_Resultat_out contient le numero de ligne du fichier qui est en cours 
*            de traitement.
*                 
* ARGUMENTS EN SORTIE :  aucun
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
*
* CONDITION D'UTILISATION
*
* FONCTION 
*    Doit appeler XZAO187_Ajouter_Eqt_generique_LCR
*     
------------------------------------------------------*/
{
    static char *version     = "$Id: cgcd_mSGBDeq.c,v 1.33 2021/05/10 08:52:03 pc2dpdy Exp $ : cmvf_FctUti_MSE_XZAO197_187";
    char    vl_NomBase[4]    ;
    int     vl_retcode     = XDC_OK;
    int     vl_retcode_out     = XDC_OK;
    char      pl_Msg[500];            /* contient le mesg a tracer*/
    char      pl_Msg2[500];            /* contient modification si vl_Resultat_out XDC_AJOUT,...*/
    char      pl_NomProcs[80] ;
    
    XDY_ResConf vl_Resultat_out;
    
    XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MSE_XZAO197_187");
    
    strcpy( vl_NomBase,CGCDG_Base_Courante);    
    if ((vl_retcode =XZAO187_Ajouter_Eqt_generique_LCR(vl_NomBase,va_EqtGeneriqueLCR_in, &vl_Resultat_out))!=XDC_OK)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO197_187 a echoue sur ligne %ld",(long)va_Resultat_out);
        vl_retcode_out = XDC_NOK;    
    }
    else
    /*A
    ** Test sur parametres de sortie de XZAO187_Ajouter_Eqt_generique_LCR sur coherence base
    ** 4 valeurs de status d execution de la sp dans vl_Resultat_out
    ** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
    */
    {
        if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
        {
            strcpy(pl_NomProcs,"XZAO187_Ajouter_Eqt_generique_LCR");
            XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld",pl_NomProcs,pl_Msg2,(long)va_Resultat_out);
            
            sprintf(pl_Msg,"%s %s sur la ligne %ld",XDF_CONF_EQT_GEN_LCR,pl_Msg2,(long)va_Resultat_out);
            cuti_BilanConf_Trace(pl_Msg);    /* Notification dans Bilan config     */
            
        }
        else
        {
            XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO197_187 : cmvf_Trace_TYPE_Result a echoue");
            vl_retcode_out = XDC_NOK;
        }
    
        vl_retcode = vl_retcode_out;
    }    
    XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MSE_XZAO197_187 : sortie avec %d",vl_retcode_out);
    return(vl_retcode_out);
}
/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*    cmvf_FctUti_MSE_XZAO403_404 : Fct utilisateur passee a xzao404_Lire_Config_Eqt_ZDP
*  qui appelle XZAO403_Ajouter_Eqt_ZDP avec le nom de la base sur la quelle XZAO403_Ajouter_Eqt_ZDP 
*  va avoir une action.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cmvf_FctUti_MSE_XZAO407_408(XZAOT_ConfEqtUGTP       va_Eqt_UGTP_in, XDY_Entier     va_Resultat_out) 
/*
* ARGUMENTS EN ENTREE :  va_Eqt_UGTP_in contient l'Åquipement UGTP (TDP) a rajouter
*            va_Resultat_out contient le numero de ligne du fichier qui est en cours 
*            de traitement.
*                 
* ARGUMENTS EN SORTIE :  aucun
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
*
* CONDITION D'UTILISATION
*
* FONCTION 
*    Doit appeler XZAO407_Ajouter_Eqt_TDP
*     
------------------------------------------------------*/
{
    static char *version     = "$Id: cgcd_mSGBDeq.c,v 1.33 2021/05/10 08:52:03 pc2dpdy Exp $ : cmvf_FctUti_MSE_XZAO407_408";
    char    vl_NomBase[4]    ;
    int     vl_retcode     = XDC_OK;
    int     vl_retcode_out     = XDC_OK;
    char      pl_Msg[500];            /* contient le mesg a tracer*/
    char      pl_Msg2[500];            /* contient modification si vl_Resultat_out XDC_AJOUT,...*/
    char      pl_NomProcs[80] ;
    
    XDY_ResConf vl_Resultat_out;
    
    XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MSE_XZAO407_408");
    
    strcpy( vl_NomBase,CGCDG_Base_Courante);    
    if ((vl_retcode =XZAO407_Ajouter_Eqt_TDP(vl_NomBase,va_Eqt_UGTP_in, &vl_Resultat_out))!=XDC_OK)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO407_408 a echoue sur ligne %ld",(long)va_Resultat_out);
        vl_retcode_out = XDC_NOK;    
    }
    else
    /*A
    ** Test sur parametres de sortie de XZAO407_Ajouter_Eqt_TDP sur coherence base
    ** 4 valeurs de status d execution de la sp dans vl_Resultat_out
    ** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
    */
    {
        if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
        {
            strcpy(pl_NomProcs,"XZAO407_Ajouter_Eqt_TDP");
            XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld",pl_NomProcs,pl_Msg2,(long)va_Resultat_out);
            
            sprintf(pl_Msg,"%s %s sur la ligne %ld",XDF_CONF_EQT_TDP,pl_Msg2,(long)va_Resultat_out);
            cuti_BilanConf_Trace(pl_Msg);    /* Notification dans Bilan config     */
            
        }
        else
        {
            XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO407_408 : cmvf_Trace_TYPE_Result a echoue");
            vl_retcode_out = XDC_NOK;
        }
    
        vl_retcode = vl_retcode_out;
    }    
    XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MSE_XZAO407_408 : sortie avec %d",vl_retcode_out);
    return(vl_retcode_out);
}
/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*    cmvf_FctUti_MSE_XZAO403_404 : Fct utilisateur passee a xzao404_Lire_Config_Eqt_ZDP
*  qui appelle XZAO403_Ajouter_Eqt_ZDP avec le nom de la base sur la quelle XZAO403_Ajouter_Eqt_ZDP 
*  va avoir une action.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cmvf_FctUti_MSE_XZAO403_404(XZAOT_ConfEqtZDP       va_Eqt_ZDP_in, XDY_Entier     va_Resultat_out) 
/*
* ARGUMENTS EN ENTREE :  va_Eqt_ZDP_in contient l'Åquipement UGTP (TDP) a rajouter
*            va_Resultat_out contient le numero de ligne du fichier qui est en cours 
*            de traitement.
*                 
* ARGUMENTS EN SORTIE :  aucun
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
*
* CONDITION D'UTILISATION
*
* FONCTION 
*    Doit appeler XZAO403_Ajouter_Eqt_ZDP
*     
------------------------------------------------------*/
{
    static char *version     = "$Id: cgcd_mSGBDeq.c,v 1.33 2021/05/10 08:52:03 pc2dpdy Exp $ : cmvf_FctUti_MSE_XZAO403_404";
    char    vl_NomBase[4]    ;
    int     vl_retcode     = XDC_OK;
    int     vl_retcode_out     = XDC_OK;
    char      pl_Msg[500];            /* contient le mesg a tracer*/
    char      pl_Msg2[500];            /* contient modification si vl_Resultat_out XDC_AJOUT,...*/
    char      pl_NomProcs[80] ;
    
    XDY_ResConf vl_Resultat_out;
    
    XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MSE_XZAO403_404");
    
    strcpy( vl_NomBase,CGCDG_Base_Courante);    
    if ((vl_retcode =XZAO403_Ajouter_Eqt_ZDP(vl_NomBase,va_Eqt_ZDP_in, &vl_Resultat_out))!=XDC_OK)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO403_404 a echoue sur ligne %ld",(long)va_Resultat_out);
        vl_retcode_out = XDC_NOK;    
    }
    else
    /*A
    ** Test sur parametres de sortie de XZAO403_Ajouter_Eqt_ZDP sur coherence base
    ** 4 valeurs de status d execution de la sp dans vl_Resultat_out
    ** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
    */
    {
        if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
        {
            strcpy(pl_NomProcs,"XZAO403_Ajouter_Eqt_ZDP");
            XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld",pl_NomProcs,pl_Msg2,(long)va_Resultat_out);
            
            sprintf(pl_Msg,"%s %s sur la ligne %ld",XDF_CONF_EQT_ZDP,pl_Msg2,(long)va_Resultat_out);
            cuti_BilanConf_Trace(pl_Msg);    /* Notification dans Bilan config     */
            
        }
        else
        {
            XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO403_404 : cmvf_Trace_TYPE_Result a echoue");
            vl_retcode_out = XDC_NOK;
        }
    
        vl_retcode = vl_retcode_out;
    }    
    XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MSE_XZAO403_404 : sortie avec %d",vl_retcode_out);
    return(vl_retcode_out);
}




/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*    cmvf_FctUti_MSE_XZAO417_416 : Fct utilisateur passee a xzao417_Lire_Config_Zon_PMV
*  qui appelle XZAO416_Ajouter_Zon_PMV avec le nom de la base sur la quelle XZAO416_Ajouter_Zon_PMV 
*  va avoir une action.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cmvf_FctUti_MSE_XZAO417_416(    char         *va_autoroute_pmv,
                    XDY_Sens     va_sens_pmv,
                    XDY_PR        va_pr_pmv,
                    char        *va_site_pmv,
                    char        *va_autoroute_zone,
                    XDY_Sens    va_sens_zone,
                    XDY_PR        va_debut_zone,
                    XDY_PR        va_fin_zone,
                    char        *va_direction_zone,
                    XDY_Booleen    va_inhibition_zone,
                    XDY_PR        va_pr_1_pmv_amont,
                    XDY_Sens    va_sens_1_pmv_amont,
                    char        *va_autoroute_1_pmv_amont,
                    int        va_distance_1_pmv_amont,
                    XDY_PR        va_pr_1_pmv_amont_bis,
                    XDY_Sens    va_sens_1_pmv_amont_bis,
                    char        *va_autoroute_1_pmv_amont_bis,
                    int        va_distance_1_pmv_amont_bis,
                    XDY_PR        va_pr_1_pmv_amont_ter,
                    XDY_Sens    va_sens_1_pmv_amont_ter,
                    char        *va_autoroute_1_pmv_amont_ter,
                    int        va_distance_1_pmv_amont_ter,
                    XDY_Entier         va_Resultat_out) 
/*
* ARGUMENTS EN ENTREE :  infos de la zone d'influence PMV
*            va_Resultat_out contient le numero de ligne du fichier qui est en cours 
*            de traitement.
*                 
* ARGUMENTS EN SORTIE :  aucun
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
*
* CONDITION D'UTILISATION
*
* FONCTION 
*    Doit appeler XZAO403_Ajouter_Eqt_ZDP
*     
------------------------------------------------------*/
{
    static char *version     = "$Id: cgcd_mSGBDeq.c,v 1.33 2021/05/10 08:52:03 pc2dpdy Exp $ : cmvf_FctUti_MSE_XZAO417_416";
    char    vl_NomBase[4]    ;
    int     vl_retcode     = XDC_OK;
    int     vl_retcode_out     = XDC_OK;
    char      pl_Msg[500];            /* contient le mesg a tracer*/
    char      pl_Msg2[500];            /* contient modification si vl_Resultat_out XDC_AJOUT,...*/
    char      pl_NomProcs[80] ;
    
    XDY_ResConf vl_Resultat_out;
    
    XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MSE_XZAO417_416");
    
    strcpy( vl_NomBase,CGCDG_Base_Courante);    
    if ((vl_retcode =XZAO416_Ajouter_Zon_PMV(    vl_NomBase,
                            va_autoroute_pmv,
                            va_sens_pmv,
                            va_pr_pmv,
                            va_site_pmv,
                            va_autoroute_zone,
                            va_sens_zone,
                            va_debut_zone,
                            va_fin_zone,
                            va_direction_zone,
                            va_inhibition_zone,
                            va_pr_1_pmv_amont,
                            va_sens_1_pmv_amont,
                            va_autoroute_1_pmv_amont,
                            va_distance_1_pmv_amont,
                            va_pr_1_pmv_amont_bis,
                            va_sens_1_pmv_amont_bis,
                            va_autoroute_1_pmv_amont_bis,
                            va_distance_1_pmv_amont_bis,
                            va_pr_1_pmv_amont_ter,
                            va_sens_1_pmv_amont_ter,
                            va_autoroute_1_pmv_amont_ter,
                            va_distance_1_pmv_amont_ter,
                            &vl_Resultat_out))!=XDC_OK)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO417_416 a echoue sur ligne %ld",(long)va_Resultat_out);
        vl_retcode_out = XDC_NOK;    
    }
    else
    /*A
    ** Test sur parametres de sortie de XZAO416_Ajouter_Zon_PMV sur coherence base
    ** 4 valeurs de status d execution de la sp dans vl_Resultat_out
    ** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
    */
    {
        if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
        {
            strcpy(pl_NomProcs,"XZAO416_Ajouter_Zon_PMV");
            XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld",pl_NomProcs,pl_Msg2,(long)va_Resultat_out);
            
            sprintf(pl_Msg,"%s %s sur la ligne %ld",XDF_CONF_ZON_PMV,pl_Msg2,(long)va_Resultat_out);
            cuti_BilanConf_Trace(pl_Msg);    /* Notification dans Bilan config     */
            
        }
        else
        {
            XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO417_416 : cmvf_Trace_TYPE_Result a echoue");
            vl_retcode_out = XDC_NOK;
        }
    
        vl_retcode = vl_retcode_out;
    }    
    XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MSE_XZAO417_416 : sortie avec %d",vl_retcode_out);
    return(vl_retcode_out);
}


/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*    cmvf_FctUti_MSE_XZAO421_420 : Fct utilisateur passee a xzao421_Lire_Config_PAL
*  qui appelle XZAO420_Ajouter_PAL avec le nom de la base sur laquelle XZAO420_Ajouter_PAL 
*  va avoir une action.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cmvf_FctUti_MSE_XZAO421_420(XZAOT_AjoutPAL   va_PAL_in, XDY_Entier     va_Resultat_out) 
/*
* ARGUMENTS EN ENTREE :  va_PAL_in contient le PAL a rajouter
*            va_Resultat_out contient le numero de ligne du fichier qui est en cours 
*            de traitement.
*                 
* ARGUMENTS EN SORTIE :  aucun
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
*
* CONDITION D'UTILISATION
*
* FONCTION 
*    Doit appeler XZAO421_Ajouter_PAL
*     
------------------------------------------------------*/
{
    static char *version     = "$Id: cgcd_mSGBDeq.c,v 1.33 2021/05/10 08:52:03 pc2dpdy Exp $ : cmvf_FctUti_MSE_XZAO421_420";
    char    vl_NomBase[4]    ;
    int     vl_retcode     = XDC_OK;
    int     vl_retcode_out     = XDC_OK;
    char      pl_Msg[500];            /* contient le mesg a tracer*/
    char      pl_Msg2[500];            /* contient modification si vl_Resultat_out XDC_AJOUT,...*/
    char      pl_NomProcs[80] ;
    
    XDY_ResConf vl_Resultat_out;
    
    XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MSE_XZAO160_161");
    
    strcpy( vl_NomBase,CGCDG_Base_Courante);    
    if ((vl_retcode =XZAO421_Ajouter_PAL(vl_NomBase,va_PAL_in, &vl_Resultat_out))!=XDC_OK)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO421_420 a echoue sur ligne %ld",(long)va_Resultat_out);
        vl_retcode_out = XDC_NOK;    
    }
    else
    /*A
    ** Test sur parametres de sortie de XZAO421_Ajouter_PAL sur coherence base
    ** 4 valeurs de status d execution de la sp dans vl_Resultat_out
    ** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
    */
    {
        if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
        {
            strcpy(pl_NomProcs,"XZAO421_Ajouter_PAL");
            XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld",pl_NomProcs,pl_Msg2,(long)va_Resultat_out);
            
            sprintf(pl_Msg,"%s %s sur la ligne %ld",XDF_CONF_PAL,pl_Msg2,(long)va_Resultat_out);
            cuti_BilanConf_Trace(pl_Msg);    /* Notification dans Bilan config     */
            
        }
        else
        {
            XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO421_420 : cmvf_Trace_TYPE_Result a echoue");
            vl_retcode_out = XDC_NOK;
        }
    
        vl_retcode = vl_retcode_out;
    }    
    XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MSE_XZAO421_420 : sortie avec %d",vl_retcode_out);
    return(vl_retcode_out);
}





/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*    cmvf_FctUti_MSE_XZAO422_720: Fct utilisateur passee a xzao103_Lire_Config_PMV_Picto,
*  qui appelle XZAO720_Ajouter_PMVA avec le nom de la base sur la quelle XZAO720_Ajouter_PMVA va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cmvf_FctUti_MSE_XZAO422_720 ( XZAOT_ConfPMVA           va_PMV_in, XDY_Entier     va_Resultat_out) 
/*
* ARGUMENTS EN ENTREE :  va_PMV_in contient le PMVA a rajouter
*            va_Resultat_out contient le numero de ligne du fichier qui est en cours 
*            de traitement.
*                 
* ARGUMENTS EN SORTIE :  aucun
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
*
* CONDITION D'UTILISATION
*
* FONCTION 
*    Doit appeler XZAO720_Ajouter_PMVA
*     
------------------------------------------------------*/
{
    static char *version     = "$Id: cgcd_mSGBDeq.c,v 1.33 2021/05/10 08:52:03 pc2dpdy Exp $ $Revision: 1.33 $ $Date: 2021/05/10 08:52:03 $: cmvf_FctUti_MSE_XZAO422_720 ";
    char    vl_NomBase[4]    ;
    int     vl_retcode     = XDC_OK;
    int     vl_retcode_out     = XDC_OK;
    char      pl_Msg[500];            /* contient le mesg a tracer*/
    char      pl_Msg2[500];            /* contient modification si vl_Resultat_out XDC_AJOUT,...*/
    char      pl_NomProcs[80] ;
    
    XDY_ResConf vl_Resultat_out;
    
    XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MSE_XZAO422_720 ");
    XZST_03EcritureTrace(XZSTC_WARNING,"BASE(%s) nom(%s) auto(%s) PR(%d) sens(%d) serveur(%s) liaison(%d) rgs(%s) adresse(%s) port (%d) idLCR (%s)",
                CGCDG_Base_Courante,
                va_PMV_in.NomPMVA,
                va_PMV_in.Autoroute,
                va_PMV_in.PR,
                va_PMV_in.Sens,
                va_PMV_in.NomServeur,
                va_PMV_in.LiaisonPMVA,
                va_PMV_in.RgsPMVA,
		va_PMV_in.AdresseIP,
		va_PMV_in.Port,
		va_PMV_in.IdLCR);
    
    strcpy( vl_NomBase,CGCDG_Base_Courante);    
    if ((vl_retcode = XZAO720_Ajouter_PMVA (vl_NomBase,va_PMV_in, &vl_Resultat_out))!=XDC_OK)
    {
        XZST_03EcritureTrace(XZSTC_WARNING," cmvf_FctUti_MSE_XZAO422_720 a echoue sur ligne %ld",(long)va_Resultat_out);
        vl_retcode_out = XDC_NOK;    
    }
    else
    /*A
    ** Test sur parametres de sortie de XZAO720_Ajouter_PMVA sur coherence base
    ** 4 valeurs de status d execution de la sp dans vl_Resultat_out
    ** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
    */
    {
        if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
        {
            strcpy(pl_NomProcs," cmvf_FctUti_MSE_XZAO422_720 ");
            XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld",pl_NomProcs,pl_Msg2,(long)va_Resultat_out);
            
            sprintf(pl_Msg,"%s %s sur la ligne %ld",XDF_CONF_PMVA,pl_Msg2,(long)va_Resultat_out);
            cuti_BilanConf_Trace(pl_Msg);    /* Notification dans Bilan config     */
            
        }
        else
        {
            XZST_03EcritureTrace(XZSTC_WARNING," cmvf_FctUti_MSE_XZAO422_720 : cmvf_Trace_TYPE_Result a echoue");
            vl_retcode_out = XDC_NOK;
        }
    
        vl_retcode = vl_retcode_out;
    }    
    XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MSE_XZAO422_720: sortie avec %d",vl_retcode_out);
    return(vl_retcode_out);
}
/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*    cmvf_FctUti_MSE_XZAO257_PRV: Fct utilisateur passee a xzao255_Lire_Config_PRV,
*  qui appelle XZAO257_Ajouter_PRV avec le nom de la base sur la quelle XZAO257_Ajouter_PRV va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cmvf_FctUti_MSE_XZAO257_PRV ( XZAOT_ConfPRV           va_PRV_in, XDY_Entier     va_Resultat_out) 
/*
* ARGUMENTS EN ENTREE :  va_PRV_in contient le PRV a rajouter
*            va_Resultat_out contient le numero de ligne du fichier qui est en cours 
*            de traitement.
*                 
* ARGUMENTS EN SORTIE :  aucun
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
*
* CONDITION D'UTILISATION
*
* FONCTION 
*    Doit appeler XZAO257_Ajouter_PRV
*     
------------------------------------------------------*/
{
    static char *version     = "@(#)cgcd_mSGBDeq.c	1.17 1.17 05/25/09: cmvf_FctUti_MSE_XZAO257_PRV ";
    char    vl_NomBase[4]    ;
    int     vl_retcode     = XDC_OK;
    int     vl_retcode_out     = XDC_OK;
    char      pl_Msg[500];            /* contient le mesg a tracer*/
    char      pl_Msg2[500];            /* contient modification si vl_Resultat_out XDC_AJOUT,...*/
    char      pl_NomProcs[80] ;
    
    XDY_ResConf vl_Resultat_out;
    
    XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MSE_XZAO257_PRV ");
    XZST_03EcritureTrace(XZSTC_WARNING,"BASE(%s) nom(%s) auto(%s) PR(%d) sens(%d) serveur(%s) liaison(%d) rgs(%s) nom site(%s) vitesse nominale (%d) nom zone (%s) critique (%d) bretelle(%d) rappel nominal (%d)",
                CGCDG_Base_Courante,
                va_PRV_in.NomPRV,
                va_PRV_in.Autoroute,
                va_PRV_in.PR,
                va_PRV_in.Sens,
                va_PRV_in.NomServeur,
                va_PRV_in.LiaisonPRV,
                va_PRV_in.RgsPRV,
		va_PRV_in.NomSite,
		va_PRV_in.VitesseNominale,
		va_PRV_in.NomZone,
		va_PRV_in.Critique,
		va_PRV_in.BretelleEntree,
		va_PRV_in.RappelNominal);
    
    strcpy( vl_NomBase,CGCDG_Base_Courante);    
    if ((vl_retcode = XZAO257_Ajouter_PRV (vl_NomBase,va_PRV_in, &vl_Resultat_out))!=XDC_OK)
    {
        XZST_03EcritureTrace(XZSTC_WARNING," cmvf_FctUti_MSE_XZAO257_PRV a echoue sur ligne %ld",(long)va_Resultat_out);
        vl_retcode_out = XDC_NOK;    
    }
    else
    /*A
    ** Test sur parametres de sortie de XZAO257_Ajouter_PRV sur coherence base
    ** 4 valeurs de status d execution de la sp dans vl_Resultat_out
    ** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
    */
    {
        if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
        {
            strcpy(pl_NomProcs," cmvf_FctUti_MSE_XZAO257_PRV ");
            XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld",pl_NomProcs,pl_Msg2,(long)va_Resultat_out);
            
            sprintf(pl_Msg,"%s %s sur la ligne %ld",XDF_CONF_PRV,pl_Msg2,(long)va_Resultat_out);
            cuti_BilanConf_Trace(pl_Msg);    /* Notification dans Bilan config     */
            
        }
        else
        {
            XZST_03EcritureTrace(XZSTC_WARNING," cmvf_FctUti_MSE_XZAO257_PRV : cmvf_Trace_TYPE_Result a echoue");
            vl_retcode_out = XDC_NOK;
        }
    
        vl_retcode = vl_retcode_out;
    }    
    XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MSE_XZAO257_PRV: sortie avec %d",vl_retcode_out);
    return(vl_retcode_out);
}

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*    cmvf_FctUti_MSE_XZAO2571_PIC: Fct utilisateur passee a xzao2571_Lire_Config_PIC,
*  qui appelle XZAO2571_Ajouter_PIC avec le nom de la base sur la quelle XZAO2571_Ajouter_PIC va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cmvf_FctUti_MSE_XZAO2571_PIC ( XZAOT_ConfPIC    va_PIC_in, XDY_Entier     va_Resultat_out) 
/*
* ARGUMENTS EN ENTREE :  va_PIC_in contient le PIC a rajouter
*            va_Resultat_out contient le numero de ligne du fichier qui est en cours 
*            de traitement.
*                 
* ARGUMENTS EN SORTIE :  aucun
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
*
* CONDITION D'UTILISATION
*
* FONCTION 
*    Doit appeler XZAO2571_Ajouter_PIC
*     
------------------------------------------------------*/
{
    static char *version     = "@(#)cgcd_mSGBDeq.c	1.17 1.17 05/25/09: cmvf_FctUti_MSE_XZAO2571_PIC ";
    char    vl_NomBase[4]    ;
    int     vl_retcode     = XDC_OK;
    int     vl_retcode_out     = XDC_OK;
    char      pl_Msg[500];            /* contient le mesg a tracer*/
    char      pl_Msg2[500];            /* contient modification si vl_Resultat_out XDC_AJOUT,...*/
    char      pl_NomProcs[80] ;
    
    XDY_ResConf vl_Resultat_out;
    
    XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MSE_XZAO2571_PIC ");

    XZST_03EcritureTrace(XZSTC_WARNING,"BASE(%s) nom(%s) auto(%s) PR(%d) sens(%d) serveur(%s) liaison(%d) rgs(%s) nom site(%s) supprime(%s) Adresse_IP(%s) Port(%d)",
                CGCDG_Base_Courante,
                va_PIC_in.NomPIC,
                va_PIC_in.Autoroute,
                va_PIC_in.PR,
                va_PIC_in.Sens,
                va_PIC_in.NomServeur,
                va_PIC_in.LiaisonPIC,
				va_PIC_in.RgsPIC,
		        va_PIC_in.NomSite,
				va_PIC_in.Supprime,
				va_PIC_in.Adresse_IP,
				va_PIC_in.Port);
    
    strcpy( vl_NomBase,CGCDG_Base_Courante);    
    if ((vl_retcode = XZAO2571_Ajouter_PIC (vl_NomBase,va_PIC_in, &vl_Resultat_out))!=XDC_OK)
    {
        XZST_03EcritureTrace(XZSTC_WARNING," cmvf_FctUti_MSE_XZAO2571_PIC a echoue sur ligne %ld",(long)va_Resultat_out);
        vl_retcode_out = XDC_NOK;    
    }
    else
    /*A
    ** Test sur parametres de sortie de XZAO2571_Ajouter_PIC sur coherence base
    ** 4 valeurs de status d execution de la sp dans vl_Resultat_out
    ** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
    */
    {
        if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
        {
            strcpy(pl_NomProcs," cmvf_FctUti_MSE_XZAO2571_PIC ");
            XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld",pl_NomProcs,pl_Msg2,(long)va_Resultat_out);
            
            sprintf(pl_Msg,"%s %s sur la ligne %ld",XDF_CONF_PIC,pl_Msg2,(long)va_Resultat_out);
            cuti_BilanConf_Trace(pl_Msg);    /* Notification dans Bilan config     */
            
        }
        else
        {
            XZST_03EcritureTrace(XZSTC_WARNING," cmvf_FctUti_MSE_XZAO2571_PIC : cmvf_Trace_TYPE_Result a echoue");
            vl_retcode_out = XDC_NOK;
        }
    
        vl_retcode = vl_retcode_out;
    }    
    XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MSE_XZAO2571_PIC: sortie avec %d",vl_retcode_out);
    return(vl_retcode_out);
}

/*X*/
/*----------------------------------------------------
* SERVICE RENDU :
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cmvf_FctUti_MSE_XZAO450_BAF(XZAOT_AjoutBAF   va_BAF_in, XDY_Entier     va_Resultat_out) 
/*
* ARGUMENTS EN ENTREE :  va_NAV_in contient la NAV a rajouter
*            va_Resultat_out contient le numero de ligne du fichier qui est en cours 
*            de traitement.
*                 
* ARGUMENTS EN SORTIE :  aucun
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
*
* CONDITION D'UTILISATION
*
* FONCTION 
*    Doit appeler XZAO161_Ajouter_NAV
*     
------------------------------------------------------*/
{
    static char *version     = "$Id: cgcd_mSGBDeq.c,v 1.33 2021/05/10 08:52:03 pc2dpdy Exp $ : cmvf_FctUti_MSE_XZAO450_BAF";
    char    vl_NomBase[4]    ;
    int     vl_retcode     = XDC_OK;
    int     vl_retcode_out     = XDC_OK;
    char      pl_Msg[500];            /* contient le mesg a tracer*/
    char      pl_Msg2[500];            /* contient modification si vl_Resultat_out XDC_AJOUT,...*/
    char      pl_NomProcs[80] ;
    
    XDY_ResConf vl_Resultat_out;
    
    XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MSE_XZAO450_BAF");
    
    strcpy( vl_NomBase,CGCDG_Base_Courante);    
    if ((vl_retcode =XZAO450_Ajouter_BAF(vl_NomBase,va_BAF_in, &vl_Resultat_out))!=XDC_OK)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO450_BAF a echoue sur ligne %ld",(long)va_Resultat_out);
        vl_retcode_out = XDC_NOK;    
    }
    else
    /*A
    ** Test sur parametres de sortie de XZAO161_Ajouter_NAV sur coherence base
    ** 4 valeurs de status d execution de la sp dans vl_Resultat_out
    ** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
    */
    {
        if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
        {
            strcpy(pl_NomProcs,"XZAO450_Ajouter_BAF");
            XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld",pl_NomProcs,pl_Msg2,(long)va_Resultat_out);
            
            sprintf(pl_Msg,"%s %s sur la ligne %ld",XDF_CONF_BAF,pl_Msg2,(long)va_Resultat_out);
            cuti_BilanConf_Trace(pl_Msg);    /* Notification dans Bilan config     */
            
        }
        else
        {
            XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO450_BAF : cmvf_Trace_TYPE_Result a echoue");
            vl_retcode_out = XDC_NOK;
        }
    
        vl_retcode = vl_retcode_out;
    }    
    XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MSE_XZAO450_BAF : sortie avec %d",vl_retcode_out);
    return(vl_retcode_out);
}


/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cmvf_FctUti_MSE_XZAO451_BAD(XZAOT_AjoutBAD   va_BAD_in, XDY_Entier     va_Resultat_out) 
/*
* ARGUMENTS EN ENTREE :  va_NAV_in contient la NAV a rajouter
*            va_Resultat_out contient le numero de ligne du fichier qui est en cours 
*            de traitement.
*                 
* ARGUMENTS EN SORTIE :  aucun
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
*
* CONDITION D'UTILISATION
*
* FONCTION 
*    Doit appeler XZAO161_Ajouter_NAV
*     
------------------------------------------------------*/
{
    static char *version     = "$Id: cgcd_mSGBDeq.c,v 1.33 2021/05/10 08:52:03 pc2dpdy Exp $ : cmvf_FctUti_MSE_XZAO451_BAD";
    char    vl_NomBase[4]    ;
    int     vl_retcode     = XDC_OK;
    int     vl_retcode_out     = XDC_OK;
    char      pl_Msg[500];            /* contient le mesg a tracer*/
    char      pl_Msg2[500];            /* contient modification si vl_Resultat_out XDC_AJOUT,...*/
    char      pl_NomProcs[80] ;
    
    XDY_ResConf vl_Resultat_out;
    
    XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MSE_XZAO451_BAD");
    
    strcpy( vl_NomBase,CGCDG_Base_Courante);    
    if ((vl_retcode =XZAO451_Ajouter_BAD(vl_NomBase,va_BAD_in, &vl_Resultat_out))!=XDC_OK)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO451_BAD a echoue sur ligne %ld",(long)va_Resultat_out);
        vl_retcode_out = XDC_NOK;    
    }
    else
    /*A
    ** Test sur parametres de sortie de XZAO161_Ajouter_NAV sur coherence base
    ** 4 valeurs de status d execution de la sp dans vl_Resultat_out
    ** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
    */
    {
        if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
        {
            strcpy(pl_NomProcs,"XZAO451_Ajouter_BAD");
            XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld",pl_NomProcs,pl_Msg2,(long)va_Resultat_out);
            
            sprintf(pl_Msg,"%s %s sur la ligne %ld",XDF_CONF_BAD,pl_Msg2,(long)va_Resultat_out);
            cuti_BilanConf_Trace(pl_Msg);    /* Notification dans Bilan config     */
            
        }
        else
        {
            XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO451_BAD : cmvf_Trace_TYPE_Result a echoue");
            vl_retcode_out = XDC_NOK;
        }
    
        vl_retcode = vl_retcode_out;
    }    
    XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MSE_XZAO451_BAD : sortie avec %d",vl_retcode_out);
    return(vl_retcode_out);
}




/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*    cmvf_FctUti_MSE_XZAO428_ZON_PMVA : Fct utilisateur passee a xzao428_Lire_Config_Zon_PMVA
*  qui appelle XZAO4290_Ajouter_Zon_PMVA avec le nom de la base sur la quelle XZAO4290_Ajouter_Zon_PMVA
*  va avoir une action.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cmvf_FctUti_MSE_XZAO428_ZON_PMVA(    char         *va_autoroute_pmv,
                    XDY_Sens     va_sens_pmv,
                    XDY_PR        va_pr_pmv,
                    char        *va_site_pmv,
                    XDY_Entier   va_distance_pmv,
                    char        *va_autoroute_zone,
                    XDY_Sens    va_sens_zone,
                    XDY_PR        va_debut_zone,
                    XDY_PR        va_fin_zone,
                    char        *va_direction_zone,
                    XDY_Sens    va_type_zone,
                    XDY_Booleen    va_inhibition_zone,
                    XDY_Entier         va_Resultat_out) 
/*
* ARGUMENTS EN ENTREE :  infos de la zone d'influence PMV
*            va_Resultat_out contient le numero de ligne du fichier qui est en cours 
*            de traitement.
*                 
* ARGUMENTS EN SORTIE :  aucun
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
*
* CONDITION D'UTILISATION
*
* FONCTION 
*    Doit appeler XZAO403_Ajouter_Eqt_ZDP
*     
------------------------------------------------------*/
{
    static char *version     = "$Id: cgcd_mSGBDeq.c,v 1.33 2021/05/10 08:52:03 pc2dpdy Exp $ : cmvf_FctUti_MSE_XZAO428_ZON_PMVA";
    char    vl_NomBase[4]    ;
    int     vl_retcode     = XDC_OK;
    int     vl_retcode_out     = XDC_OK;
    char      pl_Msg[500];            /* contient le mesg a tracer*/
    char      pl_Msg2[500];            /* contient modification si vl_Resultat_out XDC_AJOUT,...*/
    char      pl_NomProcs[80] ;
    
    XDY_ResConf vl_Resultat_out;
    
    XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MSE_XZAO428_ZON_PMVA");
    
    strcpy( vl_NomBase,CGCDG_Base_Courante);    
    if ((vl_retcode = XZAO4290_Ajouter_Zon_PMVA(    vl_NomBase,
                            va_autoroute_pmv,
                            va_sens_pmv,
                            va_pr_pmv,
                            va_site_pmv,
                            va_distance_pmv,
                            va_autoroute_zone,
                            va_sens_zone,
                            va_debut_zone,
                            va_fin_zone,
                            va_direction_zone,
                            va_type_zone,
                            va_inhibition_zone,
                            &vl_Resultat_out))!=XDC_OK)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO428_ZON_PMVA a echoue sur ligne %ld",(long)va_Resultat_out);
        vl_retcode_out = XDC_NOK;    
    }
    else
    /*A
    ** Test sur parametres de sortie de XZAO4290_Ajouter_Zon_PMVA sur coherence base
    ** 4 valeurs de status d execution de la sp dans vl_Resultat_out
    ** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
    */
    {
        if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
        {
            strcpy(pl_NomProcs,"XZAO4290_Ajouter_Zon_PMVA");
            XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld",pl_NomProcs,pl_Msg2,(long)va_Resultat_out);
            
            sprintf(pl_Msg,"%s %s sur la ligne %ld",XDF_CONF_ZON_PMVA,pl_Msg2,(long)va_Resultat_out);
            cuti_BilanConf_Trace(pl_Msg);    /* Notification dans Bilan config     */
            
        }
        else
        {
            XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO428_ZON_PMVA : cmvf_Trace_TYPE_Result a echoue");
            vl_retcode_out = XDC_NOK;
        }
    
        vl_retcode = vl_retcode_out;
    }    
    XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MSE_XZAO428_ZON_PMVA : sortie avec %d",vl_retcode_out);
    return(vl_retcode_out);
}



/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*    cmvf_FctUti_MSE_XZAO433_ZON_BAF : Fct utilisateur passee a xzao433_Lire_Config_Zon_BAF
*  qui appelle XZAO435_Ajouter_Zon_BAF avec le nom de la base sur la quelle XZAO435_Ajouter_Zon_BAF
*  va avoir une action.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cmvf_FctUti_MSE_XZAO433_ZON_BAF(    char         *va_autoroute_pmv,
                    XDY_Sens     va_sens_pmv,
                    XDY_PR        va_pr_pmv,
                    char        *va_site_pmv,
                    char        *va_autoroute_zone,
                    XDY_Sens    va_sens_zone,
                    XDY_PR        va_debut_zone,
                    XDY_PR        va_fin_zone,
                    XDY_Booleen    va_inhibition_zone,
                    XDY_Entier         va_Resultat_out) 
/*
* ARGUMENTS EN ENTREE :  infos de la zone d'influence PMV
*            va_Resultat_out contient le numero de ligne du fichier qui est en cours 
*            de traitement.
*                 
* ARGUMENTS EN SORTIE :  aucun
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
*
* CONDITION D'UTILISATION
*
* FONCTION 
*    Doit appeler XZAO403_Ajouter_Eqt_ZDP
*     
------------------------------------------------------*/
{
    static char *version     = "$Id: cgcd_mSGBDeq.c,v 1.33 2021/05/10 08:52:03 pc2dpdy Exp $ : cmvf_FctUti_MSE_XZAO433_ZON_BAF";
    char    vl_NomBase[4]    ;
    int     vl_retcode     = XDC_OK;
    int     vl_retcode_out     = XDC_OK;
    char      pl_Msg[500];            /* contient le mesg a tracer*/
    char      pl_Msg2[500];            /* contient modification si vl_Resultat_out XDC_AJOUT,...*/
    char      pl_NomProcs[80] ;
    
    XDY_ResConf vl_Resultat_out;
    
    XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MSE_XZAO433_ZON_BAF");
    
    strcpy( vl_NomBase,CGCDG_Base_Courante);    
    if ((vl_retcode =XZAO435_Ajouter_Zon_BAF(    vl_NomBase,
                            va_autoroute_pmv,
                            va_sens_pmv,
                            va_pr_pmv,
                            va_site_pmv,
                            va_autoroute_zone,
                            va_sens_zone,
                            va_debut_zone,
                            va_fin_zone,
                            va_inhibition_zone,
                            &vl_Resultat_out))!=XDC_OK)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO433_ZON_BAF a echoue sur ligne %ld",(long)va_Resultat_out);
        vl_retcode_out = XDC_NOK;    
    }
    else
    /*A
    ** Test sur parametres de sortie de XZAO433_Ajouter_Zon_BAF sur coherence base
    ** 4 valeurs de status d execution de la sp dans vl_Resultat_out
    ** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
    */
    {
        if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
        {
            strcpy(pl_NomProcs,"XZAO433_Ajouter_Zon_BAF");
            XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld",pl_NomProcs,pl_Msg2,(long)va_Resultat_out);
            
            sprintf(pl_Msg,"%s %s sur la ligne %ld",XDF_CONF_ZON_BAF,pl_Msg2,(long)va_Resultat_out);
            cuti_BilanConf_Trace(pl_Msg);    /* Notification dans Bilan config     */
            
        }
        else
        {
            XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO428_ZON_PMVA : cmvf_Trace_TYPE_Result a echoue");
            vl_retcode_out = XDC_NOK;
        }
    
        vl_retcode = vl_retcode_out;
    }    
    XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MSE_XZAO433_ZON_BAF : sortie avec %d",vl_retcode_out);
    return(vl_retcode_out);
}







/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*    cmvf_FctUti_MSE_XZAO434_ZON_BAD : Fct utilisateur passee a xzao433_Lire_Config_Zon_BAF
*  qui appelle XZAO436_Ajouter_Zon_BAD avec le nom de la base sur la quelle XZAO436_Ajouter_Zon_BAF
*  va avoir une action.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cmvf_FctUti_MSE_XZAO434_ZON_BAD(    char         *va_autoroute_pmv,
                    XDY_Sens     va_sens_pmv,
                    XDY_PR        va_pr_pmv,
                    char        *va_site_pmv,
                    char        *va_autoroute_zone,
                    XDY_Sens    va_sens_zone,
                    XDY_PR        va_debut_zone,
                    XDY_PR        va_fin_zone,
                    XDY_Booleen    va_inhibition_zone,
                    XDY_Entier         va_Resultat_out) 
/*
* ARGUMENTS EN ENTREE :  infos de la zone d'influence PMV
*            va_Resultat_out contient le numero de ligne du fichier qui est en cours 
*            de traitement.
*                 
* ARGUMENTS EN SORTIE :  aucun
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
*
* CONDITION D'UTILISATION
*
* FONCTION 
*    Doit appeler XZAO403_Ajouter_Eqt_ZDP
*     
------------------------------------------------------*/
{
    static char *version     = "$Id: cgcd_mSGBDeq.c,v 1.33 2021/05/10 08:52:03 pc2dpdy Exp $ : cmvf_FctUti_MSE_XZAO434_ZON_BAD";
    char    vl_NomBase[4]    ;
    int     vl_retcode     = XDC_OK;
    int     vl_retcode_out     = XDC_OK;
    char      pl_Msg[500];            /* contient le mesg a tracer*/
    char      pl_Msg2[500];            /* contient modification si vl_Resultat_out XDC_AJOUT,...*/
    char      pl_NomProcs[80] ;
    
    XDY_ResConf vl_Resultat_out;
    
    XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MSE_XZAO434_ZON_BAD");
    
    strcpy( vl_NomBase,CGCDG_Base_Courante);    
    if ((vl_retcode =XZAO436_Ajouter_Zon_BAD(    vl_NomBase,
                            va_autoroute_pmv,
                            va_sens_pmv,
                            va_pr_pmv,
                            va_site_pmv,
                            va_autoroute_zone,
                            va_sens_zone,
                            va_debut_zone,
                            va_fin_zone,
                            va_inhibition_zone,
                            &vl_Resultat_out))!=XDC_OK)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO434_ZON_BAD a echoue sur ligne %ld",(long)va_Resultat_out);
        vl_retcode_out = XDC_NOK;    
    }
    else
    /*A
    ** Test sur parametres de sortie de XZAO434_Ajouter_Zon_BAD sur coherence base
    ** 4 valeurs de status d execution de la sp dans vl_Resultat_out
    ** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
    */
    {
        if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
        {
            strcpy(pl_NomProcs,"XZAO434_Ajouter_Zon_BAD");
            XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld",pl_NomProcs,pl_Msg2,(long)va_Resultat_out);
            
            sprintf(pl_Msg,"%s %s sur la ligne %ld",XDF_CONF_ZON_BAD,pl_Msg2,(long)va_Resultat_out);
            cuti_BilanConf_Trace(pl_Msg);    /* Notification dans Bilan config     */
            
        }
        else
        {
            XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO428_ZON_PMVA : cmvf_Trace_TYPE_Result a echoue");
            vl_retcode_out = XDC_NOK;
        }
    
        vl_retcode = vl_retcode_out;
    }    
    XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MSE_XZAO434_ZON_BAD : sortie avec %d",vl_retcode_out);
    return(vl_retcode_out);
}

/* AAZ 26/09/2007 */
/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*    cmvf_FctUti_MSE_XZAO640_600 : Fct utilisateur passee a xzao640_Lire_Config_SIG,
*       qui appelle XZAO640_Ajouter_SIG avec le nom de la base sur la quelle XZAO640_Ajouter_SIG 
*       va avoir une action.
*       Cette fonction trace les erreurs dans le fichier Bilan config.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cmvf_FctUti_MSE_XZAO640_600( XZAOT_ConfSIG  va_SIG_in,
                                 XDY_Entier     va_Resultat_out) 
/*
* ARGUMENTS EN ENTREE :
*      va_SIG_in contient l equipement du domaine signalisation a rajouter
*      va_Resultat_out contient le numero de ligne du fichier qui est en cours 
*         de traitement.
*                 
* ARGUMENTS EN SORTIE :  aucun
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
*
* CONDITION D'UTILISATION
*
* FONCTION 
*    Doit appeler XZAO600_Ajouter_SIG
*     
------------------------------------------------------*/
{
    static char *version       = "$Id: cgcd_mSGBDeq.c,v 1.33 2021/05/10 08:52:03 pc2dpdy Exp $ : cmvf_FctUti_MSE_XZAO640_600";
    char        vl_NomBase[4];
    int         vl_retcode     = XDC_OK;
    int         vl_retcode_out = XDC_OK;
    char        pl_Msg[500];            /* contient le mesg a tracer*/
    char        pl_Msg2[500];            /* contient modification si vl_Resultat_out XDC_AJOUT,...*/
    char        pl_NomProcs[80] ;

    XDY_ResConf vl_Resultat_out;

    XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MSE_XZAO640_600");
    printf("AZA IN  : cmvf_FctUti_MSE_XZAO640_600");

    strcpy( vl_NomBase, CGCDG_Base_Courante);    
    if ((vl_retcode =XZAO600_Ajouter_SIG(vl_NomBase, va_SIG_in, &vl_Resultat_out))!=XDC_OK)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO640_600 a echoue sur ligne %ld", (long)va_Resultat_out);
        vl_retcode_out = XDC_NOK;    
    }
    else
    /*A
    ** Test sur parametres de sortie de XZAO600_Ajouter_SIG sur coherence base
    ** 4 valeurs de status d execution de la sp dans vl_Resultat_out
    ** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
    */
    {
        if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
        {
            strcpy(pl_NomProcs,"XZAO600_Ajouter_SIG");
            XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld", pl_NomProcs, pl_Msg2, (long)va_Resultat_out);

            sprintf(pl_Msg,"%s %s sur la ligne %ld", XDF_CONF_SIG, pl_Msg2, (long)va_Resultat_out);
            cuti_BilanConf_Trace(pl_Msg);    /* Notification dans Bilan config     */
        }
        else
        {
            XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO640_600 : cmvf_Trace_TYPE_Result a echoue");
            vl_retcode_out = XDC_NOK;
        }

        vl_retcode = vl_retcode_out;
    }    
    XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MSE_XZAO640_600 : sortie avec %d", vl_retcode_out);
    return(vl_retcode_out);
}

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*    cmvf_FctUti_MSE_XZAO641_601 : Fct utilisateur passee a xzao641_Lire_Config_ECL,
*       qui appelle XZAO641_Ajouter_ECL avec le nom de la base sur la quelle XZAO641_Ajouter_ECL 
*       va avoir une action.
*       Cette fonction trace les erreurs dans le fichier Bilan config.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cmvf_FctUti_MSE_XZAO641_601( XZAOT_ConfECL  va_ECL_in,
                                 XDY_Entier     va_Resultat_out) 
/*
* ARGUMENTS EN ENTREE :
*      va_ECL_in contient l equipement du domaine eclairage a rajouter
*      va_Resultat_out contient le numero de ligne du fichier qui est en cours 
*         de traitement.
*                 
* ARGUMENTS EN SORTIE :  aucun
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
*
* CONDITION D'UTILISATION
*
* FONCTION 
*    Doit appeler XZAO601_Ajouter_ECL
*     
------------------------------------------------------*/
{
    static char *version       = "$Id: cgcd_mSGBDeq.c,v 1.33 2021/05/10 08:52:03 pc2dpdy Exp $ : cmvf_FctUti_MSE_XZAO641_601";
    char        vl_NomBase[4];
    int         vl_retcode     = XDC_OK;
    int         vl_retcode_out = XDC_OK;
    char        pl_Msg[500];            /* contient le mesg a tracer*/
    char        pl_Msg2[500];            /* contient modification si vl_Resultat_out XDC_AJOUT,...*/
    char        pl_NomProcs[80] ;

    XDY_ResConf vl_Resultat_out;

    XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MSE_XZAO641_601");

    strcpy( vl_NomBase, CGCDG_Base_Courante);    
    if ((vl_retcode =XZAO601_Ajouter_ECL(vl_NomBase, va_ECL_in, &vl_Resultat_out))!=XDC_OK)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO641_601 a echoue sur ligne %ld", (long)va_Resultat_out);
        vl_retcode_out = XDC_NOK;    
    }
    else
    /*A
    ** Test sur parametres de sortie de XZAO601_Ajouter_ECL sur coherence base
    ** 4 valeurs de status d execution de la sp dans vl_Resultat_out
    ** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
    */
    {
        if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
        {
            strcpy(pl_NomProcs,"XZAO601_Ajouter_ECL");
            XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld", pl_NomProcs, pl_Msg2, (long)va_Resultat_out);

            sprintf(pl_Msg,"%s %s sur la ligne %ld", XDF_CONF_ECL, pl_Msg2, (long)va_Resultat_out);
            cuti_BilanConf_Trace(pl_Msg);    /* Notification dans Bilan config     */
        }
        else
        {
            XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO641_601 : cmvf_Trace_TYPE_Result a echoue");
            vl_retcode_out = XDC_NOK;
        }

        vl_retcode = vl_retcode_out;
    }    
    XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MSE_XZAO641_601 : sortie avec %d", vl_retcode_out);
    return(vl_retcode_out);
}

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*    cmvf_FctUti_MSE_XZAO642_602 : Fct utilisateur passee a xzao642_Lire_Config_VEN,
*       qui appelle XZAO642_Ajouter_VEN avec le nom de la base sur la quelle XZAO642_Ajouter_VEN 
*       va avoir une action.
*       Cette fonction trace les erreurs dans le fichier Bilan config.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cmvf_FctUti_MSE_XZAO642_602( XZAOT_ConfVEN  va_VEN_in,
                                 XDY_Entier     va_Resultat_out) 
/*
* ARGUMENTS EN ENTREE :
*      va_VEN_in contient l equipement du domaine ventilation a rajouter
*      va_Resultat_out contient le numero de ligne du fichier qui est en cours 
*         de traitement.
*                 
* ARGUMENTS EN SORTIE :  aucun
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
*
* CONDITION D'UTILISATION
*
* FONCTION 
*    Doit appeler XZAO602_Ajouter_VEN
*     
------------------------------------------------------*/
{
    static char *version       = "$Id: cgcd_mSGBDeq.c,v 1.33 2021/05/10 08:52:03 pc2dpdy Exp $ : cmvf_FctUti_MSE_XZAO642_602";
    char        vl_NomBase[4];
    int         vl_retcode     = XDC_OK;
    int         vl_retcode_out = XDC_OK;
    char        pl_Msg[500];            /* contient le mesg a tracer*/
    char        pl_Msg2[500];            /* contient modification si vl_Resultat_out XDC_AJOUT,...*/
    char        pl_NomProcs[80] ;

    XDY_ResConf vl_Resultat_out;

    XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MSE_XZAO642_602");

    strcpy( vl_NomBase, CGCDG_Base_Courante);    
    if ((vl_retcode =XZAO602_Ajouter_VEN(vl_NomBase, va_VEN_in, &vl_Resultat_out))!=XDC_OK)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO642_602 a echoue sur ligne %ld", (long)va_Resultat_out);
        vl_retcode_out = XDC_NOK;    
    }
    else
    /*A
    ** Test sur parametres de sortie de XZAO602_Ajouter_VEN sur coherence base
    ** 4 valeurs de status d execution de la sp dans vl_Resultat_out
    ** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
    */
    {
        if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
        {
            strcpy(pl_NomProcs,"XZAO602_Ajouter_VEN");
            XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld", pl_NomProcs, pl_Msg2, (long)va_Resultat_out);

            sprintf(pl_Msg,"%s %s sur la ligne %ld", XDF_CONF_VEN, pl_Msg2, (long)va_Resultat_out);
            cuti_BilanConf_Trace(pl_Msg);    /* Notification dans Bilan config     */
        }
        else
        {
            XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO642_602 : cmvf_Trace_TYPE_Result a echoue");
            vl_retcode_out = XDC_NOK;
        }

        vl_retcode = vl_retcode_out;
    }    
    XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MSE_XZAO642_602 : sortie avec %d", vl_retcode_out);
    return(vl_retcode_out);
}

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*    cmvf_FctUti_MSE_XZAO643_603 : Fct utilisateur passee a xzao643_Lire_Config_ENR,
*       qui appelle XZAO643_Ajouter_ENR avec le nom de la base sur la quelle XZAO643_Ajouter_ENR 
*       va avoir une action.
*       Cette fonction trace les erreurs dans le fichier Bilan config.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cmvf_FctUti_MSE_XZAO643_603( XZAOT_ConfENR  va_ENR_in,
                                 XDY_Entier     va_Resultat_out) 
/*
* ARGUMENTS EN ENTREE :
*      va_ENR_in contient l equipement du domaine energie a rajouter
*      va_Resultat_out contient le numero de ligne du fichier qui est en cours 
*         de traitement.
*                 
* ARGUMENTS EN SORTIE :  aucun
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
*
* CONDITION D'UTILISATION
*
* FONCTION 
*    Doit appeler XZAO603_Ajouter_ENR
*     
------------------------------------------------------*/
{
    static char *version       = "$Id: cgcd_mSGBDeq.c,v 1.33 2021/05/10 08:52:03 pc2dpdy Exp $ : cmvf_FctUti_MSE_XZAO643_603";
    char        vl_NomBase[4];
    int         vl_retcode     = XDC_OK;
    int         vl_retcode_out = XDC_OK;
    char        pl_Msg[500];            /* contient le mesg a tracer*/
    char        pl_Msg2[500];            /* contient modification si vl_Resultat_out XDC_AJOUT,...*/
    char        pl_NomProcs[80] ;

    XDY_ResConf vl_Resultat_out;

    XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MSE_XZAO643_603");

    strcpy( vl_NomBase, CGCDG_Base_Courante);    
    if ((vl_retcode =XZAO603_Ajouter_ENR(vl_NomBase, va_ENR_in, &vl_Resultat_out))!=XDC_OK)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO643_603 a echoue sur ligne %ld", (long)va_Resultat_out);
        vl_retcode_out = XDC_NOK;    
    }
    else
    /*A
    ** Test sur parametres de sortie de XZAO603_Ajouter_ENR sur coherence base
    ** 4 valeurs de status d execution de la sp dans vl_Resultat_out
    ** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
    */
    {
        if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
        {
            strcpy(pl_NomProcs,"XZAO603_Ajouter_ENR");
            XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld", pl_NomProcs, pl_Msg2, (long)va_Resultat_out);

            sprintf(pl_Msg,"%s %s sur la ligne %ld", XDF_CONF_ENR, pl_Msg2, (long)va_Resultat_out);
            cuti_BilanConf_Trace(pl_Msg);    /* Notification dans Bilan config     */
        }
        else
        {
            XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO643_603 : cmvf_Trace_TYPE_Result a echoue");
            vl_retcode_out = XDC_NOK;
        }

        vl_retcode = vl_retcode_out;
    }    
    XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MSE_XZAO643_603 : sortie avec %d", vl_retcode_out);
    return(vl_retcode_out);
}

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*    cmvf_FctUti_MSE_XZAO644_604 : Fct utilisateur passee a xzao644_Lire_Config_INC,
*       qui appelle XZAO644_Ajouter_INC avec le nom de la base sur la quelle XZAO644_Ajouter_INC 
*       va avoir une action.
*       Cette fonction trace les erreurs dans le fichier Bilan config.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cmvf_FctUti_MSE_XZAO644_604( XZAOT_ConfINC  va_INC_in,
                                 XDY_Entier     va_Resultat_out) 
/*
* ARGUMENTS EN ENTREE :
*      va_INC_in contient l equipement du domaine protection incendie a rajouter
*      va_Resultat_out contient le numero de ligne du fichier qui est en cours 
*         de traitement.
*                 
* ARGUMENTS EN SORTIE :  aucun
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
*
* CONDITION D'UTILISATION
*
* FONCTION 
*    Doit appeler XZAO604_Ajouter_INC
*     
------------------------------------------------------*/
{
    static char *version       = "$Id: cgcd_mSGBDeq.c,v 1.33 2021/05/10 08:52:03 pc2dpdy Exp $ : cmvf_FctUti_MSE_XZAO644_604";
    char        vl_NomBase[4];
    int         vl_retcode     = XDC_OK;
    int         vl_retcode_out = XDC_OK;
    char        pl_Msg[500];            /* contient le mesg a tracer*/
    char        pl_Msg2[500];            /* contient modification si vl_Resultat_out XDC_AJOUT,...*/
    char        pl_NomProcs[80] ;

    XDY_ResConf vl_Resultat_out;

    XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MSE_XZAO644_604");

    strcpy( vl_NomBase, CGCDG_Base_Courante);    
    if ((vl_retcode =XZAO604_Ajouter_INC(vl_NomBase, va_INC_in, &vl_Resultat_out))!=XDC_OK)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO644_604 a echoue sur ligne %ld", (long)va_Resultat_out);
        vl_retcode_out = XDC_NOK;    
    }
    else
    /*A
    ** Test sur parametres de sortie de XZAO604_Ajouter_INC sur coherence base
    ** 4 valeurs de status d execution de la sp dans vl_Resultat_out
    ** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
    */
    {
        if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
        {
            strcpy(pl_NomProcs,"XZAO604_Ajouter_INC");
            XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld", pl_NomProcs, pl_Msg2, (long)va_Resultat_out);

            sprintf(pl_Msg,"%s %s sur la ligne %ld", XDF_CONF_INC, pl_Msg2, (long)va_Resultat_out);
            cuti_BilanConf_Trace(pl_Msg);    /* Notification dans Bilan config     */
        }
        else
        {
            XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO644_604 : cmvf_Trace_TYPE_Result a echoue");
            vl_retcode_out = XDC_NOK;
        }

        vl_retcode = vl_retcode_out;
    }    
    XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MSE_XZAO644_604 : sortie avec %d", vl_retcode_out);
    return(vl_retcode_out);
}

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*    cmvf_FctUti_MSE_XZAO645_605 : Fct utilisateur passee a xzao645_Lire_Config_EXP,
*       qui appelle XZAO645_Ajouter_EXP avec le nom de la base sur la quelle XZAO645_Ajouter_EXP 
*       va avoir une action.
*       Cette fonction trace les erreurs dans le fichier Bilan config.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cmvf_FctUti_MSE_XZAO645_605( XZAOT_ConfEXP  va_EXP_in,
                                 XDY_Entier     va_Resultat_out) 
/*
* ARGUMENTS EN ENTREE :
*      va_EXP_in contient l equipement du domaine exploitation a rajouter
*      va_Resultat_out contient le numero de ligne du fichier qui est en cours 
*         de traitement.
*                 
* ARGUMENTS EN SORTIE :  aucun
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
*
* CONDITION D'UTILISATION
*
* FONCTION 
*    Doit appeler XZAO605_Ajouter_EXP
*     
------------------------------------------------------*/
{
    static char *version       = "$Id: cgcd_mSGBDeq.c,v 1.33 2021/05/10 08:52:03 pc2dpdy Exp $ : cmvf_FctUti_MSE_XZAO645_605";
    char        vl_NomBase[4];
    int         vl_retcode     = XDC_OK;
    int         vl_retcode_out = XDC_OK;
    char        pl_Msg[500];            /* contient le mesg a tracer*/
    char        pl_Msg2[500];            /* contient modification si vl_Resultat_out XDC_AJOUT,...*/
    char        pl_NomProcs[80] ;

    XDY_ResConf vl_Resultat_out;

    XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MSE_XZAO645_605");

    strcpy( vl_NomBase, CGCDG_Base_Courante);    
    if ((vl_retcode =XZAO605_Ajouter_EXP(vl_NomBase, va_EXP_in, &vl_Resultat_out))!=XDC_OK)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO645_605 a echoue sur ligne %ld", (long)va_Resultat_out);
        vl_retcode_out = XDC_NOK;    
    }
    else
    /*A
    ** Test sur parametres de sortie de XZAO605_Ajouter_EXP sur coherence base
    ** 4 valeurs de status d execution de la sp dans vl_Resultat_out
    ** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
    */
    {
        if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
        {
            strcpy(pl_NomProcs,"XZAO605_Ajouter_EXP");
            XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld", pl_NomProcs, pl_Msg2, (long)va_Resultat_out);

            sprintf(pl_Msg,"%s %s sur la ligne %ld", XDF_CONF_EXP, pl_Msg2, (long)va_Resultat_out);
            cuti_BilanConf_Trace(pl_Msg);    /* Notification dans Bilan config     */
        }
        else
        {
            XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO645_605 : cmvf_Trace_TYPE_Result a echoue");
            vl_retcode_out = XDC_NOK;
        }

        vl_retcode = vl_retcode_out;
    }    
    XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MSE_XZAO645_605 : sortie avec %d", vl_retcode_out);
    return(vl_retcode_out);
}

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*    cmvf_FctUti_MSE_XZAO646_606 : Fct utilisateur passee a xzao646_Lire_Config_SYS,
*       qui appelle XZAO646_Ajouter_SYS avec le nom de la base sur la quelle XZAO646_Ajouter_SYS 
*       va avoir une action.
*       Cette fonction trace les erreurs dans le fichier Bilan config.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cmvf_FctUti_MSE_XZAO646_606( XZAOT_ConfSYS  va_SYS_in,
                                 XDY_Entier     va_Resultat_out) 
/*
* ARGUMENTS EN ENTREE :
*      va_SYS_in contient l equipement du domaine systeme a rajouter
*      va_Resultat_out contient le numero de ligne du fichier qui est en cours 
*         de traitement.
*                 
* ARGUMENTS EN SORTIE :  aucun
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
*
* CONDITION D'UTILISATION
*
* FONCTION 
*    Doit appeler XZAO606_Ajouter_SYS
*     
------------------------------------------------------*/
{
    static char *version       = "$Id: cgcd_mSGBDeq.c,v 1.33 2021/05/10 08:52:03 pc2dpdy Exp $ : cmvf_FctUti_MSE_XZAO646_606";
    char        vl_NomBase[4];
    int         vl_retcode     = XDC_OK;
    int         vl_retcode_out = XDC_OK;
    char        pl_Msg[500];            /* contient le mesg a tracer*/
    char        pl_Msg2[500];            /* contient modification si vl_Resultat_out XDC_AJOUT,...*/
    char        pl_NomProcs[80] ;

    XDY_ResConf vl_Resultat_out;

    XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MSE_XZAO646_606");

    strcpy( vl_NomBase, CGCDG_Base_Courante);    
    if ((vl_retcode =XZAO606_Ajouter_SYS(vl_NomBase, va_SYS_in, &vl_Resultat_out))!=XDC_OK)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO646_606 a echoue sur ligne %ld", (long)va_Resultat_out);
        vl_retcode_out = XDC_NOK;    
    }
    else
    /*A
    ** Test sur parametres de sortie de XZAO606_Ajouter_SYS sur coherence base
    ** 4 valeurs de status d execution de la sp dans vl_Resultat_out
    ** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
    */
    {
        if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
        {
            strcpy(pl_NomProcs,"XZAO606_Ajouter_SYS");
            XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld", pl_NomProcs, pl_Msg2, (long)va_Resultat_out);

            sprintf(pl_Msg,"%s %s sur la ligne %ld", XDF_CONF_SYS, pl_Msg2, (long)va_Resultat_out);
            cuti_BilanConf_Trace(pl_Msg);    /* Notification dans Bilan config     */
        }
        else
        {
            XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO646_606 : cmvf_Trace_TYPE_Result a echoue");
            vl_retcode_out = XDC_NOK;
        }

        vl_retcode = vl_retcode_out;
    }    
    XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MSE_XZAO646_606 : sortie avec %d", vl_retcode_out);
    return(vl_retcode_out);
}

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*    cmvf_FctUti_MSE_XZAO647_607 : Fct utilisateur passee a xzao647_Lire_Config_LTN,
*       qui appelle XZAO647_Ajouter_LTN avec le nom de la base sur la quelle XZAO647_Ajouter_LTN 
*       va avoir une action.
*       Cette fonction trace les erreurs dans le fichier Bilan config.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cmvf_FctUti_MSE_XZAO647_607( XZAOT_ConfLTN  va_LTN_in,
                                 XDY_Entier     va_Resultat_out) 
/*
* ARGUMENTS EN ENTREE :
*      va_LTN_in contient l equipement du domaine locaux techniques a rajouter
*      va_Resultat_out contient le numero de ligne du fichier qui est en cours 
*         de traitement.
*                 
* ARGUMENTS EN SORTIE :  aucun
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
*
* CONDITION D'UTILISATION
*
* FONCTION 
*    Doit appeler XZAO607_Ajouter_LTN
*     
------------------------------------------------------*/
{
    static char *version       = "$Id: cgcd_mSGBDeq.c,v 1.33 2021/05/10 08:52:03 pc2dpdy Exp $ : cmvf_FctUti_MSE_XZAO647_607";
    char        vl_NomBase[4];
    int         vl_retcode     = XDC_OK;
    int         vl_retcode_out = XDC_OK;
    char        pl_Msg[500];            /* contient le mesg a tracer*/
    char        pl_Msg2[500];            /* contient modification si vl_Resultat_out XDC_AJOUT,...*/
    char        pl_NomProcs[80] ;

    XDY_ResConf vl_Resultat_out;

    XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MSE_XZAO647_607");

    strcpy( vl_NomBase, CGCDG_Base_Courante);    
    if ((vl_retcode =XZAO607_Ajouter_LTN(vl_NomBase, va_LTN_in, &vl_Resultat_out))!=XDC_OK)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO647_607 a echoue sur ligne %ld", (long)va_Resultat_out);
        vl_retcode_out = XDC_NOK;    
    }
    else
    /*A
    ** Test sur parametres de sortie de XZAO607_Ajouter_LTN sur coherence base
    ** 4 valeurs de status d execution de la sp dans vl_Resultat_out
    ** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
    */
    {
        if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
        {
            strcpy(pl_NomProcs,"XZAO607_Ajouter_LTN");
            XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld", pl_NomProcs, pl_Msg2, (long)va_Resultat_out);

            sprintf(pl_Msg,"%s %s sur la ligne %ld", XDF_CONF_LTN, pl_Msg2, (long)va_Resultat_out);
            cuti_BilanConf_Trace(pl_Msg);    /* Notification dans Bilan config     */
        }
        else
        {
            XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO647_607 : cmvf_Trace_TYPE_Result a echoue");
            vl_retcode_out = XDC_NOK;
        }

        vl_retcode = vl_retcode_out;
    }    
    XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MSE_XZAO647_607 : sortie avec %d", vl_retcode_out);
    return(vl_retcode_out);
}


/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*    cmvf_FctUti_MSE_XZAO991_992 : Fct utilisateur passee a xzao991_Lire_Config_Equipement,
*  qui appelle XZAO992_Ajouter_Equipement avec le nom de la base sur la quelle XZAO992_Ajouter_Equipement 
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cmvf_FctUti_MSE_XZAO991_992( XZAOT_ConfEqt va_EqtGTC_in, XDY_Entier     va_Resultat_out ) 
/*
* ARGUMENTS EN ENTREE :  va_EqtGTC_in contient le EqtGTC a rajouter
*            va_Resultat_out contient le numero de ligne du fichier qui est en cours 
*            de traitement.
*                 
* ARGUMENTS EN SORTIE :  aucun
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
*
* CONDITION D'UTILISATION
*
* FONCTION 
*    Doit appeler XZAO992_Ajouter_Equipement
*     
------------------------------------------------------*/
{
    static char *version       = "$Id: cgcd_mSGBDeq.c,v 1.33 2021/05/10 08:52:03 pc2dpdy Exp $ : cmvf_FctUti_MSE_XZAO991_992";
    char        vl_NomBase[4]    ;
    int         vl_retcode     = XDC_OK;
    int         vl_retcode_out = XDC_OK;
    char        pl_Msg[500];     /* contient le mesg a tracer*/
    char        pl_Msg2[500];    /* contient modification si vl_Resultat_out XDC_AJOUT,...*/
    char        pl_NomProcs[80] ;
    
    XDY_ResConf vl_Resultat_out;
    
    XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MSE_XZAO991_992");
    XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : cmvf_FctUti_MSE_XZAO991_992 : Type = %s\tNom = %s\tAutoroute = %s\tMaitre = %s\tPR = %d\tSens = %d\tVoie = %d\tInverse = %d\tDomaine = %s",
            va_EqtGTC_in.TypeEqt,
            va_EqtGTC_in.NomEqt,
            va_EqtGTC_in.Autoroute,
            va_EqtGTC_in.NomMaitre,
            va_EqtGTC_in.PR,
            va_EqtGTC_in.Sens,
            va_EqtGTC_in.Voie,
            va_EqtGTC_in.Inverse,
            va_EqtGTC_in.Domaine );
        
    strcpy( vl_NomBase,CGCDG_Base_Courante);    
    if ((vl_retcode =XZAO992_Ajouter_Equipement(vl_NomBase,va_EqtGTC_in, &vl_Resultat_out))!=XDC_OK)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO991_992 a echoue sur ligne %ld",(long)va_Resultat_out);
        vl_retcode_out = XDC_NOK;    
    }
    else
    /*A
    ** Test sur parametres de sortie de XZAO992_Ajouter_Equipement sur coherence base
    ** 4 valeurs de status d execution de la sp dans vl_Resultat_out
    ** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
    */
    {
        if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
        {
            strcpy(pl_NomProcs,"XZAO992_Ajouter_Equipement");
            XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld",pl_NomProcs,pl_Msg2,(long)va_Resultat_out);
            
            sprintf(pl_Msg,"%s %s sur la ligne %ld",XDF_Equipement_GTCNiceRenovee,pl_Msg2,(long)va_Resultat_out);
            cuti_BilanConf_Trace(pl_Msg);    /* Notification dans Bilan config     */
            
        }
        else
        {
            XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO991_992 : cmvf_Trace_TYPE_Result a echoue");
            vl_retcode_out = XDC_NOK;
        }
    
        vl_retcode = vl_retcode_out;
    }    
    XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MSE_XZAO991_992 : sortie avec %d",vl_retcode_out);
    return(vl_retcode_out);
}

/* AAZ */


/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*    cmvf_FctUti_MSE_XZAO553 : Fct utilisateur passee a XZAO553_Ajouter_Zone_Reg,
*  qui appelle XZAO553_Ajouter_Zone_Reg avec le nom de la base sur la quelle XZAO553_Ajouter_Zone_Reg 
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cmvf_FctUti_MSE_XZAO553_Zone_Regul( XZAOT_ConfZoneReg va_ZoneReg_in, XDY_Entier     va_Resultat_out ) 
/*
* ARGUMENTS EN ENTREE :  va_ZoneReg_in contient le EqtGTC a rajouter
*            va_Resultat_out contient le numero de ligne du fichier qui est en cours 
*            de traitement.
*                 
* ARGUMENTS EN SORTIE :  aucun
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
*
* CONDITION D'UTILISATION
*
* FONCTION 
*    Doit appeler XZAO553_Ajouter_Zone_Reg
*     
------------------------------------------------------*/
{
    static char *version       = "@(#)cgcd_mSGBDeq.c	1.17 05/25/09 : cmvf_FctUti_MSE_XZAO571";
    char        vl_NomBase[4]    ;
    int         vl_retcode     = XDC_OK;
    int         vl_retcode_out = XDC_OK;
    char        pl_Msg[500];     /* contient le mesg a tracer*/
    char        pl_Msg2[500];    /* contient modification si vl_Resultat_out XDC_AJOUT,...*/
    char        pl_NomProcs[80] ;
    
    XDY_ResConf vl_Resultat_out;
    
    XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MSE_XZAO553");
    XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : cmvf_FctUti_MSE_XZAO553 : tNom = %s\tAutoroute = %s\tSens = %d\tPR_Debut = %d\tPR_Fin = %d\tVitesse = %d\tInhibition = %d\tMode_Horaire = %d\tNomServeur = %s",
            va_ZoneReg_in.Nom,
            va_ZoneReg_in.Autoroute,
            va_ZoneReg_in.Sens,
            va_ZoneReg_in.PR_Debut,
            va_ZoneReg_in.PR_Fin,
            va_ZoneReg_in.Vitesse,
            va_ZoneReg_in.Inhibition,
            va_ZoneReg_in.Mode_Horaire,
            va_ZoneReg_in.NomServeur);
			
    strcpy( vl_NomBase,CGCDG_Base_Courante);    
    if ((vl_retcode =XZAO553_Ajouter_Zone_Reg(vl_NomBase,va_ZoneReg_in, &vl_Resultat_out))!=XDC_OK)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO553 a echoue sur ligne %ld",(long)va_Resultat_out);
        vl_retcode_out = XDC_NOK;    
    }
    else
    /*A
    ** Test sur parametres de sortie de XZAO553_Ajouter_Zone_Reg sur coherence base
    ** 4 valeurs de status d execution de la sp dans vl_Resultat_out
    ** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
    */
    {
        if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
        {
            strcpy(pl_NomProcs,"XZAO553_Ajouter_Zone_Reg");
            XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld",pl_NomProcs,pl_Msg2,(long)va_Resultat_out);
            
            sprintf(pl_Msg,"%s %s sur la ligne %ld",XDF_CONF_ZON_REG,pl_Msg2,(long)va_Resultat_out);
            cuti_BilanConf_Trace(pl_Msg);    /* Notification dans Bilan config     */
            
        }
        else
        {
            XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO553 : cmvf_Trace_TYPE_Result a echoue");
            vl_retcode_out = XDC_NOK;
        }
    
        vl_retcode = vl_retcode_out;
    }    
    XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MSE_XZAO553 : sortie avec %d",vl_retcode_out);
    return(vl_retcode_out);
}

/* AAZ */



/*X*/
/*----------------------------------------------------
* SERVICE RENDU :
*    cmvf_FctUti_MSE_XZAO352_351 : Fct utilisateur passee a xzao417_Lire_Config_Zon_TUN
*  qui appelle XZAO351_Ajouter_Zon_TUN avec le nom de la base sur la quelle XZAO351_Ajouter_Zon_TUN
*  va avoir une action.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cmvf_FctUti_MSE_XZAO352_351(    char         *va_autoroute_tun,
                    XDY_Sens     va_sens_tun,
                    XDY_PR        va_pr_tun,
                    char        *va_autoroute_zone,
                    XDY_Sens    va_sens_zone,
                    XDY_PR        va_debut_zone,
                    XDY_PR        va_fin_zone,
                    XDY_Octet   va_type_zone,
                  char *va_groupe_tunnel,
                    XDY_PR              va_PRPSTT_in,
                    XDY_Booleen    va_inhibition_zone,
                    XDY_Entier         va_Resultat_out)
/*
* ARGUMENTS EN ENTREE :  infos de la zone d'influence PMV
*            va_Resultat_out contient le numero de ligne du fichier qui est en cours
*            de traitement.
*
* ARGUMENTS EN SORTIE :  aucun
* CODE RETOUR :
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
*
* CONDITION D'UTILISATION
*
* FONCTION
*    Doit appeler XZAO403_Ajouter_Eqt_ZDP
*    
------------------------------------------------------*/
{
    static char *version     = "$Id: cgcd_mSGBDeq.c,v 1.33 2021/05/10 08:52:03 pc2dpdy Exp $ : cmvf_FctUti_MSE_XZAO417_416";
    char    vl_NomBase[4]    ;
    int     vl_retcode     = XDC_OK;
    int     vl_retcode_out     = XDC_OK;
    char      pl_Msg[500];            /* contient le mesg a tracer*/
    char      pl_Msg2[500];            /* contient modification si vl_Resultat_out XDC_AJOUT,...*/
    char      pl_NomProcs[80] ;

    XDY_ResConf vl_Resultat_out;


    XZST_03EcritureTrace(XZSTC_WARNING,"IN  : cmvf_FctUti_MSE_XZAO352_351");

    strcpy( vl_NomBase,CGCDG_Base_Courante);
    if ((vl_retcode =XZAO351_Ajouter_Zon_TUN(    vl_NomBase,
                            va_autoroute_tun,
                            va_sens_tun,
                            va_pr_tun,
                            va_autoroute_zone,
                            va_sens_zone,
                            va_debut_zone,
                            va_fin_zone,
                            va_type_zone,
                            va_groupe_tunnel,
                            va_inhibition_zone,
                            va_PRPSTT_in,
                            &vl_Resultat_out))!=XDC_OK)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO352_351 a echoue sur ligne %ld",(long)va_Resultat_out);
        vl_retcode_out = XDC_NOK;
    }
    else
    /*A
    ** Test sur parametres de sortie de XZAO416_Ajouter_Zon_PMV sur coherence base
    ** 4 valeurs de status d execution de la sp dans vl_Resultat_out
    ** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
   */
    {
        if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
        {
            strcpy(pl_NomProcs,"XZAO352_Ajouter_Zon_TUN");
            XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld",pl_NomProcs,pl_Msg2,(long)va_Resultat_out);

            sprintf(pl_Msg,"%s %s sur la ligne %ld",XDF_CONF_ZON_TUN,pl_Msg2,(long)va_Resultat_out);
            cuti_BilanConf_Trace(pl_Msg);    /* Notification dans Bilan config     */

        }
        else
        {
            XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO352_351 : cmvf_Trace_TYPE_Result a echoue");
            vl_retcode_out = XDC_NOK;
        }

        vl_retcode = vl_retcode_out;
    }   
    XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MSE_XZAO352_351 : sortie avec %d",vl_retcode_out);
    return(vl_retcode_out);
}



/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*    cmvf_FctUti_MSE_XZAO422_730 : Fct utilisateur passee a xzao480_Lire_Config_Eqt_SAGA,
*  qui appelle XZAO730_Ajouter_Eqt_SAGA avec le nom de la base a configurer.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cmvf_FctUti_MSE_XZAO422_730( XZAOT_ConfEqtSAGA va_Eqt_in, XDY_Entier     va_Resultat_out ) 
/*
* ARGUMENTS EN ENTREE :  va_EqtGTC_in contient le EqtGTC a rajouter
*            va_Resultat_out contient le numero de ligne du fichier qui est en cours 
*            de traitement.
*                 
* ARGUMENTS EN SORTIE :  aucun
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
*
* CONDITION D'UTILISATION
*
* FONCTION 
*    Doit appeler XZAO992_Ajouter_Equipement
*     
------------------------------------------------------*/
{
    static char *version       = "$Id: cgcd_mSGBDeq.c,v 1.33 2021/05/10 08:52:03 pc2dpdy Exp $ : cmvf_FctUti_MSE_XZAO991_992";
    char        vl_NomBase[4]    ;
    int         vl_retcode     = XDC_OK;
    int         vl_retcode_out = XDC_OK;
    char        pl_Msg[500];     /* contient le mesg a tracer*/
    char        pl_Msg2[500];    /* contient modification si vl_Resultat_out XDC_AJOUT,...*/
    char        pl_NomProcs[80] ;
    
    XDY_ResConf vl_Resultat_out;
    
    XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MSE_XZAO422_730");
        
    strcpy( vl_NomBase,CGCDG_Base_Courante);    
    if ((vl_retcode =XZAO730_Ajouter_Eqt_SAGA(vl_NomBase,va_Eqt_in, &vl_Resultat_out))!=XDC_OK)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO422_730 a echoue sur ligne %ld",(long)va_Resultat_out);        
        vl_retcode_out = XDC_NOK;    
    }
    else
    /*A
    ** Test sur parametres de sortie de XZAO992_Ajouter_Equipement sur coherence base
    ** 4 valeurs de status d execution de la sp dans vl_Resultat_out
    ** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
    */
    {
        if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
        {
            strcpy(pl_NomProcs,"XZAO730_Ajouter_Eqt_SAGA");
            XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld",pl_NomProcs,pl_Msg2,(long)va_Resultat_out);
            
            sprintf(pl_Msg,"%s %s sur la ligne %ld",XDF_CONF_EQT_SAGA,pl_Msg2,(long)va_Resultat_out);    
            cuti_BilanConf_Trace(pl_Msg);    /* Notification dans Bilan config     */
            
        }
        else
        {
            XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO422_730 : cmvf_Trace_TYPE_Result a echoue");
            vl_retcode_out = XDC_NOK;
        }
    
        vl_retcode = vl_retcode_out;
    }    
    XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MSE_XZAO422_730 : sortie avec %d",vl_retcode_out);
    return(vl_retcode_out);
}

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*    cmvf_FctUti_MSE_XZAO816 : Fct utilisateur passee a xzao417_Lire_Config_Zon_PMV_SAGA
*  qui appelle XZAO416_Ajouter_Zon_PMV avec le nom de la base sur la quelle XZAO416_Ajouter_Zon_PMV_SAGA
*  va avoir une action.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cmvf_FctUti_MSE_XZAO816(    char         *va_autoroute_pmv,
                    XDY_Sens     va_sens_pmv,
                    XDY_PR        va_pr_pmv,
                    char        *va_site_pmv,
                    char        *va_autoroute_zone,
                    XDY_Sens    va_sens_zone,
                    XDY_PR        va_debut_zone,
                    XDY_PR        va_fin_zone,
                    char        *va_direction_zone,
                    XDY_Booleen    va_inhibition_zone,
                    XDY_PR        va_pr_1_pmv_amont,
                    XDY_Sens    va_sens_1_pmv_amont,
                    char        *va_autoroute_1_pmv_amont,
                    int        va_distance_1_pmv_amont,
                    XDY_PR        va_pr_1_pmv_amont_bis,
                    XDY_Sens    va_sens_1_pmv_amont_bis,
                    char        *va_autoroute_1_pmv_amont_bis,
                    int        va_distance_1_pmv_amont_bis,
                    XDY_PR        va_pr_1_pmv_amont_ter,
                    XDY_Sens    va_sens_1_pmv_amont_ter,
                    char        *va_autoroute_1_pmv_amont_ter,
                    int        va_distance_1_pmv_amont_ter,
                    XDY_Entier         va_Resultat_out) 
/*
* ARGUMENTS EN ENTREE :  infos de la zone d'influence PMV
*            va_Resultat_out contient le numero de ligne du fichier qui est en cours 
*            de traitement.
*                 
* ARGUMENTS EN SORTIE :  aucun
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
*
* CONDITION D'UTILISATION
*
* FONCTION 
*    Doit appeler XZAO403_Ajouter_Eqt_ZDP
*     
------------------------------------------------------*/
{
    static char *version     = "$Id: cgcd_mSGBDeq.c,v 1.33 2021/05/10 08:52:03 pc2dpdy Exp $ : cmvf_FctUti_MSE_XZAO417_416";
    char    vl_NomBase[4]    ;
    int     vl_retcode     = XDC_OK;
    int     vl_retcode_out     = XDC_OK;
    char      pl_Msg[500];            /* contient le mesg a tracer*/
    char      pl_Msg2[500];            /* contient modification si vl_Resultat_out XDC_AJOUT,...*/
    char      pl_NomProcs[80] ;
    
    XDY_ResConf vl_Resultat_out;
    
    XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MSE_XZAO417_416");
    
    strcpy( vl_NomBase,CGCDG_Base_Courante);    
    if ((vl_retcode =XZAO816_Ajouter_Zon_PMV_SAGA(    vl_NomBase,
                            va_autoroute_pmv,
                            va_sens_pmv,
                            va_pr_pmv,
                            va_site_pmv,
                            va_autoroute_zone,
                            va_sens_zone,
                            va_debut_zone,
                            va_fin_zone,
                            va_direction_zone,
                            va_inhibition_zone,
                            va_pr_1_pmv_amont,
                            va_sens_1_pmv_amont,
                            va_autoroute_1_pmv_amont,
                            va_distance_1_pmv_amont,
                            va_pr_1_pmv_amont_bis,
                            va_sens_1_pmv_amont_bis,
                            va_autoroute_1_pmv_amont_bis,
                            va_distance_1_pmv_amont_bis,
                            va_pr_1_pmv_amont_ter,
                            va_sens_1_pmv_amont_ter,
                            va_autoroute_1_pmv_amont_ter,
                            va_distance_1_pmv_amont_ter,
                            &vl_Resultat_out))!=XDC_OK)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO816 a echoue sur ligne %ld",(long)va_Resultat_out);        
        vl_retcode_out = XDC_NOK;    
    }
    else
    /*A
    ** Test sur parametres de sortie de XZAO416_Ajouter_Zon_PMV sur coherence base
    ** 4 valeurs de status d execution de la sp dans vl_Resultat_out
    ** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
    */
    {
        if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
        {
            strcpy(pl_NomProcs,"XZAO816");
            XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld",pl_NomProcs,pl_Msg2,(long)va_Resultat_out);
            
            sprintf(pl_Msg,"%s %s sur la ligne %ld",XDF_CONF_ZON_PMV_SAGA,pl_Msg2,(long)va_Resultat_out);    
            cuti_BilanConf_Trace(pl_Msg);    /* Notification dans Bilan config     */
            
        }
        else
        {
            XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO816 : cmvf_Trace_TYPE_Result a echoue");
            vl_retcode_out = XDC_NOK;
        }
    
        vl_retcode = vl_retcode_out;
    }    
    XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MSE_XZAO816 : sortie avec %d",vl_retcode_out);
   return(vl_retcode_out);
} 

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*    cmvf_FctUti_MSE_XZAO428_ZON_PMVA : Fct utilisateur passee a xzao428_Lire_Config_Zon_PMVA
*  qui appelle XZAO4290_Ajouter_Zon_PMVA avec le nom de la base sur la quelle XZAO4290_Ajouter_Zon_PMVA
*  va avoir une action.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cmvf_FctUti_MSE_XZAO830(    char         *va_autoroute_pmv,
                    XDY_Sens     va_sens_pmv,
                    XDY_PR        va_pr_pmv,
                    char        *va_site_pmv,
                    XDY_Entier   va_distance_pmv,
                    char        *va_autoroute_zone,
                    XDY_Sens    va_sens_zone,
                    XDY_PR        va_debut_zone,
                    XDY_PR        va_fin_zone,
                    char        *va_direction_zone,
                    XDY_Sens    va_type_zone,
                    XDY_Booleen    va_inhibition_zone,
                    XDY_Entier         va_Resultat_out) 
/*
* ARGUMENTS EN ENTREE :  infos de la zone d'influence PMV
*            va_Resultat_out contient le numero de ligne du fichier qui est en cours 
*            de traitement.
*                 
* ARGUMENTS EN SORTIE :  aucun
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
*
* CONDITION D'UTILISATION
*
* FONCTION 
*    Doit appeler XZAO403_Ajouter_Eqt_ZDP
*     
------------------------------------------------------*/
{
    static char *version     = "$Id: cgcd_mSGBDeq.c,v 1.33 2021/05/10 08:52:03 pc2dpdy Exp $ : cmvf_FctUti_MSE_XZAO428_ZON_PMVA";
    char    vl_NomBase[4]    ;
    int     vl_retcode     = XDC_OK;
    int     vl_retcode_out     = XDC_OK;
    char      pl_Msg[500];            /* contient le mesg a tracer*/
    char      pl_Msg2[500];            /* contient modification si vl_Resultat_out XDC_AJOUT,...*/
    char      pl_NomProcs[80] ;
    
    XDY_ResConf vl_Resultat_out;
    
    XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MSE_XZAO428_ZON_PMVA");
    
    strcpy( vl_NomBase,CGCDG_Base_Courante);    
    if ((vl_retcode = XZAO830_Ajouter_Zon_PMVA_SAGA(    vl_NomBase,
                            va_autoroute_pmv,
                            va_sens_pmv,
                            va_pr_pmv,
                            va_site_pmv,
                            va_distance_pmv,
                            va_autoroute_zone,
                            va_sens_zone,
                            va_debut_zone,
                            va_fin_zone,
                            va_direction_zone,
                            va_type_zone,
                            va_inhibition_zone,
                            &vl_Resultat_out))!=XDC_OK)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO830 a echoue sur ligne %ld",(long)va_Resultat_out);        
        vl_retcode_out = XDC_NOK;    
    }
    else
    /*A
    ** Test sur parametres de sortie de XZAO4290_Ajouter_Zon_PMVA sur coherence base
    ** 4 valeurs de status d execution de la sp dans vl_Resultat_out
    ** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
    */
    {
        if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
        {
            strcpy(pl_NomProcs,"XZAO830_Ajouter_Zon_PMVA_SAGA");
            XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld",pl_NomProcs,pl_Msg2,(long)va_Resultat_out);
            
            sprintf(pl_Msg,"%s %s sur la ligne %ld",XDF_CONF_ZON_PMVA_SAGA,pl_Msg2,(long)va_Resultat_out);    
            cuti_BilanConf_Trace(pl_Msg);    /* Notification dans Bilan config     */
            
        }
        else
        {
            XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO830 : cmvf_Trace_TYPE_Result a echoue");
            vl_retcode_out = XDC_NOK;
        }
    
        vl_retcode = vl_retcode_out;
    }    
    XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MSE_XZAO830 : sortie avec %d",vl_retcode_out);
    return(vl_retcode_out);
}

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*    cmvf_FctUti_MSE_XZAO953_952 : Fct utilisateur passee a xzao952_Lire_Config_ZoneUrbaine,
*  qui appelle XZAO953_Ajouter_ZoneUrbaine avec le nom de la base sur la quelle XZAO953_Ajouter_ZoneUrbaine 
*  va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cmvf_FctUti_MSE_XZAO953_952( XZAOT_ConfZoneUrbaine va_ZoneUrbaine_in, XDY_Entier     va_Resultat_out ) 
/*
* ARGUMENTS EN ENTREE :  va_EqtGTC_in contient le EqtGTC a rajouter
*            va_Resultat_out contient le numero de ligne du fichier qui est en cours 
*            de traitement.
*                 
* ARGUMENTS EN SORTIE :  aucun
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
*
* CONDITION D'UTILISATION
*
* FONCTION 
*    Doit appeler XZAO953_Ajouter_ZoneUrbaine
*     
------------------------------------------------------*/
{
    static char *version       = "$Id: cgcd_mSGBDeq.c,v 1.33 2021/05/10 08:52:03 pc2dpdy Exp $ : cmvf_FctUti_MSE_XZAO953_952";
    char        vl_NomBase[4]    ;
    int         vl_retcode     = XDC_OK;
    int         vl_retcode_out = XDC_OK;
    char        pl_Msg[500];     /* contient le mesg a tracer*/
    char        pl_Msg2[500];    /* contient modification si vl_Resultat_out XDC_AJOUT,...*/
    char        pl_NomProcs[80] ;
    
    XDY_ResConf vl_Resultat_out;
    
    XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MSE_XZAO953_952");
        
    strcpy( vl_NomBase,CGCDG_Base_Courante);    
    if ((vl_retcode =XZAO953_Ajouter_ZoneUrbaine(vl_NomBase,va_ZoneUrbaine_in, &vl_Resultat_out))!=XDC_OK)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO953_952 a echoue sur ligne %ld",(long)va_Resultat_out);        
        vl_retcode_out = XDC_NOK;    
    }
    else
    /*A
    ** Test sur parametres de sortie de XZAO953_Ajouter_ZoneUrbaine sur coherence base
    ** 4 valeurs de status d execution de la sp dans vl_Resultat_out
    ** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
    */
    {
        if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
        {
            strcpy(pl_NomProcs,"XZAO953_Ajouter_ZoneUrbaine");
            XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld",pl_NomProcs,pl_Msg2,(long)va_Resultat_out);
            
            sprintf(pl_Msg,"%s %s sur la ligne %ld",XDF_CONF_ZON_URB,pl_Msg2,(long)va_Resultat_out);    
            cuti_BilanConf_Trace(pl_Msg);    /* Notification dans Bilan config     */
            
        }
        else
        {
            XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO953_952 : cmvf_Trace_TYPE_Result a echoue");
            vl_retcode_out = XDC_NOK;
        }
    
        vl_retcode = vl_retcode_out;
    }    
    XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MSE_XZAO953_952 : sortie avec %d",vl_retcode_out);
    return(vl_retcode_out);
}

/*X*/
/*----------------------------------------------------
* SERVICE RENDU :
*    cmvf_FctUti_MSE_XZAO261_262 : Fct utilisateur passee a xzao261_Lire_Config_Stations_DIT,
*  qui appelle XZAO262_Ajouter_Station_DIT avec le nom de la base sur laquelle elle doit operer.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cmvf_FctUti_MSE_XZAO261_262 (XZAOT_Conf_Station_DIT va_Station_DIT_in, XDY_Entier va_Resultat_out)
/*
* ARGUMENTS EN ENTREE :
*            va_Station_DIT_in contient les donnees du point de mesure (station) a rajouter
*            va_Resultat_out contient le numero de ligne du fichier qui est en cours
*            de traitement.
*
* ARGUMENTS EN SORTIE :  aucun
*
* CODE RETOUR :
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
*
* CONDITION D'UTILISATION
*
* FONCTION
*    Doit appeler XZAO262_Ajouter_Station_DIT
*
------------------------------------------------------*/
{
    static char *version     = "$Id: cgcd_mSGBDeq.c,v 1.33 2021/05/10 08:52:03 pc2dpdy Exp $ : cmvf_FctUti_MSE_XZAO261_262";
    char    vl_NomBase[4]    ;
    int     vl_retcode     = XDC_OK;
    int     vl_retcode_out     = XDC_OK;
    char      pl_Msg[500];            /* contient le mesg a tracer*/
    char      pl_Msg2[500];            /* contient modification si vl_Resultat_out XDC_AJOUT,...*/
    char      pl_NomProcs[80] ;
 
    XDY_ResConf vl_Resultat_out;
 
    XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MSE_XZAO261_262");
 
    strcpy( vl_NomBase,CGCDG_Base_Courante);
    if ((vl_retcode = XZAO262_Ajouter_Station_DIT (vl_NomBase, va_Station_DIT_in, &vl_Resultat_out))!=XDC_OK)
    {
        XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO261_262 a echoue sur ligne %ld",(long)va_Resultat_out);
        vl_retcode_out = XDC_NOK;
    }
    else
    /*A
    ** Test sur parametres de sortie de la fonction appelee sur coherence base
    ** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
    */
    {
        if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
        {
            strcpy(pl_NomProcs,"XZAO262_Ajouter_Station_DIT");
            XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld",pl_NomProcs,pl_Msg2,(long)va_Resultat_out);

            sprintf(pl_Msg,"%s %s sur la ligne %ld",XDF_CONF_STATION_DIT,pl_Msg2,(long)va_Resultat_out);
            cuti_BilanConf_Trace(pl_Msg);    /* Notification dans Bilan config     */
        }
        else
        {
            XZST_03EcritureTrace(XZSTC_WARNING,"cmvf_FctUti_MSE_XZAO261_262 : cmvf_Trace_TYPE_Result a echoue");
            vl_retcode_out = XDC_NOK;
        }

        vl_retcode = vl_retcode_out;
    }
    XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MSE_XZAO261_262 : sortie avec %d",vl_retcode_out);
    return(vl_retcode_out);
}


/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*    cmvf_FctUti_MSE_XZAO258_CFE: Fct utilisateur passee a xzao259_Lire_Config_CFE,
*  qui appelle XZAO258_Ajouter_CFE avec le nom de la base sur la quelle XZAO258_Ajouter_CFE va avoir une action.
*  Cette fonction trace les erreurs dans le fichier Bilan config.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cmvf_FctUti_MSE_XZAO258_CFE ( XZAOT_ConfCFE va_CFE_in, XDY_Entier va_Resultat_out) 
/*
* ARGUMENTS EN ENTREE :  va_CFE_in contient le CFE a rajouter
*            va_Resultat_out contient le numero de ligne du fichier qui est en cours 
*            de traitement.
*                 
* ARGUMENTS EN SORTIE :  aucun
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
*
* CONDITION D'UTILISATION
*
* FONCTION 
*    Doit appeler XZAO258_Ajouter_CFE
*     
------------------------------------------------------*/
{
    static char *version     = "@(#)cgcd_mSGBDeq.c	1.26 1.26 06/02/18: cmvf_FctUti_MSE_XZAO258_CFE ";
    char    vl_NomBase[4]    ;
    int     vl_retcode     = XDC_OK;
    int     vl_retcode_out     = XDC_OK;
    char      pl_Msg[500];            /* contient le mesg a tracer*/
    char      pl_Msg2[500];            /* contient modification si vl_Resultat_out XDC_AJOUT,...*/
    char      pl_NomProcs[80] ;
    
    XDY_ResConf vl_Resultat_out;
    
    XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MSE_XZAO258_CFE ");
    XZST_03EcritureTrace(XZSTC_WARNING,"BASE(%s) nom(%s) auto(%s) PR(%d) sens(%d) adresse IP(%s) port(%d) nom site(%s)",
                CGCDG_Base_Courante,
                va_CFE_in.NomCFE,
                va_CFE_in.Autoroute,
                va_CFE_in.PR,
                va_CFE_in.Sens,
                va_CFE_in.AdresseIP,
                va_CFE_in.Port,
		va_CFE_in.NomSite
		);
    
    strcpy( vl_NomBase,CGCDG_Base_Courante);    
    if ((vl_retcode = XZAO258_Ajouter_CFE (vl_NomBase,va_CFE_in, &vl_Resultat_out))!=XDC_OK)
    {
        XZST_03EcritureTrace(XZSTC_WARNING," cmvf_FctUti_MSE_XZAO258_CFE a echoue sur ligne %ld",(long)va_Resultat_out);
        vl_retcode_out = XDC_NOK;    
    }
    else
    /*A
    ** Test sur parametres de sortie de XZAO258_Ajouter_CFE sur coherence base
    ** 4 valeurs de status d execution de la sp dans vl_Resultat_out
    ** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
    */
    {
        if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
        {
            strcpy(pl_NomProcs," cmvf_FctUti_MSE_XZAO258_CFE ");
            XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld",pl_NomProcs,pl_Msg2,(long)va_Resultat_out);
            
            sprintf(pl_Msg,"%s %s sur la ligne %ld",XDF_CONF_CFE,pl_Msg2,(long)va_Resultat_out);
            cuti_BilanConf_Trace(pl_Msg);    /* Notification dans Bilan config     */
            
        }
        else
        {
            XZST_03EcritureTrace(XZSTC_WARNING," cmvf_FctUti_MSE_XZAO258_CFE : cmvf_Trace_TYPE_Result a echoue");
            vl_retcode_out = XDC_NOK;
        }
    
        vl_retcode = vl_retcode_out;
    }    
    XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MSE_XZAO258_CFE: sortie avec %d",vl_retcode_out);
    return(vl_retcode_out);
}


int cmvf_FctUti_MSE_XZAO265_CAP ( XZAOT_ConfCAP        va_CAP_in, XDY_Entier     va_Resultat_out)
/*
* ARGUMENTS EN ENTREE :  va_CAP_in contient le CAP a rajouter
*            va_Resultat_out contient le numero de ligne du fichier qui est en cours 
*            de traitement.
*                 
* ARGUMENTS EN SORTIE :  aucun
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
*
* CONDITION D'UTILISATION
*
* FONCTION 
*    Doit appeler XZAO265_Ajouter_CAP
*     
------------------------------------------------------*/
{
    static char *version     = "@(#)cgcd_mSGBDeq.c      1.17 1.17 05/25/09: cmvf_FctUti_MSE_XZAO265_CAP ";
    char    vl_NomBase[4]    ;
    int     vl_retcode     = XDC_OK;
    int     vl_retcode_out     = XDC_OK;
    char      pl_Msg[500];            /* contient le mesg a tracer*/
    char      pl_Msg2[500];            /* contient modification si vl_Resultat_out XDC_AJOUT,...*/
    char      pl_NomProcs[80] ;

    XDY_ResConf vl_Resultat_out;

    XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MSE_XZAO265_CAP ");
    XZST_03EcritureTrace(XZSTC_WARNING,"BASE(%s) nom(%s) auto(%s) PR(%d) sens(%d) type(%d) rgs(%s) IP(%s) port maitre(%d) port esclave (%d) point carac(%d) serveur(%s) nom site(%s) ",
                CGCDG_Base_Courante,
                va_CAP_in.NomCAP,
                va_CAP_in.Autoroute,
                va_CAP_in.PR,
                va_CAP_in.Sens,
                va_CAP_in.Type,
                va_CAP_in.AdresseRgs,
                va_CAP_in.AdresseIP,
                va_CAP_in.PortMaitre,
                va_CAP_in.PortEsclave,
                va_CAP_in.PointCaracteristique,
                va_CAP_in.NomServeur,
                va_CAP_in.NomSite);

    strcpy( vl_NomBase,CGCDG_Base_Courante);
    if ((vl_retcode = XZAO265_Ajouter_CAP (vl_NomBase,va_CAP_in, &vl_Resultat_out))!=XDC_OK)
    {
        XZST_03EcritureTrace(XZSTC_WARNING," cmvf_FctUti_MSE_XZAO265_CAP a echoue sur ligne %ld",(long)va_Resultat_out);
        vl_retcode_out = XDC_NOK;
    }
    else
    /*A
    ** Test sur parametres de sortie de XZAO265_Ajouter_CAP sur coherence base
    ** 4 valeurs de status d execution de la sp dans vl_Resultat_out
    ** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
    */
    {
        if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
        {
            strcpy(pl_NomProcs," cmvf_FctUti_MSE_XZAO265_CAP ");
            XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld",pl_NomProcs,pl_Msg2,(long)va_Resultat_out);

            sprintf(pl_Msg,"%s %s sur la ligne %ld",XDF_CONF_CAP,pl_Msg2,(long)va_Resultat_out);
            cuti_BilanConf_Trace(pl_Msg);    /* Notification dans Bilan config     */

        }
        else
        {
            XZST_03EcritureTrace(XZSTC_WARNING," cmvf_FctUti_MSE_XZAO265_CAP : cmvf_Trace_TYPE_Result a echoue");
            vl_retcode_out = XDC_NOK;
        }

        vl_retcode = vl_retcode_out;
    }
    XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MSE_XZAO265_CAP: sortie avec %d",vl_retcode_out);
    return(vl_retcode_out);
}



int cmvf_FctUti_MSE_XZAO492_IMU ( XZAOT_ConfIMU        va_IMU_in, XDY_Entier     va_Resultat_out)
/*
* ARGUMENTS EN ENTREE :  va_IMU_in contient l IMU a rajouter
*            va_Resultat_out contient le numero de ligne du fichier qui est en cours 
*            de traitement.
*                 
* ARGUMENTS EN SORTIE :  aucun
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
*
* CONDITION D'UTILISATION
*
* FONCTION 
*    Doit appeler XZAO492_Ajouter_IMU
*     
------------------------------------------------------*/
{
    static char *version     = "@(#)cgcd_mSGBDeq.c      1.17 1.17 05/25/09: cmvf_FctUti_MSE_XZAO492_IMU ";
    char    vl_NomBase[4]    ;
    int     vl_retcode     = XDC_OK;
    int     vl_retcode_out     = XDC_OK;
    char      pl_Msg[500];            /* contient le mesg a tracer*/
    char      pl_Msg2[500];            /* contient modification si vl_Resultat_out XDC_AJOUT,...*/
    char      pl_NomProcs[80] ;

    XDY_ResConf vl_Resultat_out;

    XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MSE_XZAO492_IMU ");
    XZST_03EcritureTrace(XZSTC_WARNING,"BASE(%s) nom(%s) auto(%s) PR(%d) sens(%d) end point OPCUA(%s) user OPCUA(%s) password OPCUA (%s) BICE(%s) serveur(%s) nom site(%s) port maitre (%d)",
                CGCDG_Base_Courante,
                va_IMU_in.NomIMU,
                va_IMU_in.Autoroute,
                va_IMU_in.PR,
                va_IMU_in.Sens,
                va_IMU_in.AdresseEndPointOPCUA,
                va_IMU_in.UserOPCUA,
                va_IMU_in.PasswordOPCUA,
                va_IMU_in.NomBICE,
                va_IMU_in.NomServeur,
                va_IMU_in.NomSite,
		va_IMU_in.PortMaitre,
		va_IMU_in.SousType);

    strcpy( vl_NomBase,CGCDG_Base_Courante);
    if ((vl_retcode = XZAO492_Ajouter_IMU (vl_NomBase,va_IMU_in, &vl_Resultat_out))!=XDC_OK)
    {
        XZST_03EcritureTrace(XZSTC_WARNING," cmvf_FctUti_MSE_XZAO492_IMU a echoue sur ligne %ld",(long)va_Resultat_out);
        vl_retcode_out = XDC_NOK;
    }
    else
    /*A
    ** Test sur parametres de sortie de XZAO492_Ajouter_IMU sur coherence base
    ** 4 valeurs de status d execution de la sp dans vl_Resultat_out
    ** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
    */
    {
        if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
        {
            strcpy(pl_NomProcs," cmvf_FctUti_MSE_XZAO492_IMU ");
            XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld",pl_NomProcs,pl_Msg2,(long)va_Resultat_out);

            sprintf(pl_Msg,"%s %s sur la ligne %ld",XDF_CONF_IMU,pl_Msg2,(long)va_Resultat_out);
            cuti_BilanConf_Trace(pl_Msg);    /* Notification dans Bilan config     */

        }
        else
        {
            XZST_03EcritureTrace(XZSTC_WARNING," cmvf_FctUti_MSE_XZAO492_IMU : cmvf_Trace_TYPE_Result a echoue");
            vl_retcode_out = XDC_NOK;
        }

        vl_retcode = vl_retcode_out;
    }
    XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MSE_XZAO492_IMU: sortie avec %d",vl_retcode_out);
    return(vl_retcode_out);
}



int cmvf_FctUti_MSE_XZAO495_Zon_IMU ( XZAOT_Conf_Zon_IMU        va_ZoneIMU_in, XDY_Entier     va_Resultat_out)
/*
* ARGUMENTS EN ENTREE :  va_ZoneIMU_in contient la zone a rajouter
*            va_Resultat_out contient le numero de ligne du fichier qui est en cours
*            de traitement.
*
* ARGUMENTS EN SORTIE :  aucun
* CODE RETOUR :
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
*
* CONDITION D'UTILISATION
*
* FONCTION
*    Doit appeler XZAO495_Ajouter_Zon_IMU
*
------------------------------------------------------*/
{
    static char *version     = "@(#)cgcd_mSGBDeq.c      1.17 1.17 05/25/09: cmvf_FctUti_MSE_XZAO495_Zon_IMU ";
        char    vl_NomBase[4]    ;
	int     vl_retcode     = XDC_OK;
        int     vl_retcode_out     = XDC_OK;
	char      pl_Msg[500];            /* contient le mesg a tracer*/
        char      pl_Msg2[500];            /* contient modification si vl_Resultat_out XDC_AJOUT,...*/
	char      pl_NomProcs[80] ;

    XDY_ResConf vl_Resultat_out;

    XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MSE_XZAO495_Zon_IMU ");
    XZST_03EcritureTrace(XZSTC_WARNING,"BASE(%s) numero eqt(%d) id (%d) libelle (%s)",
		CGCDG_Base_Courante,
    		va_ZoneIMU_in.NumeroEqt,
    		va_ZoneIMU_in.Id,
    		va_ZoneIMU_in.Libelle);
    
    strcpy( vl_NomBase,CGCDG_Base_Courante);

    if ((vl_retcode = XZAO495_Ajouter_Zon_IMU (vl_NomBase, va_ZoneIMU_in, &vl_Resultat_out))!=XDC_OK)
    {
       XZST_03EcritureTrace(XZSTC_WARNING," cmvf_FctUti_MSE_XZAO495_Zon_IMU a echoue sur ligne %ld",(long)va_Resultat_out);
       vl_retcode_out = XDC_NOK;
    }

    else
    /*A
    ** Test sur parametres de sortie de XZAO495_Ajouter_Zon_IMU sur coherence base
    ** 4 valeurs de status d execution de la sp dans vl_Resultat_out
    ** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
    */
    {
       if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
       {
          strcpy(pl_NomProcs," cmvf_FctUti_MSE_XZAO495_Zon_IMU ");
	  XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld", pl_NomProcs, pl_Msg2,(long)va_Resultat_out);

	  sprintf(pl_Msg,"%s %s sur la ligne %ld", XDF_CONF_IMU_ZON, pl_Msg2,(long)va_Resultat_out);
	  cuti_BilanConf_Trace(pl_Msg);    /* Notification dans Bilan config     */

       }
       else
       {
          XZST_03EcritureTrace(XZSTC_WARNING," cmvf_FctUti_MSE_XZAO495_Zon_IMU : cmvf_Trace_TYPE_Result a echoue");
	  vl_retcode_out = XDC_NOK;
       }

       vl_retcode = vl_retcode_out;
    }

   XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MSE_XZAO495_Zon_IMU: sortie avec %d",vl_retcode_out);
   return(vl_retcode_out);

}


int cmvf_FctUti_MSE_XZAO498_Msg_IMU ( XZAOT_ConfMsgIMU        va_MsgIMU_in, XDY_Entier     va_Resultat_out)
/*
* ARGUMENTS EN ENTREE :  va_MsgIMU_in contient le message a rajouter
*            va_Resultat_out contient le numero de ligne du fichier qui est en cours
*            de traitement.
*
* ARGUMENTS EN SORTIE :  aucun
* CODE RETOUR :
*  XDC_OK si tout s est bien passe
*  sinon XDC_NOK.
*
*
* CONDITION D'UTILISATION
*
* FONCTION
*    Doit appeler XZAO498_Ajouter_Msg_IMU
*
------------------------------------------------------*/
{
    static char *version     = "@(#)cgcd_mSGBDeq.c      1.17 1.17 05/25/09: cmvf_FctUti_MSE_XZAO498_Msg_IMU ";
    	char    vl_NomBase[4]    ;
	int     vl_retcode     = XDC_OK;
	int     vl_retcode_out     = XDC_OK;
	char      pl_Msg[500];            /* contient le mesg a tracer*/
	char      pl_Msg2[500];            /* contient modification si vl_Resultat_out XDC_AJOUT,...*/
	char      pl_NomProcs[80] ;

    XDY_ResConf vl_Resultat_out;

    XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : cmvf_FctUti_MSE_XZAO498_Msg_IMU ");
    XZST_03EcritureTrace(XZSTC_WARNING,"BASE(%s) numero eqt(%d) libelle (%s)",
    	CGCDG_Base_Courante,
	va_MsgIMU_in.NumeroEqt,
	va_MsgIMU_in.Libelle);

    strcpy( vl_NomBase,CGCDG_Base_Courante);

    if ((vl_retcode = XZAO498_Ajouter_Msg_IMU (vl_NomBase, va_MsgIMU_in, &vl_Resultat_out))!=XDC_OK)
    {
       XZST_03EcritureTrace(XZSTC_WARNING," cmvf_FctUti_MSE_XZAO498_Msg_IMU a echoue sur ligne %ld",(long)va_Resultat_out);
       vl_retcode_out = XDC_NOK;
    }

    else
    /*A
    ** Test sur parametres de sortie de XZAO498_Ajouter_Msg_IMU sur coherence base
    ** 4 valeurs de status d execution de la sp dans vl_Resultat_out
    ** Si vl_Resultat_out == XDC_Echec, on retourne XDC_NOK sinon XDC_OK.
    */

    {
       if ((vl_retcode = cmvf_Trace_TYPE_Result(vl_Resultat_out,pl_Msg2,&vl_retcode_out))==XDC_OK)
       {
          strcpy(pl_NomProcs," cmvf_FctUti_MSE_XZAO498_Msg_IMU ");
	  XZST_03EcritureTrace(XZSTC_WARNING,"%s %s sur la ligne %ld", pl_NomProcs, pl_Msg2,(long)va_Resultat_out);

	  sprintf(pl_Msg,"%s %s sur la ligne %ld", XDF_CONF_IMU_MSG, pl_Msg2,(long)va_Resultat_out);
	  cuti_BilanConf_Trace(pl_Msg);    /* Notification dans Bilan config     */
       }
       
       else
       {
          XZST_03EcritureTrace(XZSTC_WARNING," cmvf_FctUti_MSE_XZAO498_Msg_IMU : cmvf_Trace_TYPE_Result a echoue");
	  vl_retcode_out = XDC_NOK;
       }

       vl_retcode = vl_retcode_out;
    }

    XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : cmvf_FctUti_MSE_XZAO498_Msg_IMU: sortie avec %d",vl_retcode_out);
    return(vl_retcode_out);

}
