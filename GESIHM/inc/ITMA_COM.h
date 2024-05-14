/*E*/
/* Fichier : $Id: ITMA_COM.h,v 1.109 2021/10/20 16:16:40 pc2dpdy Exp $        Release : $Revision: 1.109 $        Date : $Date: 2021/10/20 16:16:40 $
-------------------------------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
-------------------------------------------------------------------------------
* SOUS-SYSTEME  GESIHM
-------------------------------------------------------------------------------
* MODULE ITMA_COM * FICHIER ITMA_COM.h
-------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module de donnees et primitives communes aux differents modules de la tache
* IHM textuelle TITMA.
*
*   Non decrit dans le DCG
*
-------------------------------------------------------------------------------
* HISTORIQUE :
*
* Lecrivain	30 Aug 1994 : Creation
* Noel Bernard  05 Oct 1994 : ajout du canal de MTMC		           V1.20
*				Chgt structure COM_localisation
*				et COM_Intitule_FMC
* Lecrivain	05 Oct 1994 : Ajout du format COM_Alarme
* Lecrivain	10 Oct 1994 : Ajout de canaux de comm. inter-modules       V1.26
* Lecrivain	11 Oct 1994 : Ajout du car. separateur de champs           V1.28
* Cornu		11 Oct 1994 : Ajout du cannal poke pour XZEC               V1.29
* Noel		12 Oct 1994 : suppression de la definition des
*				canaux que l'on retrouve sous
*				XDMICG/inc/xzic_ax.h	 		   V1.33
* Lecrivain	24 Oct 1994 : Ajout de types (interface SYBASE)            V1.35
* Lecrivain	27 Oct 1994 : Ajout constante COM_REFUSE                   V1.37
* Lecrivain	02 Nov 1994 : Ajout types identifiant evenement            V1.38
* Lecrivain	02 Nov 1994 : Ajout types identifiant plan d'action        V1.39
* Lecrivain	08 Nov 1994 : Ajout des types des identifiants
*                                 d'equipement et de type d'equipement     V1.41
* Lecrivain	08 Nov 1994 : Ajout du type identifiant district           V1.42
* Lecrivain	08 Nov 1994 : Ajout type identifiant type evenement        V1.43
* Lecrivain	09 Nov 1994 : Ajout types identifiants operateur,
*                                 poste oper, motif appel, action TFM,...  V1.44
* Lecrivain	09 Nov 1994 : Ajout constantes d'erreur                    V1.45
* Lecrivain	09 Nov 1994 : Modif. constantes de connexion au SGBD       V1.47
* Lecrivain	24 Nov 1994 : Surcharge des types Sybase INT et BIT        V1.48
* Lecrivain	01 Dec 1994 : Ajout des types d'id. et site action         V1.50
* Lecrivain	01 Dec 1994 : Ajout du type du type d'intervention         V1.51
* Lecrivain	16 Dec 1994 : Ajout du format des infos equipements        V1.52
* Lecrivain	19 Dec 1994 : Definition du repertoire de pictogrammes     V1.53
* Lecrivain     21 Dec 1994 : Changement repertoire des pictogrammes       V1.54
* Guilhou	26 dec 1994 : ajout titres colonnes de la liste des causes V1.56
* Lecrivain     27 Dec 1994 : Ajout constantes de tailles de la fenetre    V1.57
* Cornu         13 Jan 1995 : Ajout constante d'appel repondeur pour NAV   V1.59
* Guilhou	17 jan 1995 : ajout modes de formation			V1.60
* Lecrivain     20 Fev 1995 : Ajout constante de format d'impression      V1.66
* Lecrivain     20 Fev 1995 : Ajout ctes de repertoires des fichiers V1.67,1.68
* Lecrivain     14 Avr 1995 : Ajout mode d'impression PostScript          V1.71
* Lecrivain     31 Mai 1995 : Ajout du sous-repertoire d'infos            V1.72
* Torregrossa   16 Jun 1995 : Ajout COM_SENS_EQUIPEMENT                   V1.73
* Guilhou   	04 Sep 1995 : Ajout COM_SOUS_REP_LOCAL                    V1.74
* Noel		21 Sep 1995 : Ajout COM_COULEUR
*				    COM_NOIR_BLANC			  V1.75
* Torregrossa   05 Oct 1995 : Ajout COM_FICH_MENU_SPREADSHEET             V1.76
* Torregrossa   20 Nov 1995 : Ajout COM_SOUS_REP_PMV	             V1.78
* Torregrossa   25 Jan 1996 : Ajout COM_LIB_FMC_OUVERTE	             V1.79
*                             et modif libelle COM_LIB_DEJA_OUVERTE
* Noel		06 Fev 1996 : Ajout de la description du poke 201	  V1.80
* Torregrossa   04 Mar 1996 : Ajout COM_NB_MIN_CAR_PWD	             V1.81
* Noel		29 Avr 1996 : Ajout COM_SOUS_REP_CONFIG_IGN		V1.82
*				et COM_SOUS_REP_IGN (DEM1096)
* Guilhou	19 sep 1996 : ajout C_LIB_PERTE_CI (RADT)	V1.85
* Torregrossa   17 Oct 1996 : Ajout format et define sur type de carburants V1.86
* Niepceron	04 Nov 1996 : ajout du rep d'acces a NTFM (dem/1248) v1.87
* Guilhou	05 nov 1996 : ajout date debut prevu dans COM_Intitule_FMC (DEM/1281) 1.88
* Niepceron     13 Jan 1997 : ajout du rep d'acces a OPER (dem/1285) v1.89
* JPL		23/07/07 : Ajout du repertoire d'acces a "deq" (DEM 672) v1.90
* JPL		24/07/07 : Ajout code/libelle erreur "fichier non trouve" v1.91
* JMG		13/11/07 : ajout site de gestion DEM715 v1.92
* JPL		20/11/07 : Ajout sous-repertoire 'BDC' v1.93
* JBL		01/12/07 : gtc renovee DEM 665
* ABE		05/06/08 : gtc renovee spécificité tunnel MONACO
* JPL		23/06/08 : Ajout sous-repertoire 'RENOV' (pour DEM 783) v1.96
* ABE		05/01/09 : inversion valeurs constantes COM_SCEN_VENT_SS_CONGS et COM_SCEN_VENT_AV_CONGS
* JPL		04/09/09 : Ajout constantes EV_TBL_xxx manquant dans en-tete APPLIX (evts. sur widgets) v1.98
* JPL		19/11/09 : Ajout sous-repertoire 'SCH_ECOULT' (pour DEM 919) v1.99
* ABE   20/10/09 : ajout de la constante "DEFINE	COM_SC_VT_MON_ARR_PURGE	44"
* JPL		01/10/10 : Ajout format COM_Parametres_Requete 1.101
* ???		xx/12/10 : Ajout constantes pour Linux  1.102
* JPL		09/03/11 : Modification constante COM_CANAL_MTOU_FIN_TRT (conflit avec communs)  1.103
* JPL		10/10/11 : Ajout champ "origine_creation" au format COM_Intitule_FMC (DEM 995)  1.104
* JMG		11/01/12	: correction base Linux 1.104
* JPL		06/03/12 : Ajout constantes pour selection unique / multiple (DEM 1028)  1.105
* JPL		23/03/16 : Ajout sous-repertoire COM_SOUS_REP_MOD (pour DEM 1155)  1.107
* ABE		14/11/19 : Ajout ouvrage TOULON non generique comme les ouvrages de A8
* ABE		14/10/21 : Ajout Sous rep fichiers/adm DEM-SAE228 1.108
---------------------------------------------------------------------------- */

