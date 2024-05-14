/*E*/ 
/*Fichier :  $Id: xzex.h,v 1.118 2021/05/03 14:05:18 pc2dpdy Exp $      Release : $Revision: 1.118 $        Date : $Date: 2021/05/03 14:05:18 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE XZEX  * FICHIER xzex.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  < Structure des donnees du fichier xzex.c >
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	25 Aug 1994	: Creation
* Mismer.D	version 1.2	30 Sep 1994	:
* Nagiel.E	version 1.3	03 Oct 1994	:
* Nagiel.E	version 1.4	03 Oct 1994	:
* Nagiel.E	version 1.5	03 Oct 1994	:
* Nagiel.E	version 1.6	04 Oct 1994	:
* Nagiel.E	version 1.7	07 Oct 1994	:
* Nagiel.E	version 1.8	07 Oct 1994	:
* Nagiel.E	version 1.9	11 Oct 1994	:
* Nagiel.E	version 1.10	11 Oct 1994	:
* Mismer.D	version 1.11	13 Oct 1994	:
* Nagiel.E	version 1.12	13 Oct 1994	: Ajout des types de trame, des priorites, des modes
* Nagiel.E	version 1.13	13 Oct 1994	: Ajout des XZEXC_TRAME_DATE
* Nagiel.E      version 1.14	14 Oct 1994     : ajout de XZEXC_MODE_FONCT_NORMAL et XZEXC_MODE_FONCT_ISOLE
*						: ajout des const NAV
* Nagiel.E	version 1.15	18 Oct 1994	:
* Nagiel.E	version 1.16	20 Oct 1994	: Ajout des XZEXC_TRAME_CONF_DAI
* Nagiel.E	version 1.17	20 Oct 1994	:
* Nagiel.E	version 1.18	21 Oct 1994	:
* Mismer.D	version 1.19	25 Oct 1994	: Ajout des XZEXC_MODE_XXXX
*                                          	: Ajout defs. constantes et types specifiques TELMi
*						: Ajout defs. constantes et types specifiques EX_MTCR
*						: Ajout types erreurs TEDI
* Mismer.D	version 1.20	02 Nov 1994	:
* Nagiel.E	version 1.21	08 Nov 1994	:
* Flaidar.A	version 1.22	08 Nov 1994	:
* Flaidar.A	version 1.23	08 Nov 1994	:
* Flaidar.A	version 1.24	09 Nov 1994	:
* Flaidar.A	version 1.25 	09 Nov 1994	:
* Flaidar.A	version 1.26	09 Nov 1994	:
* Flaidar.A	version 1.27	09 Nov 1994	:
* Flaidar.A	version 1.28	09 Nov 1994	:
* Flaidar.A	version 1.29	09 Nov 1994	:
* Flaidar.A	version 1.30	09 Nov 1994	:
* Nagiel.E	version 1.31	16 Nov 1994	:
* Flaidar.A	version 1.32	16 Nov 1994	:
* Flaidar.A	version 1.33	17 Nov 1995	:
* Flaidar.A	version 1.34	17 Nov 1994	:
* Flaidar.A	version 1.35	17 Nov 1994	:
* Nagiel.E	version 1.36	18 Nov 1994	:
* Nagiel.E	version 1.37	18 Nov 1994	:
* Nagiel.E	version 1.38	25 Nov 1994	:
* Flaidar.A	version 1.39	25 Nov 1994	: ajout vals. defaut timer attente socket Serveur de Terminaux.
* Flaidar.A	version 1.40	25 Nov 1994	: ajout types config. taches protocoles.
* 						: ajout types sokets taches protocoles.
* Mismer.D	version 1.41	28 Nov 1994	: ajout types config ligne et equipement XZEXT_ENG_CONF_LIGNE. 
* Mismer.D	version 1.42	28 Nov 1994	: Suppression des #define XDF
* Mismer.D	version 1.43	30 Nov 1994	:
* Nagiel.E	version 1.44	30 Nov 1994	:
* Mismer.D	version 1.45	30 Nov 1994	:
* Nagiel.E	version 1.46	30 Nov 1994	: ajout cst rau 
* Nagiel.E	version 1.47	02 Dec 1994	:
* Flaidar.A	version 1.48	02 Dec 1994	: ajout types reponse tache traitement vers TELEi
* Nagiel.E	version 1.49	09 Dec 1994	:
* Mismer.D	version 1.50	09 Dec 1994	:
* Mismer.D	version 1.51	12 Dec 1994	: ajout XZEXC_ADR_SOCK_TEXA0
* Mismer.D	version 1.52	13 Dec 1994	: Modif acces repertoire config 
* Nagiel.E	version 1.53	20 Dec 1994	:
* Nagiel.E	version 1.54	20 Dec 1994	:
* Mismer.D	version 1.55	20 Dec 1994	: Modif taille msg socket
* Nagiel.E	version 1.56	21 Dec 1994	:
* Volcic.F	version 1.57	21 Dec 1994	: Ajout constantes pour TERDT et TETDC
* Nagiel.E	version 1.58	26 Dec 1994	:
* Nagiel.E	version 1.59	26 Dec 1994	:
* Volcic.F	version 1.60	29 Dec 1994	: Ajout constantes Path fichiers pour calcul TERDT
* Volcic.F	version 1.61	29 Dec 1994	: Ajout constantes Path fichiers pour calcul TERDT
* Nouaille.M	version 1.62	02 Jan 1995	: Ajout cstes pour telephone et operator
* Mismer.D	version 1.63	04 Jan 1995	: Ajout constante pour commande hors base
* Gabert.P	version 1.64	05 Jan 1995	: Ajout constante pour fichier SATIR
* Mismer.D	version 1.65	13 Jan 1995	:
* Gabert.P	version 1.66	13 Jan 1995	:
* Nagiel.E	version 1.67	16 Jan 1995	:
* Mismer.D	version 1.68	18 Jan 1995	:
* Nagiel.E	version 1.69	19 Jan 1995	:
* Mismer.D	version 1.70	24 Jan 1995	: Modif acces repertoire EQUEXT en relatif
* Mismer.D	version 1.71	25 Jan 1995	:
* Mismer.D	version 1.72	25 Jan 1995	:
* Mismer.D	version 1.73	25 Jan 1995	:
* Mismer.D	version 1.74	30 Jan 1995	:
* Mismer.D	version 1.75	13 Fev 1995	:
* Mismer.D	version 1.76	21 Fev 1995	:
* Mismer.D	version 1.77	08 Mar 1995	:
* Volcic.F	version 1.78	15 Mar 1995	: Modif nom fichier HORODATAGE Satir  
* Fontaine.C	version 1.79 	24 Mai 1995	: Reprise entete, historique
* Volcic.F	version 1.80	08 Jun 1995	: Modif socket TETDC  
* Volcic.F	version 1.81	14 Jun 1995	: Ajout XZEXC_PATH_FIC_EQA 
* Volcic.F	version 1.82	21 Jun 1995	: Ajout XZEXC_PING, XZEXC_BYTES,XZEXC_COUNT_PING,XZEXC_TOKEN_TST 
* Mismer.D	version 1.83	29 Jun 1995	: Ajout XZEXC_DEMANDE_RECONNEXION   
* Mismer.D	version 1.83	07 Aug 1995	: Ajout XZEX04   
* Mismer.D	version 1.84	07 Aug 1995	: Correction XZEX04   
* Mismer.D	version 1.85	21 Aug 1995	: Ajout XZEXC_PORT_SERVEUR_SD   
* Volcic.F	version 1.86	21 Sep 1995	: Ajout XZEX05  
* Volcic.F	version 1.87	04 Oct 1995	: Ajout param horodate dans XZEX05  
* Mismer.D	version 1.88	20 Mar 1996	: Changement heure synchro-date (DEM/1102)
*				        	  (04:05:00 a la place de 03:05:00)
* Mismer.D	version 1.89	06 Sep 1996	: Ajout XZEXC_PATH_REFERENCE et XZEXC_RECONFIG_LCR (RADT_W)  
* Mismer.D	version 1.90	03 Oct 1996	: Modif pour mesures pesage (DEM/1227)
* Mismer.D	version 1.91	24 Oct 1996	: Ajout type XZEXT_CONFIG_PAU pour Alerte PAU HS (DEM/1282)
* Orengo.A	version 1.92	07 Nov 1996	: Ajout type XZEXC_ADR_SOCK_TEQTX, XZEXC_ADR_SOCK_QTXTIM,
*						  XZEXC_ADR_SOCK_QTXTELM, XZEXC_ADR_SOCK_QTXTELE et
*						  XZEXC_TRAME_DONNEES_METEO pour METEO (DEM/1232)
* Mismer.D	version 1.93	10 Fev 1997	: Ajout constantes TDP (DEM/1395)
* Mismer.D	version 1.94	10 Fev 1997	: Ajout constantes TDP suite (DEM/1396)
* Mismer.D	version 1.95	15 Jan 1998	: Ajout constantes PAL (DEM/)
* cluchague     version 1.96	16 Avr 2002	: pour les panneaux d'alerte le nb max de scenarios passe de 20 a 30
* cluchague     version 1.98	16 Avr 2002	: pour les panneaux d'alerte le nb max de scenarios passe de 30 a 40
* Moulin     	version 1.99	23 Avr 2002	: Ajout des donnees de la nouvelle GTC Nice 
* cluchague  	version 1.101	15 Nov 2004	: Pour BRA NG le parametre XZEXC_nb_module_max passe a 35 et
						  XZEXC_NB_SCENAR_PREDEF passe a 100 (1.101)
* JMG		version 1.102	13/09/05	: ajout PMVA BAF BAD 
* JMG		version 1.103	15/11/05	: correction trame BAD 
* JMG   13/11/07        : ajout site de gestion DEM715
* AAZ                       03/10/07    : DEM 662 Ajout XZEXC_ADR_SOCK_TENIX
* + JMG			    31/01/08	: DEM749 : ajout MaitreCI dans la structure XZEXT_CONFIG_PAU
* ADB   15/02/2010              : Passage a 10 pour la constante XZEXC_NB_CNX_PDTE : Nb de cnx simultanes sur la meme socket
* JMG	29/05/12	: linux
* VR	29/05/2012	: ajout equipement PRv (DEM/1016)
* JMG	01/07/16	version 1.109 : ajout XZEXC_TRAME_ERAU_TRONCON
* JPL	28/10/16	: Ajout type XZEXT_IP_Config pour descriptions unites IP (DEM 1189)  1.110
* LCL	22/02/18	: Ajout controleurs de feux CTRL_FEUX 1.111 DEM1284
* LCL	11/05/18	: Ajout capteurs bouchons GABEB 1.112 DEM1283
* PNI	10/12/18	: correctif du type XZEXT_IP_Config (DEM 1189)  1.113
* JMG	07/05/19	: GAPLA DEM1334 1.114
* JMG	01/01/2020	: IMU 1.115
* NB    28/09/20        : Ajout deuxieme adresse IP DEM-SAE175 1.116
* ABE	15/12/20	: Ajout XZEXC_TEXTE_IMU_NON_REP, XZEXC_TEXTE_TACHE_IMU_ARRETEE, XZEXT_TRAM_PS_IMU et XZEXT_Cmde_OPC DEM-SAE155 1.117
* ABE	02/04/21	: Ajout PAU_SONO DEM-SAE244 1.118
* LCL   25/10/21    : Modification constante XZEXT_TRAME_PS_NAV 4096 correction plantage tepmv POLTDP100 JIRA 426
* LCL   06/11/23    : Definition des chemins en absolu pour fichiers DEM-483
* GGY	14/11/23	: Ajout PIC (DEM483)
------------------------------------------------------ */

