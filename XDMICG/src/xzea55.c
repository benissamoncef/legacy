/*E*/
/*Fichier :  $Id: xzea55.c,v 1.1 2008/02/13 13:25:01 devgtie Exp $      Release : $Revision: 1.1 $        Date : $Date: 2008/02/13 13:25:01 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE xzea  * FICHIER xzea55.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module d'interface pour l'abonnement aux donnees des equipements externes >
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D    version 1.1    21 Sep 2007    : Creation DEM 662
* LCL		20/12/21	: MOVIS P2 ems SAE-312
*
------------------------------------------------------*/

/* fichiers inclus */

#include "xzea.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char     *version             = "@(#)xzea55.c    1.1 09/21/07 : xzea55" ;
XDY_Fonction    pg_FonctUtil_XZEA_55 = NULL;
T_CB            vg_CB_XZEA55         = NULL;

/* declaration de fonctions internes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

 void    XZEA_Etat_Domaine ( T_IPC_CONN                  va_Cnx,
                             T_IPC_CONN_PROCESS_CB_DATA  pa_Data,
                             T_CB_ARG                    pa_ARG )
 
/*
* ARGUMENTS EN ENTREE :
*  Message RTworks
*
* ARGUMENTS EN SORTIE :
*   aucun
*
*
* CODE RETOUR : 
*   aucun
*
* CONDITION D'UTILISATION
*  Fonction appelee lors de la reception du message
*  XDM_ETAT_DOMAINE 
*
* FONCTION 
*
*
------------------------------------------------------*/
{
    static char       *version    = "@(#)xzea55.c    1.1 09/21/07 : XZEA_Etat_Domaine ";
    XZEAT_Domaine     vl_Etat;
#ifndef _TIBCO_EMS
    T_INT4            vl_TypeOuvrage,
                      vl_IdOuvrage,
                      vl_Domaine,
                      vl_Mode,
                      vl_IdScenario;
#else
    XDY_Entier            vl_TypeOuvrage,
                      vl_IdOuvrage,
                      vl_Domaine,
                      vl_Mode,
                      vl_IdScenario;
#endif
    int               vl_Resultat = XDC_OK;
    XDY_NomMachine vl_machine="";
   
    /* Recuperation des parametres du message XDM_ETAT_DOMAINE */
    if ( !TipcMsgRead( pa_Data -> msg,
                       T_IPC_FT_INT4, &vl_TypeOuvrage,
                       T_IPC_FT_INT4, &vl_IdOuvrage,
                       T_IPC_FT_INT4, &vl_Domaine,
                       T_IPC_FT_INT4, &vl_Mode,                       
                       T_IPC_FT_INT4, &vl_IdScenario,
                       NULL) ) 
    {
        XZST_03EcritureTrace ( XZSTC_WARNING," Impossible de lire le MSG XDM_ETAT_DOMAINE dans XZEA_Etat_Domaine ");
    }
    else
    {
        XZST_03EcritureTrace ( XZSTC_FONCTION, 
                               "IN  : XZEA_Etat_Domaine vl_TypeOuvrage : %d, vl_IdOuvrage : %d, vl_Domaine : %d, vl_Mode : %d, vl_IdScenario : %d.",
                               vl_TypeOuvrage,
                               vl_IdOuvrage,
                               vl_Domaine,
                               vl_Mode,
                               vl_IdScenario);

        /*A Renseignement de la structure XZEAT_Domaine */
        vl_Etat.TypeOuvrage     = vl_TypeOuvrage;
        vl_Etat.IdOuvrage		= vl_IdOuvrage;
		vl_Etat.Domaine			= vl_Domaine;
		vl_Etat.Mode			= vl_Mode;
		vl_Etat.IdScenario		= vl_IdScenario;

        if (XZSC_GetDestinataire(pa_Data, vl_machine)!=XDC_OK) {
	        XZST_03EcritureTrace(XZSTC_WARNING,"XZEA_Etat_Domaine : probleme dans recuperation machine destinataire du message");
	  }
       
      /*A Appel de la fonction utilsateur avec la structure XZEAT_Domaine en argument */
      pg_FonctUtil_XZEA_55 ( vl_Etat, &vl_Resultat, vl_machine );
      XZST_03EcritureTrace ( XZSTC_FONCTION,  "OUT : XZEA_Etat_Domaine %d", vl_Resultat );
   }
}
/* definition de fonctions externes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Abonnement aux etats des domainess de la GTC de Nice ( GTC Renovee)  >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int    XZEA55_Abt_Etat_Domaine ( XDY_Booleen    va_Abonnement,
                                XDY_Datagroup  pa_DGInit,
                                XDY_Fonction   pa_FonctUtilisateur,
                                XDY_Nom        pa_NomSite,
                                XDY_Entier     va_Domaine,
                                XDY_Eqt        va_TypeEqt,
                                XDY_Eqt        va_NumEqt,
                                int *          pa_Resultat )

/*
* ARGUMENTS EN ENTREE :
*
* XDY_Booleen    va_Abonnement        : Abonnement : Debut ou fin d'abonnement
* XDY_Datagroup  pa_DGInit            : Nom du DataGroup d'Init
* XDY_Fonction   pa_FonctUtilisateur  : Nom de la fonction utilisateur
* XDY_Nom        pa_NomSite           : Nom du site demande
* XDY_Eqt        va_TypeEqt           : Type ouvrage
* XDY_Eqt        va_NumEqt            : Numero ouvrage
* XDY_Eqt        va_Domaine           : Doamine
*
* ARGUMENTS EN SORTIE :
*
* int            pa_resultat          : Code retour resultat
*
* CODE RETOUR : 
*   XDC_OK        Succes
*
* CONDITION D'UTILISATION
*   Ce service est demande pour s'abonner aux etats des equipements de la GTC Nice
*
* FONCTION 
*   Si demande d'abonnement
*     Alors : armement CallBack et joindre les Datagroups correspondant
*     Sinon : Desarmment CallBack (si demande fermeture) et disjoindre les Datagroups
*
------------------------------------------------------*/
{
static char      *version  = "@(#)xzea55.c    1.1 09/21/07 : XZEA55_Abt_Etat_Domaine ";
XDY_Datagroup    pl_NomDG;
int              vl_ValRet = XDC_OK;
T_BOOL           vl_EtatDg = FALSE;
XDY_Entier       vl_TypeEqt = va_TypeEqt;
XDY_Entier       vl_NumEqt  = va_NumEqt;

    XZST_03EcritureTrace ( XZSTC_FONCTION, 
                           "IN  : XZEA55_Abt_Etat_Domaine Abt:%d DGInit:%s Fonct:%ld NomSite:%s TypeEqt:%d NumEqt:%d. Domaine:%d",
                           va_Abonnement,
                           pa_DGInit,
                           pa_FonctUtilisateur,
                           pa_NomSite,
                           va_TypeEqt,
                           va_NumEqt,
                           va_Domaine );

    /* Memorisation en globale du nom de la fonction utilisateur */        
    pg_FonctUtil_XZEA_55 = pa_FonctUtilisateur;

    /*A Si demande d'abonnement */
    if ( va_Abonnement == XZEAC_DEBUT_ABONNEMENT )
    {
        /*A  Armement Callback sur Reception XDM_ETAT_DOMAINE */
        if ( vg_CB_XZEA55 == NULL )
        {
            if ( ( vg_CB_XZEA55 = TipcSrvProcessCbCreate ( XDM_IdentMsg(XDM_ETAT_DOMAINE),
                                                           XZEA_Etat_Domaine,
                                                           (T_CB_ARG)NULL ) ) == NULL ) 
            {    
                /*B Ecriture Trace */
                XZST_03EcritureTrace ( XZSTC_WARNING, "Echec armement Callback sur XDM_ETAT_DOMAINE." );
                vl_ValRet = XDC_NOK;
            }
            else
            {    
                /*B Ecriture Trace */
                XZST_03EcritureTrace ( XZSTC_DEBUG1, "Armement Callback sur XDM_ETAT_DOMAINE." );
            }
        }

        /*A Abonnement au Datagroup XDG_ETAT_DOMAINE<NomSite> */
        /*A Construction du nom du DataGroup */
        sprintf ( pl_NomDG, "%s_%s", XDG_ETAT_DOMAINE, pa_NomSite );
        /*A Joindre ce datagroup s'il n'est pas deja joint */
        TipcSrvSubjectGetSubscribe ( pl_NomDG, &vl_EtatDg );
        if ( !vl_EtatDg )
        {
            if ( !XDG_JoindreDG( pl_NomDG ) )
            {
                /*B Ecriture Trace */
                XZST_03EcritureTrace ( XZSTC_WARNING, "Echec abonnement au Datagroup %s ", pl_NomDG );
                vl_ValRet = XDC_NOK;
            }
            else
            {
                XZST_03EcritureTrace ( XZSTC_DEBUG1, "Abonnement au Datagroup %s ", pl_NomDG );
            }
        }

        /*A Abonnement au Datagroup DGInit passe en parametre */
        /*A Joindre ce datagroup s'il n'est pas deja joint */
        TipcSrvSubjectGetSubscribe ( pa_DGInit, &vl_EtatDg );
        if ( ( !vl_EtatDg ) && ( pa_DGInit != NULL ) )
        {
            if ( !XDG_JoindreDG( pa_DGInit ) )
            {
                /*B Ecriture Trace */
                XZST_03EcritureTrace ( XZSTC_WARNING, "Echec abonnement au Datagroup %s ", pa_DGInit );
                vl_ValRet = XDC_NOK;
            }
            else
            {
                XZST_03EcritureTrace ( XZSTC_DEBUG1, "Abonnement au Datagroup %s ", pa_DGInit );
            }
        }
        /* Envoi du message de demande d'init XDM_EInit_IHM_NICERenovee */
        /*A Construction du DataGroup XDG_EInit_IHM_NICE_<NomSite> */
        XDG_EncodeDG2 ( pl_NomDG, XDG_EInit_IHM_NICE, pa_NomSite );

        /*A Envoi du message XDM_EInit_IHM_NICERenovee en fournissant les valeur recues  */

        if (!TipcSrvMsgWrite ( pl_NomDG,
                               XDM_IdentMsg(XDM_EInit_IHM_NICERenovee),
                               XDM_FLG_SRVMSGWRITE,
                               T_IPC_FT_STR,  pa_DGInit,
                               T_IPC_FT_INT4, va_Domaine,
                               T_IPC_FT_INT4, vl_TypeEqt,
                               T_IPC_FT_INT4, vl_NumEqt,
                               T_IPC_FT_INT4, XDC_GTC_DOMAINE,
                               NULL))
        {
            /*A Trace envoi message XDM_EInit_IHM_NICERenovee non effectue */        
            XZST_03EcritureTrace(XZSTC_WARNING, "Echec envoi message XDM_EInit_IHM_NICERenovee.");
        }
        else
        {
            /*A Trace envoi message XDM_EInit_IHM_NICERenovee */        
            XZST_03EcritureTrace(XZSTC_DEBUG1, "Envoi du message XDM_EInit_IHM_NICERenovee sur %s.", pl_NomDG );
        }
    }
    else
    {
        if ( ( va_Abonnement == XZEAC_FERMER_ABONNEMENT ) && ( vg_CB_XZEA55 != NULL ) )
        {
            /*A  Desarmement Callback sur Reception XDM_ETAT_DOMAINE */
            if ( TutCbDestroy ( vg_CB_XZEA55 ) == FALSE )  
            {    
                /*B Ecriture Trace */
                XZST_03EcritureTrace ( XZSTC_WARNING, "Echec desarmement Callback sur XDM_ETAT_DOMAINE ." );
                vl_ValRet = XDC_NOK;
            }
            else
            {
                XZST_03EcritureTrace ( XZSTC_DEBUG1, "Desarmement Callback sur XDM_ETAT_DOMAINE ." );
                vg_CB_XZEA55 = NULL;
            }
        }

        /*A Desabonnement au Datagroup XDG_ETAT_DOMAINE<NomSite> */
        /*A Construction du nom du DataGroup */
        sprintf ( pl_NomDG, "%s_%s", XDG_ETAT_DOMAINE, pa_NomSite );
        /*A Disjoindre ce datagroup s'il est deja joint */
        TipcSrvSubjectGetSubscribe ( pl_NomDG, &vl_EtatDg );
        if ( vl_EtatDg )
        {
            if ( !XDG_QuitterDG( pl_NomDG ) )
            {
                /*B Ecriture Trace */
                XZST_03EcritureTrace ( XZSTC_WARNING, "Echec desabonnement au Datagroup %s ", pl_NomDG );
                vl_ValRet = XDC_NOK;
            }
            else
            {
                XZST_03EcritureTrace ( XZSTC_DEBUG1, "Desabonnement au Datagroup %s ", pl_NomDG );
            }
        }

        /*A Desabonnement au Datagroup DGInit passe en parametre */
        /*A Disjoindre ce datagroup s'il est deja joint */
        TipcSrvSubjectGetSubscribe ( pa_DGInit, &vl_EtatDg );
        if ( ( vl_EtatDg ) && ( pa_DGInit != NULL ) )
        {
            if ( !XDG_QuitterDG( pa_DGInit ) )
            {
                /*B Ecriture Trace */
                XZST_03EcritureTrace ( XZSTC_WARNING, "Echec desabonnement au Datagroup %s ", pa_DGInit );
                vl_ValRet = XDC_NOK;
            }
            else
            {
                XZST_03EcritureTrace ( XZSTC_DEBUG1, "Desabonnement au Datagroup %s ", pa_DGInit );
            }
        }
    }

    XZST_03EcritureTrace ( XZSTC_FONCTION, "OUT : XZEA55_Abt_Etat_Domaine Retour:%d.", vl_ValRet );
    *pa_Resultat = vl_ValRet;
    return ( vl_ValRet );                

}


