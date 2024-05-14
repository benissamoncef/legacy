/*E*/
/*  Fichier : $Id: xdm.h,v 1.152 2021/06/10 08:48:43 pc2dpdy Exp $      Release : $Revision: 1.152 $        Date : $Date: 2021/06/10 08:48:43 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE xdm * FICHIER xdm.h
******************************************************
* DESCRIPTION DU MODULE :
*
*       Fichier definissant les identifiants des
*       messages RTWorks
*
******************************************************
* HISTORIQUE :
*
* T.Millevillei 23 Aou 1994     : Creation
********************************
* Mismer.D	24 Aou 1994     : Ajout Num Msg EQUEXT
********************************
* T.Milleville  29 Sep 1994	: Ajout Num Msg XDM_TST_TSUP	V 1.10
********************************
* D.Mismer	30 Sep 1994 	: Ajout des Msg 
*				XDM_EMES_6mn_Voie, XDM_EMES_6mn_Chaussee,
*				XDM_EMES_6gl_Voie, XDM_EMES_6gl_Chaussee
********************************
* T.Milleville  30 Sep 1994     : Ajout des Msg
*				XDM_TSG_VERIF, XDM_TSG_INIT
*                               XDM_TSE_APPLI, XDM_TSE_RES, XDM_TSE_TOUT
*                               XDM_TSE_VIE, XDM_TSE_SURV, XDM_TSE_DG_STATUS
*
********************************
* Mercier O.     4 Oct 1994	: Ajout de OFFSET_BAS V 1.15
*				XDM_AT_TUBE,XDM_AT_echangeur,
*				XDM_AT_NAV,XDM_AT_PMV
*
********************************
* T.Milleville  07 Oct 1994     : Destruction du Msg XDM_TSP_VLOG
*
********************************
* I.Thomas	10 Oct 1994	: Ajout de XDM_DIN_PA
*
********************************
* T.Milleville  11 Oct 1994     : Ajout du Msg XDM_TSP_ETAT  V 1.19
*
********************************
* Niepceron     12 Oct 1994     : Modification des XDM_APPLIX_xxxx
*                                        par   des XDM_IAX_xxxx      V 1.20
********************************
* Mercier O.    12 Oct 1994	: Modification du nom de XDM_CGD_*   V 1.22
*		XDM_CGD_LIV, XDM_CGD_DIF, XDM_CGD_MBD, XDM_CGD_VRF, 
*		XDM_CGD_DBD
********************************
* Nagiel E.	12 Oct 1994	: Ajout de XDM_ETAT_Contraste 	     V 1.23
********************************
* Mismer D.	13 Oct 1994	: Ajout de XDM_ETAT_JourNuit 	     V 1.26
********************************
* Mismer D.	14 Oct 1994	: Ajout de XDM_EInit_IHM_Classe_DIV  V 1.27
********************************
* Niepceron     18 Oct 1994     : Ajout de XDM_IRAF_DAI1 XDM_IRAF_DAI2 
*                                 XDM_IRAF_DAI3 XDM_IRAF_DAI4 XDM_IRAF_DAI5 V1.28
********************************
* Niepceron     19 Oct 1994     : Ajout de XDM_IETAT_DISPO_EQT  XDM_IRAF_BOUCHON_CHAUSSEE_PORTION 
*                                 XDM_IRAF_VOIE_NEUTRALISEE XDM_IRAF_VOIE_RADT_INVERSE
*                                 XDM_IRAF_BASCULEMENT_BOUCHON_VOIE XDM_IRAF_OBJET_MOBILE_PORTION
*                                 XDM_IRAF_OBJET_MOBILE_PORT_AFF_VOIES       V1.29
*
* T.Milleville  20 Oct 1994     : Ajout des Msg  V 1.30
*			XDM_ADM_SSS, XDM_ADM_TAC, XDM_ADM_ARC_SER,
*                       XDM_ADM_ARC_PER, XDM_ADM_ARC_INT,
*                       XDM_ADM_TRA, XDM_ADM_ARC_STA_M,
*                       XDM_ADM_ARC_STA_E, XDM_ADM_CON, XDM_ADM_RAF
********************************
* Niepceron     20 Oct 1994     : Modif de XDM_IRAF_OBJET_MOBILE_PORT_AFF_VOIES 
*                                    XDM_IRAF_PORT_AFF_VOIES V 1.31
********************************
* Niepceron     21 Oct 1994     : Ajout de XDM_IPOSIT_INDIC_ALARME V 1.32 
*
********************************
* MERCIER O.	24 Oct 1994	: Ajout de XDM_AA_alerte	V 1.33
*
********************************
* MERCIER O.	24 Oct 1994	: Ajout de XDM_AM_alarme	V 1.34
********************************
* Niepceron     26 Oct 1994     : Ajout de XDM_IRAF_POURCENT     V 1.35
*
* T.Milleville  20 Oct 1994     : Ajout des Offsets  V 1.36
*					OFFSET_ARC, OFFSET_AGL
********************************
* Mercier O.	28 Oct 1994	: Ajout de XDM_A_INIT_IHM	V1.38
********************************
* Niepceron     31 Oct 1994     : Ajout de XDM_ISYN_SERVEUR     	V 1.39
********************************
* MERCIER O.	2 Nov  1994	: Ajout de XDM_A_TIMER_RAFRAI_IHM	V 1.40
********************************
* MERCIER O.	7 Nov  1994	: Ajout de XDM_AN_PURGE			V 1.41
********************************
* MERCIER O.	7 Nov  1994	: Ajout de XDM_AG_PURGE			V 1.42
*					   XDM_AG_PURGEHISTO
********************************
* Niepceron     15 Nov 1994     : Ajout de XDM_IDONNEES_FT     	V 1.44
********************************
* MERCIER O.    15 Nov 1994     : Ajout de XDM_AS_FdeT     		V 1.45
********************************
* Niepceron     15 Nov 1994     : Ajout de XDM_IRAF_COURBE    	V 1.46
********************************
* Niepceron     16 Nov 1994     : Correction de XDM_AS_FdeT    	V 1.47
********************************
* NAGIEL        17 Nov 1994     :Ajout de  XDM_ECMD_MAT_NAV   	V 1.48
******************************************************
* NIEPCERON  	25 Nov 1994	: Ajout de  XDM_IDONNEES_INRETS_PR
*					    XDM_IDONNEES_INRETS_TRA  	V 1.49
******************************************************
* MISMER  	28 Nov 1994	: Ajout de  XDM_ETAT_GTC_NICE	V 1.51
********************************
* Lecrivain     28 Nov 1994     : Ajout de XDM_ICREER_ALERTE		V 1.52
******************************************************
* NIEPCERON  	29 Nov 1994	: Ajout de  XDM_IDONNEES_BOUCHONS  	V 1.54
******************************************************
* NIEPCERON  	29 Nov 1994	: Ajout de  XDM_IDONNEES_QVTO	  	V 1.55
******************************************************
* MISMER  	02 Dec 1994	: Modif XDM_ETAT_SEQUENCE		V 1.56
*
* T.Milleville  5 Dec 1994     : Ajout des Msg  V 1.56
*			XDM_ADM_REINIT
*
* I.Thomas	14 Dec 1994	: Ajout de XDM_DIN_BDC
*
* volcic	21 Dec 1994	: Ajout de XDM pour TERDT (v 1.59)
****************
* MERCIER	22 Dec 1994	: Ajout de XDM_CGD_CANAL 		V 1.60
****************
* MERCIER	23 Dec 1994	: Ajout de XDM_CGD_CHECK 		V 1.61
*
********************************
* Niepceron     23 Dec 1994     : Ajout de  XDM_IRAF_OBJET_MOBILE_ECHANG V1.62
****************
* MERCIER	2  Jan 1995	: Ajout de XDM_CGD_CHGB			V 1.63
********************************
* Niepceron     03 Jan 1995     : Ajout de  XDM_ISYN_SERVEURCV V1.64
********************************
* Niepceron     03 Jan 1995     : Ajout de  XDM_ISYN_SERVEURCV V1.64
********************************
* Gabert P      05 Jan 1995     : Ajout de  XDM_ECMD_FIC V1.65 V1.66
********************************
* Gabert P      05 Jan 1995     : Modif de  XDM_ECMD_FIC V1.67
********************************
* MERCIER	06 Jan 1995	: Ajout de XDM_CSV_CANAL	V1.68
********************************
* Gabert P      05 Jan 1995     : Modif de  XDM_ECMD_FIC V1.67
********************************
* Gaborit       11 Jan 1995     : Ajout de XDM_AH_INITLT et XDM_AH_RADT_DAI
*********************************
* Niepceron     12 Jan 1995     : Ajout de XDM_IDONNEES_RETARD  V1.72
******************************************************
* Lecrivain     12 Jan 1995     : Ajout de XDM_IARCHIVAGE	V1.73
********************************
* Gaborit       14 Jan 1995     : Ajout de XDM_AS_INRETS et XDM_AS_BOUCHON (1.74)
*
* T.Milleville  16 Jan 1995     : Ajout de XDM_TST_TESTDISK  V1.75
*
********************************
* Gaborit       16 Jan 1995     : Ajout de XDM_AH_SATIR (1.76)
*
* T.Milleville  16 Jan 1995     : Ajout de XDM_TSE_INDQUA  V1.77
*
* T.Milleville  16 Jan 1995     : Ajout de XDM_TSE_SYNCDISK  V1.78
*
* T.Milleville  25 Jan 1995     : Ajout de XDM_TST_DATASRV     V1.79
*
* Gaborit       26 Jan 1995     : Ajout de XDM_AEQA (1.81)
*
* volcic	17 Fev 1995	: Ajout de XDM_EInit_OP_VW, XDM_MODE_LENT (v 1.82)
*
* Torregrossa   17 fev 1995     : Ajout de XDM_AT_INI_EQT, XDM_AT_INI_IHM (1.83)
* Mismer   	23 fev 1995     : Ajout de XDM_ETAT_MAT_NAV (1.84)
* Volcic   	15 Mar 1995     : Ajout de XDM_ETAT_LIAISON et XDM_EInit_IHM_Liaison (1.85)
* T.Milleville  09 Avr 1995      : Ajout de XDM_TSP_STARTTMP      V1.86
* T.Milleville  09 Avr 1995      : Ajout de XDM_TSE_STARTTMP      V1.87
* T.Milleville  03 Mai 1995      : Ajout de XDM_TSP_MIGRAZUR      V1.88
* Niepceron     05 Mai 1995      :   XDM_IRAF_COURBE devient  XDM_IRAF_COURBE0 et  XDM_IRAF_COURBE1  V1.89
* Niepceron     23 Mai 1995      :  Ajout de  XDM_IRAZ_MOB  V1.90
* Niepceron	06 Jun 1995   	: Ajout XDM_IREI_EQT pour la reinitialisation des eqt  v1.91
* Mismer	19 Jun 1995   	: Ajout XDM_EMOD_DEG pour la surveillance des LT       v1.92
* Volcic   	01 Aou 1995     : Ajout de XDM_Acknoledge v1.93)
* Mismer	21 Sep 1995   	: Ajout XDM_EInit_MAT_NAV et 	XDM_Acknoledge_Ala     v1.94
* GABORIT	27 Dec 1995	: Ajout du ss-systeme MAINTE v1.95
* Niepceron     13 Fev 1995	: Ajout XDM_TSP_LTD v1.96
* Guilhou	22 Aou 1996	: Ajout messages EXTENSION OUEST v1.96
* Niepceron     27 Aou 1996	: Ajout de XDM_IAX_INITPRES v1.98
* Mismer	16 Sep 1996   	: Ajout XDM_EInit_IHM_6mn_parVoie et XDM_EMES_6mn_parVoie (RADT_W) v1.99
* Niepceron     27 Sep 1996	: Ajout de XDM_IAX_ACQT (DEM/424) v1.100
* Mismer	04 Oct 1996 	: Ajout XDM_EMES_PESAGE et XDM_Acknoledge_Pesage pour pesage RADT (DEM/1227) (1.101)
* Mismer	28 Oct 1996 	: Ajout XDM_INFO_PAU pour alerte PAU HS (DEM/1282) (1.102)
* Orengo.A	12 Nov 1996	: Ajout XDM_DONNEES_METEO_QTX et XDM_ACQ_DONNEES_METEO_QTX pour envoi
*					et l'acquitement des donnees meteo Tache teqtx (DEM/1232) 
*					et XDM_ECMD_GTC pour alerte DAI tunnel (DEM/12..)  	(v 1.102)
* Niepceron     03 Fev 1997	: Ajout de XDM_AS_TDP, XDM_IRAF_COURBE2 XDM_IDONNEES_TDP
					XDM_ETDP_SEUIL XDM_CR_TDP TDP (v 1.103)
* Vathelot	29 Avr 1997	: Ajout XDM_AS_SERV_TRAFIC et XDM_AS_PURGE_TRAFIC (Dem Strada) v1.104
* Penot		05 Mai 1997	: Ajout XDM_AS_CLT_RADT (Dem Strada) v1.105
* Guilhou	05 mai 1997	: ajout XDM_IDONNEES_TRAFIC_STRADA	1.106
*				  ajout XDM_AS_TIMER_SRVEVT_STRADA
* Penot         13 Mai 1997     : Ajout XDM_AS_CLT_EVT (Dem Strada) v1.107
* Guilhou	04 nov 1997	: ajout XDM_AS_EVT_STRADA (dem strada) 1.108
* Penot		07 Nov 1997	: ajout XDM_IEVT_STRADA
* Penot         07 Nov 1997     : ajout XDM_IAEVT_STRADA (dem strada) 1.109
* Mismer	19 Jan 1998 	: Ajout XDM_ECMD_PAL XDM_EInit_IHM_PAL XDM_ETAT_PAL pour PAL (DEM/1539) (1.110)
* Niepceron     05 Mai 1998	:Ajout  XDM_A_TIMER_FMC_LOC v1.111
* Niepceron     19 Nov 1998 	: Ajout XDM_AG_IMPCAT, XDM_AG_EXPCAT, XDM_IPMV_DATEX v1.112 dem/1723
* Guilhou	16 jan 2001	: ajout XDM_AW_TIMER 1.113
* JMG		7 dec 2001	: ajout XDM_ISYN_NOMADE_VUE, XDM_ISYN_NOMADE_MASK 1.114
* JMG		7 dec 2001	: ajout XDM_NOMADE_TIMER, XDM_NOMADE_REVEIL  1.115
* ETR		18 jui 2002     : ajout XDM_AMISOL_TIMER 	
* JMG		16/11/04	: ajout XDM_EPAL_AUTO et XDM_EPAL_AUTO_FIN
* PNI		11/03/05	: ajout de XDM_IRAF_BOUCHON_CHAUSSEE_VOIE 1.116 DEM444
* JMG		13/09/05	: ajout PMVA BAF BAD 1.117
* JMG		13/09/05	: ajout PMVA BAF BAD 1.118
* JMG		28/09/05	: ajout PMVA BAF BAD 1.119
* JMG		14/11/05	: ajout XDM_ENAV_AUTO et XDC_ENAV_AUTO_FIN 1.120
* PNI		25/10/05	: ajout XDM_EBAF_AUTO,... 1.121 DEM584
* AAZ       30/08/2007  : ajout GTC rénovée DEM 662
* JMG		15/09/08 	: ajout XDM_AS_SRVEVT_STRADA_TDP XDM_ATDP_TIMER XDM_AS_SRVEVT_STRADA_RDT
*					XDM_AS_SRVEVT_STRADA_FMC DEM / 835 
* JMG		28/09/09	: secto DEM887
* JPL		15/11/10	: Commentaires pour compilation sur architecture HP ia64 (DEM 961)  1.125
* JMG		22/10/10  : ajout messages synoptiques flash + cmd radio/tel 1.126
* JMG		18/03/11: ajout XDM_AA_alerte_localisee  et XDM_ICREER_ALERTE_LOCALISEE  1.127
* JPL		21/12/11	: Ajout XDM_ITDP_TIMER pour tempo. diffusion messages TdP radio (DEM 1007)  1.128
* SDU		20/02/12	: Ajout Communautaire (DEM/1015) 1.129
* VR		29/05/12	: ajout PRV DEM/1016 
* MG		08/06/12 : Ajout messages XDM_ETDP_IHM_PMVA XDM_CR_TDP_PMVA XDM_VAL_TDP_PMVA (DEM/1014 PMA)
* JPL		29/01/13	: Ajout XDM_ANEUTR_TIMER pour tempo. mise au neutre PMV[A] (DEM 1061)  1.133
* PNI		15/12/14	: Ajout XDM_TSE_KEEPALIVE pour activation socket DEM1116 1.134
* JMG		16/04/16 : ajout messages SAGA et XDM_ECMD_BASC_RAU 1.135
* JMG		11/12/17	ajout polyvox 1.136
* JMG		15/01/18	ajout XDM_ETAT_CAB GABEB 1.137
* JMF		15/01/18	ajout XDG DETECTION GABEB 1.138
* LCL		22/02/18	ajout CTRL_FEUX 1.139 DEM1284
* LCL		11/05/18	modification XDM_ETAT_CAB eb XDM_ETAT_CAP 1.140 DEM1283
* JMG		29/10/18	ajout msg RMVI DE%1315 1.141
* JMG		30/11/18	ajout msg AVA 1.142
* JMG		12/06/19	ajout msg ODI_C DEM1338 1.143
* LCL		11/09/19	ajout msg COCKPIT DEM1351 1.144
* JMg		12/11/19	msg ETDP_SURCHARGE DEM1365 1.145
* JMG		10/01/20	IMU 1.146
* ABE 		30/03/20	ajout Datex2 DEM 130 1.147
* GR		30/07/20        ajout EMSG_TDP_FCD DEM-SAE152 1.148	
* ABE 		30/08/20	ajout Datex2 timer situation recap DEM 130 1.148
* ABE		15/09/20	ajout Datex2 timer situation coup par coup DEM-SAE130 1.149
* ABE		15/12/20	ajout XDM pour IMU DEM-SAE155 1.150
* jmg		10/02/21 	ajout XDM_API_POKE MAGISTRA 1.150
* ABE		02/02/21	ajout PAU_SONO DEM-SAE244 1.151
* JMG		22/06/21	ajout XDM_API_SYNOS 1.152
* LCL		24/10/21	ajout XDM_INIT_TISSE 1.153
* ABE		06/12/21        Ajout Message demarrage Magistra XDM_MGA_INIT_PO 1.154
* LCL		20/12/21 : MOVIS P2 ems SAE-312
* ABE		18/01/21 : ajout msg AVA DEM-SAE367
* GGY		22/03/23 : Ajout message XDM_MGA_CONFIG_PO
* ABK		17/08/23 : ajout XDM_ETAT_PIC DEM-483
* GGY		20/11/23 : Ajout de XDM_RECORD_DAI (DEM483)
* GGY		15/01/24 : Ajout appel aarp02 pour envoi msg XDM_AS_UPDATE_PA (DEM568)
*****************************************************/
/******************************************************
* TESTS UNITAIRES  : RELECTURE CROISEE
*
* NOM DU RELECTEUR :
* DATE             :
* COMMENTAIRES     :
*
*******************************************************/
#ifndef xdm
#define xdm

