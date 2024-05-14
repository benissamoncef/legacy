/* Fichier : $Id: shor_tra.c,v 1.18 2019/06/04 13:54:25 devgtie Exp $      Release : $Revision: 1.18 $        Date : $Date: 2019/06/04 13:54:25 $
------------------------------------------------------
 * STERIA *  PROJET MIGRAZUR
------------------------------------------------------
 * SOUS-SYSTEME  SUPERV
------------------------------------------------------
 * MODULE SHOR_TRA * FICHIER shor_tra.c
------------------------------------------------------
 * DESCRIPTION DU MODULE :
 *
 * Module permettant de traiter les demandes d'actualisation
 * de l'horodate et de l'heure systeme.
 *
------------------------------------------------------
 * HISTORIQUE :
 *
 * Milleville.T	10 Aug 1994	: Creation
 * Milleville.T	03 Mar 1995 	: Modification du Datagroup pour
 *			envoyer l'heure V1.3
 * Milleville.T	09 Mar 1995 	: Modification de la fonction d'actualisation
 *		de l'heure. Si mauvaise lecture alors on envoie un msg d'alarme
 *		a l'administarteur Systeme V1.4
 *T.Milleville   19 Jul 1995	: Ajout de msg de traces et de commentaires V1.5
 *T.Milleville   25 Jul 1995	: Ajout de commentaires V1.6
 *T.Milleville   6 Aou  1995	: Modification test pour envoi de l'heure
 *	systeme. Si etat Degrade, alors on envoie V1.7
 ***
 *P.Niepceron 	22 Aou 1996	: Il n'y a pas d'horologe dans les PC simplifie, donc le CI les synchronise v1.8
 *P.Niepceron 	16 Dec 1997	: Il n'y a pas d'horologe dans les PC2 , donc le CI les synchronise v1.9 dem/1533
 *P.Niepceron    15 Mar 1999	: DA et DS ont une horloge v1.10
 * JMG		10 oct 2002	: le CI synchronise TOUT v1.11
 *L.Claudel      09 Jul 2007     : Suppression DY v1.12
 * JMG           02/10/09        : secto DEM887 1.13
 * JMG     10/02/10        : portage Linux DEM/934
 * PFE     03/06/16        : le CI synchronise TOUT meme NICE
 * PNI/RGR 10/05/19        : Modif bug datation                            1.18
 * LCL		20/12/21	: MOVIS P2 ems SAE-312
 * LCL		27/09/23 : Adaptations CNA DEM-483	
------------------------------------------------------*/

/* fichiers inclus */

#include        <stdio.h>

#include        <xdc.h>
#include        <xdl.h>
#include        <xzsc.h>
#include        <xzsm.h>
#include        <xzeh.h>

#include        <rtworks/common.h>
#include        <rtworks/ipc.h>

#ifdef _HPUX_SOURCE
#include        <rtworks/ipc_proto.h>
#endif

#include "shor_tra.h"


/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

/* declaration de fonctions internes */

/* definition de fonctions externes */

extern void sh_actualiserhoro_cb(T_IPC_CONN va_Cnx,
    T_IPC_CONN_PROCESS_CB_DATA pa_Data,
    T_CB_ARG pa_ARG);

extern void sh_synchroniser_cb(T_IPC_CONN va_Cnx,
    T_IPC_CONN_PROCESS_CB_DATA pa_Data,
    T_CB_ARG pa_ARG);

extern void sh_actualisersyste_cb(T_IPC_CONN va_Cnx,
    T_IPC_CONN_PROCESS_CB_DATA pa_Data,
    T_CB_ARG pa_ARG);


/*
------------------------------------------------------
 * SERVICE RENDU :
 *  Permet d'actualiser l'horodate et de diffuser le
 *	message MT_TIME pour mise a jour sur les
 *	diverses smachines
------------------------------------------------------
 * SEQUENCE D'APPEL :
 */

void sh_actualiserhoro_cb(T_IPC_CONN va_Cnx,
    T_IPC_CONN_PROCESS_CB_DATA pa_Data,
    T_CB_ARG pa_ARG)

