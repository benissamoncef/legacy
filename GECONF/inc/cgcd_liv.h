/*E Fichier : $Id: cgcd_liv.h,v 1.10 2016/07/13 23:02:59 devgfi Exp $      Release : $Revision: 1.10 $        Date : $Date: 2016/07/13 23:02:59 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME GECONF
------------------------------------------------------
* MODULE MLIV * FICHIER MLIV.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module de livraison de tcgcd gerant le configuration.
* (DCG-002/0.1).
*
------------------------------------------------------
* HISTORIQUE :
*
* mercier	05 Dec 1994	: Creation		V 1.1
**************
* Mercier	15 Mars 1995	: Correction de la const	V 1.3
*	 CLIVC_LIVR_EXEC_EQUEXT_SOURCE
* Niepceron     08 Jun 1995	: Ajout des cstes pour les fic. .../inf V1.4
*************
* NIEPCERON     29 Jan 1996     : Ajout du ssysteme DIE v1.5
*************
* NIEPCERON     05 Fev 1996     : Ajout du ssysteme MOD v1.6
*************
* NIEPCERON     28 Fev 1996	: Ajout des constantes pour l'espace de travail v1.7
* niepceron     21 Oct 1996     : Ajout des ssystemes OPER UTIL et NTFM v1.8
* niepceron     27 Jan 1999	: Ajout des ssystemes STRA v1.9 dem/1728
* JPL		13/07/2016	: Declaration fonctions "cliv_TypeFich_VerifGeree" et "cliv_Consult_Tab_Checklist"  1.10
------------------------------------------------------ */

#ifndef MLIV
#define MLIV

/* fichiers inclus */
	/* includes systeme */
	#include <stdio.h>
	
	/* includes Utilitaires */
	
		/* rtworks */
	#include        <rtworks/common.h>
	#include        <rtworks/ipc.h>
	#include        <rtworks/ipc_proto.h>
	
	/* includes applicatifs Migrazur */
	#include "xzcg.h"
	#include "xzss.h"
	#include "xzsm.h"
	#include "xzsp.h"
	#include "xzia.h"
	#include "xdc.h"
	#include "xdf.h"
	
/* definitions de constantes exportees */
#define CLIVC_LIVRAISON		"LIVRAISON"
#define CLIVC_DIFFUSION		"DIFFUSION"

#define CLIVC_LIVR_SCNT_SOURCE 	XDC_PATHREFFIC "/snt" 
#define CLIVC_LIVR_SCNT_CONFIG	XDC_PATHCNFFIC "/snt"
#define CLIVC_LIVR_SCNT_TRAV  	XDC_PATHTRAVFIC "/snt"

#define CLIVC_LIVR_SNAV_SOURCE 	XDC_PATHREFFIC "/nav" 
#define CLIVC_LIVR_SNAV_CONFIG	XDC_PATHCNFFIC "/nav"
#define CLIVC_LIVR_SNAV_TRAV  	XDC_PATHTRAVFIC "/nav"

#define CLIVC_LIVR_LPIC_SOURCE	XDC_PATHREFFIC "/pic"
#define CLIVC_LIVR_LPIC_CONFIG	XDC_PATHCNFFIC "/pic"
#define CLIVC_LIVR_LPIC_TRAV  	XDC_PATHTRAVFIC "/pic"

#define CLIVC_LIVR_MPMV_SOURCE	XDC_PATHREFFIC "/pmv"
#define CLIVC_LIVR_MPMV_CONFIG	XDC_PATHCNFFIC "/pmv"
#define CLIVC_LIVR_MPMV_TRAV  	XDC_PATHTRAVFIC "/pmv"

#define CLIVC_LIVR_EQAL_SOURCE	XDC_PATHREFFIC "/eqa"
#define CLIVC_LIVR_EQAL_CONFIG	XDC_PATHCNFFIC "/eqa"
#define CLIVC_LIVR_EQAL_TRAV  	XDC_PATHTRAVFIC "/eqa"

#define CLIVC_LIVR_SYNP_SOURCE	XDC_PATHREFFIC "/syn"
#define CLIVC_LIVR_SYNP_CONFIG	XDC_PATHCNFFIC "/syn"
#define CLIVC_LIVR_SYNP_TRAV  	XDC_PATHTRAVFIC "/syn"

#define CLIVC_LIVR_RSAD_SOURCE	XDC_PATHREFFIC "/bdc"
#define CLIVC_LIVR_RSAD_CONFIG	XDC_PATHCNFFIC "/bdc"
#define CLIVC_LIVR_RSAD_TRAV  	XDC_PATHTRAVFIC "/bdc"

#define CLIVC_LIVR_FAID_SOURCE	XDC_PATHREFFIC "/aid"
#define CLIVC_LIVR_FAID_CONFIG	XDC_PATHCNFFIC "/aid"
#define CLIVC_LIVR_FAID_TRAV  	XDC_PATHTRAVFIC "/aid"

