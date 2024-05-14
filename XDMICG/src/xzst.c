/*E*/
/* Fichier : $Id: xzst.c,v 1.38 2021/07/30 14:58:54 pc2dpdy Exp $        Date : $Date: 2021/07/30 14:58:54 $ 
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZSTRACE * FICHIER xzst.c
******************************************************
* DESCRIPTION DU MODULE :
*
*   Description du module d'interface XZSTRACE
*        - Declaration des fonctions externes
*        - Declaration des variables externes
*
*   Reference DCG-021 chapitre 3.7  
*
******************************************************
* HISTORIQUE :
*
*  T.Milleville  30 Aout 1994      : Creation 
*  I.Thomas     17 Nov 1994     : Ajout LIB_REGLE (1.17)
*  T.Milleville  01 Mar 1995      : Modification de la fonction XZST_10
*			Construction d'une chaine regroupant les divers 
*			arguments car le msg T_MT_INFO n'a qu'une chaine 
*			de caracteres comme Grammaire    V1.23
*  T.Milleville  09 Juin 1995      : Ajout traces et destruction de commantaires
*				V1.27
* B.G.		23 Juin 1995	: ajout exit(1) en cas de traces fatales+entete sccs (1.28)
* T.Milleville	25 Jul 1995	: Ajout de commentaires V1.29
* T.Milleville	26 Jul 1995	: Modification argument pour arret tache si FATAL. 
*	Remplace ARRET par ARRET_MARCHE V1.30
* P.Niepceron	17 Nov 1995	: Optimisation de XZST_03EcritureTrace v1.31
* JMG	16/06/10	: linux DEM/934
* JPL		08/12/10 : Migration architecture HP ia64 (DEM 961) : suppression warnings mineurs  1.33
* JPL		30/01/13 : Plus d'ecriture en trace apres fermeture du fichier  1.34
* JPL		06/02/18 : Migration Linux 64 bits (DEM 1274) : Fin de parcours de liste variable d'args.  1.35
* JMG		07/03/20 : EMS 1.36
* AMI    28/01/21 : SAE_191: chantier date: encapsulation localtime pour hiniber TZ car nbseconde deja en TZ
* LCL	 30/07/21 : SAE_303: correction Memory Leak 1.38
* RGR	 24/08/21 : SAE_303 : Correction gmtime
******************************************************/

/* fichiers inclus */
#include <rtworks/common.h>
#include <rtworks/ipc.h>
/****************************** OM*/
#ifdef _HPUX_SOURCE
#include <varargs.h>
#else
#include <stdarg.h>
#endif


#include <xzsa.h>
#include <xzsm.h>
#include <xzsx.h>  
#include <xzsp.h>

#include <xzst.h>

/* declaration des constantes locales */
static char *version="$Id: xzst.c,v 1.38 2021/07/30 14:58:54 pc2dpdy Exp $	:  XZST ";

#define LIB_DEBUG1	"DEBUG1"
#define LIB_DEBUG2	"DEBUG2"
#define LIB_DEBUG3	"DEBUG3"
#define LIB_FONCTION	"FONCTION"
#define LIB_INFO	"INFO"
#define LIB_MESSAGE	"MESSAGE"
#define LIB_SGBD	"SGBD"
#define LIB_WARNING     "WARNING"
#define LIB_FATAL	"FATAL"
#define LIB_INTERFACE	"INTERFACE"
#define LIB_REGLE       "REGLE"

#define myVaStart(liste, paramN) va_start(liste, paramN)
#define myVaArg(liste, mode)     va_arg(liste, mode)
#define myVaEnd(liste)           va_end(liste)


/* declaration des variables locales  */

	/* Niveau de trace initialise a XZSTC_RIEN */
XZSTT_NiveauTrace	XZSTV_NiveauTrace = XZSTC_RIEN;

	/* Identifiant du fichier trace initialise a NULL */
static FILE 			* XZSTV_IdFicTrace = NULL;
	/*stderr; 	*/

	/* Indicateur courant de redirection des traces 
	*				initialise a XZSTC_ECRAN */
int			XZSTV_VisuTrace  = XZSTC_NECRAN;

	/* Taille maximale du fichier trace */
static int			XZSTV_TailleFic; 		

	/* Nombre d'octets ecrits initialise a 0 */
static int			XZSTV_NbOctFic = 0; 		

	/* Descripteur du device de la console */
static FILE			*XZSTV_IdConsole = NULL;	

	/* Nom de la Tache **/
static XZSCT_NomTache		vg_tacheCourante 	= "";

	/* variable d'environnement tz */
char *tz;


/* declaration de fonctions internes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void ActualiserNivTrace (T_IPC_CONN			va_Cnx,
			 T_IPC_CONN_PROCESS_CB_DATA	pa_Data,
			 T_CB_ARG			pa_Arg)

/*
* ARGUMENTS EN ENTREE :
*   < Valeur et descriptif des parametres en entree >
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR : 
*   <Valeur de retour et un commentaire explicatif >
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   < Bref descriptif fonctionnel >
*
------------------------------------------------------*/
{
  /* variables locales */
#ifndef _TIBCO_EMS
  T_STR		vl_nomTache;
  T_INT4	vl_nivTrace;
  T_INT4	vl_visuTrace;
#else
  XDY_NomTache		vl_nomTache;
  XDY_Entier		vl_nivTrace;
  XDY_Entier		vl_visuTrace;
#endif

	static char *version="$Id: xzst.c,v 1.38 2021/07/30 14:58:54 pc2dpdy Exp $	:  ActualiserNivTrace ";

	XZST_03EcritureTrace(XZSTC_FONCTION,
		"ActualiserNivTrace : IN \n");

        if (!TipcMsgRead(pa_Data->msg
                ,T_IPC_FT_STR,&vl_nomTache
                ,T_IPC_FT_INT4,&vl_nivTrace
                ,T_IPC_FT_INT4,&vl_visuTrace
                ,NULL))
        {
      /* erreur */
      return;
    }
  /* appel de la primitive XZST_08ForceNivVisuTraceLocal */
  XZST_08ForceNivVisuTraceLocal (vl_nivTrace, vl_visuTrace);

	XZST_03EcritureTrace(XZSTC_FONCTION,
		"ActualiserNivTrace : OUT  \n");

  return;

}	/* fin ActualiserNivTrace */




