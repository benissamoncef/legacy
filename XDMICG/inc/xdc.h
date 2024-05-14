/* Fichier : xdc.h
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
* CGR		27/07/20 : Ajout XDC_MAI_IMU DEM-SAE155 1.377
* ABE		03/12/20 : Ajout des seuils de validation TDP_FCD, XDC_SEUIL_AFFICHAGE_TDP_FCD et XDC_SEUIL_AFFICHAGE_TDP_FCD_ABSENCE_TDP_ESCOTA DEM-SAE152 1.379
* CGR		04/12/20 : Ajout XDC_Lon_CommandeIMU DEM-SAE155 1.379
* ABE		15/12/20 : Modif XDC_Lon_commandeIMU pour DXC_Lon_ChaineIMU, ajout XDC_ACT_IMU, XDC_ACT_LIB, XDC_MACRO_MTIMU DEM-SAE155 1.379
* ABE		02/04/21 : Ajout PAU_SONO DEM-SAE244 1.380
* ABE		18/11/21 : Ajout enum categorie action pour filtrage dans la synthèse magistra DEM-SAE311 1.381	
* LCL		20/12/21 : MOVIS P2 ems SAE-312 / Mix MAGISTRA
* CGR		02/03/22 : Ajout XDC_ALT_VRAIE_FUSION_FMC_SAE_EXT qualification d'alerte AVA DEM-SAE367 1.382
* PNI		18/07/2022	: modification du seuil de prise en compte des donnees TDP FCD SAE-394
* ABE		02/08/2022	: Ajout NIC et IS pour localisation tunnel DEM-SAE403
* LCL		17/03/2023	: Ajout XDC_ASF pour gestion bouchons Cockpit sur A8 ASF + A7
* GGY		28/04/23	: Augmentation taille XDC_LGR_TXT_PICTO et XDC_Lon_TxtPicto et XDC_Lon_NomPicto (DEM-473)
* JPL		10/05/23	: Ajout constantes Controleurs de Feux (SAE-440)
* ABK		17/08/2023	: Ajout des constantes XDC_PIC_BPV, XDC_PIC_ACCOTEMENT, XDC_Lon_Ligne_PIC, XDC_LIB_PIC, XDC_EQT_PIC pour nouveau type picto
* LCL		26/09/23
* GGY		28/04/23	: Ajout XDC_RES_BAT et XDC_POINT_CARACT_BAT et XDC_NOM_TYP_PTCAR_BAT (DEM-483)
* GGY		28/04/23 : Augmentation XDC_LGR_TXT_PICTO 8 (DEM-473)
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

#ifndef xdc
#define xdc

/* Non Applix - fin */


/* definitions des constantes  */

/* caracteristiques du RTServer */

#define XDC_NOM_APPLICATION "migrazur"

/* booleens */

#define XDC_FAUX	0
#define XDC_VRAI	1
#define XDC_OUI		1
#define XDC_NON		0

#define XDC_LOCAL	1
#define XDC_NON_LOCAL	0


/* chaine vide ou nulle pour sybase (ne pas changer la valeur) */
#define XDC_CHAINE_VIDE		" "

/* champ numerique non value */
#define XDC_INFO_NON_VALUEE	-1

/* Texte marqueur de negation */
#define XDC_CAR_NON		"!"

/* Texte separateur d'elements de liste */
#define XDC_SEP_ELT_LST		","


/* definition des cas d'appel des fonctions et procedures stockees */

#define XDC_FCT_APL_PREMIER	1
#define XDC_FCT_APL_DERNIER	2
#define XDC_FCT_APL_UNIQUE	3	/* a la fois premier et dernier */
#define XDC_FCT_APL_SUIVANT	4


/**********  Definition des codes de retour des fonctions et procedures stockees  **********/
/*!!! *****  ILS DOIVENT ETRE DISTINCTS DES CODES DES AUTRES MODULES (XZAE  ...)  ***** !!!*/

#define XDC_OK		0	/* Fonction terminee correctement */
#define XDC_NOK		1
#define	XDC_PASOK	(XDC_NOK)
#define XDC_ARG_INV	2
#define XDC_PAS_SYN	3	/* pas de synthese pour cet evenement */
#define XDC_PRC_INC	4	/* procedure stockee inconnue */
#define XDC_REV_PB_DCL	5	/* probleme au declenchement d'un ou plusieurs reveils */

/*!!! **********  LES CODES A PARTIR DE 6 SONT UTILISES PAR LE MODULE XZAE  ********** !!!*/


#define XDC_ACT_REFUSEE		101	/* traitement refuse */
#define XDC_OBJ_AUCUN		102	/* objet introuvable */
#define XDC_OBJ_EXISTE		103	/* objet deja existant */
#define XDC_OBJ_LOC		104	/* une localisation est en cause */
#define XDC_OBJ_BORNE		105	/* une borne est en cause, ou franchie */

/**********  Fin de definition des codes de retour des fonctions et procedures  **********/
/* Categories des actions sur les fmc pour la synthhse magistra */
#define XDC_CAT_ACT_AUTRE			0
#define XDC_CAT_ACT_COMMENTAIRE		1
#define XDC_CAT_ACT_ETAPE			2
#define XDC_CAT_ACT_APPEL			3
#define XDC_CAT_ACT_COMMANDE		4
#define XDC_CAT_ACT_MODIFICATION	5

/* Etats equipements */

#define	XDC_MARCHE		"Marche"
#define	XDC_ARRET		"Arret"


#define XDC_MAX_CHAR_LEN	250 	/* Long de chaine max renvoyee*/
					/* dans une colonnesuite a un select*/
					/* effectue par une SP*/
#define XDC_PATH_ABS_SIZE	200
#define XDC_LINE_CMD_SIZE	300
#define XDC_NOM_FIC_SIZE        100

/* nombre de jours d'offset pour la date de purge */
#define XDC_OFFSET_EN_JOURS_PURGE_BASE	32

/* definition des constantes de la base test et config */

#define XDC_BASE_CFG		"CFG"
#define XDC_BASE_CFT		"CFT"

#define XDC_BASE_EXP		"EXP"
#define XDC_BASE_HIS		"HIS"

/* interface avec sybase : 1er janvier 1900 = null */

#define XDC_DATE_NULLE		"1900"
#define XDC_DATE_INFINIE	"9999"
#define XDC_DATE_CLOTURE_PURGE 	"01/01/1950 12:00"


/* definition des modes d'execution des fonctions */

#define	XDC_MODE_ASYNCHRONE	0
#define	XDC_MODE_SYNCHRONE	1


/* identification des ecrans de visualisation main courante et synoptique */

#define	XDC_ECRAN_TXT		0
#define	XDC_ECRAN_SYN		1
#define	XDC_ECRAN_UNIQUE	2


/* definition des types sur reseau */

#define XDC_RES_BAT			14
#define XDC_RES_TUBE		13
#define XDC_RES_BIFURCATION	12
#define XDC_RES_AIRE		11
#define XDC_RES_ECHANGEUR	1
#define XDC_RES_PEAGE		2
#define XDC_RES_ITPC		3
#define XDC_RES_PORTAIL		4
#define XDC_RES_PS		5
#define XDC_RES_RT		6
#define XDC_RES_VILLE		7
#define XDC_RES_PN_POL		8
#define XDC_RES_PENTE		9

#define XDC_RES_EAU 		1
#define XDC_RES_BT 		2
#define XDC_RES_MT 		3
#define XDC_RES_HT 		4
#define XDC_RES_THT 		5
#define XDC_RES_GAZ 		6
#define XDC_RES_AUTRE 		7
#define XDC_RES_PASSINF 	8

/* definition des types d'equipement */

#define XDC_EQT_NAV		1
#define XDC_LIB_NAV		"BRA"

#define XDC_EQT_PMV		2
#define XDC_LIB_PMV		"PMV"

#define XDC_EQT_TUBE		3
#define XDC_LIB_TUBE		"TUBE"

#define XDC_EQT_DAI		4
#define XDC_LIB_DAI		"DAI"

#define XDC_EQT_SYV		5
#define XDC_LIB_SYV		"Systeme video"

#define XDC_EQT_LTV		6
#define XDC_LIB_LTV		"LT video"

#define XDC_EQT_CAM		7
#define XDC_LIB_CAM		"Camera"

#define XDC_EQT_MAA		8
#define XDC_LIB_MAA		"MAA"

#define XDC_EQT_MNT		9
#define XDC_LIB_MNT		"Moniteur"

#define XDC_EQT_MAG		28
#define XDC_LIB_MAG		"Magnetoscope"

/*! * ATTENTION valeur utilisee par un autre "define" */
#define XDC_EQT_ECHANGEUR	11
#define XDC_LIB_ECHANGEUR	"Demi Echangeur"

#define XDC_EQT_RAU		12
#define XDC_LIB_RAU		"PIRAU"

#define XDC_EQT_MAC		13
#define XDC_LIB_MAC		"Machine"

#define XDC_EQT_PAU		14
#define XDC_LIB_PAU		"PAU"

#define XDC_EQT_NIC		15
#define XDC_LIB_NIC		"GTC NICE"

#define XDC_EQT_ECH		XDC_EQT_ECHANGEUR
#define XDC_LIB_ECH		XDC_LIB_ECHANGEUR

#define XDC_EQT_TUB		XDC_EQT_TUBE
#define XDC_LIB_TUB		"Tunnel"

#define XDC_EQT_CPT		18
#define XDC_LIB_CPT		"Capteur Tunnel"

#define XDC_EQT_FAV		19
#define XDC_LIB_FAV		"Feux d'affectation de voies"

#define XDC_EQT_PAN		20
#define XDC_EQT_PAN_FAV	191
#define XDC_LIB_PAN		"Panneau GTC"

#define XDC_EQT_MOD		21
#define XDC_LIB_MOD		"Module NAV"

#define XDC_EQT_SRV		22
#define XDC_LIB_SRV		"Serveur de terminaux"

#define XDC_EQT_GTC		23
#define XDC_LIB_GTC		"Mini GTC"

#define XDC_EQT_TOR		24
#define XDC_LIB_TOR		"Entree TOR"

#define XDC_EQT_PCT		25
#define XDC_LIB_PCT		"Picto"

#define XDC_EQT_RAD		26
#define XDC_LIB_RAD		"RADT"

#define XDC_EQT_ANA		27
#define XDC_LIB_ANA		"Analyseur"

#define XDC_EQT_LIT		10			/* attention valeur utilisee par un autre define */
#define XDC_LIB_LIT		"Lit d'arret"

#define XDC_EQT_EXT		29			/* extincteur */
#define XDC_LIB_EXT		"Extincteur"

#define XDC_EQT_CCO		30			/* taux de CO */
#define XDC_LIB_CCO		"Capteur taux CO"

#define XDC_EQT_OPA		31			/* opacite */
#define XDC_LIB_OPA		"Capteur Opacite"

#define XDC_EQT_OPE		32
#define XDC_LIB_OPE		"OPERATOR"

#define XDC_EQT_SAT		33
#define XDC_LIB_SAT		"SATIR"

#define XDC_EQT_FAX		34
#define XDC_LIB_FAX		"FAX"

#define XDC_EQT_GEN_LCR	35
#define XDC_LIB_GEN_LCR	"Systeme LCR externe"

#define XDC_EQT_MET		36
#define XDC_LIB_MET		"METEO"

#define XDC_EQT_GTC_SIS	37
#define XDC_LIB_GTC_SIS	"GTC Sismique"

#define XDC_EQT_TDP		38
#define XDC_LIB_TDP		"UGTP"

#define XDC_EQT_ZDP		39
#define XDC_LIB_ZDP		"Zone Temps de Parcours"

#define XDC_EQT_GAB		40			
#define XDC_LIB_GAB		"Hors gabarit"

#define XDC_EQT_VIT		41			
#define XDC_LIB_VIT		"Vitesse excessive"

#define XDC_EQT_NICHE	42			
#define XDC_LIB_NICHE	"Portes des niches"

#define XDC_EQT_FAC		43			
#define XDC_LIB_FAC		"FAC"

#define XDC_EQT_FAU		44			
#define XDC_LIB_FAU		"FAU"

#define XDC_EQT_PAL		45			
#define XDC_LIB_PAL		"PAL"

#define XDC_EQT_PMVA    46
#define XDC_LIB_PMVA    "PMVA"

#define XDC_EQT_BAF		47
#define XDC_LIB_BAF		"Barriere BAF"

#define XDC_EQT_BAD		48
#define XDC_LIB_BAD		"Barriere BAD"

#define XDC_EQT_MODBAD	49
#define XDC_LIB_MODBAD	"Module BAD"

#define XDC_EQT_MODBAF	50
#define XDC_LIB_MODBAF	"Module BAF"

#define XDC_EQT_MODPRV	51

#define XDC_EQT_PRV		62
#define XDC_LIB_PRV		"PRV"

#define XDC_EQT_SAGA	63
#define XDC_LIB_SAGA	"Eqt. SAGA"

#define XDC_EQT_CFE		66
#define XDC_LIB_CFE		"Controleur feux"

#define XDC_EQT_CAP		64
#define XDC_LIB_CAP		"Capteur"

#define XDC_EQT_IMU		67
#define XDC_LIB_IMU		"IMU"

#define XDC_EQT_NIC_IS	68
#define XDC_LIB_NIC_IS	"NICHE et Issue de Secour"

#define XDC_EQT_PIC		69
#define XDC_LIB_PIC		"PIC"

