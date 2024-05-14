/*E*/
/*Fichier : $Id: ex_mdon.c,v 1.25 2020/11/03 18:12:43 pc2dpdy Exp $      Release : $Revision: 1.25 $        Date : $Date: 2020/11/03 18:12:43 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  : EQUEXT
------------------------------------------------------
* MODULE COMMUN * FICHIER ex_mdon.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Declaration des donnees communes au Sous-Systeme EQUEXT
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	06 Sep 1994	: Creation
* Mismer.D	version 1.2	23 Nov 1994	:
* Volcic.F	version 1.3	25 Jan 1995	:
* Mismer.D	version 1.4	09 Fev 1995	:
* Fontaine.C	version 1.5	23 Mai 1995	: Reprise entete, historique
* Mismer.D	version 1.6	23 Mai 1995	: Mise a jour type equipement GTC Nice
* Volcic.F	version 1.7	21 Jun 1995	: Ajout de ex_lire_suppleant, ex_presence_machine et ex_cp_heure_RADT
* Volcic.F	version 1.8	04 Jui 1995	: Modif ex_cp_heure_RADT
* Volcic.F	version 1.9	17 Jui 1995	: Modifs diverses
* Volcic.F	version 1.10	26 Sep 1995	: Modif traces ex_cp_heure_RADT
* Mismer.D	version 1.11	20 Oct 1995	: Ajout fonction ex_lire_serveur
* Mismer.D	version 1.12	08 Fev 1996	: Suppression fonction ex_presence_machine
* Mismer.D      version 1.13    12 Jun 1996     : Ajout fonction ex_lire_idpassw (DEM/1135)
* Mismer.D	version 1.14	13 Nov 1996	: Modif METEO (DEM/1232)
* Mismer.D	version 1.15	10 Fev 1997	: Modif TDP (DEM/1395)
* Mismer.D	version 1.16	24 Fev 1997	: Modif TDP suite (DEM/1395)
* Mismer.D	version 1.11	15 Jan 1998	: Modif FAC et FAU (DEM/1544)
* JMG				11/10/05	: ajout PMVA BAF BAD
* JBL				15/10/2008  : ajout gestion des types d'eqt GTC renovee
* JPL		23/02/11 : Migration architecture HP ia64 (DEM 975) : suppr. '=' au lieu de '==' (!!! effet de bord !!!)  1.20
* JPL		23/02/11 : Migration architecture HP ia64 (DEM 975) : adresses des champs 'scanf'  1.21
* JPL		23/02/11 : Migration HP ia64 (DEM 975) : Gestion conversions types de tailles differentes (=,-)  1.22
* VR	20/12/11		: Ajout PRV (DEM/1016)
* JMG	2018	ajout CFE+CAP
* CGR	29/07/20	: Ajout IMU 1.25 DEM-SAE155
------------------------------------------------------*/

/* fichiers inclus */

#include "ex_mdon.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: ex_mdon.c,v 1.25 2020/11/03 18:12:43 pc2dpdy Exp $ : ex_mdon";

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Fonction appel≈e pour lire une ligne dans un fichier ASCII
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	ex_LireLigneFich ( 	int 	va_Fd,
				char	*pa_Ligne,
				int	va_NbCar )


