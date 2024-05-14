/* Fichier : $Id: isbi_dir.c,v 1.8 2021/01/29 09:08:28 pc2dpdy Exp $        $Revision: 1.8 $        $Date: 2021/01/29 09:08:28 $
------------------------------------------------------
 * STERIA *  PROJET MIGRAZUR
------------------------------------------------------
 * SOUS-SYSTEME  GESIHM
------------------------------------------------------
 * MODULE MSMA * FICHIER isbi_dir.c
------------------------------------------------------
 * DESCRIPTION DU MODULE :
 *
 * Module directeur de l'appli isbi
 *
------------------------------------------------------
 * HISTORIQUE :
 *
 * Niepceron	24 Nov 1994	: Creation
 * JMG		15/02/13 : ajout PROJETER
 * JMG		206/17 : tigmot tisup 1.3
 * JPL	19/02/18 : Migration Linux 64 bits (DEM 1274) : type args. fonctions; modification champs de donnees socket obsoletes  1.5
 * RGR 23/04/19 : DEM1331 - Mecanisme Keep alive 1.6
* JMG 22/05/20 : EMMS 1.7:
* ABE 26/01/20 : Ajout gestion fichier de config TOPS, pour IP:DISPLAY.PORT en param du synoptique DEM-SAE228 V1.8
* JMG 10/06/21 : brique API tisse 1.9
------------------------------------------------------*/

/* fichiers inclus */
#include <xzsem.h>
#include <signal.h>
#include "isbi_dir.h"
#include "isbi_sma.h"
#include "isbi_sop.h"
#include "isbi_sra_tri.h"
#include "xzis.h"

/* definitions de constantes */

#define CM_ATTENTE 0.0
#define ISBI_COM_BUFFER_LEN 1024

#define ISBI_CMD_CLIC_MOBILE 1
#define ISBI_CMD_CLIC_EQUIP 2
#define ISBI_CMD_CLIC_OBJET 3
#define ISBI_CMD_CLIC_BOUTON 4
#define ISBI_CMD_ARRET_COURBE 5
#define ISBI_CMD_PROJETER_VUE 6 /*RETRO*/
#define ISBI_CMD_PROJETER_MASQUE 7
#define ISBI_CMD_TIGMOT 8

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

extern int vm_sockfd, vm_ElfFd; /* Sockets pour RPC venant d'applix */
extern XDY_Booleen vm_cx;
extern XZSCT_NomMachine vm_NomMachine;
extern XZSCT_NomMachine vm_NomMachineSS;

int vg_serv_sock_ToFlash = -1;
int vg_cli_sock_ToFlash = -1;

int vg_serv_sock_FromFlash = -1;
int vg_cli_sock_FromFlash = -1;

int vg_serv_sock_Courbes = -1;
int vg_cli_sock_Courbes = -1;

T_INT4 vg_sock_RT = -1;
int vg_cpte_err = 0;
XDY_Booleen vg_tigmot_tisup = XDC_FAUX;
XDY_Booleen vg_pas_tisse = XDC_VRAI;

/* declaration de fonctions internes */
void openSockets();
void closeSockets();
void arret();
int TraiterReceptionMessageFlash();
void TraiterReceptionRTServer();
int lire_config_TOPS(XDY_Texte va_param, XDY_Texte va_IP, XDY_Entier *va_display, XDY_Entier *va_port);

/* declaration de fonctions externes */
extern void MSMA_arret();

main(argc, argv) int argc;
char *argv[];

