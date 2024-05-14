/*E*/
/* Fichier : $Id: xzaec_ax.h,v 1.55 2021/05/03 14:05:18 pc2dpdy Exp $        $Revision: 1.55 $        $Date: 2021/05/03 14:05:18 $
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
* C.T.  10/10/94	: Creation a partir du fichier xzae.h situe ds BASEDD/inc
* I.T.	10/10/94	: Suppression #endif
* C.T.	10/10/94	: Ajout commentaire pour synthese
* C.T.	17/10/94	: Ajout commentaire pour synthese
* C.T.	20/10/94	: Ajout code d'erreur de localisation
* C.T.	24/11/94	: Ajout code d'erreur de plan de secours (V 1.6)
* B.G.	21/12/94	: Ajout point pour espace synthese (1.7)
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
* C.T.  10/11/96    : Ajout XZAEC_SYN_FAUSSE_ALERTE (1282)
* C.T.  14/11/96    : Ajout XZAEC_SEUIL_BAU_ETROITE (1283)
* C.T.  11/12/96    : Ajout XZAEC_SYN_LON_ESC XZAEC_BOU_Escargot (FMC7)
* JMG	27/12/96	: Ajout XZAEC_SYN_CFG_VOIES_BAS (IHM15) (DEM/1372) 1.24
* JMG	03/06/97	: pas de perception peage devient sans perception peage (DEM/1435) 1.25
* C.T.  18/12/96    : Ajout XZAEC_FMC_INIT_... (1531)
* C.T.  16/01/98    : Ajout XZAEC_SYN_ACT_PAL..., XZAEC_SYN_ACT_FAC, XZAEC_SYN_ACT_FAU (1.27)
* JMG	22/01/98	: accent de succes (dem/1547) 1.29
* C.T.  29/01/98	: Ajout XZAEC_SYN_FMC_DEGRADE (1.30)
* C.T.  03/03/98	: Ajout XZAEC_FMC_INIT_NON_AUTORISEE (1583) (1.31)
* C.T.  03/03/98	: Ajout XZAEC_SYN_ACT_PMV_BAND (1607) (1.31)
* C.T.  03/03/98	: Ajout XZAEC_FMC_CLOSE_PAS_ENR (1612) (1.32)
* C.P	18/05/98	: Correction accent (dem 1358 et 1615) (1.33)
* ???	14/10/98	: Ajout XZAEC_SYN_ACT_ANNULEE dem/1500 1.34
* P.N	12/01/99	: Modifs intergestionnaire dem/1724 (1.35)
* P.C   18/03/03	: Ajout types de travaux (1.36)
* P.N	28/02/05	: Ajout type de bouchon "En accordeon" 1.37 DEM434
* CHI	13/04/05	: Ajout sous type meteo "Grele sur chaussee" et "Chaussee mouillee" 1.38 DEM456
* CHI   18/07/05 	: Ajout type XZAEC_ClasseNature + XZAEC_SYN_NATURE_OBSTACLE  v1.39 DEM487
* JMG	13/09/05	: ajout PMVA BAF BAD 1.40
* JMG	02/10/05	: ajout modules BAF et BAD 1.41
* PNI	17/10/05	: correction blesses --> blesses v1.42 DEM529
* JMG	10/11/05	: correction XZAEC_SYN_ACT_PMVA 1.43
* JPL	11/04/07	: Libelle XZAEC_SYN_NATURE_OBS plus general (nature evt.) 1.44
* JPL	17/04/07	: Ajout types FMC Marche Arriere, Traitement Chaussees, Echangeur deconseille,
*                            Chaussee glissante, Basculement pour Travaux (DEM 643-647) 1.45
* JPL	10/05/07	: Ajout des (sous-)types de traitement chaussees et de chaussee glissante (DEM 644,646) 1.46
* JPL	15/05/07	: Suppression de doublons 1.47
* JPL	15/05/07	: Ajout de constantes de libelles pour fmc Echangeur deconseille (DEM 645) 1.48
* JPL	13/06/07	: Ajout libelle pour configuration des voies de Basculement pour travaux (DEM 647) 1.49
* JPL	10/07/07	: Modif libelle pour configuration des voies de Basculement pour travaux (DEM GO) 1.50
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
* PNI	15/04/15	: Remplacement SIS par Surveillance Reseau DEM1125 1.71
* NDE	20/08/15	: Nouvelles constantes pour intitules deviation, basculement travaux, etc. (DEM 1136) 1.73
* JMG	26/11/176	: ajout majeur lot 23 1.74
* JPL	12/12/16	: Libelles de niveaux de bouchon pour textes de mails (DEM 1193)  1.75
* JPL	21/03/17	: Changement du terme Secteur en Region (DEM 1173)  1.76
* JMG	12/12/17	: ajout SMS 1.77
* LCL	22/02/18	: Ajout CTRL_FEUX 1.78 presence personnel travaux
* JPL	22/06/18	: Caracteres minuscules accentues en ISO Latin (DEM 1274)  1.79
* JMG	10/10/18	: ajout SMS LOT25 DEM1301 1.80
* JPL	04/04/19	: Ajout libelle XZAEC_SYN_ECOUL (DEM 1326)  1.81
* RGR	17/06/19	: Correction orthographe manifestants		1.82
* JPL	01/10/19	: Ajout libelle XZAEC_SYN_BOU_MOBILE (DEM 1346)  1.83
* CGR	07/12/20	: Ajout XZAEC_SYN_ACT_IMU et XZAEC_SYN_ACT_IMU_TXT DEM-SAE155 1.84
* ABE	02/04/21	: Ajout PAU_SONO DEM-SAE244 1.85
* GGY	14/11/23	: Ajout PIC (DEM483)
--------------------------------------------------------------------------------- */


/* fichiers inclus */

