/*E*/
/*Fichier : $Id: etel_tra.c,v 1.10 2007/11/20 08:35:07 pc2dpdy Exp $     Release : $Revision: 1.10 $       Date : $Date: 2007/11/20 08:35:07 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  EQUEXT
------------------------------------------------------
* MODULE TETEL * FICHIER etel_tra.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* c'est le module de traitement des trames provenant des eqt telephoniques
*
------------------------------------------------------
* HISTORIQUE :
*
* Nouaille.M	Version 1.1	30 Nov 1994	: Creation
* Mismer.D      Version 1.2     28 Jan 1995     : RAS
* Fontaine.C	Version 1.3	12 Mar 1995	: Traitement repondeur
* Mismer.D	Version 1.4	31 Mar 1995	: Modif etat repondeur
* Fontaine.C    Version 1.5     03 Mai 1995     : Reprise entete et commentaire
* Fontaine.C    Version 1.6     17 Mai 1995     : Reprise historique
* Volcic.F   	Version 1.7     26 Jul 1995     : Ajout static
* Mismer.D      Version 1.8     15 Nov 1996     : Modif Alarmes (DEM/1232)
* Diaz.L  	version 1.9     09 Oct 1998     : Suppresion de traces du PABX
* JMG   13/11/07        : ajout site de gestion DEM715
* LCL		20/12/21	: MOVIS P2 ems SAE-312
*
------------------------------------------------------*/

/* fichiers inclus */

#include "etel_tra.h"

/* definitions de constantes */


/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Decodage de la trame et traitement
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int et_analyser_trame ( int va_socket )