/* definition de fonctions externes */


/*X*/
/* Fichier : @(#)xzst.c	1.18	1.18	Release :1.18        Date : 11/22/94 
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSTRACE 
******************************************************
* ALGORITHME :
*
* Arguments
*
* Fonction 
* Cette fonction recupere avec la primitive XZSC_13
* la taille maximale des fichiers traces, ainsi que
* le chemin d'acces des fichiers traces et la taille 
* du fichier trace. 
* Si la taille est superieure a la taille maximale,
* la fonction retourne XZSTC_ARG_INV.
* Puis elle teste l'existence du fichier NomTache.tra 
* dans le repertoire d'acces recupere. Si le fichier
* existe, il est renomme en fichier NomTache.old.
* La fonction ouvre le fichier trace en lecture et
* ecriture. L'identifiant du fichier ouvert est
* memorise dans la variable XZSTV_IdFicTrace et la 
* taille du fichier est memorisee dans XZSTV_TailleFic.
*
* Codes retour
* XZSTC_ARG_INV         Argument invalide.
* XZSTC_IDF_INV         Probleme ouverture fichier.
* XZSTC_INS_INV         Erreur initialisation noyau 
* 			supervision.
* XDC_OK              ok.
*
* Mecanismes
* Fonction systeme UNIX fopen.
******************************************************/

extern int XZST_01OuvertureTrace()
{
  char			tl_nomFicTrace [XDC_NOM_FIC_SIZE] = "",
  			tl_commandeBackup [300] = "";
  char			tl_pathTrace [XDC_PATH_ABS_SIZE] = "";
  int			vl_tailleFic = 0;
  int 			vl_maxTailleFic = 0;
  int			vl_ValRet = XDC_OK;
  FILE                 *vl_commande	= NULL;
  char 			* pl_PathEnv	= NULL;

  /* gestion des versions */
  static char* version = "$Id: xzst.c,v 1.38 2021/07/30 14:58:54 pc2dpdy Exp $: XZST01_OuvertureTrace";


	XZST_03EcritureTrace(XZSTC_INTERFACE,
		"XZST01_OuvertureTrace : IN \n");

    /* Recuperation des infos de taille et de paths sur le fichier de trace */
  if ( XZSC_13InfoFicTrace ( tl_pathTrace, &vl_tailleFic, &vl_maxTailleFic ) 
				== XZSCC_INI_INV )
    {
      /* prevenir l'user et SUPERV */
      vl_ValRet = XZSCC_INI_INV;
    }
  else if ( vl_tailleFic > vl_maxTailleFic ) 
    {
      vl_ValRet = XDC_ARG_INV;
    }
  else 
    {
      /* recuperation du nom de la tache courante */
      if ( XZSC_06NomTache (vg_tacheCourante) != XDC_OK )
	{
	  /* prevenir l'user et SUPERV */
	  vl_ValRet = XDC_NOK;
	}
      /* vl_Creation du nom complet suppose du fichier de trace */
      /* ou redirection sur la sortie erreur */
      if (strcmp (tl_pathTrace, "?") == 0)
	{
		/*B Si repertoire non defini, on utilise la
		* variable d'environnement XZSTC_REP_TRACE **/
		if ( (pl_PathEnv = getenv(XZSTC_REP_TRACE)) == NULL)
		{
			/*B Fichier de traces est le Standart Erreur */
	  		XZSTV_IdFicTrace = stderr;
		}
		else
		{
			/*B Enregistrement du Path */
			strcpy(tl_pathTrace,pl_PathEnv);
		}
	}

	if (strcmp (tl_pathTrace, "?"))
	{
	  sprintf (tl_nomFicTrace, "%s/%s.tra.old", tl_pathTrace, vg_tacheCourante);
	  
	  /* Verification de l'existence de ce fichier avec son path complet */
	  if (access (tl_nomFicTrace, F_OK) == 0)
	    {
	      /* S'il existe et est accessible en ecriture: */
	      /* Renommage en NomTache.old */
	      sprintf ( tl_commandeBackup, 
			"mv %s %s.old", tl_nomFicTrace, tl_nomFicTrace );
	      vl_commande = popen ( tl_commandeBackup, "r" );
	      if (vl_commande ==  NULL)
		{
		  perror ("popen broken by");
	      	  fprintf (stderr, "mv '%s' '%s.old' impossible\n", 
					tl_nomFicTrace, tl_nomFicTrace);
		  return (vl_ValRet = XDC_NOK);
		}
              else
		{
		  pclose(vl_commande);
                }
	    }

	  sprintf (tl_nomFicTrace, "%s/%s.tra", tl_pathTrace, vg_tacheCourante);
	  
	  /* Verification de l'existence de ce fichier avec son path complet */
	  if (access (tl_nomFicTrace, F_OK) == 0)
	    {
	      /* S'il existe et est accessible en ecriture: */
	      /* Renommage en NomTache.old */
	      sprintf ( tl_commandeBackup, 
			"mv %s %s.old", tl_nomFicTrace, tl_nomFicTrace );
	      vl_commande = popen ( tl_commandeBackup, "r" );
	      if (vl_commande ==  NULL)
		{
		  perror ("popen broken by");
	      	  fprintf (stderr, "mv '%s' '%s.old' impossible\n", 
					tl_nomFicTrace, tl_nomFicTrace);
		  return (vl_ValRet = XDC_NOK);
		}
              else
		{
		  pclose(vl_commande);
                }
	    }
	  /* Ouverture du nouveau fichier de trace */
	  if ((XZSTV_IdFicTrace = fopen ( tl_nomFicTrace, "w" )) 
							== (FILE*) NULL)
	    {
	      perror ("fopen broken by");
	      fprintf (stderr, "Ouverture du fichier de trace '%s' \
					impossible pour '%s'\n", 
				tl_nomFicTrace, vg_tacheCourante);
	      vl_ValRet = XDC_NOK;
	    }
	  else
	    {
	      chmod( tl_nomFicTrace, 0666 );
	      XZSTV_TailleFic = vl_tailleFic;
	    }
	}
    }

	XZST_03EcritureTrace(XZSTC_WARNING,
		"XZST01_OuvertureTrace : OUT   Code Retour %d \n", vl_ValRet);

  return( vl_ValRet );

}	/* Fin XZST01_OuvertureTrace */


