/*E*/
/* Fichier : $Id: esur_ctl.c,v 1.12 2019/03/12 15:13:28 pc2dpdy Exp $        Release : $Revision: 1.12 $        Date : $Date: 2019/03/12 15:13:28 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TESUR * FICHIER esur_ctl.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Module de gestion des fonction de controle de la tache TESUR
*  (lancement et arret>
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	23 Aug 1994	: Creation
* Mismer.D	version 1.2	25 Jan 1995	:
* Mismer.D	version 1.3	25 Jan 1995	:
* Mismer.D	version 1.4	26 Jan 1995	:
* Volcic.F	version 1.5	10 Mai 1995	:
* Fontaine.C	version 1.6	22 Mai 1995	: Reprise entete, historique
* Mismer.D	version 1.7	04 Jul 1995	: Ajout fonction el_arret_tache
* Mismer.D	version 1.8	01 Sep 1995	: Modif lancement tetim
* Mismer.D	version 1.9	13 Mar 1996	: Modif lancement tache protocole operator (DEM/1100)
*
* JPL		24/02/11 : Migration architecture HP ia64 (DEM 975) : format des commentaires  1.11
* JMG		05/03/19 : IP 1.12
------------------------------------------------------*/

/* fichiers inclus */

#include "esur_ctl.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "@(#)esur_ctl.c	1.9 03/19/96 : esur_ctl" ;

/* declaration de fonctions internes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Fonction permettant de copier un fichier executable avec le nom de la tache avec le numero de ligne dans le nom
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int el_copier_fic_exe ( char 	*pa_NumLiaison,
			char 	*pa_NomProtocole,
			char	*pa_NomTache )

/*
* ARGUMENTS EN ENTREE :
*  va_TypeProtocole		Type de tache protocole a creer
*  pa_NumLiaison		Numero de la liaison
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
*  XDC_OK		succes
*  XDC_NOK		echec
*
* CONDITION D'UTILISATION
*  
*
* FONCTION 
*   Creer un fichier executable avec le nom de la tache avec le numero de ligne dans le nom.
*
------------------------------------------------------*/
{
static char *version = "@(#)esur_ctl.c	1.9 03/19/96 : el_copier_fic_exe " ;
XDY_NomTache	vl_NomTacheSrc, vl_NomTacheDest;
char 		pl_NomRepertoire[XDC_PATH_ABS_SIZE];
XZEXT_MASK *   	pl_masque = NULL;
XDY_NomMachine	pl_NomMachine;


        /*A Recuperation du nom de la machine */
	if ( XZSC_07NomMachine(pl_NomMachine) != XDC_OK)
	{
	   /* Ecriture Trace */
	   XZST_03EcritureTrace( XZSTC_WARNING, "Recuperation du Nom de la machine impossible ." );
	}
	
   	sprintf ( vl_NomTacheDest, "%s%s%d.xs", XZECC_TACHE_EQUEXT, pa_NomProtocole, atoi(pa_NumLiaison) );
   	sprintf ( vl_NomTacheSrc, "%s%si.x", XZECC_TACHE_EQUEXT, pa_NomProtocole );
   	if ( XZSS_20PidTache ( vl_NomTacheDest ) != 0 )
   	{
            XZST_03EcritureTrace( XZSTC_INFO, "Tache protocole %s deja en cours", vl_NomTacheDest );
	    return ( XDC_NOK );
   	   
   	}
   	
        if ( XZSS_07TransfertFichier (    pl_NomMachine,
               				  vl_NomTacheSrc,
               				  XZEXC_PATH_TACHE_EQUEXT,
               				  pl_NomMachine,
               				  vl_NomTacheDest,
               				  XZEXC_PATH_TACHE_EQUEXT ) != XDC_OK )
        {
            XZST_03EcritureTrace( XZSTC_WARNING, "Copie fichier %s vers %s impossible",
               					   vl_NomTacheSrc,
               					   vl_NomTacheDest );
	    return ( XDC_NOK );
        } 
   	
   	sprintf ( pa_NomTache, "%s%s%d", XZECC_TACHE_EQUEXT, pa_NomProtocole, atoi(pa_NumLiaison) );   	
	return ( XDC_OK );
   	
}

