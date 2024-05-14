/*E*/
/*Fichier :  $Id: ex_msta.c,v 1.19 2011/02/23 18:09:14 gesconf Exp $      Release : $Revision: 1.19 $        Date : $Date: 2011/02/23 18:09:14 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  : EQUEXT
------------------------------------------------------
* MODULE COMMUN * FICHIER ex_msta.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Module commun pour envoi des status 
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	28 Oct 1994	: Creation
* Mismer.D	version 1.2	23 Nov 1994	:
* Nagiel.E	version 1.3	23 Nov 1994	:
* Mismer.D	version 1.4	28 Nov 1994	:
* Volcic.F	version 1.5	13 Dec 1994	: Ajout ex_traitER
* Mismer.D	version 1.6	06 Jan 1995	:
* Volcic.F	version 1.7	15 Fev 1995	:
* Mismer.D	version 1.8	08 Mar 1995	:
* Mismer.D	version 1.9	09 Avr 1995	:
* Mismer.D	version 1.10	22 Mai 1995	: Modif ex_decode_status, reprise historique
* Volcic.F	version 1.11	11 Jui 1995	: Modif ex_traitBAT
* Mismer.D	version 1.12	12 Dec 1995	: Modif ex_traitERR
* Mismer.D	version 1.13	15 Nov 1996	: Modif METEO (DEM/1232)
* Mismer.D	version 1.14	10 Fev 1997	: Modif TDP (DEM/1395)
* Mismer.D      version 1.15    25 Mar 1997     : Modif TDP suite (DEM/1396)
* Mismer.D      version 1.16    08 Jan 1999     : Modif new alerte DAI (DEM/APG17) fiche ANA74
* JMG   13/11/07        : ajout site de gestion DEM715
* JPL	23/02/11	: Migration architecture HP ia64 (DEM 975) : suppression warnings mineurs  1.19
------------------------------------------------------*/

/* fichiers inclus */

#include "ex_mala.h"

#include "ex_msta.h"

/* definitions de constantes */

/* definitions de types locaux */

typedef	struct	
{
	int	Numero;
	char	Status[6];
	int	Alarme;
	XDY_Mot	Fugitive;
	XDY_Mot	Complement;
	void *	Suivant;
}	EXT_DONNEES_STATUS;

/* definition de macro locales */

/* declaration de variables locales */

EXT_DONNEES_STATUS *	pg_DebutStatus = NULL;

static char *version = "$Id: ex_msta.c,v 1.19 2011/02/23 18:09:14 gesconf Exp $      :ex_msta" ;

/* declaration de fonctions internes */

