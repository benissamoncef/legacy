/*Fichier : $Id: isbi_sma.c,v 1.9 2020/11/03 17:49:24 pc2dpdy Exp $      Release : $Revision: 1.9 $        Date : $Date: 2020/11/03 17:49:24 $
------------------------------------------------------
 * STERIA *  PROJET MIGRAZUR
------------------------------------------------------
 * SOUS-SYSTEME  GESIHM
------------------------------------------------------
 * MODULE MSMA * FICHIER isbi_sma.c
------------------------------------------------------
 * DESCRIPTION DU MODULE :
 *
 * < description rapide des fonctions remplies par le module
 *   et reference au dossier de conception generale
 *   ou reference au fichier isbi_sma.h >
 *
------------------------------------------------------
 * HISTORIQUE :
 *
 * Niepceron	13 Sep 1994	: Creation
 * VR		29 mai 2012 : PRV DEM 1016
 * JMG   20/03/17        sag DEM1191
 * JMG	20/06/17	tigmot-tisup 1.4
 * LCL	17/12/17	optimisation tigmot-tisup 1.5
 * RGR 23/04/19 : DEM1331 - Mecanisme Keep Alive 1.5
* JMG	08/06/20 : EMS 1.9
* LCL		20/12/21	: MOVIS P2 ems SAE-312
------------------------------------------------------*/

/* fichiers inclus */
#include <xzsem.h>
#include "isbi_sma.h"
#include "isbi_sco.h"
#include "xzao790.h"
#include "xzao730.h"
#include "xzsa.h"
#include "xzsi.h"
#include "xzao000.h"

/* definitions de constantes */
#define CM_ATTENTE 0.001
#define CM_3MEGA 3000000
#define C_PATH "/produits/migrazur/appliSD/fichiers/syn/supervisionCS5/share/xml/"
/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static int vm_MSMA_init = XDC_NON;
static char vm_region[5];
static char vm_machine_courante[20];
static int vm_nb_mac = 0;
static int vm_nb_tac = 0;

DONNEES *pg_debut;

typedef struct
{
  int NumMsg;       /* Numero message RTW */
  void (*FoncCB)(); /* Fonction callback */
} tm_callback;

tm_callback pm_TabCB[] = {
    {XDM_ADM_SSS, (void (*)())MajAdmSsSys},
    {XDM_ADM_TAC, (void (*)())MajAdmTache},
    {XDM_ADM_ARC_SER, (void (*)())MajAdmServeur},
    {XDM_ADM_ARC_PER, (void (*)())MajAdmPeriph},
    {XDM_ADM_ARC_INT, (void (*)())MajAdmInterf},
    {XDM_ADM_TRA, (void (*)())MajAdmTrace},
    {XDM_ADM_ARC_STA_M, (void (*)())MajAdmStatiM},
    {XDM_ADM_ARC_STA_E, (void (*)())MajAdmStatiE}};

typedef struct
{
  int *buffer;
  int currIndex;
  int maxIndex;
  XZSCT_NomSite site;
  XZSCT_NomMachine machine;
} tm_SYMTAB_BUILD_ARGS;

extern XDY_Booleen vg_tigmot_tisup;
extern XDY_Booleen vm_MSOP_courbe;
extern XDY_Nom vm_NomMachineSS;

/* declaration de fonctions internes */
void MSMA_arret();
void MSMA_construit_region();
void MSMA_construit_taches_machine();
void MSMA_add_region(XZSCT_NomSite va_nom,
                     int va_nb,
                     FILE *pa_fd);
void MSMA_add_machines_region(XZSCT_NomMachine va_machine,
                              int va_nb,
                              FILE *pa_fd);
void MSMA_add_taches_machine(XZSCT_NomTache va_tac,
                             int va_nb,
                             FILE *pa_fd);

/* definition de fonctions externes */
extern int MSVU_init_affichage_vues(char *, char *);
extern int MSOP_init_trt_actions_operateur();
extern int MSRA_init_rafraichissement_synopt();
extern void MSRA_Fin();
extern int MSCO_init_gestion_courbes();

extern int MSOP_Fermer_dairadtaires();
extern int MSVU_afficher_vue();
extern int MSVU_init_applix();
extern int vg_serv_sock_ToFlash;
extern int vg_serv_sock_FromFlash;
extern int vg_serv_sock_Courbes;
extern int vg_cli_sock_ToFlash;
extern int vg_cli_sock_FromFlash;
extern int vg_cli_sock_Courbes;
extern FILE *vg_tracefile;
extern XDY_Booleen vg_pas_tisse;

extern void iterate_msg(XZSCT_NomMachine va_machine);

extern void MajAdm_Mach_Synt(
    XZSCT_NomMachine va_NomMachine,
    int va_EtatMach);

extern void MajAdm_Ent_Synt(
    XZSCT_NomEntite va_NomEntite,
    int va_EtatEnt,
    int va_TypeEnt);

extern void MajAdm_Res_Synt(
    XZSCT_NomMachine va_NomMachine,
    int va_EtatMach);
extern void MajSynt_Mach_Synop(
    XZSCT_NomMachine pa_NomMachine,
    int va_NumOrdre,
    void **pa_DonneesMsg);
extern void MajSynt_Ent_Synop(
    XZSCT_NomEntite pa_NomEntite,
    int va_NumOrdre,
    void **pa_DonneesMsg);
extern void MajSynt_Res_Synop(
    XZSCT_NomMachine pa_NomMachine,
    int va_NumOrdre,
    void **pa_DonneesMsg);

float vm_posX_start_regions = 0;
float vm_posY_start_regions = 0;
int vm_nb_par_ligne_regions = 5;
int vm_decalage_X_regions = 10;
int vm_decalage_Y_regions = 10;
float vm_posX_start_machines = 0;
float vm_posY_start_machines = 0;
int vm_nb_par_ligne_machines = 5;
int vm_decalage_X_machines = 10;
int vm_decalage_Y_machines = 10;
float vm_posX_start_taches = 0;
float vm_posY_start_taches = 0;
int vm_nb_par_ligne_taches = 5;
int vm_decalage_X_taches = 10;
int vm_decalage_Y_taches = 10;

