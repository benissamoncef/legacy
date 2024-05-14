/*E*/
/*Fichier : @(#)ex_mrtf.c	1.13      Release : 1.13        Date : 02/24/11
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  : EQUEXT
------------------------------------------------------
* MODULE COMMUN * FICHIER ex_mrtf.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module commun pour ecriture des messages RTworks dans un fichier >
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	02 Sep 1994	: Creation
* Mismer.D	version 1.2	23 Nov 1994	: Modif repertoire fichier message rtworks
* Fontaine.C	version 1.3	23 Mai 1995	: Reprise historique
* Mismer.D	version 1.4	02 Jun 1995	: Ajout fonction reconnexion auto rtserver
* Mismer.D	version 1.5	12 Jun 1995	: Modif n�1 fonction reconnexion auto rtserver 
* Mismer.D	version 1.6	30 Jun 1995	: Modif n�2 fonction reconnexion auto rtserver
* Mismer.D	version 1.7	31 Aou 1995	: Modif n�3 fonction reconnexion auto rtserver
* Mismer.D	version 1.8	31 Oct 1995	: Ajout trace si fermeture socket RTserver
* Mismer.D	version 1.9	10 Nov 1995	: Suppression TipcSrvMainLoop
* Mismer.D	version 1.10	08 Fev 1995	: Correction test nom tache dans ex_RecopieFichierMsg (DEM/1100)
*
* Mismer.D	version 1.11	10 Mar 1996	: Correction ex_InitCnxTache
* NIEPCERON	version 1.12	02/09/09	: Passage en niveau INFO de la trace Copie fichier %s/%s  vers %s executee DEM
* JPL		24/02/11 : Migration HP ia64 (DEM 975) : interface SS68 : type socket et args. des fonctions IPC  1.13
* SFO           06/10/14 : Adaptation pour compilation sous linux  1.14
* JPL		13/02/18 : Migration Linux 64 bits (DEM 1274) : Terminaison du parcours d'une liste variable d'arguments  1.15
* JMG		07/03/20 : EMS 1.16
* KOU		25/09/2023	: Adaptation code pour rabbitmq DEM-483
------------------------------------------------------*/

/* fichiers inclus */

#include "ex_msok.h"
#include "ex_mrtf.h"
#include "xzems.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "@(#)ex_mrtf.c	1.13 02/24/11 : ex_mrtf" ;

T_IPC_MSG_FILE		vg_MsgFile = NULL;

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Sauvegarde d'un message RTworks dans un fichier ASCII.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int ex_OuvertureFichierMsg (	XDY_NomMachine	pa_NomMachine )

/*
* ARGUMENTS EN ENTREE :
*
*  XDY_NomMachine	pa_NomMachine : Nom Machine
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
char 		pl_NomFichier[XDC_PATH_ABS_SIZE];
XDY_NomMachine  pl_NomMachine = "";
XZSCT_NomTache  vl_NomTache   = "";
#ifndef _TIBCO_EMS
	/* Lecture et verification du nom de la tache */
	XZSC_06NomTache( vl_NomTache );
	XZSC_07NomMachine( pl_NomMachine );
	if ( strcmp ( pa_NomMachine, pl_NomMachine ) )
	{
	   return ( XDC_OK );
	}
	
        /* Construction du nom du fichier source et destination */
        sprintf ( pl_NomFichier, "%s%s_%s", XZEXC_PATH_WRITE_FILE_MSG, vl_NomTache, pa_NomMachine );
        
	if ( (vg_MsgFile = TipcMsgFileCreate (pl_NomFichier, T_IPC_MSG_FILE_CREATE_APPEND )) == NULL)
	{
	   if ( (vg_MsgFile = TipcMsgFileCreate (pl_NomFichier, T_IPC_MSG_FILE_CREATE_WRITE )) == NULL)
	   {
	      XZST_03EcritureTrace(XZSTC_WARNING, "Echec ouverture fichier %s.", pl_NomFichier );
	      return ( XDC_NOK );
	   }
	}
