/*E*/
/*  Fichier : $Id: exa0_tra.c,v 1.12 1997/06/25 12:49:48 gesconf Exp $      Release : $Revision: 1.12 $        Date : $Date: 1997/06/25 12:49:48 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEXA0 * FICHIER exa0_tra.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Module de traitement des sockets provenant de TEXAO
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	22 Nov 1994	: Creation
* Mismer.D	version 1.2	09 Fev 1995	:
* Mismer.D	version 1.3	09 Mar 1995	:
* Mismer.D	version 1.4	09 Mai 1995	:
* Mismer.D	version 1.5	18 Mai 1995	: Raz flag API5000 sur coupure connexion, reprise entete, historique
* Mismer.D	version 1.6	30 Mai 1995	: Modif appel fonction ea_gestion_cmd
* Mismer.D	version 1.7	10 Aou 1995	: Modif condition reiteration transfert vers tenic
* Mismer.D	version 1.8	06 Sep 1995	: Modif ecriture fichier trace commande
* Mismer.D      version 1.9     24 Avr 1997     : Ajout alerte monaco (DEM/1421)
* Mismer.D      version 1.10    06 Jun 1997     : Ajout alerte monaco suite (DEM/1421)
* Mismer.D      version 1.11    06 Jun 1997     : Ajout alerte monaco suite2 (DEM/1421)
* Mismer.D      version 1.12    23 Jun 1997     : Ajout alerte monaco suite3 (DEM/1421)
*
------------------------------------------------------*/

/* fichiers inclus */
#include <xzsem.h>

#include "exa0_tra.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */


