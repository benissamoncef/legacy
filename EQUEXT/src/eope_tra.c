/*E*/
/*Fichier : $Id: eope_tra.c,v 1.6 2007/11/20 08:35:06 pc2dpdy Exp $      Release : $Revision: 1.6 $        Date : $Date: 2007/11/20 08:35:06 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  EQUEXT
------------------------------------------------------
* MODULE eope_tra * FICHIER eope_tra.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* c'est le module de traitement des trames provenant des eqt OPERATOR
*
------------------------------------------------------
* HISTORIQUE :
*
* Nouaille.M	Version 1.1	02 Dec 1994	: Creation
* Nouaille.M    Version 1.2     11 Jan 1995     : RAS
* Fontaine.C    Version 1.3     17 Mai 1995     : Reprise historique
* Mismer.D      Version 1.4     04 Dec 1995     : Correction type equipement pour envoi alarme
* Mismer.D      Version 1.5     15 Nov 1996     : Modif Alarmes (DEM/1232)
* JMG   13/11/07        : ajout site de gestion DEM715
* LCL		20/12/21	: MOVIS P2 ems SAE-312
*
------------------------------------------------------*/

/* fichiers inclus */

#include "eope_tra.h"
#include "ex_mala.h"

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
   XDY_NoLS			vl_NoLiaison;
   int				vl_typetrame;
   XZEXT_MSG_SOCKET		vl_Msg;
   int				vl_param;
   int				vl_nbnonrep, vl_nberrcrc ;
   EOPE_DONNEES_OPER		*pl_listeOPER;
   XDY_Datagroup		pl_DG;
   XDY_Horodate			vl_horodate;
   int 				vl_nocmd; 
   char                         vl_horod1[20],
                                vl_horod2[9];
   XZSMT_Horodate       	pl_Horodatedecompo;
   int 				vl_typ_rep;
   int 				vl_al;
   char				vl_arg[6];

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
      					 
      /*A Conversion horodate et recuperation liaison */
      sprintf(pl_Horodatedecompo,"%s %s",vl_horod1,vl_horod2);
      XZSM_13ConversionHorodateSec(pl_Horodatedecompo,&vl_horodate);
      vl_NoLiaison = vl_adrrgs[0] ;
      
      /*B Recherche du No OPERATOR concern� */
      for ( pl_listeOPER = pg_debutOPER; pl_listeOPER != NULL; pl_listeOPER = pl_listeOPER->Suivant )
      {
         if (pl_listeOPER->Config.NoLSMaitre == vl_NoLiaison) break;
      }
      if (pl_listeOPER == NULL)
      {
         XZST_03EcritureTrace( XZSTC_WARNING, " Equipement OPERATOR non trouve en memoire mes=%s",vl_Msg);
         return;
      }
      
      /*A Gestion de l'alarme non rep*/
      if ( vl_nbnonrep == 0 ) 
      {
         vl_al = XDC_FAUX;
         pl_listeOPER->EtatLiaison  = XDC_OK ;
      }
      else
      {
         vl_al = XDC_VRAI;
         XZST_03EcritureTrace( XZSTC_WARNING, " Erreur non reponse de l'eqt OPERATOR no %d " ,pl_listeOPER->Config.Numero);
         pl_listeOPER->EtatLiaison = XDC_HS ;
      }
      
      sprintf(vl_arg,"%d",vl_nbnonrep);
      
      /* Envoi de l'alarme */
      if ( !(pl_listeOPER->Etat_Service) )
         ex_env_alarme ( 	vg_NomMachine,
				vl_horodate,
				XDC_EQT_MAA,
				pl_listeOPER->Config.Numero,
				XZAMC_DEF_TRANS_TEL,
				vl_al,
				vl_arg,
				XZSC_22NumeroSiteLocal());
      
      /*A Selon la reponse formulee rangement compte rendu 
          commande en cours dans la structure EOPE_DONNEES_OPER */
      switch(vl_typ_rep) 
      {
      case XDC_OK :		ex_env_alarme (	vg_NomMachine,
						vl_horodate,
						XDC_EQT_MAA,
						pl_listeOPER->Config.Numero,
						XZAMC_DEF_REP_TEL,
						XDC_FAUX,
						NULL,
						XZSC_22NumeroSiteLocal());
      case XDC_PHASE_1 :	;
      case XDC_PHASE_2 :	;
      case XDC_PHASE_3 :	pl_listeOPER->CptRenduCde = vl_typ_rep ;
      				break ;
      				
      case XDC_SYNTAX_NOK :	;
      case XDC_OPERATOR_NOK :	;
      case XDC_OPERATOR_HS :	;
      case XDC_TIME_OUT :	;
      case XDC_SYSTEM_HS :	pl_listeOPER->CptRenduCde = vl_typ_rep ;
      				if ( !(pl_listeOPER->Etat_Service) )
				   ex_env_alarme  (	vg_NomMachine,
							vl_horodate,
							XDC_EQT_MAA,
							pl_listeOPER->Config.Numero,
							XZAMC_DEF_REP_TEL,
							XDC_VRAI,
							NULL,
							XZSC_22NumeroSiteLocal());
      				break ;

      default :			pl_listeOPER->CptRenduCde = XDC_NORESPONS ;
      				if ( (vl_al == XDC_FAUX) && !(pl_listeOPER->Etat_Service) )
      				{
      				   ex_env_alarme (	vg_NomMachine,
							vl_horodate,
							XDC_EQT_MAA,
							pl_listeOPER->Config.Numero,
							XZAMC_DEF_REP_TEL,
							XDC_VRAI,
							NULL ,
							XZSC_22NumeroSiteLocal());
      				}
      				break ;       			        			     			 	
       
      }
    
 
 
      /*A Envoi message XDG_EETAT_OPER */
       
      XDG_EncodeDG2 ( pl_DG, XDG_EETAT_OPER, vg_NomSite) ;	              
      if (!TipcSrvMsgWrite ( pl_DG,			
   	           XDM_IdentMsg(XDM_ETAT_OPER),
               	   XDM_FLG_SRVMSGWRITE,
      		   T_IPC_FT_INT4,       pl_listeOPER->Config.Numero,
       		   T_IPC_FT_INT4,       pl_listeOPER->NoCdeCourante,
       		   T_IPC_FT_STR,        pl_listeOPER->NumOperateur,
          	   T_IPC_FT_STR,        pl_listeOPER->MessageEnvoye,
             	   T_IPC_FT_INT4,       pl_listeOPER->CptRenduCde,
                   T_IPC_FT_INT4,  	pl_listeOPER->EtatLiaison,
                   NULL))
      {
 	XZST_03EcritureTrace(XZSTC_WARNING, "Envoi message XDM_ETAT_OPER non effectue" );
         				
      }
      else
      {
 	XZST_03EcritureTrace(XZSTC_MESSAGE, 	"Envoi message XDM_ETAT_OPER effectue sur %s - %d - %d - %d " ,pl_DG,
 						pl_listeOPER->NoCdeCourante,
 						pl_listeOPER->CptRenduCde,
 						pl_listeOPER->EtatLiaison);
         				
      }

   }
   return ;
}