{

  static fd_set vl_fdLire;
  static fd_set vl_fdRTServer; // vg_sock_RT
  static fd_set vl_fdEcrireEvt;
  static fd_set vl_fdEcrireCourbes;

  XDY_Booleen vl_quit;

  int vl_evts;

  int maxFd, cpt_err_sock;
  struct timeval vl_timeout;
  char vl_display[15];
  char vl_str[100];

  XDY_Texte va_IP, vl_commande;
  XDY_Entier va_display, va_port;

  /*A
   *  initialisation semaphore
   */
  sem_init(&vg_semaphore, 0, 1);

  signal(SIGINT, MSMA_arret);

  vg_tigmot_tisup = XDC_FAUX;
  if (!strcmp(argv[0], "tisup.x"))
    vg_tigmot_tisup = XDC_VRAI;
  vg_pas_tisse = XDC_VRAI;
  if (!strcmp(argv[0], "tisse.x"))
    vg_pas_tisse = XDC_FAUX;

  vm_cx = XDC_OUI;
  vl_quit = XDC_NON;

  XZST_08ForceNivVisuTraceLocal(XZSTC_FATAL | XZSTC_WARNING | XZSTC_DEBUG1 | XZSTC_INFO | XZSTC_FONCTION | XZSTC_INTERFACE | XZSTC_MESSAGE, XZSTC_NECRAN);

XZST_08ForceNivVisuTraceLocal(XZSTC_FATAL|XZSTC_WARNING|XZSTC_DEBUG1|XZSTC_INFO|XZSTC_FONCTION|XZSTC_INTERFACE|XZSTC_MESSAGE,XZSTC_NECRAN);
if (!strcmp(argv[0],"tisup.x"))
  		system("/produits/migrazur/appliSD/exec/synoptiques_tisup.ksh");
if ( MSMA_init(argc, argv) != XDC_OK )
{
  XZST_03EcritureTrace(   XZSTC_WARNING, "Probleme lors de l'initialisation" );
  exit(2);
}
if ( MSMA_init_reg( vm_NomSite ) != XDC_OK )
{
  XZST_03EcritureTrace(   XZSTC_WARNING, "Probleme lors de l'initialisation" );
  exit(2);
}
if (vg_pas_tisse) 
{
	if (!strcmp(argv[0],"tisbu.x")) 
	{
  		strcpy(vl_display,argv[2]);
  		sprintf(vl_str,"/produits/migrazur/appliSD/exec/synobu.ksh %s",vl_display);
  		system(vl_str);
	}
	/*else if (!strcmp(argv[0],"tisup.x"))
  		system("/produits/migrazur/appliSD/exec/synoptiques_tisup.ksh");*/
	else if (lire_config_TOPS(CO_FIC_TOPS_DISPLAY2, va_IP, &va_display, &va_port)==XDC_OK)
  	{
 		sprintf(vl_commande,"/produits/migrazur/appliSD/exec/synoptiques.ksh %s %d %d",va_IP, va_display, va_port);
   		system(vl_commande);
	}
	else
    		system("/produits/migrazur/appliSD/exec/synoptiques.ksh");
}

  XZST_08ForceNivVisuTraceLocal(XZSTC_FATAL | XZSTC_WARNING | XZSTC_DEBUG1 | XZSTC_INFO | XZSTC_FONCTION | XZSTC_INTERFACE | XZSTC_MESSAGE, XZSTC_NECRAN);

/*recuperation du fd de connexion RTworks*/
#ifndef _TIBCO_EMS
  TipcSrvGetSocket(&vg_sock_RT);
#endif

  if (vg_pas_tisse)
    openSockets();

  MSMA_init(argc, argv);
  MSMA_init_cb_tseta();

  MSRA_init_tabordobj();

  XZST_08ForceNivVisuTraceLocal(XZSTC_FATAL | XZSTC_WARNING | XZSTC_INFO | XZSTC_FONCTION | XZSTC_INTERFACE | XZSTC_MESSAGE, XZSTC_NECRAN);
  /*XZST_08ForceNivVisuTraceLocal(XZSTC_WARNING,XZSTC_NECRAN);*/

  vg_cpte_err = 0;

  if (vg_cli_sock_FromFlash > vg_sock_RT)
    maxFd = vg_cli_sock_FromFlash + 1;
  else
    maxFd = vg_sock_RT + 1;

  cpt_err_sock = 0;

  /* Boucle infinie sur evt rtworks, dataviews et rpc applix */
  while (1)
  {
    FD_ZERO(&vl_fdLire);
    FD_SET(vg_sock_RT, &vl_fdLire);
    if (vg_pas_tisse)
      FD_SET(vg_cli_sock_FromFlash, &vl_fdLire);

#ifndef _TIBCO_EMS
    vl_timeout.tv_sec = 5;
    vl_timeout.tv_usec = XZEXC_TIME_OUT_SELECT;
#else
    vl_timeout.tv_sec = 0;
    vl_timeout.tv_usec = 500;
#endif

    vl_evts = select((size_t)(maxFd),
                     (fd_set *)&vl_fdLire, (fd_set *)0, (fd_set *)0,
                     (struct timeval *)&vl_timeout);

    if (vl_evts < 0)
    {
      if (errno == EINTR)
        continue;
      else if (vl_evts == -1)
      {
        XZST_03EcritureTrace(XZSTC_WARNING, "Erreur lors de la scrutation");
        arret();
      }
    }
    if (vl_evts > 0)
    {
#ifndef _TIBCO_EMS
      if (FD_ISSET(vg_sock_RT, &vl_fdLire))
      {
        TraiterReceptionRTServer();
      }
#else
      TipcSrvMsgNext(500);
#endif

      if (vg_pas_tisse)
      {
        if (FD_ISSET(vg_cli_sock_FromFlash, &vl_fdLire))
        {
          if (TraiterReceptionMessageFlash(vg_cli_sock_FromFlash) == XDC_NOK)
          {
            cpt_err_sock++;
            sleep(1);
          }
          else
            cpt_err_sock = 0;
        }
      }
    }

    /* Sortie de la boucle */
    if (vl_quit == XDC_OUI)
      break;
    /*MSVU_faire_maj_vue();*/

    if (vg_pas_tisse)
    {
      if (cpt_err_sock > 25)
      {
        XZST_03EcritureTrace(XZSTC_FATAL, "Plus de reception sur socket Flash");
        closeSockets();
        openSockets();
        cpt_err_sock = 0;

        vm_cx = XDC_OUI;
        vl_quit = XDC_NON;

        if (MSMA_init(argc, argv) != XDC_OK)
        {
          XZST_03EcritureTrace(XZSTC_WARNING, "Probleme lors de l'initialisation");
          exit(2);
        }

        /*recuperation du fd de connexion RTworks*/
#ifndef _TIBCO_EMS
        TipcSrvGetSocket(&vg_sock_RT);
#endif
      }
    }
  }

  exit(3);

  return XDC_OK;
}