static void GMInitVarTac(XZSCT_NomMachine va_nom,
                         int va_nb,
                         tm_SYMTAB_BUILD_ARGS *pa_Args)
{
  XZSE_22InitSynoptiqueTache(va_nom);
  XZST_09InitSynoptiqueTrace(va_nom);
}

static void GMInitVarSSys(XZSCT_NomSite va_nom,
                          int va_nb,
                          tm_SYMTAB_BUILD_ARGS *pa_Args)
{
  XZSE_21InitSynoptiqueSS(va_nom);
}

int isbi_ajouter_eqt(XZAOT_ConfEqtSAGA va_eqt)
{
  DONNEES *pl_NewZone;
  DONNEES *pl_Zones;

  XZST_03EcritureTrace(XZSTC_INTERFACE, "ec_ajouter_eqt : eqt %s",
                       va_eqt.NomEqtSAGA);

  if ((pl_NewZone = (DONNEES *)malloc(sizeof(DONNEES))) == NULL)
  {
    XZST_03EcritureTrace(XZSTC_WARNING, " OUT: creer_liste_zones Allocation memoire impossible");
    return XDC_NOK;
  }

  pl_NewZone->Config = va_eqt;
  pl_NewZone->Suivant = NULL;
  pl_NewZone->Suivant = pg_debut;
  pg_debut = pl_NewZone;

  return (XDC_OK);
}

/*------------------------------------------------------
 * SERVICE RENDU :
 *  Fonction d'init de la tache TISYN
------------------------------------------------------
 * SEQUENCE D'APPEL :
 */

MSMA_init(
    argc, argv)

    int argc;
char **argv;
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
 *   XDC_OK
 *   XDC_NOK
 *
 * CONDITION D'UTILISATION
 *   < Explication succinte sur les conditions  d'utilisation
 *     de la fonction
 *
 * FONCTION
 *
 *