/* fichiers inclus */
#include <rtworks/common.h>
#include <rtworks/ipc.h>

/* definition des constantes internes */

#define OFFSET_SUP 1000
#define OFFMSG_TST (OFFSET_SUP + 0 )
#define OFFMSG_TSH (OFFSET_SUP + 100 )
#define OFFMSG_TSG (OFFSET_SUP + 200 )
#define OFFMSG_TSP (OFFSET_SUP + 300 )
#define OFFMSG_TSA (OFFSET_SUP + 400 )
#define OFFMSG_TSE (OFFSET_SUP + 500 )
#define OFFMSG_SYN (OFFSET_SUP + 600 )

#define OFFSET_EXT 2000

#define OFFSET_GES 3000

#define OFFSET_IHM 4000

#define OFFSET_SAI 5000

#define OFFSET_BAS 6000

#define OFFSET_ARC 7000		/* Utilise dans XDL */

#define OFFSET_ALG 8000		/* Utilise dans XDL */

#define OFFSET_MAI 9000		/* sous système MAINTE */

#define OFFSET_MAG 10000        /* Offset message Magistra */

#define OFFSET_GTW 11000        /* Offset message gateways externes*/

/* definitions de constantes exportees */

/* Numeros des messages du noyau de supervision */
#define XDM_TST_PROG    (OFFMSG_TST + 0)
#define XDM_TST_ANNU    (OFFMSG_TST + 1)
#define XDM_TST_CMDT    (OFFMSG_TST + 2)
#define XDM_TST_TRAC    (OFFMSG_TST + 3)
#define XDM_TST_INIT    (OFFMSG_TST + 4)
#define XDM_TST_TINIT   (OFFMSG_TST + 5)
#define XDM_TST_TSUP    (OFFMSG_TST + 6)
#define XDM_TST_TESTDISK    (OFFMSG_TST + 7)
#define XDM_TST_DATASRV    (OFFMSG_TST + 8)

