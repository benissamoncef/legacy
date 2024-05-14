/*E*/
/*  Fichier : $Id: xzeu.c,v 1.14 2020/10/01 12:22:44 gesconf Exp $      Release : $Revision: 1.14 $        Date : $Date: 2020/10/01 12:22:44 $
------------------------------------------------------
* GRANIOU *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE XZEU  * FICHIER xzeu.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* c'est le module d'interface de la rau
*
------------------------------------------------------
* HISTORIQUE :
*
* Nagiel.E	version 1.1	29 Nov 1994	: Creation
* Nagiel.E	version 1.2	05 Dec 1994	:
* Nagiel.E	version 1.3	05 Dec 1994	:
* Nagiel.E	version 1.4	05 Dec 1994	:
* Nagiel.E	version 1.5	05 Dec 1994	:
* Nagiel.E	version 1.6	05 Dec 1994	:
* Nagiel.E	version 1.7	16 Jan 1995	:
* Fontaine.C	version 1.8	24 Mai 1995	: Reprise historique
* Mismer.D	version 1.9	24 Mai 1998	: Ajout num antenne pour GN PC2 
* JMG		version 1.10	22 oct 2007 	: ajout XZEU10_Demande_Etat_Bascul_RAU DEM716
* JMG		version 1.11	17 aou 2016 	: ajout XZEU11_Bascul_RAU
* JMG   09/03/17        : REGIONALISATION - DEM1220 1.13
* PNI	25/09/20	: Appel PAU multi platine SAE_178 1.14
* LCL	03/10/23	: Adaptations CNA DEM-483
------------------------------------------------------*/

/* fichiers inclus */

#include "xzeu.h"


/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */
static char *version = "$Id: xzeu.c,v 1.14 2020/10/01 12:22:44 gesconf Exp $ : xzeu.c" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande d'envoi message 
*
------------------------------------------------------*/
int XZEU_Envoyer_message(XDY_NomSite 	va_nomSite,
                     int 		va_codecmd,
                     int 		va_numPAU,
                     int 		va_noPlat)


/*
* ARGUMENTS EN ENTREE :
* va_nomSite : nom du site 
* va_codecmd : no commande
* va_numPAU : no de pau
* va_noPlat : no platine
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
*   Envoie le message XDM_ECMD_RAU sur le 
*   datagroup XDG_ECMD_RAU_<nomSite>
*
------------------------------------------------------*/
{
XDY_Datagroup	pl_NomDG;

   /*A Construction du DatGroup XDG_ECMD_RAU_<nomSite> */
   
   XDG_EncodeDG2 ( pl_NomDG, XDG_ECMD_RAU, va_nomSite ) ;
   
   /*A Envoi du message XDM_ECMD_RAU en fournissant les valeur recues  */
   
   if (!TipcSrvMsgWrite (  pl_NomDG
      			  ,XDM_IdentMsg(XDM_ECMD_RAU)
  			  ,XDM_FLG_SRVMSGWRITE
 			  ,T_IPC_FT_INT4,va_codecmd
 			  ,T_IPC_FT_INT4,va_numPAU
 			  ,T_IPC_FT_INT4,va_noPlat
 			  ,NULL))
      	{
  		/*A Trace envoi message XDM_ECMD_RAU non effectue */    	
      	
         	XZST_03EcritureTrace(XZSTC_INFO
 		   ,"Envoi message XDM_ECMD_RAU non effectue \n");
    
         	return(	XDC_NOK );		
	}
        else
        {
                /*A Envoi du message */
   
                TipcSrvFlush();

                /*A Trace envoi message XDM_ECMD_RAU effectue avec succes */    	
      	
                XZST_03EcritureTrace(XZSTC_INFO
                   ,"Envoi message XDM_ECMD_RAU effectue avec succes sur %s\n",pl_NomDG);
                   
         	return(	XDC_OK );		
        }
 
}
/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande mise en communication pau-platine
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZEU01_Demande_Mise_En_Comm(XDY_NomSite 	va_nomSite,
                                  int 		va_NumPAU,
                                  int 		va_PO)

