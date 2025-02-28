/*E*/
/* Fichier : $Id: etdp_cfg.c,v 1.18 2021/06/14 17:23:20 devgfi Exp $        $Revision: 1.18 $       $Date: 2021/06/14 17:23:20 $
-------------------------------------------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
-------------------------------------------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
-------------------------------------------------------------------------------------------
* MODULE TETDP * FICHIER etdp_cfg.c
-------------------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module d'init et de reinit, de controle et d'exit de la tache TETDP
*
-------------------------------------------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	13 Fev 1997	: Creation (DEM/1396)
* Mismer.D	version 1.2	25 Mar 1997	: Affichage TDP sur PMV (DEM/1417)
* Mismer.D	version 1.3	28 Mai 1997	: Affichage TDP sur PMV Suite (DEM/1417)
* Mismer.D	version 1.4	05 Jun 1997	: Ajout zone statique (DEM/1441)
* JMG		02/10/08	: ajout indice et nombre dans fct util DEM/837
* PNI		08/07/09	: Appel de ex_lire_idpassw a la place de ec_lire_idpassw DEM898 v1.6
* JPL		07/03/11 : Migration architecture HP ia64 (DEM 975) : suppression warnings mineurs  1.7
* JPL		07/03/11 : Migration architecture HP ia64 (DEM 975) : format champs de 'printf'  1.8
* VR		18/11/11 : Ajout Destination Noeud v1.9 (DEM/1014)
* MG		29/05/12 : Ajout TDP PMVA (DEM/1014 PMA)
* SFO           06/10/14 : Adaptation pour compilation sous linux
* JPL		06/10/17 : Restauration numeros de version des fonctions  1.14
* JPL		06/10/17 : Liste destinations PMV[A]: eqt., destination, code zdp sur 2 octets; xzao4001 remplace xzao400 (DEM 1236) 1.15
* JMG		12/11/19 ; surcharge TDP DEM1365 1.16
* GR            09/07/2020 : EMS 1.17
* JPL		02/06/21 : Initialisation complete des donnees PMV et PMVA (SAE-286)  1.18
* LCL		20/12/21	: MOVIS P2 ems SAE-312
* LCL		04/10/2023	Adaptation CNA DEM-483
-----------------------------------------------------------------------------------------*/

/* Fichiers inclus */
/* --------------- */
#include <xzsem.h>
#include	<xdy.h>
#include	<xdc.h>
#include	<xzst.h>
#include	<xzat.h>
#include	<xzao.h>
#include	<xzez.h>
#include	<xzit.h>
#include	<xzad05.h>
#include	<xzad07.h>
#include	<xzad08.h>
#include	<xzad17.h>
#include	<xzad20.h>

#include 	"ex_msok.h"
#include 	"ex_mlcr.h"
#include 	"ex_mrtf.h"

#include	"etdp_don.h"
#include	"etdp_dir.h"
#include	"etdp_cfg.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* Declaration de variables locales */
/* -------------------------------- */

static char *version = "$Id: etdp_cfg.c,v 1.18 2021/06/14 17:23:20 devgfi Exp $ : etdp_cfg" ;

ETDP_DONNEES_UGTP 		vg_DonneesUGTP[ETDPC_NB_MAX_UGTP];
ETDP_DONNEES_CONFIG_TACHE	vg_ConfigTDP;
XZEZT_Liste_Zdp *		pg_ListeZone = NULL;
XZEZT_Liste_Destinations *	pg_ListeDest = NULL;
ETDP_CONFIG_PMV *		pg_ListePMV  = NULL;
ETDP_CONFIG_PMVA *		pg_ListePMVA  = NULL;
char                  vg_idpassw[18] = "";
XDY_District		vg_site;

/* declaration de fonctions internes */

/* Definition de fonctions externes */
/* -------------------------------- */

static int lecture_fichier_config_UGTP( char *);
static int lecture_fichier_config_zone (char *);
static int lecture_fichier_param_UGTP (char *);
static int lecture_config_en_base( void );
void ec_sortir( void);
static int ec_creer_liste_Destination_pmv ( XDY_Eqt , XDY_Mot , XDY_Octet , XDY_Mot , int             );
static int ec_creer_liste_Destination_pmva ( XDY_Eqt, XDY_Mot, XDY_Octet, XDY_Mot, int );


void et_init_surcharge (
        T_IPC_CONN                      va_Cnx,
      T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
       T_PTR                           pa_ARG )
{

 #ifndef _TIBCO_EMS  
  T_STR	vl_poste;
#else
  XDY_NomEqt vl_poste;
#endif

   XZEZT_Liste_Zdp *     pl_Zone = NULL;

   /* Recuperation des parametres du message XDM_VAL_TDP */
   if ( !TipcMsgRead(pa_Data -> msg,
                        T_IPC_FT_STR,  &vl_poste,
                        NULL) )
   {
      XZST_03EcritureTrace ( XZSTC_WARNING," Impossible de lire le XDM_ETDP_INIT_SURCHARGE dans et_init_surcharge@");
        return;
   }
   else
   {

      XZST_03EcritureTrace ( XZSTC_FONCTION,"IN : et_init_surcharge   poste %s",
                                                         vl_poste);
   }
   for ( pl_Zone = pg_ListeZone; pl_Zone != NULL; pl_Zone = pl_Zone->suivant ) {
		if (!TipcSrvMsgWrite(vl_poste,
			TipcMtLookupByNum(XDM_ETDP_ETAT_SURCHARGE),
			FALSE,
			T_IPC_FT_INT2, pl_Zone->zdp.Numero,
			T_IPC_FT_INT4, pl_Zone->zdp.TDP_Courant,
			T_IPC_FT_INT4, pl_Zone->zdp.TDP_Surcharge,
			NULL))
			XZST_03EcritureTrace(XZSTC_WARNING,"Impossible d'envoyer XDM_ETDP_ETAT_SURCHARGE");
		else  {
   			TipcSrvFlush();
			XZST_03EcritureTrace(XZSTC_FONCTION,"XDM_ETDP_ETAT_SURCHARGE envoye sur %s (%d/%d/%d/%s)", vl_poste,
					pl_Zone->zdp.Numero,
					pl_Zone->zdp.TDP_Courant,
					pl_Zone->zdp.TDP_Surcharge,
					pl_Zone->zdp.Libelle);
   		}
   }

}




void et_surcharge (
        T_IPC_CONN                      va_Cnx,
      T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
       T_PTR                           pa_ARG )
{
#ifdef _TIBCO_EMS
   XDY_Mot               vl_zone;
   XDY_Entier               vl_surcharge;
   XDY_Entier	vl_numero;
   XDY_Mot	vl_cle;
#else
   T_INT2               vl_zone;
   T_INT4               vl_surcharge;
   T_INT4	vl_numero;
   T_INT2	vl_cle;
#endif
   XZEZT_Liste_Zdp *     pl_Zone = NULL;
   XZITT_Destinataire      vl_Destinataire ;
   XDY_Evt         vl_Evt_In;
   XDY_Datagroup vl_dg;

     XDG_EncodeDG2(vl_dg , XDG_IETAT_DISPO_EQT, vg_ConfigTDP.NomSite );

   /* Recuperation des parametres du message XDM_VAL_TDP */
   if ( !TipcMsgRead(pa_Data -> msg,
                        T_IPC_FT_INT2,  &vl_zone,
                        T_IPC_FT_INT4,  &vl_surcharge,
                        T_IPC_FT_INT4,  &vl_numero,
                        T_IPC_FT_INT2,  &vl_cle,
                        NULL) )
   {
      XZST_03EcritureTrace ( XZSTC_WARNING," Impossible de lire le XDM_ETDP_SURCHARGE dans et_surcharge@");
        return;
   }
   else
   {

      XZST_03EcritureTrace ( XZSTC_FONCTION,"IN : et_surcharge   zone %d surcharge %d fmc %d/%d",
                                         vl_zone, vl_surcharge, vl_numero,vl_cle);
   }

   /* pas de zone, on est dans le cas de fin d'une FMC*/
   /* donc on supprime la surcharge sur toutes les zones liees*/
   if (vl_zone==0)  {
     for ( pl_Zone = pg_ListeZone; pl_Zone != NULL; pl_Zone = pl_Zone->suivant ) {
            if ( pl_Zone->zdp.Numero_Surcharge == vl_numero &&
	         pl_Zone->zdp.Cle_Surcharge == vl_cle) {
		pl_Zone->zdp.TDP_Surcharge =0;
		 pl_Zone->zdp.Numero_Surcharge =0;
		 pl_Zone->zdp.Cle_Surcharge =0;
		if (!TipcSrvMsgWrite(vl_dg,
			TipcMtLookupByNum(XDM_ETDP_ETAT_SURCHARGE),
			FALSE,
			T_IPC_FT_INT2, pl_Zone->zdp.Numero,
			T_IPC_FT_INT4, pl_Zone->zdp.TDP_Courant,
			T_IPC_FT_INT4, pl_Zone->zdp.TDP_Surcharge,
			NULL))
			XZST_03EcritureTrace(XZSTC_WARNING,"Impossible d'envoyer XDM_ETDP_ETAT_SURCHARGE");
		else  {
   			TipcSrvFlush();
			XZST_03EcritureTrace(XZSTC_FONCTION,"XDM_ETDP_ETAT_SURCHARGE envoye sur %s (%d/%d/%d/%s)", vl_dg,
					pl_Zone->zdp.Numero,
					pl_Zone->zdp.TDP_Courant,
					pl_Zone->zdp.TDP_Surcharge,
					pl_Zone->zdp.Libelle);
   		}
            }
     }
    return;
   }

   /* demande de reveil FMC pour une FMC si zone en surcharge*/
   if ((vl_zone==-1) && ((vg_site==XDC_CI) || (vg_site==XDC_CA))) {
     for ( pl_Zone = pg_ListeZone; pl_Zone != NULL; pl_Zone = pl_Zone->suivant ) {
	if ( pl_Zone->zdp.Numero_Surcharge == vl_numero &&
		pl_Zone->zdp.Cle_Surcharge == vl_cle) {
		XZAD20_Reveil_Operateur(vl_numero,vl_cle);
		return;
     	}
     }
     return;
   }

   /*fin de surcharge pour une zone donc on oublie la FMC associee*/
   if (vl_surcharge==0) {
	vl_numero=0;
	vl_cle=0;
   }

   for ( pl_Zone = pg_ListeZone; pl_Zone != NULL; pl_Zone = pl_Zone->suivant ) {
            if ( pl_Zone->zdp.Numero == vl_zone ) {
		pl_Zone->zdp.TDP_Surcharge = vl_surcharge;
		pl_Zone->zdp.Numero_Surcharge = vl_numero;
		pl_Zone->zdp.Cle_Surcharge = vl_cle;
		XZST_03EcritureTrace(XZSTC_FONCTION,"surcharge zone %d pour TDP %d pour fmc %d/%d",
				pl_Zone->zdp.Numero,
				pl_Zone->zdp.TDP_Surcharge,
				pl_Zone->zdp.Numero_Surcharge,
				pl_Zone->zdp.Cle_Surcharge);
		if (!TipcSrvMsgWrite(vl_dg,
			TipcMtLookupByNum(XDM_ETDP_ETAT_SURCHARGE),
			FALSE,
			T_IPC_FT_INT2, pl_Zone->zdp.Numero,
			T_IPC_FT_INT4, pl_Zone->zdp.TDP_Courant,
			T_IPC_FT_INT4, pl_Zone->zdp.TDP_Surcharge,
			NULL))
			XZST_03EcritureTrace(XZSTC_WARNING,"Impossible d'envoyer XDM_ETDP_ETAT_SURCHARGE");
		else  {
   			TipcSrvFlush();
			XZST_03EcritureTrace(XZSTC_FONCTION,"XDM_ETDP_ETAT_SURCHARGE envoye sur %s (%d/%d/%d/%s)", vl_dg,
					pl_Zone->zdp.Numero,
					pl_Zone->zdp.TDP_Courant,
					pl_Zone->zdp.TDP_Surcharge,
					pl_Zone->zdp.Libelle);
   		}
		return;
             }
   }
}