/*
* ARGUMENTS EN ENTREE :
*  va_Fd		File descripteur du fichier
*  va_NbCar		Taille maxi de la ligne
*
* ARGUMENTS EN SORTIE :
*  pa_Ligne		Contenu de la ligne lu
*
* CODE RETOUR : 
*  int			Nombre caratere retourner
*  XZEXC_EOF		Fin du fichier atteindre
*  XDC_NOK		echec lecture ligne
*
* CONDITION D'UTILISATION
*
* FONCTION 

*
------------------------------------------------------*/
{
static char *version = "$Id: ex_mdon.c,v 1.25 2020/11/03 18:12:43 pc2dpdy Exp $ : ex_LireLigneFich ";
int			vl_ValRet;
XDY_Texte		pl_Ligne = {0};
int 			vl_Size = sizeof ( pl_Ligne );
ssize_t			vl_NbCarLu = 0;
size_t			vl_LongLigne;

   if ( va_NbCar < sizeof ( pl_Ligne ) )
   {
      /*A Mise a jour du nombre de caractere a lire */
      vl_Size = va_NbCar;
   }
        
   /*A Lecture du fichier */
   if ( ( vl_NbCarLu = read ( va_Fd, pl_Ligne, vl_Size ) ) > 0)
   {
      /*A Recherche de la longueur de la ligne */
      vl_LongLigne = strcspn( pl_Ligne, XZEXC_LINE_FEED);
      /*A Recopie du contenu des caractere lu pour l'appelant */
      pl_Ligne[vl_LongLigne] = '\0';
      memcpy ( pa_Ligne, pl_Ligne, strlen(pl_Ligne));
      strcpy ( pa_Ligne, pl_Ligne);  
      /*A Positionnement du pointeur du fichier au debut de la ligne suivante */
      lseek ( va_Fd, vl_LongLigne + 1 - vl_NbCarLu , SEEK_CUR );
      /*A Recopie du nombre de caractere lu pour le code retour de la fonction */
      vl_ValRet = (int) vl_LongLigne;
   }
   else
   {
      /*A SI le nombre caractere lu est nul */
      if ( vl_NbCarLu == 0)
      {
         /*A Retourner fin de fichier atteinte */
         vl_ValRet = XZEXC_EOF;
      }
      else
      {
         /*A Retourner erreur lecture fichier */
         vl_ValRet = XZEXC_EOF/*XDC_NOK*/;
      }
   }
   return ( vl_ValRet );  
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Fonction retournant le nom de l'equipement correspondant
*  au type d'equipement fournit en entree
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int ex_nom_eqt ( int	va_TypeEqt , char	*pa_NomEqt )

/*
* ARGUMENTS EN ENTREE :
*  va_TypeEqt		: Type d'equipement
*
*
* ARGUMENTS EN SORTIE :
*  paNomEqt		: Nom de l'equipement
*
*
* CODE RETOUR : 
*  XDC_OK		: Succes
*  XDC_NOK		: Echec
*
* CONDITION D'UTILISATION
*    
*
* FONCTION 
*   
*
------------------------------------------------------*/
{
static char *version = "$Id: ex_mdon.c,v 1.25 2020/11/03 18:12:43 pc2dpdy Exp $ : ex_nom_eqt ";
int	vl_ValRet = XDC_OK;

	/*A Selon le type d'equipement recopie du nom de l'quipement dans l'argument de sortie */
  	switch ( va_TypeEqt )
  	{
   	case 	XZECC_TYPE_EQT_RAU	: 
   	case 	XDC_EQT_PAU		: 
   			strcpy ( pa_NomEqt, XZECC_TACHE_RAU );
   			break;
   	case 	XDC_EQT_GEN_LCR		: 
   			strcpy ( pa_NomEqt, XZECC_TACHE_QTX );
   			break;
   	case 	XDC_EQT_TDP		: 
   	case 	XDC_EQT_ZDP		: 
   			strcpy ( pa_NomEqt, XZECC_TACHE_TDP );
   			break;
   	case	XZECC_TYPE_EQT_TEL	: 
   			strcpy ( pa_NomEqt, XZECC_TACHE_TEL );
   			break;
	case	XZECC_TYPE_EQT_OPE	: 
			strcpy ( pa_NomEqt, XZECC_TACHE_OPE );
			break;
	case	XZECC_TYPE_EQT_VID	: 
	case	XZECC_TYPE_EQT_MAGN	: 
			strcpy ( pa_NomEqt, XZECC_TACHE_VID );
			break;
	case	XZECC_TYPE_EQT_NIC	: 
	case	XDC_EQT_TUBE		: 
	case	XDC_EQT_ECHANGEUR	: 
	case	XDC_EQT_CPT		: 
	case	XDC_EQT_PAN		: 
			strcpy ( pa_NomEqt, XZECC_TACHE_NIC );
			break;
	case	XZECC_TYPE_EQT_SAT	: 
			strcpy ( pa_NomEqt, XZECC_TACHE_SAT );
			break;
	case	XZECC_TYPE_EQT_RDT	: 
			strcpy ( pa_NomEqt, XZECC_TACHE_RDT );
			 break;
	case	XZECC_TYPE_EQT_PAL	: 
			strcpy ( pa_NomEqt, XZECC_TACHE_PAL );
			 break;
	case	XZECC_TYPE_EQT_DAI	: 
	case	XZECC_TYPE_EQT_ANA	: 
			strcpy ( pa_NomEqt, XZECC_TACHE_DAI );
			break;
	case	XZECC_TYPE_EQT_PMV	: 
	case	XZECC_TYPE_EQT_PICTO	: 
			strcpy ( pa_NomEqt, XZECC_TACHE_PMV );
			break;
	case    XZECC_TYPE_EQT_PMVA	:
			strcpy ( pa_NomEqt, XZECC_TACHE_PMVA);
			break;
	case	XZECC_TYPE_EQT_NAV	: 
	case	XDC_EQT_FAU		: 
	case	XDC_EQT_FAC		: 
	case	XDC_EQT_MOD		: 
			strcpy ( pa_NomEqt, XZECC_TACHE_NAV );
			break;
	case	XZECC_TYPE_EQT_BAF	:
	case	XDC_EQT_MODBAF		:
			strcpy ( pa_NomEqt, XZECC_TACHE_BAF);
			break;
	case	XZECC_TYPE_EQT_BAD	:
	case	XDC_EQT_MODBAD		:
			strcpy ( pa_NomEqt, XZECC_TACHE_BAD);
			break;
	case    XZECC_TYPE_EQT_PRV	:
			strcpy ( pa_NomEqt, XZECC_TACHE_PRV);
			break;
	case 	XZECC_TYPE_EQT_CFE :	
			strcpy ( pa_NomEqt, XZECC_TACHE_CFE);
			break;
	case    XZECC_TYPE_EQT_CAP:
			strcpy ( pa_NomEqt, XZECC_TACHE_CAP);
			break;
	case    XZECC_TYPE_EQT_IMU:
			strcpy ( pa_NomEqt, XZECC_TACHE_IMU);
			break;
	case	XZECC_TYPE_EQT_GTC	: 
	case	XDC_EQT_TOR		: 
			strcpy ( pa_NomEqt, XZECC_TACHE_GTC );
			break;
   	default				: 
			if (va_TypeEqt >= XDC_DEBUT_TYPEQT_GTCRENOVEE)  /* GTC renovee */
			{
				strcpy ( pa_NomEqt, XZECC_TACHE_NIC );	
			}
			else
			{
				strcpy ( pa_NomEqt, XZECC_TACHE_QTX );
			}
   			break;
	}
	
	return ( vl_ValRet );
      
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Cette fonction effectue la lecture du fichier XDF_Machine_Sup
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int ex_lire_suppleant	( XDY_NomMachine *pa_NomMachine )

/*
* ARGUMENTS EN ENTREE :
*
* ARGUMENTS EN SORTIE :
*
* XDY_NomMachine *pa_NomMachine
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
   static char *version = "$Id: ex_mdon.c,v 1.25 2020/11/03 18:12:43 pc2dpdy Exp $ : ex_lire_suppleant " ;
   int				vl_Fd;
   char 			pl_PathEtFic[XDC_PATH_ABS_SIZE];
   XDY_Commentaire		pl_LigneFich;
   int				vl_NbConfLigne = 0;
   XDY_NomMachine       	pl_NomMachine;
   

        /*A Recuperation du nom de la machine */
	if ( XZSC_07NomMachine(pl_NomMachine) != XDC_OK)
	{
	   /* Ecriture Trace */
	   XZST_03EcritureTrace( XZSTC_WARNING, "Recuperation du Nom de la machine impossible ." );
	}
	 
      /*A
       * Lecture du fichier de configuration  XDF_Machine_Sup
       */
      /*A Construction du nom du fichier XDF_Machine_Sup  */
      sprintf ( pl_PathEtFic, "../fichiers/deq/%s_%s", XDF_Machine_Sup, pl_NomMachine );
      
      /*A Ouverture du fichier XDF_Machine_Sup  */
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
             /*A Extraire le nom de l'equipement et l'option presence */
             if ( sscanf ( pl_LigneFich, "%s %s", pl_NomMachine, *pa_NomMachine ) != 2 )
             {
                 XZST_03EcritureTrace( XZSTC_WARNING, " Impossible de lire le nom de la machine supleante " ); 
             }
             else
             {
                 XZST_03EcritureTrace( XZSTC_INFO, " Nom de la machine supleante:%s et surveillee :%s", 
                 			pl_NomMachine, pa_NomMachine ); 
             }
          }
      }
      /*A Fermeture du fichier  XDF_Config_Lignes_<NomMachine> */
      XZSS_13FermerFichier( vl_Fd );
      /*A Retourne compte rendu OK */
      return ( XDC_OK );
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Cette fonction effectue la lecture du fichier XDF_Nom_Serveur.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int ex_lire_serveur	( XDY_NomMachine *pa_NomServeur )

