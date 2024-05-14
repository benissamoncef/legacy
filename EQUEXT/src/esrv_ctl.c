/*E*/
/* Fichier : $Id: esrv_ctl.c,v 1.21 2020/11/03 18:12:31 pc2dpdy Exp $      Release : $Revision: 1.21 $        Date : $Date: 2020/11/03 18:12:31 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TESRV * FICHIER esrv_ctl.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module de gestion des fonction de controle des taches equipements  (lancement et arret>
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	23 Aug 1994	: Creation
* Mismer.D	version 1.2	28 Oct 1994	:
* Mismer.D 	version 1.3 	29 Nov 1994 	:
* Mismer.D	version 1.4 	08 Dec 1994 	:
* Mismer.D	version 1.5	09 Dec 1994	:
* Mismer.D 	version 1.6	12 Jan 1995	:
* Mismer.D	version 1.7	25 Jan 1995	:
* Mismer.D 	version 1.8	26 Jan 1995	:
* Mismer.D	version 1.9	24 Avr 1995	:
* Mismer.D	version 1.10	10 Mai 1995	:
* Fontaine.C	version 1.11	22 Mai 1995	: Reprise entete, historique
* Mismer.D	version 1.12	24 Mai 1995	: Modif pour lancement tache protocole AprilNet
* Mismer.D	version 1.13	24 Mai 1995	: Correction modif pour lancement tache protocole AprilNet
* Mismer.D	version 1.14	06 Jun 1995	: Modif pour forcage arret tache
* Mismer.D	version 1.15	01 Sep 1995	: Modif time-out apres un fork
* ???		version 1.16	xx/xx/06	: Tibco SS66 : 'vfork' change en 'fork'
*
* JMG		version 1.18 26/06/10	: linux DEM/934
* JPL		24/02/11 : Migration architecture HP ia64 (DEM 975) : suppression warnings mineurs  1.19
* JMG		24/02/18 : tache IP CTRL_FEUX 1.20 DEM1284
* LCL		20/12/21	: MOVIS P2 ems SAE-312
------------------------------------------------------*/

/* fichiers inclus */

#include "ex_mdon.h"

#include "esrv_art.h"
#include "esrv_ctl.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: esrv_ctl.c,v 1.21 2020/11/03 18:12:31 pc2dpdy Exp $ : esrv_ctl" ;

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
static char *version = "$Id: esrv_ctl.c,v 1.21 2020/11/03 18:12:31 pc2dpdy Exp $ : el_copier_fic_exe " ;
XDY_NomTache	vl_NomTacheSrc, vl_NomTacheDest;
XDY_NomMachine	vl_NomMachine = "";
char 		pl_NomRepertoire[XDC_PATH_ABS_SIZE];
XZEXT_MASK *   	pl_masque = NULL;


	/*A Recuperation du nom de la machine */
	if ( (XZSC_07NomMachine(vl_NomMachine) != XDC_OK) || (strlen (vl_NomMachine) == 0) )
	{
	   /* Ecriture Trace */
	   XZST_03EcritureTrace( XZSTC_WARNING, "Recuperation du Nom de la machine impossible ." );
	   return ( XDC_NOK );
	} 


   	sprintf ( vl_NomTacheDest, "%s%s%d.x", XZECC_TACHE_EQUEXT, pa_NomProtocole, atoi(pa_NumLiaison) );
   	sprintf ( vl_NomTacheSrc, "%s%si.x", XZECC_TACHE_EQUEXT, pa_NomProtocole );
   	if ( XZSS_20PidTache ( vl_NomTacheDest ) != 0 )
   	{
            if (!strcmp(vl_NomTacheDest ,"telm7.x"))
		XZST_03EcritureTrace( XZSTC_INFO, "Tache protocole %s deja en cours", vl_NomTacheDest );
	    return ( XDC_NOK );
   	   
   	}
   	
	if (!strcmp(vl_NomTacheDest ,"telm7.x"))
		
        
	/************************************************************************************************/
{
	static char *	version = "@(#)xzss.c	1.28 10/14/98 : XZSS_20PidTache" ;

	char commande[XDC_LINE_CMD_SIZE]	= "";
	FILE *ps				= NULL;
	char line[XDC_LINE_CMD_SIZE] 		= "";

	XZST_03EcritureTrace( XZSTC_INFO, "Tache protocole %s deja en cours", vl_NomTacheDest );

	/* Recuperation du PID de la tache */
	sprintf(commande,"ps -ef | awk \'{if (length($5) == 3) {if ($9 == \"%s\"){print $2}} else{if ($8 == \"%s\"){print $2}}}\'",vl_NomTacheDest,vl_NomTacheDest);

		XZST_03EcritureTrace(XZSTC_WARNING,
			  "Recherche PID tache %s -- Cmd %s \n",
			  vl_NomTacheDest,commande);

 	/* * Get processes information.  */
	if ((ps = popen (commande, "r")) != NULL) 
	{
		/* Recuperation de la ligne indiquant le PID */
		errno =  0;
		fgets(line,XDC_LINE_CMD_SIZE,ps);	
		if (errno)
		{
			/* ecriture trace pour connaitre la ligne lu */
			XZST_03EcritureTrace(XZSTC_WARNING,
			  "ERROR fgets PID %d ",errno);
		}

		/* ecriture trace pour connaitre la ligne lu */
		XZST_03EcritureTrace(XZSTC_WARNING,
			  "PID %s trouve pour tache %s",line,vl_NomTacheDest);

#if 1 
			/* Modif V1.27 */	
		/* Fermeture du Fichier */
		(void) pclose (ps);
#endif
	}
	else 
	{
		/*  Ecriture trace indiquant un probleme de lecture PID */
		XZST_03EcritureTrace(XZSTC_WARNING,
		  "lecture du PID par popen a echou� pour %s -- errno %d\n",vl_NomTacheDest,errno);
	}

#if 0
	(void) pclose (ps);
#endif

	/* On n a pas trouve le pid de la tache !!! */
   	if (!strcmp(line,""))
	{
		strcpy(line,"0");
	}
					   
	XZST_03EcritureTrace(XZSTC_WARNING,
			"XZSS_20PidTache : OUT  Pid : %s \n",line);





}	/* Fin XZSS_20PidTache */

