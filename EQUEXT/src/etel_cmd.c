/*E*/
/*Fichier :  @(#)etel_cmd.c	1.15      Release : 1.15        Date : 12/04/03
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  EQUEXT
------------------------------------------------------
* MODULE TETEL * FICHIER etel_cmd.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* C'est le fichier des callbacks 
*
------------------------------------------------------
* HISTORIQUE :
*
* Nouaille.M	Version 1.1	30 Nov 1994	: Creation
* Mismer.D      Version 1.2     28 Jan 1995     : RAS
* Fontaine.C	Version 1.3  	 7 Mar 1995	: Prise en compte des modif. pour REPONDEUR
* Fontaine.C    Version 1.4     28 Avr 1995     : Modifictaion Commande repondeur
* Fontaine.C    Version 1.5     03 Mai 1995     : Reprise entete et commentaire
* Fontaine.C    Version 1.6     17 Mai 1995     : Reprise historique
* Mismer.D      Version 1.7     22 Mai 1995     : Changement nom fonction EnvoyerTrame
* Mismer.D      Version 1.8     14 Jun 1995     : Ajout verification inhibition MAA
* Volcic.F      Version 1.9     14 Jul 1995     : Modif pour essai numerotation interne
* Volcic.F      Version 1.10    20 Sep 1995     : Modif pour numerotation avec nouvelle installation
* Diaz.L	Version 1.11	15 Avr 1998	: Ajout de la fonction ed_cmd_ALCATEL
* Diaz.L  	version 1.12    09 Oct 1998     : Suppresion de traces du PABX
* Cluchague     version 1.15    23 Sep 2002     : Adaptation pour Mandelieu    
* Hilmarcher                                      Suite adaptation pour mandelieu
*
* SFO           06/10/14 : Adaptation pour compilation sous linux
* GR            09/07/2020 : EMS 1.18
* LCL		20/12/21	: MOVIS P2 ems SAE-312
------------------------------------------------------*/

/* fichiers inclus */

#include "etel_cmd.h"
/*#include "cstadrv.h"*/


