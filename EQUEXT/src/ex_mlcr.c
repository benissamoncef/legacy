/*E*/
/*Fichier : $Id: ex_mlcr.c,v 1.37 2021/05/03 13:44:23 pc2dpdy Exp $      Release : $Revision: 1.37 $        Date : $Date: 2021/05/03 13:44:23 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  : EQUEXT
------------------------------------------------------
* MODULE COMMUN * FICHIER ex_mlcr.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module de traitement des demandes de la fenetre LCR.
*
------------------------------------------------------
* HISTORIQUE :
*
* Flaidar.A	version 1.1	10 Nov 1994	: Creation
* Mismer.D	version 1.2	07 Dec 1994	:
* Nagiel.E	version 1.3	08 Dec 1994	:
* Nagiel.E	version 1.4	16 Dec 1994	:
* Mismer.D	version 1.5	22 Dec 1994	:
* Mismer.D	version 1.6	22 Dec 1994	:
* Mismer.D	version 1.7	22 Dec 1994	:
* Nagiel.E	version 1.8 	23 Dec 1994	:
* Mismer.D	version 1.9	05 Jan 1995	:
* Mismer.D	version 1.10	06 Jan 1995	:
* Mismer.D	version 1.11	10 Jan 1995	:
* Mismer.D	version 1.12	16 Jan 1995	:
* Mismer.D	version 1.13	19 Jan 1995	:
* Mismer.D	version 1.14	09 Fev 1995	:
* Mismer.D	version 1.15	22 Mai 1995	: Ajout ecriture fichier trace des commandes LCR, reprise historique
* Mismer.D	version 1.16	30 Mai 1995	: Modif fonction lecture fichier LCR pour nom equipement
* Mismer.D	version 1.17	31 Mai 1995	: Modif taille nom equipement
* Mismer.D	version 1.18	22 Jun 1995	: Modif nom fichier systeme video
* Mismer.D	version 1.19	08 Aou 1995	: Modif nom equipement ETOR et analyseur
* Mismer.D	version 1.20	31 Aou 1995	: Modif trace FATAL si echec envoi trame vers TELMI
* Mismer.D	version 1.21	30 Oct 1995	: Modif parametre trace de envoi trame vers TELMI
* Mismer.D	version 1.22	09 Nov 1995	: Modif parametre trace de fonction fenetre LCR
* Mismer.D  	version 1.23	13 Sep 1996	: Ajout type machine (RADT_W)
* Mismer.D  	version 1.24	05 Dec 1996	: Modif pour meteo (DEM/1232)
* Mismer.D	version 1.25	10 Fev 1997	: Modif TDP (DEM/1395)
* Mismer.D	version 1.25	24 Fev 1997	: Modif TDP Suite (DEM/1395)
* Mismer.D      version 1.26    25 Mar 1997     : Modif TDP encore Suite  (DEM/1396)
* Mismer.D	version 1.27	15 Jan 1998	: Modif FAC et FAU (DEM/1544)
* JMG		version 1.28	11/10/05	: ajout PMVA BAF BAD
* JMG		version 1.29	22/06/10	: linux DEM/934
* JPL		24/02/11 : Migration architecture HP ia64 (DEM 975) : retour C.R. OK ou NOK pour plusieurs fonctions  1.30
* JPL		24/02/11 : Migration architecture HP ia64 (DEM 975) : scan numero eqt. en fichier analyseurs  1.31
* JPL		24/02/11 : Migration HP ia64 (DEM 975) : Gestion conversions types de tailles differentes (=,+)  1.32
* VR		23/12/11 : AJOUT PRV (DEM/1016)
* JPL		13/02/18 : Migration Linux 64 bits (DEM 1274) : initialisation variable  1.34
* JMG		15/05/18 : ajout IP LCR  et IMU 1.35
* ABE		16/02/21 : Modification IMU DEM-SAE155 1.36
* GGY    15/06/23 :  Correction ex_mlcr pour pilotage eqt IP (DEM471) (DEM472)
* ABK    29/08/23 : Ajout du nouvel equipement picto (DEM/483)
------------------------------------------------------*/

/* fichiers inclus */
#include <xzsem.h>

#include "ex_mlcr.h"
#include "etim_don.h"

/* definitions de constantes */

#define ELCRC_TailleFic 300000
#define ELCRC_DEBUT 1
#define ELCRC_FIN 2

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: ex_mlcr.c,v 1.37 2021/05/03 13:44:23 pc2dpdy Exp $ : ex_mlcr" ;
EXT_NOM_EQT_LCR *	pg_DebutNomPMV   = NULL;
EXT_NOM_EQT_LCR *	pg_DebutNomPCT 	 = NULL;
EXT_NOM_EQT_LCR *	pg_DebutNomDAI 	 = NULL;
EXT_NOM_EQT_LCR *	pg_DebutNomRAD 	 = NULL;
EXT_NOM_EQT_LCR *	pg_DebutNomNAV 	 = NULL;
EXT_NOM_EQT_LCR *	pg_DebutNomPAL 	 = NULL;
EXT_NOM_EQT_LCR *	pg_DebutNomGTC 	 = NULL;
EXT_NOM_EQT_LCR *	pg_DebutNomRAU 	 = NULL;
EXT_NOM_EQT_LCR *	pg_DebutNomPAU 	 = NULL;
EXT_NOM_EQT_LCR *	pg_DebutNomVID 	 = NULL;
EXT_NOM_EQT_LCR *	pg_DebutNomQTX 	 = NULL;
EXT_NOM_EQT_LCR *	pg_DebutNomTDP 	 = NULL;
EXT_NOM_EQT_LCR *	pg_DebutNomZDP 	 = NULL;
EXT_NOM_EQT_LCR *	pg_DebutNomPMVA	 = NULL;
EXT_NOM_EQT_LCR *	pg_DebutNomPRV	 = NULL;
EXT_NOM_EQT_LCR *	pg_DebutNomBAF 	 = NULL;
EXT_NOM_EQT_LCR *	pg_DebutNomBAD 	 = NULL;
EXT_NOM_EQT_LCR *	pg_DebutNomPIC 	 = NULL;

/* declaration de fonctions externes */
int vg_size_IP = 30;
typedef struct
{
   int Numero;
   XDY_AdresseIP AdresseIP;
   int Port;
} T_IP;
T_IP tg_IP[30] = {{0, "", 0}, {0, "", 0}, {0, "", 0}, {0, "", 0}, {0, "", 0}, {0, "", 0}, {0, "", 0}, {0, "", 0}, {0, "", 0}, {0, "", 0}, {0, "", 0}, {0, "", 0}, {0, "", 0}, {0, "", 0}, {0, "", 0}, {0, "", 0}, {0, "", 0}, {0, "", 0}, {0, "", 0}, {0, "", 0}, {0, "", 0}, {0, "", 0}, {0, "", 0}, {0, "", 0}, {0, "", 0}, {0, "", 0}, {0, "", 0}, {0, "", 0}, {0, "", 0}, {0, "", 0}};

/* definition de fonctions internes */

void ex_ecriture_resultat(INFO_EQT_FEN_LCR *, int);
int ex_ecriture_trace_lcr(XDY_Eqt, XDY_Eqt, char *, char *, char *, int);
void ex_transfert_ficres(INFO_EQT_FEN_LCR *,
                         INFO_EQT_FEN_LCR *,
                         INFO_EQT_FEN_LCR **,
                         int);
void ex_transfert_ficres_IP(INFO_EQT_FEN_LCR_IP *,
                            INFO_EQT_FEN_LCR_IP *,
                            INFO_EQT_FEN_LCR_IP **,
                            int);

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* Traitement des demandes fenetre LCR.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int ex_cmd_LCR(char *pa_NomFichCmdesLCR,
               XDY_NomMachine va_NomMachDist,
               XDY_TypeEqt va_typeeqt,
               XDY_Eqt va_noeqt,
               XDY_Rgs va_AdresseRGS,
               int va_Socket,
               char *pa_NomFichResLCR,
               XDY_Entier *va_Position,
               char *pa_TexteCmd)

