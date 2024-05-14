/*E*/
/* Fichier : $Id: evid_cmd.c,v 1.32 2020/11/03 18:12:41 pc2dpdy Exp $        Release : $Revision: 1.32 $        Date : $Date: 2020/11/03 18:12:41 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEVID * FICHIER evid_cmd.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* C'est le fichier des callbacks de la tache VIDEO
*
------------------------------------------------------
* HISTORIQUE :
*
* Nagiel.E	version 1.1	02 Sep 1994	: Creation
* Nagiel.E	version 1.2	01 Dec 1994	:
* Nagiel.e	version 1.3	23 Dec 1994	:
* Nagiel.E	version 1.4	26 Dec 1994	:
* Mismer.D	version 1.5	17 Jan 1995	:
* Volcic.F	version 1.6	24 Jan 1995	:
* Volcic.F	version 1.7	13 Fev 1995	:
* Mismer.D	version 1.8	09 Avr 1995	:
* Mismer.D	version 1.9	11 Avr 1995	:
* Mismer.D	version 1.10	14 Avr 1995	:
* Mismer.D	version 1.11	21 Avr 1995	:
* Mismer.D	version 1.12	21 Avr 1995	:
* Volcic.F	version 1.13	02 Mai 1995	: Modif retour des etats a l'init IHM
* Mismer.D	version 1.14	18 Mai 1995	: Modif gestion compte rendu commande, reprise historique
* Volcic.F	version 1.15	29 Jun 1995	: Modif pour magnetos
* Mismer.D	version 1.16	28 Sep 1995	: Ajout dernier argument NULL pour XZST_10
* Mismer.D	version 1.17	24 Avr 1997	: Commande magneto EQT2 (DEM/1427)
* Mismer.D	version 1.18	27 Mai 1997	: Commande magneto EQT2 Suite (DEM/1427)
* Mismer.D	version 1.19	28 Mai 1997	: Commande magneto EQT2 Suite (DEM/1427)
* Mismer.D	version 1.20	18 Jan 1998	: Modif pour basculement PC2
* Mismer.D	version 1.21	08 Jan 1999	: Modif pour Trace cmd Magnetoscope (DEM/APG16) Fiche ANA75
* Mismer.D	version 1.22	29 Mai 2000	: Modif pour basculement PC2 Suite et fin (DEM/65)
* Cluchague     version 1.23	25 Nov 2003	: Traces plus explicites 
* Hilmarcher    version 1.24    24/04/2004      : Modif pour pilotage cam pc niv 2 DEM 270 
* Niepceron	version 1.25	24/11/2005	: Les codes moniteurs 99 correspondent au code pour l'envoi des images au CI. DEM479
* Niepceron	version 1.26	15/05/2008	: Modif traces suite a la DEM775
* JMG		version 1.27	02/10/09	: secto dem887
* JPL		25/02/11 : Migration architecture HP ia64 (DEM 975) : retour C.R. OK pour fonction 'ev_ecriture_trace_cmd'  1.28
* PNI		18/05/11 : Prise en compte de no moniteur � 3 chiffres 1.29
* JPL		27/10/16 : Commande unique affectation/position; commande LCR ineffective (DEM 1189)  1.30
* JPL		02/03/17 : Attente du code camera et non leur numero SAE en arguments de commandes  1.31
* JMG		15/05/20 : EMS 1.32
* GR            10/07/20 : EMS 1.33
* LCL		20/12/21	: MOVIS P2 ems SAE-312
------------------------------------------------------*/

/* fichiers inclus */

#include "evid_com.h"

#include "evid_cmd.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: evid_cmd.c,v 1.32 2020/11/03 18:12:41 pc2dpdy Exp $ : evid_cmd" ;

/* declaration de fonctions internes */
int	ev_sysv_commande ( int, char *, int);

