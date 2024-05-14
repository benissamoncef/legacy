/*E*/
/*Fichier :  $Id: xzic.h,v 1.77 2021/05/03 14:05:18 pc2dpdy Exp $      Release : $Revision: 1.77 $        Date : $Date: 2021/05/03 14:05:18 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME GESIHM
------------------------------------------------------
* MODULE xzic * FICHIER xzic.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Definition des elements communs au sous systeme GESIHM
*
------------------------------------------------------
* HISTORIQUE :
*
* NOEL	  	12 oct 1994	: Creation			   V1.1
* CORNU   	12 Oct 1994	: Ajout COM_CANAL_TAR01
*                             	  & COM_CANAL_MTPM		   V1.2
* GUILHOU 	13 oct 1994   	: Modif numero COM_CANAL_TAR01     V1.3
* GUILHOU 	14 oct 1994   	: Ajout COM_CANAL_MTMC_TFM
				& COM_CANAL_MTMC_PA		   v1.4
* NOEL    	14 Oct 1994	: Ajout canal MTMT_ALM		   V1.5
* GUILHOU 	14 Oct 199	: Ajout canal MTMT_MTLL		   V1.6
* CORNU   	26 Oct 1994	: Ajout COM_CANAL_MTCA                    V1.8
* CORNU   	04 Nov 1994	: Ajout COM_CANAL_MTEC
*			      	  & COM_CANAL_MTTU                        V1.10
* Lecrivain 	23 Nov 1994	: Ajout canaux MTAT et ASTREINTES         V1.13
* Cornu   	01 Dec 1994	: Ajout COM_CANAL_MTNA	                  V1.14
* Noel   	05 Dec 1994	: Ajout COM_CANAL_MTPA_FIN                V1.15
* Lecrivain 	06 Dec 1994	: Ajout canaux MTFS et MTLS               V1.16
* Lecrivain 	16 Dec 1994	: Ajout canal MTRA_ETAT                   V1.17
* Cornu   	20 Dec 1994	: Ajout COM_CANAL_MTPM_ETAT	          V1.18
* Guilhou	26 dec 1994	: ajout COM_CANAL_MTMT			  V1.19
* Guilhou	28 dec 1994	: ajout COM_CANAL_MTMC_SURCHARGE	  V1.21
* Guilhou	30 dec 1994	: ajout COM_CANAL_MTAA			  V1.22
* Lecrivain 	30 Dec 1994	: Ajout canal MTAT_ETAT                   V1.25
* Lecrivain 	03 Jan 1995	: Canal MTAT --> ETAT_TEL et ETAT_OPE     V1.26
* Guilhou       09 jan 1995	: canal MTLT_SURCHARGE			  V1.27
* Guilhou       10 jan 1995	: canal MTLT_FINSURCHARGE		  V1.28
* Guilhou       17 jan 1995	: canal MTLT_FIN			  V1.30
* Lecrivain 	18 Jan 1995	: Ajout canal MTRA                        V1.31
* Volcic 	20 Jan 1995	: Ajout canal MTCM                        V1.32
* Guilhou 	24 Jan 1995	: Ajout canal MTMT_REVEIL_ON              V1.33
					      MTMT_REVEIL_OFF
* Lecrivain 	13 Fev 1995	: Ajout canal MTLT_FMC                    V1.36
* Lecrivain 	29 Mar 1995	: Ajout canaux MTPI                 V1.38, 1.39
* Lecrivain 	04 Avr 1995	: Ajout canal MTMT_REPONDEUR              V1.40
* Lecrivain 	20 Avr 1995	: Ajout canal COM_CANAL_FIN_MSG           V1.41
* Lecrivain 	28 Avr 1995	: Ajout canal COM_CANAL_MTLL_MODIF        V1.42
* Lecrivain 	01 Jun 1995	: Ajout canal COM_CANAL_MTMT_COMM_GN      V1.43
* Torregrossa	27 Jun 1995	: Ajout canaux COM_CANAL_MTLT_OUVERTURE_FMC et
*                                 COM_CANAL_MTLT_VALIDATION_FMC           V1.44
* Lecrivain 	31 Jul 1995	: Ajout MTRA_RAFRAICHIR,MT_COMM_GN_TRANSFR V1.45
* Mismer 	21 Sep 1995	: Ajout canal COM_CANAL_MTNM	       	V1.46
* Torregrossa	15 Jan 1996	: Ajout canal COM_CANAL_MTAA_FIN V1.47
* Noel		6  Fev 1996	: Commentaire sur la plage de pokes 	V1.48
* Guilhou	22 aut 1996 	: Ajout COM_CANAL_MTMT_ETAT_PCSIMPL	V1.49
* Verdier	27 aou 1996	: Ajout COM_CANAL_VEH_FINI  (DEM76)
* Guilhou	20 sep 1996	: Ajout COM_CANAL_MTLT_TRV (PC SIMPLIFIE) V1.51
* niepceron	26 sep 1996     : Ajout COM_CANAL_MTLT_ACQT (DEL/424)	v1.52
				  Ajout COM_CANAL_MTLT_RAF  (DEL/424)   v1.52
* Guilhou	21 nov 2996	: ajout COM_CANAL_MTAS	(gestion astreintes) (DEM/1306) 1.53
* Mismer	25 Mar 1997	: ajout constantes TDP (DEM/1417) 1.54 
* Mismer	20 Jan 1998	: ajout constantes PAL (DEM/) 1.55 
* Niepceron	10 Mar 2005 	: ajout COM_CANAL_MTCA_FIN DEM434 1.56 
* Hilmarcher    17 Aou 2005     : ajout COM_CANAL_INDIV DEM493 v1.58
* JMG		13/09/05	: ajout PMVA BAF BAD 1.59
* JMG		13/09/05	: correction COM_CANAL_MTPMVA_ETAT 1.60
* JBL		Nov 2007	: DEM 662
* JPL		17/09/08	: Ajout canal COM_CANAL_MTMT_ETAT_PIRAU (DEM 808) 1.62
* JPL		19/10/10	: Ajout COM_CANAL_MTCA_RAPIDE, COM_CANAL_MTCA_RAPIDE_FIN (DEM 949)  1.63
* JPL		15/11/10	: Commentaires pour compilation sur architecture HP ia64 (DEM 961)  1.64
* JMG		18/01/11	: ajout COM_CANAL_MTMT_ALARME et  COM_CANAL_MTMT_EQT 1.65
* JMG           04/04/12 : ajout COM_CANAL_COMMU 1.66
* VR		08/05/12	 ajout equipement PRV (DEM/1016) 1.67
* JPL		13/11/12	: Ajout constantes caracteres de messages IHM <-> tache relais  1.68
* PNI		15/12/14	: Ajout COM_CANAL_KEEP_ALIVE_TIMER 1.69 DEM1116
* JMG		26/04/16	: jaout COM_CANAL_MTSAGA_ETAT 1.70 SAGA
* LCL		22/02/18	: ajout COM_CANAL_MTCFE CTRL_FEUX 1.71 DEM1284
* LCL		11/05/18	: ajout COM_CANAL_MTCAP GABEB 1.72 DEM1283
* JMG		29/10/18	: ajout COM_CANAL_RMVI 1.73
* JMG		12/06/19 	: ajout COM_INFOMESSAGE ODI-C DEM1338 1.74
* JMG		17/11/19	: ajout COM_CANAL_TDP_SURCHARGE DEM1365 1.75
* CGR		24/11/20	: ajout COM_CANAL_MTIMU DEM-SAE155 1.76
* ABE		02/04/21	: ajout COM_CANAL_MTRA_ETAT_SONO DEM-SAE244 1.76
* GGY		24/11/23	: ajour COM_CANAL_MTPIC_ETAT (DEM483)
------------------------------------------------------ */