/*X*/
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU : 
*  Permet selon message socket recu de tesrv 
*    - soit de terminer la tache
*    - soit de configurer la tache
*
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void ec_controle( int va_NumSock, XDY_NomMachine pa_NomMachine)

/*
* ARGUMENTS EN ENTREE :
*
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
-----------------------------------------------------------------------------------------*/
{
   static char *version = "$Id: etdp_cfg.c,v 1.18 2021/06/14 17:23:20 devgfi Exp $ : ec_controle" ;
   int vl_IndCnx = 0;
   int vl_LgMsg  = 0;
   XZEXT_MSG_SOCKET pl_Msg;
   
   /*A
    *  Lecture du message recu dans la socket appelante
    */
	 
   if ( ex_lire_msg ( va_NumSock, pl_Msg, &vl_LgMsg, &vg_SockMask, &vl_IndCnx ) != XDC_OK )
   {
      /* Ecriture trace Erreur lecture message socket. */
      XZST_03EcritureTrace( XZSTC_WARNING, " Erreur lecture message socket. " );
   }   
   else
   {
      /*A Si il s'agit d'une deconnexion */
      if ( vl_IndCnx == XZEXC_DECONNEC )
      {
         /*A Arreter la tache par appel de ec_sortir */
         ec_sortir();
      }
   
      /* Si le message est correct  */
      if( vl_LgMsg != 0 ) 
      {
  	  /*A Si il s'agit d'une demande d'arret */
  	  if( strstr ( pl_Msg, XZEXC_DEMANDE_ARRET ) != NULL )
  	  {
  	     /*A Arreter la tache par appel de ec_sortir */
  	     ec_sortir();
  	  }

  	  /*A Si il s'agit d'une demande d'Init */
  	  if( strstr ( pl_Msg, XZEXC_DEMANDE_INIT ) != NULL )
  	  {
  	     /*A Reconfiguration de la tache par appel de ec_init */
  	     ec_init ( pa_NomMachine );
  	  }
  	  /*A Si il s'agit du changement du mode de fonctionnement */
  	  if( strstr ( pl_Msg, XZEXC_MODE_FONCT_NORMAL ) != NULL )
  	  {
  	     /*A Maj de la variable en question */
  	     vg_ConfigTDP.Mode_Fonct = XZEXC_MODE_NORMAL;
  	     ex_RecopieFichierMsg( pa_NomMachine);
  	  }
  	  if( strstr( pl_Msg, XZEXC_MODE_FONCT_ISOLE ) != NULL )
  	  {
  	     /*A Maj de la variable en question */
  	     vg_ConfigTDP.Mode_Fonct = XZEXC_MODE_ISOLE;
  	  }
       }
    }
}

/*X*/
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU : 
*  Effectue l'initialisation de la tache
*
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int ec_init( XDY_NomMachine pa_NomMachine )

/*
* ARGUMENTS EN ENTREE :
*
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
*   Appel de ex_mini
*   Lecture des fichier de configuration des DAi et des Analyseur
*   Remplissage des structures de donnees
*   Connexion socket avec la tache protocole
*
-----------------------------------------------------------------------------------------*/
{
   static char *version = "$Id: etdp_cfg.c,v 1.18 2021/06/14 17:23:20 devgfi Exp $ : ec_init" ;
   char vl_nom_fichier[256];
   XZEXT_MASK *pl_Socket;
   XZEXT_ADR_SOCK pl_TacheTrt;
   XZEXT_ADR_SOCK pl_TacheProt;
   int vl_SockProt = 0;
   int i, vl_Index;
   int				vl_retcode = 0; 

   /* Boucle pour la RAZ de la table de donnees */
   vg_site = XZSC_22NumeroSiteLocal();

   for(i=0 ;i!=ETDPC_NB_MAX_UGTP; i++)
   {
      /* Invalidation des donnees */
      vg_DonneesUGTP[i].ChampValide=Non_valide;
      for ( vl_Index = 0; vl_Index <= ETDPC_NB_MAX_ZONE; vl_Index++ )
      {  
         vg_DonneesUGTP[i].Zone[vl_Index].Etat_Service = 0;
         vg_DonneesUGTP[i].Zone[vl_Index].ChampValide=Non_valide;
      }
   }
   vg_ConfigTDP.DelaiDdeHoraire = 2;
   vg_ConfigTDP.NumAlerte = 0;
   vg_ConfigTDP.LibAlerte[0] = '\0';

   /* Lecture des fichiers de configuration et remplissage des structures de donnees */
   
   sprintf( vl_nom_fichier, "%s/%s_%s", XZEXC_PATH_CONFIG,
				XDF_Config_TDP, vg_ConfigTDP.NomMachine);
   
   /* Si il y a probleme de configuration */
   
   if( lecture_fichier_config_UGTP( vl_nom_fichier) == XDC_NOK)
   {
      /* Alors */
      /* Retourner NOK */
      
      return(XDC_NOK);
   }

   /* Si il y a probleme de configuration  zone*/
   sprintf( vl_nom_fichier, "%s/%s", XZEXC_PATH_CONFIG, XDF_CONF_ZONE_UGTP );
   if( lecture_fichier_config_zone( vl_nom_fichier ) == XDC_NOK)
   {
      /* Alors */
      /* Retourner NOK */
      
      return(XDC_NOK);
   }
   
   sprintf( vl_nom_fichier, "%s/%s", XZEXC_PATH_CONFIG, XDF_PARAM_UGTP );
   lecture_fichier_param_UGTP ( vl_nom_fichier );
   
   if( lecture_config_en_base ( ) == XDC_NOK)
   {
      /* Alors */
      /* Retourner NOK */
      
      return(XDC_NOK);
   }
   
   /*A Lecture du password de la machine (user/password) */
   if ( ( vl_retcode = ex_lire_idpassw ( pa_NomMachine, vg_idpassw ) ) != XDC_OK )
   {
   	XZST_03EcritureTrace(XZSTC_WARNING,"%s: ex_lire_idpassw retourne %d", version, vl_retcode);
	return (XDC_NOK);
   }

   /*A Ouverture du fichier d'alerte */
   ex_OuvertureFichierMsg(vg_ConfigTDP.NomMachine);

   /* Appel xzat26_Init_Etats_Eqts pour Initialisation des etats equipement (HS ou Actif) */
   xzat26_Init_Etats_Eqts( XDC_EQT_TDP, XDC_ETAT_TOUS_EQT, vg_ConfigTDP.NomMachine );
   xzat26_Init_Etats_Eqts( XDC_EQT_ZDP, XDC_ETAT_TOUS_EQT, vg_ConfigTDP.NomMachine );
   
   /*A
    * Connexion socket en mode client pour communiquer avec la tache TELMi
    */
   /*A Rechercher si la tache protocole associee existe */
   
   for( i=0; i!=ETDPC_NB_MAX_UGTP; i++)
   {
      /* Si les donnees de la table sont valides */
      
      XZST_03EcritureTrace(XZSTC_FONCTION,"numLS %d, valide %d",
			vg_DonneesUGTP[i].NumLS,
			vg_DonneesUGTP[i].ChampValide);

      if (vg_DonneesUGTP[i].ChampValide == Valide && vg_DonneesUGTP[i].NumLS > 0 )
      {
         /* Alors */
         /* Construction du nom de la tache protocole telmi */
   
         sprintf( pl_TacheProt, "%s%d", XZEXC_ADR_SOCK_TELM, vg_DonneesUGTP[i].NumLS);
         sprintf( pl_TacheTrt, "%s%d", XZEXC_ADR_SOCK_TDPTELM, vg_DonneesUGTP[i].NumLS);
   
         /*A Recherche sur tout le masque des socket si la socket exite */
   
         for( pl_Socket = vg_SockMask; pl_Socket != NULL; pl_Socket = pl_Socket->Suivant )
         {
            if( !strcmp ( pl_Socket->adresse, pl_TacheProt ) )
            {
               /* Mise a jour du numero de socket */
         
               vg_DonneesUGTP[i].SocketLS = pl_Socket->desc_sock;
         
              /*B Arret: la socket avec la tache existe */
              break;
            }
         }
      
         /*A Si la socket avec la tache telmi n'existe pas */
         
         if( pl_Socket == NULL )
         {
            /*A Appel de ex_cnx_client pour etablir la liaison avec la tache protocole */
      
            if( ex_cnx_client(pl_TacheTrt, pl_TacheProt, &vg_SockMask, &vl_SockProt) != XDC_OK )
            {
	       /*B Ecriture Trace */
               XZST_03EcritureTrace( XZSTC_FATAL, "Impossible de creer la connexion client avec la tache %s .", pl_TacheProt );	
            }
            else
            {
	       /* Memorisation du numero de socket */
	       vg_DonneesUGTP[i].SocketLS = vl_SockProt;
	    }
	 }
      }
      /* Finsi */	
   }


   /*A
    * Demande de status pour init des etats d'alarme 
    */
   for( i=0; i!=ETDPC_NB_MAX_UGTP; i++)
   {
      /* Si les donnees de la table sont valides */
      
      if( vg_DonneesUGTP[i].ChampValide == Valide && vg_DonneesUGTP[i].NumLS > 0)
      {
	  EnvoyerTrame(	"ST",
	              	XZEXC_TRAME_STAT_COMP,
	                vg_DonneesUGTP[i].AdresseRGS,
	                XZEXC_NOCMD,
	                XZEXC_PRIORITEFAIBLE,
	                vg_DonneesUGTP[i].SocketLS );
      }
   }
   
   return (XDC_OK);
}

