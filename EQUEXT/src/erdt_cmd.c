/*E*/
/* Fichier : $Id: erdt_cmd.c,v 1.32 2021/01/29 15:05:25 akkaakka Exp $        Release : $Revision: 1.32 $        Date : $Date: 2021/01/29 15:05:25 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TERDT * FICHIER erdt_cmd.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module de traitement des commandes recues par la tache TERDT >
*
------------------------------------------------------
* HISTORIQUE :
*
* Volcic.F	version 1.1	08 Dec 1994	: Creation
* Volcic.F	version 1.2	13 Fev 1995	:
* Volcic.F	version 1.3	20 Mar 1995	: Modifs Diverses
* Volcic.F	version 1.4	19 Avr 1995	:
* Volcic.F	version 1.5	03 Mai 1995	: Reprise historique,
* Volcic.F	version 1.6	02 Jun 1995	: Modif commande LCR pour toutes les Stations RADT du LT
* Volcic.F	version 1.7	01 Aou 1995	: Ajout Acknoledge sur envoi mesures 6 minutes,
* Mismer.D	version 1.8	31 Aou 1995	: Modif trace FATAL sur echec envoi trame TELMI
* Volcic.F	version 1.9	20 Oct 1995	: Modif Acknoledge sur envoi mesures 6 minutes pour extrapolation,
* Volcic.F	version 1.10	07 Nov 1995	: Ajout Traces er_Dem_Indiv_cb
* Volcic.F	version 1.11	29 Nov 1995	: Modif Traces er_Envoyer_Trame
* Mismer.D	version 1.14	06 Fev 1996	: Suppression de l'espace dans la trame cmd LCR (er_Envoyer_Trame)
* Mismer.D	version 1.15	15 Fev 1996	: Suppression de "N�" dans fichier donn�e individuelle
Mismer.D	version 1.16	29 Mai 1996	: Inversion sens et voie dans XDM_EInit_IHM_6gl_Voie (DEM/1050)
* Volcic.F	version 1.18	06 Sep 1996	: Modif Extension Ouest (RADT_W)
* Mismer.D	version 1.19	03 Sep 1996	: Modif pour mesures pesage (DEM/1227)
* Mismer.D	version 1.20	12 Dec 1996	: Calcul des niveaux de services pourstations virtuelles (DEM/1338)
* Mismer.D	version 1.21	27 Jan 1997	: Modif calcul DIV su MI (DEM/1392)
* Mismer.D	version 1.22	22 Jan 1998	: Modif raf syno (DEM/1543)
* Diaz.L 	version 1.23    09 Oct 1998     : Correction(DEM/1495&1614&1615)
* Hilmarcher    version 1.25    22 juin 2005    : modif smartsockets: TipcSrvDgGetRecv devient TipcSrvSubjectGetSubscribe
* JPL		1.26		23/11/09	: Changement de niveaux de traces, notamment "Warning" en "Info" (DEM 908)
* JPL		17/03/11 : Migration HP ia64 (DEM 975) : Gestion conversions types de tailles differentes (=)  1.27
* JMG		04/07/12 : simulation DEM 1016  1.28
* JPL	15/02/18 : Migration Linux 64 bits (DEM 1274) : 'er_Init_IHM_6gl_Chaussee_cb': recherche mesure limitee a l'index 1mn  1.29
* JMG	31/10/18 : ajout IP DEM1306 1.30
* JMG	11/05/20 : EMS 1.341
* GR  10/07/20 : EMS 1.32
* AMI 28/01/21 : SAE_191: chantier date: encapsulation localtime pour hiniber TZ car nbseconde deja en TZ
* LCL		20/12/21	: MOVIS P2 ems SAE-312
------------------------------------------------------*/

/* fichiers inclus */
#include <xzsem.h>

#include "ex_mala.h"

#include "erdt_cmd.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

/* declaration de fonctions internes */
extern XDY_Booleen vg_mode_simu;

