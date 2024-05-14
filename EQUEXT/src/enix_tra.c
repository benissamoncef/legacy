/*E*/
/*Fichier :  $Id: enix_tra.c,v 1.11 2015/03/13 11:32:54 pc2dpdy Exp $      Release : $Revision: 1.11 $        Date : $Date: 2015/03/13 11:32:54 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR  
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TENIX * FICHIER enix_tra.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* c'est le module de traitement des sockets provenant de Wizcon maitre
* et Wizcon esclave pour la tache TENIX, gtc Nice
*
------------------------------------------------------
* HISTORIQUE :
*
* Ce module est une modification du module enix_tra.c
* AAZ       Version 1.1   17 Septembre 2007 : GTC r�nov�e DEM 664
* JBL		13 Oct 2008 : Suppression code inutilise (stockage interne alarmes et alertes)
* ADB		Mai 2009 : Mise a jour des traces
* ABE 		07-01-2010 : Partie traitement domaine incendie : le code �tait en commentaires, 
*						 code pass� en exploitation avec correction de domaine = 9 (?? auparavant)
* ADB       22 mars 2010 : FT 3448 - Gestion des etats liaisons
* ABE       26-03-2013 : gestion du retour � la normale avec ajout de sc�narios pour le domaine exploitation
* SFO       06-10-2014 : Adaptation pour compilation sous linux
* PNI	18/01/15 : Test du vl_Mot==NULL avant son utilisation 
------------------------------------------------------*/
 
/* fichiers inclus */
#include <xzsem.h>

#include "enix_tra.h"

/* definitions de constantes */
#define ENIX_Lon_Fin_Trame    5

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */
static char *version = "$Id: enix_tra.c,v 1.11 2015/03/13 11:32:54 pc2dpdy Exp $      : enix_tra" ;

#define TAILLE_MSG_COURANT		1024
char    vg_message_courant[TAILLE_MSG_COURANT];
int		vg_position_message_courant = 0;

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
* va_socket    : socket de lecture
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
    int                 vl_IndCnx    = 0,
                        vl_LgMsg     = 0,
                        vl_LgReelle  = 0;
    char *              pl_Message;
    char vl_mon_msg[200]="";
	char				vl_buffer_local[TAILLE_MSG_COURANT];
	int					i;
	
	
    /*A
    *  Lecture du message recu dans la socket appelante
    */

    XZST_03EcritureTrace( XZSTC_INFO, "IN : et_traitement");

	memset(vl_buffer_local, 0, TAILLE_MSG_COURANT);
	
	if (vg_position_message_courant == 0)
	{
		memset(vg_message_courant, 0, TAILLE_MSG_COURANT);
	}

#ifdef _HPUX_SOURCE
    fcntl ( va_socket, F_SETFL, O_NDELAY | fcntl ( va_socket, F_GETFL, 0 ) );
#else
      fcntl ( va_socket, F_SETFL, O_NONBLOCK | fcntl ( va_socket, F_GETFL, 0 ) );
#endif

	sem_wait(&vg_semaphore); 
    if ( (vl_LgMsg = recv ( va_socket, vl_buffer_local, TAILLE_MSG_COURANT-2, MSG_PEEK )) == -1 )                                   
    {
        sem_post(&vg_semaphore); 
        /*B Ecriture dans le fichier trace en cas d'chec */
        XZST_03EcritureTrace( XZSTC_WARNING, "TRAME : Erreur de lecture de message socket Wizcon.");
        return ( XDC_NOK );
    }
    sem_post(&vg_semaphore); 

    /*A Si il s'agit d'une deconnexion */
    if ( vl_LgMsg == 0 )
    {
        /*A Fermeture de la socket */
        sem_wait(&vg_semaphore); 
        read ( va_socket, vl_buffer_local, TAILLE_MSG_COURANT);
        sem_post(&vg_semaphore); 
        ex_fin_cnx ( va_socket, &vg_SockMask );
		/* Reinitialisation buffer */
		vg_position_message_courant = 0;
        /* Ecriture trace . */
        XZST_03EcritureTrace( XZSTC_WARNING, "TRAME :  Demande deconnexion ou message vide de la socket de Wizcon. " );
        return ( XDC_NOK );
    }
    sem_wait(&vg_semaphore); 
	read ( va_socket, vl_buffer_local, vl_LgMsg );
    sem_post(&vg_semaphore); 
	strcpy(vl_mon_msg,vl_buffer_local);

	XZST_03EcritureTrace( XZSTC_INFO, "vl_mon_msg : %s ",vl_mon_msg);
	/* decoupage de la trame */
	char *debut = vl_buffer_local;
	char *pos = strstr(debut, "&");
	while (pos != NULL)
	{
		strcpy(vg_message_courant+vg_position_message_courant, debut);
		XZST_03EcritureTrace( XZSTC_INFO, "TRAME : trame complete : %s ", vg_message_courant );
		et_analyse(vg_message_courant, va_socket);
		vg_position_message_courant = 0;

		/* passage a la trame suivante */
		debut = pos + 2;
		pos = strstr(debut, "&");
	}
	if (strlen(debut) > 0)
	{
		strcpy(vg_message_courant, debut);
		vg_position_message_courant = strlen(debut);
		XZST_03EcritureTrace( XZSTC_DEBUG1, "TRAME :  trame residuelle : %s ", vg_message_courant );
	}
	else
	{
		XZST_03EcritureTrace( XZSTC_DEBUG1, "TRAME :  pas de trame residuelle");
	}
	return XDC_OK;
}

/*------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int et_analyse (char * pa_Msg , int va_socket )

/*
* ARGUMENTS EN ENTREE :
*   
* va_socket    : socket de lecture
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
   int                         vl_IndCnx = 0,
                               vl_LgMsg  = 0;
   XDY_Horodate                vl_Horodate; 
   XDY_Texte                   vl_TexteAlarme;
   int                         vl_typetrame;
   int                         vl_Etat_Service = 0;
   char    *                   vl_Mot;
   struct tm *                 pl_InfoTemps ;
   struct tm                   pl_Temps ;
   time_t                      vl_Heure;
   char                        vl_fin_chaine[ENIX_Lon_Fin_Trame];
   short                       vl_Etat,
                               vl_Change[XZETC_MAX_UNITAIRE/XZETC_NB_ETAT_UNITAIRE];
   int                         vl_TypeAlarme,
							   vl_TypeAlerte,	
							   vl_IdAlarme,
							   vl_IdAlerte,
							   vl_IdEquipement,
							   vl_TypeEvt,
                               vl_TypeCmd,
                               vl_TypeOuvrage,
                               vl_NumOuvrage,
                               vl_Index,
                               vl_NumBit1,
                               vl_NumBit2,
                               vl_NumMot1,
                               vl_NumMot2,
                               vl_EtatEqt1,
                               vl_EtatEqt2,
                               vl_OP,
							   vl_ModeRafale,
                               vl_CO;
   int                         vl_JourSem;

   /* AAZ 18/07/2007 */
   ENIX_DONNEES_Equipement *   pl_Equipement;
   ENIX_DONNEES_Domaine *      pl_Domaine;
   ENIX_DONNEES_Sequence *     pl_Sequence;
   /* ENIX_DONNEES_Capteur *    pl_Capt; */

   ENIX_DONNEES_SIG  *         pl_SIG;
   ENIX_DONNEES_ECL  *         pl_ECL;
   ENIX_DONNEES_VEN  *         pl_VEN;
   ENIX_DONNEES_ENR  *         pl_ENR;
   ENIX_DONNEES_INC  *         pl_INC;
   ENIX_DONNEES_EXP  *         pl_EXP;
   ENIX_DONNEES_SYS  *         pl_SYS;
   ENIX_DONNEES_LTN  *         pl_LTN;

   div_t                       vl_div;
   XDY_Datagroup               pl_NomDG;
   XDY_NomSite                 pl_NomSite;
   short                       vl_energie,
                               vl_local,
                               vl_lampe,
                               vl_typecapteur,
                               vl_bitnumcapteur,
                               vl_numcapteur,
                               vl_numalerte,
                               vl_etatcapteur,
                               vl_numsequence,
                               vl_etatdisc,
                               vl_numpanneau,
                               vl_etatpanneau,
                               vl_domaine,
                               vl_domaine_socket,
                               vl_typeequipement,
                               vl_numequipement,
                               vl_etatequipement,
                               vl_etattechnique,
                               vl_mode,
							   vl_IdScenario,
                               vl_defaut,
                               vl_typequiment,
                               vl_numeroalerte,
							   vl_etatalarme,
                               vl_etatalerte;

    /* AAZ 03/09/2007 */
    XDY_Eqt                    vl_numero = 0;
    ENIX_DONNEES_Equipement *  vl_Equipement;
    ENIX_DONNEES_Sequence *    vl_Sequence;
    XDY_Mot                    vl_Etat_Sequence;
    XDY_Texte                  vl_Texte_Etat_Sequence;
    XDY_Mot                    vl_Derniere_Sequence;
	int						   vl_trouve = XDC_NOK;

	XZEXT_MSG_SOCKET	vl_message_sauve;

    vl_energie = 0;
    vl_local   = 0;
    vl_lampe   = 0;
    vl_mode    = 0;
    vl_defaut  = 0;
	vl_ModeRafale = 0;
    /* AAZ */
	pl_SIG = NULL;
	pl_ECL = NULL;
	pl_VEN = NULL; 
	pl_ENR = NULL;
	pl_INC = NULL;
	pl_EXP = NULL;
	pl_SYS = NULL;
	pl_LTN = NULL;
	
	
    XZST_03EcritureTrace( XZSTC_INFO, "IN : et_analyse");

    XZST_03EcritureTrace( XZSTC_INFO, "et_analyse pa_Msg : %s", pa_Msg);

	memcpy(vl_message_sauve, pa_Msg, sizeof(XZEXT_MSG_SOCKET));

    /* Pointeur sur une structure de temps existantes */
    pl_InfoTemps = &pl_Temps;

    vl_Mot       = strtok( pa_Msg, "|" );
    XZST_03EcritureTrace( XZSTC_INFO, "TRAME : Type de commande <%s>. ",
                          vl_Mot);

    /*A Scan du type de commande re�u provenant de la socket */
    if ( vl_Mot == NULL )
    {
        /* Ecriture trace Erreur l */
        XZST_03EcritureTrace( XZSTC_WARNING, "TRAME :  Impossible de lire la commande du message Wizcon");
        XZST_03EcritureTrace( XZSTC_WARNING, "TRAME :  La trame lue est <%s>", vl_message_sauve);
        return ( XDC_NOK );
    }

    vl_TypeCmd = atoi( vl_Mot );
    vl_TypeEvt = vl_TypeCmd; /* c'est pareil */

    /* La trame maitre est particuli�re, elle est courte*/
    if ( vl_TypeCmd == ENIX_REC_MAITRE )
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
        return( XDC_OK );
    }

    /* On lit le Type d'ouvrage */
    vl_Mot = strtok( NULL, "|" );
    XZST_03EcritureTrace( XZSTC_INFO, "TRAME : Valeur type ouvrage <%s>. ",
                          vl_Mot);

    if ( vl_Mot == NULL )
    {
        /* Ecriture trace Erreur l */
        XZST_03EcritureTrace( XZSTC_WARNING, "TRAME : Impossible de lire le type Ouvrage du message Wizcon. " );
        XZST_03EcritureTrace( XZSTC_WARNING, "TRAME :  La trame lue est <%s>", vl_message_sauve);
        return ( XDC_NOK );
    }
    vl_TypeOuvrage = atoi( vl_Mot );

    /* On lit le Numero d'ouvrage */
    vl_Mot     = strtok( NULL, "|" );
    XZST_03EcritureTrace( XZSTC_INFO, "TRAME : Valeur num Ouvrage <%s>. ",
                          vl_Mot);
                          
    if ( vl_Mot == NULL )
    {
        /* Ecriture trace Erreur l */
        XZST_03EcritureTrace( XZSTC_WARNING, "TRAME : Impossible de lire le numero Ouvrage du message Wizcon. " );
        XZST_03EcritureTrace( XZSTC_WARNING, "TRAME :  La trame lue est <%s>", vl_message_sauve);
        return ( XDC_NOK );
    }
    vl_NumOuvrage = atoi( vl_Mot );

    /* Lecture de l'horodate */ 
    vl_Mot=strtok( NULL, "~" );

    if ( vl_Mot == NULL )
    {
        /* Ecriture trace Erreur l */
	XZST_03EcritureTrace( XZSTC_WARNING, "TRAME : Impossible de lire la date. " );
	XZST_03EcritureTrace( XZSTC_WARNING, "TRAME :  La trame lue est <%s>", vl_message_sauve);
	return ( XDC_NOK );
    }

    XZST_03EcritureTrace( XZSTC_INFO, "TRAME : Valeur-annee <%s>. ",
                          vl_Mot);

    if ((vl_Mot[0]!='2') && (vl_Mot[1]!='0'))
      return (XDC_NOK);
    pl_InfoTemps->tm_year = atoi( vl_Mot ) - 1900;

    vl_Mot = strtok( NULL, "~" );
    if ( vl_Mot == NULL )
    {
          /* Ecriture trace Erreur l */
           XZST_03EcritureTrace( XZSTC_WARNING, "TRAME : Impossible de lire la date. " );
            XZST_03EcritureTrace( XZSTC_WARNING, "TRAME :  La trame lue est <%s>", vl_message_sauve);
				            return ( XDC_NOK );
     }   
    XZST_03EcritureTrace( XZSTC_INFO, "TRAME : Valeur mois <%s>. ",
                          vl_Mot);
    pl_InfoTemps->tm_mon = atoi( vl_Mot );

    vl_Mot = strtok( NULL, "~" );
    if ( vl_Mot == NULL )
    {
            /* Ecriture trace Erreur l */
            XZST_03EcritureTrace( XZSTC_WARNING, "TRAME : Impossible de lire la date. " );
            XZST_03EcritureTrace( XZSTC_WARNING, "TRAME :  La trame lue est <%s>", vl_message_sauve);
            return ( XDC_NOK );
        }   

    XZST_03EcritureTrace( XZSTC_INFO, "TRAME : Valeur jour <%s>. ",
                          vl_Mot);
    pl_InfoTemps->tm_mday = atoi(vl_Mot);

    vl_Mot = strtok( NULL, "~" );
    if ( vl_Mot == NULL )
    {
        /* Ecriture trace Erreur l */
        XZST_03EcritureTrace( XZSTC_WARNING, "TRAME : Impossible de lire la date. " );
        XZST_03EcritureTrace( XZSTC_WARNING, "TRAME :  La trame lue est <%s>", vl_message_sauve);
        return ( XDC_NOK );
    }   

	XZST_03EcritureTrace( XZSTC_INFO, "TRAME : Valeur heure <%s>. ",
                          vl_Mot);
    pl_InfoTemps->tm_hour = atoi( vl_Mot );

    vl_Mot = strtok( NULL, "~" );
    if ( vl_Mot == NULL )
    {
        /* Ecriture trace Erreur l */
        XZST_03EcritureTrace( XZSTC_WARNING, "TRAME : Impossible de lire la date. " );
        XZST_03EcritureTrace( XZSTC_WARNING, "TRAME :  La trame lue est <%s>", vl_message_sauve);
        return ( XDC_NOK );
    }   

	XZST_03EcritureTrace( XZSTC_INFO, "TRAME : Valeur minute <%s>. ",
                          vl_Mot);
    pl_InfoTemps->tm_min = atoi( vl_Mot );

    vl_Mot = strtok( NULL, "|" );
    if ( vl_Mot == NULL )
    {
        /* Ecriture trace Erreur l */
        XZST_03EcritureTrace( XZSTC_WARNING, "TRAME : Impossible de lire la date. " );
        XZST_03EcritureTrace( XZSTC_WARNING, "TRAME :  La trame lue est <%s>", vl_message_sauve);
        return ( XDC_NOK );
    }   

    XZST_03EcritureTrace( XZSTC_INFO, "TRAME : Valeur seconde <%s>. ",
                          vl_Mot);
    pl_InfoTemps->tm_sec = atoi( vl_Mot );

    XZSM_07LireHorodate( XZSMC_TYPE_SYSTEME,
                         &vl_JourSem,
                         &vl_Horodate );

    if (vl_Mot == NULL)
    {
        /* Ecriture trace Erreur l */
        XZST_03EcritureTrace( XZSTC_WARNING, "TRAME : Impossible de lire l'horodate du message Wizcon. " );
        XZST_03EcritureTrace( XZSTC_WARNING, "TRAME :  La trame lue est <%s>", vl_message_sauve);
    }


    /* AAZ 03/09/2007 */
    switch ( vl_TypeOuvrage )
    {
        case XDC_EQT_TUBE :
            XZST_03EcritureTrace( XZSTC_INFO, "ANALYSE : Recherche du Tube correspondant");
            break;
        case XDC_EQT_ECH :
            XZST_03EcritureTrace( XZSTC_INFO, "ANALYSE : Recherche de l'echangeur correspondant");
            break;
        case XDC_EQT_LTN :
            XZST_03EcritureTrace( XZSTC_INFO, "ANALYSE : Recherche eqt du LTN correspondant");
            break;
        case XDC_EQT_HT :
            XZST_03EcritureTrace( XZSTC_INFO, "ANALYSE : Recherche eqt de l energie HT correspondante");
            break;        
        case XDC_EQT_SYS :
            XZST_03EcritureTrace( XZSTC_INFO, "ANALYSE : Recherche eqt du systeme correspondant");
            break;
        case XDC_EQT_LI :
            XZST_03EcritureTrace( XZSTC_INFO, "ANALYSE : Recherche eqt du lit d arret correspondant");
            break;
        default :
            XZST_03EcritureTrace( XZSTC_INFO, "ANALYSE : Ouvrage inconnu pour cet equipement");
            break;        
    }

    /* On lit le domaine */
    vl_Mot = strtok( NULL, "|" );
    XZST_03EcritureTrace( XZSTC_INFO, "TRAME : Valeur domaine <%s>. ",
                          vl_Mot);

    if ( vl_Mot == NULL )
    {
        /* Ecriture trace Erreur l */
        XZST_03EcritureTrace( XZSTC_WARNING, "TRAME : Impossible de lire le domaine du message Wizcon. " );
        XZST_03EcritureTrace( XZSTC_WARNING, "TRAME :  La trame lue est <%s>", vl_message_sauve);
        return ( XDC_NOK );
    }
    vl_domaine = atoi( vl_Mot );


    /*A Recherche dans la liste des locaux techniques si type evenement SIG */
    for ( pl_SIG = pg_debutSIG; 
          (vl_domaine == 1) && (pl_SIG != NULL);
          pl_SIG = pl_SIG->Suivant )
    {
        if ( pl_SIG->Numero == vl_NumOuvrage && pl_SIG->TypeOuvrage == vl_TypeOuvrage )
        {
            vl_Etat_Service = pl_SIG->Etat_Service;
            XZST_03EcritureTrace( XZSTC_INFO, "ANALYSE: SIG N� %d trouv�, etat de service %d.",
                                  pl_SIG->Numero,
                                  vl_Etat_Service );
            break;
        }
    }
   
    /*A Recherche dans la liste des locaux techniques si type evenement VEN */
    for ( pl_VEN = pg_debutVEN; 
          (vl_domaine == 2) && (pl_VEN != NULL);
          pl_VEN = pl_VEN->Suivant )
    {
        if ( pl_VEN->Numero == vl_NumOuvrage && pl_VEN->TypeOuvrage == vl_TypeOuvrage)
        {
            vl_Etat_Service = pl_VEN->Etat_Service;
            XZST_03EcritureTrace( XZSTC_INFO, "ANALYSE: VEN N� %d trouv�, etat de service %d.",
                                  pl_VEN->Numero,
                                  vl_Etat_Service );
            break;
        }
    }
   
    /*A Recherche dans la liste des locaux techniques si type evenement ECL */
    for ( pl_ECL = pg_debutECL; 
          (vl_domaine == 3) && (pl_ECL != NULL);
          pl_ECL = pl_ECL->Suivant )
    {
        if ( pl_ECL->Numero == vl_NumOuvrage && pl_ECL->TypeOuvrage == vl_TypeOuvrage)
        {
            vl_Etat_Service = pl_ECL->Etat_Service;
            XZST_03EcritureTrace( XZSTC_INFO, "ANALYSE: ECL N� %d trouv�, etat de service %d.",
                                  pl_ECL->Numero,
                                  vl_Etat_Service );
            break;
        }
    }
   
    /*A Recherche dans la liste des locaux techniques si type evenement ENR */
    for ( pl_ENR = pg_debutENR; 
          (vl_domaine == 4) && (pl_ENR != NULL);
          pl_ENR = pl_ENR->Suivant )
    {
        if ( pl_ENR->Numero == vl_NumOuvrage && pl_ENR->TypeOuvrage == vl_TypeOuvrage)
        {
            vl_Etat_Service = pl_ENR->Etat_Service;
            XZST_03EcritureTrace( XZSTC_INFO, "ANALYSE: ENR N� %d trouv�, etat de service %d.",
                                  pl_ENR->Numero,
                                  vl_Etat_Service );
            break;
        }
    }
   