/*A ***************************************************************************
 **                                                                          **
 **              Surcharge des types Sybase INT1_, INT2_ et BIT_             **
 **             (donc a include APRES le fichier Applix dbase_.am)           **
 **                                                                          **
 *****************************************************************************/

DEFINE	SYB#INT1_	56					' ie SYB#INT4_
DEFINE	SYB#INT2_	56
DEFINE	SYB#BIT_	56



/*A ***************************************************************************
 **                                                                          **
 **    Definition des constantes d'evenements sur les objets graphiques,     **
 **    manquants dans les fichiers d'en-tete Applix (hormis les exemples)    **
 **                                                                          **
 *****************************************************************************/

DEFINE	EV_TBL_SELECTION_CHANGE_	50
DEFINE	EV_TBL_DBLCLICK_		51
DEFINE	EV_TBL_KEYPRESS_		55
DEFINE	EV_TBL_COLUMN_RESIZE_		56
DEFINE	EV_TBL_COPY_TO_CLIPBOARD_	58
DEFINE	EV_TBL_PASTE_FROM_CLIPBOARD_	59
DEFINE	EV_TBL_BUTTON_PRESS_		60



/*A ***************************************************************************
 **                                                                          **
 **           Definition de constantes communes a tous les modules           **
 **                                                                          **
 *****************************************************************************/