void openSockets()
{
  XDY_Nom vl_NomMachine;

  struct sockaddr_in vl_serveur;
  struct hostent *pl_serveur;

  struct sockaddr_in localaddr;
  struct sockaddr_in localaddrFromFlash;
  struct sockaddr_in localaddrCourbes;

  socklen_t localaddr_len;
  socklen_t localaddrCourbes_len;
  socklen_t localaddrFromFlash_len;

  struct sockaddr distaddr;
  struct sockaddr distaddrFromFlash;
  struct sockaddr distaddrCourbes;

  socklen_t distaddr_len;
  socklen_t distaddrFromFlash_len;
  socklen_t distaddrCourbes_len;

  localaddrCourbes_len = sizeof(localaddrCourbes);
  localaddrFromFlash_len = sizeof(localaddrFromFlash);
  localaddr_len = sizeof(localaddr);

  int indice = 1;

  XZST_03EcritureTrace(XZSTC_WARNING, "En attente connexion Flash ....");

  XZSC_07NomMachine(vl_NomMachine);
  pl_serveur = gethostbyname(vl_NomMachine);

  /*Socket ToFlash*/
  XZST_03EcritureTrace(XZSTC_INFO, "Creation Socket ToFlash");
  XZST_03EcritureTrace(XZSTC_INFO, "1");
  vg_serv_sock_ToFlash = socket(AF_INET, SOCK_STREAM, 0);

  XZST_03EcritureTrace(XZSTC_INFO, "2 : fd=%d", vg_serv_sock_ToFlash);
  if (vg_serv_sock_ToFlash == -1)
  {
    XZST_03EcritureTrace(XZSTC_WARNING, "impossible creer socket");
    arret();
  }

  indice = 1;
  setsockopt(vg_serv_sock_ToFlash, SOL_SOCKET, SO_REUSEADDR, &indice, sizeof(indice));
  bzero((char *)&localaddr, localaddr_len);

  memcpy(&localaddr.sin_addr, pl_serveur->h_addr_list[0], (unsigned)pl_serveur->h_length);

  localaddr.sin_family = AF_INET;
  if (!vg_tigmot_tisup)
    localaddr.sin_port = htons(49021);
  else
    localaddr.sin_port = htons(49221);

  if (bind(vg_serv_sock_ToFlash, (struct sockaddr *)&localaddr, localaddr_len) == -1)
  {
    XZST_03EcritureTrace(XZSTC_WARNING, "impossible attacher socket ToFlash");
    arret();
  }

  XZST_03EcritureTrace(XZSTC_INFO, "3 bind : IP:?? port=%d", (int)localaddr.sin_port); // localaddr.sin_addr);
  if (listen(vg_serv_sock_ToFlash, 1) == -1)
  {
    XZST_03EcritureTrace(XZSTC_WARNING, "impossible listen socket");
    arret();
  }
  XZST_03EcritureTrace(XZSTC_INFO, "4");

  vg_cli_sock_ToFlash = accept(vg_serv_sock_ToFlash, (struct sockaddr *)&distaddr, &distaddr_len);

  if (vg_cli_sock_ToFlash == -1)
  {
    XZST_03EcritureTrace(XZSTC_WARNING, "impossible accept socket");
    arret();
  }
  XZST_03EcritureTrace(XZSTC_INFO, "5");

  /*Ouverture Socket en Reception de Flash*/
  XZST_03EcritureTrace(XZSTC_INFO, "Creation Socket FromFlash");
  XZST_03EcritureTrace(XZSTC_INFO, "1");
  vg_serv_sock_FromFlash = socket(AF_INET, SOCK_STREAM, 0);
  XZST_03EcritureTrace(XZSTC_INFO, "2 : fd=%d", vg_serv_sock_FromFlash);
  if (vg_serv_sock_FromFlash == -1)
  {
    XZST_03EcritureTrace(XZSTC_WARNING, "impossible creer socket");
    arret();
  }

  indice = 1;
  setsockopt(vg_serv_sock_FromFlash, SOL_SOCKET, SO_REUSEADDR, &indice, sizeof(indice));
  bzero((char *)&localaddrFromFlash, localaddrFromFlash_len);

  memcpy(&localaddrFromFlash.sin_addr, pl_serveur->h_addr_list[0], (unsigned)pl_serveur->h_length);
  localaddrFromFlash.sin_family = AF_INET;
  if (!vg_tigmot_tisup)
    localaddrFromFlash.sin_port = htons(49022);
  else
    localaddrFromFlash.sin_port = htons(49222);

  if (bind(vg_serv_sock_FromFlash, (struct sockaddr *)&localaddrFromFlash, localaddrFromFlash_len) == -1)
  {
    XZST_03EcritureTrace(XZSTC_WARNING, "impossible attacher socket FromFlash");
    arret();
  }

  XZST_03EcritureTrace(XZSTC_INFO, "3 bind : IP:?? port=%d", (int)localaddrFromFlash.sin_port); // localaddrFromFlash.sin_addr);
  if (listen(vg_serv_sock_FromFlash, 1) == -1)
  {
    XZST_03EcritureTrace(XZSTC_WARNING, "impossible listen socket");
    arret();
  }
  XZST_03EcritureTrace(XZSTC_INFO, "4 listen ok");

  vg_cli_sock_FromFlash = accept(vg_serv_sock_FromFlash, (struct sockaddr *)&distaddrFromFlash, &distaddrFromFlash_len);

  if (vg_cli_sock_FromFlash == -1)
  {
    XZST_03EcritureTrace(XZSTC_WARNING, "impossible accept socket");
    arret();
  }

  fcntl(vg_cli_sock_FromFlash, F_SETFL, O_RDONLY);

  XZST_03EcritureTrace(XZSTC_WARNING, "5");

  if (vg_tigmot_tisup)
    return;

  /* Socket Ecriture pour les courbes*/

  XZST_03EcritureTrace(XZSTC_WARNING, "Creation de la Socket Courbes");

  vg_serv_sock_Courbes = socket(AF_INET, SOCK_STREAM, 0);

  XZST_03EcritureTrace(XZSTC_WARNING, "2");
  if (vg_serv_sock_Courbes == -1)
  {
    XZST_03EcritureTrace(XZSTC_WARNING, "impossible creer socket");
    arret();
  }

  indice = 1;
  setsockopt(vg_serv_sock_Courbes, SOL_SOCKET, SO_REUSEADDR, &indice, sizeof(indice));
  bzero((char *)&localaddrCourbes, localaddrCourbes_len);
  localaddrCourbes.sin_family = AF_INET;

  memcpy(&localaddrCourbes.sin_addr, pl_serveur->h_addr_list[0], (unsigned)pl_serveur->h_length);

  localaddrCourbes.sin_port = htons(49023);
  if (bind(vg_serv_sock_Courbes, (struct sockaddr *)&localaddrCourbes, localaddrCourbes_len) == -1)
  {
    XZST_03EcritureTrace(XZSTC_WARNING, "impossible attacher socket");
    arret();
  }

  if (listen(vg_serv_sock_Courbes, 1) == -1)
  {
    XZST_03EcritureTrace(XZSTC_WARNING, "impossible listen socket");
    arret();
  }

  vg_cli_sock_Courbes = accept(vg_serv_sock_Courbes, (struct sockaddr *)&distaddrCourbes, &distaddrCourbes_len);

  if (vg_cli_sock_Courbes == -1)
  {
    XZST_03EcritureTrace(XZSTC_WARNING, "impossible accept socket");
    arret();
  }
}

