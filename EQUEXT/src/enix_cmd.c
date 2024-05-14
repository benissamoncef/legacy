/*E*/
/*Fichier :  @(#)enix_cmd.c	1.6      Release : 1.6        Date : 06/16/09
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TENIX * FICHIER enix_cmd.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module des fonction de commande ( Callback RTworks )
* pour la tache TENIX, gtc Nice
*
------------------------------------------------------
* HISTORIQUE :
*
* AAZ       version 1.5 20 Juin 2007    : GTC r�nov�e DEM 664
* JBL		Oct 2008 : Suppression code inutilise
* ADB           Mai 2009 : Mise a jour des traces
* ADB       22 mars 2010 : FT 3448 - Gestion des etats liaisons
* ABE		24-03-2010 : FT 33450 - correction probl�me d'envoi de TC eqp sur les �changeurs
* ABE       26-03-2013 : gestion du retour � la normale avec ajout de sc�narios pour le domaine exploitation
* SFO           06-10-2014 : Adaptation pour compilation sous linux
* JMG	15/05/20     : EEEEEEEEEEMS 1.7
* GR    09/07/2020   : EMS 1.8
* AMI 28/01/21 : SAE_191: chantier date: encapsulation localtime pour hiniber TZ car nbseconde deja en TZ
* LCL		20/12/21	: MOVIS P2 ems SAE-312
------------------------------------------------------*/
 
/* fichiers inclus */

#include "enix_cmd.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "@(#)enix_cmd.c	1.6 06/16/09      : enix_cmd" ;

/* declaration de fonctions internes */

short determiner_mode_rafale(ENIX_DONNEES_Equipement * pl_Equipement)
{
	short vl_ModeRafale = XDC_NON;
	if (pl_Equipement == NULL)
	{
		vl_ModeRafale = XDC_NON;
	}
	else if (pl_Equipement->InfoRecue == XDC_OUI)
	{
		vl_ModeRafale = XDC_OUI;
	}
	else 
	{
		vl_ModeRafale = determiner_mode_rafale(pl_Equipement->Suivant);
	}
	return vl_ModeRafale;
}

