/*E*/
/*Fichier :  $Id: enix_red.c,v 1.2 2010/05/17 15:02:04 devgtie Exp $      Release : $Revision: 1.2 $        Date : $Date: 2010/05/17 15:02:04 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TENIX * FICHIER enix_red.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* c'est le module qui g�re la redondance entre les 
* machine ma�tre et esclave de Wizcon, elle met � jour
* une variable globale d�crivant le ma�tre.
*
------------------------------------------------------
* HISTORIQUE :
*
* AAZ       version 1.1 19 Septembre 2007    : GTC r�nov�e DEM 664
* ADB       22 mars 2010 : FT 3448 - Gestion des etats liaisons
* LCL       27/04/2022 : JIRA 377 : probleme de calcul avec des double : non declenchement de la synchro horaire et de la gestion de la redondance
*
------------------------------------------------------*/

/* fichiers inclus */
#include <xzsem.h>
 
#include "enix_red.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Analyse de la redondance
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int et_MessageInit ()

/*
* ARGUMENTS EN ENTREE :
*   
* 
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
*
*
* CONDITION D'UTILISATION
* execute a chaque reprise de comm
*
* FONCTION 
*
------------------------------------------------------*/
{
    XZEXT_MSG_SOCKET         pl_message;

    XZST_03EcritureTrace( XZSTC_INFO, "IN : et_MessageInit");
    
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

}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Analyse de la redondance
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int et_Redondance (  )