/* definitions de constantes exportees */


/*A Codes d'erreur des procedures stockees traitant les FMC */
/*A (validation, achevement, lecture, transmission des droits d'enrichir ...) */

/*! Ils completent les codes XDC generaux, et DOIVENT EN ETRE DISTINCTS */

#define XZAEC_FMC_PAS_ENR	6	/* pas le droit d'enrichir */
#define XZAEC_FMC_ETAT_INC	7	/* FMC n'est pas dans l'etat */
					/* adequate pour la valider */
#define XZAEC_FMC_DEB_PRV_INC	8	/* date de debut prevu trop tot */
					/* pour la prevision (au moins +24h)*/
#define XZAEC_FMC_VOIE_INC	9	/* etat des voies incorrectes */
#define XZAEC_FMC_LOC_INC	10	/* localisation de fmc incorrecte */
#define XZAEC_FMC_OPR_INC	11	/* operateur enrichisseur incorrect */
#define XZAEC_FMC_ACT_NON_FIN	12	/* actions relatives a un evt non terminees */
#define XZAEC_FMC_NON_FIN	13	/* evenement non termine */
#define XZAEC_FMC_A_TRT		14	/* evenement a traiter sur un district */
#define XZAEC_FMC_PAS_RETYPAGE	15	/* retypage non autorise */
#define XZAEC_FMC_INC_RETYPAGE	16	/* nouveau type pas de meme classe que le type de fmc actuel */
#define XZAEC_FMC_INIT_INC	17	/* fmc initiale inexistante */
#define XZAEC_FMC_INIT_DEG	18	/* fmc initiale de type degrade avec fmc origine */
#define XZAEC_FMC_DEG_ENCOURS	19	/* fmc est initiale d'autres fmc degrade en cours */
#define XZAEC_FMC_INIT_INCOMP	20	/* fmc initiale incompatible avec la fmc degrade */
#define XZAEC_FMC_INIT_PAS_ENR	21	/* pas le droit d'enrichir la fmc initiale */
#define XZAEC_FMC_INIT_NON_AUTORISEE	22	/* lien avec cette fmc initiale non autoris� */
#define XZAEC_FMC_CLOSE_PAS_ENR	23	/* pas le droit d'enrichir la partie tampon d'une fmc close*/
#define XZAEC_FMC_PAS_DONNEES	24	/* donnees absentes (ex. partie specifique de la fmc) */
#define XZAEC_FMC_ERR_DONNEES	25	/* donnees erronees (ex. partie specifique de la fmc) */
#define XZAEC_FMC_ERR_VAL	26	/* donnees de validation absentes (fmc non validee) ou erronees */
#define XZAEC_FMC_PAS_DON_VAL	27	/* donnees d'une validation absentes (partie specifique de la fmc) */
#define XZAEC_FMC_ERR_DON_VAL	28	/* donnees d'une validation erronees (partie specifique de la fmc) */
#define XZAEC_FMC_ERR_ACTION	29	/* donnees d'une action erronees */


/*A differentes classes de fiches main courante */
#define XZAEC_ClasseAccident		0
#define XZAEC_ClasseDelestage		1
#define XZAEC_ClasseVehicule		2
#define XZAEC_ClasseVehLent			3
#define XZAEC_ClasseMeteo			4
#define XZAEC_ClasseEchangeur		5
#define XZAEC_ClasseSousConcess		6
#define XZAEC_ClasseManif			7
#define XZAEC_ClasseBouchon			8
#define XZAEC_ClasseTravaux			9
#define XZAEC_ClasseBasculement		10
#define XZAEC_ClassePosteOper		11
#define XZAEC_ClasseGenerique		12	/* tous types de FMC sans donnees specifiques */
#define XZAEC_ClasseNature			13
#define XZAEC_ClasseBasculTrav		14
#define XZAEC_ClasseRegulation		15
#define XZAEC_ClasseIncendie		16


/*A types de FMC */
#define XZAEC_FMC_Accident			1
#define XZAEC_FMC_Delestage			2
#define XZAEC_FMC_Deviation			3
#define XZAEC_FMC_PosteOperateur	4
#define XZAEC_FMC_Meteo				5
#define XZAEC_FMC_Contresens		6
#define XZAEC_FMC_VehArrete			7
#define XZAEC_FMC_VehPanne			8
#define XZAEC_FMC_VehFeu			9
#define XZAEC_FMC_Cirque			10
#define XZAEC_FMC_ConvoiExc			11
#define XZAEC_FMC_ConvoiMil			12
#define XZAEC_FMC_VehLent			13
#define XZAEC_FMC_Manifestation		14
#define XZAEC_FMC_GreveESCOTA		15
#define XZAEC_FMC_Escargot			16
#define XZAEC_FMC_Basculement		17
#define XZAEC_FMC_Travaux			18
#define XZAEC_FMC_IncSsConces		19
#define XZAEC_FMC_QueueBouchon		20
#define XZAEC_FMC_FermetureEch		21
#define XZAEC_FMC_Eboulement		22
#define XZAEC_FMC_Incendie			23
#define XZAEC_FMC_Inondation		24
#define XZAEC_FMC_Obstacle			25
#define XZAEC_FMC_ChausseDeter		26
#define XZAEC_FMC_FlaqueHuile		27
#define XZAEC_FMC_AnimalErrant		28
#define XZAEC_FMC_FrontiereFermee	29
#define XZAEC_FMC_AltercationUsager	30
#define XZAEC_FMC_Agression			31
#define XZAEC_FMC_MalaiseUsager		32
#define XZAEC_FMC_EvtHorsReseau		33
#define XZAEC_FMC_AireFermee		34
#define XZAEC_FMC_Salage			35
#define XZAEC_FMC_HoldUpVol			36
#define XZAEC_FMC_TeteBouchon		37
#define XZAEC_FMC_PietonChaussee	38
#define XZAEC_FMC_Autre				39
#define XZAEC_FMC_VehMarcheArr		40
#define XZAEC_FMC_TraitChaussees	41
#define XZAEC_FMC_EchDeconseille	42
#define XZAEC_FMC_ChausseeGliss		43
#define XZAEC_FMC_BasculTravaux		44
#define XZAEC_FMC_ZoneStockage		45
#define XZAEC_FMC_Regulation		46

