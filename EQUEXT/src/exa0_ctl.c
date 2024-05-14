/*E*/
/*Fichier : $Id: exa0_ctl.c,v 1.7 1997/05/06 19:54:36 mismer Exp $      Release : $Revision: 1.7 $        Date : $Date: 1997/05/06 19:54:36 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEXA0 * FICHIER exa0_ctl.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Module de controle de la tache TEXA0, protocole de la gtc Nice
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	22 Nov 1994	: Creation
* Mismer.D	version 1.2	09 Fev 1995	:
* Fontaine.C	version 1.3	23 Mai 1995	: Reprise entete, historique
* Mismer.D	version 1.4	16 Aou 1995	: Changement niveau trace succes action AprilNet
* Mismer.D	version 1.5	25 Aou 1995	: Modif gestion liaison AprilNet sur echec
* Mismer.D	version 1.6	27 Oct 1995	: Modif sortie tache sur panne liaison AprilNet
* Mismer.D      version 1.7     24 Avr 1997     : Ajout alerte monaco (DEM/1421)
*
------------------------------------------------------*/

/* fichiers inclus */

#include "exa0_ctl.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: exa0_ctl.c,v 1.7 1997/05/06 19:54:36 mismer Exp $ : exa0_ctl" ;

static EXA0_DESCRI_VARIABLE 	vg_DescriTelealarme;
static EXA0_DESCRI_VARIABLE 	vg_DescriSignalisation;
static EXA0_DESCRI_VARIABLE 	vg_DescriETOR;
static EXA0_DESCRI_VARIABLE 	vg_DescriITOR;
static EXA0_DESCRI_VARIABLE 	vg_DescriAlerte;

static EXA0_NOM_VARIABLE 	NomTelealarme 		= { "Telealarme" };
static EXA0_NOM_VARIABLE 	NomSignalisation 	= { "Signalisation" };
static EXA0_NOM_VARIABLE 	NomETOR 		= { "ETOR" };
static EXA0_NOM_VARIABLE 	NomITOR 		= { "ITOR" };
static EXA0_NOM_VARIABLE 	NomAlerte 		= { "Alerte" };

lmidevT vg_VarTelealarme	= { NomTelealarme, 	vg_DescriTelealarme };
lmidevT vg_VarSignalisation	= { NomSignalisation, 	vg_DescriSignalisation };
lmidevT vg_VarETOR		= { NomETOR, 		vg_DescriETOR };
lmidevT vg_VarITOR		= { NomITOR, 		vg_DescriITOR };
lmidevT vg_VarAlerte		= { NomAlerte, 		vg_DescriAlerte };

/* declaration de fonctions internes */

/* definition de fonctions externes */




/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Effectue l'initialisation de la tache
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	ea_init	( 	XDY_NomMachine  	pa_NomMachine,
			XDY_NomTache 		pa_NomTache  )