/* AAZ 18/06/2007 */
#define XDC_EQT_SIG         51
#define XDC_LIB_SIG         "Signalisation"
#define XDC_EQT_ECL         52
#define XDC_LIB_ECL         "Eclairage"
#define XDC_EQT_VEN         53
#define XDC_LIB_VEN         "Ventilation"
#define XDC_EQT_ENR         54
#define XDC_LIB_ENR         "Energie"
#define XDC_EQT_INC         55   
#define XDC_LIB_INC         "Protection incendie"
#define XDC_EQT_EXP         56
#define XDC_LIB_EXP         "Exploitation"
#define XDC_EQT_SYS         102
#define XDC_LIB_SYS         "SYS"
#define XDC_EQT_DOMAINE     58
#define XDC_LIB_DOMAINE     "Domaine"
#define XDC_EQT_EQT         59
#define XDC_LIB_EQT         "Equipement"
#define XDC_EQT_NIX         60
#define XDC_LIB_NIX         "GTC NICE RENOVEE"
#define XDC_EQT_LTN         100
#define XDC_LIB_LTN         "LTN"

#define XDC_EQT_HT		101
#define XDC_LIB_HT		"HT"
#define XDC_EQT_LI		103
#define XDC_LIB_LI		"LIT"
#define XDC_EQT_ZONE_REGUL		104
#define XDC_LIB_ZONE_REGUL		"Zone regulation"

#define XDC_DEBUT_TYPEQT_GTCRENOVEE		100

/* XPH : ajout des sequences speciales par domaine fonctionnel */
#define XDC_SIG_SEQ_FERMETURE_TUNNEL	1
#define XDC_SIG_SEQ_RETOUR_NORMAL	2
#define XDC_SIG_SEQ_HORS_SEQUENCE	20

#define XDC_ECL_SEQ_PLEIN_SOLEIL 1
#define XDC_ECL_SEQ_HORS_SEQUENCE 6
#define XDC_ECL_SEQ_RETOUR_NORMAL 20

#define XDC_VEN_SEQ_INCENDIE		1
#define XDC_VEN_SEQ_DESENF_TETE		2
#define XDC_VEN_SEQ_DESENF_MOITIE1	3
#define XDC_VEN_SEQ_DESENF_MOITIE2	4
#define XDC_VEN_SEQ_DESENF_SANS_CONG	5
#define XDC_VEN_SEQ_DESENF_AVEC_CONG	6
#define XDC_VEN_SEQ_DESENF_ANTI_RECYC	7
#define XDC_VEN_SEQ_ARRET_SEQ_DESENF	8
#define XDC_VEN_SEQ_HORS_SEQUENCE	9
#define XDC_VEN_SEQ_DESENFUM_NON_LOC	37
#define XDC_VEN_SEQ_RETOUR_NORMAL	46

#define XDC_HT_SEQ_HORS_SEQUENCE 17
#define XDC_HT_SEQ_RETOUR_NORMAL 18
#define XDC_BT_SEQ_HORS_SEQUENCE 17
#define XDC_BT_SEQ_RETOUR_NORMAL 18

#define XDC_INC_SEQ_HORS_SEQUENCE 17
#define XDC_INC_SEQ_RETOUR_NORMAL 18

#define XDC_EXP_SEQ_HORS_SEQUENCE 1
#define XDC_EXP_SEQ_RETOUR_NORMAL 2


#define XDC_EQTALL		0  		/* Tous les equipements */

/* numerotation des moniteurs de PC district et CI */
#define XDC_MONITEUR1		1
#define XDC_MONITEUR2		2
#define XDC_MONITEUR3		3
#define XDC_MONITEUR4		4
#define XDC_MONITEUR5		5
#define XDC_MONITEUR6		6
#define XDC_MONITEUR7		7
#define XDC_MONITEUR8		8

/* sous types de capteur tunnel */

#define XDC_CAPT_LIT_ARRET	XDC_EQT_LIT	/* attention valeur utilisee par un autre define */
#define XDC_CAPT_EXTINCTEUR	XDC_EQT_EXT	/* extincteur */
#define XDC_CAPT_CO		XDC_EQT_CCO	/* taux de CO */
#define XDC_CAPT_OPACITE	XDC_EQT_OPA	/* opacite */

#define XDC_CAP_TYPE_BOUCHON	1
#define XDC_CAP_TYPE_LIT_ARRET	2
#define XDC_CAP_DEB_SENS_1	3
#define XDC_CAP_FIN_SENS_1	1
#define XDC_CAP_DEB_SENS_2	4
#define XDC_CAP_FIN_SENS_2	2

/* Definition du maitre et de l'exclave */
#define XDC_MAITRE_INCONNU -1
#define XDC_MAITRE_TWWI0 0
#define XDC_MAITRE_TWWI1 1
#define XDC_TIME_OUT_ESSAI 30
#define XDC_TIME_OUT_SYNCHRO 300

/* definition des maitres des types d'equipement */

#define XDC_MAI_NAV		XDC_EQT_SRV
#define XDC_MAI_PMV		XDC_EQT_SRV
#define XDC_MAI_DAI		XDC_EQT_SRV
#define XDC_MAI_SYV		XDC_EQT_SRV
#define XDC_MAI_LTV		XDC_EQT_SYV
#define XDC_MAI_CAM		XDC_EQT_LTV
#define XDC_MAI_MAA		XDC_EQT_SRV
#define XDC_MAI_MNT		XDC_EQT_SYV
#define XDC_MAI_MAG		XDC_EQT_SYV
#define XDC_MAI_RAU		XDC_EQT_SRV
#define XDC_MAI_MAC		XDC_EQT_MAC
#define XDC_MAI_PAU		XDC_EQT_RAU
#define XDC_MAI_NIC		XDC_EQT_MAC
#define XDC_MAI_ECH		XDC_EQT_NIC
#define XDC_MAI_TUB		XDC_EQT_NIC
#define XDC_MAI_CPT		XDC_EQT_TUB
#define XDC_MAI_FAV		XDC_EQT_TUB
#define XDC_MAI_PAN_TUB		XDC_EQT_TUB	/* pour les panneaux d'un tube */
#define XDC_MAI_PAN_ECH		XDC_EQT_ECH	/* pour les panneaux d'un echangeur */
#define XDC_MAI_MOD		XDC_EQT_NAV
#define XDC_MAI_SRV		XDC_EQT_MAC
#define XDC_MAI_GTC		XDC_EQT_SRV
#define XDC_MAI_TOR		XDC_EQT_GTC
#define XDC_MAI_PCT		XDC_EQT_SRV
#define XDC_MAI_RAD		XDC_EQT_SRV
#define XDC_MAI_ANA		XDC_EQT_DAI
#define XDC_MAI_PAL		XDC_EQT_SRV
#define XDC_MAI_MODBAF	XDC_EQT_BAF
#define XDC_MAI_MODBAD	XDC_EQT_BAD
#define XDC_MAI_MODPRV	XDC_EQT_PRV 
#define XDC_MAI_SAGA	XDC_EQT_SAGA
#define XDC_MAI_CFE	XDC_EQT_SRV
#define XDC_MAI_CAP	XDC_EQT_SRV
#define XDC_MAI_IMU     XDC_EQT_SRV		

/* AAZ 30/08/2007 */
#define XDC_MAI_SIG     XDC_EQT_SIG
#define XDC_MAI_ECL     XDC_EQT_ECL
#define XDC_MAI_VEN     XDC_EQT_VEN
#define XDC_MAI_ENR     XDC_EQT_ENR
#define XDC_MAI_INC     XDC_EQT_INC
#define XDC_MAI_EXP     XDC_EQT_EXP
#define XDC_MAI_SYS     XDC_EQT_NIC
#define XDC_MAI_LTN     XDC_EQT_NIC
#define XDC_MAI_LIT     XDC_EQT_NIC
#define XDC_MAI_EHT     XDC_EQT_NIC

#define XDC_MAI_EQT_TUB     XDC_EQT_TUB
#define XDC_MAI_EQT_ECH     XDC_EQT_ECH

#define XDC_MAI_EQT_SIG     XDC_EQT_SIG
#define XDC_MAI_EQT_ECL     XDC_EQT_ECL
#define XDC_MAI_EQT_VEN     XDC_EQT_VEN
#define XDC_MAI_EQT_ENR     XDC_EQT_ENR
#define XDC_MAI_EQT_INC     XDC_EQT_INC
#define XDC_MAI_EQT_EXP     XDC_EQT_EXP
#define XDC_MAI_EQT_SYS     XDC_EQT_SYS
#define XDC_MAI_EQT_LTN     XDC_EQT_LTN
#define XDC_MAI_EQT_LIT     XDC_EQT_LI
#define XDC_MAI_EQT_EHT     XDC_EQT_HT

/* scenario de rentrer de la NAV en mode automatique */

#define XDC_NAV_RENTREE		"XXXX"	/************ a revoir par Denis Mismer ********/

/* sequence pour eteindre affichage tunnel et echangeur */

#define XDC_TUB_ETEINT		0	/************ a revoir par Denis Mismer ********/
#define	XDC_ECH_ETEINT		0	/************ a revoir par Denis Mismer ********/

/* differents types de point caracteristique */

#define XDC_POINT_CARACT_AUCUN			0
#define XDC_POINT_CARACT_ECHANGEUR		XDC_RES_ECHANGEUR	/* =  1 */
#define XDC_POINT_CARACT_PEAGE			XDC_RES_PEAGE		/* =  2 */
#define XDC_POINT_CARACT_LIT_ARRET		XDC_CAPT_LIT_ARRET	/* = 10 */
#define XDC_POINT_CARACT_AIRE			XDC_RES_AIRE		/* = 11 */
#define XDC_POINT_CARACT_BIFURCATION	XDC_RES_BIFURCATION	/* = 12 */
#define XDC_POINT_CARACT_TUBE			XDC_RES_TUBE		/* = 13 */
#define XDC_POINT_CARACT_BAT			XDC_RES_BAT			/* = 14 */

#define XDC_NOM_TYP_PTCAR_NUL			"Section courante"
#define XDC_NOM_TYP_PTCAR_ECH			"Echangeur"
#define XDC_NOM_TYP_PTCAR_PEA			"Peage"
#define XDC_NOM_TYP_PTCAR_LIT			"Lit d'arret"
#define XDC_NOM_TYP_PTCAR_AIR			"Aire"
#define XDC_NOM_TYP_PTCAR_BIF			"Bifurcation"
#define XDC_NOM_TYP_PTCAR_TUB			"Tunnel"
#define XDC_NOM_TYP_PTCAR_BAT			"Batiment"

/* position pazr defaut sur echangeur */

#define XDC_POINT_CARACT_POS_DEF	2

/* PR de la frontiere*/
#define XDC_PR_FRONTIERE		224000

/* différents etats d'actions */

#define XDC_ACT_ETAT_LANC		1
#define XDC_ACT_ETAT_LANC_FIN		2
#define XDC_ACT_ETAT_SUCCES		3
#define XDC_ACT_ETAT_ECHEC		4
#define XDC_ACT_ETAT_FIN		5
#define XDC_ACT_ETAT_SUCCES_FIN		6
#define XDC_ACT_ETAT_ECHEC_FIN		7

/* différents etats d'alerte (ne pas modifier, utilise pour replication) */

#define XDC_ALT_INCONNUE		0
#define XDC_ALT_FAUSSE			1
#define XDC_ALT_EXPLOITATION		2
#define XDC_ALT_VRAIE			3	/* vraie sans creation evt */
#define XDC_ALT_CONF_EVT		4	/* a confirmer vraie avec creation evt */
#define XDC_ALT_TEST			5	/* pour test */
#define XDC_ALT_VRAIE_EVT		10	/* vraie avec creation evt */
#define XDC_ALT_VRAIE_FUSION_FMC_SAE_EXT		11	/* vraie avec creation evt et lien FMC AVA */

/* differents etats d'un equipement au niveau de la base     */
/* et changement d'etat                                      */
/* (valeur correspond a un positionnement de bit pour l'etat */
/*  global de l'equipement et le changement global d'etat)   */

#define XDC_EQT_MINEUR		1
#define XDC_EQT_MAJEUR		2
#define XDC_EQT_CRITIQUE	4
#define XDC_EQT_DESACTIVE	8
#define XDC_EQT_HS		16
#define XDC_EQT_INHIBE		32
#define XDC_EQT_A_DESACTIVER	64
#define XDC_EQT_A_REACTIVER	128
#define XDC_EQT_NON_MINEUR	XDC_EQT_MINEUR
#define XDC_EQT_NON_MAJEUR	XDC_EQT_MAJEUR
#define XDC_EQT_NON_CRITIQUE	XDC_EQT_CRITIQUE
#define XDC_EQT_NON_DESACTIVE	XDC_EQT_DESACTIVE
#define XDC_EQT_NON_HS		XDC_EQT_HS
#define XDC_EQT_NON_INHIBE	XDC_EQT_INHIBE
#define XDC_EQT_NON_A_DESACTIVER XDC_EQT_A_DESACTIVER
#define XDC_EQT_NON_A_REACTIVER	XDC_EQT_A_REACTIVER

/* differents types d'action */

