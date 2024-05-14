/* Fichier : erdt_cyc.c
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TERDT * FICHIER erdt_cyc.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  < Module de gestion cyclique de la tache TERDT >
*
------------------------------------------------------
* HISTORIQUE :
*
* Volcic.F	version 1.1	08 Dec 1994	: Creation
* Volcic.F	version 1.2	25 Jan 1995	: Modif Trace 
* Volcic.F	version 1.3	13 Fev 1995	: 
* Volcic.F	version 1.4	20 Mar 1995	: Modif Nom Cstes TIMER 
* Volcic.F	version 1.5	21 Avr 1995	: Reprise entete, historique
* Volcic.F	version 1.6	07 Jun 1995	: Ajout traitement Mode Lent
* Volcic.F	version 1.7	07 Jun 1995	: Ajout Timer 30 Min
* Volcic.F	version 1.8	06 Jui 1995	: Mise a jour commentaires
* Volcic.F	version 1.9	12 Nov 1995	: Modif ConfRADT.Fd
* Volcic.F	version 1.13	23 Jan 1996	: Modif Cyclique 1min et 6min
* Mismer.D	version 1.14	15 Fev 1996	: Remplacement de MI 0 par MI 1
* Mismer.D	version 1.15	05 Mar 1996	: Ajout limite taille fichier Erreur_trame et config voie (DEM/1050)
* Volcic.F	version 1.16	06 Sep 1996	: Modif Extension Ouest (RADT_W)
* Mismer.D	version 1.17	20 Sep 1996	: Modif Extension Ouest Suite (RADT_W)
* Mismer.D	version 1.18	04 Sep 1996	: Modif pour mesures pesage (DEM/1227)
* Mismer.D	version 1.19	12 Dec 1996	: Calcul des niveaux de services pourstations virtuelles (DEM/1338)
* Mismer.D      version 1.20    25 Mar 1997     : Modif date pour TDP (DEM/1396)
* JMG		v1.22		29/01/2008	: Modif pour site de gestion DEM748
* JPL		1.23		23/11/09	: Changement de niveaux de traces, notamment "Warning" en "Info" (DEM 908)
* JPL		1.24		17/03/11	: Migration architecture HP ia64 (DEM 975) : suppression warnings mineurs
* JMG		1/25		04/07/12	: simulation DEM/1016
* JMG		1.26		31/10/18	: ajout IP DEM1306 
 * JMG 		1.27		07/03/20 	: EMS 
* PNI		1.28		08/12/20 	: time remplacer par XZSM_07 SAE_191
* JPL		1.30		25/07/22	: Commande de datation quotidienne (SAE_399); initialisation des donnees
* JPL		1.31		15/09/22	: Commande de date "normale" (sera sans ID) pour stations IP (SAE_399)
------------------------------------------------------*/

/* fichiers inclus */

#include "ex_mrtf.h"

#include "erdt_cyc.h"
#include "erdt_cmd.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */
extern XDY_Booleen vg_mode_simu;

/* declaration de variables locales */

static char *version = "erdt_cyc.c 1.31" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int er_traitement_cyclique ( int va_NumSock )