/*X*/
/* Fichier : @(#)xzst.c	1.18	1.18	Release :1.18        Date : 11/22/94 
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSTRACE 
******************************************************
* ALGORITHME :
*
* Arguments
*
* Fonction 
* Cette fonction ferme le fichier trace precedemment 
* ouvert. Si la variable XZSTV_IdFicTrace est NULL, 
* le fichier n'a pas ete ouvert.
*
* Codes retour
* XZSTC_INI_INV            Ouverture non effectuee.
* XDC_OK                 ok.
*
* Mecanismes
* Fonction systeme UNIX fclose.
******************************************************/

extern int XZST_02FermetureTrace ()
{ 
  static char* version = "$Id: xzst.c,v 1.38 2021/07/30 14:58:54 pc2dpdy Exp $	: XZST_02FermetureTrace";
  /* Variables Locales */
  int			vl_ValRet = XDC_OK;
  

	XZST_03EcritureTrace(XZSTC_INTERFACE,
		"XZST_02FermetureTrace : IN \n");

  /* Tentative de fermerure du fichier de Trace */
  if ( fclose ( XZSTV_IdFicTrace ) != XDC_OK )
    {
      vl_ValRet = XZSTC_FERM_INV;
    }

  return ( vl_ValRet );
}


/*X*/
/* Fichier : @(#)xzst.c	1.18	1.18	Release :1.18        Date : 11/22/94 
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSTRACE 
******************************************************
* ALGORITHME :
*
* Arguments
* NivTrace	: Niveau de trace affecte a l'ecriture 
*		  de cette trace.
* Texte		: Libelle et format du texte a ecrire.
* P0 a  P9	: Arguments optionnels a imprimer dans
*		  la trace. Le nombre d'arguments doit
*		  correspondre au format.
*
* Fonction
* Cette fonction commence par tester si NivTrace est 
* active par rapport au niveau de trace global 
* XZSTV_NiveauTrace.
* Si c'est le cas les traitements suivants sont 
* effectues :
*	- Test de l'ouverture du fichier trace : la 
*	  variable XZSTV_IdFicTrace doit etre 
*	  differente de NULL.
*	- L'heure est rajoutee en debut trace et un 
*	  saut de ligne "\n" en fin de trace. Appel
*	  primitive XZSM_07 et XZSM_11.
*	- Le nombre d'octets ecrit est memorise dans
*	  la variable XZSTV_NbOctFic. Si le nombre 
*	  d'octets a ecrire depasse la taille du 
*	  fichier trace XZSTV_TailleFic, le pointeur
*	  d'ecriture est remis au debut du fichier et
*	  la variable XZSTV_TailleFic est reinitialisee.
*	- Ecriture dans le fichier trace du libelle avec
*	  les arguments.
*	- Lorsque l'ecriture s'est effectuee correctement 
*	  la variable XZSTV_NbOctFic est remise a jour.
*	- Si l'option redirection evl_Cran est precisee 
*	  (XZSTV_VisuTrace = XZSTC_ECRAN) le libelle est 
*         redirige sur /dev/console.
*	- Si le niveau de trace est XZSTC_FATAL l'arret de 
*	  la tache est demande. Appel primitive XZSP_03. 
*	  Les arguments de cette primitive sont determines
*	  par les primitives XZSC_05 , XZSC_06 et XZSC_07.
*
* Codes retour
* XZSTC_INI_INV		Ouverture non effectuee.
* XZSTC_ECR_INV		Probleme ecriture fichier.
* XDC_OK		ok.
*
* Mecanismes
* Fonctions systeme UNIX fprintf, fseek, et strlen.
******************************************************/