#define XDC_ACT_NAV		XDC_EQT_NAV	/* prevenir si modif valeurs de ces define */
#define XDC_ACT_PMV		XDC_EQT_PMV
#define XDC_ACT_TUNNEL		XDC_EQT_TUBE
#define XDC_ACT_DEPANNEUR	4
#define XDC_ACT_AFF_GARE	5
#define XDC_ACT_PAT_SIS		6
#define XDC_ACT_INTERVENTION	7
#define XDC_ACT_PMV_REMORQUE	8
#define XDC_ACT_PMV_FOURGON	9
#define XDC_ACT_TFM		10
#define XDC_ACT_ECHANGEUR	XDC_EQT_ECHANGEUR
#define XDC_ACT_FAX		12
#define XDC_ACT_TEL		13
#define XDC_ACT_BIP		14
#define XDC_APPEL_RADIO		15
#define XDC_ACT_PAU		16

#define XDC_ACT_APPEL		XDC_ACT_TEL	/* prevenir si modif valeurs de ces define */


#define XDC_ACT_FAC		XDC_EQT_FAC
#define XDC_ACT_FAU		XDC_EQT_FAU
#define XDC_ACT_PAL		XDC_EQT_PAL

#define XDC_ACT_EXP		50
#define XDC_ACT_EXPTFM		51
#define XDC_ACT_PMVDAT		52
#define XDC_ACT_DATEX           53  /* Ajout SAGA */

/* AAZ */
#define XDC_ACT_GTC_SIG         54
#define XDC_ACT_GTC_ECL         55
#define XDC_ACT_GTC_VEN         56
#define XDC_ACT_GTC_ENR         57
#define XDC_ACT_GTC_INC         58
#define XDC_ACT_GTC_EXP         59
#define XDC_ACT_GTC_SIG_ECH     60

#define XDC_ACT_CCH		64

#define XDC_ACT_SAGA		63
#define XDC_ACT_SMS		65
#define XDC_ACT_CFE		66
#define XDC_ACT_IMU		XDC_EQT_IMU		

#define XDC_SAGA_OFFSET_PMV	100 /*utilise par tdrol entre autres*/

#define XDC_SAGA_PMV_1_18	201
#define XDC_SAGA_PMV_2_18	202
#define XDC_SAGA_PMV_2_15	203
#define XDC_SAGA_PMV_3_15	204
#define XDC_SAGA_PMV_2_18_P	205
#define XDC_SAGA_PMV_3_15_P	206
#define XDC_SAGA_BAF		207
#define XDC_SAGA_BRA		208
#define XDC_SAGA_PMVA		209
#define XDC_SAGA_DAI		210
#define XDC_SAGA_CAMERA		211	
#define XDC_SAGA_RADT		212	
#define XDC_SAGA_META_EQT	213
#define XDC_SAGA_RAU		214
#define XDC_SAGA_SAV_1		215
#define XDC_SAGA_SAV_2		216
#define XDC_SAGA_SAV_3		217
#define XDC_SAGA_SAV_4		218
#define XDC_SAGA_SAV_5		219
#define XDC_SAGA_SAV_6		220

/* AAZ */
/*libelles de ces actions*/

#define XDC_ACT_LIB_NAV		"BRA"
#define XDC_ACT_LIB_PMV		"PMV"
#define XDC_ACT_LIB_TUNNEL	"Signalisation tunnel"
#define XDC_ACT_LIB_DEPANNEUR	"Depanneur"
#define XDC_ACT_LIB_AFF_GARE	"Affichage en gare"
#define XDC_ACT_LIB_PAT_SIS     "Patrouille Surveillance Reseau"	
#define XDC_ACT_LIB_INTERVENTION "Intervention"	
#define XDC_ACT_LIB_PMV_REMORQUE	"PMV sur remorque"
#define XDC_ACT_LIB_PMV_FOURGON "PMV sur fourgon"	
#define XDC_ACT_LIB_TFM		"Message RVA"
#define XDC_ACT_LIB_ECHANGEUR	"Echangeur"
#define XDC_ACT_LIB_FAX		"Envoi mail"
#define XDC_ACT_LIB_TEL		"Appel interlocuteur"
#define XDC_ACT_LIB_BIP		"Appel bip"
#define XDC_ACT_LIB_TELEPHONE	"Appel "
#define XDC_ACT_LIB_PAL		"Panneau d'alerte"
#define XDC_ACT_LIB_FAU		"Ferm. autoroute"
#define XDC_ACT_LIB_FAC		"Ferm. acces"
#define XDC_ACT_LIB_EXP		"Export intergestionnaires"
#define XDC_ACT_LIB_EXPTFM	"Export RVA"
#define XDC_ACT_LIB_PMVDAT	"PMV DATEX"
#define XDC_ACT_LIB_DATEX       "DATEX"   /* Ajout SAGA */
#define XDC_ACT_LIB_RADIO       "Appel Radio" 
#define XDC_ACT_LIB_SMS		"Envoi SMS"

#define XDC_ACT_LIB_APPEL	XDC_ACT_LIB_TEL	
#define XDC_ACT_LIB_SAGA	"SAGA"
#define XDC_ACT_LIB_CFE		"Controleur Feux"
#define XDC_ACT_LIB_IMU		"IMU"
#define XDC_ACT_LIB_PAU		"PAU SONO"

/* AAZ */
#define XDC_ACT_LIB_GTC_SIG	"Signalisation T"
#define XDC_ACT_LIB_GTC_SIG_ECH	"Signalisation E"
#define XDC_ACT_LIB_GTC_ECL	"Eclairage"
#define XDC_ACT_LIB_GTC_VEN	"Ventilation"
#define XDC_ACT_LIB_GTC_ENR	"Energie"
#define XDC_ACT_LIB_GTC_INC	"Protection incendie"
#define XDC_ACT_LIB_GTC_EXP	"Exploitation"

#define XDC_ACT_LIB_CCH		"Envoi Conditions Conduite" 

/* AAZ */
/* nombre de type d'actions */
#define XDC_ACT_NB_TYP		18	/* attention mettre a jour si modif de type d'action */

/* type d'intervenant */

#define	XDC_ACT_INT_POM		1
#define	XDC_ACT_INT_GND		2
#define	XDC_ACT_INT_AUTRE	3

/* attributs des messages TFM */

#define  XDC_TFM_MESSAGE         0		/* types (message / annotation) de (fragment de) texte */
#define  XDC_TFM_ANNOTATION      1

#define  XDC_TFM_MSG_MODIFIE	2		/* Indicateurs combinables des proprietes d'un message */

/* Niveaux de messages TFM */
#define XDC_SANS_NTFM		-1
#define XDC_NIV_TFM_MAX		255


/* sens de circulation */

#define XDC_SENS_INCONNU		0
#define XDC_LIB_SENS_INCONNU		"0"
#define XDC_LIB_SENS_NON_PREC		"Non precise"
#define XDC_LIB_2_SENS			"2 sens"

#define XDC_SENS_NORD			2	/* section courante (P.R. decroissants) */
#define XDC_LIB_SENS_NORD		"2"

#define XDC_SENS_SUD			1	/* section courante (P.R. croissants) */
#define XDC_LIB_SENS_SUD		"1"

#define XDC_SENS_SORTIE_NORD		4	/* aire ou echangeur */
#define XDC_LIB_SENS_SORTIE_NORD	"2, bretelle de sortie"

#define XDC_SENS_SORTIE_SUD		3	/* aire ou echangeur */
#define XDC_LIB_SENS_SORTIE_SUD		"1, bretelle de sortie"

#define XDC_SENS_ENTREE_NORD		8	/* aire ou echangeur */
#define XDC_LIB_SENS_ENTREE_NORD	"2, bretelle d'entree"

#define XDC_SENS_ENTREE_SUD		7	/* aire ou echangeur */
#define XDC_LIB_SENS_ENTREE_SUD		"1, bretelle d'entree"


/* Denomination alternative des sens de circulation */
#define XDC_LES_2_SENS			XDC_SENS_INCONNU
#define XDC_SENS_2			XDC_SENS_NORD
#define XDC_SENS_1			XDC_SENS_SUD
#define XDC_SORTIE_SENS_2		XDC_SENS_SORTIE_NORD
#define XDC_SORTIE_SENS_1		XDC_SENS_SORTIE_SUD
#define XDC_ENTREE_SENS_2		XDC_SENS_ENTREE_NORD
#define XDC_ENTREE_SENS_1		XDC_SENS_ENTREE_SUD


/* Types de sens de localisation (lies aux sens ci-dessus) */
#define XDC_ACCES_AUCUN			0
#define XDC_ACCES_SORTIE		2
#define XDC_ACCES_ENTREE		6


/* localisation non connue (pour le sens cf ci dessus) */

#define XDC_AUT_INCONNUE		0
#define XDC_PR_INCONNU			-1

/* sens des PR */

#define XDC_PR_NOR			1
#define XDC_PR_INV			-1

/* Format de la loacalisation */
#define XDC_FORMAT_LOCALISATION 	"%4s-%s-%7s"

/* Visibilite inconnue */

#define XDC_VISIBILITE_INCONNUE		-1

/* code d'erreur du a une verification de localisation */

#define XDC_LOC_SENS_MOD		1	/* sens incorrect */
#define XDC_LOC_PR_MOD			2	/* PR incorrect */
#define XDC_LOC_AUT_MOD			4	/* autoroute incorrecte */
#define XDC_LOC_INC			8	/* localisation inconnue */

/* distance de l'evenement */

#define XDC_DISTANCE_INCONNUE		-1



/* Etats d'une fiche main courante */

#define XDC_FMC_ETAT_PREV	1	/* en prevision */
#define XDC_FMC_ETAT_TRAI	2	/* a traiter */
#define XDC_FMC_ETAT_CLOS	3	/* clos */
#define XDC_FMC_ETAT_CLOS_PURGE	4	/* fmc close et purgee (utiliser par la synthese) */

/* Etats d'une fiche main courante A TRAITER */

#define XDC_FMC_ETA_PREVU	1	/* prevue, non debutee */
#define XDC_FMC_ETA_ENCOURS	2	/* debutee, non terminee */
#define XDC_FMC_ETA_TERMINE	3	/* date de fin renseignee */



/* criteres de recherche d'un evenement sur son type operation ou evenement */

#define XDC_TYP_FMC_EVT		1	/* evenement */
#define XDC_TYP_FMC_OPR		2	/* operation */
#define XDC_PAS_TYP_FMC		3	/* pas de recherche sur ce critere */

/* criteres de recherche d'un evenement sur son type de fmc */

#define XDC_TOUS_TYP_FMC	-1	/* pas de recherche sur ce critere */

/* numero du bit correspond aux differents districts et CI utilises */
/* pour la creation de criteres de recherche */

#define XDC_HIS_EXP_CI		1 	/*Pour CI et CA*/
#define XDC_HIS_EXP_VC		2
#define XDC_HIS_EXP_DP		4

/* categorie des elements contenus dans l'historique d'exploitation */

#define XDC_HIS_EXP_EVT		0
#define XDC_HIS_EXP_ACT		1

/* differents types de voie pour les synoptiques */

#define XDC_VOIE_INCONNUE	0
#define XDC_VOIE_SENS_NORMAL	1
#define XDC_VOIE_SENS_INVERSE	2
#define XDC_VOIE_BLOQUEE	3
#define XDC_VOIE_INEXISTANTE	4
#define XDC_VOIE_NEUTRALISEE	5
#define XDC_VOIE_BLOQUEE_NEUTRALISEE 6
#define XDC_VOIE_BAU_NORMALE	7
#define XDC_VOIE_BAU_ETROITE	16

/* textes des reveils */

#define XDC_REV_URGENT	"!"				/* marque caracterisant un reveil a traiter immediatement */
#define XDC_REV_MSG	">"				/* marque caracterisant un message sous forme de reveil */
#define XDC_REV_TXT_SEP	"#"				/* caractere separateur de champs pour textes de reveils */

#define XDC_REV_TDP_SURCHARGE	"SURCHARGE TDP"
#define XDC_REV_VOL_EQT	"L'equipement "			/* pour les actions sur eqt */
#define XDC_REV_VOL_EQT_SUITE	" est requisitionne"	/* pour les actions sur eqt */
#define XDC_REV_CMD_EQT	"Echec commande "		/* pour les actions sur eqt */
#define XDC_REV_FMC_CRE	"NOUVELLE FMC"			/* pour la validation de FMC */
#define XDC_REV_FMC_MOD	"MODIFICATION FMC"		/* pour la validation de FMC */
#define XDC_REV_FMC_FIN	"FIN FMC"			/* pour la validation de FMC */
#define XDC_REV_FMC_REC	"FMC SUR ZONE RECOUVREMENT"	/* pour la validation de FMC */
#define XDC_REV_FMC_ENR	"VOUS POUVEZ ENRICHIR"		/* pour le transfert de droit d'enrich de FMC */
#define XDC_REV_FMC_PAS_ENR	"VOUS AVEZ PERDU LE DROIT D'ENRICHIR"		/* pour le transfert de droit d'enrich de FMC */
#define XDC_REV_FMC_AUTO_T1	"VOUS N'AVEZ PAS CONSULTE LA FMC PENDANT "	/* reveil automatique sur fmc */
#define XDC_REV_FMC_AUTO_T2	"L'HEURE DE DEBUT PREVU EST ATTEINTE"		/* reveil automatique sur fmc */
#define XDC_REV_FMC_AUTO_T3	"L'HEURE DE FIN PREVUE EST ATTEINTE"		/* reveil autmatique sur fmc */
#define XDC_REV_FMC_AUTO_T4	"VOUS N'AVEZ JAMAIS CONSULTE CETTE FMC"		/* reveil autmatique sur fmc */
#define XDC_REV_FMC_AUTO_T5	"VOUS N'AVEZ PAS CONSULTE LA FMC DEPUIS QUE VOUS AVEZ LE DROIT D'ENRICHIR"		/* reveil autmatique sur fmc */
#define XDC_REV_LIB_EQT		"L'equipement "	/* pour la liberation d'eqt suite a une action terminee */
#define XDC_REV_LIB_EQT_SUITE	" est libere"	/* pour la liberation d'eqt suite a une action terminee */
#define XDC_REV_DSP_EQT		"L'equipement "	/* pour la redisponibilite de l'eqt suite a fin d'alarme */
#define XDC_REV_DSP_EQT_SUITE	" est redevenu disponible"	/* pour la redisponibilite de l'eqt suite a fin d'alarme */