/* Fonction interne pour insertion dans la liste des parametres */
EX_FONCT_INSERER   ( EXT_DONNEES_STATUS, ex_ins_liste_status, pg_DebutStatus, Numero, Suivant, Suivant)

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Lecture du fichier de liste des parametres de status.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int ex_lecture_fichier_status (	 )

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
*  Construction du message et envoi du message
*
------------------------------------------------------*/
{
   static char *version = "$Id: ex_msta.c,v 1.19 2011/02/23 18:09:14 gesconf Exp $      : ex_lecture_fichier_status";

   int				vl_Fd;
   char 			pl_PathEtFic[XDC_PATH_ABS_SIZE];
   XDY_Texte			pl_LigneFich;
   EXT_DONNEES_STATUS		vl_DonneesStatus = {0},
   				*pl_DonCrt;
   int				vl_Debut    = XDC_NOK;
   int				vl_index    = 0;
   XZSCT_NomTache  		vl_NomTache;
   
      /* Lecture du nom de la tache */
      XZSC_06NomTache( vl_NomTache );
      for ( vl_index = 0; vl_NomTache[vl_index] != '\0'; vl_index++ )
         if ( vl_NomTache[vl_index] == '.' ) vl_NomTache[vl_index] = '\0';
      
      /*A Ouverture du fichier des parametres */
      /* Construction du nom du fichier des parametres */
      sprintf ( pl_PathEtFic, "%s/%s", XZEXC_PATH_FIC_PARAM_STATUS, XDF_Param_Status );
      
      /*A Ouverture du fichier des parametres */
      if ( XZSS_12OuvrirFichier( O_RDONLY , pl_PathEtFic, &vl_Fd ) != XDC_OK )
      {
         /* Ecriture trace absence fichier */
         XZST_03EcritureTrace( XZSTC_WARNING, " Manque fichier %s. \n", pl_PathEtFic ); 
         /* Sortie de la fonction */
         return ( XDC_NOK );
      }
 
      /*A Lecture de chaque ligne du fichier */
      while ( ex_LireLigneFich ( vl_Fd, pl_LigneFich, sizeof ( pl_LigneFich ) ) != XZEXC_EOF ) 
      {
      	  /* Si debut de la liste des status de la tache concernee */
          if ( !strncmp(&pl_LigneFich[1], vl_NomTache, strlen(vl_NomTache)) )
          {
              /* Memorisation du debut de la liste */
              vl_Debut = XDC_OK;
          }
          /* Si fin de la liste des d'status de la tache concernee */
          if ( ( vl_Debut == XDC_OK ) && ( !strncmp(pl_LigneFich, "#fin", 4) ) )
          {
              /* fin de la lecture du fichier */
              break;
          }
          /* Lire ligne suivante si c'est une ligne de commentaire */
          if ( ( vl_Debut == XDC_OK ) && ( *pl_LigneFich != XDF_COMMENTAIRE ) )
          {
             /*B Extraire le texte de l'status */
             if ( sscanf ( pl_LigneFich, "%d %s %d %hd %hd",  
					&vl_DonneesStatus.Numero,
					vl_DonneesStatus.Status,
					&vl_DonneesStatus.Alarme,
					&vl_DonneesStatus.Fugitive,
					&vl_DonneesStatus.Complement ) >= 2 )
             {
                 /*B Inserer dans la liste des structures texte d'status */
                 ex_ins_liste_status ( &vl_DonneesStatus, 0 );
             }
         }
      }
      /*A Fermeture du fichier des parametres  */
      XZSS_13FermerFichier( vl_Fd );
      
     
      /* Ecriture trace de la config pour debug */
      for ( pl_DonCrt = pg_DebutStatus; pl_DonCrt != NULL; pl_DonCrt = pl_DonCrt->Suivant )
      {
          XZST_03EcritureTrace ( XZSTC_DEBUG1, "Status: %d %s %d %d %d",  
               			 pl_DonCrt->Numero,
				 pl_DonCrt->Status,
				 pl_DonCrt->Alarme,
				 pl_DonCrt->Fugitive,
				 pl_DonCrt->Complement );				
      }
      /* Retourne compte rendu OK */
      return ( XDC_OK );

}



