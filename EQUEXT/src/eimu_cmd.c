/*E*/
/* Fichier : $Id: eimu_cmd.c,v 1.3 2021/06/02 10:06:28 pc2dpdy Exp $     Release : $Revision: 1.3 $      Date :  $Date: 2021/06/02 10:06:28 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEIMU * FICHIER eimu_cmd.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* c'est le modules des callbacks
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	03/01/2020	creation				
* GR    09/07/2020      EMS 1.2
* ABE	02/02/2021	Modification DEM-SAE155 1.3
* ABE	02/06/2021	Ajout pilotage du tag EIMUC_TAG_NB_CYCLES, si tag EIMUC_TAG_DUREE nul et inversement DEM-SAE155 1.4
* LCL		20/12/21	: MOVIS P2 ems SAE-312
------------------------------------------------------*/

/* fichiers inclus */

#include "eimu_cmd.h"

/* definitions de constantes */
#define C_PIPE          "|"								
/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

/* declaration de fonctions internes */
int en_attendre_reponse(T_INT2 va_noeqt, char *va_tag_attendu);
void supprime_blancs_fin(char *);
/* definition de fonctions externes */





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
*  Pour touts les IMU , envoyer l'etat  par XDM_ETAT_LIAISON dans le DataGroup passe en parametre
*
------------------------------------------------------*/
{
   static char *version = "$Id: eimu_cmd.c,v 1.3 2021/06/02 10:06:28 pc2dpdy Exp $ :  ep_Init_IHM_Liaison_cb " ;

   #ifndef _TIBCO_EMS
   T_STR			pl_DGInit;
   #else
   XDY_Datagroup    pl_DGInit;
   #endif

   EIMU_DONNEES_IMU *		pl_ListeIMU;
   

   XZST_03EcritureTrace(XZSTC_FONCTION, "IN: ep_Init_IHM_Liaison_cb ");
		  
   /*A Recuperation des parametres du message  */
   if(!TipcMsgRead(pa_Data -> msg,
               T_IPC_FT_STR,  &pl_DGInit,
	       NULL))
   {       
      /* Trace : Impossibilite de lire le message */
      XZST_03EcritureTrace(XZSTC_WARNING, "OUT: ep_Init_IHM_Liaison_cb messsage XDM_EInit_IHM_Liaison non lu \n" );
      return;
   }
	        
   /*A Pour chaque IMU : envoi du msg XDM_ETAT_LIAISON  */
  
   for ( pl_ListeIMU = pg_debutIMU; pl_ListeIMU != NULL; pl_ListeIMU = pl_ListeIMU->Suivant )
   {
      ex_env_etat_liaison ( vg_NomMachine, pl_DGInit, pl_ListeIMU->Config.Numero, XDC_EQT_IMU ); 
   }  

   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: ep_Init_IHM_Liaison_cb ");
	       
}






