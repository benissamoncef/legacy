/*E*/
/* Fichier : $Id: xdg.h,v 1.147 2021/05/03 14:05:18 pc2dpdy Exp $        Release : $Revision: 1.147 $        Date : $Date: 2021/05/03 14:05:18 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE xdg * FICHIER xdg.h
******************************************************
* DESCRIPTION DU MODULE :
*
* Declaration des DATAGROUPS
*
******************************************************
* HISTORIQUE :
*
* T.Milleville 23 Aou 1994	: Creation
********************************
* Mismer.D     30 Aou 1994	: Ajout des Datagroup EQUEXT
********************************
* Nagiel.E     23 Sept 1994	: Ajout de XDG_ECMD_CONTRASTE
********************************
* Guilhou.JM   28 Sept 1994	: Ajout des datagroups GESIHM
********************************
* Mismer.D     03 Oct  1994	: Ajout des Datagroups d'init IHM
********************************
* Mercier O.   04 Oct  1994	: Ajout des Datagroups de BASEDD : V 1.13
*				 XDG_AT
* Noel B.	06 Oct	1994	: Ajout DG pour TFM			V1.14
*
* Niepceron.	12 Oct	1994	: Ajout DG XDG_IAX_TEXTE et XDG_IAX_SYNOP
*                                                                       V1.15
* Nagiel.E	12 Oct  1994	:  Ajout de XDG_ETAT_CONTRASTE		V1.17
* Guilhou.JM	13 Oct  1994	:  Ajout de XDG_IMODIF_FMC		V1.18
* Guilhou.JM	13 Oct  1994	:  Ajout de XDG_ICREER_ALERTE et
					XDG_ISUPPRIMER_ALERTE		V1.19
* Niepceron.	14 Oct	1994	: Ajout DG XDG_IABTINIT_PMV XDG_IABTINIT_PICTO 
                                  XDG_IABTINIT_RADT XDG_IABTINIT_NIV 
                                  XDG_IABTINIT_NAV XDG_IABTINIT_CAMERA 
                                  XDG_IABTINIT_DAI XDG_IABTINIT_DIV     V1.21
* Niepceron.	14 Oct	1994	: Ajout DG XDG_IABTINIT_RAU 		V1.22
* Mismer D.	14 Oct  1994	: Rien					V 1.23
* Nagiel.E	17 Oct  1994	: Ajout de XDG_ECMD_DAI			V 1.24
* Niepceron.	18 Oct	1994	: Ajout DG XDG_IRAF_DAI			V 1.25
* Niepceron     19 Oct 1994     : Ajout de XDG_IETAT_DISPO_EQT  XDG_IRAF_BOUCHON_CHAUSSEE_PORTION 
*                                 XDG_IRAF_VOIE_NEUTRALISEE XDG_IRAF_VOIE_RADT_INVERSE
*                                 XDG_IRAF_BASCULEMENT_BOUCHON_VOIE XDG_IRAF_OBJET_MOBILE_PORTION
*                                 XDG_IRAF_OBJET_MOBILE_PORT_AFF_VOIES       V1.26
* Niepceron     20 Oct 1994     : Modif de XDG_IRAF_OBJET_MOBILE_PORT_AFF_VOIES en
*                                 XDG_IRAF_PORT_AFF_VOIES       	V1.27
********************************
* Niepceron     21 Oct 1994     : Ajout de XDG_IPOSIT_INDIC_ALARME 	V 1.28 
* Noel		21 Oct 1994	: Ajout de XDG_IMODIF_LISTE_TFM		V1.29
* MERCIER  	24 Oct 1994	: Ajout de XDG_AA			V1.30
*					   XDG_AM
* T.Milleville 24 Oct 1994	: Ajout du DataGroup XDG_SYNCHRO	V 1.31
********************************
* Niepceron     26 Oct 1994     : Ajout de XDG_IRAF_POURCENT     	V 1.32
*
* Nagiel E.	27 Oct  1994	: Correction erreur XDG_ECMD_VIDEO  	V 1.33
******************************
* MERCIER O.	28 Oct 	1994	: Ajout de XDG_A_IHM			V1.34
******************************
* MERCIER O.   	28 Oct 1994	: RIEN					V1.35
********************************
* Niepceron     31 Oct 1994     : Ajout de XDG_IABTINT_JOUNUIT     	V 1.36
* Niepceron     31 Oct 1994     : Ajout de XDG_ISYN_SERVEUR     	V 1.37
********************************
* MERCIER O.	02 Nov 1994	: Ajout de XDG_A_TIMER_RAFRAI_IHM	V 1.38
********************************
* MERCIER O.	07 Nov 1994	: Ajout de XDG_AN			V 1.39
********************************
* MERCIER O.	07 Nov 1994	: Ajout de XDG_AG			V 1.39
******************************************************
* MISMER D.	08 Nov 1994	: Ajout de XDG_EMES_RADT_6MN		V 1.40 V 1.41
******************************************************
* MERCIER O.	09 Nov 1994	: Ajout de  XDG_AM_INIT_MES6MnsVoie 	V 1.42
*		XDG_AM_INIT_MES6MnsCh   
*		XDG_AM_INIT_MESPOIDS   
*		XDG_AM_INIT_NivServ
******************************************************
* NIEPCERON  	14 Nov 1994	: Ajout de  XDG_IABTINIT_RADT_COURBES 	V 1.43
******************************************************
* NIEPCERON  	15 Nov 1994	: Ajout de  XDG_IDONNEES_FT  	 	V 1.44
******************************************************
* MERCIER O.  	15 Nov 1994	: Ajout de  XDG_AS_COURBE  	 	V 1.45
******************************************************
* NIEPCERON  	16 Nov 1994	: Ajout de  XDG_IRAF_COURBE  	 	V 1.46
******************************************************
* NAGIEL  	17 Nov 1994	: Ajout de  XDG_ECMD_MAT_NAV  	 	V 1.47
******************************************************
* NIEPCERON  	25 Nov 1994	: Ajout de  XDG_IDONNEES_INRETS_PR
*					    XDG_IDONNEES_INRETS_TRA  	V 1.48
******************************************************
* MISMER  	28 Nov 1994	: Ajout de  XDG_ETAT_GTC_NICE		V 1.49
* MISMER  	28 Nov 1994	: Ajout de  XDG_ECMD_GTC_NICE		V 1.50
******************************************************		
* NIEPCERON  	29 Nov 1994	: Ajout de  XDG_IDONNEES_BOUCHONS	V 1.51
******************************************************		
* NIEPCERON  	29 Nov 1994	: Ajout de  XDG_IDONNEES_QVTO	V 1.52
******************************************************		
* NIEPCERON  	 1 Dec 1994	: Ajout de  	XDG_IABTINIT_TUB
*                				XDG_IABTINIT_ECH 	V 1.53
******************************************************		
* MISMER  	 1 Dec 1994	: Ajout de  	XDG_ECMD		V 1.54
******************************************************		
* Guilhou	21dec 1994	: ajout de XDG_IAVERT_OPERATEUR		V1.55
* Guilhou	21dec 1994	: ajout de XDG_IAVERT_ADMINISTRATEUR	V1.56
******************************************************
* volcic	21 Dec 1994	: ajout de XDG pour TERDT	V1.57
******************************************************
* Cornu   	22 Dec 1994	: ajout de XDG pour fenetre LCR	V1.58
******************************************************
* Cornu   	22 Dec 1994	: ajout de XDG pour fenetre LCR	V1.58
******************************************************
* Niepceron     23 Dec 1994     : Ajout de  XDG_IRAF_OBJET_MOBILE_ECHANG V1.60
******************************************************
* Guilhou	27 dec 1994	: ajout de XDG_IDEMANDE_SURCHARGE et
* 				  et XDG_IREPONSE_SURCHARGE		  V1.61
******************************************************
* Charles	28 dec 1994	: ajout de XDG_IABTINIT_ETOR V1.62
******************************************************
* F. VOLCIC	02 Jan 1995	: Rien V1.63
******************************************************
* F. VOLCIC	02 Jan 1995	: Mise a jour Histo V1.64
******************************************************
* Lecrivain     02 Jan 1995   : Ajout XDG_NUL (pseudo-abonnement Operator) V1.65
******************************************************
* P GABERT      04 Jan 1995   : Ajout XDG_ECMD_SATIR V1.66
******************************************************
*  GABORIT      11 Jan 1995   : Ajout XDG_AH_INITLT et XDG_AH_RADT_DAI V1.67
***************************************************
*  NIEPCERON    12 Jan 1995   : Ajout XDG_IDONNEES_RETARD V1.68
***************************************************
*  LECRIVAIN    12 Jan 1995   : Ajout XDG_IARCHIVAGE V1.70
******************************************************
*  GABORIT      16 Jan 1995   : Ajout XDG_AH_SATIR V1.71
******************************************************
*  NIECPERON      16 Jan 1995   : Reduction de la long des datagroup pour les courbes V1.73
******************************************************
*  GABORIT      16 Jan 1995   :Reduction de la long des datagroup pour BASEDD 1.76
*
*  T.Milleville      20 Jan 1995   :Ajout du Datagroup XDG_NSUPDISK utile
*					pour la synchronisation des disques
*					miroirs V1.78
*
*  MISMER       24 Jan 1995   : Ajout XDG_ESUR (Surveillance du LT voisin) V1.79
*  GABORIT      26 Jan 1995   : ajout XDG_AEQA 1.80
*  NIEPCERON	22 Fev 1995   : pour volcic (qui est sur le terrain et donc injoingnable, si , si ):
				Ajout XDG_MODE_LENT XDG_EInit_OP_VW			v 1.81
*  NIEPCERON	22 Fev 1995   : Ajout XDG_IETAT_INIEQT pour init des etat eqts v1.82
* T.Milleville 23 Fev 1995	: Ajout du DataGroup XDG_NSUPINDQUA	V 1.83
* T.Milleville 28 Fev 1995	: Modification Nom du dg XDG_TIME V1.84
* F.Volcic     15 Mar 1995	: Ajout du dg XDG_MODE_LENT et XDG_ETAT_LIAISON V1.85
*  NIEPCERON	23 Mai 1995   : Ajout XDG_IRAZ_MOB pour la remise a zero des mobiles  v1.86
*  NIEPCERON    24 Mai 1995   : Correction sur la def de XDG_IRAZ_MOB v1.88
*  NIEPCERON    24 Mai 1995   : Correction sur la def de XDG_IRAZ_MOB v1.89
*  NIEPCERON	06 Jun 1995   : Ajout XDG_IREI_EQT pour la reinitialisation des eqt  v1.90
*  MISMER	19 Jun 1995   : Ajout XDG_EMOD_DEG pour la surveillance LT	     v1.91
* F.Volcic      01 Aou 1995 	: Ajout du dg XDG_ACKNOLEDGE V1.92
* F.Volcic      10 Aou 1995	: Ajout du dg XDG_EMES_RADT_6GL_V et XDG_EMES_RADT_6MN_V  V1.93
* F.Volcic      10 Aou 1995	: Ajout du dg XDG_EMES_RADT_V  V1.94
* D.Mismer      21 Sep 1995	: Ajout du dg XDG_ETAT_MAT_NAV  et du dg XDG_ACKNOLEDGE_ALA V1.95
* GABORIT	27 Dec 1995	: Ajout des dg du ss-systeme MAINTE v1.96
* GUILHOU	22 Aou 1996 	: Ajout des dg EXTENSION OUEST v1.97
* NIEPCERON     27 Aou 1996	: Ajout dg XDG_IAX_INITPRES v1.98
* D.Mismer      16 Sep 1996	: Ajout du dg XDG_EMES_RADT_6mn_R  (RADT_W) V1.99
* NIEPCERON     27 Sep 1996     : Ajout du XDG_IAX_ACQT (dem/424) v1.100
* D.Mismer      16 Sep 1996	: Ajout du dg  XDG_EMES_PESAGE (Pesage RADT) (DEM/1227) V1.101
* Orengo.A	07 Nov 1996	: Ajout du XDG_ECMD_QTX ( METEO ) (DEM/1232) V1.102
* NIEPCERON     17 Jan 1997     : Ajout du XDG_IABT_PAN (dem/1373) v1.103
* NIEPCERON     03 Fev 1997	: Ajout de XDG_IDONNEES_TDP tdp v1.104
* NIEPCERON     04 Fev 1997     : Ajout de XDG_AM_INIT_TDP tdp v1.105
* GUILHOU	24 Fev 1997	: Ajout de XDG_S_TDP6, XDG_ETDP_SEUIL tdp 
*			  ajout de XDG_IABTINIT_TDP v1.106
* VATHELOT	29 Avr 1997	: Ajout de XDG_AS_TIMER_SRV_RADT et XDG_AS_TIMER_PURGE_RADT (Dem Strada) v1.106
* PENOT		05 Mai 1997	: Ajout de XDG_AS_TIMER_CLT_RADT (Dem Strada) v1.107
* Guilhou	05 mai 1997	: ajout de XDG_IDONNEES_TRAFIC_STRADA	1.108
* Guilhou	08 mai 1997	: ajout de XDG_AS_TIMER_SRVEVT_STRADA (Strada) 1.109
* PENOT		13 Mai 1997	: Ajout de XDG_AS_TIMER_CLT_EVT (Dem Strada) v1.107	
* Guilhou	13 mai 1997	: renommage de XDG_AS_TIMER_CLT_EVT 1.109
* Guilhou	04 nov 1997	: ajout de XDG_AS_EVT_STRADA (dem strada) 1.110
* Penot		07 Nov 1997	: ajout de XDG_IAEVT_STRADA
* D.Mismer      19 Jan 1998	: Ajout du dg XDG_ECMD_PAL  XDG_EInit_IHM_PAL XDG_ETAT_PAL (DEM/) V1.111
******************************************************
* NIEPCERON     05 Mai 1998	: ajout de XDG_A_TIMER_FMC_LOC V1.112
* NIEPCERON     19 Nov 1998	: Ajout de XDG_AG_IMPCAT, XDG_AG_EXPCAT, XDG_IPMV_DATEX v1.113
* JMG		16 jan 2001 	: ajout de XDG_AW_TIMER 1.114
* JMG		07 dec 2001 	: ajout de XDG_NOMADE_TIMER 1.115
* ETR		18 jui 2002     : ajout de XDG_AMISOL_TIMER
* JMG		16/11/04	: ajout de XDG_EPAL_AUTO
* JMG		13/09/05	: ajout PMVA BAF BAD
* JMG		13/09/05	: ajout XDG_ETDP_PMVA 1.117
* JMG		11/10/05	: remplacement  TipcSrvDgSetWatch par TipcMonSubjectSubscribeSetWatch 1.118
				  ajout XDG_ENAV_AUTO
* AAZ       30/08/2007  : DEM 662 ajout XDG_ETAT_SEQ_SIG, XDG_IABTINIT_SIG,
*                               XDG_ETAT_SEQ_ECL, XDG_IABTINIT_ECL,
*                               XDG_ETAT_SEQ_VEN, XDG_IABTINIT_VEN,
*                               XDG_ETAT_SEQ_ENR, XDG_IABTINIT_ENR
*                               XDG_ETAT_SEQ_INC, XDG_IABTINIT_INC,
*                               XDG_IABTINIT_EXP,
*                               XDG_IABTINIT_SYS
* JMG		15/09/08	ajout XDG_ATDP_TIMER DEM 837
* JMG		12/10/08	ajout XDG_AGEN_TIMER DEM 837
* JMG		22/11/10	ajout XDG_ECMD_AUTORAD, XDG_IMASQUE, XDG_IVUE, XDG_IGRAPHE, XDG_IPLANSECOURS 
* JMG		15/02/11	ajout XDG_ACOP_TIMER 
* JPL		21/12/11	ajout XDG_ITDP_TIMER (DEM 1007)  1.125
* SDU		20/02/12	ajout Communautaire (DEM/1015)
* VR		25/05/12	ajout PRV DEM/1016
* JPL		29/01/13	ajout XDG_ANEUTR_TIMER (DEM 1061)  1.128
* JMG		24/02/14	ajout XDG_IMODIF_ALARMES_CRIT 1.129
* PNI		15/12/14	ajout XDG_KEEPALIVE 1.130 DEM1116
* JMG		01/3/16		ajout XDG_TIMER_SAGA 1.131
* JMG		16/04/16 	ajout SAGA 1.132
* JMG		11/12/17	ajout polyvox 1.133
* JM		15/01/18 	ajout GABEB 1.134
* LCL		22/02/18	ajout CTRL_FEUX 1.135 DEM1284
* LCL		11/05/18	modification XDG_ETAT_CAB en XDG_ETAT_CAP 1.136 DEM1283
* JMG		28/08/18	ajout XDG_ETEMIP suppression SAGA 1.137
* JMG		29/10/18	ajout XDG_RMVI 1.138
* JMG		30/11/18	ajout XDG_AVA AVA 1.139
* JMG		14/11/19	ajout XDG_ETDP_SURCHARGE DEM1365 1.140
* JMG		10/01/20	ajout IMU 1.141
* ABE		30/03/20	ajout TIMER Datex2 DEM 130 1.141
* ABE		22/07/20	ajout XDG_TIMER_PAT DEM149 1.142
* GR            30/07/20        ajout XDG_ETDP_FCD DEM-SAE152 1.143
* ABE		24/09/20	ajout XDG_TIMER_TAGEN DEM-SAE152 1.144
* ABE		15/12/20	ajout XDG pour IMU DEM-SAE155 1.145
* jmg		10/02/21 	ajout XDG_API MAGISTRA 1.146
* jmg		03/03/21	ajout XDG_TITRE 1.146
* ABE		02/04/21	ajout XDG RAU DEM-SAE244 1.147
* ABE       06/12/21    Ajout DG Magistra XDG_MGA_CONFIG_PO  1.148
* ABE		17/12/21	Ajout XDG_MGA_REMOTE_CMD pour app externe MAGISTRA SAE311 1.149
* LCL		20/12/21	: MOVIS P2 ems SAE-312
* ABE       18/01/21 	ajout DG AVA XDG_API_GW_AVA DEM-SAE367
******************************************************/
/******************************************************
* TESTS UNITAIRES  : RELECTURE CROISEE
*
* NOM DU RELECTEUR 
* DATE             : 
* COMMENTAIRES     :
*
*******************************************************/


