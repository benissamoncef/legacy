/*E*/
/*Fichier :  $Id: etdp_cyc.c,v 1.14 2021/01/06 08:44:59 akkaakka Exp $      Release : $Revision: 1.14 $        Date : $Date: 2021/01/06 08:44:59 $ ------------------------------------------------------
-------------------------------------------------------------------------------------------
* GTIE  *  PROJET MIGRAZUR 
-------------------------------------------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
-------------------------------------------------------------------------------------------
* MODULE TETDP * FICHIER etdp_cyc.c
-------------------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* C'est le module permetant les declenchements cycliques
* de la tache TETDP
*
-------------------------------------------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	13 Fev 1997	: Creation (DEM/1396)
* Mismer.D	version 1.2	25 Mar 1997	: Affichage TDP sur PMV (DEM/1417)
* Mismer.D	version 1.3	05 Jun 1997	: Ajout alerte sur panne zone (DEM/1447)
* Mismer.D      version 1.4     16 Oct 1997     : Modif pour affichage minutes pour TDP (DEM/1505)
* P.NIEPCERON	version 1.6	08 Jul 2009	: Ajout mise � l'heure DEM898 v1.6
* JPL		07/03/11 : Migration architecture HP ia64 (DEM 975) : retour C.R. OK (ey_envoi_demande_donnees_horaire)  1.7
* JMG		26/01/11 : pas de trame envoi heure sur UGTP virtuelle 1.8
* MG		04/06/12 : traitement cyclique du rafraichissement des destinations TDP (DEM/1014 PMA)
* PNI		16/09/13 : Suppression for�age du tdp radt au tdp normal et de son ecretage v1.10 DEM1082 1.13
* PNI    08/12/20 : time remplacer par XZSM_07 SAE_191
* LCL		20/12/21	: MOVIS P2 ems SAE-312
-----------------------------------------------------------------------------------------*/

/* fichiers inclus */

#include <unistd.h>

#include 	"etim_don.h"
#include 	"ex_mlcr.h"
#include	<xzao.h>
#include	<xzez.h>

#include	"etdp_don.h"
#include	"etdp_cfg.h"
#include	"etdp_cmd.h"
#include	"etdp_tra.h"
#include	"etdp_cyc.h"
#include	"etdp_pmv.h"
#include	"etdp_pma.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: etdp_cyc.c,v 1.14 2021/01/06 08:44:59 akkaakka Exp $      : etdp_cyc" ;

/* Declaration de fonctions internes */
/* --------------------------------- */

static int ey_traitement_cyclique_synchro_date( int, XZEXT_CLE_PROG, XDY_Horodate  );
static int ey_traitement_cyclique_demande_donnees ( XZEXT_CLE_PROG );

extern char                   vg_idpassw[18];



/*X*/
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU :
* 
*	Traitement des evenements relatifs aux TIMERS
*
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int ey_traitement_cyclique ( int va_NumSock )
								