/*
* ARGUMENTS EN ENTREE :
* va_nomSite : nom du site pour construire le datagroup
* va_NumPAU : no pau
* va_PO : Numero poste operateur = no platine telephone
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK		succes
*   XDC_NOK		echec 
*   XZEUC_ERR_PO        po invalide
*
* CONDITION D'UTILISATION
*    
*
* FONCTION 
*   Envoie le message XDM_ECMD_RAU sur le 
*   datagroup XDG_ECMD_RAU_<nomSite>
*
------------------------------------------------------*/
{
static char *version = "$Id: xzeu.c,v 1.14 2020/10/01 12:22:44 gesconf Exp $ : XZEU01_Demande_Mise_En_Comm" ;
    
    /*A Trace des parametres en entree */
    
   XZST_03EcritureTrace(XZSTC_INTERFACE,
 	"IN: XZEU01_Demande_Mise_En_Comm avec %s %d %d",
        va_nomSite,
        va_NumPAU,
        va_PO);
 
   /*A Controle du no platine */
   if ((va_PO != XZEUC_PLATINE_PO1) && (va_PO != XZEUC_PLATINE_PO2))
   {
      /*A Trace sortie fonction  avec XDC_NOK */   

      XZST_03EcritureTrace(XZSTC_INTERFACE
              ,"OUT: XZEU01_Demande_Mise_En_Comm avec XZEUC_ERR_PO");
 	
      return ( XZEUC_ERR_PO );
   }
 	
   /*A Envoi du message XDM_ECMD_RAU */
   if (XZEU_Envoyer_message (va_nomSite,
                        XZEUC_DEM_COMM,
                        va_NumPAU,
                        va_PO) == XDC_NOK )
   {
      /*A Trace sortie fonction  avec XDC_NOK */   

      XZST_03EcritureTrace(XZSTC_INTERFACE
              ,"OUT: XZEU01_Demande_Mise_En_Comm avec XDC_NOK");
 	
      return ( XDC_NOK);
   }
   else
   {
      /*A Trace sortie fonction XZEU01_Demande_Mise_En_Comm avec XDC_OK */
   
      XZST_03EcritureTrace(XZSTC_INTERFACE
         ,"OUT: XZEU01_Demande_Mise_En_Comm avec XDC_OK");
 	
      return ( XDC_OK ); 
   }  
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande d'appel borne
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZEU02_Appel_Borne(XDY_NomSite 	va_nomSite,
                       int 		va_NumPAU,
                       int 		va_PO)

/*
* ARGUMENTS EN ENTREE :
* va_nomSite : nom du site pour construire le datagroup
* va_NumPAU : no pau
* va_PO : Numero poste operateur = no platine telephone
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK		succes
*   XDC_NOK		echec 
*   XZEUC_ERR_PO        po invalide
*
* CONDITION D'UTILISATION
*    
*
* FONCTION 
*   Envoie le message XDM_ECMD_RAU sur le 
*   datagroup XDG_ECMD_RAU_<nomSite>
*
------------------------------------------------------*/
{
static char *version = "$Id: xzeu.c,v 1.14 2020/10/01 12:22:44 gesconf Exp $ : XZEU02_Appel_Borne" ;
    
    /*A Trace des parametres en entree */
    
   XZST_03EcritureTrace(XZSTC_WARNING,
 	"IN: XZEU02_Appel_Borne avec %s %d %d",
        va_nomSite,
        va_NumPAU,
        va_PO);
 
   /*A Controle du no platine, � modifier pour DEM SAE_178 */
   /*if ((va_PO != XZEUC_PLATINE_PO1) && (va_PO != XZEUC_PLATINE_PO2)
   && (va_PO != XZEUC_PLATINE_PO1+10) && (va_PO != XZEUC_PLATINE_PO2+10)) */
   if (va_PO > 29)
   {
      /*A Trace sortie fonction  avec XDC_NOK */   

      XZST_03EcritureTrace(XZSTC_WARNING
              ,"OUT: XZEU02_Appel_Borne avec XZEUC_ERR_PO");
 	
      return ( XZEUC_ERR_PO );
   }
 	
   /*A Envoi du message XDM_ECMD_RAU */
   if (XZEU_Envoyer_message (va_nomSite,
                        XZEUC_APPEL,
                        va_NumPAU,
                        va_PO) == XDC_NOK )
   {
      /*A Trace sortie fonction  avec XDC_NOK */   

      XZST_03EcritureTrace(XZSTC_WARNING
              ,"OUT: XZEU02_Appel_Borne x  avec XDC_NOK");
 	
      return ( XDC_NOK);
   }
   else
   {
      /*A Trace sortie fonction XZEU02_Appel_Borne avec XDC_OK */
   
      XZST_03EcritureTrace(XZSTC_WARNING
         ,"OUT: XZEU02_Appel_Borne avec XDC_OK");
 	
      return ( XDC_OK ); 
   }  
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande liberation d'une ligne
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZEU03_Liberation_Ligne(XDY_NomSite 	va_nomSite,
                            int 		va_NumPAU,
                            int 		va_PO)

/*
* ARGUMENTS EN ENTREE :
* va_nomSite : nom du site pour construire le datagroup
* va_NumPAU : no pau
* va_PO : Numero poste operateur = no platine telephone
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK		succes
*   XDC_NOK		echec 
*   XZEUC_ERR_PO        po invalide
*
* CONDITION D'UTILISATION
*    
*
* FONCTION 
*   Envoie le message XDM_ECMD_RAU sur le 
*   datagroup XDG_ECMD_RAU_<nomSite>
*
------------------------------------------------------*/
{
static char *version = "$Id: xzeu.c,v 1.14 2020/10/01 12:22:44 gesconf Exp $ : XZEU03_Liberation_Ligne" ;
    
    /*A Trace des parametres en entree */
    
   XZST_03EcritureTrace(XZSTC_INTERFACE,
 	"IN: XZEU03_Liberation_Ligne avec %s %d %d",
        va_nomSite,
        va_NumPAU,
        va_PO);
 
   /*A Controle du no platine */
   if ((va_PO != XZEUC_PLATINE_PO1) && (va_PO != XZEUC_PLATINE_PO2))
   {
      /*A Trace sortie fonction  avec XDC_NOK */   

      XZST_03EcritureTrace(XZSTC_INTERFACE
              ,"OUT: XZEU03_Liberation_Ligne avec XZEUC_ERR_PO");
 	
      return ( XZEUC_ERR_PO );
   }
 	
   /*A Envoi du message XDM_ECMD_RAU */
   if (XZEU_Envoyer_message (va_nomSite,
                        XZEUC_LIB_COMM,
                        va_NumPAU,
                        va_PO) == XDC_NOK )
   {
      /*A Trace sortie fonction  avec XDC_NOK */   

      XZST_03EcritureTrace(XZSTC_INTERFACE
              ,"OUT: XZEU03_Liberation_Ligne avec XDC_NOK");
 	
      return ( XDC_NOK);
   }
   else
   {
      /*A Trace sortie fonction XZEU03_Liberation_Ligne avec XDC_OK */
   
      XZST_03EcritureTrace(XZSTC_INTERFACE
         ,"OUT: XZEU03_Liberation_Ligne avec XDC_OK");
 	
      return ( XDC_OK ); 
   }  
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande mise en garde d'une ligne
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZEU04_Mise_En_Garde(XDY_NomSite 	va_nomSite,
                         int 		va_NumPAU)

/*
* ARGUMENTS EN ENTREE :
* va_nomSite : nom du site pour construire le datagroup
* va_NumPAU : no pau
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
*   Envoie le message XDM_ECMD_RAU sur le 
*   datagroup XDG_ECMD_RAU_<nomSite>
*
------------------------------------------------------*/
{
static char *version = "$Id: xzeu.c,v 1.14 2020/10/01 12:22:44 gesconf Exp $ : XZEU04_Mise_En_Garde" ;
    
    /*A Trace des parametres en entree */
    
   XZST_03EcritureTrace(XZSTC_INTERFACE,
 	"IN: XZEU04_Mise_En_Garde avec %s %d",
        va_nomSite,
        va_NumPAU);
 
   /*A Envoi du message XDM_ECMD_RAU */
   if (XZEU_Envoyer_message (va_nomSite,
                        XZEUC_GARDE,
                        va_NumPAU,
                        0) == XDC_NOK )
   {
      /*A Trace sortie fonction  avec XDC_NOK */   

      XZST_03EcritureTrace(XZSTC_INTERFACE
              ,"OUT: XZEU04_Mise_En_Garde avec XDC_NOK");
 	
      return ( XDC_NOK);
   }
   else
   {
      /*A Trace sortie fonction XZEU04_Mise_En_Garde avec XDC_OK */
   
      XZST_03EcritureTrace(XZSTC_INTERFACE
         ,"OUT: XZEU04_Mise_En_Garde avec XDC_OK");
 	
      return ( XDC_OK ); 
   }  
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande reprise garde d'une ligne
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZEU05_Reprise_Garde(XDY_NomSite 	va_nomSite,
                         int 		va_NumPAU,
                         int 		va_PO)

/*
* ARGUMENTS EN ENTREE :
* va_nomSite : nom du site pour construire le datagroup
* va_NumPAU : no pau
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
*   Envoie le message XDM_ECMD_RAU sur le 
*   datagroup XDG_ECMD_RAU_<nomSite>
*
------------------------------------------------------*/
{
static char *version = "$Id: xzeu.c,v 1.14 2020/10/01 12:22:44 gesconf Exp $ : XZEU05_Reprise_Garde" ;
    
    /*A Trace des parametres en entree */
    
   XZST_03EcritureTrace(XZSTC_INTERFACE,
 	"IN: XZEU05_Reprise_Garde avec %s %d",
        va_nomSite,
        va_NumPAU);
 
   /*A Controle du no platine */
   if ((va_PO != XZEUC_PLATINE_PO1) && (va_PO != XZEUC_PLATINE_PO2))
   {
      /*A Trace sortie fonction  avec XDC_NOK */   

      XZST_03EcritureTrace(XZSTC_INTERFACE
              ,"OUT: XZEU03_Liberation_Ligne avec XZEUC_ERR_PO");
 	
      return ( XZEUC_ERR_PO );
   }
 	
   /*A Envoi du message XDM_ECMD_RAU */
   if (XZEU_Envoyer_message (va_nomSite,
                        XZEUC_REPRISE,
                        va_NumPAU,
                        va_PO) == XDC_NOK )
   {
      /*A Trace sortie fonction  avec XDC_NOK */   

      XZST_03EcritureTrace(XZSTC_INTERFACE
              ,"OUT: XZEU05_Reprise_Garde avec XDC_NOK");
 	
      return ( XDC_NOK);
   }
   else
   {
      /*A Trace sortie fonction XZEU05_Reprise_Garde avec XDC_OK */
   
      XZST_03EcritureTrace(XZSTC_INTERFACE
         ,"OUT: XZEU05_Reprise_Garde avec XDC_OK");
 	
      return ( XDC_OK ); 
   }  
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande transfert d'une ligne vers la gendarmerie
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZEU06_Transfert_GN(XDY_NomSite 	va_nomSite,
                        int 		va_NumPAU)

/*
* ARGUMENTS EN ENTREE :
* va_nomSite : nom du site pour construire le datagroup
* va_NumPAU : no pau
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
*   Envoie le message XDM_ECMD_RAU sur le 
*   datagroup XDG_ECMD_RAU_<nomSite>
*
------------------------------------------------------*/
{
static char *version = "$Id: xzeu.c,v 1.14 2020/10/01 12:22:44 gesconf Exp $ : XZEU06_Transfert_GN" ;
    
    /*A Trace des parametres en entree */
    
   XZST_03EcritureTrace(XZSTC_INTERFACE,
 	"IN: XZEU06_Transfert_GN avec %s %d",
        va_nomSite,
        va_NumPAU);
 
   /*A Envoi du message XDM_ECMD_RAU */
   if (XZEU_Envoyer_message (va_nomSite,
                        XZEUC_TRANS_GN,
                        va_NumPAU,
                        0) == XDC_NOK )
   {
      /*A Trace sortie fonction  avec XDC_NOK */   

      XZST_03EcritureTrace(XZSTC_INTERFACE
              ,"OUT: XZEU06_Transfert_GN avec XDC_NOK");
 	
      return ( XDC_NOK);
   }
   else
   {
      /*A Trace sortie fonction XZEU06_Transfert_GN avec XDC_OK */
   
      XZST_03EcritureTrace(XZSTC_INTERFACE
         ,"OUT: XZEU06_Transfert_GN avec XDC_OK");
 	
      return ( XDC_OK ); 
   }  
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande prise en main d'une ligne vers la gendarmerie
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZEU07_Demande_Prise_En_Main_GN(XDY_NomSite 	va_nomSite,
                                    int 		va_NumPAU)

/*
* ARGUMENTS EN ENTREE :
* va_nomSite : nom du site pour construire le datagroup
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
*   Envoie le message XDM_ECMD_RAU sur le 
*   datagroup XDG_ECMD_RAU_<nomSite>
*
------------------------------------------------------*/
{
static char *version = "$Id: xzeu.c,v 1.14 2020/10/01 12:22:44 gesconf Exp $ : XZEU07_Demande_Prise_En_Main_GN" ;
    
    /*A Trace des parametres en entree */
    
   XZST_03EcritureTrace(XZSTC_INTERFACE,
 	"IN: XZEU07_Demande_Prise_En_Main_GN avec %s",
        va_nomSite);
 
   /*A Envoi du message XDM_ECMD_RAU */
   if (XZEU_Envoyer_message (va_nomSite,
                        XZEUC_ECOUTE_GN,
                        va_NumPAU,
                        0) == XDC_NOK )
   {
      /*A Trace sortie fonction  avec XDC_NOK */   

      XZST_03EcritureTrace(XZSTC_INTERFACE
              ,"OUT: XZEU07_Demande_Prise_En_Main_GN avec XDC_NOK");
 	
      return ( XDC_NOK);
   }
   else
   {
      /*A Trace sortie fonction XZEU07_Demande_Prise_En_Main_GN avec XDC_OK */
   
      XZST_03EcritureTrace(XZSTC_INTERFACE
         ,"OUT: XZEU07_Demande_Prise_En_Main_GN avec XDC_OK");
 	
      return ( XDC_OK ); 
   }  
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande de test manuel
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZEU08_Test_Unitaire(XDY_NomSite 	va_nomSite,
                         int 		va_NumPAU,
                         int 		va_PO)

/*
* ARGUMENTS EN ENTREE :
* va_nomSite : nom du site pour construire le datagroup
* va_NumPAU : no pau
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
*   Envoie le message XDM_ECMD_RAU sur le 
*   datagroup XDG_ECMD_RAU_<nomSite>
*
------------------------------------------------------*/
{
static char *version = "$Id: xzeu.c,v 1.14 2020/10/01 12:22:44 gesconf Exp $ : XZEU08_Test_Manuel" ;
    
    /*A Trace des parametres en entree */
    
   XZST_03EcritureTrace(XZSTC_INTERFACE,
 	"IN: XZEU08_Test_Manuel avec %s %d",
        va_nomSite,
        va_NumPAU);
 
   /*A Controle du no platine */
   if ((va_PO != XZEUC_PLATINE_PO1) && (va_PO != XZEUC_PLATINE_PO2))
   {
      /*A Trace sortie fonction  avec XDC_NOK */   

      XZST_03EcritureTrace(XZSTC_INTERFACE
              ,"OUT: XZEU08_Test_Manuel avec XZEUC_ERR_PO");
 	
      return ( XZEUC_ERR_PO );
   }
 	
   /*A Envoi du message XDM_ECMD_RAU */
   if (XZEU_Envoyer_message (va_nomSite,
                        XZEUC_TST_UNITAIRE,
                        va_NumPAU,
                        va_PO) == XDC_NOK )
   {
      /*A Trace sortie fonction  avec XDC_NOK */   

      XZST_03EcritureTrace(XZSTC_INTERFACE
              ,"OUT: XZEU08_Test_Manuel avec XDC_NOK");
 	
      return ( XDC_NOK);
   }
   else
   {
      /*A Trace sortie fonction XZEU08_Test_Manuel avec XDC_OK */
   
      XZST_03EcritureTrace(XZSTC_INTERFACE
         ,"OUT: XZEU08_Test_Manuel avec XDC_OK");
 	
      return ( XDC_OK ); 
   }  
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande test semi-automatique
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZEU09_Test_Semi_Auto(XDY_NomSite 	va_nomSite,
			  int		va_antenne,
			  int 		va_PO)

/*
* ARGUMENTS EN ENTREE :
* va_nomSite : nom du site pour construire le datagroup
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
*   Envoie le message XDM_ECMD_RAU sur le 
*   datagroup XDG_ECMD_RAU_<nomSite>
*
------------------------------------------------------*/
{
static char *version = "$Id: xzeu.c,v 1.14 2020/10/01 12:22:44 gesconf Exp $ : XZEU09_Test_Semi_Auto" ;
    
    /*A Trace des parametres en entree */
    
   XZST_03EcritureTrace(XZSTC_INTERFACE,
 	"IN: XZEU09_Test_Semi_Auto avec %s",
        va_nomSite);
 
   /*A Controle du no platine */
   if ((va_PO != XZEUC_PLATINE_PO1) && (va_PO != XZEUC_PLATINE_PO2))
   {
      /*A Trace sortie fonction  avec XDC_NOK */   

      XZST_03EcritureTrace(XZSTC_INTERFACE
              ,"OUT: XZEU09_Test_Semi_Auto avec XZEUC_ERR_PO");
 	
      return ( XZEUC_ERR_PO );
   }
 	
   /*A Envoi du message XDM_ECMD_RAU */
   if (XZEU_Envoyer_message (va_nomSite,
                        XZEUC_TST_SEMI_AUTO,
                        va_antenne,
                        va_PO) == XDC_NOK )
   {
      /*A Trace sortie fonction  avec XDC_NOK */   

      XZST_03EcritureTrace(XZSTC_INTERFACE
              ,"OUT: XZEU09_Test_Semi_Auto avec XDC_NOK");
 	
      return ( XDC_NOK);
   }
   else
   {
      /*A Trace sortie fonction XZEU09_Test_Semi_Auto avec XDC_OK */
   
      XZST_03EcritureTrace(XZSTC_INTERFACE
         ,"OUT: XZEU09_Test_Semi_Auto avec XDC_OK");
 	
      return ( XDC_OK ); 
   }  
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande etat basculement RAU
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZEU10_Demande_Etat_Bascul_RAU(XDY_NomSite 	va_nomSite)

/*
* ARGUMENTS EN ENTREE :
* va_nomSite : nom du site pour construire le datagroup
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
*   Envoie le message XDM_ECMD_RAU sur le 
*   datagroup XDG_ECMD_RAU_<nomSite>
*
------------------------------------------------------*/
{
static char *version = "$Id: xzeu.c,v 1.14 2020/10/01 12:22:44 gesconf Exp $ : XZEU10_Demande_Etat_Bascul_RAU" ;
    
    /*A Trace des parametres en entree */
    
   XZST_03EcritureTrace(XZSTC_INTERFACE,
 	"IN: XZEU10_Demande_Etat_Bascul_RAU avec %s",
        va_nomSite);
 
   /*A Envoi du message XDM_ECMD_RAU */
   if (XZEU_Envoyer_message (va_nomSite,
                        XZEUC_ETAT_BASCUL,
                        0,
                        0) == XDC_NOK )
   {
      /*A Trace sortie fonction  avec XDC_NOK */   

      XZST_03EcritureTrace(XZSTC_INTERFACE
              ,"OUT: XZEU10_Demande_Etat_Bascul_RAU avec XDC_NOK");
 	
      return ( XDC_NOK);
   }
   else
   {
      /*A Trace sortie fonction XZEU10_Demande_Etat_Bascul_RAU avec XDC_OK */
   
      XZST_03EcritureTrace(XZSTC_INTERFACE
         ,"OUT: XZEU10_Demande_Etat_Bascul_RAU avec XDC_OK");
 	
      return ( XDC_OK ); 
   }  
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande mise en communication pau-platine
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZEU11_Bascul_RAU(XDY_Octet 	va_site,
                                  char *va_zone,
                                  char *va_etat)

/*
* ARGUMENTS EN ENTREE :
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
*
------------------------------------------------------*/
{
static char *version = "$Id: xzeu.c,v 1.14 2020/10/01 12:22:44 gesconf Exp $ : XZEU11_Bascul_RAU" ;
XDY_Datagroup	pl_NomDG;

    /*A Trace des parametres en entree */
    
   XZST_03EcritureTrace(XZSTC_INTERFACE,
 	"IN: XZEU11_Bascul_RAU avec %d %s %s",
        va_site,
        va_zone,
        va_etat);
 
   switch(va_site) {
     case (XDC_CI) : XDG_EncodeDG2(pl_NomDG, XDG_ECMD_RAU, XDC_NOM_SITE_CI);
				break;
     case (XDC_VC) : XDG_EncodeDG2(pl_NomDG, XDG_ECMD_RAU, XDC_NOM_SITE_VC);
				break;
     case (XDC_DP) : XDG_EncodeDG2(pl_NomDG, XDG_ECMD_RAU, XDC_NOM_SITE_DP);
				break;
     case (XDC_CA) : XDG_EncodeDG2(pl_NomDG, XDG_ECMD_RAU, XDC_NOM_SITE_CA);
				break;
   }

   /*A Envoi du message XDM_ECMD_RAU en fournissant les valeur recues  */
   if (!TipcSrvMsgWrite (  pl_NomDG
      			  ,XDM_IdentMsg(XDM_ECMD_BASC_RAU)
  			  ,XDM_FLG_SRVMSGWRITE
			  ,T_IPC_FT_STR,va_zone
			  ,T_IPC_FT_STR,va_etat
 			  ,NULL))
      	{
  		/*A Trace envoi message XDM_ECMD_RAU non effectue */    	
      	
         	XZST_03EcritureTrace(XZSTC_INFO
 		   ,"Envoi message XDM_ECMD_RAU non effectue \n");
    
         	return(	XDC_NOK );		
	}
        else
        {
                /*A Envoi du message */
   
                TipcSrvFlush();

                /*A Trace envoi message XDM_ECMD_RAU effectue avec succes */    	
      	
                XZST_03EcritureTrace(XZSTC_INFO
                   ,"Envoi message XDM_ECMD_RAU effectue avec succes sur %s\n",pl_NomDG);
                   
         	return(	XDC_OK );		
        }
 	
}
