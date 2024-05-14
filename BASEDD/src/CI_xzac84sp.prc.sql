/*E*/
/* Fichier : $Id: xdc.h,v 1.378 2020/12/03 10:49:09 pc2dpdy Exp $        $Revision: 1.378 $        $Date: 2020/12/03 10:49:09 $
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
* GABORIT	21 Oct 1994 : échange des valeurs XDC_SENS_xxx entre nord et sud		(v1.94)
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
*				rendu visible les XDC_PATHxxx à Applix
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
* Niepceron     19 Dec 1996 : Mise à 30 du nb de familles (et non 28) (dem/1359) (1.252)
* Guilhou	26 dec 1996 : ajout de XDC_LIB_TYPE_VOIE (lib. predefinis) (DEM/1372) 1.253
* Guilhou	26 dec 1996 : ajout des XDC_CODE_VOIE_xxx (DEM/1372) 1.254
* Guilhou	04 fev 1997 : ajout des XDC_SEUIL_TDP_xxx (temps de parcours) 1.255
* VATHELOT	27 fev 1997 : ajout des XDC_LINEAIRE_xxxx (DEM/1397 : fiche metier 21) 1.256 
* Mismer	03 Mar 1997 : ajout des XDC_PMV_xxx (temps de parcours)(DEM/1396) 1.257
* VATHELOT	04 Mar 1997 : ajout des XDC_CONDITION_xxxx (DEM/1397 : fiche metier 22) 1.258
* Niepceron     26 Mar 1997 : ajout des XDC_LIB_POPAN1 (dem/1359) 1.259
* VATHELOT      02 Avr 1997 : ajout des types de periode (DEM/1397 : fiche metier 09) 1.260
* 				+ ajout des types de mesures 
* Guilhou     	07 Avr 1997 : RE-Mise à 30 du nb de familles (et non 28) (dem/1359) (1.261)
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
* ABE-GTIE	27/05/13 : mise à jour de la gestion du retour à la normale avec les modif : #define XDC_ECL_SEQ_RETOUR_NORMAL 5 -> 20 ET #define XDC_VEN_SEQ_RETOUR_NORMAL 8 -> 46
* JPL		15/10/13 : Ajout sequences Incendie, Desenfumage et 'Plein soleil'; ajout XDC_ACCES_xxx (pour DEM 1083)  1.346
* JPL		01/04/14 : Ajout sequences de ventilation (pour DEM 1089)  1.347
* PNI		03/12/14 : Modif lib action fax --> Mail DEM1115 1.348
* PNI		15/04/15 : Remplacement SIS par Surveillance Réseau DEM1126 1.349
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
* JPL		22/06/18 : Caractères minuscules accentués en ISO Latin (DEM 1274)  1.365
* JPL		28/06/18 : Ajout constante XDC_TFM_MSG_MODIFIE (DEM 1293)  1.366
* JPL		13/07/18 : Ajout sequence GTC SIG 'Fermeture tunnel' (DEM 1294)  1.367
* JMG		08/08/18 : ligne PMV passe a 18 car. suppression SAGA DEM1306 1.368
* RG		11/10/18 : Ajout constante XDC_PRV_BPV (DEM 1307) 1.369
* JPL		27/11/18 : Ajout constantes types de jour pour calendrier et type de libellé prédéfini (DEM 1312)  1.370
* JPL		28/03/19 : Ajout constantes XDC_LOCAL et XDC_NON_LOCAL  1.371
* JPL		24/06/19 : Ajout constantes d'état des FMC à traiter (configurées et présentées sur postes opérateurs) (DEM 1325)  1.372
* JPL		04/09/19 : Ajout constante de type de libelle predefini : Nature d'incendie (DEM 1352)  1.373
* JMG		10/12/19 : Ajout constante XDC_REV_TDP_SURCHARGE DEM1365 1.374
* LCL		18/11/19 : Ajout constant prefixe nom serveur SYBASE SQL_ MOVIS DEMXXXX 1.375	
* JMG		01/01/20 : ajout IMU 1.376
* ABE		15/07/20 : Ajout path ADA pour tada2.x DEM130 1.376
* CGR           27/07/20 : Ajout XDC_MAI_IMU DEM-SAE155 1.377
* ABE		03/12/20 : Ajout des seuils de validation TDP_FCD, XDC_SEUIL_AFFICHAGE_TDP_FCD et XDC_SEUIL_AFFICHAGE_TDP_FCD_ABSENCE_TDP_ESCOTA DEM-SAE152 1.379
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
/* Texte marqueur de négation */
/* Texte separateur d'elements de liste */
/* definition des cas d'appel des fonctions et procedures stockees */
/**********  Definition des codes de retour des fonctions et procedures stockees  **********/
/*!!! *****  ILS DOIVENT ETRE DISTINCTS DES CODES DES AUTRES MODULES (XZAE  ...)  ***** !!!*/
/*!!! **********  LES CODES A PARTIR DE 6 SONT UTILISES PAR LE MODULE XZAE  ********** !!!*/
/**********  Fin de definition des codes de retour des fonctions et procedures  **********/
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
/* différents états d'actions */
/* différents états d'alerte (ne pas modifier, utilisé pour réplication) */
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
/* Commandes d'états d'un equipement */
/* Constantes d'abonnement aux donnees EQUEXT */
/* noms des repertoires */
/*#define XDC_PATH_IMAGE       	     XDC_PATH_MIGRAZURHOME "/config/fichiers/adm"*/
/* PATH pour le fichier de bilan de configuration SOUS-SYS GECONF */
/*#define XDC_FICHIER_ASTREINTES_BC   " " */
/* Repertoire Temporaire pour le fichier d'Administartion */
/* Repertoire executable RTWorks */
/* Combiner les valeurs par des additions et non des "ou logiques" pour Applix */
/********** Taille des données *****************/
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
/******  Constantes Vidéo *****************************************/
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
/* Fichier : $Id: xzaec.h,v 1.83 2019/10/01 15:47:15 devgfi Exp $        $Revision: 1.83 $        $Date: 2019/10/01 15:47:15 $
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
/*E Fichier : @(#)xzaac.h	1.20      Release : 1.20        Date : 04/04/11
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME _SS_SYSTEME_
------------------------------------------------------
* MODULE XZAA * FICHIER xzaac.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Alertes
*
------------------------------------------------------
* HISTORIQUE :
* B.G.  21/10/94	: Creation
* B.G.  28/12/94	: modif (1.7)
* B.G.  28/12/94	: suppr TOR 1 a 13 (1.9)
* B.G.  23/01/95        :  ajout equation evt (1.12)
* F.V.  01/02/95        :  ajout equation NIV radt (1.13)
* C.T.  01/12/95        :  ajout d'un type d'alerte et du libelle a afficher associe (1.14)
* D.M.  21/03/96        :  ajout d'un type d'alerte pour mini GTC (DEM/734) (1.15)
* C.T.  25/03/96        :  modif d'un libelle a afficher associe (1.16-1.17)
* C.T.  22/04/96        :  suppression alerte sur fax (1.16-1.17)
* PNI	04/07/08	: ajout alerte EDF DEM788
* JMG	18/03/11	: ajout constantes PAU virtuel
* SDU   10/02/12    : DEM 1015 - Communautaire
* SDU   12/04/12    : DEM 1016 - Ajout Alerte Regulation
* JMG	26/09/16	: alerte DAI SAGA DEM11991
* JMG	18/01/18 	: alerte GABEB
* JMG	13/05/19	: ajout alerte 106 GAGEB
------------------------------------------------------ */
/*A Alertes DAI (nom du type sur 25 caracteres maxi */
/*A Alertes PMV */
/* Alertes GTC de NICE */
/*A Alertes NAV */
/*A Alertes RAU */
/*A Alertes TOR */
/*A Alertes RADT */
/*A Alertes VIDEO */
/*A Alertes equation evt */
/*A Alertes RADT */
/*A Alertes Mini GTC suite */
/*E Fichier : $Id: xzacc.h,v 1.2 2013/03/22 10:48:14 devgfi Exp $      Release : $Revision: 1.2 $        Date : $Date: 2013/03/22 10:48:14 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME _SS_SYSTEME_
------------------------------------------------------
* MODULE XZAA * FICHIER xzacc.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Alarmes
*
------------------------------------------------------
* HISTORIQUE :
* C.T.  22/10/94	: Creation
* JPL	22/03/13	: Ajout constante de priorite d'action programmee (DEM 1061)  1.2
------------------------------------------------------ */
/*A texte d'avertissement de FAX brule */
/*E Fichier : $Id: xzahc.h,v 1.9 2007/05/11 15:59:18 gesconf Exp $      Release : $Revision: 1.9 $        Date : $Date: 2007/05/11 15:59:18 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME _SS_SYSTEME_
------------------------------------------------------
* MODULE XZAH * FICHIER xzahc.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Animation ihm
*
------------------------------------------------------
* HISTORIQUE :
* B.G.  18/10/94	: Creation
* B.G.  25/10/94        : Modif valeur XZAHC_VoieInconnue (1.2)
* C.T.  04/12/94        : Ajout XZAHC_VOIES_xxxx  et
*                         XZAHC_Bouchon XZAHC_Basculement (1.3)
* C.T.  27/03/95        : Ajout XZAHC_Gare (1.4)
* C.T.  30/03/95        : Ajout des objets mobiles de type accident (1.5)
* C.T.  05/04/95        : Ajout des objets inervention autre (1.7)
* P.N.  08/01/96        : Ajout des constantes de voies (dem/1372) (1.8)
* JPL.	11/05/07	: Decalage constantes de types perturbations meteo (lie a DEM 643-647) (1.9)
------------------------------------------------------ */
/*A differentes voies */
/*A type d'extrÅmitÅ */
/*A tete et queue de bouchon */
/*A types d'objets mobiles (attention , ne pas reprendre les mÁmes valeurs
 que pour XZAEC_FMC_xxx */
