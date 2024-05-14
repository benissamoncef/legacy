/*
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEPMV * FICHIER ewsp_cyc.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Module de gestion cyclique de la tache TEWSP
*
------------------------------------------------------
* HISTORIQUE :
*
* VR	19/12/11	: Creation (DEM/1016)
* JMG	12/06/19 	: ODI_C DEM1338 1.2
* LCL   12/06/19        ODI-C DEM1338 1.3
* LCL	06/09/19	LOT27 COCPIT DEM1351 1.4 1.5
* ABE	26/02/20	LOT27 COCKPIT DEM1351 1.6
* GR    09/07/2020      EMS 1.7
* ABE	12/08/2020	Ajout taitement donn�es 6min TDP FCD DEM-SAE152 1.8				
* ABE	23/09/2020	Appel XZEZ12 � la place de directement XZAO960 pour ajout des donn�e dans TDP_FCD DEM-SAE152 1.9
* ABE	07/10/2020	Ajout trace pour probl�me blocage sur ODI suite � DEM-SAE152 1.12
* ABE	02/12/2020	Ajout test du retour du fopen pour ODI DEM-SAE130 1.13
* ABE	05/01/2021	ODI modification boucle else if pour passer toujours dans le fclose DEM-SAE130 1.14
* ABE	19/01/2021	conversion horodate dans le nom du fichier push_gateway vers le bon format pour syabase dem-SAE152 1.15
* LCL		20/12/21	: MOVIS P2 ems SAE-312
* PNI	13/04/23	: Manque de données en base, filtre sur autroutes SAE 469
* LCL	14/04/23	: Insertion directe des donnees en base sans passer par tacli suite pb de prod ems SAE-469 
* JPL	30/01/24	: Ecriture en base des donnees TDP FCD pour la N105
------------------------------------------------------*/

/* fichiers inclus */

#include "ewsp_cyc.h"
#include "xzaa.h"
#include "xzaac.h"
#include "xzaec.h"
#include "xzao960.h"

/* definitions de constantes */
#define NUM_EQUIPEMENT 94
#define PREFIXE_ID_ALERTE	"POLYVOX-WAZE"

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "%W 1.2 03/26/07 : ewsp_cyc" ;

/* declaration de fonctions internes */
int siteGestionFromNomAuto(char *va_autoroute, XDY_PR pr);
int siteGestionFromNumAuto(int va_NumAutoroute, XDY_PR pr);