/*A
 * Codes de retour de fonctions, en particulier OK, NOK
 * et erreur Applix (DIFFERENT des codes d'erreur SYBASE)
 * ------------------------------------------------------
 */

DEFINE	COM_ERR_APPLIX	(-9999)
DEFINE	COM_OK		0
DEFINE	COM_NOK		1
DEFINE	COM_QUITTE	2
DEFINE	COM_VALIDE	3
DEFINE	COM_REFUSE	4
DEFINE	COM_SUPPRIME	5


/*A
** Valeurs de criteres de selection
** --------------------------------
*/

DEFINE	COM_CHOIX_AUCUN		0
DEFINE	COM_CHOIX_UNIQUE	1
DEFINE	COM_CHOIX_MULTIPLE	99


/*A
** Coordonnees de positions "standard" des fenetres a l'ecran
** ----------------------------------------------------------
*/

DEFINE	COM_POS_X		283
DEFINE	COM_POS_Y_MENU	 75
DEFINE	COM_POS_Y_FEN	225


/*A
 * Definition des codes (compris entre 1 et 1000) et
 * des libelles d'erreurs Applix globales a l'application
 * ------------------------------------------------------
 */

DEFINE	COM_ERREUR_SGBD 			1000
DEFINE	COM_LIBELLE_ERREUR_SGBD		"Erreur SGBD"

DEFINE	COM_ERR_NON_SAISI			999
DEFINE	COM_LIB_NON_SAISI			"Saisie obligatoire"

DEFINE	COM_ERREUR_PROC_SQL			998
DEFINE	COM_LIB_ERR_PROC_SQL_NOK		"Exécution requête erronée"
DEFINE	COM_LIB_ERR_PROC_SQL_ARG_INV		"Arguments de requête invalides"
DEFINE	COM_LIB_ERR_PROC_SQL_PRC_INC		"Requête inconnue"

DEFINE	COM_ERR_DATE_INVAL			997
DEFINE	COM_LIB_DATE_INVAL			"Date invalide"

DEFINE	COM_ERR_VAL_INVAL			996
DEFINE	COM_LIB_VAL_INVAL			"Valeur invalide"

DEFINE	COM_ERR_DEJA_OUVERTE			995
DEFINE  COM_LIB_FMC_OUVERTE       		"Terminer d'abord la fiche en cours"
DEFINE	COM_LIB_DEJA_OUVERTE			"Fenêtre déjà ouverte"

DEFINE  COM_ERR_FICH_NON_TROUVE			994
DEFINE  COM_LIB_FICH_NON_TROUVE			"Fichier non trouvé"

DEFINE	C_LIB_PERTE_CI				"Fonction indisponible! Perte de communication avec le CI"

/*A
 * Definition des niveaux d'erreur
 * -------------------------------
 */

DEFINE	COM_DEBUG1	0x00000001
DEFINE	COM_DEBUG2	0x00000002
DEFINE	COM_DEBUG3	0x00000004
DEFINE	COM_FONCTION	0x00000008			' Trace appels internes
DEFINE	COM_INFO		0x00000010			' Integration
DEFINE	COM_MESSAGE	0x00000020			' Communication
DEFINE	COM_SGBD		0x00000040			' Erreurs SGBD
DEFINE	COM_WARNING   	0x00000080			' Erreur non fatale
DEFINE	COM_FATAL		0x00000100			' Arret total
DEFINE	COM_INTERFACE	0x00000200			' Trace appels externes


DEFINE       COM_HOTE_LX     "SDCI1"
DEFINE       COM_HOTE_HIS_LX     "SDHIS"
DEFINE       COM_SERVEUR_LX  ""


/*A
 * Pour definition des caracteres valides lors de
 * saisies numeriques (en particulier de dates)
 * ----------------------------------------------
 */

DEFINE	COM_CHIFFRES	"0123456789"



/*A
 * Definition du caractere separateur de champs dans
 * les chaines de caracteres echangees entre modules
 * -------------------------------------------------
 */

DEFINE	COM_SEPAR_CHAMPS	"|"



/*A
 * Definition de la longueur d'un troncon de
 * texte et du caractere separateur de lignes
 * dans les textes stockes en base de donnees
 * ------------------------------------------
 */

