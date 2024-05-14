/*E*/
/* Fichier : $Id: ITMA_TAR.h,v 1.77 2021/05/03 13:48:04 pc2dpdy Exp $        Release : $Revision: 1.77 $        Date : $Date: 2021/05/03 13:48:04 $
-------------------------------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
-------------------------------------------------------------------------------
* SOUS-SYSTEME  GESIHM
-------------------------------------------------------------------------------
* MODULE MTAR * FICHIER ITMA_TAR.h
-------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* constantes de communication avec la tache relais
*
*   Cf. DCG 2.4.9
*
-------------------------------------------------------------------------------
* HISTORIQUE :
*
* Guilhou	28 Sep 1994	: Creation
* Cornu		11 Oct 1994	: Ajout de MTAR_FCT_XZEA28		   v1.04
* Cornu		12 Oct 1994	: Ajout de MTAR_FCT_XZEC02		   v1.05
* Cornu		19 Oct 1994	: Ajout de MTAR_FCT_XZEV01 a 07		   v1.07
* Cornu		26 Oct 1994	: Ajout de MTAR_FCT_XZEA13 		   v1.08
* Cornu		10 Nov 1994	: Ajout de MTAR_FCT_XZET01 & 02		   v1.09
* Cornu		18 Nov 1994	: Ajout de MTAR_FCT_XZEA18,22,23,24
*				  Ajout de MTAR_FCT_XZAC05,07		   v1.18
* Cornu		01 Dec 1994	: Ajout de MTAR_FCT_XZEA10 		   v1.20
* Cornu		08 Dec 1994	: Ajout des MTAR_FCT_XZEN (1 a 4)	   v1.21
* Cornu		08 Dec 1994	: Ajout des MTAR_FCT_XZEA08	           v1.22
* Cornu		20 Dec 1994	: Ajout des MTAR_FCT_XZEP01 & 02	   v1.22
* Guilhou	20 dec 1994	: ajout de MTAR_FCT_OUVREFMC		   v1.23
* Guilhou	27 dec 1994	: ajout de MTAR_FCT_DEMANDESURCHARGE
*				  et MTAR_FCT_REPONSESURCHARGE	           V1.25	
* Lecrivain	29 dec 1994	: Ajout primitives pilotage RAU 
                                  et telephone                             V1.27
* Lecrivain	02 Jan 1995	: Ajout fonction MTAR_FCT_XZEA30           V1.28
* Lecrivain	02 Jan 1995	: Ajout fonction MTAR_FCT_XZEL01           V1.29
* Lecrivain	02 Jan 1995	: Ajout pseudo-fonction MTAR_FCT_XZEU00    V1.30
* Guilhou	10 jan 1995	: Ajout fct MTAR_FCT_FINSURCHARGE	   V1.31
* Cornu 	13 jan 1995	: Ajout fct MTAR_FCT_XZEL02 & 03	   V1.32
* Lecrivain	12 Jan 1995	: Ajout MTAR_FCT_XZCA02_03                 V1.34
* Lecrivain	27 Jan 1995	: Ajout MTAR_FCT_XZEC0x                    V1.37
* Lecrivain	29 Jan 1995	: Ajout MTAR_FCT_XZAA10                    V1.38
* Lecrivain	04 Avr 1995	: Ajout MTAR_FCT_XZEL02_03 et MTAR_FCT_MSG_ADM    V1.41
* Torregrossa	06 Jul 1995	: Ajout fonction MTAR_FCT_SONALERTE        V1.43
* Volcic	17 Jul 1995	: Modif fonction MTAR_FCT_XZEP01_ARRET     V1.44
* Guilhou	19 Sep 1995	: Ajout fonction MTAR_FCT_DONINDIV	   V1.45
* Mismer	21 Sep 1995	: Ajout fonction MTAR_FCT_XZEA26	   V1.47
* Noel		07 Fev 1996	: Ajout des fonctions d'arret et relance 
*					des synoptiques.		   V1.48
* Guilhou 	22 Aou 1996	: Ajout MTAR_FCT_FIN_POSTE (RADT)	V1.49
* niepceron	26 Sep 1996	: Ajout de MTAR_FCT_ACQ_VOISIN (dem/424) v1.50
* Mismer  	25 Mar 1997 	: Ajout de MTAR pour TDP (DEM/1417) V1.51
* Guilhou	19 jan 1998	: ajout MTAR_FCT_INITPRES 1.52
* Mismer	26 jan 1998	: ajout constantes PAL 1.53 (DEM/1539)
* niepceron	20 nov 1998	: ajout MTAR_FCT_DEMCAT et EXPCAT 1.53 dem/1723
* cluchague     30 sep 2002	: ajout MTAR_FCT_XZET03 1.55 (REPERE1)
* JMG		13 sep 2005	: ajout PMVA BAF BAD 1.56
* JMG		20/10/05	: correction MTAR_FCT pour PMVA 1.58
* JMG		23/10/07	: ajout MTAR_FCT_XZEU10 1.59
* JBL		Nov 2007	: DEM 665
* PNI		09/03/11	: Suppression de la seconde definition de MTAR_FCT_XZEA25 1.62
* VR		15/12/11	: Ajout PRV (DEM/1016)
* JPL		23/12/11	: Constantes inutilisees; homogeneisation  1.63
* JPL		26/12/11	: Ajout constante MTAR_FCT_TIMER_TDP (DEM 1007)  1.64
* SDU       30/01/12    : Ajout MTAR_FCT_XZEL10 à MTAR_FCT_XZEL12 (DEM 1015)
* MG		16/04/12	: Ajout du MTAR_FCT_xzav03 demarrer simulation DEM 1016
* MG		11/06/12	: Ajout ITRE_AX_XZEZ04 (DEM/1014 PMA)
* JMG		29/09/14	 : Ajout MTAR_FCT_FACTIS
* JMG		26/04/16 : ajout MTAR_FCT_XZEA180 SAGA 1.69
* JMG		20/06/17 : ajout MTAR_FCT_TIGMOT* tigmot 1.70
* LCL		19/03/18 : ajout MTAR_FCT_XZEP07 MTAR_FCT_XZEA082 CTRL_FEUX 1.71 DEM1284
* JMG		29/10/19 : ajout MTAR_FCT_XZIS50 RMVI DEM1315 1.72
* JMG		30/11/18 ; ajout MTAR_FCT_XZIS53 AVA 1.73
* JMG		24/06/19 : ajout MTAR_FCT_XZEL16 1.74
* JMG		19/11/19 : ajout TDP surcharge 1.75
* CGR		13/01/21 : ajout MTAR_FCT_XZEP08 IMU DEM-SAE155 1.76
* CGR           02/04/21 : ajout MTAR_FCT_XZEP09 et MTAR_FCT_XZEA85 SONO PAU DEM-SAE244 1.76
* CGR		04/03/22 : ajout MTAR_FCT_AVA_RETOUR_ALERTE DEM-SAE367 1.77
---------------------------------------------------------------------------- */