/* definition de fonctions externes */



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  commande de l'eqt en fonction de la demande.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	ev_cmd_VIDEO_cb ( 
        T_IPC_CONN 			va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG                        pa_ARG)
 
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
*  XDM_ECMD_VIDEO
*
* FONCTION 
*  Suivant la commande -> envoi de la commande correspondante
*
------------------------------------------------------*/
{
   static char *version = "$Id: evid_cmd.c,v 1.32 2020/11/03 18:12:41 pc2dpdy Exp $ : ev_cmd_VIDEO_cb" ;

#ifndef _TIBCO_EMS   
   T_STR			vl_NomPO;
   T_INT4			vl_codecmd;
   T_INT4			vl_noimage;
   T_INT4			vl_nomoniteur;
   T_INT4			vl_noposition;
   T_INT4			vl_nojoystick;
   T_INT4			vl_nomagneto;
   T_INT4			vl_nocanal;
   T_INT4			vl_noltvideo;
#else
   XDY_NomEqt                   vl_NomPO;
   XDY_Entier			vl_codecmd;
   XDY_Entier			vl_noimage;
   XDY_Entier			vl_nomoniteur;
   XDY_Entier			vl_noposition;
   XDY_Entier			vl_nojoystick;
   XDY_Entier			vl_nomagneto;
   XDY_Entier			vl_nocanal;
   XDY_Entier			vl_noltvideo;
#endif
   
   EVID_DONNEES_CAMERA *	pl_ListeCamera = NULL, *pl_ListeCam = NULL;

   XZEXT_Trame			vl_trame = "";
   short			vl_on_arrete = XDC_FAUX;
   int				vl_cr = 0;

   int				vl_NumMoniteur  = 0;
   int				vl_CodeCamera   = 0,
   				vl_CodeMoniteur = 0,
   				vl_NumCamera    = 0,
   				vl_NumMagneto   = 0;
#ifndef _TIBCO_EMS
   T_STR			pl_Emetteur;
   char				vl_PosteCmd[10];
   
   TipcMsgGetSender(pa_Data -> msg, &pl_Emetteur);
   
   memcpy ( vl_PosteCmd, pl_Emetteur+1, 5 );	
   vl_PosteCmd[5] = '\0';
   XZST_03EcritureTrace(XZSTC_FONCTION, "IN: ev_cmd_VIDEO_cb emetteur:<%s>",vl_PosteCmd);

#endif

   /*A Recuperation des parametres du message XDM_ECMD_VIDEO */
   if(!TipcMsgRead(pa_Data -> msg
 			  ,T_IPC_FT_INT4,&vl_codecmd
 			  ,T_IPC_FT_INT4,&vl_noimage
 			  ,T_IPC_FT_INT4,&vl_nomoniteur
 			  ,T_IPC_FT_INT4,&vl_noposition
 			  ,T_IPC_FT_INT4,&vl_nojoystick
 			  ,T_IPC_FT_INT4,&vl_nomagneto
 			  ,T_IPC_FT_INT4,&vl_nocanal
 			  ,T_IPC_FT_INT4,&vl_noltvideo
 			  ,T_IPC_FT_STR, &vl_NomPO
	       		  ,NULL))
   {       
      XZST_03EcritureTrace( XZSTC_WARNING, "messsage XDM_ECMD_VIDEO non lu." );
      XZST_10ArchiverMsgSyst(0,"ev_cmd_VIDEO_cb : Message XDM_ECMD_VIDEO recu avec parametre(s) invalide(s)", NULL);
      XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: ev_cmd_VIDEO_cb ");
      return;
   }
   
   /*A Controle validite des elements du messge */

   XZST_03EcritureTrace(XZSTC_DEBUG1, "%s : CodeCmd = %d\tNo Im. = %d\tMon. = %d\tPos. = %d\tJoy = %d\tMag; = %d\tCan; = %d\tLTV = %d\tArret = %d po=%s", 
                   version, 
                   vl_codecmd,
		   vl_noimage,
                   vl_nomoniteur,
                   vl_noposition,
                   vl_nojoystick,
                   vl_nomagneto,
                   vl_nocanal,
                   vl_noltvideo,
                   vl_on_arrete,
                   vl_NomPO);
      
   /* verif du code commande */
   if ( (vl_codecmd != XDC_AFFECT_IMAG_JOYSTICK) && (vl_codecmd != XDC_CMD_POS) )
   {
      XZST_03EcritureTrace(XZSTC_WARNING," Commande video : No de commande Invalide : %d",vl_codecmd );
      vl_on_arrete = XDC_VRAI;
   }


   /* verif du no de camera */
   vl_CodeCamera = vl_noimage;

//   if (0 != vl_noimage)
//   {
//      for ( pl_ListeCamera = pg_debutCamera; pl_ListeCamera != NULL ; pl_ListeCamera = pl_ListeCamera->Suivant )
//      {
//         if ( pl_ListeCamera->Config.Numero == vl_noimage ) 
//         {
//            vl_CodeCamera = pl_ListeCamera->Config.Code;
//            vl_NumCamera  = pl_ListeCamera->Config.Numero;
//            XZST_03EcritureTrace(XZSTC_INFO," camera ok: %d (%d,%d)",vl_noimage, pl_ListeCamera->Config.Code,pl_ListeCamera->Config.Numero );
//	    break;
//         }
//      }
//      if ( pl_ListeCamera == NULL )
//      {
//         XZST_03EcritureTrace(XZSTC_WARNING," Commande video : No de camera Invalide %d",vl_noimage );
//         vl_on_arrete = XDC_VRAI;
//      }   
//   }


   /* verif du no de moniteur */
   vl_CodeMoniteur = vl_nomoniteur;

   /*vl_NomPO = "POCI1";*/
//   if (0 != vl_nomoniteur)
//   { 
//      for ( pl_ListeMoniteur = pg_debutMoniteur; pl_ListeMoniteur != NULL ; pl_ListeMoniteur = pl_ListeMoniteur->Suivant )
//      {
//	 
//	 if ( !strcmp ( pl_ListeMoniteur->Config.PO , vl_NomPO ) && (++vl_NumMoniteur == vl_nomoniteur))
//         {
//            vl_CodeMoniteur = pl_ListeMoniteur->Config.Code;
//            if ((vl_CodeMoniteur==99) && (vl_nomoniteur == 1)) {vl_CodeMoniteur = 301;}
//	    if ((vl_CodeMoniteur==99) && (vl_nomoniteur == 2)) {vl_CodeMoniteur = 302;}
//	    if ((vl_CodeMoniteur==99) && (vl_nomoniteur == 3)) {vl_CodeMoniteur = 303;}
//	    if ((vl_CodeMoniteur==99) && (vl_nomoniteur == 4)) {vl_CodeMoniteur = 304;}
//	    if ((vl_CodeMoniteur==99) && (vl_nomoniteur == 7)) {vl_CodeMoniteur = 305;}
//	    if ((vl_CodeMoniteur==99) && (vl_nomoniteur == 8)) {vl_CodeMoniteur = 306;}
//	    XZST_03EcritureTrace(XZSTC_WARNING," moniteur ok : %d", vl_nomoniteur );
//	    break;
//         }
//      }
//      if ( pl_ListeMoniteur == NULL )
//      {
//         XZST_03EcritureTrace(XZSTC_WARNING," verif code commande  : No de moniteur Invalide %d ",vl_nomoniteur );
//         vl_on_arrete = XDC_VRAI;
//      }   
//   }


   /*A Si parametre invalide -> arret */
   if (vl_on_arrete)
   {
      /* message administrateur systeme */
      XZST_10ArchiverMsgSyst(0,"ed_cmd_VIDEO_cb : Message XDM_ECMD_VIDEO recu avec parametre(s) invalides", NULL );
      XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: ed_cmd_VIDEO_cb ");
      return;
   }


   /*A Traitement de la commande */ 

   switch(vl_codecmd)
   {
      /*B Traitement commande d'affectation camera/moniteur/position */
      case XDC_AFFECT_IMAG_JOYSTICK:
      {
	 /* Verif no de positionnement */
         if ( (vl_nojoystick != XZEVC_POS_AUCUNE)  &&  ( (vl_nojoystick < XDC_LES_2_SENS) || (vl_nojoystick > XDC_ENTREE_SENS_2) ) )
         {
            XZST_03EcritureTrace(XZSTC_WARNING,"Affectation camera-moniteur-position: No de position Invalide %d",
		  vl_nojoystick );
            return;
         }
         /*! Ramener le numero de positionnement camera dans l'intervalle connu du systeme video */
         if (vl_nojoystick == XDC_ENTREE_SENS_1) {
            vl_nojoystick = XZEVC_POS_ENTREE_S1;
         } else if (vl_nojoystick == XDC_ENTREE_SENS_2) {
            vl_nojoystick = XZEVC_POS_ENTREE_S2;
         }

         /* Construction de la trame */
         sprintf (vl_trame, "KV CA = %d %d %d"
         		,vl_nojoystick
                        ,vl_CodeCamera
                        ,vl_CodeMoniteur);

//         sprintf (vl_trame, "ACT XV %d %d %d",
//                        vl_CodeCamera,
//                        vl_CodeMoniteur,
//                        vl_nojoystick);
//
//        /* Mise en memoire des donnees de la commande */
//        for ( pl_ListeCam = pg_debutCamera; pl_ListeCam != NULL ; pl_ListeCam = pl_ListeCam->Suivant )
//        {
//           if ( pl_ListeCam->Etat.NoMoni == vl_nomoniteur  ) 
//              pl_ListeCam->Etat.NoMoni = 0;
//        }
//        pl_ListeCamera->Etat.CmdCour=XDC_AFFECT_IMAG_JOYSTICK;
//        pl_ListeCamera->Etat.NoMoni=vl_nomoniteur;
//        pl_ListeCamera->Etat. /*NoJoy*/ NoPosi=vl_nojoystick;
//        pl_ListeCamera->Etat.CRCmdCour=XZEXC_CMD_EN_COURS;
//
//        /* Mise en memoire de l'etat moniteur */
//        pl_ListeMoniteur->CamAssocie=vl_noimage;

        /* Envoi de la trame */
	vl_cr = ev_sysv_commande ( (int) vl_codecmd, vl_trame, (int) strlen(vl_trame) );

        break;
      } 


      /*B Traitement commande positionnement ... et affectation */
      case XDC_CMD_POS:
      {
	 /* Verif no de positionnement */
         if ( (vl_noposition != XZEVC_POS_AUCUNE) && ( (vl_noposition < XDC_LES_2_SENS) || (vl_noposition > XDC_ENTREE_SENS_2) ) )
         {
            XZST_03EcritureTrace (XZSTC_WARNING, "affectation camera-moniteur-position : No de position Invalide %d",
		  vl_noposition );
            return;
         }

#if defined (TEST)
        sprintf (vl_trame, "KV CA = %d %d %d", vl_noposition, vl_CodeCamera, vl_CodeMoniteur);
        if (131 == vl_noimage) sprintf (vl_trame, "KV CA = %d %d %d", vl_noposition, 54321, vl_CodeMoniteur);
        if (132 == vl_noimage) sprintf (vl_trame, "KV CA = %d %d ", vl_noposition, vl_CodeCamera, vl_CodeMoniteur);
        if (133 == vl_noimage) sprintf (vl_trame, "KV CA = %d %d %dx", vl_noposition, vl_CodeCamera, vl_CodeMoniteur);
        if (134 == vl_noimage) sprintf (vl_trame, "KV CA=%d %d %d", vl_noposition, vl_CodeCamera, vl_CodeMoniteur);
        if (135 == vl_noimage) sprintf (vl_trame, "KV CA = d %d %d", vl_noposition, vl_CodeCamera, vl_CodeMoniteur);
        if (136 == vl_noimage) sprintf (vl_trame, "KV CA = d %d %d", vl_noposition, vl_CodeCamera, vl_CodeMoniteur);
#else
         /*! Ramener le numero de positionnement camera dans l'intervalle connu du systeme video */
         if (vl_noposition == XDC_ENTREE_SENS_1) {
            vl_noposition = XZEVC_POS_ENTREE_S1;
         } else if (vl_noposition == XDC_ENTREE_SENS_2) {
            vl_noposition = XZEVC_POS_ENTREE_S2;
         }

         /* Construction de la trame */
         sprintf (vl_trame, "KV CA = %d %d %d"
         		,vl_noposition
                        ,vl_CodeCamera
                        ,vl_CodeMoniteur);
#endif

//         /* Mise en memoire des donnees de la commande */
//         pl_ListeCamera->Etat.CmdCour=XDC_CMD_POS;
//         pl_ListeCamera->Etat.NoMoni=vl_nomoniteur;
//         pl_ListeCamera->Etat.NoPosi=vl_noposition;
//         pl_ListeCamera->Etat.CRCmdCour=XZEXC_CMD_EN_COURS;

        /* Envoi de la trame */
	vl_cr = ev_sysv_commande ( (int) vl_codecmd, vl_trame, (int) strlen(vl_trame) );

        break;
      }
   }

   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: ev_cmd_VIDEO_cb ");
}