/*A type de donnees sur les voies initialisant le sous systeme EQUEXT */
/*A Type d'evenement : bouchon ou basculement */
/*A Type de voie */
/*E*/
/* Fichier : $Id: xzac84sp.prc,v 1.10 2020/10/13 17:33:18 pc2dpdy Exp $        Release : $Revision: 1.10 $        Date : $Date: 2020/10/13 17:33:18 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzac84sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des actions
*
------------------------------------------------------
* HISTORIQUE :
*
* jmg	10/06/05	creation
* LCL   09/07/07        Suppression DY
* JMG           02/10/09        : secto DEM887 1.3
* VR	30/05/2012	: AjouT TDP PMVA (DEM/1014 PMA)
* PNI           19/04/16 : Correction des appels dans PRC des XZAC sur les serveurs sites v1.5
 DEM1159
* JMG	06/03/17	: regionalisation code region  DEM1220 v1.6
* JPL	08/03/17 : Enregistrement des neutres 3points et Temps de Parcours en table dediee (DEM 1196)  1.7
* LCL	03/07/17	: Modif PRA
* JPL	01/09/17 : En mode DEV, pas d'envoi de commande a l'equipement et action mise en fin (DEM 1231)  1.9
* LCL	20/11/19	: MOVIS DEM-SAE93 1.10
* GRO	12/10/2020      : MOVIS DEM-SAE93 1.11				
-----------------------------------------------------*/
/*X*/
/*-----------------------------------------------------
* Service rendu
* Terminer l'action d'affichage PMVA.
* 
* Sequence d'appel
* SP	XZAC84_Commande_Fin_PMVA
* 
* Arguments en entree
* XDY_Eqt	va_NumeroPMA_in
* XDY_Entier	va_Site_in
* XDY_Horodate	va_HeureLancementFin_in
* 
* Arguments en sortie
* 
* Code retour
* XDC_OK
* XDC_NOK	: actio non trouvee ou site incorrect
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* Pour revenir l'affichage par defaut sur PMVA
* 
* Fonction
* Inserer dans la table ACTION la date de fin lance de l'actio.
* ACRP84 :	Appeler XZEP01 pour arreter l'affichage sur l'equipement PMVA
* 		Appeler XZEP02 pour arreter l'affichage du picto
-----------------------------------------------------*/
use PRC
go
if exists (select * from sysobjects where name = 'XZAC84' and type = 'P')
 drop procedure XZAC84
