/*E*/
/*Fichier :  @(#)enix_ctl.c	1.3      Release : 1.3        Date : 05/17/10
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TENIX * FICHIER enix_ctl.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module de gestion cyclique de la tache TENIX, gtc Nice
*
------------------------------------------------------
* HISTORIQUE :
*
* AAZ       version 1.1 03 Sept 2007    : GTC renovee DEM 664
* JBL		13 Oct 2008 : Suppression code inutilise (stockage interne alarmes et alertes)
------------------------------------------------------*/
 
/* fichiers inclus */
#include <xzsem.h>

#include "enix_ctl.h"

/* definitions  de constantes */

/* definitions  de types locaux */

/* definitions  de macro locales */

/* declaration  de variables locales */

static char *version = "@(#)enix_ctl.c	1.3 05/17/10      : enix_ctl" ;

/* declarations de fonctions internes */

/* definitions  de fonctions externes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Permet selon message socket recu de tesrv 
*    - soit de terminer la tache
*    - soit de configurer la tache
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void et_controle ( int             va_NumSock,
                   XDY_NomMachine  pa_NomMachine )

/*
* ARGUMENTS EN ENTREE :
*   va_NumSock          : Numero de la socket de TESRV
*   pa_NomMachine       : Nom de la machine
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
*   Lecture du message socket
*   Si message d'init alors lancer la fonction et_init
*   Si message d'arret alors lancer la fonction et_sortir
*
------------------------------------------------------*/
{
    static char          *version = "@(#)enix_ctl.c	1.3 05/17/10      : et_controle" ;

    int                  vl_IndCnx = 0,
                         vl_LgMsg  = 0;
    XZEXT_MSG_SOCKET     pl_Msg;

    XZST_03EcritureTrace( XZSTC_INFO, "IN : et_controle");

    /*A
     *  Lecture du message recu dans la socket appelante
     */
         
    if ( ex_lire_msg ( va_NumSock, pl_Msg, &vl_LgMsg, &vg_SockMask, &vl_IndCnx ) != XDC_OK )
    {
        /* Ecriture trace Erreur lecture message socket. */
        XZST_03EcritureTrace( XZSTC_WARNING, " Erreur lecture message socket. " );
    }   
    else
    {
        /*A Si il s'agit d'une deconnexion */
        if ( vl_IndCnx == XZEXC_DECONNEC )
        {
            /*A Arreter la tache par appel de et_sortir */
            et_sortir ();
        }
   
        /* Si le message est correct  */
        if ( vl_LgMsg != 0 ) 
        {
            /*A Si il s'agit d'une demande d'arret */
            if ( strstr ( pl_Msg, XZEXC_DEMANDE_ARRET ) != NULL )
            {
                /*A Arreter la tache par appel de et_sortir */
                et_sortir ();
            }

            /*A Si il s'agit d'une demande d'Init */
            if ( strstr ( pl_Msg, XZEXC_DEMANDE_INIT ) != NULL )
            {
                /*A Reconfiguration de la tache par appel de et_init */
                et_init ( pa_NomMachine );
            }
        }
    }
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Effectue l'initialisation de la tache
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void  et_init ( XDY_NomMachine  pa_NomMachine )

/*
* ARGUMENTS EN ENTREE :
*
*   pa_NomMachine       : Nom de la machine
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
*   Appel de ex_mini
*   Lecture des fichier de configuration 
*   Remplissage des structures de donnees
*
------------------------------------------------------*/
{
    static char                  *version = "@(#)enix_ctl.c	1.3 05/17/10      : et_init" ;
    XZEXT_MSG_SOCKET             pl_message;


    XZST_03EcritureTrace( XZSTC_INFO, "IN : et_init");

    /*A
     * Lecture des fichiers de configuration et remplissage des structure de donnees 
     */

    /*A Configuration des tubes et echangeurs (appel de la fonction et_lire_config_GTCNice) */
    et_lire_config_GTCNice  ( pa_NomMachine );
	
    /*A Configuration des Sequence (appel de la fonction et_lire_config_Sequence ) */
    et_lire_config_Sequence ( pa_NomMachine );

    /*A Configuration des Capteurs et Panneaux (appel de la fonction et_lire_config_Capt_Pan ) */
    /* AAZ 03/09/2007 */

    /*A Configuration des equipements (appel de la fonction et_lire_config_Equipement ) */
    et_lire_config_Equipement  ( pa_NomMachine );

    /*A Appel  xzat26_Init_Etats_Eqts pour Initialisation des etats equipement (HS ou Actif) */
    xzat26_Init_Etats_Eqts ( XDC_EQT_TUB, XDC_ETAT_TOUS_EQT, pa_NomMachine );
    xzat26_Init_Etats_Eqts ( XDC_EQT_ECH, XDC_ETAT_TOUS_EQT, pa_NomMachine );
    xzat26_Init_Etats_Eqts ( XDC_EQT_PAN, XDC_ETAT_TOUS_EQT, pa_NomMachine );
    xzat26_Init_Etats_Eqts ( XDC_EQT_CPT, XDC_ETAT_TOUS_EQT, pa_NomMachine );

    /* AAZ 03 sept 2007 ajout SIG, ECL, VEN, ENR, INC, EXP et SYS */
    xzat26_Init_Etats_Eqts ( XDC_EQT_SIG, XDC_ETAT_TOUS_EQT, pa_NomMachine );
    xzat26_Init_Etats_Eqts ( XDC_EQT_ECL, XDC_ETAT_TOUS_EQT, pa_NomMachine );
    xzat26_Init_Etats_Eqts ( XDC_EQT_VEN, XDC_ETAT_TOUS_EQT, pa_NomMachine );
    xzat26_Init_Etats_Eqts ( XDC_EQT_ENR, XDC_ETAT_TOUS_EQT, pa_NomMachine );
    xzat26_Init_Etats_Eqts ( XDC_EQT_INC, XDC_ETAT_TOUS_EQT, pa_NomMachine );
    xzat26_Init_Etats_Eqts ( XDC_EQT_EXP, XDC_ETAT_TOUS_EQT, pa_NomMachine );
    xzat26_Init_Etats_Eqts ( XDC_EQT_SYS, XDC_ETAT_TOUS_EQT, pa_NomMachine );
    xzat26_Init_Etats_Eqts ( XDC_EQT_LTN, XDC_ETAT_TOUS_EQT, pa_NomMachine );

    /* AAZ 03 sept 2007 ajout EQT */
    /*  xzat26_Init_Etats_Eqts ( XDC_EQT_EQT, XDC_ETAT_TOUS_EQT, pa_NomMachine ); */

    /*A
     * Envoie du message de demande d'init donnees 
     */

    /* Construction du message d'init */
    sprintf ( pl_message, "%d|1|%s", ENIX_CMD_DEMANDE_INIT, XZEXC_FIN_MSG_SOCK_WIZCON );

    /* Envoie de la demande d'init */
    sem_wait(&vg_semaphore); 
    if ( write ( vg_SockTWWI0, pl_message, strlen (pl_message) ) == XDC_NOK )
    { 
        /* Ecriture trace impossible d'emettre une demande d'arret */
        XZST_03EcritureTrace(XZSTC_WARNING, " Impossible d'emettre une demande d'init vers maitre Wizcon." );
    }
    sem_post(&vg_semaphore); 

    /* Construction du message d'init */
    sprintf ( pl_message, "%d|1|%s", ENIX_CMD_DEMANDE_INIT, XZEXC_FIN_MSG_SOCK_WIZCON );

    /* Envoie de la demande d'init */
    sem_wait(&vg_semaphore); 
    if ( write ( vg_SockTWWI1, pl_message, strlen (pl_message) ) == XDC_NOK )
    { 
        /* Ecriture trace impossible d'emettre une demande d'arret */
        XZST_03EcritureTrace(XZSTC_WARNING, " Impossible d'emettre une demande d'init vers esclave Wizcon." );
    }
    sem_post(&vg_semaphore); 

    /* Programation timer pour relecture flag AprilNet */
    ETIM_REVEIL_CYCLIQUE ( ENICC_REVEIL_15MN, "Nice0_0_0_0", vg_SockTETIM, pl_message )


}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Effectue les operation de terminaison de la tache
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void  et_sortir  ()

/*
* ARGUMENTS EN ENTREE :
*   aucun
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
*   Liberation des ressources memoires
*   Envoyer le message aux taches protocoles
*
------------------------------------------------------*/
{
    static char              *version = "@(#)enix_ctl.c	1.3 05/17/10      : et_sortir" ;

    ENIX_DONNEES_Tube        *pl_DonCrtTube,
                             *pl_DonSupTube;
    ENIX_DONNEES_Echangeur   *pl_DonCrtEch,
                             *pl_DonSupEch;
    XZEXT_MSG_SOCKET         pl_message;

    /* AAZ 18/07/2007 */
    ENIX_DONNEES_SIG         *pl_DonCrtSIG,
                             *pl_DonSupSIG;
    ENIX_DONNEES_ECL         *pl_DonCrtECL,
                             *pl_DonSupECL;
    ENIX_DONNEES_VEN         *pl_DonCrtVEN,
                             *pl_DonSupVEN;
    ENIX_DONNEES_ENR         *pl_DonCrtENR,
                             *pl_DonSupENR;
    ENIX_DONNEES_INC         *pl_DonCrtINC,
                             *pl_DonSupINC;
    ENIX_DONNEES_EXP         *pl_DonCrtEXP,
                             *pl_DonSupEXP;
    ENIX_DONNEES_SYS         *pl_DonCrtSYS,
                             *pl_DonSupSYS;
    ENIX_DONNEES_LTN         *pl_DonCrtLTN,
                             *pl_DonSupLTN;

    XZST_03EcritureTrace( XZSTC_INFO, "IN : et_sortir");

    /*A
     * Liberation des ressources memoires
     */
   
    /*A Liberation des ressources memoires des donnees Tube */

    for ( pl_DonCrtTube =  pl_DonSupTube = pg_debutTube;
          pl_DonCrtTube != NULL; 
          pl_DonSupTube =  pl_DonCrtTube )
    {
        /* Liberation memoire */
        et_sup_liste_Sequence   ( pl_DonCrtTube->ListeSequence );
        et_sup_liste_Equipement ( pl_DonCrtTube->ListeEquipement );
        pl_DonCrtTube =           pl_DonCrtTube->Suivant;
        free ( pl_DonSupTube );
    }
    pg_debutTube = NULL;
 
    /*A Suppression des structures de donnees Echangeur */
    for ( pl_DonCrtEch =  pl_DonSupEch = pg_debutEchangeur;
          pl_DonCrtEch != NULL; 
          pl_DonSupEch =  pl_DonCrtEch )
    {
        /* Liberation memoire */
        et_sup_liste_Sequence   ( pl_DonCrtTube->ListeSequence );
        et_sup_liste_Equipement ( pl_DonCrtEch->ListeEquipement );
        pl_DonCrtEch =            pl_DonCrtEch->Suivant;
        free ( pl_DonSupEch );
    }
    pg_debutEchangeur = NULL;

    /* AZAZAZAZAZA */
    /* AAZ 06/09/2007 ajout SIG, ECL, VEN, ENR, INC, EXP, SYS */      

    /*A Suppression des structures de donnees SIGNALISATION */
    for ( pl_DonCrtSIG =  pl_DonSupSIG = pg_debutSIG;
          pl_DonCrtSIG != NULL; 
          pl_DonSupSIG =  pl_DonCrtSIG )
    {
        /* Liberation memoire */
        et_sup_liste_Sequence   ( pl_DonCrtSIG->ListeSequence );
        et_sup_liste_Equipement ( pl_DonCrtSIG->ListeEquipement );
        pl_DonCrtSIG =            pl_DonCrtSIG->Suivant;
        free ( pl_DonSupSIG );
    }
    pg_debutSIG = NULL;

    /*A Suppression des structures de donnees ECLAIRAGE */
    for ( pl_DonCrtECL =  pl_DonSupECL = pg_debutECL;
          pl_DonCrtECL != NULL; 
          pl_DonSupECL =  pl_DonCrtECL )
    {
        /* Liberation memoire */
        et_sup_liste_Sequence   ( pl_DonCrtECL->ListeSequence );
        et_sup_liste_Equipement ( pl_DonCrtECL->ListeEquipement );
        pl_DonCrtECL =            pl_DonCrtECL->Suivant;
        free ( pl_DonSupECL );
    }
    pg_debutECL = NULL;

    /*A Suppression des structures de donnees VENTILATION */
    for ( pl_DonCrtVEN =  pl_DonSupVEN = pg_debutVEN;
          pl_DonCrtVEN != NULL; 
          pl_DonSupVEN =  pl_DonCrtVEN )
    {
        /* Liberation memoire */
        et_sup_liste_Sequence   ( pl_DonCrtVEN->ListeSequence );
        et_sup_liste_Equipement ( pl_DonCrtVEN->ListeEquipement );
        pl_DonCrtVEN =            pl_DonCrtVEN->Suivant;
        free ( pl_DonSupVEN );
    }
    pg_debutVEN = NULL;

    /*A Suppression des structures de donnees ENERGIE */
    for ( pl_DonCrtENR =  pl_DonSupENR = pg_debutENR;
          pl_DonCrtENR != NULL; 
          pl_DonSupENR =  pl_DonCrtENR )
    {
        /* Liberation memoire */
        et_sup_liste_Sequence   ( pl_DonCrtENR->ListeSequence );
        et_sup_liste_Equipement ( pl_DonCrtENR->ListeEquipement );
        pl_DonCrtENR =            pl_DonCrtENR->Suivant;
        free ( pl_DonSupENR );
    }
    pg_debutENR = NULL;

    /*A Suppression des structures de donnees PROTECTION INCENDIE */
    for ( pl_DonCrtINC =  pl_DonSupINC = pg_debutINC;
          pl_DonCrtINC != NULL; 
          pl_DonSupINC =  pl_DonCrtINC )
    {
        /* Liberation memoire */
        et_sup_liste_Sequence   ( pl_DonCrtINC->ListeSequence );
        et_sup_liste_Equipement ( pl_DonCrtINC->ListeEquipement );
        pl_DonCrtINC =            pl_DonCrtINC->Suivant;
        free ( pl_DonSupINC );
    }
    pg_debutINC = NULL;

    /*A Suppression des structures de donnees EXPLOITATION */
    for ( pl_DonCrtEXP =  pl_DonSupEXP = pg_debutEXP;
          pl_DonCrtEXP != NULL; 
          pl_DonSupEXP =  pl_DonCrtEXP )
    {
        /* Liberation memoire */
        et_sup_liste_Sequence   ( pl_DonCrtEXP->ListeSequence );
        et_sup_liste_Equipement ( pl_DonCrtEXP->ListeEquipement );
        pl_DonCrtEXP =            pl_DonCrtEXP->Suivant;
        free ( pl_DonSupEXP );
    }
    pg_debutEXP = NULL;

    /*A Suppression des structures de donnees SYSTEME */
    for ( pl_DonCrtSYS =  pl_DonSupSYS = pg_debutSYS;
          pl_DonCrtSYS != NULL; 
          pl_DonSupSYS =  pl_DonCrtSYS )
    {
        /* Liberation memoire */
        et_sup_liste_Sequence   ( pl_DonCrtSYS->ListeSequence );
        et_sup_liste_Equipement ( pl_DonCrtSYS->ListeEquipement );
        pl_DonCrtSYS =            pl_DonCrtSYS->Suivant;
        free ( pl_DonSupSYS );
    }
    pg_debutSYS = NULL;

    /*A Suppression des structures de donnees Locaux techniques */
    for ( pl_DonCrtLTN =  pl_DonSupLTN = pg_debutLTN;
          pl_DonCrtLTN != NULL; 
          pl_DonSupLTN =  pl_DonCrtLTN )
    {
        /* Liberation memoire */
        et_sup_liste_Sequence   ( pl_DonCrtLTN->ListeSequence );
        et_sup_liste_Equipement ( pl_DonCrtLTN->ListeEquipement );
        pl_DonCrtLTN =            pl_DonCrtLTN->Suivant;
        free ( pl_DonSupLTN );
    }
    pg_debutLTN = NULL;
    
    /*A
     * Envoie du message d'arret aux taches protocoles 
     */
    
    /* Construction du message d'arret */
    sprintf ( pl_message, "%d|1|%s", ENIX_CMD_DEMANDE_ARRET, XZEXC_FIN_MSG_SOCK_WIZCON );

    /* Envoie de la demande d'arret */
    sem_wait(&vg_semaphore); 
    if ( write ( vg_SockTWWI0, pl_message, strlen (pl_message) ) == XDC_NOK )
    { 
        /* Ecriture trace impossible d'emettre une demande d'arret */
        XZST_03EcritureTrace(XZSTC_WARNING, " Impossible d'emettre une demande d'arret vers maitre Wizcon ." );
    }
    sem_post(&vg_semaphore); 

    /* Construction du message d'arret */
    sprintf ( pl_message, "%d|1|%s", ENIX_CMD_DEMANDE_ARRET, XZEXC_FIN_MSG_SOCK_WIZCON );
    /* Envoie de la demande d'arret */
    sem_wait(&vg_semaphore); 
    if ( write ( vg_SockTWWI1, pl_message, strlen (pl_message) ) == XDC_NOK )
    { 
        /* Ecriture trace impossible d'emettre une demande d'arret */
        XZST_03EcritureTrace(XZSTC_WARNING, " Impossible d'emettre une demande d'arret vers esclave Wizcon ." );
    }
    sem_post(&vg_semaphore); 

    /*A Attendre */
    sleep (XZEXC_ATTENTE_ARRET_TACHE);

    /* Fin de la tache */
    exit (0);
}