void ewsp_odi_c(T_IPC_CONN va_Cnx,
		T_IPC_CONN_PROCESS_CB_DATA pa_data,
		T_CB_ARG	pa_Arg)
{
  #ifndef _TIBCO_EMS  
  T_STR vl_po ; 
  T_STR vl_portable ; 
  T_STR vl_depanneur ; 
  T_STR vl_horodate ; 
  T_INT2 vl_type_fmc;
#else
  XDY_NomEqt  vl_po ; 
  XDY_Tel vl_portable ; 
  XDY_Texte vl_depanneur ; 
  XDY_Texte vl_horodate ;
  short vl_type_fmc;
#endif


	char	vl_message[50];
	char vl_cmd_json[1000];
	int vl_erreur;
	char vl_Ligne[10000];
	char cmd_line[500];
	char vl_NomFichier[255];
	FILE *vl_Fd;
	int  vl_jour, vl_mois, vl_minute, vl_heure, vl_seconde, vl_annee;
	int vl_ret;

	XZST_03EcritureTrace(XZSTC_FONCTION, "ewsp_odi_c IN");

	if (!TipcMsgRead(pa_data->msg,
		T_IPC_FT_STR,&vl_po,
		T_IPC_FT_INT2, &vl_type_fmc,
		T_IPC_FT_STR,&vl_portable,
		T_IPC_FT_STR,&vl_depanneur,
		T_IPC_FT_STR,&vl_horodate,
		NULL))
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"pb decodage msg ODI_C");
		return;
	}
	else
		XZST_03EcritureTrace(XZSTC_WARNING,"msg ODI_C decode");

	XZST_03EcritureTrace(XZSTC_DEBUG1,"vl_po : [%s]", vl_po);
	XZST_03EcritureTrace(XZSTC_DEBUG1,"vl_portable : [%s]", vl_portable);
	XZST_03EcritureTrace(XZSTC_DEBUG1,"vl_depanneur : [%s]", vl_depanneur);
	XZST_03EcritureTrace(XZSTC_DEBUG1,"vl_horodate : [%s]", vl_horodate);

	sscanf(vl_horodate, "%2d/%2d/%4d %2d:%2d",
				&vl_jour,
				&vl_mois,
				&vl_annee,
				&vl_heure,
				&vl_minute);

	sprintf(vl_horodate, "%4d-%02d-%02dT%02d:%02d:00.000Z", vl_annee, vl_mois, vl_jour, vl_heure, vl_minute);
	XZST_03EcritureTrace(XZSTC_DEBUG1,"vl_horodate apres : [%s]", vl_horodate);


	/*appel CURL*/
	/* generation de la commande json */
	sprintf(vl_cmd_json,"{\"CodeEmetteur\":\"ESCOTA\",\"TypeMessage\":\"SMS\",\"IdentifiantDestinataire\":\"%s\",\"Langues\":[\"fr\",\"en\"],\"TypeFormulaire\":\"DEPANNAGE\",\"CibleSatisfaction\":\"%s\",\"DateIntervention\":\"%s\",\"Content\":{\"QuestionRappel\":\"true\"}}",
               vl_portable,
               vl_depanneur,
               vl_horodate);
 
	XZST_03EcritureTrace(XZSTC_DEBUG1,"vl_cmd_json : [%s]", vl_cmd_json);

  /*appel WS*/
  sprintf( cmd_line, "curl -m%d -X POST --header 'Content-Type: application/json' --header 'Accept: application/json' -d '%s' %s > %s%s",   
  								MAX_TIME_TRANSFER,
                                                               	vl_cmd_json,
                                                               	ADRESSE_SERVEUR_ODIC_WS,
                                                               	WORKPATH,
                                                               	WORKFILE_JSON );
  vl_erreur = 0;
  XZST_03EcritureTrace(XZSTC_WARNING,"commande envoyee : [%s]", cmd_line);

  system(cmd_line);
  sleep (1);
  XZST_03EcritureTrace(XZSTC_WARNING,"Fin call system");
  if (1)
  {
  	/* le serveur a repondu */
	/* erreur ou succes ? */
        /* Analyser le contenu du fichier de reponse */
        sprintf(vl_NomFichier,"%s%s", WORKPATH, WORKFILE_JSON);
	XZST_03EcritureTrace(XZSTC_WARNING,"avant fopen"		);
        if ((vl_Fd = fopen(vl_NomFichier, "r")) == NULL)	
        {
            XZST_03EcritureTrace( XZSTC_WARNING, "Impossible d'ouvrir le fichier %s (fopen==NULL)", vl_NomFichier );
  	    sprintf(vl_message,"Il y a eu une erreur dans l'envoi du SMS");
	    vl_erreur = 1;
        }
	else if (vl_Fd == -1)
	{
	    XZST_03EcritureTrace( XZSTC_WARNING, "Impossible d'ouvrir le fichier %s (fopen==-1)", vl_NomFichier );
  	    sprintf(vl_message,"Il y a eu une erreur dans l'envoi du SMS");
	    vl_erreur = 1;
	}
        else
        {
	    XZST_03EcritureTrace(XZSTC_WARNING,"fopen success");
            /* Lecture contenu fichier brut */
            if ( fgets ( vl_Ligne, sizeof(vl_Ligne), vl_Fd ) != NULL )
            {
                XZST_03EcritureTrace( XZSTC_WARNING, "Ligne lue [%s]", vl_Ligne );
                if (strstr(vl_Ligne, STATUT_STR_SUCCES ) != NULL )
                {
                    XZST_03EcritureTrace(XZSTC_WARNING,"ewsp_odi_c : cmd success");            
                       sprintf(vl_message, "en cas de succes inutile ce message n'est pas envoye au PO");
                }
                else
                {
                    if (strstr( vl_Ligne, STATUT_STR_ERREUR ) != NULL )
                    {
                            /* libelle erreur a retourner */        
                        sprintf(vl_message,"Il y a eu une erreur dans l'envoi du SMS");
                        vl_erreur = 1;
                    }
                    else
                    {
                        /* Dans le cas ou le fichier est ni succes ni erreur */
                           /* libelle erreur a retourner */
                        sprintf(vl_message,"Il y a eu une erreur dans l'envoi du SMS");
                        vl_erreur = 1;
                    }
                }
            }                
            else
            {
                /* Dans le cas ou le fichier est vide */
                /* libelle erreur a retourner */
                XZST_03EcritureTrace( XZSTC_WARNING, "Erreur systeme commande");
                sprintf(vl_message,"Il y a eu une erreur dans l'envoi du SMS");
                vl_erreur = 1;
            } 

            vl_ret = fclose( vl_Fd );
            XZST_03EcritureTrace(XZSTC_WARNING,"ewsp_odi_c : fclose ret =%d",vl_ret);
 							
	}
  }
  else
  {
        XZST_03EcritureTrace( XZSTC_WARNING, "Erreur systeme commande");
	/* la commande system a retourne une erreur */
  	sprintf(vl_message,"Il y a eu une erreur dans l'envoi du SMS");
	vl_erreur = 1;	
  }


  if (vl_erreur)
	{
		
		XZST_03EcritureTrace(XZSTC_WARNING,"ewsp_odi_c : Message erreur : %s",vl_message);
		XZIT23_Info_Message(vl_message,vl_po); 
	}

  XZST_03EcritureTrace(XZSTC_WARNING,"ewsp_odi_c : OUT");
}

