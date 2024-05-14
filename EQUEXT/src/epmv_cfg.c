/* Fichier : epmv_cfg.c
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEPMV * FICHIER epmv_cfg.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module de configuration de la tache TEPMV
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	07 Sep 1994	: Creation
* Nagiel.E	version 1.2	27 Sep 1994	: MAJ:Gestion des picto
* Mismer.D	version 1.3	01 Dec 1994	: Ajout socket PICTO
*						  Renomage du XDF_Config_Picto
* Mismer.D	version 1.4	15 Dec 1994	:
* Mismer.D	version 1.5 	15 Dec 1994	:
* Nagiel.E	version 1.6	16 Dec 1994 	:
* Mismer.D	version 1.7	23 Jan 1995	:
* Mismer.D	version 1.8	26 Jan 1995	:
* Mismer.D	version 1.9	09 Fev 1995	:
* Mismer.D	version 1.10	21 Fev 1995	:
* Mismer.D	version 1.11	08 Mar 1995	:
* Volcic.F	version 1.12	26 Avr 1995	:
* Mismer.D	version 1.13	11 Mai 1995	: Ajout fonction de reconnexion aux taches protocoles
* Fontaine.C	version 1.14	18 Mai 1995	: Reprise entete, historique
* Mismer.D	version 1.15	30 Mai 1995	: Ajout fonction de lecture fichier affichage par defaut
* Mismer.D	version 1.16	01 Dec 1995	: Correction fonction de lecture fichier affichage par defaut
* Mismer.D	version 1.17	12 Jun 1996	: Appel de ex_lire_idpassw a la place de ec_lire_idpassw (DEM/1135)
* Mismer.D      version 1.18    29 Aou 1996     : Ajout ligne direction pour PMV 4 lignes (DEM/1162)
* Mismer.D	version 1.19	13 Sep 1996	: Ajout periode variable en fonction NB eqt (RADT_W)
* Mismer.D      version 1.20    10 Mar 1998     : Modif pour PMV PICTO avec la meme RGS (DEM/1618)
* JMG   13/11/07 : ajout site de gestion DEM715  1.21
* JPL	18/03/11 : Migration architecture HP ia64 (DEM 975) : suppression warnings mineurs  1.22
* PNI   17/07/12        : nb eqt ligne passe � 21 (au lieu de 16) V1.23
* NDE	03/08/15 : Type d'affichage par d�faut 3 POINTS (DEM1135) 1.24
* JPL	13/02/18 : Migration Linux 64 bits (DEM 1274) : Init. donnees PMV/Picto; lecture affichage defaut sans debordement  1.25
* JMG	02/08/18 : ajout picto dans type pmv suppression SAGA DEM1306 1.26
* JPL	16/11/22 : Forcage du mode Normal du contraste pour tous les PMV et Pictos a l'initialisation  SAE-429
* GGY   15/06/23    :   Ajout EnvoyerTrameIp et ex_ret_LCR_IP (DEM472)
------------------------------------------------------*/

/* fichiers inclus */
#include <xzsem.h>

#include "xzat.h"
#include "xzex.h"

#include "ex_mrtf.h"

#include "epmv_cfg.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

/* declaration de fonctions internes */

/* definition de fonctions externes */
extern int vg_SockTEMIP;

extern void ep_maj_contraste_tous(int);

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*
* init ou reinit de la config pmv
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int ec_lire_config_PMV (XDY_NomMachine pa_NomMachine)