/*fonctions dont la tache relais demande l'execution a Applix*/
define MTAR_MSG_QUIT			1		/*RT->A deconnexion du process*/
define MTAR_MSG_SEND_POKE		2		/*RT->A reception d'un poke*/
define MTAR_MSG_NEW_TASK		3		/*RT->A demande lancement task*/
define MTAR_MSG_ECRIRE_VAR_CTX		4		/*RT->A reception CTX VAR*/
define MTAR_MSG_LIRE_VAR_CTX		5		/*RT->A demande valeur CTX VAR*/
/*
define MTAR_MSG_RET_VAR_CTX		6		/*A->RT envoi valeur CTX VAR*
define MTAR_MSG_RET_TASK		7		/*A->RT retour task lancee*
*/
define MTAR_MSG_FIN_TACHE		8		/*a-<Rt fin de la tache gesihm*/


/* Commandes d'Applix a la tache relais */
/* (les fonctions executables par MTAR_MSG_EXEC sont recensees ci-dessous) */
define MTAR_MSG_EXEC			11		/* executer une fonction C */
define MTAR_MSG_SEND			12		/* Envoyer un message sur le reseau RT */

/* Liste des sous-fonctions specifiques a la GTC tunnels */
#define MTTU_FCT_SIG			1		/* Identifiant de la fonction signalisation  */
#define MTTU_FCT_VEN			2		/* Identifiant de la fonction ventilation  */
#define MTTU_FCT_ECL			3		/* Identifiant de la fonction éclairage  */
#define MTTU_FCT_ENE			4		/* Identifiant de la fonction énergie  */
#define MTTU_FCT_EXP			5		/* Identifiant de la fonction exploitation  */
#define MTTU_FCT_LIT			6		/* Identifiant de la fonction lit arret  */
#define MTTU_FCT_LT				7		/* Identifiant de la fonction locaux techniques  */
#define MTTU_FCT_SYS			8		/* Identifiant de la fonction systeme  */
#define MTTU_FCT_SYN			9		/* Identifiant de la fonction synthese  */