/*
* ARGUMENTS EN ENTREE :
*
* ARGUMENTS EN SORTIE :
*
* XDY_NomMachine *pa_NomMachine
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
   static char *version = "$Id: ex_mdon.c,v 1.25 2020/11/03 18:12:43 pc2dpdy Exp $ : ex_lire_serveur " ;
   int				vl_Fd;
   int				vl_ValRet = XDC_OK;
   char 			pl_PathEtFic[XDC_PATH_ABS_SIZE];
   XDY_Commentaire		pl_LigneFich;

       /*A Construction du nom du fichier XDF_Nom_Serveur  */
       sprintf ( pl_PathEtFic, "%s/%s", XZEXC_PATH_CONFIG, XDF_Nom_Serveur );
       
       /*A Ouverture du fichier XDF_Nom_Serveur  */
       if ( (vl_Fd = open (pl_PathEtFic, O_RDONLY)) <= 0 )
       {
          /*A Ecriture trace absence fichier */
          XZST_03EcritureTrace( XZSTC_FATAL, " Manque fichier %s. ", pl_PathEtFic );
       }
       else
       {

          /*A Lecture de chaque ligne du fichier */
          while ( ex_LireLigneFich ( vl_Fd, pl_LigneFich, sizeof ( pl_LigneFich ) ) != XZEXC_EOF )
          {
             /*A Lire ligne suivante si c'est une ligne de commentaire */
             if ( *pl_LigneFich != XDF_COMMENTAIRE )
             {
                /*A Extraire le nom de l'equipement et l'option presence */
                if ( sscanf ( pl_LigneFich, "%s", *pa_NomServeur ) != 1 )
                {
                   XZST_03EcritureTrace( XZSTC_WARNING, "Impossible de lire le nom du serveur " );
                   vl_ValRet = XDC_NOK;
                }
                else
                {
                   XZST_03EcritureTrace( XZSTC_INFO, "Nom du serveur :%s ", pa_NomServeur );
                }
             }
          }
          /*A Fermeture du fichier XDF_Nom_Serveur  */
          XZSS_13FermerFichier( vl_Fd );
        }
        
        return ( vl_ValRet );

}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	ex_cp_heure_RADT ( 	XDY_NomMachine	pa_NomMachine, XDY_NomMachine	pa_Suppleant)