------------------------------------------------------*/
{

  XZSCT_Datagroup vl_Datagroup;
  char *pl;
  FILE *vl_fd;

  XDY_Entier vl_cr;
  char vl_Lig[500];
  int vl_ValRet;
  T_IPC_MT vl_mt;
  int vl_nCB;
  int vl_numMsg;
  int vl_dummy;
  void *pl_dummy;

  XZST_03EcritureTrace(XZSTC_FONCTION, "IN  : MSMA_init");
  /* Initialisation si cela n'a pas deja ete fait */
  if (vm_MSMA_init == XDC_NON)
  {
    vm_MSMA_init = XDC_OUI;

    /* Connexion a la supervision */
    if (vm_cx == XDC_OUI)
    {
      /* Initialisation des structures des messages utilises et connection au RTServer */
      XZSC_01InitCnxSup(NULL,
                        argv[0],
                        XZSCC_INTERDIRE,
                        XZSCC_AUTORISER,
                        XZSCC_INTERDIRE,
                        MSMA_arret,
                        getenv("tty")
                        /* "/dev/null"*/);

      /* Recuperation du nom machine */
      if (XZSC_07NomMachine(vm_NomMachine) == XZSCC_INI_INV)
      {
        XZST_03EcritureTrace(XZSTC_WARNING, "Initialisation du nom de la machine non effectuee ");
        return XDC_NOK;
      }
      /* Recuperation du nom du site */
      if (XZSC_04NomSite(vm_NomSite) == XZSCC_INI_INV)
      {
        XZST_03EcritureTrace(XZSTC_WARNING, "Initialisation du nom du site non effectuee ");
        return XDC_NOK;
      }
      /* Recuperation du type de machine */
      if (XZSC_05TypeMachine(&vm_TypeMachine) == XZSCC_INI_INV)
      {
        XZST_03EcritureTrace(XZSTC_WARNING, "Initialisation du type de machine non effectuee ");
        return XDC_NOK;
      }

      /* Abonnement au DataGroup KEEPALIVE */
      XZSC_31AbtKeepAlive(vm_NomSite);
    }

    if (argc == 3)
    {
      if (!strcmp(argv[2], "trace"))
      {
        /*ouverture fichier de trace*/
        vg_tracefile = fopen("/produits/migrazur/appliSD/exec/tisbi.log", "w");
        if (vg_tracefile == NULL)
          XZST_03EcritureTrace(XZSTC_WARNING, "Probleme lors de l ouverture du fichier trace");
        else
          XZST_03EcritureTrace(XZSTC_WARNING, "mode trace");
      }
      else
        XZST_03EcritureTrace(XZSTC_WARNING, "mode no trace");
    }

    /*XZST_08ForceNivVisuTraceLocal(XZSTC_FATAL|XZSTC_WARNING|XZSTC_INFO|XZSTC_FONCTION|XZSTC_INTERFACE|XZSTC_MESSAGE,XZSTC_NECRAN);*/
    XZST_08ForceNivVisuTraceLocal(XZSTC_WARNING, XZSTC_NECRAN);

        if (vg_tigmot_tisup == XDC_VRAI)
        { /* tigmot */
          XZST_03EcritureTrace(XZSTC_FONCTION, "init tigmot tisup");
    /*construction du fichier xml region_tisup_region_object.xml*/
          MSMA_construit_region();
        }
    /*Abonnement aux messages tseta*/
    //            for ( vl_nCB = 0; vl_nCB < (sizeof(pm_TabCB)/sizeof(pm_TabCB[0])); vl_nCB++ )
    //            {
    //              vl_mt = XDM_IdentMsg( pm_TabCB[vl_nCB].NumMsg );
    //              if ( vl_mt == NULL )
    //                XZST_03EcritureTrace( XZSTC_WARNING,
    //                    "main: callback arme sur message numero %d global",
    //                    pm_TabCB[vl_nCB].NumMsg );
    //              if (!TipcSrvProcessCbCreate( vl_mt,
    //                  pm_TabCB[vl_nCB].FoncCB,
    //                  (T_CB_ARG*)NULL) )
    //              {
    //                XZST_03EcritureTrace( XZSTC_FATAL,
    //                    "main de gmot: pb au TipcSrvProcessCbCreate");
    //              }
    //            }
    //            if (!XDG_JoindreDG( XDG_IHM_ADM))
    //              XZST_03EcritureTrace( XZSTC_FATAL, "main de gmot: impossible de s'abonner a XDG_IHM_ADM" );
    //    }
    
  }
  else
  {
    /* Init du titre de la fenetre des synoptiques */
    if (vg_tigmot_tisup == XDC_FAUX)
      strcpy(vg_titre, "SYNOPTIQUES D'EXPLOITATION");
    else
      strcpy(vg_titre, "SYNOPTIQUES D'ADMINISTRATION");

    /* Initialisation du module MSVU */
    /* Initialisation du module MSOP */
    /* Initialisation du module MSRA */
    if (vg_tigmot_tisup == XDC_FAUX)
    {
      if (MSRA_init_rafraichissement_synopt(argc, argv) == XDC_NOK)
      {
        XZST_03EcritureTrace(XZSTC_WARNING, "Probleme lors de l'initialisation du module MSRA");
        return XDC_NOK;
      }
    }
  }

  if (argc == 3)
  {
    if (!strcmp(argv[2], "play"))
    {
      vl_fd = fopen("/produits/migrazur/appliSD/exec/tisbi.log", "r");
      if (vl_fd != NULL)
      {
        while (fgets(vl_Lig, sizeof(vl_Lig), vl_fd) != NULL)
        {
          send_message(vl_Lig, 0, "");
        }
        fclose(vl_fd);
      }
    }
  }
  /* Initialisation du module MSCO */

  if (strncmp(argv[0], "tisys", 5)) /* Pas d'abonnement serveur de synoptique si la tache est tisys?.x : DEM780 */
  {
    /* Abonnement au datagroup du serveur de synoptique */
    XZST_03EcritureTrace(XZSTC_DEBUG1, "MSMA_init: Abonnement XDG_ISYN_SERVEUR ");
    /*TODO*/
    XDG_EncodeDG2(vl_Datagroup, XDG_ISYN_SERVEUR, vm_NomMachine);
    if (!XDG_JoindreDG(vl_Datagroup))
    {
      /* Ecriture Trace */
      XZST_03EcritureTrace(XZSTC_WARNING,
                           "Abonnement sur %s non effectuee. \n ", vl_Datagroup);
      /*return XDC_NOK;*/
    }
  }

  if (!vg_pas_tisse)
  {
    XDG_EncodeDG2(vl_Datagroup, XDG_IETAT_INIEQT, vm_NomSite);
    XDG_JoindreDG(vl_Datagroup);
  }
  if (vg_tigmot_tisup == XDC_FAUX)
  {
    if ((vl_ValRet = XZAG02_Connexion_Base(vm_NomSite)) != XDC_OK)
    {
      XZST_03EcritureTrace(XZSTC_WARNING, "Impossible de se connecter a la base erreur : %d.", vl_ValRet);
      return (XDC_NOK);
    }
    if (XZAO790_Liste_Eqt_SAGA((XDY_FonctionInt)isbi_ajouter_eqt, XDC_BASE_CFG, &vl_cr) != XDC_OK)
    {
      XZST_03EcritureTrace(XZSTC_WARNING, "MSMA_init : pb appel a XZAO730_Ajouter_Eqt_SAGA");
      return (XDC_NOK);
    }

    XZAG03_Deconnexion_Base();

    /* Initialisation de l'IHm avec la Base de Donnee des Evt si la machine n'est pas un PC simplifie */
    if (vm_TypeMachine != XZSCC_TYPEM_PCS)
      if (XZAH11_Init_IHM() == XDC_NOK)
      {
        XZST_03EcritureTrace(XZSTC_WARNING, "Probleme lors de l'initialisation de l'IHM avec la Base de donnees");
      }

    /* Initialisation de l'IHm avec la Base de Donnee des Eqt*/
    XDG_EncodeDG2(vl_Datagroup, XDG_IETAT_INIEQT, vm_NomMachine);
    if (xzat27_Init_Etats_EqtsIhm(XDC_EQT_NAV, 0xFF, vl_Datagroup) == XDC_NOK)
    {
      XZST_03EcritureTrace(XZSTC_WARNING, "Probleme lors de l'initialisation des NAV");
    }

    if (xzat27_Init_Etats_EqtsIhm(XDC_EQT_PMV, 0xFF, vl_Datagroup) == XDC_NOK)
    {
      XZST_03EcritureTrace(XZSTC_WARNING, "Probleme lors de l'initialisation des PMV ");
    }

    if (xzat27_Init_Etats_EqtsIhm(XDC_EQT_CAM, 0xFF, vl_Datagroup) == XDC_NOK)
    {
      XZST_03EcritureTrace(XZSTC_WARNING, "Probleme lors de l'initialisation des CAM");
    }
    if (xzat27_Init_Etats_EqtsIhm(XDC_EQT_RAD, 0xFF, vl_Datagroup) == XDC_NOK)
    {
      XZST_03EcritureTrace(XZSTC_WARNING, "Probleme lors de l'initialisation des RADT");
    }

    if (xzat27_Init_Etats_EqtsIhm(XDC_EQT_PAL, 0xFF, vl_Datagroup) == XDC_NOK)
    {
      XZST_03EcritureTrace(XZSTC_WARNING, "Probleme lors de l'initialisation des panneaux d'alerte");
    }

    if (xzat27_Init_Etats_EqtsIhm(XDC_EQT_FAC, 0xFF, vl_Datagroup) == XDC_NOK)
    {
      XZST_03EcritureTrace(XZSTC_WARNING, "Probleme lors de l'initialisation des Fermetures d'acces");
    }

    if (xzat27_Init_Etats_EqtsIhm(XDC_EQT_FAU, 0xFF, vl_Datagroup) == XDC_NOK)
    {
      XZST_03EcritureTrace(XZSTC_WARNING, "Probleme lors de l'initialisation des Fermetures d'acces");
    }

    /* si la machine n'est pas un PC simplifie alors on demande les autres EQT ( ana, pau , tube ...) */
    if (vm_TypeMachine != XZSCC_TYPEM_PCS)
    {
      if (xzat27_Init_Etats_EqtsIhm(XDC_EQT_ANA, 0xFF, vl_Datagroup) == XDC_NOK)
      {
        XZST_03EcritureTrace(XZSTC_WARNING, "Probleme lors de l'initialisation des ANA");
      }

      if (xzat27_Init_Etats_EqtsIhm(XDC_EQT_PAU, 0xFF, vl_Datagroup) == XDC_NOK)
      {
        XZST_03EcritureTrace(XZSTC_WARNING, "Probleme lors de l'initialisation des PAU");
      }

      if (xzat27_Init_Etats_EqtsIhm(XDC_EQT_TUB, 0xFF, vl_Datagroup) == XDC_NOK)
      {
        XZST_03EcritureTrace(XZSTC_WARNING, "Probleme lors de l'initialisation des TUB");
      }

      if (xzat27_Init_Etats_EqtsIhm(XDC_EQT_ECH, 0xFF, vl_Datagroup) == XDC_NOK)
      {
        XZST_03EcritureTrace(XZSTC_WARNING, "Probleme lors de l'initialisation des PAN");
      }

      if (xzat27_Init_Etats_EqtsIhm(XDC_EQT_PAN, 0xFF, vl_Datagroup) == XDC_NOK)
      {
        XZST_03EcritureTrace(XZSTC_WARNING, "Probleme lors de l'initialisation des PAN");
      }
    }

    if (xzat27_Init_Etats_EqtsIhm(XDC_EQT_PMVA, 0xFF, vl_Datagroup) == XDC_NOK)
    {
      XZST_03EcritureTrace(XZSTC_WARNING, "Probleme lors de l'initialisation des Fermetures d'acces");
    }

    if (xzat27_Init_Etats_EqtsIhm(XDC_EQT_BAF, 0xFF, vl_Datagroup) == XDC_NOK)
    {
      XZST_03EcritureTrace(XZSTC_WARNING, "Probleme lors de l'initialisation des Fermetures d'acces");
    }

    if (xzat27_Init_Etats_EqtsIhm(XDC_EQT_BAD, 0xFF, vl_Datagroup) == XDC_NOK)
    {
      XZST_03EcritureTrace(XZSTC_WARNING, "Probleme lors de l'initialisation des Fermetures d'acces");
    }
    if (xzat27_Init_Etats_EqtsIhm(XDC_EQT_PRV, 0xFF, vl_Datagroup) == XDC_NOK)
    {
      XZST_03EcritureTrace(XZSTC_WARNING, "Probleme lors de l'initialisation des Fermetures d'acces");
    }
  }
  else
  { /*tigmot*/
    XZST_03EcritureTrace(XZSTC_FONCTION, "init tigmot tisup");

    XZSE_20InitSynoptiqueAG();

    XZSA_18ListeSite(GMInitVarSSys, &vl_dummy, pl_dummy);
    XZSA_01ListeMachine(GMInitVarTac, &vl_dummy, pl_dummy);
    /* Demande d'initialisation de la synthese du reseau  */
    MajAdm_Mach_Synt(XDC_ACTIF_CI, XZSIC_ETAT_VEILLE);
    MajAdm_Mach_Synt(XDC_ACTIF_DN, XZSIC_ETAT_VEILLE);
    MajAdm_Mach_Synt(XDC_ACTIF_DM, XZSIC_ETAT_VEILLE);
    MajAdm_Res_Synt(XDC_ACTIF_CI, XZSIC_ETAT_VEILLE);
    MajAdm_Res_Synt(XDC_ACTIF_DN, XZSIC_ETAT_VEILLE);
    MajAdm_Res_Synt(XDC_ACTIF_DM, XZSIC_ETAT_VEILLE);

    /*MSRA_Fin();*/
  }

  XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : MSMA_init");
  return XDC_OK;
}