#define XDM_TSH_HORO    (OFFMSG_TSH + 0)
#define XDM_TSH_SYNC    (OFFMSG_TSH + 1)
#define XDM_TSH_MODE    (OFFMSG_TSH + 2)
#define XDM_TSH_SYST    (OFFMSG_TSH + 3)

#define XDM_TSP_INIT    (OFFMSG_TSP + 0)
#define XDM_TSP_APPL    (OFFMSG_TSP + 1)
#define XDM_TSP_SSYS    (OFFMSG_TSP + 2)
#define XDM_TSP_TACH    (OFFMSG_TSP + 3)
#define XDM_TSP_STOP    (OFFMSG_TSP + 4)
#define XDM_TSP_EXIT    (OFFMSG_TSP + 5)
#define XDM_TSP_ETAT    (OFFMSG_TSP + 6)
#define XDM_TSP_STARTTMP    (OFFMSG_TSP + 7)
#define XDM_TSP_MIGRAZUR    (OFFMSG_TSP + 8)
#define XDM_TSP_LTD    (OFFMSG_TSP + 9)

#define XDM_TSA_ADMI    (OFFMSG_TSA + 0)

#define XDM_TSG_PING    (OFFMSG_TSG + 0)
#define XDM_TSG_VERIF    (OFFMSG_TSG + 1)
#define XDM_TSG_STOP_PING    (OFFMSG_TSG + 2)
#define XDM_TSG_INIT    (OFFMSG_TSG + 3)

