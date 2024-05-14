/* Fichier : $Id: etdp_tra.c,v 1.17 2020/11/26 15:45:44 gesconf Exp $        $Revision: 1.17 $        $Date: 2020/11/26 15:45:44 $
-------------------------------------------------------------------------------------------
 * GTIE *  PROJET MIGRAZUR
-------------------------------------------------------------------------------------------
 * SOUS-SYSTEME : EQUEXT
-------------------------------------------------------------------------------------------
 * MODULE TETDP * FICHIER etdp_tra.c
-------------------------------------------------------------------------------------------
 * DESCRIPTION DU MODULE :
 *
 * c'est le module de traitement des trames provenant des eqt temps de parcours (UGTP)
 *
-------------------------------------------------------------------------------------------
 * HISTORIQUE :
 *
 * Mismer.D	version 1.1	13 Fev 1997	: Creation (DEM/1396)
 * Mismer.D	version 1.2	25 Mar 1997	: Affichage TDP sur PMV (DEM/1417)
 * Mismer.D	version 1.3	05 Jun 1997	: Ajout lib alerte (DEM/1445) + alerte panne zone (DEM/1447)
 * Mismer.D	version 1.4	23 Jun 1997	: Suppression test alarme pour traitement donnees (DEM/1455)
 * Mismer.D      version 1.5     16 Oct 1997     : Modif pour affichage minutes pour TDP  (DEM/1505)
 * JMG   13/11/07        : ajout site de gestion DEM715  1.9
 * JPL		07/03/11 : Migration architecture HP ia64 (DEM 975) : retour de C.R. OK ou NOK  1.10
 * VR		21/11/11 : Ajout Destination Noeud dans fonction em_envoi_alarme_zone v1.9 (DEM/1014)
 * PNI		09/08/17 : Passage du code zone de 2 a 3 digits v1.12  v1.13 DEM1236
 * JMG		15/03/19 : forcer les donnees depuis pg_zones pour les TD P voisins (Marseille) 1.13
 * PNI/RGR	10/05/19 : Correction gestion des TDP voisins 					                      1.16
 * PNI		26/11/20 : Patche : rallong des buffer et mise en commentaire des envoie de ST ANT ST IPC en cas de pb 1.17
* LCL		20/12/21	: MOVIS P2 ems SAE-312
-----------------------------------------------------------------------------------------*/

/* fichiers inclus */

#include	<xdy.h>
#include	<xdc.h>
#include	<xzst.h>
#include	<xzao.h>
#include	<xzamc.h>
#include	<xzez.h>

#include 	"ex_msok.h"
#include 	"ex_mlcr.h"
#include 	"ex_mala.h"
#include 	"ex_msta.h"

#include 	"etim_don.h"

#include	"etdp_don.h"
#include	"etdp_cfg.h"
#include	"etdp_cmd.h"
#include	"etdp_dir.h"
#include	"etdp_pmv.h"
#include	"etdp_cyc.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

/* declaration de fonctions internes */
int em_envoi_alarme_zone ( int, int, int, XDY_Horodate, char * );
int em_traite_mesure_6MN ( int, XDY_Horodate );

/* definition de fonctions externes */
extern int et_calcul_seuil_TDP ( XDY_Eqt );


/*X*/
/*-----------------------------------------------------------------------------------------
 * SERVICE RENDU :
 *  Decodage de la trame et traitement
--------------------------------------------------------------------------------------------
 * SEQUENCE D'APPEL :
 */

int em_traitement_trame( int va_socket, int va_indice_table_eqt)

