/*E*/
/*  Fichier : $Id: exa0_cfg.c,v 1.5 1997/06/25 12:49:47 gesconf Exp $      Release : $Revision: 1.5 $        Date : $Date: 1997/06/25 12:49:47 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEXA0 * FICHIER exa0_cfg.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Module de configuration de la tache TEXA0, protocole 
*   de la gtc Nice
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	22 Nov 1994	: Creation
* Mismer.D	version 1.2	26 Jan 1995	:
* Fontaine.C	version 1.3	23 Mai 1995	: Reprise entete, historique
* Mismer.D      version 1.4     24 Avr 1997     : Ajout alerte monaco (DEM/1421)
* Mismer.D      version 1.5     23 Jui 1997     : Ajout alerte monaco suite (DEM/1421)
*
------------------------------------------------------*/

/* fichiers inclus */

#include "exa0_cfg.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: exa0_cfg.c,v 1.5 1997/06/25 12:49:47 gesconf Exp $ : exa0_cfg" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Cette fonction effectue la lecture du fichier XDF_Config_GTCNice_<NomMachine>
*  et met a jour la liste des structure de donnees GTCNice;
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int ea_lire_config_GTCNice	( XDY_NomMachine		pa_NomMachine )
			  
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
   static char *version = "$Id: exa0_cfg.c,v 1.5 1997/06/25 12:49:47 gesconf Exp $ : ea_lire_config_GTCNice " ;

   int				vl_ValRet = XDC_OK;
   int				vl_Fd;
   char 			pl_PathEtFic[XDC_PATH_ABS_SIZE];
   XDY_Texte			pl_LigneFich;
   EXA0_DONNEES_Tube		vl_DonneesTube = {0},
   				*pl_DonCrtTube,
   				*pl_DonSupTube;
   EXA0_DONNEES_Echangeur	vl_DonneesEch  = {0},
   				*pl_DonCrtEch,
   				*pl_DonSupEch;
   
      /*A Ouverture du fichier de configuration GTCNice */
      /* Construction du nom du fichier XDF_Config_GTCNice_<NomMachine>  */
      sprintf ( pl_PathEtFic, "%s/%s_%s", XZEXC_PATH_CONFIG, XDF_Config_GTCNice, pa_NomMachine );
      
      /* Ouverture du fichier XDF_Config_GTCNice_<NomMachine>  */
      if ( (vl_Fd = open (pl_PathEtFic, O_RDONLY)) <= 0 )
      {
         /* Ecriture trace absence fichier */
         XZST_03EcritureTrace( XZSTC_WARNING, " Manque fichier %s. \n", pl_PathEtFic ); 
         /* Sortie de la fonction */
         return ( XDC_NOK );
      }
      
      /*A Suppression des structures de donnees Tube */
      for ( pl_DonCrtTube = pl_DonSupTube = pg_debutTube;
            pl_DonCrtTube != NULL; 
            pl_DonSupTube = pl_DonCrtTube )
      {
      	 /* Liberation memoire */
         pl_DonCrtTube = pl_DonCrtTube->Suivant;
         free ( pl_DonSupTube );
      }
      pg_debutTube = NULL;
 
      /*A Suppression des structures de donnees Echangeur */
      for ( pl_DonCrtEch = pl_DonSupEch = pg_debutEchangeur;
            pl_DonCrtEch != NULL; 
            pl_DonSupEch = pl_DonCrtEch )
      {
      	 /* Liberation memoire */
         pl_DonCrtEch = pl_DonCrtEch->Suivant;
         free ( pl_DonSupEch );
      }
      pg_debutEchangeur = NULL;
      
      /*A Lecture de chaque ligne du fichier */
      while ( ex_LireLigneFich ( vl_Fd, pl_LigneFich, sizeof ( pl_LigneFich ) ) != XZEXC_EOF ) 
      {
          /* Lire ligne suivante si c'est une ligne de commentaire */
          if ( *pl_LigneFich != XDF_COMMENTAIRE )
          {
             /* Si il s'agit d'un tube */
             if ( *pl_LigneFich == 'T' )
             {
                /*B Extraire les parametres*/
                if ( sscanf ( pl_LigneFich, "T %hd %hd %hd %hd %hd %hd %hd %hd %hd %hd %hd ",  
					&vl_DonneesTube.Numero,
					&vl_DonneesTube.AdresseEtat,
					&vl_DonneesTube.AdresseCommande,
					&vl_DonneesTube.AdresseDiscord,
					&vl_DonneesTube.AdresseExtincteur,
					&vl_DonneesTube.AdresseLit,
					&vl_DonneesTube.AdresseOPCO,
					&vl_DonneesTube.AdresseTelealarme,
					&vl_DonneesTube.AdresseGabarit,
					&vl_DonneesTube.AdresseVitesse,
					&vl_DonneesTube.AdresseNiche ) == 11 )
	        {
                   /*B Inserer dans la liste des structures Tube */
                   ea_ins_liste_Tube ( &vl_DonneesTube, 0 );
                }
             }

             /* Si il s'agit d'un echangeur */
             if ( *pl_LigneFich == 'E' )
             {
                /*B Extraire les parametres*/
                if ( sscanf ( pl_LigneFich, "E %hd %hd %hd %hd %hd ",  
					&vl_DonneesEch.NumeroDemiEch,
					&vl_DonneesEch.AdresseEtat,
					&vl_DonneesEch.AdresseCommande,
					&vl_DonneesEch.AdresseDiscord,
					&vl_DonneesEch.AdresseTelealarme ) == 5 )
	        {
                   /*B Inserer dans la liste des structures Echangeur */
                   ea_ins_liste_Ech ( &vl_DonneesEch, 0 );
                }
             }
                
             /* Si il s'agit de la config Adresse  */
             if ( *pl_LigneFich == 'G' )
             {
                if ( sscanf ( pl_LigneFich,"G %hd\t%s\t%hd\t%hd\t%hd\t%hd\t%hd\t%hd\t%hd\t%hd\t%hd\t%hd\t%hd\t%hd\t%hd\t%hd\t%hd\t%hd\t%hd\t%hd\t%hd\t%hd\t\n#\0",
					&vg_ConfigAdresse.Numero,
					vg_ConfigAdresse.NomServeur,
					&vg_ConfigAdresse.LongueurITOR,
					&vg_ConfigAdresse.AdresseITOR,
					&vg_ConfigAdresse.LongueurETOR,
					&vg_ConfigAdresse.AdresseETOR,
					&vg_ConfigAdresse.LongTelealarme,
					&vg_ConfigAdresse.AdresseTelealarme,
					&vg_ConfigAdresse.LongTelecomEch,
					&vg_ConfigAdresse.AdresseTelecomEch,
					&vg_ConfigAdresse.LongTelecomTub,
					&vg_ConfigAdresse.AdresseTelecomTub,
					&vg_ConfigAdresse.LongTelesignEch,
					&vg_ConfigAdresse.AdresseTelesignEch,
					&vg_ConfigAdresse.LongTelesignTub,
					&vg_ConfigAdresse.AdresseTelesignTub,
					&vg_ConfigAdresse.LongSeqEch,
					&vg_ConfigAdresse.AdresseSeqEch,
					&vg_ConfigAdresse.LongSeqTub,
					&vg_ConfigAdresse.AdresseSeqTub,
					&vg_ConfigAdresse.LongAlerteTub,
					&vg_ConfigAdresse.AdresseAlerteTub ) != 22 )
	        {
                   XZST_03EcritureTrace ( XZSTC_WARNING, "Config adresse automate GTC de NIce incorrect " ); 
                }
             }
         }
      }
      /*A Fermeture du fichier XDF_Config_GTCNice_<NomMachine>  */
      XZSS_13FermerFichier( vl_Fd );
      
     
      /* Ecriture trace de la config pour debug */
      for ( pl_DonCrtTube = pg_debutTube; pl_DonCrtTube != NULL; pl_DonCrtTube = pl_DonCrtTube->Suivant )
      {
          XZST_03EcritureTrace ( XZSTC_WARNING, "Config Tube: %hd %hd %hd %hd %hd %hd %hd %hd ",  
					vl_DonneesTube.Numero,
					vl_DonneesTube.AdresseEtat,
					vl_DonneesTube.AdresseCommande,
					vl_DonneesTube.AdresseDiscord,
					vl_DonneesTube.AdresseExtincteur,
					vl_DonneesTube.AdresseLit,
					vl_DonneesTube.AdresseOPCO,
					vl_DonneesTube.AdresseTelealarme,
					vl_DonneesTube.AdresseGabarit,
					vl_DonneesTube.AdresseVitesse,
					vl_DonneesTube.AdresseNiche );				
      }

      /* Ecriture trace de la config pour debug */
      for ( pl_DonCrtEch = pg_debutEchangeur; pl_DonCrtEch != NULL; pl_DonCrtEch = pl_DonCrtEch->Suivant )
      {
          XZST_03EcritureTrace ( XZSTC_WARNING, "Config Ech: %hd %hd %hd %hd %hd ",  
					vl_DonneesEch.NumeroDemiEch,
					vl_DonneesEch.AdresseEtat,
					vl_DonneesEch.AdresseCommande,
					vl_DonneesEch.AdresseDiscord,
					vl_DonneesEch.AdresseTelealarme );				
      }
      /* Retourne compte rendu OK */
      return ( vl_ValRet );
}



