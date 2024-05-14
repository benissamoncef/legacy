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
* ABE   24/01/22    : ajout alerte doublon potentiel AVA DEM-SAE367
------------------------------------------------------ */

#define XZAAC_DEJA_TRT		3

/*A Alertes DAI (nom du type sur 25 caracteres maxi */

#define XZAAC_DAI_RAL		1
#define XZAAC_DAI_RAL_NOM	"RALENTISSEMENT"

#define XZAAC_DAI_ARR		2
#define XZAAC_DAI_ARR_NOM	"VEHICULE ARRETE"

#define XZAAC_DAI_ARR_CON	3
#define XZAAC_DAI_ARR_CON_NOM	"VEHICULE ARRETE + BOUCHON"

/*A Alertes PMV */

#define XZAAC_AFF_DIF		4
#define XZAAC_AFF_DIF_NOM	"AFFICHAGE INCOHERENT"

/* Alertes GTC de NICE */

#define XZAAC_SIG_LOCAL         5
#define XZAAC_SIG_LOCAL_NOM     "GTC NICE EN MODE LOCAL"

#define XZAAC_LIT               6
#define XZAAC_LIT_NOM           "LIT D'ARRET OCCUPE"

#define XZAAC_EXTINCTEUR        7
#define XZAAC_EXTINCTEUR_NOM    "EXTINCTEUR DECROCHE"

#define XZAAC_SEUIL_CO          8
#define XZAAC_SEUIL_CO_NOM      "SEUIL CO ATTEINT"

#define XZAAC_SEUIL_OP          9
#define XZAAC_SEUIL_OP_NOM      "SEUIL OPACITE ATTEINT"

/*A Alertes NAV */

#define XZAAC_CLL	        10
#define XZAAC_CLL_NOM      	"CONNEXION LOCALE LECTURE"

#define XZAAC_CLE	        11
#define XZAAC_CLE_NOM      	"CONNEXION LOCALE ECRITURE"

#define XZAAC_CLI	        12
#define XZAAC_CLI_NOM      	"CONNEXION LOCALE INCONNU"

#define XZAAC_COL	        13
#define XZAAC_COL_NOM      	"COLLISION BARRIERE"

/*A Alertes RAU */

#define XZAAC_APPEL_PAU	        14
#define XZAAC_APPEL_PAU_NOM 	"APPEL PAU"

#define XZAAC_GN_ECOUTE_PAU	15
#define XZAAC_GN_ECOUTE_PAU_NOM "GENDARME EN ECOUTE"

#define XZAAC_GN_FIN_ECOUTE_PAU		16
#define XZAAC_GN_FIN_ECOUTE_PAU_NOM 	"FIN GN EN ECOUTE"

#define XZAAC_GN_DEM_SECR_PAU		17
#define XZAAC_GN_DEM_SECR_PAU_NOM 	"GN DEMANDE LE SECRET"

#define XZAAC_GN_REPR_SAE_PAU		18
#define XZAAC_GN_REPR_SAE_PAU_NOM 	"GN DEMANDE REPRISE SAE"

/*A Alertes TOR */

#define XZAAC_TOR_0			19
#define XZAAC_TOR_0_NOM 		"PASSAGE JOUR-NUIT"

#define XZAAC_TOR_14			20
#define XZAAC_TOR_14_NOM 		"INTRUSION 1"

#define XZAAC_TOR_15			21
#define XZAAC_TOR_15_NOM 		"INTRUSION 2"

/*A Alertes RADT */

#define XZAAC_EQA_RADT			22
#define XZAAC_EQA_RADT_NOM		"EQUATION D'ALERTE RADT"

/*A Alertes VIDEO */

#define XZAAC_LTV_SATURE                 23
#define XZAAC_LTV_SATURE_NOM             "LTV SATURE"

/*A Alertes equation evt */

#define XZAAC_EQA_EVT                  24
#define XZAAC_EQA_EVT_NOM              "EQUATION D'ALERTE EVENT"

/*A Alertes RADT */

#define XZAAC_NIV_RADT			25
#define XZAAC_NIV_RADT_NOM		"CHANGEMENT NIV DE SERVICE"

/*A Alertes Mini GTC suite */

#define XZAAC_PROB_TEMP			27
#define XZAAC_PROB_TEMP_NOM		"PROBLEME DE TEMPERATURE"

#define XZAAC_NIV_MAX_EAU		28
#define XZAAC_NIV_MAX_EAU_NOM		"NIVEAU D'EAU MAX ATTEINT"

#define XZAAC_ALERTE_N1			29
#define XZAAC_ALERTE_N1_NOM		"ALERTE 1"

#define XZAAC_ALERTE_N2			30
#define XZAAC_ALERTE_N2_NOM		"ALERTE 2"

#define XZAAC_ALERTE_N3			31
#define XZAAC_ALERTE_N3_NOM		"ALERTE 3"