#ifndef xdg
#define xdg

/* fichiers inclus */
#include "xzsc.h"

/* definitions de constantes exportees */
#define 	XDG_NUL				""


#define 	XDG_API			"API"
#define 	XDG_ACT			"ACT"
#define 	XDG_TITRE			"TITRE"
#define 	XDG_TIME			"TIME"
#define 	XDG_NSUPAPLG			"NSUPAPLG"
#define 	XDG_NSUPADMI			"NSUPADMI"
#define 	XDG_NSUPDIFA			"NSUPDIFA"
#define 	XDG_NSUPSYNCHRO			"NSUPSYNCHRO"

#define         XDG_NPINGINIT                   "NPINGINIT"
#define         XDG_NTIMEINIT                   "NTIMEINIT"

#define 	XDG_NSUPHIST			"NSUPHIST"
#define		XDG_NSUPETAT			"NSUPETAT"
#define		XDG_NSUPVIE			"NSUPVIE"
#define 	XDG_NSUPTIME			"NSUPTIME"
#define		XDG_NSUPPING			"NSUPPING"
#define 	XDG_NSUPHORL			"NSUPHORL"
#define 	XDG_NSUPDISK			"NSUPDISK"
#define 	XDG_NSUPINDQUA			"NSUPINDQUA"

