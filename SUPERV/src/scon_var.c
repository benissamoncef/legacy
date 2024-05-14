/*E*/
/* Fichier : $Id: scon_var.c,v 1.9 1998/09/22 13:59:13 gaborit Exp $      Release : $Revision: 1.9 $        Date : $Date: 1998/09/22 13:59:13 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE scon_var * FICHIER scon_var.c
******************************************************
* DESCRIPTION DU MODULE :
*
* Declaration globale des Widgets utilisees dans la
*	tache tsconf.x
*
******************************************************
* HISTORIQUE :
*
* T.Milleville   30 Aou 1994	: Creation
* T.Milleville   09 Avr 1995 	: Ajout Tache Shell Script V1.5
* T.Milleville   25 Jul 1995 	: Mise a jour entete V1.6
* P.Niepceron    22 Aou 1995 	: Ajout PC simplifie v1.7
* P.Niepceron    16 Dec 1997	: Ajout PC 2 v1.8
* P.Niepceron   01 Jul 1998     : Ajout du type de machine Historique (dem/1697) v1.9
*******************************************************/

/* fichiers inclus */

#include "scon_ser.h"
#include "scon_var.h"

/* declaration de variables globales */

static char *version = "$Id: scon_var.c,v 1.9 1998/09/22 13:59:13 gaborit Exp $ : scon_var" ;