/*
* ARGUMENTS EN ENTREE :
*
*   pa_NomMachine	: Nom de la machine
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
   static char *version = "$Id: exa0_ctl.c,v 1.7 1997/05/06 19:54:36 mismer Exp $ : ea_init" ;
   
   static char		vl_DescriEvt[32];
   static lmiinaT 	vl_Service = { 0, ODSSER | ODISER | REVSER | WRVSER | DEESER | DLESER | REESER };
   				 
   static evtdesT 	vl_CondEvt	= { 0, 0 };
   static lmideeT 	vl_VarEvt	= { vl_DescriEvt, LOCAL_VAR, 0, 0, &vl_CondEvt };
   static lmireeT 	vl_BlkCtrl	= { 0, NOPURGE };
   static lmoreeT 	vl_CtrlSortie;
   static EXA0_AST_PARAM vl_Evenement   = { &vg_ErrApril, &vl_BlkCtrl, &vl_CtrlSortie, &vl_VarEvt.varnam };				 



   /*A
    * Connexion avec AprilNet Manager 
    */
   /*A Enregistrement aupres des couches ISO */ 
   lm_con ( "/produits/migrazur/appliSD/traces/AprilDebug", &vg_SockLms1, &vg_ErrApril );
   ea_gestion_erreur ( &vg_ErrApril, "lm_con Connexion AprilNet Manager" );
   if ( vg_Etat_AprilNet != XDC_OK )
   {
      vg_SockLms1[0].fd = 0;
      return;
   }

   /*A
    * Lecture des fichiers de configuration et remplissage des structure de donnees 
    */
   /*A Configuration des tubes et echangeurs (appel de la fonction ea_lire_config_GTCNice) */
   ea_lire_config_GTCNice ( pa_NomMachine );
 
   /*A
    * Ouverture association avec AprilNet Manager 
    */
   lm_ina ( 	vg_ConfigAdresse.NomServeur, 
   		0, 
   		0, 
   		&vl_Service, 
   		&vg_AssocApril, 
   		&vg_ErrApril );
   ea_gestion_erreur ( &vg_ErrApril, "lm_ina Ouverture association" );
   if ( vg_Etat_AprilNet != XDC_OK )
   {
      vg_SockLms1[0].fd = 0;
      return;
   }
   else
   {
      ex_ecriture_trace_cmd ( XDC_EQT_NIC, 0, "Connection coupleur AprilNet ", NULL, 0 );
   }


   /*A
    * Definition des variables AprilNet  
    */

   /* Declaration variable Telealarme */
   sprintf ( vg_DescriTelealarme, "%%MW%d:%d", 
   				vg_ConfigAdresse.AdresseTelealarme, 
   				vg_ConfigAdresse.LongTelealarme );
   lm_dev ( vg_AssocApril.assoc, &vg_VarTelealarme, &vg_ErrApril );
   ea_gestion_erreur ( &vg_ErrApril, "lm_dev definition variable Telealarme" );

   /* Declaration variable Signalisation */
   sprintf ( vg_DescriSignalisation, "%%MW%d:%d", 
   				vg_ConfigAdresse.AdresseTelesignTub, 
   				vg_ConfigAdresse.LongTelesignTub );
   lm_dev ( vg_AssocApril.assoc, &vg_VarSignalisation, &vg_ErrApril );
   ea_gestion_erreur ( &vg_ErrApril, "lm_dev definition variable Signalisation " );

   /* Declaration variable ETOR */
   sprintf ( vg_DescriETOR, "%%MW%d:%d", 
   				vg_ConfigAdresse.AdresseETOR, 
   				vg_ConfigAdresse.LongueurETOR );
   lm_dev ( vg_AssocApril.assoc, &vg_VarETOR, &vg_ErrApril );
   ea_gestion_erreur ( &vg_ErrApril, "lm_dev definition variable ETOR " );


   /* Declaration variable ITOR */
   sprintf ( vg_DescriITOR, "%%MW%d:%d", 
   				vg_ConfigAdresse.AdresseITOR, 
   				vg_ConfigAdresse.LongueurITOR );
   lm_dev ( vg_AssocApril.assoc, &vg_VarITOR, &vg_ErrApril );
   ea_gestion_erreur ( &vg_ErrApril, "lm_dev definition variable ITOR " );


   /* Declaration variable Alerte */
   sprintf ( vg_DescriAlerte, "%%MW%d:%d", 
   				vg_ConfigAdresse.AdresseAlerteTub, 
   				vg_ConfigAdresse.LongAlerteTub );
   lm_dev ( vg_AssocApril.assoc, &vg_VarAlerte, &vg_ErrApril );
   ea_gestion_erreur ( &vg_ErrApril, "lm_dev definition variable Alerte " );


   /*A Definition evenement AprilNet */
   sprintf ( vl_DescriEvt, "%%MW%d:%d", XZETC_ADRESSE_FLAG, XZETC_TAILLE_FLAG );
   lm_dee ( vg_AssocApril.assoc, 
            0, 
            0, 
            &vl_VarEvt, 
            &vl_BlkCtrl.evtId, 
            &vg_ErrApril );
   ea_gestion_erreur ( &vg_ErrApril, "lm_dee definition evenement " );

   /*A Lancement de l'ecoute d'evenement */
   lm_ree ( (XDY_Fonction)ea_traitement_April, 
            &vl_Evenement, 
            &vl_BlkCtrl, 
            &vl_CtrlSortie, 
            &vg_ErrApril );
   ea_gestion_erreur ( &vg_ErrApril, "lm_ree lancement evenement " );

   /*A
    * Allocation memoire pour les tables de lecture automate  
    */
   pg_Telealarme	  = (short *) malloc (  vg_ConfigAdresse.LongTelealarme	 * sizeof(short) );
   memset ( pg_Telealarme, 		0, 	vg_ConfigAdresse.LongTelealarme	 * sizeof(short) );
   pg_ResultTelealarme	  = (short *) malloc (  vg_ConfigAdresse.LongTelealarme  * sizeof(short) );
   memset ( pg_ResultTelealarme, 	0,	vg_ConfigAdresse.LongTelealarme	 * sizeof(short) );
   pg_Signalisation	  = (short *) malloc (  vg_ConfigAdresse.LongTelesignTub * sizeof(short) );
   memset ( pg_Signalisation, 		0,	vg_ConfigAdresse.LongTelesignTub * sizeof(short) );
   pg_ResultSignalisation = (short *) malloc ( 	vg_ConfigAdresse.LongTelesignTub * sizeof(short) );
   memset ( pg_ResultSignalisation, 	0,	vg_ConfigAdresse.LongTelesignTub * sizeof(short) );
   pg_ETOR		  = (short *) malloc ( 	vg_ConfigAdresse.LongueurETOR 	 * sizeof(short) );
   memset ( pg_ETOR, 			0,	vg_ConfigAdresse.LongueurETOR	 * sizeof(short) );
   pg_ResultETOR	  = (short *) malloc ( 	vg_ConfigAdresse.LongueurETOR 	 * sizeof(short) );
   memset ( pg_ResultETOR, 		0,	vg_ConfigAdresse.LongueurETOR	 * sizeof(short) );
   pg_ITOR		  = (short *) malloc ( 	vg_ConfigAdresse.LongueurITOR 	 * sizeof(short) );
   memset ( pg_ITOR, 			0,	vg_ConfigAdresse.LongueurITOR	 * sizeof(short) );
   pg_ResultITOR	  = (short *) malloc ( 	vg_ConfigAdresse.LongueurITOR 	 * sizeof(short) );
   memset ( pg_ResultITOR, 		0,	vg_ConfigAdresse.LongueurITOR 	 * sizeof(short) );
   pg_Alerte		  = (short *) malloc ( 	vg_ConfigAdresse.LongAlerteTub	 * sizeof(short) );
   memset ( pg_Alerte, 			0,	vg_ConfigAdresse.LongAlerteTub	 * sizeof(short) );
   pg_ResultAlerte	  = (short *) malloc ( 	vg_ConfigAdresse.LongAlerteTub	 * sizeof(short) );
   memset ( pg_ResultAlerte, 		0,	vg_ConfigAdresse.LongAlerteTub	 * sizeof(short) );
   
   if ( pa_NomTache != NULL )
   {
      if( XZSC_InitCnxSup(  	NULL,
				pa_NomTache,
				XZSCC_INTERDIRE,
				XZSCC_AUTORISER,
				XZSCC_INTERDIRE,
				ea_sortir,
				"/dev/null" ) != XDC_OK )
      {
         /*B Affichage Trace */
         XZST_03EcritureTrace( XZSTC_FATAL, "Ne peut ouvrir la connexion avec le RTserver." );
      }
      else
      {
        /*B Affichage Trace */
        XZST_03EcritureTrace( XZSTC_DEBUG1, "Ouverture de la connexion avec le RTserver." );
        XZST_03EcritureTrace( XZSTC_WARNING, "Ouverture de la connexion avec le RTserver." );
      }
   }

}