#define 	XDG_KEEPALIVE			"KEEPALIVE"

#define 	XDG_GESCONFIG			"GESCONFIG"
#define 	XDG_CBDARCHIVE			"CBDARCHIVE"

#define		XDG_REGUL_DETECTION		"REGUL_DETECTION"
#define		XDG_REGUL_SIMU			"REGUL_SIMU"
#define		XDG_REGUL_START_SIMU	"REGUL_START_SIMU"
#define		XDG_DEMANDE_SIMU		"DEMANDE_SIMU"
#define		XDG_EREG_FMC			"EREG_FMC"
#define		XDG_ETEMIP			"ETEMIP"
#define		XDG_ETEOPC			"ETEOPC"

#define XDG_REGUL_APPLICATION		"REGUL_APPLICATION"
#define XDG_ETAT_PRV				"ETAT_PRV"

#define XDG_GABEB_DETECTION	"GABEB_DETECTION"
#define XDG_GABEB_FMC	"GABEB_FMC"
#define XDG_GABEB_APPLICATION	"GABEB_APPLICATION"
#define XDG_ETAT_CAP		"ETAT_CAP"

/* Declaration des Datagroup pour le sous-systeme EQUEXT */

#define		XDG_EINIT			"EINIT"
#define		XDG_ESUR			"ESUR"
#define		XDG_EINIT_ABT			"EINIT_ABT"
#define		XDG_EFIN_CONFIG			"EFIN_CONFIG"
#define		XDG_EFIN_CONFIG_SUR		"EFIN_CONFIG_SUR"
#define		XDG_ECONFIG_EQPT		"ECONFIG_EQPT"
#define		XDG_ECTL_EQPT			"ECTL_EQPT"
#define		XDG_ECMD			"ECMD"
#define		XDG_ECMD_RAU			"ECMD_RAU"
#define		XDG_ECMD_QTX			"ECMD_QTX"
#define		XDG_ECMD_TDP			"ECMD_TDP"
#define         XDG_EETAT_RAU                   "EETAT_RAU"
#define		XDG_ECMD_VIDEO			"ECMD_VIDEO"
#define         XDG_EETAT_VIDEO                 "EETAT_VIDEO"
#define		XDG_ECMD_AUTOTEL		"ECMD_AUTOTEL"
#define         XDG_EETAT_AUTOTEL               "EETAT_AUTOTEL"
#define		XDG_ECMD_OPER			"ECMD_OPER"
#define         XDG_EETAT_OPER               	"EETAT_OPER"
#define		XDG_ECPT_LCR			"ECPT_LCR"
#define		XDG_EMES_RADT_6MN_R		"EMES_6MN_R"
#define		XDG_EMES_RADT_6MN_V		"EMES_6MN_V"
#define		XDG_EMES_RADT_6MN		"EMES_6MN"
#define		XDG_EMES_RADT_6mn_R		"EMES_6MN_R"
#define		XDG_EMES_RADT_6GL_R		"EMES_6GL_R"
#define		XDG_EMES_RADT_6GL_V		"EMES_6GL_V"
#define		XDG_EMES_RADT_V			"EMES_V"
#define		XDG_EMES_CLASSE_DIV		"EMES_CL_DIV"
#define		XDG_ENIVEAU_SERVICE		"ENIV_SERV"
#define		XDG_EMES_POIDS			"EMES_POIDS"
#define		XDG_EMES_DAI			"EMES_DAI"
#define		XDG_EMES_UGTP_6MN		"EMES_TDP_6MN"
#define		XDG_EMES_UGTP_HORAIRE		"EMES_TDP_1H"
#define		XDG_ETAT_PMV			"ETAT_PMV"
#define		XDG_ETAT_PICTO			"ETAT_PICTO"
#define		XDG_ETAT_NAV			"ETAT_NAV"
#define		XDG_ETAT_MAT_NAV		"ETAT_MAT_NAV"
#define		XDG_ETAT_GTC			"ETAT_GTC"
#define		XDG_ECMD_PMV			"ECMD_PMV"
#define		XDG_ECMD_PICTO			"ECMD_PICTO"
#define		XDG_ECMD_CONTRASTE		"ECMD_CONT"
#define		XDG_ECMD_NAV			"ECMD_NAV"
#define		XDG_ECMD_SCEN			"ECMD_SCEN"
#define		XDG_ECMD_SCEN_FICH		"ECMD_SCN_FICH"
#define		XDG_ECMD_GTC			"ECMD_GTC"
#define		XDG_ECMD_DAI			"ECMD_DAI"
#define		XDG_ECONF_VOIE_DAI		"ECONF_V_DAI"
#define		XDG_ECONF_VOIE_RADT		"ECONF_V_RADT"
#define		XDG_EDEM_INDIV			"EDEM_INDIV"
#define		XDG_ECMD_GTC_NICE		"ECMD_GTC_NICE"
#define 	XDG_ETAT_TELEALARME		"ETAT_TELEALM"
#define 	XDG_ETAT_PANNEAU		"ETAT_PANNEAU"
#define 	XDG_ETAT_SEQ_TUBE		"ETAT_SEQ_TUBE"
#define 	XDG_ETAT_SEQ_ECH		"ETAT_SEQ_ECH"
#define 	XDG_ETAT_CAPTEUR		"ETAT_CAPTEUR"
#define		XDG_ETAT_CO			"ETAT_CO"
#define		XDG_ETAT_OPACITE		"ETAT_OPACITE"
/* AAZ */
#define     XDG_ETAT_SEQ_SIG                "ETAT_SEQ_SIG"
#define     XDG_ETAT_SEQ_ECL                "ETAT_SEQ_ECL"
#define     XDG_ETAT_SEQ_VEN                "ETAT_SEQ_VEN"
#define     XDG_ETAT_SEQ_ENR                "ETAT_SEQ_ENR"
#define     XDG_ETAT_SEQ_INC                "ETAT_SEQ_INC"
#define     XDG_ETAT_SEQ_EXP                "ETAT_SEQ_EXP"
#define 	XDG_ETAT_EQUIPEMENT		        "ETAT_EQUIPEMENT"
#define     XDG_ETAT_DOMAINE                "ETAT_DOMAINE"
#define     XDG_ETAT_ALERTE                 "ETAT_ALERTE"
#define     XDG_ETAT_ALARME                 "ETAT_ALARME"