#define XDM_TSE_DIFE    (OFFMSG_TSE + 9)
#define XDM_TSE_INIT    (OFFMSG_TSE + 10)
#define XDM_TSE_MODI    (OFFMSG_TSE + 11)
#define XDM_TSE_MODR    (OFFMSG_TSE + 12)
#define XDM_TSE_BASC    (OFFMSG_TSE + 13)
#define XDM_TSE_SSYS    (OFFMSG_TSE + 14)
#define XDM_TSE_TACH    (OFFMSG_TSE + 15)
#define XDM_TSE_MACH    (OFFMSG_TSE + 16)
#define XDM_TSE_PERI    (OFFMSG_TSE + 17)
#define XDM_TSE_INTE    (OFFMSG_TSE + 18)
#define XDM_TSE_MODE    (OFFMSG_TSE + 20)
#define XDM_TSE_ISAG    (OFFMSG_TSE + 21)
#define XDM_TSE_ISSS    (OFFMSG_TSE + 22)
#define XDM_TSE_ISTC    (OFFMSG_TSE + 23)
#define XDM_TSE_MAJM    (OFFMSG_TSE + 25)
#define XDM_TSE_MAJE    (OFFMSG_TSE + 26)
#define XDM_TSE_MAJD    (OFFMSG_TSE + 27)
#define XDM_TSE_MAJC    (OFFMSG_TSE + 28)
#define XDM_TSE_QUES    (OFFMSG_TSE + 29)
#define XDM_TSE_MESG    (OFFMSG_TSE + 30)
#define XDM_TSE_TTMP    (OFFMSG_TSE + 31)
#define XDM_TSE_QACT    (OFFMSG_TSE + 32)
#define XDM_TSE_RACT    (OFFMSG_TSE + 33)
#define XDM_TSE_APPLI   (OFFMSG_TSE + 34)
#define XDM_TSE_RES     (OFFMSG_TSE + 35)
#define XDM_TSE_TOUT    (OFFMSG_TSE + 36)
#define XDM_TSE_VIE     (OFFMSG_TSE + 37)
#define XDM_TSE_SURV    (OFFMSG_TSE + 38)
#define XDM_TSE_DG_STATUS    (OFFMSG_TSE + 39)
#define XDM_TSE_INDQUA       (OFFMSG_TSE + 40)
#define XDM_TSE_SYNCDISK     (OFFMSG_TSE + 41)
#define XDM_TSE_STARTTMP     (OFFMSG_TSE + 42)
#define XDM_TSE_WATCH     (OFFMSG_TSE + 43)

/* Numeros des messages pour la gestion de configuration */

#define XDM_CSV_SBD             (OFFSET_GES + 0)
#define XDM_CSV_ARC             (OFFSET_GES + 1)
#define XDM_CSV_INF             (OFFSET_GES + 2)
#define XDM_CSV_RES             (OFFSET_GES + 3)
#define XDM_CSV_CANAL           (OFFSET_GES + 4)
#define XDM_CGD_LIV             (OFFSET_GES + 5)
#define XDM_CGD_DIF             (OFFSET_GES + 6)
#define XDM_CGD_MBD             (OFFSET_GES + 7)
#define XDM_CGD_VRF             (OFFSET_GES + 8)
#define XDM_CGD_DBD             (OFFSET_GES + 9)
#define XDM_CGD_CANAL           (OFFSET_GES + 10)
#define XDM_CGD_CHECK           (OFFSET_GES + 11)
#define XDM_CGD_CHGB            (OFFSET_GES + 12)


/* Numeros des messages de l'interface homme-machine */

#define XDM_ITEST_RAU            (OFFSET_IHM + 0 )
#define XDM_IMESSAGE_TFM_PRET   (OFFSET_IHM + 1 )
#define XDM_IPA_PRET            (OFFSET_IHM + 2 )
#define XDM_IMODIF_LISTE_TFM    (OFFSET_IHM + 3 )
#define XDM_IMODIF_ALARMES      (OFFSET_IHM + 4 )
#define XDM_IMODIF_FMC          (OFFSET_IHM + 5 )
#define XDM_ADM_SSS             (OFFSET_IHM + 6 )
#define XDM_ADM_TAC             (OFFSET_IHM + 7 )
#define XDM_ADM_ARC_SER         (OFFSET_IHM + 8 )
#define XDM_ADM_ARC_PER         (OFFSET_IHM + 9 )
#define XDM_ADM_ARC_INT         (OFFSET_IHM + 10 )
#define XDM_ADM_ARC_STA_M       (OFFSET_IHM + 11 )
#define XDM_ADM_ARC_STA_E       (OFFSET_IHM + 12 )
#define XDM_ADM_TRA             (OFFSET_IHM + 13 )
#define XDM_ADM_CON             (OFFSET_IHM + 14 )
#define XDM_ADM_RAF             (OFFSET_IHM + 15 )
#define XDM_ICREER_ALERTE       (OFFSET_IHM + 16 )
#define XDM_ADM_REINIT          (OFFSET_IHM + 17 )
#define XDM_IARCHIVAGE          (OFFSET_IHM + 18 )
#define XDM_IAEVT_STRADA		(OFFSET_IHM + 19 )	
#define XDM_IMASQUE             (OFFSET_IHM + 20)
#define XDM_IVUE                (OFFSET_IHM + 21)
#define XDM_IGRAPHE             (OFFSET_IHM + 22)
#define XDM_IPLANSECOURS        (OFFSET_IHM + 23)
#define XDM_ICREER_ALERTE_LOCALISEE	(OFFSET_IHM + 24)
#define XDM_ITDP_TIMER          (OFFSET_IHM + 25)
#define XDM_IMASQUE_PROJETER             (OFFSET_IHM + 26)/*RETRO*/

#define XDM_IAX        	        (OFFSET_IHM + 300 )
#define XDM_IAX_JOINDG          (OFFSET_IHM + 301 )
#define XDM_IAX_SENDPOKE        (OFFSET_IHM + 302 )
#define XDM_IAX_NEWTASK         (OFFSET_IHM + 303 )
#define XDM_IAX_SETVARCTX       (OFFSET_IHM + 304 )

#define XDM_IRAF_DAI1         (OFFSET_IHM + 305 )
#define XDM_IRAF_DAI2         (OFFSET_IHM + 306 )
#define XDM_IRAF_DAI3         (OFFSET_IHM + 307 )
#define XDM_IRAF_DAI4         (OFFSET_IHM + 308 )
#define XDM_IRAF_DAI5         (OFFSET_IHM + 309 )

#define XDM_IETAT_DISPO_EQT                 (OFFSET_IHM + 310 )
#define XDM_IRAF_BOUCHON_CHAUSSEE_PORTION   (OFFSET_IHM + 311 )
#define XDM_IRAF_VOIE_NEUTRALISEE           (OFFSET_IHM + 312 )
#define XDM_IRAF_VOIE_RADT_INVERSE          (OFFSET_IHM + 313 )
#define XDM_IRAF_BASCULEMENT_BOUCHON_VOIE   (OFFSET_IHM + 314 )
#define XDM_IRAF_OBJET_MOBILE_PORTION       (OFFSET_IHM + 315 )
#define XDM_IRAF_PORT_AFF_VOIES             (OFFSET_IHM + 316 )

#define XDM_IPOSIT_INDIC_ALARME             (OFFSET_IHM + 317 )

#define XDM_IRAF_POURCENT                   (OFFSET_IHM + 318 )
 
#define XDM_ISYN_SERVEUR                    (OFFSET_IHM + 319 )

#define XDM_IDONNEES_FT                     (OFFSET_IHM + 320 )

#define XDM_IRAF_COURBE0                     (OFFSET_IHM + 321 )

#define XDM_IDONNEES_INRETS_PR		    (OFFSET_IHM + 322 )
#define XDM_IDONNEES_INRETS_TRA             (OFFSET_IHM + 323 )
#define XDM_IDONNEES_BOUCHONS               (OFFSET_IHM + 324 )
#define XDM_IDONNEES_TDP                    (OFFSET_IHM + 325 )
#define XDM_IDONNEES_RETARD                 (OFFSET_IHM + 326 )