/*
 * ARGUMENTS EN ENTREE :
 *
 *	va_socket		:	socket de lecture
 *	va_indice_table_eqt	:	Indice de l'equipement dans la table des donnees
 *
 * ARGUMENTS EN SORTIE :
 *
 *
 * CODE RETOUR :
 *
 * CONDITION D'UTILISATION
 * execute sur reception de message socket
 *
 * FONCTION
 *
 *  Decodage de la trame et traitement
-----------------------------------------------------------------------------------------*/
{

  int vl_IndiceUGTP = 0;
  int i = 0;
  int vl_IndCnx = 0;
  int vl_LgMsg  = 0;
  char	*pl_trame;
  char	*pl_Texte;
  char vl_entete1[10];
  char vl_entete2[10];
  char vl_adrrgs[10];
  int vl_typetrame;
  char vl_Msg[2560];
  int vl_param;
  int vl_nbnonrep;
  int vl_nberrcrc;
  XDY_Horodate vl_horodate;
  int vl_nocmd;
  char vl_horod1[20],vl_date[20];
  char vl_horod2[9],vl_heure[9];
  XZSMT_Horodate pl_Horodatedecompo;
  int	vl_heures = 0;
  int	vl_Minutes = 0;
  int vl_typ_rep;
  int vl_Alarme = 0, vl_Cpt = 0, vl_ITP = 0;
  char vl_Status[1280] = "";
  char vl_InfoStatus[1280] = "";
  char vl_TempSatus[800] = "";
  XZEXT_TEXTE_STATUS		vl_tabstatus[110];
  int		vl_Zone, vl_Temps, vl_NbApp, vl_NbZone = 0;
  int		vl_Index = 0;
  int		vl_longueur = 0, vl_pas_pointeur = 0;
  XDY_TDP 	vl_Tdp = 0;
  ldiv_t	vl_Result_Div;

  /*
   *  Lecture du message recu dans la socket appelante
   */

  if ( ex_lire_msg ( va_socket, vl_Msg, &vl_LgMsg, &vg_SockMask, &vl_IndCnx ) != XDC_OK )
  {
    /* Ecriture trace Erreur lecture message socket. */
    XZST_03EcritureTrace( XZSTC_WARNING, " Erreur lecture message socket LS. " );
  }
  else
  {
    XZST_03EcritureTrace( XZSTC_WARNING, " --------------- message : %s", vl_Msg);

    /* Scan du message provenant de la socket */

    vl_param=sscanf(vl_Msg,"%s %s %s %s %d %d %d %s %d %d",vl_entete1,
        vl_entete2, vl_horod1, vl_horod2, &vl_nbnonrep, &vl_nberrcrc,
        &vl_nocmd, vl_adrrgs, &vl_typetrame, &vl_typ_rep);

    if (vl_param != 10)
    {
      XZST_03EcritureTrace( XZSTC_WARNING,
          " Lecture message socket maitre impossible mes=%s",vl_Msg);
      return (XDC_NOK);
    }

    /* Conversion de l'horodate recue */
    sscanf ( vl_horod2, "%02d:%02d:", &vl_heures,&vl_Minutes );
    sprintf(pl_Horodatedecompo,"%s %02d:%02d:%s", vl_horod1, vl_heures, vl_Minutes,"01");
    XZSM_13ConversionHorodateSec(pl_Horodatedecompo,&vl_horodate);

    /* Recherche l'equipement concerne */
    for( i=0; i!=ETDPC_NB_MAX_UGTP; i++)
    {
      if ( !strcmp(vg_DonneesUGTP[i].AdresseRGS, vl_adrrgs) )
      {
        vl_IndiceUGTP = i;
        break;
      }
    }

    /* Si l'equipement concerne ne peut pas etre touve, retourne erreur*/
    if ( i == ETDPC_NB_MAX_UGTP )
    {
      XZST_03EcritureTrace( XZSTC_WARNING, "Equipement inconnu  mes=%s",vl_Msg);
      return (XDC_NOK);
    }

    /* Gestion des trames provenant de telmi */

    /* Si l'equipement n'est pas declare HS */

    if( !(vg_DonneesUGTP[vl_IndiceUGTP].Etat_Service & XDC_EQT_HS) )
    {
      /* Alors */
      /* Traitement des alarmes transmission avec l'equipement */
      ex_traite_alarme_com ( 	vg_ConfigTDP.NomMachine,
          XDC_EQT_TDP,
          vg_DonneesUGTP[vl_IndiceUGTP].Numero,
          vl_typ_rep,
          vl_nbnonrep,
          vl_nberrcrc,
          vl_typetrame,
          vg_ConfigTDP.Mode_Fonct,
          XZSC_22NumeroSiteLocal());
    }
    /* Finsi */
    /* On se positionne sur le debut de la trame */

    pl_trame=strstr(vl_Msg,XZEXC_SEP_TRAME)+3;

    XZST_03EcritureTrace( XZSTC_WARNING, " --------------- Analyse TRAME : %s", pl_trame);

    /* Recherche du type de trame */
    switch(vl_typetrame)
    {
      /*B C'est une trame de reponse pour la fenetre LCR */
      case XZEXC_TRAME_FEN_LCR:
      {

        /* Appel de ex_mrcr */
        ex_ret_LCR( pl_trame, vl_adrrgs, &pg_DebutListeTDPFLcr, vl_typ_rep );
        break;
      }

      /* ******** c'est une trame de status complet (retour ST) ******** */
      case XZEXC_TRAME_STAT_COMP:
        XZST_03EcritureTrace( XZSTC_WARNING, "er_traitement_trame : Demande Status Complet ");

        if ( ( vl_typ_rep != XZEXC_TRAME_NON_TRANSM ) && ( vl_typ_rep != XZEXC_REP_NAK ) )
        {
          /* Decodage de la trame de status */
          ex_decode_status ( 	pl_trame,
              vl_tabstatus,
              10,
              vl_adrrgs,
              vl_horodate,
              XDC_EQT_TDP,
              vg_DonneesUGTP[vl_IndiceUGTP].Numero,
              vg_ConfigTDP.NomMachine,
              XZSC_22NumeroSiteLocal());

          /* Si probleme antenne alors demande ST ANT */
	  /*
          if ( strstr(pl_trame, ETDPC_ETAT_ANT_OK) == NULL )
          {
            EnvoyerTrame(	"ST ANT",
                XZEXC_TRAME_STAT_ANT,
                vl_adrrgs,
                XZEXC_NOCMD,
                XZEXC_PRIORITEFAIBLE,
                va_socket );
            vg_DonneesUGTP[vl_IndiceUGTP].StatusTR = XZEXC_TRAME_STAT_ANT;
          }
          else
          {

            for ( vl_Index = 0; vl_Index <= ETDPC_NB_MAX_ZONE; vl_Index++ )
              vg_DonneesUGTP[vl_IndiceUGTP].Zone[vl_Index].AlarmeAntenne = XDC_FAUX;
            ex_decode_status_comp  ( ETDPC_ETAT_ANT_OK, vl_Status, &vl_Alarme );
            em_envoi_alarme_zone ( vl_IndiceUGTP, XZEXC_TRAME_STAT_ANT, vl_Alarme, vl_horodate, "" );
          }
*/
          /* Si probleme ITP alors demande ST ITP */
  /*        if ( strstr(pl_trame, ETDPC_ETAT_ITP_OK) == NULL )
          {
            EnvoyerTrame("ST ITP",
                XZEXC_TRAME_STAT_ITP,
                vl_adrrgs,
                XZEXC_NOCMD,
                XZEXC_PRIORITEFAIBLE,
                va_socket );
            vg_DonneesUGTP[vl_IndiceUGTP].StatusTR = XZEXC_TRAME_STAT_ITP;
          }
          else
          {
            for ( vl_Index = 0; vl_Index <= ETDPC_NB_MAX_ZONE; vl_Index++ )
              vg_DonneesUGTP[vl_IndiceUGTP].Zone[vl_Index].AlarmeITP = XDC_FAUX;
            ex_decode_status_comp  ( ETDPC_ETAT_ITP_OK, vl_Status, &vl_Alarme );
            em_envoi_alarme_zone ( vl_IndiceUGTP, XZEXC_TRAME_STAT_ITP, vl_Alarme, vl_horodate, "" );
          }
*/
          /* Si probleme transmission alors demande ST THS */
  /*        if ( strstr(pl_trame, ETDPC_ETAT_TRANS_OK) == NULL )
          {
            EnvoyerTrame(	"ST THS",
                XZEXC_TRAME_STAT_TRANS,
                vl_adrrgs,
                XZEXC_NOCMD,
                XZEXC_PRIORITEFAIBLE,
                va_socket );
            vg_DonneesUGTP[vl_IndiceUGTP].StatusTR = XZEXC_TRAME_STAT_TRANS;
          }
          else
          {
            for ( vl_Index = 0; vl_Index <= ETDPC_NB_MAX_ZONE; vl_Index++ )
              vg_DonneesUGTP[vl_IndiceUGTP].Zone[vl_Index].AlarmeTrans = XDC_FAUX;
            ex_decode_status_comp  ( ETDPC_ETAT_TRANS_OK, vl_Status, &vl_Alarme );
            em_envoi_alarme_zone ( vl_IndiceUGTP, XZEXC_TRAME_STAT_TRANS, vl_Alarme, vl_horodate, "" );
          }
*/
        }
        if ( vg_DonneesUGTP[vl_IndiceUGTP].StatusTR == XZEXC_TRAME_STAT_COMP )
        {
          XZST_03EcritureTrace( XZSTC_WARNING, "***** TRAITEMENT 6 MIN : ST : GLOBAL ");
          em_traite_mesure_6MN ( vl_IndiceUGTP, vl_horodate );
        }
        else
        {
          XZST_03EcritureTrace( XZSTC_WARNING, "PAS DE TRAITEMENT 6 MIN : ST : GLOBAL : ");
        }
        break;

        /* ******** c'est une trame de status antenne (retour ST ANT) ******** */
      case XZEXC_TRAME_STAT_ANT:
        XZST_03EcritureTrace( XZSTC_DEBUG1, "er_traitement_trame : Demande Status Antenne " );
        if ( ( vl_typ_rep != XZEXC_TRAME_NON_TRANSM ) && ( vl_typ_rep != XZEXC_REP_NAK ) )
        {
          /* Decodage de la trame de status */
          ex_decode_status_comp  ( pl_trame, vl_Status, &vl_Alarme );
          strcpy ( vl_InfoStatus, strtok(vl_Status,"\n") );
          XZST_03EcritureTrace( XZSTC_WARNING, "Status Antenne <%s> Ala:%d", vl_InfoStatus, vl_Alarme);
          if ( vl_Alarme != 0 )
          {
            for ( vl_Index = 0; vl_Index <= ETDPC_NB_MAX_ZONE; vl_Index++ )
              vg_DonneesUGTP[vl_IndiceUGTP].Zone[vl_Index].AlarmeAntenne = XDC_FAUX;
            pl_Texte = strtok(vl_Status,"=");
            while ( (pl_Texte = strtok(NULL," ")) != NULL )
            {
              if ( (vl_Cpt = atoi (pl_Texte)) != 0 )
              {
                for ( vl_Index = 0; vl_Index <= ETDPC_NB_MAX_ZONE; vl_Index++ )
                {
                  if ( (vg_DonneesUGTP[vl_IndiceUGTP].Zone[vl_Index].NumAntDeb == vl_Cpt) ||
                      (vg_DonneesUGTP[vl_IndiceUGTP].Zone[vl_Index].NumAntFin == vl_Cpt) )
                  {
                    vg_DonneesUGTP[vl_IndiceUGTP].Zone[vl_Index].AlarmeAntenne = XDC_VRAI;
                  }

                }
              }
            }
            em_envoi_alarme_zone ( vl_IndiceUGTP, XZEXC_TRAME_STAT_ANT, vl_Alarme, vl_horodate, vl_InfoStatus );
          }
        }

        if ( vg_DonneesUGTP[vl_IndiceUGTP].StatusTR == XZEXC_TRAME_STAT_ANT )
        {
          XZST_03EcritureTrace( XZSTC_WARNING, "***** TRAITEMENT 6 MIN : ST : ANTENNE ");
          em_traite_mesure_6MN ( vl_IndiceUGTP, vl_horodate );
        }
        else
        {
          XZST_03EcritureTrace( XZSTC_WARNING, "PAS DE TRAITEMENT 6 MIN : ST : ANTENNE : ");
        }
        break;

        /* ******** c'est une trame de status ITP (retour ST ITP) ******** */
      case XZEXC_TRAME_STAT_ITP:
        XZST_03EcritureTrace( XZSTC_DEBUG1, "er_traitement_trame : Demande Status ITP " );
        if ( ( vl_typ_rep != XZEXC_TRAME_NON_TRANSM ) && ( vl_typ_rep != XZEXC_REP_NAK ) )
        {
          /* Decodage de la trame de status */
          ex_decode_status_comp  ( pl_trame, vl_Status, &vl_Alarme );
          strcpy ( vl_InfoStatus, strtok(vl_Status,"\n") );
          XZST_03EcritureTrace( XZSTC_WARNING, "Status ITP <%s> Ala:%d", vl_InfoStatus, vl_Alarme);
          if ( vl_Alarme != 0 )
          {
            for ( vl_Index = 0; vl_Index <= ETDPC_NB_MAX_ZONE; vl_Index++ )
              vg_DonneesUGTP[vl_IndiceUGTP].Zone[vl_Index].AlarmeITP = XDC_FAUX;
            pl_Texte = strtok(vl_Status,"=");
            while ( (pl_Texte = strtok(NULL," ")) != NULL )
            {
              if ( (vl_ITP = atoi (pl_Texte)) != 0 )
              {
                for ( vl_Index = 0; vl_Index <= ETDPC_NB_MAX_ZONE; vl_Index++ )
                {
                  if ( (vg_DonneesUGTP[vl_IndiceUGTP].Zone[vl_Index].NumITPDeb == vl_ITP) ||
                      (vg_DonneesUGTP[vl_IndiceUGTP].Zone[vl_Index].NumITPFin == vl_ITP) )
                  {
                    vg_DonneesUGTP[vl_IndiceUGTP].Zone[vl_Index].AlarmeITP = XDC_VRAI;
                  }

                }
              }
            }
            em_envoi_alarme_zone ( vl_IndiceUGTP, XZEXC_TRAME_STAT_ITP, vl_Alarme, vl_horodate, vl_InfoStatus );

          }
        }

        if ( vg_DonneesUGTP[vl_IndiceUGTP].StatusTR == XZEXC_TRAME_STAT_ITP )
        {
          XZST_03EcritureTrace( XZSTC_WARNING, "***** DE TRAITEMENT 6 MIN : ST : ITP : ");
          em_traite_mesure_6MN ( vl_IndiceUGTP, vl_horodate );
        }
        else
        {
          XZST_03EcritureTrace( XZSTC_WARNING, "PAS DE TRAITEMENT 6 MIN : ST : ITP : ");
        }
        break;

        /* ******** c'est une trame de status transmission (retour ST THS) ******** */
      case XZEXC_TRAME_STAT_TRANS:
        XZST_03EcritureTrace( XZSTC_DEBUG1, "er_traitement_trame : Demande Status Transmission " );
        if ( ( vl_typ_rep != XZEXC_TRAME_NON_TRANSM ) && ( vl_typ_rep != XZEXC_REP_NAK ) )
        {
          /* Decodage de la trame de status */
          ex_decode_status_comp  ( pl_trame, vl_Status, &vl_Alarme );
          strcpy ( vl_InfoStatus, strtok(vl_Status,"\n") );
          XZST_03EcritureTrace( XZSTC_WARNING, "Status Transmission <%s> Ala:%d", vl_InfoStatus, vl_Alarme);
          if ( vl_Alarme != 0 )
          {
            for ( vl_Index = 0; vl_Index <= ETDPC_NB_MAX_ZONE; vl_Index++ )
              vg_DonneesUGTP[vl_IndiceUGTP].Zone[vl_Index].AlarmeTrans = XDC_FAUX;
            pl_Texte = strtok(vl_Status,"=");
            while ( (pl_Texte = strtok(NULL," ")) != NULL )
            {
              sscanf ( pl_Texte, "%s", vl_TempSatus );
              vl_ITP = vl_Cpt = 0;
              sscanf ( vl_TempSatus, "%d.%d", &vl_ITP, &vl_Cpt );
              if ( (vl_Cpt != 0) && (vl_ITP != 0) )
              {
                for ( vl_Index = 0; vl_Index <= ETDPC_NB_MAX_ZONE; vl_Index++ )
                {
                  if ( ((vg_DonneesUGTP[vl_IndiceUGTP].Zone[vl_Index].NumITPDeb == vl_ITP) &&
                      (vg_DonneesUGTP[vl_IndiceUGTP].Zone[vl_Index].NumCptDeb == vl_Cpt)) ||
                      ((vg_DonneesUGTP[vl_IndiceUGTP].Zone[vl_Index].NumITPFin == vl_ITP) &&
                          (vg_DonneesUGTP[vl_IndiceUGTP].Zone[vl_Index].NumCptFin == vl_Cpt)) )
                  {
                    vg_DonneesUGTP[vl_IndiceUGTP].Zone[vl_Index].AlarmeTrans = XDC_VRAI;
                  }

                }
              }
              if ( (vl_Cpt == 0) && (vl_ITP != 0) )
              {
                for ( vl_Index = 0; vl_Index <= ETDPC_NB_MAX_ZONE; vl_Index++ )
                {
                  if ( (vg_DonneesUGTP[vl_IndiceUGTP].Zone[vl_Index].NumITPDeb == vl_ITP) ||
                      (vg_DonneesUGTP[vl_IndiceUGTP].Zone[vl_Index].NumITPFin == vl_ITP) )
                  {
                    vg_DonneesUGTP[vl_IndiceUGTP].Zone[vl_Index].AlarmeTrans = XDC_VRAI;
                  }

                }
              }
            }
            em_envoi_alarme_zone ( vl_IndiceUGTP, XZEXC_TRAME_STAT_TRANS, vl_Alarme, vl_horodate, vl_InfoStatus );
          }
        }

        if ( vg_DonneesUGTP[vl_IndiceUGTP].StatusTR == XZEXC_TRAME_STAT_TRANS )
        {
          XZST_03EcritureTrace( XZSTC_WARNING, "***** TRAITEMENT 6 MIN : ST : TRANS : ");
          em_traite_mesure_6MN ( vl_IndiceUGTP, vl_horodate );
        }
        else
        {
          XZST_03EcritureTrace( XZSTC_WARNING, "PAS DE TRAITEMENT 6 MIN : ST : TRANS : ");
        }

        break;

        /*B Recuperation des donnees 6mn */
      case XZEXC_TRAME_DONNEES_TDP_6MN :
        /* Traitement de la TRAME recue */
        XZST_03EcritureTrace( XZSTC_WARNING, " ***** TRAME DONNEES 6 MN : %d : %d : %s :", vl_typetrame, vl_typ_rep, pl_trame);

        /*B Si le status temps reel est different de "@" => demande du status complet */
        if ( (strstr(pl_trame, ETDPC_STATUS_TMP_REEL_STRG ) == NULL ) &&
            (vl_typ_rep != XZEXC_TRAME_NON_TRANSM ) &&
            (vl_typ_rep != XZEXC_REP_NAK ) )
        {

          EnvoyerTrame(	"ST",
              XZEXC_TRAME_STAT_COMP,
              vl_adrrgs,
              XZEXC_NOCMD,
              XZEXC_PRIORITEFAIBLE,
              va_socket );
          strcpy ( vg_DonneesUGTP[vl_IndiceUGTP].TrameMB, pl_trame );
          vg_DonneesUGTP[vl_IndiceUGTP].StatusTR = XZEXC_TRAME_STAT_COMP;
        }
        else
        {
          if ( ( vl_typ_rep != XZEXC_TRAME_NON_TRANSM ) && ( vl_typ_rep != XZEXC_REP_NAK ) )
          {
            strcpy ( vg_DonneesUGTP[vl_IndiceUGTP].TrameMB, strtok(pl_trame, ETDPC_STATUS_TMP_REEL_STRG));
          }
          else
          {
            strcpy ( vg_DonneesUGTP[vl_IndiceUGTP].TrameMB, " " );
          }
          XZST_03EcritureTrace( XZSTC_WARNING, "OK OK ****************** TRAITEMENT 6 MN : STATUS : 6MN");
          em_traite_mesure_6MN ( vl_IndiceUGTP, vl_horodate );
        }

        break;

        /*B Recuperation des donnees HORAIRE */
      case XZEXC_TRAME_DONNEES_TDP_HORAIRE :
        /* Traitement de la TRAME recue */

        for ( vl_Index = 0; vl_Index <= ETDPC_NB_MAX_ZONE; vl_Index++ )
          if ( vg_DonneesUGTP[vl_IndiceUGTP].Zone[vl_Index].ChampValide == Valide )
          {
            vg_Mesure[vl_IndiceUGTP].TDPHoraire[vl_Index] = 0;
            vg_Mesure[vl_IndiceUGTP].NbApp[vl_Index] = 0;
            vg_Mesure[vl_IndiceUGTP].ValiditeHoraire[vl_Index] = XDC_FAUX;
          }
        vg_DonneesUGTP[vl_IndiceUGTP].Horodate1H = vg_DonneesUGTP[vl_IndiceUGTP].Horodate_Histo;

        if ( ( vl_typ_rep != XZEXC_TRAME_NON_TRANSM ) && ( vl_typ_rep != XZEXC_REP_NAK ) )
        {
          vl_longueur = (int) strlen(pl_trame);
          for ( vl_Index = 0; vl_Index <=vl_longueur ; vl_Index++ )
            if ( pl_trame[vl_pas_pointeur] == ' ' ) vl_pas_pointeur++;
            else break;
          sscanf(pl_trame+vl_pas_pointeur, "%02d", &vl_NbZone);
          vl_pas_pointeur += 2;
          for ( vl_Index = 0; vl_Index <=vl_longueur ; vl_Index++ )
            if ( pl_trame[vl_pas_pointeur] == ' ' ) vl_pas_pointeur++;
            else break;

          /*B Lecture de la trame recue */
          while ( (vl_pas_pointeur < ( vl_longueur - 1 )) && (vl_NbZone-- > 0) )
          {
            if ( sscanf ( pl_trame+vl_pas_pointeur, "%03d%04d%04d", &vl_Zone, &vl_Temps, &vl_NbApp ) == 3 )
            {
              XZST_03EcritureTrace( XZSTC_DEBUG1, "Donnee horaire : Zone = %d\tTemps = %d\tNb App = %d", vl_Zone, vl_Temps, vl_NbApp );
              if ( (vl_Zone > 0) &&
                  (vl_Zone <= ETDPC_NB_MAX_ZONE) &&
                  (vg_DonneesUGTP[vl_IndiceUGTP].Zone[vl_Zone].ChampValide == Valide) /*!!! &&
	      		    (vg_DonneesUGTP[vl_IndiceUGTP].Zone[vl_Zone].AlarmeTrans == XDC_FAUX) &&
	      		    (vg_DonneesUGTP[vl_IndiceUGTP].Zone[vl_Zone].AlarmeAntenne == XDC_FAUX) &&
	      		    (vg_DonneesUGTP[vl_IndiceUGTP].Zone[vl_Zone].AlarmeITP == XDC_FAUX) */ )
              {
                vg_Mesure[vl_IndiceUGTP].TDPHoraire[vl_Zone] 		= vl_Temps;
                vg_Mesure[vl_IndiceUGTP].NbApp[vl_Zone] 		= vl_NbApp;
                vg_Mesure[vl_IndiceUGTP].ValiditeHoraire[vl_Zone] 	= XDC_VRAI;
              }
            }
            vl_pas_pointeur += ETDPC_LONG_DONNEE_1H;
          } /*fin while */
          vg_DonneesUGTP[vl_IndiceUGTP].Horodate_Histo += (vg_DonneesUGTP[vl_IndiceUGTP].Nb_Heure_Histo*3600);
          vg_DonneesUGTP[vl_IndiceUGTP].Nb_Heure_Histo = 0;
          et_Stockage_Mesures_horaire ( vl_IndiceUGTP, XDC_VRAI );
          /*!! 	        et_Envoi_Mesure_TDP_Horaire ( vl_IndiceUGTP, XDC_FAUX ); */
        }
        else
        {
          XZST_03EcritureTrace( XZSTC_WARNING,"Reponse incorrecte pour Eqt %d", vg_DonneesUGTP[vl_IndiceUGTP].Numero );
          et_Stockage_Mesures_horaire ( vl_IndiceUGTP, XDC_FAUX );
          /*!!               if ( vg_DonneesUGTP[vl_IndiceUGTP].Nb_Heure_Histo == 1 )
	          et_Envoi_Mesure_TDP_Horaire ( vl_IndiceUGTP, XDC_FAUX ); */
        }
        if ( vg_DonneesUGTP[vl_IndiceUGTP].Horodate_Histo + (vg_DonneesUGTP[vl_IndiceUGTP].Nb_Heure_Histo*3600) > vl_horodate )
          et_Envoi_Mesure_TDP_Horaire ( vl_IndiceUGTP, XDC_FAUX );
        ey_envoi_demande_donnees_horaire ( vl_IndiceUGTP );

        break;

    }
  }

  return (XDC_OK);
}

