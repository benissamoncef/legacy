/*E*/
/* Fichier : $Id: xzel.c,v 1.11 2021/04/08 13:24:57 pc2dpdy Exp $      Release : $Revision: 1.11 $        Date : $Date: 2021/04/08 13:24:57 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE XZEL  * FICHIER xzel.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* c'est le module d'interface telephonique
*
------------------------------------------------------
* HISTORIQUE :
*
* Nouaille.M	version 1.1	29 Nov 1994	: Creation
* Mismer.D	version 1.2	27 Jan 1995	: Suppression controle parametres d'entrees
* Fontaine.C	version 1.3	24 Mai 1995	: RAS
* JMG		version 1.4	22/11/10	: radio/tel
* SDU		version 1.5	10/02/12 	: DEM 1015 -Communautaire
* JMG		version 1.6 14/04/12 	: dem 1016 regulation
* JMG   13/12/16 : ajout XZEL14 SAGA conf scenario   1.7 DEM1191
* JMG	12/06/19 : ajout XZEL16 ODI-C DEM1338 1.8
* ABE	15/12/20 : Ajout IMU DEM-SAE155 1.10
------------------------------------------------------*/

/* fichiers inclus */

#include "xzel.h"


/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */
static char *version = "$Id: xzel.c,v 1.11 2021/04/08 13:24:57 pc2dpdy Exp $ : xzel" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande appel numero telephone a tache TETEL
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZEL01_Demande_Appel_Tel(XDY_NomSite 		va_nomSite,
                             XDY_Tel			va_NumTel,
                             XDY_Operateur		va_NumPO )