extern int XZST_03EcritureTrace ( XZSTT_NiveauTrace NivTrace, char* Texte, ... )
{
  /* Variables Locales */
  va_list		vl_listeArgument;
  int			vl_ValRet 		= XDC_OK;

  /* gestion des versions */
  static char* version = "$Id: xzst.c,v 1.38 2021/07/30 14:58:54 pc2dpdy Exp $	: XZST_03EcritureTrace";


  
  /* Verification de l'activation de NivTrace (local) dans XZSTV_NiveauTrace (global) */
  if ( (NivTrace & XZSTV_NiveauTrace) > 0 )
  {
      /* Variables Locales */
      int			vl_jourSemaine		= 0;
      int  			vl_Cr			= XDC_OK;
      time_t                vl_Heure;
	struct tm   * now;

      double 		vl_horodateSec 		= 0.0;
      char			vl_texteHorod [500]	= "\0";
      char			vl_texteHorod_Out [500]	= "\0";
      char        vl_LibNiveau[30]	= "\0";

      XZSMT_Horodate	vl_heureTrace		= "\0";
      XZSCT_NomMachine	vl_nomMachine		= "\0";
      XZSCT_NomTache	vl_nomTache 		= "\0";

      /* Initialisation du Pointeur d'argument sur le premier argument non nomme */
      memset (&vl_listeArgument,0,sizeof(va_list));
      va_start (vl_listeArgument, Texte);
      /* le flag est active */
      /* test de l'ouverture du fichier de trace */
      if ( XZSTV_IdFicTrace == (FILE*) NULL )
      {
        vl_ValRet = XZSTC_INI_INV;
      }
      /* recuperation de l'heure courante */
      else if ((vl_Cr = XZSM_07LireHorodate ( XZSMC_TYPE_SYSTEME, &vl_jourSemaine, &vl_horodateSec )) != XDC_OK )
	   {
	     vl_ValRet = vl_Cr;
	   }
      else
      {
         /* formattage de l heure specifique aux traces (avec mivl_Cro-secondes ) */
         vl_Heure = (time_t)vl_horodateSec ;
	 /* appel de localtime pour recuperer l'indicateur d'�t� dans tm_isdst */
	 now = localtime(&vl_Heure);
	 vl_Heure = vl_Heure -3600-3600*now->tm_isdst;
/* Modifs RGR */
strftime((char *)vl_heureTrace,30 ,"%d/%m/%y %H:%M:%S" , localtime(&vl_Heure)  );

//   le format ne corresond pas exactement a l'attendu ( les année sont sur 4 (2021)
//         XZSM_11ConversionHorodate ( vl_horodateSec, &vl_heureTrace );


         /* recuperation du libelle du niveau de trace */
         switch (NivTrace) 
         {
           case XZSTC_DEBUG1   : strcpy(vl_LibNiveau,LIB_DEBUG1);
		           	  break;
           case XZSTC_DEBUG2   : strcpy(vl_LibNiveau,LIB_DEBUG2);
			           break;
           case XZSTC_DEBUG3   : strcpy(vl_LibNiveau,LIB_DEBUG3);
			           break;
           case XZSTC_FONCTION : strcpy(vl_LibNiveau,LIB_FONCTION);
			           break;
           case XZSTC_MESSAGE  : strcpy(vl_LibNiveau,LIB_MESSAGE);
			           break;
           case XZSTC_SGBD     : strcpy(vl_LibNiveau,LIB_SGBD);
			           break;
           case XZSTC_INFO     : strcpy(vl_LibNiveau,LIB_INFO);
			           break;
           case XZSTC_WARNING  : strcpy(vl_LibNiveau,LIB_WARNING);
			           break;
           case XZSTC_FATAL    : strcpy(vl_LibNiveau,LIB_FATAL);
			           break;
           case XZSTC_INTERFACE    : strcpy(vl_LibNiveau,LIB_INTERFACE);
			           break;
           case XZSTC_REGLE    : strcpy(vl_LibNiveau,LIB_REGLE);
		              break;
           default : break;
         }

         /* Ecriture de l'horodate et  du niveau de trace 
         *	dans la ligne de trace pour le fichier de Traces*/
         sprintf ( vl_texteHorod, "%s -%s- %s\n", vl_heureTrace, vl_LibNiveau, Texte );
         
         /* Ecriture de l'horodate , du niveau de trace et  du 
         *	nom de la tache dans la ligne de trace 
         *	pour l'Ecran ou la Console */
         sprintf ( vl_texteHorod_Out, "%s : %s :  -%s- %s\n", 
	         vl_heureTrace, vg_tacheCourante, vl_LibNiveau, Texte );

         /* Memorisation du nombre d'octets a ecrire en trace */
         if ((XZSTV_IdFicTrace != stderr ) &&  ((XZSTV_NbOctFic = ftell (XZSTV_IdFicTrace)) >  XZSTV_TailleFic))
	      {
            /* Depassement : on rembobine et on reinitialise la tache */
            XZSTV_NbOctFic = 0;
            rewind ( XZSTV_IdFicTrace );
	      }
         /* Ecriture dans le fichier de trace */
         if ( !vfprintf ( XZSTV_IdFicTrace, vl_texteHorod, vl_listeArgument ))
         {
            vl_ValRet = XZSTC_ECR_INV;
         }
         else 
         {
            /* Flush de l'ecriture */
            fflush (XZSTV_IdFicTrace);

            /* Mise a Jour du nombre d'octet du fichier */
            XZSTV_NbOctFic =  ftell (XZSTV_IdFicTrace);

            /* Ecriture sur la console si besoin est */
            if ( ( XZSTV_VisuTrace ==  XZSTC_ECRAN ) && 
                 ( XZSTV_IdConsole = fopen ("/dev/console", "w") ) )
            {
               vfprintf ( XZSTV_IdConsole, vl_texteHorod_Out, vl_listeArgument );
               fclose (XZSTV_IdConsole);
	         }
	               
            /* Si le niveau de trace est XZSTC_FATAL, arret de la tache */
            if ( NivTrace == XZSTC_FATAL )
        		{
		           /* NomMachine */
		           XZSC_07NomMachine ( vl_nomMachine );
		           
		           /* NomTache */
		           XZSC_06NomTache ( vl_nomTache );
		           
		           /* arret de la tache */
		           vl_ValRet = XZSP_03CmdTache (vl_nomMachine, vl_nomTache, XZSPC_ARRET_MARCHE);
		           exit(1);
	         }
         }
      }
   }


   return( vl_ValRet );

}	/* Fin XZST_03EcritureTrace */



