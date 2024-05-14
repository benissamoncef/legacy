/*E*/
/*Fichier :  @(#)eniw_tra.c	1.3      Release : 1.3        Date : 05/17/02
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR  
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TENIW * FICHIER eniw_tra.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* c'est le module de traitement des sockets provenant de Wizcon maitre
* et Wizcon esclave pour la tache TENIW, gtc Nice
*
------------------------------------------------------
* HISTORIQUE :
*
* Moulin.D	version 1.1	07 Janv 2002	: Creation
* Moulin.D	version 1.2	23 Avr 2002	: Installation PLate forme
* Moulin.D	version 1.3	15 Mai 2002	: Correction anomalie douuble envoi alerte
* Moulin.D	version 1.4	19 Sep 2002	: Modification gestion trame et capteur niche 
* Moulin.D	version 1.5	30 Sep 2002	: modification Horodate
* PNI 		Version 1.18	12 Jan 2007	: Pas d'envoi de status NOK pour une discordance panneau sur Tunnel DEM609
* JMG   13/11/07        : ajout site de gestion DEM715
* ADB   14/05/09        : mise a jour des traces
*
------------------------------------------------------*/
 
/* fichiers inclus */
#include <xzsem.h>

#include "eniw_tra.h"

/* definitions de constantes */
#define ENIW_Lon_Fin_Trame	5

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

/* declaration de fonctions internes */
/* definition de fonctions externes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Decodage de la socket de Wizcon esclave ou Wizcon maitre
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int et_traitement ( int va_socket )

/*
* ARGUMENTS EN ENTREE :
*   
* va_socket	: socket de lecture
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
*
* CONDITION D'UTILISATION
* execute sur reception de message socket
*
* FONCTION 
*
*  Decodage de la trame et traitement
------------------------------------------------------*/
{
   int				vl_IndCnx = 0,
   					vl_LgMsg  = 0,
   					vl_LgReelle  = 0;
   XZEXT_MSG_SOCKET	pl_Msg;
   char *	pl_Message;
   char *	pl_pos;
   
	/*A
	*  Lecture du message recu dans la socket appelante
	*/
 

	fcntl ( va_socket, F_SETFL, O_NDELAY | fcntl ( va_socket, F_GETFL, 0 ) );
	sem_wait(&vg_semaphore); 
   	if  ( (vl_LgMsg = recv ( va_socket, pl_Msg, sizeof(XZEXT_MSG_SOCKET)-2, MSG_PEEK )) == -1 ) 				  				
   	{   
		   sem_post(&vg_semaphore); 
      		/*B Ecriture dans le fichier trace en cas d'�chec */
      		XZST_03EcritureTrace( XZSTC_WARNING, "TRAME : Erreur de lecture de message socket Wizcon.");
		return ( XDC_NOK );
   	}
	sem_post(&vg_semaphore); 

	/*A Si il s'agit d'une deconnexion */
	if ( vl_LgMsg == 0 )
	{
         	/*A Fermeture de la socket */
			sem_wait(&vg_semaphore); 
         	read ( va_socket, pl_Msg, sizeof(XZEXT_MSG_SOCKET)-2 );
			sem_post(&vg_semaphore); 
         	ex_fin_cnx ( va_socket, &vg_SockMask );
		/* Ecriture trace . */
		XZST_03EcritureTrace( XZSTC_WARNING, "TRAME :  Demande d�connexion ou message vide de la socket de Wizcon. " );
		return ( XDC_NOK );
	}

	/* Vidage du buffer */
		sem_wait(&vg_semaphore); 
        read ( va_socket, pl_Msg, vl_LgMsg );
		sem_post(&vg_semaphore); 

	pl_Msg[vl_LgMsg]='\0';

	/* Longueur reelle de la chaine*/
	vl_LgReelle = strlen(pl_Msg);

	/* Init des pointeurs*/
	pl_Message=pl_Msg;
	pl_pos=pl_Msg;

	/* Tant qu'il faut en lire */
	while (vl_LgMsg > 0)

	{

		XZST_03EcritureTrace( XZSTC_INFO, "TRAME : Taille message totale <%d>, taille <%d> message <%s>. ",vl_LgMsg,vl_LgReelle ,pl_Message );

		/* Appel du traitement */
		et_analyse(pl_Message,va_socket);

		/* On se d�cale */
		pl_pos+=vl_LgReelle+1;	   
		pl_Message=pl_pos;
	
		/* La longueur restante */
		vl_LgMsg= vl_LgMsg - vl_LgReelle-1;
		vl_LgReelle=strlen(pl_Message);

	}
}