DEFINE	COM_LG_TXT	250
DEFINE	COM_SEPAR_TXT	"\n"


/*A titres des colonnes de la liste des causes*/
DEFINE	COM_CAUSE_NUMERO		"Numéro"
DEFINE	COM_CAUSE_CLE			"Créé au"
DEFINE	COM_CAUSE_TYPE			"Type"
DEFINE	COM_CAUSE_DATE			"Date de début"
DEFINE	COM_CAUSE_LOCALISATION 	"Localisation"
DEFINE 	COM_CAUSE_NULL			""

DEFINE	COM_CAUSE_LONG_NUMERO		54
DEFINE	COM_CAUSE_LONG_CLE			55
DEFINE	COM_CAUSE_LONG_TYPE			105
DEFINE	COM_CAUSE_LONG_DATE			106
DEFINE	COM_CAUSE_LONG_LOCALISATION 	182
DEFINE 	COM_CAUSE_LONG_NULL			0

/*libelle des valeurs de la veracite */
DEFINE	COM_LIB_FMC_SIGNALEE		"signalée"
DEFINE	COM_LIB_FMC_CONFIRMEE		"confirmée"

/*A modes de formation*/
DEFINE	COM_FORMATION_CREATION		1
DEFINE	COM_FORMATION_ACTIF			2
DEFINE 	COM_FORMATION_PASSIF		3
DEFINE	COM_FORMATION_DIDACTIQUE		4



/*A ***************************************************************************
 **                                                                          **
 **              Definition de types communs a tous les modules              **
 **                                                                          **
 *****************************************************************************/

DEFINE	COM_Identifiant_Autoroute	SYB#INT1_

DEFINE	COM_Identifiant_PR		SYB#INT4_

DEFINE	COM_Identifiant_Sens_Circul	SYB#INT1_


DEFINE	COM_Identifiant_District	SYB#INT1_


DEFINE	COM_Identifiant_Echangeur	SYB#INT2_


DEFINE	COM_Identifiant_Type_Equip	SYB#INT1_
DEFINE	COM_Identifiant_Equipement	SYB#INT2_

DEFINE	COM_Identifiant_Critere_Equip	SYB#INT4_


DEFINE	COM_Identifiant_Type_Evenement	SYB#INT2_
DEFINE	COM_Numero_Evenement		SYB#INT4_
DEFINE	COM_Cle_Evenement		SYB#INT1_


DEFINE	COM_Numero_PA			SYB#INT2_
DEFINE	COM_Numero_Ordre_PA		SYB#INT1_


DEFINE	COM_Identifiant_Operateur	SYB#INT2_


DEFINE	COM_Identifiant_Poste_Oper	SYB#INT2_


DEFINE	COM_Identifiant_Motif_Appel	SYB#INT1_


DEFINE	COM_Identifiant_Action_TFM	SYB#INT4_


DEFINE	COM_Identifiant_Action		SYB#INT4_
DEFINE	COM_Identifiant_Site_Action	SYB#INT1_
DEFINE	COM_Identifiant_Type_Interv	SYB#INT1_


/*A ***************************************************************************
 **                                                                          **
 **              Formats des donnees echangees entre modules IHM             **
 **                                                                          **
 *****************************************************************************/

/*A
 * Format des donnees obligatoires des parametres d'une requete au SGBD
 * --------------------------------------------------------------------
 */

FORMAT  COM_Parametres_Requete
	sortie,					' Indicateur d'un parametre retourne si vrai
	type,					' Type du parametre, (si parametre d'entree)
	valeur					' La valeur fournise, si parametre d'entree



/*A
 * Format d'un identifiant de fiche main courante
 * ----------------------------------------------
 */

FORMAT  COM_Identifiant_FMC
	numero,					' Couple d'entiers identifiant
	cle					' de maniere unique un evenement



/*A
 * Format d'un intitule de fiche main courante
 * -------------------------------------------
 */

FORMAT	COM_Localisation
	NumAuto,
	PR,
	sens_circulation,
	type_point_caract,
	point_caract,
	position

FORMAT	COM_Intitule_FMC
	FORMAT	COM_Identifiant_FMC	IdFMC,
	Numtype,
	FORMAT COM_Localisation localisation,
	date,
	date_prevu,
	origine_creation



/*A
 * Format d'un identifiant d'equipement
 * ------------------------------------
 */

FORMAT	COM_Identifiant_Equip
	type,
	numero