/* definition de fonctions externes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Decodage des trames de status
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int ex_decode_status  ( char *			pa_TrameStatus,
			XZEXT_TEXTE_STATUS *	pa_TabStatus,
			int			va_SizeTab,
            		XDY_Rgs			va_adrrgs,
			XDY_Horodate		va_horodate,
            		int 			va_typeeqt,
            		XDY_Mot	        	va_noeqt,
            		XDY_NomMachine		va_NomMachine,
			XDY_District		va_SiteGestion)


/*
* ARGUMENTS EN ENTREE :
*
*   char *			pa_TrameStatus	: Trame a decomposee  
*   int				va_SizeTab      : Taille du tableau de reponse
*
* ARGUMENTS EN SORTIE :
*
*   XZEXT_TEXTE_STATUS *	pa_TabStatus	: Tableau de reponse
*
* CODE RETOUR :
*  XDC_OK / XDC_NOK
*
* CONDITION D'UTILISATION
* 
* FONCTION 
*  Pour chaque type de status, recherche dans la trame et mise a jour tableau retour
*
------------------------------------------------------*/
{
static char *version = "$Id: ex_msta.c,v 1.19 2011/02/23 18:09:14 gesconf Exp $      :  ex_decode_status ";
EXT_DONNEES_STATUS	*pl_DonCrt = NULL;
XZEXT_Trame		pl_Trame,
			vl_Tampon,
			vl_StatusRes;
char			vl_CodeStatus[7] = "";
char			*vl_Status,
			*vl_Mot,
 			*pl_TrameStatus;
int			vl_al;
XDY_Texte		vl_TexteArg = "";
int			vl_Index;

   XZST_03EcritureTrace( XZSTC_FONCTION , " IN : ex_decode_status "  );

   /*A Si la liste des parametres est vide */
   if ( pg_DebutStatus == NULL )
   {
      /*A Lecture de la liste des parametres */
      ex_lecture_fichier_status ();
   }
   
   /*A Initialisation du tableau */
   for ( vl_Index = 0; vl_Index < va_SizeTab; vl_Index++ )
   {
      strcpy ( pa_TabStatus[vl_Index], "" );
   }
   
   /*A Recherche du status concernee */
   for ( pl_DonCrt = pg_DebutStatus;
   	 pl_DonCrt != NULL;
   	 pl_DonCrt = pl_DonCrt->Suivant )
   {
      /* Init du pointeur de travail sur la trame de status */
      strcpy ( pl_Trame, pa_TrameStatus );
      strcat ( pl_Trame, " END=" );
      /* Si Num status trouve */
      sprintf ( vl_CodeStatus, "%s=\0", pl_DonCrt->Status );
      if ( ( pl_TrameStatus = strstr ( pl_Trame, vl_CodeStatus ) ) != NULL )
      {
         /* Separation du morceau de la trame concerne */
         vl_Status = strtok ( pl_TrameStatus, "=" );
         vl_Status = strtok ( NULL, "=" );
         /* Recherche de chaque mot significatif */
         vl_Mot = strtok ( vl_Status, " ");
         vl_Tampon[0] = '\0'; 
         while ( vl_Mot != NULL )
         {
            strcat ( vl_Tampon, vl_Mot );
            strcat ( vl_Tampon, " " );
            if ( ( vl_Mot = strtok ( NULL, " " ) ) != NULL )
            {
               strcpy ( vl_StatusRes, vl_Tampon );
            }
         }
	 
	 /*A Mise a jour du tableau de reponse */
	 if ( pl_DonCrt->Numero < va_SizeTab )
	 {
	    strcpy ( pa_TabStatus[pl_DonCrt->Numero], vl_StatusRes );
	    /* Si il n'y a pas de traitement complementaire envoyer l'alarme */
	    if ( (pl_DonCrt->Complement == XDC_FAUX) && (pl_DonCrt->Alarme > 0) )
	    {
               if (strncmp(pl_DonCrt->Status,"ADR",3) )
               {
                  if (atoi(vl_StatusRes)==0)
                  {
                    vl_al = XDC_FAUX;
                  }
                  else
                  {
                    vl_al = XDC_VRAI;
                    XZST_03EcritureTrace( XZSTC_INFO, "Defaut %s sur l'eqt no %d " , pl_DonCrt->Status, va_noeqt);
                  }
               }
               else
               {
                  if (!strncmp(vl_StatusRes,va_adrrgs,3))
                  {
                     vl_al = XDC_FAUX;
                  }
                  else
                  {
                     vl_al = XDC_VRAI;
                     XZST_03EcritureTrace( XZSTC_INFO, "Adresse RGS incoherente sur l'eqt no %d " ,va_noeqt);
                  }
               }
   
               /* envoi de l'alarme */
               sprintf ( vl_TexteArg, "%s%s\0", vl_CodeStatus, vl_StatusRes );
               if ( (pl_DonCrt->Fugitive == XDC_VRAI) && (vl_al == XDC_VRAI) )
               {
                  ex_env_alarme  (  va_NomMachine,
			            va_horodate,
			            va_typeeqt,
			            va_noeqt,
			            pl_DonCrt->Alarme,
			            XDC_VRAI,
			            vl_TexteArg,
				    va_SiteGestion);
                  ex_env_alarme  (  va_NomMachine,
			            va_horodate,
			            va_typeeqt,
			            va_noeqt,
			            pl_DonCrt->Alarme,
			            XDC_FAUX,
			            NULL,
				    va_SiteGestion);
               }
               else
               {
                  ex_env_alarme  (  va_NomMachine,
			            va_horodate,
			            va_typeeqt,
			            va_noeqt,
			            pl_DonCrt->Alarme,
			            vl_al,
			            (vl_al == XDC_VRAI) ? vl_TexteArg : NULL ,
				    va_SiteGestion);
	    
	       }
            }
	 }
      }
   }   

   for ( pl_DonCrt = pg_DebutStatus; pl_DonCrt != NULL;pl_DonCrt = pl_DonCrt->Suivant )
   {
      XZST_03EcritureTrace( XZSTC_DEBUG1 , "ex_decode_status: Tab[%d][%s]=<%s>", pl_DonCrt->Numero, pl_DonCrt->Status, pa_TabStatus[pl_DonCrt->Numero] );
   }
   
   XZST_03EcritureTrace( XZSTC_FONCTION , " OUT: ex_decode_status "  );
   return ( XDC_OK );

}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Decodage des trames de status complementaire
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int ex_decode_status_comp  ( char *			pa_TrameStatus,
			     char *			pa_Status,
			     int *			va_NumAlarme )