go
create procedure XZAC84
 @va_NumeroPMA_in T_EQUIPEMENT = null,
 @va_Site_in T_SITE = null,
 @va_HeureLancementFin_in datetime = null,
 @va_TypeNeutre_in tinyint = null,
 @va_NomSiteLocal_in char(2) = null,
 @va_distant_in int = 0
as
 declare @vl_PMATexte tinyint, @vl_PMAPasFlash tinyint, @vl_Clignotement tinyint,
  @vl_Texte char(15),
  @vl_NumeroAction int, @vl_Status int, @vl_NomMachine char(10),
  @vl_TypeEqtBoucle tinyint, @vl_NumEqtBoucle T_EQUIPEMENT,
  @vl_NumMaitre T_EQUIPEMENT, @vl_TypeMaitre tinyint,
  @vl_MachineNonTrouvee bit, @vl_Commande int,
     @vl_Dest_Un_Proche smallint, @vl_Dest_Un_Lointaine smallint, @vl_Dest_Deux_Proche smallint, @vl_Dest_Deux_Lointaine smallint, @vl_DepSeuil tinyint,
  @vl_Site tinyint
 /*! transfo en parametres nulls bug applix */
 if @va_NumeroPMA_in = 0 select @va_NumeroPMA_in = null
 if @va_Site_in = 0 select @va_Site_in = null
 if @va_HeureLancementFin_in = "1900" select @va_HeureLancementFin_in = null
 select @vl_Dest_Un_Proche=0
 select @vl_Dest_Un_Lointaine=0
 select @vl_Dest_Deux_Proche=0
 select @vl_Dest_Deux_Lointaine=0
 select @vl_DepSeuil=0
 /*A controle des parametres obligatoires */
 if @va_NumeroPMA_in = null or @va_Site_in = null or
     @va_HeureLancementFin_in = null
  return 2
        /*A Determiner le site local */
        /* MOVIS */
 if @va_NomSiteLocal_in = null
         select @vl_Site = numero
         from CFG..RES_DIS
         where serveur = @@servername
 else
        begin
                if @va_distant_in = 0
                begin
   select @vl_Site = numero
   from CFG..RES_DIS
   where code = @va_NomSiteLocal_in
                end
                else
                begin
                        select @vl_Site = @va_Site_in
                end
        end
 if @vl_Site is null
  return 1
 /* PRA
	if @@servername = XDC_SQLCI
	*/
 if @vl_Site = 43 and @va_distant_in = 0
 begin
  if @va_Site_in = 26
   exec @vl_Status = SQL_DS.PRC..XZAC84 @va_NumeroPMA_in, @va_Site_in, @va_HeureLancementFin_in, @va_TypeNeutre_in, @va_NomSiteLocal_in, 1
  else if @va_Site_in = 25
   exec @vl_Status = SQL_DM.PRC..XZAC84 @va_NumeroPMA_in, @va_Site_in, @va_HeureLancementFin_in, @va_TypeNeutre_in, @va_NomSiteLocal_in, 1
  else return 1
  return @vl_Status
 end
 /*A tester si le site est correct */
 if not exists ( select numero from CFG..RES_DIS
                                where numero = @va_Site_in )