/*X*/
/* Fichier : @(#)xzst.c	1.18	1.18	Release :1.18        Date : 11/22/94 
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSTRACE 
******************************************************
* ALGORITHME :
*
* Arguments
* Texte		: Libelle et format du texte a ecrire.
* P0 a  P9	: Arguments optionnels a imprimer dans
*		  la trace. Le nombre d'arguments doit
*		  correspondre au format.
*
* Fonction
* Cette fonction commence par tester si NivTrace est 
* active par rapport au niveau de trace global 
* XZSTV_NiveauTrace.
* Si c'est le cas les traitements suivants sont 
* effectues :
*	- Test de l'ouverture du fichier trace : la 
*	  variable XZSTV_IdFicTrace doit etre 
*	  differente de NULL.
*	- L'heure est rajoutee en debut trace et un 
*	  saut de ligne "\n" en fin de trace. Appel
*	  primitive XZSM_07 et XZSM_11.
*	- Le nombre d'octets ecrit est memorise dans
*	  la variable XZSTV_NbOctFic. Si le nombre 
*	  d'octets a ecrire depasse la taille du 
*	  fichier trace XZSTV_TailleFic, le pointeur
*	  d'ecriture est remis au debut du fichier et
*	  la variable XZSTV_TailleFic est reinitialisee.
*	- Ecriture dans le fichier trace du libelle avec
*	  les arguments.
*	- Lorsque l'ecriture s'est effectuee correctement 
*	  la variable XZSTV_NbOctFic est remise a jour.
*	- Si l'option redirection evl_Cran est precisee 
*	  (XZSTV_VisuTrace = XZSTC_ECRAN) le libelle est 
*         redirige sur /dev/console.
*	- Si le niveau de trace est XZSTC_FATAL l'arret de 
*	  la tache est demande. Appel primitive XZSP_03. 
*	  Les arguments de cette primitive sont determines
*	  par les primitives XZSC_05 , XZSC_06 et XZSC_07.
*
* Codes retour
* XZSTC_INI_INV		Ouverture non effectuee.
* XZSTC_ECR_INV		Probleme ecriture fichier.
* XDC_OK		ok.
*
* Mecanismes
* Fonctions systeme UNIX fprintf, fseek, et strlen.
******************************************************/
extern int XZST_04TraceMess ( char* Texte, ... )
{
  /* Variables Locales */
  va_list		vl_listeArgument;
  int			vl_jourSemaine,
  			vl_ValRet = XDC_OK,
  			vl_Cr;

  double 		vl_horodateSec = 0.0;
  char			vl_texteHorod [200];

  XZSMT_Horodate	vl_heureTrace;
  XZSCT_NomMachine	vl_nomMachine;
  XZSCT_NomTache	vl_nomTache;
  XZSTT_NiveauTrace	NivTrace;

  /* gestion des versions */
  static char* version = "$Id: xzst.c,v 1.38 2021/07/30 14:58:54 pc2dpdy Exp $	: XZST_04TraceMess";

	XZST_03EcritureTrace(XZSTC_INTERFACE,
		"XZST_04TraceMess : IN  \n");

  /* NivTrace force a MESS */
  NivTrace = XZSTC_MESSAGE;

  /* Initialisation du Pointeur d'argument sur le premier argument non nomme */
  va_start (vl_listeArgument, Texte);
  
  /* Verification de l'activation de NivTrace (local) dans XZSTV_NiveauTrace (global) */
  if ( (NivTrace & XZSTV_NiveauTrace) > 0 )
    {
      /* le flag est active */
      /* test de l'ouverture du fichier de trace */
      if ( XZSTV_IdFicTrace == (FILE*) NULL )
	{
	  vl_ValRet = XZSTC_INI_INV;
	}
      /* recuperation de l'heure courante */
      else if ((vl_Cr = XZSM_07LireHorodate ( XZSMC_TYPE_SYSTEME, &vl_jourSemaine, &vl_horodateSec )) != XDC_OK )
	{
	  vl_ValRet = vl_Cr;
	}
      /* Conversion de l'horodate de seconde en texte */
      else if ((vl_Cr = XZSM_11ConversionHorodate ( vl_horodateSec, &vl_heureTrace )) != XDC_OK )
	{
	  vl_ValRet = vl_Cr;
	}
      else
	{
	  /* Ecriture de l'horodate dans la ligne de trace */
	  sprintf ( vl_texteHorod, "%s %s\n", vl_heureTrace, Texte );
	  
	  /* Memorisation du nombre d'octets a ecrire en trace */
	  if ((XZSTV_IdFicTrace != stderr ) && 
	      ((XZSTV_NbOctFic = ftell (XZSTV_IdFicTrace)) >  XZSTV_TailleFic))
	    {
	      /* Depassement : on rembobine et on reinitialise la tache */
	      XZSTV_NbOctFic = 0;
	      rewind ( XZSTV_IdFicTrace );
	    }
	  /* Ecriture dans le fichier de trace */
	  if ( !vfprintf ( XZSTV_IdFicTrace, vl_texteHorod, vl_listeArgument ))
	    {
	      vl_ValRet = XZSTC_ECR_INV;
	    }
	  else 
	    {
	      /* Flush de l'ecriture */
	      fflush (XZSTV_IdFicTrace);

	      /* Mise a Jour du nombre d'octet du fichier */
	      XZSTV_NbOctFic =  ftell (XZSTV_IdFicTrace);
	    }
	}
    }

	XZST_03EcritureTrace(XZSTC_INTERFACE,
		"XZST_04TraceMess : OUT   Code Retour %d \n",vl_ValRet);

  return( vl_ValRet );

}	/* Fin XZST_04TraceMess */