/* definition de fonctions externes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Fonction permettant de creer des tache temporaire du sous-systeme EQUEXT
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int el_lancer_tache_protocole ( 	char 	va_TypeProtocole,
					char	va_Sens,
				       	char 	*pa_NumLiaison )

/*
* ARGUMENTS EN ENTREE :
*  va_TypeProtocole		Type de protocole a lancer
*  va_Sens			Sens maitre ou esclave
*  pa_NumLiaison		Numero de la liaison
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
*  XDC_OK		succes
*  XDC_NOK		echec
*
* CONDITION D'UTILISATION
*  
*
* FONCTION 
*   Selon le type de protocole ( XZECC_PROT_LCR_MAITRE, XZECC_PROT_LCR_ESCLAVE, 
*   XZECC_PROT_APRILNET, XZECC_PROT_OPERATOR ) lancement de la tache temporaire
*   corespondante.
*
------------------------------------------------------*/
{
static char *version = "@(#)esur_ctl.c	1.9 03/19/96 : el_lancer_tache_protocole " ;
register int	vl_ValRet = XDC_OK;
XDY_NomTache	pl_NomTache;

   /*A Selon le type de protocole */
   switch ( va_TypeProtocole )
   {
   case XZECC_PROT_LCR	:
      /*A Si sens maitre */
      if ( va_Sens == XZECC_SENS_MAITRE )
      {
        /*A Construction du nom de la tache TELM<NoLigne> sur le modele TELMi */
   	if ( (vl_ValRet = el_copier_fic_exe(pa_NumLiaison, XZECC_TACHE_LCR_MAITRE, pl_NomTache)) == XDC_OK )
   	   /*A Appel de la fonction el_lancer_tache */
   	   vl_ValRet = el_lancer_tache ( pl_NomTache, pa_NumLiaison );
      }
      /*A sinon sens esclave */
      else
      {
        /*A Construction du nom de la tache TELE<NoLigne> sur le modele TELEi */
   	if ( (vl_ValRet = el_copier_fic_exe(pa_NumLiaison, XZECC_TACHE_LCR_ESCLAVE, pl_NomTache)) == XDC_OK )
      	   /*A Appel de la fonction el_lancer_tache */
   	   vl_ValRet = el_lancer_tache ( pl_NomTache, pa_NumLiaison );
      }
      break;
   
   case XZECC_PROT_IP   :
   case XZECC_PROT_DIASER       :
      /*le num liaison contient le type d eqt*/
      /*A Si sens maitre */
      if ( va_Sens == XZECC_SENS_MAITRE )
      {
         /*A Construction du nom de la tache TEIM<NoLigne> sur le modele TELMi */
         if ( (vl_ValRet = el_copier_fic_exe (pa_NumLiaison, XZECC_TACHE_IP_MAITRE, pl_NomTache)) == XDC_OK )
            /*A Appel de la fonction el_lancer_tache */
            vl_ValRet = el_lancer_tache ( pl_NomTache, pa_NumLiaison );
      }
      /*A sinon sens esclave */
      else
      {
         /*A Construction du nom de la tache TEIE<NoLigne> sur le modele TELEi */
         if ( (vl_ValRet = el_copier_fic_exe(pa_NumLiaison, XZECC_TACHE_IP_ESCLAVE, pl_NomTache)) == XDC_OK )
            /*A Appel de la fonction el_lancer_tache */
            vl_ValRet = el_lancer_tache ( pl_NomTache, pa_NumLiaison );
        }
        break;

   case XZECC_PROT_APRIL	:
        /*! Construction du nom de la tache XZECC_TACHE_APRILNET */
/*
        sprintf ( pl_NomTache, "%s%s.xs", XZECC_TACHE_EQUEXT, XZECC_TACHE_APRILNET );
        if ( XZSS_20PidTache ( pl_NomTache ) != 0 )
        {
            XZST_03EcritureTrace( XZSTC_INFO, "Tache protocole %s deja en cours", pl_NomTache );
            return ( XDC_NOK );
        }
*/
        /*A Appel de la fonction el_lancer_tache */
/*
        sprintf ( pl_NomTache, "%s%s", XZECC_TACHE_EQUEXT, XZECC_TACHE_APRILNET );
        vl_ValRet = el_lancer_tache ( pl_NomTache, pa_NumLiaison );
*/
        break;

   case XZECC_PROT_TEL	:
        /*A Construction du nom de la tache  */
   	if ( (vl_ValRet = el_copier_fic_exe(pa_NumLiaison, XZECC_TACHE_AUTOTEL, pl_NomTache)) == XDC_OK )
           /*A Appel de la fonction el_lancer_tache */
   	   vl_ValRet = el_lancer_tache ( pl_NomTache, pa_NumLiaison );
   	break;
   	
   case XZECC_PROT_OPE	:
        /*A Construction du nom de la tache TEOX<NoLigne> sur le modele TEXOi */
   	if ( (vl_ValRet = el_copier_fic_exe(pa_NumLiaison, XZECC_TACHE_OPERATOR, pl_NomTache)) == XDC_OK )
   	   /*A Appel de la fonction el_lancer_tache */
   	   vl_ValRet = el_lancer_tache ( pl_NomTache, pa_NumLiaison  );
   	break;   
   default	:
   	vl_ValRet =  XDC_ARG_INV;
   	break;
   }	/*A fin switch */
   
   return ( vl_ValRet );
   
}  /*A fin fonction el_lancer_tache_temporaire */



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Fonction permettant de creer une tache equipement suivant
*  le type de l'equipement specifie
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int el_lancer_tache_eqt ( char 	*pa_TypeEqt )