/*
 * ARGUMENTS EN ENTREE :
 *   Arguments d'une callback
 *
 * ARGUMENTS EN SORTIE :
 *   Aucun
 *
 *
 * CODE RETOUR :
 *   Aucun
 *
 * CONDITION D'UTILISATION
 *   Fonction declenche sur la reception du message
 *		XDM_TSH_HORO
 *
 * FONCTION
 *   Recupere l'horodate dans le message.
 *   Suivant l'etat du calculateur , envoie le message T_MT_TIME
 *   puis actualise l'horodate;
 *
------------------------------------------------------*/
{
#ifndef _TIBCO_EMS
  T_REAL8 vl_ValHorodate 	= 0.0;
#else
  XDY_Horodate vl_ValHorodate 	= 0.0;
#endif
  int vl_EtatMachine 	= 0;
  int pl_TypeMachine = 0;
  T_IPC_MT vl_Msg = 0;
  int			vl_ValRet	= XDC_OK;
  XZSCT_Datagroup         vl_Datagroup    = "";
  XZSCT_NomSite         	vl_NomSite    = "";


  /* Recuperation de l'horodate dans le message */
  if (TipcMsgRead(pa_Data->msg,T_IPC_FT_REAL8,&vl_ValHorodate,NULL) )
  {

    /* Actualiser l'horodate */
    XZST_03EcritureTrace(XZSTC_WARNING, "sh_actualiserhoro_cb : XZSM_10ActualiserDeltaHorodate");
    if ( (vl_ValRet = XZSM_10ActualiserDeltaHorodate((double)vl_ValHorodate)) != XDC_OK )
    {
      /* Affichage trace */
      XZST_03EcritureTrace(XZSTC_WARNING, "XZSM_10ActualiserDeltaHorodate invalide " );

    }
  }
  else
  {
    XZST_03EcritureTrace(XZSTC_WARNING, "XZSM_TraiterTime:TipcMsgRead") ;
  }

  /*  Recuperation du Nom du Site */
  XZSC_04NomSite(vl_NomSite);

  /* Recherche de l'etat de la machine */
  XZSE_09EtatMachine(&vl_EtatMachine);

  XZSC_05TypeMachine(&pl_TypeMachine);

  /* Diffusion de l'heure uniquement par SDCI1 */
  if ( (pl_TypeMachine == XZSCC_TYPEM_SD))
  {

    /* si le calculateur est actif ou degrade  */
    if ( (vl_EtatMachine == XZSEC_ETAT_NACTIF) || (vl_EtatMachine == XZSEC_ETAT_DEGRADE) )
    {
      /* creation du message */
      if ((vl_Msg = XDM_IdentMsg ( T_MT_TIME )) == NULL )
      {
        XZST_03EcritureTrace(XZSTC_WARNING, "XZSM_ActualiserHorodate:Erreur IdentMsg(T_MT_TIME)");
      }

      XZST_03EcritureTrace(XZSTC_WARNING, "sh_actualiserhoro_cb : Emission du message time");

      XDG_EncodeDG2 (vl_Datagroup, XDG_TIME, vl_NomSite);

      /* Emission du message time */
      if (!TipcSrvMsgWrite(vl_Datagroup, vl_Msg, XDM_FLG_SRVMSGWRITE, T_IPC_FT_REAL8, vl_ValHorodate, NULL ))
      {
        XZST_03EcritureTrace(XZSTC_WARNING, "XZSM_ActualiserHorodate:Erreur TipcSrvMsgWrite(vl_Datagroup,XDM_FLG_SRVMSGWRITE,,%f)", vl_ValHorodate);
      }

    }
  }

  /* Heure recue */
  XZSMV_TimeRecu = 1 ;

  XZST_03EcritureTrace(XZSTC_FONCTION, " sh_actualiserhoro_cb : OUT \n");

}


/*
 * ------------------------------------------------------
 * SERVICE RENDU :
 *  Permet de recuperer la demande de synchronisation d'une tache
 *	pour lui indiquer le mode utilisee dans la diffusion
 *	de l'heure
------------------------------------------------------
 * SEQUENCE D'APPEL :
 */

void sh_synchroniser_cb(T_IPC_CONN va_Cnx,
    T_IPC_CONN_PROCESS_CB_DATA pa_Data,
    T_CB_ARG pa_ARG)

