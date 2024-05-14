/*E*/
/*Fichier : $Id: xzma.c,v 1.3 1996/06/17 19:08:02 gaborit Exp $      Release : $Revision: 1.3 $        Date : $Date: 1996/06/17 19:08:02 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  MAINTE
------------------------------------------------------
* MODULE XZMA * FICHIER xzma.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Le module xzma sert � mettre � jour les compteurs de
* marche des �quipements pour alimenter la base de donn�es
* de la GMAO.
*
* Les compteurs de marche peuvent etre de deux types :
*	- en nombre d'utilisation (ex : d�ploiement BRA)
*	- en dur�e d'utilisation (ex : affichage PMV)
*
* Les primitives d'interface XZMA01 et XZMA02 permettent
* au taches d'EQUEXT de mettre � jour les valeurs de ces
* compteurs dans des fichiers duree_xxxx ou nombre_xxxx.
*
* Un callback est automatiquement arm� au premier appel
* d'une primitive du module, et sert � relever les
* compteurs dans un fichier transfert_xxxx.
*
* Ce fichier sera rapatri� sur POCMA (poste de maintenance)
* par la tache tmcpt.x qui appelle le script xzma_transfert.ksh.
*
------------------------------------------------------
* HISTORIQUE :
*
* B.GABORIT	26 dec 1995 :	Cr�ation	(1.1)
* C.T.     	14 jui 1996 :	correction envoi sur CPT_PRET	(1.2)
* LCL		20/12/21	: MOVIS P2 ems SAE-312
------------------------------------------------------*/

/* fichiers inclus */
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <time.h>
#include "xzma.h"

/* definitions de constantes */
#define XZMAC_taille_fichier 512

/* definitions de types locaux */
typedef char	XZMAT_fichier[XZMAC_taille_fichier]; /* nom de fichier en absolu */

/* definition de macro locales */

/* declaration de variables locales */
int		vm_init=XDC_NON;
XDY_NomTache	vm_tache;
XDY_NomMachine	vm_machine;

/* declaration de fonctions internes */

/* definition de fonctions externes */



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : Rel�ve les fichiers de compteurs pour envoi � POCMA
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	XZMA_Callback_relever(
	T_IPC_CONN			va_Cnx,
	T_IPC_CONN_PROCESS_CB_DATA	pa_Data,
	T_CB_ARG			pa_ARG
          )

/*
* ARGUMENTS EN ENTREE : 
*
* structure rtworks
*
* ARGUMENTS EN SORTIE : aucun
*
*
* CODE RETOUR : aucun
*
*
* CONDITION D'UTILISATION
*
* appel� sur r�ception du message XDM_M_RELEVER_COMPTEUR
*
* FONCTION 
*
* construit un fichier transfert_tache_machine_jj_mm_aa � partir
* des fichiers duree_tache_machine_x_y et nombre_tache_machine_x_y,
* puis envoie un message pour indiquer que la r�cup�ration du fichier
* transfert peut commencer.
*
------------------------------------------------------*/
{
static char 		*version = "$Id: xzma.c,v 1.3 1996/06/17 19:08:02 gaborit Exp $ : XZMA_Callback_relever" ;
#ifndef _TIBCO_EMS
	T_STR                   vl_Cle  = "";
#else
	char                   vl_Cle[255]  = "";
#endif
DIR 			*pl_dir;
FILE 			*pl_fichier;
FILE			*pl_fichier_transfert;
struct dirent 		*pl_fichierdir;
XZMAT_fichier 		vl_debutfichierduree;
XZMAT_fichier 		vl_debutfichiernombre;
int 			vl_tailledebutduree;
int 			vl_tailledebutnombre;
XZMAT_fichier 		pl_nomfichier;
int 			vl_JourSemaine=0;
XDY_Horodate 		vl_Nb_Secondes=0.0;
double 			vl_horodate;
double 			vl_compteur;
time_t			vl_Temps;
struct tm		*pl_InfoTemps;
int			vl_type;
int			vl_numero;
int			vl_etat;
XZSCT_NomMachine	vl_NomMachineLocale;


XZST_03EcritureTrace(XZSTC_INTERFACE , "IN : %s",version );

/*A r�cup�ration du message */
if (TipcMsgRead(pa_Data->msg, T_IPC_FT_STR, &vl_Cle, NULL))
{
	XZST_03EcritureTrace(XZSTC_INFO, "%s : appel� par timer",version);
}
else
{
	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : %s Sortie sur �chec TipcMsgRead",version);
	return;
}

/*A r�cup�ration de l'horodate */
if (XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME,&vl_JourSemaine,&vl_Nb_Secondes)!=XZSMC_OK)
{
	XZST_03EcritureTrace(XZSTC_WARNING,"%s : la r�cup�ration de l'horodate a �chou�", version);
}
else