/*X*/
/* Fichier : @(#)xzst.c	1.18	1.18	Release :1.18        Date : 11/22/94 
 ******************************************************
 * STERIA *  PROJET MIGRAZUR 
 ******************************************************
 * MODULE XZSTRACE 
 ******************************************************
 * ALGORITHME :
 *
 * Arguments
 * NomMachine	: Nom de la machine ou se trouve la 
 * 		  tache precisee.
 * NomTache	: Nom de la tache dont les traces sont
 * 		  a commander.
 * NivTrace      : Niveau de trace a activer.
 * VisuTrace	: XZSTC_ECRAN ou XZSTC_NECRAN.
 *
 * Fonction
 * Cette fonction controle l'argument d'entree VisuTrace 
 * et les NomMachine et NomTache avec les primitives 
 * XZSA_05 et XZSA_07.
 * Cette fonction envoi le message XDM_TST_CMDT au 
 * datagroup concatene avec le NomMachine et le NomTache.
 * Ce message contient NomTache NivTrace VisuTrace avec 
 * le format %s %d %d.
 *
 * Codes retour
 * XZSTC_ARG_INV           Argument VisuTrace invalide.
 * XZSTC_NOM_INV           Nom machine invalide.
 * XZSTC_NOT_INV           Nom tache invalide.
 * XDC_OK                ok.
 *
 * Mecanismes
 * RTworks : RTS2
 ******************************************************/

extern int XZST_06CmdNivVisuTrace (XZSCT_NomMachine NomMachine, 
				   XZSCT_NomTache NomTache, 
				   XZSTT_NiveauTrace NivTrace, 
				   int VisuTrace)
{
  /* variables locales */
  int				vl_Cr, vl_ValRet = XDC_OK;
  char				nomDatagroup [100];
  
  /* gestion des versions */
  static char* version = "$Id: xzst.c,v 1.38 2021/07/30 14:58:54 pc2dpdy Exp $	: XZST_06CmdNivVisuTrace";
 
	XZST_03EcritureTrace(XZSTC_INTERFACE,
		"XZST_06CmdNivVisuTrace :   IN   \
	    Nom Mach %s -- Nom Tache %s -- Niv Trace %d -- Visu Trace %d \n",
	NomMachine, NomTache, NivTrace, VisuTrace);

  /* Verification des arguments */
  if  ( (( vl_Cr = XZSA_05ExisterMachine (NomMachine)) != XDC_OK) ||
       (( vl_Cr = XZSA_07ExisterTache (NomTache)) != XDC_OK )) 
    {
      vl_ValRet = vl_Cr;
    }

  
/* Construction et Envoi du message XDM_TST_CMDT */
  else 
    {
      XDG_EncodeDG2 (nomDatagroup, NomMachine , NomTache);
      TipcSrvMsgWrite (nomDatagroup, 
		       XDM_IdentMsg (XDM_TST_CMDT),
		       XDM_FLG_SRVMSGWRITE,
		       T_IPC_FT_STR, NomTache,
		       T_IPC_FT_INT4, NivTrace,
		       T_IPC_FT_INT4, VisuTrace,
		       NULL );
      
      TipcSrvFlush ();
    }
  
	XZST_03EcritureTrace(XZSTC_INTERFACE,
		"XZST_06CmdNivVisuTrace :   OUT   Code Retour %d \n",vl_ValRet);

  return ( vl_ValRet );

}	/* Fin XZST_06CmdNivVisuTrace */



/*X*/
/* Fichier : @(#)xzst.c	1.18	1.18	Release :1.18        Date : 11/22/94 
 ******************************************************
 * STERIA *  PROJET MIGRAZUR 
 ******************************************************
 * MODULE XZSTRACE 
 ******************************************************
 * ALGORITHME :
 *
 * Arguments
 *
 * Fonction
 * La fonction arme un callback sur reception du message 
 * XDM_TST_CMDT. Ce callback est la fonction interne 
 * suivante XZST_ActualiserNivTrace.
 * XZST_ActualiserNivTrace ((IN)(T_MSG_HEADER *)msg_header,
 * 			(OUT)(char *)msg_data)
 * 	Les arguments sont specifiques a un callback IPC. 
 * 	Cette fonction recupere les informations NivTrace 
 * 	VisuTrace et appelle la fonction XZST_08 pour 
 * 	forcer le niveau de trace.
 *
 * Codes retour
 * XDC_OK           ok.
 *
 * Mecanismes
 * RTworks : RTS3
 ******************************************************/

extern int XZST_07AutoriserCmdNivVisuTrace()
{
  /* Variables locales */
  int			vl_ValRet = XDC_OK;
  
  /* gestion des versions */
  static char* version = "$Id: xzst.c,v 1.38 2021/07/30 14:58:54 pc2dpdy Exp $	: XZST_07AutoriserCmdNivVisuTrace";

	XZST_03EcritureTrace(XZSTC_INTERFACE,
		" XZST_07AutoriserCmdNivVisuTrace :  IN \n");
  
  /* Armemement du Callback XZST_ActualiserNivTrace sur reception de XDM_TST_CMDT */
  if (TipcSrvProcessCbCreate (XDM_IdentMsg(XDM_TST_CMDT), 
			      (T_IPC_CONN_PROCESS_CB_FUNC) ActualiserNivTrace, 
			      (T_CB_ARG) NULL) == NULL)
    {
      vl_ValRet = XDC_NOK;
    }
  
	XZST_03EcritureTrace(XZSTC_INTERFACE,
		" XZST_07AutoriserCmdNivVisuTrace :  OUT   Code Retour %d \n",
		vl_ValRet);

  return (vl_ValRet);

}	/* Fin XZST_07AutoriserCmdNivVisuTrace */


