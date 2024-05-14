											/*E*/
/* Fichier : $Id: eimu_don.h,v 1.2 2021/05/03 13:39:43 pc2dpdy Exp $     Release : $Revision: 1.2 $      Date :  $Date: 2021/05/03 13:39:43 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEIMU * FICHIER eimu_don.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module dÅcrivant les structures de donnÅes internes
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	01/12/20	: Creation
* CGR	02/02/21	: Modifications pour OPCUA DEM-SAE155 1.2
------------------------------------------------------ */

#ifndef eimu_don
#define eimu_don

/* fichiers inclus */

#include "xdg.h"

#include "ex_mdon.h"
#include "etim_don.h"
#include <xzea.h>
#include <xzeb.h>
#include <xzamc.h>
#include <xzma.h>
#include <xzaac.h>
#include "eimu_cfg.h"
#include "eimu_cyc.h"
#include "ex_mlcr.h"

#include <endian.h>

/* definitions de constantes exportees */

#define EIMUC_REVEIL_10S	10
#define EIMUC_TIMER_10S		"IMU_Timer_10s"
#define EIMUC_REVEIL_1MN	60
#define EIMUC_TIMER_1MN		"IMU_Timer_1mn"

#define EIMUC_EQT_LIBRE		1
#define EIMUC_EQT_OCCUP		2
#define EIMUC_EQT_PANNE		3
#define EIMUC_TAG_MESSAGE_EN_COURS	"[SeeBiceTags]OTA"
#define EIMUC_TAG_ID_ZONE	"[SeeBiceTags]ZONEID"
#define EIMUC_TAG_NOM_MESSAGE	"[SeeBiceTags]PLAYLISTKEY"								
#define EIMUC_TAG_CMD		"tag_cmd"
#define EIMUC_TAG_INTERVALLE	"[SeeBiceTags]PERDIFF"
#define EIMUC_TAG_NB_CYCLES	"[SeeBiceTags]NBDIFF"
#define EIMUC_TAG_DUREE		"[SeeBiceTags]TPSDIFF"

#define EIMUC_TAG_RESET		"[SeeBiceTags]RESET"
#define EIMUC_TAG_RESETS	"[SeeBiceTags]RESETS"
#define EIMUC_TAG_ADD		"[SeeBiceTags]ADD"
#define EIMUC_TAG_START		"[SeeBiceTags]START"
#define EIMUC_TAG_STARTS	"[SeeBiceTags]STARTS"
#define EIMUC_TAG_KILL		"[SeeBiceTags]KILL"
#define EIMUC_TAG_KILLS		"[SeeBiceTags]KILLS"


#define EIMUC_STATUT_NON_COMMENCE	0
#define EIMUC_STATUT_INIT_LANCE		1
#define EIMUC_STATUT_INIT_DONE		2
#define EIMUC_STATUT_BUFFER_LANCE	3
#define EIMUC_STATUT_BUFFER_DONE	4
#define EIMUC_STATUT_DIFFU_LANCE	5
#define EIMUC_STATUT_DIFFU_DONE		6
#define EIMUC_STATUT_STOP_LANCE		7
#define EIMUC_STATUT_STOP_DONE		8
#define EIMUC_STATUT_ERREUR		9

#define EIMUC_NB_MAX_CMD_ZONE		22
#define EIMUC_NB_MAX_BICE		14

/* definitions de types exportes */


typedef struct	{
  		   	XDY_Eqt			Numero; //Numéro déquipement piloté
   		   	XDY_Mot			TypeIMU;
			XDY_Mot         	SiteGestion;
			XDY_AdresseIP		AdresseIP;
			XDY_PortIP		Port;
			XDY_Texte		EndPointOPCUA;
			XDY_Texte		UserOPCUA;
			XDY_Texte		PasswordOPCUA;
			XDY_Texte		NomBICE;
			XDY_Entier		PortMaitre;
			XDY_Entier		SousType;
			XDY_NomMachine		NomMachine;
   		   	XDY_Eqt			NumeroEqtCommunication; //Numéro de l'équipement pour la communication si un BICE pilote plusieurs équipements
		}EIMU_CONFIG_IMU;
		
		
typedef struct	{
		   EIMU_CONFIG_IMU	Config;
		   XDY_Etat_IMU		Etat;
		   XDY_Etat_IMU		EtatCmd;
		   int			Etat_Service;
  		   int			Socket;
		   XDY_Mot		StatusTR;
   		   int			NumCmd;
	           void *		Suivant;
		}EIMU_DONNEES_IMU;

typedef struct {
			XDY_Entier	NumBICE;
			XDY_Texte	NomMessage;
			XDY_Entier	NombreDiffusion;
			XDY_Entier	Cycles;
			XDY_Entier	Intervalle;
			XDY_Entier	Duree;
			XDY_Entier	Statut;
		} CMD_BICE;


/* definition de macro exportees */

/* declarations de donnees globales */

extern EIMU_DONNEES_IMU		*pg_debutIMU;
extern int			vg_Mode_Fonct;
extern XDY_NomMachine		vg_NomMachine;
extern int    			vg_SockTIM;
extern int			vg_Contexte;
char				vg_idpassw[18];
int 				vg_BICE_en_cours_pilotage;
CMD_BICE			tg_bices[EIMUC_NB_MAX_CMD_ZONE];
 
/* delaration de fonctions externes */

EX_DECLA_INSERER   ( EIMU_DONNEES_IMU, ed_ins_liste_IMU )	
EX_DECLA_SUPPRIMER ( ed_sup_liste_IMU )	

#endif
