/*E*/
/*Fichier : @(#)xzea37.c	1.1      Release : 1.1        Date : 04/07/97
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE xzea  * FICHIER xzea37.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module d'interface pour l'abonnement aux mesures 1heu Temps de parcours >
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	19 Fev 1997	: Creation pour TDP (DEM/1396)
* JMG		1.2		10/05/20 : EMS
* LCL		20/12/21	: MOVIS P2 ems SAE-312
------------------------------------------------------*/

/* fichiers inclus */

#include "xzea.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "@(#)xzea37.c	1.1 04/07/97 : xzea37" ;
XDY_Fonction		pg_FonctUtil_XZEA_37 = NULL;
T_CB			vg_CB_XZEA37         = NULL;

/* declaration de fonctions internes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	XZEA_Abt_TdP_1heu ( 
        T_IPC_CONN 			va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG                        pa_ARG )
 
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
*  XDM_EMES_TDP_HORAIRE 
*
* FONCTION 
*
*
------------------------------------------------------*/
{
   static char *version = "@(#)xzea37.c	1.1 04/07/97 : XZEA_Abt_TdP_1heu ";
   XDY_EMes_TdP_1h		vl_MesTDP1heu;
#ifndef _TIBCO_EMS
   T_REAL8			vl_Horodate;
   T_INT2			*pl_Numero;
   T_INT2			*pl_TDP1heu;
   T_INT2			*pl_Validite1heu;
   T_INT2			*pl_NbApp;
   T_INT2			vl_NumUGTP;
   T_INT2			vl_CleAcquittement;
   T_STR			pl_NomMachine;
   T_INT4			vl_SizeNum = 0;
   T_INT4			vl_SizeTemps = 0;
   T_INT4			vl_SizeValide = 0;
   T_INT4			vl_SizeApp = 0;
#else
   XDY_Horodate			vl_Horodate;
   XDY_Mot			pl_Numero[500];
   XDY_Mot			pl_TDP1heu[500];
   XDY_Mot			pl_Validite1heu[500];
   XDY_Mot			pl_NbApp[500];
   XDY_Mot			vl_NumUGTP;
   XDY_Mot			vl_CleAcquittement;
   XDY_NomMachine		pl_NomMachine;
   XDY_Entier			vl_SizeNum = 0;
   XDY_Entier			vl_SizeTemps = 0;
   XDY_Entier			vl_SizeValide = 0;
   XDY_Entier			vl_SizeApp = 0;
#endif

   XDY_Datagroup		pl_NomDG;
   int				vl_Resultat = XDC_OK,
   				vl_Index = 0;
   
		  
   /* Recuperation des parametres du message XDM_EMES_TDP_HORAIRE */
   if( !TipcMsgRead( pa_Data -> msg, 
               	     	T_IPC_FT_REAL8,		&vl_Horodate,
		     	T_IPC_FT_INT2, 		&vl_NumUGTP,
		     	T_IPC_FT_INT2, 		&vl_CleAcquittement,
		     	T_IPC_FT_STR, 		&pl_NomMachine,
               		T_IPC_FT_INT2_ARRAY, 	&pl_Numero, 	 &vl_SizeNum,
               		T_IPC_FT_INT2_ARRAY, 	&pl_TDP1heu, 	 &vl_SizeTemps,
               		T_IPC_FT_INT2_ARRAY, 	&pl_NbApp, 	 &vl_SizeApp,
               		T_IPC_FT_INT2_ARRAY, 	&pl_Validite1heu, &vl_SizeValide,
                     	NULL) )   
   {
      XZST_03EcritureTrace ( XZSTC_WARNING," Impossible de lire le MSG XDM_EMES_TDP_HORAIRE dans XZEA_Abt_TdP_1heu ");
   }
   else
   { 
      XZST_03EcritureTrace ( XZSTC_FONCTION, "IN  : %s.", version );

      for ( vl_Index = 0; vl_Index < vl_SizeNum; vl_Index++ )
      {
         vl_MesTDP1heu.Horodate =   vl_Horodate;     
         vl_MesTDP1heu.Numero   =  (XDY_Eqt) 	 (pl_Numero[vl_Index]);      
         vl_MesTDP1heu.Dispo    =  (XDY_Booleen) (pl_Validite1heu[vl_Index]);        
         vl_MesTDP1heu.Temps    =  (XDY_TDP) 	 (pl_TDP1heu[vl_Index]);       
         vl_MesTDP1heu.NbApp    =  (XDY_TDP) 	 (pl_NbApp[vl_Index]);       
         XZST_03EcritureTrace ( XZSTC_INTERFACE,"Appel de la fonction de mise en base des donn�es TDP 1heu avec:\n\tNumero\t: %d\n\tValidit�\t: %d\
            				\n\tTemps\t: %d\n\tNbApp\t: %d\n\tHorodate\t: %lf",
            				vl_MesTDP1heu.Numero,
            				vl_MesTDP1heu.Dispo,
            				vl_MesTDP1heu.Temps,
            				vl_MesTDP1heu.NbApp,
            				vl_MesTDP1heu.Horodate );
         /*A Appel de la fonction utilsateur avec la structure XZEAT_EMES_METEO en argument */
         pg_FonctUtil_XZEA_37 ( vl_MesTDP1heu, &vl_Resultat );
      }

      if ( vl_Resultat == XDC_OK )
      {
         /*A Construction du DatGroup XDG_ECMD_TDP */
         sprintf ( pl_NomDG, "%s_%s", XDG_ECMD_TDP, pl_NomMachine ) ;
   	
         /*A Envoi du message XDM_ACQ_DONNEES_TDP en fournissant les valeur recues  */
         if (!TipcSrvMsgWrite (  pl_NomDG
      			  	,XDM_IdentMsg(XDM_ACQ_DONNEES_TDP)
  			  	,XDM_FLG_SRVMSGWRITE
 			  	,T_IPC_FT_INT2, vl_NumUGTP 
 			  	,T_IPC_FT_INT2, vl_CleAcquittement 
                          	,NULL))
         {
  		/*A Trace envoi message XDM_ACQ_DONNEES_TDP  non effectue */    	
         	XZST_03EcritureTrace(XZSTC_WARNING ,"%s : Envoi message XDM_ACQ_DONNEES_TDP non effectue", version);
         }
      }
      else
      {
         XZST_03EcritureTrace ( XZSTC_WARNING,"Echec mise en base des donn�es TDP 1heu avec:\n\tNumero\t: %d\n\tValidit�\t: %d\
            				\n\tTemps\t: %d\n\tNbApp\t: %d\n\tHorodate\t: %lf",
            				vl_MesTDP1heu.Numero,
            				vl_MesTDP1heu.Dispo,
            				vl_MesTDP1heu.Temps,
            				vl_MesTDP1heu.NbApp,
            				vl_MesTDP1heu.Horodate );
      }
      
      /*A Envoi du message */
      TipcSrvFlush();
      XZST_03EcritureTrace ( XZSTC_FONCTION,  "OUT : %d %s", vl_Resultat, version );
   }
}