/*X*/
/*------------------------------------------------------------------------------
* SERVICE RENDU :
*
*  Envoi d'une commande a un destinataire de type Systeme video.
*
--------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	ev_sysv_commande_sysv (
	int		va_num_sysV,
	int		va_type_cmde,
	char		*pa_commande,
	int		va_lgr_cmde)

/*
* ARGUMENTS EN ENTREE :
*   va_num_sysV		: Identifiant du destinataire de la commande.
*   va_type_cmde	: Type de commande au systeme video.
*   pa_commande		: Donnees de la commande.
*   pl_lgr_cmde		: Taille de la commande.
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
*
* CODE RETOUR : 
*   XDC_OK en cas de succes, XDC_NOK sinon.
*
* CONDITION D'UTILISATION
*  Fonction appelee lors de la reception du message
*  XDM_ECMD_VIDEO
*
* FONCTION 
*  Tente d'envoyer la commande a l'exemplaire du Systeme video indique.
*  En cas d'erreur, emet un avertissement et clot la connexion vers le serveur.
*
------------------------------------------------------------------------------*/
{
	XZEXT_MASK	**pl_ptr_masque;
	char		*vl_msg_erreur = NULL;
	int		vl_cr, vl_cr_reponse;

	/* Verifier les parametres d'entree */
	if ( (va_num_sysV != EVIDC_SYSV1)  &&  (va_num_sysV != EVIDC_SYSV2) ) {
		return (XZEXC_ARG_INV);
	}

	/* Verifier si la connexion est etablie */
	if ( ! estConnecte (va_num_sysV) ) {
		return (XDC_NOK);
	}

	pl_ptr_masque = &vg_sysvid[va_num_sysV].Masque;

	/*A Purger la ligne vers le serveur puis envoyer la commande */
	ec_sysv_purger (va_num_sysV);
	vl_cr = ec_sysv_ecrire (va_num_sysV, pa_commande, va_lgr_cmde);

	if (vl_cr != XDC_OK) {
		vl_msg_erreur = "Erreur de commande Systeme video %d; cloture connexion.";
	}


	/* Si la commande a ete envoyee alors */
	if ( vl_cr == XDC_OK ) {
		/* attendre la reponse du systeme video; tracer et memoriser une non-reponse */
		vl_cr = ex_select (vg_sysvid[va_num_sysV].Masque->desc_sock, XZEXC_VAL_DEFAUT_TIMER);

		if ( vl_cr != XDC_OK ) {
			XZST_03EcritureTrace (XZSTC_WARNING, "Le Systeme video %d ne repond pas.", va_num_sysV);
			vl_msg_erreur = "Le Systeme video %d ne repond pas; cloture connexion.";
		}
	}

	/* Si une reponse a ete recue alors */
	if ( vl_cr == XDC_OK ) {
		/* la lire et la traiter; memoriser toute erreur de lecture */
		em_traitement_trame ((*pl_ptr_masque)->desc_sock, va_type_cmde, &vl_cr_reponse);

		if ( vl_cr_reponse != XDC_OK ) {
			vl_cr = XDC_NOK;
			vl_msg_erreur = "Erreur en lecture de reponse du Systeme video %d; cloture connexion.";
		}
	}

	/* Si l'une des operations a echoue alors */
	if ( vl_msg_erreur != NULL ) {
		/* emettre un message d'avertissement et clore la connexion */
		ec_message_operateurs (vl_msg_erreur, va_num_sysV);
		ex_fin_cnx ((*pl_ptr_masque)->desc_sock, pl_ptr_masque);

		/* et avertir d'une perte totale d'acces au Systeme video */
		if ( ! estConnecte (EVIDC_SYSV1)  &&  ! estConnecte (EVIDC_SYSV2) ) {
			XZST_03EcritureTrace (XZSTC_WARNING, "AUCUN ACCES AU SYSTEME VIDEO !");
			ec_message_operateurs ("AUCUN ACCES AU SYSTEME VIDEO !", 0);
		}
	}

	return (vl_cr);
}