/*
* ARGUMENTS EN ENTREE :
*
*   char *			pa_TrameStatus	: Trame a decomposee  
*
* ARGUMENTS EN SORTIE :
*
*   XZEXT_TEXTE_STATUS *	pa_TabStatus	: Tableau de reponse
*
* CODE RETOUR :
*  XDC_OK / XDC_NOK
*
* CONDITION D'UTILISATION
* 
* FONCTION 
*  Pour chaque type de status, recherche dans la trame et mise a jour tableau retour
*
------------------------------------------------------*/
{
static char *version = "$Id: ex_msta.c,v 1.19 2011/02/23 18:09:14 gesconf Exp $      :  ex_decode_status_comp ";
EXT_DONNEES_STATUS	*pl_DonCrt = NULL;
XZEXT_Trame		pl_Trame;
char			vl_CodeStatus[7] = "";
char			*pl_TrameStatus = NULL;

   XZST_03EcritureTrace( XZSTC_FONCTION , " IN : ex_decode_status_comp "  );

   /*A Si la liste des parametres est vide */
   if ( pg_DebutStatus == NULL )
   {
      /*A Lecture de la liste des parametres */
      ex_lecture_fichier_status ();
   }
      
   /*A Recherche du status concernee */
   for ( pl_DonCrt = pg_DebutStatus;
   	 pl_DonCrt != NULL;
   	 pl_DonCrt = pl_DonCrt->Suivant )
   {
      if ( (pl_DonCrt->Complement == XDC_VRAI) && (pl_DonCrt->Alarme > 0) )
      {
         /* Init du pointeur de travail sur la trame de status */
         strcpy ( pl_Trame, pa_TrameStatus );
         /* Si Num status trouve */
         sprintf ( vl_CodeStatus, "%s\0", pl_DonCrt->Status );
         if ( ( pl_TrameStatus = strstr ( pl_Trame, vl_CodeStatus ) ) != NULL )
         {
	    /*A Mise a jour de la reponse */
	    strcpy ( pa_Status, strtok( pl_TrameStatus, XZEXC_FIN_MSG_SOCK) );
	    *va_NumAlarme = pl_DonCrt->Alarme;
	    break;
	 }
      }
   }   

      
   XZST_03EcritureTrace( XZSTC_FONCTION , " OUT: ex_decode_status_comp <%s>", pa_Status );
   return ( XDC_OK );

}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Liberation memoire parametre status.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void ex_liberation_memoire_status (	 )

/*
* ARGUMENTS EN ENTREE :
*
*  aucun
*
* ARGUMENTS EN SORTIE :
*  aucun
*
* CODE RETOUR :
*  aucun
*
* CONDITION D'UTILISATION
* 
* FONCTION 
*  Liberation memoire parametre status.
*
------------------------------------------------------*/
{
static char *version = "$Id: ex_msta.c,v 1.19 2011/02/23 18:09:14 gesconf Exp $      :  ex_liberation_memoire_status ";
EXT_DONNEES_STATUS	*pl_DonCrt,
			*pl_DonSup;

	
      /* Liberation memoire parametre status */
      for ( pl_DonCrt = pl_DonSup = pg_DebutStatus;
            pl_DonCrt != NULL; 
            pl_DonSup = pl_DonCrt )
      {
          pl_DonCrt = pl_DonCrt->Suivant;				
          free ( pl_DonSup );
      }
      
      pg_DebutStatus = NULL;
      

}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  analyse parametre ERI et envoi l'alarme 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void ex_traitERI_PMV(XZEXT_TEXTE_STATUS	va_tabstatus,
            XDY_Horodate	va_horodate,
            int 		va_typeeqt,
            XDY_Mot	        va_noeqt,
            XDY_NomMachine	va_NomMachine,
	    XDY_District	va_SiteGestion)