void closeSockets()

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
 *
------------------------------------------------------*/
{

  XZST_03EcritureTrace(XZSTC_FONCTION, "IN:closeSockets");

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
  if (!vg_tigmot_tisup)
  {
    XZST_03EcritureTrace(XZSTC_INFO, "Socket Courbes");
    if (vg_cli_sock_Courbes != -1)
      close(vg_cli_sock_Courbes);
    if (vg_serv_sock_Courbes != -1)
    {
      XZST_03EcritureTrace(XZSTC_INFO, "...shutdown Serveur de Socket Courbes");
      shutdown(vg_serv_sock_Courbes, SHUT_RDWR);
      close(vg_serv_sock_Courbes);
    }
  }
}

#ifndef _TIBCO_EMS
void TraiterReceptionRTServer()
{

  double vl_attente = CM_ATTENTE;
  int vl_code_retour, vl_code_retour_prec = TRUE;
  T_IPC_CONN vl_Cnx;
  T_IPC_CONN_PROCESS_CB_DATA pl_Data;
  T_CB_ARG pl_ARG;

  int vl_RT_err = 0;

  /* Reception des evt RTWORKS */
  vl_code_retour = TipcSrvMainLoop(vl_attente);

  /* Si Erreur RTServer  */
  if (vl_code_retour == FALSE)
  {
    vl_RT_err = TutErrNumGet();
    /* filtre des erreur RT */
    switch (vl_RT_err)
    {
    /*	si EOM alors OK */
    case T_ERR_DOESNT_EXIST:
    case T_ERR_MSG_EOM:
      XZST_03EcritureTrace(XZSTC_WARNING, "tisbi:Erreure RTserver TutErrNumGet()=%d ", vl_RT_err);
      vl_code_retour = TRUE;
      vg_cpte_err = 0;
      break;

      /*	sinon perte RT */
    default:
      XZST_03EcritureTrace(XZSTC_WARNING, "tisbi:Perte RTserver TutErrNumGet()=%d ", vl_RT_err);
      vg_cpte_err++;
      if (vg_cpte_err > XDC_EXIT_RTCLIENT)
      {
        /* Avertissement operateur avant arret des syn */
        /*XZIA_06AffMsgSyst( vm_NomMachine, "Arret et Relance des Synoptiques" );*/
        /*XZST_03EcritureTrace( XZSTC_FATAL , "Trop de perte (%d) RTserver: sortie de tisbi" ,vg_cpte_err);*/
        /*exit(0);*/
        vg_cpte_err = 0;
        XZST_03EcritureTrace(XZSTC_WARNING, "Trop de perte (%d) RTserver: reinit du cpteur a 0", vg_cpte_err);
      }
      /*sleep(1);*/
      if ((vg_cpte_err % 20) == 0)
      {
        /*sleep(5);*/
      }
    }
  }
  else
    vg_cpte_err = 0;

  /* Si on recupere la comm precedement perdue avec le rtserver
      alors on reinit les eqt */

  if ((vl_code_retour == TRUE) && (vl_code_retour_prec == FALSE))
  {
    XZST_03EcritureTrace(XZSTC_WARNING, "Reprise de communication avec le RTserver: Demande de reinit. des Eqt.");
    (void)MSRA_Reinit(vl_Cnx, pl_Data, pl_ARG);
  }

  vl_code_retour_prec = vl_code_retour;
}
#endif

