/*E*/
/* Fichier : $Id: xdc.h,v 1.381 2021/05/03 14:05:18 pc2dpdy Exp $        $Revision: 1.381 $        $Date: 2021/05/03 14:05:18 $
-------------------------------------------------------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
-------------------------------------------------------------------------------------------------------
* SOUS-SYSTEME XDMICG
-------------------------------------------------------------------------------------------------------
* MODULE XDTYPE * FICHIER xdc.h
-------------------------------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Definition des constantes
*
-------------------------------------------------------------------------------------------------------
* HISTORIQUE :
*
* NOEL         17 Aou 1994 : Creation
* NAGIEL       19 Sep 1994 : Ajout constantes PMV et PICTO
* GABORIT      23 Sep 1994 : Ajout XDC_MAI_eqt 							(v1.31)
* NAGIEL       23 Sep 1994 : Ajout constantes contraste
* NAGIEL       26 Sep 1994 : Ajout XDC_PICTO_PASPICTO
* TORREGROSSA  26 Sep 1994 : Ajout XDC_CHAINE_VIDE
* TORREGROSSA  27 Sep 1994 : Ajout XDC_EQT_MAJEUR, XDC_EQT_MINEUR,
*                            XDC_EQT_HS, XDC_EQT_INHIBE, XDC_EQT_A_DESACTIVER
*                            XDC_EQT_A_REACTIVER, XDC_EQT_CRITIQUE,
*                            XDC_EQT_DESACTIVE
* LECRIVAIN    27 Sep 1994 : Ajout libelles types equipts.
* GABORIT      28 Sep 1994 : Ajout sens de circulation 						(v1.38)
* GABORIT      28 Sep 1994 : Ajout XDC_FMC_ETAT_xxx 						(v1.44)
* LECRIVAIN    28 Sep 1994 : Ajout libelles sens de circulation 				(v1.46)
* LECRIVAIN    29 Sep 1994 : Ajout codes de fonctions du logiciel 				(v1.48)
* TORREGROSSA  30 Sep 1994 : Ajout les criteres de recherche sur le type operation d'une FMC :
*                            XDC_TYP_FMC_EVT, XDC_TYP_FMC_OPR, XDC_PAS_TYP_FMC
* GABORIT      03 oct 1994 : Ajout des types de voies 						(v1.54)
* GABORIT      04 oct 1994 : idem (suite) 							(v1.55)
* NAGIEL       04 Oct 1994 : Ajout XDC_JOUR et XDC_NUIT
* GABORIT      05 oct 1994 : Ajout XDC_ALT_xx (alertes) 					(v1.58)
* NIEPCERON    05 oct 1994 : Ajout XDC_DEBUT_TRAVAUX 
*                            XDC_FIN_TRAVAUX (basculement) 					(v1.60)
*                            Ajout des types d'objets mobiles    				(v1.60)
* VOLCIC       06 Oct 1994 : Ajout XDC_BASE_CFG
*		             XDC_BASE_CFT 							(V1.61) 
* T.Milleville 10 Oct 1994 : Ajout XDC_REP_TMP 							(v1.66)
* NAGIEL       11 Oct 1994 : Ajout XDC_PMV_FLASHPRESENT et Ajout XDC_PMV_FLASHABSENT 		(v1.67)
* GABORIT      11 Oct 1994 : modif en XDC_SYN_xxx des objets mobiles 				(v1.68)
* NIEPCERON    12 Oct 1994 : Ajout des Noms de Macro APPLIX : XDC_MACRO_... 			(v1.71) 
* MERCIER      13 Oct 1994 : Ajout de XDC_Lon_TypeFich 						(v1.72)
* NIEPCERON    13 Oct 1994 : Deplacement des XDC_SYN_xxx en zone non applis  			(v1.76)
* GABORIT      13 Oct 1994 : Ajout de XDC_LU 							(v1.79)
* GABORIT      14 Oct 1994 : Modif de XDC_xxx=0 en XDC_xxx!=0 					(v1.81)
* MERCIER      14 OCT 1994 : Ajout de XDC_BILAN_CONFIG_PATH  					(V1.82)
* C.T.         14 OCT 1994 : Ajout de XDC_VOIE_NEUTRALISEE et
*                            des intervenants pour action intervention
* TORREGROSSA	17 Oct 1994 : Modif valeur de XDC_ACT_APPEL et ajout XDC_ACT_BIP
* GABORIT	18 Oct 1994 : Ajout XDC_VOIE_BLOQUEE_NEUTRALISEE 				(v1.86)
* LECRIVAIN	19 Oct 1994 : Ajout codes et libelles de types de bip 				(v1.87)
* TORREGROSSA	19 Oct 1994 : Ajout les textes de reveil pour validation FMC
*                             et code d'erreur XDC_PRC_INC (si procedure inconnue)
* NAGIEL	19 Oct 1994 : Rien 								(v1.89)
* TORREGROSSA	19 Oct 1994 : Ajout les textes de reveil pour validation FMC
*                             et code d'erreur XDC_PRC_INC (si procedure inconnue)
* VOLCIC	19 Oct 1994 : Ajout XDC_Lon_NomAuto 						(v1.90)
* CORNU		20 Oct 1994 : Ajout XDC_MONITEUR1 a XDC_MONITEUR6 				(v1.92)
* TORREGROSSA	20 Oct 1994 : Ajout les codes d'erreur de verification de localisation
*                             et XDC_PR_INCONNU, XDC_AUT_INCONNUE
* GABORIT	21 Oct 1994 : Ã©change des valeurs XDC_SENS_xxx entre nord et sud		(v1.94)
* TORREGROSSA	24 Oct 1994 : Ajout le texte pour un reveil                                     (v1.95)
*
* T .Milleville 26 Oct 1994 : Ajout constante XDC_EXEC_SCON  V 1.96
* TORREGROSSA	26 Oct 1994 : Ajout les valeurs des mesures de trafic dans le cas non valuees	(v1.97)
* TORREGROSSA	27 Oct 1994 : Ajout les types de courbes de mesure trafic  			(v1.98)
* LECRIVAIN	03 Nov 1994 : Ajout primitives appelees par synoptique 				(v1.103)
* NOEL		03 Nov 1994 : Rien								(V1.104)
* NIEPCERON     03 Nov 1994 : Ajout des XDC_AST_  		                         	(v1.105)
* NAGIEL	04 Nov 1994 : Ajout de XDC_DEBUT_ALARME et XDC_FIN_ALARME 			(v1.106)
*
* MERCIER	07 Noc 1994 : Ajout								(v1.107)
*                             de XDC_NOM_SITE_CI,XDC_NOM_SITE_DN,XDC_NOM_SITE_CA
* LECRIVAIN   	03 Nov 1994 : Ajout primitive MTUE_xxx pour le synoptique 			(v1.108)
* CORNU		09 Nov 1994 : Ajout du nom des macros tunnel & echangeur  			(v1.109)
* GUILHOU	09 Nov 1994 : modif des constantes XDC_VOIE_BAU_xxx      			(v1.110)
* GABORIT	09 Nov 1994 : ajout XDC_DATE_NULLE      					(v1.111, v1.112)
* TORREGROSSA	10 Nov 1994 : ajout XDC_DISTANCE_INCONNUE      					(v1.113)
* NIEPCERON     16 Nov 1994 : ajout XDC_RES_EAU ...                                             (v1.114)		
* NIEPCERON     17 Nov 1994 : ajout XDC_RADT_Vx et XDC_RADT_CHAUSSEE ...                        (v1.115)
* NIEPCERON     17 Nov 1994 : ajout de XDC_MACRO_MTMTEI  et XDC_MACRO_MTMTLM                    (v1.116)
* NIEPCERON     17 Nov 1994 : ajout de XDC_MACRO_MTEI  et XDC_MACRO_MTLM                        (v1.117)
* CORNU		20 Nov 1994 : Ajout de XDC_NORMAL & XDC_DEFAUT pour retour d'abonnement  	(v1.118)
* NAGIEL	21 Nov 1994 : Ajout de XDC_Lon_Nb_Mod_Nav		                        (v1.119)
* GUILHOU	24 Nov 1994 : Ajout des libelles des actions					(v1.121)
* MISMER	28 Nov 1994 : Ajout des XDC_GTC_NICE						(v1.121)
* LECRIVAIN	30 Nov 1994 : Ajout type d'intervenant XDC_ACT_INT_AUTRE  			(v1.125)
* MISMER	02 Dec 1994 : Ajout des XDC automate telephonique et operator			(v1.126)
* VOLCIC	05 Dec 1994 : Changt XDC_EQT_MAG et XDC_CAPT 					(v1.127)
* GUILHOU	05 Dec 1994 : Changt sens "tous" en sens "inconnu" 				(v1.128)
* MERCIER	07 Dec 1994 : Ajout de XDC_MACHINE_GECONF	                                (v1.129)
* NAGIEL	13 Dec 1994 : Ajout de XDC_EQTALL                                               (v1.131)
* volcic	13 Dec 1994 : Ajout de XDC_Lon_Texte_Alarme  	                                (v1.132)
*			      Modif ITMA_TCA_pilotage_d_une_camera
* MERCIER	15 Dec 1994 : Ajout de XDC_BASEDD		                                (v1.133)
*                             XDC_EQUEXT XDC_GECONF XDC_GESIHM XDC_MAINTE XDC_SAIDEC XDC_SIMTST
*                             XDC_SUPERV XDC_XDMICG qui contiennent les noms des sous-systemes 
* LECRIVAIN	16 Dec 1994 : Ajout famille speciale XDC_FAM_DEV_MIGRAZUR                       (v1.134)
* NIECPERON     19 Dec 1994 : Ajout de constantes pour l'affichage des courbe          (v1.135 et 1.136)
* THOMAS	19 Dec 1994 : Modif des constantes donnant les paths de la BDC                  (v1.137)
* NAGIEL	19 Dec 1994 : Ajout de XDC_NOM_CELLULEJN  	                                (v1.138)
* LECRIVAIN	19 Dec 1994 : Ajout de noms de macros Applix                                    (v1.139)
* THOMAS	20 Dec 1994 : Modif des constantes donnant les paths de la BDC                  (v1.140)
* GABORIT	21 Dec 1994 : modif texte reveil      					        (v1.142)
* volcic        22 Dec 1994 : Ajout XDC_Lon_TrameMesure                                         (v1.143)
* Nouaille M.   02 Jan 1995 : Ajout cstes telephone                                             (v1.144)
* NIECPERON     03 jan 1995 : Ajout  XDC_MACRO_MTLE                                             (v1.148)
* Nouaille M.   03 Jan 1995 : Ajout cstes Operator PHASE_n                                      (v1.151)
* NIECPERON     03 jan 1995 : Ajout XDC_RADT_DIV_INCONNU                                        (v1.152)
* GABERT     	04 jan 1995 : Ajout XDC_Lon_Ligne_SATIR	                                        (v1.153)
* MISMER     	12 jan 1995 : Modif XDC_Lon_NomTache	                                        (v1.158)
* GABORIT	17 jan 1994 : ajout DATE_INFINIE      					        (v1.160)
* LECRIVAIN	18 Jan 1995 : Ajout des constantes d'etats RAU                                  (v1.161)
* CORNU         23 Jan 1995 : Ajout de XDC_MONITEUR7 & XDC_MONITEUR8                            (v1.162)
* Guilhou	24 jan 1995 : modif libelles des sens de circulation				(v1.163)
* Cornu         25 jan 1995 : Ajout des constantes videos                                       (v1.165)
* NIECPERON     25 jan 1995 : Ajout XDC_AST_ESCOTA                                       	(v1.166)
* GABORIT	26 jan 1994 : ajout XDC_PATH_EQA      					        (v1.167)
* LECRIVAIN	27 Jan 1995 : Ajout des constantes de commandes machines                       (v1.168)
* LECRIVAIN	10 Fev 1995 : Ajout des constantes de format d'impression  (v1.169)
* TORREGROSSA	14 Fev 1995 : Ajout reveils (1.170)
* MISMER	16 Fev 1995 : Ajout XDC_ETAT_TOUS_EQT  (1.171)
* MISMER	22 Fev 1995 : Ajout XDC_Lon_Matrice XDC_Lon_Params_Matrice (1.172)
* MISMER	23 Fev 1995 : Ajout XDC_EQT_ETAT_INHIBE et XDC_EQT_ETAT_NON_INHIBE (1.173)
* Guilhou	28 Fev 1995 : Ajout XDC_PR_FRONTIERE	 (1.174)
* MISMER	23 Fev 1995 : Ajout XDC_EQT_ETAT_INHIBE et XDC_EQT_ETAT_NON_INHIBE (1.173)
* TORREGROSSA	14 Mar 1995 : Ajout reveils et XDC_Lon_TexteReveil (1.176)
* MISMER	16 Mar 1995 : Ajout XDC_PMV_ETEINDRE  (1.177)
* T.Milleville	20 Mar 1995 : Ajout XDC_PATHREF_FICADM  (1.178)
* F.Volcic	24 Mar 1995 : Modif diverses sur XDC_EQT_HS ... (1.179)
* NIECPERON     24 jan 1995 : Ajout XDC_Lon_Ech (1.180)                                       	(v1.166)
* TORREGROSSA	27 Mar 1995 : Ajout XDC_POINT_CARACT_POS_DEF , 
*                             XDC_ALT_CONF_EVT, XDC_SYN_TRAVAUX_MOBILES_TETE_REELS 
*                             XDC_SYN_TRAVAUX_MOBILES_QUEUE_REELS,
*                             XDC_SYN_TRAVAUX_MOBILES_QUEUE_PREVUS,
*                             et XDC_SYN_TRAVAUX_MOBILES_TETE_PREVUS,
*                             pour les scenario et sequence de fin :
*                             XDC_SCN_LIB_NAV, XDC_SEQ_OUV_TUN, XDC_SEQ_OUV_ECH(1.181)
* TORREGROSSA	28 Mar 1995 : Modif libelle XDC_LIB_NAV (v1.182)
* TORREGROSSA	30 Mar 1995 : Ajout objets mobiles pour accident  (v1.183)
* LECRIVAIN	31 Mar 1995 : Ajout etat Marche/arret (pour repondeur) (v1.184)
* GABORIT	03 avr 1995 : Ajout de XDC_EQT_SAT (1.186)
* NIEPCERON	03 avr 1995 : Ajout de XDC_SYN_METEO... (1.187)
* NIEPCERON	04 avr 1995 : Ajout de XDC_SYN_INTERVENANT_AUTRE (1.188)
* LECRIVAIN	06 Avr 1995 : Ajout constante XDC_SITE_INCONNU           v1.189
* NIEPCERON	04 Mai 1995 : Ajout des XDC_NOM_DISTRICT_DN XDC_NOM_DISTRICT_DM et 
			      XDC_MACRO_MTCACI v1.190
* NIEPCERON	22 Mai 1995 : Ajout de  XDC_MACRO_MTIA     XDC_MACRO_MTID XDC_MACRO_MTIP
				XDC_MACRO_MTFO XDC_MACRO_MTMO    " XDC_MACRO_MTGA v1.191
* TORREGROSSA	29 Mai 1995 : Ajout de  XDC_ACT_REFUSEE v1.192
* NIEPCERON	06 Jun 1995 : Modification de XDC_Lon_Datagroup (21->41) v1.193
* NIEPCERON     06 Jun 1995 : Modification de XDC_Lon_Datagroup (41->61) v1.194
* NIEPCERON     06 Jun 1995 : Modification de XDC_Lon_Datagroup (41->61 effectif) v1.195
* TORREGROSSA	14 Jun 1995 : Modif valeur de XDC_PMV_DEFAUT V1.197
* GABORIT       23 Jun 1995 : ajout XDC_DELAI_RTSERVER pour reconnexion au rtserver (1.198)
* TORREGROSSA	23 Jun 1995 : ajout XDC_ETAT_SUCCES_FIN et XDC_ETAT_ECHEC_FIN 
*                             ajout des textes des reveils automatiques             (1.199)
* TORREGROSSA	04 Jul 1995 : Ajout des noms machines et des libelles (1.200)
*                             et XDC_PATHREFFICINF
* NIEPCERON     06 Jul 1995 : Ajout de XDC_PATH_INF 1.201
* T.Milleville  25 Jul 1995 : Ajout de XDC_EXIT_RTCLIENT 1.202
* F.Volcic	08 Aou 1995 : Ajout de XDC_NB_ETOR et XDC_NBMAX_ETOR  (1.203)
* TORREGROSSA	09 Aou 1995 : Ajout de XDC_OFFSET_PURGE_BASE (1.204)
* GABORIT       21 sep 1995 : modif scrogneugneu (1.205)
* TORREGROSSA	25 Sep 1995 : Modif accent libelle (1.206)
* NIEPCERON     05 Sep 1995 : Ajout de XDC_PATH_SYN (1.207)
* T.Milleville  12 Oct 1995 : Ajout de XDC_ACTIF_CI, XDC_ACTIF_DN , XDC_ACTIF_DM
*	XDC_PASSIF_CI, XDC_PASSIF_DN, XDC_PASSIF_DM   V1.208
* TORREGROSSA	24 Oct 1995 : Ajout XDC_PR_NOR, XDC_PR_INV (1.209)
* T.Milleville  9 Nov 1995 : Ajout de la constante XDC_PATH_RTWORKS V1.210
* TORREGROSSA	13 Nov 1995 : Ajout XDC_LIB_SENS_NON_PEC (1.211)
* TORREGROSSA	01 Dec 1995 : Ajout XDC_EQT_FAX (1.212)
* GABORIT	27 Dec 1995 : Ajout XDC_PATHCMPT (1.213)
* TORREGROSSA	19 Jan 1996 : Modif libelle NAV par BRA pour action (1.214)
* TORREGROSSA	26 Jan 1996 : Modif libelle sens inconnu (1.215)
* TORREGROSSA	06 Fev 1996 : Ajout XDC_TOUS_TYP_FMC (1.216)
* TORREGROSSA	08 Fev 1996 : Ajout XDC_BASE_EXP, XDC_BASE_HIS (1.217)
* NOEL		08 Fev 1996 : Ajout de XDC_VISIBILITE_INCONNUE pour FMC meteo (DEM 828) (V1.218)
* TORREGROSSA	13 Fev 1996 : Ajout XDC_REV_DSP_EQT, XDC_REV_DSP_EQT_SUITE (1.219)
* NIEPCERON     19 Fev 1996 : Ajout de XDC_PATHTRAV  XDC_PATHTRAVFIC XDC_PATHTRAVFIC (1.220)
* NOEL		19 Mar 1996 : Suppression des commentaires d'exclusion applix autour 
*				de la diclaration des nom des machines			(1.221)
* TORREGROSSA	18 Jui 1996 : Ajout XDC_ETAT_CLOS_PURGE (1.222)
* VERDIER	22 Jul 1996 : Ajout XDC_AST_DEP_... (1.223) 
* VERDIER       30 Jul 1996 : Ajout des types de libelles (1.224) (DEM76)
* VERDIER	07 Aou 1996 : Ajout des numero appel par district (1.225)
* VATHELOT	12 Aou 1996 : Ajout TYP_ACTION (1.227)
* VERDIER 	16 Aou 1996 : Ajout Numero type de libelles (1.228) (ANA32)
* VATHELOT	21 Aou 1996 : Ajout TYP_RESEAU_TRANSVERSAL (1.229)
* NIEPCERON	22 Aou 1996 : Ajout de XDC_NOM_SITE_VE,XDC_NOM_SITE_PR  XDC_NOM_DISTRICT_DC 
*				XDC_NOM_DISTRICT_DS XDC_SQL_DC XDC_SQL_DS (1.231)
* NIEPCERON	22 Aou 1996 : Ajout de XDC_NOM_ACTIF_DC,XDC_NOM_ACTIF_DS   (1.232)
* Guilhou	23 aut 1996 : declaration des types de machines pour Applix (RADT) (1.233)
				+ XDC_NOM_SITE_DY DA DP
* NIEPCERON     28 Aou 1996 : Ajout de XDC_TRAIT_BLOC (RADT) (1.234)
* Noel		29 Aou 1996 : Ajout de XDC_PMV_NON_EXIST (1.235)
*				rendu visible les XDC_PATHxxx Ã  Applix
* Guilhou	02 sep 1996 : Ajout de XDC_FAM_PC_SIMPLIFIE (1.236)
* Guilhou	04 sep 1996 : modif nom TYPE_FMC devient TYPE_FICHE_MC (1.237)
* NIEPCERON     05 sep 1996 : Ajout de XDC_FORMAT_LOCALISATIOn (1.238)
* Guilhou	05 sep 1996 : correction de XDC_FORMAT_LOCALISATION (1.239)
* Guilhou	19 sep 1996 : ajout de XDC_PODC1 et XDC_PODS1 (RADT) (1.240)
* TORREGROSSA	23 Sep 1996 : Ajout XDC_REV_PAS_ENR (1.241)
* TORREGROSSA	27 Sep 1996 : Ajout XDC_INFO_NON_VALUEE (1.242)
* MISMER	03 Oct 1996 : Ajout XDC_ pour pesage RADT (DEM/1227) (1.242bis)
* Guilhou	10 oct 1996 : ajout XDC_DATE_CLOTURE_PURGE (FMC/6) (1.243)
* NIEPCERON     11 oct 1996 : Ajout de XDC_LIB_MOTIF_APPEL (DEM/1235) (1.244)
* NIEPCERON     25 oct 1996 : Ajout XDC_PATH_NTFM XDC_SANS_NTFM XDC_SANS_NTFMAX (DEM/1248) (1.245)
* TORREGROSSA	13 Nov 1996 : Ajout XDC_VOIE_BAU_ETROITE (1.246)
* Guilhou	21 nov 1996 : Ajout XDC_APPEL_RADIO	1.247
* TORREGROSSA	28 Nov 1996 : Modif valeur de TYPE_FMC et renommage en XDC_TYPE_FMC (1.248)
* Guilhou	28 nov 1996 : ajout XDC_AST_SIS (DEM/1306) 1.249
* MISMER	12 Dec 1996 : Modif libelle equipement generique LCR (DEM/1315) (1.250)
* Niepceron	19 Dec 1996 : Ajout de la famille XDC_FAM_HISTO_PEAGE (dem/1359) (1.251)
* Niepceron     19 Dec 1996 : Mise Ã  30 du nb de familles (et non 28) (dem/1359) (1.252)
* Guilhou	26 dec 1996 : ajout de XDC_LIB_TYPE_VOIE (lib. predefinis) (DEM/1372) 1.253
* Guilhou	26 dec 1996 : ajout des XDC_CODE_VOIE_xxx (DEM/1372) 1.254
* Guilhou	04 fev 1997 : ajout des XDC_SEUIL_TDP_xxx (temps de parcours) 1.255
* VATHELOT	27 fev 1997 : ajout des XDC_LINEAIRE_xxxx (DEM/1397 : fiche metier 21) 1.256 
* Mismer	03 Mar 1997 : ajout des XDC_PMV_xxx (temps de parcours)(DEM/1396) 1.257
* VATHELOT	04 Mar 1997 : ajout des XDC_CONDITION_xxxx (DEM/1397 : fiche metier 22) 1.258
* Niepceron     26 Mar 1997 : ajout des XDC_LIB_POPAN1 (dem/1359) 1.259
* VATHELOT      02 Avr 1997 : ajout des types de periode (DEM/1397 : fiche metier 09) 1.260
* 				+ ajout des types de mesures 
* Guilhou     	07 Avr 1997 : RE-Mise Ã  30 du nb de familles (et non 28) (dem/1359) (1.261)
* Mismer	23 Avr 1997 : ajout des XDC_EQT gabarit,vitess,niche pour EQT6 (DEM/1421) 1.262
* Guilhou	05 mai 1997 : ajout XDC_PATH_STRADA 1.263
* Vathelot	26 mai 1997 : ajout XDC_CONDITION_OUVRAGE_PEAGE (DEM/1397 : fiche metier 22) 1.264
* TORREGROSSA	10 Dec 1997 : Ajout XDC_TYPE_PCNIVEAU2, XDC_TYPE_PCNIVEAU1 1.265
* Niepceron	16 Dec 1997 : Ajout XDC_TYPEM_PC2 et XDC_TYPEM_PC2LT 1.266 (DEM/1533)
* Niepceron     22 Dec 1997 : Ajout XDC_TYPEM_STA (Poste analyse) 1.267
* Guilhou	07 jan 1998 : ajout des PC niveau 2 (dem/1536) 1.268
* Guilhou	13 jan 1998 : ajout de XDC_FAM_EXPLOITATION_PC2 (dem/1533) 1.269
* Guilhou	13 jan 1998 : ajout des noms des postes operateurs des nouveaux PC (dem/1533) 1.270
* TORREGROSSA	16 Jan 1998 : Ajout XDC_EQT_FAC, XDC_EQT_FAU, XDC_EQT_PAL, XDC_ACT_PAL, XDC_ACT_FAC, XDC_ACT_FAU (1538) 1.271
* TORREGROSSA	21 Jan 1998 : Ajout XDC_MAI_PAL (1545) 1.272
* NIEPCRON	29 Jan 1998 : Ajout XDC_MACRO_MTPP (1537) 1.272
* MISMER	29 Jan 1998 : Ajout XDC_GN_DONNE_RAU et XDC_GN_PREND_RAU (1542) 1.273
* TORREGROSSA	24 Fev 1998 : Modif XDC_ACT_NB_TYP (1538) V1.274
* Guilhou	25 fev 1998 : ajout XDC_FAM_ACHEVER_PC2 (1578) 1.275
* NIEPCRON      04 Jun 1998 : Ajout du site SQL_HIS 1.276
				Ajout de XDC_SERVEUR_HIS 1.276
				Ajout de XDC_TYPEM_HIS 1.276
* Niepceron     19 Nov 1998 : Ajout de XDC_ACT_EXP,XDC_ACT_EXPTFM, XDC_ACT_PMVDAT 1.277 dem/1723
				Ajout XDC_LIB_TYPEM_DATEX dem/1724
* ESCOTA        25 Mar 1999 : Ajout du libelle pre defini 26 (champ origine)                       
* ESCOTA        29 Mar 1999 : changement du libelle pre defini en 31 (champ origine) 1.279                       
* NIEPCRON      04 Mai 1999 : Ajout XDC_SYN_... 1.280
* Guilhou	16 jan 2001 : ajout constantes passerelle WEB 1.281 (XDC_WEB_...)
* Moulin	23 avr 2002 : ajout constantes liees a la nouvelle GTC 1.282
* CLUCHAGUE     04 jul 2002 : on vire l'accent circonflexe ligne 515
* CLUCHAGUE     26 jul 2002 : le type pompier est 44 et non 22 V1.284 (REPERE1)
* MOULIN     	15 jan 2003 : Diminution du temps de synchronisation de tenic 
* MISMER     	27 avr 2004 :  Modif constant XDC_Lon_Nb_Mod_Pal    1.286
* CLUCHAGUE  	24 dec 2004 :  Modif constant XDC_Lon_Nb_Mod_Nav 1.287
* Niepceron     28 Fev 2005 : Ajout de XDC_AST_SIS2 XDC_AST_SISR v1.288 DEM431
* CHI           13/04/05        : Ajout sous type meteo "Grele sur chaussee" et "Chaussee mouillee" 1.289 DEM456
* F. Lizot      02 Mar 2004 : Ajout des constantes CLE_SAGA,XDC_ACT_DATEX,XDC_ACT_LIB_DATEX,XDC_DDE83 (SAGA) V1.290
* JMG		13/09/05 : ajout constantes PMVA BAF BAD 1.291
* JMG		28/09/05 : ajout constantes PMVA BAF BAD 1.293
* PNI		06/04/06 : Ajout de XDC_Lon_CodeCam 5 1.294 DEM565
* PCL		12/01/07 : Ajout de XDC_POINT_CARACT_BIFURCATION et XDC_RES_BIFURCATION 1.294
* JMG		27/03/07 : ajout de XDC_DISTANCE_TDP 1.295
* JPL		06/04/07 : ajout de XDC_TYPE_LIB_OBSTACLE, XDC_TYPE_LIB_PIETON, XDC_TYPE_LIB_AUTRE_EVT 1.296
*		              + correction xdc_ax.h 1.110: valeurs XDC_Lon_Nb_Mod_BAF et XDC_Lon_Nb_Mod_BAD
* JPL		13/04/07 : ajout de XDC_LIB_2_SENS 1.297
* JPL		23/04/07 : ajout de XDC_SYN_FMC_xxx pour Vehicule en Marche Arriere, Traitement chaussees,
*                             Ech. deconseille, Chaussee Glissante, Basculement pour Travaux (DEM 643-647) 1.298
* JPL		26/04/07 : Ajout de XDC_TYPE_LIB_TRAIT_CHAUSSEE, XDC_TYPE_LIB_CHAUSSEE_GLISS 1.299
* CLAUDEL       09/07/07 : Suppression DY 1.300
* JMG		12/12/07 : Modif lib action fax pour mail DEM718 1.301
* AAZ       30/08/2007 : DEM 662 Ajout de 
*               XDC_EQT_SIG, XDC_LIB_SIG, XDC_MAI_SIG, XDC_MAI_EQT_SIG, XDC_ACT_GTC_SIG, XDC_ACT_LIB_GTC_SIG
*               XDC_EQT_ECL, XDC_LIB_ECL, XDC_MAI_ECL, XDC_MAI_EQT_ECL, XDC_ACT_GTC_ECL, XDC_ACT_LIB_GTC_ECL
*               XDC_EQT_VEN, XDC_LIB_VEN, XDC_MAI_VEN, XDC_MAI_EQT_VEN, XDC_ACT_GTC_VEN, XDC_ACT_LIB_GTC_VEN
*               XDC_EQT_ENR, XDC_LIB_ENR, XDC_MAI_ENR, XDC_MAI_EQT_ENR, XDC_ACT_GTC_ENR, XDC_ACT_LIB_GTC_ENR
*               XDC_EQT_INC, XDC_LIB_INC, XDC_MAI_INC, XDC_MAI_EQT_INC, XDC_ACT_GTC_INC, XDC_ACT_LIB_GTC_INC
* [JPL 29/05/09] GTIE 2007-2008 Ajout constantes de defaut equipement XDC_EQP_xxx (!!! dans xdc_ax seul !!!)
* JBL		Mai 2008 : DEM 662 ajout XDC_ACT_GTC_SIG_ECH 1.303
* GTIE		Juin 2008 : changement de valeur de XDC_SIG_SEQ_HORS_SEQUENCE 1.304
* JBL           15 Oct 2008 : ajout XDC_DEBUT_TYPEQT_GTCRENOVEE
* JPL		19/08/08 : ajout de XDC_REV_URGENT (DEM 721) 1.305
* JPL		20/01/09 : ajout de XDC_REV_FMC_RESERVEE (DEM 848) 1.308
* JPL		29/05/09 : Remise en coherence des versions xdc et xdc_ax 1.309
* JPL		02/06/09 : Definition de constantes de cas d'appel des fonctions et procedures stockees 1.310
* JMG		28/90/09 : secto DEM887 1.311
* JPL		22/10/09 : Ajout constantes de modes sync/async et id. ecrans texte/synoptique 1.312
* JPL		02/11/09 : Suppression de definitions dupliquees 1.313
* JPL		02/11/09 : Ajout constantes generales d'actions / resultats pour rafraichissement IHM 1.314
* JPL		19/11/09 : Ajout constante numero d'ecran unique (pour DEM 919) 1.315
* PNI		15/12/09 : Ajout de XDC_ALT_TEST DEM923
* JPL		19/08/10 : Ajout XDC_SYN_FMC_ZONE_STOCKAGE (DEM 942); ajout XDC_POINT_CARACT_AUCUN 1.317
* JPL		15/10/10 : Ajout XDC_Lon_AdresseIP  (DEM 954)  1.318
* JMG		22/11/10 : ajout constantes radio/tel 1.319
* JPL		14/12/10 : Ajout XDC_Lon_Protocole et XDC_Lon_Encoder  (DEM 954)  1.320
* JMG		02/03/11: ajout XDC_SYN_IPHONE	1.321
* JPL		16/03/11 : Ajout cttes. mesures RADT; suppr. cttes. specifiques fiche metier (DEM 977)  1.322
* PNI		06/04/11 : : Ajout cttes. mesures TDP specifiques fiche metier (DEM977 ) 1.323
* LCL         19/07/11 : Ajout des constantes "rappel client" XDC_AST_CLIENT et XDC_LIB_AST_CLIENT 1.324 DEM 1000
* JPL		26/09/11 : Ajout de constantes de types de points caracteristiques pour localisation generale  1.325
* JPL		03/11/11 : Ajout de constantes de types de conditions d'accident pour EVEREST  (DEM 1006) 1.326
* JPL		06/01/12 : Changement des libelles "Trafic FM" en "Radio Vinci Autoroutes" (DEM 1007)  1.327
* VR            04/11/11 : Ajout destination 1.328 (DEM/1014)
* JPL		01/02/12 : Ajout constante destinee a identifier les bifurcations (DEM 1011)  1.329
* JPL		08/03/12 : Ajout constantes de point caracteristique 'Tunnel' (DEM 1022,1023)  1.330
* SDU           10/02/12 : DEM 1015 -Communautaire
* VR		29/05/12 : DEM 1016 PRV
* JPL		18/06/12 : Ajout constante de longueur de numero (textuel) de sortie (DEM 1033)  1.333
* JPL		27/06/12 : Ajout constante de type de categorie de (victime de) hold-up (DEM 1034)  1.334
* JPL		22/08/12 : Renommage de constantes de libelles predefinis  1.335
* VR		08/02/12 : AJout TDP PMVA (DEM/1014 PMA)
* JPL		19/10/12 : Declaration symboles AVANT ceux qui en dependent pour Java (XDC_ACT_TEL)  1.337
* JPL		19/10/12 : Ajout action d'envoi document VH et libelle associe (DEM 1032)  1.338
* JPL		21/11/12 : Ajout type de libelle predefini XDC_LIB_TYPE_CCH (DEM 1032)  1.339
* JMg		12/12/12 : ajout XDC_SYN_FMC_REGULATION_VITESSE 1.340
* JPL		14/01/13 : Renommage de constantes de libelles predefinis  1.341
* JPL		13/02/13 : Ajout constante XDC_DTP_AUCUNE (DEM 1061)  1.342
* JPL		08/03/13 : Ajout constante XDC_OBJ_AUCUN ; suppression conflit XDC_ACT_REFUSEE  1.343
* PNI		30/05/13 : Ajout constantes supprimees types de libelles 1.344 DEM 1070 
* ABE-GTIE	27/05/13 : mise Ã  jour de la gestion du retour Ã  la normale avec les modif : #define XDC_ECL_SEQ_RETOUR_NORMAL 5 -> 20 ET #define XDC_VEN_SEQ_RETOUR_NORMAL 8 -> 46
* JPL		15/10/13 : Ajout sequences Incendie, Desenfumage et 'Plein soleil'; ajout XDC_ACCES_xxx (pour DEM 1083)  1.346
* JPL		01/04/14 : Ajout sequences de ventilation (pour DEM 1089)  1.347
* PNI		03/12/14 : Modif lib action fax --> Mail DEM1115 1.348
* PNI		15/04/15 : Remplacement SIS par Surveillance RÃ©seau DEM1126 1.349
* NDE		27/07/15 : Ajout constantes XDC_TMA (DEM1140) 1.350
* NDE		29/07/15 : Ajout constante XDC_PMV_3POINTS (DEM1135) 1.350
* PNI		25/03/16 : Ajout des constantes pour les regions DRE 1.351
* JMG		10/04/16 : ajout XDC_EQT_SAGA 1.352
* JPL		24/05/16 : Ajout constante de chemin d'acces au repertoire DEQ (pour DEM 1165)  1.353
* JMG		06/06/16 : ajout XDC_EQT_SAGA suite 1.354
* LCL		28/02/17 : REGIONALISATION 1.355
* JPL		18/07/17 : Ajustement constante de taille des portions de texte en base (XDC_MAX_CHAR_LEN)  1.356
* JPL		19/07/17 : Ajout definition des postes Serveur de synoptique et NOMADE (pour DEM 1229)  1.357
* JPL		21/07/17 : Ajout constantes de mode d'affichage des PMV et PMVA (clignotement, alternat) (DEM 1230)  1.358
* JMG		05/12/17 : correction XDC_MACRO_SAGA_BRA 1.359
* JMG		12/12/17 : envoi SMS 1.306
* JMG		12/01/18 : ajout XDC_EQT_CAB GABEB 1.361
* LCL		23/01/18 : ajout XDC_PREFIXE_ALERTE_POLYVOX 1.362
* LCL		21/02/18 : ajout constantes CFE CTRL_FEUX 1.363 DEM1284
*			  ajout XDC_PATH_DRA 
* LCL		11/05/18 : Modification _CAB en _CAP GABEB DEM1283 1.364
* JPL		22/06/18 : CaractÃ¨res minuscules accentuÃ©s en ISO Latin (DEM 1274)  1.365
* JPL		28/06/18 : Ajout constante XDC_TFM_MSG_MODIFIE (DEM 1293)  1.366
* JPL		13/07/18 : Ajout sequence GTC SIG 'Fermeture tunnel' (DEM 1294)  1.367
* JMG		08/08/18 : ligne PMV passe a 18 car. suppression SAGA DEM1306 1.368
* RG		11/10/18 : Ajout constante XDC_PRV_BPV (DEM 1307) 1.369
* JPL		27/11/18 : Ajout constantes types de jour pour calendrier et type de libellÃ© prÃ©dÃ©fini (DEM 1312)  1.370
* JPL		28/03/19 : Ajout constantes XDC_LOCAL et XDC_NON_LOCAL  1.371
* JPL		24/06/19 : Ajout constantes d'Ã©tat des FMC Ã  traiter (configurÃ©es et prÃ©sentÃ©es sur postes opÃ©rateurs) (DEM 1325)  1.372
* JPL		04/09/19 : Ajout constante de type de libelle predefini : Nature d'incendie (DEM 1352)  1.373
* JMG		10/12/19 : Ajout constante XDC_REV_TDP_SURCHARGE DEM1365 1.374
* LCL		18/11/19 : Ajout constant prefixe nom serveur SYBASE SQL_ MOVIS DEMXXXX 1.375	
* JMG		01/01/20 : ajout IMU 1.376
* ABE		15/07/20 : Ajout path ADA pour tada2.x DEM130 1.376
* CGR           27/07/20 : Ajout XDC_MAI_IMU DEM-SAE155 1.377
* ABE		03/12/20 : Ajout des seuils de validation TDP_FCD, XDC_SEUIL_AFFICHAGE_TDP_FCD et XDC_SEUIL_AFFICHAGE_TDP_FCD_ABSENCE_TDP_ESCOTA DEM-SAE152 1.379
* CGR		04/12/20 : Ajout XDC_Lon_CommandeIMU DEM-SAE155 1.379
* ABE		15/12/20 : Modif XDC_Lon_commandeIMU pour DXC_Lon_ChaineIMU, ajout XDC_ACT_IMU, XDC_ACT_LIB, XDC_MACRO_MTIMU DEM-SAE155 1.379
* ABE		02/04/21 : Ajout PAU_SONO DEM-SAE244 1.380
* ABE		18/11/21 : Ajout enum categorie action pour filtrage dans la synthÃ¨se magistra DEM-SAE311 1.381	
* LCL		20/12/21 : MOVIS P2 ems SAE-312 / Mix MAGISTRA
* CGR		02/03/22 : Ajout XDC_ALT_VRAIE_AVA qualification d'alerte AVA DEM-SAE367 1.382
* PNI      18/07/2022 : modification du seuil de prise en compte des donnees TDP FCD SAE-394
* ABE		02/08/2022 : Ajout NIC et IS pour localisation tunnel DEM-SAE403
------------------------------------------------------------------------------------------------------ */


