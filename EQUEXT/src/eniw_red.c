/*E*/
/*Fichier :  $Id: eniw_red.c,v 1.4 2021/01/29 14:02:46 akkaakka Exp $      Release : $Revision: 1.4 $        Date : $Date: 2021/01/29 14:02:46 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TENIW * FICHIER eniw_red.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* c'est le module qui g�re la redondance entre les 
* machine ma�tre et esclave de Wizcon, elle met � jour
* une variable globale d�crivant le ma�tre.
*
------------------------------------------------------
* HISTORIQUE :
*
* Moulin.D	version 1.1	07 Janv 2002	: Creation
* Moulin.D	version 1.2	23 Avr 2002	: Installation PLate forme
* Moulin.D	version 1.3	24 Sep 2002     : Forcage init sur reprise de comm
*
* AMI 28/01/21 : SAE_191: chantier date: encapsulation localtime pour hiniber TZ car nbseconde deja en TZ
------------------------------------------------------*/

/* fichiers inclus */
#include <xzsem.h>

#include "eniw_red.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Analyse de la redondance
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int et_MessageInit ()

/*
* ARGUMENTS EN ENTREE :
*   
* 
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
*
*
* CONDITION D'UTILISATION
* execute a chaque reprise de comm
*
* FONCTION 
*
------------------------------------------------------*/
{
   XZEXT_MSG_SOCKET 		pl_message;

         /* Construction du message d'init */
            sprintf ( pl_message, "%d|1|%s", ENIW_CMD_DEMANDE_INIT, XZEXC_FIN_MSG_SOCK_WIZCON );
         /* Envoie de la demande d'init */
		 sem_wait(&vg_semaphore); 
         if ( write ( vg_SockTWWI0, pl_message, strlen (pl_message) ) == XDC_NOK )
         { 
            /* Ecriture trace impossible d'emettre une demande d'arret */
             XZST_03EcritureTrace(XZSTC_WARNING, " Impossible d'emettre une demande d'init vers maitre Wizcon." );
         }
		 sem_post(&vg_semaphore); 

         /* Construction du message d'init */
            sprintf ( pl_message, "%d|1|%s", ENIW_CMD_DEMANDE_INIT, XZEXC_FIN_MSG_SOCK_WIZCON );
         /* Envoie de la demande d'init */
		 sem_wait(&vg_semaphore); 
         if ( write ( vg_SockTWWI1, pl_message, strlen (pl_message) ) == XDC_NOK )
         { 
            /* Ecriture trace impossible d'emettre une demande d'arret */
             XZST_03EcritureTrace(XZSTC_WARNING, " Impossible d'emettre une demande d'init vers esclave Wizcon." );
         }
		 sem_post(&vg_semaphore); 

}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Analyse de la redondance
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int et_Redondance (  )

