/*E*/
/*Fichier : $Id: exa0_cmd.c,v 1.5 1995/08/25 10:44:51 volcic Exp $      Release : $Revision: 1.5 $        Date : $Date: 1995/08/25 10:44:51 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEXA0 * FICHIER exa0_cmd.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module des fonctions de commande de la tache TEXA0,
* protocole de la gtc Nice
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	23 Nov 1994	: Creation
* Mismer.D	version 1.2	09 Fev 1995	:
* Mismer.D	version 1.3	10 Mai 1995	:
* Mismer.D	version 1.4	18 Mai 1995	: Ajout traces pour test, reprise entete, historique
* Mismer.D	version 1.5	16 Aou 1995	: Modif niveau traces pour test
*
------------------------------------------------------*/

/* fichiers inclus */

#include "exa0_cmd.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: exa0_cmd.c,v 1.5 1995/08/25 10:44:51 volcic Exp $ : exa0_cmd" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Traitement des info de la tache traitement TENIC
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void ea_traitement_cmd ( int	va_NumSock )

/*
* ARGUMENTS EN ENTREE :
*   
* int	va_NumSock	: Numero de la socket TENIC
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
*
* CONDITION D'UTILISATION
* execute sur evenement socket TENIC
*
* FONCTION 
*
*
------------------------------------------------------*/
{
   static char *version = "$Id: exa0_cmd.c,v 1.5 1995/08/25 10:44:51 volcic Exp $ : ea_traitement_cmd " ;
   
   int			vl_IndCnx = 0,
   			vl_LgMsg  = 0,
   			vl_Ind    = 0,
   			vl_Index  = 0,
   			vl_jourSemaine;
   XZEXT_MSG_SOCKET 	pl_Msg;
   static EXA0_ENG_CMD	vl_EngCmd;
   char	*	 	vl_Mot;
   
   /*A
    *  Lecture du message recu dans la socket appelante
    */
	 
   if ( ex_lire_msg ( va_NumSock, pl_Msg, &vl_LgMsg, &vg_SockMask, &vl_IndCnx ) != XDC_OK )
   {
      /* Ecriture trace Erreur lecture message socket. */
      XZST_03EcritureTrace( XZSTC_WARNING, " Erreur lecture message socket. " );
      return;
   }

   if ( (vl_IndCnx == XZEXC_DECONNEC) || ( strstr(pl_Msg, XZEXC_DEMANDE_ARRET) != NULL) )
   {
      /* Ecriture trace . */
      XZST_03EcritureTrace( XZSTC_WARNING, " Fin de la tache. " );
      ea_sortir();
   }

   if ( strstr(pl_Msg, XZEXC_DEMANDE_INIT) != NULL )
   {
      ea_init_donnee_April();
      return;
   }

   /*A Si il s'agit d'une deconnexion */
   if ( vl_LgMsg == 0 )
   {
      /* Ecriture trace . */
      XZST_03EcritureTrace( XZSTC_WARNING, " Erreur lecture message socket. " );
      return;
   }
  
   /*A Scan du message provenant de la socket */
   if ( sscanf ( pl_Msg, "%d %d %d ", &vl_EngCmd.Type, &vl_EngCmd.Adresse, &vl_EngCmd.NbrCmd ) != 3 )
   {
      /* Ecriture trace Erreur */
      XZST_03EcritureTrace( XZSTC_WARNING, " Erreur syntaxe message de TENIC: %s ", pl_Msg );
      return;
   }
   
   
   XZST_03EcritureTrace( XZSTC_MESSAGE, " Message de TENIC: %s ", pl_Msg );
   vl_Ind = sprintf ( vl_EngCmd.Msg, "%d ", vl_EngCmd.Adresse ); 
   
   /* Lecture des etats panneaux */
   vl_Mot = strtok ( pl_Msg, " ");
   vl_Mot = strtok ( NULL, " " );
   vl_Mot = strtok ( NULL, " " );
   for ( vl_Index = 0; 
         (vl_Index < vl_EngCmd.NbrCmd) && ( vl_Mot != NULL );
         vl_Index++ )
   {
      if ( ( vl_Mot = strtok ( NULL, " " ) ) != NULL )
      {
         vl_Mot += 2;
         vl_EngCmd.TabCmd[vl_Index] = 	((*(vl_Mot+0)<'A') ? (*(vl_Mot+0)-'0') : (*(vl_Mot+0)-'A'+10)) * 0x1000 +
         				((*(vl_Mot+1)<'A') ? (*(vl_Mot+1)-'0') : (*(vl_Mot+1)-'A'+10)) * 0x0100 +
         				((*(vl_Mot+2)<'A') ? (*(vl_Mot+2)-'0') : (*(vl_Mot+2)-'A'+10)) * 0x0010 +
         				((*(vl_Mot+3)<'A') ? (*(vl_Mot+3)-'0') : (*(vl_Mot+3)-'A'+10)) * 0x0001 ;
         vl_Ind += sprintf ( vl_EngCmd.Msg+vl_Ind, " 0x%s", vl_Mot ); 
      }
   }
   
   /* Envoyer la commande a l'automate */
   if ( ea_ecriture_cmd ( vl_EngCmd ) != XDC_OK )
   {
      /* Si la commande ne peut pas etre envoyer */
      /*B Lecture de l'horodate actuelle */
      XZST_03EcritureTrace( XZSTC_MESSAGE, " Memorisation de : %s ", vl_EngCmd.Msg );
      XZSM_07LireHorodate ( XZSMC_TYPE_SYSTEME, &vl_jourSemaine, &vl_EngCmd.Horodate );
      ea_ins_liste_cmd ( &vl_EngCmd, 0 );
   }


   
}