/* Modifcation ABE du 07-01-2010 : tout �tait en commentaires, pass� en exploitation avec ajout domaine = 9 (?? auparavant) */    
    /*A Recherche dans la liste des locaux techniques si type evenement INC */
    for ( pl_INC = pg_debutINC; 
          (vl_domaine == 9) && (pl_INC != NULL);
          pl_INC = pl_INC->Suivant )
    {
        if ( pl_INC->Numero == vl_NumOuvrage && pl_INC->TypeOuvrage == vl_TypeOuvrage)
        {
            vl_Etat_Service = pl_INC->Etat_Service;
            XZST_03EcritureTrace( XZSTC_INFO, "ANALYSE: INC N� %d trouv�, etat de service %d.",
                                  pl_INC->Numero,
                                  vl_Etat_Service );
            break;
        }
    }
    
   
    /*A Recherche dans la liste des locaux techniques si type evenement EXP */
    for ( pl_EXP = pg_debutEXP; 
          (vl_domaine == 5 || vl_domaine == 6) && (pl_EXP != NULL);
          pl_EXP = pl_EXP->Suivant )
    {
		XZST_03EcritureTrace( XZSTC_INFO, "Recherche de l'ouvrage lie a l'exploitation : courant = (%d, %d), en cours = (%d, %d) ",
				pl_EXP->Numero,
				pl_EXP->TypeOuvrage,
				vl_NumOuvrage,
				vl_TypeOuvrage);
        if ( pl_EXP->Numero == vl_NumOuvrage && pl_EXP->TypeOuvrage == vl_TypeOuvrage)
        {
            vl_Etat_Service = pl_EXP->Etat_Service;
            XZST_03EcritureTrace( XZSTC_INFO, "ANALYSE: EXP N� %d trouv�, etat de service %d.",
                                  pl_EXP->Numero,
                                  vl_Etat_Service );
            break;
        }
    }

    /*A Recherche dans la liste des locaux techniques si type evenement LTN */
    for ( pl_LTN = pg_debutLTN; 
          (vl_domaine == 7) && (pl_LTN != NULL);
          pl_LTN = pl_LTN->Suivant )
    {
        XZST_03EcritureTrace( XZSTC_INFO, "Recherche LTN:  courant (pl_LTN->Numero, pl_LTN->TypeOuvrage) = (%d, %d), recherche = (%d, %d)",
						pl_LTN->Numero,
						pl_LTN->TypeOuvrage,
						vl_NumOuvrage,
						vl_TypeOuvrage);
        if ( pl_LTN->Numero == vl_NumOuvrage && pl_LTN->TypeOuvrage == vl_TypeOuvrage)
        {
            vl_Etat_Service = pl_LTN->Etat_Service;
            XZST_03EcritureTrace( XZSTC_INFO, "ANALYSE: SYS N� %d trouv�, etat de service %d.",
                                  pl_LTN->Numero,
                                  vl_Etat_Service );
            break;
        }
    }

    /*A Recherche dans la liste des locaux techniques si type evenement SYS */
    for ( pl_SYS = pg_debutSYS; 
          (vl_domaine == 8) && (pl_SYS != NULL);
          pl_SYS = pl_SYS->Suivant )
    {
        if ( pl_SYS->Numero == vl_NumOuvrage && pl_SYS->TypeOuvrage == vl_TypeOuvrage)
        {
            vl_Etat_Service = pl_SYS->Etat_Service;
            XZST_03EcritureTrace( XZSTC_INFO, "ANALYSE: SYS N� %d trouv�, etat de service %d.",
                                  pl_SYS->Numero,
                                  vl_Etat_Service );
            break;
        }
    }

    /* Retour a l'appelant si echangeur, tube, SIG, ECL, VEN, ENR, INC, EXP, SYS non trouve */      
    if ( (pl_SIG  == NULL) &&
         (pl_ECL  == NULL) &&
         (pl_VEN  == NULL) &&
         (pl_ENR  == NULL) &&
		 (pl_INC  == NULL) &&
         (pl_EXP  == NULL) &&
         (pl_SYS  == NULL) &&
         (pl_LTN  == NULL)) 
    {
        XZST_03EcritureTrace( XZSTC_DEBUG1, "ANALYSE:Numero ech, tube, SIG, ECL, VEN, ENR, INC, EXP ou SYS non trouve.=>SORTIE" );
        return;
    }
                           
    /*A Recuperation du site de la machine */
    if ( XZSC_04NomSite( pl_NomSite ) != XDC_OK)
    {
       /* Ecriture Trace */
       XZST_03EcritureTrace( XZSTC_WARNING, "ANALYSE : Recuperation du Nom du site impossible ." );
    }

   XZST_03EcritureTrace( XZSTC_DEBUG1, "ANALYSE : Analyse detaillee du type d'evenement <%d>",
                         vl_TypeEvt);

   /*A Recherche du type d'evenement */
    switch( vl_TypeEvt )
    {
         /*A C'est une alarme */
         case ENIX_REC_ALARME :
            /* On lit le type d'alarme */
            vl_Mot     = strtok( NULL, "|" );
            XZST_03EcritureTrace( XZSTC_INFO, "TRAME : Valeur type alarme <%s>. ",
                                  vl_Mot);
                                  
            if ( vl_Mot == NULL )
            {
                /* Ecriture trace Erreur l */
                XZST_03EcritureTrace( XZSTC_WARNING, "TRAME : Impossible de lire le type d'alarme du message Wizcon. " );
        	XZST_03EcritureTrace( XZSTC_WARNING, "TRAME :  La trame lue est <%s>", vl_message_sauve);
                return ( XDC_NOK );
            }
            vl_TypeAlarme = atoi( vl_Mot );

            /* Lecture type equipement */
            XZST_03EcritureTrace( XZSTC_INFO, "ANALYSE : Analyse detaillee du type d'equipement <%d> : TELEALARME",
                                  vl_TypeEvt);
            vl_Mot = strtok( NULL, "|" );
            XZST_03EcritureTrace( XZSTC_INFO, "ANALYSE: type equipement TELEALARME <%s>. ",
                                  vl_Mot);
            vl_typeequipement = atoi( vl_Mot );
            if ( vl_Mot == NULL )
            {
                /* Ecriture trace Erreur l */
                XZST_03EcritureTrace( XZSTC_WARNING, "ANALYSE: Impossible de lire la donnee type equipement de la TELEALARME du message Wizcon. " );
        	XZST_03EcritureTrace( XZSTC_WARNING, "TRAME :  La trame lue est <%s>", vl_message_sauve);
                return ( XDC_NOK );
            }
    
            /* Lecture id equipement */
            vl_Mot = strtok( NULL, "|" );
            XZST_03EcritureTrace( XZSTC_INFO, "ANALYSE: Valeur id equipement TELELARME <%s>. ",
                                  vl_Mot);
            vl_IdEquipement = atoi( vl_Mot );
            if ( vl_Mot == NULL )
            {
                /* Ecriture trace Erreur l */
                XZST_03EcritureTrace( XZSTC_WARNING, "ANALYSE: Impossible de lire la donnee id equipement de la TELEALARME du message Wizcon. " );
        	XZST_03EcritureTrace( XZSTC_WARNING, "TRAME :  La trame lue est <%s>", vl_message_sauve);
                return ( XDC_NOK );
            }
    
            /* Lecture id alarme */
            vl_Mot = strtok( NULL, "|" );
            XZST_03EcritureTrace( XZSTC_INFO, "ANALYSE: Valeur id alarme TELEALARME <%s>. ",
                                  vl_Mot);
            vl_IdAlarme = atoi( vl_Mot );
            if ( vl_Mot == NULL )
            {
                /* Ecriture trace Erreur l */
                XZST_03EcritureTrace( XZSTC_WARNING, "ANALYSE: Impossible de lire la donnee id alarme du message Wizcon. " );
        	XZST_03EcritureTrace( XZSTC_WARNING, "TRAME :  La trame lue est <%s>", vl_message_sauve);
                return ( XDC_NOK );
            }


            /* Lecture etat alarme */
            vl_Mot = strtok( NULL, "|" );
            XZST_03EcritureTrace( XZSTC_INFO, "ANALYSE: Valeur etat TELEALARME <%s>. ",
                                  vl_Mot);
            vl_etatalarme = atoi( vl_Mot );
            if ( vl_Mot == NULL )
            {
                /* Ecriture trace Erreur l */
                XZST_03EcritureTrace( XZSTC_WARNING, "ANALYSE: Impossible de lire la donnee etat alarme du message Wizcon. " );
        	XZST_03EcritureTrace( XZSTC_WARNING, "TRAME :  La trame lue est <%s>", vl_message_sauve);
                return ( XDC_NOK );
            }
            break;

         /*A C'est un ETOR, un CO ou une ALERTE */
         case ENIX_REC_ALERTE :
            XZST_03EcritureTrace( XZSTC_INFO, "ANALYSE : Analyse detaillee du type d'evenement <%d> : ALERTE", vl_TypeEvt);
    
            /* On lit le type d'alerte */
            vl_Mot     = strtok( NULL, "|" );
            XZST_03EcritureTrace( XZSTC_INFO, "ALERTE : Type d'alerte : <%s>", vl_Mot);
                                  
            if ( vl_Mot == NULL )
            {
                /* Ecriture trace Erreur l */
                XZST_03EcritureTrace( XZSTC_WARNING, "TRAME : Impossible de lire le type d'alerte du message Wizcon. " );
        	XZST_03EcritureTrace( XZSTC_WARNING, "TRAME :  La trame lue est <%s>", vl_message_sauve);
                return ( XDC_NOK );
            } 
            vl_TypeAlerte = atoi( vl_Mot );
			
            /* Lecture type equipement */
            vl_Mot = strtok( NULL, "|" );
            XZST_03EcritureTrace( XZSTC_INFO, "ALERTE: Type d'equipement : <%s>",
                                  vl_Mot);
            vl_typeequipement = atoi( vl_Mot );
            if ( vl_Mot == NULL )
            {
                /* Ecriture trace Erreur l */
                XZST_03EcritureTrace( XZSTC_WARNING, "ANALYSE: Impossible de lire la donnee type equipement de l'alerte du message Wizcon. " );
        	XZST_03EcritureTrace( XZSTC_WARNING, "TRAME :  La trame lue est <%s>", vl_message_sauve);
                return ( XDC_NOK );
            }
    
            /* Lecture id equipement */
            vl_Mot = strtok( NULL, "|" );
            XZST_03EcritureTrace( XZSTC_INFO, "ALERTE : Id equipement : <%s>",
                                  vl_Mot);
            vl_IdEquipement = atoi( vl_Mot );
            if ( vl_Mot == NULL )
            {
                /* Ecriture trace Erreur l */
                XZST_03EcritureTrace( XZSTC_WARNING, "ANALYSE: Impossible de lire la donnee id equipement de l'ALERTE du message Wizcon. " );
        	XZST_03EcritureTrace( XZSTC_WARNING, "TRAME :  La trame lue est <%s>", vl_message_sauve);
                return ( XDC_NOK );
            }
    
            /* Lecture id alerte */
            vl_Mot = strtok( NULL, "|" );
            XZST_03EcritureTrace( XZSTC_INFO, "ALERTE : Identifiant de l'alerte : <%s>",
                                  vl_Mot);
            vl_IdAlerte = atoi( vl_Mot );
            if ( vl_Mot == NULL )
            {
                /* Ecriture trace Erreur l */
                XZST_03EcritureTrace( XZSTC_WARNING, "ANALYSE: Impossible de lire la donnee id alerte du message Wizcon. " );
        	XZST_03EcritureTrace( XZSTC_WARNING, "TRAME :  La trame lue est <%s>", vl_message_sauve);
                return ( XDC_NOK );
            }

            /* Lecture etat alerte */
            vl_Mot = strtok( NULL, "|" );
            XZST_03EcritureTrace( XZSTC_INFO, "ALERTE : Etat de l'alerte : <%s>",
                                  vl_Mot);
            vl_etatalerte = atoi( vl_Mot );
            if ( vl_Mot == NULL )
            {
                /* Ecriture trace Erreur l */
                XZST_03EcritureTrace( XZSTC_WARNING, "ANALYSE: Impossible de lire la donnee etat alerte du message Wizcon. " );
        	XZST_03EcritureTrace( XZSTC_WARNING, "TRAME :  La trame lue est <%s>", vl_message_sauve);
                return ( XDC_NOK );
            }			
            break;

        /*A C'est un DISCORDANCE Equipement */
        case ENIX_REC_DISCEQT :
        /*A C'est un equipement */
        case ENIX_REC_EQUIPEMENT :         
            
            XZST_03EcritureTrace( XZSTC_INFO, "ANALYSE : Analyse detaillee du type d'evenement <%d> : DISCEQT, ou ETAT EQT",
                                  vl_TypeEvt);


            /* On lit le type d'equipement */
            vl_Mot     = strtok( NULL, "|" );
            XZST_03EcritureTrace( XZSTC_INFO, "TRAME : Valeur type equipement <%s>. ", vl_Mot);
                                  
            if ( vl_Mot == NULL )
            {
                /* Ecriture trace Erreur l */
                XZST_03EcritureTrace( XZSTC_WARNING, "TRAME : Impossible de lire le type equipement du message Wizcon. " );
        	XZST_03EcritureTrace( XZSTC_WARNING, "TRAME :  La trame lue est <%s>", vl_message_sauve);
                return ( XDC_NOK );
            }
            vl_typeequipement = atoi( vl_Mot );


            /* Lecture numero de equipement */
            vl_Mot = strtok( NULL, "|" );
            XZST_03EcritureTrace( XZSTC_INFO, "ANALYSE :  Valeur numero equipement <%s>. ",
                                  vl_Mot);
            if ( vl_Mot == NULL )
            {
                /* Ecriture trace Erreur l */
                XZST_03EcritureTrace( XZSTC_WARNING, "ANALYSE :  Impossible de lire le numero equipement du message Wizcon. " );
        	XZST_03EcritureTrace( XZSTC_WARNING, "TRAME :  La trame lue est <%s>", vl_message_sauve);
                return ( XDC_NOK );
            }
            vl_numequipement = atoi( vl_Mot );
            
            /* Lecture etat equipement */
            vl_Mot = strtok( NULL, "|" );
            XZST_03EcritureTrace( XZSTC_INFO, "ANALYSE :  Valeur etat equipement <%s>. ",
                                  vl_Mot);
            if ( vl_Mot == NULL )
            {
                /* Ecriture trace Erreur l */
                XZST_03EcritureTrace( XZSTC_WARNING, "ANALYSE :  Impossible de lire l'etat equipement du message Wizcon. " );
        	XZST_03EcritureTrace( XZSTC_WARNING, "TRAME :  La trame lue est <%s>", vl_message_sauve);
                return ( XDC_NOK );
            }
            vl_etatequipement = atoi( vl_Mot );
            
            /* Lecture etat technique equipement */
            vl_Mot = strtok( NULL, "|" );
            XZST_03EcritureTrace( XZSTC_INFO, "ANALYSE :  Valeur etat technique equipement <%s>. ",
                                  vl_Mot);
            if ( vl_Mot == NULL )
            {
                /* Ecriture trace Erreur l */
                XZST_03EcritureTrace( XZSTC_WARNING, "ANALYSE :  Impossible de lire l'etat technique equipement du message Wizcon. " );
        	XZST_03EcritureTrace( XZSTC_WARNING, "TRAME :  La trame lue est <%s>", vl_message_sauve);
                return ( XDC_NOK );
            }
            vl_etattechnique = atoi( vl_Mot );
            //break;
            
            /* Lecture mode equipement */
            vl_Mot = strtok( NULL, "|" );
            XZST_03EcritureTrace( XZSTC_INFO, "ANALYSE :  Valeur mode equipement <%s>. ",
                                  vl_Mot);
            if ( vl_Mot == NULL )
            {
                /* Ecriture trace Erreur l */
                XZST_03EcritureTrace( XZSTC_WARNING, "ANALYSE :  Impossible de lire le mode equipement du message Wizcon. " );
        	XZST_03EcritureTrace( XZSTC_WARNING, "TRAME :  La trame lue est <%s>", vl_message_sauve);
                return ( XDC_NOK );
            }
            vl_mode = atoi( vl_Mot );
            if (vl_TypeEvt == ENIX_REC_DISCEQT)
            {
            
                /* Lecture mode equipement */
                vl_Mot = strtok( NULL, "|" );
                XZST_03EcritureTrace( XZSTC_INFO, "ANALYSE :  Valeur defaut discordance <%s>. ",
                                      vl_Mot);
                if ( vl_Mot == NULL )
                {
                    /* Ecriture trace Erreur l */
                    XZST_03EcritureTrace( XZSTC_WARNING, "ANALYSE :  Impossible de lire le defaut discordance du message Wizcon. " );
        	XZST_03EcritureTrace( XZSTC_WARNING, "TRAME :  La trame lue est <%s>", vl_message_sauve);
                    return ( XDC_NOK );
                }
                vl_defaut = atoi( vl_Mot );                
            }
			
			/* Lecture mode rafale */
			vl_Mot = strtok( NULL, "|" );
			XZST_03EcritureTrace( XZSTC_INFO, "ANALYSE :  Valeur mode rafale <%s>. ",
								  vl_Mot);
			if ( vl_Mot == NULL )
			{
				/* Ecriture trace Erreur l */
				XZST_03EcritureTrace( XZSTC_WARNING, "ANALYSE :  Impossible de lire le mode rafale du message Wizcon. " );
        			XZST_03EcritureTrace( XZSTC_WARNING, "TRAME :  La trame lue est <%s>", vl_message_sauve);
				return ( XDC_NOK );
			}
			vl_ModeRafale = atoi( vl_Mot );                
			
            break;

         /*A C'est un DISCORDANCE SEQUENCE */
         case ENIX_REC_DISCSEQ :
            /* Lecture etat equipement */
            XZST_03EcritureTrace( XZSTC_INFO, "ANALYSE : Analyse detaillee du type d'evenement <%d> : DISC SEQ",
                                  vl_TypeEvt);
            vl_Mot = strtok( NULL, "|" );
            XZST_03EcritureTrace( XZSTC_INFO, "ANALYSE :  Valeur numero sequence panneau <%s>. ",
                                  vl_Mot);
            if ( vl_Mot == NULL )
            {
                /* Ecriture trace Erreur l */
                XZST_03EcritureTrace( XZSTC_WARNING, "ANALYSE :  Impossible de lire le numero de sequence du message Wizcon. " );
        	XZST_03EcritureTrace( XZSTC_WARNING, "TRAME :  La trame lue est <%s>", vl_message_sauve);
                return ( XDC_NOK );
            }
            vl_numsequence = atoi( vl_Mot );
    
            /* Lecture discordance de panneau */
            vl_Mot = strtok( NULL, "|" );
            XZST_03EcritureTrace( XZSTC_INFO, "ANALYSE :  Valeur discordance sequence <%s>. ",
                                  vl_Mot);
            if ( vl_Mot == NULL )
            {
                /* Ecriture trace Erreur l */
                XZST_03EcritureTrace( XZSTC_WARNING, "ANALYSE :  Impossible de lire l'etat de discordance sequence du message Wizcon. " );
        	XZST_03EcritureTrace( XZSTC_WARNING, "TRAME :  La trame lue est <%s>", vl_message_sauve);
                return ( XDC_NOK );
            }
            vl_etatdisc = atoi( vl_Mot );
            break;
        /*A C'est un domaine */
        case ENIX_REC_DOMAINE :
            
            XZST_03EcritureTrace( XZSTC_INFO, "ANALYSE : Analyse detaillee du type d'evenement <%d> : ETAT DOMAINE",
                                  vl_TypeEvt);
            
            /* Lecture etat domaine */
            vl_Mot = strtok( NULL, "|" );
            XZST_03EcritureTrace( XZSTC_INFO, "ANALYSE :  Valeur mode domaine <%s>. ",
                                  vl_Mot);
            if ( vl_Mot == NULL )
            {
                /* Ecriture trace Erreur l */
                XZST_03EcritureTrace( XZSTC_WARNING, "ANALYSE :  Impossible de lire le mode domaine du message Wizcon. " );
        	XZST_03EcritureTrace( XZSTC_WARNING, "TRAME :  La trame lue est <%s>", vl_message_sauve);
                return ( XDC_NOK );
            }
            vl_mode = atoi( vl_Mot );
			
			/**/
            vl_Mot = strtok( NULL, "|" );
            XZST_03EcritureTrace( XZSTC_INFO, "ANALYSE :  Valeur id scenario domaine <%s>. ",
                                  vl_Mot);
            if ( vl_Mot == NULL )
            {
                /* Ecriture trace Erreur l */
                XZST_03EcritureTrace( XZSTC_WARNING, "ANALYSE :  Impossible de lire l'id scenario domaine du message Wizcon. " );
        	XZST_03EcritureTrace( XZSTC_WARNING, "TRAME :  La trame lue est <%s>", vl_message_sauve);
                return ( XDC_NOK );
            }
            vl_IdScenario = atoi( vl_Mot );			
            break;         
    }

