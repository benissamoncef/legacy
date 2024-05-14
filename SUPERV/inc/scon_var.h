/* Fichier : @(#)scon_var.h	2.1      Release : 2.1        Date : 31 May 1994
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE scon_var * FICHIER scon_var.h
******************************************************
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale >
*
******************************************************
* HISTORIQUE :
*
* T.Milleville   30 Aou 1994	: Creation
* T.Milleville   09 Avr 1995 4	: Ajout Tache Shell Script V1.5
* T.Milleville   22 Aou 1996	: Ajout PC simplifie v1.6
* P.Niepceron	 15 Dec 1995	: Ajout PC2 + STA v1.7
* P.Niepceron   01 Jul 1998     : Ajout du type de machine Historique (dem/1697) v1.8
******************************************************/

#ifndef scon_var
#define scon_var

/* declarations de variables globales */

extern Widget BDCON1;
extern Widget BDCON1_FM;
extern Widget BDCON1_LA;
extern Widget BDCON2;
extern Widget BDCON2_FM;
extern Widget BDCON2_LA;
extern Widget BDTAS;
extern Widget BDTAS_FM;
extern Widget BDTAS_CH_SSY;
extern Widget BDTAS_CH_TAC;
extern Widget BDTAS_TO_ST;
extern Widget BDTAS_TO_STA;
extern Widget BDTAS_TO_HIS;
extern Widget BDTAS_TO_SD;
extern Widget BDTAS_TO_SC;
extern Widget BDTAS_TO_LT;
extern Widget BDTAS_TO_SP;
extern Widget BDTAS_TO_PCS;
extern Widget BDTAS_TO_PC2;
extern Widget BDTAS_TO_PC2LT;
extern Widget BDENT;
extern Widget BDENT_FM;
extern Widget BDENT_LA_SI;
extern Widget BDENT_CH_SI;
extern Widget BDENT_TO_PER;
extern Widget BDENT_TO_INT;
extern Widget BDENT_TO_CON;
extern Widget BDENT_TO_TAC;
extern Widget BDENT_LA_TA;
extern Widget BDENT_CH_TA;
extern Widget BDENT_LA_CA;
extern Widget BDENT_CH_CA;
extern Widget BDENT_LA_EN;
extern Widget BDENT_CH_EN;
extern Widget BDCJA;
extern Widget BDCJA_FM;
extern Widget BDCJA_CH_TAI;
extern Widget BDERR;
extern Widget BDERR_FM; 
extern Widget BDERR_LA; 
extern Widget BDSSY;
extern Widget BDSSY_FM;
extern Widget BDSSY_CH_SSY;
extern Widget BDSSY_TO_ST;
extern Widget BDSSY_TO_STA;
extern Widget BDSSY_TO_HIS;
extern Widget BDSSY_TO_SD;
extern Widget BDSSY_TO_SC;
extern Widget BDSSY_TO_LT;
extern Widget BDSSY_TO_SP;
extern Widget BDSSY_TO_PCS;
extern Widget BDSSY_TO_PC2;
extern Widget BDSSY_TO_PC2LT;
extern Widget BDCAL;
extern Widget BDCAL_FM;
extern Widget BDCAL_CH_CAL;
extern Widget BDSIT;
extern Widget BDSIT_FM;
extern Widget BDSIT_CH_SIT;
extern Widget BDCTM;
extern Widget BDCTM_FM;
extern Widget BDCTM_CH_CAL;
extern Widget BDCTM_CH_TAC;
extern Widget BDCMA;
extern Widget BDCOMA;
extern Widget BDCMA_FM;
extern Widget BDCOMA_FM;
extern Widget BDCMA_CH_CAL;
extern Widget BDCOMA_CH_CAL_DEST;
extern Widget BDCOMA_CH_CAL_EXPED;
extern Widget BDTAC;
extern Widget BDTAC_FM;
extern Widget BDTAC_CH_TAC;
extern Widget BDTAC_CH_LRT;
extern Widget BDTAC_CH_LRL;
extern Widget BDTAC_CH_TAI;
extern Widget BDMAC;
extern Widget BDMAC_FM;
extern Widget BDMAC_CH_IMP;
extern Widget BDMAC_CH_MIM;
extern Widget BDMAC_CH_IMPSEC;
extern Widget BDMAC_CH_TRA;
extern Widget BDMAC_CH_MTR;
extern Widget BDMAC_CH_CHE;
extern Widget BDMAC_CH_TAI;
extern Widget BDMAC_CH_PER;
extern Widget BDMAC_CH_DAB;
extern Widget BDMAC_CH_ECR;
extern Widget BDMAC_CH_MAC;
extern Widget BDMAC_CH_SIT;
extern Widget BDMAC_CH_PSR;
extern Widget BDMAC_TO_ST;
extern Widget BDMAC_TO_STA;
extern Widget BDMAC_TO_HIS;
extern Widget BDMAC_TO_SD;
extern Widget BDMAC_TO_SC;
extern Widget BDMAC_TO_SP;
extern Widget BDMAC_TO_LT;
extern Widget BDMAC_TO_PCS;
extern Widget BDMAC_TO_PC2;
extern Widget BDMAC_TO_PC2LT;
extern Widget BDMAC_TO_OUI;
extern Widget BDMAC_TO_NON;
extern Widget CONFM;
extern Widget CONFM_FM;
extern Widget Radio_TypeTache;
extern Widget Tache_Temporaire;
extern Widget Tache_Permanente;
extern Widget Tache_ShellScript;
extern Widget Type_Profil;
extern Widget T_PRO_NUL;
extern Widget T_PRO_ADM;
extern Widget T_PRO_GIT;
extern Widget T_PRO_OTR;

extern int vg_operation;

#endif