/* messages d'information transmis au moyen de reveils mais non affiches */

#define XDC_REV_FMC_RESERVEE	"FMC RESERVEE"		/* pour la gestion des droits d'enrichir les FMC */

/* valeurs a utiliser pour indiquer la non valuation d'une mesure RADT */

#define XDC_RADT_V_INCONNU	255
#define XDC_RADT_Q_INCONNU	-1
#define XDC_RADT_TO_INCONNU	255
#define XDC_RADT_PL_INCONNU	255
#define XDC_RADT_CUMUL_INCONNU	-1
#define XDC_RADT_NBPL_INCONNU	-1
#define XDC_RADT_NIV_INCONNU	0
#define XDC_RADT_MES_INCONNU	0	/* mesure inexistante pour une courbe  de trafic */
#define XDC_RADT_DIV_INCONNU	-1

/* valeurs pour le niveau de trafic */

#define XDC_RADT_FLUIDE	1	/* trafic fluide */
#define XDC_RADT_PRESAT	2	/* trafic presature */
#define XDC_RADT_SATURE	3	/* trafic sature */

/* valeurs de la validite de Q, V, TO, PL pour les mesures de trafic */

#define XDC_RADT_RECONST	0	/* mesure reconstituee */
#define XDC_RADT_CORRECT	1	/* mesure correcte */
#define XDC_RADT_INCONNU	2	/* mesure inexistante */

/* type de courbe pour le trafic */

#define XDC_RADT_V		1
#define XDC_RADT_Q		2
#define XDC_RADT_TO		3
#define XDC_RADT_PL		4
#define XDC_RADT_NIV		5
#define XDC_RADT_Q_VL		6	/* pseudo-mesure (calculee) */
#define XDC_RADT_Q_PL		7	/* pseudo-mesure (calculee) */

/* Nb de classe par type de pesage */

#define XDC_NB_CLASSE_SILHOUETTE	14
#define XDC_NB_CLASSE_ESSIEU		12
#define XDC_NB_CLASSE_POIDS		6

#define XDC_MESURE_SILHOUETTE		1
#define XDC_MESURE_ESSIEU		2
#define XDC_MESURE_POIDS		3


/* type de fenetre pour affichage des courbe */
#define XDC_GR_FENETRE_GAUCHE 1
#define XDC_GR_FENETRE_DROITE 2

/* type de courbe Q V TO */
#define XDC_GR_Q_V	1
#define XDC_GR_V_TO	2
#define XDC_GR_Q_TO	3


/* type de courbe trafic */
#define XDC_GR_INRETS		1
#define XDC_GR_INRETS_BOUCHON	2
#define XDC_GR_BOUCHON		3

/* noms des serveurs */

#define XDC_PREF_SQL	"SQL_"
#define XDC_SQLCI	"SQL_CI"
#define XDC_SQLCA	"SQL_CA"
#define XDC_SQLDP	"SQL_DS"
#define XDC_SQLVC	"SQL_DM"
#define XDC_SQLHIS	"SQL_HIS"

/* noms des sites */

#define XDC_NOM_SITE_CI	"CI"
#define XDC_NOM_SITE_CA	"CA"
#define XDC_NOM_SITE_DP "DP"
#define XDC_NOM_SITE_VC "VC"
#define XDC_NOM_SITE_HIS "HIS"
#define XDC_NOM_SITE_ASF "ASF"

/* numeros des sites des serveurs */

#define XDC_CI	43
#define XDC_CA	15
#define XDC_DP	26
#define XDC_VC	25
#define XDC_HIS 0
#define XDC_DDE83       83 /* Ajout SAGA */
#define XDC_ASF 201

#define XDC_SITE_INCONNU	(-1)

/* Numero de code des numero d'appel par district */

#define XDC_APPEL_CI 1
#define XDC_APPEL_DP 5
#define XDC_APPEL_VC 1
#define XDC_APPEL_CA 1


/* noms des districts */

#define XDC_NOM_DISTRICT_VC	"Var Cote d'Azur"
#define XDC_NOM_DISTRICT_DP	"Durance Provence"
#define XDC_NOM_DISTRICT_CA	"Angers"

/* Valeurs generales d'actions ou de resultats d'actions utilisees notamment pour
   configuration des bases de donnees, rafraichissement des IHM synoptique et textuelle ... */

#define XDC_LU		0
#define XDC_AJOUT	1
#define XDC_MODIF	2
#define XDC_SUPPR	3 /* conserver SUPPR > MODIF > AJOUT */
#define XDC_ECHEC	4
#define XDC_SUCCES	5
#define XDC_TERMINE	6


#define XDC_TRAIT_BLOC  2
/* scenario et sequences de fin pour tunnel, echangeur, NAv */

#define	XDC_SCN_LIB_NAV	"LEVR"
#define	XDC_SEQ_OUV_TUN	2
#define	XDC_SEQ_OUV_ECH	2

/* Commandes d'etats d'un equipement */
#define XDC_EQT_HORS_SRV	XDC_EQT_HS			/* Mise hors service */
#define XDC_EQT_EN_SRV		(XDC_EQT_HS ^ 0xFFFF)		/* Mise en service */
#define XDC_EQT_INACTIF		XDC_EQT_DESACTIVE		/* Desactivation */
#define XDC_EQT_ACTIF		(XDC_EQT_DESACTIVE ^ 0xFFFF)	/* Activation */
#define XDC_EQT_ETAT_INHIBE	XDC_EQT_INHIBE			/* Inhibition */
#define XDC_EQT_ETAT_NON_INHIBE	(XDC_EQT_INHIBE ^ 0xFFFF)	/* Non Inhibition */

/* Constantes d'abonnement aux donnees EQUEXT */

#define	XDC_DEBUT_ABONNEMENT_EQUEXT	1
#define	XDC_FIN_ABONNEMENT_EQUEXT	2
#define	XDC_FERMER_ABONNEMENT_EQUEXT	3


/* noms des repertoires */

#define XDC_MACHINE_GECONF	"PODFA"

#define XDC_PATH_MIGRAZURHOME       "/produits/migrazur/appliSD"
#define XDC_PATH_LIVRAISON          "."
/*#define XDC_PATH_IMAGE       	     XDC_PATH_MIGRAZURHOME "/config/fichiers/adm"*/
#define XDC_NSUP_PATHCONF           XDC_PATH_MIGRAZURHOME "/config/fichiers/adm"
#define XDC_NSUP_PATHOPER           XDC_PATH_MIGRAZURHOME "/fichiers/adm"
#define XDC_NSUP_PATHEXEC           XDC_PATH_MIGRAZURHOME "/exec"
#define XDC_NSUP_PATHBL        	    XDC_PATH_MIGRAZURHOME "/fichiers/adm"
#define XDC_NSUP		    "nsup"
#define XDC_PATHREF                 "/users/gesconf/migrazur/reference"
#define XDC_PATHBIN                 "/users/gesconf/bin"
#define XDC_PATHREFFIC              XDC_PATHREF "/fichiers"
#define XDC_PATHREFFICDYN           XDC_PATHREF "/fichiers/dyn"
#define XDC_PATHREFFICINF           XDC_PATHREF "/fichiers/inf"
#define XDC_PATHREF_FICADM           XDC_PATHREF "/fichiers/adm"

#define XDC_PATHCNF                 "/produits/migrazur/appliSD/config"
#define XDC_PATHCNFEXE              XDC_PATHCNF "/exec"
#define XDC_PATHCNFFIC              XDC_PATHCNF "/fichiers"
#define XDC_PATHCNFFICDYN           XDC_PATHCNF "/fichiers/dyn"

#define XDC_PATHTRAV                 "/produits/migrazur/appliSD/modif"
#define XDC_PATHTRAVFIC 	     XDC_PATHTRAV "/fichiers"
#define XDC_PATHTRAVFICADM	     XDC_PATHTRAVFIC "/adm"

#define XDC_PATHEXE                 "/produits/migrazur/appliSD/exec"
#define XDC_PATHFIC                 "/produits/migrazur/appliSD/fichiers"
#define XDC_PATHFICDYN              XDC_PATHFIC "/dyn"
#define XDC_PATH_BDC_COURANTE	    XDC_PATHFIC "/bdc"
#define XDC_PATH_DEQ		    XDC_PATHFIC "/deq"
#define XDC_PATH_EQA		    XDC_PATHFIC "/eqa"
#define XDC_PATH_INF		    XDC_PATHFIC "/inf"
#define XDC_PATH_SYN		    XDC_PATHFIC "/syn"
#define XDC_PATH_NTFM		    XDC_PATHFIC "/ntfm"
#define XDC_PATH_STRADA		    XDC_PATHFIC "/stra"
#define XDC_PATH_DRA		    XDC_PATHFIC "/dra"
#define XDC_PATH_ADA		    XDC_PATHFIC "/ada"

/* PATH pour le fichier de bilan de configuration SOUS-SYS GECONF */
#define XDC_BILAN_CONFIG_PATH	    XDC_PATHCNFFICDYN

#define XDC_PATH_BDC_FUTURE	    XDC_PATHCNFFIC "/bdc"
#define XDC_PATHCMPT			XDC_PATH_MIGRAZURHOME "/fichiers/compteur"

/*#define XDC_FICHIER_ASTREINTES_BC   " " */

/* Repertoire Temporaire pour le fichier d'Administartion */
#define XDC_REP_TMP             "/tmp"

/* Repertoire executable RTWorks */
#define XDC_PATH_RTWORKS	"/produits/rtworks30/bin/hp700_hpux_9_0"


/* Combiner les valeurs par des additions et non des "ou logiques" pour Applix */
#define XDC_ETAT_TOUS_EQT	(XDC_EQT_MINEUR + XDC_EQT_MAJEUR + XDC_EQT_CRITIQUE + XDC_EQT_DESACTIVE + XDC_EQT_HS + XDC_EQT_INHIBE)



/********** Taille des donnees *****************/

#define XDC_LGR_STD_LIG_PMV	15
#define XDC_LGR_MAX_LIG_PMV	18
#define XDC_LGR_PICTO		10
#define XDC_LGR_NOM_PICTO	XDC_LGR_PICTO
#define XDC_LGR_TXT_PICTO	12
#define XDC_LGR_LIG_PMVA	15
#define XDC_LGR_LIG_PRV		15
#define XDC_LGR_NOM			25
#define XDC_LGR_NOM_MACHINE	10
#define XDC_LGR_NOM_SITE	2
#define XDC_LGR_NOM_AUTO	4
#define XDC_LGR_NOM_EQT		10
#define XDC_LGR_RGS			3
#define XDC_LGR_CODE_EQT	3
#define XDC_LGR_CODE_CAM	4
#define XDC_LGR_BASEDD		3
#define XDC_LGR_SCENARIO	4
#define XDC_LGR_ECH			30

#define XDC_Lon_Sortie		20
#define XDC_Lon_AdresseIP	20
#define XDC_Lon_Protocole	10
#define XDC_Lon_Encoder		20

#define XDC_LGR_Picto_PIC	4
#define XDC_LGR_Bandeau_PIC	4

/******************************************************************************
**                             Non Applix - debut                            **
******************************************************************************/

/********** Longueur des tableaux de caracteres *****************/

#define XDC_Lon_Ligne_PMV			19
#define XDC_Lon_Ligne_PMVA			16
#define XDC_Lon_Ligne_Eqt_SAGA		19
#define XDC_Lon_Ligne_PRV			16
#define XDC_Lon_Nom 				26
#define XDC_Lon_NomMachine			11
#define XDC_Lon_NomSite				3
#define XDC_Lon_NomAuto				5
#define XDC_Lon_NomTache			20
#define XDC_Lon_Datagroup			61
#define XDC_Lon_NomSysteme			10
#define XDC_Lon_TexteAlerte			101
#define XDC_Lon_Phrase				51
#define XDC_Lon_Commentaire			251
#define XDC_Lon_NomEqt				11
#define XDC_Lon_Tel					17
#define XDC_Lon_Picto				11
#define XDC_Lon_TexteAnn			101
#define XDC_Lon_Rgs					4
#define XDC_Lon_NomFich				11
#define XDC_Lon_Abrev				16
#define XDC_Lon_CodeEqt				4
#define XDC_Lon_CodeCam				5
#define XDC_Lon_Login				9
#define XDC_Lon_BaseDD				4
#define XDC_Lon_TxtPicto			13
#define XDC_Lon_NomPicto			11
#define XDC_Lon_Texte				251
#define XDC_Lon_Scenario			5
#define XDC_Lon_MatDang				7
#define XDC_Lon_TexteVeh			101
#define XDC_Lon_TextePicto			XDC_Lon_TxtPicto
#define XDC_Lon_EtatCom				32	
#define XDC_Lon_MsgES80				81	
#define XDC_Lon_TypeFich			5
#define XDC_Lon_Matrice				5
#define XDC_Lon_Params_Matrice		32
#define XDC_Lon_Texte_Alarme		26
#define XDC_Lon_TrameMesure			11
#define XDC_Lon_Texte_SATIR			256
#define XDC_Lon_TexteReveil			101
#define XDC_Lon_Ech					31
#define XDC_Lon_TypeAppelRadio		3
#define XDC_Lon_GroupeRadio			10
#define XDC_Lon_Radio      			25
#define XDC_Lon_VitesseRegul		5
#define XDC_Lon_Bandeau				18
#define XDC_Lon_Bandeau_PIC			5
#define XDC_Lon_Picto_PIC			5
#define XDC_Lon_ZoneRegul			50
#define XDC_Lon_LibellePlanDeFeux	150
#define	XDC_Lon_ChaineIMU			1024
#define XDC_Lon_MessageSono			31