/* definition de fonctions externes */
void ewsp_cyc_60()
{
  XDY_TexteAlerte vl_TexteAlerte;
  XDY_Texte vl_type_evt;
  char 	vl_id[10];
  char	vl_nom_autoroute[5];
  XDY_PR vl_PR_DEB;
  XDY_PR vl_PR_FIN;
  XDY_Sens vl_sens;
  XDY_Entier vl_tendance = -99; /*valeur forcee pour distinguer d un evt communautaire*/
  XDY_Entier vl_synthese = -99; /*valeur forcee pour distinguer d un evt communautaire*/
  XDY_Texte vl_Ligne;
  XDY_Texte vl_tmpLigne;
  XDY_Texte vl_buf;
  XDY_Texte vl_NomFichier;
  XDY_Texte vl_NomDossier;
  FILE* vl_Fd;
  int vl_NbElements;
  XDY_Texte vl_NomCle;
  XDY_Texte vl_ValeurCle;
  XDY_Booleen vl_envoiAlerte;
  int vl_pr_deb;
  int vl_distance_deb;
  int vl_pr_fin;
  int vl_distance_fin;
  int vl_probabilite=0;
  XDY_Datagroup pl_NomDg;
  XDY_NomSite pl_NomSite;
  int vl_TypeFMC;
  int vl_poste;
  XDY_Horodate vl_Horodate;
  int temp;
  DIR *d;
  struct dirent *dir;
  char *pos;

  XZST_03EcritureTrace( XZSTC_WARNING,"ewsp_cyc_60 : IN");

  strcpy(vl_NomDossier, VAL_CLE_DOSSIER_COCKPIT_PUSH);

  /*Ouvertur dossier push*/
  d=opendir(vl_NomDossier);
		
  if(d)
  {
	/* Pour chaque fichier */
	while ((dir=readdir(d)) !=NULL)
	{
		/* excepter . et .. */
		if( ( !strcmp(dir->d_name,".") || !strcmp(dir->d_name,"..") ))
		{
			continue; //Arret de l'it�ration sur fichier passage au suivant
		}
		
		/*cr�ation du path complet du fichier*/
		strcpy(vl_NomFichier,vl_NomDossier);
		strcat(vl_NomFichier,"/");
		strcat(vl_NomFichier,dir->d_name);
		XZST_03EcritureTrace( XZSTC_WARNING, "path fichier:%s\n",vl_NomFichier);

		vl_envoiAlerte = FALSE;

		if ((vl_Fd = fopen(vl_NomFichier, "r")) == NULL)
  		{
  			XZST_03EcritureTrace( XZSTC_WARNING, "Impossible d'ouvrir le fichier %s\n errno=%s", vl_NomFichier, strerror(errno) );
      		    	return ;
  		}

		/* reset vl_pr_fin et vl_distance_fin reseign� dans le fichier que si bouchon*/
		vl_distance_fin=0;
		vl_pr_fin=0;

		/* pour chacun des push recus*/
		while ( fgets ( vl_Ligne, sizeof(vl_Ligne), vl_Fd ) != NULL )
		{

  			if ( vl_Ligne[0] != XDF_COMMENTAIRE )
  			{
				vl_NbElements = sscanf( vl_Ligne, "%s %s",
						vl_NomCle,
						vl_ValeurCle);
				if(vl_NbElements==2)
					XZST_03EcritureTrace( XZSTC_WARNING, "Lecture Cle [%s] Valeur [%s]", vl_NomCle, vl_ValeurCle);

				if (!strcmp(vl_NomCle,"KO"))
				{
					XZST_03EcritureTrace( XZSTC_WARNING, "Erreur retournee par le webservice Polyvox : %s", vl_Ligne);
					return;
				}
		
				if ( vl_NbElements == 2 )
				{
					if (!strcmp(vl_NomCle, CLE_JSON_ID))
					{
						sscanf( vl_ValeurCle, "PolyVox_%[a-zA-Z]_P_%s", vl_buf, &vl_id );
					}
				
					else if (!strcmp(vl_NomCle, CLE_JSON_TYPE))
					{
						vl_envoiAlerte = TRUE;

						vl_TypeFMC = 0;

						if (!strcmp(vl_ValeurCle, VAL_CLE_JSON_VEH_ARR))
						{
							strcpy(vl_type_evt, "VehiculeArrete" );
							vl_TypeFMC = XZAEC_FMC_VehArrete;
						}
						else if (!strcmp(vl_ValeurCle, VAL_CLE_JSON_OBSTACLE))
						{
							strcpy(vl_type_evt, "Obstacle" );
							vl_TypeFMC = XZAEC_FMC_Obstacle;
						}
						else if (!strcmp(vl_ValeurCle, VAL_CLE_JSON_ACCIDENT))
						{
							strcpy(vl_type_evt, "Accident" );
							vl_TypeFMC = XZAEC_FMC_Accident;
						}
						else if (!strcmp(vl_ValeurCle, VAL_CLE_JSON_BOUCHON))
						{
							strcpy(vl_type_evt, "Bouchon" );
							vl_TypeFMC = XZAEC_FMC_QueueBouchon;
						}
						else
							vl_envoiAlerte = FALSE;

					
						XZST_03EcritureTrace( XZSTC_WARNING, "TypeFMC %d", vl_TypeFMC);
					
					}
					else if (!strcmp(vl_NomCle, CLE_JSON_AXE))
					{
						strcpy(vl_nom_autoroute, vl_ValeurCle); 
					}
					else if (!strcmp(vl_NomCle, CLE_JSON_PROBA))
					{
						if (vl_TypeFMC==XZAEC_FMC_QueueBouchon)
							vl_probabilite = XZAEC_FMC_CONFIRMEE;
						else	 
							vl_probabilite = XZAEC_FMC_SIGNALEE;

						 XZST_03EcritureTrace( XZSTC_WARNING, "Probabilite=%d", vl_probabilite);
							
					}
					else if (!strcmp(vl_NomCle, CLE_JSON_SENS))
					{
						if (!strcmp(vl_ValeurCle, VAL_CLE_JSON_SENS_SUD))
							vl_sens = XDC_SENS_SUD;
						else if (!strcmp(vl_ValeurCle, VAL_CLE_JSON_SENS_NORD))
							vl_sens = XDC_SENS_NORD;
						else if (!strcmp(vl_ValeurCle,VAL_CLE_JSON_SENS_INC))
							vl_sens = XDC_SENS_INCONNU;
					}
					else if (!strcmp(vl_NomCle, CLE_JSON_NUMPR_DEB))
					{
						sscanf( vl_ValeurCle, "%d", &vl_pr_deb );
					}
					else if (!strcmp(vl_NomCle, CLE_JSON_NUMPR_FIN))
					{
						sscanf( vl_ValeurCle, "%d", &vl_pr_fin );
						XZST_03EcritureTrace( XZSTC_WARNING, "pr_fin [%d]", vl_pr_fin );
					}
					else if (!strcmp(vl_NomCle, CLE_JSON_OFFSET_DEB))
					{
						sscanf( vl_ValeurCle, "%d", &vl_distance_deb );
						vl_PR_DEB = vl_pr_deb*1000+vl_distance_deb;
						XZST_03EcritureTrace( XZSTC_WARNING, "PR_DEB = [%d]", vl_PR_DEB );
					}
					else if (!strcmp(vl_NomCle, CLE_JSON_OFFSET_FIN))
					{
						sscanf( vl_ValeurCle, "%d", &vl_distance_fin );
						XZST_03EcritureTrace( XZSTC_WARNING, "pr_fin [%d]", vl_pr_fin );
						XZST_03EcritureTrace( XZSTC_WARNING, "vl_distance_fin: [%d]", vl_distance_fin );
						vl_PR_FIN = vl_pr_fin*1000+vl_distance_fin;
						XZST_03EcritureTrace( XZSTC_WARNING, "PR_FIN = [%d]", vl_PR_FIN );
					}
				}
				else
				{		
				/*Remove CrLf from line*/
					strcpy(vl_tmpLigne,vl_Ligne);
					if ((pos=strchr(vl_tmpLigne, '\n')) != NULL)
    						*pos = '\0';
					XZST_03EcritureTrace(XZSTC_WARNING, "Ligne <%s> incorrecte. Nb elements=%d" ,vl_tmpLigne,vl_NbElements );
				}		
			}
			else
				XZST_03EcritureTrace(XZSTC_WARNING, "Ligne <%s> incorrecte. Ligne[0]=%s" ,vl_Ligne[0] );
		}
  		fclose( vl_Fd );
		/*Supprimer le fichier polyvox traite*/
		if(strstr(vl_NomFichier,"polyvox"))
			remove(vl_NomFichier);

		if ( vl_envoiAlerte )
		{
			/* Si A57 inverser le sens */
			if (!strcmp(vl_nom_autoroute,"A57"))
			{
				if (vl_sens==XDC_SENS_NORD)
					vl_sens = XDC_SENS_SUD;
				else if (vl_sens==XDC_SENS_SUD)
					vl_sens = XDC_SENS_NORD;
			}


			XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME, &temp, &vl_Horodate);

			XZST_03EcritureTrace( XZSTC_WARNING, "vl_id = [%s]", vl_id );

			XZST_03EcritureTrace(XZSTC_WARNING,"Creer_Alerte_Cockpit %d %s %d %d %d", siteGestionFromNomAuto(vl_nom_autoroute, vl_PR_DEB), 
						vl_nom_autoroute,
                                                vl_PR_DEB,
						vl_PR_FIN,
                                                vl_sens);

		      	/* Ecriture d'une alerte dans le fichier des message RTworks */
		      	XZST_03EcritureTrace( XZSTC_DEBUG1, "Ecriture dans File Msg XDM_AA_alerte_cockpit %lf TypeEqt:%d NumEqt:%d Type:%d Texte:%s",
                   				vl_Horodate,
				                XDC_EQT_MAC,  
				                NUM_EQUIPEMENT,
				                XZAAC_ALERTE_COMMUNAUTAIRE,  	/*va_Type,*/
				                vl_TexteAlerte );
		      	
			/*preparation de la creation d'une alerte localisee*/
			sprintf(vl_TexteAlerte, "%s %s%s#%s %s-%d-%d.%03d#%d#%d#%d#%s %s-%d-%d.%03d", PREFIXE_ID_ALERTE, vl_id, vl_type_evt, "Autoroute", vl_nom_autoroute, vl_sens, vl_pr_deb, vl_distance_deb, vl_probabilite, vl_tendance, vl_synthese, "Autoroute", vl_nom_autoroute, vl_sens, vl_pr_fin, vl_distance_fin);

			
			XZST_03EcritureTrace( XZSTC_WARNING, "TexteAlerte:%s",vl_TexteAlerte);
			/* Recuperation du nom du site */
			XZSC_04NomSite ( pl_NomSite );
			/* Concatenation du DataGroup */
			XDG_EncodeDG2( pl_NomDg, XDG_AA, pl_NomSite );
			
			/* Determination poste operateur */
			if (siteGestionFromNomAuto(vl_nom_autoroute, vl_PR_DEB)==XDC_VC)
			{
				vl_poste=15; //VC
			}
			else
			{
				vl_poste=58; //DP
			}

      			/* Ecriture du Msg XDM_AA_alerte dans le fichier des message RTworks */

      			if ( !TipcSrvMsgWrite (        pl_NomDg,
        	      				XDM_IdentMsg(XDM_AA_alerte_cockpit),
                      				XDM_FLG_SRVMSGWRITE,
						T_IPC_FT_INT4, 	vl_TypeFMC,
						T_IPC_FT_INT4, 	vl_poste,
						T_IPC_FT_INT4,	siteGestionFromNomAuto(vl_nom_autoroute, vl_PR_DEB),
						T_IPC_FT_STR,   vl_id,
                        			T_IPC_FT_REAL8, vl_Horodate,
                                		T_IPC_FT_CHAR,  XDC_EQT_MAC,
                                        	T_IPC_FT_INT2,  NUM_EQUIPEMENT,
                                        	T_IPC_FT_CHAR,  XZAAC_ALERTE_COMMUNAUTAIRE,
                                        	T_IPC_FT_STR,   vl_TexteAlerte,
                                        	T_IPC_FT_STR,   vl_nom_autoroute,
                                        	T_IPC_FT_INT4, 	vl_PR_DEB,
                                        	T_IPC_FT_INT4, 	vl_PR_FIN,
                                        	T_IPC_FT_INT4, 	vl_sens,
						T_IPC_FT_INT4,	vl_probabilite,
						NULL
						))
      			{
                        	                   
         			XZST_03EcritureTrace( XZSTC_WARNING, "Erreur ecriture Msg XDM_AA_alerte DG:%s H:%lf TypeEqt:%d NumEqt:%d Type:%d",
                        	                   pl_NomDg,
                                                   vl_Horodate,
                                                   XDC_EQT_MAC,
                                                   NUM_EQUIPEMENT,
                                                   XZAAC_ALERTE_COMMUNAUTAIRE);

      			} 
			else 
			{
				XZST_03EcritureTrace(XZSTC_WARNING,"tewsp_cyc : ecriture success Horodate = %lf\n TypeEqt = %d\n Eqt = %d\n TypeAlerte = %d\n Texte de l alerte = %s\n IDCockpit= %s\n Autoroute=%s\n PRi deb=%d\n PR fin=%d\n Sens=%d\n:q Probabilite=%d\n",
								vl_Horodate,
								XDC_EQT_MAC,
								NUM_EQUIPEMENT,
								vl_TypeFMC,
								vl_TexteAlerte,
								vl_id,
								vl_nom_autoroute,
								vl_PR_DEB,
								vl_PR_FIN,
								vl_sens,
								vl_probabilite);


				XZST_03EcritureTrace(XZSTC_WARNING,"tewsp_cyc : ecriture success Poste = %d\n Site = %d\n ConfirmeeSignalee = %d",vl_poste,siteGestionFromNomAuto(vl_nom_autoroute, vl_PR_DEB), vl_probabilite);

			}
					
		}
		else
		{
			XZST_03EcritureTrace(XZSTC_FONCTION, "ABANDON ALERTE type evenement non envoye");
			/*Supprimer le fichier polyvox non trait�*/
			if(strstr(vl_NomFichier,"polyvox"))
				remove(vl_NomFichier);
		}
	}
	closedir(d);
	XZST_03EcritureTrace(XZSTC_WARNING, "Plus de fichier a traiter, fin de la fonction");
	XZST_03EcritureTrace( XZSTC_WARNING,"ewsp_cyc_60 : OUT");
	return;
  }
  else
  {
	XZST_03EcritureTrace(XZSTC_WARNING, "Impossible d'ouvrir le dossier %s\n errno=%s",vl_NomDossier,strerror(errno));
	XZST_03EcritureTrace( XZSTC_WARNING,"ewsp_cyc_60 : OUT");
	return;
  }

}