#define XDM_IRAF_OBJET_MOBILE_ECHANG        (OFFSET_IHM + 327 )

#define XDM_ISYN_SERVEURCV                  (OFFSET_IHM + 328 )

#define XDM_IRAF_COURBE1                    (OFFSET_IHM + 329 )

#define XDM_IRAZ_MOB                        (OFFSET_IHM + 330 )

#define XDM_IREI_EQT                        (OFFSET_IHM + 331 )

#define XDM_IAX_FINPOSTE		    (OFFSET_IHM + 332 )
#define XDM_IRAF_DEBUT			    (OFFSET_IHM + 333 )
#define XDM_IRAF_FIN			    (OFFSET_IHM + 334 )
#define XDM_IRAF_FMCLOC			    (OFFSET_IHM + 335 )
#define XDM_IAX_INITPRES                    (OFFSET_IHM + 336 )
#define XDM_IAX_ACQT		    	    (OFFSET_IHM + 337 )
#define XDM_IRAF_COURBE2		    (OFFSET_IHM + 338 )

#define XDM_IDONNEES_TRAFIC_STRADA	    (OFFSET_IHM + 339 )

#define XDM_IDONNEES_QVTO		    (OFFSET_IHM + 340 ) 
#define XDM_IPMV_DATEX			    (OFFSET_IHM + 341 )

#define XDM_ISYN_NOMADE_VUE		    (OFFSET_IHM + 342 )
#define XDM_ISYN_NOMADE_MASK		    (OFFSET_IHM + 343 )

#define XDM_IRAF_BOUCHON_CHAUSSEE_VOIE   (OFFSET_IHM + 344 )
#define XDM_COMMU_ACQ                       (OFFSET_IHM + 345)
#define XDM_COMMU_RAPPROCHE                 (OFFSET_IHM + 346)
#define XDM_COMMU_IGNORE                    (OFFSET_IHM + 347)
#define XDM_COMMU_FMC                       (OFFSET_IHM + 348)
#define XDM_IAX_INFOMESSAGE       	(OFFSET_IHM + 349 )
#define XDM_API_POKE       		(OFFSET_IHM + 350 )
#define XDM_INIT_TITRE       		(OFFSET_IHM + 351 )
#define XDM_API_SETVARCTX       	(OFFSET_IHM + 352 )
#define XDM_API_NEWTASK         	(OFFSET_IHM + 353 )
#define XDM_API_SYNOS         		(OFFSET_IHM + 354 )
#define XDM_EXEC_TITRE         		(OFFSET_IHM + 355 )
#define XDM_INIT_TISSE       		(OFFSET_IHM + 356 )

/* Numeros des messages systeme d'aide a la decision */

#define XDM_DIN_PA		(OFFSET_SAI + 0 )
#define XDM_DIN_BDC		(OFFSET_SAI + 1 )


/* Numeros des messages equipements externes  */

