/*E*/
/*Fichier :  $Id: evid_don.h,v 1.13 2017/02/28 14:50:12 devgfi Exp $      Release : $Revision: 1.13 $        Date : $Date: 2017/02/28 14:50:12 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEVID * FICHIER evid_don.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Structure des donnees du fichier de donnees internes
* de la tache VIDEO.
*
------------------------------------------------------
* HISTORIQUE :
*
* Nagiel.E	version 1.1	31 Oct 1994	: Creation
* Mismer.D	version 1.2	28 Nov 1994	: Suppression des #define XDF
* Nagiel.E	version 1.3	02 Dec 1994	:
* Nagiel.E	version 1.4	23 Dec 1994	:
* Nagiel.E	version 1.5	26 Dec 1994	:
* Volcic.F	version 1.6	24 Jan 1995	:
* Mismer.D	version 1.7	09 Avr 1995	:
* Mismer.D	version 1.8	18 Mai 1995	: Modif gestion compte rendu commande
* Fontaine.C	version 1.9 	23 Mai 1995	: Reprise entete, historique
* Mismer.D	version 1.10	18 Jan 1998	: Modif pour basculement PC2
* Mismer.D	version 1.11	29 Mai 2000	: Modif pour basculement PC2 suite et fin (DEM/65)
**  
* PNI		version 1.12	15 Mai 2008	: DEM775 dépassement de capacite du champ code caméra XDY_Mot=>int
* JPL		27/10/16 : Suppression etat PC2, LTV, magnetos; donnees de 2 Systemes video IP (DEM 1189)  1.13
------------------------------------------------------ */

#ifndef evid_don
#define evid_don

/* fichiers inclus */


#include "etim_don.h"
#include "ex_mdon.h"
#include "ex_msok.h"
#include "ex_mlcr.h"
#include "evid_cfg.h"
#include "evid_cmd.h"
#include "evid_tra.h"
#include "xzev.h"
#include "ex_mala.h"
#include "ex_malt.h"
#include "xdf.h"


/* definitions de constantes exportees */

#define EVIDC_SYSV1		1
#define EVIDC_SYSV2		2


/* Commande speciale au systeme video, distincte de celles definies au niveau des donnees communes */
#define EVIDC_DEMANDE_ETAT	101


#define EVIDC_REVEIL_1MN	60
#define EVIDC_TIMER_1MN		"VID_Timer_1mn"



/* definitions de types exportes */

typedef struct	{
		   XZEXT_IP_Config	Config;
		   XDY_Nom		Nom;
		   XDY_Eqt		Numero;
		   XZEXT_MASK		*Masque;
		}EVID_DONNEES_SYSTVIDEO;


typedef struct	{
   		   XDY_Eqt		Numero;
   		   int	                Code;
   		   XDY_Eqt		NumeroLTV;
   		   XDY_Mot		Nature;
		}EVID_CONFIG_CAMERA;
		
typedef struct	{
   		   XDY_Mot		EtatLiaison;
   		   XDY_Mot		CmdCour;
   		   XDY_Mot		NoMoni;
   		   XDY_Mot		NoPosi;
   		   XDY_Mot		NoJoy;
   		   XDY_Mot		Magneto;
   		   XDY_Mot		CRCmdCour;
		}EVID_ETAT_CAMERA;

typedef struct	{
                   EVID_CONFIG_CAMERA   Config;
                   EVID_ETAT_CAMERA     Etat;
		   int			Etat_Service;
		   void *		Suivant;
		}EVID_DONNEES_CAMERA;


//typedef struct	{
//   		   XDY_Eqt		Numero;
//   		   XDY_Mot              Code;
//   		   XDY_NomEqt		PO;
//		}EVID_CONFIG_MONITEUR;
//
//typedef struct	{
//		   EVID_CONFIG_MONITEUR Config;
//		   int			CamAssocie;
//		   int			Etat_Service;
//		   void *		Suivant;
//		}EVID_DONNEES_MONITEUR;


/* definition de macro exportees */

#define		estConfigure(va_NumSysV)	(vg_sysvid[ (va_NumSysV) ].Config.No_port > 0)

#define		estConnecte(va_NumSysV)		(vg_sysvid[ (va_NumSysV) ].Masque != NULL)


/* declarations de donnees globales */

extern EVID_DONNEES_CAMERA	*pg_debutCamera;

//extern EVID_DONNEES_MONITEUR	*pg_debutMoniteur;

extern EVID_DONNEES_SYSTVIDEO	vg_sysvid[];
extern int			vg_noSV;

extern INFO_EQT_FEN_LCR		*pg_DebutListeVIDFLcr;

extern int			vg_Mode_Fonct;
extern int			vg_SockTIM;
			

/* delaration de fonctions externes */

EX_DECLA_INSERER   ( EVID_DONNEES_CAMERA, ed_ins_liste_Camera )
EX_DECLA_SUPPRIMER ( ed_sup_liste_Camera )	

//EX_DECLA_INSERER   ( EVID_DONNEES_MONITEUR, ed_ins_liste_Moniteur )
//EX_DECLA_SUPPRIMER ( ed_sup_liste_Moniteur )	

#endif