/* declaration de fonctions internes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Traitement des info recu de AprilNet
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static void ea_lecture_automate ( int va_TypeEvt )

/*
* ARGUMENTS EN ENTREE :
*   
* int va_TypeEvt	: Table automate a lire
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
*
* CONDITION D'UTILISATION
* execute sur evenements AprilNet
*
* FONCTION 
*
*  Lecture des donnees automate 
*
------------------------------------------------------*/
{
   static char *version = "$Id: exa0_tra.c,v 1.12 1997/06/25 12:49:48 gesconf Exp $ : ea_lecture_automate " ;
   
   int				vl_Adresse, vl_Eqt, vl_Numero, i,vl_PosMsg,
   				vl_Index;
   EXA0_DONNEES_Tube *		pl_Tube;
   EXA0_DONNEES_Echangeur *	pl_Ech;  
   char vl_Msg[80]="";   
   
   
   switch( va_TypeEvt )
   {
      case XZETC_TELEALARME :
         XZST_03EcritureTrace( XZSTC_MESSAGE, "Succes Lecture table TELEALARME " );
         break;
      case XZETC_ETOR :
         XZST_03EcritureTrace( XZSTC_MESSAGE, "Succes Lecture table ETOR " );
         break;
      case XZETC_DISCORDANCE :
         XZST_03EcritureTrace( XZSTC_MESSAGE, "Succes Lecture table ITOR " );
         break;
      case XZETC_PANNEAU :
         XZST_03EcritureTrace( XZSTC_MESSAGE, "Succes Lecture table PANNEAU " );
         break;
      case XZETC_ALERTE :
         XZST_03EcritureTrace( XZSTC_WARNING, "Succes Lecture table ALERTE " );
         break;
    }

   /*A Recherche pour la liste des tubes si evenement present */
   for ( pl_Tube = pg_debutTube; pl_Tube != NULL; pl_Tube = pl_Tube->Suivant )
   {
      /*A Recherche du type d'evenement */
      switch( va_TypeEvt )
      {
         /*A C'est une Telealarame */
         case XZETC_TELEALARME :
            /* Si evenement sur le tube courant */
            vl_Index = pl_Tube->AdresseTelealarme - vg_ConfigAdresse.AdresseTelealarme;
            if ( (vl_Index <= vg_ConfigAdresse.LongTelealarme) && (vl_Index >= 0) &&
                 (pg_ResultTelealarme[vl_Index] != pg_Telealarme[vl_Index]) )
            {
               /* Transmettre evenement sur Telealarmes */
               XZST_03EcritureTrace( XZSTC_INFO, "Evenement TELEALARME (Table:%d).", vl_Index);
               ea_transmettre_evt ( XZETC_TELEALARME,
               			    pl_Tube->AdresseTelealarme,
               			    XDC_EQT_TUBE,
               			    pl_Tube->Numero,
               			    &pg_ResultTelealarme[vl_Index],
               			    &pg_Telealarme[vl_Index], 
               			    1 );
            }
            vl_Index  =  pl_Tube->AdresseGabarit - vg_ConfigAdresse.AdresseTelealarme;
            if ( (vl_Index <= vg_ConfigAdresse.LongTelealarme) && (vl_Index >= 0) &&
                 (memcmp(&pg_ResultTelealarme[vl_Index], 
                         &pg_Telealarme[vl_Index], 
                         XZETC_NB_MOT_GABARIT * sizeof(short))) )
            {
               /* Transmettre evenement  gabarit */
               XZST_03EcritureTrace( XZSTC_WARNING, "Evenement Tube Gabarit (Table:%d).", vl_Index);
               ea_transmettre_evt ( XZETC_ALERTE,
               			    pl_Tube->AdresseGabarit,
               			    XDC_EQT_TUBE,
               			    pl_Tube->Numero,
               			    &pg_ResultTelealarme[vl_Index],
               			    &pg_Telealarme[vl_Index], 
               			    XZETC_NB_MOT_GABARIT );
               
            }
            vl_Index  =  pl_Tube->AdresseVitesse - vg_ConfigAdresse.AdresseTelealarme;
            if ( (vl_Index <= vg_ConfigAdresse.LongTelealarme) && (vl_Index >= 0) &&
                 (memcmp(&pg_ResultTelealarme[vl_Index], 
                         &pg_Telealarme[vl_Index], 
                         XZETC_NB_MOT_VITESSE * sizeof(short))) )
            {
               /* Transmettre evenement  vitesse */
               XZST_03EcritureTrace( XZSTC_WARNING, "Evenement Tube Vitesse (Table:%d).", vl_Index);
               ea_transmettre_evt ( XZETC_ALERTE,
               			    pl_Tube->AdresseVitesse,
               			    XDC_EQT_TUBE,
               			    pl_Tube->Numero,
               			    &pg_ResultTelealarme[vl_Index],
               			    &pg_Telealarme[vl_Index], 
               			    XZETC_NB_MOT_VITESSE );
               
            }
            vl_Index  =  pl_Tube->AdresseNiche - vg_ConfigAdresse.AdresseTelealarme;
            if ( (vl_Index <= vg_ConfigAdresse.LongTelealarme) && (vl_Index >= 0) &&
                 (memcmp(&pg_ResultTelealarme[vl_Index], 
                         &pg_Telealarme[vl_Index], 
                         XZETC_NB_MOT_NICHE * sizeof(short))) )
            {
               /* Transmettre evenement  niche */
               XZST_03EcritureTrace( XZSTC_WARNING, "Evenement Tube Niche (Table:%d).", vl_Index);
               ea_transmettre_evt ( XZETC_ALERTE,
               			    pl_Tube->AdresseNiche,
               			    XDC_EQT_TUBE,
               			    pl_Tube->Numero,
               			    &pg_ResultTelealarme[vl_Index],
               			    &pg_Telealarme[vl_Index], 
               			    XZETC_NB_MOT_NICHE );
               
            }
            break;
            
         /*A C'est une ETOR */
         case XZETC_ETOR :
            vl_Index = pl_Tube->AdresseExtincteur - vg_ConfigAdresse.AdresseETOR;
            if ( (vl_Index <= vg_ConfigAdresse.LongueurETOR) && (vl_Index >= 0) &&
                 (pg_ResultETOR[vl_Index] != pg_ETOR[vl_Index]) )
            {
               /* Transmettre evenement sur ETOR */
               XZST_03EcritureTrace( XZSTC_INFO, "Evenement Extincteur (Table:%d).", vl_Index);
               ea_transmettre_evt ( XZETC_ETOR,
               			    pl_Tube->AdresseExtincteur,
               			    XDC_EQT_TUBE,
               			    pl_Tube->Numero,
               			    &pg_ResultETOR[vl_Index],
               			    &pg_ETOR[vl_Index], 
               			    1 );
            }
            vl_Index = pl_Tube->AdresseLit - vg_ConfigAdresse.AdresseETOR;
            if ( (vl_Index <= vg_ConfigAdresse.LongueurETOR) && (vl_Index >= 0) &&
                 (pg_ResultETOR[vl_Index] != pg_ETOR[vl_Index]) ) 
            {
               /* Transmettre evenement sur ETOR */
               XZST_03EcritureTrace( XZSTC_INFO, "Evenement Lit d'arret (Table:%d).", vl_Index);
               ea_transmettre_evt ( XZETC_ETOR,
               			    pl_Tube->AdresseLit,
               			    XDC_EQT_TUBE,
               			    pl_Tube->Numero,
               			    &pg_ResultETOR[vl_Index],
               			    &pg_ETOR[vl_Index], 
               			    1 );
            }
            vl_Index  =  pl_Tube->AdresseGabarit - vg_ConfigAdresse.AdresseETOR;
            if ( (vl_Index <= vg_ConfigAdresse.LongueurETOR) && (vl_Index >= 0) &&
                 (memcmp(&pg_ResultETOR[vl_Index], 
                         &pg_ETOR[vl_Index], 
                         XZETC_NB_MOT_GABARIT * sizeof(short))) )
            {
               /* Transmettre evenement  gabarit */
               XZST_03EcritureTrace( XZSTC_WARNING, "Evenement Tube Gabarit (Table:%d).", vl_Index);
               ea_transmettre_evt ( XZETC_ALERTE,
               			    pl_Tube->AdresseGabarit,
               			    XDC_EQT_TUBE,
               			    pl_Tube->Numero,
               			    &pg_ResultETOR[vl_Index],
               			    &pg_ETOR[vl_Index], 
               			    XZETC_NB_MOT_GABARIT );
               
            }
            vl_Index  =  pl_Tube->AdresseVitesse - vg_ConfigAdresse.AdresseETOR;
            if ( (vl_Index <= vg_ConfigAdresse.LongueurETOR) && (vl_Index >= 0) &&
                 (memcmp(&pg_ResultETOR[vl_Index], 
                         &pg_ETOR[vl_Index], 
                         XZETC_NB_MOT_VITESSE * sizeof(short))) )
            {
               /* Transmettre evenement  vitesse */
               XZST_03EcritureTrace( XZSTC_WARNING, "Evenement Tube Vitesse (Table:%d).", vl_Index);
               ea_transmettre_evt ( XZETC_ALERTE,
               			    pl_Tube->AdresseVitesse,
               			    XDC_EQT_TUBE,
               			    pl_Tube->Numero,
               			    &pg_ResultETOR[vl_Index],
               			    &pg_ETOR[vl_Index], 
               			    XZETC_NB_MOT_VITESSE );
               
            }
            vl_Index  =  pl_Tube->AdresseNiche - vg_ConfigAdresse.AdresseETOR;
            if ( (vl_Index <= vg_ConfigAdresse.LongueurETOR) && (vl_Index >= 0) &&
                 (memcmp(&pg_ResultETOR[vl_Index], 
                         &pg_ETOR[vl_Index], 
                         XZETC_NB_MOT_NICHE * sizeof(short))) )
            {
               /* Transmettre evenement  niche */
               XZST_03EcritureTrace( XZSTC_WARNING, "Evenement Tube Niche (Table:%d).", vl_Index);
               ea_transmettre_evt ( XZETC_ALERTE,
               			    pl_Tube->AdresseNiche,
               			    XDC_EQT_TUBE,
               			    pl_Tube->Numero,
               			    &pg_ResultETOR[vl_Index],
               			    &pg_ETOR[vl_Index], 
               			    XZETC_NB_MOT_NICHE );
               
            }
            break;
               
         /*A C'est une discordance   */
         case XZETC_DISCORDANCE :
            vl_Index  =  pl_Tube->AdresseDiscord - vg_ConfigAdresse.AdresseITOR;
            if ( (vl_Index <= vg_ConfigAdresse.LongueurITOR) && (vl_Index >= 0) &&
                 (memcmp(&pg_ResultITOR[vl_Index], 
                         &pg_ITOR[vl_Index], 
                         XZETC_NB_MOT_ITOR * sizeof(short))) )
            {
               /* Transmettre evenement  discordance*/
               XZST_03EcritureTrace( XZSTC_INFO, "Evenement Tube ITOR (Table:%d).", vl_Index);
               ea_transmettre_evt ( XZETC_DISCORDANCE,
               			    pl_Tube->AdresseDiscord,
               			    XDC_EQT_TUBE,
               			    pl_Tube->Numero,
               			    &pg_ResultITOR[vl_Index],
               			    &pg_ITOR[vl_Index], 
               			    XZETC_NB_MOT_ITOR );
               
            }
            break;
            
         /*A C'est un changement d'etat d'un panneau */
         case XZETC_PANNEAU :
            vl_Index  = pl_Tube->AdresseEtat - vg_ConfigAdresse.AdresseTelesignTub;
            if ( (vl_Index <= vg_ConfigAdresse.LongTelesignTub) && (vl_Index >= 0) &&
                 (memcmp(&pg_ResultSignalisation[vl_Index], 
                         &pg_Signalisation[vl_Index], 
                         (XZETC_MAX_PANNEAU/XZETC_NB_ETAT_PAN) * sizeof(short))) )
            {
               /* Transmettre evenement  discordance*/
               XZST_03EcritureTrace( XZSTC_INFO, "Evenement Tube PANNEAU (Table:%d).", vl_Index);
               ea_transmettre_evt ( XZETC_PANNEAU,
               			    pl_Tube->AdresseEtat,
               			    XDC_EQT_TUBE,
               			    pl_Tube->Numero,
               			    &pg_ResultSignalisation[vl_Index],
               			    &pg_Signalisation[vl_Index], 
               			    XZETC_MAX_PANNEAU/XZETC_NB_ETAT_PAN );
            }
            break;
               
         /*A C'est une alerte   */
         case XZETC_ALERTE :
            vl_Index  =  pl_Tube->AdresseGabarit - vg_ConfigAdresse.AdresseAlerteTub;
            if ( (vl_Index <= vg_ConfigAdresse.LongAlerteTub) && (vl_Index >= 0) &&
                 (memcmp(&pg_ResultAlerte[vl_Index], 
                         &pg_Alerte[vl_Index], 
                         XZETC_NB_MOT_GABARIT * sizeof(short))) )
            {
               /* Transmettre evenement  gabarit */
               XZST_03EcritureTrace( XZSTC_WARNING, "Evenement Tube Gabarit (Table:%d).", vl_Index);
               ea_transmettre_evt ( XZETC_ALERTE,
               			    pl_Tube->AdresseGabarit,
               			    XDC_EQT_TUBE,
               			    pl_Tube->Numero,
               			    &pg_ResultAlerte[vl_Index],
               			    &pg_Alerte[vl_Index], 
               			    XZETC_NB_MOT_GABARIT );
               
            }
            vl_Index  =  pl_Tube->AdresseVitesse - vg_ConfigAdresse.AdresseAlerteTub;
            if ( (vl_Index <= vg_ConfigAdresse.LongAlerteTub) && (vl_Index >= 0) &&
                 (memcmp(&pg_ResultAlerte[vl_Index], 
                         &pg_Alerte[vl_Index], 
                         XZETC_NB_MOT_VITESSE * sizeof(short))) )
            {
               /* Transmettre evenement  vitesse */
               XZST_03EcritureTrace( XZSTC_WARNING, "Evenement Tube Vitesse (Table:%d).", vl_Index);
               ea_transmettre_evt ( XZETC_ALERTE,
               			    pl_Tube->AdresseVitesse,
               			    XDC_EQT_TUBE,
               			    pl_Tube->Numero,
               			    &pg_ResultAlerte[vl_Index],
               			    &pg_Alerte[vl_Index], 
               			    XZETC_NB_MOT_VITESSE );
               
            }
            vl_Index  =  pl_Tube->AdresseNiche - vg_ConfigAdresse.AdresseAlerteTub;
            if ( (vl_Index <= vg_ConfigAdresse.LongAlerteTub) && (vl_Index >= 0) &&
                 (memcmp(&pg_ResultAlerte[vl_Index], 
                         &pg_Alerte[vl_Index], 
                         XZETC_NB_MOT_NICHE * sizeof(short))) )
            {
               /* Transmettre evenement  niche */
               XZST_03EcritureTrace( XZSTC_WARNING, "Evenement Tube Niche (Table:%d).", vl_Index);
               ea_transmettre_evt ( XZETC_ALERTE,
               			    pl_Tube->AdresseNiche,
               			    XDC_EQT_TUBE,
               			    pl_Tube->Numero,
               			    &pg_ResultAlerte[vl_Index],
               			    &pg_Alerte[vl_Index], 
               			    XZETC_NB_MOT_NICHE );
               
            }
            break;

         /*A Ce n'est pas un cas traite alors trace erreur */
         default :
            XZST_03EcritureTrace( XZSTC_WARNING, "Erreur lecture automate inconnue." );
            break;
      }
      
   }

   for ( pl_Ech = pg_debutEchangeur; pl_Ech != NULL; pl_Ech = pl_Ech->Suivant )
   {
      /*A Recherche du type d'evenement */
      switch( va_TypeEvt )
      {
         /*A C'est une Telealarame */
         case XZETC_TELEALARME :
            /* Si evenement sur le tube courant */
            vl_Index = pl_Ech->AdresseTelealarme - vg_ConfigAdresse.AdresseTelealarme;
            if ( (vl_Index <= vg_ConfigAdresse.AdresseTelealarme) && (vl_Index >= 0) &&
                 (pg_ResultTelealarme[vl_Index] != pg_Telealarme[vl_Index]) )
            {
               XZST_03EcritureTrace( XZSTC_INFO, "Evenement Ech TELEALARME (Table:%d).", vl_Index);
               /* Transmettre evenement sur Telealarmes */
               ea_transmettre_evt ( XZETC_TELEALARME,
               			    pl_Ech->AdresseTelealarme,
               			    XDC_EQT_ECH,
               			    pl_Ech->NumeroDemiEch,
               			    &pg_ResultTelealarme[vl_Index],
               			    &pg_Telealarme[vl_Index], 
               			    1 );
            }
            break;
            
         /*A C'est une discordance   */
         case XZETC_DISCORDANCE :
            vl_Index  = pl_Ech->AdresseDiscord - vg_ConfigAdresse.AdresseITOR;
            if ( (vl_Index <= vg_ConfigAdresse.LongueurITOR) && (vl_Index >= 0) &&
                 (memcmp(&pg_ResultITOR[vl_Index], 
                         &pg_ITOR[vl_Index], 
                         XZETC_NB_MOT_ITOR * sizeof(short))) )
            {
               XZST_03EcritureTrace( XZSTC_INFO, "Evenement Ech ITOR (Table:%d).", vl_Index);
               /* Transmettre evenement  discordance*/
               ea_transmettre_evt ( XZETC_DISCORDANCE,
               			    pl_Ech->AdresseDiscord,
               			    XDC_EQT_ECH,
               			    pl_Ech->NumeroDemiEch,
               			    &pg_ResultITOR[vl_Index],
               			    &pg_ITOR[vl_Index], 
               			    XZETC_NB_MOT_ITOR );
            }
            break;
            
         /*A C'est un changement d'etat d'un panneau */
         case XZETC_PANNEAU :
            vl_Index  = pl_Ech->AdresseEtat - vg_ConfigAdresse.AdresseTelesignEch;
            if ( (vl_Index <= vg_ConfigAdresse.LongTelesignEch) && (vl_Index >= 0) &&
                 (memcmp(&pg_ResultSignalisation[vl_Index], 
                         &pg_Signalisation[vl_Index], 
                         (XZETC_MAX_PANNEAU/XZETC_NB_ETAT_PAN) * sizeof(short))) )
            {
               XZST_03EcritureTrace( XZSTC_INFO, "Evenement Ech PANNEAU (Table:%d).", vl_Index);
               /* Transmettre evenement  discordance*/
               ea_transmettre_evt ( XZETC_PANNEAU,
               			    pl_Ech->AdresseEtat,
               			    XDC_EQT_ECH,
               			    pl_Ech->NumeroDemiEch,
               			    &pg_ResultSignalisation[vl_Index],
               			    &pg_Signalisation[vl_Index], 
               			    XZETC_MAX_PANNEAU/XZETC_NB_ETAT_PAN );
            }
            break;
            
         case XZETC_ETOR :
         case XZETC_ALERTE :
            break;
            
         /*A Ce n'est pas un cas traite alors trace erreur */
         default :
            XZST_03EcritureTrace( XZSTC_WARNING, "Erreur lecture automate inconnue." );
            break;
      }
      
   }
   
   /*A Recherche du type d'evenement */
   switch( va_TypeEvt )
   {
      /*A C'est une Telealarame */
      case XZETC_TELEALARME :
         /* Memorisation de l'etat des  */
         memcpy ( pg_Telealarme, pg_ResultTelealarme, vg_ConfigAdresse.LongTelealarme * sizeof(short) );
         ea_ecriture_flag ( XZETC_ADRESSE_FLAG + XZETC_FLAG_TELEALARME , 0 );
         break;
            
      /*A C'est une ETOR */
      case XZETC_ETOR :
         /* Memorisation de l'etat des  */
         memcpy ( pg_ETOR, pg_ResultETOR, vg_ConfigAdresse.LongueurETOR * sizeof(short) );
         ea_ecriture_flag ( XZETC_ADRESSE_FLAG + XZETC_FLAG_ETOR , 0 );
         break;
               
      /*A C'est une discordance   */
      case XZETC_DISCORDANCE :
         memcpy ( pg_ITOR, pg_ResultITOR, vg_ConfigAdresse.LongueurITOR * sizeof(short) );
         ea_ecriture_flag ( XZETC_ADRESSE_FLAG + XZETC_FLAG_ITOR , 0 );
         break;
            
      /*A C'est un changement d'etat d'un panneau */
      case XZETC_PANNEAU :
         memcpy ( pg_Signalisation, pg_ResultSignalisation, vg_ConfigAdresse.LongTelesignTub * sizeof(short) );
         ea_ecriture_flag ( XZETC_ADRESSE_FLAG + XZETC_FLAG_SIGNALISATION , 0 );
         break;
               
      /*A C'est une alerte   */
      case XZETC_ALERTE :
         memcpy ( pg_Alerte, pg_ResultAlerte, vg_ConfigAdresse.LongAlerteTub * sizeof(short) );
         ea_ecriture_flag ( XZETC_ADRESSE_FLAG + XZETC_FLAG_ALERTE , 0 );
         break;
   }
}