/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande Init IHM IMU.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	en_Init_IHM_IMU_cb ( 
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
*  XDM_EInit_IHM_IMU 
*
* FONCTION 								
*  Pour touts les IMU, envoyer l'etat IMU par XDM_ETAT_IMU dans le DataGroup passe en parametre
*
------------------------------------------------------*/
{
   static char *version = "$Id: eimu_cmd.c,v 1.3 2021/06/02 10:06:28 pc2dpdy Exp $ : ep_Init_IHM_IMU_cb" ;

   #ifndef _TIBCO_EMS
   T_STR			pl_DGInit;
   #else
   XDY_Datagroup    pl_DGInit;
   #endif   

   EIMU_DONNEES_IMU *		pl_ListeIMU;
   int i;
   static char *		pl_Tab[XDC_Lon_Nb_Mod_Pal];

   XZST_03EcritureTrace(XZSTC_FONCTION, "IN: ep_Init_IHM_IMU_cb ");
		  
   /*A Recuperation des parametres du message XDM_EInit_IHM_IMU */
   if(!TipcMsgRead(pa_Data -> msg,
               T_IPC_FT_STR,  &pl_DGInit,
	       NULL))
   {       
      /* Trace : Impossibilite de lire le message */
      XZST_03EcritureTrace(XZSTC_WARNING,"OUT: ep_Init_IHM_IMU_cb messsage XDM_EInit_IHM_IMU non lu." );
      return;
   }    
	       
   /*A Pour chaque IMU : envoi du msg XDM_ETAT_IMU */
  
   for ( pl_ListeIMU = pg_debutIMU; pl_ListeIMU != NULL; pl_ListeIMU = pl_ListeIMU->Suivant )
   {
      /* si l'eqt est en service et actif */
      if ( !(pl_ListeIMU->Etat_Service & XDC_EQT_HORS_SRV) )
      {
     	 if (!TipcSrvMsgWrite ( pl_DGInit,
      			        XDM_IdentMsg(XDM_ETAT_IMU),
                          	XDM_FLG_SRVMSGWRITE,
                          	T_IPC_FT_INT2, pl_ListeIMU->Config.Numero,
                          	T_IPC_FT_STR,  pl_ListeIMU->EtatCmd.ChaineIMU,
                          	T_IPC_FT_INT4, pl_ListeIMU->EtatCmd.Intervalle,
                          	T_IPC_FT_INT4, pl_ListeIMU->EtatCmd.Cycles,
                          	T_IPC_FT_INT4, pl_ListeIMU->EtatCmd.Duree,
				T_IPC_FT_INT4, pl_ListeIMU->NumCmd,
                          	NULL))
      	 {
         	XZST_03EcritureTrace(XZSTC_WARNING,"Envoi message XDM_ETAT_IMU non effectue." );
	 }
	 else
      	 {
         	XZST_03EcritureTrace(XZSTC_MESSAGE,"Envoi message XDM_ETAT_IMU effectue." );
	 }
      }	
   }  
   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: ep_Init_IHM_IMU_cb ");
}





/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande de commande pal
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	en_cmd_IMU_cb ( 
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
* Fonction executee sur reception du message XDM_ECMD_IMU
*
* FONCTION 
*  Piloter le module concern�
*
------------------------------------------------------*/
{
	static char *version = "$Id: eimu_cmd.c,v 1.3 2021/06/02 10:06:28 pc2dpdy Exp $ : en_cmd_IMU_cb" ;

#ifdef _TIBCO_EMS
	XDY_Mot			vl_NoEqtIMU;
	char			vl_chaine_imu[1024];
	XDY_Entier		vl_intervalle;
	XDY_Entier		vl_nb_cycles;
	XDY_Entier		vl_duree;
	XDY_Entier		vl_NoCMD;
	XDY_NomMachine		vl_NomMachine;
#else
	T_INT2			vl_NoEqtIMU;
	T_STR			vl_chaine_imu;
	T_INT4			vl_intervalle;
	T_INT4			vl_nb_cycles;
	T_INT4			vl_duree;
	T_INT4			vl_NoCMD;
	T_STR			vl_NomMachine;
#endif

	char			vl_strok[256];
	T_STR			vl_message_imu;
	T_STR			vl_IdZone[EIMUC_NB_MAX_BICE]={NULL};
	EIMU_DONNEES_IMU 	*pl_listeIMU;
	XDY_Horodate		vl_horodate = (XDY_Horodate) 0.0;
	int                     vl_JourSemaine;
	T_INT4			vl_Taille = 0;
	char 			vl_str[200];
	T_INT2			vl_val=0;
	T_INT2			vl_val2=0;
	T_INT2			vl_val3=0;
	XDY_Booleen		vl_zone_a_rajouter=0;
	char 			*strtoken;
	int 			vl_SockRet = 0;
	int			vl_count=0;
	int			vl_nb_zone=0;
	char			vl_TAG_LANCEMENT[20];
	char			vl_TAG_ARRET[19];

	XZST_03EcritureTrace(XZSTC_FONCTION, "IN: en_cmd_IMU_cb");
	
	XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME , &vl_JourSemaine, &vl_horodate);
	  
	/*A Recuperation des parametres du message XDM_ECMD_IMU */
	if(!TipcMsgRead(pa_Data -> msg
		          ,T_IPC_FT_INT2, &vl_NoEqtIMU
		          ,T_IPC_FT_STR,  &vl_chaine_imu
		          ,T_IPC_FT_INT4, &vl_intervalle 
		          ,T_IPC_FT_INT4, &vl_nb_cycles 
		          ,T_IPC_FT_INT4, &vl_duree
		          ,T_IPC_FT_INT4, &vl_NoCMD
		          ,T_IPC_FT_STR,  &vl_NomMachine
		          ,NULL))
	{       
		/* Trace : Impossibilite de lire le message */
		XZST_03EcritureTrace(XZSTC_WARNING,"OUT: en_cmd_IMU_cb messsage XDM_ECMD_IMU non lu." );
		return;
	}
	else
	{
		supprime_blancs_fin(vl_chaine_imu);

		/* Trace du message */				
		XZST_03EcritureTrace(XZSTC_WARNING, "mes cmd imu = [%d] M:([%s] [%d] [%d] [%d]) cmd=[%d], machine=[%s]"
					     ,vl_NoEqtIMU
					     ,vl_chaine_imu
					     ,vl_intervalle
					     ,vl_nb_cycles
					     ,vl_duree
					     ,vl_NoCMD
					     ,vl_NomMachine);

	}

	/* Parsage de chaineIMU=Message|zone1|Zone2|... */
	strcpy(vl_strok,vl_chaine_imu);
	vl_message_imu=strtok(vl_strok,C_PIPE);
	for(vl_val=0;vl_val<EIMUC_NB_MAX_CMD_ZONE;vl_val++)
	{
		strtoken=strtok(NULL,C_PIPE);
		if (strtoken==NULL)								
			break; //Fin de la chaineIMU
		vl_IdZone[vl_val]=strtoken;
		XZST_03EcritureTrace(XZSTC_WARNING, "Zone[%d]=%s",vl_val,vl_IdZone[vl_val]);
	}
	vl_nb_zone=vl_val;

	/* V�rification Message en cours */
	/* if (tg_bices[0].NumBICE!=0) //Si il n'y a pas de diffusion en cours 
	{
	for(vl_val=0;vl_val<EIMUC_NB_MAX_CMD_ZONE;vl_val++) //Pour chaque �lement de tg_bices
	{
		if(vl_IdZone[vl_val]==NULL)//on arrive en fin de tableau, on sort
			break;		
	
		tg_bices[vl_val].NumBICE=atoi(vl_IdZone[vl_val]);
		strcpy(tg_bices[vl_val].NomMessage,vl_message_imu);													
		tg_bices[vl_val].Cycles=vl_nb_cycles;
		tg_bices[vl_val].Intervalle=vl_intervalle;
		tg_bices[vl_val].Duree=vl_duree;
		tg_bices[vl_val].Statut=EIMUC_STATUT_NON_COMMENCE;
	}
	}*/								
	/*else // Il y a des diffusions en cours 
	{
	for(vl_val=0;vl_val<EIMUC_NB_MAX_BICE;vl_val++) //Pour chaque zone de chaine IMU
	{
		if (vl_IdZone[vl_val]==NULL)
			break; //Fin de la chaineIMU
		for(vl_val2=0;vl_val2<EIMUC_NB_MAX_CMD_ZONE;vl_val++) //Pour chaque �lement de tg_bices
		{
			if(tg_bices[vl_val2].IdZone==0)//on arrive en fin de tableau, on sort
				break; 			

			//TODO On fait quoi si la zone est en diffusion d'un message diff�rent
			//TODO On fait quoi si il y a un message diff�rent dans une autre zone du BICE							

			if(vl_IdZone[vl_val]==tg_bices[vl_val2].IdZone) //Zone du message trouv�e dans tg_zone
			{
				vl_zone_a_rajouter=0;
				//TODO
				break;
			}
			else
				vl_zone_a_rajouter=1;
		}
		if (vl_zone_a_rajouter)												
		{				
			//On rajoute les zone dans tg_bices
			vl_zone_a_rajouter=0; //reset de bool
			for(vl_val3=0;vl_val3<EIMUC_NB_MAX_CMD_ZONE;vl_val++)
			{
				if (tg_bices[vl_val3].IdZone==0)
				{
					tg_bices[vl_val3].IdZone=vl_IdZone[vl_val];
					strcpy(tg_bices[vl_val3].Message,vl_message_imu);
					tg_bices[vl_val3].Cycles=vl_nb_cycles;
					tg_bices[vl_val3].Intervalle=vl_intervalle;
					tg_bices[vl_val3].Duree=vl_duree;
					tg_bices[vl_val3].Statut=EIMUC_STATUT_NON_COMMENCE;
				}
			}
		}
	}
	}*/

	/*A Verif des parametres en entree */

	for ( pl_listeIMU = pg_debutIMU; pl_listeIMU != NULL; pl_listeIMU = pl_listeIMU->Suivant )
	{
		if ( pl_listeIMU->Config.Numero == vl_NoEqtIMU) break;
	}

	/* Numero eqt pal invalide */
	if (pl_listeIMU == NULL)
	{
		/* message administrateur systeme */
		XZST_03EcritureTrace(XZSTC_WARNING, "OUT: en_cmd_IMU_cb Message XDM_ECMD_IMU recu avec no eqt pal invalide");
		xzat253_CompteRendu_IMU(vl_NoCMD,vl_horodate,XDC_NOK, pl_listeIMU->Config.SiteGestion);
		pl_listeIMU->NumCmd=0;
		return;
	}
	else
		pl_listeIMU->NumCmd=vl_NoCMD;

	/* Verif de l'etat d'activite et de l'etat de service */
	if ( pl_listeIMU->Etat_Service & XDC_EQT_HORS_SRV )
	{												
		XZST_03EcritureTrace(XZSTC_WARNING, "en_cmd_IMU_cb : Commande refuse car eqt hors service ");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: en_cmd_IMU_cb ");
		xzat253_CompteRendu_IMU(vl_NoCMD,vl_horodate,XDC_NOK, pl_listeIMU->Config.SiteGestion);
		pl_listeIMU->NumCmd=0;
		return;
	}

	// Renseignement de l'imu en cours de pilotage pour gestion du retour teimu_tra, sp�cificit� cas Toulon, on discute avec le BICE3 pour piloter l'imu 4
	vg_BICE_en_cours_pilotage=vl_NoEqtIMU;

	/* Sinon generer l'envoi des trames OPC pour lancer une diffusion */	

	sprintf(vl_str,"%s %d", EIMUC_TAG_ADD, 0);
	EnvoyerTrameOPC(     vl_str,
			 XZEXC_TRAME_CMD_IMU_INT,
			 pl_listeIMU->Config.NumeroEqtCommunication,
			 XZEXC_NOCMD,
			 XZEXC_PRIORITEFAIBLE,
			 pl_listeIMU->Socket );

	sprintf(vl_str,"%s %d", EIMUC_TAG_START, 0);
	EnvoyerTrameOPC(     vl_str,
			 XZEXC_TRAME_CMD_IMU_INT,
			 pl_listeIMU->Config.NumeroEqtCommunication,
			 XZEXC_NOCMD,
			 XZEXC_PRIORITEFAIBLE,
			 pl_listeIMU->Socket );

	sprintf(vl_str,"%s %d", EIMUC_TAG_STARTS, 0);
	EnvoyerTrameOPC(     vl_str,
			 XZEXC_TRAME_CMD_IMU_INT,
			 pl_listeIMU->Config.NumeroEqtCommunication,
			 XZEXC_NOCMD,
			 XZEXC_PRIORITEFAIBLE,
			 pl_listeIMU->Socket );

	sprintf(vl_str,"%s %d", EIMUC_TAG_RESET, 0);
	EnvoyerTrameOPC(     vl_str,
			 XZEXC_TRAME_CMD_IMU_INT,
			 pl_listeIMU->Config.NumeroEqtCommunication,
			 XZEXC_NOCMD,
			 XZEXC_PRIORITEFAIBLE,
			 pl_listeIMU->Socket );

	sprintf(vl_str,"%s %d", EIMUC_TAG_RESETS, 0);
	EnvoyerTrameOPC(     vl_str,
			 XZEXC_TRAME_CMD_IMU_INT,
			 pl_listeIMU->Config.NumeroEqtCommunication,
			 XZEXC_NOCMD,
			 XZEXC_PRIORITEFAIBLE,
			 pl_listeIMU->Socket );

	sprintf(vl_str,"%s %d", EIMUC_TAG_KILL, 0);
	EnvoyerTrameOPC(     vl_str,
			 XZEXC_TRAME_CMD_IMU_INT,
			 pl_listeIMU->Config.NumeroEqtCommunication,
			 XZEXC_NOCMD,
			 XZEXC_PRIORITEFAIBLE,
			 pl_listeIMU->Socket );

	sprintf(vl_str,"%s %d", EIMUC_TAG_KILLS, 0);
	EnvoyerTrameOPC(     vl_str,
			 XZEXC_TRAME_CMD_IMU_INT,
			 pl_listeIMU->Config.NumeroEqtCommunication,
			 XZEXC_NOCMD,
			 XZEXC_PRIORITEFAIBLE,
			 pl_listeIMU->Socket );

	if (pl_listeIMU->Config.SousType==0) //Gestion des cas autre que Toulon
	{
		sprintf(vl_str,"%s %d", EIMUC_TAG_RESET, 1);
		EnvoyerTrameOPC(     vl_str,
				 XZEXC_TRAME_CMD_IMU_INT,
				 pl_listeIMU->Config.NumeroEqtCommunication,
				 XZEXC_NOCMD,
				 XZEXC_PRIORITEFAIBLE,
				 pl_listeIMU->Socket );							

		/* Attente de la reponse RESET ==2 pour continuer */
		if(en_attendre_reponse(vl_NoEqtIMU,EIMUC_TAG_RESET))
		{
			// pas de tag RESET = 2, fin de la commande 
			XZST_03EcritureTrace(XZSTC_WARNING, "en_cmd_IMU_cb : erreur retour tag RESET");
			xzat253_CompteRendu_IMU(vl_NoCMD,vl_horodate,XDC_NOK, pl_listeIMU->Config.SiteGestion);
			pl_listeIMU->NumCmd=0;
			vg_BICE_en_cours_pilotage=-1;
			return;
		}
		XZST_03EcritureTrace(XZSTC_WARNING, "NEXT TAG RESET=2");
	}

	/* pour chaque zone concern�e par le message */
	for(vl_val=0;vl_val<vl_nb_zone;vl_val++)
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "MSG:%s Zone:%s Intervalle:%d Cyc:%d Duree:%d",vl_message_imu, vl_IdZone[vl_val], vl_intervalle,vl_nb_cycles,vl_duree);
		sprintf(vl_str,"%s %s", EIMUC_TAG_NOM_MESSAGE, vl_message_imu); //tg_bices[vl_val].NomMessage);
		EnvoyerTrameOPC(     vl_str,
				 XZEXC_TRAME_CMD_IMU_STRING,
				 pl_listeIMU->Config.NumeroEqtCommunication,
				 XZEXC_NOCMD,
				 XZEXC_PRIORITEFAIBLE,
				 pl_listeIMU->Socket );

		sprintf(vl_str,"%s %d",EIMUC_TAG_INTERVALLE, vl_intervalle);//tg_bices[vl_val].Intervalle);
		XZST_03EcritureTrace(XZSTC_WARNING,"vl_str=%s",vl_str);
		EnvoyerTrameOPC(     vl_str,
				 XZEXC_TRAME_CMD_IMU_INT,
				 pl_listeIMU->Config.NumeroEqtCommunication,
				 XZEXC_NOCMD,
				 XZEXC_PRIORITEFAIBLE,
				 pl_listeIMU->Socket );

		if (vl_duree==0) //Le pilotage du tag EIMUC_TAG_NB_CYCLES �crase le tag EIMUC_TAG_DUREE
		{
			sprintf(vl_str,"%s %d",EIMUC_TAG_NB_CYCLES, vl_nb_cycles);//tg_bices[vl_val].Cycles);
			XZST_03EcritureTrace(XZSTC_WARNING,"vl_str=%s",vl_str);
			EnvoyerTrameOPC(     vl_str,
					 XZEXC_TRAME_CMD_IMU_INT,
					 pl_listeIMU->Config.NumeroEqtCommunication,
					 XZEXC_NOCMD,
					 XZEXC_PRIORITEFAIBLE,
					 pl_listeIMU->Socket );
		}
		if (vl_nb_cycles ==0) //Le pilotage du tag EIMUC_TAG_DUREE �crase le tag EIMUC_TAG_NB_CYCLES
		{
			sprintf(vl_str,"%s %d",EIMUC_TAG_DUREE, vl_duree);//tg_bices[vl_val].Duree);
			XZST_03EcritureTrace(XZSTC_WARNING,"vl_str=%s",vl_str);
			EnvoyerTrameOPC(     vl_str,
					 XZEXC_TRAME_CMD_IMU_INT,
					 pl_listeIMU->Config.NumeroEqtCommunication,
					 XZEXC_NOCMD,
					 XZEXC_PRIORITEFAIBLE,
					 pl_listeIMU->Socket );
		}

		sprintf(vl_str,"%s %s",EIMUC_TAG_ID_ZONE, vl_IdZone[vl_val]);
		EnvoyerTrameOPC(     vl_str,
				 XZEXC_TRAME_CMD_IMU_INT,
				 pl_listeIMU->Config.NumeroEqtCommunication,
				 XZEXC_NOCMD,
				 XZEXC_PRIORITEFAIBLE,
				 pl_listeIMU->Socket );

		sprintf(vl_str,"%s %d", EIMUC_TAG_ADD, 1);
		EnvoyerTrameOPC(     vl_str,
				 XZEXC_TRAME_CMD_IMU_INT,
				 pl_listeIMU->Config.NumeroEqtCommunication,
				 XZEXC_NOCMD,
				 XZEXC_PRIORITEFAIBLE,
				 pl_listeIMU->Socket );

		/* Attente de la reponse ADD ==2 pour continuer */
		if(en_attendre_reponse(vl_NoEqtIMU,EIMUC_TAG_ADD))
		{
			// pas de tag ADD = 2, fin de la commande 
			XZST_03EcritureTrace(XZSTC_WARNING, "en_cmd_IMU_cb : erreur retour tag ADD");
			xzat253_CompteRendu_IMU(vl_NoCMD,vl_horodate,XDC_NOK, pl_listeIMU->Config.SiteGestion);
			pl_listeIMU->NumCmd=0;
			vg_BICE_en_cours_pilotage=-1;
			return;
		}
		XZST_03EcritureTrace(XZSTC_WARNING, "NEXT TAG ADD=2");
		sprintf(vl_str,"%s %d", EIMUC_TAG_ADD, 0);
		EnvoyerTrameOPC(     vl_str,
			 XZEXC_TRAME_CMD_IMU_INT,
			 pl_listeIMU->Config.NumeroEqtCommunication,
			 XZEXC_NOCMD,
			 XZEXC_PRIORITEFAIBLE,
			 pl_listeIMU->Socket );
	}


	/* Si demande d'arret (cmd <0) generer l'envoi des trames OPC pour arreter la diffusion */
	if (vl_NoCMD < XZEXC_NOCMD)
	{
		if (pl_listeIMU->Config.SousType==1) //Gestion du cas Toulon
			strcpy(vl_TAG_ARRET,EIMUC_TAG_KILLS);
		else
			strcpy(vl_TAG_ARRET,EIMUC_TAG_KILL);

		sprintf(vl_str,"%s %d", vl_TAG_ARRET, 1);
		EnvoyerTrameOPC(     vl_str,
		         XZEXC_TRAME_CMD_IMU_INT,
		         pl_listeIMU->Config.NumeroEqtCommunication,
		         XZEXC_NOCMD,
		         XZEXC_PRIORITEFAIBLE,
		         pl_listeIMU->Socket );
					
		/* Attente de la reponse KILL ==2 pour continuer */
		if(en_attendre_reponse(vl_NoEqtIMU,vl_TAG_ARRET))
		{
			// pas de tag RESET = KILL, fin de la commande 
			XZST_03EcritureTrace(XZSTC_WARNING, "en_cmd_IMU_cb : Arr�t commande en erreur : retour tag %s",vl_TAG_ARRET);
			xzat253_CompteRendu_IMU(vl_NoCMD,vl_horodate,XDC_NOK, pl_listeIMU->Config.SiteGestion);
			pl_listeIMU->NumCmd=0;
			vg_BICE_en_cours_pilotage=-1;
			return;
		}
	}												
	else 
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "test");
		/* Cas Toulon pour arr�ter les diffusion sur les zone concern� par la commande car on ne peux pas �craser une commande en cours */
		if (pl_listeIMU->Config.SousType==1)
		{
			sprintf(vl_str,"%s %d", EIMUC_TAG_KILLS, 1);
			EnvoyerTrameOPC(     vl_str,
				 XZEXC_TRAME_CMD_IMU_INT,
				 pl_listeIMU->Config.NumeroEqtCommunication,
				 XZEXC_NOCMD,
				 XZEXC_PRIORITEFAIBLE,
				 pl_listeIMU->Socket );
					
			/* Attente de la reponse KILL ==2 pour continuer */
			if(en_attendre_reponse(vl_NoEqtIMU,EIMUC_TAG_KILLS))
			{
				// pas de tag RESET = KILL, fin de la commande 
				XZST_03EcritureTrace(XZSTC_WARNING, "en_cmd_IMU_cb : Arr�t commande en erreur : retour tag %s",vl_TAG_ARRET);
				xzat253_CompteRendu_IMU(vl_NoCMD,vl_horodate,XDC_NOK, pl_listeIMU->Config.SiteGestion);
				pl_listeIMU->NumCmd=0;
				vg_BICE_en_cours_pilotage=-1;
				return;
			}
		}

		if (pl_listeIMU->Config.SousType==1) //Gestion du cas Toulon
			strcpy(vl_TAG_LANCEMENT,EIMUC_TAG_STARTS);
		else
			strcpy(vl_TAG_LANCEMENT,EIMUC_TAG_START);
		sprintf(vl_str,"%s %d", vl_TAG_LANCEMENT, 1);
		EnvoyerTrameOPC(     vl_str,
			         XZEXC_TRAME_CMD_IMU_INT,
			         pl_listeIMU->Config.NumeroEqtCommunication,
			         XZEXC_NOCMD,
			         XZEXC_PRIORITEFAIBLE,
			         pl_listeIMU->Socket );
		/* Attente de la reponse START ==2 pour continuer */
		if(en_attendre_reponse(vl_NoEqtIMU,vl_TAG_LANCEMENT))
		{
			// pas de tag START = 2, fin de la commande 
			XZST_03EcritureTrace(XZSTC_WARNING, "en_cmd_IMU_cb : erreur retour tag %s",vl_TAG_LANCEMENT);
			xzat253_CompteRendu_IMU(vl_NoCMD,vl_horodate,XDC_NOK, pl_listeIMU->Config.SiteGestion);
			pl_listeIMU->NumCmd=0;
			vg_BICE_en_cours_pilotage=-1;
			return;
		}
	}

	XZST_03EcritureTrace(XZSTC_WARNING,"succes commande du IMU %d cmd=%d",
		pl_listeIMU->Config.Numero,
		pl_listeIMU->NumCmd);
	xzat253_CompteRendu_IMU(vl_NoCMD,vl_horodate,XDC_OK, pl_listeIMU->Config.SiteGestion);
	  