void MSMA_init_cb_tseta()
{
  int vl_nCB;
  T_IPC_MT vl_mt;

  if (vg_tigmot_tisup == XDC_VRAI)
  { /* tigmot */
    /*Abonnement aux messages tseta*/
    for (vl_nCB = 0; vl_nCB < (sizeof(pm_TabCB) / sizeof(pm_TabCB[0])); vl_nCB++)
    {
      vl_mt = XDM_IdentMsg(pm_TabCB[vl_nCB].NumMsg);
      if (vl_mt == NULL)
        XZST_03EcritureTrace(XZSTC_WARNING,
                             "main: callback arme sur message numero %d global",
                             pm_TabCB[vl_nCB].NumMsg);
      if (!TipcSrvProcessCbCreate(vl_mt,
                                  pm_TabCB[vl_nCB].FoncCB,
                                  (T_CB_ARG *)NULL))
      {
        XZST_03EcritureTrace(XZSTC_FATAL,
                             "main de gmot: pb au TipcSrvProcessCbCreate");
      }
    }
    if (!XDG_JoindreDG(XDG_IHM_ADM))
      XZST_03EcritureTrace(XZSTC_FATAL, "main de gmot: impossible de s'abonner a XDG_IHM_ADM");
  }
}
/*------------------------------------------------------
 * SERVICE RENDU :
 *  Fonction arret de la tache TISYN
------------------------------------------------------
 * SEQUENCE D'APPEL :
 */

void MSMA_arret()

