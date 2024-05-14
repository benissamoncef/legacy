/*E*/
/*Fichier :  $Id: eniw_cfg.c,v 1.2 2002/04/24 08:46:53 gesconf Exp $      Release : $Revision: 1.2 $        Date : $Date: 2002/04/24 08:46:53 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TENIW * FICHIER eniw_cfg.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Module de configuration de la tache TENIW, gtc Nice
*
------------------------------------------------------
* HISTORIQUE :
*
* Moulin.D	version 1.1	07 Janv 2002	: Creation
* Moulin.D	version 1.2	23 Avr 2002	: Installation PLate forme
*
------------------------------------------------------*/
 
/* fichiers inclus */

#include "eniw_cfg.h"

/* definitions de constantes */
#define ENIW_Wizcon "Config_Wizcon_GTCNice"

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: eniw_cfg.c,v 1.2 2002/04/24 08:46:53 gesconf Exp $      : eniw_cfg" ;

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

int et_lire_config_GTCNice	( XDY_NomMachine		pa_NomMachine )
			  
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
   static char *version = "$Id: eniw_cfg.c,v 1.2 2002/04/24 08:46:53 gesconf Exp $      : et_lire_config_GTCNice " ;

   int				vl_ValRet = XDC_OK;
   int				vl_Fd;
   char 			pl_PathEtFic[XDC_PATH_ABS_SIZE];
   XDY_Texte			pl_LigneFich;
   ENIW_DONNEES_Tube		vl_DonneesTube = {0},
   				*pl_DonCrtTube,
   				*pl_DonSupTube;
   ENIW_DONNEES_Echangeur	vl_DonneesEch  = {0},
   				*pl_DonCrtEch,
   				*pl_DonSupEch;
   
      /*A Ouverture du fichier de configuration GTCNice */
      XZST_03EcritureTrace( XZSTC_WARNING, "Config GTC Nice"); 
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
      	 et_sup_liste_Sequence ( pl_DonCrtTube->ListeSequence );
      	 et_sup_liste_Panneau  ( pl_DonCrtTube->ListePanneau );
      	 et_sup_liste_Capteur  ( pl_DonCrtTube->ListeCapteur );
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
      	 et_sup_liste_Sequence ( pl_DonCrtTube->ListeSequence );
      	 et_sup_liste_Panneau  ( pl_DonCrtEch->ListePanneau );
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
          	   vl_DonneesTube.Etat_Service   = 0;
		   vl_DonneesTube.ListeSequence	 = NULL;
	 	   vl_DonneesTube.ListePanneau	 = NULL;
	 	   vl_DonneesTube.ListeCapteur	 = NULL;
	 	   vl_DonneesTube.NbrPanFAV	 = 0;
                   et_ins_liste_Tube ( &vl_DonneesTube, 0 );
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
          	   vl_DonneesEch.Etat_Service    = 0;
		   vl_DonneesTube.ListeSequence	 = NULL;
	 	   vl_DonneesEch.ListePanneau	 = NULL;
	 	   vl_DonneesEch.NbrPanFAV	 = 0;
                   et_ins_liste_Ech ( &vl_DonneesEch, 0 );
                }
            }
         }
      }
      /*A Fermeture du fichier XDF_Config_GTCNice_<NomMachine>  */
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