#ifndef xzex
#define xzex


/* fichiers inclus */

#include <xdc.h>
#include "xzec.h"
#include <rtworks/common.h>

/* definitions de constantes exportees */

#define	XZEXC_ERR_EQT			2	/* Numero d'equipement invalide */
#define	XZEXC_ERR_CLIGN			3	/* Type de clignotement invalide */
#define	XZEXC_ERR_PICTO			4	/* Symbole Picto invalide */
#define	XZEXC_ERR_SIG			5	/* Numero d'affichage invalide */
#define	XZEXC_ERR_SEQ			6	/* Numero de sequence incorrecte */

#define XZEXC_EQT_PRESENT		1
#define XZEXC_EQT_ABSENT		0
#define	XZEXC_EXISTE			12
#define	XZEXC_NOUVEAU			21
#define	XZEXC_LIMITE_ETAT_EQT		0x1000

#define XZEXC_EOF			            -1	/* Fin de fichier atteinte */
#define XZEXC_LINE_FEED			        "\n"	/* Line Feed */
#define XZEXC_PATH_TACHE_EQUEXT 	    XDC_PATH_MIGRAZURHOME "/exec"
#define XZEXC_PATH_FIC			        XDC_PATH_MIGRAZURHOME "/fichiers"
#define	XZEXC_PATH_CONFIG		        XZEXC_PATH_FIC "/deq"
#define XZEXC_PATH_CONFIG1		        XZEXC_PATH_FIC "/deq"
#define XZEXC_PATH_FIC_PARAM_STATUS 	XZEXC_PATH_FIC "/deq"
#define XZEXC_PATH_WRITE_FILE_MSG 	    XZEXC_PATH_FIC "/wrf/"
#define XZEXC_PATH_READ_FILE_MSG 	    XZEXC_PATH_FIC "/rdf/"
#define XZEXC_PATH_FIC_TEXTE_ALERTE 	XZEXC_PATH_FIC "/deq"
#define XZEXC_PATH_FIC_ALARME	 	    XZEXC_PATH_FIC "/ala"
#define XZEXC_PATH_DIFFUSION 		    XZEXC_PATH_FIC "/deq"
#define XZEXC_PATH_FIC_DYN 		        XZEXC_PATH_FIC "/dyn"
#define XZEXC_PATH_DIFF_DEST 		    XZEXC_PATH_FIC "/deq"
#define XZEXC_PATH_DIFF_DYN 		    XZEXC_PATH_FIC "/dyn"
#define XZEXC_PATH_REFERENCE 		    XDC_PATH_MIGRAZURHOME "/config/fichiers/deq"
#define XZEXC_PATH_SOCKET		        XZEXC_PATH_FIC "/dyn"
#define XZEXC_PATH_FIC_LCR		        XZEXC_PATH_FIC "/deq"
#define XZEXC_PATH_FIC_DFT		        XZEXC_PATH_FIC "/dft"
#define XZEXC_PATH_FIC_SNT		        XZEXC_PATH_FIC "/snt"
#define XZEXC_PATH_FIC_EQA		        XZEXC_PATH_FIC "/eqa"
#define XZEXC_REP_ALA			        "ala"
#define XZEXC_REP_RDF			        "rdf"
#define XZEXC_REP_WRF			        "wrf"
#define XZEXC_REP_LCR			        "lcr"