Widget BDCON1 = (Widget) NULL;
Widget BDCON1_FM = (Widget) NULL;
Widget BDCON1_LA = (Widget) NULL;
Widget BDCON2 = (Widget) NULL;
Widget BDCON2_FM = (Widget) NULL;
Widget BDCON2_LA = (Widget) NULL;
Widget BDTAS = (Widget) NULL;
Widget BDTAS_FM = (Widget) NULL;
Widget BDTAS_CH_SSY = (Widget) NULL;
Widget BDTAS_CH_TAC = (Widget) NULL;
Widget BDTAS_TO_ST = (Widget) NULL;
Widget BDTAS_TO_STA= (Widget) NULL;
Widget BDTAS_TO_HIS= (Widget) NULL;
Widget BDTAS_TO_SD = (Widget) NULL;
Widget BDTAS_TO_SC = (Widget) NULL;
Widget BDTAS_TO_LT = (Widget) NULL;
Widget BDTAS_TO_SP = (Widget) NULL;
Widget BDTAS_TO_PCS = (Widget) NULL;
Widget BDTAS_TO_PC2 = (Widget) NULL;
Widget BDTAS_TO_PC2LT= (Widget) NULL;
Widget BDENT = (Widget) NULL;
Widget BDENT_FM = (Widget) NULL;
Widget BDENT_LA_SI = (Widget) NULL;
Widget BDENT_CH_SI = (Widget) NULL;
Widget BDENT_TO_PER = (Widget) NULL;
Widget BDENT_TO_INT = (Widget) NULL;
Widget BDENT_TO_CON = (Widget) NULL;
Widget BDENT_TO_TAC = (Widget) NULL;
Widget BDENT_LA_TA = (Widget) NULL;
Widget BDENT_CH_TA = (Widget) NULL;
Widget BDENT_LA_CA = (Widget) NULL;
Widget BDENT_CH_CA = (Widget) NULL;
Widget BDENT_LA_EN = (Widget) NULL;
Widget BDENT_CH_EN = (Widget) NULL;
Widget BDCJA = (Widget) NULL;
Widget BDCJA_FM = (Widget) NULL;
Widget BDCJA_CH_TAI = (Widget) NULL;
Widget BDERR = (Widget) NULL;
Widget BDERR_FM = (Widget) NULL;
Widget BDERR_LA = (Widget) NULL;
Widget BDSSY = (Widget) NULL;
Widget BDSSY_FM = (Widget) NULL;
Widget BDSSY_CH_SSY = (Widget) NULL;
Widget BDSSY_TO_ST = (Widget) NULL;
Widget BDSSY_TO_STA = (Widget) NULL;
Widget BDSSY_TO_HIS = (Widget) NULL;
Widget BDSSY_TO_SD = (Widget) NULL;
Widget BDSSY_TO_SC = (Widget) NULL;
Widget BDSSY_TO_LT = (Widget) NULL;
Widget BDSSY_TO_SP = (Widget) NULL;
Widget BDSSY_TO_PCS = (Widget) NULL;
Widget BDSSY_TO_PC2 = (Widget) NULL;
Widget BDSSY_TO_PC2LT = (Widget) NULL;
Widget BDCAL = (Widget) NULL;
Widget BDCAL_FM = (Widget) NULL;
Widget BDCAL_CH_CAL = (Widget) NULL;
Widget BDSIT = (Widget) NULL;
Widget BDSIT_FM = (Widget) NULL;
Widget BDSIT_CH_SIT = (Widget) NULL;
Widget BDCTM = (Widget) NULL;
Widget BDCTM_FM = (Widget) NULL;
Widget BDCTM_CH_CAL = (Widget) NULL;
Widget BDCTM_CH_TAC = (Widget) NULL;
Widget BDCMA = (Widget) NULL;
Widget BDCOMA = (Widget) NULL;
Widget BDCMA_FM = (Widget) NULL;
Widget BDCOMA_FM = (Widget) NULL;
Widget BDCMA_CH_CAL = (Widget) NULL;
Widget BDCOMA_CH_CAL_DEST = (Widget) NULL;
Widget BDCOMA_CH_CAL_EXPED = (Widget) NULL;
Widget BDTAC = (Widget) NULL;
Widget BDTAC_FM = (Widget) NULL;
Widget BDTAC_CH_TAC = (Widget) NULL;
Widget BDTAC_CH_LRT = (Widget) NULL;
Widget BDTAC_CH_LRL = (Widget) NULL;
Widget BDTAC_CH_TAI = (Widget) NULL;
Widget BDMAC = (Widget) NULL;
Widget BDMAC_FM = (Widget) NULL;
Widget BDMAC_CH_IMP = (Widget) NULL;
Widget BDMAC_CH_MIM = (Widget) NULL;
Widget BDMAC_CH_TRA = (Widget) NULL;
Widget BDMAC_CH_MTR = (Widget) NULL;
Widget BDMAC_CH_CHE = (Widget) NULL;
Widget BDMAC_CH_TAI = (Widget) NULL;
Widget BDMAC_CH_PER = (Widget) NULL;
Widget BDMAC_CH_DAB = (Widget) NULL;
Widget BDMAC_CH_ECR = (Widget) NULL;
Widget BDMAC_CH_MAC = (Widget) NULL;
Widget BDMAC_CH_SIT = (Widget) NULL;
Widget BDMAC_CH_PSR = (Widget) NULL;
Widget BDMAC_TO_ST = (Widget) NULL;
Widget BDMAC_TO_STA = (Widget) NULL;
Widget BDMAC_TO_HIS = (Widget) NULL;
Widget BDMAC_TO_SD = (Widget) NULL;
Widget BDMAC_TO_SC = (Widget) NULL;
Widget BDMAC_TO_SP = (Widget) NULL;
Widget BDMAC_TO_LT = (Widget) NULL;
Widget BDMAC_TO_PCS = (Widget) NULL;
Widget BDMAC_TO_PC2 = (Widget) NULL;
Widget BDMAC_TO_PC2LT = (Widget) NULL;
Widget BDMAC_TO_OUI = (Widget) NULL;
Widget BDMAC_TO_NON = (Widget) NULL;
Widget CONFM = (Widget) NULL;
Widget CONFM_FM = (Widget) NULL;
Widget Radio_TypeTache = (Widget) NULL;
Widget Tache_Temporaire = (Widget) NULL;
Widget Tache_Permanente = (Widget) NULL;
Widget Tache_ShellScript = (Widget) NULL;
Widget BDMAC_CH_IMPSEC = (Widget) NULL;
Widget Type_Profil = (Widget) NULL;
Widget T_PRO_NUL = (Widget) NULL;
Widget T_PRO_ADM = (Widget) NULL;
Widget T_PRO_GIT = (Widget) NULL;
Widget T_PRO_OTR = (Widget) NULL;

int vg_operation;