/*X*/
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU : 
*
*	Lit le fichier config des UGTP et remplit la structure associee
*
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static int lecture_fichier_config_UGTP(char *va_nom_fichier)

/*
* ARGUMENTS EN ENTREE :
*
*	va_nom_fichier : Nom du fichier config tdp
*   aucun
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
*
*
* CONDITION D'UTILISATION
*
* FONCTION 
*
*	Lit le fichier config TDP et remplit la structure associee
*
-----------------------------------------------------------------------------------------*/
{
   static char *version = "$Id: etdp_cfg.c,v 1.18 2021/06/14 17:23:20 devgfi Exp $ : lecture_fichier_config_UGTP" ;
   char vl_ligne_fichier[512];
   int vl_nb_elements_lus;
   int vl_Numero;
   int vl_NumLS;
   FILE *fp;
   int i=0;
   int vl_cr = XDC_OK;
   
   /* Si le fichier existe */
   
   if(( fp=fopen(va_nom_fichier, "r"))!=NULL)
   {
      /* Alors */
      /* Tant que le fichier n'est pas completement lu */
      
      while(fgets(vl_ligne_fichier, 512, fp)!=NULL)
      {
         /* Si Le caractere lu n'est pas un '#' */
         
         if(vl_ligne_fichier[0]!= XDF_COMMENTAIRE)
         {
            /* Alors */
            /* Si le nombre maximum d'equipements gerable par la tache atteint */
            
            if( i==ETDPC_NB_MAX_UGTP)
            {
               /* Alors */
               /* Tracer l'erreur et quitter */
               
               XZST_03EcritureTrace(XZSTC_WARNING,
                " Le nombre maximum d'equipements gerables par la tache TETDP est atteint");
              
               return(XDC_NOK);
            }
               
            /* Recuperer les elements de la ligne */
            
            vl_nb_elements_lus = sscanf ( vl_ligne_fichier, XZEXC_FORMAT_CONF_TDP,
               						&vl_Numero, 
                      					vg_DonneesUGTP[i].NomST,
                        				vg_DonneesUGTP[i].AdresseRGS, 
                        				&vl_NumLS );
		            
            /* Si nombre d'elements lus n'est pas OK */
            if( vl_nb_elements_lus!=4)
            {
               /* Alors */
               /* Tracer l'erreur et quitter */
               
               XZST_03EcritureTrace(XZSTC_WARNING,
                   " Recuperation des donnees fichier '%s' impossible.", va_nom_fichier);
              
               return(XDC_NOK);
            }
            /* Finsi */
            
            /* Mise dans le structure des datas lues */
            
            vg_DonneesUGTP[i].Numero = (XDY_Eqt )vl_Numero;
            vg_DonneesUGTP[i].NumLS  = (XDY_Mot )vl_NumLS;
            
            /* Validation des donnees lues */
            
            vg_DonneesUGTP[i].ChampValide=Valide;
            vg_DonneesUGTP[i].Etat_Service = 0;
            vg_DonneesUGTP[i].TrameMB[0] = '\0';
            
	    et_Lecture_Fichier_heure_TDP_1H ( i );
            
            XZST_03EcritureTrace(XZSTC_WARNING, " Config : num:%d LS:%d ST:%s RGS:%s", 
            				vg_DonneesUGTP[i].Numero,
            				vg_DonneesUGTP[i].NumLS,
            				vg_DonneesUGTP[i].NomST,
            				vg_DonneesUGTP[i].AdresseRGS );
            
            i++;
         }
         /* Finsi */
      }
      /* Fin Tant */
      fclose ( fp );
   }
   else
   {
      /* Sinon */
      
      XZST_03EcritureTrace(XZSTC_WARNING,
      				" Acces au fichier '%s' impossible.", va_nom_fichier);
      vl_cr = XDC_NOK;
   }
   /* Finsi */
   
   return (vl_cr);
}


/*X*/
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU : 
*
*	Lit le fichier config des Zones et remplit la structure associee
*
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static int lecture_fichier_config_zone (char *va_nom_fichier)

/*
* ARGUMENTS EN ENTREE :
*
*	va_nom_fichier : Nom du fichier config des zone de parcours
*   aucun
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
*
*
* CONDITION D'UTILISATION
*
* FONCTION 
*
*	Lit le fichier config zone de parcours et remplit la structure associee
*
-----------------------------------------------------------------------------------------*/
{
   static char *version = "$Id: etdp_cfg.c,v 1.18 2021/06/14 17:23:20 devgfi Exp $ : lecture_fichier_config_zone" ;
   char vl_ligne_fichier[512];
   char vl_NomMaitre[12] = "";
   int vl_nb_elements_lus;
   int vl_Numero;
   FILE *fp;
   int i=0;
   ETDP_INFO_ZONE vl_Zone;
   int vl_cr = XDC_OK;
   
   /* Si le fichier existe */
   
   if(( fp=fopen(va_nom_fichier, "r"))!=NULL)
   {
      /* Alors */
      /* Tant que le fichier n'est pas completement lu */
      
      while(fgets(vl_ligne_fichier, 512, fp)!=NULL)
      {
         /* Si Le caractere lu n'est pas un '#' */
         
         if(vl_ligne_fichier[0]!= XDF_COMMENTAIRE)
         {
            /* Alors */
               
            /* Recuperer les elements de la ligne */
            
            vl_nb_elements_lus = sscanf ( strtok(vl_ligne_fichier,"\n"), "%d\t%s\t%hd\t%hd\t%hd\t%hd\t%hd\t%hd\t",
               						&vl_Numero, 
                      					vl_NomMaitre,
                        				&vl_Zone.NumITPDeb,
                        				&vl_Zone.NumCptDeb,
                        				&vl_Zone.NumITPFin,
                        				&vl_Zone.NumCptFin,
                        				&vl_Zone.NumAntDeb,
                        				&vl_Zone.NumAntFin );
		            
            /* Si nombre d'elements lus n'est pas OK */
            if( vl_nb_elements_lus != 8 )
            {
               /* Alors */
               /* Tracer l'erreur et quitter */
               
               XZST_03EcritureTrace(XZSTC_WARNING,
                   " Recuperation des donnees fichier '%s' impossible.", va_nom_fichier);
              
            }
            else
            {
            
               /* Mise dans le structure des datas lues */
               if ( !strcmp(vg_ConfigTDP.NomMachine, vl_NomMaitre) )
               { 
                 for(i=0 ;((i!=ETDPC_NB_MAX_UGTP) && (vg_DonneesUGTP[i].ChampValide==Valide)); i++)
                 {
                    if ( vl_Numero <= ETDPC_NB_MAX_ZONE )
                    {
                       memcpy ( &vg_DonneesUGTP[i].Zone[vl_Numero], &vl_Zone, sizeof(ETDP_INFO_ZONE) );
                       vg_DonneesUGTP[i].Zone[vl_Numero].Etat_Service = 0;
                       vg_DonneesUGTP[i].Zone[vl_Numero].ChampValide  = Non_valide;
                       vg_DonneesUGTP[i].Zone[vl_Numero].EtatAlerte   = XDC_FAUX;
                       vg_DonneesUGTP[i].Zone[vl_Numero].EtatPanne    = XDC_FAUX;
                        
                       XZST_03EcritureTrace(XZSTC_WARNING, " Config : zone:%d itpdeb:%d cdeb:%d itpfin:%d cfin:%d adeb:%d afin:%d", 
            				vl_Numero,
            				vg_DonneesUGTP[i].Zone[vl_Numero].NumITPDeb,
            				vg_DonneesUGTP[i].Zone[vl_Numero].NumCptDeb,
            				vg_DonneesUGTP[i].Zone[vl_Numero].NumITPFin,
            				vg_DonneesUGTP[i].Zone[vl_Numero].NumCptFin,
            				vg_DonneesUGTP[i].Zone[vl_Numero].NumAntDeb,
            				vg_DonneesUGTP[i].Zone[vl_Numero].NumAntFin  );
                    }
                  }
               }
            
            }
         }
         /* Finsi */
      }
      /* Fin Tant */
      fclose ( fp );
   }
   else
   {
      /* Sinon */
      
      XZST_03EcritureTrace(XZSTC_WARNING,
      				" Acces au fichier '%s' impossible.", va_nom_fichier);
      vl_cr = XDC_NOK;
   }
   /* Finsi */
   
   return (vl_cr);
}


/*X*/
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU : 
*
*	Lit le fichier de parametrage des UGTP et remplit la structure associee
*
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static int lecture_fichier_param_UGTP (char *va_nom_fichier)