/******************************************************************************
**                                                                           **
** Ce fichier definit des constantes communes a tous les sous-systemes,      **
** y compris le sous-systeme IHM developpe avec Applix.                      **
**                                                                           **
** Des commentaires de la forme "Non Applix - debut" et "Non Applix - fin"   **
** delimitent par la suite les zones dont la syntaxe n'est pas reconnue par  **
** l'interpreteur Applix de maniere a pouvoir les filtrer simplement.        **
**                                                                           **
** Une section en fin de fichier contient les constantes qui au contraire    **
** ont ete definies specifiquement pour l'IHM, et dont les valeurs choisies  **
** ne sont pas a ce jour en parfaite coherence avec les autres (a revoir ?). **
**                                                                           **
** Le fichier 'xdc.h' CONSTITUE L'UNIQUE SOURCE DE CES DEFINITIONS,          **
** la version 'xdc_ax.h' destinee a Applix DEVANT ETRE REGENEREE             **
** AU MOYEN DE L'OUTIL ADEQUAT LORS DE CHAQUE MODIFICATION.                  **
**                                                                           **
******************************************************************************/


/* Non Applix - debut */




/* Non Applix - fin */


/* definitions des constantes  */

/* caracteristiques du RTServer */



/* booleens */
/* chaine vide ou nulle pour sybase (ne pas changer la valeur) */
/* champ numerique non value */
/* Texte marqueur de negation */
/* Texte separateur d'elements de liste */
/* definition des cas d'appel des fonctions et procedures stockees */
/**********  Definition des codes de retour des fonctions et procedures stockees  **********/
/*!!! *****  ILS DOIVENT ETRE DISTINCTS DES CODES DES AUTRES MODULES (XZAE  ...)  ***** !!!*/
/*!!! **********  LES CODES A PARTIR DE 6 SONT UTILISES PAR LE MODULE XZAE  ********** !!!*/
/**********  Fin de definition des codes de retour des fonctions et procedures  **********/
/* Categories des actions sur les fmc pour la synthhse magistra */
/* Etats equipements */
     /* dans une colonnesuite a un select*/
     /* effectue par une SP*/