{
	/*A test de l'existence du fichier transfert_tache_machine_jj_mm_aa */
	/*B construction du nom */

	/* conversion de l'horodate en champs jj, mm, aa */
	vl_Temps = (time_t)vl_Nb_Secondes ;
	pl_InfoTemps = localtime(&vl_Temps) ;

	sprintf(pl_nomfichier,"%s/transfert_%s_%s_%02u_%02u_%02u",
				XDC_PATHCMPT,
				vm_tache, 
				vm_machine,
				pl_InfoTemps->tm_mday,
				pl_InfoTemps->tm_mon,
				pl_InfoTemps->tm_year);

	/*B ouverture du fichier en lecture */
	pl_fichier_transfert=fopen(pl_nomfichier,"r");
	if (pl_fichier_transfert!=NULL)
	/*A si le fichier existe : fin des traitements */
	{
		XZST_03EcritureTrace(XZSTC_INFO,"%s : le fichier %s existe d�j�",version,pl_nomfichier);
		fclose(pl_fichier_transfert);

		/*A envoie un message XDM_M_COMPTEUR_RELEVE */
		/*! le fichier est deja genere mais n'a pas ete recupere */
		if (XZSC_07NomMachine(vl_NomMachineLocale)!=XDC_OK)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"%s : impossible de r�cup�rer le nom de la machine",version);
		}
		else if (!TipcSrvMsgWrite(	XDG_M_COMPTEUR_PRET,
				XDM_IdentMsg(XDM_M_COMPTEUR_RELEVE),
				XDM_FLG_SRVMSGWRITE,
				T_IPC_FT_STR, vl_NomMachineLocale,
				NULL ))
			XZST_03EcritureTrace(XZSTC_WARNING ,"%s : impossible d'envoyer le message XDM_M_COMPTEUR_RELEVE",version);
		return;
	}

	/*A sinon, cr�ation du fichier */
	pl_fichier_transfert=fopen(pl_nomfichier,"w");
	if (pl_fichier_transfert==NULL)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"%s : �chec cr�ation %s", version,pl_nomfichier);
		return;
	}

	/*A r�cup�ration de la liste des fichiers */
	pl_dir=opendir(XDC_PATHCMPT);
	if (pl_dir==NULL)
	/* echec ouverture directory */
	{
		XZST_03EcritureTrace( XZSTC_WARNING,"%s : echec ouverture du directory %s", version,XDC_PATHCMPT);
		fclose(pl_fichier_transfert);
		return;
	}
	/* ouverture correcte du directory */

	/* construction des noms de fichier types */
	sprintf (vl_debutfichierduree,"duree_%s_%s",vm_tache,vm_machine);
	vl_tailledebutduree=strlen(vl_debutfichierduree);
	sprintf (vl_debutfichiernombre,"nombre_%s_%s",vm_tache,vm_machine);
	vl_tailledebutnombre=strlen(vl_debutfichiernombre);

	/*A pour chaque fichier du directory */
	while ((pl_fichierdir=readdir(pl_dir))!=NULL)

	/*A si il correspond � duree_tache_machine */
	if (strncmp(pl_fichierdir->d_name,vl_debutfichierduree,vl_tailledebutduree)==0)
	{
		/*B ouverture du fichier en lecture */
		sprintf(pl_nomfichier,"%s/%s", XDC_PATHCMPT, pl_fichierdir->d_name);
		pl_fichier=fopen(pl_nomfichier,"r");
		if (pl_fichier!=NULL)

		/*B le fichier existe, lecture de la valeur */
		{
			if ( fscanf(pl_fichier,"COMPTEUR = %lf ETAT = %d HORODATE = %lf", &vl_compteur, &vl_etat, &vl_horodate)!=3)

			/* erreur de lecture du contenu du fichier */
			{
				XZST_03EcritureTrace(XZSTC_WARNING,"%s : erreur de lecture du fichier %s", version, pl_nomfichier);
				fclose(pl_fichier);
			}
			else

			/*A mise � z�ro du compteur */
			{
				fclose(pl_fichier);

				/* reouverture en �criture */
				pl_fichier=fopen(pl_nomfichier,"w");
				if (pl_fichier!=NULL)
				{
					fprintf(pl_fichier,"COMPTEUR = %lf ETAT = %d HORODATE = %lf", (double)0.0, vl_etat, vl_horodate);
					fclose(pl_fichier);

					/*A r�cup�ration du type et du num�ro d'�quipement � partir du nom de fichier */
					if (sscanf(pl_fichierdir->d_name,"duree_%*[^_]_%*[^_]_%d_%d",&vl_type,&vl_numero)!=2)
					{
						XZST_03EcritureTrace(XZSTC_WARNING,"%s : erreur de d�codage du nom du fichier %s", version, pl_fichierdir->d_name);
					}
					else

					/*A ajout d'une ligne dans le fichier transfert si compteur<>0 */
					if (vl_compteur!=0.0)
						fprintf(pl_fichier_transfert,"TYPE = %d NUMERO = %d TOTAL = %d\n",vl_type,vl_numero,(int)vl_compteur);
				}
				else
				{
					XZST_03EcritureTrace(XZSTC_WARNING,"%s : impossible d'�crire dans le fichier %s", version, pl_nomfichier);
				}
			}
		}
	}
	else
	/*A si il correspond � nombre_tache_machine */
	if (strncmp(pl_fichierdir->d_name,vl_debutfichiernombre,vl_tailledebutnombre)==0)
	{
		/*B ouverture du fichier en lecture */
		sprintf(pl_nomfichier,"%s/%s", XDC_PATHCMPT, pl_fichierdir->d_name);
		pl_fichier=fopen(pl_nomfichier,"r");
		if (pl_fichier!=NULL)

		/*B le fichier existe, lecture de la valeur */
		{
			if ( fscanf(pl_fichier,"COMPTEUR = %lf", &vl_compteur)!=1)

			/* erreur de lecture du contenu du fichier */
			{
				XZST_03EcritureTrace(XZSTC_WARNING,"%s : erreur de lecture du fichier %s", version, pl_nomfichier);
				fclose(pl_fichier);
			}
			else

			/*A mise � z�ro du compteur */
			{
				fclose(pl_fichier);

				/* reouverture en �criture */
				pl_fichier=fopen(pl_nomfichier,"w");
				if (pl_fichier!=NULL)
				{
					fprintf(pl_fichier,"COMPTEUR = 0");
					fclose(pl_fichier);

					/*A r�cup�ration du type et du num�ro d'�quipement � partir du nom de fichier */
					if (sscanf(pl_fichierdir->d_name,"nombre_%*[^_]_%*[^_]_%d_%d",&vl_type,&vl_numero)!=2)
					{
						XZST_03EcritureTrace(XZSTC_WARNING,"%s : erreur de d�codage du nom du fichier %s", version, pl_fichierdir->d_name);
					}
					else

					/*A ajout d'une ligne dans le fichier transfert si compteur<>0 */
					if (vl_compteur!=0.0)
						fprintf(pl_fichier_transfert,"TYPE = %d NUMERO = %d TOTAL = %d\n",vl_type,vl_numero,(int)vl_compteur);
				}
				else
				{
					XZST_03EcritureTrace(XZSTC_WARNING,"%s : impossible d'�crire dans le fichier %s", version, pl_nomfichier);
				}
			}
		}
	}
	closedir(pl_dir);
	fclose(pl_fichier_transfert);

	/*A envoie un message XDM_M_COMPTEUR_RELEVE */
	if (XZSC_07NomMachine(vl_NomMachineLocale)!=XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"%s : impossible de r�cup�rer le nom de la machine",version);
	}
	else
	if (!TipcSrvMsgWrite(	XDG_M_COMPTEUR_PRET,
				XDM_IdentMsg(XDM_M_COMPTEUR_RELEVE),
				XDM_FLG_SRVMSGWRITE,
				T_IPC_FT_STR, vl_NomMachineLocale,
				NULL ))
		XZST_03EcritureTrace(XZSTC_WARNING ,"%s : impossible d'envoyer le message XDM_M_COMPTEUR_RELEVE",version);
}