#define XDM_EINIT               	(OFFSET_EXT + 0 )
#define XDM_EINIT_ABT			(OFFSET_EXT + 1 )
#define XDM_EFIN_CONFIG			(OFFSET_EXT + 2 )
#define XDM_EFIN_CONFIG_SUR		(OFFSET_EXT + 3 )
#define XDM_ECONFIG_EQPT		(OFFSET_EXT + 4 )
#define XDM_ECMD_EQPT			(OFFSET_EXT + 5 )
#define XDM_ECMD_RAU			(OFFSET_EXT + 6 )
#define XDM_ETAT_RAU			(OFFSET_EXT + 7 )
#define XDM_ECMD_VIDEO			(OFFSET_EXT + 8 )
#define XDM_ETAT_VIDEO			(OFFSET_EXT + 9 )
#define XDM_ETAT_MAGNETO		(OFFSET_EXT + 10 )
#define XDM_ECMD_AUTOTEL		(OFFSET_EXT + 11 )
#define XDM_ETAT_AUTOTEL		(OFFSET_EXT + 12 )
#define XDM_ECMD_OPER			(OFFSET_EXT + 13 )
#define XDM_ETAT_OPER			(OFFSET_EXT + 14 )
#define XDM_EMES_RADT_6mn		(OFFSET_EXT + 15 )
#define XDM_EMES_6mn_Voie		(OFFSET_EXT + 16 )
#define XDM_EMES_6mn_Chaussee		(OFFSET_EXT + 17 )
#define XDM_EMES_RADT_6gl		(OFFSET_EXT + 18 )
#define XDM_EMES_6gl_Voie		(OFFSET_EXT + 19 )
#define XDM_EMES_6gl_Chaussee		(OFFSET_EXT + 20 )
#define XDM_EMES_Classe_DIV		(OFFSET_EXT + 21 )
#define XDM_ENiveau_Service		(OFFSET_EXT + 22 )
#define XDM_EConf_Voie_DAI		(OFFSET_EXT + 23 )
#define XDM_EMES_DAI			(OFFSET_EXT + 24 )
#define XDM_ETAT_PMV			(OFFSET_EXT + 25 )
#define XDM_ETAT_PICTO			(OFFSET_EXT + 26 )
#define XDM_ETAT_NAV			(OFFSET_EXT + 27 )
#define XDM_ETAT_GTC			(OFFSET_EXT + 28 )
#define XDM_ECMD_PMV			(OFFSET_EXT + 29 )
#define XDM_ECMD_PICTO			(OFFSET_EXT + 30 )
#define XDM_ECMD_NAV			(OFFSET_EXT + 31 )
#define XDM_ECMD_SCEN			(OFFSET_EXT + 32 )
#define XDM_ECMD_SCEN_FICH		(OFFSET_EXT + 33 )
#define XDM_ECMD_DAI			(OFFSET_EXT + 34 )
#define XDM_EMES_POIDS			(OFFSET_EXT + 35 )
#define XDM_EConf_Voie			(OFFSET_EXT + 36 )
#define XDM_EDEM_Indiv			(OFFSET_EXT + 37 )
#define XDM_ELCR			(OFFSET_EXT + 38 )
#define XDM_ECPT_LCR			(OFFSET_EXT + 39 )
#define XDM_EInit_IHM_6gl_Voie		(OFFSET_EXT + 40 )
#define XDM_EInit_IHM_6gl_Chaussee	(OFFSET_EXT + 41 )
#define XDM_ECMD_SIGE			(OFFSET_EXT + 42 )
#define XDM_ECMD_SEQT			(OFFSET_EXT + 43 )
#define XDM_ECMD_SEQE			(OFFSET_EXT + 44 )
#define XDM_ECMD_SIGT			(OFFSET_EXT + 45 )
#define XDM_EInit_IHM_PMV		(OFFSET_EXT + 46 )
#define XDM_EInit_IHM_Picto		(OFFSET_EXT + 47 )
#define XDM_EInit_IHM_NAV		(OFFSET_EXT + 48 )
#define XDM_EInit_IHM_DAI		(OFFSET_EXT + 49 )
#define XDM_EInit_IHM_GTC		(OFFSET_EXT + 50 )
#define XDM_EInit_IHM_NICE		(OFFSET_EXT + 51 )
#define XDM_ETAT_JourNuit		(OFFSET_EXT + 52 )
#define XDM_ECMD_METEO			(OFFSET_EXT + 53 )
#define XDM_ECMD_Contraste		(OFFSET_EXT + 54 )
#define XDM_ETAT_TELEALARME		(OFFSET_EXT + 55 )
#define XDM_ETAT_PANNEAU		(OFFSET_EXT + 56 )
#define XDM_ETAT_SEQUENCE_TUBE		(OFFSET_EXT + 57 )
#define XDM_ETAT_SEQUENCE_ECH		(OFFSET_EXT + 58 )
#define XDM_ETAT_CAPTEUR		(OFFSET_EXT + 59 )
#define XDM_Etat_CO			(OFFSET_EXT + 60 )
#define XDM_Etat_Opacite		(OFFSET_EXT + 61 )
#define XDM_ETAT_Contraste		(OFFSET_EXT + 62 )
#define XDM_EInit_IHM_Contraste		(OFFSET_EXT + 63 )
#define XDM_EInit_IHM_JourNuit		(OFFSET_EXT + 64 )
#define XDM_EInit_IHM_Classe_DIV	(OFFSET_EXT + 65 )
#define XDM_ECMD_MAT_NAV		(OFFSET_EXT + 66 )
#define XDM_ECMD_ARRET_SCEN		(OFFSET_EXT + 67 )
#define XDM_EConf_Voie_RADT		(OFFSET_EXT + 68 )
#define XDM_ECMD_FIC			(OFFSET_EXT + 69 )
#define XDM_MODE_LENT			(OFFSET_EXT + 70 )
#define XDM_EInit_OP_VW			(OFFSET_EXT + 71 )
#define XDM_ETAT_MAT_NAV		(OFFSET_EXT + 72 )
#define XDM_EInit_IHM_Liaison		(OFFSET_EXT + 73 )
#define XDM_ETAT_LIAISON		(OFFSET_EXT + 74 )
#define XDM_EMOD_DEG			(OFFSET_EXT + 75 )
#define XDM_EMOD_ISOLE			(OFFSET_EXT + 76 )
#define XDM_Acknoledge			(OFFSET_EXT + 77 )
#define XDM_EInit_IHM_MAT_NAV		(OFFSET_EXT + 78 )
#define XDM_Acknoledge_Ala		(OFFSET_EXT + 79 )
#define XDM_EInit_IHM_6mn_parVoie	(OFFSET_EXT + 80 )
#define XDM_EMES_6mn_parVoie		(OFFSET_EXT + 81 )
#define XDM_EMES_PESAGE			(OFFSET_EXT + 82 )
#define XDM_Acknoledge_Pesage		(OFFSET_EXT + 83 )
#define XDM_INFO_PAU			(OFFSET_EXT + 84 )
#define XDM_DONNEES_METEO_QTX		(OFFSET_EXT + 85 )
#define XDM_ACQ_DONNEES_METEO_QTX	(OFFSET_EXT + 86 )
#define XDM_ECMD_GTC			(OFFSET_EXT + 87 )
#define XDM_ACQ_DONNEES_TDP		(OFFSET_EXT + 88 )
#define XDM_EMES_TDP_6MN		(OFFSET_EXT + 89 )
#define XDM_EMES_TDP_HORAIRE		(OFFSET_EXT + 90 )
#define XDM_EMES_TDP_6MN_PAR_ZONE	(OFFSET_EXT + 91 )
#define XDM_ETDP_SEUIL			(OFFSET_EXT + 92 )
#define XDM_ETDP_PMV			(OFFSET_EXT + 93 )
#define XDM_VAL_TDP			(OFFSET_EXT + 94 )
#define XDM_ETDP_IHM			(OFFSET_EXT + 95 )
#define XDM_CR_TDP			(OFFSET_EXT + 96 )
#define XDM_ECMD_PAL			(OFFSET_EXT + 97 )
#define XDM_EInit_IHM_PAL		(OFFSET_EXT + 98 )
#define XDM_ETAT_PAL			(OFFSET_EXT + 99 )
#define XDM_EPAL_AUTO			(OFFSET_EXT + 100 )
#define XDM_EPAL_AUTO_FIN		(OFFSET_EXT + 101 )
#define XDM_ETAT_PMVA	(OFFSET_EXT + 102)
#define XDM_ECMD_PMVA	(OFFSET_EXT + 103)
#define XDM_EInit_IHM_PMVA	(OFFSET_EXT + 104)
#define XDM_EInit_IHM_PRV	(OFFSET_EXT + 104)
#define XDM_EInit_IHM_PIC	(OFFSET_EXT + 104)
#define XDM_ETAT_BAF		(OFFSET_EXT + 105)
#define XDM_ETAT_BAD		(OFFSET_EXT + 106)
/*****************************************************/
/* Deplace au bon endroit et renumerote		     */
/*#define XDM_ETAT_PRV		(OFFSET_EXT + 163)   */
/*****************************************************/
#define XDM_ECMD_BAF		(OFFSET_EXT + 107)
#define XDM_ECMD_BAD		(OFFSET_EXT + 108)
/*****************************************************/
/* Deplace au bon endroit et renumerote		     */
/*#define XDM_ECMD_PRV		(OFFSET_EXT + 162)   */
/*****************************************************/
#define XDM_EInit_IHM_BAF		(OFFSET_EXT + 109)
#define XDM_EInit_IHM_BAD		(OFFSET_EXT + 110)
#define XDM_ETDP_PMVA			(OFFSET_EXT + 111)
#define XDM_ECMD_SCEN_BAD		(OFFSET_EXT + 112)
#define XDM_ENAV_AUTO			(OFFSET_EXT + 113)
#define XDM_ENAV_AUTO_FIN		(OFFSET_EXT + 114)
#define XDM_EBAF_AUTO			(OFFSET_EXT + 115)
#define XDM_EBAF_AUTO_FIN		(OFFSET_EXT + 116)
#define XDM_EPMV_AUTO			(OFFSET_EXT + 117)
#define XDM_EPMV_AUTO_FIN		(OFFSET_EXT + 118)
#define XDM_EPMVA_AUTO			(OFFSET_EXT + 119)
#define XDM_EPMVA_AUTO_FIN		(OFFSET_EXT + 120)
#define XDM_EBAD_AUTO			(OFFSET_EXT + 121)
#define XDM_EBAD_AUTO_FIN		(OFFSET_EXT + 122)
#define XDM_EPRV_AUTO			(OFFSET_EXT + 167)
#define XDM_EPRV_AUTO_FIN		(OFFSET_EXT + 168)
#define XDM_ETUNNEL_AUTO		(OFFSET_EXT + 123)
#define XDM_ETUNNEL_AUTO_FIN		(OFFSET_EXT + 124)
#define XDM_ECMD_BAF_GLOBALE		(OFFSET_EXT + 125)

/*****************************************************/
/* Deplace au bon endroit et renumerote		     */
//#define XDM_ETDP_IHM_PMVA		(OFFSET_EXT + 165 )
///#define XDM_CR_TDP_PMVA			(OFFSET_EXT + 166 )
//#define XDM_VAL_TDP_PMVA		(OFFSET_EXT + 167 )
/*****************************************************/