/******************************************************************************
**                              Non Applix - fin                             **
******************************************************************************/


/*#define XDC_Lon_Nb_Mod_Nav	22*/
#define XDC_Lon_Nb_Mod_Nav	35
#define XDC_Lon_Nb_Mod_Pal	8
#define XDC_Lon_Nb_Mod_BAF	6
#define XDC_Lon_Nb_Mod_BAD	2
#define XDC_Lon_Nb_Mod_PRV	2


/******  Constantes PMV *****************************************/

#define XDC_PMV_NON_EXIST	0	/* Quatrieme ligne inexistante		*/
#define XDC_PMV_TEXTE		1	/* Un texte sera affiche sur le PMV */
#define XDC_PMV_TEMPERATURE	2	/* Une temperature sera affiche sur le PMV */
#define XDC_PMV_HEURE		3	/* L'heure sera affiche sur le PMV */
#define XDC_PMV_ETEINDRE	4	/* Extinction du PMV */
#define XDC_PMV_DEFAUT		5	/* affichage par defaut du PMV */
#define XDC_PMV_TDP		6	/* affichage temps de parcours sur PMV */
#define XDC_PMV_TDP_RAF		7	/* rafraichissement temps de parcours sur PMV */
#define XDC_PMV_3POINTS		8	/* affichage trois points de suspension*/
#define XDC_PMV_PASCLIGNO	1	/* Pas de ligne clignotante */
#define XDC_PMV_CLIGNOLENT	2	/* Clignotement lent sur une ligne PMV */
#define XDC_PMV_CLIGNORAPIDE	3	/* Clignotement rapide sur une ligne PMV */
#define XDC_PMV_PASFLASH	1	/* Flash PMV desactive */
#define XDC_PMV_FLASH		2	/* Flash PMV active */
#define XDC_PMV_FLASHABSENT	0	/* Flash non instale sur PMV */
#define XDC_PMV_FLASHPRESENT	1	/* Flash instale sur PMV */
#define XDC_PMVA_TEMPERATURE	2

/******  Constantes PICTO *****************************************/

#define XDC_PICTO_PASCLIGNO	XDC_PMV_PASCLIGNO	/* Pas de picto clignotant */
#define XDC_PICTO_CLIGNO	XDC_PMV_CLIGNOLENT	/* picto clignotant */
#define XDC_PICTO_PASPICTO	"XXXX"			/* picto eteint */

/******  Constantes de mode d'affichage PMV (EXCLUSIFS) ***********/

#define XDC_PMV_MODE_NORMAL	XDC_PMV_PASCLIGNO	/* Mode normal d'affichage, constant */
#define XDC_PMV_MODE_CLIGNO	XDC_PMV_CLIGNORAPIDE	/* Mode d'affichage clignotant */
#define XDC_PMV_MODE_ALTERNE	11			/* Mode d'affichage alterne */


/******  Constantes Video *****************************************/

#define XDC_ENREG_EN_COURS	1
#define XDC_AUCUN_ENREG		2
#define XDC_SATURE	        3			
#define XDCC_NON_SATURE	        4

/* defs. type camera */
#define XDC_MOBILE		1
#define XDC_FIXE		2
#define XDC_CYCLE		3
#define XDC_POSI		4

/******  Constantes Contraste *************************************/

#define XDC_CONTRASTENORMAL	1	/* Contraste Normal */
#define XDC_CONTRASTEBRILLANT	2	/* Surbrillance */

/******  Constantes GTC NICE **************************************/

#define XDC_GTC_CAPTEUR		XDC_EQT_CPT
#define XDC_GTC_TELEALARME	XDC_EQT_CPT - 1
#define XDC_GTC_ETOR		XDC_EQT_CPT - 2
#define XDC_GTC_PANNEAU		XDC_EQT_PAN
#define XDC_GTC_DISCORDANCE	XDC_EQT_PAN + 1
#define XDC_GTC_SEQUENCE	XDC_EQT_PAN + 2

/* AAZ 25/07/2007 */
#define XDC_GTC_EQUIPEMENT  XDC_EQT_EQT
#define XDC_GTC_DOMAINE     XDC_EQT_DOMAINE


/******  Constantes Controleur de Feux ****************************/
#define XDC_CFE_FCT_NUL		'X'
#define XDC_CFE_FCT_ETEINT	'A'
#define XDC_CFE_FCT_CLIGNO	'B'
#define XDC_CFE_FCT_INIT	'C'

#define XDC_CFE_PLN_NUL		0


/****** Definition des niveaux de traces en fonction de la severite ****/
/****** d erreur rendu par le sql Serveur ou l Open Client          ****/

#define XDC_SGBD_WARNING	1
#define XDC_SGBD_FATAL		17



/******  Constantes Temps de Parcours et Destinations *********************/

#define XDC_DTP_AUCUNE		0

/* type de franchissement de seuil pour temps de parcours*/
#define XDC_SEUIL_TDP_ABSENT	0	/*pas de franchissement de seuil*/
#define XDC_SEUIL_TDP_BAS	1	/*franchissement de seuil bas*/
#define XDC_SEUIL_TDP_HAUT	2	/*franchissement de seuil haut*/

/*distance min entre PMV et destination*/
#define XDC_DISTANCE_TDP 	2500
#define XDC_DESTINATION_PROCHE 		1
#define XDC_DESTINATION_LOINTAINE	2
#define XDC_DESTINATION_NOEUD		3
#define XDC_DESTINATION_TRAVERSEE	99

/*distance min entre PMA et destination*/
#define XDC_DEST_UN_PROCHE 		1
#define XDC_DEST_UN_LOIN 		2
#define XDC_DEST_DEUX_PROCHE	4
#define XDC_DEST_DEUX_LOIN		5

/*type de zone de parcours*/
#define XDC_ZDP_TDP_SECTION	1	/*zdp sur section courante*/
#define XDC_ZDP_TDP_FOURCHE	0	/*zdp sur fourche*/




/*definition des familles*/
#define XDC_NOMBRE_FAMILLES	36

#define XDC_FAM_ADMINISTRATION		1
#define XDC_FAM_DEVEL_CONFIG		2
#define XDC_FAM_SYSTEME			3
#define XDC_FAM_EXPLOITATION		4
#define XDC_FAM_EXPLOITATION_DISTRICT	5
#define XDC_FAM_EXPLOITATION_CI		6
#define XDC_FAM_RADIO_TRAFIC_FM		7
#define XDC_FAM_ADM_ANNUAIRE		8
#define XDC_FAM_CONSULT_ANNUAIRE	9
#define XDC_FAM_VISU_SYNOPTIQUES	10
#define XDC_FAM_VISU_COURBES		11
#define XDC_FAM_VISU_STATS		12
#define XDC_FAM_VISU_EXPLOITATION	13
#define XDC_FAM_COMM_CONFIG_MAINTENANCE	14
#define XDC_FAM_MAINTENANCE		15
#define XDC_FAM_LCR			16
#define XDC_FAM_ETUDIANT		17
#define XDC_FAM_TOUT_LE_MONDE		18
#define XDC_FAM_ENSEIGNANT		19
#define XDC_FAM_PREVISION		20
#define XDC_FAM_MODIF_HISTORIQUE	21
#define XDC_FAM_PLAN_SECOURS		22
#define XDC_FAM_OUTILS_TABLEUR		23
#define XDC_FAM_OUTILS_INFOCENTRE	24
#define XDC_FAM_PILOTAGE_MANUEL_NAV	25
#define XDC_FAM_MATRICE_NAV		26
#define XDC_FAM_CONFIG_CAMERA		27
#define XDC_FAM_DEV_MIGRAZUR		28
#define XDC_FAM_PC_SIMPLIFIE		29
#define XDC_FAM_HISTO_PEAGE		30
#define XDC_FAM_EXPLOITATION_PC2	31
#define XDC_FAM_ACHEVER_PC2		32
#define XDC_FAM_VISU_REGULATION		33
#define XDC_FAM_ADM_REGULATION		34
#define XDC_FAM_PILOTAGE_PRV		35
#define XDC_FAM_PILOTAGE_REGUL		36

/*constantes REGULATION*/
/*definition des PARAM REG*/

#define XDC_REG_PARAM_GENERAUX	1
#define XDC_REG_PARAM_PARAM		2
#define XDC_REG_PARAM_TERDT		3
#define XDC_REG_PARAM_DETECTION	4
#define XDC_REG_PARAM_AJUSTEMENT	5
#define XDC_REG_PARAM_VALIDATION	6
#define XDC_REG_PARAM_APPLICATION	7
#define XDC_REG_PARAM_SIMULATION	8

#define XDC_PARAM_GEN	44

#define XDC_REG_NIVEAU_AUTO_SECTEUR_PR		1
#define XDC_REG_NIVEAU_AUTO_SECTEUR_VE		2
#define XDC_REG_NIVEAU_AUTO_SECTEUR_CA		3
#define XDC_REG_NIVEAU_AUTO_SECTEUR_VD		4
#define XDC_REG_DISTANCE_PRV_CRIT			5
#define XDC_REG_DECRE_VIT_PRVAMONT_DEBCH	6
#define XDC_REG_ACTIVE_VAL_NEG				7
#define XDC_REG_ACTIVE_DEB_EXCESS			8
#define XDC_REG_ACTIVE_TAUX_EXCESS			9
#define	XDC_REG_ACTIVE_QTO_NULS				10
#define XDC_REG_ACTIVE_QMAX_TONULS			11
#define XDC_REG_ACTIVE_SEUIL_NUL			12
#define XDC_REG_ACTIVE_SEUIL_MIN			13
#define XDC_REG_SEUIL_DEBIT_MAX				14
#define XDC_REG_SEUIL_DEBIT_NUL				15
#define XDC_REG_SEUIL_TO_INVALIDE			16
#define XDC_REG_NBMAX_CYCLE_DEB_NUL			17
#define XDC_REG_DEB_CHAUSSEE_MIN			18
#define XDC_REG_HEURE_DEB_JOUR				19
#define XDC_REG_HEURE_FIN_JOUR				20
#define	XDC_REG_COEF_CALCUL_PENTE			21
#define XDC_REG_FREQ_FONCT_MODU_AJUST		22
#define XDC_REG_DECR_VIT_FINCHANT_PRV_AVAL_PLEIN_VOIE			23
#define XDC_REG_DELAI_LAT_DETEC_EVOL		24
#define XDC_REG_DELAI_LAT_GEN_ALERTE		25
#define XDC_REG_DUREE_APP_SCEN_FORCE		26
#define XDC_REG_DELAI_NON_REPONSE_TEPRV		27
#define XDC_REG_DELAI_LAT_TEPRV_PERTE_REGUl	28
#define XDC_REG_DELAI_RECO_SERV_FAX_MESS	29
#define XDC_REG_CADENCE_MODE_SIMU			30
#define	XDC_REG_COEF_CALCUL_PENTE_FIN		31
#define XDC_REG_DELAI_LAT_TERDT				32

#define XDC_REG_SIMU_DEJA_EN_COURS	1
#define XDC_REG_SIMU_TERMINEE		2

#define XDC_REG_LONG_NOM_PARAM		100

#define XDC_REG_CONDITION_TRAF_ACTIV	0
#define XDC_REG_CONDITION_TRAF_DESACTIV	1
#define XDC_REG_CONDITION_TRAF_HORAIRE	2

#define XDC_REG_OPERATEUR_INF	0
#define XDC_REG_OPERATEUR_SUP	1
#define XDC_REG_OPERATEUR_EGAL	2

#define XDC_REG_DONNEE_Q	0
#define XDC_REG_DONNEE_V	1
#define XDC_REG_DONNEE_PENTE	2
#define XDC_REG_DONNEE_TO	3

#define XDC_REG_HORAIRE_HORODATE	0	/*plage*/
#define XDC_REG_HORAIRE_HEURE_JOUR	1	/*lundi 8h 12h*/
#define XDC_REG_HORAIRE_HEURE	2		/*12h 14h ts les jours*/
#define XDC_REG_HORAIRE_TYPE_HEURE	3	/*week ends 8h 10h*/

#define XDC_REG_JOURS_OUVRES	8
#define XDC_REG_JOURS_WE	9

#define XDC_REG_MODE_MANUEL	2
#define XDC_REG_MODE_SEMIAUTO	1
#define XDC_REG_MODE_AUTO 	0