#endif
	return ( XDC_OK );

}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Sauvegarde d'un message RTworks dans un fichier ASCII.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int ex_RecopieFichierMsg (	XDY_NomMachine	pa_NomMachine )

/*
* ARGUMENTS EN ENTREE :
*
*  XDY_NomMachine	pa_NomMachine : Nom Machine
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
*  Recopie du fichier de message RTworks dans le repertoire de lecture pour TEMOD
*
------------------------------------------------------*/
{
char 		pl_NomFichierSrc[XDC_PATH_ABS_SIZE],
		pl_NomFichierDes[XDC_PATH_ABS_SIZE];
XZSCT_NomTache  vl_NomTache	= "";
int		vl_cr = XDC_OK;
#ifndef _TIBCO_EMS
	/* Lecture du nom de la tache */
	XZSC_06NomTache( vl_NomTache );

        /* Construction du nom du fichier source et destination */
        sprintf ( pl_NomFichierSrc, "%s%s_%s", XZEXC_PATH_WRITE_FILE_MSG, vl_NomTache, pa_NomMachine );
        sprintf ( pl_NomFichierDes, "%s%s_%s", XZEXC_PATH_READ_FILE_MSG, vl_NomTache, pa_NomMachine );
        
        /* Si le fichier n'existe pas deja dans le repertoire de TEMOD */
        if ( !access(pl_NomFichierSrc, F_OK|R_OK) && access(pl_NomFichierDes, F_OK|R_OK) )
	{
            /* Construction du nom du fichier source et destination */
            sprintf ( pl_NomFichierSrc, "%s_%s", strtok(vl_NomTache, "/"), pa_NomMachine );
            sprintf ( pl_NomFichierDes, "%s_%s", strtok(vl_NomTache, "/"), pa_NomMachine );
            /* Recopie du fichier de message RTworks */
            if ( XZSS_07TransfertFichier (    pa_NomMachine,
               				      pl_NomFichierSrc,
               				      XZEXC_PATH_WRITE_FILE_MSG,
               				      pa_NomMachine,
               				      pl_NomFichierDes,
               				      XZEXC_PATH_READ_FILE_MSG ) != XDC_OK )
            {
                XZST_03EcritureTrace( XZSTC_WARNING, "Copie fichier %s%s  vers %s impossible",
               					   XZEXC_PATH_WRITE_FILE_MSG,
               					   pl_NomFichierSrc,
               					   XZEXC_PATH_READ_FILE_MSG );
                vl_cr = XDC_NOK;
            }
            else
            {
                XZST_03EcritureTrace( XZSTC_INFO, "Copie fichier %s/%s  vers %s executee",
               					   XZEXC_PATH_WRITE_FILE_MSG,
               					   pl_NomFichierSrc,
               					   XZEXC_PATH_READ_FILE_MSG );
               /*A Destruction du fichier de message */
	       if ( !TipcMsgFileDestroy ( vg_MsgFile ) )
	       {
	          XZST_03EcritureTrace( XZSTC_WARNING, "Impossible de detruire le fichier %s", pl_NomFichierSrc );
	       }
	       else
	       {
	          vg_MsgFile = NULL;
	          /* Suppression de l'ancien fichier de message */
                  sprintf ( pl_NomFichierSrc, "%s%s_%s", XZEXC_PATH_WRITE_FILE_MSG, vl_NomTache, pa_NomMachine );
	          XZSS_06DetruireFichier ( pl_NomFichierSrc );
	          /* Ouverture d'un nouveau fichier de message */
	          ex_OuvertureFichierMsg (pa_NomMachine);
	       }
	    }
	 }

#endif
	 return (vl_cr);
}





void ex_test ( int va_signal )
{

           XZST_03EcritureTrace(XZSTC_WARNING ,"Reception signal :%d", va_signal );


}