#define CLIVC_LIVR_FINF_SOURCE  XDC_PATHREFFIC "/inf"
#define CLIVC_LIVR_FINF_CONFIG  XDC_PATHCNFFIC "/inf"
#define CLIVC_LIVR_FINF_TRAV  	XDC_PATHTRAVFIC "/inf"

#define CLIVC_LIVR_PIGN_SOURCE	XDC_PATHREFFIC "/ign"
#define CLIVC_LIVR_PIGN_CONFIG	XDC_PATHCNFFIC "/ign"
#define CLIVC_LIVR_PIGN_TRAV  	XDC_PATHTRAVFIC "/ign"

#define CLIVC_LIVR_DIE_SOURCE  XDC_PATHREFFIC "/die"
#define CLIVC_LIVR_DIE_CONFIG  XDC_PATHCNFFIC "/die"
#define CLIVC_LIVR_DIE_TRAV  	XDC_PATHTRAVFIC "/die"

#define CLIVC_LIVR_MOD_SOURCE  XDC_PATHREFFIC "/mod"
#define CLIVC_LIVR_MOD_CONFIG  XDC_PATHCNFFIC "/mod"
#define CLIVC_LIVR_MOD_TRAV  	XDC_PATHTRAVFIC "/mod"

#define CLIVC_LIVR_MFBO_SOURCE	XDC_PATHREFFIC "/mbo"
#define CLIVC_LIVR_MFBO_CONFIG	XDC_PATHCNFFIC "/mbo"
#define CLIVC_LIVR_MFBO_TRAV  	XDC_PATHTRAVFIC "/mbo"

#define CLIVC_LIVR_DEFT_SOURCE	XDC_PATHREFFIC "/dft"
#define CLIVC_LIVR_DEFT_CONFIG	XDC_PATHCNFFIC "/dft"
#define CLIVC_LIVR_DEFT_TRAV  	XDC_PATHTRAVFIC "/dft"

#define CLIVC_LIVR_FADM_SOURCE	XDC_PATHREFFIC "/adm"
#define CLIVC_LIVR_FADM_CONFIG	XDC_PATHCNFFIC "/adm"
#define CLIVC_LIVR_FADM_TRAV  	XDC_PATHTRAVFIC "/adm"

#define CLIVC_LIVR_DORA_SOURCE	XDC_PATHREFFIC "/dra"
#define CLIVC_LIVR_DORA_CONFIG	XDC_PATHCNFFIC "/dra"
#define CLIVC_LIVR_DORA_TRAV  	XDC_PATHTRAVFIC "/dra"

#define CLIVC_LIVR_EQUI_SOURCE	XDC_PATHREFFIC "/deq"
#define CLIVC_LIVR_EQUI_CONFIG	XDC_PATHCNFFIC "/deq"
#define CLIVC_LIVR_EQUI_TRAV  	XDC_PATHTRAVFIC "/deq"

#define CLIVC_LIVR_OPER_SOURCE  XDC_PATHREFFIC "/opr"
#define CLIVC_LIVR_OPER_CONFIG  XDC_PATHCNFFIC "/opr"
#define CLIVC_LIVR_OPER_TRAV    XDC_PATHTRAVFIC "/opr"

#define CLIVC_LIVR_UTIL_SOURCE  XDC_PATHREFFIC "/util"
#define CLIVC_LIVR_UTIL_CONFIG  XDC_PATHCNFFIC "/util"
#define CLIVC_LIVR_UTIL_TRAV    XDC_PATHTRAVFIC "/util"

#define CLIVC_LIVR_NTFM_SOURCE  XDC_PATHREFFIC "/ntfm"
#define CLIVC_LIVR_NTFM_CONFIG  XDC_PATHCNFFIC "/ntfm"
#define CLIVC_LIVR_NTFM_TRAV    XDC_PATHTRAVFIC "/ntfm"

#define CLIVC_LIVR_STRA_SOURCE  XDC_PATHREFFIC "/stra"
#define CLIVC_LIVR_STRA_CONFIG  XDC_PATHCNFFIC "/stra"
#define CLIVC_LIVR_STRA_TRAV    XDC_PATHTRAVFIC "/stra"

#define CLIVC_LIVR_EXEC_BASEDD_SOURCE 	XDC_PATHREF "/BASEDD/bin"	
#define CLIVC_LIVR_EXEC_BASEDD_CONFIG	XDC_PATHCNFEXE

#define CLIVC_LIVR_EXEC_EQUEXT_SOURCE 	XDC_PATHREF "/EQUEXT/bin"
#define CLIVC_LIVR_EXEC_EQUEXT_CONFIG	XDC_PATHCNFEXE

#define CLIVC_LIVR_EXEC_GECONF_SOURCE 	XDC_PATHREF "/GECONF/bin"
#define CLIVC_LIVR_EXEC_GECONF_CONFIG	XDC_PATHCNFEXE

#define CLIVC_LIVR_EXEC_GESIHM_SOURCE 	XDC_PATHREF "/GESIHM/bin"
#define CLIVC_LIVR_EXEC_GESIHM_CONFIG	XDC_PATHCNFEXE
	

