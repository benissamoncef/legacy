/*E*/
/*  Fichier : $Id: eqtx_cfg.c,v 1.7 2011/02/25 16:47:17 gesconf Exp $      Release : $Revision: 1.7 $        Date : $Date: 2011/02/25 16:47:17 $
-------------------------------------------------------------------------------------------
* GTIE / GRANIOU *  PROJET MIGRAZUR 
-------------------------------------------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
-------------------------------------------------------------------------------------------
* MODULE TERAU * FICHIER eqtx_cfg.c
-------------------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module d'init et de reinit, de controle et d'exit de la tache TEQTX
*
-------------------------------------------------------------------------------------------
* HISTORIQUE :
*
* Orengo.A	version 1.1	29 Oct 1996	: Creation
* Orengo.A	version 1.2	07 Nov 1996	: Modification du nom des equipements
* Mismer.D	version 1.4	13 Jan 1997	: Modification protocole (DEM/1393)
* JMG   13/11/07        : ajout site de gestion DEM715
* PNI	08/07/09	: Memorisation du passwd DEM898 v1.6
* JPL	25/02/11	: Migration architecture HP ia64 (DEM 975) : adresses des champs 'scanf'  1.7
-----------------------------------------------------------------------------------------*/

/* Fichiers inclus */
/* --------------- */
#include <xzsem.h>
#include	<xdy.h>
#include	<xdc.h>
#include	<xzst.h>
#include	<xzat.h>

#include 	"ex_msok.h"
#include 	"ex_mdon.h"
#include 	"ex_mrtf.h"

#include	"eqtx_don.h"
#include	"eqtx_cfg.h"
#include	"eqtx_dir.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* Declaration de variables locales */
/* -------------------------------- */

static char *version = "$Id: eqtx_cfg.c,v 1.7 2011/02/25 16:47:17 gesconf Exp $ : eqtx_cfg" ;
EQTX_DONNEES_SYSTQTX vg_donnees_systeme_qtx;
char                  vg_idpassw[18] = "";

/* declaration de fonctions internes */

/* Definition de fonctions externes */
/* -------------------------------- */