/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Connexion Socket RTserver.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int ex_ConnexionSocketRTserver ( 	T_SOCK 		*va_SockRT,
					int		va_SocketHS,
 					XZEXT_MASK *    *pa_masque )

/*
* ARGUMENTS EN ENTREE :
* XZEXT_MASK *    *pa_masque 	: liste des sockets tache EQUEXT
*
* ARGUMENTS EN SORTIE :
* int 		  *va_SockRT	: descripteur de la socket RTserver
*
* CODE RETOUR :
*  XDC_OK / XDC_NOK
*
* CONDITION D'UTILISATION
* 
* FONCTION 
*  
*
------------------------------------------------------*/
{
  int                vl_CodeRetour = 0;
  XZEXT_MASK         *pl_ListeSock;
  XZEXT_MASK         vl_masque;  
  T_SOCK	     vl_SockRT = 0;
  T_IPC_MSG	     vl_Msg;
  
#ifdef _TIBCO_EMS
*va_SockRT = -1000;
vl_Msg = TipcSrvMsgNext(500);
	      TipcMsgDestroy(vl_Msg);
return (XDC_OK);
#else

/*  vl_CodeRetour = TipcSrvMainLoop (0.01);
  if ( (vl_CodeRetour != 1) && (vl_CodeRetour != 0) )
     XZST_03EcritureTrace ( XZSTC_WARNING, "Erreur TipcSrvMainLoop :%d", vl_CodeRetour );*/
  
  if ( *va_SockRT == XZEXC_SOCKET_NULL )  *va_SockRT = 0;
  
  if ( va_SocketHS != 0 )
  {
     if ( va_SocketHS == *va_SockRT )
     {
        if ( ex_select(*va_SockRT, 0.001) == XDC_NOK )
	   XZST_03EcritureTrace (XZSTC_WARNING, "Erreur Socket RTWorks -- errno %d", errno );
     }
     
     for ( pl_ListeSock = *pa_masque; pl_ListeSock != NULL; pl_ListeSock = pl_ListeSock->Suivant )
     {
        if  ( (pl_ListeSock->desc_sock == va_SocketHS) )
        {
           XZST_03EcritureTrace ( XZSTC_WARNING, "Erreur sur socket %s Num:%d.", pl_ListeSock->adresse, pl_ListeSock->desc_sock);
           ex_fin_cnx ( va_SocketHS, pa_masque );
           if ( pl_ListeSock->desc_sock == *va_SockRT ) *va_SockRT = 0;
        }
     }
  }
  else
  {
     if ( *va_SockRT && (ex_select(*va_SockRT, 0.001) == XDC_OK) )
     {
        /*B Met les interruptions SIGALRM en attente	*/
        sighold(SIGALRM);
	while ( 1 )
	{
	   vl_Msg = TipcSrvMsgNext(0.0);
		if(isMessageNotNULL(vl_Msg))
	   {
	      TipcSrvMsgProcess(vl_Msg);
	      TipcMsgDestroy(vl_Msg);
	   }
	   else
	   {
	      if ( (TutErrNumGet() != T_ERR_CONN_EOF) && (TutErrNumGet() != T_ERR_TIMEOUT_REACHED) )
	      {
                 /*B Erreur RTWorks */
	         XZST_03EcritureTrace (XZSTC_WARNING, "Erreur RTWorks num:%d -- errno:%d.", TutErrNumGet(), errno);
	      }
	      break;
	   }
        }
        /*B Re-armement du traitement de SIGALRM */
        sigrelse(SIGALRM);
     }  
  }

  sighold(SIGALRM);
  vl_CodeRetour = TipcSrvGetSocket ( &vl_SockRT );
  sigrelse(SIGALRM);
  if ( !vl_CodeRetour )
  {
     XZST_03EcritureTrace (XZSTC_WARNING, "Erreur RTWorks sur TipcSrvGetSocket num: %d -- errno:%d -- ret:%d", TutErrNumGet(), errno, vl_CodeRetour );
     if ( *va_SockRT != 0 )
     {
         XZST_03EcritureTrace(XZSTC_WARNING, "Deconnexion RTServer." );
         ex_fin_cnx ( *va_SockRT, pa_masque );
         *va_SockRT = 0;
     }
     return ( XDC_NOK );
  }
  
  if ( vl_SockRT != 0 )
  {
     if ( *va_SockRT == 0 )
     {
        signal ( SIGHUP, SIG_IGN );
        vl_masque.desc_sock = vl_SockRT;
        vl_masque.Type = AF_INET;
        strcpy ( vl_masque.adresse, "RTserver" );
        ex_ins_masque ( &vl_masque, pa_masque );
        XZST_03EcritureTrace(XZSTC_WARNING, "Connexion socket RTserver." );
        *va_SockRT = vl_SockRT;
     }
     else
     {
         for ( pl_ListeSock = *pa_masque; pl_ListeSock != NULL; pl_ListeSock = pl_ListeSock->Suivant )
         {
            if  ( (pl_ListeSock->desc_sock == *va_SockRT) && (*va_SockRT != vl_SockRT))
            {
               XZST_03EcritureTrace(XZSTC_WARNING, "Nouvelle connexion socket RTserver." );
               pl_ListeSock->desc_sock = vl_SockRT;
               break;
            }
	 }
         *va_SockRT = vl_SockRT;
      }
   }

#endif
   return ( XDC_OK );

}