/*
* ARGUMENTS EN ENTREE :
* va_nomSite : nom du site pour construire le datagroup
* va_NumTel :  no de telephone a appeler
* va_NumPO :   no du poste operateur demandeur
* 
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK			succes
*   XDC_NOK			echec : erreur envoi message datagroup
*   XDC_ERR_POSTE_OP		echec : numero de poste invalide 
*
* CONDITION D'UTILISATION
*    
*
* FONCTION 
*   Envoie le message XDM_ECMD_AUTOTEL sur le 
*   datagroup XDG_ECMD_AUTOTEL_<nomSite>
*
------------------------------------------------------*/
{
  static char *version = "$Id: xzel.c,v 1.11 2021/04/08 13:24:57 pc2dpdy Exp $ :XZEL01_Demande_Appel_Tel " ;

    
    /*A Trace des parametres en entree */
    
   XZST_03EcritureTrace(XZSTC_INTERFACE,
 	"IN: XZEL01_Demande_Appel_Tel avec %s %s %d ",
        va_nomSite,
        va_NumTel,
        va_NumPO);
  		
   /*A Envoi du message XDM_ECMD_AUTOTEL */
   if (Envoyer_Mess_Tel (va_nomSite,
                        XDC_APPEL,
                        va_NumTel,
                        va_NumPO) == XDC_NOK )
   {
      /*A Trace sortie fonction  avec XDC_ERR_POSTE_OP */   

      XZST_03EcritureTrace(XZSTC_INTERFACE
              ,"OUT: XZEL01_Demande_Appel_Tel avec XDC_NOK");
 	
      return ( XDC_NOK);
   }
   else
   {
      /*A Trace sortie fonction XZEL01_Demande_Appel_Tel avec XDC_OK */
   
      XZST_03EcritureTrace(XZSTC_INTERFACE
         ,"OUT: XZEL01_Demande_Appel_Tel avec XDC_OK");
 	
      return ( XDC_OK ); 
   }  
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande de renvoi vers repondeur a tache TETEL
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZEL02_Renvoi_Appel(XDY_NomSite 		va_nomSite,
                             XDY_Tel			va_NumRepondeur,
                             XDY_Operateur		va_NumPO )

/*
* ARGUMENTS EN ENTREE :
* va_nomSite : nom du site pour construire le datagroup
* va_NumRep :  no de repondeur a appeler
* va_NumPO :   no du poste operateur demandeur
* 
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK			succes
*   XDC_NOK			echec : erreur envoi message datagroup
*   XDC_ERR_POSTE_OP		echec : numero de poste invalide 
*
* CONDITION D'UTILISATION
*    
*
* FONCTION 
*   Envoie le message XDM_ECMD_AUTOTEL sur le 
*   datagroup XDG_ECMD_AUTOTEL_<nomSite>
*
------------------------------------------------------*/
{
  static char *version = "$Id: xzel.c,v 1.11 2021/04/08 13:24:57 pc2dpdy Exp $ :XZEL02_Renvoi_Appel " ;

    
    /*A Trace des parametres en entree */
    
   XZST_03EcritureTrace(XZSTC_INTERFACE,
 	"IN: XZEL02_Renvoi_Appel avec %s %s %d ",
        va_nomSite,
        va_NumRepondeur,
        va_NumPO);
 
 		
   /*A Envoi du message XDM_ECMD_AUTOTEL */
   if (Envoyer_Mess_Tel (va_nomSite,
                        XDC_RENVOI,
                        va_NumRepondeur,
                        va_NumPO) == XDC_NOK )
   {
      /*A Trace sortie fonction XZEL02_Renvoi_Appel avec XDC_ERR_POSTE_OP */   

      XZST_03EcritureTrace(XZSTC_INTERFACE
              ,"OUT: XZEL02_Renvoi_Appel avec XDC_NOK");
 	
      return ( XDC_NOK);
   }
   else
   {
      /*A Trace sortie fonction XZEL02_Renvoi_Appel avec XDC_OK */
   
      XZST_03EcritureTrace(XZSTC_INTERFACE
         ,"OUT: XZEL02_Renvoi_Appel avec XDC_OK");
 	
      return ( XDC_OK ); 
   }  
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande d'affectation canal moniteur
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZEL03_Annul_Renvoi     (XDY_NomSite 		va_nomSite,
                             XDY_Operateur		va_NumPO )

/*
* ARGUMENTS EN ENTREE :
* va_nomSite : nom du site pour construire le datagroup
* va_NumPO :   no du poste operateur demandeur
* 
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK			succes
*   XDC_NOK			echec : erreur envoi message datagroup
*   XDC_ERR_POSTE_OP		echec : numero de poste invalide 
*
* CONDITION D'UTILISATION
*    
*
* FONCTION 
*   Envoie le message XDM_ECMD_AUTOTEL sur le 
*   datagroup XDG_ECMD_AUTOTEL_<nomSite>
*
------------------------------------------------------*/
{
  static char *version = "$Id: xzel.c,v 1.11 2021/04/08 13:24:57 pc2dpdy Exp $ :XZEL03_Annul_Renvoi " ;
  
    
    /*A Trace des parametres en entree */
    
   XZST_03EcritureTrace(XZSTC_INTERFACE,
 	"IN: XZEL03_Annul_Renvoi avec %s %d ",
        va_nomSite,
        va_NumPO);
 
 		
   /*A Envoi du message XDM_ECMD_AUTOTEL */
   if (Envoyer_Mess_Tel (va_nomSite,
                        XDC_ANNUL_RENVOI,
                        "\0",
                        va_NumPO) == XDC_NOK )
   {
      /*A Trace sortie fonction XZEL03_Annul_Renvoi avec XDC_ERR_POSTE_OP */   

      XZST_03EcritureTrace(XZSTC_INTERFACE
              ,"OUT: XZEL03_Annul_Renvoi avec XDC_NOK");
 	
      return ( XDC_NOK);
   }
   else
   {
      /*A Trace sortie fonction XZEL03_Annul_Renvoi avec XDC_OK */
   
      XZST_03EcritureTrace(XZSTC_INTERFACE
         ,"OUT: XZEL03_Annul_Renvoi avec XDC_OK");
 	
      return ( XDC_OK ); 
   }  
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande d'envoi message 
*
------------------------------------------------------*/
 int Envoyer_Mess_Tel(XDY_NomSite 		va_nomSite,
                    int 			va_codecmd,
                    XDY_Tel			va_NumTel,
                    XDY_Operateur		va_NumPO )
                    

/*
* ARGUMENTS EN ENTREE :
* va_nomSite : nom du site 
* va_codecmd : no de commande
* va_NumTel :  no de telephone
* va_NumPO :   no du poste Operateur appelant
*
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK		succes
*   XDC_NOK		echec 
*
* CONDITION D'UTILISATION
*    
*
* FONCTION 
*   Envoie le message XDM_ECMD_AUTOTEL sur le 
*   datagroup XDG_ECMD_AUTOTEL_<nomSite>
*
------------------------------------------------------*/
{
XDY_Datagroup	pl_NomDG;

   /*A Construction du DatGroup XDG_ECMD_AUTOTEL_<nomSite> */
   
   XDG_EncodeDG2 ( pl_NomDG, XDG_ECMD_AUTOTEL, va_nomSite ) ;
   
   /*A Envoi du message XDM_ECMD_AUTOTEL en fournissant les valeur recues  */
   
   if (!TipcSrvMsgWrite (  XDG_ECMD_AUTOTEL
      			  ,XDM_IdentMsg(XDM_ECMD_AUTOTEL2)
  			  ,XDM_FLG_SRVMSGWRITE
 			  ,T_IPC_FT_INT4,va_codecmd
 			  ,T_IPC_FT_STR, va_NumTel
 			  ,T_IPC_FT_INT2,va_NumPO
			  ,T_IPC_FT_STR,va_nomSite
                          ,NULL))
      	{
  		/*A Trace envoi message XDM_ECMD_AUTOTEL non effectue */    	
      	
         	XZST_03EcritureTrace(XZSTC_INFO
		   ,"Envoi message XDM_ECMD_AUTOTEL non effectue \n");
    
         	return(	XDC_NOK );		
	}
        else
        {
                /*A RAZ Buffer envoi du message */
   
                TipcSrvFlush();

                /*A Trace envoi message XDM_ECMD_VIDEO effectue avec succes */    	
      	
                XZST_03EcritureTrace(XZSTC_INFO
                   ,"Envoi message XDM_ECMD_AUTOTEL effectue avec succes sur %s\n",XDG_ECMD_AUTOTEL);
                   
         	return(	XDC_OK );		
        }
 
}





/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  mise en garde
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZEL04_Mise_En_Garde(XDY_NomSite 		va_nomSite,
                             XDY_Operateur		va_NumPO )

/*
* ARGUMENTS EN ENTREE :
* va_nomSite : nom du site pour construire le datagroup
* va_NumPO :   no du poste operateur demandeur
* 
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK			succes
*   XDC_NOK			echec : erreur envoi message datagroup
*   XDC_ERR_POSTE_OP		echec : numero de poste invalide 
*
* CONDITION D'UTILISATION
*    
*
* FONCTION 
*   Envoie le message XDM_ECMD_AUTOTEL sur le 
*   datagroup XDG_ECMD_AUTOTEL_<nomSite>
*
------------------------------------------------------*/
{
	return (Envoyer_Mess_Tel (va_nomSite,
	                        XDC_RENVOI,
			        "\0",
			        va_NumPO))
				;

}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  reprise
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZEL05_Reprise(XDY_NomSite 		va_nomSite,
                             XDY_Operateur		va_NumPO )

/*
* ARGUMENTS EN ENTREE :
* va_nomSite : nom du site pour construire le datagroup
* va_NumRep :  no de repondeur a appeler
* va_NumPO :   no du poste operateur demandeur
* 
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK			succes
*   XDC_NOK			echec : erreur envoi message datagroup
*   XDC_ERR_POSTE_OP		echec : numero de poste invalide 
*
* CONDITION D'UTILISATION
*    
*
* FONCTION 
*   Envoie le message XDM_ECMD_AUTOTEL sur le 
*   datagroup XDG_ECMD_AUTOTEL_<nomSite>
*
------------------------------------------------------*/
{
 return (Envoyer_Mess_Tel (va_nomSite,
                         XDC_ANNUL_RENVOI,
                      "\0",
                     va_NumPO));

}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande appel radio
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZEL06_Demande_Appel_Radio (
        XDY_NomSite va_nomSite,
        XDY_TypeAppelRadio va_typeAppel,
        XDY_GroupeRadio va_numeroGroupeRadio,
        XDY_Radio va_numeroCibleAppel,
        XDY_Operateur va_NumPO)

/*
* ARGUMENTS EN ENTREE :
* va_nomSite : nom du site pour construire le datagroup
* va_NumRep :  no de repondeur a appeler
* va_NumPO :   no du poste operateur demandeur
* 
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK			succes
*   XDC_NOK			echec : erreur envoi message datagroup
*   XDC_ERR_POSTE_OP		echec : numero de poste invalide 
*
* CONDITION D'UTILISATION
*    
*
* FONCTION 
*   Envoie le message XDM_ECMD_AUTOTEL sur le 
*   datagroup XDG_ECMD_AUTOTEL_<nomSite>
*
------------------------------------------------------*/
{
 return (Envoyer_Mess_Rad(va_nomSite,va_typeAppel,
 		va_numeroGroupeRadio,va_numeroCibleAppel,va_NumPO));
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  appel radio
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int Envoyer_Mess_Rad (
        XDY_NomSite va_nomSite,
        XDY_TypeAppelRadio va_typeAppel,
        XDY_GroupeRadio va_numeroGroupeRadio,
        XDY_Radio va_numeroCibleAppel,
         XDY_Operateur va_NumPO)

/*
* ARGUMENTS EN ENTREE :
* va_nomSite : nom du site pour construire le datagroup
* va_NumRep :  no de repondeur a appeler
* va_NumPO :   no du poste operateur demandeur
* 
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK			succes
*   XDC_NOK			echec : erreur envoi message datagroup
*   XDC_ERR_POSTE_OP		echec : numero de poste invalide 
*
* CONDITION D'UTILISATION
*    
*
* FONCTION 
*   Envoie le message XDM_ECMD_AUTOTEL sur le 
*   datagroup XDG_ECMD_AUTOTEL_<nomSite>
*
------------------------------------------------------*/
{

if (!TipcSrvMsgWrite (  XDG_ECMD_AUTORAD
        ,XDM_IdentMsg(XDM_ECMD_AUTORAD)
                ,XDM_FLG_SRVMSGWRITE
                        ,T_IPC_FT_INT4, va_typeAppel
                                ,T_IPC_FT_STR,  va_numeroGroupeRadio
                                        ,T_IPC_FT_STR,  va_numeroCibleAppel
                                                ,T_IPC_FT_INT2, va_NumPO
                                                        ,T_IPC_FT_STR,  va_nomSite
                                                                ,NULL))

        {
                /*A Trace envoi message XDM_ECMD_AUTOTEL non effectue */

                XZST_03EcritureTrace(XZSTC_INFO
                   ,"Envoi message XDM_ECMD_AUTORAD non effectue \n");

                return( XDC_NOK );
        }
        else
        {
                /*A RAZ Buffer envoi du message */

                TipcSrvFlush();

                /*A Trace envoi message XDM_ECMD_VIDEO effectue avec succes */

                XZST_03EcritureTrace(XZSTC_INFO
                   ,"Envoi message XDM_ECMD_AUTORAD effectue avec succes sur %s\n",XDG_ECMD_AUTORAD);
XZST_03EcritureTrace(XZSTC_INFO, "Envoyer_Mess_Rad message appel : type=%d",va_typeAppel);
    XZST_03EcritureTrace(XZSTC_INFO, "Envoyer_Mess_Rad message appel :  numeroGroupeRadio:%s",va_numeroGroupeRadio);
    XZST_03EcritureTrace(XZSTC_INFO, "Envoyer_Mess_Rad message appel : numeroCibleAppel:%s",va_numeroCibleAppel);
    XZST_03EcritureTrace(XZSTC_INFO, "Envoyer_Mess_Rad message appel : NumPO:%d",va_NumPO);
    XZST_03EcritureTrace(XZSTC_INFO, "Envoyer_Mess_Rad message appel : nomSite:%s",va_nomSite);
                return( XDC_OK );
        }


}


/*------------------------------------------------------
* SERVICE RENDU :
*  Demande d'acquitement du message communautaire
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZEL10_Acquitter_Communautaire (XDY_Entier va_idComm)
/*
* ARGUMENTS EN ENTREE :
* id_Commm : id du message communautaire
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR :
*   XDC_OK			succes
*   XDC_NOK			echec : erreur envoi message datagroup
*
* CONDITION D'UTILISATION
*
* FONCTION
*   Envoie le message XDM_COMMU_ACQ sur le
*   datagroup XDG_COMMUNAUTAIRE_<nomSite>
*
------------------------------------------------------*/
{
  static char *version = "$Id: xzel.c,v 1.11 2021/04/08 13:24:57 pc2dpdy Exp $ :XZEL10_Acquitter_Communautaire " ;

   XZST_03EcritureTrace(XZSTC_INTERFACE,
 	"IN: XZEL10_Acquitter_Communautaire id:%d ",
 		va_idComm);


   /*Envoi du message XDM_COMMU_ACK */
   if (!TipcSrvMsgWrite (  XDG_COMMUNAUTAIRE
           ,XDM_IdentMsg(XDM_COMMU_ACQ)
		   ,XDM_FLG_SRVMSGWRITE
		   ,T_IPC_FT_INT4, va_idComm
		   ,NULL))
   {
	   XZST_03EcritureTrace(XZSTC_INFO
		  ,"Envoi message XDM_COMMU_ACQ non effectue \n");
	   XZST_03EcritureTrace(XZSTC_INTERFACE
			   ,"OUT: XZEL10_Acquitter_Communautaire avec XDC_NOK");

	   return( XDC_NOK );
   }
   else
   {
	   /*RAZ Buffer envoi du message */
	   TipcSrvFlush();

	   XZST_03EcritureTrace(XZSTC_INFO
		  ,"Envoi message XDM_COMMU_ACQ effectue avec succes sur %s\n",XDG_COMMUNAUTAIRE);
	   XZST_03EcritureTrace(XZSTC_INTERFACE
			   ,"OUT: XZEL10_Acquitter_Communautaire avec XDC_OK");
	   return( XDC_OK );
   }
}


/*------------------------------------------------------
* SERVICE RENDU :
*  Demande de rapprochement du message communautaire avec une FMC
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZEL11_Rapprocher_Communautaire (XDY_Entier va_idComm,
		 XDY_Evt va_idFMC, char *va_type)
/*
* ARGUMENTS EN ENTREE :
* id_Commm : id du message communautaire
* idFMC : id de la FMC
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR :
*   XDC_OK			succes
*   XDC_NOK			echec : erreur envoi message datagroup
*
* CONDITION D'UTILISATION
*
* FONCTION
*   Envoie le message XDM_COMMU_RAPPROCHE sur le
*   datagroup XDG_COMMUNAUTAIRE_<nomSite>
*
------------------------------------------------------*/
{
  static char *version = "$Id: xzel.c,v 1.11 2021/04/08 13:24:57 pc2dpdy Exp $ :XZEL10_Acquitter_Communautaire " ;

   /* Trace des parametres en entree */
   XZST_03EcritureTrace(XZSTC_INTERFACE,
 	"IN: XZEL11_Rapprocher_Communautaire id:%d fmc:%d/%d type:%s",
 		va_idComm,va_idFMC.numero,va_idFMC.cle, va_type);

   /*Envoi du message XDM_COMMU_ACK */
   if (!TipcSrvMsgWrite (  XDG_COMMUNAUTAIRE
           ,XDM_IdentMsg(XDM_COMMU_RAPPROCHE)
		   ,XDM_FLG_SRVMSGWRITE
		   ,T_IPC_FT_INT4, va_idComm
		   ,T_IPC_FT_INT4, va_idFMC.numero
		   ,T_IPC_FT_INT2, va_idFMC.cle
		   ,T_IPC_FT_STR, va_type
		   ,NULL))
   {
	   XZST_03EcritureTrace(XZSTC_INFO
		  ,"Envoi message XDM_COMMU_RAPPROCHE non effectue \n");
	   XZST_03EcritureTrace(XZSTC_INTERFACE
			   ,"OUT: XZEL11_Rapprocher_Communautaire avec XDC_NOK");

	   return( XDC_NOK );
   }
   else
   {
	   /*RAZ Buffer envoi du message */
	   TipcSrvFlush();

	   XZST_03EcritureTrace(XZSTC_INFO
		  ,"Envoi message XDM_COMMU_RAPPROCHE effectue avec succes sur %s\n",XDG_COMMUNAUTAIRE);
	   XZST_03EcritureTrace(XZSTC_INTERFACE
			   ,"OUT: XZEL11_Rapprocher_Communautaire avec XDC_OK");
	   return( XDC_OK );
   }
}


/*------------------------------------------------------
* SERVICE RENDU :
*  Demande d'ignorer le message communautaire
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZEL12_Ignorer_Communautaire (XDY_Entier va_idComm,
								  XDY_Octet va_motif)
/*
* ARGUMENTS EN ENTREE :
* id_Commm : id du message communautaire
* motif : motif
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR :
*   XDC_OK			succes
*   XDC_NOK			echec : erreur envoi message datagroup
*
* CONDITION D'UTILISATION
*
* FONCTION
*   Envoie le message XDM_COMMU_IGNORE sur le
*   datagroup XDG_COMMUNAUTAIRE_<nomSite>
*
------------------------------------------------------*/
{
  static char *version = "$Id: xzel.c,v 1.11 2021/04/08 13:24:57 pc2dpdy Exp $ :XZEL10_Acquitter_Communautaire " ;

   /* Trace des parametres en entree */
   XZST_03EcritureTrace(XZSTC_INTERFACE,
 	"IN: XZEL12_Ignorer_Communautaire id:%d motif:%d",
 		va_idComm,
 		va_motif);


   /*Envoi du message XDM_COMMU_IGNORE */
   if (!TipcSrvMsgWrite (  XDG_COMMUNAUTAIRE
           ,XDM_IdentMsg(XDM_COMMU_IGNORE)
		   ,XDM_FLG_SRVMSGWRITE
		   ,T_IPC_FT_INT4, va_idComm
		   ,T_IPC_FT_INT2, va_motif
		   ,NULL))
   {
	   XZST_03EcritureTrace(XZSTC_INFO
		  ,"Envoi message XDM_COMMU_IGNORE non effectue \n");
	   XZST_03EcritureTrace(XZSTC_INTERFACE
			   ,"OUT: XZEL12_Ignorer_Communautaire avec XDC_NOK");

	   return( XDC_NOK );
   }
   else
   {
	   /*RAZ Buffer envoi du message */
	   TipcSrvFlush();

	   XZST_03EcritureTrace(XZSTC_INFO
		  ,"Envoi message XDM_COMMU_IGNORE effectue avec succes sur %s\n",XDG_COMMUNAUTAIRE);
	   XZST_03EcritureTrace(XZSTC_INTERFACE
			   ,"OUT: XZEL12_Ignorer_Communautaire avec XDC_OK");
	   return( XDC_OK );
   }
}
/*------------------------------------------------------
* SERVICE RENDU :
* acquittement d'une alerte communautaire
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZEL13_Acquitter_Alerte_Regulation (char *va_site,
				  char  *va_texte,
				  int va_zone,
				  int va_qualif, int va_numero)
/*
* ARGUMENTS EN ENTREE :
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR :
*   XDC_OK			succes
*   XDC_NOK			echec : erreur envoi message datagroup
*
* CONDITION D'UTILISATION
*
* FONCTION
*   Envoie le message XDM_COMMU_IGNORE sur le
*   datagroup XDG_COMMUNAUTAIRE_<nomSite>
*
------------------------------------------------------*/
{
  static char *version = "$Id: xzel.c,v 1.11 2021/04/08 13:24:57 pc2dpdy Exp $ :XZEL13_Acquitter_Alerte_Regulation " ;
  char vl_dg[50];
   /* Trace des parametres en entree */
   XZST_03EcritureTrace(XZSTC_INTERFACE,
 	"IN: XZEL13_Acquitter_Alerte_Regulation site:%s texte:%s zone:%d qualif:%d num:%d",
 		va_site,
 		va_texte, va_zone, va_qualif, va_numero);

   sprintf(vl_dg,"%s_%s",XDG_REGUL_APPLICATION,va_site);
   /*Envoi du message XDM_ACQUITTE_REGUL*/
   if (!TipcSrvMsgWrite (  vl_dg
           ,XDM_IdentMsg(XDM_ACQUITTE_REGUL)
		   ,XDM_FLG_SRVMSGWRITE
		   ,T_IPC_FT_INT4, va_zone
		   ,T_IPC_FT_INT2, va_qualif
		   ,T_IPC_FT_STR, va_texte
		   , T_IPC_FT_INT4, va_numero
		   ,NULL))
   {
	   XZST_03EcritureTrace(XZSTC_INFO
		  ,"Envoi message XDM_ACQUITTE_REGUL non effectue \n");
	   XZST_03EcritureTrace(XZSTC_INTERFACE
			   ,"OUT: XZEL13_Acquitter_Alerte_Regulation avec XDC_NOK");

	   return( XDC_NOK );
   }
   else
   {
	   /*RAZ Buffer envoi du message */
	   TipcSrvFlush();

	   XZST_03EcritureTrace(XZSTC_INFO
		  ,"Envoi message XDM_ACQUITTE_REGUL effectue avec succes sur %s\n",vl_dg);
	   XZST_03EcritureTrace(XZSTC_INTERFACE
			   ,"OUT: XZEL13_Acquitter_Alerte_Regulation avec XDC_OK");
	   return( XDC_OK );
   }
}





/*------------------------------------------------------
* SERVICE RENDU :
* acquittement d'une alerte conf scenario
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZEL14_Acquitter_Alerte_Scenario_SAGA ( char  *va_texte,
				  int va_qualif)
/*
* ARGUMENTS EN ENTREE :
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR :
*   XDC_OK			succes
*   XDC_NOK			echec : erreur envoi message datagroup
*
* CONDITION D'UTILISATION
*
* FONCTION
*   Envoie le message XDM_COMMU_IGNORE sur le
*   datagroup XDG_COMMUNAUTAIRE_<nomSite>
*
------------------------------------------------------*/
{
  static char *version = "$Id: xzel.c,v 1.11 2021/04/08 13:24:57 pc2dpdy Exp $ :XZEL13_Acquitter_Alerte_Regulation " ;
   /* Trace des parametres en entree */
   XZST_03EcritureTrace(XZSTC_INTERFACE,
 	"IN: XZEL14_Acquitter_Alerte_Scenario_SAGA texte:%s qualif:%d",
 		va_texte, va_qualif );

   /*Envoi du message XDM_ACQUITTE_REGUL*/
   if (!TipcSrvMsgWrite (  XDG_ACQUITTE_SAGA
           ,XDM_IdentMsg(XDM_ACQUITTE_REGUL)
		   ,XDM_FLG_SRVMSGWRITE
		   ,T_IPC_FT_INT4, 0
		   ,T_IPC_FT_INT2, va_qualif
		   ,T_IPC_FT_STR, va_texte
		   , T_IPC_FT_INT4, 0
		   ,NULL))
   {
	   XZST_03EcritureTrace(XZSTC_INFO
		  ,"Envoi message XDM_ACQUITTE_REGUL non effectue \n");
	   XZST_03EcritureTrace(XZSTC_INTERFACE
			   ,"OUT: XZEL13_Acquitter_Alerte_Regulation avec XDC_NOK");

	   return( XDC_NOK );
   }
   else
   {
	   /*RAZ Buffer envoi du message */
	   TipcSrvFlush();

	   XZST_03EcritureTrace(XZSTC_INFO
		  ,"Envoi message XDM_ACQUITTE_REGUL effectue avec succes \n");
	   XZST_03EcritureTrace(XZSTC_INTERFACE
			   ,"OUT: XZEL13_Acquitter_Alerte_Regulation avec XDC_OK");
	   return( XDC_OK );
   }
}



/*------------------------------------------------------
* SERVICE RENDU :
* acquittement d'une alerte conf scenario
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZEL15_Acquitter_Alerte_GABEB (char *va_site,
                                  char  *va_texte,
			                           int va_zone,
			                 int va_qualif, int va_numero )
/*
* ARGUMENTS EN ENTREE :
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR :
*   XDC_OK			succes
*   XDC_NOK			echec : erreur envoi message datagroup
*
* CONDITION D'UTILISATION
*
* FONCTION
*   Envoie le message XDM_COMMU_IGNORE sur le
*   datagroup XDG_COMMUNAUTAIRE_<nomSite>
*
------------------------------------------------------*/
{ XDY_Datagroup vl_dg;
  static char *version = "$Id: xzel.c,v 1.11 2021/04/08 13:24:57 pc2dpdy Exp $ :XZEL13_Acquitter_Alerte_Regulation " ;
   /* Trace des parametres en entree */
   XZST_03EcritureTrace(XZSTC_INTERFACE,
 	"IN: XZEL15_Acquitter_Alerte_GABEB texte:%s qualif:%d",
 		va_texte, va_qualif );

   /*Envoi du message XDM_ACQUITTE_REGUL*/
   sprintf(vl_dg,"%s_%s",XDG_GABEB_DETECTION,va_site);
   if (!TipcSrvMsgWrite (  vl_dg
           ,XDM_IdentMsg(XDM_ACQUITTE_GABEB)
		   ,XDM_FLG_SRVMSGWRITE
		   ,T_IPC_FT_INT4, va_zone
		   ,T_IPC_FT_INT2, va_qualif
		   ,T_IPC_FT_STR, va_texte
		   , T_IPC_FT_INT4, va_numero
		   ,NULL))
   {
	   XZST_03EcritureTrace(XZSTC_INFO
		  ,"Envoi message XDM_ACQUITTE_GABEB non effectue \n");
	   XZST_03EcritureTrace(XZSTC_INTERFACE
			   ,"OUT: XZEL15_Acquitter_Alerte_GABEB avec XDC_NOK");

	   return( XDC_NOK );
   }
   else
   {
	   /*RAZ Buffer envoi du message */
	   TipcSrvFlush();

	   XZST_03EcritureTrace(XZSTC_INFO
		  ,"Envoi message XDM_ACQUITTE_REGUL effectue avec succes \n");
	   XZST_03EcritureTrace(XZSTC_INTERFACE
			   ,"OUT: XZEL15_Acquitter_Alerte_GABEB avec XDC_OK");
	   return( XDC_OK );
   }
   }




/*------------------------------------------------------
* SERVICE RENDU :
* acquittement d'une alerte conf scenario
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZEL16_SMS ( char  *va_po,
		XDY_Octet va_type_fmc,
		char *va_portable,
		char *va_depanneur,
		char *va_horodate)
/*
* ARGUMENTS EN ENTREE :
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR :
*   XDC_OK			succes
*   XDC_NOK			echec : erreur envoi message datagroup
*
* CONDITION D'UTILISATION
*
* FONCTION
*   Envoie le message XDM_COMMU_IGNORE sur le
*   datagroup XDG_COMMUNAUTAIRE_<nomSite>
*
------------------------------------------------------*/
{
  static char *version = "$Id: xzel.c,v 1.11 2021/04/08 13:24:57 pc2dpdy Exp $ :XZEL13_Acquitter_Alerte_Regulation " ;
   /* Trace des parametres en entree */
   XZST_03EcritureTrace(XZSTC_INTERFACE,
 	"IN: XZEL16_SMS po:%s type:%d portable:%s depanneur:%s horodate:%s",
 		va_po, va_type_fmc, va_portable, va_depanneur, va_horodate );

   /*Envoi du message XDM_ACQUITTE_REGUL*/
   if (!TipcSrvMsgWrite (  XDG_TIMER_POLYVOX
           ,XDM_IdentMsg(XDM_ODI_C)
		   ,XDM_FLG_SRVMSGWRITE
		   ,T_IPC_FT_STR, va_po
		   ,T_IPC_FT_INT2, va_type_fmc
		   ,T_IPC_FT_STR, va_portable
		   ,T_IPC_FT_STR, va_depanneur
		   ,T_IPC_FT_STR, va_horodate
		   ,NULL))
   {
	   XZST_03EcritureTrace(XZSTC_INFO
		  ,"Envoi message XDM_ODI_C non effectue \n");
	   XZST_03EcritureTrace(XZSTC_INTERFACE
			   ,"OUT: XZEL16_SMS avec XDC_NOK");

	   return( XDC_NOK );
   }
   else
   {
	   /*RAZ Buffer envoi du message */
	   TipcSrvFlush();

	   XZST_03EcritureTrace(XZSTC_INTERFACE
			   ,"OUT: XZEL16_SMS avec XDC_OK");
	   return( XDC_OK );
   }
}


int XZEL24_Commande_IMU (       XZELT_CMD_IMU   va_Commande,
                                         int             va_NoCMD,
                                XDY_NomMachine  pa_NomMachine)
{
static char *version = "@(#)xzep.c      1.18 03/26/07 : XZEL24_Commande_IMU" ;
XDY_Datagroup   pl_NomDG;

 XZST_03EcritureTrace(XZSTC_INTERFACE,
		"XZEL24_Commande_IMU avec %d %s %d %d %d %d %s",
		va_Commande.Numero,
		va_Commande.ChaineIMU,
		va_Commande.Intervalle,
		va_Commande.Cycles,
		va_Commande.Duree,				
		va_NoCMD,
		pa_NomMachine);


  XDG_EncodeDG2 ( pl_NomDG, XDG_ECMD_IMU, strtok ( pa_NomMachine, " ") ) ;
	
   if (!TipcSrvMsgWrite (  pl_NomDG
                          ,XDM_IdentMsg(XDM_ECMD_IMU)
		,XDM_FLG_SRVMSGWRITE
		,T_IPC_FT_INT2, va_Commande.Numero
		,T_IPC_FT_STR,  va_Commande.ChaineIMU
		,T_IPC_FT_INT4, va_Commande.Intervalle
		,T_IPC_FT_INT4, va_Commande.Cycles
		,T_IPC_FT_INT4, va_Commande.Duree
		,T_IPC_FT_INT4, va_NoCMD
		,T_IPC_FT_STR, pa_NomMachine
                          ,NULL))
        {
                XZST_03EcritureTrace(XZSTC_WARNING
                   ,"Envoi message XDM_ECMD_IMU non effectue");

                /*A Trace sortie fonction XZEL24_Commande_IMU avec XDC_NOK */
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT: XZEL24_Commande_IMU avec XDC_NOK");
                return ( XDC_NOK );

        }
	TipcSrvFlush();
   return ( XDC_OK );
}