/*
 * ARGUMENTS EN ENTREE :
 *   aucun
 *
 *
 * ARGUMENTS EN SORTIE :
 *   aucun
 *
 *
 * CODE RETOUR :
 *   exit(0)
 *
 * CONDITION D'UTILISATION
 *
 *
 * FONCTION
 *   Arret de tous les traitement de la tache TISYN
 *
------------------------------------------------------*/
{

  XZST_03EcritureTrace(XZSTC_FONCTION, "IN:MSMA_arret");

  XZST_03EcritureTrace(XZSTC_WARNING, "MSMA_arret: Demande d'arret de la tache tisbi.x");

  XZST_03EcritureTrace(XZSTC_INFO, "Fermeture des sockets");

  /*socket ToFlash*/
  XZST_03EcritureTrace(XZSTC_INFO, "Socket ToFlash");
  if (vg_cli_sock_ToFlash != -1)
    close(vg_cli_sock_ToFlash);
  if (vg_serv_sock_ToFlash != -1)
  {
    XZST_03EcritureTrace(XZSTC_INFO, "...shutdown Serveur de Socket ToFlash");
    shutdown(vg_serv_sock_ToFlash, SHUT_RDWR);
    close(vg_serv_sock_ToFlash);
  }

  /*socket FromFlash*/
  XZST_03EcritureTrace(XZSTC_INFO, "Socket FromFlash");
  if (vg_cli_sock_FromFlash != -1)
    close(vg_cli_sock_FromFlash);
  if (vg_serv_sock_FromFlash != -1)
  {
    XZST_03EcritureTrace(XZSTC_INFO, "...shutdown Serveur de Socket FromFlash");
    shutdown(vg_serv_sock_FromFlash, SHUT_RDWR);
    close(vg_serv_sock_FromFlash);
  }

  /*socket Courbes*/
  XZST_03EcritureTrace(XZSTC_INFO, "Socket Courbes");
  if (vg_cli_sock_Courbes != -1)
    close(vg_cli_sock_Courbes);
  if (vg_serv_sock_Courbes != -1)
  {
    XZST_03EcritureTrace(XZSTC_INFO, "...shutdown Serveur de Socket Courbes");
    shutdown(vg_serv_sock_Courbes, SHUT_RDWR);
    close(vg_serv_sock_Courbes);
  }

  exit(0);
}

/*------------------------------------------------------
 * SERVICE RENDU :
 *  Fonction deserveur de syn
------------------------------------------------------
 * SEQUENCE D'APPEL :
 */

void MSMA_Serveur_syn(
    T_IPC_CONN va_Cnx,
    T_IPC_CONN_PROCESS_CB_DATA pa_Data,
    T_CB_ARG pa_ARG)

/*
 * ARGUMENTS EN ENTREE :
 *   aucun
 *
 *
 * ARGUMENTS EN SORTIE :
 *   aucun
 *
 *
 * CODE RETOUR :
 *   XDC_OK
 *   XDC_NOK
 *
 * CONDITION D'UTILISATION
 *
 *
 * FONCTION
 *   Reception le message pour affichage d'une vue sur serveur de synoptique
 *
------------------------------------------------------*/
{

  T_STR *pl_tab_radt,
      *pl_tab_aires,
      *pl_tab_dai;

  T_INT4 vl_nb_radt = 0;
  T_INT4 vl_nb_dai = 0;
  T_INT4 vl_nb_aires = 0;

  float vl_fradt_visi, vl_fpmv_visi, vl_fcamera_visi, vl_fdai_visi, vl_fpau_visi,
      vl_freseau_pi_visi, vl_freseau_eau_visi, vl_freseau_gaz_visi, vl_freseau_elect_visi, vl_freseau_autre_visi,
      vl_flit_visi, vl_fpat_visi, vl_fnav_visi, vl_fltv_visi, vl_ftravaux_visi, vl_fstrada_visi,
      vl_fpmva_visi, vl_fbaf_visi, vl_fbad_visi, vl_fcanton_visi;
  int vl_radt_visi, vl_pmv_visi, vl_camera_visi, vl_dai_visi, vl_pau_visi,
      vl_reseau_pi_visi, vl_reseau_eau_visi, vl_reseau_gaz_visi, vl_reseau_elect_visi, vl_reseau_autre_visi,
      vl_lit_visi, vl_pat_visi, vl_nav_visi, vl_ltv_visi, vl_travaux_visi, vl_strada_visi,
      vl_pmva_visi, vl_baf_visi, vl_bad_visi, vl_canton_visi;

  T_INT2 vl_vue_ech = 0;
  T_INT2 vl_vue_tunnel = 0;
  T_STR pl_nom_vue;
  T_STR pl_nom_ech;
  T_STR pl_nom_tunnel;
  T_STR pl_str_visi;
  char vl_nom_tunnel[256];
  char vl_nom_echangeur[256];
  T_INT2 vl_existe_ign;
  T_INT2 vl_pt_x, vl_pt_y, vl_fenetre_x, vl_fenetre_y, vl_plan;
  T_STR pl_nom_zoom;
  T_INT2 *pl_prespostit;
  T_STR *pl_textpostit;
  T_INT2 *pl_visipostit;
  T_INT4 vl_nb_pres = 0, vl_nb_text = 0, vl_nb_visi = 0;

  int i;

  XZST_03EcritureTrace(XZSTC_FONCTION, "IN  : MSMA_Serveur_syn ");

  XZST_03EcritureTrace(XZSTC_FONCTION, "OUT  : MSMA_Serveur_syn ");
  return;
}

/*------------------------------------------------------
 * SERVICE RENDU :
 *  Fonction deserveur de syn pour les courbes
------------------------------------------------------
 * SEQUENCE D'APPEL :
 */

void MSMA_Serveur_synCV(
    T_IPC_CONN va_Cnx,
    T_IPC_CONN_PROCESS_CB_DATA pa_Data,
    T_CB_ARG pa_ARG)