#define		XDG_EINIT_ABT_RAU		"EINIT_ABT_RAU"
#define		XDG_EINIT_ABT_VIDEO		"EINIT_ABT_VIDEO"
#define		XDG_EINIT_ABT_AUTOTEL		"EINIT_ABT_AUTOTEL"
#define		XDG_EINIT_ABT_OPER		"EINIT_ABT_OPER"
#define		XDG_EINIT_ABT_TDP		"EINIT_ABT_TDP"
#define		XDG_EInit_IHM_NICE		"EInit_IHM_NICE"
#define		XDG_EInit_IHM_RADT		"EInit_IHM_RADT"
#define		XDG_EInit_IHM_PMV		"EInit_IHM_PMV"
#define		XDG_EInit_IHM_IMU		"EInit_IHM_IMU"
#define		XDG_EInit_IHM_Picto		"EInit_IHM_Picto"
#define		XDG_EInit_IHM_NAV		"EInit_IHM_NAV"
#define		XDG_EInit_IHM_DAI		"EInit_IHM_DAI"
#define		XDG_EInit_IHM_GTC		"EInit_IHM_GTC"
#define		XDG_EInit_IHM_CONTRASTE		"EInit_IHM_CONT"
#define		XDG_EInit_IHM_JOURNUIT		"EInit_IHM_JN"
#define		XDG_ETAT_CONTRASTE		"ETAT_CONTRASTE"
#define		XDG_ETAT_JOURNUIT		"ETAT_JOURNUIT"
#define		XDG_ECMD_MAT_NAV		"ECMD_MAT_NAV"
#define		XDG_ECMD_ARRET_SCEN		"ECMD_ARRET_SCEN"
#define		XDG_ECMD_RADT			"ECMD_RADT"
#define		XDG_FEN_LCR			"FEN_LCR"
#define		XDG_ECMD_SATIR			"ECMD_SATIR"
#define		XDG_EInit_OP_VW			"EInit_OP_VW"
#define		XDG_EInit_IHM_Liaison		"EInit_IHM_Liaison"
#define		XDG_EMOD_DEG		        "EMOD_DEG"
#define		XDG_EMOD_ISOLE		        "EMOD_ISOLE"
#define		XDG_ACKNOLEDGE		        "ACKNOLEDGE"
#define		XDG_ACKNOLEDGE_ALA		"ACKNOLEDGE_ALA"
#define		XDG_EMES_PESAGE			"EMES_PESAGE"
#define		XDG_DONNEES_METEO_QTX		"DONNEES_METEO_QTX"
#define         XDG_ETDP_SEUIL			"ETDP_SEUIL"
#define		XDG_ETDP_PMV			"ETDP_PMV"
#define		XDG_ECMD_PAL			"ECMD_PAL"
#define		XDG_EInit_IHM_PAL		"EInit_IHM_PAL"
#define		XDG_ETAT_PAL			"ETAT_PAL"
#define		XDG_EPAL_AUTO			"EPAL_AUTO"
#define		XDG_ENAV_AUTO			"ENAV_AUTO"
#define		XDG_EPMV_AUTO			"EPMV_AUTO"
#define		XDG_EBAF_AUTO			"EBAF_AUTO"
#define		XDG_EBAD_AUTO			"EBAD_AUTO"
#define		XDG_EPMVA_AUTO			"EPMVA_AUTO"
#define		XDG_EPRV_AUTO			"EPRV_AUTO"
#define		XDG_ETUNNEL_AUTO		"ETUNNEL_AUTO"
#define         XDG_ECMD_AUTORAD                "ECMD_AUTORAD"
#define 	XDG_RMVI			"RMVI"
#define 	XDG_AVA			"AVA"
#define 	XDG_ETDP_SURCHARGE	"ETDP_SURCHARGE"