/*------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int et_analyse (char * pa_Msg , int va_socket )

/*
* ARGUMENTS EN ENTREE :
*   
* va_socket	: socket de lecture
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
*
* CONDITION D'UTILISATION
* execute sur reception de message socket
*
* FONCTION 
*
*  Decodage de la trame et traitement
------------------------------------------------------*/
{
   int				vl_IndCnx = 0,
   					vl_LgMsg  = 0;
   XDY_Horodate		vl_Horodate; 
   XDY_Texte		vl_TexteAlarme;
   int				vl_typetrame;
   int				vl_Etat_Service = 0;
   char	*			vl_Mot;
   struct tm *		pl_InfoTemps ;
   struct tm 		pl_Temps ;
   time_t           vl_Heure;
   char				vl_fin_chaine[ENIW_Lon_Fin_Trame];
   short			vl_Etat,
   					vl_Change[XZETC_MAX_PANNEAU/XZETC_NB_ETAT_PAN];
   int				vl_TypeEvt,vl_TypeCmd,
   					 vl_TypeEqt, vl_NumEqt,
   					vl_Index,
   					vl_NumBit1, vl_NumBit2, vl_NumMot1, vl_NumMot2,
   					vl_EtatPan1, vl_EtatPan2,
   					vl_OP, vl_CO;
   int vl_JourSem;
   ENIW_DONNEES_Panneau	*	pl_Panneau;
   ENIW_DONNEES_Sequence *	pl_Sequence;
   ENIW_DONNEES_Capteur	*	pl_Capt;
   ENIW_DONNEES_Tube *		pl_Tube;
   ENIW_DONNEES_Echangeur *	pl_Ech;
   div_t			vl_div;
   XDY_Datagroup		pl_NomDG;
   XDY_NomSite      	  	pl_NomSite;
   short			vl_energie,vl_local,vl_lampe,
					vl_typecapteur,vl_bitnumcapteur,vl_numcapteur,vl_numalerte,vl_etatcapteur,
					vl_numsequence,vl_etatdisc,
					vl_numpanneau,vl_etatpanneau;
   
	/* Pointeur sur une structure de temps existantes */
	pl_InfoTemps=&pl_Temps;

	vl_Mot=strtok(pa_Msg,"|");
	XZST_03EcritureTrace( XZSTC_INFO, "TRAME : Type de commande <%s>. ", vl_Mot);

	/*A Scan du type de commande re�u provenant de la socket */
	if ( vl_Mot == NULL )
	{
		/* Ecriture trace Erreur l */
		XZST_03EcritureTrace( XZSTC_WARNING, "TRAME :  Impossible de lire la commande du message Wizcon. " );
		return ( XDC_NOK );
	}

	vl_TypeCmd=atoi(vl_Mot);
	vl_TypeEvt=vl_TypeCmd; /* c'est pareil */

	/* La trame maitre est particuli�re, elle est courte*/
	if ( vl_TypeCmd == ENIW_REC_MAITRE )
	{
		/* Si la maitre est la tache Wizcon maitre */
		if (va_socket == vg_SockTWWI0)
		{
			XZST_03EcritureTrace( XZSTC_INFO, "TRAME :  Reception message <maitre actif> depuis le Wizcon principal. " );
			vg_maitre_actif = XDC_MAITRE_TWWI0;
		}

		/* Si la maitre est la tache Wizcon esclave */
		if (va_socket == vg_SockTWWI1)
		{
			XZST_03EcritureTrace( XZSTC_INFO, "TRAME :  Reception message <maitre actif> depuis le Wizcon secondaire. " );
			vg_maitre_actif = XDC_MAITRE_TWWI1;
		}
		return(XDC_OK);
	}

	/* On lit le Type d'equipement */
	vl_Mot=strtok(NULL,"|");
	XZST_03EcritureTrace( XZSTC_INFO, "TRAME : Valeur type eqt <%s>. ", vl_Mot);

	if ( vl_Mot == NULL )
	{
		/* Ecriture trace Erreur l */
		XZST_03EcritureTrace( XZSTC_WARNING, "TRAME : Impossible de lire la type Eqt du message Wizcon. " );
		return ( XDC_NOK );
	}
   	vl_TypeEqt=atoi(vl_Mot);

	/* On lit le Numero d'equipement */
	vl_Mot=strtok(NULL,"|");
	XZST_03EcritureTrace( XZSTC_INFO, "TRAME : Valeur num eqt <%s>. ", vl_Mot);

	if ( vl_Mot == NULL )
	{
		/* Ecriture trace Erreur l */
		XZST_03EcritureTrace( XZSTC_WARNING, "TRAME : Impossible de lire le numero Eqt du message Wizcon. " );
		return ( XDC_NOK );
	}
   	vl_NumEqt=atoi(vl_Mot);

	/* Lecture de l'horodate */ 
	vl_Mot=strtok(NULL,"~");
	XZST_03EcritureTrace( XZSTC_INFO, "TRAME : Valeur annee <%s>. ", vl_Mot);
	pl_InfoTemps->tm_year=atoi(vl_Mot)-1900;

	vl_Mot=strtok(NULL,"~");
	XZST_03EcritureTrace( XZSTC_INFO, "TRAME : Valeur mois <%s>. ", vl_Mot);
	pl_InfoTemps->tm_mon=atoi(vl_Mot);

	vl_Mot=strtok(NULL,"~");
	XZST_03EcritureTrace( XZSTC_INFO, "TRAME : Valeur jour <%s>. ", vl_Mot);
	pl_InfoTemps->tm_mday=atoi(vl_Mot);

	vl_Mot=strtok(NULL,"~");
	XZST_03EcritureTrace( XZSTC_INFO, "TRAME : Valeur heure <%s>. ", vl_Mot);
	pl_InfoTemps->tm_hour=atoi(vl_Mot);

	vl_Mot=strtok(NULL,"~");
	XZST_03EcritureTrace( XZSTC_INFO, "TRAME : Valeur minute <%s>. ", vl_Mot);
	pl_InfoTemps->tm_min=atoi(vl_Mot);

	vl_Mot=strtok(NULL,"|");
	XZST_03EcritureTrace( XZSTC_INFO, "TRAME : Valeur seconde <%s>. ", vl_Mot);
	pl_InfoTemps->tm_sec=atoi(vl_Mot);

        XZSM_07LireHorodate( XZSMC_TYPE_SYSTEME, &vl_JourSem, &vl_Horodate );

	if (vl_Mot == NULL)
	{
		/* Ecriture trace Erreur l */
		XZST_03EcritureTrace( XZSTC_WARNING, "TRAME : Impossible de lire l'horodate du message Wizcon. " );
	 }

	if (vl_TypeEqt == XDC_EQT_TUBE)
		XZST_03EcritureTrace( XZSTC_INFO, "ANALYSE : Recherche du Tube correspondant");
	else
		XZST_03EcritureTrace( XZSTC_INFO, "ANALYSE : Recherche de l'echangeur coorespondant");
      
	/*A Recherche dans la liste des tubes si type evenement tube*/
	for ( pl_Tube = pg_debutTube; 
		  (vl_TypeEqt == XDC_EQT_TUBE) && (pl_Tube != NULL); 
		  pl_Tube = pl_Tube->Suivant )
	{
	  if ( pl_Tube->Numero == vl_NumEqt )
	  {
		 vl_Etat_Service = pl_Tube->Etat_Service;
		 XZST_03EcritureTrace( XZSTC_INFO, "ANALYSE : Tube N� %d trouve, etat de service %d.",pl_Tube->Numero,vl_Etat_Service );
		 break;
	  }
	}
   
	/*A Recherche dans la liste des echangeurs si type evenement echangeur */
	for ( pl_Ech = pg_debutEchangeur; 
		  (vl_TypeEqt == XDC_EQT_ECH) && (pl_Ech != NULL);
		  pl_Ech = pl_Ech->Suivant )
	{
	  if ( pl_Ech->NumeroDemiEch == vl_NumEqt )
	  {
		 vl_Etat_Service = pl_Ech->Etat_Service;
		 XZST_03EcritureTrace( XZSTC_INFO, "ANALYSE: Ech N� %d trouv�, etat de service %d.",pl_Ech->NumeroDemiEch,vl_Etat_Service );
		 break;
	  }
	}
            
    /* Retour a l'appelant si echangeur ou tube non trouve */      
    if ( (pl_Tube == NULL) && (pl_Ech == NULL) ) 
    {
       XZST_03EcritureTrace( XZSTC_WARNING, "ANALYSE:Numero de tube ou d'echangeur non trouve.=>SORTIE" );
    	return;
    }
      					 
    /*A Recuperation du site de la machine */
    if ( XZSC_04NomSite(pl_NomSite) != XDC_OK)
    {
       /* Ecriture Trace */
       XZST_03EcritureTrace( XZSTC_WARNING, "ANALYSE : Recuperation du Nom du site impossible ." );
    } 

   XZST_03EcritureTrace( XZSTC_INFO, "ANALYSE : Analyse detaillee du type d'evenement <%d>", vl_TypeEvt);
   /*A Recherche du type d'evenement */
    switch( vl_TypeEvt )
    {
         /*A C'est une Telealarame */
         case ENIW_REC_TELEALARME :
		/* Lecture energie */
		XZST_03EcritureTrace( XZSTC_INFO, "ANALYSE : Analyse detaillee du type d'evenement <%d> : TELEALARME", vl_TypeEvt);
		vl_Mot=strtok(NULL,"|");
		XZST_03EcritureTrace( XZSTC_INFO, "ANALYSE: Valeur energie TELELARME <%s>. ", vl_Mot);
		vl_energie=atoi(vl_Mot);
		if ( vl_Mot == NULL )
		{
			/* Ecriture trace Erreur l */
			XZST_03EcritureTrace( XZSTC_WARNING, "ANALYSE: Impossible de lire la donnee energie de la TELELARME du message Wizcon. " );
			return ( XDC_NOK );
		}

		/* Lecture local */
		vl_Mot=strtok(NULL,"|");
		XZST_03EcritureTrace( XZSTC_INFO, "ANALYSE: Valeur local TELELARME <%s>. ", vl_Mot);
		vl_local=atoi(vl_Mot);
		if ( vl_Mot == NULL )
		{
			/* Ecriture trace Erreur l */
			XZST_03EcritureTrace( XZSTC_WARNING, "ANALYSE: Impossible de lire la donnee gestion local de la TELELARME du message Wizcon. " );
			return ( XDC_NOK );
		}

		/* Lecture lampe */
		vl_Mot=strtok(NULL,"|");
		XZST_03EcritureTrace( XZSTC_INFO, "ANALYSE: Valeur lampe TELELARME <%s>. ", vl_Mot);
		vl_lampe=atoi(vl_Mot);
		if ( vl_Mot == NULL )
		{
			/* Ecriture trace Erreur l */
			XZST_03EcritureTrace( XZSTC_WARNING, "ANALYSE: Impossible de lire la donnee lampe de la TELELARME du message Wizcon. " );
			return ( XDC_NOK );
		}
		break;

		 /*A C'est un ETOR */
         case ENIW_REC_ETOR :
		 /*A C'est un CO */
         case ENIW_REC_CO :		 
		 /*A C'est un ALERTE */
         case ENIW_REC_ALERTE :
		/* Lecture Type capteur */
		XZST_03EcritureTrace( XZSTC_INFO, "ANALYSE : Analyse detaillee du type d'evenement <%d> : ETOR, CO ou ALERTE", vl_TypeEvt);
		vl_Mot=strtok(NULL,"|");
		XZST_03EcritureTrace( XZSTC_INFO, "ANALYSE :  Valeur type capteur <%s>. ", vl_Mot);
		vl_typecapteur=atoi(vl_Mot);
		if ( vl_Mot == NULL )
		{
			/* Ecriture trace Erreur l */
			XZST_03EcritureTrace( XZSTC_WARNING, "ANALYSE :  Impossible de lire le type de capteur du message Wizcon. " );
			return ( XDC_NOK );
		}

		/* Lecture Numero capteur */
		vl_Mot=strtok(NULL,"|");
		XZST_03EcritureTrace( XZSTC_INFO, "ANALYSE :  Valeur numero bit sur capteur <%s>. ", vl_Mot);
		vl_bitnumcapteur=atoi(vl_Mot);
		/* Pour l'instant le capteur est inconnu */
		vl_numcapteur=-1;
		if ( vl_Mot == NULL )
		{
			/* Ecriture trace Erreur l */
			XZST_03EcritureTrace( XZSTC_WARNING, "ANALYSE :  Impossible de lire le numero du bit du capteur du message Wizcon. " );
			return ( XDC_NOK );
		}

		/* Lecture numero d'alerte */
		vl_Mot=strtok(NULL,"|");
		XZST_03EcritureTrace( XZSTC_INFO, "ANALYSE :  Valeur numero alerte <%s>. ", vl_Mot);
		vl_numalerte=atoi(vl_Mot);
		if ( vl_Mot == NULL )
		{
			/* Ecriture trace Erreur l */
			XZST_03EcritureTrace( XZSTC_WARNING, "ANALYSE :  Impossible de lire le numero d'alerte du message Wizcon. " );
			return ( XDC_NOK );
		}

		/* Lecture Type capteur */
		vl_Mot=strtok(NULL,"|");
		XZST_03EcritureTrace( XZSTC_INFO, "ANALYSE :  Valeur etat capteur <%s>. ", vl_Mot);
		vl_etatcapteur=atoi(vl_Mot);
		if ( vl_Mot == NULL )
		{
			/* Ecriture trace Erreur l */
			XZST_03EcritureTrace( XZSTC_WARNING, "ANALYSE :  Impossible de lire l'etat du capteur du message Wizcon. " );
			return ( XDC_NOK );
		}
		/*Recherche du numero de capteur avec les infos */
		for ( pl_Capt = pl_Tube->ListeCapteur; pl_Capt != NULL; pl_Capt = pl_Capt->Suivant )
		{
			XZST_03EcritureTrace(XZSTC_INFO, "ANALYSE : Capteur <%d> Type <%d> Etat <%d> Service <%d> Num bit <%d>.",pl_Capt->NumeroCapteur,
			pl_Capt->Type,pl_Capt->EtatCapteur,pl_Capt->Etat_Service,pl_Capt->NumBit);
			if ( (pl_Capt->NumBit == vl_bitnumcapteur) && (pl_Capt->Type == vl_typecapteur) )
			{
				vl_numcapteur=pl_Capt->NumeroCapteur;	
				XZST_03EcritureTrace( XZSTC_INFO, "ANALYSE :  Correspondance capteur trouve <%d>. ",vl_numcapteur );
			}
		}
		break;

		 /*A C'est un DISCORDANCE PANNEAU */
         case ENIW_REC_DISCPAN :
		 /*A C'est un PANNEAU */
         case ENIW_REC_PANNEAU :		 

		XZST_03EcritureTrace( XZSTC_INFO, "ANALYSE : Analyse detaillee du type d'evenement <%d> : DISCPAN, ou ETAT PAN", vl_TypeEvt);
		/* Lecture numero de panneau */
		vl_Mot=strtok(NULL,"|");
		XZST_03EcritureTrace( XZSTC_INFO, "ANALYSE :  Valeur numero panneau <%s>. ", vl_Mot);
		vl_numpanneau=atoi(vl_Mot);
		if ( vl_Mot == NULL )
		{
			/* Ecriture trace Erreur l */
			XZST_03EcritureTrace( XZSTC_WARNING, "ANALYSE :  Impossible de lire le numero de panneau du message Wizcon. " );
			return ( XDC_NOK );
		}
		
		/* Lecture etat de panneau */
		vl_Mot=strtok(NULL,"|");
		XZST_03EcritureTrace( XZSTC_INFO, "ANALYSE :  Valeur etat panneau <%s>. ", vl_Mot);
		vl_etatpanneau=atoi(vl_Mot);
		if ( vl_Mot == NULL )
		{
			/* Ecriture trace Erreur l */
			XZST_03EcritureTrace( XZSTC_WARNING, "ANALYSE :  Impossible de lire l'etat du panneau du message Wizcon. " );
			return ( XDC_NOK );
		}

		/* Lecture discordance de panneau */
		vl_Mot=strtok(NULL,"|");
		XZST_03EcritureTrace( XZSTC_INFO, "ANALYSE :  Valeur discordance panneau <%s>. ", vl_Mot);
		vl_etatdisc=atoi(vl_Mot);
		if ( vl_Mot == NULL )
		{
			/* Ecriture trace Erreur l */
			XZST_03EcritureTrace( XZSTC_WARNING, "ANALYSE :  Impossible de lire l'etat de discordance panneau du message Wizcon. " );
			return ( XDC_NOK );
		}
		break;

		 /*A C'est un DISCORDANCE SEQUENCE */
         case ENIW_REC_DISCSEQ :
		/* Lecture etat de panneau */
		XZST_03EcritureTrace( XZSTC_INFO, "ANALYSE : Analyse detaillee du type d'evenement <%d> : DISC SEQ", vl_TypeEvt);
		vl_Mot=strtok(NULL,"|");
		XZST_03EcritureTrace( XZSTC_INFO, "ANALYSE :  Valeur numero sequence panneau <%s>. ", vl_Mot);
		vl_numsequence=atoi(vl_Mot);
		if ( vl_Mot == NULL )
		{
			/* Ecriture trace Erreur l */
			XZST_03EcritureTrace( XZSTC_WARNING, "ANALYSE :  Impossible de lire le numero de sequence du message Wizcon. " );
			return ( XDC_NOK );
		}

		/* Lecture discordance de panneau */
		vl_Mot=strtok(NULL,"|");
		XZST_03EcritureTrace( XZSTC_INFO, "ANALYSE :  Valeur discordance sequence <%s>. ", vl_Mot);
		vl_etatdisc=atoi(vl_Mot);
		if ( vl_Mot == NULL )
		{
			/* Ecriture trace Erreur l */
			XZST_03EcritureTrace( XZSTC_WARNING, "ANALYSE :  Impossible de lire l'etat de discordance sequence du message Wizcon. " );
			return ( XDC_NOK );
		}
		break;

	}

    /*A Recherche du type d'evenement */
    XZST_03EcritureTrace( XZSTC_INFO, " Traitement du type d'evenement");
    switch( vl_TypeEvt )
    {
         /*A C'est une Telealarame */
         case ENIW_REC_TELEALARME :
               
			XZST_03EcritureTrace( XZSTC_INFO, "TELEALARME : Debut");
			/* Memorisation etat si hors service et on sort de suite */
			if ( vl_TypeEqt == XDC_EQT_TUBE )
			{
				   if ( pl_Tube->Etat_Service != 0 )
				   {
					   pl_Tube->EtatEnergie = vl_energie;
					   pl_Tube->EtatLocal = vl_local;
					   pl_Tube->EtatLampe = vl_lampe;
					   break;
				   }
			}
			else
			{

				   if ( pl_Ech->Etat_Service != 0 )
				   {
					   pl_Ech->EtatEnergie = vl_energie;
					   pl_Ech->EtatLocal = vl_local;
					   pl_Ech->EtatLampe = vl_lampe;
					   break;
				   }
			}

			/*A Envoie Alerte ou alarme selon type de telealarme */
			/*A Si alarme energie */
			if  (((vl_TypeEqt == XDC_EQT_TUBE) && (pl_Tube->EtatEnergie != vl_energie)) ||
				((vl_TypeEqt == XDC_EQT_ECH) && (pl_Ech->EtatEnergie != vl_energie)))
			{
			   /* Envoie Alarme Energie Tube */
				XZST_03EcritureTrace( XZSTC_WARNING, "TELEALARME : Envoi alerte energie : TypeEqt = %d, NumEqt = %d, Etat <%d>",
					vl_TypeEqt, 
					vl_NumEqt, 
					vl_energie );
  				ex_env_alarme (	vg_NomMachine,
				vl_Horodate,
				vl_TypeEqt,
				(vl_TypeEqt == XDC_EQT_TUBE) ? pl_Tube->Numero : pl_Ech->NumeroDemiEch,
				XZAMC_ENERGIE,
				vl_energie,
				NULL,
				XZSC_22NumeroSiteLocal());
			}
                  
            /*A Si alerte signalisation en local */ 
			if  (((vl_TypeEqt == XDC_EQT_TUBE) && (pl_Tube->EtatLocal != vl_local)) ||
				((vl_TypeEqt == XDC_EQT_ECH) && (pl_Ech->EtatLocal != vl_local)))
			{
               /* Envoie Alerte Tube en local */
				XZST_03EcritureTrace( XZSTC_WARNING, "TELEALARME : Envoi alerte local : TypeEqt = %d, NumEqt = %d, Etat <%d>",
					vl_TypeEqt, 
					vl_NumEqt, 
					vl_local );
               ex_env_alerte  ( vl_Horodate,
                                vl_TypeEqt,
                                vl_NumEqt,
                                XZAAC_SIG_LOCAL,
                                vg_Mode_Fonct,
				XZSC_22NumeroSiteLocal(),
                                NULL ); 
            }                

            /*A Si alarme defaut lampe */ 
			if  (((vl_TypeEqt == XDC_EQT_TUBE) && (pl_Tube->EtatLampe != vl_lampe)) ||
				((vl_TypeEqt == XDC_EQT_ECH) && (pl_Ech->EtatLampe != vl_lampe)))
			{
				XZST_03EcritureTrace( XZSTC_WARNING, "TELEALARME : Envoi alerte lampe : TypeEqt = %d, NumEqt = %d, Etat <%d>",
					vl_TypeEqt, 
					vl_NumEqt, 
					vl_lampe );
               /* Envoie Alarme lampe */
  				ex_env_alarme (	vg_NomMachine,
				vl_Horodate,
				vl_TypeEqt,
				(vl_TypeEqt == XDC_EQT_TUBE) ? pl_Tube->Numero : pl_Ech->NumeroDemiEch,
				XZAMC_LAMPE,
				vl_local,
				NULL,
				XZSC_22NumeroSiteLocal());
            }                
            
            /* Envoi etat tube dans XDM_ETAT_TELEALARME sur XDG_ETAT_TELEALARME */
			sprintf ( pl_NomDG, "%s_%s", XDG_ETAT_TELEALARME, pl_NomSite );
            if (!TipcSrvMsgWrite ( 	pl_NomDG,
      	      	      	            	XDM_IdentMsg(XDM_ETAT_TELEALARME),
                          	    	XDM_FLG_SRVMSGWRITE,
                          	    	T_IPC_FT_INT4, vl_TypeEqt,
                          	    	T_IPC_FT_INT4, vl_NumEqt,
                          	    	T_IPC_FT_INT4, vl_energie,
                          	    	T_IPC_FT_INT4, vl_local,
                          	    	T_IPC_FT_INT4, vl_lampe,
                          	    	NULL ) )
            {
                XZST_03EcritureTrace(XZSTC_WARNING, "TELEALARME : Envoi message XDM_ETAT_TELEALARME non effectue." );
            }
			else
			{
				XZST_03EcritureTrace( XZSTC_INFO, "TELEALARME : Envoi message XDM_ETAT_TELEALARME <NomDG>%s_<TypRqt>%d_<NumEqt>%d_<Energie>%d_<Local>%d_<Lampe>%d",pl_NomDG,vl_TypeEqt,vl_NumEqt,vl_energie,vl_local,vl_lampe);
			}

			/* Memorisation etat des telealarmes */
			if ( vl_TypeEqt == XDC_EQT_TUBE )
			{
				   pl_Tube->EtatEnergie = vl_energie;
				   pl_Tube->EtatLocal = vl_local;
				   pl_Tube->EtatLampe = vl_lampe;
			}
			else
			{
				   pl_Ech->EtatEnergie = vl_energie;
				   pl_Ech->EtatLocal = vl_local;
				   pl_Ech->EtatLampe = vl_lampe;
			}	
	XZST_03EcritureTrace( XZSTC_INFO, "TELEALARME : Fin");

            break;
            
         /*A C'est une ETOR */
         case ENIW_REC_ETOR :

			XZST_03EcritureTrace( XZSTC_INFO, "ETOR : Debut");
			 /* Envoi alerte Capteur */
			 if ( (vl_etatcapteur == 1) &&
				  (pl_Tube->Etat_Service ==0) ) 
			 {
				if ( vl_typecapteur == XDC_EQT_EXT )
				{
					XZST_03EcritureTrace( XZSTC_INFO, "ETOR : Envoi alerte extincteur NumTube <%d>, Num Capteur <%d>, Etat capteur <%d> , numero alerte <%d> ",pl_Tube->Numero,vl_numcapteur,vl_etatcapteur,vl_numalerte);
					ex_env_alerte  ( vl_Horodate,
									 vl_typecapteur,
									 vl_numcapteur,
									 vl_numalerte,
									 vg_Mode_Fonct,
									 XZSC_22NumeroSiteLocal(),
									 NULL );
				}

				if ( vl_typecapteur == XDC_EQT_LIT )
				{
					XZST_03EcritureTrace( XZSTC_INFO, "ETOR : Envoi alerte lit d'arret NumTube <%d>, Num Capteur <%d>, Etat capteur <%d> , numero alerte <%d> ",pl_Tube->Numero,vl_numcapteur,vl_etatcapteur,vl_numalerte);
					ex_env_alerte  ( vl_Horodate,
									 vl_typecapteur,
									 vl_numcapteur,
									 vl_numalerte,
									 vg_Mode_Fonct,
									 XZSC_22NumeroSiteLocal(),
									 NULL );
				}
			 }
			 /* Envoi etat Capteur dans XDM_ETAT_CAPTEUR sur XDG_ETAT_CAPTEUR */
			 if ( pl_Tube->Etat_Service == 0) 
			 {
			sprintf ( pl_NomDG, "%s_%s", XDG_ETAT_CAPTEUR, pl_NomSite );
				if (!TipcSrvMsgWrite ( 	pl_NomDG,
      	      	      					XDM_IdentMsg(XDM_ETAT_CAPTEUR),
                          			XDM_FLG_SRVMSGWRITE,
                          			T_IPC_FT_INT4, pl_Tube->Numero,
                          			T_IPC_FT_INT4, vl_numcapteur,
                          			T_IPC_FT_INT4, vl_etatcapteur,
                          			NULL ) )
				{
					XZST_03EcritureTrace(XZSTC_WARNING, "ETOR : Envoi message XDM_ETAT_CAPTEUR non effectue." );
				}
				else
				{
					XZST_03EcritureTrace( XZSTC_INFO, "ETOR : Envoi message XDM_ETAT_CAPTEUR <NomDG>%s_<NumTub>%d_<NumCapt>%d_<EtatCapt>%d",pl_NomDG,pl_Tube->Numero,vl_numcapteur,vl_etatcapteur);
				}

			 }

		XZST_03EcritureTrace( XZSTC_INFO, "ETOR : Fin");
            break;
               
         /*A C'est une discordance panneau  */
         case ENIW_REC_DISCPAN :
			XZST_03EcritureTrace( XZSTC_INFO, "DISCPAN : Debut");
			/* Si discordance panneau */
			/* Recherche panneau concernee */
			for ( pl_Panneau = (vl_TypeEqt == XDC_EQT_TUBE) ? pl_Tube->ListePanneau : pl_Ech->ListePanneau;
				 pl_Panneau != NULL; 
				 pl_Panneau = pl_Panneau->Suivant )
			{
				XZST_03EcritureTrace(XZSTC_INFO, "DISCPAN : Panneau <%d> Etat <%d> Service <%d>",pl_Panneau->NumeroPanneau,pl_Panneau->EtatPanneau,pl_Panneau->Etat_Service);
				if (vl_numpanneau == pl_Panneau->NumeroPanneau)
				{
					XZST_03EcritureTrace(XZSTC_INFO, "DISCPAN : Panneau trouve <%d> Etat <%d> Service <%d> Discordance <%>",pl_Panneau->NumeroPanneau,pl_Panneau->EtatPanneau,pl_Panneau->Etat_Service,pl_Panneau->EtatDisc);
					/* Memorisation Etat panneau */
					pl_Panneau->EtatDisc = vl_etatdisc;
					pl_Panneau->EtatPanneau = vl_etatpanneau;

					 /* Envoyer Alarme discordance panneau */
					 if ( vl_Etat_Service==0 )
					 {
						 XZST_03EcritureTrace(XZSTC_INFO, "DISCPAN : Envoi alarme Etat Panneau <%d> Etat <%d> Service <%d> Etat disc <%d>",pl_Panneau->NumeroPanneau,pl_Panneau->EtatPanneau,pl_Panneau->Etat_Service,vl_etatdisc);
						ex_env_alarme (	vg_NomMachine,
							vl_Horodate,
							XDC_EQT_PAN,
							pl_Panneau->NumeroPanneau,
							XZAMC_DISC_PANNEAU,
							vl_etatdisc,
							NULL,
							XZSC_22NumeroSiteLocal());
					}

					 /* Envoi etat Panneau dans XDM_ETAT_PANNEAU sur XDG_ETAT_PANNEAU */
					 sprintf ( pl_NomDG, "%s_%s", XDG_ETAT_PANNEAU, pl_NomSite );
					 if ( vl_Etat_Service == 0 )
						if (!TipcSrvMsgWrite ( 	pl_NomDG,
      	      	      							XDM_IdentMsg(XDM_ETAT_PANNEAU),
                          					XDM_FLG_SRVMSGWRITE,
                          					T_IPC_FT_INT4, vl_TypeEqt,
                          					T_IPC_FT_INT4, vl_NumEqt,
                          					T_IPC_FT_INT4, pl_Panneau->NumeroPanneau,
                           					T_IPC_FT_INT4, pl_Panneau->EtatPanneau,
                           					T_IPC_FT_INT4, pl_Panneau->EtatDisc,
                          					NULL ) )
						{
						   XZST_03EcritureTrace(XZSTC_WARNING, "DISCPAN : Envoi message XDM_ETAT_PANNEAU non effectue." );
						}
						else
						{
							XZST_03EcritureTrace( XZSTC_INFO, "DISCPAN : Envoi message XDM_ETAT_PANNEAU <NomDG>%s_<TypeEqt>%d_<NumEqt>%d_<NumPan>%d_<EtatPan>%d_<EtatDisc>%d",pl_NomDG,vl_TypeEqt,vl_NumEqt,pl_Panneau->NumeroPanneau,pl_Panneau->EtatPanneau,pl_Panneau->EtatDisc);
						}
				}
			}
			XZST_03EcritureTrace( XZSTC_INFO, "DISCPAN : Fin");
			break;
 
         /*A C'est une discordance sequence  */
         case ENIW_REC_DISCSEQ :            

			XZST_03EcritureTrace( XZSTC_INFO, "DISCSEQ : Debut");
			/* Recherche si sequence concernee */
			for ( pl_Sequence = (vl_TypeEqt == XDC_EQT_TUBE) ? pl_Tube->ListeSequence : pl_Ech->ListeSequence;
				 pl_Sequence != NULL; 
				 pl_Sequence = pl_Sequence->Suivant )
			{
				XZST_03EcritureTrace(XZSTC_INFO, "DISCSEQ : Sequence <%d> Disc <%d> ",pl_Sequence->NumSequence,pl_Sequence->EtatDisc);
				if (pl_Sequence->NumSequence == vl_numsequence)
				{
					XZST_03EcritureTrace(XZSTC_INFO, "DISCSEQ : Sequence trouvee <%d> Disc <%d> ",pl_Sequence->NumSequence,pl_Sequence->EtatDisc);

					/* Memorisation etat sequence */
					pl_Sequence->EtatDisc = vl_etatdisc;
					/* Envoyer compte rendu action a BASEDD */
					if ( (vl_TypeEqt == XDC_EQT_TUBE) && (pl_Tube->Etat_Service==0) )
					{
						if ( (pl_Sequence->EtatDisc != 0) && (pl_Sequence->NumAction > 0) )
						{
                           				/* Appel de XZAT22 avec XDC_OK et NumAction */ /* DEM609 12/01/07 : Envoi Status OK en cas de discordance */

	                   				xzat22_CompteRendu_Tube ( pl_Sequence->NumAction, vl_Horodate, XDC_OK );

							XZST_03EcritureTrace(XZSTC_INFO, "DISCSEQ : Envoi XZAT22 pour Tube <%d> Sequence <%d> Numaction <%d> Disc <%d> ",pl_Tube->Numero,pl_Sequence->NumSequence,pl_Sequence->NumAction,pl_Sequence->EtatDisc);
						}

						if ( (pl_Sequence->EtatDisc != 0) && (pl_Sequence->NumAction <= 0) )
						{
						XZST_03EcritureTrace(XZSTC_INFO, "DISCSEQ : Envoi alerte Tube <%d> Sequence <%d> Numaction <%d> Disc <%d> ",pl_Tube->Numero,pl_Sequence->NumSequence,pl_Sequence->NumAction,pl_Sequence->EtatDisc);
						   /* Envoyer alerte discordance sequence */
						   ex_env_alerte  ( 	vl_Horodate,
											XDC_EQT_TUB,
											pl_Tube->Numero,
											XZAAC_AFF_DIF,
											vg_Mode_Fonct,
											XZSC_22NumeroSiteLocal(),
											pl_Sequence->NumSequence,
											NULL );
						/* DEM609 12/01/07 : Envoi Status OK en cas de discordance a l'ouverture du tunnel*/
						xzat22_CompteRendu_Tube ( pl_Sequence->NumAction, vl_Horodate, XDC_OK );
						}

					}

					if ( (vl_TypeEqt == XDC_EQT_ECH) && (pl_Ech->Etat_Service==0) )
					{
						/* Envoyer compte rendu action a BASEDD */
						if ( (pl_Sequence->EtatDisc != 0) && (pl_Sequence->NumAction > 0) )
						{
							/* Appel de XZAT23 avec XDC_NOK et NumAction */
							xzat23_CompteRendu_DemiEch ( pl_Sequence->NumAction, vl_Horodate, XDC_NOK );
							XZST_03EcritureTrace(XZSTC_INFO, "DISCSEQ : Envoi XZAT23 pour Echangeur <%d> Sequence <%d> Numaction <%d> Disc <%d> ",pl_Ech->NumeroDemiEch,pl_Sequence->NumSequence,pl_Sequence->NumAction,pl_Sequence->EtatDisc);
						}

						if ( (pl_Sequence->EtatDisc != 0) && (pl_Sequence->NumAction <= 0) )
						{
							/* Envoyer alerte discordance sequence */
							ex_env_alerte  ( 	vl_Horodate,
											XDC_EQT_ECH,
											pl_Ech->NumeroDemiEch,
											XZAAC_AFF_DIF,
											vg_Mode_Fonct,
											pl_Sequence->NumSequence,
											NULL );

						}
					}

					/* Envoyer Alarme discordance sequence */
					sprintf ( vl_TexteAlarme, "%d", pl_Sequence->NumSequence );
					if ( vl_Etat_Service==0 )
					{
						XZST_03EcritureTrace(XZSTC_INFO, "DISCSEQ : Envoi alerte Discordance Sequence <%d> Type Eqt <%d> Num Eqt <%d> Etat disc <%d> texte <%s> ",
							pl_Sequence->NumSequence,vl_TypeEqt,vl_NumEqt,vl_etatdisc,vl_TexteAlarme);
						ex_env_alarme (	vg_NomMachine,
							vl_Horodate,
							vl_TypeEqt,
							vl_NumEqt,
							XZAMC_DISC_SEQUENCE,
							vl_etatdisc,
							vl_TexteAlarme ,
							XZSC_22NumeroSiteLocal());
					}

					/* Envoi etat Sequence dans XDM_ETAT_SEQUENCE sur XDG_ETAT_SEQUENCE */
					sprintf ( pl_NomDG, "%s_%s", (vl_TypeEqt == XDC_EQT_TUBE) ? XDG_ETAT_SEQ_TUBE : XDG_ETAT_SEQ_ECH, pl_NomSite );
					if ( (vl_Etat_Service == 0) )
						if (!TipcSrvMsgWrite ( 	pl_NomDG,
									XDM_IdentMsg((vl_TypeEqt == XDC_EQT_TUBE) ? XDM_ETAT_SEQUENCE_TUBE : XDM_ETAT_SEQUENCE_ECH),
									XDM_FLG_SRVMSGWRITE,
									T_IPC_FT_INT4, vl_TypeEqt,
									T_IPC_FT_INT4, vl_NumEqt,
									T_IPC_FT_INT4, (vl_TypeEqt == XDC_EQT_TUBE) ? pl_Tube->DerniereSequence : pl_Ech->DerniereSequence,
									T_IPC_FT_INT4, pl_Sequence->NumSequence,
									T_IPC_FT_INT4, vl_etatdisc,
									NULL ) )
					{
						XZST_03EcritureTrace(XZSTC_WARNING, "DISCSEQ : Envoi message XDM_ETAT_SEQUENCE non effectue." );
					}
					else
					{
								XZST_03EcritureTrace( XZSTC_INFO, "DISCSEQ : Envoi message XDM_ETAT_SEQUENCE <NomDG>%s_<TypEqt>%d_<NumEqt>%d_<DernSeq>%d_<NumSeq>%d_<EtatDisc>%d",pl_NomDG,vl_TypeEqt,vl_NumEqt,(vl_TypeEqt == XDC_EQT_TUBE) ? pl_Tube->DerniereSequence : pl_Ech->DerniereSequence,pl_Sequence->NumSequence,vl_etatdisc);
					}
				} 
			}
			break;
                     

		case ENIW_REC_CO :      
            /*A Si evenement seuil CO ou opacite */
				XZST_03EcritureTrace( XZSTC_INFO, "CO : Debut");
				/*A Recherche capteur concernee */
				for ( pl_Capt = pl_Tube->ListeCapteur; pl_Capt != NULL; pl_Capt = pl_Capt->Suivant )
				{
				XZST_03EcritureTrace(XZSTC_INFO, "CO : Capteur : <%d> Type <%d> Etat <%d> Service <%d> Num bit <%d>.",pl_Capt->NumeroCapteur,
           			pl_Capt->Type,pl_Capt->EtatCapteur,pl_Capt->Etat_Service,pl_Capt->NumBit);
				 if ( (pl_Capt->NumeroCapteur == vl_numcapteur) && ((pl_Capt->Type==XDC_EQT_CCO)||(pl_Capt->Type==XDC_EQT_OPA)) )
				 {
					 XZST_03EcritureTrace(XZSTC_INFO, "CO : Capteur trouve, Etat service du Tube : <%d> Etat <%d>.",pl_Tube->Numero, pl_Tube->Etat_Service);
					/*A Memorisation etat Capteur */
					pl_Capt->EtatCapteur = vl_etatcapteur;
					pl_Capt->NumAlerte = vl_numalerte;

					/*A Envoi alerte Capteur */
					if ( (pl_Capt->EtatCapteur == 1) &&
						 (vl_Etat_Service==0) )
					{
					XZST_03EcritureTrace(XZSTC_INFO, "CO : Envoi alerte Type <%d> num <%d> Num alerte .",pl_Capt->Type,pl_Capt->NumeroCapteur,pl_Capt->NumAlerte);
					   ex_env_alerte  ( vl_Horodate,
									 pl_Capt->Type,
									 pl_Capt->NumeroCapteur,
									 pl_Capt->NumAlerte,
									 vg_Mode_Fonct,
									 XZSC_22NumeroSiteLocal(),
									 NULL );
					}
				 }
				}
		XZST_03EcritureTrace( XZSTC_INFO, "CO : Fin");
            break;

         /*A C'est une Alerte */
         case ENIW_REC_ALERTE :
	XZST_03EcritureTrace( XZSTC_INFO, "ALERTE : Debut");
            /* Recherche capteur concernee */
           for ( pl_Capt = pl_Tube->ListeCapteur; pl_Capt != NULL; pl_Capt = pl_Capt->Suivant )
           {
           			XZST_03EcritureTrace(XZSTC_INFO, "ALERTE : Capteur : <%d> Type <%d> Etat <%d> Service <%d> Num bit <%d>.",pl_Capt->NumeroCapteur,
           			pl_Capt->Type,pl_Capt->EtatCapteur,pl_Capt->Etat_Service,pl_Capt->NumBit);
           	
              if ( ((pl_Capt->Type == XDC_EQT_GAB) ||
                   (pl_Capt->Type == XDC_EQT_VIT) || 
                   (pl_Capt->Type == XDC_EQT_NICHE))  && (pl_Capt->NumeroCapteur == vl_numcapteur))
              {
			          	XZST_03EcritureTrace(XZSTC_INFO, "ALERTE : Capteur trouve, Etat service du Tube : <%d> Etat <%d>.",pl_Tube->Numero, pl_Tube->Etat_Service);

					/* Memorisation etat Capteur */
					pl_Capt->EtatCapteur = vl_etatcapteur;
					pl_Capt->NumAlerte = vl_numalerte;

					/* Envoi alerte Capteur */
					if ( (pl_Capt->EtatCapteur == 1) && (pl_Tube->Etat_Service==0 )) 
					{
						XZST_03EcritureTrace(XZSTC_INFO, "ALERTE : Envoi alerte Type <%d> num <%d> Num alerte .",pl_Capt->Type,pl_Capt->NumeroCapteur,pl_Capt->NumAlerte);
						 ex_env_alerte  (    vl_Horodate,
										 pl_Capt->Type,
										 pl_Capt->NumeroCapteur,
										 pl_Capt->NumAlerte,
										 vg_Mode_Fonct,
										 NULL );
					}
					/* Envoi etat Capteur dans XDM_ETAT_CAPTEUR sur XDG_ETAT_CAPTEUR */
					if ( (pl_Tube->Etat_Service == 0) )
					{
						sprintf ( pl_NomDG, "%s_%s", XDG_ETAT_CAPTEUR, pl_NomSite );
						 if (!TipcSrvMsgWrite ( 	pl_NomDG,
      	      	      								XDM_IdentMsg(XDM_ETAT_CAPTEUR),
												XDM_FLG_SRVMSGWRITE,
												T_IPC_FT_INT4, pl_Tube->Numero,
												T_IPC_FT_INT4, pl_Capt->NumeroCapteur,
												T_IPC_FT_INT4, pl_Capt->EtatCapteur,
												NULL ) )
						 {
							 XZST_03EcritureTrace(XZSTC_WARNING, "ALERTE : Envoi message XDM_ETAT_CAPTEUR non effectue." );
						 }
						 else
						 {
									XZST_03EcritureTrace( XZSTC_INFO, "ALERTE : Envoi message XDM_ETAT_CAPTEUR <NomDG>%s_<NumTub>%d_<NumCapt>%d_<EtatCapt>%d",pl_NomDG,pl_Tube->Numero,pl_Capt->NumeroCapteur,pl_Capt->EtatCapteur);
						 }
					}
               }
            }
	XZST_03EcritureTrace( XZSTC_INFO, "ALERTE : Fin");
            break;
            
         /*A C'est un changement d'etat d'un panneau */
         case ENIW_REC_PANNEAU :
 		   			XZST_03EcritureTrace( XZSTC_INFO, "ETAT PANNEAU : Debut");
			/* Recherche pour tout les panneaux si il y a un evenement */
			for ( pl_Panneau = (vl_TypeEqt == XDC_EQT_TUBE) ? pl_Tube->ListePanneau : pl_Ech->ListePanneau;
				  pl_Panneau != NULL; 
				  pl_Panneau = pl_Panneau->Suivant )
			{ 
				XZST_03EcritureTrace(XZSTC_INFO, "ETAT PANNEAU : Panneau <%d> Etat <%d> Service <%d>",pl_Panneau->NumeroPanneau,pl_Panneau->EtatPanneau,pl_Panneau->Etat_Service);				
   
			   if ( pl_Panneau->NumeroPanneau == vl_numpanneau)
			   {
					XZST_03EcritureTrace(XZSTC_INFO, "ETAT PANNEAU : Panneau trouve <%d> Etat <%d> Service <%d>",pl_Panneau->NumeroPanneau,pl_Panneau->EtatPanneau,pl_Panneau->Etat_Service);				
					/* Calcul de l'etat du panneau */
					pl_Panneau->EtatPanneau = vl_etatpanneau;
					pl_Panneau->EtatDisc = vl_etatdisc;
      
					/* Envoi etat Panneau dans XDM_ETAT_PANNEAU sur XDG_ETAT_PANNEAU */
					sprintf ( pl_NomDG, "%s_%s", XDG_ETAT_PANNEAU, pl_NomSite );
					if ( vl_Etat_Service == 0 )
					 if (!TipcSrvMsgWrite ( 	pl_NomDG,
      	      	      							XDM_IdentMsg(XDM_ETAT_PANNEAU),
											XDM_FLG_SRVMSGWRITE,
											T_IPC_FT_INT4, vl_TypeEqt,
											T_IPC_FT_INT4, vl_NumEqt,
											T_IPC_FT_INT4, pl_Panneau->NumeroPanneau,
											T_IPC_FT_INT4, pl_Panneau->EtatPanneau,
											T_IPC_FT_INT4, pl_Panneau->EtatDisc,
											NULL ) )
					 {
						XZST_03EcritureTrace(XZSTC_WARNING, "ETAT PANNEAU : Envoi message XDM_ETAT_PANNEAU non effectue." );
					 }
					 else
					 {
						XZST_03EcritureTrace( XZSTC_INFO, "ETAT PANNEAU : Envoi message XDM_ETAT_PANNEAU <NomDG>%s_<TypEqt>%d_<NumEqt>%d_<NumPan>%d_<EtatPan>%d_<EtatDisc>%d",pl_NomDG,vl_TypeEqt,vl_NumEqt,pl_Panneau->NumeroPanneau,pl_Panneau->EtatPanneau,pl_Panneau->EtatDisc);
					 }
     
			   }
			}
			XZST_03EcritureTrace( XZSTC_INFO, "ETAT PANNEAU : Fin");
			break;

   }/* Fin du switch evt*/

}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Traitement des receptions de fin de time-out discordance sequence
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int et_traitement_timers ( )