/********************************************************************************************/

	if ( XZSS_07TransfertFichier (    vl_NomMachine,
               				  vl_NomTacheSrc,
               				  XZEXC_PATH_TACHE_EQUEXT,
               				  vl_NomMachine,
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
static char *version = "$Id: esrv_ctl.c,v 1.21 2020/11/03 18:12:31 pc2dpdy Exp $ : el_lancer_tache_protocole " ;
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
   	 if ( (vl_ValRet = el_copier_fic_exe (pa_NumLiaison, XZECC_TACHE_LCR_MAITRE, pl_NomTache)) == XDC_OK )
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
   
   case XZECC_PROT_IP	:
   case XZECC_PROT_DIASER	:
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
        /*A Construction du nom de la tache XZECC_TACHE_APRILNET */
   	sprintf ( pl_NomTache, "%s%s.x", XZECC_TACHE_EQUEXT, XZECC_TACHE_APRILNET );
   	if ( XZSS_20PidTache ( pl_NomTache ) != 0 )
   	{
            XZST_03EcritureTrace( XZSTC_INFO, "Tache protocole %s deja en cours", pl_NomTache );
	    return ( XDC_NOK );
   	}
   	/*A Appel de la fonction el_lancer_tache */
   	sprintf ( pl_NomTache, "%s%s", XZECC_TACHE_EQUEXT, XZECC_TACHE_APRILNET );
   	vl_ValRet = el_lancer_tache ( pl_NomTache, pa_NumLiaison );
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
   	   vl_ValRet = el_lancer_tache ( pl_NomTache, pa_NumLiaison );
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
static char *version = "$Id: esrv_ctl.c,v 1.21 2020/11/03 18:12:31 pc2dpdy Exp $ : el_lancer_tache_eqt " ;
register int	vl_ValRet = XDC_OK;
XDY_NomTache	pl_NomTache;
XDY_NomTache	pl_NomTacheTrt;


   /*A Construction du nom de la tache TEzzz */
   sprintf ( pl_NomTache, "%s%s", XZECC_TACHE_EQUEXT, pa_TypeEqt );
   sprintf ( pl_NomTacheTrt, "%s%s.x", XZECC_TACHE_EQUEXT, pa_TypeEqt );
   /*A Appel de la fonction el_lancer_tache */
   
   if ( XZSS_20PidTache ( pl_NomTacheTrt ) != 0 )
   {
      if ( !strcmp(pl_NomTacheTrt ,"tepma.x") )
		XZST_03EcritureTrace( XZSTC_WARNING, "Tache traitement %s deja en cours", pl_NomTacheTrt );
      vl_ValRet = XDC_NOK;
   }
   else
   {
      if (!strcmp(pl_NomTacheTrt ,"telm7.x") || !strcmp(pl_NomTacheTrt ,"tepma.x") )
	
{
	static char *	version = "@(#)xzss.c	1.28 10/14/98 : XZSS_20PidTache" ;

	char commande[XDC_LINE_CMD_SIZE]	= "";
	FILE *ps				= NULL;
	char line[XDC_LINE_CMD_SIZE] 		= "";

		XZST_03EcritureTrace( XZSTC_WARNING, "Tache traitement %s relanc�e", pl_NomTacheTrt);


	/* Recuperation du PID de la tache */
	sprintf(commande,"ps -ef | awk \'{if (length($5) == 3) {if ($9 == \"%s\"){print $2}} else{if ($8 == \"%s\"){print $2}}}\'",pl_NomTacheTrt,pl_NomTacheTrt);

		XZST_03EcritureTrace(XZSTC_WARNING,
			  "Recherche PID tache %s -- Cmd %s \n",
			  pl_NomTacheTrt,commande);

 	/* * Get processes information.  */
	if ((ps = popen (commande, "r")) != NULL) 
	{
		/* Recuperation de la ligne indiquant le PID */
		errno =  0;
		fgets(line,XDC_LINE_CMD_SIZE,ps);	
		if (errno)
		{
			/* ecriture trace pour connaitre la ligne lu */
			XZST_03EcritureTrace(XZSTC_WARNING,
			  "ERROR fgets PID %d ",errno);
		}

		/* ecriture trace pour connaitre la ligne lu */
		XZST_03EcritureTrace(XZSTC_WARNING,
			  "PID %s trouve pour tache %s",line,pl_NomTacheTrt);

#if 1 
			/* Modif V1.27 */	
		/* Fermeture du Fichier */
		(void) pclose (ps);
#endif
	}
	else 
	{
		/*  Ecriture trace indiquant un probleme de lecture PID */
		XZST_03EcritureTrace(XZSTC_WARNING,
		  "lecture du PID par popen a echou� pour %s -- errno %d\n",pl_NomTacheTrt,errno);
	}



	/* On n a pas trouve le pid de la tache !!! */
   	if (!strcmp(line,""))
	{
		strcpy(line,"0");
	}
					   
	XZST_03EcritureTrace(XZSTC_WARNING,
			"XZSS_20PidTache : OUT  Pid : %s \n",line);



}	/* Fin XZSS_20PidTache */

      vl_ValRet = el_lancer_tache ( pl_NomTache, NULL );
   }
   
   return ( vl_ValRet );
   
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
  static char *version = "$Id: esrv_ctl.c,v 1.21 2020/11/03 18:12:31 pc2dpdy Exp $ : el_lancer_tache " ;
  int			vl_ValRet 	= XDC_OK;
  XDY_NomTache		vl_NomTache	= "";
  XDY_NomMachine	vl_NomMachine 	= "";
  XDY_Nom		vl_LS 	      	= "";
  int			vl_cr		= 0;
  static char		*vl_argvt[3]; 	/* tableau de pointeurs de strings */
  char                  vl_Commande [XZSSC_LINE_CMD_SIZE] = "";
  FILE*                 vl_Cmd 		= NULL;
  int                   vl_Pid 		= 0;

	/*A Recuperation du nom de la machine */
	if ( (XZSC_07NomMachine(vl_NomMachine) != XDC_OK) || (strlen (vl_NomMachine) == 0) )
	{
	   /* Ecriture Trace */
	   XZST_03EcritureTrace( XZSTC_WARNING, "Recuperation du Nom de la machine impossible ." );
	} 
	
	
	/*A Atteindre le repertoire contenant les taches EQUEXT */
        chdir ( XZEXC_PATH_TACHE_EQUEXT );

	/*A Construction du nom complet de la tache */
        sprintf ( vl_NomTache, "%s.x", pa_NomTache );
        strcpy  ( vl_LS, (pa_NumLiaison != NULL) ? pa_NumLiaison : "" );
        if ( access(vl_NomTache, F_OK) )
        {
            XZST_03EcritureTrace( XZSTC_WARNING, "Manque executable : <%s>", vl_NomTache );
	    return ( XDC_NOK );
	}
	
	if ( !strcmp(pa_NomTache, XZEXC_TACHE_TETIM) )
	{
	   while ( (vl_Pid = XZSS_20PidTache(vl_NomTache)) != 0 )
          {
             /* Arret de la tache  */
             XZST_03EcritureTrace( XZSTC_INFO, "Kill de <%s>", vl_NomTache );
             sprintf ( vl_Commande, "kill %d", vl_Pid );
             if ( (vl_Cmd = popen (vl_Commande, "r"))!= NULL ) pclose ( vl_Cmd );
             sleep (1);
	   }
	}
	
	
	/*A Creation de la commande de lancement de la tache */
	vl_argvt[0] = vl_NomTache;		/* nom de la commande 		$0 */
	vl_argvt[1] = vl_NomMachine;		/* nom de la machine		$1 */
	vl_argvt[2] = vl_LS;			/* argument 2  de la commande	$2 */

	/*A lance la tache par le principe du fork unix */

	/* vl_cr = vfork();*/
        /*TIBCO SS66*/
        vl_cr = fork();

	if (vl_cr == 0 )
	{  /* process fils */
	   execvp( vl_argvt[0], vl_argvt );
           exit(0);
	}

	sleep (1);
	/*A Lecture du PID de la tache */
   	if ( XZSS_20PidTache ( vl_NomTache ) == 0 )
   	{
            XZST_03EcritureTrace( XZSTC_WARNING, "Echec du fork pour <%s>", vl_NomTache );
            ex_rep_socket ( vl_NomMachine );
	    return ( XDC_NOK );
   	}
	

       ex_rep_socket ( vl_NomMachine );
       return( XDC_OK );
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Fonction permettant d'arreter une tache.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int el_arreter_tache ( int	va_TypeTache )

/*
* ARGUMENTS EN ENTREE :
*  va_TypeTache		: Type de la tache a arreter
*
*
* ARGUMENTS EN SORTIE :
*   aucun
*
*
* CODE RETOUR : 
*   XDC_OK		: Succes
*   XDC_NOK		: Echec
*
* CONDITION D'UTILISATION
*    
*
* FONCTION 
*   Envoie du message de demande d'arret par socket 
*
------------------------------------------------------*/
{
static char *version = "$Id: esrv_ctl.c,v 1.21 2020/11/03 18:12:31 pc2dpdy Exp $ : el_arreter_tache " ;
XDY_NomTache	pl_NomTache;
char		pl_PrefTache[5];
XZEXT_MASK *	pl_masque;

	/*A Construction du nom de la tache a partir du type d'equipement */
	if ( ex_nom_eqt ( va_TypeTache, pl_PrefTache ) != XDC_OK )
	{
	   /*A retourne echec type de tache incorrecte */
	   return ( XDC_NOK );
	}
	sprintf ( pl_NomTache, "%s%s", XZECC_TACHE_EQUEXT, pl_PrefTache );
	
	/*A 
	 * Recherche si la tache existe ( scrutation de la liste des sockets ) 
	 */
	 
	/*A Pour toute les sockets du masque */
       for ( pl_masque = vg_SockMask; pl_masque != NULL; pl_masque = pl_masque->Suivant )
       {
          /*A Si differente de la socket RTserver  et differente de la socket d'ecoute*/
          if ( ( pl_masque->desc_sock != vg_SockRT ) && ( pl_masque->desc_sock != vg_SockEct ) )
          {
	      /*A Si l'adresse de la socket correspont au nom de la tache */
	      if ( strstr ( pl_masque->adresse, pl_NomTache ) != NULL )
	      {
                 /*A Appel de la fonction ee_config pour emission de la demande d'arret */
	         ee_config ( XZEXC_DEMANDE_ARRET, pl_masque->desc_sock, pl_masque->adresse );	      
	      }	     
	   }   
	}   /* fin for */	 

	return (XDC_OK);
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
  static char *version = "$Id: esrv_ctl.c,v 1.21 2020/11/03 18:12:31 pc2dpdy Exp $ : el_arret_tache " ;
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
   	sprintf ( vl_NomTache, "%s%s%d.x", XZECC_TACHE_EQUEXT, XZECC_TACHE_LCR_MAITRE, atoi(pa_NumLiaison) );   	
      /*A sinon sens esclave */
      else
   	sprintf ( vl_NomTache, "%s%s%d.x", XZECC_TACHE_EQUEXT, XZECC_TACHE_LCR_ESCLAVE, atoi(pa_NumLiaison) );   	
      break;
   
   case XZECC_PROT_APRIL :
   	sprintf ( vl_NomTache, "%s%s.x", XZECC_TACHE_EQUEXT, XZECC_TACHE_APRILNET );
   	break;

   case XZECC_PROT_TEL	:
   	sprintf ( vl_NomTache, "%s%s%d.x", XZECC_TACHE_EQUEXT, XZECC_TACHE_AUTOTEL, atoi(pa_NumLiaison) );   	
   	break;
   case XZECC_PROT_OPE	:
   	sprintf ( vl_NomTache, "%s%s%d.x", XZECC_TACHE_EQUEXT, XZECC_TACHE_OPERATOR, atoi(pa_NumLiaison) );   	
        break;   
   default	:
   	sprintf ( vl_NomTache, "%s%s.x", XZECC_TACHE_EQUEXT, pa_NumLiaison ); 
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