#define XDC_REG_MASK_CONTEXT_CHANTIERS				1
#define XDC_REG_MASK_CONTEXT_DECL_HORAIRE			2
#define XDC_REG_MASK_CONTEXT_EVT_IMPACTANTS			4
#define XDC_REG_MASK_CONTEXT_PANNE_PRV_CRITIQUE		8
#define XDC_REG_MASK_CONTEXT_PANNE_PRV_NON_CRITIQUE	16
#define XDC_REG_MASK_CONTEXT_QUALIF_OPERATEUR		32
#define XDC_REG_MASK_CONTEXT_SCENARIO_IDEAL			64

/******  Constantes Contexte ********************************************/

#define	XDC_JOUR		1	/* Il fait jour ! */
#define	XDC_NUIT		2	/* Il fait nuit ! */

/********** Constantes indiquant basculement de debut ou de fin de travaux 
            XDY_booleen                             *************************/
#define XDC_DEBUT_TRAVAUX       0
#define XDC_FIN_TRAVAUX         1



/********** Constantes de Type d'objet Mobile ********************/
#define XDC_SYN_OBJ_INCONNU		-1
#define XDC_SYN_TRAVAUX_MOBILES_PREVUS 	0
#define XDC_SYN_TRAVAUX_MOBILES_REELS 	1
#define XDC_SYN_TRAVAUX_FIXES_PREVUS 	2
#define XDC_SYN_TRAVAUX_FIXES_REELS 	3
#define XDC_SYN_INTERVENANT_GENDARMERIE 4
#define XDC_SYN_INTERVENANT_POMPIER 	5
#define XDC_SYN_INTERVENANT_DEPANNEUR 	6
#define XDC_SYN_INTERVENANT_FOURGON_SIS 7
#define XDC_SYN_INTERVENANT_PAT_ESCOTA 	8
#define XDC_SYN_PMV_MOBILE 		9
#define XDC_SYN_FMC_ACCIDENT 		10
#define XDC_SYN_FMC_VEHICULE_CONTRE_SENS 11
#define XDC_SYN_FMC_VEHICULE_ARRETE 	12
#define XDC_SYN_FMC_VEHICULE_PANNE 	13
#define XDC_SYN_FMC_VEHICULE_FEU 	14
#define XDC_SYN_FMC_CIRQUE 		15
#define XDC_SYN_FMC_CONVOI_EXCEPTIONNEL 16
#define XDC_SYN_FMC_CONVOI_MILITAIRE 	17
#define XDC_SYN_FMC_VEHICULE_LENT 	18
#define XDC_SYN_FMC_MANIFESTATION 	19
#define XDC_SYN_FMC_GREVE_ESCOTA 	20
#define XDC_SYN_FMC_OPERATION_ESCARGOT 	21
#define XDC_SYN_FMC_INCIDENT_SOUS_CONSC 22
#define XDC_SYN_FMC_EBOULEMENT 		23
#define XDC_SYN_FMC_INCENDIE 		24
#define XDC_SYN_FMC_INONDATION 		25
#define XDC_SYN_FMC_OBSTACLE 		26
#define XDC_SYN_FMC_CHAUSSEE_DETERIOREE 27
#define XDC_SYN_FMC_FLAQUE_HUILE 	28
#define XDC_SYN_FMC_ANIMAL_ERRANT 	29
#define XDC_SYN_FMC_FRONTIERE_FERMEE 	30
#define XDC_SYN_FMC_ALTERCATION_USAGERS 31
#define XDC_SYN_FMC_AGRESSION 		32
#define XDC_SYN_FMC_MALAISE_USAGER 	33
#define XDC_SYN_FMC_HOLD_UP 		34
#define XDC_SYN_FMC_PIETON_CHAUSSEE 	35
#define XDC_SYN_FMC_AUTRE 		36
#define XDC_SYN_TRAVAUX_MOBILES_TETE_PREVUS 40
#define XDC_SYN_TRAVAUX_MOBILES_TETE_REELS 41
#define XDC_SYN_TRAVAUX_MOBILES_QUEUE_PREVUS 42
#define XDC_SYN_TRAVAUX_MOBILES_QUEUE_REELS 43
#define XDC_SYN_GARE			44
#define XDC_SYN_FMC_ACCIDENT_VL 	45
#define XDC_SYN_FMC_ACCIDENT_PL 	46
#define XDC_SYN_FMC_ACCIDENT_VLPL 	47
#define XDC_SYN_FMC_ACCIDENT_CAR 	48
#define XDC_SYN_FMC_ACCIDENT_VLCAR 	49
#define XDC_SYN_FMC_ACCIDENT_PLCAR 	50
#define XDC_SYN_FMC_ACCIDENT_VLPLCAR 	51
#define XDC_SYN_FMC_ACCIDENT_REM 	52
#define XDC_SYN_FMC_ACCIDENT_VLREM 	53
#define XDC_SYN_FMC_ACCIDENT_PLREM 	54
#define XDC_SYN_FMC_ACCIDENT_VLPLREM 	55
#define XDC_SYN_FMC_ACCIDENT_CARREM 	56
#define XDC_SYN_FMC_ACCIDENT_VLCARREM 	57
#define XDC_SYN_FMC_ACCIDENT_PLCARREM 	58
#define XDC_SYN_FMC_ACCIDENT_VLPLCARREM 59
#define XDC_SYN_FMC_ACCIDENT_MOTO 	60
#define XDC_SYN_FMC_ACCIDENT_VLMOTO 	61
#define XDC_SYN_FMC_ACCIDENT_PLMOTO 	62
#define XDC_SYN_FMC_ACCIDENT_VLPLMOTO 	63
#define XDC_SYN_FMC_ACCIDENT_CARMOTO 	64
#define XDC_SYN_FMC_ACCIDENT_VLCARMOTO 	65
#define XDC_SYN_FMC_ACCIDENT_PLCARMOTO 	66
#define XDC_SYN_FMC_ACCIDENT_VLPLCARMOTO 	67
#define XDC_SYN_FMC_ACCIDENT_REMMOTO 	68
#define XDC_SYN_FMC_ACCIDENT_VLREMMOTO 	69
#define XDC_SYN_FMC_ACCIDENT_PLREMMOTO 	70
#define XDC_SYN_FMC_ACCIDENT_VLPLREMMOTO 	71
#define XDC_SYN_FMC_ACCIDENT_CARREMMOTO 	72
#define XDC_SYN_FMC_ACCIDENT_VLCARREMMOTO 	73
#define XDC_SYN_FMC_ACCIDENT_PLCARREMMOTO 	74
#define XDC_SYN_FMC_ACCIDENT_VLPLCARREMMOTO 	75
#define XDC_SYN_FMC_METEO_CHUTE_NEIGE 		76
#define XDC_SYN_FMC_METEO_NEIGE_CHAUSSEE 	77
#define XDC_SYN_FMC_METEO_PLUIE 		78
#define XDC_SYN_FMC_METEO_VENT_FORT 		79
#define XDC_SYN_FMC_METEO_BROUILLARD 		80
#define XDC_SYN_FMC_METEO_BROUILLARD_GIVRANT 	81
#define XDC_SYN_FMC_METEO_GRELE 		82
#define XDC_SYN_FMC_METEO_VERGLAS 		83
#define XDC_SYN_FMC_METEO_AUTRE 		84
#define XDC_SYN_INTERVENANT_AUTRE 		85
#define XDC_SYN_FMC_DELESTAGE	 		86
#define XDC_SYN_FMC_DEVIATION			87
#define XDC_SYN_FMC_FERM_ECH			88
#define XDC_SYN_FMC_FERM_AIRE			89
#define XDC_SYN_FMC_HORS_RESEAU			90
#define XDC_SYN_FMC_SALAGE			91
#define XDC_SYN_FMC_METEO_GRELE_CHAUSSEE	92
#define XDC_SYN_FMC_VEHICULE_MARCHE_ARR		93
#define XDC_SYN_FMC_TRAITEMENT_CHAUSSEES	94
#define XDC_SYN_FMC_ECHANGEUR_DECONSEILLE	95
#define XDC_SYN_FMC_CHAUSSEE_GLISSANTE		96
#define XDC_SYN_FMC_BASCUL_POUR_TRAVAUX		97
#define XDC_SYN_FMC_ZONE_STOCKAGE		98
#define XDC_SYN_IPHONE				99
#define XDC_SYN_FMC_REGULATION_VITESSE		100

/* Noms des machines et leurs libelles */
#define XDC_POVC1	"POVC1"
#define XDC_LIB_POVC1	"Poste Opérateur Région Var Cote d'Azur 1"
#define XDC_POCA2	"POVC2"
#define XDC_LIB_POCA2	"Poste Opérateur Région Var Cote d'Azur 2"
#define XDC_POCA4	"POVC4"
#define XDC_LIB_POCA4	"Poste Opérateur Région Var Cote d'Azur 4"
#define XDC_POCA5	"POVC5"
#define XDC_LIB_POCA5	"Poste Opérateur Région Var Cote d'Azur 5"
#define XDC_POCA3	"POVC3"
#define XDC_LIB_POCA3	"Poste Opérateur Région Var Cote d'Azur 3"
#define XDC_POCI1	"POCI1"
#define XDC_LIB_POCI1	"Poste Opérateur Centre d'Information 1"
#define XDC_POCI2	"POCI2"
#define XDC_LIB_POCI2	"Poste Opérateur Centre d'Information 2"
#define XDC_POCI3	"POCI3"
#define XDC_LIB_POCI3	"Poste Opérateur Centre d'Information 3"
#define XDC_POTFM	"POTFM"
#define XDC_LIB_POTFM	"Poste Opérateur Radio Vinci Autoroutes"
#define XDC_POTFM2	"POTFM2"
#define XDC_LIB_POTFM2	"Poste Opérateur Radio Vinci Autoroutes 2"
#define XDC_PODFA	"PODFA"
#define XDC_LIB_PODFA	"Poste Opérateur Développement, Formation, Administration 1"
#define XDC_PODFA2	"PODFA2"
#define XDC_LIB_PODFA2	"Poste Opérateur Développement, Formation, Administration 2"
#define XDC_POCC	"POCC"
#define XDC_LIB_POCC	"Poste Opérateur Cellule de Crise"
#define XDC_POGT	"POGT"
#define XDC_LIB_POGT	"Poste Opérateur Gestionnaire Trafic"
#define XDC_POCMA	"POCMA"
#define XDC_LIB_POCMA	"Poste Opérateur Chef Maintenance"
#define XDC_PODMA1	"PODMA1"
#define XDC_LIB_PODMA1	"Poste Opérateur De Maintenance 1"
#define XDC_PODMA2      "PODMA2"
#define XDC_LIB_PODMA2	"Poste Opérateur De Maintenance 2"
#define XDC_PODMA3      "PODMA3"
#define XDC_LIB_PODMA3  "Poste Opérateur De Maintenance 3"
#define XDC_POCDCA1	"POCDCA1"
#define XDC_LIB_POCDCA1	"Poste Opérateur Chef De Région Var Cote d'Azur 1 "
#define XDC_POCDCA2	"POCDCA2"
#define XDC_LIB_POCDCA2	"Poste Opérateur Chef De Région Var Cote d'Azur 2"
#define XDC_POCDCA3     "POCDCA3"
#define XDC_LIB_POCDCA3 "Poste Opérateur Chef De Région Var Cote d'Azur 3"
#define XDC_PODR	"PODR"
#define XDC_LIB_PODR	"Poste Opérateur Directeur Réseau"
#define XDC_POVE1	"POVE1"
#define XDC_LIB_POVE1	"Poste Opérateur Var Esterel 1"
#define XDC_POVE2	"POVE2"
#define XDC_LIB_POVE2	"Poste Opérateur Var Esterel 2"
#define XDC_POPR1	"POPR1"
#define XDC_LIB_POPR1	"Poste Opérateur Provence 1"
#define XDC_POPR2	"POPR2"
#define XDC_LIB_POPR2	"Poste Opérateur Provence 2"
#define XDC_POPR3	"POPR3"
#define XDC_LIB_POPR3	"Poste Opérateur Provence 3"
#define XDC_POVD1	"POVD1"
#define XDC_LIB_POVD1	"Poste Opérateur Val Durance 1"
#define XDC_POVD2       "POVD2"
#define XDC_LIB_POVD2	"Poste Opérateur Val Durance 2"
#define XDC_POAT        "POAT"
#define XDC_LIB_POAT    "Poste Opérateur Assistant Traffic"
#define XDC_POCDPR      "POCDPR"
#define XDC_LIB_POCDPR  "Poste Opérateur Chef Secteur Provence"
#define XDC_POCDVD      "POCDVD"
#define XDC_LIB_POCDVD  "Poste Opérateur Chef Secteur Val Durance"
#define XDC_POCDVE      "POCDVE"
#define XDC_LIB_POCDVE  "Poste Opérateur Chef Secteur Var Esterel"

#define XDC_SRV_SYN	"SSCI"
#define XDC_LIB_SRV_SYN	"Serveur de synoptique"

/* Declaration des machines hors SAE */
#define	XDC_NOMADE	"NOMADE"
#define	XDC_LIB_NOMADE	"Poste NOMADE"
#define	XDC_NUM_NOMADE	(-1)


