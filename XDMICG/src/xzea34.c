/*E*/
/*Fichier : @(#)xzea34.c	1.1      Release : 1.1        Date : 12/05/96
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE xzea  * FICHIER xzea34.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module d'interface pour l'abonnement aux mesures METEO >
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	21 Nov 1996	: Creation
* LCL		version 1.2	26/06/2020	: correction LINUX64 MOVIS DEM-SAE93
* LCL    version 1.3 16/06/2022  : correction fonction XZEA_EMes_METEO taille string pour lecture messages donnees meteo DEM-311
------------------------------------------------------*/

/* fichiers inclus */

#include "xzea.h"
#include "xzsm.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "@(#)xzea34.c	1.1 12/05/96 : xzea34" ;
XDY_Fonction		pg_FonctUtil_XZEA_34 = NULL;
T_CB			vg_CB_XZEA34         = NULL;

/* declaration de fonctions internes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	XZEA_EMes_METEO ( 
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
*  XDM_DONNEES_METEO_QTX 
*
* FONCTION 
*
*
------------------------------------------------------*/
{
   static char *version = "@(#)xzea34.c	1.1 12/05/96 : XZEA_EMes_METEO ";
   XDY_EMes_Meteo		vl_MesMeteo;
#ifndef _TIBCO_EMS
   T_STR			pl_MesuresMeteo;
   T_STR			pl_CleAcquittement;
   T_STR			pl_NomMachine;
#else
   char			   pl_MesuresMeteo[255];
   XDY_Phrase		pl_CleAcquittement;
   XDY_NomMachine	pl_NomMachine;
#endif
 
   XZSMT_Horodate 		vl_date_debut;
   XZSMT_Horodate 		vl_heure_debut;
   XZSMT_Horodate 		vl_date_fin;
   XZSMT_Horodate 		vl_heure_fin;
   int 				vl_type_pluie;
   int 				vl_validite;
   int 				vl_num_equipement;
   
   XZSMT_Horodate		vl_Horodate = "";
   XDY_Datagroup		pl_NomDG;
   int				vl_Resultat = XDC_OK,
   				vl_Index = 0;
   
		  
   /* Recuperation des parametres du message XDM_DONNEES_METEO_QTX */
   if( !TipcMsgRead( pa_Data -> msg, 
		     T_IPC_FT_STR, &pl_CleAcquittement,
		     T_IPC_FT_STR, &pl_NomMachine,
                     T_IPC_FT_STR, &pl_MesuresMeteo,
                     NULL) )   
   {
      XZST_03EcritureTrace ( XZSTC_WARNING," Impossible de lire le MSG XDM_DONNEES_METEO_QTX dans XZEA_EMes_METEO ");
   }
   else
   { 
      XZST_03EcritureTrace ( XZSTC_FONCTION, "IN  : %s mes:<%s>.", version, pl_MesuresMeteo );

      /*A Renseignement de la structure XDY_EMes_Meteo */
      if ( sscanf ( pl_MesuresMeteo, XZEAC_FORMAT_FICHIER_HISTORIQUE_DONNEES, 
      				&vl_num_equipement,
      				&vl_validite,
      				vl_date_debut, 
      				vl_heure_debut,
				vl_date_fin, 
				vl_heure_fin, 
				&vl_type_pluie ) == 7 )
      {	       
            vl_MesMeteo.Numero =  vl_num_equipement;       
            vl_MesMeteo.Dispo  =  vl_validite;        
            vl_MesMeteo.Code   =  vl_type_pluie;        
            sprintf ( vl_Horodate, "%s 00:00:01", vl_date_debut ); 
            XZSM_13ConversionHorodateSec ( vl_Horodate, &vl_MesMeteo.Jour );
            sprintf ( vl_Horodate, "%s %s", vl_date_debut, vl_heure_debut ); 
            XZSM_13ConversionHorodateSec ( vl_Horodate, &vl_MesMeteo.HeureDebut );
            sprintf ( vl_Horodate, "%s %s", vl_date_fin, vl_heure_fin ); 
            XZSM_13ConversionHorodateSec ( vl_Horodate, &vl_MesMeteo.HeureFin );
            XZST_03EcritureTrace ( XZSTC_DEBUG1,"Appel de la fonction de mise en base des donn�es M�t�o avec:\n\tNumero\t: %d\n\tValidit�\t: %d\
            				\n\tCode\t: %d\n\tJour\t: %lf\n\tD�but\t: %lf\n\tFin\t: %lf",
            				vl_MesMeteo.Numero,
            				vl_MesMeteo.Dispo,
            				vl_MesMeteo.Code,
            				vl_MesMeteo.Jour,
            				vl_MesMeteo.HeureDebut,
            				vl_MesMeteo.HeureFin );
            /*A Appel de la fonction utilsateur avec la structure XZEAT_EMES_METEO en argument */
            pg_FonctUtil_XZEA_34 ( vl_MesMeteo, &vl_Resultat );
      }
      else
      {
         XZST_03EcritureTrace ( XZSTC_WARNING, " %s impossible de lire les infos m�t�o du message : <%s>.", version, pl_MesuresMeteo );
      }
 	
      if ( vl_Resultat == XDC_OK )
      {
         /*A Construction du DatGroup XDG_ECMD_QTX */
         sprintf ( pl_NomDG, "%s_%s", XDG_ECMD_QTX, pl_NomMachine ) ;
   	
         /*A Envoi du message XDM_ACQ_DONNEES_METEO_QTX en fournissant les valeur recues  */
         if (!TipcSrvMsgWrite (  pl_NomDG
      			  	,XDM_IdentMsg(XDM_ACQ_DONNEES_METEO_QTX)
  			  	,XDM_FLG_SRVMSGWRITE
 			  	,T_IPC_FT_STR, pl_CleAcquittement 
                          	,NULL))
         {
  		/*A Trace envoi message XDM_ACQ_DONNEES_METEO_QTX  non effectue */    	
         	XZST_03EcritureTrace(XZSTC_WARNING ,"%s : Envoi message XDM_Acknoledge non effectue", version);
         }
      }
      else
      {
          XZST_03EcritureTrace(XZSTC_DEBUG1 ,"Mise en base des mesures m�t�o impossible pour :\n\tNumero\t: %d\n\tValidit�\t: %d\
            				\n\tCode\t: %d\n\tJour\t: %lf\n\tD�but\t: %lf\n\tFin\t: %lf",
            				vl_MesMeteo.Numero,
            				vl_MesMeteo.Dispo,
            				vl_MesMeteo.Code,
            				vl_MesMeteo.Jour,
            				vl_MesMeteo.HeureDebut,
            				vl_MesMeteo.HeureFin );
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

int	XZEA34_Abt_Mesures_Meteo (	XDY_Fonction	pa_FonctUtil,
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
*   Ce service est demande pour s'abonner aux mesures Meteo
*
* FONCTION 
*   Armement CallBack et joindre le Datagroups
*
------------------------------------------------------*/
{
static char *version = "@(#)xzea34.c	1.1 12/05/96 : XZEA34_Abt_Mesures_Meteo ";
int 		vl_ValRet = XDC_OK;
T_BOOL		vl_EtatDg = FALSE;
XDY_Datagroup	pl_NomDG;

        XZST_03EcritureTrace ( XZSTC_INTERFACE, "IN  : %s", version );
     		
        /* Memorisation en globale du nom de la fonction utilisateur */		
        pg_FonctUtil_XZEA_34 = pa_FonctUtil;
    
        /*A  Armement Callback sur Reception XDM_DONNEES_METEO_QTX */
        if ( vg_CB_XZEA34 == NULL )
        {
	   if ( ( vg_CB_XZEA34 = TipcSrvProcessCbCreate (	XDM_IdentMsg(XDM_DONNEES_METEO_QTX),
							      	XZEA_EMes_METEO,
								(T_CB_ARG)NULL ) ) == NULL ) 
	   {	
	       XZST_03EcritureTrace ( XZSTC_WARNING, "Echec armement Callback sur XDM_DONNEES_METEO_QTX." );
	       vl_ValRet = XDC_NOK;
	   }
	   else
	   {	
	       XZST_03EcritureTrace ( XZSTC_DEBUG1, "Armement Callback sur XDM_DONNEES_METEO_QTX." );
	   }
        }

	/*A Abonnement au Datagroup XDG_DONNEES_METEO_QTX */
   	/*A Joindre ce datagroup s'il n'est pas deja joint */
   	TipcSrvSubjectGetSubscribe ( XDG_DONNEES_METEO_QTX, &vl_EtatDg );
   	if ( !vl_EtatDg )
   	{
   	   if ( !XDG_JoindreDG( XDG_DONNEES_METEO_QTX ) )
   	   {
      	      XZST_03EcritureTrace ( XZSTC_WARNING, "Echec abonnement au Datagroup %s ", XDG_DONNEES_METEO_QTX );
	      vl_ValRet = XDC_NOK;
	   }
	   else
	   {
      	      XZST_03EcritureTrace ( XZSTC_DEBUG1, "Abonnement au Datagroup %s ", XDG_DONNEES_METEO_QTX );
	   }
	}
  	
        XZST_03EcritureTrace ( XZSTC_INTERFACE, "OUT : %d Retour:%s.", vl_ValRet, version );
        *pa_Resultat = vl_ValRet;
        return ( vl_ValRet );				
				
}