/*
* ARGUMENTS EN ENTREE :
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
*
* CONDITION D'UTILISATION
* execute sur reception de message socket
*
* FONCTION 
*
*  
------------------------------------------------------*/
{
   int				vl_IndCnx = 0,
   				vl_LgMsg  = 0;
   XDY_Horodate			vl_Horodate;   
   XZSMT_Horodate          	pl_StrHorodate;
   XZEXT_MSG_SOCKET		pl_Msg;
   XDY_Mot			vl_NumSeq, vl_TypeEqt, vl_NumEqt;
   ENIW_DONNEES_Sequence *	pl_Sequence;
   ENIW_DONNEES_Tube *		pl_Tube;
   ENIW_DONNEES_Echangeur *	pl_Ech;
   int				vl_Delai;
   
   /*A
    *  Lecture du message recu dans la socket appelante
    */
	 
   if ( ex_lire_msg ( vg_SockTETIM, pl_Msg, &vl_LgMsg, &vg_SockMask, &vl_IndCnx ) != XDC_OK )
   {
      /* Ecriture trace Erreur lecture message socket. */
      XZST_03EcritureTrace( XZSTC_WARNING, " Erreur lecture message socket timer. " );
      return ( XDC_NOK );
   }

   /* Si le message est correct et si ce n'est pas une deconnexion */
   if ( (vl_LgMsg == 0) || (vl_IndCnx == XZEXC_DECONNEC) || (strstr(pl_Msg, XZEXC_MSG_REVEIL) == NULL) )
   {
      /* Ecriture trace message socket timer incorrect. */
      XZST_03EcritureTrace( XZSTC_WARNING, " Erreur lecture message socket TETIM incorrect. " );
      return ( XDC_NOK );
   }
   
   sscanf ( &pl_Msg[strlen(XZEXC_MSG_REVEIL)], " %lf %d Nice%hd_%hd_%hd ",
  	  					&vl_Horodate,
  	  					&vl_Delai,
  	     					&vl_TypeEqt,
  	     					&vl_NumEqt,
						&vl_NumSeq );
  
   if ( vl_Delai == ENICC_REVEIL_15MN )
   {

         /* Construction du message de demande de relecture */
         sprintf ( pl_Msg, "%d|1|%s", ENIW_CMD_DEMANDE_INIT, XZEXC_FIN_MSG_SOCK_WIZCON );
         /* Envoie de la demande d'init */
		 sem_wait(&vg_semaphore); 
         if ( write ( vg_SockTWWI0, pl_Msg, strlen (pl_Msg) ) == XDC_NOK )
         { 
            /* Ecriture trace impossible d'emettre une demande d'arret */
             XZST_03EcritureTrace(XZSTC_WARNING, " Impossible d'emettre une demande d'init vers Wizcon principal." );
         }
		 sem_post(&vg_semaphore); 
         sprintf ( pl_Msg, "%d|1|%s", ENIW_CMD_DEMANDE_INIT, XZEXC_FIN_MSG_SOCK_WIZCON );
		 sem_wait(&vg_semaphore); 
         if ( write ( vg_SockTWWI1, pl_Msg, strlen (pl_Msg) ) == XDC_NOK )
         { 
            /* Ecriture trace impossible d'emettre une demande d'arret */
             XZST_03EcritureTrace(XZSTC_WARNING, " Impossible d'emettre une demande d'init vers Wizcon secondaire." );
         }
		 sem_post(&vg_semaphore); 
         return (XDC_OK);

   
   }  
  	  					         
   if ( vl_TypeEqt == XDC_EQT_TUBE )
   {
      /*A Recherche dans la liste des tubes */
      for ( pl_Tube = pg_debutTube; pl_Tube != NULL; pl_Tube = pl_Tube->Suivant )
      {
         if ( pl_Tube->Numero ==  vl_NumEqt )
         {
             /* Recherche de la sequence concernee */
             for ( pl_Sequence = pl_Tube->ListeSequence; pl_Sequence != NULL; pl_Sequence = pl_Sequence->Suivant )
             {
                if ( (pl_Sequence->NumSequence == vl_NumSeq) && (pl_Sequence->NumAction != 0) && (pl_Sequence->EtatDisc == 0) )
                {
                   /* Appel de XZAT22 avec XDC_OK et NumAction */
	           xzat22_CompteRendu_Tube ( pl_Sequence->NumAction, vl_Horodate, XDC_OK );
                   XZSM_11ConversionHorodate ( vl_Horodate, &pl_StrHorodate );
                   XZST_03EcritureTrace(XZSTC_INFO, "Envoi CR OK pour Tube:%d Cmd:%d Seq:%d H:%s.",
                   		 pl_Tube->Numero,
                   		 pl_Sequence->NumAction,
                   		 pl_Sequence->NumSequence,
                   		 pl_StrHorodate );
                   et_wfic_derniere_seq ( XDC_EQT_TUBE );
                   break;
                }
             }
          }
       }
    }
    else
    {
       /*A Recherche dans la liste des echangeurs */
       for ( pl_Ech = pg_debutEchangeur; pl_Ech != NULL; pl_Ech = pl_Ech->Suivant )
       {
          if ( pl_Ech->NumeroDemiEch ==  vl_NumEqt )
          {
             /* Recherche de la sequence concernee */
             for ( pl_Sequence = pl_Ech->ListeSequence; pl_Sequence != NULL; pl_Sequence = pl_Sequence->Suivant )
             {
                if ( (pl_Sequence->NumSequence == vl_NumSeq) && (pl_Sequence->NumAction != 0) && (pl_Sequence->EtatDisc == 0) )
                {
                   /* Appel de XZAT23 avec XDC_OK et NumAction */
	           xzat23_CompteRendu_DemiEch ( pl_Sequence->NumAction, vl_Horodate, XDC_OK );
                   XZSM_11ConversionHorodate ( vl_Horodate, &pl_StrHorodate );
                   XZST_03EcritureTrace(XZSTC_INFO, "Envoi CR OK pour Echangeur:%d Cmd:%d Seq:%d H:%s.",
                   		 pl_Ech->NumeroDemiEch,
                   		 pl_Sequence->NumAction,
                   		 pl_Sequence->NumSequence,
                   		 pl_StrHorodate );
                   et_wfic_derniere_seq ( XDC_EQT_ECH );
                   break;
                }
             }
          }
       }   
    }
    
    return ( XDC_OK );
	        
}






	   		
	   		
	   		
	   		
	   		
	   		
	   		
	   		
	   		
	   		
	   		
	   		
	   		
	   		
	   		
	   		
	   		
	   		
	   		
	   		
	   		
	   		