/*
* ARGUMENTS EN ENTREE :
*
*	va_nom_fichier : Nom du fichier de parametrage des UGTP
*   aucun
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
*
*
* CONDITION D'UTILISATION
*
* FONCTION 
*
*	Lit le fichier de parametrage des UGTP et remplit la structure associee
*
-----------------------------------------------------------------------------------------*/
{
   static char *version = "$Id: etdp_cfg.c,v 1.18 2021/06/14 17:23:20 devgfi Exp $ : lecture_fichier_param_UGTP" ;
   char vl_ligne_fichier[512];
   int vl_nb_elements_lus;
   XDY_TexteAlerte vl_ArgParam = "", vl_NomParam = "";
   FILE *fp;
   int i=0;
   int vl_cr = XDC_OK;
   
   /* Si le fichier existe */
   
   if(( fp=fopen(va_nom_fichier, "r"))!=NULL)
   {
      /* Alors */
      /* Tant que le fichier n'est pas completement lu */
      
      while(fgets(vl_ligne_fichier, 512, fp)!=NULL)
      {
         /* Si Le caractere lu n'est pas un '#' */
         
         if(vl_ligne_fichier[0]!= XDF_COMMENTAIRE)
         {
            /* Alors */
               
            /* Recuperer les elements de la ligne */
            
            vl_nb_elements_lus = sscanf ( strtok(vl_ligne_fichier,"\n"), "%[^;];%[^;];",
               						vl_NomParam, vl_ArgParam );
		            
            /* Si nombre d'elements lus n'est pas OK */
            if( vl_nb_elements_lus != 2 )
            {
               /* Alors */
               /* Tracer l'erreur et quitter */
               
               XZST_03EcritureTrace(XZSTC_WARNING,
                   " Recuperation des donnees fichier '%s' impossible.", va_nom_fichier);
              
            }
            else
            {
            
               /* Si argument delai dde horaire alors mise en memoire */
               if ( !strcmp( vl_NomParam, ETDPC_DELAI_DDE_UGTP ) )
               {
                  vg_ConfigTDP.DelaiDdeHoraire = atoi(vl_ArgParam);
               }
               /* Si argument numero d'alerte alors mise en memoire */
               if ( !strcmp( vl_NomParam, ETDPC_NUM_ALERTE_UGTP ) )
               {
                  vg_ConfigTDP.NumAlerte = atoi(vl_ArgParam);
               }
               /* Si argument libelle d'alerte alors mise en memoire */
               if ( !strcmp( vl_NomParam, ETDPC_LIB_ALERTE_UGTP ) )
               {
                  strcpy( vg_ConfigTDP.LibAlerte, vl_ArgParam );
               }
               /* Si argument numero d'alerte panne zone alors mise en memoire */
               if ( !strcmp( vl_NomParam, ETDPC_NUM_ALERTE_PANNE ) )
               {
                  vg_ConfigTDP.NumPanne = atoi(vl_ArgParam);
               }
               /* Si argument libelle d'alerte panne zone alors mise en memoire */
               if ( !strcmp( vl_NomParam, ETDPC_LIB_ALERTE_PANNE ) )
               {
                  strcpy( vg_ConfigTDP.LibPanne, vl_ArgParam );
               }
            
            }
         }
         /* Finsi */
      }
      /* Fin Tant */
      
      fclose ( fp );
      XZST_03EcritureTrace( XZSTC_WARNING," Params : Delai : %d  Alerte : %d <%s> Panne : %d <%s>",
      				vg_ConfigTDP.DelaiDdeHoraire,
      				vg_ConfigTDP.NumAlerte,
      				vg_ConfigTDP.LibAlerte,
      				vg_ConfigTDP.NumPanne,
      				vg_ConfigTDP.LibPanne );
   }
   else
   {
      /* Sinon */
      
      XZST_03EcritureTrace(XZSTC_WARNING,
      				" Acces au fichier '%s' impossible.", va_nom_fichier);
      vl_cr = XDC_NOK;
   }
   /* Finsi */

   return (vl_cr);
}

/*X*/
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU : 
*
*	Creation de liste des zones
*
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static int ec_creer_liste_ZDP ( int va_nb, int va_i, XDY_ZDP 	va_ZDP, char *va_nom,
				int 		va_NumAppel )

/*
* ARGUMENTS EN ENTREE :
*
*   aucun
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
*
*
* CONDITION D'UTILISATION
*
* FONCTION 
*
*	Creation de liste des zones
*
-----------------------------------------------------------------------------------------*/
{
   static char *version = "$Id: etdp_cfg.c,v 1.18 2021/06/14 17:23:20 devgfi Exp $ : ec_creer_liste_ZDP" ;
   XZEZT_Liste_Zdp *	pl_NewZone = NULL;
   int				i = 0;
   EXT_NOM_EQT_LCR *	 	pl_NewNomZone = NULL;
   int				vl_Trouve = XDC_FAUX;
   XDY_Datagroup		pl_NomDG;
   T_BOOL			vl_EtatDg = FALSE;
   XDY_Horodate  		vl_horodate;
   int           		vl_jour;

   XZST_03EcritureTrace ( XZSTC_FONCTION,"ec_creer_liste_ZDP in ");


      XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME,&vl_jour,&vl_horodate);
   XZAD05_Dernier_TDP(va_ZDP.Numero, vl_horodate, &(va_ZDP.TDP_Courant));     
   XZST_03EcritureTrace ( XZSTC_FONCTION,"ec_creer_liste_ZDP in ");
      for ( i = (int) strlen(va_ZDP.Libelle)-1; i>0; i--) 
         if (va_ZDP.Libelle[i]==' ') va_ZDP.Libelle[i]='\0';
         else break;
XZST_03EcritureTrace ( XZSTC_INFO, " Liste zone : Numero : %d\nCode : %d\nSite : %d\nLibelle : <%s>\nAuto : %d\nNomAutoroute : %s\n\
Maitre : %d\nPR_Debut : %d\nPR_Fin : %d\nSens : %d\nTDP_Normal : %ld\nTDP_Max : %d\nTDP_Courant : %ld\nSeuil_Bas : %ld \n\
Seuil_Haut : %ld\nType_Zone : %d\n", 
           va_ZDP.Numero,
           va_ZDP.Code_Zone,
           (int) va_ZDP.Site,
           va_ZDP.Libelle,
           va_ZDP.Autoroute,
           va_ZDP.NomAutoroute,
           va_ZDP.Maitre,
           va_ZDP.PR_Debut,
           va_ZDP.PR_Fin,
           va_ZDP.Sens,
           va_ZDP.TDP_Normal,
           va_ZDP.TDP_Max,
           va_ZDP.TDP_Courant,
           va_ZDP.Seuil_Bas,
           va_ZDP.Seuil_Haut,
           va_ZDP.Type_Zone );
           
           
      /*A Allocation memoire pour la nouvelle structure */
      if ( ( pl_NewZone = (XZEZT_Liste_Zdp *) malloc ( sizeof(XZEZT_Liste_Zdp) ) ) == NULL)
      {
         /* Ecriture trace */ 
         XZST_03EcritureTrace ( XZSTC_WARNING, " OUT: ec_creer_liste_ZDP Allocation memoire impossible");
         return XDC_NOK;
      }

      /*A Mise a jour de la structure a inserer */
      va_ZDP.Franchissement_Seuil = XDC_SEUIL_TDP_ABSENT;
      memcpy ( &(pl_NewZone->zdp), &va_ZDP, sizeof (XDY_ZDP) );
      pl_NewZone->zdp.Seuil_Haut = (va_ZDP.TDP_Normal * va_ZDP.Seuil_Haut) / 100;
      pl_NewZone->zdp.Seuil_Bas = (va_ZDP.TDP_Normal * va_ZDP.Seuil_Bas) / 100;
      pl_NewZone->zdp.TDP_Surcharge = 0;
      pl_NewZone->zdp.Numero_Surcharge = 0;
      pl_NewZone->zdp.Cle_Surcharge = 0;

      /*A Ajout de la structure dans la liste */
      pl_NewZone->suivant = pg_ListeZone;
      pg_ListeZone        = pl_NewZone;
      
      for(i=0 ;i!=ETDPC_NB_MAX_UGTP; i++)
      {
         if ( (va_ZDP.Maitre == vg_DonneesUGTP[i].Numero) &&
              (va_ZDP.Code_Zone <= ETDPC_NB_MAX_ZONE) &&
              (va_ZDP.Code_Zone > 0) )
         {
            /* Validation de la zone */
            vg_Mesure[i].Validite6mn[va_ZDP.Code_Zone] = XDC_VRAI;
            vg_Mesure[i].ValiditeHoraire[va_ZDP.Code_Zone] = XDC_VRAI;
            vg_Mesure[i].Numero[va_ZDP.Code_Zone] = va_ZDP.Numero;
            vg_DonneesUGTP[i].Zone[va_ZDP.Code_Zone].ChampValide=Valide;
	     XZST_03EcritureTrace ( XZSTC_DEBUG1, " validation %d", va_ZDP.Code_Zone);
            sprintf ( vg_DonneesUGTP[i].Zone[va_ZDP.Code_Zone].Nom, "%s-%d-%d", va_ZDP.NomAutoroute, va_ZDP.Sens, va_ZDP.PR_Debut );
            vl_Trouve = XDC_VRAI;
            break;
         }
	 else
	   XZST_03EcritureTrace ( XZSTC_DEBUG1, "invalidation %d maitre %d , numero %d",va_ZDP.Code_Zone,va_ZDP.Maitre,vg_DonneesUGTP[i].Numero);
      }
      
      /* Si la zone est une voisine alors s'abonner aux mesures */
      if ( (vl_Trouve == XDC_FAUX) && (va_ZDP.Code_Zone > 0) )
      {
	sprintf ( pl_NomDG, "%s_V%d", XDG_EMES_UGTP_6MN, va_ZDP.Numero );
   	/*A Joindre ce datagroup s'il n'est pas deja joint */
   	TipcSrvSubjectGetSubscribe ( pl_NomDG, &vl_EtatDg );
   	if ( !vl_EtatDg )
   	{
   	   if ( !XDG_JoindreDG( pl_NomDG ) )
   	   {
	      /*B Ecriture Trace */
      	      XZST_03EcritureTrace ( XZSTC_WARNING, "Echec abonnement au Datagroup %s ", pl_NomDG );
	   }
	   else
	   {
      	      XZST_03EcritureTrace ( XZSTC_WARNING, "Abonnement au Datagroup <%s> ", pl_NomDG );
	   }
	}
         
      }
      
      /* Creation de la liste des nom de zone */
      /*A Allocation memoire pour la nouvelle structure */
      if ( ( pl_NewNomZone = (EXT_NOM_EQT_LCR *) malloc ( sizeof(EXT_NOM_EQT_LCR) ) ) == NULL)
      {
         /* Ecriture trace */ 
         XZST_03EcritureTrace ( XZSTC_WARNING, " OUT: ec_creer_liste_ZDP Allocation memoire nom zoneimpossible");
         return XDC_NOK;
      }

      /*A Mise a jour de la structure a inserer */
      strcpy ( pl_NewNomZone->NomMachine, vg_ConfigTDP.NomMachine );
      sprintf ( pl_NewNomZone->NomEqt, " ZDP %s-%d-%d", va_ZDP.NomAutoroute, va_ZDP.Sens, va_ZDP.PR_Debut );
      pl_NewNomZone->Numero = va_ZDP.Numero;
      /*A Ajout de la structure dans la liste */
      pl_NewNomZone->Suivant = pg_DebutNomZDP;
      pg_DebutNomZDP         = pl_NewNomZone;
      
      return(XDC_OK);

}   