/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Effectue les operation de terminaison de la tache
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	ea_sortir	(  )

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
   static char *version = "$Id: exa0_ctl.c,v 1.7 1997/05/06 19:54:36 mismer Exp $ : ea_sortir" ;

   EXA0_DONNEES_Tube		*pl_DonCrtTube,
   				*pl_DonSupTube;
   EXA0_DONNEES_Echangeur	*pl_DonCrtEch,
   				*pl_DonSupEch;
   int				vl_SockApril = vg_SockApril;

  /*A
   * Liberation des ressources memoires
   */
   
      /*A Liberation des ressources memoires des donnees Tube */
   
      for ( pl_DonCrtTube = pl_DonSupTube = pg_debutTube;
            pl_DonCrtTube != NULL; 
            pl_DonSupTube = pl_DonCrtTube )
      {
      	 /* Liberation memoire */
         pl_DonCrtTube = pl_DonCrtTube->Suivant;
         free ( pl_DonSupTube );
      }
      pg_debutTube = NULL;
 
      /*A Suppression des structures de donnees Echangeur */
      for ( pl_DonCrtEch = pl_DonSupEch = pg_debutEchangeur;
            pl_DonCrtEch != NULL; 
            pl_DonSupEch = pl_DonCrtEch )
      {
      	 /* Liberation memoire */
         pl_DonCrtEch = pl_DonCrtEch->Suivant;
         free ( pl_DonSupEch );
      }
      pg_debutEchangeur = NULL;
      
	
  /*A
   * Deconnection de AprilNet Manager
   */
      /*A Fermeture de l'association avec le coupleur AprilNet  */
      lm_cla ( vg_AssocApril.assoc, 0, 0, &vg_ErrApril );
      ea_gestion_erreur ( &vg_ErrApril, "lm_cla" );

      /*A Desenregistrement des couches ISO  */
      lm_dec ( &vg_ErrApril );
      ea_gestion_erreur ( &vg_ErrApril, "lm_dec"  );
      

 /*A
  * Liberation memoire des tables de lecture automate  
  */
   free ( pg_Telealarme );
   free ( pg_ResultTelealarme );
   free ( pg_Signalisation );
   free ( pg_ResultSignalisation );
   free ( pg_ETOR );
   free ( pg_ResultETOR );
   free ( pg_ITOR );
   free ( pg_ResultITOR );
   free ( pg_Alerte );
   free ( pg_ResultAlerte );
   
   /* Fin de la tache */
   if ( vl_SockApril != 0 )
     exit (0);
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
* Trace des erreur AprilNet
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void ea_gestion_erreur ( errblT *	pa_ErrBlk,
			 char *		pa_Msg )

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
* FONCTION 
*
*
------------------------------------------------------*/
{   
   static char *version = "$Id: exa0_ctl.c,v 1.7 1997/05/06 19:54:36 mismer Exp $ : ea_gestion_erreur " ;

   switch ( pa_ErrBlk->class )
   {
      case  NOINOUT :      
         XZST_03EcritureTrace( XZSTC_WARNING, 
         			"Erreur AprilNet:%s sur fonct:%s Argument d'entree incorrect.",
         			lm_msg(&vg_ErrApril), pa_Msg );
         break;
      case  NOERRBLK :      
         XZST_03EcritureTrace( XZSTC_WARNING, 
         			"Erreur AprilNet:%s sur fonct:%s Argument retour erreur incorrect.",
         			lm_msg(&vg_ErrApril), pa_Msg );
         break;
      case REJECT  :      
         XZST_03EcritureTrace( XZSTC_WARNING, 
         			"Erreur AprilNet:%s sur fonct:%s le coupleur ne peut decoder la requete.",
         			lm_msg(&vg_ErrApril), pa_Msg );
         break;
      case  ABORT :
         vg_Etat_AprilNet = XDC_NOK;
         switch ( pa_ErrBlk->code )
         {
         case  REMOTE :      
            XZST_03EcritureTrace( XZSTC_WARNING, 
         			"Erreur AprilNet:%s sur fonct:%s Demande annulation coupleur.",
         			lm_msg(&vg_ErrApril), pa_Msg );
            break;
         case LOCAL  :      
            XZST_03EcritureTrace( XZSTC_WARNING, 
         			"Erreur AprilNet:%s sur fonct:%s  la librairie a annule l'association.",
         			lm_msg(&vg_ErrApril), pa_Msg );
            break;
         default :
            XZST_03EcritureTrace( XZSTC_WARNING, "Erreur AprilNet:%s sur fonct:%s .",
         			lm_msg(&vg_ErrApril), pa_Msg );
            break;
         }
         break;
      case  SERVICE :
         switch ( pa_ErrBlk->code )
         {
         case UNNEGO_SERV  :      
            XZST_03EcritureTrace( XZSTC_WARNING, 
         			"Erreur AprilNet:%s sur fonct:%s  service refuse a l'ouverture de l'association.",
         			lm_msg(&vg_ErrApril), pa_Msg );
            break;
         case DECODING  :      
            XZST_03EcritureTrace( XZSTC_WARNING, 
         			"Erreur AprilNet:%s sur fonct:%s  erreur de decodage de la reponse du coupleur.",
         			lm_msg(&vg_ErrApril), pa_Msg );
            break;
         case  TIME_OUT :      
            XZST_03EcritureTrace( XZSTC_WARNING, 
         			"Erreur AprilNet:%s sur fonct:%s  pas de reponse du coupleur dans les 30 secondes.",
         			lm_msg(&vg_ErrApril), pa_Msg );
            break;
         default :
            XZST_03EcritureTrace( XZSTC_WARNING, "Erreur AprilNet:%s sur fonct:%s .",
         			lm_msg(&vg_ErrApril), pa_Msg );
            break;
         }
         break;
      case  JBUS_SUBNETWK :
         switch ( pa_ErrBlk->code )
         {
         case INV_ADDRESS  :      
            XZST_03EcritureTrace( XZSTC_WARNING, 
         			"Erreur AprilNet:%s sur fonct:%s Adresse JBus incorrecte.",
         			lm_msg(&vg_ErrApril), pa_Msg );
            break;
         default :
            XZST_03EcritureTrace( XZSTC_WARNING, "Erreur AprilNet:%s sur fonct:%s .",
         			lm_msg(&vg_ErrApril), pa_Msg );
            break;
         }
         break;
      case PARAMETER :
         switch ( pa_ErrBlk->code )
         {
         case INVALID_MNEMO  :      
            XZST_03EcritureTrace( XZSTC_WARNING, 
         			"Erreur AprilNet:%s sur fonct:%s Syntaxe mnemonique incorrecte.",
         			lm_msg(&vg_ErrApril), pa_Msg );
            break;
         case  SHORT_BUF :      
            XZST_03EcritureTrace( XZSTC_WARNING, 
         			"Erreur AprilNet:%s sur fonct:%s buffer de donnee trop petit.",
         			lm_msg(&vg_ErrApril), pa_Msg );
            break;
         case  NIL_VALUE :      
            XZST_03EcritureTrace( XZSTC_WARNING, 
         			"Erreur AprilNet:%s sur fonct:%s pas de description de variable.",
         			lm_msg(&vg_ErrApril), pa_Msg );
            break;
         default :
            XZST_03EcritureTrace( XZSTC_WARNING, "Erreur AprilNet:%s sur fonct:%s .",
         			lm_msg(&vg_ErrApril), pa_Msg );
            break;
         }
         break;
      case OBJECT :
         switch ( pa_ErrBlk->code )
         {
         case OBJECTUNEXISTANT  :      
            XZST_03EcritureTrace( XZSTC_WARNING, 
         			"Erreur AprilNet:%s sur fonct:%s Variable donnee inexistante.",
         			lm_msg(&vg_ErrApril), pa_Msg );
            break;
         default :
            XZST_03EcritureTrace( XZSTC_WARNING, "Erreur AprilNet:%s sur fonct:%s .",
         			lm_msg(&vg_ErrApril), pa_Msg );
            break;
         }
         break;
      case ACCESS :
         switch ( pa_ErrBlk->code )
         {
         case  NOASSOCIATION :      
            vg_Etat_AprilNet = XDC_NOK;
            XZST_03EcritureTrace( XZSTC_WARNING, 
         			"Erreur AprilNet:%s sur fonct:%s Identificateur association invalide.",
         			lm_msg(&vg_ErrApril), pa_Msg );
           break;
         default :
            XZST_03EcritureTrace( XZSTC_WARNING, "Erreur AprilNet:%s sur fonct:%s .",
         			lm_msg(&vg_ErrApril), pa_Msg );
            break;
         }
         break;
      case RESOURCE :
         switch ( pa_ErrBlk->code )
         {
         case UNSU_MEM  :      
            XZST_03EcritureTrace( XZSTC_WARNING, 
         			"Erreur AprilNet:%s sur fonct:%s  insuffisance memoire.",
         			lm_msg(&vg_ErrApril), pa_Msg );
            break;
         default :
            XZST_03EcritureTrace( XZSTC_WARNING, "Erreur AprilNet:%s sur fonct:%s .",
         			lm_msg(&vg_ErrApril), pa_Msg );
            break;
         }
         break;
      case LM_SERVICE :
         switch ( pa_ErrBlk->code )
         {
         case  SER_OBJCONSTCONFLICT :      
            XZST_03EcritureTrace( XZSTC_WARNING, 
         			"Erreur AprilNet:%s sur fonct:%s  coupleur protege pour cette commande.",
         			lm_msg(&vg_ErrApril), pa_Msg );
            break;
         case SER_PRIMOUTSEQUENCE  :      
            XZST_03EcritureTrace( XZSTC_WARNING, 
         			"Erreur AprilNet:%s sur fonct:%s  l'automate refuse un etat ou identificateur d'assoc inconnu sur le coupleur.",
         			lm_msg(&vg_ErrApril), pa_Msg );
            break;
         default :
            XZST_03EcritureTrace( XZSTC_WARNING, "Erreur AprilNet:%s sur fonct:%s .",
         			lm_msg(&vg_ErrApril), pa_Msg );
            break;
         }
         break;
      case LM_RESOURCE :
         switch ( pa_ErrBlk->code )
         {
         case RES_MEMUNAVAILABLE  :      
            XZST_03EcritureTrace( XZSTC_WARNING, 
         			"Erreur AprilNet:%s sur fonct:%s  Trop d'octets a ecrire ou lire (1100 max) ou depassement memoire.",
         			lm_msg(&vg_ErrApril), pa_Msg );
            break;
         default :
            XZST_03EcritureTrace( XZSTC_WARNING, "Erreur AprilNet:%s sur fonct:%s .",
         			lm_msg(&vg_ErrApril), pa_Msg );
            break;
         }
         break;
      case DEFINITION :
         switch ( pa_ErrBlk->code )
         {
         case DEF_TYPEUNSUPPORTED  :      
            XZST_03EcritureTrace( XZSTC_WARNING, 
         			"Erreur AprilNet:%s sur fonct:%s  variable AprilNet invalide.",
         			lm_msg(&vg_ErrApril), pa_Msg );
            break;
         case DEF_TYPEINCONSISTENT  :      
            XZST_03EcritureTrace( XZSTC_WARNING, 
         			"Erreur AprilNet:%s sur fonct:%s  les parametre de mise a jour de la date de l'automate incorrect.",
         			lm_msg(&vg_ErrApril), pa_Msg );
           break;
         default :
            XZST_03EcritureTrace( XZSTC_WARNING, "Erreur AprilNet:%s sur fonct:%s .",
         			lm_msg(&vg_ErrApril), pa_Msg );
            break;
         }
         break;
      case LM_ACCESS :
         switch ( pa_ErrBlk->code )
         {
         case ACC_OBJACCDENIED  :      
            XZST_03EcritureTrace( XZSTC_WARNING, 
         			"Erreur AprilNet:%s sur fonct:%s  l'horadateur de l'automate refuse la mise a jour.",
         			lm_msg(&vg_ErrApril), pa_Msg );
            break;
         default :
            XZST_03EcritureTrace( XZSTC_WARNING, "Erreur AprilNet:%s sur fonct:%s .",
         			lm_msg(&vg_ErrApril), pa_Msg );
            break;
         }
      default :
         if ( strcmp ( lm_msg(&vg_ErrApril), "(SUCESS,NOCOD)" ) )
         {
            XZST_03EcritureTrace( XZSTC_WARNING, "Erreur AprilNet:%s sur fonct:%s .",
         			lm_msg(&vg_ErrApril), pa_Msg );
            if ( !strcmp ( lm_msg(&vg_ErrApril), "(INITIATE,INVALID_COM)" ) )
               vg_Etat_AprilNet = XDC_NOK;
         }
         else
         {
            XZST_03EcritureTrace( XZSTC_MESSAGE, "Reussite action AprilNet:%s .", pa_Msg );
            vg_Etat_AprilNet = XDC_OK;
         }
         break;
   }
   
   
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
* Compte rendu d'ecriture automate
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void ea_ecriture_automate ( char *	pa_CompteRendu )

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
* FONCTION 
*
*
------------------------------------------------------*/
{   
   static char *version = "$Id: exa0_ctl.c,v 1.7 1997/05/06 19:54:36 mismer Exp $ : ea_ecriture_automate " ;
   
         XZST_03EcritureTrace( XZSTC_MESSAGE, "Reussite action AprilNet d'ecriture automate : %s ", pa_CompteRendu );

}



