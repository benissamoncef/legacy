/*E*/
/* Fichier : $Id: esrv_cfg.c,v 1.40 2021/05/04 13:25:03 pc2dpdy Exp $        Release : $Revision: 1.40 $        Date : $Date: 2021/05/04 13:25:03 $
-----------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TESRV * FICHIER esrv_cfg.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Module d'initialisation et de configuration du sous-systeme
*   EQUEXT pour la tache TESRV, en mode nominal.
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	23 Aug 1994	: Creation
* Mismer.D	version 1.2	28 Oct 1994	:
* Mismer.D	version 1.3	29 Nov 1994	:
* Mismer.D	version 1.4	09 Dec 1994	:
* Mismer.D	version 1.5	25 Jan 1995	:
* Mismer.D	version 1.6	26 Jan 1995	:
* Mismer.D	version 1.7	26 Jan 1995	:
* Mismer.D	version 1.8	09 Fev 1995	:
* Mismer.D	version 1.9	08 Mar 1995	:
* Mismer.D	version 1.10	07 Avr 1995	:
* Mismer.D	version 1.11	14 Avr 1995	:
* Mismer.D	version 1.12	14 Avr 1995	:
* Mismer.D	version 1.13	24 Avr 1995	:
* Fontaine.C	version 1.14	22 Mai 1995	: Reprise entete, historique
* Mismer.D	version 1.15	06 Jun 1995	: Modif parametrage serveur de terminaux
* Mismer.D	version 1.16	01 Sep 1995	: Lancement possible sans parametrage serveur de terminaux
* Mismer.D	version 1.17	20 Sep 1995	: Ajout arret de tetdc si terdt present
* Mismer.D  	version 1.19	19 Oct 1995	: Modif dialogue avec tesur pendant le lancement des taches
* Mismer.D  	version 1.20	07 Dec 1995	: Ajout parametre time-out console transerver
* Mismer.D  	version 1.21	06 Fev 1996	: Modif pour renvoie etat serveur de terminaux
* Mismer.D  	version 1.22	08 Fev 1996	: Correction modif pour renvoie etat serveur de terminaux
* Mismer.D  	version 1.23	14 Mar 1996	: Correction modif pour renvoie etat serveur de terminaux suite (DEM/781)
* Mismer.D  	version 1.24	10 Mai 1996	: Correction parametre config transerver idletimer=2 (DEM/1111)
* Mismer.D      version 1.25    10 Sep 1996     : Ajout type machine pour inhibe le lancement de tetdc (RADT_W)
* Mismer.D      version 1.26    30 Mai 1997     : Mise en place serveur de terminaux XYPLEX (DEM/1431)
* JBL	Nov 2007 : DEM 664
* JBL	Mars 2009 : ajout 5e tenx
* ADB   Fevrier 2010 : surveillance tenx
* ABE	Juin-2010 : correction FT 3493: Pas d'arret - relance de tenx5 sur reconfiguration du SAE
* JPL	24/02/11 : Migration architecture HP ia64 (DEM 975) : suppression warnings mineurs  1.34
* ABE	27-04-2015 : projet suppression ancienne GTC (GTCA)
* JMG	29/02/16 : ST 40 ports
* JMG	24/02/18 : tache IP CTRL_FEUX 1.37 DEM1284
* JMG	17/03/20 : EMS
* GR    10/07/2020 : EMS 1.39
* LCL		20/12/21	: MOVIS P2 ems SAE-312
---------------------------------------------------------------------------------------------------------------------------*/

/* fichiers inclus */

#include "xzsa.h"

#include "ex_mdon.h"
#include "ex_mrtf.h"

#include "esrv_ctl.h"
#include "esrv_art.h"
#include "esrv_cfg.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "@(#)esrv_cfg.c	1.28 05/31/00      :  esrv_cfg";

/* Config standard d'un serveur de terminaux */
static char vg_ConfigST[][96] = {
    "def po 0 inac logout ena\r",
    "def server console logout ena inactivity timer 2\r",
    "def po 1-%d access remote telnet transmit buff 512 typeahead size 1024 def sess mod trans\r",
    "def po 1-%d loss notification disable internet connection disable line editor disable\r",
    "def po 1-%d input flow control disable output flow control disable Message codes disable\r",
    "def po 1-%d autoprompt disable verification disable uli disable autobaud disable\r",
    "def po 1-%d char size 7 parity even stop bit 1 speed 9600 Flow Control disable\r",
    "def po %d telnet transmit immediat\r",
    "logout po 1-%d\r",
    ""};

#define C_SCRIPT_40 "/produits/migrazur/appliSD/exec/ConfigSTTelnet.sh"