/* nombre de jours d'offset pour la date de purge */
/* definition des constantes de la base test et config */
/* interface avec sybase : 1er janvier 1900 = null */
/* definition des modes d'execution des fonctions */
/* identification des ecrans de visualisation main courante et synoptique */
/* definition des types sur reseau */
/* definition des types d'equipement */
/*! * ATTENTION valeur utilisee par un autre "define" */
/* AAZ 18/06/2007 */
/* XPH : ajout des sequences speciales par domaine fonctionnel */
/* numerotation des moniteurs de PC district et CI */
/* sous types de capteur tunnel */
/* Definition du maitre et de l'exclave */
/* definition des maitres des types d'equipement */
/* AAZ 30/08/2007 */
/* scenario de rentrer de la NAV en mode automatique */
/* sequence pour eteindre affichage tunnel et echangeur */
/* differents types de point caracteristique */
/* position pazr defaut sur echangeur */
/* PR de la frontiere*/
/* diffÃ©rents etats d'actions */
/* diffÃ©rents etats d'alerte (ne pas modifier, utilise pour replication) */
/* differents etats d'un equipement au niveau de la base     */
/* et changement d'etat                                      */
/* (valeur correspond a un positionnement de bit pour l'etat */
/*  global de l'equipement et le changement global d'etat)   */
/* differents types d'action */
/* AAZ */
/* AAZ */
/*libelles de ces actions*/
/* AAZ */
/* AAZ */
/* nombre de type d'actions */
/* type d'intervenant */
/* attributs des messages TFM */
/* Niveaux de messages TFM */
/* sens de circulation */
/* Denomination alternative des sens de circulation */
/* Types de sens de localisation (lies aux sens ci-dessus) */
/* localisation non connue (pour le sens cf ci dessus) */
/* sens des PR */
/* Format de la loacalisation */
/* Visibilite inconnue */
/* code d'erreur du a une verification de localisation */
/* distance de l'evenement */
/* Etats d'une fiche main courante */
/* Etats d'une fiche main courante A TRAITER */
/* criteres de recherche d'un evenement sur son type operation ou evenement */
/* criteres de recherche d'un evenement sur son type de fmc */
/* numero du bit correspond aux differents districts et CI utilises */
/* pour la creation de criteres de recherche */
/* categorie des elements contenus dans l'historique d'exploitation */
/* differents types de voie pour les synoptiques */
/* textes des reveils */
/* messages d'information transmis au moyen de reveils mais non affiches */
/* valeurs a utiliser pour indiquer la non valuation d'une mesure RADT */
/* valeurs pour le niveau de trafic */
/* valeurs de la validite de Q, V, TO, PL pour les mesures de trafic */
/* type de courbe pour le trafic */
/* Nb de classe par type de pesage */
/* type de fenetre pour affichage des courbe */
/* type de courbe Q V TO */
/* type de courbe trafic */
/* noms des serveurs */
/* noms des sites */
/* numeros des sites des serveurs */
/* Numero de code des numero d'appel par district */
/* noms des districts */
/* Valeurs generales d'actions ou de resultats d'actions utilisees notamment pour
   configuration des bases de donnees, rafraichissement des IHM synoptique et textuelle ... */