/*
* ARGUMENTS EN ENTREE :
*
* pa_NomFichCmdesLCR	: nom fichier contenant les cmdes. de la fenetre LCR.
* va_NomMachDist	: nom machine distante sur laquelle se trouve le fichier cmdes. LCR.
* va_typeeqt		: Type d'equipement
* va_noeqt		: No d'equipement
* va_AdresseRGS		: Adresse rgs de l'eqt a piloter
* va_Socket		: Socket de l'eqt a piloter
* pa_NomFichResLCR	: nom fichier contenant le resultat de la fenetre LCR
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR :
* XDC_OK : Succes
* XDC_NOK : KO
*
* CONDITION D'UTILISATION
*
*
* FONCTION
* - decodage demande LCR
* - recuperation fichier cmdes. fenetre LCR.
* - ouverture/creation du fichier resultat.
* - envoi 1ere cmdes. LCR.
*
------------------------------------------------------*/
{
   static char *version = "$Id: ex_mlcr.c,v 1.37 2021/05/03 13:44:23 pc2dpdy Exp $ : ex_cmd_LCR";

   int vl_Fd;                         /* desc. fichier cmdes. fenetre LCR */
   XZSCT_NomMachine vl_MachineLocale; /* nom de la machine locale */
   char pl_NomFichier[256];
   XDY_Texte pl_LigneFich;
   XZSMT_Horodate vl_Horodate; /* horodate en format lisible */
   XDY_Horodate vl_HorodateSec;
   XZSMT_Horodate vl_Horodatedecompo;
   int vl_JourSem; /* jour de la semaine */
   int vl_indice;
   int vl_trouve = 0;
   XZST_03EcritureTrace(XZSTC_FONCTION, "IN:ex_cmd_LCR");

   /*A Si le fichier LCR n'existe pas -> transfert de celui-ci */

   sprintf(pl_NomFichier, "%s/%s", XZEXC_PATH_FIC_LCR, pa_NomFichCmdesLCR);

   /* SI recuperation nom de la machine locale OK */

   if (XZSC_07NomMachine(vl_MachineLocale) != XDC_OK)
      XZST_03EcritureTrace(XZSTC_DEBUG1, "Recup. nom machine locale impossible");

   XZST_03EcritureTrace(XZSTC_FONCTION, "Nom machine locale : %s/%s", vl_MachineLocale, va_NomMachDist);

    if ( !strcmp(vl_MachineLocale, va_NomMachDist) )
    {
       XZST_03EcritureTrace( XZSTC_FONCTION, "Pas de transfert : Fenetre LCR sur machine locale");
    }
    else
    {
       /*A transfert du fichier des cmdes. LCR */
       XZST_03EcritureTrace( XZSTC_INFO, "Transfert fichier cmdes. LCR <%s:%s/%s> : ", 
                                     va_NomMachDist, 
                                     XZEXC_PATH_FIC_LCR, 
                                     pa_NomFichCmdesLCR);
    		
       /* transfert de fichier cmdes LCR */
       XZSS_07TransfertFichier( 	va_NomMachDist, 
                                	pa_NomFichCmdesLCR,
                                	XZEXC_PATH_FIC_LCR ,
    					vl_MachineLocale, 
    					pa_NomFichCmdesLCR, 
    					XZEXC_PATH_FIC_LCR );

    }
    
    if ( access ( pl_NomFichier, F_OK ) )
    {
       XZST_03EcritureTrace( XZSTC_WARNING, "ex_cmd_LCR Transfert fichier LCR Echoue" );
       XZST_03EcritureTrace( XZSTC_FONCTION, "OUT : ex_cmd_LCR" );
       return(XDC_NOK);
    }
    
    /*A Ouverture du fichier lcr */
      
    if ( XZSS_12OuvrirFichier( O_RDONLY , pl_NomFichier, &vl_Fd ) != XDC_OK )
    {
       /* Ecriture trace absence fichier */
       XZST_03EcritureTrace( XZSTC_WARNING, "OUT : ex_cmd_LCR Manque fichier %s. \n", pl_NomFichier ); 
       /* Sortie de la fonction */
       XZST_03EcritureTrace( XZSTC_FONCTION, "OUT : ex_cmd_LCR" );
       return(XDC_NOK);
    }
 
    *va_Position = 0;   
    /*A Lecture 1iere commande du fichier */
    while ( ex_LireLigneFich ( vl_Fd, pl_LigneFich, sizeof ( pl_LigneFich ) ) != XZEXC_EOF ) 
    {
       /* Lire ligne suivante si c'est une ligne de commentaire */
       if ( *pl_LigneFich != XDF_COMMENTAIRE )
       {
          *va_Position += (int) strlen(pl_LigneFich) + 1;
          break;
       }
       else
          *va_Position += (int) strlen(pl_LigneFich) + 1;
    }
    /*A Fermeture du fichier lcr  */
    XZSS_13FermerFichier( vl_Fd );
    
    /*A Envoyer trame a la tache TELMi */
	XZST_03EcritureTrace( XZSTC_WARNING, "avant EnvoyerTrame");	


   /*si eqt IP*/
   vl_trouve = 0;
   /*recherche dans tg_IP (type, numero, adresseIP, port*/
   for (vl_indice = 0; vl_indice < vg_size_IP; vl_indice++)
   {
      if (tg_IP[vl_indice].Numero == va_noeqt)
      {
         XZST_03EcritureTrace(XZSTC_WARNING, "eqt trouve");
         vl_trouve = 1;
         break;
      }
   }

   /*si trouve, appeler EnvoyterTrameIP*/
   if (vl_trouve == 1)
   {
      if (EnvoyerTrameIP(pl_LigneFich,
                         XZEXC_TRAME_FEN_LCR,
                         tg_IP[vl_indice].AdresseIP,
                         tg_IP[vl_indice].Port,
                         va_AdresseRGS,
                         XZEXC_NOCMD,
                         XZEXC_PRIORITEFAIBLE,
                         va_Socket) == XDC_OK)
      {
         /* Ecriture de la commande dans le fichier resultat */
         strcpy(pa_TexteCmd, pl_LigneFich);
         XZST_03EcritureTrace(XZSTC_FONCTION, "OUT:ex_cmd_LCR trameIP");
         return (XDC_OK);
      }
      else
      {
         XZST_03EcritureTrace(XZSTC_WARNING, "OUT:ex_cmd_LCR Impossible d'emettre la trame vers TEMIP.");
         XZST_03EcritureTrace(XZSTC_FONCTION, "OUT:ex_cmd_LCR");
         return (XDC_NOK);
      }
   }
   else
   {
      if (EnvoyerTrame(pl_LigneFich,
                       XZEXC_TRAME_FEN_LCR,
                       va_AdresseRGS,
                       XZEXC_NOCMD,
                       XZEXC_PRIORITEFAIBLE,
                       va_Socket) == XDC_OK)
      {
         /* Ecriture de la commande dans le fichier resultat */
         strcpy(pa_TexteCmd, pl_LigneFich);
         XZST_03EcritureTrace(XZSTC_FONCTION, "OUT:ex_cmd_LCR");
         return (XDC_OK);
      }
      else
      {
         XZST_03EcritureTrace(XZSTC_WARNING, "OUT:ex_cmd_LCR Impossible d'emettre la trame vers TELMi.");
         XZST_03EcritureTrace(XZSTC_FONCTION, "OUT:ex_cmd_LCR");
         return (XDC_NOK);
      }
   }
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Envoi une trame a TELMi.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int EnvoyerTrame(XZEXT_Trame trame,
                 int type_msg,
                 char *adr_rgs,
                 int nocmd,
                 int priorite,
                 int socket)
/*
* ARGUMENTS EN ENTREE :
*  trame : trame a envoyer
*  type_msg : type du message
*  adr_rgs : adresse rgs
*  nocmd : numero de commande
*  priorite : priorite de la trame
*  socket : socket d'emision
*
* ARGUMENTS EN SORTIE :
*   aucun
*
*
* CODE RETOUR :
*   XDC_OK : succes
*   XDC_NOK : Erreur
*
* CONDITION D'UTILISATION
*
*   Fonction appele apres avoir construit la trame
*
* FONCTION
*  Demande d'envoi de trame a telmi
*
------------------------------------------------------*/
{
   static char *version = "$Id: ex_mlcr.c,v 1.37 2021/05/03 13:44:23 pc2dpdy Exp $ : EnvoyerTrame";

   XZEXT_MSG_SOCKET vl_message;
   ssize_t vl_RetCode = 0;

   /*A Construction de la trame */
   sprintf(vl_message, "%s%d %s %d %d %s%s %s",
           XZEXC_DEMANDE_LCR,
           type_msg,
           adr_rgs,
           nocmd,
           priorite,
           XZEXC_SEP_TRAME,
           trame,
           XZEXC_FIN_MSG_SOCK);

   fcntl(socket, F_SETFL, O_NDELAY | fcntl(socket, F_GETFL, 0));

   // sem_wait(&vg_semaphore);
   if ((vl_RetCode = write(socket, vl_message, strlen(vl_message))) < 0)
   {
      // sem_post(&vg_semaphore);
      /*B Ecriture dans le fichier trace en cas d'�chec */
      XZST_03EcritureTrace(XZSTC_FATAL, "Impossible d'emettre la trame serie vers TELMi.");
      return (XDC_NOK);
   }
   else
   { // sem_post(&vg_semaphore);
      /*B Ecriture dans le fichier trace en cas de succes */
      if (vl_RetCode != strlen(vl_message))
      {
         XZST_03EcritureTrace(XZSTC_WARNING, "Trame serie INCOMPLETE emise vers TELMi <%s>", vl_message);
         return (XDC_OK);
      }
      else
      {
         XZST_03EcritureTrace(XZSTC_INFO, "Trame serie emise vers TELMi  <%s>", vl_message);
         return (XDC_OK);
      }
   }
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* Traitement des demandes fenetre LCR.
*
------------------------------------------------------
*
* ARGUMENTS EN ENTREE :
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR :
* XDC_OK : Succes
* XDC_NOK : KO
*
* CONDITION D'UTILISATION
*
*
* FONCTION
* - decodage demande LCR
* - recuperation fichier cmdes. fenetre LCR.
* - ouverture/creation du fichier resultat.
* - envoi 1ere cmdes. LCR.
*
------------------------------------------------------*/
int ex_ret_LCR(char *va_trame,
               XDY_Rgs va_AdresseRGS,
               INFO_EQT_FEN_LCR **pg_DebutListeEqt,
               int va_TypeRep)
{
   INFO_EQT_FEN_LCR *pl_ListeEqt, *pl_eqtprec;
   XZSCT_NomMachine vl_MachineLocale; /* nom de la machine locale */
   char pl_NomFichier[256];
   int vl_Fd; /* desc. fichier  */
   int vl_cmd = XDC_FAUX;
   XDY_Texte pl_LigneFich;
   char vl_ficres[21];
   XDY_Mot vl_indiv;
   char vl_laCommande[XZSSC_LINE_CMD_SIZE] = "";
   FILE *vl_commande = NULL;
   int vl_Position;

   XZST_03EcritureTrace(XZSTC_FONCTION, "IN:ex_ret_LCR Trame reponse: %s %s", va_trame, va_AdresseRGS);

   /*A Recherche de l'eqt dans la liste */
   for (pl_eqtprec = pl_ListeEqt = *pg_DebutListeEqt;
        pl_ListeEqt != NULL;
        pl_eqtprec = pl_ListeEqt, pl_ListeEqt = pl_ListeEqt->Suivant)
   {
      XZST_03EcritureTrace(XZSTC_INFO, "ex_ret_LCR : %s %s", pl_ListeEqt->AdresseRGS, va_AdresseRGS);
      if (!strcmp(pl_ListeEqt->AdresseRGS, va_AdresseRGS))
         break;
   }
   if (pl_ListeEqt == NULL)
   {
      XZST_03EcritureTrace(XZSTC_WARNING, "OUT:ex_ret_LCR Eqt introuvable");
      return (XDC_NOK);
   }

   /*A Recherche s'il s'agit d'un bloc intermediaire : si oui fin traitement */

   vl_Position = (!strlen(pl_ListeEqt->TexteReponse)) ? ELCRC_DEBUT : ELCRC_FIN;

   strcpy(pl_ListeEqt->TexteReponse, va_trame);
   if (va_TypeRep == XZEXC_REP_NAK)
      strcpy(pl_ListeEqt->TexteReponse, "Reponse NAK");
   if (va_TypeRep == XZEXC_REP_ACK)
      strcpy(pl_ListeEqt->TexteReponse, "Reponse ACK");
   if (va_TypeRep == XZEXC_TRAME_NON_TRANSM)
      strcpy(pl_ListeEqt->TexteReponse, "Pas de reponse ");

   /*A Ecriture du resultat de la commande ds le fichier resultat */
   ex_ecriture_trace_lcr(pl_ListeEqt->TypeEqt,
                         pl_ListeEqt->NoEqt,
                         pl_ListeEqt->TexteCmd,
                         pl_ListeEqt->TexteReponse,
                         pl_ListeEqt->NomMachine,
                         vl_Position);

   ex_ecriture_resultat(pl_ListeEqt, vl_Position);

   if (va_TypeRep == XZEXC_REP_BLC_INT)
      return (XDC_OK);

   /* Positionnement du pointeur du fichier */
   if (strcmp(pl_ListeEqt->TexteReponse, XZEXC_DDE_ANNUL_CMD_LCR))
   {
      sprintf(pl_NomFichier, "%s/%s", XZEXC_PATH_FIC_LCR, pl_ListeEqt->NomFichCmdesLCR);

      XZST_03EcritureTrace(XZSTC_DEBUG1, "Ouverture fichier %s ", pl_NomFichier);
      if (((vl_Fd = open(pl_NomFichier, O_RDWR)) <= 0) ||
          (!lseek(vl_Fd, pl_ListeEqt->Position, SEEK_SET)))
      {
         XZST_03EcritureTrace(XZSTC_WARNING, "OUT:ex_ret_LCR Erreur sur fichier %s ", pl_NomFichier);
         ex_transfert_ficres(pl_ListeEqt, pl_eqtprec, pg_DebutListeEqt, XDC_NOK);
         close(vl_Fd);
         return (XDC_NOK);
      }

      /*A Lecture commande suivante du fichier */

      vl_cmd = XDC_FAUX;
      while (ex_LireLigneFich(vl_Fd, pl_LigneFich, sizeof(pl_LigneFich)) != XZEXC_EOF)
      {
         /* Lire ligne suivante si c'est une ligne de commentaire */
         if (*pl_LigneFich != XDF_COMMENTAIRE)
         {
            vl_cmd = XDC_VRAI;
            break;
         }
         else
         {
            pl_ListeEqt->Position += (int)strlen(pl_LigneFich);
         }
      }
      close(vl_Fd);
   }

   XZST_03EcritureTrace(XZSTC_WARNING, "Trame reponse LCR:%s (Cmd:%s)", pl_ListeEqt->TexteReponse, pl_ListeEqt->TexteCmd);

   if ((vl_cmd == XDC_FAUX) || (!strcmp(pl_ListeEqt->TexteReponse, XZEXC_DDE_ANNUL_CMD_LCR)))
   {
      XZST_03EcritureTrace(XZSTC_DEBUG1, "Fin du fichier de commande atteinte Pos:%d ", pl_ListeEqt->Position);
      if ((pl_ListeEqt->Periode == 0) || (!strcmp(pl_ListeEqt->TexteReponse, XZEXC_DDE_ANNUL_CMD_LCR)))
      {
         vl_indiv = pl_ListeEqt->Indiv;
         /* Preparation de la commande cat */
         sprintf(vl_laCommande, "cat %s/%s_%d >> %s/%s",
                 XZEXC_PATH_FIC_LCR,
                 pl_ListeEqt->NomFichResLCR,
                 pl_ListeEqt->NoEqt,
                 XZEXC_PATH_FIC_LCR,
                 pl_ListeEqt->NomFichResLCR);
         /*  Execution !	*/
         vl_commande = popen(vl_laCommande, "r");
         if (vl_commande != NULL)
            pclose(vl_commande);
         sprintf(vl_laCommande, "%s/%s_%d", XZEXC_PATH_FIC_LCR, vl_ficres, pl_ListeEqt->NoEqt);
         XZSS_06DetruireFichier(vl_laCommande);

         /* Si travail termin� -> transfert du fichier resultat */
         if ((vl_indiv == XDC_VRAI) || ((vl_indiv == XDC_FAUX) && ((*pg_DebutListeEqt)->Suivant == NULL)))
         {
            ex_transfert_ficres(pl_ListeEqt, pl_eqtprec, pg_DebutListeEqt, XDC_OK);
         }
         else
         {
            if ((pl_eqtprec == *pg_DebutListeEqt) && (pl_ListeEqt == pl_eqtprec))
               *pg_DebutListeEqt = pl_ListeEqt->Suivant;
            else
               pl_eqtprec->Suivant = pl_ListeEqt->Suivant;
            free(pl_ListeEqt);
         }
      }
      else
      {
         pl_ListeEqt->Position = 0;
      }
   }
   else
   {
      /*A Envoyer trame a la tache TELMi */

      if (XZSC_07NomMachine(vl_MachineLocale) != XDC_OK)
         XZST_03EcritureTrace(XZSTC_DEBUG1, "Recup. nom machine locale impossible");

      strcpy(pl_ListeEqt->TexteReponse, "");
      strcpy(pl_ListeEqt->TexteCmd, pl_LigneFich);
      if (EnvoyerTrame(pl_LigneFich,
                       XZEXC_TRAME_FEN_LCR,
                       pl_ListeEqt->AdresseRGS,
                       XZEXC_NOCMD,
                       XZEXC_PRIORITEFAIBLE,
                       pl_ListeEqt->Socket) == XDC_OK)
      {
         /* Ecriture de la commande dans le fichier resultat */
         pl_ListeEqt->Position = pl_ListeEqt->Position + (int)(strlen(pl_LigneFich) + 1);
         XZST_03EcritureTrace(XZSTC_FONCTION, "OUT:ex_ret_LCR");
         return (XDC_OK);
      }
      else
      {
         /* Ecriture de la commande dans le fichier resultat */
         strcpy ( pl_ListeEqt->TexteReponse, "Impossible de transmettre la commande" );
         ex_ecriture_resultat ( pl_ListeEqt, ELCRC_DEBUT );
         ex_transfert_ficres ( pl_ListeEqt, pl_eqtprec, pg_DebutListeEqt, XDC_OK );
         XZST_03EcritureTrace( XZSTC_FONCTION, "OUT:ex_cmd_LCR Impossible d'emettre la trame vers TELMi." );
         return(XDC_NOK);
      }
   }
   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT:ex_ret_LCR");
   return (XDC_OK);
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* Insere un eqt dans la liste des eqt pilotes par la fenetre lcr
*
------------------------------------------------------
*
* ARGUMENTS EN ENTREE :
* pa_Eqt : infos de l'eqt
* pg_DebutListeEqt : Pointeur du debut de liste des eqt pilotes par la fenetre lcr
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR :
* XDC_OK : Succes
* XDC_NOK : KO
*
* CONDITION D'UTILISATION
*
*
* FONCTION
*
------------------------------------------------------*/
int ex_ins_eqt_liste(INFO_EQT_FEN_LCR *pa_Eqt, INFO_EQT_FEN_LCR **pg_DebutListeEqt)
{
   INFO_EQT_FEN_LCR *pl_ListeEqt, *pl_NewEqt;

   XZST_03EcritureTrace(XZSTC_FONCTION, "IN:ex_ins_eqt_liste");

   /*A Recherche de l'eqt dans la liste */
   for (pl_ListeEqt = *pg_DebutListeEqt; pl_ListeEqt != NULL; pl_ListeEqt = pl_ListeEqt->Suivant)
   {
      if (pl_ListeEqt->NoEqt == pa_Eqt->NoEqt)
         break;
   }
   if (pl_ListeEqt != NULL)
   {
      XZST_03EcritureTrace(XZSTC_FONCTION, "OUT:ex_ins_eqt_liste  L'eqt existe deja");
      return (XDC_NOK);
   }

   if ((pl_NewEqt = (INFO_EQT_FEN_LCR *)malloc(sizeof(INFO_EQT_FEN_LCR))) == NULL)
   {
      XZST_03EcritureTrace(XZSTC_FONCTION, "OUT:ex_ins_eqt_liste  Malloc echoue");
      return (XDC_NOK);
   }

   memcpy(pl_NewEqt, pa_Eqt, sizeof(INFO_EQT_FEN_LCR));

   if (*pg_DebutListeEqt == NULL)
   {
      *pg_DebutListeEqt = pl_NewEqt;
   }
   else
   {
      for (pl_ListeEqt = *pg_DebutListeEqt; pl_ListeEqt != NULL; pl_ListeEqt = pl_ListeEqt->Suivant)
      {
         if (pl_ListeEqt->Suivant == NULL)
            break;
      }
      pl_NewEqt->Suivant = NULL;
      pl_ListeEqt->Suivant = pl_NewEqt;
   }

   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT:ex_ins_eqt_liste");
   return (XDC_OK);
}

void ex_transfert_ficres(INFO_EQT_FEN_LCR *pa_InfoEqt,
                         INFO_EQT_FEN_LCR *pa_EqtPrec,
                         INFO_EQT_FEN_LCR **pa_DebutListeEqt,
                         int va_CodeRetour)

{
   XZSCT_NomMachine vl_MachineLocale; /* nom de la machine locale */

   /*A transfert du fichier des cmdes. LCR */
   XZSC_07NomMachine(vl_MachineLocale);
   XZST_03EcritureTrace(XZSTC_INFO, "Transfert fichier cmdes. LCR <%s:%s/%s> : ",
                        pa_InfoEqt->NomMachine,
                        vl_MachineLocale,
                        pa_InfoEqt->NomFichResLCR);

   XZSS_07TransfertFichier(vl_MachineLocale,
                           pa_InfoEqt->NomFichResLCR,
                           XZEXC_PATH_FIC_LCR,
                           pa_InfoEqt->NomMachine,
                           pa_InfoEqt->NomFichResLCR,
                           XZEXC_PATH_FIC_LCR);

   XZEX02_Retour_LCR(va_CodeRetour, pa_InfoEqt->NomMachine);

   if ((pa_EqtPrec == *pa_DebutListeEqt) && (pa_InfoEqt == pa_EqtPrec))
      *pa_DebutListeEqt = pa_InfoEqt->Suivant;
   else
      pa_EqtPrec->Suivant = pa_InfoEqt->Suivant;
   free(pa_InfoEqt);
}
void ex_transfert_ficres_IP(INFO_EQT_FEN_LCR_IP *pa_InfoEqt,
                            INFO_EQT_FEN_LCR_IP *pa_EqtPrec,
                            INFO_EQT_FEN_LCR_IP **pa_DebutListeEqt,
                            int va_CodeRetour)

{
   XZSCT_NomMachine vl_MachineLocale; /* nom de la machine locale */

   /*A transfert du fichier des cmdes. LCR */
   XZSC_07NomMachine(vl_MachineLocale);
   XZST_03EcritureTrace(XZSTC_INFO, "Transfert fichier cmdes. LCR <%s:%s/%s> : ",
                        pa_InfoEqt->NomMachine,
                        vl_MachineLocale,
                        pa_InfoEqt->NomFichResLCR);

   XZSS_07TransfertFichier(vl_MachineLocale,
                           pa_InfoEqt->NomFichResLCR,
                           XZEXC_PATH_FIC_LCR,
                           pa_InfoEqt->NomMachine,
                           pa_InfoEqt->NomFichResLCR,
                           XZEXC_PATH_FIC_LCR);

   XZEX02_Retour_LCR(va_CodeRetour, pa_InfoEqt->NomMachine);

   if ((pa_EqtPrec == *pa_DebutListeEqt) && (pa_InfoEqt == pa_EqtPrec))
      *pa_DebutListeEqt = pa_InfoEqt->Suivant;
   else
      pa_EqtPrec->Suivant = pa_InfoEqt->Suivant;
   free(pa_InfoEqt);
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* Relance un fichier LCR en fonctionnement periodique
*
------------------------------------------------------
*
* ARGUMENTS EN ENTREE :
* pa_Eqt : infos de l'eqt
* pg_DebutListeEqt : Pointeur du debut de liste des eqt pilotes par la fenetre lcr
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR :
* XDC_OK : Succes
* XDC_NOK : KO
*
* CONDITION D'UTILISATION
*
*
* FONCTION
*
------------------------------------------------------*/
int ex_relance_lcr(XDY_Eqt va_NumEqt,
                   char va_Cas,
                   INFO_EQT_FEN_LCR **pg_DebutListeEqt,
                   XDY_Horodate va_Horodate,
                   XZEXT_CLE_PROG pa_CleProg,
                   int va_SockTIM)
{
   INFO_EQT_FEN_LCR *pl_ListeEqt;
   XZEXT_MSG_SOCKET pl_MsgTimer;
   XZEXT_CLE_PROG pl_CleProg;
   int vl_trouve, vl_indice;

   XZST_03EcritureTrace(XZSTC_FONCTION, "IN: ex_relance_lcr ");
   /*A Recherche de l'eqt dans la liste */
   for (pl_ListeEqt = *pg_DebutListeEqt; pl_ListeEqt != NULL; pl_ListeEqt = pl_ListeEqt->Suivant)
   {
      if (pl_ListeEqt->NoEqt == va_NumEqt)
         break;
   }
   if (pl_ListeEqt == NULL)
   {
      sprintf(pl_CleProg, "%s%c", pa_CleProg, 'C');
      ETIM_ANNUL_REVEIL(pl_ListeEqt->Periode, pa_CleProg, va_SockTIM, pl_MsgTimer)
      return (XDC_OK);
   }

   if (pl_ListeEqt->HrFin < (va_Horodate + (double)pl_ListeEqt->Periode))
   {
      if (va_Cas == 'C')
      {
         sprintf(pl_CleProg, "%s%c", pa_CleProg, 'C');
         ETIM_ANNUL_REVEIL(pl_ListeEqt->Periode, pa_CleProg, va_SockTIM, pl_MsgTimer)
         sprintf(pl_CleProg, "%s%c", pa_CleProg, 'D');
         ETIM_ANNUL_REVEIL(pl_ListeEqt->Periode, pa_CleProg, va_SockTIM, pl_MsgTimer)
      }
      pl_ListeEqt->Periode = 0;
   }
   else
   {
      if (va_Cas == 'D')
      {
         sprintf(pl_CleProg, "%s%c", pa_CleProg, 'C');
         ETIM_REVEIL_CYCLIQUE(pl_ListeEqt->Periode, pa_CleProg, va_SockTIM, pl_MsgTimer)
      }
   }

   /*Recherche si eqt IP*/
   vl_trouve = 0;
   /*recherche dans tg_IP (type, numero, adresseIP, port*/
   for (vl_indice = 0; vl_indice < vg_size_IP; vl_indice++)
   {
      if (tg_IP[vl_indice].Numero == va_NumEqt)
      {
         vl_trouve = 1;
         break;
      }
   }
   strcpy(pl_ListeEqt->TexteReponse, "");

   /*si trouve, appeler ex_cmd_LCR_IP*/
   if (vl_trouve == 1)
   {
      ex_cmd_LCR_IP(pl_ListeEqt->NomFichCmdesLCR,
                    pl_ListeEqt->NomMachine,
                    pl_ListeEqt->TypeEqt,
                    pl_ListeEqt->NoEqt,
                    pl_ListeEqt->AdresseRGS,
                    pl_ListeEqt->Socket,
                    pl_ListeEqt->NomFichResLCR,
                    &pl_ListeEqt->Position,
                    pl_ListeEqt->TexteCmd,
                    tg_IP[vl_indice].AdresseIP,
                    tg_IP[vl_indice].Port);
   }
   else
   {
      ex_cmd_LCR(pl_ListeEqt->NomFichCmdesLCR,
                 pl_ListeEqt->NomMachine,
                 pl_ListeEqt->TypeEqt,
                 pl_ListeEqt->NoEqt,
                 pl_ListeEqt->AdresseRGS,
                 pl_ListeEqt->Socket,
                 pl_ListeEqt->NomFichResLCR,
                 &pl_ListeEqt->Position,
                 pl_ListeEqt->TexteCmd);
   }
   return (XDC_OK);
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* Relance un fichier LCR en fonctionnement periodique
*
------------------------------------------------------
*
* ARGUMENTS EN ENTREE :
* pa_Eqt : infos de l'eqt
* pg_DebutListeEqt : Pointeur du debut de liste des eqt pilotes par la fenetre lcr
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR :
* XDC_OK : Succes
* XDC_NOK : KO
*
* CONDITION D'UTILISATION
*
*
* FONCTION
*
------------------------------------------------------*/
int ex_test_autorisation_eqt(INFO_EQT_FEN_LCR **pg_DebutListeEqt,
                             INFO_EQT_FEN_LCR *va_Eqt,
                             int va_Type)
{
   INFO_EQT_FEN_LCR *pl_ListeEqt;
   char vl_laCommande[XZSSC_LINE_CMD_SIZE] = "";
   FILE *vl_commande = NULL;

   XZST_03EcritureTrace(XZSTC_FONCTION, "IN: ex_test_autorisation_eqt ");
   va_Eqt->Position = 0;
   va_Eqt->Suivant = NULL;
   if (va_Eqt->NoEqt == XDC_EQTALL)
   {
      va_Eqt->Indiv = XDC_FAUX;
      if ((va_Eqt->Periode != XZEXC_ANNUL_CMD_LCR) &&
          ((*pg_DebutListeEqt != NULL) && (va_Eqt->TypeEqt == va_Type)))
      {
         XZEX02_Retour_LCR(XDC_NOK, va_Eqt->NomMachine);
         XZST_03EcritureTrace(XZSTC_WARNING, "OUT: ex_test_autorisation_eqt XDC_NOK 1");
         return (XDC_NOK);
      }
      if (va_Eqt->Periode != XZEXC_ANNUL_CMD_LCR)
      {
         /* destruction du fichier resultat s'il existe deja */
         sprintf(vl_laCommande, "rm %s/%s*", XZEXC_PATH_FIC_LCR, va_Eqt->NomFichResLCR);
         /*  Execution !	*/
         vl_commande = popen(vl_laCommande, "r");
         if (vl_commande != NULL)
            pclose(vl_commande);
      }
   }
   else
   {
      va_Eqt->Indiv = XDC_VRAI;
      /*A Recherche de l'eqt dans la liste */
      for (pl_ListeEqt = *pg_DebutListeEqt;
           (pl_ListeEqt != NULL) && (va_Eqt->TypeEqt == va_Type);
           pl_ListeEqt = pl_ListeEqt->Suivant)
      {
         if ((pl_ListeEqt->NoEqt == va_Eqt->NoEqt) && (va_Eqt->Periode != XZEXC_ANNUL_CMD_LCR))
         {
            XZEX02_Retour_LCR(XDC_NOK, va_Eqt->NomMachine);
            XZST_03EcritureTrace(XZSTC_WARNING, "OUT: ex_test_autorisation_eqt XDC_NOK 2");
            return (XDC_NOK);
         }
      }
      if (va_Eqt->Periode != XZEXC_ANNUL_CMD_LCR)
      {
         /* destruction du fichier resultat s'il existe deja */
         sprintf(vl_laCommande, "%s/%s", XZEXC_PATH_FIC_LCR, va_Eqt->NomFichResLCR);
         XZSS_06DetruireFichier(vl_laCommande);

         sprintf(vl_laCommande, "%s/%s_%d", XZEXC_PATH_FIC_LCR, va_Eqt->NomFichResLCR, va_Eqt->NoEqt);
         XZSS_06DetruireFichier(vl_laCommande);
      }
   }

   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: ex_test_autorisation_eqt ");
   return (XDC_OK);
}

void ex_init_ip()
{
   int vl_indice;
   for (vl_indice = 0; vl_indice < vg_size_IP; vl_indice++)
   {
      tg_IP[vl_indice].Numero = 0;
      tg_IP[vl_indice].Port = 0;
      strcpy(tg_IP[vl_indice].AdresseIP, "");
   }
}
void ex_init_lcr_ip(INFO_EQT_FEN_LCR **pa_DebutListeEqt,
                    INFO_EQT_FEN_LCR va_Eqt,
                    int va_Socket,
                    XDY_Rgs va_Rgs,
                    XDY_Eqt va_NumEqt,
                    char va_Cle[20],
                    int va_SockTim,
                    char *va_adresseIP,
                    int va_port)
{

   int vl_indice = 0;

   /*stockage dans tg_IP[]*/
   for (vl_indice = 0; vl_indice < vg_size_IP; vl_indice++)
   {
      XZST_03EcritureTrace(XZSTC_WARNING, "tg_IP[vl_indice].Numero %d", tg_IP[vl_indice].Numero);
      if (tg_IP[vl_indice].Numero == 0)
      {
         XZST_03EcritureTrace(XZSTC_WARNING, "ajout eqt %s %d", va_adresseIP, va_port);
         tg_IP[vl_indice].Numero = va_NumEqt;
         strcpy(tg_IP[vl_indice].AdresseIP, va_adresseIP);
         tg_IP[vl_indice].Port = va_port;
         break;
      }
   }
   ex_init_lcr(pa_DebutListeEqt, va_Eqt, va_Socket, va_Rgs, va_NumEqt, va_Cle, va_SockTim);
}

void ex_init_lcr(INFO_EQT_FEN_LCR **pa_DebutListeEqt,
                 INFO_EQT_FEN_LCR va_Eqt,
                 int va_Socket,
                 XDY_Rgs va_Rgs,
                 XDY_Eqt va_NumEqt,
                 char va_Cle[20],
                 int va_SockTim)
{
   XZEXT_MSG_SOCKET pl_MsgTimer;
   XZEXT_CLE_PROG vl_clef;
   INFO_EQT_FEN_LCR vl_Eqt;
   int vl_js;
   XDY_Horodate vl_horodate;
   int vl_trouve, vl_indice;

   memcpy(&vl_Eqt, &va_Eqt, sizeof(INFO_EQT_FEN_LCR));
   vl_Eqt.Socket = va_Socket;
   strcpy(vl_Eqt.AdresseRGS, va_Rgs);
   vl_Eqt.NoEqt = va_NumEqt;
   strcpy(vl_Eqt.TexteReponse, "");
   XZST_03EcritureTrace(XZSTC_DEBUG1, "Periode :%d ", vl_Eqt.Periode);

   if (vl_Eqt.Periode == 0)
   {

      /*Recherche si eqt IP*/
      vl_trouve = 0;
      /*recherche dans tg_IP (type, numero, adresseIP, port*/
      for (vl_indice = 0; vl_indice < vg_size_IP; vl_indice++)
      {
         if (tg_IP[vl_indice].Numero == vl_Eqt.NoEqt)
         {
            vl_trouve = 1;
            break;
         }
      }

      /*si trouve, appeler ex_cmd_LCR_IP*/
      if (vl_trouve == 1)
      {
         if (ex_cmd_LCR_IP(vl_Eqt.NomFichCmdesLCR,
                           vl_Eqt.NomMachine,
                           vl_Eqt.TypeEqt,
                           vl_Eqt.NoEqt,
                           vl_Eqt.AdresseRGS,
                           vl_Eqt.Socket,
                           vl_Eqt.NomFichResLCR,
                           &vl_Eqt.Position,
                           vl_Eqt.TexteCmd,
                           tg_IP[vl_indice].AdresseIP,
                           tg_IP[vl_indice].Port) == XDC_NOK)
         {
            XZEX02_Retour_LCR(XDC_NOK, vl_Eqt.NomMachine);
         }
         else
            ex_ins_eqt_liste(&vl_Eqt, pa_DebutListeEqt);
      }
      else
      {
         if (ex_cmd_LCR(vl_Eqt.NomFichCmdesLCR,
                        vl_Eqt.NomMachine,
                        vl_Eqt.TypeEqt,
                        vl_Eqt.NoEqt,
                        vl_Eqt.AdresseRGS,
                        vl_Eqt.Socket,
                        vl_Eqt.NomFichResLCR,
                        &vl_Eqt.Position,
                        vl_Eqt.TexteCmd) == XDC_NOK)
         {
            XZEX02_Retour_LCR(XDC_NOK, vl_Eqt.NomMachine);
         }
         else
            ex_ins_eqt_liste(&vl_Eqt, pa_DebutListeEqt);
      }
   }
   else
   {
      if (vl_Eqt.Periode == XZEXC_ANNUL_CMD_LCR)
      {
         ex_ret_LCR(XZEXC_DDE_ANNUL_CMD_LCR, vl_Eqt.AdresseRGS, pa_DebutListeEqt, XZEXC_REP_BLC_FIN);
         sprintf(vl_clef, "%sC%d", va_Cle, vl_Eqt.NoEqt);
         ETIM_ANNUL_REVEIL(vl_Eqt.Periode, vl_clef, va_SockTim, pl_MsgTimer)
      }
      else
      {
         XZSM_07LireHorodate(XZSMC_TYPE_RTWORKS, &vl_js, &vl_horodate);
         if (vl_Eqt.HrDebut < vl_horodate)
         {
            /*Recherche si eqt IP*/
            vl_trouve = 0;
            /*recherche dans tg_IP (type, numero, adresseIP, port*/
            for (vl_indice = 0; vl_indice < vg_size_IP; vl_indice++)
            {
               if (tg_IP[vl_indice].Numero == vl_Eqt.NoEqt)
               {
                  vl_trouve = 1;
                  break;
               }
            }
            /*si trouve, appeler ex_cmd_LCR_IP*/
            if (vl_trouve == 1)
            {
               if (ex_cmd_LCR_IP(vl_Eqt.NomFichCmdesLCR,
                                 vl_Eqt.NomMachine,
                                 vl_Eqt.TypeEqt,
                                 vl_Eqt.NoEqt,
                                 vl_Eqt.AdresseRGS,
                                 vl_Eqt.Socket,
                                 vl_Eqt.NomFichResLCR,
                                 &vl_Eqt.Position,
                                 vl_Eqt.TexteCmd,
                                 tg_IP[vl_indice].AdresseIP,
                                 tg_IP[vl_indice].Port) == XDC_NOK)
               {
                  XZEX02_Retour_LCR(XDC_NOK, vl_Eqt.NomMachine);
               }
               else
               {
                  /* Programmation timer cyclique */
                  sprintf(vl_clef, "%sC%d", va_Cle, vl_Eqt.NoEqt);
                  ETIM_REVEIL_CYCLIQUE(vl_Eqt.Periode, vl_clef, va_SockTim, pl_MsgTimer)
                  ex_ins_eqt_liste(&vl_Eqt, pa_DebutListeEqt);
               }
            }
            else
            {
               if (ex_cmd_LCR(vl_Eqt.NomFichCmdesLCR,
                              vl_Eqt.NomMachine,
                              vl_Eqt.TypeEqt,
                              vl_Eqt.NoEqt,
                              vl_Eqt.AdresseRGS,
                              vl_Eqt.Socket,
                              vl_Eqt.NomFichResLCR,
                              &vl_Eqt.Position,
                              vl_Eqt.TexteCmd) == XDC_NOK)
               {
                  XZEX02_Retour_LCR(XDC_NOK, vl_Eqt.NomMachine);
               }
               else
               {
                  /* Programmation timer cyclique */
                  sprintf(vl_clef, "%sC%d", va_Cle, vl_Eqt.NoEqt);
                  ETIM_REVEIL_CYCLIQUE(vl_Eqt.Periode, vl_clef, va_SockTim, pl_MsgTimer)
                  ex_ins_eqt_liste(&vl_Eqt, pa_DebutListeEqt);
               }
            }
         }
         else
         {
            /* Programmation timer a l'heure de debut */
            sprintf(vl_clef, "%sD%d", va_Cle, vl_Eqt.NoEqt);
            ETIM_PROG_REVEIL(vl_Eqt.HrDebut, vl_clef, va_SockTim, pl_MsgTimer)
            ex_ins_eqt_liste(&vl_Eqt, pa_DebutListeEqt);
         }
      }
   }
}
/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Lecture du fichier Nom Eqt LCR.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void ex_ecriture_resultat(INFO_EQT_FEN_LCR *pa_InfoEqt,
                          int va_Position)

/*
* ARGUMENTS EN ENTREE :
*
*  aucun
*
* ARGUMENTS EN SORTIE :
*  aucun
*
* CODE RETOUR :
*  XDC_OK / XDC_NOK
*
* CONDITION D'UTILISATION
*
* FONCTION
*
*
------------------------------------------------------*/
{
   static char *version = "$Id: ex_mlcr.c,v 1.37 2021/05/03 13:44:23 pc2dpdy Exp $ :  ex_ecriture_resultat ";
   int vl_Fd; /* desc. fichier cmdes. fenetre LCR */
   char pl_NomFichier[128] = "";
   XZEXT_TRAME_PS_NAV pl_LigneFich = "";
   char vl_NomEqt[48] = "";
   XDY_NomMachine vl_NomMachine = "";
   XZSMT_Horodate pl_StrHorodate;
   XDY_Horodate vl_Horodate;
   int vl_JourSem; /* jour de la semaine */
   int vl_CptCar = 0;

   sprintf(pl_NomFichier, "%s/%s_%d", XZEXC_PATH_FIC_LCR, pa_InfoEqt->NomFichResLCR, pa_InfoEqt->NoEqt);
   if (!access(pl_NomFichier, F_OK))
   {
      vl_Fd = open(pl_NomFichier, O_RDWR | O_APPEND);
   }
   else
   {
      /*A Creation du fichier resultat */
      vl_Fd = open(pl_NomFichier, O_RDWR | O_CREAT, S_IRUSR | S_IRGRP | S_IWUSR | S_IWGRP | S_IROTH);
   }

   /* Enregistrement de la commande */
   if (va_Position == ELCRC_DEBUT)
   {
      ex_nom_equipement(pa_InfoEqt->TypeEqt, pa_InfoEqt->NoEqt, NULL, vl_NomEqt, vl_NomMachine);
      XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME, &vl_JourSem, &vl_Horodate);
      XZSM_11ConversionHorodate(vl_Horodate, &pl_StrHorodate);
      vl_CptCar = sprintf(pl_LigneFich, "\n# %s %s\n%s\n",
                          vl_NomEqt,
                          pl_StrHorodate,
                          pa_InfoEqt->TexteCmd);
   }

   sprintf(pl_LigneFich + vl_CptCar, "%s\n", pa_InfoEqt->TexteReponse);

   write(vl_Fd, pl_LigneFich, strlen(pl_LigneFich));
   write(vl_Fd, "\n", 1);
   close(vl_Fd);
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Lecture du fichier Nom Eqt LCR.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int ex_nom_equipement(XDY_Eqt va_TypeEqt,
                      XDY_Eqt va_NumEqt,
                      XDY_NomMachine va_NomMachine,
                      char *pa_NomEqt,
                      char *pa_NomMachine)

/*
* ARGUMENTS EN ENTREE :
*
*  aucun
*
* ARGUMENTS EN SORTIE :
*  aucun
*
* CODE RETOUR :
*  XDC_OK / XDC_NOK
*
* CONDITION D'UTILISATION
*
* FONCTION
*
*
------------------------------------------------------*/
{
   static char *version = "$Id: ex_mlcr.c,v 1.37 2021/05/03 13:44:23 pc2dpdy Exp $ : ex_nom_equipement ";

   int vl_Fd;
   char pl_PathEtFic[XDC_PATH_ABS_SIZE];
   XDY_Texte pl_LigneFich;
   EXT_NOM_EQT_LCR vl_NomEqt = {0},
                   *pl_DonCrt = NULL,
                   *pl_DebutNomLCR = NULL,
                   *pl_NewEqtLCR = NULL,
                   *pl_ExEqtLCR = NULL;
   XDY_NomEqt vl_NomTypeEqt = "";
   XDY_Mot vl_NumeroDAI = 0,
           vl_NumeroNAV = 0,
           vl_NumeroBAF = 0,
           vl_NumeroBAD = 0,
           vl_NoAnaly = 0,
           vl_NumEqt = 0,
           vl_NoModule = 0;

   strcpy(pa_NomEqt, "");

   /*A Lecture du nom du type d'equipement correspondant */
   XZEX03_nom_eqt((int)va_TypeEqt, vl_NomTypeEqt);
   if (va_NomMachine != NULL)
   {
      strcpy(pa_NomMachine, va_NomMachine);
      if (!strcmp(vl_NomTypeEqt, XZECC_NOM_TEL))
         strcpy(pa_NomEqt, XDC_LIB_MAA);
      if (!strcmp(vl_NomTypeEqt, XZECC_NOM_NIC))
         strcpy(pa_NomEqt, XDC_LIB_NIC);
      if (!strcmp(vl_NomTypeEqt, XZECC_NOM_SAT))
         strcpy(pa_NomEqt, XDC_LIB_SAT);
      if (!strcmp(vl_NomTypeEqt, XZECC_NOM_OPE))
         strcpy(pa_NomEqt, XDC_LIB_OPE);
      if (!strcmp(vl_NomTypeEqt, XZECC_NOM_CAP))
         strcpy(vl_NomTypeEqt, XDC_LIB_CAP);
      if (strlen(pa_NomEqt) > 0)
         return (XDC_OK);
   }
   switch (va_TypeEqt)
   {
      case XZECC_TYPE_EQT_RAU :
         sprintf ( pl_PathEtFic, "%s/%s_LCR", XZEXC_PATH_FIC_LCR, XDF_Config_RAU );
         pl_DebutNomLCR = pg_DebutNomRAU;
         break;
      case XDC_EQT_PAU :
         sprintf ( pl_PathEtFic, "%s/%s_%s", XZEXC_PATH_FIC_LCR, XDF_Config_PAU, va_NomMachine );
         pl_DebutNomLCR = pg_DebutNomPAU;
         break;
      case XDC_EQT_GEN_LCR :
         sprintf ( pl_PathEtFic, "%s/%s_LCR", XZEXC_PATH_FIC_LCR, XDF_Config_EQT_GEN_LCR );
         pl_DebutNomLCR = pg_DebutNomQTX;
         break;
      case XDC_EQT_TDP :
         sprintf ( pl_PathEtFic, "%s/%s_LCR", XZEXC_PATH_FIC_LCR, XDF_Config_TDP );
         pl_DebutNomLCR = pg_DebutNomTDP;
         break;
      case XDC_EQT_ZDP :
         pl_DebutNomLCR = pg_DebutNomZDP;
         break;
      case XZECC_TYPE_EQT_VID :
         sprintf ( pl_PathEtFic, "%s/%s_LCR", XZEXC_PATH_FIC_LCR, XDF_Config_SYSVIDEO );
         pl_DebutNomLCR = pg_DebutNomVID;
         break;
      case XZECC_TYPE_EQT_RDT :
         sprintf ( pl_PathEtFic, "%s/%s_LCR", XZEXC_PATH_FIC_LCR, XDF_Config_RADT );
         pl_DebutNomLCR = pg_DebutNomRAD;
         break;
      case XZECC_TYPE_EQT_DAI :
         sprintf ( pl_PathEtFic, "%s/%s_LCR", XZEXC_PATH_FIC_LCR, XDF_Config_DAI );
         pl_DebutNomLCR = pg_DebutNomDAI;
         break;
      case XZECC_TYPE_EQT_PMV :
         sprintf ( pl_PathEtFic, "%s/%s_LCR", XZEXC_PATH_FIC_LCR, XDF_Config_PMV );
         pl_DebutNomLCR = pg_DebutNomPMV;
         break;
      case XZECC_TYPE_EQT_PMVA :
         sprintf ( pl_PathEtFic, "%s/%s_LCR", XZEXC_PATH_FIC_LCR, XDF_Config_PMVA );
         pl_DebutNomLCR = pg_DebutNomPMVA;
         break;
	  case XZECC_TYPE_EQT_PRV :
         sprintf ( pl_PathEtFic, "%s/%s_LCR", XZEXC_PATH_FIC_LCR, XDF_Config_PRV );
         pl_DebutNomLCR = pg_DebutNomPRV;
         break;
      case XZECC_TYPE_EQT_PIC :
         sprintf ( pl_PathEtFic, "%s/%s_LCR", XZEXC_PATH_FIC_LCR, XDF_Config_PIC );
         pl_DebutNomLCR = pg_DebutNomPIC;
         break;
      case XZECC_TYPE_EQT_PICTO :
         sprintf ( pl_PathEtFic, "%s/%s_LCR", XZEXC_PATH_FIC_LCR, XDF_Config_PICTO );
         pl_DebutNomLCR = pg_DebutNomPCT;
         break;
      case XZECC_TYPE_EQT_NAV :
      case XDC_EQT_FAU :
      case XDC_EQT_FAC :
         sprintf ( pl_PathEtFic, "%s/%s_LCR", XZEXC_PATH_FIC_LCR, XDF_Config_NAV );
         pl_DebutNomLCR = pg_DebutNomNAV;
         break;
      case XZECC_TYPE_EQT_PAL :
         sprintf ( pl_PathEtFic, "%s/%s_LCR", XZEXC_PATH_FIC_LCR, XDF_Config_PAL );
         pl_DebutNomLCR = pg_DebutNomPAL;
         break;
      case XZECC_TYPE_EQT_BAF :
         sprintf ( pl_PathEtFic, "%s/%s_LCR", XZEXC_PATH_FIC_LCR, XDF_Config_BAF );
         pl_DebutNomLCR = pg_DebutNomBAF;
         break;
      case XZECC_TYPE_EQT_BAD :
         sprintf ( pl_PathEtFic, "%s/%s_LCR", XZEXC_PATH_FIC_LCR, XDF_Config_BAD );
         pl_DebutNomLCR = pg_DebutNomBAD;
         break;
      case XZECC_TYPE_EQT_GTC :
         sprintf ( pl_PathEtFic, "%s/%s_LCR", XZEXC_PATH_FIC_LCR, XDF_Config_GTC );
         pl_DebutNomLCR = pg_DebutNomGTC;
         break;
      case XDC_EQT_TOR :
         strcpy ( pa_NomEqt, XDC_LIB_TOR );
         return (XDC_OK);
         break;
      default : 
         if ( (va_TypeEqt != XDC_EQT_ANA) && (va_TypeEqt != XDC_EQT_MOD) && (va_TypeEqt != XDC_EQT_MODBAD) && (va_TypeEqt != XDC_EQT_MODBAF) )
         {
            sprintf ( pl_PathEtFic, "%s/%s_LCR", XZEXC_PATH_FIC_LCR, XDF_Config_EQT_GEN_LCR );
            pl_DebutNomLCR = pg_DebutNomQTX;
         }
         else
         {
            pl_DebutNomLCR = NULL;
         }
   }

   /* Si la liste des noms d'equipement est vide */
   if ((pl_DebutNomLCR == NULL) && (va_TypeEqt != XDC_EQT_ANA) && (va_TypeEqt != XDC_EQT_MOD) && (va_TypeEqt != XDC_EQT_MODBAD) && (va_TypeEqt != XDC_EQT_MODBAF))
   {

      /*A Ouverture du fichier des noms d'equipement */
      if (XZSS_12OuvrirFichier(O_RDONLY, pl_PathEtFic, &vl_Fd) != XDC_OK)
      {
         if (XZSS_12OuvrirFichier(O_RDONLY, pl_PathEtFic, &vl_Fd) != XDC_OK)
         {
            XZST_03EcritureTrace(XZSTC_WARNING, " Manque fichier %s. \n", pl_PathEtFic);
            strcpy(pa_NomEqt, "Inconnu");
            strcpy(pa_NomMachine, "INCONNU");
            return (XDC_NOK);
         }
      }

      /*A Lecture de chaque ligne du fichier */
      while (ex_LireLigneFich(vl_Fd, pl_LigneFich, sizeof(pl_LigneFich)) != XZEXC_EOF)
      {
         /* Lire ligne suivante si c'est une ligne de commentaire */
         if (*pl_LigneFich != XDF_COMMENTAIRE)
         {
            /*B Extraire le texte  */
            strcpy(vl_NomEqt.NomEqt, strtok(pl_LigneFich, ","));
            sscanf(strtok(NULL, ","), "%d,", &vl_NomEqt.Numero);
            strcpy(vl_NomEqt.NomMachine, strtok(NULL, ","));
            /*A Allocation memoire pour la nouvelle structure */
            if ((pl_NewEqtLCR = (EXT_NOM_EQT_LCR *)malloc(sizeof(EXT_NOM_EQT_LCR))) == NULL)
            {
               /* Ecriture trace */
               XZST_03EcritureTrace(XZSTC_WARNING, " Allocation memoire impossible");
            }
            else
            {
               /*A Mise a jour de la structure a inserer */
               memcpy(pl_NewEqtLCR, &vl_NomEqt, sizeof(EXT_NOM_EQT_LCR));
               /*A Ajout de la structure dans la liste */
               pl_NewEqtLCR->Suivant = NULL;
               if (pl_DebutNomLCR == NULL)
                  pl_DebutNomLCR = pl_NewEqtLCR;
               else
                  pl_ExEqtLCR->Suivant = pl_NewEqtLCR;
               pl_ExEqtLCR = pl_NewEqtLCR;
            }
         }
      }
      /*A Fermeture du fichier des textes d'alerte  */
      XZSS_13FermerFichier(vl_Fd);

      switch (va_TypeEqt)
      {
         case XZECC_TYPE_EQT_RAU :
            pg_DebutNomRAU = pl_DebutNomLCR;
            break;
         case XDC_EQT_GEN_LCR :
            pg_DebutNomQTX = pl_DebutNomLCR;
            break;
         case XDC_EQT_TDP :
            pg_DebutNomTDP = pl_DebutNomLCR;
            break;
         case XDC_EQT_ZDP :
            pg_DebutNomZDP = pl_DebutNomLCR;
            break;
         case XDC_EQT_PAU :
            pg_DebutNomPAU = pl_DebutNomLCR;
            break;
         case XZECC_TYPE_EQT_VID :
            pg_DebutNomVID = pl_DebutNomLCR;
            break;
         case XZECC_TYPE_EQT_RDT :
            pg_DebutNomRAD = pl_DebutNomLCR;
            break;
         case XZECC_TYPE_EQT_DAI :
            pg_DebutNomDAI = pl_DebutNomLCR;
            break;
         case XZECC_TYPE_EQT_PMV :
            pg_DebutNomPMV = pl_DebutNomLCR;
            break;
         case XZECC_TYPE_EQT_PMVA :
            pg_DebutNomPMVA = pl_DebutNomLCR;
            break;
		 case XZECC_TYPE_EQT_PRV :
            pg_DebutNomPRV = pl_DebutNomLCR;
            break;
         case XZECC_TYPE_EQT_PIC :
            pg_DebutNomPIC = pl_DebutNomLCR;
            break;
         case XZECC_TYPE_EQT_PICTO :
            pg_DebutNomPCT = pl_DebutNomLCR;
            break;
         case XZECC_TYPE_EQT_NAV :
         case XDC_EQT_FAU :
         case XDC_EQT_FAC :
            pg_DebutNomNAV = pl_DebutNomLCR;
            break;
         case XZECC_TYPE_EQT_PAL :
            pg_DebutNomPAL = pl_DebutNomLCR;
            break;
         case XZECC_TYPE_EQT_BAF :
            pg_DebutNomBAF = pl_DebutNomLCR;
            break;
         case XZECC_TYPE_EQT_BAD :
            pg_DebutNomBAD = pl_DebutNomLCR;
            break;
         case XZECC_TYPE_EQT_GTC :
            pg_DebutNomGTC = pl_DebutNomLCR;
            break;
         default : 
            if ( (va_TypeEqt != XDC_EQT_ANA) && (va_TypeEqt != XDC_EQT_MOD) && (va_TypeEqt != XDC_EQT_MODBAF) && (va_TypeEqt != XDC_EQT_MODBAD) )
               pg_DebutNomQTX = pl_DebutNomLCR;
            else
               return (XDC_NOK);
      }

      /* Ecriture trace de la config pour debug */
      for (pl_DonCrt = pl_DebutNomLCR; pl_DonCrt != NULL; pl_DonCrt = pl_DonCrt->Suivant)
      {
         XZST_03EcritureTrace(XZSTC_WARNING, "Donnee LCR: %d <%s> <%s>",
                              pl_DonCrt->Numero,
                              pl_DonCrt->NomEqt,
                              pl_DonCrt->NomMachine);
      }
   }

   if (va_NomMachine != NULL)
   {
      strcpy(pa_NomMachine, va_NomMachine);
      if ((!strcmp(vl_NomTypeEqt, XZECC_NOM_RAU) && (va_TypeEqt != XDC_EQT_PAU)) ||
          !strcmp(vl_NomTypeEqt, XZECC_NOM_VID) ||
          !strcmp(vl_NomTypeEqt, XZECC_NOM_GTC))
      {
         for (pl_DonCrt = pl_DebutNomLCR; pl_DonCrt != NULL; pl_DonCrt = pl_DonCrt->Suivant)
         {
            strcpy(pa_NomMachine, strtok(pl_DonCrt->NomMachine, " \t"));
            if (!strncmp(pa_NomMachine, va_NomMachine, strlen(va_NomMachine)))
            {
               strcpy(pa_NomEqt, pl_DonCrt->NomEqt);
               break;
            }
         }
         if (pl_DonCrt == NULL)
         {
            for (pl_DonCrt = pl_DebutNomLCR; pl_DonCrt != NULL; pl_DonCrt = pl_DonCrt->Suivant)
            {
               if (pl_DonCrt->Numero == va_NumEqt)
               {
                  strcpy(pa_NomMachine, va_NomMachine);
                  strcpy(pa_NomEqt, pl_DonCrt->NomEqt);
                  break;
               }
            }
         }
      }
      else
      {
         if ((va_TypeEqt != XDC_EQT_ANA) && (va_TypeEqt != XDC_EQT_MOD) && (va_TypeEqt != XDC_EQT_MODBAF) && (va_TypeEqt != XDC_EQT_MODBAD))
            /* Retourne le nom de l'equipement concerne */
            for (pl_DonCrt = pl_DebutNomLCR; pl_DonCrt != NULL; pl_DonCrt = pl_DonCrt->Suivant)
               if (pl_DonCrt->Numero == va_NumEqt)
               {
                  strcpy(pa_NomEqt, pl_DonCrt->NomEqt);
                  break;
               }
      }

      if (va_TypeEqt == XDC_EQT_ANA)
      {
         /*A Ouverture du fichier de configuration des Analyseurs */
         /* Construction du nom du fichier XDF_Config_Analyseur_<NomMachine>  */
         sprintf(pl_PathEtFic, "%s/%s_%s", XZEXC_PATH_CONFIG, XDF_Config_Analyseur, va_NomMachine);

         /* Ouverture du fichier XDF_Config_Analyseur_<NomMachine>  */
         if ((vl_Fd = open(pl_PathEtFic, O_RDONLY)) <= 0)
         {
            strcpy(pa_NomEqt, XDC_LIB_ANA);
            return (XDC_NOK);
         }

         /*A Lecture de chaque ligne du fichier */
         while (ex_LireLigneFich(vl_Fd, pl_LigneFich, sizeof(pl_LigneFich)) != XZEXC_EOF)
         {
            /* Lire ligne suivante si c'est une ligne de commentaire */
            if (*pl_LigneFich != XDF_COMMENTAIRE)
            {
               if ((sscanf(pl_LigneFich, " %hd %hd %hd", &vl_NumeroDAI, &vl_NoAnaly, &vl_NumEqt) == 3) &&
                   (vl_NumEqt == va_NumEqt))
               {
                  for (pl_DonCrt = pl_DebutNomLCR; pl_DonCrt != NULL; pl_DonCrt = pl_DonCrt->Suivant)
                  {
                     if (pl_DonCrt->Numero == vl_NumeroDAI)
                     {
                        strcpy(pa_NomEqt, pl_DonCrt->NomEqt);
                        break;
                     }
                  }
               }
            }
         }
         XZSS_13FermerFichier(vl_Fd);
      }

      if (va_TypeEqt == XDC_EQT_MOD)
      {
         /*A Ouverture du fichier de configuration des NAV */
         /* Construction du nom du fichier XDF_Config_NAV_<NomMachine>  */
         sprintf(pl_PathEtFic, "%s/%s_%s", XZEXC_PATH_CONFIG, XDF_Config_NAV, va_NomMachine);

         /* Ouverture du fichier XDF_Config_NAV_<NomMachine>  */
         if ((vl_Fd = open(pl_PathEtFic, O_RDONLY)) <= 0)
         {
            strcpy(pa_NomEqt, XDC_LIB_MOD);
            return (XDC_NOK);
         }

         /*A Lecture de chaque ligne du fichier */
         while (ex_LireLigneFich(vl_Fd, pl_LigneFich, sizeof(pl_LigneFich)) != XZEXC_EOF)
         {
            /* Lire ligne suivante si c'est une ligne de commentaire */
            if (*pl_LigneFich != 'N')
               sscanf(pl_LigneFich, "N %hd ", &vl_NumeroNAV);

            if (*pl_LigneFich != 'M')
            {
               if ((sscanf(pl_LigneFich, "M %hd ", &vl_NoModule) == 1) && (vl_NoModule == va_NumEqt))
               {
                  for (pl_DonCrt = pl_DebutNomLCR; pl_DonCrt != NULL; pl_DonCrt = pl_DonCrt->Suivant)
                  {
                     if (pl_DonCrt->Numero == vl_NumeroNAV)
                     {
                        strcpy(pa_NomEqt, pl_DonCrt->NomEqt);
                        break;
                     }
                  }
                  break;
               }
            }
         }
         XZSS_13FermerFichier(vl_Fd);
      }

      if (va_TypeEqt == XDC_EQT_MODBAF)
      {
         /*A Ouverture du fichier de configuration des BAF */
         /* Construction du nom du fichier XDF_Config_BAF_<NomMachine>  */
         sprintf(pl_PathEtFic, "%s/%s_%s", XZEXC_PATH_CONFIG, XDF_Config_BAF, va_NomMachine);

         /* Ouverture du fichier XDF_Config_NAV_<NomMachine>  */
         if ((vl_Fd = open(pl_PathEtFic, O_RDONLY)) <= 0)
         {
            strcpy(pa_NomEqt, XDC_LIB_MODBAF);
            return (XDC_NOK);
         }

         /*A Lecture de chaque ligne du fichier */
         while (ex_LireLigneFich(vl_Fd, pl_LigneFich, sizeof(pl_LigneFich)) != XZEXC_EOF)
         {
            /* Lire ligne suivante si c'est une ligne de commentaire */
            if (*pl_LigneFich != 'N')
               sscanf(pl_LigneFich, "N %hd ", &vl_NumeroBAF);

            if (*pl_LigneFich != 'M')
            {
               if ((sscanf(pl_LigneFich, "M %hd ", &vl_NoModule) == 1) && (vl_NoModule == va_NumEqt))
               {
                  for (pl_DonCrt = pl_DebutNomLCR; pl_DonCrt != NULL; pl_DonCrt = pl_DonCrt->Suivant)
                  {
                     if (pl_DonCrt->Numero == vl_NumeroBAF)
                     {
                        strcpy(pa_NomEqt, pl_DonCrt->NomEqt);
                        break;
                     }
                  }
                  break;
               }
            }
         }
         XZSS_13FermerFichier(vl_Fd);
      }

      if (va_TypeEqt == XDC_EQT_MODBAD)
      {
         /*A Ouverture du fichier de configuration des BAD */
         /* Construction du nom du fichier XDF_Config_BAD_<NomMachine>  */
         sprintf(pl_PathEtFic, "%s/%s_%s", XZEXC_PATH_CONFIG, XDF_Config_BAD, va_NomMachine);

         /* Ouverture du fichier XDF_Config_NAV_<NomMachine>  */
         if ((vl_Fd = open(pl_PathEtFic, O_RDONLY)) <= 0)
         {
            strcpy(pa_NomEqt, XDC_LIB_MODBAD);
            return (XDC_NOK);
         }

         /*A Lecture de chaque ligne du fichier */
         while (ex_LireLigneFich(vl_Fd, pl_LigneFich, sizeof(pl_LigneFich)) != XZEXC_EOF)
         {
            /* Lire ligne suivante si c'est une ligne de commentaire */
            if (*pl_LigneFich != 'N')
               sscanf(pl_LigneFich, "N %hd ", &vl_NumeroBAD);

            if (*pl_LigneFich != 'M')
            {
               if ((sscanf(pl_LigneFich, "M %hd ", &vl_NoModule) == 1) && (vl_NoModule == va_NumEqt))
               {
                  for (pl_DonCrt = pl_DebutNomLCR; pl_DonCrt != NULL; pl_DonCrt = pl_DonCrt->Suivant)
                  {
                     if (pl_DonCrt->Numero == vl_NumeroBAD)
                     {
                        strcpy(pa_NomEqt, pl_DonCrt->NomEqt);
                        break;
                     }
                  }
                  break;
               }
            }
         }
         XZSS_13FermerFichier(vl_Fd);
      }
   }
   else
   {
      /* Retourne le nom de l'equipement concerne */
      for (pl_DonCrt = pl_DebutNomLCR; pl_DonCrt != NULL; pl_DonCrt = pl_DonCrt->Suivant)
      {
         if (pl_DonCrt->Numero == va_NumEqt)
         {
            strcpy(pa_NomEqt, pl_DonCrt->NomEqt);
            strcpy(pa_NomMachine, strtok(pl_DonCrt->NomMachine, " \t"));
            break;
         }
      }
   }

   if (strcmp(pa_NomEqt, "") == 0)
      return (XDC_NOK);
   else
      return (XDC_OK);
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* Ecriture dans un fichier de trace de toutes les commandes LCR
*
------------------------------------------------------
*
* ARGUMENTS EN ENTREE :
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR :
* XDC_OK : Succes
* XDC_NOK : KO
*
* CONDITION D'UTILISATION
*
*
* FONCTION
*
------------------------------------------------------*/
int ex_ecriture_trace_lcr(XDY_Eqt va_TypeEqt,
                          XDY_Eqt va_NumEqt,
                          char *pa_TexteCmd,
                          char *pa_TexteReponse,
                          char *pa_NomPO,
                          int va_Position)
{

   int vl_Fd = 0;
   long vl_PtrFile = 0;
   char pl_NomFichier[XDC_PATH_ABS_SIZE];
   XZEXT_TRAME_PS_NAV pl_LigneFich = "";
   XDY_Texte pl_Entete = "";
   XDY_NomEqt vl_NomTypeEqt = "";
   XDY_Nom vl_NomEqt = "";
   XDY_NomMachine vl_NomMachine = "";
   XZSMT_Horodate pl_StrHorodate;
   XDY_Horodate vl_Horodate;
   int vl_JourSem; /* jour de la semaine */
   static char vl_Entete[] = "#Pointeur dernier enregistrement: %09ld\n";
   int vl_CptCar = 0;

   /* Si le fichier de traces des commandes LCR  existe */
   XZEX03_nom_eqt((va_TypeEqt == XDC_EQT_PCT) ? XDC_EQT_PMV : va_TypeEqt, vl_NomTypeEqt);
   ex_nom_equipement(va_TypeEqt, va_NumEqt, NULL, vl_NomEqt, vl_NomMachine);
   sprintf(pl_NomFichier, "%s/Trace_LCR_%s_%s.lcr", XZEXC_PATH_FIC_LCR, vl_NomTypeEqt, vl_NomMachine);

   if (!access(pl_NomFichier, F_OK))
   {
      /* Ouverture du fichier en Lecture et ecriture */
      chmod(pl_NomFichier, 0644);
      if ((vl_Fd = open(pl_NomFichier, O_RDWR)) <= 0)
      {
         XZST_03EcritureTrace(XZSTC_WARNING, "Ouverture du fichier  '%s' impossible ", pl_NomFichier);
         return (XDC_NOK);
      }

      /* Lecture de l'entete du fichier afin de connaitre la position du prochain enregistrement */
      if (ex_LireLigneFich(vl_Fd, pl_Entete, sizeof(XDY_Texte)) == XZEXC_EOF)
      {
         XZST_03EcritureTrace(XZSTC_WARNING, "Lecture entete fichier '%s' impossible", pl_NomFichier);
         close(vl_Fd);
         chmod(pl_NomFichier, 0444);
         return (XDC_NOK);
      }

      /* Extraction de la valeur du pointeur */
      if (sscanf(pl_Entete, vl_Entete, &vl_PtrFile) != 1)
      {
         XZST_03EcritureTrace(XZSTC_WARNING, "Pointeur fichier '%s' incorrect", pl_NomFichier);
         close(vl_Fd);
         chmod(pl_NomFichier, 0444);
         return (XDC_NOK);
      }

      /* Positionnement du pointeur du fichier */
      if (!lseek(vl_Fd, vl_PtrFile, SEEK_SET))
      {
         XZST_03EcritureTrace(XZSTC_WARNING, "Erreur positionnement sur fichier '%s' incorrect", pl_NomFichier);
         close(vl_Fd);
         chmod(pl_NomFichier, 0444);
         return (XDC_NOK);
      }
   }
   else
   {
      /* Creation du fichier de traces des commandes LCR */
      if ((vl_Fd = open(pl_NomFichier, O_RDWR | O_CREAT)) < 0)
      {
         XZST_03EcritureTrace(XZSTC_WARNING, "Impossible de creer le fichier '%s'", pl_NomFichier);
         return (XDC_NOK);
      }

      /* Ecriture de l'entete dans le fichier */
      sprintf(pl_Entete, vl_Entete, vl_PtrFile);
      // sem_wait(&vg_semaphore);
      if (write(vl_Fd, pl_Entete, strlen(pl_Entete)) < 0)
      {
         // sem_post(&vg_semaphore);
         XZST_03EcritureTrace(XZSTC_WARNING, "Ecriture entete fichier '%s' incorrect", pl_NomFichier);
         close(vl_Fd);
         chmod(pl_NomFichier, 0444);
         return (XDC_NOK);
      }
      else
      {
         /* Mise a jour du pointeur */
         vl_PtrFile = strlen(pl_Entete);
      }
      // sem_post(&vg_semaphore);
   }

   /* Enregistrement de la commande */
   if (va_Position == ELCRC_DEBUT)
   {
      XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME, &vl_JourSem, &vl_Horodate);
      XZSM_11ConversionHorodate(vl_Horodate, &pl_StrHorodate);
      vl_CptCar = sprintf(pl_LigneFich, "\n%s Demmande LCR : <%s> sur %s par %s\nReponse :%s",
                          pl_StrHorodate,
                          pa_TexteCmd,
                          vl_NomEqt,
                          pa_NomPO,
                          pa_TexteReponse);
   }
   sprintf(pl_LigneFich + vl_CptCar, "%s\n", pa_TexteReponse);
   XZST_03EcritureTrace(XZSTC_INFO, "Trace cmd LCR: %s", pl_LigneFich);

   /* Si taille maxi du fichier d'alarme atteinte */
   if (vl_PtrFile + strlen(pl_LigneFich) > ELCRC_TailleFic)
   {
      /* Postionnement du pointeur au debut */
      vl_PtrFile = strlen(pl_Entete) + 1;
   }

   if (!lseek(vl_Fd, vl_PtrFile, SEEK_SET))
   {
      XZST_03EcritureTrace(XZSTC_WARNING, "Erreur positionnement sur fichier '%s'", pl_NomFichier);
      chmod(pl_NomFichier, 0444);
      return (XDC_NOK);
   }

   // sem_wait(&vg_semaphore);
   if (write(vl_Fd, pl_LigneFich, strlen(pl_LigneFich)) < 0)
   {
      // sem_post(&vg_semaphore);
      XZST_03EcritureTrace(XZSTC_WARNING, "Ecriture dans fichier '%s' impossible", pl_NomFichier);
      close(vl_Fd);
      chmod(pl_NomFichier, 0444);
      return (XDC_NOK);
   }
   else
   {
      // sem_post(&vg_semaphore);
      /* Mise a jour du pointeur */
      vl_PtrFile += strlen(pl_LigneFich);
      /* Enregistrement dans le fichier du nouveau pointeur */
      sprintf(pl_LigneFich, vl_Entete, vl_PtrFile);
      /* Positionnement du pointeur du fichier */
      if (lseek(vl_Fd, 0, SEEK_SET))
      {
         XZST_03EcritureTrace(XZSTC_WARNING, "Erreur positionnement sur fichier '%s' incorrect", pl_NomFichier);
         close(vl_Fd);
         chmod(pl_NomFichier, 0444);
         return (XDC_NOK);
      }

      // sem_wait(&vg_semaphore);
      if (write(vl_Fd, pl_LigneFich, strlen(pl_LigneFich)) < 0)
      {
         // sem_post(&vg_semaphore);
         XZST_03EcritureTrace(XZSTC_WARNING, "Ecriture entete fichier '%s' incorrect", pl_NomFichier);
         close(vl_Fd);
         chmod(pl_NomFichier, 0444);
         return (XDC_NOK);
      }
      // sem_post(&vg_semaphore);

      close(vl_Fd);
      chmod(pl_NomFichier, 0444);
   }

   return (XDC_OK);
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* Ecriture dans un fichier de trace de toutes les commandes NON LCR
*
------------------------------------------------------
*
* ARGUMENTS EN ENTREE :
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR :
* XDC_OK : Succes
* XDC_NOK : KO
*
* CONDITION D'UTILISATION
*
*
* FONCTION
*
------------------------------------------------------*/
int ex_ecriture_trace_cmd(XDY_Eqt va_TypeEqt,
                          XDY_Eqt va_NumEqt,
                          char *pa_TexteCmd,
                          char *pa_TexteReponse,
                          int va_NumCmd)
{

   int vl_Fd = 0;
   long vl_PtrFile = 0;
   char pl_NomFichier[XDC_PATH_ABS_SIZE];
   XZEXT_TRAME_PS_NAV pl_LigneFich = "";
   XDY_Texte pl_Entete = "";
   XDY_NomEqt vl_NomTypeEqt = "";
   XDY_Nom vl_NomEqt = "";
   XDY_NomMachine vl_NomMachine = "";
   XZSMT_Horodate pl_StrHorodate;
   XDY_Horodate vl_Horodate;
   int vl_JourSem; /* jour de la semaine */
   static char vl_Entete[] = "#Pointeur dernier enregistrement: %09ld\n";
   int vl_CptCar = 0;

   /* Si le fichier de traces des commandes non LCR existe */
   if (va_TypeEqt != XDC_EQT_NIC)
   {
      XZEX03_nom_eqt((va_TypeEqt == XDC_EQT_PCT) ? XDC_EQT_PMV : va_TypeEqt, vl_NomTypeEqt);
      ex_nom_equipement(va_TypeEqt, va_NumEqt, NULL, vl_NomEqt, vl_NomMachine);
      sprintf(pl_NomFichier, "%s/Trace_CMD_%s_%s", XZEXC_PATH_FIC_LCR, vl_NomTypeEqt, vl_NomMachine);
   }
   else
   {
      sprintf(pl_NomFichier, "%s/Trace_CMD_NIC", XZEXC_PATH_FIC_LCR);
      strcpy(vl_NomEqt, XDC_LIB_NIC);
   }

   if (!access(pl_NomFichier, F_OK))
   {
      /* Ouverture du fichier en Lecture et ecriture */
      chmod(pl_NomFichier, 0644);
      if ((vl_Fd = open(pl_NomFichier, O_RDWR)) <= 0)
      {
         XZST_03EcritureTrace(XZSTC_WARNING, "Ouverture du fichier  '%s' impossible ", pl_NomFichier);
         return (XDC_NOK);
      }

      /* Lecture de l'entete du fichier afin de connaitre la position du prochain enregistrement */
      if (ex_LireLigneFich(vl_Fd, pl_Entete, sizeof(XDY_Texte)) == XZEXC_EOF)
      {
         XZST_03EcritureTrace(XZSTC_WARNING, "Lecture entete fichier '%s' impossible", pl_NomFichier);
         close(vl_Fd);
         chmod(pl_NomFichier, 0444);
         return (XDC_NOK);
      }

      /* Extraction de la valeur du pointeur */
      if (sscanf(pl_Entete, vl_Entete, &vl_PtrFile) != 1)
      {
         XZST_03EcritureTrace(XZSTC_WARNING, "Pointeur fichier '%s' incorrect", pl_NomFichier);
         close(vl_Fd);
         chmod(pl_NomFichier, 0444);
         return (XDC_NOK);
      }

      /* Positionnement du pointeur du fichier */
      if (!lseek(vl_Fd, vl_PtrFile, SEEK_SET))
      {
         XZST_03EcritureTrace(XZSTC_WARNING, "Erreur positionnement sur fichier '%s' incorrect", pl_NomFichier);
         close(vl_Fd);
         chmod(pl_NomFichier, 0444);
         return (XDC_NOK);
      }
   }
   else
   {
      /* Creation du fichier de traces des commandes LCR */
      if ((vl_Fd = open(pl_NomFichier, O_RDWR | O_CREAT)) < 0)
      {
         XZST_03EcritureTrace(XZSTC_WARNING, "Impossible de creer le fichier '%s'", pl_NomFichier);
         return (XDC_NOK);
      }

      /* Ecriture de l'entete dans le fichier */
      sprintf(pl_Entete, vl_Entete, vl_PtrFile);
      // sem_wait(&vg_semaphore);
      if (write(vl_Fd, pl_Entete, strlen(pl_Entete)) < 0)
      {
         // sem_post(&vg_semaphore);
         XZST_03EcritureTrace(XZSTC_WARNING, "Ecriture entete fichier '%s' incorrect", pl_NomFichier);
         close(vl_Fd);
         chmod(pl_NomFichier, 0444);
         return (XDC_NOK);
      }
      else
      {
         // sem_post(&vg_semaphore);
         /* Mise a jour du pointeur */
         vl_PtrFile = strlen(pl_Entete);
      }
   }

   XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME, &vl_JourSem, &vl_Horodate);
   XZSM_11ConversionHorodate(vl_Horodate, &pl_StrHorodate);

   /* Enregistrement de la commande */
   if (pa_TexteCmd != NULL)
      vl_CptCar = sprintf(pl_LigneFich, "\n%s Commande : <%s> sur %s cmd: %d\n",
                          pl_StrHorodate,
                          pa_TexteCmd,
                          vl_NomEqt,
                          va_NumCmd);

   if (pa_TexteReponse != NULL)
      sprintf(pl_LigneFich + vl_CptCar, "\n%s Reponse : <%s> sur %s cmd: %d\n",
              pl_StrHorodate,
              pa_TexteReponse,
              vl_NomEqt,
              va_NumCmd);

   /* Si taille maxi du fichier d'alarme atteinte */
   if (vl_PtrFile + strlen(pl_LigneFich) > ELCRC_TailleFic)
   {
      /* Postionnement du pointeur au debut */
      vl_PtrFile = strlen(pl_Entete) + 1;
   }

   if (!lseek(vl_Fd, vl_PtrFile, SEEK_SET))
   {
      XZST_03EcritureTrace(XZSTC_WARNING, "Erreur positionnement sur fichier '%s'", pl_NomFichier);
      chmod(pl_NomFichier, 0444);
      return (XDC_NOK);
   }

   // sem_wait(&vg_semaphore);
   if (write(vl_Fd, pl_LigneFich, strlen(pl_LigneFich)) < 0)
   {
      // sem_post(&vg_semaphore);
      XZST_03EcritureTrace(XZSTC_WARNING, "Ecriture dans fichier '%s' impossible", pl_NomFichier);
      close(vl_Fd);
      chmod(pl_NomFichier, 0444);
      return (XDC_NOK);
   }
   else
   {
      // sem_post(&vg_semaphore);
      /* Mise a jour du pointeur */
      vl_PtrFile += strlen(pl_LigneFich);
      /* Enregistrement dans le fichier du nouveau pointeur */
      sprintf(pl_LigneFich, vl_Entete, vl_PtrFile);
      /* Positionnement du pointeur du fichier */
      if (lseek(vl_Fd, 0, SEEK_SET))
      {
         XZST_03EcritureTrace(XZSTC_WARNING, "Erreur positionnement sur fichier '%s' incorrect", pl_NomFichier);
         close(vl_Fd);
         chmod(pl_NomFichier, 0444);
         return (XDC_NOK);
      }

      // sem_wait(&vg_semaphore);
      if (write(vl_Fd, pl_LigneFich, strlen(pl_LigneFich)) < 0)
      {
         // sem_post(&vg_semaphore);
         XZST_03EcritureTrace(XZSTC_WARNING, "Ecriture entete fichier '%s' incorrect", pl_NomFichier);
         close(vl_Fd);
         chmod(pl_NomFichier, 0444);
         return (XDC_NOK);
      }
      // sem_post(&vg_semaphore);

      close(vl_Fd);
      chmod(pl_NomFichier, 0444);
   }

   return (XDC_OK);
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Envoi une trame a TEMIP.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int EnvoyerTrameIP(XZEXT_Trame trame,
                   int type_msg,
                   char *adr,
                   int port,
                   char *rgs,
                   int nocmd,
                   int priorite,
                   int socket)
/*
* ARGUMENTS EN ENTREE :
*  trame : trame a envoyer
*  type_msg : type du message
*  adr_rgs : adresse rgs
*  nocmd : numero de commande
*  priorite : priorite de la trame
*  socket : socket d'emision
*
* ARGUMENTS EN SORTIE :
*   aucun
*
*
* CODE RETOUR :
*   XDC_OK : succes
*   XDC_NOK : Erreur
*
* CONDITION D'UTILISATION
*
*   Fonction appele apres avoir construit la trame
*
* FONCTION
*  Demande d'envoi de trame a telmi
*
------------------------------------------------------*/
{
   static char *version = "$Id: ex_mlcr.c,v 1.37 2021/05/03 13:44:23 pc2dpdy Exp $ : EnvoyerTrame";

   XZEXT_MSG_SOCKET vl_message;
   ssize_t vl_RetCode = 0;

   /*A Construction de la trame */
   sprintf(vl_message, "%s%d %s %d %s %d %d %s%s %s",
           XZEXC_DEMANDE_LCR,
           type_msg,
           adr,
           port,
           rgs,
           nocmd,
           priorite,
           XZEXC_SEP_TRAME,
           trame,
           XZEXC_FIN_MSG_SOCK);

   fcntl(socket, F_SETFL, O_NDELAY | fcntl(socket, F_GETFL, 0));

   // sem_wait(&vg_semaphore);
   if ((vl_RetCode = write(socket, vl_message, strlen(vl_message))) < 0)
   {
      // sem_post(&vg_semaphore);
      /*B Ecriture dans le fichier trace en cas d'echec */
      XZST_03EcritureTrace(XZSTC_FATAL, "Impossible d'emettre la trame IP (%s) vers TEIM.", vl_message);
      return (XDC_NOK);
   }
   else
   {
      // sem_post(&vg_semaphore);
      /*B Ecriture dans le fichier trace en cas de succes */
      if (vl_RetCode != strlen(vl_message))
      {
         XZST_03EcritureTrace(XZSTC_WARNING, "Trame IP INCOMPLETE emise vers TEIM <%s>", vl_message);
         return (XDC_OK);
      }
      else
      {
         XZST_03EcritureTrace(XZSTC_WARNING, "Trame IP emise vers TEIMI  <%s>, socket=%d", vl_message, socket);
         return (XDC_OK);
      }
   }
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* Traitement des demandes fenetre LCR.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int ex_cmd_LCR_IP(char *pa_NomFichCmdesLCR,
                  XDY_NomMachine va_NomMachDist,
                  XDY_TypeEqt va_typeeqt,
                  XDY_Eqt va_noeqt,
                  XDY_Rgs va_AdresseRGS,
                  int va_Socket,
                  char *pa_NomFichResLCR,
                  XDY_Entier *va_Position,
                  char *pa_TexteCmd,
                  XDY_AdresseIP va_AdresseIP,
                  XDY_PortIP va_Port)

/*
* ARGUMENTS EN ENTREE :
*
* pa_NomFichCmdesLCR	: nom fichier contenant les cmdes. de la fenetre LCR.
* va_NomMachDist	: nom machine distante sur laquelle se trouve le fichier cmdes. LCR.
* va_typeeqt		: Type d'equipement
* va_noeqt		: No d'equipement
* va_AdresseRGS		: Adresse rgs de l'eqt a piloter
* va_Socket		: Socket de l'eqt a piloter
* pa_NomFichResLCR	: nom fichier contenant le resultat de la fenetre LCR
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR :
* XDC_OK : Succes
* XDC_NOK : KO
*
* CONDITION D'UTILISATION
*
*
* FONCTION
* - decodage demande LCR
* - recuperation fichier cmdes. fenetre LCR.
* - ouverture/creation du fichier resultat.
* - envoi 1ere cmdes. LCR.
*
------------------------------------------------------*/
{
   static char *version = "$Id: ex_mlcr.c,v 1.37 2021/05/03 13:44:23 pc2dpdy Exp $ : ex_cmd_LCR_IP";

   int vl_Fd;                         /* desc. fichier cmdes. fenetre LCR */
   XZSCT_NomMachine vl_MachineLocale; /* nom de la machine locale */
   char pl_NomFichier[256];
   XDY_Texte pl_LigneFich;
   XZSMT_Horodate vl_Horodate; /* horodate en format lisible */
   XDY_Horodate vl_HorodateSec;
   XZSMT_Horodate vl_Horodatedecompo;
   int vl_JourSem; /* jour de la semaine */

   XZST_03EcritureTrace(XZSTC_FONCTION, "IN:ex_cmd_LCR_IP");

   /*A Si le fichier LCR n'existe pas -> transfert de celui-ci */

   sprintf(pl_NomFichier, "%s/%s", XZEXC_PATH_FIC_LCR, pa_NomFichCmdesLCR);

   /* SI recuperation nom de la machine locale OK */

   if (XZSC_07NomMachine(vl_MachineLocale) != XDC_OK)
      XZST_03EcritureTrace(XZSTC_WARNING, "ex_cmd_LCR_IP Recup. nom machine locale impossible");

   XZST_03EcritureTrace(XZSTC_WARNING, "ex_cmd_LCR_IP Nom machine locale : %s", vl_MachineLocale);

   if (!strcmp(vl_MachineLocale, va_NomMachDist))
   {
      XZST_03EcritureTrace(XZSTC_WARNING, "ex_cmd_LCR_IP Pas de transfert : Fenetre LCR sur machine locale");
   }
   else
   {
      /*A transfert du fichier des cmdes. LCR */
      XZST_03EcritureTrace(XZSTC_WARNING, "ex_cmd_LCR_IP Transfert fichier cmdes. LCR <%s:%s/%s> : ",
                           va_NomMachDist,
                           XZEXC_PATH_FIC_LCR,
                           pa_NomFichCmdesLCR);

      /* transfert de fichier cmdes LCR */
      XZSS_07TransfertFichier(va_NomMachDist,
                              pa_NomFichCmdesLCR,
                              XZEXC_PATH_FIC_LCR,
                              vl_MachineLocale,
                              pa_NomFichCmdesLCR,
                              XZEXC_PATH_FIC_LCR);
   }

   if (access(pl_NomFichier, F_OK))
   {
      XZST_03EcritureTrace(XZSTC_WARNING, "ex_cmd_LCR_IP Transfert fichier LCR Echoue");
      XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : ex_cmd_LCR_IP");
      return (XDC_NOK);
   }

   /*A Ouverture du fichier lcr */

   if (XZSS_12OuvrirFichier(O_RDONLY, pl_NomFichier, &vl_Fd) != XDC_OK)
   {
      /* Ecriture trace absence fichier */
      XZST_03EcritureTrace(XZSTC_WARNING, "OUT : ex_cmd_LCR_IP Manque fichier %s. \n", pl_NomFichier);
      /* Sortie de la fonction */
      XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : ex_cmd_LCR_IP");
      return (XDC_NOK);
   }

   *va_Position = 0;
   /*A Lecture 1iere commande du fichier */
   while (ex_LireLigneFich(vl_Fd, pl_LigneFich, sizeof(pl_LigneFich)) != XZEXC_EOF)
   {
      /* Lire ligne suivante si c'est une ligne de commentaire */
      if (*pl_LigneFich != XDF_COMMENTAIRE)
      {
         *va_Position += (int)strlen(pl_LigneFich) + 1;
         break;
      }
      else
         *va_Position += (int)strlen(pl_LigneFich) + 1;
   }
   /*A Fermeture du fichier lcr  */
   XZSS_13FermerFichier(vl_Fd);

   /*A Envoyer trame a la tache TELMi */

   if (EnvoyerTrameIP(pl_LigneFich,
                      XZEXC_TRAME_FEN_LCR,
                      va_AdresseIP,
                      va_Port,
                      va_AdresseRGS,
                      XZEXC_NOCMD,
                      XZEXC_PRIORITEFAIBLE,
                      va_Socket) == XDC_OK)
   {
      /* Ecriture de la commande dans le fichier resultat */
      strcpy(pa_TexteCmd, pl_LigneFich);
      XZST_03EcritureTrace(XZSTC_WARNING, "ex_cmd_LCR_IP pa_TexteCmd = [%s]", pa_TexteCmd);
      XZST_03EcritureTrace(XZSTC_WARNING, "OUT:ex_cmd_LCR_IP");
      return (XDC_OK);
   }
   else
   {
      XZST_03EcritureTrace(XZSTC_WARNING, "OUT:ex_cmd_LCR_IP Impossible d'emettre la trame vers TELMi.");
      XZST_03EcritureTrace(XZSTC_FONCTION, "OUT:ex_cmd_LCR_IP");
      return (XDC_NOK);
   }
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* Insere un eqt dans la liste des eqt pilotes par la fenetre lcr
*
------------------------------------------------------
*
* ARGUMENTS EN ENTREE :
* pa_Eqt : infos de l'eqt
* pg_DebutListeEqt : Pointeur du debut de liste des eqt pilotes par la fenetre lcr
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR :
* XDC_OK : Succes
* XDC_NOK : KO
*
* CONDITION D'UTILISATION
*
*
* FONCTION
*
------------------------------------------------------*/
int ex_ins_eqt_liste_IP(INFO_EQT_FEN_LCR_IP *pa_Eqt, INFO_EQT_FEN_LCR_IP **pg_DebutListeEqt)
{
   INFO_EQT_FEN_LCR_IP *pl_ListeEqt, *pl_NewEqt;

   XZST_03EcritureTrace(XZSTC_FONCTION, "IN:ex_ins_eqt_liste_IP");

   /*A Recherche de l'eqt dans la liste */
   for (pl_ListeEqt = *pg_DebutListeEqt; pl_ListeEqt != NULL; pl_ListeEqt = pl_ListeEqt->Suivant)
   {
      if (pl_ListeEqt->NoEqt == pa_Eqt->NoEqt)
         break;
   }
   if (pl_ListeEqt != NULL)
   {
      XZST_03EcritureTrace(XZSTC_FONCTION, "OUT:ex_ins_eqt_liste  L'eqt existe deja");
      return (XDC_NOK);
   }

   if ((pl_NewEqt = (INFO_EQT_FEN_LCR_IP *)malloc(sizeof(INFO_EQT_FEN_LCR_IP))) == NULL)
   {
      XZST_03EcritureTrace(XZSTC_FONCTION, "OUT:ex_ins_eqt_liste  Malloc echoue");
      return (XDC_NOK);
   }

   memcpy(pl_NewEqt, pa_Eqt, sizeof(INFO_EQT_FEN_LCR_IP));

   if (*pg_DebutListeEqt == NULL)
   {
      *pg_DebutListeEqt = pl_NewEqt;
   }
   else
   {
      for (pl_ListeEqt = *pg_DebutListeEqt; pl_ListeEqt != NULL; pl_ListeEqt = pl_ListeEqt->Suivant)
      {
         if (pl_ListeEqt->Suivant == NULL)
            break;
      }
      pl_NewEqt->Suivant = NULL;
      pl_ListeEqt->Suivant = pl_NewEqt;
   }

   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT:ex_ins_eqt_liste_IP");
   return (XDC_OK);
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* Traitement des demandes fenetre LCR.
*
------------------------------------------------------
*
* ARGUMENTS EN ENTREE :
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR :
* XDC_OK : Succes
* XDC_NOK : KO
*
* CONDITION D'UTILISATION
*
*
* FONCTION
* - decodage demande LCR
* - recuperation fichier cmdes. fenetre LCR.
* - ouverture/creation du fichier resultat.
* - envoi 1ere cmdes. LCR.
*
------------------------------------------------------*/
int ex_ret_LCR_IP(char *va_trame,
                  XDY_Rgs va_AdresseRGS,
                  INFO_EQT_FEN_LCR_IP **pg_DebutListeEqt,
                  int va_TypeRep)
{
   INFO_EQT_FEN_LCR_IP *pl_ListeEqt, *pl_eqtprec;
   XZSCT_NomMachine vl_MachineLocale; /* nom de la machine locale */
   char pl_NomFichier[256];
   int vl_Fd; /* desc. fichier  */
   int vl_cmd = XDC_FAUX;
   XDY_Texte pl_LigneFich;
   char vl_ficres[21];
   XDY_Mot vl_indiv;
   char vl_laCommande[XZSSC_LINE_CMD_SIZE] = "";
   FILE *vl_commande = NULL;
   int vl_Position;
   INFO_EQT_FEN_LCR pl_eqt;

   XZST_03EcritureTrace(XZSTC_FONCTION, "IN:ex_ret_LCR Trame reponse: %s %s", va_trame, va_AdresseRGS);

   /*A Recherche de l'eqt dans la liste */
   for (pl_eqtprec = pl_ListeEqt = *pg_DebutListeEqt;
        pl_ListeEqt != NULL;
        pl_eqtprec = pl_ListeEqt, pl_ListeEqt = pl_ListeEqt->Suivant)
   {
      XZST_03EcritureTrace(XZSTC_INFO, "ex_ret_LCR : %s %s", pl_ListeEqt->AdresseRGS, va_AdresseRGS);
      if (!strcmp(pl_ListeEqt->AdresseRGS, va_AdresseRGS))
         break;
   }
   if (pl_ListeEqt == NULL)
   {
      XZST_03EcritureTrace(XZSTC_WARNING, "OUT:ex_ret_LCR Eqt introuvable");
      return (XDC_NOK);
   }

   /*A Recherche s'il s'agit d'un bloc intermediaire : si oui fin traitement */

   vl_Position = (!strlen(pl_ListeEqt->TexteReponse)) ? ELCRC_DEBUT : ELCRC_FIN;

   strcpy(pl_ListeEqt->TexteReponse, va_trame);
   if (va_TypeRep == XZEXC_REP_NAK)
      strcpy(pl_ListeEqt->TexteReponse, "Reponse NAK");
   if (va_TypeRep == XZEXC_REP_ACK)
      strcpy(pl_ListeEqt->TexteReponse, "Reponse ACK");
   if (va_TypeRep == XZEXC_TRAME_NON_TRANSM)
      strcpy(pl_ListeEqt->TexteReponse, "Pas de reponse ");

   /*A Ecriture du resultat de la commande ds le fichier resultat */
   ex_ecriture_trace_lcr(pl_ListeEqt->TypeEqt,
                         pl_ListeEqt->NoEqt,
                         pl_ListeEqt->TexteCmd,
                         pl_ListeEqt->TexteReponse,
                         pl_ListeEqt->NomMachine,
                         vl_Position);

   strcpy(pl_eqt.NomFichResLCR, pl_ListeEqt->NomFichResLCR);
   pl_eqt.NoEqt = pl_ListeEqt->NoEqt;
   pl_eqt.TypeEqt = pl_ListeEqt->TypeEqt;
   strcpy(pl_eqt.TexteReponse, pl_ListeEqt->TexteReponse);
   ex_ecriture_resultat(&pl_eqt, vl_Position);

   if (va_TypeRep == XZEXC_REP_BLC_INT)
      return (XDC_OK);

   /* Positionnement du pointeur du fichier */
   if (strcmp(pl_ListeEqt->TexteReponse, XZEXC_DDE_ANNUL_CMD_LCR))
   {
      sprintf(pl_NomFichier, "%s/%s", XZEXC_PATH_FIC_LCR, pl_ListeEqt->NomFichCmdesLCR);

      XZST_03EcritureTrace(XZSTC_DEBUG1, "Ouverture fichier %s ", pl_NomFichier);
      if (((vl_Fd = open(pl_NomFichier, O_RDWR)) <= 0) ||
          (!lseek(vl_Fd, pl_ListeEqt->Position, SEEK_SET)))
      {
         XZST_03EcritureTrace(XZSTC_WARNING, "OUT:ex_ret_LCR Erreur sur fichier %s ", pl_NomFichier);
         ex_transfert_ficres_IP(pl_ListeEqt, pl_eqtprec, pg_DebutListeEqt, XDC_NOK);
         close(vl_Fd);
         return (XDC_NOK);
      }

      /*A Lecture commande suivante du fichier */

      vl_cmd = XDC_FAUX;
      while (ex_LireLigneFich(vl_Fd, pl_LigneFich, sizeof(pl_LigneFich)) != XZEXC_EOF)
      {
         /* Lire ligne suivante si c'est une ligne de commentaire */
         if (*pl_LigneFich != XDF_COMMENTAIRE)
         {
            vl_cmd = XDC_VRAI;
            break;
         }
         else
         {
            pl_ListeEqt->Position += (int)strlen(pl_LigneFich);
         }
      }
      close(vl_Fd);
   }

   XZST_03EcritureTrace(XZSTC_WARNING, "Trame reponse LCR:%s (Cmd:%s)", pl_ListeEqt->TexteReponse, pl_ListeEqt->TexteCmd);

   if ((vl_cmd == XDC_FAUX) || (!strcmp(pl_ListeEqt->TexteReponse, XZEXC_DDE_ANNUL_CMD_LCR)))
   {
      XZST_03EcritureTrace(XZSTC_DEBUG1, "Fin du fichier de commande atteinte Pos:%d ", pl_ListeEqt->Position);
      if ((pl_ListeEqt->Periode == 0) || (!strcmp(pl_ListeEqt->TexteReponse, XZEXC_DDE_ANNUL_CMD_LCR)))
      {
         vl_indiv = pl_ListeEqt->Indiv;
         /* Preparation de la commande cat */
         sprintf(vl_laCommande, "cat %s/%s_%d >> %s/%s",
                 XZEXC_PATH_FIC_LCR,
                 pl_ListeEqt->NomFichResLCR,
                 pl_ListeEqt->NoEqt,
                 XZEXC_PATH_FIC_LCR,
                 pl_ListeEqt->NomFichResLCR);
         /*  Execution !	*/
         vl_commande = popen(vl_laCommande, "r");
         if (vl_commande != NULL)
            pclose(vl_commande);
         sprintf(vl_laCommande, "%s/%s_%d", XZEXC_PATH_FIC_LCR, vl_ficres, pl_ListeEqt->NoEqt);
         XZSS_06DetruireFichier(vl_laCommande);

         /* Si travail termin� -> transfert du fichier resultat */
         if ((vl_indiv == XDC_VRAI) || ((vl_indiv == XDC_FAUX) && ((*pg_DebutListeEqt)->Suivant == NULL)))
         {
            ex_transfert_ficres_IP(pl_ListeEqt, pl_eqtprec, pg_DebutListeEqt, XDC_OK);
         }
         else
         {
            if ((pl_eqtprec == *pg_DebutListeEqt) && (pl_ListeEqt == pl_eqtprec))
               *pg_DebutListeEqt = pl_ListeEqt->Suivant;
            else
               pl_eqtprec->Suivant = pl_ListeEqt->Suivant;
            free(pl_ListeEqt);
         }
      }
      else
      {
         pl_ListeEqt->Position = 0;
      }
   }
   else
   {
      /*A Envoyer trame a la tache TELMi */

      if (XZSC_07NomMachine(vl_MachineLocale) != XDC_OK)
         XZST_03EcritureTrace(XZSTC_DEBUG1, "Recup. nom machine locale impossible");

      strcpy(pl_ListeEqt->TexteReponse, "");
      strcpy(pl_ListeEqt->TexteCmd, pl_LigneFich);
      if (EnvoyerTrameIP(pl_LigneFich,
                         XZEXC_TRAME_FEN_LCR,
                         pl_ListeEqt->AdresseIP,
                         pl_ListeEqt->Port,
                         pl_ListeEqt->AdresseRGS,
                         XZEXC_NOCMD,
                         XZEXC_PRIORITEFAIBLE,
                         pl_ListeEqt->Socket) == XDC_OK)
      {
         /* Ecriture de la commande dans le fichier resultat */
         pl_ListeEqt->Position = pl_ListeEqt->Position + (int)(strlen(pl_LigneFich) + 1);
         XZST_03EcritureTrace(XZSTC_FONCTION, "OUT:ex_ret_LCR");
         return (XDC_OK);
      }
      else
      {
         /* Ecriture de la commande dans le fichier resultat */
         strcpy ( pl_ListeEqt->TexteReponse, "Impossible de transmettre la commande" );
         ex_ecriture_resultat ( &pl_eqt, ELCRC_DEBUT );
         ex_transfert_ficres_IP ( pl_ListeEqt, pl_eqtprec, pg_DebutListeEqt, XDC_OK );
         XZST_03EcritureTrace( XZSTC_FONCTION, "OUT:ex_cmd_LCR Impossible d'emettre la trame vers TELMi." );
         return(XDC_NOK);
      }
   }
   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT:ex_ret_LCR_IP");
   return (XDC_OK);
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* Ecriture dans un fichier de trace de toutes les alertes
*
------------------------------------------------------
*
* ARGUMENTS EN ENTREE :
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR :
* XDC_OK : Succes
* XDC_NOK : KO
*
* CONDITION D'UTILISATION
*
*
* FONCTION
*
------------------------------------------------------*/
int ex_ecriture_trace_alerte(XDY_Eqt va_TypeEqt,
                             XDY_Eqt va_NumEqt,
                             char *pa_Texte)
{

   int vl_Fd = 0;
   long vl_PtrFile = 0;
   char pl_NomFichier[XDC_PATH_ABS_SIZE];
   XZEXT_TRAME_PS_NAV pl_LigneFich = "";
   XDY_Texte pl_Entete = "";
   XDY_NomEqt vl_NomTypeEqt = "";
   XDY_Nom vl_NomEqt = "";
   XDY_NomMachine vl_NomMachine = "";
   XZSMT_Horodate pl_StrHorodate;
   XDY_Horodate vl_Horodate;
   int vl_JourSem; /* jour de la semaine */
   static char vl_Entete[] = "#Pointeur dernier enregistrement: %09ld\n";
   int vl_CptCar = 0;

   /* Si le fichier de traces des commandes non LCR existe */
   if (va_TypeEqt != XDC_EQT_NIC)
   {
      XZEX03_nom_eqt((va_TypeEqt == XDC_EQT_PCT) ? XDC_EQT_PMV : va_TypeEqt, vl_NomTypeEqt);
      ex_nom_equipement(va_TypeEqt, va_NumEqt, NULL, vl_NomEqt, vl_NomMachine);
      sprintf(pl_NomFichier, "%s/Trace_Alerte%s_%s", XZEXC_PATH_FIC_LCR, vl_NomTypeEqt, vl_NomMachine);
   }
   else
   {
      sprintf(pl_NomFichier, "%s/Trace_Alerte_NIC", XZEXC_PATH_FIC_LCR);
      strcpy(vl_NomEqt, XDC_LIB_NIC);
   }

   if (!access(pl_NomFichier, F_OK))
   {
      /* Ouverture du fichier en Lecture et ecriture */
      chmod(pl_NomFichier, 0644);
      if ((vl_Fd = open(pl_NomFichier, O_RDWR)) <= 0)
      {
         XZST_03EcritureTrace(XZSTC_WARNING, "Ouverture du fichier  '%s' impossible ", pl_NomFichier);
         return (XDC_NOK);
      }

      /* Lecture de l'entete du fichier afin de connaitre la position du prochain enregistrement */
      if (ex_LireLigneFich(vl_Fd, pl_Entete, sizeof(XDY_Texte)) == XZEXC_EOF)
      {
         XZST_03EcritureTrace(XZSTC_WARNING, "Lecture entete fichier '%s' impossible", pl_NomFichier);
         close(vl_Fd);
         chmod(pl_NomFichier, 0444);
         return (XDC_NOK);
      }

      /* Extraction de la valeur du pointeur */
      if (sscanf(pl_Entete, vl_Entete, &vl_PtrFile) != 1)
      {
         XZST_03EcritureTrace(XZSTC_WARNING, "Pointeur fichier '%s' incorrect", pl_NomFichier);
         close(vl_Fd);
         chmod(pl_NomFichier, 0444);
         return (XDC_NOK);
      }

      /* Positionnement du pointeur du fichier */
      if (!lseek(vl_Fd, vl_PtrFile, SEEK_SET))
      {
         XZST_03EcritureTrace(XZSTC_WARNING, "Erreur positionnement sur fichier '%s' incorrect", pl_NomFichier);
         close(vl_Fd);
         chmod(pl_NomFichier, 0444);
         return (XDC_NOK);
      }
   }
   else
   {
      /* Creation du fichier de traces des alertes LCR */
      if ((vl_Fd = open(pl_NomFichier, O_RDWR | O_CREAT)) < 0)
      {
         XZST_03EcritureTrace(XZSTC_WARNING, "Impossible de creer le fichier '%s'", pl_NomFichier);
         return (XDC_NOK);
      }

      /* Ecriture de l'entete dans le fichier */
      sprintf(pl_Entete, vl_Entete, vl_PtrFile);
      // sem_wait(&vg_semaphore);
      if (write(vl_Fd, pl_Entete, strlen(pl_Entete)) < 0)
      {
         // sem_post(&vg_semaphore);
         XZST_03EcritureTrace(XZSTC_WARNING, "Ecriture entete fichier '%s' incorrect", pl_NomFichier);
         close(vl_Fd);
         chmod(pl_NomFichier, 0444);
         return (XDC_NOK);
      }
      else
      {
         // sem_post(&vg_semaphore);
         /* Mise a jour du pointeur */
         vl_PtrFile = strlen(pl_Entete);
      }
   }

   XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME, &vl_JourSem, &vl_Horodate);
   XZSM_11ConversionHorodate(vl_Horodate, &pl_StrHorodate);

   /* Enregistrement de la commande */
   if (pa_Texte != NULL)
      vl_CptCar = sprintf(pl_LigneFich, "\n%s Alerte : <%s> sur %s\n",
                          pl_StrHorodate,
                          pa_Texte,
                          vl_NomEqt);

   /* Si taille maxi du fichier d'alarme atteinte */
   if (vl_PtrFile + strlen(pl_LigneFich) > ELCRC_TailleFic)
   {
      /* Postionnement du pointeur au debut */
      vl_PtrFile = strlen(pl_Entete) + 1;
   }

   if (!lseek(vl_Fd, vl_PtrFile, SEEK_SET))
   {
      XZST_03EcritureTrace(XZSTC_WARNING, "Erreur positionnement sur fichier '%s'", pl_NomFichier);
      chmod(pl_NomFichier, 0444);
      return (XDC_NOK);
   }

   // sem_wait(&vg_semaphore);
   if (write(vl_Fd, pl_LigneFich, strlen(pl_LigneFich)) < 0)
   {
      // sem_post(&vg_semaphore);
      XZST_03EcritureTrace(XZSTC_WARNING, "Ecriture dans fichier '%s' impossible", pl_NomFichier);
      close(vl_Fd);
      chmod(pl_NomFichier, 0444);
      return (XDC_NOK);
   }
   else
   {
      // sem_post(&vg_semaphore);
      /* Mise a jour du pointeur */
      vl_PtrFile += strlen(pl_LigneFich);
      /* Enregistrement dans le fichier du nouveau pointeur */
      sprintf(pl_LigneFich, vl_Entete, vl_PtrFile);
      /* Positionnement du pointeur du fichier */
      if (lseek(vl_Fd, 0, SEEK_SET))
      {
         XZST_03EcritureTrace(XZSTC_WARNING, "Erreur positionnement sur fichier '%s' incorrect", pl_NomFichier);
         close(vl_Fd);
         chmod(pl_NomFichier, 0444);
         return (XDC_NOK);
      }

      // sem_wait(&vg_semaphore);
      if (write(vl_Fd, pl_LigneFich, strlen(pl_LigneFich)) < 0)
      {
         // sem_post(&vg_semaphore);
         XZST_03EcritureTrace(XZSTC_WARNING, "Ecriture entete fichier '%s' incorrect", pl_NomFichier);
         close(vl_Fd);
         chmod(pl_NomFichier, 0444);
         return (XDC_NOK);
      }
      // sem_post(&vg_semaphore);

      close(vl_Fd);
      chmod(pl_NomFichier, 0444);
   }

   return (XDC_OK);
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Envoi une trame a TEOPC.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int EnvoyerTrameOPC(XZEXT_Trame trame,
                    int type_msg,
                    int IdBICE,
                    int nocmd,
                    int priorite,
                    int socket)
/*
* ARGUMENTS EN ENTREE :
*  trame : trame a envoyer
*  type_msg : type du message
*  adr_rgs : adresse rgs
*  nocmd : numero de commande
*  priorite : priorite de la trame
*  socket : socket d'emision
*
* ARGUMENTS EN SORTIE :
*   aucun
*
*
* CODE RETOUR :
*   XDC_OK : succes
*   XDC_NOK : Erreur
*
* CONDITION D'UTILISATION
*
*   Fonction appele apres avoir construit la trame
*
* FONCTION
*  Demande d'envoi de trame a teopc
*
------------------------------------------------------*/
{
   static char *version = "$Id: ex_mlcr.c,v 1.37 2021/05/03 13:44:23 pc2dpdy Exp $ : EnvoyerTrame";

   XZEXT_MSG_SOCKET vl_message;
   ssize_t vl_RetCode = 0;

   XZST_03EcritureTrace(XZSTC_WARNING, "EnvoyerTrameOPC message en param : %s", trame);

   /*A Construction de la trame */
   sprintf(vl_message, "%s %d %d %d %d %s%s%s",
           XZEXC_DEMANDE_OPC,
           type_msg,
           IdBICE,
           nocmd,
           priorite,
           XZEXC_SEP_TRAME,
           trame,
           XZEXC_FIN_MSG_SOCK);

   fcntl(socket, F_SETFL, O_NDELAY | fcntl(socket, F_GETFL, 0));

   // sem_wait(&vg_semaphore);
   if ((vl_RetCode = write(socket, vl_message, strlen(vl_message))) < 0)
   {
      // sem_post(&vg_semaphore);
      /*B Ecriture dans le fichier trace en cas d'echec */
      XZST_03EcritureTrace(XZSTC_WARNING, "Impossible d'emettre la trame OPC (%s) socket =%d errno:%s", vl_message, socket, strerror(errno));
      return (XDC_NOK);
   }
   else
   {
      // sem_post(&vg_semaphore);
      /*B Ecriture dans le fichier trace en cas de succes */
      if (vl_RetCode != strlen(vl_message))
      {
         XZST_03EcritureTrace(XZSTC_WARNING, "Trame OPC INCOMPLETE emise <%s>", vl_message);
         return (XDC_OK);
      }
      else
      {
         XZST_03EcritureTrace(XZSTC_WARNING, "Trame OPC emise <%s>, socket:%d", vl_message, socket);
         return (XDC_OK);
      }
   }
}