/*
* ARGUMENTS EN ENTREE :
*   < Valeur et descriptif des parametres en entree >
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR : 
*   <Valeur de retour et un commentaire explicatif >
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   < Bref descriptif fonctionnel >
*
------------------------------------------------------*/
{
   XDY_Datagroup   	pl_DG_Poids;
   int			vl_IndCnx = 0;
   int			vl_LgMsg  = 0;
   XZEXT_MSG_SOCKET	pl_Msg;
   XZEXT_MSG_SOCKET	pl_MsgTimer;
   XDY_Horodate		vl_Horodate;
//   XZSMT_Horodate	vl_Horodatedecompo;
   int			vl_Delai;
   int			vl_Num_RADT;
   int			vl_Sens;
   XZEXT_CLE_PROG	pl_CleProg;
   ERDT_DONNEES_RADT 	*pl_listeRADT;
   XDY_EMes_Poids	vl_Mes_Poids;
   XZEXT_Trame		vl_tlcr;
   int			vl_noeqt = 0;
   XZSCT_NomSite 	pl_NomSite;
   XZSCT_NomMachine 	vl_NomMachine = "";
   time_t		vl_time = 0;
   int			vl_jourSemaine	= 0;
   XDY_Horodate		vl_horodateSec	= 0.0;
   XZSMT_Horodate	vl_heure_string;
   char			vl_Cas = 0;
   XDY_Entier		vl_retcode = 0;
   FILE                 *pl_Fd;
   char                 pl_PathEtFic[50] = "";
   char                 pl_PathEtFicOld[50] = "";
   long			vl_TailleFic = 0;
   char            	pl_Texte[20] = "";
   int 			vl_jour = 0, vl_mois = 0, vl_indexJour, vl_indexMois;
   int vl_i;

   XZST_03EcritureTrace( XZSTC_FONCTION, "IN er_traitement_cyclique: %s", version );
   strcpy (pl_CleProg, "");
   
   /*A  Lecture du message recu dans la socket appelante  */
   if ( ex_lire_msg ( va_NumSock, pl_Msg, &vl_LgMsg, &vg_SockMask, &vl_IndCnx ) != XDC_OK )
   {
      XZST_03EcritureTrace( XZSTC_WARNING, "er_traitement_cyclique : Erreur lecture message socket. " );
   }   
   else
   {
      /*A Si le message est correct et si ce n'est pas une deconnexion */
      if ( ( vl_LgMsg != 0 ) && ( vl_IndCnx != XZEXC_DECONNEC ) )
      {
  	  /*A S'il s'agit d'un reveil cyclique */
  	  if ( strstr ( pl_Msg, XZEXC_MSG_REVEIL ) != NULL )
  	  {
  	     sscanf ( &pl_Msg[strlen(XZEXC_MSG_REVEIL)], " %lf %d %s ",
  	  					         &vl_Horodate,
  	  					         &vl_Delai,
  	  					         pl_CleProg );
  	     
  	     if (strstr(pl_CleProg,"RADT_FLcr_NoEqt:")!=NULL)
  	     { 
  	        sscanf(pl_CleProg,"RADT_FLcr_NoEqt:%c%d",&vl_Cas,&vl_noeqt);
  	        
                ex_relance_lcr  ( 	vl_noeqt,
                			vl_Cas,
			 		&pg_DebutListeRADTFLcr,
			 		vl_Horodate, 
			 		pl_CleProg,
			 		va_NumSock );
	        return (XDC_OK);
  	     }
  	     
  	     /*B Recherche de la date en cours */
             
             /*time ( &vl_time );
   		vl_horodateSec = (XDY_Horodate) vl_time;  remplacer par XZSM_07 XZAE_191*/
   	     XZSM_07LireHorodate( XZSMC_TYPE_SYSTEME, &vl_i, &vl_horodateSec );      
  	     switch ( vl_Delai )
  	     {
  	         /*A  Cas ou periode Timer =  30s */
  	         case ERDTC_REVEIL_30S	 :
  	         
		    if (vg_mode_simu)
			return (XDC_OK);
  	            XZST_03EcritureTrace( XZSTC_INFO, "%s", pl_CleProg );
  	            
  	            /*B Pour les RADT de la machine => demande mesures individuelles si la RADT n'est pas HS et pas de mode lent */
                    if ( vg_Mode_Lent == XDC_FAUX )
                    {
                       for ( pl_listeRADT = pg_debutRADT; pl_listeRADT != NULL; pl_listeRADT = pl_listeRADT->Suivant )
                       {
                          if ( !(pl_listeRADT->Etat_Service) && 
                                (pl_listeRADT->EtatTrans != ERDTC_EQT_PANNE) && 
                                (pl_listeRADT->Don_HisRADT.Demande_Histo != XDC_VRAI) &&
                                (pl_listeRADT->ConfRADT.Liaison > 0) )
                          {
                              if ( vg_Demande_Individuelle[pg_Tab_RADT[pl_listeRADT->ConfRADT.NumeroStation]] == XDC_VRAI )
			      {
				   if (!strcmp(pl_listeRADT->ConfRADT.AdresseIP, XZECC_IP_VIDE)) {
				   	if ( ( vl_retcode = er_EnvoyerTrame(	"MI 1",
                                	   				XZEXC_TRAME_CLASSE_DIV,
                                   					pl_listeRADT->ConfRADT.RGS,
                                   					XZEXC_NOCMD,
	                           					XZEXC_PRIORITEFAIBLE,
                                   					pl_listeRADT->Socket ) ) != XDC_OK )
                           	   	{
                            			XZST_03EcritureTrace( XZSTC_WARNING,"er_traitement_cyclique : EnvoyerTrame retourne %d", vl_retcode );
  	                   	   	}	
			      	   }
				   else {
					EnvoyerTrameIP( "MI 1",
							XZEXC_TRAME_CLASSE_DIV,
							pl_listeRADT->ConfRADT.AdresseIP,
							pl_listeRADT->ConfRADT.Port,
							pl_listeRADT->ConfRADT.RGS,
							XZEXC_NOCMD,
							XZEXC_PRIORITEFAIBLE,
							pl_listeRADT->Socket );
				   }
			      }
  	                  } 
  	               }  
  	            }
  	            break;
  	         
  	         /*A Cas ou periode Timer = 1mn */
  	         case ERDTC_REVEIL_1MN	 :
  	         
		    if (vg_mode_simu)
			return (XDC_OK);

  	            XZST_03EcritureTrace( XZSTC_INFO, "pl_CleProg %s", pl_CleProg );
  	            
                    /*B Pour tous les RADT de la machine => demande mesure 1mn %PL et demande mesure 1mn Q V TO si pas de mode lent */
                    if (( vg_Mode_Lent == XDC_FAUX ) && ( !vg_Mac_Ext_Ouest ))
                    {
                       for ( pl_listeRADT = pg_debutRADT; pl_listeRADT != NULL; pl_listeRADT = pl_listeRADT->Suivant )
                       {
  	                  XZST_03EcritureTrace( XZSTC_DEBUG3, "Timer 1 MIN %d : %d : %d", pl_listeRADT->Etat_Service, pl_listeRADT->EtatTrans,pl_listeRADT->Don_HisRADT.Demande_Histo );
                          /*B demande mesure 1mn Q V TO si la RADT n'est pas HS */ 
                          if ( !(pl_listeRADT->Etat_Service & XDC_EQT_HORS_SRV ) && 
                                (pl_listeRADT->Don_HisRADT.Demande_Histo != XDC_VRAI)  &&
                                (pl_listeRADT->ConfRADT.Liaison > 0) &&
				(pl_listeRADT->ConfRADT.Type!=0) )
				/*
				((pl_listeRADT->ConfRADT.NumeroStation<212) || (pl_listeRADT->ConfRADT.NumeroStation>223)) &&
				((pl_listeRADT->ConfRADT.NumeroStation<230) || (pl_listeRADT->ConfRADT.NumeroStation>232)) )*/
                          {
			      if (!strcmp(pl_listeRADT->ConfRADT.AdresseIP, XZECC_IP_VIDE)) {
                              	if ( ( vl_retcode = er_EnvoyerTrame(	"Mm",
                                   					XZEXC_TRAME_MES_1MN_Q_V_TO,
                                   					pl_listeRADT->ConfRADT.RGS,
                                   					XZEXC_NOCMD,
	                           					XZEXC_PRIORITEFAIBLE,
                                   					pl_listeRADT->Socket ) ) != XDC_OK )
                              {
                            	   XZST_03EcritureTrace( XZSTC_WARNING,"er_traitement_cyclique : EnvoyerTrame retourne %d", vl_retcode );
  	                      }
			     }
				   else {
					EnvoyerTrameIP( "Mm",
							XZEXC_TRAME_MES_1MN_Q_V_TO,
							pl_listeRADT->ConfRADT.AdresseIP,
							pl_listeRADT->ConfRADT.Port,
							pl_listeRADT->ConfRADT.RGS,
							XZEXC_NOCMD,
							XZEXC_PRIORITEFAIBLE,
							pl_listeRADT->Socket );
				   }
  	                   } 
                       }
                    }     
  	            break;
  	        
  	         /*A Cas ou periode Timer = 6mn */
  	         case ERDTC_REVEIL_6MN	 :
  	            XZST_03EcritureTrace( XZSTC_WARNING, "ERDTC_REVEIL_6MN pl_CleProg %s", pl_CleProg );
  	           
			if (vg_mode_simu) {
				 XZST_03EcritureTrace( XZSTC_INFO, "mode SIMU");
				 XZSM_07LireHorodate( XZSMC_TYPE_SYSTEME, &vl_i, &vl_horodateSec );
				       er_lit_fichier_simu(vl_horodateSec);
				return (XDC_OK);
			}

  	            /*B Pour tous les RADT de la machine => demande mesure 6mn %PL et demande mesure 6mn Q V TO si la RADT n'est pas HS */
                    for ( pl_listeRADT = pg_debutRADT; pl_listeRADT != NULL; pl_listeRADT = pl_listeRADT->Suivant )
                    {
                        /*B demande mesure 6mn %PL et demande mesure 6mn Q V TO */ 
                        if ( !(pl_listeRADT->Etat_Service & XDC_EQT_HORS_SRV ) && (pl_listeRADT->ConfRADT.Liaison > 0) )
                        { 
                           if  ( pl_listeRADT->Don_HisRADT.Demande_Histo != XDC_VRAI )
                           {
                              /*B demande mesure 6mn PL si le type de station n'est pas de type echangeur */ 
                              if ( pl_listeRADT->ConfRADT.TypeStation != ERDTC_ECHANGEUR )
				 if (!strcmp(pl_listeRADT->ConfRADT.AdresseIP, XZECC_IP_VIDE)) {
                                	 if ( ( vl_retcode = er_EnvoyerTrame(	"MB LC",
                                   					XZEXC_TRAME_MES_6MN_PL,
                                   					pl_listeRADT->ConfRADT.RGS,
                                   					XZEXC_NOCMD,
	                           					XZEXC_PRIORITEFAIBLE,
                                   					pl_listeRADT->Socket ) ) != XDC_OK )
                                 {
                            	    XZST_03EcritureTrace( XZSTC_WARNING,"er_traitement_cyclique : EnvoyerTrame retourne %d", vl_retcode );
  	                         }
                    		} 
				   else {
					EnvoyerTrameIP( "MB LC",
							XZEXC_TRAME_MES_6MN_PL,
							pl_listeRADT->ConfRADT.AdresseIP,
							pl_listeRADT->ConfRADT.Port,
							pl_listeRADT->ConfRADT.RGS,
							XZEXC_NOCMD,
							XZEXC_PRIORITEFAIBLE,
							pl_listeRADT->Socket );
				   }
                               sleep ( 1 );
                       
                               /*B demande mesure 6mn Q V TO */ 
			        if (!strcmp(pl_listeRADT->ConfRADT.AdresseIP, XZECC_IP_VIDE)) {
                               if ( ( vl_retcode = er_EnvoyerTrame( "MB",
                                   				    XZEXC_TRAME_MES_6MN_Q_V_TO,
                                   				    pl_listeRADT->ConfRADT.RGS,
                                   				    XZEXC_NOCMD,
	                           			  	    XZEXC_PRIORITEFAIBLE,
                                   				    pl_listeRADT->Socket ) ) != XDC_OK )
                               {
                                   XZST_03EcritureTrace( XZSTC_WARNING,"er_traitement_cyclique : EnvoyerTrame retourne %d", vl_retcode );
  	                       }
  	                      	} 
				   else {
					EnvoyerTrameIP( "MB",
							XZEXC_TRAME_MES_6MN_Q_V_TO,
							pl_listeRADT->ConfRADT.AdresseIP,
							pl_listeRADT->ConfRADT.Port,
							pl_listeRADT->ConfRADT.RGS,
							XZEXC_NOCMD,
							XZEXC_PRIORITEFAIBLE,
							pl_listeRADT->Socket );
				   }

                               if ( pl_listeRADT->ConfRADT.CapteurPoids )
                               {
                                   if ( (pl_listeRADT->Don_HisSilhouette.Horodate_Histo + 480) < vg_Date_Actuelle )
                                   {
  		                     pl_listeRADT->Don_HisSilhouette.Demande_Histo = 1 + (vg_Date_Actuelle  - pl_listeRADT->Don_HisSilhouette.Horodate_Histo) / 3600;
                                     /*B demande mesure pesage silhouette MH KC */ 
                                     if ( pl_listeRADT->Don_HisSilhouette.Demande_Histo > 1 ) 
                                        sprintf ( pl_Texte, "MH %d KC\0", pl_listeRADT->Don_HisSilhouette.Demande_Histo );
                                     else
                                        strcpy ( pl_Texte, "MH KC\0" );
                                     XZST_03EcritureTrace( XZSTC_INFO,"Trame pesage : <%s> station : %d", pl_Texte, pl_listeRADT->ConfRADT.NumeroStation );
				     if (!strcmp(pl_listeRADT->ConfRADT.AdresseIP, XZECC_IP_VIDE)) {
                                     	er_EnvoyerTrame( 	pl_Texte,
                                   		      	XZEXC_TRAME_PESAGE_SILHOUETTE,
                                   		      	pl_listeRADT->ConfRADT.RGS,
                                   			XZEXC_NOCMD,
	                           			XZEXC_PRIORITEFAIBLE,
                                   			pl_listeRADT->Socket );
                                      } 
				   else {
					EnvoyerTrameIP( pl_Texte,
							XZEXC_TRAME_PESAGE_SILHOUETTE,
							pl_listeRADT->ConfRADT.AdresseIP,
							pl_listeRADT->ConfRADT.Port,
							pl_listeRADT->ConfRADT.RGS,
							XZEXC_NOCMD,
							XZEXC_PRIORITEFAIBLE,
							pl_listeRADT->Socket );
				   }
                                   }
                                   else
                                      er_Recopie_Fichier_Mesure_Pesage ( pl_listeRADT->ConfRADT.NumeroStation, XDC_MESURE_SILHOUETTE );

                                   if ( (pl_listeRADT->Don_HisEssieu.Horodate_Histo + 480) < vg_Date_Actuelle )
                                   {
  		                      pl_listeRADT->Don_HisEssieu.Demande_Histo = 1 + (vg_Date_Actuelle  - pl_listeRADT->Don_HisEssieu.Horodate_Histo) / 3600;
                                     /*B demande mesure pesage Nb Essieu MH EC */ 
                                     if ( pl_listeRADT->Don_HisEssieu.Demande_Histo > 1 ) 
                                        sprintf ( pl_Texte, "MH %d EC\0", pl_listeRADT->Don_HisEssieu.Demande_Histo );
                                     else
                                        strcpy ( pl_Texte, "MH EC\0" );
                                     XZST_03EcritureTrace( XZSTC_INFO,"Trame pesage : <%s> station : %d", pl_Texte, pl_listeRADT->ConfRADT.NumeroStation );
				     if (!strcmp(pl_listeRADT->ConfRADT.AdresseIP, XZECC_IP_VIDE)) {
                                     	er_EnvoyerTrame( 	pl_Texte,
                                   		      	XZEXC_TRAME_PESAGE_ESSIEU,
                                   		      	pl_listeRADT->ConfRADT.RGS,
                                   			XZEXC_NOCMD,
	                           			XZEXC_PRIORITEFAIBLE,
                                   			pl_listeRADT->Socket );
                                      } 
				   else {
					EnvoyerTrameIP( pl_Texte,
							XZEXC_TRAME_PESAGE_ESSIEU,
							pl_listeRADT->ConfRADT.AdresseIP,
							pl_listeRADT->ConfRADT.Port,
							pl_listeRADT->ConfRADT.RGS,
							XZEXC_NOCMD,
							XZEXC_PRIORITEFAIBLE,
							pl_listeRADT->Socket );
				   }
                                   }
                                   else
                                      er_Recopie_Fichier_Mesure_Pesage ( pl_listeRADT->ConfRADT.NumeroStation, XDC_MESURE_ESSIEU );

                                   if ( (pl_listeRADT->Don_HisPoids.Horodate_Histo + 480) < vg_Date_Actuelle )
                                   {
  		                      pl_listeRADT->Don_HisPoids.Demande_Histo = 1 + (vg_Date_Actuelle  - pl_listeRADT->Don_HisPoids.Horodate_Histo) / 3600;
                                     /*B demande mesure pesage Poids MH PC */ 
                                     if ( pl_listeRADT->Don_HisPoids.Demande_Histo > 1 ) 
                                        sprintf ( pl_Texte, "MH %d PC\0", pl_listeRADT->Don_HisPoids.Demande_Histo );
                                     else
                                        strcpy ( pl_Texte, "MH PC\0" );
                                     XZST_03EcritureTrace( XZSTC_INFO,"Trame pesage : <%s> station : %d", pl_Texte, pl_listeRADT->ConfRADT.NumeroStation );
				     if (!strcmp(pl_listeRADT->ConfRADT.AdresseIP, XZECC_IP_VIDE)) {
                                     	er_EnvoyerTrame( 	pl_Texte,
                                   		      	XZEXC_TRAME_PESAGE_POIDS,
                                   		      	pl_listeRADT->ConfRADT.RGS,
                                   			XZEXC_NOCMD,
	                           			XZEXC_PRIORITEFAIBLE,
                                   			pl_listeRADT->Socket );
                                      } 
				   else {
					EnvoyerTrameIP( pl_Texte,
							XZEXC_TRAME_PESAGE_POIDS,
							pl_listeRADT->ConfRADT.AdresseIP,
							pl_listeRADT->ConfRADT.Port,
							pl_listeRADT->ConfRADT.RGS,
							XZEXC_NOCMD,
							XZEXC_PRIORITEFAIBLE,
							pl_listeRADT->Socket );
				   }
                                   }
                                   else
                                      er_Recopie_Fichier_Mesure_Pesage ( pl_listeRADT->ConfRADT.NumeroStation, XDC_MESURE_POIDS );

                               }
  	                       
  	                   }
  	                } 
  	                else
  	                {
  	                   if ( pl_listeRADT->ConfRADT.Liaison > 0 )
                              er_Maj_Fichier_heure ( pl_listeRADT->ConfRADT.NumeroStation, vg_Date_Actuelle );
                        }

                     }  
  	             break;
  	            
  	         /*A Cas ou periode Timer = 30mn */
  	         case ERDTC_REVEIL_30MN	 :
		    if (vg_mode_simu)
			return (XDC_OK);
  	         
  	            XZST_03EcritureTrace( XZSTC_INFO, "%s", pl_CleProg );
  	            
  	            /*B Pour les RADT de la machine => demande de status complet si la RADT n'est pas HS et pas de mode lent */
                    if ( vg_Mode_Lent == XDC_FAUX )
                    {
                       for ( pl_listeRADT = pg_debutRADT; pl_listeRADT != NULL; pl_listeRADT = pl_listeRADT->Suivant )
                       {
                          if ( !(pl_listeRADT->Etat_Service & XDC_EQT_HORS_SRV ) && 
                                (pl_listeRADT->Don_HisRADT.Demande_Histo != XDC_VRAI) &&
                                (pl_listeRADT->ConfRADT.Liaison > 0) )
                          {
			      XZST_03EcritureTrace( XZSTC_WARNING,"ADRESSE %s",pl_listeRADT->ConfRADT.AdresseIP);
			      if (!strcmp(pl_listeRADT->ConfRADT.AdresseIP, XZECC_IP_VIDE)) {
                                if ( ( vl_retcode = er_EnvoyerTrame("ST",
                                	   			   XZEXC_TRAME_STAT_COMP,
                                   				   pl_listeRADT->ConfRADT.RGS,
                                   				   XZEXC_NOCMD,
	                           				   XZEXC_PRIORITEFAIBLE,
                                   				   pl_listeRADT->Socket ) ) != XDC_OK )
                              {
                            	 XZST_03EcritureTrace( XZSTC_WARNING,"er_traitement_cyclique : EnvoyerTrame retourne %d", vl_retcode );
  	                      }
			    }
				   else {
					EnvoyerTrameIP( "ST",
							XZEXC_TRAME_STAT_COMP,
							pl_listeRADT->ConfRADT.AdresseIP,
							pl_listeRADT->ConfRADT.Port,
							pl_listeRADT->ConfRADT.RGS,
							XZEXC_NOCMD,
							XZEXC_PRIORITEFAIBLE,
							pl_listeRADT->Socket );
				   }
  	                  } 
  	               }  
  	            }
  	            XZST_03EcritureTrace( XZSTC_INFO, "%s", pl_CleProg );
  	            
  	            if (!vg_Mac_Ext_Ouest)
		    {
		       /*B Recopie des fichiers d'heure 6min QVTO et PL sur la machine suppleante */
  	               if ( ex_presence_machine ( vg_NomSuppleant ) != XDC_OK )
                          XZST_03EcritureTrace( XZSTC_WARNING,"%s : Machine Suppleante %s non presente", version, vg_NomSuppleant );
                       else
  	                  /*B Recopie des fichiers d'heure 6min QVTO et PL sur la machine suppleante */
  	                  if ( ex_cp_heure_RADT ( vg_NomMachine, vg_NomSuppleant ) != XDC_OK )
                             XZST_03EcritureTrace( XZSTC_WARNING,"%s : Copie Fichiers heures QVTO et PL sur %s impossible", version, vg_NomSuppleant );
  	            }
		 } 
  	     }
	  
		    if (vg_mode_simu)
			return (XDC_OK);
	  /*A S'il s'agit du reveil correspondant au traitement de Minuit */
	  if ( strstr ( pl_CleProg, ERDTC_TIMER_MINUIT ) != NULL )
	  {
             /*B Demande de l'heure courante */
   		/*time ( &vl_time );
   		vl_horodateSec = (XDY_Horodate) vl_time;  remplacer par XZSM_07 XZAE_191*/
   	     XZSM_07LireHorodate( XZSMC_TYPE_SYSTEME, &vl_i, &vl_horodateSec );
	
	     /*B Conversion de l'heure recue (en secondes) en date (char[20]) */
	     if ( ( vl_retcode = XZSM_11ConversionHorodate ( vl_horodateSec, &vl_heure_string ) ) != XDC_OK )
	     {
	 	if ( ( vl_retcode = XZST_10ArchiverMsgSyst ( 0 , "%s : Probleme pour conversion heure RTWORKS, la tache TREDT ne peut pas demarer", 
	 							version, NULL ) ) != XDC_OK )
		{	
		    XZST_03EcritureTrace(XZSTC_WARNING,"%s : Appel XZST_10ArchiverMsgSyst retourne ", version, vl_retcode );
		}
	     }
             
             /*A  Construction du DatGroup XDG_EMES_POIDS_<NomSite> */
	    /* XDG_EncodeDG2 ( pl_DG_Poids, XDG_EMES_POIDS, vg_NomSite );*/
   			
             for ( pl_listeRADT = pg_debutRADT; pl_listeRADT != NULL; pl_listeRADT = pl_listeRADT->Suivant )
             {
                vl_Num_RADT = pl_listeRADT->ConfRADT.NumeroStation;

		/*SITEGESTION DEM748*/
		XDG_EncodeDG2 ( pl_DG_Poids, XDG_EMES_POIDS, XZSC23_Nom_From_Numero(pl_listeRADT->ConfRADT.SiteGestion));
                 
                for ( vl_Sens = 1 ; vl_Sens <= ERDTC_NB_CHAUS_S_COUR; vl_Sens ++ )
                {
               	   if ( pg_ERDT_Mes_6mn_PL[pg_Tab_RADT[vl_Num_RADT]][vl_Sens].Sens != XDC_SENS_INCONNU )
               	   {
                      /*A Envoi du message XDM_EMES_POIDS en fournissant les valeur recues */
 
   		      if (!TipcSrvMsgWrite (  	pl_DG_Poids
                          		  	,XDM_IdentMsg(XDM_EMES_POIDS)
                          			,XDM_FLG_SRVMSGWRITE
                          			,T_IPC_FT_REAL8, vl_horodateSec
                          			,T_IPC_FT_INT2, pg_ERDT_Mes_6mn_PL[pg_Tab_RADT[vl_Num_RADT]][vl_Sens].Numero
                          			,T_IPC_FT_INT2, pg_ERDT_Mes_6mn_PL[pg_Tab_RADT[vl_Num_RADT]][vl_Sens].Sens
                          			,T_IPC_FT_INT2, pg_ERDT_Mes_6mn_PL[pg_Tab_RADT[vl_Num_RADT]][vl_Sens].Cumul
                          			,T_IPC_FT_INT2, pg_ERDT_Mes_6mn_PL[pg_Tab_RADT[vl_Num_RADT]][vl_Sens].NB_PL
                          			,NULL))
        	      {
               	 	  XZST_03EcritureTrace(XZSTC_WARNING,"Envoi message XDM_EMES_POIDS non effectue \n");
 		      }
   
		      /*A Envoi du message */
                      TipcSrvFlush();

   		      XZST_03EcritureTrace(XZSTC_INFO,"Envoi message XDM_EMES_POIDS effectue avec succes\n");
  		
  		       /*A RAZ du cumul Poids et NbPL */
  		       pg_ERDT_Mes_6mn_PL[pg_Tab_RADT[vl_Num_RADT]][vl_Sens].Cumul = 0;
  		       pg_ERDT_Mes_6mn_PL[pg_Tab_RADT[vl_Num_RADT]][vl_Sens].NB_PL = 0;
  		    }
                 }
  	      }  
  	            
  	      /*A Rearmement du reveil de Minuit */
              ETIM_SYNCHRO_DATE ( XZEXC_HEURE_MINUIT, ERDTC_TIMER_MINUIT, va_NumSock, pl_MsgTimer )
  	   }
  	  
  	   /*A S'il s'agit du reveil pour la synchronisation de la date */
	   if ( strstr ( pl_CleProg, ERDTC_TIMER_SYNCHRO ) != NULL )
	   {
//             /*A Convertir l'horodate en JJ/MM/AAAA HH:MM:SS */
//             XZSM_11ConversionHorodate ( vl_Horodate, &vl_Horodatedecompo );
//             
//             /* passage de l'annee sur 2 digit */
//             memmove(&vl_Horodatedecompo[6],&vl_Horodatedecompo[8],12);
//
//             /*B Composition de la trame de mise a la date et a l'heure */
//  	     sprintf ( vl_tlcr, "DATE ID=%s %s",vg_idpassw,vl_Horodatedecompo);
  	     sprintf ( vl_tlcr, "DT" );

  	     XZST_03EcritureTrace( XZSTC_DEBUG1, "trame de date:%s", vl_tlcr );
  	     
  	     /*A Pour tous les rdt -> demande de mise a l'heure */
             for ( pl_listeRADT = pg_debutRADT; pl_listeRADT != NULL; pl_listeRADT = pl_listeRADT->Suivant )
             {
                if ( pl_listeRADT->ConfRADT.Liaison > 0 )  {
		   if (!strcmp(pl_listeRADT->ConfRADT.AdresseIP, XZECC_IP_VIDE))
                     er_EnvoyerTrame( vl_tlcr,
                           XZEXC_TRAME_DATE_ID_DT,
                           pl_listeRADT->ConfRADT.RGS,
                           XZEXC_NOCMD,
	                   XZEXC_PRIORITEFAIBLE,
                           pl_listeRADT->Socket);
			   else {
					EnvoyerTrameIP( vl_tlcr,
							XZEXC_TRAME_DATE,
							pl_listeRADT->ConfRADT.AdresseIP,
							pl_listeRADT->ConfRADT.Port,
							pl_listeRADT->ConfRADT.RGS,
							XZEXC_NOCMD,
							XZEXC_PRIORITEFAIBLE,
							pl_listeRADT->Socket );
				   }
	       }
             }  
  	            
             for ( pl_listeRADT = pg_debutRADT; pl_listeRADT != NULL; pl_listeRADT = pl_listeRADT->Suivant )
             {
                if ( !access ( "erreur_trame", F_OK ) )
                {
                   sprintf ( pl_PathEtFic, "%s/Erreur_Trame_R%d", ERDTC_PATH_FIC_DYN, pl_listeRADT->ConfRADT.NumeroStation );
                   if ( (pl_Fd = fopen(pl_PathEtFic, "a+")) != NULL )
                   {
  	              fseek ( pl_Fd, 0, SEEK_END );
                      vl_TailleFic = ftell ( pl_Fd );
                      fclose ( pl_Fd );
                      if ( vl_TailleFic > 500000 )
                      {
                         sprintf ( pl_PathEtFicOld, "%s.old", pl_PathEtFic );
                         rename ( pl_PathEtFic, pl_PathEtFicOld );
                      }
                   }
                }				
                
                if ( !access ( "stock_mesures", F_OK ) )
                {
                   XZSM_11ConversionHorodate ( vl_Horodate, &pl_Texte );
                   sscanf ( pl_Texte, "%02d/%02d", &vl_jour, &vl_mois );
                   for ( vl_indexMois = 1; vl_indexMois <= ERDTC_NB_MOIS_AN; vl_indexMois++ )
                   {
                      if ( vl_indexMois != vl_mois )
                      {
                          sprintf ( pl_PathEtFicOld, "%s/Mes_%02d", ERDTC_PATH_FIC_DYN, vl_indexMois );
                          if ( access ( pl_PathEtFicOld, F_OK ) )
                             mkdir ( pl_PathEtFicOld, S_IRUSR | S_IRGRP | S_IROTH | S_IWUSR | S_IWGRP | S_IWOTH | S_IXUSR | S_IXGRP | S_IXOTH );
                          for ( vl_indexJour = 1; vl_indexJour <= ERDTC_NB_JOUR_MOIS; vl_indexJour++ )
                          {
                              sprintf ( pl_PathEtFic, "%s/%s_R%d.%02d_%02d", ERDTC_PATH_FIC_DYN, ERDTC_FIC_SAUV_QVTO,
                          		     pl_listeRADT->ConfRADT.NumeroStation, vl_indexJour, vl_indexMois );
                              if ( !access ( pl_PathEtFic, F_OK ) )
                              {
                                 sprintf ( pl_PathEtFicOld, "%s/Mes_%02d/%s_R%d.%02d_%02d", ERDTC_PATH_FIC_DYN, vl_indexMois,
                                 		ERDTC_FIC_SAUV_QVTO, pl_listeRADT->ConfRADT.NumeroStation, vl_indexJour, vl_indexMois ); 
                                 rename ( pl_PathEtFic, pl_PathEtFicOld );                             
                              }
                          }
                       }
                    }
                 }
             }  

  	     /*A Rearmement du reveil synchro date  */
             ETIM_SYNCHRO_DATE ( XZEXC_HEURE_SYNCHRO, ERDTC_TIMER_SYNCHRO, va_NumSock, pl_MsgTimer )
	  }
       }
    }
    XZST_03EcritureTrace( XZSTC_FONCTION, "OUT : %s", version );
    return (XDC_OK);
}