/*
* ARGUMENTS EN ENTREE :
*  pa_TypeEqt		Type d'equipement a lancer
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
*  XDC_OK		succes
*  XDC_NOK		echec
*
* CONDITION D'UTILISATION
*  
*
* FONCTION 
*   Selon le type d'equipement ( XZECC_TYPE_EQT_xxx ) creation de la tache TEzzz
*   relative a l'equipement identifie par zzz.
*
------------------------------------------------------*/
{
static char *version = "@(#)esur_ctl.c	1.9 03/19/96 : el_lancer_tache_eqt " ;
register int	vl_ValRet   	= XDC_OK;
XDY_NomTache	pl_NomTache 	= "";
XDY_NomTache	vl_NomTacheSrc	= "", 
		vl_NomTacheDest	= "";
XDY_NomMachine	pl_NomMachine	= "";
char            vl_Commande [XZSSC_LINE_CMD_SIZE] = "";
FILE*           vl_Cmd  	= NULL;
int             vl_Pid  	= 0;


    /*A Recuperation du nom de la machine */
    if ( XZSC_07NomMachine(pl_NomMachine) != XDC_OK)
    {
	/* Ecriture Trace */
	XZST_03EcritureTrace( XZSTC_WARNING, "Recuperation du Nom de la machine impossible ." );
    }
	   
   /*A Construction du nom de la tache TEzzz */
   sprintf ( pl_NomTache, "%s%s", XZECC_TACHE_EQUEXT, pa_TypeEqt );
   sprintf ( vl_NomTacheDest, "%s.xs", pl_NomTache );
   sprintf ( vl_NomTacheSrc, "%s.x", pl_NomTache );
   
   if ( !strcmp(pl_NomTache, XZEXC_TACHE_TETIM) )
   {
      while ( (vl_Pid = XZSS_20PidTache(vl_NomTacheDest)) != 0 )
      {
         /* Arret de la tache  */
         XZST_03EcritureTrace( XZSTC_WARNING, "Kill de <%s>", vl_NomTacheDest );
         sprintf ( vl_Commande, "kill %d", vl_Pid );
         if ( (vl_Cmd = popen (vl_Commande, "r"))!= NULL ) pclose ( vl_Cmd );
         sleep (1);
       }
   }
   else
   {
      if ( XZSS_20PidTache ( vl_NomTacheDest ) != 0 )
      {
         XZST_03EcritureTrace( XZSTC_INFO, "Tache traitement %s deja en cours", vl_NomTacheDest );
         return ( XDC_NOK );
      }
   }

   if ( XZSS_07TransfertFichier (    	pl_NomMachine,
               				vl_NomTacheSrc,
               				XZEXC_PATH_TACHE_EQUEXT,
               				pl_NomMachine,
               				vl_NomTacheDest,
               				XZEXC_PATH_TACHE_EQUEXT ) != XDC_OK )
   {
       XZST_03EcritureTrace( XZSTC_WARNING, "Copie fichier %s vers %s impossible",
               					   vl_NomTacheSrc,
               					   vl_NomTacheDest );
       return ( XDC_NOK );
   } 
      
   /*A Appel de la fonction el_lancer_tache */
   return ( el_lancer_tache ( pl_NomTache, NULL ) );
   
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Fonction permettant de creer une tache quelconque dont le nom est specifie
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int el_lancer_tache ( char 	*pa_NomTache, char *	pa_NumLiaison )

/*
* ARGUMENTS EN ENTREE :
*  pa_NomTache		Nom de la tache a creer
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
*  			succes PID de la tache
*  XDC_NOK		echec
*
* CONDITION D'UTILISATION
*  
*
* FONCTION 
*   Creation de la tache TEzzz specifie en entree
*
------------------------------------------------------*/

{
  static char *version = "@(#)esur_ctl.c	1.9 03/19/96 : el_lancer_tache " ;
  int			vl_ValRet = XDC_OK;
  XDY_NomTache		vl_NomTache;
  int			vl_cr;
  static char		*vl_argvt[3]; 	/* tableau de pointeurs de strings */

	
	/*A Atteindre le repertoire contenant les taches EQUEXT */
        chdir ( XZEXC_PATH_TACHE_EQUEXT );
        /*A Construction du nom complet de la tache */
        sprintf ( vl_NomTache, "%s.xs", pa_NomTache );
        if ( access(vl_NomTache, F_OK) )
        {
            XZST_03EcritureTrace( XZSTC_WARNING, "Manque executable : <%s>", vl_NomTache );
            return ( XDC_NOK );
        }
	/*A Creation de la commande de lancement de la tache */
	vl_argvt[0] = vl_NomTache;		/* nom de la commande 		$0 */
	vl_argvt[1] = vg_NomMachine;		/* nom de la machine		$1 */
	vl_argvt[2] = pa_NumLiaison;		/* argument 2  de la commande	$2 */

	/*A lance la tache par le principe du fork unix */
        /*vl_cr = vfork();*/
	 /*TIBCO SS66*/
        vl_cr = fork();
	if ( vl_cr == 0 )
	{  /* process fils */
	   execvp( vl_argvt[0], vl_argvt );
           exit(0);
	}

	sleep (1);
	/*A Lecture du PID de la tache */
   	if ( XZSS_20PidTache ( vl_NomTache ) == 0 )
   	{
            XZST_03EcritureTrace( XZSTC_WARNING, "Echec du fork pour '%s'", vl_NomTache );
            ex_rep_socket ( vg_NomMachine );
	    return ( XDC_NOK );
   	}
	

       ex_rep_socket ( vg_NomMachine );
       return( XDC_OK );
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Fonction permettant d'arret une tache quelconque dont le nom est specifie
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void el_arret_tache ( 	char 	va_TypeProtocole,
			char	va_Sens,
			char 	*pa_NumLiaison )

/*
* ARGUMENTS EN ENTREE :
*  pa_NomTache		Nom de la tache a creer
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
*  			succes PID de la tache
*  XDC_NOK		echec
*
* CONDITION D'UTILISATION
*  
*
* FONCTION 
*   Arret de la tache TEzzz specifie en entree
*
------------------------------------------------------*/

{
  static char *version = "@(#)esur_ctl.c	1.9 03/19/96 : el_arret_tache " ;
  XDY_NomTache		vl_NomTache;
  char                  vl_Commande [XZSSC_LINE_CMD_SIZE] = "";
  FILE*                 vl_Cmd = NULL;
  int			vl_Pid = 0;

   /*A Selon le type de protocole */
   switch ( va_TypeProtocole )
   {
   case XZECC_PROT_LCR	:
      /*A Si sens maitre */
      if ( va_Sens == XZECC_SENS_MAITRE )
   	sprintf ( vl_NomTache, "%s%s%d.xs", XZECC_TACHE_EQUEXT, XZECC_TACHE_LCR_MAITRE, atoi(pa_NumLiaison) );   	
      /*A sinon sens esclave */
      else
   	sprintf ( vl_NomTache, "%s%s%d.xs", XZECC_TACHE_EQUEXT, XZECC_TACHE_LCR_ESCLAVE, atoi(pa_NumLiaison) );   	
      break;
   
   case XZECC_PROT_IP   :
   case XZECC_PROT_DIASER       :
      /*le num liaison contient le type d eqt*/
      /*A Si sens maitre */
      if ( va_Sens == XZECC_SENS_MAITRE )
	sprintf ( vl_NomTache, "%s%s%d.xs",XZECC_TACHE_EQUEXT, XZECC_TACHE_IP_MAITRE, atoi(pa_NumLiaison));
      /*A sinon sens esclave */
      else
	sprintf ( vl_NomTache, "%s%s%d.xs",XZECC_TACHE_EQUEXT, XZECC_TACHE_IP_ESCLAVE, atoi(pa_NumLiaison));
        break;

   case XZECC_PROT_APRIL :
   	sprintf ( vl_NomTache, "%s%s.xs", XZECC_TACHE_EQUEXT, XZECC_TACHE_APRILNET );
   	break;

   case XZECC_PROT_TEL	:
   	sprintf ( vl_NomTache, "%s%s%d.xs", XZECC_TACHE_EQUEXT, XZECC_TACHE_AUTOTEL, atoi(pa_NumLiaison) );   	
   	break;
   case XZECC_PROT_OPE	:
   	sprintf ( vl_NomTache, "%s%s%d.xs", XZECC_TACHE_EQUEXT, XZECC_TACHE_OPERATOR, atoi(pa_NumLiaison) );   	
        break;   
   default	:
   	sprintf ( vl_NomTache, "%s%s.xs", XZECC_TACHE_EQUEXT, pa_NumLiaison ); 
   	break;  	
   }	/*A fin switch */
	
   
   /*A Lecture du PID de la tache */
   if ( (vl_Pid = XZSS_20PidTache(vl_NomTache)) != 0 )
   {
       /* Arret de la tache  */
       XZST_03EcritureTrace( XZSTC_INFO, "Kill de <%s>", vl_NomTache );
       sleep (XZEXC_ATTENTE_ARRET_TACHE);
       sprintf ( vl_Commande, "kill %d", vl_Pid );
       if ( (vl_Cmd = popen (vl_Commande, "r"))!= NULL ) pclose ( vl_Cmd );
   }

}