#define XZEXC_HEURE_QVTO		"der_heure_MB_QVTO"
#define XZEXC_HEURE_PL			"der_heure_MB_PL"

#define XZEXC_TACHE_TETIM		"tetim"

#define XZEXC_ATTENTE_ARRET_TACHE	3

#define XZEXC_DEMANDE_ARRET		"Arreter la tache "
#define XZEXC_DEMANDE_INIT		"Demande d'init "
#define XZEXC_DEMANDE_REVEIL		"Demande de reveil "
#define XZEXC_DEMANDE_LCR		"Demande de lcr "
#define XZEXC_DEMANDE_TEL		"Demande telephone"
#define XZEXC_DEMANDE_OPE		"Demande OPERATOR"
#define XZEXC_ANNULATION_REVEIL		"Annulation reveil "
#define XZEXC_MSG_REVEIL		"Message de reveil "
#define XZEXC_SYNCHRO_DATE		"Demande synchro date "
#define	XZEXC_HEURE_SYNCHRO		"04:05:00" 
#define	XZEXC_HEURE_MINUIT		"00:00:00"
#define	XZEXC_MODE_FONCT_NORMAL		"Mode fonct normal"
#define	XZEXC_MODE_FONCT_ISOLE		"Mode fonct isole"
#define	XZEXC_DEMANDE_RECONNEXION	"Demande reconnexion"
#define XZEXC_CONFIG_IP			"Config IP "
#define XZEXC_DEMANDE_DIASER		"Demande Diaser "
#define XZEXC_DEMANDE_IP_LCR		"Demande IP/LCR "
#define XZEXC_DEMANDE_OPC		"Demande OPC "

#define XZEXC_TEXTE_PMV_NON_REP		"     ? ? ?     "
#define XZEXC_TEXTE_TACHE_PMV_ARRETEE	"     ! ! !     "
#define XZEXC_TEXTE_PMVA_NON_REP	"     ? ? ?     "
#define XZEXC_TEXTE_TACHE_PMVA_ARRETEE	"     ! ! !     " 
#define XZEXC_TEXTE_PRV_NON_REP	"     ? ? ?     "
#define XZEXC_TEXTE_TACHE_PRV_ARRETEE	"     ! ! !     " 
#define XZEXC_TEXTE_CFE_NON_REP	"     ? ? ?     "
#define XZEXC_TEXTE_TACHE_CFE_ARRETEE	"     ! ! !     "
#define XZEXC_TEXTE_CAP_NON_REP	"     ? ? ?     "
#define XZEXC_TEXTE_TACHE_CAP_ARRETEE	"     ! ! !     "
#define XZEXC_TEXTE_IMU_NON_REP	"     ? ? ?     "
#define XZEXC_TEXTE_TACHE_IMU_ARRETEE	"     ! ! !     "