/*
* ARGUMENTS EN ENTREE :
*   
* 
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
*
* CONDITION D'UTILISATION
* execute de fa�on p�riodique dans le eniw_dir
*
* FONCTION 
*
*  Analyse de la redondance des superviseur de la GTC
------------------------------------------------------*/
{
	int vl_retour=XDC_OK;
	static double vl_heure_dernier_essai_TWWI0=0;
	static double vl_heure_dernier_essai_TWWI1=0;
	static double vl_heure_debut_NOK_TWWI0=0;
	static double vl_heure_debut_NOK_TWWI1=0;
	static double vl_heure_dernier_alive=0;
	static int vl_stock_NOK_TWWI0=XDC_FAUX;
	static int vl_stock_NOK_TWWI1=XDC_FAUX;
	static int vl_dernier_etat_TWWI0=XDC_OK;
	static int vl_dernier_etat_TWWI1=XDC_OK;
	static int vl_nb_essai_TWWI0=0;
	static int vl_nb_essai_TWWI1=0;
	static int vl_alive=XDC_FAUX;

	/* Si la connexion avec la tache Wizcon maitre est mauvaise*/

	if (vg_etat_SockTWWI0 == XDC_NOK)
	{
		/* Stockage de l'heure de d�but de probleme */
		if (vl_stock_NOK_TWWI0 == XDC_FAUX)
		{
			vl_stock_NOK_TWWI0 = XDC_VRAI;
			            //vl_heure_debut_NOK_TWWI0=TutGetWallTime();   
      // reccupere la date courrante en Nb de seconde "TZ"
   XZSM_07LireHorodate( XZSMC_TYPE_SYSTEME, &vl_JourSem, &vl_HorodateSec );                   
		}           

		if (et_Relance_connexion(vl_nb_essai_TWWI0,vl_heure_dernier_essai_TWWI0) == XDC_OUI)
		{
			/*B Ecriture Trace */
			XZST_03EcritureTrace( XZSTC_WARNING, "Nouvel essai de connexion avec serveur Wizcon principal ." );	
			/* On refait un essai de connexion */
			//vl_heure_dernier_essai_TWWI0=TutGetWallTime();
         // reccupere la date courrante en Nb de seconde "TZ"
   XZSM_07LireHorodate( XZSMC_TYPE_SYSTEME, &vl_JourSem, &vl_HorodateSec );
			if ( ex_cnx_client_ST ( 	vg_nom_machine_TWWI0,
									vg_Port_SockTWWI0,
									&vg_SockMask,
									&vg_SockTWWI0 ) != XDC_OK )
			{
				/*B Ecriture Trace */
				XZST_03EcritureTrace( XZSTC_WARNING, "Impossible de re-creer la connexion serveur Wizcon principal ." );	
				
				vl_nb_essai_TWWI0++;
			}                       
			else
			{
				vg_etat_SockTWWI0=XDC_OK;
				vl_nb_essai_TWWI0=0;
			}

		}
	}
	/* La connexion avec la t�che Wizcon maitre est en etat de marche */
	else
	{
			//vl_heure_dernier_essai_TWWI0=TutGetWallTime();

		   // reccupere la date courrante en Nb de seconde "TZ"

         XZSM_07LireHorodate( XZSMC_TYPE_SYSTEME, &vl_JourSem, &vl_HorodateSec );
			vl_nb_essai_TWWI0=0;
			vg_etat_SockTWWI0=XDC_OK;
			vl_stock_NOK_TWWI0 = XDC_FAUX;
	}


	/* Si la connexion avec la tache Wizcon esclave est mauvaise*/

	if (vg_etat_SockTWWI1 == XDC_NOK)
	{
		/* Stockage de l'heure de d�but de probleme */
		if (vl_stock_NOK_TWWI1 == XDC_FAUX)
		{
			vl_stock_NOK_TWWI1 = XDC_VRAI;
			//vl_heure_debut_NOK_TWWI1=TutGetWallTime();

       // reccupere la date courrante en Nb de seconde "TZ"
       XZSM_07LireHorodate( XZSMC_TYPE_SYSTEME, &vl_JourSem, &vl_HorodateSec );
		}
		if (et_Relance_connexion(vl_nb_essai_TWWI1,vl_heure_dernier_essai_TWWI1) == XDC_OUI)
		{
			/*B Ecriture Trace */
			XZST_03EcritureTrace( XZSTC_WARNING, "Nouvel essai de connexion avec serveur Wizcon secondaire ." );	
			/* On refait un essai de connexion */
			//vl_heure_dernier_essai_TWWI1=TutGetWallTime();
        // reccupere la date courrante en Nb de seconde "TZ"
        XZSM_07LireHorodate( XZSMC_TYPE_SYSTEME, &vl_JourSem, &vl_HorodateSec );
			vl_nb_essai_TWWI1++;
			if ( ex_cnx_client_ST ( 	vg_nom_machine_TWWI1,
									vg_Port_SockTWWI1,
									&vg_SockMask,
									&vg_SockTWWI1 ) != XDC_OK )
			{
				/*B Ecriture Trace */
				XZST_03EcritureTrace( XZSTC_WARNING, "Impossible de re-creer la connexion avec serveur Wizcon secondaire ." );	
			}
			else
			{
				vg_etat_SockTWWI1=XDC_OK;
				vl_nb_essai_TWWI1=0;
			}

		}
	}
	/* La connexion avec la t�che Wizcon esclave est en etat de marche */
	else
	{
			//vl_heure_dernier_essai_TWWI1=TutGetWallTime();

         // reccupere la date courrante en Nb de seconde "TZ"
          XZSM_07LireHorodate( XZSMC_TYPE_SYSTEME, &vl_JourSem, &vl_HorodateSec );
			vl_nb_essai_TWWI1=0;
			vg_etat_SockTWWI1=XDC_OK;
			vl_stock_NOK_TWWI1 = XDC_FAUX;
	}	
	
	/* Ya t'il eu un changement d'�tat depuis la derni�re fois */
	if ((vg_etat_SockTWWI0 != vl_dernier_etat_TWWI0) ||
		(vg_etat_SockTWWI1 != vl_dernier_etat_TWWI1))
	{
		XZST_03EcritureTrace( XZSTC_WARNING, "Il y a un changement d'�tat sur les serveurs Wizcon." );	
		/* On indique qu'il va falloir stocker les heures de debut de NOK */
		vl_stock_NOK_TWWI0 = XDC_FAUX;
		vl_stock_NOK_TWWI1 = XDC_FAUX;

		/* Si les deux sockets ne r�pondent plus alors */
		if ((vg_etat_SockTWWI0 == XDC_NOK) && (vg_etat_SockTWWI1 == XDC_NOK))
		{
			/*B Ecriture Trace */
			XZST_03EcritureTrace( XZSTC_WARNING, "Les deux serveur Wizcon ne r�pondent plus ." );	
			/* Mise a jour des derniers etats*/
			vl_dernier_etat_TWWI0=vg_etat_SockTWWI0;
			vl_dernier_etat_TWWI1=vg_etat_SockTWWI1;
			vg_maitre_actif = XDC_MAITRE_INCONNU;
		}
		else
		{
			/* On est pass� de OK � NOK sur la tache Wizcon maitre */
			if ((vg_etat_SockTWWI0 == XDC_NOK) && (vl_dernier_etat_TWWI0 == XDC_OK))
			{
				XZST_03EcritureTrace( XZSTC_WARNING, "Le serveur Wizcon principal est passe a NOK." );	
				/* Le ma�tre �tait la tache Wizcon maitre */
				if (vg_maitre_actif == XDC_MAITRE_TWWI0)
				{
					/* Le nouveau maitre est Wizcon esclave*/
					/*B Ecriture Trace */
					XZST_03EcritureTrace( XZSTC_WARNING, "Forcage serveur Wizcon secondaire pour devenir maitre ." );	
					if (et_cmd_maitre(vg_SockTWWI1) != XDC_OK)
					{
						/*B Ecriture Trace */
						XZST_03EcritureTrace( XZSTC_WARNING, "Impossible de forcer serveur Wizcon secondaire pour devenir maitre ." );	
					}
					else
					{
						/*B Ecriture Trace */
						XZST_03EcritureTrace( XZSTC_WARNING, "Forcage OK serveur Wizcon secondaire pour devenir maitre ." );	
						vg_maitre_actif = XDC_MAITRE_TWWI1;
						/*Envoi de demande d'init sur reprise de comm*/
						et_MessageInit();
					}
				}
				vl_dernier_etat_TWWI0=vg_etat_SockTWWI0;
			}
			/* On est pass� de OK � NOK sur la tache Wizcon esclave */
			else if ((vg_etat_SockTWWI1 == XDC_NOK) && (vl_dernier_etat_TWWI1 == XDC_OK))
			{
				XZST_03EcritureTrace( XZSTC_WARNING, "Le serveur Wizcon secondaire est passe a NOK." );	
				/* Le ma�tre �tait la tache Wizcon esclave */
				if (vg_maitre_actif == XDC_MAITRE_TWWI1)
				{
					/*B Ecriture Trace */
					XZST_03EcritureTrace( XZSTC_WARNING, "Forcage serveur Wizcon principal pour devenir maitre ." );	
					/* Le nouveau maitre est Wizcon maitre*/
					if (et_cmd_maitre(vg_SockTWWI0) != XDC_OK)
					{
						/*B Ecriture Trace */
						XZST_03EcritureTrace( XZSTC_WARNING, "Impossible de forcer serveur Wizcon principal pour devenir maitre ." );	
					}
					else
					{
						/*B Ecriture Trace */
						XZST_03EcritureTrace( XZSTC_WARNING, "Forcage OK serveur Wizcon principal pour devenir maitre ." );	
						vg_maitre_actif = XDC_MAITRE_TWWI0;
						/*Envoi de demande d'init sur reprise de comm*/
						et_MessageInit();
					}
				}
				vl_dernier_etat_TWWI1=vg_etat_SockTWWI1;
			}
			/* On est pass� de NOK � OK sur la tache Wizcon maitre */
			else if ((vg_etat_SockTWWI0 == XDC_OK) && (vl_dernier_etat_TWWI0 == XDC_NOK))
			{
				XZST_03EcritureTrace( XZSTC_WARNING, "Le serveur Wizcon principal est passe a OK." );	
				/* On ne fait rien car le maitre est dej� Wizcon maitre */	
				vl_dernier_etat_TWWI0=vg_etat_SockTWWI0;
				/*Envoi de demande d'init sur reprise de comm*/
				et_MessageInit();
			}
			/* On est pass� de NOK � OK sur la tache Wizcon esclave */
			else if ((vg_etat_SockTWWI1 == XDC_OK) && (vl_dernier_etat_TWWI1 == XDC_NOK))
			{
				XZST_03EcritureTrace( XZSTC_WARNING, "Le serveur Wizcon secondaire est passe a OK." );	
				/* On ne fait rien car le maitre est dej� Wizcon maitre */	
				vl_dernier_etat_TWWI1=vg_etat_SockTWWI1;
			}
		}	
		/* Forcer un petit dialogue pour remettre � jour les donn�es */
		vl_alive=XDC_VRAI;
	}
	else
	{
		/* On va traiter le cas ou un seul Wizcon r�pond l'autre est en NOK  et le maitre est inconnu*/
		/* depuis 3 essais = 3 * 30, on force le seul dialoguant a etre maitre */
		if ((vg_etat_SockTWWI0 == XDC_OK) && (vg_etat_SockTWWI1 == XDC_NOK) && (vg_maitre_actif == XDC_MAITRE_INCONNU))
		{
			//if (abs((int)(vl_heure_debut_NOK_TWWI1 - TutGetWallTime())) > 3 * XDC_TIME_OUT_ESSAI)
            // reccupere la date courrante en Nb de seconde "TZ"
           XZSM_07LireHorodate( XZSMC_TYPE_SYSTEME, &vl_JourSem, &vl_HorodateSec );

                        
			{
				/* Le nouveau maitre est Wizcon maitre*/
				if (et_cmd_maitre(vg_SockTWWI0) != XDC_OK)
				{
					/*B Ecriture Trace */
					XZST_03EcritureTrace( XZSTC_WARNING, "Impossible de forcer serveur Wizcon principal pour devenir maitre ." );	
				}
				else
				{
					/*B Ecriture Trace */
					XZST_03EcritureTrace( XZSTC_WARNING, "Forcage OK serveur Wizcon principal pour devenir maitre ." );	
					vg_maitre_actif = XDC_MAITRE_TWWI0;
					/*Envoi de demande d'init sur reprise de comm*/
					et_MessageInit();
				}
			}
		}
		if ((vg_etat_SockTWWI1 == XDC_OK) && (vg_etat_SockTWWI0 == XDC_NOK) && (vg_maitre_actif == XDC_MAITRE_INCONNU))
		{
			//if (abs((int)(vl_heure_debut_NOK_TWWI0 - TutGetWallTime())) > 3 * XDC_TIME_OUT_ESSAI)
         // reccupere la date courrante en Nb de seconde "TZ"
          XZSM_07LireHorodate( XZSMC_TYPE_SYSTEME, &vl_JourSem, &vl_HorodateSec );
                        
			{
				/* Le nouveau maitre est serveur secondaire*/
				if (et_cmd_maitre(vg_SockTWWI1) != XDC_OK)
				{
					/*B Ecriture Trace */
					XZST_03EcritureTrace( XZSTC_WARNING, "Impossible de forcer serveur Wizcon secondaire pour devenir maitre ." );	
				}
				else
				{
					/*B Ecriture Trace */
					XZST_03EcritureTrace( XZSTC_WARNING, "Forcage OK serveur Wizcon secondaire pour devenir maitre ." );	
					vg_maitre_actif = XDC_MAITRE_TWWI1;
					/*Envoi de demande d'init sur reprise de comm*/
					et_MessageInit();
				}
			}
		}
	}

	/* On envoie un petit message pour v�rifier que les deux Wizcon sont joignables toutes les 30 s*/
	/* ou s'il y a eu u changement d'�tat car ceci permet de recaler les valeurs des OK et NOK */
	if ((abs((int)(vl_heure_dernier_alive - TutGetWallTime())) > XDC_TIME_OUT_ESSAI) || (vl_alive == XDC_VRAI))
	{
		/* RAZ du alive*/
		vl_alive=XDC_FAUX;
		vl_heure_dernier_alive=TutGetWallTime();
		if (vg_etat_SockTWWI0 == XDC_OK)
		{
			if (et_cmd_alive(vg_SockTWWI0) != XDC_OK)
			{
				vg_etat_SockTWWI0 = XDC_NOK;
				if (vg_maitre_actif == XDC_MAITRE_TWWI0)
					XZST_03EcritureTrace( XZSTC_WARNING, "Probleme de communication avec serveur principal Wizcon qui est le maitre ");	
				else
					XZST_03EcritureTrace( XZSTC_WARNING, "Probleme de communication avec serveur principal Wizcon qui est l'esclave ");	
			}
		}
		if (vg_etat_SockTWWI1 == XDC_OK)
		{
			if (et_cmd_alive(vg_SockTWWI1) != XDC_OK)
			{
				vg_etat_SockTWWI1 = XDC_NOK;
				if (vg_maitre_actif == XDC_MAITRE_TWWI1)
					XZST_03EcritureTrace( XZSTC_WARNING, "Probleme de communication avec serveur secondaire Wizcon qui est le maitre ");	
				else
					XZST_03EcritureTrace( XZSTC_WARNING, "Probleme de communication avec serveur secondaire Wizcon qui est l'esclave ");	
			}
		}
	}
	return(vl_retour);
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Analyse de la redondance
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int et_Relance_connexion (	int a_nb_essai,
							double a_heure_dernier_essai )

/*
* ARGUMENTS EN ENTREE :
*   
* 
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
*
* CONDITION D'UTILISATION
* execute de fa�on p�riodique dans le eniw_dir
*
* FONCTION 
*
*  D�termine s'il faut relancer la connexion
------------------------------------------------------*/
{
	int vl_retour=XDC_NON;

	/* A partir de trois tentatives les essais sont espac�s*/
	if (a_nb_essai >= 3)
	{
		XZST_03EcritureTrace( XZSTC_DEBUG1, "pr�c�dente %f",a_heure_dernier_essai);	
		//XZST_03EcritureTrace( XZSTC_DEBUG1, "actuelle %f",TutGetWallTime());	
		//XZST_03EcritureTrace( XZSTC_DEBUG1, "difference %f",TutGetWallTime()-a_heure_dernier_essai);
		/* Diff�rence entre maintenant et le dernier essai*/
		//if (abs((int)(TutGetWallTime()-a_heure_dernier_essai))	 > XDC_TIME_OUT_ESSAI)

      // reccupere la date courrante en Nb de seconde "TZ"
   XZSM_07LireHorodate( XZSMC_TYPE_SYSTEME, &vl_JourSem, &vl_HorodateSec );
		{
			vl_retour=XDC_OUI;
		}
	}
	else
	{
		vl_retour=XDC_OUI;
	}
	return (vl_retour);
}
/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Analyse de la redondance
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int et_Synchro ()

/*
* ARGUMENTS EN ENTREE :
*   
* 
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
*
* CONDITION D'UTILISATION
* execute de fa�on p�riodique dans le eniw_dir
*
* FONCTION 
*
*  Permet de lancer une synchro de temps en temps
------------------------------------------------------*/
{
	int vl_retour=XDC_NON;
	static double vl_heure_dernier_synchro=0;

	/* On envoie un petit message pour v�rifier que les deux Wizcon sont synchroniser toutes les N s*/
	//if (abs((int)(vl_heure_dernier_synchro - TutGetWallTime())) > XDC_TIME_OUT_SYNCHRO) 

                        
		//vl_heure_dernier_synchro=TutGetWallTime();
                         
   // reccupere la date courrante en Nb de seconde "TZ"
   XZSM_07LireHorodate( XZSMC_TYPE_SYSTEME, &vl_JourSem, &vl_HorodateSec );

}
		if (et_cmd_synchro(vg_SockTWWI0) != XDC_OK)
		{
			if (vg_maitre_actif == XDC_MAITRE_TWWI0)
				XZST_03EcritureTrace( XZSTC_WARNING, "Probleme de synchronisation avec le maitre ");	
			else
				XZST_03EcritureTrace( XZSTC_WARNING, "Probleme de synchronisation avec l'esclave ");	
		}

		if (et_cmd_synchro(vg_SockTWWI1) != XDC_OK)
		{
			if (vg_maitre_actif == XDC_MAITRE_TWWI1)
				XZST_03EcritureTrace( XZSTC_WARNING, "Probleme de synchronisation avec le maitre ");	
			else
				XZST_03EcritureTrace( XZSTC_WARNING, "Probleme de synchronisation avec l'esclave ");	
		}
	}
	return (vl_retour);
}

