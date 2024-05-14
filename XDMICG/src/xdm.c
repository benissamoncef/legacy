/*E*/
/* Fichier : $Id: xdm.c,v 1.240 2021/06/10 08:47:20 pc2dpdy Exp $        Release : $Revision: 1.240 $        Date : $Date: 2021/06/10 08:47:20 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE xdm * FICHIER xdm.c
******************************************************
* DESCRIPTION DU MODULE :
*
*	Liste des messages avec leur grammaire associe
*
******************************************************
* HISTORIQUE :
*
* T.Millevillei	23 Aou 19940	: Creation
* D.Mismer	16 Sep 1994	: Ajout messages EQUEXT
* JM.Guilhou	29 Sep 1994	: Ajout messages Applix
* T.Milleville  29 Sep 1994 	: Ajout duMsg XDM_TST_TSUP   		V 1.10
* D.Mismer	30 Sep 1994 	: Ajout des Msg XDM_EMES_6mn_Voie, 
*				XDM_EMES_6mn_Chaussee,
*				XDM_EMES_6gl_Voie, XDM_EMES_6gl_Chaussee
* T.Milleville  30 Sep 1994     : Ajout des messages 
*				XDM_TSG_VERIF, XDM_TSG_INIT
*				XDM_TSE_APPLI, XDM_TSE_RES, XDM_TSE_TOUT
*				XDM_TSE_VIE, XDM_TSE_SURV, XDM_TSE_DG_STATUS
*
* O.Mercier	03 Oct 1994	: Ajout de msg pour BASEDD 		V 1.16
*				XDM_AT_TUBE
*				XDM_AT_echangeur
*  				XDM_AT_NAV 
* 				XDM_AT_PMV
*
* T.Milleville  07 Oct 1994     : Destruction du Msg 			V 1.17
*					XDM_TSP_VLOG
*
* I.Thomas	10 Oct 1994	: Ajout du message XDM_DIN_PA
*
* T.Milleville  11 Oct 1994     : Ajout du Msg XDM_TSP_ETAT  		V 1.19
* Niepceron     12 Oct 1994     : Modification des XDM_APPLIX_xxxx
*                                        par   des XDM_IAX_xxxx      	V 1.25
*
* O.Mercier	12 Oct 1994	: Ajout de msg pour GECONF		V 1.26
*				XDM_CGD_LIV,XDM_CGD_DIF,
*				XDM_CGD_MBD,XDM_CGD_VRF,
*				XDM_CGD_DBD,
*				XDM_CSV_SBD,XDM_CSV_ARC
*  				XDM_CSV_INF,XDM_CSV_RES.
*
* I.Thomas	12 Oct 1994	: Modification du message XDM_DIN_PA	V 1.27
*
* E.Nagiel 	13 Oct 1994	: Ajout de XDM_ETAT_Contraste 	V1.33
* B NOEL	13 Oct 1994	: Rien				V1.34
* O MERCIER     13 Oct 1994     : Modification de XDM_CGD_DIF   V1.35
* B NOEL	13 Oct 1994	: Rien				V1.34
* D MISMER	13 Oct 1994	: Ajout de XDM_EInit_IHM_Contraste	V1.36
* JM Guilhou	13 Oct 1994	: Modification du message XDM_IAX_SENDPOKE	V1.37
* Mismer D.	14 Oct 1994	: Ajout de XDM_EInit_IHM_Classe_DIV 	        V 1.27
********************************
* Niepceron     18 Oct 1994     : Ajout de XDM_IRAF_DAI1 XDM_IRAF_DAI2 
*                                 XDM_IRAF_DAI3 XDM_IRAF_DAI4 XDM_IRAF_DAI5 V1.41
********************************
* Mismer D.	18 Oct 1994	: Modif Grammaire de XDM_EInit_IHM_DAI 	        V 1.42
********************************
* Niepceron     19 Oct 1994     : Ajout de XDM_IETAT_DISPO_EQT  XDM_IRAF_BOUCHON_CHAUSSEE_PORTION 
*                                 XDM_IRAF_VOIE_NEUTRALISEE XDM_IRAF_VOIE_RADT_INVERSE
*                                 XDM_IRAF_BASCULEMENT_BOUCHON_VOIE XDM_IRAF_OBJET_MOBILE_PORTION
*                                 XDM_IRAF_OBJET_MOBILE_PORT_AFF_VOIES       V1.43
* Niepceron     19 Oct 1994     : v1.44 
*
* T.Milleville  20 Oct 1994     : Ajout des Msg  V 1.45
*			XDM_ADM_SSS, XDM_ADM_TAC, XDM_ADM_ARC_SER,
*			XDM_ADM_ARC_PER, XDM_ADM_ARC_INT,
*			XDM_ADM_TRA, XDM_ADM_ARC_STA_M,
*			XDM_ADM_ARC_STA_E, XDM_ADM_CON, XDM_ADM_RAF
********************************
* Niepceron     19 Oct 1994     : Modif de XDM_IRAF_OBJET_MOBILE_PORT_AFF_VOIES en
*                                 XDM_IRAF_PORT_AFF_VOIES  
*                                 Modif de XDM_IRAF_OBJET_MOBILE_PORTION  V1.45
********************************
* Niepceron     21 Oct 1994     : Ajout de XDM_IPOSIT_INDIC_ALARME V 1.46 
* NOEL B.	21 Oct 1994	: modif grammaire IMODIF_ALARMES
*					IMODIF_FMC			V1.48
* NOEL B.	24 Oct 1994	: Corrections syntaxiques		V1.49
*******************************
* MERCIER O.	24 Oct 1994 	: Ajout de XDM_AA_alerte		V1.50
*******************************
* MERCIER O.	24 Oct 1994	: Ajout de XDM_AM_alarme		V1.51
********************************
* Niepceron     26 Oct 1994     : Ajout de XDM_IRAF_POURCENT     	V 1.52
*******************************
* MERCIER O.	28 Oct 1994	: Ajout de XDM_A_INIT_IHM		V1.53
********************************
* Niepceron     31 Oct 1994     : Ajout de XDM_ISYN_SERVEUR     	V 1.54
********************************
* Niepceron      2 Nov 1994     : Correction sur  XDM_ISYN_SERVEUR     	V 1.55
********************************
* MERCIER O.	2 Nov	1994	: Ajout de XDM_A_TIMER_RAFRAI_IHM	V 1.56
********************************
* MERCIER O.	7 Nov	1994	: Ajout de XDM_AN_PURGE			V 1.57
********************************
* MERCIER O.	7 Nov	1994	: Ajout de XDM_AG_PURGE			V 1.58
*					   XDM_AG_PURGEHISTO
********************************
* MERCIER O.	15 Nov 	1994	: Ajout de XDM_AS_FdeT			V 1.59
********************************
* Niepceron     15 Nov 1994     : Ajout de XDM_IDONNEES_FT       	V 1.63
********************************
* Niepceron     15 Nov 1994     : Ajout de XDM_IRAF_COURBE       	V 1.64
*******************************************************
* NAGIEL        16 Nov 1994     : Modif de XDM_ECMD_NAV      		V 1.65 et V 1.66
*******************************************************
* NAGIEL        16 Nov 1994     : Ajout de XDM_ECMD_MAT_NAV      	V 1.67
*******************************************************
* Niepceron     23 Nov 1994     : Modif de XDM_ISYN_SERVEUR       	V 1.69
******************************************************
* NIEPCERON  	25 Nov 1994	: Ajout de  XDM_IDONNEES_INRETS_PR
*					    XDM_IDONNEES_INRETS_TRA  	V 1.70
******************************************************
* NIEPCERON  	25 Nov 1994	: Modif de  XDM_IDONNEES_INRETS_PR
*					    XDM_IDONNEES_INRETS_TRA  	V 1.71
******************************************************
* MISMER  	28 Nov 1994	: Ajout de XDM_ETAT_GTC_NICE		V 1.72
******************************************************
* Lecrivain	28 Nov 1994	: Ajout du message XDM_ICREER_ALERTE	V 1.73
******************************************************
* MISMER  	02 Dec 1994	: Modif XDM_ETAT_SEQUENCE		V 1.72
******************************************************
* NIEPCERON  	02 DEC 1994	: Modif de XDM_IRAF_PORT_AFF_VOIES  	V 1.77
* NAGIEL  	  	        : Modif de XDM_ETAT_RAU          	V 1.77*
* T.Milleville  5 Dec 1994     : Ajout du message  V 1.78 
*				XDM_ADM_REINIT
******************************************************
* NIEPCERON  	06 Dec 1994	: Remise des version 1.74 et 1.75         v1.79
******************************************************
* NIEPCERON  	07 Dec 1994	: Modif de  XDM_IDONNEES_INRETS_PR
*					    XDM_IDONNEES_INRETS_TRA  	V 1.80
******************************************************
* NAGIEL  	07 Dec 1994	: Modif de  XDM_ECPT_LCR et XDM_ELCR	V 1.81
******************************************************
* NIEPCERON  	07 Dec 1994	: Modif de  XDM_IDONNEES_INRETS_PR
*					    XDM_IDONNEES_INRETS_TRA  	V 1.80
* NAGIEL  	07 Dec 1994	: Modif de  XDM_ECMD_PMV et XDM_ECMDPICTO	V 1.82
*
* I.Thomas	14 Dec 1994	: Ajout du message XDM_DIN_BDC		V 1.83
********
* F. VOLCIC 	21 Dec 1994	: Modif XDM_EDem_Indiv : 	V 1.85
********
* MERCIER 	22 Dec 1994	: Modif XDM_CGD_DBD : contient 1 int4	V 1.86
********
* F. VOLCIC 	22 Dec 1994	: Modif XDM_EDem_Indiv en XDM_EDEM_Indiv	V 1.87
********
* MERCIER 	22 Dec 1994	: Modif XDM_CGD_CANAL : contient 1 int4	V 1.88
*   a jout de XDM_CGD_CHECK
********************************
* Niepceron     23 Dec 1994     : Ajout de XDM_IRAF_OBJET_MOBILE_ECHANG V1.89
********************************
* Niepceron     23 Dec 1994     : Suppresssion de lignes dupliquees... v1.90
********************************************************
* Noel     	28 Dec 1994     : modif grammaire  XDM_IMODIF_LISTE_TFM	V1.93
********************************************************
* F. VOLCIC     02 Jan 1995     : modif grammaire  XDM_ENiveau_Service	V1.94
********************************
* MERCIER	02 Jan 1995     : Ajout de XDM_CGD_CHGB			V 1.95
********************************
* MERCIER	02 Jan 1995	: Correction du commantaire entre version  V1.96
* 1.93 et 1.94 : probleme de compilation.
********************************
* NIEPCERON    03 JAN 1995	: lodif dans XDM_ISYN_SERVEUR             V1.97
********************************
* GABERT       05 JAN 1995	: Ajout XDM_ECMD_FIC                      V1.99
********************************
* GABERT       05 JAN 1995	: Modif XDM_ECMD_FIC                      V1.100
********************************
* MERCIER	06 Jan 1995	: Ajout de XDM_CSV_CANAL		V 1.101
********************************
* GABORIT       11 JAN 1995      : Ajout XDM_AH_INITLT et XDM_AH_RADT_DAI  V1.102
********************************
* NIEPCERON     12 JAN 1995      : Ajout  XDM_IDONNEES_RETARD V1.103
********************************
* LECRIVAIN     12 JAN 1995      : Ajout  XDM_IDONNEES_RETARD V1.104
********************************
* GABORIT       14 JAN 1995      : Ajout XDM_AS_INRETS et XDM_AS_BOUCHON  V1.105
*
* T.Milleville  16 Jan 1995     : Ajout du message XDM_TST_TESTDISK V 1.106 
********************************
* GABORIT       16 JAN 1995      : Ajout XDM_AH_SATIR  V1.107
*
* T.Milleville  16 Jan 1995     : Ajout du message XDM_TSE_INDQUA V 1.108 
*
* T.Milleville  19 Jan 1995     : Ajout du message XDM_TSE_SYNCDISK V1.110
*
********************************
* NIEPCERON     18 JAN 1995      : Modif de XDM_DONNEES_BOUCHON V 1.109
********************************
*
* T.Milleville  20 Jan 1995     : Modification argument du message 
*					XDM_TSE_SYNCDISK V1.111
*
* T.Milleville  25 Jan 1995     : Ajout de XDM_TST_DATASRV   V1.79
* GABORIT       26 JAN 1995      : Ajout XDM_AEQA  V1.114
* GABORIT       27 JAN 1995      : Modif XDM_AEQA  V1.115
* volcic        10 FEV 1995      : Ajout XDM_MODE_LENT  V1.116
* TORREGROSSA   17 FEV 1995      : Ajout XDM_AT_INI_EQT et XDM_AT_INI_IHM  V1.118
* TORREGROSSA   20 FEV 1995      : Modif XDM_AT_INI_EQT et XDM_AT_INI_IHM  V1.119
* MISMER	23 FEV 1995      : Ajout XDM_ETAT_MAT_NAV V1.119
* T.Milleville	06 Mar 1995      : Modification de la grammaire du 
*		msg XDM_TSE_MESG  V1.120
* F.Volcic	16 Mar 1995      : Ajout XDM_EInit_IHM_Liaison et XDM_ETAT_LIAISON v1.121
* MISMER	28 Mar 1995      : Modif XDM_ECMD_VIDEO v1.124
* T.Milleville	09 Avr 1995       : Ajout Msg XDM_TSP_STARTTMP V1.125
* T.Milleville	09 Avr 1995       : Ajout Msg XDM_TSE_STARTTMP V1.126
* T.Milleville	19 Avr 1995       : Suppression d'un des 2 msg XDM_TSP_ETAT V1.128
* T.Milleville	09 Avr 1995       : Ajout Msg XDM_TSP_STARTTMP V1.129
* Niepceron	21 Avr 1995       : Ajout d'un champs echelle ds XDM_ISYN_SERVEURCV V1.130
* Niepceron	22 Avr 1995       : Ajout d'un champs nom station ds XDM_ISYN_SERVEURCV V1.131
* T.Milleville	27 Avr 1995       : Modification grammaire Msg XDM_TSE_STARTTMP V1.132
* T.Milleville	03 Mai 1995       : Ajout msg XDM_TSP_MIGRAZUR V1.133
* F.Volcic	03 Mai 1995       : Ajout XDM_EInit_IHM_Liaison et XDM_ETAT_LIAISON v1.134
* Niepceron	05 Mai 1995       : XDM_IRAF_COURBE devient  XDM_IRAF_COURBE0 et XDM_IRAF_COURBE1 V1.135
* Niepceron	23 Mai 1995 	  : Ajout de XDM_IRAZ_MOB v1.136
* Niepceron	29 Mai 1995 	  : Modif de XDM_IRAZ_MOB v1.137
* NIEPCERON	06 Jun 1995   	  : Ajout XDM_IREI_EQT pour la reinitialisation des eqt  v1.138
* NIEPCERON	06 Jun 1995   	  : Suppression du deuxieme XDM_MODE_LENT ?!?  		 v1.139
* MISMER        19 Jun 1995     : Ajout XDM_EMOD_DEG pour la surveillance des LT         v1.140
* T.Milleville  4  JuL 1995     : Ajout d'un champ dans le msg XDM_TST_PROG V1.141
* T.Milleville  4  JuL 1995     : Ajout d'un champ dans les msg TST_TESTDISK et TST_DATASRV V1.142
* T.Milleville  12  JuL 1995     : Modification du dernier champ de XDM_TST_PROG 
*	de int4 en int2		v1.143
* T.Milleville  13  JuL 1995     : Ajout argument aux msg XDM_AH_SATIR et XDM_AH_RADT_DAI V1.145
* F.Volcic	28 Jul 1995      : Modif grammaire messages RADT v1.146
* F.Volcic	01 Aou 1995      : Ajout XDM_ACKNOLEDGE  v1.147
* D.Mismer	21 Sep 1995      : Ajout XDM_EInit_IHM_MAT_NAV  et  XDM_Acknoledge_Ala	v1.148
* T.Milleviller	27 Sep 1995      : Ajout d'un champ dans XDM_TSP_MIGRAZUR V1.149
* B.Gaborit	02 Oct 1995	 : Ajout d'un champ dans XDM_AM_alarme (1.150)
* F.Volcic	04 Aou 1995      : Ajout  d'un champ dans XDM_Acknoledge_Ala v1.151
* F.Volcic	04 Aou 1995      : Correction  d'un champ dans XDM_Acknoledge_Ala v1.152
* T.Milleville	10 Oct 1995      : Ajout d'un champ INT4 dans le msg XDM_TSE_PERI 
*	afin de preciser le type de l'entite V1.153
* T.Milleville	19 Oct 1995      : Ajout d'un champ INT4 dans le msg XDM_ADM_ARC_PER 
* GABORIT	27 Dec 1995	: Ajout du ss-système MAINTE v1.154
* Niepceron	23 Jan 1996	: ajout de XDM_TSP_LTD v1.155
* D.Mismer	13 Fev 1996      : Correction XDM_EMES_POIDS	v1.156
* Niepceron	14 Jun 1996      : Modification de XDM_ISYN_SERVEUR pour multi postit	v1.157
* Guilhou	22 aou 1996	: ajout des messages EXTENSION OUEST v1.158
* Niepceron     27 aou 1996	: ajout XDM_IAX_INITPRES v1.159
* Guilhou	28 aut 1996	: modif des messages XDM_At_NAV, PMv, ECH (num action sur INT4) DEM/1172 v1.160
* Mismer	29 aut 1996	: modif des messages XDM_ECMD_PMV, Picto (num action sur INT4) DEM/1172 v1.162
* Mismer	30 aut 1996	: modif des messages XDM_ECMD et ETAT PMV, Picto pour PMV 4 lignes DEM/1162 v1.163
* Niepceron     06 Sep 1996	: Ajout d'un flag dans le XDM_ISYN_SERVEURCV (radt) v1.164
* Mismer	16 Sep 1996   	: Ajout XDM_EInit_IHM_6mn_parVoie et XDM_EMES_6mn_parVoie (RADT_W) v1.165
* Niepceron     23 Sep 1996     : Ajout de la plage dans XDM_AS_INRETS (ana3) v1.166
* Niepceron     27 Sep 1996     : Ajout de XDM_IAX_ACQT (dem/424) v1.167
* Niepceron     01 Oct 1996	: Ajout de la periode dans XDM_ISYN_SERVEURCV (ihm9) v1.168
* Mismer	04 Oct 1996 	: Ajout XDM_EMES_PESAGE et XDM_Acknoledge_Pesage pour pesage RADT (DEM/1227) (1.169)
* Mismer	28 Oct 1996 	: Ajout XDM_INFO_PAU pour alerte PAU HS (DEM/1282) (1.170)
* Orengo.A	14 Nov 1996	: Ajout XDM_DONNEES_METEO_QTX et XDM_ACQ_DONNEES_METEO_QTX pour envoi
*					et l'acquitement des donnees meteo Tache teqtx (DEM/1232) 
*                                       et XDM_ECMD_GTC pour alerte DAI tunnel (DEM/12..)       (v 1.171)
* Torregrossa	04 Dec 1996 	: Modif XDM_IRAF_FMCLOC (680) (1.172)
* Niepceron     09 Jan 1997	: Ajout d'un num ordre (XDM_IRAF_PORT_AFF_VOIES) pour les rabt de présignal (dem/1372) v1.173
* Niepceron     03 Fev 1997	: Ajout XDM_AS_TDP XDM_ICOURBE2,XDM_IDONNEES_TDP,XDM_ETDP_SEUIL,XDM_ETDP_IHM,XDM_CR_TDP et XDM_VAL_TDP TDP v1.174
* Vathelot	29 Avr 1997	: Ajout XDM_AS_TIMER_SRV_RADT et XDM_AS_TIMER_PURGE_RADT (dem Strada) v1.174
* Penot		05 Mai 1997	: Ajout xDM_AS_TIMER_CLT_RADT (dem Strada) v1.175
*				  Ajout de XDM_AS_TIMER_SRVEVT_STRADA
* Penot		13 Mai 1997	: Ajout XDM_AS_TIMER_CLT_EVT (dem Strada) v1.175	
* Guilhou	03 juin 1997	: suppression num ordre (XDM_IRAF_PORT_AFF_VOIES) TEMPORAIRE car pas livre en juin 97 1.176
* Guilhou	27 oct 1997 	: ajout XDM_AS_SRVEVT_STRADA 1.178
* Guilhou	04 nov 1997 	: ajout XDM_AS_EVT_STRADA 1.179
* Penot		07 Nov 1997	: ajout XDM_IAEVT_STRADA
* Guilhou	14 nov 1997	: remise num ordre (XDM_IRAF_PORT_AFF_VOIES) 1.181
* Mismer	19 Jan 1998 	: Ajout XDM_ECMD_PAL XDM_EInit_IHM_PAL XDM_ETAT_PAL pour PAL (DEM/1539) (1.182)
* Niepceron     05 Mai 1998	: Ajout XDM_A_TIMER_FMC_LOC v1.182
* Niepceron     19 Nov 1998     : Ajout XDM_AG_IMPCAT, XDM_AG_EXPCAT, XDM_IPMV_DATEX v1.183 dem/1723
* Niepceron     10 Mar 199	: Ajout datex ds XDM_IRAF_FMCLOC 1.184
* Guilhou	16 jan 2001	: ajout XDM_AW_TIMER 1.185
* JMG		7 dec 2001 	: ajout XDM_ISYN_NOMADE_VUE, XDM_ISYN_NOMADE_MASK 1.186
* JMG		7 dec 2001 	: ajout XDM_NOMADE_TIMER, XDM_NOMADE_REVEIL 1.187
* ETR		26 jui 2002	: ajout XDM_AMISOL_TIMER
* JMG		27/04/04	: modif XDM_ETAT_PAL et XDM_ECMD_PAL 
* JMG		16/11/04	: ajout XDM_EPAL_AUTO et XDM_EPAL_AUTO_FIN
* PNI		11/03/05	: ajout de XDM_IRAF_BOUCHON_CHAUSSEE_VOIE DEM444 v1.190
* JMG		13/09/05	: ajout PMVA BAF BAD 1.191
* CHI		21/09/05	: modif XDM_IDONNEES_BOUCHONS DEM509 v1.192
* JMG		28/09/05	: ajout PMVA BAF BAD 1.193
* JMG		14/11/0(	: ajout XDM_ENAV_AUTO et XDM_ENAV_AUTO_FIN 1.194
* PNI		25/10/05	: ajout XDM_EBAF_AUTO,... 1.195 DEM584
* JMG           31/10/07         : modif XDM_A_PRES_OP (ajout str) 1.196
*JMG   13/11/07        : ajout site de gestion DEM715
*AAZ   01/12/07		   : gtc renovee DEM 662
* JBL	Mai 2008	: gtc renovee DEM 662 (echangeurs)
* JBL        17/10/2008	: modification de XDM_AM_alarme int2 au lieu de char pour le type d'alarme
* JMG   02/10/08        : ajout XDM_AS_SRVEVT_STRADA_TDP
				XDM_ATDP_TIMER XDM_AS_SRVEVT_STRADA_RDT
				XDM_AS_SRVEVT_STRADA_FMC DEM/835 836 837
* JMG           28/09/09         : SECTO : DEM 887
* JPL		03/11/09	: Ajout parametre chaine a XDM_IMODIF_LISTE_TFM (DEM 917) 1.202
* JMG		25/11/10 : ajout messages flash + radio/tel 1.203
* JMG		18/03/11 : ajout XDM_AA_alerte_localisee  et XDM_ICREER_ALERTE_LOCALISEE  1.204
* JPL		23//12/11 : Ajout message XDM_ITDP_TIMER pour tempo. diffusion messages TdP radio (DEM 1007)  1.205
* VR            04/11/11 : modification XDM_EPMV_AUTO XDM_ECMD_PMV XDM_ETDP_PMV XDM_ETDP_IHM XDM_VAL_TDP 1.206 (DEM/1014)
* SDU/JMG	22/02/12 : Communautaire (DEM/1015)  1.207
* JPL		21/03/12 : Correction parametres message XDM_AA_alerte_localisee (DEM 1013)  1.208
* JMG		25/05/12 : ajout PRV DEM/1016
* VR		25/05/12 : Ajout DTP PMVA (DEM/1014 PMA)
* JPL		29/01/13 : Ajout message XDM_ANEUTR_TIMER pour mise au neutre PMV[A] (DEM 1061)  1.211
* PNI		27/08/13 : Correction XDM_EPMVA_AUTO dem1074 1.212
* PNI		06/01/15 : Ajout XDM_TSE_KEEPALIVE DEM1116 1.213
* JMG		16/04/16 : ajout msg SAGA et XDM_ECMD_BASC_RAU 1.214
* JMG		11/12/17 : ajout polyvox 1.216
* JMG		15/01/18 : ajout XDM_ETAT_CAB GABEB 1.217
* JMG		15/01/19 : ajout XDG DETECTION GABEB 1.218
* JPL		15/01/18 : Syntaxe  1.219
* LCL		26/03/18 : Ajout CFE CTRL_FEUX 1.220 DEM1284
* LCL		11/05/18 : Ajout GABEB 1.221 DEM1283
* JMG		21/06/18 : correction XDM_ETAT_CAP 1.222
* JMG		29/10/18 : ajout msg RMVI DEM1315 1.223
* JMG		30/11/18 : ajout msg AVA 1.224
* JMG		12/06/19 : ajout XDM_IAX_INFO_MESSAGE DEM1338 ODI-C 1.225
* LCL		11/09/19 : ajout XDM_COCKPIT DEM1251 1.226
* JMG		12/11/19 : XDM_ETDP_SURCHARGE DEM1365 1.227
* JMG		10/01/20 : ajout IMU 1.228
* ABE		26/06/20 : Modification message cockpit, ajout variable pour DEM1351 1.229
* ABE		30/03/20 : Ajout TIMER Datex2 DEM130 1.230
* ABE		30/03/20 : Ajout TIMER RADT DEM130 1.231
* GR            30/07/20 : Ajout EMSG_TDP_FCD DEM-SAE152 1.231
* ABE		15/09/20 : Ajout Datex2 timer situation coup par coup DEM-SAE130 1.232
* ABE		24/09/20 : Modification message EMSG_TDP_FCD DEM-SAE152 1.233
* JMG		03/03/21 : messages pour API 1.234
* ABE		02/04/21 : Ajout RAU SONO DEM-SAE244 1.240
* LCL		24/10/21 : Ajout INIT_TISSE 1.241
* ABE   06/12/21 : Ajout Message demarrage Magistra XDM_MGA_INIT_PO 1.242
* ABE   18/01/22 : Ajout msg AVA DEM-SAE367
* GGY   22/03/23 : Ajout message XDM_MGA_CONFIG_PO
* GGY	15/01/24 : Ajout appel aarp02 pour envoi msg XDM_AS_UPDATE_PA (DEM568)
******************************************************* 

*******************************************************
* TESTS UNITAIRES  : RELECTURE CROISEE
*
*
*******************************************************/