/*X*/
/*-----------------------------------------------------------------------------------------
 * SERVICE RENDU :
 *  envoi a la base les alarme zone
--------------------------------------------------------------------------------------------
 * SEQUENCE D'APPEL :
 */

int em_envoi_alarme_zone ( int va_NumUGTP, int va_TypeAlarme, int va_NumAlarme, XDY_Horodate va_horodate, char * va_TexteInfo )

/*
 * ARGUMENTS EN ENTREE :
 *
 *	va_NumUGTP	:	numero d'UGTP
 *	va_TypeAlarme	:	type d'alarme (THS ou ANT)
 *	va_NumAlarme	:	numero d'alarme
 *	va_horodate	:	horodate d'alarme
 *	va_TexteInfo	:	texte d'info d'alarme
 *
 * ARGUMENTS EN SORTIE :
 *
 *
 * CODE RETOUR :
 *
 * CONDITION D'UTILISATION
 *
 * FONCTION
 *
-----------------------------------------------------------------------------------------*/
{
  int			vl_Index = 0;
  XZEZT_Liste_Zdp *	pl_Zone = NULL;
  ETDP_CONFIG_PMV *	pl_PMV  = NULL;
  XDY_TexteAlerte		vl_TexteLib = "";
  XDY_TexteAlerte		vl_TexteZone = "";
  XDY_TexteAlerte		vl_TexteAlerte = "";
  XZST_03EcritureTrace( XZSTC_WARNING, "IN  :em_envoi_alarme_zone" );

  for ( vl_Index = 0; vl_Index <= ETDPC_NB_MAX_ZONE; vl_Index++ )
    if ( !(vg_DonneesUGTP[va_NumUGTP].Zone[vl_Index].Etat_Service & XDC_EQT_HS) &&
        (vg_DonneesUGTP[va_NumUGTP].Zone[vl_Index].ChampValide == Valide) )
    {
      XZST_03EcritureTrace( XZSTC_WARNING, "Appel de ex_env_alarme : %s, %d %d %d %s" ,vg_ConfigTDP.NomMachine,vg_Mesure[va_NumUGTP].Numero[vl_Index],va_NumAlarme,(va_TypeAlarme == XZEXC_TRAME_STAT_TRANS) ? \
          vg_DonneesUGTP[va_NumUGTP].Zone[vl_Index].AlarmeTrans : \
          (va_TypeAlarme == XZEXC_TRAME_STAT_ANT) ? \
              vg_DonneesUGTP[va_NumUGTP].Zone[vl_Index].AlarmeAntenne : \
              vg_DonneesUGTP[va_NumUGTP].Zone[vl_Index].AlarmeITP,
              va_TexteInfo);
      ex_env_alarme ( vg_ConfigTDP.NomMachine,
          va_horodate,
          XDC_EQT_ZDP,
          vg_Mesure[va_NumUGTP].Numero[vl_Index],
          va_NumAlarme,
          (va_TypeAlarme == XZEXC_TRAME_STAT_TRANS) ? \
              vg_DonneesUGTP[va_NumUGTP].Zone[vl_Index].AlarmeTrans : \
              (va_TypeAlarme == XZEXC_TRAME_STAT_ANT) ? \
                  vg_DonneesUGTP[va_NumUGTP].Zone[vl_Index].AlarmeAntenne : \
                  vg_DonneesUGTP[va_NumUGTP].Zone[vl_Index].AlarmeITP,
                  va_TexteInfo,
                  XZSC_22NumeroSiteLocal());

      /* Envoi alerte panne zone si affichage PMV avec zone concernee */
      if ( (vg_DonneesUGTP[va_NumUGTP].Zone[vl_Index].EtatPanne == XDC_FAUX) && vg_ConfigTDP.NumPanne &&
          (((va_TypeAlarme == XZEXC_TRAME_STAT_TRANS) && vg_DonneesUGTP[va_NumUGTP].Zone[vl_Index].AlarmeTrans) ||
              ((va_TypeAlarme == XZEXC_TRAME_STAT_ANT) && vg_DonneesUGTP[va_NumUGTP].Zone[vl_Index].AlarmeAntenne) ||
              ((va_TypeAlarme == XZEXC_TRAME_STAT_ITP) && vg_DonneesUGTP[va_NumUGTP].Zone[vl_Index].AlarmeITP)) )
      {
        vl_TexteLib[0] = '\0';
        for ( pl_PMV = pg_ListePMV; pl_PMV != NULL; pl_PMV = pl_PMV->suivant )
        {
          if ( pl_PMV->EtatPMV.Destination )
          {
            if ( et_Recherche_zone_PMV ( pl_PMV->PMV.Autoroute,
                pl_PMV->PMV.PR,
                pl_PMV->PMV.Sens,
                pl_PMV->EtatPMV.Destination,
                vg_Mesure[va_NumUGTP].Numero[vl_Index] ) == XDC_OK )
            {
              strcat ( vl_TexteLib, pl_PMV->Nom);
            }

          }
          if ( pl_PMV->EtatPMV.DestinationAlternat )
          {
            if ( et_Recherche_zone_PMV ( pl_PMV->PMV.Autoroute,
                pl_PMV->PMV.PR,
                pl_PMV->PMV.Sens,
                pl_PMV->EtatPMV.DestinationAlternat,
                vg_Mesure[va_NumUGTP].Numero[vl_Index] ) == XDC_OK )
            {
              strcat ( vl_TexteLib, pl_PMV->Nom);
            }
          }
          if ( pl_PMV->EtatPMV.DestinationNoeud )
          {
            if ( et_Recherche_zone_PMV ( pl_PMV->PMV.Autoroute,
                pl_PMV->PMV.PR,
                pl_PMV->PMV.Sens,
                pl_PMV->EtatPMV.DestinationNoeud,
                vg_Mesure[va_NumUGTP].Numero[vl_Index] ) == XDC_OK )
            {
              strcat ( vl_TexteLib, pl_PMV->Nom);
            }
          }
        }
        if ( vl_TexteLib[0] != '\0' )
        {
          vg_DonneesUGTP[va_NumUGTP].Zone[vl_Index].EtatPanne = XDC_VRAI;
          for ( pl_Zone = pg_ListeZone; pl_Zone != NULL; pl_Zone = pl_Zone->suivant )
            if ( pl_Zone->zdp.Numero == vg_Mesure[va_NumUGTP].Numero[vl_Index] )
            {
              sprintf ( vl_TexteZone, "%s %s", vg_DonneesUGTP[va_NumUGTP].Zone[vl_Index].Nom, pl_Zone->zdp.Libelle );
              break;
            }
          sprintf ( vl_TexteAlerte, vg_ConfigTDP.LibPanne, vl_TexteZone, vl_TexteLib );
          ex_env_alerte  ( va_horodate,
              XDC_EQT_ZDP,
              vg_Mesure[va_NumUGTP].Numero[vl_Index],
              vg_ConfigTDP.NumPanne,
              XZEXC_MODE_NORMAL,
              XZSC_22NumeroSiteLocal(),
              vl_TexteAlerte,
              NULL );
        }

      }
      if ( !vg_DonneesUGTP[va_NumUGTP].Zone[vl_Index].AlarmeTrans &&
          !vg_DonneesUGTP[va_NumUGTP].Zone[vl_Index].AlarmeAntenne &&
          !vg_DonneesUGTP[va_NumUGTP].Zone[vl_Index].AlarmeITP )
      {
        vg_DonneesUGTP[va_NumUGTP].Zone[vl_Index].EtatPanne = XDC_FAUX;
      }

    }

  return (XDC_OK);
}