/*PMVA BAF BAD*/
#define XDG_ETAT_PMVA	"ETAT_PMVA"
#define XDG_ETAT_PRV		"ETAT_PRV"
#define XDG_ETAT_PIC		"ETAT_PIC"
#define XDG_ETAT_BAF		"ETAT_BAF"
#define XDG_ETAT_BAD		"ETAT_BAD"
#define XDG_ECMD_PMVA	"ECMD_PMVA"
#define XDG_ECMD_BAF		"ECMD_BAF"
#define XDG_ECMD_BAD		"ECMD_BAD"
#define XDG_ECMD_PRV		"ECMD_PRV"
#define XDG_ECMD_PIC		"ECMD_PIC"
#define XDG_ECMD_SCEN_BAD	"ECMD_SCEN_BAD"
#define XDG_ECMD_ARRET_SCEN_BAD		"ECMD_ARRET_SCEN_BAD"
#define XDG_EInit_IHM_PMVA		"EInit_IHM_PMVA"
#define XDG_EInit_IHM_BAF		"EInit_IHM_BAF"
#define XDG_EInit_IHM_BAD		"EInit_IHM_BAD"
#define XDG_EInit_IHM_PRV		"EInit_IHM_PRV"
#define XDG_EInit_IHM_PIC		"EInit_IHM_PIC"
#define XDG_ETDP_PMVA	"ETDP_PMVA"