/*
* ARGUMENTS EN ENTREE :
*   
* va_socket:socket de lecture
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
*
* CONDITION D'UTILISATION
* execute sur reception de message socket
*
* FONCTION 
*
*  Decodage de la trame et traitement
------------------------------------------------------*/
{
   int				vl_IndCnx = 0,
   				vl_LgMsg  = 0;
   char				*pl_trame;
   char				vl_entete1[10],
   				vl_entete2[10],
   				vl_adrrgs[10];
   int				vl_NoLiaison;
   int				vl_typetrame;
   XZEXT_MSG_SOCKET		vl_Msg;
   int				vl_param;
   int				vl_nbnonrep, vl_nberrcrc ;
   ETEL_DONNEES_MAA		*pl_listeMAA;
   ETEL_DONNEES_AUTOTEL		*pl_listeAUTOTEL;
   XDY_Datagroup		pl_DG;
   XDY_Horodate			vl_horodate;
   int 				vl_nocmd; 
   char                         vl_horod1[20],
                                vl_horod2[9];
   XZSMT_Horodate       	pl_Horodatedecompo;
   int 				vl_typ_rep;
   int 				vl_al;
   char				vl_arg[6];
   XDY_Eqt			vl_PO = 0;
   XDY_Tel			vl_EtatRepondeur = "";

   static char *version = "@(#)etel_tra.c	1.8 12/05/96 : et_analyser_trame " ;

   /*A
    *  Lecture du message recu dans la socket appelante
    */
	 
   if ( ex_lire_msg ( va_socket, vl_Msg, &vl_LgMsg, &vg_SockMask, &vl_IndCnx ) != XDC_OK )
   {
      /* Ecriture trace Erreur lecture message socket. */
      XZST_03EcritureTrace( XZSTC_WARNING, " Erreur lecture message socket. " );
   }
   else
   {
      /*A Scan du message provenant de la socket */
      vl_param=sscanf(vl_Msg,"%s %s %s %s %d %d %d %s %d %d",vl_entete1,
      					   	    vl_entete2,
      					   	    vl_horod1,
      					   	    vl_horod2,
      					   	    &vl_nbnonrep,
      					  	    &vl_nberrcrc,  /* Non utilise */
      					  	    &vl_nocmd,
      					   	    vl_adrrgs,	   /* 1er octet = No liaison */
      					  	    &vl_typetrame, /* Non utilise */
      					  	    &vl_typ_rep);
      					 
      if (vl_param != 10)
      {
         XZST_03EcritureTrace( XZSTC_WARNING, " Lecture message socket impossible mes=%s",vl_Msg);
         return;
      }
      
      XZST_03EcritureTrace( XZSTC_WARNING, "<<<%s>>>",vl_Msg);
      					 
      /*A Conversion horodate et recuperation liaison */
      sprintf(pl_Horodatedecompo,"%s %s",vl_horod1,vl_horod2);
      XZSM_13ConversionHorodateSec(pl_Horodatedecompo,&vl_horodate);
      sscanf ( vl_adrrgs, "%d", &vl_NoLiaison );
      
      /*B Recherche du No MAA concern� */
      for ( pl_listeMAA = pg_debutMAA; pl_listeMAA != NULL; pl_listeMAA = pl_listeMAA->Suivant )
      {
         if (pl_listeMAA->Config.NoLSMaitre == vl_NoLiaison) break;
      }
      if (pl_listeMAA == NULL)
      {
         XZST_03EcritureTrace( XZSTC_WARNING, " Equipement MAA non trouve en memoire mes=%s",vl_Msg);
         return;
      }
      
      /*A Gestion de l'alarme non rep*/
      if(vl_nbnonrep==0) 
      {
         vl_al=XDC_FAUX;
         pl_listeMAA->EtatLiaison = XDC_OK ;
      }
      else
      {
         vl_al=XDC_VRAI;
         XZST_03EcritureTrace( XZSTC_WARNING, " Erreur non reponse de l'eqt MAA no %d " ,pl_listeMAA->Config.Numero);
         pl_listeMAA->EtatLiaison = XDC_HS ;
      }
      
      sprintf(vl_arg,"%d",vl_nbnonrep);
      
      /* Envoi de l'alarme absence de reponse */
      ex_env_alarme  ( 	vg_NomMachine,
			vl_horodate,
			XZECC_TYPE_EQT_MAA,
			pl_listeMAA->Config.Numero,
			XZAMC_DEF_TRANS_TEL,
			vl_al,
			vl_arg,
			XZSC_22NumeroSiteLocal());
      
      /*A Selon la reponse formulee rangement compte rendu 
          commande en cours dans la structure ETEL_DONNEES_MAA */
      switch(vl_typ_rep) 
      {
      
      case XDC_OK :		;		
      case XDC_CONNECT :	;			
      case XDC_RING :		;		
      case XDC_NOCARRIER :	;
      case XDC_BUSY :		;		
      case XDC_NOANSWER :	;	
      case XDC_LIBRE :		;
      				pl_listeMAA->CptRenduCde = vl_typ_rep ;
      				ex_env_alarme  (vg_NomMachine,
				vl_horodate,
				XZECC_TYPE_EQT_MAA,
				pl_listeMAA->Config.Numero,
				XZAMC_DEF_REP_TEL,
				XDC_FAUX,
				0,
				XZSC_22NumeroSiteLocal());
      				break ;
      case XDC_ERROR :		pl_listeMAA->CptRenduCde = XDC_ERROR ;
      				ex_env_alarme  (vg_NomMachine,
				vl_horodate,
				XZECC_TYPE_EQT_MAA,
				pl_listeMAA->Config.Numero,
				XZAMC_DEF_REP_TEL,
				XDC_VRAI,
				0,
				XZSC_22NumeroSiteLocal());
      				break ;
      default :			pl_listeMAA->CptRenduCde = XDC_NORESPONS ;
      				break ;       			        			     			 	
       
      }
    
 
      /*A Envoi message XDG_EETAT_AUTOTEL */
      
      switch ( vl_nocmd )
      {
         case XDC_APPEL : 
            strcpy ( vl_EtatRepondeur, "" );
            vl_PO = pl_listeMAA->Config.NumeroPO;
            break;
         case XDC_RENVOI : 
            strcpy ( vl_EtatRepondeur, XDC_MARCHE );
            vl_PO = 0;
            if ( vl_typ_rep == XDC_OK )
            {
               pl_listeMAA->RenvoiSurRepondeur =  XDC_VRAI ;
            }
	    if ( pl_listeMAA->RenvoiSurRepondeur ==  XDC_VRAI )
	    { 
               strcpy ( vl_EtatRepondeur, XDC_MARCHE );
            }
            else
	    { 
               strcpy ( vl_EtatRepondeur, XDC_ARRET );
            }
            break;
         case XDC_ANNUL_RENVOI : 
            vl_PO = 0;
            if ( vl_typ_rep == XDC_OK )
            {
               pl_listeMAA->RenvoiSurRepondeur =  XDC_FAUX ;
            }
	    if ( pl_listeMAA->RenvoiSurRepondeur ==  XDC_VRAI )
	    { 
               strcpy ( vl_EtatRepondeur, XDC_MARCHE );
            }
            else
	    { 
               strcpy ( vl_EtatRepondeur, XDC_ARRET );
            }
            break;
      }
       
      XDG_EncodeDG2 ( pl_DG, XDG_EETAT_AUTOTEL, vg_NomSite) ;	              
      if (!TipcSrvMsgWrite ( pl_DG,			
   	           XDM_IdentMsg(XDM_ETAT_AUTOTEL),
               	   XDM_FLG_SRVMSGWRITE,
      		   T_IPC_FT_INT4,       vl_PO,
       		   T_IPC_FT_INT4,       pl_listeMAA->NoCdeCourante,
       		   T_IPC_FT_STR,        pl_listeMAA->NoTelAppele,
          	   T_IPC_FT_STR,        vl_EtatRepondeur,
             	   T_IPC_FT_INT4,       pl_listeMAA->CptRenduCde,
                   T_IPC_FT_INT4,  	pl_listeMAA->EtatLiaison,
                   NULL))
      {
 	XZST_03EcritureTrace(XZSTC_WARNING
        	,"Envoi message XDM_ETAT_AUTOTEL non effectue." );
         				
      }
      else
      {
      
        XZST_03EcritureTrace(XZSTC_INFO
 		,"Envoi message XDM_ETAT_AUTOTEL effectue sur %s - %d - %d - %d " ,pl_DG,
 		pl_listeMAA->NoCdeCourante,
 		pl_listeMAA->CptRenduCde,
 		pl_listeMAA->EtatLiaison);
         				
      }


   }
   return ;
}