/* Non Applix - debut */
#ifndef xzic
#define xzic
/* Non Applix - fin */

/* fichiers inclus */

/* definitions de constantes exportees */

#define	XZIC_MSG_ASC_SEP_ELEM		"~"
#define	XZIC_MSG_ASC_SEP_PARAM		"|"
#define	XZIC_MSG_ASC_FIN		"\n"


/*
 * Definition des canaux de communication inter-modules
 *
 * ATTENTION LA PLAGE AUTORISEE EST [1, 200] le reste
 * est interne au sous systeme GESIHM 
 * ----------------------------------------------------
 */

#define  COM_CANAL_FIN   	         1
#define  COM_CANAL_MTMA  	         2
#define  COM_CANAL_MTMC  	         3
#define  COM_CANAL_MTLT  	         4
#define  COM_CANAL_MTLV  	         5
#define  COM_CANAL_MTFX  	         6
#define  COM_CANAL_MTAV  	         7
#define  COM_CANAL_QUIT	 	         8	/* reserve tache relais! ne pas modifier!*/
#define  COM_CANAL_TAR01 	         9	/* Communication applix --> module C */
#define  COM_CANAL_MTPM  	        10	/* reception message pilotage intensite */
#define  COM_CANAL_MTMC_TFM  	        11 	/* message TFM pret */
#define  COM_CANAL_MTMC_PA   	        12 	/* plan d'actions pret */
#define  COM_CANAL_MTMT_ALM  	        13 	/* reception d'une modif dans la liste des alarmes */
#define  COM_CANAL_MTLL 	        14
#define  COM_CANAL_MTCA  	        15	/* reception message pilotage magnetoscope */
#define  COM_CANAL_MTEC_ALM  	        16	/* reception message d'alarmes pilotage echangeur */
#define  COM_CANAL_MTEC_PAN  	        17	/* reception message etat panneau pilotage echangeur */
#define  COM_CANAL_MTEC_SEQ  	        18	/* reception message etat sequence pilotage echangeur */
#define  COM_CANAL_MTTU_ALM  	        19	/* reception message d'alarmes pilotage tunnel */
#define  COM_CANAL_MTTU_PAN  	        20	/* reception message etat panneau pilotage tunnel */
#define  COM_CANAL_MTTU_SEQ  	        21	/* reception message etat sequence pilotage tunnel */
#define  COM_CANAL_MTAT		        22	/* appel telephonique */
#define  COM_CANAL_ASTREINTES	        23	/* signal de fin dialogue astreintes */
#define  COM_CANAL_MTNA  	        24	/* reception message etat panneau pilotage NAV */
#define  COM_CANAL_MTPA  	        25	/* demande de fin envoyee par la main courante MTMC */ 
#define  COM_CANAL_MTFS  	        26	/* reception demande de MTAT (MTPA) */
#define  COM_CANAL_MTLS  	        27	/* reception demande de MTAT (MTPA) */
#define  COM_CANAL_MTRA_ETAT  	        28	/* reception message de l'equipement */
#define  COM_CANAL_MTAT_ETAT_TEL        29	/* reception etat Autotel */
#define  COM_CANAL_MTPM_ETAT	        30	/* reception message d'abt etat PMV */
#define  COM_CANAL_MTPM_PICTO	        31	/* reception message d'abt etat Picto */
#define  COM_CANAL_MTIE	                32	/* reception message d'abt LCR */
#define  COM_CANAL_MTMT	                33	/* message de terminaison de menu */
#define  COM_CANAL_MTMC_SURCHARGE       34	/* reponse a la demande de surcharge*/
#define  COM_CANAL_MTAA   		35	/* message systeme*/
#define  COM_CANAL_MTAT_ETAT_OPE        36	/* reception etat Operator */
#define  COM_CANAL_MTLT_SURCHARGE       37	/* poke de surcharge */
#define  COM_CANAL_MTLT_FINSURCHARGE    38	/* poke de fin de surcharge */
#define  COM_CANAL_MTLT_FIN	        39	/* poke de fin de la liste  */
#define  COM_CANAL_MTRA   		40	/* Appel RAU : MTFL -> MTRA */
#define  COM_CANAL_MTCM 	        41	/* reception message pilotage camera */
#define	 COM_CANAL_MTMT_REVEIL_ON	42	/* presence d'un reveil*/
#define  COM_CANAL_MTMT_REVEIL_OFF	43	/* plus de reveil*/
#define  COM_CANAL_MTLT_FMC		44	/* demande ouverture FMC */
#define	 COM_CANAL_MTAV_FIN		45	/* fin de la tache*/
#define  COM_CANAL_MTLL_FIN		46	/* fin de la tache*/
#define  COM_CANAL_MTPI			47	/* message au post-it */
#define  COM_CANAL_MTPI_FIN		48	/* fermeture du post-it */
#define  COM_CANAL_MTMT_REPONDEUR	49	/* etat repondeur telephonique*/
#define  COM_CANAL_FIN_MSG		50	/* Fin d'affichage message op.*/
#define  COM_CANAL_MTLL_MODIF		51	/* Modif. liste des alertes */
#define  COM_CANAL_MTMT_COMM_GN		52	/* Mise a jour icone comm. GN */
#define  COM_CANAL_MTLT_OUVERTURE_FMC	53	/* Mise a jour reveil automatique fmc */
#define  COM_CANAL_MTLT_VALIDATION_FMC	54	/* Mise a jour reveil automatique fmc */
#define  COM_CANAL_MTLT_TIMER		55	/* envoi d'un top toutes les minutes a la liste a traiter */
#define  COM_CANAL_MTRA_RAFRAICHIR	56	/* Etat ecoute GN: MTMT-> MTRA*/
#define  COM_CANAL_MTMT_COMM_GN_TRANSFR	57	/* Transfert effectue MTRA->MT*/
#define  COM_CANAL_MTNM  	        58	/* reception message etat matice NAV */
#define  COM_CANAL_MTAA_FIN		59	/* fin de la tache message systeme*/
#define  COM_CANAL_MTMT_ETAT_PCSIMPL	60	/* demande d'etat du PC simplifie*/
#define  COM_CANAL_MTLT_LOCAL		61	/* envoi de la liste des FMC locales*/
#define	 COM_CANAL_MTLT_OPERATEUR	62	/* ajout FMC operateur dans la liste des FMC locales*/
#define  COM_CANAL_VEH_FINI		63	/* MAJ des fiches vehicules terminees */
#define  COM_CANAL_MTLT_TRV		64	/* maj de la liste des travaux en prevision */
#define  COM_CANAL_MTLT_ACQT            65      /* demande d'acquittement d'un evt au voisin */
#define  COM_CANAL_MTLT_RAF		66	/* demande de rafraichissement des icones */
#define	 COM_CANAL_MTAS			67	/* indique le resultat de l'appel telephonique*/
#define  COM_CANAL_MTPM_TDP	        68	/* reception message valeur TDP pour PMV */
#define  COM_CANAL_MTES			69	/* evt strada */
#define  COM_CANAL_MTES_FIN		70	
#define	 COM_CANAL_MTES_MODIF		71
#define  COM_CANAL_MTPP  	        72	/* reception message etat panneau pilotage PAL */
#define  COM_CANAL_MTCA_FIN                     73
#define  COM_CANAL_MTDP_TIMER			74      /*reception message synthese des tdp pour RTFM uniquement */ 
#define  COM_CANAL_INDIV  		75      /*reception message pour le calcul des donnees individuelles des stations RADT */
#define  COM_CANAL_FIN_INDIV            76      /*reception message fin timer pour les donnees individuelles des stations RADT */
#define  COM_CANAL_MTPMVA_ETAT            77
#define COM_CANAL_MTBAF		78
#define COM_CANAL_MTBAD		79
#define COM_CANAL_MTPMVA_TDP	80
#define  COM_CANAL_MTMT_ETAT_PIRAU	81	/* reception de l'etat Pirau */
#define	 COM_CANAL_MTCA_RAPIDE		82	/* commande de pilotage rapide des cameras */
#define  COM_CANAL_MTCA_RAPIDE_FIN	83	/* fin de la tache de pilotage rapide des cameras */
#define  COM_CANAL_DEJA_SIMU		84
#define	 COM_CANAL_FIN_SIMU			85
#define  COM_CANAL_MTPRV_ETAT       86
#define  COM_CANAL_MTPRV			87
#define  COM_CANAL_MTCFE            88
#define  COM_CANAL_MTCAP            89
#define  COM_CANAL_MTIMU            90
#define	 COM_CANAL_MTRA_ETAT_SONO   91
#define  COM_CANAL_MTPIC_ETAT				86	