/*
 * ARGUMENTS EN ENTREE :
 *   aucun
 *
 *
 * ARGUMENTS EN SORTIE :
 *   aucun
 *
 *
 * CODE RETOUR :
 *   XDC_OK
 *   XDC_NOK
 *
 * CONDITION D'UTILISATION
 *
 *
 * FONCTION
 *   Reception le message pour affichage d'une vue sur serveur de synoptique
 *
------------------------------------------------------*/
{

  tm_varcaract vl_varcaract[CM_MAX_COURBES_FT];
#ifndef _TIBCO_EMS
  T_INT4 vl_nb_courbes;
  T_INT2 vl_no_station;
  T_INT2 vl_tempsreel;
  T_REAL8 vl_horodeb;
  T_INT2 vl_fenetre;
  T_INT2 vl_echelle;
  T_INT2 vl_nb_graph = 0;
  T_INT2 *pl_station;
  T_STR *pl_nom_station;
  T_STR pl_libbouch;
  T_STR pl_libevt;
  T_INT2 *pl_mesure;
  T_INT2 *pl_voie;
  T_INT2 *pl_sens;
  T_REAL8 vl_horodebbouch = 0;
  T_REAL8 vl_horofinbouch = 0;
  T_REAL8 vl_horodebevt = 0;
  T_REAL8 vl_horofinevt = 0;
  T_INT2 vl_periode;
#else
  XDY_Entier vl_nb_courbes;
  XDY_Entier vl_no_station;
  XDY_Mot vl_tempsreel;
  XDY_Horodate vl_horodeb;
  XDY_Mot vl_fenetre;
  XDY_Mot vl_echelle;
  XDY_Mot vl_nb_graph = 0;
  XDY_Mot *pl_station;
  XDY_Nom pl_nom_station[CM_MAX_COURBES_FT];
  XDY_Phrase pl_libbouch;
  XDY_Phrase pl_libevt;
  XDY_Mot *pl_mesure;
  XDY_Mot *pl_voie;
  XDY_Mot *pl_sens;
  XDY_Horodate vl_horodebbouch = 0;
  XDY_Horodate vl_horofinbouch = 0;
  XDY_Horodate vl_horodebevt = 0;
  XDY_Horodate vl_horofinevt = 0;
  XDY_Mot vl_periode;
#endif
  XZSMT_Horodate vl_Horodate;
  XZSMT_Horodate vl_Horodebe;
  XZSMT_Horodate vl_Horofine;
  XZSMT_Horodate vl_Horodebb;
  XZSMT_Horodate vl_Horofinb;

  int i;

  XZST_03EcritureTrace(XZSTC_FONCTION, "IN  : MSMA_Serveur_synCV ");

  /* Recuperation des donnees du message */
  if (TipcMsgRead(pa_Data->msg, T_IPC_FT_INT2, &vl_nb_graph, T_IPC_FT_INT2, &vl_tempsreel, T_IPC_FT_REAL8, &vl_horodeb, T_IPC_FT_INT2, &vl_fenetre, T_IPC_FT_INT2, &vl_echelle, T_IPC_FT_INT2_ARRAY, &pl_station, &vl_nb_courbes, T_IPC_FT_STR_ARRAY, &pl_nom_station, &vl_nb_courbes, T_IPC_FT_INT2_ARRAY, &pl_mesure, &vl_nb_courbes, T_IPC_FT_INT2_ARRAY, &pl_voie, &vl_nb_courbes, T_IPC_FT_INT2_ARRAY, &pl_sens, &vl_nb_courbes, T_IPC_FT_STR, &pl_libbouch, T_IPC_FT_REAL8, &vl_horodebbouch, T_IPC_FT_REAL8, &vl_horofinbouch, T_IPC_FT_STR, &pl_libevt, T_IPC_FT_REAL8, &vl_horodebevt, T_IPC_FT_REAL8, &vl_horofinevt, T_IPC_FT_INT2, &vl_periode, NULL))
  {
    if (vl_nb_graph != 0)
    {
      if (vl_nb_courbes != 0)
      {
        for (i = 0; i < vl_nb_courbes; i++)
        {
          vl_varcaract[i].no_station = (XDY_Octet)pl_station[i];
          strcpy(vl_varcaract[i].nom_station, pl_nom_station[i]);
          vl_varcaract[i].mesure = (XDY_Octet)pl_mesure[i];
          vl_varcaract[i].voie = (XDY_Voie)pl_voie[i];
          vl_varcaract[i].sens = (XDY_Sens)pl_sens[i];
        }

        XZSM_11ConversionHorodate((XDY_Horodate)vl_horodeb, &vl_Horodate);

        /*
           if ( MSCO01_init_courbe_temps(
                                   vl_varcaract,
                                   vl_nb_courbes ,
                                   vl_tempsreel ,
                                   vl_Horodate ,
                                   (XDY_Octet) vl_fenetre ,
                                   (XDY_Octet) vl_echelle,
                                   (XDY_Octet) vl_periode )!= XDC_OK )
              {
              XZST_03EcritureTrace(XZSTC_WARNING,"Impossible de creer la courbe");
              return;
              }
         */
      }
      if (vl_horodebbouch != 0.0)
      {
        XZSM_11ConversionHorodate(vl_horodebbouch, &vl_Horodebb);
        XZSM_11ConversionHorodate(vl_horofinbouch, &vl_Horofinb);
        /*
               if ( MSCO_Creer_barre_bouchon(vl_Horodebb,vl_Horofinb,(XDY_Octet)vl_fenetre)== XDC_NOK )
                 {
                  XZST_03EcritureTrace(   XZSTC_WARNING, "Impossible de creer la barre bouchon"  );
                  return ;
                  }
               if ( MSCO_Afficher_Lib_bouchon(pl_libbouch,(XDY_Octet)vl_fenetre)== XDC_NOK )
                  {
                  XZST_03EcritureTrace(   XZSTC_WARNING, "Impossible d'afficher le libelle du bouchon"  );
                  return ;
                  }
         */
      }
      if (vl_horodebevt != 0.0)
      {
        XZSM_11ConversionHorodate(vl_horodebevt, &vl_Horodebe);
        XZSM_11ConversionHorodate(vl_horofinevt, &vl_Horofine);
        /*
               if ( MSCO_Creer_barre_evt(vl_Horodebe,vl_Horofine,(XDY_Octet)vl_fenetre)== XDC_NOK )
                 {
                  XZST_03EcritureTrace(   XZSTC_WARNING, "Impossible de creer la barre bouchon"  );
                  return ;
                  }
               if ( MSCO_Afficher_Lib_evt(pl_libevt,(XDY_Octet)vl_fenetre)== XDC_NOK )
                  {
                  XZST_03EcritureTrace(   XZSTC_WARNING, "Impossible d'afficher le libelle du bouchon"  );
                  return ;
                  }
         */
      }
    }
  }
  else
  {
    XZST_03EcritureTrace(XZSTC_WARNING, "MSMA_Serveur_synC: Impossible de recuperer le message");
    return;
  }

  XZST_03EcritureTrace(XZSTC_FONCTION, "OUT  : MSMA_Serveur_synCV ");
  return;
}

/*------------------------------------------------------
 * SERVICE RENDU :
 *  Traitement des Evt souris et clavier pour DV
------------------------------------------------------
 * SEQUENCE D'APPEL :
 */