#define XDG_ECMD_EQT_SAGA	"ECMD_EQT_SAGA"
#define XDG_EInit_IHM_EQT_SAGA	"EInit_IHM_EQT_SAGA"
#define XDG_ETAT_EQT_SAGA		"ETAT_EQT_SAGA"
#define XDG_ACQUITTE_SAGA	"ACQUITTE_SAGA"

#define XDG_ETAT_CFE				"ETAT_CFE"
#define XDG_ECMD_CFE				"ECMD_CFE"
#define XDG_EInit_IHM_CFE			"EInit_IHM_CFE"
#define XDG_IABTINIT_CFE			"IABT_CFE"

#define XDG_ETAT_IMU 				"ETAT_IMU"
#define XDG_ECMD_IMU 				"ECMD_IMU"
#define XDG_EInit_IHM_IMU 			"EInit_IHM_IMU"
#define XDG_IABTINIT_IMU 			"IABT_IMU"

#define XDG_ETAT_SONO 				"ETAT_RAU_SONO"
#define XDG_ECMD_SONO 				"ECMD_RAU_SONO"
#define XDG_EInit_IHM_SONO 			"EInit_IHM_RAU_SONO"
#define XDG_IABTINIT_SONO 			"IABT_RAU_SONO"

/* Declaration des Datagroup pour le sous-systeme BASEDD */

#define		XDG_AT			"AT"
#define		XDG_AA			"AA"
#define		XDG_AM			"AM"
#define		XDG_A_IHM		"A_IHM"
#define		XDG_A_TIMER_RAFRAI_IHM	"A_TIMER_IHM"
#define		XDG_AN			"AN"
#define		XDG_AG			"AG"
#define         XDG_AG_IMPCAT           "AG_IMPCAT"
#define         XDG_AG_EXPCATC           "AG_EXPCATC"
#define         XDG_AG_EXPCATF		"AG_EXPCATF"
#define         XDG_AM_INIT_MES6MnsVoie "AM_6MnsVoie"
#define         XDG_AM_INIT_MES6MnsCh   "AM_6MnsCh"
#define         XDG_AM_INIT_MESPOIDS    "AM_POIDS"
#define         XDG_AM_INIT_NivServ   	"AM_NivServ"
#define 	XDG_AM_INIT_TDP		"AM_TDP"
#define         XDG_AS_COURBE   	"AS_COURBE"
#define		XDG_AH_INITLT		"AH_INITLT"
#define         XDG_AH_RADT_DAI		"AH_RADT_DAI"
#define         XDG_AH_SATIR		"AH_SATIR"
#define         XDG_AEQA		"AEQA"
#define 	XDG_A_TIMER_TRV_COURS	"A_TIMER_TRV_COURS"
#define 	XDG_A_PRES_OP		"A_PRES_OP"
#define		XDG_A_TIMER_RAF_IHM_BLOC	"A_TIMER_IHM_BLOC"
#define         XDG_A_TIMER_FMC_LOC	"A_TIMER_FMC_BLOC"
#define		XDG_AS_TIMER_SRV_RADT	"AS_TIMER_SRV_RADT"
#define		XDG_AS_TIMER_PURGE_RADT	"AS_TIMER_PURGE_RADT"
#define		XDG_AS_TIMER_CLT_RADT	"AS_TIMER_CLT_RADT"
#define 	XDG_AS_TIMER_SRVEVT_STRADA	"AS_TIMER_SRVEVT_STRADA"
#define		XDG_AS_SRVEVT_STRADA		"AS_SRVEVT_STRADA"
#define         XDG_AS_TIMER_CLT_EVT   "AS_TIMER_CLT_EVT"
#define		XDG_AS_EVT_STRADA	"AS_EVT_STRADA"
#define         XDG_MODIF_STRADA	"A_MODIF_STRA"
#define		XDG_AW_TIMER		"AW_TIMER"
#define		XDG_NOMADE_TIMER	"NOMADE_TIMER"
#define         XDG_AMISOL_TIMER        "AMISOL_TIMER"
#define         XDG_ATDP_TIMER          "ATDP_TIMER"
#define         XDG_AGEN_TIMER          "AGEN_TIMER"
#define         XDG_ACOP_TIMER          "ACOP_TIMER"
#define         XDG_AS_COMMU_FMC        "AS_COMMU_FMC"
#define         XDG_ANEUTR_TIMER        "ANEUTR_TIMER"
#define		XDG_TIMER_SAGA		"TIMER_SAGA"
#define		XDG_TIMER_SAGA_EQT	"TIMER_SAGA_EQT"
#define 	XDG_TIMER_POLYVOX	"TIMER_POLYVOX"
#define		XDG_TIMER_DATEX2	"TIMER_DATEX2"
#define		XDG_TIMER_TAGEN		"TIMER_TAGEN"
#define		XDG_TIMER_PAT		"TIMER_PAT"
#define         XDG_ETDP_FCD            "ETDP_FCD"