/*A nombre total de type FMC */
#define XZAEC_FMC_NB_TYP			46	/* si modification du nb de type fmc, */ 
										/* mettre a jour ce define */

/* pour renommer la tete de bouchon pour vues metier */
#define XZAEC_FMC_LIB_BOU			"BOUCHON"

/*A nature des travaux */
#define XZAEC_TRV_Neufs				1
#define XZAEC_TRV_Entretien			0

/*A type de travaux */
#define XZAEC_TRV_Glissieres		1
#define XZAEC_TRV_Vegetation		2
#define XZAEC_TRV_OuvHydro			3
#define XZAEC_TRV_OuvArt			4
#define XZAEC_TRV_Terrass			5
#define XZAEC_TRV_SignalVert		6
#define XZAEC_TRV_SignalHori		7
#define XZAEC_TRV_Eclairage			8
#define XZAEC_TRV_CableRAU			9
#define XZAEC_TRV_LavBalTun			10
#define XZAEC_TRV_LavBalAutre		11
#define XZAEC_TRV_MaintTun			12
#define XZAEC_TRV_MaintAutre		13
#define XZAEC_TRV_Elargiss			14
#define XZAEC_TRV_MurAntibruit		15
#define XZAEC_TRV_ConfGliss			16
#define XZAEC_TRV_Autre				17
#define XZAEC_TRV_Enrobes			18
#define XZAEC_TRV_MaintVideo		19
#define XZAEC_TRV_PAU				20
#define XZAEC_TRV_Cloture			21
#define XZAEC_TRV_FO				22
#define XZAEC_TRV_Accident			23
#define XZAEC_TRV_Stockage			24
#define XZAEC_TRV_Tunnels			25

/*A type de bouchon */
#define XZAEC_BOU_Dense			1
#define XZAEC_BOU_Ralenti		2
#define XZAEC_BOU_Bouche		3
#define XZAEC_BOU_Bloque		4
#define XZAEC_BOU_Escargot		5
#define XZAEC_BOU_Accordeon		6

#define XZAEC_BOU_LIB_Dense		"DENSE"
#define XZAEC_BOU_LIB_Ralenti		"RALENTI"
#define XZAEC_BOU_LIB_Bouche		"BOUCHE"
#define XZAEC_BOU_LIB_Bloque		"BLOQUE"
#define XZAEC_BOU_LIB_Escargot		"ESCARGOT"
#define XZAEC_BOU_LIB_Accordeon		"EN ACCORDEON"


/*A type de perturbation meteo */
#define XZAEC_MET_ChuteNeige		1
#define XZAEC_MET_NeigeChaus		2
#define XZAEC_MET_Pluie			3
#define XZAEC_MET_VentFort		4
#define XZAEC_MET_Brouillard		5
#define XZAEC_MET_BrouillardGiv		6
#define XZAEC_MET_Grele			7
#define XZAEC_MET_Verglas		8
#define XZAEC_MET_Autre			9
#define XZAEC_MET_GreleChaus		10
#define XZAEC_MET_ChausMouill		11

/*A type de chaussee glissante */
    /*B types identiques a une perturbation meteo */
#define XZAEC_CH_GLIS_Neige		2
#define XZAEC_CH_GLIS_Verglas		8
#define XZAEC_CH_GLIS_Autre		9
#define XZAEC_CH_GLIS_Grele		10
    /*B autres types de chaussee glissante */
#define XZAEC_CH_GLIS_Huile		101
#define XZAEC_CH_GLIS_Carburant		102
#define XZAEC_CH_GLIS_Gasoil		103

/*A conditions de conduite pour la Viabilite Hivernale */
#define XZAEC_CCH_NR			0
#define XZAEC_CCH_C1			1
#define XZAEC_CCH_C2			2
#define XZAEC_CCH_C3			3
#define XZAEC_CCH_C4			4

#define XZAEC_CCH_LIB_NR		"NR"
#define XZAEC_CCH_LIB_C1		"C1"
#define XZAEC_CCH_LIB_C2		"C2"
#define XZAEC_CCH_LIB_C3		"C3"
#define XZAEC_CCH_LIB_C4		"C4"


/*A type de traitement des chaussees */
#define XZAEC_TRT_CH_Salage		201
#define XZAEC_TRT_CH_Deneigement	202

/*A type de vehicule */
#define XZAEC_VEH_PL			1
#define XZAEC_VEH_VL			2
#define XZAEC_VEH_Car			3
#define XZAEC_VEH_Moto			4
#define XZAEC_VEH_Remorque		5

/*A utiliser pour XZAE68 : pour autoriser ou non l'effacement des */
/*A resultats a la sortie de la procedure stockee                 */
#define XZAEC_EffacDonnees		1
#define XZAEC_PasEffacDonnees		0

/*A utiliser pour XZAE70 et XZAE68 : pour la memorisation en memoire ou */
/*A non des consequences d'un evenement                                 */

#define XZAEC_PasMemoriserConseq	0
#define XZAEC_MemoriserConseq		1


/*A nombre de digits pour le libelle fmc synthese */
/*A et action                                     */

#define XZAEC_NB_DIGITS_LIB_FMC		6
#define XZAEC_NB_DIGITS_LIB_ACT		6