/*
* ARGUMENTS EN ENTREE :
*
*	va_NumSock : Numero de la socket
*
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
*
*
* CONDITION D'UTILISATION
*
*	Appel lors de la reception d'un evenement TIMER sur la socket
*
* FONCTION
*
*	Traitement des evenements suivants:
*	   - L'envoi de l'heure a l'equipement 1 fois par jour
*	   - Une demande periodique declenchee suite a une demande de la fenetre LCR
*	   - Un reveil periodique pour connaitre l'etat de la liaison avec l'equipement
*	   - Un reveil periodique pour demander les donnees a l'equipement
*	   - Un reveil periodique pour une alarme liaison avec l'equipement Esclave
*
-----------------------------------------------------------------------------------------*/
{
   static char *version = "$Id: etdp_cyc.c,v 1.14 2021/01/06 08:44:59 akkaakka Exp $      : ey_traitement_cyclique" ;
   int vl_IndCnx = 0;
   int vl_LgMsg  = 0;
   XZEXT_MSG_SOCKET pl_Msg;
   XZEXT_MSG_SOCKET pl_MsgTimer;
   XDY_Horodate vl_Horodate;
   int vl_Delai;
   XZEXT_CLE_PROG pl_CleProg;
   int vl_noeqt;
   char vl_Cas = 0;
   
   /*A
    *  Lecture du message recu dans la socket appelante
    */
	 
   if( ex_lire_msg ( va_NumSock, pl_Msg, &vl_LgMsg, &vg_SockMask, &vl_IndCnx ) != XDC_OK )
   {
      /* Ecriture trace Erreur lecture message socket. */
      
      XZST_03EcritureTrace( XZSTC_WARNING, " Erreur lecture message socket. " );
   }   
   else
   {
      /* Si le message est correct et si ce n'est pas une deconnexion */
      
      if( ( vl_LgMsg != 0 ) && ( vl_IndCnx != XZEXC_DECONNEC ) )
      {
         /*A Si il s'agit d'un reveil cyclique */
         
  	 if ( strstr ( pl_Msg, XZEXC_MSG_REVEIL ) != NULL )
  	 {
  	    sscanf ( &pl_Msg[strlen(XZEXC_MSG_REVEIL)], " %lf %d %s ",
  	  					&vl_Horodate,&vl_Delai, pl_CleProg );
  	  		
            /* Demande periodique de la fenetre LCR */
            
  	    if (strstr(pl_CleProg,ETDPC_TIMER_FEN_LCR)!=NULL)
  	    { 
  	       sscanf(&pl_CleProg[strlen(ETDPC_TIMER_FEN_LCR)],
  	        					"%c%d",&vl_Cas,&vl_noeqt);
               ex_relance_lcr(vl_noeqt,vl_Cas,&pg_DebutListeTDPFLcr,
			 			vl_Horodate,pl_CleProg,va_NumSock );
               return(XDC_OK);
            }
            
            /*A Si il s'agit d'un reveil pour une demande d'envoi de donnees */
            
            if( ey_traitement_cyclique_demande_donnees(pl_CleProg)==XDC_OK)
            {
               return(XDC_OK);
            }
	  
            
	    /*A Si il s'agit d'un reveil pour synchro de la date */
           
            if( ey_traitement_cyclique_synchro_date(va_NumSock, pl_CleProg, vl_Horodate)==XDC_OK)
            {
               return(XDC_OK);
            }            
         }
      }
   }
   return(XDC_NOK);
}

   
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU :
* 
*	Traitement des evenements relatifs a la mise a l'heure �quipement
*
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static int ey_traitement_cyclique_synchro_date(  int va_NumSock,
				      XZEXT_CLE_PROG pa_CleProg, XDY_Horodate va_Horodate  )


/*
* ARGUMENTS EN ENTREE :
*
*	va_NumSock : Numero de la socket
*	pa_CleProg : Cle d'appel timer demande donnees
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
*
*	XDC_OK : Si la fonction est consernee
*	XDC_NOK : Si la fonction n'est pas consernee
*
* CONDITION D'UTILISATION
*
*	Appel lors de la reception d'un evenement TIMER sur la socket
*
* FONCTION
*
*	Traitement de l' evenements suivant:
*	   - Un reveil periodique pour tester la ligne esclave avec les equipements
*
-----------------------------------------------------------------------------------------*/
{
   static char *version = "$Id: etdp_cyc.c,v 1.14 2021/01/06 08:44:59 akkaakka Exp $      : ey_traitement_cyclique_synchro_date" ;
   XZSMT_Horodate vl_Horodatedecompo;
   XZEXT_Trame		vl_tlcr;
   XZEXT_MSG_SOCKET pl_MsgTimer;
   int i;
   
   /*A Si il s'agit d'un reveil pour synchro de la date */
           
   if( strstr( pa_CleProg, ETDPC_TIMER_SYNCHRO ) != NULL)
   {
  	       
      XZSM_11ConversionHorodate ( va_Horodate, &vl_Horodatedecompo );
                          
      /* passage de l'annee sur 2 digit */
      memcpy(&vl_Horodatedecompo[6],&vl_Horodatedecompo[8],12);
             
      /*B Composition de la trame mise a la date et a l'heure */
      sprintf ( vl_tlcr, "DATE ID=%s %s", vg_idpassw, vl_Horodatedecompo);
             
      XZST_03EcritureTrace( XZSTC_DEBUG1, "trame de date:%s", vl_tlcr );
      for( i=0; i!=ETDPC_NB_MAX_UGTP; i++)
      {
         if( !(vg_DonneesUGTP[i].Etat_Service & XDC_EQT_HORS_SRV) && 
	      (vg_DonneesUGTP[i].NumLS >0) &&
              (vg_DonneesUGTP[i].ChampValide == Valide) )
  	 {
            /*B Envoi trame  date et heure a l'equipement */
            EnvoyerTrame ( 	vl_tlcr, 
            			XZEXC_TRAME_DATE,
                       		vg_DonneesUGTP[i].AdresseRGS,
                       		XZEXC_NOCMD,
	                 	XZEXC_PRIORITEFAIBLE,
	                    	vg_DonneesUGTP[i].SocketLS );
XZST_03EcritureTrace( XZSTC_WARNING, "Mise � l'heure : %s %s",vg_DonneesUGTP[i].AdresseRGS,vl_tlcr);
         }
      }

      /* Rearmement du reveil synchro date */
      ETIM_SYNCHRO_DATE ( XZEXC_HEURE_SYNCHRO, ETDPC_TIMER_SYNCHRO, va_NumSock, pl_MsgTimer )
      return( XDC_OK);
   }
   return( XDC_NOK);
}
      