/*fonctions dont Applix demande l'execution a la tache relais*/
define MTAR_FCT_LEAVEDG			6		/*quitter un datagroup  numero  reserve*/
define MTAR_FCT_TRACE			10		/*  ????  */
define MTAR_FCT_PRINT			11		/*demande d'impression*/
define MTAR_FCT_LANCEMCE		12		/*lancer menu de l'ecran textuel*/
define MTAR_FCT_XZDD02			13		/* Demander calcul du plan d'actions*/
define MTAR_FCT_LOGIN			14		/*afficher boite de login*/
define  MTAR_FCT_OUVREFMC		15		/*ouvrir fiche fmc*/
define MTAR_FCT_DEMANDESURCHARGE	16		/*demande de surcharge au CI*/
define MTAR_FCT_REPONSESURCHARGE	17		/*reponse a cette demande par le CI*/
define MTAR_FCT_FINSURCHARGE		18		/*fin de la surcharge par le CI*/

define MTAR_FCT_TIME			20		/*  ????  */
define MTAR_FCT_XZEA28			21		/* Appel XZEA28_Abt_Etat_Contraste */
define MTAR_FCT_XZEC02			22		/* Appel XZEC02_commande_contraste */
define MTAR_FCT_XZEV01			23		/* Appel XZEV01_Affect_Canal_Moniteur */
define MTAR_FCT_XZEV02			24		/* Appel XZEV02_Affecte_Image_Monit_Joy */
define MTAR_FCT_XZEV03			25		/* Appel XZEV03_Positionnement */
define MTAR_FCT_XZEV04			26		/* Appel XZEV04_Memorise_Position */
define MTAR_FCT_XZEV05			27		/* Appel XZEV05_Eteint_Moniteur */
define MTAR_FCT_XZEV06			28		/* Appel XZEV06_Enreg_Magneto */
define MTAR_FCT_XZEV07			29		/* Appel XZEV07_Arret_Magneto */
define MTAR_FCT_XZEA13			30		/* Appel XZEA13_Abt_Etat_Magnetos */
define MTAR_FCT_XZET01			31		/* Appel XZET01_Commande_SIGT */
define MTAR_FCT_XZET02			32		/* Appel XZET02_Commande_SIGE */
define MTAR_FCT_XZET03			98		/* Appel XZET03               */
define MTAR_FCT_XZEA18			33		/* Appel XZEA18_Abt_Etat_Panneaux */
define MTAR_FCT_XZEA22			34		/* Appel XZEA22_Abt_Telealarmes */
define MTAR_FCT_XZEA23			35		/* Appel XZEA23_Abt_Etat_Seq_Tunnel */
define MTAR_FCT_XZEA24			36		/* Appel XZEA24_Abt_Etat_Seq_Echangeur */
define MTAR_FCT_XZAC05			37		/* Appel XZAC05_Fin_Cmd_Manuelle_Tunnel */
define MTAR_FCT_XZAC07			38		/* Appel XZAC07_Fin_Cmd_Manuelle_Echangeur */
define MTAR_FCT_XZEA10			39		/* Appel XZEA10_Abt_Etat_Panneau_NAV */
define MTAR_FCT_XZEN01			40		/* Appel XZEN01_Commande_NAV */
define MTAR_FCT_XZEN02			41		/* Appel XZEN02_Commande_SCEN */
define MTAR_FCT_XZEN03			42		/* Appel XZEN03_Config_Matrice_Test */
define MTAR_FCT_XZEN04			43		/* Appel XZEN04_Arret_NAV */
define MTAR_FCT_XZEA08			44		/* Appel XZEA08_Abt_Etat_PMV */
define MTAR_FCT_XZEP01			45		/* Appel XZEP01_Commande_PMV */
define MTAR_FCT_XZEP01_ARRET		46		/* Appel XZEP01_Commande_PMV en mode arret */
define MTAR_FCT_XZEP02			47		/* Appel XZEP02_Commande_Picto */
define MTAR_FCT_XZEA09			48		/* Appel XZEA08_Abt_Etat_PMV */
define MTAR_FCT_XZEX01			49		/* Appel XZEX01 */
define MTAR_FCT_XZEX02			50		/* Appel XZEX02 */
define MTAR_FCT_XZEA12			51		/* Abonnement Autotel */
define MTAR_FCT_XZEO01			52		/* Appel Bip numerique*/
define MTAR_FCT_XZEO02			53		/* Appel Bip alphanum.*/
define MTAR_FCT_XZEA25			54		/* Abonnement etat PAU*/
define MTAR_FCT_XZEU00			55      /* Pseudo-fonction PAU*/
define MTAR_FCT_XZEU01			56          /* Commande d'un PAU */
define MTAR_FCT_XZEU02			57          /* Commande d'un PAU */
define MTAR_FCT_XZEU03			58          /* Commande d'un PAU */
define MTAR_FCT_XZEU04			59          /* Commande d'un PAU */
define MTAR_FCT_XZEU05			60          /* Commande d'un PAU */
define MTAR_FCT_XZEU06			61          /* Commande d'un PAU */
define MTAR_FCT_XZEU07			62          /* Commande d'un PAU */
define MTAR_FCT_XZEU08			63          /* Commande d'un PAU */
define MTAR_FCT_XZEU09			64          /* Commande d'un PAU */
define MTAR_FCT_XZEA30			65		/* Abonnement Operator*/
define MTAR_FCT_XZEL01			66  		/* Appel Telephonique */
define MTAR_FCT_XZEL02			67		/* Renvoi Appel Repondeur */
define MTAR_FCT_XZEL03			68		/* Annul Renvoi Appel */
define MTAR_FCT_XZCA02_03		69		/* Sauv/restaur histo.*/
define MTAR_FCT_XZEA14			70		/* Abonnement RAU */
define MTAR_FCT_XZEA15			71		/* Appel Abonnement Etat Camera */
define MTAR_FCT_XZEC00			72      /* Pseudo-fonction equipement*/
define MTAR_FCT_XZEC01			73      /* Commande de reconfig. machine */
define MTAR_FCT_XZEC05			74      /* Commande d'arret/demarrage taches */
define MTAR_FCT_XZAA10			75      /* Equation d'alerte */
define MTAR_FCT_XZEL02_03		76	/* Commande du repondeur telephonique */
define MTAR_FCT_MSG_ADM			77	/* Etat du repondeur telephonique */
define MTAR_FCT_Jour_Nuit		78	/* Fonction Jour - Nuit */
define MTAR_FCT_SONALERTE		79	/* Fonction bip */
define MTAR_FCT_DONINDIV			80	/* demande de donnees individuelles*/
define MTAR_FCT_LOGINDONE		81
define MTAR_FCT_XZEA26			82	/* Appel XZEA26_Abt_Etat_Mat_NAV */
define MTAR_FCT_ARRET_SYNOP		83	/* Arret de la tache synoptique tisyn */
define MTAR_FCT_RELANCER_SYNOP	84	/* Relance de la tache synoptique tisyn */
define MTAR_FCT_FIN_POSTE		85	/*demande de fin de poste envoyee au PC simplifie*/
define MTAR_FCT_ETAT_SIMPLIFIE		86	/*abonnement aux etats des PC simplifies*/
define MTAR_FCT_PRESENCE_PCS		87	/*presence d'un operateur dans un PC simplifie*/
define MTAR_FCT_XZST10			88	/*appel a XZST10_Archiver_MsgSyst*/
define MTAR_FCT_NUM_SITE		89	/*transmssion du numero du site a titbi ou titmo*/
define MTAR_FCT_ACQ_VOISIN		90	/*Demande d'acquittement d'un evt au voisin*/
define MTAR_FCT_XZEZ01			91	/*Demande calcul temps de parcours pour PMV */
define MTAR_FCT_INITPRES		92	/*demande init presence operateur*/
define MTAR_FCT_XZEB01			93	/* Commande d'un PAL */
define MTAR_FCT_XZEA38			94	/* Abonnement PAL */
define MTAR_FCT_DEMCAT			95	/* demande import catalog */
define MTAR_FCT_EXPCATF			96	/* demande export catalog Fournisseur*/
define MTAR_FCT_EXPCATC			97	/* demande export catalog Client */

