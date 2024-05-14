/*E*/
/*Fichier : $Id: xzez.c,v 1.23 2020/09/25 07:35:22 pc2dpdy Exp $      Release : $Revision: 1.23 $        Date : $Date: 2020/09/25 07:35:22 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE XZEZ  * FICHIER xzez.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* c'est le module d'interface de la tache Temps de Parcours
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	03 Mar 1997	: Creation (DEM/1417)
* Mismer.D	version 1.2	05 Jun 1997	: Ajout zone statique (DEM/1441)
* Mismer.D	version 1.3	23 Jun 1997	: Correction calcul TDP (DEM/1457)
* Guilhou	version 1.4	19 jan 2000	: correction calcul tdp si zdp en panne (SAE/32)
* Niepceron	version 1.6	29 avr 2005	: tdp negatif si zdp en panne ou donnée null DEM464
* Niepceron	version 1.7	22 Sep 2005	: si Destination n'est pas trouvée ou =0 alors le tdp retourné est <0 dEM517
* LCL		version 1.8	22/06/11	: TDP A8
* VR		version 1.9	04/11/11	: Ajout destNoeud (DEM/1014)
* JMG		version 1.10 	23/07/12 :  correction xzez02 pour linux
* JMG		11/06/12	ajout de xzez04, xzez05, xzez06 (DEM/1014 PMA)
* JMG		25/01/13 :	correction A57
* JMG		13/02/13 :	correction PMVA et destination dans la meme zone
* JMG		22/02/13 :  correction coeff zone 1 si autoroutes differentes
* JMG		17/01/17 : ajout XZEZ10_Calcul_Traverse (lot 23) 
* JMG		17/01/17 : ajout XZEZ10_Calcul_Traverse (lot 23) 1.15
* PNI		16/08/17 : remplacement de 0 par -1 pour valeur inconnue de vl_pr_destination 1.18 DEM1237
* JMG		02/09/18 : ajout XZEZ11 LOT25 DEM1301 1.19
* PNI		26/06/19 : correctif calcul tdp traversee en cas d'indispo d'une zone 1.20 DEM1340
* JMG		12/11/19 : ajout surcharge TDP DEM1365 1.21
* GR            05/08/20 : ajout XZEZ12_Envoi_TDP_FCD 1.22 DEM-SAE152
* ABE		24/09/20 : Appel acli_XZAO960_cb dans XZEZ12 Creation DEM-SAE152 V1.23
* LCL		20/12/21	: MOVIS P2 ems SAE-312
------------------------------------------------------*/

/* fichiers inclus */

#include "xzez.h"


/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "@(#)xzez.c	1.8 06/23/11 : xzea35" ;
XDY_Fonction		pg_FonctUtil_XZEZ_01 = NULL;
XDY_Fonction		pg_FonctUtil_XZEZ_04 = NULL;
T_CB			vg_CB_XZEZ01         = NULL;
T_CB			vg_CB_XZEZ04         = NULL;

static int vg_toggle=0;

/* declaration de fonctions internes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	XZEZ_Dde_Valeur_TDP ( 
        T_IPC_CONN 			va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_PTR                           pa_ARG )
 
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
*  Fonction appelÅe lors de la rÅception du message
*  XDM_VAL_TDP 
*
* FONCTION 
*
*
------------------------------------------------------*/
{
   static char *version = "@(#)xzez.c	1.8 06/23/11 : XZEZ_Dde_Valeur_TDP ";
#ifndef _TIBCO_EMS
   T_INT2		vl_Numero;
   T_STR		vl_Dest;
   T_STR		vl_DestAlt;
   T_STR		vl_DestNoeud;
#else
   XDY_Mot		vl_Numero;
   XDY_Ligne_PMV	vl_Dest;
   XDY_Ligne_PMV	vl_DestAlt;
   XDY_Ligne_PMV	vl_DestNoeud;
#endif
   XZEZT_Val_TDP	vl_TDP;
   int			vl_Resultat = XDC_OK;
   XDY_NomMachine vl_machine="";
   
		  
   /* Recuperation des parametres du message XDM_VAL_TDP */
   if ( !TipcMsgRead(pa_Data -> msg,
               		T_IPC_FT_INT2, 	&vl_Numero,
               		T_IPC_FT_STR, 	&vl_Dest,
               		T_IPC_FT_STR, 	&vl_DestAlt,
					T_IPC_FT_STR, &vl_DestNoeud,
					
	       		NULL) )
   {
      XZST_03EcritureTrace ( XZSTC_WARNING," Impossible de lire le MSG XDM_VAL_TDP dans XZEZ_Dde_Valeur_TDP ");
   }
   else
   {
      
      XZST_03EcritureTrace ( XZSTC_FONCTION,"IN : XZEZ_Dde_Valeur_TDP  avec:\tNumero\t: %d\tDest\t: <%s>\tDestAlt\t: <%s>\tDestNoeud\t: <%s>",
            				 vl_Numero,
            				 vl_Dest,
            				 vl_DestAlt,
							 vl_DestNoeud);
      vl_TDP.Numero = vl_Numero;
      strcpy ( vl_TDP.Dest, vl_Dest );
      strcpy ( vl_TDP.DestAlt, vl_DestAlt );
	  strcpy ( vl_TDP.DestNoeud, vl_DestNoeud );

	  if (XZSC_GetDestinataire(pa_Data, vl_machine)!=XDC_OK) {
		XZST_03EcritureTrace(XZSTC_WARNING,"XZEZ_Dde_Valeur_TDP : probleme dans recuperation machine destinataire du message");
	  }
             
      /*A Appel de la fonction utilsateur avec la structure XDY_EMes_TdP_6mn en argument */
      pg_FonctUtil_XZEZ_01 ( vl_TDP, &vl_Resultat, vl_machine  );
      XZST_03EcritureTrace ( XZSTC_FONCTION,  "OUT : XZEZ_Dde_Valeur_TDP " );
   }
}
/* definition de fonctions externes */



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	XZEZ01_Dde_Valeur_TDP (		XDY_Datagroup	pa_DGReponse,
					XDY_Fonction	pa_FonctUtilisateur,
					XZEZT_DDE_TDP	va_TDP,
					int *		pa_Resultat )

