/* Fichier : $Id: ervi_cmd.c,v 1.2 2020/11/03 18:12:28 pc2dpdy Exp $     Release : $Revision: 1.2 $      Date :  $Date: 2020/11/03 18:12:28 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TECFE * FICHIER ervi_cmd.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* c'est le modules des callbacks
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	14/11/18 : Creation RMVI DEM1315 1.1
* GR    09/07/2020 : EMS 1.2	
* LCL		20/12/21	: MOVIS P2 ems SAE-312
------------------------------------------------------*/

/* fichiers inclus */

#include "ervi_cmd.h"
#include "xzis.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

/* declaration de fonctions internes */

/* definition de fonctions externes */



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Message de contr�le.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	en_demande_cb ( 
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
   static char *version = "$Id: ervi_cmd.c,v 1.2 2020/11/03 18:12:28 pc2dpdy Exp $ : en_controle_cb  " ;

#ifndef _TIBCO_EMS
   T_STR			va_requete;
#else
   XDY_Nom                      va_requete;
#endif

 #ifndef _TIBCO_EMS  
   T_STR			va_autoroute;
#else
   XDY_NomAuto                  va_autoroute;
#endif
   
#ifndef _TIBCO_EMS
   T_STR		va_type;
   T_INT4			va_pr;
   T_INT2			va_sens;
#else
   XDY_TexteAlerte      va_type;
   XDY_Entier			va_pr;
   XDY_Mot			va_sens;
#endif

    char		vl_site[5]; 
    int vl_nb;
    XDY_Phrase tl_id[NB_MAX_ELMTS];
    XDY_Phrase tl_type[NB_MAX_ELMTS];
    T_INT4 tl_taux[NB_MAX_ELMTS];
    XDY_Phrase tl_position[NB_MAX_ELMTS];
    T_INT4 tl_vitesse[NB_MAX_ELMTS];
    T_INT4 tl_tdp[NB_MAX_ELMTS];
    T_INT4 tl_note[NB_MAX_ELMTS];
    T_INT4 tl_dlp[NB_MAX_ELMTS];
    char *pl_lexon;
    int vl_indice;
  int vl_status;
  char vl_Texte[100];
  char cmd_line[500];
  char vl_cmd[500];
  char vl_cmd_json[1000];
  char vl_NomFichier[255];
  FILE *vl_Fd; 
  char vl_Ligne[10000];
  int vl_NbElements;
  char vl_NomCle[100];
  char vl_ValeurCle[200];
  

   XZST_03EcritureTrace(XZSTC_FONCTION, "IN: en_demande_cb ");
		  
   /*A Recuperation des parametres du message XDM_RMVI_DEMANDE */
   if (!TipcMsgRead(pa_Data -> msg
                          ,T_IPC_FT_STR,&va_requete
                          ,T_IPC_FT_STR, &va_autoroute
                          ,T_IPC_FT_INT4,&va_pr
                          ,T_IPC_FT_INT2,&va_sens
                          ,T_IPC_FT_STR,&va_type
	       		  ,NULL))
	       
   {      
      /* Trace : Impossibilite de lire le message */
      XZST_03EcritureTrace(XZSTC_WARNING,"messsage XDM_RMVI_DEMANDE non lu." );
   }
	      
   /* Trace du message en entree de la fonction */
   XZST_03EcritureTrace(XZSTC_WARNING,"callback en_demande_cb, msg= %s %s %d %d %s",
                          va_requete,
                          va_autoroute,
                          va_pr,
                          va_sens,
                          va_type);
   
   /*extraction site de la requete= numero_plan-nom site*/
   pl_lexon = strtok(va_requete,"-");
   strcpy(vl_site, va_requete+1+strlen(pl_lexon));   

   /* generation de la commande json */
   sprintf(vl_cmd_json,"{\"autoroute\":\"%s\",\"idrequete\":\"%s-%s\",\"typeEvtInitial\":\"%s\",\"sens\":%d,\"pr\":%d}",
   		va_autoroute,
		va_requete,
		vl_site,
		va_type,
		va_sens,
		va_pr);

   /*appel WS*/
   sprintf( cmd_line, "curl -m %d -X POST -d '%s' %s > %s%s",   MAX_TIME_TRANSFER,
   								vl_cmd_json,
								ADRESSE_SERVEUR_WS,
								WORKPATH,
								WORKFILE_JSON );

   XZST_03EcritureTrace(XZSTC_WARNING,"commande envoyee : %s", cmd_line);
   system( cmd_line );

   strcpy(vl_Texte,"Initialisation chaine statut erreur");

   /* Analyser le contenu du fichier de reponse */
   sprintf(vl_NomFichier,"%s%s", WORKPATH, WORKFILE_JSON);
   if ((vl_Fd = fopen(vl_NomFichier, "r")) == NULL)
   {
       XZST_03EcritureTrace( XZSTC_WARNING, "Impossible d'ouvrir le fichier %s", vl_NomFichier );
       vl_status = STATUT_TIMEOUT;
       sprintf(vl_Texte, "Probleme de traitement de la reponse de serveur KOARIO");
   }
   else
   {
	   /* Lecture contenu fichier brut */
	   if ( fgets ( vl_Ligne, sizeof(vl_Ligne), vl_Fd ) != NULL )
	   {
	   	if (strstr( vl_Ligne, STATUT_STR_ERREUR ) != NULL )
		{
			vl_status = STATUT_ERREUR;

			/* libelle erreur a retourner */
			strcpy(vl_Texte, vl_Ligne);	
		}
		else if (strstr(vl_Ligne, STATUT_STR_SUCCES ) != NULL )
		{
			vl_status = STATUT_OK;
       			sprintf(vl_Texte, "Succes");

		}
		else
		{
			vl_status = STATUT_TIMEOUT;
       			sprintf(vl_Texte, "Erreur timeout webservice");
		}
	   }
	   else
	   {
	       XZST_03EcritureTrace( XZSTC_WARNING, "fichier %s vide : timeout", vl_NomFichier );
	       vl_status = STATUT_TIMEOUT;
	       sprintf(vl_Texte, "service de localisation indisponible");
	   }
	
	   fclose( vl_Fd );

	   /* si succes */
	   if (vl_status==STATUT_OK)
	   {
		/* deserialisation fichier json */
		sprintf( vl_cmd, "cat %s%s | python $HOME/exec/parser_json_rmvi.py", WORKPATH, WORKFILE_JSON);
   		XZST_03EcritureTrace(XZSTC_WARNING,"commande envoyee : %s", vl_cmd);
   		system( vl_cmd );
	
		/* lecture du fichier */
		sprintf( vl_NomFichier, "%s%s", WORKPATH, WORKFILE_TXT);
		if ((vl_Fd = fopen(vl_NomFichier, "r")) == NULL)
		{
      			XZST_03EcritureTrace( XZSTC_WARNING, "Impossible d'ouvrir le fichier %s", vl_NomFichier );
		} else
		{
			vl_indice = 0;

			while ( fgets ( vl_Ligne, sizeof(vl_Ligne), vl_Fd ) != NULL )
			      if ( vl_Ligne[0] != XDF_COMMENTAIRE )
			      {
					XZST_03EcritureTrace( XZSTC_WARNING, "vl_Ligne %s",vl_Ligne);

					vl_NbElements = sscanf( vl_Ligne, "%s %[^#]", vl_NomCle, vl_ValeurCle);
					vl_ValeurCle[strlen(vl_ValeurCle)-1]='\0';
					
					XZST_03EcritureTrace( XZSTC_WARNING, "Lecture Cle [%s] Valeur [%s] vl_indice [%d]", vl_NomCle, vl_ValeurCle, vl_indice);
					if (vl_NbElements == 2)
					{
						if (!strcmp(vl_NomCle, "position"))
						{
							strcpy(tl_position[vl_indice], vl_ValeurCle); 
						}
						else
						if (!strcmp(vl_NomCle, "tempsParcours"))
						{
							tl_tdp[vl_indice] = atoi(vl_ValeurCle);
						}
						else
						if (!strcmp(vl_NomCle, "idintervenant"))
						{
							strcpy(tl_id[vl_indice], vl_ValeurCle);
						}
						else
						if (!strcmp(vl_NomCle, "typeIntervenant"))
						{
							strcpy(tl_type[vl_indice], vl_ValeurCle);
						}
						else
						if (!strcmp(vl_NomCle, "tauxFiabilite"))
						{
							tl_taux[vl_indice] = atoi(vl_ValeurCle);
						}
						else 
						if (!strcmp(vl_NomCle, "vitesse"))
						{
							tl_vitesse[vl_indice] = atoi(vl_ValeurCle);
						}
						else
						if (!strcmp(vl_NomCle, "note"))
						{
							tl_note[vl_indice] = atoi(vl_ValeurCle);
							vl_indice++;
						}
						else
						if (!strcmp(vl_NomCle, "dlp"))
						{
							/* plus utilise */
							tl_dlp[vl_indice] = 0;
						}
					}

				}

			fclose( vl_Fd );

			XZST_03EcritureTrace( XZSTC_WARNING, "Nombre de vehicules lus : %d", vl_indice);


	   		/*envoi liste vehicules*/
			if (vl_indice>0)
			{
	   			if (XZIS52_Vehicules_RMVI(vl_site, va_requete, vl_indice, tl_id, tl_type, tl_tdp, tl_taux, tl_position, tl_vitesse, tl_dlp, tl_note) != XDC_OK) 
				{
	   				XZST_03EcritureTrace(XZSTC_WARNING,"pb appel a XZIS52_Vehicules_RMVI");
				}

				XZST_03EcritureTrace( XZSTC_WARNING, "Fonction XZIS52_Vehicules_RMVI appelee");
			}
			else
			{
				strcpy(vl_Texte, "Aucun vehicule trouve");
				vl_status = STATUT_EMPTY;
			}
	   	}
	    }
	}

	/*envoi retour status*/
   	if (XZIS51_Status_RMVI(vl_site, vl_status, vl_Texte) != XDC_OK) {
		XZST_03EcritureTrace(XZSTC_WARNING,"pb appel a XZIS51_Status_RMVI");
   }

   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: en_demande_cb ");
}