/* definition des datagroups pour le sous systeme GESIHM*/
#define 	XDG_AXMCE			"AXMCE"
#define		XDG_AXDIAL			"AXDIAL"
#define		XDG_IMESSAGE_TFM_PRET		"IMESS_TFM"
#define		XDG_IPA_PRET			"IPA_PRET"
/*#define		XDG_IAX_TEXTE		        "/IAX_TEXTE" 
#define		XDG_IAX_TEXTE_EMS	        "IAX_TEXTE"
#define		XDG_IAX_SYNOP		        "/IAX_SYNOP"
*/
#define		XDG_IAX_TEXTE		        "/IAX_TEXTE" 
#define		XDG_IAX_TEXTE_EMS	        "IAX_TEXTE"
#define		XDG_IAX_TEXTE_EMS_SS	    ".IAX_TEXTE"
#define		XDG_IAX_SYNOP		        "IAX_SYNOP"
#define 	XDG_ICREER_ALERTE		"ICREER_ALT"
#define 	XDG_ISUPPRIMER_ALERTE		"ISUPP_ALT"
#define 	XDG_IAVERT_OPERATEUR		"IAVERT_OP"
#define 	XDG_IAVERT_ADMINISTRATEUR	"IAVERT_ADM"
#define 	XDG_IDEMANDE_SURCHARGE		"IDEM_SURC"
#define 	XDG_IREPONSE_SURCHARGE		"IREP_SUR"
#define		XDG_IABTINIT_PMV 	        "IABT_PMV"
#define		XDG_IABTINIT_IMU 	        "IABT_IMU"
#define		XDG_IABTINIT_PICTO 	        "IABT_PICTO"
#define		XDG_IABTINIT_RADT 	        "IABT_RADT"
#define		XDG_IABTINIT_NIV 	        "IABT_NIV"
#define		XDG_IABTINIT_NAV 	        "IABT_NAV"
#define		XDG_IABTINIT_RAU 	        "IABT_RAU"
#define		XDG_IABTINIT_CAMERA 	        "IABT_CAM"
#define		XDG_IABTINIT_DAI 	        "IABT_DAI"
#define		XDG_IABTINIT_DIV 	        "IABT_DIV"
#define		XDG_IABTINIT_JOURNUIT 	        "IABT_JN"
#define		XDG_IABTINIT_PAN 	        "IABT_PAN"
#define		XDG_IABTINIT_PAL 	        "IABT_PAL"
#define		XDG_IABTINIT_TUB 	        "IABT_TUB"
#define		XDG_IABTINIT_ECH	        "IABT_ECH"
#define		XDG_IABTINIT_ETOR	        "IABT_ETOR"
#define         XDG_IABTINIT_TDP		"IABT_TDP"
#define 	XDG_IMODIF_ALARMES		"IMODIF_ALM"
#define 	XDG_IMODIF_ALARMES_CRIT		"IMODIF_ALM_CRIT"
#define 	XDG_IMODIF_FMC			"IMODIF_FMC"
#define         XDG_IHM_ADM                     "IHM_ADM"
#define         XDG_IMODIF_LISTE_TFM		"IMODIF_TFM"
#define         XDG_IARCHIVAGE			"IARCHIVAGE"
#define         XDG_ITDP_TIMER			"ITDP_TIMER"

/* AAZ  */
#define     XDG_IABTINIT_SIG            "IABT_SIG"
#define     XDG_IABTINIT_ECL            "IABT_ECL"
#define     XDG_IABTINIT_VEN            "IABT_VEN"
#define     XDG_IABTINIT_ENR            "IABT_ENR"
#define     XDG_IABTINIT_INC            "IABT_INC"
#define     XDG_IABTINIT_EXP            "IABT_EXP"
#define     XDG_IABTINIT_SYS            "IABT_SYS"
#define     XDG_IABTINIT_LTN            "IABT_LTN"


#define XDG_IABTINIT_PMVA		"IABT_PMVA"
#define XDG_IABTINIT_BAF		"IABT_BAF"
#define XDG_IABTINIT_BAD		"IABT_BAD"
#define XDG_IABTINIT_PRV		"IABT_PRV"
#define XDG_IABTINIT_PIC		"IABT_PIC"
#define XDG_IABTINIT_SAGA		"IABT_SAGA"

#define		XDG_IAX_FINPOSTE		"IAX_FINPOSTE"
#define         XDG_IAX_INITPRES		"IAX_INITPRES"
#define         XDG_IAX_ACQT			"IAX_ACQT"

#define		XDG_IRAF_DAI 	                "IRAF_DAI"

#define 	XDG_IETAT_DISPO_EQT                 "IETAT_DSP_EQT"
#define 	XDG_IRAF_BOUCHON_CHAUSSEE_PORTION   "IRAF_B_PORT"
#define 	XDG_IRAF_VOIE_NEUTRALISEE           "IRAF_V_NEUT"
#define 	XDG_IRAF_VOIE_RADT_INVERSE          "IRAF_V_RADTINV"
#define	 	XDG_IRAF_BASCULEMENT_BOUCHON_VOIE   "IRAF_BASCVOIE"
#define 	XDG_IRAF_OBJET_MOBILE_PORTION       "IRAF_O_MOB_POR"
#define 	XDG_IRAF_OBJET_MOBILE_ECHANG        "IRAF_O_MOB_ECH"
#define 	XDG_IRAF_PORT_AFF_VOIES             "IRAF_PORT_AF_V"
#define		XDG_IRAF_FMCLOC		            "IRAF_FMCLOC"
#define		XDG_IRAF_DEBUT			    "IRAF_DEBUT"
#define		XDG_IRAF_FIN			    "IRAF_FIN"