/*A
 * Format d'un identifiant d'alarme
 * --------------------------------
 */

FORMAT	COM_Identifiant_Alarme
	num_type_alarme,
	num_type_equip,
	num_equip



/*A
 * Format des donnees de carburants
 * --------------------------------
 */

DEFINE	COM_NUMERO_CARBURANT	0
DEFINE	COM_NOM_CARBURANT	1

FORMAT	COM_Carburants
	numero,
	nom,
	supprime

/*A
 * Format des donnees equipements lues en base de donnees
 * ------------------------------------------------------
 */

DEFINE	COM_NUMERO_EQUIPEMENT	1
DEFINE	COM_NOM_EQUIPEMENT	2
DEFINE	COM_AUTO_EQUIPEMENT	3
DEFINE	COM_PR_EQUIPEMENT	4
DEFINE	COM_SENS_EQUIPEMENT	5

FORMAT	COM_Donnees_Equipements
	type,					' Type de l'equipement (entier)
	numero,					' Numero de l'equipement
	nom,					' Nom de l'equipement
	autoroute,
	PR,
	sens,
	dispo,
	date,
	sitegestion



/*A ***************************************************************************
 **                                                                          **
 **                       Parametrage de l'application                       **
 **                                                                          **
 *****************************************************************************/

/*A
 * Localisation des fichiers
 * -------------------------
 */

DEFINE	COM_SOUS_REP_TRACE	"traces/"

DEFINE	COM_SOUS_REP_AIDE	"fichiers/aid/"
DEFINE	COM_SUFFIXE_AIDE	".aide"

DEFINE	COM_SOUS_REP_PMV	"fichiers/pmv/"
DEFINE	COM_SOUS_REP_PICTOS	"fichiers/pic/"
DEFINE	COM_SOUS_REP_IHM	"fichiers/syn/"
DEFINE	COM_SOUS_REP_INFO	"fichiers/inf/"
DEFINE	COM_SOUS_REP_IGN	"fichiers/ign/"
DEFINE	COM_SOUS_REP_DYN	"fichiers/dyn/"
DEFINE	COM_SOUS_REP_MBO	"fichiers/mbo/"
DEFINE	COM_SOUS_REP_MOD	"fichiers/mod/"
DEFINE  COM_SOUS_REP_NTFM	"fichiers/ntfm"
DEFINE  COM_SOUS_REP_OPER	"fichiers/opr/"
DEFINE  COM_SOUS_REP_DEQ	"fichiers/deq/"
DEFINE	COM_SOUS_REP_BDC	"fichiers/bdc/"
DEFINE	COM_SOUS_REP_RENOV	"fichiers/renov/"
DEFINE	COM_SOUS_REP_SCH_ECOULT	"fichiers/sch/"
DEFINE	COM_SOUS_REP_ADM	"fichiers/adm/"

DEFINE	COM_SOUS_REP_LOCAL	"fichiers/travaux/local/"
DEFINE	COM_SOUS_REP_PUBLIC	"fichiers/travaux/public/"
DEFINE	COM_SOUS_REP_CONFIG     "modif/fichiers/"
DEFINE	COM_SOUS_REP_CONFIG_IGN	"modif/fichiers/ign/"


/*A
 * Noms des fichiers de config applix
 * ----------------------------------
 */

/* Nom du fichier contenant les pictogrammes utilises */
DEFINE COM_FICH_CONFIG_PICTO		"Config_Pictogrammes.cfg"



/*A
 * Parametres de connexion a la base de donnees
 * --------------------------------------------
 */

DEFINE	COM_NOM_SGBD	"Sybase"
DEFINE	COM_USER	"MIGRAZUR"
DEFINE	COM_MOT_PASSE	"MIGRAZUR"
DEFINE	COM_BASE	"PRC"
DEFINE	COM_HOTE	""		' ... ie nom machine !
DEFINE	COM_SERVEUR	"SQL_"		' auquel ajouter le site

/*A
 * nombre de caracteres minimum pour le mot de passe de l'application
 * ------------------------------------------------------------------
 */
DEFINE	COM_NB_MIN_CAR_PWD	6	'nombre min de caracteres du password


/*A
 * Dimensions maximales d'une fenetre
 * ----------------------------------
 */

DEFINE	COM_LARGEUR_MAX		1264
DEFINE	COM_LARGEUR_FMC		1000

DEFINE	COM_HAUTEUR_MAX		897