/* AZAZAZAZAZA */
    switch( vl_domaine )
    {
         /*A C'est une Telealarame */
         case 1 :
             vl_domaine_socket = XDC_EQT_SIG;
			 if (pl_SIG == NULL)
			 {
                XZST_03EcritureTrace( XZSTC_INFO, "ANALYSE :  Aucun ouvrage de type signalisation, abandon " );
				return XDC_NOK;
			 }
             break;
         case 2 :
             vl_domaine_socket = XDC_EQT_VEN;
			 if (pl_VEN == NULL)
			 {
                XZST_03EcritureTrace( XZSTC_INFO, "ANALYSE :  Aucun ouvrage de type ventilation, abandon " );
				return XDC_NOK;
			 }
             break;
         case 3 :
             vl_domaine_socket = XDC_EQT_ECL;
			 if (pl_ECL == NULL)
			 {
                XZST_03EcritureTrace( XZSTC_INFO, "ANALYSE :  Aucun ouvrage de type eclairage, abandon " );
				return XDC_NOK;
			 }
             break;
         case 4 :
             vl_domaine_socket = XDC_EQT_ENR;
			 if (pl_ENR == NULL)
			 {
                XZST_03EcritureTrace( XZSTC_INFO, "ANALYSE :  Aucun ouvrage de type energie, abandon " );
				return XDC_NOK;
			 }
             break;
         case 5 :
         case 6 :
             vl_domaine_socket = XDC_EQT_EXP;
			 if (pl_EXP == NULL)
			 {
                XZST_03EcritureTrace( XZSTC_INFO, "ANALYSE :  Aucun ouvrage de type exploitation, abandon " );
				return XDC_NOK;
			 }
             break;
#ifdef _HPUX_SOURCE
             vl_domaine_socket = NULL;
#else
             vl_domaine_socket = 0;
#endif
             break;
         case 7 :
             vl_domaine_socket = XDC_EQT_LTN;
			 if (pl_LTN == NULL)
			 {
                XZST_03EcritureTrace( XZSTC_INFO, "ANALYSE :  Aucun ouvrage de type ltn, abandon " );
				return XDC_NOK;
			 }             
			 break;
         case 8 :
             vl_domaine_socket = XDC_EQT_SYS;
			 if (pl_SYS == NULL)
			 {
                XZST_03EcritureTrace( XZSTC_INFO , "ANALYSE :  Aucun ouvrage de type systeme, abandon " );
				return XDC_NOK;
			 }
             break;
         case 9 :
             vl_domaine_socket = XDC_EQT_INC;
			 if (pl_INC == NULL)
			 {
                XZST_03EcritureTrace( XZSTC_INFO , "ANALYSE :  Aucun ouvrage de type protection incendie, abandon " );
				return XDC_NOK;
			 }
             break;
         default :
			XZST_03EcritureTrace( XZSTC_INFO, "ANALYSE :  Aucun domaine, abandon " );
			return XDC_NOK;
            break;
     }
	 

    /*A Recherche du type d'evenement */
    XZST_03EcritureTrace( XZSTC_INFO, " Traitement du type d'evenement" );
    switch( vl_TypeEvt )
    {
         /*A C'est une Telealarame */
         case ENIX_REC_ALARME :

            XZST_03EcritureTrace( XZSTC_INFO, "TELEALARME : Domaine = %d, TypeEqt = %d, NumEqt = %d, TypeAlarme = %d, Etat = %d",
			vl_domaine, vl_typeequipement , vl_IdEquipement ,  vl_TypeAlarme, vl_etatalarme);
			switch( vl_domaine )
			{
				case 1 :
						pl_SIG->InfoRecue = XDC_OUI;
						break;
				case 2 :
						pl_VEN->InfoRecue = XDC_OUI;
						break;
				case 3 :
						pl_ECL->InfoRecue = XDC_OUI;
						break;
				case 4 :
						pl_ENR->InfoRecue = XDC_OUI;
						break;
				case 5 :
				case 6 :
						pl_EXP->InfoRecue = XDC_OUI;
						break;
				case 7 :
						pl_LTN->InfoRecue = XDC_OUI;
						break;
				case 8 :
						pl_SYS->InfoRecue = XDC_OUI;
						break;
				case 9 :
						pl_INC->InfoRecue = XDC_OUI;
						break;
				default :
						XZST_03EcritureTrace(XZSTC_WARNING, "TELEALARME : Impossible de determiner le domaine pour l'alarme : %d",vl_domaine  );
						break;
			}
			if (vl_IdEquipement != 0 && vl_typeequipement != 0 && vl_TypeAlarme != 0)
			{
				/******************************************/
				/* L'alarme est rattachee a un equipement */
				/******************************************/
				
				/* Envoi de l'alarme rattachee a un equipement */
				ex_env_alarme ( vg_NomMachine,
								vl_Horodate,
								vl_typeequipement,
								vl_IdEquipement,
								vl_TypeAlarme,
								vl_etatalarme,
								NULL,
								XZSC_22NumeroSiteLocal());
			} 
			else if (vl_TypeOuvrage != 0 && vl_NumOuvrage != 0 && vl_TypeAlarme != 0)
			{
				/******************************************/
				/* L'alarme est rattachee a un ouvrage    */
				/******************************************/
				/* Envoi de l'alarme rattachee a un ouvrage */
				ex_env_alarme ( vg_NomMachine,
								vl_Horodate,
								vl_TypeOuvrage,
								vl_NumOuvrage,
								vl_TypeAlarme,
								vl_etatalarme,
								NULL,
								XZSC_22NumeroSiteLocal());
			}
			
            XZST_03EcritureTrace( XZSTC_INFO, "TELEALARME : Fin");
            break;

         /*A C'est une discordance sequence  */
         case ENIX_REC_DISCSEQ :            
            XZST_03EcritureTrace( XZSTC_INFO, "DISCSEQ : Debut");
            /* Recherche si sequence concernee */
            switch( vl_domaine )
            {
                case 1 :
                    vl_Sequence = pl_SIG->ListeSequence;
					pl_SIG->InfoRecue = XDC_OUI;
                    break;
                case 2 :
                    vl_Sequence = pl_VEN->ListeSequence;
					pl_VEN->InfoRecue = XDC_OUI;
                    break;
                case 3 :
                    vl_Sequence = pl_ECL->ListeSequence;
					pl_ECL->InfoRecue = XDC_OUI;
                    break;
                case 4 :
                    vl_Sequence = pl_ENR->ListeSequence;
					pl_ENR->InfoRecue = XDC_OUI;
                    break;
                case 5 :
                case 6 :
                    vl_Sequence = pl_EXP->ListeSequence;
					pl_EXP->InfoRecue = XDC_OUI;
                    break;
                case 7 :
                    vl_Sequence = pl_LTN->ListeSequence;
					pl_LTN->InfoRecue = XDC_OUI;
                    break;
                case 8 :
                    vl_Sequence = pl_SYS->ListeSequence;
					pl_SYS->InfoRecue = XDC_OUI;
                    break;
                case 9 :
                    vl_Sequence = pl_INC->ListeSequence;
					pl_INC->InfoRecue = XDC_OUI;
                    break;
                default :
                    vl_Sequence = NULL;
                    break;
            }

            for ( pl_Sequence =  vl_Sequence;
                  pl_Sequence != NULL; 
                  pl_Sequence =  pl_Sequence->Suivant )
            {
                XZST_03EcritureTrace( XZSTC_INFO, "DISCSEQ : Sequence <%d> Disc <%d> ",
                                      pl_Sequence->NumSequence,
                                      pl_Sequence->EtatDisc);
                if (pl_Sequence->NumSequence == vl_numsequence)
                {
                    /* Memorisation etat sequence */
                    XZST_03EcritureTrace( XZSTC_INFO, "DISCSEQ : Sequence trouvee <%d> Disc <%d> ",
                                          pl_Sequence->NumSequence,
                                          pl_Sequence->EtatDisc);
                    XZST_03EcritureTrace( XZSTC_DEBUG1, "Mise a jour de la discordance : avant = %d, apres = %d (sequence %d)",
										  pl_Sequence->EtatDisc,
										  vl_etatdisc,
                                          pl_Sequence->NumSequence
                                          );
										  
					pl_Sequence->AttenteEtat = XDC_NON;
                    pl_Sequence->EtatDisc = vl_etatdisc;
					pl_Sequence->InfoRecue	  = XDC_OUI;
					
                    if ( (vl_domaine == 1) && (pl_SIG->Etat_Service==0) )
                    {
						/* Appel de XZAT40 avec XDC_NOK et NumAction */
						XZST_03EcritureTrace( XZSTC_DEBUG1, "Envoi d'un CR pour la sequence %d de l'action %d",
											  pl_Sequence->NumSequence,
											  pl_Sequence->NumAction);
						XZST_03EcritureTrace ( XZSTC_DEBUG1 ,"Appel de xzat40_CompteRendu_SIG (numact =%d) : %s", pl_Sequence->NumAction, pl_Sequence->EtatDisc != 0 ? "NOK": "OK");
						xzat40_CompteRendu_SIG ( pl_Sequence->NumAction,
												 vl_Horodate,
												 XDC_OK);
						XZST_03EcritureTrace( XZSTC_INFO, "DISCSEQ : Envoi XZAT40 pour domaine signalisation <%d> Sequence <%d> Numaction <%d> Disc <%d> ",
											  pl_SIG->Numero,
											  pl_Sequence->NumSequence,
											  pl_Sequence->NumAction,
											  pl_Sequence->EtatDisc);
                        if ( (pl_Sequence->EtatDisc != 0) && (pl_Sequence->NumAction <= 0) )
                        {
                            /* Envoyer alerte discordance sequence */
							XZST_03EcritureTrace ( XZSTC_INFO ,"Generation d'une alerte de discordance de sequence domaine Signalisation");
                            ex_env_alerte ( vl_Horodate,
                                            vl_TypeOuvrage,
                                            pl_SIG->Numero,
                                            XZAAC_AFF_DIF,
                                            vg_Mode_Fonct,
											XZSC_22NumeroSiteLocal(),
                                            pl_Sequence->NumSequence,
                                            NULL );
                        }
						else
						{
							XZST_03EcritureTrace ( XZSTC_DEBUG1 ,"Pas de generation d'une alerte de discordance de sequence");
						}
                    }

                    if ( (vl_domaine == 3) && (pl_ECL->Etat_Service==0) )
                    {
						/* Appel de XZAT41 avec XDC_NOK et NumAction */
						xzat41_CompteRendu_ECL ( pl_Sequence->NumAction,
												 vl_Horodate,
												 XDC_OK);
						XZST_03EcritureTrace( XZSTC_INFO, "DISCSEQ : Envoi XZAT41 pour domaine eclairage <%d> Sequence <%d> Numaction <%d> Disc <%d> ",
											  pl_ECL->Numero,
											  pl_Sequence->NumSequence,
											  pl_Sequence->NumAction,
											  pl_Sequence->EtatDisc);
                        if ( (pl_Sequence->EtatDisc != 0) )
                        {
                            /* Envoyer alerte discordance sequence */
							XZST_03EcritureTrace ( XZSTC_INFO ,"Generation d'une alerte de discordance de sequence domaine Eclairage");
                            ex_env_alerte ( vl_Horodate,
                                            vl_TypeOuvrage,
                                            pl_ECL->Numero,
                                            XZAAC_AFF_DIF,
                                            vg_Mode_Fonct,
											XZSC_22NumeroSiteLocal(),
                                            pl_Sequence->NumSequence,
                                            NULL );
						}
                    }

                    if ( (vl_domaine == 2) && (pl_VEN->Etat_Service==0) )
                    {
						/* Appel de XZAT42 avec XDC_NOK et NumAction */
						xzat42_CompteRendu_VEN ( pl_Sequence->NumAction,
												 vl_Horodate,
												 XDC_OK);
						XZST_03EcritureTrace( XZSTC_INFO, "DISCSEQ : Envoi XZAT42 pour domaine ventilation <%d> Sequence <%d> Numaction <%d> Disc <%d> ",
											  pl_VEN->Numero,
											  pl_Sequence->NumSequence,
											  pl_Sequence->NumAction,
											  pl_Sequence->EtatDisc);
                        if ( (pl_Sequence->EtatDisc != 0) )
                        {
                            /* Envoyer alerte discordance sequence */
							XZST_03EcritureTrace ( XZSTC_INFO ,"Generation d'une alerte de discordance de sequence domaine Ventilation");
                            ex_env_alerte ( vl_Horodate,
                                            vl_TypeOuvrage,
                                            pl_VEN->Numero,
                                            XZAAC_AFF_DIF,
                                            vg_Mode_Fonct,
											XZSC_22NumeroSiteLocal(),
                                            pl_Sequence->NumSequence,
                                            NULL );
                        }
                    }

                    if ( (vl_domaine == 4) && (pl_ENR->Etat_Service==0) )
                    {
						/* Appel de XZAT43 avec XDC_NOK et NumAction */
						xzat43_CompteRendu_ENR ( pl_Sequence->NumAction,
												 vl_Horodate,
												 XDC_OK);
						XZST_03EcritureTrace( XZSTC_INFO, "DISCSEQ : Envoi XZAT43 pour domaine energie <%d> Sequence <%d> Numaction <%d> Disc <%d> ",
											  pl_ENR->Numero,
											  pl_Sequence->NumSequence,
											  pl_Sequence->NumAction,
											  pl_Sequence->EtatDisc);
                        if (pl_Sequence->EtatDisc != 0)
                        {
                            /* Envoyer alerte discordance sequence */
							XZST_03EcritureTrace ( XZSTC_INFO ,"Generation d'une alerte de discordance de sequence domaine Energie");
                            ex_env_alerte ( vl_Horodate,
                                            vl_TypeOuvrage,
                                            pl_ENR->Numero,
                                            XZAAC_AFF_DIF,
                                            vg_Mode_Fonct,
											XZSC_22NumeroSiteLocal(),
                                            pl_Sequence->NumSequence,
                                            NULL );
                        }
                    }
                    if ( (vl_domaine == 5 || vl_domaine == 6) && (pl_EXP->Etat_Service==0) )
                    {
						/* Appel de XZAT45 avec XDC_NOK et NumAction */
						xzat45_CompteRendu_EXP ( pl_Sequence->NumAction,
												 vl_Horodate,
												 XDC_OK);
						XZST_03EcritureTrace( XZSTC_INFO, "DISCSEQ : Envoi XZAT45 pour domaine exploitation <%d> Sequence <%d> Numaction <%d> Disc <%d> ",
											  pl_EXP->Numero,
											  pl_Sequence->NumSequence,
											  pl_Sequence->NumAction,
											  pl_Sequence->EtatDisc);
                        if (pl_Sequence->EtatDisc != 0)
                        {
                            /* Envoyer alerte discordance sequence */
							XZST_03EcritureTrace ( XZSTC_INFO ,"Generation d'une alerte de discordance de sequence domaine Exploitation");
                            ex_env_alerte ( vl_Horodate,
                                            vl_TypeOuvrage,
                                            pl_EXP->Numero,
                                            XZAAC_AFF_DIF,
                                            vg_Mode_Fonct,
											XZSC_22NumeroSiteLocal(),
                                            pl_Sequence->NumSequence,
                                            NULL );
                        }
                    }	

                    /* Envoyer Alarme discordance sequence */
                    sprintf ( vl_TexteAlarme, "%d", pl_Sequence->NumSequence );
                    if ( vl_Etat_Service==0 )
                    {
                        XZST_03EcritureTrace( XZSTC_INFO,
                                              "DISCSEQ : Envoi alerte Discordance Sequence <%d> Type Eqt <%d> Num Eqt <%d> Etat disc <%d> texte <%s> ",
                                              pl_Sequence->NumSequence,
                                              vl_TypeOuvrage,
                                              vl_NumOuvrage,
                                              vl_etatdisc,
                                              vl_TexteAlarme );

                        ex_env_alarme ( vg_NomMachine,
                                        vl_Horodate,
                                        vl_TypeOuvrage,
                                        vl_NumOuvrage,
                                        XZAMC_DISC_SEQUENCE,
                                        vl_etatdisc,
                                        vl_TexteAlarme,
										XZSC_22NumeroSiteLocal());
                    }
                    /* Envoi etat Sequence dans XDM_ETAT_SEQUENCE sur XDG_ETAT_SEQUENCE */
                    switch( vl_domaine )
                    {
        
                        case 1 :
                            sprintf( vl_Texte_Etat_Sequence, "%s",
                                     XDG_ETAT_SEQ_SIG);
                            vl_Etat_Sequence = XDM_ETAT_SEQSIG;
                            vl_Derniere_Sequence = pl_SIG->DerniereSequence;
                            break;
                            
                        case 3 :
                            sprintf( vl_Texte_Etat_Sequence, "%s",
                                     XDG_ETAT_SEQ_ECL);
                            vl_Etat_Sequence = XDM_ETAT_SEQECL;
                            vl_Derniere_Sequence = pl_ECL->DerniereSequence;
                            break;
        
                        case 2 :
                            sprintf( vl_Texte_Etat_Sequence, "%s",
                                     XDG_ETAT_SEQ_VEN);
                            vl_Etat_Sequence     = XDM_ETAT_SEQVEN;
                            vl_Derniere_Sequence = pl_VEN->DerniereSequence;
                            break;

                        case 4 :
                            sprintf( vl_Texte_Etat_Sequence, "%s",
                                     XDG_ETAT_SEQ_ENR );
                            vl_Etat_Sequence     = XDM_ETAT_SEQENR;
                            vl_Derniere_Sequence = pl_ENR->DerniereSequence;
                            break;
                        case 5 :
                        case 6 :
                            sprintf( vl_Texte_Etat_Sequence, "%s",
                                     XDG_ETAT_SEQ_EXP );
                            vl_Etat_Sequence     = XDM_ETAT_SEQEXP;
                            vl_Derniere_Sequence = pl_EXP->DerniereSequence;
                            break;
						default :
                            sprintf( vl_Texte_Etat_Sequence, "0" );
                            vl_Etat_Sequence     = 0;
                            vl_Derniere_Sequence = 0;
                    }

                    sprintf ( pl_NomDG, "%s_%s", 
                              vl_Texte_Etat_Sequence,
                              pl_NomSite );
                    if ( (vl_Etat_Service == 0) )
                        if (!TipcSrvMsgWrite ( pl_NomDG,
                                               XDM_IdentMsg(vl_Etat_Sequence),
                                               XDM_FLG_SRVMSGWRITE,
                                               T_IPC_FT_INT4, vl_TypeOuvrage,
                                               T_IPC_FT_INT4, vl_NumOuvrage,
                                               T_IPC_FT_INT4, vl_Derniere_Sequence,
                                               T_IPC_FT_INT4, pl_Sequence->NumSequence,
                                               T_IPC_FT_INT4, vl_etatdisc,
                                               NULL ) )
                        {
                            XZST_03EcritureTrace(XZSTC_WARNING, "DISCSEQ : Envoi message XDM_ETAT_SEQxxx non effectue." );
                        }
                        else
                        {
                            XZST_03EcritureTrace( XZSTC_DEBUG1, "DISCSEQ : Envoi message XDM_ETAT_SEQUxxx <NomDG>%s<TypEqt>%d<NumEqt>%d<DernSeq>%d<NumSeq>%d<EtatDisc>%d<IdMessage>%d",
                                                  pl_NomDG,
                                                  vl_TypeOuvrage,
                                                  vl_NumOuvrage,
                                                  vl_Derniere_Sequence,
                                                  pl_Sequence->NumSequence,
                                                  vl_etatdisc,
												  vl_Etat_Sequence);
                        }
                } 
            }
            break;

        /*A C'est une Alerte */
        case ENIX_REC_ALERTE :

            XZST_03EcritureTrace( XZSTC_INFO, "ALERTE : Debut");
            /* Recherche alerte pour equipement/domaine */
            switch( vl_domaine )
            {
                case 1 :
                    vl_etatequipement = pl_SIG->Etat_Service;
					pl_SIG->InfoRecue = XDC_OUI;
                    break;
                case 3 :
                    vl_etatequipement = pl_ECL->Etat_Service;
					pl_ECL->InfoRecue = XDC_OUI;
                    break;
                case 2 :
                    vl_etatequipement = pl_VEN->Etat_Service;
					pl_VEN->InfoRecue = XDC_OUI;
                    break;
                case 4 :
                    vl_etatequipement = pl_ENR->Etat_Service;
					pl_ENR->InfoRecue = XDC_OUI;
                    break;
                case 5 :
                case 6 :
                    vl_etatequipement = pl_EXP->Etat_Service;
					pl_EXP->InfoRecue = XDC_OUI;
                    break;
                case 7 :
                    vl_etatequipement = pl_LTN->Etat_Service;
					pl_LTN->InfoRecue = XDC_OUI;
                    break;
                case 8 :
                    vl_etatequipement = pl_SYS->Etat_Service;
					pl_SYS->InfoRecue = XDC_OUI;
                    break;
                case 9 :
                    vl_etatequipement = pl_INC->Etat_Service;
					pl_INC->InfoRecue = XDC_OUI;
                    break;
				default :
                    vl_etatequipement = 0;
                    break;
            }
			if (vl_IdEquipement != 0 && vl_typeequipement != 0 && vl_TypeAlerte != 0)
			{
				/******************************************/
				/* L'alerte est rattachee a un equipement */
				/******************************************/
				// Envoi alerte
				if ( vl_etatalerte == 1 && vl_etatequipement == 0 ) 
				{
					XZST_03EcritureTrace ( XZSTC_INFO ,"Generation d'une alerte");
					ex_env_alerte ( vl_Horodate,
									vl_typeequipement,
									vl_IdEquipement,
									vl_TypeAlerte,
									vg_Mode_Fonct,
									XZSC_22NumeroSiteLocal(),
									NULL );
				}
									
				// Envoi etat Alerte dans XDM_ETAT_ALERTE sur XDG_ETAT_ALERTE
				if ( ( vl_etatequipement == 0) )
				{
					sprintf ( pl_NomDG, "%s_%s",
							  XDG_ETAT_ALERTE,
							  pl_NomSite );

					if (!TipcSrvMsgWrite ( pl_NomDG,
										   XDM_IdentMsg(XDM_ETAT_ALERTE),
										   XDM_FLG_SRVMSGWRITE,
										   T_IPC_FT_INT4, vl_IdEquipement,
										   T_IPC_FT_INT4, vl_IdAlerte,
										   T_IPC_FT_INT4, vl_etatalerte,
										   NULL ) )
					{
						XZST_03EcritureTrace(XZSTC_WARNING, "ALERTE : Envoi message XDM_ETAT_ALERTE non effectue." );
					}
					else
					{
						XZST_03EcritureTrace( XZSTC_INFO, "ALERTE : Envoi message XDM_ETAT_ALERTE <NomDG>%s_<NumEqt>%d_<NumAlerte>%d_<EtatAlerte>%d",
											  pl_NomDG,
											  vl_IdEquipement,
											  vl_IdAlerte,
											  vl_etatalerte);
					}
				}	
			}
			else if (vl_TypeOuvrage != 0 && vl_NumOuvrage != 0 && vl_TypeAlerte != 0)
			{
				/******************************************/
				/* L'alerte est rattachee a un ouvrage    */
				/******************************************/
				// Envoi alerte
				if ( vl_etatalerte == 1 && vl_etatequipement == 0 ) 
				{
					XZST_03EcritureTrace ( XZSTC_INFO ,"Generation d'une alerte");
					ex_env_alerte ( vl_Horodate,
									vl_TypeOuvrage,
									vl_NumOuvrage,
									vl_TypeAlerte,
									vg_Mode_Fonct,
									XZSC_22NumeroSiteLocal(),
									NULL );
				}
									
				// Envoi etat Alerte dans XDM_ETAT_ALERTE sur XDG_ETAT_ALERTE
				if ( ( vl_etatequipement == 0) )
				{
					sprintf ( pl_NomDG, "%s_%s",
							  XDG_ETAT_ALERTE,
							  pl_NomSite );

					if (!TipcSrvMsgWrite ( pl_NomDG,
										   XDM_IdentMsg(XDM_ETAT_ALERTE),
										   XDM_FLG_SRVMSGWRITE,
										   T_IPC_FT_INT4, vl_NumOuvrage,
										   T_IPC_FT_INT4, vl_IdAlerte,
										   T_IPC_FT_INT4, vl_etatalerte,
										   NULL ) )
					{
						XZST_03EcritureTrace(XZSTC_WARNING, "ALERTE : Envoi message XDM_ETAT_ALERTE non effectue." );
					}
					else
					{
						XZST_03EcritureTrace( XZSTC_INFO, "ALERTE : Envoi message XDM_ETAT_ALERTE <NomDG>%s_<NumEqt>%d_<NumAlerte>%d_<EtatAlerte>%d",
											  pl_NomDG,
											  vl_NumOuvrage,
											  vl_IdAlerte,
											  vl_etatalerte);
					}
				}	
			}
            break;

        /*A C'est un changement d'etat d'un equipement */
        case ENIX_REC_DISCEQT :
        case ENIX_REC_EQUIPEMENT :
			/*************************************************/
			/* traitement de la trame d'�tat d'un equipement */
			/*************************************************/
            XZST_03EcritureTrace( XZSTC_DEBUG1, "ETAT EQUIPEMENT : Debut");
            XZST_03EcritureTrace( XZSTC_DEBUG1, "ETAT EQUIPEMENT : vl_domaine %d, %d", vl_domaine, vl_domaine_socket);
            /* Recherche pour tout les equipements si il y a un evenement */
            switch( vl_domaine )
            {
                case 1 :
                    XZST_03EcritureTrace( XZSTC_DEBUG1, "ETAT EQUIPEMENT : Signalisation");
                    vl_Equipement = pl_SIG->ListeEquipement;
					pl_SIG->InfoRecue = XDC_OUI;
                    break;
                case 2 :
                    XZST_03EcritureTrace( XZSTC_DEBUG1, "ETAT EQUIPEMENT : Ventilation");
                    vl_Equipement = pl_VEN->ListeEquipement;
					pl_VEN->InfoRecue = XDC_OUI;
                    break;
                case 3 :
                    XZST_03EcritureTrace( XZSTC_DEBUG1, "ETAT EQUIPEMENT : Eclairage");
                    vl_Equipement = pl_ECL->ListeEquipement;
					pl_ECL->InfoRecue = XDC_OUI;
                    break;
                case 4 :
                    XZST_03EcritureTrace( XZSTC_DEBUG1, "ETAT EQUIPEMENT : Energie");
                    vl_Equipement = pl_ENR->ListeEquipement;
					pl_ENR->InfoRecue = XDC_OUI;
                    break;
                case 5 :
                case 6 :
                    XZST_03EcritureTrace( XZSTC_DEBUG1, "ETAT EQUIPEMENT : Exploitation");
                    vl_Equipement = pl_EXP->ListeEquipement;
					pl_EXP->InfoRecue = XDC_OUI;
                    break;
                case 7 :
                    XZST_03EcritureTrace( XZSTC_DEBUG1, "ETAT EQUIPEMENT : Locaux techniques");
                    vl_Equipement = pl_LTN->ListeEquipement;
					pl_LTN->InfoRecue = XDC_OUI;
                    break;
                case 8 :
                    XZST_03EcritureTrace( XZSTC_DEBUG1, "ETAT EQUIPEMENT : Systeme");
                    vl_Equipement = pl_SYS->ListeEquipement;
					pl_SYS->InfoRecue = XDC_OUI;
                    break;
                case 9 :
                    XZST_03EcritureTrace( XZSTC_DEBUG1, "ETAT EQUIPEMENT : Protection Incendie");
                    vl_Equipement = pl_INC->ListeEquipement;
					pl_INC->InfoRecue = XDC_OUI;
                    break;
                default :
                    XZST_03EcritureTrace( XZSTC_DEBUG1, "ETAT EQUIPEMENT : Domaine vl_domaine <%d> non trouve", vl_domaine);
                    vl_Equipement = NULL;
                    break;
            }
            
            for ( pl_Equipement =  vl_Equipement;
                  pl_Equipement != NULL; 
                  pl_Equipement =  pl_Equipement->Suivant )
            { 
                XZST_03EcritureTrace( XZSTC_DEBUG1, "ETAT EQUIPEMENT : Equipement courant type <%d> numero <%d> Etat <%d> Service <%d> equipement recherche <%d> ",
                                      pl_Equipement->TypeEquipement,
                                      pl_Equipement->NumeroEquipement,
                                      pl_Equipement->EtatEquipement,
                                      pl_Equipement->Etat_Service,
                                      vl_numequipement);
   
                if ( pl_Equipement->NumeroEquipement == vl_numequipement && pl_Equipement->TypeEquipement == vl_typeequipement)
                {
                    XZST_03EcritureTrace( XZSTC_DEBUG1, "ETAT EQUIPEMENT : Equipement trouve <%d> domaine <%d> Etat <%d> Service <%d>",
                                          pl_Equipement->NumeroEquipement,
                                          vl_domaine_socket,
                                          pl_Equipement->EtatEquipement,
                                          pl_Equipement->Etat_Service);
                    /* Calcul de l'etat de l'equipement */
                    pl_Equipement->EtatEquipement = vl_etatequipement;
                    pl_Equipement->Etat_Service   = vl_etattechnique;
                    pl_Equipement->Mode           = vl_mode;
                    pl_Equipement->EtatDisc       = vl_defaut;
					pl_Equipement->InfoRecue	  = XDC_OUI;
      
                    /* Envoi etat Equipement dans XDM_ETAT_EQUIPEMENT sur XDG_ETAT_EQUIPEMENT*/
                    sprintf ( pl_NomDG, "%s_%s",
                              XDG_ETAT_EQUIPEMENT,
                              pl_NomSite );
                    if ( vl_Etat_Service == 0 )
                    if (!TipcSrvMsgWrite ( pl_NomDG,
                                           XDM_IdentMsg(XDM_ETAT_EQUIPEMENT),
                                           XDM_FLG_SRVMSGWRITE,
                                           T_IPC_FT_INT4, vl_domaine_socket,
                                           T_IPC_FT_INT4, vl_TypeOuvrage,
                                           T_IPC_FT_INT4, vl_NumOuvrage,
                                           T_IPC_FT_INT4, pl_Equipement->TypeEquipement,
                                           T_IPC_FT_INT4, pl_Equipement->NumeroEquipement,
                                           T_IPC_FT_INT4, pl_Equipement->EtatEquipement,
                                           T_IPC_FT_INT4, pl_Equipement->Etat_Service,
                                           T_IPC_FT_INT4, pl_Equipement->Mode,
                                           T_IPC_FT_INT4, pl_Equipement->EtatDisc,
					   T_IPC_FT_INT2, vl_ModeRafale == 1 ? XDC_OUI : XDC_NON,
                                           NULL ) )
                    {
                        XZST_03EcritureTrace(XZSTC_DEBUG1, "ETAT EQUIPEMENT : Envoi message XDM_ETAT_EQUIPEMENT non effectue." );
                    }
                    else
                    {
                        XZST_03EcritureTrace( XZSTC_DEBUG1, "ETAT EQUIPEMENT : Envoi message XDM_ETAT_EQUIPEMENTT OK <NomDG>%s_<domaine>%d_<TypEqt>%d_<NumEqt>%d_<TypeEquipement>%d<NumEqt>%d_<EtatEqt>%d_<EtatDisc>%d<Mode>%d",
                                              pl_NomDG,
                                              vl_domaine_socket,
                                              vl_TypeOuvrage,
                                              vl_NumOuvrage,
                                              pl_Equipement->TypeEquipement,
                                              pl_Equipement->NumeroEquipement,
                                              pl_Equipement->EtatEquipement,
                                              pl_Equipement->EtatDisc,
											  pl_Equipement->Mode);
                    }
					
					/* Envoi de la discordance panneau si necessaire */
					if (vl_TypeEvt == ENIX_REC_DISCEQT)
					{
						if (vl_domaine_socket == XDC_EQT_SIG)
						{
	                        XZST_03EcritureTrace(XZSTC_DEBUG1, "ETAT EQUIPEMENT : Envoi de l'alarme discordance panneau" );
							ex_env_alarme ( vg_NomMachine,
											vl_Horodate,
											pl_Equipement->TypeEquipement,
											pl_Equipement->NumeroEquipement,
											XZAMC_DISC_PANNEAU,
											pl_Equipement->EtatDisc,
											NULL,
											XZSC_22NumeroSiteLocal());					
						}
					}
                }
            }
            break;
		case(ENIX_REC_DOMAINE):
			/********************************************/
			/* traitement de la trame d'�tat de domaine */
			/********************************************/
			XZST_03EcritureTrace( XZSTC_INFO, "Traitement de la trame d'etat de domaine");
			sprintf ( pl_NomDG, "%s_%s",
					  XDG_ETAT_DOMAINE,
					  pl_NomSite );
			switch(vl_domaine)
			{
			case(1):
				XZST_03EcritureTrace( XZSTC_INFO, "Domaine signalisation, parcours de la liste des ouvrages ");
				for ( pl_SIG = pg_debutSIG; pl_SIG != NULL; pl_SIG = pl_SIG->Suivant )
				{
					if (pl_SIG->Numero == vl_NumOuvrage && pl_SIG->TypeOuvrage == vl_TypeOuvrage)
					{
						pl_SIG->ModeDomaine = vl_mode;
						pl_SIG->DerniereSequence = vl_IdScenario;
						pl_SIG->InfoRecue	  = XDC_OUI;
						
	                    /* Envoi etat domaine dans XDM_ETAT_DOMAINE sur XDG_ETAT_DOMAINE */
	                    if (!TipcSrvMsgWrite ( pl_NomDG,
	                                           XDM_IdentMsg(XDM_ETAT_DOMAINE),
	                                           XDM_FLG_SRVMSGWRITE,
	                                           T_IPC_FT_INT4, pl_SIG->TypeOuvrage,
	                                           T_IPC_FT_INT4, pl_SIG->Numero,
	                                           T_IPC_FT_INT4, XDC_EQT_SIG,
	                                           T_IPC_FT_INT4, pl_SIG->ModeDomaine,
	                                           T_IPC_FT_INT4, pl_SIG->DerniereSequence,
	                                           NULL ) )
	                    {
	                        XZST_03EcritureTrace(XZSTC_WARNING, "ETAT EQUIPEMENT : Envoi message XDM_ETAT_DOMAINE non effectue." );
	                    }
	                    else
	                    {
	                        XZST_03EcritureTrace( XZSTC_DEBUG1, "ETAT EQUIPEMENT : Envoi message XDM_ETAT_DOMAINE <NomDG>%s_<TypeOuvrage>%d_<Numero>%d_<Domaine>%d_<ModeDomaine>%d_<DerniereSequence>%d",
	                                              pl_NomDG,
	                                              pl_SIG->TypeOuvrage,
	                                              pl_SIG->Numero,
	                                              XDC_EQT_SIG,
	                                              pl_SIG->ModeDomaine,
	                                              pl_SIG->DerniereSequence);
	                    }
						
					}
				}
				XZST_03EcritureTrace( XZSTC_INFO, "ETAT DOMAINE : Fin");
				break;
			case(2):
				XZST_03EcritureTrace( XZSTC_DEBUG1, "Domaine ventilation, parcours de la liste des ouvrages ");
				XZST_03EcritureTrace( XZSTC_DEBUG1, "numero recherche : %d, type recherche : %d ", vl_NumOuvrage, vl_TypeOuvrage);
				for ( pl_VEN = pg_debutVEN; pl_VEN != NULL; pl_VEN = pl_VEN->Suivant )
				{
					XZST_03EcritureTrace( XZSTC_DEBUG1, "numero : %d, type : %d ", pl_VEN->Numero, pl_VEN->TypeOuvrage);
					if (pl_VEN->Numero == vl_NumOuvrage && pl_VEN->TypeOuvrage == vl_TypeOuvrage)
					{
						pl_VEN->ModeDomaine = vl_mode;
						pl_VEN->DerniereSequence = vl_IdScenario;
						pl_VEN->InfoRecue	  = XDC_OUI;
						
	                    /* Envoi etat domaine dans XDM_ETAT_DOMAINE sur XDG_ETAT_DOMAINE */
	                    if (!TipcSrvMsgWrite ( pl_NomDG,
	                                           XDM_IdentMsg(XDM_ETAT_DOMAINE),
	                                           XDM_FLG_SRVMSGWRITE,
	                                           T_IPC_FT_INT4, pl_VEN->TypeOuvrage,
	                                           T_IPC_FT_INT4, pl_VEN->Numero,
	                                           T_IPC_FT_INT4, XDC_EQT_VEN,
	                                           T_IPC_FT_INT4, pl_VEN->ModeDomaine,
	                                           T_IPC_FT_INT4, pl_VEN->DerniereSequence,
	                                           NULL ) )
	                    {
	                        XZST_03EcritureTrace(XZSTC_WARNING, "ETAT EQUIPEMENT : Envoi message XDM_ETAT_DOMAINE non effectue." );
	                    }
	                    else
	                    {
	                        XZST_03EcritureTrace( XZSTC_DEBUG1, "ETAT EQUIPEMENT : Envoi message XDM_ETAT_DOMAINE <NomDG>%s_<TypeOuvrage>%d_<Numero>%d_<Domaine>%d_<ModeDomaine>%d_<DerniereSequence>%d",
	                                              pl_NomDG,
	                                              pl_VEN->TypeOuvrage,
	                                              pl_VEN->Numero,
	                                              XDC_EQT_VEN,
	                                              pl_VEN->ModeDomaine,
	                                              pl_VEN->DerniereSequence);
	                    }
						
					}
				}
				XZST_03EcritureTrace( XZSTC_INFO, "ETAT DOMAINE : Fin");
				break;		
			case(3):
				XZST_03EcritureTrace( XZSTC_INFO, "Domaine eclairage, parcours de la liste des ouvrages ");
				XZST_03EcritureTrace( XZSTC_INFO, "numero recherche : %d, type recherche : %d ", vl_NumOuvrage, vl_TypeOuvrage);
				for ( pl_ECL = pg_debutECL; pl_ECL != NULL; pl_ECL = pl_ECL->Suivant )
				{
					XZST_03EcritureTrace( XZSTC_INFO, "numero : %d, type : %d ", pl_ECL->Numero, pl_ECL->TypeOuvrage);
					if (pl_ECL->Numero == vl_NumOuvrage && pl_ECL->TypeOuvrage == vl_TypeOuvrage)
					{
						pl_ECL->ModeDomaine = vl_mode;
						pl_ECL->DerniereSequence = vl_IdScenario;
						pl_ECL->InfoRecue	  = XDC_OUI;
						
	                    /* Envoi etat domaine dans XDM_ETAT_DOMAINE sur XDG_ETAT_DOMAINE */
	                    if (!TipcSrvMsgWrite ( pl_NomDG,
	                                           XDM_IdentMsg(XDM_ETAT_DOMAINE),
	                                           XDM_FLG_SRVMSGWRITE,
	                                           T_IPC_FT_INT4, pl_ECL->TypeOuvrage,
	                                           T_IPC_FT_INT4, pl_ECL->Numero,
	                                           T_IPC_FT_INT4, XDC_EQT_ECL,
	                                           T_IPC_FT_INT4, pl_ECL->ModeDomaine,
	                                           T_IPC_FT_INT4, pl_ECL->DerniereSequence,
	                                           NULL ) )
	                    {
	                        XZST_03EcritureTrace(XZSTC_WARNING, "ETAT EQUIPEMENT : Envoi message XDM_ETAT_DOMAINE non effectue." );
	                    }
	                    else
	                    {
	                        XZST_03EcritureTrace( XZSTC_INFO, "ETAT EQUIPEMENT : Envoi message XDM_ETAT_DOMAINE <NomDG>%s_<TypeOuvrage>%d_<Numero>%d_<Domaine>%d_<ModeDomaine>%d_<DerniereSequence>%d",
	                                              pl_NomDG,
	                                              pl_ECL->TypeOuvrage,
	                                              pl_ECL->Numero,
	                                              XDC_EQT_ECL,
	                                              pl_ECL->ModeDomaine,
	                                              pl_ECL->DerniereSequence);
	                    }
						
					}
				}
				XZST_03EcritureTrace( XZSTC_INFO, "ETAT DOMAINE : Fin");
				break;								
			case(4):
				XZST_03EcritureTrace( XZSTC_INFO, "Domaine energie, parcours de la liste des ouvrages ");
				XZST_03EcritureTrace( XZSTC_INFO, "numero recherche : %d, type recherche : %d ", vl_NumOuvrage, vl_TypeOuvrage);
				for ( pl_ENR = pg_debutENR; pl_ENR != NULL; pl_ENR = pl_ENR->Suivant )
				{
					XZST_03EcritureTrace( XZSTC_INFO, "numero : %d, type : %d ", pl_ENR->Numero, pl_ENR->TypeOuvrage);
					if (pl_ENR->Numero == vl_NumOuvrage && pl_ENR->TypeOuvrage == vl_TypeOuvrage)
					{
						pl_ENR->ModeDomaine = vl_mode;
						pl_ENR->DerniereSequence = vl_IdScenario;
						pl_ENR->InfoRecue	  = XDC_OUI;
						
	                    /* Envoi etat domaine dans XDM_ETAT_DOMAINE sur XDG_ETAT_DOMAINE */
	                    if (!TipcSrvMsgWrite ( pl_NomDG,
	                                           XDM_IdentMsg(XDM_ETAT_DOMAINE),
	                                           XDM_FLG_SRVMSGWRITE,
	                                           T_IPC_FT_INT4, pl_ENR->TypeOuvrage,
	                                           T_IPC_FT_INT4, pl_ENR->Numero,
	                                           T_IPC_FT_INT4, XDC_EQT_ENR,
	                                           T_IPC_FT_INT4, pl_ENR->ModeDomaine,
	                                           T_IPC_FT_INT4, pl_ENR->DerniereSequence,
	                                           NULL ) )
	                    {
	                        XZST_03EcritureTrace(XZSTC_WARNING, "ETAT EQUIPEMENT : Envoi message XDM_ETAT_DOMAINE non effectue." );
	                    }
	                    else
	                    {
	                        XZST_03EcritureTrace( XZSTC_DEBUG1, "ETAT EQUIPEMENT : Envoi message XDM_ETAT_DOMAINE <NomDG>%s_<TypeOuvrage>%d_<Numero>%d_<Domaine>%d_<ModeDomaine>%d_<DerniereSequence>%d",
	                                              pl_NomDG,
	                                              pl_ENR->TypeOuvrage,
	                                              pl_ENR->Numero,
	                                              XDC_EQT_ENR,
	                                              pl_ENR->ModeDomaine,
	                                              pl_ENR->DerniereSequence);
	                    }
						
					}
				}
				XZST_03EcritureTrace( XZSTC_INFO, "ETAT DOMAINE : Fin");
				break;								
			case(5):
			case(6):
				XZST_03EcritureTrace( XZSTC_INFO, "Domaine exploitation, parcours de la liste des ouvrages ");
				XZST_03EcritureTrace( XZSTC_INFO, "numero recherche : %d, type recherche : %d ", vl_NumOuvrage, vl_TypeOuvrage);
				for ( pl_EXP = pg_debutEXP; pl_EXP != NULL; pl_EXP = pl_EXP->Suivant )
				{
					XZST_03EcritureTrace( XZSTC_INFO, "numero : %d, type : %d ", pl_EXP->Numero, pl_EXP->TypeOuvrage);
					if (pl_EXP->Numero == vl_NumOuvrage && pl_EXP->TypeOuvrage == vl_TypeOuvrage)
					{
						pl_EXP->ModeDomaine = vl_mode;
						pl_EXP->DerniereSequence = vl_IdScenario;
						pl_EXP->InfoRecue	  = XDC_OUI;
						
	                    /* Envoi etat domaine dans XDM_ETAT_DOMAINE sur XDG_ETAT_DOMAINE */
	                    if (!TipcSrvMsgWrite ( pl_NomDG,
	                                           XDM_IdentMsg(XDM_ETAT_DOMAINE),
	                                           XDM_FLG_SRVMSGWRITE,
	                                           T_IPC_FT_INT4, pl_EXP->TypeOuvrage,
	                                           T_IPC_FT_INT4, pl_EXP->Numero,
	                                           T_IPC_FT_INT4, XDC_EQT_EXP,
	                                           T_IPC_FT_INT4, pl_EXP->ModeDomaine,
	                                           T_IPC_FT_INT4, pl_EXP->DerniereSequence,
	                                           NULL ) )
	                    {
	                        XZST_03EcritureTrace(XZSTC_WARNING, "ETAT EQUIPEMENT : Envoi message XDM_ETAT_DOMAINE non effectue." );
	                    }
	                    else
	                    {
	                        XZST_03EcritureTrace( XZSTC_INFO, "ETAT EQUIPEMENT : Envoi message XDM_ETAT_DOMAINE <NomDG>%s_<TypeOuvrage>%d_<Numero>%d_<Domaine>%d_<ModeDomaine>%d_<DerniereSequence>%d",
	                                              pl_NomDG,
	                                              pl_EXP->TypeOuvrage,
	                                              pl_EXP->Numero,
	                                              XDC_EQT_EXP,
	                                              pl_EXP->ModeDomaine,
	                                              pl_EXP->DerniereSequence);
	                    }
						
					}
				}
				XZST_03EcritureTrace( XZSTC_INFO, "ETAT DOMAINE : Fin");
				break;								
			case(7):
				XZST_03EcritureTrace( XZSTC_INFO, "Domaine locaux techniques, parcours de la liste des ouvrages ");
				XZST_03EcritureTrace( XZSTC_INFO, "numero recherche : %d, type recherche : %d ", vl_NumOuvrage, vl_TypeOuvrage);
				for ( pl_LTN = pg_debutLTN; pl_LTN != NULL; pl_LTN = pl_LTN->Suivant )
				{
					XZST_03EcritureTrace( XZSTC_INFO, "numero : %d, type : %d ", pl_LTN->Numero, pl_LTN->TypeOuvrage);
					if (pl_LTN->Numero == vl_NumOuvrage && pl_LTN->TypeOuvrage == vl_TypeOuvrage)
					{
						pl_LTN->ModeDomaine = vl_mode;
						pl_LTN->DerniereSequence = vl_IdScenario;
						pl_LTN->InfoRecue	  = XDC_OUI;
						
	                    /* Envoi etat domaine dans XDM_ETAT_DOMAINE sur XDG_ETAT_DOMAINE */
	                    if (!TipcSrvMsgWrite ( pl_NomDG,
	                                           XDM_IdentMsg(XDM_ETAT_DOMAINE),
	                                           XDM_FLG_SRVMSGWRITE,
	                                           T_IPC_FT_INT4, pl_LTN->TypeOuvrage,
	                                           T_IPC_FT_INT4, pl_LTN->Numero,
	                                           T_IPC_FT_INT4, XDC_EQT_LTN,
	                                           T_IPC_FT_INT4, pl_LTN->ModeDomaine,
	                                           T_IPC_FT_INT4, pl_LTN->DerniereSequence,
	                                           NULL ) )
	                    {
	                        XZST_03EcritureTrace(XZSTC_WARNING, "ETAT EQUIPEMENT : Envoi message XDM_ETAT_DOMAINE non effectue." );
	                    }
	                    else
	                    {
	                        XZST_03EcritureTrace( XZSTC_INFO, "ETAT EQUIPEMENT : Envoi message XDM_ETAT_DOMAINE <NomDG>%s_<TypeOuvrage>%d_<Numero>%d_<Domaine>%d_<ModeDomaine>%d_<DerniereSequence>%d",
	                                              pl_NomDG,
	                                              pl_LTN->TypeOuvrage,
	                                              pl_LTN->Numero,
	                                              XDC_EQT_LTN,
	                                              pl_LTN->ModeDomaine,
	                                              pl_LTN->DerniereSequence);
	                    }
						
					}
				}
				XZST_03EcritureTrace( XZSTC_INFO, "ETAT DOMAINE : Fin");
				break;	
			case(8):
				XZST_03EcritureTrace( XZSTC_INFO, "Domaine systeme, parcours de la liste des ouvrages ");
				XZST_03EcritureTrace( XZSTC_INFO, "numero recherche : %d, type recherche : %d ", vl_NumOuvrage, vl_TypeOuvrage);
				for ( pl_SYS = pg_debutSYS; pl_SYS != NULL; pl_SYS = pl_SYS->Suivant )
				{
					XZST_03EcritureTrace( XZSTC_INFO, "numero : %d, type : %d ", pl_SYS->Numero, pl_SYS->TypeOuvrage);
					if (pl_SYS->Numero == vl_NumOuvrage && pl_SYS->TypeOuvrage == vl_TypeOuvrage)
					{
						pl_SYS->ModeDomaine = vl_mode;
						pl_SYS->DerniereSequence = vl_IdScenario;
						pl_SYS->InfoRecue	  = XDC_OUI;
						
	                    /* Envoi etat domaine dans XDM_ETAT_DOMAINE sur XDG_ETAT_DOMAINE */
	                    if (!TipcSrvMsgWrite ( pl_NomDG,
	                                           XDM_IdentMsg(XDM_ETAT_DOMAINE),
	                                           XDM_FLG_SRVMSGWRITE,
	                                           T_IPC_FT_INT4, pl_SYS->TypeOuvrage,
	                                           T_IPC_FT_INT4, pl_SYS->Numero,
	                                           T_IPC_FT_INT4, XDC_EQT_SYS,
	                                           T_IPC_FT_INT4, pl_SYS->ModeDomaine,
	                                           T_IPC_FT_INT4, pl_SYS->DerniereSequence,
	                                           NULL ) )
	                    {
	                        XZST_03EcritureTrace(XZSTC_WARNING, "ETAT EQUIPEMENT : Envoi message XDM_ETAT_DOMAINE non effectue." );
	                    }
	                    else
	                    {
	                        XZST_03EcritureTrace( XZSTC_DEBUG1, "ETAT EQUIPEMENT : Envoi message XDM_ETAT_DOMAINE <NomDG>%s_<TypeOuvrage>%d_<Numero>%d_<Domaine>%d_<ModeDomaine>%d_<DerniereSequence>%d",
	                                              pl_NomDG,
	                                              pl_SYS->TypeOuvrage,
	                                              pl_SYS->Numero,
	                                              XDC_EQT_SYS,
	                                              pl_SYS->ModeDomaine,
	                                              pl_SYS->DerniereSequence);
	                    }
						
					}
				}
				XZST_03EcritureTrace( XZSTC_INFO, "ETAT DOMAINE : Fin");
				break;
			}
			

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
    int                      vl_IndCnx = 0,
                             vl_LgMsg  = 0;
    XDY_Horodate             vl_Horodate;   
    XZSMT_Horodate           pl_StrHorodate;
    XZEXT_MSG_SOCKET         pl_Msg;
    XDY_Mot                  vl_NumSeq,
                             vl_TypeEqt,
							 vl_Domaine,
                             vl_NumEqt;
    ENIX_DONNEES_Sequence *  pl_Sequence;
    ENIX_DONNEES_Tube *      pl_Tube;
    ENIX_DONNEES_Echangeur * pl_Ech;
    int                      vl_Delai;
    /* AAZ 03/09/2007 */
    ENIX_DONNEES_SIG *       pl_SIG;
    ENIX_DONNEES_ECL *       pl_ECL;
    ENIX_DONNEES_VEN *       pl_VEN;
    ENIX_DONNEES_ENR *       pl_ENR;
    ENIX_DONNEES_INC *       pl_INC;
    ENIX_DONNEES_EXP *       pl_EXP;
    
    XZST_03EcritureTrace( XZSTC_DEBUG1, "IN : et_traitement_timers");

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
   
    sscanf ( &pl_Msg[strlen(XZEXC_MSG_REVEIL)], " %lf %d Nice%hd_%hd_%hd_%hd ",
             &vl_Horodate,
             &vl_Delai,
             &vl_TypeEqt,
             &vl_NumEqt,
             &vl_NumSeq,
			 &vl_Domaine);
			 
	XZST_03EcritureTrace( XZSTC_DEBUG1, "Reveil : TypeEqt = %d, NumEqt = %d, NumSeq = %d, Domaine=%d",
			vl_TypeEqt,
			vl_NumEqt,
			vl_NumSeq,
			vl_Domaine);

    if ( vl_Delai == ENICC_REVEIL_15MN )
    {

        /* Construction du message de demande de relecture */
        sprintf ( pl_Msg, "%d|1|%s", ENIX_CMD_DEMANDE_INIT, XZEXC_FIN_MSG_SOCK_WIZCON );

        /* Envoie de la demande d'init */
        sem_wait(&vg_semaphore); 
        if ( write ( vg_SockTWWI0, pl_Msg, strlen (pl_Msg) ) == XDC_NOK )
        { 
            /* Ecriture trace impossible d'emettre une demande d'arret */
            XZST_03EcritureTrace(XZSTC_WARNING, " Impossible d'emettre une demande d'init vers Wizcon principal." );
        }
        sem_post(&vg_semaphore); 

        sprintf ( pl_Msg, "%d|1|%s", ENIX_CMD_DEMANDE_INIT, XZEXC_FIN_MSG_SOCK_WIZCON );
        sem_wait(&vg_semaphore); 
        if ( write ( vg_SockTWWI1, pl_Msg, strlen (pl_Msg) ) == XDC_NOK )
        { 
            /* Ecriture trace impossible d'emettre une demande d'arret */
            XZST_03EcritureTrace(XZSTC_WARNING, " Impossible d'emettre une demande d'init vers Wizcon secondaire." );
        }
        sem_post(&vg_semaphore); 
        return (XDC_OK);

    }  

    switch( vl_Domaine )
    {
        case XDC_EQT_SIG :

            /*A Recherche dans la liste eqt du domaine Signalisation */
            for ( pl_SIG =  pg_debutSIG;
                  pl_SIG != NULL;
                  pl_SIG =  pl_SIG->Suivant )
            {
                if ( pl_SIG->Numero ==  vl_NumEqt && pl_SIG->TypeOuvrage == vl_TypeEqt && pl_SIG->InfoRecue == XDC_OUI)
                {
                    /* Recherche de la sequence concernee */
                    for ( pl_Sequence =  pl_SIG->ListeSequence;
                          pl_Sequence != NULL;
                          pl_Sequence =  pl_Sequence->Suivant )
                    {
					
						XZST_03EcritureTrace(XZSTC_INFO, "pl_Sequence->NumSequence = %d, pl_Sequence->NumAction = %d, pl_Sequence->AttenteEtat = %d, plSequence->EtatDisc = %d", pl_Sequence->NumSequence, pl_Sequence->NumAction, pl_Sequence->AttenteEtat, pl_Sequence->EtatDisc);
                        if ( (pl_Sequence->NumSequence == vl_NumSeq) &&
                             (pl_Sequence->NumAction   != 0)         &&
                             (pl_Sequence->AttenteEtat == XDC_OUI) && 
							  pl_Sequence->EtatDisc == 0)
                        {
                            /* Appel de XZAT40 avec XDC_OK et NumAction */
							XZST_03EcritureTrace ( XZSTC_DEBUG1 ,"Appel de xzat40_CompteRendu_SIG (numact =%d) : %s", pl_Sequence->NumAction, pl_Sequence->AttenteEtat == XDC_NON ? "OK" : "NOK");
							
							xzat40_CompteRendu_SIG ( pl_Sequence->NumAction,
													 vl_Horodate,
													 XDC_OK);
							pl_Sequence->AttenteEtat = XDC_NON;
							pl_Sequence->EtatDisc = 1;

                            XZSM_11ConversionHorodate( vl_Horodate,
                                                       &pl_StrHorodate );

							XZST_03EcritureTrace ( XZSTC_INFO ,"Generation d'une alerte de discordance de sequence domaine Signalisation (timeout)");
                            ex_env_alerte ( vl_Horodate,
                                            pl_SIG->TypeOuvrage,
                                            pl_SIG->Numero,
                                            XZAAC_AFF_DIF,
                                            vg_Mode_Fonct,
											XZSC_22NumeroSiteLocal(),
                                            pl_Sequence->NumSequence,
                                            NULL );

							XZST_03EcritureTrace( XZSTC_DEBUG1,
                                                  "Envoi CR OK pour Eqt SIG:%d Cmd:%d Seq:%d H:%s.",
                                                  pl_SIG->Numero,
                                                  pl_Sequence->NumAction,
                                                  pl_Sequence->NumSequence,
                                                  pl_StrHorodate );
                            et_wfic_derniere_seq( XDC_EQT_SIG );
                            break;
                        }
                    }
		    XZST_03EcritureTrace ( XZSTC_DEBUG1 ,"fin FOR");
                }
            }
	   break;

        case XDC_EQT_ECL :
            /*A Recherche dans la liste eqt du domaine Eclairage */
            for ( pl_ECL =  pg_debutECL;
                  pl_ECL != NULL;
                  pl_ECL =  pl_ECL->Suivant )
            {
                if ( pl_ECL->Numero ==  vl_NumEqt && pl_ECL->TypeOuvrage == vl_TypeEqt && pl_ECL->InfoRecue == XDC_OUI)
                {
                    /* Recherche de la sequence concernee */
                    for ( pl_Sequence =  pl_ECL->ListeSequence;
                          pl_Sequence != NULL;
                          pl_Sequence =  pl_Sequence->Suivant )
                    {
                        if ( (pl_Sequence->NumSequence == vl_NumSeq) &&
                             (pl_Sequence->NumAction   != 0)         &&
                             (pl_Sequence->AttenteEtat == XDC_OUI) && 
							  pl_Sequence->EtatDisc == 0)
                        {
                            /* Appel de XZAT41 avec XDC_OK et NumAction */
                            xzat41_CompteRendu_ECL ( pl_Sequence->NumAction,
                                                     vl_Horodate,
                                                     XDC_OK
													 );
							pl_Sequence->AttenteEtat = XDC_NON;
							pl_Sequence->EtatDisc = 1;

                            XZSM_11ConversionHorodate ( vl_Horodate,
                                                        &pl_StrHorodate );

                            XZST_03EcritureTrace( XZSTC_DEBUG1,
                                                  "Envoi CR OK pour Eqt ECL:%d Cmd:%d Seq:%d H:%s.",
                                                  pl_ECL->Numero,
                                                  pl_Sequence->NumAction,
                                                  pl_Sequence->NumSequence,
                                                  pl_StrHorodate );

							XZST_03EcritureTrace ( XZSTC_INFO ,"Generation d'une alerte de discordance de sequence domaine Eclairage (timeout)");
                            ex_env_alerte ( vl_Horodate,
                                            pl_ECL->TypeOuvrage,
                                            pl_ECL->Numero,
                                            XZAAC_AFF_DIF,
                                            vg_Mode_Fonct,
											XZSC_22NumeroSiteLocal(),
                                            pl_Sequence->NumSequence,
                                            NULL );


							et_wfic_derniere_seq( XDC_EQT_ECL );

                            break;
                        }
                    }
                }
            }
	    break;

        case XDC_EQT_VEN :
            /*A Recherche dans la liste eqt du domaine Ventilation */
            for ( pl_VEN =  pg_debutVEN;
                  pl_VEN != NULL;
                  pl_VEN =  pl_VEN->Suivant )
            {
                if ( pl_VEN->Numero ==  vl_NumEqt && pl_VEN->TypeOuvrage == vl_TypeEqt && pl_VEN->InfoRecue == XDC_OUI)
                {
                    /* Recherche de la sequence concernee */
                    for ( pl_Sequence =  pl_VEN->ListeSequence;
                          pl_Sequence != NULL;
                          pl_Sequence =  pl_Sequence->Suivant )
                    {
                        if ( (pl_Sequence->NumSequence == vl_NumSeq) &&
                             (pl_Sequence->NumAction   != 0)         &&
                             (pl_Sequence->AttenteEtat == XDC_OUI)&& 
							  pl_Sequence->EtatDisc == 0)
                        {
                            /* Appel de XZAT42 avec XDC_OK et NumAction */
                            xzat42_CompteRendu_VEN ( pl_Sequence->NumAction,
                                                     vl_Horodate,
                                                     XDC_OK
													 );
							pl_Sequence->AttenteEtat = XDC_NON;
							pl_Sequence->EtatDisc = 1;

                            XZSM_11ConversionHorodate ( vl_Horodate,
                                                        &pl_StrHorodate );
														
							XZST_03EcritureTrace ( XZSTC_INFO ,"Generation d'une alerte de discordance de sequence domaine Ventilation (timeout)");
                            ex_env_alerte ( vl_Horodate,
                                            pl_VEN->TypeOuvrage,
                                            pl_VEN->Numero,
                                            XZAAC_AFF_DIF,
                                            vg_Mode_Fonct,
											XZSC_22NumeroSiteLocal(),
                                            pl_Sequence->NumSequence,
                                            NULL );
														
                            XZST_03EcritureTrace( XZSTC_DEBUG1,
                                                  "Envoi CR OK pour Eqt VEN:%d Cmd:%d Seq:%d H:%s.",
                                                  pl_VEN->Numero,
                                                  pl_Sequence->NumAction,
                                                  pl_Sequence->NumSequence,
                                                  pl_StrHorodate );

                            et_wfic_derniere_seq( XDC_EQT_VEN );

                            break;
                        }
                    }
                }
            }
	    break;

        case XDC_EQT_ENR :
            /*A Recherche dans la liste eqt du domaine Energie */
            for ( pl_ENR =  pg_debutENR;
                  pl_ENR != NULL;
                  pl_ENR =  pl_ENR->Suivant )
            {
                if ( pl_ENR->Numero ==  vl_NumEqt && pl_ENR->TypeOuvrage == vl_TypeEqt && pl_ENR->InfoRecue == XDC_OUI)
                {
                    /* Recherche de la sequence concernee */
                    for ( pl_Sequence =  pl_ENR->ListeSequence;
                          pl_Sequence != NULL;
                          pl_Sequence =  pl_Sequence->Suivant )
                    {
                        if ( (pl_Sequence->NumSequence == vl_NumSeq) &&
                             (pl_Sequence->NumAction   != 0)         &&
                             (pl_Sequence->AttenteEtat == XDC_OUI) && 
							  pl_Sequence->EtatDisc == 0)
                        {
                            /* Appel de XZAT43 avec XDC_OK et NumAction */
                            xzat43_CompteRendu_ENR ( pl_Sequence->NumAction,
                                                     vl_Horodate,
                                                     XDC_OK
													 );
							pl_Sequence->AttenteEtat = XDC_NON;
							pl_Sequence->EtatDisc = 1;

                            XZSM_11ConversionHorodate ( vl_Horodate,
                                                        &pl_StrHorodate );

                            XZST_03EcritureTrace( XZSTC_DEBUG1,
                                                  "Envoi CR OK pour Eqt ENR:%d Cmd:%d Seq:%d H:%s.",
                                                  pl_ENR->Numero,
                                                  pl_Sequence->NumAction,
                                                  pl_Sequence->NumSequence,
                                                  pl_StrHorodate );

							XZST_03EcritureTrace ( XZSTC_INFO ,"Generation d'une alerte de discordance de sequence domaine Energie (timeout)");
                            ex_env_alerte ( vl_Horodate,
                                            pl_ENR->TypeOuvrage,
                                            pl_ENR->Numero,
                                            XZAAC_AFF_DIF,
                                            vg_Mode_Fonct,
											XZSC_22NumeroSiteLocal(),
                                            pl_Sequence->NumSequence,
                                            NULL );
												  
                            et_wfic_derniere_seq( XDC_EQT_ENR );

                            break;
                        }
                    }
                }
            }
	    break;

        case XDC_EQT_INC :

            /*A Recherche dans la liste eqt du domaine Protection incendie */
            for ( pl_INC =  pg_debutINC;
                  pl_INC != NULL;
                  pl_INC =  pl_INC->Suivant )
            {
                if ( pl_INC->Numero ==  vl_NumEqt && pl_INC->TypeOuvrage == vl_TypeEqt && pl_ENR->InfoRecue == XDC_OUI)
                {
                    /* Recherche de la sequence concernee */
                    for ( pl_Sequence =  pl_INC->ListeSequence;
                          pl_Sequence != NULL;
                          pl_Sequence =  pl_Sequence->Suivant )
                    {
                        if ( (pl_Sequence->NumSequence == vl_NumSeq) &&
                             (pl_Sequence->NumAction   != 0)         &&
                             (pl_Sequence->AttenteEtat == XDC_OUI) && 
							  pl_Sequence->EtatDisc == 0)
                        {
                            /* Appel de XZAT44 avec XDC_OK et NumAction */
                            xzat44_CompteRendu_INC ( pl_Sequence->NumAction,
                                                     vl_Horodate,
                                                     XDC_OK
													 );
							pl_Sequence->AttenteEtat = XDC_NON;
							pl_Sequence->EtatDisc = 1;
							
                            XZSM_11ConversionHorodate ( vl_Horodate,
                                                        &pl_StrHorodate );

							XZST_03EcritureTrace ( XZSTC_INFO ,"Generation d'une alerte de discordance de sequence domaine Incendie (timeout)");
                            ex_env_alerte ( vl_Horodate,
                                            pl_INC->TypeOuvrage,
                                            pl_INC->Numero,
                                            XZAAC_AFF_DIF,
                                            vg_Mode_Fonct,
											XZSC_22NumeroSiteLocal(),
                                            pl_Sequence->NumSequence,
                                            NULL );
														
                            XZST_03EcritureTrace( XZSTC_DEBUG1,
                                                  "Envoi CR OK pour INC:%d Cmd:%d Seq:%d H:%s.",
                                                  pl_INC->Numero,
                                                  pl_Sequence->NumAction,
                                                  pl_Sequence->NumSequence,
                                                  pl_StrHorodate );

																			  
                            et_wfic_derniere_seq( XDC_EQT_INC );

                            break;
                        }
                    }
                }
            }
	    break;
            
		/* ABE 26-03-2013 : gestion du retour � la normale avec ajout de sc�narios pour le domaine exploitation */
        case XDC_EQT_EXP :
            /*A Recherche dans la liste eqt du domaine Exploitation */
            for ( pl_EXP =  pg_debutEXP;
                  pl_EXP != NULL;
                  pl_EXP =  pl_EXP->Suivant )
            {
                if ( pl_EXP->Numero ==  vl_NumEqt && pl_EXP->TypeOuvrage == vl_TypeEqt && pl_EXP->InfoRecue == XDC_OUI)
                {
                    /* Recherche de la sequence concernee */
                    for ( pl_Sequence =  pl_EXP->ListeSequence;
                          pl_Sequence != NULL;
                          pl_Sequence =  pl_Sequence->Suivant )
                    {
                        if ( (pl_Sequence->NumSequence == vl_NumSeq) &&
                             (pl_Sequence->NumAction   != 0)         &&
                             (pl_Sequence->AttenteEtat == XDC_OUI)&& 
							  pl_Sequence->EtatDisc == 0)
                        {
                            /* Appel de XZAT42 avec XDC_OK et NumAction */
                            xzat45_CompteRendu_EXP ( pl_Sequence->NumAction,
                                                     vl_Horodate,
                                                     XDC_OK
													 );
							pl_Sequence->AttenteEtat = XDC_NON;
							pl_Sequence->EtatDisc = 1;

                            XZSM_11ConversionHorodate ( vl_Horodate,
                                                        &pl_StrHorodate );
														
							XZST_03EcritureTrace ( XZSTC_INFO ,"Generation d'une alerte de discordance de sequence domaine Exploitation (timeout)");
                            ex_env_alerte ( vl_Horodate,
                                            pl_EXP->TypeOuvrage,
                                            pl_EXP->Numero,
                                            XZAAC_AFF_DIF,
                                            vg_Mode_Fonct,
											XZSC_22NumeroSiteLocal(),
                                            pl_Sequence->NumSequence,
                                            NULL );
														
                            XZST_03EcritureTrace( XZSTC_DEBUG1,
                                                  "Envoi CR OK pour Eqt EXP:%d Cmd:%d Seq:%d H:%s.",
                                                  pl_EXP->Numero,
                                                  pl_Sequence->NumAction,
                                                  pl_Sequence->NumSequence,
                                                  pl_StrHorodate );

                            et_wfic_derniere_seq( XDC_EQT_EXP );

                            break;
                        }
                    }
                }
            }
            
    }
    XZST_03EcritureTrace( XZSTC_DEBUG1,"SORTIE");
    return ( XDC_OK );
}
