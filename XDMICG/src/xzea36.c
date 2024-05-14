/*E*/
/*Fichier : @(#)xzea36.c	1.1      Release : 1.1        Date : 04/07/97
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE xzea  * FICHIER xzea36.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module d'interface pour l'abonnement aux mesures 6mn Temps de parcours >
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	19 Fev 1997	: Creation pour TDP (DEM/1396)
* JMG		version 1.2	13/01/12	: tdp etendu DEM/1014
* LCL		version 1.3	26/06/2020	: correcttion LINUX64 MOVIS DEM-SAE93
* LCL		20/12/21	: MOVIS P2 ems SAE-312
------------------------------------------------------*/

/* fichiers inclus */

#include "xzea.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "@(#)xzea36.c	1.1 04/07/97 : xzea36" ;
XDY_Fonction		pg_FonctUtil_XZEA_36 = NULL;
T_CB			vg_CB_XZEA36         = NULL;

/* declaration de fonctions internes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	XZEA_TdP_6mn ( 
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
*  XDM_EMES_TDP_6MN 
*
* FONCTION 
*
*
------------------------------------------------------*/
{
   static char *version = "@(#)xzea36.c	1.1 04/07/97 : XZEA_TdP_6mn ";
   XDY_EMes_TdP_6mn		vl_MesTDP6mn;
   XDY_EMes_TdP_6mn		vl_MesTDP6mnRADT;
#ifdef _TIBCO_EMS
   XDY_NomMachine	pl_NomMachine;
   XDY_Horodate		vl_Horodate;
   XDY_Mot		pl_Numero[255];
   XDY_Mot		pl_TDP6mn[255];
   XDY_Mot		pl_TDP6mnRADT[255];
   XDY_Mot		pl_Validite6mn[255];
   XDY_Mot		pl_Validite6mnRADT[255];
   XDY_Mot		vl_NumUGTP;
   XDY_Mot		vl_CleAcquittement;
   XDY_Entier		vl_SizeNum = 0;
   XDY_Entier		vl_SizeTemps = 0;
   XDY_Entier		vl_SizeTempsRADT = 0;
   XDY_Entier		vl_SizeValide = 0;
   XDY_Entier		vl_SizeValideRADT = 0;
#else
   T_STR			pl_NomMachine;
   T_REAL8			vl_Horodate;
   T_INT2			*pl_Numero;
   T_INT2			*pl_TDP6mn;
   T_INT2			*pl_TDP6mnRADT;
   T_INT2			*pl_Validite6mn;
   T_INT2			*pl_Validite6mnRADT;
   T_INT2			vl_NumUGTP;
   T_INT2			vl_CleAcquittement;
   T_INT4			vl_SizeNum = 0;
   T_INT4			vl_SizeTemps = 0;
   T_INT4			vl_SizeTempsRADT = 0;
   T_INT4			vl_SizeValide = 0;
   T_INT4			vl_SizeValideRADT = 0;
#endif
   
   XDY_Datagroup		pl_NomDG;
   int				vl_Resultat = XDC_OK,
   				vl_Index = 0;
  

   /* Recuperation des parametres du message XDM_EMES_TDP_6MN */
   if( !TipcMsgRead( pa_Data -> msg, 
               	     	T_IPC_FT_REAL8,		&vl_Horodate,
		     	T_IPC_FT_INT2, 		&vl_NumUGTP,
		     	T_IPC_FT_INT2, 		&vl_CleAcquittement,
		     	T_IPC_FT_STR, 		&pl_NomMachine,
               		T_IPC_FT_INT2_ARRAY, 	&pl_Numero, 	 &vl_SizeNum,
               		T_IPC_FT_INT2_ARRAY, 	&pl_TDP6mn, 	 &vl_SizeTemps,
               		T_IPC_FT_INT2_ARRAY, 	&pl_Validite6mn, &vl_SizeValide,
               		T_IPC_FT_INT2_ARRAY, 	&pl_TDP6mnRADT, 	 &vl_SizeTempsRADT,
               		T_IPC_FT_INT2_ARRAY, 	&pl_Validite6mnRADT, &vl_SizeValideRADT,
                     	NULL) )   
   {
      XZST_03EcritureTrace ( XZSTC_WARNING," Impossible de lire le MSG XDM_EMES_TDP_6MN dans XZEA_TdP_6mn ");
   }
   else
   { 
      XZST_03EcritureTrace ( XZSTC_FONCTION, "IN  : %s.", version );

      for ( vl_Index = 0; vl_Index < vl_SizeNum; vl_Index++ )
      {
         vl_MesTDP6mn.Horodate =   vl_Horodate;     
         vl_MesTDP6mn.Numero   =  (XDY_Eqt) 	(pl_Numero[vl_Index]);      
         vl_MesTDP6mn.Dispo    =  (XDY_Booleen) (pl_Validite6mn[vl_Index]);        
         vl_MesTDP6mn.Temps    =  (XDY_TDP) 	(pl_TDP6mn[vl_Index]);       
	 vl_MesTDP6mnRADT.Horodate =   vl_Horodate;
	 vl_MesTDP6mnRADT.Numero	= (XDY_Eqt)     (pl_Numero[vl_Index]);
	vl_MesTDP6mnRADT.Dispo = (XDY_Booleen) (pl_Validite6mnRADT[vl_Index]);
	vl_MesTDP6mnRADT.Temps = (XDY_TDP)     (pl_TDP6mnRADT[vl_Index]);


          XZST_03EcritureTrace ( XZSTC_INTERFACE,"Appel de la fonction de mise en base des donn�es TDP 6mn avec:\n\tNumero\t: %d\n\tValidit�\t: %d\
            				\n\tTemps\t: %d\n\tHorodate\t: %lf",
            				vl_MesTDP6mn.Numero,
            				vl_MesTDP6mn.Dispo,
            				vl_MesTDP6mn.Temps,
            				vl_MesTDP6mn.Horodate );
         XZST_03EcritureTrace ( XZSTC_INTERFACE,"Appel de la fonction de mise en base des donn�es TDP 6mn RADT avec:\n\tNumero\t: %d\n\tValidit�\t: %d\
            				\n\tTemps\t: %d\n\tHorodate\t: %lf",
            				vl_MesTDP6mnRADT .Numero,
            				vl_MesTDP6mnRADT .Dispo,
            				vl_MesTDP6mnRADT .Temps,
            				vl_MesTDP6mnRADT .Horodate );
         /*A Appel de la fonction utilsateur avec la structure XZEAT_EMES_METEO en argument */
         pg_FonctUtil_XZEA_36 ( vl_MesTDP6mn, vl_MesTDP6mnRADT, &vl_Resultat );
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
      XZST_03EcritureTrace ( XZSTC_WARNING, "Message XDM_ACQ_DONNEES_TDP envoye");

      }
      else
      {
         XZST_03EcritureTrace ( XZSTC_WARNING,"Echec mise en base des donn�es TDP 6mn avec:\n\tNumero\t: %d\n\tValidit�\t: %d\
            				\n\tTemps\t: %d\n\tHorodate\t: %lf",
            				vl_MesTDP6mn.Numero,
            				vl_MesTDP6mn.Dispo,
            				vl_MesTDP6mn.Temps,
            				vl_MesTDP6mn.Horodate );
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

int	XZEA36_Abt_TdP_6mn (	XDY_Fonction	pa_FonctUtil,
				XDY_Nom		pa_NomSite,
				int *		pa_Resultat )

/*
* ARGUMENTS EN ENTREE :
*
* XDY_Fonction	pa_FonctUtil	: Nom de la fonction utilisateur
* XDY_Nom	pa_NomSite	: Nom du site demande
*
* ARGUMENTS EN SORTIE :
*
* int		pa_resultat		: Code retour resultat
*
* CODE RETOUR : 
*   XDC_OK		Succes
*
* CONDITION D'UTILISATION
*   Ce service est demande pour s'abonner aux mesures TdP 6 minutes
*
* FONCTION 
*   Armement CallBack et joindre le Datagroups
*
------------------------------------------------------*/
{
static char *version = "@(#)xzea36.c	1.1 04/07/97 : XZEA36_Abt_TdP_6mn ";
int 		vl_ValRet = XDC_OK;
T_BOOL		vl_EtatDg = FALSE;
XDY_Datagroup	pl_NomDG;

        XZST_03EcritureTrace ( XZSTC_INTERFACE, "IN  : %s", version );
     		
        /* Memorisation en globale du nom de la fonction utilisateur */		
        pg_FonctUtil_XZEA_36 = pa_FonctUtil;
    
        /*A  Armement Callback sur Reception XDM_EMES_TDP_6MN */
        if ( vg_CB_XZEA36 == NULL )
        {
	   if ( ( vg_CB_XZEA36 = TipcSrvProcessCbCreate (	XDM_IdentMsg(XDM_EMES_TDP_6MN),
							      	XZEA_TdP_6mn,
								(T_CB_ARG)NULL ) ) == NULL ) 
	   {	
	       XZST_03EcritureTrace ( XZSTC_WARNING, "Echec armement Callback sur XDM_EMES_TDP_6MN." );
	       vl_ValRet = XDC_NOK;
	   }
	   else
	   {	
	       XZST_03EcritureTrace ( XZSTC_DEBUG1, "Armement Callback sur XDM_EMES_TDP_6MN." );
	   }
        }

	/*A Abonnement au Datagroup XDG_EMES_UGTP_6MN */
	/*A Construction du nom du DataGroup */
	sprintf ( pl_NomDG, "%s_%s", XDG_EMES_UGTP_6MN, pa_NomSite );
   	/*A Joindre ce datagroup s'il n'est pas deja joint */
   	TipcSrvSubjectGetSubscribe ( pl_NomDG, &vl_EtatDg );
   	if ( !vl_EtatDg )
   	{
   	   if ( !XDG_JoindreDG( pl_NomDG ) )
   	   {
      	      XZST_03EcritureTrace ( XZSTC_WARNING, "Echec abonnement au Datagroup %s ", XDG_EMES_UGTP_6MN );
	      vl_ValRet = XDC_NOK;
	   }
	   else
	   {
      	      XZST_03EcritureTrace ( XZSTC_DEBUG1, "Abonnement au Datagroup %s ", XDG_EMES_UGTP_6MN );
	   }
	}
  	
        XZST_03EcritureTrace ( XZSTC_INTERFACE, "OUT : %d Retour:%s.", vl_ValRet, version );
        *pa_Resultat = vl_ValRet;
        return ( vl_ValRet );				
				
}