/*X*/
/*------------------------------------------------------------------------------
* SERVICE RENDU :
*
*  Envoi d'une commande au Systeme video.
*
--------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	ev_sysv_commande (
	int		va_type_cmde,
	char		*pa_commande,
	int		va_lgr_cmde)

/*
* ARGUMENTS EN ENTREE :
*   pl_type_cmde	: Type de la commande a envoyer.
*   pa_commande		: Donnees de la commande.
*   pl_lgr_cmde		: Taille de la commande.
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
*
* CODE RETOUR : 
*   XDC_OK en cas de succes, XDC_NOK sinon.
*
* CONDITION D'UTILISATION
*  Fonction appelee lors de la reception du message
*  XDM_ECMD_VIDEO
*
* FONCTION 
*  Tente d'envoyer la commande au systeme video courant, et bascule sur
*  l'autre en cas d'echec.
*
------------------------------------------------------------------------------*/
{
	int	vl_lAutre_SV = (EVIDC_SYSV1 + EVIDC_SYSV2) - vg_noSV;
	int	vl_cr = XDC_NOK;

	/* Envoyer la commande au systeme video courant s'il est connecte */
	if ( estConnecte (vg_noSV) ) {
		vl_cr = ev_sysv_commande_sysv (vg_noSV, va_type_cmde, pa_commande, va_lgr_cmde);
	}

	/* Si la commande n'a pu etre envoyee, basculer sur l'autre systeme video s'il est connecte */
	if ( (vl_cr != XDC_OK)  &&  estConnecte (vl_lAutre_SV) ) {
		vg_noSV = vl_lAutre_SV;
		vl_cr = ev_sysv_commande_sysv (vg_noSV, va_type_cmde, pa_commande, va_lgr_cmde);
	}

	return (vl_cr);
}





