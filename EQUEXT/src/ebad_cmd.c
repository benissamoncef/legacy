/*E*/
/* Fichier : $Id: ebad_cmd.c,v 1.2 2007/11/20 08:35:15 pc2dpdy Exp $     Release : $Revision: 1.2 $       Date : $Date: 2007/11/20 08:35:15 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEBAD * FICHIER ebad_cmd.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module d'initialisation des structures en memoires
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	15/09/05	creation
* JMG   13/11/07        : ajout site de gestion DEM715
* LCL		20/12/21	: MOVIS P2 ems SAE-312
*
------------------------------------------------------ */

/* fichiers inclus */

#include "ebad_cmd.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

/* declaration de fonctions internes */
static void supprime_blancs_fin(char *);

/* definition de fonctions externes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Demande Init IHM nav.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void en_Init_IHM_contraste_cb(
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
*  XDM_EInit_IHM_Contraste
*
* FONCTION
*  Pour toutes les nav , envoyer l'etat  par XDM_ETAT_Contraste dans le DataGroup passe en parametre
*
------------------------------------------------------*/
{
   static char *version = "@(#)ebad_cmd.c	1.20 01/10/05 : en_Init_IHM_contraste_cb";

   T_STR pl_DGInit;
   EBAD_DONNEES_BAD *pl_listeBAD;

   XZST_03EcritureTrace(XZSTC_FONCTION, "IN: en_Init_IHM_contraste_cb ");

   /*A Recuperation des parametres du message XDM_EInit_IHM_Contraste */
   if (!TipcMsgRead(pa_Data->msg,
                    T_IPC_FT_STR, &pl_DGInit,
                    NULL))
   {
      /* Trace : Impossibilite de lire le message */
      XZST_03EcritureTrace(XZSTC_WARNING, "OUT: en_Init_IHM_contraste_cb messsage XDM_EInit_IHM_Contraste non lu.");
      return;
   }

   /*A Pour chaque nav : envoi du msg XDM_ETAT_Contraste si il est en service et actif */

   for (pl_listeBAD = pg_debutBAD; pl_listeBAD != NULL; pl_listeBAD = pl_listeBAD->Suivant)
   {
      /* si l'eqt est en service et actif */
      if (!(pl_listeBAD->Etat_Service & XDC_EQT_HORS_SRV))
      {
         if (!TipcSrvMsgWrite(pl_DGInit,
                              XDM_IdentMsg(XDM_ETAT_Contraste),
                              XDM_FLG_SRVMSGWRITE,
                              T_IPC_FT_INT2, XZECC_TYPE_EQT_BAD,
                              T_IPC_FT_INT2, pl_listeBAD->Config.Numero,
                              T_IPC_FT_INT2, (pl_listeBAD->ContrasteEtat == 'S') ? XDC_CONTRASTEBRILLANT : XDC_CONTRASTENORMAL,
                              NULL))
         {
            XZST_03EcritureTrace(XZSTC_WARNING, "Envoi message XDM_ETAT_Contraste non effectue.");
         }
         else
         {
            XZST_03EcritureTrace(XZSTC_MESSAGE, "Envoi message XDM_ETAT_Contraste effectue.");
         }
      }
   }

   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: en_Init_IHM_contraste_cb ");
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Demande Init IHM Liaison.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void ep_Init_IHM_Liaison_cb(
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
*  XDM_EInit_IHM_Liaison
*
* FONCTION
*  Pour touts les BAD , envoyer l'etat  par XDM_ETAT_LIAISON dans le DataGroup passe en parametre
*
------------------------------------------------------*/
{
   static char *version = "@(#)ebad_cmd.c	1.20 01/10/05 :  ep_Init_IHM_Liaison_cb ";

   T_STR pl_DGInit;
   EBAD_DONNEES_BAD *pl_ListeBAD;

   XZST_03EcritureTrace(XZSTC_FONCTION, "IN: ep_Init_IHM_Liaison_cb ");

   /*A Recuperation des parametres du message  */
   if (!TipcMsgRead(pa_Data->msg,
                    T_IPC_FT_STR, &pl_DGInit,
                    NULL))
   {
      /* Trace : Impossibilite de lire le message */
      XZST_03EcritureTrace(XZSTC_WARNING, "OUT: ep_Init_IHM_Liaison_cb messsage XDM_EInit_IHM_Liaison non lu \n");
      return;
   }

   /*A Pour chaque BAD : envoi du msg XDM_ETAT_LIAISON  */

   for (pl_ListeBAD = pg_debutBAD; pl_ListeBAD != NULL; pl_ListeBAD = pl_ListeBAD->Suivant)
   {
      ex_env_etat_liaison(vg_NomMachine, pl_DGInit, pl_ListeBAD->Config.Numero, XDC_EQT_BAD);
   }

   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: ep_Init_IHM_Liaison_cb ");
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Demande Init IHM BAD.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void en_Init_IHM_BAD_cb(
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
*  XDM_EInit_IHM_BAD
*
* FONCTION
*  Pour touts les BAD, envoyer l'etat BAD par XDM_ETAT_BAD dans le DataGroup passe en parametre
*
------------------------------------------------------*/
{
   static char *version = "@(#)ebad_cmd.c	1.20 01/10/05 : ep_Init_IHM_BAD_cb";

#ifdef _TIBCO_EMS
   XDY_Datagroup pl_DGInit;
#else
   T_STR pl_DGInit;
#endif
   EBAD_DONNEES_BAD *pl_ListeBAD;
   int i;
   char **pl;
   static char *pl_Tab[XZEXC_nb_module_max_BAD];
   char vl_str[1024];

   XZST_03EcritureTrace(XZSTC_FONCTION, "IN: ep_Init_IHM_BAD_cb ");

   /*A Recuperation des parametres du message XDM_EInit_IHM_BAD */
   if (!TipcMsgRead(pa_Data->msg,
                    T_IPC_FT_STR, &pl_DGInit,
                    NULL))
   {
      /* Trace : Impossibilite de lire le message */
      XZST_03EcritureTrace(XZSTC_WARNING, "OUT: ep_Init_IHM_BAD_cb messsage XDM_EInit_IHM_BAD non lu.");
      return;
   }

   /*A Pour chaque BAD : envoi du msg XDM_ETAT_BAD */

   for (pl_ListeBAD = pg_debutBAD; pl_ListeBAD != NULL; pl_ListeBAD = pl_ListeBAD->Suivant)
   {
      /* si l'eqt est en service et actif */
      if (!(pl_ListeBAD->Etat_Service & XDC_EQT_HORS_SRV))
      {
         XZST_03EcritureTrace(XZSTC_DEBUG1, "CLUCHAGUE nav no:%d", pl_ListeBAD->Config.Numero);
         strcpy(vl_str, "");

         for (i = 0; i < XZEXC_nb_module_max_BAD; i++)
         {
            if (strcmp(vl_str, ""))
               sprintf(vl_str, "%s|%s", vl_str, pl_ListeBAD->Etat.Module[i]);
            else
               sprintf(vl_str, "%s", pl_ListeBAD->Etat.Module[i]);

            pl_Tab[i] = pl_ListeBAD->Etat.Module[i];
            XZST_03EcritureTrace(XZSTC_DEBUG1, "CLUCHAGUE etat module no:%d=%s", i, pl_Tab[i]);
         }

         if (!TipcSrvMsgWrite(pl_DGInit,
                              XDM_IdentMsg(XDM_ETAT_BAD),
                              XDM_FLG_SRVMSGWRITE,
                              T_IPC_FT_INT2, pl_ListeBAD->Config.Numero,
                              T_IPC_FT_INT2, pl_ListeBAD->Config.Type,
                              T_IPC_FT_STR_ARRAY, pl_Tab, XZEXC_nb_module_max_BAD,
                              NULL))
         {
            XZST_03EcritureTrace(XZSTC_WARNING, "Envoi message XDM_ETAT_BAD non effectue.");
         }
         else
         {
            XZST_03EcritureTrace(XZSTC_MESSAGE, "Envoi message XDM_ETAT_BAD effectue no:%d type:%d nb modules:%d ", pl_ListeBAD->Config.Numero,
                                 pl_ListeBAD->Config.Type, XZEXC_nb_module_max_BAD);
         }
         if (!TipcSrvMsgWrite(pl_DGInit,
                              XDM_IdentMsg(XDM_ETAT_BAD_EMS),
                              XDM_FLG_SRVMSGWRITE,
                              T_IPC_FT_INT2, pl_ListeBAD->Config.Numero,
                              T_IPC_FT_INT2, pl_ListeBAD->Config.Type,
                              T_IPC_FT_STR, vl_str,
                              NULL))
         {
            XZST_03EcritureTrace(XZSTC_WARNING, "Envoi message XDM_ETAT_BAD non effectue. vl_str=[%s]");
         }
         else
         {
            XZST_03EcritureTrace(XZSTC_MESSAGE, "Envoi message XDM_ETAT_BAD effectue no:%d type:%d nb modules:%d vl_str=[%s]", pl_ListeBAD->Config.Numero,
                                 pl_ListeBAD->Config.Type, XZEXC_nb_module_max_BAD, vl_str);
         }
      }
   }
   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: ep_Init_IHM_BAD_cb ");
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Demande de commande nav
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void en_cmd_BAD_cb(
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
*
* Fonction executee sur reception du message XDM_ECMD_BAD
*
* FONCTION
*  Piloter le module concern�
*
------------------------------------------------------*/
{
   static char *version = "@(#)ebad_cmd.c	1.20 01/10/05 : en_cmd_BAD_cb";

#ifdef _TIBCO_EMS
   XDY_Mot vl_NoEqtBAD;
   XDY_Etat_Module vl_Etat;
   XDY_Mot vl_Module;
   XDY_Octet vl_Flash;
   XDY_Entier vl_NoCMD;
#else
   T_INT2 vl_NoEqtBAD;
   T_STR vl_Etat;
   T_INT2 vl_Module;
   T_INT2 vl_Flash;
   T_INT4 vl_NoCMD;
#endif

   EBAD_DONNEES_BAD *pl_listeBAD;
   int vl_Index, vl_js;
   XZEXT_Trame pl_mlcr;
   XDY_Horodate vl_horodate;

   XZST_03EcritureTrace(XZSTC_FONCTION, "IN: en_cmd_BAD_cb");

   /*A Recuperation des parametres du message XDM_ECMD_BAD */
   if (!TipcMsgRead(pa_Data->msg, T_IPC_FT_INT2, &vl_NoEqtBAD, T_IPC_FT_INT2, &vl_Module, T_IPC_FT_STR, &vl_Etat, T_IPC_FT_INT2, &vl_Flash, T_IPC_FT_INT4, &vl_NoCMD, NULL))
   {
      /* Trace : Impossibilite de lire le message */
      XZST_03EcritureTrace(XZSTC_WARNING, "OUT: en_cmd_BAD_cb messsage XDM_ECMD_BAD non lu.");
      return;
   }
   else
   {
      /* Trace du message */
      XZST_03EcritureTrace(XZSTC_INFO, "mes cmd nav = %d %d %s %d %d", vl_NoEqtBAD, vl_Module, vl_Etat, vl_Flash, vl_NoCMD);
   }

   /*A Verif des parametres en entree */

   for (pl_listeBAD = pg_debutBAD; pl_listeBAD != NULL; pl_listeBAD = pl_listeBAD->Suivant)
   {
      if (pl_listeBAD->Config.Numero == vl_NoEqtBAD)
         break;
   }

   /* Numero eqt nav invalide */
   if (pl_listeBAD == NULL)
   {
      /* message administrateur systeme */
      XZST_03EcritureTrace(XZSTC_WARNING, "OUT: en_cmd_BAD_cb Message XDM_ECMD_BAD recu avec no eqt nav invalide");
      return;
   }

   /*A si cmd en cours refus de la commande */
   if ((*pl_listeBAD->Etat.Scenario != '\0') || (pl_listeBAD->EtatCmd.Module[vl_Module][0] != '\0'))
   {
      /* Trace */
      XZST_03EcritureTrace(XZSTC_WARNING, "Commande refusee car commande en cours");
      XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: en_cmd_SCEN_cb");
      return;
   }

   /* Verif de l'etat d'activite et de l'etat de service */
   if ((pl_listeBAD->Etat_Service & XDC_EQT_HORS_SRV) ||
       (pl_listeBAD->Config.Liste_Module[vl_Module].Etat_Service & XDC_EQT_HORS_SRV))
   {
      XZST_03EcritureTrace(XZSTC_WARNING, "en_cmd_BAD_cb : Message BAD ou Module commande invalide");
      XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: en_cmd_BAD_cb ");
      return;
   }

   /*A Recherche si Module valide */

   if ((vl_Module >= XZEXC_nb_module_max_BAD) || (pl_listeBAD->Config.Liste_Module[vl_Module].NoModule == 0))
   {
      /* message administrateur systeme */
      XZST_03EcritureTrace(XZSTC_WARNING, "eOUT: n_cmd_BAD_cb : Message XDM_ECMD_BAD recu avec No Module nav invalide");
      return;
   }

   /*A Construction de ma trame */
   sprintf(pl_mlcr, "P ID=%s AM=%s AF=%s EC=%s",
           vg_idpassw,
           pl_listeBAD->Config.Liste_Module[vl_Module].Module,
           vl_Etat,
           (pl_listeBAD->Contraste == XDC_CONTRASTEBRILLANT) ? "S" : ((vg_Contexte == XDC_JOUR) ? "J1" : (vg_Contexte == XDC_NUIT) ? "N1"
                                                                                                                                   : "A"));

   /*A Envoi de la trame */
   if (EnvoyerTrame(pl_mlcr,
                    XZEXC_TRAME_CMD_BAD,
                    pl_listeBAD->Config.AdresseRGS,
                    0,
                    XZEXC_PRIORITEFAIBLE,
                    pl_listeBAD->Socket) == XDC_NOK)
   {
      XZST_03EcritureTrace(XZSTC_WARNING, "Impossible d'envoyer la commande BAD");
      return;
   }

   /* Ecriture dans fichier Trace_CMD_BAD de la commande */
   ex_ecriture_trace_cmd(XDC_EQT_BAD,
                         vl_NoEqtBAD,
                         pl_mlcr,
                         NULL,
                         vl_Module + 1);

   /*A envoi de la trame de relecture */
   if (EnvoyerTrame("PS",
                    XZEXC_TRAME_ETAT_POS_BAD,
                    pl_listeBAD->Config.AdresseRGS,
                    vl_Module + 1,
                    XZEXC_PRIORITEFAIBLE,
                    pl_listeBAD->Socket) == XDC_NOK)
   {
      return;
   }

   /* Mise a jour memoire commande BAD */
   strcpy(pl_listeBAD->EtatCmd.Module[vl_Module], vl_Etat);
   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: en_cmd_BAD_cb ");
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Demande de commande scen nav
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void en_cmd_SCEN_cb(
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
*
* Fonction executee sur reception du message XDM_ECMD_SCEN
*
* FONCTION
*  Piloter la nav par scenario
*
------------------------------------------------------*/
{
   static char *version = "@(#)ebad_cmd.c	1.20 01/10/05 : en_cmd_SCEN_cb";

#ifdef _TIBCO_EMS
   XDY_Mot vl_NoEqtBAD;
   char vl_Scenar[100];
   XDY_Entier vl_NoCMD;
#else
   T_INT2 vl_NoEqtBAD;
   T_STR vl_Scenar;
   T_INT4 vl_NoCMD;
#endif

   EBAD_DONNEES_BAD *pl_listeBAD;
   EBAD_LISTE_SCEN *pl_listeSCEN;

   int vl_Index, vl_js, i;
   XZEXT_Trame pl_mlcr;
   XDY_Horodate vl_horodate;
   char vl_Scenario[10] = "";

   XZST_03EcritureTrace(XZSTC_FONCTION, "IN: en_cmd_SCEN_cb");

   /*A Recuperation des parametres du message XDM_ECMD_BAD */
   if (!TipcMsgRead(pa_Data->msg, 
                     T_IPC_FT_INT2, &vl_NoEqtBAD, 
                     T_IPC_FT_STR, &vl_Scenar, 
                     T_IPC_FT_INT4, &vl_NoCMD, 
                     NULL))
   {
      /* Trace : Impossibilite de lire le message */
      XZST_03EcritureTrace(XZSTC_WARNING, "messsage XDM_ECMD_BAD non lu.");
      XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: en_cmd_SCEN_cb");
      return;
   }
   else
   {
      /* Trace du message */
      supprime_blancs_fin(vl_Scenar);
      XZST_03EcritureTrace(XZSTC_INFO, "mes cmd bad = %d %s %d", vl_NoEqtBAD, vl_Scenar, vl_NoCMD);
   }

   /* verif no eqt nav */
   for (pl_listeBAD = pg_debutBAD; pl_listeBAD != NULL; pl_listeBAD = pl_listeBAD->Suivant)
   {
      if (pl_listeBAD->Config.Numero == vl_NoEqtBAD)
         break;
   }

   /* Numero eqt nav invalide */
   if (pl_listeBAD == NULL)
   {
      if (vl_NoCMD != 0)
      {
         /* message administrateur systeme */
         XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME, &vl_js, &vl_horodate);
         xzat24_CompteRendu_Nav(vl_NoCMD, vl_horodate, XDC_NOK, 0);
      }
      XZST_03EcritureTrace(XZSTC_WARNING, "OUT: en_cmd_SCEN_cb avec no eqt nav invalide");
      return;
   }

   /*A si cmd en cours refus de la commande */
   if (*pl_listeBAD->Etat.Scenario != '\0')
   {
      if (vl_NoCMD != 0)
      {
         XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME, &vl_js, &vl_horodate);
         xzat24_CompteRendu_Nav(vl_NoCMD, vl_horodate, XDC_NOK, pl_listeBAD->Config.SiteGestion);
      }
      /* Trace */
      XZST_03EcritureTrace(XZSTC_WARNING, "Commande refusee car commande en cours");
      XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: en_cmd_SCEN_cb");
      return;
   }

   /* Verif de l'etat d'activite et de l'etat de service */
   if ((pl_listeBAD->Etat_Service & XDC_EQT_HORS_SRV) ||
       ((pl_listeBAD->Etat_Service & XDC_EQT_INACTIF) && (vl_NoCMD != 0)) ||
       ((pl_listeBAD->Etat_Service & XDC_EQT_INHIBE) && (vl_NoCMD != 0)))
   {
      if (vl_NoCMD != 0)
      {
         /* message administrateur systeme */
         XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME, &vl_js, &vl_horodate);
         xzat24_CompteRendu_Nav(vl_NoCMD, vl_horodate, XDC_NOK, pl_listeBAD->Config.SiteGestion);
      }
      XZST_03EcritureTrace(XZSTC_WARNING, "OUT: en_cmd_SCEN_cb commande pour eqt HS ou inactif");
      return;
   }

   /*A Construction de ma trame */
   sprintf(pl_mlcr, "PM ID=%s AF=%s", vg_idpassw, vl_Scenar);

   /*A Envoi de la trame */
   if (EnvoyerTrame(pl_mlcr,
                    XZEXC_TRAME_CMD_SCEN_BAD,
                    pl_listeBAD->Config.AdresseRGS,
                    0,
                    XZEXC_PRIORITEFAIBLE,
                    pl_listeBAD->Socket) == XDC_NOK)
   {
      XZST_03EcritureTrace(XZSTC_WARNING, "Impossible d'envoyer la commande scenario");
      if (vl_NoCMD != 0)
      {
         XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME, &vl_js, &vl_horodate);
         xzat24_CompteRendu_Nav(vl_NoCMD, vl_horodate, XDC_NOK, pl_listeBAD->Config.SiteGestion);
      }
      XZST_03EcritureTrace(XZSTC_WARNING, "OUT: en_cmd_SCEN_cb Impossible d'envoyer la commande");
      return;
   }

   /* Ecriture dans fichier Trace_CMD_BAD de la commande */
   ex_ecriture_trace_cmd(XDC_EQT_BAD,
                         vl_NoEqtBAD,
                         pl_mlcr,
                         NULL,
                         vl_NoCMD);

   /*A mise en memoire du scenario */
   strcpy(pl_listeBAD->Etat.Scenario, vl_Scenario);
   pl_listeBAD->NumCmd = (vl_NoCMD == 0) ? XZEXC_CMD_HORS_BASE : vl_NoCMD;

   /*A envoi de la trame de relecture */
   if (EnvoyerTrame("PS",
                    XZEXC_TRAME_ETAT_POS_BAD,
                    pl_listeBAD->Config.AdresseRGS,
                    0,
                    XZEXC_PRIORITEFAIBLE,
                    pl_listeBAD->Socket) == XDC_NOK)
   {
      return;
   }

   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: en_cmd_SCEN_cb ");
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Demande d'arret scen nav
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void en_cmd_Arret_SCEN_cb(
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
*
* Fonction executee sur reception du message XDM_ECMD_ARRET_SCEN
*
* FONCTION
*  Arret pilotage nav par scenario
*
------------------------------------------------------*/
{
   static char *version = "@(#)ebad_cmd.c	1.20 01/10/05 : en_cmd_Arret_SCEN_cb";

#ifdef _TIBCO_EMS
   XDY_Mot vl_NoEqtBAD;
   XDY_Entier vl_NoCMD;
#else
   T_INT2 vl_NoEqtBAD;
   T_INT4 vl_NoCMD;
#endif

   EBAD_DONNEES_BAD *pl_listeBAD;

   int vl_Index, vl_js;
   XZEXT_Trame pl_mlcr;
   XDY_Horodate vl_horodate;

   XZST_03EcritureTrace(XZSTC_FONCTION, "IN: en_cmd_Arret_SCEN_cb");

   /*A Recuperation des parametres du message XDM_ECMD_ARRET_SCEN */
   if (!TipcMsgRead(pa_Data->msg, T_IPC_FT_INT2, &vl_NoEqtBAD, T_IPC_FT_INT4, &vl_NoCMD, NULL))
   {
      /* Trace : Impossibilite de lire le message */
      XZST_03EcritureTrace(XZSTC_WARNING, "messsage XDM_ECMD_ARRET_SCEN non lu.");
      XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: en_cmd_Arret_SCEN_cb");
      return;
   }
   else
   {
      /* Trace du message */
      XZST_03EcritureTrace(XZSTC_INFO, "Demande Arret Scenario ");
   }

   /*A Verif des paramestes en entree */

   /* verif no eqt nav */
   for (pl_listeBAD = pg_debutBAD; pl_listeBAD != NULL; pl_listeBAD = pl_listeBAD->Suivant)
   {
      if (pl_listeBAD->Config.Numero == vl_NoEqtBAD)
         break;
   }

   /* Numero eqt nav invalide */
   if (pl_listeBAD == NULL)
   {
      /* message administrateur systeme */
      XZST_03EcritureTrace(XZSTC_WARNING, "OUT: en_cmd_SCEN_cb avec no eqt nav invalide");
      return;
   }

   /*A Construction de ma trame */
   sprintf(pl_mlcr, "BRK ID=%s", vg_idpassw);

   /*A Envoi de la trame */
   if (EnvoyerTrame(pl_mlcr,
                    XZEXC_TRAME_CMD_ARRET_SCEN_BAD,
                    pl_listeBAD->Config.AdresseRGS,
                    0,
                    XZEXC_PRIORITEFAIBLE,
                    pl_listeBAD->Socket) == XDC_NOK)
   {
      XZST_03EcritureTrace(XZSTC_WARNING, "OUT: en_cmd_Arret_SCEN_cb Impossible d'envoyer la commande arret scenario");
      return;
   }

   /* Ecriture dans fichier Trace_CMD_BAD de la commande */
   ex_ecriture_trace_cmd(XDC_EQT_BAD,
                         vl_NoEqtBAD,
                         pl_mlcr,
                         NULL,
                         0);

   EnvoyerTrame("ST",
                XZEXC_TRAME_STAT_COMP,
                pl_listeBAD->Config.AdresseRGS,
                XZEXC_CMD_AU_NAV,
                XZEXC_PRIORITEFAIBLE,
                pl_listeBAD->Socket);

   pl_listeBAD->ArretUrg = XZEXC_CMD_AU_NAV;

   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: en_cmd_SCEN_cb ");
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Envoi de la trace de contraste suivant le contraste et Jour/Nuit pour le BAD
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int em_envoyer_contraste_BAD(EBAD_DONNEES_BAD *pa_ListeBAD,
                             int va_Contraste,
                             int va_NumCmd)
/*
* ARGUMENTS EN ENTREE :
*  pa_ListeBAD : pointeur sur liste BAD
*  va_Contraste : Contraste a envoyer
*
* ARGUMENTS EN SORTIE :
*   XDC_OK : succes
*   XDC_NOK : Erreur
*
*
* CODE RETOUR :
*   aucun
*
* CONDITION D'UTILISATION
*
*
* FONCTION
*
*  Envoi de la trame de contraste a telmi pour le BAD
*
------------------------------------------------------*/
{
   static char *version = "@(#)ebad_cmd.c	1.20 01/10/05 :  em_envoyer_contraste_BAD ";

   XZEXT_Trame vl_lcr = "";
   char vl_Contraste[3];

   /*A Choix de la commande en fonction du contexte jour nuit */
   switch (va_Contraste)
   {
   case XDC_CONTRASTEBRILLANT:
      strcpy(vl_Contraste, "S");
      break;
   case XDC_CONTRASTENORMAL:
      strcpy(vl_Contraste, (vg_Contexte == XDC_JOUR) ? "J1" : (vg_Contexte == XDC_NUIT) ? "N1"
                                                                                        : "A");
      break;
   }

   /*A envoi de la trame de contraste */
   sprintf(vl_lcr, "P ID=%s AM=* EC=%s", vg_idpassw, vl_Contraste);
   EnvoyerTrame(vl_lcr,
                XZEXC_TRAME_CONTRASTE,
                pa_ListeBAD->Config.AdresseRGS,
                XZEXC_NOCMD,
                XZEXC_PRIORITEFAIBLE,
                pa_ListeBAD->Socket);

   /*A envoi de la trame de relecture */
   if (va_NumCmd != XZEXC_NOCMD)
   {
      if (EnvoyerTrame("PS",
                       XZEXC_TRAME_ETAT_POS_BAD,
                       pa_ListeBAD->Config.AdresseRGS,
                       va_NumCmd,
                       XZEXC_PRIORITEFAIBLE,
                       pa_ListeBAD->Socket) == XDC_NOK)
      {
         return (XDC_NOK);
      }
   }

   if (va_NumCmd != XZEXC_NOCMD)
      pa_ListeBAD->ContrasteCmd = (va_Contraste == XDC_CONTRASTEBRILLANT) ? 'S' : (vg_Contexte == XDC_JOUR) ? 'J'
                                                                              : (vg_Contexte == XDC_NUIT)   ? 'N'
                                                                                                            : 'A';
   return (XDC_OK);
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Demande de changement de contraste .
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void en_contraste_cb(
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
*  XDM_ECMD_Contraste
*
* FONCTION
*
*  suivant le type d'equipement envoi du message de contraste
*
------------------------------------------------------*/
{
   static char *version = "@(#)ebad_cmd.c	1.20 01/10/05 : ep_contraste_cb ";

#ifdef _TIBCO_EMS
   XDY_Mot va_Contraste;
   XDY_Mot va_NoEqt;
   XDY_Mot va_TypeEqt;
#else
   T_INT2 va_Contraste;
   T_INT2 va_NoEqt;
   T_INT2 va_TypeEqt;
#endif
   EBAD_DONNEES_BAD *pl_ListeBAD;
   XZEXT_Trame vl_lcr = "";

   XZST_03EcritureTrace(XZSTC_FONCTION, "IN: ep_contraste_cb");

   /* Recuperation des parametres du message XDM_ECMD_Contraste */
   if (!TipcMsgRead(pa_Data->msg, T_IPC_FT_INT2, &va_Contraste, T_IPC_FT_INT2, &va_NoEqt, T_IPC_FT_INT2, &va_TypeEqt, NULL))
   {
      /* Trace : Impossibilite de lire le message */
      XZST_03EcritureTrace(XZSTC_WARNING, "messsage XDM_ECMD_Contraste non lu.");
      XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: ep_contraste_cb");
      return;
   }

   if (va_TypeEqt == XZECC_TYPE_EQT_BAD)
   {

      /*B Recherche du No d'equipement dans la structure config_BAD en memoire */
      for (pl_ListeBAD = pg_debutBAD; pl_ListeBAD != NULL;
           pl_ListeBAD = pl_ListeBAD->Suivant)
      {
         if ((pl_ListeBAD->Config.Numero == va_NoEqt) ||
             (va_NoEqt == XZEXC_SURB_EQTALL))
         {
            /*B Composition de la trame contraste et envoi */
            pl_ListeBAD->Contraste = va_Contraste;
            em_envoyer_contraste_BAD(pl_ListeBAD, va_Contraste, XZEXC_CMD_HORS_BASE);
            if (pl_ListeBAD->Config.Numero == va_NoEqt)
               break;
         }
      }
   }

   /*A Trace sortie fonction ep_contraste_cb */
   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: ep_contraste_cb");
}
/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Message de contr�le.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void en_controle_cb(
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
*  XDM_ECMD_EQPT
*
* FONCTION
* Met a jour les variables d'etat dans les structures en memoire
*
------------------------------------------------------*/
{
   static char *version = "@(#)ebad_cmd.c	1.20 01/10/05 : en_controle_cb  ";

#ifdef _TIBCO_EMS
   XDY_Entier va_Etat;
   XDY_NomMachine va_NomMachine;
   XDY_Entier va_TypeEqt;
   XDY_Entier va_NumEqt;
#else
   T_INT4 va_Etat;
   T_STR va_NomMachine;
   T_INT4 va_TypeEqt;
   T_INT4 va_NumEqt;
#endif
   EBAD_DONNEES_BAD *pl_ListeBAD;
   int vl_Index;

   XZST_03EcritureTrace(XZSTC_FONCTION, "IN: en_controle_cb ");

   /*A Recuperation des parametres du message XDM_ECMD_EQPT */
   if (!TipcMsgRead(pa_Data->msg, T_IPC_FT_INT4, &va_Etat, T_IPC_FT_STR, &va_NomMachine, T_IPC_FT_INT4, &va_TypeEqt, T_IPC_FT_INT4, &va_NumEqt, NULL))

   {
      /* Trace : Impossibilite de lire le message */
      XZST_03EcritureTrace(XZSTC_WARNING, "messsage XDM_ECMD_EQPT non lu.");
   }

   /* Trace du message en entree de la fonction */
   XZST_03EcritureTrace(XZSTC_WARNING, "callback ep_controle_cb, msg= %d %s %d %d",
                        va_Etat,
                        va_NomMachine,
                        va_TypeEqt,
                        va_NumEqt);

   /*A si l'eqt est une BAD */
   if (va_TypeEqt == XZECC_TYPE_EQT_BAD)
   {
      /*B Recherche du No d'equipement dans la structure config_BAD en memoire */
      for (pl_ListeBAD = pg_debutBAD; pl_ListeBAD != NULL;
           pl_ListeBAD = pl_ListeBAD->Suivant)
      {
         if (pl_ListeBAD->Config.Numero == va_NumEqt)
         {
            if (va_Etat > XZEXC_LIMITE_ETAT_EQT)
               pl_ListeBAD->Etat_Service &= va_Etat;
            else
               pl_ListeBAD->Etat_Service |= va_Etat;
            break;
         }
      }
   }

   /*A si l'eqt est un module BAD */
   if (va_TypeEqt == XDC_EQT_MODBAD)
   {
      /*B Recherche du No d'equipement dans la structure config_BAD en memoire */
      for (pl_ListeBAD = pg_debutBAD; pl_ListeBAD != NULL;
           pl_ListeBAD = pl_ListeBAD->Suivant)
      {
         for (vl_Index = 0; vl_Index != XZEXC_nb_module_max_BAD; vl_Index++)
         {
            if (pl_ListeBAD->Config.Liste_Module[vl_Index].NoModule == va_NumEqt)
            {
               if (va_Etat > XZEXC_LIMITE_ETAT_EQT)
                  pl_ListeBAD->Config.Liste_Module[vl_Index].Etat_Service &= va_Etat;
               else
                  pl_ListeBAD->Config.Liste_Module[vl_Index].Etat_Service |= va_Etat;

               /*                  if ((va_Etat == XDC_EQT_HORS_SRV) || (va_Etat == XDC_EQT_EN_SRV))
                                 {
                                    pl_ListeBAD->Config.Liste_Module[vl_Index].Etat_Service = va_Etat;
                                 }
                                 if ((va_Etat == XDC_EQT_ACTIF) || (va_Etat == XDC_EQT_INACTIF))
                                 {
                                    pl_ListeBAD->Config.Liste_Module[vl_Index].Etat_Activite = va_Etat;
                                 }*/
               break;
            }
         }
         if (pl_ListeBAD->Config.Liste_Module[vl_Index].NoModule == va_NumEqt)
            break;
      }
   }

   /* Trace des Differents etats service et activite */
   for (pl_ListeBAD = pg_debutBAD; pl_ListeBAD != NULL;
        pl_ListeBAD = pl_ListeBAD->Suivant)
   {
      XZST_03EcritureTrace(XZSTC_DEBUG1, "servBAD:%d ", pl_ListeBAD->Etat_Service);
   }

   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: en_controle_cb ");
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Changement de contexte.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void en_journuit(XZEAT_JourNuit va_contexte, int va_resultat)

/*
* ARGUMENTS EN ENTREE :
*
*
* ARGUMENTS EN SORTIE :
*   vl_resultat
*
*
* CODE RETOUR :
*   aucun
*
* CONDITION D'UTILISATION
*
* FONCTION
* Met a jour la variable vg_contexte
*
------------------------------------------------------*/
{
   static char *version = "@(#)ebad_cmd.c	1.20 01/10/05 : en_journuit  ";
   XZEXT_Trame vl_lcr = "";
   char vl_Contraste[3];
   EBAD_DONNEES_BAD *pl_ListeBAD;

   /*A si le contexte recu est jour : contexte = jour sinon contexte = nuit */
   if (va_contexte.JourNuit == XDC_JOUR)
      vg_Contexte = XDC_JOUR;
   else
      vg_Contexte = XDC_NUIT;

   XZST_03EcritureTrace(XZSTC_INFO, "Nouveau contexte : %d", vg_Contexte);

   /*B Pour tout les BAD en memoire */
   for (pl_ListeBAD = pg_debutBAD; pl_ListeBAD != NULL;
        pl_ListeBAD = pl_ListeBAD->Suivant)
   {
      /* Envoyer nouveau contraste */
      em_envoyer_contraste_BAD(pl_ListeBAD, pl_ListeBAD->Contraste, XZEXC_CMD_HORS_BASE);
   }
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Demande de fenetre LCR
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void en_CMD_LCR(T_IPC_CONN va_Cnx,
                T_IPC_CONN_PROCESS_CB_DATA data,
                T_CB_ARG arg)

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
*  XDM_ELCR
*
* FONCTION
*  Demande fenetre LCR
*
------------------------------------------------------*/
{
   static char *version = "@(#)ebad_cmd.c	1.20 01/10/05 : en_CMD_LCR";

#ifdef _TIBCO_EMS
   XDY_Mot vl_TypeEqt;
   XDY_Mot vl_NoEqt;
   XDY_FichEquext vl_FicCmdLcr;
   XDY_FichEquext vl_FicResLcr;
   XDY_Mot vl_Periode;
   XDY_Horodate vl_HrDebut;
   XDY_Horodate vl_HrFin;
   XDY_NomMachine vl_NomMachine;
#else
   T_INT2 vl_TypeEqt;
   T_INT2 vl_NoEqt;
   T_STR vl_FicCmdLcr;
   T_STR vl_FicResLcr;
   T_INT2 vl_Periode;
   T_REAL8 vl_HrDebut;
   T_REAL8 vl_HrFin;
   T_STR vl_NomMachine;
#endif
   EBAD_DONNEES_BAD *pl_ListeBAD;
   INFO_EQT_FEN_LCR pl_eqt, *pl_ListeEqt;
   XZEXT_MSG_SOCKET pl_MsgTimer;
   XZEXT_CLE_PROG vl_clef;

   /*A Lecture du message */

   if (!TipcMsgRead(data->msg,
                    T_IPC_FT_INT2, &vl_TypeEqt,
                    T_IPC_FT_INT2, &vl_NoEqt,
                    T_IPC_FT_STR, &vl_FicCmdLcr,
                    T_IPC_FT_STR, &vl_FicResLcr,
                    T_IPC_FT_INT2, &vl_Periode,
                    T_IPC_FT_REAL8, &vl_HrDebut,
                    T_IPC_FT_REAL8, &vl_HrFin,
                    T_IPC_FT_STR, &vl_NomMachine,
                    NULL))

   {
      /* Trace : Impossibilite de lire le message */
      XZST_03EcritureTrace(XZSTC_WARNING, "OUT: ep_CMD_LCR messsage XDM_ELCR non lu.");
      return;
   }

   XZST_03EcritureTrace(XZSTC_FONCTION, "IN ep_CMD_LCR");

   /*A Construction de la liste globale des eqt a piloter par la fenetre LCR */

   pl_eqt.TypeEqt = vl_TypeEqt;
   strcpy(pl_eqt.NomFichCmdesLCR, vl_FicCmdLcr);
   strcpy(pl_eqt.NomFichResLCR, vl_FicResLcr);
   pl_eqt.Periode = vl_Periode;
   pl_eqt.HrDebut = vl_HrDebut;
   pl_eqt.HrFin = vl_HrFin;
   strcpy(pl_eqt.NomMachine, vl_NomMachine);
   pl_eqt.Position = 0;
   pl_eqt.Suivant = NULL;
   pl_eqt.NoEqt = vl_NoEqt;

   /* Recherche si pas de commande LCR en cours sur equipement concerne */
   if ((vl_TypeEqt == XDC_EQT_BAD) &&
       (ex_test_autorisation_eqt(&pg_DebutListeBADFLcr,
                                 &pl_eqt,
                                 XDC_EQT_BAD) == XDC_NOK))
   {
      return;
   }

   if (vl_TypeEqt == XDC_EQT_BAD)
   {
      for (pl_ListeBAD = pg_debutBAD; pl_ListeBAD != NULL;
           pl_ListeBAD = pl_ListeBAD->Suivant)
      {
         if ((pl_ListeBAD->Config.Numero == vl_NoEqt) || (vl_NoEqt == XDC_EQTALL))
         {
            ex_init_lcr(&pg_DebutListeBADFLcr,
                        pl_eqt,
                        pl_ListeBAD->Socket,
                        pl_ListeBAD->Config.AdresseRGS,
                        pl_ListeBAD->Config.Numero,
                        "BAD_FLcr_NoEqt:",
                        vg_SockTIM);
         }
         if (pl_ListeBAD->Config.Numero == vl_NoEqt)
            break;
      }
      if ((pl_ListeBAD == NULL) && (vl_NoEqt != XDC_EQTALL))
      {
         XZEX02_Retour_LCR(XDC_NOK, vl_NomMachine);
      }
   }

   /* Fin du callback */
   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT Callback ep_CMD_LCR");
}

static void supprime_blancs_fin(char *Ligne)

/*
* ARGUMENTS EN ENTREE :
*       T_STR           Ligne;
*
* ARGUMENTS EN SORTIE :
*       T_STR           Ligne;
*
* CODE RETOUR :
*   aucun.
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction
*
* FONCTION
*   parcours de la chaine
*
------------------------------------------------------*/
{
   int i = 0, j = 0, k = 0;

   /*A on supprime les espaces de fin de ligne */
   k = strlen(Ligne);
   for (i = 0, j = 0; i < k; i++)
   {
      if (Ligne[i] != ' ')
         break;
   }
   if (i != k - 1) /* Il n'y a pas que des blancs */
   {
      i = k;
      for (j = i - 1; j >= 0; j--)
      {
         if (Ligne[j] == ' ')
            Ligne[j] = '\0';
         else
            break;
      }
   }
}
