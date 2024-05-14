/*E*/
/* Fichier : $Id: xzae103.c,v 1.9 2020/11/03 17:56:43 pc2dpdy Exp $      Release : $Revision: 1.9 $        Date : $Date: 2020/11/03 17:56:43 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZAE * FICHIER xza103.c
******************************************************
* DESCRIPTION DU MODULE :
*
* xzae103  appartient A BASEDD .
* permet de s'abonner a l'etat de presence d'un operateur 
* dans un PC simplifie
*
******************************************************
* HISTORIQUE :
*
*  Niepceron    27 Aou 1994     : Creation
*  Claudel      09 Jul 2007     : Suppression DY
* Guilhou        31 oct 2007    : N postes operateur par district DEM/717
* JMG           29/09/09        : SECTO DEM 887
* JPL		08/12/10 : Migration architecture HP ia64 (DEM 961) : suppression warnings mineurs  1.6
* JPL		02/12/16 : Traces  1.7
* JMG   09/03/17        : REGIONALISATION - DEM1220 1.8
* LCL		20/12/21	: MOVIS P2 ems SAE-312
* LCL	  27/09/23	: Adaptations CNA DEM-483
*******************************************************/


/* fichiers inclus */

#include "xzae103.h"  

/* declaration des constantes locales */

/* declaration des variables locales  */
static char *version="@(#)xzae103.c	1.1 08/27/96 : XZAE " ;

T_CB			vg_CB_XZAE103  = NULL;
XDY_Fonction           pg_FonctUtil_XZAE103 = NULL;

XZAET_Machine_Presente vg_postes[XZAEC_NB_PO] ;