/* scenario et sequences de fin pour tunnel, echangeur, NAv */
/* Commandes d'etats d'un equipement */
/* Constantes d'abonnement aux donnees EQUEXT */
/* noms des repertoires */
/*#define XDC_PATH_IMAGE       	     XDC_PATH_MIGRAZURHOME "/config/fichiers/adm"*/
/* PATH pour le fichier de bilan de configuration SOUS-SYS GECONF */
/*#define XDC_FICHIER_ASTREINTES_BC   " " */
/* Repertoire Temporaire pour le fichier d'Administartion */
/* Repertoire executable RTWorks */
/* Combiner les valeurs par des additions et non des "ou logiques" pour Applix */
/********** Taille des donnees *****************/
/******************************************************************************
**                             Non Applix - debut                            **
******************************************************************************/
/********** Longueur des tableaux de caracteres *****************/
/******************************************************************************
**                              Non Applix - fin                             **
******************************************************************************/
/*#define XDC_Lon_Nb_Mod_Nav	22*/
/******  Constantes PMV *****************************************/
/******  Constantes PICTO *****************************************/
/******  Constantes de mode d'affichage PMV (EXCLUSIFS) ***********/
/******  Constantes Video *****************************************/
/* defs. type camera */
/******  Constantes Contraste *************************************/
/******  Constantes GTC NICE **************************************/
/* AAZ 25/07/2007 */
/****** Definition des niveaux de traces en fonction de la severite ****/
/****** d erreur rendu par le sql Serveur ou l Open Client          ****/
/******  Constantes Temps de Parcours et Destinations *********************/
/* type de franchissement de seuil pour temps de parcours*/
/*distance min entre PMV et destination*/
/*distance min entre PMA et destination*/
/*type de zone de parcours*/
/*definition des familles*/
/*constantes REGULATION*/
/*definition des PARAM REG*/
/******  Constantes Contexte ********************************************/
/********** Constantes indiquant basculement de debut ou de fin de travaux 
            XDY_booleen                             *************************/