XZST_03EcritureTrace(XZSTC_INTERFACE , "OUT : %s" , version);
return;
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : Initialise le module XZMA de gestion des compteurs de marche
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	XZMA_Init(
	XDY_NomTache	va_tache_in,
	XDY_NomMachine	va_machine_in
          )

/*
* ARGUMENTS EN ENTREE : 
*
*
* ARGUMENTS EN SORTIE : aucun
*
*
* CODE RETOUR : 
*
* 	XDC_OK 		si ex�cution correcte de la primitive.
*	XDC_NOK 	si erreur d'ex�cution de la primitive
*
* CONDITION D'UTILISATION
*
* appel� par XZMA01 ou XZMA02
*
* FONCTION 
*
* si l'init est d�j� effectu� : return
* sinon :
*	abonne la tache au datagroup XDG_M_COMPTEUR
*	arme un callback sur le message XDM_M_RELEVER_COMPTEUR
*	met � jour les variables vm_init, vm_tache et vm_machine
*	positionne ETAT � XDC_FAUX et HORODATE � horodate courante
*		dans tous les fichiers duree_tache_machine_x_y
*
------------------------------------------------------*/
{
static char 		*version = "$Id: xzma.c,v 1.3 1996/06/17 19:08:02 gaborit Exp $ : XZMA_Init" ;
int 			vl_code_retour=XDC_OK ;
DIR 			*pl_dir;
FILE 			*pl_fichier;
struct dirent 		*pl_fichierdir;
XZMAT_fichier 		vl_debutfichier;
int 			vl_tailledebut;
XZMAT_fichier 		pl_nomfichier;
int 			vl_JourSemaine=0;
XDY_Horodate 		vl_Nb_Secondes=0.0;
double 			vl_horodate;
double 			vl_compteur;
int 			vl_etat;


XZST_03EcritureTrace(XZSTC_INTERFACE , "IN : %s",version );

/*A teste si l'init a d�ja �t� r�alis� */
if (vm_init==XDC_NON)
{
	/*A positionne la variable d'init � XDC_OUI */
	vm_init=XDC_OUI;

	/*A met � jour les variables modules */
	strcpy(vm_tache, va_tache_in);
	strcpy(vm_machine, va_machine_in);

	/*A arme un callback sur XDM_M_RELEVER_COMPTEUR */
	if (TipcSrvProcessCbCreate ( XDM_IdentMsg(XDM_M_RELEVER_COMPTEUR), XZMA_Callback_relever, (T_CB_ARG)NULL) == NULL)
	{
		XZST_03EcritureTrace( XZSTC_WARNING,"%s : Callback sur XDM_M_RELEVER_COMPTEUR non cr��", version);
		vm_init=XDC_NON;
		vl_code_retour=XDC_NOK;
	}
	else

	/*A abonne la tache � XDG_M_COMPTEUR */
	if (!XDG_JoindreDG(XDG_M_COMPTEUR))
	{
		XZST_03EcritureTrace( XZSTC_WARNING,"%s : Abonnement impossible � XDG_M_COMPTEUR", version);
		vm_init=XDC_NON;
		vl_code_retour=XDC_NOK;
	}
	else

	/*A r�cup�ration de l'horodate */
	if (XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME,&vl_JourSemaine,&vl_Nb_Secondes)!=XZSMC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"%s : la r�cup�ration de l'horodate a �chou�", version);
		vl_code_retour=XDC_NOK;
	}
	else

	/*A met � jour les fichiers existants */
	{
		/*B r�cup�ration de la liste des fichiers */
		pl_dir=opendir(XDC_PATHCMPT);
		if (pl_dir==NULL)
		/* echec ouverture directory */
		{
			XZST_03EcritureTrace( XZSTC_WARNING,"%s : echec ouverture du directory %s", version,XDC_PATHCMPT);
			vl_code_retour=XDC_NOK;
		}
		else
		/* ouverture correcte du directory */
		{
			/* construction du nom de fichier type */
			sprintf (vl_debutfichier,"duree_%s_%s",vm_tache,vm_machine);
			vl_tailledebut=strlen(vl_debutfichier);

			/*B pour chaque fichier du directory */
			while ((pl_fichierdir=readdir(pl_dir))!=NULL)

			/*B si il correspond � duree_tache_machine */
			if (strncmp(pl_fichierdir->d_name,vl_debutfichier,vl_tailledebut)==0)
			{
				/*B mise � jour du fichier avec ETAT=XDC_FAUX et HORODATE=horodate_courante */
				sprintf(pl_nomfichier,"%s/%s", XDC_PATHCMPT, pl_fichierdir->d_name);

				/*B ouverture du fichier en lecture */
				pl_fichier=fopen(pl_nomfichier,"r");
				if (pl_fichier!=NULL)

				/*B le fichier existe, lecture de la valeur */
				{
					if ( fscanf(pl_fichier,"COMPTEUR = %lf ETAT = %d HORODATE = %lf", &vl_compteur, &vl_etat, &vl_horodate)!=3)

					/* erreur de lecture du contenu du fichier */
					{
						XZST_03EcritureTrace(XZSTC_WARNING,"%s : erreur de lecture du fichier %s", version, pl_nomfichier);
						fclose(pl_fichier);
						vl_code_retour=XDC_NOK;
					}
					else

					/* modification de l'�tat et de l'horodate et re�criture du fichier */
					{
						fclose(pl_fichier);

						/* reouverture en �criture */
						pl_fichier=fopen(pl_nomfichier,"w");
						if (pl_fichier!=NULL)
						{
							fprintf(pl_fichier,"COMPTEUR = %lf ETAT = %d HORODATE = %lf", vl_compteur, (int)XDC_FAUX, (double)vl_Nb_Secondes);
							fclose(pl_fichier);
						}
						else
						{
							XZST_03EcritureTrace(XZSTC_WARNING,"%s : impossible d'�crire dans le fichier %s", version, pl_nomfichier);
							vl_code_retour=XDC_NOK;
						}
					}
				}
			}
			closedir(pl_dir);
		}
	}
	/*A fin de l'initialisation */
	XZST_03EcritureTrace(XZSTC_INFO , "%s init avec tache %s et machine %s",version,vm_tache,vm_machine);
}