/*X*/
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU :
* 
*	Traitement des evenements relatifs aux demande de donn�es �quipement
*
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static int ey_traitement_cyclique_demande_donnees ( 	XZEXT_CLE_PROG pa_CleProg )

/*
* ARGUMENTS EN ENTREE :
*
*	pa_CleProg : Cle d'appel timer demande donnees
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
*
*	XDC_OK : Si la fonction est consernee
*	XDC_NOK : Si la fonction n'est pas consernee
*
* CONDITION D'UTILISATION
*
*	Appel lors de la reception d'un evenement TIMER sur la socket
*
* FONCTION
*
*	Traitement des evenements suivants:
*	   - Un reveil periodique pour demander les donnees a l'equipement
*
-----------------------------------------------------------------------------------------*/
{
   static char *version = "$Id: etdp_cyc.c,v 1.14 2021/01/06 08:44:59 akkaakka Exp $      : ey_traitement_cyclique_demande_donnees" ;
   int i,vl_Index;
   int 			vl_jourSemaine;
   XDY_Horodate		vl_Horodate;
   XZSMT_Horodate       pl_Horodatedecompo;
   XZEZT_Liste_Zdp *    pl_Zone;
   XDY_Entier   vl_TdP;

   /*A Si il s'agit d'un reveil pour une demande d'envoi de donnees */
            
/*   if( strstr( pa_CleProg, ETDPC_TIMER_1H ) != NULL)
   {
      for( i=0; i!=ETDPC_NB_MAX_UGTP; i++)
      {
         if( (vg_DonneesUGTP[i].ChampValide == Valide) &&
 	     (vg_DonneesUGTP[i].NumLS > 0) &&
	    !(vg_DonneesUGTP[i].Etat_Service & XDC_EQT_HORS_SRV) )
  	 {
*/
            /*B Appel fonction envoi demande heure */
/*
            vg_DonneesUGTP[i].Nb_Heure_Histo = 0;
            ey_envoi_demande_donnees_horaire ( i );
         }
         if( vg_DonneesUGTP[i].Etat_Service & XDC_EQT_HORS_SRV )
         {
            XZSM_07LireHorodate ( XZSMC_TYPE_SYSTEME, &vl_jourSemaine, &vl_Horodate );
            XZSM_11ConversionHorodate ( vl_Horodate, &pl_Horodatedecompo );
            sprintf ( &pl_Horodatedecompo[ETDPC_DIXMINUTE_STRING], "00:01" );
            XZSM_13ConversionHorodateSec(pl_Horodatedecompo,&vg_DonneesUGTP[i].Horodate_Histo);
            vg_DonneesUGTP[i].Nb_Heure_Histo = 0;
            et_Maj_Fichier_heure_TDP_1H ( i );
         }
      }
      lancement_timer_etdp( ETDPC_REVEIL_1H, ETDPC_TIMER_1H );
      return(XDC_OK);
   }
*/
   if( strstr( pa_CleProg, ETDPC_TIMER_6MN_RAF ) != NULL)
   {
      XZST_03EcritureTrace( XZSTC_DEBUG1, "Envoi rafress" );
   /* Appel fonction rafraichissement PMV */
      et_Rafraichissment_PMV ( );
/***********************************************/
  /* Appel fonction rafraichissement PMVA */
	 et_Rafraichissment_PMVA ( );
/***********************************************/
      return(XDC_OK);
   }
   if( strstr( pa_CleProg, ETDPC_TIMER_6MN ) != NULL)
   {
      XZST_03EcritureTrace( XZSTC_DEBUG1, "Envoi demande trame donnees 6mn" );
      for( i=0; i!=ETDPC_NB_MAX_UGTP; i++)
      {
	/* if ( vg_DonneesUGTP[i].NumLS > 0 )*/
                /* c'est une UGTP reelle */
             /*   {*/
         if( (vg_DonneesUGTP[i].ChampValide == Valide) &&
	    !(vg_DonneesUGTP[i].Etat_Service & XDC_EQT_HORS_SRV) )
  	 {
            /*B Envoi trame demande donnees a l'equipement */
            EnvoyerTrame( 	"MB", 
            			XZEXC_TRAME_DONNEES_TDP_6MN,
                    		vg_DonneesUGTP[i].AdresseRGS,
                       		XZEXC_NOCMD, 
                       		XZEXC_PRIORITEFAIBLE,
	               	 	vg_DonneesUGTP[i].SocketLS );
         }
 	/*}
	else {*/
                        /* reinitialisation de la liste des zones */
                        for ( vl_Index = 0; vl_Index <= ETDPC_NB_MAX_ZONE; vl_Index++ )
                                if ( vg_DonneesUGTP[i].Zone[vl_Index].ChampValide == Valide )
                                {
                                        vg_Mesure[i].TDP6mnRADT[vl_Index] = 0;
                                        vg_Mesure[i].Validite6mnRADT[vl_Index] = XDC_FAUX;
                                }

                       /* Recherche du temps de parcours de chaque zone de la liste */
                        for ( pl_Zone = pg_ListeZone; pl_Zone != NULL; pl_Zone = pl_Zone->suivant )
                                if ( (pl_Zone->zdp.Maitre == vg_DonneesUGTP[i].Numero) && (pl_Zone->zdp.Type_Zone == 1) )
                                {
                                        XZAS100_TdP_RADT ( pl_Zone->zdp.Autoroute, pl_Zone->zdp.PR_Debut, pl_Zone->zdp.PR_Fin, pl_Zone->zdp.Sens, &vl_TdP );

                                        /* ajustement temps de parcours en fonction du retour de XZAS100 */
                                        if ( vl_TdP <= 0 )
                                                /* TdP invalide */
                                                vl_TdP = pl_Zone->zdp.TDP_Normal;
                                        /*else if ( vl_TdP > pl_Zone->zdp.TDP_Max ) suppression ecretage */
                                                /* TdP Max */
                                                /*vl_TdP = pl_Zone->zdp.TDP_Max;*/
                                       /*else
                                                Suprresion du forcage du tdp normal a la place du calcul RADT 
                                                vl_TdP = pl_Zone->zdp.TDP_Normal;*/
                                        vg_DonneesUGTP[i].Zone[pl_Zone->zdp.Code_Zone].EtatAlerte = XDC_FAUX;
                                        vg_Mesure[i].TDP6mnRADT[pl_Zone->zdp.Code_Zone] = vl_TdP;
                                        vg_Mesure[i].Validite6mnRADT[pl_Zone->zdp.Code_Zone] = XDC_VRAI;
				}

                        /* Calculer l'horodate */
            XZSM_07LireHorodate ( XZSMC_TYPE_SYSTEME, &vl_jourSemaine, &vl_Horodate );
            XZSM_11ConversionHorodate ( vl_Horodate, &pl_Horodatedecompo );
            sprintf ( &pl_Horodatedecompo[ETDPC_DIXMINUTE_STRING], "00:01" );
            XZSM_13ConversionHorodateSec(pl_Horodatedecompo,&vg_DonneesUGTP[i].Horodate6mn);

                        /* stockage et envoi des mesures */
                  /*      et_Stockage_Mesures_6mn ( i );
                        et_Envoi_Mesure_6mn( i, XDC_FAUX );*/

                        /* Appel fonction calcul des seuils */
                    /*    et_calcul_seuil_TDP( i );
                        et_Envoi_Mesure_6mn_par_zone( i );*/

/*	}*/
      }
      lancement_timer_etdp( ETDPC_REVEIL_6MN, ETDPC_TIMER_6MN );
      return(XDC_OK);
   }
   return(XDC_NOK);
}