static int lecture_fichier_config_qtx( char *);
static int lecture_fichier_config_alarmes_alertes( char *, int);
void ec_sortir( void);

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
   static char *version = "$Id: eqtx_cfg.c,v 1.7 2011/02/25 16:47:17 gesconf Exp $ : ec_controle" ;
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
  	     vg_donnees_systeme_qtx.Mode_Fonct = XZEXC_MODE_NORMAL;
  	     ex_RecopieFichierMsg( pa_NomMachine);
  	  }
  	  if( strstr( pl_Msg, XZEXC_MODE_FONCT_ISOLE ) != NULL )
  	  {
  	     /*A Maj de la variable en question */
  	     vg_donnees_systeme_qtx.Mode_Fonct = XZEXC_MODE_ISOLE;
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
   static char *version = "$Id: eqtx_cfg.c,v 1.7 2011/02/25 16:47:17 gesconf Exp $ : ec_init" ;
   char vl_nom_fichier[256];
   XZEXT_MASK *pl_Socket;
   XZEXT_ADR_SOCK pl_TacheTrt;
   XZEXT_ADR_SOCK pl_TacheProt;
   int vl_SockProt = 0;
   int vl_TabType[NB_EQT_MAX_TEQTX];
   int i, j;
   int				vl_retcode = 0; 

   /* Boucle pour la RAZ de la table de donnees */
   
   for(i=0 ;i!=NB_EQT_MAX_TEQTX; i++)
   {
      /* Invalidation des donnees */
      
      vg_donnees_systeme_qtx.DonneesEqt[i].ChampValide=Non_valide;
      vg_donnees_systeme_qtx.DonneesEqt[i].LSMaitreValide=Non_valide;
      vl_TabType[i] = 0;
  }

   /* Lecture des fichiers de configuration et remplissage des structures de donnees */
   
   sprintf( vl_nom_fichier, "%s/%s_%s", XZEXC_PATH_CONFIG,
				XDF_Config_EQT_GEN_LCR, vg_donnees_systeme_qtx.NomMachine);
   
   /* Si il y a probleme de configuration */
   
   if( lecture_fichier_config_qtx( vl_nom_fichier)==XDC_NOK)
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

   /*A Ouverture du fichier d'alarme */
   
   ex_OuvertureFichierMsg(vg_donnees_systeme_qtx.NomMachine);

   /* Boucle sur l'ensemble des equipements */
   
   for( i=0; i!=NB_EQT_MAX_TEQTX; i++)
   {
      /* Si les donnees de la table sont valides */
      
      if( vg_donnees_systeme_qtx.DonneesEqt[i].ChampValide== Valide)
      {
         for ( j=0; j!=NB_EQT_MAX_TEQTX; j++)
         {
            if ( vl_TabType[j] == vg_donnees_systeme_qtx.DonneesEqt[i].TypeEqt )
               break;
         }
         if ( j == NB_EQT_MAX_TEQTX )
         {
            /* Alors */
            /* Appel xzat26_Init_Etats_Eqts pour Initialisation
   						des etats equipement (HS ou Actif) */

            xzat26_Init_Etats_Eqts( vg_donnees_systeme_qtx.DonneesEqt[i].TypeEqt, XDC_ETAT_TOUS_EQT,
							vg_donnees_systeme_qtx.NomMachine );
         
            /* Lecture du fichier config alarmes/alertes */
         
            sprintf( vl_nom_fichier, "%s/%s_%s", XZEXC_PATH_CONFIG, XDF_CONF_ALARME_ALERTE,
					vg_donnees_systeme_qtx.DonneesEqt[i].LibelleNomEqt);
					
            lecture_fichier_config_alarmes_alertes( vl_nom_fichier, i);
            vl_TabType[i] = vg_donnees_systeme_qtx.DonneesEqt[i].TypeEqt;
         }
         else
         {
            for ( j=0; j!=i; j++)
            {
               if ( vg_donnees_systeme_qtx.DonneesEqt[i].TypeEqt == vg_donnees_systeme_qtx.DonneesEqt[j].TypeEqt )
               {
                  vg_donnees_systeme_qtx.DonneesEqt[i].ptr_alarmes_alertes = vg_donnees_systeme_qtx.DonneesEqt[j].ptr_alarmes_alertes;
                  break;
               }
            }
         }
      }
      /* Finsi */
   }
   /*A
    * Connexion socket en mode client pour communiquer avec la tache TELEi
    */
   /*A Pour le systeme video rechercher si la tache protocole associee existe */
   
   /* Boucle sur l'ensemble des taches protocoles telei */
   
   for( i=0; i!=NB_EQT_MAX_TEQTX; i++)
   {
      /* Si les donnees de la table sont valides */
      
      if( vg_donnees_systeme_qtx.DonneesEqt[i].ChampValide== Valide)
      {
         /* Alors */
         /* Construction du nom de la tache protocole telei */
   
         sprintf( pl_TacheProt, "%s%d", XZEXC_ADR_SOCK_TELE,
					vg_donnees_systeme_qtx.DonneesEqt[i].NoLSEsclave);
         sprintf( pl_TacheTrt, "%s%d", XZEXC_ADR_SOCK_QTXTELE,
					vg_donnees_systeme_qtx.DonneesEqt[i].NoLSEsclave);
   
         /*A Recherche sur tout le masque des socket si la socket existe */
   
         for( pl_Socket = vg_SockMask; pl_Socket != NULL; pl_Socket = pl_Socket->Suivant )
         {
            if( !strcmp( pl_Socket->adresse, pl_TacheProt ) )
            {
               /* Mise a jour du numero de socket */
         
               vg_donnees_systeme_qtx.DonneesEqt[i].SocketEsclave = pl_Socket->desc_sock;
         
               /*B Arret: la socket avec la tache existe */
               break;
            }
         }
      
         /*A Si la socket avec la tache telei n'existe pas */
   
         if( pl_Socket == NULL )
         {
            /*A Appel de ex_cnx_client pour etablir la liaison avec la tache protocole */
      
            if( ex_cnx_client( pl_TacheTrt, pl_TacheProt, &vg_SockMask,
         							&vl_SockProt ) != XDC_OK )
            {
	       /*B Ecriture Trace */
	 
               XZST_03EcritureTrace( XZSTC_FATAL,
         		"Impossible de creer la connexion client avec %s .", pl_TacheProt);
            }
            else
            {
	       /* Memorisation du numero de socket */
	 
	       vg_donnees_systeme_qtx.DonneesEqt[i].SocketEsclave = vl_SockProt;
	    }
         }	
      }
      /* Finsi */
   }
   
   /*A
    * Connexion socket en mode client pour communiquer avec la tache TELMi
    */
   /*A Pour le systeme video rechercher si la tache protocole associee existe */
   
   for( i=0; i!=NB_EQT_MAX_TEQTX; i++)
   {
      /* Si les donnees de la table sont valides */
      
      if( vg_donnees_systeme_qtx.DonneesEqt[i].ChampValide== Valide &&
      			vg_donnees_systeme_qtx.DonneesEqt[i].LSMaitreValide== Valide)
     {
         /* Alors */
         /* Construction du nom de la tache protocole telmi */
   
         sprintf( pl_TacheProt, "%s%d", XZEXC_ADR_SOCK_TELM,
        				vg_donnees_systeme_qtx.DonneesEqt[i].NoLSMaitre);
         sprintf( pl_TacheTrt, "%s%d", XZEXC_ADR_SOCK_QTXTELM,
         				vg_donnees_systeme_qtx.DonneesEqt[i].NoLSMaitre);
   
         /*A Recherche sur tout le masque des socket si la socket exite */
   
         for( pl_Socket = vg_SockMask; pl_Socket != NULL; pl_Socket = pl_Socket->Suivant )
         {
            if( !strcmp ( pl_Socket->adresse, pl_TacheProt ) )
            {
               /* Mise a jour du numero de socket */
         
               vg_donnees_systeme_qtx.DonneesEqt[i].SocketMaitre = pl_Socket->desc_sock;
         
              /*B Arret: la socket avec la tache existe */
              break;
            }
         }
      
         /*A Si la socket avec la tache telmi n'existe pas */
         
         if( pl_Socket == NULL )
         {
            /*A Appel de ex_cnx_client pour etablir la liaison avec la tache protocole */
      
            if( ex_cnx_client( pl_TacheTrt,pl_TacheProt,&vg_SockMask,
            							&vl_SockProt ) != XDC_OK)
            {
	       /*B Ecriture Trace */
	       
               XZST_03EcritureTrace( XZSTC_FATAL,
         	     "Impossible de creer la connexion client avec %s .", pl_TacheProt );	
            }
            else
            {
	       /* Memorisation du numero de socket */
	       
	       vg_donnees_systeme_qtx.DonneesEqt[i].SocketMaitre = vl_SockProt;
	    }
	 }
      }
      /* Finsi */	
   }
   return (XDC_OK);
}