#define XZEXC_ADR_SOCK_TESRV		"tesrv"
#define XZEXC_ADR_SOCK_TEMOD		"temod"
#define XZEXC_ADR_SOCK_TETIM		"tetim"
#define XZEXC_ADR_SOCK_TETIM_SRV	"timsrv"
#define XZEXC_ADR_SOCK_TEINI		"teini"
#define XZEXC_ADR_SOCK_TELM		"telm"
#define XZEXC_ADR_SOCK_TEIM		"teim"												
#define XZEXC_ADR_SOCK_TELM_SRV		"telmsrv"
#define XZEXC_ADR_SOCK_TELE		"tele"
#define XZEXC_ADR_SOCK_TEIE		"teie"
#define XZEXC_ADR_SOCK_TELE_SRV		"telesrv"
#define XZEXC_ADR_SOCK_TETDC_SRV	"tetdcsrv"
#define XZEXC_ADR_SOCK_TEOPE		"teope"
#define XZEXC_ADR_SOCK_TEOPC		"teopc"
#define XZEXC_ADR_SOCK_TEIMU		"teimu"
#define XZEXC_ADR_SOCK_TEOM		"teom"
#define XZEXC_ADR_SOCK_TEOM_SRV		"teomsrv"
#define XZEXC_ADR_SOCK_OPERTEOM		"operteom"
#define XZEXC_REP_TEOMi			"rep. teomi"
#define XZEXC_ADR_SOCK_TETEL		"tetel"
#define XZEXC_ADR_SOCK_TETM		"tetm"
#define XZEXC_ADR_SOCK_MAATETM		"maatetm"
#define XZEXC_ADR_SOCK_TETM_SRV		"tetmsrv"
#define XZEXC_REP_TETMi			"rep. tetmi"
#define XZEXC_ADR_SOCK_TEXA0		"texa0"
#define XZEXC_ADR_SOCK_TENAV		"tenav"
#define XZEXC_ADR_SOCK_TEPAL		"tepal"
#define XZEXC_ADR_SOCK_TEPMV		"tepmv"
#define XZEXC_ADR_SOCK_TEDAI		"tedai"
#define XZEXC_ADR_SOCK_TENIC   		"tenic"
#define XZEXC_ADR_SOCK_TENIW   		"tenic"
#define XZEXC_ADR_SOCK_TENIX   		"tenix"
#define XZEXC_ADR_SOCK_TERAU		"terau"
#define XZEXC_ADR_SOCK_TEQTX		"teqtx"
#define XZEXC_ADR_SOCK_TETDP		"tetdp"
#define XZEXC_ADR_SOCK_TEGTC		"tegtc"
#define XZEXC_ADR_SOCK_TERDT		"terdt"
#define XZEXC_ADR_SOCK_TETDC		"tetdc"
#define XZEXC_ADR_SOCK_TEVID		"tevid"
#define XZEXC_ADR_SOCK_SRVTIM		"srvtim"
#define XZEXC_ADR_SOCK_NAVTIM		"navtim"
#define XZEXC_ADR_SOCK_PALTIM		"paltim"
#define XZEXC_ADR_SOCK_PMVTIM		"pmvtim"
#define XZEXC_ADR_SOCK_DAITIM		"daitim"
#define XZEXC_ADR_SOCK_NICTIM		"nictim"
#define XZEXC_ADR_SOCK_RAUTIM		"rautim"
#define XZEXC_ADR_SOCK_QTXTIM		"qtxtim"
#define XZEXC_ADR_SOCK_TDPTIM		"tdptim"
#define XZEXC_ADR_SOCK_GTCTIM		"gtctim"
#define XZEXC_ADR_SOCK_RDTTIM		"rdttim"
#define XZEXC_ADR_SOCK_RDTTDC		"rdttetdc"
#define XZEXC_ADR_SOCK_VIDTIM		"vidtim"
#define XZEXC_ADR_SOCK_MODTIM		"modtim"
#define XZEXC_ADR_SOCK_IMUTIM		"imutim"
#define XZEXC_ADR_SOCK_SAGATIM		"sagatim"
#define XZEXC_ADR_SOCK_PMVTELM		"pmvtelm"
#define XZEXC_ADR_SOCK_DAITELM		"daitelm"
#define XZEXC_ADR_SOCK_DAITELE		"daitele"
#define XZEXC_ADR_SOCK_VIDTELM		"vidtelm"
#define XZEXC_ADR_SOCK_NAVTELM		"navtelm"
#define XZEXC_ADR_SOCK_PALTELM		"paltelm"
#define XZEXC_ADR_SOCK_RAUTELM		"rautelm"
#define XZEXC_ADR_SOCK_QTXTELM		"qtxtelm"
#define XZEXC_ADR_SOCK_TDPTELM		"tdptelm"
#define XZEXC_ADR_SOCK_RAUTELE		"rautele"
#define XZEXC_ADR_SOCK_QTXTELE		"qtxtele"
#define XZEXC_ADR_SOCK_GTCTELM		"gtctelm"
#define XZEXC_ADR_SOCK_RDTTELM		"rdttelm"
#define XZEXC_ADR_SOCK_NICXA0		"nicxa0"
#define XZEXC_ADR_SOCK_TEXA0_SRV	"xa0srv"
#define XZEXC_ADR_SOCK_SX0SRV		"sx0srv"
#define XZEXC_ADR_SOCK_TESX0		"tesx0"
#define XZEXC_ADR_SOCK_SATSX0		"satsx0"
#define XZEXC_ADR_SOCK_SX0TIM		"sx0tim"
#define XZEXC_ADR_SOCK_NICWW0		"nicww0"
#define XZEXC_ADR_SOCK_TWWI0_SRV	"ww0srv"
#define XZEXC_ADR_SOCK_NICWW1		"nicww1"
#define XZEXC_ADR_SOCK_TWWI1_SRV	"ww1srv"
#define XZEXC_ADR_SOCK_TWWI0		"twwi0"
#define XZEXC_ADR_SOCK_TWWI1		"twwi1"
#define XZEXC_ADR_SOCK_TEPMVA		"tepma"
#define XZEXC_ADR_SOCK_TEPRV		"teprv"
#define XZEXC_ADR_SOCK_TEPIC		"tepic"
#define XZEXC_ADR_SOCK_TEBAF		"tebaf"
#define XZEXC_ADR_SOCK_TEBAD		"tebad"
#define XZEXC_ADR_SOCK_TECFE		"tecfe"
#define XZEXC_ADR_SOCK_TECAP		"tecap"
#define XZEXC_ADR_SOCK_PMVATIM		"pmatim"
#define XZEXC_ADR_SOCK_PRVTIM		"prvtim"
#define XZEXC_ADR_SOCK_PICTIM		"pictim"
#define XZEXC_ADR_SOCK_BAFTIM		"baftim"
#define XZEXC_ADR_SOCK_BADTIM		"badtim"
#define XZEXC_ADR_SOCK_CFETIM		"cfetim"
#define XZEXC_ADR_SOCK_PMVATELM		"pmatelm"
#define XZEXC_ADR_SOCK_PRVTELM		"prvtelm"
#define XZEXC_ADR_SOCK_PICTELM		"pictelm"
#define XZEXC_ADR_SOCK_BAFTELM		"baftelm"
#define XZEXC_ADR_SOCK_BADTELM		"badtelm"
#define XZEXC_ADR_SOCK_CFETEIM		"cfeteim"
#define XZEXC_ADR_SOCK_CAPTIM		"captim"


#define XZEXC_NO_EVT			-1
#define XZEXC_ARG_INV			2
#define XZEXC_CONNEC_UNDEF		-1
#define XZEXC_DECONNEC			0
#define XZEXC_CONNEC			1
#define XZEXC_FIN_MSG_SOCK     		"~|~"
#define XZEXC_FIN_MSG_SOCK_WIZCON	"&"
#define XZEXC_TAILLE_MSG_SOCK     	1024
#define XZEXC_NB_CNX_PDTE		10
#define XZEXC_PORT_SERVEUR_LT		5555
#define XZEXC_PORT_SERVEUR_SD		5556
#define	XZEXC_TIME_OUT_SELECT 		10
#define	XZEXC_SOCKET_NULL		-3