/*
* ARGUMENTS EN ENTREE :
*   
* 
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
*
* CONDITION D'UTILISATION
* execute de fa�on p�riodique dans le enix_dir
*
* FONCTION 
*
*  Analyse de la redondance des superviseur de la GTC
------------------------------------------------------*/
{
    int           vl_retour                    = XDC_OK;
    static double vl_heure_dernier_essai_TWWI0 = 0;
    static double vl_heure_dernier_essai_TWWI1 = 0;
    static double vl_heure_debut_NOK_TWWI0     = 0;
    static double vl_heure_debut_NOK_TWWI1     = 0;
    static double vl_heure_dernier_alive       = 0;
    static int    vl_stock_NOK_TWWI0=XDC_FAUX;
    static int    vl_stock_NOK_TWWI1=XDC_FAUX;
    static int    vl_dernier_etat_TWWI0=XDC_OK;
    static int    vl_dernier_etat_TWWI1=XDC_OK;
    static int    vl_nb_essai_TWWI0            = 0;
    static int    vl_nb_essai_TWWI1            = 0;
    static int    vl_alive                     = XDC_FAUX;
	int			  vl_initNecessaire			   = XDC_NON;

    /*XZST_03EcritureTrace( XZSTC_INFO, "IN : et_redondance");*/
   
    /* Patch suite passage EMS et problemes de calcul avec des double */
    /* NE PAS MODIFIER ET LAISSER LES VARIABLES LOCALES */
    double vl_local_vl_heure_dernier_essai_TWWI0   = vl_heure_dernier_essai_TWWI0;
    double vl_local_vl_heure_dernier_essai_TWWI1   = vl_heure_dernier_essai_TWWI1;
    double vl_local_vl_heure_debut_NOK_TWWI0       = vl_heure_debut_NOK_TWWI0;
    double vl_local_vl_heure_debut_NOK_TWWI1       = vl_heure_debut_NOK_TWWI1;
    double vl_local_vl_heure_dernier_alive         = vl_heure_dernier_alive;
    double vl_local_TutGetWallTime                 = TutGetWallTime();

    XZST_03EcritureTrace( XZSTC_DEBUG1, "et_Redondance : check connexions avec Wizcons");

    /* Si la connexion avec la tache Wizcon maitre est mauvaise*/
    
    if (vg_etat_SockTWWI0 == XDC_NOK)
    {
        XZST_03EcritureTrace( XZSTC_DEBUG1, "et_Redondance : Liaison Wizcon principal DOWN");

        /* Stockage de l'heure de debut de probleme */
        if (vl_stock_NOK_TWWI0 == XDC_FAUX)
        {
            vl_stock_NOK_TWWI0       = XDC_VRAI;
            vl_heure_debut_NOK_TWWI0 = vl_local_TutGetWallTime;
            vl_local_vl_heure_debut_NOK_TWWI0 = vl_heure_debut_NOK_TWWI0;
        }

        if (et_Relance_connexion(vl_nb_essai_TWWI0,vl_local_vl_heure_dernier_essai_TWWI0) == XDC_OUI)
        {
            /*B Ecriture Trace */
            XZST_03EcritureTrace( XZSTC_WARNING, "Nouvel essai de connexion avec serveur Wizcon principal ." );

            /* On refait un essai de connexion */
            vl_heure_dernier_essai_TWWI0=vl_local_TutGetWallTime;

            if ( ex_cnx_client_ST ( vg_nom_machine_TWWI0,
                                    vg_Port_SockTWWI0,
                                    &vg_SockMask,
                                    &vg_SockTWWI0 ) != XDC_OK )
            {
                /*B Ecriture Trace */
                XZST_03EcritureTrace( XZSTC_WARNING, "Impossible de re-creer la connexion serveur Wizcon principal ." );    
                vl_nb_essai_TWWI0++;
            }
            else
            {
                vg_etat_SockTWWI0 = XDC_OK;
                vl_nb_essai_TWWI0 = 0;
				vl_initNecessaire = XDC_OUI;

            }

        }
    }

    /* La connexion avec la t�che Wizcon maitre est en etat de marche */
    else
    {
        vl_heure_dernier_essai_TWWI0 = vl_local_TutGetWallTime;
        vl_nb_essai_TWWI0            = 0;
        vg_etat_SockTWWI0            = XDC_OK;
        vl_stock_NOK_TWWI0           = XDC_FAUX;

    }

    /* Si la connexion avec la tache Wizcon esclave est mauvaise*/

    if (vg_etat_SockTWWI1 == XDC_NOK)
    {
        XZST_03EcritureTrace( XZSTC_DEBUG1, "et_Redondance : Liaison Wizcon secondaire DOWN");

        /* Stockage de l'heure de d�but de probleme */
        if (vl_stock_NOK_TWWI1 == XDC_FAUX)
        {
            vl_stock_NOK_TWWI1       = XDC_VRAI;
            vl_heure_debut_NOK_TWWI1 = vl_local_TutGetWallTime;
            vl_local_vl_heure_debut_NOK_TWWI1 = vl_heure_debut_NOK_TWWI1;
        }
        if (et_Relance_connexion(vl_nb_essai_TWWI1,vl_local_vl_heure_dernier_essai_TWWI1) == XDC_OUI)
        {
            /*B Ecriture Trace */
            XZST_03EcritureTrace( XZSTC_WARNING, "Nouvel essai de connexion avec serveur Wizcon secondaire ." );    

            /* On refait un essai de connexion */
            vl_heure_dernier_essai_TWWI1 = vl_local_TutGetWallTime;
            vl_nb_essai_TWWI1++;
            if ( ex_cnx_client_ST ( vg_nom_machine_TWWI1,
                                    vg_Port_SockTWWI1,
                                    &vg_SockMask,
                                    &vg_SockTWWI1 ) != XDC_OK )
            {
                /*B Ecriture Trace */
                XZST_03EcritureTrace( XZSTC_WARNING, "Impossible de re-creer la connexion avec serveur Wizcon secondaire ." );    
            }
            else
            {
                vg_etat_SockTWWI1=XDC_OK;
                vl_nb_essai_TWWI1=0;
				vl_initNecessaire = XDC_OUI;

            }
        }
    }
    /* La connexion avec la t�che Wizcon esclave est en etat de marche */
    else
    {
        vl_heure_dernier_essai_TWWI1 = vl_local_TutGetWallTime;
        vl_nb_essai_TWWI1            = 0;
        vg_etat_SockTWWI1            = XDC_OK;
        vl_stock_NOK_TWWI1           = XDC_FAUX;
    
    }    

    /* Ya t'il eu un changement d'etat depuis la derniere fois */
    if ((vg_etat_SockTWWI0 != vl_dernier_etat_TWWI0) ||
        (vg_etat_SockTWWI1 != vl_dernier_etat_TWWI1))
    {
        XZST_03EcritureTrace( XZSTC_WARNING, "Il y a un changement d'etat sur les serveurs Wizcon." );
        ex_env_liaison ( vg_nom_machine_TWWI0, vg_etat_SockTWWI0, vg_TypeEqt_LiaiTWWI0, vg_NumEqt_LiaiTWWI0); 
        ex_env_liaison ( vg_nom_machine_TWWI1, vg_etat_SockTWWI1, vg_TypeEqt_LiaiTWWI1, vg_NumEqt_LiaiTWWI1); 

        /* On indique qu'il va falloir stocker les heures de debut de NOK */
        vl_stock_NOK_TWWI0 = XDC_FAUX;
        vl_stock_NOK_TWWI1 = XDC_FAUX;

        /* Si les deux sockets ne repondent plus alors */
        if ((vg_etat_SockTWWI0 == XDC_NOK) &&
            (vg_etat_SockTWWI1 == XDC_NOK))
        {
            /*B Ecriture Trace */
            XZST_03EcritureTrace( XZSTC_WARNING, "Les deux serveur Wizcon ne repondent plus ." );    

            /* Mise a jour des derniers etats*/
            vl_dernier_etat_TWWI0 = vg_etat_SockTWWI0;
            vl_dernier_etat_TWWI1 = vg_etat_SockTWWI1;
            vg_maitre_actif       = XDC_MAITRE_INCONNU;
        }
        else
        {
            /* On est passe de OK a NOK sur la tache Wizcon maitre */
            if ((vg_etat_SockTWWI0     == XDC_NOK) &&
                (vl_dernier_etat_TWWI0 == XDC_OK))
            {
                XZST_03EcritureTrace( XZSTC_WARNING, "Le serveur Wizcon principal est passe a NOK." );    
                /* Le maitre etait la tache Wizcon maitre */
                if (vg_maitre_actif == XDC_MAITRE_TWWI0)
                {
                    /* Le nouveau maitre est Wizcon esclave*/
                    /*B Ecriture Trace */
                    XZST_03EcritureTrace( XZSTC_WARNING, "Forcage serveur Wizcon secondaire pour devenir maitre ." );    
                    if (et_cmd_maitre(vg_SockTWWI1) != XDC_OK)
                    {
                        /*B Ecriture Trace */
                        XZST_03EcritureTrace( XZSTC_WARNING, "Impossible de forcer serveur Wizcon secondaire pour devenir maitre ." );    
                    }
                    else
                    {
                        /*B Ecriture Trace */
                        XZST_03EcritureTrace( XZSTC_WARNING, "Forcage OK serveur Wizcon secondaire pour devenir maitre ." );    
                        vg_maitre_actif = XDC_MAITRE_TWWI1;

                        /*Envoi de demande d'init sur reprise de comm*/
                        et_MessageInit();
                    }
                }
                vl_dernier_etat_TWWI0=vg_etat_SockTWWI0;
            }
            /* On est passe de OK a NOK sur la tache Wizcon esclave */
            else if ((vg_etat_SockTWWI1     == XDC_NOK) &&
                     (vl_dernier_etat_TWWI1 == XDC_OK))
            {
                XZST_03EcritureTrace( XZSTC_WARNING, "Le serveur Wizcon secondaire est passe a NOK." );
                /* Le maitre etait la tache Wizcon esclave */
                if (vg_maitre_actif == XDC_MAITRE_TWWI1)
                {
                    /*B Ecriture Trace */
                    XZST_03EcritureTrace( XZSTC_WARNING, "Forcage serveur Wizcon principal pour devenir maitre ." );

                    /* Le nouveau maitre est Wizcon maitre*/
                    if (et_cmd_maitre(vg_SockTWWI0) != XDC_OK)
                    {
                        /*B Ecriture Trace */
                        XZST_03EcritureTrace( XZSTC_WARNING, "Impossible de forcer serveur Wizcon principal pour devenir maitre ." );    
                    }
                    else
                    {
                        /*B Ecriture Trace */
                        XZST_03EcritureTrace( XZSTC_WARNING, "Forcage OK serveur Wizcon principal pour devenir maitre ." );    
                        vg_maitre_actif = XDC_MAITRE_TWWI0;

                        /*Envoi de demande d'init sur reprise de comm*/
                        et_MessageInit();
                    }
                }
                vl_dernier_etat_TWWI1=vg_etat_SockTWWI1;
            }

            /* On est passe de NOK a OK sur la tache Wizcon maitre */
            else if ((vg_etat_SockTWWI0 == XDC_OK) && (vl_dernier_etat_TWWI0 == XDC_NOK))
            {
                XZST_03EcritureTrace( XZSTC_WARNING, "Le serveur Wizcon principal est passe a OK." );    

                /* On ne fait rien car le maitre est dej� Wizcon maitre */    
                vl_dernier_etat_TWWI0=vg_etat_SockTWWI0;

                /*Envoi de demande d'init sur reprise de comm*/
                et_MessageInit();
            }

            /* On est pass� de NOK � OK sur la tache Wizcon esclave */
            else if ((vg_etat_SockTWWI1 == XDC_OK) && (vl_dernier_etat_TWWI1 == XDC_NOK))
            {
                XZST_03EcritureTrace( XZSTC_WARNING, "Le serveur Wizcon secondaire est passe a OK." );    

                /* On ne fait rien car le maitre est deja Wizcon maitre */    
                vl_dernier_etat_TWWI1=vg_etat_SockTWWI1;
            }
        }    
        /* Forcer un petit dialogue pour remettre a jour les donnees */
        vl_alive=XDC_VRAI;
    }
    else
    {
        /* On va traiter le cas ou un seul Wizcon repond l'autre est en NOK  et le maitre est inconnu*/
        /* depuis 3 essais = 3 * 30, on force le seul dialoguant a etre maitre */
        if ((vg_etat_SockTWWI0 == XDC_OK)  &&
            (vg_etat_SockTWWI1 == XDC_NOK) &&
            (vg_maitre_actif   == XDC_MAITRE_INCONNU))
        {
            XZST_03EcritureTrace( XZSTC_DEBUG1, "et_Redondance : serveur maitre inconnu. Forcage Principal dans %d secondes", (3*XDC_TIME_OUT_ESSAI)-abs((int)(vl_local_vl_heure_debut_NOK_TWWI1 - vl_local_TutGetWallTime)));

            if (abs((int)(vl_local_vl_heure_debut_NOK_TWWI1 - vl_local_TutGetWallTime)) > 3 * XDC_TIME_OUT_ESSAI)
            {
                /* Le nouveau maitre est Wizcon maitre*/
                if (et_cmd_maitre(vg_SockTWWI0) != XDC_OK)
                {
                    /*B Ecriture Trace */
                    XZST_03EcritureTrace( XZSTC_WARNING, "Impossible de forcer serveur Wizcon principal pour devenir maitre ." );    
                }
                else
                {
                    /*B Ecriture Trace */
                    XZST_03EcritureTrace( XZSTC_WARNING, "Forcage OK serveur Wizcon principal pour devenir maitre ." );    
                    vg_maitre_actif = XDC_MAITRE_TWWI0;

                    /*Envoi de demande d'init sur reprise de comm*/
                    et_MessageInit();
                }
            }
        }
        if ((vg_etat_SockTWWI1 == XDC_OK)  &&
            (vg_etat_SockTWWI0 == XDC_NOK) &&
            (vg_maitre_actif   == XDC_MAITRE_INCONNU))
        {
            if (abs((int)(vl_local_vl_heure_debut_NOK_TWWI0 - vl_local_TutGetWallTime)) > 3 * XDC_TIME_OUT_ESSAI)
            {
                XZST_03EcritureTrace( XZSTC_DEBUG1, "et_Redondance : serveur maitre inconnu. Forcage Secondaire dans %d secondes", (3*XDC_TIME_OUT_ESSAI)-abs((int)(vl_local_vl_heure_debut_NOK_TWWI1 - vl_local_TutGetWallTime)));

               /* Le nouveau maitre est serveur secondaire*/
                if (et_cmd_maitre(vg_SockTWWI1) != XDC_OK)
                {
                    /*B Ecriture Trace */
                    XZST_03EcritureTrace( XZSTC_WARNING, "Impossible de forcer serveur Wizcon secondaire pour devenir maitre ." );    
                }
                else
                {
                    /*B Ecriture Trace */
                    XZST_03EcritureTrace( XZSTC_WARNING, "Forcage OK serveur Wizcon secondaire pour devenir maitre ." );    
                    vg_maitre_actif = XDC_MAITRE_TWWI1;

                    /*Envoi de demande d'init sur reprise de comm*/
                    et_MessageInit();
                }
            }
        }
    }

    /* On envoie un petit message pour verifier que les deux Wizcon sont joignables toutes les 30 s*/
    /* ou s'il y a eu u changement d'etat car ceci permet de recaler les valeurs des OK et NOK */
    XZST_03EcritureTrace( XZSTC_DEBUG1, "et_Redondance : keepalive dans %d secondes", XDC_TIME_OUT_ESSAI-abs((int)(vl_local_vl_heure_dernier_alive - vl_local_TutGetWallTime)));

    if ((abs((int)(vl_local_vl_heure_dernier_alive - vl_local_TutGetWallTime)) > XDC_TIME_OUT_ESSAI) || (vl_alive == XDC_VRAI))
    {
        /* RAZ du alive*/
        vl_alive               = XDC_FAUX;
        vl_heure_dernier_alive = vl_local_TutGetWallTime;
        vl_local_vl_heure_dernier_alive = vl_heure_dernier_alive;

        if (vg_etat_SockTWWI0 == XDC_OK)
        {
            if (et_cmd_alive(vg_SockTWWI0) != XDC_OK)
            {
                vg_etat_SockTWWI0 = XDC_NOK;

                
                if (vg_maitre_actif == XDC_MAITRE_TWWI0)
                    XZST_03EcritureTrace( XZSTC_WARNING, "Probleme de communication avec serveur principal Wizcon qui est le maitre ");    
                else
                    XZST_03EcritureTrace( XZSTC_WARNING, "Probleme de communication avec serveur principal Wizcon qui est l'esclave ");    
            }
        }
        if (vg_etat_SockTWWI1 == XDC_OK)
        {
            if (et_cmd_alive(vg_SockTWWI1) != XDC_OK)
            {
                vg_etat_SockTWWI1 = XDC_NOK;

                if (vg_maitre_actif == XDC_MAITRE_TWWI1)
                    XZST_03EcritureTrace( XZSTC_WARNING, "Probleme de communication avec serveur secondaire Wizcon qui est le maitre ");    
                else
                    XZST_03EcritureTrace( XZSTC_WARNING, "Probleme de communication avec serveur secondaire Wizcon qui est l'esclave ");    
            }
        }
    }
	
	if (vl_initNecessaire == XDC_OUI)
	{
		et_MessageInit();
	}

    return(vl_retour);
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Analyse de la redondance
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int et_Relance_connexion (  int    a_nb_essai,
                            double a_heure_dernier_essai )

/*
* ARGUMENTS EN ENTREE :
*   
* 
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
*
* CONDITION D'UTILISATION
* execute de fa�on p�riodique dans le enix_dir
*
* FONCTION 
*
*  D�termine s'il faut relancer la connexion
------------------------------------------------------*/
{
    int vl_retour=XDC_NON;

   /* XZST_03EcritureTrace( XZSTC_INFO, "IN : et_Relance_connexion");*/
    
    /* A partir de trois tentatives les essais sont espac�s*/
    if (a_nb_essai >= 3)
    {
        /*XZST_03EcritureTrace( XZSTC_DEBUG1, "precedente %f", a_heure_dernier_essai);    
        XZST_03EcritureTrace( XZSTC_DEBUG1, "actuelle %f",   TutGetWallTime());    
        XZST_03EcritureTrace( XZSTC_DEBUG1, "difference %f", TutGetWallTime()-a_heure_dernier_essai);*/

        /* Diff�rence entre maintenant et le dernier essai*/
        if (abs((int)(TutGetWallTime()-a_heure_dernier_essai)) > XDC_TIME_OUT_ESSAI)
        {
            vl_retour=XDC_OUI;
        }
    }
    else
    {
        vl_retour=XDC_OUI;
    }
    return (vl_retour);
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Analyse de la redondance
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int et_Synchro ()

/*
* ARGUMENTS EN ENTREE :
*   
* 
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
*
* CONDITION D'UTILISATION
* execute de fa�on p�riodique dans le enix_dir
*
* FONCTION 
*
*  Permet de lancer une synchro de temps en temps
------------------------------------------------------*/
{
    int           vl_retour                = XDC_NON;
    static double vl_heure_dernier_synchro = 0.0;
 
  /*  XZST_03EcritureTrace( XZSTC_INFO, "IN : et_synchro");*/

   double vl_local_heure_dernier_synchro;
   double vl_local_TutGetWallTime;
   char   vl_buffer[255];

    /* Patch suite passage EMS et problemes de calcul avec des double */
    /* NE PAS MODIFIER ET LAISSER LES VARIABLES LOCALES */
    vl_local_heure_dernier_synchro=vl_heure_dernier_synchro;
    vl_local_TutGetWallTime=TutGetWallTime();

    sprintf(vl_buffer,"Envoi trame synchro horaire aux Wizcons dans %d secondes ",
                       XDC_TIME_OUT_SYNCHRO-abs((int)(vl_local_heure_dernier_synchro-vl_local_TutGetWallTime)));
    XZST_03EcritureTrace( XZSTC_DEBUG1, vl_buffer);

    /* On envoie un petit message pour verifier que les deux Wizcon sont synchroniser toutes les N s*/
    if (abs((int)(vl_local_heure_dernier_synchro - vl_local_TutGetWallTime)) > XDC_TIME_OUT_SYNCHRO) 
    {
        vl_heure_dernier_synchro = vl_local_TutGetWallTime;

        XZST_03EcritureTrace( XZSTC_DEBUG1, "Envoi trame synchro horaire auw Wizcons maintenant");

        if (et_cmd_synchro(vg_SockTWWI0) != XDC_OK)
        {
            if (vg_maitre_actif == XDC_MAITRE_TWWI0)
                XZST_03EcritureTrace( XZSTC_WARNING, "Probleme de synchronisation avec le maitre ");    
            else
                XZST_03EcritureTrace( XZSTC_WARNING, "Probleme de synchronisation avec l'esclave ");    
        }

        if (et_cmd_synchro(vg_SockTWWI1) != XDC_OK)
        {
            if (vg_maitre_actif == XDC_MAITRE_TWWI1)
                XZST_03EcritureTrace( XZSTC_WARNING, "Probleme de synchronisation avec le maitre ");    
            else
                XZST_03EcritureTrace( XZSTC_WARNING, "Probleme de synchronisation avec l'esclave ");    
        }
    }
    return (vl_retour);
}


int  ex_env_liaison ( vg_nom_machine, vg_etat_Sock,  vg_TypeEqt_Liai, vg_NumEqt_Liai)
{
	XDY_NomSite                 pl_NomSite;
    XDY_Datagroup               pl_NomDG;
    ENIX_DONNEES_Equipement    pl_Equipement;


if (vg_etat_Sock == XDC_NOK )

	pl_Equipement.EtatEquipement = 2; 
else
	pl_Equipement.EtatEquipement = 1; 


	XZST_03EcritureTrace( XZSTC_WARNING, "Envoi etat liaison <%s, %d>, Eqt<%d,%d>.", vg_nom_machine, vg_etat_Sock, vg_TypeEqt_Liai, vg_NumEqt_Liai );

   /*A Recuperation du site de la machine */
    if ( XZSC_04NomSite( pl_NomSite ) != XDC_OK)
    {
       /* Ecriture Trace */
       XZST_03EcritureTrace( XZSTC_WARNING, "ANALYSE : Recuperation du Nom du site impossible ." );
    }

	
    sprintf ( pl_NomDG, "%s_%s",XDG_ETAT_EQUIPEMENT, pl_NomSite );
    /* Calcul de l'etat de l'equipement */
    pl_Equipement.NumeroEquipement =  vg_NumEqt_Liai; 
    pl_Equipement.TypeEquipement =  vg_TypeEqt_Liai; 
    pl_Equipement.Etat_Service   = 0;
    pl_Equipement.Mode           = -1;
    pl_Equipement.EtatDisc       = 0;
	pl_Equipement.InfoRecue	  = XDC_OUI;



    if (!TipcSrvMsgWrite ( pl_NomDG,
                       XDM_IdentMsg(XDM_ETAT_EQUIPEMENT),
                       XDM_FLG_SRVMSGWRITE,
                       T_IPC_FT_INT4, 102,
                       T_IPC_FT_INT4, 102,
                       T_IPC_FT_INT4, 1,
                       T_IPC_FT_INT4, pl_Equipement.TypeEquipement,
                       T_IPC_FT_INT4, pl_Equipement.NumeroEquipement,
                       T_IPC_FT_INT4, pl_Equipement.EtatEquipement,
                       T_IPC_FT_INT4, pl_Equipement.Etat_Service,
                       T_IPC_FT_INT4, pl_Equipement.Mode ,
                       T_IPC_FT_INT4, pl_Equipement.EtatDisc ,
		        T_IPC_FT_INT2, XDC_NON,
                       NULL ) )
    {
        XZST_03EcritureTrace(XZSTC_WARNING, "ETAT EQUIPEMENT : Envoi message XDM_ETAT_EQUIPEMENT non effectue." );
    }
    else
    {
        XZST_03EcritureTrace( XZSTC_INFO, "ETAT EQUIPEMENT : Envoi message XDM_ETAT_EQUIPEMENTT <NomDG>%s_<domaine>%d_<TypEqt>%d_<NumEqt>%d_<TypeEquipement>%d<NumEqt>%d_<EtatEqt>%d_<EtatDisc>%d<Mode>%d",
                              pl_NomDG,
                              102,
                              102,
                              1,
                              pl_Equipement.TypeEquipement,
                              pl_Equipement.NumeroEquipement,
                              pl_Equipement.EtatEquipement,
                              pl_Equipement.EtatDisc,
							  pl_Equipement.Mode);
    }
	
	XZST_03EcritureTrace( XZSTC_INFO, "Envoi etat liaison <%s, %d> FINI.", vg_nom_machine, vg_etat_Sock );
	
}