/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Commande LCR.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/


void	ev_CMD_LCR ( T_IPC_CONN			va_Cnx,
		     T_IPC_CONN_PROCESS_CB_DATA		data,
		     T_CB_ARG				arg)
		     
		     
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
*  XDM_ELCR
*
* FONCTION 
*  Demande fenetre LCR
*
------------------------------------------------------*/
{
   static char *version = "$Id: evid_cmd.c,v 1.32 2020/11/03 18:12:41 pc2dpdy Exp $ : ev_CMD_LCR" ;

#ifndef _TIBCO_EMS   
   T_INT2			vl_TypeEqt;
   T_INT2			vl_NoEqt;
   T_STR			vl_FicCmdLcr;
   T_STR			vl_FicResLcr;
   T_INT2			vl_Periode;
   T_REAL8			vl_HrDebut;
   T_REAL8			vl_HrFin;
#else
   XDY_Mot			vl_TypeEqt;
   XDY_Mot			vl_NoEqt;
   XDY_FichEquext		vl_FicCmdLcr;
   XDY_FichEquext		vl_FicResLcr;
   XDY_Mot			vl_Periode;
   XDY_Horodate			vl_HrDebut;
   XDY_Horodate			vl_HrFin;
#endif   

    #ifndef _TIBCO_EMS
    T_STR			vl_NomMachine;
    #else
    XDY_NomMachine		vl_NomMachine;
    #endif   

   INFO_EQT_FEN_LCR	        pl_eqt,*pl_ListeEqt;

	XZST_03EcritureTrace(XZSTC_WARNING,"IN ev_CMD_LCR");

	/*A Lecture du message */
	if (!TipcMsgRead(data->msg,
	                   T_IPC_FT_INT2,	&vl_TypeEqt,
	                   T_IPC_FT_INT2,	&vl_NoEqt,
	                   T_IPC_FT_STR,	&vl_FicCmdLcr,
	                   T_IPC_FT_STR,	&vl_FicResLcr,
	                   T_IPC_FT_INT2,	&vl_Periode,
	                   T_IPC_FT_REAL8, &vl_HrDebut,
	                   T_IPC_FT_REAL8, &vl_HrFin,
	                   T_IPC_FT_STR,	&vl_NomMachine,
	                   NULL) )
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"messsage XDM_ELCR non lu." );
		XZST_10ArchiverMsgSyst(0,"ev_CMD_LCR : Message XDM_ELCR(VID) recu avec parametre(s) invalide(s)",NULL);
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: ev_CMD_LCR");
		return;
	}

	/*! DEM1189 Video IP : les commandes au Systeme video ne sont plus envoyees selon le protocole LCR */
	XZEX02_Retour_LCR (XDC_NOK, vl_NomMachine);