/*types de machine*/
#define XDC_TYPEM_ST          1
#define XDC_TYPEM_SC          2
#define XDC_TYPEM_SD          3
#define XDC_TYPEM_LT          4
#define XDC_TYPEM_SP          5
#define XDC_TYPEM_PCS         6
#define XDC_TYPEM_PC2	      7
#define XDC_TYPEM_PC2LT	      8
#define XDC_TYPEM_STA	      9
#define XDC_TYPEM_HIS	      10

/*libelles des types de machine*/
#define XDC_LIB_TYPEM_SD      "CI"
#define XDC_LIB_TYPEM_SC      "PC1"
#define XDC_LIB_TYPEM_PC2     "PC2"
#define XDC_LIB_TYPEM_PCS     "PC3"
#define XDC_LIB_TYPEM_DATEX	"DATEX"

/*types de pc pour applix */
#define XDC_TYPE_PCCI		XDC_TYPEM_SD
#define XDC_TYPE_PCNIVEAU1	XDC_TYPEM_SC
#define XDC_TYPE_PCNIVEAU2	XDC_TYPEM_PC2
#define XDC_TYPE_PCNIVEAU3	XDC_TYPEM_PCS

/* Liste des Macros applix appelee */
#define XDC_MACRO_MTCA    "ITMA_TCA_pilotage_camera"
#define XDC_MACRO_MTCACI  "ITMA_TCA_pilotage_camera_CI"
#define XDC_MACRO_MTCV    "ITMA_TCV_Courbe_A_Visualiser"
#define XDC_MACRO_MTEC    "ITMA_TEC_pilotage_echangeur"
#define XDC_MACRO_MTEI    "ITMA_TEI_Equipements_Indispo"
#define XDC_MACRO_MTFA    "ITMA_TFA_Fiche_Annuaire"
#define XDC_MACRO_MTIO    "ITMA_TIO_Info_Objet_Synoptique"
#define XDC_MACRO_MTLM    "ITMA_TLM_Liste_Des_Alarmes"
#define XDC_MACRO_MTLS    "ITMA_TLS_Liste_Astreintes"
#define XDC_MACRO_MTMC    "ITMA_TMC_Fiche_Main_Courante"
#define XDC_MACRO_MTMT01  "a completer"
#define XDC_MACRO_MTMT02  "a completer"
#define XDC_MACRO_MTMT03  "a completer"
#define XDC_MACRO_MTMT04  "a completer"
#define XDC_MACRO_MTMT05  "a completer"
#define XDC_MACRO_MTMT06  "a completer"
#define XDC_MACRO_MTNA    "ITMA_TNA_pilotage_NAV"
#define XDC_MACRO_MTPM    "ITMA_TPM_pilotage_d_un_PMV"
#define XDC_MACRO_MTRA    "ITMA_TRA_Commander_RAU"
#define XDC_MACRO_MTSE    "ITMA_TSE_Synthese_Evt"
#define XDC_MACRO_MTTU    "ITMA_TTU_pilotage_tunnel"
#define XDC_MACRO_MTUE    "ITMA_TUE_Utilisation_Equip"
#define XDC_MACRO_MTLE    "ITMA_TLE_Liste_Evenements"
#define XDC_MACRO_MTPI    "ITMA_TPI_post_it"
#define XDC_MACRO_MTIA    "ITMA_TIA_Interv_Autre"
#define XDC_MACRO_MTID    "ITMA_TID_Interv_Depanneur"
#define XDC_MACRO_MTIP    "ITMA_TIP_Interv_Patrouille_SIS"
#define XDC_MACRO_MTFO    "ITMA_TFO_pmvSurFourgon"
#define XDC_MACRO_MTMO    "ITMA_TMO_pmvMobile"
#define XDC_MACRO_MTGA    "ITMA_TGA_AffichageGare"
#define XDC_MACRO_MTPP	  "ITMA_TPP_pilotage_PAL"
#define XDC_MACRO_MTPA		"ITMA_TPMVA_pilotage_d_un_PMVA"
#define XDC_MACRO_MTBAF		"ITMA_TBAF_pilotage_BAF"
#define XDC_MACRO_MTBAD		"ITMA_TBAD_pilotage_BAD"
#define XDC_MACRO_MTPRV		"ITMA_TPRV_pilotage_d_un_PRV"
#define XDC_MACRO_MTCFE		"ITMA_TCFE_pilotage_CFE"
#define XDC_MACRO_MTIMU		"ITMA_TIMU_pilotage_IMU"
#define XDC_MACRO_MTPAU		"ITMA_TRAU_pilotage_SONO"

#define XDC_MACRO_MTSAGA	"ITMA_TSAGA_pilotage_d_un_eqt"
#define XDC_MACRO_SAGA_PMV	"ITMA_TSAGA_pilotage_d_un_PMV"
#define XDC_MACRO_SAGA_PMVA	"ITMA_TSAGA_pilotage_d_un_PMVA"
#define XDC_MACRO_SAGA_BAF	"ITMA_TSAGA_pilotage_BAF"
#define XDC_MACRO_SAGA_BRA	"ITMA_TSAGA_pilotage_NAV"

/* Types de Bip */

#define	XDC_BIP_INCONNU			0
#define	XDC_BIP_EUROSIGNAL		1
#define	XDC_BIP_OPERATOR_80_CAR		2
#define	XDC_BIP_OPERATOR_8_CHIFFRES	3

#define	XDC_NOM_BIP_INCONNU		""
#define	XDC_NOM_BIP_EUROSIGNAL		"Eurosignal"
#define	XDC_NOM_BIP_OPERATOR_80_CAR	"Operator 80 car."
#define	XDC_NOM_BIP_OPERATOR_8_CHIFF	"Operator 10 chiffres"

/* Types d'astreintes */ /* ATTENTION DE PAS MODIFIER, l'IHM s'en sert ....*/

#define XDC_AST_SIS	8
#define XDC_AST_SIS2	28
#define XDC_AST_SISR	27
/* REPERE1*/
#define XDC_AST_POMPIER	44
#define XDC_AST_DEPANN	20
#define XDC_AST_GENDARM	16
#define XDC_AST_ESCOTA	4
#define XDC_AST_DEP_VL	20
#define XDC_AST_DEP_PL	21
#define XDC_AST_DEP_LEVAGE 29
#define XDC_AST_CLIENT        60
#define XDC_LIB_AST_CLIENT    "client"
#define XDC_AST_GARE_PEAGE_SENS_1	9
#define XDC_AST_GARE_PEAGE_SENS_2	24
#define XDC_AST_PEAGE	35


/* Constante alarme */

#define	XDC_DEBUT_ALARME		1
#define	XDC_FIN_ALARME			2
#define	XDC_DEFAUT			1
#define	XDC_NORMAL			0

/* Constante Voies RADT */

#define XDC_RADT_CHAUSSEE 	0
#define XDC_RADT_V1		1
#define XDC_RADT_V2		2
#define XDC_RADT_V3		3
#define XDC_RADT_V4		4

/* Constante contenant les noms des sous-systemes */

#define XDC_BASEDD "BASEDD"
#define XDC_EQUEXT "EQUEXT"
#define XDC_GECONF "GECONF"
#define XDC_GESIHM "GESIHM"
#define XDC_MAINTE "MAINTE"
#define XDC_SAIDEC "SAIDEC"
#define XDC_SIMTST "SIMTST"
#define XDC_SUPERV "SUPERV"
#define XDC_XDMICG "XDMICG"

/* Constante contenant les noms des carburants */

#define XDC_CRB_SUPER		1
#define XDC_CRB_NOM_SUPER	"SUPER"
#define XDC_CRB_GAZOLE		2
#define XDC_CRB_NOM_GAZOLE	"GAZOLE"
#define XDC_CRB_SSPB95		3
#define XDC_CRB_NOM_SSPB95	"SANS PLOMB 95"
#define XDC_CRB_SSPB98		4
#define XDC_CRB_NOM_SSPB98	"SANS PLOMB 98"
#define XDC_CRB_GPL		5
#define XDC_CRB_NOM_GPL		"GPL"


/* Constantes utilisees pour conversation telephonique ou OPERATOR */

#define XDC_HS				1

#define XDC_CONNECT			1
#define XDC_RING			2
#define XDC_NOCARRIER			3
#define XDC_ERROR			4
#define XDC_BUSY			7
#define XDC_NOANSWER			8
#define XDC_LIBRE			5
#define XDC_NORESPONS			10
#define XDC_ATTENTE                     11

#define XDC_ETAT_OK			"- OK -"
#define XDC_ETAT_CONNECT		"- APPEL -"
#define XDC_ETAT_RING			"- SONNERIE -"
#define XDC_ETAT_NOCARRIER		"- ABSENCE PORTEUSE -"
#define XDC_ETAT_ERROR			"- ERREUR -"
#define XDC_ETAT_BUSY			"- OCCUPE -"
#define XDC_ETAT_NOANSWER		"- PAS DE REPONSE -"
#define XDC_ETAT_LIBRE			"- LIBRE -"
#define XDC_ETAT_HS			"- HORS SERVICE -"
#define XDC_ETAT_NORESPONS		"- ? -"
#define XDC_ETAT_ATTENTE		"- ATTENTE -"

/* commandes */
#define XDC_APPEL			1
#define XDC_RENVOI			2
#define XDC_ANNUL_RENVOI		3
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
#define XDC_SYNTAX_NOK			1
#define XDC_OPERATOR_NOK		2
#define XDC_OPERATOR_HS			3
#define XDC_SYSTEM_HS			4
#define XDC_TIME_OUT			5
#define XDC_PHASE_1			12
#define XDC_PHASE_2			13
#define XDC_PHASE_3			14

#define XDC_ETAT_SYNTAX_NOK		"- Syntaxe incorrecte -"
#define XDC_ETAT_OPERATOR_NOK		"- Numero OPERATOR incorrect -"
#define XDC_ETAT_OPERATOR_HS		"- numero OPERATOR invalide -"
#define XDC_ETAT_SYSTEM_HS		"- systeme OPERATOR indisponible -"
#define XDC_ETAT_TIME_OUT		"- time-out systeme OPERATOR -"
#define XDC_ETAT_PHASE_1		"- Connexion service OPERATOR en cours ... -"
#define XDC_ETAT_PHASE_2		"- Envoi Mot de passe en cours ... -"
#define XDC_ETAT_PHASE_3		"- Envoi Message Operator en cours ... -"

#define XDC_APPEL_NUM			1
#define XDC_APPEL_ALPHA			2

#define XDC_ENV_OPERATOR_0		"ATE0\r"
#define XDC_ENV_OPERATOR_1		"ATDP36090940\r"
#define XDC_ENV_OPERATOR_2		"AUTOMA\r"
#define XDC_REP_OPERATOR_1		"\nCONNECT\r\n"
#define XDC_REP_OPERATOR_2		"veuillez saisir votre message:> "


/* Constantes de comptes-rendus de commandes RAU */

#define	XDC_RAU_CMD_EN_COURS	1
#define	XDC_RAU_PAS_DE_CMD	2
#define	XDC_RAU_CROK		3
#define	XDC_RAU_CRNOK		4
#define	XDC_RAU_FIN_TEST	5
#define	XDC_RAU_ABORT_TEST	6


/* Constantes d'etats RAU */

#define	XDC_ETAT_RAU_REPOS	1
#define	XDC_ETAT_RAU_APPEL	2
#define	XDC_ETAT_RAU_COMM	3
#define	XDC_ETAT_RAU_GARDE	4
#define	XDC_ETAT_RAU_DEFAUT	5
#define	XDC_ETAT_RAU_EN_TEST	6

/* Constantes pour etat RAU avec la GN */
#define XDC_GN_ECOUTE_PAU	240
#define XDC_GN_FIN_ECOUTE_PAU	241
#define XDC_GN_DEM_SECR_PAU	242
#define XDC_GN_REPR_SAE_PAU	243
#define XDC_GN_PREND_RAU	244
#define XDC_GN_DONNE_RAU	245


/* Constantes des Mini GTC */

#define XDC_NB_ETOR		16
#define XDC_NBMAX_ETOR          32

/* Constantes de la video */

#define XDC_AFFECT_IMAG		1
#define XDC_AFFECT_IMAG_JOYSTICK	2
#define XDC_CMD_POS			3
#define XDC_MEM_POS			4
#define XDC_ETEINT_MONIT		5
#define XDC_ENREG_MAGNETO		6
#define XDC_ARRET_MAGNETO		7
#define XDC_LECTURE_MAGNETO		8
#define XDC_AVANCE_MAGNETO		9
#define XDC_RETOUR_MAGNETO		10
#define XDC_EJECT_MAGNETO		11
#define XDC_PAUSE_MAGNETO		12

/* Constante contenant le nom TOR de la Cellule JN */

#define XDC_NOM_CELLULEJN "Cellule JN"


#define	XDC_TOUTES_MACHINES	"TOUTES" /* Tous les machines */
#define	XDC_EQT_MARCHE		28	/* Action equipt : mise en marche */
#define	XDC_EQT_ARRET		29	/* Action equipt : arret */


#define	XDC_IMP_ASCII		1	/* Format d'impression ASCII */
#define	XDC_IMP_PCL		2	/* Format d'impression PCL */
#define	XDC_IMP_POSTSCRIPT	3	/* Format d'impression PostScript */

/* delai de reconnection rtserver */
#define XDC_DELAI_RTSERVER	3	/* delai avant d'essayer de se reconnecter au rtserver */
#define XDC_DELAI_RTSERVER_SUP	1	/* idem pour les taches de superv */