/* declaration de fonctions internes */



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	XZAE_Presence_OP_PCsimpl ( 
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
*  XDM_A_PRES_OP 
*
* FONCTION 
*
*
------------------------------------------------------*/
{
   static char *version="@(#)xzae103.c	1.1 08/27/96 : XZAE_Presence_OP_PCsimpl";
   
  int		vl_Resultat = XDC_OK;
#ifndef _TIBCO_EMS 
  T_STR		vl_Machine;
  T_INT2        vl_Site ;  /* variables de lecture de message */
  T_INT2        vl_Presence;
#else
  XDY_NomMachine		vl_Machine;
  XDY_Mot        vl_Site ;  /* variables de lecture de message */
  XDY_Mot        vl_Presence;
#endif
  XZAET_EPresence_Op_PCsimpl	vl_presence_op;
  int		vl_trouve, vl_trouveAutre;
  int		vl_index_insertion;
  int		vl_indice;

  /* decodage du message */
  if (!TipcMsgRead(pa_Data->msg,
                   T_IPC_FT_INT2,&vl_Site,
                   T_IPC_FT_INT2,&vl_Presence,
		   T_IPC_FT_STR,&vl_Machine,
                   NULL))
  {
    XZST_03EcritureTrace( XZSTC_WARNING,
		"XZAE_Presence_OP_PCsimpl : Pb lecture message XDM_A_PRES_OP " );
    return;
  }

   XZST_03EcritureTrace ( XZSTC_FONCTION, 
   "IN  : XZAE_Presence_OP_PCsimpl %d-%d-%s",
		vl_Site,vl_Presence,vl_Machine);
   vl_presence_op.Presence = (XDY_Booleen) vl_Presence;
   
  /*si une machine devient presente*/
  if (vl_presence_op.Presence) {
    /* je recherche ma machine dans la liste*/
       vl_trouve=0;
       vl_index_insertion=-1;
       for (vl_indice=0; vl_indice<XZAEC_NB_PO;vl_indice++) {
         if ( (!strcmp(vg_postes[vl_indice].Machine,vl_Machine)) &&
	      (vg_postes[vl_indice].Site==vl_Site)
	    ) {
		vl_trouve=1;
		break;
	 }
	 else if  (( vg_postes[vl_indice].Site==0) &&
		   (vl_index_insertion==-1)
		 )
		vl_index_insertion=vl_indice;
       }
       /*si machine pas trouvee*/
       if (vl_trouve==0) {
		XZST_03EcritureTrace( XZSTC_INFO,"Presence notifiee machine %s / %d (%d)",vl_Machine,(int) vl_Site,vl_index_insertion);
	  strcpy(vg_postes[vl_index_insertion].Machine,vl_Machine);
	  vg_postes[vl_index_insertion].Site=vl_Site;
       }
  }
  else { /*une machine disparait*/
    /* je recherche ma machine dans la liste*/
    vl_trouve=0;
    for (vl_indice=0; vl_indice<XZAEC_NB_PO;vl_indice++) {
      if ( (!strcmp(vg_postes[vl_indice].Machine,vl_Machine)) &&
	      (vg_postes[vl_indice].Site==vl_Site)
	    ) {
        vl_trouve=1;
        break;
      }
    }
    /*si machine trouvee*/
    if (vl_trouve==1) {
      strcpy(vg_postes[vl_indice].Machine,"");
      vg_postes[vl_indice].Site=0;
    }

    vl_trouveAutre=0;
    /*je regarde maintenant s'il reste d'autres PO sur le site*/
    for (vl_indice=0; vl_indice<XZAEC_NB_PO;vl_indice++) {
      if (vg_postes[vl_indice].Site==vl_Site) {
	  vl_trouveAutre=1;
	  break;
      }
    }
    /*si une autre machine du site est present, le site est present*/
    if (vl_trouveAutre==1) {
      vl_presence_op.Presence=TRUE;
      if (vl_trouve==1) {
	XZST_03EcritureTrace( XZSTC_INFO,"Absence notifiee machine %s",vl_Machine);
      }
    } else {
      vl_presence_op.Presence=FALSE;
      if (vl_trouve==1) {
	XZST_03EcritureTrace( XZSTC_INFO,"Absence notifiee machine %s, site %d absent",vl_Machine,(int) vl_Site);
      }
    }
  }

   switch ( vl_Site )
   {
   case XDC_CI :
   	strcpy( vl_presence_op.NomSite, XDC_NOM_SITE_CI );
   	break;
   case XDC_VC :
   	strcpy( vl_presence_op.NomSite, XDC_NOM_SITE_VC );
   	break;
   case XDC_DP :
   	strcpy( vl_presence_op.NomSite, XDC_NOM_SITE_DP );
   	break;
   case XDC_CA :
   	strcpy( vl_presence_op.NomSite, XDC_NOM_SITE_CA );
   	break;
   default :
          XZST_03EcritureTrace ( XZSTC_WARNING,  "XZAE_Presence_OP_PCsimpl: Reception site Inconnu : %d ", vl_Site );
          return ;
   }

   /*A Appel de la fonction utilsateur avec la structure XZSET_EComm_Site en argument */
   pg_FonctUtil_XZAE103 ( vl_presence_op );
   
   XZST_03EcritureTrace ( XZSTC_FONCTION,  "OUT : XZAE_Presence_OP_PCsimpl " );
}



int XZAE103_Abt_Presence_Op_PCsimpl(XDY_Booleen va_abonnement, XDY_Fonction pa_fonction)

/*X*/
/******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSE 
******************************************************
* ALGORITHME :
*
*
* ARGUMENTS EN ENTREE :
*
* XDY_Booleen	va_abonnement		: Abonnement : Debut ou fin d'abonnement
* XDY_Fonction	pa_fonction		: Nom de la fonction utilisateur
*
* ARGUMENTS EN SORTIE :
*
* int		pa_resultat		: Code retour resultat
*
* CODE RETOUR : 
*   XDC_OK		Succes
*
* CONDITION D'UTILISATION
*   Ce service est demande pour s'abonner aux etat de communication des Sites
*
* FONCTION 
*   Si demande d'abonnement
*     Alors : armement CallBack et joindre le Datagroup correspondant
*     Sinon : Desarmment CallBack (si demande fermeture) et disjoindre le Datagroup
********************************************************/

{
   static char *version="@(#)xzae103.c	1.1 08/27/96 : XZAE103_Abt_Presence_Op_PCsimpl";
XDY_Datagroup	pl_NomDG;
int 		vl_ValRet = XDC_OK;
T_BOOL		vl_EtatDg = FALSE;
int	vl_indice;

     XZST_03EcritureTrace ( XZSTC_FONCTION, 
     "IN  :  XZAE103_Abt_Presence_Op_PCsimpl Abt:%d Fonct:%ld .",
     		va_abonnement,
     		pa_fonction);
     		
     /* Memorisation en globale du nom de la fonction utilisateur */		
     pg_FonctUtil_XZAE103 = pa_fonction;
     
     /* init des tableaux de presence*/
     for (vl_indice=0; vl_indice < XZAEC_NB_PO ; vl_indice++) {
	vg_postes[vl_indice].Site=0;
     }

     /*A Si demande d'abonnement */
     if ( va_abonnement == XZEAC_DEBUT_ABONNEMENT )
     {
	/*A  Armement Callback sur Reception XDM_A_PRES_OP */
	if ( vg_CB_XZAE103 == NULL )
	{
	   if ( ( vg_CB_XZAE103 = TipcSrvProcessCbCreate (	XDM_IdentMsg( XDM_A_PRES_OP),
							      	XZAE_Presence_OP_PCsimpl,
								(T_CB_ARG)NULL ) ) == NULL ) 
	   {	
	      /*B Ecriture Trace */
	      XZST_03EcritureTrace ( XZSTC_WARNING, "Echec armement Callback sur XDM_A_PRES_OP ." );
	      vl_ValRet = XDC_NOK;
	   }
	   else
	   {	
	      /*B Ecriture Trace */
	      XZST_03EcritureTrace ( XZSTC_DEBUG1, "Armement Callback sur XDM_A_PRES_OP ." );
	   }
	   
	}
	
	

   	/*A Joindre ce datagroup s'il n'est pas deja joint */
   	TipcSrvSubjectGetSubscribe ( XDG_A_PRES_OP, &vl_EtatDg );
   	if ( !vl_EtatDg )
   	{
   	   if ( !XDG_JoindreDG( XDG_A_PRES_OP ) )
   	   {
	      /*B Ecriture Trace */
      	      XZST_03EcritureTrace ( XZSTC_WARNING, "Echec abonnement au Datagroup %s ", XDG_A_PRES_OP );
	      vl_ValRet = XDC_NOK;
	   }
	   else
	   {
      	      XZST_03EcritureTrace ( XZSTC_DEBUG1, "Abonnement au Datagroup %s ", XDG_A_PRES_OP );
	   }
	}
	/*A Demande l'etat des operateurs */
        if (!TipcSrvMsgWrite(   XDG_IAX_INITPRES
                        ,XDM_IdentMsg(XDM_IAX_INITPRES )
                        ,XDM_FLG_SRVMSGWRITE
                        ,NULL) )
             {
             /* Affichage Trace */
             XZST_03EcritureTrace(XZSTC_DEBUG1
                        ,"XZAE103:TipcSrvMsgWrite(%s,%u,XDM_FLG_SRVMSGWRITE,NULL)"
                        ,XDG_IAX_INITPRES,XDM_IdentMsg(XDM_IAX_INITPRES) );
             vl_ValRet  = XDC_NOK;
             }
	
	
     }
     else
     {
        if ( ( va_abonnement == XZEAC_FERMER_ABONNEMENT ) && ( vg_CB_XZAE103 != NULL ) )
        {
           /*A  Desarmement Callback sur Reception XDG_A_PRES_OP */
	   if ( TutCbDestroy ( vg_CB_XZAE103 ) == FALSE )  
	   {	
	      /*B Ecriture Trace */
	      XZST_03EcritureTrace ( XZSTC_WARNING, "Echec desarmement Callback sur XDG_A_PRES_OP ." );
	      vl_ValRet = XDC_NOK;
	   }
	   else
	   {
	      XZST_03EcritureTrace ( XZSTC_DEBUG1, "Desarmement Callback surXDG_A_PRES_OP  ." );
	      vg_CB_XZAE103 = NULL;
	   }
	}

	/*A Desabonnement au Datagroup XDG_A_PRES_OP */
   	/*A Disjoindre ce datagroup s'il est deja joint */
   	TipcSrvSubjectGetSubscribe ( XDG_A_PRES_OP, &vl_EtatDg );
   	if ( vl_EtatDg )
   	{
   	   if ( !XDG_QuitterDG( XDG_A_PRES_OP ) )
   	   {
	      /*B Ecriture Trace */
      	      XZST_03EcritureTrace ( XZSTC_WARNING, "Echec desabonnement au Datagroup %s ", XDG_A_PRES_OP );
	      vl_ValRet = XDC_NOK;
	   }
	   else
	   {
      	      XZST_03EcritureTrace ( XZSTC_DEBUG1, "Desabonnement au Datagroup %s ", XDG_A_PRES_OP );
	   }
	}
	
     }
     
   XZST_03EcritureTrace ( XZSTC_FONCTION, "OUT :  XZAE103_Abt_Presence_Op_PCsimpl Retour:%d.", vl_ValRet );
   return ( vl_ValRet );				
				
}