MSMA_Reception_Evt_DV(
    pa_quit)

    XDY_Booleen *pa_quit;

/*
 * ARGUMENTS EN ENTREE :
 *   aucun
 *
 *
 * ARGUMENTS EN SORTIE :
 *   pa_quit : pointeur sur bool demande d'abondon
 *
 *
 * CODE RETOUR :
 *   XDC_OK
 *   XDC_NOK
 *
 * CONDITION D'UTILISATION
 *
 *
 * FONCTION
 *   Reception le message pour affichage d'une vue sur serveur de synoptique
 *
------------------------------------------------------*/
{
}

/* Pour satisfaire le link de isbi , on fournit la fct (vide) de icfg */
int MTCG06_Construction_Ign() {}

void MSMA_construit_region()
{
  char vl_str[200];
  char vl_type[50];
  int dummy = 0;
  FILE *pl_fd = NULL;
  char vl_Lig[255];
  XDY_Entier vl_Resultat_out = 0;
  int vl_Xstart, vl_Ystart, vl_nb_lignes, vl_decalageX, vl_decalageY;
  int vl_retcode;

  if (!vg_pas_tisse)
    return;

  /*decodage du fichier des parametres a faire*/
  sprintf(vl_str, "/produits/migrazur/appliSD/fichiers/syn/supervisionCS5/share/tisup.cfg");
  pl_fd = fopen(vl_str, "r");
  if (pl_fd == NULL)
  {
    XZST_03EcritureTrace(XZSTC_WARNING, "pb ouverture fichier parametrage %s", vl_str);
    return;
  }
  while ((vl_retcode = XZAO000_LireLigneFich(pl_fd, vl_Lig, sizeof(vl_Lig), &vl_Resultat_out)) != XDC_NOK)
  {
    if (vl_Resultat_out != XZAO_EOF)
    {
      if (vl_Lig[0] != '#')
      {
        sscanf(vl_Lig, "%[^#]#%d#%d#%d#%d#%d",
               vl_type, &vl_Xstart, &vl_Ystart, &vl_nb_lignes, &vl_decalageX, &vl_decalageY);
        if (!strcmp(vl_type, "regions"))
        {
          vm_posX_start_regions = vl_Xstart;
          vm_posY_start_regions = vl_Ystart;
          vm_nb_par_ligne_regions = vl_nb_lignes;
          vm_decalage_X_regions = vl_decalageX;
          vm_decalage_Y_regions = vl_decalageY;
        }
        else if (!strcmp(vl_type, "machines"))
        {
          vm_posX_start_machines = vl_Xstart;
          vm_posY_start_machines = vl_Ystart;
          vm_nb_par_ligne_machines = vl_nb_lignes;
          vm_decalage_X_machines = vl_decalageX;
          vm_decalage_Y_machines = vl_decalageY;
        }
        else
        {
          vm_posX_start_taches = vl_Xstart;
          vm_posY_start_taches = vl_Ystart;
          vm_nb_par_ligne_taches = vl_nb_lignes;
          vm_decalage_X_taches = vl_decalageX;
          vm_decalage_Y_taches = vl_decalageY;
        }
      }
    }
    else
      break;
  }
  fclose(pl_fd);
  sprintf(vl_str, "%sREGION_tisup_region_object.xml", C_PATH);
  pl_fd = fopen(vl_str, "w");
  if (pl_fd == NULL)
  {
    XZST_03EcritureTrace(XZSTC_WARNING, "pb ouverture fichier region %s", vl_str);
    return;
  }

  XZST_03EcritureTrace(XZSTC_FONCTION, "avant appel a XZSA_18ListeSite");
  /* Entete xml */
  fputs("<saeObjects>", pl_fd);
  XZSA_18ListeSite(&MSMA_add_region, &dummy, pl_fd);
  fputs("</saeObjects>", pl_fd);
  fclose(pl_fd);
}

void MSMA_add_region(XZSCT_NomSite va_nom,
                     int va_nb, /*vaut 0 pour le premier*/
                     FILE *pa_fd)
{
  char tl_str[200];
  int vl_X, vl_Y;
  int vl_nb_lignes_deja_faites;
  XZST_03EcritureTrace(XZSTC_FONCTION, "MSMA_add_region : ajout %s(%d)",
                       va_nom, va_nb);

  vl_nb_lignes_deja_faites = floor(va_nb / vm_nb_par_ligne_regions);
  XZST_03EcritureTrace(XZSTC_FONCTION, "%d ", vl_nb_lignes_deja_faites);
  vl_X = vm_posX_start_regions + (va_nb % vm_nb_par_ligne_regions) * vm_decalage_X_regions;
  vl_Y = vm_posY_start_regions + vl_nb_lignes_deja_faites * vm_decalage_Y_regions;

  sprintf(tl_str, "<object id=\"%s\" type=\"REGION\" visible=\"true\">",
          va_nom);
  fputs(tl_str, pa_fd);
  sprintf(tl_str, "<nom>%s</nom>\n", va_nom);
  fputs(tl_str, pa_fd);
  sprintf(tl_str, "<numero>%d</numero>\n", va_nb + 1);
  fputs(tl_str, pa_fd);
  fputs("<location>", pa_fd);
  sprintf(tl_str, "<posX>%d</posX>\n", vl_X);
  fputs(tl_str, pa_fd);
  sprintf(tl_str, "<posY>%d</posY>\n", vl_Y);
  fputs(tl_str, pa_fd);
  fputs("</location>", pa_fd);
  fputs("<typeVue>generale</typeVue>\n", pa_fd);
  fputs("</object>", pa_fd);

  /*construction de la vue machines par region*/
  strcpy(vm_region, va_nom);
  MSMA_construit_machines_region(va_nom);
}

