/* Fichier :  $Id: itre_ax.c,v 1.153 2021/06/03 07:21:36 pc2dpdy Exp $      Release : $Revision: 1.153 $        Date : $Date: 2021/06/03 07:21:36 $
*************************************************
* STERIA * Projet MIGRAZUR			*
*************************************************
* MODULE  : MTRE				*
* FICHIER : itre_ax.c				*	
*************************************************
* DESCRIPTION :
* contient toutes les fonctions de traitement des 
* messages pour Applix ou envoyes par Applix
*
*************************************************
* HISTORIQUE :
*
* J.M. Guilhou	28 Sep 1994	Creation 					(v1.01)
* J.Cornu	20 Oct 1994	Ajout des XZEV pour reception Applix des
*				commandes de pilotage de l'équipement vidéo.	(v1.10)
* J.Cornu	26 Oct 1994	Ajout de itre_ax_piloterMagnetos 		(v1.14)
* J.Cornu	29 Oct 1994	Ajout des abonnements tunnels et échangeurs 	(v1.18)
* J.Cornu	09 Dec 1994	Ajout de l'abonnement NAV 
*				Ajout des co												mmandes NAV				(v1.11)
* Lecrivain     12 Dec 1994	Mise a jour lancement IHM cote MCE              (v1.19)
* Guilhou	19 dec 1994	modif lanceMce pour changement de poste		(V1.21)
* Guilhou	22 dec 1994	modif OUVREFMC 					(V1.25)
* J.Cornu	22 Dec 1994	Ajout des XZEX01 & 02                   	(v1.26)
* Guilhou	27 dec 1994	Ajout fonctions de surcharge			(v1.28)
* Lecrivain	29 dec 1994	Ajout fonctions pilotage RAU et tel. V1.37,V1.38
* Lecrivain	02 Jan 1995	Ajout fonctions pilotage Operator    V1.40,V1.43
* Guilhou	10 jan 1995	Ajout fonction fin de surcharge			(V1.55)
* Guilhou	10 jan 1995	modif fin de surcharge au CI		(V1.56)
* Lecrivain	13 Jan 1995	Ajout fonction XZCA02_03                1.62
* Guilhou	17 jan 1995	modif appel xzdd02			1.63
* Lecrivain	18 Jan 1995	Ajout fonction XZEA14                      1.65
* Volcic	20 Jan 1995	Modifs diverses et variees ...           1.66
* Cornu         24 Jan 1995     Modifs piloterMagneto et piloterCamera  (V1.69)
* Volcic	27 Jan 1995	Modifs taille string                       1.73
* Lecrivain	29 Jan 1995	Ajout fonction XZAA10                      1.74
* Lecrivain	28 Fev 1995	Test RAU (XZEA14) sur ecran textuel        1.81
* Mismer	28 Mar 1995	Modif XZET01 et XZET02		           1.83
* Lecrivain	02 Avr 1995	Arg. canal IHM pour itre_ax_processMsg et
*                               vg_socEnvToAxMce remplace vg_socEnvToAx    1.84
* Lecrivain	04 Avr 1995	Ajout ITRE_AX_MSG_ADM et XZEL02_03         1.85
* Mismer	18 Mai 1995	Modif XZEA22, 23, 24		           1.88
* Gaborit	18 Mai 1995	Ajout Trace dans itre_ax_processMsg 		1.89
* Gaborit       19 Mai 1995     Ajout Trace dans itre_ax_processMsg (suite)	1.90
* Niepceron     06 Jul 1995     Ajout du son des reveils ou alerte	v1.92
* Volcic        17 Jul 1995     Modif XZEP01 Arret PMV			v1.93
* Lecrivain	28 Jul 1995	         				1.94
* Mismer	07 Aug 1995	Ajout Timeout demande LCR	        1.95
* Guilhou       13 Sep 1995	Correction equa alerte sur evt v1.97
* Guilhou	19 Sep 1995	Ajout demande de donnees individuelles v1.98
* Mismer	21 Sep 1995	Modif appel XZEN03	        	1.100
* Niepceron	 2 Oct 1995	Filtrage ds itre_ax_processMsg des Msg operateur et Administrateteur 1.101
* Mismer	 2 Nov 1995	Correction argument appel XZEN03 	1.102
* Torregrossa	17 Jan 1996	Envoi des messages systeme par sendpoke 1.103
* Noel		07 Fev 1996	Execution des arret relance synoptique	(DEM807) 1.104
* Guilhou	22 aut 1996	Ajout ITRE_AX_FIN_POSTE  (RADT) 	1.105
* Mismer.D      29 Aou 1996     Ajout ligne direction pour PMV 4 lignes (DEM/1162) 1.106
* Niepceron	26 Sep 1996	Ajout ITRE_AX_ACQT (DEM/424) 1.107 1.108
* Mismer.D      25 Mar 1997     Ajout affichage TDP  (DEM/1417) 	1.110
* Mismer.D      24 Avr 1997     Ajout commande magneto EQT2 (DEM/1427) 	1.111
* Guilhou	04 sep 1997	gestion icones de commm sur tous les sites 1.112
* Guilhou	19 jan 1998	ajout ITRE_AX_INITPRES 1.112
* Mismer.D      25 Jan 1998     Ajout affichage PAL  (DEM/1417) 	1.114
* Mismer.D      25 Mai 1998     Ajout antenne pour test RAU  (DEM/) 	1.115
* Niepceron     27 Mai 1998	Ajout envoi etat comm a gauche (DEM/1673) 1.116
* Niepceron     21 Dec 1998	Limitation des envoi INITPRES 1.117
* Niepceron     20 Nov 1998	Ajout de ITRE_AX_DEMCAT et EXPCAT (DEM/1723) 1.118
* cluchague     20 sep 2002	Ajout de XZET03 (98) 1.119 (REPERE1) Attention : ligne 1715 la fonction 
				XZER02 pose probleme et a ete mise en commentaire ainsi que XZEA26 XZSS04
				XZEL02
* Mismer.D      28 avr 2004      Modif pour PAL 	1.120
* JMG		28/09/05	ajout PMVA BAD BAF 	1.121
* JMG		31/10/07	ajout machine dans message XDM_A_PRES_OP 	1.122
*				traitement ITRE_AX_XZEU10
* JBL			Nov 2007 : DEM 665
* JPL		06/05/09	Fonction de trace Applix: trace du message complet 1.125
* JPL		21/10/09	Supression bug dans la fonction d'emission de Poke 1.126
* JPL		22/10/09	Changement de format de la fonction d'emission de commande NEWTASK 1.127
* JPL		13/11/09	Interface Applix (MTIR) -> C : choix du separateur de champs 1.128
* JMG		26/06/10 	portage linux DEM/934 1.129
* JMG		21/12/10	correction XZIS26 DEM/934 1.130
* JPL		23/12/11	Ajout support de fonctions d'activation/desactivation timer TdP (DEM 1007)  1.131
* LCL		31/01/12	Ajout TdP etendus (DEM/1014) 1.132
* SDU       30/01/12    traitement des alertes communautaires (DEM 1015) 1.132
* VR		29/05/12 	regumlation DEM 1016
* MG		16/04/12	rajout du MTAR_FCT_xzav03 demarrer simulation DEM 1016
* MG		11/06/12	rajout ITRE_AX_XZEP04 et ITRE_AX_XZEZ04 (DEM/1014 PMA)
* PNI		15/10/12	correction filtre strtok pour cmde unitaites GTC 
* PNI		15/10/12	traitement des presences ope pour VH � VD dem1055 1.136
* PNI		16/10/12	correction filtre strtok pour cmde unitaites GTC 1.137
* PNI		22/10/12	traitement des presences ope pour VH  � VD dem1055 1.138
* PNI		04/09/13 :	traitement des presences ope pour VH  � CA dem1079 1.139
* PNI		20/11/13 :	remise en place du support de fonctions d'activation/desactivation timer TdP (DEM 1007) 1.140
* JMG		20/10/14 : 	ajout ITRE_AX_FACTIS 1.141
* JMG		26/04/16 : ajout PMVA SAGA 1.143 DEM1191
* JMG		20/06/17 : tigmot 1.144
* JMG		29/10/18 : ajout RMVI et AVA 1.146
* LCL		06/12/18 : tag DEM1315 RMVI 1.147
* JMG		12/06/19 : ajout INFOMESSAGE ODI_C DEM1338 1.148
* JMG		12/11/19 : TDFP surcharge DEM1365 1.149
* PNI   13/05/20 : Augmentation taille vl_tableau_* pour bug de limitation de nb de commande unitaire eqt GTC 1.150 SAE-113
* JMG		22/05/20 : EMS 1.151
* GR            15/07/20 : EMS 
* CGR		29/01/21 : Ajout IMU DEM-SAE155 1.152
* CGR		02/04/21 : Ajout SONO PAU DEM-SAE244 1.153
* LCL		20/12/21	: MOVIS P2 ems SAE-312
* LCL		21/01/22 	: MAGISTRA SAE-311
* LCL		07/02/2023 	: MAGISTRA SAE-322 Correction itre_ax_Abt_Etat_Equipement pour format message Poke vers titre
* LCL		27/09/23 	: Adaptations CNA DEM-483	
* LCL		08/12/23	: MAGISTRA-SAE-557 Correction type de variables
* GGY		21/11/23 	: ajout ITRE_AX_XZED03 pour demande d'enregistrement video DAI (DEM483)
* GGY		24/11/23	: ajout appel XZEA71_Abt_Etat_PIC dans itre_ax_processExec (DEM483)
******************************************************************************/

#include "itre_ax.h"
#include "itre_son.h"
#include "itre_rtw.h"
#include "xzaa.h"
#include "xzav03.h"
#include "xzea.h"
#include "xzev.h"
#include "xzet.h"
#include "xzen.h"
#include "xzeb.h"
#include "xzep.h"
#include "xzex.h"
#include "xzez.h"
#include "xzsm.h"
#include "xzsp.h"
#include "xzis.h"
#include "xzcg.h"


/* definition des variables locales au module */
static char *version = "$Id: itre_ax.c,v 1.153 2021/06/03 07:21:36 pc2dpdy Exp $ : module" ;
static int vm_mceLancee=1;
static void itre_ax_piloterContraste();
static void itre_ax_Etat_Comm();
static void itre_ax_Presence_PCS();
static void itre_ax_Abt_Etat_Eqt_SAGA(XZEAT_Etat_Eqt_SAGA ,int *, char *);

#define CMD_LEN 		(50*200) /* Taille du buffer qui contient les etats des eqt GTC renovee non envoyes */
#define MAX_PACKETS		130 /* Nb max de trames contenues dans le buffer temporaire des etats des eqt GTC renovee */
static short vg_mode_rafale = XDC_NON; /* Flag qui contient l'etat du mode rafale precedent */
static char vg_cmd[CMD_LEN] = ""; /* buffer qui contient les trames non envoyees des etats des eqt GTC renovee */
static int vg_nb_packets = 0; /* nombre de trames en cours */
extern int vg_titre;

XDY_NomMachine vg_nomMachineLCR;

/*************************************************
* MODULE  : MTRE				*
*************************************************
* DESCRIPTION :
* traite les messages en provenance du RT server
*************************************************
* SEQUENCE D'APPEL :
*/
  
extern int itre_ax_processMsg(
T_IPC_CONN			va_cnx,
T_IPC_CONN_PROCESS_CB_DATA	pa_data,
T_CB_ARG			pa_arg,
int				va_canal_IHM
)

/*
* PARAMETRE EN ENTREE :
*
* arg1		Description
* arg2		..........
*
* PARAMETRE EN SORTIE :
*
* VALEUR DE RETOUR :
*
* CONDITION D'ERREUR :
*
* CONDITION LIMITES :
************************************************/
{

/*#ifndef _TIBCO_EMS
  T_INT4 vl_commande = 0;

  T_INT4 vl_numPoke = 0;
  T_INT4 vl_num = 0;
  T_INT4 vl_mode = 0;

  T_STR tl_arg1 = "";
  T_STR tl_arg2 = "";
*/
  XDY_Entier vl_commande = 0;

  XDY_Entier vl_numPoke = 0;
  XDY_Entier vl_num = 0;
  XDY_Entier vl_mode = 0;

  T_STR tl_arg1 = "";
  T_STR tl_arg2 = "";
/*
#endif
char *buf[255];
*/
  char *tl_params[10];

  XDY_NomMachine vl_machine="";

  static char *version = "$Id: itre_ax.c,v 1.153 2021/06/03 07:21:36 pc2dpdy Exp $ : itre_ax_processMsg" ;
  /*lecture du message */
  TipcMsgNextInt4(pa_data->msg,&vl_commande);
	XZST_03EcritureTrace(XZSTC_WARNING,"itre_ax_processMsg commande %d",vl_commande);

  if (XZSC_GetDestinataire(pa_data, vl_machine)!=XDC_OK) {
	XZST_03EcritureTrace(XZSTC_WARNING,"itre_ax_processMsg : probleme dans recuperation machine destinataire du message");
  }

  /*suivant la commande recue*/
  switch(vl_commande)
  {
    /*commande JOIN_DG*/
    case (ITRE_AX_JOINDG):
      /*lecture des arguments*/
      TipcMsgNextStr(pa_data->msg,&tl_arg1);
#ifdef _TIBCO_EMS
//	XZEMS_03DestroyMsg(pa_data->msg);
#endif
	XZST_03EcritureTrace(XZSTC_MESSAGE,"%s : JOIN_DG %s",version,tl_arg1);
      /*execution de la commande*/
      XDG_JoindreDG(tl_arg1);
      TipcSrvFlush();
      break;
      
    /*commande SEND_POKE*/
    case (ITRE_AX_SENDPOKE):
      /*lecture des arguments*/
      TipcMsgNextInt4(pa_data->msg,&vl_numPoke);
      TipcMsgNextStr(pa_data->msg,&tl_arg1); /*arguments*/
#ifdef _TIBCO_EMS
//	XZEMS_03DestroyMsg(pa_data->msg);
#endif

      XZST_03EcritureTrace(XZSTC_WARNING,"tre_ax_processMsg : poke %d msg %s", vl_numPoke,tl_arg1);
      XZST_03EcritureTrace(XZSTC_MESSAGE,"%s : poke %d msg %s", version,vl_numPoke,tl_arg1);
      /*Test de la validite du texte a afficher ds la fenetre Avertiss. Operateur */
      if ( (( vl_numPoke == COM_CANAL_MTAV )  ||
	    ( vl_numPoke == COM_CANAL_MTAA )) &&
           ( *tl_arg1 == '\0' ) )
         {
         XZST_03EcritureTrace(XZSTC_WARNING,"Envoie a COM_CANAL_MTAx dangereux:%s : poke %d msg %s; pas d'envoi", version,vl_numPoke,tl_arg1);
         return (XDC_OK);
         }
      else 
         {
         /*envoi du poke a Applix*/
         tl_params[0]=tl_arg1;
      XZST_03EcritureTrace(XZSTC_WARNING,"tre_ax_processMsg : tl_params[0]=[%s]", tl_params[0]);
         if (itre_com_SendPoke(va_canal_IHM,vl_numPoke,tl_params,1,vl_machine)!=XDC_OK)
         {
            return (XDC_NOK);
         }
	 if (vl_numPoke == COM_CANAL_MTMT_ALM) {
		itre_com_SendPoke(vg_socEnvToAxDial,vl_numPoke,tl_params,1,vl_machine);
	 }
         }
      break;
 
    /*commande newtask*/
    case (ITRE_AX_NEWTASK):
      /*lecture des arguments*/
      TipcMsgNextStr(pa_data->msg,&tl_arg1); /*nom macro*/
      TipcMsgNextStr(pa_data->msg,&tl_arg2); /*arguments*/
      TipcMsgNextInt4(pa_data->msg,&vl_mode); /*mode synchrone ou non*/
#ifdef _TIBCO_EMS
//	XZEMS_03DestroyMsg(pa_data->msg);
#endif

	XZST_03EcritureTrace(XZSTC_MESSAGE,"%s : newtask %s, arg %s, mode %d",version,tl_arg1,tl_arg2,vl_mode);
      /*Test de la validite des messages a envoyer : message systeme refuse */
      if ( strcmp( tl_arg1 , "ITMA_TAA_Message_Adm") == NULL) 
         {
         XZST_03EcritureTrace(XZSTC_WARNING,"Envoi refuse des messages systeme par newtask a %s : arg %s; pas d'envoi",tl_arg1,tl_arg2);
         return (XDC_OK);
         }
      else 
         {
         /*envoie la commande*/
         if (itre_com_NewTask(va_canal_IHM,tl_arg1,tl_arg2,1,vl_mode,(char *)0,(int *)0,vl_machine)!=XDC_OK)
         {
	   return (XDC_NOK);
         }
         }

      break;

    /*commande des sons */
    case (ITRE_AX_SONS):
      /*lecture des arguments*/
      TipcMsgNextStr(pa_data->msg,&tl_arg1); /*no du son*/
      TipcMsgNextStr(pa_data->msg,&tl_arg2); /*arguments*/
      TipcMsgNextInt4(pa_data->msg,&vl_mode); /*mode synchrone ou non*/
#ifdef _TIBCO_EMS
//	XZEMS_03DestroyMsg(pa_data->msg);
#endif

	XZST_03EcritureTrace(XZSTC_MESSAGE,"%s : newtask sons %s, arg %s, mode %d",version,tl_arg1,tl_arg2,vl_mode);
      /*envoie la commande de son */
      if (itr_LanceSon( atoi(tl_arg1) )!=XDC_OK)
      {
	return (XDC_NOK);
      }

      break;

    /*commande SETVARCTX*/
    case (ITRE_AX_SETVARCTX):
      /*lecture des arguments*/
      TipcMsgNextStr(pa_data->msg,&tl_arg1); /*nom de la variable*/
      TipcMsgNextStr(pa_data->msg,&tl_arg2); /*valeur a assigner*/
#ifdef _TIBCO_EMS
//	XZEMS_03DestroyMsg(pa_data->msg);
#endif

	XZST_03EcritureTrace(XZSTC_MESSAGE,"%s : SETVARCTX var %s, val %s",version,tl_arg1,tl_arg2);
      /*envoie la commande*/
      if (itre_com_EcrireVarCtx(va_canal_IHM,tl_arg1,tl_arg2,ITRE_COM_TRUE,vl_machine)!=XDC_OK)
      {
	return (XDC_NOK);
      }
      break;

    /*commande SETVARCTX*/
    case (ITRE_AX_INFO_MESSAGE):
      /*lecture des arguments*/
      TipcMsgNextStr(pa_data->msg,&tl_arg1); /*nom de la variable*/
#ifdef _TIBCO_EMS
//	XZEMS_03DestroyMsg(pa_data->msg);
#endif

	XZST_03EcritureTrace(XZSTC_FONCTION,"%s :INFOMESSAGE var %s",version,tl_arg1);
      /*envoie la commande*/
      if (itre_com_InfoMessage(va_canal_IHM,tl_arg1)!=XDC_OK)
      {
	return (XDC_NOK);
      }
      break;

    /*commande GETVARCTX*/
    case (ITRE_AX_GETVARCTX):
      return (XDC_OK);
  }
  return (XDC_OK);
}






/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static void itre_ax_piloterMagnetos(XZEAT_Etat_Magneto va_magneto,int *pa_resultat, char *va_nomMachine)

/*
* ARGUMENTS EN ENTREE :
*   < Valeur et descriptif des parametres en entree >
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
* CODE RETOUR : 
*   <Valeur de retour et un commentaire explicatif >
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   < Bref descriptif fonctionnel >
*
------------------------------------------------------*/
{

  char vl_cmd[100] = "";
  char *tl_params[10];

  
  static char *version = "$Id: itre_ax.c,v 1.153 2021/06/03 07:21:36 pc2dpdy Exp $ : itre_ax_piloterMagnetos" ;
  
  XZST_03EcritureTrace(XZSTC_FONCTION,"IN -- %s", version);

  XZST_03EcritureTrace(XZSTC_FONCTION,"%d %d %d %d", va_magneto.Numero,
						     va_magneto.NumImage,
						     va_magneto.CodeCmd,
			    			     va_magneto.CrCmd);
			    			 
  /*je formate ces donnes pour Applix et je les envoie*/
  sprintf(vl_cmd,"%d,%d,%d,%d", va_magneto.Numero,
			        va_magneto.NumImage,
			        va_magneto.CodeCmd,
			        va_magneto.CrCmd);

  /*envoi du poke a Applix*/
  tl_params[0]=vl_cmd;
  if (itre_com_SendPoke(vg_socEnvToAxMce,COM_CANAL_MTCA,tl_params,1,va_nomMachine)!=XDC_OK)
  {
     XZST_03EcritureTrace(XZSTC_WARNING,"%s: erreur sur le send poke", version);
     *pa_resultat=XDC_NOK;
  }
  else
     *pa_resultat=XDC_OK;
  
  XZST_03EcritureTrace(XZSTC_FONCTION,"OUT -- %s", version);

}	



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static void itre_ax_piloterCamera(XZEAT_Etat_Camera va_Camera,int *pa_resultat, char *va_nomMachine)

/*
* ARGUMENTS EN ENTREE :
*   < Valeur et descriptif des parametres en entree >
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
* CODE RETOUR : 
*   <Valeur de retour et un commentaire explicatif >
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   < Bref descriptif fonctionnel >
*
------------------------------------------------------*/
{

  char vl_cmd[50] = "";
  char *tl_params[10];

  static char *version = "$Id: itre_ax.c,v 1.153 2021/06/03 07:21:36 pc2dpdy Exp $ : itre_ax_piloterCamera" ;
  
  XZST_03EcritureTrace(XZSTC_FONCTION,"IN -- %s", version);

  XZST_03EcritureTrace(XZSTC_FONCTION,"%s : Num = %d\tCodeCmd = %d\tCrCmd = %d\tMon = %d\tPos. = %d\tMag = %d\tEtatCom = %d", version, 
  						 va_Camera.Numero,
						 va_Camera.CodeCmd,
			    			 va_Camera.CrCmd,
			    			 va_Camera.NumMoniteur,
			    			 va_Camera.NumPosition,
			    			 va_Camera.NumMagneto,
			    			 va_Camera.EtatCom );
			    			 
  /*je formate ces donnes pour Applix et je les envoie*/
  sprintf(vl_cmd,"%d,%d,%d,%d,%d,%d,%d", va_Camera.Numero,
			                 va_Camera.CodeCmd,
			                 va_Camera.CrCmd,
			                 va_Camera.NumMoniteur,
			                 va_Camera.NumPosition,
			                 va_Camera.NumMagneto,
			                 va_Camera.EtatCom );

  /*envoi du poke a Applix*/
  tl_params[0]=vl_cmd;
  if (itre_com_SendPoke(vg_socEnvToAxMce,COM_CANAL_MTCM,tl_params,1,va_nomMachine)!=XDC_OK)
  {
     XZST_03EcritureTrace(XZSTC_WARNING,"%s: erreur sur le send poke", version);
     *pa_resultat=XDC_NOK;
  }
  else
     *pa_resultat=XDC_OK;
  
  XZST_03EcritureTrace(XZSTC_FONCTION,"OUT -- %s", version);

}	



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static void itre_ax_piloterContraste(XZEAT_Contraste va_contraste,int *pa_resultat, char *va_nomMachine)

/*
* ARGUMENTS EN ENTREE :
*   < Valeur et descriptif des parametres en entree >
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR : 
*   <Valeur de retour et un commentaire explicatif >
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   < Bref descriptif fonctionnel >
*
------------------------------------------------------*/
{
  char vl_cmd[50] = "";
  char *tl_params[10];

  static char *version = "$Id: itre_ax.c,v 1.153 2021/06/03 07:21:36 pc2dpdy Exp $ : itre_ax_piloterContraste" ;
  
  XZST_03EcritureTrace(XZSTC_FONCTION,"IN -- %s", version);

  /*exemple de decodage d'un message recu*/

  XZST_03EcritureTrace(XZSTC_FONCTION,"%d %d %d",va_contraste.TypeEqt,
				                 va_contraste.Numero,
			                         va_contraste.Contraste);
			                         
			    
  /*je formate ces donnes pour Applix et je les envoie*/
  sprintf(vl_cmd,"%d,%d,%d",va_contraste.TypeEqt,
		       	    va_contraste.Numero,
			    va_contraste.Contraste);

  /*envoi du poke a Applix*/
  tl_params[0]=vl_cmd;
  if (itre_com_SendPoke(vg_socEnvToAxMce,COM_CANAL_MTPM,tl_params,1,va_nomMachine)!=XDC_OK)
  {
     XZST_03EcritureTrace(XZSTC_WARNING,"itre_ax_piloterContraste: erreur sur le send poke");
     *pa_resultat=XDC_NOK;
  }
  else
     *pa_resultat=XDC_OK;
  
  XZST_03EcritureTrace(XZSTC_FONCTION,"OUT -- %s", version);
}	







/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static void itre_ax_Abt_Telealarmes(XZEAT_Telealarme va_etat,int *pa_resultat, char *va_nomMachine)

/*
* ARGUMENTS EN ENTREE :
*   < Valeur et descriptif des parametres en entree >
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR : 
*   <Valeur de retour et un commentaire explicatif >
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   < Bref descriptif fonctionnel >
*
------------------------------------------------------*/
{
  char vl_cmd[50] = "";
  char *tl_params[10];
  
  static char *version = "$Id: itre_ax.c,v 1.153 2021/06/03 07:21:36 pc2dpdy Exp $ : itre_ax_Abt_Telealarmes" ;

  /*trace de l'execution*/
  XZST_03EcritureTrace(XZSTC_FONCTION,"IN -- %s :%d %d %d %d %d", version, va_etat.TypeEqt,
						       va_etat.NumEqt,
			    			       va_etat.Energie,
		    			               va_etat.Local,
			    			       va_etat.Lampe);
			    
  /*je formate ces donnes pour Applix et je les envoie*/
  sprintf(vl_cmd,"%d,%d,%d,%d,%d",va_etat.TypeEqt,
		       	          va_etat.NumEqt,
			          va_etat.Energie,
			          va_etat.Local,
			          va_etat.Lampe);


/*envoi du poke a Applix*/
  tl_params[0]=vl_cmd;
  
  if (va_etat.TypeEqt==XDC_EQT_TUBE)
  {
	  if (itre_com_SendPoke(vg_socEnvToAxMce,COM_CANAL_MTTU_ALM,tl_params,1,va_nomMachine)!=XDC_OK)
	  {
	     XZST_03EcritureTrace(XZSTC_WARNING,"itre_ax_Abt_Telealarmes: erreur sur le send poke COM_CANAL_MTTU_ALM");
	     *pa_resultat=XDC_NOK;
	  }
	  else *pa_resultat=XDC_OK;
  }
  else
  {
	  if (itre_com_SendPoke(vg_socEnvToAxMce,COM_CANAL_MTEC_ALM,tl_params,1,va_nomMachine)!=XDC_OK)
	  {
	     XZST_03EcritureTrace(XZSTC_WARNING,"itre_ax_Abt_Telealarmes: erreur sur le send poke COM_CANAL_MTEC_ALM");
	     *pa_resultat=XDC_NOK;
	  }
	  else 
		*pa_resultat=XDC_OK;
  }
  XZST_03EcritureTrace(XZSTC_FONCTION,"OUT -- %s", version);   	       
  	       
}	








/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static void itre_ax_Abt_Etat_Panneaux(XZEAT_Panneau va_etat,int *pa_resultat, char *va_nomMachine)

/*
* ARGUMENTS EN ENTREE :
*   < Valeur et descriptif des parametres en entree >
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR : 
*   <Valeur de retour et un commentaire explicatif >
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   < Bref descriptif fonctionnel >
*
------------------------------------------------------*/
{
  char vl_cmd[50] = "";
  char *tl_params[10];

  /*trace de l'execution*/
  static char *version = "$Id: itre_ax.c,v 1.153 2021/06/03 07:21:36 pc2dpdy Exp $ : itre_ax_Abt_Etat_Panneaux" ;

  XZST_03EcritureTrace(XZSTC_FONCTION,"IN -- %s : %d %d %d %d %d", version, va_etat.TypeEqt,
						       va_etat.NumEqt,
			    			       va_etat.NumPanneau,
		    			               va_etat.EtatPanneau,
			    			       va_etat.EtatDisc);
			    
/*je formate ces donnes pour Applix et je les envoie*/
  sprintf(vl_cmd,"%d,%d,%d,%d,%d",va_etat.TypeEqt,
		       	          va_etat.NumEqt,
			          va_etat.NumPanneau,
			          va_etat.EtatPanneau,
			          va_etat.EtatDisc);

  /*envoi du poke a Applix*/
  tl_params[0]=vl_cmd;
  
  if (itre_com_SendPoke(vg_socEnvToAxMce,COM_CANAL_MTTU_PAN,tl_params,1,va_nomMachine)!=XDC_OK)
  {
     XZST_03EcritureTrace(XZSTC_WARNING,"itre_ax_Abt_Telealarmes: erreur sur le send poke COM_CANAL_MTTU_PAN");
     *pa_resultat=XDC_NOK;
  }
  else *pa_resultat=XDC_OK;
  
  if (itre_com_SendPoke(vg_socEnvToAxMce,COM_CANAL_MTEC_PAN,tl_params,1,va_nomMachine)!=XDC_OK)
  {
     XZST_03EcritureTrace(XZSTC_WARNING,"itre_ax_Abt_Telealarmes: erreur sur le send poke COM_CANAL_MTEC_PAN");
     *pa_resultat=XDC_NOK;
  }
  else *pa_resultat=XDC_OK;
  
  XZST_03EcritureTrace(XZSTC_FONCTION,"OUT -- %s", version); 
}	


static void itre_ax_Abt_Etat_Domaine(XZEAT_Domaine va_etat,int *pa_resultat, char *va_nomMachine)

/*
* ARGUMENTS EN ENTREE :
*   < Valeur et descriptif des parametres en entree >
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR : 
*   <Valeur de retour et un commentaire explicatif >
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   < Bref descriptif fonctionnel >
*
------------------------------------------------------*/
{
  char vl_cmd[50] = "";
  char *tl_params[10];
  int vl_TypeEtat = 0;
  

  /*trace de l'execution*/
  static char *version = "$Id: itre_ax.c,v 1.153 2021/06/03 07:21:36 pc2dpdy Exp $ : itre_ax_Abt_Etat_Domaine" ;

  XZST_03EcritureTrace(XZSTC_FONCTION,"IN -- %s : %d %d %d %d %d", version, 
							va_etat.TypeOuvrage,
						    va_etat.IdOuvrage,
						    va_etat.Domaine,
						    va_etat.Mode,
						    va_etat.IdScenario);
			    

	switch(va_etat.Domaine)
	{
	case(XDC_EQT_SIG):
		if (va_etat.TypeOuvrage == XDC_EQT_TUB)
		{
			vl_TypeEtat = COM_CANAL_MTTU_DOM_SIGT;
		} else if (va_etat.TypeOuvrage == XDC_EQT_ECH)
		{
			vl_TypeEtat = COM_CANAL_MTTU_DOM_SIGE;
		}
		break;
	case(XDC_EQT_VEN):
		vl_TypeEtat = COM_CANAL_MTTU_DOM_VEN;
		break;
	case(XDC_EQT_ECL):
		vl_TypeEtat = COM_CANAL_MTTU_DOM_ECL;
		break;
	case(XDC_EQT_ENR):
		vl_TypeEtat = COM_CANAL_MTTU_DOM_ENE;
		break;
	case(XDC_EQT_EXP):
		vl_TypeEtat = COM_CANAL_MTTU_DOM_EXP;
		if (va_etat.TypeOuvrage == XDC_EQT_LI)
		{
			vl_TypeEtat = COM_CANAL_MTTU_DOM_LIT;
		}
		break;
	case(XDC_EQT_LTN):
		vl_TypeEtat = COM_CANAL_MTTU_DOM_LT;
		break;
	case(XDC_EQT_SYS):
		vl_TypeEtat = COM_CANAL_MTTU_DOM_SYS;
		break;
	}
			
/*je formate ces donnes pour Applix et je les envoie*/
  sprintf(vl_cmd,"%d,%d,%d,%d",
			va_etat.TypeOuvrage,
			va_etat.IdOuvrage,
			va_etat.Mode,
			va_etat.IdScenario);
  XZST_03EcritureTrace(XZSTC_INFO,"itre_ax_Abt_Etat_Domaine: envoi de la trame %s vers Applix", vl_cmd);

  /*envoi du poke a Applix*/
  tl_params[0]=vl_cmd;
  
  if (itre_com_SendPoke(vg_socEnvToAxMce,vl_TypeEtat,tl_params,1,va_nomMachine)!=XDC_OK)
  {
     XZST_03EcritureTrace(XZSTC_WARNING,"itre_ax_Abt_Etat_Domaine: erreur sur le send poke COM_CANAL_MTTU_PAN");
     *pa_resultat=XDC_NOK;
  }
  else *pa_resultat=XDC_OK;
  XZST_03EcritureTrace(XZSTC_FONCTION,"OUT -- %s", version); 
}	

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static void itre_ax_Abt_Etat_Equipement(XZEAT_Equipement va_etat,int *pa_resultat, char *va_nomMachine)

/*
* ARGUMENTS EN ENTREE :
*   < Valeur et descriptif des parametres en entree >
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR : 
*   <Valeur de retour et un commentaire explicatif >
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   < Bref descriptif fonctionnel >
*
------------------------------------------------------*/
{
//  char vl_cmd[50] = "";
  char *vl_cmd;
  char *tl_params[10];
  int type_etat = 0;
  int type_ouvrage = 0;
  int id_ouvrage = 0;
  int id_equipement = 0;
  int etat_fonctionnel = 0;
  int etat_technique = 0;
  int type_equipement = 0;
  int mode = 0;
  int etat_disc = 0;
  short mode_rafale = XDC_NON;
  short envoi_possible = XDC_OUI;
  char lib_trace[50];

  /*trace de l'execution*/
  static char *version = "$Id: itre_ax.c,v 1.153 2021/06/03 07:21:36 pc2dpdy Exp $ : itre_ax_Abt_Etat_Equipement" ;

  XZST_03EcritureTrace(XZSTC_FONCTION,"IN -- %s : %d %d %d %d %d %d %d %d %d %d", version, 
								va_etat.Domaine,
								va_etat.TypeEqt,
								va_etat.NumEqt,
			    			    va_etat.TypeEquipement,
			    			    va_etat.NumEquipement,
		    			        va_etat.EtatEquipement,
								va_etat.EtatTechnique,
								va_etat.Mode,
			    			    va_etat.EtatDisc,
								va_etat.ModeRafale);
			    
/*je formate ces donnes pour Applix et je les envoie*/
  type_ouvrage = va_etat.TypeEqt;
  id_ouvrage = va_etat.NumEqt;
  id_equipement = va_etat.NumEquipement;
  etat_fonctionnel = va_etat.EtatEquipement;
  etat_technique = va_etat.EtatTechnique;
  type_equipement = va_etat.TypeEquipement;
  mode = va_etat.Mode;
  mode_rafale = va_etat.ModeRafale;
  etat_disc = va_etat.EtatDisc;
  
  switch(va_etat.Domaine)
  {
	case(XDC_EQT_SIG):
		if (type_ouvrage == XDC_EQT_TUBE) 
		{
			type_etat = COM_CANAL_MTTU_PAN;
		} 
		else if (type_ouvrage == XDC_EQT_ECHANGEUR) 
		{
			type_etat = COM_CANAL_MTEC_PAN;
		}
		break;
	case(XDC_EQT_ECL):
		if (type_ouvrage == XDC_EQT_TUBE) 
		{
			type_etat = COM_CANAL_MTTU_EQP_ECL;
		} 
		break;
	case(XDC_EQT_VEN):
		if (type_ouvrage == XDC_EQT_TUBE) 
		{
			type_etat = COM_CANAL_MTTU_EQP_VEN;
		} 
		break;
	case(XDC_EQT_ENR):
		type_etat = COM_CANAL_MTTU_EQP_ENE;
		break;
	case(XDC_EQT_INC):
		break;
	case(XDC_EQT_EXP):
		if (type_ouvrage == XDC_EQT_LI || type_equipement == XDC_EQT_LIT) 
		{
			type_etat = COM_CANAL_MTTU_EQP_LIT;
		} 
		else 
		{
			type_etat = COM_CANAL_MTTU_EQP_EXP;
		}
		break;
	case(XDC_EQT_SYS):
		type_etat = COM_CANAL_MTTU_EQP_SYS;
		break;
	case(XDC_EQT_LTN):
		type_etat = COM_CANAL_MTTU_EQP_LT;
		break;
  }

	XZST_03EcritureTrace(XZSTC_INFO,"Ancien mode rafale : %s, nouveau mode rafale : %s", 
						vg_mode_rafale == XDC_OUI ? "OUI" : "NON",
						mode_rafale == XDC_OUI ? "OUI" : "NON");
						
	if (vg_mode_rafale == XDC_NON && mode_rafale == XDC_OUI)
	{
		memset(vg_cmd, 0, CMD_LEN);
		vl_cmd = vg_cmd;
		vg_nb_packets = 1;
		envoi_possible = XDC_NON;
	}
	else if (vg_mode_rafale == XDC_NON && mode_rafale == XDC_NON)
	{
		memset(vg_cmd, 0, CMD_LEN);
		vg_nb_packets = 1;
		vl_cmd = vg_cmd;
		envoi_possible = XDC_OUI;
	}
	else if (vg_mode_rafale == XDC_OUI && mode_rafale == XDC_OUI)
	{
		vg_cmd[strlen(vg_cmd)] = '|';
		vl_cmd = vg_cmd + strlen(vg_cmd);
		vg_nb_packets++;
		envoi_possible = XDC_NON;
	}
	else if (vg_mode_rafale == XDC_OUI && mode_rafale == XDC_NON)
	{
		vg_cmd[strlen(vg_cmd)] = '|';
		vl_cmd = vg_cmd + strlen(vg_cmd);
		vg_nb_packets++;
	}
	vg_mode_rafale = mode_rafale;

	if (vg_nb_packets >= MAX_PACKETS)
	{
		envoi_possible = XDC_OUI;
		vg_mode_rafale = XDC_NON;
	}
	
	switch(type_etat)
	{
		case(COM_CANAL_MTEC_PAN):
			strcpy(lib_trace,"COM_CANAL_MTEC_PAN");
			sprintf(vl_cmd,"%d,%d,%d,%d,%d,%d,%d",
						type_ouvrage,
						id_ouvrage,
						type_equipement,
						id_equipement,
						etat_fonctionnel,
						etat_technique,
						mode);
			break;
		case(COM_CANAL_MTTU_PAN):
			strcpy(lib_trace,"COM_CANAL_MTTU_PAN");
			sprintf(vl_cmd,"%d,%d,%d,%d,%d,%d,%d",
						type_ouvrage,
						id_ouvrage,
						type_equipement,
						id_equipement,
						etat_fonctionnel,
						etat_technique,
						mode);
			break;
		case(COM_CANAL_MTTU_EQP_VEN):
			strcpy(lib_trace,"COM_CANAL_MTTU_EQP_VEN");
			sprintf(vl_cmd,"%d,%d,%d,%d,%d,%d,%d",
						type_ouvrage,
						id_ouvrage,
						type_equipement,
						id_equipement,
						etat_fonctionnel,
						etat_technique,
						mode);
			break;
		case(COM_CANAL_MTTU_EQP_ECL):
			strcpy(lib_trace,"COM_CANAL_MTTU_EQP_ECL");
			sprintf(vl_cmd,"%d,%d,%d,%d,%d,%d,%d",
						type_ouvrage,
						id_ouvrage,
						type_equipement,
						id_equipement,
						etat_fonctionnel,
						etat_technique,
						mode);
			break;
		case(COM_CANAL_MTTU_EQP_ENE):
			strcpy(lib_trace,"COM_CANAL_MTTU_EQP_ENE");
			sprintf(vl_cmd,"%d,%d,%d,%d,%d,%d,%d",
						type_ouvrage,
						id_ouvrage,
						type_equipement,
						id_equipement,
						etat_fonctionnel,
						etat_technique,
						mode);
			break;
		case(COM_CANAL_MTTU_EQP_EXP):
			strcpy(lib_trace,"COM_CANAL_MTTU_EQP_EXP");
			sprintf(vl_cmd,"%d,%d,%d,%d,%d,%d,%d",
						type_ouvrage,
						id_ouvrage,
						type_equipement,
						id_equipement,
						etat_fonctionnel,
						etat_technique,
						mode);
			break;
		case(COM_CANAL_MTTU_EQP_LIT):
			strcpy(lib_trace,"COM_CANAL_MTTU_EQP_LIT");
			sprintf(vl_cmd,"%d,%d,%d,%d,%d,%d,%d",
						type_ouvrage,
						id_ouvrage,
						type_equipement,
						id_equipement,
						etat_fonctionnel,
						etat_technique,
						mode);
			break;
		case(COM_CANAL_MTTU_EQP_LT):
			strcpy(lib_trace,"COM_CANAL_MTTU_EQP_LT");
			sprintf(vl_cmd,"%d,%d,%d,%d,%d,%d,%d",
						type_ouvrage,
						id_ouvrage,
						type_equipement,
						id_equipement,
						etat_fonctionnel,
						etat_technique,
						mode);
			break;
		case(COM_CANAL_MTTU_EQP_SYS):
			strcpy(lib_trace,"COM_CANAL_MTTU_EQP_SYS");
			sprintf(vl_cmd,"%d,%d,%d,%d,%d,%d,%d",
						type_ouvrage,
						id_ouvrage,
						type_equipement,
						id_equipement,
						etat_fonctionnel,
						etat_technique,
						mode);
			break;
		default:
			strcpy(lib_trace,"CANAL NON IDENTIFIE");
			sprintf(vl_cmd,"%d,%d,%d,%d,%d,%d,%d",
						type_ouvrage,
						id_ouvrage,
						type_equipement,
						id_equipement,
						etat_fonctionnel,
						etat_technique,
						mode);
			break;
	}
	if (envoi_possible == XDC_NON)
	{
		*pa_resultat=XDC_OK;
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_INFO,"Trame envoyee a Applix : %d - %s", type_etat, vg_cmd);
		/*envoi du poke a Applix*/
		tl_params[0]=vg_cmd;
	  
		if (itre_com_SendPoke(vg_socEnvToAxMce,type_etat,tl_params,1,va_nomMachine)!=XDC_OK)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"itre_ax_Abt_Telealarmes: erreur sur le send poke %s", lib_trace);
			*pa_resultat=XDC_NOK;
		}
		else 
		{
			*pa_resultat=XDC_OK;
		}
	}
  
  XZST_03EcritureTrace(XZSTC_FONCTION,"OUT -- %s", version); 
}	



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static void itre_ax_Abt_Etat_Seq_Ech(XZEAT_Sequence va_etat,int *pa_resultat, char *va_nomMachine)

/*
* ARGUMENTS EN ENTREE :
*   < Valeur et descriptif des parametres en entree >
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR : 
*   <Valeur de retour et un commentaire explicatif >
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   < Bref descriptif fonctionnel >
*
------------------------------------------------------*/
{
  char vl_cmd[50] = "";
  char *tl_params[10];
  
  
  static char *version = "$Id: itre_ax.c,v 1.153 2021/06/03 07:21:36 pc2dpdy Exp $ : itre_ax_Abt_Etat_Seq_Ech" ;

  /*trace de l'execution*/
  XZST_03EcritureTrace(XZSTC_FONCTION,"IN -- %s : %d %d %d %d", version, va_etat.TypeEqt,
						       va_etat.NumEqt,
			    			       va_etat.SeqChange,
		    			               va_etat.EtatSeq);
			    
/*je formate ces donnes pour Applix et je les envoie*/
  sprintf(vl_cmd,"%d,%d,%d,%d",va_etat.TypeEqt,
		       	          va_etat.NumEqt,
			          va_etat.SeqChange,
			          va_etat.EtatSeq);

  /*envoi du poke a Applix*/
  tl_params[0]=vl_cmd;
  
  if (itre_com_SendPoke(vg_socEnvToAxMce,COM_CANAL_MTEC_SEQ,tl_params,1,va_nomMachine)!=XDC_OK)
  {
     XZST_03EcritureTrace(XZSTC_WARNING,"itre_ax_Abt_Telealarmes: erreur sur le send poke COM_CANAL_MTEC_SEQ");
     *pa_resultat=XDC_NOK;
  }
  else *pa_resultat=XDC_OK;

  XZST_03EcritureTrace(XZSTC_FONCTION,"OUT -- %s",version);
}	








/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static void itre_ax_Abt_Etat_Seq_Tun(XZEAT_Sequence va_etat,int *pa_resultat, char *va_nomMachine)

/*
* ARGUMENTS EN ENTREE :
*   < Valeur et descriptif des parametres en entree >
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR : 
*   <Valeur de retour et un commentaire explicatif >
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   < Bref descriptif fonctionnel >
*
------------------------------------------------------*/
{
  char vl_cmd[50];
  char *tl_params[10];

  static char *version = "$Id: itre_ax.c,v 1.153 2021/06/03 07:21:36 pc2dpdy Exp $ : itre_ax_Abt_Etat_Seq_Tun" ;

  /*trace de l'execution*/
  XZST_03EcritureTrace(XZSTC_FONCTION,"IN -- %s :%d %d %d %d", version, va_etat.TypeEqt,
						       va_etat.NumEqt,
			    			       va_etat.SeqChange,
		    			               va_etat.EtatSeq);
			    
/*je formate ces donnes pour Applix et je les envoie*/
  sprintf(vl_cmd,"%d,%d,%d,%d",va_etat.TypeEqt,
		       	          va_etat.NumEqt,
			          va_etat.SeqChange,
			          va_etat.EtatSeq);
  
  /*envoi du poke a Applix*/
  tl_params[0]=vl_cmd;
  
  if (itre_com_SendPoke(vg_socEnvToAxMce,COM_CANAL_MTTU_SEQ,tl_params,1,va_nomMachine)!=XDC_OK)
  {
     XZST_03EcritureTrace(XZSTC_WARNING,"itre_ax_Abt_Telealarmes: erreur sur le send poke COM_CANAL_MTTU_SEQ");
     *pa_resultat=XDC_NOK;
  }
  else *pa_resultat=XDC_OK;

  XZST_03EcritureTrace(XZSTC_FONCTION,"OUT -- %s", version);   
}	



/*X*/
/* GTC Renovee */
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static void itre_ax_Abt_Etat_Seq_SIG(XZEAT_Sequence va_etat,int *pa_resultat, char *va_nomMachine)

/*
* ARGUMENTS EN ENTREE :
*   < Valeur et descriptif des parametres en entree >
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR : 
*   <Valeur de retour et un commentaire explicatif >
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   < Bref descriptif fonctionnel >
*
------------------------------------------------------*/
{
  char vl_cmd[50];
  char *tl_params[10];
  int type_etat = 0;
  int type_ouvrage = 0;
  int id_ouvrage = 0;
  int id_sequence = 0;
  int disponibilite = 0;
  
  static char *version = "$Id: itre_ax.c,v 1.153 2021/06/03 07:21:36 pc2dpdy Exp $ : itre_ax_Abt_Etat_Seq_SIG" ;
  XZST_03EcritureTrace(XZSTC_FONCTION, "Entree dans itre_ax_Abt_Etat_Seq_SIG ");
  
  /*trace de l'execution*/
  XZST_03EcritureTrace(XZSTC_FONCTION,"IN -- %s :%d %d %d %d", version, va_etat.TypeEqt,
						       va_etat.NumEqt,
			    			   va_etat.SeqChange,
		    			       va_etat.EtatSeq);
			    
/*je formate ces donnes pour Applix et je les envoie*/

	type_ouvrage = va_etat.TypeEqt;
	id_ouvrage = va_etat.NumEqt;
	id_sequence = va_etat.SeqChange;
	disponibilite = va_etat.EtatSeq;
	
	if (type_ouvrage == XDC_EQT_TUBE) 
	{
		type_etat = COM_CANAL_MTTU_SEQ;
	} 
	else 
	{
		type_etat = COM_CANAL_MTEC_SEQ;
	}
	
	
	sprintf(vl_cmd,"%d,%d,%d,%d",
			type_ouvrage,
			id_ouvrage,
			id_sequence,
			disponibilite);
	XZST_03EcritureTrace(XZSTC_FONCTION,"Message envoye a Applix : %s", vl_cmd);
			
  /*envoi du poke a Applix*/
  tl_params[0]=vl_cmd;
  
  if (itre_com_SendPoke(vg_socEnvToAxMce,type_etat,tl_params,1,va_nomMachine)!=XDC_OK)
  {
     XZST_03EcritureTrace(XZSTC_WARNING,"itre_ax_Abt_Etat_Seq_SIG: erreur sur le send poke COM_CANAL_MTTU_SEQ");
     *pa_resultat=XDC_NOK;
  }
  else *pa_resultat=XDC_OK;

  XZST_03EcritureTrace(XZSTC_FONCTION,"OUT -- %s", version);   
}	


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static void itre_ax_Abt_Etat_Seq_VEN(XZEAT_Sequence va_etat,int *pa_resultat, char *va_nomMachine)

/*
* ARGUMENTS EN ENTREE :
*   < Valeur et descriptif des parametres en entree >
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR : 
*   <Valeur de retour et un commentaire explicatif >
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   < Bref descriptif fonctionnel >
*
------------------------------------------------------*/
{
  char vl_cmd[50] = "";
  char *tl_params[10];
  int type_etat = 0;
  int type_ouvrage = 0;
  int id_ouvrage = 0;
  int id_sequence = 0;
  int disponibilite = 0;
  
  
  static char *version = "$Id: itre_ax.c,v 1.153 2021/06/03 07:21:36 pc2dpdy Exp $ : itre_ax_Abt_Etat_Seq_VEN" ;

  /*trace de l'execution*/
  XZST_03EcritureTrace(XZSTC_FONCTION,"IN -- %s : %d %d %d %d", version, va_etat.TypeEqt,
						       va_etat.NumEqt,
			    			       va_etat.SeqChange,
		    			               va_etat.EtatSeq);
			    
/*je formate ces donnes pour Applix et je les envoie*/
	type_ouvrage = va_etat.TypeEqt;
	id_ouvrage = va_etat.NumEqt;
	id_sequence = va_etat.SeqChange;
	disponibilite = va_etat.EtatSeq;
	type_etat = COM_CANAL_MTTU_SEQ_VEN;
	sprintf(vl_cmd,"%d,%d,%d,%d",
			type_ouvrage,
			id_ouvrage,
			id_sequence,
			disponibilite);
	XZST_03EcritureTrace(XZSTC_FONCTION,"Message envoye a Applix : %s", vl_cmd);

  /*envoi du poke a Applix*/
  tl_params[0]=vl_cmd;
  
  if (itre_com_SendPoke(vg_socEnvToAxMce,type_etat,tl_params,1,va_nomMachine)!=XDC_OK)
  {
     XZST_03EcritureTrace(XZSTC_WARNING,"itre_ax_Abt_Etat_Seq_VEN: erreur sur le send poke COM_CANAL_MTEC_SEQ");
     *pa_resultat=XDC_NOK;
  }
  else *pa_resultat=XDC_OK;

  XZST_03EcritureTrace(XZSTC_FONCTION,"OUT -- %s",version);
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static void itre_ax_Abt_Etat_Seq_ECL(XZEAT_Sequence va_etat,int *pa_resultat, char *va_nomMachine)

/*
* ARGUMENTS EN ENTREE :
*   < Valeur et descriptif des parametres en entree >
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR : 
*   <Valeur de retour et un commentaire explicatif >
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   < Bref descriptif fonctionnel >
*
------------------------------------------------------*/
{
  char vl_cmd[50] = "";
  char *tl_params[10];
  int type_etat = 0;
  int type_ouvrage = 0;
  int id_ouvrage = 0;
  int id_sequence = 0;
  int disponibilite = 0;
  
  static char *version = "$Id: itre_ax.c,v 1.153 2021/06/03 07:21:36 pc2dpdy Exp $ : itre_ax_Abt_Etat_Seq_ECL" ;

  /*trace de l'execution*/
  XZST_03EcritureTrace(XZSTC_FONCTION,"IN -- %s : %d %d %d %d", version, va_etat.TypeEqt,
						       va_etat.NumEqt,
			    			       va_etat.SeqChange,
		    			               va_etat.EtatSeq);
			    
/*je formate ces donnes pour Applix et je les envoie*/
	type_ouvrage = va_etat.TypeEqt;
	id_ouvrage = va_etat.NumEqt;
	id_sequence = va_etat.SeqChange;
	disponibilite = va_etat.EtatSeq;
	type_etat = COM_CANAL_MTTU_SEQ_ECL;
	sprintf(vl_cmd,"%d,%d,%d,%d",
			type_ouvrage,
			id_ouvrage,
			id_sequence,
			disponibilite);
	XZST_03EcritureTrace(XZSTC_FONCTION,"Message envoye a Applix : %s", vl_cmd);

  /*envoi du poke a Applix*/
  tl_params[0]=vl_cmd;
  
  if (itre_com_SendPoke(vg_socEnvToAxMce,type_etat,tl_params,1,va_nomMachine)!=XDC_OK)
  {
     XZST_03EcritureTrace(XZSTC_WARNING,"itre_ax_Abt_Etat_Seq_ECL: erreur sur le send poke COM_CANAL_MTEC_SEQ");
     *pa_resultat=XDC_NOK;
  }
  else *pa_resultat=XDC_OK;

  XZST_03EcritureTrace(XZSTC_FONCTION,"OUT -- %s",version);
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static void itre_ax_Abt_Etat_Seq_ENR(XZEAT_Sequence va_etat,int *pa_resultat, char *va_nomMachine)

/*
* ARGUMENTS EN ENTREE :
*   < Valeur et descriptif des parametres en entree >
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR : 
*   <Valeur de retour et un commentaire explicatif >
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   < Bref descriptif fonctionnel >
*
------------------------------------------------------*/
{
  char vl_cmd[50] = "";
  char *tl_params[10];
  int type_etat = 0;
  int type_ouvrage = 0;
  int id_ouvrage = 0;
  int id_sequence = 0;
  int disponibilite = 0;
  
  static char *version = "$Id: itre_ax.c,v 1.153 2021/06/03 07:21:36 pc2dpdy Exp $ : itre_ax_Abt_Etat_Seq_ENR" ;

  /*trace de l'execution*/
  XZST_03EcritureTrace(XZSTC_FONCTION,"IN -- %s : %d %d %d %d", version, va_etat.TypeEqt,
						       va_etat.NumEqt,
			    			       va_etat.SeqChange,
		    			               va_etat.EtatSeq);
			    
/*je formate ces donnes pour Applix et je les envoie*/
	type_ouvrage = va_etat.TypeEqt;
	id_ouvrage = va_etat.NumEqt;
	id_sequence = va_etat.SeqChange;
	disponibilite = va_etat.EtatSeq;
	type_etat = COM_CANAL_MTTU_SEQ_ENR;
	sprintf(vl_cmd,"%d,%d,%d,%d",
			type_ouvrage,
			id_ouvrage,
			id_sequence,
			disponibilite);
	XZST_03EcritureTrace(XZSTC_FONCTION,"Message envoye a Applix : %s", vl_cmd);
	

  /*envoi du poke a Applix*/
  tl_params[0]=vl_cmd;
  
  if (itre_com_SendPoke(vg_socEnvToAxMce,type_etat,tl_params,1,va_nomMachine)!=XDC_OK)
  {
     XZST_03EcritureTrace(XZSTC_WARNING,"itre_ax_Abt_Etat_Seq_ENR: erreur sur le send poke COM_CANAL_MTEC_SEQ");
     *pa_resultat=XDC_NOK;
  }
  else *pa_resultat=XDC_OK;

  XZST_03EcritureTrace(XZSTC_FONCTION,"OUT -- %s",version);
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static void itre_ax_Abt_Etat_Seq_EXP(XZEAT_Sequence va_etat,int *pa_resultat, char *va_nomMachine)

/*
* ARGUMENTS EN ENTREE :
*   < Valeur et descriptif des parametres en entree >
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR : 
*   <Valeur de retour et un commentaire explicatif >
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   < Bref descriptif fonctionnel >
*
------------------------------------------------------*/
{
  char vl_cmd[50] = "";
  char *tl_params[10];
  int type_etat = 0;
  int type_ouvrage = 0;
  int id_ouvrage = 0;
  int id_sequence = 0;
  int disponibilite = 0;
  
  static char *version = "$Id: itre_ax.c,v 1.153 2021/06/03 07:21:36 pc2dpdy Exp $ : itre_ax_Abt_Etat_Seq_EXP" ;

  /*trace de l'execution*/
  XZST_03EcritureTrace(XZSTC_FONCTION,"IN -- %s : %d %d %d %d", version, va_etat.TypeEqt,
						       va_etat.NumEqt,
			    			       va_etat.SeqChange,
		    			               va_etat.EtatSeq);
			    
/*je formate ces donnes pour Applix et je les envoie*/
	type_ouvrage = va_etat.TypeEqt;
	id_ouvrage = va_etat.NumEqt;
	id_sequence = va_etat.SeqChange;
	disponibilite = va_etat.EtatSeq;
	type_etat = COM_CANAL_MTTU_SEQ_EXP;
	sprintf(vl_cmd,"%d,%d,%d,%d",
			type_ouvrage,
			id_ouvrage,
			id_sequence,
			disponibilite);
	XZST_03EcritureTrace(XZSTC_FONCTION,"Message envoye a Applix : %s", vl_cmd);
	

  /*envoi du poke a Applix*/
  tl_params[0]=vl_cmd;
  
  if (itre_com_SendPoke(vg_socEnvToAxMce,type_etat,tl_params,1,va_nomMachine)!=XDC_OK)
  {
     XZST_03EcritureTrace(XZSTC_WARNING,"itre_ax_Abt_Etat_Seq_EXP: erreur sur le send poke COM_CANAL_MTEC_SEQ");
     *pa_resultat=XDC_NOK;
  }
  else *pa_resultat=XDC_OK;

  XZST_03EcritureTrace(XZSTC_FONCTION,"OUT -- %s",version);
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static void itre_ax_Abt_Etat_Seq_INC(XZEAT_Sequence va_etat,int *pa_resultat, char *va_nomMachine)

/*
* ARGUMENTS EN ENTREE :
*   < Valeur et descriptif des parametres en entree >
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR : 
*   <Valeur de retour et un commentaire explicatif >
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   < Bref descriptif fonctionnel >
*
------------------------------------------------------*/
{
  char vl_cmd[50] = "";
  char *tl_params[10];
  int type_etat = 0;
  int type_ouvrage = 0;
  int id_ouvrage = 0;
  int id_sequence = 0;
  int disponibilite = 0;
  
  static char *version = "$Id: itre_ax.c,v 1.153 2021/06/03 07:21:36 pc2dpdy Exp $ : itre_ax_Abt_Etat_Seq_INC" ;

  /*trace de l'execution*/
  XZST_03EcritureTrace(XZSTC_FONCTION,"IN -- %s : %d %d %d %d", version, va_etat.TypeEqt,
						       va_etat.NumEqt,
			    			       va_etat.SeqChange,
		    			               va_etat.EtatSeq);
			    
/*je formate ces donnes pour Applix et je les envoie*/
	type_ouvrage = va_etat.TypeEqt;
	id_ouvrage = va_etat.NumEqt;
	id_sequence = va_etat.SeqChange;
	disponibilite = va_etat.EtatSeq;
	type_etat = COM_CANAL_MTTU_SEQ_INC;
	sprintf(vl_cmd,"%d,%d,%d,%d",
			type_ouvrage,
			id_ouvrage,
			id_sequence,
			disponibilite);
	XZST_03EcritureTrace(XZSTC_FONCTION,"Message envoye a Applix : %s", vl_cmd);
	

  /*envoi du poke a Applix*/
  tl_params[0]=vl_cmd;
  
  if (itre_com_SendPoke(vg_socEnvToAxMce,type_etat,tl_params,1,va_nomMachine)!=XDC_OK)
  {
     XZST_03EcritureTrace(XZSTC_WARNING,"itre_ax_Abt_Etat_Seq_INC: erreur sur le send poke COM_CANAL_MTEC_SEQ");
     *pa_resultat=XDC_NOK;
  }
  else *pa_resultat=XDC_OK;

  XZST_03EcritureTrace(XZSTC_FONCTION,"OUT -- %s",version);
}

/* Fin GTC Renovee */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Fonction appeler lors d'un chgt d'etat d'un PMV
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static void itre_ax_Abt_Etat_PMV(XZEAT_Etat_PMV va_etat,int *pa_resultat, char *va_nomMachine)

/*
* ARGUMENTS EN ENTREE :
*   < Valeur et descriptif des parametres en entree >
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR : 
*   <Valeur de retour et un commentaire explicatif >
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   < Bref descriptif fonctionnel >
*
------------------------------------------------------*/
{
  char vl_cmd[250] = "";
  char *tl_params[30];

  static char *version = "$Id: itre_ax.c,v 1.153 2021/06/03 07:21:36 pc2dpdy Exp $ : itre_ax_Abt_Etat_PMV" ;
  
 /*trace de l'execution*/
  XZST_03EcritureTrace(XZSTC_FONCTION,"IN -- %s : %d %s %d %d %s %s %d %d %s %s %d %d %s %d", version,
  	                                                                          va_etat.Numero,          /* N³ d' equipement */
	                                                                          va_etat.Texte1,	   /* Texte 1ere ligne */
	                                                                          va_etat.Affichage1,	   /* texte,temperature ou heure */
	                                                                          va_etat.Clign1,	   /* Clignotement 1ere ligne */
	                                                                          va_etat.Alternat1,	   /* Texte 1ere ligne alternativement */
	                                                                          va_etat.Texte2,          /* Texte 2eme ligne */
	                                                                          va_etat.Affichage2,	   /* texte,temperature ou heure */
	                                                                          va_etat.Clign2,          /* Clignotement 2eme ligne */
	                                                                          va_etat.Alternat2,	   /* Texte 2eme ligne alternativement */
	                                                                          va_etat.Texte3,	   /* Texte 3eme ligne */
	                                                                          va_etat.Affichage3,	   /* texte,temperature ou heure */
	                                                                          va_etat.Clign3,	   /* Clignotement 3eme ligne */
	                                                                          va_etat.Alternat3,	   /* Texte 3eme ligne alternativement */
	                                                                          va_etat.Texte4,	   /* Texte 4eme ligne */
	                                                                          va_etat.Affichage4,	   /* texte,temperature ou heure */
	                                                                          va_etat.Clign4,	   /* Clignotement 4eme ligne */
	                                                                          va_etat.Alternat4,	   /* Texte 4eme ligne alternativement */
	                                                                          va_etat.Flash);          /* Etat du flash */
			    
  /*je formate ces donnes pour Applix et je les envoie*/
  if ( va_etat.Affichage4 == XDC_PMV_NON_EXIST )
     /* PMV 3 Lignes */
     sprintf(vl_cmd,"%d,%s,%d,%d,%s,%s,%d,%d,%s,%s,%d,%d,%s,%d",va_etat.Numero,       
	                                                     va_etat.Texte1,	   
	                                                     va_etat.Affichage1,   
	                                                     va_etat.Clign1,	   
	                                                     va_etat.Alternat1,	   
	                                                     va_etat.Texte2,       
	                                                     va_etat.Affichage2,   
	                                                     va_etat.Clign2,       
	                                                     va_etat.Alternat2,	   
	                                                     va_etat.Texte3,	   
	                                                     va_etat.Affichage3,   
	                                                     va_etat.Clign3,	   
	                                                     va_etat.Alternat3,	   
	                                                     va_etat.Flash);       
  else
     /* PMV 4 Lignes */
     sprintf(vl_cmd,"%d,%s,%d,%d,%s,%s,%d,%d,%s,%s,%d,%d,%s,%s,%d,%d,%s,%d",va_etat.Numero,       
	                                                     va_etat.Texte1,	   
	                                                     va_etat.Affichage1,   
	                                                     va_etat.Clign1,	   
	                                                     va_etat.Alternat1,	   
	                                                     va_etat.Texte2,       
	                                                     va_etat.Affichage2,   
	                                                     va_etat.Clign2,       
	                                                     va_etat.Alternat2,	   
	                                                     va_etat.Texte3,	   
	                                                     va_etat.Affichage3,   
	                                                     va_etat.Clign3,	   
	                                                     va_etat.Alternat3,	   
	                                                     va_etat.Texte4,	   
	                                                     va_etat.Affichage4,   
	                                                     va_etat.Clign4,	   
	                                                     va_etat.Alternat4,	   
	                                                     va_etat.Flash);       

  /*envoi du poke a Applix*/
  tl_params[0]=vl_cmd; 
  
  if (itre_com_SendPoke(vg_socEnvToAxMce,COM_CANAL_MTPM_ETAT,tl_params,1,va_nomMachine)!=XDC_OK)
  {
     XZST_03EcritureTrace(XZSTC_WARNING,"itre_ax_Abt_Etat_PMV: erreur sur le send poke COM_CANAL_MTPM_ETAT");
     *pa_resultat=XDC_NOK;
  }
  else *pa_resultat=XDC_OK;

  XZST_03EcritureTrace(XZSTC_FONCTION,"OUT -- %s", version);
}	



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Fonction appeler lors d'un chgt d'etat d'un PMVA
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static void itre_ax_Abt_Etat_PMVA(XZEAT_Etat_PMVA va_etat,int *pa_resultat, char *va_nomMachine)

/*
* ARGUMENTS EN ENTREE :
*   < Valeur et descriptif des parametres en entree >
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR : 
*   <Valeur de retour et un commentaire explicatif >
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   < Bref descriptif fonctionnel >
*
------------------------------------------------------*/
{
  char vl_cmd[250] = "";
  char *tl_params[30];

  static char *version = "$Id: itre_ax.c,v 1.153 2021/06/03 07:21:36 pc2dpdy Exp $ : itre_ax_Abt_Etat_PMVA" ;
  
 /*trace de l'execution*/
  XZST_03EcritureTrace(XZSTC_FONCTION,"IN -- %s : %d %s %d %d %s %s %d %d %s %s %d %d %s %d", version,
  	                                                                          va_etat.Numero,          /* N³ d' equipement */
	                                                                          va_etat.Texte1,	   /* Texte 1ere ligne */
	                                                                          va_etat.Affichage1,	   /* texte,temperature ou heure */
	                                                                          va_etat.Clign1,	   /* Clignotement 1ere ligne */
	                                                                          va_etat.Alternat1,	   /* Texte 1ere ligne alternativement */
	                                                                          va_etat.Texte2,          /* Texte 2eme ligne */
	                                                                          va_etat.Affichage2,	   /* texte,temperature ou heure */
	                                                                          va_etat.Clign2,          /* Clignotement 2eme ligne */
	                                                                          va_etat.Alternat2,	   /* Texte 2eme ligne alternativement */
	                                                                          va_etat.Texte3,	   /* Texte 3eme ligne */
	                                                                          va_etat.Affichage3,	   /* texte,temperature ou heure */
	                                                                          va_etat.Clign3,	   /* Clignotement 3eme ligne */
	                                                                          va_etat.Alternat3,	   /* Texte 3eme ligne alternativement */
	                                                                          va_etat.Texte4,	   /* Texte 4eme ligne */
	                                                                          va_etat.Affichage4,	   /* texte,temperature ou heure */
	                                                                          va_etat.Clign4,	   /* Clignotement 4eme ligne */
	                                                                          va_etat.Alternat4,	   /* Texte 4eme ligne alternativement */
	                                                                          va_etat.Texte5,	   /* Texte 4eme ligne */
	                                                                          va_etat.Affichage5,	   /* texte,temperature ou heure */
	                                                                          va_etat.Clign5,	   /* Clignotement 4eme ligne */
	                                                                          va_etat.Alternat5,	   /* Texte 4eme ligne alternativement */
	                                                                          va_etat.Flash);          /* Etat du flash */
			    
     sprintf(vl_cmd,"%d,%s,%d,%d,%s,%s,%d,%d,%s,%s,%d,%d,%s,%s,%d,%d,%s,%s,%d,%d,%s,%d",va_etat.Numero,       
	                                                     va_etat.Texte1,	   
	                                                     va_etat.Affichage1,   
	                                                     va_etat.Clign1,	   
	                                                     va_etat.Alternat1,	   
	                                                     va_etat.Texte2,       
	                                                     va_etat.Affichage2,   
	                                                     va_etat.Clign2,       
	                                                     va_etat.Alternat2,	   
	                                                     va_etat.Texte3,	   
	                                                     va_etat.Affichage3,   
	                                                     va_etat.Clign3,	   
	                                                     va_etat.Alternat3,	   
	                                                     va_etat.Texte4,	   
	                                                     va_etat.Affichage4,   
	                                                     va_etat.Clign4,	   
	                                                     va_etat.Alternat4,	   
	                                                     va_etat.Texte5,	   
	                                                     va_etat.Affichage5,   
	                                                     va_etat.Clign5,	   
	                                                     va_etat.Alternat5,	   
	                                                     va_etat.Flash);       

  /*envoi du poke a Applix*/
  tl_params[0]=vl_cmd; 
  
  if (itre_com_SendPoke(vg_socEnvToAxMce,COM_CANAL_MTPMVA_ETAT,tl_params,1,va_nomMachine)!=XDC_OK)
  {
     XZST_03EcritureTrace(XZSTC_WARNING,"itre_ax_Abt_Etat_PMVA: erreur sur le send poke COM_CANAL_MTPMVA_ETAT");
     *pa_resultat=XDC_NOK;
  }
  else *pa_resultat=XDC_OK;

  XZST_03EcritureTrace(XZSTC_FONCTION,"OUT -- %s", version);
}	


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	itre_ax_retour_lcr ( 
        T_IPC_CONN 			va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG                        va_nomMachine)
 
/*
* ARGUMENTS EN ENTREE :
*  Message RTworks
*
* ARGUMENTS EN SORTIE :
*   aucun
*
*
* CODE RETOUR : 
*   aucun
*
* CONDITION D'UTILISATION
*  Fonction appelee lors de la reception du message
*   
*
* FONCTION 
*
*
------------------------------------------------------*/
{
#ifndef _TIBCO_EMS
   T_INT2		vl_CrLCR = 0;
#else
   XDY_Mot		vl_CrLCR = 0;
#endif
   char                 vl_cmd[100] = "";
   char                 *tl_params[10];

		  
  XDY_NomMachine vl_machine="";

  static char *version = "$Id: itre_ax.c,v 1.153 2021/06/03 07:21:36 pc2dpdy Exp $ : itre_ax_retour_lcr" ;
   
   XZEX04_Annul_TimeOut ();
   XZST_03EcritureTrace(XZSTC_FONCTION,"IN -- %s", version);
   
   /* Recuperation des parametres du message XDM_ECPT_LCR */
   if (!TipcMsgRead( pa_Data -> msg,
 		     T_IPC_FT_INT2 ,&vl_CrLCR,
	             NULL) )
   {
      XZST_03EcritureTrace ( XZSTC_WARNING,"%s Impossible de lire le MSG XDM_ECPT_LCR", version );
   }
   else
   { 
     XZST_03EcritureTrace ( XZSTC_INFO, "%s : Cr:%d", version, vl_CrLCR );

	if (vg_titre)
	{

		if(va_nomMachine!=NULL)
		{
			strncpy(vl_machine,(char *)va_nomMachine,11);//Machine fournit par magistra
			XZST_03EcritureTrace ( XZSTC_INFO, "itre_ax_retour_lcr : Retour commande LCR sur machine : %s ", vl_machine);
		}
		else
			XZST_03EcritureTrace ( XZSTC_WARNING, "itre_ax_retour_lcr : Retour commande LCR NomMachine VIDE !!!");
	}		
	else
	{
		if (XZSC_GetDestinataire(pa_Data, vl_machine)!=XDC_OK) {
			XZST_03EcritureTrace(XZSTC_WARNING,"itre_ax_retour_lcr : probleme dans recuperation machine destinataire du message");
		}
	}
    /*je formate ces donnes pour Applix et je les envoie*/
    sprintf(vl_cmd,"%d", vl_CrLCR);
    /*envoi du poke a Applix*/
    tl_params[0]=vl_cmd;
  
    if (itre_com_SendPoke(vg_socEnvToAxMce,COM_CANAL_MTIE,tl_params,1,vl_machine)!=XDC_OK)
    {
       XZST_03EcritureTrace(XZSTC_WARNING,"itre_ax_retour_lcr: erreur sur le send poke COM_CANAL_MTIE");
     
    }
     
   }
   
   XZST_03EcritureTrace(XZSTC_FONCTION,"OUT -- %s", version);
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	itre_ax_timeout_lcr()
 
/*
* ARGUMENTS EN ENTREE :
*  aucun
*
* ARGUMENTS EN SORTIE :
*   aucun
*
*
* CODE RETOUR : 
*   aucun
*
* CONDITION D'UTILISATION
*  Fonction appelÅe sur timeout demande LCR
*   
*
* FONCTION 
*
*
------------------------------------------------------*/
{
   char                 vl_cmd[100] = "";
   char                 *tl_params[10];
		  
  static char *version = "$Id: itre_ax.c,v 1.153 2021/06/03 07:21:36 pc2dpdy Exp $ : itre_ax_timeout_lcr" ;
   
   XZST_03EcritureTrace(XZSTC_FONCTION,"IN -- %s", version);
   
   /*je formate les donnes pour Applix et je les envoie*/
   sprintf(vl_cmd,"%d", XDC_ECHEC );
   /*envoi du poke a Applix*/
   tl_params[0]=vl_cmd;
  
   if (itre_com_SendPoke(vg_socEnvToAxMce,COM_CANAL_MTIE,tl_params,1,"")!=XDC_OK)
   {
      XZST_03EcritureTrace(XZSTC_WARNING,"itre_ax_retour_lcr: erreur sur le send poke COM_CANAL_MTIE");
   }
     
   
   XZST_03EcritureTrace(XZSTC_FONCTION,"OUT -- %s", version);
}










/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Fonction appeler lors d'un chgt d'etat d'un Picto
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static void itre_ax_Abt_Etat_Picto(XZEAT_Etat_Picto va_etat,int *pa_resultat, char *va_nomMachine)

/*
* ARGUMENTS EN ENTREE :
*   < Valeur et descriptif des parametres en entree >
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR : 
*   <Valeur de retour et un commentaire explicatif >
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   < Bref descriptif fonctionnel >
*
------------------------------------------------------*/
{
  char vl_cmd[250] = "";
  char *tl_params[10];

  static char *version = "$Id: itre_ax.c,v 1.153 2021/06/03 07:21:36 pc2dpdy Exp $ : itre_ax_Abt_Etat_Picto" ;
 
  /*trace de l'execution*/
  XZST_03EcritureTrace(XZSTC_FONCTION,"IN -- %s : %d %s %s %d %d", version,
  	                                               va_etat.Numero,          /* N³ d' equipement */
	                                               va_etat.Texte,	        /* Texte du picto */
	                                               va_etat.Picto,	        /* Type de picto*/
	                                               va_etat.Clign,	        /* Clignotement  */
	                                               va_etat.NoCMD);         /* n³ de commande */
			    
  /*je formate ces donnes pour Applix et je les envoie*/
  sprintf(vl_cmd,"%d,%s,%s,%d,%d",va_etat.Numero,          /* N³ d' equipement */
	                          va_etat.Texte,	   /* Texte du picto */
	                          va_etat.Picto,           /* Type de picto */
	                          va_etat.Clign,	   /* Clignotement picto */
	                          va_etat.NoCMD);          /* n³ de commande */


  /*envoi du poke a Applix*/
  tl_params[0]=vl_cmd;
  
  if (itre_com_SendPoke(vg_socEnvToAxMce,COM_CANAL_MTPM_PICTO,tl_params,1,va_nomMachine)!=XDC_OK)
  {
     XZST_03EcritureTrace(XZSTC_WARNING,"itre_ax_Abt_Etat_Picto: erreur sur le send poke COM_CANAL_MTPM_PICTO");
     *pa_resultat=XDC_NOK;
  }
  else *pa_resultat=XDC_OK;
  
  XZST_03EcritureTrace(XZSTC_FONCTION,"OUT -- %s", version);
}	


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Fonction appeler lors de la reception d'une valeur TDP
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static void itre_ax_Dde_Valeur_TDP(XZEZT_Val_TDP va_valeur,int *pa_resultat, char *va_nomMachine)

/*
* ARGUMENTS EN ENTREE :
*   < Valeur et descriptif des parametres en entree >
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR : 
*   <Valeur de retour et un commentaire explicatif >
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   < Bref descriptif fonctionnel >
*
------------------------------------------------------*/
{
  char vl_cmd[250] = "";
  char *tl_params[10];

  static char *version = "$Id: itre_ax.c,v 1.153 2021/06/03 07:21:36 pc2dpdy Exp $ : itre_ax_Dde_Valeur_TDP" ;
 
  /*trace de l'execution*/
  XZST_03EcritureTrace(XZSTC_FONCTION,"IN -- %s : %d %s %s %s", version,
  	                                               va_valeur.Numero,          /* N³ du PMV */
	                                               va_valeur.Dest,	        /* Valeur TDP */
	                                               va_valeur.DestAlt,         /* Valeur TDP pour alternat */
						       va_valeur.DestNoeud);	/* Valeur TdP Noeud */
			    
  /*je formate ces donnes pour Applix et je les envoie*/
  sprintf(vl_cmd,"%d,%s,%s,%s",
  	                                               va_valeur.Numero,          /* N³ du PMV */
	                                               va_valeur.Dest,	        /* Valeur TDP */
						       va_valeur.DestAlt,         /* Valeur TDP pour alternat */
	                                               va_valeur.DestNoeud );         /* Valeur TdP Noeud */

  /*envoi du poke a Applix*/
  tl_params[0]=vl_cmd;
  
  if (itre_com_SendPoke(vg_socEnvToAxMce,COM_CANAL_MTPM_TDP,tl_params,1,va_nomMachine)!=XDC_OK)
  {
     XZST_03EcritureTrace(XZSTC_WARNING,"itre_ax_Dde_Valeur_TDP: erreur sur le send poke COM_CANAL_MTPM_TDP");
     *pa_resultat=XDC_NOK;
  }
  else *pa_resultat=XDC_OK;
  
  XZST_03EcritureTrace(XZSTC_FONCTION,"OUT -- %s", version);
}	


static void itre_ax_Dde_Valeur_TDP_PMVA(XZEZT_Val_TDP_PMVA va_valeur,int *pa_resultat, char *va_nomMachine)

/*
* ARGUMENTS EN ENTREE :
*   < Valeur et descriptif des parametres en entree >
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR :
*   <Valeur de retour et un commentaire explicatif >
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction
*
* FONCTION
*   < Bref descriptif fonctionnel >
*
------------------------------------------------------*/
{
  char vl_cmd[250] = "";
  char *tl_params[10];

  /*DEM 1014*/
  static char *	version = "@(#)itre_ax.c	1.122 12/05/06 : itre_ax_Dde_Valeur_TDP_PMVA" ;

  /*trace de l'execution*/
  XZST_03EcritureTrace(XZSTC_FONCTION,"IN -- %s : %d %s %s %s %s", version,
  	                                               va_valeur.Numero,
	                                               va_valeur.Dest1_proche,
	                                               va_valeur.Dest1_loin,
	                                               va_valeur.Dest2_proche,
						       va_valeur.Dest2_loin);

  /*je formate ces donnes pour Applix et je les envoie*/
  sprintf(vl_cmd,"%d,%s,%s,%s,%s",
  	                                               va_valeur.Numero,          /* N� du PMV */
	                                               va_valeur.Dest1_proche,	        /* Valeur TDP */
	                                               va_valeur.Dest1_loin,
						       va_valeur.Dest2_proche,
						       va_valeur.Dest2_loin);         /* Valeur TDP pour alternat */

  /*envoi du poke a Applix*/
  tl_params[0]=vl_cmd;

  if (itre_com_SendPoke(vg_socEnvToAxMce,COM_CANAL_MTPMVA_TDP,tl_params,1,va_nomMachine)!=XDC_OK)
  {
     XZST_03EcritureTrace(XZSTC_WARNING,"itre_ax_Dde_Valeur_TDP: erreur sur le send poke COM_CANAL_MTPM_TDP");
     *pa_resultat=XDC_NOK;
  }
  else *pa_resultat=XDC_OK;

  XZST_03EcritureTrace(XZSTC_FONCTION,"OUT -- %s", version);
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Fonction appeler lors d'un chgt d'etat d'un BRA
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static void itre_ax_MAJ_etat_NAV(XZEAT_Etat_NAV va_etat,int *pa_resultat, char *va_nomMachine)

/*
* ARGUMENTS EN ENTREE :
*   < Valeur et descriptif des parametres en entree >
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR : 
*   <Valeur de retour et un commentaire explicatif >
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   < Bref descriptif fonctionnel >
*
------------------------------------------------------*/
{
  char vl_cmd[255] = "";
  char *tl_params[250];
  char vl_table[250];
  int  vl_index = 0, vl_ptr = 0;

  static char *version = "$Id: itre_ax.c,v 1.153 2021/06/03 07:21:36 pc2dpdy Exp $ : itre_ax_MAJ_etat_NAV" ;
  
  /*trace de l'execution*/
  XZST_03EcritureTrace(XZSTC_FONCTION,"IN -- %s :%d %s", version,
  	                                      va_etat.Etat.Numero,         /* N³ d' equipement */
	                                      va_etat.Etat.Modules[0]);    /* Etat de chaque module */
			    
  for ( vl_ptr = vl_index = 0; vl_index < XDC_Lon_Nb_Mod_Nav; vl_index++ )
  {
    vl_ptr += sprintf ( vl_table+vl_ptr, "%d,%s,", vl_index,va_etat.Etat.Modules[vl_index] );
  }
  
  XZST_03EcritureTrace(XZSTC_DEBUG1," Table:%s",vl_table );
  
  /*je formate ces donnes pour Applix et je les envoie*/
  sprintf(vl_cmd,"%d,%s",va_etat.Etat.Numero,     /* N³ d' equipement */
	                 vl_table);          /* chaine de l'etat de chaque module */
	                 

  /*envoi du poke a Applix*/
  tl_params[0]=vl_cmd;

  if (itre_com_SendPoke(vg_socEnvToAxMce,COM_CANAL_MTNA,tl_params,1,va_nomMachine)!=XDC_OK)
  {
     XZST_03EcritureTrace(XZSTC_WARNING,"itre_ax_MAJ_Etat_NAV: erreur sur le send poke COM_CANAL_MTNA");
     *pa_resultat=XDC_NOK;
  }
  else *pa_resultat=XDC_OK;
  
  XZST_03EcritureTrace(XZSTC_FONCTION,"OUT -- %s", version );
 
}	


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Fonction appeler lors d'un chgt d'etat d'un BAF
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static void itre_ax_MAJ_etat_BAF(XZEAT_Etat_BAF va_etat,int *pa_resultat, char *va_nomMachine)

/*
* ARGUMENTS EN ENTREE :
*   < Valeur et descriptif des parametres en entree >
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR : 
*   <Valeur de retour et un commentaire explicatif >
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   < Bref descriptif fonctionnel >
*
------------------------------------------------------*/
{
  char vl_cmd[255] = "";
  char *tl_params[255];
  char vl_table[250]="";
  int  vl_index = 0, vl_ptr = 0;

  static char *version = "$Id: itre_ax.c,v 1.153 2021/06/03 07:21:36 pc2dpdy Exp $ : itre_ax_MAJ_etat_BAF" ;
  
  /*trace de l'execution*/
  XZST_03EcritureTrace(XZSTC_FONCTION,"IN -- %s :%d %s %s %s %s %s %s", version,
  	                                      va_etat.Etat.Numero,         /* N³ d' equipement */
	                                      va_etat.Etat.Modules[0],
	                                      va_etat.Etat.Modules[1],
	                                      va_etat.Etat.Modules[2],
	                                      va_etat.Etat.Modules[3],
	                                      va_etat.Etat.Modules[4],
	                                      va_etat.Etat.Modules[5]);    /* Etat de chaque module */
			    
  for ( vl_ptr = vl_index = 0; vl_index < XDC_Lon_Nb_Mod_BAF; vl_index++ )
  {
    vl_ptr += sprintf ( vl_table+vl_ptr, "%d,%s%s", vl_index,va_etat.Etat.Modules[vl_index], (vl_index<XDC_Lon_Nb_Mod_BAF-1)?",":"" );
  }
  
  XZST_03EcritureTrace(XZSTC_DEBUG1," Table:%s",vl_table );

  /*je formate ces donnes pour Applix et je les envoie*/
  sprintf(vl_cmd,"%d,%s",va_etat.Etat.Numero,     /* N³ d' equipement */
	                 vl_table);          /* chaine de l'etat de chaque module */
	                 
  /*envoi du poke a Applix*/
  tl_params[0]=vl_cmd;

  if (itre_com_SendPoke(vg_socEnvToAxMce,COM_CANAL_MTBAF,tl_params,1,va_nomMachine)!=XDC_OK)
  {
     XZST_03EcritureTrace(XZSTC_WARNING,"itre_ax_MAJ_Etat_BAF: erreur sur le send poke COM_CANAL_MTBAF");
     *pa_resultat=XDC_NOK;
  }
  else *pa_resultat=XDC_OK;
  
  XZST_03EcritureTrace(XZSTC_FONCTION,"OUT -- %s", version );
 
}	


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Fonction appeler lors d'un chgt d'etat d'un BAD
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static void itre_ax_MAJ_etat_BAD(XZEAT_Etat_BAD va_etat,int *pa_resultat, char *va_nomMachine)

/*
* ARGUMENTS EN ENTREE :
*   < Valeur et descriptif des parametres en entree >
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR : 
*   <Valeur de retour et un commentaire explicatif >
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   < Bref descriptif fonctionnel >
*
------------------------------------------------------*/
{
  char vl_cmd[255] = "";
  char *tl_params[20];
  char vl_table[250];
  int  vl_index = 0, vl_ptr = 0;

  static char *version = "$Id: itre_ax.c,v 1.153 2021/06/03 07:21:36 pc2dpdy Exp $ : itre_ax_MAJ_etat_BAD" ;
  
  /*trace de l'execution*/
  XZST_03EcritureTrace(XZSTC_WARNING,"IN -- %s :%d %s", version,
  	                                      va_etat.Etat.Numero,         /* N³ d' equipement */
	                                      va_etat.Etat.Modules[0]);    /* Etat de chaque module */
			    
  for ( vl_ptr = vl_index = 0; vl_index < XDC_Lon_Nb_Mod_BAD; vl_index++ )
  {
    vl_ptr += sprintf ( vl_table+vl_ptr, "%d,%s,", vl_index,va_etat.Etat.Modules[vl_index] );
  }
  
  XZST_03EcritureTrace(XZSTC_DEBUG1," Table:%s",vl_table );
  
  /*je formate ces donnes pour Applix et je les envoie*/
  sprintf(vl_cmd,"%d,%s",va_etat.Etat.Numero,     /* N³ d' equipement */
	                 vl_table);          /* chaine de l'etat de chaque module */
	                 

  /*envoi du poke a Applix*/
  tl_params[0]=vl_cmd;

  if (itre_com_SendPoke(vg_socEnvToAxMce,COM_CANAL_MTBAD,tl_params,1,va_nomMachine)!=XDC_OK)
  {
     XZST_03EcritureTrace(XZSTC_WARNING,"itre_ax_MAJ_Etat_BAD: erreur sur le send poke COM_CANAL_MTBAD");
     *pa_resultat=XDC_NOK;
  }
  else *pa_resultat=XDC_OK;
  
  XZST_03EcritureTrace(XZSTC_WARNING,"OUT -- %s", version );
 
}	


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Fonction appeler lors d'un chgt d'etat d'un PRV
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static void itre_ax_MAJ_etat_PRV(XZEAT_Etat_PRV va_etat,int *pa_resultat, char *va_nomMachine)

/*
* ARGUMENTS EN ENTREE :
*   < Valeur et descriptif des parametres en entree >
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR : 
*   <Valeur de retour et un commentaire explicatif >
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   < Bref descriptif fonctionnel >
*
------------------------------------------------------*/
{
  char vl_cmd[255] = "";
  char *tl_params[20];
  char vl_table[250];
  int  vl_index = 0, vl_ptr = 0;

  static char *	version = "@(#)itre_ax.c	1.122 12/05/06 : itre_ax_MAJ_etat_PRV" ;
  
  /*trace de l'execution*/
  XZST_03EcritureTrace(XZSTC_WARNING,"IN -- %s :%d %d", version,
  	                                      va_etat.Numero,         /* Num d' equipement */
  	                                      va_etat.Vitesse,			/* Vitesse */
  	                                      va_etat.Bandeau,			/* Bandeau */
	                                      va_etat.NoCMD);    	/* Numero de commande */
			    
  /*je formate ces donnes pour Applix et je les envoie*/
  sprintf(vl_cmd,"%d,%s,%s",va_etat.Numero,     /* Num d' equipement */
		  	  	  	  	    va_etat.Vitesse,	/* Vitesse */
		  	  	  	  	    va_etat.Bandeau);	/* Bandeau */
	                 

  /*envoi du poke a Applix*/
  tl_params[0]=vl_cmd;

  if (itre_com_SendPoke(vg_socEnvToAxMce,COM_CANAL_MTPRV_ETAT,tl_params,1,va_nomMachine)!=XDC_OK)
  {
     XZST_03EcritureTrace(XZSTC_WARNING,"itre_ax_MAJ_Etat_PRV: erreur sur le send poke COM_CANAL_MTPRV");
     *pa_resultat=XDC_NOK;
  }
  else *pa_resultat=XDC_OK;
  
  XZST_03EcritureTrace(XZSTC_WARNING,"OUT -- %s", version );
 
}	

/*------------------------------------------------------
* SERVICE RENDU : 
*  Fonction appeler lors d'un chgt d'etat d'un PIC
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
static void itre_ax_MAJ_etat_PIC (XZEAT_Etat_PIC va_etat, int *pa_resultat, char *va_nomMachine)
/*
* ARGUMENTS EN ENTREE :
*   < Valeur et descriptif des parametres en entree >
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR : 
*   <Valeur de retour et un commentaire explicatif >
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   < Bref descriptif fonctionnel >
*
------------------------------------------------------*/
{
	char		vl_cmd [255] = "";
	char		* tl_params [20];
	char		vl_table [250];
	int			vl_index = 0, vl_ptr = 0;
	static char * version = "itre_ax.c : itre_ax_MAJ_etat_PIC";
  
	/* trace de l'execution */
	XZST_03EcritureTrace (XZSTC_WARNING,"IN -- %s :%d %s %s %s %d",	version,
																	va_etat.Numero,		/* Num d' equipement */
																	va_etat.Bandeau,	/* Bandeau */
																	va_etat.Flash,		/* Flash */
																	va_etat.Picto,		/* Picto */
																	va_etat.NoCMD );	/* Numero de commande */
			    
	/* je formate ces donnes pour Applix et je les envoie */
	sprintf (vl_cmd,"%d,%s,%s,%s",	va_etat.Numero,     /* Num d' equipement */
									va_etat.Bandeau,	/* Bandeau */
									va_etat.Flash,		/* Flash */
									va_etat.Picto );	/* Picto */

	/* envoi du poke a Applix */
	tl_params [0] = vl_cmd;

	if (itre_com_SendPoke (vg_socEnvToAxMce, COM_CANAL_MTPIC_ETAT, tl_params, 1, va_nomMachine) != XDC_OK)
	{
		XZST_03EcritureTrace (XZSTC_WARNING, "itre_ax_MAJ_Etat_PIC: erreur sur le send poke COM_CANAL_MTPIC");
		*pa_resultat = XDC_NOK;
	}
	else
		*pa_resultat = XDC_OK;
	XZST_03EcritureTrace (XZSTC_WARNING,"OUT -- %s", version );
}	

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Fonction appeler lors d'un chgt d'etat d'un CFE
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static void itre_ax_Abt_Etat_CFE(XZEAT_Etat_CFE va_etat,int *pa_resultat, char *va_nomMachine)

/*
* ARGUMENTS EN ENTREE :
*   < Valeur et descriptif des parametres en entree >
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR : 
*   <Valeur de retour et un commentaire explicatif >
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   < Bref descriptif fonctionnel >
*
------------------------------------------------------*/
{
  char vl_cmd[255] = "";
  char *tl_params[20];
  char vl_table[250];
  int  vl_index = 0, vl_ptr = 0;

  static char *	version = "@(#)itre_ax.c	1.122 12/05/06 : itre_ax_MAJ_etat_CFE" ;
  
  /*trace de l'execution*/
  XZST_03EcritureTrace(XZSTC_WARNING,"IN -- %s :%d %d", version,
  	                                      va_etat.Numero,         /* Num d' equipement */
  	                                      va_etat.NoPlanDeFeux,			/* Vitesse */
  	                                      va_etat.LibellePlanDeFeux);    	/* Numero de commande */
			    
  /*je formate ces donnes pour Applix et je les envoie*/
  sprintf(vl_cmd,"%d,%d,%s",va_etat.Numero,     /* Num d' equipement */
		  	  	  	  	    va_etat.NoPlanDeFeux,	/* Vitesse */
		  	  	  	  	    va_etat.LibellePlanDeFeux);	/* Bandeau */
	                 

  /*envoi du poke a Applix*/
  tl_params[0]=vl_cmd;

  if (itre_com_SendPoke(vg_socEnvToAxMce,COM_CANAL_MTCFE,tl_params,1,va_nomMachine)!=XDC_OK)
  {
     XZST_03EcritureTrace(XZSTC_WARNING,"itre_ax_MAJ_Etat_CFE: erreur sur le send poke COM_CANAL_MTPRV");
     *pa_resultat=XDC_NOK;
  }
  else *pa_resultat=XDC_OK;
  
  XZST_03EcritureTrace(XZSTC_WARNING,"OUT -- %s", version );
} 



/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Fonction appelee lors d'un chgt d'etat d'un IMU
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static void itre_ax_Abt_Etat_IMU(XZEAT_Etat_IMU va_etat,int *pa_resultat, char *va_nomMachine)

/*
* ARGUMENTS EN ENTREE :
*   < Valeur et descriptif des parametres en entree >
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR :
*   <Valeur de retour et un commentaire explicatif >
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction
*
* FONCTION
*   < Bref descriptif fonctionnel >
*
------------------------------------------------------*/

{
  char vl_cmd[255] = "";
  char *tl_params[20];
  char vl_table[250];
  int  vl_index = 0, vl_ptr = 0;

  static char * version = "@(#)itre_ax.c        1.122 12/05/06 : itre_ax_MAJ_etat_IMU" ;

  /*trace de l'execution*/
  XZST_03EcritureTrace(XZSTC_WARNING,"IN -- %s :%d %s %d %d %d", version,
 				 			va_etat.Numero,         	/* Num d' equipement */
							va_etat.ChaineIMU,                 /* Commande */
							va_etat.Intervalle,		/* Delais entre repetitions */
							va_etat.Cycles,			/* Nombre de repetitions */
							va_etat.Duree);       		/* Duree de la diffusion */

  /*je formate ces donnes pour Applix et je les envoie*/
  sprintf(vl_cmd,"%d,%s,%d,%d,%d",va_etat.Numero,     	/* Num d' equipement */
  				va_etat.ChaineIMU,       	/* Commande */
				va_etat.Intervalle,	/* Delais entre repetitions */
				va_etat.Cycles,		/* Nombre de repetitions*/
				va_etat.Duree); 	/* Duree de la diffusion */


  /*envoi du poke a Applix*/
  tl_params[0]=vl_cmd;

  if (itre_com_SendPoke(vg_socEnvToAxMce,COM_CANAL_MTIMU,tl_params,1,va_nomMachine)!=XDC_OK)
  {
    XZST_03EcritureTrace(XZSTC_WARNING,"itre_ax_MAJ_Etat_IMU: erreur sur le send poke COM_CANAL_MTIMU");
    *pa_resultat=XDC_NOK;
  }
  else *pa_resultat=XDC_OK;

  XZST_03EcritureTrace(XZSTC_WARNING,"OUT -- %s", version );
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Fonction appelee lors d'un chgt d'etat du RAU
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static void itre_ax_Abt_Etat_SONO(XZEAT_Etat_SONO va_etat,int *pa_resultat, char *va_nomMachine)

/*
* ARGUMENTS EN ENTREE :
*   < Valeur et descriptif des parametres en entree >
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR :
*   <Valeur de retour et un commentaire explicatif >
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction
*
* FONCTION
*   < Bref descriptif fonctionnel >
*
------------------------------------------------------*/

{
	char vl_cmd[255] = "";
  	char *tl_params[20];
    	char vl_table[250];
      	int  vl_index = 0, vl_ptr = 0;

        static char * version = "@(#)itre_ax.c        1.122 12/05/06 : itre_ax_Abt_Etat_SONO" ;

	/*trace de l'execution*/
	XZST_03EcritureTrace(XZSTC_WARNING,"IN -- %s :%s %d %d", version,
						va_etat.Message,
						va_etat.Nb_PAUErreur,
						va_etat.Nb_PAUPilotes);

	/*je formate ces donnes pour Applix et je les envoie*/
	sprintf(vl_cmd,"%s,%d,%d",va_etat.Message,
				va_etat.Nb_PAUErreur,
				va_etat.Nb_PAUPilotes);


	/*envoi du poke a Applix*/
	tl_params[0]=vl_cmd;

	if (itre_com_SendPoke(vg_socEnvToAxMce,COM_CANAL_MTRA_ETAT_SONO,tl_params,1,va_nomMachine)!=XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"itre_ax_Abt_Etat_SONO: erreur sur le send poke COM_CANAL_MTRA_ETAT_SONO");
		*pa_resultat=XDC_NOK;
	}
	else *pa_resultat=XDC_OK;

	XZST_03EcritureTrace(XZSTC_WARNING,"OUT -- %s", version );
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Fonction appeler lors d'un chgt d'etat d'un PAL
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static void itre_ax_MAJ_etat_PAL(XZEAT_Etat_PAL va_etat,int *pa_resultat, char *va_nomMachine)

/*
* ARGUMENTS EN ENTREE :
*   < Valeur et descriptif des parametres en entree >
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR : 
*   <Valeur de retour et un commentaire explicatif >
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   < Bref descriptif fonctionnel >
*
------------------------------------------------------*/
{
  char vl_cmd[255] = "";
  char *tl_params[20];
  char vl_table[250];
  int  vl_index = 0, vl_ptr = 0;

  static char *version = "$Id: itre_ax.c,v 1.153 2021/06/03 07:21:36 pc2dpdy Exp $ : itre_ax_MAJ_etat_PAL" ;
  
  /*trace de l'execution*/
  XZST_03EcritureTrace(XZSTC_WARNING,"IN -- %s :%d %s>%s>%s>%s>%s>%s>%s>%s>", version,
  	                                      va_etat.Numero,                          	/* N³ d' equipement */
	                                      va_etat.Modules[0],  		  	/* Etat de chaque module */
	                                      va_etat.Modules[1],  		  	/* Etat de chaque module */
	                                      va_etat.Modules[2],  		  	/* Etat de chaque module */
	                                      va_etat.Modules[3],  		  	/* Etat de chaque module */
	                                      va_etat.Modules[4],  		  	/* Etat de chaque module */
	                                      va_etat.Modules[5],  		  	/* Etat de chaque module */
	                                      va_etat.Modules[6],  		  	/* Etat de chaque module */
	                                      va_etat.Modules[7]);  		  	/* Etat de chaque module */
			    
  for ( vl_ptr = vl_index = 0; vl_index < XDC_Lon_Nb_Mod_Pal; vl_index++ )
  {
    vl_ptr += sprintf ( vl_table+vl_ptr, "%d,%s,", vl_index, va_etat.Modules[vl_index] );
  }
  
  /*je formate ces donnes pour Applix et je les envoie*/
  sprintf(vl_cmd,"%d,%s",va_etat.Numero,     /* N³ d' equipement */
	                 vl_table);          /* chaine de l'etat de chaque module */
	                 

  /*envoi du poke a Applix*/
  tl_params[0]=vl_cmd;

  if (itre_com_SendPoke(vg_socEnvToAxMce,COM_CANAL_MTPP,tl_params,1,va_nomMachine)!=XDC_OK)
  {
     XZST_03EcritureTrace(XZSTC_WARNING,"itre_ax_MAJ_Etat_PAL: erreur sur le send poke COM_CANAL_MTPP");
     *pa_resultat=XDC_NOK;
  }
  else *pa_resultat=XDC_OK;
  
  XZST_03EcritureTrace(XZSTC_FONCTION,"OUT -- %s", version );
 
}	


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Fonction appeler lors d'un chgt d'etat d'une matice NAV
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static void itre_ax_MAJ_etat_MAT_NAV (XZEAT_Etat_Mat_NAV va_etat, int *pa_resultat, char *va_nomMachine)

/*
* ARGUMENTS EN ENTREE :
*   < Valeur et descriptif des parametres en entree >
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR : 
*   <Valeur de retour et un commentaire explicatif >
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   < Bref descriptif fonctionnel >
*
------------------------------------------------------*/
{
  char vl_cmd[255] = "";
  char *tl_params[20];
  char vl_table[250];
  int  vl_index = 0, vl_ptr = 0;

  static char *version = "$Id: itre_ax.c,v 1.153 2021/06/03 07:21:36 pc2dpdy Exp $ : itre_ax_MAJ_etat_MAT_NAV" ;
  
  /*trace de l'execution*/
  XZST_03EcritureTrace(XZSTC_FONCTION,"IN -- %s :%d:%s", version, va_etat.NumEqt, va_etat.Matrice );  
			    
  for ( vl_ptr = vl_index = 0; vl_index < XDC_Lon_Params_Matrice; vl_index++ )
  {
    vl_ptr += sprintf ( vl_table+vl_ptr, ",%d", va_etat.Params[vl_index] );
  }
  
  XZST_03EcritureTrace(XZSTC_DEBUG1," Table:%s",vl_table );
  
  /*je formate ces donnes pour Applix et je les envoie*/
  sprintf(vl_cmd,"%d,%s%s",va_etat.NumEqt,     /* N³ d' equipement */
  			 va_etat.Matrice,      /* Nom Matrice */
	                 vl_table);            /* chaine de l'etat de chaque module */
	                 

  /*envoi du poke a Applix*/
  tl_params[0]=vl_cmd;

  if (itre_com_SendPoke(vg_socEnvToAxMce,COM_CANAL_MTNM,tl_params,1,va_nomMachine)!=XDC_OK)
  {
     XZST_03EcritureTrace(XZSTC_WARNING,"itre_ax_MAJ_Etat_MAT_NAV: erreur sur le send poke COM_CANAL_MTNM");
     *pa_resultat=XDC_NOK;
  }
  else *pa_resultat=XDC_OK;
  
  XZST_03EcritureTrace(XZSTC_FONCTION,"OUT -- %s", version );
 
}	



/*X*/
/*------------------------------------------------------------------------
* SERVICE RENDU : 
*  Fonction appeler lors d'un changement d'etat de l'automate telephonique
* ------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static void itre_ax_Abt_Etat_Autotel (XZEAT_Etat_Autotel va_etat, int *pa_resultat, char *va_nomMachine)

/*
* ARGUMENTS EN ENTREE :
*   va_etat   : Structure de donnees d'etat de l'automate telephonique
*
*
* ARGUMENTS EN SORTIE :
*   pa_resultat : Compte-rendu d'execution.
*
*
* CODE RETOUR :  Aucun
*
*
* CONDITION D'UTILISATION
*   Appelee lors d'un changement d'etat de l'automate telephonique
*
* FONCTION 
*   Envoie un message a la tache Applix l'informant du changement d'etat.
*
------------------------------------------------------*/
{
  char vl_cmd[50] = "";
  char *tl_params[10];
  int  vl_destinataire;
  
  static char *version = "$Id: itre_ax.c,v 1.153 2021/06/03 07:21:36 pc2dpdy Exp $ : itre_ax_Abt_Etat_Autotel" ;
  
  /*trace de l'execution*/
  XZST_03EcritureTrace(XZSTC_FONCTION, "IN -- %s :%d,%d,%s,%s,%d,%d", version,
			  va_etat.Numero,         /* N³ du poste operateur */
	                  va_etat.CodeCmd,        /* Code derniere commande */
	                  va_etat.NumAppel,       /* Dernier numero d'appel */
	                  va_etat.NumRepondeur,   /* Numero ligne repondeur */
	                  va_etat.CptRendu,       /* Compte rendu commande */
	                  va_etat.EtatCom);       /* Etat de la communication */


  /* Si le numero de poste est nul il s'agit d'un etat repondeur */
  if (va_etat.Numero == 0) {
    sprintf (vl_cmd, "%d",
	     strcmp (va_etat.NumRepondeur,XDC_MARCHE) == 0 ? XDC_OUI : XDC_NON);
    vl_destinataire = COM_CANAL_MTMT_REPONDEUR;
  } else {
    /* sinon il s'agit d'un etat automate telephonique */
    sprintf (vl_cmd, "%d|%d|%s|%d|%d", va_etat.Numero,
					va_etat.CodeCmd,
					va_etat.NumAppel,
					va_etat.CptRendu,
					va_etat.EtatCom);
    vl_destinataire = COM_CANAL_MTAT_ETAT_TEL;
  }

  /*envoi du poke a Applix*/
  tl_params[0]=vl_cmd;

  if (itre_com_SendPoke (vg_socEnvToAxMce, vl_destinataire, tl_params, 1,va_nomMachine) != XDC_OK)
  {
     XZST_03EcritureTrace (XZSTC_WARNING,
			    "itre_ax_Abt_Etat_Autotel: erreur sur envoi poke");
     *pa_resultat = XDC_NOK;
  }
  else *pa_resultat = XDC_OK;
  
  XZST_03EcritureTrace(XZSTC_FONCTION, "OUT -- %s", version);
}	





/*X*/
/*------------------------------------------------------------------------
* SERVICE RENDU : 
*  Fonction appeler lors d'un changement d'etat de l'automate operator
* ------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static void itre_ax_Abt_Etat_Operator (XZEAT_Etat_Operator va_etat, int *pa_resultat, char *va_nomMachine)

/*
* ARGUMENTS EN ENTREE :
*   va_etat   : Structure de donnees d'etat de l'automate
*
*
* ARGUMENTS EN SORTIE :
*   pa_resultat : Compte-rendu d'execution.
*
*
* CODE RETOUR :  Aucun
*
*
* CONDITION D'UTILISATION
*   Appelee lors d'un changement d'etat de l'automate
*
* FONCTION 
*   Envoie un message a la tache Applix l'informant du changement d'etat.
*
------------------------------------------------------*/
{
  char vl_cmd[50];
  char *tl_params[10];

  static char *version = "$Id: itre_ax.c,v 1.153 2021/06/03 07:21:36 pc2dpdy Exp $ : itre_ax_Abt_Etat_Operator" ;
  
  /*trace de l'execution*/
  XZST_03EcritureTrace(XZSTC_FONCTION, "IN -- %s : %d,%d,%s,%s,%d,%d", version,
			  va_etat.Numero,         /* N³ de l'equipement */
	                  va_etat.CodeCmd,        /* Code derniere commande */
	                  va_etat.NumOper,        /* Numero de l'abonne */
	                  va_etat.Msg,            /* Message transmis */
	                  va_etat.CptRendu,       /* Compte rendu commande */
	                  va_etat.EtatCom);       /* Etat de la communication */


  /*je formate ces donnees pour Applix et je les envoie*/
  sprintf (vl_cmd, "%d|%d|%s|%d|%d", va_etat.Numero,
				     va_etat.CodeCmd,
				     va_etat.NumOper,
				     va_etat.CptRendu,
				     va_etat.EtatCom);

  /*envoi du poke a Applix*/
  tl_params[0] = vl_cmd;

  if (itre_com_SendPoke (vg_socEnvToAxMce, COM_CANAL_MTAT_ETAT_OPE, tl_params, 1,va_nomMachine) != XDC_OK)
  {
     XZST_03EcritureTrace (XZSTC_WARNING,
			    "itre_ax_Abt_Etat_Operator: erreur sur envoi poke");
     *pa_resultat = XDC_NOK;
  }
  else *pa_resultat = XDC_OK;
  
  XZST_03EcritureTrace(XZSTC_FONCTION, "OUT -- %s", version);

}	





/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Fonction appeler lors d'un changement d'etat d'un PAU
--------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static void itre_ax_Abt_Etat_PAU (XZEAT_Etat_PAU va_etat, int *pa_resultat, char *va_nomMachine)

/*
* ARGUMENTS EN ENTREE :
*   va_etat   : Structure de donnees d'etat du PAU.
*
*
* ARGUMENTS EN SORTIE :
*   pa_resultat : Compte-rendu d'execution.
*
*
* CODE RETOUR :  Aucun
*
*
* CONDITION D'UTILISATION
*   Appelee lors d'un changement d'etat d'un PAU.
*
* FONCTION 
*   Envoie un message a la tache Applix l'informant du changement d'etat.
*
------------------------------------------------------*/
{
  char vl_cmd[50];
  char *tl_params[10];

  static char *version = "$Id: itre_ax.c,v 1.153 2021/06/03 07:21:36 pc2dpdy Exp $ : itre_ax_Abt_Etat_PAU" ;
  
  /*trace de l'execution*/
  XZST_03EcritureTrace(XZSTC_FONCTION, "IN -- %s : %d,%d,%d,%d,%d,%d", version,
			  va_etat.Numero,         /* N³ de PAU */
	                  va_etat.NumPoste,       /* Numero poste operateur */
	                  va_etat.CodeCmd,        /* Code derniere commande */
	                  va_etat.CrCmd,          /* Compte rendu commande */
	                  va_etat.EtatComRAU,     /* Etat communication RAU */
	                  va_etat.EtatComGN);     /* Etat communication GN */


  /*je formate ces donnees pour Applix et je les envoie*/
  sprintf (vl_cmd, "%d|%d|%d|%d|%d|%d", va_etat.Numero,
	                                va_etat.NumPoste,
	                                va_etat.CodeCmd,
	                                va_etat.CrCmd,
	                                va_etat.EtatComRAU,
	                                va_etat.EtatComGN);

  /*envoi du poke a Applix*/
  tl_params[0] = vl_cmd;

  if (itre_com_SendPoke (vg_socEnvToAxMce, COM_CANAL_MTRA_ETAT, tl_params, 1,va_nomMachine) != XDC_OK)
  {
     XZST_03EcritureTrace (XZSTC_WARNING,
			    "itre_ax_Abt_Etat_PAU: erreur sur envoi poke");
     *pa_resultat = XDC_NOK;
  }
  else *pa_resultat = XDC_OK;
  
  XZST_03EcritureTrace(XZSTC_FONCTION, "OUT -- %s", version);
  
}	




/*************************************************
* MODULE  : MTRE				*
*************************************************
* DESCRIPTION :
* traite les messages de type EXEC envoyes par Applix
*
*************************************************
* SEQUENCE D'APPEL :
*/

extern int itre_ax_processExec(
int va_numCmd,
int va_numFct,
char *pa_arg,
char *va_nomMachine
)

/*
* PARAMETRE EN ENTREE :
*
* arg1		Description
* arg2		..........
*
* PARAMETRE EN SORTIE :
*
* VALEUR DE RETOUR :
*
* CONDITION D'ERREUR :
*
* CONDITION LIMITES :
************************************************/
{
	XZSMT_Horodate  vl_Horodate;

  char 			vl_path[100] = "";
  char 			vl_nom[200] = "";
  XDY_Datagroup 	vl_dg = "";
  XDY_Horodate		vl_horodate = 0.0;
  int			vl_numMachine = 0;
  XDY_NomMachine	vl_nomMachine = "";
  XZSCT_NomTache	vl_nomTache = "";
  XDY_Evt		vl_numEvt;
  char			vl_Module = 0;
  char			vl_Symbole = 0;
  int			vl_ModuleInt = 0;
  int			vl_Flash = 0;
  int			vl_enrichir = 0;
  XZETT_CMD_SIG_NIC     vl_cmd_GTC;
  XZETT_CMD_SIG_NIX     vl_cmd_GTCRenovee;
  XZETT_CMD_DOM_NIX     vl_Commande_SIG_NIX;
  /* REPERE1*/
  XZETT_CMD_SEQ_NIC     vl_Commande_SIG_NIC;
  /* FIN REPERE1*/
  XZENT_CMD_NAV		vl_cmd_NAV;
  XZEBT_CMD_PAL		vl_cmd_PAL;
  XZENT_CMD_SCEN	vl_cmd_SCEN;
  XZENT_CMD_MAT_NAV	vl_cmd_MAT;
  XZEPT_CMD_PMV         vl_PMV;
  XZEPT_CMD_PICTO       vl_Picto;
  XZEZT_DDE_TDP         vl_Tdp;
  /***********************************/
  XZEZT_DDE_TDP_PMVA    vl_Tdp_PMVA;
  /***********************************/

  int                   vl_NumCmd = 0;
  int			vl_NumAction = 0;
  XDY_Scenario		vl_Scenario = "";
  char			vl_Matrice[512] = "";
  int			vl_typeEvt = 0;
  int			vl_typePA = 0;
  int			vl_mode = 0;
  int 			vl_typePeriph = 0;
  int 			vl_cr = 0;
  int			vl_NumCanal = 0;
  int 			vl_cle = 0;
  int 			vl_numero = 0;
  int			vl_position = 0;
  int 			vl_modeAbonnement = 0;
  int 			vl_contraste = 0;
  int			vl_numEqt = 0;
  int			vl_typeEqt = 0;
  int 			vl_NumImage = 0;
  int 			vl_NumLT = 0;
  int			vl_NumMagneto;
  XDY_Eqt		vl_PosteOperateur = 0;
  int			vl_PO_joy = 0;
  int			vl_IdPosition = 0;
  int			vl_NumMoniteur = 0;
  int 			vl_numPoste = 0, vl_numOper = 0, vl_numProfil = 0;
  char			vl_date[30] = "";
  char			vl_reponse[30] = "";
  char			vl_arg[200] = "";
  char			vl_nomSite[20] = "";
  char                  vl_FichierLCR[256] = "";
  char                  vl_FichierResultat[256] = "";
  int			vl_Periode = 0;
  char			vl_HrDebut[20] = "";
  char			vl_HrFin[20] = "";
  XDY_Horodate		vl_HrDebutSec = 0;
  XDY_Horodate		vl_HrFinSec = 0;
  XDY_Mot 	        vl_CrLCR = 0;
  XDY_Datagroup         pl_NomDG = "";			           
  T_BOOL		vl_EtatDg = FALSE;
  XDY_NomMachine        vl_NomMachine = "";
  XDY_Entier            vl_Clign1 = 0, vl_Clign2 = 0, vl_Clign3 = 0, vl_flash = 0, vl_Cmd = 0, vl_Numero = 0;
  XDY_Entier		vl_Clign4 = 0, vl_Clign5 = 0;
  int    		vl_NumPosteOper = 0;
  int    		vl_NumPAU = 0;
  XDY_Msg_ES80          vl_Message = "";
  XDY_Tel               vl_NumTel1 = "", vl_NumTel2 = "";
  XDY_Entier            vl_Mois = 0, vl_Annee = 0;
  char			vl_Mode_Sauvegarde = '0';
  char			*pl_arg = NULL, *pl_arg1 = NULL;
  char			*pl_Pan = NULL, *pl_Pos = NULL, *pl_Type = NULL;
  int			vl_Index = 0;
  char			vl_CmdMAT[128] = "";
  int			vl_vr, vl_vm2, vl_vm1, vl_vl, vl_bau;
  int			vl_vr_i, vl_vm2_i, vl_vm1_i, vl_vl_i, vl_bau_i;
  int			vl_duree;
  float			vl_longueur;
  char                  vl_tableau_type[9000] = "";
  char                  vl_tableau_panneau[9000] = "";
  char                  vl_tableau_position[9000] = "";
  int                   vl_sequence = 0 ;
  int                   vl_format;
  int			vl_no;
  XZEPT_CMD_PMVA 	vl_PMVA ;
  XZEPT_CMD_EQT_SAGA 	vl_saga ;
  XZENT_CMD_BAF		vl_cmd_BAF;
  XZENT_CMD_BAD		vl_cmd_BAD;
  XZEPT_CMD_CFE		vl_CFE;
  XZEPT_CMD_IMU		vl_IMU;
  XZEPT_CMD_SONO	vl_SONO;

  char vl_numero_groupe[20];
  int vl_type_appel ;
  int vl_numPS=0;
  XDY_Entier vl_idComm;
  XDY_Octet vl_type_fmc;
  XDY_Entier vl_motif;
  char vl_type[50] = "";
  //char vl_dateDeb[30] = "", vl_dateFin[30] = "";
  XDY_Horodate  vl_dateDeb, vl_dateFin;
  char vl_horodateD[30] = " " , vl_horodateF[30] = " " ;

  XDY_NomMachine vl_nomPosteOperateur = "";
  XZEPT_CMD_PRV		vl_PRV;

  XZSCT_Datagroup vl_dg_mce;
  
  int vl_ava_id_mission, vl_ava_num_evt_cause, vl_ava_cle_cause, vl_ava_creation_avec_lien, vl_ava_alerte_numero, vl_ava_alerte_sit;

	/* Variables pour XZED03 */
	XDY_Eqt			vl_NumEqt;
	XDY_Mot			vl_NumVoie;
	XDY_Nom			vl_cmd;
	XDY_LongEntier	vl_Duree;

  static char *version = "$Id: itre_ax.c,v 1.153 2021/06/03 07:21:36 pc2dpdy Exp $ : itre_ax_processExec" ;
  
  XZST_03EcritureTrace(XZSTC_FONCTION, "IN -- %s : Cmd = %d   Fct = %d   arg = [%s] nomMachine = [%s]", version, va_numCmd, va_numFct, pa_arg, va_nomMachine );
  if (strcmp(va_nomMachine,""))
  {
	XDG_EncodeDG2 (vl_dg_mce, XDG_IAX_TEXTE, va_nomMachine);
	XZST_03EcritureTrace(XZSTC_WARNING,"Surcharge machine %s",va_nomMachine);
  }
  else
	strcpy (vl_dg_mce, vg_dg_mce);
  XZST_03EcritureTrace(XZSTC_WARNING, "Topic original : [%s] - Topic de reemission : [%s]", vg_dg_mce, vl_dg_mce);

  /*suivant la fonction a executer*/
  switch(va_numFct)
  {
    case (ITRE_AX_FACTIS):
      XZIS01_Positionner_Indicateur_Alarme(0);
      break;

    /*recuperation du numero du site transmis par applix*/
    case (ITRE_AX_NUM_SITE):
      /*lecture des arguments*/
      if (sscanf(pa_arg,"%d",&vg_numero_site) != 1) 
	XZST_03EcritureTrace(XZSTC_WARNING,"%s est non numerique",pa_arg);
      break;
      
    /*login effectue*/
    case (ITRE_AX_LOGINDONE):
      vg_operateurPresent=TRUE;
      break;

    /* appel de la fonction de demande de donnees individuelles*/
    case (ITRE_AX_DONINDIV):
      /*lecture des arguments*/
      if (sscanf(pa_arg,"%d|%d|%s",&vl_numero,&vl_no,vl_NomMachine) != 3)
      { XZST_03EcritureTrace(XZSTC_WARNING,"%s est non numerique",pa_arg);
      break;
      }
      vl_cr = XZER02_Demande_Individuelle (vl_numero, vl_no, vl_NomMachine);
      break;

    /*appel de la fonction reponse a la surcharge*/
    case (ITRE_AX_REPONSESURCHARGE):
      /*lecture des arguments*/
      if (sscanf(pa_arg,"%[^|]|%d",vl_arg,&vl_numero)!=2)
      { XZST_03EcritureTrace(XZSTC_WARNING,"%s est non chaine",pa_arg);
        break;
      }

      /*je leave le dg du poste PC qui a demande la surcharge*/
      /*puisque je reponds avant mon collegue du CI*/
      XDG_QuitterDG(vl_arg);
      TipcSrvFlush();

      /*emission du message*/
      sprintf(vl_reponse,"%d|%s",vl_numero,vl_arg);
      TipcSrvMsgWrite(vl_arg,
		  TipcMtLookupByNum(XDM_IAX_SENDPOKE),
		  FALSE,
		  T_IPC_FT_INT4,ITRE_AX_SENDPOKE,
		  T_IPC_FT_INT4,COM_CANAL_MTMC_SURCHARGE,
		  T_IPC_FT_STR,vl_reponse,
		  NULL);
      TipcSrvFlush();
      break;


    /* appel de la fonction fin de surcharge*/
    case (ITRE_AX_FINSURCHARGE):
      /*lecture des arguments*/
      if (sscanf(pa_arg,"%[^|]|%d|%d",vl_dg,
					&vl_numero,
					&vl_cle)!=3)
      { XZST_03EcritureTrace(XZSTC_WARNING,"%s est non numerique",pa_arg);
        break;
      }

      sprintf(vl_reponse,"%d|%d",vl_numero,vl_cle);
      /*emisssion du message*/
      TipcSrvMsgWrite(vl_dg,
			TipcMtLookupByNum(XDM_IAX_SENDPOKE),
			FALSE,
			T_IPC_FT_INT4,ITRE_AX_SENDPOKE,
			T_IPC_FT_INT4,COM_CANAL_MTLT_FINSURCHARGE,
			T_IPC_FT_INT4,0,
			NULL);
      TipcSrvFlush();
      break;

    /* appel de la fonction demande de surcharge*/
    case (ITRE_AX_DEMANDESURCHARGE):
      /*lecture des arguments*/
      if (sscanf(pa_arg,"%d|%d|%[^|]|%[^|]|%d|%d|%s",&vl_numero,
					&vl_cle,
					vl_nom,
					vl_nomSite,
					&vl_numPoste,
					&vl_enrichir,
					vl_reponse)!=7)
      { XZST_03EcritureTrace(XZSTC_WARNING,"%s est non numerique",pa_arg);
        break;
      }

      /*je passe mon datagroup pour la reponse*/ 
      XDG_EncodeDG2(vl_dg,XDG_IREPONSE_SURCHARGE,vg_nomMachine);
      pl_arg1=strcat(vl_dg,"|");
      pl_arg=strcat(pl_arg1,pa_arg);
      strcpy(vl_arg,pl_arg);

      /*je dis aux postes du CI de s'abonner au dg de reponse*/
      strcpy(vl_nom,vl_dg);
      XDG_EncodeDG2(vl_dg,XDG_IDEMANDE_SURCHARGE,XDC_NOM_SITE_CI);
      TipcSrvMsgWrite(vl_dg,
			TipcMtLookupByNum(XDM_IAX_JOINDG),
			FALSE,
			T_IPC_FT_INT4,ITRE_AX_JOINDG,
			T_IPC_FT_STR,vl_nom,
			NULL);
      TipcSrvFlush();

      /*emisssion du message*/
      TipcSrvMsgWrite(vl_dg,
			TipcMtLookupByNum(XDM_IAX_NEWTASK),
			FALSE,
			T_IPC_FT_INT4,ITRE_AX_NEWTASK,
			T_IPC_FT_STR,"ITMA_TMC_reponse_surcharge",
			T_IPC_FT_STR,vl_arg,
			T_IPC_FT_INT4,0,
			NULL);
      TipcSrvFlush();
      break;


    /*** appel de la fonction XZEX01 ( j.c 22/12/94 ) ***/
    case (ITRE_AX_XZEX01):
    
      /** lecture des arguments **/
      if (sscanf(pa_arg,"%d|%d|%[^|]|%[^|]|%d|%[^|]|%[^|]|%s",&vl_typeEqt,
                                                  &vl_numEqt,
                                                  vl_FichierLCR,
                                                  vl_FichierResultat,
			                          &vl_Periode,
			                          vl_HrDebut,
			                          vl_HrFin,
			                          vl_nomMachine)!=8)
			                          
      { XZST_03EcritureTrace(XZSTC_WARNING,"%s est non numerique",pa_arg);
        break; }
        
	/*A Recuperation du nom de la machine */
	if ( XZSC_07NomMachine(vl_NomMachine) != XDC_OK)
	{
	   /* Ecriture Trace */
	   XZST_03EcritureTrace( XZSTC_WARNING, "Recuperation du Nom de la machine impossible ." );
	} 

	/*A Abonnement au Datagroup XDG_FEN_LCR_<NomMachine> */
	   /*A Construction du nom du DataGroup */
	   sprintf ( pl_NomDG, "%s_%s", XDG_ECPT_LCR, vl_NomMachine );
	   
	/* Chargement de la machine pour le retour LCR */
	strcpy(vg_nomMachineLCR,va_nomMachine);
	XZST_03EcritureTrace( XZSTC_WARNING, "ITRE_AX_XZEX01 retour lcr arm� sur va_nomMachine:%s",vg_nomMachineLCR);

   	/*A Joindre ce datagroup s'il n'est pas deja joint */
   	TipcSrvDgGetRecv ( pl_NomDG, &vl_EtatDg );
   	if ( !vl_EtatDg )
   	{
		/*A  Armement Callback sur Reception XDM_ECPT_LCR */
		if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_ECPT_LCR),
			itre_ax_retour_lcr,(T_CB_ARG)vg_nomMachineLCR) == NULL) 
		{
			/* Ecriture Trace */
			XZST_03EcritureTrace( XZSTC_WARNING, "Callback sur XDM_ECPT_LCR non declenchee.");
		}
		if ( !XDG_JoindreDG( pl_NomDG ) )
		{
			XZST_03EcritureTrace ( XZSTC_WARNING, "Echec abonnement au Datagroup %s ", pl_NomDG );
		}
	}


       if ( vl_Periode > 0 )
       {
          XZSM_13ConversionHorodateSec ( vl_HrDebut, &vl_HrDebutSec );
          XZSM_13ConversionHorodateSec ( vl_HrFin, &vl_HrFinSec );
       }
      /** on lance XZEX01 **/
      XZST_03EcritureTrace ( XZSTC_INFO, "Nom Machine LT:%s Type:%d Num:%d Cmd:%s Res:%s Periode:%d deb:%lf fin:%lf", 
                             vl_nomMachine, 
                             vl_typeEqt,
                             vl_numEqt,
                             vl_FichierLCR,
                             vl_FichierResultat,
                             vl_Periode,
                             vl_HrDebutSec,
                             vl_HrFinSec);
      if (XZEX01_Demande_LCR(vl_typeEqt,
                             vl_numEqt,
                             vl_FichierLCR,
                             vl_FichierResultat,
                             vl_Periode,
                             vl_HrDebutSec,
                             vl_HrFinSec,
                             vl_nomMachine,
			     itre_ax_timeout_lcr )!=XDC_OK)
                             
      { XZST_03EcritureTrace(XZSTC_WARNING,"tache relais: erreur dans XZEX01");
	break; }
      break;




    /*** lancement du calcul des equations evenements ( J.P.L 29/01/95 ) ***/
    case (ITRE_AX_XZAA10):
    
      /** lecture des arguments **/
      if (sscanf (pa_arg, "%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%f|%[^|]|%d",
				&vl_typeEvt,
				&vl_vr,
				&vl_vm2,
				&vl_vm1,
				&vl_vl,
				&vl_bau,
				&vl_vr_i,
				&vl_vm2_i,
				&vl_vm1_i,
				&vl_vl_i,
				&vl_bau_i,
				&vl_duree,
				&vl_longueur,
				vl_date,
				&vl_numMachine) != 15)
      { XZST_03EcritureTrace(XZSTC_WARNING,
		"Liste d'arguments invalide pour XZAA10 : \"%s\"\n", pa_arg);
        break; }

      /** transformation de la date en numerique **/
      XZSM_13ConversionHorodateSec(vl_date,&vl_horodate);

/*
fprintf(stderr, "Appel XZAA10 (%d, %d, %d, %d, %d, %d,\n              %d, %d, %d, %d, %d,\n              %f, %f, %lf, %d)\n\n",
vl_typeEvt, vl_vr, vl_vm2, vl_vm1, vl_vl, vl_bau,
vl_vr_i, vl_vm2_i, vl_vm1_i, vl_vl_i, vl_bau_i,
(float) vl_duree, (float) vl_longueur, vl_horodate, vl_numMachine);
*/

      /** appel de la fonction indiquee **/
      if (xzaa10_Equation ((XDY_Octet) vl_typeEvt,
			   (XDY_Entier) vl_vr,
			   (XDY_Entier) vl_vm2,
			   (XDY_Entier) vl_vm1,
			   (XDY_Entier) vl_vl,
			   (XDY_Entier) vl_bau,
			   (XDY_Entier) vl_vr_i,
			   (XDY_Entier) vl_vm2_i,
			   (XDY_Entier) vl_vm1_i,
			   (XDY_Entier) vl_vl_i,
			   (XDY_Entier) vl_bau_i,
			   (float) vl_duree,
			   (float) (vl_longueur/1000),
			   vl_horodate,
			   (XDY_Eqt) vl_numMachine) != XDC_OK)
      { XZST_03EcritureTrace (XZSTC_WARNING, "Erreur dans XZAA10\n");
	break; }
	
      break;




       /*** appel d'une fonction PAU XZEUnn ( J.P.L 06/01/95 ) ***/
    case (ITRE_AX_XZEU00):
    
      /** lecture des arguments **/
      if (sscanf (pa_arg, "%d|%d|%d|%s", &vl_NumCmd,
					 &vl_NumPosteOper,
					 &vl_NumPAU,
					 vl_nomSite) != 4)
      { XZST_03EcritureTrace(XZSTC_WARNING,
		"Liste d'arguments invalide pour XZEUnn : \"%s\"\n", pa_arg);
        break; }

      /** appel de la fonction indiquee **/
      switch (vl_NumCmd) {
		case ITRE_AX_XZEU01 :
		vl_cr = XZEU01_Demande_Mise_En_Comm (vl_nomSite, vl_NumPAU, vl_NumPosteOper);
		break;

		case ITRE_AX_XZEU02 :
		XZST_03EcritureTrace(XZSTC_WARNING,"Liste d'arguments:\"%s\"\n%s;%d;%d;", pa_arg,vl_nomSite, vl_NumPAU, vl_NumPosteOper);

		vl_cr = XZEU02_Appel_Borne (vl_nomSite, vl_NumPAU, vl_NumPosteOper);
		break;

		case ITRE_AX_XZEU03 :
		vl_cr = XZEU03_Liberation_Ligne(vl_nomSite, vl_NumPAU, vl_NumPosteOper);
		break;

		case ITRE_AX_XZEU04 :
		vl_cr = XZEU04_Mise_En_Garde (vl_nomSite, vl_NumPAU);
		break;

		case ITRE_AX_XZEU05 :
		vl_cr = XZEU05_Reprise_Garde (vl_nomSite, vl_NumPAU, vl_NumPosteOper);
		break;

		case ITRE_AX_XZEU06 :
		vl_cr = XZEU06_Transfert_GN (vl_nomSite, vl_NumPAU);
		break;

		case ITRE_AX_XZEU07 :
		vl_cr = XZEU07_Demande_Prise_En_Main_GN (vl_nomSite, vl_NumPAU);
		break;

		case ITRE_AX_XZEU08 :
		vl_cr = XZEU08_Test_Unitaire (vl_nomSite, vl_NumPAU, vl_NumPosteOper);
		break;

		case ITRE_AX_XZEU09 :
		vl_cr = XZEU09_Test_Semi_Auto (vl_nomSite, vl_NumPAU, vl_NumPosteOper);
		break;

		case ITRE_AX_XZEU10 :
		XZST_03EcritureTrace (XZSTC_WARNING,"appel XZEU10_Demande_Etat_Bascul_RAU avec %s",
					vl_nomSite);
		vl_cr = XZEU10_Demande_Etat_Bascul_RAU(vl_nomSite);
		break;
      }

      if (vl_cr != XDC_OK)
      { 
			XZST_03EcritureTrace (XZSTC_WARNING,"Tache relais: erreur dans XZEUnn\n");
			break; 
	  }
	
      break;


    case (ITRE_AX_XZEU11):
      /** lecture des arguments **/
      if (sscanf (pa_arg, "%d|%[^|]|%s",
      		&vl_NumCmd, /*site*/
			vl_nomSite, /*zone*/
			vl_nomMachine) != 3) 
	  { /*etat*/
			XZST_03EcritureTrace(XZSTC_WARNING, "Liste d'arguments invalide pour XZEU11 : \"%s\"\n", pa_arg);
			break;	
      }
      if (XZEU11_Bascul_RAU(vl_NumCmd,vl_nomSite,vl_nomMachine) != XDC_OK) 
	  {
			XZST_03EcritureTrace(XZSTC_WARNING,"tache relais: erreur dans XZEU11");
      }
      break;

    /*** appel d'une fonction de reconfig machine XZECnn ( J.P.L 27/01/95 ) ***/
    case (ITRE_AX_XZEC00):
    
      /** lecture des arguments **/
      if (sscanf (pa_arg, "%d|%[^|]|%[^|]|%d|%d", &vl_NumCmd,
						  vl_nomSite,
						  vl_nomMachine,
						  &vl_typeEqt,
						  &vl_mode) != 5)
      { 
		  	XZST_03EcritureTrace(XZSTC_WARNING, "Liste d'arguments invalide pour XZECnn : \"%s\"\n", pa_arg);
        	break; 
	  }

      /** appel de la fonction indiquee **/
      switch (vl_NumCmd) 
	  {
      	case ITRE_AX_XZEC01 :
			vl_cr = XZEC01_Config (vl_nomSite, vl_nomMachine, vl_typeEqt);
			break;

      	case ITRE_AX_XZEC05 :
			vl_cr = XZEC05_Cmd_TacheTmp_Eqt (vl_nomSite, vl_nomMachine, vl_typeEqt, vl_mode);
			break;
      }

      if (vl_cr != XDC_OK)
      { 
			XZST_03EcritureTrace (XZSTC_WARNING,"Tache relais: erreur dans XZECnn\n");
			break; 
	  }
	
      break;




    /*** appel de la fonction XZEX02 ( j.c 22/12/94 ) ***/
    case (ITRE_AX_XZEX02):
    
      /** lecture des arguments **/
      if (sscanf(pa_arg,"%d|%s",&vl_CrLCR,
			        vl_nomMachine)!=2)
			                          
      { 
		  	XZST_03EcritureTrace(XZSTC_WARNING,"%s est non numerique",pa_arg);
        	break; 
	  }

      /** on lance XZEX02 **/
      if (XZEX02_Retour_LCR(vl_CrLCR, vl_nomMachine)!=XDC_OK)
      {
			XZST_03EcritureTrace(XZSTC_WARNING,"tache relais: erreur dans XZEX02");
		break; 
	  }
      break;




 



    /*** appel de la fonction Jour / Nuit ( j.c 14/04/95 ) ***/
    case (ITRE_AX_Jour_Nuit):
    
      /** lecture des arguments **/
      if (sscanf(pa_arg,"%d|%s",&vl_mode,
			        vl_nomSite)!=2)
			                          
      { XZST_03EcritureTrace(XZSTC_WARNING,"%s est non numerique",pa_arg);
        break; }

      XDG_EncodeDG2 ( vl_dg, XDG_ETAT_JOURNUIT, vl_nomSite) ;
      			
      if (!TipcSrvMsgWrite ( 	vl_dg,
      			     	XDM_IdentMsg(XDM_ETAT_JourNuit),
                          	XDM_FLG_SRVMSGWRITE,
                          	T_IPC_FT_INT2, (vl_mode==1) ? XDC_NUIT : XDC_JOUR,
                          	NULL) )
                             
	{	XZST_03EcritureTrace(XZSTC_WARNING
 		,"Envoi message XDM_ETAT_JourNuit non effectue \n" );
	}
	else
	{	XZST_03EcritureTrace(XZSTC_WARNING
 		,"Envoi message XDM_ETAT_JourNuit effectue \n" );
    }
         
    break;




   /*** appel de la fonction XZEC02 ( j.c 20/10/94 ) ***/
    case (ITRE_AX_XZEC02):
    
      /** lecture des arguments **/
      if (sscanf(pa_arg,"%d|%d|%d|%s",&vl_contraste,
                                      &vl_numEqt,
			              &vl_typeEqt,
			              vl_nomSite)!=4)
			              
      { 
			XZST_03EcritureTrace(XZSTC_WARNING,"%s est non numerique",pa_arg);
        	break; 
	  }

      /** on lance XZEC02 **/
      if (XZEC02_Commande_Contraste(vl_contraste,
      				    vl_numEqt,
      				    vl_typeEqt,
				    vl_nomSite)!=XDC_OK)
				    
      { 
			XZST_03EcritureTrace(XZSTC_WARNING,"tache relais: erreur dans XZEC02");
			break; 
	  }
      break;





    /*** appel de la fonction XZEV01 ( j.c 20/10/94 ) ***/
    case (ITRE_AX_XZEV01):
    
      /** lecture des arguments **/
      if (sscanf(pa_arg,"%[^|]|%d|%d|%d",vl_nomSite,
      				         &vl_NumCanal,
      				         &vl_NumMoniteur,
        			         &vl_NumLT)!=4)
      { 
			XZST_03EcritureTrace(XZSTC_WARNING,"%s est non chaine",pa_arg);
        	break; 
	  }

      /** appel a XZEV01 **/
      if (XZEV01_Affecte_Canal_Moniteur(vl_nomSite,
     				        vl_NumCanal,
     				        vl_NumMoniteur,
      				        vl_NumLT)!=XDC_OK)
      { 
			XZST_03EcritureTrace(XZSTC_WARNING,"tache relais: erreur dans XZEV01\n");
			break; 
	  }
	
      break;




  
    /*** appel de la fonction XZEV02 ( j.c 20/10/94 ) ***/
    case (ITRE_AX_XZEV02):
    
      /** lecture des arguments **/
      if (sscanf(pa_arg,"%[^|]|%d|%d|%d",vl_nomSite,
      				         &vl_NumImage,
      				         &vl_NumMoniteur,
        			         &vl_PO_joy)!=4)
      { 
			XZST_03EcritureTrace(XZSTC_WARNING,"%s est non chaine",pa_arg);
        	break; 
	  }

      /*appel a XZEV02*/
      if (XZEV02_Affecte_Image_Monit_Joy(vl_nomSite,
     					 vl_NumImage,
     					 vl_NumMoniteur,
      					 vl_PO_joy)!=XDC_OK)
      { 
			XZST_03EcritureTrace(XZSTC_WARNING,"tache relais: erreur dans XZEV02\n");
			break; 
	  }
	
      break;




    /*** appel de la fonction XZEV03 ( j.c 20/10/94 ) ***/
    case (ITRE_AX_XZEV03):
    
      /** lecture des arguments **/
      if (sscanf(pa_arg,"%[^|]|%d|%d|%d",vl_nomSite,
      				         &vl_NumImage,
      				         &vl_NumMoniteur,
        			         &vl_IdPosition)!=4)
      { 
			XZST_03EcritureTrace(XZSTC_WARNING,"%s est non chaine",pa_arg);
        	break; 
	  }

      /** appel a XZEV03 **/
      if (XZEV03_Positionnement(vl_nomSite,
     				vl_NumImage,
     				vl_NumMoniteur,
      				vl_IdPosition)!=XDC_OK)
      { 
		  	XZST_03EcritureTrace(XZSTC_WARNING,"tache relais: erreur dans XZEV03\n");
			break; 
	  }
	
      break;



    /*** appel de la fonction XZEV04 ( j.c 20/10/94 ) ***/
    case (ITRE_AX_XZEV04):
    
      /** lecture des arguments **/
      if (sscanf(pa_arg,"%[^|]|%d|%d",vl_nomSite,
      				      &vl_NumImage,
      				      &vl_IdPosition)!=3)
      { 
		  	XZST_03EcritureTrace(XZSTC_WARNING,"%s est non chaine",pa_arg);
        	break; 
	  }

      /** appel a XZEV04 **/
      if (XZEV04_Memorise_Position(vl_nomSite,
     				   vl_NumImage,
      				   vl_IdPosition)!=XDC_OK)
      { 
		  	XZST_03EcritureTrace(XZSTC_WARNING,"tache relais: erreur dans XZEV04\n");
			break; 
	  }
	
      break;



    /*** appel de la fonction XZEV05 ( j.c 20/10/94 ) ***/
    case (ITRE_AX_XZEV05):
    
      /** lecture des arguments **/
      if (sscanf(pa_arg,"%[^|]|%d",vl_nomSite,
      			           &vl_NumMoniteur)!=2)
      { 
		  	XZST_03EcritureTrace(XZSTC_WARNING,"%s est non chaine",pa_arg);
        	break; 
	  }

      /** appel a XZEV05 **/
      if (XZEV05_Eteint_Moniteur(vl_nomSite,
				 vl_NumMoniteur)!=XDC_OK)
      { 
		  	XZST_03EcritureTrace(XZSTC_WARNING,"tache relais: erreur dans XZEV05\n");
			break; 
	  }
	
      break;





    /*** appel de la fonction XZEV06 ( j.c 20/10/94 ) ***/
    case (ITRE_AX_XZEV06):
    
      /** lecture des arguments **/
      if (sscanf(pa_arg,"%[^|]|%d|%d|%d|%d",vl_nomSite,
      			              	&vl_NumCmd,
      			              	&vl_NumImage,
      			              	&vl_NumMagneto,
      			              	&vl_NumMoniteur)!=5)
      { 
		  	XZST_03EcritureTrace(XZSTC_WARNING,"%s est non chaine",pa_arg);
        	break; 
	  }

      /** appel a XZEV06 **/
      if (XZEV06_Commande_Magneto(	vl_nomSite,
			       		vl_NumCmd,
			       		vl_NumImage,
			       		vl_NumMagneto,
			       		vl_NumMoniteur)!=XDC_OK)
      { 
		  	XZST_03EcritureTrace(XZSTC_WARNING,"tache relais: erreur dans XZEV06\n");
			break; 
	  }
	
      break;




      
    /*** appel de la fonction XZEN01 ***/
    case (ITRE_AX_XZEN01):
    
      /** lecture des arguments **/
      if (sscanf(pa_arg,"%d|%d|%[^|]|%d|%d|%s",&vl_numEqt,
      					       &vl_ModuleInt,
      					       vl_cmd_NAV.Symb,
      					       &vl_Flash,
      					       &vl_NumAction,
      					       vl_nomMachine)!=6)
      				
      { 
		  	XZST_03EcritureTrace(XZSTC_WARNING,"%s n'est pas une chaine",pa_arg);
        	break; 
	  }

  
      vl_cmd_NAV.NoEqt = vl_numEqt;
      vl_cmd_NAV.Flash = vl_Flash;
      vl_cmd_NAV.Module = vl_ModuleInt;
      
      XZST_03EcritureTrace(XZSTC_INTERFACE,"Appel XZEN01: E:%d M:%d S:%s F:%d <%s>",
      						vl_cmd_NAV.NoEqt,
      						vl_cmd_NAV.Module,
      						vl_cmd_NAV.Symb,
      						vl_cmd_NAV.Flash,
      						vl_nomMachine );
      
      /** appel a XZEN01 **/
      if (XZEN01_Commande_NAV(vl_cmd_NAV,
      			      vl_NumAction,
      			      vl_nomMachine)!=XDC_OK)
      			      
      { 
		  	XZST_03EcritureTrace(XZSTC_WARNING,"tache relais: erreur dans XZEN01\n");
			break; 
	  }
	
      break;




    /*** appel de la fonction XZEN10 ***/
    case (ITRE_AX_XZEN10):
    
      /** lecture des arguments **/
      if (sscanf(pa_arg,"%d|%d|%[^|]|%d|%d|%s",&vl_numEqt,
      					       &vl_ModuleInt,
      					       vl_cmd_BAF.Message,
      					       &vl_Flash,
      					       &vl_NumAction,
      					       vl_nomMachine)!=6)
      				
      { 
		  	XZST_03EcritureTrace(XZSTC_WARNING,"%s n'est pas une chaine",pa_arg);
        	break; 
	  }

  
      vl_cmd_BAF.NoEqt = vl_numEqt;
      vl_cmd_BAF.Cligno = vl_Flash;
      vl_cmd_BAF.Module = vl_ModuleInt;
      
      XZST_03EcritureTrace(XZSTC_INTERFACE,"Appel XZEN10: E:%d M:%d S:%s F:%d <%s>",
      						vl_cmd_BAF.NoEqt,
      						vl_cmd_BAF.Module,
      						vl_cmd_BAF.Message,
      						vl_cmd_BAF.Cligno,
      						vl_nomMachine );
      
      /** appel a XZEN10 **/
      if (XZEN10_Commande_BAF(vl_cmd_BAF,
      			      vl_NumAction,
      			      vl_nomMachine)!=XDC_OK)
      			      
      { 
		  	XZST_03EcritureTrace(XZSTC_WARNING,"tache relais: erreur dans XZEN10\n");
			break; 
	  }
	
      break;




    /*** appel de la fonction XZEN11 ***/
    case (ITRE_AX_XZEN11):
    
      /** lecture des arguments **/
      if (sscanf(pa_arg,"%d|%d|%[^|]|%d|%d|%s",&vl_numEqt,
      					       &vl_ModuleInt,
      					       vl_cmd_BAD.Message,
      					       &vl_Flash,
      					       &vl_NumAction,
      					       vl_nomMachine)!=6)
      				
      { 
		  	XZST_03EcritureTrace(XZSTC_WARNING,"%s n'est pas une chaine",pa_arg);
        	break; 
	  }

  
      vl_cmd_BAD.NoEqt = vl_numEqt;
      vl_cmd_BAD.Cligno = vl_Flash;
      vl_cmd_BAD.Module = vl_ModuleInt;
      
      XZST_03EcritureTrace(XZSTC_INTERFACE,"Appel XZEN11: E:%d M:%d S:%s F:%d <%s>",
      						vl_cmd_BAD.NoEqt,
      						vl_cmd_BAD.Module,
      						vl_cmd_BAD.Message,
      						vl_cmd_BAD.Cligno,
      						vl_nomMachine );
      
      /** appel a XZEN11 **/
      if (XZEN11_Commande_BAD(vl_cmd_BAD,
      			      vl_NumAction,
      			      vl_nomMachine)!=XDC_OK)
      			      
      { 
		  	XZST_03EcritureTrace(XZSTC_WARNING,"tache relais: erreur dans XZEN11\n");
			break; 
	  }
	
      break;




     /*** appel de la fonction XZEN02 ***/
    case (ITRE_AX_XZEN02):
    
      XZST_03EcritureTrace(XZSTC_WARNING,"tache relais: Passage dans XZEN02");
      
      /** lecture des arguments **/
      if (sscanf(pa_arg,"%d|%[^|]|%d|%s",&vl_numEqt,
      				         vl_cmd_SCEN.Scenar,
      				         &vl_NumAction,
      				         vl_nomMachine)!=4)
      				      
      { 
		  	XZST_03EcritureTrace(XZSTC_WARNING,"%s n'est pas une chaine",pa_arg);
        	break; 
	  }
        
      vl_cmd_SCEN.NoEqt = vl_numEqt;
      
      XZST_03EcritureTrace(XZSTC_INTERFACE,"Appel XZEN02: E:%d S:%s A:%d <%s>",
      						vl_cmd_SCEN.NoEqt,
      						vl_cmd_SCEN.Scenar,
      						vl_NumAction,
      						vl_nomMachine );

      /** appel a XZEN02 **/
      if (XZEN02_Commande_SCEN(vl_cmd_SCEN,
      			       vl_NumAction,
      			       vl_nomMachine)!=XDC_OK)
      			      
      { 
		  	XZST_03EcritureTrace(XZSTC_WARNING,"tache relais: erreur dans XZEN02\n");
			break; 
	  }
	
      break;




     /*** appel de la fonction XZEN13 ***/
    case (ITRE_AX_XZEN13):
    
      XZST_03EcritureTrace(XZSTC_WARNING,"tache relais: Passage dans XZEN13");
      
      /** lecture des arguments **/
      if (sscanf(pa_arg,"%d|%[^|]|%d|%s",&vl_numEqt,
      				         vl_cmd_SCEN.Scenar,
      				         &vl_NumAction,
      				         vl_nomMachine)!=4)
      				      
      { 
		  	XZST_03EcritureTrace(XZSTC_WARNING,"%s n'est pas une chaine",pa_arg);
        	break; 
	  }
        
      vl_cmd_SCEN.NoEqt = vl_numEqt;
      
      XZST_03EcritureTrace(XZSTC_INTERFACE,"Appel XZEN13: E:%d S:%s A:%d <%s>",
      						vl_cmd_SCEN.NoEqt,
      						vl_cmd_SCEN.Scenar,
      						vl_NumAction,
      						vl_nomMachine );
      /** appel a XZEN13 **/
/* MODIF PFE CAR IMPOSSIBLE DE RECOMPILER TITMO ET TITBI MERCI JEAN MI MI      if (XZEN13_Commande_SCEN_BAD(vl_cmd_SCEN,
      			       vl_NumAction,
      			       vl_nomMachine)!=XDC_OK)
      			      
      { XZST_03EcritureTrace(XZSTC_WARNING,"tache relais: erreur dans XZEN13\n");
	break; }
*/	
      break;




      /*** appel de la fonction XZEN03 ***/
    case (ITRE_AX_XZEN03):
    
      /** lecture des arguments **/
      if (sscanf(pa_arg,"%d|%[^|]|%[^|]|%d|%[^|]|%s",&vl_numEqt,
 				         vl_cmd_MAT.Matrice,
      				         vl_Scenario,
      				         &vl_NumAction,
      				         vl_nomMachine,
      				         vl_CmdMAT)!=6)

      { 
		  	XZST_03EcritureTrace(XZSTC_WARNING,"%s n'est pas une chaine",pa_arg);
        	break; 
	  }
      				         
       vl_cmd_MAT.NoEqt = vl_numEqt;
      				
      /** DÅcripter la ligne de la matrice ici */
      for ( /*strtok ( vl_CmdMAT, "|" ),*/ vl_Index = 0; vl_Index < XZENC_Nb_Params_Matrice; vl_Index++ )
      {
         vl_cmd_MAT.ListeParam[vl_Index] = atoi ( strtok((vl_Index == 0) ? vl_CmdMAT: NULL,"|") );
      }			       

      /** appel a XZEN03 **/
      if (XZEN03_Config_Matrice_Test(vl_cmd_MAT,
      			             vl_NumAction,
      			             vl_nomMachine)!=XDC_OK)
      			       
      			      
      { 
		  	XZST_03EcritureTrace(XZSTC_WARNING,"tache relais: erreur dans XZEN03\n");
			break; 
	  }
	
      break;




    /*** appel de la fonction XZEN04 ***/
    case (ITRE_AX_XZEN04):
    
      /** lecture des arguments **/
      if (sscanf(pa_arg,"%d|%d|%s",&vl_numEqt,
      				   &vl_NumAction,
      				   vl_nomMachine)!=3)
      				
      { 
		  	XZST_03EcritureTrace(XZSTC_WARNING,"%s n'est pas une chaine",pa_arg);
        	break; 
	  }

      /** appel a XZEN04 **/
      if (XZEN04_Arret_SCEN(  vl_numEqt,
      			      vl_NumAction,
      			      vl_nomMachine)!=XDC_OK)
      			      
      { 
		  	XZST_03EcritureTrace(XZSTC_WARNING,"tache relais: erreur dans XZEN04\n");
			break; 
	  }
	
      break;




    /*** appel de la fonction XZEN15 ***/
    case (ITRE_AX_XZEN15):
    
      /** lecture des arguments **/
      if (sscanf(pa_arg,"%d|%d|%s",&vl_numEqt,
      				   &vl_NumAction,
      				   vl_nomMachine)!=3)
      				
      { 
		  	XZST_03EcritureTrace(XZSTC_WARNING,"%s n'est pas une chaine",pa_arg);
        	break; 
	  }

      /** appel a XZEN15 **/
      if (XZEN15_Arret_SCEN_BAD(  vl_numEqt,
      			      vl_NumAction,
      			      vl_nomMachine)!=XDC_OK)
      			      
      { 
		  	XZST_03EcritureTrace(XZSTC_WARNING,"tache relais: erreur dans XZEN15\n");
			break; 
	  }
	
      break;





    /*** appel de la fonction XZEB01 ***/
    case (ITRE_AX_XZEB01):
      /** lecture des arguments **/
      /** lecture des arguments **/
      if (sscanf(pa_arg,"%d|%[^|]|%d|%s",   &vl_numEqt,
      				            vl_tableau_position,
      				            &vl_NumAction,
      					    vl_nomMachine)!=4)

      { 
		  	XZST_03EcritureTrace(XZSTC_WARNING,"XZEB01 %s n'est pas une chaine",pa_arg);
        	break; 
	  }
     		         
      XZST_03EcritureTrace(XZSTC_DEBUG1,"XZEB01 No:%d pos:%s act:%d %s",vl_numEqt,
      				            vl_tableau_position,
      				            vl_NumAction,
      					    vl_nomMachine);
      				
      /** DÅcripter la ligne du tableau de panneau ici */
      pl_Pan = strtok ( vl_tableau_position, "," );
      strcpy ( vl_cmd_PAL.Scenario, pl_Pan );
      XZST_03EcritureTrace(XZSTC_DEBUG1,"XZEB01 pan:%s", vl_cmd_PAL.Scenario );
  
      vl_cmd_PAL.Numero = vl_numEqt;
      
      
      /** appel a XZEB01 **/
      if (XZEB01_Commande_PAL(vl_cmd_PAL,
      			      vl_NumAction,
      			      vl_nomMachine)!=XDC_OK)
      			      
      { 
		  	XZST_03EcritureTrace(XZSTC_WARNING,"tache relais: erreur dans XZEB01\n");
			break; 
	  }
	
      break;







      /*** appel de la fonction XZET02 ***/
    case (ITRE_AX_XZET02):
		switch (va_numCmd)
		{
			/** commande unitaire pour l'etat echangeur du domaine signalisation **/
			case(ITRE_AX_DOMAINE_SIG) :
			
					/** lecture des arguments **/
			      if (sscanf(pa_arg,"%d|%[^|]|%[^|]|%[^|]|%d",&vl_numEqt,
											vl_tableau_type,
											vl_tableau_panneau,
			      				            vl_tableau_position,
			      				            &vl_sequence)!=5)

			      { XZST_03EcritureTrace(XZSTC_WARNING,"%s n'est pas une chaine",pa_arg);
			        break; }
			      
// 			      XZST_03EcritureTrace(XZSTC_MESSAGE,"XZET02->XZET05 Cmde Unitaire Echangeur Domaine SIG No:%d Eqt:%s pos:%s seq:%d",vl_numEqt,
// 			 				                  vl_tableau_panneau,
// 			      				            vl_tableau_position,
// 			      				            vl_sequence);

			      XZST_03EcritureTrace(XZSTC_INFO,"XZET05 No:%d Eqt:%s pos:%s seq:%d (max = %d)",vl_numEqt,
			 				                  vl_tableau_panneau,
			      				            vl_tableau_position,
			      				            vl_sequence,
											XZETC_MAX_UNITAIRE);
			      				
			      /** type */
			      pl_Type = strtok ( vl_tableau_type, "," );
			      for ( vl_Index = 0; vl_Index < XZETC_MAX_UNITAIRE; vl_Index++ )
			      {
			         if (pl_Type != NULL)  vl_cmd_GTCRenovee.NumTypeEqt[vl_Index] = atoi ( pl_Type );
			         pl_Type = strtok (NULL,",");
			         XZST_03EcritureTrace(XZSTC_DEBUG1,"XZET05 Eqt:%d",vl_cmd_GTCRenovee.NumTypeEqt[vl_Index]);
				  }

			      /** DÅcripter la ligne du tableau de panneau ici */
			      pl_Pan = strtok ( vl_tableau_panneau, "," );
			      for ( vl_Index = 0; vl_Index < XZETC_MAX_UNITAIRE; vl_Index++ )
			      {
			         if (pl_Pan != NULL)  vl_cmd_GTCRenovee.NumEqt[vl_Index] = atoi ( pl_Pan );
			         pl_Pan = strtok (NULL,",");
			         XZST_03EcritureTrace(XZSTC_DEBUG1,"XZET05 Eqt:%d",vl_cmd_GTCRenovee.NumEqt[vl_Index]);
			      }			       

			      /** DÅcripter la ligne du tableau de position ici */
			      pl_Pos = strtok ( vl_tableau_position, "," );
			      for ( vl_Index = 0; vl_Index < XZETC_MAX_UNITAIRE; vl_Index++ )
			      {
			         if (pl_Pos != NULL)  vl_cmd_GTCRenovee.NumCmd[vl_Index] = atoi ( pl_Pos );
			         pl_Pos = strtok (NULL,",");
			         XZST_03EcritureTrace(XZSTC_INFO,"XZET05 pos:%d",vl_cmd_GTCRenovee.NumCmd[vl_Index]);
			      }			       

			      vl_cmd_GTCRenovee.NumTE = vl_numEqt;
			      vl_cmd_GTCRenovee.NumSeq = vl_sequence;
			      vl_NumCmd = 0 ;

			      /** appel a la fonction commande unitaire signalisation**/
			      if (XZET05_Commande_SIG(vl_cmd_GTCRenovee, vl_NumCmd)!=XDC_OK)

			      { XZST_03EcritureTrace(XZSTC_WARNING,"tache relais: erreur dans XZET05");
				break; }
			break;
			
			/** commande unitaire pour l'etat echangeur du domaine ventilation **/
			case(ITRE_AX_DOMAINE_VEN) :
			
					/** lecture des arguments **/
			      if (sscanf(pa_arg,"%d|%[^|]|%[^|]|%[^|]|%d",
											&vl_numEqt,
											vl_tableau_type,
											vl_tableau_panneau,
			      				            vl_tableau_position,
			      				            &vl_sequence)!=5)

			      { XZST_03EcritureTrace(XZSTC_WARNING,"%s n'est pas une chaine",pa_arg);
			        break; }
			     		         
// 			      XZST_03EcritureTrace(XZSTC_MESSAGE,"XZET02->XZET07 Cmde Unitaire Echangeur Domaine VEN No:%d Eqt:%s pos:%s seq:%d",vl_numEqt,
// 			 				                  vl_tableau_panneau,
// 			      				            vl_tableau_position,
// 			      				            vl_sequence);

			      XZST_03EcritureTrace(XZSTC_DEBUG1,"XZET07 No:%d Eqt:%s pos:%s seq:%d",vl_numEqt,
			 				            vl_tableau_panneau,
			      				            vl_tableau_position,
			      				            vl_sequence);
			      				
			      /** type */
			      pl_Type = strtok ( vl_tableau_type, "," );
			      for ( vl_Index = 0; vl_Index < XZETC_MAX_UNITAIRE; vl_Index++ )
			      {
			         if (pl_Type != NULL)  vl_cmd_GTCRenovee.NumTypeEqt[vl_Index] = atoi ( pl_Type );
			         pl_Type = strtok (NULL,",");
			         XZST_03EcritureTrace(XZSTC_DEBUG1,"XZET05 Eqt:%d",vl_cmd_GTCRenovee.NumTypeEqt[vl_Index]);
				  }
				  
			      /** DÅcripter la ligne du tableau de panneau ici */
			      pl_Pan = strtok ( vl_tableau_panneau, "," );
			      for ( vl_Index = 0; vl_Index < XZETC_MAX_UNITAIRE; vl_Index++ )
			      {
			         if (pl_Pan != NULL)  vl_cmd_GTCRenovee.NumEqt[vl_Index] = atoi ( pl_Pan );
			         pl_Pan = strtok (NULL,",");
			         XZST_03EcritureTrace(XZSTC_DEBUG1,"XZET07 Eqt:%d",vl_cmd_GTCRenovee.NumEqt[vl_Index]);
			      }			       

			      /** DÅcripter la ligne du tableau de position ici */
			      pl_Pos = strtok ( vl_tableau_position, "," );
			      for ( vl_Index = 0; vl_Index < XZETC_MAX_UNITAIRE; vl_Index++ )
			      {
			         if (pl_Pos != NULL)  vl_cmd_GTCRenovee.NumCmd[vl_Index] = atoi ( pl_Pos );
			         pl_Pos = strtok (NULL,",");
			         XZST_03EcritureTrace(XZSTC_DEBUG1,"XZET07 pos:%d",vl_cmd_GTCRenovee.NumCmd[vl_Index]);
			      }			       

			      vl_cmd_GTCRenovee.NumTE = vl_numEqt;
			      vl_cmd_GTCRenovee.NumSeq = vl_sequence;
			      vl_NumCmd = 0 ;

			      /** appel a la fonction commande unitaire ventilation **/
			      if (XZET07_Commande_VEN(vl_cmd_GTCRenovee, vl_NumCmd)!=XDC_OK)

			      { XZST_03EcritureTrace(XZSTC_WARNING,"tache relais: erreur dans XZET07");
				break; }
			break;
			
			/** commande unitaire pour l'etat echangeur du domaine eclairage **/
			case(ITRE_AX_DOMAINE_ECL) :
			
					/** lecture des arguments **/
			      if (sscanf(pa_arg,"%d|%[^|]|%[^|]|%[^|]|%d",&vl_numEqt,
											vl_tableau_type,
											vl_tableau_panneau,
			      				            vl_tableau_position,
			      				            &vl_sequence)!=5)

			      { XZST_03EcritureTrace(XZSTC_WARNING,"%s n'est pas une chaine",pa_arg);
			        break; }
			     		         
// 			      XZST_03EcritureTrace(XZSTC_MESSAGE,"XZET02->XZET06 Cmde Unitaire Echangeur Domaine ECL No:%d Eqt:%s pos:%s seq:%d",vl_numEqt,
// 			 				                  vl_tableau_panneau,
// 			      				            vl_tableau_position,
// 			      				            vl_sequence);
			      				            
			      XZST_03EcritureTrace(XZSTC_DEBUG1,"XZET06 No:%d Eqt:%s pos:%s seq:%d",vl_numEqt,
			 				            vl_tableau_panneau,
			      				            vl_tableau_position,
			      				            vl_sequence);
			      				
			      /** type */
			      pl_Type = strtok ( vl_tableau_type, "," );
			      for ( vl_Index = 0; vl_Index < XZETC_MAX_UNITAIRE; vl_Index++ )
			      {
			         if (pl_Type != NULL)  vl_cmd_GTCRenovee.NumTypeEqt[vl_Index] = atoi ( pl_Type );
			         pl_Type = strtok (NULL,",");
			         XZST_03EcritureTrace(XZSTC_DEBUG1,"XZET05 Eqt:%d",vl_cmd_GTCRenovee.NumTypeEqt[vl_Index]);
				  }
			      /** DÅcripter la ligne du tableau de panneau ici */
			      pl_Pan = strtok ( vl_tableau_panneau, "," );
			      for ( vl_Index = 0; vl_Index < XZETC_MAX_UNITAIRE; vl_Index++ )
			      {
			         if (pl_Pan != NULL)  vl_cmd_GTCRenovee.NumEqt[vl_Index] = atoi ( pl_Pan );
			         pl_Pan = strtok (NULL,",");
			         XZST_03EcritureTrace(XZSTC_DEBUG1,"XZET06 Eqt:%d",vl_cmd_GTCRenovee.NumEqt[vl_Index]);
			      }			       

			      /** DÅcripter la ligne du tableau de position ici */
			      pl_Pos = strtok ( vl_tableau_position, "," );
			      for ( vl_Index = 0; vl_Index < XZETC_MAX_UNITAIRE; vl_Index++ )
			      {
			         if (pl_Pos != NULL)  vl_cmd_GTCRenovee.NumCmd[vl_Index] = atoi ( pl_Pos );
			         pl_Pos = strtok (NULL,",");
			         XZST_03EcritureTrace(XZSTC_DEBUG1,"XZET06 pos:%d",vl_cmd_GTCRenovee.NumCmd[vl_Index]);
			      }			       

			      vl_cmd_GTCRenovee.NumTE = vl_numEqt;
			      vl_cmd_GTCRenovee.NumSeq = vl_sequence;
			      vl_NumCmd = 0 ;

			      /** appel a la fonction commande unitaire eclairage**/
			      if (XZET06_Commande_ECL(vl_cmd_GTCRenovee, vl_NumCmd)!=XDC_OK)

			      { XZST_03EcritureTrace(XZSTC_WARNING,"tache relais: erreur dans XZET06");
				break; }
			break;
			
			/** commande unitaire pour l'etat echangeur du domaine energie **/
			case(ITRE_AX_DOMAINE_ENR) :
				
					/** lecture des arguments **/
			      if (sscanf(pa_arg,"%d|%[^|]|%[^|]|%[^|]|%d",&vl_numEqt,
											vl_tableau_type,
											vl_tableau_panneau,
			      				            vl_tableau_position,
			      				            &vl_sequence)!=5)

			      { XZST_03EcritureTrace(XZSTC_WARNING,"%s n'est pas une chaine",pa_arg);
			        break; }
			     		         
// 			      XZST_03EcritureTrace(XZSTC_MESSAGE,"XZET02->XZET08 Cmde Unitaire Echangeur Domaine ENR No:%d Eqt:%s pos:%s seq:%d",vl_numEqt,
// 			 				                  vl_tableau_panneau,
// 			      				            vl_tableau_position,
// 			      				            vl_sequence);
			      				            
			      XZST_03EcritureTrace(XZSTC_DEBUG1,"XZET08 No:%d Eqt:%s pos:%s seq:%d",vl_numEqt,
			 				            vl_tableau_panneau,
			      				            vl_tableau_position,
			      				            vl_sequence);
			      				
			      /** type */
			      pl_Type = strtok ( vl_tableau_type, "," );
			      for ( vl_Index = 0; vl_Index < XZETC_MAX_UNITAIRE; vl_Index++ )
			      {
			         if (pl_Type != NULL)  vl_cmd_GTCRenovee.NumTypeEqt[vl_Index] = atoi ( pl_Type );
			         pl_Type = strtok (NULL,",");
			         XZST_03EcritureTrace(XZSTC_DEBUG1,"XZET05 Eqt:%d",vl_cmd_GTCRenovee.NumTypeEqt[vl_Index]);
				  }
			      /** DÅcripter la ligne du tableau de panneau ici */
			      pl_Pan = strtok ( vl_tableau_panneau, "," );
			      for ( vl_Index = 0; vl_Index < XZETC_MAX_UNITAIRE; vl_Index++ )
			      {
			         if (pl_Pan != NULL)  vl_cmd_GTCRenovee.NumEqt[vl_Index] = atoi ( pl_Pan );
			         pl_Pan = strtok (NULL,",");
			         XZST_03EcritureTrace(XZSTC_DEBUG1,"XZET08 Eqt:%d",vl_cmd_GTCRenovee.NumEqt[vl_Index]);
			      }			       

			      /** DÅcripter la ligne du tableau de position ici */
			      pl_Pos = strtok ( vl_tableau_position, "," );
			      for ( vl_Index = 0; vl_Index < XZETC_MAX_UNITAIRE; vl_Index++ )
			      {
			         if (pl_Pos != NULL)  vl_cmd_GTCRenovee.NumCmd[vl_Index] = atoi ( pl_Pos );
			         pl_Pos = strtok (NULL,",");
			         XZST_03EcritureTrace(XZSTC_DEBUG1,"XZET08 pos:%d",vl_cmd_GTCRenovee.NumCmd[vl_Index]);
			      }			       

			      vl_cmd_GTCRenovee.NumTE = vl_numEqt;
			      vl_cmd_GTCRenovee.NumSeq = vl_sequence;
			      vl_NumCmd = 0 ;

			      /** appel a la fonction commande unitaire energie **/
			      if (XZET08_Commande_ENR(vl_cmd_GTCRenovee, vl_NumCmd)!=XDC_OK)

			      { XZST_03EcritureTrace(XZSTC_WARNING,"tache relais: erreur dans XZET08");
				break; }
			break;
			/** commande unitaire pour l'etat echangeur du domaine exploitation **/
			case(ITRE_AX_DOMAINE_EXP) :
				
					/** lecture des arguments **/
			      if (sscanf(pa_arg,"%d|%[^|]|%[^|]|%[^|]|%d",&vl_numEqt,
											vl_tableau_type,
											vl_tableau_panneau,
			      				            vl_tableau_position,
			      				            &vl_sequence)!=5)

			      { XZST_03EcritureTrace(XZSTC_WARNING,"%s n'est pas une chaine",pa_arg);
			        break; }
			      XZST_03EcritureTrace(XZSTC_DEBUG1,"XZET08 No:%d Eqt:%s pos:%s seq:%d",vl_numEqt,
			 				            vl_tableau_panneau,
			      				            vl_tableau_position,
			      				            vl_sequence);
			      				
			      /** type */
			      pl_Type = strtok ( vl_tableau_type, "," );
			      for ( vl_Index = 0; vl_Index < XZETC_MAX_UNITAIRE; vl_Index++ )
			      {
			         if (pl_Type != NULL)  vl_cmd_GTCRenovee.NumTypeEqt[vl_Index] = atoi ( pl_Type );
			         pl_Type = strtok (NULL,",");
			         XZST_03EcritureTrace(XZSTC_DEBUG1,"XZET05 Eqt:%d",vl_cmd_GTCRenovee.NumTypeEqt[vl_Index]);
				  }
			      /** DÅcripter la ligne du tableau de panneau ici */
			      pl_Pan = strtok ( vl_tableau_panneau, "," );
			      for ( vl_Index = 0; vl_Index < XZETC_MAX_UNITAIRE; vl_Index++ )
			      {
			         if (pl_Pan != NULL)  vl_cmd_GTCRenovee.NumEqt[vl_Index] = atoi ( pl_Pan );
			         pl_Pan = strtok (NULL,",");
			         XZST_03EcritureTrace(XZSTC_DEBUG1,"XZET08 Eqt:%d",vl_cmd_GTCRenovee.NumEqt[vl_Index]);
			      }			       

			      /** DÅcripter la ligne du tableau de position ici */
			      pl_Pos = strtok ( vl_tableau_position, "," );
			      for ( vl_Index = 0; vl_Index < XZETC_MAX_UNITAIRE; vl_Index++ )
			      {
			         if (pl_Pos != NULL)  vl_cmd_GTCRenovee.NumCmd[vl_Index] = atoi ( pl_Pos );
			         pl_Pos = strtok (NULL,",");
			         XZST_03EcritureTrace(XZSTC_DEBUG1,"XZET45 pos:%d",vl_cmd_GTCRenovee.NumCmd[vl_Index]);
			      }			       

			      vl_cmd_GTCRenovee.NumTE = vl_numEqt;
			      vl_cmd_GTCRenovee.NumSeq = vl_sequence;
			      vl_NumCmd = 0 ;

			      /** appel a la fonction commande unitaire exploitation **/
			      if (XZET11_Commande_EXP(vl_cmd_GTCRenovee, vl_NumCmd)!=XDC_OK)

			      { XZST_03EcritureTrace(XZSTC_WARNING,"tache relais: erreur dans XZET08");
				break; }
			break;			
			
			/** commande unitaire pour l'etat echangeur pour les tubes non rénovés **/
			default :
			       /** lecture des arguments **/
			      if (sscanf(pa_arg,"%d|%[^|]|%[^|]|%d",&vl_numEqt,
											vl_tableau_panneau,
			      				            vl_tableau_position,
			      				            &vl_sequence)!=4)

			      { XZST_03EcritureTrace(XZSTC_WARNING,"%s n'est pas une chaine",pa_arg);
			        break; }
			     		         
// 			      XZST_03EcritureTrace(XZSTC_MESSAGE,"XZET02 Cmde Unitaire Echangeur non renove No:%d Eqt:%s pos:%s seq:%d",vl_numEqt,
// 			 				                  vl_tableau_panneau,
// 			      				            vl_tableau_position,
// 			      				            vl_sequence);
			      				            
			      XZST_03EcritureTrace(XZSTC_DEBUG1,"XZET02 No:%d pan:%s pos:%s seq:%d",vl_numEqt,
			 				            vl_tableau_panneau,
			      				            vl_tableau_position,
			      				            vl_sequence);
			      				
			      /** Decripter la ligne du tableau de panneau ici */
			      pl_Pan = strtok ( vl_tableau_panneau, "," );
			      for ( vl_Index = 0; vl_Index < XZETC_MAX_PANNEAU; vl_Index++ )
			      {
				 if (pl_Pan != NULL)  vl_cmd_GTC.NumPan[vl_Index] = atoi ( pl_Pan );
			         pl_Pan = strtok (NULL,",");
			         XZST_03EcritureTrace(XZSTC_DEBUG1,"XZET02 pan:%d",vl_cmd_GTC.NumPan[vl_Index]);
			      }			       

			      /** Decripter la ligne du tableau de position ici */
			      pl_Pos = strtok ( vl_tableau_position, "," );
			      for ( vl_Index = 0; vl_Index < XZETC_MAX_PANNEAU; vl_Index++ )
			      {
				 if (pl_Pos != NULL)  vl_cmd_GTC.NumCmd[vl_Index] = atoi ( pl_Pos );
			         pl_Pos = strtok (NULL,",");
			         XZST_03EcritureTrace(XZSTC_DEBUG1,"XZET02 pos:%d",vl_cmd_GTC.NumPan[vl_Index]);
			      }			       

			      vl_cmd_GTC.NumTE = vl_numEqt;
			      vl_cmd_GTC.NumSeq = vl_sequence;
			      vl_NumCmd = 0 ;

			      /** appel a XZET02 **/
			      if (XZET02_Commande_SIGE(vl_cmd_GTC,
			      			        vl_NumCmd)!=XDC_OK)

			      { XZST_03EcritureTrace(XZSTC_WARNING,"tache relais: erreur dans XZET02");
				break; }
			break;
		}

      break;






      /*** appel de la fonction XZET01 ***/
    case (ITRE_AX_XZET01):
		switch (va_numCmd)
		{
			/** commande unitaire pour l'etat Tunnel du domaine signalisation **/
			case(ITRE_AX_DOMAINE_SIG) :
			
					/** lecture des arguments **/
			      if (sscanf(pa_arg,"%d|%[^|]|%[^|]|%[^|]|%d",&vl_numEqt,
											vl_tableau_type,
											vl_tableau_panneau,
			      				            vl_tableau_position,
			      				            &vl_sequence)!=5)

			      { XZST_03EcritureTrace(XZSTC_WARNING,"%s n'est pas une chaine",pa_arg);
			        break; }
			     		         
			      XZST_03EcritureTrace(XZSTC_MESSAGE,"XZET01->XZET05 Cmde Unitaire Tunnel Domaine SIG No:%d Eqt:%s pos:%s seq:%d",vl_numEqt,
			 				                  vl_tableau_panneau,
			      				            vl_tableau_position,
			      				            vl_sequence);

			      XZST_03EcritureTrace(XZSTC_WARNING,"XZET05 No:%d Type:%s Eqt:%s pos:%s seq:%d",vl_numEqt,
			 				            vl_tableau_type,
			 				            vl_tableau_panneau,
			      				            vl_tableau_position,
			      				            vl_sequence);
			      				
			      /** type */
			      pl_Type = strtok ( vl_tableau_type, "," );
			      for ( vl_Index = 0; vl_Index < XZETC_MAX_UNITAIRE; vl_Index++ )
			      {
			         if (pl_Type != NULL)  vl_cmd_GTCRenovee.NumTypeEqt[vl_Index] = atoi ( pl_Type );
			         pl_Type = strtok (NULL,",");
			         XZST_03EcritureTrace(XZSTC_DEBUG1,"XZET05 Eqt:%d",vl_cmd_GTCRenovee.NumTypeEqt[vl_Index]);
				  }
				  
				  /** DÅcripter la ligne du tableau de panneau ici */
			      pl_Pan = strtok ( vl_tableau_panneau, "," );
			      for ( vl_Index = 0; vl_Index < XZETC_MAX_UNITAIRE; vl_Index++ )
			      {
			         if (pl_Pan != NULL)  vl_cmd_GTCRenovee.NumEqt[vl_Index] = atoi ( pl_Pan );
			         pl_Pan = strtok (NULL,",");
			         XZST_03EcritureTrace(XZSTC_DEBUG1,"XZET05 Eqt:%d",vl_cmd_GTCRenovee.NumEqt[vl_Index]);
			      }			       

			      /** DÅcripter la ligne du tableau de position ici */
			      pl_Pos = strtok ( vl_tableau_position, "," );
			      for ( vl_Index = 0; vl_Index < XZETC_MAX_UNITAIRE; vl_Index++ )
			      {
			         if (pl_Pos != NULL)  vl_cmd_GTCRenovee.NumCmd[vl_Index] = atoi ( pl_Pos );
			         pl_Pos = strtok (NULL,",");
			         XZST_03EcritureTrace(XZSTC_DEBUG1,"XZET05 pos:%d",vl_cmd_GTCRenovee.NumCmd[vl_Index]);
			      }			       

			      vl_cmd_GTCRenovee.NumTE = vl_numEqt;
			      vl_cmd_GTCRenovee.NumSeq = vl_sequence;
			      vl_NumCmd = 0 ;

			      /** appel a la fonction commande unitaire signalisation**/
			      if (XZET05_Commande_SIG(vl_cmd_GTCRenovee, vl_NumCmd)!=XDC_OK)

			      { XZST_03EcritureTrace(XZSTC_WARNING,"tache relais: erreur dans XZET05");
				break; }
			break;
			
			/** commande unitaire pour l'etat Tunnel du domaine ventilation **/
			case(ITRE_AX_DOMAINE_VEN) :
			
			      /** lecture des arguments **/
			      if (sscanf(pa_arg,"%d|%[^|]|%[^|]|%[^|]|%d",&vl_numEqt,
											vl_tableau_type,
											vl_tableau_panneau,
			      				            vl_tableau_position,
			      				            &vl_sequence)!=5)

			      { XZST_03EcritureTrace(XZSTC_WARNING,"%s n'est pas une chaine",pa_arg);
			        break; }
			     		         
			      XZST_03EcritureTrace(XZSTC_MESSAGE,"XZET01->XZET07 Cmde Unitaire Tunnel Domaine VEN No:%d Eqt:%s pos:%s seq:%d",vl_numEqt,
			 				                  vl_tableau_panneau,
			      				            vl_tableau_position,
			      				            vl_sequence);

			      XZST_03EcritureTrace(XZSTC_DEBUG1,"XZET07 No:%d Eqt:%s pos:%s seq:%d",vl_numEqt,
			 				            vl_tableau_panneau,
			      				            vl_tableau_position,
			      				            vl_sequence);
			      				
			      /** type */
			      pl_Type = strtok ( vl_tableau_type, "," );
			      for ( vl_Index = 0; vl_Index < XZETC_MAX_UNITAIRE; vl_Index++ )
			      {
			         if (pl_Type != NULL)  vl_cmd_GTCRenovee.NumTypeEqt[vl_Index] = atoi ( pl_Type );
			         pl_Type = strtok (NULL,",");
			         XZST_03EcritureTrace(XZSTC_DEBUG1,"XZET05 Eqt:%d",vl_cmd_GTCRenovee.NumTypeEqt[vl_Index]);
				  }

			      /** DÅcripter la ligne du tableau de panneau ici */
			      pl_Pan = strtok ( vl_tableau_panneau, "," );
			      for ( vl_Index = 0; vl_Index < XZETC_MAX_UNITAIRE; vl_Index++ )
			      {
			         if (pl_Pan != NULL)  vl_cmd_GTCRenovee.NumEqt[vl_Index] = atoi ( pl_Pan );
			         pl_Pan = strtok (NULL,",");
			         XZST_03EcritureTrace(XZSTC_DEBUG1,"XZET07 Eqt:%d",vl_cmd_GTCRenovee.NumEqt[vl_Index]);
			      }			       

			      /** DÅcripter la ligne du tableau de position ici */
			      pl_Pos = strtok ( vl_tableau_position, "," );
			      for ( vl_Index = 0; vl_Index < XZETC_MAX_UNITAIRE; vl_Index++ )
			      {
			         if (pl_Pos != NULL)  vl_cmd_GTCRenovee.NumCmd[vl_Index] = atoi ( pl_Pos );
			         pl_Pos = strtok (NULL,",");
			         XZST_03EcritureTrace(XZSTC_DEBUG1,"XZET07 pos:%d",vl_cmd_GTCRenovee.NumCmd[vl_Index]);
			      }			       

			      vl_cmd_GTCRenovee.NumTE = vl_numEqt;
			      vl_cmd_GTCRenovee.NumSeq = vl_sequence;
			      vl_NumCmd = 0 ;

			      /** appel a la fonction commande unitaire ventilation **/
			      if (XZET07_Commande_VEN(vl_cmd_GTCRenovee, vl_NumCmd)!=XDC_OK)

			      { XZST_03EcritureTrace(XZSTC_WARNING,"tache relais: erreur dans XZET07");
				break; }
			break;
			
			/** commande unitaire pour l'etat Tunnel du domaine eclairage **/
			case(ITRE_AX_DOMAINE_ECL) :
			
					/** lecture des arguments **/
			      if (sscanf(pa_arg,"%d|%[^|]|%[^|]|%[^|]|%d",&vl_numEqt,
											vl_tableau_type,
											vl_tableau_panneau,
			      				            vl_tableau_position,
			      				            &vl_sequence)!=5)

			      { XZST_03EcritureTrace(XZSTC_WARNING,"%s n'est pas une chaine",pa_arg);
			        break; }
			     		         
			      XZST_03EcritureTrace(XZSTC_MESSAGE,"XZET01->XZET06 Cmde Unitaire Tunnel Domaine ECL No:%d Eqt:%s pos:%s seq:%d",vl_numEqt,
			 				                  vl_tableau_panneau,
			      				            vl_tableau_position,
			      				            vl_sequence);

			      XZST_03EcritureTrace(XZSTC_DEBUG1,"XZET06 No:%d Eqt:%s pos:%s seq:%d",vl_numEqt,
			 				            vl_tableau_panneau,
			      				            vl_tableau_position,
			      				            vl_sequence);
			      				
			      /** type */
			      pl_Type = strtok ( vl_tableau_type, "," );
			      for ( vl_Index = 0; vl_Index < XZETC_MAX_UNITAIRE; vl_Index++ )
			      {
			         if (pl_Type != NULL)  vl_cmd_GTCRenovee.NumTypeEqt[vl_Index] = atoi ( pl_Type );
			         pl_Type = strtok (NULL,",");
			         XZST_03EcritureTrace(XZSTC_DEBUG1,"XZET05 Eqt:%d",vl_cmd_GTCRenovee.NumTypeEqt[vl_Index]);
				  }
			      /** DÅcripter la ligne du tableau de panneau ici */
			      pl_Pan = strtok ( vl_tableau_panneau, "," );
			      for ( vl_Index = 0; vl_Index < XZETC_MAX_UNITAIRE; vl_Index++ )
			      {
			         if (pl_Pan != NULL)  vl_cmd_GTCRenovee.NumEqt[vl_Index] = atoi ( pl_Pan );
			         pl_Pan = strtok (NULL,",");
			         XZST_03EcritureTrace(XZSTC_DEBUG1,"XZET06 Eqt:%d",vl_cmd_GTCRenovee.NumEqt[vl_Index]);
			      }			       

			      /** DÅcripter la ligne du tableau de position ici */
			      pl_Pos = strtok ( vl_tableau_position, "," );
			      for ( vl_Index = 0; vl_Index < XZETC_MAX_UNITAIRE; vl_Index++ )
			      {
			         if (pl_Pos != NULL)  vl_cmd_GTCRenovee.NumCmd[vl_Index] = atoi ( pl_Pos );
			         pl_Pos = strtok (NULL,",");
			         XZST_03EcritureTrace(XZSTC_DEBUG1,"XZET06 pos:%d",vl_cmd_GTCRenovee.NumCmd[vl_Index]);
			      }			       

			      vl_cmd_GTCRenovee.NumTE = vl_numEqt;
			      vl_cmd_GTCRenovee.NumSeq = vl_sequence;
			      vl_NumCmd = 0 ;

			      /** appel a la fonction commande unitaire eclairage **/
			      if (XZET06_Commande_ECL(vl_cmd_GTCRenovee, vl_NumCmd)!=XDC_OK)

			      { XZST_03EcritureTrace(XZSTC_WARNING,"tache relais: erreur dans XZET06");
				break; }
			break;
			
			/** commande unitaire pour l'etat Tunnel du domaine energie **/
			case(ITRE_AX_DOMAINE_ENR) :
				
					/** lecture des arguments **/
			      if (sscanf(pa_arg,"%d|%[^|]|%[^|]|%[^|]|%d",&vl_numEqt,
											vl_tableau_type,
											vl_tableau_panneau,
			      				            vl_tableau_position,
			      				            &vl_sequence)!=5)

			      { XZST_03EcritureTrace(XZSTC_WARNING,"%s n'est pas une chaine",pa_arg);
			        break; }
			     		         
			      XZST_03EcritureTrace(XZSTC_MESSAGE,"XZET01->XZET08 Cmde Unitaire Tunnel Domaine ENR No:%d Eqt:%s pos:%s seq:%d",vl_numEqt,
			 				                  vl_tableau_panneau,
			      				            vl_tableau_position,
			      				            vl_sequence);
			      				            
			      XZST_03EcritureTrace(XZSTC_DEBUG1,"XZET08 No:%d Eqt:%s pos:%s seq:%d",vl_numEqt,
			 				            vl_tableau_panneau,
			      				            vl_tableau_position,
			      				            vl_sequence);
			      				
			      /** type */
			      pl_Type = strtok ( vl_tableau_type, "," );
			      for ( vl_Index = 0; vl_Index < XZETC_MAX_UNITAIRE; vl_Index++ )
			      {
			         if (pl_Type != NULL)  vl_cmd_GTCRenovee.NumTypeEqt[vl_Index] = atoi ( pl_Type );
			         pl_Type = strtok (NULL,",");
			         XZST_03EcritureTrace(XZSTC_DEBUG1,"XZET05 Eqt:%d",vl_cmd_GTCRenovee.NumTypeEqt[vl_Index]);
				  }
			      /** DÅcripter la ligne du tableau de panneau ici */
			      pl_Pan = strtok ( vl_tableau_panneau, "," );
			      for ( vl_Index = 0; vl_Index < XZETC_MAX_UNITAIRE; vl_Index++ )
			      {
			        if (pl_Pan != NULL)   vl_cmd_GTCRenovee.NumEqt[vl_Index] = atoi ( pl_Pan );
			         pl_Pan = strtok (NULL,",");
			         XZST_03EcritureTrace(XZSTC_DEBUG1,"XZET08 Eqt:%d",vl_cmd_GTCRenovee.NumEqt[vl_Index]);
			      }			       

			      /** DÅcripter la ligne du tableau de position ici */
			      pl_Pos = strtok ( vl_tableau_position, "," );
			      for ( vl_Index = 0; vl_Index < XZETC_MAX_UNITAIRE; vl_Index++ )
			      {
			         if (pl_Pos != NULL)  vl_cmd_GTCRenovee.NumCmd[vl_Index] = atoi ( pl_Pos );
			         pl_Pos = strtok (NULL,",");
			         XZST_03EcritureTrace(XZSTC_DEBUG1,"XZET08 pos:%d",vl_cmd_GTCRenovee.NumCmd[vl_Index]);
			      }			       

			      vl_cmd_GTCRenovee.NumTE = vl_numEqt;
			      vl_cmd_GTCRenovee.NumSeq = vl_sequence;
			      vl_NumCmd = 0 ;

			      /** appel a la fonction commande unitaire energie**/
			      if (XZET08_Commande_ENR(vl_cmd_GTCRenovee, vl_NumCmd)!=XDC_OK)

			      { XZST_03EcritureTrace(XZSTC_WARNING,"tache relais: erreur dans XZET08");
				break; }
			break;
			
			/** commande unitaire pour l'etat Tunnel du domaine exploitation **/
			case(ITRE_AX_DOMAINE_EXP) :
				
					/** lecture des arguments **/
			      if (sscanf(pa_arg,"%d|%[^|]|%[^|]|%[^|]|%d",&vl_numEqt,
											vl_tableau_type,
											vl_tableau_panneau,
			      				            vl_tableau_position,
			      				            &vl_sequence)!=5)

			      { XZST_03EcritureTrace(XZSTC_WARNING,"%s n'est pas une chaine",pa_arg);
			        break; }
			     		         
			      XZST_03EcritureTrace(XZSTC_MESSAGE,"XZET01->XZET11 Cmde Unitaire Tunnel Domaine EXP No:%d Eqt:%s pos:%s seq:%d",vl_numEqt,
			 				                  vl_tableau_panneau,
			      				            vl_tableau_position,
			      				            vl_sequence);
			      				            
			      XZST_03EcritureTrace(XZSTC_DEBUG1,"XZET11 No:%d Eqt:%s pos:%s seq:%d",vl_numEqt,
			 				            vl_tableau_panneau,
			      				            vl_tableau_position,
			      				            vl_sequence);
			      				
			      /** type */
			      pl_Type = strtok ( vl_tableau_type, "," );
			      for ( vl_Index = 0; vl_Index < XZETC_MAX_UNITAIRE; vl_Index++ )
			      {
			         if (pl_Type != NULL)  vl_cmd_GTCRenovee.NumTypeEqt[vl_Index] = atoi ( pl_Type );
			         pl_Type = strtok (NULL,",");
			         XZST_03EcritureTrace(XZSTC_DEBUG1,"XZET05 Eqt:%d",vl_cmd_GTCRenovee.NumTypeEqt[vl_Index]);
				  }
			      /** DÅcripter la ligne du tableau de panneau ici */
			      pl_Pan = strtok ( vl_tableau_panneau, "," );
			      for ( vl_Index = 0; vl_Index < XZETC_MAX_UNITAIRE; vl_Index++ )
			      {
			         if (pl_Pan != NULL)  vl_cmd_GTCRenovee.NumEqt[vl_Index] = atoi ( pl_Pan );
			         pl_Pan = strtok (NULL,",");
			         XZST_03EcritureTrace(XZSTC_DEBUG1,"XZET11 Eqt:%d",vl_cmd_GTCRenovee.NumEqt[vl_Index]);
			      }			       

			      /** DÅcripter la ligne du tableau de position ici */
			      pl_Pos = strtok ( vl_tableau_position, "," );
			      for ( vl_Index = 0; vl_Index < XZETC_MAX_UNITAIRE; vl_Index++ )
			      {
			         if (pl_Pos != NULL)  vl_cmd_GTCRenovee.NumCmd[vl_Index] = atoi ( pl_Pos );
			         pl_Pos = strtok (NULL,",");
			         XZST_03EcritureTrace(XZSTC_DEBUG1,"XZET11 pos:%d",vl_cmd_GTCRenovee.NumCmd[vl_Index]);
			      }			       

			      vl_cmd_GTCRenovee.NumTE = vl_numEqt;
			      vl_cmd_GTCRenovee.NumSeq = vl_sequence;
			      vl_NumCmd = 0 ;

			      /** appel a la fonction commande unitaire exploitation**/
			      if (XZET11_Commande_EXP(vl_cmd_GTCRenovee, vl_NumCmd)!=XDC_OK)

			      { XZST_03EcritureTrace(XZSTC_WARNING,"tache relais: erreur dans XZET11");
				break; }
			break;
			
						
			
			/** commande unitaire pour l'etat Tunnel pour les tubes non renovés**/	
			default :
			      /** lecture des arguments **/
			      if (sscanf(pa_arg,"%d|%[^|]|%[^|]|%d",&vl_numEqt,
			 				            vl_tableau_panneau,
			      				            vl_tableau_position,
			      				            &vl_sequence)!=4)

			      { XZST_03EcritureTrace(XZSTC_WARNING,"%s n'est pas une chaine",pa_arg);
			        break; }
			     		         
			      XZST_03EcritureTrace(XZSTC_MESSAGE,"XZET01 Cmde Unitaire Tunnel non renove No:%d Eqt:%s pos:%s seq:%d",vl_numEqt,
			 				                  vl_tableau_panneau,
			      				            vl_tableau_position,
			      				            vl_sequence);
			      				            
			      XZST_03EcritureTrace(XZSTC_DEBUG1,"XZET01 No:%d pan:%s pos:%s seq:%d",vl_numEqt,
			 				            vl_tableau_panneau,
			      				            vl_tableau_position,
			      				            vl_sequence);
			      				
			      /** DÅcripter la ligne du tableau de panneau ici */
			      pl_Pan = strtok ( vl_tableau_panneau, "," );
			      for ( vl_Index = 0; vl_Index < XZETC_MAX_PANNEAU; vl_Index++ )
			      {
			         if (pl_Pan != NULL)  vl_cmd_GTC.NumPan[vl_Index] = atoi ( pl_Pan );
			         pl_Pan = strtok (NULL,",");
			         XZST_03EcritureTrace(XZSTC_DEBUG1,"XZET01 pan:%d",vl_cmd_GTC.NumPan[vl_Index]);
			      }			       

			      /** DÅcripter la ligne du tableau de position ici */
			      pl_Pos = strtok ( vl_tableau_position, "," );
			      for ( vl_Index = 0; vl_Index < XZETC_MAX_PANNEAU; vl_Index++ )
			      {
			         if (pl_Pos != NULL)  vl_cmd_GTC.NumCmd[vl_Index] = atoi ( pl_Pos );
			         pl_Pos = strtok (NULL,",");
			         XZST_03EcritureTrace(XZSTC_DEBUG1,"XZET01 pos:%d",vl_cmd_GTC.NumPan[vl_Index]);
			      }			       

			      vl_cmd_GTC.NumTE = vl_numEqt;
			      vl_cmd_GTC.NumSeq = vl_sequence;
			      vl_NumCmd = 0 ;

			      /** appel a XZET01 **/
			      if (XZET01_Commande_SIGT(vl_cmd_GTC, vl_NumCmd)!=XDC_OK)

			      { XZST_03EcritureTrace(XZSTC_WARNING,"tache relais: erreur dans XZET01");
				break; }
			break;
		}
      break;



	  /*** appel de la fonction pour la commande unitaire pour les LTN  ***/
    case (ITRE_AX_XZEA114):
		switch (va_numCmd)
		{
			/** commande unitaire pour l'etat LTN  du domaine signalisation **/
			case(ITRE_AX_DOMAINE_SIG) :
			
					/** lecture des arguments **/
			      if (sscanf(pa_arg,"%d|%[^|]|%[^|]|%[^|]|%d",&vl_numEqt,
											vl_tableau_type,
											vl_tableau_panneau,
			      				            vl_tableau_position,
			      				            &vl_sequence)!=5)

			      { XZST_03EcritureTrace(XZSTC_WARNING,"%s n'est pas une chaine",pa_arg);
			        break; }
			     		         
			      XZST_03EcritureTrace(XZSTC_MESSAGE,"XZET102->XZET05 Cmde Unitaire LTN Domaine SIG No:%d Eqt:%s pos:%s seq:%d",vl_numEqt,
			 				                  vl_tableau_panneau,
			      				            vl_tableau_position,
			      				            vl_sequence);
			      				            
			      XZST_03EcritureTrace(XZSTC_DEBUG1,"XZET05 No:%d Eqt:%s pos:%s seq:%d",vl_numEqt,
			 				            vl_tableau_panneau,
			      				            vl_tableau_position,
			      				            vl_sequence);
			      				
			      /** type */
			      pl_Type = strtok ( vl_tableau_type, "," );
			      for ( vl_Index = 0; vl_Index < XZETC_MAX_UNITAIRE; vl_Index++ )
			      {
			         if (pl_Type != NULL)  vl_cmd_GTCRenovee.NumTypeEqt[vl_Index] = atoi ( pl_Type );
			         pl_Type = strtok (NULL,",");
			         XZST_03EcritureTrace(XZSTC_DEBUG1,"XZET05 Eqt:%d",vl_cmd_GTCRenovee.NumTypeEqt[vl_Index]);
				  }

			      /** DÅcripter la ligne du tableau de panneau ici */
			      pl_Pan = strtok ( vl_tableau_panneau, "," );
			      for ( vl_Index = 0; vl_Index < XZETC_MAX_UNITAIRE; vl_Index++ )
			      {
			         if (pl_Pan != NULL)  vl_cmd_GTCRenovee.NumEqt[vl_Index] = atoi ( pl_Pan );
			         pl_Pan = strtok (NULL,",");
			         XZST_03EcritureTrace(XZSTC_DEBUG1,"XZET05 Eqt:%d",vl_cmd_GTCRenovee.NumEqt[vl_Index]);
			      }			       

			      /** DÅcripter la ligne du tableau de position ici */
			      pl_Pos = strtok ( vl_tableau_position, "," );
			      for ( vl_Index = 0; vl_Index < XZETC_MAX_UNITAIRE; vl_Index++ )
			      {
			         if (pl_Pos != NULL)  vl_cmd_GTCRenovee.NumCmd[vl_Index] = atoi ( pl_Pos );
			         pl_Pos = strtok (NULL,",");
			         XZST_03EcritureTrace(XZSTC_DEBUG1,"XZET05 pos:%d",vl_cmd_GTCRenovee.NumCmd[vl_Index]);
			      }			       

			      vl_cmd_GTCRenovee.NumTE = vl_numEqt;
			      vl_cmd_GTCRenovee.NumSeq = vl_sequence;
			      vl_NumCmd = 0 ;

			      /** appel a la fonction commande unitaire signalisation**/
			      if (XZET05_Commande_SIG(vl_cmd_GTCRenovee, vl_NumCmd)!=XDC_OK)

			      { XZST_03EcritureTrace(XZSTC_WARNING,"tache relais: erreur dans XZET05");
				break; }
			break;
			
			/** commande unitaire pour l'etat LTN  du domaine ventilation **/
			case(ITRE_AX_DOMAINE_VEN) :
			
					/** lecture des arguments **/
			      if (sscanf(pa_arg,"%d|%[^|]|%[^|]|%[^|]|%d",&vl_numEqt,
											vl_tableau_type,
											vl_tableau_panneau,
			      				            vl_tableau_position,
			      				            &vl_sequence)!=5)

			      { XZST_03EcritureTrace(XZSTC_WARNING,"%s n'est pas une chaine",pa_arg);
			        break; }
			     		         
			      XZST_03EcritureTrace(XZSTC_MESSAGE,"XZET102->XZET07 Cmde Unitaire LTN Domaine VEN No:%d Eqt:%s pos:%s seq:%d",vl_numEqt,
			 				                  vl_tableau_panneau,
			      				            vl_tableau_position,
			      				            vl_sequence);
			      				            
			      XZST_03EcritureTrace(XZSTC_DEBUG1,"XZET07 No:%d Eqt:%s pos:%s seq:%d",vl_numEqt,
			 				            vl_tableau_panneau,
			      				            vl_tableau_position,
			      				            vl_sequence);
			      				
			      /** type */
			      pl_Type = strtok ( vl_tableau_type, "," );
			      for ( vl_Index = 0; vl_Index < XZETC_MAX_UNITAIRE; vl_Index++ )
			      {
			         if (pl_Type != NULL)  vl_cmd_GTCRenovee.NumTypeEqt[vl_Index] = atoi ( pl_Type );
			         pl_Type = strtok (NULL,",");
			         XZST_03EcritureTrace(XZSTC_DEBUG1,"XZET05 Eqt:%d",vl_cmd_GTCRenovee.NumTypeEqt[vl_Index]);
				  }
			      /** DÅcripter la ligne du tableau de panneau ici */
			      pl_Pan = strtok ( vl_tableau_panneau, "," );
			      for ( vl_Index = 0; vl_Index < XZETC_MAX_UNITAIRE; vl_Index++ )
			      {
			         if (pl_Pan != NULL)  vl_cmd_GTCRenovee.NumEqt[vl_Index] = atoi ( pl_Pan );
			         pl_Pan = strtok (NULL,",");
			         XZST_03EcritureTrace(XZSTC_DEBUG1,"XZET07 Eqt:%d",vl_cmd_GTCRenovee.NumEqt[vl_Index]);
			      }			       

			      /** DÅcripter la ligne du tableau de position ici */
			      pl_Pos = strtok ( vl_tableau_position, "," );
			      for ( vl_Index = 0; vl_Index < XZETC_MAX_UNITAIRE; vl_Index++ )
			      {
			         if (pl_Pos != NULL)  vl_cmd_GTCRenovee.NumCmd[vl_Index] = atoi ( pl_Pos );
			         pl_Pos = strtok (NULL,",");
			         XZST_03EcritureTrace(XZSTC_DEBUG1,"XZET07 pos:%d",vl_cmd_GTCRenovee.NumCmd[vl_Index]);
			      }			       

			      vl_cmd_GTCRenovee.NumTE = vl_numEqt;
			      vl_cmd_GTCRenovee.NumSeq = vl_sequence;
			      vl_NumCmd = 0 ;

			      /** appel a la fonction commande unitaire ventilation **/
			      if (XZET07_Commande_VEN(vl_cmd_GTCRenovee, vl_NumCmd)!=XDC_OK)

			      { XZST_03EcritureTrace(XZSTC_WARNING,"tache relais: erreur dans XZET07");
				break; }
			break;
			
			/** commande unitaire pour l'etat LTN  du domaine eclairage **/
			case(ITRE_AX_DOMAINE_ECL) :
			
					/** lecture des arguments **/
			      if (sscanf(pa_arg,"%d|%[^|]|%[^|]|%[^|]|%d",&vl_numEqt,
											vl_tableau_type,
											vl_tableau_panneau,
			      				            vl_tableau_position,
			      				            &vl_sequence)!=5)

			      { XZST_03EcritureTrace(XZSTC_WARNING,"%s n'est pas une chaine",pa_arg);
			        break; }
			     		         
			      XZST_03EcritureTrace(XZSTC_MESSAGE,"XZET102->XZET06 Cmde Unitaire LTN Domaine ECL No:%d Eqt:%s pos:%s seq:%d",vl_numEqt,
			 				                  vl_tableau_panneau,
			      				            vl_tableau_position,
			      				            vl_sequence);
			      				            
			      XZST_03EcritureTrace(XZSTC_DEBUG1,"XZET06 No:%d Eqt:%s pos:%s seq:%d",vl_numEqt,
			 				            vl_tableau_panneau,
			      				            vl_tableau_position,
			      				            vl_sequence);
			      				
			      /** type */
			      pl_Type = strtok ( vl_tableau_type, "," );
			      for ( vl_Index = 0; vl_Index < XZETC_MAX_UNITAIRE; vl_Index++ )
			      {
			         if (pl_Type != NULL)  vl_cmd_GTCRenovee.NumTypeEqt[vl_Index] = atoi ( pl_Type );
			         pl_Type = strtok (NULL,",");
			         XZST_03EcritureTrace(XZSTC_DEBUG1,"XZET05 Eqt:%d",vl_cmd_GTCRenovee.NumTypeEqt[vl_Index]);
				  }
			      /** DÅcripter la ligne du tableau de panneau ici */
			      pl_Pan = strtok ( vl_tableau_panneau, "," );
			      for ( vl_Index = 0; vl_Index < XZETC_MAX_UNITAIRE; vl_Index++ )
			      {
			         if (pl_Pan != NULL)  vl_cmd_GTCRenovee.NumEqt[vl_Index] = atoi ( pl_Pan );
			         pl_Pan = strtok (NULL,",");
			         XZST_03EcritureTrace(XZSTC_DEBUG1,"XZET06 Eqt:%d",vl_cmd_GTCRenovee.NumEqt[vl_Index]);
			      }			       

			      /** DÅcripter la ligne du tableau de position ici */
			      pl_Pos = strtok ( vl_tableau_position, "," );
			      for ( vl_Index = 0; vl_Index < XZETC_MAX_UNITAIRE; vl_Index++ )
			      {
			         if (pl_Pos != NULL)  vl_cmd_GTCRenovee.NumCmd[vl_Index] = atoi ( pl_Pos );
			         pl_Pos = strtok (NULL,",");
			         XZST_03EcritureTrace(XZSTC_DEBUG1,"XZET06 pos:%d",vl_cmd_GTCRenovee.NumCmd[vl_Index]);
			      }			       

			      vl_cmd_GTCRenovee.NumTE = vl_numEqt;
			      vl_cmd_GTCRenovee.NumSeq = vl_sequence;
			      vl_NumCmd = 0 ;

			      /** appel a la fonction commande unitaire eclairage**/
			      if (XZET06_Commande_ECL(vl_cmd_GTCRenovee, vl_NumCmd)!=XDC_OK)

			      { XZST_03EcritureTrace(XZSTC_WARNING,"tache relais: erreur dans XZET06");
				break; }
			break;
			
			/** commande unitaire pour l'etat LTN  du domaine energie **/
			case(ITRE_AX_DOMAINE_ENR) :
				
					/** lecture des arguments **/
			      if (sscanf(pa_arg,"%d|%[^|]|%[^|]|%[^|]|%d",&vl_numEqt,
											vl_tableau_type,
											vl_tableau_panneau,
			      				            vl_tableau_position,
			      				            &vl_sequence)!=5)

			      { XZST_03EcritureTrace(XZSTC_WARNING,"%s n'est pas une chaine",pa_arg);
			        break; }
			     		         
			      XZST_03EcritureTrace(XZSTC_MESSAGE,"XZET102->XZET08 Cmde Unitaire LTN Domaine ENR No:%d Eqt:%s pos:%s seq:%d",vl_numEqt,
			 				                  vl_tableau_panneau,
			      				            vl_tableau_position,
			      				            vl_sequence);
			      				            
			      XZST_03EcritureTrace(XZSTC_DEBUG1,"XZET08 No:%d Eqt:%s pos:%s seq:%d",vl_numEqt,
			 				            vl_tableau_panneau,
			      				            vl_tableau_position,
			      				            vl_sequence);
			      				
			      /** type */
			      pl_Type = strtok ( vl_tableau_type, "," );
			      for ( vl_Index = 0; vl_Index < XZETC_MAX_UNITAIRE; vl_Index++ )
			      {
			         if (pl_Type != NULL)  vl_cmd_GTCRenovee.NumTypeEqt[vl_Index] = atoi ( pl_Type );
			         pl_Type = strtok (NULL,",");
			         XZST_03EcritureTrace(XZSTC_DEBUG1,"XZET05 Eqt:%d",vl_cmd_GTCRenovee.NumTypeEqt[vl_Index]);
				  }
			      /** DÅcripter la ligne du tableau de panneau ici */
			      pl_Pan = strtok ( vl_tableau_panneau, "," );
			      for ( vl_Index = 0; vl_Index < XZETC_MAX_UNITAIRE; vl_Index++ )
			      {
			         if (pl_Pan != NULL)  vl_cmd_GTCRenovee.NumEqt[vl_Index] = atoi ( pl_Pan );
			         pl_Pan = strtok (NULL,",");
			         XZST_03EcritureTrace(XZSTC_DEBUG1,"XZET08 Eqt:%d",vl_cmd_GTCRenovee.NumEqt[vl_Index]);
			      }			       

			      /** DÅcripter la ligne du tableau de position ici */
			      pl_Pos = strtok ( vl_tableau_position, "," );
			      for ( vl_Index = 0; vl_Index < XZETC_MAX_UNITAIRE; vl_Index++ )
			      {
			        if (pl_Pos != NULL)   vl_cmd_GTCRenovee.NumCmd[vl_Index] = atoi ( pl_Pos );
			         pl_Pos = strtok (NULL,",");
			         XZST_03EcritureTrace(XZSTC_DEBUG1,"XZET08 pos:%d",vl_cmd_GTCRenovee.NumCmd[vl_Index]);
			      }			       

			      vl_cmd_GTCRenovee.NumTE = vl_numEqt;
			      vl_cmd_GTCRenovee.NumSeq = vl_sequence;
			      vl_NumCmd = 0 ;

			      /** appel a la fonction commande unitaire energien**/
			      if (XZET08_Commande_ENR(vl_cmd_GTCRenovee, vl_NumCmd)!=XDC_OK)

			      { XZST_03EcritureTrace(XZSTC_WARNING,"tache relais: erreur dans XZET08");
				break; }
			break;
			
						
		}
      break;




	 /*** appel de la fonction pour la commande unitaire pour l'etat HT ***/
    case (ITRE_AX_XZEA115):
		switch (va_numCmd)
		{
			/** commande unitaire pour l'etat HT du domaine signalisation **/
			case(ITRE_AX_DOMAINE_SIG) :
			
					/** lecture des arguments **/
			      if (sscanf(pa_arg,"%d|%[^|]|%[^|]|%[^|]|%d",&vl_numEqt,
											vl_tableau_type,
											vl_tableau_panneau,
			      				            vl_tableau_position,
			      				            &vl_sequence)!=5)

			      { XZST_03EcritureTrace(XZSTC_WARNING,"%s n'est pas une chaine",pa_arg);
			        break; }
			     		         
			      XZST_03EcritureTrace(XZSTC_MESSAGE,"XZET103->XZET05 Cmde Unitaire HT Domaine SIG No:%d Eqt:%s pos:%s seq:%d",vl_numEqt,
			 				                  vl_tableau_panneau,
			      				            vl_tableau_position,
			      				            vl_sequence);

			      XZST_03EcritureTrace(XZSTC_DEBUG1,"XZET05 No:%d Eqt:%s pos:%s seq:%d",vl_numEqt,
			 				            vl_tableau_panneau,
			      				            vl_tableau_position,
			      				            vl_sequence);
			      				
			      /** type */
			      pl_Type = strtok ( vl_tableau_type, "," );
			      for ( vl_Index = 0; vl_Index < XZETC_MAX_UNITAIRE; vl_Index++ )
			      {
			         if (pl_Type != NULL)  vl_cmd_GTCRenovee.NumTypeEqt[vl_Index] = atoi ( pl_Type );
			         pl_Type = strtok (NULL,",");
			         XZST_03EcritureTrace(XZSTC_DEBUG1,"XZET05 Eqt:%d",vl_cmd_GTCRenovee.NumTypeEqt[vl_Index]);
				  }
			      /** DÅcripter la ligne du tableau de panneau ici */
			      pl_Pan = strtok ( vl_tableau_panneau, "," );
			      for ( vl_Index = 0; vl_Index < XZETC_MAX_UNITAIRE; vl_Index++ )
			      {
			         if (pl_Pan != NULL)  vl_cmd_GTCRenovee.NumEqt[vl_Index] = atoi ( pl_Pan );
			         pl_Pan = strtok (NULL,",");
			         XZST_03EcritureTrace(XZSTC_DEBUG1,"XZET05 Eqt:%d",vl_cmd_GTCRenovee.NumEqt[vl_Index]);
			      }			       

			      /** DÅcripter la ligne du tableau de position ici */
			      pl_Pos = strtok ( vl_tableau_position, "," );
			      for ( vl_Index = 0; vl_Index < XZETC_MAX_UNITAIRE; vl_Index++ )
			      {
			         if (pl_Pos != NULL)  vl_cmd_GTCRenovee.NumCmd[vl_Index] = atoi ( pl_Pos );
			         pl_Pos = strtok (NULL,",");
			         XZST_03EcritureTrace(XZSTC_DEBUG1,"XZET05 pos:%d",vl_cmd_GTCRenovee.NumCmd[vl_Index]);
			      }			       

			      vl_cmd_GTCRenovee.NumTE = vl_numEqt;
			      vl_cmd_GTCRenovee.NumSeq = vl_sequence;
			      vl_NumCmd = 0 ;

			      /** appel a la fonction commande unitaire signalisation**/
			      if (XZET05_Commande_SIG(vl_cmd_GTCRenovee, vl_NumCmd)!=XDC_OK)

			      { XZST_03EcritureTrace(XZSTC_WARNING,"tache relais: erreur dans XZET05");
				break; }
			break;
			
			/** commande unitaire pour etat HT du  domaine ventilation **/
			case(ITRE_AX_DOMAINE_VEN) :
			
					/** lecture des arguments **/
			      if (sscanf(pa_arg,"%d|%[^|]|%[^|]|%[^|]|%d",&vl_numEqt,
											vl_tableau_type,
											vl_tableau_panneau,
			      				            vl_tableau_position,
			      				            &vl_sequence)!=5)

			      { XZST_03EcritureTrace(XZSTC_WARNING,"%s n'est pas une chaine",pa_arg);
			        break; }
			     		         
			      XZST_03EcritureTrace(XZSTC_MESSAGE,"XZET103->XZET07 Cmde Unitaire HT Domaine VEN No:%d Eqt:%s pos:%s seq:%d",vl_numEqt,
			 				                  vl_tableau_panneau,
			      				            vl_tableau_position,
			      				            vl_sequence);

			      XZST_03EcritureTrace(XZSTC_DEBUG1,"XZET07 No:%d Eqt:%s pos:%s seq:%d",vl_numEqt,
			 				            vl_tableau_panneau,
			      				            vl_tableau_position,
			      				            vl_sequence);
			      				
			      /** type */
			      pl_Type = strtok ( vl_tableau_type, "," );
			      for ( vl_Index = 0; vl_Index < XZETC_MAX_UNITAIRE; vl_Index++ )
			      {
			         if (pl_Type != NULL)  vl_cmd_GTCRenovee.NumTypeEqt[vl_Index] = atoi ( pl_Type );
			         pl_Type = strtok (NULL,",");
			         XZST_03EcritureTrace(XZSTC_DEBUG1,"XZET05 Eqt:%d",vl_cmd_GTCRenovee.NumTypeEqt[vl_Index]);
				  }
			      /** DÅcripter la ligne du tableau de panneau ici */
			      pl_Pan = strtok ( vl_tableau_panneau, "," );
			      for ( vl_Index = 0; vl_Index < XZETC_MAX_UNITAIRE; vl_Index++ )
			      {
			         if (pl_Pan != NULL)  vl_cmd_GTCRenovee.NumEqt[vl_Index] = atoi ( pl_Pan );
			         pl_Pan = strtok (NULL,",");
			         XZST_03EcritureTrace(XZSTC_DEBUG1,"XZET07 Eqt:%d",vl_cmd_GTCRenovee.NumEqt[vl_Index]);
			      }			       

			      /** DÅcripter la ligne du tableau de position ici */
			      pl_Pos = strtok ( vl_tableau_position, "," );
			      for ( vl_Index = 0; vl_Index < XZETC_MAX_UNITAIRE; vl_Index++ )
			      {
			         if (pl_Pos != NULL)  vl_cmd_GTCRenovee.NumCmd[vl_Index] = atoi ( pl_Pos );
			         pl_Pos = strtok (NULL,",");
			         XZST_03EcritureTrace(XZSTC_DEBUG1,"XZET07 pos:%d",vl_cmd_GTCRenovee.NumCmd[vl_Index]);
			      }			       

			      vl_cmd_GTCRenovee.NumTE = vl_numEqt;
			      vl_cmd_GTCRenovee.NumSeq = vl_sequence;
			      vl_NumCmd = 0 ;

			      /** appel a la fonction commande unitaire ventilation **/
			      if (XZET07_Commande_VEN(vl_cmd_GTCRenovee, vl_NumCmd)!=XDC_OK)

			      { XZST_03EcritureTrace(XZSTC_WARNING,"tache relais: erreur dans XZET07");
				break; }
			break;
			
			/** commande unitaire pour l'etat HT du domaine eclairage **/
			case(ITRE_AX_DOMAINE_ECL) :
			
					/** lecture des arguments **/
			      if (sscanf(pa_arg,"%d|%[^|]|%[^|]|%[^|]|%d",&vl_numEqt,
											vl_tableau_type,
											vl_tableau_panneau,
			      				            vl_tableau_position,
			      				            &vl_sequence)!=5)

			      { XZST_03EcritureTrace(XZSTC_WARNING,"%s n'est pas une chaine",pa_arg);
			        break; }
			     		         
			      XZST_03EcritureTrace(XZSTC_MESSAGE,"XZET103->XZET06 Cmde Unitaire HT Domaine ECL No:%d Eqt:%s pos:%s seq:%d",vl_numEqt,
			 				                  vl_tableau_panneau,
			      				            vl_tableau_position,
			      				            vl_sequence);
			      				            
			      XZST_03EcritureTrace(XZSTC_DEBUG1,"XZET06 No:%d Eqt:%s pos:%s seq:%d",vl_numEqt,
			 				            vl_tableau_panneau,
			      				            vl_tableau_position,
			      				            vl_sequence);
			      				
			      /** type */
			      pl_Type = strtok ( vl_tableau_type, "," );
			      for ( vl_Index = 0; vl_Index < XZETC_MAX_UNITAIRE; vl_Index++ )
			      {
			         if (pl_Type != NULL)  vl_cmd_GTCRenovee.NumTypeEqt[vl_Index] = atoi ( pl_Type );
			         pl_Type = strtok (NULL,",");
			         XZST_03EcritureTrace(XZSTC_DEBUG1,"XZET05 Eqt:%d",vl_cmd_GTCRenovee.NumTypeEqt[vl_Index]);
				  }
			      /** DÅcripter la ligne du tableau de panneau ici */
			      pl_Pan = strtok ( vl_tableau_panneau, "," );
			      for ( vl_Index = 0; vl_Index < XZETC_MAX_UNITAIRE; vl_Index++ )
			      {
			         if (pl_Pan != NULL)  vl_cmd_GTCRenovee.NumEqt[vl_Index] = atoi ( pl_Pan );
			         pl_Pan = strtok (NULL,",");
			         XZST_03EcritureTrace(XZSTC_DEBUG1,"XZET06 Eqt:%d",vl_cmd_GTCRenovee.NumEqt[vl_Index]);
			      }			       

			      /** DÅcripter la ligne du tableau de position ici */
			      pl_Pos = strtok ( vl_tableau_position, "," );
			      for ( vl_Index = 0; vl_Index < XZETC_MAX_UNITAIRE; vl_Index++ )
			      {
			         if (pl_Pos != NULL)  vl_cmd_GTCRenovee.NumCmd[vl_Index] = atoi ( pl_Pos );
			         pl_Pos = strtok (NULL,",");
			         XZST_03EcritureTrace(XZSTC_DEBUG1,"XZET06 pos:%d",vl_cmd_GTCRenovee.NumCmd[vl_Index]);
			      }			       

			      vl_cmd_GTCRenovee.NumTE = vl_numEqt;
			      vl_cmd_GTCRenovee.NumSeq = vl_sequence;
			      vl_NumCmd = 0 ;

			      /** appel a la fonction commande unitaire eclairage**/
			      if (XZET06_Commande_ECL(vl_cmd_GTCRenovee, vl_NumCmd)!=XDC_OK)

			      { XZST_03EcritureTrace(XZSTC_WARNING,"tache relais: erreur dans XZET06");
				break; }
			break;
			
			/** commande unitaire pour l'etat HT du domaine energie **/
			case(ITRE_AX_DOMAINE_ENR) :
				
					/** lecture des arguments **/
			      if (sscanf(pa_arg,"%d|%[^|]|%[^|]|%[^|]|%d",&vl_numEqt,
											vl_tableau_type,
											vl_tableau_panneau,
			      				            vl_tableau_position,
			      				            &vl_sequence)!=5)

			      { XZST_03EcritureTrace(XZSTC_WARNING,"%s n'est pas une chaine",pa_arg);
			        break; }
			     		         
			      XZST_03EcritureTrace(XZSTC_MESSAGE,"XZET103->XZET08 Cmde Unitaire HT Domaine ENR No:%d Eqt:%s pos:%s seq:%d",vl_numEqt,
			 				                  vl_tableau_panneau,
			      				            vl_tableau_position,
			      				            vl_sequence);
			      				            
			      XZST_03EcritureTrace(XZSTC_DEBUG1,"XZET08 No:%d Eqt:%s pos:%s seq:%d",vl_numEqt,
			 				            vl_tableau_panneau,
			      				            vl_tableau_position,
			      				            vl_sequence);
			      				
			      /** type */
			      pl_Type = strtok ( vl_tableau_type, "," );
			      for ( vl_Index = 0; vl_Index < XZETC_MAX_UNITAIRE; vl_Index++ )
			      {
			         if (pl_Type != NULL)  vl_cmd_GTCRenovee.NumTypeEqt[vl_Index] = atoi ( pl_Type );
			         pl_Type = strtok (NULL,",");
			         XZST_03EcritureTrace(XZSTC_DEBUG1,"XZET05 Eqt:%d",vl_cmd_GTCRenovee.NumTypeEqt[vl_Index]);
				  }
			      /** DÅcripter la ligne du tableau de panneau ici */
			      pl_Pan = strtok ( vl_tableau_panneau, "," );
			      for ( vl_Index = 0; vl_Index < XZETC_MAX_UNITAIRE; vl_Index++ )
			      {
			         if (pl_Pan != NULL)  vl_cmd_GTCRenovee.NumEqt[vl_Index] = atoi ( pl_Pan );
			         pl_Pan = strtok (NULL,",");
			         XZST_03EcritureTrace(XZSTC_DEBUG1,"XZET08 Eqt:%d",vl_cmd_GTCRenovee.NumEqt[vl_Index]);
			      }			       

			      /** DÅcripter la ligne du tableau de position ici */
			      pl_Pos = strtok ( vl_tableau_position, "," );
			      for ( vl_Index = 0; vl_Index < XZETC_MAX_UNITAIRE; vl_Index++ )
			      {
			         if (pl_Pos != NULL)  vl_cmd_GTCRenovee.NumCmd[vl_Index] = atoi ( pl_Pos );
			         pl_Pos = strtok (NULL,",");
			         XZST_03EcritureTrace(XZSTC_DEBUG1,"XZET08 pos:%d",vl_cmd_GTCRenovee.NumCmd[vl_Index]);
			      }			       

			      vl_cmd_GTCRenovee.NumTE = vl_numEqt;
			      vl_cmd_GTCRenovee.NumSeq = vl_sequence;
			      vl_NumCmd = 0 ;

			      /** appel a la fonction commande unitaire energie**/
			      if (XZET08_Commande_ENR(vl_cmd_GTCRenovee, vl_NumCmd)!=XDC_OK)

			      { XZST_03EcritureTrace(XZSTC_WARNING,"tache relais: erreur dans XZET08");
				break; }
			break;			
		}
      break;  
	  
	  


  /* REPERE1*/
      /*** appel de la fonction XZET03 ***/
    case (ITRE_AX_XZET03):
    
      /** lecture des arguments **/
      if (sscanf(pa_arg,"%d|%d",&vl_numEqt,
    				&vl_sequence)!=2)

      { 
	     XZST_03EcritureTrace(XZSTC_WARNING,"%s n'est pas une chaine",pa_arg);
        break; 
      }
     		         
      XZST_03EcritureTrace(XZSTC_DEBUG1,"XZET03 Tube:%d seq:%d",vl_numEqt,
      				            vl_sequence);
      				

      vl_Commande_SIG_NIC.NumTE = vl_numEqt;
      vl_Commande_SIG_NIC.NumSEQ = vl_sequence;
      vl_NumCmd = 0 ;

      /** appel a XZET03 **/
      if (XZET03_Commande_SEQT(vl_Commande_SIG_NIC,
      			        vl_NumCmd)!=XDC_OK)

      { XZST_03EcritureTrace(XZSTC_WARNING,"tache relais: erreur dans XZET03\n");
	   break; }
	
      break;
  /* FIN REPERE1*/




	/** commande de domaine pour les echangeurs **/
	case (ITRE_AX_DOMAINE_ECHANGEUR):
    
      /** lecture des arguments **/
      if (sscanf(pa_arg,"%d|%d",&vl_numEqt,
      			            &vl_sequence)!=2)

      { 
	XZST_03EcritureTrace(XZSTC_WARNING,"%s n'est pas une chaine",pa_arg);
        break; 
      }
     		         
      XZST_03EcritureTrace(XZSTC_DEBUG1,"XZET04 Ech:%d Mode:%d",vl_numEqt,
      				            vl_sequence);
      				

	  switch(va_numCmd)
	  {
	  /* va_numCmd contient le domaine au format "Applix", il faut le transformer dans le format "interne" (XDC_EQT_SIG par exemple) */
	  case(11):
		vl_Commande_SIG_NIX.Domaine = XDC_EQT_SIG;
		break;
	  case(2):
		vl_Commande_SIG_NIX.Domaine = XDC_EQT_VEN;
		break;
	  case(3):
		vl_Commande_SIG_NIX.Domaine = XDC_EQT_ECL;
		break;
	  case(4):
		vl_Commande_SIG_NIX.Domaine = XDC_EQT_ENR;
		break;
	  case(5):
		vl_Commande_SIG_NIX.Domaine = XDC_EQT_EXP;
		break;
	  case(6):
		vl_Commande_SIG_NIX.Domaine = XDC_EQT_LI;
		break;
	  case(7):
		vl_Commande_SIG_NIX.Domaine = XDC_EQT_LTN;
		break;
	  case(8):
		vl_Commande_SIG_NIX.Domaine = XDC_EQT_SYS;
		break;
	  }
      vl_Commande_SIG_NIX.TypeOuvrage = XDC_EQT_ECH;
      vl_Commande_SIG_NIX.IdOuvrage = vl_numEqt;
      vl_Commande_SIG_NIX.Mode = vl_sequence;
      vl_NumCmd = 0 ;

      /** appel a XZET10 **/
      if (XZET10_Commande_Domaine( vl_Commande_SIG_NIX,
      			        vl_NumCmd)!=XDC_OK)

      { XZST_03EcritureTrace(XZSTC_WARNING,"tache relais: erreur dans XZET10");
	    break; }
	
      break;




	/** commande de domaine pour les tunnels **/
	case (ITRE_AX_DOMAINE_TUNNEL):
    
      /** lecture des arguments **/
      if (sscanf(pa_arg,"%d|%d",&vl_numEqt,
      				            &vl_sequence)!=2)

      { 
	     XZST_03EcritureTrace(XZSTC_WARNING,"%s n'est pas une chaine",pa_arg);
        break; 
      }
     		         
      XZST_03EcritureTrace(XZSTC_DEBUG1,"XZET05 Tube:%d Mode:%d",vl_numEqt,
      				            vl_sequence);
      				
	  switch(va_numCmd)
	  {
	  /* va_numCmd contient le domaine au format "Applix", il faut le transformer dans le format "interne" (XDC_EQT_SIG par exemple) */
	  case(11):
		vl_Commande_SIG_NIX.Domaine = XDC_EQT_SIG;
		break;
	  case(2):
		vl_Commande_SIG_NIX.Domaine = XDC_EQT_VEN;
		break;
	  case(3):
		vl_Commande_SIG_NIX.Domaine = XDC_EQT_ECL;
		break;
	  case(4):
		vl_Commande_SIG_NIX.Domaine = XDC_EQT_ENR;
		break;
	  case(5):
		vl_Commande_SIG_NIX.Domaine = XDC_EQT_EXP;
		break;
	  case(6):
		vl_Commande_SIG_NIX.Domaine = XDC_EQT_LI;
		break;
	  case(7):
		vl_Commande_SIG_NIX.Domaine = XDC_EQT_LTN;
		break;
	  case(8):
		vl_Commande_SIG_NIX.Domaine = XDC_EQT_SYS;
		break;
	  }
      vl_Commande_SIG_NIX.TypeOuvrage = XDC_EQT_TUB;
      vl_Commande_SIG_NIX.IdOuvrage = vl_numEqt;
      vl_Commande_SIG_NIX.Mode = vl_sequence;
      vl_NumCmd = 0 ;

      /** appel a XZET10 **/
      if (XZET10_Commande_Domaine(vl_Commande_SIG_NIX,
      			        vl_NumCmd)!=XDC_OK)

      { XZST_03EcritureTrace(XZSTC_WARNING,"tache relais: erreur dans XZET10\n");
	     break; }
	
      break;




    /*** appel de la fonction XZEA08 ***/
    case (ITRE_AX_XZEA08):
XZST_03EcritureTrace(XZSTC_WARNING,"Reception %s sur XZEA08",pa_arg);
    
      /** lecture des arguments **/
	if (sscanf(pa_arg,"%d|%s",&vl_modeAbonnement,
						vl_nom)!=2)
      	{ 
			XZST_03EcritureTrace(XZSTC_WARNING,"%s est non chaine",pa_arg);
        	break; 
	}

      /** appel a XZEA08 **/
      if (XZEA08_Abt_Etat_PMV(vl_modeAbonnement,
	                      (vl_modeAbonnement == XZEAC_DEBUT_ABONNEMENT) ? vl_dg_mce : NULL,
			      itre_ax_Abt_Etat_PMV,
			      vl_nom,&vl_cr)!=XDC_OK)
			      
      { XZST_03EcritureTrace(XZSTC_WARNING,"tache relais: erreur dans XZEA08");
	     break; }
	
      break;
 
 
    case (ITRE_AX_XZEA082):
     /** lecture des arguments **/
      if (sscanf(pa_arg,"%d|%s",&vl_modeAbonnement,
                                vl_nom)!=2)

      { XZST_03EcritureTrace(XZSTC_WARNING,"ITRE_AX_XZEA082: %s est non chaine",pa_arg);
        break; }
      /** appel a XZEA08 **/
      XZST_03EcritureTrace(XZSTC_WARNING,"XZEA082");
      if (XZEA082_Abt_Etat_CFE(vl_modeAbonnement,
                              (vl_modeAbonnement == XZEAC_DEBUT_ABONNEMENT) ? vl_dg_mce : NULL,
                              itre_ax_Abt_Etat_CFE,
                              vl_nom,&vl_cr)!=XDC_OK)

      { XZST_03EcritureTrace(XZSTC_WARNING,"tache relais: erreur dans XZEA08");
             break; }

      break;


    case (ITRE_AX_XZEA084):
     /** lecture des arguments **/
      if (sscanf(pa_arg,"%d|%s",&vl_modeAbonnement,
      				vl_nom)!=2)

      { XZST_03EcritureTrace(XZSTC_WARNING,"ITRE_AX_XZEA084: %s est non chaine",pa_arg);
        break; }
      /** appel a XZEA084 **/
      XZST_03EcritureTrace(XZSTC_WARNING,"XZEA084");
      if (XZEA084_Abt_Etat_IMU(vl_modeAbonnement,
      			      (vl_modeAbonnement == XZEAC_DEBUT_ABONNEMENT) ? vl_dg_mce : NULL,
			      itre_ax_Abt_Etat_IMU,
			      vl_nom,&vl_cr)!=XDC_OK)

     { XZST_03EcritureTrace(XZSTC_WARNING,"tache relais: erreur dans XZEA084");
       break; }

     break;


     case (ITRE_AX_XZEA085):
       /** lecture des arguments **/
       if (sscanf(pa_arg,"%d|%s",&vl_modeAbonnement,
       				vl_nom)!=2)

       { XZST_03EcritureTrace(XZSTC_WARNING,"ITRE_AX_XZEA085: %s est non chaine",pa_arg);
         break; }
       /** appel a XZEA085 **/
       XZST_03EcritureTrace(XZSTC_WARNING,"XZEA085");
       if (XZEA085_Abt_Etat_SONO(vl_modeAbonnement,
       				(vl_modeAbonnement == XZEAC_DEBUT_ABONNEMENT) ? vl_dg_mce : NULL,
				itre_ax_Abt_Etat_SONO,
				vl_nom,&vl_cr)!=XDC_OK)

       { XZST_03EcritureTrace(XZSTC_WARNING,"tache relais: erreur dans XZEA085");
         break; }

       break;

	
    
    case (ITRE_AX_XZEA180):
      if (sscanf(pa_arg,"%d|%s",&vl_modeAbonnement,
		vl_nom)!=2)
	{ XZST_03EcritureTrace(XZSTC_WARNING,"%s est non chaine",pa_arg);
		break; }

	if (XZEA180_Abt_Etat_Eqt_SAGA(vl_modeAbonnement,
				      (vl_modeAbonnement == XZEAC_DEBUT_ABONNEMENT) ? vl_dg_mce : NULL,
				      itre_ax_Abt_Etat_Eqt_SAGA,
				      vl_nom,&vl_cr)!=XDC_OK)
	{ XZST_03EcritureTrace(XZSTC_WARNING,"tache relais: erreur dans XZEA080");
		     break; }

    break;

    /*** appel de la fonction XZEA080 ***/
    case (ITRE_AX_XZEA080):
    
      /** lecture des arguments **/
      if (sscanf(pa_arg,"%d|%s",&vl_modeAbonnement,
      				vl_nom)!=2)
      				
      { XZST_03EcritureTrace(XZSTC_WARNING,"%s est non chaine",pa_arg);
        break; }

      /** appel a XZEA080 **/
      if (XZEA080_Abt_Etat_PMVA(vl_modeAbonnement,
	                      (vl_modeAbonnement == XZEAC_DEBUT_ABONNEMENT) ? vl_dg_mce : NULL,
			      itre_ax_Abt_Etat_PMVA,
			      vl_nom,&vl_cr)!=XDC_OK)
			      
      { XZST_03EcritureTrace(XZSTC_WARNING,"tache relais: erreur dans XZEA080");
	     break; }
	
      break;

	case (ITRE_AX_XZEA70):
		if (sscanf(pa_arg,"%d|%s",&vl_modeAbonnement, vl_nom) != 2)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"%s est non chaine",pa_arg);
			break;
		}
		/** appel a XZEA70 **/
		if (XZEA70_Abt_Etat_PRV(vl_modeAbonnement,
								(vl_modeAbonnement == XZEAC_DEBUT_ABONNEMENT) ? vl_dg_mce : NULL,
								itre_ax_MAJ_etat_PRV,
								vl_nom,&vl_cr) != XDC_OK)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"tache relais: erreur dans XZEA70");
			break;
		}
		break;

	case (ITRE_AX_XZEA71):
		if (sscanf (pa_arg, "%d|%s", &vl_modeAbonnement, vl_nom) != 2)
		{
			XZST_03EcritureTrace (XZSTC_WARNING,"%s est non chaine", pa_arg);
			break;
		}
		/** appel a XZEA71 **/
		if (XZEA71_Abt_Etat_PIC (vl_modeAbonnement, 
								(vl_modeAbonnement == XZEAC_DEBUT_ABONNEMENT) ? vl_dg_mce : NULL,
								itre_ax_MAJ_etat_PIC,
								vl_nom,&vl_cr) != XDC_OK)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"tache relais: erreur dans XZEA71");
			break;
		}
	    break;

     /*** appel de la fonction XZEA09 ***/
    case (ITRE_AX_XZEA09):
    
      /** lecture des arguments **/
      if (sscanf(pa_arg,"%d|%s",&vl_modeAbonnement,
      				vl_nom)!=2)
      				
      { XZST_03EcritureTrace(XZSTC_WARNING,"%s est non chaine",pa_arg);
        break; }

      /** appel a XZEA09 **/
      if (XZEA09_Abt_Etat_Picto(vl_modeAbonnement,
	                        (vl_modeAbonnement == XZEAC_DEBUT_ABONNEMENT) ? vl_dg_mce : NULL,
			        itre_ax_Abt_Etat_Picto,
			        vl_nom,&vl_cr)!=XDC_OK)
			      
      { XZST_03EcritureTrace(XZSTC_WARNING,"tache relais: erreur dans XZEA09\n");
	     break; }
	
      break;




   /*** appel de la fonction XZEA10 ***/
    case (ITRE_AX_XZEA10):
    
      /** lecture des arguments **/
      if (sscanf(pa_arg,"%d|%s",&vl_modeAbonnement,
      				vl_nom)!=2)
      				
      { XZST_03EcritureTrace(XZSTC_WARNING,"%s est non chaine",pa_arg);
        break; }

      /** appel a XZEA10 **/
      if (XZEA10_Abt_Etat_NAV(vl_modeAbonnement,
	                      (vl_modeAbonnement == XZEAC_DEBUT_ABONNEMENT) ? vl_dg_mce : NULL,
			      itre_ax_MAJ_etat_NAV,
			      vl_nom,&vl_cr)!=XDC_OK)
				   
      { XZST_03EcritureTrace(XZSTC_WARNING,"tache relais: erreur dans XZEA10\n");
	     break; }
	
      break;




   /*** appel de la fonction XZEA40 ***/
    case (ITRE_AX_XZEA40):
    
      /** lecture des arguments **/
      if (sscanf(pa_arg,"%d|%s",&vl_modeAbonnement,
      				vl_nom)!=2)
      				
      { XZST_03EcritureTrace(XZSTC_WARNING,"%s est non chaine",pa_arg);
        break; }

      /** appel a XZEA40 **/
      if (XZEA40_Abt_Etat_BAF(vl_modeAbonnement,
	                      (vl_modeAbonnement == XZEAC_DEBUT_ABONNEMENT) ? vl_dg_mce : NULL,
			      itre_ax_MAJ_etat_BAF,
			      vl_nom,&vl_cr)!=XDC_OK)
				   
      { XZST_03EcritureTrace(XZSTC_WARNING,"tache relais: erreur dans XZEA40\n");
	     break; }
	
      break;




   /*** appel de la fonction XZEA41 ***/
    case (ITRE_AX_XZEA41):
    
      /** lecture des arguments **/
      if (sscanf(pa_arg,"%d|%s",&vl_modeAbonnement,
      				vl_nom)!=2)
      				
      { XZST_03EcritureTrace(XZSTC_WARNING,"%s est non chaine",pa_arg);
        break; }

      /** appel a XZEA41 **/
      if (XZEA41_Abt_Etat_BAD(vl_modeAbonnement,
	                      (vl_modeAbonnement == XZEAC_DEBUT_ABONNEMENT) ? vl_dg_mce : NULL,
			      itre_ax_MAJ_etat_BAD,
			      vl_nom,&vl_cr)!=XDC_OK)
				   
      { XZST_03EcritureTrace(XZSTC_WARNING,"tache relais: erreur dans XZEA41\n");
	     break; }
	
      break;




   /*** appel de la fonction XZEA38 ***/
    case (ITRE_AX_XZEA38):
    

      /** lecture des arguments **/
      if (sscanf(pa_arg,"%d|%s",&vl_modeAbonnement,
      				vl_nom)!=2)
      				
      { XZST_03EcritureTrace(XZSTC_WARNING,"%s est non chaine",pa_arg);
        break; }

      /** appel a XZEA10 **/
      if (XZEA38_Abt_Etat_PAL(vl_modeAbonnement,
	                      (vl_modeAbonnement == XZEAC_DEBUT_ABONNEMENT) ? vl_dg_mce : NULL,
			      itre_ax_MAJ_etat_PAL,
			      vl_nom,&vl_cr)!=XDC_OK)
				   
      { XZST_03EcritureTrace(XZSTC_WARNING,"tache relais: erreur dans XZEA38\n");
	     break; }
	
      break;



       /*** appel de la fonction XZEA12 ( J.P.L 30/12/94 ) ***/
    case (ITRE_AX_XZEA12):
    
      /** lecture des arguments **/
      if (sscanf (pa_arg, "%d|%s", &vl_modeAbonnement,
				   vl_nomSite) != 2)
      { XZST_03EcritureTrace(XZSTC_WARNING,
		"Liste d'arguments invalide pour XZEA12 : \"%s\"\n", pa_arg);
        break; }

      /** appel a XZEA12 **/
      if (XZEA12_Abt_Etat_AUTOTEL(vl_modeAbonnement,
	                          (vl_modeAbonnement == XZEAC_DEBUT_ABONNEMENT) ? vl_dg_mce : NULL,
				  itre_ax_Abt_Etat_Autotel,
				  vl_nomSite,
				  &vl_cr) != XDC_OK)
      { XZST_03EcritureTrace(XZSTC_WARNING,
				"Tache relais: erreur dans XZEA12\n");
	     break; }
	
      break;




    /*** appel de la fonction XZEA13 ***/
    case (ITRE_AX_XZEA13):
    
      /** lecture des arguments **/
      if (sscanf(pa_arg,"%d|%s",&vl_modeAbonnement,
      				vl_nom)!=2)
      				
      { XZST_03EcritureTrace(XZSTC_WARNING,"%s est non chaine",pa_arg);
        break; }




      /** appel a XZEA13 **/
      if (XZEA13_Abt_Etat_Magnetos(vl_modeAbonnement,
	                           (vl_modeAbonnement == XZEAC_DEBUT_ABONNEMENT) ? vl_dg_mce : NULL,
				   itre_ax_piloterMagnetos,
				   vl_nom,&vl_cr)!=XDC_OK)
				   
      { XZST_03EcritureTrace(XZSTC_WARNING,"tache relais: erreur dans XZEA13\n");
	     break; }
	
      break;




       /*** appel de la fonction XZEA14 ( J.P.L 18/01/95 ) ***/
    case (ITRE_AX_XZEA14):
    
      /** lecture des arguments **/
      if (sscanf (pa_arg, "%d|%s", &vl_modeAbonnement,
				   vl_nomSite) != 2)
      { XZST_03EcritureTrace(XZSTC_WARNING,
		"Liste d'arguments invalide pour XZEA14 : \"%s\"\n", pa_arg);
        break; }

      /** appel a XZEA14 **/
      if (XZEA14_Abt_Etat_RAU(vl_modeAbonnement,
	                         (vl_modeAbonnement == XZEAC_DEBUT_ABONNEMENT) ? vl_dg_mce : NULL,
				 itre_ax_Abt_Etat_PAU,
				 vl_nomSite,
				 &vl_cr) != XDC_OK)
      { XZST_03EcritureTrace(XZSTC_WARNING,"%s Tache relais: erreur dans XZEA14\n", version );
	     break; }
	
      break;



    /*** appel de la fonction XZEA15 ***/
    case (ITRE_AX_XZEA15):
    
      /** lecture des arguments **/
      if (sscanf(pa_arg,"%d|%s",&vl_modeAbonnement,
      				vl_nom)!=2)
      				
      { XZST_03EcritureTrace(XZSTC_WARNING,"%s est non chaine",pa_arg);
        break; }

      /** appel a XZEA15 **/
      if (XZEA15_Abt_Etat_Camera(vl_modeAbonnement,
	                           (vl_modeAbonnement == XZEAC_DEBUT_ABONNEMENT) ? vl_dg_mce : NULL,
				   itre_ax_piloterCamera,
				   vl_nom,&vl_cr)!=XDC_OK)
				   
      { XZST_03EcritureTrace(XZSTC_WARNING,"tache relais: erreur dans XZEA15\n");
	     break; }
	
      break;




   /*** appel de la fonction XZEA18 ***/
    case (ITRE_AX_XZEA18):
		switch (va_numCmd)
		{
		/** abonnement des equipements du domaine signalisation **/
		case(ITRE_AX_DOMAINE_SIG) :
				/*** appel de la fonction XZEA42**/
		      /** lecture des arguments **/
		      if (sscanf(pa_arg,"%d|%[^|]|%d|%d",&vl_modeAbonnement,
		      				         vl_nom,
		      				         &vl_typeEqt,
		      				         &vl_numEqt)!=4)
		      { XZST_03EcritureTrace(XZSTC_WARNING,"%s : %s est non chaine",version,pa_arg);
		        break; }
			      
		      XZST_03EcritureTrace(XZSTC_MESSAGE,"XZEA18->XZEA42 et XZEA55 Abonnement Domaine SIG Abt:%d Nom:%s TypeEqt:%d NumEqt:%d",
		                                 vl_modeAbonnement,
		      				               vl_nom,
		      				               vl_typeEqt,
		      				               vl_numEqt);

		      /** appel a XZEA42*/
		      if (XZEA42_Abt_Etat_EqtSIG(vl_modeAbonnement,
			                           (vl_modeAbonnement == XZEAC_DEBUT_ABONNEMENT) ? vl_dg_mce : NULL,
						   itre_ax_Abt_Etat_Equipement,
						   vl_nom,
						   XDC_EQT_SIG,
						   vl_typeEqt,
						   vl_numEqt,
						   &vl_cr)!=XDC_OK)
		      { XZST_03EcritureTrace(XZSTC_WARNING,"%s : tache relais: erreur dans XZEA42\n", version);
			   break; }
			
			   /** appel a XZEA55**/
		      if (XZEA55_Abt_Etat_Domaine(vl_modeAbonnement,
			                           (vl_modeAbonnement == XZEAC_DEBUT_ABONNEMENT) ? vl_dg_mce : NULL,
						   itre_ax_Abt_Etat_Domaine,
						   vl_nom,
						   XDC_EQT_SIG,
						   vl_typeEqt,
						   vl_numEqt,
						   &vl_cr)!=XDC_OK)
		      { XZST_03EcritureTrace(XZSTC_WARNING,"%s : tache relais: erreur dans XZEA55\n", version);
			   break; }
			   
			   /** appel a XZEA55**/
		      if (XZEA55_Abt_Etat_Domaine(vl_modeAbonnement,
			                           (vl_modeAbonnement == XZEAC_DEBUT_ABONNEMENT) ? vl_dg_mce : NULL,
						   itre_ax_Abt_Etat_Domaine,
						   vl_nom,
						   XDC_EQT_VEN,
						   vl_typeEqt,
						   vl_numEqt,
						   &vl_cr)!=XDC_OK)
		      { XZST_03EcritureTrace(XZSTC_WARNING,"%s : tache relais: erreur dans XZEA55\n", version);
			   break; }
			   
			   /** appel a XZEA55**/
		      if (XZEA55_Abt_Etat_Domaine(vl_modeAbonnement,
			                           (vl_modeAbonnement == XZEAC_DEBUT_ABONNEMENT) ? vl_dg_mce : NULL,
						   itre_ax_Abt_Etat_Domaine,
						   vl_nom,
						   XDC_EQT_ECL,
						   vl_typeEqt,
						   vl_numEqt,
						   &vl_cr)!=XDC_OK)
		      { XZST_03EcritureTrace(XZSTC_WARNING,"%s : tache relais: erreur dans XZEA55\n", version);
			   break; }			   
		break;
		
		/** abonnement des equipements du domaine ventilation **/
		case(ITRE_AX_DOMAINE_VEN) :
			/*** appel de la fonction XZEA44**/
		      /** lecture des arguments **/
		      if (sscanf(pa_arg,"%d|%[^|]|%d|%d",&vl_modeAbonnement,
		      				         vl_nom,
		      				         &vl_typeEqt,
		      				         &vl_numEqt)!=4)
		      { XZST_03EcritureTrace(XZSTC_WARNING,"%s : %s est non chaine",version,pa_arg);
		        break; }

		      XZST_03EcritureTrace(XZSTC_MESSAGE,"XZEA18->XZEA44 et XZEA55 Abonnement Domaine VEN Abt:%d Nom:%s TypeEqt:%d NumEqt:%d",
		                                 vl_modeAbonnement,
		      				               vl_nom,
		      				               vl_typeEqt,
		      				               vl_numEqt);

		      /** appel a XZEA44*/
		      if (XZEA44_Abt_Etat_EqtVEN(vl_modeAbonnement,
			                           (vl_modeAbonnement == XZEAC_DEBUT_ABONNEMENT) ? vl_dg_mce : NULL,
						   itre_ax_Abt_Etat_Equipement,
						   vl_nom,
						   XDC_EQT_VEN,
						   vl_typeEqt,
						   vl_numEqt,
						   &vl_cr)!=XDC_OK)
		      { XZST_03EcritureTrace(XZSTC_WARNING,"%s : tache relais: erreur dans XZEA44\n", version);
			break; }
			
			/** appel a XZEA55**/
		      if (XZEA55_Abt_Etat_Domaine(vl_modeAbonnement,
			                           (vl_modeAbonnement == XZEAC_DEBUT_ABONNEMENT) ? vl_dg_mce : NULL,
						   itre_ax_Abt_Etat_Domaine,
						   vl_nom,
						   XDC_EQT_VEN,
						   vl_typeEqt,
						   vl_numEqt,
						   &vl_cr)!=XDC_OK)
		      { XZST_03EcritureTrace(XZSTC_WARNING,"%s : tache relais: erreur dans XZEA55\n", version);
			break; }
			
			/** appel a XZEA55**/
			if (XZEA55_Abt_Etat_Domaine(vl_modeAbonnement,
								   (vl_modeAbonnement == XZEAC_DEBUT_ABONNEMENT) ? vl_dg_mce : NULL,
					   itre_ax_Abt_Etat_Domaine,
					   vl_nom,
					   XDC_EQT_ECL,
					   vl_typeEqt,
					   vl_numEqt,
					   &vl_cr)!=XDC_OK)
			{ XZST_03EcritureTrace(XZSTC_WARNING,"%s : tache relais: erreur dans XZEA55\n", version);
			break; }					
		break;
		
		/** abonnement des equipements du domaine eclairage **/
		case(ITRE_AX_DOMAINE_ECL) :
			/*** appel de la fonction XZEA43*/
		      /** lecture des arguments **/
		      if (sscanf(pa_arg,"%d|%[^|]|%d|%d",&vl_modeAbonnement,
		      				         vl_nom,
		      				         &vl_typeEqt,
		      				         &vl_numEqt)!=4)
		      { XZST_03EcritureTrace(XZSTC_WARNING,"%s : %s est non chaine",version,pa_arg);
		        break; }

		      XZST_03EcritureTrace(XZSTC_MESSAGE,"XZEA18->XZEA43 et XZEA55 Abonnement Domaine ECL Abt:%d Nom:%s TypeEqt:%d NumEqt:%d",
		                                 vl_modeAbonnement,
		      				               vl_nom,
		      				               vl_typeEqt,
		      				               vl_numEqt);
		       
		      /** appel a XZEA43**/
		      if (XZEA43_Abt_Etat_EqtECL(vl_modeAbonnement,
			                           (vl_modeAbonnement == XZEAC_DEBUT_ABONNEMENT) ? vl_dg_mce : NULL,
						   itre_ax_Abt_Etat_Equipement,
						   vl_nom,
						   XDC_EQT_ECL,
						   vl_typeEqt,
						   vl_numEqt,
						   &vl_cr)!=XDC_OK)
		      { XZST_03EcritureTrace(XZSTC_WARNING,"%s : tache relais: erreur dans XZEA53\n", version);
			break; }
			
			/** appel a XZEA55**/
		      if (XZEA55_Abt_Etat_Domaine(vl_modeAbonnement,
			                           (vl_modeAbonnement == XZEAC_DEBUT_ABONNEMENT) ? vl_dg_mce : NULL,
						   itre_ax_Abt_Etat_Domaine,
						   vl_nom,
						   XDC_EQT_ECL,
						   vl_typeEqt,
						   vl_numEqt,
						   &vl_cr)!=XDC_OK)
		      { XZST_03EcritureTrace(XZSTC_WARNING,"%s : tache relais: erreur dans XZEA55\n", version);
			break; }
			

			/** appel a XZEA55**/
		      if (XZEA55_Abt_Etat_Domaine(vl_modeAbonnement,
			                           (vl_modeAbonnement == XZEAC_DEBUT_ABONNEMENT) ? vl_dg_mce : NULL,
						   itre_ax_Abt_Etat_Domaine,
						   vl_nom,
						   XDC_EQT_VEN,
						   vl_typeEqt,
						   vl_numEqt,
						   &vl_cr)!=XDC_OK)
		      { XZST_03EcritureTrace(XZSTC_WARNING,"%s : tache relais: erreur dans XZEA55\n", version);
			break; }			
		break;
		
		/** abonnement des equipements du domaine energie **/
		case(ITRE_AX_DOMAINE_ENR) :
			/*** appel de la fonction XZEA45*/
		      /** lecture des arguments **/
		      if (sscanf(pa_arg,"%d|%[^|]|%d|%d",&vl_modeAbonnement,
		      				         vl_nom,
		      				         &vl_typeEqt,
		      				         &vl_numEqt)!=4)
		      { XZST_03EcritureTrace(XZSTC_WARNING,"%s : %s est non chaine",version,pa_arg);
		        break; }

		      XZST_03EcritureTrace(XZSTC_MESSAGE,"XZEA18->XZEA45 et XZEA55 Abonnement Domaine ENR Abt:%d Nom:%s TypeEqt:%d NumEqt:%d",
		                                 vl_modeAbonnement,
		      				               vl_nom,
		      				               vl_typeEqt,
		      				               vl_numEqt);

		      /** appel a XZEA45**/
		      if (XZEA45_Abt_Etat_EqtENR(vl_modeAbonnement,
			                           (vl_modeAbonnement == XZEAC_DEBUT_ABONNEMENT) ? vl_dg_mce : NULL,
						   itre_ax_Abt_Etat_Equipement,
						   vl_nom,
						   XDC_EQT_ENR,
						   vl_typeEqt,
						   vl_numEqt,
						   &vl_cr)!=XDC_OK)
		      { XZST_03EcritureTrace(XZSTC_WARNING,"%s : tache relais: erreur dans XZEA45\n", version);
			break; }
			
			/** appel a XZEA55**/
		      if (XZEA55_Abt_Etat_Domaine(vl_modeAbonnement,
			                           (vl_modeAbonnement == XZEAC_DEBUT_ABONNEMENT) ? vl_dg_mce : NULL,
						   itre_ax_Abt_Etat_Domaine,
						   vl_nom,
						   XDC_EQT_ENR,
						   vl_typeEqt,
						   vl_numEqt,
						   &vl_cr)!=XDC_OK)
		      { XZST_03EcritureTrace(XZSTC_WARNING,"%s : tache relais: erreur dans XZEA55\n", version);
			break; }
			
			/** appel a XZEA55**/
		      if (XZEA55_Abt_Etat_Domaine(vl_modeAbonnement,
			                           (vl_modeAbonnement == XZEAC_DEBUT_ABONNEMENT) ? vl_dg_mce : NULL,
						   itre_ax_Abt_Etat_Domaine,
						   vl_nom,
						   XDC_EQT_ECL,
						   vl_typeEqt,
						   vl_numEqt,
						   &vl_cr)!=XDC_OK)
		      { XZST_03EcritureTrace(XZSTC_WARNING,"%s : tache relais: erreur dans XZEA55\n", version);
			break; }
			

			/** appel a XZEA55**/
		      if (XZEA55_Abt_Etat_Domaine(vl_modeAbonnement,
			                           (vl_modeAbonnement == XZEAC_DEBUT_ABONNEMENT) ? vl_dg_mce : NULL,
						   itre_ax_Abt_Etat_Domaine,
						   vl_nom,
						   XDC_EQT_VEN,
						   vl_typeEqt,
						   vl_numEqt,
						   &vl_cr)!=XDC_OK)
		      { XZST_03EcritureTrace(XZSTC_WARNING,"%s : tache relais: erreur dans XZEA55\n", version);
			break; }				
			
		break;
		
		/** abonnement des equipements du domaine exploitation **/
		case(ITRE_AX_DOMAINE_EXP) :
			/*** appel de la fonction XZEA47*/
		      /** lecture des arguments **/
		      if (sscanf(pa_arg,"%d|%[^|]|%d|%d",&vl_modeAbonnement,
		      				         vl_nom,
		      				         &vl_typeEqt,
		      				         &vl_numEqt)!=4)
		      { XZST_03EcritureTrace(XZSTC_WARNING,"%s : %s est non chaine",version,pa_arg);
		        break; }

		      XZST_03EcritureTrace(XZSTC_MESSAGE,"XZEA18->XZEA47 et XZEA55 Abonnement Domaine EXP Abt:%d Nom:%s TypeEqt:%d NumEqt:%d",
		                                 vl_modeAbonnement,
		      				               vl_nom,
		      				               vl_typeEqt,
		      				               vl_numEqt);

		      /** appel a XZEA47**/
		      if (XZEA47_Abt_Etat_EqtEXP(vl_modeAbonnement,
			                           (vl_modeAbonnement == XZEAC_DEBUT_ABONNEMENT) ? vl_dg_mce : NULL,
						   itre_ax_Abt_Etat_Equipement,
						   vl_nom,
						   XDC_EQT_EXP,
						   vl_typeEqt,
						   vl_numEqt,
						   &vl_cr)!=XDC_OK)
		      { XZST_03EcritureTrace(XZSTC_WARNING,"%s : tache relais: erreur dans XZEA47\n", version);
			break; }
			
			/** appel a XZEA55**/
		      if (XZEA55_Abt_Etat_Domaine(vl_modeAbonnement,
			                           (vl_modeAbonnement == XZEAC_DEBUT_ABONNEMENT) ? vl_dg_mce : NULL,
						   itre_ax_Abt_Etat_Domaine,
						   vl_nom,
						   XDC_EQT_EXP,
						   vl_typeEqt,
						   vl_numEqt,
						   &vl_cr)!=XDC_OK)
		      { XZST_03EcritureTrace(XZSTC_WARNING,"%s : tache relais: erreur dans XZEA55\n", version);
			break; }
			
			/** appel a XZEA55**/
		      if (XZEA55_Abt_Etat_Domaine(vl_modeAbonnement,
			                           (vl_modeAbonnement == XZEAC_DEBUT_ABONNEMENT) ? vl_dg_mce : NULL,
						   itre_ax_Abt_Etat_Domaine,
						   vl_nom,
						   XDC_EQT_ECL,
						   vl_typeEqt,
						   vl_numEqt,
						   &vl_cr)!=XDC_OK)
		      { XZST_03EcritureTrace(XZSTC_WARNING,"%s : tache relais: erreur dans XZEA55\n", version);
			break; }
			

			/** appel a XZEA55**/
		      if (XZEA55_Abt_Etat_Domaine(vl_modeAbonnement,
			                           (vl_modeAbonnement == XZEAC_DEBUT_ABONNEMENT) ? vl_dg_mce : NULL,
						   itre_ax_Abt_Etat_Domaine,
						   vl_nom,
						   XDC_EQT_VEN,
						   vl_typeEqt,
						   vl_numEqt,
						   &vl_cr)!=XDC_OK)
		      { XZST_03EcritureTrace(XZSTC_WARNING,"%s : tache relais: erreur dans XZEA55\n", version);
			break; }				
		break;
		
		/** abonnement des equipements du domaine Lit d'arrets **/
		case(ITRE_AX_DOMAINE_EXP_LIT) :
			/*** appel de la fonction XZEA47*/
		      /** lecture des arguments **/
		      if (sscanf(pa_arg,"%d|%[^|]|%d|%d",&vl_modeAbonnement,
		      				         vl_nom,
		      				         &vl_typeEqt,
		      				         &vl_numEqt)!=4)
		      { XZST_03EcritureTrace(XZSTC_WARNING,"%s : %s est non chaine",version,pa_arg);
		        break; }

		      XZST_03EcritureTrace(XZSTC_MESSAGE,"XZEA18->XZEA47 et XZEA55 Abonnement Domaine EXP_LIT Abt:%d Nom:%s TypeEqt:%d NumEqt:%d",
		                                 vl_modeAbonnement,
		      				               vl_nom,
		      				               vl_typeEqt,
		      				               vl_numEqt);

		      /** appel a XZEA47**/
		      if (XZEA47_Abt_Etat_EqtEXP(vl_modeAbonnement,
			                           (vl_modeAbonnement == XZEAC_DEBUT_ABONNEMENT) ? vl_dg_mce : NULL,
						   itre_ax_Abt_Etat_Equipement,
						   vl_nom,
						   XDC_EQT_EXP,
						   vl_typeEqt,
						   vl_numEqt,
						   &vl_cr)!=XDC_OK)
		      { XZST_03EcritureTrace(XZSTC_WARNING,"%s : tache relais: erreur dans XZEA47\n", version);
			break; }
			
			/** appel a XZEA55**/
		      if (XZEA55_Abt_Etat_Domaine(vl_modeAbonnement,
			                           (vl_modeAbonnement == XZEAC_DEBUT_ABONNEMENT) ? vl_dg_mce : NULL,
						   itre_ax_Abt_Etat_Domaine,
						   vl_nom,
						   XDC_EQT_EXP,
						   vl_typeEqt,
						   vl_numEqt,
						   &vl_cr)!=XDC_OK)
		      { XZST_03EcritureTrace(XZSTC_WARNING,"%s : tache relais: erreur dans XZEA55\n", version);
			break; }
			
			/** appel a XZEA55**/
		      if (XZEA55_Abt_Etat_Domaine(vl_modeAbonnement,
			                           (vl_modeAbonnement == XZEAC_DEBUT_ABONNEMENT) ? vl_dg_mce : NULL,
						   itre_ax_Abt_Etat_Domaine,
						   vl_nom,
						   XDC_EQT_ECL,
						   vl_typeEqt,
						   vl_numEqt,
						   &vl_cr)!=XDC_OK)
		      { XZST_03EcritureTrace(XZSTC_WARNING,"%s : tache relais: erreur dans XZEA55\n", version);
			break; }
			

			/** appel a XZEA55**/
		      if (XZEA55_Abt_Etat_Domaine(vl_modeAbonnement,
			                           (vl_modeAbonnement == XZEAC_DEBUT_ABONNEMENT) ? vl_dg_mce : NULL,
						   itre_ax_Abt_Etat_Domaine,
						   vl_nom,
						   XDC_EQT_VEN,
						   vl_typeEqt,
						   vl_numEqt,
						   &vl_cr)!=XDC_OK)
		      { XZST_03EcritureTrace(XZSTC_WARNING,"%s : tache relais: erreur dans XZEA55\n", version);
			break; }				
		break;
		
		/** abonnement des equipements du domaine Locaux techniques **/
		case(ITRE_AX_DOMAINE_LTN) :
			/*** appel de la fonction XZEA48*/
		      /** lecture des arguments **/
		      if (sscanf(pa_arg,"%d|%[^|]|%d|%d",&vl_modeAbonnement,
		      				         vl_nom,
		      				         &vl_typeEqt,
		      				         &vl_numEqt)!=4)
		      { XZST_03EcritureTrace(XZSTC_WARNING,"%s : %s est non chaine",version,pa_arg);
		        break; }

		      XZST_03EcritureTrace(XZSTC_MESSAGE,"XZEA18->XZEA48 et XZEA55 Abonnement Domaine LTN Abt:%d Nom:%s TypeEqt:%d NumEqt:%d",
		                                 vl_modeAbonnement,
		      				               vl_nom,
		      				               vl_typeEqt,
		      				               vl_numEqt);

		      if (XZEA48_Abt_Etat_EqtLTN(vl_modeAbonnement,
			                           (vl_modeAbonnement == XZEAC_DEBUT_ABONNEMENT) ? vl_dg_mce : NULL,
						   itre_ax_Abt_Etat_Equipement,
						   vl_nom,
						   XDC_EQT_LTN,
						   vl_typeEqt,
						   vl_numEqt,
						   &vl_cr)!=XDC_OK)
		      { XZST_03EcritureTrace(XZSTC_WARNING,"%s : tache relais: erreur dans XZEA48\n", version);
			break; }
			
			/** appel a XZEA55**/
		      if (XZEA55_Abt_Etat_Domaine(vl_modeAbonnement,
			                           (vl_modeAbonnement == XZEAC_DEBUT_ABONNEMENT) ? vl_dg_mce : NULL,
						   itre_ax_Abt_Etat_Domaine,
						   vl_nom,
						   XDC_EQT_LTN,
						   vl_typeEqt,
						   vl_numEqt,
						   &vl_cr)!=XDC_OK)
		      { XZST_03EcritureTrace(XZSTC_WARNING,"%s : tache relais: erreur dans XZEA55\n", version);
			break; }
			
			/** appel a XZEA55**/
		      if (XZEA55_Abt_Etat_Domaine(vl_modeAbonnement,
			                           (vl_modeAbonnement == XZEAC_DEBUT_ABONNEMENT) ? vl_dg_mce : NULL,
						   itre_ax_Abt_Etat_Domaine,
						   vl_nom,
						   XDC_EQT_ECL,
						   vl_typeEqt,
						   vl_numEqt,
						   &vl_cr)!=XDC_OK)
		      { XZST_03EcritureTrace(XZSTC_WARNING,"%s : tache relais: erreur dans XZEA55\n", version);
			break; }
			

			/** appel a XZEA55**/
		      if (XZEA55_Abt_Etat_Domaine(vl_modeAbonnement,
			                           (vl_modeAbonnement == XZEAC_DEBUT_ABONNEMENT) ? vl_dg_mce : NULL,
						   itre_ax_Abt_Etat_Domaine,
						   vl_nom,
						   XDC_EQT_VEN,
						   vl_typeEqt,
						   vl_numEqt,
						   &vl_cr)!=XDC_OK)
		      { XZST_03EcritureTrace(XZSTC_WARNING,"%s : tache relais: erreur dans XZEA55\n", version);
			break; }				
		break;
		
		/** abonnement des equipements du domaine système **/
		case(ITRE_AX_DOMAINE_SYS) :
			/*** appel de la fonction XZEA49*/
		      /** lecture des arguments **/
		      if (sscanf(pa_arg,"%d|%[^|]|%d|%d",&vl_modeAbonnement,
		      				         vl_nom,
		      				         &vl_typeEqt,
		      				         &vl_numEqt)!=4)
		      { XZST_03EcritureTrace(XZSTC_WARNING,"%s : %s est non chaine",version,pa_arg);
		        break; }

		      XZST_03EcritureTrace(XZSTC_MESSAGE,"XZEA18->XZEA49 et XZEA55 Abonnement Domaine SYS Abt:%d Nom:%s TypeEqt:%d NumEqt:%d",
		                                 vl_modeAbonnement,
		      				               vl_nom,
		      				               vl_typeEqt,
		      				               vl_numEqt);

		      /** appel a XZEA49**/
		      if (XZEA49_Abt_Etat_EqtSYS(vl_modeAbonnement,
			                           (vl_modeAbonnement == XZEAC_DEBUT_ABONNEMENT) ? vl_dg_mce : NULL,
						   itre_ax_Abt_Etat_Equipement,
						   vl_nom,
						   XDC_EQT_SYS,
						   vl_typeEqt,
						   vl_numEqt,
						   &vl_cr)!=XDC_OK)
		      { XZST_03EcritureTrace(XZSTC_WARNING,"%s : tache relais: erreur dans XZEA49\n", version);
			break; }
			
			/** appel a XZEA55**/
		      if (XZEA55_Abt_Etat_Domaine(vl_modeAbonnement,
			                           (vl_modeAbonnement == XZEAC_DEBUT_ABONNEMENT) ? vl_dg_mce : NULL,
						   itre_ax_Abt_Etat_Domaine,
						   vl_nom,
						   XDC_EQT_SYS,
						   vl_typeEqt,
						   vl_numEqt,
						   &vl_cr)!=XDC_OK)
		      { XZST_03EcritureTrace(XZSTC_WARNING,"%s : tache relais: erreur dans XZEA55\n", version);
			break; }
			
			/** appel a XZEA55**/
		      if (XZEA55_Abt_Etat_Domaine(vl_modeAbonnement,
			                           (vl_modeAbonnement == XZEAC_DEBUT_ABONNEMENT) ? vl_dg_mce : NULL,
						   itre_ax_Abt_Etat_Domaine,
						   vl_nom,
						   XDC_EQT_ECL,
						   vl_typeEqt,
						   vl_numEqt,
						   &vl_cr)!=XDC_OK)
		      { XZST_03EcritureTrace(XZSTC_WARNING,"%s : tache relais: erreur dans XZEA55\n", version);
			break; }
			

			/** appel a XZEA55**/
		      if (XZEA55_Abt_Etat_Domaine(vl_modeAbonnement,
			                           (vl_modeAbonnement == XZEAC_DEBUT_ABONNEMENT) ? vl_dg_mce : NULL,
						   itre_ax_Abt_Etat_Domaine,
						   vl_nom,
						   XDC_EQT_VEN,
						   vl_typeEqt,
						   vl_numEqt,
						   &vl_cr)!=XDC_OK)
		      { XZST_03EcritureTrace(XZSTC_WARNING,"%s : tache relais: erreur dans XZEA55\n", version);
			break; }				
		break;
		
		/** abonnement des equipements pour les tubes non rénovés**/
		default :

			/*** appel de la fonction XZEA18 ***/
		      /** lecture des arguments **/
		     if (sscanf(pa_arg,"%d|%[^|]|%d|%d",&vl_modeAbonnement,
		      				         vl_nom,
		      				         &vl_typeEqt,
		      				         &vl_numEqt)!=4)
		      { XZST_03EcritureTrace(XZSTC_WARNING,"%s : %s est non chaine",version,pa_arg);
		        break; }

		      XZST_03EcritureTrace(XZSTC_MESSAGE,"XZEA18 Abonnement non renove Abt:%d Nom:%s TypeEqt:%d NumEqt:%d",
		                                 vl_modeAbonnement,
		      				               vl_nom,
		      				               vl_typeEqt,
		      				               vl_numEqt);

		      /** appel a XZEA18 **/
		      if (XZEA18_Abt_Etat_Panneaux(vl_modeAbonnement,
			                           (vl_modeAbonnement == XZEAC_DEBUT_ABONNEMENT) ? vl_dg_mce : NULL,
						   itre_ax_Abt_Etat_Panneaux,
						   vl_nom,
						   vl_typeEqt,
						   vl_numEqt,
						   &vl_cr)!=XDC_OK)
		      { XZST_03EcritureTrace(XZSTC_WARNING,"%s : tache relais: erreur dans XZEA18\n", version);
			break; }
		break;
		
		}
      break;




    /*** appel de la fonction XZEA22 ***/
    case (ITRE_AX_XZEA22):
    
      /** lecture des arguments **/
      if (sscanf(pa_arg,"%d|%[^|]|%d|%d",&vl_modeAbonnement,
      				         vl_nom,
      				         &vl_typeEqt,
      				         &vl_numEqt)!=4)
      { XZST_03EcritureTrace(XZSTC_WARNING,"%s : %s est non chaine",version,pa_arg);
        break; }

      /** appel a XZEA22 **/
      if (XZEA22_Abt_Etat_Telealarme(vl_modeAbonnement,
	                             (vl_modeAbonnement == XZEAC_DEBUT_ABONNEMENT) ? vl_dg_mce : NULL,
				     itre_ax_Abt_Telealarmes,
				     vl_nom,				   
				     vl_typeEqt,
				     vl_numEqt,
				     &vl_cr)!=XDC_OK)
      { XZST_03EcritureTrace(XZSTC_WARNING,"%s : erreur dans XZEA22\n", version);
	break; }
	
      break;




      /*** appel de la fonction XZEA23 ***/
    case (ITRE_AX_XZEA23):
		switch (va_numCmd)
		{
		/** abonnement à l'etat des séquences pour l'etat tunnel  du domaine signalisation **/
		case(ITRE_AX_DOMAINE_SIG) :
			 /** lecture des arguments **/
		      if (sscanf(pa_arg,"%d|%[^|]|%d",&vl_modeAbonnement,
		      				         vl_nom,
		      				         &vl_numEqt)!=3)
		      { XZST_03EcritureTrace(XZSTC_WARNING,"%s est non chaine",pa_arg);
		        break; }

		      XZST_03EcritureTrace(XZSTC_MESSAGE,"XZEA23->XZEA50 Abonnement Sequence etat Tunnel Domaine SIG Abt:%d Nom:%s NumEqt:%d",
		                                 vl_modeAbonnement,
		      				               vl_nom,
		      				               vl_numEqt);

		      /** appel a XZEA50**/
		      if (XZEA50_Abt_Etat_Sequence_SIG(vl_modeAbonnement,
			                                (vl_modeAbonnement == XZEAC_DEBUT_ABONNEMENT) ? vl_dg_mce : NULL,
						    	itre_ax_Abt_Etat_Seq_SIG,
						    	vl_nom,
								XDC_EQT_TUB,
						    	vl_numEqt,
						    	&vl_cr)!=XDC_OK)
						    	
		      { XZST_03EcritureTrace(XZSTC_WARNING,"tache relais: erreur dans XZEA50\n");
			break; }
		break;
		
		/** abonnement à l'etat des séquences pour l'etat tunnel  du domaine ventilation **/
		case(ITRE_AX_DOMAINE_VEN) :
			 /** lecture des arguments **/
		      if (sscanf(pa_arg,"%d|%[^|]|%d",&vl_modeAbonnement,
		      				         vl_nom,
		      				         &vl_numEqt)!=3)
		      { XZST_03EcritureTrace(XZSTC_WARNING,"%s est non chaine",pa_arg);
		        break; }

		      XZST_03EcritureTrace(XZSTC_MESSAGE,"XZEA23->XZEA52 Abonnement Sequence etat Tunnel Domaine VEN Abt:%d Nom:%s NumEqt:%d",
		                                 vl_modeAbonnement,
		      				               vl_nom,
		      				               vl_numEqt);

		      /** appel a XZEA52**/
		      if (XZEA52_Abt_Etat_Sequence_VEN(vl_modeAbonnement,
			                                (vl_modeAbonnement == XZEAC_DEBUT_ABONNEMENT) ? vl_dg_mce : NULL,
						    	itre_ax_Abt_Etat_Seq_VEN,
						    	vl_nom,
								XDC_EQT_TUB,
						    	vl_numEqt,
						    	&vl_cr)!=XDC_OK)
						    	
		      { XZST_03EcritureTrace(XZSTC_WARNING,"tache relais: erreur dans XZEA52\n");
			break; }
		break;		
		
		/** abonnement à l'etat des séquences pour l'etat tunnel  du domaine eclairage **/
		case(ITRE_AX_DOMAINE_ECL) :
			 /** lecture des arguments **/
		      if (sscanf(pa_arg,"%d|%[^|]|%d",&vl_modeAbonnement,
		      				         vl_nom,
		      				         &vl_numEqt)!=3)
		      { XZST_03EcritureTrace(XZSTC_WARNING,"%s est non chaine",pa_arg);
		        break; }

		      XZST_03EcritureTrace(XZSTC_MESSAGE,"XZEA23->XZEA51 Abonnement Sequence etat Tunnel Domaine ECL Abt:%d Nom:%s NumEqt:%d",
		                                 vl_modeAbonnement,
		      				               vl_nom,
		      				               vl_numEqt);

		      /** appel a XZEA51**/
		      if (XZEA51_Abt_Etat_Sequence_ECL(vl_modeAbonnement,
			                                (vl_modeAbonnement == XZEAC_DEBUT_ABONNEMENT) ? vl_dg_mce : NULL,
						    	itre_ax_Abt_Etat_Seq_ECL,
						    	vl_nom,
								XDC_EQT_TUB,
						    	vl_numEqt,
						    	&vl_cr)!=XDC_OK)
						    	
		      { XZST_03EcritureTrace(XZSTC_WARNING,"tache relais: erreur dans XZEA51\n");
			break; }
		break;
		
		
		/** abonnement à l'etat des séquences pour l'etat tunnel  du domaine energie**/
		case(ITRE_AX_DOMAINE_ENR) :
			 /** lecture des arguments **/
		      if (sscanf(pa_arg,"%d|%[^|]|%d",&vl_modeAbonnement,
		      				         vl_nom,
		      				         &vl_numEqt)!=3)
		      { XZST_03EcritureTrace(XZSTC_WARNING,"%s est non chaine",pa_arg);
		        break; }

		      XZST_03EcritureTrace(XZSTC_MESSAGE,"XZEA23->XZEA53 Abonnement Sequence etat Tunnel Domaine ENR Abt:%d Nom:%s NumEqt:%d",
		                                 vl_modeAbonnement,
		      				               vl_nom,
		      				               vl_numEqt);

		      /** appel a XZEA53**/
		      if (XZEA53_Abt_Etat_Sequence_ENR(vl_modeAbonnement,
			                                (vl_modeAbonnement == XZEAC_DEBUT_ABONNEMENT) ? vl_dg_mce : NULL,
						    	itre_ax_Abt_Etat_Seq_ENR,
						    	vl_nom,
								XDC_EQT_TUB,
						    	vl_numEqt,
						    	&vl_cr)!=XDC_OK)
						    	
		      { XZST_03EcritureTrace(XZSTC_WARNING,"tache relais: erreur dans XZEA53\n");
			break; }
		break;
		
		/** abonnement à l'etat des séquences pour l'etat tunnel  du domaine exploitation**/
		case(ITRE_AX_DOMAINE_EXP) :
			 /** lecture des arguments **/
		      if (sscanf(pa_arg,"%d|%[^|]|%d",&vl_modeAbonnement,
		      				         vl_nom,
		      				         &vl_numEqt)!=3)
		      { XZST_03EcritureTrace(XZSTC_WARNING,"%s est non chaine",pa_arg);
		        break; }

		      XZST_03EcritureTrace(XZSTC_MESSAGE,"XZEA23->XZEA53 Abonnement Sequence etat Tunnel Domaine EXP Abt:%d Nom:%s NumEqt:%d",
		                                 vl_modeAbonnement,
		      				               vl_nom,
		      				               vl_numEqt);

		      /** appel a XZEA56**/
		      if (XZEA56_Abt_Etat_Sequence_EXP(vl_modeAbonnement,
			                                (vl_modeAbonnement == XZEAC_DEBUT_ABONNEMENT) ? vl_dg_mce : NULL,
						    	itre_ax_Abt_Etat_Seq_EXP,
						    	vl_nom,
								XDC_EQT_TUB,
						    	vl_numEqt,
						    	&vl_cr)!=XDC_OK)
						    	
		      { XZST_03EcritureTrace(XZSTC_WARNING,"tache relais: erreur dans XZEA53\n");
			break; }
		break;
		
		/** abonnement à l'etat des séquences pour l'etat tunnel  du domaine protection incendie**/
		case(ITRE_AX_DOMAINE_INC) :
			 /** lecture des arguments **/
		      if (sscanf(pa_arg,"%d|%[^|]|%d",&vl_modeAbonnement,
		      				         vl_nom,
		      				         &vl_numEqt)!=3)
		      { XZST_03EcritureTrace(XZSTC_WARNING,"%s est non chaine",pa_arg);
		        break; }

		      XZST_03EcritureTrace(XZSTC_MESSAGE,"XZEA23->XZEA53 Abonnement Sequence etat Tunnel Domaine INC Abt:%d Nom:%s NumEqt:%d",
		                                 vl_modeAbonnement,
		      				               vl_nom,
		      				               vl_numEqt);

		      /** appel a XZEA54**/
		      if (XZEA54_Abt_Etat_Sequence_INC(vl_modeAbonnement,
			                                (vl_modeAbonnement == XZEAC_DEBUT_ABONNEMENT) ? vl_dg_mce : NULL,
						    	itre_ax_Abt_Etat_Seq_INC,
						    	vl_nom,
								XDC_EQT_TUB,
						    	vl_numEqt,
						    	&vl_cr)!=XDC_OK)
						    	
		      { XZST_03EcritureTrace(XZSTC_WARNING,"tache relais: erreur dans XZEA53\n");
			break; }
		break;		
		
		/** abonnement à l'etat des séquences pour l'etat tunnel  des tubes non rénovés **/		
		default :
		
			/** lecture des arguments **/
		     if (sscanf(pa_arg,"%d|%[^|]|%d",&vl_modeAbonnement,
		      				         vl_nom,
		      				         &vl_numEqt)!=3)
		      { XZST_03EcritureTrace(XZSTC_WARNING,"%s est non chaine",pa_arg);
		        break; }

		      XZST_03EcritureTrace(XZSTC_MESSAGE,"XZEA23 Abonnement etat Tunnel non renove Abt:%d Nom:%s NumEqt:%d",
		                                 vl_modeAbonnement,
		      				               vl_nom,
		      				               vl_numEqt);

		      /** appel a XZEA23 **/
		     if (XZEA23_Abt_Etat_Sequence_Tube(vl_modeAbonnement,
			                                (vl_modeAbonnement == XZEAC_DEBUT_ABONNEMENT) ? vl_dg_mce : NULL,
						    	itre_ax_Abt_Etat_Seq_Tun,
						    	vl_nom,
						    	vl_numEqt,
						    	&vl_cr)!=XDC_OK)
						    	
		      { XZST_03EcritureTrace(XZSTC_WARNING,"tache relais: erreur dans XZEA23\n");
			break; }			
		break;
		}
			 
      break;




       /*** appel de la fonction XZEA24 ***/
    case (ITRE_AX_XZEA24):
		switch(va_numCmd)
		{
		/** abonnement à l'etat des séquences pour l'etat echangeur  du domaine signalisation **/
		case(ITRE_AX_DOMAINE_SIG) :
			 /** lecture des arguments **/
		      if (sscanf(pa_arg,"%d|%[^|]|%d",&vl_modeAbonnement,
		      				         vl_nom,
		      				         &vl_numEqt)!=3)
		      { XZST_03EcritureTrace(XZSTC_WARNING,"%s est non chaine",pa_arg);
		        break; }

		      XZST_03EcritureTrace(XZSTC_MESSAGE,"XZEA24->XZEA50 Abonnement Sequence etat Echangeur Domaine ENR Abt:%d Nom:%s NumEqt:%d",
		                                 vl_modeAbonnement,
		      				               vl_nom,
		      				               vl_numEqt);

		      /** appel a XZEA50**/
		      if (XZEA50_Abt_Etat_Sequence_SIG(vl_modeAbonnement,
			                                (vl_modeAbonnement == XZEAC_DEBUT_ABONNEMENT) ? vl_dg_mce : NULL,
						    	itre_ax_Abt_Etat_Seq_SIG,
						    	vl_nom,
								XDC_EQT_ECH,
						    	vl_numEqt,
						    	&vl_cr)!=XDC_OK)
						    	
		      { XZST_03EcritureTrace(XZSTC_WARNING,"tache relais: erreur dans XZEA50\n");
			break; }
		break;
		
		/** abonnement à l'etat des séquences pour l'etat echangeur  du domaine ventilation **/
		case(ITRE_AX_DOMAINE_VEN) :
			 /** lecture des arguments **/
		      if (sscanf(pa_arg,"%d|%[^|]|%d",&vl_modeAbonnement,
		      				         vl_nom,
		      				         &vl_numEqt)!=3)
		      { XZST_03EcritureTrace(XZSTC_WARNING,"%s est non chaine",pa_arg);
		        break; }

		      XZST_03EcritureTrace(XZSTC_MESSAGE,"XZEA24->XZEA52 Abonnement Sequence etat Echangeur Domaine VEN Abt:%d Nom:%s NumEqt:%d",
		                                 vl_modeAbonnement,
		      				               vl_nom,
		      				               vl_numEqt);

		      /** appel a XZEA52**/
		      if (XZEA52_Abt_Etat_Sequence_VEN(vl_modeAbonnement,
			                                (vl_modeAbonnement == XZEAC_DEBUT_ABONNEMENT) ? vl_dg_mce : NULL,
						    	itre_ax_Abt_Etat_Seq_VEN,
						    	vl_nom,
								XDC_EQT_ECH,
						    	vl_numEqt,
						    	&vl_cr)!=XDC_OK)
						    	
		      { XZST_03EcritureTrace(XZSTC_WARNING,"tache relais: erreur dans XZEA52\n");
			break; }
		break;		
		
		/** abonnement à l'etat des séquences pour l'etat echangeur  du domaine eclairage **/
		case(ITRE_AX_DOMAINE_ECL) :
			 /** lecture des arguments **/
		      if (sscanf(pa_arg,"%d|%[^|]|%d",&vl_modeAbonnement,
		      				         vl_nom,
		      				         &vl_numEqt)!=3)
		      { XZST_03EcritureTrace(XZSTC_WARNING,"%s est non chaine",pa_arg);
		        break; }

		      XZST_03EcritureTrace(XZSTC_MESSAGE,"XZEA24->XZEA51 Abonnement Sequence etat Echangeur Domaine ECL Abt:%d Nom:%s NumEqt:%d",
		                                 vl_modeAbonnement,
		      				               vl_nom,
		      				               vl_numEqt);

		      /** appel a XZEA51**/
		      if (XZEA51_Abt_Etat_Sequence_ECL(vl_modeAbonnement,
			                                (vl_modeAbonnement == XZEAC_DEBUT_ABONNEMENT) ? vl_dg_mce : NULL,
						    	itre_ax_Abt_Etat_Seq_ECL,
						    	vl_nom,
								XDC_EQT_ECH,
						    	vl_numEqt,
						    	&vl_cr)!=XDC_OK)
						    	
		      { XZST_03EcritureTrace(XZSTC_WARNING,"tache relais: erreur dans XZEA51\n");
			break; }
		break;
		
		
		/** abonnement à l'etat des séquences pour l'etat echangeur  du domaine energie **/
		case(ITRE_AX_DOMAINE_ENR) :
			 /** lecture des arguments **/
		      if (sscanf(pa_arg,"%d|%[^|]|%d",&vl_modeAbonnement,
		      				         vl_nom,
		      				         &vl_numEqt)!=3)
		      { XZST_03EcritureTrace(XZSTC_WARNING,"%s est non chaine",pa_arg);
		        break; }

		      XZST_03EcritureTrace(XZSTC_MESSAGE,"XZEA24->XZEA53 Abonnement Sequence etat Echangeur Domaine ENR Abt:%d Nom:%s NumEqt:%d",
		                                 vl_modeAbonnement,
		      				               vl_nom,
		      				               vl_numEqt);

		      /** appel a XZEA53**/
		      if (XZEA53_Abt_Etat_Sequence_ENR(vl_modeAbonnement,
			                                (vl_modeAbonnement == XZEAC_DEBUT_ABONNEMENT) ? vl_dg_mce : NULL,
						    	itre_ax_Abt_Etat_Seq_ENR,
						    	vl_nom,
								XDC_EQT_ECH,
						    	vl_numEqt,
						    	&vl_cr)!=XDC_OK)
						    	
		      { XZST_03EcritureTrace(XZSTC_WARNING,"tache relais: erreur dans XZEA53\n");
			break; }
		break;
		
		/** abonnement à l'etat des séquences pour l'etat echangeur  du domaine exploitation **/
		case(ITRE_AX_DOMAINE_EXP) :
			 /** lecture des arguments **/
		      if (sscanf(pa_arg,"%d|%[^|]|%d",&vl_modeAbonnement,
		      				         vl_nom,
		      				         &vl_numEqt)!=3)
		      { XZST_03EcritureTrace(XZSTC_WARNING,"%s est non chaine",pa_arg);
		        break; }

		      XZST_03EcritureTrace(XZSTC_MESSAGE,"XZEA24->XZEA53 Abonnement Sequence etat Echangeur Domaine EXP Abt:%d Nom:%s NumEqt:%d",
		                                 vl_modeAbonnement,
		      				               vl_nom,
		      				               vl_numEqt);

		      /** appel a XZEA56**/
		      if (XZEA56_Abt_Etat_Sequence_EXP(vl_modeAbonnement,
			                                (vl_modeAbonnement == XZEAC_DEBUT_ABONNEMENT) ? vl_dg_mce : NULL,
						    	itre_ax_Abt_Etat_Seq_EXP,
						    	vl_nom,
								XDC_EQT_ECH,
						    	vl_numEqt,
						    	&vl_cr)!=XDC_OK)
						    	
		      { XZST_03EcritureTrace(XZSTC_WARNING,"tache relais: erreur dans XZEA53\n");
			break; }
		break;	
		
		/** abonnement à l'etat des séquences pour l'etat echangeur  du domaine exploitation **/
		case(ITRE_AX_DOMAINE_INC) :
			 /** lecture des arguments **/
		      if (sscanf(pa_arg,"%d|%[^|]|%d",&vl_modeAbonnement,
		      				         vl_nom,
		      				         &vl_numEqt)!=3)
		      { XZST_03EcritureTrace(XZSTC_WARNING,"%s est non chaine",pa_arg);
		        break; }

		      XZST_03EcritureTrace(XZSTC_MESSAGE,"XZEA24->XZEA53 Abonnement Sequence etat Echangeur Domaine INC Abt:%d Nom:%s NumEqt:%d",
		                                 vl_modeAbonnement,
		      				               vl_nom,
		      				               vl_numEqt);

		      /** appel a XZEA54**/
		      if (XZEA54_Abt_Etat_Sequence_INC(vl_modeAbonnement,
			                                (vl_modeAbonnement == XZEAC_DEBUT_ABONNEMENT) ? vl_dg_mce : NULL,
						    	itre_ax_Abt_Etat_Seq_INC,
						    	vl_nom,
								XDC_EQT_ECH,
						    	vl_numEqt,
						    	&vl_cr)!=XDC_OK)
						    	
		      { XZST_03EcritureTrace(XZSTC_WARNING,"tache relais: erreur dans XZEA53\n");
			break; }
		break;			
		/** abonnement à l'etat des séquences pour l'etat echangeur  des tubes non rénovés **/
		default :
		
			/** lecture des arguments **/
		      if (sscanf(pa_arg,"%d|%[^|]|%d",&vl_modeAbonnement,
		      				         vl_nom,
		      				         &vl_numEqt)!=3)
		      { XZST_03EcritureTrace(XZSTC_WARNING,"%s est non chaine",pa_arg);
		        break; }

		      XZST_03EcritureTrace(XZSTC_MESSAGE,"XZEA24 Abonnement Sequence etat Echangeur non renove Abt:%d Nom:%s NumEqt:%d",
		                                 vl_modeAbonnement,
		      				               vl_nom,
		      				               vl_numEqt);

		      /** appel a XZEA24 **/
		      if (XZEA24_Abt_Etat_Sequence_Ech(vl_modeAbonnement,
			                                     (vl_modeAbonnement == XZEAC_DEBUT_ABONNEMENT) ? vl_dg_mce : NULL,
						    	     itre_ax_Abt_Etat_Seq_Ech,
						    	     vl_nom,
						    	     vl_numEqt,
						    	     &vl_cr)!=XDC_OK)
		      { XZST_03EcritureTrace(XZSTC_WARNING,"tache relais: erreur dans XZEA24\n");
			break; }
		break;
		
		}
    
      break;




	/** Appel de la fonction d'abonnement aux sequences pour les  des LTN **/
	case (ITRE_AX_XZEA112):
		switch(va_numCmd)
		{
		/** abonnement à l'etat des séquences pour l'etat LTN du domaine signalisation **/
		case(ITRE_AX_DOMAINE_SIG) :
			 /** lecture des arguments **/
		      if (sscanf(pa_arg,"%d|%[^|]|%d",&vl_modeAbonnement,
		      				         vl_nom,
		      				         &vl_numEqt)!=3)
		      { XZST_03EcritureTrace(XZSTC_WARNING,"%s est non chaine",pa_arg);
		        break; }

		      /** appel a XZEA50**/
		      if (XZEA50_Abt_Etat_Sequence_SIG(vl_modeAbonnement,
			                                (vl_modeAbonnement == XZEAC_DEBUT_ABONNEMENT) ? vl_dg_mce : NULL,
						    	itre_ax_Abt_Etat_Seq_SIG,
						    	vl_nom,
								XDC_EQT_LTN,
						    	vl_numEqt,
						    	&vl_cr)!=XDC_OK)
						    	
		      { XZST_03EcritureTrace(XZSTC_WARNING,"tache relais: erreur dans XZEA50\n");
			break; }
		break;
		
		/** abonnement à l'etat des séquences pour l'etat LTN du domaine ventilation **/
		case(ITRE_AX_DOMAINE_VEN) :
			 /** lecture des arguments **/
		      if (sscanf(pa_arg,"%d|%[^|]|%d",&vl_modeAbonnement,
		      				         vl_nom,
		      				         &vl_numEqt)!=3)
		      { XZST_03EcritureTrace(XZSTC_WARNING,"%s est non chaine",pa_arg);
		        break; }

		      /** appel a XZEA52**/
		      if (XZEA52_Abt_Etat_Sequence_VEN(vl_modeAbonnement,
			                                (vl_modeAbonnement == XZEAC_DEBUT_ABONNEMENT) ? vl_dg_mce : NULL,
						    	itre_ax_Abt_Etat_Seq_VEN,
						    	vl_nom,
								XDC_EQT_LTN,
						    	vl_numEqt,
						    	&vl_cr)!=XDC_OK)
						    	
		      { XZST_03EcritureTrace(XZSTC_WARNING,"tache relais: erreur dans XZEA52\n");
			break; }
		break;		
		
		/** abonnement à l'etat des séquences pour l'etat LTN du domaine eclairage **/
		case(ITRE_AX_DOMAINE_ECL) :
			 /** lecture des arguments **/
		      if (sscanf(pa_arg,"%d|%[^|]|%d",&vl_modeAbonnement,
		      				         vl_nom,
		      				         &vl_numEqt)!=3)
		      { XZST_03EcritureTrace(XZSTC_WARNING,"%s est non chaine",pa_arg);
		        break; }

		      /** appel a XZEA51**/
		      if (XZEA51_Abt_Etat_Sequence_ECL(vl_modeAbonnement,
			                                (vl_modeAbonnement == XZEAC_DEBUT_ABONNEMENT) ? vl_dg_mce : NULL,
						    	itre_ax_Abt_Etat_Seq_ECL,
						    	vl_nom,
								XDC_EQT_LTN,
						    	vl_numEqt,
						    	&vl_cr)!=XDC_OK)
						    	
		      { XZST_03EcritureTrace(XZSTC_WARNING,"tache relais: erreur dans XZEA51\n");
			break; }
		break;
		
		/** abonnement à l'etat des séquences pour l'etat LTN du domaine energie **/		
		case(ITRE_AX_DOMAINE_ENR) :
			 /** lecture des arguments **/
		      if (sscanf(pa_arg,"%d|%[^|]|%d",&vl_modeAbonnement,
		      				         vl_nom,
		      				         &vl_numEqt)!=3)
		      { XZST_03EcritureTrace(XZSTC_WARNING,"%s est non chaine",pa_arg);
		        break; }

		      /** appel a XZEA53**/
		      if (XZEA53_Abt_Etat_Sequence_ENR(vl_modeAbonnement,
			                                (vl_modeAbonnement == XZEAC_DEBUT_ABONNEMENT) ? vl_dg_mce : NULL,
						    	itre_ax_Abt_Etat_Seq_ENR,
						    	vl_nom,
								XDC_EQT_LTN,
						    	vl_numEqt,
						    	&vl_cr)!=XDC_OK)
						    	
		      { XZST_03EcritureTrace(XZSTC_WARNING,"tache relais: erreur dans XZEA53\n");
			break; }
		break;
		
		/** abonnement à l'etat des séquences pour l'etat LTN du domaine exploitation **/		
		case(ITRE_AX_DOMAINE_EXP) :
			 /** lecture des arguments **/
		      if (sscanf(pa_arg,"%d|%[^|]|%d",&vl_modeAbonnement,
		      				         vl_nom,
		      				         &vl_numEqt)!=3)
		      { XZST_03EcritureTrace(XZSTC_WARNING,"%s est non chaine",pa_arg);
		        break; }

		      /** appel a XZEA56**/
		      if (XZEA56_Abt_Etat_Sequence_EXP(vl_modeAbonnement,
			                                (vl_modeAbonnement == XZEAC_DEBUT_ABONNEMENT) ? vl_dg_mce : NULL,
						    	itre_ax_Abt_Etat_Seq_EXP,
						    	vl_nom,
								XDC_EQT_LTN,
						    	vl_numEqt,
						    	&vl_cr)!=XDC_OK)
						    	
		      { XZST_03EcritureTrace(XZSTC_WARNING,"tache relais: erreur dans XZEA53\n");
			break; }
		break;
		
		/** abonnement à l'etat des séquences pour l'etat LTN du domaine exploitation **/		
		case(ITRE_AX_DOMAINE_INC) :
			 /** lecture des arguments **/
		      if (sscanf(pa_arg,"%d|%[^|]|%d",&vl_modeAbonnement,
		      				         vl_nom,
		      				         &vl_numEqt)!=3)
		      { XZST_03EcritureTrace(XZSTC_WARNING,"%s est non chaine",pa_arg);
		        break; }

		      /** appel a XZEA54**/
		      if (XZEA54_Abt_Etat_Sequence_INC(vl_modeAbonnement,
			                                (vl_modeAbonnement == XZEAC_DEBUT_ABONNEMENT) ? vl_dg_mce : NULL,
						    	itre_ax_Abt_Etat_Seq_INC,
						    	vl_nom,
								XDC_EQT_LTN,
						    	vl_numEqt,
						    	&vl_cr)!=XDC_OK)
						    	
		      { XZST_03EcritureTrace(XZSTC_WARNING,"tache relais: erreur dans XZEA53\n");
			break; }
		break;		
		/** abonnement à l'etat des séquences pour l'etat LTN des tubes non rénovés**/
		default :
		
			/** lecture des arguments **/
		      if (sscanf(pa_arg,"%d|%[^|]|%d",&vl_modeAbonnement,
		      				         vl_nom,
		      				         &vl_numEqt)!=3)
		      { XZST_03EcritureTrace(XZSTC_WARNING,"%s est non chaine",pa_arg);
		        break; }

		      /** appel a XZEA24 **/
		      if (XZEA24_Abt_Etat_Sequence_Ech(vl_modeAbonnement,
			                                     (vl_modeAbonnement == XZEAC_DEBUT_ABONNEMENT) ? vl_dg_mce : NULL,
						    	     itre_ax_Abt_Etat_Seq_Ech,
						    	     vl_nom,
						    	     vl_numEqt,
						    	     &vl_cr)!=XDC_OK)
		      { XZST_03EcritureTrace(XZSTC_WARNING,"tache relais: erreur dans XZEA24\n");
			break; }
		break;
		
		}
    
      break;
	
	/** Appel de la fonction abonnement séquences HT  **/	
	case (ITRE_AX_XZEA113):
		switch(va_numCmd)
		{
			/** abonnement à l'etat des séquences pour l'etat HT du domaine signalisation **/
			case(ITRE_AX_DOMAINE_SIG) :
				/** lecture des arguments **/
				if (sscanf(pa_arg,"%d|%[^|]|%d",&vl_modeAbonnement,
				      				vl_nom,
								&vl_numEqt)!=3)
				   { XZST_03EcritureTrace(XZSTC_WARNING,"%s est non chaine",pa_arg);
				    break; }

				      /** appel a XZEA50**/
				if (XZEA50_Abt_Etat_Sequence_SIG(vl_modeAbonnement,
					                        (vl_modeAbonnement == XZEAC_DEBUT_ABONNEMENT) ? vl_dg_mce : NULL,
								itre_ax_Abt_Etat_Seq_SIG,
								vl_nom,
								XDC_EQT_ENR,
							 	vl_numEqt,
								&vl_cr)!=XDC_OK)
								    	
					{ XZST_03EcritureTrace(XZSTC_WARNING,"tache relais: erreur dans XZEA50\n");
					break; }
			break;
			
			/** abonnement à l'etat des séquences pour l'etat HT du domaine ventilation **/			
			case(ITRE_AX_DOMAINE_VEN) :
					 /** lecture des arguments **/
				if (sscanf(pa_arg,"%d|%[^|]|%d",&vl_modeAbonnement,
				      				vl_nom,
				      				&vl_numEqt)!=3)
					{ XZST_03EcritureTrace(XZSTC_WARNING,"%s est non chaine",pa_arg);
				     break; }

				      /** appel a XZEA52**/
				if (XZEA52_Abt_Etat_Sequence_VEN(vl_modeAbonnement,
					                        (vl_modeAbonnement == XZEAC_DEBUT_ABONNEMENT) ? vl_dg_mce : NULL,
								itre_ax_Abt_Etat_Seq_VEN,
								vl_nom,
								XDC_EQT_ENR,
								vl_numEqt,
								&vl_cr)!=XDC_OK)
								    	
				    { XZST_03EcritureTrace(XZSTC_WARNING,"tache relais: erreur dans XZEA52\n");
					break; }
			break;		
			
			/** abonnement à l'etat des séquences pour l'etat HT du domaine eclairage **/
			case(ITRE_AX_DOMAINE_ECL) :
					 /** lecture des arguments **/
				    if (sscanf(pa_arg,"%d|%[^|]|%d",&vl_modeAbonnement,
				      				 vl_nom,
				      				&vl_numEqt)!=3)
				     { XZST_03EcritureTrace(XZSTC_WARNING,"%s est non chaine",pa_arg);
				     break; }

				      /** appel a XZEA51**/
				     if (XZEA51_Abt_Etat_Sequence_ECL(vl_modeAbonnement,
					                            (vl_modeAbonnement == XZEAC_DEBUT_ABONNEMENT) ? vl_dg_mce : NULL,
								    itre_ax_Abt_Etat_Seq_ECL,
								    vl_nom,
								    XDC_EQT_ENR,
								    vl_numEqt,
								    &vl_cr)!=XDC_OK)
								    	
				      { XZST_03EcritureTrace(XZSTC_WARNING,"tache relais: erreur dans XZEA51\n");
					break; }
			break;
				
			/** abonnement à l'etat des séquences pour l'etat HT du domaine energie **/					
			case(ITRE_AX_DOMAINE_ENR) :
					 /** lecture des arguments **/
				    if (sscanf(pa_arg,"%d|%[^|]|%d",&vl_modeAbonnement,
				      				    vl_nom,
				      				   &vl_numEqt)!=3)
				      { XZST_03EcritureTrace(XZSTC_WARNING,"%s est non chaine",pa_arg);
				        break; }

				      /** appel a XZEA53**/
				    if (XZEA53_Abt_Etat_Sequence_ENR(vl_modeAbonnement,
					                             (vl_modeAbonnement == XZEAC_DEBUT_ABONNEMENT) ? vl_dg_mce : NULL,
								    	itre_ax_Abt_Etat_Seq_ENR,
								    	vl_nom,
										XDC_EQT_ENR,
								    	vl_numEqt,
								    	&vl_cr)!=XDC_OK)
								    	
				      { XZST_03EcritureTrace(XZSTC_WARNING,"tache relais: erreur dans XZEA53\n");
					break; }
			break;
			
			/** abonnement à l'etat des séquences pour l'etat HT du domaine exploitation **/					
			case(ITRE_AX_DOMAINE_EXP) :
					 /** lecture des arguments **/
				    if (sscanf(pa_arg,"%d|%[^|]|%d",&vl_modeAbonnement,
				      				    vl_nom,
				      				   &vl_numEqt)!=3)
				      { XZST_03EcritureTrace(XZSTC_WARNING,"%s est non chaine",pa_arg);
				        break; }

				      /** appel a XZEA56**/
				    if (XZEA56_Abt_Etat_Sequence_EXP(vl_modeAbonnement,
					                             (vl_modeAbonnement == XZEAC_DEBUT_ABONNEMENT) ? vl_dg_mce : NULL,
								    	itre_ax_Abt_Etat_Seq_EXP,
								    	vl_nom,
										XDC_EQT_EXP,
								    	vl_numEqt,
								    	&vl_cr)!=XDC_OK)
								    	
				      { XZST_03EcritureTrace(XZSTC_WARNING,"tache relais: erreur dans XZEA53\n");
					break; }
			break;
						
			/** abonnement à l'etat des séquences pour l'etat HT du domaine protection incendie **/					
			case(ITRE_AX_DOMAINE_INC) :
					 /** lecture des arguments **/
				    if (sscanf(pa_arg,"%d|%[^|]|%d",&vl_modeAbonnement,
				      				    vl_nom,
				      				   &vl_numEqt)!=3)
				      { XZST_03EcritureTrace(XZSTC_WARNING,"%s est non chaine",pa_arg);
				        break; }

				      /** appel a XZEA54**/
				    if (XZEA54_Abt_Etat_Sequence_INC(vl_modeAbonnement,
					                             (vl_modeAbonnement == XZEAC_DEBUT_ABONNEMENT) ? vl_dg_mce : NULL,
								    	itre_ax_Abt_Etat_Seq_INC,
								    	vl_nom,
										XDC_EQT_INC,
								    	vl_numEqt,
								    	&vl_cr)!=XDC_OK)
								    	
				      { XZST_03EcritureTrace(XZSTC_WARNING,"tache relais: erreur dans XZEA53\n");
					break; }
			break;						
			/** abonnement à l'etat des séquences pour l'etat HT des tubes non rénovés **/
			default :
				
					/** lecture des arguments **/
				    if (sscanf(pa_arg,"%d|%[^|]|%d",&vl_modeAbonnement,
				      				      vl_nom,
				      				     &vl_numEqt)!=3)
				    { XZST_03EcritureTrace(XZSTC_WARNING,"%s est non chaine",pa_arg);
				     break; }

				      /** appel a XZEA24 **/
				    if (XZEA24_Abt_Etat_Sequence_Ech(vl_modeAbonnement,
					                                     (vl_modeAbonnement == XZEAC_DEBUT_ABONNEMENT) ? vl_dg_mce : NULL,
								    	  itre_ax_Abt_Etat_Seq_Ech,
								    	  vl_nom,
								    	  vl_numEqt,
								    	  &vl_cr)!=XDC_OK)
				    { XZST_03EcritureTrace(XZSTC_WARNING,"tache relais: erreur dans XZEA24\n");
					break; }
			break;
				
		}
		    
	break;




       /*** appel de la fonction XZEA25 ( J.P.L 30/12/94 ) ***/
    case (ITRE_AX_XZEA25):
    
      /** lecture des arguments **/
      if (sscanf (pa_arg, "%d|%d|%s", &vl_modeAbonnement,
				      &vl_numEqt,
				      vl_nomSite) != 3)
      { XZST_03EcritureTrace(XZSTC_WARNING,
		"%s : Liste d'arguments invalide pour XZEA25 : \"%s\"\n", version, pa_arg);
        break; }

       XZST_03EcritureTrace(XZSTC_WARNING,"Liste d'arguments : %s",pa_arg);
      
      /** appel a XZEA25 **/
      if (XZEA25_Abt_Etat_PAU(vl_modeAbonnement,
	                         (vl_modeAbonnement == XZEAC_DEBUT_ABONNEMENT) ? vl_dg_mce : NULL,
				 itre_ax_Abt_Etat_PAU,
				 vl_nomSite,
				 vl_numEqt,
				 &vl_cr) != XDC_OK)
      { XZST_03EcritureTrace(XZSTC_WARNING,"%s : Tache relais: erreur dans XZEA25\n", version);
	break; }
	
      break;




   /*** appel de la fonction XZEA26 ***/
    case (ITRE_AX_XZEA26):
    
      /** lecture des arguments **/
      if (sscanf(pa_arg,"%d|%d",&vl_modeAbonnement,&vl_numEqt)!=2)
      				
      { XZST_03EcritureTrace(XZSTC_WARNING,"%s est non chaine",pa_arg);
        break; }

      /** appel a XZEA26 **/
      if (XZEA26_Abt_Etat_Mat_NAV(vl_modeAbonnement,
	                      (vl_modeAbonnement == XZEAC_DEBUT_ABONNEMENT) ? vl_dg_mce : NULL,
			      itre_ax_MAJ_etat_MAT_NAV,
			      vl_numEqt,&vl_cr)!=XDC_OK)
				   
      { XZST_03EcritureTrace(XZSTC_WARNING,"tache relais: erreur dans XZEA26\n");
	break; }
	
      break;




   /*** appel de la fonction XZEA28 ***/
    case (ITRE_AX_XZEA28):
    
      /** lecture des arguments **/
      if (sscanf(pa_arg,"%d|%s",&vl_modeAbonnement,
      				vl_nom)!=2)
      { XZST_03EcritureTrace(XZSTC_WARNING,"%s est non chaine",pa_arg);
        break; }

      /** appel a XZEA28 **/
      if (XZEA28_Abt_Etat_Contraste(vl_modeAbonnement,
	                            (vl_modeAbonnement == XZEAC_DEBUT_ABONNEMENT) ? vl_dg_mce : NULL,
				    itre_ax_piloterContraste,
				    vl_nom,&vl_cr)!=XDC_OK)
      { XZST_03EcritureTrace(XZSTC_WARNING,"tache relais: erreur dans XZEA28\n");
	break; }
	
      break;




       /*** appel de la fonction XZEA30 ( J.P.L 02/01/95 ) ***/
    case (ITRE_AX_XZEA30):
    
      /** lecture des arguments **/
      if (sscanf (pa_arg, "%d|%s", &vl_modeAbonnement,
				   vl_nomSite) != 2)
      { XZST_03EcritureTrace(XZSTC_WARNING,
		"Liste d'arguments invalide pour XZEA30 : \"%s\"\n", pa_arg);
        break; }

      /** appel a XZEA30 **/
      if (XZEA30_Abt_Etat_OPERATOR(vl_modeAbonnement,
				  XDG_NUL,
				  itre_ax_Abt_Etat_Operator,
				  vl_nomSite,
				  &vl_cr) != XDC_OK)
      { XZST_03EcritureTrace(XZSTC_WARNING,
				"Tache relais: erreur dans XZEA30\n");
	break; }
	
      break;



  case (ITRE_AX_XZEL16):
  	if (sscanf (pa_arg, "%d|%[^|]|%[^|]|%[^|]",
					&vl_type_fmc, /* type FMC*/
					vl_date, 	/*portable*/
					vl_CmdMAT,	/*depanneur*/
					vl_reponse) != 4) 	/*horodate*/
        { XZST_03EcritureTrace(XZSTC_WARNING,
			"Liste d'arguments invalide pour XZEL16 : \"%s\"\n", pa_arg);
		break; }
		if (strcmp(va_nomMachine,""))
			XZEL16_SMS(va_nomMachine, vl_type_fmc, vl_date, vl_CmdMAT, vl_reponse);
		else
			XZEL16_SMS(vg_nomMachine, vl_type_fmc, vl_date, vl_CmdMAT, vl_reponse);

	break;

       /*** appel de la fonction XZEL01 ( J.P.L 02/01/95 ) ***/
    case (ITRE_AX_XZEL01):
    
      /** lecture des arguments **/
      if (sscanf (pa_arg, "%d|%[^|]|%s", &vl_NumPosteOper,
					 vl_nomSite,
					 vl_NumTel1) != 3)
      { XZST_03EcritureTrace(XZSTC_WARNING,
		"Liste d'arguments invalide pour XZEL01 : \"%s\"\n", pa_arg);
        break; }

      /** appel a XZEL01 **/
      if (XZEL01_Demande_Appel_Tel (vl_nomSite,
				    vl_NumTel1,
				    (XDY_Operateur) vl_NumPosteOper) != XDC_OK)
      { XZST_03EcritureTrace(XZSTC_WARNING,
				"Tache relais: erreur dans XZEL01\n");
	break; }

      break;




       /*** appel fonction repondeur telephonique XZEL0n ( J.P.L 04/04/95 ) ***/
    case (ITRE_AX_XZEL02_03):
      if (sscanf (pa_arg, "%d|%d|%s", &vl_NumPosteOper,
					&vl_mode,
					vl_nomSite) != 3)
      { XZST_03EcritureTrace(XZSTC_WARNING,
		"Liste d'arguments invalide pour XZEL0n : \"%s\"\n", pa_arg);
        break; }

      /** renvoyer l'appel ou annuler le renvoi comme indique **/
      if (vl_mode == XDC_OUI)
	vl_cr = XZEL02_Renvoi_Appel (vl_nomSite, "", (XDY_Operateur) vl_NumPosteOper);
      else vl_cr = XZEL03_Annul_Renvoi (vl_nomSite, (XDY_Operateur) vl_NumPosteOper);

      if (vl_cr != XDC_OK)
      { XZST_03EcritureTrace (XZSTC_WARNING,
				"Tache relais: erreur dans XZEL0n\n");
	break; }

      break;



   case (ITRE_AX_XZEL04):

      /** lecture des arguments **/
      if (sscanf (pa_arg, "%d|%[^|]|%s", &vl_NumPosteOper,
                                         vl_nomSite,
                                         vl_NumTel1) != 3)
      { XZST_03EcritureTrace(XZSTC_WARNING,
                "Liste d'arguments invalide pour XZEL04 : \"%s\"\n", pa_arg);
        break; }

      /** appel a XZEL01 **/
      if (XZEL04_Mise_En_Garde (vl_nomSite,
                                    (XDY_Operateur) vl_NumPosteOper) != XDC_OK)
      { XZST_03EcritureTrace(XZSTC_WARNING,
                                "Tache relais: erreur dans XZEL01\n");
        break; }

      break;



    case (ITRE_AX_XZEL05):

      /** lecture des arguments **/
      if (sscanf (pa_arg, "%d|%[^|]|%s", &vl_NumPosteOper,
                                         vl_nomSite,
                                         vl_NumTel1) != 3)
      { XZST_03EcritureTrace(XZSTC_WARNING,
                "Liste d'arguments invalide pour XZEL01 : \"%s\"\n", pa_arg);
        break; }
      /** appel a XZEL05 **/
      if (XZEL05_Reprise (vl_nomSite,
                                    (XDY_Operateur) vl_NumPosteOper) != XDC_OK)
      { XZST_03EcritureTrace(XZSTC_WARNING,
                                "Tache relais: erreur dans XZEL01\n");
        break; }

      break;



    case (ITRE_AX_XZEL06) :
      if  (sscanf (pa_arg, "%d|%[^|]|%[^|]|%[^|]|%d",
                        &vl_NumPosteOper,
                        vl_nomSite,
                        vl_NumTel1,
                        vl_numero_groupe,
                        &vl_type_appel) != 5) {
          XZST_03EcritureTrace(XZSTC_WARNING,
          "Liste d'arguments invalide pour XZEL06 : \"%s\"\n", pa_arg);
        }
        if (XZEL06_Demande_Appel_Radio(vl_nomSite,
                                vl_type_appel,
                                vl_numero_groupe,
                                vl_NumTel1,
                                vl_NumPosteOper) != XDC_OK)
        {
                XZST_03EcritureTrace(XZSTC_WARNING,
                                "Tache relais: erreur dans XZEL06\n");
        }
        break;




       /*** appel de la fonction XZEO01 ( J.P.L 02/01/95 ) ***/
    case (ITRE_AX_XZEO01):
    
      /** lecture des arguments **/
      if (sscanf (pa_arg, "%[^|]|%[^|]|%s", vl_nomSite,
					    vl_NumTel1,
					    vl_NumTel2) != 3)
      { XZST_03EcritureTrace(XZSTC_WARNING,
		"Liste d'arguments invalide pour XZEO01 : \"%s\"\n", pa_arg);
        break; }

      /** appel a XZEO01 **/
      if (XZEO01_Appel_Op_Num (vl_nomSite,
				vl_NumTel1,
				vl_NumTel2) != XDC_OK)
      { XZST_03EcritureTrace(XZSTC_WARNING,
				"Tache relais: erreur dans XZEO01\n");
	break; }
	
      break;






       /*** appel de la fonction XZEO02 ( J.P.L 02/01/95 ) ***/
    case (ITRE_AX_XZEO02):
    
      /** lecture des arguments **/
      if (sscanf (pa_arg, "%[^|]|%[^|]|%[^|]", vl_nomSite,
						vl_NumTel1,
						vl_Message) != 3)
      { XZST_03EcritureTrace(XZSTC_WARNING,
		"Liste d'arguments invalide pour XZEO02 : \"%s\"\n", pa_arg);
        break; }

      /** appel a XZEO02 **/
      if (XZEO02_Appel_Op_AlphaNum (vl_nomSite,
				    vl_NumTel1,
				    vl_Message) != XDC_OK)
      { XZST_03EcritureTrace(XZSTC_WARNING,
				"Tache relais: erreur dans XZEO02\n");
	break; }
	
      break;






    /*** appel de la fonction XZEP01 ***/
    case (ITRE_AX_XZEP01):
    
      /** lecture des arguments **/
      if (sscanf(pa_arg,"%d|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%d|%d|%d|%d|%d|%[^|]",&vl_Numero,
                                                                                      vl_PMV.Texte1,
                                                                                      vl_PMV.Texte2,
                                                                                      vl_PMV.Texte3,
                                                                                      vl_PMV.Alternat1,
                                                                                      vl_PMV.Alternat2,
                                                                                      vl_PMV.Alternat3,
                                                                                      &vl_Clign1,
                                                                                      &vl_Clign2,
                                                                                      &vl_Clign3,
                                                                                      &vl_flash,
                                                                                      &vl_Cmd,
                                                                                      vl_nomMachine)!=13)
      				
      { XZST_03EcritureTrace(XZSTC_WARNING,"%s est non chaine",pa_arg);
        break; }
        
      vl_PMV.Numero = vl_Numero;
      vl_PMV.Clign1 = vl_Clign1;
      vl_PMV.Clign2 = vl_Clign2;
      vl_PMV.Clign3 = vl_Clign3;
      vl_PMV.Flash  = vl_flash;
      vl_PMV.Destination  = 0;
      vl_PMV.DestinationAlternat  = 0;
      vl_NumCmd    = vl_Cmd;
        
       vl_PMV.Affichage1 = XDC_PMV_TEXTE;
       vl_PMV.Affichage2 = XDC_PMV_TEXTE;
       vl_PMV.Affichage3 = XDC_PMV_TEXTE;

       XZST_03EcritureTrace(XZSTC_WARNING,"tache relais: %d\n",vl_PMV.Numero );  
           
      /** appel a XZEP01 **/
      if (XZEP01_Commande_PMV(vl_PMV,
	                      vl_NumCmd,
			      vl_nomMachine)!=XDC_OK)
			      
      { XZST_03EcritureTrace(XZSTC_WARNING,"tache relais: erreur dans XZEP01\n");
	break; }
	
      break;



    /*** appel de la fonction XZEP04 ***/
    case (ITRE_AX_XZEP04):
    
      /** lecture des arguments **/
      if (sscanf(pa_arg,"%d|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%d|%d|%d|%d|%d|%[^|]",&vl_Numero,
                                                                                      vl_PMVA.Texte1,
                                                                                      vl_PMVA.Texte2,
                                                                                      vl_PMVA.Texte3,
                                                                                      vl_PMVA.Texte4,
                                                                                      vl_PMVA.Texte5,
                                                                                      vl_PMVA.Alternat1,
                                                                                      vl_PMVA.Alternat2,
                                                                                      vl_PMVA.Alternat3,
                                                                                      vl_PMVA.Alternat4,
                                                                                      vl_PMVA.Alternat5,
                                                                                      &vl_Clign1,
                                                                                      &vl_Clign2,
                                                                                      &vl_Clign3,
                                                                                      &vl_Clign4,
                                                                                      &vl_Clign5,
                                                                                      &vl_flash,
                                                                                      &vl_Cmd,
                                                                                      vl_nomMachine)!=19)
      				
      { XZST_03EcritureTrace(XZSTC_WARNING,"%s est non chaine",pa_arg);
        break; }
        
      vl_PMVA.Numero = vl_Numero;
      vl_PMVA.Clign1 = vl_Clign1;
      vl_PMVA.Clign2 = vl_Clign2;
      vl_PMVA.Clign3 = vl_Clign3;
      vl_PMVA.Clign4 = vl_Clign4;
      vl_PMVA.Clign5 = vl_Clign5;
      vl_PMVA.Flash  = vl_flash;
      vl_PMVA.Dest1_proche  = 0;
      vl_PMVA.Dest1_loin = 0;
      vl_PMVA.Dest2_proche = 0;
      vl_PMVA.Dest2_loin = 0;
      vl_NumCmd    = vl_Cmd;
        
       vl_PMVA.Affichage1 = XDC_PMVA_TEXTE;
       vl_PMVA.Affichage2 = XDC_PMVA_TEXTE;
       vl_PMVA.Affichage3 = XDC_PMVA_TEXTE;
       vl_PMVA.Affichage4 = XDC_PMVA_TEXTE;
       vl_PMVA.Affichage5 = XDC_PMVA_TEXTE;

       XZST_03EcritureTrace(XZSTC_WARNING,"tache relais: %d\n",vl_PMVA.Numero );  
           
      //appel a XZEP04
      if (XZEP04_Commande_PMVA(vl_PMVA,
	                      vl_NumCmd,
			      vl_nomMachine)!=XDC_OK)
			      
      { XZST_03EcritureTrace(XZSTC_WARNING,"tache relais: erreur dans XZEP04\n");
	break; }
	
      break;


    /*** appel de la fonction XZEP104 ***/
    case (ITRE_AX_XZEP104):
    
      /** lecture des arguments **/
      if (sscanf(pa_arg,"%d|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%d|%d|%d|%d|%d|%[^|]",&vl_Numero,
                                                                                      vl_saga.Texte1,
                                                                                      vl_saga.Texte2,
                                                                                      vl_saga.Texte3,
                                                                                      vl_saga.Texte4,
                                                                                      vl_saga.Texte5,
                                                                                      vl_saga.Alternat1,
                                                                                      vl_saga.Alternat2,
                                                                                      vl_saga.Alternat3,
                                                                                      vl_saga.Alternat4,
                                                                                      vl_saga.Alternat5,
										      &vl_Clign1,
										      &vl_Clign2,
										      vl_saga.Picto,
										      vl_saga.Bandeau,
										      &vl_Clign3,
										      &vl_Clign4,
                                                                                      &vl_Cmd,
                                                                                      vl_nomMachine)!=19)
      				
      { XZST_03EcritureTrace(XZSTC_WARNING,"%s est non chaine",pa_arg);
        break; }
        
      vl_saga.Numero = vl_Numero;
      vl_saga.Luminosite = vl_Clign2;
      vl_saga.Barriere = vl_Clign3;
      vl_saga.Scenario = vl_Clign4;
      vl_NumCmd    = vl_Cmd;
        
       vl_saga.Affichage1 = XDC_PMVA_TEXTE;
       vl_saga.Affichage2 = XDC_PMVA_TEXTE;
       vl_saga.Affichage3 = XDC_PMVA_TEXTE;
       vl_saga.Affichage4 = XDC_PMVA_TEXTE;
       vl_saga.Affichage5 = XDC_PMVA_TEXTE;

       XZST_03EcritureTrace(XZSTC_WARNING,"tache relais: %d\n",vl_saga.Numero );  
           
      //appel a XZEP104
      if (XZEP104_Commande_Eqt_SAGA(vl_saga,
	                      vl_NumCmd,
			      vl_nomMachine)!=XDC_OK)
			      
      { XZST_03EcritureTrace(XZSTC_WARNING,"tache relais: erreur dans XZEP104\n");
	break; }
	
      break;


    /*** appel de la fonction XZEP01 en mode arrÁt PMV ***/
    case (ITRE_AX_XZEP01_ARRET):
    
      /** lecture des arguments **/
      if (sscanf(pa_arg,"%d|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%d|%d|%d|%d|%d|%[^|]",&vl_Numero,
                                                                                      vl_PMV.Texte1,
                                                                                      vl_PMV.Texte2,
                                                                                      vl_PMV.Texte3,
                                                                                      vl_PMV.Alternat1,
                                                                                      vl_PMV.Alternat2,
                                                                                      vl_PMV.Alternat3,
                                                                                      &vl_Clign1,
                                                                                      &vl_Clign2,
                                                                                      &vl_Clign3,
                                                                                      &vl_flash,
                                                                                      &vl_Cmd,
                                                                                      vl_nomMachine)!=13)
      				
      { XZST_03EcritureTrace(XZSTC_WARNING,"%s est non chaine",pa_arg);
        break; }
      
      vl_PMV.Numero = vl_Numero;      
      vl_PMV.Clign1 = vl_Clign1;
      vl_PMV.Clign2 = vl_Clign2;
      vl_PMV.Clign3 = vl_Clign3;
      vl_PMV.Flash  = vl_flash;
      vl_NumCmd    = vl_Cmd;
      
       vl_PMV.Affichage1 = XDC_PMV_DEFAUT;
       vl_PMV.Affichage2 = XDC_PMV_DEFAUT;
       vl_PMV.Affichage3 = XDC_PMV_DEFAUT;

      /** appel a XZEP01 **/
      if (XZEP01_Commande_PMV(vl_PMV,
	                      vl_NumCmd,
			      vl_nomMachine)!=XDC_OK)
			      
      { XZST_03EcritureTrace(XZSTC_WARNING,"tache relais: erreur dans XZEP01_arret\n");
	break; }
	
      break;



    /*** appel de la fonction XZEP04 en mode arrÁt PMVA ***/
    case (ITRE_AX_XZEP04_ARRET):
    
      /** lecture des arguments **/
      if (sscanf(pa_arg,"%d|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%d|%d|%d|%d|%d|%[^|]",&vl_Numero,
                                                                                      vl_PMVA.Texte1,
                                                                                      vl_PMVA.Texte2,
                                                                                      vl_PMVA.Texte3,
                                                                                      vl_PMVA.Texte4,
                                                                                      vl_PMVA.Texte5,
                                                                                      vl_PMVA.Alternat1,
                                                                                      vl_PMVA.Alternat2,
                                                                                      vl_PMVA.Alternat3,
                                                                                      vl_PMVA.Alternat4,
                                                                                      vl_PMVA.Alternat5,
                                                                                      &vl_Clign1,
                                                                                      &vl_Clign2,
                                                                                      &vl_Clign3,
                                                                                      &vl_Clign4,
                                                                                      &vl_Clign5,
                                                                                      &vl_flash,
                                                                                      &vl_Cmd,
                                                                                      vl_nomMachine)!=19)
      				
      { XZST_03EcritureTrace(XZSTC_WARNING,"%s est non chaine",pa_arg);
        break; }
      
      vl_PMVA.Numero = vl_Numero;      
      vl_PMVA.Clign1 = vl_Clign1;
      vl_PMVA.Clign2 = vl_Clign2;
      vl_PMVA.Clign3 = vl_Clign3;
      vl_PMVA.Clign4 = vl_Clign4;
      vl_PMVA.Clign5 = vl_Clign5;
      vl_PMVA.Flash  = vl_flash;
      vl_NumCmd    = vl_Cmd;
      
       vl_PMVA.Affichage1 = XDC_PMVA_DEFAUT;
       vl_PMVA.Affichage2 = XDC_PMVA_DEFAUT;
       vl_PMVA.Affichage3 = XDC_PMVA_DEFAUT;
       vl_PMVA.Affichage4 = XDC_PMVA_DEFAUT;
       vl_PMVA.Affichage5 = XDC_PMVA_DEFAUT;

      // appel a XZEP04
      if (XZEP04_Commande_PMVA(vl_PMVA,
	                      vl_NumCmd,
			      vl_nomMachine)!=XDC_OK)
			      
      { XZST_03EcritureTrace(XZSTC_WARNING,"tache relais: erreur dans XZEP04_arret\n");
	break; }
	
      break;


      /*** appel de la fonction XZEP06 ***/
      case (ITRE_AX_XZEP06):

        /** lecture des arguments **/
        if (sscanf(pa_arg,"%d|%[^|]|%[^|]|%d",&vl_Numero,
        										vl_PRV.Bandeau,
        										vl_PRV.Vitesse,
        										vl_PRV.NoCMD,
        										vl_nomMachine)!=5)

        { XZST_03EcritureTrace(XZSTC_WARNING,"%s est non chaine",pa_arg);
          break; }

        vl_PRV.Numero = vl_Numero;
        vl_NumCmd    = vl_PRV.NoCMD;

         XZST_03EcritureTrace(XZSTC_WARNING,"tache relais: %d\n",vl_PMV.Numero );

        /** appel a XZEP06 **/
        if (XZEP06_Commande_PRV(vl_PRV,
  	                      vl_NumCmd,
  			      vl_nomMachine)!=XDC_OK)

        { XZST_03EcritureTrace(XZSTC_WARNING,"tache relais: erreur dans XZEP06\n");
  	break; }

        break;

      /*** appel de la fonction XZEP07 ***/
      case (ITRE_AX_XZEP07):

        /** lecture des arguments **/
        if (sscanf(pa_arg,"%d|%d|%d",&vl_Numero,
        				&vl_Clign1,
					&vl_NumCmd,
        				vl_nomMachine)!=4)

        { XZST_03EcritureTrace(XZSTC_WARNING,"%s est non chaine",pa_arg);
          break; }

        vl_CFE.Numero = vl_Numero;
	vl_CFE.NoPlanDeFeux = vl_Clign1;

         XZST_03EcritureTrace(XZSTC_WARNING,"tache relais: %d\n",vl_PMV.Numero );

        /** appel a XZEP06 **/
        if (XZEP07_Commande_CFE(vl_CFE,
  	                      vl_NumCmd,
  			      vl_nomMachine)!=XDC_OK)

        { XZST_03EcritureTrace(XZSTC_WARNING,"tache relais: erreur dans XZEP07\n");
  	break; }

        break;


      /*** appel de la fonction XZEP08 ***/
      case (ITRE_AX_XZEP08):

	/** lecture des arguments **/
	if (sscanf(pa_arg,"%d|%s|%d|%d|%d|%d|%s",&vl_Numero,
					vl_IMU.ChaineIMU,
					&vl_IMU.Intervalle,
					&vl_IMU.Cycles,
					&vl_IMU.Duree,
					&vl_NumCmd,
					vl_nomMachine)!=7)

	{ XZST_03EcritureTrace(XZSTC_WARNING,"%s est non chaine",pa_arg);
	  break; }

	vl_IMU.Numero = vl_Numero;

	XZST_03EcritureTrace(XZSTC_WARNING,"tache relais: %d\n",vl_IMU.Numero );

	/** appel a XZEP08 **/
	if (XZEP08_Commande_IMU(vl_IMU,
	  			vl_NumCmd,
				vl_nomMachine)!=XDC_OK)

	{ XZST_03EcritureTrace(XZSTC_WARNING,"tache relais: erreur dans XZEP08\n");
	  break; }

	break;


      /*** appel de la fonction XZEP09 ***/
      case (ITRE_AX_XZEP09):

	/** lecture des arguments **/
	if (sscanf(pa_arg,"%d|%s|%d|%d|%d|%s",&vl_Numero,
					vl_SONO.Message,
					&vl_SONO.Nb_PAUErreur,
					&vl_SONO.Nb_PAUPilotes,
					&vl_NumCmd,
					vl_nomMachine)!=6)

	{ XZST_03EcritureTrace(XZSTC_WARNING,"%s est non chaine",pa_arg);
	  break; }

	XZST_03EcritureTrace(XZSTC_WARNING,"tache relais: %d\n",vl_Numero );

	/** appel a XZEP09 **/
	if (XZEP09_Commande_RAU_SONO(vl_SONO,
				     vl_NumCmd,
				     vl_nomMachine)!=XDC_OK)

	{ XZST_03EcritureTrace(XZSTC_WARNING,"tache relais: erreur dans XZEP09\n");
	  break; }

	break;




        /*** appel de la fonction XZEP06 en mode arret PRV ***/
        case (ITRE_AX_XZEP06_ARRET):

		/** lecture des arguments **/
		if (sscanf(pa_arg,"%d|%[^|]|%[^|]|%d",&vl_Numero,
					vl_PRV.Bandeau,
					vl_PRV.Vitesse,
					vl_PRV.NoCMD,
					vl_nomMachine)!=5)
			{
			XZST_03EcritureTrace(XZSTC_WARNING,"%s est non chaine",pa_arg);
			break;
			}

			vl_PRV.Numero = vl_Numero;
			vl_NumCmd    = vl_PRV.NoCMD;

			 XZST_03EcritureTrace(XZSTC_WARNING,"tache relais: %d\n",vl_PMV.Numero );

			/** appel a XZEP06 **/
			if (XZEP06_Commande_PRV(vl_PRV,
						  vl_NumCmd,
						  vl_nomMachine)!=XDC_OK)

			{
				XZST_03EcritureTrace(XZSTC_WARNING,"tache relais: erreur dans XZEP06_arret\n");
				break;
			}

          break;

     /*** appel de la fonction XZEP02 ***/
    case (ITRE_AX_XZEP02):
    
      /** lecture des arguments **/
      if (sscanf(pa_arg,"%d|%[^|]|%[^|]|%d|%d|%[^|]",&vl_Numero,
                                                     vl_Picto.Texte,
                                                     vl_Picto.Picto,
                                                     &vl_Clign1,
                                                     &vl_NumCmd,
                                                     vl_nomMachine)!=6)
                                                           				
      { XZST_03EcritureTrace(XZSTC_WARNING,"%s est non chaine",pa_arg);
        break; }
        
      vl_Picto.Numero = vl_Numero;
      vl_Picto.Clign = vl_Clign1;
        
      /** appel a XZEP02 **/
      if (XZEP02_Commande_Picto(vl_Picto,
	                        vl_NumCmd,
			        vl_nomMachine)!=XDC_OK)
			      
      { XZST_03EcritureTrace(XZSTC_WARNING,"tache relais: erreur dans XZEP02\n");
	break; }
	
      break;

     case (ITRE_AX_TDP_INIT_SURCHARGE):
      TipcSrvMsgWrite(XDG_ETDP_SURCHARGE,
			    TipcMtLookupByNum(XDM_ETDP_INIT_SURCHARGE),
			    FALSE,
			    T_IPC_FT_STR, vl_dg_mce,
			    NULL);
      TipcSrvFlush();
	XZST_03EcritureTrace(XZSTC_FONCTION,"init surcharge TDP : envoi msg XDM_ETDP_INIT_SURCHARGE %s sur %s",
					vl_dg_mce, XDG_ETDP_SURCHARGE);
	break;
     	

     case (ITRE_AX_TDP_SURCHARGE):
      if (sscanf(pa_arg,"%d|%d|%d|%d",&vl_Numero,&vl_no,&vl_numero,&vl_cle) !=4) 
      { XZST_03EcritureTrace(XZSTC_WARNING,"%s est non chaine",pa_arg);
        break; }
      TipcSrvMsgWrite(XDG_ETDP_SURCHARGE,
			    TipcMtLookupByNum(XDM_ETDP_SURCHARGE),
			    FALSE,
			    T_IPC_FT_INT2, vl_Numero,
			    T_IPC_FT_INT4, vl_no,
			    T_IPC_FT_INT4, vl_numero,
			    T_IPC_FT_INT2, vl_cle,
			    NULL);
      TipcSrvFlush();
	XZST_03EcritureTrace(XZSTC_FONCTION,"surcharge TDP : envoi msg XDM_ETDP_SURCHARGE %d sur %s",
					vl_no, XDG_ETDP_SURCHARGE);
	break;
     	
     /*** appel de la fonction XZEZ01 ***/
    case (ITRE_AX_XZEZ01):
      if (sscanf(pa_arg,"%d|%hd|%hd|%hd|%hd",&vl_Numero,
                                     &vl_Tdp.Dest,
                                     &vl_Tdp.DestAlt,
				     &vl_Tdp.DestNoeud,
                                     &vl_Tdp.Site)!=5)
                                                           				
      { XZST_03EcritureTrace(XZSTC_WARNING,"%s est non chaine",pa_arg);
        break; }
        
      vl_Tdp.Numero = vl_Numero;
        
      /** appel a XZEZ01 **/
      if (XZEZ01_Dde_Valeur_TDP(vl_dg_mce,
			        itre_ax_Dde_Valeur_TDP,
			        vl_Tdp,
			        &vl_cr)!=XDC_OK)
			      
      { XZST_03EcritureTrace(XZSTC_WARNING,"tache relais: erreur dans XZEZ01\n");
	break; }
	
      break;

      /*******************	PMVA	*****************************************/
      // appel de la fonction XZEZ04

     case (ITRE_AX_XZEZ04):
       // lecture des arguments
       XZST_03EcritureTrace(XZSTC_WARNING,"XZEZ04 %s",pa_arg);
       if (sscanf(pa_arg,"%d|%hd|%hd|%hd|%hd|%hd",&vl_Numero,
                                      &vl_Tdp_PMVA.Dest1_proche,
                                      &vl_Tdp_PMVA.Dest1_loin,
                                      &vl_Tdp_PMVA.Dest2_proche,
                                      &vl_Tdp_PMVA.Dest2_loin,
                                      &vl_Tdp_PMVA.Site)!=6)

       { XZST_03EcritureTrace(XZSTC_WARNING,"%s est non chaine",pa_arg);
         break; }

       vl_Tdp_PMVA.Numero = vl_Numero;

       // appel a XZEZ04
       if (XZEZ04_Dde_Valeur_TDP_PMVA(vl_dg_mce,
    		   itre_ax_Dde_Valeur_TDP_PMVA,
 			        vl_Tdp_PMVA,
 			        &vl_cr)!=XDC_OK)

       { XZST_03EcritureTrace(XZSTC_WARNING,"tache relais: erreur dans XZEZ04\n");
 	break; }
       break;
       /************************************************************/

      XDY_Entier vl_idComm;


       /*** appel de la fonction XZCA02 ou XZCA03 ( J.P.L 13/01/95 ) ***/
    case (ITRE_AX_XZCA02_03):
    
      /** lecture des arguments **/
      if (sscanf (pa_arg, "%c|%ld|%ld", &vl_Mode_Sauvegarde,
					&vl_Mois,
					&vl_Annee) != 3)
      { XZST_03EcritureTrace(XZSTC_WARNING,
		"Liste d'arguments invalide pour XZCA02 ou XZCA03 : \"%s\"\n", pa_arg);
        break; }

      /** appel a XZCA02 ou XZCA03 **/
      if (vl_Mode_Sauvegarde == 'S'  ||  vl_Mode_Sauvegarde == 's')
         vl_cr = xzca02_Sauvegarder_Archive (vl_Mois, vl_Annee);
      else vl_cr = xzca03_Restaurer_Archive (vl_Mois, vl_Annee);

      if (vl_cr != XDC_OK)
      { XZST_03EcritureTrace(XZSTC_WARNING,
				"Tache relais: erreur dans XZCA02 ou XZCA03\n");
	break; }
	
      break;
 
 



   /*** appel a la fonction XZDD02 ***/
    case (ITRE_AX_XZDD02):
	XZST_03EcritureTrace(XZSTC_WARNING,"%s",pa_arg);
      /*lecture des arguments*/
      if (sscanf(pa_arg,"%[^|]|%d|%[^|]|%d|%d|%d|%d|%d",vl_nom,
			                              &vl_numMachine,
			                              vl_nomMachine,
			                              &(vl_numEvt.numero),
			                              &vl_cle,
			                              &vl_typeEvt,
			                              &vl_typePA,
			                              &vl_mode)!=8)
      {
        XZST_03EcritureTrace(XZSTC_WARNING,"%s est non numerique",pa_arg);
        break;
      }

      /*conversion de l'horodate en nombre de secondes*/
      if (XZSM_13ConversionHorodateSec(vl_nom,&vl_horodate)!=XDC_OK)
      {
	XZST_03EcritureTrace(XZSTC_WARNING,"Erreur dans la conversion de l'horodate pour le plan d'actions");
	break;

      }

      /*execution de la commande*/
      vl_numEvt.cle=vl_cle;
      if (XZDD02_CalculerPA(vl_horodate,
                            vl_numMachine,
                            vl_nomMachine,
			    vl_numEvt,
			    vl_typeEvt,
			    vl_typePA,
			    vl_mode)!=XDC_OK)
      {
        XZST_03EcritureTrace(XZSTC_WARNING,"tache relais:Erreur dans XZDD02");
      }
      else 
      {
	XZST_03EcritureTrace(XZSTC_WARNING,"tache relais:appel a XZDD02 OK!");
      }
      break;



    /*commande LEAVEDG*/
    case (ITRE_AX_LEAVEDG):
      /*lecture des arguments*/
      if (sscanf(pa_arg,"%s",vl_dg)!=1)
      {
        XZST_03EcritureTrace(XZSTC_WARNING,"%s est non chaine",pa_arg);
        break;
      }
  
      /*execution de la commande*/
      XDG_QuitterDG(vl_dg);
      TipcSrvFlush();
      break;



    /*demande d'impression d'un fichier*/
    case (ITRE_AX_PRINT):
      /*lecture des arguments*/
      if (sscanf(pa_arg,"%[^|]|%[^|]|%d",vl_nom,vl_path,&vl_format)!=3)
      {
        XZST_03EcritureTrace(XZSTC_WARNING,"%s est non numerique",pa_arg);
        break;
      }
    
      /*A
      * demande d'impression*/
      vl_typePeriph = XZSSC_TYPE_IMPRIMA;
      if ((vl_cr=XZSS_04ImprimerFichier(vl_typePeriph,vl_nom,vl_path,vl_format))
		!=XDC_OK)
        /* Affichage Trace */
        XZST_03EcritureTrace(XZSTC_WARNING
	    ,"impression fichier %s: erreur %d"
	    ,vl_nom ,vl_cr) ;
      break;



    /*demande d'ouverture d'une fiche main courante sur l'ecran mce*/
    case (ITRE_AX_OUVREFMC):
      /*lecture des arguments*/
      if (sscanf(pa_arg, "%[^|]|%d|%[^|]|%d|%d|%d",
	      vl_nom, &vl_typeEvt, vl_date, &vl_numero,&vl_cle,&vl_mode) != 6)
      {
	    XZST_03EcritureTrace(XZSTC_WARNING,"%s : arguments invalides",pa_arg);
	    break;
      }

      /*formattage des arguments */
      sprintf(vl_arg,"%s|%d|%s|%d|%d|%d",vl_nom,vl_typeEvt,vl_date,vl_numero,vl_cle,vl_mode);

      /*envoi du message a l'autre tache relais (celle de la main courante)*/
      TipcSrvMsgWrite(vl_dg_mce,
			TipcMtLookupByNum(XDM_IAX_NEWTASK),
			FALSE,
			T_IPC_FT_INT4,3,
			T_IPC_FT_STR,"ITMA_TMC_Fiche_Main_Courante",
			T_IPC_FT_STR,vl_arg,
			T_IPC_FT_INT4,0,
			NULL);
      TipcSrvFlush();
      break;



    /*demande d'affichage de la fenetre de login par la tache relais*/
    /*cote synoptique*/
    case (ITRE_AX_LOGIN):
      /*execution de la commande*/
      itre_rtw_lanceDial(vg_dg_dial,pa_arg);
      break;



    /*lancement de la main courante*/
    case (ITRE_AX_LANCEMCE):
      /*lecture des arguments*/
      if (sscanf(pa_arg, "%[^|]|%d|%d|%d|",
		   vl_nom, &vl_numPoste, &vl_numOper, &vl_numProfil) != 4)
      {
        XZST_03EcritureTrace(XZSTC_WARNING,"%s : arguments invalides",pa_arg);
        break;
      }

      /*j'appelle la macro de lancement du menu*/
      itre_rtw_lanceMce(vl_dg_mce, pa_arg);
      break;



    /*** Abonnement/desabonnement messages administrateurs (J.P.L 04/04/95) ***/
    case (ITRE_AX_MSG_ADM):
      /*lecture des arguments*/
      if (sscanf(pa_arg, "%d", &vl_mode) != 1)
      {
        XZST_03EcritureTrace(XZSTC_WARNING,"%s : arguments invalides",pa_arg);
        break;
      }

      /* joindre ou quitter les datagroups d'avertissement administrateur
       * pour la machine et le site locaux */
	if (vl_mode == XDC_OUI )
      {
		XDG_JoindreDG(vg_dg_admin_site);
		XDG_JoindreDG(vg_dg_admin_poste);
      }
		else
      {
		XDG_QuitterDG(vg_dg_admin_site);
		XDG_QuitterDG(vg_dg_admin_poste);
      }
      TipcSrvFlush();
      break;



    /*** Trace ( J.P.L 07/04/95) ***/
    case (ITRE_AX_TRACE):
      /*lecture des arguments*/
      if (sscanf(pa_arg, "%d|", &vl_mode) != 1)
      {
        XZST_03EcritureTrace(XZSTC_WARNING,"%s : arguments invalides", pa_arg);
        break;
      }

      /* Tracer le message avec le niveau requis */
      pl_arg = strchr (pa_arg, '|') + 1;
      XZST_03EcritureTrace (vl_mode, pl_arg);
      break;


    /*appel de la fonction diffusant un son sur les HP*/
    case (ITRE_AX_SONALERTE):
      /*lecture des arguments*/
      if (sscanf(pa_arg, "%d", &vl_no) != 1)
      {
        XZST_03EcritureTrace(XZSTC_WARNING,"%s : arguments invalides",pa_arg);
        break;
      }
      
      if ( itr_LanceSon( vl_no ) != XDC_OK )
      {
        XZST_03EcritureTrace(XZSTC_WARNING,"Probleme pour l'envoi de sons ");
        break;
      }
      break;


    /* Execution de la arret de tisyn */
    case (ITRE_AX_ARRET_SYNOP):
      if (sscanf(pa_arg,"%[^|]|%[^|]",vl_nomMachine, vl_nomTache)!=2)
      {
        XZST_03EcritureTrace(XZSTC_WARNING,"%s : arguments invalides",pa_arg);
        break;
      }

      vl_cr = XZSP_03CmdTache( vl_nomMachine, vl_nomTache, XZSPC_ARRET ) ;
      if (vl_cr != XDC_OK)
      {
        XZST_03EcritureTrace(XZSTC_WARNING,"cr : %d vl_nomMachine : %s vl_nomTache : %s, Demande : %d",vl_cr, vl_nomMachine, vl_nomTache, XZSPC_ARRET);
        break;
      }
    break;


    /* Execution de la relance de tisyn */
    case (ITRE_AX_RELANCER_SYNOP):
      if (sscanf(pa_arg,"%[^|]|%[^|]",vl_nomMachine, vl_nomTache)!=2)
      {
        XZST_03EcritureTrace(XZSTC_WARNING,"%s : arguments invalides",pa_arg);
        break;
      }

      vl_cr = XZSP_03CmdTache( vl_nomMachine, vl_nomTache, XZSPC_MARCHE ) ;
      if (vl_cr != XDC_OK)
      {
        XZST_03EcritureTrace(XZSTC_WARNING,"cr : %d vl_nomMachine : %s vl_nomTache : %s, Demande : %d",vl_cr, vl_nomMachine, vl_nomTache, XZSPC_MARCHE);
        break;
      }
    break;



    /*demande de fin de poste a envoyer au PC simplifie*/
    case (ITRE_AX_FIN_POSTE):
      if (sscanf(pa_arg,"%d|%s",&vl_numero,vl_nom)!=2)
      {
	XZST_03EcritureTrace(XZSTC_WARNING,"%s : arguments invalides",pa_arg);
	break;
      }
      /*envoi du message de demande de fin de poste au PC simplifie*/
      XDG_EncodeDG2(vl_dg,XDG_IAX_FINPOSTE,vl_nom);
      TipcSrvMsgWrite(vl_dg,
			    TipcMtLookupByNum(XDM_IAX_FINPOSTE),
			    FALSE,
			    NULL);
      TipcSrvFlush();

      /*envoi du message qui dit que le PC simplifie n'a plus d'operateur*/
	  if (strcmp(va_nomMachine,""))
		TipcSrvMsgWrite(XDG_A_PRES_OP,
				TipcMtLookupByNum(XDM_A_PRES_OP),
				FALSE,
				T_IPC_FT_INT2,vl_numero,
				T_IPC_FT_INT2,XDC_FAUX,
				T_IPC_FT_STR,va_nomMachine,
				NULL);
	  else
		TipcSrvMsgWrite(XDG_A_PRES_OP,
				TipcMtLookupByNum(XDM_A_PRES_OP),
				FALSE,
				T_IPC_FT_INT2,vl_numero,
				T_IPC_FT_INT2,XDC_FAUX,
				T_IPC_FT_STR,vg_nomMachine,
				NULL);

      TipcSrvFlush();
      break;
      


    /*demande d'acquittement d'un evt au poste voisin */
    case (ITRE_AX_ACQT):
      if (sscanf(pa_arg,"%[^|]|%d|%d|%d",vl_NomMachine,&vl_position,&vl_numero,&vl_cle )!=4)
      {
			XZST_03EcritureTrace(XZSTC_WARNING,"%s : arguments invalides",pa_arg);
			break;
      }
      /*envoi du message de demande de fin de poste au PC simplifie*/
      XDG_EncodeDG2(vl_dg,XDG_IAX_ACQT,vl_NomMachine);
      TipcSrvMsgWrite(vl_dg,
			    TipcMtLookupByNum(XDM_IAX_ACQT),
			    FALSE,
			    T_IPC_FT_INT4,(T_INT4)vl_position,
			    T_IPC_FT_INT4,(T_INT4)vl_numero,
			    T_IPC_FT_INT2,(T_INT2)vl_cle,
			    NULL);
      TipcSrvFlush();

	  /* En version classique, titre poste le message sur IAX_ACQT_site a destination de tous les autres postes abonnes a ce topic */
	  /* En version Magistra, titre est le producteur du message mais aussi le consommateur : le callback ne declenche pas         */
	  /* il faut donc appeler explicitement le callback defini sur reception du message XDM_IAX_ACQT  en le customisant un peu    */
	  if (vg_titre)
	  {
		if (itre_rtw_processACQT_titre( vl_position, vl_numero, vl_cle, vl_NomMachine) != XDC_OK)
			XZST_03EcritureTrace(XZSTC_WARNING,"ITRE_AX_ACQT : pb appel itre_rtw_processACQT_titre");
	  }

      break;


    /*demande d'abonnement aux etats des PC simplifies*/
    case (ITRE_AX_ETAT_SIMPLIFIE):
      if (sscanf(pa_arg,"%d",&vl_mode)!=1) {
	XZST_03EcritureTrace(XZSTC_WARNING,"%s : arguments invalides",pa_arg);
	break;
      }
      
      /*appel a XZSE_Abt_Etat_Comm_Site */
      if (XZSE_Abt_Etat_Comm_Site(vl_mode,itre_ax_Etat_Comm) != XDC_OK) {
	XZST_03EcritureTrace(XZSTC_WARNING,"ITRE_AX_ETAT_SIMPLIFIE: pb dans l'appel a XZSE_Abt_Etat_Comm_Site");
	break;
      }
   
      /* appel a XZAE_103Abt_Presence_Op_PCsimpl*/ /* DEM1055+DEM1079 */
      if ((!strcmp(vg_nomSite, XDC_NOM_SITE_CI)) || (!strcmp(vg_nomSite, XDC_NOM_SITE_DP)) || (!strcmp(vg_nomSite, XDC_NOM_SITE_VC))){
      	if (XZAE103_Abt_Presence_Op_PCsimpl(vl_mode,itre_ax_Presence_PCS) != XDC_OK) {
		XZST_03EcritureTrace(XZSTC_WARNING,"ITRE_AX_ETAT_SIMPLIFIE: pb dans l'appel a XZAE103_Abt_Presence_Op_PCsimpl");
		break;
        }
      }

      XZST_03EcritureTrace(XZSTC_FONCTION,"itre_ax: demande abonnement etats PC simplifies: %d",vl_mode);
      break;



    /*envoi du message qui dit que l'operateur est present sur le PC simplifie*/
    case (ITRE_AX_PRESENCE_PCS):
      if (sscanf(pa_arg,"%d|%d",&vl_numero,&vl_mode)!=2) {
	XZST_03EcritureTrace(XZSTC_WARNING,"%s : arguments invalides",pa_arg);
	break;
      }

      vg_operateurPresent=TRUE;

      /*envoi du message qui dit que je suis la sur le PC simplifie*/
	  if (strcmp(va_nomMachine,""))
		TipcSrvMsgWrite(XDG_A_PRES_OP,
				TipcMtLookupByNum(XDM_A_PRES_OP),
				FALSE,
				T_IPC_FT_INT2,vl_numero,
				T_IPC_FT_INT2,XDC_VRAI,
				T_IPC_FT_STR,va_nomMachine,
				NULL);
	  else
		TipcSrvMsgWrite(XDG_A_PRES_OP,
				TipcMtLookupByNum(XDM_A_PRES_OP),
				FALSE,
				T_IPC_FT_INT2,vl_numero,
				T_IPC_FT_INT2,XDC_VRAI,
				T_IPC_FT_STR,vg_nomMachine,
				NULL);
	  TipcSrvFlush();
      break;



    /*demande d'init de la presence d'un operateur*/
    case (ITRE_AX_INITPRES):
      TipcSrvMsgWrite(   XDG_IAX_INITPRES
		      ,XDM_IdentMsg(XDM_IAX_INITPRES )
		      ,XDM_FLG_SRVMSGWRITE
		      ,NULL);
      TipcSrvFlush();
      break;



    /*demande du catalog*/
    case (ITRE_AX_DEMCAT):
	if (!TipcSrvMsgWrite(   XDG_AG_IMPCAT
                        ,XDM_IdentMsg(XDM_AG_IMPCAT ) 
                        ,XDM_FLG_SRVMSGWRITE
			,NULL) )
      	XZST_03EcritureTrace(XZSTC_WARNING,"Impossible de demander le catalog");
      TipcSrvFlush();
      break;



    /*demande d'export du catalog*/
    case (ITRE_AX_EXPCATF):
	if (!TipcSrvMsgWrite(   XDG_AG_EXPCATF
                        ,XDM_IdentMsg(XDM_AG_EXPCATF ) 
                        ,XDM_FLG_SRVMSGWRITE
			,NULL) )
      	XZST_03EcritureTrace(XZSTC_WARNING,"Impossible de demander l'export du catalog");
      TipcSrvFlush();
      break;



    /*demande d'export du catalog*/
    case (ITRE_AX_EXPCATC):
	if (!TipcSrvMsgWrite(   XDG_AG_EXPCATC
                        ,XDM_IdentMsg(XDM_AG_EXPCATC ) 
                        ,XDM_FLG_SRVMSGWRITE
			,NULL) )
      	XZST_03EcritureTrace(XZSTC_WARNING,"Impossible de demander l'export du catalog");
      TipcSrvFlush();
      break;



    /*appel a XZSE10*/
    case (ITRE_AX_XZST10):
      if (sscanf(pa_arg,"%s",vl_nom)!=1) {
	XZST_03EcritureTrace(XZSTC_WARNING,"%s : arguments invalides",pa_arg);
	break;
      }

      if (XZST_10ArchiverMsgSyst(XDL_GES64_ACHEV,vl_nom,NULL) != XDC_OK)
	XZST_03EcritureTrace(XZSTC_WARNING,"ITRE_AX_XZST10: pb dans l'appel a XZST_10ArchiverMsgSyst");
      break;



    /*appel a XZIS25_MASQUE*/
    case (ITRE_AX_FCT_MSQ):
      if (sscanf(pa_arg,"%d",&vl_typeEqt)!=1) {
        XZST_03EcritureTrace(XZSTC_WARNING,"%s : arguments invalides",pa_arg);
        break;
      }

      if (XZIS25_MASQUE(vl_typeEqt) != XDC_OK)
        XZST_03EcritureTrace(XZSTC_WARNING,"ITRE_AX_FCT_MSQ: pb dans l'appel a XZIS25_MASQUE");
      break;


        case (ITRE_AX_XZEG01):
                XZST_03EcritureTrace(XZSTC_WARNING,"processEXEC : XZEG01");
                if (sscanf(pa_arg,"%d|%d|%d|%d|%d|%s",&vl_numero,&vl_cle,&vl_numEqt,
                                &vl_motif,&vl_mode,vl_nomSite)!= 6) {
                  XZST_03EcritureTrace(XZSTC_WARNING,"%s : arguments invalides",pa_arg);
                  break;
                }
                XZST_03EcritureTrace(XZSTC_WARNING,"%d %d %d %d %d %s",
                                        vl_numero,vl_cle,vl_numEqt,
                                        vl_motif,vl_mode,vl_nomSite);
                if (XZEG01_informer_regulation(vl_numero, vl_cle, vl_numEqt, vl_motif,vl_mode,vl_nomSite) != XDC_OK)
                  XZST_03EcritureTrace(XZSTC_WARNING,"ITRE_AX_XZEG01: pb dans l appel a XZEG01_informer_regulation");
                break;

    /*appel a XZIS26_VUE*/
    case (ITRE_AX_FCT_VUE):
      if (XZIS26_VUE(pa_arg) != XDC_OK)
          XZST_03EcritureTrace(XZSTC_WARNING,"ITRE_AX_FCT_VUE: pb dans l'appel a XZIS26_VUE");
      break;



      /*appel a XZIS27_TDP_MZ*/
      case (ITRE_AX_FCT_TDP_MZ):
          XZST_03EcritureTrace(XZSTC_WARNING,"processEXEC : TDP_MZ");
        if (XZIS27_Graphe(XZISC_GRAPHE_TDP_MZ,pa_arg) != XDC_OK)
                XZST_03EcritureTrace(XZSTC_WARNING,"ITRE_AX_FCT_TDP_MZ: pb dans l'appel a XZIS27_TDP_MZ");
        break;



      case (ITRE_AX_FCT_F2T):
          XZST_03EcritureTrace(XZSTC_WARNING,"processEXEC : F2T");
          if (XZIS27_Graphe(XZISC_GRAPHE_F2T,pa_arg) != XDC_OK)
                XZST_03EcritureTrace(XZSTC_WARNING,"ITRE_AX_FCT_F2T: pb dans l'appel a XZIS27_TDP_MZ");
          break;



          case (ITRE_AX_FCT_F2T_REF):
                  XZST_03EcritureTrace(XZSTC_WARNING,"processEXEC : F2T_REF");
                  if (XZIS27_Graphe(XZISC_GRAPHE_F2T_REF,pa_arg) != XDC_OK)
                        XZST_03EcritureTrace(XZSTC_WARNING,"ITRE_AX_FCT_F2T_REF: pb dans l'appel a XZIS27_TDP_MZ");
                  break;



          case (ITRE_AX_FCT_INRETS):
                  XZST_03EcritureTrace(XZSTC_WARNING,"processEXEC : TDPINRETS");
                  if (XZIS27_Graphe(XZISC_GRAPHE_INRETS,pa_arg) != XDC_OK)
                        XZST_03EcritureTrace(XZSTC_WARNING,"ITRE_AX_FCT_INRETS: pb dans l'appel a XZIS27_TDP_MZ");
                  break;

         case (ITRE_AX_PLANSECOURS):
                  XZST_03EcritureTrace(XZSTC_WARNING,"processEXEC : PLANSECOURS");
                  if (sscanf(pa_arg,"%d",&vl_numPS)!=1) {
                        XZST_03EcritureTrace(XZSTC_WARNING,"%s : arguments invalides",pa_arg);
                        break;
              }

                  if (XZIS28_PlanSecours(vl_numPS) != XDC_OK)
                        XZST_03EcritureTrace(XZSTC_WARNING,"ITRE_AX_PLANSECOURS: pb dans l'appel a XZIS28_PLANSECOURS");
                  break;

         case (ITRE_AX_XZEL10):
		 XZST_03EcritureTrace(XZSTC_WARNING,"processEXEC : XZEL10");
				 if (sscanf(pa_arg,"%d",&vl_idComm)!=1) {
					   XZST_03EcritureTrace(XZSTC_WARNING,"%s : arguments invalides", pa_arg);
					   break;
				 }
                 if (XZEL10_Acquitter_Communautaire(vl_idComm) != XDC_OK)
		       XZST_03EcritureTrace(XZSTC_WARNING,"ITRE_AX_XZEL10: pb dans l'appel a XZEL10_Acquitter_Communautaire");
		XZST_03EcritureTrace(XZSTC_WARNING,"processEXEC : apres  XZEL10");
		break;

	case (ITRE_AX_XZEL11):
		XZST_03EcritureTrace(XZSTC_WARNING,"processEXEC : XZEL11");
			if (sscanf(pa_arg,"%d|%d|%d|%s",&vl_idComm, &vl_numero, &vl_cle, vl_type)!=4) {
				XZST_03EcritureTrace(XZSTC_WARNING,"%s : arguments invalides",pa_arg);
				break;
			}
			vl_numEvt.numero = vl_numero;
			vl_numEvt.cle = vl_cle;
		if (XZEL11_Rapprocher_Communautaire(vl_idComm, vl_numEvt,vl_type) != XDC_OK)
			XZST_03EcritureTrace(XZSTC_WARNING,"ITRE_AX_XZEL10: pb dans l'appel a XZEL11_Rapprocher_Communautaire");
		break;

case (ITRE_AX_XZEL12):
                 XZST_03EcritureTrace(XZSTC_WARNING,"processEXEC : XZEL12");
				 if (sscanf(pa_arg,"%d|%d",&vl_idComm, &vl_motif)!=2) {
					   XZST_03EcritureTrace(XZSTC_WARNING,"%s : arguments invalides",pa_arg);
					   break;
				 }
				 XZST_03EcritureTrace(XZSTC_WARNING,"XZEL12 %d %d", vl_idComm,vl_motif);
                 if (XZEL12_Ignorer_Communautaire(vl_idComm, vl_motif) != XDC_OK)
                       XZST_03EcritureTrace(XZSTC_WARNING,"ITRE_AX_XZEL10: pb dans l'appel a XZEL12_Ignorer_Communautaire");
                 break;

	case (ITRE_AX_TIGMOT_MODIF_BASE):
		XZST_03EcritureTrace(XZSTC_WARNING,"processEXEC : ITRE_AX_TIGMOT_MODIF_BASE %s",pa_arg);
		if (sscanf(pa_arg,"%d",&vl_motif)!=2) {
			XZST_03EcritureTrace(XZSTC_WARNING,"%s : arguments invalides",pa_arg);
			break;
                }
		if (xzcg07_EnvoieNomMachineLocal ()!= XDC_OK)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"PB pour recevoir les messages de TCGCD. \n");
		}
		if(xzcg_ChgmtBaseCour(XDC_BASE_CFG) != XDC_OK)
		{
			XZST_03EcritureTrace( XZSTC_WARNING, "gmot.x : xzcg_ChgmtBaseCour () a echoue");
		}
		if (xzcg04_Verification_Coherence(vl_motif) != XDC_OK)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"PB Modification ");
		}
		break;

	case (ITRE_AX_TIGMOT_VERIF_COHERENCE  ):
		XZST_03EcritureTrace(XZSTC_WARNING,"processEXEC : ITRE_AX_TIGMOT_VERIF_COHERENCE %s",pa_arg);
		if (sscanf(pa_arg,"%d",&vl_motif)!=1) {
			XZST_03EcritureTrace(XZSTC_WARNING,"%s : arguments invalides",pa_arg);
			break;
		}
		if (xzcg07_EnvoieNomMachineLocal ()!= XDC_OK)
		 {
			 XZST_03EcritureTrace(XZSTC_WARNING,"PB pour recevoir les messages de TCGCD. \n");
		 }
		 if (vl_motif>=50) {
		   vl_motif = vl_motif / 10;
		   if(xzcg_ChgmtBaseCour(XDC_BASE_CFG)!= XDC_OK)
		   {
			    XZST_03EcritureTrace( XZSTC_WARNING, "TCGCD : xzcg_ChgmtBaseCour () a echoue");
		     }
		}
		else {
		 if(xzcg_ChgmtBaseCour(XDC_BASE_CFT) != XDC_OK)
		 {
			 XZST_03EcritureTrace( XZSTC_WARNING, "TCGCD : xzcg_ChgmtBaseCour () a echoue");
		 }
		}
		 if (xzcg04_Verification_Coherence (vl_motif)!= XDC_OK)
		{
		 XZST_03EcritureTrace(XZSTC_WARNING,"PB pour envoyer la demande de verification ");
		 }

		break;

	case (ITRE_AX_TIGMOT_DUPLIQUE_CFG):
		XZST_03EcritureTrace(XZSTC_WARNING,"processEXEC : ITRE_AX_TIGMOT_DUPLIQUE_CFG %s",pa_arg);
		if (xzcg07_EnvoieNomMachineLocal ()!= XDC_OK)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"PB pour recevoir les messages de TCGCD. \n");
		}
		if (xzcg06_DuplicationBD(XZCGC_RESTAURATION_DANS_CFT) != XDC_OK)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"PB Restauration avant Verif \n");
		}
		break;

	case (ITRE_AX_TIGMOT_SAUVE_CFG):
		XZST_03EcritureTrace(XZSTC_WARNING,"processEXEC : ITRE_AX_TIGMOT_SAUVE_CFG %s",pa_arg);
		if (xzcg07_EnvoieNomMachineLocal ()!= XDC_OK)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"PB pour recevoir les messages de TCGCD. \n");
		}
		if (xzcg06_DuplicationBD(XZCGC_SAUVEGARDE_CFG) != XDC_OK)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"PB Sauvegarde avnt Verif \n");
	        }
		break;

	case (ITRE_AX_XZEL14):
		XZST_03EcritureTrace(XZSTC_WARNING,"processEXEC : XZEL14 %s",pa_arg);

		if (sscanf(pa_arg, "|%d|%[^|]",
					&vl_motif, vl_path) != 2) {
			XZST_03EcritureTrace(XZSTC_WARNING,"%s : arguments invalides",pa_arg);
			break;
		}
		XZEL14_Acquitter_Alerte_Scenario_SAGA(vl_path,vl_motif);
		break;

	case (ITRE_AX_XZEL13):
		XZST_03EcritureTrace(XZSTC_WARNING,"processEXEC : XZEL13 %s",pa_arg);
		if (sscanf(pa_arg,"%[^|]|%d|%d|%d|%[^|]", vl_path,&vl_idComm, &vl_motif,&vl_numero,vl_nom) !=5) {
			XZST_03EcritureTrace(XZSTC_WARNING,"%s : arguments invalides",pa_arg);
			break;
		}
		XZST_03EcritureTrace(XZSTC_WARNING,"XZEL13 %s %s %d %d",vl_path,vl_nom, vl_idComm, vl_numero, vl_motif);
		if (XZEL13_Acquitter_Alerte_Regulation(vl_path,vl_nom,vl_idComm,vl_motif, vl_numero) != XDC_OK)
			XZST_03EcritureTrace(XZSTC_WARNING,"ITRE_AX_XZEL13 : pb dans l'appel a  XZEL13_Acquitter_Alerte_Regulation");
		break;

	case (ITRE_AX_XZIS50):
		XZST_03EcritureTrace(XZSTC_WARNING,"processEXEC :ITRE_AX_XZIS50 %s",pa_arg);
		if (sscanf(pa_arg,"%[^|]|%[^|]|%d|%d|%[^|]",
						vl_path,
						vl_nom,
						&vl_idComm,
						&vl_numero,
						vl_nomTache) != 5)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"%s : arguments invalides",pa_arg);
			break;
		}
		if (XZIS50_Demande_RMVI(vg_nomSite, vl_path, vl_nom, vl_idComm, vl_numero, vl_nomTache)!=XDC_OK) {
			XZST_03EcritureTrace(XZSTC_WARNING,"ITRE_AX_XZIS50: pb appel a XZIS50_Demande_RMVI");
		}
		break;

	case (ITRE_AX_XZIS53):
		XZST_03EcritureTrace(XZSTC_WARNING,"processEXEC :ITRE_AX_XZIS53 %s",pa_arg);
		if (sscanf(pa_arg,"%[^|]|%[^|]|%d|%d",
				vl_nom, vl_nomTache,
				&vl_idComm, &vl_numero) != 4)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"%s : arguments invalides",pa_arg);
			break;
		}
		if (XZIS53_Connexion_AVA(vg_nomSite, vl_nom, vl_nomTache, vl_idComm, vl_numero)!=XDC_OK) {
			XZST_03EcritureTrace(XZSTC_WARNING,"ITRE_AX_XZIS50: pb appel a XZIS53_Connexion_AVA");
		}
		break;

	case (ITRE_AX_GABEB):
		XZST_03EcritureTrace(XZSTC_WARNING,"processEXEC : XZEL15 %s",pa_arg);
		if (sscanf(pa_arg,"%[^|]|%d|%d|%d|%[^|]", vl_path,&vl_idComm, &vl_motif,&vl_numero,vl_nom) !=5) {
			XZST_03EcritureTrace(XZSTC_WARNING,"%s : arguments invalides",pa_arg);
			break;
		}
		XZST_03EcritureTrace(XZSTC_WARNING,"XZEL15 %s %s %d %d",vl_path,vl_nom, vl_idComm, vl_numero, vl_motif);
		if (XZEL15_Acquitter_Alerte_GABEB(vl_path,vl_nom,vl_idComm,vl_motif, vl_numero) != XDC_OK)
			XZST_03EcritureTrace(XZSTC_WARNING,"ITRE_AX_GABEB : pb dans l'appel a  XZEL15_Acquitter_Alerte_GABEB");
		break;

	case (ITRE_AX_XZAV03):

			XZST_03EcritureTrace(XZSTC_WARNING,"processEXEC : XZAV03");
			if (sscanf(pa_arg,"%[^|]|%[^|]|%d|%[^|]",&vl_horodateD,&vl_horodateF,&vl_PosteOperateur,&vl_nomPosteOperateur)!= 4)
			{
			  XZST_03EcritureTrace(XZSTC_WARNING,"%s : arguments invalides",pa_arg);
			  break;
			}
			XZST_03EcritureTrace(XZSTC_WARNING,"+-+-+-+-+-+-+ date debut simu : %s",vl_horodateD);
			XZST_03EcritureTrace(XZSTC_WARNING,"+-+-+-+-+-+-+ date fin simu : %s",vl_horodateF);
			XZSM_13ConversionHorodateSec(vl_horodateD,&vl_dateDeb);
			XZSM_13ConversionHorodateSec(vl_horodateF,&vl_dateFin);

			XZSM_11ConversionHorodate(vl_dateDeb , &vl_Horodate);
			XZST_03EcritureTrace(XZSTC_WARNING,"*-*-*-*-*-*-* date debut simu : %s",vl_Horodate);

			XZSM_11ConversionHorodate(vl_dateFin , &vl_Horodate);
			XZST_03EcritureTrace(XZSTC_WARNING,"*-*-*-*-*-*-* date fin simu : %s",vl_Horodate);

			if (XZAV03_Demarrer_Simulation(vl_dateDeb,vl_dateFin,vl_PosteOperateur,vl_nomPosteOperateur) != XDC_OK)
			  XZST_03EcritureTrace(XZSTC_WARNING,"ITRE_AX_XZAV03: pb dans l appel a XZAV03_demarrer_simulation");
			break;

    /* Demande d'activation / desactivation du timer TdP */
    case (ITRE_AX_TIMER_TDP):
      if (sscanf(pa_arg, "%d", &vl_mode) != 1)
      {
        XZST_03EcritureTrace(XZSTC_WARNING,"Exec ITRE_AX_TIMER_TDP: %s : arguments invalides",pa_arg);
        break;
      }
      itre_Timer_TdP_activer (vl_mode);
      break;

	case (ITRE_AX_AVA_RETOUR_ALERTE):
		if (sscanf(pa_arg, "%d %d %d %d %d %d", &vl_ava_id_mission, &vl_ava_num_evt_cause, &vl_ava_cle_cause, &vl_ava_creation_avec_lien, &vl_ava_alerte_numero, &vl_ava_alerte_sit) != 6)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"Exec ITRE_AX_AVA_RETOUR_ALERTE: %s : arguments invalides",pa_arg);
			break;
		}
		if ( !TipcSrvMsgWrite ( XDG_API_GW_AVA,
						XDM_IdentMsg(XDM_GTW_AVA_RETOUR_ALERTE),
						XDM_FLG_SRVMSGWRITE,
						T_IPC_FT_INT4,vl_ava_id_mission,
						T_IPC_FT_INT4,vl_ava_num_evt_cause,
						T_IPC_FT_INT4,vl_ava_cle_cause,
						T_IPC_FT_INT4,vl_ava_creation_avec_lien,
						T_IPC_FT_INT4,vl_ava_alerte_numero,
						T_IPC_FT_INT4,vl_ava_alerte_sit,
					NULL))
            XZST_03EcritureTrace(XZSTC_FONCTION, "Exec ITRE_AX_AVA_RETOUR_ALERTE: erreur envoie message sur le DG %d MSG:%d",XDG_API_GW_AVA, XDM_GTW_AVA_RETOUR_ALERTE);
	break;

	case (ITRE_AX_XZED03):
		if (sscanf(pa_arg, "%d %d %s %d", &vl_NumEqt, &vl_NumVoie, &vl_cmd, &vl_Duree) != 4)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"Exec ITRE_AX_XZED03: %s : arguments invalides", pa_arg);
			break;
		}
		if (XZED03_Demande_Enregistrement_DAI(vl_NumEqt, vl_NumVoie, vl_cmd, vl_Duree)!=XDC_OK) {
			XZST_03EcritureTrace(XZSTC_WARNING,"ITRE_AX_XZED03: pb appel a XZIS50_Demande_RMVI");
		}
		break;

  }
  XZST_03EcritureTrace(XZSTC_FONCTION, "OUT -- %s", version);
  
  
  return (XDC_OK);
}




/* fichier : @(#)itre_ax.c	1.75   Release : 1.75   Date : %D	*
*************************************************
* STERIA * Projet MIGRAZUR			*
*************************************************
* MODULE  : MTRE				*
*************************************************
* DESCRIPTION :
* traite les messages de type SEND envoyes par Applix
*
*************************************************
* SEQUENCE D'APPEL :
*/

extern int itre_ax_processSend(
int va_msgType,
char *va_dg,
char *pa_arg
)

/*
* PARAMETRE EN ENTREE :
*
* arg1		Description
* arg2		..........
*
* PARAMETRE EN SORTIE :
*
* VALEUR DE RETOUR :
*
* CONDITION D'ERREUR :
*
* CONDITION LIMITES :
************************************************/
{
  T_IPC_MSG vl_msg;
  char vl_arg1[100],vl_arg2[100];
  int vl_numCmd;
  int vl_num;
  int vl_numPoke;

  static char *version = "$Id: itre_ax.c,v 1.153 2021/06/03 07:21:36 pc2dpdy Exp $ : itre_ax_processSend" ;
  
  switch(va_msgType)
  {
    /*A
    * envoyer un message type join datagroup*/
    case (XDM_IAX_JOINDG):
      if (sscanf(pa_arg,"%d!%[^!]",&vl_numCmd,vl_arg1)!=2)
      {
        XZST_03EcritureTrace(XZSTC_WARNING,"Args de JOINDG invalides: %s",pa_arg);
        break;
      }
      TipcSrvMsgWrite(va_dg,
		  TipcMtLookupByNum(va_msgType),
		  FALSE,
		  T_IPC_FT_INT4,vl_numCmd,
		  T_IPC_FT_STR,vl_arg1,
		  NULL);
      break;

    /*A
    * envoyer un message send poke*/
    case (XDM_IAX_SENDPOKE):
      if (sscanf(pa_arg,"%d!%d!%[^!]",&vl_numCmd,&vl_numPoke,vl_arg1)!=3)
      {
        XZST_03EcritureTrace(XZSTC_WARNING,"Args de SENDPOKE invalides: %s",pa_arg);
        break;
      }
      TipcSrvMsgWrite(va_dg,
		  TipcMtLookupByNum(va_msgType),
		  FALSE,
		  T_IPC_FT_INT4,vl_numCmd,
		  T_IPC_FT_INT4,vl_numPoke,
		  T_IPC_FT_STR,vl_arg1,
		  NULL);
      break;

    /*A
    * envoyer un message newtask*/
    case (XDM_IAX_NEWTASK):
      if (sscanf(pa_arg,"%d!%d!%[^!]!%[^!]",&vl_numCmd,&vl_num,vl_arg1,vl_arg2)!=4)
      {
        XZST_03EcritureTrace(XZSTC_WARNING,"Args de NEWTASK invalides: %s",pa_arg);
        break;
      }
      TipcSrvMsgWrite(va_dg,
		  TipcMtLookupByNum(va_msgType),
		  FALSE,
		  T_IPC_FT_INT4,vl_numCmd,
		  T_IPC_FT_STR,vl_arg1,
		  T_IPC_FT_STR,vl_arg2,
		  T_IPC_FT_INT4,vl_num,
		  NULL);
      break;

    /*A
    * envoyer un message de type setvarctx*/
    case (XDM_IAX_SETVARCTX):
      if (sscanf(pa_arg,"%d!%[^!]!%[^!]",&vl_numCmd,vl_arg1,vl_arg2)!=3)
      {
        XZST_03EcritureTrace(XZSTC_WARNING,"Args de SETVARCTX invalides: %s",pa_arg);
        break;
      }
      TipcSrvMsgWrite(va_dg,
		  TipcMtLookupByNum(va_msgType),
		  FALSE,
		  T_IPC_FT_INT4,vl_numCmd,
		  T_IPC_FT_STR,vl_arg1,
		  T_IPC_FT_STR,vl_arg2,
		  NULL);
      break;
  }

  TipcSrvFlush();
 return (XDC_OK);
}




/*************************************************
* MODULE  : MTRE				*
*************************************************
* DESCRIPTION :
*  fonction utilisateur passee dans XZSE_Abt_Etat_Comm_Site
*
*************************************************
* SEQUENCE D'APPEL :
*/

static void itre_ax_Etat_Comm(
		XZSET_EComm_Site	va_comm, char *va_nomMachine
)

/*
* PARAMETRE EN ENTREE :
* va_comm : etat de la communication avec un site
*
* PARAMETRE EN SORTIE :
*
* VALEUR DE RETOUR :
*
* CONDITION D'ERREUR :
*
* CONDITION LIMITES :
************************************************/
{
  char vl_str[5];
  char *tl_params[3];
  char vl_var[25];

  XZST_03EcritureTrace(XZSTC_FONCTION,"IN: fonction itre_ax_Etat_Comm");

  /*trace de debug*/
  XZST_03EcritureTrace(XZSTC_DEBUG1,"itre_ax_Etat_Comm: site %s, etat %d",va_comm.Site,va_comm.Etat);
  sprintf(vl_str,"%d",va_comm.Etat);

  /*si je suis sur un PC simplifie*/
  if (vg_type_machine==XZSAC_TYPEM_PCS) {
    /*je regarde l'etat de la comm avec le CI*/
    if (!strcmp(va_comm.Site,XDC_NOM_SITE_CI)) {
      /*envoi de l'etat de la comm avec le CI a applix*/
      strcpy(vl_var,"vg_comm_CI");
      tl_params[0]=vl_var;
      tl_params[1]=vl_str;
      if (itre_com_SendPoke(vg_socEnvToAxMce,COM_CANAL_MTMT_ETAT_PCSIMPL,tl_params,2,va_nomMachine)!=XDC_OK)
	return; 
    }
  }
  /*sinon si je suis au CI ou DP ou VC ou CA*/
  else if ( (!strcmp(vg_nomSite,XDC_NOM_SITE_CI)) || 
  			(!strcmp(vg_nomSite, XDC_NOM_SITE_DP)) || 
			(!strcmp(vg_nomSite, XDC_NOM_SITE_VC)) || 
			(!strcmp(vg_nomSite, XDC_NOM_SITE_CA)))  /* VH DEM1055+DEM1079 */{
    /*je regarde l'etat de la comm avec les PC simplifies*/
    sprintf(vl_var,"vg_comm_%s",va_comm.Site);
    tl_params[0]=vl_var;
    tl_params[1]=vl_str;
    
    if (itre_com_SendPoke(vg_socEnvToAxMce,COM_CANAL_MTMT_ETAT_PCSIMPL,tl_params,2,va_nomMachine)!=XDC_OK)
      return;
    if (itre_com_SendPoke(vg_socEnvToAxDial,COM_CANAL_MTMT_ETAT_PCSIMPL,tl_params,2,va_nomMachine)!=XDC_OK)
      return;
  }
  else {
    /*sur les districts regarder uniquement l'etat CI*/
    if (!strcmp(va_comm.Site,XDC_NOM_SITE_CI)) {
      /*envoi de l'etat de la comm avec le CI a applix*/
      strcpy(vl_var,"vg_comm_CI");
      tl_params[0]=vl_var;
      tl_params[1]=vl_str;
      if (itre_com_SendPoke(vg_socEnvToAxMce,COM_CANAL_MTMT_ETAT_PCSIMPL,tl_params,2,va_nomMachine)!=XDC_OK)
	return;
      if (itre_com_SendPoke(vg_socEnvToAxDial,COM_CANAL_MTMT_ETAT_PCSIMPL,tl_params,2,va_nomMachine)!=XDC_OK)
        return;
    }
  }

  XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: fonction itre_ax_Etat_Comm");
  return;
}



/*************************************************
* MODULE  : MTRE                                *
*************************************************
* DESCRIPTION :
* fonction utilisateur passe a
* XZAE103_Abt_Presence_Op_PCsimpl
*************************************************
* SEQUENCE D'APPEL :
*/

static void itre_ax_Presence_PCS(
XZAET_EPresence_Op_PCsimpl      va_presence
)

/*
* PARAMETRE EN ENTREE :
*
* PARAMETRE EN SORTIE :
*
* VALEUR DE RETOUR :
*
* CONDITION D'ERREUR :
*
* CONDITION LIMITES :
************************************************/
{
  char  vl_variable[30];
  char  vl_valeur[5];
  char  *tl_params[2];

  XZST_03EcritureTrace(XZSTC_FONCTION,"IN: itre_ax_presence_PCS");

  /*trace*/
  XZST_03EcritureTrace(XZSTC_FONCTION,"itre_ax_presence_PCS: presence %d, site %s",
				    va_presence.Presence,
				    va_presence.NomSite);

  /*fabrique le nom de la variable a envoyer a Applix*/
  sprintf(vl_variable,"vg_operateur_%s",va_presence.NomSite);
  sprintf(vl_valeur,"%d",va_presence.Presence);

  tl_params[0]=vl_variable;
  tl_params[1]=vl_valeur;

  /*envoi a applix*/
  if (itre_com_SendPoke(vg_socEnvToAxMce,COM_CANAL_MTMT_ETAT_PCSIMPL,tl_params,2,"") != XDC_OK)
    XZST_03EcritureTrace(XZSTC_WARNING,"itre_ax_presence_PCS: pb dans l'appel a itre_com_SendPoke");
  if (itre_com_SendPoke(vg_socEnvToAxDial,COM_CANAL_MTMT_ETAT_PCSIMPL,tl_params,2,"") != XDC_OK)
    XZST_03EcritureTrace(XZSTC_WARNING,"itre_ax_presence_PCS: pb dans l'appel a itre_com_SendPoke");

  XZST_03EcritureTrace(XZSTC_FONCTION,"OUT itre_ax_presence_PCS");

}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Fonction appeler lors d'un chgt d'etat d'un eqt SAGA
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static void itre_ax_Abt_Etat_Eqt_SAGA(XZEAT_Etat_Eqt_SAGA va_etat,int *pa_resultat, char *va_nomMachine)

/*
* ARGUMENTS EN ENTREE :
*   < Valeur et descriptif des parametres en entree >
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR : 
*   <Valeur de retour et un commentaire explicatif >
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   < Bref descriptif fonctionnel >
*
------------------------------------------------------*/
{
  char vl_cmd[250] = "";
  char *tl_params[30];

  static char *version = "$Id: itre_ax.c,v 1.153 2021/06/03 07:21:36 pc2dpdy Exp $ : itre_ax_Abt_Etat_SAGA" ;
  
     if (va_etat.Numero==0)
       return ;
     sprintf(vl_cmd,"%d,%s,%d,%s,%s,%d,%s,%s,%d,%s,%s,%d,%s,%s,%d,%s,%d,%d,%s,%s,%d,%d",va_etat.Numero,       
	                                                     va_etat.Texte1,	   
	                                                     va_etat.Affichage1,   
	                                                     va_etat.Alternat1,	   
	                                                     va_etat.Texte2,       
	                                                     va_etat.Affichage2,   
	                                                     va_etat.Alternat2,	   
	                                                     va_etat.Texte3,	   
	                                                     va_etat.Affichage3,   
	                                                     va_etat.Alternat3,	   
	                                                     va_etat.Texte4,	   
	                                                     va_etat.Affichage4,   
	                                                     va_etat.Alternat4,	   
	                                                     va_etat.Texte5,	   
	                                                     va_etat.Affichage5,   
	                                                     va_etat.Alternat5,	   
							     va_etat.Clignotement,
							     va_etat.Luminosite,
							     va_etat.Picto,
							     va_etat.Bandeau,
							     va_etat.Barriere,
							     va_etat.Scenario);

  /*envoi du poke a Applix*/
  tl_params[0]=vl_cmd; 
  
  if (itre_com_SendPoke(vg_socEnvToAxMce,COM_CANAL_MTSAGA_ETAT,tl_params,1,va_nomMachine)!=XDC_OK)
  {
     XZST_03EcritureTrace(XZSTC_WARNING,"itre_ax_Abt_Etat_Eqt_SAGA: erreur sur le send poke COM_CANAL_MTSAGA_ETAT");
     *pa_resultat=XDC_NOK;
  }
  else *pa_resultat=XDC_OK;

  XZST_03EcritureTrace(XZSTC_FONCTION,"OUT -- %s", version);
}	


