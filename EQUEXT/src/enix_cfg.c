/*E*/
/*Fichier :  $Id: enix_cfg.c,v 1.4 2013/09/30 11:15:25 devgtie Exp $      Release : $Revision: 1.4 $        Date : $Date: 2013/09/30 11:15:25 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TENIX * FICHIER enix_cfg.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Module de configuration de la tache TENIX, gtc Nice
*
------------------------------------------------------
* HISTORIQUE : 
*
* Moulin.D  version 1.1 07 Janv 2002    : Creation
* Moulin.D  version 1.2 23 Avr  2002    : Installation PLate forme
* AAZ       version 1.3 07 Juin 2007    : Rénovation de la GTC DEM 664
* JBL		13 Oct 2008 : Suppression code inutilise (stockage interne alarmes et alertes)
* ADB       22 mars 2010 : FT 3448 - Gestion des etats liaisons
* ABE       26-03-2013 : gestion du retour à la normale avec ajout de scénarios pour le domaine exploitation
------------------------------------------------------*/
 
/* fichiers inclus */

#include "enix_cfg.h"

/* definitions de constantes */
#define ENIX_Wizcon "Config_Wizcon_GTCNice"

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "@(#)enix_cfg.c  1.3 06/07/07      : enix_cfg" ;

/* declaration de fonctions internes */
void et_rfic_derniere_seq ( int );

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
int et_lire_config_GTCNice  ( XDY_NomMachine        pa_NomMachine )
              