#define  COM_CANAL_MTTU_EQP_VEN  	        200	/* reception message etat equipement ventilation tunnel */
#define  COM_CANAL_MTTU_SEQ_VEN  	        210	/* reception message etat sequence ventilation tunnel */

#define  COM_CANAL_MTTU_EQP_ECL  	        201	/* reception message etat equipement eclairage tunnel */
#define  COM_CANAL_MTTU_SEQ_ECL  	        211	/* reception message etat sequence eclairage tunnel */
#define  COM_CANAL_MTTU_SEQ_ENR  	        212	/* reception message etat sequence energie */
#define  COM_CANAL_MTTU_SEQ_EXP  	        213	/* reception message etat sequence exploitation */
#define  COM_CANAL_MTTU_SEQ_INC  	        218	/* reception message etat sequence exploitation */

#define  COM_CANAL_MTTU_EQP_ENE  	        202	/* reception message etat equipement energie */
#define  COM_CANAL_MTTU_EQP_EXP  	        203	/* reception message etat equipement exploitation */
#define  COM_CANAL_MTTU_EQP_LIT  	        204	/* reception message etat equipement lit arret */
#define  COM_CANAL_MTTU_EQP_LT  	        205	/* reception message etat equipement locaux techniques */
#define  COM_CANAL_MTTU_EQP_SYS  	        206	/* reception message etat equipement systeme */