/*X*/
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU : 
*
*	Lit le fichier config des alarmes et alertes et remplit la structure associee
*
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static int lecture_fichier_config_qtx(char *va_nom_fichier)

/*
* ARGUMENTS EN ENTREE :
*
*	va_nom_fichier : Nom du fichier config qtx
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
*	Lit le fichier config qtx et remplit la structure associee
*
-----------------------------------------------------------------------------------------*/
{
   static char *version = "$Id: eqtx_cfg.c,v 1.7 2011/02/25 16:47:17 gesconf Exp $ : lecture_fichier_config_qtx";
   char vl_ligne_fichier[512];
   int vl_nb_elements_lus;
   int vl_Numero;
   int vl_TypeEqt;
   int vl_NoLSMaitre;
   int vl_NoLSEsclave;
   int vl_NoSiteGestion;
   FILE *fp;
   int i=0;
   
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
            
            if( i==NB_EQT_MAX_TEQTX)
            {
               /* Alors */
               /* Tracer l'erreur et quitter */
               
               XZST_03EcritureTrace(XZSTC_WARNING,
                " Le nombre maximum d'equipements gerables par la tache TEQTX est atteint");
              
               return(XDC_NOK);
            }
               
            /* Recuperer les elements de la ligne */
            
            vl_nb_elements_lus=sscanf(vl_ligne_fichier, XZEXC_FORMAT_CONF_EQTX,
               &vl_Numero, &vl_TypeEqt, vg_donnees_systeme_qtx.DonneesEqt[i].LibelleNomEqt,
                   &vg_donnees_systeme_qtx.DonneesEqt[i].code_systeme_gere, 
                      vg_donnees_systeme_qtx.DonneesEqt[i].NomST,
                        vg_donnees_systeme_qtx.DonneesEqt[i].AdresseRGS, &vl_NoLSMaitre,
									    &vl_NoLSEsclave,
									    &vl_NoSiteGestion);
		            
            /* Si nombre d'elements lus n'est pas OK */
            
            if( vl_nb_elements_lus!=9)
            {
               /* Alors */
               /* Tracer l'erreur et quitter */
               
               XZST_03EcritureTrace(XZSTC_WARNING,
                   " Recuperation des donnees fichier '%s' impossible.", va_nom_fichier);
              
               return(XDC_NOK);
            }
            /* Finsi */
            
            /* Mise dans le structure des datas lues */
            
            vg_donnees_systeme_qtx.DonneesEqt[i].Numero= (XDY_Eqt )vl_Numero;
            vg_donnees_systeme_qtx.DonneesEqt[i].TypeEqt= (XDY_TypeEqt )vl_TypeEqt;
            vg_donnees_systeme_qtx.DonneesEqt[i].NoLSMaitre= (XDY_Mot )vl_NoLSMaitre;
            vg_donnees_systeme_qtx.DonneesEqt[i].NoLSEsclave= (XDY_Mot )vl_NoLSEsclave;
            vg_donnees_systeme_qtx.DonneesEqt[i].SiteGestion= (XDY_District )vl_NoSiteGestion;
            
            /* Validation des donnees lues */
            
            vg_donnees_systeme_qtx.DonneesEqt[i].ChampValide=Valide;
            vg_donnees_systeme_qtx.DonneesEqt[i].Etat_Service = 0;
            
            /* Si la liaison LCR maitre existe */
            
            if( vl_NoLSMaitre != 0)
            {
               /* Alors */
               /* Valider la liaison Maitre */
               
               vg_donnees_systeme_qtx.DonneesEqt[i].LSMaitreValide=Valide;
            }
            
            i++;
         }
         /* Finsi */
      }
      /* Fin Tant */
   }
   else
   {
      /* Sinon */
      
      XZST_03EcritureTrace(XZSTC_WARNING,
      				" Acces au fichier '%s' impossible.", va_nom_fichier);
      return(XDC_NOK);
   }
   /* Finsi */
   
   return(XDC_OK);
}
   