/*A commentaires pour la synthese d'un evenement */
#define XZAEC_SYN_CAR_SEPAR	"*"
#define XZAEC_SYN_TXT_A		"� "

#define XZAEC_SYN_Majeur 	"Evenement majeur"
#define XZAEC_SYN_ALT		"ALERTE     "		/* 11 carcateres */
#define XZAEC_SYN_ALT_COM	"commentaire : "	/* 14 carcateres */
#define XZAEC_SYN_ALT_LOC	"position    : "	/* 14 carcateres */
#define XZAEC_SYN_CREATION	"CREATION   "		/* 11 carcateres */
#define XZAEC_SYN_FMC_DEGRADE	"FMC initiale : N"	/* 17 carcateres */

#define XZAEC_SYN_ORI_CRE	"origine     : "	/* 14 carcateres */
#define XZAEC_SYN_DEB		"d�but       : "	/* 14 carcateres */
#define XZAEC_SYN_FIN		"fin         : "	/* 14 carcateres */
#define XZAEC_SYN_DEB_PREV	"d�but pr�vu : "	/* 14 carcateres */
#define XZAEC_SYN_FIN_PREV	"fin pr�vue  : "	/* 14 carcateres */
#define XZAEC_SYN_PAS_DATE	".                   "	/* 20 carcateres */
#define XZAEC_SYN_ACT_REUSSIE	" : succ�s"
#define XZAEC_SYN_ACT_ECHOUEE	" : �chec"
#define XZAEC_SYN_ACT_RESULT_SUC	"r�sultat : succ�s "
#define XZAEC_SYN_ACT_RESULT_ECHEC	"r�sultat : �chec  "
#define XZAEC_SYN_VEH_AIR	"sur aire    : "	/* 14 carcateres */
#define XZAEC_SYN_VEH_ECH	"sur �changeur : "	/* 16 carcateres */
#define XZAEC_SYN_VEH_LIT	"sur lit arr�t : "	/* 16 carcateres */
#define XZAEC_SYN_VEH_PEA	"sur p�age   : "	/* 14 carcateres */
#define XZAEC_SYN_SIGNALEE	"signal�"		/* 7 carcateres */
#define XZAEC_SYN_CONFIRMEE	"confirm�"		/* 8 carcateres */
#define XZAEC_SYN_VAL		"VALIDATION "		/* 11 carcateres */
#define XZAEC_SYN_CAUSE		"CAUSE               "	/* 20 carcateres */
#define XZAEC_SYN_COM_VAL	"commentaire valable jusqu � " /* 28 carcateres */
#define XZAEC_SYN_COM		"commentaire "		/* 12 carcateres */
#define XZAEC_SYN_CONSEQ	"CONSEQUENCES        "	/* 20 carcateres */
#define XZAEC_SYN_AUCUNE_CONSEQ	"N�ant"
#define XZAEC_SYN_FINIE		"FIN D'EVENEMENT"
#define XZAEC_SYN_LON_ESC	"longueur (en km) : "
#define XZAEC_SYN_GRAVITE	"Gravit�    : "
#define XZAEC_SYN_TRAFIC	"Trafic     : "
#define XZAEC_SYN_BLOQUANT	"Bloquant   : oui"
#define XZAEC_SYN_NONBLOQUANT	"Bloquant   : non"
#define XZAEC_SYN_GRAVITE0	"zero"
#define XZAEC_SYN_GRAVITE1	"un"
#define XZAEC_SYN_GRAVITE2	"deux"
#define XZAEC_SYN_GRAVITE3	"trois"
#define XZAEC_SYN_TRAFIC_FLUIDE	"fluide"
#define XZAEC_SYN_TRAFIC_PRESAT	"pr�-satur�"
#define XZAEC_SYN_TRAFIC_SATURE	"satur�"

#define XZAEC_SYN_ACT_ANNULEE	": Annul�e"
#define XZAEC_SYN_MAJEUR	"Majeur"
#define XZAEC_SYN_DEG_DOM	"d�gats domaine"
#define XZAEC_SYN_ECOUL		"pr�sence d'�coulement"
#define XZAEC_SYN_PERS		"pr�sence de personalit�s"
#define XZAEC_SYN_PER_ESC	"pr�sence de personnel ESCOTA"
#define XZAEC_SYN_VEH_ESC	"pr�sence de v�hicule ESCOTA"
#define XZAEC_SYN_PRE_MAT_DNG	"pr�sence de mati�res dangereuses"
#define XZAEC_SYN_MOR		"nombre de morts            : "
#define XZAEC_SYN_BLE_GRA	"nombre de bless�s graves   : "
#define XZAEC_SYN_BLE_LEG	"nombre de bless�s l�gers   : "
#define XZAEC_SYN_REMORQ	"nombre de remorques        : "
#define XZAEC_SYN_MOTO		"nombre de motos            : "
#define XZAEC_SYN_CAR		"nombre de cars             : "
#define XZAEC_SYN_PL		"nombre de poids lourds     : "
#define XZAEC_SYN_VL		"nombre de v�hicules l�gers : "

#define XZAEC_SYN_SCEN		"Sc�nario : "
#define XZAEC_SYN_CON		"Contexte : "
#define XZAEC_SYN_TRA		"Transitoire : "
#define XZAEC_SYN_ZON		"Zone : "
#define XZAEC_SYN_MES		"Mesure : "
#define XZAEC_SYN_IND_HOR	"D�clenchement horaire"
#define XZAEC_SYN_HOR		"horodate"