/********** Constantes de Type d'objet Mobile ********************/
/* Noms des machines et leurs libelles */
/* Declaration des machines hors SAE */
/*types de machine*/
/*libelles des types de machine*/
/*types de pc pour applix */
/* Liste des Macros applix appelee */
/* Types de Bip */
/* Types d'astreintes */ /* ATTENTION DE PAS MODIFIER, l'IHM s'en sert ....*/
/* REPERE1*/
/* Constante alarme */
/* Constante Voies RADT */
/* Constante contenant les noms des sous-systemes */
/* Constante contenant les noms des carburants */
/* Constantes utilisees pour conversation telephonique ou OPERATOR */
/* commandes */
/*
#define XDC_MODE_ECHO			4
#define XDC_CODE_RESULT			5
#define XDC_FORMAT_CODE			6
#define XDC_ATTENTE_DECROCHE		7
*/
/* identifiants operateurs autorises
#define XDC_POSTE_OP1			1
#define XDC_POSTE_OP2			2
*/
/* compte-rendu */
/*#define XDC_ERR_POSTE_OP		1*/
/* Constantes de comptes-rendus de commandes RAU */
/* Constantes d'etats RAU */
/* Constantes pour etat RAU avec la GN */
/* Constantes des Mini GTC */
/* Constantes de la video */
/* Constante contenant le nom TOR de la Cellule JN */
/* delai de reconnection rtserver */
/* definition des types de libelles */
/* Une modification d'un de ces define entraine la modification */
/* des vues utilisant ces define */
/* par ex le changement du define VEH_categorie */
/* entraine la modif du fichier fmc_veh.vue */
/* #define XDC_TYPE_COND_OBSTACLE		41	ANNULE */
/* #define XDC_TYPE_COND_ANIMAL			42	ANNULE */
/* VATHELOT : Ajout des constantes : types de lineaire pour fiche metier 21 */
/* VATHELOT : Ajout des constantes : types de condition accidentogene pour fiche metier 22 */
/* VATHELOT : Ajout des constantes : conditions accidentogenes chaussee pour fiche metier 22 */
/* VATHELOT : Ajout des constantes : condition accidentogene ouvrage pour fiche metier 22 */
/* DEM 1397 VATHELOT: Ajout des types de periode pour fiche metier 09 */
/*constantes WEB*/
/******  Constantes de mode d'affichage PMVA (EXCLUSIFS) **********/
/******  Constantes PRV *****************************************/
/* Communautaire */
/* Liste Messages RVA */
/* Types de jour pour calendrier */
/******************************************************************************
**                                                                           **
**  DEFINITIONS NE CONCERNANT PAS L'IHM                                      **
**   (A INCLURE UNIQUEMENT DANS xdc.h)                                       **
**                                                                           **
******************************************************************************/
/* Non Applix - debut */
/* Nombre d'erreurs RTworks pour un RTClient avant de sortir de la Tache */
 /* Declaration des variables definissant les serveurs actifs
	*	et passifs sur les differents sites */