/*
* ARGUMENTS EN ENTREE :
*   XDY_NomMachine	pa_NomMachine	nom machine destinatrice
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
* CODE RETOUR : 
*   Aucun
*
* CONDITION D'UTILISATION
*   
*
* FONCTION 
*   Test si une machine est accessible sur le reseau
*
------------------------------------------------------*/
{
        char    pl_Cmd[512]              = "\0";
        FILE    *pl_P                    = NULL ;


      /* construit la commande test reseau de la machine demandee */
      sprintf(pl_Cmd,"rcp %s/%s_%s_R* %s:%s",XZEXC_PATH_DIFF_DYN,XZEXC_HEURE_QVTO,pa_NomMachine,
                        pa_Suppleant,XZEXC_PATH_DIFF_DYN);
      
      XZST_03EcritureTrace( XZSTC_INFO, "ex_cp_heure_RADT : %s", pl_Cmd ); 

      /* execute la commande, */
      if ((pl_P = popen(pl_Cmd,"r")) != (FILE *)NULL)
         pclose(pl_P);
      else
         XZST_03EcritureTrace( XZSTC_WARNING, "ex_cp_heure_RADT : Echec execution Cmde Shell %s", pl_Cmd );

      return (XDC_OK);
}       

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Permet de configurer l'ID et le password
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int ex_lire_idpassw     ( char        *pa_NomMachine, char * va_idpassw  )