/*X*/
/* Fichier : @(#)xzst.c	1.18	1.18	Release :1.18        Date : 11/22/94 
 ******************************************************
 * STERIA *  PROJET MIGRAZUR 
 ******************************************************
 * MODULE XZSTRACE 
 ******************************************************
 * ALGORITHME :
 *
 * Arguments
 * NivTrace  :	Niveau de trace a activer.
 * VisuTrace :	XZSTC_ECRAN ou XZSTC_NECRAN.
 *
 * Fonction
 * Cette fonction met a jour les variables 
 * XZSTV_NiveauTrace et XZSTV_VisuTrace.
 * XZSTV_NiveauTrace=XZSTC_FATAL|XZSTC_WARNING|NivTrace.
 * Elle recupere le NomTache et le NomMachine avec les
 * primitives XZSC_06 et XZSC_07.
 * Appel de la primitive XZSE_26 pour envoyer les 
 * informations.
 *
 * Codes retour
 * XDC_OK           ok.
 *
 * Mecanismes
 * RTworks : RTS2
 ******************************************************/

extern int XZST_08ForceNivVisuTraceLocal( 
		XZSTT_NiveauTrace NivTrace, 
		int VisuTrace )
{
  /* Variables locales */
  int			vl_ValRet = XDC_OK,
    			vl_Cr;
  XZSCT_NomTache	nomTacheCourante;
  XZSCT_NomMachine	nomMachineCourante;
  
  /* gestion des versions */
  static char* version = "$Id: xzst.c,v 1.38 2021/07/30 14:58:54 pc2dpdy Exp $	: XZST_08ForceNivVisuTraceLocal";
  
	XZST_03EcritureTrace(XZSTC_INTERFACE,
		"  XZST_08ForceNivVisuTraceLocal :  IN \
		Niv Trace %d -- Visu Trace %d \n",NivTrace,VisuTrace );


  /* Recuperation du nom de la machine qui call */
  if ((vl_Cr = XZSC_06NomTache (nomTacheCourante)) != XDC_OK)
    {
      vl_ValRet = vl_Cr;
    }
  
  /* Recuperation du nom de la machine courante */
  else if (( vl_Cr = XZSC_07NomMachine (nomMachineCourante)) != XDC_OK)
    {
      vl_ValRet = vl_Cr;
    }
  else
    {
      /* Mise a jour du niveau de trace a activer */
      XZSTV_NiveauTrace = XZSTC_FATAL | XZSTC_WARNING | NivTrace;
      XZSTV_VisuTrace = VisuTrace;
      
      /* Envoi des infos */
      if ((vl_Cr = XZSE_26EnvoieEtatTrace (nomMachineCourante, 
					nomTacheCourante, 
					XZSTV_NiveauTrace, 
					VisuTrace)) != XDC_OK)
	{
	  vl_ValRet = vl_Cr;
	}
    }
  
	XZST_03EcritureTrace(XZSTC_INTERFACE,
		"  XZST_08ForceNivVisuTraceLocal :  OUT   Code Retour %d \n",
		vl_ValRet);

  return (vl_ValRet);

}	/* Fin XZST_08ForceNivVisuTraceLocal */



/*X*/
/* Fichier : @(#)xzst.c	1.18	1.18	Release :1.18        Date : 11/22/94 
 ******************************************************
 * STERIA *  PROJET MIGRAZUR 
 ******************************************************
 * MODULE XZSTRACE 
 ******************************************************
 * ALGORITHME :
 *
 * Arguments
 * NomMachine :	Nom de la machine caracterisant le 
 *		synoptique de gestion des traces.
 *
 * Fonction
 * Cette fonction controle l'argument d'entree 
 * NomMachine  XZSA_05.
 * Puis elle envoie le message XDM_TST_INIT au 
 * datagroup XDG_NSUPETAT avec l'information NomMachine 
 * au format %s.
 *
 * Codes retour
 * XZSTC_NOM_INV		Nom machine invalide
 * XDC_OK		ok.
 *
 * Mecanismes
 * RTworks : RTS2
 * ******************************************************/

extern int XZST_09InitSynoptiqueTrace ( XZSCT_NomMachine NomMachine )
{
  /* Variables locales */
  int			vl_ValRet = XDC_OK;
  int 			vl_Cr;
  
  /* gestion des versions */
  static char* version = "$Id: xzst.c,v 1.38 2021/07/30 14:58:54 pc2dpdy Exp $	: XZST_09InitSynoptiqueTrace	 ";
  
	XZST_03EcritureTrace(XZSTC_INTERFACE ,
		"  XZST_09InitSynoptiqueTrace : In  Nom Mach %s \n",
		NomMachine );


  /* Controle de l'existence de la machine */
  if ( ( vl_Cr = XZSA_05ExisterMachine (NomMachine)) != XDC_OK)
    {
      vl_ValRet = vl_Cr;
    }
  else
    {
      /* envoi du message XDM_TST_INIT au datagroup XDG_NSUPETAT */
      TipcSrvMsgWrite (XDG_NSUPETAT, 
		       XDM_IdentMsg (XDM_TST_INIT),
		       XDM_FLG_SRVMSGWRITE,
		       T_IPC_FT_STR, NomMachine,
		       NULL);
    }
  
	XZST_03EcritureTrace(XZSTC_INTERFACE ,
		"  XZST_09InitSynoptiqueTrace : OUT   Code Retour %d\n",
		vl_ValRet);

  return (vl_ValRet);

}	/* FIn XZST_09InitSynoptiqueTrace */