/*A
 * Formats d'impression de documents Applix
 * ----------------------------------------
 */

DEFINE	COM_FORMAT_PCL		"PCL5"
DEFINE	COM_FORMAT_PS		"PostScript"



/*A
 * Definition des pokes utilises uniquement dans le sous systeme GESIHM
 *
 * ATTENTION les plages initialement autorisees etaient :
 *   - jusqu'a 200 pour les communs definis dans "xzic.h" ;
 *   - depuis 201 jusqu'a 500 pour GESIHM (textuel + synoptique)
 *   - a partir de 500 pour les fiches métiers.
 *
 * Cependant les communs definis ont depasse les bornes prevues.
 * --------------------------------------------------------------------
 */

DEFINE	COM_CANAL_MTOU_FIN_TRT	500



/*A
 * Definition des scenarios ventilation
 * ------------------------------------
 */

DEFINE	COM_SCEN_VENT_INCENDIE		1
DEFINE	COM_SCEN_VENT_DES_TETE		2
DEFINE	COM_SCEN_VENT_DES_MOI1		3
DEFINE	COM_SCEN_VENT_DES_MOI2		4
DEFINE	COM_SCEN_VENT_SS_CONGS		5
DEFINE	COM_SCEN_VENT_AV_CONGS		6
DEFINE	COM_SCEN_VENT_RECYCLAG		7
DEFINE	COM_SCEN_VENT_ARRET		8

DEFINE	COM_SCEN_VENT_MONACO_1		11
DEFINE	COM_SCEN_VENT_MONACO_2 		12
DEFINE	COM_SCEN_VENT_MONACO_3 		13
DEFINE	COM_SCEN_VENT_MONACO_4 		14
DEFINE	COM_SCEN_VENT_MONACO_5 		15
DEFINE	COM_SCEN_VENT_MONACO_6 		16
DEFINE	COM_SCEN_VENT_MONACO_7 		17
DEFINE	COM_SCEN_VENT_MONACO_8 		18
DEFINE	COM_SCEN_VENT_MONACO_9 		19
DEFINE	COM_SCEN_VENT_MONACO_10		20
DEFINE	COM_SCEN_VENT_MONACO_11		21
DEFINE	COM_SCEN_VENT_MONACO_12		22
DEFINE	COM_SCEN_VENT_MONACO_13		23
DEFINE	COM_SCEN_VENT_MONACO_14		24
DEFINE	COM_SCEN_VENT_MONACO_15		25
DEFINE	COM_SCEN_VENT_MONACO_16		26
DEFINE	COM_SCEN_VENT_MONACO_17		27
DEFINE	COM_SCEN_VENT_MONACO_18		28
DEFINE	COM_SCEN_VENT_MONACO_19		29
DEFINE	COM_SCEN_VENT_MONACO_20		30
DEFINE	COM_SCEN_VENT_MONACO_21		31
DEFINE	COM_SCEN_VENT_MONACO_22		32
DEFINE	COM_SCEN_VENT_MONACO_23		33
DEFINE	COM_SCEN_VENT_MONACO_24		34
DEFINE	COM_SCEN_VENT_MONACO_25		35
DEFINE	COM_SCEN_VENT_MONACO_26		36
DEFINE	COM_SC_VT_MON_INC_NON_LOC	37
DEFINE	COM_SC_VT_MON_ARR_PURGE	44