/*X*/
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU : 
*
*	Creation de liste des destination
*
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static int ec_creer_liste_Destination ( XDY_Destination va_Destination, 
					int 		va_NumAppel )

/*
* ARGUMENTS EN ENTREE :
*
*   aucun
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
*
*
* CONDITION D'UTILISATION
*
* FONCTION 
*
*	Creation de liste des destination
*
-----------------------------------------------------------------------------------------*/
{
   static char *version = "$Id: etdp_cfg.c,v 1.18 2021/06/14 17:23:20 devgfi Exp $ : ec_creer_liste_Destination" ;
   XZEZT_Liste_Destinations *	pl_NewDest = NULL;
   
XZST_03EcritureTrace ( XZSTC_DEBUG1, " Liste destination : Numero : %d\nMsg TFM : <%s>\nMsg PMV : <%s>\nAuto : %d\n\
PR : %d\nSens : %d\nType_Loc : %d\n", 
           va_Destination.Numero,
           va_Destination.Libelle_TFM,
           va_Destination.Libelle_PMV,
           va_Destination.Autoroute,
           va_Destination.PR,
           va_Destination.Sens,
           va_Destination.Type_Localisation);
   

      /*A Allocation memoire pour la nouvelle structure */
      if ( ( pl_NewDest = (XZEZT_Liste_Destinations *) malloc ( sizeof(XZEZT_Liste_Destinations) ) ) == NULL)
      {
         /* Ecriture trace */ 
         XZST_03EcritureTrace ( XZSTC_WARNING, " OUT: ec_creer_liste_Destination Allocation memoire impossible");
         return XDC_NOK;
      }

      /*A Mise a jour de la structure a inserer */
      memcpy ( &(pl_NewDest->destination), &va_Destination, sizeof (XDY_Destination) );
   
      /*A Ajout de la structure dans la liste */
      pl_NewDest->suivant = pg_ListeDest;
      pg_ListeDest        = pl_NewDest;

      return(XDC_OK);

} 

    
/*X*/
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU : 
*
*	Creation de liste des PMV
*
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static int ec_creer_liste_PMV ( XZAOT_PMV_TDP 	va_PMV, 
				int 		va_NumAppel )

/*
* ARGUMENTS EN ENTREE :
*
*   aucun
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
*
*
* CONDITION D'UTILISATION
*
* FONCTION 
*
*	Creation de liste des PMV
*
-----------------------------------------------------------------------------------------*/
{
   static char *version = "$Id: etdp_cfg.c,v 1.18 2021/06/14 17:23:20 devgfi Exp $ : ec_creer_liste_PMV" ;
   ETDP_CONFIG_PMV *	pl_NewPMV = NULL;
   XZEZT_Liste_Zdp *	pl_Zone   = NULL;
   int			i = 0;
   XDY_Datagroup	pl_NomDG;
   T_BOOL		vl_EtatDg = FALSE;
   
#if 0
   for(i=0 ;i!=ETDPC_NB_MAX_UGTP; i++)
   {
      if ( vg_DonneesUGTP[i].ChampValide == Valide )
      {
         for ( pl_Zone = pg_ListeZone; pl_Zone != NULL; pl_Zone = pl_Zone->suivant )
         {
            if ( (pl_Zone->zdp.Maitre == vg_DonneesUGTP[i].Numero) &&
                 (pl_Zone->zdp.Autoroute == va_PMV.Autoroute) && 
                 (pl_Zone->zdp.Sens == va_PMV.Sens) &&
                 (((pl_Zone->zdp.Sens == XDC_SENS_SUD)  && (pl_Zone->zdp.PR_Debut <= va_PMV.PR) && (pl_Zone->zdp.PR_Fin >  va_PMV.PR)) ||
                  ((pl_Zone->zdp.Sens == XDC_SENS_NORD) && (pl_Zone->zdp.PR_Debut <  va_PMV.PR) && (pl_Zone->zdp.PR_Fin >= va_PMV.PR))) ) 
            {
#endif
	       sprintf ( pl_NomDG, "%s_%s", XDG_ETDP_PMV, va_PMV.NomMachine );
   	       /*A Joindre ce datagroup s'il n'est pas deja joint */
   	       TipcSrvSubjectGetSubscribe ( pl_NomDG, &vl_EtatDg );
   	       if ( !vl_EtatDg )
   	       {
   	          if ( !XDG_JoindreDG( pl_NomDG ) )
   	          {
	             /*B Ecriture Trace */
      	             XZST_03EcritureTrace ( XZSTC_WARNING, "Echec abonnement au Datagroup %s ", pl_NomDG );
	          }
	          else
	          {
      	             XZST_03EcritureTrace ( XZSTC_WARNING, "Abonnement au Datagroup <%s> ", pl_NomDG );
	          }
	       }
	          
               /*A Allocation memoire pour la nouvelle structure */
               if ( ( pl_NewPMV = (ETDP_CONFIG_PMV *) malloc ( sizeof(ETDP_CONFIG_PMV) ) ) == NULL)
               {
                  /* Ecriture trace */ 
                  XZST_03EcritureTrace ( XZSTC_WARNING, " OUT: ec_creer_liste_PMV Allocation memoire impossible");
                  return XDC_NOK;
               }
               memset ( (void *) pl_NewPMV, 0, sizeof(ETDP_CONFIG_PMV) );

               XZST_03EcritureTrace ( XZSTC_DEBUG1, " Liste PMV : Numero : %d\nAuto 	: %d\nPR	: %ld\nSens	: %d\nLT	: %s", 
				va_PMV.Numero, va_PMV.Autoroute, va_PMV.PR, va_PMV.Sens, va_PMV.NomMachine);

               /*A Mise a jour de la structure a inserer */
               pl_NewPMV->EtatPMV.Numero = va_PMV.Numero;
               pl_NewPMV->EtatPMV.Destination 		= 0;
               pl_NewPMV->EtatPMV.DestinationAlternat 	= 0;
			   pl_NewPMV->EtatPMV.DestinationNoeud		= 0;
               pl_NewPMV->NumAction = 0;
      	       sprintf ( pl_NewPMV->Nom, " PMV A8-%d-%d", va_PMV.Sens, va_PMV.PR );
               pl_NewPMV->PMV = va_PMV;
	       pl_NewPMV->Destination=0;
	       pl_NewPMV->Graphe_Destination=NULL;
  	       pl_NewPMV->DestinationLointaine=0;
	       pl_NewPMV->Graphe_DestinationLointaine=0;
	       pl_NewPMV->DestinationNoeud=0;
	       pl_NewPMV->Graphe_DestinationNoeud=NULL;

		/*chargement de la destination proche et de ses zdp*/

               /*A Ajout de la structure dans la liste */
               pl_NewPMV->suivant = pg_ListePMV;
               pg_ListePMV        = pl_NewPMV;

	          
               return(XDC_OK);
#if 0
            }
         }
      }
   }
#endif
   return(XDC_OK);

}     

/************* PMVA ***********************/
/*X*/
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU :
*
*	Creation de liste des PMV
*
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/
static int ec_creer_liste_PMVA ( XZAOT_PMV_TDP 	va_PMVA,
				int 		va_NumAppel )