/*                                where serveur = @@servername )
*/
  return 1
 /*A Rechercher la derniere action en cours sur cet equipement */
 select @vl_NumeroAction = max(numero)
 from EXP..ACT_GEN
 where equipement = @va_NumeroPMA_in and
  sit = @va_Site_in and type = 46
 if @@rowcount = 0
  select @vl_NumeroAction = 0
 /*A initialisation des var locales pour la recherche */
 /*A du nom de la machine pilotant le PMVA */
 select @vl_MachineNonTrouvee = 0,
  @vl_NumEqtBoucle = @va_NumeroPMA_in,
  @vl_TypeEqtBoucle = 46
 /*A recherche le nom de la machine pilotant le PMVA */
 while @vl_MachineNonTrouvee = 0
 begin
  select
   @vl_NumMaitre = maitre,
   @vl_TypeMaitre = type_maitre,
   @vl_NomMachine = nom
  from CFG..EQT_GEN
  where numero = @vl_NumEqtBoucle and type = @vl_TypeEqtBoucle
  if @vl_NumMaitre = null or @@rowcount != 1
   return 1
  /*A test si c'est la machine qui pilote */
  if @vl_TypeMaitre = 13
  begin
   select @vl_NomMachine = nom
   from CFG..EQT_GEN
   where numero = @vl_NumMaitre and type = @vl_TypeMaitre
   select @vl_MachineNonTrouvee = 1
  end
  else select @vl_NumEqtBoucle = @vl_NumMaitre,
    @vl_TypeEqtBoucle = @vl_TypeMaitre
 end
 if @vl_MachineNonTrouvee = 0
  return 1
 /*A initialisation des types et texte d'affichage */
 select @vl_PMATexte = isnull(@va_TypeNeutre_in,5),
  @vl_PMAPasFlash = 1,
  @vl_Clignotement = 1,
  @vl_Texte = null
 if @vl_PMATexte = 0
  select @vl_PMATexte = 5
 /*A numero de commande a envoyer : - numero action */
 select @vl_Commande = - @vl_NumeroAction
 begin tran
  /*A mettre a jour la date de lancement fin s'il existe une action a terminee */
  if @vl_NumeroAction != 0
  begin
   update EXP..ACT_GEN set heure_lancement_fin = @va_HeureLancementFin_in
                         , heure_fin = @va_HeureLancementFin_in
   where numero = @vl_NumeroAction and sit = @va_Site_in and
    heure_lancement_fin is null
  end
 commit tran
 return 0
go