/* Cle utilisee pour creer une FMC provenant de la DDE 83 (SAGA) */
/* Pour compatibilite seulement !!! */
/* Non Applix - fin */
/* Const seuils de basculement TDP ESCOTA / TDP_FCD */
/* Const pour recherche si proche niche et issue de secour */
/******************************************************************************
**                                                                           **
**     DEFINITIONS POUR L'IHM SEULEMENT                                      **
**   (A INCLURE UNIQUEMENT DANS xdc_ax.h)                                    **
**                                                                           **
******************************************************************************/
/* Non Applix - debut */
/* Non Applix - fin */
/* fin de fichier */
/*E*/
/* Fichier : $Id: xzaec.h,v 1.85 2021/05/03 14:05:18 pc2dpdy Exp $        $Revision: 1.85 $        $Date: 2021/05/03 14:05:18 $
------------------------------------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------------------------------------
* SOUS-SYSTEME XDMICG
------------------------------------------------------------------------------------
* MODULE XZAE * FICHIER xzaec.h
------------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante et de leur complement
*
------------------------------------------------------------------------------------
* HISTORIQUE :
* C.T.  10/10/94	: Creation a partir du fichier
*                         xzae.h situe ds BASEDD/inc
* I.T.	10/10/94	: Suppression #endif
* C.T.	10/10/94	: Ajout commentaire pour synthese
* C.T.	17/10/94	: Ajout commentaire pour synthese
* C.T.	20/10/94	: Ajout code d'erreur de localisation
* C.T.	24/11/94	: Ajout code d'erreur de plan de secours (V 1.6)
* B.G.	21/12/94	: Ajout point pour espace synthèse (1.7)
* B.G.	27/12/94	: Modif reveils (1.8)
* C.T.	14/02/95	: Ajout reveils (1.9)
* C.T.	16/02/95	: Ajout commentaires pour synthese (1.10)
* C.T.	22/02/95	: Ajout commentaires pour synthese (1.11)
* C.T.	18/07/95	: Ajout commentaires pour vue metier (1.12)
*                         Ajout commentaires pour synthese 
* C.T.	16/11/95	: Ajout commentaires pour synthese (1.13)
* C.T.	09/01/96	: Modif libelle synthese PR balisage (1.14)
* C.T.	06/02/96	: Modif accent des libelles synthese  (1.16)
* C.T.	15/03/96	: Modif libelle travaux discontinus pour synthese travaux (1.17)
* C.T.	01/10/96	: Ajout XZAEC_FMC_PAS_RETYPAGE,  XZAEC_FMC_INC_RETYPAGE (FMC3)(1.18)
*                         XZAEC_FMC_LRG_VEH, XZAEC_FMC_HAU_VEH, XZAEC_FMC_CNT_VEH, XZAEC_FMC_POI_VEH
* C.T.  10/11/96        : Ajout XZAEC_SYN_FAUSSE_ALERTE (1282)
* C.T.  14/11/96        : Ajout XZAEC_SEUIL_BAU_ETROITE (1283)
* C.T.  11/12/96        : Ajout XZAEC_SYN_LON_ESC XZAEC_BOU_Escargot (FMC7)
* JMG	27/12/96	: Ajout XZAEC_SYN_CFG_VOIES_BAS (IHM15) (DEM/1372) 1.24
* JMG	03/06/97	: pas de perception peage devient sans perception peage (DEM/1435) 1.25
* C.T.  18/12/96        : Ajout XZAEC_FMC_INIT_... (1531)
* C.T.  16/01/98        : Ajout XZAEC_SYN_ACT_PAL..., XZAEC_SYN_ACT_FAC, XZAEC_SYN_ACT_FAU (1.27)
* JMG	22/01/98	: accent de succes (dem/1547) 1.29
* C.T.  29/01/98        : Ajout XZAEC_SYN_FMC_DEGRADE (1.30)
* C.T.  03/03/98        : Ajout XZAEC_FMC_INIT_NON_AUTORISEE (1583) (1.31)
* C.T.  03/03/98        : Ajout XZAEC_SYN_ACT_PMV_BAND (1607) (1.31)
* C.T.  03/03/98        : Ajout XZAEC_FMC_CLOSE_PAS_ENR (1612) (1.32)
* C.P	18/05/98	: Correction accent (dem 1358 et 1615) (1.33)
* ???	14/10/98	: Ajout XZAEC_SYN_ACT_ANNULEE dem/1500 1.34
* P.N	12/01/99	: Modifs intergestionnaire dem/1724 (1.35)
* P.C   18/03/03        : Ajout types de travaux (1.36)
* P.N	28/02/05	: Ajout type de bouchon "En accordeon" 1.37 DEM434
* CHI	13/04/05	: Ajout sous type meteo "Grele sur chaussee" et "Chaussee mouillee" 1.38 DEM456
* CHI   18/07/05        : Ajout type XZAEC_ClasseNature + XZAEC_SYN_NATURE_OBSTACLE  v1.39 DEM487
* JMG	13/09/05	: ajout PMVA BAF BAD 1.40
* JMG	02/10/05	: ajout modules BAF et BAD 1.41
* PNI	17/10/05	: correction bléssés --> blessés v1.42 DEM529
* JMG	10/11/05	: correction XZAEC_SYN_ACT_PMVA 1.43
* JPL	11/04/07	: Libelle XZAEC_SYN_NATURE_OBS plus general (nature evt.) 1.44
* JPL	17/04/07	: Ajout types FMC Marche Arriere, Traitement Chaussees, Echangeur deconseille,
*                            Chaussee glissante, Basculement pour Travaux (DEM 643-647) 1.45
* JPL	10/05/07	: Ajout des (sous-)types de traitement chaussees et de chaussee glissante (DEM 644,646) 1.46
* JPL	15/05/07	: Suppression de doublons 1.47
* JPL	15/05/07	: Ajout de constantes de libellés pour fmc Echangeur deconseille (DEM 645) 1.48
* JPL	13/06/07	: Ajout libellé pour configuration des voies de Basculement pour travaux (DEM 647) 1.49
* JPL	10/07/07	: Modif libellé pour configuration des voies de Basculement pour travaux (DEM GO) 1.50
* JPL	06/11/07	: Ajout code erreur d'absence donnees fmc specifiques (pour DEM 706 mais general) 1.51
* AAZ   27/08/2007  : Ajout Domaine DEM 662 1.52
* JBL   Mai 2008	: DEM 662 ajout GTC_ECH 1.53
* JPL	28/11/08	: Ajout de constantes de droits d'enrichissement (lie a DEM 833) 1.54
* JPL	19/05/09	: Ajout de constantes d'erreurs pour la generation de la synthese d'un evenement 1.55
* JMG	28/9/09		: secto DEM887 1.56
* JPL	23/07/10	: Ajout Fmc Zone stockage et libelles synthese (Pr debut, en cours, ...) (DEM 942) 1.57
* JMG	25/11/10	: ajout horodate feu maitrise FMC_VEH 1.58
* JPL	09/02/11	: Ajout du type de travaux "Tunnels"; suppression libelles lus en base (DEM 951)  1.59
* JMG	15/02/11	: ajout appel radio pour synthese 1.60
* LCL	18/07/11	: ajout rappel client 1.61, 1.62 DEM 1000
* LCL	05/12/11	: ajout vitesse et vitesse opposee DEM 1015  1.63
* JPL	06/01/12	: Changement des libelles "Trafic FM" en "Message RVA" (DEM 1007)  1.64
* VR	26/01/12	: ajout regulation DEM/1016 1.65
* JPL	30/08/12	: Ajout de la classe "Incendie" et d'un libelle pour la synthese (DEM 1037)  1.66
* JPL	26/09/12	: Ajout constantes numeriques et libelles de conditions de conduite hivernale (DEM 1032)  1.67
* JPL	26/09/12	: Coherence des caracteres des libelles pour la synthese  1.68
* JPL	26/09/12	: Ajout textes d'en-tete des conditions de conduite pour Fmc Meteo (DEM 1032)  1.69
* PNI	11/12/14	: Modif lib fax->Mail des lib action dDEM1115 1.70
* PNI	15/04/15	: Remplacement SIS par Surveillance Réseau DEM1125 1.71
* NDE	20/08/15	: Nouvelles constantes pour intitules déviation, basculement travaux, etc. (DEM 1136) 1.73
* JMG	26/11/176	: ajout majeur lot 23 1.74
* JPL	12/12/16	: Libelles de niveaux de bouchon pour textes de mails (DEM 1193)  1.75
* JPL	21/03/17	: Changement du terme Secteur en Region (DEM 1173)  1.76
* JMG	12/12/17	: ajout SMS 1.77
* LCL	22/02/18	: Ajout CTRL_FEUX 1.78
*				presence personnel travaux
* JPL	22/06/18	: Caractères minuscules accentués en ISO Latin (DEM 1274)  1.79
* JMG	10/10/18	: ajout SMS LOT25 DEM1301 1.80
* JPL	04/04/19	: Ajout libellé XZAEC_SYN_ECOUL (DEM 1326)  1.81
* RGR	17/06/19	: Correction orthographe manifestants		1.82
* JPL	01/10/19	: Ajout libellé XZAEC_SYN_BOU_MOBILE (DEM 1346)  1.83
* CGR	07/12/20	: Ajout XZAEC_SYN_ACT_IMU et XZAEC_SYN_ACT_IMU_TXT DEM-SAE155 1.84
* ABE	02/04/21	: Ajout PAU_SONO DEM-SAE244 1.85
--------------------------------------------------------------------------------- */
/* fichiers inclus */
/* definitions de constantes exportees */
/*A Codes d'erreur des procedures stockees traitant les FMC */
/*A (validation, achevement, lecture, transmission des droits d'enrichir ...) */
/*! Ils completent les codes XDC generaux, et DOIVENT EN ETRE DISTINCTS */
     /* adequate pour la valider */
     /* pour la prevision (au moins +24h)*/
/*A differentes classes de fiches main courante */
/*A types de FMC */
/*A nombre total de type FMC */
      /* mettre a jour ce define */
/* pour renommer la tete de bouchon pour vues metier */
/*A nature des travaux */
/*A type de travaux */
/*A type de bouchon */
/*A type de perturbation meteo */
/*A type de chaussee glissante */
    /*B types identiques a une perturbation meteo */
    /*B autres types de chaussee glissante */
/*A conditions de conduite pour la Viabilite Hivernale */
/*A type de traitement des chaussees */
/*A type de vehicule */
/*A utiliser pour XZAE68 : pour autoriser ou non l'effacement des */
/*A resultats a la sortie de la procedure stockee                 */
/*A utiliser pour XZAE70 et XZAE68 : pour la memorisation en memoire ou */
/*A non des consequences d'un evenement                                 */
/*A nombre de digits pour le libelle fmc synthese */
/*A et action                                     */
/*A commentaires pour la synthese d'un evenement */
/* AAZ 27/08/2007 */
/* AAZ */
/* FMC signalee ou confirmee */
/* perception au peage ou non lors de manifestation */
/*A nature des travaux */
/*A indicateur de type d'heure (utiliser pour XZAE;02 et XZAE;82) */
/*A Droits d'enrichissement des FMC */
/* type de reveil a declencher pour une validation de fmc */
/* code d'erreur pour la verification d'une localisation */
/* code d'erreur pour un plan de secours inconnu pour la fiche delestage */
/* numero de vehicule non value */
/* seuil en dm pour la bau etroite */
/* types de rappel client */
/* definitions de types exportes */
/* definition de macro exportees */
/* declarations de donnees globales */
/* delaration de fonctions externes */
/*E*/
/* Fichier : $Id: vuem01sp_go.prc,v 1.4 2016/07/08 15:16:52 devgfi Exp $        Release : $Revision: 1.4 $        Date : $Date: 2016/07/08 15:16:52 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER vuem01sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* vues metiers
*
------------------------------------------------------
* HISTORIQUE :
*
* PCL  17/02/2011 : bug origine alerte car site alerte pas pris en compte 1.3 MANTIS 511 
* JPL	22/08/2012 : Simple renommage de constantes  1.3
* JPL	08/07/2016 : Support regions pour criteres geographiques; compatible precedente (DEM 1171)  1.4
-----------------------------------------------------*/
/*X*/
/*-----------------------------------------------------
* Sequence d'appel
* SP	VUEM01_Detail_exploitation
* 
* Arguments en entree
* XDY_Horodate	va_Debut_in
* XDY_Site	va_District_in
* 
* Arguments en sortie
* 
* premiere liste retournee
*	debut
*	fin
*	duree,
*	autoroute,
*	PR,
*	sens,
*	nom_type,
*	longueur,
*	duree_bouchon,
*	type_bouchon,
*	nombre_de_vl,
*	nombre_de_pl,
*	nombre_de_car,
*	nombre_de_moto,
*	nombre_de_remorques,
*	morts,
*	blesses_graves,
*	blesses_legers,
*	fausse_alerte,
*	voies_neutralisees,
*	origine,
*	point_caracteristique,
*	type_meteo,
*	numero,
*	cle
* 
* deuxieme liste retournee : liste des commentaires par fmc
*	numero,
*	cle
*	texte
*
* Service rendu
* Detail des actions d'exploitation/evenement d'un mois pour un district
* 
* Code retour
* XDC_OK
* XDC_NOK	: actio non trouvee
* XDC_ARG_INV	: parametres d'entree null ou site incorrect
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* 
* Fonction
-----------------------------------------------------------------------*/
use PRC
go
if exists (select 1 from sysobjects where name = 'VUEM01_GO' and type = 'P')
 drop procedure VUEM01_GO
go
create procedure VUEM01_GO
 @va_Debut_in datetime = null,
 @va_District_in T_SITE = null
