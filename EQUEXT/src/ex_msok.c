/*E*/
/*  Fichier : $Id: ex_msok.c,v 1.47 2021/05/25 13:37:27 devgfi Exp $      Release : $Revision: 1.47 $        Date : $Date: 2021/05/25 13:37:27 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  : EQUEXT
------------------------------------------------------
* MODULE COMMUN * FICHIER ex_msok.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Module de manipulation des sockets Unix. Il contient
*   des fonctions de connexion, d'attente, de lecture
*   et d'�criture.
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	12 Aug 1994	: Creation
* Mismer.D	version 1.2	10 Nov 1994	: Ajout des fonction pour gestion socket Transerver
* Mismer.D	version 1.3	23 Nov 1994	:
* Mismer.D	version 1.4	28 Nov 1994	: Ajout des fonction pour gestion socket mode de grade
* Mismer.D	version 1.5	22 Dec 1994	:
* Mismer.D	version 1.6	03 Jan 1995	:
* Mismer.D	version 1.7	04 Jan 1995	:
* Mismer.D	version 1.8	25 Jan 1995	:
* Mismer.D	version 1.9	25 Jan 1995	:
* Mismer.D	version 1.10	27 Jan 1995	:
* Mismer.D	version 1.11	08 Mar 1995	:
* Mismer.D	version 1.12 	09 Avr 1995	:
* Mismer.D	version 1.13	19 Avr 1995	:
* Mismer.D	version 1.14	10 Mai 1995	:
* Mismer.D	version 1.15	11 Mai 1995	: Modif connextion client avec echec
* Fontaine.C	version 1.16 	23 Mai 1995	: Reprise entete, historique
* Mismer.D	version 1.17	02 Jun 1995	: Modif fonction ex_envoyer_commande_ST
* Mismer.D	version 1.18	24 Jun 1995	: Ajout fonction ex_cnx_serveur_LT
* Mismer.D	version 1.19	01 Jul 1995	: Ajout fonction ex_get_host_adresse
* Mismer.D	version 1.20	07 Sep 1995	: Modif fonction ex_cnx_serveur_LT
* Mismer.D	version 1.21	27 Sep 1995	: Modif fonction ex_lire_console
* Mismer.D	version 1.22	11 Oct 1995	: Modif fonction ex_lire_console suite
* Mismer.D	version 1.24	31 Oct 1995	: Modif time-out connexion client LT
* Mismer.D	version 1.25	07 Nov 1995	: Modif pour tache protocole LCR esclave
* Mismer.D	version 1.26	29 Nov 1995	: Modif pour probleme socket serveur REUSE
* Mismer.D	version 1.27	19 Dec 1995	: Ajout de ex_presence_machine et ex_cnx_test_ST
* Mismer.D	version 1.28	08 Mar 1996	: Contournement pour blocage rlogin dans  ex_presence_machine (DEM/1095)
* Mismer.D	version 1.29	15 Avr 1996	: Correction ex_sup_masque (DEM/1052)
* Mismer.D	version 1.30	19 Jun 1996	: Modification time-out lecture console (DEM/1111)
* Mismer.D	version 1.31	26 Sep 1996	: Modification pour extension OUEST (RADT_W)
* Mismer.D      version 1.32    20 Jan 1997     : Modif pour trame trop longue RADT (DEM/1...)
* Mismer.D      version 1.33    30 Mai 1997     : Mise en place serveur de terminaux XYPLEX (DEM/1431)
* JMG		version 1.34	8 juin 2010 	: linux DEM934
* JPL		20/12/10 : Migration architecture HP ia64 (DEM 961) : suppression warnings mineurs  1.35
* JMG		xx/02/11 : linux  1.36
* JPL		25/02/11 : Migration HP ia64 (DEM 975) : Gestion conversions types de tailles differentes (=,+)  1.37
* SFO           06/10/14 : Adaptation pour compilation sous linux
* JMG		01/03/16 : ST 40 ports 1.39
* JPL		08/11/16 : Traces d'etat transitoire en categorie DEBUG  1.40
* ???		09/02/18 : Envoi de commandes d'acces aux serveurs de terminaux 20 ports  1.41
* JPL		13/02/18 : Migration Linux 64 bits (DEM 1274) : 'ushort' et champs donnees socket obsoletes; types args. fonctions  1.42
* JMG		15/03/19 : identification mode ST 1/1.43
* ABE		15/02/21 : Ajout serveur client IPV4 DEM-SAE155 1.144
* JPL		10/05/21 : Traces  1.46
* JPL		17/05/21 : Suppression des declarations de variables en registre  1.47
* LCL		20/12/21	: MOVIS P2 ems SAE-312
-----------------------------------------------------*/

/* fichiers inclus */
#include <xzsem.h>
#include "ex_msok.h"
#include "ex_mcom.h"

/* definitions de constantes */

#define CM_PORT_CONSOLE_TRANSERVER 2000
#define CM_SU_TRANSERVER "ACCESS\r"
#define CM_SU1_TRANSERVER "CONSOLE\r"
#define CM_SU2_TRANSERVER "SET PRIV\r"
#define CM_PSWD_TRANSERVER "SYSTEM\r"
#define CM_FIN1_TRANSERVER "\377\373\003"
#define CM_FIN2_TRANSERVER "> "
#define CM_FIN3_TRANSERVER "#"

#define CM_PORT_CONSOLE_TRANSERVER40 23
#define CM_SU_TRANSERVER40 "InReach\r"
#define CM_SU1_TRANSERVER40 "access\r"
#define CM_SU2_TRANSERVER40 "enable\r"
#define CM_PSWD_TRANSERVER40 "system\r"

#define C_FILE_ST40 "/produits/migrazur/appliSD/fichiers/deq/ConfigST.txt"
/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

int vm_num_port_serie;

static char *version = "$Id: ex_msok.c,v 1.47 2021/05/25 13:37:27 devgfi Exp $ : ex_msok";
int vm_TacheEsclave = XDC_FAUX;

/* declaration de fonctions internes */
int ex_select(int, double);
int ex_select_write(int, double);

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Insertion d'un elements dans le masque des sockets
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void ex_ins_masque(XZEXT_MASK *pa_Enr,
                   XZEXT_MASK **pa_debut)