/*X*/
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU :
* 
*	Traitement des evenements relatifs aux demande de mesure horaire
*
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int ey_envoi_demande_donnees_horaire ( XDY_Eqt va_NumUGTP )

/*
* ARGUMENTS EN ENTREE :
*
*	va_NumUGTP : numero d'UGTP
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
*
*
* CONDITION D'UTILISATION
*
*
* FONCTION
*
*
-----------------------------------------------------------------------------------------*/
{
   static char *version = "$Id: etdp_cyc.c,v 1.14 2021/01/06 08:44:59 akkaakka Exp $      : ey_envoi_demande_donnees_horaire ";

   XDY_Horodate  	vl_horodateSec = .0;
   XDY_Horodate  	vl_HeureDem 	= .0;
   int		 	vl_DeltaHeure	=  0;
   // time_t		vl_time	= 0;
   int            vl_iJourSemaine = 0 ; // plus utilis� retourne 2... 
   XZSMT_Horodate	pl_Horo_Txt = "";
   char			pl_TrameRequete[8] = "";
   
   /*A Recherche de l'horodate systeme */								
   /*time ( &vl_time );
   vl_horodateSec = (XDY_Horodate) vl_time;  remplacer par XZSM_07 XZAE_191*/
   XZSM_07LireHorodate( XZSMC_TYPE_SYSTEME, &vl_iJourSemaine , &vl_horodateSec );
   vl_HeureDem = 	vl_horodateSec - 
   			vg_DonneesUGTP[va_NumUGTP].Horodate_Histo - 
   			(vg_DonneesUGTP[va_NumUGTP].Nb_Heure_Histo*3600);
   
   if ( vl_HeureDem < 0 ) return(XDC_OK);
   
   if ( vl_HeureDem > (3600*23) )
   {
      vl_HeureDem = vl_horodateSec - (3600*23);
      XZSM_11ConversionHorodate ( vl_HeureDem, &pl_Horo_Txt );
      strcpy ( &pl_Horo_Txt[ETDPC_DIXMINUTE_STRING], "00:01" );
      XZSM_13ConversionHorodateSec(pl_Horo_Txt,&vl_HeureDem);
      vg_DonneesUGTP[va_NumUGTP].Horodate_Histo = vl_HeureDem;
   }
   else
   {
      vl_HeureDem = vg_DonneesUGTP[va_NumUGTP].Horodate_Histo -  (vg_DonneesUGTP[va_NumUGTP].Nb_Heure_Histo*3600);
   }
   
      
   XZSM_11ConversionHorodate ( vl_HeureDem, &pl_Horo_Txt );
   XZST_03EcritureTrace( XZSTC_INFO, "heure demande <%s>", pl_Horo_Txt );
   vl_DeltaHeure = atoi ( &pl_Horo_Txt[ETDPC_HEURE_STRING] );

   sprintf ( pl_TrameRequete, "MH%02d", vl_DeltaHeure );   
   /*B Envoi trame demande donnees a l'equipement */
   XZST_03EcritureTrace( XZSTC_WARNING, "Envoi demande trame donnees 1H <%s>", pl_TrameRequete );
   EnvoyerTrame( 	pl_TrameRequete, 
            		XZEXC_TRAME_DONNEES_TDP_HORAIRE,
                    	vg_DonneesUGTP[va_NumUGTP].AdresseRGS,
                       	XZEXC_NOCMD, 
                       	XZEXC_PRIORITEFAIBLE,
	               	vg_DonneesUGTP[va_NumUGTP].SocketLS );
   
   vg_DonneesUGTP[va_NumUGTP].Nb_Heure_Histo++;

   return (XDC_OK);
}