XZST_03EcritureTrace(XZSTC_INTERFACE , "OUT : %s retourne %d" , version, vl_code_retour);
return vl_code_retour;
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : Met � jour le compteur de marche(dur�e) d'un �quipement
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	XZMA01_Maj_duree_marche(
	XDY_Eqt		va_numero_in,
	XDY_TypeEqt	va_type_in,
	XDY_Entier	va_compter_in,
	XDY_NomTache	va_tache_in,
	XDY_NomMachine	va_machine_in
          )

/*
* ARGUMENTS EN ENTREE :
*	XDY_Eqt		va_numero_in
*	XDY_TypeEqt	va_type_in
*	XDY_Entier	va_compter_in (XDC_VRAI pour d�marrage et XDC_FAUX pour arret)
*	XDY_NomTache	va_tache_in
*	XDY_NomMachine	va_machine_in (nom de la machine trait�e, qui peut etre <> de la machine locale)
*
* ARGUMENTS EN SORTIE : aucun
*
*
* CODE RETOUR : 
*
* 	XDC_OK 		si ex�cution correcte de la primitive.
*	XDC_NOK 	si erreur d'ex�cution de la primitive
*
* CONDITION D'UTILISATION
*
* appel dans le cas d'un �quipement dont on compte la dur�e d'utilisation (pour le nombre, cf XZMA02).
* Le nom de machine est celui de la machine pour qui l'on traite (en cas de reprise par une autre machine)
*
* FONCTION 
*
* Mise � jour ou cr�ation du fichier duree_tache_machine_type_numero
*
------------------------------------------------------*/
{
static char 	*version = "$Id: xzma.c,v 1.3 1996/06/17 19:08:02 gaborit Exp $ : XZMA01_Maj_duree_marche" ;
int 		vl_code_retour=XDC_OK ;
XZMAT_fichier 	pl_nomfichier;
FILE 		*pl_fichier;
int 		vl_JourSemaine=0;
XDY_Horodate 	vl_Nb_Secondes=0.0;
double 		vl_horodate;
double 		vl_compteur;
int 		vl_etat;

XZST_03EcritureTrace(XZSTC_INTERFACE , "IN : %s",version );
XZST_03EcritureTrace(XZSTC_INFO, "%s appel� avec eqt %d type %d compter %d tache %s machine %s", version, va_numero_in, va_type_in, va_compter_in, va_tache_in, va_machine_in);

/*A appel de l'init */
if (XZMA_Init(va_tache_in, va_machine_in) !=XDC_OK)
{
	XZST_03EcritureTrace(XZSTC_WARNING, "%s : l'init du module XZMA a �chou�", version);
	vl_code_retour=XDC_NOK;
}
else
{
	/*A r�cup�ration de l'horodate */
	if (XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME,&vl_JourSemaine,&vl_Nb_Secondes)!=XZSMC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"%s : la r�cup�ration de l'horodate a �chou�", version);
		vl_code_retour=XDC_NOK;
	}
	else

	/*A mise � jour du fichier duree_tache_machine_type_numero */
	{
		sprintf(pl_nomfichier,"%s/duree_%s_%s_%d_%d", XDC_PATHCMPT, va_tache_in, va_machine_in, va_type_in, va_numero_in);

		/*B ouverture du fichier en lecture */
		pl_fichier=fopen(pl_nomfichier,"r");
		if (pl_fichier!=NULL)

		/*B le fichier existe, lecture de la valeur */
		{
			if ( fscanf(pl_fichier,"COMPTEUR = %lf ETAT = %d HORODATE = %lf", &vl_compteur, &vl_etat, &vl_horodate)!=3)

			/* erreur de lecture du contenu du fichier */
			{
				XZST_03EcritureTrace(XZSTC_WARNING,"%s : erreur de lecture du fichier %s", version, pl_nomfichier);
				fclose(pl_fichier);
				vl_code_retour=XDC_NOK;
			}
			else

			/* modification de la valeur du compteur et re�criture du fichier */
			{
				if (vl_etat==XDC_VRAI)
					vl_compteur = vl_compteur + vl_Nb_Secondes - vl_horodate;

				XZST_03EcritureTrace(XZSTC_INFO,"%s : fichier %s, nouvelle valeur du compteur : %lf", version, pl_nomfichier, vl_compteur);
				fclose(pl_fichier);

				/* reouverture en �criture */
				pl_fichier=fopen(pl_nomfichier,"w");
				if (pl_fichier!=NULL)
				{
					fprintf(pl_fichier,"COMPTEUR = %lf ETAT = %d HORODATE = %lf", vl_compteur, (int)va_compter_in, (double)vl_Nb_Secondes);
					fclose(pl_fichier);
				}
				else
				{
					XZST_03EcritureTrace(XZSTC_WARNING,"%s : impossible d'�crire dans le fichier %s", version, pl_nomfichier);
					vl_code_retour=XDC_NOK;
				}
			}
		}
		else
		/*B le fichier n'existe pas, cr�ation */
		{
			pl_fichier=fopen(pl_nomfichier,"w");
			if (pl_fichier!=NULL)
			{
				fprintf(pl_fichier,"COMPTEUR = %lf ETAT = %d HORODATE = %lf", (double)0.0, (int)va_compter_in, (double)vl_Nb_Secondes);
				fclose(pl_fichier);
				XZST_03EcritureTrace(XZSTC_INFO,"%s : fichier %s cr�� avec compteur=0", version, pl_nomfichier);
			}
			else
			{
				XZST_03EcritureTrace(XZSTC_WARNING,"%s : impossible de cr�er le fichier %s", version, pl_nomfichier);
				vl_code_retour=XDC_NOK;
			}
		}
	}
}

