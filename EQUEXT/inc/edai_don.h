/*E*/
/*Fichier :  $Id: edai_don.h,v 1.20 2020/11/03 18:06:38 pc2dpdy Exp $      Release : $Revision: 1.20 $        Date : $Date: 2020/11/03 18:06:38 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEDAI * FICHIER edai_don.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Structure des donnees du fichier de donnees de la tache TEDAI.
*
------------------------------------------------------
* HISTORIQUE :
*
* Nagiel.E	version 1.1	18 Oct 1994	: Creation
* Mismer.D	version 1.2	28 Nov 1994	: Suppression des #define XDF 
* Nagiel.E	version 1.3	01 Dec 1994	:
* Mismer.D	version 1.4	02 Dec 1994	:
* Mismer.D	version 1.5	22 Dec 1994	:
* Mismer.D	version 1.6	16 Jan 1995	:
* Mismer.D	version 1.7	17 Mar 1995	:
* Volcic.F	version 1.8	22 Mar 1995	: Modifs apres essais
* Fontaine.C	version 1.9	23 Mai 1995	: Reprise entete, historique
* Volcic.F	version 1.10	20 Jui 1995	: Ajout constantes Timeout
* Mismer.D	version 1.11	12 Dec 1995	: Ajout constantes Nb voie
* Mismer.D	version 1.12	19 Avr 1996	: Ajout MemTrace pour stockage mesures en fichier (DEM/1...) 
* Mismer.D	version 1.13	05 Dec 1996	: Modif alerte DAI tunnel (DEM/1309) 
* Mismer.D	version 1.14 	07 Jan 1997	: Ajout trace config voie sur travaux (DEM/1155)
* Mismer.D      version 1.15    08 Jan 1998     : Modif new alerte DAI (DEM/APG17) fiche ANA74
* Mismer.D      version 1.16    30 Mai 2000     : Modif pour config avec 2 DAI sur un LT (DEM/102)
* GUILHOU       version 1.17    1 oct 2007      : ajout site de gestion
* JPL		12/02/18 : Migration Linux 64 bits (DEM 1274) : type 'ushort' de EDAI_DONNEES_ANALYSEUR.MemTrace devient short  1.18
* JMG		version 1.19 01/06/19 passage IP DEM1333
* CGR		16/09/20 : Ajout deuxieme adresse IP DEM-SAE175 1.20
------------------------------------------------------ */

#ifndef edai_don
#define edai_don

/* fichiers inclus */


#include <time.h>
#include "ex_mdon.h"
#include "ex_mala.h"
#include "ex_msok.h"
#include "etim_don.h"
#include "edai_cfg.h"
#include "edai_cmd.h"
#include "edai_cyc.h"
#include "xzec.h"
#include "xzamc.h"
#include "xdf.h"
#include "ex_mlcr.h"
#include "xzah26.h"

/* definitions de constantes exportees */

#define	NB_ERR_POSSI            10

#define EDAIC_REVEIL_1MN	60

#define EDAIC_EQT_PANNE		2
#define EDAIC_EQT_LIBRE		0
#define EDAIC_EQT_OCCUP		1

#define EDAIC_NOCMD_COURS_OK	0
#define EDAIC_CMD_COURS_OK	1
#define EDAIC_NOCMD_COURS_NOK	2
#define EDAIC_CMD_COURS_NOK	3

#define EDAIC_CONF_DEBUT	0
#define EDAIC_CONF_ATTENTE	1
#define EDAIC_CONF_FIN		2

#define EDAIC_TIMEOUT		20
#define EDAIC_WAIT		3

#define EDAIC_NB_VOIE		5
#define EDAIC_MES_INCONNU	-1
#define EDAIC_AUCUN_VEHICULE	255
#define EDAIC_MAX_ALERTE_TUNNEL	20

/* definitions de types exportes */

typedef struct	{
   		   XDY_Eqt		Numero;
   		   XDY_NomST		NumeroST;
   		   XDY_NoLS		NoLSMaitre;
   		   XDY_NoLS		NoLSEsclave;
   		   XDY_Rgs		AdresseRGS;
		XDY_District         SiteGestion;
		XDY_PortIP		Port;
		XDY_AdresseIP		AdresseIP;
		XDY_PortIP		PortE;
		XDY_AdresseIP		AdresseIPE;
		XDY_PortIP		Port2;
		XDY_AdresseIP		Adresse2IP;
		XDY_PortIP		Port2E;
		XDY_AdresseIP		Adresse2IPE;
                int                     DAIBis; /*if true, use '2'*/
		}EDAI_CONFIG_DAI;
		
typedef struct	{
		   EDAI_CONFIG_DAI	Config;
		   int			Etat_Service;
   		   int			SocketMaitre;
   		   int			SocketEsclave;
   		   int			EtatTrans;
	           void *		Suivant;
		}EDAI_DONNEES_DAI;
		
typedef struct	{
   		   XDY_Eqt		NumeroDAI;
   		   XDY_Mot		NoAnaly;
   		   XDY_Eqt		Numero;
   		   XDY_Mot		NbVoiesGerees;
		   XDY_District		SiteGestion;
		}EDAI_CONFIG_ANALYSEUR;
		
typedef struct	{
		   EDAI_CONFIG_ANALYSEUR Config;
   		   XDY_EMes_DAI		Mes;
		   int			EtatConf;
		   int			EtatConfVoie[EDAIC_NB_VOIE];
		   XDY_Voie		EtatVoie[EDAIC_NB_VOIE];
		   int			Etat_Service;
		   int			Alerte;
		   int			DernAlerte;
		   int			TabErrSyst[NB_ERR_POSSI];
		   short		MemTrace[10];
		   void *		Suivant;
		}EDAI_DONNEES_ANALYSEUR;

typedef struct	{
   		   XDY_Eqt		Analyseur[2];
   		   XDY_NomMachine	LT;
   		   XDY_Mot		Mot;
   		   XDY_Mot		Bit;
   		   XDY_Mot		Etat;
   		   XDY_Entier		Duree;
   		   XDY_Eqt		Alerte;
   		   XDY_TexteAlerte	Libelle;
   		   XDY_Booleen		EtatAlerte[2];
		} EDAI_CONFIG_ALERTE_DAI_TUNNEL;
		

/* definition de macro exportees */

/* declarations de donnees globales */

extern EDAI_DONNEES_DAI		*pg_debutDAI;
extern EDAI_DONNEES_DAI		*pg_ConfDAI;
extern EDAI_DONNEES_ANALYSEUR	*pg_debutAnaly;
extern EDAI_DONNEES_ANALYSEUR	*pg_ConfAnaly;
extern INFO_EQT_FEN_LCR		*pg_DebutListeDAIFLcr;
extern int			vg_Mode_Fonct;
extern int			vg_NumConf[128];
extern int			vg_SockTIM;
extern char			vg_idpassw[18];

/* delaration de fonctions externes */

EX_DECLA_INSERER   ( EDAI_DONNEES_DAI, ed_ins_liste_DAI )
EX_DECLA_SUPPRIMER ( ed_sup_liste_DAI )	
EX_DECLA_INSERER   ( EDAI_DONNEES_ANALYSEUR, ed_ins_liste_Analy )
EX_DECLA_SUPPRIMER ( ed_sup_liste_Analy )	

#endif