/*
* ARGUMENTS EN ENTREE :
*   
* vl_tabstatus : donnee ADR du status
*
* ARGUMENTS EN SORTIE :
*
* CODE RETOUR : 
*
* CONDITION D'UTILISATION
*
* FONCTION 
*
------------------------------------------------------*/
{
   static char *version = "$Id: ex_msta.c,v 1.19 2011/02/23 18:09:14 gesconf Exp $      : ex_traitERI_PMV" ;
   int	 vl_al		= 0;
   char  vl_ideqt[3]	= "";
   float vl_nomod	= 0;
   int   vl_noerr	= 0,
         vl_etateqt	= 0,
         vl_no		= 0;
  int    vl_nbparam	= 0;
  XDY_Texte	vl_TexteArg = "";
 
   vl_nbparam=sscanf(va_tabstatus,"%2c%2d/%f/%d"
                      		 ,vl_ideqt
                      		 ,&vl_noerr
                      		 ,&vl_nomod
                      		 ,&vl_etateqt);
                      		 
   if (vl_nbparam != 4) vl_etateqt = 0;
   
   if (vl_etateqt==0)
   {
     vl_al = XDC_FAUX;
   }
   else
   {
     vl_al = XDC_VRAI;
     XZST_03EcritureTrace( XZSTC_INFO, "Erreur pmv sur l'eqt no:%d id eqt:%s module:%d" 
                                        ,va_noeqt
                                        ,vl_ideqt
                                        ,vl_nomod);
   }
   
   switch(vl_noerr)
   {
      case 1:
      {
         vl_no=XZAMC_ERR_MESS_PERDU;
         break;
      }
      case 2:
      {
         vl_no=XZAMC_ERR_HORL_HS;
         break;
      }
      case 3:
      {
         vl_no=XZAMC_ERR_PB_MODU_MIN;
         break;
      }
      case 4:
      {
         vl_no=XZAMC_ERR_PB_MODU_MAJ;
         break;
      }
      case 5:
      {
         vl_no=XZAMC_ERR_PB_LAMPE_MIN;
         break;
      }
      case 6:
      {
         vl_no=XZAMC_ERR_PB_LAMPE_MAJ;
         break;
      }
      case 7:
      {
         vl_no=XZAMC_ERR_PB_TEMP;
         break;
      }
      case 8:
      {
         vl_no=XZAMC_ERR_PB_CELL;
         break;
      }
       case 9:
      {
         vl_no=XZAMC_ERR_DEF_TRANS;
         break;
      }
      case 10:
      {
         vl_no=XZAMC_ERR_DEF_ALIM_LAMP;
      }
  }
   
   /* envoi de l'alarme */
   if ( vl_no != 0 )
   {
      sprintf ( vl_TexteArg, "module: %f\0", vl_nomod );
      ex_env_alarme  ( 	va_NomMachine,
			va_horodate,
			va_typeeqt,
			va_noeqt,
			vl_no,
			vl_al,
			vl_TexteArg,
			va_SiteGestion);
   }

}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  analyse parametre ERI et envoi l'alarme 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void ex_traitERI_NAV(XZEXT_TEXTE_STATUS	va_tabstatus,
            XDY_Horodate	va_horodate,
            int 		va_typeeqt,
            XDY_Mot	        va_noeqt,
            XDY_NomMachine	va_NomMachine,
	    XDY_District	va_SiteGestion)