#define XZAEC_SYN_MAT_DNG	"mati�res dangereuses : "
#define XZAEC_SYN_VEH_PL	"type de v�hicule     : poids lourd"
#define XZAEC_SYN_VEH_VL	"type de v�hicule     : v�hicule l�ger"
#define XZAEC_SYN_VEH_CAR	"type de v�hicule     : car"
#define XZAEC_SYN_VEH_MOT	"type de v�hicule     : moto"
#define XZAEC_SYN_VEH_REM	"type de v�hicule     : remorque"
#define XZAEC_SYN_VEH_FEU_MAITRISE	"feu maitris� : "
#define XZAEC_SYN_INC_MAITRISE	"incendie maitris� : "

#define XZAEC_SYN_PLN_SCR	"plan de secours          : "
#define XZAEC_SYN_ECH_SOR	"�changeur de sortie      : "
#define XZAEC_SYN_ECH_INJ	"�changeur de r�injection : "

#define XZAEC_SYN_LON_VEH	"longueur du v�hicule : "
#define XZAEC_SYN_VIT_VEH	"vitesse du v�hicule  : "
#define XZAEC_SYN_LRG_VEH	"largeur du v�hicule  : "
#define XZAEC_SYN_HAU_VEH	"hauteur du v�hicule  : "
#define XZAEC_SYN_POI_VEH	"poids du v�hicule    : "
#define XZAEC_SYN_CNT_VEH	"contenu du v�hicule  : "

#define XZAEC_SYN_NATURE	"Nature : "

#define XZAEC_SYN_PERSONNEL	"Presence de personnel"
#define XZAEC_SYN_VITESSE	"Vitesse : "
#define XZAEC_SYN_VITESSE_OPPOSEE	"Vitesse opposee : "

#define XZAEC_SYN_CHUT_NEI	"type : chutes de neige"
#define XZAEC_SYN_NEI_CHA	"type : chauss�e enneig�e"
#define XZAEC_SYN_PLU		"type : pluie"
#define XZAEC_SYN_VEN		"type : vent fort"
#define XZAEC_SYN_BROUIL	"type : brouillard"
#define XZAEC_SYN_BRO_GIV	"type : brouillard et givre"
#define XZAEC_SYN_GRELE		"type : gr�le"
#define XZAEC_SYN_VERG		"type : verglas"
#define XZAEC_SYN_AUT		"type : autre"
#define XZAEC_SYN_GREL_CH	"type : gr�le sur chauss�e"
#define XZAEC_SYN_CH_MOUI	"type : chauss�e mouill�e"
#define XZAEC_SYN_VIS_MET	"visibilit� : "
#define XZAEC_SYN_FIN_MET	"PR fin : "
#define XZAEC_SYN_PRECIPIT	"Pr�cipitations"

#define XZAEC_SYN_CHS_HUILE	"type : huile sur chauss�e"
#define XZAEC_SYN_CHS_CARBURANT	"type : carburant sur chauss�e"
#define XZAEC_SYN_CHS_GASOIL	"type : gasoil sur chauss�e"

#define XZAEC_SYN_TRT_SALAGE	"type : salage"
#define XZAEC_SYN_TRT_DENEIG	"type : d�neigement"


#define XZAEC_SYN_CNC		"sous concessionnaire : "
#define XZAEC_SYN_PEN_CRB	"p�nurie de "
#define XZAEC_SYN_CNC_FERM	"fermeture du sous concessionnaire"

#define XZAEC_SYN_PAS_PER_PEA	"sans perception p�age"
#define XZAEC_SYN_VIO_MNF	"violence"
#define XZAEC_SYN_NBR_MNF	"nombre de manifestants : "

#define XZAEC_SYN_FIN_BAS	"fin du basculement � "
#define XZAEC_SYN_CFG_VOIES_BAS	"Configuration finale des voies "
#define XZAEC_SYN_VOIE_BAS	"Voie"
#define XZAEC_SYN_PRD_BAS	"PR d�but"
#define XZAEC_SYN_PRF_BAS	"PR fin"
#define XZAEC_SYN_HD_BAS	"Horodate d�but"
#define XZAEC_SYN_HF_BAS	"Horodate fin"

#define XZAEC_SYN_CFG_VOIES_BAS_TRV	"Traitement des voies"
#define XZAEC_SYN_BOU_MOBILE		"bouchon mobile"

#define XZAEC_SYN_TRV_GLI	"type : glissi�res"
#define XZAEC_SYN_TRV_VEG	"type : v�g�tation"
#define XZAEC_SYN_TRV_HYD	"type : ouvrage hydraulique"
#define XZAEC_SYN_TRV_ART	"type : ouvrage d'art"
#define XZAEC_SYN_TRV_TER	"type : chauss�e et terrassement"
#define XZAEC_SYN_TRV_SGV	"type : signalisation verticale"
#define XZAEC_SYN_TRV_SGH	"type : signalisation horizontale"
#define XZAEC_SYN_TRV_ECL	"type : �clairage"
#define XZAEC_SYN_TRV_RAU	"type : r�seau d'appel d'urgence"
#define XZAEC_SYN_TRV_LBT	"type : lavage, balayage tunnel"
#define XZAEC_SYN_TRV_LBA	"type : lavage balayage"
#define XZAEC_SYN_TRV_MTN	"type : maintenance d'�quipement de tunnel"
#define XZAEC_SYN_TRV_MAU	"type : maintenance d'�quipement autre que tunnel"
#define XZAEC_SYN_TRV_ELA	"type : �largissement"
#define XZAEC_SYN_TRV_MUR	"type : mur anti-bruit"
#define XZAEC_SYN_TRV_CGL	"type : conformit� de glissi�re"
#define XZAEC_SYN_TRV_AUT	"type : autres travaux"
#define XZAEC_SYN_TRV_ENR	"type : enrob�s"
#define XZAEC_SYN_TRV_MTV	"type : maintenance vid�o"
#define XZAEC_SYN_TRV_PAU	"type : travaux sur postes d'appel d'urgence"
#define XZAEC_SYN_TRV_CLO	"type : travaux de cl�ture"
#define XZAEC_SYN_TRV_FON	"type : travaux sur fibre optique"
#define XZAEC_SYN_TRV_ACC	"type : travaux suite � accident"
#define XZAEC_SYN_TRV_STO	"type : stockage de poids lourds"
#define XZAEC_SYN_TRV_TUN	"type : tunnels"