/*
 * ARGUMENTS EN ENTREE :
 *   Argument d'une callback
 *
 *
 * ARGUMENTS EN SORTIE :
 *   Aucun
 *
 *
 * CODE RETOUR :
 *   Aucun
 *
 * CONDITION D'UTILISATION
 *   Declenchee sur la reception du message XDM_TSH_SYNCH
 *
 * FONCTION
 *   Recupere le nom de la machine et de la tache dans le message.
 *   Preparation du message XDM_TSH_MODE(XZSM_09) et envoie du message
 *   Suivant le mode preparation message T_MT_TIME et envoie.
 *
------------------------------------------------------*/
{

  char *                  pl_NomTache 		= NULL;
  char *                  pl_NomMachine 		= NULL;
  XZSCT_Datagroup         vl_Dg 			= "";
  int                     vl_Jour_De_La_Semaine 	= 0;
  double                  vl_Horodate 		= 0.0;
  int			vl_ModeEmis		= 0;
  int			vl_ValRet		= XDC_OK;
  int pl_TypeMachine = 0;


  /* si la recuperation du nom machine et du nom de la tache s'effectue sans probleme */
  if (TipcMsgRead(pa_Data->msg,
      T_IPC_FT_STR,&pl_NomTache,
      T_IPC_FT_STR,&pl_NomMachine,
      NULL) )
  {

    /* Recuperation du mode d'emission */
    vl_ValRet = XZSM_09ModeHorodate(XZSMC_LECTURE,&vl_ModeEmis);

    /* construction du datagroup cible */
    XDG_EncodeDG2 (vl_Dg, pl_NomMachine,pl_NomTache) ;

    /* envoie du message XDM_TSH_MODE */
    if (!TipcSrvMsgWrite(vl_Dg,
        XDM_IdentMsg(XDM_TSH_MODE),
        XDM_FLG_SRVMSGWRITE,
        T_IPC_FT_INT4,
        vl_ModeEmis,
        NULL) )
    {
      XZST_03EcritureTrace(XZSTC_WARNING, "XZSM_Synchronisation:TipcSrvMsgWrite(%s,%u,XDM_FLG_SRVMSGWRITE,,%u)", vl_Dg ,XDM_IdentMsg(XDM_TSH_MODE) ,XZSMC_SYSTE) ;
    }

    XZST_03EcritureTrace(XZSTC_DEBUG1, "XZSM_Synchronisation:  Test si envoi de T_MT_TIME Variable Tiem recu %d -- Mode emission %d \n",
        XZSMV_TimeRecu,vl_ModeEmis);

    /* si l'heure a ete recue auparavant */
    if ( (XZSMV_TimeRecu) && ( (vl_ModeEmis == XZSMC_NSUPV) || (vl_ModeEmis == XZSMC_SYSTE) ) )
    {

      /* connaissance de l'heure */
      if (XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME, &vl_Jour_De_La_Semaine, &vl_Horodate) == XZSMC_OK)
      {

        XZSC_05TypeMachine(&pl_TypeMachine);

        /* Diffusion de l'heure uniquement par SDCI1 */
        if ( (pl_TypeMachine == XZSCC_TYPEM_SD))
        {

          XZST_03EcritureTrace(XZSTC_WARNING, "sh_synchroniser_cb: Emission du message time");

          /* emission de l'horodate */
          if (!TipcSrvMsgWrite(vl_Dg,
              XDM_IdentMsg(T_MT_TIME),
              XDM_FLG_SRVMSGWRITE,
              T_IPC_FT_REAL8,
              vl_Horodate,
              NULL))
          {
            XZST_03EcritureTrace(XZSTC_WARNING, "TipcSrvMsgWrite(%s,%u,XDM_FLG_SRVMSGWRITE,%f)",
                vl_Dg ,XDM_IdentMsg(T_MT_TIME),vl_Horodate);
          }
        }

      }
      else
      {
        XZST_03EcritureTrace(XZSTC_WARNING, "XZSM_Synchronisation:XZSM_07");
      }

    }

  }

}


/*
------------------------------------------------------
 * SERVICE RENDU :
 *  Permet cycliquement de lire la centrale horaire et ensuite de
 *	diffuser l'heure aux machines du site et de memoriser l'heure
 *	systeme
------------------------------------------------------
 * SEQUENCE D'APPEL :
 */

void sh_actualisersyste_cb(T_IPC_CONN va_Cnx,
    T_IPC_CONN_PROCESS_CB_DATA pa_Data,
    T_CB_ARG pa_ARG)