/* definition de fonctions externes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*  Demande Individuelles
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	er_Dem_Indiv_cb ( 
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
*
*  Fonction appel�e lors de la r�ception du message XDM_EDEM_Indiv
* 
*
* FONCTION 
*
*  Fonction appelee lors de la reception du message XDM_EDEM_Indiv
* Ouverture du fichier de stockage des mesures individuelles
* Indicateur de demande des mesures individuelles pour le traitement
*
------------------------------------------------------*/
{
   static char *version = "$Id: erdt_cmd.c,v 1.32 2021/01/29 15:05:25 akkaakka Exp $      : er_Dem_Indiv_cb" ;

   #ifndef _TIBCO_EMS
   T_STR			pl_DGInit;
   T_INT2 			vl_NumEqt = 0;
   T_INT2			vl_Demande = 0;
   #else
   XDY_Datagroup                pl_DGInit;
   XDY_Mot 			vl_NumEqt = 0;
   XDY_Mot			vl_Demande = 0;
   #endif   

   ERDT_DONNEES_RADT *		pl_listeRADT;
   ERDT_DONNEES_PtMes		*pl_ListePtMes;
   XDY_FichEquext		pl_PathFic = "";
   XDY_Commentaire		pl_Ligne = "";
  // time_t			vl_time = 0; SAE191
   XDY_Horodate		vl_time = 0;
   int			        vl_retcode = 0;
   int vl_jourSemaine = 0  ; // SAE_191 pas utilise    

   XZST_03EcritureTrace(XZSTC_FONCTION, "IN: er_Dem_Indiv_cb");
		  
   /*A Recuperation des parametres du message XDM_EDEM_Indiv */
   if(!TipcMsgRead(	pa_Data -> msg,
               		T_IPC_FT_INT2, &vl_NumEqt,
               		T_IPC_FT_INT2, &vl_Demande,
	       		NULL))
   {       
      XZST_03EcritureTrace(XZSTC_WARNING,"messsage XDM_EDEM_Indiv non lu" );
      XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: er_Dem_Indiv_cb");
      return;
   }

   XZST_03EcritureTrace(XZSTC_INFO, "er_Dem_Indiv_cb : Eqt = <%d>  Dem. = <%d>", vl_NumEqt, vl_Demande);
	           
   /*A Recuperation de l'horodate */
   //time ( &vl_time ); / SAE_191 modif chantier date
  
   XZSM_07LireHorodate ( XZSMC_TYPE_SYSTEME, &vl_jourSemaine, &vl_time );                                   
   vg_Horo_Dem_Ind[pg_Tab_RADT[vl_NumEqt]] = (XDY_Horodate) vl_time;
   
   /*A Ouverture du fichier de stockage des mesures individuelles */
   sprintf ( pl_PathFic,"%s/%s_R%d", ERDTC_PATH_FIC_DYN, XDF_Dem_Indiv, vl_NumEqt );
   
   if ( vl_Demande == XDC_VRAI )
   {
      if ( !access ( pl_PathFic, F_OK ) )
      {
         if ( vg_Fd_Dem_Ind[pg_Tab_RADT[vl_NumEqt]] != 0)
         	fclose ( vg_Fd_Dem_Ind[pg_Tab_RADT[vl_NumEqt]] );
         vg_Fd_Dem_Ind[pg_Tab_RADT[vl_NumEqt]] = 0;
         XZSS_06DetruireFichier ( pl_PathFic);
      }
      
      if ( ( vg_Fd_Dem_Ind[pg_Tab_RADT[vl_NumEqt]] = fopen ( pl_PathFic, "a+" ) ) == NULL )
      {
   	XZST_03EcritureTrace( XZSTC_WARNING,"Fichier %s non trouve", pl_PathFic );
 	XZST_03EcritureTrace( XZSTC_FONCTION, "OUT: er_Dem_Indiv_cb" );
   	return;
      }
      XZST_03EcritureTrace(XZSTC_DEBUG1, "er_Dem_Indiv_cb : Fich = <%s> FD = <%d>", pl_PathFic, vg_Fd_Dem_Ind[pg_Tab_RADT[vl_NumEqt]]);
      
      /*B Raz memo dernier passage */
      for ( pl_ListePtMes = pg_debutPtMes; pl_ListePtMes != NULL ; pl_ListePtMes = pl_ListePtMes->Suivant )
         if ( pl_ListePtMes->ConfPtMes.NumeroStation == vl_NumEqt )
            pl_ListePtMes->Sec_dernier_veh = 0.0;
   
      /*A Ecriture de l'entete du fichier de stockage des mesures individuelles */
      fprintf ( vg_Fd_Dem_Ind[pg_Tab_RADT[vl_NumEqt]],"%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n", 
      							"#STATION", "SENS", "VOIE", "DATE", "HEURE","VITESSE", "POIDS", "DIV", "LONGUEUR", "Tps_Inter_Veh", "Seconde_passage" );
      fflush ( vg_Fd_Dem_Ind[pg_Tab_RADT[vl_NumEqt]] ); 
   }
   else
   {
      if ( vg_Fd_Dem_Ind[pg_Tab_RADT[vl_NumEqt]] != 0 )
      		fclose ( vg_Fd_Dem_Ind[pg_Tab_RADT[vl_NumEqt]] );
      vg_Fd_Dem_Ind[pg_Tab_RADT[vl_NumEqt]] = 0;
   }
	    			      
   /*A Indicateur de demande mesures individuelles pour traitement */
   vg_Demande_Individuelle[pg_Tab_RADT[vl_NumEqt]] = vl_Demande;
   XZST_03EcritureTrace(XZSTC_DEBUG1, "er_Dem_Indiv_cb : Horo = <%lf> vg_Dem = <%d>", vg_Horo_Dem_Ind[pg_Tab_RADT[vl_NumEqt]], vg_Demande_Individuelle[pg_Tab_RADT[vl_NumEqt]]);
   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: er_Dem_Indiv_cb ");
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*  Changement de configuration des voies.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	er_Conf_Voie_RADT_cb ( 	T_IPC_CONN 			va_Cnx,
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
*  XDM_EConf_Voie_RADT 
*
* FONCTION 
*
*  Remplissage des structures de config des voies
*
------------------------------------------------------*/
{
   static char *version = "$Id: erdt_cmd.c,v 1.32 2021/01/29 15:05:25 akkaakka Exp $      : er_Conf_Voie_RADT_cb" ;

   #ifndef _TIBCO_EMS
   T_STR			pl_DGInit;
   #else
   XDY_Datagroup                pl_DGInit;
   #endif   

   ERDT_DONNEES_PtMes 	 	*pl_ListePtMes;
   XDY_Horodate 		vl_Horodate = 0.0;
   XDY_Entier 			vl_Index = 0;
   XDY_Entier 			vl_i = 0;
   XDY_Entier 			vl_j = 0;
#ifndef _TIBCO_EMS
   T_INT2 			vl_NumEqt = 0;
   T_INT2 			*pl_Voies;
   T_INT2			vl_Sens = 0;
   T_INT4 			vl_Nb_Voie = 0;
#else 
   XDY_Mot 			vl_NumEqt = 0;
   XDY_Mot 			pl_Voies[255];
   XDY_Mot			vl_Sens = 0;
   XDY_Entier 		vl_Nb_Voie = 0;
#endif
   T_INT2 			vl_Voies[5];
   
   XZST_03EcritureTrace(XZSTC_FONCTION, "IN: er_Conf_Voie_RADT_cb ");
		  
   /*A Recuperation des parametres du message XDM_EConf_Voie_RADT */
   if(!TipcMsgRead(	pa_Data -> msg,
               		T_IPC_FT_INT2, &vl_NumEqt,
               		T_IPC_FT_INT2, &vl_Sens,
               		T_IPC_FT_INT2_ARRAY, &pl_Voies, &vl_Nb_Voie,
               		NULL))
   {       
      XZST_03EcritureTrace(XZSTC_WARNING,"messsage XDM_EConf_Voie_RADT non lu" );
      XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: er_Conf_Voie_RADT_cb");
      return;
   }
	        
   /* recuperation du tableau de config des voies */ 
#ifndef _TIBCO_EMS
   for ( vl_i = 0;(vl_i < vl_Nb_Voie);vl_Voies[vl_i] = *pl_Voies, vl_i++, pl_Voies++);
#else 
   for ( vl_i = 0;(vl_i < vl_Nb_Voie); vl_i++)
	   vl_Voies[vl_i] = pl_Voies[vl_i];
#endif

   /*A Mise a jour du champ Voie du tableau 6mn des mesures
       Pour chaque chaussee de la station RADT : Remplir la structure des tables de config des voies */
   for ( vl_i = 1; vl_i < vl_Nb_Voie; vl_i ++ )
   {
   	for ( pl_ListePtMes = pg_debutPtMes; pl_ListePtMes != NULL; pl_ListePtMes = pl_ListePtMes->Suivant )
   	{
      		/*A Si la station RADT est dans la liste => mise a jour de l'etat des voies */
      		if (   	pl_ListePtMes->ConfPtMes.NumeroStation == vl_NumEqt &&
             		pl_ListePtMes->ConfPtMes.Sens == vl_Sens &&
             		pl_ListePtMes->ConfPtMes.Voie == vl_i )
      		{
	        	pl_ListePtMes->EtatVoie = vl_Voies[vl_i];
        	
                        XZST_03EcritureTrace(XZSTC_INFO, "er_Conf_Voie_RADT_cb : St = %d   Ss = %d   V = %d   Etat = %d  EtatStr = %d  ", 
                        	vl_NumEqt, vl_Sens, vl_i, vl_Voies[vl_i], pl_ListePtMes->EtatVoie);
                        
        		/*B Mise a jour du champ Voie du tableau 6mn des mesures pour le calcul des mesures par chaussee (er_Calcul_Chaussee) */
        		for ( vl_Index = 0; vl_Index < ERDTC_NB_INDEX_1MN; vl_Index ++ )
        			pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[vl_NumEqt]][vl_Sens][vl_i][vl_Index].Voie = \
        			   ((vl_Voies[vl_i] == XDC_VOIE_SENS_NORMAL) || (vl_Voies[vl_i] == XDC_VOIE_SENS_INVERSE)) ? vl_Voies[vl_i] : XDC_VOIE_INCONNUE;
        		for ( vl_Index = 0; vl_Index < ERDTC_NB_INDEX_6MN; vl_Index ++ )
        			pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_NumEqt]][vl_Sens][vl_i][vl_Index].Voie = \
        			   ((vl_Voies[vl_i] == XDC_VOIE_SENS_NORMAL) || (vl_Voies[vl_i] == XDC_VOIE_SENS_INVERSE)) ? vl_Voies[vl_i] : XDC_VOIE_INCONNUE;
        		break;
		}
      }	
   }  
   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: er_Conf_Voie_RADT_cb OK ");
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*
*  Demande Init IHM 6gl Voie.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void    er_Init_IHM_6gl_Voie_cb (	T_IPC_CONN                      va_Cnx,
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
*  XDM_EInit_IHM_6gl_Voie
*
* FONCTION
*  Pour toutes les PtMes, envoi de XDM_EInit_IHM_6gl_Voie dans le DataGroup passe en parametre
*
------------------------------------------------------*/
{
   static char *version = "$Id: erdt_cmd.c,v 1.32 2021/01/29 15:05:25 akkaakka Exp $      : er_Init_IHM_6gl_Voie_cb" ;

   #ifndef _TIBCO_EMS
   T_STR			pl_DGInit;
   T_INT2 			vl_NumEqt = 0;
   T_INT2			vl_Sens = 0;
   T_INT2			vl_Voie = 0;
   #else
   XDY_Datagroup                pl_DGInit;
   XDY_Mot 			vl_NumEqt = 0;
   XDY_Mot			vl_Sens = 0;
   XDY_Mot			vl_Voie = 0;
   #endif

   ERDT_DONNEES_PtMes 		*pl_ListePtMes;
   XDY_Entier			vl_i = 0;
   XDY_Entier			vl_Index = 0;
   XDY_Horodate			vl_Horodate = 0.0;
   
   XZST_03EcritureTrace(XZSTC_FONCTION, "IN: er_Init_IHM_6gl_Voie_cb ");

   /*A Recuperation des parametres du message XDM_EInit_IHM_6gl_Voie */
   if(!TipcMsgRead(	pa_Data -> msg,
               		T_IPC_FT_STR,  &pl_DGInit,
               		T_IPC_FT_INT2, &vl_NumEqt,
               		T_IPC_FT_INT2, &vl_Voie,
               		T_IPC_FT_INT2, &vl_Sens,
               		NULL))
   {
      XZST_03EcritureTrace(XZSTC_WARNING,"messsage XDM_EInit_IHM_6gl_Voie non lu" );
      XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: er_Init_IHM_6gl_Voie_cb NOK");
      return;
   }

   /*A Si le numero de la station est 0 ou si le sens est inconnu ou la voie inconnue => return */
   if ( (vl_NumEqt == 0) || (vl_Sens == XDC_SENS_INCONNU) || (vl_Voie < XDC_RADT_CHAUSSEE) || (vl_Voie > XDC_RADT_V4)  )
   {
      XZST_03EcritureTrace(XZSTC_WARNING,"er_Init_IHM_6gl_Voie_cb : XDM_EInit_IHM_6gl_Voie incorrect Eqt:%d Sens:%d Voie:%d", vl_NumEqt, vl_Sens, vl_Voie );
      XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: er_Init_IHM_6gl_Voie_cb NOK");
      return;
   }
   
   /*A Pour chaque PtMes de la station : envoi du msg XDM_EInit_IHM_6gl_Voie dans XDM_EMES_6gl_Voie */
   for ( pl_ListePtMes = pg_debutPtMes; pl_ListePtMes != NULL; pl_ListePtMes = pl_ListePtMes->Suivant )
   {
      if (   (pl_ListePtMes->ConfPtMes.NumeroStation == vl_NumEqt) &&
             (pl_ListePtMes->ConfPtMes.Sens == vl_Sens) &&
             ((pl_ListePtMes->EtatVoie == XDC_VOIE_SENS_NORMAL) || (pl_ListePtMes->EtatVoie == XDC_VOIE_SENS_INVERSE)) &&
             ((pl_ListePtMes->ConfPtMes.Voie == vl_Voie) || (vl_Voie == XDC_RADT_CHAUSSEE)) )
      {
      		/*A Recherche de la derniere horodate stockee dans le tableau de mesures 6mn gl */
      		vl_Horodate = 0;
      		
      		for ( vl_i = 0; vl_i < ERDTC_NB_INDEX_1MN; vl_i ++ )
      		{	
      			if ( vl_Horodate < pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[vl_NumEqt]][vl_Sens][vl_Voie][vl_i].Horodate )
      			{ 
      				vl_Index = vl_i;
      				vl_Horodate = pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[vl_NumEqt]][vl_Sens][vl_Voie][vl_i].Horodate;
      			}
      		}
     	 	
     	 	/*A Envoi des valeurs des mesures de la station a l'IHM */
      		if (!TipcSrvMsgWrite ( pl_DGInit,
                             XDM_IdentMsg(XDM_EMES_6gl_Voie),
                             XDM_FLG_SRVMSGWRITE,
                             T_IPC_FT_REAL8, pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[vl_NumEqt]][vl_Sens][vl_Voie][vl_Index].Horodate,
                             T_IPC_FT_INT2, pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[vl_NumEqt]][vl_Sens][vl_Voie][vl_Index].Periode,
                             T_IPC_FT_INT2, pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[vl_NumEqt]][vl_Sens][vl_Voie][vl_Index].Sens,
                             T_IPC_FT_INT2, vl_Voie,
                             T_IPC_FT_INT2, pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[vl_NumEqt]][vl_Sens][vl_Voie][vl_Index].Mesures.ValQ.Val,
                             T_IPC_FT_INT2, pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[vl_NumEqt]][vl_Sens][vl_Voie][vl_Index].Mesures.ValQ.Indic,
                             T_IPC_FT_INT2, pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[vl_NumEqt]][vl_Sens][vl_Voie][vl_Index].Mesures.ValV.Val,
                             T_IPC_FT_INT2, pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[vl_NumEqt]][vl_Sens][vl_Voie][vl_Index].Mesures.ValV.Indic,
                             T_IPC_FT_INT2, pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[vl_NumEqt]][vl_Sens][vl_Voie][vl_Index].Mesures.ValTO.Val,
                             T_IPC_FT_INT2, pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[vl_NumEqt]][vl_Sens][vl_Voie][vl_Index].Mesures.ValTO.Indic,
                             T_IPC_FT_INT2, pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[vl_NumEqt]][vl_Sens][vl_Voie][vl_Index].Mesures.ValPL.Val,
                             T_IPC_FT_INT2, pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[vl_NumEqt]][vl_Sens][vl_Voie][vl_Index].Mesures.ValPL.Indic,
                             NULL))
         	{
               	   XZST_03EcritureTrace(XZSTC_WARNING,"Envoi message XDM_EInit_IHM_6gl_Voie non effectue" );
         	}
         	else
         	{
               	   XZST_03EcritureTrace(XZSTC_MESSAGE,"Envoi message XDM_EInit_IHM_6gl_Voie effectue" );
         	}
         	break;
       }
   } 
   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: er_Init_IHM_6gl_Voie_cb ");
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Demande Init IHM 6gl Chaussee.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void    er_Init_IHM_6gl_Chaussee_cb (	T_IPC_CONN                      va_Cnx,
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
*  XDM_EInit_IHM_6gl_Chaussee
*
* FONCTION
*  Pour toutes les Chaussee, envoi de XDM_EInit_IHM_6gl_Chaussee dans le DataGroup passe en parametre
*
------------------------------------------------------*/
{
   static char *version = "$Id: erdt_cmd.c,v 1.32 2021/01/29 15:05:25 akkaakka Exp $      : er_Init_IHM_6gl_Chaussee_cb" ;

   #ifndef _TIBCO_EMS
   T_STR			   pl_DGInit;
   T_INT2 			vl_NumEqt = 0;
   #else
   XDY_Datagroup  pl_DGInit;
   XDY_Mot 			vl_NumEqt = 0;
   #endif

   ERDT_DONNEES_PtMes 		*pl_ListePtMes;
   XDY_Sens 			vl_Sens = 0;
   XDY_Entier			vl_Index = 0;
   XDY_Horodate			vl_Horodate = 0.0;
   XDY_Entier			vl_i = 0;
   
   XZST_03EcritureTrace(XZSTC_FONCTION, "IN: er_Init_IHM_6gl_Chaussee_cb ");

   /*A Recuperation des parametres du message XDM_EInit_IHM_6gl_Chaussee */
   if(!TipcMsgRead(	pa_Data -> msg,
               		T_IPC_FT_STR,  &pl_DGInit,
               		T_IPC_FT_INT2, &vl_NumEqt,
               		NULL))
   {
      XZST_03EcritureTrace(XZSTC_WARNING,"messsage XDM_EInit_IHM_6gl_Chaussee non lu" );
      XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: er_Init_IHM_6gl_Chaussee_cb NOK");
      return;
   }

   /*A Pour chaque PtMes de la station : envoi du msg XDM_EInit_IHM_6gl_Chaussee dans XDM_EMES_6gl_Chaussee */
   for ( pl_ListePtMes = pg_debutPtMes; pl_ListePtMes != NULL; pl_ListePtMes = pl_ListePtMes->Suivant )
   {
      if ( (vl_NumEqt == XDC_EQTALL) || (pl_ListePtMes->ConfPtMes.NumeroStation == vl_NumEqt) )
      {
          /*A Recherche des chaussees de la stations RADT */
          for ( vl_Sens = 0; vl_Sens < ERDTC_NB_CHAUSSEE; vl_Sens ++ )
          {
      		/*A Si la chaussee existe pour cette station RADT */
      		if ( pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[vl_NumEqt]][vl_Sens][0][vl_Index].Voie != XDC_VOIE_INCONNUE )
      		{
      			/*A Recherche de la derniere horodate stockee dans le tableau de mesures 6mn gl */
      		
      			vl_Horodate = 0;
      		
      			for ( vl_i = 0; vl_i < ERDTC_NB_INDEX_1MN; vl_i ++ )
      			{	
      				if ( vl_Horodate < pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[vl_NumEqt]][vl_Sens][0][vl_i].Horodate )
      				{ 
      					vl_Index = vl_i;
      					vl_Horodate = pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[vl_NumEqt]][vl_Sens][0][vl_i].Horodate;
      				}
      			}
     	 	
     	 		/*A Envoi des valeurs des mesures de la station a l'IHM */
      			if (!TipcSrvMsgWrite ( pl_DGInit,
                             XDM_IdentMsg(XDM_EMES_6gl_Chaussee),
                             XDM_FLG_SRVMSGWRITE,
                             T_IPC_FT_REAL8, pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[vl_NumEqt]][vl_Sens][0][vl_Index].Horodate,
                             T_IPC_FT_STR,  vg_NomSite,
                             T_IPC_FT_INT2, pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[vl_NumEqt]][vl_Sens][0][vl_Index].Periode,
                             T_IPC_FT_INT2, pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[vl_NumEqt]][vl_Sens][0][vl_Index].Sens,
                             T_IPC_FT_INT2, pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[vl_NumEqt]][vl_Sens][0][vl_Index].Voie,
                             T_IPC_FT_INT2, pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[vl_NumEqt]][vl_Sens][0][vl_Index].Mesures.ValQ.Val,
                             T_IPC_FT_INT2, pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[vl_NumEqt]][vl_Sens][0][vl_Index].Mesures.ValQ.Indic,
                             T_IPC_FT_INT2, pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[vl_NumEqt]][vl_Sens][0][vl_Index].Mesures.ValV.Val,
                             T_IPC_FT_INT2, pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[vl_NumEqt]][vl_Sens][0][vl_Index].Mesures.ValV.Indic,
                             T_IPC_FT_INT2, pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[vl_NumEqt]][vl_Sens][0][vl_Index].Mesures.ValTO.Val,
                             T_IPC_FT_INT2, pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[vl_NumEqt]][vl_Sens][0][vl_Index].Mesures.ValTO.Indic,
                             T_IPC_FT_INT2, pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[vl_NumEqt]][vl_Sens][0][vl_Index].Mesures.ValPL.Val,
                             T_IPC_FT_INT2, pg_ERDT_Mes_6mn_gl[pg_Tab_RADT[vl_NumEqt]][vl_Sens][0][vl_Index].Mesures.ValPL.Indic,
                             NULL))
         		{
               	 		XZST_03EcritureTrace(XZSTC_WARNING,"Envoi message XDM_EInit_IHM_6gl_Chaussee non effectue" );
         		}
         		else
         		{
               	 		XZST_03EcritureTrace(XZSTC_MESSAGE,"Envoi message XDM_EInit_IHM_6gl_Chaussee effectue" );
         		}
         	}
           }
       }
   } 
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Demande Init IHM 6mn Chaussee.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void    er_Init_IHM_6mn_parVoie_cb (	T_IPC_CONN                      va_Cnx,
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
*  XDM_EInit_IHM_6mn_parVoie
*
* FONCTION
*  Pour toutes les Chaussee, envoi de XDM_EInit_IHM_6mn_parVoie dans le DataGroup passe en parametre
*
------------------------------------------------------*/
{
   static char *version = "$Id: erdt_cmd.c,v 1.32 2021/01/29 15:05:25 akkaakka Exp $      : er_Init_IHM_6mn_parVoie_cb" ;

   #ifndef _TIBCO_EMS
   T_STR			   pl_DGInit;
   T_INT2 			vl_NumEqt = 0;
   T_INT2			vl_Chaussee = 0;
   T_INT2			vl_Voie = 0;
   #else
   XDY_Datagroup  pl_DGInit;
   XDY_Mot 			vl_NumEqt = 0;
   XDY_Mot			vl_Chaussee = 0;
   XDY_Mot			vl_Voie = 0;
   #endif   

   ERDT_DONNEES_PtMes 		*pl_ListePtMes;
   XDY_Entier			vl_Index = 0;
   XDY_Horodate			vl_Horodate = 0.0;
   XDY_Entier			vl_i = 0;
   
   XZST_03EcritureTrace(XZSTC_FONCTION, "IN: er_Init_IHM_6mn_parVoie_cb");

   /*A Recuperation des parametres du message XDM_EInit_IHM_6mn_parVoie */
   if(!TipcMsgRead(	pa_Data -> msg,
               		T_IPC_FT_STR,  &pl_DGInit,
               		T_IPC_FT_INT2, &vl_NumEqt,
               		T_IPC_FT_INT2, &vl_Voie,
               		T_IPC_FT_INT2, &vl_Chaussee,
               		NULL))
   {
      XZST_03EcritureTrace(XZSTC_WARNING,"message XDM_EInit_IHM_6mn_parVoie non lu" );
      XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: er_Init_IHM_6mn_parVoie_cb NOK");
      return;
   }

   /*A Pour chaque PtMes de la station : envoi du msg XDM_EInit_IHM_6mn_parVoie dans XDM_EMES_6gl_Chaussee */
   for ( pl_ListePtMes = pg_debutPtMes; pl_ListePtMes != NULL; pl_ListePtMes = pl_ListePtMes->Suivant )
   {
      if ( pl_ListePtMes->ConfPtMes.NumeroStation == vl_NumEqt )
      {
      	 /*A Recherche de la derniere horodate stockee dans le tableau de mesures 6mn gl */
      	 vl_Horodate = 0;
      	 for ( vl_i = 0; vl_i < ERDTC_NB_INDEX_6MN; vl_i ++ )
      	 {	
            /*A Si la chaussee existe pour cette station RADT */
      	    if ( pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_NumEqt]][vl_Chaussee][vl_Voie][vl_i].Voie != XDC_VOIE_INCONNUE )
      	    {
      	        if ( vl_Horodate < pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_NumEqt]][vl_Chaussee][vl_Voie][vl_i].Horodate )
      		{ 
      		   vl_Index = vl_i;
      		   vl_Horodate = pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_NumEqt]][vl_Chaussee][vl_Voie][vl_i].Horodate;
      		}
      	    }
         }
         XZST_03EcritureTrace(XZSTC_DEBUG1, "IN: er_Init_IHM_6mn_parVoie_cb <%s>  eqt:%d ch:%d voie:%d etat:%lf", 
         					pl_DGInit, vl_NumEqt, vl_Chaussee, vl_Voie, vl_Horodate);
     	 if ( vl_Horodate > 0.0 )
     	 {
     	    /*A Envoi des valeurs des mesures de la station a l'IHM */
      	    if (!TipcSrvMsgWrite ( pl_DGInit,
                             XDM_IdentMsg(XDM_EMES_6mn_parVoie),
                             XDM_FLG_SRVMSGWRITE,
                             T_IPC_FT_REAL8, pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_NumEqt]][vl_Chaussee][vl_Voie][vl_Index].Horodate,
                             T_IPC_FT_INT2, pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_NumEqt]][vl_Chaussee][vl_Voie][vl_Index].Numero,
                             T_IPC_FT_INT2, pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_NumEqt]][vl_Chaussee][vl_Voie][vl_Index].Periode,
                             T_IPC_FT_INT2, pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_NumEqt]][vl_Chaussee][vl_Voie][vl_Index].Sens,
                             T_IPC_FT_INT2, vl_Voie,
                             T_IPC_FT_INT2, pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_NumEqt]][vl_Chaussee][vl_Voie][vl_Index].Mesures.ValQ.Val,
                             T_IPC_FT_INT2, pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_NumEqt]][vl_Chaussee][vl_Voie][vl_Index].Mesures.ValQ.Indic,
                             T_IPC_FT_INT2, pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_NumEqt]][vl_Chaussee][vl_Voie][vl_Index].Mesures.ValV.Val,
                             T_IPC_FT_INT2, pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_NumEqt]][vl_Chaussee][vl_Voie][vl_Index].Mesures.ValV.Indic,
                             T_IPC_FT_INT2, pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_NumEqt]][vl_Chaussee][vl_Voie][vl_Index].Mesures.ValTO.Val,
                             T_IPC_FT_INT2, pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_NumEqt]][vl_Chaussee][vl_Voie][vl_Index].Mesures.ValTO.Indic,
                             T_IPC_FT_INT2, pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_NumEqt]][vl_Chaussee][(vl_Voie) == 0 ? 1: vl_Voie][vl_Index].Mesures.ValPL.Val,
                             T_IPC_FT_INT2, pg_ERDT_Mes_6mn[pg_Tab_RADT[vl_NumEqt]][vl_Chaussee][(vl_Voie) == 0 ? 1: vl_Voie][vl_Index].Mesures.ValPL.Indic,
                             NULL))
            {
               XZST_03EcritureTrace(XZSTC_WARNING,"Envoi message XDM_EMES_6mn_parVoie non effectue" );
            }
            else
            {
               XZST_03EcritureTrace(XZSTC_MESSAGE,"Envoi message XDM_EMES_6mn_parVoie effectue pour St:%d Sens:%d Voie:%d sur <%s>", vl_NumEqt, vl_Chaussee,vl_Voie, pl_DGInit);
            }
         }
         break;
      }
   } 
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande Init IHM Liaison.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	ep_Init_IHM_Liaison_cb ( 
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
*  XDM_EInit_IHM_Liaison
*
* FONCTION 
*  Pour touts les RADT , envoyer l'etat  par XDM_ETAT_LIAISON dans le DataGroup passe en parametre
*
------------------------------------------------------*/
{
   static char *version = "@(#)erdt_cmd.c	1.22 01/30/98 :  ep_Init_IHM_Liaison_cb " ;

   #ifndef _TIBCO_EMS
   T_STR			pl_DGInit;
   #else
   XDY_Datagroup                pl_DGInit;
   #endif   

   ERDT_DONNEES_RADT *		pl_ListeRADT;
   

   XZST_03EcritureTrace(XZSTC_FONCTION, "IN: ep_Init_IHM_Liaison_cb ");
		  
   /*A Recuperation des parametres du message  */
   if(!TipcMsgRead(pa_Data -> msg,
               T_IPC_FT_STR,  &pl_DGInit,
	       NULL))
   {       
      /* Trace : Impossibilite de lire le message */
      XZST_03EcritureTrace(XZSTC_WARNING, "message XDM_EInit_IHM_Liaison non lu " );
      XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: ep_Init_IHM_Liaison_cb");
      return;
   }
	        
   /*A Pour chaque RADT : envoi du msg XDM_ETAT_LIAISON  */
   for ( pl_ListeRADT = pg_debutRADT; pl_ListeRADT != NULL; pl_ListeRADT = pl_ListeRADT->Suivant )
   {
      ex_env_etat_liaison ( vg_NomMachine, pl_DGInit, pl_ListeRADT->ConfRADT.NumeroStation, XDC_EQT_RAD ); 
   }  

   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: ep_Init_IHM_Liaison_cb ");
	       
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Envoi une trame a TELMi.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int  er_EnvoyerTrame ( 	XZEXT_Trame	trame,
		    	int		type_msg,
		    	char*		adr_rgs,
		    	int 		nocmd,
		    	int 		priorite,
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
*  Demande d'envoi de trame a telmi
*
------------------------------------------------------*/
{		     
static char *version = "$Id: erdt_cmd.c,v 1.32 2021/01/29 15:05:25 akkaakka Exp $      : er_EnvoyerTrame " ;

XZEXT_MSG_SOCKET	   vl_message;    
ssize_t			   vl_RetCode = 0; 
        
        sprintf(vl_message,"%s%d %s %d %d %s%s %s",
           			XZEXC_DEMANDE_LCR,
           			type_msg,
           			adr_rgs,
 		    		nocmd,
		    		priorite,
          			XZEXC_SEP_TRAME,
          			trame,
           			XZEXC_FIN_MSG_SOCK);
            			
           			
        fcntl ( socket, F_SETFL, O_NDELAY | fcntl ( socket, F_GETFL, 0 ) );
        sem_wait(&vg_semaphore); 
        if ( (vl_RetCode = write ( socket , vl_message , strlen(vl_message) )) < 0 ) 				  				
        {
            sem_post(&vg_semaphore); 
            /*B Ecriture dans le fichier trace en cas d'�chec */
            XZST_03EcritureTrace( XZSTC_FATAL, "Impossible d'�mettre la trame vers TELMi." );
	         return(XDC_NOK);
        }
        else
        {   
            sem_post(&vg_semaphore); 
            /*B Ecriture dans le fichier trace en cas de succes */
            if ( vl_RetCode != strlen(vl_message) )
            {
               XZST_03EcritureTrace( XZSTC_WARNING, "Trame INCOMPLETE �mise vers TELMi <%s>" ,vl_message );
               return(XDC_NOK);
            }
            else
            {
               XZST_03EcritureTrace( XZSTC_INFO, "Trame �mise vers TELMi  <%s>" ,vl_message );
               return(XDC_OK);
            }
        }
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*  Message de contr�le.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	er_controle_cb ( 
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
   static char *version = "$Id: erdt_cmd.c,v 1.32 2021/01/29 15:05:25 akkaakka Exp $      : er_controle_cb  " ;


#ifndef _TIBCO_EMS
   T_INT4			vl_Etat;
   T_STR			   vl_NomMachine;
   T_INT4			vl_TypeEqt;
   T_INT4			vl_NumEqt;
#else
   XDY_Entier			vl_Etat;
   XDY_NomMachine		vl_NomMachine;
   XDY_Entier			vl_TypeEqt;
   XDY_Entier			vl_NumEqt;
#endif   

   ERDT_DONNEES_RADT 		*pl_ListeRADT;

   XZST_03EcritureTrace(XZSTC_FONCTION, "IN: er_controle_cb ");
		  
   /*A Recuperation des parametres du message XDM_ECMD_EQPT */
   if (!TipcMsgRead(pa_Data -> msg
                          ,T_IPC_FT_INT4,&vl_Etat
                          ,T_IPC_FT_STR, &vl_NomMachine
                          ,T_IPC_FT_INT4,&vl_TypeEqt
                          ,T_IPC_FT_INT4,&vl_NumEqt
	       		  ,NULL))
	       
   {      
      /* Trace : Impossibilite de lire le message */
      XZST_03EcritureTrace(XZSTC_WARNING,"message XDM_ECMD_EQPT non lu \n" );
   }
	      
   /* Trace du message en entree de la fonction */
   XZST_03EcritureTrace(XZSTC_INFO ,"callback er_controle_cb, msg= %d %s %d %d",
                          vl_Etat,
                          vl_NomMachine,
                          vl_TypeEqt,
                          vl_NumEqt);
   
      /*A 
      ** Si l'equipement est une station RADT 
      */
      
      if ( vl_TypeEqt == XDC_EQT_RAD )
      {
         /*B 
         ** Recherche du No d'equipement dans la structure config_RADT en memoire 
         */
         
         for ( pl_ListeRADT = pg_debutRADT; pl_ListeRADT != NULL ; 
	         pl_ListeRADT = pl_ListeRADT->Suivant )
	 {
	    if (pl_ListeRADT->ConfRADT.NumeroStation == vl_NumEqt)
	    {
      	       if ( vl_Etat > XZEXC_LIMITE_ETAT_EQT )
      	          pl_ListeRADT->Etat_Service &= vl_Etat;
      	       else
      	          pl_ListeRADT->Etat_Service |= vl_Etat;
 	       break;
    	    }
	 }
      }   
   
   /* Trace des Differents etats service et activite */
   for ( pl_ListeRADT = pg_debutRADT; pl_ListeRADT != NULL ; pl_ListeRADT = pl_ListeRADT->Suivant )
   {
      XZST_03EcritureTrace(XZSTC_DEBUG1,"er_controle_cb : serrdt :%d ",pl_ListeRADT->Etat_Service);
   }
   
   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: er_controle_cb ");
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*  Changement de contexte.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	er_journuit ( XZEAT_JourNuit va_contexte, int va_resultat)
 
/*
* ARGUMENTS EN ENTREE :
*  
*
* ARGUMENTS EN SORTIE :
*   vl_resultat
*
*
* CODE RETOUR : 
*   aucun
*
* CONDITION D'UTILISATION
*
* FONCTION 
* Met a jour la variable vg_contexte
*
------------------------------------------------------*/
{
   static char *version = "$Id: erdt_cmd.c,v 1.32 2021/01/29 15:05:25 akkaakka Exp $      : er_journuit  " ;

   XZST_03EcritureTrace(XZSTC_FONCTION, "IN: er_journuit ");
   /*A si le contexte recu est jour : contexte = jour sinon contexte = nuit */
   if (va_contexte.JourNuit==XDC_JOUR)
   {
      vg_Contexte=XDC_JOUR;
   }
   else
   {
      vg_Contexte=XDC_NUIT;
   }
   XZST_03EcritureTrace(XZSTC_INFO,"Nouveau contexte : %d",vg_Contexte );
   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: er_journuit ");
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*  Etat de liaison avec les autres sites.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	er_etat_liaison ( XZSET_EComm_Site va_Liaison )
 
/*
* ARGUMENTS EN ENTREE :
*  
*
* ARGUMENTS EN SORTIE :
*   
*
*
* CODE RETOUR : 
*   aucun
*
* CONDITION D'UTILISATION
*
* FONCTION 
* Met a jour la variable vg_Etat_Liaison_CI
*
------------------------------------------------------*/
{
   static char *version = "$Id: erdt_cmd.c,v 1.32 2021/01/29 15:05:25 akkaakka Exp $      : er_etat_liaison  " ;

XDY_Datagroup   	pl_NomDG;
int             	vl_ValRet = XDC_OK;
T_BOOL          	vl_EtatDg = FALSE;
ERDT_DONNEES_RADT       *pl_listeRADT;

   /*A si le contexte recu est jour : contexte = jour sinon contexte = nuit */
   if ( !strcmp (va_Liaison.Site, XDC_NOM_SITE_CI) && (va_Liaison.Etat == XDC_NOK) )
   {
      vg_Etat_Liaison_CI = ERDTC_LIAISON_CI_HS;
      for ( pl_listeRADT = pg_debutRADT; pl_listeRADT != NULL; pl_listeRADT = pl_listeRADT->Suivant)
      {
         /*A Abonnement au Datagroup XDG_EMES_RADT_6mn_R<NumeroSensVoie> */
         /*A Construction du nom du DataGroup */
         sprintf ( pl_NomDG, "%s_R%d", XDG_ACKNOLEDGE, pl_listeRADT->ConfRADT.NumeroStation ) ; 
         /*A Disjoindre ce datagroup s'il est deja joint */
         TipcSrvSubjectGetSubscribe ( pl_NomDG, &vl_EtatDg );
         if ( vl_EtatDg )
         {
            if ( !XDG_QuitterDG( pl_NomDG ) )
            {
               /*B Ecriture Trace */
               XZST_03EcritureTrace ( XZSTC_WARNING, "Echec desabonnement au Datagroup %s ", pl_NomDG);
               vl_ValRet = XDC_NOK;
            }
            else
            {
               XZST_03EcritureTrace ( XZSTC_DEBUG1, "Desabonnement au Datagroup %s ", pl_NomDG );
            }
         }
      }
      XZST_03EcritureTrace(XZSTC_WARNING,"Liaison rompue avec le CI : %d",vg_Etat_Liaison_CI);
   }
   if ( !strcmp (va_Liaison.Site, XDC_NOM_SITE_CI) && (va_Liaison.Etat == XDC_OK) )
   {
      vg_Etat_Liaison_CI = ERDTC_LIAISON_CI_OK;
      for ( pl_listeRADT = pg_debutRADT; pl_listeRADT != NULL; pl_listeRADT = pl_listeRADT->Suivant)
      {
         /*A Abonnement au Datagroup XDG_EMES_RADT_6mn_R<NumeroSensVoie> */
         /*A Construction du nom du DataGroup */
         sprintf ( pl_NomDG, "%s_R%d", XDG_ACKNOLEDGE, pl_listeRADT->ConfRADT.NumeroStation ) ; 
         /*A Joindre ce datagroup s'il n'est pas deja joint */
         TipcSrvSubjectGetSubscribe ( pl_NomDG, &vl_EtatDg );
         if ( !vl_EtatDg )
         {
            if ( !XDG_JoindreDG( pl_NomDG ) )
            {
               /*B Ecriture Trace */
               XZST_03EcritureTrace ( XZSTC_WARNING, "Echec abonnement au Datagroup %s ", pl_NomDG);
               vl_ValRet = XDC_NOK;
            }
            else
            {
               XZST_03EcritureTrace ( XZSTC_DEBUG1, "Abonnement au Datagroup %s ", pl_NomDG );
            }
         }
      }
      XZST_03EcritureTrace(XZSTC_INFO,"Liaison etablie avec le CI : %d",vg_Etat_Liaison_CI);
   }
   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: er_etat_liaison ");
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande de fenetre LCR
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	er_CMD_LCR ( T_IPC_CONN				va_Cnx,
		     T_IPC_CONN_PROCESS_CB_DATA		data,
		     T_CB_ARG				arg)
		     
		     
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
*  XDM_ELCR
*
* FONCTION 
*  Demande fenetre LCR
*
------------------------------------------------------*/
{
static char *version = "$Id: erdt_cmd.c,v 1.32 2021/01/29 15:05:25 akkaakka Exp $      : er_CMD_LCR";


#ifndef _TIBCO_EMS   
   T_STR			vl_FicCmdLcr;
   T_STR			vl_FicResLcr;
   T_STR			vl_NomMachine;
   T_INT2		vl_Periode;
   T_REAL8		vl_HrDebut;
   T_REAL8		vl_HrFin;
   T_INT2		vl_TypeEqt;
   T_INT2		vl_NoEqt;
#else
   XDY_FichEquext		vl_FicCmdLcr;
   XDY_FichEquext		vl_FicResLcr;
   XDY_NomMachine		vl_NomMachine;
   XDY_Mot		vl_Periode;
   XDY_Horodate		vl_HrDebut;
   XDY_Horodate		vl_HrFin;
   XDY_Mot		vl_TypeEqt;
   XDY_Mot		vl_NoEqt;
#endif   


   ERDT_DONNEES_RADT		*pl_ListeRADT;
   INFO_EQT_FEN_LCR	        pl_eqt,*pl_ListeEqt;
   XZEXT_MSG_SOCKET		pl_MsgTimer;
   
   XZST_03EcritureTrace(XZSTC_FONCTION,"IN Callback ep_CMD_LCR" );
   	/*A Lecture du message */
   	if(!TipcMsgRead(data->msg,
   			T_IPC_FT_INT2,	&vl_TypeEqt,
   			T_IPC_FT_INT2,	&vl_NoEqt,
   			T_IPC_FT_STR,	&vl_FicCmdLcr,
   			T_IPC_FT_STR,	&vl_FicResLcr,
   			T_IPC_FT_INT2,	&vl_Periode,
   			T_IPC_FT_REAL8,&vl_HrDebut,
   			T_IPC_FT_REAL8,&vl_HrFin,
   			T_IPC_FT_STR,	&vl_NomMachine,
	          	NULL))
      	{       
      	   /* Trace : Impossibilite de lire le message */
           XZST_03EcritureTrace(XZSTC_WARNING, "messsage XDM_ELCR non lu" );
           XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: ep_CMD_LCR");
           return;
	}
	      
        /*A Construction de la liste globale des eqt a piloter par la fenetre LCR */
		
        pl_eqt.TypeEqt=vl_TypeEqt;
        strcpy(pl_eqt.NomFichCmdesLCR,vl_FicCmdLcr);
        strcpy(pl_eqt.NomFichResLCR,vl_FicResLcr);
        pl_eqt.Periode=vl_Periode;
        pl_eqt.HrDebut=vl_HrDebut;
        pl_eqt.HrFin=vl_HrFin;
        strcpy(pl_eqt.NomMachine,vl_NomMachine);
        pl_eqt.Position=0;
        pl_eqt.Suivant=NULL;
        pl_eqt.NoEqt=vl_NoEqt;
        
        /* Recherche si pas de commande LCR en cours sur equipement concerne */
        if ( (vl_TypeEqt == XDC_EQT_RAD) &&
             (ex_test_autorisation_eqt ( &pg_DebutListeRADTFLcr, 
        				 &pl_eqt,
        				 XDC_EQT_RAD) == XDC_NOK) )
        {
           return;
        }

        /*A Recherche de l'eqt dans la liste */
	if (vl_TypeEqt == XDC_EQT_RAD)
	{
	   for ( pl_ListeRADT = pg_debutRADT; pl_ListeRADT != NULL ; 
	         pl_ListeRADT = pl_ListeRADT->Suivant )
	   {
	      if ( (pl_ListeRADT->ConfRADT.NumeroStation == vl_NoEqt) || (vl_NoEqt==XDC_EQTALL) )
	      {
		    if (!strcmp(pl_ListeRADT->ConfRADT.AdresseIP,XZECC_IP_VIDE))
        	      ex_init_lcr ( 	&pg_DebutListeRADTFLcr,
        	    			pl_eqt,
        	    			pl_ListeRADT->Socket,
        	    			pl_ListeRADT->ConfRADT.RGS,
        	    			pl_ListeRADT->ConfRADT.NumeroStation,
        	    			"RADT_FLcr_NoEqt:",
        	    			vg_SockTIM );
	      		else
			ex_init_lcr_ip(     &pg_DebutListeRADTFLcr,
					pl_eqt,
					pl_ListeRADT->Socket,
					pl_ListeRADT->ConfRADT.RGS,
					pl_ListeRADT->ConfRADT.NumeroStation,
					"RADT_FLcr_NoEqt:",
					vg_SockTIM,
					pl_ListeRADT->ConfRADT.AdresseIP,
					pl_ListeRADT->ConfRADT.Port);
              }
              if (pl_ListeRADT->ConfRADT.NumeroStation == vl_NoEqt)
                break;
	   }
           if ( ( pl_ListeRADT == NULL ) && (vl_NoEqt != XDC_EQTALL) )
           {
              XZEX02_Retour_LCR(XDC_NOK,vl_NomMachine);
	   }
	}
   	/* Fin du callback */
        XZST_03EcritureTrace(XZSTC_FONCTION,"OUT Callback ep_CMD_LCR" );
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*  Passage en Mode Lent.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void er_Mode_Lent_cb ( 	T_IPC_CONN 			va_Cnx,
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
*  XDM_MODE_LENT 
*
* FONCTION 
* Met a jour les variables d'etat dans les structures en memoire
*
------------------------------------------------------*/
{
   static char *version = "$Id: erdt_cmd.c,v 1.32 2021/01/29 15:05:25 akkaakka Exp $      :  er_Mode_Lent_cb" ;

#ifndef _TIBCO_EMS
   T_INT2			vl_Etat = XDC_FAUX;
#else
   XDY_Mot			vl_Etat = XDC_FAUX;
#endif

   XZST_03EcritureTrace(XZSTC_FONCTION, "IN: er_Mode_Lent_cb ");
		  
   /*A Recuperation des parametres du message XDM_MODE_LENT */
   if (!TipcMsgRead(pa_Data -> msg
                          ,T_IPC_FT_INT2,&vl_Etat
	       		  ,NULL))
	       
   {      
      /* Trace : Impossibilite de lire le message */
      XZST_03EcritureTrace(XZSTC_WARNING,"message XDM_MODE_LENT non lu \n" );
   }
	      
   /* Trace du message en entree de la fonction */
   XZST_03EcritureTrace(XZSTC_INFO,"callback er_Mode_Lent_cb, msg= %d", vl_Etat);
   
   vg_Mode_Lent = vl_Etat;
   
   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: er_Mode_Lent_cb ");

}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*  Acknoledge Envoi Mesures 6 Minutes Q V TO PL.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void er_Acknoledge_cb ( T_IPC_CONN 			va_Cnx,
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
*  XDM_ACKNOLEDGE 
*
* FONCTION 
* Met a jour les horodates d'historique
*
------------------------------------------------------*/
{
   static char *version = "$Id: erdt_cmd.c,v 1.32 2021/01/29 15:05:25 akkaakka Exp $      :  er_Acknoledge_cb" ;

   #ifndef _TIBCO_EMS
   T_INT2			vl_Num_RADT = 0;
   T_REAL8			vl_Horodate = 0.0;
   #else
   XDY_Mot			vl_Num_RADT = 0;
   XDY_Horodate	vl_Horodate = 0.0;
   #endif
   
   if (vg_mode_simu)
   return;

   XZST_03EcritureTrace(XZSTC_FONCTION, "IN: er_Acknoledge_cb %s ", version);
		  
   /*A Recuperation des parametres du message XDM_Acknoledge */
   if (!TipcMsgRead(pa_Data -> msg
                          ,T_IPC_FT_INT2,&vl_Num_RADT
                          ,T_IPC_FT_REAL8,&vl_Horodate
	       		  ,NULL))
	       
   {      
      /* Trace : Impossibilite de lire le message */
      XZST_03EcritureTrace(XZSTC_WARNING,"message XDM_Acknoledge non lu \n" );
   }
	      
   /* Trace du message en entree de la fonction */
   XZST_03EcritureTrace(XZSTC_INFO,"IN callback %s, msg= %d  %lf", version, vl_Num_RADT, vl_Horodate);
   
   /*A Envoi des donnees par chaussee => Appel de la fonction er_Envoi_Mesure */
   if ( er_Envoi_Mesure ( vl_Num_RADT, XDC_VRAI ) != XDC_OK )
       XZST_03EcritureTrace(XZSTC_WARNING,"%s : 6min QVTO : er_Envoi_Mesure retourne NOK", version);
 	   	 	   	
   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: %s ", version);

}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*  Acknoledge Envoi Mesures 6 Minutes Q V TO PL.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void er_Acknoledge_Pesage_cb ( 	T_IPC_CONN 			va_Cnx,
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
*  XDM_Acknoledge_Pesage 
*
* FONCTION 
* Met a jour les horodates d'historique du pesage
*
------------------------------------------------------*/
{
   static char *version = "$Id: erdt_cmd.c,v 1.32 2021/01/29 15:05:25 akkaakka Exp $      :  er_Acknoledge_Pesage_cb" ;

#ifndef _TIBCO_EMS
   T_INT2			vl_Num_RADT 	= 0;
   T_INT2			vl_TypePesage 	= 0;
   T_REAL8			vl_Horodate 	= 0.0;
#else
   XDY_Mot			vl_Num_RADT 	= 0;
   XDY_Mot			vl_TypePesage 	= 0;
   XDY_Horodate   vl_Horodate 	= 0.0;
#endif

   XZST_03EcritureTrace(XZSTC_FONCTION, "IN: %s ", version);
		  
   /*A Recuperation des parametres du message XDM_Acknoledge_Pesage */
   if (!TipcMsgRead(pa_Data -> msg
                          ,T_IPC_FT_INT2,&vl_Num_RADT
                          ,T_IPC_FT_REAL8,&vl_Horodate
                          ,T_IPC_FT_INT2,&vl_TypePesage
	       		  ,NULL))
	       
   {      
      /* Trace : Impossibilite de lire le message */
      XZST_03EcritureTrace(XZSTC_WARNING,"messsage XDM_Acknoledge_Pesage non lu" );
   }
	      
   /* Trace du message en entree de la fonction */
   XZST_03EcritureTrace(XZSTC_INFO,"IN callback %s, msg= %d  %lf", version, vl_Num_RADT, vl_Horodate, vl_TypePesage );
   
   /*A Envoi des donnees par chaussee => Appel de la fonction er_Envoi_Mesure */
   if ( er_Envoi_Mesure_Pesage ( vl_Num_RADT, XDC_VRAI, vl_TypePesage ) != XDC_OK )
       XZST_03EcritureTrace(XZSTC_WARNING,"%s : Pesage : er_Envoi_Mesure_Pesage retourne NOK", version);
 	   	 	   	
   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: %s ", version);

}