int TraiterReceptionMessageFlash(int fdSocket)
{
  char buffer[ISBI_COM_BUFFER_LEN];
  int i;
  char *pch;
  int codeMessage;
  int bouton;
  int typeObjet;
  char chaine[ISBI_COM_BUFFER_LEN];
  int typeEquip;
  int typeAction;
  int numEquip;
  int tmp_cmd;
  char fenetre;

  memset(buffer, 0, sizeof(ISBI_COM_BUFFER_LEN));
  memset(chaine, 0, sizeof(ISBI_COM_BUFFER_LEN));

  // sem_wait(&vg_semaphore);
  i = read(fdSocket, buffer, ISBI_COM_BUFFER_LEN); // ISBI_COM_ENTETE); /*LINUX*/
  buffer[i] = '\0';
  // sem_post(&vg_semaphore);
  XZST_03EcritureTrace(XZSTC_INFO, "Longueur lue dans la Socket = %d , buffer=%s", i, buffer);
  if (i <= 0)
    return XDC_NOK;

  /*LINUX*/
  pch = strtok(buffer, "~");
  sscanf(pch, "%d", &codeMessage);

  XZST_03EcritureTrace(XZSTC_WARNING, "CMD=%d, chaine=%s", codeMessage, pch);
  switch (codeMessage)
  {
  case ISBI_CMD_CLIC_MOBILE:
    sscanf(pch, "%d|%d|%d|%s", &tmp_cmd, &bouton, &typeObjet, chaine);
    XZST_03EcritureTrace(XZSTC_INFO, "Commande Mobile (Bt=%d, type=%d, chaine=%s)", bouton, typeObjet, chaine);
    MSOP_receptCommandeMobile(bouton, typeObjet, chaine);
    break;

  case ISBI_CMD_CLIC_EQUIP:
    sscanf(pch, "%d|%d|%d|%d", &tmp_cmd, &bouton, &typeEquip, &numEquip);
    XZST_03EcritureTrace(XZSTC_INFO, "Commande Equipement (Bt=%d, type=%d, no=%d)", bouton, typeEquip, numEquip);
    MSOP_receptCommandeEquipement(bouton, typeEquip, numEquip);
    break;

  case ISBI_CMD_CLIC_OBJET:
    sscanf(pch, "%d|%d|%d|%d|%[^#]", &tmp_cmd, &bouton, &typeEquip, &numEquip, chaine);
    XZST_03EcritureTrace(XZSTC_INFO, "Commande Objet (Bt=%d, type=%d, no=%d, chaine=%s)", bouton, typeEquip, numEquip, chaine);
    MSOP_receptCommandeObjet(bouton, typeEquip, numEquip, chaine);
    break;

  case ISBI_CMD_CLIC_BOUTON:
    sscanf(pch, "%d|%d|%[^#]", &tmp_cmd, &typeAction, chaine);
    XZST_03EcritureTrace(XZSTC_INFO, "Commande Bouton (typeAction=%d, chaine=%s)", typeAction, chaine);
    MSOP_receptCommandeBouton(typeAction, chaine);
    break;

  case ISBI_CMD_TIGMOT:
    sscanf(pch, "%d|%[^#]", &typeAction, chaine);
    XZST_03EcritureTrace(XZSTC_WARNING, "Commande tigmot (typeAction=%d, chaine=%s)", typeAction, chaine);
    MSOP_receptCommandeTigmot(typeAction, chaine);
    break;

  case ISBI_CMD_ARRET_COURBE:
    sscanf(pch, "%d|%c", &tmp_cmd, &fenetre);
    XZST_03EcritureTrace(XZSTC_INFO, "Commande Arret Courbe (fenetre=%c)", fenetre);
    MSCO_demande_arret_courbe(fenetre);
    break;

  case ISBI_CMD_PROJETER_VUE:
    sscanf(pch, "%d|%d|%[^#]", &tmp_cmd, &bouton, chaine);
    XZST_03EcritureTrace(XZSTC_INFO, "projection vue %s", chaine);
    XZIS30_VUE_PROJETER(vm_NomMachineSS, chaine);
    break;

  case ISBI_CMD_PROJETER_MASQUE:
    sscanf(pch, "%d|%d|%d", &tmp_cmd, &typeEquip, &bouton);
    XZST_03EcritureTrace(XZSTC_WARNING, "projection masque eqt=%d masque =%d", typeEquip, bouton);
    XZIS29_MASQUE_PROJETER(vm_NomMachineSS, typeEquip, bouton);
    break;

  default:
    XZST_03EcritureTrace(XZSTC_WARNING, "Commande INCONNUE : %d", codeMessage);
    break;
  }

  return XDC_OK;
}

