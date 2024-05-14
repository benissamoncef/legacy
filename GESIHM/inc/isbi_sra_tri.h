/*E Fichier : $Id: isbi_sra_tri.h,v 1.4 2013/08/28 09:34:12 pc2dpdy Exp $      Release : $Revision: 1.4 $        Date : $Date: 2013/08/28 09:34:12 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME GESIHM
------------------------------------------------------
* MODULE MSRA * FICHIER isyn_sra_tri.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Creation et Mise a jour des Variables d'animations 
*   graphiques : Fonction de tri des objets par ordre 
*   d'apparition a l'ecran
*
------------------------------------------------------
* HISTORIQUE :
*
* Niepceron	23 Sep 1994	: Creation
* JMG	04/07/12	PRV 
* JMG	22/08/12 : renum
* JMG	14/01/13 : ajout regul
------------------------------------------------------ */

#ifndef MSRA_tri
#define MSRA_tri

/* fichiers inclus */

#include "xzst.h"
#include "xzsc.h"
#include "xdy.h"


/* definitions de constantes exportees */

#define  CM_ORD_FMC_ACCIDENT 			1
#define  CM_ORD_FMC_ACCIDENT_VL 		2
#define  CM_ORD_FMC_ACCIDENT_PL 		3
#define  CM_ORD_FMC_ACCIDENT_VLPL		4
#define  CM_ORD_FMC_ACCIDENT_CAR 		5
#define  CM_ORD_FMC_ACCIDENT_VLCAR 		6
#define  CM_ORD_FMC_ACCIDENT_PLCAR 		7
#define  CM_ORD_FMC_ACCIDENT_VLPLCAR 		8
#define  CM_ORD_FMC_ACCIDENT_REM		9
#define  CM_ORD_FMC_ACCIDENT_VLREM 		10
#define  CM_ORD_FMC_ACCIDENT_PLREM 		11
#define  CM_ORD_FMC_ACCIDENT_VLPLREM 		12
#define  CM_ORD_FMC_ACCIDENT_CARREM 		13
#define  CM_ORD_FMC_ACCIDENT_VLCARREM 		14
#define  CM_ORD_FMC_ACCIDENT_PLCARREM 		15
#define  CM_ORD_FMC_ACCIDENT_VLPLCARREM 	16
#define  CM_ORD_FMC_ACCIDENT_MOTO 		17
#define  CM_ORD_FMC_ACCIDENT_VLMOTO 		18
#define  CM_ORD_FMC_ACCIDENT_PLMOTO 		19
#define  CM_ORD_FMC_ACCIDENT_VLPLMOTO 		20
#define  CM_ORD_FMC_ACCIDENT_CARMOTO 		21
#define  CM_ORD_FMC_ACCIDENT_VLCARMOTO 		22
#define  CM_ORD_FMC_ACCIDENT_PLCARMOTO 		23
#define  CM_ORD_FMC_ACCIDENT_VLPLCARMOTO 	24
#define  CM_ORD_FMC_ACCIDENT_REMMOTO 		25
#define  CM_ORD_FMC_ACCIDENT_VLREMMOTO 		26
#define  CM_ORD_FMC_ACCIDENT_PLREMMOTO 		27
#define  CM_ORD_FMC_ACCIDENT_VLPLREMMOTO 	28
#define  CM_ORD_FMC_ACCIDENT_CARREMMOTO 	29
#define  CM_ORD_FMC_ACCIDENT_VLCARREMMOTO 	30
#define  CM_ORD_FMC_ACCIDENT_PLCARREMMOTO 	31
#define  CM_ORD_FMC_ACCIDENT_VLPLCARREMMOTO 	32
#define  CM_ORD_FMC_VEHICULE_CONTRE_SENS 	33
#define  CM_ORD_FMC_VEHICULE_ARRETE 		34
#define  CM_ORD_FMC_VEHICULE_PANNE 		35
#define  CM_ORD_FMC_VEHICULE_FEU 		36
#define  CM_ORD_FMC_CIRQUE 			37
#define  CM_ORD_FMC_CONVOI_EXCEPTIONNEL 	38
#define  CM_ORD_FMC_CONVOI_MILITAIRE 		39
#define  CM_ORD_FMC_VEHICULE_LENT 		40
#define  CM_ORD_FMC_MANIFESTATION 		41
#define  CM_ORD_FMC_GREVE_ESCOTA 		42
#define  CM_ORD_FMC_OPERATION_ESCARGOT 		43
#define  CM_ORD_FMC_INCIDENT_SOUS_CONSC 	44
#define  CM_ORD_FMC_EBOULEMENT 			45
#define  CM_ORD_FMC_INCENDIE 			46
#define  CM_ORD_FMC_INONDATION 			47
#define  CM_ORD_FMC_OBSTACLE 			48
#define  CM_ORD_FMC_CHAUSSEE_DETERIOREE 	49
#define  CM_ORD_FMC_FLAQUE_HUILE 		50
#define  CM_ORD_FMC_ANIMAL_ERRANT 		51
#define  CM_ORD_FMC_FRONTIERE_FERMEE 		52
#define  CM_ORD_FMC_ALTERCATION_USAGERS 	53
#define  CM_ORD_FMC_AGRESSION 			54
#define  CM_ORD_FMC_MALAISE_USAGER 		55
#define  CM_ORD_FMC_HOLD_UP 			56
#define  CM_ORD_FMC_PIETON_CHAUSSEE 		57
#define  CM_ORD_FMC_AUTRE 			58
#define  CM_ORD_FMC_METEO_CHUTE_NEIGE		59
#define  CM_ORD_FMC_METEO_NEIGE_CHAUSSEE	60
#define  CM_ORD_FMC_METEO_GRELE_CHAUSSEE	61
#define  CM_ORD_FMC_METEO_PLUIE 		62
#define  CM_ORD_FMC_METEO_VENT 			63
#define  CM_ORD_FMC_METEO_BROUILLARD 		64
#define  CM_ORD_FMC_METEO_BROUILLARD_GIVRANT 	65
#define  CM_ORD_FMC_METEO_GRELE 		66
#define  CM_ORD_FMC_METEO_VERGLAS 		67
#define  CM_ORD_FMC_METEO_AUTRE 		68
#define  CM_ORD_FMC_VEHICULE_MARCHE_ARR		69
#define  CM_ORD_FMC_TRAITEMENT_CHAUSSEES	70
#define  CM_ORD_FMC_ECHANGEUR_DECONSEILLE	71
#define  CM_ORD_FMC_CHAUSSEE_GLISSANTE		72
#define  CM_ORD_INTERVENANT_AUTRE 		73
#define  CM_ORD_TRAVAUX_MOBILES_PREVUS 		74
#define  CM_ORD_TRAVAUX_MOBILES_REELS 		75
#define  CM_ORD_TRAVAUX_FIXES_PREVUS 		76
#define  CM_ORD_TRAVAUX_FIXES_REELS 		77
#define  CM_ORD_TRAVAUX_MOBILES_TETE_PREVUS 	78
#define  CM_ORD_TRAVAUX_MOBILES_TETE_REELS  	79
#define  CM_ORD_TRAVAUX_MOBILES_QUEUE_PREVUS 	80
#define  CM_ORD_TRAVAUX_MOBILES_QUEUE_REELS 	81
#define  CM_ORD_AFFICH_GARE 			82
#define  CM_ORD_INTERVENANT_GENDARMERIE 	83
#define  CM_ORD_INTERVENANT_POMPIER 		84
#define  CM_ORD_INTERVENANT_DEPANNEUR 		85
#define  CM_ORD_INTERVENANT_FOURGON_SIS 	86
#define  CM_ORD_INTERVENANT_PAT_ESCOTA 		87
#define  CM_ORD_PMV_MOBILE 			88
#define  CM_ORD_AFF_VOIE 			89
#define  CM_ORD_FMC_REGULATION_VITESSE		90