#define XZAEC_SYN_TRV_NEU	"travaux neufs"
#define XZAEC_SYN_TRV_ENT	"travaux d'entretien"
#define XZAEC_SYN_TRV_MOB	"travaux mobiles"

#define XZAEC_SYN_DEB_PLA	"d�but planifi�              : " 
#define XZAEC_SYN_FIN_PLA	"fin planifi�e               : "
#define XZAEC_SYN_TET_PLA	"d�but des travaux planifi�s : "
#define XZAEC_SYN_QUE_PLA	"fin des travaux planifi�s   : "

#define XZAEC_SYN_PR_TET	"d�but des travaux           : "
#define XZAEC_SYN_VOI_RET	"r�tr�cissement de voie"
#define XZAEC_SYN_TRV_DISCONT	"travaux discontinus planifi�s : "

#define XZAEC_SYN_CHANTIER_EN_COURS		"CHANTIER EN COURS            "
#define XZAEC_SYN_CHANTIER_PREVU		"CHANTIER PREVU               "
#define XZAEC_SYN_DEVIATION_EN_COURS		"DEVIATION EN COURS           "
#define XZAEC_SYN_DEVIATION_PREVUE		"DEVIATION PREVUE             "
#define XZAEC_SYN_FERMETURE_ECH_EN_COURS	"FERMETURE ECHANGEUR EN COURS "
#define XZAEC_SYN_FERMETURE_ECH_PREVUE		"FERMETURE ECHANGEUR PREVUE   "
#define XZAEC_SYN_AIRE_FERMEE_EN_COURS		"AIRE FERMEE EN COURS         "
#define XZAEC_SYN_AIRE_FERMEE_PREVUE		"AIRE FERMEE PREVUE           "
#define XZAEC_SYN_BASCUL_TRAVAUX_EN_COURS	"BASCULEMENT TRAVAUX EN COURS "
#define XZAEC_SYN_BASCUL_TRAVAUX_PREVU		"BASCULEMENT TRAVAUX PREVU    "

#define XZAEC_SYN_STK_PRT	"d�but de stockage           : "
#define XZAEC_SYN_STK_PREPAR	"stockage en pr�paration"
#define XZAEC_SYN_STK_ENCOURS	"stockage en cours"

#define XZAEC_SYN_FMC_TeteBouchon	"TETE BOUCHON"
#define XZAEC_SYN_FMC_QueueBouchon	"QUEUE BOUCHON"
#define XZAEC_SYN_BOU_TET	"tete de bouchon  : "
#define XZAEC_SYN_BOU_QUE	"queue de bouchon : "
#define XZAEC_SYN_BOU_TYP	"type             : "
#define XZAEC_SYN_BOU_LON	"longueur (en km) : "
#define XZAEC_SYN_BOU_DNS	"type             : dense"
#define XZAEC_SYN_BOU_RAL	"type             : ralentissement"
#define XZAEC_SYN_BOU_BOU	"type             : bouch�"
#define XZAEC_SYN_BOU_BLQ	"type             : bloqu�"
#define XZAEC_SYN_BOU_ACD	"type             : en accord�on"

#define XZAEC_SYN_OPR		"poste op�rateur : "

#define XZAEC_SYN_SOR_N_OUV	"sortie sens 2 ouverte"
#define XZAEC_SYN_SOR_S_OUV	"sortie sens 1 ouverte"
#define XZAEC_SYN_ENT_N_OUV	"entree sens 2 ouverte"
#define XZAEC_SYN_ENT_S_OUV	"entree sens 1 ouverte"
#define XZAEC_SYN_SOR_N_FERM	"sortie sens 2 ferm�e"
#define XZAEC_SYN_SOR_S_FERM	"sortie sens 1 ferm�e"
#define XZAEC_SYN_ENT_N_FERM	"entree sens 2 ferm�e"
#define XZAEC_SYN_ENT_S_FERM	"entree sens 1 ferm�e"

#define XZAEC_SYN_SOR_N_DECONS	"sortie sens 2 d�conseill�e"
#define XZAEC_SYN_SOR_S_DECONS	"sortie sens 1 d�conseill�e"
#define XZAEC_SYN_ENT_N_DECONS	"entr�e sens 2 d�conseill�e"
#define XZAEC_SYN_ENT_S_DECONS	"entr�e sens 1 d�conseill�e"

#define XZAEC_SYN_ACT_DEB	" : DEBUT   "
#define XZAEC_SYN_ACT_ECHEC        " : ECHEC   "
#define XZAEC_SYN_ACT_FIN	" : FIN     "
#define XZAEC_SYN_ACT_DPT	" : DEPART  "
#define XZAEC_SYN_ACT_ARR	" : ARRIVEE "

#define XZAEC_SYN_ACT_FAX	"envoi mail         "		/* 19 caracteres */
#define XZAEC_SYN_ACT_TEL	"appel astreinte par t�l "	/* 24 caracteres */
#define XZAEC_SYN_ACT_BIP	"appel astreinte par bip/radio "	/* 30 caracteres */
#define XZAEC_SYN_ACT_FAX_AST	"envoi mail � "
#define XZAEC_SYN_ACT_TEL_AST	"appel par t�l "
#define XZAEC_SYN_ACT_BIP_AST	"appel par bip/radio "
#define XZAEC_SYN_ACT_RADIO	"appel par radio "
#define XZAEC_SYN_ACT_SMS	"envoi SMS          "
#define XZAEC_SYN_ACT_SMS_AST	"envoi SMS � "