int lire_config_TOPS(XDY_Texte va_param, XDY_Texte va_IP, XDY_Entier *va_display, XDY_Entier *va_port)
{
  XDY_Texte vl_NomFichier;
  XDY_Texte pl_param;
  FILE *pl_fp;
  XDY_Texte pl_ligne;
  XZSCT_NomMachine pl_NomMachine = "\0";
  XDY_Texte vl_IP;
  XDY_Entier vl_display, vl_port;

  XZSC_07NomMachine(pl_NomMachine);
  strcpy(vl_NomFichier, CO_FIC_TOPS_GEN_PATH);
  strcat(vl_NomFichier, pl_NomMachine);
  strcat(vl_NomFichier, ".txt");

  /* Ouverture du fichier de config */
  if ((pl_fp = fopen(vl_NomFichier, "r")) == NULL)
  {
    XZST_03EcritureTrace(XZSTC_WARNING, "erreur de lecture du fichier %s !", vl_NomFichier);
    return (XDC_NOK);
  }

  /* On lit les lignes du fichier une par une */
  /* ------------------------------------------------------ */

  while (fgets(pl_ligne, 200, pl_fp) != NULL)
  {
    /* On ignore les lignes commencant par un # */
    if (pl_ligne[0] == '#')
      continue;

    /* Pour les autres lignes on recherche la frequence de notre tache */
    if (sscanf(pl_ligne, "%s\t%s\t%d\t%d", pl_param, &vl_IP, &vl_display, &vl_port) == 4)
    {
      if (!strcmp(pl_param, va_param))
      {
        strcpy(va_IP, vl_IP);
        *va_display = vl_display;
        *va_port = vl_port;
        XZST_03EcritureTrace(XZSTC_WARNING, "lecture fichier config TOP param fichier %s, ligne %s = %s:%d.%d", vl_NomFichier, pl_param, va_IP, *va_display, *va_port);
        fclose(pl_fp);
        return (XDC_OK);
      }
    }
    else
    {
      XZST_03EcritureTrace(XZSTC_WARNING, "le fichier %s n'a pas le format demande !", vl_NomFichier);
    }
  }
  fclose(pl_fp);
  return (XDC_NOK);
}

void arret()
{
  MSMA_arret();
}