XZST_03EcritureTrace(XZSTC_INTERFACE , "OUT : %s retourne %d" , version, vl_code_retour);
return vl_code_retour;
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : Met � jour le compteur de marche (nb utilisations) d'un �quipement
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	XZMA02_Maj_nb_utilisations(
	XDY_Eqt		va_numero_in,
	XDY_TypeEqt	va_type_in,
	XDY_NomTache	va_tache_in,
	XDY_NomMachine	va_machine_in
          )

/*
* ARGUMENTS EN ENTREE :
*	XDY_Eqt		va_numero_in
*	XDY_TypeEqt	va_type_in
*	XDY_NomTache	va_tache_in
*	XDY_NomMachine	va_machine_in (nom de la machine trait�e, qui peut etre <> de la machine locale)
*
* ARGUMENTS EN SORTIE : aucun
*
*
* CODE RETOUR : 
*
* 	XDC_OK 		si ex�cution correcte de la primitive.
*	XDC_NOK 	si erreur d'ex�cution de la primitive
*
* CONDITION D'UTILISATION
*
* appel dans le cas d'un �quipement dont on compte le nombre d'utilisation (pour la dur�e, cf XZMA01).
* Le nom de machine est celui de la machine pour qui l'on traite (en cas de reprise par une autre machine)
*
* FONCTION 
*
* Mise � jour ou cr�ation du fichier nombre_tache_machine_type_numero
*
------------------------------------------------------*/
{
static char 	*version = "$Id: xzma.c,v 1.3 1996/06/17 19:08:02 gaborit Exp $ : XZMA02_Maj_nb_utilisations" ;
int 		vl_code_retour=XDC_OK ;
XZMAT_fichier 	pl_nomfichier;
FILE 		*pl_fichier;
int 		vl_compteur;

XZST_03EcritureTrace(XZSTC_INTERFACE , "IN : %s",version );
XZST_03EcritureTrace(XZSTC_INFO, "%s appel� avec eqt %d type %d tache %s machine %s", version, va_numero_in, va_type_in, va_tache_in, va_machine_in);

/*A appel de l'init */
if (XZMA_Init(va_tache_in, va_machine_in) !=XDC_OK)
{
	XZST_03EcritureTrace(XZSTC_WARNING, "%s : l'init du module XZMA a �chou�", version);
	vl_code_retour=XDC_NOK;
}
else
{
	/*A mise � jour du fichier nombre_tache_machine_type_numero */
	sprintf(pl_nomfichier,"%s/nombre_%s_%s_%d_%d", XDC_PATHCMPT, va_tache_in, va_machine_in, va_type_in, va_numero_in);

	/*B ouverture du fichier en lecture */
	pl_fichier=fopen(pl_nomfichier,"r");
	if (pl_fichier!=NULL)

	/*B le fichier existe, lecture de la valeur */
	{
		if ( fscanf(pl_fichier,"COMPTEUR= %d", &vl_compteur)!=1)

		/* erreur de lecture du contenu du fichier */
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"%s : erreur de lecture du fichier %s", version, pl_nomfichier);
			fclose(pl_fichier);
			vl_code_retour=XDC_NOK;
		}
		else

		/* modification de la valeur du compteur et re�criture du fichier */
		{
			vl_compteur++;
			XZST_03EcritureTrace(XZSTC_INFO,"%s : fichier %s, nouvelle valeur du compteur : %d", version, pl_nomfichier, vl_compteur);
			fclose(pl_fichier);

			/* reouverture en �criture */
			pl_fichier=fopen(pl_nomfichier,"w");
			if (pl_fichier!=NULL)
			{
				fprintf(pl_fichier,"COMPTEUR= %d", vl_compteur);
				fclose(pl_fichier);
			}
			else
			{
				XZST_03EcritureTrace(XZSTC_WARNING,"%s : impossible d'�crire dans le fichier %s", version, pl_nomfichier);
				vl_code_retour=XDC_NOK;
			}
		}
	}
	else
	/*B le fichier n'existe pas, cr�ation */
	{
		pl_fichier=fopen(pl_nomfichier,"w");
		if (pl_fichier!=NULL)
		{
			fprintf(pl_fichier,"COMPTEUR= 1");
			fclose(pl_fichier);
			XZST_03EcritureTrace(XZSTC_INFO,"%s : fichier %s cr�� avec compteur=1", version, pl_nomfichier);
		}
		else
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"%s : impossible de cr�er le fichier %s", version, pl_nomfichier);
			vl_code_retour=XDC_NOK;
		}
	}
}

XZST_03EcritureTrace(XZSTC_INTERFACE , "OUT : %s retourne %d" , version, vl_code_retour);
return vl_code_retour;
}

