/*E*/
/*Fichier :  $Id: xzea44.c,v 1.1 2008/02/13 13:24:59 devgtie Exp $      Release : $Revision: 1.1 $        Date : $Date: 2008/02/13 13:24:59 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE xzea  * FICHIER xzea44.c
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

static char     *version             = "@(#)xzea44.c    1.1 09/21/07 : xzea44" ;
XDY_Fonction    pg_FonctUtil_XZEA_44 = NULL;
T_CB            vg_CB_XZEA44         = NULL;

/* declaration de fonctions internes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

 void    XZEA_Etat_EqtVEN ( T_IPC_CONN                  va_Cnx,
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
*  XDM_ETAT_EQUIPEMENT 
*
* FONCTION 
*
*
------------------------------------------------------*/
{
    static char       *version    = "@(#)xzea44.c    1.1 09/21/07 : XZEA_Etat_EqtVEN ";
    XZEAT_Equipement  vl_Etat;
#ifndef _TIBCO_EMS
	T_INT4            vl_Domaine,
	                  vl_TypeEqt,
                      vl_NumEqt,
                      vl_TypeEquipement,
                      vl_NumEquipement,
                      vl_EtatEquipement,
                      vl_EtatTechnique,
                      vl_Mode,
                      vl_EtatDisc;
	T_INT2			  vl_ModeRafale;
#else
	XDY_Entier            vl_Domaine,
	                  vl_TypeEqt,
                      vl_NumEqt,
                      vl_TypeEquipement,
                      vl_NumEquipement,
                      vl_EtatEquipement,
                      vl_EtatTechnique,
                      vl_Mode,
                      vl_EtatDisc;
	XDY_Mot			  vl_ModeRafale;
#endif
    int               vl_Resultat = XDC_OK;
    XDY_NomMachine vl_machine="";
   
    /* Recuperation des parametres du message XDM_ETAT_EQUIPEMENT */
    if ( !TipcMsgRead( pa_Data -> msg,
                       T_IPC_FT_INT4, &vl_Domaine,
                       T_IPC_FT_INT4, &vl_TypeEqt,
                       T_IPC_FT_INT4, &vl_NumEqt,
                       T_IPC_FT_INT4, &vl_TypeEquipement,
                       T_IPC_FT_INT4, &vl_NumEquipement,
                       T_IPC_FT_INT4, &vl_EtatEquipement,
                       T_IPC_FT_INT4, &vl_EtatTechnique,
                       T_IPC_FT_INT4, &vl_Mode,
                       T_IPC_FT_INT4, &vl_EtatDisc,
					   T_IPC_FT_INT2, &vl_ModeRafale,
                       NULL) ) 
    {
        XZST_03EcritureTrace ( XZSTC_WARNING," Impossible de lire le MSG XDM_ETAT_EQUIPEMENT dans XZEA_Etat_EqtVEN ");
    }
    else
    {
        XZST_03EcritureTrace ( XZSTC_FONCTION, 
                               "IN  : XZEA_Etat_EqtVEN Domaine:%d TypeEqt:%d NumTube:%d TypeEquipement:%d NumEquipement:%d EtatEquipement:%d EtatTechnique:%d Mode:%d EtatDisc:%d ModeRafale:%d.",
							   vl_Domaine,
                               vl_TypeEqt,
                               vl_NumEqt,
                               vl_TypeEquipement,
                               vl_NumEquipement,
                               vl_EtatEquipement,
                               vl_EtatTechnique,
                               vl_Mode,
                               vl_EtatDisc,
							   vl_ModeRafale);

        /*A Renseignement de la structure XZEAT_Equipement */
        vl_Etat.Domaine         = vl_Domaine;
        vl_Etat.TypeEqt         = vl_TypeEqt;
        vl_Etat.NumEqt          = vl_NumEqt;
        vl_Etat.TypeEquipement   = vl_TypeEquipement;
        vl_Etat.NumEquipement   = vl_NumEquipement;
        vl_Etat.EtatEquipement  = vl_EtatEquipement;
        vl_Etat.EtatTechnique   = vl_EtatTechnique;
        vl_Etat.Mode            = vl_Mode;
        vl_Etat.EtatDisc        = vl_EtatDisc;
		vl_Etat.ModeRafale		= vl_ModeRafale;
           
      if (XZSC_GetDestinataire(pa_Data, vl_machine)!=XDC_OK) {
	    XZST_03EcritureTrace(XZSTC_WARNING,"XZEA_Etat_EqtVEN : probleme dans recuperation machine destinataire du message");
	  }

      /*A Appel de la fonction utilsateur avec la structure XZEAT_Equipement en argument */
      pg_FonctUtil_XZEA_44 ( vl_Etat, &vl_Resultat, vl_machine );
      XZST_03EcritureTrace ( XZSTC_FONCTION,  "OUT : XZEA_Etat_EqtVEN %d", vl_Resultat );
   }
}
/* definition de fonctions externes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Abonnement aux etats des equipements de la GTC de Nice ( Equipements GTC Renovee)  >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int    XZEA44_Abt_Etat_EqtVEN ( XDY_Booleen    va_Abonnement,
                                XDY_Datagroup  pa_DGInit,
                                XDY_Fonction   pa_FonctUtilisateur,
                                XDY_Nom        pa_NomSite,
 							    XDY_Entier	   va_Domaine,
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
static char      *version  = "@(#)xzea44.c    1.1 09/21/07 : XZEA44_Abt_Etat_EqtVEN ";
XDY_Datagroup    pl_NomDG;
int              vl_ValRet = XDC_OK;
T_BOOL           vl_EtatDg = FALSE;
XDY_Entier       vl_TypeEqt = va_TypeEqt;
XDY_Entier       vl_NumEqt  = va_NumEqt;

    XZST_03EcritureTrace ( XZSTC_FONCTION, 
                           "IN  : XZEA44_Abt_Etat_EqtVEN Abt:%d DGInit:%s Fonct:%ld NomSite:%s TypeEqt:%d NumEqt:%d.",
                           va_Abonnement,
                           pa_DGInit,
                           pa_FonctUtilisateur,
                           pa_NomSite,
                           va_TypeEqt,
                           va_NumEqt );

    /* Memorisation en globale du nom de la fonction utilisateur */        
    pg_FonctUtil_XZEA_44 = pa_FonctUtilisateur;

    /*A Si demande d'abonnement */
    if ( va_Abonnement == XZEAC_DEBUT_ABONNEMENT )
    {
        /*A  Armement Callback sur Reception XDM_ETAT_EQUIPEMENT */
        if ( vg_CB_XZEA44 == NULL )
        {
            if ( ( vg_CB_XZEA44 = TipcSrvProcessCbCreate ( XDM_IdentMsg(XDM_ETAT_EQUIPEMENT),
                                                           XZEA_Etat_EqtVEN,
                                                           (T_CB_ARG)NULL ) ) == NULL ) 
            {    
                /*B Ecriture Trace */
                XZST_03EcritureTrace ( XZSTC_WARNING, "Echec armement Callback sur XDM_ETAT_EQUIPEMENT." );
                vl_ValRet = XDC_NOK;
            }
            else
            {    
                /*B Ecriture Trace */
                XZST_03EcritureTrace ( XZSTC_DEBUG1, "Armement Callback sur XDM_ETAT_EQUIPEMENT." );
            }
        }

        /*A Abonnement au Datagroup XDG_ETAT_EQUIPEMENT<NomSite> */
        /*A Construction du nom du DataGroup */
        sprintf ( pl_NomDG, "%s_%s", XDG_ETAT_EQUIPEMENT, pa_NomSite );
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
							   T_IPC_FT_INT4,  va_Domaine,
                               T_IPC_FT_INT4, vl_TypeEqt,
                               T_IPC_FT_INT4, vl_NumEqt,
                               T_IPC_FT_INT4, XDC_GTC_EQUIPEMENT,
                               NULL))
        {
            /*A Trace envoi message XDM_EInit_IHM_NICERenovee non effectue */        
            XZST_03EcritureTrace(XZSTC_WARNING, "Echec envoi message XDM_EInit_IHM_NICERenoveeRenovee.");
        }
        else
        {
            /*A Trace envoi message XDM_EInit_IHM_NICERenovee */        
            XZST_03EcritureTrace(XZSTC_DEBUG1, "Envoi du message XDM_EInit_IHM_NICERenovee sur %s.", pl_NomDG );
        }
    }
    else
    {
        if ( ( va_Abonnement == XZEAC_FERMER_ABONNEMENT ) && ( vg_CB_XZEA44 != NULL ) )
        {
            /*A  Desarmement Callback sur Reception XDM_ETAT_EQUIPEMENT */
            if ( TutCbDestroy ( vg_CB_XZEA44 ) == FALSE )  
            {    
                /*B Ecriture Trace */
                XZST_03EcritureTrace ( XZSTC_WARNING, "Echec desarmement Callback sur XDM_ETAT_EQUIPEMENT ." );
                vl_ValRet = XDC_NOK;
            }
            else
            {
                XZST_03EcritureTrace ( XZSTC_DEBUG1, "Desarmement Callback sur XDM_ETAT_EQUIPEMENT ." );
                vg_CB_XZEA44 = NULL;
            }
        }

        /*A Desabonnement au Datagroup XDG_ETAT_EQUIPEMENT<NomSite> */
        /*A Construction du nom du DataGroup */
        sprintf ( pl_NomDG, "%s_%s", XDG_ETAT_EQUIPEMENT, pa_NomSite );
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

    XZST_03EcritureTrace ( XZSTC_FONCTION, "OUT : XZEA44_Abt_Etat_EqtVEN Retour:%d.", vl_ValRet );
    *pa_Resultat = vl_ValRet;
    return ( vl_ValRet );                

}