/* AAZ */
#define XDM_ETAT_SEQSIG	    		(OFFSET_EXT + 126)
#define XDM_ETAT_SEQECL	    		(OFFSET_EXT + 127)
#define XDM_ETAT_SEQVEN	    		(OFFSET_EXT + 128)
#define XDM_ETAT_SEQENR	    		(OFFSET_EXT + 129)
#define XDM_ETAT_SEQINC	    		(OFFSET_EXT + 130)
#define XDM_ETAT_SEQEXP	    		(OFFSET_EXT + 131)
#define XDM_ETAT_SIGSIG	    		(OFFSET_EXT + 132)
#define XDM_ETAT_SIGECL	    		(OFFSET_EXT + 133)
#define XDM_ETAT_SIGVEN	    		(OFFSET_EXT + 134)
#define XDM_ETAT_SIGENR	    		(OFFSET_EXT + 135)
#define XDM_ETAT_SIGINC	    		(OFFSET_EXT + 136)
#define XDM_ETAT_SIGEXP	    		(OFFSET_EXT + 137)
#define XDM_ETAT_SIGSYS	    		(OFFSET_EXT + 138)
#define XDM_ECMD_SIG	    		(OFFSET_EXT + 139)
#define XDM_ECMD_ECL	    		(OFFSET_EXT + 140)
#define XDM_ECMD_VEN	    		(OFFSET_EXT + 141)
#define XDM_ECMD_ENR	    		(OFFSET_EXT + 142)
#define XDM_ECMD_INC	    		(OFFSET_EXT + 143)
#define XDM_ECMD_EXP	    		(OFFSET_EXT + 144)
#define XDM_ECMD_SYS	    		(OFFSET_EXT + 145)
#define XDM_ECMD_SEQSIG	    		(OFFSET_EXT + 146)
#define XDM_ECMD_SEQECL	    		(OFFSET_EXT + 147)
#define XDM_ECMD_SEQVEN	    		(OFFSET_EXT + 148)
#define XDM_ECMD_SEQENR	    		(OFFSET_EXT + 149)
#define XDM_ECMD_SEQEXP	    		(OFFSET_EXT + 150)
#define XDM_ECMD_SEQINC	    		(OFFSET_EXT + 151)
#define XDM_ETAT_EQUIPEMENT			(OFFSET_EXT + 152)
#define XDM_ETAT_DOMAINE			(OFFSET_EXT + 153)
#define XDM_ETAT_ALERTE	    		(OFFSET_EXT + 154)
#define XDM_ETAT_SIGLTN	    		(OFFSET_EXT + 155)
#define XDM_ECMD_LTN	    		(OFFSET_EXT + 156)
#define XDM_ECMD_DOMAINE    		(OFFSET_EXT + 157)

#define XDM_EInit_IHM_NICERenovee	(OFFSET_EXT + 158 )
#define XDM_ECMD_AUTOTEL2     		(OFFSET_EXT + 159)
#define XDM_ECMD_AUTORAD      		(OFFSET_EXT + 160)

#define XDM_REGUL_DETECTION			(OFFSET_EXT + 161)
#define XDM_REGUL_SIMU				(OFFSET_EXT + 162)
#define XDM_REGUL_START_SIMU		(OFFSET_EXT + 163)
#define XDM_EREG_FMC				(OFFSET_EXT + 164)
#define XDM_REGUL_APPLICATION		(OFFSET_EXT + 165)
#define XDM_ACQUITTE_REGUL			(OFFSET_EXT + 166)
#define XDM_CHECK_FAX				(OFFSET_EXT + 167)

#define XDM_ETAT_EQT_SAGA			(OFFSET_EXT + 168)
#define XDM_EInit_IHM_EQT_SAGA		(OFFSET_EXT + 169)
#define XDM_ECMD_EQT_SAGA			(OFFSET_EXT + 170)
#define XDM_ECMD_BASC_RAU			(OFFSET_EXT + 171)
#define XDM_ETAT_CAP				(OFFSET_EXT + 172)
#define XDM_GABEB_DETECTION			(OFFSET_EXT + 173)
#define XDM_GABEB_APPLICATION		(OFFSET_EXT + 174)
#define XDM_ACQUITTE_GABEB			(OFFSET_EXT + 175)
#define XDM_GABEB_FMC				(OFFSET_EXT + 176)

#define XDM_ETAT_CFE				(OFFSET_EXT + 177)
#define XDM_ECMD_CFE				(OFFSET_EXT + 178)
#define XDM_AT_CFE					(OFFSET_EXT + 179)
#define XDM_EInit_IHM_CFE			(OFFSET_EXT + 180)
#define XDM_EInit_IHM_CAP			(OFFSET_EXT + 181)
#define XDM_ECMD_CAP				(OFFSET_EXT + 183)
#define XDM_ETDP_SURCHARGE			(OFFSET_EXT + 184)
#define XDM_ETDP_ETAT_SURCHARGE		(OFFSET_EXT + 185)
#define XDM_ETDP_INIT_SURCHARGE		(OFFSET_EXT + 186)
#define XDM_ETAT_IMU				(OFFSET_EXT + 187)
#define XDM_ECMD_IMU				(OFFSET_EXT + 188)
#define XDM_AT_IMU 					(OFFSET_EXT + 189)
#define XDM_EInit_IHM_IMU			(OFFSET_EXT + 190)
#define XDM_ETAT_SONO				(OFFSET_EXT + 191)
#define XDM_ECMD_SONO				(OFFSET_EXT + 192)
#define XDM_AT_SONO					(OFFSET_EXT + 193)
#define XDM_EInit_IHM_SONO			(OFFSET_EXT + 194)
#define XDM_ETDP_IHM_PMVA			(OFFSET_EXT + 195)
#define XDM_CR_TDP_PMVA				(OFFSET_EXT + 196)
#define XDM_VAL_TDP_PMVA			(OFFSET_EXT + 197)
#define XDM_ETAT_BAF_EMS			(OFFSET_EXT + 198)
#define XDM_ETAT_BAD_EMS			(OFFSET_EXT + 199)
#define XDM_ETAT_NAV_EMS			(OFFSET_EXT + 200)
#define XDM_ETAT_PAL_EMS			(OFFSET_EXT + 201)
#define XDM_ETAT_PRV				(OFFSET_EXT + 202)
#define XDM_ECMD_PRV				(OFFSET_EXT + 203)
#define XDM_ETAT_PIC				(OFFSET_EXT + 204)
#define XDM_ECMD_PIC				(OFFSET_EXT + 205)
#define XDM_RECORD_DAI				(OFFSET_EXT + 206)

/* AZA */
/* Numeros des messages Base DD  */