/*
* ARGUMENTS EN ENTREE :
*
*   aucun
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR :
*
*
* CONDITION D'UTILISATION
*
* FONCTION
*
*	Creation de liste des PMV
*
-----------------------------------------------------------------------------------------*/
{
   static char *version = "$Id: etdp_cfg.c,v 1.18 2021/06/14 17:23:20 devgfi Exp $ : ec_creer_liste_PMVA" ;
   ETDP_CONFIG_PMVA *	pl_NewPMVA = NULL;
   XZEZT_Liste_Zdp *	pl_Zone   = NULL;
   int					i = 0;
   XDY_Datagroup		pl_NomDG;
   T_BOOL				vl_EtatDg = FALSE;

#if 0
   for(i=0 ;i!=ETDPC_NB_MAX_UGTP; i++)
   {
      if ( vg_DonneesUGTP[i].ChampValide == Valide )
      {
         for ( pl_Zone = pg_ListeZone; pl_Zone != NULL; pl_Zone = pl_Zone->suivant )
         {
            if ( (pl_Zone->zdp.Maitre == vg_DonneesUGTP[i].Numero) &&
                 (pl_Zone->zdp.Autoroute == va_PMV.Autoroute) &&
                 (pl_Zone->zdp.Sens == va_PMV.Sens) &&
                 (((pl_Zone->zdp.Sens == XDC_SENS_SUD)  && (pl_Zone->zdp.PR_Debut <= va_PMV.PR) && (pl_Zone->zdp.PR_Fin >  va_PMV.PR)) ||
                  ((pl_Zone->zdp.Sens == XDC_SENS_NORD) && (pl_Zone->zdp.PR_Debut <  va_PMV.PR) && (pl_Zone->zdp.PR_Fin >= va_PMV.PR))) )
            {
#endif
	       sprintf ( pl_NomDG, "%s_%s", XDG_ETDP_PMVA, va_PMVA.NomMachine );
   	       /*A Joindre ce datagroup s'il n'est pas deja joint */
   	       TipcSrvSubjectGetSubscribe ( pl_NomDG, &vl_EtatDg );
   	       if ( !vl_EtatDg )
   	       {
   	          if ( !XDG_JoindreDG( pl_NomDG ) )
   	          {
	             /*B Ecriture Trace */
      	             XZST_03EcritureTrace ( XZSTC_WARNING, "Echec abonnement au Datagroup %s ", pl_NomDG );
	          }
	          else
	          {
      	             XZST_03EcritureTrace ( XZSTC_WARNING, "Abonnement au Datagroup <%s> ", pl_NomDG );
	          }
	       }

               /*A Allocation memoire pour la nouvelle structure */
               if ( ( pl_NewPMVA = (ETDP_CONFIG_PMVA *) malloc ( sizeof(ETDP_CONFIG_PMVA) ) ) == NULL)
               {
                  /* Ecriture trace */
                  XZST_03EcritureTrace ( XZSTC_WARNING, " OUT: ec_creer_liste_PMV Allocation memoire impossible");
                  return XDC_NOK;
               }
               memset ( (void *) pl_NewPMVA, 0, sizeof(ETDP_CONFIG_PMVA) );

               XZST_03EcritureTrace ( XZSTC_DEBUG1, " Liste PMVA : Numero : %d\nAuto 	: %d\nPR	: %ld\nSens	: %d\nLT	: %s",
				va_PMVA.Numero, va_PMVA.Autoroute, va_PMVA.PR, va_PMVA.Sens, va_PMVA.NomMachine);

               /*A Mise a jour de la structure a inserer */
               pl_NewPMVA->EtatPMVA.Numero = va_PMVA.Numero;
               pl_NewPMVA->EtatPMVA.Dest1_proche 	= 0;
               pl_NewPMVA->EtatPMVA.Dest1_loin 		= 0;
			   pl_NewPMVA->EtatPMVA.Dest2_proche		= 0;
			   pl_NewPMVA->EtatPMVA.Dest2_loin		= 0;

               pl_NewPMVA->NumAction = 0;
      	       sprintf ( pl_NewPMVA->Nom, " PMVA A8-%d-%d", va_PMVA.Sens, va_PMVA.PR );
               pl_NewPMVA->PMVA = va_PMVA;
	       pl_NewPMVA->Destination1_proche = 0;
	       pl_NewPMVA->Destination1_loin = 0;
	       pl_NewPMVA->Destination2_proche = 0;
	       pl_NewPMVA->Destination2_loin = 0;

	       pl_NewPMVA->Graphe_Destination1=NULL;
	       pl_NewPMVA->Graphe_Destination2=NULL;
	       pl_NewPMVA->Graphe_Destination1Lointaine=0;
	       pl_NewPMVA->Graphe_Destination2Lointaine=0;

		/*chargement de la destination proche et de ses zdp*/

               /*A Ajout de la structure dans la liste */
               pl_NewPMVA->suivant = pg_ListePMVA;
               pg_ListePMVA        = pl_NewPMVA;


               return(XDC_OK);
#if 0
            }
         }
      }
   }
#endif
   return(XDC_OK);

}

/*X*/
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU :
*
*	Mise a jour de l'etat des PMVA
*
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/
static int ec_mise_jour_etat_PMVA ( 	XDY_Etat_PMVA 	va_PMVA,
					int 		va_NumAction )
/*
* ARGUMENTS EN ENTREE :
*
*   aucun
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR :
*
*
* CONDITION D'UTILISATION
*
* FONCTION
*
*	Mise a jour de l'etat des PMV
*
-----------------------------------------------------------------------------------------*/
{
   static char *version = "$Id: etdp_cfg.c,v 1.18 2021/06/14 17:23:20 devgfi Exp $ : ec_mise_jour_etat_PMVA" ;
   ETDP_CONFIG_PMVA *	pl_PMVA = NULL;

   for ( pl_PMVA = pg_ListePMVA; pl_PMVA != NULL; pl_PMVA = pl_PMVA->suivant  )
   {
      if ( va_PMVA.Numero == pl_PMVA->PMVA.Numero )
      {
          XZST_03EcritureTrace ( XZSTC_WARNING, " Etat PMVA : Numero : %d\nL1	: <%s>\nL2	: <%s>\nL3	: <%s>\nL4	: <%s>\nL5	: <%s>\n\
A1	: <%s>\nA2	: <%s>\nA3	: <%s>\nA4	: <%s>\nA5	: <%s>\nC1	: %d\nC2	: %d\nC3	: %d\nC4	: %d\nC5	: %d\n\
Dest1_proche	: %d\nDest1_loin	: %d\nDest2_proche	: %d\nDest2_loin	: %d\nAction  : %d",
           	va_PMVA.Numero,
          	va_PMVA.Texte1,va_PMVA.Texte2,va_PMVA.Texte3,va_PMVA.Texte4,va_PMVA.Texte5,
           	va_PMVA.Alternat1,va_PMVA.Alternat2,va_PMVA.Alternat3,va_PMVA.Alternat4,va_PMVA.Alternat5,
	   	va_PMVA.Clign1,va_PMVA.Clign2,va_PMVA.Clign3,va_PMVA.Clign4,va_PMVA.Clign5,
	   	va_PMVA.Dest1_proche,  va_PMVA.Dest1_loin, va_PMVA.Dest2_proche,va_PMVA.Dest2_loin,
	   	va_NumAction);

          /*A Mise a jour de la structure */
          va_PMVA.Affichage1 		= XDC_PMV_TEXTE;
          va_PMVA.Affichage2 		= XDC_PMV_TEXTE;
          va_PMVA.Affichage3 		= XDC_PMV_TEXTE;
          va_PMVA.Affichage4 		= XDC_PMV_TEXTE;
          va_PMVA.Affichage5 		= XDC_PMV_TEXTE;
          va_PMVA.Clign1 		= XDC_PMV_PASCLIGNO;
          va_PMVA.Clign2 		= XDC_PMV_PASCLIGNO;
          va_PMVA.Clign3	 		= XDC_PMV_PASCLIGNO;
          va_PMVA.Clign4 		= XDC_PMV_PASCLIGNO;
          va_PMVA.Clign4 		= XDC_PMV_PASCLIGNO;
          va_PMVA.Clign5 		= XDC_PMV_PASCLIGNO;

          if ( !strcmp(va_PMVA.Texte1, XDC_CHAINE_VIDE) )    strcpy ( va_PMVA.Texte1, ETDPC_LIGNE_VIDE);
          if ( !strcmp(va_PMVA.Texte2, XDC_CHAINE_VIDE) )    strcpy ( va_PMVA.Texte2, ETDPC_LIGNE_VIDE);
          if ( !strcmp(va_PMVA.Texte3, XDC_CHAINE_VIDE) )    strcpy ( va_PMVA.Texte3, ETDPC_LIGNE_VIDE);
          if ( !strcmp(va_PMVA.Texte4, XDC_CHAINE_VIDE) )    strcpy ( va_PMVA.Texte4, ETDPC_LIGNE_VIDE);
          if ( !strcmp(va_PMVA.Texte5, XDC_CHAINE_VIDE) )    strcpy ( va_PMVA.Texte5, ETDPC_LIGNE_VIDE);

          if ( !strcmp(va_PMVA.Alternat1, XDC_CHAINE_VIDE) ) strcpy ( va_PMVA.Alternat1, ETDPC_LIGNE_VIDE);
          if ( !strcmp(va_PMVA.Alternat2, XDC_CHAINE_VIDE) ) strcpy ( va_PMVA.Alternat2, ETDPC_LIGNE_VIDE);
          if ( !strcmp(va_PMVA.Alternat3, XDC_CHAINE_VIDE) ) strcpy ( va_PMVA.Alternat3, ETDPC_LIGNE_VIDE);
          if ( !strcmp(va_PMVA.Alternat4, XDC_CHAINE_VIDE) ) strcpy ( va_PMVA.Alternat4, ETDPC_LIGNE_VIDE);
          if ( !strcmp(va_PMVA.Alternat5, XDC_CHAINE_VIDE) ) strcpy ( va_PMVA.Alternat5, ETDPC_LIGNE_VIDE);

          memcpy ( &(pl_PMVA->EtatPMVA), &va_PMVA, sizeof (XDY_Etat_PMVA) );
          pl_PMVA->NumAction = va_NumAction;
          break;
      }
   }

   return(XDC_OK);

}
/******************************************************************/
    
/*X*/
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU : 
*
*	Mise a jour de l'etat des PMV
*
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static int ec_mise_jour_etat_PMV ( 	XDY_Etat_PMV 	va_PMV,
					int 		va_NumAction )

/*
* ARGUMENTS EN ENTREE :
*
*   aucun
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
*
*
* CONDITION D'UTILISATION
*
* FONCTION 
*
*	Mise a jour de l'etat des PMV
*
-----------------------------------------------------------------------------------------*/
{
   static char *version = "$Id: etdp_cfg.c,v 1.18 2021/06/14 17:23:20 devgfi Exp $ : ec_mise_jour_etat_PMV" ;
   ETDP_CONFIG_PMV *	pl_PMV = NULL;
   
   for ( pl_PMV = pg_ListePMV; pl_PMV != NULL; pl_PMV = pl_PMV->suivant  )
   {
      if ( va_PMV.Numero == pl_PMV->PMV.Numero )
      {
          XZST_03EcritureTrace ( XZSTC_WARNING, " Etat PMV : Numero : %d\nL1	: <%s>\nL2	: <%s>\nL3	: <%s>\n\
A1	: <%s>\nA2	: <%s>\nA3	: <%s>\nC1	: %d\nC2	: %d\nC3	: %d\n\
L4	: <%s>\nA4	: <%s>\nDest	: %d\nDestAlt	: %d\nAction  : %d\nDestNoeud	: %ld", 
           	va_PMV.Numero,
          	va_PMV.Texte1,va_PMV.Texte2,va_PMV.Texte3,
           	va_PMV.Alternat1,va_PMV.Alternat2,va_PMV.Alternat3,
	   	va_PMV.Clign1,va_PMV.Clign2,va_PMV.Clign3,
	   	va_PMV.Texte4,va_PMV.Alternat4,
	   	va_PMV.Destination,  va_PMV.DestinationAlternat, va_NumAction , va_PMV.DestinationNoeud );
   
          /*A Mise a jour de la structure */
          va_PMV.Affichage1 		= XDC_PMV_TEXTE;
          va_PMV.Affichage2 		= XDC_PMV_TEXTE;
          va_PMV.Affichage3 		= XDC_PMV_TEXTE;
          va_PMV.Affichage4 		= XDC_PMV_TEXTE;
          va_PMV.Clign1 		= XDC_PMV_PASCLIGNO;
          va_PMV.Clign2 		= XDC_PMV_PASCLIGNO;
          va_PMV.Clign3	 		= XDC_PMV_PASCLIGNO;
          va_PMV.Clign4 		= XDC_PMV_PASCLIGNO;
          if ( !strcmp(va_PMV.Texte4, XDC_CHAINE_VIDE) )    strcpy ( va_PMV.Texte4, ETDPC_LIGNE_VIDE);
          if ( !strcmp(va_PMV.Alternat4, XDC_CHAINE_VIDE) ) strcpy ( va_PMV.Alternat4, ETDPC_LIGNE_VIDE);
          memcpy ( &(pl_PMV->EtatPMV), &va_PMV, sizeof (XDY_Etat_PMV) );
          pl_PMV->NumAction = va_NumAction;
          break;
      }
   }
   
   return(XDC_OK);

}        

