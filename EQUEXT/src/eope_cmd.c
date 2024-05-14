/*E*/
/*Fichier : $Id: eope_cmd.c,v 1.4 2020/11/03 18:09:45 pc2dpdy Exp $      Release : $Revision: 1.4 $        Date : $Date: 2020/11/03 18:09:45 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  EQUEXT
------------------------------------------------------
* MODULE eope_cmd * FICHIER eope_cmd.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* C'est le fichier des callbacks 
*
------------------------------------------------------
* HISTORIQUE :
*
* Nouaille.M	Version 1.1	02 Dec 1994	: Creation
* Mismer.D      Version 1.2     22 Mai 1995     : Changement nom fonction EnvoyerTrame
* Mismer.D      Version 1.3     04 Dec 1995     : Correction etat equipement 
* GR            Version 1.4     08 Juil 2020    : EMS 
* LCL		20/12/21	: MOVIS P2 ems SAE-312
------------------------------------------------------*/

/* fichiers inclus */
#include <xzsem.h>

#include "eope_cmd.h"


/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: eope_cmd.c,v 1.4 2020/11/03 18:09:45 pc2dpdy Exp $ : eope_cmd" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande Lecture etat(s) d'un ou de tous les equipements OPERATOR
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	ed_init_etat_OPER ( 
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
*  Pour 1 ou tous les OPERATOR , envoyer l'etat  par XDM_ETAT_OPER
*
------------------------------------------------------*/
{
   static char *version = "$Id: eope_cmd.c,v 1.4 2020/11/03 18:09:45 pc2dpdy Exp $ : ed_init_etat_OPER" ;

   #ifndef _TIBCO_EMS
   T_STR            pl_DGInit;
   T_INT4			vl_NumEqt, vl_TypEqt;
   #else
   XDY_Datagroup    pl_DGInit;
   XDY_Entier			vl_NumEqt, vl_TypEqt;
   #endif

   EOPE_DONNEES_OPER *		pl_listeOPER;   
 

   XZST_03EcritureTrace(XZSTC_FONCTION, "IN: ed_init_etat_OPER ");
		  
   /*A Recuperation des parametres du message XDM_EINIT_ABT */
   if(!TipcMsgRead(pa_Data -> msg,
               T_IPC_FT_STR,  &pl_DGInit,
               T_IPC_FT_INT4, &vl_TypEqt,
               T_IPC_FT_INT4, &vl_NumEqt,
	       NULL))
   {       
      /* Trace : Impossibilite de lire le message */
      XZST_03EcritureTrace(XZSTC_WARNING, "messsage XDM_EINIT_ABT non lu." );
      XZST_10ArchiverMsgSyst(0,"ed_init_etat_OPER : Message XDM_EINIT_ABT recu avec parametre(s) invalide(s)", NULL);
      XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: ed_init_etat_OPER ");
      return;
   }

   /*A Verification si type equipement est un OPERATOR */
   if (vl_TypEqt != XZECC_TYPE_EQT_OPE) 
   {
   	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: ed_init_etat_OPER ");
      	return;
   }
   	        
   /*A Pour 1 ou tous les OPERATOR : envoi du msg XDM_ETAT_OPER si il est en service et actif */
  
   for ( pl_listeOPER = pg_debutOPER; pl_listeOPER != NULL; pl_listeOPER = pl_listeOPER->Suivant )
   {
      /* si le OPERATOR est en service et actif */
      if ( !(pl_listeOPER->Etat_Service & XDC_EQT_HORS_SRV) && !(pl_listeOPER->Etat_Service & XDC_EQT_INACTIF) )
      {
         /* pour ts les eqt ou un eqt precis */
         if ((vl_NumEqt==0) || (vl_NumEqt==pl_listeOPER->Config.Numero))
         { 
                 
     	    if (!TipcSrvMsgWrite ( pl_DGInit,
      			           XDM_IdentMsg(XDM_ETAT_OPER),
                          	   XDM_FLG_SRVMSGWRITE,
               			   T_IPC_FT_INT4,       pl_listeOPER->Config.Numero,
               			   T_IPC_FT_INT4,       pl_listeOPER->NoCdeCourante,
               			   T_IPC_FT_STR,        pl_listeOPER->NumOperateur,  /* no sur 6 chiffres */
               			   T_IPC_FT_STR,        pl_listeOPER->MessageEnvoye,
               			   T_IPC_FT_INT4,       pl_listeOPER->CptRenduCde,
               			   T_IPC_FT_INT4,  	pl_listeOPER->EtatLiaison,
                          	   NULL))
      	    {
         	XZST_03EcritureTrace(XZSTC_WARNING, "Envoi message XDM_ETAT_OPER non effectue" );
	    }
	    else
      	    {
         	XZST_03EcritureTrace( XZSTC_MESSAGE
 		,"eope_cmd.c : Envoi message XDM_ETAT_OPER effectue sur %s - %d - %d - %d " , pl_DGInit,
 		pl_listeOPER->NoCdeCourante,
 		pl_listeOPER->CptRenduCde,
 		pl_listeOPER->EtatLiaison);
	    }
	 }
      }	
   }  
  
   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: ed_init_etat_OPER ");
	       
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Envoi commande appel, renvoi ... vers OPERATOR
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	ed_cmd_OPER ( 
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
*  XDM_ECMD_OPER
*
* FONCTION 
*  Pour le OPERATOR concerne, envoyer la commande d'envoi
*  message Numerique ou Alphanumerique via modem.
*
------------------------------------------------------*/
{
   static char *version = "$Id: eope_cmd.c,v 1.4 2020/11/03 18:09:45 pc2dpdy Exp $ : ed_cmd_OPER" ;
   

#ifndef _TIBCO_EMS  
   T_STR			va_NumOp;
   T_INT4			va_CodeCde;
   #else
   XDY_Num_Oper                 va_NumOp;
   XDY_Entier			va_CodeCde;
   #endif

#ifndef _TIBCO_EMS
   T_STR			va_Message;
#else
   XDY_Msg_ES80                 va_Message;
#endif

   EOPE_DONNEES_OPER *		pl_listeOPER;
   XZEXT_Trame			pl_mlcr; 
   XDY_Num_Oper			vl_NumOp ;
   XDY_Msg_ES80			vl_Message;
   XDY_Entier			vl_CodeRetour ;
   XDY_Datagroup		pl_DG;
   	     
 

   XZST_03EcritureTrace(XZSTC_FONCTION, "IN: ed_cmd_OPER ");
		  
   /*A Recuperation des parametres du message XDM_ECMD_OPER */
   if(!TipcMsgRead(pa_Data -> msg,
               T_IPC_FT_INT4,  &va_CodeCde,
               T_IPC_FT_STR,   &va_NumOp,
               T_IPC_FT_STR,   &va_Message,
	       NULL))
   {       
      /* Trace : Impossibilite de lire le message */
      XZST_03EcritureTrace(XZSTC_WARNING, "messsage XDM_ECMD_OPER non lu." );
      XZST_10ArchiverMsgSyst(0,"ed_cmd_OPER : Message XDM_ECMD_OPER recu avec parametre(s) invalide(s)", NULL);
      XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: ed_cmd_OPER ");
      return;
   }

   
   	        
   /*A Pour chaque OPERATOR : verification si OPERATOR est concerne */
  
   for ( pl_listeOPER = pg_debutOPER; pl_listeOPER != NULL; pl_listeOPER = pl_listeOPER->Suivant )
   {
      /*B OPERATOR concerne puisque unique par serveur de terminaux */
      /* si le OPER est en service et actif */
        if ( !(pl_listeOPER->Etat_Service & XDC_EQT_HORS_SRV) && !(pl_listeOPER->Etat_Service & XDC_EQT_INACTIF) ) break ;	
   }
   
   /*A Controle existance OPERATOR */
   if(pl_listeOPER == NULL)
   {       
      /* Trace : Impossibilite recuperation equipemt OPERATOR */
      XZST_03EcritureTrace(XZSTC_WARNING, "Equipement OPERATOR non repertorie." );
      XZST_10ArchiverMsgSyst(0,"ed_cmd_OPER : Equipement OPERATOR non repertorie", NULL);
      XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: ed_cmd_OPER ");
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
          XZST_03EcritureTrace ( XZSTC_WARNING, "Envoi message XDM_ETAT_OPER non effectue." );
       }
       else
       {
          XZST_03EcritureTrace ( XZSTC_MESSAGE,
 		"Envoi message XDM_ETAT_OPER effectue sur %s - %d - %d - %d " ,pl_DG,
 	    	pl_listeOPER->NoCdeCourante,
 		pl_listeOPER->CptRenduCde,
 		pl_listeOPER->EtatLiaison);
         				
       }
  
  
      return;
   }

  
  /*A Envoi de la commande vers la tache teomi */
  
  vl_CodeRetour = va_CodeCde ;
  sprintf(vl_NumOp, "%06d", atoi(va_NumOp) );
  
  strcpy(vl_Message, va_Message) ;
  
  switch(va_CodeCde) 
  {
     /*B Commande appel no telephone */
     case XDC_APPEL_NUM :
        sprintf(pl_mlcr,"PN,%s,%s\r0", vl_NumOp, vl_Message);
        XZST_03EcritureTrace(XZSTC_DEBUG1, "Callback ed_cmd_OPER = trame : %s", pl_mlcr );
 	EnvoyerTrame_Operator(pl_mlcr,
        		      vl_CodeRetour,
        		      "0",
		 	      XZEXC_NOCMD,
		 	      XZEXC_PRIORITEFORTE,
        		      pl_listeOPER->SocketMaitre);
        /* rangement etats dans table de structure */
        strcpy(pl_listeOPER->NumOperateur, vl_NumOp);
        pl_listeOPER->NoCdeCourante =  XDC_APPEL_NUM;
        pl_listeOPER->CptRenduCde = XDC_ATTENTE;       
  	break;
  	
     case XDC_APPEL_ALPHA :  
        sprintf ( pl_mlcr, "PA,%s,%s\r0", vl_NumOp, vl_Message );
        XZST_03EcritureTrace(XZSTC_DEBUG1, "Callback ed_cmd_OPER = trame : %s", pl_mlcr );
 	EnvoyerTrame_Operator(pl_mlcr,
        		      vl_CodeRetour,
        		      "0",
		 	      XZEXC_NOCMD,
		 	      XZEXC_PRIORITEFORTE,
        		      pl_listeOPER->SocketMaitre);
        /* rangement etats dans table de structure */
        strcpy(pl_listeOPER->NumOperateur, vl_NumOp) ;
        pl_listeOPER->NoCdeCourante =  XDC_APPEL_ALPHA ;
        pl_listeOPER->CptRenduCde  = XDC_ATTENTE ;       
        break ;
  }
  
  /*A fin module */
   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: ed_init_etat_OPER ");
	       
}




/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Envoi une trame a TEOMi.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int  EnvoyerTrame_Operator ( char*	trame,
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
*  Demande d'envoi de trame a teomi
*
------------------------------------------------------*/
{		     
static char *version = "$Id: eope_cmd.c,v 1.4 2020/11/03 18:09:45 pc2dpdy Exp $ : EnvoyerTrame_Operator" ;
   
XZEXT_MSG_SOCKET	   vl_message;     
        
        /*A Construction de la trame */
        sprintf(vl_message,"%s%d %s %d %d %s %s%s",
           			XZEXC_DEMANDE_OPE,
           			type_msg,
           			adr_rgs,
 		    		nocmd,
		    		priorite,
          			XZEXC_SEP_TRAME,
          			trame,
           			XZEXC_FIN_MSG_SOCK);
            			
        /* Trace du message socket */
        XZST_03EcritureTrace( XZSTC_DEBUG1, "Message socket : %s",vl_message );
           			
        /*A Envoi de la trame */
        sem_wait(&vg_semaphore); 
        if ( write ( socket , vl_message , strlen ( vl_message ) ) <= 0 ) 				  				
        {
           sem_post(&vg_semaphore); 
           /* Ecriture dans le fichier trace en cas d'�chec */
           XZST_03EcritureTrace( XZSTC_FONCTION, "Impossible d'�mettre la trame vers TEOMi." );
           return(XDC_NOK);
        }
        else
        {   
           sem_post(&vg_semaphore); 
           /*B Ecriture dans le fichier trace en cas de succes */
           XZST_03EcritureTrace( XZSTC_INFO, "Trame �mise vers TEOMi." );
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
   static char *version = "$Id: eope_cmd.c,v 1.4 2020/11/03 18:09:45 pc2dpdy Exp $ : ed_controle_cb" ;
   
    #ifndef _TIBCO_EMS
    T_STR			va_NomMachine;
   T_INT4			va_TypeEqt;
   T_INT4			va_NumEqt;
   T_INT4			va_Etat;
    #else
    XDY_NomMachine              va_NomMachine;
   XDY_Entier			va_TypeEqt;
   XDY_Entier			va_NumEqt;
   XDY_Entier			va_Etat;
    #endif  
   

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
      XZST_03EcritureTrace(XZSTC_WARNING, "messsage XDM_ECMD_EQPT non lu" );
      XZST_10ArchiverMsgSyst(0,"ed_controle_cb : Message XDM_ECMD_EQPT recu avec parametre(s) invalide(s)", NULL);
      XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: ed_controle_cb ");
      return;
   }
	      
   /* Trace du message en entree de la fonction */
   XZST_03EcritureTrace(XZSTC_WARNING, "callback ed_controle_cb, msg= %d %s %d %d",
                          va_Etat,
                          va_NomMachine,
                          va_TypeEqt,
                          va_NumEqt);
   
   
   /* si equipement operator */   
   if ( va_TypeEqt == XZECC_TYPE_EQT_MAA )
   {
      /*A Recherche du No d'equipement dans la structure OPERATOR en memoire */
      for ( pl_ListeOPER = pg_debutOPER; pl_ListeOPER != NULL; pl_ListeOPER = pl_ListeOPER->Suivant )
      {
	 /*A Affectation de l'etat */
	 if (pl_ListeOPER->Config.Numero == va_NumEqt)
	 {
            if ( va_Etat > XZEXC_LIMITE_ETAT_EQT )
               pl_ListeOPER->Etat_Service &= va_Etat;
            else
               pl_ListeOPER->Etat_Service += va_Etat;
	    break;
	 }
      }
   }      
   
   /* Trace des Differents etats service et activite */
   
   for ( pl_ListeOPER = pg_debutOPER; pl_ListeOPER != NULL ; 
	         pl_ListeOPER = pl_ListeOPER->Suivant )
   {
      XZST_03EcritureTrace(XZSTC_DEBUG1, "ServOPER:%d ", pl_ListeOPER->Etat_Service );
   }
   
   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: ed_controle_cb ");
}