#define XZEXC_TRAME_AFF			1
#define XZEXC_TRAME_ETAT_AFF_PMV	2
#define XZEXC_TRAME_STAT_TR		3
#define XZEXC_TRAME_STAT_COMP		4
#define XZEXC_TRAME_FEN_LCR		5
#define XZEXC_TRAME_CONTRASTE		6
#define XZEXC_TRAME_CLIGNO		7
#define XZEXC_TRAME_ETAT_AFF_PICTO	8
#define XZEXC_TRAME_DATE		9
#define XZEXC_TRAME_CONF_DAI		10
#define XZEXC_TRAME_INFO_TRAFIC_DAI	11
#define XZEXC_TRAME_ALERTE_DAI		12
#define XZEXC_TRAME_AFFECT_CAM_MON_JOY	13
#define XZEXC_TRAME_POSITIONNEMENT	14
#define XZEXC_TRAME_MEMO_POSI		15
#define XZEXC_TRAME_ETEINT_MONIT	16
#define XZEXC_TRAME_ENREG_MAGNETO	17
#define XZEXC_TRAME_ARRET_MAGNETO	18
#define XZEXC_TRAME_STAT_SYS 		19
#define XZEXC_TRAME_TELEI_KO 		20
#define XZEXC_TRAME_TELEI_OK 		21
#define XZEXC_TRAME_CMD_NAV		22
#define XZEXC_TRAME_CMD_SCEN_NAV	23
#define XZEXC_TRAME_ARRET_SCEN_NAV	24
#define XZEXC_TRAME_ETAT_POS_NAV	25
#define XZEXC_TRAME_APPEL_PAU		26
#define XZEXC_TRAME_LIB_LIGNE		27
#define XZEXC_TRAME_MISE_EN_GARDE	28
#define XZEXC_TRAME_REPRISE_GARDE	29
#define XZEXC_TRAME_TELEI_ACK 		30
#define XZEXC_TRAME_TELEI_NAK 		31
#define XZEXC_TRAME_TELEI_NREP 		32
#define XZEXC_TRAME_TST_UNITAIRE	33
#define XZEXC_TRAME_TST_SEMI_AUTO	34
#define XZEXC_TRAME_TRANS_GN		35
#define XZEXC_TRAME_ECOUTE_GN		36
#define XZEXC_TRAME_ALERTE_RAU		37
#define XZEXC_TRAME_RAU_MANUEL		38
#define XZEXC_TRAME_RAU_GEND		39
#define XZEXC_TRAME_RAU_TEST		40
#define XZEXC_TRAME_ENTREES_TOR		41
#define XZEXC_TRAME_MAT_NAV 		42
#define XZEXC_TRAME_MES_IND	 	43
#define XZEXC_TRAME_MES_6MN_PL	 	44
#define XZEXC_TRAME_MES_6MN_Q_V_TO	45
#define XZEXC_TRAME_MES_1MN_Q_V_TO 	46
#define XZEXC_TRAME_MES_6MN_PL_HIS	47
#define XZEXC_TRAME_MES_6MN_Q_V_TO_HIS	48
#define XZEXC_TRAME_CLASSE_DIV		49
#define XZEXC_TRAME_TEST_COM		50
#define XZEXC_TRAME_PESAGE_SILHOUETTE	51
#define XZEXC_TRAME_PESAGE_ESSIEU	52
#define XZEXC_TRAME_PESAGE_POIDS	53
#define XZEXC_TRAME_DONNEES_METEO	54
#define XZEXC_TRAME_CMD_GTC		55
#define XZEXC_TRAME_DONNEES_TDP_HORAIRE	56
#define XZEXC_TRAME_DONNEES_TDP_6MN	57
#define XZEXC_TRAME_STAT_ANT		58
#define XZEXC_TRAME_STAT_TRANS		59
#define XZEXC_TRAME_STAT_ITP		60
#define XZEXC_TRAME_DATE_ID_DATE	61
#define XZEXC_TRAME_DATE_ID_DT		62
#define XZEXC_TRAME_DATE_ID_SUITE	63

#define XZEXC_TRAME_CMD_PAL		70
#define XZEXC_TRAME_ETAT_POS_PAL	71

#define XZEXC_TRAME_AFF_PMVA		72
#define XZEXC_TRAME_AFF_PRV		105
#define XZEXC_TRAME_ETAT_AFF_PMVA		73
#define XZEXC_TRAME_ETAT_AFF_PRV		106
#define XZEXC_TRAME_CMD_BAF		74
#define XZEXC_TRAME_CMD_PRV		107
#define XZEXC_TRAME_ETAT_POS_BAF		75
#define XZEXC_TRAME_ETAT_POS_PRV		108
#define XZEXC_TRAME_CMD_CFE			109
#define XZEXC_TRAME_ETAT_POS_CFE		110
#define XZEXC_TRAME_CMD_CAP			111
#define XZEXC_TRAME_ETAT_POS_CAP		112
#define XZEXC_TRAME_CMD_BAD		76
#define XZEXC_TRAME_CMD_SCEN_BAD		77
#define XZEXC_TRAME_CMD_ARRET_SCEN_BAD	78
#define XZEXC_TRAME_ETAT_POS_BAD		79
#define XZEXC_TRAME_RAU_TRONCON		80

#define XZEXC_TRAME_ALERTE_GAPLA		113
#define XZEXC_TRAME_DEMANDE_ETAT_IMU		114
#define XZEXC_TRAME_CMD_IMU_STRING		115
#define XZEXC_TRAME_CMD_IMU_INT		116
#define XZEXC_TRAME_ETAT_IMU_STRING		117
#define XZEXC_TRAME_ETAT_IMU_INT		118
#define XZEXC_TRAME_CR_IMU		119
#define XZEXC_TRAME_CMD_IMU		120
#define XZEXC_TRAME_ABONNEMENT_TAG	121
#define XZEXC_TRAME_CONF_DAI_IP		122
#define XZEXC_TRAME_DEMANDE_ETAT_RAU_SONO            123
#define XZEXC_TRAME_CMD_RAU_SONO_STRING              124
#define XZEXC_TRAME_CMD_RAU_SONO_INT                 125
#define XZEXC_TRAME_ETAT_RAU_SONO_STRING             126
#define XZEXC_TRAME_ETAT_RAU_SONO_INT                127
#define XZEXC_TRAME_CR_RAU_SONO                      128
#define XZEXC_TRAME_CMD_RAU_SONO                     129
#define XZEXC_TRAME_ETAT_AFF_PIC                     130
	
#define XZEXC_NOCMD			     0
#define XZEXC_CMD_HORS_BASE		-1
#define XZEXC_CMD_LT_PAL		-2
#define XZEXC_CMD_AU_NAV		-2
#define XZEXC_CMD_INIT			-3

#define XZEXC_ANNUL_CMD_LCR		-1
#define XZEXC_DDE_ANNUL_CMD_LCR		"ANNULATION DEMANDEE"

#define XZEXC_SURB_EQTALL		0

#define XZEXC_PRIORITEFORTE		0
#define XZEXC_PRIORITEFAIBLE		1

/* defs. contexte */
#define XZEXC_MODE_NORMAL		0
#define XZEXC_MODE_CONNECTION_LOCALE	1
#define XZEXC_MODE_ISOLE		2
#define XZEXC_MODE_LENT			3
#define XZEXC_RECONFIG_LCR		4

/* defs. type camera */
#define XZEXC_MOBILE			1
#define XZEXC_FIXE			2
#define XZEXC_CYCLE			3
#define XZEXC_POSI			4

/* defs. canaux LTV */
#define XZEX_NB_CANAUX_MAX_ATTEINT	2
#define XZEXC_SATURE			3
#define XZEXC_NON_SATURE		4

/* defs. etat du magnetoscope */
#define XZEXC_ENREG_EN_COURS		1
#define XZEXC_AUCUN_ENREG		2

/* defs. etat Scenario */
#define XZEXC_SCENAR_EN_COURS		1
#define XZEXC_AUCUN_SCENAR		2

/* defs. modules NAV */
#define XZEXC_nb_module_max		35
/*#define XZEXC_nb_module_max		22*/
#define XZEXC_nb_module_max_BAF	6
#define XZEXC_nb_module_max_BAD	2