/*
* ARGUMENTS EN ENTREE :
*   pa_NomMachine   : Nom de la machine
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK  succes
*   XDC_NOK echec
*
* CONDITION D'UTILISATION
*   
*
* FONCTION 
*   
*
------------------------------------------------------*/
{
   static char *version = "@(#)enix_cfg.c   1.3 06/07/007      : et_lire_config_GTCNice " ;

   int                    vl_ValRet = XDC_OK;
   int                    vl_Fd;
   char                   pl_PathEtFic[XDC_PATH_ABS_SIZE];
   int					  vl_NumeroCourant = 0;
   XDY_Texte              pl_LigneFich;
   ENIX_DONNEES_Tube      vl_DonneesTube = {0},
                          *pl_DonCrtTube,
                          *pl_DonSupTube;
   ENIX_DONNEES_Echangeur vl_DonneesEch  = {0},
                          *pl_DonCrtEch,
                          *pl_DonSupEch;
   ENIX_DONNEES_SIG       vl_DonneesSIG  = {0},
                          *pl_DonCrtSIG,
                          *pl_DonSupSIG;
   ENIX_DONNEES_ECL       vl_DonneesECL  = {0},
                          *pl_DonCrtECL,
                          *pl_DonSupECL;
   ENIX_DONNEES_VEN       vl_DonneesVEN  = {0},
                          *pl_DonCrtVEN,
                          *pl_DonSupVEN;
   ENIX_DONNEES_ENR       vl_DonneesENR  = {0},
                          *pl_DonCrtENR,
                          *pl_DonSupENR;
   ENIX_DONNEES_INC       vl_DonneesINC  = {0},
                          *pl_DonCrtINC,
                          *pl_DonSupINC;
   ENIX_DONNEES_EXP       vl_DonneesEXP  = {0},
                          *pl_DonCrtEXP,
                          *pl_DonSupEXP;
   ENIX_DONNEES_SYS       vl_DonneesSYS  = {0},
                          *pl_DonCrtSYS,
                          *pl_DonSupSYS;
   ENIX_DONNEES_LTN       vl_DonneesLTN  = {0},
                          *pl_DonCrtLTN,
                          *pl_DonSupLTN;
   
      /*A Ouverture du fichier de configuration GTCNice */
      XZST_03EcritureTrace( XZSTC_INFO, "Config GTC Nice"); 
      /* Construction du nom du fichier XDF_Config_GTCNiceRenovee_<NomMachine>  */
      sprintf ( pl_PathEtFic, "%s/%s_%s", XZEXC_PATH_CONFIG, XDF_Config_GTCNiceRenovee, pa_NomMachine );
      
      /* Ouverture du fichier XDF_Config_GTCNiceRenovee_<NomMachine>  */
      if ( (vl_Fd = open (pl_PathEtFic, O_RDONLY)) <= 0 )
      {
         /* Ecriture trace absence fichier */
         XZST_03EcritureTrace( XZSTC_INFO, " Manque fichier %s. \n", pl_PathEtFic ); 
         /* Sortie de la fonction */
         return ( XDC_NOK );
      }
      
      /*A Suppression des structures de donnees Tube */
      for ( pl_DonCrtTube = pl_DonSupTube = pg_debutTube;
            pl_DonCrtTube != NULL; 
            pl_DonSupTube = pl_DonCrtTube )
      {
         /* Liberation memoire */
         et_sup_liste_Sequence    ( pl_DonCrtTube->ListeSequence );
         et_sup_liste_Equipement  ( pl_DonCrtTube->ListeEquipement );
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
         et_sup_liste_Sequence    ( pl_DonCrtEch->ListeSequence );
         et_sup_liste_Equipement  ( pl_DonCrtEch->ListeEquipement );
         pl_DonCrtEch = pl_DonCrtEch->Suivant;
         free ( pl_DonSupEch );
      }
      pg_debutEchangeur = NULL;
 
      /* AAZ */
      /*A Suppression des structures de donnees domaine signalisation */
      for ( pl_DonCrtSIG = pl_DonSupSIG = pg_debutSIG;
            pl_DonCrtSIG != NULL; 
            pl_DonSupSIG = pl_DonCrtSIG )
      {
         /* Liberation memoire */
         et_sup_liste_Sequence    ( pl_DonCrtSIG->ListeSequence );
         et_sup_liste_Equipement  ( pl_DonCrtSIG->ListeEquipement );
         pl_DonCrtSIG = pl_DonCrtSIG->Suivant;
         free ( pl_DonSupSIG );
      }
      pg_debutSIG = NULL;
 
      /*A Suppression des structures de donnees domaine eclairage */
      for ( pl_DonCrtECL = pl_DonSupECL = pg_debutECL;
            pl_DonCrtECL != NULL; 
            pl_DonSupECL = pl_DonCrtECL )
      {
         /* Liberation memoire */
         et_sup_liste_Sequence    ( pl_DonCrtECL->ListeSequence );
         et_sup_liste_Equipement  ( pl_DonCrtECL->ListeEquipement );
         pl_DonCrtECL = pl_DonCrtECL->Suivant;
         free ( pl_DonSupECL );
      }
      pg_debutECL = NULL;
 
      /*A Suppression des structures de donnees domaine ventilaion */
      for ( pl_DonCrtVEN = pl_DonSupVEN = pg_debutVEN;
            pl_DonCrtVEN != NULL; 
            pl_DonSupVEN = pl_DonCrtVEN )
      {
         /* Liberation memoire */
         et_sup_liste_Sequence    ( pl_DonCrtVEN->ListeSequence );
         et_sup_liste_Equipement  ( pl_DonCrtVEN->ListeEquipement );
         pl_DonCrtVEN = pl_DonCrtVEN->Suivant;
         free ( pl_DonSupVEN );
      }
      pg_debutVEN = NULL;

      /*A Suppression des structures de donnees domaine energie */
      for ( pl_DonCrtENR = pl_DonSupENR = pg_debutENR;
            pl_DonCrtENR != NULL; 
            pl_DonSupENR = pl_DonCrtENR )
      {
         /* Liberation memoire */
         et_sup_liste_Sequence    ( pl_DonCrtENR->ListeSequence );
         et_sup_liste_Equipement  ( pl_DonCrtENR->ListeEquipement );
         pl_DonCrtENR = pl_DonCrtENR->Suivant;
         free ( pl_DonSupENR );
      }
      pg_debutENR = NULL;

      /*A Suppression des structures de donnees domaine protection incendie */
      for ( pl_DonCrtINC = pl_DonSupINC = pg_debutINC;
            pl_DonCrtINC != NULL; 
            pl_DonSupINC = pl_DonCrtINC )
      {
         /* Liberation memoire */
         et_sup_liste_Sequence    ( pl_DonCrtINC->ListeSequence );
         et_sup_liste_Equipement  ( pl_DonCrtINC->ListeEquipement );
         pl_DonCrtINC = pl_DonCrtINC->Suivant;
         free ( pl_DonSupINC );
      }
      pg_debutINC = NULL;

      /*A Suppression des structures de donnees domaine Exploitation */
      for ( pl_DonCrtEXP = pl_DonSupEXP = pg_debutEXP;
            pl_DonCrtEXP != NULL; 
            pl_DonSupEXP = pl_DonCrtEXP )
      {
         /* Liberation memoire */
         et_sup_liste_Sequence    ( pl_DonCrtEXP->ListeSequence );
         et_sup_liste_Equipement  ( pl_DonCrtEXP->ListeEquipement );
         pl_DonCrtEXP = pl_DonCrtEXP->Suivant;
         free ( pl_DonSupEXP );
      }
      pg_debutEXP = NULL;
 
      /*A Suppression des structures de donnees Système */
      for ( pl_DonCrtSYS = pl_DonSupSYS = pg_debutSYS;
            pl_DonCrtSYS != NULL; 
            pl_DonSupSYS = pl_DonCrtSYS )
      {
         /* Liberation memoire */
         et_sup_liste_Equipement  ( pl_DonCrtSYS->ListeEquipement );
         pl_DonCrtSYS = pl_DonCrtSYS->Suivant;
         free ( pl_DonSupSYS );
      }
      pg_debutSYS = NULL;

      /*A Lecture de chaque ligne du fichier */
      while ( ex_LireLigneFich ( vl_Fd, pl_LigneFich, sizeof ( pl_LigneFich ) ) != XZEXC_EOF ) 
      {
          /* Lire ligne suivante si c'est une ligne de commentaire */
          if ( *pl_LigneFich != XDF_COMMENTAIRE )
          {
             /* S'il s'agit d'un tube */
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
                    vl_DonneesTube.Etat_Service   = 0;
                    vl_DonneesTube.ListeSequence     = NULL;
                    vl_DonneesTube.ListeEquipement  = NULL;
                    vl_DonneesTube.NbrEquipement     = 0;
                    et_ins_liste_Tube ( &vl_DonneesTube, 0 );
                }
             }

             /* S'il s'agit d'un echangeur */
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
                    vl_DonneesEch.Etat_Service      = 0;
                    vl_DonneesTube.ListeSequence    = NULL;
                    vl_DonneesEch.ListeEquipement   = NULL;
                    vl_DonneesEch.NbrEquipement         = 0;
                    et_ins_liste_Ech ( &vl_DonneesEch, 0 );
                }
             }

             /* AAZ */
             /* S'il s'agit du domaine signalisation */
             if ( *pl_LigneFich == 'S' )
             {
                 
                /*B Extraire les parametres*/
                if ( sscanf ( pl_LigneFich, "S %hd %hd %hd ",  
                    &vl_DonneesSIG.Numero,
					&vl_DonneesSIG.TypeOuvrage,
                    &vl_DonneesSIG.AdresseEtat ) == 3 )
                {

                    /*B Inserer dans la liste des structures Signalisation */
					vl_DonneesSIG.NumeroAbsolu	   = vl_NumeroCourant++;
                    vl_DonneesSIG.ListeSequence    = NULL;
                    vl_DonneesSIG.ListeEquipement  = NULL;
                    et_ins_liste_SIG ( &vl_DonneesSIG, 0 );
                }
             }

             /* S'il s'agit du domaine eclairage */
             if ( *pl_LigneFich == 'C' )
             {
                /*B Extraire les parametres*/
                if ( sscanf ( pl_LigneFich, "C %hd %hd %hd",  
                    &vl_DonneesECL.Numero,
					&vl_DonneesECL.TypeOuvrage,
                    &vl_DonneesECL.AdresseEtat ) == 3 )
                {
                    /*B Inserer dans la liste des structures Eclairage */
					vl_DonneesECL.NumeroAbsolu	   = vl_NumeroCourant++;
                    vl_DonneesECL.ListeSequence    = NULL;
                    vl_DonneesECL.ListeEquipement  = NULL;
                    et_ins_liste_ECL ( &vl_DonneesECL, 0 );
                }
             }

             /* S'il s'agit du domaine ventilation */
             if ( *pl_LigneFich == 'V' )
             {
                /*B Extraire les parametres*/
                if ( sscanf ( pl_LigneFich, "V %hd %hd %hd",  
                    &vl_DonneesVEN.Numero,
                    &vl_DonneesVEN.TypeOuvrage,
                    &vl_DonneesVEN.AdresseEtat ) == 3 )
                {
                    /*B Inserer dans la liste des structures Ventilation */
					vl_DonneesVEN.NumeroAbsolu	   = vl_NumeroCourant++;
                    vl_DonneesVEN.ListeSequence    = NULL;
                    vl_DonneesVEN.ListeEquipement  = NULL;
                    et_ins_liste_VEN ( &vl_DonneesVEN, 0 );
                }
             }
             /* S'il s'agit du domaine energie */
             if ( *pl_LigneFich == 'N' )
             {
                /*B Extraire les parametres*/
                if ( sscanf ( pl_LigneFich, "N %hd %hd %hd",  
                    &vl_DonneesENR.Numero,
                    &vl_DonneesENR.TypeOuvrage,
                    &vl_DonneesENR.AdresseEtat ) == 3 )
                {
                    /*B Inserer dans la liste des structures energie */
					vl_DonneesENR.NumeroAbsolu	   = vl_NumeroCourant++;
                    vl_DonneesENR.ListeSequence    = NULL;
                    vl_DonneesENR.ListeEquipement  = NULL;
                    et_ins_liste_ENR ( &vl_DonneesENR, 0 );
                }
             }
             /* S'il s'agit du domaine protection incendie */
             if ( *pl_LigneFich == 'I' )
             {
                /*B Extraire les parametres*/
                if ( sscanf ( pl_LigneFich, "I %hd %hd %hd",  
                    &vl_DonneesINC.Numero,
                    &vl_DonneesINC.TypeOuvrage,
                    &vl_DonneesINC.AdresseEtat ) == 3 )
                {
                    /*B Inserer dans la liste des structures protection incendie */
					vl_DonneesINC.NumeroAbsolu	   = vl_NumeroCourant++;
                    vl_DonneesINC.ListeSequence    = NULL;
                    vl_DonneesINC.ListeEquipement  = NULL;
                    et_ins_liste_INC ( &vl_DonneesINC, 0 );
                }
             }
             /* S'il s'agit du domaine exploitation */
             if ( *pl_LigneFich == 'X' )
             {
                /*B Extraire les parametres*/
                if ( sscanf ( pl_LigneFich, "X %hd %hd %hd",  
                    &vl_DonneesEXP.Numero,
                    &vl_DonneesEXP.TypeOuvrage,
                    &vl_DonneesEXP.AdresseEtat ) == 3 )
                {
                    /*B Inserer dans la liste des structures exploitation */
					vl_DonneesEXP.NumeroAbsolu	   = vl_NumeroCourant++;
                    vl_DonneesEXP.ListeSequence    = NULL;
                    vl_DonneesEXP.ListeEquipement  = NULL;
                    et_ins_liste_EXP ( &vl_DonneesEXP, 0 );
                }
             }
             /* S'il s'agit du domaine systeme */
             if ( *pl_LigneFich == 'Y' )
             {
                /*B Extraire les parametres*/
                if ( sscanf ( pl_LigneFich, "Y %hd %hd %hd",  
                    &vl_DonneesSYS.Numero,
                    &vl_DonneesSYS.TypeOuvrage,
                    &vl_DonneesSYS.AdresseEtat ) == 3 )
                {
                    /*B Inserer dans la liste des structures energie */
					vl_DonneesSYS.NumeroAbsolu	   = vl_NumeroCourant++;
                    vl_DonneesSYS.ListeSequence    = NULL;
                    vl_DonneesSYS.ListeEquipement  = NULL;
                    et_ins_liste_SYS ( &vl_DonneesSYS, 0 );
                }
             }
             /* S'il s'agit du domaine locaux techniques */
             if ( *pl_LigneFich == 'L' )
             {
                /*B Extraire les parametres*/
                if ( sscanf ( pl_LigneFich, "L %hd %hd %hd",  
                    &vl_DonneesLTN.Numero,
                    &vl_DonneesLTN.TypeOuvrage,
                    &vl_DonneesLTN.AdresseEtat ) == 3 )
                {
                    /*B Inserer dans la liste des structures energie */
					vl_DonneesLTN.NumeroAbsolu	   = vl_NumeroCourant++;
                    vl_DonneesLTN.ListeSequence    = NULL;
                    vl_DonneesLTN.ListeEquipement  = NULL;
                    et_ins_liste_LTN ( &vl_DonneesLTN, 0 );
                }
             }
             
             /* AAZ */

         }
      }

      /*A Fermeture du fichier XDF_Config_GTCNiceRenovee_<NomMachine>  */
      XZSS_13FermerFichier( vl_Fd );
      
      /* Ecriture trace de la config pour debug */
      for ( pl_DonCrtTube = pg_debutTube; pl_DonCrtTube != NULL; pl_DonCrtTube = pl_DonCrtTube->Suivant )
      {
          XZST_03EcritureTrace ( XZSTC_DEBUG3, "Config Tube: %hd %hd %hd %hd %hd %hd %hd %hd %hd %hd %hd ",  
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
          XZST_03EcritureTrace ( XZSTC_DEBUG3, "Config Ech: %hd %hd %hd %hd %hd ",  
                    vl_DonneesEch.NumeroDemiEch,
                    vl_DonneesEch.AdresseEtat,
                    vl_DonneesEch.AdresseCommande,
                    vl_DonneesEch.AdresseDiscord,
                    vl_DonneesEch.AdresseTelealarme );              
      }

      /* AAZ 20 Septembre 2007 Trace pour domaines */
      /* Ecriture trace de la config pour debug */
      for ( pl_DonCrtSIG = pg_debutSIG; pl_DonCrtSIG != NULL; pl_DonCrtSIG = pl_DonCrtSIG->Suivant )
      {
          XZST_03EcritureTrace ( XZSTC_DEBUG3, "Config SIG: %hd %hd ",  
                                 vl_DonneesSIG.Numero,
                                 vl_DonneesSIG.AdresseEtat );
      }

      /* Ecriture trace de la config pour debug */
      for ( pl_DonCrtECL = pg_debutECL; pl_DonCrtECL != NULL; pl_DonCrtECL = pl_DonCrtECL->Suivant )
      {
          XZST_03EcritureTrace ( XZSTC_DEBUG3, "Config ECL: %hd %hd ",  
                                 vl_DonneesECL.Numero,
                                 vl_DonneesECL.AdresseEtat );
      }

      /* Ecriture trace de la config pour debug */
      for ( pl_DonCrtVEN = pg_debutVEN; pl_DonCrtVEN != NULL; pl_DonCrtVEN = pl_DonCrtVEN->Suivant )
      {
          XZST_03EcritureTrace ( XZSTC_DEBUG3, "Config VEN: %hd %hd ",  
                                 vl_DonneesVEN.Numero,
                                 vl_DonneesVEN.AdresseEtat );
      }

      /* Ecriture trace de la config pour debug */
      for ( pl_DonCrtENR = pg_debutENR; pl_DonCrtENR != NULL; pl_DonCrtENR = pl_DonCrtENR->Suivant )
      {
          XZST_03EcritureTrace ( XZSTC_DEBUG3, "Config ENR: %hd %hd ",  
                                 vl_DonneesENR.Numero,
                                 vl_DonneesENR.AdresseEtat );
      }

      /* Ecriture trace de la config pour debug */
      for ( pl_DonCrtINC = pg_debutINC; pl_DonCrtINC != NULL; pl_DonCrtINC = pl_DonCrtINC->Suivant )
      {
          XZST_03EcritureTrace ( XZSTC_DEBUG3, "Config INC: %hd %hd ",  
                                 vl_DonneesINC.Numero,
                                 vl_DonneesINC.AdresseEtat );
      }

      /* Ecriture trace de la config pour debug */
      for ( pl_DonCrtEXP = pg_debutEXP; pl_DonCrtEXP != NULL; pl_DonCrtEXP = pl_DonCrtEXP->Suivant )
      {
          XZST_03EcritureTrace ( XZSTC_DEBUG3, "Config EXP: %hd %hd ",  
                                 vl_DonneesEXP.Numero,
                                 vl_DonneesEXP.AdresseEtat );
      }

      /* Ecriture trace de la config pour debug */
      for ( pl_DonCrtSYS = pg_debutSYS; pl_DonCrtSYS != NULL; pl_DonCrtSYS = pl_DonCrtSYS->Suivant )
      {
          XZST_03EcritureTrace ( XZSTC_DEBUG3, "Config SYS: %hd %hd ",  
                                 vl_DonneesSYS.Numero,
                                 vl_DonneesSYS.AdresseEtat );
      }
      /* AAZ */


      /* Retourne compte rendu OK */
      return ( vl_ValRet );
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Cette fonction effectue la lecture du fichier XDF_Sequence_GTCNice_<NomMachine>
*  et met a jour le liste des structure de donnees Sequence
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int et_lire_config_Sequence ( XDY_NomMachine        pa_NomMachine )
              
/*
* ARGUMENTS EN ENTREE :
*   pa_NomMachine   : Nom de la machine
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK  succes
*   XDC_NOK echec
*
* CONDITION D'UTILISATION
*   
*
* FONCTION 
*   
*
------------------------------------------------------*/
{
   static char *version = "@(#)enix_cfg.c   1.2 04/24/02      :  et_lire_config_Sequence" ;

   int                    vl_ValRet = XDC_OK;
   int                    vl_Fd;
   char                   pl_PathEtFic[XDC_PATH_ABS_SIZE];
   XDY_Texte              pl_LigneFich;
   ENIX_DONNEES_Sequence  vl_DonneesSeq = {0};
   short                  vl_NumMaitre;
   
   XDY_Mot                vl_TypeMaitre;
   int				      vl_Domaine;

   
      XZST_03EcritureTrace( XZSTC_INFO, "IN : et_lire_config_Sequence");
  
      /*A Ouverture du fichier de configuration des Sequence */
      /* Construction du nom du fichier XDF_Sequence_GTCNice_<NomMachine>  */
      sprintf ( pl_PathEtFic, "%s/%s_%s", XZEXC_PATH_CONFIG, XDF_Sequence_GTCNiceRenovee, pa_NomMachine );
      
      /* Ouverture du fichier XDF_Sequence_GTCNice_<NomMachine>  */
      if ( (vl_Fd = open (pl_PathEtFic, O_RDONLY)) <= 0 )
      {
         /* Ecriture trace absence fichier */
         XZST_03EcritureTrace( XZSTC_INFO, " Manque fichier %s. \n", pl_PathEtFic ); 
         /* Sortie de la fonction */
         return ( XDC_NOK );
      }
 
      /*A Lecture de chaque ligne du fichier */
      while ( ex_LireLigneFich ( vl_Fd, pl_LigneFich, sizeof ( pl_LigneFich ) ) != XZEXC_EOF ) 
      {
          /* Lire ligne suivante si c'est une ligne de commentaire */
          if ( *pl_LigneFich != XDF_COMMENTAIRE )
          {
             /*B Extraire les parametre sequence */
             if ( sscanf ( pl_LigneFich+1," %hd %hd %hd ",  
                    &vl_TypeMaitre,
                    &vl_NumMaitre,
                    &vl_DonneesSeq.NumSequence) == 3 )
             {

                /*B Inserer dans la liste des structures sequence */
                switch ( *pl_LigneFich )
                {
                    case 'S' :
                        vl_Domaine = XDC_EQT_SIG;
                        break;
                    case 'C' :
                        vl_Domaine = XDC_EQT_ECL;
                        break;
                    case 'V' :
                        vl_Domaine = XDC_EQT_VEN;
                        break;
                    case 'N' :
                        vl_Domaine = XDC_EQT_ENR;
                        break;
                    case 'I' :
                        vl_Domaine = XDC_EQT_INC;
                        break;
                    case 'X' :
                        vl_Domaine = XDC_EQT_EXP;
                        break;
                }
                et_ins_liste_Sequence ( &vl_DonneesSeq, vl_NumMaitre, vl_TypeMaitre, vl_Domaine );
             }
         }
      }

      /*A Fermeture du fichier XDF_Sequence_GTCNice_<NomMachine>  */
      XZSS_13FermerFichier( vl_Fd );
      
     
      /*A Lecture du fichier de memorisation des dernieres sequence  */
      et_rfic_derniere_seq ( XDC_EQT_TUBE );
      et_rfic_derniere_seq ( XDC_EQT_ECH );
      et_rfic_derniere_seq ( XDC_EQT_SIG );
      et_rfic_derniere_seq ( XDC_EQT_ECL );
      et_rfic_derniere_seq ( XDC_EQT_VEN );
      et_rfic_derniere_seq ( XDC_EQT_ENR );
      et_rfic_derniere_seq ( XDC_EQT_INC );
      et_rfic_derniere_seq ( XDC_EQT_EXP );
     
      /* Retourne compte rendu OK */
      return ( vl_ValRet );
      
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Cette fonction effectue la lecture du fichier XDF_Equipement_GTCNiceRenovee_<NomOuvrage>
*  et met a jour le liste des structure de donnees Equipements;
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int et_lire_config_Equipement ( XDY_NomMachine        pa_NomMachine )
              
/*
* ARGUMENTS EN ENTREE :
*   pa_NomMachine   : Nom de la machine
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK  succes
*   XDC_NOK echec
*
* CONDITION D'UTILISATION
*   
*
* FONCTION 
*   
*
------------------------------------------------------*/
{
    static char *version = "@(#)enix_cfg.c   1.2 04/24/02      : et_lire_config_Equipement " ;
    
    int                     vl_ValRet = XDC_OK;
    int                     vl_Fd,
    vl_FdO;
    char                    pl_PathEtFic[XDC_PATH_ABS_SIZE];
    char                    pl_PathOuvFic[XDC_PATH_ABS_SIZE];
    XDY_Texte               pl_LigneFich;
    XDY_Texte               pl_LigOuvFich;
    ENIX_DONNEES_Equipement vl_DonneesEqt   = {0};
    short                   vl_NumMaitre,
    vl_TypeMaitre;
    XDY_Texte               vl_NomOuvrage;
    XDY_Texte               pl_NomDomaine;
    XDY_Texte               pl_NomOuvrage;
    
      XZST_03EcritureTrace( XZSTC_INFO, "IN : et_lire_config_Equipement");
    
    /*A Ouverture du fichier des ouvrages  */
    
    /* Construction du nom du fichier XDF_Ouvrage_GTCNiceRenovee_<NomMachine>  */
    sprintf ( pl_PathOuvFic, "%s/%s_%s", XZEXC_PATH_CONFIG1, XDF_Ouvrage_GTCNiceRenovee, pa_NomMachine );

    XZST_03EcritureTrace( XZSTC_INFO, "AZA IN : et_lire_config_Equipement fichier ouvrage %s",  pl_PathOuvFic);
    
    /* Ouverture du fichier XDF_Ouvrage_GTCNiceRenovee_<NomMachine>  */
    if ( (vl_FdO = open (pl_PathOuvFic, O_RDONLY)) <= 0 )
    {
        /* Ecriture trace absence fichier */
        XZST_03EcritureTrace( XZSTC_INFO, " Manque fichier %s. \n", pl_PathOuvFic ); 
        /* Sortie de la fonction */
        return ( XDC_NOK );
    }
    
    /*A Lecture de chaque ligne du fichier */
    while ( ex_LireLigneFich ( vl_FdO, pl_LigOuvFich, sizeof ( pl_LigOuvFich ) ) != XZEXC_EOF ) 
    {
        XZST_03EcritureTrace( XZSTC_INFO, "AZA IN : et_lire_config_Equipement ligne ouvrage %s",  pl_LigOuvFich);
        /* Seuls les lignes ouvrages sont lues */
        if ( *pl_LigOuvFich == 'O' )
        {
            XZST_03EcritureTrace( XZSTC_INFO, "AZA IN : et_lire_config_Equipement ouvrage %s",  pl_NomOuvrage);
    
            if ( sscanf ( pl_LigOuvFich,"O %s",  pl_NomOuvrage) == 1)
            {
    
                /*A Ouverture du fichier de configuration des équipements */
    
                /* Construction du nom du fichier XDF_Equipement_GTCNiceRenovee_<NomOuvrage>_<NomMachine>  */
                sprintf ( pl_PathEtFic, "%s/%s_%s_%s", XZEXC_PATH_CONFIG1, XDF_Equipement_GTCNiceRenovee, pl_NomOuvrage, pa_NomMachine );

                XZST_03EcritureTrace( XZSTC_INFO, "AZA IN : et_lire_config_Equipement fichier equipement %s",  pl_PathEtFic);
    
                /* Ouverture du fichier XDF_Equipement_GTCNiceRenovee_<NomMachine>_<NomOuvrage>  */
                if ( (vl_Fd = open (pl_PathEtFic, O_RDONLY)) <= 0 )
                {
                    /* Ecriture trace absence fichier */
                    XZST_03EcritureTrace( XZSTC_INFO, " Manque fichier %s. \n", pl_PathEtFic ); 
                }
                else
                {
                    /*A Lecture de chaque ligne du fichier */
                    while ( ex_LireLigneFich ( vl_Fd, pl_LigneFich, sizeof ( pl_LigneFich ) ) != XZEXC_EOF ) 
                    {
                        XZST_03EcritureTrace( XZSTC_INFO, "AZA IN : et_lire_config_Equipement equipement %s",  pl_LigneFich);
                        /* Lire ligne suivante si c'est une ligne de commentaire */
                        if ( *pl_LigneFich != XDF_COMMENTAIRE )
                        {
                            /* Si il s'agit d'un equipement */
                            if ( *pl_LigneFich == 'Q' )
                            {
                                /*B Extraire les parametre du equipement */
                                if ( sscanf ( pl_LigneFich,"Q %hd %hd %hd %hd %s",  
                                              &vl_TypeMaitre,
                                              &vl_NumMaitre,
											  &vl_DonneesEqt.TypeEquipement,
                                              &vl_DonneesEqt.NumeroEquipement,
											  pl_NomDomaine
                                               ) == 5 )
                                {
									switch(pl_NomDomaine[0])
									{
									case('S'):
										vl_DonneesEqt.Domaine = XDC_EQT_SIG;
										break;
									case('C'):
										vl_DonneesEqt.Domaine = XDC_EQT_ECL;
										break;
									case('V'):
										vl_DonneesEqt.Domaine = XDC_EQT_VEN;
										break;
									case('N'):
										vl_DonneesEqt.Domaine = XDC_EQT_ENR;
										break;
									case('I'):
										vl_DonneesEqt.Domaine = XDC_EQT_INC;
										break;
									case('X'):
										vl_DonneesEqt.Domaine = XDC_EQT_EXP;
										break;
									case('Y'):
										vl_DonneesEqt.Domaine = XDC_EQT_SYS;
										break;
									case('L'):
										vl_DonneesEqt.Domaine = XDC_EQT_LTN;
										break;
									}

									XZST_03EcritureTrace( XZSTC_INFO, "Insertion equipement : domaine %d (%s) - numero %d - type %d - ouvrage %d - type ouvrage %d",
												vl_DonneesEqt.Domaine,
												pl_NomDomaine,
												vl_DonneesEqt.NumeroEquipement,
												vl_DonneesEqt.TypeEquipement,
												vl_NumMaitre,
												vl_TypeMaitre);

									
                                    /*B Inserer dans la liste des structures equipement */
                                    vl_DonneesEqt.Etat_Service  = 0;
                                    vl_DonneesEqt.Mode = 0;
                                    et_ins_liste_Equipement ( &vl_DonneesEqt, vl_NumMaitre, vl_TypeMaitre );
                                }
                            }
            
                        }
                    }
                }
                /*A Fermeture du fichier XDF_Equipement_GTCNiceRenovee_<NomMachine>_<NomOuvrage>  */
                XZSS_13FermerFichier( vl_Fd );
            }
        }
    }
    /*A Fermeture du fichier XDF_Ouvrage_GTCNiceRenovee_<NomMachine>  */
    XZSS_13FermerFichier( vl_FdO );
    
    /* Retourne compte rendu OK */
    return ( vl_ValRet );
      
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void et_wfic_derniere_seq ( int va_Type )
 
/*
* ARGUMENTS EN ENTREE :
*  
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
*   
*
* FONCTION 
* creer le fichier de memorisation des dernieres sequences
*
------------------------------------------------------*/
{
static char *version = "@(#)enix_cfg.c  1.3 06/07/07      : et_wfic_derniere_seq " ;
int                     vl_Fd        = 0;
int                     vl_Compteur  = 0;
char                    pl_NomFichier[XDC_PATH_ABS_SIZE];
XDY_Texte               pl_LigneFich = "";
ENIX_DONNEES_Tube       *pl_Tube;
ENIX_DONNEES_Echangeur  *pl_Ech;
ENIX_DONNEES_SIG        *pl_SIG;
ENIX_DONNEES_ECL        *pl_ECL;
ENIX_DONNEES_VEN        *pl_VEN;
ENIX_DONNEES_ENR        *pl_ENR;
ENIX_DONNEES_INC        *pl_INC;
ENIX_DONNEES_EXP        *pl_EXP;

      XZST_03EcritureTrace( XZSTC_INFO, "IN : et_wfic_dernier_seq");

   /* Si le fichier d'enregistrement des dernieres sequences existe */
   sprintf ( pl_NomFichier, "%s/%s_%02d", XZEXC_PATH_CONFIG, XDF_Sequence_GTCNice, va_Type );
   
   /* Creation du fichier d'enregistrement des alarmes */
   if ( ( vl_Fd = open ( pl_NomFichier, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH ) ) < 0 )
   {
       XZST_03EcritureTrace( XZSTC_INFO, "Impossible de creer le fichier des dernieres sequences " );
       return;
   }

   /*A Ecriture de toutes les alarmes de la liste */
   for ( pl_Tube = pg_debutTube; (pl_Tube != NULL) && (va_Type == XDC_EQT_TUBE); pl_Tube = pl_Tube->Suivant )
       vl_Compteur += sprintf ( pl_LigneFich+vl_Compteur, "%03hd%03hd*", pl_Tube->Numero, pl_Tube->DerniereSequence);

   for ( pl_Ech = pg_debutEchangeur; (pl_Ech != NULL) && (va_Type == XDC_EQT_ECH); pl_Ech = pl_Ech->Suivant )
       vl_Compteur += sprintf ( pl_LigneFich+vl_Compteur, "%03hd%03hd*", pl_Ech->NumeroDemiEch, pl_Ech->DerniereSequence);

   for ( pl_SIG = pg_debutSIG; (pl_SIG != NULL) && (va_Type == XDC_EQT_SIG); pl_SIG = pl_SIG->Suivant )
       vl_Compteur += sprintf ( pl_LigneFich+vl_Compteur, "%03hd%03hd*", pl_SIG->Numero, pl_SIG->DerniereSequence);

   for ( pl_ECL = pg_debutECL; (pl_ECL != NULL) && (va_Type == XDC_EQT_ECL); pl_ECL = pl_ECL->Suivant )
       vl_Compteur += sprintf ( pl_LigneFich+vl_Compteur, "%03hd%03hd*", pl_ECL->Numero, pl_ECL->DerniereSequence);

   for ( pl_VEN = pg_debutVEN; (pl_VEN != NULL) && (va_Type == XDC_EQT_VEN); pl_VEN = pl_VEN->Suivant )
       vl_Compteur += sprintf ( pl_LigneFich+vl_Compteur, "%03hd%03hd*", pl_VEN->Numero, pl_VEN->DerniereSequence);

   for ( pl_ENR = pg_debutENR; (pl_ENR != NULL) && (va_Type == XDC_EQT_ENR); pl_ENR = pl_ENR->Suivant )
       vl_Compteur += sprintf ( pl_LigneFich+vl_Compteur, "%03hd%03hd*", pl_ENR->Numero, pl_ENR->DerniereSequence);

   for ( pl_INC = pg_debutINC; (pl_INC != NULL) && (va_Type == XDC_EQT_INC); pl_INC = pl_INC->Suivant )
       vl_Compteur += sprintf ( pl_LigneFich+vl_Compteur, "%03hd%03hd*", pl_INC->Numero, pl_INC->DerniereSequence);

   for ( pl_EXP = pg_debutEXP; (pl_EXP != NULL) && (va_Type == XDC_EQT_EXP); pl_EXP = pl_EXP->Suivant )
       vl_Compteur += sprintf ( pl_LigneFich+vl_Compteur, "%03hd%03hd*", pl_EXP->Numero, pl_EXP->DerniereSequence);
      
   if ( write ( vl_Fd, pl_LigneFich, strlen(pl_LigneFich) ) < 0 )
   {
       XZST_03EcritureTrace( XZSTC_INFO, "Ecriture dans fichier des dernieres sequences incorrect" );
   }
   
   close ( vl_Fd );
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void et_rfic_derniere_seq ( int va_Type  )
 
/*
* ARGUMENTS EN ENTREE :
*  
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
*   
*
* FONCTION 
* lire le fichier de memorisation des dernieres sequences
*
------------------------------------------------------*/
{
static char *version = "@(#)enix_cfg.c  1.3 06/07/07      : et_rfic_derniere_seq " ;
int                     vl_Fd         = 0;
char                    pl_NomFichier[XDC_PATH_ABS_SIZE];
XDY_Texte               pl_LigneFich  = "";
int                     vl_Index      = 0;
XDY_Mot                 vl_Numero     = 0;
XDY_Mot                 vl_Sequence   = 0;
ENIX_DONNEES_Tube       *pl_Tube;
ENIX_DONNEES_Echangeur  *pl_Ech;
ENIX_DONNEES_SIG        *pl_SIG;
ENIX_DONNEES_ECL        *pl_ECL;
ENIX_DONNEES_VEN        *pl_VEN;
ENIX_DONNEES_ENR        *pl_ENR;
ENIX_DONNEES_INC        *pl_INC;
ENIX_DONNEES_EXP        *pl_EXP;

      XZST_03EcritureTrace( XZSTC_INFO, "IN : et_rfic_derniere_seq");

   /* Si le fichier d'enregistrement des dernieres sequences existe */
   sprintf ( pl_NomFichier, "%s/%s_%02d", XZEXC_PATH_CONFIG, XDF_Sequence_GTCNice, va_Type );
   if ( access ( pl_NomFichier, F_OK ) )
   {
       return;
   }
   /* Creation du fichier d'enregistrement des dernieres sequences */
   if ( ( vl_Fd = open ( pl_NomFichier, O_RDWR ) ) < 0 )
   {
       XZST_03EcritureTrace( XZSTC_INFO, "Impossible d'ouvrir le fichier des dernieres sequences " );
       return;
   }

   /*A Lecture de chaque ligne du fichier */
   while ( ex_LireLigneFich ( vl_Fd, pl_LigneFich, sizeof ( pl_LigneFich ) ) != XZEXC_EOF )
   {
      /*A Lire ligne suivante si c'est une ligne de commentaire */
      if ( *pl_LigneFich != XDF_COMMENTAIRE )
      {
         for ( vl_Index = 0; vl_Index < strlen(pl_LigneFich); vl_Index += 7 )
         {
            if ( sscanf (& pl_LigneFich[vl_Index], "%03hd%03hd", &vl_Numero, &vl_Sequence ) == 2 )
            {
               for ( pl_Tube = pg_debutTube; (pl_Tube != NULL) && (va_Type == XDC_EQT_TUBE); pl_Tube = pl_Tube->Suivant )
               {
                  if ( vl_Numero == pl_Tube->Numero )
                  {
                     pl_Tube->DerniereSequence = vl_Sequence;
                     break;
                  }
               }

               for ( pl_Ech = pg_debutEchangeur; (pl_Ech != NULL) && (va_Type == XDC_EQT_ECH); pl_Ech = pl_Ech->Suivant )
               {
                  if ( vl_Numero == pl_Ech->NumeroDemiEch )
                  {
                     pl_Ech->DerniereSequence = vl_Sequence;
                     break;
                  }
               }

               for ( pl_SIG = pg_debutSIG; (pl_SIG != NULL) && (va_Type == XDC_EQT_SIG); pl_SIG = pl_SIG->Suivant )
               {
                  if ( vl_Numero == pl_SIG->Numero )
                  {
                     pl_SIG->DerniereSequence = vl_Sequence;
                     break;
                  }
               }

               for ( pl_ECL = pg_debutECL; (pl_ECL != NULL) && (va_Type == XDC_EQT_ECL); pl_ECL = pl_ECL->Suivant )
               {
                  if ( vl_Numero == pl_ECL->Numero )
                  {
                     pl_ECL->DerniereSequence = vl_Sequence;
                     break;
                  }
               }

               for ( pl_VEN = pg_debutVEN; (pl_VEN != NULL) && (va_Type == XDC_EQT_VEN); pl_VEN = pl_VEN->Suivant )
               {
                  if ( vl_Numero == pl_VEN->Numero )
                  {
                     pl_VEN->DerniereSequence = vl_Sequence;
                     break;
                  }
               }

               for ( pl_ENR = pg_debutENR; (pl_ENR != NULL) && (va_Type == XDC_EQT_ENR); pl_ENR = pl_ENR->Suivant )
               {
                  if ( vl_Numero == pl_ENR->Numero )
                  {
                     pl_ENR->DerniereSequence = vl_Sequence;
                     break;
                  }
               }

               for ( pl_INC = pg_debutINC; (pl_INC != NULL) && (va_Type == XDC_EQT_INC); pl_INC = pl_INC->Suivant )
               {
                  if ( vl_Numero == pl_INC->Numero )
                  {
                     pl_INC->DerniereSequence = vl_Sequence;
                     break;
                  }
               }

               for ( pl_EXP = pg_debutEXP; (pl_EXP != NULL) && (va_Type == XDC_EQT_EXP); pl_EXP = pl_EXP->Suivant )
               {
                  if ( vl_Numero == pl_EXP->Numero )
                  {
                     pl_EXP->DerniereSequence = vl_Sequence;
                     break;
                  }
               }
               
            }
         }
      }
   }
   
   close ( vl_Fd );
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Cette fonction effectue la lecture du fichier XDF_Presence_Equip_<NomMachine>.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int ex_lire_config_Wizcon   ( int va_Instance_Tenic)

/*
* ARGUMENTS EN ENTREE :
*   pa_NomMachine   : Nom de la machine
*
* ARGUMENTS EN SORTIE :
*
* CODE RETOUR : 
*   XDC_OK  succes
*   XDC_NOK echec
*
* CONDITION D'UTILISATION
*   
*
* FONCTION 
*   
*
------------------------------------------------------*/
{
   int              vl_ValRet = XDC_OK;
   int              vl_Fd;
   char             pl_PathEtFic[XDC_PATH_ABS_SIZE];
   char             *pl_Donnee;
   XDY_Commentaire  pl_LigneFich;
   int              vl_index=0;

    static char *version = "@(#)enix_cfg.c  1.3 06/07/07      : et_lire_config_Wizcon " ;

      XZST_03EcritureTrace( XZSTC_INFO, "IN : ex_lire_config_Wizcon");

      /*A Ouverture du fichier de config Wizcon */
      /* Construction du nom du fichier Config_Wizcon_GTCNice */
      sprintf ( pl_PathEtFic, "%s/%s%1d", XZEXC_PATH_CONFIG1, ENIX_Wizcon, va_Instance_Tenic);
      XZST_03EcritureTrace(XZSTC_INFO, "AZA DEBUG Fichier reconstruit %s.", pl_PathEtFic );      
      /*A Ouverture du fichier  */
      if ( (vl_Fd = open (pl_PathEtFic, O_RDONLY)) <= 0 )
      {
         /*A Ecriture trace absence fichier */
         XZST_03EcritureTrace( XZSTC_INFO, " Manque fichier %s. ", pl_PathEtFic ); 
         /*A Sortie de la fonction */
         return ( XDC_NOK );
      }
 
      /*A Lecture de chaque ligne du fichier */
      while ( ex_LireLigneFich ( vl_Fd, pl_LigneFich, sizeof ( pl_LigneFich ) ) != XZEXC_EOF ) 
      {
          /*A Lire ligne suivante si c'est une ligne de commentaire */
          if ( *pl_LigneFich != XDF_COMMENTAIRE )
          {
         if (vl_index == 0)
         {
                /*A Extraire le nom de la machine et du port s'ils sont presents */
                sscanf ( pl_LigneFich,  "%s %d %d %d" , vg_nom_machine_TWWI0,&vg_Port_SockTWWI0, &vg_TypeEqt_LiaiTWWI0, &vg_NumEqt_LiaiTWWI0);
                            
                XZST_03EcritureTrace( XZSTC_WARNING, "machine 0:%s Port TCPIP:%d Type Eqt = %d, NumEqt = %d ", 
                	vg_nom_machine_TWWI0, vg_Port_SockTWWI0, vg_TypeEqt_LiaiTWWI0, vg_NumEqt_LiaiTWWI0 );
         }
         if (vl_index == 1)
         {
                /*A Extraire le nom de la machine et du port s'ils sont presents */
                sscanf ( pl_LigneFich,  "%s %d %d %d", vg_nom_machine_TWWI1,&vg_Port_SockTWWI1, &vg_TypeEqt_LiaiTWWI1, &vg_NumEqt_LiaiTWWI1);
                            
                XZST_03EcritureTrace( XZSTC_WARNING, "machine 1:%s Port TCPIP:%d Type Eqt = %d, NumEqt = %d ",
                	 vg_nom_machine_TWWI1, vg_Port_SockTWWI1, vg_TypeEqt_LiaiTWWI1, vg_NumEqt_LiaiTWWI1);
         }
         vl_index ++;
         }
      }
      /*A Fermeture du fichier Config_Wizcon_GTCNice */
      XZSS_13FermerFichier( vl_Fd );

      /*A Retourne compte rendu OK */
      return ( vl_ValRet );
}