/*X*/
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU : 
*
*	Mise a jour des franchissement de seuil 
*
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static int ec_mise_jour_Franchissements ( 	XDY_Eqt		va_Zone )

/*
* ARGUMENTS EN ENTREE :
*
*   aucun
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
*
*
* CONDITION D'UTILISATION
*
* FONCTION 
*
*	Mise a jour de zone avec un seuil depasse
*
-----------------------------------------------------------------------------------------*/
{
   static char *version = "$Id: etdp_cfg.c,v 1.18 2021/06/14 17:23:20 devgfi Exp $ : ec_mise_jour_Franchissements" ;
   XZEZT_Liste_Zdp 	*pl_Zone   = NULL;
   
   for ( pl_Zone = pg_ListeZone; pl_Zone != NULL; pl_Zone = pl_Zone->suivant  )
   {
      if ( va_Zone == pl_Zone->zdp.Numero )
      {
          /*A Mise a jour du franchissement */
          XZST_03EcritureTrace ( XZSTC_WARNING, " Franchissement Zone	: %d", va_Zone );
          pl_Zone->zdp.Franchissement_Seuil    = XDC_SEUIL_TDP_HAUT;
          break;
      }
   }
   
   return(XDC_OK);

}   
/*X*/
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU : 
*
*	Lecture de la config des zone et des point remarquable en base 
*
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static int lecture_config_en_base( void )

/*
* ARGUMENTS EN ENTREE :
*
*   aucun
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
*
*
* CONDITION D'UTILISATION
*
* FONCTION 
*
*	Lecture de la config des zone et des point remarquable en base 
*	et rempli les listes associes
*
-----------------------------------------------------------------------------------------*/
{
   static char *version = "$Id: etdp_cfg.c,v 1.18 2021/06/14 17:23:20 devgfi Exp $ : lecture_config_en_base" ;
   int				vl_ValRet = XDC_OK;
   XDY_Entier			vl_NbElement = 0;
   XZEZT_Liste_Zdp 		*pl_Zone   = NULL,
   			 	*pl_ExZone = NULL;
   EXT_NOM_EQT_LCR	 	*pl_NomZone   = NULL,
   			 	*pl_ExNomZone = NULL;
   XZEZT_Liste_Destinations 	*pl_Dest   = NULL,
   				*pl_ExDest = NULL;
   ETDP_CONFIG_PMV	 	*pl_PMV   = NULL,
   				*pl_ExPMV = NULL;

   /********** PMVA *************/
   ETDP_CONFIG_PMVA	 	*pl_PMVA   = NULL,
						*pl_ExPMVA = NULL;
   /*****************************/
   int				i = 0;
   
   /*A
    * Connexion a la base de donnees 
    */
	

XZST_03EcritureTrace( XZSTC_WARNING,"Nom du site %s",vg_ConfigTDP.NomSite);
   if ( ( vl_ValRet = XZAG02_Connexion_Base(vg_ConfigTDP.NomSite) ) != XDC_OK )
   {
      XZST_03EcritureTrace( XZSTC_WARNING, "Impossible de se connecter a la base erreur : %d.", vl_ValRet );
      ec_sortir();
   }
   
   /*A liberation de la liste des zones TDP  */
   for ( pl_Zone = pl_ExZone = pg_ListeZone; pl_Zone != NULL; pl_ExZone = pl_Zone  )
   {
      /*A Liberation memoire */
      pl_Zone = pl_Zone->suivant;
      free ( pl_ExZone );
   }
   pg_ListeZone = NULL;

   /*A liberation de la liste des noms de zone TDP  */
   for ( pl_NomZone = pl_ExNomZone = pg_DebutNomZDP; pl_NomZone != NULL; pl_ExNomZone = pl_NomZone  )
   {
      /*A Liberation memoire */
      pl_NomZone = pl_NomZone->Suivant;
      free ( pl_ExNomZone );
   }
   pg_DebutNomZDP = NULL;

   /*A liberation de la liste des destination TDP  */
   for ( pl_Dest = pl_ExDest = pg_ListeDest; pl_Dest != NULL; pl_ExDest = pl_Dest  )
   {
      /*A Liberation memoire */
      pl_Dest = pl_Dest->suivant;
      free ( pl_ExDest );
   }
   pg_ListeDest = NULL;

   /*A liberation de la liste des PMV  */
   for ( pl_PMV = pl_ExPMV = pg_ListePMV; pl_PMV != NULL; pl_ExPMV = pl_PMV  )
   {
      /*A Liberation memoire */
      pl_PMV = pl_PMV->suivant;
      free ( pl_ExPMV );
   }
   pg_ListePMV = NULL;
 
   /************************************/
   /*A liberation de la liste des PMVA  */
   for ( pl_PMVA = pl_ExPMVA = pg_ListePMVA; pl_PMVA != NULL; pl_ExPMVA = pl_PMVA  )
   {
      /*A Liberation memoire */
      pl_PMVA = pl_PMVA->suivant;
      free ( pl_ExPMVA );
   }
   pg_ListePMVA = NULL;
   /************************************/

#ifdef _HPUX_SOURCE
   vl_ValRet = XZAO4001_Liste_Zones_De_Parcours ( XDC_BASE_CFG, 
   						NULL,
   					 	ec_creer_liste_ZDP,
   					 	&vl_NbElement );
#else
   vl_ValRet = XZAO4001_Liste_Zones_De_Parcours ( XDC_BASE_CFG, 
						     0,
						     ec_creer_liste_ZDP,
						     &vl_NbElement );
#endif    

   vl_ValRet = XZAO401_Lire_Liste_Destination_TDP ( XDC_BASE_CFG,
   					 	ec_creer_liste_Destination,
   					 	&vl_NbElement );

   vl_ValRet = XZAO410_Lire_Liste_PMV_TDP ( 	ec_creer_liste_PMV,
   						&vl_NbElement );
   
   vl_ValRet = XZAD07_Liste_PMV_TDP_Actif ( 	ec_mise_jour_etat_PMV,
   						&vl_NbElement );

   /************************************/
   vl_ValRet = XZAO468_Lire_Liste_PMVA_TDP ( 	ec_creer_liste_PMVA,
      						&vl_NbElement );

   vl_ValRet = XZAD17_Liste_PMVA_TDP_Actif ( 	ec_mise_jour_etat_PMVA,
      						&vl_NbElement );
   /************************************/

#ifdef _HPUX_SOURCE
   vl_ValRet = XZAD08_Liste_Franchissements (	ec_mise_jour_Franchissements );
#else
   vl_ValRet = XZAD08_Liste_Franchissements (	(XDY_FonctionInt)ec_mise_jour_Franchissements );
#endif

#ifdef _HPUX_SOURCE
   vl_ValRet = XZAO470_Lire_Liste_Destination_PMV (XDC_BASE_CFG,		
						ec_creer_liste_Destination_pmv,
						&vl_NbElement );
#else
   vl_ValRet = XZAO470_Lire_Liste_Destination_PMV (XDC_BASE_CFG,		
						(XDY_FonctionInt)ec_creer_liste_Destination_pmv,
						&vl_NbElement );
#endif
   /************************************/
#ifdef _HPUX_SOURCE
   vl_ValRet = XZAO469_Lire_Liste_Destination_PMVA (XDC_BASE_CFG,
		   	   	   	   	  ec_creer_liste_Destination_pmva,
   						&vl_NbElement );
#else
   vl_ValRet = XZAO469_Lire_Liste_Destination_PMVA (XDC_BASE_CFG,
		   	   	   	   	  (XDY_FonctionInt)ec_creer_liste_Destination_pmva,
   						&vl_NbElement );
#endif
   /************************************/

   /*A Deconnexion de la base de donnees */
   /*XZAG03_Deconnexion_Base();*/
   
   
   return(XDC_OK);
}

 
/*X*/
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU : 
*  Effectue les operation de terminaison de la tache
*
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void ec_sortir( void)

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
*
-----------------------------------------------------------------------------------------*/
{
   static char *version = "$Id: etdp_cfg.c,v 1.18 2021/06/14 17:23:20 devgfi Exp $ : ec_sortir" ;
   XZEXT_MASK * pl_Socket;
   XZEXT_MSG_SOCKET pl_message;

   /*A
    * Envoie du message d'arret aux taches protocoles 
    */
    
   for ( pl_Socket = vg_SockMask; pl_Socket != NULL; pl_Socket = pl_Socket->Suivant )
   {
      if( !strncmp(pl_Socket->adresse,XZEXC_ADR_SOCK_TELM, strlen(XZEXC_ADR_SOCK_TELM)) )
      {
         /* Construction du message d'arret */
            sprintf ( pl_message, "%s%s%s", XZEXC_DEMANDE_ARRET,
                                pl_Socket->adresse, XZEXC_FIN_MSG_SOCK );
                                
         /* Envoie de la demande d'arret */
         sem_wait(&vg_semaphore); 
         if ( write ( pl_Socket->desc_sock, pl_message, strlen (pl_message) ) == XDC_NOK )
         { 
            /* Ecriture trace impossible d'emettre une demande d'arret */
             XZST_03EcritureTrace(XZSTC_WARNING,
             				" Impossible d'emettre une demande d'arret." );
         }
         sem_post(&vg_semaphore); 

      }
   }
   
   /*A Attendre */
   sleep (XZEXC_ATTENTE_ARRET_TACHE);
   
   /*A
    * Fermeture de toutes les sockets 
    */
	 
   /* Pour toute les sockets du masque */
   for ( pl_Socket = vg_SockMask; pl_Socket != NULL; pl_Socket = pl_Socket->Suivant )
   {
      /* Si differente de la socket RTserver */
      if ( pl_Socket->desc_sock != vg_SockRT ) 
      {
         /* Fermeture de la socket courante */
         ex_fin_cnx ( pl_Socket->desc_sock, &vg_SockMask );
      }   
   }

   /* Fin de la tache */
   exit (0);
}