/* Mise a jour memoire commande IMU */
strcpy ( pl_listeIMU->EtatCmd.ChaineIMU,vl_chaine_imu);
pl_listeIMU->EtatCmd.Cycles = vl_nb_cycles;
pl_listeIMU->EtatCmd.Intervalle = vl_intervalle;
pl_listeIMU->EtatCmd.Duree = vl_duree;
pl_listeIMU->NumCmd = (vl_NoCMD < XZEXC_NOCMD) ? 0 : vl_NoCMD;

   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: en_cmd_IMU_cb ");
}




/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Message de contr�le.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	en_controle_cb ( 
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
   static char *version = "$Id: eimu_cmd.c,v 1.3 2021/06/02 10:06:28 pc2dpdy Exp $ : en_controle_cb  " ;


#ifndef _TIBCO_EMS
    T_STR			va_NomMachine;
    T_INT4			va_Etat;
    T_INT4			va_TypeEqt;
    T_INT4			va_NumEqt;
#else
    XDY_NomMachine		va_NomMachine;
    XDY_Entier			va_Etat;
    XDY_Entier			va_TypeEqt;
    XDY_Entier			va_NumEqt;
#endif
   
   EIMU_DONNEES_IMU *		pl_ListeIMU;
   int				vl_Index;

   XZST_03EcritureTrace(XZSTC_FONCTION, "IN: en_controle_cb ");
		  
   /*A Recuperation des parametres du message XDM_ECMD_EQPT */
   if (!TipcMsgRead(pa_Data -> msg
                          ,T_IPC_FT_INT4,&va_Etat
                          ,T_IPC_FT_STR, &va_NomMachine
                          ,T_IPC_FT_INT4,&va_TypeEqt
                          ,T_IPC_FT_INT4,&va_NumEqt
	       		  ,NULL))
	       
   {      
      /* Trace : Impossibilite de lire le message */
      XZST_03EcritureTrace(XZSTC_WARNING,"messsage XDM_ECMD_EQPT non lu." );
   }
	      
   /* Trace du message en entree de la fonction */
   XZST_03EcritureTrace(XZSTC_WARNING,"callback ep_controle_cb, msg= %d %s %d %d",
                          va_Etat,
                          va_NomMachine,
                          va_TypeEqt,
                          va_NumEqt);
   
      /*A si l'eqt est une IMU */
      if (va_TypeEqt ==  XZECC_TYPE_EQT_IMU)
      {
         /*B Recherche du No d'equipement dans la structure config_IMU en memoire */
         for ( pl_ListeIMU = pg_debutIMU; pl_ListeIMU != NULL ; 
	         pl_ListeIMU = pl_ListeIMU->Suivant )
	 {
	    if (pl_ListeIMU->Config.Numero == va_NumEqt)
	    {
      	       if ( va_Etat > XZEXC_LIMITE_ETAT_EQT )
      	          pl_ListeIMU->Etat_Service &= va_Etat;
      	       else
      	          pl_ListeIMU->Etat_Service |= va_Etat;
	       break;
    	    }
	 }
      }   
   

   /* Trace des Differents etats service et activite */
   for ( pl_ListeIMU = pg_debutIMU; pl_ListeIMU != NULL ; 
	         pl_ListeIMU = pl_ListeIMU->Suivant )
   {
      XZST_03EcritureTrace(XZSTC_DEBUG1,"servIMU:%d ",pl_ListeIMU->Etat_Service);
   }
   
   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: en_controle_cb ");
}

