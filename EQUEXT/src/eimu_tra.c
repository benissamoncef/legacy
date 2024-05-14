/*E*/
/* Fichier : $Id: eimu_tra.c,v 1.2 2021/05/03 13:43:21 pc2dpdy Exp $     Release : $Revision: 1.2 $      Date :  $Date: 2021/05/03 13:43:21 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEIMU * FICHIER eimu_tra.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* c'est le module de traitement des trames provenant de l'eqt
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	01/12/2020 creation 1.1
* ABE	02/02/2021 Ajout traitement tram IMU DEM-SAE155 1.2
* LCL		20/12/21	: MOVIS P2 ems SAE-312
------------------------------------------------------*/

/* fichiers inclus */

#include "eimu_tra.h"

/* definitions de constantes */
#define C_PIPE          "|"

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */
static char *version = "$Id: eimu_tra.c,v 1.2 2021/05/03 13:43:21 pc2dpdy Exp $ : eimu_tra.c" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Analyse de la trame >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int em_traitement_trame ( int va_socket )

/*
* ARGUMENTS EN ENTREE :
*   
* va_socket : no socket sur laquelle le message est arriv�
*
* ARGUMENTS EN SORTIE :
*
* CODE RETOUR : 
*
* CONDITION D'UTILISATION
*
* FONCTION 
*  analyse la trame et execute les actions correspondantes
*
------------------------------------------------------*/
{
   static char *version = "$Id: eimu_tra.c,v 1.2 2021/05/03 13:43:21 pc2dpdy Exp $ : em_traitement_trame" ;

   int				vl_IndCnx = 0,
   				vl_LgMsg  = 0;
   char				*pl_trame_ptr,*pl_trame_ps_ptr,*pl_PtrContraste;
   char				vl_entete1[10],
   				vl_entete2[10];
   int				vl_typetrame;
   char   			vl_TexteAlerte[10] = "";
   int 				vl_typ_rep;
   XZEXT_MSG_SOCKET		pl_trame, pl_TrameContraste;
   XZEXT_Msg_Socket 		vl_Msg; //structure message et len
   int				vl_param,i,j;
   int				vl_nbnonrep,
  				vl_nberrcrc;
   int				vl_etat;
   char				vl_rang[16] = "", 
   				vl_pos[16]  = "";
   EIMU_DONNEES_IMU		*pl_ListeIMU;
   EIMU_DONNEES_IMU		*pl_ListeIMU_Recherche_SousType;
   char				vl_zone_soustype_recherchee[1];
   XDY_Datagroup		pl_DG;
   int				vl_js;
   XDY_Horodate			vl_horodate;
   int                     vl_JourSemaine;
   int 				vl_nocmd;
   char * 			vl_status[32];
   char  			vl_tag[50];
   char  			vl_valeur[256];
   char  			*vl_buff;
   char				vl_strok[256];
   XDY_Mot	                vl_noeqt;
   int 				vl_val;
   static XZEXT_TEXTE_STATUS	vl_tabstatus[12];
   char *			vl_Mot;
   XDY_Mot	    		vl_Changement = XDC_FAUX,
   				vl_IMU_allume = XDC_FAUX,
   				vl_IMU_anc_allume = XDC_FAUX,
   				vl_EchecCmd = XDC_FAUX;
   XZSCT_NomTache          	vl_NomTache   = "";
   XDY_NomMachine          	vl_NomMachine = "";
   int				vl_valeur_int;
   XZEXT_Cmde_OPC 		vl_Cmde;
   

   XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME , &vl_JourSemaine, &vl_horodate);

   /*A
    *  Lecture du message recu dans la socket appelante
    */
 
   if ( ex_lire_msg ( va_socket, vl_Msg.msg, &vl_Msg.len, &vg_SockMask, &vl_IndCnx ) != XDC_OK )
   {
      /*  Ecriture trace Erreur lecture message socket.  */
      XZST_03EcritureTrace( XZSTC_WARNING, "em_traitement_trame : Erreur lecture message socket. " );
   }
   else
   {
	if (vl_IndCnx==XZEXC_DECONNEC) //Si lire msg a re�u un message de fin
	{
		XZST_03EcritureTrace( XZSTC_FATAL, "traitement trame OPC : Socket %d deconnecte�",va_socket);
		return XDC_NOK;		
	}
	/* parse de la trame */
	XZST_03EcritureTrace( XZSTC_WARNING, "traitement trame OPC : %s sur socket %d",vl_Msg.msg, va_socket);
      if (ecom_extraire_cmd_OPC(&vl_Msg.msg, &vl_Cmde, va_socket))
      {
	XZST_03EcritureTrace( XZSTC_WARNING, "Erreur extraire cmd. " );
	return;
      }
      

      sscanf(vl_Cmde.cmd, "%s %s",vl_tag, vl_valeur);
      vl_typetrame=vl_Cmde.type_msg;								
      if(vl_typetrame==XZEXC_TRAME_ETAT_IMU_INT)
	vl_valeur_int=atoi(vl_valeur);

	if (vg_BICE_en_cours_pilotage==-1)//retour de la phase d'init on ne pilote rien
	{
	 XZST_03EcritureTrace( XZSTC_WARNING, "Pas de BICE en cours de pilotage");
	 return;
	}

	XZST_03EcritureTrace( XZSTC_DEBUG1, " Msg Teopc :%s. type trame=%d vl_tag=%s value =%s, idopc=%s, IMU pilote:%d", vl_Cmde.cmd, vl_typetrame, vl_tag, vl_valeur,vl_Cmde.idBICE,vg_BICE_en_cours_pilotage);
      												
      /*A Recherche de la pal concern� */
      for ( pl_ListeIMU = pg_debutIMU; pl_ListeIMU != NULL ; pl_ListeIMU = pl_ListeIMU->Suivant )
      {
	 //if (pl_ListeIMU->Config.Numero==atoi(vl_Cmde.idBICE)) // on ne r�pond plus au BICE de la trame mais celui d�finit au d�but de commande dans eimu_cmd, pour traiter le cas Toulon
	 if (pl_ListeIMU->Config.Numero==vg_BICE_en_cours_pilotage)							
         {
	    vl_noeqt = pl_ListeIMU->Config.Numero;
            break;
         }
      }

      /* eqt non trouve */
      if ( pl_ListeIMU == NULL ) 
      {
         XZST_03EcritureTrace( XZSTC_WARNING, " IMU Non trouve en memoire mes=%s",vl_Msg.msg);
         return;
      }

      /*A Traitement des alarmes transmission avec l'equipement */
      if ( !(pl_ListeIMU->Etat_Service & XDC_EQT_HORS_SRV) )
           ex_traite_alarme_com ( vg_NomMachine,
				  XDC_EQT_IMU,
				  vl_noeqt,
      				  vl_typ_rep,
      				  vl_nbnonrep,
      				  vl_nberrcrc,
      				  vl_typetrame,
      				  vg_Mode_Fonct,
				  pl_ListeIMU->Config.SiteGestion);
      				  
      if ( (pl_ListeIMU->Etat_Service & XDC_EQT_HORS_SRV) && (vl_typetrame != XZEXC_TRAME_FEN_LCR) )
          return;
      
      /*A Recherche du type de trame */
      switch(vl_typetrame)
      {												
         /*A C'est une trame de reponse etat de positionnement pal */
         case XZEXC_TRAME_ETAT_IMU_STRING:
		if (!strcmp(vl_tag,EIMUC_TAG_MESSAGE_EN_COURS))
		{
			if (strcmp(vl_valeur,"")!=0) //Test si il y a un message en cours sur des zones
			{
				/* Parsage de chaine IMU de l'eqt vers le format SAE */
				strcpy(vl_strok,vl_valeur);
				vl_buff=strtok(vl_strok,":");
				strcpy(pl_ListeIMU->EtatCmd.ChaineIMU,vl_buff);
				XZST_03EcritureTrace(XZSTC_WARNING, "Commande IMU renvoyee par l'eqt :%s",pl_ListeIMU->EtatCmd.ChaineIMU);
				if (pl_ListeIMU->Config.SousType==1) //Cas tunnel TOULON
				{
					XZST_03EcritureTrace(XZSTC_WARNING, "Cas TOULON");
					for ( pl_ListeIMU_Recherche_SousType = pg_debutIMU; pl_ListeIMU_Recherche_SousType != NULL ; pl_ListeIMU_Recherche_SousType = pl_ListeIMU_Recherche_SousType->Suivant )
					{
						XZST_03EcritureTrace(XZSTC_WARNING, "Cas TOULON plsoustypeNum/Soustype=%d/%d, plIMU=%d",pl_ListeIMU_Recherche_SousType->Config.Numero,pl_ListeIMU_Recherche_SousType->Config.SousType,pl_ListeIMU->Config.Numero);
						if (pl_ListeIMU_Recherche_SousType->Config.SousType==1)
						{
							if (pl_ListeIMU_Recherche_SousType->Config.Numero == vg_BICE_en_cours_pilotage) //Si le premier des sous type 1 est celui qu'on traite
								if (vg_BICE_en_cours_pilotage==4)
									strcpy(vl_zone_soustype_recherchee,"1");
								else
									strcpy(vl_zone_soustype_recherchee,"2");				
							else
								strcpy(vl_zone_soustype_recherchee,"");				
							break;
						}
					}
					XZST_03EcritureTrace(XZSTC_WARNING, "Cas TOULON l'eqt est le %s de la liste",vl_zone_soustype_recherchee);
					for(vl_val=0;vl_val<EIMUC_NB_MAX_CMD_ZONE;vl_val++)
					{
						vl_buff=strtok(NULL,";");
						if (vl_buff==NULL)								
							break; //Fin de la chaineIMU
						if(!strcmp(vl_buff,vl_zone_soustype_recherchee))//cas la zone trouv�e est la zone recherch�e
						{
							strcat(pl_ListeIMU->EtatCmd.ChaineIMU,"|");
							strcat(pl_ListeIMU->EtatCmd.ChaineIMU,vl_buff);
							XZST_03EcritureTrace(XZSTC_WARNING, "Commande IMU renvoyee par l'eqt :%s",pl_ListeIMU->EtatCmd.ChaineIMU);
							break;
						}
						else 
						{
							vl_buff=strtok(NULL,":");
							if (vl_buff==NULL)
							{
								XZST_03EcritureTrace(XZSTC_WARNING, "Cas TOULON pas de message pour la zone%s",vl_zone_soustype_recherchee);
								strcpy(pl_ListeIMU->EtatCmd.ChaineIMU,"");//Pas de message, chaine vide
								break;
							}
							strcpy(pl_ListeIMU->EtatCmd.ChaineIMU,vl_buff); //On r�cup�re le message suivant
							vl_buff=strtok(NULL,";");
							if (vl_buff==NULL)
								break;
							strcat(pl_ListeIMU->EtatCmd.ChaineIMU,"|");
							strcat(pl_ListeIMU->EtatCmd.ChaineIMU,vl_buff);
							XZST_03EcritureTrace(XZSTC_WARNING, "Commande IMU renvoyee par l'eqt :%s",pl_ListeIMU->EtatCmd.ChaineIMU);
							break;
						}
					}												
				}
				else
				{
					for(vl_val=0;vl_val<EIMUC_NB_MAX_CMD_ZONE;vl_val++)
					{
						vl_buff=strtok(NULL,";");
						if (vl_buff==NULL)								
							break; //Fin de la chaineIMU
						strcat(pl_ListeIMU->EtatCmd.ChaineIMU,"|");
						strcat(pl_ListeIMU->EtatCmd.ChaineIMU,vl_buff);
						XZST_03EcritureTrace(XZSTC_WARNING, "Commande IMU renvoyee par l'eqt :%s",pl_ListeIMU->EtatCmd.ChaineIMU);				
						vl_buff=strtok(NULL,":");
					}
				}

			}
			else
				strcpy(pl_ListeIMU->EtatCmd.ChaineIMU,""); // pas de message en cours copie de chaine vide

				
				if (pl_ListeIMU->NumCmd!=0)
				{
					XDG_EncodeDG2 ( pl_DG, XDG_ETAT_IMU, 
								XZSC23_Nom_From_Numero(pl_ListeIMU->Config.SiteGestion));
					
		       			if (!TipcSrvMsgWrite ( pl_DG,
						XDM_IdentMsg(XDM_ETAT_IMU),
				  		XDM_FLG_SRVMSGWRITE,
				  		T_IPC_FT_INT2, pl_ListeIMU->Config.Numero,
				  		T_IPC_FT_STR,  pl_ListeIMU->EtatCmd.ChaineIMU,
				  		T_IPC_FT_INT4, pl_ListeIMU->Etat.Intervalle,
				  		T_IPC_FT_INT4, pl_ListeIMU->Etat.Cycles,
				  		T_IPC_FT_INT4, pl_ListeIMU->Etat.Duree,
						T_IPC_FT_INT4, pl_ListeIMU->NumCmd,
				  		NULL))
		       			{
		 	  			XZST_03EcritureTrace(XZSTC_WARNING,
								"Envoi message XDM_ETAT_IMU non effectue." );
		       			}
		       			else								
		       			{
		 	  			XZST_03EcritureTrace(XZSTC_WARNING,
							"Envoi message XDM_ETAT_IMU effectue. Sur XDG:%s",pl_DG);
						XZST_03EcritureTrace( XZSTC_DEBUG1,"pl numero=%d, ChaineIMU=%s Intervalle=%d, Cycle=%d, Duree=%d, NumCmd=%d",pl_ListeIMU->Config.Numero,pl_ListeIMU->EtatCmd.ChaineIMU,
									pl_ListeIMU->Etat.Intervalle,pl_ListeIMU->Etat.Cycles,pl_ListeIMU->Etat.Duree,pl_ListeIMU->NumCmd);
		       			}
				}
		}
		else if (!strcmp(vl_tag,EIMUC_TAG_NOM_MESSAGE))//TODO
		  	strcpy(pl_ListeIMU->Etat.ChaineIMU,vl_valeur);
/* EIMUC_TAG_NUMERO_MESSAGE devient EIMUC_TAG_NOM_MESSAGE */		
	 	break;

         case XZEXC_TRAME_ETAT_IMU_INT:				
		if (!strcmp(vl_tag,EIMUC_TAG_INTERVALLE))
		  pl_ListeIMU->Etat.Intervalle=vl_valeur_int;								
		else if (!strcmp(vl_tag,EIMUC_TAG_NB_CYCLES))
		  pl_ListeIMU->Etat.Cycles=vl_valeur_int;
		else if (!strcmp(vl_tag,EIMUC_TAG_DUREE))
		  pl_ListeIMU->Etat.Duree=vl_valeur_int;
		else if( !strcmp(vl_tag,EIMUC_TAG_RESET))
		{
			XZST_03EcritureTrace( XZSTC_WARNING, "TAG RESET");
			if(vl_valeur_int==2)
				pl_ListeIMU->StatusTR=EIMUC_STATUT_INIT_DONE;
			else if(vl_valeur_int==3)
				XZST_03EcritureTrace( XZSTC_WARNING, "Erreur retour tag%s value%d",vl_tag,vl_valeur_int);
		}								
		else if( !strcmp(vl_tag,EIMUC_TAG_ADD))
		{
			XZST_03EcritureTrace( XZSTC_WARNING, "TAG ADD");
			if(vl_valeur_int==2)
				pl_ListeIMU->StatusTR=EIMUC_STATUT_BUFFER_DONE;
			else if(vl_valeur_int==3)
				XZST_03EcritureTrace( XZSTC_WARNING, "Erreur retour tag%s value%d",vl_tag,vl_valeur_int);
		}
		else if( !strcmp(vl_tag,EIMUC_TAG_KILL))
		{
			XZST_03EcritureTrace( XZSTC_WARNING, "TAG KILL");
			if(vl_valeur_int==2)
				pl_ListeIMU->StatusTR=EIMUC_STATUT_STOP_DONE;
			else if(vl_valeur_int==3)
				XZST_03EcritureTrace( XZSTC_WARNING, "Erreur retour tag%s value%d",vl_tag,vl_valeur_int);
		}
		else if( !strcmp(vl_tag,EIMUC_TAG_KILLS))
		{
			XZST_03EcritureTrace( XZSTC_WARNING, "TAG KILLS");
			if(vl_valeur_int==2)
				pl_ListeIMU->StatusTR=EIMUC_STATUT_STOP_DONE;
			else if(vl_valeur_int==3)
				XZST_03EcritureTrace( XZSTC_WARNING, "Erreur retour tag%s value%d",vl_tag,vl_valeur_int);
		}
		else if( !strcmp(vl_tag,EIMUC_TAG_START))
		{
			XZST_03EcritureTrace( XZSTC_WARNING, "TAG START");
			if(vl_valeur_int==2)
			{	       			
				pl_ListeIMU->StatusTR=EIMUC_STATUT_DIFFU_DONE;
			}
			else if(vl_valeur_int==3)
				XZST_03EcritureTrace( XZSTC_WARNING, "Erreur retour tag%s value%d",vl_tag,vl_valeur_int);
		}
		else if( !strcmp(vl_tag,EIMUC_TAG_STARTS))
		{
			XZST_03EcritureTrace( XZSTC_WARNING, "TAG STARTS");
			if(vl_valeur_int==2)
			{	       			
				pl_ListeIMU->StatusTR=EIMUC_STATUT_DIFFU_DONE;
			}
			else if(vl_valeur_int==3)
				XZST_03EcritureTrace( XZSTC_WARNING, "Erreur retour tag%s value%d",vl_tag,vl_valeur_int);
		}
 		XZST_03EcritureTrace( XZSTC_WARNING, "IMU No:%d Statut=%d",pl_ListeIMU->Config.Numero,pl_ListeIMU->StatusTR);
	 	break;
      }
   }
}