#define 	XDG_IPOSIT_INDIC_ALARME             "IPOSIT_IND_ALRM"

#define 	XDG_IRAF_POURCENT                   "IRAF_POURCENT"

#define 	XDG_IRAF_COURBE                     "IRAF_COURBE"

#define 	XDG_ISYN_SERVEUR                    "ISYN_SERVEUR"


#define 	XDG_IABTINIT_RADT_COURBES           "IABTINI_RADTC"

#define 	XDG_IDONNEES_FT                     "IDONNEES_FT"

#define 	XDG_IDONNEES_INRETS_PR              "IINRETS_PR"

#define		XDG_IDONNEES_INRETS_TRA             "IINRETS_TRA"

#define		XDG_IDONNEES_BOUCHONS               "IBOUCHONS"

#define         XDG_IDONNEES_TDP		    "ITDP"

#define		XDG_IDONNEES_QVTO              	    "IQVTO"

#define		XDG_IDONNEES_RETARD                 "IRETARD"

#define		XDG_IETAT_INIEQT	            "IETAT_INIEQT"

#define		XDG_IRAZ_MOB			    "IRAZ_MOB"

#define		XDG_IREI_EQT			    "IREI_EQT"

#define		XDG_MODE_LENT	                    "MODE_LENT"
#define		XDG_ETAT_LIAISON	            "ETAT_LIAISON"

#define 	XDG_IDONNEES_TRAFIC_STRADA	    "ITRAFIC_STRADA"
#define		XDG_IAEVT_STRADA		    "IAEVT_STRADA"
#define         XDG_IPMV_DATEX			    "IPMV_DATEX"

#define         XDG_IMASQUE                     "IMASQUE"
#define         XDG_IVUE                        "IVUE"
#define         XDG_IGRAPHE                        "IGRAPHE"
#define         XDG_IPLANSECOURS              "IPLANSECOURS"

#define         XDG_COMMUNAUTAIRE               "COMMUNAUTAIRE"

#define         XDG_API_GW_TEL               "API_GW_TEL"
#define 	XDG_MGA_CONFIG_PO "MGA_CONFIG_PO"
#define 	XDG_MGA_REMOTE_CMD "MGA_REMOTE_CMD"
/* GATEWAYS */
#define 		XDG_API_GW_AVA			"API_GW_AVA"
#define 		XDG_API_GW_PIT_BOUCHON	"API_GW_PIT_BOUCHON"
#define			XDG_API_GW_RAD			"API_GW_RAD"

/* Declaration des Datagroup pour le sous-systeme MAINTE */

#define		XDG_M_COMPTEUR			"M_COMPTEUR"
#define		XDG_M_TIMER_COMPTEUR		"M_TIMER_CPT"
#define		XDG_M_COMPTEUR_PRET		"M_CPT_PRET"

/* Declaration des Datagroup pour le sous-systeme SUPERV */
#define		XDG_S_TDP6			"S_TDP6"

#define         XDG_WATCH		"S_WATCH"

/* definitions des positions de champs dans les noms de datagroup */
#define XDG_POS_NSUPETAT_NOMMACHINE 2
#define XDG_NSUPETAT_SEPARATEUR "_"

/* definitions de types exportes */

/* definition de macro exportees */
#define XDG_EncodeDG2( p, p1 , p2 ) \
 sprintf((p),"%s_%s",(p1),(p2))

#define XDG_EncodeDG3( p, p1 , p2 , p3 ) \
 sprintf((p),"%s_%s_%s",(p1),(p2),(p3))

#ifndef _TIBCO_EMS
#define XDG_JoindreDG( dg ) \
 TipcSrvDgSetRecv( dg, T_TRUE )

#define XDG_QuitterDG( dg ) \
 TipcSrvDgSetRecv( dg, T_FALSE )

#endif

/*
#define XDG_SurveillerDG( dg ) \
 TipcSrvDgSetWatch( dg, TRUE )

#define XDG_AnnulerSurvDG( dg ) \
 TipcSrvDgSetWatch( dg, FALSE )
*/

#define XDG_SurveillerDG( dg ) \
 TipcMonSubjectSubscribeSetWatch( dg, T_TRUE)

#define XDG_AnnulerSurvDG( dg ) \
 TipcMonSubjectSubscribeSetWatch( dg, T_FALSE)

/* declarations de donnees globales */

/* delaration de fonctions externes */


/* Fichier : $Id: xdg.h,v 1.147 2021/05/03 14:05:18 pc2dpdy Exp $        Release : $Revision: 1.147 $        Date : $Date: 2021/05/03 14:05:18 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XDG 
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* 
******************************************************
* SEQUENCE D'APPEL :
* XDG_strmot(pa_Datagroup, va_mot, va_separateur, pa_nom); 
*
* Arguments
* Datagroup 	: Nom du datagroup
* mot		: numero du mot a extraire
* separateur	: caractere de separation de deux mots
* mot		: mot extrait
*
* codes retour 
* XDC_OK  	: si OK
* XDC_NOK 	: si le caractere de separation ne 
*             	  figure pas dans la chaine si le numero
*		  du mot a extraire est superieur au 
*		  nombre de mot delimite par les 
*		  caracteres de separation dans la
*		  chaine Datagroup
******************************************************/
 
extern int XDG_strmot(XZSCT_Datagroup pa_Datagroup, 
		  int va_mot, 
		  char *va_separateur, 
		  char *pa_nom); 




#endif