/*X*/
/*-----------------------------------------------------------------------------------------
 * SERVICE RENDU :
 *  envoi a la base les alarme zone
--------------------------------------------------------------------------------------------
 * SEQUENCE D'APPEL :
 */

int em_traite_mesure_6MN ( int va_NumUGTP, XDY_Horodate va_horodate )

/*
 * ARGUMENTS EN ENTREE :
 *
 *	va_NumUGTP	:	numero d'UGTP
 *	va_horodate	:	horodate d'acquisition
 *
 * ARGUMENTS EN SORTIE :
 *
 *
 * CODE RETOUR :
 *
 * CONDITION D'UTILISATION
 *
 * FONCTION
 *
-----------------------------------------------------------------------------------------*/
{
  int		vl_Zone, vl_Temps, vl_NbZone = 0;
  int		vl_Index = 0;
  int		vl_longueur = 0, vl_pas_pointeur = 0;
  XDY_TDP 	vl_Tdp = 0;
  int		vl_Minutes = 0;
  int		vl_NbParam = 0;
  ldiv_t	vl_Result_Div;
  char 	vl_Trace[ETDPC_LONG_DONNEE_6MN+1] = "";
  XDY_TexteAlerte	vl_TexteLib = "";
  XDY_TexteAlerte	vl_TexteAlerte = "";
  XZEZT_Liste_Zdp *	pl_Zone = NULL;


  XZST_03EcritureTrace( XZSTC_FONCTION, "IN  :em_traite_mesure_6MN" );
  vg_DonneesUGTP[va_NumUGTP].StatusTR = 0;

  if ( vg_DonneesUGTP[va_NumUGTP].TrameMB[0] == '\0' ) return (XDC_OK);

  XZST_03EcritureTrace( XZSTC_WARNING, "IN  :em_traite_mesure_6MN <%s>", vg_DonneesUGTP[va_NumUGTP].TrameMB );

  for ( vl_Index = 0; vl_Index <= ETDPC_NB_MAX_ZONE; vl_Index++ )
    if ( vg_DonneesUGTP[va_NumUGTP].Zone[vl_Index].ChampValide == Valide )
    {
      vg_Mesure[va_NumUGTP].TDP6mn[vl_Index] = 0;
      vg_Mesure[va_NumUGTP].Validite6mn[vl_Index] = XDC_FAUX;
    }

  vl_longueur = (int) strlen(vg_DonneesUGTP[va_NumUGTP].TrameMB);

  for ( vl_Index = 0; vl_Index <=vl_longueur ; vl_Index++ )
    if ( vg_DonneesUGTP[va_NumUGTP].TrameMB[vl_pas_pointeur] == ' ' ) vl_pas_pointeur++;
    else break;

  vl_NbParam = sscanf(&vg_DonneesUGTP[va_NumUGTP].TrameMB[vl_pas_pointeur], "%02d", &vl_NbZone);
  vl_pas_pointeur += 2;

  for ( vl_Index = 0; vl_Index <=vl_longueur ; vl_Index++ )
    if ( vg_DonneesUGTP[va_NumUGTP].TrameMB[vl_pas_pointeur] == ' ' ) vl_pas_pointeur++;
    else break;

  /*B Lecture de la trame recue */
  while ( (vl_pas_pointeur < ( vl_longueur - 1 )) && (vl_NbZone-- > 0) )
  {
    vl_Trace[0] = '\0';
    strncpy ( vl_Trace, &vg_DonneesUGTP[va_NumUGTP].TrameMB[vl_pas_pointeur], ETDPC_LONG_DONNEE_6MN );
    vl_NbParam = sscanf(vl_Trace, "%03d%04d", &vl_Zone, &vl_Temps);
    XZST_03EcritureTrace( XZSTC_INFO, "%d %d Donnee 6mn : Zone = %d Temps = %d <%s> param:%d NZ:%d vl_NbParam=%d ChampValide=%d", vl_pas_pointeur,vl_longueur,
        vl_Zone, vl_Temps, vl_Trace, vl_NbParam, vl_NbZone ,vl_NbParam,vg_DonneesUGTP[va_NumUGTP].Zone[vl_Zone].ChampValide );
    if ( (vl_NbParam == 2) &&
        (vl_Zone > 0) &&
        (vl_Zone <= ETDPC_NB_MAX_ZONE) &&
        (vg_DonneesUGTP[va_NumUGTP].Zone[vl_Zone].ChampValide == Valide) /*!!! &&
	   (vg_DonneesUGTP[va_NumUGTP].Zone[vl_Zone].AlarmeTrans == XDC_FAUX) &&
	   (vg_DonneesUGTP[va_NumUGTP].Zone[vl_Zone].AlarmeAntenne == XDC_FAUX) &&
	   (vg_DonneesUGTP[va_NumUGTP].Zone[vl_Zone].AlarmeITP == XDC_FAUX) */ )
    {
      vg_DonneesUGTP[va_NumUGTP].Zone[vl_Zone].EtatAlerte = XDC_FAUX;
      vg_Mesure[va_NumUGTP].TDP6mn[vl_Zone] 	= vl_Temps;
      if ( vl_Temps == 0 )
        vg_Mesure[va_NumUGTP].Validite6mn[vl_Zone] = ETDPC_MES_INCONNU;
      else
        vg_Mesure[va_NumUGTP].Validite6mn[vl_Zone] = XDC_VRAI;
      XZST_03EcritureTrace( XZSTC_INFO, "%d,%d Temps = %d, tdp:%d, Validite6mn:%d",va_NumUGTP,vl_Zone,vl_Temps,vg_Mesure[va_NumUGTP].TDP6mn[vl_Zone],vg_Mesure[va_NumUGTP].Validite6mn[vl_Zone]);
    }
    if ( (vl_NbParam == 1) &&
        (vl_Zone > 0) &&
        (vl_Zone <= ETDPC_NB_MAX_ZONE) &&
        (!strncmp (&vl_Trace[2], ETDPC_TDP_BLANC, strlen(ETDPC_TDP_BLANC))) &&
        (vg_DonneesUGTP[va_NumUGTP].Zone[vl_Zone].ChampValide == Valide) /*!!! &&
	   (vg_DonneesUGTP[va_NumUGTP].Zone[vl_Zone].AlarmeTrans == XDC_FAUX) &&
	   (vg_DonneesUGTP[va_NumUGTP].Zone[vl_Zone].AlarmeAntenne == XDC_FAUX) &&
	   (vg_DonneesUGTP[va_NumUGTP].Zone[vl_Zone].AlarmeITP == XDC_FAUX) */ &&
	   !(vg_DonneesUGTP[va_NumUGTP].Zone[vl_Index].Etat_Service) )
    {
      XZST_03EcritureTrace( XZSTC_WARNING, "Donnee 6mn : Donnee a blanc pour vl_zone : %d. EtatAlerte=%d;vg_ConfigTDP.NumAlerte=%d ",
          vl_Zone,vg_DonneesUGTP[va_NumUGTP].Zone[vl_Zone].EtatAlerte,vg_ConfigTDP.NumAlerte );
      vg_Mesure[va_NumUGTP].TDP6mn[vl_Zone] 	= 0;
      vg_Mesure[va_NumUGTP].Validite6mn[vl_Zone] = ETDPC_MES_INCONNU;
      if ( (vg_DonneesUGTP[va_NumUGTP].Zone[vl_Zone].EtatAlerte == XDC_FAUX) &&
          (vg_ConfigTDP.NumAlerte > 0) )
      {
        for ( pl_Zone = pg_ListeZone; pl_Zone != NULL; pl_Zone = pl_Zone->suivant )
          if ( pl_Zone->zdp.Numero == vl_Zone )
          {
            sprintf ( vl_TexteLib, "%s %s", vg_DonneesUGTP[va_NumUGTP].Zone[vl_Zone].Nom, pl_Zone->zdp.Libelle );
            break;
          }
        sprintf ( vl_TexteAlerte, vg_ConfigTDP.LibAlerte, vl_TexteLib );
        XZST_03EcritureTrace( XZSTC_WARNING, "Donnee 6mn : envoi alerte : %s",vl_TexteAlerte);
        ex_env_alerte  (    va_horodate,
            XDC_EQT_ZDP,
            vg_Mesure[va_NumUGTP].Numero[vl_Zone],
            vg_ConfigTDP.NumAlerte,
            XZEXC_MODE_NORMAL,
            XZSC_22NumeroSiteLocal(),
            vl_TexteAlerte,
            NULL );
      }
      vg_DonneesUGTP[va_NumUGTP].Zone[vl_Zone].EtatAlerte = XDC_VRAI;
    }
    vl_pas_pointeur += ETDPC_LONG_DONNEE_6MN;
  } /*fin while */

  /*traitement des voisins*/
  for ( pl_Zone = pg_ListeZone; pl_Zone != NULL; pl_Zone = pl_Zone->suivant )
  {
    if (pl_Zone->zdp.Maitre == vg_DonneesUGTP[va_NumUGTP].Numero)
    {
      if (vg_Mesure[va_NumUGTP].Validite6mn[pl_Zone->zdp.Code_Zone] == XDC_FAUX)
      {
        vg_Mesure[va_NumUGTP].TDP6mn[pl_Zone->zdp.Code_Zone]  = pl_Zone->zdp.TDP_Courant;
        vg_Mesure[va_NumUGTP].Validite6mn[pl_Zone->zdp.Code_Zone] = XDC_VRAI;
      }
    }
  }

  /* Arrondi aux 6 min pleines */
  vl_Result_Div = ldiv ( vl_Minutes, 6 );
  vg_DonneesUGTP[va_NumUGTP].Horodate6mn = va_horodate - ( vl_Result_Div.rem * 60 );
  et_Stockage_Mesures_6mn ( va_NumUGTP );
  et_Envoi_Mesure_6mn ( va_NumUGTP, XDC_FAUX );
  /* Appel fonction calcul des seuil */
  et_calcul_seuil_TDP ( va_NumUGTP );
  et_Envoi_Mesure_6mn_par_zone ( va_NumUGTP );

  /* Appel fonction rafraichissement PMV */
  /*!!!   et_Rafraichissment_PMV ( ); */

  vg_DonneesUGTP[va_NumUGTP].TrameMB[0] = '\0';

  return (XDC_OK);
}