/* definition de fonctions externes */



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Abt aux mesures des station M�t�o
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	XZEA37_Abt_TdP_1heu (	XDY_Fonction	pa_FonctUtil,
				int *		pa_Resultat )

/*
* ARGUMENTS EN ENTREE :
*
* XDY_Fonction	pa_FonctUtil	: Nom de la fonction utilisateur
*
* ARGUMENTS EN SORTIE :
*
* int		pa_resultat		: Code retour resultat
*
* CODE RETOUR : 
*   XDC_OK		Succes
*
* CONDITION D'UTILISATION
*   Ce service est demande pour s'abonner aux mesures TdP horaire
*
* FONCTION 
*   Armement CallBack et joindre le Datagroups
*
------------------------------------------------------*/
{
static char *version = "@(#)xzea37.c	1.1 04/07/97 : XZEA37_Abt_TdP_1heu ";
int 		vl_ValRet = XDC_OK;
T_BOOL		vl_EtatDg = FALSE;

        XZST_03EcritureTrace ( XZSTC_INTERFACE, "IN  : %s", version );
     		
        /* Memorisation en globale du nom de la fonction utilisateur */		
        pg_FonctUtil_XZEA_37 = pa_FonctUtil;
    
        /*A  Armement Callback sur Reception XDM_EMES_TDP_HORAIRE */
        if ( vg_CB_XZEA37 == NULL )
        {
	   if ( ( vg_CB_XZEA37 = TipcSrvProcessCbCreate (	XDM_IdentMsg(XDM_EMES_TDP_HORAIRE),
							      	XZEA_Abt_TdP_1heu,
								(T_CB_ARG)NULL ) ) == NULL ) 
	   {	
	       XZST_03EcritureTrace ( XZSTC_WARNING, "Echec armement Callback sur XDM_EMES_TDP_HORAIRE." );
	       vl_ValRet = XDC_NOK;
	   }
	   else
	   {	
	       XZST_03EcritureTrace ( XZSTC_DEBUG1, "Armement Callback sur XDM_EMES_TDP_HORAIRE." );
	   }
        }

	/*A Abonnement au Datagroup XDG_EMES_UGTP_HORAIRE */
   	/*A Joindre ce datagroup s'il n'est pas deja joint */
   	TipcSrvSubjectGetSubscribe ( XDG_EMES_UGTP_HORAIRE, &vl_EtatDg );
   	if ( !vl_EtatDg )
   	{
   	   if ( !XDG_JoindreDG( XDG_EMES_UGTP_HORAIRE ) )
   	   {
      	      XZST_03EcritureTrace ( XZSTC_WARNING, "Echec abonnement au Datagroup %s ", XDG_EMES_UGTP_HORAIRE );
	      vl_ValRet = XDC_NOK;
	   }
	   else
	   {
      	      XZST_03EcritureTrace ( XZSTC_DEBUG1, "Abonnement au Datagroup %s ", XDG_EMES_UGTP_HORAIRE );
	   }
	}
  	
        XZST_03EcritureTrace ( XZSTC_INTERFACE, "OUT : %d Retour:%s.", vl_ValRet, version );
        *pa_Resultat = vl_ValRet;
        return ( vl_ValRet );				
				
}