//	/*A Construction de la liste globale des eqt a piloter par la fenetre LCR */
//	pl_eqt.TypeEqt=vl_TypeEqt;
//	strcpy(pl_eqt.NomFichCmdesLCR,vl_FicCmdLcr);
//	strcpy(pl_eqt.NomFichResLCR,vl_FicResLcr);
//	pl_eqt.Periode=vl_Periode;
//	pl_eqt.HrDebut=vl_HrDebut;
//	pl_eqt.HrFin=vl_HrFin;
//	strcpy(pl_eqt.NomMachine,vl_NomMachine);
//	pl_eqt.Position=0;
//	pl_eqt.Suivant=NULL;
//	pl_eqt.NoEqt=vl_NoEqt;
//
//	/* Recherche si pas de commande LCR en cours sur equipement concerne */
//	if ( (vl_TypeEqt == XDC_EQT_SYV) &&
//	     (ex_test_autorisation_eqt ( &pg_DebutListeVIDFLcr, &pl_eqt, XDC_EQT_SYV ) == XDC_NOK) )
//	{
//		return;
//	}
//
//	if (vl_TypeEqt == XDC_EQT_SYV)
//	{
//		if ( (vg_sysvid.Numero == vl_NoEqt) || (vl_NoEqt==XDC_EQTALL) )
//		{
//			ex_init_lcr (	&pg_DebutListeVIDFLcr,
//        	    			pl_eqt,
//        	    			vg_sysvid.SocketMaitre,
//        	    			vg_sysvid.AdresseRGS,
//        	    			vg_sysvid.Numero,
//        	    			"VID_FLcr_NoEqt:",
//        	    			vg_SockTIM );
//			return;
//		} else {
//			XZEX02_Retour_LCR (XDC_NOK,vl_NomMachine);
//		}
//	} else {
//		XZEX02_Retour_LCR (XDC_NOK,vl_NomMachine);
//	}

	/* Fin du callback */
	XZST_03EcritureTrace(XZSTC_FONCTION ,"OUT Callback ev_CMD_LCR" );
	return;
}