/* defs. etat des commandes */
#define XZEXC_CMD_EN_COURS		1
#define XZEXC_PAS_DE_CMD		2
#define XZEXC_CROK			3
#define XZEXC_CRNOK			4
#define XZEXC_FIN_TEST			5

/* defs. types reponses tache TELMi */
#define XZEXC_REP_ACK		55
#define	XZEXC_REP_NAK		56
#define XZEXC_TRAME_NON_TRANSM	57
#define XZEXC_REP_BLC_INT	58
#define XZEXC_REP_BLC_FIN	59

/* defs. types commandes pour taches TETMi */
#define XZEXC_MAA_AVEC_REPONSE		1
#define XZEXC_MAA_SANS_REPONSE		2


/* nbre. min. sockets tache TELxi 
 * ( comms. TEzzz, comms. serv. terms., comms. RTServer ) */
#define XZEXC_NBR_MIN_SOCK_TELxi	3

/* defs. type socket active */
#define XZEXC_SOCK_ACT_UNDEF		101
#define XZEXC_SOCK_ACT_LSER		102
#define XZEXC_SOCK_ACT_DONN		103
#define XZEXC_SOCK_ACT_RTSRV		104
#define XZEXC_SOCK_ACT_IP_LCR		105
#define XZEXC_SOCK_ACT_IP_DIASER	106
#define XZEXC_SOCK_ACT_IP_LCR_E		107
#define XZEXC_SOCK_ACT_IP_OPC		108

#define XZEXC_PERTE_LIAISON_DAI		1099

/* defs. vals. hexa des cars. speciaux du protocole TEDI/LCR
 * utilises pour l'encapsulation des trames
 */
#define XZEXC_NULC		0x00
#define XZEXC_CRC_MASK		0x7F

#define XZEXC_CAR1_REMPLISSAGE	0x00
#define XZEXC_CAR2_REMPLISSAGE	0x7F

#define XZEXC_CAR_ENQ		0x05
#define XZEXC_CAR_STX		0x02
#define XZEXC_CAR_ETX		0x03
#define XZEXC_CAR_ETB		0x17
#define XZEXC_CAR_ACK		0x06
#define XZEXC_CAR_NAK		0x15
#define XZEXC_CAR_SEP		0x20
#define XZEXC_CAR_PREF		XZEXC_NULC		
#define XZEXC_CAR_SUFF		XZEXC_NULC
#define XZEXC_CAR_JOKER		'0'
#define XZEXC_CAR_NO_BLC_REQ	'0'	

/* defs. type du message TEDI */

#define XZEXC_MSG_REQ		30
#define	XZEXC_MSG_INFO		31
#define XZEXC_BLC_INFO		32
#define	XZEXC_MSG_ACQUIT	33	

/* defs. types erreurs TEDI */
#define XZEXC_MSG_OK		10	/* message TEDI OK */
#define XZEXC_BLC_OK		12	/* bloc interm. OK */
#define XZEXC_ACK_OK		14	/* acquitt. pos. OK */
#define XZEXC_NAK_OK		16	/* acquitt. neg. OK */
#define XZEXC_TEDI_NOK		11	/* msg. sans struct. TEDI */
#define XZEXC_ERR_CRC_MSG	13	/* err. CRC message */
#define XZEXC_ERR_CRC_BLC	15	/* err. CRC bloc interm */

#define	XZEXC_NOBL_REQ_TEDI	0	/* no. bloc requette TEDI [ENQ]rgsb ... [ETX][BCC] */
#define XZEXC_LONG_DDM_TEDI	5	/* longueur entete msg. TEDI = xrgsb */
#define XZEXC_LONG_FDM_TEDI	2	/* long. fin msg. TEDI */
#define XZEXC_LONG_ACQUIT_TEDI	2	/* long. d'un msg. acquit. TEDI */
#define XZEXC_RGS_SAE		"EsM"

/* defs. types reponses tache TELMi */
#define XZEXC_REP_ACK		55
#define	XZEXC_REP_NAK		56
#define XZEXC_TRAME_NON_TRANSM	57
#define XZEXC_REP_BLC_INT	58
#define XZEXC_REP_BLC_FIN	59	/* utilise par TELEi pour msg. correcte */

#define XZEXC_REP_ALARME_TELEi	60


#define XZEXC_REP_TELEi		"rep. telei"
#define XZEXC_REP_TELMi		"rep. telmi"
#define XZEXC_SEP_TRAME		"$$$"

/* format lecture/ecriture fichier de config EQT X */
#define XZEXC_FORMAT_CONF_EQTX	" %d\t%d\t%s\t%d\t%s\t%s\t%d\t%d\t%d\n"

/* format lecture/ecriture fichier de config TDP (Temps de Parcours) */
#define XZEXC_FORMAT_CONF_TDP	" %d\t%s\t%s\t%d\t\n"


/* valeurs par defaut timer attente sur socket ST */
#define XZEXC_VAL_DEFAUT_TIMER		2.5
#define XZEXC_TIMER_ECOULE		0.0

/* duree attente TELxi entre deux tentatives d'init */
#define XZEXC_DUREE_ATTENTE	10

/* Constantes etat communication RAU */
#define XZEXC_EN_ECOUTE		-1
#define XZEXC_LIBRE		0
#define XZEXC_OCCUPE		1

/* Def du nb de scenario predefini max */
#define XZEXC_NB_SCENAR_PREDEF	100

/* Constantes des fichiers SATIR */

#define XZEXC_NOM_FICHIER_HORODATAGE	"HEURE"
#define XZEXC_NOM_FICHIER_LISTE		"liste"

#define	XZEXC_FIC_ACCIDENT		"Acc"
#define	XZEXC_FIC_TRAFIC		"Tra"
#define	XZEXC_FIC_TRAVAUX		"TVX"
#define	XZEXC_FIC_METEO			"MTO"
#define	XZEXC_FIC_INFO_USAGER		"PMV"
#define	XZEXC_FIC_FERMETURE		"Fer"
#define	XZEXC_FIC_ETAT_LIAISON		"ETAT"

#define XZEXC_NOM_FICHIER_ETAT_CANNET		"ETAT_CA"
#define XZEXC_NOM_FICHIER_ETAT_MAXIMIN		"ETAT_SM"
#define XZEXC_NOM_FICHIER_ETAT_AUBAGNE		"ETAT_AU"
#define XZEXC_NOM_FICHIER_ETAT_MEYRARGUES	"ETAT_ME"
#define XZEXC_NOM_FICHIER_ETAT_PEYRUIS		"ETAT_PE"
#define XZEXC_NOM_FICHIER_SURVEILLANCE		"ETAT_SAE"