#define  CM_ORD_DAITXT				91   /* Type objet DAI */
#define  CM_ORD_DAI 				92   /* Type objet DAI */
#define  CM_ORD_CAMERATXT 			93   /* Type objet CAMERA */
#define  CM_ORD_CAMERA 				94   /* Type objet CAMERA */
#define  CM_ORD_NAV 				95   /* Type objet NAV */
#define  CM_ORD_PAU 				96  /* Type objet PAU */
#define  CM_ORD_RADTXTQ 			97   /* Type objet RADT texte q*/
#define  CM_ORD_RADTXTV 			98   /* Type objet RADT texte v*/
#define  CM_ORD_RADTTXT 			99   /* Type objet RADT */
#define  CM_ORD_RADT 				100   /* Type objet RADT */
#define  CM_ORD_PMVTXT				101   /* Type objet PMV */
#define  CM_ORD_PMV 				102   /* Type objet PMV */
#define  CM_ORD_PMV_PICTO 			103   /* Type objet PMV */
#define  CM_ORD_PANALT                          104   /* Type objet panneau alerte */
#define CM_ORD_PMVATXT                          105
#define CM_ORD_PMVA                             106
#define CM_ORD_PMVAD                            107
#define CM_ORD_BAF                              108
#define CM_ORD_BAD                              109
#define CM_ORD_PRV                              110
#define  CM_ORD_FERM	 			111   /* Type objet fermeture */
#define  CM_ORD_PEAGE 				112   /* Type objet PEAGE */
#define  CM_ORD_PEAGETXT 			113   /* Type objet PEAGE */
#define  CM_ORD_ITPC 				114   /* Type objet ITPC */
#define  CM_ORD_VILLE 				115  /* Type objet VILLE */
#define  CM_ORD_LIT 				116 /* Type objet lit arret */
#define  CM_ORD_PANPOLTXT 			117  /* Type objet texte Panneau police */
#define  CM_ORD_PANPOL 				118  /* Type objet Panneau police */
#define  CM_ORD_PENTETXT			119 /* Type objet texte Pente */
#define  CM_ORD_PENTE 				120 /* Type objet Pente */
#define  CM_ORD_LTV 				121 /* Type objet LTV */
#define  CM_ORD_PORTAIL 			122  /* Type objet portail **/
#define  CM_ORD_CANTON				123 /* Type objet canton **/
#define  CM_ORD_AIRE 				124 /* Type objet aire */
#define  CM_ORD_PETROL 				125 /* Type objet PETROLIER */
#define  CM_ORD_RESEAU 				126  /* Type objet reseau */
#define  CM_ORD_PASSAGE 			127  /* Type objet passage sup */
#define  CM_ORD_EXTINC 				128  /* Type objet extincteur */
#define  CM_ORD_FAV 				129  /* Type objet fav */
#define  CM_ORD_TUNNEL 				130  /* Type objet tunnel */
#define  CM_ORD_CECHANGEUR 			131  /* Type objet texte ECHANGEUR */
#define  CM_ORD_ECHANGEURTXT 			132  /* Type objet texte ECHANGEUR */
#define  CM_ORD_QECHANGEUR 			133  /* Type objet ECHANGEUR */
#define  CM_ORD_ECHANGEUR 			134  /* Type objet ECHANGEUR */
#define  CM_ORD_ZDP 				135  /* Type objet ZDP */
#define  CM_ORD_VIADUC 				136  /* Type objet viaduc */
#define  CM_ORD_BINTERV 			137  /* Type objet bouton intervenant */
#define  CM_ORD_BALARM 				138  /* Type objet bouton intervenant */
#define  CM_ORD_BETAT 				139  /* Type objet bouton intervenant */
#define  CM_ORD_NAVIG 				140  /* Type objet bouton de navigation */
#define  CM_ORD_SIGNALE 			141  /* Type objet bouton de navigation */
#define  CM_ORD_SIGNALT 			142  /* Type objet bouton de navigation */
#define  CM_ORD_SERVEUR 			143  /* Type objet bouton de navigation */
#define  CM_ORD_IGN	 			144  /* Type objet bouton de navigation */