/*X*/
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU : 
*
*	Lit le fichier config qtx et remplit la structure associee
*
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static int lecture_fichier_config_alarmes_alertes( char *va_nom_fichier,
								int va_indice_table_eqt)

/*
* ARGUMENTS EN ENTREE :
*
*	va_nom_fichier		:	Nom du fichier config alarmes alerte
*	va_indice_table_eqt	:	Indice de l'equipement dans la table des donnees
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
*	Lit le fichier config alarmes alerte et remplit la structure associee
*
-----------------------------------------------------------------------------------------*/
{
   static char *version = "$Id: eqtx_cfg.c,v 1.7 2011/02/25 16:47:17 gesconf Exp $ : lecture_fichier_config_alarmes_alertes";
   char vl_ligne_fichier[TAILLE_LIGNE_FICHIER_CONFIG_ALARME_ALERTE+1];
   int vl_nb_elements_lus;
   int vl_code_alarme_alerte;
   int vl_numero_alerte;
   int vl_numero_alarme;
   int vl_num_ligne=0;
   char *pl_Texte_Alerte;
   XDY_TexteAlerte vl_Texte_Alerte;
   int vl_lg_Texte_Alerte;
   FILE *fp;
   EQTX_ALARMES_ALERTES_SYSTQTX *pl_alarmes_alertes;
   
   /* Allocation de la memoire dynamique */
   
   pl_alarmes_alertes=(EQTX_ALARMES_ALERTES_SYSTQTX *)calloc(
		sizeof(EQTX_ALARMES_ALERTES_SYSTQTX), NB_MAX_ALARME_ALERTE_FICHIER_CONFIG);

   /* Si l'allocation n'a pas reussi */
   
   if( pl_alarmes_alertes==NULL)
   {
      /* Alors */
      /* Tracer l'erreur et quitter */
      
      XZST_03EcritureTrace(XZSTC_WARNING,
       " Allocation memoire pour les donnees du fichier '%s' impossible.", va_nom_fichier);
      
      return(XDC_NOK);
   }
   /* Finsi */
      
   /* Si le fichier existe */
   
   if(( fp=fopen(va_nom_fichier, "r"))!=NULL)
   {
      /* Alors */
      /* Tant que le fichier n'est pas completement lu */
      
      while( fgets(vl_ligne_fichier, TAILLE_LIGNE_FICHIER_CONFIG_ALARME_ALERTE, fp)!=NULL)
      {
         /* Incrementer le numero de ligne */
         
         vl_num_ligne++;
         
         /* Si Le caractere lu n'est pas un '#' */
         
         if(vl_ligne_fichier[0]!= XDF_COMMENTAIRE)
         {
            /* Alors */
            /* Recuperer les elements de la ligne */
            /* Seuls les 3 premiers sont exploitables ( Du a la fonction sscanf ) */
            
            vl_nb_elements_lus=sscanf( vl_ligne_fichier, "%d\t%d\t%d\t%s",
					&vl_code_alarme_alerte, &vl_numero_alerte,
						&vl_numero_alarme, vl_Texte_Alerte);
		            
            /* Si le numero de l'alarme n'est pas dans l'intervale autorise */
            
            if( vl_code_alarme_alerte<0 || 
            		    vl_code_alarme_alerte>(NB_MAX_ALARME_ALERTE_FICHIER_CONFIG-1))
            {
               /* Alors */
               /* Tracer l'erreur et quitter */
               
               XZST_03EcritureTrace(XZSTC_WARNING,
                   " Recuperation des donnees fichier '%s' impossible. Ligne %d",
							va_nom_fichier, vl_num_ligne);
               
               free(( void *)pl_alarmes_alertes);
               fclose(fp);
              
               return(XDC_NOK);
            }
            /* Finsi */
            /* Si nombre d'elements lus est egal a 4 */
            
            if( vl_nb_elements_lus==4)
            {
               /* Alors */
               /* Recuperation du libelle d'alerte */
                      
               pl_Texte_Alerte=strrchr(vl_ligne_fichier, '\t')+1;
            
               /* Pour supprimer la fin de ligne ( LF ) */
            
               vl_lg_Texte_Alerte = (int) strlen(pl_Texte_Alerte);
               *(pl_Texte_Alerte+vl_lg_Texte_Alerte-1)='\0';
               strcpy( (char *)(pl_alarmes_alertes+vl_code_alarme_alerte)->Texte_Alerte,
            								pl_Texte_Alerte);
            }
            else
            {
               /* Sinon */
               /* Si nombre d'elements lus est egal a 3 */
            
               if( vl_nb_elements_lus==3)
               {
                  /* Alors */
                  /* Mettre le libelle alerte a vide */
                  
                  strcpy( (char *)(pl_alarmes_alertes+vl_code_alarme_alerte)->
						                  Texte_Alerte, "");
               }
               else
               {
                  /* Sinon */
                  /* Tracer l'erreur et quitter */
               
                  XZST_03EcritureTrace(XZSTC_WARNING,
                   " Recuperation des donnees fichier '%s' impossible. Ligne %d",
							va_nom_fichier, vl_num_ligne);
               
                  free(( void *)pl_alarmes_alertes);
                  fclose(fp);
              
                  return(XDC_NOK);
               }
               /* Finsi */
            }
            /* Finsi */
            /* Mise dans le structure des datas lues */
            
            (pl_alarmes_alertes+vl_code_alarme_alerte)->Numero_Alerte=vl_numero_alerte;
            (pl_alarmes_alertes+vl_code_alarme_alerte)->Numero_Alarme=vl_numero_alarme;
         }
         /* Finsi */
      }
      /* Fin Tant */
   }
   else
   {
      /* Sinon */
      
      XZST_03EcritureTrace(XZSTC_WARNING,
      				" Acces au fichier '%s' impossible.", va_nom_fichier);
      
      free(( void *)pl_alarmes_alertes);
      return(XDC_NOK);
   }
   /* Finsi */
   
   vg_donnees_systeme_qtx.DonneesEqt[va_indice_table_eqt].ptr_alarmes_alertes=
   									pl_alarmes_alertes;
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
*   Fermeture des sockets
*
-----------------------------------------------------------------------------------------*/
{
   static char *version = "$Id: eqtx_cfg.c,v 1.7 2011/02/25 16:47:17 gesconf Exp $ : ec_sortir" ;
   XZEXT_MASK * pl_Socket;
   XZEXT_ADR_SOCK pl_TacheProt;
   XZEXT_ADR_SOCK pl_TacheTrt;
   XZEXT_MSG_SOCKET pl_message;
   XDY_Datagroup pl_NomDG;
   int vl_resultat;

   /*A
    * Envoie du message d'arret aux taches protocoles 
    */
    
   for ( pl_Socket = vg_SockMask; pl_Socket != NULL; pl_Socket = pl_Socket->Suivant )
   {
      if( !strncmp(pl_Socket->adresse,XZEXC_ADR_SOCK_TELM, strlen(XZEXC_ADR_SOCK_TELM)) ||
           !strncmp(pl_Socket->adresse,XZEXC_ADR_SOCK_TELE, strlen(XZEXC_ADR_SOCK_TELE) ))
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