as
 declare @vl_Fin datetime, @vl_Type smallint, @vl_TypeBouchon tinyint,
  @vl_NumEvt int, @vl_CleEvt tinyint, @vl_Cause int,
  @vl_CleCause tinyint, @vl_CauseBouch int, @vl_CleCauseBouch tinyint,
  @vl_Sens T_SENS, @vl_NomSens varchar(100),
  @vl_Longueur int, @vl_fausse_alerte tinyint, @vl_Autoroute T_NOM, @vl_PR T_PR,
  @vl_MaxPR int, @vl_MinPR int, @vl_TypeVeh tinyint, @vl_Duree int,
  @vl_nombre_de_vl tinyint, @vl_nombre_de_pl tinyint, @vl_nombre_de_car tinyint,
  @vl_nombre_de_moto tinyint, @vl_nombre_de_remorques tinyint, @vl_morts tinyint,
  @vl_blesses_graves tinyint, @vl_blesses_legers tinyint, @vl_Densite char(15),
  @vl_Site T_SITE, @vl_NumAutoroute T_AUTOROUTE, @vl_Total tinyint, @vl_TypeCauseBouch tinyint,
  @vl_Texte char(250), @vl_PointCar tinyint, @vl_DateCourante datetime, @vl_DateButee datetime,
  @vl_TypePointCar varchar(100), @vl_Suppression bit, @vl_auto char(5),
                @vl_BAU tinyint, @vl_VM1 tinyint, @vl_VM2 tinyint, @vl_VL tinyint, @vl_VR tinyint
 /*A controle des parametres d'entree */
 if @va_Debut_in is null
  return 2
 select @vl_Fin = dateadd(month, 1, @va_Debut_in), @vl_DateCourante = getdate()
 if @vl_DateCourante > @vl_Fin
  select @vl_DateButee = @vl_Fin
 else select @vl_DateButee = getdate()
 /*A table des PRs des bouchons */
 create table #BOU_PR (numero int, cle tinyint, horodate datetime null, PR int null, cause int null, cle_cause tinyint null)
 create table #GRA_BOU ( horodate datetime , PR_debut int , PR_fin int, longueur int null)
 /* creer et renseigner une table temporaires des sites (regions, secteurs, districts) */
 create table #LISTE_SITES ( numero tinyint null )
 insert #LISTE_SITES ( numero ) values ( @va_District_in )
 /* completer les regions par les secteurs */
 if exists ( select 1 from #LISTE_SITES where numero = 25 )
 begin
  insert #LISTE_SITES ( numero ) values ( 15 )
  insert #LISTE_SITES ( numero ) values ( 16 )
 end
 if exists ( select 1 from #LISTE_SITES where numero = 26 )
 begin
  insert #LISTE_SITES ( numero ) values ( 17 )
  insert #LISTE_SITES ( numero ) values ( 18 )
 end
 /* completer les secteurs par les districts */
 if exists ( select 1 from #LISTE_SITES where numero = 15 )
 begin
  insert #LISTE_SITES ( numero ) values ( 11 )
  insert #LISTE_SITES ( numero ) values ( 14 )
 end
 if exists ( select 1 from #LISTE_SITES where numero = 16 )
 begin
  insert #LISTE_SITES ( numero ) values ( 13 )
 end
 if exists ( select 1 from #LISTE_SITES where numero = 17 )
 begin
  insert #LISTE_SITES ( numero ) values ( 21 )
  insert #LISTE_SITES ( numero ) values ( 22 )
 end
 if exists ( select 1 from #LISTE_SITES where numero = 18 )
 begin
  insert #LISTE_SITES ( numero ) values ( 24 )
 end
 /*A rechercher les fmc du mois */
 select @vl_Texte=null
 select @vl_TypePointCar=null
 select @vl_auto = null
 select
  numero = HIS..FMC_GEN.numero,
  cle = HIS..FMC_GEN.cle,
  debut=debut,
  fin=fin,
  duree=null,
  autoroute=@vl_auto,
  PR = null,
  sens=null,
  nom_sens=" ",
  point_caracteristique=@vl_TypePointCar,
  type=HIS..FMC_GEN.type,
  nom_type=CFG..TYP_FMC.nom,
  fausse_alerte=null,
  nombre_de_vl=0,
  nombre_de_pl=0,
  nombre_de_car=0,
  nombre_de_moto=0,
  nombre_de_remorques=0,
  morts=0,
  blesses_legers=0,
  blesses_graves=0,
  cause=cause,
  cle_cause=cle_cause,
  voies_neutralisees=0,
  longueur=null,
  type_bouchon=" ",
  duree_bouchon=null,
  origine=origine_creation,
  commentaires= @vl_Texte,
  type_meteo=null,
  alerte=alerte,
  site_alerte=sit_alerte,
                bau=0,vl=0,vm1=0,vm2=0,vr=0
 into #LISTE_FMC
 from HIS..FMC_GEN, CFG..TYP_FMC
 where type != 37 and
  type != 4 and
  type != 18 and
  ((datex<>1 and @va_District_in <> 99) or (datex=1 and @va_District_in =99)) and
  debut >= @va_Debut_in and debut < @vl_Fin and
  CFG..TYP_FMC.numero = HIS..FMC_GEN.type
 order by HIS..FMC_GEN.numero
 /*A Ajout de l'origine de l'alerte : PAU, DAI ou ... */
 update #LISTE_FMC set origine = libelle
 from #LISTE_FMC,CFG..LIB_PRE,HIS..ALT_EXP
 where alerte is not null and
  CFG..LIB_PRE.notyp = 23 and
  HIS..ALT_EXP.numero = #LISTE_FMC.alerte and
  HIS..ALT_EXP.sit = #LISTE_FMC.site_alerte and
  CFG..LIB_PRE.code = HIS..ALT_EXP.type_equipement
 select numero,
  cle,
  type,
  cause,
  cle_cause
 into #COPIE
 from #LISTE_FMC
 declare pointeur cursor
 for select
  numero,
  cle,
  type,
  cause,
  cle_cause
 from #COPIE
 open pointeur
 fetch pointeur into @vl_NumEvt, @vl_CleEvt, @vl_Type,
  @vl_Cause, @vl_CleCause
 while @@sqlstatus = 0
 begin
    /*B rechercher la derniere position */
    select
  @vl_NumAutoroute = HIS..FMC_HIS.autoroute,
  @vl_Autoroute = CFG..RES_AUT.nom,
  @vl_PR = HIS..FMC_HIS.PR,
  @vl_Sens = HIS..FMC_HIS.sens,
  @vl_PointCar = HIS..FMC_HIS.point_caracteristique,
  @vl_fausse_alerte = fausse_alerte
    from HIS..FMC_HIS, CFG..RES_AUT, HIS..FMC_GEN
    where HIS..FMC_HIS.numero = @vl_NumEvt and HIS..FMC_HIS.cle = @vl_CleEvt and
  HIS..FMC_GEN.numero = HIS..FMC_HIS.numero and HIS..FMC_GEN.cle = HIS..FMC_HIS.cle and
  CFG..RES_AUT.numero = HIS..FMC_HIS.autoroute and
  horodate_validation in ( select max(horodate_validation)
     from HIS..FMC_HIS
     where numero = @vl_NumEvt and cle = @vl_CleEvt)
   if @@rowcount = 0
  select @vl_Suppression = 1
   else select @vl_Suppression = 0
          /* recherche de la ligne ou il y a le plus de nombre de voies bloquees ou neutralisees ou les 2
             avec un poids affecte pour chaque voie de 1,2,3,4,20 pour les VR,VM2,VM1,VL,BAU */
          select @vl_BAU=BAU,@vl_VL=VL, @vl_VM1=VM1,@vl_VM2=VM2,@vl_VR=VR from HIS..FMC_HIS
                 where HIS..FMC_HIS.numero = @vl_NumEvt and HIS..FMC_HIS.cle = @vl_CleEvt
                    group by HIS..FMC_HIS.numero, HIS..FMC_HIS.cle
                    having (BAU%3*BAU%5*BAU%6*20+VL%3*VL%5*VL%6*4+VM1%3*VM1%5*VM1%6*3+VM2%3*VM2%5*VM2%6*2+VM1%3*VM1%5*VM1%6*3)
                      =min (BAU%3*BAU%5*BAU%6*20+VL%3*VL%5*VL%6*4+VM1%3*VM1%5*VM1%6*3+VM2%3*VM2%5*VM2%6*2+VM1%3*VM1%5*VM1%6*3)
                             and HIS..FMC_HIS.numero = @vl_NumEvt and HIS..FMC_HIS.cle = @vl_CleEvt
          /* mise a jour des champs voie */
          update #LISTE_FMC set bau=isnull(@vl_BAU,0),vl=isnull(@vl_VL,0),vm1=isnull(@vl_VM1,0),vm2=isnull(@vl_VM2,0),vr=isnull(@vl_VR,0) where numero=@vl_NumEvt and cle=@vl_CleEvt
   /*B si bouchon alors recherche le PR initial de la tete de bouchon */
   if @vl_Type = 20
  select @vl_PR = PR, @vl_PointCar = HIS..FMC_HIS.point_caracteristique from HIS..FMC_HIS
  where numero = @vl_Cause and cle = @vl_CleCause
  group by numero, cle
  having numero = @vl_Cause and cle = @vl_CleCause and
   horodate_validation = min(horodate_validation)
    /* recherche du district */
    select
  @vl_Site = district
    from CFG..RES_POR, CFG..RES_DIS
    where autoroute = @vl_NumAutoroute and
  PR_debut <= @vl_PR and
  PR_fin > @vl_PR and
  CFG..RES_DIS.numero = district
    /* fmc en limite de concession */
    if @@rowcount = 0
  select @vl_Site = district
     from CFG..RES_POR, CFG..RES_DIS
     where autoroute = @vl_NumAutoroute and
   PR_fin = @vl_PR and
   CFG..RES_DIS.numero = district
    if ( @vl_Site in (select numero from #LISTE_SITES) or @va_District_in = 99 ) and @vl_Suppression = 0
    begin
  select @vl_Duree = isnull(datediff(minute, debut, fin), datediff(minute, debut, @vl_DateButee))
  from #LISTE_FMC
  where numero = @vl_NumEvt and cle = @vl_CleEvt
  if @vl_Sens = 0
   select @vl_Sens = 0
  else if @vl_Sens % 2 = 1
   select @vl_Sens = 1
  else
   select @vl_Sens = 2
  select
   @vl_NomSens = libelle
  from CFG..LIB_PRE
  where notyp = 17 and code = @vl_Sens
  if @vl_PointCar is not null
   select @vl_TypePointCar = libelle
   from CFG..LIB_PRE
   where notyp = 18 and code = @vl_PointCar
  else select @vl_TypePointCar = null
  update #LISTE_FMC set
   duree = @vl_Duree,
   autoroute = @vl_Autoroute,
   fausse_alerte = @vl_fausse_alerte,
   PR = @vl_PR,
   sens = @vl_Sens,
   nom_sens = @vl_NomSens,
   point_caracteristique = @vl_TypePointCar
  where numero = @vl_NumEvt and cle = @vl_CleEvt
  /*B pour une perturbation rechercher son type */
  if @vl_Type = 5
  begin
   update #LISTE_FMC set type_meteo = type_de_perturbation
   from HIS..FMC_MET
   where #LISTE_FMC.numero = @vl_NumEvt and #LISTE_FMC.cle = @vl_CleEvt and
    HIS..FMC_MET.numero = #LISTE_FMC.numero and
    HIS..FMC_MET.cle = #LISTE_FMC.cle
  end
  /*B pour un accident rechercher les infos de l'accident */
  if @vl_Type = 1
  begin
   select @vl_nombre_de_vl = isnull(HIS..FMC_ACC.nombre_de_vl,0),
    @vl_nombre_de_pl = isnull(HIS..FMC_ACC.nombre_de_pl,0),
    @vl_nombre_de_remorques = isnull(HIS..FMC_ACC.nombre_de_remorques,0),
    @vl_nombre_de_car = isnull(HIS..FMC_ACC.nombre_de_car,0),
    @vl_nombre_de_moto = isnull(HIS..FMC_ACC.nombre_de_moto,0),
    @vl_morts = isnull(HIS..FMC_ACC.morts,0),
    @vl_blesses_legers = isnull(HIS..FMC_ACC.blesses_legers,0),
    @vl_blesses_graves = isnull(HIS..FMC_ACC.blesses_graves,0)
   from HIS..FMC_ACC
   where numero = @vl_NumEvt and
    cle = @vl_CleEvt
   group by numero,cle
   having validation=max(validation)
   update #LISTE_FMC set
    nombre_de_vl = @vl_nombre_de_vl,
    nombre_de_pl = @vl_nombre_de_pl,
    nombre_de_remorques = @vl_nombre_de_remorques,
    nombre_de_car = @vl_nombre_de_car,
    nombre_de_moto = @vl_nombre_de_moto,
    morts = @vl_morts,
    blesses_legers = @vl_blesses_legers,
    blesses_graves = @vl_blesses_graves
   where #LISTE_FMC.numero = @vl_NumEvt and #LISTE_FMC.cle = @vl_CleEvt
  end
  /*B pour un vehicule rechercher les infos du vehicule */
  if @vl_Type = 6 or
     @vl_Type = 7 or
     @vl_Type = 8 or
     @vl_Type = 9
  begin
   select
    @vl_TypeVeh = type_de_vehicule
   from HIS..FMC_VEH
   where numero = @vl_NumEvt and cle = @vl_CleEvt
   if @vl_TypeVeh = 1
    update #LISTE_FMC set nombre_de_pl = 1
    where numero = @vl_NumEvt and cle = @vl_CleEvt
   else if @vl_TypeVeh = 2
    update #LISTE_FMC set nombre_de_vl = 1
    where numero = @vl_NumEvt and cle = @vl_CleEvt
   else if @vl_TypeVeh = 3
    update #LISTE_FMC set nombre_de_car = 1
    where numero = @vl_NumEvt and cle = @vl_CleEvt
   else if @vl_TypeVeh = 4
    update #LISTE_FMC set nombre_de_moto = 1
    where numero = @vl_NumEvt and cle = @vl_CleEvt
   else if @vl_TypeVeh = 5
    update #LISTE_FMC set nombre_de_remorques = 1
    where numero = @vl_NumEvt and cle = @vl_CleEvt
  end
  /*B supprimer les basculements dus a des travaux */
  if @vl_Type = 17
  begin
   if exists (select numero from HIS..FMC_GEN
    where numero = @vl_Cause and cle = @vl_CleCause and
     type = 18 and datex<>1)
    delete #LISTE_FMC
    where numero = @vl_NumEvt and cle = @vl_CleEvt
  end
  if @vl_Type = 20
  begin
   /*B recherche des PR min et max */
   insert #BOU_PR (numero, cle, horodate, PR)
   select
    HIS..FMC_HIS.numero,
    HIS..FMC_HIS.cle,
    HIS..FMC_HIS.horodate_validation,
    HIS..FMC_HIS.PR
   from HIS..FMC_GEN, HIS..FMC_HIS
   where HIS..FMC_GEN.numero = @vl_Cause and
    HIS..FMC_GEN.cle = @vl_CleCause and
    HIS..FMC_GEN.datex<>1 and
    HIS..FMC_HIS.numero = HIS..FMC_GEN.numero and
    HIS..FMC_HIS.cle = HIS..FMC_GEN.cle
   insert #BOU_PR (numero, cle, horodate, PR, cause, cle_cause)
   select
    HIS..FMC_HIS.numero,
    HIS..FMC_HIS.cle,
    HIS..FMC_HIS.horodate_validation,
    HIS..FMC_HIS.PR,
    HIS..FMC_GEN.cause,
    HIS..FMC_GEN.cle_cause
   from HIS..FMC_GEN, HIS..FMC_HIS
   where HIS..FMC_GEN.numero = @vl_NumEvt and
    HIS..FMC_GEN.cle = @vl_CleEvt and
    HIS..FMC_GEN.datex<>1 and
    HIS..FMC_HIS.numero = HIS..FMC_GEN.numero and
    HIS..FMC_HIS.cle = HIS..FMC_GEN.cle
   insert #GRA_BOU (horodate, PR_debut, PR_fin)
   select distinct
    Q.horodate,
    T.PR,
    Q.PR
    from #BOU_PR T, #BOU_PR Q
   where T.numero = Q.cause and
    T.cle = Q.cle_cause and
    T.horodate <= Q.horodate
   group by T.numero,T.cle, Q.numero,Q.cle,Q.horodate
   having T.numero = Q.cause and
    T.cle = Q.cle_cause and
    T.horodate <= Q.horodate and
    T.horodate = max(T.horodate)
   insert #GRA_BOU (horodate, PR_debut, PR_fin)
   select distinct
    T.horodate,
    T.PR,
    Q.PR
   from #BOU_PR T, #BOU_PR Q
   where Q.cause = T.numero and
    Q.cle_cause = T.cle and
    Q.horodate <= T.horodate
   group by
    Q.numero,Q.cle,
    T.numero,T.cle,T.horodate
   having Q.cause = T.numero and
    Q.cle_cause = T.cle and
    Q.horodate <= T.horodate and
    Q.horodate = max(Q.horodate)
   update #GRA_BOU set longueur = abs(PR_fin - PR_debut)
   select @vl_MinPR = 0, @vl_MaxPR = 0, @vl_Longueur = 0
   if @vl_Sens = 1
   select @vl_MinPR = PR_debut, @vl_MaxPR = PR_fin
   from #GRA_BOU
   having longueur = max(longueur)
   if @vl_Sens = 2
   select @vl_MaxPR = PR_debut, @vl_MinPR = PR_fin
   from #GRA_BOU
   having longueur = max(longueur)
   delete #BOU_PR
   delete #GRA_BOU
   /*B calcul de la longueur de bouchon en prenant en compte les PR fantomes */
   exec XZAO;22 @vl_NumAutoroute, @vl_MinPR, @vl_NumAutoroute, @vl_MaxPR ,@vl_Longueur output
   /*B cause du bouchon */
   select @vl_CauseBouch=cause, @vl_CleCauseBouch=cle_cause, @vl_TypeCauseBouch=type
   from HIS..FMC_GEN
   where HIS..FMC_GEN.numero = @vl_Cause and
    HIS..FMC_GEN.cle = @vl_CleCause and
    HIS..FMC_GEN.datex<>1
   /*B niveau final du bouchon */
   select @vl_TypeBouchon = max(type)
   from HIS..FMC_BOU
   where HIS..FMC_BOU.numero = @vl_NumEvt and HIS..FMC_BOU.cle = @vl_CleEvt
   group by numero, cle
   having numero = @vl_NumEvt and cle = @vl_CleEvt and
    horodate = max(horodate)
   /*B rechercher le libelle associe au niveau final du bouchon */
   select @vl_Densite = libelle
   from CFG..LIB_PRE
   where notyp = 15 and code = @vl_TypeBouchon
   if @@rowcount = 0
    select @vl_Densite = " "
   select @vl_Cause=@vl_CauseBouch, @vl_CleCause=@vl_CleCauseBouch
   /*B si le bouchon est du a un autre incident alors renseigner     */
   /*B les champs de la cause pour ce bouchon sinon faire apparaitre */
   /*B le bouchon comme les autres incidents dans la liste           */
   if @vl_CauseBouch is not null and @vl_TypeCauseBouch != 18
   begin
    /*B si le bouchon est la premiere consequence alors le faire apparaitre sans cause */
    /*B sinon le faire apparaitre comme un evenement sans cause */
    if exists (select * from #LISTE_FMC
        where numero = @vl_CauseBouch and cle = @vl_CleCauseBouch and
      duree_bouchon is null)
    begin
     delete #LISTE_FMC
     where numero = @vl_NumEvt and cle = @vl_CleEvt
     update #LISTE_FMC set longueur = @vl_Longueur,
      PR = @vl_PR,
      duree_bouchon = @vl_Duree,
      type_bouchon = @vl_Densite
     where numero = @vl_CauseBouch and cle= @vl_CleCauseBouch
    end
    else
     update #LISTE_FMC set longueur = @vl_Longueur,
      PR = @vl_PR,
      duree = @vl_Duree,
      nom_type = "BOUCHON",
      type_bouchon = @vl_Densite
     where numero = @vl_NumEvt and cle= @vl_CleEvt
   end
   else
   begin
    update #LISTE_FMC set longueur = @vl_Longueur,
     PR = @vl_PR,
     duree = @vl_Duree,
     nom_type = "BOUCHON",
     type_bouchon = @vl_Densite
    where numero = @vl_NumEvt and cle= @vl_CleEvt
   end
   select @vl_Duree=0
  end
    end
    else
  delete #LISTE_FMC
  where numero = @vl_NumEvt and cle = @vl_CleEvt
    /*B passage a l'evenement suivant */
    fetch pointeur into @vl_NumEvt, @vl_CleEvt, @vl_Type,
   @vl_Cause, @vl_CleCause
 end
 /*A retourner la liste des fmc d'un mois */
 select convert(char(8),debut,3),
        convert(char(8),debut,8),
        convert(char(8),fin,3),
        convert(char(8),fin,8),
  duree,
  autoroute,
  PR,
  nom_sens,
  nom_type,
  isnull(longueur,0),
  duree_bouchon,
  type_bouchon,
  nombre_de_vl,
  nombre_de_pl,
  nombre_de_car,
  nombre_de_moto,
  nombre_de_remorques,
  morts,
  blesses_graves,
  blesses_legers,
  isnull(fausse_alerte,0),
  voies_neutralisees,
  origine,
  point_caracteristique,
  type_meteo,
  numero,
  cle,
                bau, vl, vm1, vm2,vr
 from #LISTE_FMC
 order by debut, autoroute, PR, sens
 /*A retourner les commentaires des fmc */
 select
  evenement,
  HIS..FMC_COM.cle,
  texte
 from #LISTE_FMC,HIS..FMC_COM
 where HIS..FMC_COM.evenement = #LISTE_FMC.numero and
  HIS..FMC_COM.cle = #LISTE_FMC.cle
 order by evenement, HIS..FMC_COM.cle, HIS..FMC_COM.numero
 return 0
go