/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "@(#)etel_cmd.c	1.15 12/04/03      : etel_cmd" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande Lecture etat(s) d'un ou de tous les equipements MAA
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	ed_init_etat_AUTOTEL ( 
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
*  Fonction appel�e lors de la r�ception du message
*  XDM_EINIT_ABT
*
* FONCTION 
*  Pour 1 ou tous les MAA , envoyer l'etat  par XDM_ETAT_AUTOTEL
*
------------------------------------------------------*/
{
   static char *version = "@(#)etel_cmd.c	1.15 12/04/03      : ed_init_etat_AUTOTEL" ;

   #ifndef _TIBCO_EMS
   T_STR			pl_DGInit;
   T_INT4			vl_NumEqt, vl_TypEqt;
   #else
   XDY_Datagroup    pl_DGInit;
   XDY_Entier			vl_NumEqt, vl_TypEqt;
   #endif 

   ETEL_DONNEES_MAA   *	        pl_listeMAA;   
   XDY_Tel			vl_EtatRepondeur;
   XDY_Eqt                      vl_PO = 0 ; 
   
 

   XZST_03EcritureTrace(XZSTC_FONCTION, "IN: ed_init_etat_AUTOTEL ");
		  
   /*A Recuperation des parametres du message XDM_EINIT_ABT */
   if(!TipcMsgRead(pa_Data -> msg,
               T_IPC_FT_STR,  &pl_DGInit,
               T_IPC_FT_INT4, &vl_TypEqt,
               T_IPC_FT_INT4, &vl_NumEqt,
	       NULL))
   {       
      /* Trace : Impossibilite de lire le message */
      XZST_03EcritureTrace(XZSTC_WARNING
 		,"messsage XDM_EINIT_ABT non lu ." );
 		
      XZST_10ArchiverMsgSyst(0,"ed_init_etat_AUTOTEL : Message XDM_EINIT_ABT recu avec parametre(s) invalide(s)"
                               , NULL);
      XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: ed_init_etat_AUTOTEL ");
      return;
   }

   /*A Verification si type equipement est un AUTOTEL */
   if (vl_TypEqt != XZECC_TYPE_EQT_MAA) 
   {
   	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: ed_init_etat_AUTOTEL ");
      	return;
   }
   	        
   /*A Pour 1 ou tous les MAA : envoi du msg XDM_ETAT_AUTOTEL si il est en service et actif */
  
   for ( pl_listeMAA = pg_debutMAA; pl_listeMAA != NULL; pl_listeMAA = pl_listeMAA->Suivant )
   {
      /* si le MAA est en service */
      if ( !(pl_listeMAA->Etat_Service & XDC_EQT_HORS_SRV) )
      {
         /* pour ts les eqt ou un eqt precis */
         if ((vl_NumEqt==0) || (vl_NumEqt==pl_listeMAA->Config.Numero) )
         { 
            if ( atoi(pl_listeMAA->Config.RepAssoc) == XDC_FAUX )
            {
               strcpy(vl_EtatRepondeur, "" );
      	       if (!TipcSrvMsgWrite ( pl_DGInit,
      			              XDM_IdentMsg(XDM_ETAT_AUTOTEL),
                            	      XDM_FLG_SRVMSGWRITE,
               			      T_IPC_FT_INT4,       pl_listeMAA->Config.NumeroPO,
               			      T_IPC_FT_INT4,       pl_listeMAA->NoCdeCourante,
               			      T_IPC_FT_STR,        pl_listeMAA->NoTelAppele,
               			      T_IPC_FT_STR,        vl_EtatRepondeur,
               			      T_IPC_FT_INT4,       (!pl_listeMAA->Etat_Service) ? pl_listeMAA->CptRenduCde : XDC_ERROR,
               			      T_IPC_FT_INT4,  	   pl_listeMAA->EtatLiaison,
                          	      NULL))
      	       {
         	   XZST_03EcritureTrace(XZSTC_WARNING, "Envoi message XDM_ETAT_AUTOTEL non effectue ." );
	       }
	       else
      	       {
      	           XZST_03EcritureTrace(XZSTC_INFO
 		      ,"etel_cmd.c : Envoi message XDM_ETAT_AUTOTEL effectue sur %s - %d - %d - %d " ,pl_DGInit,
 		      pl_listeMAA->NoCdeCourante,
 		      pl_listeMAA->CptRenduCde,
 		      pl_listeMAA->EtatLiaison);

	       }
	    }
	    else
	    {
	       if ( (pl_listeMAA->RenvoiSurRepondeur == XDC_FAUX) || !pl_listeMAA->Etat_Service )
	       {
	          strcpy(vl_EtatRepondeur, XDC_ARRET);
	       }
	       else
	       {
	          strcpy(vl_EtatRepondeur, XDC_MARCHE);
               }
               
      	       if (!TipcSrvMsgWrite ( pl_DGInit,
      			              XDM_IdentMsg(XDM_ETAT_AUTOTEL),
                            	      XDM_FLG_SRVMSGWRITE,
               			      T_IPC_FT_INT4,       0,
               			      T_IPC_FT_INT4,       pl_listeMAA->NoCdeCourante,
               			      T_IPC_FT_STR,        pl_listeMAA->NoTelAppele,
               			      T_IPC_FT_STR,        vl_EtatRepondeur,
               			      T_IPC_FT_INT4,       pl_listeMAA->CptRenduCde,
               			      T_IPC_FT_INT4,  	   pl_listeMAA->EtatLiaison,
                          	      NULL))
      	       {
         	   XZST_03EcritureTrace(XZSTC_WARNING, "Envoi message XDM_ETAT_AUTOTEL non effectue ." );
	       }
	       else
      	       {
      	           XZST_03EcritureTrace(XZSTC_INFO
 		      ,"etel_cmd.c : Envoi message XDM_ETAT_AUTOTEL effectue sur %s - %d - %d - %d " ,pl_DGInit,
 		      pl_listeMAA->NoCdeCourante, 
 		      pl_listeMAA->CptRenduCde,
 		      pl_listeMAA->EtatLiaison); 

	       }


	  
	  
	    }
	 }
      }	
   }  
  
   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: ed_init_etat_AUTOTEL ");
	       
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Envoi commande appel, renvoi ... vers AUTOTEL ou MAA 
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	ed_cmd_AUTOTEL ( 
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
*  Fonction appel�e lors de la r�ception du message
*  XDM_ECMD_AUTOTEL
*
* FONCTION 
*  Pour la MAA concernee, envoyer la commande d'appel, de renvoi ou 
*  d'annulation de renvoi sur repondeur.
*
------------------------------------------------------*/
{
   static char *version = "@(#)etel_cmd.c	1.15 12/04/03      : ed_cmd_AUTOTEL" ;
   
   #ifndef _TIBCO_EMS  
   T_STR			va_NumTel;
   T_INT4			va_CodeCde;
   T_INT2			va_NumeroPO;
   #else
   XDY_Tel                      va_NumTel;
   XDY_Entier			va_CodeCde;
   XDY_Mot			va_NumeroPO;
   #endif
   

   ETEL_DONNEES_MAA *		pl_listeMAA;
   ETEL_DONNEES_AUTOTEL *	pl_listeAUTOTEL;
   XZEXT_Trame			pl_mlcr; 
   XDY_Entier    		vl_Cste_MaxNumTel;
   XDY_Entier 			vl_Cste_TaillePrefixe; 
   XDY_Prefixe			vl_Cste_CodePrefixe;
   XDY_Renvoi			vl_Cste_CodeRenvoi;
   XDY_Tel			vl_NumTel;
   XDY_Entier			vl_CodeRetour = XZEXC_MAA_AVEC_REPONSE ;
   XDY_Datagroup		pl_DG;
   char				vl_NumLS[4];
   	     
 

   XZST_03EcritureTrace(XZSTC_FONCTION, "IN: ed_cmd_AUTOTEL ");
		  
   /*A Recuperation des parametres du message XDM_ECMD_AUTOTEL */
   if(!TipcMsgRead(pa_Data -> msg,
               T_IPC_FT_INT4,  &va_CodeCde,
               T_IPC_FT_STR,   &va_NumTel,
               T_IPC_FT_INT2,  &va_NumeroPO,
	       NULL))
   {       
      /* Trace : Impossibilite de lire le message */
      XZST_03EcritureTrace(XZSTC_WARNING,"messsage XDM_ECMD_AUTOTEL non lu ." );
      XZST_10ArchiverMsgSyst(0,"ed_cmd_AUTOTEL : Message XDM_ECMD_AUTOTEL recu avec parametre(s) invalide(s)", NULL );
      XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: ed_cmd_AUTOTEL ");
      return;
   }
  XZST_03EcritureTrace(XZSTC_WARNING, "Commande ttoto");
   if (vg_Mac_PC2 == XDC_VRAI)
   {
   	ed_cmd_ALCATEL(va_CodeCde, va_NumTel, va_NumeroPO);
   	XZST_03EcritureTrace(XZSTC_WARNING, "Commande ALCATEL: %d, NumTel : %s, NumPO: %d", va_CodeCde, va_NumTel, va_NumeroPO);

   }
   else
   {
      XZST_03EcritureTrace(XZSTC_WARNING, "Commande : %d, NumTel : %s, NumPO: %d", va_CodeCde, va_NumTel, va_NumeroPO);

   	        
   /*A Pour chaque MAA : verification si MAA est concerne */
  
   for ( pl_listeMAA = pg_debutMAA; pl_listeMAA != NULL; pl_listeMAA = pl_listeMAA->Suivant )
   {
   
     /*B MAA concerne si Numero PO est identique a celui de l'appelant */
      if ( (( pl_listeMAA->Config.NumeroPO == va_NumeroPO ) &&  ( va_CodeCde == XDC_APPEL)) || 
           (( va_CodeCde != XDC_APPEL ) && ( atoi(pl_listeMAA->Config.RepAssoc) == XDC_VRAI )) ) 
      {
      
     
     /* si le MAA est en service et actif */
        if ( !pl_listeMAA->Etat_Service ) 
        {
          if ( va_CodeCde == XDC_APPEL)
	  {
            /* Recuperation constantes generales provenant de l'AUTOTEL concerne */
            for ( pl_listeAUTOTEL = pg_debutAUTOTEL; pl_listeAUTOTEL != NULL; pl_listeAUTOTEL = pl_listeAUTOTEL->Suivant )
   	    {
   	      if (strstr(pl_listeMAA->Config.AdrAUTOTEL, pl_listeAUTOTEL->Config.Adr) != NULL )
   	      {
   	        if (pl_listeAUTOTEL->Config.MaxNumTel < (sizeof(XDY_Tel) -1) )
	       	   vl_Cste_MaxNumTel = pl_listeAUTOTEL->Config.MaxNumTel ;
     	        else
   	           vl_Cste_MaxNumTel = sizeof(XDY_Tel) -1 ;
   	        strcpy(vl_NumTel, va_NumTel) ;   
   	        vl_NumTel[vl_Cste_MaxNumTel] = '\0' ;  
   	        if (pl_listeAUTOTEL->Config.TaillePrefixe < (sizeof(XDY_Prefixe) -1) )
   	     	   vl_Cste_TaillePrefixe = pl_listeAUTOTEL->Config.TaillePrefixe ;
   	        else
   	           vl_Cste_TaillePrefixe = sizeof(XDY_Prefixe) -1 ;
   	        strcpy(vl_Cste_CodePrefixe, pl_listeAUTOTEL->Config.CodePrefixe );
   	        vl_Cste_CodePrefixe[vl_Cste_TaillePrefixe] = '\0' ;
   	        strcpy(vl_Cste_CodeRenvoi, pl_listeAUTOTEL->Config.CodeRenvoi);
   	        sprintf ( vl_NumLS, "%02d", pl_listeMAA->Config.NoLSMaitre );
   	        break ;
   	      
   	      }
   	      /* fin if */	
   	    }
   	    /* fin for */
          }
	  else
	  {
	    sprintf ( vl_NumLS, "%02d", pl_listeMAA->Config.NoLSMaitre );
	  }
	  /* fin if */
       }
       else
       {
          XDG_EncodeDG2 ( pl_DG, XDG_EETAT_AUTOTEL, vg_NomSite ) ;
          if ( atoi(pl_listeMAA->Config.RepAssoc) == XDC_FAUX )
          {
      	       if (!TipcSrvMsgWrite ( pl_DG,
      			              XDM_IdentMsg(XDM_ETAT_AUTOTEL),
                            	      XDM_FLG_SRVMSGWRITE,
               			      T_IPC_FT_INT4,       pl_listeMAA->Config.NumeroPO,
               			      T_IPC_FT_INT4,       pl_listeMAA->NoCdeCourante,
               			      T_IPC_FT_STR,        pl_listeMAA->NoTelAppele,
               			      T_IPC_FT_STR,        "",
               			      T_IPC_FT_INT4,       XDC_ERROR,
               			      T_IPC_FT_INT4,  	   pl_listeMAA->EtatLiaison,
                          	      NULL))
      	       {
         	   XZST_03EcritureTrace(XZSTC_WARNING, "Envoi message XDM_ETAT_AUTOTEL non effectue ." );
	       }
	   }
	   else
	   {
      	       if (!TipcSrvMsgWrite ( pl_DG,
      			              XDM_IdentMsg(XDM_ETAT_AUTOTEL),
                            	      XDM_FLG_SRVMSGWRITE,
               			      T_IPC_FT_INT4,       0,
               			      T_IPC_FT_INT4,       pl_listeMAA->NoCdeCourante,
               			      T_IPC_FT_STR,        pl_listeMAA->NoTelAppele,
               			      T_IPC_FT_STR,        XDC_ARRET,
               			      T_IPC_FT_INT4,       pl_listeMAA->CptRenduCde,
               			      T_IPC_FT_INT4,  	   pl_listeMAA->EtatLiaison,
                          	      NULL))
      	       {
         	   XZST_03EcritureTrace(XZSTC_WARNING, "Envoi message XDM_ETAT_AUTOTEL non effectue ." );
	       }
	   }
	}
	break;
      }	
   }
  
     /*A Controle existance MAA */
   if (( pl_listeMAA == NULL ) && ( va_CodeCde == XDC_APPEL ))
   {       
      /* Trace : Impossibilite recuperation MAA */
      XZST_03EcritureTrace(XZSTC_WARNING,"equipement MAA non repertorie." );
 		
      XZST_10ArchiverMsgSyst(0,"ed_cmd_AUTOTEL : Equipement MAA non repertorie", NULL);
      XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: ed_cmd_AUTOTEL !!!");
      return;
   }
   
        /*A Controle existance AUTOTEL en liaison avec MAA */
   if (( pl_listeAUTOTEL == NULL ) && ( va_CodeCde == XDC_APPEL ))
   {       
      /* Trace : Impossibilite recuperation MAA */
      XZST_03EcritureTrace(XZSTC_WARNING,"automate telephonique du MAA non repertorie." );
      XZST_10ArchiverMsgSyst(0,"ed_cmd_AUTOTEL : automate telephonique du MAA non repertorie", NULL );
      XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: ed_cmd_AUTOTEL ");
      return;
   }  
    
  
  /*A Envoi de la commande vers la tache tetmi */
                
  switch(va_CodeCde) {
  
  /*B Commande appel no telephone */
  case XDC_APPEL :        if (pl_listeAUTOTEL->Config.CodeRetour == XDC_FAUX) 
                          vl_CodeRetour = XZEXC_MAA_SANS_REPONSE ;
 		          XZST_03EcritureTrace(XZSTC_WARNING,"Demande Telephonique");
                          sprintf(pl_mlcr,"ATE0,S240=1,D%s;\r\n", vl_NumTel);                            
                          XZST_03EcritureTrace(XZSTC_WARNING
 		                 ,"Callback ed_cmd_AUTOTEL = trame : %s" ,pl_mlcr);
 		          EnvoyerTrame_Tel("ATH0\r\n",
        		        vl_CodeRetour,
        		        vl_NumLS,
		 	        XDC_APPEL,
		 	        XZEXC_PRIORITEFORTE,
        		        pl_listeMAA->SocketMaitre);
 		          EnvoyerTrame_Tel(pl_mlcr,
        		        vl_CodeRetour,
        		        vl_NumLS,
		 	        XDC_APPEL,
		 	        XZEXC_PRIORITEFORTE,
        		        pl_listeMAA->SocketMaitre);
        		  /* rangement etats dans table de structure */
       		  	  strcpy(pl_listeMAA->NoTelAppele, vl_NumTel) ;
         		  pl_listeMAA->NoCdeCourante =  XDC_APPEL ;
        		  pl_listeMAA->CptRenduCde = XDC_ATTENTE ;       
  		   	  break ;
  		   	  
  case XDC_RENVOI :	  
                          vl_CodeRetour = XZEXC_MAA_SANS_REPONSE ;
                          XZST_03EcritureTrace(XZSTC_DEBUG1
 		                 ,"Callback ed_cmd_AUTOTEL = Demande de Mise en Repondeur");
 		          sprintf(pl_mlcr,"4\033[B ;");
 		          XZST_03EcritureTrace(XZSTC_WARNING,"Mise en Rep");

			  EnvoyerTrame_Tel(pl_mlcr,
        		             vl_CodeRetour,
        		             vl_NumLS,
		 	             XDC_RENVOI,
		 	             XZEXC_PRIORITEFORTE,
        		             pl_listeMAA->SocketMaitre);
        		  /* rangement etats dans table de structure */
         		   pl_listeMAA->NoCdeCourante =  XDC_RENVOI ;
        		   pl_listeMAA->CptRenduCde = XDC_ATTENTE ; 
        		          
  		   	   break ;
  			  
  case XDC_ANNUL_RENVOI : 
                           vl_CodeRetour = XZEXC_MAA_SANS_REPONSE ;
                           XZST_03EcritureTrace(XZSTC_DEBUG1
 		                     ,"Callback ed_cmd_AUTOTEL = Demande d'Annulation de Mise en Repondeur");
		           sprintf(pl_mlcr,"0\033[B1;");
			   EnvoyerTrame_Tel(pl_mlcr,
			            vl_CodeRetour,
			            vl_NumLS,
			            XDC_ANNUL_RENVOI,
		 	            XZEXC_PRIORITEFORTE,
        		            pl_listeMAA->SocketMaitre);
        		      /* rangement etats dans table de structure */
        		   pl_listeMAA->NoCdeCourante =  XDC_ANNUL_RENVOI ;
        		   pl_listeMAA->CptRenduCde = XDC_ATTENTE ; 
        		          
  		   	   break ;
     }
  }
    
  
  /*A fin module */
   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: ed_cmd_AUTOTEL ");
	       
}




/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Envoi une trame a TETMi.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int  EnvoyerTrame_Tel ( char*	trame,
		    int		type_msg,
		    char*	adr_rgs,   /* Non utilise */
		    int 	nocmd,
		    int 	priorite,  /* Non utilise : toutes dde prioritaire */
		    int		socket)
/*
* ARGUMENTS EN ENTREE :
*  trame : trame a envoyer
*  type_msg : type du message
*  adr_rgs : adresse rgs 
*  nocmd : numero de commande
*  priorite : priorite de la trame
*  socket : socket d'emision
*
* ARGUMENTS EN SORTIE :
*   aucun
*
*
* CODE RETOUR : 
*   XDC_OK : succes
*   XDC_NOK : Erreur
*
* CONDITION D'UTILISATION
*  
*   Fonction appele apres avoir construit la trame
*
* FONCTION 
*  Demande d'envoi de trame a tetmi
*
------------------------------------------------------*/
{		     
static char *version = "@(#)etel_cmd.c	1.15 12/04/03      : EnvoyerTrame_Tel" ;
   
XZEXT_MSG_SOCKET	   vl_message;     
        
        /*A Construction de la trame */
        sprintf(vl_message,"%s%d %s %d %d %s %s %s",
           			XZEXC_DEMANDE_TEL,
           			type_msg,
           			adr_rgs,
 		    		nocmd,
		    		priorite,
          			XZEXC_SEP_TRAME,
          			trame,
           			XZEXC_FIN_MSG_SOCK);
            			
        /* Trace du message socket */
        XZST_03EcritureTrace( XZSTC_WARNING, "Message socket : %s",vl_message );
        XZST_03EcritureTrace( XZSTC_WARNING, "Envoi : ls: %s, cmde :%d", adr_rgs, nocmd );
         			
        /*A Envoi de la trame */
        if ( send ( socket , vl_message , strlen ( vl_message ), 0 ) <= 0 ) 				  				
        {
           /* Ecriture dans le fichier trace en cas d'�chec */
           XZST_03EcritureTrace( XZSTC_FONCTION, "Impossible d'�mettre la trame vers TETMi." );
           return(XDC_NOK);
        }
        else
        {   
           /*B Ecriture dans le fichier trace en cas de succes */
           XZST_03EcritureTrace( XZSTC_INFO, "Trame �mise vers TETMi." );
	   return(XDC_OK);
        }
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Message de controle.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	ed_controle_cb ( 
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
*  Fonction appel�e lors de la r�ception du message
*  XDM_ECMD_EQPT 
*
* FONCTION 
* Met a jour les variables d'etat dans les structures en memoire
*
------------------------------------------------------*/
{
   static char *version = "@(#)etel_cmd.c	1.15 12/04/03      : ed_controle_cb" ;
   
    #ifndef _TIBCO_EMS
    T_STR			va_NomMachine;
   T_INT4			va_Etat;
   T_INT4			va_TypeEqt;
   T_INT4			va_NumEqt;
    #else
    XDY_NomMachine		va_NomMachine;
   XDY_Entier			va_Etat;
   XDY_Entier			va_TypeEqt;
   XDY_Entier			va_NumEqt;
    #endif   

   ETEL_DONNEES_MAA *	pl_ListeMAA;
   

   XZST_03EcritureTrace(XZSTC_FONCTION, "IN: ed_controle_cb ");
		  
   /*A Recuperation des parametres du message XDM_ECMD_EQPT */
   if (!TipcMsgRead(pa_Data -> msg
                          ,T_IPC_FT_INT4,&va_Etat
                          ,T_IPC_FT_STR, &va_NomMachine
                          ,T_IPC_FT_INT4,&va_TypeEqt
                          ,T_IPC_FT_INT4,&va_NumEqt
	       		  ,NULL))
	       
   {      
      /* Trace : Impossibilite de lire le message */
      XZST_03EcritureTrace(XZSTC_WARNING,"messsage XDM_ECMD_EQPT non lu ." );
      XZST_10ArchiverMsgSyst(0,"ed_controle_cb : Message XDM_ECMD_EQPT recu avec parametre(s) invalide(s)", NULL );
      XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: ed_controle_cb ");
      return;
   }
	      
   /* Trace du message en entree de la fonction */
   XZST_03EcritureTrace(XZSTC_WARNING
        ,"callback ed_controle_cb, msg= %d %s %d %d",
                          va_Etat,
                          va_NomMachine,
                          va_TypeEqt,
                          va_NumEqt);
   
   {
   
   /* si equipement telephonique */   
       if (va_TypeEqt ==  XZECC_TYPE_EQT_MAA)
      {
         /*A Recherche du No d'equipement dans la structure MAA en memoire */
         for ( pl_ListeMAA = pg_debutMAA; pl_ListeMAA != NULL ; 
	         pl_ListeMAA = pl_ListeMAA->Suivant )
	 {
	    /*A Affectation de l'etat */
	    if (pl_ListeMAA->Config.Numero == va_NumEqt)
	    {
               if ( va_Etat > XZEXC_LIMITE_ETAT_EQT )
                  pl_ListeMAA->Etat_Service &= va_Etat;
               else
                  pl_ListeMAA->Etat_Service += va_Etat;
	       break;
    	    }
	 }
      }      
   }
   
   /* Trace des Differents etats service et activite */
   
   for ( pl_ListeMAA = pg_debutMAA; pl_ListeMAA != NULL; pl_ListeMAA = pl_ListeMAA->Suivant )
      XZST_03EcritureTrace(XZSTC_DEBUG1,"servMAA:%d ",pl_ListeMAA->Etat_Service);
   
   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: ed_controle_cb ");
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande d'Initialisation des MAA             
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	ed_init_MAA ()
 
/*
* ARGUMENTS EN ENTREE :
*   aucun
*
* ARGUMENTS EN SORTIE :
*   aucun
*
*
* CODE RETOUR : 
*   aucun
*
* CONDITION D'UTILISATION
*  Fonction appel�e lors d'une initialisation
*  complete des equipements telephoniques
*
* FONCTION 
*  Pour chaque MAA , envoyer trames de commandes d'init. aux equipements
*  Commandes a envoyer : mode echo(force a 0), mode emission code resultat (O/N),
*  			 format reponse (N ou A), temps attente (en sec.)
*
------------------------------------------------------*/
{
   static char *version = "@(#)etel_cmd.c	1.15 12/04/03      : ed_init_MAA" ;
   
   ETEL_DONNEES_MAA *		pl_listeMAA;   
   ETEL_DONNEES_AUTOTEL *	pl_listeAUTOTEL;   
   XDY_Entier			vl_CodeRetour = XZEXC_MAA_AVEC_REPONSE ;
   XZEXT_Trame			pl_mlcr;
   char				vl_NumLS[4];

   XZST_03EcritureTrace(XZSTC_FONCTION , "IN: ed_init_MAA ");
   
 /*A Pour chaque MAA en service :  */
  
   for ( pl_listeMAA = pg_debutMAA; pl_listeMAA != NULL; pl_listeMAA = pl_listeMAA->Suivant )
   {

      /*B controle si MAA en service et actif */
      if ( !(pl_listeMAA->Etat_Service & XDC_EQT_HORS_SRV) &&
            (atoi(pl_listeMAA->Config.RepAssoc) == XDC_FAUX ))
      {
      
      /*B Recuperation donnees venant de l'AUTOTEL correspondant */

        for ( pl_listeAUTOTEL = pg_debutAUTOTEL; pl_listeAUTOTEL != NULL; pl_listeAUTOTEL = pl_listeAUTOTEL->Suivant )
        {
   	   if (strstr(pl_listeMAA->Config.AdrAUTOTEL, pl_listeAUTOTEL->Config.Adr) != NULL)  break ;
 
        } /* fin for */
        
        if (pl_listeAUTOTEL == NULL) 
   	{
    	   XZST_03EcritureTrace(XZSTC_WARNING,"equipement MAA sans AUTOTEL configure ." );
 	   continue ;			     
   	}
   	   
   	/*B Preparation et envoi trames de commande vers AUTOTEL via MAA */
   	sprintf ( vl_NumLS, "%02d", pl_listeMAA->Config.NoLSMaitre );

   	/* mode echo (force � 0) */
   	pl_listeAUTOTEL->Config.ModeEcho = 0 ;
        sprintf(pl_mlcr,"ATE%01d",pl_listeAUTOTEL->Config.ModeEcho);
        XZST_03EcritureTrace(XZSTC_DEBUG1 ,"Callback ed_init_MAA  = trame : %s" ,pl_mlcr);
        EnvoyerTrame_Tel(pl_mlcr,
                     XZEXC_MAA_AVEC_REPONSE,
                     vl_NumLS,
	             XDC_APPEL,
		     XZEXC_PRIORITEFORTE,
        	     pl_listeMAA->SocketMaitre);
        if (pl_listeAUTOTEL->Config.CodeRetour == XDC_FAUX) 
        vl_CodeRetour = XZEXC_MAA_SANS_REPONSE ;
        	     
   	/* mode emission code resultat (force a 0) */
        sprintf(pl_mlcr,"ATQ0");
        XZST_03EcritureTrace(XZSTC_DEBUG1,"Callback ed_init_MAA  = trame : %s" ,pl_mlcr);
        EnvoyerTrame_Tel(pl_mlcr,
                     vl_CodeRetour,
                     vl_NumLS,
	             XDC_APPEL,
		     XZEXC_PRIORITEFORTE,
        	     pl_listeMAA->SocketMaitre);
        	     
  	/* mode format Ascii/Numerique */
        sprintf(pl_mlcr,"ATV%01d",pl_listeAUTOTEL->Config.FormatCodeRetour);
        XZST_03EcritureTrace(XZSTC_DEBUG1,"Callback ed_init_MAA  = trame : %s" ,pl_mlcr);
        EnvoyerTrame_Tel(pl_mlcr,
                     vl_CodeRetour,
                     vl_NumLS,
	             XDC_APPEL,
		     XZEXC_PRIORITEFORTE,
        	     pl_listeMAA->SocketMaitre);
        	     
  	/* mode config. temps d'attente (0-255 sec.) */
  	if (pl_listeAUTOTEL->Config.TpsAttente < 0) pl_listeAUTOTEL->Config.TpsAttente = 0 ;
        if (pl_listeAUTOTEL->Config.TpsAttente > 255) pl_listeAUTOTEL->Config.TpsAttente = 255 ;
        sprintf(pl_mlcr,"ATS7=%d",pl_listeAUTOTEL->Config.TpsAttente);
        XZST_03EcritureTrace(XZSTC_DEBUG1,"Callback ed_init_MAA  = trame : %s" ,pl_mlcr);
        EnvoyerTrame_Tel(pl_mlcr,
                     vl_CodeRetour,
                     vl_NumLS,
	             XDC_APPEL,
		     XZEXC_PRIORITEFORTE,
        	     pl_listeMAA->SocketMaitre); 	     
      }	
      
      /*B Annulation de Mise en Repondeur */
      if ( !(pl_listeMAA->Etat_Service & XDC_EQT_HORS_SRV) &&
            (atoi(pl_listeMAA->Config.RepAssoc) == XDC_VRAI ))
      {
         vl_CodeRetour = XZEXC_MAA_SANS_REPONSE ;
         XZST_03EcritureTrace(XZSTC_DEBUG1
 	      ,"Callback ed_cmd_AUTOTEL = Demande d'Annulation de Mise en Repondeur");
         sprintf(pl_mlcr,"0\033[B1;");
 	 
 	 EnvoyerTrame_Tel(pl_mlcr,
	              vl_CodeRetour,
		      vl_NumLS,
		      XDC_ANNUL_RENVOI,
		      XZEXC_PRIORITEFORTE,
        	      pl_listeMAA->SocketMaitre);
         /* rangement etats dans table de structure */
         pl_listeMAA->NoCdeCourante =  XDC_ANNUL_RENVOI ;
         pl_listeMAA->CptRenduCde = XDC_ATTENTE ; 
      }
   
   }
      
		  
  
   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: ed_init_MAA ");
}


/*X*/
/*----------------------------------------------------------
* SERVICE RENDU : 
*  Envoi commande appel, renvoi ... vers PABX 4200 d'ALCATEL
*
------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	ed_cmd_ALCATEL ( 
        T_INT4	 			vaa_CodeCde,
        T_STR				vaa_NumTel,
        T_INT2	                        vaa_NumeroPO)
 
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
*  du PABX d'ALCATEL
*
* FONCTION 
*  Pour la MAA concernee, envoyer la commande d'appel, de renvoi ou 
*  d'annulation de renvoi sur repondeur.
*
------------------------------------------------------*/
{
   static char *version = "@(#)etel_cmd.c	1.15 12/04/03      : ed_cmd_ALCATEL" ;
   
   ETEL_DONNEES_MAA *		pl_listeMAA; 
   XDY_Tel			vl_NumTel;
   XDY_Entier			vl_CodeRetour = 0 ;
   XDY_Entier			vl_Cste_MaxNumTel ;
   XDY_Datagroup		pl_DG;
#ifdef _HPUX_SOURCE
   CstaCrid			id;
   CstaLcin			state;
   CstaMsg			*id_msge;
#endif
   char				va_num_PO[4];
   char                         temp_num[10];	     

   int             vl_fd;
   char            pl_PathEtFic[XDC_PATH_ABS_SIZE];
   XDY_Texte       pl_LigneFich;
   int 		   vl_int1, vl_int2; 

   XZST_03EcritureTrace(XZSTC_FONCTION, "IN: ed_cmd_ALCATEL ");
		  
   XZST_03EcritureTrace(XZSTC_WARNING, "Commande ed_cmd_ALCATEL : %d, NumTel : %s, NumPO: %d", vaa_CodeCde, vaa_NumTel, vaa_NumeroPO);

   	        
   /*A Pour chaque MAA : verification si MAA est concerne */
  for ( pl_listeMAA = pg_debutMAA; pl_listeMAA != NULL; pl_listeMAA = pl_listeMAA->Suivant )
   {
   
     /*B MAA concerne si Numero PO est identique a celui de l'appelant */
      if ( pl_listeMAA->Config.NumeroPO == vaa_NumeroPO ) 
      {
        XZST_03EcritureTrace(XZSTC_WARNING,"si Numero PO est identique a celui de l'appelant");
       /* si le MAA est en service et actif */
        if ( !pl_listeMAA->Etat_Service ) 
        {
          XZST_03EcritureTrace(XZSTC_WARNING,"si le MAA est en service et actif"); 
          if ( vaa_CodeCde == XDC_APPEL)
	  {
              XZST_03EcritureTrace(XZSTC_WARNING, "CodeCde == XDC_APPE"); 
            /*  vl_NumTel[vl_Cste_MaxNumTel] = '\0';
              strcpy( vl_NumTel, vaa_NumTel );*/
              break;
          }
	  else
	  {
	  XZST_03EcritureTrace(XZSTC_WARNING, "PAS CodeCde == XDC_APPE");
	  /* sprintf ( vl_NumLS, "%02d", pl_listeMAA->Config.NoLSMaitre );*/
	  }
	  /* fin if */
	 }
	 break;
       }
   }
   /*A Controle existance MAA */
   if ( pl_listeMAA == NULL )
   {      
      /* Trace : Impossibilite recuperation MAA */
      XZST_03EcritureTrace(XZSTC_WARNING,"equipement MAA non repertorie." );
      XZST_10ArchiverMsgSyst(0,"ed_cmd_ALCATEL : Equipement MAA non repertorie", NULL);
      XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: ed_cmd_ALCATEL ");
      return;
   }
   
  /*
  *Modification Hilmarcher pour prise en compte nveau PABX ALCATEL Mandelieu
  *--------------------------------------------------------------------------
  */
  
  /* Construction du nom du fichier  */
  sprintf ( pl_PathEtFic, "%s/%s", XZEXC_PATH_CONFIG1, "Config_ALCATEL_Numero" );

  /* Ouverture du fichier Config_ALCATEL_Numero  */
  if ( (vl_fd = open (pl_PathEtFic, O_RDONLY)) <= 0 )
  {
	/* Ecriture trace absence fichier */
	XZST_03EcritureTrace( XZSTC_WARNING, " Manque fichier %s.", pl_PathEtFic );
  }

  else
  {
  	while ( ex_LireLigneFich ( vl_fd, pl_LigneFich, sizeof ( pl_LigneFich ) ) != XZEXC_EOF )
  	{
		if ( *pl_LigneFich != XDF_COMMENTAIRE )
		{
		 	sscanf ( pl_LigneFich, "%d %d",
					&vl_int1,
					&vl_int2);

		 	if (vl_int1 == vaa_NumeroPO)
				{sprintf(vg_Numero_PO,"%d", vl_int2);}
		}
  	}
  close(vl_fd);
  }
  /*
  *fin modification Hilmarcher
  *------------------------------------------------------------------------
  */		
  
  /*A Envoi de la commande vers la tache tetmi */
                
  switch(vaa_CodeCde) {
  
  /*B Commande appel no telephone */
  case XDC_APPEL :	  if ( vaa_NumTel[0] != '0' )
			  {
                             sprintf(temp_num, "1300%s", vaa_NumTel);
			     strcpy(vaa_NumTel, temp_num);
			  }
			  
			  XZST_03EcritureTrace(XZSTC_WARNING,"valeur de vaa_NumeroPO: %d",vaa_NumeroPO);
			  XZST_03EcritureTrace(XZSTC_WARNING,"valeur de  vg_Numero_PO: %s", vg_Numero_PO);

#ifdef _HPUX_SOURCE
			  SendMonitorStart(vg_Alcatel, vg_Numero_PO, ROSE_NONE, 0, 0, NULL);
		   	  SendMakeCall(vg_Alcatel, vg_Numero_PO, vaa_NumTel, ROSE_NONE, NULL);
#endif
  		   	  strcpy(pl_listeMAA->NoTelAppele, vaa_NumTel) ;
  		   	  pl_listeMAA->NoCdeCourante =  XDC_APPEL ;
                          pl_listeMAA->CptRenduCde = XDC_ATTENTE ;       
			  break ;
  		   	  
  case XDC_RENVOI :	  vl_CodeRetour = XZEXC_MAA_SANS_REPONSE ;
                          XZST_03EcritureTrace(XZSTC_DEBUG1
 		                 ,"Callback ed_cmd_AUTOTEL = Demande de Mise en Repondeur");
#ifdef _HPUX_SOURCE
 		          SendMonitorStart(vg_Alcatel, vg_Numero_PO, ROSE_NONE, 0, 0, NULL);
	   		  SendMakeCall(vg_Alcatel, vg_Numero_PO, "795106", ROSE_NONE, NULL);
#endif
        		  /* rangement etats dans table de structure */
         		  pl_listeMAA->NoCdeCourante =  XDC_RENVOI ;
        		  pl_listeMAA->CptRenduCde = XDC_ATTENTE ;
        		  pl_listeMAA->RenvoiSurRepondeur = XDC_VRAI;
  		   	  break ;  
  			  
  case XDC_ANNUL_RENVOI :  vl_CodeRetour = XZEXC_MAA_SANS_REPONSE ;
                           XZST_03EcritureTrace(XZSTC_DEBUG1
 		                     ,"Callback ed_cmd_AUTOTEL = Demande d'Annulation de Mise en Repondeur");
#ifdef _HPUX_SOURCE
        		   SendMonitorStart(vg_Alcatel, vg_Numero_PO, ROSE_NONE, 0, 0, NULL);
	   		   SendMakeCall(vg_Alcatel, vg_Numero_PO, "790", ROSE_NONE, NULL);
#endif
        		   /* rangement etats dans table de structure */
        		   pl_listeMAA->NoCdeCourante =  XDC_ANNUL_RENVOI ;
        		   pl_listeMAA->CptRenduCde = XDC_ATTENTE ;
        		   pl_listeMAA->RenvoiSurRepondeur =  XDC_FAUX; 
 		   	   break ;
  			  
  }
    
  XZST_03EcritureTrace(XZSTC_WARNING,"Demande Telephonique : FIN du module pilotage ALCATEL ");
  /*A fin module */
   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: ed_cmd_ALCATEL ");
	       
}