static XZEXT_ENG_CONF_LIGNE vg_TableLigne[XZECC_MAX_EQT];
static ESRV_EQUIPEMENT vg_TableEqt[XZECC_MAX_EQT];
static char vm_ListeST[16][16] = {"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""};
static int vm_EtatST[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
static int vm_Type_Machine = 0;
extern int vg_ConfigTerm;
/* declaration de fonctions internes */

/* definition de fonctions externes */
extern void ed_arreter_tache_eqpt();

extern void ed_dialogue_tesur();

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Initialisation du sous-systeme ou une reconfiguration
*  des �quipements.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void ec_init_cb(
    T_IPC_CONN va_Cnx,
    T_IPC_CONN_PROCESS_CB_DATA pa_Data,
    T_CB_ARG pa_ARG)

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
*  Fonction appel�e lors de la r�ception du message
*  XDM_EFIN_CONFIG

* FONCTION
*
*
------------------------------------------------------*/
{
   static char *version = "@(#)esrv_cfg.c	1.28 05/31/00      :  ec_init_cb ";
#ifdef _TIBCO_EMS
   XDY_Entier vl_TypeEqupt; /* type d'equipement (un ou tous) */
   XDY_Entier vl_Status;    /* status d'init ou de conf */
#else
   T_INT4 vl_TypeEqupt; /* type d'equipement (un ou tous) */
   T_INT4 vl_Status;    /* status d'init ou de conf */
#endif
   XDY_NomMachine pl_NomMachine;
   XDY_NomSite pl_NomSite;
   XDY_Datagroup pl_NomDG;
   int vl_Fd;
   char pl_PathEtFicEqt[XDC_PATH_ABS_SIZE],
       pl_PathEtFicLig[XDC_PATH_ABS_SIZE];
   int vl_NbLignes;
   double vl_Horodate;
   int vl_jourSemaine;
   XZEXT_MSG_SOCKET pl_MsgTimer;

   XZST_03EcritureTrace(XZSTC_FONCTION, "FONCTION: entree ec_init_cb");

   /*A Recuperation du status d'execution du message XDM_EFIN_CONFIG */
   TipcMsgRead(pa_Data->msg,
               T_IPC_FT_INT4, &vl_TypeEqupt,
               T_IPC_FT_INT4, &vl_Status,
               NULL);

   XZST_03EcritureTrace(XZSTC_DEBUG1, "type:%d status:%d", vl_TypeEqupt, vl_Status);

   /*A Recuperation du site de la machine */
   if (XZSC_04NomSite(pl_NomSite) != XDC_OK)
   {
      /* Ecriture Trace */
      XZST_03EcritureTrace(XZSTC_WARNING, "Recuperation du Nom du site impossible . ");
   }

   /*A Recuperation du nom de la machine */
   if (XZSC_07NomMachine(pl_NomMachine) != XDC_OK)
   {
      /* Ecriture Trace */
      XZST_03EcritureTrace(XZSTC_WARNING, "Recuperation du Nom de la machine impossible . ");
   }

   /*A Test si les fichier de config ligne et presence equipement sont presents */
   sprintf(pl_PathEtFicEqt, "%s/%s_%s", XZEXC_PATH_CONFIG1, XDF_Presence_Equip, pl_NomMachine);
   sprintf(pl_PathEtFicLig, "%s/%s_%s", XZEXC_PATH_CONFIG1, XDF_Config_Lignes, pl_NomMachine);
   if (access(pl_PathEtFicEqt, F_OK | R_OK) && access(pl_PathEtFicLig, F_OK | R_OK))
   {
      /*A Demande d'initialisation de tous les equipement pour la machine locale a TEINI si les fichiers de config sont absents */
      /*A Emission du message XDM_EINIT sur le DatGroup XDG_EINIT_NomSite */
      sleep(30);
      XDG_EncodeDG2(pl_NomDG, XDG_EINIT, pl_NomSite);
      if (!TipcSrvMsgWrite(pl_NomDG,
                           XDM_IdentMsg(XDM_EINIT),
                           XDM_FLG_SRVMSGWRITE,
                           T_IPC_FT_STR, pl_NomMachine,
                           T_IPC_FT_INT4, XZECC_TYPE_EQT_ALL,
                           T_IPC_FT_INT4, XZECC_MACHINE_PRINC,
                           NULL))
      {
         XZST_03EcritureTrace(XZSTC_WARNING, " Envoi message XDM_EINIT non effectue ");
      }
   }

   /*A Appel de la fonction ec_config du module MCFG pour la configuration des  taches */
   XZST_03EcritureTrace(XZSTC_WARNING, "Debut lancement de l'application. ");
   vg_ConfigTerm = XDC_NOK;
   if (ec_config(vl_TypeEqupt) != XDC_OK)
   {
      /*A Ecriture trace Configuration de l'application non effectuee */
      XZST_03EcritureTrace(XZSTC_WARNING, "Configuration de l'application non effectuee. ");
   }
   else
   {
      /* Memorisation lancement des taches effectuee avec succes */
      XZST_03EcritureTrace(XZSTC_WARNING, "Application lancee avec succes ");
      vg_ConfigTerm = XDC_OK;
   }

   XZST_03EcritureTrace(XZSTC_FONCTION, "FONCTION: sortie ec_init_cb");

   return;
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Cette fonction effectue la configuration de l'application
*  la machine locale.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int ec_config(int va_TypeEquipt)

/*
* ARGUMENTS EN ENTREE :
*   va_TypeEqupt	: type d'�quipement ( un ou tous )
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR :
*   XDC_OK	succes
*   XDC_NOK	echec
*
* CONDITION D'UTILISATION
*
*
* FONCTION
*
*
------------------------------------------------------*/
{
   static char *version = "@(#)esrv_cfg.c	1.28 05/31/00      :  ec_config ";
   int vl_ValRet = XDC_OK;
   XDY_NomMachine pl_NomMachine;
   XDY_NomSite pl_NomSite;
   XDY_NomTache pl_NomTache;
   XDY_NomTache pl_NomTacheSpec = "";
   XDY_Datagroup vl_datagroup;
   XZEXT_MASK *pl_masque;
   char pl_PathEtFicEqt[XDC_PATH_ABS_SIZE],
       pl_PathEtFicLig[XDC_PATH_ABS_SIZE];
   char pl_PrefTache[5];
   int vl_NbEqt = 0,
       vl_NbConfLigne = 0,
       vl_IndexEqt = 0;
   XZEXT_MASK *pl_masqueST = NULL;
   XZEXT_MSG_SOCKET vl_Command;
   char vl_NomST[16];
   int vl_SockRet = 0, vl_NumLigne = 0, vl_NumLigneMax = 0;
   int vl_Index, vl_IndexLig;
   int vl_nbTestST = 0;
   T_BOOL vl_EtatDg = FALSE;

   /*A Recuperation du site de la machine */
   if (XZSC_04NomSite(pl_NomSite) != XDC_OK)
   {
      /* Ecriture Trace */
      XZST_03EcritureTrace(XZSTC_WARNING, "Recuperation du Nom du site impossible . ");
   }

   /*A Recuperation du nom de la machine */
   if (XZSC_07NomMachine(pl_NomMachine) != XDC_OK)
   {
      /* Ecriture Trace */
      XZST_03EcritureTrace(XZSTC_WARNING, "Recuperation du Nom de la machine impossible . ");
   }

   /*A Recuperation du type de la machine */
   if (XZSA_17TypeMachine(pl_NomMachine, &vm_Type_Machine) != XDC_OK)
   {
      XZSA_20InitAdmin();
      if (XZSA_17TypeMachine(pl_NomMachine, &vm_Type_Machine) != XDC_OK)
         XZST_03EcritureTrace(XZSTC_WARNING, "Impossible de trouve le type de la machine");
   }

   /*A Test si les fichier de config ligne et presence equipement sont presents */
   sprintf(pl_PathEtFicEqt, "%s/%s_%s", XZEXC_PATH_CONFIG1, XDF_Presence_Equip, pl_NomMachine);
   sprintf(pl_PathEtFicLig, "%s/%s_%s", XZEXC_PATH_CONFIG1, XDF_Config_Lignes, pl_NomMachine);
   if (access(pl_PathEtFicEqt, F_OK | R_OK) && access(pl_PathEtFicLig, F_OK | R_OK))
   {
      XZST_03EcritureTrace(XZSTC_WARNING, "Fichier de config ligne <%s> ou equipement absents <%s>.", pl_PathEtFicLig, pl_PathEtFicEqt);
      return (XDC_NOK);
   }

   /*A
    * Phase d'initialisation
    */

   /*A
    * Lecture du fichier de configuration XDF_Config_Lignes_<NomMachine>
    */

   if (ex_lire_config_lignes(pl_NomMachine, &vl_NbConfLigne) != XDC_OK)
   {
      XZST_03EcritureTrace(XZSTC_WARNING, "Absence Fichier XDF_Config_Lignes_<NomMachine> . ");
      /*A Si pas de config lignes sortie sans lancement de la configuration */
      return (XDC_NOK);
   }

   /*A Si fichier XDF_Config_Lignes_<NomMachine> est vide */
   /*if ( vl_NbConfLigne == 0 )
   {*/
   /*A Ecriture trace fichier vide */ /*
   XZST_03EcritureTrace(XZSTC_WARNING,"Fichier XDF_Config_Lignes_<NomMachine> vide. " );
   return ( XDC_NOK );
}*/

   /*A
    * Lecture du fichier de configuration XDF_Presence_Equip_<NomMachine>
    */
   if (ex_lire_config_eqt(pl_NomMachine, &vl_NbEqt) != XDC_OK)
   {
      XZST_03EcritureTrace(XZSTC_WARNING, "Absence Fichier XDF_Presence_Equip_<NomMachine> . ");
      /*A Sortie sans lancement de la configuration */
      return (XDC_NOK);
   }

   /*A Si fichier XDF_Presence_Equip_<NomMachine> est vide */
   if (vl_NbEqt == 0)
   {
      /*A Ecriture trace fichier vide */
      XZST_03EcritureTrace(XZSTC_WARNING, "Fichier XDF_Presence_Equip_<NomMachine> vide. ");
      return (XDC_NOK);
   }

   /*A Si configuration  de tous les equipements */
   if (va_TypeEquipt == XZECC_TYPE_EQT_ALL)
   {
      /*A
       * Arreter toutes les taches traitement et protocole
       */
      ed_arreter_tache_eqpt();
      vg_InitTranserver = XDC_VRAI;
      ex_ConnexionSocketRTserver(&vg_SockRT, 0, &vg_SockMask);
      if (vg_ConfigTerm == XDC_OK)
         return (XDC_OK);

      /*
       *A Test si le serveur de terminaux est accessible
       */
      vl_NumLigneMax = 0;
      for (vl_IndexLig = 0; vl_IndexLig < vl_NbConfLigne; vl_IndexLig++)
      {
         if ((strlen(vg_TableLigne[vl_IndexLig].NumeroST) > 0) &&
             ((vl_NumLigne = atoi(vg_TableLigne[vl_IndexLig].NumLigne)) >= 0) &&
             ((vg_TableLigne[vl_IndexLig].Protocole == XZECC_PROT_LCR) ||
              (vg_TableLigne[vl_IndexLig].Protocole == XZECC_PROT_TEL) ||
              (vg_TableLigne[vl_IndexLig].Protocole == XZECC_PROT_OPE)))
         {
            if (vl_NumLigneMax < vl_NumLigne)
            {
               vl_NumLigneMax = vl_NumLigne;
               strcpy(vl_NomST, vg_TableLigne[vl_IndexLig].NumeroST);
            }
         }
      }

      ed_dialogue_tesur();

      if (ex_st40(pl_NomMachine) == XDC_NOK)
      {
         XZST_03EcritureTrace(XZSTC_WARNING, "TESRV : 20 ports");

         while (ex_ouvrir_console_ST(vl_NomST, &vl_SockRet, &pl_masqueST, pl_NomMachine) != XDC_OK)
         {
            sleep(10);
            ed_dialogue_tesur();
            if (vl_nbTestST++ > 2)
            {
               XZST_03EcritureTrace(XZSTC_WARNING, "Lancement de l'appli sans config transerver %s", vl_NomST);
               vg_InitTranserver = XDC_FAUX;
               break;
            }
         }

         /* Si serveur OK envoyer la config standard */
         if (vg_InitTranserver == XDC_VRAI)
         {
            sprintf(vl_Command, vg_ConfigST[0]);
            XZST_03EcritureTrace(XZSTC_WARNING, ":1:%d", ex_envoyer_commande_ST(vl_Command, vl_SockRet, &pl_masqueST));
            ed_dialogue_tesur();
            sprintf(vl_Command, vg_ConfigST[1]);
            XZST_03EcritureTrace(XZSTC_WARNING, "2:%d", ex_envoyer_commande_ST(vl_Command, vl_SockRet, &pl_masqueST));
            sprintf(vl_Command, vg_ConfigST[2], vl_NumLigneMax);
            XZST_03EcritureTrace(XZSTC_WARNING, "3:%d", ex_envoyer_commande_ST(vl_Command, vl_SockRet, &pl_masqueST));
            ed_dialogue_tesur();
            sprintf(vl_Command, vg_ConfigST[3], vl_NumLigneMax);
            XZST_03EcritureTrace(XZSTC_WARNING, "4:%d", ex_envoyer_commande_ST(vl_Command, vl_SockRet, &pl_masqueST));
            sprintf(vl_Command, vg_ConfigST[4], vl_NumLigneMax);
            XZST_03EcritureTrace(XZSTC_WARNING, "5:%d", ex_envoyer_commande_ST(vl_Command, vl_SockRet, &pl_masqueST));
            sprintf(vl_Command, vg_ConfigST[5], vl_NumLigneMax);
            XZST_03EcritureTrace(XZSTC_WARNING, "6:%d", ex_envoyer_commande_ST(vl_Command, vl_SockRet, &pl_masqueST));
            // ed_dialogue_tesur ();
            sprintf(vl_Command, vg_ConfigST[6], vl_NumLigneMax);
            XZST_03EcritureTrace(XZSTC_WARNING, "7:%d", ex_envoyer_commande_ST(vl_Command, vl_SockRet, &pl_masqueST));

            /* Si tache esclave transmit immediat */
            for (vl_IndexLig = 0; vl_IndexLig < vl_NbConfLigne; vl_IndexLig++)
            {
               if ((strlen(vg_TableLigne[vl_IndexLig].NumeroST) > 0) &&
                   ((vl_NumLigne = atoi(vg_TableLigne[vl_IndexLig].NumLigne)) >= 0) &&
                   (vg_TableLigne[vl_IndexLig].Protocole == XZECC_PROT_LCR) &&
                   (vg_TableLigne[vl_IndexLig].Sens == XZECC_SENS_ESCLAVE))
               {
                  sprintf(vl_Command, vg_ConfigST[7], vl_NumLigne);
                  XZST_03EcritureTrace(XZSTC_WARNING, "8:%d", ex_envoyer_commande_ST(vl_Command, vl_SockRet, &pl_masqueST));
               }
            }

            sprintf(vl_Command, vg_ConfigST[8], vl_NumLigneMax);
            XZST_03EcritureTrace(XZSTC_WARNING, "9:%d", ex_envoyer_commande_ST(vl_Command, vl_SockRet, &pl_masqueST));

            ed_dialogue_tesur();
         }

         /* Fermeture de la connexion avec le serveur de terminaux */
         if (vl_SockRet != 0)
         {
            ex_fin_cnx(vl_SockRet, &pl_masqueST);
            vl_SockRet = 0;
         }
      }
      else
      { /*config 40 ports*/
         XZST_03EcritureTrace(XZSTC_WARNING, "TESRV : 40 ports");
         system(C_SCRIPT_40);
      }

      ex_ConnexionSocketRTserver(&vg_SockRT, 0, &vg_SockMask);
      if (vg_ConfigTerm == XDC_OK)
         return (XDC_OK);

      /*
       *A Lancement des taches protocoles
       */
      /*A Boucle sur tout les Lignes pour lancer les taches protocoles correspondantes */
      for (vl_IndexLig = 0; vl_IndexLig < vl_NbConfLigne; vl_IndexLig++)
      {
         /*A Appel de la fonction el_lancer_tache_protocole */
         XZST_03EcritureTrace(XZSTC_WARNING, "LIGNE %s", vg_TableLigne[vl_IndexLig].NumLigne);
         if ((vl_NumLigne = atoi(vg_TableLigne[vl_IndexLig].NumLigne)) != 0)
         {
            el_lancer_tache_protocole(vg_TableLigne[vl_IndexLig].Protocole,
                                      vg_TableLigne[vl_IndexLig].Sens,
                                      vg_TableLigne[vl_IndexLig].NumLigne);
            XZST_03EcritureTrace(XZSTC_WARNING, "avant ed_dialogue_tesur");
            ed_dialogue_tesur();
            XZST_03EcritureTrace(XZSTC_WARNING, "apres ed_dialogue_tesur");
            ex_ConnexionSocketRTserver(&vg_SockRT, 0, &vg_SockMask);
            XZST_03EcritureTrace(XZSTC_WARNING, "apres ex_ConnexionSocketRTserver");
            if (vg_ConfigTerm == XDC_OK)
               return (XDC_OK);
            XZST_03EcritureTrace(XZSTC_WARNING, "apres vg_ConfigTerm");
            sleep(10);
            XZST_03EcritureTrace(XZSTC_WARNING, "apres sleep");
         }
      }

      /*A Boucle sur tout les equipement pour lancer les taches traitement correspondantes */
      for (vl_IndexEqt = 0; strlen(vg_TableEqt[vl_IndexEqt].Eqt.Equipement) > 0; vl_IndexEqt++)
      {
         /*A Si l'equipement est present */
         if (vg_TableEqt[vl_IndexEqt].Eqt.Present == XZEXC_EQT_PRESENT)
         {
            /*A Creation des taches traitement avec un appel a el_lancer_tache_eqt */
            ed_dialogue_tesur();
            ex_ConnexionSocketRTserver(&vg_SockRT, 0, &vg_SockMask);
            if (vg_ConfigTerm == XDC_OK)
               return (XDC_OK);

            /*B Si tache SATIR presente alors lancer tache tesx0.x  */
            if (vg_TableEqt[vl_IndexEqt].Eqt.Type == XZECC_TYPE_EQT_SAT)
            {
               strcpy(pl_NomTacheSpec, XZECC_TACHE_SX0);
               el_lancer_tache_eqt(pl_NomTacheSpec);
               sleep(2);
            }
            /*B Si tache RADT presente alors lancer tache tetdc.x ( Equation d'alerte RADT ) */
            if ((vm_Type_Machine != XZSCC_TYPEM_PCS) && (vg_TableEqt[vl_IndexEqt].Eqt.Type == XZECC_TYPE_EQT_RDT))
            {
               strcpy(pl_NomTacheSpec, XZECC_TACHE_TDC);
               el_lancer_tache_eqt(pl_NomTacheSpec);
               sleep(2);
            }

            /* ABE	27-04-2015 : projet suppression ancienne GTC (GTCA) */

            /* GTC renovee : on lance les 5 taches pour A8-A500 et 1 tache pour toulon */
            if (strcmp(vg_TableEqt[vl_IndexEqt].Eqt.Equipement, XZECC_TACHE_NIC) == 0)
            {
               if (strcmp(pl_NomSite, XDC_NOM_SITE_VC) == 0)
               {
                  /* A8-A500 */
                  el_lancer_tache_eqt(XZECC_TACHE_NX1);
                  el_lancer_tache_eqt(XZECC_TACHE_NX2);
                  el_lancer_tache_eqt(XZECC_TACHE_NX3);
                  el_lancer_tache_eqt(XZECC_TACHE_NX4);
                  el_lancer_tache_eqt(XZECC_TACHE_NX5);
               }
               if (strcmp(pl_NomSite, XDC_NOM_SITE_DP) == 0)
               {
                  /* ABE	fevrier-2020 : int�gration tenix6 pour toulon */
                  el_lancer_tache_eqt(XZECC_TACHE_NX6);
               }
            }
            else
            {
               el_lancer_tache_eqt(vg_TableEqt[vl_IndexEqt].Eqt.Equipement);
            }
         }
      }

      /*A Joindre le DataGroup XDG_ECTL_EQPT_<NomMachine> si pas deja abonne */
      XDG_EncodeDG2(vl_datagroup, XDG_ECTL_EQPT, pl_NomMachine);
      TipcSrvSubjectGetSubscribe(vl_datagroup, &vl_EtatDg);
      if (!vl_EtatDg)
      {
         if (!XDG_JoindreDG(vl_datagroup))
         {
            /*A Ecriture trace */
            XZST_03EcritureTrace(XZSTC_WARNING, " Impossible de joindre le datagroup %s. ", vl_datagroup);
         }
      }
   }
   else /*A Sinon configuration d'un seul equipement */
   {
      /*A Envoi d'une demande de reconfiguration de l'equipement */

      /*A Construction du nom de la tache a partir du type d'equipement */
      if (ex_nom_eqt(va_TypeEquipt, pl_PrefTache) != XDC_OK)
      {
         /*A retourne echec  */
         XZST_03EcritureTrace(XZSTC_WARNING, " Type:%d Pref:%s ", va_TypeEquipt, pl_PrefTache);
         return (XDC_NOK);
      }
      sprintf(pl_NomTache, "%s%s", XZECC_TACHE_EQUEXT, pl_PrefTache);

      /*A
       * Recherche si la tache existe ( scrutation de la liste des sockets )
       */

      /*A Pour toute les sockets du masque */
      for (pl_masque = vg_SockMask; pl_masque != NULL; pl_masque = pl_masque->Suivant)
      {
         /*A Si differente de la socket RTserver */
         if ((pl_masque->desc_sock != vg_SockRT) && (pl_masque->desc_sock != vg_SockEct))
         {
            /*A Si l'adresse de la socket correspont au nom de la tache */
            if (strstr(pl_masque->adresse, pl_NomTache) != NULL)
            {
               /*A Appel de la fonction ee_config pour emission de la demande de reconfig */
               ee_config(XZEXC_DEMANDE_INIT, pl_masque->desc_sock, pl_masque->adresse);
               break;
            }
         }
      } /* fin for */
   }    /* Fin si configuration d'un seul equipement */

   return (vl_ValRet);
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Commande des taches equipements ( arret ou lancement tache equipement )
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void ec_cmd_cb(
    T_IPC_CONN va_Cnx,
    T_IPC_CONN_PROCESS_CB_DATA pa_Data,
    T_CB_ARG pa_ARG)

/*
* ARGUMENTS EN ENTREE :
*  Message RTWorks
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
* CODE RETOUR :
*   Aucun
*
* CONDITION D'UTILISATION
*  Fonction appel�e lors de la r�ception du message
*  XDM_ECMD_EQPT
*
* FONCTION

*
------------------------------------------------------*/
{
   static char *version = "@(#)esrv_cfg.c	1.28 05/31/00      :  ec_cmd_cb ";
   XDY_NomMachine pl_NomMachine;

#ifndef _TIBCO_EMS
   T_STR pl_NomMachineRecp;
   T_INT4 vl_TypeEqt;  /* type d'equipement (un ou tous) */
   T_INT4 vl_Commande; /* status d'init ou de conf */
   T_INT4 vl_NumEqt;   /* Numero de l'equipement */
#else
   XDY_NomMachine pl_NomMachineRecp;
   XDY_Entier vl_TypeEqt;  /* type d'equipement (un ou tous) */
   XDY_Entier vl_Commande; /* status d'init ou de conf */
   XDY_Entier vl_NumEqt;   /* Numero de l'equipement */
#endif

   XDY_NomSite pl_NomSite;
   int vl_ValRet = XDC_OK;
   XDY_NomTache pl_NomTache;
   XDY_NomTache pl_NomTacheTenx1;
   XDY_NomTache pl_NomTacheTenx2;
   XDY_NomTache pl_NomTacheTenx3;
   XDY_NomTache pl_NomTacheTenx4;
   XDY_NomTache pl_NomTacheTenx5;
   XDY_NomTache pl_NomTacheTenx6;
   char pl_PrefTache[5];
   XZEXT_MASK *pl_masque;
   int vl_IndexLig = 0, vl_IndexEqt = 0, vl_Index = 0;
   T_BOOL vl_tache_tenic = FALSE;

   XZST_03EcritureTrace(XZSTC_FONCTION, "FONCTION: entree ec_cmd_cb");

   /*A Recuperation du status d'execution du message XDM_ECMD_EQPT */
   TipcMsgRead(pa_Data->msg,
               T_IPC_FT_INT4, &vl_Commande,
               T_IPC_FT_STR, &pl_NomMachineRecp,
               T_IPC_FT_INT4, &vl_TypeEqt,
               T_IPC_FT_INT4, &vl_NumEqt,
               NULL);

   /*A Recuperation du site de la machine */
   if (XZSC_04NomSite(pl_NomSite) != XDC_OK)
   {
      /* Ecriture Trace */
      XZST_03EcritureTrace(XZSTC_WARNING, "Recuperation du Nom du site impossible . ");
   }

   /*A Recuperation du nom de la machine */
   if (XZSC_07NomMachine(pl_NomMachine) != XDC_OK)
   {
      /* Ecriture Trace */
      XZST_03EcritureTrace(XZSTC_WARNING, "Recuperation du Nom de la machine impossible . ");
   }

   /*A Si le nom de la machine recu dans le message est different du nom de la machine de la tache */
   if (strcmp(pl_NomMachineRecp, pl_NomMachine) != 0)
   {
      /*A Ecriture trace nom machine incorrecte */
      XZST_03EcritureTrace(XZSTC_WARNING, "ec_cmd_cb : Nom Machine incorrect . ");
      XZST_03EcritureTrace(XZSTC_FONCTION, "FONCTION: sortie ec_cmd_cb");
      return;
   }
   else
   {
      /*A Construction du nom de la tache a partir du type d'equipement */
      if (ex_nom_eqt(vl_TypeEqt, pl_PrefTache) != XDC_OK)
      {
         /*A retourne echec  */
         XZST_03EcritureTrace(XZSTC_WARNING, "ec_cmd_cb : Type equipement incorrect . ");
         XZST_03EcritureTrace(XZSTC_FONCTION, "FONCTION: sortie ec_cmd_cb");
         return;
      }

      for (vl_IndexEqt = 0; strlen(vg_TableEqt[vl_IndexEqt].Eqt.Equipement) > 0; vl_IndexEqt++)
      {
         /*A Si l'equipement est present */
         if ((vg_TableEqt[vl_IndexEqt].Eqt.Present == XZEXC_EQT_PRESENT) &&
             (vg_TableEqt[vl_IndexEqt].Eqt.Type == vl_TypeEqt))
            vg_TableEqt[vl_IndexEqt].Etat = vl_Commande;
      }

      sprintf(pl_NomTache, "%s%s", XZECC_TACHE_EQUEXT, pl_PrefTache);
      sprintf(pl_NomTacheTenx1, "%s%s", XZECC_TACHE_EQUEXT, XZECC_TACHE_NX1);
      sprintf(pl_NomTacheTenx2, "%s%s", XZECC_TACHE_EQUEXT, XZECC_TACHE_NX2);
      sprintf(pl_NomTacheTenx3, "%s%s", XZECC_TACHE_EQUEXT, XZECC_TACHE_NX3);
      sprintf(pl_NomTacheTenx4, "%s%s", XZECC_TACHE_EQUEXT, XZECC_TACHE_NX4);
      sprintf(pl_NomTacheTenx5, "%s%s", XZECC_TACHE_EQUEXT, XZECC_TACHE_NX5);
      sprintf(pl_NomTacheTenx6, "%s%s", XZECC_TACHE_EQUEXT, XZECC_TACHE_NX6);

      /* Modification Bertola 11-06-2010 : correction FT 3493: Pas d'arret / relance de tenx5 sur reconfiguration du SAE */

      /* Marquer le fait que l'on traite la t�che tenic */
      if (strstr(pl_NomTache, "tenic") != NULL)
         vl_tache_tenic = TRUE;

      /*A
       * Recherche si la tache existe ( scrutation de la liste des sockets )
       */

      /* on traite diff�remment si t�che = tenic ou non */

      /* t�che non tenic : aucune modif par rapport au code existant */
      if (vl_tache_tenic == FALSE)
      {
         /*A Pour toutes les sockets du masque */
         for (pl_masque = vg_SockMask; pl_masque != NULL; pl_masque = pl_masque->Suivant)
         {
            /*A Si differente de la socket RTserver */
            if ((pl_masque->desc_sock != vg_SockRT) && (pl_masque->desc_sock != vg_SockEct))
            {
               /*A Si l'adresse de la socket correspont au nom de la tache */
               if (strstr(pl_masque->adresse, pl_NomTache) != NULL)
               {
                  /*A Si demmande d'arret */
                  if (vl_Commande == XZECC_EQT_ARRET)
                  {
                     /*A Appel de la fonction ee_config pour emission de la demande d'arret */
                     ee_config(XZEXC_DEMANDE_ARRET, pl_masque->desc_sock, pl_masque->adresse);
                     XZST_03EcritureTrace(XZSTC_FONCTION, "FONCTION: sortie ec_cmd_cb");
                     return;
                  }
                  else
                  /*A Si demmande de lancement de tache */
                  {
                     XZST_03EcritureTrace(XZSTC_WARNING, "ec_cmd_cb : Tache %s deja lanc�e . ", pl_NomTache);
                     XZST_03EcritureTrace(XZSTC_FONCTION, "FONCTION: sortie ec_cmd_cb");
                     return;
                  }
               }
            }
         } /* fin for */
      }    /* fin if */
      else
      {
         /* t�che tenic : on arrete la t�che tenic et les 5 t�ches tenx */

         /*A Pour toutes les sockets du masque */
         for (pl_masque = vg_SockMask; pl_masque != NULL; pl_masque = pl_masque->Suivant)
         {
            /*A Si differente de la socket RTserver */
            if ((pl_masque->desc_sock != vg_SockRT) && (pl_masque->desc_sock != vg_SockEct))
            {
               /*A Si l'adresse de la socket correspont au nom de la tache */
               if (strstr(pl_masque->adresse, pl_NomTache) != NULL || strstr(pl_masque->adresse, pl_NomTacheTenx1) != NULL || strstr(pl_masque->adresse, pl_NomTacheTenx2) != NULL ||
                   strstr(pl_masque->adresse, pl_NomTacheTenx3) != NULL || strstr(pl_masque->adresse, pl_NomTacheTenx4) != NULL || strstr(pl_masque->adresse, pl_NomTacheTenx5) != NULL)
               {
                  /*A Si demmande d'arret */
                  if (vl_Commande == XZECC_EQT_ARRET)
                  {
                     /*A Appel de la fonction ee_config pour emission de la demande d'arret */
                     XZST_03EcritureTrace(XZSTC_WARNING, "ec_cmd_cb : On arrete la tache  %s", pl_masque->adresse);
                     ee_config(XZEXC_DEMANDE_ARRET, pl_masque->desc_sock, pl_masque->adresse);
                  }
                  else
                  /*A Si demande de lancement de tache */
                  {
                     XZST_03EcritureTrace(XZSTC_WARNING, "ec_cmd_cb : Tache %s deja lanc�e . ", pl_masque->adresse);
                  }
               }
            }
         } /* fin for */
         XZST_03EcritureTrace(XZSTC_FONCTION, "FONCTION: sortie ec_cmd_cb");
         return;
      } /* fin if else */

      /*A
       * La tache demandee n'est pas en cours d'execution
       */

      /*A Si demmande d'arret */
      if (vl_Commande == XZECC_EQT_ARRET)
      {
         /*A Sortie erreur */
         XZST_03EcritureTrace(XZSTC_WARNING, "ec_cmd_cb : Tache %s n'est pas en cours . ", pl_NomTache);
         XZST_03EcritureTrace(XZSTC_FONCTION, "FONCTION: sortie ec_cmd_cb");
         return;
      }

      /*A Si demmande de lancement de tache */

      /*A Boucle sur tout les equipement pour lancer les taches traitement correspondantes */
      for (vl_IndexEqt = 0; strlen(vg_TableEqt[vl_IndexEqt].Eqt.Equipement) > 0; vl_IndexEqt++)
      {
         /*A Si l'equipement est present */
         if ((vg_TableEqt[vl_IndexEqt].Eqt.Present == XZEXC_EQT_PRESENT) &&
             (vg_TableEqt[vl_IndexEqt].Eqt.Type == vl_TypeEqt))
         {
            /*A lancement des taches protocoles */
            for (vl_Index = 0; vl_Index < XZECC_MAX_EQT; vl_Index++)
            {
               if (strlen(vg_TableEqt[vl_IndexEqt].LigneEqt[vl_Index].NumLigne) != 0)
               {
                  el_lancer_tache_protocole(vg_TableEqt[vl_IndexEqt].LigneEqt[vl_Index].Protocole,
                                            vg_TableEqt[vl_IndexEqt].LigneEqt[vl_Index].Sens,
                                            vg_TableEqt[vl_IndexEqt].LigneEqt[vl_Index].NumLigne);
                  sleep(3);
               }
            }

            /*B Si tache SATIR presente alors lancer tache tesx0.x  */
            if (vg_TableEqt[vl_IndexEqt].Eqt.Type == XZECC_TYPE_EQT_SAT)
            {
               el_lancer_tache_eqt(XZECC_TACHE_SX0);
               sleep(2);
            }
            /*B Si tache RADT presente alors lancer tache tetdc.x ( Equation d'alerte RADT ) */
            if ((vm_Type_Machine != XZSCC_TYPEM_PCS) && (vg_TableEqt[vl_IndexEqt].Eqt.Type == XZECC_TYPE_EQT_RDT))
            {
               el_lancer_tache_eqt(XZECC_TACHE_TDC);
               sleep(2);
            }

            /* ABE	27-04-2015 : projet suppression ancienne GTC (GTCA) */

            /* GTC renovee : on lance les 5 taches pour A8-A500 et 1 tache pour toulon */
            if (strcmp(vg_TableEqt[vl_IndexEqt].Eqt.Equipement, XZECC_TACHE_NIC) == 0)
            {
               if (strcmp(pl_NomSite, XDC_NOM_SITE_VC) == 0)
               {
                  /* A8-A500 */
                  el_lancer_tache_eqt(XZECC_TACHE_NX1);
                  el_lancer_tache_eqt(XZECC_TACHE_NX2);
                  el_lancer_tache_eqt(XZECC_TACHE_NX3);
                  el_lancer_tache_eqt(XZECC_TACHE_NX4);
                  el_lancer_tache_eqt(XZECC_TACHE_NX5);
               }
               if (strcmp(pl_NomSite, XDC_NOM_SITE_DP) == 0)
               {
                  /* ABE	fevrier-2020 : int�gration tenix6 pour toulon */
                  el_lancer_tache_eqt(XZECC_TACHE_NX6);
               }
            }
            else
            {
               /*A Creation des taches traitement avec un appel a el_lancer_tache_eqt */
               el_lancer_tache_eqt(vg_TableEqt[vl_IndexEqt].Eqt.Equipement);
            }

            return;
         }
      }
   }

   XZST_03EcritureTrace(XZSTC_WARNING, " Tache traitement %s inconnue. ",
                        vg_TableEqt[vl_IndexEqt].Eqt.Equipement);
   return;
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Cette fonction effectue la surveillance des taches de la machine.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void ec_surveillance_tache(void)

/*
* ARGUMENTS EN ENTREE :
*   aucun
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR :
*   XDC_OK	succes
*   XDC_NOK	echec
*
* CONDITION D'UTILISATION
*
*
* FONCTION
*
*
------------------------------------------------------*/
{
   static char *version = "@(#)esrv_cfg.c	1.28 05/31/00      :  ec_surveillance_tache ";
   XDY_NomTache pl_NomTache;
   char pl_PrefTache[5];
   XZEXT_MASK *pl_masque;
   int vl_IndexLig = 0, vl_IndexEqt = 0, vl_Index = 0;
   static int vl_CptTest = 4;
   XDY_NomSite pl_NomSite;

   XZST_03EcritureTrace(XZSTC_FONCTION, " FONCTION: IN ec_surveillance_tache");
   /*A Recuperation du site de la machine */
   if (XZSC_04NomSite(pl_NomSite) != XDC_OK)
   {
      /* Ecriture Trace */
      XZST_03EcritureTrace(XZSTC_WARNING, "Recuperation du Nom du site impossible . ");
   }

   /*A Boucle sur tout les equipement pour lancer les taches traitement correspondantes */
   for (vl_IndexEqt = 0; strlen(vg_TableEqt[vl_IndexEqt].Eqt.Equipement) > 0; vl_IndexEqt++)
   {
      // XZST_03EcritureTrace( XZSTC_WARNING, "ec_surveillance_tache : boucle sur vg_TableEqt avec vl_IndexEqt = %d",  vl_IndexEqt);
      /*A Si l'equipement est present */
      if ((vg_TableEqt[vl_IndexEqt].Eqt.Present == XZEXC_EQT_PRESENT) &&
          (vg_TableEqt[vl_IndexEqt].Etat != XZECC_EQT_ARRET))
      {
         if (ex_nom_eqt(vg_TableEqt[vl_IndexEqt].Eqt.Type, pl_PrefTache) == XDC_OK)
         {
            if (strcmp(vg_TableEqt[vl_IndexEqt].Eqt.Equipement, XZECC_TACHE_NIC) == 0)
               sprintf(pl_NomTache, "%s%s1", XZECC_TACHE_EQUEXT, pl_PrefTache);
            else
               sprintf(pl_NomTache, "%s%s", XZECC_TACHE_EQUEXT, pl_PrefTache);

            /*A  Recherche si la tache existe ( scrutation de la liste des sockets ) */
            for (pl_masque = vg_SockMask; pl_masque != NULL; pl_masque = pl_masque->Suivant)
               /*A Si l'adresse de la socket correspont au nom de la tache */
               if (strstr(pl_masque->adresse, pl_NomTache) != NULL)
                  break;
            if (pl_masque == NULL)
            {
               /*A lancement des taches protocoles */
               for (vl_Index = 0; vl_Index < XZECC_MAX_EQT; vl_Index++)
               {
                  if (strlen(vg_TableEqt[vl_IndexEqt].LigneEqt[vl_Index].NumLigne) != 0)
                  {
                     el_lancer_tache_protocole(vg_TableEqt[vl_IndexEqt].LigneEqt[vl_Index].Protocole,
                                               vg_TableEqt[vl_IndexEqt].LigneEqt[vl_Index].Sens,
                                               vg_TableEqt[vl_IndexEqt].LigneEqt[vl_Index].NumLigne);
                     sleep(10);
                  }
               }

               /* Si tache SATIR presente alors lancer tache tesx0.x  */
               if (vg_TableEqt[vl_IndexEqt].Eqt.Type == XZECC_TYPE_EQT_SAT)
               {
                  el_lancer_tache_eqt(XZECC_TACHE_SX0);
                  sleep(2);
               }
               /* Si tache RADT presente alors lancer tache tetdc.x ( Equation d'alerte RADT ) */
               if ((vm_Type_Machine != XZSCC_TYPEM_PCS) && (vg_TableEqt[vl_IndexEqt].Eqt.Type == XZECC_TYPE_EQT_RDT))
               {
                  el_lancer_tache_eqt(XZECC_TACHE_TDC);
                  sleep(2);
               }

               /* ABE	27-04-2015 : projet suppression ancienne GTC (GTCA) */

               /* GTC renovee : on lance les 5 taches pour A8-A500 et 1 tache pour toulon */
               if (strcmp(vg_TableEqt[vl_IndexEqt].Eqt.Equipement, XZECC_TACHE_NIC) == 0)
               {
                  if (strcmp(pl_NomSite, XDC_NOM_SITE_VC) == 0)
                  {
                     /* A8-A500 */
                     el_lancer_tache_eqt(XZECC_TACHE_NX1);
                     el_lancer_tache_eqt(XZECC_TACHE_NX2);
                     el_lancer_tache_eqt(XZECC_TACHE_NX3);
                     el_lancer_tache_eqt(XZECC_TACHE_NX4);
                     el_lancer_tache_eqt(XZECC_TACHE_NX5);
                  }
                  if (strcmp(pl_NomSite, XDC_NOM_SITE_DP) == 0)
                  {
                     /* ABE	fevrier-2020 : int�gration tenix6 pour toulon */
                     el_lancer_tache_eqt(XZECC_TACHE_NX6);
                  }
               }
               else
               {
                  /*A Creation des taches traitement avec un appel a el_lancer_tache_eqt */
                  el_lancer_tache_eqt(vg_TableEqt[vl_IndexEqt].Eqt.Equipement);
               }
            }
         }
      }
   }
   // XZST_03EcritureTrace( XZSTC_WARNING, "ec_surveillance_tache : on est sorti de la boucle vl_CptTest = %d",vl_CptTest);

   /*A Test etat serveur de terminaux */
   if (vl_CptTest++ > 3)
   {
      vl_CptTest = 0;
      for (vl_Index = 0; vm_ListeST[vl_Index][0] != '\0'; vl_Index++)
      {
         if (ex_presence_machine(vm_ListeST[vl_Index]) == XDC_OK)
         {
            if (vm_EtatST[vl_Index] != XZSEC_ETAT_OK)
            {
               vm_EtatST[vl_Index] = XZSEC_ETAT_OK;
               XZSE_14EnvoieEtatPeriph(vm_ListeST[vl_Index], vm_EtatST[vl_Index]);
               XZST_03EcritureTrace(XZSTC_WARNING, "Transerver %s OK", vm_ListeST[vl_Index]);
            }
         }
         else
         {
            if (vm_EtatST[vl_Index] != XZSEC_ETAT_NOK)
            {
               vm_EtatST[vl_Index] = XZSEC_ETAT_NOK;
               XZSE_14EnvoieEtatPeriph(vm_ListeST[vl_Index], vm_EtatST[vl_Index]);
               XZST_03EcritureTrace(XZSTC_WARNING, "Transerver %s NOK", vm_ListeST[vl_Index]);
            }
         }
      }
   }
   XZST_03EcritureTrace(XZSTC_FONCTION, " FONCTION: OUT ec_surveillance_tache");
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Envoie a tsetat le status des serveurs de terminaux
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void ec_dde_etat_ST_cb(
    T_IPC_CONN va_Cnx,
    T_IPC_CONN_PROCESS_CB_DATA pa_Data,
    T_CB_ARG pa_ARG)

/*
* ARGUMENTS EN ENTREE :
*  Message RTWorks
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
* CODE RETOUR :
*   Aucun
*
* CONDITION D'UTILISATION
*  Fonction appel�e lors de la r�ception du message
*  XDM_TSE_DIFE
*
* FONCTION

*
------------------------------------------------------*/
{
   static char *version = "@(#)esrv_cfg.c	1.28 05/31/00      :  ec_dde_etat_ST_cb ";
   int vl_Index = 0;
   T_IPC_MT vl_Mt;
#ifdef _TIBCO_EMS
   XDY_Entier vl_Num_Msg = 0;
#else
   T_INT4 vl_Num_Msg = 0;
#endif
   XZST_03EcritureTrace(XZSTC_FONCTION, "FONCTION: IN  ec_dde_etat_ST_cb");

#ifndef _TIBCO_EMS
   if ((TipcMsgGetType(pa_Data->msg, &vl_Mt)) &&
       (TipcMtGetNum(vl_Mt, &vl_Num_Msg)) &&
       (vl_Num_Msg == XDM_TSE_DIFE))
#else
   if ((TipcMsgGetType(pa_Data->msg, (int *)&vl_Num_Msg)) &&
       (vl_Num_Msg == XDM_TSE_DIFE))
#endif
   {
      /*A Pour tous les serveurs de terminaux */
      for (vl_Index = 0; vm_ListeST[vl_Index][0] != '\0'; vl_Index++)
         /*A Appel de XZSE_14EnvoieEtatPeriph */
         XZSE_14EnvoieEtatPeriph(vm_ListeST[vl_Index], vm_EtatST[vl_Index]);
   }

   XZST_03EcritureTrace(XZSTC_FONCTION, "FONCTION: OUT ec_dde_etat_ST_cb Numero de Msg:%d", vl_Num_Msg);
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Cette fonction effectue la lecture du fichier XDF_Presence_Equip_<NomMachine>.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int ex_lire_config_eqt(XDY_NomMachine pa_NomMachine,
                       int *pa_NbEqt)

/*
* ARGUMENTS EN ENTREE :
*   pa_NomMachine	: Nom de la machine
*
* ARGUMENTS EN SORTIE :
*   pa_NbEqt		: Nombre d'equipement a configurer
*
* CODE RETOUR :
*   XDC_OK	succes
*   XDC_NOK	echec
*
* CONDITION D'UTILISATION
*
*
* FONCTION
*
*
------------------------------------------------------*/
{
   static char *version = "@(#)esrv_cfg.c	1.28 05/31/00      :  ex_lire_config_eqt ";
   int vl_ValRet = XDC_OK;
   int vl_Fd;
   char pl_PathEtFic[XDC_PATH_ABS_SIZE];
   char *pl_Donnee;
   XDY_Commentaire pl_LigneFich;
   int vl_NbEqt = 0, vl_Presence = 0, vl_Index = 0;

   /*A Ouverture du fichier presence equipement */
   /* Construction du nom du fichier XDF_Presence_Equip_<NomMachine>  */
   sprintf(pl_PathEtFic, "%s/%s_%s", XZEXC_PATH_CONFIG1, XDF_Presence_Equip, pa_NomMachine);

   /*A Ouverture du fichier XDF_Presence_Equip_<NomMachine>  */
   if ((vl_Fd = open(pl_PathEtFic, O_RDONLY)) <= 0)
   {
      /*A Ecriture trace absence fichier */
      XZST_03EcritureTrace(XZSTC_WARNING, " Manque fichier %s. ", pl_PathEtFic);
      /*A Sortie de la fonction */
      return (XDC_NOK);
   }

   /*A Lecture de chaque ligne du fichier */
   while (ex_LireLigneFich(vl_Fd, pl_LigneFich, sizeof(pl_LigneFich)) != XZEXC_EOF)
   {
      /*A Lire ligne suivante si c'est une ligne de commentaire */
      if (*pl_LigneFich != XDF_COMMENTAIRE)
      {
         /*A Extraire le nom de l'equipement et si il est present */
         sscanf(pl_LigneFich, "%s %d %d",
                vg_TableEqt[vl_NbEqt].Eqt.Equipement,
                &vl_Presence,
                &vg_TableEqt[vl_NbEqt].Eqt.Type);
         vg_TableEqt[vl_NbEqt].Eqt.Present = vl_Presence;
         XZST_03EcritureTrace(XZSTC_DEBUG1, "Equipt:%s Pres:%d type:%d",
                              vg_TableEqt[vl_NbEqt].Eqt.Equipement,
                              vg_TableEqt[vl_NbEqt].Eqt.Present,
                              vg_TableEqt[vl_NbEqt].Eqt.Type);
         if ((pl_Donnee = strtok(pl_LigneFich, "$")) != NULL)
         {
            while ((pl_Donnee = strtok(NULL, "$")) != NULL)
            {
               XZST_03EcritureTrace(XZSTC_DEBUG1, "Param:%s", pl_Donnee);
               if (sscanf(pl_Donnee, " %s %s %c %c",
                          vg_TableEqt[vl_NbEqt].LigneEqt[vl_Index].NumeroST,
                          vg_TableEqt[vl_NbEqt].LigneEqt[vl_Index].NumLigne,
                          &vg_TableEqt[vl_NbEqt].LigneEqt[vl_Index].Protocole,
                          &vg_TableEqt[vl_NbEqt].LigneEqt[vl_Index].Sens) == 4)
               {
                  XZST_03EcritureTrace(XZSTC_DEBUG1, "ST:%s Lig:%s Prot:%c Sens:%c",
                                       vg_TableEqt[vl_NbEqt].LigneEqt[vl_Index].NumeroST,
                                       vg_TableEqt[vl_NbEqt].LigneEqt[vl_Index].NumLigne,
                                       vg_TableEqt[vl_NbEqt].LigneEqt[vl_Index].Protocole,
                                       vg_TableEqt[vl_NbEqt].LigneEqt[vl_Index].Sens);
                  vl_Index++;
               }
            }
         }

         XZST_03EcritureTrace(XZSTC_DEBUG1, " Eqt : %s      Presence : %d \n", vg_TableEqt[vl_NbEqt].Eqt.Equipement, vg_TableEqt[vl_NbEqt].Eqt.Present);
         /*A Si ligne lu non vide */
         if (strlen(vg_TableEqt[vl_NbEqt].Eqt.Equipement) > 0)
         {
            /*A Incrementer le pointeur sur le tableau */
            vl_NbEqt++;
         }
      }
   }
   /*A Fermeture du fichier XDF_Presence_Equip_<NomMachine>  */
   XZSS_13FermerFichier(vl_Fd);
   /*A Renvoie le nombre d'equipement a configurer */
   *pa_NbEqt = vl_NbEqt;
   /*A Retourne compte rendu OK */
   return (vl_ValRet);
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Cette fonction effectue la lecture du fichier XDF_Config_Lignes_<NomMachine>.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int ex_lire_config_lignes(XDY_NomMachine pa_NomMachine,
                          int *pa_NbLigne)

/*
* ARGUMENTS EN ENTREE :
*   pa_NomMachine	: Nom de la machine
*
* ARGUMENTS EN SORTIE :
*   pa_NbLigne		: Nombre de ligne a configurer
*
* CODE RETOUR :
*   XDC_OK	succes
*   XDC_NOK	echec
*
* CONDITION D'UTILISATION
*
*
* FONCTION
*
*
------------------------------------------------------*/
{
   static char *version = "@(#)esrv_cfg.c	1.28 05/31/00      :  ex_lire_config_lignes ";
   int vl_ValRet = XDC_OK;
   int vl_Fd;
   char pl_PathEtFic[XDC_PATH_ABS_SIZE];
   XDY_Commentaire pl_LigneFich;
   int vl_NbConfLigne = 0;
   int vl_NbST = 0;

   /*A
    * Lecture du fichier de configuration XDF_Config_Lignes_<NomMachine>
    */
   /*A Construction du nom du fichier XDF_Config_Lignes_<NomMachine>  */
   sprintf(pl_PathEtFic, "%s/%s_%s", XZEXC_PATH_CONFIG1, XDF_Config_Lignes, pa_NomMachine);

   /*A Ouverture du fichier XDF_Config_Lignes_<NomMachine>  */
   if ((vl_Fd = open(pl_PathEtFic, O_RDONLY)) <= 0)
   {
      /*A Ecriture trace absence fichier */
      XZST_03EcritureTrace(XZSTC_WARNING, " Manque fichier %s. ", pl_PathEtFic);
      /*A Sortie de la fonction */
      return (XDC_NOK);
   }

   /*A Lecture de chaque ligne du fichier */
   while (ex_LireLigneFich(vl_Fd, pl_LigneFich, sizeof(pl_LigneFich)) != XZEXC_EOF)
   {
      /*A Lire ligne suivante si c'est une ligne de commentaire */
      if (*pl_LigneFich != XDF_COMMENTAIRE)
      {
         /*A Extraire le nom de l'equipement et l'option presence */
         sscanf(pl_LigneFich, "%s %s %c %c",
                vg_TableLigne[vl_NbConfLigne].NumeroST,
                vg_TableLigne[vl_NbConfLigne].NumLigne,
                &(vg_TableLigne[vl_NbConfLigne].Protocole),
                &(vg_TableLigne[vl_NbConfLigne].Sens));

         XZST_03EcritureTrace(XZSTC_DEBUG1, "ST:%s/NL:%s/P:%c/S:%c",
                              vg_TableLigne[vl_NbConfLigne].NumeroST,
                              vg_TableLigne[vl_NbConfLigne].NumLigne,
                              vg_TableLigne[vl_NbConfLigne].Protocole,
                              vg_TableLigne[vl_NbConfLigne].Sens);
         /*A Si ligne lu non vide */
         if (strlen(vg_TableLigne[vl_NbConfLigne].NumeroST) > 0)
         {
            /*B Incrementer le nombre d'equipement */
            if (strcmp(vm_ListeST[vl_NbST], vg_TableLigne[vl_NbConfLigne].NumeroST))
            {
               vl_NbST += (vm_ListeST[vl_NbST][0] == '\0') ? 0 : 1;
               strcpy(vm_ListeST[vl_NbST], vg_TableLigne[vl_NbConfLigne].NumeroST);
            }
            vl_NbConfLigne++;
         }
      }
   }
   /*A Fermeture du fichier  XDF_Config_Lignes_<NomMachine> */
   XZSS_13FermerFichier(vl_Fd);

   /*A Renvoie le nombre de ligne a configurer */
   *pa_NbLigne = vl_NbConfLigne;
   strcpy(vm_ListeST[++vl_NbST], "");
   /*A Retourne compte rendu OK */
   return (vl_ValRet);
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void ec_verif_arret_tache(void)

/*
* ARGUMENTS EN ENTREE :
*
* ARGUMENTS EN SORTIE :
*
* CODE RETOUR :
*
* CONDITION D'UTILISATION
*
*
* FONCTION
*
*
------------------------------------------------------*/
{
   int vl_IndexEqt = 0;
   int vl_IndexLig = 0;

   /*
    *A Verification arret des taches protocoles
    */
   /*A Boucle sur tout les Lignes pour arret les taches protocoles correspondantes */
   for (vl_IndexLig = 0; vl_IndexLig < XZECC_MAX_EQT; vl_IndexLig++)
   {
      /*A Appel de la fonction el_lancer_tache_protocole */
      if (atoi(vg_TableLigne[vl_IndexLig].NumLigne) != 0)
         el_arret_tache(vg_TableLigne[vl_IndexLig].Protocole,
                        vg_TableLigne[vl_IndexLig].Sens,
                        vg_TableLigne[vl_IndexLig].NumLigne);
   }

   /*A Boucle sur tout les equipement pour arreter les taches traitement correspondantes */
   for (vl_IndexEqt = 0; strlen(vg_TableEqt[vl_IndexEqt].Eqt.Equipement) > 0; vl_IndexEqt++)
   {
      /*A Si l'equipement est present */
      if (vg_TableEqt[vl_IndexEqt].Eqt.Present == XZEXC_EQT_PRESENT)
      {
         if ((vm_Type_Machine != XZSCC_TYPEM_PCS) && (vg_TableEqt[vl_IndexEqt].Eqt.Type == XZECC_TYPE_EQT_RDT))
            el_arret_tache(0, 0, XZECC_TACHE_TDC);
         if (vg_TableEqt[vl_IndexEqt].Eqt.Type == XZECC_TYPE_EQT_SAT)
            el_arret_tache(0, 0, XZECC_TACHE_SX0);
         el_arret_tache(0, 0, vg_TableEqt[vl_IndexEqt].Eqt.Equipement);
      }
   }
}