#define  COM_CANAL_MTTU_DOM_SIGT  	        301	/* reception message etat domaine signalisation tunnel */
#define  COM_CANAL_MTTU_DOM_SIGE  	        302	/* reception message etat domaine signalisation echangeur */
#define  COM_CANAL_MTTU_DOM_VEN  	        303	/* reception message etat domaine ventilation tunnel */
#define  COM_CANAL_MTTU_DOM_ECL  	        304	/* reception message etat domaine eclairage tunnel */
#define  COM_CANAL_MTTU_DOM_ENE  	        305	/* reception message etat domaine energie */
#define  COM_CANAL_MTTU_DOM_EXP 	        306	/* reception message etat domaine exploitation tunnel */
#define  COM_CANAL_MTTU_DOM_LIT  	        307	/* reception message etat domaine lit arret */
#define  COM_CANAL_MTTU_DOM_LT  	        308	/* reception message etat domaine LT */
#define  COM_CANAL_MTTU_DOM_SYS  	        309	/* reception message etat domaine systeme */

#define COM_CANAL_MTMT_ALARME			310
#define COM_CANAL_MTMT_EQT			311

#define COM_CANAL_COMMU				312
#define COM_CANAL_KEEP_ALIVE_TIMER		313

#define COM_CANAL_MTSAGA_ETAT			314

#define COM_CANAL_TRAU_BASCUL			315

#define COM_CANAL_RMVI_STATUS			316
#define COM_CANAL_RMVI_RETOUR			317
#define COM_CANAL_TDP_SURCHARGE			318

#define  COM_CANAL_TEST				999	/* reserve */


/*A
 * Fonctions dont DataViews demande l'execution a Applix
 * -----------------------------------------------------
 */

#define COM_SENDPOKE        	(2)
#define COM_NEWTASK	 	(3)
#define COM_SETVARCTX       	(4)
#define COM_GETVARCTX       	(5)
#define COM_INFOMESSAGE       	(13)


/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

/* Non Applix - debut */
#endif
/* Non Applix - fin */