#define XZEXC_NOM_REDUIT_ETAT_CANNET		"CA"
#define XZEXC_NOM_REDUIT_ETAT_MAXIMIN		"SM"
#define XZEXC_NOM_REDUIT_ETAT_AUBAGNE		"AU"
#define XZEXC_NOM_REDUIT_ETAT_MEYRARGUES	"ME"
#define XZEXC_NOM_REDUIT_ETAT_PEYRUIS		"PE"

/* definitions de types exportes */

/* Type enregistrement configuration lignes contenu dans le fichier XDF_Config_Lignes */
typedef struct	{
	char	NumeroST[26];	/* Identification du serveur de terminaux (adresse) */
	char	NumLigne[16];	/* Identification de la ligne */
	char	Protocole;	/* Type de protocole ( L, A, O, S ) */
	char 	Sens;		/* Mode de la liaison  ( M : maitre, E : esclave ) */
	char	Vitesse[6];	/* Vitesse sur la liaison en bauds */
	int	NbEssais;	/* Nbre maxi de tentatives de commandes en cas de non reponse */
	int	NbCarPref;	/* Nbre de caractere pour le prefixe de la trame */
	int	NbCarSuff;	/* Nbre de caractere pour le suffixe de la trame */
		}	XZEXT_ENG_CONF_LIGNE;	
		
/* Type enregistrement presence equipement contenu dans le fichier XDF_Presence_Equip */
typedef struct	{
	char	Equipement[4];	/* Identification de l'equipement  */
	char	Present;	/* Identicateur de presence de l'equipement sur la machine */
	int	Type;		/* Type de l'equipement sur la machine */
		}	XZEXT_ENG_PRESENCE_EQT;	

typedef	char		XZEXT_ADR_SOCK[32];

typedef	char		XZEXT_MSG_SOCKET[XZEXC_TAILLE_MSG_SOCK];

typedef	char		XZEXT_TRAME_PS_NAV[4096];       /* JIRA 426 - Plantage tepmv sor POLTDP100 */
typedef	char		XZEXT_TRAME_PS_BAF[2048];
typedef	char		XZEXT_TRAME_PS_BAD[2048];
typedef	char		XZEXT_TRAME_PS_PRV[2048];
typedef	char 		XZEXT_TRAME_PS_CFE[2048];
typedef	char 		XZEXT_TRAME_PS_CAP[2048];
typedef	char 		XZEXT_TRAME_PS_IMU[2048];

typedef	char		XZEXT_TRAME_PS_PAL[512];

typedef struct	{
	int		desc_sock;		/* Descripteur de socket */
	XZEXT_ADR_SOCK	adresse;		/* Adresse de la socket */
	short		Type;			/* Type de socket (UNIX INET) */
	char		Protocole;
	int		Port;
	void *		Suivant;
		}	XZEXT_MASK;

typedef char		XZEXT_CLE_PROG[50];

typedef	char		XZEXT_LigneFichier[256];/* Chaine de caractere pour lecture fichier ASCII */

typedef	char		XZEXT_Trame[XZEXC_TAILLE_MSG_SOCK]; /* Trame LCR */

typedef	char		XZEXT_TEXTE_STATUS[24]; /* Trame LCR */

typedef struct XZEXT_Msg_Socket		/* type message socket */
{
    XZEXT_MSG_SOCKET	msg;	/* stokage message proprement dit */
    int			len;	/* longueur message */
}XZEXT_Msg_Socket;

typedef struct XZEXT_ST_Sockets	/* struct. stokage descs. sockets specifiques aux taches protocole
 				 * utilisant le Serveur de Terminaux  */
{
    T_INT4 rtsrv;  /* desc. socket RTServer */
    int    ect;	  /* desc. socket ecoute en mode serveur pour comms. avec tache TEzzz */
    int    lser;	  /* desc. socket ligne serie en mode client - comms. avec serveur terms. */
    int    curr;	  /* desc. socket active */
    char   curr_addr[XDC_PATH_ABS_SIZE]; /* adresse client sur socket active */
}XZEXT_ST_Sockets;

/* def. type trame TEDI */
typedef char	XZEXT_Trame_TEDI[275];	/* 256 cars. pour le mesg. TEDI + max. 18 cars. suff. & pref. */

/* def. type message TEDI */
typedef struct 	XZEXT_Msg_TEDI 
{
    short		len;		/* longueur trame TEDI. Evite la perte d'un CRC = 0x00 */
    XZEXT_Trame_TEDI	trame_TEDI;	/* message TEDI/LCR */
} XZEXT_Msg_TEDI;

/* def. type composants de la trame TEDI */
typedef struct  XZEXT_Comp_Trame_TEDI 
{
    char 	adr_RGSB[5]; 	/* adresse rgs et ( ou seulement pour les acquitt. ) no. bloc */
    XZEXT_Trame	trame_LCR;	/* message LCR */
    short	long_pref;	/* longueur prefixe */
    short	long_suff;	/* longueur suffixe */
    int		type_msg;	/* type message : requette, info., acquitt. pos. ou neg. */
} XZEXT_Comp_Trame_TEDI;

typedef struct XZEXT_Cmde_LCR
{
    int		type_cmd;	/* type cmde. LCR envoye par le client TEzzz */
    char	rgs[5];		/* adresse rgs de l'equip. cible */
    int		no_cmd;		/* no. cmde. a renvoyer tout simplement */
    int 	prior;		/* priorite de la cmde */
    XZEXT_Trame cmd_LCR;	/* cmde LCR */
    int		sock_clnt;	/* desc. socket comms. avec le client TEzzz */
    int		type_msg;	/* type message TEDI associe */
    char	ack[3];		/* aquit. des eventuels blocs receptiones suite a
    				 * l'emission de la cmde. cmd_LCR */
    char	no_blc_suivant;	/* no. du bloc suivant a receptioner si echange en blocs */
}XZEXT_Cmde_LCR;

typedef struct XZEXT_Cmde_DIASER
{
	int         type_cmd;       /* type cmde. LCR envoye par le client TEzzz */
	XDY_AdresseIP	adresseIP;
	int	port;
	int         no_cmd;         /* no. cmde. a renvoyer tout simplement */
	int         prior;          /* priorite de la cmde */
	XZEXT_Trame cmd;
	int         sock_clnt;      /* desc. socket comms. avec le client TEzzz */
}XZEXT_Cmde_DIASER;

typedef struct XZEXT_Cmde_OPC
{
	int        	type_cmd;       /* type cmde. LCR envoye par le client TEzzz */
	int	    	type_msg;
	int		port;
    	char		idBICE[50];
	int        	no_cmd;         /* no. cmde. a renvoyer tout simplement */
	int        	prior;          /* priorite de la cmde */
	XZEXT_Trame 	cmd;
	int         	sock_clnt;      /* desc. socket comms. avec le client TEzzz */
}XZEXT_Cmde_OPC;