int siteGestionFromNomAuto(char *va_autoroute, XDY_PR va_pr)
{
        XZST_03EcritureTrace(XZSTC_FONCTION, "siteGestionFromNomAuto IN: va_autoroute=%s, va_pr=%d",  va_autoroute, va_pr);

        if (!strcmp(va_autoroute, "A51"))
                return XDC_DP;
        else if (!strcmp(va_autoroute, "A500"))
                return XDC_VC;
        else if (!strcmp(va_autoroute, "A57"))
	{
        	if (va_pr > 7700)
			return XDC_VC;
		else
			return XDC_DP;
	}
        else if (!strcmp(va_autoroute, "A8"))
        {
                if (va_pr > 57700)
                        return XDC_VC;
                else
                        return XDC_DP;
        }
        else return XDC_DP;

        XZST_03EcritureTrace(XZSTC_FONCTION, "siteGestionFromNomAuto OUT");
}		

int siteGestionFromNumAuto(int va_NumAutoroute, XDY_PR va_pr)
{
        XZST_03EcritureTrace(XZSTC_FONCTION, "siteGestionFromNumAuto IN: va_NumAutoroute=%d, va_pr=%d",  va_NumAutoroute, va_pr);

        if (va_NumAutoroute==5)		/* A51 */
                return XDC_DP;
        else if (va_NumAutoroute==2)  /*"A500*/
                return XDC_VC;
        else if (va_NumAutoroute==8)  /*"A57"*/
		{
        	if (va_pr > 7700)
			return XDC_VC;
		else
			return XDC_DP;
		}
        else if (va_NumAutoroute==1)  /*"A8*/
        {
                if (va_pr > 57700)
                        return XDC_VC;
                else
                        return XDC_DP;
        }
        else return XDC_DP;

        XZST_03EcritureTrace(XZSTC_FONCTION, "siteGestionFromNumAuto OUT");
}		

		
void ewsp_fcd()
{
	XDY_Texte vl_NomDossier;
	DIR *d;
	struct dirent *dir;
	XDY_Texte vl_NomFichier;
	FILE* vl_Fd;
	XDY_Texte vl_Ligne;
	char *ext;
	int vl_NbElements,vl_NbElements2;
	XDY_Texte vl_NomCle;
	XDY_Texte vl_ValeurCle;
	XDY_Texte vl_tmpLigne;
	char *pos;
	int temp;	
	int vl_jour, vl_mois, vl_minute, vl_heure, vl_seconde, vl_annee;
	static char vl_str[50]="";
	struct tm tm;
	char *vl_ch;

	/* variables de r�cup�ration des champs */
	char vl_traffic_color[16];

	/* variable temporaire */
	int vl_tmp_PR_Amont;
	int vl_tmp_Distance_PR_Amont;
	int vl_tmp_PR_Aval;
	int vl_tmp_Distance_PR_Aval;
								
	/* variable de transfert client */
	XDY_Horodate 	vl_Horodate_in;
        XDY_PR 		vl_PR1_in;
        XDY_PR 		vl_PR2_in; 
        XDY_Sens 	vl_Sens_in;		
		XDY_Sens 	vl_Sens;
        XDY_Entier	vl_Tdp_in;
		int 		vl_dummy;
        XDY_Octet 	vl_Validite_in;
        XDY_NomAuto vl_Autoroute_in;

	XZST_03EcritureTrace( XZSTC_WARNING,"ewsp_fcd : IN");

	strcpy(vl_NomDossier, VAL_CLE_DOSSIER_TDP_PUSH);

	/*Ouvertur dossier push*/
	d=opendir(vl_NomDossier);

	if(d)
	{
		/* Pour chaque fichier */
		while ((dir=readdir(d)) !=NULL)
		{
			/* excepter . et .. */
			if( ( !strcmp(dir->d_name,".") || !strcmp(dir->d_name,"..") ))
			{
				continue; //Arret de l'iteration sur fichier passage au suivant
			}
		
			/*cr�ation du path complet du fichier*/
			strcpy(vl_NomFichier,vl_NomDossier);
			strcat(vl_NomFichier,"/");
			strcat(vl_NomFichier,dir->d_name);
			ext = strrchr(dir->d_name,'.');

			if ((vl_Fd = fopen(vl_NomFichier, "r")) == NULL)
			{
				XZST_03EcritureTrace( XZSTC_WARNING, "ewsp_fcd : Impossible d'ouvrir le fichier %s\n errno=%s", vl_NomFichier, strerror(errno) );
				return ;
			}

			/* pour chacune des lignes du fichier*/
			while ( fgets ( vl_Ligne, sizeof(vl_Ligne), vl_Fd ) != NULL )
			{
				if ( vl_Ligne[0] != XDF_COMMENTAIRE )
				{
					if ( !strcmp(ext,".csv")) 
					{
/*						vl_NbElements = sscanf( vl_Ligne, "%[^;];%d;%d;%d;%d;%d;%d;%d;%d;%[^;]",
									vl_Autoroute_in,
									vl_Sens,
									&vl_tmp_PR_Amont,
									&vl_tmp_Distance_PR_Amont,
									&vl_tmp_PR_Aval,
									&vl_tmp_Distance_PR_Aval,
									&vl_Tdp_in,
									&vl_dummy,
									&vl_Validite_in,
									vl_traffic_color
									);
*/
						XZST_03EcritureTrace( XZSTC_DEBUG1,"ligne =%s",vl_Ligne);

						strcpy(vl_tmpLigne,vl_Ligne);
						vl_ch=strtok(vl_tmpLigne,";");
						strcpy(vl_Autoroute_in, vl_ch);
						vl_ch=strtok(NULL,";");
						vl_Sens=atoi(vl_ch);
						vl_ch=strtok(NULL,";");
						vl_tmp_PR_Amont=atoi(vl_ch);
						vl_ch=strtok(NULL,";");
						vl_tmp_Distance_PR_Amont=atoi(vl_ch);
						vl_ch=strtok(NULL,";");
						vl_tmp_PR_Aval=atoi(vl_ch);
						vl_ch=strtok(NULL,";");
						vl_tmp_Distance_PR_Aval=atoi(vl_ch);
						vl_ch=strtok(NULL,";");
						vl_Tdp_in=atoi(vl_ch);
						vl_ch=strtok(NULL,";");
						vl_dummy=atoi(vl_ch);
						vl_ch=strtok(NULL,";");
						vl_Validite_in=atoi(vl_ch);
						vl_ch=strtok(NULL,";");
						strcpy(vl_traffic_color, vl_ch);
						vl_NbElements=10;


						if(vl_NbElements==10)
						{
						
							/* r�cup�ration du sens */	
							if (vl_Sens==XDC_SENS_SUD)
								vl_Sens_in = XDC_SENS_SUD;
							else if (vl_Sens==XDC_SENS_NORD)
								vl_Sens_in = XDC_SENS_NORD;
							else  
								vl_Sens_in = XDC_SENS_INCONNU;


							XZST_03EcritureTrace( XZSTC_DEBUG1,"vl_Autoroute_in =%s",vl_Autoroute_in);
							XZST_03EcritureTrace( XZSTC_DEBUG1,"vl_Sens =%d",vl_Sens);
							XZST_03EcritureTrace( XZSTC_DEBUG1,"vl_Sens_in =%d",vl_Sens_in);
							XZST_03EcritureTrace( XZSTC_DEBUG1,"vl_tmp_PR_Amont =%d",vl_tmp_PR_Amont);
							XZST_03EcritureTrace( XZSTC_DEBUG1,"vl_tmp_Distance_PR_Amont =%d",vl_tmp_Distance_PR_Amont);
							XZST_03EcritureTrace( XZSTC_DEBUG1,"vl_tmp_PR_Aval =%d",vl_tmp_PR_Aval);
							XZST_03EcritureTrace( XZSTC_DEBUG1,"vl_tmp_Distance_PR_Aval =%d",vl_tmp_Distance_PR_Aval);
							XZST_03EcritureTrace( XZSTC_DEBUG1,"vl_Tdp_in =%d",vl_Tdp_in);
							XZST_03EcritureTrace( XZSTC_DEBUG1,"vl_Validite_in =%d",vl_Validite_in);
							XZST_03EcritureTrace( XZSTC_DEBUG1,"vl_traffic_color =%s",vl_traffic_color);

							/* Calcule du pr1 et du pr2 par addition de PR amont/aval et de la distance amont/aval */
							vl_PR1_in=vl_tmp_PR_Amont+vl_tmp_Distance_PR_Amont;
							vl_PR2_in=vl_tmp_PR_Aval+vl_tmp_Distance_PR_Aval;

							/* horodate doit provenir du nom du fichier */	
							memcpy(vl_str,dir->d_name,strlen(dir->d_name)-4);	
							//XZST_03EcritureTrace( XZSTC_WARNING,"horodate=%s",vl_str);							
							
							/* conversion horodate str en float */
							sscanf(vl_str,"%d-%d-%dT%d:%d:%d.csv",
										&vl_annee,
										&vl_mois,
										&vl_jour,
										&vl_heure,
										&vl_minute,
										&vl_seconde);

							sprintf(vl_str,"%02d/%02d/%04d %02d:%02d:%02d",
										vl_jour,	
										vl_mois,	
										vl_annee,
										vl_heure,
										vl_minute,
										vl_seconde);

							XZSM_13ConversionHorodateSec(vl_str,&vl_Horodate_in);
XZST_03EcritureTrace( XZSTC_WARNING,"ewsp_fcd : autoroute=%s",vl_Autoroute_in);
							/*XZST_03EcritureTrace( XZSTC_WARNING,"ewsp_fcd : Horodate = %f\tPR1 = %d\tPR2 = %d\tSens = %d\tTdp = %d\tValidite = %d\tAutoroute = %s\n",		
									vl_Horodate_in,
									vl_PR1_in,
									vl_PR2_in,
									vl_Sens_in,
									vl_Tdp_in,
									vl_Validite_in,
									vl_Autoroute_in);*/
							if ((!strcmp(vl_Autoroute_in,"A8")) || (!strcmp(vl_Autoroute_in,"A500")) || (!strcmp(vl_Autoroute_in,"A57"))  || (!strcmp(vl_Autoroute_in,"A51"))  || (!strcmp(vl_Autoroute_in,"A50"))  || (!strcmp(vl_Autoroute_in,"A501"))  || (!strcmp(vl_Autoroute_in,"A52"))  || (!strcmp(vl_Autoroute_in,"A520"))  || (!strcmp(vl_Autoroute_in,"A7"))  || (!strcmp(vl_Autoroute_in,"A9"))  || (!strcmp(vl_Autoroute_in,"A54"))  || (!strcmp(vl_Autoroute_in,"6007"))  || (!strcmp(vl_Autoroute_in,"6098"))  || (!strcmp(vl_Autoroute_in,"MC"))  || (!strcmp(vl_Autoroute_in,"N105")) )
							{
								/* Patch probleme de prod suite au decomissionnement de la gateway rtserver */
								/* perte de messages ems */
								/* On ecrit directement en base au lieu de passer par tacli */
								/*
								if (XZEZ12_Envoi_TDP_FCD (vl_Horodate_in, 
										    vl_PR1_in, 		
										    vl_PR2_in,   
										    vl_Sens_in,
										    vl_Tdp_in,      
										    vl_Validite_in, 
										    vl_Autoroute_in)  != XDC_OK)
								{
									XZST_03EcritureTrace( XZSTC_WARNING, "ewsp_fcd : Erreur appel XZEZ12_Envoi_TDP_FCD");
								}
								*/
								if (XZAO960_insert_TDP_FCD (vl_Horodate_in,
															vl_PR1_in, 		
															vl_PR2_in,   
															vl_Sens_in,
															vl_Tdp_in,      
															vl_Validite_in, 
															vl_Autoroute_in)  != XDC_OK)
								{
									XZST_03EcritureTrace(XZSTC_FONCTION,"ewsp_cyc : pb appel a XZAO960_insert_TDP_FCD");
								}

								
							}
						}
						else
						{
							/*Remove CrLf from line*/
							strcpy(vl_tmpLigne,vl_Ligne);
							if ((pos=strchr(vl_tmpLigne, '\n')) != NULL)
								*pos = '\0';
							XZST_03EcritureTrace(XZSTC_WARNING, "ewsp_fcd : Ligne <%s> incorrecte. Nb elements=%d" ,vl_tmpLigne,vl_NbElements );
						}
					}	
				}
				else		
				{
					XZST_03EcritureTrace(XZSTC_WARNING, "ewsp_fcd : Ligne <%s> incorrecte. Ligne[0]=%s" ,vl_Ligne[0] );
				}
			}
	
			/* toute les lignes trait�es */
			fclose( vl_Fd );

			/*Supprimer le fichier data traite*/
			if(strstr(vl_NomFichier,".csv"))
				remove(vl_NomFichier);
		}
		
		closedir(d);
		XZST_03EcritureTrace(XZSTC_WARNING, "ewsp_fcd : Plus de fichier a traiter, fin de la fonction");
		XZST_03EcritureTrace( XZSTC_WARNING,"ewsp_fcd : OUT");
		return;
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "ewsp_fcd : Impossible d'ouvrir le dossier %s\n errno=%s",vl_NomDossier,strerror(errno));
		XZST_03EcritureTrace( XZSTC_WARNING,"ewsp_fcd : OUT");
		return;
	}
}	
