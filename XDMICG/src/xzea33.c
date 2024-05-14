/*E*/
/*Fichier : @(#)xzea33.c	1.1      Release : 1.1        Date : 10/09/96
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE xzea  * FICHIER xzea33.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module d'interface pour l'abonnement aux mesures de pesage par classes >
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	01 Oct 1996	: Creation
* JMG		1.2	10/05/20 : EMS
* LCL		20/12/21	: MOVIS P2 ems SAE-312
------------------------------------------------------*/

/* fichiers inclus */

#include "xzea.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "@(#)xzea33.c	1.1 10/09/96 : xzea33" ;
XDY_Fonction		pg_FonctUtil_XZEA_33[3] = { NULL, NULL, NULL };
T_CB			vg_CB_XZEA33         = NULL;

/* declaration de fonctions internes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	XZEA_EMes_Pesage ( 
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
*  XDM_EMES_PESAGE 
*
* FONCTION 
*
*
------------------------------------------------------*/
{
   static char *version = "@(#)xzea33.c	1.1 10/09/96 : XZEA_EMes_Pesage ";
   XZEAT_EMES_RADT_SILHOUETTE	vl_MesSilhouette;
   XZEAT_EMES_RADT_ESSIEU	vl_MesEssieu;
   XZEAT_EMES_RADT_POIDS	vl_MesPoids;
#ifndef _TIBCO_EMS
   T_INT2			vl_Numero, vl_Voie, vl_Sens, vl_TypePesage;
   T_REAL8			vl_Horodate;
   T_INT4			*pl_Mesure;
   T_INT4			vl_Size = 0;
#else
   XDY_Mot			vl_Numero, vl_Voie, vl_Sens, vl_TypePesage;
   XDY_Horodate			vl_Horodate;
   XDY_Entier			pl_Mesure[20];
   XDY_Entier			vl_Size = 0;
#endif
   XDY_Datagroup		pl_NomDG;
   int				vl_Resultat = XDC_OK,
   				vl_Index = 0;
   
		  
   /* Recuperation des parametres du message XDM_EMES_PESAGE */
   if ( !TipcMsgRead(pa_Data -> msg,
               T_IPC_FT_REAL8,       &vl_Horodate,
               T_IPC_FT_INT2,        &vl_Numero,
               T_IPC_FT_INT2,        &vl_Sens,
               T_IPC_FT_INT2,        &vl_Voie,
               T_IPC_FT_INT2,        &vl_TypePesage,
               T_IPC_FT_INT4_ARRAY,  &pl_Mesure, &vl_Size,
	       NULL) )
   {
      XZST_03EcritureTrace ( XZSTC_WARNING," Impossible de lire le MSG XDM_EMES_PESAGE dans XZEA_EMes_Pesage ");
   }
   else
   { 
      XZST_03EcritureTrace ( XZSTC_FONCTION, "IN  : %s  Horodate:%lf Station:%d Voie:%d Sens:%d Pesage:%d.",
     				version, vl_Horodate, vl_Numero, vl_Voie, vl_Sens, vl_TypePesage );

      switch ( vl_TypePesage )
      {
         case XDC_MESURE_SILHOUETTE :
            /*A Renseignement de la structure XZEAT_EMES_RADT_SILHOUETTE */
            vl_MesSilhouette.Horodate	= vl_Horodate;
            vl_MesSilhouette.Numero	= vl_Numero;
            vl_MesSilhouette.Voie	= vl_Voie;
            vl_MesSilhouette.Sens	= vl_Sens;
            for ( vl_Index = 0;
                  (vl_Index < vl_Size) && (vl_Index < XDC_NB_CLASSE_SILHOUETTE);
                  vl_MesSilhouette.Mesures[vl_Index] = (XDY_Mot) (pl_Mesure[vl_Index]), vl_Index++ );
	       
            /*A Appel de la fonction utilsateur avec la structure XZEAT_EMES_RADT_SILHOUETTE en argument */
            pg_FonctUtil_XZEA_33[0] ( vl_MesSilhouette, &vl_Resultat );
            break;
    
         case XDC_MESURE_ESSIEU :
            /*A Renseignement de la structure XZEAT_EMES_RADT_ESSIEU */
            vl_MesEssieu.Horodate	= vl_Horodate;
            vl_MesEssieu.Numero		= vl_Numero;
            vl_MesEssieu.Voie		= vl_Voie;
            vl_MesEssieu.Sens		= vl_Sens;
            for ( vl_Index = 0;
                  (vl_Index < vl_Size) && (vl_Index < XDC_NB_CLASSE_ESSIEU);
                  vl_MesEssieu.Mesures[vl_Index] = (XDY_Mot) (pl_Mesure[vl_Index]), vl_Index++ );
	       
            /*A Appel de la fonction utilsateur avec la structure XZEAT_EMES_RADT_ESSIEU en argument */
            pg_FonctUtil_XZEA_33[1] ( vl_MesEssieu, &vl_Resultat );
            break;
    
         case XDC_MESURE_POIDS :
            /*A Renseignement de la structure XZEAT_EMES_RADT_POIDS */
            vl_MesPoids.Horodate	= vl_Horodate;
            vl_MesPoids.Numero		= vl_Numero;
            vl_MesPoids.Voie		= vl_Voie;
            vl_MesPoids.Sens		= vl_Sens;
            for ( vl_Index = 0;
                  (vl_Index < vl_Size) && (vl_Index < XDC_NB_CLASSE_POIDS);
                  vl_MesPoids.Mesures[vl_Index] = (XDY_Mot) (pl_Mesure[vl_Index]), vl_Index++ );
	       
            /*A Appel de la fonction utilsateur avec la structure XZEAT_EMES_RADT_POIDS en argument */
            pg_FonctUtil_XZEA_33[2] ( vl_MesPoids, &vl_Resultat );
            break;
       }
 	
      if ( vl_Resultat == XDC_OK )
      {
         /*A Construction du DatGroup XDG_ACKNOLEDGE_R<NumRADT> */
         sprintf ( pl_NomDG, "%s_R%d", XDG_ACKNOLEDGE, vl_Numero ) ;
   	
        /*A Envoi du message XDM_Acknoledge_Pesage en fournissant les valeur recues  */
        if (!TipcSrvMsgWrite (  pl_NomDG
      			  ,XDM_IdentMsg(XDM_Acknoledge_Pesage)
  			  ,XDM_FLG_SRVMSGWRITE
 			  ,T_IPC_FT_INT2,  vl_Numero
 			  ,T_IPC_FT_REAL8, vl_Horodate
 			  ,T_IPC_FT_INT2,  vl_TypePesage
                          ,NULL))
         {
  		/*A Trace envoi message XDM_Acknoledge_Pesage  non effectue */    	
         	XZST_03EcritureTrace(XZSTC_WARNING ,"%s : Envoi message XDM_Acknoledge non effectue", version);
         }
      }
      else
      {
          XZST_03EcritureTrace(XZSTC_WARNING ,"%s : Mise en base des mesures de poids impossible pour station:%d sens:%d voie:%d type:%d", 
          			version, vl_Horodate, vl_Numero, vl_Sens, vl_Voie, vl_TypePesage);
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
*  Abt aux mesures de Pesage des stations RADT
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	XZEA33_Abt_Mesures_Pesage (	XDY_Fonction	pa_FonctUtilSilhouette,
					XDY_Fonction	pa_FonctUtilEssieu,
					XDY_Fonction	pa_FonctUtilPoids,
					XDY_Nom		pa_NomSite,
					int *		pa_Resultat )

/*
* ARGUMENTS EN ENTREE :
*
* XDY_Fonction	pa_FonctUtilSilhouette	: Nom de la fonction utilisateur pour les silhouettes
* XDY_Fonction	pa_FonctUtilEssieu	: Nom de la fonction utilisateur pour les nb essieu
* XDY_Fonction	pa_FonctUtilPoids	: Nom de la fonction utilisateur pour les poids
* XDY_Nom	pa_NomSite		: Nom du site demande
*
* ARGUMENTS EN SORTIE :
*
* int		pa_resultat		: Code retour resultat
*
* CODE RETOUR : 
*   XDC_OK		Succes
*
* CONDITION D'UTILISATION
*   Ce service est demande pour s'abonner aux mesures Pesage RADT
*
* FONCTION 
*   Armement CallBack et joindre le Datagroups
*
------------------------------------------------------*/
{
static char *version = "@(#)xzea33.c	1.1 10/09/96 : XZEA33_Abt_Mesures_Pesage ";
int 		vl_ValRet = XDC_OK;
T_BOOL		vl_EtatDg = FALSE;
XDY_Datagroup	pl_NomDG;

        XZST_03EcritureTrace ( XZSTC_INTERFACE, "IN  : %s", version );
     		
        /* Memorisation en globale du nom de la fonction utilisateur */		
        pg_FonctUtil_XZEA_33[0] = pa_FonctUtilSilhouette;
        pg_FonctUtil_XZEA_33[1] = pa_FonctUtilEssieu;
        pg_FonctUtil_XZEA_33[2] = pa_FonctUtilPoids;
    
        /*A  Armement Callback sur Reception XDM_EMES_PESAGE */
        if ( vg_CB_XZEA33 == NULL )
        {
	   if ( ( vg_CB_XZEA33 = TipcSrvProcessCbCreate (	XDM_IdentMsg(XDM_EMES_PESAGE),
							      	XZEA_EMes_Pesage,
								(T_CB_ARG)NULL ) ) == NULL ) 
	   {	
	       XZST_03EcritureTrace ( XZSTC_WARNING, "Echec armement Callback sur XDM_EMES_PESAGE." );
	       vl_ValRet = XDC_NOK;
	   }
	   else
	   {	
	       XZST_03EcritureTrace ( XZSTC_DEBUG1, "Armement Callback sur XDM_EMES_PESAGE." );
	   }
        }

	/*A Abonnement au Datagroup XDG_EMES_PESAGE<NomSite> */
	/*A Construction du nom du DataGroup */
	sprintf ( pl_NomDG, "%s_%s", XDG_EMES_PESAGE, pa_NomSite );
   	/*A Joindre ce datagroup s'il n'est pas deja joint */
   	TipcSrvSubjectGetSubscribe ( pl_NomDG, &vl_EtatDg );
   	if ( !vl_EtatDg )
   	{
   	   if ( !XDG_JoindreDG( pl_NomDG ) )
   	   {
      	      XZST_03EcritureTrace ( XZSTC_WARNING, "Echec abonnement au Datagroup %s ", pl_NomDG );
	      vl_ValRet = XDC_NOK;
	   }
	   else
	   {
      	      XZST_03EcritureTrace ( XZSTC_DEBUG1, "Abonnement au Datagroup %s ", pl_NomDG );
	   }
	}
  	
        XZST_03EcritureTrace ( XZSTC_INTERFACE, "OUT : %d Retour:%s.", vl_ValRet, version );
        *pa_Resultat = vl_ValRet;
        return ( vl_ValRet );				
				
}