/*PMVA*/
define MTAR_FCT_XZEA080                 99      /*appel XZEA080_Abt_Etat_PMVA*/
define MTAR_FCT_XZEP04                  101     /*appel XZEP04_Commande_PMVA*/
define MTAR_FCT_XZEP04_ARRET            102     /*appel XZEP04_Commande_PMVA mode arret*/
define MTAR_FCT_XZEA40                  107     /*appel XZAEA40_Abt_Etat_Panneau_BAF*/
define MTAR_FCT_XZEA41                  108     /*appel XZAEA41_Abt_Etat_Panneau_BAD*/
define MTAR_FCT_XZEU10			109	
define MTAR_FCT_XZEN10                  103      /*appel XZEN10_Commande_BAF*/
define MTAR_FCT_XZEN11                  104     /*appel XZEN11_Commande_BAD*/
define MTAR_FCT_XZEN13                  105     /*appel XZEN13_Commande_SCEN_BAD*/
define MTAR_FCT_XZEN15                  106     /*appel XZEN15_Arret_BAD*/
define MTAR_FCT_XZEN16                  107      /*appel XZEN10_Commande_PRV*/

/*#define MTAR_DOM_TTU			97
#define MTAR_DOM_ECC			98 */
#define MTAR_DOM_TTU			110		/* Domaine tunnel */
#define MTAR_DOM_ECC			111		/* Domaine echangeur */