#define XDM_AT_TUBE               	(OFFSET_BAS + 0 )
#define XDM_AT_echangeur               	(OFFSET_BAS + 1 )
#define XDM_AT_NAV               	(OFFSET_BAS + 2 )
#define XDM_AT_PMV               	(OFFSET_BAS + 3 )
#define XDM_AA_alerte               	(OFFSET_BAS + 4 )
#define XDM_AM_alarme               	(OFFSET_BAS + 5 )
#define XDM_A_INIT_IHM               	(OFFSET_BAS + 6 )
#define XDM_A_TIMER_RAFRAI_IHM         	(OFFSET_BAS + 7 )
#define XDM_AN_PURGE         		(OFFSET_BAS + 8 )
#define XDM_AG_PURGE         		(OFFSET_BAS + 9 )
#define XDM_AG_PURGEHISTO      		(OFFSET_BAS + 10 )
#define XDM_AS_FdeT      		(OFFSET_BAS + 11 )
#define XDM_AH_INITLT			(OFFSET_BAS + 12 )
#define XDM_AH_RADT_DAI			(OFFSET_BAS + 13 )
#define XDM_AS_INRETS                   (OFFSET_BAS + 14 )
#define XDM_AS_BOUCHON                  (OFFSET_BAS + 15 )
#define XDM_AH_SATIR                    (OFFSET_BAS + 16 )
#define XDM_AEQA                        (OFFSET_BAS + 17 )
#define XDM_AT_INI_EQT                  (OFFSET_BAS + 18 )
#define XDM_AT_INI_IHM                  (OFFSET_BAS + 19 )
#define XDM_A_TIMER_TRV_COURS		(OFFSET_BAS + 20 )
#define XDM_A_PRES_OP			(OFFSET_BAS + 21 )
#define XDM_AS_TDP			(OFFSET_BAS + 22 )
#define XDM_AS_TIMER_SRV_RADT		(OFFSET_BAS + 23 )
#define XDM_AS_TIMER_PURGE_RADT		(OFFSET_BAS + 24 )
#define XDM_AS_TIMER_CLT_RADT		(OFFSET_BAS + 25 )
#define XDM_AS_TIMER_SRVEVT_STRADA	(OFFSET_BAS + 26 )
#define XDM_AS_SRVEVT_STRADA		(OFFSET_BAS + 27 )
#define XDM_AS_TIMER_CLT_EVT           	(OFFSET_BAS + 28 )
#define XDM_AS_EVT_STRADA		(OFFSET_BAS + 29 )
#define XDM_AT_PAL               	(OFFSET_BAS + 30 )
#define XDM_A_TIMER_FMC_LOC		(OFFSET_BAS + 31 )
#define XDM_AG_IMPCAT			(OFFSET_BAS + 32 )
#define XDM_AG_EXPCATF                   (OFFSET_BAS + 33 )
#define XDM_AG_EXPCATC                   (OFFSET_BAS + 34 )
#define XDM_AW_TIMER			(OFFSET_BAS + 35)
#define XDM_NOMADE_TIMER		(OFFSET_BAS + 36)
#define XDM_NOMADE_REVEIL		(OFFSET_BAS + 37)
#define XDM_AMISOL_TIMER                (OFFSET_BAS + 38)
#define XDM_AT_PMVA	(OFFSET_BAS + 39)
#define XDM_AT_BAF		(OFFSET_BAS + 40)
#define XDM_AT_BAD		(OFFSET_BAS + 41)
#define XDM_AT_PRV		(OFFSET_BAS + 161)
#define XDM_AT_EQT_SAGA		(OFFSET_BAS + 162)
#define XDM_AS_TIMER_SAGA_60          	(OFFSET_BAS + 163 )
#define XDM_AS_TIMER_SAGA_3600         	(OFFSET_BAS + 164 )
#define XDM_AS_TIMER_SAGA_DATE         	(OFFSET_BAS + 165 )
#define XDM_AS_TIMER_POLYVOX		(OFFSET_BAS + 166)
#define XDM_RMVI_DEMANDE		(OFFSET_BAS + 167)
#define XDM_RMVI_STATUS			(OFFSET_BAS + 168)
#define XDM_RMVI_RETOUR			(OFFSET_BAS + 169)
#define XDM_AVA_CONNEXION		(OFFSET_BAS + 170)
#define XDM_ODI_C			(OFFSET_BAS + 171)
#define XDM_AA_alerte_cockpit		(OFFSET_BAS + 172)
#define XDM_AS_TIMER_DATEX2     (OFFSET_BAS + 173)
#define XDM_AS_TIMER_SITU_RECAP     (OFFSET_BAS + 174)
#define XDM_ADA_EVT			(OFFSET_BAS + 175)
#define XDM_AS_TIMER_PAT		(OFFSET_BAS + 176)
#define XDM_ADA_TIMER_RADT		(OFFSET_BAS + 177)
#define EMSG_TDP_FCD                     (OFFSET_BAS + 178)
#define XDM_AS_TIMER_TDP_FCD		(OFFSET_BAS + 179)
#define XDM_AS_TIMER_SITU_CPC		(OFFSET_BAS + 180)
#define XDM_RMVI_RETOUR_EMS		(OFFSET_BAS + 181)
#define XDM_AT_PIC				(OFFSET_BAS + 182)
#define XDM_AS_UPDATE_PA		(OFFSET_BAS + 183)

/* AAZ */
#define XDM_AT_SIG	        (OFFSET_BAS + 42)
#define XDM_AT_ECL	        (OFFSET_BAS + 43)
#define XDM_AT_VEN	        (OFFSET_BAS + 44)
#define XDM_AT_ENR	        (OFFSET_BAS + 45)
#define XDM_AT_INC	        (OFFSET_BAS + 46)
#define XDM_AT_EXP	        (OFFSET_BAS + 47)

#define XDM_AS_SRVEVT_STRADA_TDP        (OFFSET_BAS + 48)
#define XDM_ATDP_TIMER                  (OFFSET_BAS + 49)
#define XDM_AS_SRVEVT_STRADA_RDT        (OFFSET_BAS + 50)
#define XDM_AS_SRVEVT_STRADA_FMC        (OFFSET_BAS + 51)
#define XDM_AA_alerte_localisee 	(OFFSET_BAS + 52)

#define XDM_AA_COMMU_FMC                (OFFSET_BAS + 53)

#define XDM_ANEUTR_TIMER                (OFFSET_BAS + 54)

#define XDM_ATDP_TIMER_ASF              (OFFSET_BAS + 55)
#define XDM_AT_CAP			(OFFSET_BAS + 56)

/* Numeros des messages MAINTEnance */

#define XDM_M_RELEVER_COMPTEUR		(OFFSET_MAI + 0 )
#define XDM_M_COMPTEUR_RELEVE		(OFFSET_MAI + 1 )
#define XDM_M_TIMER_COMPTEUR		(OFFSET_MAI + 2 )
#define XDM_TSE_KEEPALIVE	(OFFSET_MAI + 3)

/* Numéro des message MAGistra */
#define XDM_MGA_PAS_UTILISE             (OFFSET_MAG + 0)
#define XDM_MGA_COURRIER                (OFFSET_MAG + 1)
#define XDM_MGA_INIT_PO                 (OFFSET_MAG + 2)
#define XDM_MGA_CMD_DEL_SEMA           	(OFFSET_MAG + 3)
#define XDM_MGA_CMD_CFG_PRA_PO     	(OFFSET_MAG + 4)
#define XDM_MGA_CMD_LIVRE_MODERNE	(OFFSET_MAG + 5)
#define XDM_MGA_CMD_JAR_REMORQUE	(OFFSET_MAG + 6)
#define XDM_MGA_CMD_HIVERNAGE   	(OFFSET_MAG + 7)
#define XDM_MGA_CMD_HIVERNAGE_ASF       (OFFSET_MAG + 8)
#define XDM_MGA_CMD_HIVERNAGE_EXPORT    (OFFSET_MAG + 9)
#define XDM_MGA_CMD_RETOUR          	(OFFSET_MAG + 10)
#define XDM_MGA_CMD_ALARME_LT          	(OFFSET_MAG + 11)
#define XDM_MGA_BIBLIO_PMV              (OFFSET_MAG + 12)
#define XDM_MGA_CMD_JAR_REMORQUE_CA	(OFFSET_MAG + 13)
#define XDM_MGA_CONFIG_PO               (OFFSET_MAG + 14)

/* Numero des messages utilises pour les gateways externes (AVA, Cockpit)*/
#define XDM_GTW_AVA_CREATION          	(OFFSET_GTW + 0)
#define XDM_GTW_AVA_MODIFICATION        (OFFSET_GTW + 1)
#define XDM_GTW_AVA_RETOUR_ALERTE       (OFFSET_GTW + 2)
#define XDM_GTW_TEL_RETOUR_APPEL        (OFFSET_GTW + 3)
#define XDM_GTW_TEL_APPEL               (OFFSET_GTW + 4)
#define XDM_GTW_RAD_APPEL               (OFFSET_GTW + 5)
#define XDM_GTW_RAD_RETOUR_APPEL        (OFFSET_GTW + 6)
#define XDM_GTW_PIT_BOUCHON         	(OFFSET_GTW + 7)
#define XDM_GTW_PIT_BOU_RETOUR_ALERTE   (OFFSET_GTW + 8)



/* Constantes utilise par la couche IP RTWORKS V3 */
#define XDM_FLG_SRVMSGWRITE  TRUE


/* definitions de types exportes */

typedef struct
{
        char    Nom[40] ;
        int     Id ;
        char   Grammaire[255] ;

} T_TYPEMSG ;

/* definition de macro exportees */

/* Identifiant de msg pour TipcSrvMsgWrite */
#define XDM_IdentMsg(num) TipcMtLookupByNum((T_INT4)num)

/* declarations de donnees globales */
#ifndef xdm_c
extern T_TYPEMSG tg_Mtb[];
#endif

/* delaration de fonctions externes */

#endif