/*
* ARGUMENTS EN ENTREE :
*
* XDY_Datagroup	pa_DGReponse		: Nom du DataGroup d'Init
* XDY_Fonction	pa_FonctUtilisateur	: Nom de la fonction utilisateur
* XZEZT_DDE_TDP	va_TDP			: Numero du PMV , de la destination et de la destination pour alternat
*
* ARGUMENTS EN SORTIE :
*
* int		pa_resultat		: Code retour resultat
*
* CODE RETOUR : 
*   XDC_OK		Succes
*
* CONDITION D'UTILISATION
*   Ce service est demande pour recevoir le TDP pour un PMV et une destination donnee
*
* FONCTION 
*     Armement CallBack et joindre le Datagroup d'entree
*
------------------------------------------------------*/
{
static char *version = "@(#)xzez.c	1.8 06/23/11 : XZEZ01_Dde_Valeur_TDP ";
int 		vl_ValRet = XDC_OK;
T_BOOL		vl_EtatDg = FALSE;
XDY_Datagroup	vl_dg;

     XZST_03EcritureTrace ( XZSTC_FONCTION, 
     "IN  : XZEZ01_Dde_Valeur_TDP DGInit:%s Fonct:%ld NumPMV:%d Dest:%d DestAlt:%d DestNoeud=%d. site = %d",
     		pa_DGReponse,
     		pa_FonctUtilisateur,
     		va_TDP.Numero,
     		va_TDP.Dest,
     		va_TDP.DestAlt,
			va_TDP.DestNoeud ,
			va_TDP.Site);
     		
     /* Memorisation en globale du nom de la fonction utilisateur */		
     pg_FonctUtil_XZEZ_01 = pa_FonctUtilisateur;
     
	/*A  Armement Callback sur Reception XDM_VAL_TDP */
	if ( vg_CB_XZEZ01 == NULL )
	{
	   if ( ( vg_CB_XZEZ01 = TipcSrvProcessCbCreate (	XDM_IdentMsg(XDM_VAL_TDP),
							      	XZEZ_Dde_Valeur_TDP,
								(T_CB_ARG)NULL ) ) == NULL ) 
	   {	
	      /*B Ecriture Trace */
	      XZST_03EcritureTrace ( XZSTC_WARNING, "Echec armement Callback sur XDM_VAL_TDP." );
	      vl_ValRet = XDC_NOK;
	   }
	   else
	   {	
	      /*B Ecriture Trace */
	      XZST_03EcritureTrace ( XZSTC_DEBUG1, "Armement Callback sur XDM_VAL_TDP." );
	   }
	}
		
	/*A Abonnement au Datagroup DGInit passe en parametre */
   	/*A Joindre ce datagroup s'il n'est pas deja joint */
/*RT35 obsolete   	TipcSrvDgGetRecv ( pa_DGReponse, &vl_EtatDg ); */
	TipcSrvSubjectGetSubscribe ( pa_DGReponse, &vl_EtatDg );
   	if ( ( !vl_EtatDg ) && ( pa_DGReponse != NULL ) )
   	{
   	   if ( !XDG_JoindreDG( pa_DGReponse ) )
   	   {
	      /*B Ecriture Trace */
      	      XZST_03EcritureTrace ( XZSTC_WARNING, "Echec abonnement au Datagroup %s ", pa_DGReponse );
	      vl_ValRet = XDC_NOK;
	   }
	   else
	   {
      	      XZST_03EcritureTrace ( XZSTC_DEBUG1, "Abonnement au Datagroup %s ", pa_DGReponse );
	   }
	}

           /*A Envoi du message de demande d'init XDM_ETDP_IHM en fournissant les valeur recues  */
   
	   sprintf(vl_dg,"%s_%d",XDG_EINIT_ABT_TDP,va_TDP.Site);
   	   if (!TipcSrvMsgWrite ( 	vl_dg,
      			  		XDM_IdentMsg(XDM_ETDP_IHM),
  			  		XDM_FLG_SRVMSGWRITE,
                          		T_IPC_FT_INT2, va_TDP.Numero,
                          		T_IPC_FT_INT2, va_TDP.Dest,
                          		T_IPC_FT_INT2, va_TDP.DestAlt,
                          		T_IPC_FT_STR,  pa_DGReponse,
					T_IPC_FT_INT4, va_TDP.DestNoeud, 
                          		NULL))
      	   {
  		/*A Trace envoi message XDM_ETDP_IHM non effectue */    	
         	XZST_03EcritureTrace(XZSTC_WARNING, "Echec envoi message XDM_ETDP_IHM.");
	   }
	   else
      	   {
  		/*A Trace envoi message XDM_ETDP_IHM */    	
         	XZST_03EcritureTrace(XZSTC_DEBUG1, "Envoi du message XDM_ETDP_IHM sur %s.", vl_dg );
	   }
     
   XZST_03EcritureTrace ( XZSTC_FONCTION, "OUT : XZEZ01_Dde_Valeur_TDP Retour:%d.", vl_ValRet );
   *pa_Resultat = vl_ValRet;
   return ( vl_ValRet );				
				
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	XZEZ02_Calcul_TDP (	XDY_Autoroute			va_autoroute_pmv,
XDY_PR				va_pr_pmv,
XDY_Sens			va_sens_pmv,
XDY_Eqt				va_dest,
XZEZT_Liste_Zdp	*		pa_ListeZone,
XZEZT_Liste_Destinations *	pa_ListeDest,
XZEZT_Liste_Zdp	*		pa_ListeZoneData,
XDY_TDP *			pa_tdp ,
char				*pa_nom_dest)	

/*
* ARGUMENTS EN ENTREE :
*
* XDY_PR	va_autoroute_pmv	: Si =255 On ne calcul que des zones virtuelles
*
* ARGUMENTS EN SORTIE :
*
* int		pa_resultat		: Code retour resultat
*
* CODE RETOUR : 
*   XDC_OK		Succes
*
* CONDITION D'UTILISATION
*   Ce service est demande pour recevoir le TDP pour un PMV et une destination donnee
*
* FONCTION 
*
------------------------------------------------------*/
{
	static char *version = "@(#)xzez.c	1.8 06/23/11 :  XZEZ02_Calcul_TDP ";
	float			vl_tdp_pmv=0;
	float			vl_tdp_dest=0;
	float			vl_tdp_zdp=0;
	float			vl_tdp_inter;
	XDY_Autoroute		vl_autoroute;
	float		vl_tdp_calcul=0;
	XDY_PR			vl_pr_destination=-1;
	XDY_PR			vl_pr_deb=0,vl_pr_fin=0;
	XDY_PR			vl_pr;
	XDY_Booleen			vl_Type_Loc=0;
	float 			vl_Coef = 0;
	XZEZT_Liste_Zdp *		pl_Zone;
	XZEZT_Liste_Zdp *		pl_ZoneData;
	XZEZT_Liste_Destinations *	pl_Dest;
	XDY_Booleen			vl_tdpvalide=XDC_OK;
	float vl_mon_tdp;
	XDY_Booleen vl_HorsReseau=XDC_FAUX;
	XDY_Booleen vl_TDP_terrain_valide = XDC_VRAI;

	XZST_03EcritureTrace(XZSTC_FONCTION,"IN : %s pour PMV %d-%d-%ld  et dest:%d", version, va_autoroute_pmv, va_sens_pmv, va_pr_pmv, va_dest );

	if (!strcmp(pa_nom_dest,"HORS RESEAU")) //Test si la destination est hors reseau, utilisation du nom du PMV qui est set a "" juste apres pour eviter de rajouter une var et relivrer moult tache
		vl_HorsReseau=XDC_VRAI;

	XZST_03EcritureTrace(XZSTC_INFO,"Hors reseau=%d",vl_HorsReseau);

	strcpy(pa_nom_dest,"");
	/*DEM517 si dest=0 alors on retourne pa_tdp=-1 */
	if ( va_dest == 0 )
	{
		*pa_tdp=-1;
		return (XDC_OK);
	}

	/* Recherche de la destination dans la liste */
	vl_pr_destination=0;
	for ( pl_Dest = pa_ListeDest; pl_Dest != NULL; pl_Dest = pl_Dest->suivant ) {
		XZST_03EcritureTrace(XZSTC_DEBUG1,"destination %d",pl_Dest->destination.Numero);
		if ( pl_Dest->destination.Numero == va_dest )
		{
			vl_pr_destination = pl_Dest->destination.PR;
			vl_Type_Loc 	 = pl_Dest->destination.Type_Localisation;
			vl_autoroute	 = pl_Dest->destination.Autoroute;
			strncpy(pa_nom_dest,pl_Dest->destination.Libelle_PMV,14);
			pa_nom_dest[14]='\0';
			break;
		}
	}
	/*DEM517 si dest non trouvée alors on retourne pa_tdp=-1 */
	/*if ( pl_Dest->destination.Numero != va_dest )*/
	if (vl_pr_destination==-1)
	{
		*pa_tdp=-1;
		return (XDC_OK);
	}

	XZST_03EcritureTrace(XZSTC_INFO," %s PR dest:%d loc:%d", version, vl_pr_destination, vl_Type_Loc );
	vl_tdpvalide=XDC_NOK;
	for ( pl_Zone = pa_ListeZone; pl_Zone != NULL; pl_Zone = pl_Zone->suivant )
	{
		if (pl_Zone != NULL) {
			XZST_03EcritureTrace(XZSTC_WARNING,"pl_Zone non null %d code zone %d",pl_Zone->zdp.Numero,pl_Zone->zdp.Code_Zone);
		}
		else
		XZST_03EcritureTrace(XZSTC_WARNING,"debut pl_Zone == null");
			
			if (vl_HorsReseau == XDC_FAUX) //Si on est pas sur un destination hors reseau, on prend tout
				vl_TDP_terrain_valide = XDC_VRAI;
			else if ( pl_Zone->zdp.Code_Zone >= 200 && pl_Zone->zdp.Code_Zone <= 300) // Si on est sur une destination hors reseau, on prend que les zones virtuelle entre 200 et 300
				vl_TDP_terrain_valide = XDC_VRAI;
			else
				vl_TDP_terrain_valide = XDC_FAUX;
				
			XZST_03EcritureTrace(XZSTC_INFO,"zone : %d code zone : %d TDP_valide pour calcul hybride: %d",pl_Zone->zdp.Numero,pl_Zone->zdp.Code_Zone,vl_TDP_terrain_valide);
			if (vl_TDP_terrain_valide) 
			{
				/*recup du TDP*/
				for ( pl_ZoneData = pa_ListeZoneData; pl_ZoneData != NULL; pl_ZoneData = pl_ZoneData->suivant ) {
					if (pl_ZoneData->zdp.Code_Zone == pl_Zone->zdp.Code_Zone)
					break;
				}
				if (pl_ZoneData==NULL) {
					*pa_tdp=-1;
					return XDC_OK;
				}

				if (pl_ZoneData->zdp.TDP_Surcharge !=0)
				{
					if (pl_ZoneData->zdp.TDP_Surcharge==-1)
					vl_tdp_calcul=0;
					else
					vl_tdp_calcul = pl_ZoneData->zdp.TDP_Surcharge;
					XZST_03EcritureTrace(XZSTC_INFO,"surcharge TDP %f",vl_tdp_calcul);
				}
				else {
					vl_tdp_calcul = pl_ZoneData->zdp.TDP_Courant;
					XZST_03EcritureTrace(XZSTC_INFO,"TDP classique %f",vl_tdp_calcul);	
				}

				vl_mon_tdp = vl_tdp_calcul;
				vl_tdpvalide=XDC_OK;
				/*si zone virtuelle*/
				XZST_03EcritureTrace(XZSTC_DEBUG1," type zone %d",pl_Zone->zdp.Type_Zone);
				if (pl_Zone->zdp.Type_Zone==0)
				vl_mon_tdp = pl_Zone->zdp.TDP_Normal;

				if (vl_mon_tdp != 0)
				{
					/*si PMV et destination dans la ZDP*/
					if ( ( pl_Zone->zdp.PR_Debut < vl_pr_destination && pl_Zone->zdp.PR_Fin > vl_pr_destination && pl_Zone->zdp.Autoroute == vl_autoroute ) &&
							( pl_Zone->zdp.PR_Debut < va_pr_pmv && pl_Zone->zdp.PR_Fin > va_pr_pmv  && pl_Zone->zdp.Autoroute ==va_autoroute_pmv)) {
						if (pl_Zone->zdp.Sens == XDC_SENS_NORD)
							vl_Coef = (float)(va_pr_pmv - vl_pr_destination)/ (float)(pl_Zone->zdp.PR_Fin - pl_Zone->zdp.PR_Debut);
						else
							vl_Coef = (float)(vl_pr_destination - va_pr_pmv)/ (float)(pl_Zone->zdp.PR_Fin - pl_Zone->zdp.PR_Debut);

					}
					/* si la Zdp encadre la sortie, il faut appliquer le ratio */
					/* PR destination compris entre PR debut et PR fin de la zone, et sur le meme autoroute */
					else if ( pl_Zone->zdp.PR_Debut < vl_pr_destination && pl_Zone->zdp.PR_Fin > vl_pr_destination && pl_Zone->zdp.Autoroute == vl_autoroute )  {
						if (pl_Zone->zdp.Autoroute == 8) {
							if (pl_Zone->zdp.Sens == XDC_SENS_NORD)
								vl_Coef = (float)(vl_pr_destination - pl_Zone->zdp.PR_Debut) / (float)(pl_Zone->zdp.PR_Fin - pl_Zone->zdp.PR_Debut);
							else
								vl_Coef = (float)(pl_Zone->zdp.PR_Fin - vl_pr_destination) / (float)(pl_Zone->zdp.PR_Fin - pl_Zone->zdp.PR_Debut);
						}
						else {
							if (pl_Zone->zdp.Sens == XDC_SENS_NORD)
								vl_Coef = (float)(pl_Zone->zdp.PR_Fin - vl_pr_destination) / (float)(pl_Zone->zdp.PR_Fin - pl_Zone->zdp.PR_Debut);
							else 
								vl_Coef = (float)(vl_pr_destination - pl_Zone->zdp.PR_Debut) / (float)(pl_Zone->zdp.PR_Fin - pl_Zone->zdp.PR_Debut);
						}
					}
					/* Si la Zdp encadre le PMV, il faut aussi appliquer le ratio */
					else if ( ( pl_Zone->zdp.PR_Debut < va_pr_pmv && pl_Zone->zdp.PR_Fin > va_pr_pmv ) && ( pl_Zone->zdp.Autoroute ==va_autoroute_pmv )){
						if (pl_Zone->zdp.Autoroute == 8) {
							if (pl_Zone->zdp.Sens == XDC_SENS_SUD)
								vl_Coef = (float)(va_pr_pmv - pl_Zone->zdp.PR_Debut) / (float)(pl_Zone->zdp.PR_Fin - pl_Zone->zdp.PR_Debut);
							else
								vl_Coef = (float)(pl_Zone->zdp.PR_Fin - va_pr_pmv) / (float)(pl_Zone->zdp.PR_Fin - pl_Zone->zdp.PR_Debut);
						}
						else {
							if (pl_Zone->zdp.Sens == XDC_SENS_SUD)
								vl_Coef = (float)(pl_Zone->zdp.PR_Fin - va_pr_pmv) / (float)(pl_Zone->zdp.PR_Fin - pl_Zone->zdp.PR_Debut);
							else 
								vl_Coef = (float)(va_pr_pmv - pl_Zone->zdp.PR_Debut) / (float)(pl_Zone->zdp.PR_Fin - pl_Zone->zdp.PR_Debut);
						}
					}	
					else
					vl_Coef = 1;

					/*PATCH HORRIBLE POUR MONACO*/
					if ((va_dest==30)&&(pl_Zone->zdp.Code_Zone==15))
					vl_Coef = 0.24;

					XZST_03EcritureTrace(XZSTC_FONCTION,"Donnee Zone PMV : coeff %f",vl_Coef);
					vl_tdp_zdp += vl_mon_tdp*vl_Coef;

					XZST_03EcritureTrace(XZSTC_FONCTION,"Donnee Zone PMV : %ld a %ld TDP:%f val:%f",pl_Zone->zdp.PR_Debut, pl_Zone->zdp.PR_Fin,vl_mon_tdp,vl_tdp_zdp );
				}
				else
				{
					vl_tdp_zdp += pl_Zone->zdp.TDP_Normal;
					XZST_03EcritureTrace(XZSTC_WARNING,"Pas de donnee Zone PMV : %ld a %ld TDP:%f val:%f", 
					pl_Zone->zdp.PR_Debut, pl_Zone->zdp.PR_Fin,vl_mon_tdp,vl_tdp_zdp );
					vl_tdpvalide=XDC_NOK;
					break;
				}
			}
			XZST_03EcritureTrace(XZSTC_INFO,"Zone INTER : %ld a %ld TDP:%f", pl_Zone->zdp.PR_Debut, pl_Zone->zdp.PR_Fin,vl_mon_tdp );
			XZST_03EcritureTrace(XZSTC_INFO,"fin");
		}

		/*retourne le tdp total*/
		/* *pa_tdp=vl_tdp_pmv+vl_tdp_dest+vl_tdp_zdp;*/
		XZST_03EcritureTrace(XZSTC_INFO,"info");
		XZST_03EcritureTrace(XZSTC_INFO,"%s PMV : %f  DEST : %f ENTRE : %f", version, vl_tdp_pmv, vl_tdp_dest, vl_tdp_zdp );
		/* Arrondi du TDP a la minute superieure */
		vl_tdp_inter=vl_tdp_pmv+vl_tdp_dest+vl_tdp_zdp;
		vl_tdp_inter = vl_tdp_inter / 60;

		*pa_tdp = (XDY_TDP)vl_tdp_inter + 1;

		/* retourne un tdp negatif si une donnée a été nulle */
		if ( vl_tdpvalide == XDC_NOK ) {
			*pa_tdp = -(*pa_tdp);
		}

		XZST_03EcritureTrace(XZSTC_FONCTION,"OUT: %s pour : Dest : %d Tdp : %d NOM : %s", version, va_dest, *pa_tdp,pa_nom_dest );
		return (XDC_OK);

	}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	XZEZ03_CompteRendu_TDP (	XDY_Entier	va_Num_Action,
					XDY_Booleen  	va_ResultatAction,
					XDY_Eqt		va_NumPMV,
					XDY_NomMachine	va_NomMachine )

/*
* ARGUMENTS EN ENTREE :
* va_Num_Action		: Numero action affichage TDP
* va_ResultatAction	: Resultat action (OK ou NOK)
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
*   XDC_OK		Succes
*
* CONDITION D'UTILISATION
*   Ce service est demande pour envoyer le compte rendu d'un affichage TDP
*
* FONCTION 
*
------------------------------------------------------*/
{
static char *version = "@(#)xzez.c	1.8 06/23/11 : XZEZ03_CompteRendu_TDP ";
int 		vl_ValRet = XDC_OK;
XDY_Datagroup   pl_NomDG;

     XZST_03EcritureTrace ( XZSTC_WARNING, 
     "IN  : XZEZ03_CompteRendu_TDP Action:%d CR:%d PMV:%d.", va_Num_Action, va_ResultatAction, va_NumPMV );
     		

     /*A Envoi du message de demande d'init XDM_CR_TDP en fournissant les valeur recues  */
     /*A Construction du DatGroup XDG_ETDP_PMV_<NomMachine> */
     XDG_EncodeDG2 ( pl_NomDG, XDG_ETDP_PMV, va_NomMachine ) ;   
     if (!TipcSrvMsgWrite ( 	pl_NomDG,
      			  	XDM_IdentMsg(XDM_CR_TDP),
  			  	XDM_FLG_SRVMSGWRITE,
                          	T_IPC_FT_INT4, va_Num_Action,
                          	T_IPC_FT_INT2, va_ResultatAction,
                          	T_IPC_FT_INT2, va_NumPMV,
                          	NULL))
     {
         /*A Trace envoi message XDM_CR_TDP non effectue */    	
         XZST_03EcritureTrace(XZSTC_WARNING, "Echec envoi message XDM_CR_TDP.");
         vl_ValRet = XDC_NOK;
     }
     else
     {
  	 /*A Trace envoi message XDM_CR_TDP */    	
         XZST_03EcritureTrace(XZSTC_WARNING, "Envoi du message XDM_CR_TDP sur %s.", pl_NomDG );
         vl_ValRet = XDC_OK;
     }
     
   XZST_03EcritureTrace ( XZSTC_WARNING, "OUT : XZEZ03_CompteRendu_TDP Retour:%d.", vl_ValRet );
   return ( vl_ValRet );				
				
}
/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	XZEZ_Dde_Valeur_TDP_PMVA (
        T_IPC_CONN 			va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_PTR                           pa_ARG )

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
*  XDM_VAL_TDP
*
* FONCTION
*
*
------------------------------------------------------*/
{
   static char *version = "@(#)xzez.c	1.8 06/23/11 : XZEZ_Dde_Valeur_TDP ";
   XZEZT_Val_TDP_PMVA	vl_TDP;
   int			vl_Resultat = XDC_OK;
#ifndef _TIBCO_EMS
   T_INT2		vl_Numero;
   T_STR		vl_Dest1_proche;
   T_STR		vl_Dest1_loin;
   T_STR		vl_Dest2_proche;
   T_STR		vl_Dest2_loin;
#else
   T_INT2		vl_Numero;
   XDY_Ligne_PMVA		vl_Dest1_proche;
   XDY_Ligne_PMVA		vl_Dest1_loin;
   XDY_Ligne_PMVA		vl_Dest2_proche;
   XDY_Ligne_PMVA		vl_Dest2_loin;
#endif
   XDY_NomMachine vl_machine="";

   /* Recuperation des parametres du message XDM_VAL_TDP */
   if ( !TipcMsgRead(pa_Data -> msg,
               		T_IPC_FT_INT2, 	&vl_Numero,
               		T_IPC_FT_STR, 	&vl_Dest1_proche,
               		T_IPC_FT_STR, 	&vl_Dest1_loin,
               		T_IPC_FT_STR, 	&vl_Dest2_proche,
               		T_IPC_FT_STR, 	&vl_Dest2_loin, NULL) )
   {
      XZST_03EcritureTrace ( XZSTC_WARNING," Impossible de lire le MSG XDM_VAL_TDP_PMVA dans XZEZ_Dde_Valeur_TDP_PMVA ");
   }
   else
   {

      XZST_03EcritureTrace ( XZSTC_FONCTION,"IN : XZEZ_Dde_Valeur_TDP_PMVA  avec:\tNumero\t: %d\tDest1_p\t: <%s>\tDest1_l\t: <%s>\tDest2_p\t: <%s>\tDest2_l\t: <%s>",
            				 vl_Numero,
            				 vl_Dest1_proche,
            				 vl_Dest1_loin,
            				 vl_Dest2_proche,
            				 vl_Dest2_loin);
      vl_TDP.Numero = vl_Numero;
      strcpy ( vl_TDP.Dest1_proche, vl_Dest1_proche );
      strcpy ( vl_TDP.Dest1_loin, vl_Dest1_loin );
	  strcpy ( vl_TDP.Dest2_proche, vl_Dest2_proche );
	  strcpy ( vl_TDP.Dest2_loin, vl_Dest2_loin );

	  if (XZSC_GetDestinataire(pa_Data, vl_machine)!=XDC_OK) {
		XZST_03EcritureTrace(XZSTC_WARNING,"XZEZ_Dde_Valeur_TDP_PMVA : probleme dans recuperation machine destinataire du message");
	  }

      /*A Appel de la fonction utilsateur avec la structure XDY_EMes_TdP_6mn en argument */
      XZST_03EcritureTrace ( XZSTC_FONCTION,  "avant pg_FonctUtil_XZEZ_01");
      pg_FonctUtil_XZEZ_04 ( vl_TDP, &vl_Resultat, vl_machine  );
      XZST_03EcritureTrace ( XZSTC_FONCTION,  "OUT : XZEZ_Dde_Valeur_TDP_PMVA " );
   }
}
/* definition de fonctions externes */



/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	XZEZ04_Dde_Valeur_TDP_PMVA (		XDY_Datagroup	pa_DGReponse,
					XDY_Fonction	pa_FonctUtilisateur,
					XZEZT_DDE_TDP_PMVA	va_TDP,
					int *		pa_Resultat )

/*
* ARGUMENTS EN ENTREE :
*
* XDY_Datagroup	pa_DGReponse		: Nom du DataGroup d'Init
* XDY_Fonction	pa_FonctUtilisateur	: Nom de la fonction utilisateur
* XZEZT_DDE_TDP_PMVA	va_TDP			: Numero du PMVA , des 4 destinations
*
* ARGUMENTS EN SORTIE :
*
* int		pa_resultat		: Code retour resultat
*
* CODE RETOUR :
*   XDC_OK		Succes
*
* CONDITION D'UTILISATION
*   Ce service est demande pour recevoir le TDP pour un PMVA et une destination donnee
*
* FONCTION
*     Armement CallBack et joindre le Datagroup d'entree
*
------------------------------------------------------*/
{
static char *version = "@(#)xzez.c	1.8 06/23/11 : XZEZ04_Dde_Valeur_TDP_PMVA ";
int 		vl_ValRet = XDC_OK;
T_BOOL		vl_EtatDg = FALSE;
XDY_Datagroup	vl_dg;

     XZST_03EcritureTrace ( XZSTC_FONCTION,
     "IN  : XZEZ01_Dde_Valeur_TDP DGInit:%s NumPMVA:%d Dest1_p:%d Dest1_l:%d Dest2_p=%d Dest2_l=%d. site = %d",
     		pa_DGReponse,
     		va_TDP.Numero,
     		va_TDP.Dest1_proche,
     		va_TDP.Dest1_loin,
			va_TDP.Dest2_proche,
			va_TDP.Dest2_loin,
			va_TDP.Site);

     /* Memorisation en globale du nom de la fonction utilisateur */
     pg_FonctUtil_XZEZ_04 = pa_FonctUtilisateur;

	/*A  Armement Callback sur Reception XDM_VAL_TDP */
	if ( vg_CB_XZEZ04 == NULL )
	{
	   if ( ( vg_CB_XZEZ04 = TipcSrvProcessCbCreate (	XDM_IdentMsg(XDM_VAL_TDP_PMVA),
							      	XZEZ_Dde_Valeur_TDP_PMVA,
								(T_CB_ARG)NULL ) ) == NULL )
	   {
	      /*B Ecriture Trace */
	      XZST_03EcritureTrace ( XZSTC_WARNING, "Echec armement Callback sur XDM_VAL_TDP_PMVA." );
	      vl_ValRet = XDC_NOK;
	   }
	   else
	   {
	      /*B Ecriture Trace */
	      XZST_03EcritureTrace ( XZSTC_DEBUG1, "Armement Callback sur XDM_VAL_TDP_PMVA." );
	   }
	}

	/*A Abonnement au Datagroup DGInit passe en parametre */
   	/*A Joindre ce datagroup s'il n'est pas deja joint */
/*RT35 obsolete   	TipcSrvDgGetRecv ( pa_DGReponse, &vl_EtatDg ); */
	TipcSrvSubjectGetSubscribe ( pa_DGReponse, &vl_EtatDg );
   	if ( ( !vl_EtatDg ) && ( pa_DGReponse != NULL ) )
   	{
   	   if ( !XDG_JoindreDG( pa_DGReponse ) )
   	   {
	      /*B Ecriture Trace */
      	      XZST_03EcritureTrace ( XZSTC_WARNING, "Echec abonnement au Datagroup %s ", pa_DGReponse );
	      vl_ValRet = XDC_NOK;
	   }
	   else
	   {
      	      XZST_03EcritureTrace ( XZSTC_DEBUG1, "Abonnement au Datagroup %s ", pa_DGReponse );
	   }
	}

           /*A Envoi du message de demande d'init XDM_ETDP_IHM en fournissant les valeur recues  */

	   sprintf(vl_dg,"%s_%d",XDG_EINIT_ABT_TDP,va_TDP.Site);
   	   if (!TipcSrvMsgWrite ( 	vl_dg,
      			  		XDM_IdentMsg(XDM_ETDP_IHM_PMVA),
  			  		XDM_FLG_SRVMSGWRITE,
                          		T_IPC_FT_INT2, va_TDP.Numero,
                          		T_IPC_FT_INT2, va_TDP.Dest1_proche,
                          		T_IPC_FT_INT2, va_TDP.Dest1_loin,
                          		T_IPC_FT_INT2, va_TDP.Dest2_proche,
								T_IPC_FT_INT2, va_TDP.Dest2_loin,
                          		T_IPC_FT_STR,  pa_DGReponse,
                          		/*T_IPC_FT_INT4, va_TDP.DestNoeud,*/
                          		NULL))
      	   {
  		/*A Trace envoi message XDM_ETDP_IHM non effectue */
         	XZST_03EcritureTrace(XZSTC_WARNING, "Echec envoi message XDM_ETDP_IHM_PMVA.");
	   }
	   else
      	   {
  		/*A Trace envoi message XDM_ETDP_IHM */
         	XZST_03EcritureTrace(XZSTC_DEBUG1, "Envoi du message XDM_ETDP_IHM_PMVA sur %s.", vl_dg );
	   }

   XZST_03EcritureTrace ( XZSTC_FONCTION, "OUT : XZEZ01_Dde_Valeur_TDP_PMVA Retour:%d.", vl_ValRet );
   *pa_Resultat = vl_ValRet;
   return ( vl_ValRet );

}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	XZEZ05_Calcul_TDP_PMVA (	XDY_Autoroute			va_autoroute_pmva,
				XDY_PR				va_pr_pmva,
				XDY_Sens			va_sens_pmva,
				XDY_Eqt				va_dest1_p,
				XDY_Eqt				va_dest1_l,
				XDY_Eqt				va_dest2_p,
				XDY_Eqt				va_dest2_l,
				XZEZT_Liste_Zdp	*		pa_ListeZone,
				XZEZT_Liste_Destinations *	pa_ListeDest,
				XDY_TDP *			pa_tdp )

/*
* ARGUMENTS EN ENTREE :
*
*
* ARGUMENTS EN SORTIE :
*
* int		pa_resultat		: Code retour resultat
*
* CODE RETOUR :
*   XDC_OK		Succes
*
* CONDITION D'UTILISATION
*   Ce service est demande pour recevoir le TDP pour un PMV et une destination donnee
*
* FONCTION
*
------------------------------------------------------*/
{
static char *version = "@(#)xzez.c	1.8 06/23/11 :  XZEZ02_Calcul_TDP ";
  int			vl_tdp_pmva=0;
  int			vl_tdp_dest1_p=0;
  int			vl_tdp_dest1_l=0;
  int			vl_tdp_dest2_p=0;
  int			vl_tdp_dest2_l=0;
  int			vl_tdp_zdp=0;
  int			vl_tdp_inter;
  XDY_Autoroute		vl_autoroute;
  int			vl_tdp_calcul=0;
  XDY_PR			vl_pr_destination=0;
  XDY_PR			vl_pr_deb=0,vl_pr_fin=0;
  XDY_Booleen			vl_Type_Loc=0;
  float 			vl_Coef = 0;
  XZEZT_Liste_Zdp *		pl_Zone;
  XZEZT_Liste_Destinations *	pl_Dest;
  XDY_Booleen			vl_tdpvalide=XDC_OK;

  XZST_03EcritureTrace(XZSTC_FONCTION,"IN : %s pour PMVA %d-%d-%ld  et dest:%d",
		  	  	  	  version, va_autoroute_pmva, va_sens_pmva, va_pr_pmva, va_dest1_p );

  /*DEM517 si dest=0 alors on retourne pa_tdp=-1 */
  if ( va_dest1_p == 0 )
    {
	*pa_tdp=-1;
	return (XDC_OK);
	}

  /* Recherche de la destination dans la liste */
  for ( pl_Dest = pa_ListeDest; pl_Dest != NULL; pl_Dest = pl_Dest->suivant )
    if ( pl_Dest->destination.Numero == va_dest1_p )
    {
       vl_pr_destination = pl_Dest->destination.PR;
       vl_Type_Loc 	 = pl_Dest->destination.Type_Localisation;
       vl_autoroute	 = pl_Dest->destination.Autoroute;
       //break;
    }

  	  if ( pl_Dest->destination.Numero == va_dest1_l )
      {
         vl_pr_destination = pl_Dest->destination.PR;
         vl_Type_Loc 	 = pl_Dest->destination.Type_Localisation;
         vl_autoroute	 = pl_Dest->destination.Autoroute;
         //break;
      }

  	  if ( pl_Dest->destination.Numero == va_dest2_p )
  	      {
  	         vl_pr_destination = pl_Dest->destination.PR;
  	         vl_Type_Loc 	 = pl_Dest->destination.Type_Localisation;
  	         vl_autoroute	 = pl_Dest->destination.Autoroute;
  	         //break;
  	      }

  		if ( pl_Dest->destination.Numero == va_dest2_l )
  	      {
  	         vl_pr_destination = pl_Dest->destination.PR;
  	         vl_Type_Loc 	 = pl_Dest->destination.Type_Localisation;
  	         vl_autoroute	 = pl_Dest->destination.Autoroute;
  	         //break;
  	      }

  /*DEM517 si dest non trouv�e alors on retourne pa_tdp=-1 */
  if ( pl_Dest->destination.Numero != va_dest1_l )
    {
	*pa_tdp=-1;
	return (XDC_OK);
	}

  XZST_03EcritureTrace(XZSTC_WARNING," %s PR dest:%d loc:%d", version, vl_pr_destination, vl_Type_Loc );
  vl_tdpvalide=XDC_OK;
  /*recherche de la zdp associee au pmv*/
  for ( pl_Zone = pa_ListeZone; pl_Zone != NULL; pl_Zone = pl_Zone->suivant )
  {
	if (pl_Zone->zdp.TDP_Surcharge !=0) {
		vl_tdp_calcul = pl_Zone->zdp.TDP_Surcharge;
		XZST_03EcritureTrace(XZSTC_FONCTION,"surcharge TDP %d",
					vl_tdp_calcul);
	}
	else
		vl_tdp_calcul = pl_Zone->zdp.TDP_Courant;

 	if (vl_tdp_calcul != 0)
	{
		/* si la Zdp encadre la sortie, il faut appliquer le ratio */
		/* PR destination compris entre PR debut et PR fin de la zone, et sur le meme autoroute */
		if ( pl_Zone->zdp.PR_Debut <= vl_pr_destination && pl_Zone->zdp.PR_Fin >= vl_pr_destination && pl_Zone->zdp.Autoroute == vl_autoroute )
			vl_Coef = (float)(vl_pr_destination- pl_Zone->zdp.PR_Debut) / (float) (pl_Zone->zdp.PR_Fin - pl_Zone->zdp.PR_Debut);
		/* Si la Zdp encadre le PMV, il faut aussi appliquer le ratio */
		else if ( pl_Zone->zdp.PR_Debut <= va_pr_pmva && pl_Zone->zdp.PR_Fin >= va_pr_pmva )
			vl_Coef = (float)(pl_Zone->zdp.PR_Fin - va_pr_pmva) / (float)(pl_Zone->zdp.PR_Fin - pl_Zone->zdp.PR_Debut);
		else
			vl_Coef = 1;

		vl_tdp_zdp += vl_tdp_calcul*vl_Coef;

		XZST_03EcritureTrace(XZSTC_FONCTION,"Donnee Zone PMVA : %ld a %ld TDP:%d val:%d",pl_Zone->zdp.PR_Debut, pl_Zone->zdp.PR_Fin,vl_tdp_calcul,vl_tdp_zdp );
	}
	else
	{
		vl_tdp_zdp += pl_Zone->zdp.TDP_Normal;
		XZST_03EcritureTrace(XZSTC_WARNING,"Pas de donnee Zone PMVA : %ld a %ld TDP:%d val:%d",
			pl_Zone->zdp.PR_Debut, pl_Zone->zdp.PR_Fin,vl_tdp_calcul,vl_tdp_zdp );
		vl_tdpvalide=XDC_NOK;
	}
	XZST_03EcritureTrace(XZSTC_DEBUG2,"Zone INTER : %ld a %ld TDP:%d", pl_Zone->zdp.PR_Debut, pl_Zone->zdp.PR_Fin,vl_tdp_calcul );
  }

  /*retourne le tdp total*/
  *pa_tdp=vl_tdp_pmva+vl_tdp_dest1_p+vl_tdp_dest1_l+vl_tdp_dest2_p+vl_tdp_dest2_l+vl_tdp_zdp;

  XZST_03EcritureTrace(XZSTC_INFO,"%s PMV : %d  DEST : %d ENTRE : %d", version, vl_tdp_pmva, vl_tdp_dest1_p, vl_tdp_zdp );
  /* Arrondi du TDP a la minute superieure */
  vl_tdp_inter=vl_tdp_pmva+vl_tdp_dest1_p+vl_tdp_dest1_l+vl_tdp_dest2_p+vl_tdp_dest2_l+vl_tdp_zdp;
  vl_tdp_inter = vl_tdp_inter / 60;

  *pa_tdp = (XDY_TDP)vl_tdp_inter + 1;

  /* retourne un tdp negatif si une donn�e a �t� nulle */
/*  if ( vl_tdpvalide == XDC_NOK ) {
	*pa_tdp = -(*pa_tdp);
	}*/

  XZST_03EcritureTrace(XZSTC_FONCTION,"OUT: %s pour : Dest : %d Tdp : %d", version, va_dest1_p, *pa_tdp );
  return (XDC_OK);

}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	XZEZ06_CompteRendu_TDP_PMVA (	XDY_Entier	va_Num_Action,
					XDY_Booleen  	va_ResultatAction,
					XDY_Eqt		va_NumPMVA,
					XDY_NomMachine	va_NomMachine )

/*
* ARGUMENTS EN ENTREE :
* va_Num_Action		: Numero action affichage TDP
* va_ResultatAction	: Resultat action (OK ou NOK)
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR :
*   XDC_OK		Succes
*
* CONDITION D'UTILISATION
*   Ce service est demande pour envoyer le compte rendu d'un affichage TDP
*
* FONCTION
*
------------------------------------------------------*/
{
static char *version = "@(#)xzez.c	1.8 06/23/11 : XZEZ03_CompteRendu_TDP ";
int 		vl_ValRet = XDC_OK;
XDY_Datagroup   pl_NomDG;

     XZST_03EcritureTrace ( XZSTC_FONCTION,
     "IN  : XZEZ06_CompteRendu_TDP_PMVA Action:%d CR:%d PMV:%d.", va_Num_Action, va_ResultatAction, va_NumPMVA );


     /*A Envoi du message de demande d'init XDM_CR_TDP en fournissant les valeur recues  */
     /*A Construction du DatGroup XDG_ETDP_PMV_<NomMachine> */
     XDG_EncodeDG2 ( pl_NomDG, XDG_ETDP_PMV, va_NomMachine ) ;
     if (!TipcSrvMsgWrite ( 	pl_NomDG,
      			  	XDM_IdentMsg(XDM_CR_TDP_PMVA),
  			  	XDM_FLG_SRVMSGWRITE,
                          	T_IPC_FT_INT4, va_Num_Action,
                          	T_IPC_FT_INT2, va_ResultatAction,
                          	T_IPC_FT_INT2, va_NumPMVA,
                          	NULL))
     {
         /*A Trace envoi message XDM_CR_TDP non effectue */
         XZST_03EcritureTrace(XZSTC_WARNING, "Echec envoi message XDM_CR_TDP.");
         vl_ValRet = XDC_NOK;
     }
     else
     {
  	 /*A Trace envoi message XDM_CR_TDP */
         XZST_03EcritureTrace(XZSTC_MESSAGE, "Envoi du message XDM_CR_TDP sur %s.", pl_NomDG );
         vl_ValRet = XDC_OK;
     }

   XZST_03EcritureTrace ( XZSTC_FONCTION, "OUT : XZEZ06_CompteRendu_TDP_PMVA Retour:%d.", vl_ValRet );
   return ( vl_ValRet );

}




/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	XZEZ10_Calcul_Traversee (	XDY_Autoroute			va_autoroute_pmv,
				XDY_PR				va_pr_pmv,
				XDY_Sens			va_sens_pmv,
				XDY_PR				va_pr_min_bouchon,
				XDY_PR				va_pr_max_bouchon,
				XZEZT_Liste_Zdp	*		pa_ListeZone,
				XZEZT_Liste_Zdp	*		pa_ListeZoneData, 
				XDY_TDP *			pa_tdp)

/*
* ARGUMENTS EN ENTREE :
*
*
* ARGUMENTS EN SORTIE :
*
* int		pa_resultat		: Code retour resultat
*
* CODE RETOUR : 
*   XDC_OK		Succes
*
* CONDITION D'UTILISATION
*   Ce service est demande pour recevoir le TDP pour un PMV et une destination donnee
*
* FONCTION 
*
------------------------------------------------------*/
{
static char *version = "@(#)xzez.c      1.8 06/23/11 :  XZEZ02_Calcul_TDP ";
  float                 vl_tdp_pmv=0;
  float                 vl_tdp_dest=0;
  float                 vl_tdp_zdp=0;
  float                 vl_tdp_inter;
  float			vl_tdp_calcul=0;
  XDY_Autoroute         vl_autoroute;
XDY_PR                        vl_pr_deb=0,vl_pr_fin=0;
  XDY_PR                        vl_pr;
  XZEZT_Liste_Zdp *             pl_Zone;
  XZEZT_Liste_Zdp *             pl_ZoneData;
  XDY_Booleen                   vl_tdpvalide=XDC_OK;
  float vl_mon_tdp;
  float vl_mon_ttr;
  float vl_ttr;
  XDY_PR		vl_pr_min_zone, vl_pr_max_zone;
  float vl_ratio;
  XDY_PR		vl_pr_min;
  XDY_PR		vl_pr_max;

  XZST_03EcritureTrace(XZSTC_FONCTION,"IN : %s pour PMV %d-%d-%ld  et bouchon %d-%d",
		version, va_autoroute_pmv, va_sens_pmv, va_pr_pmv, va_pr_min_bouchon, va_pr_max_bouchon);

  vl_tdpvalide=XDC_OK;
  for ( pl_Zone = pa_ListeZone; pl_Zone != NULL; pl_Zone = pl_Zone->suivant )
  {
         /*recup du TDP*/
        for ( pl_ZoneData = pa_ListeZoneData; pl_ZoneData != NULL; pl_ZoneData = pl_ZoneData->suivant ) {
                if (pl_ZoneData->zdp.Code_Zone == pl_Zone->zdp.Code_Zone)
                        break;
        }
        if (pl_ZoneData==NULL) {
          *pa_tdp=-1;
 	  vl_tdpvalide=XDC_NOK;
          return XDC_OK;
        }

	if (pl_ZoneData->zdp.TDP_Surcharge != 0) {
		vl_tdp_calcul = pl_ZoneData->zdp.TDP_Surcharge;
		XZST_03EcritureTrace(XZSTC_FONCTION,"surcharge TDP %f",vl_tdp_calcul);
        }
	else
		vl_tdp_calcul =  pl_ZoneData->zdp.TDP_Courant;

	vl_mon_tdp = pl_ZoneData->zdp.TDP_Normal;
	
	  vl_pr_min_zone = pl_Zone->zdp.PR_Debut <= pl_Zone->zdp.PR_Fin ? pl_Zone->zdp.PR_Debut : pl_Zone->zdp.PR_Fin;
	  vl_pr_max_zone = pl_Zone->zdp.PR_Debut >= pl_Zone->zdp.PR_Fin ? pl_Zone->zdp.PR_Debut : pl_Zone->zdp.PR_Fin;

	  XZST_03EcritureTrace(XZSTC_FONCTION,"min zone %d, max zone %d",vl_pr_min_zone, vl_pr_max_zone);

	  /*si le bouchon n'est pas dans la zone*/
	  if ((va_pr_min_bouchon > vl_pr_max_zone) || (va_pr_max_bouchon < vl_pr_min_zone)) {
	    XZST_03EcritureTrace(XZSTC_FONCTION,"CAS : zone non concernee par le bouchon");
	  }
	  else {
	    if (vl_tdp_calcul==0) {
	     vl_mon_tdp = pl_Zone->zdp.TDP_Normal;
	     XZST_03EcritureTrace(XZSTC_WARNING,"Pas de donnee Zone PMV : %ld a %ld TDP:%f ",
	     	pl_Zone->zdp.PR_Debut, pl_Zone->zdp.PR_Fin,vl_mon_tdp);
		vl_tdpvalide=XDC_NOK;
	    }

	    /*calcul de la portion de la zone en bouchon*/
	    vl_pr_min = vl_pr_min_zone <= va_pr_min_bouchon ? va_pr_min_bouchon : vl_pr_min_zone;
	    vl_pr_max = vl_pr_max_zone <= va_pr_max_bouchon ? vl_pr_max_zone : va_pr_max_bouchon;
	    vl_ratio = (float)(vl_pr_max - vl_pr_min)/(float)(vl_pr_max_zone - vl_pr_min_zone);
	    XZST_03EcritureTrace(XZSTC_FONCTION,"min %d / max %d / ratio %f",vl_pr_min,vl_pr_max,vl_ratio);

	    vl_ttr = vl_tdp_calcul - (1-vl_ratio) * pl_Zone->zdp.TDP_Normal;

	    XZST_03EcritureTrace(XZSTC_FONCTION,"TTR de la zone %f", vl_ttr);
	    vl_mon_ttr += vl_ttr;
	  
	}
  }
  vl_mon_ttr = vl_mon_ttr / 60;
  *pa_tdp = (XDY_TDP)vl_mon_ttr + 1;

  /* retourne un tdp negatif si une donnée a été nulle */
  if ( vl_tdpvalide == XDC_NOK ) {
	  *pa_tdp = -(*pa_tdp);
  }

  XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : TTR = %d",*pa_tdp);
  return (XDC_OK);

}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	XZEZ11_Calcul_Traversee_Bouchon (XDY_Autoroute			va_autoroute,
					XDY_Sens			va_sens,
					XDY_PR				va_pr_min_bouchon,
					XDY_PR				va_pr_max_bouchon,
					XZEZT_Liste_Zdp	*		pa_ListeZone,
					XZEZT_Liste_Zdp	*		pa_ListeZoneData, 
					XDY_TDP *			pa_tdp)

/*
* ARGUMENTS EN ENTREE :
*
*
* ARGUMENTS EN SORTIE :
*
* int		pa_resultat		: Code retour resultat
*
* CODE RETOUR : 
*   XDC_OK		Succes
*
* CONDITION D'UTILISATION
*   Ce service est demande pour recevoir le TDP pour un PMV et une destination donnee
*
* FONCTION 
*
------------------------------------------------------*/
{
static char *version = "@(#)xzez.c      1.8 06/23/11 :  XZEZ02_Calcul_TDP ";
  float                 vl_tdp_pmv=0;
  float                 vl_tdp_dest=0;
  float                 vl_tdp_zdp=0;
  float                 vl_tdp_inter;
  XDY_Autoroute         vl_autoroute;
  float			vl_tdp_calcul=0;
XDY_PR                  vl_pr_deb=0,vl_pr_fin=0;
  XDY_PR                vl_pr;
  XZEZT_Liste_Zdp *     pl_Zone;
  XZEZT_Liste_Zdp *     pl_ZoneData;
  XDY_Booleen           vl_tdpvalide=XDC_OK;
  float 		vl_mon_tdp;
  float 		vl_mon_ttr;
  float 		vl_ttr;
  XDY_PR		vl_pr_min_zone, vl_pr_max_zone;
  float 		vl_ratio;
  XDY_PR		vl_pr_min;
  XDY_PR		vl_pr_max;

  XZST_03EcritureTrace(XZSTC_FONCTION,"IN : %s pour %d-%d  et bouchon %d-%d",
		version, va_autoroute, va_sens, va_pr_min_bouchon, va_pr_max_bouchon);

  vl_tdpvalide=XDC_OK;
  for ( pl_Zone = pa_ListeZone; pl_Zone != NULL; pl_Zone = pl_Zone->suivant )
  {
         /*recup du TDP*/
        for ( pl_ZoneData = pa_ListeZoneData; pl_ZoneData != NULL; pl_ZoneData = pl_ZoneData->suivant ) {
                if (pl_ZoneData->zdp.Code_Zone == pl_Zone->zdp.Code_Zone)
                        break;
        }
        if (pl_ZoneData==NULL) {
          *pa_tdp=-1;
	  vl_tdpvalide=XDC_NOK;
          return XDC_OK;
        }

	if (pl_ZoneData->zdp.TDP_Surcharge !=0) {
		vl_tdp_calcul = pl_ZoneData->zdp.TDP_Surcharge;
		XZST_03EcritureTrace(XZSTC_FONCTION,"surcharge TDP %f",vl_tdp_calcul);
	}
	else
		vl_tdp_calcul = pl_ZoneData->zdp.TDP_Courant;

	vl_mon_tdp = pl_ZoneData->zdp.TDP_Normal;
	
	  vl_pr_min_zone = pl_Zone->zdp.PR_Debut <= pl_Zone->zdp.PR_Fin ? pl_Zone->zdp.PR_Debut : pl_Zone->zdp.PR_Fin;
	  vl_pr_max_zone = pl_Zone->zdp.PR_Debut >= pl_Zone->zdp.PR_Fin ? pl_Zone->zdp.PR_Debut : pl_Zone->zdp.PR_Fin;

	  XZST_03EcritureTrace(XZSTC_FONCTION,"min zone %d, max zone %d",vl_pr_min_zone, vl_pr_max_zone);

	  /*si le bouchon n'est pas dans la zone*/
	  if ((va_pr_min_bouchon > vl_pr_max_zone) || (va_pr_max_bouchon < vl_pr_min_zone) ||
	      (va_autoroute != pl_Zone->zdp.Autoroute) ||
	      (va_sens != pl_Zone->zdp.Sens)) {
	    XZST_03EcritureTrace(XZSTC_FONCTION,"CAS : zone non concernee par le bouchon");
	  }
	  else {
	    if (vl_tdp_calcul==0) {
	     vl_mon_tdp = pl_Zone->zdp.TDP_Normal;
	     XZST_03EcritureTrace(XZSTC_WARNING,"Pas de donnee Zone PMV : %ld a %ld TDP:%f ",
	     	pl_Zone->zdp.PR_Debut, pl_Zone->zdp.PR_Fin,vl_mon_tdp);
		vl_tdpvalide=XDC_NOK;
	    }

	    /*calcul de la portion de la zone en bouchon*/
	    vl_pr_min = vl_pr_min_zone <= va_pr_min_bouchon ? va_pr_min_bouchon : vl_pr_min_zone;
	    vl_pr_max = vl_pr_max_zone <= va_pr_max_bouchon ? vl_pr_max_zone : va_pr_max_bouchon;
	    vl_ratio = (float)(vl_pr_max - vl_pr_min)/(float)(vl_pr_max_zone - vl_pr_min_zone);
	    XZST_03EcritureTrace(XZSTC_FONCTION,"min %d / max %d / ratio %f",vl_pr_min,vl_pr_max,vl_ratio);

	    vl_ttr = vl_tdp_calcul - (1-vl_ratio) * pl_Zone->zdp.TDP_Normal;

	    XZST_03EcritureTrace(XZSTC_FONCTION,"TTR de la zone %f", vl_ttr);
	    if (vl_ttr < 0)
	    	XZST_03EcritureTrace(XZSTC_FONCTION,"TDP_Courant %f %f",vl_tdp_calcul, pl_Zone->zdp.TDP_Normal);
	    vl_mon_ttr += vl_ttr;
	  
	}
  }
  vl_mon_ttr = vl_mon_ttr / 60;
  *pa_tdp = (XDY_TDP)vl_mon_ttr + 1;

  /* retourne un tdp negatif si une donnée a été nulle */
  if ( vl_tdpvalide == XDC_NOK ) {
	  *pa_tdp = -(*pa_tdp);
  }

  XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : TTR = %d",*pa_tdp);
  return (XDC_OK);

}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZEZ12_Envoi_TDP_FCD (  XDY_Horodate	va_Horodate,
				XDY_PR	        va_PR_debut,
				XDY_PR	        va_PR_fin,
				XDY_Sens	va_Sens,
				XDY_TDP	        va_Tdp,
				XDY_Octet	va_Validite,
				XDY_NomAuto	va_Autoroute)

/*
* ARGUMENTS EN ENTREE :
*va_Horodate :	Horodate donne
*va_PR_debut :	PR de debut
*va_PR_fin :	PR de fin
*va_Sens:   	Sens
*va_Tdp :   	Valeur TDP
*va_Validite :	Indicateur validite
*va_Autoroute :	Numero autoroute
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
*   XDC_OK      Succes
*   XDC_NOK     Echec
*
* CONDITION D'UTILISATION
*   Envoie le message EMSG_TDP_FCD sur le datagroup XDG_ETDP_FCD pour les temps de parcours FCD
*
* FONCTION 
*
------------------------------------------------------*/
{
static char *version = "@(#)xzez.c	1.8 06/23/11 : XZEZ_12Envoi_TDP_FCD ";
int 		vl_ValRet = XDC_OK;

     XZST_03EcritureTrace ( XZSTC_FONCTION, 
     "IN  : XZEZ_12Envoi_TDP_FCD Horodate = %f\tPR1 = %d\tPR2 = %d\tSens = %d\tTdp = %d\tValidite = %d\tAutoroute = %s\n",
			va_Horodate,
			va_PR_debut,
			va_PR_fin,
			va_Sens,
			va_Tdp,
			va_Validite,
			va_Autoroute);
     		

     /*A Envoi du message de demande d'init XDM_CR_TDP en fournissant les valeur recues  */
     
     if (!TipcSrvMsgWrite ( 	XDG_ETDP_FCD,
      			  	XDM_IdentMsg(EMSG_TDP_FCD),
  				XDM_FLG_SRVMSGWRITE,
				T_IPC_FT_REAL8, va_Horodate,
				T_IPC_FT_INT4, va_PR_debut,
				T_IPC_FT_INT4, va_PR_fin,
				T_IPC_FT_INT2, va_Sens,
				T_IPC_FT_INT4, va_Tdp,
				T_IPC_FT_INT2, va_Validite,
				T_IPC_FT_STR,  va_Autoroute,
                          	NULL))
     {
         /*A Trace envoi message EMSG_TDP_FCD non effectue */    	
         XZST_03EcritureTrace(XZSTC_WARNING, "Echec envoi message EMSG_TDP_FCD.");
         vl_ValRet = XDC_NOK;
     }
     else
     {
       /*A Trace envoi message EMSG_TDP_FCD */    	
         XZST_03EcritureTrace(XZSTC_MESSAGE, "Envoi du message EMSG_TDP_FCD sur %s.", XDG_ETDP_FCD );
         vl_ValRet = XDC_OK;
     }
     
   XZST_03EcritureTrace ( XZSTC_FONCTION, "OUT : XZEZ12_Envoi_TDP_FCD Retour:%d.", vl_ValRet );
   return ( vl_ValRet );
}	