#define MTAR_FCT_MSQ                    116
#define MTAR_FCT_VUE                    117

#define MTAR_FCT_XZEL04                 118
#define MTAR_FCT_XZEL05                 119

#define MTAR_FCT_TDP_MZ                 120
#define MTAR_FCT_F2T                    121
#define MTAR_FCT_F2T_REF                122
#define MTAR_FCT_INRETS                 123
#define MTAR_FCT_XZEL06                 124
#define MTAR_FCT_PLANSECOURS            125
#define MTAR_FCT_TIMER_TDP              126

#define MTAR_FCT_XZEL10                 127 /*appel XZEL10_Acquitter_Communautaire*/
#define MTAR_FCT_XZEL11                 128 /*appel XZEL11_Rapprocher_Communautaire*/
#define MTAR_FCT_XZEL12                 129 /*appel XZEL12_Ignorer_Communautaire*/

#define MTAR_FCT_XZEA081		136 /*etat PRV A Supprimer a terme */

#define MTAR_FCT_XZEG01			130 /*commande regul*/
#define MTAR_FCT_XZEL13			131 /*appel XZEL13_Acquitter_Alerte_Regulation*/
#define MTAR_FCT_XZAV03			132 /* appel XZAV03_demarrer_simulation */
#define MTAR_FCT_XZEA70			135 /*etat PRV */
define MTAR_FCT_XZEZ04			136	/*Demande calcul temps de parcours pour PMVA */
#define MTAR_FCT_FACTIS			140
#define MTAR_FCT_XZEA180		141
#define MTAR_FCT_XZEP104		142
#define MTAR_FCT_XZEU11			143
#define MTAR_FCT_XZEL14			144