#define XZAEC_SYN_ACT_PMV	"PMV                "	/* 19 caracteres */
#define XZAEC_SYN_ACT_PMVDAT	"PMV                "	/* 19 caracteres */
#define XZAEC_SYN_ACT_PMV_TXT	"texte   :  "
#define XZAEC_SYN_ACT_PMV_TXT_4	"ligne direction :  "
#define XZAEC_SYN_ACT_PMV_PCT	"picto   :  "
#define XZAEC_SYN_ACT_PMV_BAND	"bandeau :  "

#define XZAEC_SYN_ACT_SAGA	"Eqt SAGA           "
#define XZAEC_SYN_ACT_TUB	"tunnel             "	/* 19 caracteres */
#define XZAEC_SYN_ACT_SEQ	"s�quence : "

#define XZAEC_SYN_ACT_ECH	"�changeur          "	/* 19 caracteres */

#define XZAEC_SYN_ACT_DEP	"d�panneur          "	/* 19 caracteres */
#define XZAEC_SYN_ACT_DEP_NOM	"nom : "
#define XZAEC_SYN_ACT_RESULTAT	"r�sultat : "

#define XZAEC_SYN_ACT_NAV	"BRA                "	/* 19 caracteres */
#define XZAEC_SYN_ACT_FAU	"Fermeture d'autoroute "	/* 22 caracteres */
#define XZAEC_SYN_ACT_FAC	"Fermeture d'acc�s  "	/* 19 caracteres */
#define XZAEC_SYN_ACT_SCN	"sc�nario : "

#define XZAEC_SYN_ACT_BAL_POS	"pose balisage"
#define XZAEC_SYN_ACT_BAL_OTE	"�te balisage"
#define XZAEC_SYN_ACT_SIS	"patrouille Surveillance Reseau  "	/* 19 caracteres */
#define XZAEC_SYN_ACT_SIS_NOM	"patrouille : "
#define XZAEC_SYN_ACT_SIS_DEB_BAL	"tete de balisage  : "
#define XZAEC_SYN_ACT_SIS_FIN_BAL	"queue de balisage : "

#define XZAEC_SYN_ACT_INT_POM	"pompiers           "	/* 19 caracteres */
#define XZAEC_SYN_ACT_INT_GND	"gendarmerie        "	/* 19 caracteres */
#define XZAEC_SYN_ACT_INT_NOM	"patrouille : "

#define XZAEC_SYN_ACT_FRG	"PMV sur fourgon    "	/* 19 caracteres */
#define XZAEC_SYN_ACT_FRG_LOC	"localisation : "
#define XZAEC_SYN_ACT_FRG_TXT	"message      : "

#define XZAEC_SYN_ACT_RMQ	"PMV sur remorque   "	/* 19 caracteres */
#define XZAEC_SYN_ACT_RMQ_LOC	"localisation : "
#define XZAEC_SYN_ACT_RMQ_TXT	"message      : "

#define XZAEC_SYN_ACT_TFM	"Message RVA        "	/* 19 caracteres */
#define XZAEC_SYN_ACT_TFM_DIFF	"nombre de diffusions : "
#define XZAEC_SYN_ACT_TFM_ARRET	"arr�t diffusion � "

#define XZAEC_SYN_ACT_GAR	"affichage en gare  "	/* 19 caracteres */
#define XZAEC_SYN_ACT_GAR_NOM	"gare de p�age : "	/* 16 caracteres */
#define XZAEC_SYN_ACT_GAR_TXT	"texte : "		/* 8 caracteres */
#define XZAEC_SYN_FAUSSE_ALERTE	"FAUSSE ALERTE"
#define XZAEC_SYN_ORIGINE_DATEX	"Origine DATEX"

#define XZAEC_SYN_ACT_PAL					"Panneau d'alerte   "	/* 19 caracteres */
#define XZAEC_SYN_ACT_PAL_MOD1				"Module 1 : "
#define XZAEC_SYN_ACT_PAL_MOD2				"Module 2 : "
#define XZAEC_SYN_ACT_PAL_MOD3				"Module 3 : "
#define XZAEC_SYN_ACT_PAL_MOD4				"Module 4 : "
#define XZAEC_SYN_ACT_PAL_MOD5				"Module 5 : "

#define XZAEC_SYN_ACT_PMVA      			"PMVA               "   /* 19 caracteres */
#define XZAEC_SYN_ACT_BAF       			"BAF                "   /* 19 caracteres */
#define XZAEC_SYN_ACT_BAD       			"BAD                "   /* 19 caracteres */
#define XZAEC_SYN_ACT_PRV       			"PRV                "   /* 19 caracteres */
#define XZAEC_SYN_ACT_PMVA_TXT  			"texte   :  "
#define XZAEC_SYN_ACT_CFE       			"CFE                "   /* 19 caracteres */
#define XZAEC_SYN_ACT_PRV_TXT   			"texte   :  "
#define XZAEC_SYN_ACT_BAF_MOD1				"Feu arr�t droit "
#define XZAEC_SYN_ACT_CFE_TXT   			"Plan de feux : "
#define XZAEC_SYN_ACT_BAF_MOD2				"Feu alerte droit "
#define XZAEC_SYN_ACT_BAF_MOD3 				"Barri�re droite "
#define XZAEC_SYN_ACT_BAF_MOD4				"Feu arr�t gauche "
#define XZAEC_SYN_ACT_BAF_MOD5				"Feu alerte gauche "
#define XZAEC_SYN_ACT_BAF_MOD6				"Barri�re gauche "
#define XZAEC_SYN_ACT_BAD_MOD1				"Flash "
#define XZAEC_SYN_ACT_BAD_MOD2				"Barri�re "
#define XZAEC_SYN_ACT_BAF_FEUX_DROITS 		"Feux arr�t et alerte droits "
#define XZAEC_SYN_ACT_BAF_GLOBAL_DROIT 		"Barri�re, feux arr�t et alerte droits "
#define XZAEC_SYN_ACT_BAF_FEUX_GAUCHES 		"Feux arr�t et alerte gauches "
#define XZAEC_SYN_ACT_BAF_GLOBAL_GAUCHE 	"Barri�re, feux arr�t et alerte gauches "
#define XZAEC_SYN_ACT_IMU       			"IMU                "   /* 19 caracteres */
#define XZAEC_SYN_ACT_IMU_TXT				"Message/Zones : "
#define XZAEC_SYN_ACT_PAU					"RAU SONO           "   /* 19 caracteres */
#define XZAEC_SYN_ACT_PAU_TXT				"Diffusion "
#define XZAEC_SYN_ACT_PIC       			"PIC                "   /* 19 caracteres */
#define XZAEC_SYN_ACT_PIC_TXT   			"texte   :  "