/*  Definition des scenarios ventilation TOULON  S1 */
DEFINE	COM_SCEN_VENT_AT_TO_1	10
DEFINE	COM_SCEN_VENT_TO_1_1	98
DEFINE	COM_SCEN_VENT_TO_1_2 	95
DEFINE	COM_SCEN_VENT_TO_1_3 	94
DEFINE	COM_SCEN_VENT_TO_1_4 	92
DEFINE	COM_SCEN_VENT_TO_1_5 	123
DEFINE	COM_SCEN_VENT_TO_1_6 	90
DEFINE	COM_SCEN_VENT_TO_1_7 	120
DEFINE	COM_SCEN_VENT_TO_1_8 	88
DEFINE	COM_SCEN_VENT_TO_1_9 	119
DEFINE	COM_SCEN_VENT_TO_1_10	86
DEFINE	COM_SCEN_VENT_TO_1_11	84
DEFINE	COM_SCEN_VENT_TO_1_12	117
DEFINE	COM_SCEN_VENT_TO_1_13	114
DEFINE	COM_SCEN_VENT_TO_1_14	113
DEFINE	COM_SCEN_VENT_TO_1_15	111
DEFINE	COM_SCEN_VENT_TO_1_16	127
DEFINE	COM_SCEN_VENT_TO_1_17	109
DEFINE	COM_SCEN_VENT_TO_1_18	107
DEFINE	COM_SCEN_VENT_TO_1_19	105
DEFINE	COM_SCEN_VENT_TO_1_20	125
DEFINE	COM_SCEN_VENT_TO_1_21	103
DEFINE	COM_SCEN_VENT_TO_1_22	101
DEFINE	COM_SCEN_VENT_TO_1_23	100
DEFINE	COM_SCEN_VENT_TO_1_24	99
DEFINE	COM_SCEN_VENT_TO_1_25	96
DEFINE	COM_SCEN_VENT_TO_1_26	97
DEFINE	COM_SCEN_VENT_TO_1_27	93
DEFINE	COM_SCEN_VENT_TO_1_28	124
DEFINE	COM_SCEN_VENT_TO_1_29	91
DEFINE	COM_SCEN_VENT_TO_1_30	121
DEFINE	COM_SCEN_VENT_TO_1_31	89
DEFINE	COM_SCEN_VENT_TO_1_32	122
DEFINE	COM_SCEN_VENT_TO_1_33	87
DEFINE	COM_SCEN_VENT_TO_1_34	85
DEFINE	COM_SCEN_VENT_TO_1_35	118
DEFINE	COM_SCEN_VENT_TO_1_36	115
DEFINE	COM_SCEN_VENT_TO_1_37	116
DEFINE	COM_SCEN_VENT_TO_1_38	112
DEFINE	COM_SCEN_VENT_TO_1_39	128
DEFINE	COM_SCEN_VENT_TO_1_40	110
DEFINE	COM_SCEN_VENT_TO_1_41	108
DEFINE	COM_SCEN_VENT_TO_1_42	106
DEFINE	COM_SCEN_VENT_TO_1_43	126
DEFINE	COM_SCEN_VENT_TO_1_44	104
DEFINE	COM_SCEN_VENT_TO_1_45	102

/*  Definition des scenarios ventilation TOULON  S1 */
DEFINE	COM_SCEN_VENT_AT_TO_2	10
DEFINE	COM_SCEN_VENT_TO_2_1	65
DEFINE	COM_SCEN_VENT_TO_2_2 	64
DEFINE	COM_SCEN_VENT_TO_2_3 	63
DEFINE	COM_SCEN_VENT_TO_2_4 	62
DEFINE	COM_SCEN_VENT_TO_2_5 	61
DEFINE	COM_SCEN_VENT_TO_2_6 	60
DEFINE	COM_SCEN_VENT_TO_2_7 	59
DEFINE	COM_SCEN_VENT_TO_2_8 	58
DEFINE	COM_SCEN_VENT_TO_2_9 	57
DEFINE	COM_SCEN_VENT_TO_2_10	56
DEFINE	COM_SCEN_VENT_TO_2_11	55
DEFINE	COM_SCEN_VENT_TO_2_12	54
DEFINE	COM_SCEN_VENT_TO_2_13	53
DEFINE	COM_SCEN_VENT_TO_2_14	83
DEFINE	COM_SCEN_VENT_TO_2_15	82
DEFINE	COM_SCEN_VENT_TO_2_16	81
DEFINE	COM_SCEN_VENT_TO_2_17	80
DEFINE	COM_SCEN_VENT_TO_2_18	79
DEFINE	COM_SCEN_VENT_TO_2_19	78
DEFINE	COM_SCEN_VENT_TO_2_20	77
DEFINE	COM_SCEN_VENT_TO_2_21	76
DEFINE	COM_SCEN_VENT_TO_2_22	75
DEFINE	COM_SCEN_VENT_TO_2_23	74
DEFINE	COM_SCEN_VENT_TO_2_24	73
DEFINE	COM_SCEN_VENT_TO_2_25	72
DEFINE	COM_SCEN_VENT_TO_2_26	71
DEFINE	COM_SCEN_VENT_TO_2_27	70
DEFINE	COM_SCEN_VENT_TO_2_28	69
DEFINE	COM_SCEN_VENT_TO_2_29	68
DEFINE	COM_SCEN_VENT_TO_2_30	67
DEFINE	COM_SCEN_VENT_TO_2_31	66