int en_attendre_reponse(T_INT2 va_noeqt, char* va_tag_attendu)
{
	EIMU_DONNEES_IMU *pl_ListeIMU;
	int vl_SockRet=0;
	int count=0;
   	XZEXT_MASK		*pl_masque;

	XZST_03EcritureTrace(XZSTC_FONCTION, "en_attendre_reponse : IN");

	/*A Recherche de la pal concern� */
	for ( pl_ListeIMU = pg_debutIMU; pl_ListeIMU != NULL ; pl_ListeIMU = pl_ListeIMU->Suivant )
	{
		if (pl_ListeIMU->Config.Numero==va_noeqt)
		{
			va_noeqt = pl_ListeIMU->Config.Numero;
			break;
		}
	}
	if(pl_ListeIMU==NULL)
		return 1; //imu pas en cours

	while(count<10)
	{
		for ( pl_masque = vg_SockMask; pl_masque != NULL; pl_masque = pl_masque->Suivant )
		{
			XZST_03EcritureTrace(XZSTC_WARNING, "SocketMasque :%d",pl_masque->desc_sock);
		}
		if ( ex_attendre_msg ( XZEXC_SOCKET_NULL, &vg_SockMask, &vl_SockRet ) != XDC_OK )
		{
			XZST_03EcritureTrace(XZSTC_WARNING, "en_attendre_reponse : Probleme retour fonction attendre msg" );
			return;
		}
		else
		{
			XZST_03EcritureTrace(XZSTC_WARNING, "socket:%d statut:%d %s",vl_SockRet, pl_ListeIMU->StatusTR,va_tag_attendu);
			/* Appel traitement de la trame */
			if(vl_SockRet)
				em_traitement_trame( vl_SockRet );
		}
		
		if (!strcmp(va_tag_attendu,EIMUC_TAG_RESET) && pl_ListeIMU->StatusTR==EIMUC_STATUT_INIT_DONE)
			return 0; //Commande success
		else if (!strcmp(va_tag_attendu,EIMUC_TAG_ADD) && pl_ListeIMU->StatusTR==EIMUC_STATUT_BUFFER_DONE)
			return 0; //Commande success
		else if (!strcmp(va_tag_attendu,EIMUC_TAG_START) && pl_ListeIMU->StatusTR==EIMUC_STATUT_DIFFU_DONE)
			return 0; //Commande success
		else if (!strcmp(va_tag_attendu,EIMUC_TAG_STARTS) && pl_ListeIMU->StatusTR==EIMUC_STATUT_DIFFU_DONE)
			return 0; //Commande success
		else if (!strcmp(va_tag_attendu,EIMUC_TAG_KILL) && pl_ListeIMU->StatusTR==EIMUC_STATUT_STOP_DONE)
			return 0; //Commande success
		else if (!strcmp(va_tag_attendu,EIMUC_TAG_KILLS) && pl_ListeIMU->StatusTR==EIMUC_STATUT_STOP_DONE)
			return 0; //Commande success

		count++;
	}
	XZST_03EcritureTrace(XZSTC_FONCTION, "en_attendre_reponse : Time out attente");
	return 1; //Commande fail
}

void supprime_blancs_fin(char *Ligne)

/*
* ARGUMENTS EN ENTREE :
*       T_STR           Ligne;
*
* ARGUMENTS EN SORTIE :
*       T_STR           Ligne;
*
* CODE RETOUR :
*   aucun.
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction
*
* FONCTION
*   parcours de la chaine
*
------------------------------------------------------*/
{
        int     i=0, j=0, k=0;


        /*A on supprime les espaces de fin de ligne */
        k = strlen(Ligne);
	for (i=0,j=0; i<k;i++)
	{
		if ( Ligne[i] != ' ')
/*			return;*/
			break;
	}
	/*if ( j != k-1)*/	/* Il n'y a pas que des blancs */
	{
	i=k;
        for (j=i-1;j>=0;j--) {
                if ( Ligne[j] == ' ' )
                        Ligne[j] = XDC_FIN_CHN;
                else break;
        }
	}
}