/*X*/
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU : 
*
*	Creation de liste des destination
*
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static int ec_creer_liste_Destination_pmv ( XDY_Eqt va_PMV, XDY_Mot va_Destination, 
					XDY_Octet va_TypeDestination, XDY_Mot va_zdp,
					int 		va_NumAppel )

/*
* ARGUMENTS EN ENTREE :
*
*   aucun
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
*
*
* CONDITION D'UTILISATION
*
* FONCTION 
*
*	Creation de liste des destination
*
-----------------------------------------------------------------------------------------*/
{
 ETDP_CONFIG_PMV *    pl_PMV = NULL;
 XZEZT_Liste_Zdp *pl_NewZDP;
 XZEZT_Liste_Zdp *pl_zdp;
 XZEZT_Liste_Zdp *pl_liste_zdp;
 int vl_trouve;

  XZST_03EcritureTrace ( XZSTC_INFO,"ec_creer_liste_Destination_pmv : pmv=%d, dest=%d, type=%d, zdp=%d",
					va_PMV,va_Destination,va_TypeDestination,va_zdp);

  /*recherche la zdp*/
  vl_trouve = 0;
  for (pl_zdp = pg_ListeZone; pl_zdp!=NULL; pl_zdp = pl_zdp->suivant) {
    if (pl_zdp->zdp.Code_Zone == va_zdp) {
	vl_trouve=1;
	break;
    }
  }
  if (vl_trouve==0) {
	XZST_03EcritureTrace ( XZSTC_WARNING,"ZDP non trouve %d",va_zdp);
	return (XDC_OK);
  }

	/*recherche le PMV*/

   for ( pl_PMV = pg_ListePMV; pl_PMV != NULL; pl_PMV = pl_PMV->suivant  )
   {
      if ( va_PMV == pl_PMV->PMV.Numero )
	{
		XZST_03EcritureTrace ( XZSTC_DEBUG3,"ajout zdp %d pour pmv %d",
					va_zdp,va_PMV);
      		/*A Allocation memoire pour la nouvelle structure */
      		if ( ( pl_NewZDP = (XZEZT_Liste_Zdp *) malloc 
				( sizeof(XZEZT_Liste_Zdp) ) ) == NULL)
      		{
       		  /* Ecriture trace */ 
	  		XZST_03EcritureTrace ( XZSTC_WARNING, 
				" OUT: ec_creer_liste_Destination_pmv Allocation memoire impossible");
       		  return XDC_NOK;
      		}
      		memcpy ( &(pl_NewZDP->zdp), pl_zdp, sizeof (XDY_ZDP) );
		pl_NewZDP->suivant= NULL;

		switch (va_TypeDestination) {
			case XDC_DESTINATION_PROCHE :
					pl_PMV->Destination=va_Destination;
					pl_NewZDP->suivant = pl_PMV->Graphe_Destination;
					pl_PMV->Graphe_Destination=pl_NewZDP;

					for (pl_liste_zdp=pl_PMV->Graphe_Destination;
						pl_liste_zdp!=NULL;
						pl_liste_zdp = pl_liste_zdp->suivant) {
						XZST_03EcritureTrace ( XZSTC_DEBUG3,
								"%d %d",
								va_PMV,
								pl_liste_zdp->zdp.Code_Zone);
					}
					break;

			case XDC_DESTINATION_LOINTAINE :
					pl_PMV->DestinationLointaine=va_Destination;
					pl_NewZDP->suivant = pl_PMV->Graphe_DestinationLointaine;
					pl_PMV->Graphe_DestinationLointaine=pl_NewZDP;
					break;

			case XDC_DESTINATION_NOEUD :	
					pl_PMV->DestinationNoeud=va_Destination;
					pl_NewZDP->suivant = pl_PMV->Graphe_DestinationNoeud;
					pl_PMV->Graphe_DestinationNoeud=pl_NewZDP;
					break;
		}
	  return (XDC_OK);
	}
   }
  return (XDC_OK);
}

/******************************************************/
/*X*/
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU :
*
*	Creation de liste des destination
*
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/
static int ec_creer_liste_Destination_pmva ( XDY_Eqt va_PMVA, XDY_Mot va_Destination,
					XDY_Octet va_TypeDestination, XDY_Mot va_zdp,
					int 		va_NumAppel )
/*
* ARGUMENTS EN ENTREE :
*
*   aucun
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR :
*
*
* CONDITION D'UTILISATION
*
* FONCTION
*
*	Creation de liste des destination
*
-----------------------------------------------------------------------------------------*/
{
 ETDP_CONFIG_PMVA *    pl_PMVA = NULL;
 XZEZT_Liste_Zdp *pl_NewZDP;
 XZEZT_Liste_Zdp *pl_zdp;
 XZEZT_Liste_Zdp *pl_liste_zdp;
 int vl_trouve;

  XZST_03EcritureTrace ( XZSTC_DEBUG1,"ec_creer_liste_Destination_pmva : pmva=%d, dest=%d, type=%d, zdp=%d",
					va_PMVA,va_Destination,va_TypeDestination,va_zdp);

  /*recherche la zdp*/
  vl_trouve = 0;
  for (pl_zdp = pg_ListeZone; pl_zdp!=NULL; pl_zdp = pl_zdp->suivant) {
    if (pl_zdp->zdp.Code_Zone == va_zdp) {
	vl_trouve=1;
	break;
    }
  }
  if (vl_trouve==0) {
	XZST_03EcritureTrace ( XZSTC_WARNING,"ZDP non trouve %d",va_zdp);
	return (XDC_NOK);
  }

	/*recherche le PMVA*/

   for ( pl_PMVA = pg_ListePMVA; pl_PMVA != NULL; pl_PMVA = pl_PMVA->suivant  )
   {
      if ( va_PMVA == pl_PMVA->PMVA.Numero )
	{
		XZST_03EcritureTrace ( XZSTC_DEBUG1,"ajout zdp %d pour pmva %d",
					va_zdp,va_PMVA);
      		/*A Allocation memoire pour la nouvelle structure */
      		if ( ( pl_NewZDP = (XZEZT_Liste_Zdp *) malloc
				( sizeof(XZEZT_Liste_Zdp) ) ) == NULL)
      		{
       		  /* Ecriture trace */
	  		XZST_03EcritureTrace ( XZSTC_WARNING,
				" OUT: ec_creer_liste_Destination_pmva Allocation memoire impossible");
       		  return XDC_NOK;
      		}
      		memcpy ( &(pl_NewZDP->zdp), pl_zdp, sizeof (XDY_ZDP) );
		pl_NewZDP->suivant= NULL;

		switch (va_TypeDestination) {
			case XDC_DEST_UN_PROCHE :
					pl_PMVA->Destination1_proche = va_Destination;
					pl_NewZDP->suivant = pl_PMVA->Graphe_Destination1;
					pl_PMVA->Graphe_Destination1 = pl_NewZDP;

					for (pl_liste_zdp = pl_PMVA->Graphe_Destination1;
						pl_liste_zdp != NULL;
						pl_liste_zdp = pl_liste_zdp->suivant) {
						XZST_03EcritureTrace ( XZSTC_DEBUG1, "UN PROCHE PMVA = %d - zone %d",
										va_PMVA,
										pl_liste_zdp->zdp.Code_Zone);
					}
					break;

			case XDC_DEST_UN_LOIN :
					pl_PMVA->Destination1_loin = va_Destination;
					pl_NewZDP->suivant = pl_PMVA->Graphe_Destination1Lointaine;
					pl_PMVA->Graphe_Destination1Lointaine = pl_NewZDP;
					for (pl_liste_zdp = pl_PMVA->Graphe_Destination1Lointaine;
						pl_liste_zdp != NULL;
						pl_liste_zdp = pl_liste_zdp->suivant) {
						XZST_03EcritureTrace ( XZSTC_DEBUG1, "UN LOIN PMVA = %d - zone %d",
										va_PMVA,
										pl_liste_zdp->zdp.Code_Zone);
					}
					break;

			case XDC_DEST_DEUX_PROCHE :
					pl_PMVA->Destination2_proche = va_Destination;
					pl_NewZDP->suivant = pl_PMVA->Graphe_Destination2;
					pl_PMVA->Graphe_Destination2 = pl_NewZDP;

					for (pl_liste_zdp=pl_PMVA->Graphe_Destination2;
						pl_liste_zdp != NULL;
						pl_liste_zdp = pl_liste_zdp->suivant) {
						XZST_03EcritureTrace ( XZSTC_DEBUG1, "DEUX PROCHE PMVA = %d - zone %d",
													va_PMVA,
													pl_liste_zdp->zdp.Code_Zone);
					}
					break;

			case XDC_DEST_DEUX_LOIN :
					pl_PMVA->Destination2_loin = va_Destination;
					pl_NewZDP->suivant = pl_PMVA->Graphe_Destination2Lointaine;
					pl_PMVA->Graphe_Destination2Lointaine = pl_NewZDP;
					for (pl_liste_zdp = pl_PMVA->Graphe_Destination2Lointaine;
						pl_liste_zdp != NULL;
						pl_liste_zdp = pl_liste_zdp->suivant) {
						XZST_03EcritureTrace ( XZSTC_DEBUG1, "DEUX LOIN PMVA = %d - zone %d",
										va_PMVA,
										pl_liste_zdp->zdp.Code_Zone);
					}
					break;
		}
	  return (XDC_OK);
	}
   }
  return (XDC_OK);
}
/******************************************************/