#define XZAAC_ALERTE_N4			32
#define XZAAC_ALERTE_N4_NOM		"ALERTE 4"

#define XZAAC_ALERTE_N5			33
#define XZAAC_ALERTE_N5_NOM		"ALERTE 5"

#define XZAAC_EDF			44
#define XZAAC_EDF_NOM			"DEFAUT ALIM. ENERGIE"

#define XZAAC_APPEL_IPHONE		81
#define XZAAC_APPEL_IPHONE_NOM		"PAU VIRTUEL"

#define XZAAC_ALERTE_COMMUNAUTAIRE      82
#define XZAAC_ALERTE_COMMUNAUTAIRE_NOM  "ALERTE COMMUNAUTAIRE"
#define XZAAC_ALERTE_REGULATION      83
#define XZAAC_ALERTE_REGULATION_NOM  "ALERTE REGULATION"

#define XZAAC_ALERTE_SAGA_DAI1		86
#define XZAAC_ALERTE_SAGA_DAI1_NOM	"STOP TROTTOIR"
#define XZAAC_ALERTE_SAGA_DAI2		87
#define XZAAC_ALERTE_SAGA_DAI2_NOM	"PIETON TROTTOIR"
#define XZAAC_ALERTE_SAGA_DAI3		88
#define XZAAC_ALERTE_SAGA_DAI3_NOM	"STOP GARAGE"
#define XZAAC_ALERTE_SAGA_DAI4		89
#define XZAAC_ALERTE_SAGA_DAI4_NOM	"PIETON GARAGE"
#define XZAAC_ALERTE_SAGA_DAI5		90
#define XZAAC_ALERTE_SAGA_DAI5_NOM	"STOP GARAGE"
#define XZAAC_ALERTE_SAGA_DAI6		91
#define XZAAC_ALERTE_SAGA_DAI6_NOM	"STOP TROTTOIR OPPOSE"
#define XZAAC_ALERTE_SAGA_DAI7		92
#define XZAAC_ALERTE_SAGA_DAI7_NOM	"PIETON TROTTOIR OPPOSE"
#define XZAAC_ALERTE_SAGA_DAI8		93
#define XZAAC_ALERTE_SAGA_DAI8_NOM	"STOP VEHICULE"
#define XZAAC_ALERTE_SAGA_DAI9		94
#define XZAAC_ALERTE_SAGA_DAI9_NOM	"CTS"
#define XZAAC_ALERTE_SAGA_DAI10		95
#define XZAAC_ALERTE_SAGA_DAI10_NOM	"OBJET"
#define XZAAC_ALERTE_SAGA_DAI11		96
#define XZAAC_ALERTE_SAGA_DAI11_NOM	"VEHICULE LENT"
#define XZAAC_ALERTE_SAGA_DAI12		97
#define XZAAC_ALERTE_SAGA_DAI12_NOM	"RALENTISSEMENT"
#define XZAAC_ALERTE_SAGA_DAI13		98
#define XZAAC_ALERTE_SAGA_DAI13_NOM	"STOP VEHICULE OPPOSE"
#define XZAAC_ALERTE_SAGA_DAI14		99
#define XZAAC_ALERTE_SAGA_DAI14_NOM	"CTS OPPOSE"
#define XZAAC_ALERTE_SAGA_DAI15		100
#define XZAAC_ALERTE_SAGA_DAI15_NOM	"OBJET OPPOSE"
#define XZAAC_ALERTE_SAGA_DAI16		101
#define XZAAC_ALERTE_SAGA_DAI16_NOM	"VEHICULE LENT OPPOSE"
#define XZAAC_ALERTE_SAGA_DAI17		102
#define XZAAC_ALERTE_SAGA_DAI17_NOM	"VISIBILITE"


#define XZAAC_ALERTE_SAGA_SCENARIO	103
#define XZAAC_ALERTE_SAGA_SCENARIO_NOM	"ALERTE SCENARIO SAGA"

#define XZAAC_ALERTE_PERTE_VISI		104
#define XZAAC_ALERTE_PERTE_VISI_NOM	"PERTE DE VISIBILITE"

#define XZAAC_ALERTE_GABEB		105
#define XZAAC_ALERTE_GABEB_NOM		"DETECTION BOUCHON"
#define XZAAC_ALERTE_PANNE_GABEB		106
#define XZAAC_ALERTE_PANNE_GABEB_NOM		"DEFAUT CAPTEUR"

#define XZAAC_ALERTE_AVA_POTENTIEL_DOUBLON		176
#define XZAAC_ALERTE_AVA_POTENTIEL_DOUBLON_NOM		"DOUBLON POTENTIEL AVA"

#define XZAAC_ALERTE_COCKPIT_BOUCHON_POTENTIEL_DOUBLON  177
#define XZAAC_ALERTE_COCKPIT_BOUCHON_POTENTIEL_DOUBLON_NOM  "DOUBLON BOUCHON POTENTIEL PIT"