/* definition de fonctions externes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande Init IHM GTCNice
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void    et_Init_IHM_TNIC_cb ( T_IPC_CONN                   va_Cnx,
                              T_IPC_CONN_PROCESS_CB_DATA   pa_Data,
                              T_CB_ARG                     pa_ARG)
 
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
*  Fonction appel�e lors de la r�ception du message
*  XDM_EInit_IHM_TNIC
*
* FONCTION 
*  Renvoyer l'etat  demande
*  Cette fonction envoi l'etat des equipements connus suite
*  a une demande d'initialisation
*
------------------------------------------------------*/
{
    static char *version = "@(#)enix_cmd.c	1.6 06/16/09      : et_Init_IHM_TNIC_cb " ;

   #ifndef _TIBCO_EMS
   T_STR			pl_DGInit;
    T_INT4                    vl_NumTE,   /* Identifiant de l'ouvrage */
					          vl_Domaine, /* Domaine concerne */
                              vl_TypeEqt, /* Type d'ouvrage */
                              vl_TypeInit;
   #else
   XDY_Datagroup                pl_DGInit;
    XDY_Entier                    vl_NumTE,   /* Identifiant de l'ouvrage */
					          vl_Domaine, /* Domaine concerne */
                              vl_TypeEqt, /* Type d'ouvrage */
                              vl_TypeInit;
   #endif
    
    int                       vl_NbEqt;
    ENIX_DONNEES_Equipement * pl_Equipement;
    ENIX_DONNEES_Sequence *   pl_Sequence;
    ENIX_DONNEES_SIG *        pl_SIG;
    ENIX_DONNEES_ECL *        pl_ECL;
    ENIX_DONNEES_VEN *        pl_VEN;
    ENIX_DONNEES_ENR *        pl_ENR;
    ENIX_DONNEES_INC *        pl_INC;
    ENIX_DONNEES_EXP *        pl_EXP;
    ENIX_DONNEES_SYS *        pl_SYS;
    ENIX_DONNEES_LTN *        pl_LTN;
    ENIX_DONNEES_Domaine *    pl_Domaine;

	short 					  vl_ModeRafale = XDC_OUI;
	
	XZST_03EcritureTrace(XZSTC_INFO, "Reception d'un message XDM_EInit_IHM_TNICRenovee" );

    /*A Recuperation des parametres du message XDM_EInit_IHM_TNICRenovee */
    if(!TipcMsgRead( pa_Data -> msg,
                     T_IPC_FT_STR, &pl_DGInit,
                     T_IPC_FT_INT4, &vl_Domaine,
                     T_IPC_FT_INT4, &vl_TypeEqt,
                     T_IPC_FT_INT4, &vl_NumTE,
                     T_IPC_FT_INT4, &vl_TypeInit,
                     NULL))
    {
		/* Trace : Impossibilite de lire le message */
		XZST_03EcritureTrace(XZSTC_WARNING, "Messsage XDM_EInit_IHM_TNICRenovee non lu " );
		return;
    }

    XZST_03EcritureTrace(XZSTC_INFO, "Messsage XDM_EInit_IHM_TNICRenovee : vl_Domaine : %d - vl_TypeEqt : %d - vl_NumTE : %d - vl_TypeInit <%d>", vl_Domaine, vl_TypeEqt, vl_NumTE, vl_TypeInit);
    
    /* Trace du message en entree de la fonction */
    XZST_03EcritureTrace(XZSTC_FONCTION ," IN  : et_Init_IHM_TNIC_cb DGInit: %s Eqt: %d",
                          pl_DGInit,
                          vl_NumTE );
    /* AAZ Ajout SIG, ECL, VEN, ENR, INC, EXP, SYS */
	switch( vl_Domaine )
	{
	case( XDC_EQT_SIG ) :
		XZST_03EcritureTrace(XZSTC_INFO, "et_Init_IHM_TNIC_cb : Domaine signalisation" );
	    for ( pl_SIG = pg_debutSIG; pl_SIG != NULL; pl_SIG = pl_SIG->Suivant )
	    {
			XZST_03EcritureTrace(XZSTC_INFO, "AAZ XDM_EInit_IHM_TNIC Signalisation." );
	        switch ( vl_TypeInit )
	        {
	            case XZETC_SEQUENCE :
	                /* Envoi etat Sequence dans XDM_ETAT_SEQSIG sur pl_DGInit */
	                if  ( pl_SIG->InfoRecue == XDC_OUI && vl_TypeEqt == pl_SIG->TypeOuvrage && 
	                    ((pl_SIG->Numero == vl_NumTE) || (vl_NumTE == XDC_EQTALL)))
	                {
						XZST_03EcritureTrace(XZSTC_DEBUG1, "XDM_EInit_IHM_TNIC : Envoi de la sequence %d sur %s (message %d)", pl_SIG->DerniereSequence, pl_DGInit, XDM_ETAT_SEQSIG );
	                    if (!TipcSrvMsgWrite ( pl_DGInit,
	                                           XDM_IdentMsg(XDM_ETAT_SEQSIG),
	                                           XDM_FLG_SRVMSGWRITE,
	                                           T_IPC_FT_INT4, pl_SIG->TypeOuvrage,
	                                           T_IPC_FT_INT4, pl_SIG->Numero,
	                                           T_IPC_FT_INT4, pl_SIG->DerniereSequence,
	                                           T_IPC_FT_INT4, 0,
	                                           T_IPC_FT_INT4, 0,
	                                           NULL ) )
	                    {
	                        XZST_03EcritureTrace(XZSTC_WARNING, "Envoi message XDM_ETAT_SEQSIG non effectue." );
	                    }
	                }
	                break;

	            case XZETC_EQUIPEMENT :
	                if  (pl_SIG->InfoRecue == XDC_OUI && vl_TypeEqt == pl_SIG->TypeOuvrage && 
	                    ((pl_SIG->Numero == vl_NumTE) || (vl_NumTE == XDC_EQTALL)) )
	                {
						vl_ModeRafale = XDC_NON;
	                    for ( pl_Equipement =  pl_SIG->ListeEquipement;
	                          pl_Equipement != NULL; 
	                          pl_Equipement =  pl_Equipement->Suivant )
	                    {
						
							if (pl_Equipement->InfoRecue != XDC_OUI)
								continue;
	                        /* Envoi etat Equipement dans XDM_ETAT_EQUIPEMENT sur pl_DGInit */
							XZST_03EcritureTrace(XZSTC_DEBUG1, "SIGNALISATION : Envoi XDM_ETAT_EQUIPEMENT - ouvrage : (%d,%d) - equipement (%d,%d)",
												pl_Equipement->TypeOuvrage,
												pl_SIG->Numero,
												pl_Equipement->TypeEquipement,
												pl_Equipement->NumeroEquipement);
							vl_ModeRafale = determiner_mode_rafale(pl_Equipement->Suivant);
	                        if (!TipcSrvMsgWrite ( pl_DGInit,
	                                               XDM_IdentMsg(XDM_ETAT_EQUIPEMENT),
	                                               XDM_FLG_SRVMSGWRITE,
	                                               T_IPC_FT_INT4, XDC_EQT_SIG,
	                                               T_IPC_FT_INT4, pl_Equipement->TypeOuvrage,
	                                               T_IPC_FT_INT4, pl_SIG->Numero,
	                                               T_IPC_FT_INT4, pl_Equipement->TypeEquipement,
	                                               T_IPC_FT_INT4, pl_Equipement->NumeroEquipement,
	                                               T_IPC_FT_INT4, pl_Equipement->EtatEquipement,
	                                               T_IPC_FT_INT4, pl_Equipement->Etat_Service,
	                                               T_IPC_FT_INT4, pl_Equipement->Mode,
	                                               T_IPC_FT_INT4, pl_Equipement->EtatDisc,
												   T_IPC_FT_INT2, vl_ModeRafale,
	                                               NULL ) )
	                        {
	                            XZST_03EcritureTrace(XZSTC_WARNING, "Envoi message XDM_ETAT_EQUIPEMENT non effectue." );
	                        }
	                    }
	                }
	                break;

	            case XZETC_DOMAINE :
	                if  (pl_SIG->InfoRecue == XDC_OUI && vl_TypeEqt == pl_SIG->TypeOuvrage && 
	                    ((pl_SIG->Numero == vl_NumTE) || (vl_NumTE == XDC_EQTALL)) )
	                {
	                    /* Envoi etat Domaine dans XDM_ETAT_DOMAINE sur pl_DGInit */
						XZST_03EcritureTrace(XZSTC_DEBUG1, "Envoi XDM_ETAT_DOMAINE" );
	                    if (!TipcSrvMsgWrite( pl_DGInit,
	                                          XDM_IdentMsg(XDM_ETAT_DOMAINE),
	                                          XDM_FLG_SRVMSGWRITE,
	                                           T_IPC_FT_INT4, pl_SIG->TypeOuvrage,
	                                           T_IPC_FT_INT4, pl_SIG->Numero,
	                                           T_IPC_FT_INT4, XDC_EQT_SIG,
	                                           T_IPC_FT_INT4, pl_SIG->ModeDomaine,
	                                           T_IPC_FT_INT4, pl_SIG->DerniereSequence,
	                                          NULL ) )
	                    {
	                        XZST_03EcritureTrace(XZSTC_WARNING, "Envoi message XDM_ETAT_DOMAINE non effectue." );
	                    }
	                }
	                break;
	        }
		}
		break;
	case ( XDC_EQT_ECL ) :
		XZST_03EcritureTrace(XZSTC_INFO, "et_Init_IHM_TNIC_cb : Domaine eclairage" );
	    for ( pl_ECL = pg_debutECL; pl_ECL != NULL; pl_ECL = pl_ECL->Suivant )
	    {
			XZST_03EcritureTrace(XZSTC_INFO, "AAZ XDM_EInit_IHM_TNIC Eclairage." );
	        switch ( vl_TypeInit )
	        {
	            case XZETC_SEQUENCE :
	                /* Envoi etat Sequence dans XDM_ETAT_SEQECL sur pl_DGInit */
	                if  (pl_ECL->InfoRecue == XDC_OUI && vl_TypeEqt == pl_ECL->TypeOuvrage && 
	                    ((pl_ECL->Numero == vl_NumTE) || (vl_NumTE == XDC_EQTALL)))
	                {
						XZST_03EcritureTrace(XZSTC_DEBUG1, "Envoi XDM_ETAT_SEQECL" );
	                    if (!TipcSrvMsgWrite ( pl_DGInit,
	                                           XDM_IdentMsg(XDM_ETAT_SEQECL),
	                                           XDM_FLG_SRVMSGWRITE,
	                                           T_IPC_FT_INT4, pl_ECL->TypeOuvrage,
	                                           T_IPC_FT_INT4, pl_ECL->Numero,
	                                           T_IPC_FT_INT4, pl_ECL->DerniereSequence,
	                                           T_IPC_FT_INT4, 0,
	                                           T_IPC_FT_INT4, 0,
	                                           NULL ) )
	                    {
	                        XZST_03EcritureTrace(XZSTC_WARNING, "Envoi message XDM_ETAT_SEQECL non effectue." );
	                    }
	                }
	                break;

	            case XZETC_EQUIPEMENT :
	                if  (pl_ECL->InfoRecue == XDC_OUI && vl_TypeEqt == pl_ECL->TypeOuvrage && 
	                    ((pl_ECL->Numero == vl_NumTE) || (vl_NumTE == XDC_EQTALL)) )
	                {
						vl_ModeRafale = XDC_NON;
	                    for ( pl_Equipement =  pl_ECL->ListeEquipement;
	                          pl_Equipement != NULL; 
	                          pl_Equipement =  pl_Equipement->Suivant )
	                    {
							if (pl_Equipement->InfoRecue != XDC_OUI)
								continue;
	                        /* Envoi etat Equipement dans XDM_ETAT_EQUIPEMENT sur pl_DGInit */
							XZST_03EcritureTrace(XZSTC_DEBUG1, "ECLAIRAGE : Envoi XDM_ETAT_EQUIPEMENT - ouvrage : (%d,%d) - equipement (%d,%d)",
												pl_Equipement->TypeOuvrage,
												pl_ECL->Numero,
												pl_Equipement->TypeEquipement,
												pl_Equipement->NumeroEquipement);
							vl_ModeRafale = determiner_mode_rafale(pl_Equipement->Suivant);
	                        if (!TipcSrvMsgWrite ( pl_DGInit,
	                                               XDM_IdentMsg(XDM_ETAT_EQUIPEMENT),
	                                               XDM_FLG_SRVMSGWRITE,
	                                               T_IPC_FT_INT4, XDC_EQT_ECL,
	                                               T_IPC_FT_INT4, pl_Equipement->TypeOuvrage,
	                                               T_IPC_FT_INT4, pl_ECL->Numero,
	                                               T_IPC_FT_INT4, pl_Equipement->TypeEquipement,
	                                               T_IPC_FT_INT4, pl_Equipement->NumeroEquipement,
	                                               T_IPC_FT_INT4, pl_Equipement->EtatEquipement,
	                                               T_IPC_FT_INT4, pl_Equipement->Etat_Service,
	                                               T_IPC_FT_INT4, pl_Equipement->Mode,
	                                               T_IPC_FT_INT4, pl_Equipement->EtatDisc,
												   T_IPC_FT_INT2, vl_ModeRafale,
	                                               NULL ) )
	                        {
	                            XZST_03EcritureTrace(XZSTC_WARNING, "Envoi message XDM_ETAT_EQUIPEMENT non effectue." );
	                        }
						}
	                }
	                break;

	            case XZETC_DOMAINE :
	                if  (pl_ECL->InfoRecue == XDC_OUI && vl_TypeEqt == pl_ECL->TypeOuvrage && 
	                    ((pl_ECL->Numero == vl_NumTE) || (vl_NumTE == XDC_EQTALL)) )
	                {
	                    /* Envoi etat Domaine dans XDM_ETAT_DOMAINE sur pl_DGInit */
						XZST_03EcritureTrace(XZSTC_DEBUG1, "Envoi XDM_ETAT_DOMAINE" );
	                    if (!TipcSrvMsgWrite( pl_DGInit,
	                                          XDM_IdentMsg(XDM_ETAT_DOMAINE),
	                                          XDM_FLG_SRVMSGWRITE,
	                                           T_IPC_FT_INT4, pl_ECL->TypeOuvrage,
	                                           T_IPC_FT_INT4, pl_ECL->Numero,
	                                           T_IPC_FT_INT4, XDC_EQT_ECL,
	                                           T_IPC_FT_INT4, pl_ECL->ModeDomaine,
	                                           T_IPC_FT_INT4, pl_ECL->DerniereSequence,
	                                          NULL ) )
	                    {
	                        XZST_03EcritureTrace(XZSTC_WARNING, "Envoi message XDM_ETAT_DOMAINE non effectue." );
	                    }
	                }
	                break;
	        }
	    }
		break;
	case ( XDC_EQT_VEN ) :
		XZST_03EcritureTrace(XZSTC_INFO, "et_Init_IHM_TNIC_cb : Domaine ventilation" );
	    for ( pl_VEN = pg_debutVEN; pl_VEN != NULL; pl_VEN = pl_VEN->Suivant )
	    {
			XZST_03EcritureTrace(XZSTC_INFO, "AAZ XDM_EInit_IHM_TNIC Ventilation." );
	        switch ( vl_TypeInit )
	        {
	            case XZETC_SEQUENCE :
	                /* Envoi etat Sequence dans XDM_ETAT_SEQVEN sur pl_DGInit */
	                if  (pl_VEN->InfoRecue == XDC_OUI && vl_TypeEqt == pl_VEN->TypeOuvrage && 
	                    ((pl_VEN->Numero == vl_NumTE) || (vl_NumTE == XDC_EQTALL)))
	                {
						XZST_03EcritureTrace(XZSTC_DEBUG1, "Envoi XDM_ETAT_SEQVEN" );
	                    if (!TipcSrvMsgWrite ( pl_DGInit,
	                                           XDM_IdentMsg(XDM_ETAT_SEQVEN),
	                                           XDM_FLG_SRVMSGWRITE,
	                                           T_IPC_FT_INT4, pl_VEN->TypeOuvrage,
	                                           T_IPC_FT_INT4, pl_VEN->Numero,
	                                           T_IPC_FT_INT4, pl_VEN->DerniereSequence,
	                                           T_IPC_FT_INT4, 0,
	                                           T_IPC_FT_INT4, 0,
	                                           NULL ) )
	                    {
	                        XZST_03EcritureTrace(XZSTC_WARNING, "Envoi message XDM_ETAT_SEQVEN non effectue." );
	                    }
	                }
	                break;

	            case XZETC_EQUIPEMENT :
	                if  (pl_VEN->InfoRecue == XDC_OUI && vl_TypeEqt == pl_VEN->TypeOuvrage && 
	                    ((pl_VEN->Numero == vl_NumTE) || (vl_NumTE == XDC_EQTALL)) )
	                {
	                    vl_ModeRafale = XDC_NON;
						for ( pl_Equipement =  pl_VEN->ListeEquipement;
	                          pl_Equipement != NULL; 
	                          pl_Equipement =  pl_Equipement->Suivant )
	                    {
							if (pl_Equipement->InfoRecue != XDC_OUI)
								continue;
	                        /* Envoi etat Equipement dans XDM_ETAT_EQUIPEMENT sur pl_DGInit */
							XZST_03EcritureTrace(XZSTC_DEBUG1, "VENTILATION : Envoi XDM_ETAT_EQUIPEMENT - ouvrage : (%d,%d) - equipement (%d,%d)",
												pl_Equipement->TypeOuvrage,
												pl_VEN->Numero,
												pl_Equipement->TypeEquipement,
												pl_Equipement->NumeroEquipement);
							vl_ModeRafale = determiner_mode_rafale(pl_Equipement->Suivant);
	                        if (!TipcSrvMsgWrite ( pl_DGInit,
	                                               XDM_IdentMsg(XDM_ETAT_EQUIPEMENT),
	                                               XDM_FLG_SRVMSGWRITE,
	                                               T_IPC_FT_INT4, XDC_EQT_VEN,
	                                               T_IPC_FT_INT4, pl_Equipement->TypeOuvrage,
	                                               T_IPC_FT_INT4, pl_VEN->Numero,
	                                               T_IPC_FT_INT4, pl_Equipement->TypeEquipement,
	                                               T_IPC_FT_INT4, pl_Equipement->NumeroEquipement,
	                                               T_IPC_FT_INT4, pl_Equipement->EtatEquipement,
	                                               T_IPC_FT_INT4, pl_Equipement->Etat_Service,
	                                               T_IPC_FT_INT4, pl_Equipement->Mode,
	                                               T_IPC_FT_INT4, pl_Equipement->EtatDisc,
												   T_IPC_FT_INT2, vl_ModeRafale,
	                                               NULL ) )
	                        {
	                            XZST_03EcritureTrace(XZSTC_WARNING, "Envoi message XDM_ETAT_EQUIPEMENT non effectue." );
	                        }
	                    }
	                }
	                break;

	            case XZETC_DOMAINE :
	                if  (pl_VEN->InfoRecue == XDC_OUI && vl_TypeEqt == pl_VEN->TypeOuvrage && 
	                    ((pl_VEN->Numero == vl_NumTE) || (vl_NumTE == XDC_EQTALL)) )
	                {
	                    /* Envoi etat Domaine dans XDM_ETAT_DOMAINE sur pl_DGInit */
						XZST_03EcritureTrace(XZSTC_DEBUG1, "Envoi XDM_ETAT_DOMAINE" );
	                    if (!TipcSrvMsgWrite( pl_DGInit,
	                                          XDM_IdentMsg(XDM_ETAT_DOMAINE),
	                                          XDM_FLG_SRVMSGWRITE,
                                              T_IPC_FT_INT4, pl_VEN->TypeOuvrage,
	                                          T_IPC_FT_INT4, pl_VEN->Numero,
	                                          T_IPC_FT_INT4, XDC_EQT_VEN,
	                                          T_IPC_FT_INT4, pl_VEN->ModeDomaine,
	                                          T_IPC_FT_INT4, pl_VEN->DerniereSequence,
	                                          NULL ) )
	                    {
	                        XZST_03EcritureTrace(XZSTC_WARNING, "Envoi message XDM_ETAT_DOMAINE non effectue." );
	                    }
	                }
	                break;
	        }
	    }	
		break;
	case ( XDC_EQT_ENR ) :
		XZST_03EcritureTrace(XZSTC_INFO, "et_Init_IHM_TNIC_cb : Domaine energie" );
	    for ( pl_ENR = pg_debutENR; pl_ENR != NULL; pl_ENR = pl_ENR->Suivant )
	    {
			XZST_03EcritureTrace(XZSTC_INFO, "AAZ XDM_EInit_IHM_TNIC Energie." );
	        switch ( vl_TypeInit )
	        {
	            case XZETC_SEQUENCE :
	                /* Envoi etat Sequence dans XDM_ETAT_SEQENR sur pl_DGInit */
	                if  (pl_ENR->InfoRecue == XDC_OUI && vl_TypeEqt     == pl_ENR->TypeOuvrage && 
	                    ((pl_ENR->Numero == vl_NumTE) || (vl_NumTE == XDC_EQTALL)))
	                {
						XZST_03EcritureTrace(XZSTC_DEBUG1, "Envoi XDM_ETAT_SEQENR" );
	                    if (!TipcSrvMsgWrite ( pl_DGInit,
	                                           XDM_IdentMsg(XDM_ETAT_SEQENR),
	                                           XDM_FLG_SRVMSGWRITE,
	                                           T_IPC_FT_INT4, pl_ENR->TypeOuvrage,
	                                           T_IPC_FT_INT4, pl_ENR->Numero,
	                                           T_IPC_FT_INT4, pl_ENR->DerniereSequence,
	                                           T_IPC_FT_INT4, 0,
	                                           T_IPC_FT_INT4, 0,
	                                           NULL ) )
	                    {
	                        XZST_03EcritureTrace(XZSTC_WARNING, "Envoi message XDM_ETAT_SEQENR non effectue." );
	                    }
	                }
	                break;

	            case XZETC_EQUIPEMENT :
	                if  (pl_ENR->InfoRecue == XDC_OUI && (vl_TypeEqt     == pl_ENR->TypeOuvrage) && 
	                    ((pl_ENR->Numero == vl_NumTE) || (vl_NumTE == XDC_EQTALL)) )
	                {
						vl_ModeRafale = XDC_NON;
	                    for ( pl_Equipement =  pl_ENR->ListeEquipement;
	                          pl_Equipement != NULL; 
	                          pl_Equipement =  pl_Equipement->Suivant )
	                    {
							if (pl_Equipement->InfoRecue != XDC_OUI)
								continue;
	                        /* Envoi etat Equipement dans XDM_ETAT_EQUIPEMENT sur pl_DGInit */
							XZST_03EcritureTrace(XZSTC_DEBUG1, "ENERGIE : Envoi XDM_ETAT_EQUIPEMENT - ouvrage : (%d,%d) - equipement (%d,%d)",
												pl_Equipement->TypeOuvrage,
												pl_ENR->Numero,
												pl_Equipement->TypeEquipement,
												pl_Equipement->NumeroEquipement);
							vl_ModeRafale = determiner_mode_rafale(pl_Equipement->Suivant);
	                        if (!TipcSrvMsgWrite ( pl_DGInit,
	                                               XDM_IdentMsg(XDM_ETAT_EQUIPEMENT),
	                                               XDM_FLG_SRVMSGWRITE,
	                                               T_IPC_FT_INT4, XDC_EQT_ENR,
	                                               T_IPC_FT_INT4, pl_Equipement->TypeOuvrage,
	                                               T_IPC_FT_INT4, pl_ENR->Numero,
	                                               T_IPC_FT_INT4, pl_Equipement->TypeEquipement,
	                                               T_IPC_FT_INT4, pl_Equipement->NumeroEquipement,
	                                               T_IPC_FT_INT4, pl_Equipement->EtatEquipement,
	                                               T_IPC_FT_INT4, pl_Equipement->Etat_Service,
	                                               T_IPC_FT_INT4, pl_Equipement->Mode,
	                                               T_IPC_FT_INT4, pl_Equipement->EtatDisc,
												   T_IPC_FT_INT2, vl_ModeRafale,
	                                               NULL ) )
	                        {
	                            XZST_03EcritureTrace(XZSTC_WARNING, "Envoi message XDM_ETAT_EQUIPEMENT non effectue." );
	                        }
	                    }
	                }
	                break;

	            case XZETC_DOMAINE :
	                if  (pl_ENR->InfoRecue == XDC_OUI && (vl_TypeEqt     == pl_ENR->TypeOuvrage) && 
	                    ((pl_ENR->Numero == vl_NumTE) || (vl_NumTE == XDC_EQTALL)) )
	                {
	                    /* Envoi etat Domaine dans XDM_ETAT_DOMAINE sur pl_DGInit */
						XZST_03EcritureTrace(XZSTC_DEBUG1, "Envoi XDM_ETAT_DOMAINE" );
	                    if (!TipcSrvMsgWrite( pl_DGInit,
	                                          XDM_IdentMsg(XDM_ETAT_DOMAINE),
	                                          XDM_FLG_SRVMSGWRITE,
	                                           T_IPC_FT_INT4, pl_ENR->TypeOuvrage,
	                                           T_IPC_FT_INT4, pl_ENR->Numero,
	                                           T_IPC_FT_INT4, XDC_EQT_ENR,
	                                           T_IPC_FT_INT4, pl_ENR->ModeDomaine,
	                                           T_IPC_FT_INT4, pl_ENR->DerniereSequence,
	                                          NULL ) )
	                    {
	                        XZST_03EcritureTrace(XZSTC_WARNING, "Envoi message XDM_ETAT_DOMAINE non effectue." );
	                    }
	                }
	                break;
	        }
	    }
		break;
	case ( XDC_EQT_INC ) :
		XZST_03EcritureTrace(XZSTC_INFO, "et_Init_IHM_TNIC_cb : Domaine incendie" );
	    for ( pl_INC = pg_debutINC; pl_INC != NULL; pl_INC = pl_INC->Suivant )
	    {
			XZST_03EcritureTrace(XZSTC_INFO, "AAZ XDM_EInit_IHM_TNIC Protection incendie." );
	        switch ( vl_TypeInit )
	        {
	            case XZETC_SEQUENCE :
	                /* Envoi etat Sequence dans XDM_ETAT_SEQINC sur pl_DGInit */
	                if  (pl_INC->InfoRecue == XDC_OUI && (vl_TypeEqt     == pl_INC->TypeOuvrage) && 
	                    ((pl_INC->Numero == vl_NumTE) || (vl_NumTE == XDC_EQTALL)))
	                {
						XZST_03EcritureTrace(XZSTC_DEBUG1, "Envoi XDM_ETAT_SEQINC" );
	                    if (!TipcSrvMsgWrite ( pl_DGInit,
	                                           XDM_IdentMsg(XDM_ETAT_SEQINC),
	                                           XDM_FLG_SRVMSGWRITE,
	                                           T_IPC_FT_INT4, pl_INC->TypeOuvrage,
	                                           T_IPC_FT_INT4, pl_INC->Numero,
	                                           T_IPC_FT_INT4, pl_INC->DerniereSequence,
	                                           T_IPC_FT_INT4, 0,
	                                           T_IPC_FT_INT4, 0,
	                                           NULL ) )
	                    {
	                        XZST_03EcritureTrace(XZSTC_WARNING, "Envoi message XDM_ETAT_SEQINC non effectue." );
	                    }
	                }
	                break;

	            case XZETC_EQUIPEMENT :
	                if  (pl_INC->InfoRecue == XDC_OUI && (vl_TypeEqt     == pl_INC->TypeOuvrage) && 
	                    ((pl_INC->Numero == vl_NumTE) || (vl_NumTE == XDC_EQTALL)) )
	                {
						vl_ModeRafale = XDC_NON;
	                    for ( pl_Equipement =  pl_INC->ListeEquipement;
	                          pl_Equipement != NULL; 
	                          pl_Equipement =  pl_Equipement->Suivant )
	                    {
							if (pl_Equipement->InfoRecue != XDC_OUI)
								continue;
	                        /* Envoi etat Equipement dans XDM_ETAT_EQUIPEMENT sur pl_DGInit */
							XZST_03EcritureTrace(XZSTC_DEBUG1, "INCENDIE : Envoi XDM_ETAT_EQUIPEMENT - ouvrage : (%d,%d) - equipement (%d,%d)",
												pl_Equipement->TypeOuvrage,
												pl_INC->Numero,
												pl_Equipement->TypeEquipement,
												pl_Equipement->NumeroEquipement);
							vl_ModeRafale = determiner_mode_rafale(pl_Equipement->Suivant);
	                        if (!TipcSrvMsgWrite ( pl_DGInit,
	                                               XDM_IdentMsg(XDM_ETAT_EQUIPEMENT),
	                                               XDM_FLG_SRVMSGWRITE,
	                                               T_IPC_FT_INT4, XDC_EQT_INC,
	                                               T_IPC_FT_INT4, pl_Equipement->TypeOuvrage,
	                                               T_IPC_FT_INT4, pl_INC->Numero,
	                                               T_IPC_FT_INT4, pl_Equipement->TypeEquipement,
	                                               T_IPC_FT_INT4, pl_Equipement->NumeroEquipement,
	                                               T_IPC_FT_INT4, pl_Equipement->EtatEquipement,
	                                               T_IPC_FT_INT4, pl_Equipement->Etat_Service,
	                                               T_IPC_FT_INT4, pl_Equipement->Mode,
	                                               T_IPC_FT_INT4, pl_Equipement->EtatDisc,
												   T_IPC_FT_INT2, vl_ModeRafale,
	                                               NULL ) )
	                        {
	                            XZST_03EcritureTrace(XZSTC_WARNING, "Envoi message XDM_ETAT_EQUIPEMENT non effectue." );
	                        }
	                    }
	                }
	                break;

	            case XZETC_DOMAINE :
	                if  (pl_INC->InfoRecue == XDC_OUI && (vl_TypeEqt     == pl_INC->TypeOuvrage) && 
	                    ((pl_INC->Numero == vl_NumTE) || (vl_NumTE == XDC_EQTALL)) )
	                {
	                    /* Envoi etat Domaine dans XDM_ETAT_DOMAINE sur pl_DGInit */
						XZST_03EcritureTrace(XZSTC_DEBUG1, "Envoi XDM_ETAT_DOMAINE" );
	                    if (!TipcSrvMsgWrite( pl_DGInit,
	                                          XDM_IdentMsg(XDM_ETAT_DOMAINE),
	                                          XDM_FLG_SRVMSGWRITE,
	                                           T_IPC_FT_INT4, pl_INC->TypeOuvrage,
	                                           T_IPC_FT_INT4, pl_INC->Numero,
	                                           T_IPC_FT_INT4, XDC_EQT_INC,
	                                           T_IPC_FT_INT4, pl_INC->ModeDomaine,
	                                           T_IPC_FT_INT4, pl_INC->DerniereSequence,
	                                          NULL ) )
	                    {
	                        XZST_03EcritureTrace(XZSTC_WARNING, "Envoi message XDM_ETAT_DOMAINE non effectue." );
	                    }
	                }
	                break;
	        }
	    }
		break;
	case ( XDC_EQT_EXP ) :
		XZST_03EcritureTrace(XZSTC_INFO, "et_Init_IHM_TNIC_cb : Domaine exploitation" );
	    for ( pl_EXP = pg_debutEXP; pl_EXP != NULL; pl_EXP = pl_EXP->Suivant )
	    {
			XZST_03EcritureTrace(XZSTC_INFO, "AAZ XDM_EInit_IHM_TNIC Exploitation." );
	        switch ( vl_TypeInit )
	        {
	            case XZETC_SEQUENCE :
	                /* Envoi etat Sequence dans XDM_ETAT_SEQEXP sur pl_DGInit */
	                if  (pl_EXP->InfoRecue == XDC_OUI && (vl_TypeEqt     == pl_EXP->TypeOuvrage) && 
	                    ((pl_EXP->Numero == vl_NumTE) || (vl_NumTE == XDC_EQTALL)))
	                {
						XZST_03EcritureTrace(XZSTC_DEBUG1, "Envoi XDM_ETAT_SEQEXP" );
	                    if (!TipcSrvMsgWrite ( pl_DGInit,
	                                           XDM_IdentMsg(XDM_ETAT_SEQEXP),
	                                           XDM_FLG_SRVMSGWRITE,
	                                           T_IPC_FT_INT4, pl_EXP->TypeOuvrage,
	                                           T_IPC_FT_INT4, pl_EXP->Numero,
	                                           T_IPC_FT_INT4, pl_EXP->DerniereSequence,
	                                           T_IPC_FT_INT4, 0,
	                                           T_IPC_FT_INT4, 0,
	                                           NULL ) )
	                    {
	                        XZST_03EcritureTrace(XZSTC_WARNING, "Envoi message XDM_ETAT_SEQEXP non effectue." );
	                    }
	                }
	                break;
				case XZETC_EQUIPEMENT :
					if (pl_EXP->InfoRecue == XDC_OUI)
					{
						if (vl_TypeEqt == XDC_EQT_LI)
						{
							/* cas particulier des lits d'arret */
							vl_ModeRafale = XDC_NON;
		                    for ( pl_Equipement =  pl_EXP->ListeEquipement;
		                          pl_Equipement != NULL && pl_Equipement->TypeEquipement == XDC_EQT_LIT; 
		                          pl_Equipement =  pl_Equipement->Suivant )
							{
								if (pl_Equipement->InfoRecue != XDC_OUI)
									continue;
		                        /* Envoi etat Equipement dans XDM_ETAT_EQUIPEMENT sur pl_DGInit */
								XZST_03EcritureTrace(XZSTC_DEBUG1, "EXP-Lits d'arrets : Envoi XDM_ETAT_EQUIPEMENT - ouvrage : (%d,%d) - equipement (%d,%d)",
												pl_Equipement->TypeOuvrage,
												pl_EXP->Numero,
												pl_Equipement->TypeEquipement,
												pl_Equipement->NumeroEquipement);
								vl_ModeRafale = determiner_mode_rafale(pl_Equipement->Suivant);
		                        if (!TipcSrvMsgWrite ( pl_DGInit,
		                                               XDM_IdentMsg(XDM_ETAT_EQUIPEMENT),
		                                               XDM_FLG_SRVMSGWRITE,
		                                               T_IPC_FT_INT4, XDC_EQT_EXP,
		                                               T_IPC_FT_INT4, pl_Equipement->TypeOuvrage,
		                                               T_IPC_FT_INT4, pl_EXP->Numero,
		                                               T_IPC_FT_INT4, pl_Equipement->TypeEquipement,
		                                               T_IPC_FT_INT4, pl_Equipement->NumeroEquipement,
		                                               T_IPC_FT_INT4, pl_Equipement->EtatEquipement,
		                                               T_IPC_FT_INT4, pl_Equipement->Etat_Service,
		                                               T_IPC_FT_INT4, pl_Equipement->Mode,
		                                               T_IPC_FT_INT4, pl_Equipement->EtatDisc,
													   T_IPC_FT_INT2, vl_ModeRafale,
		                                               NULL ) )							
								{
									XZST_03EcritureTrace(XZSTC_WARNING, "Envoi message XDM_ETAT_EQUIPEMENT non effectue." );
								}
							}
						}
						else if (vl_TypeEqt == pl_EXP->TypeOuvrage && (pl_EXP->Numero == vl_NumTE || vl_NumTE == XDC_EQTALL))
						{
							/* cas des equipements exploitation lies a un tube ou a un echangeur */
							vl_ModeRafale = XDC_NON;
		                    for ( pl_Equipement =  pl_EXP->ListeEquipement;
		                          pl_Equipement != NULL; 
		                          pl_Equipement =  pl_Equipement->Suivant )
		                    {
								if (pl_Equipement->InfoRecue != XDC_OUI)
									continue;
		                        /* Envoi etat Equipement dans XDM_ETAT_EQUIPEMENT sur pl_DGInit */
								XZST_03EcritureTrace(XZSTC_DEBUG1, "EXPLOITATION : Envoi XDM_ETAT_EQUIPEMENT - ouvrage : (%d,%d) - equipement (%d,%d)",
												pl_Equipement->TypeOuvrage,
												pl_EXP->Numero,
												pl_Equipement->TypeEquipement,
												pl_Equipement->NumeroEquipement);
								vl_ModeRafale = determiner_mode_rafale(pl_Equipement->Suivant);
		                        if (!TipcSrvMsgWrite ( pl_DGInit,
		                                               XDM_IdentMsg(XDM_ETAT_EQUIPEMENT),
		                                               XDM_FLG_SRVMSGWRITE,
		                                               T_IPC_FT_INT4, XDC_EQT_EXP,
		                                               T_IPC_FT_INT4, pl_Equipement->TypeOuvrage,
		                                               T_IPC_FT_INT4, pl_EXP->Numero,
		                                               T_IPC_FT_INT4, pl_Equipement->TypeEquipement,
		                                               T_IPC_FT_INT4, pl_Equipement->NumeroEquipement,
		                                               T_IPC_FT_INT4, pl_Equipement->EtatEquipement,
		                                               T_IPC_FT_INT4, pl_Equipement->Etat_Service,
		                                               T_IPC_FT_INT4, pl_Equipement->Mode,
		                                               T_IPC_FT_INT4, pl_Equipement->EtatDisc,
													   T_IPC_FT_INT2, vl_ModeRafale,
		                                               NULL ) )
		                        {
		                            XZST_03EcritureTrace(XZSTC_WARNING, "Envoi message XDM_ETAT_EQUIPEMENT non effectue." );
		                        }
		                    }						
						}
					}
	                break;

	            case XZETC_DOMAINE :
	                if  (pl_EXP->InfoRecue == XDC_OUI && (vl_TypeEqt     == pl_EXP->TypeOuvrage) && 
	                    ((pl_EXP->Numero == vl_NumTE) || (vl_NumTE == XDC_EQTALL)) )
	                {
	                    /* Envoi etat Domaine dans XDM_ETAT_DOMAINE sur pl_DGInit */
						XZST_03EcritureTrace(XZSTC_DEBUG1, "Envoi XDM_ETAT_DOMAINE" );
	                    if (!TipcSrvMsgWrite( pl_DGInit,
	                                          XDM_IdentMsg(XDM_ETAT_DOMAINE),
	                                          XDM_FLG_SRVMSGWRITE,
	                                           T_IPC_FT_INT4, pl_EXP->TypeOuvrage,
	                                           T_IPC_FT_INT4, pl_EXP->Numero,
	                                           T_IPC_FT_INT4, XDC_EQT_EXP,
	                                           T_IPC_FT_INT4, pl_EXP->ModeDomaine,
	                                           T_IPC_FT_INT4, pl_EXP->DerniereSequence,
	                                          NULL ) )
	                    {
	                        XZST_03EcritureTrace(XZSTC_WARNING, "Envoi message XDM_ETAT_DOMAINE non effectue." );
	                    }
	                }
	                break;
	        }
	    }
		break;
	case ( XDC_EQT_SYS ) :
		XZST_03EcritureTrace(XZSTC_INFO, "et_Init_IHM_TNIC_cb : Domaine systeme" );
	    for ( pl_SYS = pg_debutSYS; pl_SYS != NULL; pl_SYS = pl_SYS->Suivant )
	    {
			XZST_03EcritureTrace(XZSTC_INFO, "AAZ XDM_EInit_IHM_TNIC Systeme." );
	        switch ( vl_TypeInit )
	        {
	            case XZETC_EQUIPEMENT :
	                if  (pl_SYS->InfoRecue == XDC_OUI && (vl_TypeEqt     == pl_SYS->TypeOuvrage) && 
	                    ((pl_SYS->Numero == vl_NumTE) || (vl_NumTE == XDC_EQTALL)) )
	                {
						vl_ModeRafale = XDC_NON;
						for ( pl_Equipement =  pl_SYS->ListeEquipement;
	                          pl_Equipement != NULL; 
	                          pl_Equipement =  pl_Equipement->Suivant )
	                    {
							if (pl_Equipement->InfoRecue != XDC_OUI)
								continue;
	                        /* Envoi etat Equipement dans XDM_ETAT_EQUIPEMENT sur pl_DGInit */
							XZST_03EcritureTrace(XZSTC_DEBUG1, "SYSTEME : Envoi XDM_ETAT_EQUIPEMENT - ouvrage : (%d,%d) - equipement (%d,%d)",
												pl_Equipement->TypeOuvrage,
												pl_SYS->Numero,
												pl_Equipement->TypeEquipement,
												pl_Equipement->NumeroEquipement);
							vl_ModeRafale = determiner_mode_rafale(pl_Equipement->Suivant);
	                        if (!TipcSrvMsgWrite ( pl_DGInit,
	                                               XDM_IdentMsg(XDM_ETAT_EQUIPEMENT),
	                                               XDM_FLG_SRVMSGWRITE,
	                                               T_IPC_FT_INT4, XDC_EQT_SYS,
	                                               T_IPC_FT_INT4, pl_Equipement->TypeOuvrage,
	                                               T_IPC_FT_INT4, pl_SYS->Numero,
	                                               T_IPC_FT_INT4, pl_Equipement->TypeEquipement,
	                                               T_IPC_FT_INT4, pl_Equipement->NumeroEquipement,
	                                               T_IPC_FT_INT4, pl_Equipement->EtatEquipement,
	                                               T_IPC_FT_INT4, pl_Equipement->Etat_Service,
	                                               T_IPC_FT_INT4, pl_Equipement->Mode,
	                                               T_IPC_FT_INT4, pl_Equipement->EtatDisc,
												   T_IPC_FT_INT2, vl_ModeRafale,
	                                               NULL ) )
	                        {
	                            XZST_03EcritureTrace(XZSTC_WARNING, "Envoi message XDM_ETAT_EQUIPEMENT non effectue." );
	                        }
	                    }


	                }
	                break;

	            case XZETC_DOMAINE :
	                if  (pl_SYS->InfoRecue == XDC_OUI && (vl_TypeEqt     == pl_SYS->TypeOuvrage) && 
	                    ((pl_SYS->Numero == vl_NumTE) || (vl_NumTE == XDC_EQTALL)) )
	                {
	                    /* Envoi etat Domaine dans XDM_ETAT_DOMAINE sur pl_DGInit */
						XZST_03EcritureTrace(XZSTC_DEBUG1, "Envoi XDM_ETAT_DOMAINE" );
	                    if (!TipcSrvMsgWrite( pl_DGInit,
	                                          XDM_IdentMsg(XDM_ETAT_DOMAINE),
	                                          XDM_FLG_SRVMSGWRITE,
	                                           T_IPC_FT_INT4, pl_SYS->TypeOuvrage,
	                                           T_IPC_FT_INT4, pl_SYS->Numero,
	                                           T_IPC_FT_INT4, XDC_EQT_SYS,
	                                           T_IPC_FT_INT4, pl_SYS->ModeDomaine,
	                                           T_IPC_FT_INT4, pl_SYS->DerniereSequence,
	                                          NULL ) )
	                    {
	                        XZST_03EcritureTrace(XZSTC_WARNING, "Envoi message XDM_ETAT_DOMAINE non effectue." );
	                    }
	                }
	                break;
	        }
	    }
			    ex_env_liaison ( vg_nom_machine_TWWI0, vg_etat_SockTWWI0, vg_TypeEqt_LiaiTWWI0, vg_NumEqt_LiaiTWWI0);
			    ex_env_liaison ( vg_nom_machine_TWWI1, vg_etat_SockTWWI1, vg_TypeEqt_LiaiTWWI1, vg_NumEqt_LiaiTWWI1);
		break;
	case ( XDC_EQT_LTN ):
		XZST_03EcritureTrace(XZSTC_INFO, "et_Init_IHM_TNIC_cb : Domaine LTN" );
	    for ( pl_LTN = pg_debutLTN; pl_LTN != NULL; pl_LTN = pl_LTN->Suivant )
	    {
			XZST_03EcritureTrace(XZSTC_INFO, "AAZ XDM_EInit_IHM_TNIC Locaux techniques." );
	        switch ( vl_TypeInit )
	        {
	            case XZETC_EQUIPEMENT :
	                if  (pl_LTN->InfoRecue == XDC_OUI && (vl_TypeEqt     == pl_LTN->TypeOuvrage) && 
	                    ((pl_LTN->Numero == vl_NumTE) || (vl_NumTE == XDC_EQTALL)) )
	                {
						vl_ModeRafale = XDC_NON;
	                    for ( pl_Equipement =  pl_LTN->ListeEquipement;
	                          pl_Equipement != NULL; 
	                          pl_Equipement =  pl_Equipement->Suivant )
	                    {
							if (pl_Equipement->InfoRecue != XDC_OUI)
								continue;
	                        /* Envoi etat Equipement dans XDM_ETAT_EQUIPEMENT sur pl_DGInit */
							XZST_03EcritureTrace(XZSTC_DEBUG1, "LTN : Envoi XDM_ETAT_EQUIPEMENT - ouvrage : (%d,%d) - equipement (%d,%d)",
												pl_Equipement->TypeOuvrage,
												pl_LTN->Numero,
												pl_Equipement->TypeEquipement,
												pl_Equipement->NumeroEquipement);
							vl_ModeRafale = determiner_mode_rafale(pl_Equipement->Suivant);
	                        if (!TipcSrvMsgWrite ( pl_DGInit,
	                                               XDM_IdentMsg(XDM_ETAT_EQUIPEMENT),
	                                               XDM_FLG_SRVMSGWRITE,
	                                               T_IPC_FT_INT4, XDC_EQT_LTN,
	                                               T_IPC_FT_INT4, pl_Equipement->TypeOuvrage,
	                                               T_IPC_FT_INT4, pl_LTN->Numero,
	                                               T_IPC_FT_INT4, pl_Equipement->TypeEquipement,
	                                               T_IPC_FT_INT4, pl_Equipement->NumeroEquipement,
	                                               T_IPC_FT_INT4, pl_Equipement->EtatEquipement,
	                                               T_IPC_FT_INT4, pl_Equipement->Etat_Service,
	                                               T_IPC_FT_INT4, pl_Equipement->Mode,
	                                               T_IPC_FT_INT4, pl_Equipement->EtatDisc,
												   T_IPC_FT_INT2, vl_ModeRafale,
	                                               NULL ) )
	                        {
	                            XZST_03EcritureTrace(XZSTC_WARNING, "Envoi message XDM_ETAT_EQUIPEMENT non effectue." );
	                        }
	                    }
	                }
	                break;

	            case XZETC_DOMAINE :
	                if  (pl_LTN->InfoRecue == XDC_OUI && (vl_TypeEqt     == pl_LTN->TypeOuvrage) && 
	                    ((pl_LTN->Numero == vl_NumTE) || (vl_NumTE == XDC_EQTALL)) )
	                {
	                    /* Envoi etat Domaine dans XDM_ETAT_DOMAINE sur pl_DGInit */
						XZST_03EcritureTrace(XZSTC_DEBUG1, "Envoi XDM_ETAT_DOMAINE" );
	                    if (!TipcSrvMsgWrite( pl_DGInit,
	                                          XDM_IdentMsg(XDM_ETAT_DOMAINE),
	                                          XDM_FLG_SRVMSGWRITE,
	                                           T_IPC_FT_INT4, pl_LTN->TypeOuvrage,
	                                           T_IPC_FT_INT4, pl_LTN->Numero,
	                                           T_IPC_FT_INT4, XDC_EQT_LTN,
	                                           T_IPC_FT_INT4, pl_LTN->ModeDomaine,
	                                           T_IPC_FT_INT4, pl_LTN->DerniereSequence,
	                                          NULL ) )
	                    {
	                        XZST_03EcritureTrace(XZSTC_WARNING, "Envoi message XDM_ETAT_DOMAINE non effectue." );
	                    }
	                }
	                break;
	        }
	    }
		break;
	}
    XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: et_Init_IHM_TNIC_cb ");
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Envoi un message de commande unitaire a la tache maitre Wizcon ou esclave Wizcon.
*
* Remplace et_envoi_cmd_panneau
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int      et_envoi_cmd_unitaire( int         va_socket,
                                XDY_Mot     va_TypeOuv,
								XDY_Mot *   pa_TypeEqt,
                                XDY_Mot *   pa_Eqt,
                                XDY_Mot *   pa_NumCmd,
                                int         va_NbCommande,
                                XDY_Mot     va_NumEqt,
                                XDY_Mot     va_Domaine,
                                XDY_Mot     va_NumSeq)
/*
* ARGUMENTS EN ENTREE :
*
* ARGUMENTS EN SORTIE :
*   aucun
*
*
* CODE RETOUR : 
*   XDC_OK  : succes
*   XDC_NOK : Erreur
*
* CONDITION D'UTILISATION
*  
*   
*
* FONCTION 
*  
*
------------------------------------------------------*/
{            
	static char *version = "@(#)enix_cmd.c  1.4 10/23/02      : et_envoi_cmd_unitaire " ;

	XZEXT_MSG_SOCKET    vl_Msg; 
	short               vl_PosMsg = 0,
	                    vl_Index,
	                    vl_NbrEqt;          
	div_t               vl_Position;
	short               vl_Cmd;
	short               vl_TabCmd[XZETC_MAX_UNITAIRE/XZETC_NB_ETAT_UNITAIRE];
	XDY_Datagroup       pl_NomDG;
	XDY_NomSite         pl_NomSite;
	char                vl_EtatSeq[20];
	XDY_Mot		        vl_TypeEqtSeq;
	int                 vl_Id_Trame;
	XDY_Mot             vl_Id_Domaine;

    XZST_03EcritureTrace( XZSTC_INFO, "IN : et_envoi_cmd_unitaire");
	XZST_03EcritureTrace( XZSTC_INFO, "Demande d'envoi d'une commande unitaire:  nbCommandes = %d", va_NbCommande);
	
    vl_Id_Trame = 1;
    /*A Recuperation du site de la machine */
    if ( XZSC_04NomSite(pl_NomSite) != XDC_OK)
    {
        /* Ecriture Trace */
        XZST_03EcritureTrace( XZSTC_WARNING, "Recuperation du Nom du site impossible ." );
    }

    /* Equipements pour les domaines SIG, ECL, VEN, ENR, INC */
    vl_Id_Domaine = 0;
    switch ( va_Domaine )
    {
        case XDC_EQT_TUBE :
            sprintf(vl_EtatSeq, "%d",  XDM_ETAT_SEQUENCE_TUBE);
            vl_TypeEqtSeq = XDM_ETAT_SEQUENCE_TUBE;
            break;
        case XDC_EQT_ECH :
            sprintf(vl_EtatSeq, "%d",  XDM_ETAT_SEQUENCE_ECH);
            vl_TypeEqtSeq = XDM_ETAT_SEQUENCE_ECH;
            break;
        case XDC_EQT_SIG :
            sprintf(vl_EtatSeq, "%s",  XDG_ETAT_SEQ_SIG);
            vl_TypeEqtSeq = XDM_ETAT_SEQSIG;
            vl_Id_Domaine = 1;
            break;
        case XDC_EQT_ECL :
            sprintf(vl_EtatSeq, "%s",  XDG_ETAT_SEQ_ECL);
            vl_TypeEqtSeq = XDM_ETAT_SEQECL;
            vl_Id_Domaine = 3;
            break;
        case XDC_EQT_VEN :
            sprintf(vl_EtatSeq, "%s",  XDG_ETAT_SEQ_VEN);
            vl_TypeEqtSeq = XDM_ETAT_SEQVEN;
            vl_Id_Domaine = 2;
            break;
        case XDC_EQT_ENR :
            sprintf(vl_EtatSeq, "%s",  XDG_ETAT_SEQ_ENR);
            vl_TypeEqtSeq = XDM_ETAT_SEQENR;
            vl_Id_Domaine = 4;
            break;
        case XDC_EQT_INC :
            sprintf(vl_EtatSeq, "%s",  XDG_ETAT_SEQ_INC);
            vl_TypeEqtSeq = XDM_ETAT_SEQINC;
            vl_Id_Domaine = 9;
            break;
        case XDC_EQT_EXP :
            sprintf(vl_EtatSeq, "%s",  XDG_ETAT_SEQ_EXP);
            vl_TypeEqtSeq = XDM_ETAT_SEQEXP;
            vl_Id_Domaine = 5;
            break;
        case XDC_EQT_LTN :
            sprintf(vl_EtatSeq, "");
#ifdef _HPUX_SOURCE
            vl_TypeEqtSeq = NULL;
#else
            vl_TypeEqtSeq = 0;
#endif
            vl_Id_Domaine = 7;
            break;
        case XDC_EQT_SYS :
            sprintf(vl_EtatSeq, "");
#ifdef _HPUX_SOURCE
            vl_TypeEqtSeq = NULL;
#else
            vl_TypeEqtSeq = 0;
#endif
            vl_Id_Domaine = 8;
            break;
        default :
            sprintf(vl_EtatSeq, "");
            /* Ecriture dans le fichier trace en cas de type equipement inconnu */
            XZST_03EcritureTrace( XZSTC_FONCTION, "Impossible d'�mettre la trame <%> vers TEXAO. Type equipement <%d> non trait�", vl_Msg,  va_TypeOuv );
            return(XDC_NOK);
    }

    /*A Construction de la trame */
    vl_PosMsg = sprintf ( vl_Msg,   "%d|%d|%d|",vl_Id_Trame,va_TypeOuv,vl_Id_Domaine);


    XZST_03EcritureTrace( XZSTC_FONCTION, "AAZ va_NbCommande : %d", va_NbCommande);

    for ( vl_Index = 0;vl_Index < va_NbCommande-1;vl_Index++ )
    {
        /* Ajout dans la trame des commandes */
        vl_PosMsg += sprintf( vl_Msg+vl_PosMsg,"%d~", pa_TypeEqt[vl_Index]);
    }
    /* le dernier */
    vl_PosMsg += sprintf( vl_Msg+vl_PosMsg,"%d|", pa_TypeEqt[va_NbCommande-1]);


    for ( vl_Index = 0;vl_Index < va_NbCommande-1;vl_Index++ )
    {
        /* Ajout dans la trame des commandes */
        vl_PosMsg += sprintf( vl_Msg+vl_PosMsg,"%d~", pa_Eqt[vl_Index]);
    }

    /* le dernier */
    vl_PosMsg += sprintf( vl_Msg+vl_PosMsg,"%d|", pa_Eqt[va_NbCommande-1]);

    for ( vl_Index = 0;vl_Index < va_NbCommande-1;vl_Index++ )
    {
        /* Ajout dans la trame des commandes */
        vl_PosMsg += sprintf( vl_Msg+vl_PosMsg,"%d~", pa_NumCmd[vl_Index]);
    }

    /* le dernier */
    vl_PosMsg += sprintf( vl_Msg+vl_PosMsg,"%d|", pa_NumCmd[va_NbCommande-1]);

    /* Fin de trame */
    vl_PosMsg += sprintf( vl_Msg+vl_PosMsg,"%s", XZEXC_FIN_MSG_SOCK_WIZCON);


    XZST_03EcritureTrace( XZSTC_FONCTION, "AAZ trame : %s", vl_Msg);


    /*A Envoi de la trame */
    fcntl ( va_socket, F_SETFL, O_NDELAY | fcntl ( va_socket, F_GETFL, 0 ) );

    if ( send ( va_socket , vl_Msg , vl_PosMsg , 0 ) <= 0 )                                 
    {
        /* Ecriture dans le fichier trace en cas d'�chec */
        XZST_03EcritureTrace( XZSTC_WARNING, "Impossible d'�mettre la trame <%s> vers maitre Wizcon ou esclave Wizcon.", vl_Msg );
        return(XDC_NOK);
    }
    else
    {   
        /*B Ecriture dans le fichier trace en cas de succes */
        XZST_03EcritureTrace( XZSTC_DEBUG1, "Trame emise vers maitre Wizcon ou esclave Wizcon: %s.", vl_Msg );

        /* Envoi etat Sequence dans XDM_ETAT_SEQUENCE sur XDG_ETAT_SEQUENCE */
        sprintf ( pl_NomDG, "%s_%s", vl_EtatSeq, pl_NomSite );

        XZST_03EcritureTrace( XZSTC_DEBUG1, "AZA envoi cmd unitaire, seq -> smart sockets pl_NomDG %s, message = %d, va_TypeOuv = %d, va_NumEqt = %d, va_NumSeq = %d, vl_EtatSeq = %d", 
				pl_NomDG, 
				vl_TypeEqtSeq, 
				va_TypeOuv,
				va_NumEqt,
				va_NumSeq,
				vl_EtatSeq); 

        if (!TipcSrvMsgWrite ( pl_NomDG,
                               XDM_IdentMsg( vl_TypeEqtSeq),
                               XDM_FLG_SRVMSGWRITE,
                               T_IPC_FT_INT4, va_TypeOuv,
                               T_IPC_FT_INT4, va_NumEqt,
                               T_IPC_FT_INT4, va_NumSeq,
                               T_IPC_FT_INT4, 0,
                               T_IPC_FT_INT4, 0,
                               NULL ) )
       {
           XZST_03EcritureTrace(XZSTC_WARNING, "Envoi message XDM_ETAT_SEQUENCE non effectue." );
       }
        return(XDC_OK);
    }
}

/*------------------------------------------------------
* SERVICE RENDU : 
*  Arme le timer de verification des sequences
*
------------------------------------------------------*/
int et_armer_timer_verification_sequence(XDY_Mot    va_TypeOuv,
										 XDY_Mot    va_Domaine,
										 XDY_Eqt    va_NumEqt,
										 XDY_Mot    va_NumSeq)
{
	int    					vl_jourSemaine;
	double 					vl_Horodate;
	XZEXT_MSG_SOCKET       	pl_MsgTimer;

	/*A Demande de reveil pour time_out discordance */
	/*B Lecture de l'horodate actuelle */
	XZSM_07LireHorodate ( XZSMC_TYPE_SYSTEME, &vl_jourSemaine, &vl_Horodate );
	sprintf ( pl_MsgTimer, "%s %lf %d Nice%hd_%hd_%hd_%hd %s",
			  XZEXC_DEMANDE_REVEIL,
			  vl_Horodate + (XZETC_TIMEOUT_DISC * 10),
			  0,
			  va_TypeOuv,
			  va_NumEqt,
			  va_NumSeq,
			  va_Domaine,
			  XZEXC_FIN_MSG_SOCK );

	/* Envoie a la TETIM une demande reveil */              
	fcntl ( vg_SockTETIM, F_SETFL, O_NDELAY | fcntl ( vg_SockTETIM, F_GETFL, 0 ) );
	if ( send ( vg_SockTETIM, pl_MsgTimer, strlen (pl_MsgTimer) , 0 ) <= 0 )
	{
	   XZST_03EcritureTrace ( XZSTC_WARNING, "Impossible d'emettre un message sur la socket Timer." );
	   return XDC_NOK;
	}
	XZST_03EcritureTrace ( XZSTC_DEBUG1, "Positionnement du timer pour le timeout de reception de l'�tat de la sequence" );
	return XDC_OK;
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Envoi un message de commande sequence a maitre Wizcon ou esclave Wizcon.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int    et_envoi_cmd_sequence ( int        va_socket,
                               XDY_Mot    va_TypeOuv,
                               XDY_Mot    va_Domaine,
                               XDY_Eqt    va_NumEqt,
                               XDY_Mot    va_NumSeq )
/*
* ARGUMENTS EN ENTREE :
*
* ARGUMENTS EN SORTIE :
*   aucun
*
*
* CODE RETOUR : 
*   XDC_OK  : succes
*   XDC_NOK : Erreur
*
* CONDITION D'UTILISATION
*  
*   
*
* FONCTION 
*  
*
------------------------------------------------------*/
{             
	static char            *version = "@(#)enix_cmd.c	1.6 06/16/09      : et_envoi_cmd_sequence " ;

	XZEXT_MSG_SOCKET       vl_Msg; 
	short                  vl_PosMsg = 0;            
	short                  vl_Cmd;
	XDY_Datagroup          pl_NomDG;
	XDY_NomSite            pl_NomSite;
	char                   vl_NomDG[20];
	int					   vl_IdMessage = 0;
	XDY_Mot                vl_Id_Domaine;

    XZST_03EcritureTrace( XZSTC_INFO, "IN : et_envoi_cmd_sequence");

    /*A Recuperation du site de la machine */
    if ( XZSC_04NomSite(pl_NomSite) != XDC_OK)
    {
        /* Ecriture Trace */
        XZST_03EcritureTrace( XZSTC_WARNING, "Recuperation du Nom du site impossible ." );
    }

    vl_Id_Domaine = 0;
    switch ( va_Domaine )
    {
        case XDC_EQT_SIG :
            vl_Id_Domaine = 1;
			vl_IdMessage = XDM_ETAT_SEQSIG;
            sprintf(vl_NomDG, "%s",  XDG_ETAT_SEQ_SIG);
            break;
        case XDC_EQT_ECL :
            vl_Id_Domaine = 3;
			vl_IdMessage = XDM_ETAT_SEQECL;
            sprintf(vl_NomDG, "%s",  XDG_ETAT_SEQ_ECL);
            break;
        case XDC_EQT_VEN :
            vl_Id_Domaine = 2;
			vl_IdMessage = XDM_ETAT_SEQVEN;
            sprintf(vl_NomDG, "%s",  XDG_ETAT_SEQ_VEN);
            break;
        case XDC_EQT_ENR :
            vl_Id_Domaine = 4;
			vl_IdMessage = XDM_ETAT_SEQENR;
            sprintf(vl_NomDG, "%s",  XDG_ETAT_SEQ_ENR);
            break;
        case XDC_EQT_INC :
            vl_Id_Domaine = 9;
            sprintf(vl_NomDG, "%s",  XDG_ETAT_SEQ_INC);
			vl_IdMessage = XDM_ETAT_SEQINC;
            break;
        case XDC_EQT_EXP :
            vl_Id_Domaine = 5;
			vl_IdMessage = XDM_ETAT_SEQEXP;
            sprintf(vl_NomDG, "%s",  XDG_ETAT_SEQ_EXP);
            break;
        case XDC_EQT_LTN :
            vl_Id_Domaine = 7;
            sprintf(vl_NomDG, "0");
            break;
        case XDC_EQT_SYS :
            vl_Id_Domaine = 8;
            sprintf(vl_NomDG, "0");
            break;
        default :
            /* Ecriture dans le fichier trace en cas de type equipement inconnu */
            XZST_03EcritureTrace( XZSTC_FONCTION, "Impossible d'�mettre la trame <%> vers TEXAO. Type equipement <%d> non trait�", vl_Msg,  va_TypeOuv );
            return(XDC_NOK);
    }

    /*A Construction de la trame */
    vl_PosMsg = sprintf ( vl_Msg, "%d|%d|%d|%d|%d|%s",
                          ENIX_CMD_SEQUENCE,
                          va_TypeOuv,
                          va_NumEqt,
                          vl_Id_Domaine,
                          va_NumSeq,
                          XZEXC_FIN_MSG_SOCK_WIZCON );

    /*A Envoi de la trame */
    fcntl ( va_socket, F_SETFL, O_NDELAY | fcntl ( va_socket, F_GETFL, 0 ) );

    if ( send ( va_socket , vl_Msg , vl_PosMsg , 0 ) <= 0 )                                   
    {
        /* Ecriture dans le fichier trace en cas d'�chec */
        XZST_03EcritureTrace( XZSTC_FONCTION, "Impossible d'�mettre la trame <%> vers maitre Wizcon ou esclave Wizcon.", vl_Msg );
        return(XDC_NOK);
    }
    else
    {   
        /*B Ecriture dans le fichier trace en cas de succes */
        XZST_03EcritureTrace( XZSTC_DEBUG1, "Trame emise vers maitre Wizcon ou esclave Wizcon: <%s>.", vl_Msg );
        /* Envoi etat Sequence dans XDM_ETAT_SEQUENCE sur XDG_ETAT_SEQUENCE */
        sprintf ( pl_NomDG, "%s_%s", vl_NomDG, pl_NomSite );
        XZST_03EcritureTrace( XZSTC_DEBUG1, "Publication d'un message SM (DG = %s, ID = %d)", 
						pl_NomDG,
						vl_IdMessage);
        if (!TipcSrvMsgWrite( pl_NomDG,
                              XDM_IdentMsg(vl_IdMessage),
                              XDM_FLG_SRVMSGWRITE,
                              T_IPC_FT_INT4, va_TypeOuv,
                              T_IPC_FT_INT4, va_NumEqt,
                              T_IPC_FT_INT4, va_NumSeq,
                              T_IPC_FT_INT4, 0,
                              T_IPC_FT_INT4, 0,
                              NULL ) )
        {
            XZST_03EcritureTrace(XZSTC_WARNING, "Envoi message XDM_ETAT_SEQUENCE non effectue." );
        }
        return(XDC_OK);
    }
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Envoi un message de commande domaine a la tache maitre Wizcon ou esclave Wizcon.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int      et_envoi_cmd_domaine ( int         va_socket,
                                XDY_Mot     va_TypeOuvrage,
                                XDY_Mot     va_NumEqt,
                                XDY_Mot     va_Domaine,
                                XDY_Mot     va_Mode,
                                XDY_Mot     va_NumSeq)
/*
* ARGUMENTS EN ENTREE :
*
* ARGUMENTS EN SORTIE :
*   aucun
*
*
* CODE RETOUR : 
*   XDC_OK  : succes
*   XDC_NOK : Erreur
*
* CONDITION D'UTILISATION
*  
*   
*
* FONCTION 
*  
*
------------------------------------------------------*/
{
	static char *version = "@(#)enix_cmd.c	1.6 06/16/09      : et_envoi_cmd_domaine " ;

	XZEXT_MSG_SOCKET    vl_Msg; 
	short               vl_PosMsg = 0,
	                    vl_Index,
	                    vl_NbrEqt;          
	div_t               vl_Position;
	short               vl_Cmd;
	short               vl_TabCmd[XZETC_MAX_UNITAIRE/XZETC_NB_ETAT_UNITAIRE];
	XDY_Datagroup       pl_NomDG;
	XDY_NomSite         pl_NomSite;
	char                vl_EtatSeq[20];
	XDY_Mot             vl_Id_Domaine;

    XZST_03EcritureTrace( XZSTC_INFO, "IN : et_envoi_cmd_domaine");

    /*A Recuperation du site de la machine */
    if ( XZSC_04NomSite(pl_NomSite) != XDC_OK)
    {
        /* Ecriture Trace */
        XZST_03EcritureTrace( XZSTC_WARNING, "Recuperation du Nom du site impossible ." );
    } 

    vl_Id_Domaine = 0;
    switch ( va_Domaine )
    {
        case XDC_EQT_TUBE :
            sprintf(vl_EtatSeq, "%d",  XDM_ETAT_SEQUENCE_TUBE);
            break;
        case XDC_EQT_ECH :
            sprintf(vl_EtatSeq, "%d",  XDM_ETAT_SEQUENCE_ECH);
            break;
        case XDC_EQT_SIG :
            vl_Id_Domaine = 1;
            sprintf(vl_EtatSeq, "%d",  XDM_ETAT_SEQSIG);
            break;
        case XDC_EQT_ECL :
            vl_Id_Domaine = 3;
            sprintf(vl_EtatSeq, "%d",  XDM_ETAT_SEQECL);
            break;
        case XDC_EQT_VEN :
            vl_Id_Domaine = 2;
            sprintf(vl_EtatSeq, "%d",  XDM_ETAT_SEQVEN);
            break;
        case XDC_EQT_ENR :
            vl_Id_Domaine = 4;
            sprintf(vl_EtatSeq, "%d",  XDM_ETAT_SEQENR);
            break;
        case XDC_EQT_INC :
            vl_Id_Domaine = 9;
            sprintf(vl_EtatSeq, "%d",  XDM_ETAT_SEQINC);
            break;
        case XDC_EQT_EXP :
            vl_Id_Domaine = 5;
            sprintf(vl_EtatSeq, "%d",  XDM_ETAT_SEQEXP);
            break;
        case XDC_EQT_LTN :
            vl_Id_Domaine = 7;
            sprintf(vl_EtatSeq, "0");
            break;
        case XDC_EQT_SYS :
            vl_Id_Domaine = 8;
            sprintf(vl_EtatSeq, "0");
            break;
        default :
            /* Ecriture dans le fichier trace en cas de type equipement inconnu */
            XZST_03EcritureTrace( XZSTC_FONCTION, "Impossible d'�mettre la trame <%> vers TEXAO. Type equipement <%d> non trait�", vl_Msg,  va_TypeOuvrage );
            return(XDC_NOK);
    }

          
    /*A Construction de la trame */
    vl_PosMsg = sprintf ( vl_Msg,   "%d|%d|%d|%d|%d|%s",
                          ENIX_CMD_DOMAINE,
                          va_TypeOuvrage,
                          va_NumEqt,
                          vl_Id_Domaine,
                          va_Mode,
                          XZEXC_FIN_MSG_SOCK_WIZCON );

    /*A Envoi de la trame */
    fcntl ( va_socket, F_SETFL, O_NDELAY | fcntl ( va_socket, F_GETFL, 0 ) );

    if ( send ( va_socket , vl_Msg , vl_PosMsg , 0 ) <= 0 )                                 
    {
        /* Ecriture dans le fichier trace en cas d'�chec */
        XZST_03EcritureTrace( XZSTC_FONCTION, "Impossible d'�mettre la trame <%> vers maitre Wizcon ou esclave Wizcon.", vl_Msg );
        return(XDC_NOK);
    }
    else
    {   
        /*B Ecriture dans le fichier trace en cas de succes */
        XZST_03EcritureTrace( XZSTC_DEBUG1, "Trame emise vers maitre Wizcon ou esclave Wizcon: %s.", vl_Msg );
       
        /* Envoi etat Sequence dans XDM_ETAT_SEQUENCE sur XDG_ETAT_SEQUENCE */
        sprintf ( pl_NomDG, "%s_%s", vl_EtatSeq, pl_NomSite );
        if (!TipcSrvMsgWrite ( pl_NomDG,
                               XDM_IdentMsg( vl_EtatSeq),
                               XDM_FLG_SRVMSGWRITE,
                               T_IPC_FT_INT4, va_TypeOuvrage,
                               T_IPC_FT_INT4, va_NumEqt,
                               T_IPC_FT_INT4, va_NumSeq,
                               T_IPC_FT_INT4, 0,
                               T_IPC_FT_INT4, 0,
                               NULL ) )
        {
            XZST_03EcritureTrace(XZSTC_WARNING, "Envoi message XDM_ETAT_SEQUENCE non effectue." );
        }
        return(XDC_OK);
    }
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Message de contr�le.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
void    et_controle_cb ( T_IPC_CONN                   va_Cnx,
                         T_IPC_CONN_PROCESS_CB_DATA   pa_Data,
                         T_CB_ARG                     pa_ARG)
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
*  Fonction appel�e lors de la r�ception du message
*  XDM_ECMD_EQPT 
*
* FONCTION 
* Met a jour les variables d'etat dans les structures en memoire
*
------------------------------------------------------*/
{
    static char *version = "@(#)enix_cmd.c	1.6 06/16/09      : et_controle_cb" ;

        #ifndef _TIBCO_EMS
        T_STR			     vl_NomMachine;
	T_INT4                       vl_Etat;
	T_INT4                       vl_TypeEqt;
	T_INT4                       vl_NumEqt;
        #else
        XDY_NomMachine	             vl_NomMachine;
	XDY_Entier                       vl_Etat;
	XDY_Entier                       vl_TypeEqt;
	XDY_Entier                       vl_NumEqt;
        #endif	

	ENIX_DONNEES_Tube *          pl_Tube;
	ENIX_DONNEES_Echangeur *     pl_Ech;
	ENIX_DONNEES_Equipement *    pl_Equipement;
	ENIX_DONNEES_Capteur *       pl_Capt;
	int                          vl_EqtTrouve = XDC_FAUX;
	/* Domaines SIG, ECL, ... */
	ENIX_DONNEES_SIG *           pl_SIG;
	ENIX_DONNEES_ECL *           pl_ECL;
	ENIX_DONNEES_VEN *           pl_VEN;
	ENIX_DONNEES_ENR *           pl_ENR;
	ENIX_DONNEES_INC *           pl_INC;
	ENIX_DONNEES_EXP *           pl_EXP;
	ENIX_DONNEES_SYS *           pl_SYS;
	ENIX_DONNEES_LTN *           pl_LTN;
	ENIX_DONNEES_Domaine *       vl_Domaine;

    XZST_03EcritureTrace( XZSTC_INFO, "IN : et_controle_cb");

    /*A Recuperation des parametres du message XDM_ECMD_EQPT */
    if (!TipcMsgRead( pa_Data -> msg,
                      T_IPC_FT_INT4,&vl_Etat,
                      T_IPC_FT_STR, &vl_NomMachine,
                      T_IPC_FT_INT4,&vl_TypeEqt,
                      T_IPC_FT_INT4,&vl_NumEqt,
                      NULL))
    {      
        /* Trace : Impossibilite de lire le message */
        XZST_03EcritureTrace(XZSTC_WARNING ,"Messsage XDM_ECMD_EQPT non lu." );
        return;
    }

    /* Trace du message en entree de la fonction */
    XZST_03EcritureTrace( XZSTC_DEBUG1 , " IN  : et_controle_cb, Etat: %d Machine: %s Type: %d Eqt: %d",
                          vl_Etat,
                          vl_NomMachine,
                          vl_TypeEqt,
                          vl_NumEqt );
    /* Selon le type d'equipement */
    switch ( vl_TypeEqt )
    {
        /* Si type d'equipement est panneau */
        case XDC_EQT_PAN :
            /* Recherche dans la liste des tubes si le panneau existe */
            for ( pl_Tube =  pg_debutTube;
                 (pl_Tube != NULL) && (vl_EqtTrouve != XDC_VRAI); 
                  pl_Tube =  pl_Tube->Suivant )
            {
                for ( pl_Equipement =  pl_Tube->ListeEquipement;
                     (pl_Equipement != NULL) && (vl_EqtTrouve != XDC_VRAI);
                      pl_Equipement =  pl_Equipement->Suivant )
                {
                    /* Si panneau trouve */
                    if ( pl_Equipement->NumeroEquipement == vl_NumEqt )
                    {
                        /* Mise a jour de l'etat de service ou d'activite du panneau */
                        if ( vl_Etat > XZEXC_LIMITE_ETAT_EQT )
                            pl_Equipement->Etat_Service &= vl_Etat;
                        else
                            pl_Equipement->Etat_Service += vl_Etat;
                        vl_EqtTrouve = XDC_VRAI;
                        break;
                    }
                }
            }
         
            /* Recherche dans la liste des echangeurs si le panneau existe */
            for ( pl_Ech =  pg_debutEchangeur;
                 (pl_Ech != NULL) && (vl_EqtTrouve != XDC_VRAI); 
                  pl_Ech =  pl_Ech->Suivant )
            {
            for ( pl_Equipement =  pl_Ech->ListeEquipement;
                 (pl_Equipement != NULL) && (vl_EqtTrouve != XDC_VRAI);
                  pl_Equipement =  pl_Equipement->Suivant )
            {
                /* Si panneau trouve */
                if ( pl_Equipement->NumeroEquipement == vl_NumEqt )
                {
                    /* Mise a jour de l'etat de service ou d'activite du panneau */
                    if ( vl_Etat > XZEXC_LIMITE_ETAT_EQT )
                        pl_Equipement->Etat_Service &= vl_Etat;
                    else
                        pl_Equipement->Etat_Service += vl_Etat;
                    vl_EqtTrouve = XDC_VRAI;
                    break;
                }
            }
        }
        break;
         
            
        /* Si type d'equipement est Capteur */ /* AAZ A VOIR */
        /*
        case XDC_EQT_CPT :
            // Recherche dans la liste des tubes si le Capteur existe
            for ( pl_Tube =  pg_debutTube;
                  pl_Tube != NULL; 
                  pl_Tube =  pl_Tube->Suivant )
            {
                for ( pl_Capt =  pl_Tube->ListeCapteur;
                     (pl_Capt != NULL) && (vl_EqtTrouve != XDC_VRAI);
                      pl_Capt =  pl_Capt->Suivant )
                {
                    // Si Capteur trouve
                    if ( pl_Capt->NumeroCapteur == vl_NumEqt )
                    {
                        // Mise a jour de l'etat de service ou d'activite du Capteur
                        if ( vl_Etat > XZEXC_LIMITE_ETAT_EQT )
                            pl_Capt->Etat_Service &= vl_Etat;
                        else
                            pl_Capt->Etat_Service += vl_Etat;
                        break;
                    }
                }
            }
            break;
            */

        /* Si type d'equipement est Tube */

        case XDC_EQT_TUB :
            /* Recherche dans la liste des tubes si le Tube existe */
            for ( pl_Tube =  pg_debutTube;
                  pl_Tube != NULL; 
                  pl_Tube =  pl_Tube->Suivant )
            {
                /* Si Tube trouve */
                if ( pl_Tube->Numero == vl_NumEqt )
                {
                    /* Mise a jour de l'etat de service ou d'activite du Tube */
                    if ( vl_Etat > XZEXC_LIMITE_ETAT_EQT )
                        pl_Tube->Etat_Service &= vl_Etat;
                    else
                        pl_Tube->Etat_Service += vl_Etat;
                    break;
                }
            }
            break;

        /* Si type d'equipement est Echangeur */
        case XDC_EQT_ECH :
            /* Recherche dans la liste des tubes si le Echangeur existe */
            for ( pl_Ech =  pg_debutEchangeur;
                  pl_Ech != NULL; 
                  pl_Ech =  pl_Ech->Suivant )
            {
                /* Si Echangeur trouve */
                if ( pl_Ech->NumeroDemiEch == vl_NumEqt )
                {
                    /* Mise a jour de l'etat de service ou d'activite du Echangeur */
                    if ( vl_Etat > XZEXC_LIMITE_ETAT_EQT )
                        pl_Ech->Etat_Service &= vl_Etat;
                    else
                        pl_Ech->Etat_Service += vl_Etat;
                    break;
                }
            }
            break;

        /* Domaines SIG, ECL, ... */                  

        /* Si type d'equipement domaine signalisation */
        case XDC_EQT_SIG :
            /* Recherche dans la liste des equipements du domaine SIGNALISATION si eqt existe */
            for ( pl_SIG =  pg_debutSIG;
                  pl_SIG != NULL; 
                  pl_SIG =  pl_SIG->Suivant )
            {
                /* Si local trouve */
                if ( pl_SIG->Numero == vl_NumEqt )
                {
                    /* Mise a jour de l'etat de service ou d'activite du local */
                    if ( vl_Etat > XZEXC_LIMITE_ETAT_EQT )
                        pl_SIG->Etat_Service &= vl_Etat;
                    else
                        pl_SIG->Etat_Service += vl_Etat;
                    break;
                }
            }
            break;

        /* Si type d'equipement domaine eclairage */
        case XDC_EQT_ECL :
            /* Recherche dans la liste des equipements du domaine ECLAIRAGE si eqt existe */
            for ( pl_ECL =  pg_debutECL;
                  pl_ECL != NULL; 
                  pl_ECL =  pl_ECL->Suivant )
            {
                /* Si local trouve */
                if ( pl_ECL->Numero == vl_NumEqt )
                {
                    /* Mise a jour de l'etat de service ou d'activite du local */
                    if ( vl_Etat > XZEXC_LIMITE_ETAT_EQT )
                        pl_ECL->Etat_Service &= vl_Etat;
                    else
                        pl_ECL->Etat_Service += vl_Etat;
                    break;
                }
            }
            break;

        /* Si type d'equipement domaine ventilation */
        case XDC_EQT_VEN :
            /* Recherche dans la liste des equipements du domaine VENTILATION si eqt existe */
            for ( pl_VEN =  pg_debutVEN;
                  pl_VEN != NULL; 
                  pl_VEN =  pl_VEN->Suivant )
            {
                /* Si local trouve */
                if ( pl_VEN->Numero == vl_NumEqt )
                {
                    /* Mise a jour de l'etat de service ou d'activite du local */
                    if ( vl_Etat > XZEXC_LIMITE_ETAT_EQT )
                        pl_VEN->Etat_Service &= vl_Etat;
                    else
                        pl_VEN->Etat_Service += vl_Etat;
                    break;
                }
            }
            break;

        /* Si type d'equipement domaine energie */
        case XDC_EQT_ENR :
            /* Recherche dans la liste des equipements du domaine ENERGIE si eqt existe */
            for ( pl_ENR =  pg_debutENR;
                  pl_ENR != NULL; 
                  pl_ENR =  pl_ENR->Suivant )
            {
                /* Si local trouve */
                if ( pl_ENR->Numero == vl_NumEqt )
                {
                    /* Mise a jour de l'etat de service ou d'activite du local */
                    if ( vl_Etat > XZEXC_LIMITE_ETAT_EQT )
                        pl_ENR->Etat_Service &= vl_Etat;
                    else
                        pl_ENR->Etat_Service += vl_Etat;
                    break;
                }
            }
            break;

        /* Si type d'equipement domaine protection incendie */
        case XDC_EQT_INC :
            /* Recherche dans la liste des equipements du domaine PROTECTION INCENDIE si eqt existe */
            for ( pl_INC =  pg_debutINC;
                  pl_INC != NULL; 
                  pl_INC =  pl_INC->Suivant )
            {
                /* Si local trouve */
                if ( pl_INC->Numero == vl_NumEqt )
                {
                    /* Mise a jour de l'etat de service ou d'activite du local */
                    if ( vl_Etat > XZEXC_LIMITE_ETAT_EQT )
                        pl_INC->Etat_Service &= vl_Etat;
                    else
                        pl_INC->Etat_Service += vl_Etat;
                    break;
                }
            }
            break;

        /* Si type d'equipement domaine exploitation */
        case XDC_EQT_EXP :
            /* Recherche dans la liste des equipements du domaine EXPLOITATION si eqt existe */
            for ( pl_EXP =  pg_debutEXP;
                  pl_EXP != NULL; 
                  pl_EXP =  pl_EXP->Suivant )
            {
                /* Si local trouve */
                if ( pl_EXP->Numero == vl_NumEqt )
                {
                    /* Mise a jour de l'etat de service ou d'activite du local */
                    if ( vl_Etat > XZEXC_LIMITE_ETAT_EQT )
                        pl_EXP->Etat_Service &= vl_Etat;
                    else
                        pl_EXP->Etat_Service += vl_Etat;
                    break;
                }
            }
            break;

        /* Si type d'equipement domaine systeme */
        case XDC_EQT_SYS :
            /* Recherche dans la liste des equipements du domaine SYSTEME si eqt existe */
            for ( pl_SYS =  pg_debutSYS;
                  pl_SYS != NULL; 
                  pl_SYS =  pl_SYS->Suivant )
            {
                /* Si local trouve */
                if ( pl_SYS->Numero == vl_NumEqt )
                {
                    /* Mise a jour de l'etat de service ou d'activite du local */
                    if ( vl_Etat > XZEXC_LIMITE_ETAT_EQT )
                        pl_SYS->Etat_Service &= vl_Etat;
                    else
                        pl_SYS->Etat_Service += vl_Etat;
                    break;
                }
            }
            break;

        /* Si type d'equipement domaine locaux tecnhiques */
        case XDC_EQT_LTN :
            /* Recherche dans la liste des equipements du domaine locaux technqiues si eqt existe */
            for ( pl_LTN =  pg_debutLTN;
                  pl_LTN != NULL; 
                  pl_LTN =  pl_LTN->Suivant )
            {
                /* Si local trouve */
                if ( pl_LTN->Numero == vl_NumEqt )
                {
                    /* Mise a jour de l'etat de service ou d'activite du local */
                    if ( vl_Etat > XZEXC_LIMITE_ETAT_EQT )
                        pl_LTN->Etat_Service &= vl_Etat;
                    else
                        pl_LTN->Etat_Service += vl_Etat;
                    break;
                }
            }
            break;
   
    }
    XZST_03EcritureTrace(XZSTC_DEBUG1, " OUT : et_controle_cb ");
}

/* Domaines SIG, ECL, VEN, ENR, INC */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande commande domaine signalisation
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void    et_cmd_sig_cb     ( T_IPC_CONN                   va_Cnx,
                            T_IPC_CONN_PROCESS_CB_DATA   pa_Data,
                            T_CB_ARG                     pa_ARG)
 
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
*  XDM_ECMD_SIG
*
* FONCTION 
*  Pour toutes la signalisations concernee
*  Envoyer a la tache TEXA0 la commande
*
------------------------------------------------------*/
{
    static char                  *version = "@(#)enix_cmd.c	1.6 06/16/09      : et_cmd_sig_cb " ;

    ENIX_DONNEES_SIG *           pl_SIG;
    ENIX_DONNEES_Equipement *    pl_Equipement;
#ifndef _TIBCO_EMS
    T_INT2                       *pl_NumEqt,
								 *pl_NumTypeEqt,
                                 *pl_NumCmd;
    T_INT4                       vl_NumSIG,
                                 vl_NumAct,
                                 vl_NumSeq,
		                 vl_NbEqt, 
				 vl_NbTypeEqt,
                                 vl_NbCmd;
#else
    XDY_Mot                       pl_NumEqt[XZETC_MAX_UNITAIRE],
				pl_NumTypeEqt[XZETC_MAX_UNITAIRE],
                                 pl_NumCmd[XZETC_MAX_UNITAIRE];
    XDY_Entier                       vl_NumSIG,
                                 vl_NumAct,
                                 vl_NumSeq,
		                 vl_NbEqt, 
				 vl_NbTypeEqt,
                                 vl_NbCmd;
#endif
    short                        vl_NumEqt[XZETC_MAX_UNITAIRE],
		                         vl_NumTypeEqt[XZETC_MAX_UNITAIRE],
								 vl_TypeEqt[XZETC_MAX_UNITAIRE],
                                 vl_Eqt[XZETC_MAX_UNITAIRE],
                                 vl_NumCmd[XZETC_MAX_UNITAIRE];
    short                        vl_Rang1[XZETC_MAX_UNITAIRE],
                                 vl_Rang2[XZETC_MAX_UNITAIRE],
                                 vl_Cmd[XZETC_MAX_UNITAIRE];
    int                          vl_Index, vl_IndexCmd;
    int                          vl_commande_TWWI0,vl_commande_TWWI1;
    
    int		                    vl_TypeOuvrage;
	int							vl_eqp_trouve;
	
    XZST_03EcritureTrace(XZSTC_FONCTION, " IN  : et_cmd_sig_cb ");

    /*A Recuperation des parametres du message XDM_ECMD_SIG */
    if(!TipcMsgRead( pa_Data -> msg,
                     T_IPC_FT_INT4,       &vl_NumSIG,
                     T_IPC_FT_INT2_ARRAY, &pl_NumTypeEqt,&vl_NbTypeEqt,
                     T_IPC_FT_INT2_ARRAY, &pl_NumEqt,&vl_NbEqt,
                     T_IPC_FT_INT2_ARRAY, &pl_NumCmd,&vl_NbCmd,
                     T_IPC_FT_INT4,       &vl_NumSeq,
                     T_IPC_FT_INT4,       &vl_NumAct,
                     NULL))
    {       
        /* Trace : Impossibilite de lire le message */
        XZST_03EcritureTrace ( XZSTC_WARNING , "Messsage XDM_ECMD_SIG non lu ." );
        return;
    }

#ifndef _TIBCO_EMS
    for ( vl_Index = 0;
          vl_Index < XZETC_MAX_UNITAIRE;
          vl_NumEqt[vl_Index] = *pl_NumEqt, 
          vl_NumTypeEqt[vl_Index] = *pl_NumTypeEqt, 
		  vl_NumCmd[vl_Index] = *pl_NumCmd , 
		  vl_Index++, 
		  pl_NumEqt++, 
		  pl_NumCmd++,
		  pl_NumTypeEqt++);
#else
    for ( vl_Index = 0;
          vl_Index < XZETC_MAX_UNITAIRE; vl_Index++)
    {       
          vl_NumEqt[vl_Index] = pl_NumEqt[vl_Index];
          vl_NumTypeEqt[vl_Index] = pl_NumTypeEqt[vl_Index];
		  vl_NumCmd[vl_Index] = pl_NumCmd[vl_Index];
    }
#endif

    vl_TypeOuvrage = 0;
    //--XDC_EQT_TUBE;
    //--XDC_EQT_ECHANGEUR
    vl_eqp_trouve = 0;
    /* Recherche dans la liste le domaine concerne */
    for ( pl_SIG = pg_debutSIG; pl_SIG != NULL; pl_SIG = pl_SIG->Suivant )
    {
        XZST_03EcritureTrace ( XZSTC_INFO , "Signalisation num:<%d>,  etat service : <%d>.",
                               pl_SIG->Numero,
                               pl_SIG->Etat_Service);

        /* Si trouve et pas signalisation */
        //--if  (pl_SIG->Numero == vl_NumSIG && pl_SIG->TypeOuvrage == vl_TypeOuvrage )
        if  (pl_SIG->Numero == vl_NumSIG )
        {
            /* Recherche le rang de commande des equipements concernes */
            for ( pl_Equipement = pl_SIG->ListeEquipement, vl_IndexCmd = 0;
                  pl_Equipement != NULL;
                  pl_Equipement = pl_Equipement->Suivant)
            {
                XZST_03EcritureTrace ( XZSTC_INFO , "AZA pl_Equipement->NumeroEquipement :<%d>,  Numero a trouver vl_NumSIG : <%d>.",
                                       pl_Equipement->NumeroEquipement,
                                       vl_NumSIG);

                vl_Cmd[vl_IndexCmd] = 0;
                vl_TypeEqt[vl_IndexCmd] = 0;
                vl_Eqt[vl_IndexCmd]=0;
                for ( vl_Index = 0; 
                     (vl_Index < XZETC_MAX_UNITAIRE) && (vl_NumEqt[vl_Index] != 0); 
                      vl_Index++ )
                {
                    /* Si equipement trouve */
					XZST_03EcritureTrace ( XZSTC_INFO , "Recherche de l'equipement : courant = (%d, %d), reference = (%d, %d)",
									   pl_Equipement->TypeEquipement,
									   pl_Equipement->NumeroEquipement,
                                       vl_NumTypeEqt[vl_Index],
                                       vl_NumEqt[vl_Index]);
                    if (pl_Equipement->NumeroEquipement == vl_NumEqt[vl_Index] && pl_Equipement->TypeEquipement == vl_NumTypeEqt[vl_Index])
                    {

                        /* Stock la commande et l equipement */
                        vl_Eqt[vl_IndexCmd]   = vl_NumEqt[vl_Index];
						vl_TypeEqt[vl_IndexCmd] = vl_NumTypeEqt[vl_Index];
                        vl_Cmd[vl_IndexCmd++] = vl_NumCmd[vl_Index];
                        vl_eqp_trouve = 1;
						/* correction ABE le 24-03-2010 : correction probl�me d'envoi de TC eqp sur les �changeurs */
                		/* recuperation du type ouvrage de l'equipement */
                    	vl_TypeOuvrage = pl_Equipement->TypeOuvrage;
                        break;
                    }
                }
            }
			/* correction ABE le 24-03-2010 : correction probl�me d'envoi de TC eqp sur les �changeurs */
            if (vl_eqp_trouve == 1)
            	break; /* Fin recherche SIG */
        }
    }

    if ( pl_SIG == NULL )
    {       
        /* Sortie : domaine signalisation non trouve ou en local */
        XZST_03EcritureTrace( XZSTC_WARNING , "Signalisation:%d non trouve ou en local ou inactif pour Action:%d.",
                              vl_NumSIG,
                              vl_NumAct );
    }
    else
    {
        /* Envoie des commandes a maitre Wizcon */
        vl_commande_TWWI0= et_envoi_cmd_unitaire( vg_SockTWWI0, 
                                                  vl_TypeOuvrage,
												  vl_TypeEqt,
                                                  vl_Eqt,
                                                  vl_Cmd,
                                                  vl_IndexCmd,
                                                  pl_SIG->Numero,
                                                  XDC_EQT_SIG, 
                                                  pl_SIG->DerniereSequence);

        /* Envoie des commandes a esclave Wizcon */     
        vl_commande_TWWI1= et_envoi_cmd_unitaire( vg_SockTWWI1, 
                                                  vl_TypeOuvrage,
												  vl_TypeEqt,
                                                  vl_Eqt,
                                                  vl_Cmd,
                                                  vl_IndexCmd,
                                                  pl_SIG->Numero,
                                                  XDC_EQT_SIG, 
                                                  pl_SIG->DerniereSequence);
        
        /* Si la maitre est la tache maitre Wizcon c'est son r�sultat qui fait foi*/
        if (vg_maitre_actif == XDC_MAITRE_TWWI0)
        { 
            if (vl_commande_TWWI0 == XDC_OK)
            {
                /*A Memorisation de la sequence en cours */
                pl_SIG->DerniereSequence = vl_NumSeq;
            }
        }
        else
        {
            if (vl_commande_TWWI1 == XDC_OK)
            {
                /*A Memorisation de la sequence en cours */
                pl_SIG->DerniereSequence = vl_NumSeq;
            }
        }

    }

    XZST_03EcritureTrace ( XZSTC_FONCTION, " OUT: et_cmd_sig_cb " );
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande commande domaine eclairage
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void    et_cmd_ecl_cb     ( T_IPC_CONN                   va_Cnx,
                            T_IPC_CONN_PROCESS_CB_DATA   pa_Data,
                            T_CB_ARG                     pa_ARG)
 
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
*  XDM_ECMD_ECL
*
* FONCTION 
*  Pour toutes la eclairages concernee
*  Envoyer a la tache TEXA0 la commande
*
------------------------------------------------------*/
{
    static char                  *version = "@(#)enix_cmd.c	1.6 06/16/09      : et_cmd_ecl_cb " ;

    ENIX_DONNEES_ECL *           pl_ECL;
    ENIX_DONNEES_Equipement *    pl_Equipement;
#ifndef _TIBCO_EMS
    T_INT2                       *pl_NumEqt,
	                             *pl_NumTypeEqt,
                                 *pl_NumCmd;
    T_INT4                       vl_NumECL,
                                 vl_NumAct,
                                 vl_NumSeq,
                                 vl_NbEqt,
                                 vl_NbTypeEqt,	
                                 vl_NbCmd;
#else
    XDY_Mot                       pl_NumEqt[XZETC_MAX_UNITAIRE],
	                             pl_NumTypeEqt[XZETC_MAX_UNITAIRE],
                                 pl_NumCmd[XZETC_MAX_UNITAIRE];
    XDY_Entier                       vl_NumECL,
                                 vl_NumAct,
                                 vl_NumSeq,
                                 vl_NbEqt,
                                 vl_NbTypeEqt,	
                                 vl_NbCmd;
#endif
    short                        vl_NumEqt[XZETC_MAX_UNITAIRE],
	                             vl_NumTypeEqt[XZETC_MAX_UNITAIRE],
                                 vl_TypeEqt[XZETC_MAX_UNITAIRE],
                                 vl_Eqt[XZETC_MAX_UNITAIRE],
                                 vl_NumCmd[XZETC_MAX_UNITAIRE];
    short                        vl_Rang1[XZETC_MAX_UNITAIRE],
                                 vl_Rang2[XZETC_MAX_UNITAIRE],
                                 vl_Cmd[XZETC_MAX_UNITAIRE];
	int                          vl_Index, vl_IndexCmd;
    int                          vl_commande_TWWI0,vl_commande_TWWI1;
    
    int		                     vl_TypeOuvrage;

    XZST_03EcritureTrace(XZSTC_FONCTION, " IN  : et_cmd_ecl_cb ");

    /*A Recuperation des parametres du message XDM_ECMD_ECL */
    if(!TipcMsgRead( pa_Data -> msg,
                     T_IPC_FT_INT4,       &vl_NumECL,
                     T_IPC_FT_INT2_ARRAY, &pl_NumTypeEqt,&vl_NbTypeEqt,
                     T_IPC_FT_INT2_ARRAY, &pl_NumEqt,&vl_NbEqt,
                     T_IPC_FT_INT2_ARRAY, &pl_NumCmd,&vl_NbCmd,
                     T_IPC_FT_INT4,       &vl_NumSeq,
                     T_IPC_FT_INT4,       &vl_NumAct,
                     NULL))
    {       
        /* Trace : Impossibilite de lire le message */
        XZST_03EcritureTrace ( XZSTC_WARNING , "Messsage XDM_ECMD_ECL non lu ." );
        return;
    }

#ifndef _TIBCO_EMS
    for ( vl_Index = 0;
          vl_Index < XZETC_MAX_UNITAIRE;
          vl_NumEqt[vl_Index] = *pl_NumEqt, 
          vl_NumTypeEqt[vl_Index] = *pl_NumTypeEqt, 
		  vl_NumCmd[vl_Index] = *pl_NumCmd , 
		  vl_Index++, 
		  pl_NumEqt++, 
		  pl_NumCmd++,
		  pl_NumTypeEqt++);
#else
    for ( vl_Index = 0;
          vl_Index < XZETC_MAX_UNITAIRE;vl_Index++)
	  {
          vl_NumEqt[vl_Index] = pl_NumEqt[vl_Index];
          vl_NumTypeEqt[vl_Index] = pl_NumTypeEqt[vl_Index]; 
		  vl_NumCmd[vl_Index] = pl_NumCmd[vl_Index] ;
    }
#endif


    vl_TypeOuvrage = 0;
    /* Recherche dans la liste le domaine concerne */
    for ( pl_ECL = pg_debutECL; pl_ECL != NULL; pl_ECL = pl_ECL->Suivant )
    {
        XZST_03EcritureTrace ( XZSTC_INFO , "Eclairage num:<%d>,  etat service : <%d>.",
                               pl_ECL->Numero,
                               pl_ECL->Etat_Service);

        /* Si trouve et pas eclairage */
        if  (pl_ECL->Numero == vl_NumECL)
        {
            /* Recherche le rang de commande des equipements concernes */
            for ( pl_Equipement = pl_ECL->ListeEquipement, vl_IndexCmd = 0;
                  pl_Equipement != NULL;
                  pl_Equipement = pl_Equipement->Suivant)
            {
                XZST_03EcritureTrace ( XZSTC_INFO , "AZA pl_Equipement->NumeroEquipement :<%d>,  Numero a trouver vl_NumECL : <%d>.",
                                       pl_Equipement->NumeroEquipement,
                                       vl_NumECL);
                /* recuperation du 1er type ouvrage des equipements */
                if (vl_TypeOuvrage == 0)
                {
                    vl_TypeOuvrage = pl_Equipement->TypeOuvrage;
                }

                vl_Cmd[vl_IndexCmd] = 0;
                vl_Eqt[vl_IndexCmd]=0;
                vl_TypeEqt[vl_IndexCmd]=0;
                for ( vl_Index = 0; 
                     (vl_Index < XZETC_MAX_UNITAIRE) && (vl_NumEqt[vl_Index] != 0); 
                      vl_Index++ )
                {
                    /* Si equipement trouve */
                    if (pl_Equipement->NumeroEquipement == vl_NumEqt[vl_Index] && pl_Equipement->TypeEquipement == vl_NumTypeEqt[vl_Index])
                    {

                        /* Stock la commande et l equipement */
                        vl_Eqt[vl_IndexCmd]   = vl_NumEqt[vl_Index];
                        vl_TypeEqt[vl_IndexCmd] = vl_NumTypeEqt[vl_Index];
                        vl_Cmd[vl_IndexCmd++] = vl_NumCmd[vl_Index];
                        break;
                    }
                }
            }
            break; /* Fin recherche ECL */
        }
    }

    if ( pl_ECL == NULL )
    {       
        /* Sortie : domaine eclairage non trouve ou en local */
        XZST_03EcritureTrace( XZSTC_WARNING , "Eclairage:%d non trouve ou en local ou inactif pour Action:%d.",
                              vl_NumECL,
                              vl_NumAct );
    }
    else
    {
        /* Envoie des commandes a maitre Wizcon */
        vl_commande_TWWI0= et_envoi_cmd_unitaire( vg_SockTWWI0, 
                                                  vl_TypeOuvrage,
                                                  vl_TypeEqt,
                                                  vl_Eqt,
                                                  vl_Cmd,
                                                  vl_IndexCmd,
                                                  pl_ECL->Numero,
                                                  XDC_EQT_ECL, 
                                                  pl_ECL->DerniereSequence);

        /* Envoie des commandes a esclave Wizcon */     
        vl_commande_TWWI1= et_envoi_cmd_unitaire( vg_SockTWWI1, 
                                                  vl_TypeOuvrage,
                                                  vl_TypeEqt,
                                                  vl_Eqt,
                                                  vl_Cmd,
                                                  vl_IndexCmd,
                                                  pl_ECL->Numero,
                                                  XDC_EQT_ECL, 
                                                  pl_ECL->DerniereSequence);
        
        /* Si la maitre est la tache maitre Wizcon c'est son r�sultat qui fait foi*/
        if (vg_maitre_actif == XDC_MAITRE_TWWI0)
        { 
            if (vl_commande_TWWI0 == XDC_OK)
            {
                /*A Memorisation de la sequence en cours */
                pl_ECL->DerniereSequence = vl_NumSeq;
            }
        }
        else
        {
            if (vl_commande_TWWI1 == XDC_OK)
            {
                /*A Memorisation de la sequence en cours */
                pl_ECL->DerniereSequence = vl_NumSeq;
            }
        }

    }
    XZST_03EcritureTrace ( XZSTC_FONCTION, " OUT: et_cmd_ecl_cb " );
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande commande domaine ventilation
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void    et_cmd_ven_cb     ( T_IPC_CONN                   va_Cnx,
                            T_IPC_CONN_PROCESS_CB_DATA   pa_Data,
                            T_CB_ARG                     pa_ARG)
 
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
*  XDM_ECMD_VEN
*
* FONCTION 
*  Pour toutes la ventilations concernee
*  Envoyer a la tache TEXA0 la commande
*
------------------------------------------------------*/
{
    static char                  *version = "@(#)enix_cmd.c	1.6 06/16/09      : et_cmd_ven_cb " ;

    ENIX_DONNEES_VEN *           pl_VEN;
    ENIX_DONNEES_Equipement *    pl_Equipement;
#ifdef _TIBCO_EMS
    XDY_Entier                       vl_NumVEN,
                                 vl_NumAct,
                                 vl_NumSeq,
                                 vl_NbEqt, 
				 vl_NbTypeEqt,
                                 vl_NbCmd;
    XDY_Mot                      pl_NumEqt[255],
	                         pl_NumTypeEqt[255],
                                 pl_NumCmd[255];
#else
    T_INT4                       vl_NumVEN,
                                 vl_NumAct,
                                 vl_NumSeq,
                                 vl_NbEqt, 
				 vl_NbTypeEqt,
                                 vl_NbCmd;
    T_INT2                       *pl_NumEqt,
	                         *pl_NumTypeEqt,
                                 *pl_NumCmd;
#endif
    short                        vl_NumEqt[XZETC_MAX_UNITAIRE],
	                             vl_NumTypeEqt[XZETC_MAX_UNITAIRE],
								 vl_TypeEqt[XZETC_MAX_UNITAIRE],
                                 vl_Eqt[XZETC_MAX_UNITAIRE],
                                 vl_NumCmd[XZETC_MAX_UNITAIRE];
    short                        vl_Rang1[XZETC_MAX_UNITAIRE],
                                 vl_Rang2[XZETC_MAX_UNITAIRE],
                                 vl_Cmd[XZETC_MAX_UNITAIRE];
    int                          vl_Index, vl_IndexCmd;
    int                          vl_commande_TWWI0,vl_commande_TWWI1;
    
    int		                     vl_TypeOuvrage;

    XZST_03EcritureTrace(XZSTC_FONCTION, " IN  : et_cmd_ven_cb ");

    /*A Recuperation des parametres du message XDM_ECMD_VEN */
    if(!TipcMsgRead( pa_Data -> msg,
                     T_IPC_FT_INT4,       &vl_NumVEN,
                     T_IPC_FT_INT2_ARRAY, &pl_NumTypeEqt,&vl_NbTypeEqt,
                     T_IPC_FT_INT2_ARRAY, &pl_NumEqt,&vl_NbEqt,
                     T_IPC_FT_INT2_ARRAY, &pl_NumCmd,&vl_NbCmd,
                     T_IPC_FT_INT4,       &vl_NumSeq,
                     T_IPC_FT_INT4,       &vl_NumAct,
                     NULL))
    {       
        /* Trace : Impossibilite de lire le message */
        XZST_03EcritureTrace ( XZSTC_WARNING , "Messsage XDM_ECMD_VEN non lu ." );
        return;
    }

#ifdef _TIBCO_EMS
    for ( vl_Index = 0;
          vl_Index < XZETC_MAX_UNITAIRE;
          vl_NumEqt[vl_Index] = pl_NumEqt[vl_Index], 
          vl_NumTypeEqt[vl_Index] = pl_NumTypeEqt[vl_Index], 
		  vl_NumCmd[vl_Index] = pl_NumCmd[vl_Index] , 
		  vl_Index++);
#else
    for ( vl_Index = 0;
          vl_Index < XZETC_MAX_UNITAIRE;
          vl_NumEqt[vl_Index] = *pl_NumEqt, 
          vl_NumTypeEqt[vl_Index] = *pl_NumTypeEqt, 
		  vl_NumCmd[vl_Index] = *pl_NumCmd , 
		  vl_Index++, 
		  pl_NumEqt++, 
		  pl_NumCmd++,
		  pl_NumTypeEqt++);
#endif

    vl_TypeOuvrage = 0;
    /* Recherche dans la liste le domaine concerne */
    for ( pl_VEN = pg_debutVEN; pl_VEN != NULL; pl_VEN = pl_VEN->Suivant )
    {
        XZST_03EcritureTrace ( XZSTC_INFO , "Ventilation num:<%d>,  etat service : <%d>.",
                               pl_VEN->Numero,
                               pl_VEN->Etat_Service);

        /* Si trouve et pas ventilation */
        if  (pl_VEN->Numero == vl_NumVEN)
        {
            /* Recherche le rang de commande des equipements concernes */
            for ( pl_Equipement = pl_VEN->ListeEquipement, vl_IndexCmd = 0;
                  pl_Equipement != NULL;
                  pl_Equipement = pl_Equipement->Suivant)
            {
                XZST_03EcritureTrace ( XZSTC_INFO , "AZA pl_Equipement->NumeroEquipement :<%d>,  Numero a trouver vl_NumVEN : <%d>.",
                                       pl_Equipement->NumeroEquipement,
                                       vl_NumVEN);
                /* recuperation du 1er type ouvrage des equipements */
                if (vl_TypeOuvrage == 0)
                {
                    vl_TypeOuvrage = pl_Equipement->TypeOuvrage;
                }

                vl_Cmd[vl_IndexCmd] = 0;
                vl_Eqt[vl_IndexCmd]=0;
                vl_TypeEqt[vl_IndexCmd]=0;
                for ( vl_Index = 0; 
                     (vl_Index < XZETC_MAX_UNITAIRE) && (vl_NumEqt[vl_Index] != 0); 
                      vl_Index++ )
                {

                XZST_03EcritureTrace ( XZSTC_INFO , "AZA Equipement vl_NumEqt[%d] : %d.", vl_Index, vl_NumEqt[vl_Index]);

                    /* Si equipement trouve */
                    if (pl_Equipement->NumeroEquipement == vl_NumEqt[vl_Index] && pl_Equipement->TypeEquipement == vl_NumTypeEqt[vl_Index])
                    {

                        /* Stock la commande et l equipement */
                        vl_Eqt[vl_IndexCmd]   = vl_NumEqt[vl_Index];
						vl_TypeEqt[vl_IndexCmd] = vl_NumTypeEqt[vl_Index];
                        vl_Cmd[vl_IndexCmd++] = vl_NumCmd[vl_Index];
                        break;
                    }
                }
            }
            break; /* Fin recherche VEN */
        }
    }

    if ( pl_VEN == NULL )
    {       
        /* Sortie : domaine ventilation non trouve ou en local */
        XZST_03EcritureTrace( XZSTC_WARNING , "Ventilation:%d non trouve ou en local ou inactif pour Action:%d.",
                              vl_NumVEN,
                              vl_NumAct );
    }
    else
    {

        /* Envoie des commandes a maitre Wizcon */
        vl_commande_TWWI0= et_envoi_cmd_unitaire( vg_SockTWWI0, 
                                                  vl_TypeOuvrage,
                                                  vl_TypeEqt,
                                                  vl_Eqt,
                                                  vl_Cmd,
                                                  vl_IndexCmd,
                                                  pl_VEN->Numero,
                                                  XDC_EQT_VEN, 
                                                  pl_VEN->DerniereSequence);

        /* Envoie des commandes a esclave Wizcon */     
        vl_commande_TWWI1= et_envoi_cmd_unitaire( vg_SockTWWI1, 
                                                  vl_TypeOuvrage,
                                                  vl_TypeEqt,
                                                  vl_Eqt,
                                                  vl_Cmd,
                                                  vl_IndexCmd,
                                                  pl_VEN->Numero,
                                                  XDC_EQT_VEN, 
                                                  pl_VEN->DerniereSequence);
        
        /* Si la maitre est la tache maitre Wizcon c'est son r�sultat qui fait foi*/
        if (vg_maitre_actif == XDC_MAITRE_TWWI0)
        { 
            if (vl_commande_TWWI0 == XDC_OK)
            {
                /*A Memorisation de la sequence en cours */
                pl_VEN->DerniereSequence = vl_NumSeq;
            }
        }
        else
        {
            if (vl_commande_TWWI1 == XDC_OK)
            {
                /*A Memorisation de la sequence en cours */
                pl_VEN->DerniereSequence = vl_NumSeq;
            }
        }

    }

    XZST_03EcritureTrace ( XZSTC_FONCTION, " OUT: et_cmd_ven_cb " );
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande commande domaine energie
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void    et_cmd_enr_cb     ( T_IPC_CONN                   va_Cnx,
                            T_IPC_CONN_PROCESS_CB_DATA   pa_Data,
                            T_CB_ARG                     pa_ARG)
 
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
*  XDM_ECMD_ENR
*
* FONCTION 
*  Pour toutes la energies concernee
*  Envoyer a la tache TEXA0 la commande
*
------------------------------------------------------*/
{
    static char                  *version = "@(#)enix_cmd.c	1.6 06/16/09      : et_cmd_enr_cb " ;

    ENIX_DONNEES_ENR *           pl_ENR;
    ENIX_DONNEES_Equipement *    pl_Equipement;
#ifdef _TIBCO_EMS
    XDY_Entier                       vl_NumENR,
                                 vl_NumAct,
                                 vl_NumSeq,
                                 vl_NbEqt,
                                 vl_NbTypeEqt,	
                                 vl_NbCmd;
    XDY_Mot                      pl_NumEqt[255],
				 pl_NumTypeEqt[255],
                                 pl_NumCmd[255];
#else
    T_INT4                       vl_NumENR,
                                 vl_NumAct,
                                 vl_NumSeq,
                                 vl_NbEqt,
                                 vl_NbTypeEqt,	
                                 vl_NbCmd;
    T_INT2                       *pl_NumEqt,
				 *pl_NumTypeEqt,
                                 *pl_NumCmd;
#endif
    short                        vl_NumEqt[XZETC_MAX_UNITAIRE],
								 vl_NumTypeEqt[XZETC_MAX_UNITAIRE],
                                 vl_TypeEqt[XZETC_MAX_UNITAIRE],
                                 vl_Eqt[XZETC_MAX_UNITAIRE],
                                 vl_NumCmd[XZETC_MAX_UNITAIRE];
    short                        vl_Rang1[XZETC_MAX_UNITAIRE],
                                 vl_Rang2[XZETC_MAX_UNITAIRE],
                                 vl_Cmd[XZETC_MAX_UNITAIRE];
    int                          vl_Index, vl_IndexCmd;
    int                          vl_commande_TWWI0,vl_commande_TWWI1;
    
    int		                     vl_TypeOuvrage;

    XZST_03EcritureTrace(XZSTC_FONCTION, " IN  : et_cmd_enr_cb ");

    /*A Recuperation des parametres du message XDM_ECMD_ENR */
    if(!TipcMsgRead( pa_Data -> msg,
                     T_IPC_FT_INT4,       &vl_NumENR,
                     T_IPC_FT_INT2_ARRAY, &pl_NumTypeEqt,&vl_NbTypeEqt,
                     T_IPC_FT_INT2_ARRAY, &pl_NumEqt,&vl_NbEqt,
                     T_IPC_FT_INT2_ARRAY, &pl_NumCmd,&vl_NbCmd,
                     T_IPC_FT_INT4,       &vl_NumSeq,
                     T_IPC_FT_INT4,       &vl_NumAct,
                     NULL))
    {       
        /* Trace : Impossibilite de lire le message */
        XZST_03EcritureTrace ( XZSTC_WARNING , "Messsage XDM_ECMD_ENR non lu ." );
        return;
    }

#ifdef _TIBCO_EMS
    for ( vl_Index = 0;
          vl_Index < XZETC_MAX_UNITAIRE;
          vl_NumEqt[vl_Index] = pl_NumEqt[vl_Index], 
          vl_NumTypeEqt[vl_Index] = pl_NumTypeEqt[vl_Index], 
		  vl_NumCmd[vl_Index] = pl_NumCmd[vl_Index] , 
		  vl_Index++);
#else
    for ( vl_Index = 0;
          vl_Index < XZETC_MAX_UNITAIRE;
          vl_NumEqt[vl_Index] = *pl_NumEqt, 
          vl_NumTypeEqt[vl_Index] = *pl_NumTypeEqt, 
		  vl_NumCmd[vl_Index] = *pl_NumCmd , 
		  vl_Index++, 
		  pl_NumEqt++, 
		  pl_NumCmd++,
		  pl_NumTypeEqt++);
#endif

    vl_TypeOuvrage = 0;
    /* Recherche dans la liste le domaine concerne */
    for ( pl_ENR = pg_debutENR; pl_ENR != NULL; pl_ENR = pl_ENR->Suivant )
    {
        XZST_03EcritureTrace ( XZSTC_INFO , "Energie num:<%d>,  etat service : <%d>.",
                               pl_ENR->Numero,
                               pl_ENR->Etat_Service);

        /* Si trouve et pas energie */
        if  (pl_ENR->Numero == vl_NumENR)
        {
            /* Recherche le rang de commande des equipements concernes */
            for ( pl_Equipement = pl_ENR->ListeEquipement, vl_IndexCmd = 0;
                  pl_Equipement != NULL;
                  pl_Equipement = pl_Equipement->Suivant)
            {
                XZST_03EcritureTrace ( XZSTC_INFO , "AZA pl_Equipement->NumeroEquipement :<%d>,  Numero a trouver vl_NumENR : <%d>.",
                                       pl_Equipement->NumeroEquipement,
                                       vl_NumENR);
                /* recuperation du 1er type ouvrage des equipements */
                if (vl_TypeOuvrage == 0)
                {
                    vl_TypeOuvrage = pl_Equipement->TypeOuvrage;
                }

                vl_Cmd[vl_IndexCmd] = 0;
                vl_Eqt[vl_IndexCmd]=0;
                vl_TypeEqt[vl_IndexCmd]=0;
                for ( vl_Index = 0; 
                     (vl_Index < XZETC_MAX_UNITAIRE) && (vl_NumEqt[vl_Index] != 0); 
                      vl_Index++ )
                {

                XZST_03EcritureTrace ( XZSTC_INFO , "AZA Equipement vl_NumEqt[%d] : %d.", vl_Index, vl_NumEqt[vl_Index]);

                    /* Si equipement trouve */
                    if (pl_Equipement->NumeroEquipement == vl_NumEqt[vl_Index] && pl_Equipement->TypeEquipement == vl_NumTypeEqt[vl_Index])
                    {

                        /* Stock la commande et l equipement */
                        vl_Eqt[vl_IndexCmd]   = vl_NumEqt[vl_Index];
                        vl_TypeEqt[vl_IndexCmd]   = vl_NumTypeEqt[vl_Index];
                        vl_Cmd[vl_IndexCmd++] = vl_NumCmd[vl_Index];
                        break;
                    }
                }
            }
            break; /* Fin recherche ENR */
        }
    }

    if ( pl_ENR == NULL )
    {       
        /* Sortie : domaine energie non trouve ou en local */
        XZST_03EcritureTrace( XZSTC_WARNING , "Energie:%d non trouve ou en local ou inactif pour Action:%d.",
                              vl_NumENR,
                              vl_NumAct );
    }
    else
    {
        /* Envoie des commandes a maitre Wizcon */
        vl_commande_TWWI0= et_envoi_cmd_unitaire( vg_SockTWWI0, 
                                                  vl_TypeOuvrage,
                                                  vl_TypeEqt,
                                                  vl_Eqt,
                                                  vl_Cmd,
                                                  vl_IndexCmd,
                                                  pl_ENR->Numero,
                                                  XDC_EQT_ENR, 
                                                  pl_ENR->DerniereSequence);

        /* Envoie des commandes a esclave Wizcon */     
        vl_commande_TWWI1= et_envoi_cmd_unitaire( vg_SockTWWI1, 
                                                  vl_TypeOuvrage,
                                                  vl_TypeEqt,
                                                  vl_Eqt,
                                                  vl_Cmd,
                                                  vl_IndexCmd,
                                                  pl_ENR->Numero,
                                                  XDC_EQT_ENR, 
                                                  pl_ENR->DerniereSequence);
        
        /* Si la maitre est la tache maitre Wizcon c'est son r�sultat qui fait foi*/
        if (vg_maitre_actif == XDC_MAITRE_TWWI0)
        { 
            if (vl_commande_TWWI0 == XDC_OK)
            {
                /*A Memorisation de la sequence en cours */
                pl_ENR->DerniereSequence = vl_NumSeq;
            }
        }
        else
        {
            if (vl_commande_TWWI1 == XDC_OK)
            {
                /*A Memorisation de la sequence en cours */
                pl_ENR->DerniereSequence = vl_NumSeq;
            }
        }

    }

    XZST_03EcritureTrace ( XZSTC_FONCTION, " OUT: et_cmd_enr_cb " );
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande commande domaine exploitation
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void    et_cmd_exp_cb     ( T_IPC_CONN                   va_Cnx,
                            T_IPC_CONN_PROCESS_CB_DATA   pa_Data,
                            T_CB_ARG                     pa_ARG)
 
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
*  XDM_ECMD_ENR
*
* FONCTION 
*  Pour toutes la energies concernee
*  Envoyer a la tache TEXA0 la commande
*
------------------------------------------------------*/
{
    static char                  *version = "@(#)enix_cmd.c	1.6 06/16/09      : et_cmd_enr_cb " ;

    ENIX_DONNEES_EXP *           pl_EXP;
    ENIX_DONNEES_Equipement *    pl_Equipement;
#ifndef _TIBCO_EMS
    T_INT4                       vl_NumEXP,
                                 vl_NumAct,
                                 vl_NumSeq,
                                 vl_NbEqt, 
	                         vl_NbTypeEqt,
                                 vl_NbCmd;
    T_INT2                       *pl_NumEqt,
	                         *pl_NumTypeEqt,
                                 *pl_NumCmd;
#else
    XDY_Entier                   vl_NumEXP,
                                 vl_NumAct,
                                 vl_NumSeq,
                                 vl_NbEqt, 
	                         vl_NbTypeEqt,
                                 vl_NbCmd;
    XDY_Mot                      pl_NumEqt[255],
	                         pl_NumTypeEqt[255],
                                 pl_NumCmd[255];
#endif
    short                        vl_NumEqt[XZETC_MAX_UNITAIRE],
								 vl_NumTypeEqt[XZETC_MAX_UNITAIRE],
                                 vl_Eqt[XZETC_MAX_UNITAIRE],
                                 vl_TypeEqt[XZETC_MAX_UNITAIRE],
                                 vl_NumCmd[XZETC_MAX_UNITAIRE];
    short                        vl_Rang1[XZETC_MAX_UNITAIRE],
                                 vl_Rang2[XZETC_MAX_UNITAIRE],
                                 vl_Cmd[XZETC_MAX_UNITAIRE];
    int                          vl_Index, vl_IndexCmd;
    int                          vl_commande_TWWI0,vl_commande_TWWI1;
    
    int		                     vl_TypeOuvrage;

    XZST_03EcritureTrace(XZSTC_FONCTION, " IN  : et_cmd_exp_cb ");

    /*A Recuperation des parametres du message XDM_ECMD_EXP */
    if(!TipcMsgRead( pa_Data -> msg,
                     T_IPC_FT_INT4,       &vl_NumEXP,
                     T_IPC_FT_INT2_ARRAY, &pl_NumTypeEqt,&vl_NbTypeEqt,
                     T_IPC_FT_INT2_ARRAY, &pl_NumEqt,&vl_NbEqt,
                     T_IPC_FT_INT2_ARRAY, &pl_NumCmd,&vl_NbCmd,
                     T_IPC_FT_INT4,       &vl_NumSeq,
                     T_IPC_FT_INT4,       &vl_NumAct,
                     NULL))
    {       
        /* Trace : Impossibilite de lire le message */
        XZST_03EcritureTrace ( XZSTC_WARNING , "Messsage XDM_ECMD_EXP non lu ." );
        return;
    }

#ifdef _TIBCO_EMS
    for ( vl_Index = 0;
          vl_Index < XZETC_MAX_UNITAIRE;
          vl_NumEqt[vl_Index] = pl_NumEqt[vl_Index], 
		  vl_NumCmd[vl_Index] = pl_NumCmd[vl_Index] , 
		  vl_NumTypeEqt[vl_Index] = pl_NumTypeEqt[vl_Index] , 
		  vl_Index++);
#else
    for ( vl_Index = 0;
          vl_Index < XZETC_MAX_UNITAIRE;
          vl_NumEqt[vl_Index] = *pl_NumEqt, 
		  vl_NumCmd[vl_Index] = *pl_NumCmd , 
		  vl_NumTypeEqt[vl_Index] = *pl_NumTypeEqt , 
		  vl_Index++, 
		  pl_NumEqt++, 
		  pl_NumCmd++,
		  pl_NumTypeEqt++);
#endif

    vl_TypeOuvrage = 0;
    /* Recherche dans la liste le domaine concerne */
    for ( pl_EXP = pg_debutEXP; pl_EXP != NULL; pl_EXP = pl_EXP->Suivant )
    {
        XZST_03EcritureTrace ( XZSTC_INFO , "Exploitation num:<%d>,  etat service : <%d>.",
                               pl_EXP->Numero,
                               pl_EXP->Etat_Service);

        /* Si trouve */
        if  (pl_EXP->Numero == vl_NumEXP)
        {
            /* Recherche le rang de commande des equipements concernes */
            for ( pl_Equipement = pl_EXP->ListeEquipement, vl_IndexCmd = 0;
                  pl_Equipement != NULL;
                  pl_Equipement = pl_Equipement->Suivant)
            {
                XZST_03EcritureTrace ( XZSTC_INFO , "AZA pl_Equipement->NumeroEquipement :<%d>,  Numero a trouver vl_NumEXP : <%d>.",
                                       pl_Equipement->NumeroEquipement,
                                       vl_NumEXP);
                /* recuperation du 1er type ouvrage des equipements */
                if (vl_TypeOuvrage == 0)
                {
                    vl_TypeOuvrage = pl_Equipement->TypeOuvrage;
                }

                vl_Cmd[vl_IndexCmd] = 0;
                vl_Eqt[vl_IndexCmd]=0;
                vl_TypeEqt[vl_IndexCmd]=0;
                for ( vl_Index = 0; 
                     (vl_Index < XZETC_MAX_UNITAIRE) && (vl_NumEqt[vl_Index] != 0); 
                      vl_Index++ )
                {

					XZST_03EcritureTrace ( XZSTC_INFO , "AZA Equipement vl_NumEqt[%d] : %d.", vl_Index, vl_NumEqt[vl_Index]);

                    /* Si equipement trouve */
                    if (pl_Equipement->NumeroEquipement == vl_NumEqt[vl_Index] && pl_Equipement->TypeEquipement == vl_NumTypeEqt[vl_Index])
                    {

                        /* Stock la commande et l equipement */
                        vl_Eqt[vl_IndexCmd]   = vl_NumEqt[vl_Index];
                        vl_TypeEqt[vl_IndexCmd]   = vl_NumTypeEqt[vl_Index];
                        vl_Cmd[vl_IndexCmd++] = vl_NumCmd[vl_Index];
                        break;
                    }
                }
            }
            break; /* Fin recherche EXP */
        }
    }

    if ( pl_EXP == NULL )
    {       
        /* Sortie : domaine energie non trouve ou en local */
        XZST_03EcritureTrace( XZSTC_WARNING , "Exploitation:%d non trouve ou en local ou inactif pour Action:%d.",
                              vl_NumEXP,
                              vl_NumAct );
    }
    else
    {
        /* Envoie des commandes a maitre Wizcon */
        vl_commande_TWWI0= et_envoi_cmd_unitaire( vg_SockTWWI0, 
                                                  vl_TypeOuvrage,
                                                  vl_TypeEqt,
                                                  vl_Eqt,
                                                  vl_Cmd,
                                                  vl_IndexCmd,
                                                  pl_EXP->Numero,
                                                  XDC_EQT_EXP, 
                                                  pl_EXP->DerniereSequence);

        /* Envoie des commandes a esclave Wizcon */     
        vl_commande_TWWI1= et_envoi_cmd_unitaire( vg_SockTWWI1, 
                                                  vl_TypeOuvrage,
                                                  vl_TypeEqt,
                                                  vl_Eqt,
                                                  vl_Cmd,
                                                  vl_IndexCmd,
                                                  pl_EXP->Numero,
                                                  XDC_EQT_EXP, 
                                                  pl_EXP->DerniereSequence);
        
        /* Si la maitre est la tache maitre Wizcon c'est son r�sultat qui fait foi*/
        if (vg_maitre_actif == XDC_MAITRE_TWWI0)
        { 
            if (vl_commande_TWWI0 == XDC_OK)
            {
                /*A Memorisation de la sequence en cours */
                pl_EXP->DerniereSequence = vl_NumSeq;
            }
        }
        else
        {
            if (vl_commande_TWWI1 == XDC_OK)
            {
                /*A Memorisation de la sequence en cours */
                pl_EXP->DerniereSequence = vl_NumSeq;
            }
        }

    }

    XZST_03EcritureTrace ( XZSTC_FONCTION, " OUT: et_cmd_exp_cb " );
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande commande domaine protection incendie
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void    et_cmd_inc_cb     ( T_IPC_CONN                   va_Cnx,
                            T_IPC_CONN_PROCESS_CB_DATA   pa_Data,
                            T_CB_ARG                     pa_ARG)
 
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
*  XDM_ECMD_INC
*
* FONCTION 
*  Pour toutes la protection incendies concernee
*  Envoyer a la tache TEXA0 la commande
*
------------------------------------------------------*/
{
    static char                  *version = "@(#)enix_cmd.c	1.6 06/16/09      : et_cmd_inc_cb " ;

    ENIX_DONNEES_INC *           pl_INC;
    ENIX_DONNEES_Equipement *    pl_Equipement;
#ifdef _TIBCO_EMS
    XDY_Entier                   vl_NumINC,
                                 vl_NumAct,
                                 vl_NumSeq,
                                 vl_NbEqt,
 	                         vl_NbTypeEqt,
                                 vl_NbCmd;
    XDY_Mot                      pl_NumEqt[255],
	                         pl_NumTypeEqt[255],
                                 pl_NumCmd[255];
#else
    T_INT4                       vl_NumINC,
                                 vl_NumAct,
                                 vl_NumSeq,
                                 vl_NbEqt,
 	                         vl_NbTypeEqt,
                                 vl_NbCmd;
    T_INT2                       *pl_NumEqt,
	                             *pl_NumTypeEqt,
                                 *pl_NumCmd;
#endif
    short                        vl_NumEqt[XZETC_MAX_UNITAIRE],
                                 vl_NumTypeEqt[XZETC_MAX_UNITAIRE],
								 vl_TypeEqt[XZETC_MAX_UNITAIRE],
								 vl_Eqt[XZETC_MAX_UNITAIRE],
                                 vl_NumCmd[XZETC_MAX_UNITAIRE];
    short                        vl_Rang1[XZETC_MAX_UNITAIRE],
                                 vl_Rang2[XZETC_MAX_UNITAIRE],
                                 vl_Cmd[XZETC_MAX_UNITAIRE];
    int                          vl_Index, vl_IndexCmd;
    int                          vl_commande_TWWI0,vl_commande_TWWI1;
    
    int		                     vl_TypeOuvrage;

    XZST_03EcritureTrace(XZSTC_FONCTION, " IN  : et_cmd_inc_cb ");

    /*A Recuperation des parametres du message XDM_ECMD_INC */
    if(!TipcMsgRead( pa_Data -> msg,
                     T_IPC_FT_INT4,       &vl_NumINC,
                     T_IPC_FT_INT2_ARRAY, &pl_NumTypeEqt,&vl_NbTypeEqt,
                     T_IPC_FT_INT2_ARRAY, &pl_NumEqt,&vl_NbEqt,
                     T_IPC_FT_INT2_ARRAY, &pl_NumCmd,&vl_NbCmd,
                     T_IPC_FT_INT4,       &vl_NumSeq,
                     T_IPC_FT_INT4,       &vl_NumAct,
                     NULL))
    {       
        /* Trace : Impossibilite de lire le message */
        XZST_03EcritureTrace ( XZSTC_WARNING , "Messsage XDM_ECMD_INC non lu ." );
        return;
    }

#ifdef _TIBCO_EMS
    for ( vl_Index = 0;
          vl_Index < XZETC_MAX_UNITAIRE;
          vl_NumEqt[vl_Index] = pl_NumEqt[vl_Index], 
          vl_NumTypeEqt[vl_Index] = pl_NumTypeEqt[vl_Index], 
		  vl_NumCmd[vl_Index] = pl_NumCmd[vl_Index] , 
		  vl_Index++);
#else
    for ( vl_Index = 0;
          vl_Index < XZETC_MAX_UNITAIRE;
          vl_NumEqt[vl_Index] = *pl_NumEqt, 
          vl_NumTypeEqt[vl_Index] = *pl_NumTypeEqt, 
		  vl_NumCmd[vl_Index] = *pl_NumCmd , 
		  vl_Index++, 
		  pl_NumEqt++, 
		  pl_NumCmd++,
		  pl_NumTypeEqt++);
#endif

    vl_TypeOuvrage = 0;
    /* Recherche dans la liste le domaine concerne */
    for ( pl_INC = pg_debutINC; pl_INC != NULL; pl_INC = pl_INC->Suivant )
    {
        XZST_03EcritureTrace ( XZSTC_INFO , "Protection incendie num:<%d>,  etat service : <%d>.",
                               pl_INC->Numero,
                               pl_INC->Etat_Service);

        /* Si trouve et pas protection incendie */
        if  (pl_INC->Numero == vl_NumINC)
        {
            /* Recherche le rang de commande des equipements concernes */
            for ( pl_Equipement = pl_INC->ListeEquipement, vl_IndexCmd = 0;
                  pl_Equipement != NULL;
                  pl_Equipement = pl_Equipement->Suivant)
            {
                XZST_03EcritureTrace ( XZSTC_INFO , "AZA pl_Equipement->NumeroEquipement :<%d>,  Numero a trouver vl_NumINC : <%d>.",
                                       pl_Equipement->NumeroEquipement,
                                       vl_NumINC);
                /* recuperation du 1er type ouvrage des equipements */
                if (vl_TypeOuvrage == 0)
                {
                    vl_TypeOuvrage = pl_Equipement->TypeOuvrage;
                }

                vl_Cmd[vl_IndexCmd] = 0;
                vl_TypeEqt[vl_IndexCmd]=0;
                vl_Eqt[vl_IndexCmd]=0;
                for ( vl_Index = 0; 
                     (vl_Index < XZETC_MAX_UNITAIRE) && (vl_NumEqt[vl_Index] != 0); 
                      vl_Index++ )
                {

                XZST_03EcritureTrace ( XZSTC_INFO , "AZA Equipement vl_NumEqt[%d] : %d.", vl_Index, vl_NumEqt[vl_Index]);

                    /* Si equipement trouve */
                    if (pl_Equipement->NumeroEquipement == vl_NumEqt[vl_Index] && pl_Equipement->TypeEquipement == vl_NumTypeEqt[vl_Index])
                    {

                        /* Stock la commande et l equipement */
                        vl_Eqt[vl_IndexCmd]   = vl_NumEqt[vl_Index];
                        vl_TypeEqt[vl_IndexCmd]   = vl_NumTypeEqt[vl_Index];
                        vl_Cmd[vl_IndexCmd++] = vl_NumCmd[vl_Index];
                        break;
                    }
                }
            }
            break; /* Fin recherche INC */
        }
    }

    if ( pl_INC == NULL )
    {       
        /* Sortie : domaine protection incendie non trouve ou en local */
        XZST_03EcritureTrace( XZSTC_WARNING , "Protection incendie:%d non trouve ou en local ou inactif pour Action:%d.",
                              vl_NumINC,
                              vl_NumAct );
    }
    else
    {

        /* Envoie des commandes a maitre Wizcon */
        vl_commande_TWWI0= et_envoi_cmd_unitaire( vg_SockTWWI0, 
                                                  vl_TypeOuvrage,
                                                  vl_TypeEqt,
                                                  vl_Eqt,
                                                  vl_Cmd,
                                                  vl_IndexCmd,
                                                  pl_INC->Numero,
                                                  XDC_EQT_INC, 
                                                  pl_INC->DerniereSequence);

        /* Envoie des commandes a esclave Wizcon */     
        vl_commande_TWWI1= et_envoi_cmd_unitaire( vg_SockTWWI1, 
                                                  vl_TypeOuvrage,
                                                  vl_TypeEqt,
                                                  vl_Eqt,
                                                  vl_Cmd,
                                                  vl_IndexCmd,
                                                  pl_INC->Numero,
                                                  XDC_EQT_INC, 
                                                  pl_INC->DerniereSequence);
        
        /* Si la maitre est la tache maitre Wizcon c'est son r�sultat qui fait foi*/
        if (vg_maitre_actif == XDC_MAITRE_TWWI0)
        { 
            if (vl_commande_TWWI0 == XDC_OK)
            {
                /*A Memorisation de la sequence en cours */
                pl_INC->DerniereSequence = vl_NumSeq;
            }
        }
        else
        {
            if (vl_commande_TWWI1 == XDC_OK)
            {
                /*A Memorisation de la sequence en cours */
                pl_INC->DerniereSequence = vl_NumSeq;
            }
        }
    }
    XZST_03EcritureTrace ( XZSTC_FONCTION, " OUT: et_cmd_inc_cb " );
}


/* Domaines SIG, ECL, VEN, ENR, INC */
/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande commande sequence domaine signalisation 
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void    et_cmd_seq_sig_cb( T_IPC_CONN                  va_Cnx,
                           T_IPC_CONN_PROCESS_CB_DATA  pa_Data,
                           T_CB_ARG                    pa_ARG)
 
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
*  XDM_ECMD_SEQSIG
*
* FONCTION 
*  Envoyer a la tache TEXA0 la commande de la sequence concernee
*
------------------------------------------------------*/
{
    static char              *version = "@(#)enix_cmd.c	1.6 06/16/09      : et_cmd_seq_sig_cb " ;

#ifdef _TIBCO_EMS
    XDY_Entier               vl_TypeOuvrage,
		 	     vl_NumSIG,
                             vl_NumAct,
                             vl_NumSeq;
#else
    T_INT4                   vl_TypeOuvrage,
		 	     vl_NumSIG,
                             vl_NumAct,
                             vl_NumSeq;
#endif
    ENIX_DONNEES_SIG *       pl_SIG;
    ENIX_DONNEES_Sequence *  pl_Seq;
    int                      vl_js;
    XDY_Horodate             vl_horodate;
    int                      vl_commande_TWWI0,
                             vl_commande_TWWI1;
    
    XZST_03EcritureTrace ( XZSTC_FONCTION, " IN  : et_cmd_seq_sig_cb " );
          
    /*A Recuperation des parametres du message XDM_ECMD_SEQSIG */
    if(!TipcMsgRead( pa_Data -> msg,
                     T_IPC_FT_INT4, &vl_TypeOuvrage,
                     T_IPC_FT_INT4, &vl_NumSIG,
                     T_IPC_FT_INT4, &vl_NumSeq,
                     T_IPC_FT_INT4, &vl_NumAct,
                     NULL))
    {       
        /* Trace : Impossibilite de lire le message */
        XZST_03EcritureTrace ( XZSTC_WARNING, "Messsage XDM_ECMD_SEQSIG non lu ." );
        return;
    }
	XZST_03EcritureTrace(XZSTC_DEBUG1 ,"Type ouvrage = %d.",vl_TypeOuvrage);

    /* Recherche dans la liste le domaine concerne */
    for ( pl_SIG = pg_debutSIG; pl_SIG != NULL; pl_SIG = pl_SIG->Suivant )
    {
        XZST_03EcritureTrace(XZSTC_INFO ,"Domaine signalisation num:<%d>, etat service : <%d>, type ouvrage = <%d>.",pl_SIG->Numero,pl_SIG->Etat_Service, pl_SIG->TypeOuvrage);
        /* Si domaine trouve et pas en local */
        if (pl_SIG->Numero == vl_NumSIG && pl_SIG->TypeOuvrage == vl_TypeOuvrage)
        {
            /* Recherche si la sequence existe */
            for ( pl_Seq = pl_SIG->ListeSequence; pl_Seq != NULL; pl_Seq = pl_Seq->Suivant )
            {
                XZST_03EcritureTrace ( XZSTC_INFO ,"Sequence num:<%d>, etat disc:<%d>.",
                                       pl_Seq->NumSequence,
                                       pl_Seq->EtatDisc);
                /* Si sequence trouve */
                if ( pl_Seq->NumSequence == vl_NumSeq )
                {
					/* Envoi de la commande sequence a maitre Wizcon */
					vl_commande_TWWI0=et_envoi_cmd_sequence ( vg_SockTWWI0, 
															  pl_SIG->TypeOuvrage,
															  XDC_EQT_SIG,
															  pl_SIG->Numero,
															  vl_NumSeq );

					/* Envoi de la commande sequence a esclave Wizcon */
					vl_commande_TWWI1=et_envoi_cmd_sequence ( vg_SockTWWI1, 
															  pl_SIG->TypeOuvrage,
															  XDC_EQT_SIG,
															  pl_SIG->Numero,
															  vl_NumSeq );

					/*A Memorisation de la sequence en cours */
					pl_SIG->DerniereSequence = vl_NumSeq;
					pl_Seq->NumAction        = vl_NumAct;
					pl_Seq->AttenteEtat = XDC_OUI;
                    break;
                }
            }
            break; /* Fin recherche SIG */
        }
    }

    if ( pl_SIG == NULL || pl_Seq == NULL )
    {
        /* Envoyer echec commande sequence a BD */
        /* Appel de XZAT40 avec XDC_NOK et NumAction */
        XZSM_07LireHorodate ( XZSMC_TYPE_SYSTEME ,
                              &vl_js ,
                              &vl_horodate );
		XZST_03EcritureTrace ( XZSTC_DEBUG1 ,"Appel de xzat40_CompteRendu_SIG (numact =%d) : NOK", vl_NumAct);
		xzat40_CompteRendu_SIG (vl_NumAct,
								vl_horodate,
								XDC_NOK);
		XZST_03EcritureTrace ( XZSTC_WARNING, "Domaine signalisation:%d non trouve ou en local ou inactif ou sequence:%d non trouvee ou en discordance pour Action:%d",
							   vl_NumSIG,
							   vl_NumSeq,
							   vl_NumAct );
    }
	else
	{
		et_armer_timer_verification_sequence(pl_SIG->TypeOuvrage,
											 XDC_EQT_SIG,
											 pl_SIG->Numero,
											 vl_NumSeq);
	}

    XZST_03EcritureTrace ( XZSTC_FONCTION, " OUT: et_cmd_seq_sig_cb " );
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande commande sequence domaine eclairage 
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void    et_cmd_seq_ecl_cb( T_IPC_CONN                  va_Cnx,
                           T_IPC_CONN_PROCESS_CB_DATA  pa_Data,
                           T_CB_ARG                    pa_ARG)
 
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
*  XDM_ECMD_SEQECL
*
* FONCTION 
*  Envoyer a la tache TEXA0 la commande de la sequence concernee
*
------------------------------------------------------*/
{
    static char              *version = "@(#)enix_cmd.c	1.6 06/16/09      : et_cmd_seq_ecl_cb " ;
  
#ifdef _TIBCO_EMS
    XDY_Entier               vl_TypeOuvrage,
    			     vl_NumECL,
                             vl_NumAct,
                             vl_NumSeq;
#else
    T_INT4                   vl_TypeOuvrage,
    			     vl_NumECL,
                             vl_NumAct,
                             vl_NumSeq;
#endif
    ENIX_DONNEES_ECL *       pl_ECL;
    ENIX_DONNEES_Sequence *  pl_Seq;
    int                      vl_js;
    XDY_Horodate             vl_horodate;
    int                      vl_commande_TWWI0,
                             vl_commande_TWWI1;

    XZST_03EcritureTrace ( XZSTC_FONCTION, " IN  : et_cmd_seq_ecl_cb " );
          
    /*A Recuperation des parametres du message XDM_ECMD_SEQECL */
    if(!TipcMsgRead( pa_Data -> msg,
                     T_IPC_FT_INT4, &vl_TypeOuvrage,
                     T_IPC_FT_INT4, &vl_NumECL,
                     T_IPC_FT_INT4, &vl_NumSeq,
                     T_IPC_FT_INT4, &vl_NumAct,
                     NULL))
    {       
        /* Trace : Impossibilite de lire le message */
        XZST_03EcritureTrace ( XZSTC_WARNING, "Messsage XDM_ECMD_SEQECL non lu ." );
        return;
    }

    /* Recherche dans la liste le domaine concerne */
    for ( pl_ECL = pg_debutECL; pl_ECL != NULL; pl_ECL = pl_ECL->Suivant )
    {
        XZST_03EcritureTrace ( XZSTC_INFO ,"Domaine eclairage num:<%d>, etat service : <%d>.",
                               pl_ECL->Numero,
                               pl_ECL->Etat_Service);
        /* Si domaine trouve et pas en local */
        if  (pl_ECL->Numero == vl_NumECL && pl_ECL->TypeOuvrage == vl_TypeOuvrage)
        {
            /* Recherche si la sequence existe */
            for ( pl_Seq = pl_ECL->ListeSequence; pl_Seq != NULL; pl_Seq = pl_Seq->Suivant )
            {
                XZST_03EcritureTrace ( XZSTC_INFO, "Sequence num:<%d>, etat disc:<%d>.",
                                       pl_Seq->NumSequence,
                                       pl_Seq->EtatDisc);
                /* Si sequence trouve */
                if ( pl_Seq->NumSequence == vl_NumSeq )
                {
					/* Envoi de la commande sequence a maitre Wizcon */
					vl_commande_TWWI0=et_envoi_cmd_sequence ( vg_SockTWWI0, 
															  pl_ECL->TypeOuvrage,
															  XDC_EQT_ECL,
															  pl_ECL->Numero,
															  vl_NumSeq );

					/* Envoi de la commande sequence a esclave Wizcon */
					vl_commande_TWWI1=et_envoi_cmd_sequence ( vg_SockTWWI1, 
															  pl_ECL->TypeOuvrage,
															  XDC_EQT_ECL,
															  pl_ECL->Numero,
															  vl_NumSeq );
					if (vg_maitre_actif == XDC_MAITRE_TWWI0)
					{
						if (vl_commande_TWWI0 == XDC_OK)
						{
							/*A Memorisation de la sequence en cours */
							pl_ECL->DerniereSequence = vl_NumSeq;
							pl_Seq->NumAction        = vl_NumAct;
						}
					}
					else
					{
						if (vl_commande_TWWI1 == XDC_OK)
						{
							/*A Memorisation de la sequence en cours */
							pl_ECL->DerniereSequence = vl_NumSeq;
							pl_Seq->NumAction        = vl_NumAct;
						}
					}
					pl_Seq->AttenteEtat = XDC_OUI;
                    break;
                }
            }
            break; /* Fin recherche ECL */
        }
    }

    if ( pl_ECL == NULL || pl_Seq == NULL )
    {
        /* Envoyer echec commande sequence a BD */
        /* Appel de XZAT41 avec XDC_NOK et NumAction */
        XZSM_07LireHorodate ( XZSMC_TYPE_SYSTEME ,
                              &vl_js,
                              &vl_horodate );
        xzat41_CompteRendu_ECL ( vl_NumAct,
                                 vl_horodate,
                                 XDC_NOK);
        XZST_03EcritureTrace ( XZSTC_WARNING, "Domaine eclairage:%d non trouve ou en local ou inactif ou sequence:%d non trouvee ou en discordance pour Action:%d",
                               vl_NumECL,
                               vl_NumSeq,
                               vl_NumAct );
    }
	else
	{
		et_armer_timer_verification_sequence(pl_ECL->TypeOuvrage,
											 XDC_EQT_ECL,
											 pl_ECL->Numero,
											 vl_NumSeq);
	}
	
    
    XZST_03EcritureTrace ( XZSTC_FONCTION, " OUT: et_cmd_seq_ecl_cb " );
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande commande sequence domaine ventilation
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void    et_cmd_seq_ven_cb( T_IPC_CONN                  va_Cnx,
                           T_IPC_CONN_PROCESS_CB_DATA  pa_Data,
                           T_CB_ARG                    pa_ARG)
 
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
*  XDM_ECMD_SEQVEN
*
* FONCTION 
*  Envoyer a la tache TEXA0 la commande de la sequence concernee
*
------------------------------------------------------*/
{
    static char              *version = "@(#)enix_cmd.c	1.6 06/16/09      : et_cmd_seq_ven_cb " ;
  
#ifdef _TIBCO_EMS
    XDY_Entier                   vl_TypeOuvrage,
							 vl_NumVEN,
                             vl_NumAct,
                             vl_NumSeq;
#else
    T_INT4                   vl_TypeOuvrage,
							 vl_NumVEN,
                             vl_NumAct,
                             vl_NumSeq;
#endif
    ENIX_DONNEES_VEN *       pl_VEN;
    ENIX_DONNEES_Sequence *  pl_Seq;
    int                      vl_js;
    XDY_Horodate             vl_horodate;
    int                      vl_commande_TWWI0,
                             vl_commande_TWWI1;

    XZST_03EcritureTrace ( XZSTC_FONCTION, " IN  : et_cmd_seq_ven_cb " );
          
    /*A Recuperation des parametres du message XDM_ECMD_SEQVEN */
    if(!TipcMsgRead( pa_Data -> msg,
                     T_IPC_FT_INT4, &vl_TypeOuvrage,
                     T_IPC_FT_INT4, &vl_NumVEN,
                     T_IPC_FT_INT4, &vl_NumSeq,
                     T_IPC_FT_INT4, &vl_NumAct,
                     NULL))
    {       
        /* Trace : Impossibilite de lire le message */
        XZST_03EcritureTrace ( XZSTC_WARNING, "Messsage XDM_ECMD_SEQVEN non lu ." );
        return;
    }

    /* Recherche dans la liste le domaine concerne */
    for ( pl_VEN = pg_debutVEN; pl_VEN != NULL; pl_VEN = pl_VEN->Suivant )
    {
        XZST_03EcritureTrace(XZSTC_INFO ,"Domaine ventilation num:<%d>, etat service : <%d>.",pl_VEN->Numero,pl_VEN->Etat_Service);
        /* Si domaine trouve et pas en local */
        if  (pl_VEN->Numero == vl_NumVEN && pl_VEN->TypeOuvrage == vl_TypeOuvrage)
        {
            /* Recherche si la sequence existe */
            for ( pl_Seq = pl_VEN->ListeSequence; pl_Seq != NULL; pl_Seq = pl_Seq->Suivant )
            {
                XZST_03EcritureTrace ( XZSTC_INFO ,"Sequence num:<%d>, etat disc:<%d>.",
                                       pl_Seq->NumSequence,
                                       pl_Seq->EtatDisc);
                /* Si sequence trouve */
                if ( pl_Seq->NumSequence == vl_NumSeq )
                {
					/* Envoi de la commande sequence a maitre Wizcon */
					vl_commande_TWWI0=et_envoi_cmd_sequence ( vg_SockTWWI0, 
															  pl_VEN->TypeOuvrage,
															  XDC_EQT_VEN,
															  pl_VEN->Numero,
															  vl_NumSeq );

					/* Envoi de la commande sequence a esclave Wizcon */
					vl_commande_TWWI1=et_envoi_cmd_sequence ( vg_SockTWWI1, 
															  pl_VEN->TypeOuvrage,
															  XDC_EQT_VEN,
															  pl_VEN->Numero,
															  vl_NumSeq );

					if (vg_maitre_actif == XDC_MAITRE_TWWI0)
					{
						if (vl_commande_TWWI0 == XDC_OK)
						{
							/*A Memorisation de la sequence en cours */
							pl_VEN->DerniereSequence = vl_NumSeq;
							pl_Seq->NumAction        = vl_NumAct;
						}
					}
					else
					{
						if (vl_commande_TWWI1 == XDC_OK)
						{
							/*A Memorisation de la sequence en cours */
							pl_VEN->DerniereSequence = vl_NumSeq;
							pl_Seq->NumAction        = vl_NumAct;
						}
					}
					pl_Seq->AttenteEtat = XDC_OUI;
                    break;
                }
            }
            break; /* Fin recherche VEN */
        }
    }

    if ( pl_VEN == NULL || pl_Seq == NULL )
    {
        /* Envoyer echec commande sequence a BD */
        /* Appel de XZAT42 avec XDC_NOK et NumAction */
        XZSM_07LireHorodate ( XZSMC_TYPE_SYSTEME ,
                              &vl_js ,
                              &vl_horodate );
        xzat42_CompteRendu_VEN (vl_NumAct,
                                vl_horodate,
                                XDC_NOK);
        XZST_03EcritureTrace ( XZSTC_WARNING, "Domaine ventilation:%d non trouve ou en local ou inactif ou sequence:%d non trouvee ou en discordance pour Action:%d",
                               vl_NumVEN,
                               vl_NumSeq,
                               vl_NumAct );
    }
    else
	{
		et_armer_timer_verification_sequence(pl_VEN->TypeOuvrage,
											 XDC_EQT_VEN,
											 pl_VEN->Numero,
											 vl_NumSeq);
	}
    XZST_03EcritureTrace ( XZSTC_FONCTION, " OUT: et_cmd_seq_ven_cb " );
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande commande sequence domaine energie
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void    et_cmd_seq_enr_cb( T_IPC_CONN                  va_Cnx,
                           T_IPC_CONN_PROCESS_CB_DATA  pa_Data,
                           T_CB_ARG                    pa_ARG)
 
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
*  XDM_ECMD_SEQENR
*
* FONCTION 
*  Envoyer a la tache TEXA0 la commande de la sequence concernee
*
------------------------------------------------------*/
{
    static char              *version = "@(#)enix_cmd.c	1.6 06/16/09      : et_cmd_seq_enr_cb " ;

#ifdef _TIBCO_EMS
    XDY_Entier                   vl_TypeOuvrage,
							 vl_NumENR,
                             vl_NumAct,
                             vl_NumSeq;
#else
    T_INT4                   vl_TypeOuvrage,
							 vl_NumENR,
                             vl_NumAct,
                             vl_NumSeq;
#endif
    ENIX_DONNEES_ENR *       pl_ENR;
    ENIX_DONNEES_Sequence *  pl_Seq;
    int                      vl_js;
    XDY_Horodate             vl_horodate;
    int                      vl_commande_TWWI0,
                             vl_commande_TWWI1;

    XZST_03EcritureTrace(XZSTC_FONCTION, " IN  : et_cmd_seq_enr_cb ");
          
    /*A Recuperation des parametres du message XDM_ECMD_SEQENR */
    if(!TipcMsgRead( pa_Data -> msg,
                     T_IPC_FT_INT4, &vl_TypeOuvrage,
                     T_IPC_FT_INT4, &vl_NumENR,
                     T_IPC_FT_INT4, &vl_NumSeq,
                     T_IPC_FT_INT4, &vl_NumAct,
                     NULL))
    {       
        /* Trace : Impossibilite de lire le message */
        XZST_03EcritureTrace(XZSTC_WARNING ,"Messsage XDM_ECMD_SEQENR non lu ." );
        return;
    }

    /* Recherche dans la liste le domaine concerne */
    for ( pl_ENR = pg_debutENR; pl_ENR != NULL; pl_ENR = pl_ENR->Suivant )
    {
        XZST_03EcritureTrace(XZSTC_INFO ,"Domaine energie num:<%d>, etat service : <%d>.",pl_ENR->Numero,pl_ENR->Etat_Service);
        /* Si domaine trouve et pas en local */
        if  (pl_ENR->Numero == vl_NumENR && pl_ENR->TypeOuvrage == vl_TypeOuvrage)
        {
            /* Recherche si la sequence existe */
            for ( pl_Seq = pl_ENR->ListeSequence; pl_Seq != NULL; pl_Seq = pl_Seq->Suivant )
            {
                XZST_03EcritureTrace(XZSTC_INFO ,"Sequence num:<%d>, etat disc:<%d>.",pl_Seq->NumSequence,pl_Seq->EtatDisc);
                /* Si sequence trouve */
                if ( pl_Seq->NumSequence == vl_NumSeq )
                {
					/* Envoi de la commande sequence a maitre Wizcon */
					vl_commande_TWWI0=et_envoi_cmd_sequence ( vg_SockTWWI0, 
															  pl_ENR->TypeOuvrage,
															  XDC_EQT_ENR,
															  pl_ENR->Numero,
															  vl_NumSeq );

					/* Envoi de la commande sequence a esclave Wizcon */
					vl_commande_TWWI1=et_envoi_cmd_sequence ( vg_SockTWWI1, 
															  pl_ENR->TypeOuvrage,
															  XDC_EQT_ENR,
															  pl_ENR->Numero,
															  vl_NumSeq );

					if (vg_maitre_actif == XDC_MAITRE_TWWI0)
					{
						if (vl_commande_TWWI0 == XDC_OK)
						{
							/*A Memorisation de la sequence en cours */
							pl_ENR->DerniereSequence = vl_NumSeq;
							pl_Seq->NumAction        = vl_NumAct;
						}
					}
					else
					{
						if (vl_commande_TWWI1 == XDC_OK)
						{
							/*A Memorisation de la sequence en cours */
							pl_ENR->DerniereSequence = vl_NumSeq;
							pl_Seq->NumAction        = vl_NumAct;
						}
					}
					pl_Seq->AttenteEtat = XDC_OUI;
                    break;
                }
            }
            break; /* Fin recherche ENR */
        }
    }

    if ( (pl_ENR == NULL) || (pl_Seq == NULL) )
    {
        /* Envoyer echec commande sequence a BD */
        /* Appel de XZAT43 avec XDC_NOK et NumAction */
        XZSM_07LireHorodate ( XZSMC_TYPE_SYSTEME ,
                              &vl_js ,
                              &vl_horodate );
        xzat43_CompteRendu_ENR ( vl_NumAct,
                                 vl_horodate,
                                 XDC_NOK);
        XZST_03EcritureTrace ( XZSTC_WARNING, "Domaine energie:%d non trouve ou en local ou inactif ou sequence:%d non trouvee ou en discordance pour Action:%d",
                               vl_NumENR,
                               vl_NumSeq,
                               vl_NumAct );
    }
    else
	{
		et_armer_timer_verification_sequence(pl_ENR->TypeOuvrage,
											 XDC_EQT_ENR,
											 pl_ENR->Numero,
											 vl_NumSeq);
	}
    
    XZST_03EcritureTrace(XZSTC_FONCTION, " OUT: et_cmd_seq_enr_cb ");
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande commande sequence domaine protection incendie
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void    et_cmd_seq_inc_cb( T_IPC_CONN                  va_Cnx,
                           T_IPC_CONN_PROCESS_CB_DATA  pa_Data,
                           T_CB_ARG                    pa_ARG)
 
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
*  XDM_ECMD_SEQINC
*
* FONCTION 
*  Envoyer a la tache TEXA0 la commande de la sequence concernee
*
------------------------------------------------------*/
{
    static char              *version = "@(#)enix_cmd.c	1.6 06/16/09      : et_cmd_seq_inc_cb " ;

#ifdef _TIBCO_EMS
    XDY_Entier                   vl_TypeOuvrage,
							 vl_NumINC,
                             vl_NumAct,
                             vl_NumSeq;
#else
    T_INT4                   vl_TypeOuvrage,
							 vl_NumINC,
                             vl_NumAct,
                             vl_NumSeq;
#endif
    ENIX_DONNEES_INC *       pl_INC;
    ENIX_DONNEES_Sequence *  pl_Seq;
    int                      vl_js;
    XDY_Horodate             vl_horodate;
    int                      vl_commande_TWWI0,
                             vl_commande_TWWI1;

    XZST_03EcritureTrace(XZSTC_FONCTION, " IN  : et_cmd_seq_inc_cb ");
          
    /*A Recuperation des parametres du message XDM_ECMD_SEQINC */
    if(!TipcMsgRead( pa_Data -> msg,
                     T_IPC_FT_INT4, &vl_TypeOuvrage,
                     T_IPC_FT_INT4, &vl_NumINC,
                     T_IPC_FT_INT4, &vl_NumSeq,
                     T_IPC_FT_INT4, &vl_NumAct,
                     NULL))
    {       
        /* Trace : Impossibilite de lire le message */
        XZST_03EcritureTrace ( XZSTC_WARNING, "Messsage XDM_ECMD_SEQINC non lu ." );
        return;
    }

    /* Recherche dans la liste le domaine concerne */
    for ( pl_INC = pg_debutINC; pl_INC != NULL; pl_INC = pl_INC->Suivant )
    {
        XZST_03EcritureTrace(XZSTC_INFO ,"Domaine prot. incendie num:<%d>, etat service : <%d>.",pl_INC->Numero,pl_INC->Etat_Service);
        /* Si domaine trouve et pas en local */
        if  (pl_INC->Numero == vl_NumINC && pl_INC->TypeOuvrage == vl_TypeOuvrage)
        {
            /* Recherche si la sequence existe */
            for ( pl_Seq = pl_INC->ListeSequence; pl_Seq != NULL; pl_Seq = pl_Seq->Suivant )
            {
                XZST_03EcritureTrace ( XZSTC_INFO, "Sequence num:<%d>, etat disc:<%d>.",
                                       pl_Seq->NumSequence,
                                       pl_Seq->EtatDisc);
                /* Si sequence trouve */
                if ( pl_Seq->NumSequence == vl_NumSeq )
                {
					/* Envoi de la commande sequence a maitre Wizcon */
					vl_commande_TWWI0=et_envoi_cmd_sequence ( vg_SockTWWI0, 
															  pl_INC->TypeOuvrage,
															  XDC_EQT_INC,
															  pl_INC->Numero,
															  vl_NumSeq );

					/* Envoi de la commande sequence a esclave Wizcon */
					vl_commande_TWWI1=et_envoi_cmd_sequence ( vg_SockTWWI1, 
															  pl_INC->TypeOuvrage,
															  XDC_EQT_INC,
															  pl_INC->Numero,
															  vl_NumSeq );


					if (vg_maitre_actif == XDC_MAITRE_TWWI0)
					{
						if (vl_commande_TWWI0 == XDC_OK)
						{
							/*A Memorisation de la sequence en cours */
							pl_INC->DerniereSequence = vl_NumSeq;
							pl_Seq->NumAction        = vl_NumAct;
						}
					}
					else
					{
						if (vl_commande_TWWI1 == XDC_OK)
						{
							/*A Memorisation de la sequence en cours */
							pl_INC->DerniereSequence = vl_NumSeq;
							pl_Seq->NumAction        = vl_NumAct;
						}
					}
					pl_Seq->AttenteEtat = XDC_OUI;
                    break;
                }
            }
            break; /* Fin recherche INC */
        }
    }

    if ( (pl_INC == NULL) || (pl_Seq == NULL) )
    {
        /* Envoyer echec commande sequence a BD */
        /* Appel de XZAT44 avec XDC_NOK et NumAction */
        XZSM_07LireHorodate ( XZSMC_TYPE_SYSTEME ,
                              &vl_js ,
                              &vl_horodate );

        xzat44_CompteRendu_INC ( vl_NumAct,
                                 vl_horodate,
                                 XDC_NOK);

        XZST_03EcritureTrace ( XZSTC_WARNING, "Domaine prot. incendie :%d non trouve ou en local ou inactif ou sequence:%d non trouvee ou en discordance pour Action:%d",
                               vl_NumINC,
                               vl_NumSeq,
                               vl_NumAct );
    }
    else
	{
		et_armer_timer_verification_sequence(pl_INC->TypeOuvrage,
											 XDC_EQT_INC,
											 pl_INC->Numero,
											 vl_NumSeq);
	}

    XZST_03EcritureTrace(XZSTC_FONCTION, " OUT: et_cmd_seq_inc_cb ");
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande commande sequence domaine exploitation
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void    et_cmd_seq_exp_cb( T_IPC_CONN                  va_Cnx,
                           T_IPC_CONN_PROCESS_CB_DATA  pa_Data,
                           T_CB_ARG                    pa_ARG)
 
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
*  XDM_ECMD_SEQEXP
*
* FONCTION 
*  Envoyer a la tache TEXA0 la commande de la sequence concernee
*
------------------------------------------------------*/
{
    static char              *version = "@(#)enix_cmd.c	1.6 06/16/09      : et_cmd_seq_exp_cb " ;

#ifdef _TIBCO_EMS
    XDY_Entier                   vl_TypeOuvrage,
							 vl_NumEXP,
                             vl_NumAct,
                             vl_NumSeq;
#else
    T_INT4                   vl_TypeOuvrage,
							 vl_NumEXP,
                             vl_NumAct,
                             vl_NumSeq;
#endif
    ENIX_DONNEES_EXP *       pl_EXP;
    ENIX_DONNEES_Sequence *  pl_Seq;
    int                      vl_js;
    XDY_Horodate             vl_horodate;
    int                      vl_commande_TWWI0,
                             vl_commande_TWWI1;

    XZST_03EcritureTrace(XZSTC_FONCTION, " IN  : et_cmd_seq_exp_cb ");
          
    /*A Recuperation des parametres du message XDM_ECMD_SEQEXP */
    if(!TipcMsgRead( pa_Data -> msg,
                     T_IPC_FT_INT4, &vl_TypeOuvrage,
                     T_IPC_FT_INT4, &vl_NumEXP,
                     T_IPC_FT_INT4, &vl_NumSeq,
                     T_IPC_FT_INT4, &vl_NumAct,
                     NULL))
    {       
        /* Trace : Impossibilite de lire le message */
        XZST_03EcritureTrace ( XZSTC_WARNING, "Messsage XDM_ECMD_SEQEXP non lu " );
        return;
    }

    /* Recherche dans la liste le domaine concerne */
    for ( pl_EXP = pg_debutEXP; pl_EXP != NULL; pl_EXP = pl_EXP->Suivant )
    {
        XZST_03EcritureTrace(XZSTC_INFO ,"Domaine exploitation num:<%d>, etat service : <%d>.",pl_EXP->Numero,pl_EXP->Etat_Service);
        /* Si domaine trouve et pas en local */
        if  (pl_EXP->Numero == vl_NumEXP && pl_EXP->TypeOuvrage == vl_TypeOuvrage)
        {
            /* Recherche si la sequence existe */
            for ( pl_Seq = pl_EXP->ListeSequence; pl_Seq != NULL; pl_Seq = pl_Seq->Suivant )
            {
                XZST_03EcritureTrace ( XZSTC_INFO, "Sequence num:<%d>, etat disc:<%d>.",
                                       pl_Seq->NumSequence,
                                       pl_Seq->EtatDisc);
                /* Si sequence trouve */
                if ( pl_Seq->NumSequence == vl_NumSeq )
                {
					/* Envoi de la commande sequence a maitre Wizcon */
					vl_commande_TWWI0=et_envoi_cmd_sequence ( vg_SockTWWI0, 
															  pl_EXP->TypeOuvrage,
															  XDC_EQT_EXP,
															  pl_EXP->Numero,
															  vl_NumSeq );

					/* Envoi de la commande sequence a esclave Wizcon */
					vl_commande_TWWI1=et_envoi_cmd_sequence ( vg_SockTWWI1, 
															  pl_EXP->TypeOuvrage,
															  XDC_EQT_EXP,
															  pl_EXP->Numero,
															  vl_NumSeq );


					if (vg_maitre_actif == XDC_MAITRE_TWWI0)
					{
						if (vl_commande_TWWI0 == XDC_OK)
						{
							/*A Memorisation de la sequence en cours */
							pl_EXP->DerniereSequence = vl_NumSeq;
							pl_Seq->NumAction        = vl_NumAct;
						}
					}
					else
					{
						if (vl_commande_TWWI1 == XDC_OK)
						{
							/*A Memorisation de la sequence en cours */
							pl_EXP->DerniereSequence = vl_NumSeq;
							pl_Seq->NumAction        = vl_NumAct;
						}
					}
					pl_Seq->AttenteEtat = XDC_OUI;
                    break;
                }
            }
            break; /* Fin recherche EXP */
        }
    }

    if ( (pl_EXP == NULL) || (pl_Seq == NULL) )
    {
        /* Envoyer echec commande sequence a BD */
        /* Appel de XZAT45 avec XDC_NOK et NumAction */
        XZSM_07LireHorodate ( XZSMC_TYPE_SYSTEME ,
                              &vl_js ,
                              &vl_horodate );

						
        xzat45_CompteRendu_EXP ( vl_NumAct,
                                 vl_horodate,
                                 XDC_NOK);

        XZST_03EcritureTrace ( XZSTC_WARNING, "Domaine exploitation :%d non trouve ou en local ou inactif ou sequence:%d non trouvee ou en discordance pour Action:%d",
                               vl_NumEXP,
                               vl_NumSeq,
                               vl_NumAct );
    }
    else
	{
		et_armer_timer_verification_sequence(pl_EXP->TypeOuvrage,
											 XDC_EQT_EXP,
											 pl_EXP->Numero,
											 vl_NumSeq);
	}

    XZST_03EcritureTrace(XZSTC_FONCTION, " OUT: et_cmd_seq_inc_cb ");
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande commande  domaine 
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void    et_cmd_domaine_cb( T_IPC_CONN                  va_Cnx,
                           T_IPC_CONN_PROCESS_CB_DATA  pa_Data,
                           T_CB_ARG                    pa_ARG)
 
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
*  XDM_ECMD_SEQINC
*
* FONCTION 
*  Envoyer a la tache TEXA0 la commande de la sequence concernee
*
------------------------------------------------------*/
{
    static char              *version = "@(#)enix_cmd.c	1.6 06/16/09      : et_cmd_domaine_cb " ;

#ifdef _TIBCO_EMS
    XDY_Entier                   vl_Domaine,
							 vl_TypeOuvrage,
							 vl_IdOuvrage,
							 vl_Mode,
                             vl_NumAct;
#else
    T_INT4                   vl_Domaine,
							 vl_TypeOuvrage,
							 vl_IdOuvrage,
							 vl_Mode,
                             vl_NumAct;
#endif
                             
    ENIX_DONNEES_SIG *        pl_SIG;
    ENIX_DONNEES_ECL *        pl_ECL;
    ENIX_DONNEES_VEN *        pl_VEN;
    ENIX_DONNEES_ENR *        pl_ENR;
    ENIX_DONNEES_INC *        pl_INC;
    ENIX_DONNEES_EXP *        pl_EXP;
    ENIX_DONNEES_SYS *        pl_SYS;
    ENIX_DONNEES_LTN *        pl_LTN;
	int						 vl_commandeOK = XDC_NOK;
    int                      vl_commande_TWWI0,
                             vl_commande_TWWI1;

    XZST_03EcritureTrace(XZSTC_FONCTION, " IN  : et_cmd_domaine_cb ");
          
    /*A Recuperation des parametres du message XDM_ECMD_DOMAINE */
    if(!TipcMsgRead( pa_Data -> msg,
                     T_IPC_FT_INT4, &vl_Domaine,
                     T_IPC_FT_INT4, &vl_TypeOuvrage,
                     T_IPC_FT_INT4, &vl_IdOuvrage,
                     T_IPC_FT_INT4, &vl_Mode,
                     T_IPC_FT_INT4, &vl_NumAct,
                     NULL))
    {       
        /* Trace : Impossibilite de lire le message */
        XZST_03EcritureTrace ( XZSTC_WARNING, "Messsage XDM_ECMD_DOMAINE non lu ." );
        return;
    }
	vl_commande_TWWI0 = et_envoi_cmd_domaine ( vg_SockTWWI0,
	                                vl_TypeOuvrage,
	                                vl_IdOuvrage,
	                                vl_Domaine,
	                                vl_Mode,
	                                vl_NumAct);	

	vl_commande_TWWI1 = et_envoi_cmd_domaine ( vg_SockTWWI1,
	                                vl_TypeOuvrage,
	                                vl_IdOuvrage,
	                                vl_Domaine,
	                                vl_Mode,
	                                vl_NumAct);	
									
	if (vg_maitre_actif == XDC_MAITRE_TWWI0)
	{
		if (vl_commande_TWWI0 == XDC_OK)
		{
			vl_commandeOK = XDC_OK;
		}
		else
		{
			/*A Echec envoie commande */
			vl_commandeOK = XDC_NOK;
		}
	}
	else
	{
		if (vl_commande_TWWI1 == XDC_OK)
		{
			/*A Memorisation de la sequence en cours */
			vl_commandeOK = XDC_OK;
		}
		else
		{
			/*A Echec envoi commande */
			vl_commandeOK = XDC_NOK;
		}
	}
	
	if (vl_commandeOK == XDC_OK)
	{
		if(vl_Domaine == XDC_EQT_SIG)
		{
			for ( pl_SIG = pg_debutSIG; pl_SIG != NULL; pl_SIG = pl_SIG->Suivant )
			{
				if (pl_SIG->Numero == vl_IdOuvrage && pl_SIG->TypeOuvrage == vl_TypeOuvrage)
				{
					pl_SIG->ModeDomaine = vl_Domaine;
					break;
				}
			}
		} 
		else if(vl_Domaine == XDC_EQT_VEN)
		{
			for ( pl_VEN = pg_debutVEN; pl_VEN != NULL; pl_VEN = pl_VEN->Suivant )
			{
				if (pl_VEN->Numero == vl_IdOuvrage && pl_VEN->TypeOuvrage == vl_TypeOuvrage)
				{
					pl_VEN->ModeDomaine = vl_Domaine;
					break;
				}
			}
		}
		else if(vl_Domaine == XDC_EQT_ECL)
		{
			for ( pl_ECL = pg_debutECL; pl_ECL != NULL; pl_ECL = pl_ECL->Suivant )
			{
				if (pl_ECL->Numero == vl_IdOuvrage && pl_ECL->TypeOuvrage == vl_TypeOuvrage)
				{
					pl_ECL->ModeDomaine = vl_Domaine;
					break;
				}
			}
		}
		else if(vl_Domaine == XDC_EQT_ENR)
		{
			for ( pl_ENR = pg_debutENR; pl_ENR != NULL; pl_ENR = pl_ENR->Suivant )
			{
				if (pl_ENR->Numero == vl_IdOuvrage && pl_ENR->TypeOuvrage == vl_TypeOuvrage)
				{
					pl_ENR->ModeDomaine = vl_Domaine;
					break;
				}
			}
		}
		else if(vl_Domaine == XDC_EQT_EXP || vl_Domaine == XDC_EQT_LI)
		{
			for ( pl_EXP = pg_debutEXP; pl_EXP != NULL; pl_EXP = pl_EXP->Suivant )
			{
				if (pl_EXP->Numero == vl_IdOuvrage && pl_EXP->TypeOuvrage == vl_TypeOuvrage)
				{
					pl_EXP->ModeDomaine = vl_Domaine;
					break;
				}
			}
		}
		else if(vl_Domaine == XDC_EQT_LTN)
		{
			for ( pl_LTN = pg_debutLTN; pl_LTN != NULL; pl_LTN = pl_LTN->Suivant )
			{
				if (pl_LTN->Numero == vl_IdOuvrage && pl_LTN->TypeOuvrage == vl_TypeOuvrage)
				{
					pl_LTN->ModeDomaine = vl_Domaine;
					break;
				}
			}
		}
		else if(vl_Domaine == XDC_EQT_SYS)
		{
			for ( pl_SYS = pg_debutSYS; pl_SYS != NULL; pl_SYS = pl_SYS->Suivant )
			{
				if (pl_SYS->Numero == vl_IdOuvrage && pl_SYS->TypeOuvrage == vl_TypeOuvrage)
				{
					pl_SYS->ModeDomaine = vl_Domaine;
					break;
				}
			}
		}
	}
    XZST_03EcritureTrace(XZSTC_FONCTION, " OUT: et_cmd_domaine_cb ");
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande commande sequence echangeur 
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int    et_cmd_maitre ( int   va_socket)
 
/*
* ARGUMENTS EN ENTREE :
*
* ARGUMENTS EN SORTIE :
*   aucun
*
*
* CODE RETOUR : 
*   aucun
*
* CONDITION D'UTILISATION
*  Fonction appel�e lors de la mise a jour du maitre
*
* FONCTION 
* Envoyer a la tache maitre Wizcon ou esclave Wizcon la notion de maitre  
*
------------------------------------------------------*/
{
    static char       *version = "@(#)enix_cmd.c	1.6 06/16/09      : et_cmd_maitre " ;
    XZEXT_MSG_SOCKET  vl_Msg; 
    short             vl_PosMsg = 0;

    XZST_03EcritureTrace ( XZSTC_FONCTION, " IN  : et_cmd_maitre " );

    /*A Construction de la trame */
    vl_PosMsg = sprintf ( vl_Msg, "%d|1|%s", ENIX_CMD_MAITRE, XZEXC_FIN_MSG_SOCK_WIZCON );

    /*A Envoi de la trame */
    fcntl ( va_socket, F_SETFL, O_NDELAY | fcntl ( va_socket, F_GETFL, 0 ) );
    if ( send ( va_socket , vl_Msg , vl_PosMsg , 0 ) <= 0 )                                   
    {
    
        /* Ecriture dans le fichier trace en cas d'�chec */
        XZST_03EcritureTrace ( XZSTC_WARNING, "Impossible d'�mettre la trame <%s> vers maitre Wizcon ou esclave Wizcon.",
                               vl_Msg );
        return(XDC_NOK);

    }

    XZST_03EcritureTrace ( XZSTC_FONCTION, " OUT  : et_cmd_maitre " );
    return(XDC_OK);

}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande commande est vivant 
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int    et_cmd_alive ( int    va_socket)
 
/*
* ARGUMENTS EN ENTREE :
*
* ARGUMENTS EN SORTIE :
*   aucun
*
*
* CODE RETOUR : 
*   aucun
*
* CONDITION D'UTILISATION
*  Fonction appel�e pour verifier la communication 
*
* FONCTION 
* Envoyer a la tache maitre Wizcon ou esclave Wizcon la notion de maitre  
*
------------------------------------------------------*/
{
    static char         *version = "@(#)enix_cmd.c	1.6 06/16/09      : et_cmd_alive " ;

    XZEXT_MSG_SOCKET    vl_Msg; 
    short               vl_PosMsg = 0;

    XZST_03EcritureTrace ( XZSTC_FONCTION, " IN  : et_cmd_alive " );
    
    /*A Construction de la trame */
    vl_PosMsg = sprintf ( vl_Msg, "%d|1|%s", ENIX_CMD_IS_ALIVE, XZEXC_FIN_MSG_SOCK_WIZCON );
    
    /*A Envoi de la trame */
    fcntl ( va_socket, F_SETFL, O_NDELAY | fcntl ( va_socket, F_GETFL, 0 ) );
    if ( send ( va_socket , vl_Msg , vl_PosMsg , 0 ) <= 0 )                                   
    {

        /* Ecriture dans le fichier trace en cas d'�chec */
        XZST_03EcritureTrace( XZSTC_WARNING, "Impossible d'�mettre la trame <%s> vers maitre Wizcon ou esclave Wizcon.", vl_Msg );
        return(XDC_NOK);
    }

    XZST_03EcritureTrace ( XZSTC_FONCTION, " OUT  : et_cmd_alive " );
    return(XDC_OK);

}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande commande synchronisation
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int    et_cmd_synchro ( int   va_socket)

/*
* ARGUMENTS EN ENTREE :
*
* ARGUMENTS EN SORTIE :
*   aucun
*
*
* CODE RETOUR : 
*   aucun
*
* CONDITION D'UTILISATION
*  Fonction appel�e pour verifier la communication 
*
* FONCTION 
* Envoyer a la tache maitre Wizcon ou esclave Wizcon la notion de maitre  
*
------------------------------------------------------*/
{
    static char       *version = "@(#)enix_cmd.c	1.6 06/16/09      : et_cmd_synchro " ;

    XZEXT_MSG_SOCKET  vl_Msg; 
    short             vl_PosMsg = 0;
    double            vl_HorodateSec;
    struct tm *       pl_InfoTemps ;
    time_t            vl_Heure;
    int               vl_JourSem;
    char *tz;

    XZST_03EcritureTrace(XZSTC_FONCTION, " IN  : et_cmd_synchro ");

    /* Utilisation de la fonction RTWorks : TutGetWallTime */
    // vl_HorodateSec = TutGetWallTime() ;
    // reccupere la date courrante en Nb de seconde "TZ"
    XZSM_07LireHorodate( XZSMC_TYPE_SYSTEME, &vl_JourSem, &vl_HorodateSec );    

    //XZSM_07LireHorodate ( XZSMC_TYPE_SYSTEME,
                         // &vl_JourSem,
                          //&vl_HorodateSec );

    // transforme le nombre de seconde TZ   en structure tm (char) 
   // ici la fonction XZSM_11 ne peut pas etre utilisee car le formatage necessaire 
   // n'est pas celui retourne par celle-ci.
    vl_Heure = (const time_t) (vl_HorodateSec);
   tz = getenv("TZ");
	setenv("TZ", "", 1);
	tzset();
	pl_InfoTemps    = localtime(&vl_Heure) ;
	if (tz)
	    setenv("TZ", tz, 1);
	else
	    unsetenv("TZ");
	tzset();


	pl_InfoTemps->tm_mon = pl_InfoTemps->tm_mon + 1;

    /*A Construction de la trame */
    vl_PosMsg = sprintf ( vl_Msg, "%d|%04d~%02d~%02d~%02d~%02d~%02d|%s", 
                          ENIX_CMD_SYNCHRO, 
                          pl_InfoTemps->tm_year+1900,
                          pl_InfoTemps->tm_mon,
                          pl_InfoTemps->tm_mday,
                          pl_InfoTemps->tm_hour,
                          pl_InfoTemps->tm_min,
                          pl_InfoTemps->tm_sec,
                          XZEXC_FIN_MSG_SOCK_WIZCON );

    XZST_03EcritureTrace ( XZSTC_WARNING, "Synchronisation GTC <%s>.",
                           vl_Msg );

    /*A Envoi de la trame */
    fcntl ( va_socket, F_SETFL, O_NDELAY | fcntl ( va_socket, F_GETFL, 0 ) );
    if ( send ( va_socket , vl_Msg , vl_PosMsg , 0 ) <= 0 )                                   
    {

        /* Ecriture dans le fichier trace en cas d'�chec */
        XZST_03EcritureTrace ( XZSTC_WARNING, "Impossible d'�mettre la trame <%s> vers maitre Wizcon ou esclave Wizcon.",
                               vl_Msg );
        return(XDC_NOK);

    }

    XZST_03EcritureTrace ( XZSTC_FONCTION, " OUT  : et_cmd_synchro " );

    return(XDC_OK);    
}