/*
 * ARGUMENTS EN ENTREE :
 *   Argument d'une Callback
 *
 *
 * ARGUMENTS EN SORTIE :
 *   Aucun
 *
 *
 * CODE RETOUR :
 *   Aucun
 *
 * CONDITION D'UTILISATION
 *   Declenchee sur reception du message XDM_TSH_SYST
 *
 * FONCTION
 *   Lecture de la centrale horaire (XZEH_02)
 *   Suivant l'etat du calculateur, preparation du message
 *   T_MT_TIME et envoie du message.
 *   Actualiser l'horodate.
 *
------------------------------------------------------*/
{

  int             vl_ValRet 		= XDC_OK;
  char  		vl_LecHorodate[20]	= "";
  double 		vl_DoubleHorodate	= 0.0;
  int             vl_EtatMachine 		= 0;
  T_IPC_MT        vl_Msg 			= 0;
  XZSCT_Datagroup	vl_Datagroup		= "";
  XZSCT_NomSite	vl_NomSite		= "";
  int     	vl_JourSemaine		= 0;
  double  	vl_Horodate		= 0.0;
  int                  	pl_TypeMachine	= 0;


  /*  Recuperation du Nom du Site */
  XZSC_04NomSite(vl_NomSite);

  /* Recuperation du type de la machine */
  if ( (XZSC_05TypeMachine(&pl_TypeMachine)) != XDC_OK)
  {
    XZST_03EcritureTrace( XZSTC_WARNING, "Recuperation du Type de la machine impossible; \n ");
  }

  if ((pl_TypeMachine != XZSCC_TYPEM_SD))
  {
    XZST_03EcritureTrace(XZSTC_WARNING ,"XZSM_07LireHorodate : site=%s on n est pas SDCI1 on lit seulement l'heure systeme",vl_NomSite);
    vl_ValRet = XZSM_07LireHorodate( XZSMC_TYPE_SYSTEME, &vl_JourSemaine, &vl_Horodate);
    sprintf(vl_LecHorodate,"%lf",vl_Horodate);
  }
  else
  {

    XZST_03EcritureTrace(XZSTC_WARNING ,"on est SDCI1 :::::: DANS LE ELSE XZSEH02_Lecture_Horloge : site=%s on lit l'horloge GORGY TIMING  *******",vl_NomSite);
    /* Lecture de la centrale horaire */
    /*	if ( (vl_ValRet = XZEH02_Lecture_Horloge(vl_LecHorodate) ) != XDC_OK )
	{
		 XZST_03EcritureTrace(XZSTC_WARNING ,
                                        "XZSEH02_Lecture_Horloge") ;	
		XZST_10ArchiverMsgSyst(XDL_SUP64_HORNOK,vl_NomSite,NULL);
     */
    /* Recuperation de l'heure courante du Serveur */
    vl_ValRet = XZSM_07LireHorodate( XZSMC_TYPE_SYSTEME, &vl_JourSemaine, &vl_Horodate);
    sprintf(vl_LecHorodate,"%lf",vl_Horodate);
    /*	}*/
  }

  if ((vl_ValRet == XDC_OK) && (pl_TypeMachine != XZSCC_TYPEM_SD))
  {
    /*  Transformation de l'horodate chaine en reel */
    vl_DoubleHorodate = atof(vl_LecHorodate);

    /* Recherche de l'etat de la machine */
    XZSE_09EtatMachine(&vl_EtatMachine) ;

    /*B si le calculateur est actif  ou degrade *
		if ( (vl_EtatMachine == XZSEC_ETAT_NACTIF) ||
			(vl_EtatMachine == XZSEC_ETAT_DEGRADE) ) * if 1 *
		{*/
    /* creation du message */
    if ((vl_Msg = XDM_IdentMsg ( T_MT_TIME )) == NULL )
    {
      XZST_03EcritureTrace(XZSTC_WARNING, "XZSM_ActualiserHorodate:Erreur IdentMsg(T_MT_TIME)" );
    }

    /* Diffusion de l'heure uniquement par SDCI1 */
    if ( (pl_TypeMachine == XZSCC_TYPEM_SD))
    {

      XZST_03EcritureTrace(XZSTC_WARNING, "sh_actualisersyste_cb : Emission du message time");

      XDG_EncodeDG2 (vl_Datagroup, XDG_TIME, vl_NomSite);

      /* Emission du message time */
      if (!TipcSrvMsgWrite(vl_Datagroup, vl_Msg,XDM_FLG_SRVMSGWRITE, T_IPC_FT_REAL8, vl_DoubleHorodate, NULL ))
      {
        XZST_03EcritureTrace(XZSTC_WARNING, "XZSM_ActualiserHorodate:Erreur TipcSrvMsgWrite(vl_Datagroup,XDM_FLG_SRVMSGWRITE,%f)",
            vl_DoubleHorodate );
      }

      if ( !strcmp( vl_NomSite , XDC_NOM_SITE_CI ))
      {

        XZST_03EcritureTrace(XZSTC_WARNING, "sh_actualisersyste_cb : Emission du message time sur VC");

        /* Envoi de l'heure sur VE */
        XDG_EncodeDG2 (vl_Datagroup, XDG_TIME, XDC_NOM_SITE_VC);

        /* Emission du message time */
        if (!TipcSrvMsgWrite(vl_Datagroup, vl_Msg, XDM_FLG_SRVMSGWRITE, T_IPC_FT_REAL8, vl_DoubleHorodate, NULL ))
        {
          XZST_03EcritureTrace(XZSTC_WARNING, "XZSM_ActualiserHorodate:PB SUR PEYRUIS : \
              Erreur TipcSrvMsgWrite(vl_Datagroup,XDM_FLG_SRVMSGWRITE,%f)", vl_DoubleHorodate );
        }


        XZST_03EcritureTrace(XZSTC_WARNING, "sh_actualisersyste_cb : Emission du message time sur CA");

        /* Envoi de l'heure sur DP */
        XDG_EncodeDG2 (vl_Datagroup, XDG_TIME, XDC_NOM_SITE_CA);

        /* Emission du message time */
        if (!TipcSrvMsgWrite(vl_Datagroup, vl_Msg, XDM_FLG_SRVMSGWRITE, T_IPC_FT_REAL8, vl_DoubleHorodate, NULL ))
        {
          XZST_03EcritureTrace(XZSTC_WARNING, "XZSM_ActualiserHorodate:PB sur CA : \
              Erreur TipcSrvMsgWrite(vl_Datagroup,XDM_FLG_SRVMSGWRITE,%f)" ,vl_DoubleHorodate );
        }

        XZST_03EcritureTrace(XZSTC_WARNING, "sh_actualisersyste_cb : Emission du message time sur DP");

        /* Envoi de l'heure sur DP */
        XDG_EncodeDG2 (vl_Datagroup, XDG_TIME, XDC_NOM_SITE_DP);

        /* Emission du message time */
        if (!TipcSrvMsgWrite(vl_Datagroup, vl_Msg, XDM_FLG_SRVMSGWRITE, T_IPC_FT_REAL8, vl_DoubleHorodate, NULL ))
        {
          XZST_03EcritureTrace(XZSTC_WARNING, "XZSM_ActualiserHorodate:PB sur DP : \
              Erreur TipcSrvMsgWrite(vl_Datagroup,XDM_FLG_SRVMSGWRITE,%f)", vl_DoubleHorodate );
        }

        XZST_03EcritureTrace(XZSTC_WARNING, "sh_actualisersyste_cb : Emission du message time sur VC");

        /* Envoi de l'heure sur VC */
        XDG_EncodeDG2 (vl_Datagroup, XDG_TIME, XDC_NOM_SITE_VC);

        /* Emission du message time */
        if (!TipcSrvMsgWrite(vl_Datagroup, vl_Msg, XDM_FLG_SRVMSGWRITE, T_IPC_FT_REAL8, vl_DoubleHorodate, NULL ))
        {
          XZST_03EcritureTrace(XZSTC_WARNING, "XZSM_ActualiserHorodate:PB sur VC : \
              Erreur TipcSrvMsgWrite(vl_Datagroup,XDM_FLG_SRVMSGWRITE,%f)", vl_DoubleHorodate );
        }

      }

    }

    /* Actualiser l'horodate */
    XZST_03EcritureTrace(XZSTC_WARNING, "sh_actualisersyste_cb : XZSM_10ActualiserDeltaHorodate");
    if ( (vl_ValRet = XZSM_10ActualiserDeltaHorodate((double)vl_DoubleHorodate)) != XDC_OK )
    {
      XZST_03EcritureTrace(XZSTC_WARNING, "XZSM_10ActualiserDeltaHorodate invalide");
    }

    /* Heure recue */
    XZSMV_TimeRecu = 1 ;
  }

}