typedef struct XZEXT_Cmde_LCR_IP
{
  int         type_cmd;       /* type cmde. LCR envoye par le client TEzzz */
  XDY_AdresseIP		adresseIP;
  int 		port;
  char        rgs[5];         /* adresse rgs de l'equip. cible */
  int         no_cmd;         /* no. cmde. a renvoyer tout simplement */
  int         prior;          /* priorite de la cmde */
  XZEXT_Trame cmd_LCR;        /* cmde LCR */
  int         sock_clnt;      /* desc. socket comms. avec le client TEzzz */
  int         type_msg;       /* type message TEDI associe */
  char        ack[3];         /* aquit. des eventuels blocs receptiones suite a
  				* l'emission de la cmde. cmd_LCR */
  char        no_blc_suivant; /* no. du bloc suivant a receptioner si echange en blocs */
XDY_Horodate	horodate;
}XZEXT_Cmde_LCR_IP;

typedef struct XZEXT_Liste_Cmdes_LCR
{
    XZEXT_Cmde_LCR	cmd;
    void		*next;
}XZEXT_Liste_Cmdes_LCR;

typedef struct XZEXT_Liste_Cmdes_LCR_IP
{
    XZEXT_Cmde_LCR_IP	cmd;
    void		*next;
}XZEXT_Liste_Cmdes_LCR_IP;

/* def. type configuration taches protocoles utilisant le serveur de terminaux */
typedef struct XZEXT_ST_Config
{
    XZEXT_ENG_CONF_LIGNE	ligne_serie;	/* config. ligne serie */
    short			no_ligne;	/* no. de la ligne serie geree */
    int				no_port;	/* no port conexion domaine INTERNET */
    XDY_NomMachine		nom_machine;	/* nom de la machine */
    XDY_NomSite			nom_site;	/* nom du site */
}XZEXT_ST_Config;

/* Configuration taches equipement (traitement) ou protocole communiquant avec une unite IP */
typedef struct XZEXT_IP_Config
{
     int                         No_port;        /* no port connexion domaine INTERNET */
         XDY_Nom              Nom_machine;    /* nom de la machine */
    XDY_AdresseIP		adresseIP;
    int				no_ligne;
    XDY_NomSite                 nom_site;
    char			protocole;
    XZEXT_ENG_CONF_LIGNE	ligne;
}XZEXT_IP_Config;

typedef struct 
{
     int                         No_port;        /* no port connexion domaine INTERNET */
         XDY_Nom              Nom_machine;    /* nom de la machine */
    XDY_AdresseIP		adresseIP;
    XDY_NomSite                 nom_site;
    char			protocole[50];
    char			servername[50];
    char			prefix[50];
    char			namespace[50];
}XZEXT_OPC_Config;

/* def. type configuration PAU pour alerte PAU HS */
typedef struct {

        XDY_Nom      	LibelleEqt;
        XDY_Eqt         NumeroEqt;
        XDY_NomMachine  Maitre;
	XDY_NomMachine  MaitreCI;
        XDY_NomEqt      NomEqt;
        XDY_Mot		NumeroAutoroute;
        XDY_PR          PR;
        XDY_Mot		Sens;
        XDY_Octet	Alarme;
        XDY_Octet	Alerte;
	void *		Suivant;

} XZEXT_CONFIG_PAU ;

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande Fenetre LCR
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZEX01_Demande_LCR( int	 	va_NoTypEqt,
                        XDY_Eqt 	va_NoEqt,
                        char *		va_FichierLCR,
                        char *		va_FichierResultat,
                        int		va_Periode,
                        XDY_Horodate	va_HrDebut,
                        XDY_Horodate	va_HrFin,
                        XDY_NomMachine	va_NomMachine,
                        XDY_Fonction va_FonctionTimeout );

/*
* ARGUMENTS EN ENTREE :
*
* va_NoTypEqt : No du type d'eqt
* va_NoEqt : No d'equipement
* va_FichierLCR : Fichier contenant les commandes LCR
* va_FichierResultat : Fichier resultat LCR
* va_Periode : Periode d'emission des messages
* va_HrDebut : Heure debut d'emission
* va_HrFin : Heure fin d'emission
* va_NomMachine : Nom Machine
* va_FonctionTimeout : Fonction appelee sur timeout
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK		succes
*   XDC_NOK		echec 
*
* CONDITION D'UTILISATION
*    
*
* FONCTION 
*   Envoie le message XDM_ECPT_LCR sur le 
*   datagroup XDG_ECPT_LCR_<NomMachine>
*
------------------------------------------------------*/

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande Fenetre LCR
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZEX02_Retour_LCR(  XDY_Mot 	va_CrLCR,
                        XDY_NomMachine	va_NomMachine);

/*
* ARGUMENTS EN ENTREE :
*
* va_CrLCR : Compte rendu de traitement
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK		succes
*   XDC_NOK		echec 
*
* CONDITION D'UTILISATION
*    
*
* FONCTION 
*   Envoie le message XDM_ECPT_LCR sur le 
*   datagroup XDG_ECPT_LCR_<NomMachine>
*
------------------------------------------------------*/

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Fonction retournant le nom de l'equipement correspondant
*  au type d'equipement fournit en entree
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZEX03_nom_eqt ( int	va_TypeEqt , char	*pa_NomEqt );

/*
* ARGUMENTS EN ENTREE :
*  va_TypeEqt		: Type d'equipement
*
*
* ARGUMENTS EN SORTIE :
*  paNomEqt		: Nom de l'equipement
*
*
* CODE RETOUR : 
*  XDC_OK		: Succes
*  XDC_NOK		: Echec
*
* CONDITION D'UTILISATION
*    
*
* FONCTION 
*   
*
------------------------------------------------------*/

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Annulation Timeout demande LCR
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZEX04_Annul_TimeOut ( void );

/*
* ARGUMENTS EN ENTREE :
*
* aucun
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK		succes
*   XDC_NOK		echec 
*
* CONDITION D'UTILISATION
*    
*
* FONCTION 
*   Annule le timeout demande LCR
*
------------------------------------------------------*/

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Acknoledge de l'envoi d'une alarme a la Base de Donnees
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZEX05_Acknoledge_Alarme (  XDY_Horodate,
				XDY_TypeEqt,
                                XDY_Eqt,
                                XDY_TypeAlarme,
                                XDY_Booleen,
                                XDY_NomMachine );

/*
* ARGUMENTS EN ENTREE :
*
* aucun
*
* ARGUMENTS EN SORTIE :
*   
*       va_Horodate_in
*       va_TypeEqt_in
*       va_Eqt_in
*       va_TypeAlarme_in
*       va_Etat_in
*	va_NomMachine_in
*
* CODE RETOUR : 
*   XDC_OK		succes
*   XDC_NOK		echec 
*
* CONDITION D'UTILISATION
*    
*
* FONCTION 
*   Acknoledge de l'envoi d'une alarme a la Base de Donnees
*
------------------------------------------------------*/


#endif