/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Traitement des info recu de AprilNet
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int ea_transmettre_evt ( 	int	va_TypeEvt,
				int	va_Adresse,
				int	va_TypeEqt,
				int	va_NumEqt,
				short * pa_EtatActuel,
				short * pa_EtatPrecedent,
				int	va_Taille )

/*
* ARGUMENTS EN ENTREE :
*   
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
*
* CONDITION D'UTILISATION
* 
*
* FONCTION 
*
*  envoi evenement vers TENIC
*
------------------------------------------------------*/
{
   static char *version = "$Id: exa0_tra.c,v 1.12 1997/06/25 12:49:48 gesconf Exp $ : ea_transmettre_evt " ;
   
   int				vl_IndCnx = 0,
   				vl_LgMsg  = 0;
   XDY_Horodate			vl_Horodate;   
   int				vl_JourSem;	
   int				vl_TraceCmd = 0;
   char	*			vl_Mot;
   XDY_Mot			vl_Change;
   XZEXT_MSG_SOCKET		vl_Msg = "", vl_MsgTrace = ""; 
   short			vl_PosMsg = 0,
   				vl_PosTra = 0,
				vl_Index;		    


        XZSM_07LireHorodate( XZSMC_TYPE_SYSTEME, &vl_JourSem, &vl_Horodate );
        /*A Construction de la trame d'emission */
        vl_PosMsg = sprintf ( vl_Msg, 	"%d %d %d %d %lf ",	
        				va_TypeEvt,
					va_Adresse,
					va_TypeEqt,
					va_NumEqt,
					vl_Horodate ); 
	for ( vl_Index = 0; vl_Index < va_Taille; vl_Index++, pa_EtatActuel++, pa_EtatPrecedent++)
	{
           /* Ajout dans la trame d'emission */
           if ( (*pa_EtatActuel <= 0xFFFF) && (*pa_EtatActuel >= 0) )
           {
              vl_PosMsg += sprintf( vl_Msg+vl_PosMsg,"0x%04X ", *pa_EtatActuel );
              vl_Change = *pa_EtatActuel ^ *pa_EtatPrecedent;
              vl_PosMsg += sprintf( vl_Msg+vl_PosMsg,"0x%04X ", vl_Change );
              vl_PosTra += sprintf( vl_MsgTrace+vl_PosTra, "MW%d=0x%04X ", va_Adresse+vl_Index, *pa_EtatActuel );	
           }
           else
           {
              vl_PosMsg += sprintf( vl_Msg+vl_PosMsg,"0x0000 " );
              vl_PosMsg += sprintf( vl_Msg+vl_PosMsg,"0x0000 " );
              vl_PosTra += sprintf( vl_MsgTrace+vl_PosTra, "MW%d=0x0000 ", va_Adresse+vl_Index );	
           }
        }
        vl_PosMsg += sprintf( vl_Msg+vl_PosMsg,"%s", XZEXC_FIN_MSG_SOCK );
        
       switch( va_TypeEvt )
       {
          case XZETC_TELEALARME :
             if ( vg_TabFlag[XZETC_FLAG_TELEALARME] == 1 ) vl_TraceCmd = 1;
             break;
          case XZETC_ETOR :
             if ( vg_TabFlag[XZETC_FLAG_ETOR] == 1 ) vl_TraceCmd = 1;
             break;
          case XZETC_DISCORDANCE :
             if ( vg_TabFlag[XZETC_FLAG_ITOR] == 1 ) vl_TraceCmd = 1;
             break;
          case XZETC_PANNEAU :
             if ( vg_TabFlag[XZETC_FLAG_SIGNALISATION] == 1 ) vl_TraceCmd = 1;
             break;
          case XZETC_ALERTE :
             if ( vg_TabFlag[XZETC_FLAG_ALERTE] == 1 ) vl_TraceCmd = 1;
             break;
        }
        if ( vl_TraceCmd )
           ex_ecriture_trace_cmd ( XDC_EQT_NIC,
                                   0, 
                                   NULL,
                                   vl_MsgTrace, 
                                   0 );
         			
        /*A Envoi de la trame */
        sem_wait(&vg_semaphore); 
        if ( write ( vg_SockTENIC, vl_Msg, vl_PosMsg ) < vl_PosMsg ) 				  				
        {
           sem_post(&vg_semaphore); 
           XZST_03EcritureTrace( XZSTC_WARNING, "Echec 1ere emmission de la trame <%s> vers TENIC." , vl_Msg);
           sleep ( 2 );
           sem_wait(&vg_semaphore); 
           if ( write ( vg_SockTENIC, vl_Msg, vl_PosMsg ) < vl_PosMsg ) 				  				
           {
              sem_post(&vg_semaphore); 
              /* Ecriture dans le fichier trace en cas d'�chec */
              XZST_03EcritureTrace( XZSTC_WARNING, "Echec 2eme emmission de la trame <%s> vers TENIC." , vl_Msg);
              sleep ( 2 );
              sem_wait(&vg_semaphore); 
              if ( write ( vg_SockTENIC, vl_Msg, vl_PosMsg ) < vl_PosMsg ) 				  				
              {
                  sem_post(&vg_semaphore); 
                  /* Ecriture dans le fichier trace en cas d'�chec */
                  XZST_03EcritureTrace( XZSTC_WARNING, "Echec 3eme emmission de la trame <%s> vers TENIC." , vl_Msg);
	               return(XDC_NOK);
	            }
               else
               {   
                  sem_post(&vg_semaphore); 
                  /*B Ecriture dans le fichier trace en cas de succes */
                  XZST_03EcritureTrace( XZSTC_INFO, "Trame �mise vers TENIC: %s.", vl_Msg );
	               return(XDC_OK);
               }
           }
           else
           {   
               sem_post(&vg_semaphore); 
               /*B Ecriture dans le fichier trace en cas de succes */
               XZST_03EcritureTrace( XZSTC_INFO, "Trame �mise vers TENIC: %s.", vl_Msg );
	            return(XDC_OK);
           }
        }
        else
        {   
           sem_post(&vg_semaphore); 
           /*B Ecriture dans le fichier trace en cas de succes */
           XZST_03EcritureTrace( XZSTC_INFO, "Trame �mise vers TENIC: %s.", vl_Msg );
	         return(XDC_OK);
        }
   
}

  
/* definition de fonctions externes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Traitement des info recu de AprilNet
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void ea_traitement_April ( EXA0_AST_PARAM *	pa_AstParam )

/*
* ARGUMENTS EN ENTREE :
*   
* EXA0_AST_PARAM *	pa_AstParam  : structure evenements AprilNet
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
*
* CONDITION D'UTILISATION
* execute sur evenements AprilNet
*
* FONCTION 
*
*  Recherche du flag evenement et appel de la fonction de lecture automate
*
------------------------------------------------------*/
{
   static char *version = "$Id: exa0_tra.c,v 1.12 1997/06/25 12:49:48 gesconf Exp $ : ea_traitement_April " ;
  
   static lmirevT vl_ArgLectureTELE = { NULL, LOCAL_VAR, 0, 0, ASYNCHRONOUS, 0};
   static lmorevT vl_ArgResultTELE;
   static lmirevT vl_ArgLectureSIG = { NULL, LOCAL_VAR, 0, 0, ASYNCHRONOUS, 0};
   static lmorevT vl_ArgResultSIG;
   static lmirevT vl_ArgLectureITOR = { NULL, LOCAL_VAR, 0, 0, ASYNCHRONOUS, 0};
   static lmorevT vl_ArgResultITOR;
   static lmirevT vl_ArgLectureETOR = { NULL, LOCAL_VAR, 0, 0, ASYNCHRONOUS, 0};
   static lmorevT vl_ArgResultETOR;
   static lmirevT vl_ArgLectureALER = { NULL, LOCAL_VAR, 0, 0, ASYNCHRONOUS, 0};
   static lmorevT vl_ArgResultALER;
	 
   /*A
    *  Controle du block d'erreur 
    */
   switch ( pa_AstParam->pErrBlk->class )
   {
      case SUCCES : 
         break;
      
      case EVENT  :
         switch ( pa_AstParam->pErrBlk->code )
         {
            case CANCELED :
               /* Ecoute avortee apres destruction evenement */
               XZST_03EcritureTrace( XZSTC_WARNING, " Ecoute avortee apres destruction evenement AprilNet. " );
               return;
               
            default :
               /* Appel routine d'erreur */
	       ea_gestion_erreur ( &vg_ErrApril, "ea_traitement_April" );
	       return;
	 }
	 break;
	 
      default :
         /* Appel routine d'erreur */
	 ea_gestion_erreur ( &vg_ErrApril, "ea_traitement_April" );
	 return;
   }
   
   
   /* Si il s'agit d'un evenement sur la table des semaphore */
   if ( pa_AstParam->Evenement )
   {
      /* Memorisation de l'etats des Flags */
      if ( (pa_AstParam->pLmoree->unitin < XZETC_TAILLE_FLAG) && (pa_AstParam->pLmoree->unitin >= 0) )
      {
         vg_TabFlag[pa_AstParam->pLmoree->unitin] = pa_AstParam->pLmoree->evtval.aShort;
      }
      /* Traitement des flags */
      switch ( pa_AstParam->pLmoree->unitin )
      {
         case XZETC_FLAG_MARCHE_DEGRADEE :
            XZST_03EcritureTrace( XZSTC_INFO,
            			 "Flag marche degradee = %d . ", 
            			 pa_AstParam->pLmoree->evtval.aShort );
            break;
            
         case XZETC_FLAG_BASCULEMENT :
            XZST_03EcritureTrace( XZSTC_INFO,
            			 "Flag basculement = %d . ", 
            			 pa_AstParam->pLmoree->evtval.aShort );
            break;
               
         case XZETC_FLAG_TELEALARME :
            XZST_03EcritureTrace( XZSTC_INFO,
            			 "Flag Telealarme = %d . ", 
            			 pa_AstParam->pLmoree->evtval.aShort );
            /* Demande de lecture dans l'automate des etats telealarme */
            if ( vg_TabFlag[XZETC_FLAG_TELEALARME] == 1 )
            {
               vl_ArgLectureTELE.varnam = vg_VarTelealarme.varnam;
               vl_ArgResultTELE.redres  = (STp)pg_ResultTelealarme;
               vl_ArgResultTELE.sizred  = vg_ConfigAdresse.LongTelealarme * sizeof(short);
               lm_rev ( 	vg_AssocApril.assoc, 
            		   	(XDY_Fonction)ea_lecture_automate,
            			(void *)(XZETC_TELEALARME),
            			&vl_ArgLectureTELE,
            			&vl_ArgResultTELE,
            			&vg_ErrApril );
            	ea_gestion_erreur ( &vg_ErrApril, "lm_rev XZETC_TELEALARME" );
            }
            break;
          
         case XZETC_FLAG_SIGNALISATION :
            XZST_03EcritureTrace( XZSTC_INFO,
            			 "Flag signalisation = %d . ", 
            			 pa_AstParam->pLmoree->evtval.aShort );
            /* Demande de lecture dans l'automate des etats panneaux */
            if ( vg_TabFlag[XZETC_FLAG_SIGNALISATION] == 1 )
            {
               vl_ArgLectureSIG.varnam = vg_VarSignalisation.varnam;
               vl_ArgResultSIG.redres  = (STp)pg_ResultSignalisation;
               vl_ArgResultSIG.sizred  = vg_ConfigAdresse.LongTelesignTub * sizeof(short);
               lm_rev ( 	vg_AssocApril.assoc, 
            			(XDY_Fonction)ea_lecture_automate,
            			(void *)(XZETC_PANNEAU),
            			&vl_ArgLectureSIG,
            			&vl_ArgResultSIG,
            			&vg_ErrApril );
               ea_gestion_erreur ( &vg_ErrApril, "lm_rev XZETC_PANNEAU" );
            }
            break;
      
         case XZETC_FLAG_ITOR :
            XZST_03EcritureTrace( XZSTC_INFO,
            			 "Flag ITOR = %d . ", 
            			 pa_AstParam->pLmoree->evtval.aShort );
            /* Demande de lecture dans l'automate des etats ITOR */
            if ( vg_TabFlag[XZETC_FLAG_ITOR] == 1 )
            {
               vl_ArgLectureITOR.varnam = vg_VarITOR.varnam;
               vl_ArgResultITOR.redres  = (STp)pg_ResultITOR;
               vl_ArgResultITOR.sizred  = vg_ConfigAdresse.LongueurITOR * sizeof(short);
               lm_rev ( 	vg_AssocApril.assoc, 
            			(XDY_Fonction)ea_lecture_automate,
            			(void *)(XZETC_DISCORDANCE),
            			&vl_ArgLectureITOR,
            			&vl_ArgResultITOR,
            			&vg_ErrApril );
               ea_gestion_erreur ( &vg_ErrApril, "lm_rev XZETC_DISCORDANCE" );
            }
            break;
      
         case XZETC_FLAG_ALERTE :
            XZST_03EcritureTrace( XZSTC_WARNING,
            			 "Flag Alerte = %d . ", 
            			 pa_AstParam->pLmoree->evtval.aShort );
            /* Demande de lecture dans l'automate des etats Alerte */
            if ( vg_TabFlag[XZETC_FLAG_ALERTE] == 1 )
            {
               vl_ArgLectureALER.varnam = vg_VarAlerte.varnam;
               vl_ArgResultALER.redres  = (STp)pg_ResultAlerte;
               vl_ArgResultALER.sizred  = vg_ConfigAdresse.LongAlerteTub * sizeof(short);
               lm_rev ( 	vg_AssocApril.assoc, 
            			(XDY_Fonction)ea_lecture_automate,
            			(void *)(XZETC_ALERTE),
            			&vl_ArgLectureALER,
            			&vl_ArgResultALER,
            			&vg_ErrApril );
               ea_gestion_erreur ( &vg_ErrApril, "lm_rev XZETC_ALERTE" );
            }
            break;
      
         case XZETC_FLAG_ETOR :
            XZST_03EcritureTrace( XZSTC_INFO,
            			 "Flag ETOR = %d . ", 
            			 pa_AstParam->pLmoree->evtval.aShort );
            /* Demande de lecture dans l'automate des etats ETOR */
            if ( vg_TabFlag[XZETC_FLAG_ETOR] == 1 )
            {
               vl_ArgLectureETOR.varnam = vg_VarETOR.varnam;
               vl_ArgResultETOR.redres  = (STp)pg_ResultETOR;
               vl_ArgResultETOR.sizred  = vg_ConfigAdresse.LongueurETOR * sizeof(short);
               lm_rev ( 	vg_AssocApril.assoc, 
            			(XDY_Fonction)ea_lecture_automate,
            			(void *)(XZETC_ETOR),
            			&vl_ArgLectureETOR,
            			&vl_ArgResultETOR,
            			&vg_ErrApril );
               ea_gestion_erreur ( &vg_ErrApril, "lm_rev XZETC_ETOR" );
            }
            break;
      
         case XZETC_FLAG_SEQ_TUBE :
            XZST_03EcritureTrace( XZSTC_INFO,
            			 "Flag Sequence tube = %d . ", 
            			 pa_AstParam->pLmoree->evtval.aShort );
            /* Si passage a zero verifier si commande en attente */
            if ( vg_TabFlag[XZETC_FLAG_SEQ_TUBE] == 0 )
            {
               ea_gestion_cmd ( );
            }
            break;
      
         case XZETC_FLAG_SEQ_ECH :
            XZST_03EcritureTrace( XZSTC_INFO,
            			 "Flag Sequence echangeur = %d . ", 
            			 pa_AstParam->pLmoree->evtval.aShort );
            /* Si passage a zero verifier si commande en attente */
            if ( vg_TabFlag[XZETC_FLAG_SEQ_ECH] == 0 )
            {
               ea_gestion_cmd ( );
            }
            break;
      
         case XZETC_FLAG_TELECOMMANDE :
            XZST_03EcritureTrace( XZSTC_INFO,
            			 "Flag Telecommande = %d . ", 
            			 pa_AstParam->pLmoree->evtval.aShort );
            /* Si passage a zero verifier si commande en attente */
            if ( vg_TabFlag[XZETC_FLAG_TELECOMMANDE] == 0 )
            {
               ea_gestion_cmd ( );
            }
            break;
            
         default : 
            XZST_03EcritureTrace( XZSTC_WARNING, "Evenement AprilNet incorrect sur %d  valeur:%d ",  
            					pa_AstParam->pLmoree->unitin,
            					pa_AstParam->pLmoree->evtval.aShort );
            break;
      }

      /*A Relance de l'ecoute d'evenement */
      lm_ree ( (XDY_Fonction)ea_traitement_April, 
            		pa_AstParam,
              	 	pa_AstParam->Evenement, 
            		pa_AstParam->pLmoree,
            		pa_AstParam->pErrBlk );
      ea_gestion_erreur ( &vg_ErrApril, "lm_ree lancement evenement " );   }
   else
   {
   }
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Traitement des info recu de AprilNet
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void ea_init_donnee_April ( void )

/*
* ARGUMENTS EN ENTREE :
*   
* EXA0_AST_PARAM *	pa_AstParam  : structure evenements AprilNet
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
*
* CONDITION D'UTILISATION
* execute sur evenements AprilNet
*
* FONCTION 
*
*  Recherche du flag evenement et appel de la fonction de lecture automate
*
------------------------------------------------------*/
{
   static char *version = "$Id: exa0_tra.c,v 1.12 1997/06/25 12:49:48 gesconf Exp $ : ea_init_donnee_April " ;
  
   static lmirevT vl_ArgLectureTELE = { NULL, LOCAL_VAR, 0, 0, ASYNCHRONOUS, 0};
   static lmorevT vl_ArgResultTELE;
   static lmirevT vl_ArgLectureSIG = { NULL, LOCAL_VAR, 0, 0, ASYNCHRONOUS, 0};
   static lmorevT vl_ArgResultSIG;
   static lmirevT vl_ArgLectureITOR = { NULL, LOCAL_VAR, 0, 0, ASYNCHRONOUS, 0};
   static lmorevT vl_ArgResultITOR;
   static lmirevT vl_ArgLectureETOR = { NULL, LOCAL_VAR, 0, 0, ASYNCHRONOUS, 0};
   static lmorevT vl_ArgResultETOR;
   static lmirevT vl_ArgLectureALER = { NULL, LOCAL_VAR, 0, 0, ASYNCHRONOUS, 0};
   static lmorevT vl_ArgResultALER;
	 
   /*A
    *   
    */
         /* XZETC_FLAG_TELEALARME */
               vl_ArgLectureTELE.varnam = vg_VarTelealarme.varnam;
               vl_ArgResultTELE.redres  = (STp)pg_ResultTelealarme;
               vl_ArgResultTELE.sizred  = vg_ConfigAdresse.LongTelealarme * sizeof(short);
               lm_rev ( 	vg_AssocApril.assoc, 
            		   	(XDY_Fonction)ea_lecture_automate,
            			(void *)(XZETC_TELEALARME),
            			&vl_ArgLectureTELE,
            			&vl_ArgResultTELE,
            			&vg_ErrApril );
          
         /* XZETC_FLAG_SIGNALISATION */
               vl_ArgLectureSIG.varnam = vg_VarSignalisation.varnam;
               vl_ArgResultSIG.redres  = (STp)pg_ResultSignalisation;
               vl_ArgResultSIG.sizred  = vg_ConfigAdresse.LongTelesignTub * sizeof(short);
               lm_rev ( 	vg_AssocApril.assoc, 
            			(XDY_Fonction)ea_lecture_automate,
            			(void *)(XZETC_PANNEAU),
            			&vl_ArgLectureSIG,
            			&vl_ArgResultSIG,
            			&vg_ErrApril );
      
         /* XZETC_FLAG_ITOR */
            /* Demande de lecture dans l'automate des etats ITOR */
               vl_ArgLectureITOR.varnam = vg_VarITOR.varnam;
               vl_ArgResultITOR.redres  = (STp)pg_ResultITOR;
               vl_ArgResultITOR.sizred  = vg_ConfigAdresse.LongueurITOR * sizeof(short);
               lm_rev ( 	vg_AssocApril.assoc, 
            			(XDY_Fonction)ea_lecture_automate,
            			(void *)(XZETC_DISCORDANCE),
            			&vl_ArgLectureITOR,
            			&vl_ArgResultITOR,
            			&vg_ErrApril );
      
         /* XZETC_FLAG_ETOR */
               vl_ArgLectureETOR.varnam = vg_VarETOR.varnam;
               vl_ArgResultETOR.redres  = (STp)pg_ResultETOR;
               vl_ArgResultETOR.sizred  = vg_ConfigAdresse.LongueurETOR * sizeof(short);
               lm_rev ( 	vg_AssocApril.assoc, 
            			(XDY_Fonction)ea_lecture_automate,
            			(void *)(XZETC_ETOR),
            			&vl_ArgLectureETOR,
            			&vl_ArgResultETOR,
            			&vg_ErrApril );
      
         /* XZETC_FLAG_ALERTE */
               vl_ArgLectureALER.varnam = vg_VarAlerte.varnam;
               vl_ArgResultALER.redres  = (STp)pg_ResultAlerte;
               vl_ArgResultALER.sizred  = vg_ConfigAdresse.LongAlerteTub * sizeof(short);
               lm_rev ( 	vg_AssocApril.assoc, 
            			(XDY_Fonction)ea_lecture_automate,
            			(void *)(XZETC_ALERTE),
            			&vl_ArgLectureALER,
            			&vl_ArgResultALER,
            			&vg_ErrApril );

      vg_TabFlag[XZETC_FLAG_TELECOMMANDE] 	= 0;
      vg_TabFlag[XZETC_FLAG_SEQ_TUBE] 		= 0;
      vg_TabFlag[XZETC_FLAG_SEQ_ECH] 		= 0;
      ea_gestion_cmd ();
      ea_gestion_cmd ();
      ea_gestion_cmd ();


}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Traitement des info recu de AprilNet
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void ea_init_Flag_April ( void )

/*
* ARGUMENTS EN ENTREE :
*   
*  aucun
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
*
* CONDITION D'UTILISATION
* execute sur reprise apres coupure connexion API5000
*
* FONCTION 
*
*  RAZ des flags API5000
*
------------------------------------------------------*/
{
   static char *version = "$Id: exa0_tra.c,v 1.12 1997/06/25 12:49:48 gesconf Exp $ : ea_init_Flag_April" ;

      /*A RAZ Flag Telealarame */
         ea_ecriture_flag ( XZETC_ADRESSE_FLAG + XZETC_FLAG_TELEALARME , 0 );
            
      /*A RAZ Flag ETOR */
         ea_ecriture_flag ( XZETC_ADRESSE_FLAG + XZETC_FLAG_ETOR , 0 );
               
      /*A RAZ Flag discordance   */
         ea_ecriture_flag ( XZETC_ADRESSE_FLAG + XZETC_FLAG_ITOR , 0 );
            
      /*A RAZ Flag etat panneau */
         ea_ecriture_flag ( XZETC_ADRESSE_FLAG + XZETC_FLAG_SIGNALISATION , 0 );

      /*A RAZ Flag alerte   */
         ea_ecriture_flag ( XZETC_ADRESSE_FLAG + XZETC_FLAG_ALERTE , 0 );

}