/*
* ARGUMENTS EN ENTREE :
*   
* vl_tabstatus : donnee ADR du status
*
* ARGUMENTS EN SORTIE :
*
* CODE RETOUR : 
*
* CONDITION D'UTILISATION
*
* FONCTION 
*
------------------------------------------------------*/
{
   static char *version = "$Id: ex_msta.c,v 1.19 2011/02/23 18:09:14 gesconf Exp $      : ex_traitERI_NAV" ;
   int	vl_al		= 0;
   char vl_ideqt[3]	= "";
   int  vl_noerr	= 0;
   int vl_nomod1	= 0;
   int vl_nomod2	= 0;
   int vl_nomod3	= 0;
   char vl_nomod	= 0;
   int  vl_etateqt	= 0,
        vl_no		= 0;
  int   vl_nbparam	= 0;
  int	vl_poub		= 0;
  char 	*pl_ptr		= NULL;
  XDY_Texte	vl_TexteArg = "";
 
   if ( (pl_ptr=strtok(va_tabstatus ,"/")) != NULL )
      vl_nbparam=sscanf(va_tabstatus, "%2c%2d", vl_ideqt, &vl_noerr);
                      		 
   if ( (pl_ptr=strtok(NULL ,"/")) != NULL )
   {
      if (strcmp( pl_ptr , "A" ))
         vl_nbparam=sscanf(pl_ptr, "%d.%d.%d", &vl_nomod1, &vl_nomod2, &vl_nomod3);
   }
   
   if ( (pl_ptr=strtok(NULL ,"/")) != NULL )             		 
      vl_nbparam=sscanf(pl_ptr, "%d", &vl_etateqt);
                         
   if (vl_etateqt==0)
   {
     vl_al = XDC_FAUX;
   }
   else
   {
     vl_al = XDC_VRAI;
     XZST_03EcritureTrace( XZSTC_WARNING, "Erreur nav sur l'eqt no:%d id eqt:%s module:%d.%d" 
                                        ,va_noeqt
                                        ,vl_ideqt
                                        ,vl_nomod1
                                        ,vl_nomod2);
   }
   
   sprintf ( vl_TexteArg, "mod: %d.%d\0", vl_nomod1, vl_nomod2 );
   
   switch(vl_noerr)
   {
      case 1:
      {
         vl_no=XZAMC_ERR_MESS_PERDU;
         break;
      }
      case 2:
      {
         vl_no=XZAMC_ERR_HORL_HS;
         break;
      }
      case 4:
      {
         vl_no=XZAMC_ERR_PB_MODU_MIN;
         break;
      }
      case 3:
      {
         vl_no=XZAMC_ERR_PB_MODU_MAJ;
         break;
      }
      case 6:
      {
         vl_no=XZAMC_ERR_PB_LAMPE_MIN;
         break;
      }
      case 5:
      {
         vl_no=XZAMC_ERR_PB_LAMPE_MAJ;
         break;
      }
      case 7:
      {
         vl_no=XZAMC_ERR_PB_TEMP;
         break;
      }
      case 8:
      {
         vl_no=XZAMC_ERR_PB_CELL;
         break;
      }
       case 9:
      {
         vl_no=XZAMC_ERR_DEF_TRANS;
         break;
      }
      case 10:
      {
         vl_no=XZAMC_ERR_DEF_ALIM_LAMP;
      }
      case 11:
      {
         vl_no=XZAMC_ERR_DEF_FCT_VERIN;
      }
  }
   
   /* envoi de l'alarme */
   if ( vl_no != 0 )
      ex_env_alarme  ( 	va_NomMachine,
			va_horodate,
			va_typeeqt,
			va_noeqt,
			vl_no,
			vl_al,
			vl_TexteArg ,
			va_SiteGestion);

}
/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  analyse parametre BAT et envoi l'alarme 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void ex_traitBAT(XZEXT_TEXTE_STATUS	va_tabstatus,
            XDY_Horodate	va_horodate,
            int 		va_typeeqt,
            XDY_Mot	        va_noeqt,
            XDY_NomMachine	va_NomMachine,
	    XDY_District	va_SiteGestion)