/* definition des types de libelles */
/* Une modification d'un de ces define entraine la modification */
/* des vues utilisant ces define */
/* par ex le changement du define VEH_categorie */
/* entraine la modif du fichier fmc_veh.vue */
#define XDC_LIB_VEH_CATEGORIE		1
#define XDC_LIB_VEH_MARQUE		2
#define XDC_LIB_VEH_COULEUR		3
#define XDC_LIB_CODE_MAT_DANG		4
#define XDC_LIB_CODE_DANGER		5
#define XDC_LIB_PAYS			6
#define XDC_LIB_VEH_NATURE_PANNE	7
#define XDC_LIB_VEH_DEM_ENLEV		8
#define XDC_LIB_TYPE_ACTION	9
#define XDC_LIB_MSG_FOURGON		10
#define XDC_LIB_TYPE_INTERV	11
#define XDC_LIB_TYPE_QUAL_ALRT	12
#define XDC_LIB_TYPE_BIP	13
#define XDC_LIB_TYPE_NEUTRAL	14
#define XDC_LIB_TYPE_BOUCHON		15
#define XDC_LIB_TYPE_CLASSE_FMC		16
#define XDC_LIB_SENS_LOC		17
#define XDC_LIB_TYPE_POINT_CAR		18
#define XDC_LIB_TYPE_POS_AIR_ECH	19
#define XDC_LIB_TYPE_PERTURBATION	20
#define	XDC_LIB_TYPE_TRAVAUX		21
#define	XDC_LIB_TYPE_NATURE_TRAV	22
#define	XDC_LIB_TYPE_EQUIPEMENT		23
#define	XDC_LIB_TYPE_RES_TRANS	24
#define XDC_LIB_MOTIF_APPEL		27
#define	XDC_TYPE_FICHE_MC	28
#define XDC_TYPE_PLUIE		29
#define XDC_LIB_TYPE_VOIE	30
#define XDC_LIB_ORIGINE       	31
#define	XDC_TYPE_LIB_OBSTACLE	32
#define	XDC_TYPE_LIB_PIETON	33
#define	XDC_TYPE_LIB_AUTRE_EVT	34
#define	XDC_TYPE_LIB_TRAIT_CHAUSSEE	35
#define	XDC_TYPE_LIB_CHAUSSEE_GLISS	36
#define	XDC_LIB_TYPE_GROUPE_RADIO	37
#define XDC_TYPE_COND_METEO		38
#define XDC_TYPE_COND_VISIB		39
#define XDC_TYPE_COND_ETAT_CHAU		40
/* #define XDC_TYPE_COND_OBSTACLE		41	ANNULE */
/* #define XDC_TYPE_COND_ANIMAL			42	ANNULE */
#define XDC_TYPE_COND_CHANTIER		43

#define XDC_LIB_TYPE_PARAM_REGUL	44
#define XDC_LIB_TYPE_MESURE_REGUL	45
#define XDC_LIB_TYPE_CONTEXTE_REGUL	46
#define XDC_LIB_TYPE_HOLD_UP		47
#define XDC_LIB_TYPE_CCH		48
#define XDC_LIB_TYPE_JOUR		49
#define XDC_LIB_TYPE_NATURE_INCENDIE	50
#define XDC_LIB_TYPE_MESSAGE_SONO	51


#define XDC_CODE_VOIE_BAU	1
#define XDC_CODE_VOIE_VL	2
#define XDC_CODE_VOIE_VM1	3
#define XDC_CODE_VOIE_VM2	4
#define XDC_CODE_VOIE_VR	5
#define XDC_CODE_VOIE_BAU_I	6
#define XDC_CODE_VOIE_VL_I	7
#define XDC_CODE_VOIE_VM1_I	8
#define XDC_CODE_VOIE_VM2_I	9
#define	XDC_CODE_VOIE_VR_I	10


/* VATHELOT : Ajout des constantes : types de lineaire pour fiche metier 21 */

#define XDC_LINEAIRE_SEGMENT	0
#define XDC_LINEAIRE_100m	1
#define XDC_LINEAIRE_500m	2
#define XDC_LINEAIRE_1000m	3	

/* VATHELOT : Ajout des constantes : types de condition accidentogene pour fiche metier 22 */

#define XDC_TYPE_CONDITION_METEO	0
#define XDC_TYPE_CONDITION_TRAVAUX	1
#define XDC_TYPE_CONDITION_CHAUSSEE	2
#define XDC_TYPE_CONDITION_OUVRAGE	3

/* VATHELOT : Ajout des constantes : conditions accidentogenes chaussee pour fiche metier 22 */

#define XDC_CONDITION_CHAUSSEE_AVEC_BAU_ETROITE	0
#define XDC_CONDITION_CHAUSSEE_AVEC_PENTE	1
#define XDC_CONDITION_CHAUSSEE_AVEC_RAMPE	2
#define XDC_CONDITION_CHAUSSEE_AVEC_NB_VOIES	3

/* VATHELOT : Ajout des constantes : condition accidentogene ouvrage pour fiche metier 22 */

#define XDC_CONDITION_OUVRAGE_TUNNEL	0
#define XDC_CONDITION_OUVRAGE_ECHANGEUR	1
#define XDC_CONDITION_OUVRAGE_PI	2
#define XDC_CONDITION_OUVRAGE_PS	3
#define XDC_CONDITION_OUVRAGE_PEAGE	4

/* DEM 1397 VATHELOT: Ajout des types de periode pour fiche metier 09 */
#define XDC_PERIODE_JOURNEE       0
#define XDC_PERIODE_HEURE 1
#define XDC_PERIODE_DEMIHEURE     2
#define XDC_PERIODE_12MN  3
#define XDC_PERIODE_6MN           4



/*constantes WEB*/
#define XDC_WEB_ROUGE	1
#define XDC_WEB_NOIR 	2
#define XDC_WEB_ORANGE 	3
#define XDC_WEB_VERT	4
#define XDC_WEB_PERTURBATION 	1
#define XDC_WEB_SERVICE 	2
#define XDC_WEB_NIVEAU_0	0
#define XDC_WEB_NIVEAU_1	1
#define XDC_WEB_NIVEAU_2	2


#define XDC_MAI_PMVA		XDC_EQT_SRV
#define XDC_MAI_BAF		XDC_EQT_SRV
#define XDC_MAI_BAD		XDC_EQT_SRV
#define XDC_MAI_PRV		XDC_EQT_SRV
#define XDC_MAI_PIC		XDC_EQT_SRV
#define XDC_ACT_PMVA	XDC_EQT_PMVA
#define XDC_ACT_BAF		XDC_EQT_BAF
#define XDC_ACT_BAD		XDC_EQT_BAD
#define XDC_ACT_PRV		XDC_EQT_PRV
#define XDC_ACT_PIC		XDC_EQT_PIC
#define XDC_ACT_LIB_PMVA	"PMV acces"
#define XDC_ACT_LIB_BAF		"BAF"
#define XDC_ACT_LIB_BAD		"BAD"
#define XDC_ACT_LIB_PRV		"PRV"
#define XDC_ACT_LIB_SAGA	"SAGA"
#define XDC_PMVA_TEXTE		1
#define XDC_PMVA_HEURE		3
#define XDC_PMVA_ETEINDRE		4
#define XDC_PMVA_DEFAUT		5
#define XDC_PMVA_TDP			6
#define XDC_PMVA_TDP_RAF		7
#define XDC_PMVA_3POINTS		8
#define XDC_PMVA_PASFLASH		1
#define XDC_PMVA_FLASH		2
#define XDC_PMVA_FLASHABSENT		0
#define XDC_PMVA_FLASHPRESENT		1
#define XDC_PMVA_PASCLIGNO	1	/* Pas de ligne clignotante */
#define XDC_PMVA_CLIGNOLENT	2	/* Clignotement lent sur une ligne PMV */
#define XDC_PMVA_CLIGNORAPIDE	3	/* Clignotement rapide sur une ligne PMV */

/******  Constantes de mode d'affichage PMVA (EXCLUSIFS) **********/

#define XDC_PMVA_MODE_NORMAL	XDC_PMVA_PASCLIGNO	/* Mode normal d'affichage, constant */
#define XDC_PMVA_MODE_CLIGNO	XDC_PMVA_CLIGNORAPIDE	/* Mode d'affichage clignotant */
#define XDC_PMVA_MODE_ALTERNE	11			/* Mode d'affichage alterne */


/******  Constantes PRV *****************************************/

#define XDC_PRV_TEXTE		2
#define XDC_PRV_HEURE		4
#define XDC_PRV_ETEINDRE		5
#define XDC_PRV_DEFAUT		6
#define XDC_PRV_TDP			7
#define XDC_PRV_TDP_RAF		8
#define XDC_PRV_PASFLASH		1
#define XDC_PRV_FLASH		3
#define XDC_PRV_FLASHABSENT		1
#define XDC_PRV_FLASHPRESENT		2
#define XDC_PRV_PASCLIGNO	2
#define XDC_PRV_CLIGNOLENT	3
#define XDC_PRV_CLIGNORAPIDE	4
#define XDC_PRV_PLEINE_VOIE 	0
#define XDC_PRV_ACCOTEMENT	1
#define XDC_PRV_BPV		10

/******  Constantes PIC *****************************************/
#define XDC_PIC_ACCOTEMENT	1
#define XDC_PIC_TEXTE		2
#define XDC_PIC_BPV			10
#define XDC_PIC_PASFLASH		1
#define XDC_PIC_FLASH		3

/* Communautaire */
#define XDC_COMMU_ESSAI         1
#define XDC_COMMU_FAUSSE_ALERTE 2

/* Liste Messages RVA */
#define XDC_TMA_BLOC_MSG	0
#define XDC_TMA_BLOC_ENT	1
#define XDC_TMA_BLOC_ARC	2


/* Types de jour pour calendrier */
#define XDC_JOUR_TOUS		0
#define XDC_JOUR_OUVRE		20
#define XDC_JOUR_OUVRE_TRV	30
#define XDC_JOUR_WEEKEND	40
#define XDC_JOUR_VACANCES	50
#define XDC_JOUR_FERIE		60



/******************************************************************************
**                                                                           **
**  DEFINITIONS NE CONCERNANT PAS L'IHM                                      **
**   (A INCLURE UNIQUEMENT DANS xdc.h)                                       **
**                                                                           **
******************************************************************************/

/* Non Applix - debut */

#define XDC_FIN_CHN	'\0'


/* Nombre d'erreurs RTworks pour un RTClient avant de sortir de la Tache */
#define XDC_EXIT_RTCLIENT	100


	/* Declaration des variables definissant les serveurs actifs
	*	et passifs sur les differents sites */
#define XDC_ACTIF_CI	"SDCI1"
#define XDC_PASSIF_CI	"SDCI2"
#define XDC_ACTIF_DN	"SDDN1"
#define XDC_PASSIF_DN	"SDDN2"
#define XDC_ACTIF_DM	"SDDM1"
#define XDC_PASSIF_DM	"SDDM2"
#define XDC_ACTIF_DS	"PODS1"
#define XDC_ACTIF_DC	"PODC1"
#define XDC_ACTIF_DA	"SDDA1"
#define XDC_ACTIF_DP	"SDDP1"

#define XDC_SERVEUR_HIS	"SDHIS"


/* Cle utilisee pour creer une FMC provenant de la DDE 83 (SAGA) */
#define CLE_SAGA        43
#endif


/* Pour compatibilite seulement !!! */
#define	TYP_TRAVAUX		XDC_LIB_TYPE_TRAVAUX
#define	TYP_EQT			XDC_LIB_TYPE_EQUIPEMENT

/* Non Applix - fin */

/* Const seuils de basculement TDP ESCOTA / TDP_FCD */

#define XDC_SEUIL_AFFICHAGE_TDP_FCD 60			
#define XDC_SEUIL_AFFICHAGE_TDP_FCD_ABSENCE_TDP_ESCOTA 60

/* Const pour recherche si proche niche et issue de secour */
#define XDC_MGA_PAR_REF_LOC_TUNNEL "LOC_TUNNEL_RAYON"
#define XDC_MGA_PAR_REF_RAYON_TOUS "RAYON_TOUS"
#define XDC_RES_NIC_IS_TYPE_NICHE 1
#define XDC_RES_NIC_IS_TYPE_ISSUE_DE_SECOUR 2

/******************************************************************************
**                                                                           **
**     DEFINITIONS POUR L'IHM SEULEMENT                                      **
**   (A INCLURE UNIQUEMENT DANS xdc_ax.h)                                    **
**                                                                           **
******************************************************************************/

/* Non Applix - debut */
#if 0
/* Non Applix - fin */

/* Constante defaut equipement */
#define	XDC_EQP_DEF_CRIT	3
#define	XDC_EQP_DEF_MAJEUR	2
#define	XDC_EQP_DEF_MINEUR	1
#define	XDC_EQP_HS			4

/* Pour compatibilite seulement !!! */
#define TYP_BOUCHON		15
#define VEH_code_matiere        XDC_LIB_CODE_MAT_DANG

#define XDC_SANS_NTFMAX		XDC_NIV_TFM_MAX

/* Libelle alerte POLYVOX/WAZE */
#define XDC_PREFIXE_ALERTE_POLYVOX "POLYVOX-WAZE"

/* Non Applix - debut */
#endif
/* Non Applix - fin */


/* fin de fichier */