#define CLIVC_LIVR_EXEC_MAINTE_SOURCE 	XDC_PATHREF "/MAINTE/bin"
#define CLIVC_LIVR_EXEC_MAINTE_CONFIG	XDC_PATHCNFEXE

#define CLIVC_LIVR_EXEC_SAIDEC_SOURCE 	XDC_PATHREF "/SAIDEC/bin"
#define CLIVC_LIVR_EXEC_SAIDEC_CONFIG	XDC_PATHCNFEXE

#define CLIVC_LIVR_EXEC_SIMTST_SOURCE 	XDC_PATHREF "/SIMTST/bin"
#define CLIVC_LIVR_EXEC_SIMTST_CONFIG	XDC_PATHCNFEXE


#define CLIVC_LIVR_EXEC_SUPERV_SOURCE 	XDC_PATHREF "/SUPERV/bin"
#define CLIVC_LIVR_EXEC_SUPERV_CONFIG	XDC_PATHCNFEXE

#define CLIVC_LIVR_EXEC_XDMICG_SOURCE 	XDC_PATHREF "/XDMICG/bin"
#define CLIVC_LIVR_EXEC_XDMICG_CONFIG	XDC_PATHCNFEXE

/* definitions de globales exportes */

extern XZIAT_destinataire CGCDV_NOMMACHINE;


/* definitions de types exportes */

/* Definition du tableau checklist */
typedef struct {
        int TypeVerif;                  /*Type de Verification  */
        int Flag;			/*XDC_OK ou XDC_NOK	*/
} CLIVT_Elt_Tab_Checklist;

CLIVT_Elt_Tab_Checklist CLIVG_Tab_Checklist[XZCGC_NB_VERIF];
extern FILE *CGCDG_Fich_CHECKLIST;
extern int CGCDV_CHECKLIST;

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

/*
*	cliv_cb : Callback de la tache TCGCD effectuant la livraison.
* SEQUENCE D'APPEL :
*/
extern void cliv_cb(T_IPC_CONN,T_IPC_CONN_PROCESS_CB_DATA ,T_CB_ARG );

/*
* SERVICE RENDU : 
*	cliv_FctUtil_Gest_ligne_bl permet de traiter 
*  une ligne du Bon de livraison conformement a la fonctionalite
*  de livraison des fichiers par tcgcd.x (page34 du DCG-002/0.1).
*/
extern int cliv_FctUtil_Gest_ligne_bl(XZCGT_ComposantBL ,int * );

/*
* SERVICE RENDU : 
*  cliv_Modif_Tab_Checklist : Permet de changer l etat 
*  dans le tableau checklist de la verification
*  pour un type de fichier donne.
*/
extern int cliv_Modif_Tab_Checklist(char * , int );


/*
* SERVICE RENDU :
*  cliv_TypeFich_VerifGeree retourne XZCGC_ERR_VERIF si le type de fichier n est pas gere au niveau du tab de checklist
*       sinon on retourne le numero de verification.
*/
extern int cliv_TypeFich_VerifGeree (char *);


/*
* SERVICE RENDU : 
*  cliv_Verif_TypeFich retourne XDC_OK si le typeFichier passe en argument
*  est un type connu dans xzcg.h sinon retourne XDC_NOK.
*/
extern int cliv_Verif_TypeFich(char * );

/*
* SERVICE RENDU : 
*  cliv_Init_Tab_Checklist : Initialise le tableau checklist
*  avec le fichier XDF_CHECKLIST qui se trouve dans XDC_PATHCNF.
*  Si le fichier n existe pas alors on le constitue avec des flags XDC_NOK
*  pour les types de fichiers connus.
*/
extern int cliv_Init_Tab_Checklist();

/*	cliv_Inform_Admin_Probl permet d informer l administrateur d un probleme
* survenu lors de l execution d une demande de livraison ou de diffusion.
*/
extern int cliv_Inform_Admin_Probl (char *,char *,char *,char *,char *,char *,char *,int);
/*
* SERVICE RENDU : 
*  cliv_Liv_Diff_Fichier : permet d effectuer la commande de livraison ou de diffusion
*  et de prevenir en cas de probleme l administrateur.
* 
*/

extern int cliv_Liv_Diff_Fichier(	char * ,
					char * ,
					char * ,
					char * ,
					char * ,
					char * ,
					char * );

/*
* SERVICE RENDU :
*  cliv_Consult_Tab_Checklist : Permet de recuperer l etat
*  dans le tableau checklist de la verification
*  pour un type de fichier donne.
*/

extern int cliv_Consult_Tab_Checklist (char *, int *);


/*
* SERVICE RENDU : 
*  cliv_ConsultGlobale_Tab_Checklist : Renvoie XDC_OK si toutes les verif ont ete effectuees
*  avec succes sinon XDC_NOK.
*/

extern int cliv_ConsultGlobale_Tab_Checklist();
					
extern int cdif_Diff_Taches     (       XZSCT_NomTache,
					int ,
					void **);

#endif