/*
* ARGUMENTS EN ENTREE :
*   pa_NomMachine       : Nom de la machine
*
* ARGUMENTS EN SORTIE :
*   va_idpassw          : Nom utilisateur/ Mot de passe
*
* CODE RETOUR :
*   aucun
*
* CONDITION D'UTILISATION
*
* FONCTION
*
* Permet de configurer l'ID et le password
*
------------------------------------------------------*/
{
   static char *version = "$Id: ex_mdon.c,v 1.25 2020/11/03 18:12:43 pc2dpdy Exp $ : ex_lire_idpassw" ;

   int                          vl_Fd;
   char               pl_PathEtFic[100];
   XDY_Texte                    pl_LigneFich;
   int                          vl_NbParam = 0;


      XZST_03EcritureTrace ( XZSTC_FONCTION, "ex_lire_idpassw");
      XZST_03EcritureTrace ( XZSTC_FONCTION, "ex_lire_idpassw");
      XZST_03EcritureTrace ( XZSTC_FONCTION, "ex_lire_idpassw");
      XZST_03EcritureTrace ( XZSTC_FONCTION, "ex_lire_idpassw");
      XZST_03EcritureTrace ( XZSTC_FONCTION, "ex_lire_idpassw");
      XZST_03EcritureTrace ( XZSTC_FONCTION, "IN : %s", pa_NomMachine);

      /*A Ouverture du fichier de configuration des Passwords et Construction du nom du fichier XDF_Config_IDPASSW_<NomMachine> */
      sprintf ( pl_PathEtFic, "%s/%s_%s", XZEXC_PATH_CONFIG, XDF_Config_IDPASSW, pa_NomMachine );

      XZST_03EcritureTrace ( XZSTC_FONCTION, "%s",pl_PathEtFic);

      /*A Ouverture du fichier XDF_Config_IDPASSW_<NomMachine> */
      if ( ( vl_Fd = open ( pl_PathEtFic, O_RDONLY ) ) == -1 )
      {
         XZST_03EcritureTrace( XZSTC_WARNING, "ex_lire_idpassw : Fichier %s introuvable \n", pl_PathEtFic );
         XZST_03EcritureTrace ( XZSTC_FONCTION, "OUT : %s", version);
         return (XDC_NOK);
      }
      /*B Lecture du fichier password (commentaire) => Aucun traitement sinon format : ID/PSSWD */
      while ( ex_LireLigneFich ( vl_Fd, pl_LigneFich, sizeof(pl_LigneFich) ) != XZEXC_EOF )
      {
         if ( *pl_LigneFich != XDF_COMMENTAIRE )
         {
	    XZST_03EcritureTrace ( XZSTC_FONCTION, "%s",pl_LigneFich);
            /*A Affectation du password lu dans la variable va_idpassw */
            vl_NbParam = sscanf ( pl_LigneFich, "%s", va_idpassw);
            if ( sscanf ( pl_LigneFich, "%s", va_idpassw) != 1 )
            {
              XZST_03EcritureTrace( XZSTC_WARNING,"ex_lire_idpassw : Password incorrect");
              close (vl_Fd);
              return (XDC_NOK);
            }
            else
            {
              XZST_03EcritureTrace ( XZSTC_FONCTION, "Config IdPassw: %s", va_idpassw);
              break;
            }
         }
      }

      XZST_03EcritureTrace ( XZSTC_FONCTION, "OUT : %s", version);
      close (vl_Fd);
      return (XDC_OK);
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Permet de configurer le clignotement lent et le clignotement rapide
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	ex_lire_cligno	( XDY_NomMachine	pa_NomMachine, char * va_clignolent, char * va_clignorapide )

/*
* ARGUMENTS EN ENTREE :
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
*
------------------------------------------------------*/
{
   static char *version = "$Id: ex_mdon.c,v 1.25 2020/11/03 18:12:43 pc2dpdy Exp $ : ex_lire_cligno" ;

   int				vl_Fd;
   char 			pl_PathEtFic[XDC_PATH_ABS_SIZE];
   XDY_Texte			pl_LigneFich;
   
      XZST_03EcritureTrace ( XZSTC_FONCTION, "IN : %s", version);

      /*A Ouverture du fichier de configuration des clignottements  */
      /* Construction du nom du fichier XDF_Config_Cligno_<NomMachine>  */
      sprintf ( pl_PathEtFic, "%s/%s_%s", XZEXC_PATH_CONFIG, XDF_Config_Cligno, pa_NomMachine );
      
      /* Ouverture du fichier XDF_Config_Cligno_<NomMachine>  */
      if ( (vl_Fd = open (pl_PathEtFic, O_RDONLY)) <= 0 )
      {
         /* Ecriture trace absence fichier */
         XZST_03EcritureTrace( XZSTC_WARNING, " Manque fichier %s. \n", pl_PathEtFic ); 
         /* Sortie de la fonction */
         return (XDC_NOK);
      }

      strcpy ( va_clignolent, "" );
      while ( ex_LireLigneFich ( vl_Fd, pl_LigneFich, sizeof(pl_LigneFich) ) != XZEXC_EOF )
      {
         if ( *pl_LigneFich != XDF_COMMENTAIRE )
         {
            /*A Affectation du clignottement */
            if ( strlen(va_clignolent) <= 0 )
            {
               if ( sscanf ( pl_LigneFich, "%s", va_clignolent ) != 1 )
               {
                 XZST_03EcritureTrace( XZSTC_WARNING,"ex_lire_cligno : Clignottement incorrect");
                 close (vl_Fd);
                 return (XDC_NOK);
               }
            }
            else
            {
               if ( sscanf ( pl_LigneFich, "%s", va_clignorapide ) != 1 )
               {
                 XZST_03EcritureTrace( XZSTC_DEBUG3,"ex_lire_cligno : Clignottement rapide incorrect");
                 strcpy ( va_clignorapide, va_clignolent );
               }
            }
         }
      }
      
      XZST_03EcritureTrace ( XZSTC_DEBUG3, "Config Cligno: %s %s", va_clignolent, va_clignorapide );
      close (vl_Fd);
      XZST_03EcritureTrace ( XZSTC_FONCTION, "OUT : %s", version);
      return (XDC_OK);
}