extern XZSCT_NomTache           XZSCV_NomTache;
extern int                      XZSCV_InitOk;
extern XZSTT_NiveauTrace        XZSTV_NiveauTrace;
extern int                      XZSCV_TailleFicTrace;

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Connexion Socket RTserver.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int ex_InitCnxTache ( XZSCT_NomTache NomTache,
                      void (*FonctionUtil)(),
                      int va_NivTache,
		      char *DestiStd )


/*
* ARGUMENTS EN ENTREE :
*
* ARGUMENTS EN SORTIE :
*
* CODE RETOUR :
*  XDC_OK / XDC_NOK
*
* CONDITION D'UTILISATION
* 
* FONCTION 
*  
*
------------------------------------------------------*/
{

   int			vl_ValRet 	= XDC_OK;
   char *		pl_P ;

   if(XZSCV_InitOk == XDC_FAUX)
   {
      if (NomTache != NULL)
      {
	 /* eventuellement, ne recupere que la partie */
         strncpy(XZSCV_NomTache
	        ,((pl_P = strrchr(NomTache,'/')) == NULL ? NomTache:pl_P+1)
		,sizeof(XZSCV_NomTache)-1
		) ;

	 /* Initialisation des variables globales de description
	    du noyau de supervision de la tache */
	 vl_ValRet = XZSC_InitDescNoyau();

         if(FonctionUtil == NULL)
            signal(SIGTERM, FonctionUtil);

             /* Redirection de la sortie standard et de la sortie d'erreur
	    standard */
         if ((vl_ValRet == XDC_OK) && (DestiStd != NULL))
	 {
	    if((freopen(DestiStd, "w", stdout) == NULL) ||
		(freopen(DestiStd, "w", stderr) == NULL))
            {
	       vl_ValRet = XZSCC_SYS_INV;
            }
         }

	 /* Affectation de l'indicateur d'initialisation */
         if(vl_ValRet == XDC_OK)
	 {
	    XZSCV_InitOk = XDC_VRAI;
         }

	 /* Mise en place eventuelle du systeme de trace */
	 if ( vl_ValRet == XDC_OK )
	 {
	    XZSCV_TailleFicTrace = 50000;
            vl_ValRet = XZST_01OuvertureTrace();
            XZSTV_NiveauTrace = va_NivTache;
         }

      }
      else
      {
	 /* Nom de la tache indefini */
	 vl_ValRet = XZSCC_NOT_INV;
      }
   }
   else
   {
      /* initialisation deja effectuee */
      vl_ValRet = XZSCC_INI_INV;
   }
   return(vl_ValRet);

}	