#define xdm_c 

/* fichiers inclus */

#include "xdm.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "@(#)xdm.c	1.206 02/14/12 : xdm" ;

T_TYPEMSG tg_Mtb[] =
{
  { "XDM_TST_PROG", XDM_TST_PROG, "real8 int4 str int4 str int2" },
  { "XDM_TST_ANNU", XDM_TST_ANNU, "str str int4" },
  { "XDM_TST_CMDT", XDM_TST_CMDT, "str int4 int4" },
  { "XDM_TST_TRAC", XDM_TST_TRAC, "str str int4 int4" },
  { "XDM_TST_INIT", XDM_TST_INIT, "str" },
  { "XDM_TST_TINIT", XDM_TST_TINIT, "" },
  { "XDM_TST_TSUP", XDM_TST_TSUP, "str str int4" },
  { "XDM_TST_TESTDISK", XDM_TST_TESTDISK, "str" },
  { "XDM_TST_DATASRV", XDM_TST_DATASRV, "str" },

  { "XDM_TSH_HORO", XDM_TSH_HORO, "real8" },
  { "XDM_TSH_SYNC", XDM_TSH_SYNC, "str str" },
  { "XDM_TSH_MODE", XDM_TSH_MODE, "int4" },
  { "XDM_TSH_SYST", XDM_TSH_SYST, "str" },

  { "XDM_TSP_INIT", XDM_TSP_INIT, "" },
  { "XDM_TSP_APPL", XDM_TSP_APPL, "int4" },
  { "XDM_TSP_SSYS", XDM_TSP_SSYS, "str int4" },
  { "XDM_TSP_TACH", XDM_TSP_TACH, "str int4" },
  { "XDM_TSP_STOP", XDM_TSP_STOP, "int4" },
  { "XDM_TSP_EXIT", XDM_TSP_EXIT, "" },
  { "XDM_TSP_ETAT", XDM_TSP_ETAT, "" },
  { "XDM_TSP_STARTTMP", XDM_TSP_STARTTMP, "str" },
  { "XDM_TSP_MIGRAZUR", XDM_TSP_MIGRAZUR, "str" },

  { "XDM_TSP_LTD" , XDM_TSP_LTD, "" },
  
  { "XDM_TSA_ADMI", XDM_TSA_ADMI, ""},  
  
  { "XDM_TSG_PING", XDM_TSG_PING,"str" },
  { "XDM_TSG_STOP_PING", XDM_TSG_STOP_PING,"str" },
 
 
 { "XDM_TSG_INIT", XDM_TSG_INIT,"" },
  { "XDM_TSG_VERIF", XDM_TSG_VERIF,"str" },
  
  { "XDM_TSE_DIFE", XDM_TSE_DIFE, "str" },
  { "XDM_TSE_INIT", XDM_TSE_INIT, "" },
  { "XDM_TSE_MODI", XDM_TSE_MODI, "int4" },
  { "XDM_TSE_MODR", XDM_TSE_MODR, "int4" },
  { "XDM_TSE_BASC", XDM_TSE_BASC, "str" },
  { "XDM_TSE_SSYS", XDM_TSE_SSYS, "str str int4" },
  { "XDM_TSE_TACH", XDM_TSE_TACH, "str str int4" },
  { "XDM_TSE_MACH", XDM_TSE_MACH, "str int4"},
  { "XDM_TSE_PERI", XDM_TSE_PERI, "str int4 int4"},
  { "XDM_TSE_INTE", XDM_TSE_INTE, "str int4"},
  { "XDM_TSE_MODE", XDM_TSE_MODE, "str int4 int4" },
  { "XDM_TSE_ISAG", XDM_TSE_ISAG, "" },
  { "XDM_TSE_WATCH", XDM_TSE_WATCH, "" },
  { "XDM_TSE_ISSS", XDM_TSE_ISSS, "str" },
  { "XDM_TSE_ISTC", XDM_TSE_ISTC, "str" },
  { "XDM_TSE_MAJM", XDM_TSE_MAJM, "int4 int4" },
  { "XDM_TSE_MAJE", XDM_TSE_MAJE, "int4" },
  { "XDM_TSE_MAJC", XDM_TSE_MAJC, "str" },
  { "XDM_TSE_MAJD", XDM_TSE_MAJD, "str" },
  { "XDM_TSE_QUES", XDM_TSE_QUES, "str str" },
  { "XDM_TSE_MESG", XDM_TSE_MESG, "int4 str str str str str str str str str str"},
  { "XDM_TSE_TTMP", XDM_TSE_TTMP, "str str int4" },
  { "XDM_TSE_QACT", XDM_TSE_QACT, "str str" },
  { "XDM_TSE_RACT", XDM_TSE_RACT, "str int4" },
  { "XDM_TSE_APPLI", XDM_TSE_APPLI, "str int4" },
  { "XDM_TSE_RES", XDM_TSE_RES, "str int4" },
  { "XDM_TSE_TOUT", XDM_TSE_TOUT, "str" },
  { "XDM_TSE_VIE", XDM_TSE_VIE, "str" },
  { "XDM_TSE_SURV", XDM_TSE_SURV, "str" },
  { "XDM_TSE_DG_STATUS", XDM_TSE_DG_STATUS, "str int4" },
  { "XDM_TSE_INDQUA", XDM_TSE_INDQUA, "str str str int4" },
  { "XDM_TSE_SYNCDISK", XDM_TSE_SYNCDISK, "" },
  { "XDM_TSE_STARTTMP", XDM_TSE_STARTTMP, "str str" },


  { "XDM_ITEST_RAU", XDM_ITEST_RAU,"" },
  { "XDM_IMESSAGE_TFM_PRET", XDM_IMESSAGE_TFM_PRET,"int4 int4 str" },
  { "XDM_IPA_PRET", XDM_IPA_PRET,"int4 int4 str" },
  { "XDM_IMODIF_LISTE_TFM", XDM_IMODIF_LISTE_TFM,"int4 int4 str" },
  { "XDM_IMODIF_ALARMES", XDM_IMODIF_ALARMES,"int4 int4" },
  { "XDM_IMODIF_FMC", XDM_IMODIF_FMC,"int4 int4 str" },
  { "XDM_ADM_SSS", XDM_ADM_SSS," str str int4" },
  { "XDM_ADM_TAC", XDM_ADM_TAC," str str int4" },
  { "XDM_ADM_ARC_SER", XDM_ADM_ARC_SER," str int4" },
  { "XDM_ADM_ARC_PER", XDM_ADM_ARC_PER," str int4 int4" },
  { "XDM_ADM_ARC_INT", XDM_ADM_ARC_INT," str int4" },
  { "XDM_ADM_ARC_STA_M", XDM_ADM_ARC_STA_M," str int4" },
  { "XDM_ADM_ARC_STA_E", XDM_ADM_ARC_STA_E," str int4" },
  { "XDM_ADM_TRA", XDM_ADM_TRA," int4 int4 str str" },
  { "XDM_ADM_CON", XDM_ADM_CON," str int4" },
  { "XDM_ADM_RAF", XDM_ADM_RAF,"" },
  { "XDM_ICREER_ALERTE", XDM_ICREER_ALERTE, "int4 int4 str" },
  { "XDM_ICREER_ALERTE_LOCALISEE", XDM_ICREER_ALERTE_LOCALISEE, "int4 int4 str" },
  { "XDM_ADM_REINIT", XDM_ADM_REINIT,"" },
  { "XDM_IARCHIVAGE", XDM_IARCHIVAGE, "int4 str str int4" },
  { "XDM_IMASQUE", XDM_IMASQUE, "int2" } ,
  { "XDM_IMASQUE_PROJETER", XDM_IMASQUE_PROJETER, "int2 int2" } , /*RETRO*/
  { "XDM_IVUE", XDM_IVUE, "str" } ,
  { "XDM_IGRAPHE", XDM_IGRAPHE, "int2 str" } ,
  { "XDM_IPLANSECOURS", XDM_IPLANSECOURS, "int4" } ,
  { "XDM_EREG_FMC", XDM_EREG_FMC, "int4 int4 int4 int4 int4" } , 
  { "XDM_GABEB_FMC", XDM_GABEB_FMC, "int4 int4 int4 int4 int4" } , 

  { "XDM_ITDP_TIMER", XDM_ITDP_TIMER, "str" },
  { "XDM_COMMU_ACQ", XDM_COMMU_ACQ, "int4"} ,
  { "XDM_COMMU_RAPPROCHE", XDM_COMMU_RAPPROCHE, "int4 int4 int2 str"} ,
  { "XDM_COMMU_IGNORE", XDM_COMMU_IGNORE, "int4 int2"} ,

/* Messages Applix/tache relais*/
  { "XDM_IAX_JOINDG", XDM_IAX_JOINDG, "int4 str" },
  { "XDM_IAX_SENDPOKE", XDM_IAX_SENDPOKE, "int4 int4 str" },
  { "XDM_IAX_NEWTASK", XDM_IAX_NEWTASK, "int4 str str int4" },
  { "XDM_IAX_SETVARCTX", XDM_IAX_SETVARCTX, "str str str" },
  { "XDM_API_SETVARCTX", XDM_API_SETVARCTX, "str str" },
  { "XDM_IAX_FINPOSTE", XDM_IAX_FINPOSTE, "" },
  { "XDM_IAX_INITPRES", XDM_IAX_INITPRES, "" },
  { "XDM_IAX_ACQT", XDM_IAX_ACQT, "int4 int4 int2" },
  { "XDM_IAEVT_STRADA",XDM_IAEVT_STRADA,""},
  { "XDM_IAX_INFOMESSAGE", XDM_IAX_INFOMESSAGE, "int4 str" },
  { "XDM_API_SYNOS", XDM_API_SYNOS, "str str" },

/* Messages programmation timer pour raf dai */
  { "XDM_IRAF_DAI1", XDM_IRAF_DAI1, "str" },
  { "XDM_IRAF_DAI2", XDM_IRAF_DAI2, "str" },
  { "XDM_IRAF_DAI3", XDM_IRAF_DAI3, "str" },
  { "XDM_IRAF_DAI4", XDM_IRAF_DAI4, "str" },
  { "XDM_IRAF_DAI5", XDM_IRAF_DAI5, "str" },
  { "XDM_IRAF_DEBUT", XDM_IRAF_DEBUT, "" },
  { "XDM_IRAF_FIN", XDM_IRAF_FIN, "" },
  { "XDM_IRAF_FMCLOC", XDM_IRAF_FMCLOC, "int4_array int2_array int2_array real8_array str_array real8_array real8_array int2_array int2_array" },
 
/* Messages  pour raf des objets du synoptique */
  { "XDM_IETAT_DISPO_EQT",  XDM_IETAT_DISPO_EQT, "int2 int2 int4" },
  { "XDM_IRAF_BOUCHON_CHAUSSEE_PORTION", XDM_IRAF_BOUCHON_CHAUSSEE_PORTION, "int4 int2 int2 int2 int2 int4 int4 int2" },
  { "XDM_IRAF_BOUCHON_CHAUSSEE_VOIE", XDM_IRAF_BOUCHON_CHAUSSEE_VOIE, "int4 int2 int2 int2 int4 int2 int2 int4 int4 int2" },
  { "XDM_IRAF_VOIE_NEUTRALISEE", XDM_IRAF_VOIE_NEUTRALISEE,"int4 int2 int2 int4 int2 int2 int2 int4 int4" },
  { "XDM_IRAF_VOIE_RADT_INVERSE", XDM_IRAF_VOIE_RADT_INVERSE, "int4 int2 int2 int4 int2 int2 int2 int4 int4 int2 int2 " },
  { "XDM_IRAF_BASCULEMENT_BOUCHON_VOIE", XDM_IRAF_BASCULEMENT_BOUCHON_VOIE, "int4 int2 int4 int2 int2 int4 int2 int2 int2 int4 int4 int2" },
  { "XDM_IRAF_OBJET_MOBILE_PORTION", XDM_IRAF_OBJET_MOBILE_PORTION, "int4 int2 int4 int2 int2 int2 int2 int2  int2 int4" },


  { "XDM_IRAF_PORT_AFF_VOIES", XDM_IRAF_PORT_AFF_VOIES , "int4 int2 int2 int2 int2 int4 int2_array int2" },
  { "XDM_IRAF_PORT_AFF_VOIES", XDM_IRAF_PORT_AFF_VOIES , "int4 int2 int2 int2 int2 int4 int2_array" },
  
  { "XDM_IPOSIT_INDIC_ALARME", XDM_IPOSIT_INDIC_ALARME , "int2" },
  
  { "XDM_IRAF_POURCENT", XDM_IRAF_POURCENT , "str" },
  
  { "XDM_ISYN_SERVEUR", XDM_ISYN_SERVEUR , "str str_array str_array str_array str int2 str int2 str int2 int2_array int2_array str_array int2 str" },

  { "XDM_IDONNEES_FT", XDM_IDONNEES_FT , "int2 int4 int2 int2 real8 int2 int2_array int2_array" },
  
  { "XDM_IRAF_COURBE0", XDM_IRAF_COURBE0 , "str" },

  { "XDM_IDONNEES_INRETS_PR" , XDM_IDONNEES_INRETS_PR , "int2 int4 int4 real8 int4_array int4_array int4_array" },
  { "XDM_IDONNEES_INRETS_TRA" , XDM_IDONNEES_INRETS_TRA ,  "int2 int4 int4 real8 int4_array real8_array int2_array" },
  { "XDM_IDONNEES_BOUCHONS" , XDM_IDONNEES_BOUCHONS ,  "int2 int4 int4 real8 int4_array real8_array real8_array int4_array int4_array int2_array" },
  { "XDM_IDONNEES_TDP" , XDM_IDONNEES_TDP ,  "str int2 int4 int4 int2 int2 int2 int2_array" },
  { "XDM_IDONNEES_RETARD" , XDM_IDONNEES_RETARD ,  "real8 str_array int4_array int2_array int2_array int2_array int4_array int4_array int4_array" },
  
  { "XDM_IRAF_OBJET_MOBILE_ECHANG", XDM_IRAF_OBJET_MOBILE_ECHANG, "int4 int2 int4 int2 str int2 int2 int2 int2" },
  
  { "XDM_ISYN_SERVEURCV", XDM_ISYN_SERVEURCV , "int2 int2 real8 int2 int2 int2_array str_array int2_array int2_array int2_array int2_array str real8 real8 str real8 real8 int2" },

  { "XDM_IRAF_COURBE1", XDM_IRAF_COURBE1 , "str" },

  { "XDM_IRAF_COURBE2", XDM_IRAF_COURBE2 , "str" },

  { "XDM_ISYN_NOMADE_VUE", XDM_ISYN_NOMADE_VUE, "str" },
  { "XDM_ISYN_NOMADE_MASK", XDM_ISYN_NOMADE_MASK, "int4" }, 
  
  { "XDM_IRAZ_MOB", XDM_IRAZ_MOB , "" },
 
  { "XDM_IREI_EQT", XDM_IREI_EQT , "" },

  { "XDM_IDONNEES_TRAFIC_STRADA", XDM_IDONNEES_TRAFIC_STRADA, "int4 int2 int2 real8 int4 int2 int4 int2 int4 int2" } ,
  { "XDM_IPMV_DATEX", XDM_IPMV_DATEX, "str str str str str" },
 

/* Messages Sous-Systeme SAIDEC */
 { "XDM_DIN_PA", XDM_DIN_PA, "real8 int2 str int4 char char char" },
 { "XDM_DIN_BDC", XDM_DIN_BDC, "" },

/* Messages Sous-Systeme EQUEXT */

  {"XDM_EINIT", 		XDM_EINIT, 		"str int4 int4" },              
  {"XDM_EINIT_ABT", 		XDM_EINIT_ABT, 		"str int4 int4" },	
  {"XDM_EFIN_CONFIG", 		XDM_EFIN_CONFIG,	"int4 int4" },
  {"XDM_EFIN_CONFIG_SUR", 	XDM_EFIN_CONFIG_SUR, 	"int4 int4 str" },
  {"XDM_ECONFIG_EQPT", 		XDM_ECONFIG_EQPT, 	"str int4" },
  {"XDM_ECMD_EQPT", 		XDM_ECMD_EQPT, 		"int4 str int4 int4" },
  {"XDM_ECMD_RAU", 		XDM_ECMD_RAU,		"int4 int4 int4" },
  {"XDM_ETAT_RAU", 		XDM_ETAT_RAU, 		"int4 int4 int4 int4 int4 int4" },	
  {"XDM_INFO_PAU", 		XDM_INFO_PAU, 		"int2_array int2_array" },	
  {"XDM_ECMD_VIDEO", 		XDM_ECMD_VIDEO, 	"int4 int4 int4 int4 int4 int4 int4 int4 str" },	
  {"XDM_ETAT_VIDEO", 		XDM_ETAT_VIDEO,	 	"int4 int4 int4 int4 int4 int4 str" },	
  {"XDM_ETAT_MAGNETO", 		XDM_ETAT_MAGNETO, 	"int4 int4 int4 str" },	
  {"XDM_ECMD_AUTOTEL", 		XDM_ECMD_AUTOTEL, 	"int4 str int2" },	
  {"XDM_ETAT_AUTOTEL", 		XDM_ETAT_AUTOTEL, 	"int4 int4 str str int4 int4" },	
  {"XDM_ECMD_OPER", 		XDM_ECMD_OPER, 		"int4 str str" },
  {"XDM_ETAT_OPER", 		XDM_ETAT_OPER, 	 	"int4 int4 str str int4 int4" },
  {"XDM_EMES_RADT_6mn", 	XDM_EMES_RADT_6mn,	"real8 int2 int2 int2 int2 int2 int2 int2 int2 int2 int2 int2 int2" },	
  {"XDM_EMES_6mn_Voie", 	XDM_EMES_6mn_Voie,	"real8 int2 int2 int2 int2 int2 int2 int2 int2 int2 int2 int2 int2" },	
  {"XDM_EMES_6mn_parVoie",	XDM_EMES_6mn_parVoie,   "real8 int2 int2 int2 int2 int2 int2 int2 int2 int2 int2 int2 int2" },	
  {"XDM_EMES_6mn_Chaussee",	XDM_EMES_6mn_Chaussee,  "real8 int2 int2 int2 int2 int2 int2 int2 int2 int2 int2 int2 int2" },	
  {"XDM_EMES_RADT_6gl", 	XDM_EMES_RADT_6gl,	"real8 int2 int2 int2 int2 int2 int2 int2 int2 int2 int2 int2 int2" },
  {"XDM_EMES_6gl_Voie", 	XDM_EMES_6gl_Voie,	"real8 int2 int2 int2 int2 int2 int2 int2 int2 int2 int2 int2 int2" },
  {"XDM_EMES_6gl_Chaussee",	XDM_EMES_6gl_Chaussee,	"real8 str int2 int2 int2 int2 int2 int2 int2 int2 int2 int2 int2 int2" },
  {"XDM_EMES_PESAGE", 		XDM_EMES_PESAGE,	"real8 int2 int2 int2 int2 int4_array"},
  {"XDM_Acknoledge",		XDM_Acknoledge,		"int2 real8" },
  {"XDM_Acknoledge_Pesage",	XDM_Acknoledge_Pesage,	"int2 real8 int2" },
  {"XDM_EMES_Classe_DIV", 	XDM_EMES_Classe_DIV,	"real8 int2 int2 int2 int2 int2 int2 int2 int2" },
  {"XDM_ENiveau_Service", 	XDM_ENiveau_Service,	"real8 int2 int2 int2" },
  {"XDM_EConf_Voie_DAI", 	XDM_EConf_Voie_DAI,	"int2 int2 int2 int2_array"},
  {"XDM_EMES_DAI", 		XDM_EMES_DAI,		"real8 int2 int2 int2 int2 int2 int2_array int2_array int2 int2 "},
  {"XDM_ETAT_PMV", 		XDM_ETAT_PMV,		"int2 str int2 str char str int2 str char str int2 str char str int2 str char char" },		
  {"XDM_ETAT_PICTO",		XDM_ETAT_PICTO,		"int2 str str int2" },
  {"XDM_ETAT_NAV", 		XDM_ETAT_NAV,		"int2 int2 str_array" },
  {"XDM_ETAT_PAL", 		XDM_ETAT_PAL,		"int2 int2 str_array" },
  {"XDM_ETAT_GTC", 		XDM_ETAT_GTC,		"int2 int2 int2" },
  {"XDM_ECMD_PMV", 		XDM_ECMD_PMV,		"int2 str int2 str char str int2 str char str int2 str char str int2 str char char int4" },
  {"XDM_ECMD_PICTO", 		XDM_ECMD_PICTO,		"int2 str str char int4"},
  {"XDM_ECMD_NAV", 		XDM_ECMD_NAV,		"int2 int2 str int2 int4"},		
  {"XDM_ECMD_PAL", 		XDM_ECMD_PAL,		"int2 str int4"},		
  {"XDM_ECMD_SCEN", 		XDM_ECMD_SCEN,		"int2 str int4"},		
  {"XDM_ECMD_SCEN_BAD",		XDM_ECMD_SCEN_BAD,	"int2 str int4"},		
  {"XDM_ECMD_SCEN_FICH", 	XDM_ECMD_SCEN_FICH,	"int2 str int4"},	
  {"XDM_ECMD_DAI", 		XDM_ECMD_DAI,		"real8 int2 int2 int2 int2 int2 int2_array int2_array int2 int2"},	
  {"XDM_EMES_POIDS",		XDM_EMES_POIDS,		"real8 int2 int2 int2 int2" },		
  {"XDM_EConf_Voie", 		XDM_EConf_Voie,		"int2_array" },		
  {"XDM_EConf_Voie_RADT", 	XDM_EConf_Voie_RADT,	"int2 int2 int2_array" },		
  {"XDM_EDEM_Indiv", 		XDM_EDEM_Indiv,		"int2 int2 int2" },		
  {"XDM_ELCR",			XDM_ELCR,		"int2 int2 str str int2 real8 real8 str" }, 		
  {"XDM_ECPT_LCR", 		XDM_ECPT_LCR,		"int2" },		
  {"XDM_EInit_IHM_6gl_Voie", 	XDM_EInit_IHM_6gl_Voie,	"str int2 int2 int2" },
  {"XDM_EInit_IHM_6mn_parVoie", XDM_EInit_IHM_6mn_parVoie,	"str int2 int2 int2" },
  {"XDM_EInit_IHM_6gl_Chaussee",XDM_EInit_IHM_6gl_Chaussee,"str int2" },	
  {"XDM_ECMD_SIGE", 		XDM_ECMD_SIGE,		"int4 int2_array int2_array int4 int4" },
  {"XDM_ECMD_SEQT", 		XDM_ECMD_SEQT,		"int4 int4 int4" },	
  {"XDM_ECMD_SEQE", 		XDM_ECMD_SEQE,		"int4 int4 int4" },
  {"XDM_ECMD_SIGT", 		XDM_ECMD_SIGT,		"int4 int2_array int2_array int4 int4" },	
  {"XDM_REGUL_START_SIMU", 		XDM_REGUL_START_SIMU,		"real8 real8 str" },	
  {"XDM_REGUL_SIMU", 		XDM_REGUL_SIMU,		"str str int4" },
  {"XDM_REGUL_APPLICATION", XDM_REGUL_APPLICATION,		"" },
  {"XDM_REGUL_DETECTION", 	XDM_REGUL_DETECTION, 		""},
  {"XDM_ACQUITTE_REGUL", 	XDM_ACQUITTE_REGUL,	"int4 int2 str int4"},
  {"XDM_CHECK_FAX", 	XDM_CHECK_FAX, 		""},
  {"XDM_GABEB_DETECTION",  XDM_GABEB_DETECTION,            ""},
  {"XDM_GABEB_APPLICATION", XDM_GABEB_APPLICATION,              "" },
  {"XDM_ACQUITTE_GABEB",        XDM_ACQUITTE_GABEB,     "int4 int2 str int4"},

  /* AAZ 11/10/2007 */
  {"XDM_ECMD_SIG", 		XDM_ECMD_SIG,		"int4 int2_array int2_array int2_array int4 int4" },
  {"XDM_ECMD_ECL", 		XDM_ECMD_ECL,		"int4 int2_array int2_array int2_array int4 int4" },
  {"XDM_ECMD_VEN", 		XDM_ECMD_VEN,		"int4 int2_array int2_array int2_array int4 int4" },
  {"XDM_ECMD_ENR", 		XDM_ECMD_ENR,		"int4 int2_array int2_array int2_array int4 int4" },
  {"XDM_ECMD_INC", 		XDM_ECMD_INC,		"int4 int2_array int2_array int2_array int4 int4" },
  {"XDM_ECMD_EXP", 		XDM_ECMD_EXP,		"int4 int2_array int2_array int2_array int4 int4" },
  {"XDM_ECMD_SYS", 		XDM_ECMD_SYS,		"int4 int2_array int2_array int2_array int4 int4" },
  {"XDM_ECMD_LTN", 		XDM_ECMD_LTN,		"int4 int2_array int2_array int2_array int4 int4" },
  {"XDM_ECMD_DOMAINE", 		XDM_ECMD_DOMAINE,		"int4 int4 int4" },
  {"XDM_ECMD_SEQSIG", 		XDM_ECMD_SEQSIG,		"int4 int4 int4 int4" },	
  {"XDM_ECMD_SEQECL", 		XDM_ECMD_SEQECL,		"int4 int4 int4 int4" },	
  {"XDM_ECMD_SEQVEN", 		XDM_ECMD_SEQVEN,		"int4 int4 int4 int4" },	
  {"XDM_ECMD_SEQENR", 		XDM_ECMD_SEQENR,		"int4 int4 int4 int4" },	
  {"XDM_ECMD_SEQINC", 		XDM_ECMD_SEQINC,		"int4 int4 int4 int4" },	
  {"XDM_ECMD_SEQEXP", 		XDM_ECMD_SEQEXP,		"int4 int4 int4 int4" },	
  /* AAZ */

  {"XDM_EInit_IHM_IMU", 	XDM_EInit_IHM_IMU,	"str" },
  {"XDM_EInit_IHM_SONO", 	XDM_EInit_IHM_SONO,	"str" },
  {"XDM_EInit_IHM_PMV", 	XDM_EInit_IHM_PMV,	"str" },
  {"XDM_EInit_IHM_Picto", 	XDM_EInit_IHM_Picto,	"str" },	
  {"XDM_EInit_IHM_NAV", 	XDM_EInit_IHM_NAV,	"str" },	
  {"XDM_EInit_IHM_PAL", 	XDM_EInit_IHM_PAL,	"str" },	
  {"XDM_EInit_IHM_DAI", 	XDM_EInit_IHM_DAI,	"str int2" },	
  {"XDM_EInit_IHM_GTC", 	XDM_EInit_IHM_GTC,	"str" },
  {"XDM_EInit_IHM_NICE", 	XDM_EInit_IHM_NICE,	"str int4 int4 int4" },	
  {"XDM_EInit_IHM_NICERenovee", 	XDM_EInit_IHM_NICERenovee,	"str int4 int4 int4 int4" },	
  {"XDM_EInit_IHM_Liaison", 	XDM_EInit_IHM_Liaison,	"str" },	
  {"XDM_ETAT_JourNuit", 	XDM_ETAT_JourNuit,	"int2" },	
  {"XDM_ECMD_METEO", 		XDM_ECMD_METEO,		"int2" },	
  {"XDM_ECMD_Contraste", 	XDM_ECMD_Contraste,	"int2" },
  {"XDM_ETAT_TELEALARME", 	XDM_ETAT_TELEALARME,	"int4 int4 int4 int4 int4" },	
  /* AAZ 11/10/2007 */
  {"XDM_ETAT_SIGSIG", 		XDM_ETAT_SIGSIG,	"int4 int4 int4 int4 int4" },		
  {"XDM_ETAT_SIGECL", 		XDM_ETAT_SIGECL,	"int4 int4 int4 int4 int4" },		
  {"XDM_ETAT_SIGVEN", 		XDM_ETAT_SIGVEN,	"int4 int4 int4 int4 int4" },		
  {"XDM_ETAT_SIGENR", 		XDM_ETAT_SIGENR,	"int4 int4 int4 int4 int4" },		
  {"XDM_ETAT_SIGINC", 		XDM_ETAT_SIGINC,	"int4 int4 int4 int4 int4" },		
  {"XDM_ETAT_EQUIPEMENT", 		XDM_ETAT_EQUIPEMENT,	"int4 int4 int4 int4 int4 int4 int4 int4 int4 int2" },
  {"XDM_ETAT_DOMAINE", 		XDM_ETAT_DOMAINE,	"int4 int4 int4 int4 int4" },		
  {"XDM_ETAT_ALERTE", 		XDM_ETAT_ALERTE,	"int4 int4 int4" },		
  {"XDM_ETAT_SEQSIG",	XDM_ETAT_SEQSIG,	"int4 int4 int4 int4 int4" },
  {"XDM_ETAT_SEQECL",	XDM_ETAT_SEQECL,	"int4 int4 int4 int4 int4" },
  {"XDM_ETAT_SEQVEN",	XDM_ETAT_SEQVEN,	"int4 int4 int4 int4 int4" },
  {"XDM_ETAT_SEQENR",	XDM_ETAT_SEQENR,	"int4 int4 int4 int4 int4" },
  {"XDM_ETAT_SEQINC",	XDM_ETAT_SEQINC,	"int4 int4 int4 int4 int4" },
  {"XDM_ETAT_SEQEXP",	XDM_ETAT_SEQEXP,	"int4 int4 int4 int4 int4" },
  /* AAZ */
  {"XDM_ETAT_PANNEAU", 		XDM_ETAT_PANNEAU,	"int4 int4 int4 int4 int4" },		
  {"XDM_ETAT_SEQUENCE_TUBE",	XDM_ETAT_SEQUENCE_TUBE,	"int4 int4 int4 int4 int4" },
  {"XDM_ETAT_SEQUENCE_ECH",	XDM_ETAT_SEQUENCE_ECH,	"int4 int4 int4 int4 int4" },
  {"XDM_ETAT_CAPTEUR", 		XDM_ETAT_CAPTEUR,	"int4 int4 int4" },	
  {"XDM_Etat_CO", 		XDM_Etat_CO,		"int2 int2 int2 int2 char" },	
  {"XDM_Etat_Opacite", 		XDM_Etat_Opacite,	"int2 int2 int2 int2 char" },	
  {"XDM_ETAT_Contraste",	XDM_ETAT_Contraste,		"int2 int2 int2" },
  {"XDM_EInit_IHM_Contraste",	XDM_EInit_IHM_Contraste,	"str" },	
  {"XDM_EInit_IHM_JourNuit",	XDM_EInit_IHM_JourNuit,		"str" },	
  {"XDM_EInit_IHM_Classe_DIV",	XDM_EInit_IHM_Classe_DIV,	"str int2" },	
  {"XDM_ECMD_MAT_NAV",	 	XDM_ECMD_MAT_NAV,		"int2 str int2_array int4" },	
  {"XDM_ECMD_ARRET_SCEN",	XDM_ECMD_ARRET_SCEN,		"int2 int4" },	
  {"XDM_ETAT_MAT_NAV",	 	XDM_ETAT_MAT_NAV,		"int2 str int2_array" },	

  {"XDM_ECMD_FIC",		XDM_ECMD_FIC,			"int4 int2 real8 " },
  {"XDM_MODE_LENT",		XDM_MODE_LENT,			"int2" },
  {"XDM_ETAT_LIAISON",		XDM_ETAT_LIAISON,		"int2 int2 int2" },
  {"XDM_EMOD_DEG",              XDM_EMOD_DEG,			"str" },
  {"XDM_EMOD_ISOLE",            XDM_EMOD_ISOLE,			"str" },
  {"XDM_EInit_IHM_MAT_NAV", 	XDM_EInit_IHM_MAT_NAV,		"str int2" },	

  {"XDM_EInit_IHM_PMVA", 	XDM_EInit_IHM_PMVA,	"str" },
  {"XDM_EInit_IHM_BAF", 	XDM_EInit_IHM_BAF,	"str" },	
  {"XDM_EInit_IHM_BAD", 	XDM_EInit_IHM_BAD,	"str" },
  {"XDM_EInit_IHM_EQT_SAGA", 	XDM_EInit_IHM_EQT_SAGA,	"str" },
  {"XDM_EInit_IHM_CFE", 	XDM_EInit_IHM_CFE,	"str" },
  {"XDM_EInit_IHM_CAP", 	XDM_EInit_IHM_CAP,	"str" },

  {"XDM_Acknoledge_Ala",	XDM_Acknoledge_Ala,		"real8 int2 int2 int2 int2" },

  {"XDM_DONNEES_METEO_QTX",	XDM_DONNEES_METEO_QTX,		"str str str" },
  {"XDM_ACQ_DONNEES_METEO_QTX",	XDM_ACQ_DONNEES_METEO_QTX,	"str" },

  {"XDM_ECMD_GTC",		XDM_ECMD_GTC,			"int2  int2 int2 int4" },

  {"XDM_ACQ_DONNEES_TDP",	XDM_ACQ_DONNEES_TDP,		"int2 int2" },
  {"XDM_EMES_TDP_6MN",		XDM_EMES_TDP_6MN,		"real8 int2 int2 str int2_array int2_array int2_array int2_array int2_array" },
  {"XDM_EMES_TDP_HORAIRE",	XDM_EMES_TDP_HORAIRE,		"real8 int2 int2 str int2_array int2_array int2_array int2_array" },
  {"XDM_EMES_TDP_6MN_PAR_ZONE",	XDM_EMES_TDP_6MN_PAR_ZONE,	"real8 int2 int2 int2" },

  {"XDM_ETDP_SURCHARGE", 		XDM_ETDP_SURCHARGE,			"int2 int4 int4 int2" },
  {"XDM_ETDP_ETAT_SURCHARGE", 		XDM_ETDP_ETAT_SURCHARGE,			"int2 int4 int4" },
  {"XDM_ETDP_INIT_SURCHARGE", 		XDM_ETDP_INIT_SURCHARGE,			"str" },
  {"XDM_ETDP_SEUIL", 		XDM_ETDP_SEUIL,			"int2 char char real8" },
  {"XDM_ETDP_IHM", 		XDM_ETDP_IHM,			"int2 int2 int2 str int4" },
  {"XDM_ETDP_IHM_PMVA", XDM_ETDP_IHM_PMVA,		"int2 int2 int2 int2 int2 str" },
  {"XDM_VAL_TDP", 		XDM_VAL_TDP,			"int2 str str str" },
  {"XDM_VAL_TDP_PMVA", 		XDM_VAL_TDP_PMVA,		"int2 str str str str" },
  {"XDM_ETDP_PMV", 		XDM_ETDP_PMV,			"int2 str int2 str char str int2 str char str int2 str char str int2 str char char int4 int4 int4 int4" },
  {"XDM_CR_TDP",                XDM_CR_TDP,                     "int4 int2 int2" },
  {"XDM_CR_TDP_PMVA",           XDM_CR_TDP_PMVA,                "int4 int2 int2" },
  {"XDM_EPAL_AUTO",             XDM_EPAL_AUTO,                  "int4 int2 int2 int2 real8 int2 int2 int4 str str int2 int2" },
  {"XDM_EPAL_AUTO_FIN",         XDM_EPAL_AUTO_FIN,              "int2 int2 real8 int2 str str" },

  {"XDM_ETAT_PMVA", XDM_ETAT_PMVA, "int2 str int2 str char str int2 str char str int2 str char str int2 str char str int2 str char char"},
  {"XDM_ECMD_PMVA", XDM_ECMD_PMVA, "int2 str int2 str int2 str int2 str int2 str int2 str int2 str int2 str int2 str int2 str int2 int2 int4"},
  {"XDM_ETDP_PMVA", XDM_ETDP_PMVA, "int2 str int2 str char str int2 str char str int2 str char str int2 str char str int2 str char char int4 int2 int2 int2 int2"},
  
  {"XDM_ECMD_EQT_SAGA", XDM_ECMD_EQT_SAGA, "int2 str int2 str str int2 str str int2 str str int2 str str int2 str int2 int2 str str int2 int4 int4"},
  {"XDM_ETAT_EQT_SAGA",XDM_ETAT_EQT_SAGA, "int2 str int2 str str int2 str str int2 str str int2 str str int2 str int2 int2 str str int2 int4 int4"}, 
  {"XDM_ETAT_PRV", XDM_ETAT_PRV, "int2 str str"},
  {"XDM_ECMD_PRV", XDM_ECMD_PRV, "int2 str str int4 str"},
  /* Ajout PIC */
  {"XDM_ETAT_PIC", XDM_ETAT_PIC, "int2 str str str"},
  {"XDM_ECMD_PIC", XDM_ECMD_PIC, "int2 str str str int4 str"},
  {"XDM_RECORD_DAI", XDM_RECORD_DAI, "int2 str int2" },

  {"XDM_ETAT_CFE", XDM_ETAT_CFE, "int2 int4 str"},
  {"XDM_ECMD_CFE", XDM_ECMD_CFE, "int2 int2 int4 str"},
  {"XDM_ETAT_CAP", XDM_ETAT_CAP, "int2 int2 int2"},
  {"XDM_ECMD_CAP", XDM_ECMD_CAP, "int2 int2 int4 str"},
  {"XDM_ETAT_IMU", XDM_ETAT_IMU, "int2 str int4 int4 int4 int4"},
  {"XDM_ECMD_IMU", XDM_ECMD_IMU, "int2 str int4 int4 int4 int4 str"},
  {"XDM_ETAT_SONO", XDM_ETAT_SONO, "str int4 int4 int4"}, /* Etat : message autoroute sens PR_debut PR_fin PAUErreur PAUPioltes NumCommande */
//  {"XDM_ECMD_SONO", XDM_ECMD_SONO, "str str int4 int4 int4 int4 str"}, /* Etat : message autoroute sens PR_debut PR_fin NumCommande Machine */
  {"XDM_ECMD_SONO", XDM_ECMD_SONO, "int4 int4 int4 int4 str str str "}, /* Etat : message autoroute sens PR_debut PR_fin NumCommande Machine */
  {"XDM_AT_SONO", 		XDM_AT_SONO, 		"int4 real8 char int2 int2" },
  
  {"XDM_ETAT_BAF", 		XDM_ETAT_BAF,		"int2 int2 str_array" },
  {"XDM_ETAT_BAD", 		XDM_ETAT_BAD,		"int2 int2 str_array" },
  {"XDM_ECMD_BAF", 		XDM_ECMD_BAF,		"int2 int2 str int2 int4"},		
  {"XDM_ECMD_BAF_GLOBALE",	XDM_ECMD_BAF_GLOBALE,	"int2 str str str str str str int2 int4"},		
  {"XDM_ECMD_BAD", 		XDM_ECMD_BAD,		"int2 int2 str int2 int4"},	
  {"XDM_ENAV_AUTO",             XDM_ENAV_AUTO,          "int4 int2 int2 int2 real8 int2 int2 int4 str int2 int2" },
  {"XDM_ENAV_AUTO_FIN",         XDM_ENAV_AUTO_FIN,      "int2 int2 real8 int2 str" },

  {"XDM_EBAF_AUTO",             XDM_EBAF_AUTO,          "int4 int2 int2 int2 real8 int2 int2 int4 int2_array int2 int2" },
  {"XDM_EBAF_AUTO_FIN",         XDM_EBAF_AUTO_FIN,      "int2 int2 real8 int2 int2_array" },

  {"XDM_EPMV_AUTO",             XDM_EPMV_AUTO,          "int4 int2 int2 int2 real8 int2 str str str str str str int2 int2 int2 int2 str str int2 int2 int4 str str int2 int4 int4 int2 int4" },
  {"XDM_EPMV_AUTO_FIN",         XDM_EPMV_AUTO_FIN,      "int2 int2 real8 int2" },

  {"XDM_EBAD_AUTO",             XDM_EBAD_AUTO,          "int4 int2 int2 int2 real8 int2 int2 int4 str int2 int2" },
  {"XDM_EBAD_AUTO_FIN",         XDM_EBAD_AUTO_FIN,      "int2 int2 real8 int2 str" },

  {"XDM_EPMVA_AUTO",             XDM_EPMVA_AUTO,          "int4 int2 int2 int2 real8 int2 str str str str str str str str str str int2 int2 int2 int2 int2 int2 int2 int4 int2 int2 int2 int2 int2 int2" },
  {"XDM_EPMVA_AUTO_FIN",         XDM_EPMVA_AUTO_FIN,      "int2 int2 real8 int2" },

  {"XDM_ETUNNEL_AUTO",             XDM_ETUNNEL_AUTO,          "int4 int2 int2 int2 real8 int2 int2 int4 str" },
  {"XDM_ETUNNEL_AUTO_FIN",         XDM_ETUNNEL_AUTO_FIN,      "int2 int2 real8 int2" },

  {"XDM_ECMD_AUTOTEL2",       XDM_ECMD_AUTOTEL2,      "int4 str int2 str" },
  {"XDM_ECMD_AUTORAD",                XDM_ECMD_AUTORAD,       "int4 str str int2 str"},
  {"XDM_EPRV_AUTO",             XDM_EPRV_AUTO,          "int4 int2 int2 int2 real8 int2 str str int2 int4 int2" },
  {"XDM_EPRV_AUTO_FIN",         XDM_EPRV_AUTO_FIN,      "int4 int2 int2 int2 real8 int2 str str int2 int4 int2" },
  {"XDM_ECMD_BASC_RAU",              XDM_ECMD_BASC_RAU,  "str str" },
  {"XDM_ETAT_BAF_EMS", 		XDM_ETAT_BAF_EMS,		"int2 int2 str" },
  {"XDM_ETAT_BAD_EMS", 		XDM_ETAT_BAD_EMS,		"int2 int2 str" },
  {"XDM_ETAT_NAV_EMS", 		XDM_ETAT_NAV_EMS,		"int2 int2 str" },
  {"XDM_ETAT_PAL_EMS", 		XDM_ETAT_PAL_EMS,		"int2 int2 str" },


/* Messages Sous-Systeme BASEDD */

  {"XDM_AA_alerte", 		XDM_AA_alerte, 		"real8 char int2 char str" }, 
  {"XDM_AA_alerte_localisee", 		XDM_AA_alerte_localisee, 		"real8 char int2 char str str int4 int4" }, 
  {"XDM_AM_alarme", 		XDM_AM_alarme, 		"real8 char int2 int2 char str" }, 
  {"XDM_AT_TUBE", 		XDM_AT_TUBE, 		"int4 real8 char" }, 
  {"XDM_AT_echangeur", 		XDM_AT_echangeur, 	"int4 real8 char" },
  /* AAZ 11/10/2007 */
  {"XDM_AT_SIG", 		XDM_AT_SIG, 		"int4 real8 char" }, 
  {"XDM_AT_ECL", 		XDM_AT_ECL, 		"int4 real8 char" }, 
  {"XDM_AT_VEN", 		XDM_AT_VEN, 		"int4 real8 char" }, 
  {"XDM_AT_ENR", 		XDM_AT_ENR, 		"int4 real8 char" }, 
  {"XDM_AT_INC", 		XDM_AT_INC, 		"int4 real8 char" }, 
  {"XDM_AT_EXP", 		XDM_AT_EXP, 		"int4 real8 char" }, 
  /* AAZ */
  {"XDM_AT_NAV", 		XDM_AT_NAV, 		"int4 real8 char" }, 
  {"XDM_AT_PAL", 		XDM_AT_PAL, 		"int4 real8 char" }, 
  {"XDM_AT_PMV", 		XDM_AT_PMV, 		"int4 real8 char" },
  {"XDM_AT_IMU", 		XDM_AT_IMU, 		"int4 real8 char" },
  {"XDM_A_INIT_IHM", 		XDM_A_INIT_IHM, 	"" },
  {"XDM_A_TIMER_RAFRAI_IHM", 	XDM_A_TIMER_RAFRAI_IHM,	"str" },
  {"XDM_AN_PURGE", 		XDM_AN_PURGE,		"str" },
  {"XDM_AG_PURGE", 		XDM_AG_PURGE,		"str" },
  {"XDM_AG_PURGEHISTO", 	XDM_AG_PURGEHISTO,	"str" },
  {"XDM_AS_FdeT", 		XDM_AS_FdeT,		"str int2 char char real8 char" },
  {"XDM_AH_INITLT",             XDM_AH_INITLT,          "str" },
  {"XDM_AH_RADT_DAI",           XDM_AH_RADT_DAI,        "str" },
  {"XDM_AS_BOUCHON",            XDM_AS_BOUCHON,         "str char int4 int4 real8" },
  {"XDM_AS_INRETS",             XDM_AS_INRETS,          "str char int4 int4 real8 int2" },
  {"XDM_AS_TDP",		XDM_AS_TDP,		"str int2 real8" },
  {"XDM_AH_SATIR",              XDM_AH_SATIR,           "str" },
  {"XDM_AEQA",                  XDM_AEQA,               "char int4 int4 int4 int4 int4 int4 int4 int4 int4 int4 real8 real8 real8 int2" },
  {"XDM_EInit_OP_VW", 	        XDM_EInit_OP_VW,	"int4 int4 int2" },
  {"XDM_AT_INI_EQT", 	        XDM_AT_INI_EQT,		"int2 int4 str" },
  {"XDM_AT_INI_IHM", 	        XDM_AT_INI_IHM,		"int2 int4 str" },
  {"XDM_A_TIMER_TRV_COURS",     XDM_A_TIMER_TRV_COURS,	"str" },
  {"XDM_A_PRES_OP",     	XDM_A_PRES_OP,		"int2 int2 str" },
  {"XDM_AS_TIMER_SRV_RADT", 	XDM_AS_TIMER_SRV_RADT, 	"" },
  {"XDM_AS_TIMER_PURGE_RADT", 	XDM_AS_TIMER_PURGE_RADT, "" },
  {"XDM_AS_TIMER_CLT_RADT", 	XDM_AS_TIMER_CLT_RADT, 	"" },
  {"XDM_AS_TIMER_SRVEVT_STRADA",XDM_AS_TIMER_SRVEVT_STRADA, 	"" },
  {"XDM_AS_TIMER_CLT_EVT",     	XDM_AS_TIMER_CLT_EVT,  "" },
  {"XDM_AS_TIMER_POLYVOX",     	XDM_AS_TIMER_POLYVOX,  "" },
  {"XDM_AS_TIMER_TDP_FCD",     	XDM_AS_TIMER_TDP_FCD,  "" },
  {"XDM_AS_TIMER_DATEX2",     	XDM_AS_TIMER_DATEX2,  "" },
  {"XDM_AS_TIMER_SITU_RECAP", 	XDM_AS_TIMER_SITU_RECAP,  "" },
  {"XDM_AS_TIMER_PAT",		XDM_AS_TIMER_PAT, "" },
  {"XDM_ADA_TIMER_RADT",	XDM_ADA_TIMER_RADT, "" },
  {"XDM_ADA_EVT", 		XDM_ADA_EVT,  "int4 int2" },
  {"XDM_AS_TIMER_SITU_CPC", 	XDM_AS_TIMER_SITU_CPC,  "int4 int2 int4" },
  {"XDM_AS_TIMER_SAGA_60",     	XDM_AS_TIMER_SAGA_60,  "" },
  {"XDM_AS_TIMER_SAGA_3600",   	XDM_AS_TIMER_SAGA_3600,  "" },
  {"XDM_AS_TIMER_SAGA_DATE",   	XDM_AS_TIMER_SAGA_DATE,  "" },
  {"XDM_AS_SRVEVT_STRADA",	XDM_AS_SRVEVT_STRADA, 	"" },
  {"XDM_AS_EVT_STRADA",		XDM_AS_EVT_STRADA, 	"str int4 str str str" },
  {"XDM_A_TIMER_FMC_LOC",	XDM_A_TIMER_FMC_LOC, "str" },
  {"XDM_AG_IMPCAT",		XDM_AG_IMPCAT, ""},
  {"XDM_AG_EXPCATF",            XDM_AG_EXPCATF, ""},
  {"XDM_AG_EXPCATC",            XDM_AG_EXPCATC, ""},
  {"XDM_AW_TIMER",              XDM_AW_TIMER,  "" },
  {"XDM_NOMADE_TIMER",          XDM_NOMADE_TIMER,  "" },
  {"XDM_NOMADE_REVEIL",          XDM_NOMADE_REVEIL,  "" },
  {"XDM_AMISOL_TIMER",          XDM_AMISOL_TIMER,  "" },
  {"XDM_AT_PMVA", 				XDM_AT_PMVA, 		"int4 real8 char" },
  {"XDM_AT_EQT_SAGA", 			XDM_AT_EQT_SAGA,	"int4 real8 char" },
  {"XDM_AT_PRV", 				XDM_AT_PRV, 		"int4 real8 char" },
  {"XDM_AT_PIC", 				XDM_AT_PIC, 		"int4 real8 char" },
  {"XDM_AT_BAF", 				XDM_AT_BAF, 		"int4 real8 char" },
  {"XDM_AT_BAD", 				XDM_AT_BAD, 		"int4 real8 char" },
  {"XDM_AT_CFE", 				XDM_AT_CFE, 		"int4 real8 char" },
  {"XDM_AT_CAP", 				XDM_AT_CAP, 		"int4 real8 char" },
  {"XDM_AS_SRVEVT_STRADA_TDP",  XDM_AS_SRVEVT_STRADA_TDP,       "" },
  {"XDM_ATDP_TIMER",              XDM_ATDP_TIMER,  "" },
  {"XDM_ATDP_TIMER_ASF",              XDM_ATDP_TIMER_ASF,  "" },
  {"XDM_AS_SRVEVT_STRADA_RDT",  XDM_AS_SRVEVT_STRADA_RDT,       "" },
  {"XDM_AS_SRVEVT_STRADA_FMC",  XDM_AS_SRVEVT_STRADA_FMC,       "" },
  {"XDM_AA_COMMU_FMC",  	XDM_AA_COMMU_FMC,       "int4_array int4_array int4_array" },
  {"XDM_COMMU_FMC", 		XDM_COMMU_FMC, "int4 int4 int2 bool int4 int4"},
  {"XDM_ANEUTR_TIMER",		XDM_ANEUTR_TIMER,	"str" },
  {"XDM_RMVI_DEMANDE",		XDM_RMVI_DEMANDE,	"str str int4 int2 str" },
  {"XDM_RMVI_STATUS",		XDM_RMVI_STATUS,	"int4 str" },
  {"XDM_RMVI_RETOUR",		XDM_RMVI_RETOUR,	"str str_array str_array int4_array int4_array str_array int4_array int4_array int4_array" },
  {"XDM_RMVI_RETOUR_EMS",	XDM_RMVI_RETOUR_EMS,	"str str str int4_array int4_array str int4_array int4_array int4_array" },
  {"XDM_AVA_CONNEXION",		XDM_AVA_CONNEXION,	"str str str int4 int4" },
  {"XDM_ODI_C",			XDM_ODI_C,		"str int2 str str str" },
  {"XDM_AA_alerte_cockpit",	XDM_AA_alerte_cockpit,	"int4 int4 int4 char real8 char int2 char str str int4 int4 int4 int4 char" },
  {"EMSG_TDP_FCD",              EMSG_TDP_FCD,           "real8 int4 int4 int2 int4 int2 str"},
  {"XDM_API_POKE",              XDM_API_POKE,           "int4 int4 str str"},
  {"XDM_INIT_TITRE",              XDM_INIT_TITRE,           "str"},
  {"XDM_EXEC_TITRE",  XDM_EXEC_TITRE, "int4 int4 int4 int4 int4 str str" },
  {"XDM_API_NEWTASK", XDM_API_NEWTASK, "str str int4 str" },
  {"XDM_INIT_TISSE",              XDM_INIT_TISSE,           "str"},
  {"XDM_AS_UPDATE_PA", 			  XDM_AS_UPDATE_PA, 		"int4 int2 int4 int4" },

 /* Message Magistra */
  {"XDM_MGA_INIT_PO",                   XDM_MGA_INIT_PO,  "str str str str str"},
  {"XDM_MGA_CMD_DEL_SEMA",              XDM_MGA_CMD_DEL_SEMA,           "str str int4 int4"},
  {"XDM_MGA_CMD_CFG_PRA_PO",            XDM_MGA_CMD_CFG_PRA_PO,           "str str"},
  {"XDM_MGA_CMD_LIVRE_MODERNE",         XDM_MGA_CMD_LIVRE_MODERNE,           ""},
  {"XDM_MGA_CMD_JAR_REMORQUE",          XDM_MGA_CMD_JAR_REMORQUE,           "str"},
  {"XDM_MGA_CMD_HIVERNAGE",             XDM_MGA_CMD_HIVERNAGE,           "str int4 int4"},
  {"XDM_MGA_CMD_HIVERNAGE_ASF",         XDM_MGA_CMD_HIVERNAGE_ASF,           "str str"},
  {"XDM_MGA_CMD_HIVERNAGE_EXPORT",      XDM_MGA_CMD_HIVERNAGE_EXPORT,           "str str"},
  {"XDM_MGA_CMD_RETOUR",                XDM_MGA_CMD_RETOUR,           "int4 str int4"},
  {"XDM_MGA_CMD_ALARME_LT",             XDM_MGA_CMD_ALARME_LT,           "str str"},
  {"XDM_MGA_CMD_JAR_REMORQUE_CA",       XDM_MGA_CMD_JAR_REMORQUE_CA,           "str"},
  {"XDM_MGA_CONFIG_PO",                 XDM_MGA_CONFIG_PO, "str str str str str"},

 
 /* Messages gateways externes */
  {"XDM_GTW_AVA_CREATION",              XDM_GTW_AVA_CREATION,       "str str int4 str str str str str str str int4 int4 str str str str str int4 str str str str str int4"},
  {"XDM_GTW_AVA_MODIFICATION",          XDM_GTW_AVA_MODIFICATION,   "str str int4 str str str str str str str int4 int4 str str str str str int4 str str str str str int4"},
  {"XDM_GTW_AVA_RETOUR_ALERTE",         XDM_GTW_AVA_RETOUR_ALERTE,   "int4 int4 int4 int4 int4 int4"},
  {"XDM_GTW_PIT_BOUCHON",               XDM_GTW_PIT_BOUCHON,       "str str int4 str str str str str str str int4 int4 str str str str str int4 str str str str str int4"},
  {"XDM_GTW_PIT_BOU_RETOUR_ALERTE",     XDM_GTW_PIT_BOU_RETOUR_ALERTE,   "int4 int4 int4 int4 int4 int4"},
  {"XDM_GTW_RAD_RETOUR_APPEL",          XDM_GTW_RAD_RETOUR_APPEL,   "int4"},  
  {"XDM_GTW_RAD_APPEL",                 XDM_GTW_RAD_APPEL,       "str str str int2 str str str str"},
  {"XDM_GTW_TEL_RETOUR_APPEL",          XDM_GTW_TEL_RETOUR_APPEL, "int4"},
  {"XDM_GTW_TEL_APPEL",                 XDM_GTW_TEL_APPEL, "str str str int2"},
 
 /* Messages Sous-Systeme GECONF */
  {"XDM_CSV_SBD",		XDM_CSV_SBD,		"" },
  {"XDM_CSV_ARC",		XDM_CSV_ARC,		"int4 int4" },
  {"XDM_CSV_INF",		XDM_CSV_INF,		"str" },
  {"XDM_CSV_RES",		XDM_CSV_RES,		"int4 int4 int4 str" },
  {"XDM_CSV_CANAL",		XDM_CSV_CANAL,		"str" },

  {"XDM_CGD_LIV",		XDM_CGD_LIV,		"" },
  {"XDM_CGD_DIF",		XDM_CGD_DIF,		"" },
  {"XDM_CGD_MBD",		XDM_CGD_MBD,		"int4" },
  {"XDM_CGD_VRF",		XDM_CGD_VRF,		"int4" },
  {"XDM_CGD_DBD",		XDM_CGD_DBD,		"int4" },
  {"XDM_CGD_CANAL",		XDM_CGD_CANAL,		"str" },
  {"XDM_CGD_CHECK",		XDM_CGD_CHECK,		"int4" },
  {"XDM_CGD_CHGB",		XDM_CGD_CHGB,		"str" },

/* Messages Sous-Systeme MAINTE */

  {"XDM_M_RELEVER_COMPTEUR",	XDM_M_RELEVER_COMPTEUR,	"str" },
  {"XDM_M_COMPTEUR_RELEVE",	XDM_M_COMPTEUR_RELEVE,	"str" },
  {"XDM_M_TIMER_COMPTEUR",	XDM_M_TIMER_COMPTEUR,	"str" },
    { "XDM_TSE_KEEPALIVE",XDM_TSE_KEEPALIVE,"str"},


  { "", 0, "" }

} ;


/* declaration de fonctions internes */

/* definition de fonctions externes */