int et_lire_config_Sequence	( XDY_NomMachine		pa_NomMachine )
			  
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
   static char *version = "$Id: eniw_cfg.c,v 1.2 2002/04/24 08:46:53 gesconf Exp $      :  et_lire_config_Sequence" ;

   int				vl_ValRet = XDC_OK;
   int				vl_Fd;
   char 			pl_PathEtFic[XDC_PATH_ABS_SIZE];
   XDY_Texte			pl_LigneFich;
   ENIW_DONNEES_Sequence	vl_DonneesSeq = {0};
   short			vl_NumMaitre;
   				
   
  
      /*A Ouverture du fichier de configuration des Sequence */
      /* Construction du nom du fichier XDF_Sequence_GTCNice_<NomMachine>  */
      sprintf ( pl_PathEtFic, "%s/%s_%s", XZEXC_PATH_CONFIG, XDF_Sequence_GTCNice, pa_NomMachine );
      
      /* Ouverture du fichier XDF_Sequence_GTCNice_<NomMachine>  */
      if ( (vl_Fd = open (pl_PathEtFic, O_RDONLY)) <= 0 )
      {
         /* Ecriture trace absence fichier */
         XZST_03EcritureTrace( XZSTC_WARNING, " Manque fichier %s. \n", pl_PathEtFic ); 
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
             if ( sscanf ( pl_LigneFich+1," %hd %hd %hd %hd %hd ",  
					&vl_NumMaitre,
					&vl_DonneesSeq.NumSequence,
					&vl_DonneesSeq.Adresse,
					&vl_DonneesSeq.BitCommande,
					&vl_DonneesSeq.BitDiscordance ) == 5 )
             {
                /*B Inserer dans la liste des structures sequence */
                et_ins_liste_Sequence ( &vl_DonneesSeq, vl_NumMaitre, (*pl_LigneFich == 'T') ? XDC_EQT_TUBE : XDC_EQT_ECH );
             }
         }
      }

      /*A Fermeture du fichier XDF_Sequence_GTCNice_<NomMachine>  */
      XZSS_13FermerFichier( vl_Fd );
      
     
      /*A Lecture du fichier de memorisation des dernieres sequence  */
      et_rfic_derniere_seq ( XDC_EQT_TUBE );
      et_rfic_derniere_seq ( XDC_EQT_ECH );
     
      /* Retourne compte rendu OK */
      return ( vl_ValRet );
      
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Cette fonction effectue la lecture du fichier XDF_Capt_Pan_GTCNice_<NomMachine>
*  et met a jour le liste des structure de donnees Capteur, Panneau et FAV;
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int et_lire_config_Capt_Pan	( XDY_NomMachine		pa_NomMachine )
			  
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
   static char *version = "$Id: eniw_cfg.c,v 1.2 2002/04/24 08:46:53 gesconf Exp $      : et_lire_config_Capt_Pan " ;

   int				vl_ValRet = XDC_OK;
   int				vl_Fd;
   char 			pl_PathEtFic[XDC_PATH_ABS_SIZE];
   XDY_Texte			pl_LigneFich;
   ENIW_DONNEES_Panneau		vl_DonneesPn   = {0};
   ENIW_DONNEES_Capteur		vl_DonneesCapt = {0};
   short			vl_NumMaitre,
   				vl_TypeMaitre;
   				
   
  
      /*A Ouverture du fichier de configuration des Capteurs et Panneaux */
      /* Construction du nom du fichier XDF_Capt_Pan_GTCNice_<NomMachine>  */
      sprintf ( pl_PathEtFic, "%s/%s_%s", XZEXC_PATH_CONFIG, XDF_Capt_Pan_GTCNice, pa_NomMachine );
      
      /* Ouverture du fichier XDF_Capt_Pan_GTCNice_<NomMachine>  */
      if ( (vl_Fd = open (pl_PathEtFic, O_RDONLY)) <= 0 )
      {
         /* Ecriture trace absence fichier */
         XZST_03EcritureTrace( XZSTC_WARNING, " Manque fichier %s. \n", pl_PathEtFic ); 
         /* Sortie de la fonction */
         return ( XDC_NOK );
      }
 
      /*A Lecture de chaque ligne du fichier */
      while ( ex_LireLigneFich ( vl_Fd, pl_LigneFich, sizeof ( pl_LigneFich ) ) != XZEXC_EOF ) 
      {
          /* Lire ligne suivante si c'est une ligne de commentaire */
          if ( *pl_LigneFich != XDF_COMMENTAIRE )
          {
             /* Si il s'agit d'un panneau */
             if ( *pl_LigneFich == 'P' )
             {
                /*B Extraire les parametre du panneau */
                if ( sscanf ( pl_LigneFich,"P %hd %hd %hd %hd %hd %hd %hd %hd %hd",  
					&vl_NumMaitre,
					&vl_TypeMaitre,
					&vl_DonneesPn.NumeroPanneau,
					&vl_DonneesPn.RangComm,
					&vl_DonneesPn.RangComm2,
					&vl_DonneesPn.RangEtat,
					&vl_DonneesPn.RangEtat2,
					&vl_DonneesPn.BitDiscordance,
					&vl_DonneesPn.BitDiscordance2 ) == 9 )
		{
                   /*B Inserer dans la liste des structures panneau */
                   vl_DonneesPn.Etat_Service  = 0;
                   et_ins_liste_Panneau ( &vl_DonneesPn, vl_NumMaitre, vl_TypeMaitre );
                }
             }
             
             /* Si il s'agit d'un capteur */
             if ( *pl_LigneFich == 'C' )
             {
                /*B Extraire les parametre du capteur */
                if ( sscanf ( pl_LigneFich,"C %hd %hd %hd %hd %hd",  
					&vl_NumMaitre,
					&vl_DonneesCapt.NumeroCapteur,
					&vl_DonneesCapt.Type,
					&vl_DonneesCapt.NumBit,
					&vl_DonneesCapt.NumAlerte ) == 5 )
                {
                    vl_DonneesCapt.Etat_Service  = 0;
                    /*B Inserer dans la liste des structures Capteur */
                    et_ins_liste_Capteur ( &vl_DonneesCapt, vl_NumMaitre );
                }
             }
             
         }
      }
      /*A Fermeture du fichier XDF_Capt_Pan_GTCNice_<NomMachine>  */
      XZSS_13FermerFichier( vl_Fd );
      
     
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
static char *version = "$Id: eniw_cfg.c,v 1.2 2002/04/24 08:46:53 gesconf Exp $      : et_wfic_derniere_seq " ;
int                     vl_Fd 	     = 0;
int 			vl_Compteur  = 0;
char                    pl_NomFichier[XDC_PATH_ABS_SIZE];
XDY_Texte		pl_LigneFich = "";
ENIW_DONNEES_Tube      *pl_Tube;
ENIW_DONNEES_Echangeur *pl_Ech;


   /* Si le fichier d'enregistrement des dernieres sequences existe */
   sprintf ( pl_NomFichier, "%s/%s_%02d", XZEXC_PATH_CONFIG, XDF_Sequence_GTCNice, va_Type );
   
   /* Creation du fichier d'enregistrement des alarmes */
   if ( ( vl_Fd = open ( pl_NomFichier, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH ) ) < 0 )
   {
      XZST_03EcritureTrace( XZSTC_WARNING, "Impossible de creer le fichier des dernieres sequences " );
      return;
   }

   /*A Ecriture de toutes les alarmes de la liste */
   for ( pl_Tube = pg_debutTube; (pl_Tube != NULL) && (va_Type == XDC_EQT_TUBE); pl_Tube = pl_Tube->Suivant )
      vl_Compteur += sprintf ( pl_LigneFich+vl_Compteur, "%03hd%03hd*", pl_Tube->Numero, pl_Tube->DerniereSequence);
   for ( pl_Ech = pg_debutEchangeur; (pl_Ech != NULL) && (va_Type == XDC_EQT_ECH); pl_Ech = pl_Ech->Suivant )
      vl_Compteur += sprintf ( pl_LigneFich+vl_Compteur, "%03hd%03hd*", pl_Ech->NumeroDemiEch, pl_Ech->DerniereSequence);
      
   if ( write ( vl_Fd, pl_LigneFich, strlen(pl_LigneFich) ) < 0 )
   {
      XZST_03EcritureTrace( XZSTC_WARNING, "Ecriture dans fichier des dernieres sequence incorrect" );
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
static char *version = "$Id: eniw_cfg.c,v 1.2 2002/04/24 08:46:53 gesconf Exp $      : et_rfic_derniere_seq " ;
int                     vl_Fd 	      = 0;
char                    pl_NomFichier[XDC_PATH_ABS_SIZE];
XDY_Texte		pl_LigneFich  = "";
int 			vl_Index      = 0;
XDY_Mot			vl_Numero     = 0;
XDY_Mot			vl_Sequence   = 0;
ENIW_DONNEES_Tube      *pl_Tube;
ENIW_DONNEES_Echangeur *pl_Ech;


   /* Si le fichier d'enregistrement des alarmes existe */
   sprintf ( pl_NomFichier, "%s/%s_%02d", XZEXC_PATH_CONFIG, XDF_Sequence_GTCNice, va_Type );
   if ( access ( pl_NomFichier, F_OK ) )
   {
      return;
   }
   /* Creation du fichier d'enregistrement des alarmes */
   if ( ( vl_Fd = open ( pl_NomFichier, O_RDWR ) ) < 0 )
   {
      XZST_03EcritureTrace( XZSTC_WARNING, "Impossible d'ouvrir le fichier des dernieres sequences " );
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

int ex_lire_config_Wizcon	( )

/*
* ARGUMENTS EN ENTREE :
*   pa_NomMachine	: Nom de la machine
*
* ARGUMENTS EN SORTIE :
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
   int				vl_ValRet = XDC_OK;
   int				vl_Fd;
   char 			pl_PathEtFic[XDC_PATH_ABS_SIZE];
   char				*pl_Donnee;
   XDY_Commentaire		pl_LigneFich;
   int				vl_index=0;

	static char *version = "$Id: eniw_cfg.c,v 1.2 2002/04/24 08:46:53 gesconf Exp $      : et_lire_config_Wizcon " ;
      /*A Ouverture du fichier de config Wizcon */
      /* Construction du nom du fichier Config_Wizcon_GTCNice */
      sprintf ( pl_PathEtFic, "%s/%s", XZEXC_PATH_CONFIG, ENIW_Wizcon);
      
      /*A Ouverture du fichier  */
      if ( (vl_Fd = open (pl_PathEtFic, O_RDONLY)) <= 0 )
      {
         /*A Ecriture trace absence fichier */
         XZST_03EcritureTrace( XZSTC_WARNING, " Manque fichier %s. ", pl_PathEtFic ); 
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
             	/*A Extraire le nom de l'equipement et si il est present */
             	sscanf ( pl_LigneFich, 	"%s %d", vg_nom_machine_TWWI0,&vg_Port_SockTWWI0);
             				
             	XZST_03EcritureTrace( XZSTC_WARNING, "machine 0:%s Port TCPIP:%d", vg_nom_machine_TWWI0, vg_Port_SockTWWI0);
	     }
	     if (vl_index == 1)
	     {
             	/*A Extraire le nom de l'equipement et si il est present */
             	sscanf ( pl_LigneFich, 	"%s %d", vg_nom_machine_TWWI1,&vg_Port_SockTWWI1);
             				
             	XZST_03EcritureTrace( XZSTC_WARNING, "machine 1:%s Port TCPIP:%d", vg_nom_machine_TWWI1, vg_Port_SockTWWI1);
	     }
	     vl_index ++;
         }
      }
      /*A Fermeture du fichier XDF_Presence_Equip_<NomMachine>  */
      XZSS_13FermerFichier( vl_Fd );

      /*A Retourne compte rendu OK */
      return ( vl_ValRet );
}