/*
* ARGUMENTS EN ENTREE :
*   pa_Enr 	: Pointeur sur enregistrement a inserer
*   pa_debut	: Pointeur sur sur le debut de la liste
*
* ARGUMENTS EN SORTIE :
*   aucun
*
*
* CODE RETOUR :
*   aucun
*
* CONDITION D'UTILISATION
*
* FONCTION
*   Allocation de memoire pour le nouvel element de la liste
*   Recherche de l'emplacement d'insertion de l'element
*   Mise a jour de la liste
*
------------------------------------------------------*/
{
   static char *version = "$Id: ex_msok.c,v 1.47 2021/05/25 13:37:27 devgfi Exp $ : ex_ins_masque ";
   XZEXT_MASK *pl_NewEnr;
   XZEXT_MASK *pl_Enr;
   XZEXT_MASK *pl_ExEnr;

   if (pa_Enr->desc_sock == 0)
      XZST_03EcritureTrace(XZSTC_WARNING, "Enregistrement de socket :%d <%>.", pa_Enr->desc_sock, pa_Enr->adresse);
   /*A si l'alocaltion memoire est ok */
   if ((pl_NewEnr = (XZEXT_MASK *)malloc(sizeof(XZEXT_MASK))) != NULL)
   {
      /*A copie enegistrement */
      memcpy(pl_NewEnr, pa_Enr, sizeof(XZEXT_MASK));

      /*A pour les enregistrements deja existant */
      for (pl_Enr = pl_ExEnr = *pa_debut; pl_Enr != NULL; pl_ExEnr = pl_Enr, pl_Enr = pl_Enr->Suivant)
      {
         /*A si le descripteur de socket est inferieure */
         if (pl_NewEnr->desc_sock < pl_Enr->desc_sock)

            /*A la place d'insertion est localisee : on stoppe la recherche */
            break;
      }
      /*A si la place d'insertion est la tete de liste */
      if (pl_Enr == *pa_debut)
      {
         /*A cas particulier de la tete de liste */
         pl_NewEnr->Suivant = *pa_debut;
         *pa_debut = pl_NewEnr;
      }
      /*A sinon insertion nominal */
      else
      {
         pl_NewEnr->Suivant = pl_ExEnr->Suivant;
         pl_ExEnr->Suivant = pl_NewEnr;
      }
   }
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Suppression d'un elements dans le masque des sockets
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
void ex_sup_masque(int va_DescSock,
                   XZEXT_MASK **pa_debut)

/*
* ARGUMENTS EN ENTREE :
*   va_DescSock	: Descripteur de la socket de l'enregistrement a supprimer
*   pa_debut	: Pointeur sur sur le debut de la liste
*
* ARGUMENTS EN SORTIE :
*   aucun
*
*
* CODE RETOUR :
*   aucun
*
* CONDITION D'UTILISATION
*
* FONCTION
*   Recherche de l'element a supprimer
*   Mise a jour de la liste
*   Liberation de la memoire
*
------------------------------------------------------*/
{
   static char *version = "$Id: ex_msok.c,v 1.47 2021/05/25 13:37:27 devgfi Exp $ : ex_sup_masque ";
   XZEXT_MASK *pl_Enr;
   XZEXT_MASK *pl_ExEnr;

   /*A pour chaque enregistrement de la liste */
   for (pl_ExEnr = pl_Enr = *pa_debut; pl_Enr != NULL; pl_ExEnr = pl_Enr, pl_Enr = pl_Enr->Suivant)
   {
      /*A si la socket est localise */
      if (pl_Enr->desc_sock == va_DescSock)
      {
         /*A supprime cette enregistrement */
         if ((pl_ExEnr == *pa_debut) && (pl_ExEnr == pl_Enr))
         {
            *pa_debut = pl_Enr->Suivant;
            pl_ExEnr = pl_Enr->Suivant;
         }
         else
         {
            pl_ExEnr->Suivant = pl_Enr->Suivant;
         }
         free(pl_Enr);
         break;
      }
   }
}

/*A definition de fonctions externes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*   Cette fonction memorise le repertoire des sockets de la tache
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void ex_rep_socket(XDY_NomMachine pa_NomMachine)

/*
* ARGUMENTS EN ENTREE :
*  XDY_NomMachine	pa_NomMachine  : Nom de la machine
*
* ARGUMENTS EN SORTIE :
*
* CODE RETOUR :
*
*
* CONDITION D'UTILISATION
*
* FONCTION
*
*  Memorisation en global du repertoire des sockets de la tache et
*  creation du repertoire s'il n'existe pas .
------------------------------------------------------*/

{
   static char *version = "$Id: ex_msok.c,v 1.47 2021/05/25 13:37:27 devgfi Exp $ : ex_rep_socket ";
   char vl_RepTrav[XDC_PATH_ABS_SIZE] = "";
   XZSCT_NomTache vl_NomTache = "";

   sprintf(vl_RepTrav, "%s/%s", XZEXC_PATH_SOCKET, pa_NomMachine);
   if (access(vl_RepTrav, F_OK))
   {
      mkdir(vl_RepTrav, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH | S_IWGRP | S_IWOTH | S_IXUSR | S_IXGRP | S_IXOTH);
      chmod(vl_RepTrav, S_IRWXU | S_IRWXG | S_IRWXO);
   }

   if (chdir(vl_RepTrav) == -1)
   {
      XZST_03EcritureTrace(XZSTC_WARNING, "Ne peut atteindre le repertoire %s.", vl_RepTrav);
   }

   XZSC_06NomTache(vl_NomTache);
   if (strstr(vl_NomTache, XZEXC_ADR_SOCK_TELE) != NULL)
      vm_TacheEsclave = XDC_VRAI;
}

/*X------------------------------------------------------
* SERVICE RENDU :
*   Cette fonction permet la connexion en mode serveur
*   sur une socket d'�coute
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int ex_cnx_serveur(
    char *pa_sockaddr,
    XZEXT_MASK **pa_masque,
    int *pa_sd_sock)

/*
* ARGUMENTS EN ENTREE :
*
*   pa_sockaddr	:	adresse (port) du serveur.
*   pa_masque	:	masque de lecture.
*
* ARGUMENTS EN SORTIE :
*
*   pa_masque	:	masque de lecture.
*   pa_sd_sock	:	descripteur de la socket d'�coute.
*
* CODE RETOUR :
*
*   XDC_OK		succ�s
*   XDC_NOK		connexion serveur impossible
*   XZEXC_ARG_INV	argument invalide
*
* CONDITION D'UTILISATION
*
* FONCTION
*
*   Cr�ation d'une socket d'�coute
*   Attachement de la socket d'�coute � une addresse
*   Insertion du descripteur de socket dans le masque
*   de lecture pour la r�ception des message
*   Ouverture du service d'�coute des demandes de connexion
*
------------------------------------------------------*/

{
   static char *version = "$Id: ex_msok.c,v 1.47 2021/05/25 13:37:27 devgfi Exp $ : ex_cnx_serveur ";
   int vl_valeurRetour = XDC_OK,
       vl_cr = XDC_OK,
       vl_sock_ecoute = XDC_NOK;
   struct sockaddr vl_sockaddr;
   XZEXT_MASK vl_masque, *pl_masque;

   /*A Initialisation de la structure sockaddr */
   vl_sockaddr.sa_family = AF_UNIX;
   strcpy(vl_sockaddr.sa_data, pa_sockaddr);

   /*A Creation d'une socket d'�coute */
   if ((vl_sock_ecoute = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
   {
      /*B Ecriture dans le fichier trace en cas d'�chec */
      XZST_03EcritureTrace(XZSTC_WARNING, "Ne peut creer une socket d'ecoute.");
      vl_valeurRetour = XDC_NOK;
   }
   else
   {
      /*A Suppression de la r�f�rence de la socket au cas ou elle exite d�ja */
      /*A Lecture du nom du repertoire courant */
      XZSS_06DetruireFichier(pa_sockaddr);

      /*A Attachement de la socket a une addresse */
      if ((vl_cr = bind(vl_sock_ecoute, &vl_sockaddr, sizeof(vl_sockaddr))) == -1)
      {
         /*B Ecriture dans le fichier trace en cas d'�chec */
         XZST_03EcritureTrace(XZSTC_WARNING, "Impossible d'attacher une adresse a une socket d'ecoute UNIX.");
         close(vl_sock_ecoute);
         *pa_sd_sock = 0;
         vl_valeurRetour = XDC_NOK;
      }
      else
      {
         /*A Insertion du descripteur de la socket dans le masque de lecture */
         vl_masque.desc_sock = vl_sock_ecoute;
         vl_masque.Type = AF_UNIX;
         strcpy(vl_masque.adresse, pa_sockaddr);
         ex_ins_masque(&vl_masque, pa_masque);

         /*A Mise � jour du descripteur de socket � retourner */
         *pa_sd_sock = vl_sock_ecoute;

         /*A Ouverture du service d'�coute des demandes de connexion */
         if ((vl_cr = listen(vl_sock_ecoute, XZEXC_NB_CNX_PDTE)) == -1)
         {
            /*B Ecriture dans le fichier trace en cas d'�chec */
            XZST_03EcritureTrace(XZSTC_WARNING, "Erreur sur demande listen socket.");
            vl_valeurRetour = XDC_NOK;
         }
      }
   }

   /*A Ajout parametre pour que les process fils n'herite pas de la socket */
   if (vl_valeurRetour == XDC_OK)
      fcntl(vl_sock_ecoute, F_SETFD, 1);

   return (vl_valeurRetour);
}

/**/
/*X------------------------------------------------------
* SERVICE RENDU :
*   Cette fonction permet la connexion en mode client
*   sur une socket de donnes.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int ex_cnx_client(
    char *pa_sockaddr_clt,
    char *pa_sockaddr_srv,
    XZEXT_MASK **pa_masque,
    int *pa_sd_sock_don)

/*
* ARGUMENTS EN ENTREE :
*
*   pa_sockaddr_clt	:	adresse (port) du client.
*   pa_sockaddr_srv	:	adresse (port) du serveur.
*   pa_masque		:	masque de lecture.
*
* ARGUMENTS EN SORTIE :
*
*   pa_masque		:	masque de lecture.
*   pa_sd_sock_don	:	descripteur de la socket de donn�es.
*
* CODE RETOUR :
*
*   XDC_OK		succ�s
*   XDC_NOK		connexion client impossible
*   XZEXC_ARG_INV	argument invalide
*
* CONDITION D'UTILISATION
*
* FONCTION
*
*   Cr�ation d'une socket de donn�es
*   Attachement de la socket de donn�es � une addresse
*   Insertion du descripteur de socket dans le masque
*   de lecture pour la r�ception des message
*   Demande connexion au serveur
*
------------------------------------------------------*/

{
   static char *version = "$Id: ex_msok.c,v 1.47 2021/05/25 13:37:27 devgfi Exp $ : ex_cnx_client ";
   int vl_valeurRetour = XDC_OK,
       vl_cr = XDC_OK,
       vl_sock_don = XDC_NOK;
   struct sockaddr vl_sockaddr_clt,
       vl_sockaddr_srv;
   XZEXT_MASK vl_masque, *pl_masque;

   /*A Initialisation des structures sockaddr */
   vl_sockaddr_clt.sa_family = AF_UNIX;
   strcpy(vl_sockaddr_clt.sa_data, pa_sockaddr_clt);
   vl_sockaddr_srv.sa_family = AF_UNIX;
   strcpy(vl_sockaddr_srv.sa_data, pa_sockaddr_srv);

   /*A Cr�ation d'une socket de donnee */
   if ((vl_sock_don = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
   {
      /*B Ecriture dans le fichier trace en cas d'�chec */
      XZST_03EcritureTrace(XZSTC_WARNING, "Ne peut cr�er une socket de donn�es.");
      vl_valeurRetour = XDC_NOK;
   }
   else
   {
      /*A Suppression de la r�f�rence de la socket au cas ou elle exite d�ja */
      /*A Lecture du nom du repertoire courant */
      XZSS_06DetruireFichier(pa_sockaddr_clt);

      /*A Attachement de la socket a une addresse */
      // sem_wait(&vg_semaphore);
      if ((vl_cr = bind(vl_sock_don, &vl_sockaddr_clt, sizeof(vl_sockaddr_clt))) == -1)
      {
         // sem_post(&vg_semaphore);
         /*B Ecriture dans le fichier trace en cas d'�chec */
         XZST_03EcritureTrace(XZSTC_WARNING, "Impossible d'attacher une adresse a une socket de donn�es.");
         close(vl_sock_don);
         *pa_sd_sock_don = 0;
         vl_valeurRetour = XDC_NOK;
      }
      else
      {
         // sem_post(&vg_semaphore);
         /*A Demande connexion au serveur */
         // sem_wait(&vg_semaphore);
         if ((vl_cr = connect(vl_sock_don, &vl_sockaddr_srv, sizeof(vl_sockaddr_srv))) == -1)
         {
            // sem_post(&vg_semaphore);
            /*B Ecriture dans le fichier trace en cas d'�chec */
            XZST_03EcritureTrace(XZSTC_WARNING, "Erreur sur demande connexion socket(%d).", vl_cr);
            close(vl_sock_don);
            *pa_sd_sock_don = 0;
            vl_valeurRetour = XDC_NOK;
         }
         else
         {
            // sem_post(&vg_semaphore);
            /*A Mise � jour du descripteur de socket � retourner */
            *pa_sd_sock_don = vl_sock_don;

            /*A Insertion du descripteur de la socket dans le masque de lecture */
            vl_masque.desc_sock = vl_sock_don;
            vl_masque.Type = AF_UNIX;
            strcpy(vl_masque.adresse, pa_sockaddr_srv);
            ex_ins_masque(&vl_masque, pa_masque);
         }
      }
   }

   /*A Ajout parametre pour que les process fils n'herite pas de la socket */
   if (vl_valeurRetour == XDC_OK)
      fcntl(vl_sock_don, F_SETFD, 1);

   return (vl_valeurRetour);
}

/*X------------------------------------------------------
* SERVICE RENDU :
*   Cette fonction permet la connexion en mode serveur
*   sur une socket d'oute
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int ex_cnx_serveur_LT(
    char *pa_NomST,
    int va_Port,
    XZEXT_MASK **pa_masque,
    int *pa_sd_sock_srv)

/*
* ARGUMENTS EN ENTREE :
*
*   pa_NomST		:	Nom du serveur de terminaux.
*   va_Port		:	Numero du port a connecter.
*   pa_masque		:	masque de lecture.
*
* ARGUMENTS EN SORTIE :
*
*   pa_masque		:	masque de lecture.
*   pa_sd_sock_don	:	descripteur de la socket de donnes
*
* CODE RETOUR :
*
*   XDC_OK		succ�s
*   XDC_NOK		connexion client impossible
*   XZEXC_ARG_INV	argument invalide
*
* CONDITION D'UTILISATION
*
* FONCTION
*
*   Cration d'une socket de donns
*   Insertion du descripteur de socket dans le masque
*   de lecture pour la rception des message
*   Demande connexion au serveur
*
------------------------------------------------------*/

{
   static char *version = "$Id: ex_msok.c,v 1.47 2021/05/25 13:37:27 devgfi Exp $ : ex_cnx_serveur_LT ";
   int vl_sock_srv = 0;
   XZEXT_MASK vl_masque;
   struct hostent *vl_Host = NULL;
   struct sockaddr_in vl_Addr;
   int vl_LgRetour = 0;
   int vl_REUSE = 1;
   int vl_KeepAlive = 1;
   struct linger vl_LINGER = {1, 0};
   int vl_ValRet = 0;

   /*A Recuperation de l'adresse TCP/IP du serveur de terminaux */
   if ((vl_Host = gethostbyname(pa_NomST)) == NULL)
   {
      XZST_03EcritureTrace(XZSTC_WARNING, "Adresse TCP/IP du serveur %s inconnu", pa_NomST);
      return (XDC_NOK);
   }

   /*A Creation de la socket */
   memset((char *)&vl_Addr, 0, sizeof(vl_Addr));
   if ((vl_sock_srv = socket(AF_INET, SOCK_STREAM, 0)) == -1)
   {
      XZST_03EcritureTrace(XZSTC_WARNING, "Erreur creation socket TCP/IP ");
      return (XDC_NOK);
   }

   /* Configuration de la socket en mode REUSE adresse */
   vl_LgRetour = sizeof(vl_REUSE);
   if ((vl_ValRet = setsockopt(vl_sock_srv, SOL_SOCKET, SO_REUSEADDR, &vl_REUSE, vl_LgRetour)) < 0)
   {
      XZST_03EcritureTrace(XZSTC_WARNING, "Erreur:%d sur set option SO_REUSEADDR socket.", vl_ValRet);
   }

   /* Configuration de la socket en mode KEEPALIVE  */
   /*!!   vl_LgRetour = sizeof(vl_KeepAlive);
      if ( (vl_ValRet = setsockopt ( vl_sock_srv, SOL_SOCKET, SO_KEEPALIVE, &vl_KeepAlive, vl_LgRetour )) < 0 )
      {
         XZST_03EcritureTrace( XZSTC_WARNING, "Erreur:%d sur set option SO_KEEPALIVE socket.", vl_ValRet );
      }
   */
   /* Configuration de la socket en mode close NO_WAIT */
   vl_LgRetour = sizeof(vl_LINGER);
   if ((vl_ValRet = setsockopt(vl_sock_srv, SOL_SOCKET, SO_LINGER, &vl_LINGER, vl_LgRetour)) < 0)
   {
      XZST_03EcritureTrace(XZSTC_WARNING, "Erreur:%d sur set option SO_LINGER socket.", vl_ValRet);
   }

   /*A Mise a jour de la structure adresse TCP/IP */
   vl_Addr.sin_family = AF_INET;
   vl_Addr.sin_port = htons(va_Port);
   /*!!   memcpy ( &vl_Addr.sin_addr, vl_Host->h_addr_list[0],(unsigned)vl_Host->h_length );*/
   vl_Addr.sin_addr.s_addr = INADDR_ANY;

   /*A Attachement socket serveur LT */
   if (bind(vl_sock_srv, (struct sockaddr *)&vl_Addr, sizeof(vl_Addr)) == -1)
   {
      vl_ValRet = errno;
      XZST_03EcritureTrace(XZSTC_WARNING, "Erreur:%d sur bind socket.", vl_ValRet);
      if (vl_ValRet == EADDRINUSE)
         XZST_03EcritureTrace(XZSTC_WARNING, "Erreur:<EADDRINUSE> sur bind socket.", vl_ValRet);
      close(vl_sock_srv);
      *pa_sd_sock_srv = 0;
      return (XDC_NOK);
   }

   /*A Insertion du descripteur de la socket dans le masque de lecture */
   vl_masque.desc_sock = vl_sock_srv;
   vl_masque.Type = AF_INET;
   strcpy(vl_masque.adresse, pa_NomST);
   ex_ins_masque(&vl_masque, pa_masque);
   *pa_sd_sock_srv = vl_sock_srv;

   /*A Ouverture du service d'ecoute des demandes de connexion */
   if (listen(vl_sock_srv, XZEXC_NB_CNX_PDTE) == -1)
   {
      XZST_03EcritureTrace(XZSTC_WARNING, "Erreur sur demande listen socket.");
      close(vl_sock_srv);
      *pa_sd_sock_srv = 0;
      return (XDC_NOK);
   }

   /*A Ajout parametre pour que les process fils n'herite pas de la socket */
   fcntl(vl_sock_srv, F_SETFD, 1);

   return (XDC_OK);
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*   Cette fonction permet la connexion en mode client
*   sur une socket de donnes.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int ex_cnx_client_ST(
    char *pa_NomST,
    int va_Port,
    XZEXT_MASK **pa_masque,
    int *pa_sd_sock_don)

/*
* ARGUMENTS EN ENTREE :
*
*   pa_NomST		:	Nom du serveur de terminaux.
*   va_Port		:	Numero du port a connecter.
*   pa_masque		:	masque de lecture.
*
* ARGUMENTS EN SORTIE :
*
*   pa_masque		:	masque de lecture.
*   pa_sd_sock_don	:	descripteur de la socket de donnees.
*
* CODE RETOUR :
*
*   XDC_OK		succes
*   XDC_NOK		connexion client impossible
*   XZEXC_ARG_INV	argument invalide
*
* CONDITION D'UTILISATION
*
* FONCTION
*
*   Creation d'une socket de donnees
*   Insertion du descripteur de socket dans le masque
*   de lecture pour la reception des message
*   Demande connexion au serveur
*
------------------------------------------------------*/

{
   static char *version = "$Id: ex_msok.c,v 1.47 2021/05/25 13:37:27 devgfi Exp $ : ex_cnx_client_ST ";
   int vl_sock_don = 0;
   XZEXT_MASK vl_masque,
       *pl_masque;
   struct hostent *vl_Host = NULL;
   struct sockaddr_in vl_Addr;
   int vl_LgRetour = 0;
   struct linger vl_LINGER = {1, 0};
   int vl_SockRet = 0;
   int vl_ValRet = 0;
   double vl_Reste = 0;
   XZEXT_MSG_SOCKET vl_message;
   XZEXT_MASK *pl_masqueST = NULL;

   /*A Recuperation de l'adresse TCP/IP du serveur de terminaux */
   if ((vl_Host = gethostbyname(pa_NomST)) == NULL)
   {
      XZST_03EcritureTrace(XZSTC_WARNING, "Adresse TCP/IP du serveur %s inconnu", pa_NomST);
      return (XDC_NOK);
   }

   /*A Creation de la socket */
   memset((char *)&vl_Addr, 0, sizeof(vl_Addr));
   if ((vl_sock_don = socket(AF_INET, SOCK_STREAM, 0)) == -1)
   {
      XZST_03EcritureTrace(XZSTC_WARNING, "Erreur creation socket TCP/IP ");
      return (XDC_NOK);
   }

   /* Configuration de la socket en mode close NO_WAIT */
   vl_LgRetour = sizeof(vl_LINGER);

   if (setsockopt(vl_sock_don, SOL_SOCKET, SO_LINGER, &vl_LINGER, vl_LgRetour) < 0)
   {
      XZST_03EcritureTrace(XZSTC_WARNING, "Erreur sur positionnemnt option SO_LINGER socket.");
   }

   /*A Mise a jour de la structure adresse TCP/IP */
   vl_Addr.sin_family = AF_INET;
   vl_Addr.sin_port = htons(va_Port);
   memcpy(&vl_Addr.sin_addr, vl_Host->h_addr_list[0], (unsigned)vl_Host->h_length);

   XZST_03EcritureTrace(XZSTC_WARNING, "pa_NomST %s // va_Port %d", pa_NomST, va_Port);
   /*A Essai connection au serveur de terminaux */
#ifdef _HPUX_SOURCE
   fcntl(vl_sock_don, F_SETFL, O_NDELAY | fcntl(vl_sock_don, F_GETFL, 0));
#else
   fcntl(vl_sock_don, F_SETFL, O_NONBLOCK | fcntl(vl_sock_don, F_GETFL, 0));
#endif
   // sem_wait(&vg_semaphore);
   if (connect(vl_sock_don, (struct sockaddr *)&vl_Addr, sizeof(vl_Addr)) == -1)
   {
      // sem_post(&vg_semaphore);
      /*B Ecriture dans le fichier trace en cas d'echec */
      if (errno == EINPROGRESS)
      {
         XZST_03EcritureTrace(XZSTC_WARNING, "Connexion en cours avec serveur %s ...", pa_NomST);
         if (ex_select_write(vl_sock_don, 15.0) == XDC_NOK)
         {
            XZST_03EcritureTrace(XZSTC_WARNING, "Erreur connexion socket avec serveur %s sur select.", pa_NomST);
            close(vl_sock_don);
            *pa_sd_sock_don = 0;
            return (XDC_NOK);
         }
         else
         {
            XZST_03EcritureTrace(XZSTC_WARNING, "avant second connect");
            // sem_wait(&vg_semaphore);
            if (connect(vl_sock_don, (struct sockaddr *)&vl_Addr, sizeof(vl_Addr)) == -1)
            {
               // sem_post(&vg_semaphore);
               if ((vl_ValRet = errno) != EISCONN)
               {
                  XZST_03EcritureTrace(XZSTC_WARNING, "Erreur connexion socket (%d) avec serveur %s(%d) errno=%d.", vl_sock_don, pa_NomST, va_Port, vl_ValRet);
                  close(vl_sock_don);
                  XZST_03EcritureTrace(XZSTC_WARNING, "apres close");
                  *pa_sd_sock_don = 0;
                  XZST_03EcritureTrace(XZSTC_WARNING, "apres pa_sockdon");
                  return (XDC_NOK);
               }
            }
            // sem_post(&vg_semaphore);
         }
      }
      else
      {
         XZST_03EcritureTrace(XZSTC_WARNING, "Erreur connexion socket avec serveur %s sur connect.", pa_NomST);
         close(vl_sock_don);
         *pa_sd_sock_don = 0;
         return (XDC_NOK);
      }
   }
   // sem_post(&vg_semaphore);

   /*A Insertion du descripteur de la socket dans le masque de lecture */
   vl_masque.desc_sock = vl_sock_don;
   vl_masque.Type = AF_INET;
   vl_masque.Port = va_Port;
   strcpy(vl_masque.adresse, pa_NomST);
   ex_ins_masque(&vl_masque, pa_masque);

   /*A Mise � jour du descripteur de socket � retourner */
   *pa_sd_sock_don = vl_sock_don;

   /*A Ajout parametre pour que les process fils n'herite pas de la socket */
   fcntl(vl_sock_don, F_SETFD, 1);

   return (XDC_OK);
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*   Cette fonction ferme la socket sp�cifi�e.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void ex_fin_cnx(
    int va_sd_sock,
    XZEXT_MASK **pa_masque)

/*
* ARGUMENTS EN ENTREE :
*
*   pa_masque	:	masque de lecture.
*
* ARGUMENTS EN SORTIE :
*
*   pa_masque	:	masque de lecture.
*   pa_sd_sock	:	descripteur de la socket.
*
* CODE RETOUR :
*
*   XDC_OK		succ�s
*   XDC_NOK		fermeture socket impossible
*   XZEXC_ARG_INV	argument invalide
*
* CONDITION D'UTILISATION
*
* FONCTION
*
*   Retirer la socket du masque de lecture
*   fermer la socket
*
------------------------------------------------------*/

{
   static char *version = "$Id: ex_msok.c,v 1.47 2021/05/25 13:37:27 devgfi Exp $ : ex_fin_cnx ";
   int vl_valeurRetour = XDC_OK,
       vl_cr = 0;
   XZEXT_MASK *pl_Enr = NULL;

   XZST_03EcritureTrace(XZSTC_WARNING, "ex_fin_cnx %d", va_sd_sock);
   /* Shutdown de la socket */
   // sem_wait(&vg_semaphore);
   for (pl_Enr = *pa_masque; pl_Enr != NULL; pl_Enr = pl_Enr->Suivant)
   {
      if ((pl_Enr->desc_sock == va_sd_sock) && (pl_Enr->Type != AF_UNIX))
      {
         shutdown(pl_Enr->desc_sock, 2);
         break;
      }
   }
   // sem_post(&vg_semaphore);

   /*A Suppression du descripteur de la socket dans le masque  de lecture */
   ex_sup_masque(va_sd_sock, pa_masque);

   /*A Fermeture de la socket */
   if ((vl_cr = close(va_sd_sock)) == XDC_NOK)
   {
      /*B Ecriture dans le fichier trace en cas d'�chec */
      XZST_03EcritureTrace(XZSTC_WARNING, "Impossible de fermer une socket.");
      vl_valeurRetour = XDC_NOK;
   }
}

/**/
/*X------------------------------------------------------
* SERVICE RENDU :
*   Cette fonction permet d'accepter une connexion
*   avec un client
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int ex_acpt_cnx(
    XDY_Mot va_Type,
    int pa_sd_sock_ect,
    XZEXT_MASK **pa_masque,
    int *pa_sd_sock_clt)

/*
* ARGUMENTS EN ENTREE :
*
*   pa_sd_sock_ect	:	descripteur de la socket d'�coute.
*   pa_masque		:	masque de lecture.
*
* ARGUMENTS EN SORTIE :
*
*   pa_masque		:	masque de lecture.
*   pa_sd_sock_clt	:	descripteur de la socket du client.
*
* CODE RETOUR :
*
*   XDC_OK		succ�s
*   XDC_NOK 		acceptation socket impossible
*   XZEXC_ARG_INV	argument invalide
*
* CONDITION D'UTILISATION
*
* FONCTION
*
*   Acceptation par le serveur de la demande de connexion du client.
*   Insertion du descripteur de socket dans le masque de lecture.
*   D�terminer le type d'�quipement en fonction de la tache
*   traitement cliente.
*
------------------------------------------------------*/

{
   static char *version = "$Id: ex_msok.c,v 1.47 2021/05/25 13:37:27 devgfi Exp $ : ex_acpt_cnx ";
   int vl_valeurRetour = XDC_OK;

   int vl_Socket = 0;
   struct sockaddr_un vl_sockaddr;
   struct sockaddr_in vl_Addr;
#ifdef _HPUX_SOURCE
   int vl_lgadr = 0;
#else
   socklen_t vl_lgadr = 0;
#endif

   int vl_LgRetour = 0;
   struct linger vl_LINGER = {1, 0};
   struct hostent *vl_Host = NULL;
   XZEXT_MASK vl_masque, *pl_masque;

   XZST_03EcritureTrace(XZSTC_DEBUG3, "accept connexion de type %d sur socket %d :", va_Type, pa_sd_sock_ect);

   /*A Initialisation de la structure sockaddr */
   if (va_Type == AF_UNIX)
   {
      vl_sockaddr.sun_family = AF_UNIX;
      strcpy(vl_sockaddr.sun_path, "");

      /*A Acceptation par le serveur de la demande de connexion du client. */
      vl_lgadr = sizeof(vl_sockaddr);
      // sem_wait(&vg_semaphore);
      vl_Socket = accept(pa_sd_sock_ect, (struct sockaddr *)&vl_sockaddr, &vl_lgadr);
      // sem_post(&vg_semaphore);
      vl_masque.desc_sock = vl_Socket;
      vl_masque.Type = AF_UNIX;
      strcpy(vl_masque.adresse, vl_sockaddr.sun_path);
   }
   else
   {
      /*A Acceptation par le serveur de la demande de connexion du client. */
      vl_Addr.sin_family = AF_INET;
      vl_lgadr = sizeof(vl_Addr);
      // sem_wait(&vg_semaphore);
      vl_Socket = accept(pa_sd_sock_ect, (struct sockaddr *)&vl_Addr, &vl_lgadr);
      // sem_post(&vg_semaphore);
      vl_masque.desc_sock = vl_Socket;
      vl_masque.Type = AF_INET;
      vl_Host = gethostbyaddr((char *)&(vl_Addr.sin_addr), sizeof(struct in_addr), AF_INET);
      strcpy(vl_masque.adresse, vl_Host->h_name);
      /* Configuration de la socket en mode close NO_WAIT */
      vl_LgRetour = sizeof(vl_LINGER);
      if (vl_Socket > 0)
      {
         if (setsockopt(vl_Socket, SOL_SOCKET, SO_LINGER, &vl_LINGER, vl_LgRetour) < 0)
         {
            XZST_03EcritureTrace(XZSTC_WARNING, "Erreur sur positionnemnt option SO_LINGER socket.");
         }
      }
   }

   if (vl_Socket <= 0)
   {
      /*B Ecriture dans le fichier trace en cas d'�chec */

      XZST_03EcritureTrace(XZSTC_WARNING, "Impossible d'accepter une demande de connexion avec un client T:%d Add:%s Num:%d. Lg:%d",
                           vl_masque.Type, vl_masque.adresse, vl_masque.desc_sock, (int)vl_lgadr);
      *pa_sd_sock_clt = 0;
      vl_valeurRetour = XDC_NOK;
   }
   else
   {
      XZST_03EcritureTrace(XZSTC_INFO, "connexion de %s acceptee, socket %d.", vl_masque.adresse, vl_Socket);

      /*A Insertion du descripteur de la socket dans le masque de lecture */
      ex_ins_masque(&vl_masque, pa_masque);
      *pa_sd_sock_clt = vl_Socket;
   }

   /*A Ajout parametre pour que les process fils n'herite pas de la socket */
   if (vl_valeurRetour == XDC_OK)
      fcntl(vl_Socket, F_SETFD, 1);

   return (vl_valeurRetour);
}

/**/
/*X------------------------------------------------------
* SERVICE RENDU :
*   Cette fonction permet d'envoyer un message par
*   une socket.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int ex_env_msg(
    char *pa_ident_dest,
    XZEXT_MASK **pa_masque,
    char *pa_message,
    int *pa_cr_emission)

/*
* ARGUMENTS EN ENTREE :
*
*   pa_ident_dest	:	Identit� du destinataire du message.
*   pa_masque		:	Masque de lecture
*   message		:	message � envoyer.
*
* ARGUMENTS EN SORTIE :
*
*   cr_emission	:	compte-rendu d'�mission.
*
* CODE RETOUR :
*
*   XDC_OK		succ�s
*   XDC_NOK		emission message impossible
*   XZEXC_ARG_INV	argument invalide
*
* CONDITION D'UTILISATION
*
* FONCTION
*
*   Identifier la socket du client.
*   Emettre le message � envoyer.
*
------------------------------------------------------*/

{
   static char *version = "$Id: ex_msok.c,v 1.47 2021/05/25 13:37:27 devgfi Exp $ : ex_env_msg ";
   int vl_sock_dest = 0,
       vl_valeurRetour = XDC_OK,
       vl_cr = XDC_OK;
   XZEXT_MASK *pl_masque;

   /*A Identification de la socket du destinataire du message */
   for (pl_masque = *pa_masque; pl_masque != NULL; pl_masque = pl_masque->Suivant)
   {
      XZST_03EcritureTrace(XZSTC_WARNING, "sock %s", pl_masque->adresse);
      if (strcmp(pl_masque->adresse, pa_ident_dest) == 0)
      {
         XZST_03EcritureTrace(XZSTC_WARNING, "socket %s", pa_ident_dest);
         vl_sock_dest = pl_masque->desc_sock;
         break;
      }
   }

   /*A Emission du message */
   //sem_wait(&vg_semaphore);
   if ((vl_cr = (int)write(vl_sock_dest, pa_message, strlen(pa_message))) < 0)
   {
      //sem_post(&vg_semaphore);
      /*B Ecriture dans le fichier trace en cas d'�chec */
      XZST_03EcritureTrace(XZSTC_WARNING, "Impossible d'�mettre un message sur une socket.");
      vl_valeurRetour = XDC_NOK;
      *pa_cr_emission = XDC_NOK;
   }
   else
   {
      //sem_post(&vg_semaphore);
      /*A Mise � jour du compte rendu d'�mission r�ussi */
      XZST_03EcritureTrace(XZSTC_FONCTION, "message %s emis", pa_message);
      *pa_cr_emission = XDC_OK;
   }

   return (vl_valeurRetour);
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*   Cette fonction permet de lire un message dans
*   la socket sp�cifi�e.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int ex_lire_msg(
    int va_sd_sock,
    char *pa_message,
    int *pa_lg_msg,
    XZEXT_MASK **pa_masque,
    int *pa_deconnex)

/*
* ARGUMENTS EN ENTREE :
*
*   va_sd_sock	:	descripteur de la socket de donn�es.
*   pa_message	:	pointeur de r�c�ption du message.
*   pa_masque	:	masque de lecture.
*
* ARGUMENTS EN SORTIE :
*
*   pa_masque	:	masque de lecture.
*   pa_message	:	pointeur de r�c�ption du message.
*   pa_lg_msg	:	pointeur de r�c�ption de la longueur du message.
*   pa_deconnex	:	indicateur de d�connexion.
*
* CODE RETOUR :
*
*   XDC_OK		succ�s
*   XDC_NOK		lecture message socket impossible
*   XZEXC_ARG_INV	argument invalide
*
* CONDITION D'UTILISATION
*
* FONCTION
*
*   Extraire un message d'une socket de donn�es.
*   Retourner le message et la longueur du message.
*
------------------------------------------------------*/
{
   static char *version = "$Id: ex_msok.c,v 1.47 2021/05/25 13:37:27 devgfi Exp $ : ex_lire_msg ";
   int vl_valeurRetour = XDC_OK,
       vl_ValRet = 0,
       vl_lg_msg = 0,
       vl_cr = 0,
       vl_Index = 0,
       vl_NbCar = 0,
       vl_Origine = 0,
       vl_LgTxtArret = (int)strlen(XZEXC_FIN_MSG_SOCK),
       vl_TypeLS = 0;
   XZEXT_MASK *pl_masque = NULL;
   static XZEXT_MSG_SOCKET vl_message = "";
   static XZEXT_MSG_SOCKET vl_MsgLecture = "";
   static XZEXT_MSG_SOCKET vl_MsgTrace = "";
   char vl_Suffixe[4];
   int vl_compteur = 0; /*linux*/

   /*A Lecture de la taille du message de la socket specifie. */
   for (pl_masque = *pa_masque; pl_masque != NULL; pl_masque = pl_masque->Suivant)
      if (pl_masque->desc_sock == va_sd_sock)
         break;

   if (pl_masque == NULL)
   {
      XZST_03EcritureTrace(XZSTC_WARNING, "Lecture sur socket:%d inconnue.", va_sd_sock);
      return (XDC_NOK);
   }

#ifdef _HPUX_SOURCE
   fcntl(va_sd_sock, F_SETFL, O_NDELAY | fcntl(va_sd_sock, F_GETFL, 0));
#else
   fcntl(va_sd_sock, F_SETFL, O_NONBLOCK | fcntl(va_sd_sock, F_GETFL, 0));
#endif
   memset(vl_message, 0, sizeof(XZEXT_MSG_SOCKET));
   //sem_wait(&vg_semaphore);
   if ((vl_cr = recv(va_sd_sock, vl_message, sizeof(XZEXT_MSG_SOCKET) - 2, MSG_PEEK)) == -1)
   {
      //sem_post(&vg_semaphore);
      /*B Ecriture dans le fichier trace en cas d'echec */
      XZST_03EcritureTrace(XZSTC_WARNING, "Erreur de lecture de message socket:%d <%s>.",
                           pl_masque->desc_sock,
                           pl_masque->adresse);
      vl_valeurRetour = XDC_NOK;
   }
   else
   {
      //sem_post(&vg_semaphore);
      /*A Si la longueur du message est nulle */
      if (vl_cr == 0)
      {
         /*A Fermeture de la socket */
         sem_wait(&vg_semaphore);
         read(va_sd_sock, vl_message, sizeof(XZEXT_MSG_SOCKET) - 2);
         sem_post(&vg_semaphore);
         XZST_03EcritureTrace(XZSTC_WARNING, "message fermeture <%s> sur %d:%s", vl_message, pl_masque->desc_sock, pl_masque->adresse);
         ex_fin_cnx(va_sd_sock, pa_masque);
         *pa_deconnex = XZEXC_DECONNEC;
      }
      else
      {
         /*A Positionnement a FAUX de l'indicateur de deconnexion socket */
         *pa_deconnex = XZEXC_CONNEC;

         /*A Recherche si le message a ete recu en entier */
         vl_TypeLS = ((!strncmp(pl_masque->adresse, "ST", 2)) || 
                     (!strncmp(pl_masque->adresse, "10.", 3)) || 
                     (!strncmp(pl_masque->adresse, "194.", 4)) || 
                     (!strncmp(pl_masque->adresse, "172.", 4))) ? 1 : 0;
         memset(vl_MsgLecture, 0, sizeof(XZEXT_MSG_SOCKET));
         memcpy(vl_MsgLecture, vl_message, vl_cr);

         XZST_03EcritureTrace(XZSTC_DEBUG1, "read socket : Lecture vl_MsgLecture=[%s] - vl_message=[%s] - vl_TypeLS=%d vl_cr=%d",vl_MsgLecture,vl_message,vl_TypeLS,vl_cr);
         while (1)
         {
            if (vl_TypeLS)
            {
               for (vl_Index = 0; vl_Index < vl_cr; vl_Index++)
               {
                  if ((vl_message[vl_Index] == XZEXC_CAR_ETX) ||
                      (vl_message[vl_Index] == XZEXC_CAR_ETB) ||
                      (vl_message[vl_Index] == XZEXC_CAR_ACK) ||
                      (vl_message[vl_Index] == XZEXC_CAR_NAK) ||
                      (vl_lg_msg > 0))
                  {
                     vl_lg_msg++;
                  }
               }
               /*!!      memcpy ( vl_MsgTrace, vl_message, vl_cr );
                     for ( vl_Index = 0; vl_Index < vl_cr; vl_Index++ )
                        if ( (vl_MsgTrace[vl_Index] == '\r') || (vl_MsgTrace[vl_Index] == '\0') ) vl_MsgTrace[vl_Index] = '|';
                     vl_MsgTrace[vl_cr] = '\0';
                     XZST_03EcritureTrace( XZSTC_WARNING, "Reception: <%s>.", vl_MsgTrace );
               */
            }
            else
            {
               vl_Origine = vl_NbCar - vl_cr - vl_LgTxtArret;
               vl_Origine = (vl_Origine < 0) ? 0 : vl_Origine;
               for (vl_Index = vl_Origine; vl_Index <= vl_NbCar + vl_cr - vl_LgTxtArret; vl_Index++)
               {
                  if (!strncmp(&vl_MsgLecture[vl_Index], XZEXC_FIN_MSG_SOCK, vl_LgTxtArret))
                  {
                     vl_lg_msg = vl_Origine + vl_Index + vl_LgTxtArret;
                     break;
                  }
               }
            }

            if (((!vl_TypeLS) && (vl_lg_msg > 0)) ||
                ((vl_TypeLS) && (vl_lg_msg > 1)))
               break;

            if ((size_t)vl_NbCar + vl_cr <= sizeof(XZEXT_MSG_SOCKET) - 2)
            {
               memcpy(vl_MsgLecture + vl_NbCar, vl_message, vl_cr);
               vl_NbCar += vl_cr;

               memset(vl_message, 0, sizeof(XZEXT_MSG_SOCKET));
               sem_wait(&vg_semaphore);
               if ((vl_cr = (int)read(va_sd_sock, vl_message, vl_NbCar)) == -1)
               {
                  sem_post(&vg_semaphore);
                  XZST_03EcritureTrace(XZSTC_WARNING, "Erreur de lecture de message socket:%d <%s>.",
                                       pl_masque->desc_sock,
                                       pl_masque->adresse);
                  return (XDC_NOK);
                  break;
               }
               sem_post(&vg_semaphore);
               vl_message[vl_cr]='\0';
               XZST_03EcritureTrace(XZSTC_WARNING, "message lu <%s><%d><%d>", vl_message, vl_cr, vl_NbCar);

               if ((ex_select(va_sd_sock, (vl_TypeLS) ? XZEXC_VAL_DEFAUT_TIMER : 0.3)) == XDC_NOK)
               {
                  XZST_03EcritureTrace(XZSTC_WARNING, "Erreur de lecture sur une socket (fin Time-Out).");
                  vl_valeurRetour = XZEXC_NO_EVT; /*XDC_NOK; */
                  break;
               }
               else
               {
                  //sem_wait(&vg_semaphore);
                  if ((vl_cr = recv(va_sd_sock, vl_message, sizeof(XZEXT_MSG_SOCKET) - 2, MSG_PEEK)) == -1)
                  {
                     //sem_post(&vg_semaphore);
                     XZST_03EcritureTrace(XZSTC_WARNING, "Erreur de lecture sur une socket (erreur systeme).");
                     vl_valeurRetour = XDC_NOK;
                     break;
                  }
                  else
                  {
                     //sem_post(&vg_semaphore);
                  }
               }
            }
            else
            {
               XZST_03EcritureTrace(XZSTC_WARNING, "Erreur de lecture sur une socket (Message trop long niv 1).");
               vl_valeurRetour = XDC_NOK;
               break;
            }
         }

         /*A Recopie du message pour l'appelant */
         if (vl_valeurRetour == XDC_OK)
         {
            sem_wait(&vg_semaphore);
            vl_cr = (int)read(va_sd_sock, vl_message, (vl_TypeLS) ? sizeof(XZEXT_MSG_SOCKET) - 2 : vl_lg_msg);
            vl_message[vl_cr] = '\0';
            sem_post(&vg_semaphore);
            if ((size_t)vl_NbCar + vl_cr <= sizeof(XZEXT_MSG_SOCKET) - 2)
            {
               memcpy(vl_MsgLecture + vl_NbCar, vl_message, (size_t)vl_cr + 1);
               vl_NbCar += vl_cr;
               vl_Index = 0;
               while (vl_TypeLS && ((vl_MsgLecture[vl_Index] == XZEXC_CAR1_REMPLISSAGE) ||
                                    (vl_MsgLecture[vl_Index] == XZEXC_CAR2_REMPLISSAGE)))
                  vl_Index++;
               memcpy(pa_message, vl_MsgLecture + vl_Index, (size_t)vl_NbCar + 1 - vl_Index);
               *pa_lg_msg = vl_NbCar - vl_Index;
               XZST_03EcritureTrace(XZSTC_DEBUG1, "Suppression %d caracteres - nouveau message pa_message=[%s]",vl_Index,pa_message);
            }
            else
            {
               XZST_03EcritureTrace(XZSTC_WARNING, "Erreur de lecture sur une socket (Message trop long niv 2).");
               strcpy(pa_message, "");
               *pa_lg_msg = 0;
               vl_valeurRetour = XDC_NOK;
            }

            /*A Lecture des caracteres du suffixe si ligne LS */
            /*linux*/
            while (vl_TypeLS && (ex_select(va_sd_sock, (vm_TacheEsclave == XDC_VRAI) ? 0.1 : 0.75) == XDC_OK) && (vl_compteur < 50))
            {
               vl_compteur++;

               /*A Si un �venement est survenu sur une socket */
               sem_wait(&vg_semaphore);
               if (read(va_sd_sock, vl_Suffixe, 1) == -1)
               {
                  sem_post(&vg_semaphore);
                  break;
               }
               else
               {
                  sem_post(&vg_semaphore);
               }
            }
         }
         else
         {
            sem_wait(&vg_semaphore);
            memset(vl_message, 0, sizeof(XZEXT_MSG_SOCKET));
            read(va_sd_sock, vl_message, sizeof(XZEXT_MSG_SOCKET) - 2);
            sem_post(&vg_semaphore);
            //strcpy(pa_message, "");
            memset(pa_message, 0, sizeof(XZEXT_MSG_SOCKET));
            *pa_lg_msg = 0;
         }
      }
   }

   return (vl_valeurRetour);
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*   Cette fonction permet de determiner un evenement en lecture sur
*   la socket specifiee.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int ex_select(int va_sd_sock,
              double va_TimeOut)

/*
* ARGUMENTS EN ENTREE :
*
*   va_sd_sock	:	descripteur de la socket de donnees.
*   va_TimeOut	:	Time out
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR :
*
*   XDC_OK		evenement sur la socket
*   XDC_NOK		buffer socket vide
*
* CONDITION D'UTILISATION
*
* FONCTION
*
*
------------------------------------------------------*/
{
   static char *version = "$Id: ex_msok.c,v 1.47 2021/05/25 13:37:27 devgfi Exp $ : ex_select ";
   int vl_nb_sock = 0;
   fd_set vl_mask_read;
   struct timeval vl_timeout;
   double vl_Delta;

   FD_ZERO(&vl_mask_read);
   vl_nb_sock = va_sd_sock + 1;
   FD_SET(va_sd_sock, &vl_mask_read);

   /*A Attendre des messages en mode non bloquant */
   vl_Delta = floor(va_TimeOut);
   vl_timeout.tv_sec = (unsigned long)vl_Delta;
   vl_timeout.tv_usec = (long)((va_TimeOut - vl_Delta) * 1000000);

   // sem_wait(&vg_semaphore);
   if (select(vl_nb_sock, &vl_mask_read, NULL, NULL, &vl_timeout) == -1)
   {
      // sem_post(&vg_semaphore);
      XZST_03EcritureTrace(XZSTC_WARNING, "Erreur ex_select_lire ");
      return (XDC_NOK);
   }
   else
   {
      // sem_post(&vg_semaphore);
      /*A Si un evenement est survenu sur une socket */
      if (FD_ISSET(va_sd_sock, &vl_mask_read))
         return (XDC_OK);
      else
         return (XDC_NOK);
   }
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*   Cette fonction permet de determiner un evement en ecriture sur
*   la socket sp�cifi�e.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int ex_select_write(int va_sd_sock,
                    double va_TimeOut)

/*
* ARGUMENTS EN ENTREE :
*
*   va_sd_sock	:	descripteur de la socket de donn�es.
*   va_TimeOut	:	Time out
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR :
*
*   XDC_OK		evenement sur la socket
*   XDC_NOK		buffer socket vide
*
* CONDITION D'UTILISATION
*
* FONCTION
*
*
------------------------------------------------------*/
{
   static char *version = "$Id: ex_msok.c,v 1.47 2021/05/25 13:37:27 devgfi Exp $ : ex_select_write ";
   int vl_nb_sock = 0;
   fd_set vl_mask_write;
   struct timeval vl_timeout;
   double vl_Delta;

   FD_ZERO(&vl_mask_write);
   vl_nb_sock = va_sd_sock + 1;
   FD_SET(va_sd_sock, &vl_mask_write);

   /*A Attendre des messages en mode non bloquant */
   vl_Delta = floor(va_TimeOut);
   vl_timeout.tv_sec = (unsigned long)vl_Delta;
   vl_timeout.tv_usec = (long)((va_TimeOut - vl_Delta) * 1000000);

   // sem_wait(&vg_semaphore);
   if (select(vl_nb_sock, NULL, &vl_mask_write, NULL, &vl_timeout) == -1)
   {
      // sem_post(&vg_semaphore);
      XZST_03EcritureTrace(XZSTC_WARNING, "Erreur ex_select_write ");
      return (XDC_NOK);
   }
   else
   {
      // sem_post(&vg_semaphore);
      /*A Si un �venement est survenu sur une socket */
      if (FD_ISSET(va_sd_sock, &vl_mask_write))
         return (XDC_OK);
      else
         return (XDC_NOK);
   }
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*   Cette fonction se met en attente d'un message
*   sur la liste des sockets
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int ex_attendre_msg(
    int pa_sd_sock_ect,
    XZEXT_MASK **pa_masque,
    int *pa_evenement)

/*
* ARGUMENTS EN ENTREE :
*
*   pa_sd_sock_ect	:	descripteur de la socket d'�coute.
*   pa_masque		:	masque de lecture.
*
* ARGUMENTS EN SORTIE :
*
*   pa_masque		:	masque de lecture.
*   pa_evenement	:	existence ou non d'�v�nement sur une socket.
*
* CODE RETOUR :
*
*   XDC_OK		succ�s
*   XZEXC_ARG_INV	argument invalide
*
* CONDITION D'UTILISATION
*
* FONCTION
*
*   Attendre des messages en mode non bloquant sur le
*   masque de lecture.
*   Retourner le masque des sockets.
*
------------------------------------------------------*/

{
   static char *version = "$Id: ex_msok.c,v 1.47 2021/05/25 13:37:27 devgfi Exp $ : ex_attendre_msg ";
   int vl_valeurRetour = XDC_OK,
       vl_nb_sock = 0,
       vl_cr = 0;

   int vl_sock_clt = 0;
   XZEXT_MASK *pl_masque;
   fd_set vl_mask_read;

   struct timeval vl_timeout;

   /*A Controle des param�tres d'entree */
   if (*pa_masque == NULL)
   {
      /*B Retourne  XZEXC_ARG_INV */
      return (XZEXC_ARG_INV);
   }

   /*A Recherche du nbre de socket dans le masque et init du masque pour select*/

   FD_ZERO(&vl_mask_read);
   for (pl_masque = *pa_masque; pl_masque != NULL; pl_masque = pl_masque->Suivant)
   {
      if (pl_masque->desc_sock > vl_nb_sock)
      {
         /*A Memorisation du descripteur le plus haut pour select */
         vl_nb_sock = pl_masque->desc_sock;
      }
      FD_SET(pl_masque->desc_sock, &vl_mask_read);
   }

   vl_nb_sock++;

   /*A Attendre des messages en mode non bloquant */
   vl_timeout.tv_sec = 3;
   vl_timeout.tv_usec = XZEXC_TIME_OUT_SELECT;

   // sem_wait(&vg_semaphore);
   if ((vl_cr = select(vl_nb_sock, &vl_mask_read, NULL, NULL, &vl_timeout)) == -1)
   {
      // sem_post(&vg_semaphore);
      /*B Ecriture dans le fichier trace en cas d'echec */
      if ((vl_cr = errno) == EINTR)
      {
         // XZST_03EcritureTrace( XZSTC_DEBUG3, "attente msg interrompue");
         *pa_evenement = XZEXC_NO_EVT;
         return (XDC_OK);
      }

      for (pl_masque = *pa_masque; pl_masque != NULL; pl_masque = pl_masque->Suivant)
      {
         FD_ZERO(&vl_mask_read);
         FD_SET(pl_masque->desc_sock, &vl_mask_read);
         vl_nb_sock = pl_masque->desc_sock + 1;

         /*A Attendre des messages en mode non bloquant */
         vl_timeout.tv_sec = 0;
         vl_timeout.tv_usec = 10;
         // sem_wait(&vg_semaphore);
         if ((vl_cr = select(vl_nb_sock, &vl_mask_read, NULL, NULL, &vl_timeout)) == -1)
         {
            // sem_post(&vg_semaphore);
            /*JMG FATAL*/
            XZST_03EcritureTrace(XZSTC_FATAL, "Erreur select sur socket:%d <%s>.", pl_masque->desc_sock, pl_masque->adresse);
            *pa_evenement = pl_masque->desc_sock;
            return (XDC_NOK);
         }
         // sem_post(&vg_semaphore);
      }
      vl_valeurRetour = XDC_NOK;
   }
   else
   {
      // sem_post(&vg_semaphore);
      /*A Si un evenement est survenu sur une socket */
      for (pl_masque = *pa_masque; pl_masque != NULL; pl_masque = pl_masque->Suivant)
      {
         /*A Test si une socket est active */
         if (FD_ISSET(pl_masque->desc_sock, &vl_mask_read))
         {
            /*A Test c'est la socket d'ecoute qui est active */
            if (pl_masque->desc_sock == pa_sd_sock_ect)
            {
               /*A Acceptation de la connection client */
               if ((vl_cr = ex_acpt_cnx(pl_masque->Type, pa_sd_sock_ect, pa_masque, &vl_sock_clt)) == XDC_NOK)
               {
                  /*B Ecriture dans le fichier trace en cas d'echec */
                  *pa_evenement = XZEXC_NO_EVT;
                  return (XDC_OK);
               }
               else
               {
                  /*A Retourne le descripteur de la socket d'ecoute */
                  *pa_evenement = pa_sd_sock_ect;
               }
            }
            else
            {
               /*A Retourne le descripteur de la socket de donn�es */
               *pa_evenement = pl_masque->desc_sock;
            }
            break; /*B for */
         }
      }

      /*A Retourne XZEXC_NO_EVT si aucun �venement n'est survenu */
      if (pl_masque == NULL)
      {
         *pa_evenement = XZEXC_NO_EVT;
      }
   }

   return (vl_valeurRetour);
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*   Cette fonction permet de lire un message dans
*   la socket console serveur de terminaux.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int ex_lire_console(
    int va_sd_sock,
    char *pa_message,
    int *pa_lg_msg)

/*
* ARGUMENTS EN ENTREE :
*
*   va_sd_sock	:	descripteur de la socket de donnees.
*   pa_message	:	pointeur de reception du message.
*
* ARGUMENTS EN SORTIE :
*
*   pa_message	:	pointeur de reception du message.
*   pa_lg_msg	:	pointeur de reception de la longueur du message.
*
* CODE RETOUR :
*
*   XDC_OK		succes
*   XDC_NOK		lecture message socket impossible
*   XZEXC_ARG_INV	argument invalide
*
* CONDITION D'UTILISATION
*
* FONCTION
*
*   Extraire un message d'une socket de donnees.
*   Retourner le message et la longueur du message.
*
------------------------------------------------------*/
{
   static char *version = "$Id: ex_msok.c,v 1.47 2021/05/25 13:37:27 devgfi Exp $ : ex_lire_console ";
   int vl_valeurRetour = XDC_OK,
       vl_lg_msg = -1,
       vl_cr = 0,
       vl_NbCar = 0,
       vl_Index = 0;
   static XZEXT_MSG_SOCKET vl_message = "";
   static XZEXT_MSG_SOCKET vl_MsgLecture = "";
   static XZEXT_MSG_SOCKET vl_MsgTrace = "";

   /*A Lecture de la taille du message de la socket specifiee. */
#ifdef _HPUX_SOURCE
   fcntl(va_sd_sock, F_SETFL, O_NDELAY | fcntl(va_sd_sock, F_GETFL, 0));
#else
   fcntl(va_sd_sock, F_SETFL, O_NONBLOCK | fcntl(va_sd_sock, F_GETFL, 0));
#endif
   /*A Recherche si le message a ete recu en entier */
   while (vl_lg_msg < 0)
   {
      sem_wait(&vg_semaphore);
      if ((vl_cr = (int)read(va_sd_sock, vl_message, sizeof(XZEXT_MSG_SOCKET) - 2)) == -1)
      {
         sem_post(&vg_semaphore);
         XZST_03EcritureTrace(XZSTC_WARNING, "Erreur de lecture sur la socket console Transerver.");
         vl_valeurRetour = XDC_NOK;
         break;
      }
      else
	  {
         sem_post(&vg_semaphore);
	  }
      if (vl_cr == 0)
      {
         /*A Si deconnexion retourne NOK */
         XZST_03EcritureTrace(XZSTC_WARNING, "Deconnexion sur la socket console Transerver.");
         vl_valeurRetour = XDC_NOK;
         break;
      }
      if (((size_t)vl_NbCar + vl_cr) < sizeof(XZEXT_MSG_SOCKET) - 2)
      {
         memcpy(vl_MsgLecture + vl_NbCar, vl_message, vl_cr);
         vl_NbCar += vl_cr;
      }
      else
      {
         memcpy(vl_MsgLecture, vl_message, vl_cr);
         vl_NbCar = vl_cr;
      }

      memcpy(vl_MsgTrace, vl_message, vl_cr);
      for (vl_Index = 0; vl_Index < vl_cr; vl_Index++)
         if ((vl_MsgTrace[vl_Index] == '\r') || (vl_MsgTrace[vl_Index] == '\0'))
            vl_MsgTrace[vl_Index] = '|';
      vl_MsgTrace[vl_cr] = '\0';
      XZST_03EcritureTrace(XZSTC_WARNING, "Console: <%s>.", vl_MsgTrace);

      for (vl_Index = 0; vl_Index <= vl_NbCar; vl_Index++)
      {
         if ((!strncmp(&vl_MsgLecture[vl_Index], CM_FIN1_TRANSERVER, strlen(CM_FIN1_TRANSERVER))) ||
             (!strncmp(&vl_MsgLecture[vl_Index], CM_FIN2_TRANSERVER, strlen(CM_FIN2_TRANSERVER))) ||
             (!strncmp(&vl_MsgLecture[vl_Index], CM_FIN3_TRANSERVER, strlen(CM_FIN3_TRANSERVER))))
         {
            vl_lg_msg = 1;
            break;
         }
      }

      if (ex_select(va_sd_sock, (vl_lg_msg > 0) ? 0.75 : XZEXC_VAL_DEFAUT_TIMER) == XDC_NOK)
      {
         if (vl_lg_msg < 0)
         {
            XZST_03EcritureTrace(XZSTC_WARNING, "Erreur de lecture console Transerver (fin Time-Out).");
            vl_valeurRetour = XDC_NOK;
         }
         break;
      }
      else
      {
         if (vl_lg_msg > 0)
            vl_lg_msg = -1;
      }
   }

   /*A Recopie du message pour l'appelant */
   if (vl_valeurRetour != XDC_NOK)
   {
      vl_MsgLecture[vl_NbCar] = '\0';
      strcpy(pa_message, vl_MsgLecture);
      *pa_lg_msg = vl_NbCar;
   }

   return (vl_valeurRetour);
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*   Cette fonction se met en attente d'un message
*   sur la liste des sockets d'un serveur de terminaux
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int ex_attendre_msg_ST(
    int pa_sd_sock_ect,
    XZEXT_MASK **pa_masque,
    double va_TimeOut,
    int *pa_evenement,
    double *pa_ResteTimeOut)

/*
* ARGUMENTS EN ENTREE :
*
*   pa_sd_sock_ect	:	descripteur de la socket d'ecoute.
*   pa_masque		:	masque de lecture.
*   va_TimeOut		:	Time-out
*
* ARGUMENTS EN SORTIE :
*
*   pa_masque		:	masque de lecture.
*   pa_evenement	:	existence ou non d'evenement sur une socket.
*
* CODE RETOUR :
*
*   XDC_OK		succes
*   XZEXC_ARG_INV	argument invalide
*
* CONDITION D'UTILISATION
*
* FONCTION
*
*   Attendre des messages en mode non bloquant sur le
*   masque de lecture.
*   Retourner le masque des sockets.
*
------------------------------------------------------*/

{
   static char *version = "$Id: ex_msok.c,v 1.47 2021/05/25 13:37:27 devgfi Exp $ : ex_attendre_msg_ST ";
   int vl_nb_sock = 0;
   int vl_sock_clt = 0;
   double vl_Delta;

   XZEXT_MASK *pl_masque;
   fd_set vl_mask_read;

   struct timeval vl_timeout;

   /*A Controle des param�tres d'entres */
   if (*pa_masque == NULL)
   {
      /*B Retourne  XZEXC_ARG_INV */
      return (XZEXC_ARG_INV);
   }

   /*A Recherche du nbre de socket dans le masque et init du masque pour select*/

   FD_ZERO(&vl_mask_read);
   for (pl_masque = *pa_masque; pl_masque != NULL; pl_masque = pl_masque->Suivant)
   {
      if (pl_masque->desc_sock > vl_nb_sock)
      {
         /*A Memorisation du descripteur le plus haut pour select */
         vl_nb_sock = pl_masque->desc_sock;
      }
      FD_SET(pl_masque->desc_sock, &vl_mask_read);
   }

   vl_nb_sock++;

   /*A Attendre des messages en mode non bloquant */
   /*A Memorisation de l'heure debut select */
   vl_Delta = floor(va_TimeOut);
   vl_timeout.tv_sec = (unsigned long)vl_Delta;
   vl_timeout.tv_usec = (long)((va_TimeOut - vl_Delta) * 1000000);

   vl_Delta = TutGetWallTime();

   // sem_wait(&vg_semaphore);
   if (select(vl_nb_sock, &vl_mask_read, NULL, NULL, &vl_timeout) == -1)
   {
      // sem_post(&vg_semaphore);
      /*B Ecriture dans le fichier trace en cas d'echec */
      *pa_ResteTimeOut = 0;
      return (XDC_NOK);
   }
   // sem_post(&vg_semaphore);

   /*A Si un evenement est survenu sur une socket */
   for (pl_masque = *pa_masque; pl_masque != NULL; pl_masque = pl_masque->Suivant)
   {
      /*A Test si une socket est active */
      if (FD_ISSET(pl_masque->desc_sock, &vl_mask_read))
      {
         /*A Test c'est la socket d'ecoute qui est active */
         if (pl_masque->desc_sock == pa_sd_sock_ect)
         {
            /*A Acceptation de la connection client */
            if (ex_acpt_cnx(pl_masque->Type, pa_sd_sock_ect, pa_masque, &vl_sock_clt) == XDC_NOK)
            {
               /*B Ecriture dans le fichier trace en cas d'�chec */
               return (XDC_NOK);
            }
            else
            {
               /*A Retourne le descripteur de la socket d'ecoute */
               *pa_evenement = pa_sd_sock_ect;
            }
         }
         else
         {
            /*A Retourne le descripteur de la socket de donn�es */
            *pa_evenement = pl_masque->desc_sock;
         }
         break;
      }
   }

   /*A Retourne XZEXC_NO_EVT si aucun �venement n'est survenu */
   if (pl_masque == NULL)
   {
      *pa_evenement = XZEXC_NO_EVT;
      *pa_ResteTimeOut = 0;
   }
   else
   {
      //       *pa_ResteTimeOut = va_TimeOut - ( TutGetWallTime() - vl_Delta );
      //       *pa_ResteTimeOut = ( *pa_ResteTimeOut < 0.0005 ) ? 0 : *pa_ResteTimeOut;
      if (vl_timeout.tv_sec == 0 && vl_timeout.tv_usec < 5)
      {
         *pa_ResteTimeOut = 0;
      }
      else
      {
         vl_Delta = (double)(vl_timeout.tv_sec);
         XZST_03EcritureTrace(XZSTC_DEBUG1, "ex_attendre_msg_ST : vl_Delta = %lf", vl_Delta);
         vl_Delta += (double)vl_timeout.tv_usec/(double)1000000.0;
         XZST_03EcritureTrace(XZSTC_DEBUG1, "ex_attendre_msg_ST : vl_Delta = %lf", vl_Delta);
         *pa_ResteTimeOut = vl_Delta;
         XZST_03EcritureTrace(XZSTC_DEBUG1, "ex_attendre_msg_ST : vl_timeout.tv_sec = %ld vl_timeout.tv_usec = %ld vl_Delta = %lf", 
                                    vl_timeout.tv_sec,
                                    vl_timeout.tv_usec,
                                    vl_Delta);
   }
      XZST_03EcritureTrace(XZSTC_DEBUG1, "ex_attendre_msg_ST : pa_ResteTimeout = %lf", *pa_ResteTimeOut);
   }

   return (XDC_OK);
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*   Cette fonction se met en attente d'un message
*   sur la liste des sockets d'un serveur de terminaux
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int ex_ouvrir_console_ST(char *pa_NomST,
                         int *pa_sd_sock,
                         XZEXT_MASK **pa_masque,
                         char *va_machine)

/*
* ARGUMENTS EN ENTREE :
*
*  char			*pa_NomST		:  Nom du serveur de terminaux
*
* ARGUMENTS EN SORTIE :
*
*  int			*pa_sd_sock	: socket console
*  XZEXT_MASK * 	*pa_masque	: masque console
*
* CODE RETOUR :
*
*   XDC_OK		succes
*   XZEXC_ARG_INV	argument invalide
*
* CONDITION D'UTILISATION
*
* FONCTION
*
*   Ouvrir un connexion avec la console du serveur de terminaux.
*
------------------------------------------------------*/
{
   static char *version = "$Id: ex_msok.c,v 1.47 2021/05/25 13:37:27 devgfi Exp $ : ex_ouvrir_console_ST ";
   int vl_SockCon = 0;
   int vl_SockRet = 0;
   int vl_ValRet = 0;
   double vl_Reste = 0;
   static XZEXT_MSG_SOCKET vl_message;
   char vl_tache[25];
   char vl_tele;

   int vl_port;

   /* Connection sur la console du transerver */
   if (ex_st40(va_machine) == XDC_NOK)
   {
      XZST_03EcritureTrace(XZSTC_WARNING, "ST 20 ports %s", pa_NomST);
      if (ex_cnx_client_ST(pa_NomST, CM_PORT_CONSOLE_TRANSERVER, pa_masque, &vl_SockCon) != XDC_OK)
         return (XDC_NOK);
      //sem_wait(&vg_semaphore);
      vl_ValRet = (int)write(vl_SockCon, CM_SU_TRANSERVER, strlen(CM_SU_TRANSERVER));
      vl_ValRet = (int)write(vl_SockCon, CM_SU_TRANSERVER, strlen(CM_SU_TRANSERVER));
      //sem_post(&vg_semaphore);
   }
   else
   {
      XZSC_06NomTache(vl_tache);
      sscanf(vl_tache, "tel%c%d.", &vl_tele, &vl_port);
      vl_port = 2000 + (vl_port * 100);
      XZST_03EcritureTrace(XZSTC_WARNING, "ST %s port %d", pa_NomST, vl_port);
      if (ex_cnx_client_ST(pa_NomST, /*2100*/ vl_port, pa_masque, &vl_SockCon) != XDC_OK)
         return (XDC_NOK);
   }
   sleep(3);

   XZST_03EcritureTrace(XZSTC_WARNING, "ICI");
   /* Envoie de la commande Super-User */
   if (ex_st40(va_machine) == XDC_NOK)
   {
      XZST_03EcritureTrace(XZSTC_WARNING, "%s ", CM_SU_TRANSERVER);
      //sem_wait(&vg_semaphore);
      vl_ValRet = (int)write(vl_SockCon, CM_SU_TRANSERVER, strlen(CM_SU_TRANSERVER));
      //sem_post(&vg_semaphore);
      sleep(1);
   }
   else
   {
      //sem_wait(&vg_semaphore);
      vl_ValRet = (int)write(vl_SockCon, CM_SU_TRANSERVER40, strlen(CM_SU_TRANSERVER40));
      //sem_post(&vg_semaphore);
      sleep(1);
   }

   ex_attendre_msg_ST(-1, pa_masque, (double)30.0, &vl_SockRet, &vl_Reste);
   if (vl_Reste > 0)
   {
      if (ex_lire_console(vl_SockRet, vl_message, &vl_ValRet) != XDC_OK)
      {
         ex_fin_cnx(vl_SockRet, pa_masque);
         return (XDC_NOK);
      }
      XZST_03EcritureTrace(XZSTC_DEBUG1, "Msg Console :<%s> reponse :<%s>", CM_SU_TRANSERVER, vl_message);
   }
   /* Envoie de la commande Super-User */
   // sem_wait(&vg_semaphore);
   if (ex_st40(va_machine) == XDC_NOK)
      vl_ValRet = (int)write(vl_SockCon, CM_SU1_TRANSERVER, strlen(CM_SU1_TRANSERVER));
   else
   {
      vl_ValRet = (int)write(vl_SockCon, CM_SU1_TRANSERVER40, strlen(CM_SU1_TRANSERVER40));
      sleep(1);
   }
   // sem_post(&vg_semaphore);

   ex_attendre_msg_ST(-1, pa_masque, (double)30.0, &vl_SockRet, &vl_Reste);
   if (vl_Reste > 0)
   {
      if (ex_lire_console(vl_SockRet, vl_message, &vl_ValRet) != XDC_OK)
      {
         ex_fin_cnx(vl_SockRet, pa_masque);
         return (XDC_NOK);
      }
      XZST_03EcritureTrace(XZSTC_DEBUG1, "Msg Console :<%s> reponse :<%s>", CM_SU1_TRANSERVER, vl_message);
   }
   if (ex_st40(va_machine) == XDC_NOK)
   {
      /* Envoie de la commande Super-User */
      //sem_wait(&vg_semaphore);
      vl_ValRet = (int)write(vl_SockCon, CM_SU2_TRANSERVER, strlen(CM_SU2_TRANSERVER));
      //sem_post(&vg_semaphore);

      ex_attendre_msg_ST(-1, pa_masque, (double)30.0, &vl_SockRet, &vl_Reste);
      if (vl_Reste > 0)
      {
         if (ex_lire_console(vl_SockRet, vl_message, &vl_ValRet) != XDC_OK)
         {
            ex_fin_cnx(vl_SockRet, pa_masque);
            return (XDC_NOK);
         }
         XZST_03EcritureTrace(XZSTC_DEBUG1, "Msg Console :<%s> reponse :<%s>", CM_SU2_TRANSERVER, vl_message);
      }

      /* Envoie du mot de passe Super-User */
      //sem_wait(&vg_semaphore);
      vl_ValRet = (int)write(vl_SockCon, CM_PSWD_TRANSERVER, strlen(CM_PSWD_TRANSERVER));
      //sem_post(&vg_semaphore);

      ex_attendre_msg_ST(-1, pa_masque, (double)30.0, &vl_SockRet, &vl_Reste);
      if (vl_Reste > 0)
      {
         if (ex_lire_console(vl_SockRet, vl_message, &vl_ValRet) != XDC_OK)
         {
            ex_fin_cnx(vl_SockRet, pa_masque);
            return (XDC_NOK);
         }
         XZST_03EcritureTrace(XZSTC_DEBUG1, "Msg Console :<%s> reponse :<%s>", CM_PSWD_TRANSERVER, vl_message);
      }
   }
   *pa_sd_sock = vl_SockCon;
   return (XDC_OK);
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*   Cette fonction se met en attente d'un message
*   sur la liste des sockets d'un serveur de terminaux
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int ex_envoyer_commande_ST(char *pa_Commande,
                           int va_sd_sock,
                           XZEXT_MASK **pa_masque)

/*
* ARGUMENTS EN ENTREE :
*
*  char			*pa_Commande		:  Commande a envoyer
*  int			 va_sd_sock	        : socket console
*
* ARGUMENTS EN SORTIE :
*
*  XZEXT_MASK * 	*pa_masque	: masque console
*
* CODE RETOUR :
*
*   XDC_OK		succ�s
*   XZEXC_ARG_INV	argument invalide
*
* CONDITION D'UTILISATION
*
* FONCTION
*
*   Envoyer une commande a la console du serveur de terminaux.
*
------------------------------------------------------*/
{
   static char *version = "$Id: ex_msok.c,v 1.47 2021/05/25 13:37:27 devgfi Exp $ : ex_envoyer_commande_ST ";
   int vl_SockRet = 0;
   int vl_ValRet = 0;
   double vl_Reste = 0;
   char *pl_Texte = NULL;
   XZEXT_MSG_SOCKET vl_message = "";
   XZEXT_MSG_SOCKET vl_reponse = "";

   /* Envoie de commande sur la console */
   //sem_wait(&vg_semaphore);
   if ((vl_ValRet = (int)write(va_sd_sock, pa_Commande, strlen(pa_Commande))) == -1)
   {
      //sem_post(&vg_semaphore);
      XZST_03EcritureTrace(XZSTC_WARNING, "Erreur envoi commande Console :%s", pa_Commande);
      return (XDC_NOK);
   }
   //sem_post(&vg_semaphore);

   /* Attente jusqu'a reponse complete terminee */
   ex_attendre_msg_ST(-1, pa_masque, (double)20.0, &vl_SockRet, &vl_Reste);
   if ((vl_Reste > 0) && (vl_SockRet != XZEXC_NO_EVT))
   {
#ifdef _HPUX_SOURCE
      memcpy(vl_message, '\0', sizeof(XZEXT_MSG_SOCKET) - 1);
#endif
      if (ex_lire_console(vl_SockRet, vl_message, &vl_ValRet) != XDC_OK)
      {
         ex_fin_cnx(vl_SockRet, pa_masque);
         return (XDC_NOK);
      }
      strcat(vl_reponse, vl_message);
      XZST_03EcritureTrace(XZSTC_DEBUG1, "Msg Console :%s", vl_message);
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

int ex_presence_machine(XDY_NomMachine pa_NomMachine)

/*
* ARGUMENTS EN ENTREE :
*   XDY_NomMachine	pa_NomMachine	nom machine destinatrice
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
* CODE RETOUR :
*   Aucun
*
* CONDITION D'UTILISATION
*
*
* FONCTION
*   Test si une machine est accessible sur le reseau
*
------------------------------------------------------*/
{
   int vl_sock_don = 0;
   struct hostent *vl_Host = NULL;
   struct sockaddr_in vl_Addr;
   int vl_LgRetour = 0;
   struct linger vl_LINGER = {1, 0};
   int vl_ValRet = XDC_OK;
   char vl_Suffixe[4];

   /*A Recuperation de l'adresse TCP/IP de la machine */
   if ((vl_Host = gethostbyname(pa_NomMachine)) == NULL)
   {
      XZST_03EcritureTrace(XZSTC_WARNING, "Adresse TCP/IP de la machine %s inconnu", pa_NomMachine);
      return (XDC_NOK);
   }

   /*A Creation de la socket */
   memset((char *)&vl_Addr, 0, sizeof(vl_Addr));
   if ((vl_sock_don = socket(AF_INET, SOCK_STREAM, 0)) == -1)
   {
      XZST_03EcritureTrace(XZSTC_WARNING, "Erreur creation socket TCP/IP ");
      return (XDC_NOK);
   }

   /* Configuration de la socket en mode close NO_WAIT */
   vl_LgRetour = sizeof(vl_LINGER);
   if (setsockopt(vl_sock_don, SOL_SOCKET, SO_LINGER, &vl_LINGER, vl_LgRetour) < 0)
   {
      XZST_03EcritureTrace(XZSTC_WARNING, "Erreur sur positionnemnt option SO_LINGER socket.");
   }

   /*A Mise a jour de la structure adresse TCP/IP */
   vl_Addr.sin_family = AF_INET;
   vl_Addr.sin_port = htons(2021);
   memcpy(&vl_Addr.sin_addr, vl_Host->h_addr_list[0], (unsigned)vl_Host->h_length);

   /*A Essai connection au serveur de terminaux */
#ifdef _HPUX_SOURCE
   fcntl(vl_sock_don, F_SETFL, O_NDELAY | fcntl(vl_sock_don, F_GETFL, 0));
#else
   fcntl(vl_sock_don, F_SETFL, O_NONBLOCK | fcntl(vl_sock_don, F_GETFL, 0));
#endif
   // sem_wait(&vg_semaphore);
   if (connect(vl_sock_don, (struct sockaddr *)&vl_Addr, sizeof(vl_Addr)) == -1)
   {
      // sem_post(&vg_semaphore);
      /*B Ecriture dans le fichier trace en cas d'�chec */
      if (errno == EINPROGRESS)
      {
         XZST_03EcritureTrace(XZSTC_DEBUG3, "Socket EINPROGRESS sur connexion avec machine %s .", pa_NomMachine);

         /*! Ajout attente et lecture telnet pour contournement pb bocage rlogin */
         ex_select_write(vl_sock_don, 7.5);

         // sem_wait(&vg_semaphore);
         connect(vl_sock_don, (struct sockaddr *)&vl_Addr, sizeof(vl_Addr));
         vl_LgRetour = errno;
         // sem_post(&vg_semaphore);
#ifdef _HPUX_SOURCE
         if (vl_LgRetour == EINVAL)
#else
         if ((vl_LgRetour == EINVAL) || (vl_LgRetour == ECONNREFUSED))
#endif
         {
            XZST_03EcritureTrace(XZSTC_DEBUG1, "Machine %s presente errno=%d.", pa_NomMachine, vl_LgRetour);
            vl_ValRet = XDC_OK;
         }
         else
         {
            XZST_03EcritureTrace(XZSTC_DEBUG1, "Machine %s absente errno=%d.", pa_NomMachine, vl_LgRetour);
            vl_ValRet = XDC_NOK;
         }
      }
      else
      {
         XZST_03EcritureTrace(XZSTC_DEBUG1, "Impossible de se connecter avec machine %s .", pa_NomMachine);
         vl_ValRet = XDC_NOK;
      }
   }

   shutdown(vl_sock_don, 2);
   close(vl_sock_don);
   // sem_post(&vg_semaphore);
   return (vl_ValRet);
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*   Cette fonction permet de tester la connexion avec
*   un serveur de terminaux
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int ex_cnx_test_ST(
    char *pa_NomST,
    int va_Port)

/*
* ARGUMENTS EN ENTREE :
*
*   pa_NomST		:	Nom du serveur de terminaux.
*   va_Port		:	Numero du port a connecter.
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR :
*
*   XDC_OK		succ�s
*   XDC_NOK		connexion client impossible
*
* CONDITION D'UTILISATION
*
* FONCTION
*
*   Demande connexion au serveur
*
------------------------------------------------------*/

{
   static char *version = "$Id: ex_msok.c,v 1.47 2021/05/25 13:37:27 devgfi Exp $ : ex_cnx_test_ST ";
   int vl_sock_don = 0;
   struct hostent *vl_Host = NULL;
   struct sockaddr_in vl_Addr;
   int vl_LgRetour = 0;
   struct linger vl_LINGER = {1, 0};
   int vl_ValRet = XDC_OK;

   XZST_03EcritureTrace(XZSTC_FONCTION, "IN  :ex_cnx_test_ST pour %s sur port %d", pa_NomST, va_Port);

   /*A Recuperation de l'adresse TCP/IP du serveur de terminaux */
   if ((vl_Host = gethostbyname(pa_NomST)) == NULL)
   {
      XZST_03EcritureTrace(XZSTC_WARNING, "Adresse TCP/IP du serveur %s inconnu", pa_NomST);
      return (XDC_NOK);
   }

   /*A Creation de la socket */
   memset((char *)&vl_Addr, 0, sizeof(vl_Addr));
   if ((vl_sock_don = socket(AF_INET, SOCK_STREAM, 0)) == -1)
   {
      XZST_03EcritureTrace(XZSTC_WARNING, "Erreur creation socket TCP/IP ");
      return (XDC_NOK);
   }

   /* Configuration de la socket en mode close NO_WAIT */
   vl_LgRetour = sizeof(vl_LINGER);
   if (setsockopt(vl_sock_don, SOL_SOCKET, SO_LINGER, &vl_LINGER, vl_LgRetour) < 0)
   {
      XZST_03EcritureTrace(XZSTC_WARNING, "Erreur sur positionnemnt option SO_LINGER socket.");
   }

   /*A Mise a jour de la structure adresse TCP/IP */
   vl_Addr.sin_family = AF_INET;
   vl_Addr.sin_port = htons(va_Port);
   memcpy(&vl_Addr.sin_addr, vl_Host->h_addr_list[0], (unsigned)vl_Host->h_length);

   /*A Essai connection au serveur de terminaux */
#ifdef _HPUX_SOURCE
   fcntl(vl_sock_don, F_SETFL, O_NDELAY | fcntl(vl_sock_don, F_GETFL, 0));
#else
   fcntl(vl_sock_don, F_SETFL, O_NONBLOCK | fcntl(vl_sock_don, F_GETFL, 0));
#endif
   // sem_wait(&vg_semaphore);
   if (connect(vl_sock_don, (struct sockaddr *)&vl_Addr, sizeof(vl_Addr)) == -1)
   {
      // sem_post(&vg_semaphore);
      /*B Ecriture dans le fichier trace en cas d'�chec */
      if (errno == EINPROGRESS)
      {
         XZST_03EcritureTrace(XZSTC_DEBUG3, "Socket EINPROGRESS sur connexion avec server %s .", pa_NomST);

         /*! Ajout attente et lecture telnet pour contournement pb bocage rlogin */
         ex_select_write(vl_sock_don, 7.5);

         // sem_wait(&vg_semaphore);
         connect(vl_sock_don, (struct sockaddr *)&vl_Addr, sizeof(vl_Addr));
         vl_LgRetour = errno;
         // sem_post(&vg_semaphore);
#ifdef _HPUX_SOURCE
         if (vl_LgRetour == EINVAL)
#else
         if ((vl_LgRetour == EINVAL) || (vl_LgRetour == ECONNREFUSED))
#endif
         {
            XZST_03EcritureTrace(XZSTC_FONCTION, "OUT :ex_cnx_test_ST retourne NOK");
            vl_ValRet = XDC_NOK;
         }
         else
         {
            XZST_03EcritureTrace(XZSTC_FONCTION, "OUT :ex_cnx_test_ST retourne OK");
            vl_ValRet = XDC_OK;
         }
      }
      else
      {
         XZST_03EcritureTrace(XZSTC_FONCTION, "OUT :ex_cnx_test_ST retourne OK");
         vl_ValRet = XDC_OK;
      }
   }

   shutdown(vl_sock_don, 2);
   close(vl_sock_don);
   // sem_post(&vg_semaphore);
   return (vl_ValRet);
}

/*X------------------------------------------------------
* SERVICE RENDU :
*   Cette fonction permet la connexion en mode serveur
*   sur une socket IPV4
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int ex_cnx_serveur_IPV4(
    int va_Port,
    XZEXT_MASK **pa_masque,
    int *pa_sock_srv)

/*
* ARGUMENTS EN ENTREE :
*
*   pa_Nom		:	Nom du serveur de terminaux.
*   va_Port		:	Numero du port a connecter.
*   pa_masque		:	masque de lecture.
*
* ARGUMENTS EN SORTIE :
*
*   pa_masque		:	masque de lecture.
*   pa_sock_srv :	descripteur de la socket serveur
*
* CODE RETOUR :
*
*   XDC_OK		success
*   XDC_NOK		Creation server impossible
*
* CONDITION D'UTILISATION
*
* FONCTION
*
*   Cration d'une socket IPV4 (serveur)
*   Retourne le descripteur de socket
*
------------------------------------------------------*/

{
   static char *version = "$Id: ex_msok.c,v 1.47 2021/05/25 13:37:27 devgfi Exp $ : ex_cnx_serveur_LT ";
   int vl_sock_srv = 0;
   XZEXT_MASK vl_masque;
   struct sockaddr_in vl_Addr;
   int vl_LgRetour = 0;
   struct linger vl_LINGER = {1, 0};
   int vl_ValRet = 0;

   /*A Creation de la socket */
   memset((char *)&vl_Addr, 0, sizeof(vl_Addr));
   if ((vl_sock_srv = socket(AF_INET, SOCK_STREAM, 0)) == -1)
   {
      XZST_03EcritureTrace(XZSTC_WARNING, "Erreur creation socket TCP/IP ");
      return (XDC_NOK);
   }

   /* Configuration de la socket en mode close NO_WAIT */
   vl_LgRetour = sizeof(vl_LINGER);
   if ((vl_ValRet = setsockopt(vl_sock_srv, SOL_SOCKET, SO_LINGER, &vl_LINGER, vl_LgRetour)) < 0)
   {
      XZST_03EcritureTrace(XZSTC_WARNING, "Erreur:%d sur set option SO_LINGER socket.", vl_ValRet);
   }

   /*A Mise a jour de la structure adresse TCP/IP */
   vl_Addr.sin_family = AF_INET;
   vl_Addr.sin_port = htons(va_Port);
   vl_Addr.sin_addr.s_addr = INADDR_ANY; // On accepte n'importe quelle adresse

   /*A Attachement socket serveur */
   if (bind(vl_sock_srv, (struct sockaddr *)&vl_Addr, sizeof(vl_Addr)) == -1)
   {
      vl_ValRet = errno;
      XZST_03EcritureTrace(XZSTC_WARNING, "Erreur:%d sur bind socket.", vl_ValRet);
      if (vl_ValRet == EADDRINUSE)
         XZST_03EcritureTrace(XZSTC_WARNING, "Erreur:<EADDRINUSE> sur bind socket.", vl_ValRet);
      close(vl_sock_srv);
      *pa_sock_srv = 0;
      return (XDC_NOK);
   }

   /*A Insertion du descripteur de la socket dans le masque de lecture */
   vl_masque.desc_sock = vl_sock_srv;
   vl_masque.Type = AF_INET;
   // strcpy ( vl_masque.adresse, pa_Nom );
   ex_ins_masque(&vl_masque, pa_masque);
   *pa_sock_srv = vl_sock_srv;

   /*A Ouverture du service d'ecoute des demandes de connexion */
   if (listen(vl_sock_srv, XZEXC_NB_CNX_PDTE) == -1)
   {
      XZST_03EcritureTrace(XZSTC_WARNING, "Erreur sur demande listen socket.");
      close(vl_sock_srv);
      *pa_sock_srv = 0;
      return (XDC_NOK);
   }

   /*A Ajout parametre pour que les process fils n'herite pas de la socket */
   fcntl(vl_sock_srv, F_SETFD, 1);

   return (XDC_OK);
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*   Cette fonction permet la connexion en mode client
*   sur une socket IPV4
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int ex_cnx_client_IPV4(
    char *pa_Nom,
    int va_Port,
    XZEXT_MASK **pa_masque,
    int *pa_sock_client)

/*
* ARGUMENTS EN ENTREE :
*
*   pa_Nom		:	Nom du serveur pour r�cuperer l'adresse.
*   va_Port		:	Numero du port a connecter.
*   pa_masque		:	masque de lecture.
*
* ARGUMENTS EN SORTIE :
*
*   pa_masque		:	masque de lecture.
*   pa_sock_client	:	descripteur de la socket de donnees.
*
* CODE RETOUR :
*
*   XDC_OK		succes
*   XDC_NOK		connexion client impossible
*   XZEXC_ARG_INV	argument invalide
*
* CONDITION D'UTILISATION
*
* FONCTION
*
*   Cration d'une socket IPV4 (client)
*   Retourne le descripteur de socket
*
------------------------------------------------------*/

{
   static char *version = "$Id: ex_msok.c,v 1.47 2021/05/25 13:37:27 devgfi Exp $ : ex_cnx_client_ST ";
   int vl_sock_client = 0;
   XZEXT_MASK vl_masque,
       *pl_masque;
   struct hostent *vl_Host = NULL;
   struct sockaddr_in vl_Addr;
   int vl_LgRetour = 0;
   struct linger vl_LINGER = {1, 0};
   int vl_ValRet = 0;

   /*A Recuperation de l'adresse TCP/IP du serveur de terminaux */
   if ((vl_Host = gethostbyname(pa_Nom)) == NULL)
   {
      XZST_03EcritureTrace(XZSTC_WARNING, "Adresse TCP/IP du serveur %s inconnu", pa_Nom);
      return (XDC_NOK);
   }

   /*A Creation de la socket */
   memset((char *)&vl_Addr, 0, sizeof(vl_Addr));
   if ((vl_sock_client = socket(AF_INET, SOCK_STREAM, 0)) == -1)
   {
      XZST_03EcritureTrace(XZSTC_WARNING, "Erreur creation socket TCP/IP ");
      return (XDC_NOK);
   }

   /* Configuration de la socket en mode close NO_WAIT */
   vl_LgRetour = sizeof(vl_LINGER);

   if (setsockopt(vl_sock_client, SOL_SOCKET, SO_LINGER, &vl_LINGER, vl_LgRetour) < 0)
   {
      XZST_03EcritureTrace(XZSTC_WARNING, "Erreur sur positionnemnt option SO_LINGER socket.");
   }

   /*A Mise a jour de la structure adresse TCP/IP */
   vl_Addr.sin_family = AF_INET;
   vl_Addr.sin_port = htons(va_Port);
   memcpy(&vl_Addr.sin_addr, vl_Host->h_addr_list[0], (unsigned)vl_Host->h_length);

   XZST_03EcritureTrace(XZSTC_WARNING, "pa_Nom %s // va_Port %d", pa_Nom, va_Port);
   /*A Essai connection au serveur de terminaux */
   /*#ifdef _HPUX_SOURCE
      fcntl ( vl_sock_client, F_SETFL, O_NDELAY | fcntl ( vl_sock_client, F_GETFL, 0 ) );
   #else
   fcntl ( vl_sock_client, F_SETFL, O_NONBLOCK | fcntl ( vl_sock_client, F_GETFL, 0 ) );
   #endif*/
   // sem_wait(&vg_semaphore);
   if (connect(vl_sock_client, (struct sockaddr *)&vl_Addr, sizeof(vl_Addr)) == -1)
   {
      // sem_post(&vg_semaphore);
      /*B Ecriture dans le fichier trace en cas d'echec */
      if (errno == EINPROGRESS)
      {
         XZST_03EcritureTrace(XZSTC_WARNING, "Connexion en cours avec serveur %s ...", pa_Nom);
         if (ex_select_write(vl_sock_client, 15.0) == XDC_NOK)
         {
            XZST_03EcritureTrace(XZSTC_WARNING, "Erreur connexion socket avec serveur %s sur select.", pa_Nom);
            close(vl_sock_client);
            *pa_sock_client = 0;
            return (XDC_NOK);
         }
         else
         {
            XZST_03EcritureTrace(XZSTC_WARNING, "avant second connect");
            // sem_wait(&vg_semaphore);
            if (connect(vl_sock_client, (struct sockaddr *)&vl_Addr, sizeof(vl_Addr)) == -1)
            {
               // sem_post(&vg_semaphore);
               if ((vl_ValRet = errno) != EISCONN)
               {
                  XZST_03EcritureTrace(XZSTC_WARNING, "Erreur connexion socket (%d) avec serveur %s(%d) errno=%d.", vl_sock_client, pa_Nom, va_Port, vl_ValRet);
                  close(vl_sock_client);
                  XZST_03EcritureTrace(XZSTC_WARNING, "apres close");
                  *pa_sock_client = 0;
                  XZST_03EcritureTrace(XZSTC_WARNING, "apres pa_sockdon");
                  return (XDC_NOK);
               }
            }
            else
            {
               // sem_post(&vg_semaphore);
            }
         }
      }
      else
      {
         XZST_03EcritureTrace(XZSTC_WARNING, "Erreur connexion socket avec serveur %s sur connect, errno=%s.", pa_Nom, strerror(errno));
         close(vl_sock_client);
         *pa_sock_client = 0;
         return (XDC_NOK);
      }
   }

   // sem_post(&vg_semaphore);
   /*A Insertion du descripteur de la socket dans le masque de lecture */
   vl_masque.desc_sock = vl_sock_client;
   vl_masque.Type = AF_INET;
   vl_masque.Port = va_Port;
   strcpy(vl_masque.adresse, pa_Nom);
   ex_ins_masque(&vl_masque, pa_masque);

   /*A Mise � jour du descripteur de socket � retourner */
   *pa_sock_client = vl_sock_client;

   /*A Ajout parametre pour que les process fils n'herite pas de la socket */
   fcntl(vl_sock_client, F_SETFD, 1);

   return (XDC_OK);
}