#define XZAEC_SYN_ACT_EXP					"Export Intergest.  "	/* 19 caracteres */
#define XZAEC_SYN_ACT_EXPTFM				"Export Message RVA "	/* 19 caracteres */
#define XZAEC_SYN_ACT_EXPTFM_FIN			"fin : "	/* 19 caracteres */

#define XZAEC_SYN_ACT_CCH					"Envoi conditions de conduite"	/* 28 caracteres */

/* AAZ 27/08/2007 */
#define XZAEC_SYN_ACT_GTC_SIG_ECH			"signalisation e    "	/* 19 caracteres */
#define XZAEC_SYN_ACT_GTC_SIG				"signalisation t    "	/* 19 caracteres */
#define XZAEC_SYN_ACT_GTC_ECL				"eclairage          "	/* 19 caracteres */
#define XZAEC_SYN_ACT_GTC_VEN				"ventilation        "	/* 19 caracteres */
#define XZAEC_SYN_ACT_GTC_ENR				"energie            "	/* 19 caracteres */
#define XZAEC_SYN_ACT_GTC_INC				"protection incendie"	/* 19 caracteres */
#define XZAEC_SYN_ACT_GTC_EXP				"exploitation       "	/* 19 caracteres */
/* AAZ */

/* FMC signalee ou confirmee */
#define XZAEC_FMC_SIGNALEE					0
#define XZAEC_FMC_CONFIRMEE					1

/* perception au peage ou non lors de manifestation */
#define XZAEC_FMC_PAS_PER					1
#define XZAEC_FMC_AVC_PER					0

/*A nature des travaux */
#define XZAEC_FMC_TRV_ENT					0
#define XZAEC_FMC_TRV_NEU					1

/*A indicateur de type d'heure (utiliser pour XZAE;02 et XZAE;82) */
#define XZAEC_ACT_SUC						0	/* heure de succes */
#define XZAEC_ACT_FIN						1	/* heure de fin */
#define XZAEC_ACT_ECHEC						2	/* heure d'echec */

/*A Droits d'enrichissement des FMC */
#define XZAEC_ENR_AUCUN						0
#define XZAEC_ENR_FMC						1
#define XZAEC_ENR_TETEBOU					2

/* type de reveil a declencher pour une validation de fmc */
#define XZAEC_REV_FMC_CRE					1	/* reveil pour un ajout d'evt dans liste a traiter dans le site local */
#define XZAEC_REV_FMC_MOD					2	/* reveil pour une modification d'evt a traiter */
#define XZAEC_REV_FMC_REC					3	/* reveil pour un ajout d'evt dans liste a traiter pour un site de recouvrement */
#define XZAEC_REV_FMC_DIS					4	/* reveil pour un ajout d'evt dans liste a traiter pour un site */
#define XZAEC_REV_FMC_FIN					5	/* reveil pour une fin d'evt dans liste a traiter pour un site */
#define XZAEC_REV_FMC_DIS_NOM				"FMC DANS VOTRE REGION"

/* code d'erreur pour la verification d'une localisation */
#define XZAEC_LOC_PT_CAR_INC				3

/* code d'erreur pour un plan de secours inconnu pour la fiche delestage */
#define XZAEC_PLN_SCR_INC					3

/* numero de vehicule non value */
#define XZAEC_VEH_INCONNU					-1

/* seuil en dm pour la bau etroite */
#define	XZAEC_SEUIL_BAU_ETROITE				20

/* types de rappel client */
#define XZAEC_TYP_RAP_NUM_CLIENT			0
#define XZAEC_TYP_RAP_REFUS_COORD			1
#define XZAEC_TYP_RAP_INJOIGNABLE			2
#define XZAEC_TYP_RAP_NUM_ETRANGER			3
#define XZAEC_TYP_RAP_DEPAN_SEUL			4
#define XZAEC_TYP_RAP_AUTRE					5
#define XZAEC_TYP_RAP_LIB_NUM_CLIENT		"Numero client : "
#define XZAEC_TYP_RAP_LIB_REFUS_COORD		"Refus donner coordonnees"
#define XZAEC_TYP_RAP_LIB_INJOIGNABLE		"Numero injoignable"
#define XZAEC_TYP_RAP_LIB_NUM_ETRANGER		"Numero etranger"
#define XZAEC_TYP_RAP_LIB_DEPAN_SEUL		"Client se depanne seul"
#define XZAEC_TYP_RAP_LIB_AUTRE				"Autre"
#define XZAEC_TYP_RAP_LIB_GENE          	"Rappel client : "

/* definitions de types exportes */
/* definition de macro exportees */
/* declarations de donnees globales */
/* delaration de fonctions externes */
