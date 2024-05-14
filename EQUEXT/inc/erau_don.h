/*E*/
/*  Fichier : $Id: erau_don.h,v 1.13 2021/06/08 12:08:06 pc2dpdy Exp $      Release : $Revision: 1.13 $        Date : $Date: 2021/06/08 12:08:06 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TERAU * FICHIER erau_don.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Structure des donnees du fichier de definition de donnees 
* de la tache TERAU
*
------------------------------------------------------
* HISTORIQUE :
*
* Nagiel.E	version 1.1	30 Nov 1994	: Creation
* Mismer.D	version 1.2	02 Jan 1995	:
* Nagiel.E	version 1.3	24 Jan 1995	:
* Fontaine.C	version 1.4	22 Mai 1995	: Reprise entete, historique
* Volcic.F	version 1.5	01 Aou 1995	: Ajout xzir.h
* Mismer.D	version 1.6	06 Nov 1995	: Suppression vg_quarte
* Mismer.D	version 1.7	24 Oct 1996	: Modif pour Alerte PAU HS (DEM/1282)
* Mismer.D	version 1.8	24 Oct 1996	: Modif pour Alerte PAU HS Suite (DEM/1308)
* Mismer.D	version 1.9	05 Dec 1996	: Suite modif pour Alerte PAU HS Suite (DEM/1308)
* Mismer.D	version 1.10	21 Jan 1998	: Modif pour basculement RAU PC2 (DEM/1542)
* Diaz.L	version 1.11	09 Oct 1998	: Modif du au PC Niv.2
* JMG		version 1.12	06/05/19	: ajout type DEM1334
* ABE		version 1.13	09/04/21	: Ajout RAU SONO DEM-SAE244
* ABE		version 1.14	08/06/21	: Fix nom autoroute int vers str RAU SONO DEM-SAE244
* GGY		07/09/23	: Passage des RAU en IP : ajout de adresse_ip_esclave, port_ip_esclave, adresse_ip_maitre, port_ip_maitre	(DEM483)
------------------------------------------------------ */
#ifndef erau_don
#define erau_don

/* fichiers inclus */
#include "ex_mdon.h"
#include "xdf.h"
#include "erau_cfg.h"
#include "erau_cmd.h"
#include "erau_tra.h"
#include "erau_cyc.h"
#include "erau_phs.h"
#include "xzeu.h"
#include "xzir.h"
#include "ex_mala.h"
#include "ex_malt.h"
#include "xzaac.h"
#include "xzamc.h"
#include "ex_mlcr.h"

/* definitions de constantes exportees */
#define ERAUC_REVEIL_1MN		60
#define ERAU_NB_MAX_CONSECUTIFS  	5
#define ERAU_NB_MIN_ALERTE	  	2

/* definitions de types exportes */
typedef struct	{
		   	XDY_Eqt			Numero;
   		   	XDY_NomST		NumeroST;
   		   	XDY_Mot			NoLSBascul;
   		   	XDY_Mot			NoLSMaitre;
   		   	XDY_Mot			NoLSEsclave;
   		   	int				TimeOut;
   		   	int				TimeOutRetour;
   		   	int				NbQuart;
   		   	XDY_Rgs			AdresseRGS;
		   	XDY_AdresseIP	AdresseIpEsclave;
			XDY_PortIP		PortIpEsclave;
			XDY_AdresseIP	AdresseIpMaitre;
			XDY_PortIP 		PortIpMaitre;
} ERAU_CONFIG_SYSTRAU;
		
typedef struct	{
		   ERAU_CONFIG_SYSTRAU	Config;
		   int			Basculement[10];
   		   XDY_Mot		Reveil;
   		   int			SocketMaitre;
   		   int			SocketBascul;
   		   int			SocketEsclave;
		   int			Etat_Service;
		   int			PC2;
		   int			Operateur;
} ERAU_DONNEES_SYSTRAU;
		
typedef struct	{
		   XDY_Eqt		Numero;
 		   int 			CodePAU;
 		   int			CodeAntenne;
		   XDY_Octet		Type;
} ERAU_CONFIG_PAU;
		
typedef struct	{
 		   int			NoDerCmd;
 		   int			CRCmd;
 		   int			Gardepau;
 		   int			Test;
		}ERAU_ETAT_PAU;
		
typedef struct	{
		   ERAU_CONFIG_PAU	Config;
		   ERAU_ETAT_PAU	Etat;
		   int			Etat_Service;
	           void *		Suivant;
		}ERAU_DONNEES_PAU;


typedef struct	{
 		   XDY_NomMachine	Machine;
	           void *		Suivant;
		}ERAU_CONFIG_VOISIN;

typedef struct	{
    		   XDY_Mot		Nb_PAU;
    		   XDY_Mot		Nb_PAU_Consecutif;
    		   XDY_Mot		Num_Alt_Consecutif;
    		   XDY_TexteAlerte	Texte_Alt_Consecutif;
    		   XDY_Mot		Nb_PAU_HS;
    		   XDY_Mot		Num_Alt_HS;
    		   XDY_TexteAlerte	Texte_Alt_HS;
    		   XDY_Mot		Alerte;
		}ERAU_PARAM_ALERTE;

typedef struct {
			XDY_MessageSONO   Message;
			XDY_Mot	     Nb_PAUErreur; 
			XDY_Mot	     Nb_PAUPilotes;								
			XDY_NomAuto	Autoroute;
			XDY_Sens	Sens;
			XDY_PR		PR_debut;
			XDY_PR		PR_fin;
	}ERAU_DONNEES_RAU_SONO;
		

/* definition de macro exportees */

/* declarations de donnees globales */

extern ERAU_DONNEES_PAU		*pg_debutPAU;
extern ERAU_DONNEES_SYSTRAU	vg_donsysrau;
extern INFO_EQT_FEN_LCR		*pg_DebutListeRAUFLcr;

/* Pointeur liste Info PAU pour alerte PAU HS */
extern XZEXT_CONFIG_PAU		*pg_DebInfoPAU;
extern ERAU_CONFIG_VOISIN	*pg_DebListeVoisin;	
extern ERAU_PARAM_ALERTE	vg_Config_PAU_HS;

extern XDY_NomMachine		vg_NomMachine;
extern XDY_NomSite        	vg_NomSite;
extern int        		vg_PC2;
extern int			vg_Mode_Fonct;
extern int			vg_etatGN;
extern int			vg_etatPO1;
extern int			vg_etatPO2;
extern int			vg_etatGNFut;
extern int			vg_etatPO1Fut;
extern int			vg_etatPO2Fut;
extern int			vg_SockTIM;
			
extern int			vg_status_recu;
extern int			vg_no_po_test;
extern int			vg_test_auto_en_cours;

/* delaration de fonctions externes */

EX_DECLA_INSERER   ( ERAU_DONNEES_PAU, ed_ins_liste_PAU )
EX_DECLA_SUPPRIMER ( ed_sup_liste_PAU )	

#endif