#define MTAR_FCT_TIGMOT_VERIF_COHERENCE	145
#define MTAR_FCT_TIGMOT_SAUVE_CFG	146
#define MTAR_FCT_TIGMOT_DUPLIQUE_CFG	147
#define MTAR_FCT_TIGMOT_MODIF_BASE	148
#define MTAR_FCT_XZEP07			150		/* Appel XZEP07_Commande_CFE */
#define MTAR_FCT_XZEA082		149		/* Abonnement etat CFE */

#define MTAR_FCT_GABEB			151
#define MTAR_FCT_XZIS50			152
#define MTAR_FCT_XZIS53			153
#define MTAR_FCT_XZEL16			155
#define MTAR_FCT_TDP_SURCHARGE		156
#define MTAR_FCT_TDP_INIT_SURCHARGE	157

#define MTAR_FCT_XZEA084		158		/* Appel XZEA084_Abt_Etat_IMU */
#define MTAR_FCT_XZEP08			159		/* Appel XZEP08_Commande_IMU */

#define MTAR_FCT_XZEP09                 160             /* Appel XZEP09_Commande_SONO */
#define MTAR_FCT_XZEA085                161             /* Appel XZEA085_Abt_Etat_SONO */

#define MTAR_FCT_AVA_RETOUR_ALERTE	162		/* retour après qualification alerte AVA  */

/*pokes*/
define 	PK_SOC_ENVOI			100
define 	PK_SOC_RESULT			200



/*numero des messages Applix*/
define	MTAR_NUM_MSG			4300
define	MTAR_NUM_JOINDG	 		4301
define	MTAR_NUM_SENDPOKE		4302
define	MTAR_NUM_NEWTASK		4303
define	MTAR_NUM_SETVARCTX		4304

/*caractere de separation des arguments d'une macro*/
#define MTAR_FCT_XZET01			31		/* Appel XZET01_Commande_SIGT */
#define MTAR_FCT_XZET02			32		/* Appel XZET02_Commande_SIGE */
#define MTAR_FCT_XZET03			95		/* Appel XZET03_Commande_SEQT */
#define MTAR_FCT_XZET04			96		/* Appel XZET04_Commande_SEQE */

#define MTAR_FCT_XZET05			100		/* Appel XZET05_Commande_SEQ_LTN */
#define MTAR_FCT_XZET06			101		/* Appel XZET06_Commande_SEQ_HT */

/*caractere de separation des arguments d'une macro*/
define MTAR_CAR_SEPAR	"|"

#define MTAR_FCT_XZEA18			33		/* Appel XZEA18_Abt_Etat_Panneaux */
#define MTAR_FCT_XZEA22			34		/* Appel XZEA22_Abt_Telealarmes */
#define MTAR_FCT_XZEA23			35		/* Appel XZEA23_Abt_Etat_Seq_Tunnel */
#define MTAR_FCT_XZEA24			36		/* Appel XZEA24_Abt_Etat_Seq_Echangeur */
#define MTAR_FCT_XZEA25			100		/* Appel XZEA25_Abt_Etat_Seq_LTN */
#define MTAR_FCT_XZEA26			101		/* Appel XZEA26_Abt_Etat_Seq_HT */

#define MTAR_MSG_WIZ_FERMETURE		10000		/*a-<Rt fin de la tache gesihm*/
#define MTTU_MSG_FERM_FENETRE		10010		/*a-<Rt fermeture fenetre*/
#define MTTU_MSG_OUV_FENETRE		10020		/*a-<Rt ouverture fenetre*/