/*
* ARGUMENTS EN ENTREE :
*   
* vl_tabstatus : donnee ADR du status
*
* ARGUMENTS EN SORTIE :
*
* CODE RETOUR : 
*
* CONDITION D'UTILISATION
*
* FONCTION 
*
------------------------------------------------------*/
{
   static char *version = "@(#)ex_msta.c	1.13 12/05/96 : ex_traitBAT" ;
   int	vl_al;
   XDY_Texte	vl_TexteArg = "";
   
   if (va_typeeqt != XDC_EQT_RAD)
      if (atoi(va_tabstatus)==0)
         vl_al = XDC_FAUX;
      else
      {
         vl_al = XDC_VRAI;
         XZST_03EcritureTrace( XZSTC_INFO, "Defaut batterie sur l'eqt no %d " ,va_noeqt);
      }
   else
      if (atoi(va_tabstatus)!=0)
         vl_al = XDC_FAUX;
      else
      {
         vl_al = XDC_VRAI;
         XZST_03EcritureTrace( XZSTC_INFO, "Defaut batterie sur l'eqt no %d " ,va_noeqt);
      }
   
   /* envoi de l'alarme */
   sprintf ( vl_TexteArg, "BAT: %s\0", va_tabstatus );
   ex_env_alarme  ( 	va_NomMachine,
			va_horodate,
			va_typeeqt,
			va_noeqt,
			XZAMC_ERR_DEF_BATTERIE,
			vl_al,
			(vl_al == XDC_VRAI) ? vl_TexteArg : NULL,
			va_SiteGestion);

}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  analyse parametre BTR et envoi l'alarme 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void ex_traitBTR(XZEXT_TEXTE_STATUS	va_tabstatus,
            XDY_Horodate	va_horodate,
            int 		va_typeeqt,
            XDY_Mot	        va_noeqt,
            XDY_NomMachine	va_NomMachine,
	    XDY_District	va_SiteGestion)

/*
* ARGUMENTS EN ENTREE :
*   
* vl_tabstatus : donnee ADR du status
*
* ARGUMENTS EN SORTIE :
*
* CODE RETOUR : 
*
* CONDITION D'UTILISATION
*
* FONCTION 
*
------------------------------------------------------*/
{
   static char *version = "@(#)ex_msta.c	1.13 12/05/96 : ex_traitBTR" ;
   int	vl_al;
   XDY_Texte	vl_TexteArg = "";
   
   if (atoi(va_tabstatus)==0)
   {
     vl_al = XDC_FAUX;
   }
   else
   {
     vl_al = XDC_VRAI;
     XZST_03EcritureTrace( XZSTC_INFO, "Panne batterie sur l'eqt no %d " ,va_noeqt);
  }
   
   /* envoi de l'alarme */
   sprintf ( vl_TexteArg, "BTR: %s\0", va_tabstatus );
   ex_env_alarme  ( 	va_NomMachine,
			va_horodate,
			va_typeeqt,
			va_noeqt,
			XZAMC_ERR_PAN_BATTERIE,
			vl_al,
			(vl_al == XDC_VRAI) ? vl_TexteArg : NULL ,
			va_SiteGestion);

}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Decodage des trames de alerte DAI
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int ex_decode_alerte_dai  ( 	char 	*pa_Trame,
	            		int 	*va_NumAlerte,
            			int	*va_Type )


/*
* ARGUMENTS EN ENTREE :
*
*   char *			pa_Trame	: Trame a decomposee  
*
* ARGUMENTS EN SORTIE :
*
*   va_NumAlerte : Id alerte trouve
*
* CODE RETOUR :
*  XDC_OK / XDC_NOK
*
* CONDITION D'UTILISATION
* 
* FONCTION 
*
------------------------------------------------------*/
{
static char *version = "$Id: ex_msta.c,v 1.19 2011/02/23 18:09:14 gesconf Exp $      :  ex_decode_alerte_dai ";
EXT_DONNEES_STATUS	*pl_DonCrt = NULL;

   XZST_03EcritureTrace( XZSTC_FONCTION , " IN : ex_decode_status "  );

   /*A Si la liste des parametres est vide */
   if ( pg_DebutStatus == NULL )
   {
      /*A Lecture de la liste des parametres */
      ex_lecture_fichier_status ();
   }
   
   
   /*A Recherche du status concernee */
   for ( pl_DonCrt = pg_DebutStatus;
   	 pl_DonCrt != NULL;
   	 pl_DonCrt = pl_DonCrt->Suivant )
   {
      if (!strcmp(pa_Trame,pl_DonCrt->Status))
      { 
      	  /* Alerte trouvee */
          *va_NumAlerte = pl_DonCrt->Alarme;
          *va_Type = pl_DonCrt->Complement;
       	  break;
      }
   }   

   
   XZST_03EcritureTrace( XZSTC_FONCTION , " OUT: ex_decode_status "  );
   return ( XDC_OK );

}