/*X*/
/* Fichier : @(#)xzst.c	1.18	1.18	Release :1.18        Date : 11/22/94 
 ******************************************************
 * STERIA *  PROJET MIGRAZUR 
 ******************************************************
 * MODULE XZSTRACE 
 ******************************************************
 * ALGORITHME :
 *
 * Arguments
 * NumTexte :	Numero du libelle et format du texte a
 * 		archiver.
 * P0 a P9  :	Arguments optionnels a imprimer dans la 
 *		trace. Le nombre d'arguments doit 
 *		correspondre au format.
 *
 * Fonction
 * Cette fonction recupere le nom de la machine avec la 
 * primitive XZSC_07 et envoie le message rtworks T_MT_INFO au 
 * datagroup XDG_NSUPHIST en construisant une chaine de caracters 
 * avec le nom de la machine, le numero du texte et les arguments. 
 *	Les arguments sont uniquement des chaines de caracteres.
 *
 * Codes retour
 * XZSTC_INI_INV		Initialisation non effectuee
 * XDC_OK		ok.
 *
 * Mecanismes
 * RTworks : RTS2
 * ******************************************************/

//#define	CM_LGR_BLK	128		/* Taille d'une unite d'allocation memoire */
#define	CM_LGR_BLK	128		/* Taille d'une unite d'allocation memoire */


extern int XZST_10ArchiverMsgSyst ( int NumTexte, ... )
{
  /* Variables locales */
  int			vl_ValRet = XDC_OK;
  va_list		pl_listeArgument;
  XZSCT_NomMachine	vl_nomMachineCourante	= "";
  T_IPC_MSG		vl_Msg ;
  char *		pl_Texte		= NULL;
  char *		pl_Info			= NULL;
  size_t		vl_TailleInfo	= 0;

  /* gestion des versions */
  static char* version = "$Id: xzst.c,v 1.38 2021/07/30 14:58:54 pc2dpdy Exp $	: XZST_10ArchiverMsgSyst	 ";

 XZST_03EcritureTrace(XZSTC_INTERFACE,
		"XZST_10ArchiverMsgSyst IN : Num Texte %d \n",NumTexte);


  /*A Recuperation du nom de la machine */
  if (( vl_ValRet = XZSC_07NomMachine (vl_nomMachineCourante )) != XDC_OK)
   {
 	XZST_03EcritureTrace(XZSTC_WARNING, "Pb Recuperation Nom Machine \n");
  } 
  else
    {
	/*B Allouer une zone memoire pour le texte du message */
	vl_TailleInfo = sizeof(char) * CM_LGR_BLK;
	pl_Info = (char *) malloc(vl_TailleInfo);

	if (pl_Info == NULL)
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "ECHEC ALLOCATION MEMOIRE\n");
		vl_ValRet = XDC_NOK;
	}
	else
	{
		sprintf(pl_Info,"%s#%d",vl_nomMachineCourante,NumTexte);
		XZST_03EcritureTrace(XZSTC_DEBUG3," Info Num Texte %s \n",pl_Info);
	}

	/*B Construction du message avec les arguments variables */
	/* Recuperation des arguments */
	myVaStart (pl_listeArgument, NumTexte);

	 while ((vl_ValRet == XDC_OK)  &&  (pl_Texte =  myVaArg(pl_listeArgument, char*)) != NULL)
	 {
 		XZST_03EcritureTrace(XZSTC_DEBUG3, "  argument %s\n", pl_Texte);
		if (strlen(pl_Info) + strlen(pl_Texte) + 2 > vl_TailleInfo)
		{
			pl_Info = realloc (pl_Info, vl_TailleInfo + ((sizeof(char) * CM_LGR_BLK)));
			if (pl_Info == NULL)
			{
				XZST_03EcritureTrace(XZSTC_WARNING, "ECHEC ALLOCATION MEMOIRE\n");
				vl_ValRet = XDC_NOK;
			}
			else
			{
				vl_TailleInfo = vl_TailleInfo + (sizeof(char) * CM_LGR_BLK);
			}
		}
		if (vl_ValRet == XDC_OK)
		{
			strcat(pl_Info,"#");
			strcat(pl_Info,pl_Texte);
		}
	 }
	  
	  /* Liberation du pointeur sur argument*/
	  myVaEnd (pl_listeArgument);

	if (vl_ValRet == XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_DEBUG3,
			" Avant Envoi du msg T_MT_INFO %s -- long %d  \n",
			pl_Info,strlen(pl_Info));

		/*A Envoi du Msg */	
		TipcSrvMsgWrite (XDG_NSUPETAT, 
			       XDM_IdentMsg (T_MT_INFO),
			       XDM_FLG_SRVMSGWRITE,
			       T_IPC_FT_STR, pl_Info,
			       NULL );
		TipcSrvFlush ();
	}

 	XZST_03EcritureTrace(XZSTC_DEBUG3, " Envoi du msg T_MT_INFO\n");
    }
 
    if (pl_Info != NULL)
    {
	free(pl_Info);
    }

 	XZST_03EcritureTrace(XZSTC_INTERFACE,
		"XZST_10ArchiverMsgSyst OUT  : Code Retour %d",vl_ValRet);

  return (vl_ValRet);

}	/* Fin XZST_10..  */


/* fin du fichier */