#define  CM_ORD_MASK 				145   /* Type objet Zone MASK */
#define  CM_ORD_NEUTRA                          146
#define  CM_ORD_BASCBOU                         147
#define  CM_ORD_INVERSE                         148
#define  CM_ORD_BOUCHON                         149  /*DEM 444*/
#define  CM_ORD_BAU                             150  /* Type objet BAU */
#define  CM_ORD_ZOOM				151  /* Type objet Zoom */
#define  CM_ORD_ZRADT 				152   /* Type objet Zone RADT */
#define  CM_ORD_PORTION 			153  /* Type objet PORTION */
#define  CM_ORD_JONCTION 			154  /* type jonction (coude) */
#define  CM_ORD_SEGMENT 			155  /* Type objet SEGMENT */
#define  CM_ORD_PORTEUSE 			156  /* Type objet PORTEUSE */
#define  CM_ORD_PORTEUSEPORTION 		157  /* Type objet porteuse pour une portion  */
#define  CM_ORD_BBOUCHON 			158  /* Type objet bouton bouchon (cf courbes) */
#define  CM_ORD_BEVT     			159  /* Type objet bouton evt (cf courbes) */
#define  CM_ORD_GRAPHE     			160 /* Type objet graphe pour affichage des courbes  */
#define  CM_ORD_GRAPHE_INRETS     		161  /* Type objet graphe inrets pour affichage des courbes  */
#define  CM_ORD_GRAPHE_TDP			162  /* Type objet graphe tdp pour affichage des courbes  */
#define  CM_ORD_IZOOM				163  /* Type objet Zoom */
#define  CM_ORD_SYN				164  /* Type objet Zoom */
#define  CM_ORD_BGAUCHE				165  /* Type objet bouton Gauche */
#define  CM_ORD_BDROITE				166  /* Type objet bouton Droite */
#define  CM_ORD_BQUIT				167  /* Type objet Quitter */
#define  CM_ORD_BIMP				168  /* Type objet Quitter */
#define  CM_ORD_POSEIT				169 /* Type objet POSEIT */
#define  CM_ORD_PASTILLE			170 /* Type objet PASTILLE DU POSTIT */
#define  CM_ORD_CARTE				171  /* Type objet CARTE */
#define  CM_ORD_PR				172 /* type PR */
#define  CM_ORD_GRAPHE_STRADA			173
#define  CM_ORD_STRADA				174
#define  CM_ORD_REFUGE 				175  /* Type objet Refuge */

#define CM_NB_TYPE_OBJET_ORDRE			175

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* declarations de donnees externes */

/* delaration de fonctions externes */

#endif