/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
* Recherche si une commande a ete memorisee et peut etre envoyee
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void  ea_gestion_cmd ( )

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
   static char *version = "$Id: exa0_cmd.c,v 1.5 1995/08/25 10:44:51 volcic Exp $ : ea_gestion_cmd " ;
   
   EXA0_ENG_CMD	* 	pl_EngCmd;
   
   /* Recherche si une commande peut etre envoyer */
   for ( pl_EngCmd = pg_debutCmd; pl_EngCmd != NULL; pl_EngCmd = pl_EngCmd->Suivant )
   {
      /* Si la commande peut etre envoyer , supprimer la commande de la lisste */
      if ( ea_ecriture_cmd ( pl_EngCmd ) == XDC_OK )
      {
         XZST_03EcritureTrace( XZSTC_MESSAGE, " Sup memoire de : %s ", pl_EngCmd->Msg );
         ea_sup_liste_cmd ( pl_EngCmd->Horodate );
         break;
      }
   }
    
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
* Envoie d'une commande a l'automate l'automate
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int ea_ecriture_cmd (	EXA0_ENG_CMD va_Cmd )

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
   static char *version = "$Id: exa0_cmd.c,v 1.5 1995/08/25 10:44:51 volcic Exp $ : ea_ecriture_cmd " ;
   
   static	XDY_Texte	 vl_MsgEcriture;
   static	short		 vl_TabCmd[XZETC_MAX_PANNEAU/XZETC_NB_ETAT_PAN];
   static	char		 vl_DescriptionVar[32];
   static	lmidevT		 vl_VarEcriture = { "VarEcriture", vl_DescriptionVar };
   static	lmiwrvT		 vl_ValEcriture = { "VarEcriture", LOCAL_VAR, 0, 0, (STp)&vl_TabCmd, ASYNCHRONOUS, 0 };
   		int		 vl_Pos = 0,
   				 vl_Index = 0;

   /* verification de l'etat du flag pour ecriture */
   switch ( va_Cmd.Type )
   {
      case XZETC_CMD_PAN :
         if ( vg_TabFlag[XZETC_FLAG_TELECOMMANDE] == 1 )
           return (XDC_NOK);
         break;
   
      case XZETC_CMD_TUBE :
         if ( vg_TabFlag[XZETC_FLAG_SEQ_TUBE] == 1 )
           return (XDC_NOK);
         break;

      case XZETC_CMD_ECH :
         if ( vg_TabFlag[XZETC_FLAG_SEQ_ECH] == 1 )
           return (XDC_NOK);
         break;
   }


   /* Declaration variable pour ecriture automate */
   sprintf ( vl_DescriptionVar, "%%MW%d:%d", va_Cmd.Adresse, va_Cmd.NbrCmd );
   vl_Pos = sprintf ( vl_MsgEcriture, " %s ",  vl_DescriptionVar );
   for ( vl_Index = 0; vl_Index < va_Cmd.NbrCmd; vl_Index++ )
   {
      vl_Pos += sprintf ( vl_MsgEcriture+vl_Pos, "0x%04X ", va_Cmd.TabCmd[vl_Index] );
   }
   memcpy ( &vl_TabCmd, &va_Cmd.TabCmd, va_Cmd.NbrCmd*sizeof(short) );
   vl_VarEcriture.descri = vl_DescriptionVar;
   lm_dev ( vg_AssocApril.assoc, &vl_VarEcriture, &vg_ErrApril );
   ea_gestion_erreur ( &vg_ErrApril, "lm_dev ea_traitement_cmd " );
   
   ex_ecriture_trace_cmd ( XDC_EQT_NIC,
                           0, 
                           vl_MsgEcriture,
                           NULL,
                           0 );
   
   /* Ecriture commande dans l'automate */
   lm_wrv ( vg_AssocApril.assoc, 
            (XDY_Fonction)ea_ecriture_automate,
            (void *)vl_MsgEcriture,
            &vl_ValEcriture,
            &vg_ErrApril );
   ea_gestion_erreur ( &vg_ErrApril, "lm_wrv ea_traitement_cmd" );
   
   /* Effacement variable pour ecriture automate */
   lm_dlv ( vg_AssocApril.assoc, vl_VarEcriture.varnam, &vg_ErrApril );
   ea_gestion_erreur ( &vg_ErrApril, "lm_dlv ea_traitement_cmd" );
   
   
   /* Positionnement du flag d'ecriture */
   switch ( va_Cmd.Type )
   {
      case XZETC_CMD_PAN :
         ea_ecriture_flag ( XZETC_ADRESSE_FLAG + XZETC_FLAG_TELECOMMANDE , 1 );
         vg_TabFlag[XZETC_FLAG_TELECOMMANDE] = 1;
         break;
   
      case XZETC_CMD_TUBE :
         ea_ecriture_flag ( XZETC_ADRESSE_FLAG + XZETC_FLAG_SEQ_TUBE , 1 );
         vg_TabFlag[XZETC_FLAG_SEQ_TUBE] = 1;
         break;

      case XZETC_CMD_ECH :
         ea_ecriture_flag ( XZETC_ADRESSE_FLAG + XZETC_FLAG_SEQ_ECH , 1 );
         vg_TabFlag[XZETC_FLAG_SEQ_ECH] = 1;
         break;
   }
     
   return ( XDC_OK );
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
* Ecriture d
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void ea_ecriture_flag (	short	va_Adresse,
			short 	va_Valeur )

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
   static char *version = "$Id: exa0_cmd.c,v 1.5 1995/08/25 10:44:51 volcic Exp $ : ea_ecriture_flag " ;
   
   static	XDY_Texte	vl_MsgEcriture;
   static	short 	 	vl_Valeur[1];   
   static	char		vl_DescriptionVar[32];
   static	lmidevT		vl_VarEcriture = { "VarFlag", vl_DescriptionVar };
   static	lmiwrvT		vl_ValEcriture = { "VarFlag", LOCAL_VAR, 0, 0, (STp)&vl_Valeur, ASYNCHRONOUS, 0 };

   /* Declaration variable pour ecriture automate */
   sprintf ( vl_DescriptionVar, "%%MW%d:1", va_Adresse );
   sprintf ( vl_MsgEcriture, " %s %d",  vl_DescriptionVar, va_Valeur );
   lm_dev ( vg_AssocApril.assoc, &vl_VarEcriture, &vg_ErrApril );
   ea_gestion_erreur ( &vg_ErrApril, "lm_dev ea_ecriture_flag " );
   vl_Valeur[0] = va_Valeur;
   /* Ecriture commande dans l'automate */
   if ( vg_TabFlag[va_Adresse - XZETC_ADRESSE_FLAG] !=  va_Valeur )
      ex_ecriture_trace_cmd ( XDC_EQT_NIC,
                              0, 
                              vl_MsgEcriture,
                              NULL,
                              0 );
   
   lm_wrv ( vg_AssocApril.assoc, 
            (XDY_Fonction)ea_ecriture_automate,
            (void *)vl_MsgEcriture,
            &vl_ValEcriture,
            &vg_ErrApril );
   ea_gestion_erreur ( &vg_ErrApril, "lm_wrv ea_ecriture_flag " );
   
   /* Effacement variable pour ecriture automate */
   lm_dlv ( vg_AssocApril.assoc, vl_VarEcriture.varnam, &vg_ErrApril );
   ea_gestion_erreur ( &vg_ErrApril, "lm_dlv ea_ecriture_flag" );
   
}