void MSMA_construit_machines_region(char *va_region)
{
  char vl_str[200];
  int vl_nb = 0;
  FILE *pl_fd = NULL;

  sprintf(vl_str,
          "%sMACHINE_%s_machine_object.xml",
          C_PATH,
          va_region);
  pl_fd = fopen(vl_str, "w");
  if (pl_fd == NULL)
  {
    XZST_03EcritureTrace(XZSTC_WARNING, "pb ouverture fichier region %s", vl_str);
    return;
  }
  /* Ajout entete */
  fputs("<saeObjects>", pl_fd);
  vm_nb_mac = 0;
  XZSA_19ListeMachineSite(va_region, MSMA_add_machines_region, &vl_nb, pl_fd);
  fputs("</saeObjects>", pl_fd);
  fclose(pl_fd);
}
void MSMA_add_machines_region(XZSCT_NomMachine va_machine,
                              int va_nb,
                              FILE *pa_fd)
{
  char tl_str[200];
  int vl_X, vl_Y;
  int vl_nb_lignes_deja_faites;
  int vl_type_machine;
  XZST_03EcritureTrace(XZSTC_FONCTION, "MSMA_add_machines_region : ajout %s(%d)",
                       va_machine, va_nb);

  vl_nb_lignes_deja_faites = floor(vm_nb_mac / vm_nb_par_ligne_machines);

  vl_X = vm_posX_start_machines + (vm_nb_mac % vm_nb_par_ligne_machines) * vm_decalage_X_machines;
  vl_Y = vm_posY_start_machines + vl_nb_lignes_deja_faites * vm_decalage_Y_machines;

  /*<object id="1" type="MACHINE" visible="true">
      <nom>POCI1</nom>
  <numero> ?</numero>
      <location>
      <posX>659.15</posX>
    <posY>-105.8</posY>
        <typeVue>region</typeVue>
  </object>
   */
  XZSA_17TypeMachine(va_machine, &vl_type_machine);
  sprintf(tl_str, "<object id=\"%s\" type=\"MACHINE\" visible=\"true\">\n",
          va_machine);
  fputs(tl_str, pa_fd);
  sprintf(tl_str, "<nom>%s</nom>\n", va_machine);
  fputs(tl_str, pa_fd);
  sprintf(tl_str, "<numero>%d</numero>\n", vm_nb_mac + 1);
  fputs(tl_str, pa_fd);
  fputs("<location>", pa_fd);
  sprintf(tl_str, "<posX>%d</posX>\n", vl_X);
  fputs(tl_str, pa_fd);
  sprintf(tl_str, "<posY>%d</posY>\n", vl_Y);
  fputs(tl_str, pa_fd);
  fputs("</location>", pa_fd);
  sprintf(tl_str, "<typeMachine>%d</typeMachine>\n", vl_type_machine);
  fputs(tl_str, pa_fd);
  fputs("<typeVue>region</typeVue>\n", pa_fd);
  fputs("</object>\n", pa_fd);
  vm_nb_mac++;
  MSMA_construit_taches_machine(va_machine);
}
void MSMA_construit_taches_machine(char *va_machine)
{
  char vl_str[200];
  int vl_nb = 0;
  FILE *pl_fd = NULL;

  sprintf(vl_str,
          "%sTACHE_%s_tache_object.xml",
          C_PATH,
          va_machine);
  pl_fd = fopen(vl_str, "w");
  if (pl_fd == NULL)
  {
    XZST_03EcritureTrace(XZSTC_WARNING, "pb ouverture fichier %s", vl_str);
    return;
  }
  strcpy(vm_machine_courante, va_machine);
  /* ajout entete */
  fputs("<saeObjects>", pl_fd);
  vm_nb_tac = 0;
  XZSA_03ListeTacheMachine(va_machine, MSMA_add_taches_machine, &vl_nb, pl_fd);
  fputs("</saeObjects>", pl_fd);
  fclose(pl_fd);
}

void MSMA_add_taches_machine(XZSCT_NomTache va_tac,
                             int va_nb,
                             FILE *pa_fd)
{
  char tl_str[200];
  int vl_X, vl_Y;
  int vl_nb_lignes_deja_faites;
  int vl_nb = vm_nb_tac + 1;
  XZST_03EcritureTrace(XZSTC_FONCTION, "MSMA_add_taches_machine : ajout %s(%d)",
                       va_tac, vm_nb_tac);
  vl_nb_lignes_deja_faites = floor(vm_nb_tac / vm_nb_par_ligne_taches);
  XZST_03EcritureTrace(XZSTC_FONCTION, "vm_nb_par_ligne_taches(%d) // vl_nb_lignes_deja_faites %d",
                       vm_nb_par_ligne_taches, vl_nb_lignes_deja_faites);
  vl_X = vm_posX_start_taches + (vm_nb_tac % vm_nb_par_ligne_taches) * vm_decalage_X_taches;
  vl_Y = vm_posY_start_taches + vl_nb_lignes_deja_faites * vm_decalage_Y_taches;

  /*<object id="1" type="TACHE" visible="true">
    <nom>tisbi.x</nom>
    <numero> ?</numero>
    <nomMachine>POVD1</nomMachine>
    <location>
      <posX>659.15</posX>
      <posY>-105.8</posY>
      <typeVue>machine</typeVue>
  </object>
   */

  sprintf(tl_str, "<object id=\"%s_%s\" type=\"TACHE\" visible=\"true\">\n",
          vm_machine_courante,
          va_tac);
  fputs(tl_str, pa_fd);
  /*	sprintf(tl_str,"<nom>%s_%s</nom>\n",vm_machine_courante,va_tac);
   */
  sprintf(tl_str, "<nom>%s</nom>\n", va_tac);
  fputs(tl_str, pa_fd);
  sprintf(tl_str, "<numero>%d</numero>\n", va_nb + 1);
  fputs(tl_str, pa_fd);
  sprintf(tl_str, "<nomMachine>%s</nomMachine>\n", vm_machine_courante);
  fputs(tl_str, pa_fd);
  fputs("<location>\n", pa_fd);
  sprintf(tl_str, "<posX>%d</posX>\n", vl_X);
  fputs(tl_str, pa_fd);
  sprintf(tl_str, "<posY>%d</posY>\n", vl_Y);
  fputs(tl_str, pa_fd);
  fputs("</location>", pa_fd);
  fputs("<typeVue>machine</typeVue>\n", pa_fd);
  fputs("</object>\n", pa_fd);
  vm_nb_tac++;
}