/*
* ARGUMENTS EN ENTREE :
*   pa_NomMachine	: Nom de la machine
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
*  Cette fonction effectue la lecture du fichier XDF_Config_PMV_<NomMachine>
*  et met a jour le liste des structure de donnees PMV;
*
------------------------------------------------------*/
{
   static char *version = "$Id: epmv_cfg.c,v 1.26 2019/01/07 13:44:02 pc2dpdy Exp $ : ec_lire_config_PMV";

   int vl_ValRet = XDC_OK;
   int vl_Fd;
   char pl_PathEtFic[XDC_PATH_ABS_SIZE];
   XDY_Texte pl_LigneFich;
   int vl_NbPMV = 0;
   EPMV_DONNEES_PMV vl_DonneesPMV,
       *pl_DonCrt;
   char vl_Typ_Eqt;
   short vl_NumeroLS = 0;
   int vl_NbParam = 0;
   int vl_site;
   EPMV_DONNEES_TYPE_PMV *pl_typepmv;
   char vl_adr[30];
   XZEXT_MSG_SOCKET pl_message;

   /*A Ouverture du fichier de configuration des PMV */
   /* Construction du nom du fichier XDF_Config_PMV_<NomMachine>  */
   sprintf(pl_PathEtFic, "%s/%s_%s", XZEXC_PATH_CONFIG, XDF_Config_PMV, pa_NomMachine);

   XZST_03EcritureTrace(XZSTC_WARNING, "Config PMV: fichier %s", pl_PathEtFic);
   /*A Ouverture du fichier XDF_Config_PMV_<NomMachine>  */
   if ((vl_Fd = open(pl_PathEtFic, O_RDONLY)) <= 0)
   {
      /* Ecriture trace absence fichier */
      XZST_03EcritureTrace(XZSTC_WARNING, " Manque fichier %s. \n", pl_PathEtFic);
      /* Sortie de la fonction */
      return (XDC_NOK);
   }
   /*A Marquage des structures afin de determiner les eventuelles suppression */
   for (pl_DonCrt = pg_debutPMV; pl_DonCrt != NULL; pl_DonCrt = pl_DonCrt->Suivant)
   {
      /* Effacement du numero de ST pour marquage */
      strcpy(pl_DonCrt->Config.NumeroST, " ");
   }

   /*A Lecture de chaque ligne du fichier */
   while (ex_LireLigneFich(vl_Fd, pl_LigneFich, sizeof(pl_LigneFich)) != XZEXC_EOF)
   {
      /* Lire ligne suivante si c'est une ligne de commentaire */
      if (*pl_LigneFich != XDF_COMMENTAIRE)
      {
         /*B Extraire les parametre du PMV */
         memset((void *)&vl_DonneesPMV, 0, sizeof(vl_DonneesPMV));
         vl_NbParam = sscanf(pl_LigneFich, " %c %hd %s %hd %3c %hd %d %s %d %d %s",
                             &vl_Typ_Eqt,
                             &vl_DonneesPMV.Config.Numero.NoEqt,
                             vl_DonneesPMV.Config.NumeroST,
                             &vl_NumeroLS,
                             vl_DonneesPMV.Config.AdresseRGS,
                             &vl_DonneesPMV.Config.TypePMV,
                             &vl_site,
                             vl_DonneesPMV.Config.AdresseIP,
                             &vl_DonneesPMV.Config.Port,
                             &vl_DonneesPMV.Config.TypeLCR,
                             vl_DonneesPMV.Config.IdLCR);

         if (vl_NbParam != 11)
            vl_NbParam = sscanf(pl_LigneFich, " %c %hd %s %hd %3c %hd %d %s %d %d",
                                &vl_Typ_Eqt,
                                &vl_DonneesPMV.Config.Numero.NoEqt,
                                vl_DonneesPMV.Config.NumeroST,
                                &vl_NumeroLS,
                                vl_DonneesPMV.Config.AdresseRGS,
                                &vl_DonneesPMV.Config.TypePMV,
                                &vl_site,
                                vl_DonneesPMV.Config.AdresseIP,
                                &vl_DonneesPMV.Config.Port,
                                &vl_DonneesPMV.Config.TypeLCR);

         if (vl_NbParam == 10)
         {
            vl_NbParam = 11;
            strcpy(vl_DonneesPMV.Config.IdLCR, "");
         }
         else
            strcat(vl_DonneesPMV.Config.IdLCR, " ");
         /* Si tout les parametre ont ete lu */
         if (vl_NbParam == 11)
         {
            vl_DonneesPMV.Config.Numero.Typ_Eqt = vl_Typ_Eqt;
            vl_DonneesPMV.Config.NumeroLS = vl_NumeroLS;
            vl_DonneesPMV.Config.SiteGestion = vl_site;
            /*B Inserer dans la liste des structures config PMV */
            if (ed_ins_liste_PMV(&vl_DonneesPMV, sizeof(vl_DonneesPMV.Config)) == XDC_OK)
            {
               /* Incrementation du nombre de PMV */
               vl_NbPMV++;

               /* connnexion eventuelle a temip*/
               if (strcmp(vl_DonneesPMV.Config.AdresseIP, XZECC_IP_VIDE))
               {
                  /*PMV IP*/
                  if (vg_SockTEMIP == -1)
                  {
                     sprintf(vl_adr, "%s%d", XZEXC_ADR_SOCK_TEIM,
                             XDC_EQT_PMV);
                     ex_cnx_client(XZEXC_ADR_SOCK_TEPMV,
                                   vl_adr,
                                   &vg_SockMask,
                                   &vg_SockTEMIP);
                  }
                  /*demande a TEMIP de connexion au PMV*/
                  sprintf(pl_message, "%s%s %d %c%s",
                          XZEXC_CONFIG_IP,
                          vl_DonneesPMV.Config.AdresseIP,
                          vl_DonneesPMV.Config.Port,
                          XZECC_PROT_LCR,
                          XZEXC_FIN_MSG_SOCK);
                  sem_wait(&vg_semaphore);
                  if (write(vg_SockTEMIP, pl_message, strlen(pl_message)) == XDC_NOK)
                  {
                     sem_post(&vg_semaphore);
                     XZST_03EcritureTrace(XZSTC_WARNING, " connexion impossible");
                     return (XDC_NOK);
                  }
                  sem_post(&vg_semaphore);
               }
            }
         }
      }
   }
   /*A Fermeture du fichier XDF_Presence_Equip_<NomMachine>  */
   XZSS_13FermerFichier(vl_Fd);

   /*A Suppression de la liste des donnees des elements ne faisant plus partie de la config */
   for (pl_DonCrt = pg_debutPMV; pl_DonCrt != NULL;)
   {
      /* Si le numero de ST est effacer */
      if (strlen(pl_DonCrt->Config.NumeroST) <= 1)
      {
         /* Suppression de la liste */
         ed_sup_liste_PMV(pl_DonCrt->Config.Numero.NoEqt);
         pl_DonCrt = pg_debutPMV;
      }
      else
      {
         /* Passage a l'element suivant */
         pl_DonCrt = pl_DonCrt->Suivant;
      }
   }

   /* Ecriture trace de la config pour debug */
   for (pl_DonCrt = pg_debutPMV; pl_DonCrt != NULL; pl_DonCrt = pl_DonCrt->Suivant)
   {
      pl_DonCrt->Etat_Service = 0;
      pl_DonCrt->Contraste = XDC_CONTRASTENORMAL;
      pl_DonCrt->Mode = XZEXC_MODE_NORMAL;

      strcpy(pl_DonCrt->Etat.Texte1, "?");
      strcpy(pl_DonCrt->Etat.Texte2, "?");
      strcpy(pl_DonCrt->Etat.Texte3, "?");
      strcpy(pl_DonCrt->Etat.Texte4, "");
      strcpy(pl_DonCrt->Etat.Alternat1, "");
      strcpy(pl_DonCrt->Etat.Alternat2, "");
      strcpy(pl_DonCrt->Etat.Alternat3, "");
      strcpy(pl_DonCrt->Etat.Alternat4, "");
      pl_DonCrt->Etat.Clign1 = XDC_PMV_PASCLIGNO;
      pl_DonCrt->Etat.Clign2 = XDC_PMV_PASCLIGNO;
      pl_DonCrt->Etat.Clign3 = XDC_PMV_PASCLIGNO;
      pl_DonCrt->Etat.Clign4 = XDC_PMV_PASCLIGNO;
      pl_DonCrt->Etat.Affichage1 = XDC_PMV_TEXTE;
      pl_DonCrt->Etat.Affichage2 = XDC_PMV_TEXTE;
      pl_DonCrt->Etat.Affichage3 = XDC_PMV_TEXTE;
      pl_DonCrt->Etat.Affichage4 = XDC_PMV_TEXTE;
      pl_DonCrt->Etat.Flash = XDC_PMV_PASFLASH;
      pl_DonCrt->Config.Eqt_PMV_Picto = XDC_FAUX;

      for (pl_typepmv = pg_debutTypePMV; pl_typepmv != NULL; pl_typepmv = pl_typepmv->Suivant)
      {
         if (pl_DonCrt->Config.TypePMV == pl_typepmv->TypePMV)
         {
            if (pl_typepmv->Picto == 0)
               pl_DonCrt->Config.Eqt_PMV_Picto = XDC_VRAI;
            break;
         }
      }

      strcpy(pl_DonCrt->Etat_Commande.Texte1, "?");
      strcpy(pl_DonCrt->Etat_Commande.Texte2, "?");
      strcpy(pl_DonCrt->Etat_Commande.Texte3, "?");
      strcpy(pl_DonCrt->Etat_Commande.Texte4, "");
      strcpy(pl_DonCrt->Etat_Commande.Alternat1, "");
      strcpy(pl_DonCrt->Etat_Commande.Alternat2, "");
      strcpy(pl_DonCrt->Etat_Commande.Alternat3, "");
      strcpy(pl_DonCrt->Etat_Commande.Alternat4, "");
      sprintf(pl_DonCrt->AffDefaut[0], "%d\0", XDC_PMV_3POINTS);
      pl_DonCrt->AffDefaut[1][0] = '\0';
      pl_DonCrt->AffDefaut[2][0] = '\0';
      pl_DonCrt->AffDefaut[3][0] = '\0';
      pl_DonCrt->Etat_Commande.Clign1 = XDC_PMV_PASCLIGNO;
      pl_DonCrt->Etat_Commande.Clign2 = XDC_PMV_PASCLIGNO;
      pl_DonCrt->Etat_Commande.Clign3 = XDC_PMV_PASCLIGNO;
      pl_DonCrt->Etat_Commande.Clign4 = XDC_PMV_PASCLIGNO;
      pl_DonCrt->Etat_Commande.Affichage1 = XDC_PMV_TEXTE;
      pl_DonCrt->Etat_Commande.Affichage2 = XDC_PMV_TEXTE;
      pl_DonCrt->Etat_Commande.Affichage3 = XDC_PMV_TEXTE;
      pl_DonCrt->Etat_Commande.Affichage4 = XDC_PMV_TEXTE;
      pl_DonCrt->Etat_Commande.Flash = XDC_PMV_PASFLASH;

      XZST_03EcritureTrace(XZSTC_WARNING, "Config PMV: %d %d %s %d %s %d %d %d %s %d",
                           pl_DonCrt->Config.Numero.Typ_Eqt,
                           pl_DonCrt->Config.Numero.NoEqt,
                           pl_DonCrt->Config.NumeroST,
                           pl_DonCrt->Config.NumeroLS,
                           pl_DonCrt->Config.AdresseRGS,
                           pl_DonCrt->Config.TypePMV,
                           pl_DonCrt->Etat_Service,
                           pl_DonCrt->Config.SiteGestion,
                           pl_DonCrt->Config.AdresseIP,
                           pl_DonCrt->Config.Port);
   }
   /* Retourne compte rendu OK */
   return (vl_ValRet);
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Cette fonction effectue la lecture du fichier XDF_Config_Type_PMV_<NomMachine>
*  et met a jour le liste des structure de donnees des types de PMV.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int ec_lire_config_TypePMV(XDY_NomMachine pa_NomMachine)

/*
* ARGUMENTS EN ENTREE :
*   pa_NomMachine	: Nom de la machine
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
   static char *version = "@(#)epmv_cfg.c	1.20 03/10/98 : ec_lire_config_TypePMV";

   int vl_ValRet = XDC_OK;
   int vl_Fd;
   char pl_PathEtFic[XDC_PATH_ABS_SIZE];
   XDY_Texte pl_LigneFich;
   int vl_NbTypePMV = 0;
   EPMV_DONNEES_TYPE_PMV vl_DonneesTypePMV = {0},
                         *pl_DonCrt;
   int vl_NbParam = 0;
   short vl_Flash = 0, vl_Picto = 0,
         vl_Temp = 0;

   /*A Ouverture du fichier de configuration des Type PMV */
   /* Construction du nom du fichier XDF_Config_Type_PMV_<NomMachine>  */
   sprintf(pl_PathEtFic, "%s/%s_%s", XZEXC_PATH_CONFIG, XDF_Config_Type_PMV, pa_NomMachine);

   /* Ouverture du fichier XDF_Config_Type_PMV_<NomMachine>  */
   if ((vl_Fd = open(pl_PathEtFic, O_RDONLY)) <= 0)
   {
      /* Ecriture trace absence fichier */
      XZST_03EcritureTrace(XZSTC_WARNING, " Manque fichier %s. \n", pl_PathEtFic);
      /* Sortie de la fonction */
      return (XDC_NOK);
   }
   /*A Marquage des structures afin de determiner les eventuelles suppression */
   for (pl_DonCrt = pg_debutTypePMV; pl_DonCrt != NULL; pl_DonCrt = pl_DonCrt->Suivant)
   {
      /* RAZ du nombre de ligne pour marquage */
      pl_DonCrt->NbLigne = 0;
   }

   /*A Lecture de chaque ligne du fichier */
   while (ex_LireLigneFich(vl_Fd, pl_LigneFich, sizeof(pl_LigneFich)) != XZEXC_EOF)
   {
      /* Lire ligne suivante si c'est une ligne de commentaire */
      if (*pl_LigneFich != XDF_COMMENTAIRE)
      {
         /*B Extraire les parametres du TypePMV */
         vl_NbParam = sscanf(pl_LigneFich, "%hd %d %d %hd %hd %hd",
                             &vl_DonneesTypePMV.TypePMV,
                             &vl_DonneesTypePMV.NbLigne,
                             &vl_DonneesTypePMV.NbCaracteres,
                             &vl_Flash,
                             &vl_Temp,
                             &vl_Picto);
         vl_DonneesTypePMV.Flash = vl_Flash;
         vl_DonneesTypePMV.Temperature = vl_Temp;
         vl_DonneesTypePMV.Picto = vl_Picto;

         /* Si tout les parametre ont ete lu */
         if (vl_NbParam == 6)
         {
            /*B Inserer dans la liste des structures config TypePMV */
            if (ed_ins_liste_TypePMV(&vl_DonneesTypePMV, 0) == XDC_OK)
            {
               /* Incrementation du nombre de TypePMV */
               vl_NbTypePMV++;
            }
         }
      }
   }
   /*A Fermeture du fichier XDF_Presence_Equip_<NomMachine>  */
   XZSS_13FermerFichier(vl_Fd);

   /*A Suppression de la liste des donnees des elements ne faisant plus partie de la config */
   for (pl_DonCrt = pg_debutTypePMV; pl_DonCrt != NULL;)
   {
      /* Si le numero de ST est effacer */
      if (pl_DonCrt->NbLigne == 0)
      {
         /* Suppression de la liste */
         ed_sup_liste_TypePMV(pl_DonCrt->TypePMV);
         pl_DonCrt = pg_debutTypePMV;
      }
      else
      {
         /* Passage a l'element suivant */
         pl_DonCrt = pl_DonCrt->Suivant;
      }
   }

   /* Ecriture trace de la config pour debug */
   for (pl_DonCrt = pg_debutTypePMV; pl_DonCrt != NULL; pl_DonCrt = pl_DonCrt->Suivant)
      XZST_03EcritureTrace(XZSTC_DEBUG3, "Config Type PMV: %hd\t%d\t%d\t%hd\t%hd\t%hd",
                           pl_DonCrt->TypePMV,
                           pl_DonCrt->NbLigne,
                           pl_DonCrt->NbCaracteres,
                           pl_DonCrt->Flash,
                           pl_DonCrt->Temperature,
                           pl_DonCrt->Picto);
   /* Retourne compte rendu OK */
   return (vl_ValRet);
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Cette fonction effectue la lecture du fichier XDF_Affichage_Def_PMV
*  (affichage par defaut PMV pour mise au neutre )
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void ec_lire_Affichage_Def_PMV(void)

/*
* ARGUMENTS EN ENTREE :
*   aucun
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR :
*   aucun
*
* CONDITION D'UTILISATION
*
*
* FONCTION
*
*
------------------------------------------------------*/
{
   static char *version = "$Id: epmv_cfg.c,v 1.26 2019/01/07 13:44:02 pc2dpdy Exp $ : ec_lire_Affichage_Def_PMV";

   int vl_Fd;
   char pl_PathEtFic[XDC_PATH_ABS_SIZE];
   XDY_Eqt vl_NumPMV = 0;
   char *vl_Mot = NULL;
   XDY_Texte pl_LigneFich;
   XDY_Texte pl_LigneFichMem;
   EPMV_DONNEES_PMV *pl_DonCrt;
   XDY_Nom vl_NomPMVFic = "";
   XDY_Nom vl_NomPMV = "";
   XDY_NomMachine vl_NomMachine = "";
   int vl_index = 0;

   /*A Ouverture du fichier des affichage par defaut des PMV */
   /* Construction du nom du fichier XDF_Affichage_Def_PMV  */
   sprintf(pl_PathEtFic, "%s/%s", XZEXC_PATH_CONFIG, XDF_Affichage_Def_PMV);

   /* Ouverture du fichier XDF_Affichage_Def_PMV  */
   if ((vl_Fd = open(pl_PathEtFic, O_RDONLY)) <= 0)
   {
      /* Ecriture trace absence fichier */
      XZST_03EcritureTrace(XZSTC_WARNING, " Manque fichier %s. \n", pl_PathEtFic);
      /* Sortie de la fonction */
      return;
   }

   /*A Lecture de chaque ligne du fichier */
   while (ex_LireLigneFich(vl_Fd, pl_LigneFich, sizeof(pl_LigneFich)) != XZEXC_EOF)
   {
      /* Lire ligne suivante si c'est une ligne de commentaire */
      if (*pl_LigneFich != XDF_COMMENTAIRE)
      {
         /*B Extraire les parametres  */
         strcpy(pl_LigneFichMem, pl_LigneFich);
         strcpy(vl_NomPMVFic, ((vl_Mot = strtok(pl_LigneFich, ",")) != NULL) ? vl_Mot : "");
         if (strlen(vl_NomPMVFic) != 0)
         {
            for (pl_DonCrt = pg_debutPMV; pl_DonCrt != NULL; pl_DonCrt = pl_DonCrt->Suivant)
            {
               ex_nom_equipement(XDC_EQT_PMV,
                                 pl_DonCrt->Config.Numero.NoEqt,
                                 NULL,
                                 vl_NomPMV,
                                 vl_NomMachine);
               if (strstr(vl_NomPMV, vl_NomPMVFic) != NULL)
                  break;
            }
            strcpy(pl_LigneFich, pl_LigneFichMem);
            strcpy(vl_NomPMVFic, ((vl_Mot = strtok(pl_LigneFich, ",")) != NULL) ? vl_Mot : "");
            vl_index = 0;
            while ((pl_DonCrt != NULL) && (vl_Mot = strtok(NULL, ",")) != NULL)
            {
               if (strlen(vl_Mot) < XDC_Lon_Ligne_PMV)
               {
                  strcpy(pl_DonCrt->AffDefaut[vl_index], vl_Mot);
               }
               else
               {
                  strncpy(pl_DonCrt->AffDefaut[vl_index], vl_Mot, XDC_Lon_Ligne_PMV - 1);
                  pl_DonCrt->AffDefaut[vl_index][XDC_Lon_Ligne_PMV - 1] = '\0';
               }
               if ((atoi(pl_DonCrt->AffDefaut[vl_index]) != 0) ||
                   (++vl_index >= sizeof(pl_DonCrt->AffDefaut) / sizeof(pl_DonCrt->AffDefaut[0])))
                  break;
            }
         }
      }
   }

   /*A Fermeture du fichier XDF_Presence_Equip_<NomMachine>  */
   XZSS_13FermerFichier(vl_Fd);

   /* Ecriture trace de la config pour debug */
   for (pl_DonCrt = pg_debutPMV; pl_DonCrt != NULL; pl_DonCrt = pl_DonCrt->Suivant)
      XZST_03EcritureTrace(XZSTC_WARNING, "Affichage par defaut PMV : %hd\t--%s--%s--%s--%s--",
                           pl_DonCrt->Config.Numero.NoEqt,
                           pl_DonCrt->AffDefaut[0],
                           pl_DonCrt->AffDefaut[1],
                           pl_DonCrt->AffDefaut[2],
                           pl_DonCrt->AffDefaut[3]);
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Cette fonction effectue la lecture du fichier XDF_Config_PICTO_<NomMachine>
*  et met a jour le liste des structure de donnees Picto;
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int ec_lire_config_Picto(XDY_NomMachine pa_NomMachine)

/*
* ARGUMENTS EN ENTREE :
*   pa_NomMachine	: Nom de la machine
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
   static char *version = "$Id: epmv_cfg.c,v 1.26 2019/01/07 13:44:02 pc2dpdy Exp $ : ec_lire_config_Picto";

   int vl_ValRet = XDC_OK;
   int vl_Fd;
   char pl_PathEtFic[XDC_PATH_ABS_SIZE];
   XDY_Texte pl_LigneFich;
   int vl_NbPICTO = 0;
   EPMV_DONNEES_Picto vl_DonneesPICTO,
       *pl_DonCrt;
   char vl_Typ_Eqt;
   short vl_NumeroLS = 0;
   int vl_NbParam = 0;

   short vl_NoEqt = 0,
         vl_NoLs = 0,
         vl_TypePicto = 0;
   XDY_NomST vl_NomST;
   int vl_site;
   int vl_ligne = 0 ;

   /*A Ouverture du fichier de configuration des PICTO */
   /* Construction du nom du fichier XDF_Config_PICTO_<NomMachine>  */
   sprintf(pl_PathEtFic, "%s/%s_%s", XZEXC_PATH_CONFIG, XDF_Config_PICTO, pa_NomMachine);
   XZST_03EcritureTrace(XZSTC_DEBUG1, "Fichier lu : [%s]",pl_PathEtFic);

   /* Ouverture du fichier XDF_Config_PICTO_<NomMachine>  */
   if ((vl_Fd = open(pl_PathEtFic, O_RDONLY)) <= 0)
   {
    	if (pg_debutPicto == NULL)
		{
			XZST_03EcritureTrace(XZSTC_WARNING, " Manque fichier %s car aucun PMV n'a un Picto associe. \n", pl_PathEtFic);
		}
		else
		{
			/* Ecriture trace absence fichier */
			XZST_03EcritureTrace(XZSTC_WARNING, " Manque fichier %s. \n", pl_PathEtFic);
      		/* Sortie de la fonction */
			return (XDC_NOK);
		}
   }
   /*A Marquage des structures afin de determiner les eventuelles suppression */
   for (pl_DonCrt = pg_debutPicto; pl_DonCrt != NULL; pl_DonCrt = pl_DonCrt->Suivant)
   {
      /* Effacement du numero de ST pour marquage */
      strcpy(pl_DonCrt->Config.NumeroST, " ");
   }

   /*A Lecture de chaque ligne du fichier */
   while (ex_LireLigneFich(vl_Fd, pl_LigneFich, sizeof(pl_LigneFich)) != XZEXC_EOF)
   {
      XZST_03EcritureTrace(XZSTC_DEBUG1, "Ligne lue : [%s]",pl_LigneFich);
      vl_ligne++;
      /* Lire ligne suivante si c'est une ligne de commentaire */
      if (*pl_LigneFich != XDF_COMMENTAIRE)
      {
         /*B Extraire les parametre du PICTO */
         memset((void *)&vl_DonneesPICTO, 0, sizeof(vl_DonneesPICTO));
         vl_NbParam = sscanf(pl_LigneFich, " %c %hd %s %hd %3c %hd %d %d %s %d %s",
                              &vl_Typ_Eqt,
                              &vl_DonneesPICTO.Config.Numero.NoEqt,
                              vl_DonneesPICTO.Config.NumeroST,
                              &vl_NumeroLS,
                              vl_DonneesPICTO.Config.AdresseRGS,
                              &vl_DonneesPICTO.Config.TypePicto,
                              &vl_DonneesPICTO.Config.SiteGestion,
                              &vl_DonneesPICTO.Config.Port,
                              vl_DonneesPICTO.Config.AdresseIP,
                              &vl_DonneesPICTO.Config.TypeLCR,
                              vl_DonneesPICTO.Config.IdLCR);
         XZST_03EcritureTrace(XZSTC_DEBUG1, "Nombre de paramètres lus : %d",vl_NbParam);
         if (vl_NbParam != 11)
         {
            vl_NbParam = sscanf(pl_LigneFich, " %c %hd %s %hd %3c %hd %d %d %s %d",
                                &vl_Typ_Eqt,
                                &vl_DonneesPICTO.Config.Numero.NoEqt,
                                vl_DonneesPICTO.Config.NumeroST,
                                &vl_NumeroLS,
                                vl_DonneesPICTO.Config.AdresseRGS,
                                &vl_DonneesPICTO.Config.TypePicto,
                                &vl_DonneesPICTO.Config.SiteGestion,
                                &vl_DonneesPICTO.Config.Port,
                                vl_DonneesPICTO.Config.AdresseIP,
                                &vl_DonneesPICTO.Config.TypeLCR);
            XZST_03EcritureTrace(XZSTC_WARNING, "Nombre de paramètres lus : %d",vl_NbParam);
            if (vl_NbParam == 10)
            {
               vl_NbParam = 11;
               strcpy(vl_DonneesPICTO.Config.IdLCR, "");
            }
            else
               XZST_03EcritureTrace(XZSTC_WARNING, "Nombre de paramètres incorrects sur la ligne %d dans [%s]",vl_ligne, pl_LigneFich);
         }
         strcat(vl_DonneesPICTO.Config.IdLCR, " ");

         /* Si tout les parametre ont ete lu */
         if (vl_NbParam == 11)
         {
            vl_DonneesPICTO.Config.Numero.Typ_Eqt = vl_Typ_Eqt;
            vl_DonneesPICTO.Config.NumeroLS = vl_NumeroLS;
            vl_DonneesPICTO.Config.SiteGestion = vl_site;
            /*B Inserer dans la liste des structures config PICTO */
            if (ed_ins_liste_Picto(&vl_DonneesPICTO, sizeof(vl_DonneesPICTO.Config)) == XDC_OK)
            {
               /* Incrementation du nombre de PICTO */
               vl_NbPICTO++;
            }
         }
      }
   }
   /*A Fermeture du fichier XDF_Presence_Equip_<NomMachine>  */
   XZSS_13FermerFichier(vl_Fd);

   /*A Suppression de la liste des donnees des elements ne faisant plus partie de la config */
   for (pl_DonCrt = pg_debutPicto; pl_DonCrt != NULL;)
   {
      /* Si le numero de ST est effacer */
      if (strlen(pl_DonCrt->Config.NumeroST) <= 1)
      {
         /* Suppression de la liste */
         ed_sup_liste_Picto(pl_DonCrt->Config.Numero.NoEqt);
         pl_DonCrt = pg_debutPicto;
      }
      else
      {
         /* Passage a l'element suivant */
         pl_DonCrt = pl_DonCrt->Suivant;
      }
   }

   /* Ecriture trace de la config pour debug et init de l'etat de service et de l'activite */
   for (pl_DonCrt = pg_debutPicto; pl_DonCrt != NULL; pl_DonCrt = pl_DonCrt->Suivant)
   {
      pl_DonCrt->Etat_Service = 0;
      pl_DonCrt->Contraste = XDC_CONTRASTENORMAL;
      pl_DonCrt->Mode = XZEXC_MODE_NORMAL;
      pl_DonCrt->Config.Eqt_PMV_Picto = XDC_FAUX;

      strcpy(pl_DonCrt->Etat.Texte, "");
      strcpy(pl_DonCrt->Etat.Picto, XDC_PICTO_PASPICTO);
      pl_DonCrt->Etat.Clign = XDC_PICTO_PASCLIGNO;

      strcpy(pl_DonCrt->Etat_Commande.Texte, "");
      strcpy(pl_DonCrt->Etat_Commande.Picto, XDC_PICTO_PASPICTO);
      pl_DonCrt->Etat_Commande.Clign = XDC_PICTO_PASCLIGNO;

      XZST_03EcritureTrace(XZSTC_WARNING, "Config PICTO: %c %d %s %d %s %d %d %s %d",
                           pl_DonCrt->Config.Numero.Typ_Eqt,
                           pl_DonCrt->Config.Numero.NoEqt,
                           pl_DonCrt->Config.NumeroST,
                           pl_DonCrt->Config.NumeroLS,
                           pl_DonCrt->Config.AdresseRGS,
                           pl_DonCrt->Config.TypePicto,
                           pl_DonCrt->Etat_Service,
                           pl_DonCrt->Config.AdresseIP,
                           pl_DonCrt->Config.Port);
   }
   /* Retourne compte rendu OK */
   return (vl_ValRet);
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Cette fonction effectue la lecture du fichier XDF_Config_Type_PICTO_<NomMachine>
*  et met a jour le liste des structure de donnees des types de Picto.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int ec_lire_config_TypePicto(XDY_NomMachine pa_NomMachine)

/*
* ARGUMENTS EN ENTREE :
*   pa_NomMachine	: Nom de la machine
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
   static char *version = "$Id: epmv_cfg.c,v 1.26 2019/01/07 13:44:02 pc2dpdy Exp $ : ec_lire_config_TypePicto";

   int vl_ValRet = XDC_OK;
   int vl_Fd;
   char pl_PathEtFic[XDC_PATH_ABS_SIZE];
   char vl_ficpictoauto[XDC_PATH_ABS_SIZE];
   XDY_Texte pl_LigneFich;
   XDY_Texte pl_LigneFich2;
   int vl_NbTypePicto = 0;
   EPMV_DONNEES_TYPE_Picto vl_DonneesTypePicto = {0},
                           *pl_DonCrt;
   int vl_NbParam = 0,
       vl_premierefois,
       vl_resultatfic;
   short vl_Flash = 0,
         vl_Temp = 0;
   EPMV_LISTE_Picto *pl_listepicto,
       *pl_debut_liste_picto_auto,
       *pl_suivant,
       *pl_prec;
   XDY_Picto vl_pictoauto;

   /*A Ouverture du fichier de configuration des Type Picto */
   /* Construction du nom du fichier XDF_Config_Type_PICTO_<NomMachine>  */
   sprintf(pl_PathEtFic, "%s/%s_%s", XZEXC_PATH_CONFIG, XDF_Config_Type_PICTO, pa_NomMachine);

   /* Ouverture du fichier XDF_Config_Type_PICTO_<NomMachine>  */
   if ((vl_Fd = open(pl_PathEtFic, O_RDONLY)) <= 0)
   {
      /* Ecriture trace absence fichier */
      XZST_03EcritureTrace(XZSTC_WARNING, " Manque fichier %s. \n", pl_PathEtFic);
      /* Sortie de la fonction */
      return (XDC_NOK);
   }
   /*A Marquage des structures afin de determiner les eventuelles suppression */
   for (pl_DonCrt = pg_debutTypePicto; pl_DonCrt != NULL; pl_DonCrt = pl_DonCrt->Suivant)
   {
      /* RAZ du nombre de ligne pour marquage */
      pl_DonCrt->NbCaracteres = 0;

      /* RAZ de la liste des picto autorises */

      for (pl_listepicto = pl_DonCrt->Liste; pl_listepicto != NULL;)
      {
         pl_suivant = pl_listepicto->Suivant;
         free(pl_listepicto);
         pl_listepicto = pl_suivant;
      }
      pl_DonCrt->Liste = NULL;
   }

   /*A Lecture de chaque ligne du fichier */
   while (ex_LireLigneFich(vl_Fd, pl_LigneFich, sizeof(pl_LigneFich)) != XZEXC_EOF)
   {
      /* Lire ligne suivante si c'est une ligne de commentaire */
      if (*pl_LigneFich != XDF_COMMENTAIRE)
      {

         /*B Extraire les parametres du TypePicto */
         vl_NbParam = sscanf(pl_LigneFich, "%hd %d %s",
                             &vl_DonneesTypePicto.TypePicto,
                             &vl_DonneesTypePicto.NbCaracteres,
                             vl_ficpictoauto);

         /* Si tout les parametre ont ete lu */
         if (vl_NbParam == 3)
         {
            /* Construction de la liste des Picto autorises */

            /*A Ouverture du fichier de configuration des Type Picto */
            /* Construction du nom du fichier XDF_Config_Type_PICTO_<NomMachine>  */

            /*B Inserer dans la liste des structures config TypePicto */
            if (ed_ins_liste_TypePicto(&vl_DonneesTypePicto, 0) == XDC_OK)
            {
               /* Incrementation du nombre de TypePicto */
               vl_NbTypePicto++;
            }
         }
      }
   }

   /*A Fermeture du fichier XDF__<NomMachine>  */
   XZSS_13FermerFichier(vl_Fd);

   /*A Suppression de la liste des donnees des elements ne faisant plus partie de la config */
   for (pl_DonCrt = pg_debutTypePicto; pl_DonCrt != NULL;)
   {
      /* Si le numero de ST est effacer */
      if (pl_DonCrt->NbCaracteres == 0)
      {
         /* Suppression de la liste */
         ed_sup_liste_TypePicto(pl_DonCrt->TypePicto);
         pl_DonCrt = pg_debutTypePicto;
      }
      else
      {
         /* Passage a l'element suivant */
         pl_DonCrt = pl_DonCrt->Suivant;
      }
   }

   /* Ecriture trace de la config pour debug */
   for (pl_DonCrt = pg_debutTypePicto; pl_DonCrt != NULL; pl_DonCrt = pl_DonCrt->Suivant)
   {
      XZST_03EcritureTrace(XZSTC_DEBUG1, "Config Type Picto: %d %d",
                           pl_DonCrt->TypePicto,
                           pl_DonCrt->NbCaracteres);
      for (pl_listepicto = pl_DonCrt->Liste; pl_listepicto != NULL; pl_listepicto = pl_listepicto->Suivant)
      {
         XZST_03EcritureTrace(XZSTC_DEBUG1, "Config Type Picto: %s",
                              pl_listepicto->Nom_Picto);
      }
   }
   /* Retourne compte rendu OK */
   return (vl_ValRet);
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Permet selon message socket recu de tesrv
*    - soit de terminer la tache
*    - soit de configurer la tache
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void ec_controle(int va_NumSock,
                 XDY_NomMachine pa_NomMachine)

/*
* ARGUMENTS EN ENTREE :
*   va_NumSock		: Numero de la socket de TESRV
*   pa_NomMachine	: Nom de la machine
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR :
*   aucun
*
* CONDITION D'UTILISATION
*
* FONCTION
*   Lecture du message socket
*   Si message d'init alors lancer la fonction ec_init
*   Si message d'arret alors lancer la fonction ec_sortir
*
------------------------------------------------------*/
{
   static char *version = "$Id: epmv_cfg.c,v 1.26 2019/01/07 13:44:02 pc2dpdy Exp $ : ec_controle";

   int vl_IndCnx = 0,
       vl_LgMsg = 0;
   XZEXT_MSG_SOCKET pl_Msg;
   XDY_Mot vl_MaxEqtLigne = 0;

   /*A
    *  Lecture du message recu dans la socket appelante
    */

   if (ex_lire_msg(va_NumSock, pl_Msg, &vl_LgMsg, &vg_SockMask, &vl_IndCnx) != XDC_OK)
   {
      /* Ecriture trace Erreur lecture message socket. */
      XZST_03EcritureTrace(XZSTC_WARNING, " Erreur lecture message socket. ");
   }
   else
   {
      /*A Si il s'agit d'une deconnexion */
      if (vl_IndCnx == XZEXC_DECONNEC)
      {
         /*B Arreter la tache par appel de ec_sortir */
         ec_sortir();
      }

      /* Si le message est correct  */
      if (vl_LgMsg != 0)
      {
         /*A Si il s'agit d'une demande d'arret */
         if (strstr(pl_Msg, XZEXC_DEMANDE_ARRET) != NULL)
         {
            /*A Arreter la tache par appel de ec_sortir */
            ec_sortir();
         }

         /*A Si il s'agit d'une demande d'Init */
         if (strstr(pl_Msg, XZEXC_DEMANDE_INIT) != NULL)
         {
            /*A Reconfiguration de la tache par appel de ec_init */
            ec_init(pa_NomMachine, &vl_MaxEqtLigne);
         }
         /*A Si il s'agit du changement du mode de fonctionnement */
         if (strstr(pl_Msg, XZEXC_MODE_FONCT_NORMAL) != NULL)
         {
            /*A Maj de la variable en question */
            vg_Mode_Fonct = XZEXC_MODE_NORMAL;
            ex_RecopieFichierMsg(pa_NomMachine);
         }
         if (strstr(pl_Msg, XZEXC_MODE_FONCT_ISOLE) != NULL)
         {
            /*A Maj de la variable en question */
            vg_Mode_Fonct = XZEXC_MODE_ISOLE;
         }
      }
   }
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Effectue l'initialisation de la tache
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int ec_init(XDY_NomMachine pa_NomMachine, XDY_Mot *va_MaxEqtLigne)

/*
* ARGUMENTS EN ENTREE :
*
*   pa_NomMachine	: Nom de la machine
*
* ARGUMENTS EN SORTIE :
*   va_MaxEqtLigne	: Nombre maxi d'equipement par ligne serie
*
* CODE RETOUR :
*   aucun
*
* CONDITION D'UTILISATION
*
* FONCTION
*   Appel de ex_mini
*   Lecture des fichier de configuration des PMV et des Picto
*   Remplissage des structures de donnees
*   Connexion socket avec la tache protocole
*
------------------------------------------------------*/
{
   static char *version = "$Id: epmv_cfg.c,v 1.26 2019/01/07 13:44:02 pc2dpdy Exp $ : ec_init";
   EPMV_DONNEES_PMV *pl_DonCrtPMV;
   EPMV_DONNEES_Picto *pl_DonCrtPICTO;
   int vl_Index = 0;
   short vl_NbEqtLigne[21] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

   /*A
    * Lecture des fichiers de configuration et remplissage des structure de donnees
    */
   /*A Configuration des  Types de PMV (appel de la fonction ec_lire_config_TypePMV) */
   if (ec_lire_config_TypePMV(pa_NomMachine) == XDC_NOK)
      return (XDC_NOK);
   /*A Configuration des PMV (appel de la fonction ec_lire_config_PMV) */
   if (ec_lire_config_PMV(pa_NomMachine) == XDC_NOK)
      return (XDC_NOK);
   /*A Configuration des Picto (appel de la fonction ec_lire_config_Picto)*/
   if (ec_lire_config_Picto(pa_NomMachine) == XDC_NOK)
      return (XDC_NOK);
   /*A Configuration des Type de Picto (appel de la fonction ec_lire_config_TypePicto) */
   if (ec_lire_config_TypePicto(pa_NomMachine) == XDC_NOK)
      return (XDC_NOK);
   /*A Configuration des clignotements */
   ex_lire_cligno(pa_NomMachine, vg_clignolent, vg_clignorapide);
   /*A Lecture user/password */
   ex_lire_idpassw(pa_NomMachine, vg_idpassw);

   /*A Lecture fichier affichage par par defaut */
   ec_lire_Affichage_Def_PMV();

   /*A Ouverture du fichier d'alarme */
   ex_OuvertureFichierMsg(vg_NomMachine);

   /*A Appel  xzat26_Init_Etats_Eqts pour Initialisation des etats equipement (HS ou Actif) */
   xzat26_Init_Etats_Eqts(XDC_EQT_PMV, XDC_ETAT_TOUS_EQT, vg_NomMachine);
   xzat26_Init_Etats_Eqts(XDC_EQT_PCT, XDC_ETAT_TOUS_EQT, vg_NomMachine);

   /*A
    * Connexion socket en mode client par appel de ec_init_cnx
    */
   ec_init_cnx();

   /*A
    * Calcul du nombre maxi de PMV par ligne
    */
   for (pl_DonCrtPMV = pg_debutPMV; pl_DonCrtPMV != NULL; pl_DonCrtPMV = pl_DonCrtPMV->Suivant)
      vl_NbEqtLigne[pl_DonCrtPMV->Config.NumeroLS]++;
   for (vl_Index = 0; vl_Index < 21; vl_Index++)
      if (*va_MaxEqtLigne < vl_NbEqtLigne[vl_Index])
         *va_MaxEqtLigne = vl_NbEqtLigne[vl_Index];

   /*A
    * Recherche des PMV et Picto ayant la meme adresse RGS
    */
   for (pl_DonCrtPMV = pg_debutPMV; pl_DonCrtPMV != NULL; pl_DonCrtPMV = pl_DonCrtPMV->Suivant)
   {
      for (pl_DonCrtPICTO = pg_debutPicto; pl_DonCrtPICTO != NULL; pl_DonCrtPICTO = pl_DonCrtPICTO->Suivant)
      {
         if (!strcmp(pl_DonCrtPMV->Config.AdresseRGS, pl_DonCrtPICTO->Config.AdresseRGS))
         {
            pl_DonCrtPMV->Config.Eqt_PMV_Picto = XDC_VRAI;
            pl_DonCrtPICTO->Config.Eqt_PMV_Picto = XDC_VRAI;
            break;
         }
      }
   }

   /*A
    * Forcage du mode normal du contraste pour tous les PMV et Pictos
    */
   ep_maj_contraste_tous(XDC_CONTRASTENORMAL);

   return (XDC_OK);
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Effectue la connection avec avec la tache protocole
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void ec_init_cnx()

/*
* ARGUMENTS EN ENTREE :
*
*  aucun
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR :
*   aucun
*
* CONDITION D'UTILISATION
*
* FONCTION
*   Connexion socket avec la tache protocole
*
------------------------------------------------------*/
{
   static char *version = "$Id: epmv_cfg.c,v 1.26 2019/01/07 13:44:02 pc2dpdy Exp $ : ec_init_cnx";

   EPMV_DONNEES_PMV *pl_DonCrtPMV;
   EPMV_DONNEES_Picto *pl_DonCrtPICTO;
   XZEXT_MASK *pl_Socket;
   XZEXT_ADR_SOCK pl_TacheTrt;
   XZEXT_ADR_SOCK pl_TacheProt;
   int vl_SockProt = 0;
   char vl_adr[30];
   XZEXT_MSG_SOCKET pl_message;

   /*A
    * Connexion socket en mode client pour communiquer avec la tache TELMi
    */
   XZST_03EcritureTrace(XZSTC_WARNING, "ec_init_cnx");
   /*A Pour tout les PMV rechercher si la tache protocole associee existe */
   for (pl_DonCrtPMV = pg_debutPMV; pl_DonCrtPMV != NULL; pl_DonCrtPMV = pl_DonCrtPMV->Suivant)
   {
      /* Construction du nom de la tache protocole telmi*/
      sprintf(pl_TacheProt, "%s%d", XZEXC_ADR_SOCK_TELM, pl_DonCrtPMV->Config.NumeroLS);
      sprintf(pl_TacheTrt, "%s%d", XZEXC_ADR_SOCK_PMVTELM, pl_DonCrtPMV->Config.NumeroLS);
      XZST_03EcritureTrace(XZSTC_WARNING, "%s", pl_TacheProt);
      /*A Recherche sur tout le masque des socket si la socket existe */
      for (pl_Socket = vg_SockMask; pl_Socket != NULL; pl_Socket = pl_Socket->Suivant)
      {
         XZST_03EcritureTrace(XZSTC_WARNING, "PMV %s", pl_DonCrtPMV->Config.AdresseRGS);
         /*PMV non IP*/
         if ((!strcmp(pl_DonCrtPMV->Config.AdresseIP, XZECC_IP_VIDE)))
         {
            if (!strcmp(pl_Socket->adresse, pl_TacheProt))
            {
               /* Mise a jour du numero de socket */
               pl_DonCrtPMV->Socket = pl_Socket->desc_sock;
               EnvoyerTrame("PS",
                            XZEXC_TRAME_ETAT_AFF_PMV,
                            pl_DonCrtPMV->Config.AdresseRGS,
                            XZEXC_CMD_INIT,
                            XZEXC_PRIORITEFAIBLE,
                            pl_DonCrtPMV->Socket);
               pl_DonCrtPMV->EtatTrans = EPMVC_EQT_OCCUP;
               /*B Arret: la socket avec la tache existe */
               break;
            }
         }
         else /* PMV IP */
         {
            pl_DonCrtPMV->Socket = vg_SockTEMIP;
            EnvoyerTrameIP("PS",
                           XZEXC_TRAME_ETAT_AFF_PMV,
                           pl_DonCrtPMV->Config.AdresseIP,
                           pl_DonCrtPMV->Config.Port,
                           pl_DonCrtPMV->Config.AdresseRGS,
                           XZEXC_CMD_INIT,
                           XZEXC_PRIORITEFAIBLE,
                           pl_DonCrtPMV->Socket);
            pl_DonCrtPMV->EtatTrans = EPMVC_EQT_OCCUP;
            break;
         }
      }

      /*A Si la socket avec la tache telmi n'existe pas */
      if (pl_Socket == NULL)
      {
         if (!strcmp(pl_DonCrtPMV->Config.AdresseIP, XZECC_IP_VIDE))
         { /*PMV NON IP*/
            /*A Appel de ex_cnx_client pour etablir la liaison avec la tache protocole */
            if (ex_cnx_client(pl_TacheTrt,
                              pl_TacheProt,
                              &vg_SockMask,
                              &vl_SockProt) != XDC_OK)
            {
               /*B Ecriture Trace */
               XZST_03EcritureTrace(XZSTC_WARNING, "Impossible de creer la connexion client  .");
               pl_DonCrtPMV->EtatTrans = EPMVC_EQT_PANNE;
            }
            else
            {
               /* Memorisation du numero de socket */
               pl_DonCrtPMV->Socket = vl_SockProt;
               EnvoyerTrame("PS",
                            XZEXC_TRAME_ETAT_AFF_PMV,
                            pl_DonCrtPMV->Config.AdresseRGS,
                            XZEXC_CMD_INIT,
                            XZEXC_PRIORITEFAIBLE,
                            pl_DonCrtPMV->Socket);
               pl_DonCrtPMV->EtatTrans = EPMVC_EQT_OCCUP;
            }
         }
         else
         { /* PMV IP*/
            sprintf(vl_adr, "%s%d", XZEXC_ADR_SOCK_TEIM, XDC_EQT_PMV);
            if (ex_cnx_client(XZEXC_ADR_SOCK_TEPMV,
                              vl_adr,
                              &vg_SockMask,
                              &vg_SockTEMIP) != XDC_OK)
            {
               XZST_03EcritureTrace(XZSTC_WARNING,
                                    "impossible de creer la connexion TEMIP");
               pl_DonCrtPMV->EtatTrans = EPMVC_EQT_PANNE;
            }
            else
            {
               pl_DonCrtPMV->Socket = vg_SockTEMIP;
               sprintf(pl_message, "%s%s %d %c%s",
                       XZEXC_CONFIG_IP,
                       pl_DonCrtPMV->Config.AdresseIP,
                       pl_DonCrtPMV->Config.Port,
                       XZECC_PROT_LCR,
                       XZEXC_FIN_MSG_SOCK);
               sem_wait(&vg_semaphore);
               write(vg_SockTEMIP, pl_message,
                     strlen(pl_message));
               sem_post(&vg_semaphore);

               EnvoyerTrameIP("PS",
                              XZEXC_TRAME_ETAT_AFF_PMV,
                              pl_DonCrtPMV->Config.AdresseIP,
                              pl_DonCrtPMV->Config.Port,
                              pl_DonCrtPMV->Config.AdresseRGS,
                              XZEXC_CMD_INIT,
                              XZEXC_PRIORITEFAIBLE,
                              pl_DonCrtPMV->Socket);
            }
         }
      }
   }

   /*A Pour tout les PICTO rechercher si la tache protocole associee existe */
   for (pl_DonCrtPICTO = pg_debutPicto; pl_DonCrtPICTO != NULL; pl_DonCrtPICTO = pl_DonCrtPICTO->Suivant)
   {
      /* Construction du nom de la tache protocole telmi*/
      sprintf(pl_TacheProt, "%s%d", XZEXC_ADR_SOCK_TELM, pl_DonCrtPICTO->Config.NumeroLS);
      sprintf(pl_TacheTrt, "%s%d", XZEXC_ADR_SOCK_PMVTELM, pl_DonCrtPICTO->Config.NumeroLS);
      XZST_03EcritureTrace(XZSTC_DEBUG1, "PICTO %s %s %d", pl_DonCrtPICTO->Config.AdresseRGS,
                           pl_DonCrtPICTO->Config.AdresseIP, pl_DonCrtPICTO->Config.Port);
      /*A Recherche sur tout le masque des socket si la socket exite */
      for (pl_Socket = vg_SockMask; pl_Socket != NULL; pl_Socket = pl_Socket->Suivant)
      {
         /*picto non IP*/
         if ((!strcmp(pl_DonCrtPICTO->Config.AdresseIP, XZECC_IP_VIDE)) &&
             (!strcmp(pl_Socket->adresse, pl_TacheProt)))
         {
            /* Mise a jour du numero de socket */
            pl_DonCrtPICTO->Socket = pl_Socket->desc_sock;
            if (pl_DonCrtPICTO->Config.Eqt_PMV_Picto == XDC_FAUX)
            {
               XZST_03EcritureTrace(XZSTC_DEBUG1, "PICTO serie");
               EnvoyerTrame("PS",
                            XZEXC_TRAME_ETAT_AFF_PICTO,
                            pl_DonCrtPICTO->Config.AdresseRGS,
                            XZEXC_CMD_INIT,
                            XZEXC_PRIORITEFAIBLE,
                            pl_DonCrtPICTO->Socket);
               pl_DonCrtPICTO->EtatTrans = EPMVC_EQT_OCCUP;
            }
            /*B Arret: la socket avec la tache existe */
            break;
         }
         else
         {
            pl_DonCrtPICTO->Socket = pl_Socket->desc_sock;
            XZST_03EcritureTrace(XZSTC_DEBUG1, "PICTO IP");
            if (strcmp(pl_DonCrtPICTO->Config.AdresseIP, XZECC_IP_VIDE))
               EnvoyerTrameIP("PS",
                              XZEXC_TRAME_ETAT_AFF_PMV,
                              pl_DonCrtPICTO->Config.AdresseIP,
                              pl_DonCrtPICTO->Config.Port,
                              pl_DonCrtPICTO->Config.AdresseRGS,
                              XZEXC_CMD_INIT,
                              XZEXC_PRIORITEFAIBLE,
                              pl_DonCrtPICTO->Socket);
         }
      }

      /*A Si la socket avec la tache telmi n'existe pas */
      if ((pl_Socket == NULL) && (!strcmp(pl_DonCrtPICTO->Config.AdresseIP, XZECC_IP_VIDE)))
      {
         /*A Appel de ex_cnx_client pour etablir la liaison avec la tache protocole */
         if (ex_cnx_client(pl_TacheTrt,
                           pl_TacheProt,
                           &vg_SockMask,
                           &vl_SockProt) != XDC_OK)
         {
            /*B Ecriture Trace */
            XZST_03EcritureTrace(XZSTC_WARNING, "Impossible de creer la connexion client  .");
            pl_DonCrtPICTO->EtatTrans = EPMVC_EQT_PANNE;
         }
         else
         {
            /* Memorisation du numero de socket */
            pl_DonCrtPICTO->Socket = vl_SockProt;
            if (pl_DonCrtPICTO->Config.Eqt_PMV_Picto == XDC_FAUX)
            {
               if (!strcmp(pl_DonCrtPICTO->Config.AdresseIP, XZECC_IP_VIDE))
               {

                  EnvoyerTrame("PS",
                               XZEXC_TRAME_ETAT_AFF_PICTO,
                               pl_DonCrtPICTO->Config.AdresseRGS,
                               XZEXC_CMD_INIT,
                               XZEXC_PRIORITEFAIBLE,
                               pl_DonCrtPICTO->Socket);
                  pl_DonCrtPICTO->EtatTrans = EPMVC_EQT_OCCUP;
               }
               else
               {
                  EnvoyerTrameIP("PS",
                                 XZEXC_TRAME_ETAT_AFF_PICTO,
                                 pl_DonCrtPICTO->Config.AdresseIP,
                                 pl_DonCrtPICTO->Config.Port,
                                 pl_DonCrtPICTO->Config.AdresseRGS,
                                 XZEXC_CMD_INIT,
                                 XZEXC_PRIORITEFAIBLE,
                                 pl_DonCrtPICTO->Socket);
                  pl_DonCrtPICTO->EtatTrans = EPMVC_EQT_OCCUP;
               }
            }
         }
      }
   }
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Effectue les operation de terminaison de la tache
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void ec_sortir()

/*
* ARGUMENTS EN ENTREE :
*   aucun
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR :
*   aucun
*
* CONDITION D'UTILISATION
*
* FONCTION
*   Liberation des ressources memoires
*   Envoyer le message aux taches protocoles
*   Attendre x secondes
*   Fermeture des sockets
*
------------------------------------------------------*/
{
   static char *version = "$Id: epmv_cfg.c,v 1.26 2019/01/07 13:44:02 pc2dpdy Exp $ : ec_sortir";

   EPMV_DONNEES_PMV *pl_PMV, *pl_PMVALiberer;
   EPMV_DONNEES_TYPE_PMV *pl_TypePMV, *pl_TypePMVALiberer;
   EPMV_DONNEES_Picto *pl_Picto, *pl_PictoALiberer;
   EPMV_DONNEES_TYPE_Picto *pl_TypePicto, *pl_TypePictoALiberer;
   EPMV_LISTE_Picto *pl_PictoAuto, *pl_PictoAutoALiberer;
   XZEXT_MASK *pl_Socket;
   XZEXT_ADR_SOCK pl_TacheProt,
       pl_TacheTrt;
   XZEXT_MSG_SOCKET pl_message;
   XDY_Datagroup pl_NomDG;
   int vl_resultat;

   /*A Desabonnement a JourNuit */
   XDG_EncodeDG2(pl_NomDG, "Einit_JourNuit_PMV", vg_NomMachine);
   XZEA29_Abt_Etat_JourNuit(XZEAC_FERMER_ABONNEMENT, pl_NomDG, ep_journuit, vg_NomSite, &vl_resultat);

   /*A
    * Liberation des ressources memoires
    */

   /*A Liberation des ressources memoires des donnees PMV */
   for (pl_PMV = pg_debutPMV; pl_PMV != NULL; free(pl_PMVALiberer))
   {
      pl_PMVALiberer = pl_PMV;
      pl_PMV = pl_PMV->Suivant;
   }
   /*A Liberation des ressources memoires des donnees Type de PMV */
   for (pl_TypePMV = pg_debutTypePMV; pl_TypePMV != NULL; free(pl_TypePMVALiberer))
   {
      pl_TypePMVALiberer = pl_TypePMV;
      pl_TypePMV = pl_TypePMV->Suivant;
   }
   /*A Liberation des ressources memoires des donnees des Picto */
   for (pl_Picto = pg_debutPicto; pl_Picto != NULL; free(pl_PictoALiberer))
   {
      pl_PictoALiberer = pl_Picto;
      pl_Picto = pl_Picto->Suivant;
   }
   /*A Liberation des ressources memoires des donnees Type de Picto */
   for (pl_TypePicto = pg_debutTypePicto; pl_TypePicto != NULL; free(pl_TypePictoALiberer))
   {
      pl_TypePictoALiberer = pl_TypePicto;

      /*A Liberation des ressources memoires des donnees Picto autorises */
      for (pl_PictoAuto = pl_TypePicto->Liste; pl_PictoAuto != NULL; free(pl_PictoAutoALiberer))
      {
         pl_PictoAutoALiberer = pl_PictoAuto;
         pl_PictoAuto = pl_PictoAuto->Suivant;
      }

      pl_TypePicto = pl_TypePicto->Suivant;
   }

   /*A
    * Envoie du message d'arret aux taches protocoles
    */

   for (pl_Socket = vg_SockMask; pl_Socket != NULL; pl_Socket = pl_Socket->Suivant)
   {
      if (!strncmp(pl_Socket->adresse, XZEXC_ADR_SOCK_TELM, strlen(XZEXC_ADR_SOCK_TELM)))
      {
         /* Construction du message d'arret */
         sprintf(pl_message, "%s%s%s",
                 XZEXC_DEMANDE_ARRET,
                 pl_Socket->adresse,
                 XZEXC_FIN_MSG_SOCK);

         /* Envoie de la demande d'arret */
         sem_wait(&vg_semaphore);
         if (write(pl_Socket->desc_sock, pl_message, strlen(pl_message)) == XDC_NOK)
         {
            /* Ecriture trace impossible d'emettre une demande d'arret */
            XZST_03EcritureTrace(XZSTC_WARNING, " Impossible d'emettre une demande d'arret.");
         }
         sem_post(&vg_semaphore);
      }
   }

   /*A Attendre */
   sleep(XZEXC_ATTENTE_ARRET_TACHE);

   /*A
    * Fermeture de toutes les sockets
    */

   /* Pour toute les sockets du masque */
   for (pl_Socket = vg_SockMask; pl_Socket != NULL; pl_Socket = pl_Socket->Suivant)
   {
      /* Si differente de la socket RTserver */
      if (pl_Socket->desc_sock != vg_SockRT)
      {
         /* Fermeture de la socket courante */
         ex_fin_cnx(pl_Socket->desc_sock, &vg_SockMask);
      }
   }

   /*A Fin de la tache */
   exit(0);
}
