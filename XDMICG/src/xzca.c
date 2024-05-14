/*E*/
/*Fichier : $Id: xzca.c,v 1.4 2010/12/08 22:34:39 gesconf Exp $      Release : $Revision: 1.4 $        Date : $Date: 2010/12/08 22:34:39 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  GECONF
------------------------------------------------------
* MODULE ZCA * FICHIER xzca.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*	Module : XZCA_Archive qui permet de gerer le catalogue
* des sauvegarde de la base Historique.
* (DCG-002/0.1 page 57)
******************
*	xzca01_Lecture_Catalogue : prend un argument une fonction utilisateur
* que l on va appeler pour chaque occurence du fichier du catalogue d archivage.
******************
*
------------------------------------------------------
* HISTORIQUE :
*
* mercier	06 Jan 1995	: Creation
*************
* MERCIER	12 Jan 1995	: Modif de xzca_Free_Tab_EltArchiv V 1.2
*************
* Mercier 	17 Fev 1995 	: Correction envoie de Msg RtWks 	V 1.3
* On n utilise pas TipcMsgCreate
* JPL		08/12/10 : Migration architecture HP ia64 (DEM 961) : suppression warnings mineurs  1.4
------------------------------------------------------*/

/* fichiers inclus */

#include "xzca.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: xzca.c,v 1.4 2010/12/08 22:34:39 gesconf Exp $ : xzca" ;

/* declaration de fonctions internes */
int xzca_Free_Tab_EltArchiv (XZCAT_ComposantCAT *, int);

/* definition de fonctions externes */
/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	xzca01_Lecture_Catalogue : prend un argument une fonction utilisateur
* que l on va appeler pour chaque occurence du fichier du catalogue d archivage.
* 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int xzca01_Lecture_Catalogue (XDY_FonctionInt pa_fct_util, int *pa_Nb_Archiv) 

/*
* ARGUMENTS EN ENTREE :
* ARGUMENTS EN SORTIE :
*   aucun
* CODE RETOUR : 
*
*  XDC_OK si tout s est bien passe
*  XZCAC_ERR_LECTURE si il y a eu un probleme de lecture sur le fichier
*  Si une autre erreur est detectee, on retourne XDC_NOK
* CONDITION D'UTILISATION
* FONCTION 
* 	
------------------------------------------------------*/
{
	static char *	version = "$Id: xzca.c,v 1.4 2010/12/08 22:34:39 gesconf Exp $	: xzca01_Lecture_Catalogue" ;
	FILE * 		pl_FD;
	char		pl_NomFile[255];
	XZCAT_ComposantCAT * pl_Tab_EltArchiv;
	int		vl_indice  = 0;
	int		vl_indice2 = 0;
	int 		vl_retcode = XDC_OK;
	
	
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : xzca01_Lecture_Catalogue ");
	/*A
	** Initialise le nb d elet traite a 0 en cas de sortie anormal de l appli
	*/
	*pa_Nb_Archiv = 0;
	
	/*A
	** Ouverture du fichier du catalogue des archives.
	*/
	sprintf(pl_NomFile,"%s/%s",XDC_PATHFICDYN,XDF_CATALOGUE_ARCHIVE);	
	
	if (!(pl_FD=fopen(pl_NomFile,"r")))
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzca01_Lecture_Catalogue : ouverture du fichier de BL %s impossible en Read-Write",pl_NomFile);
		fclose(pl_FD);
		/*B
		** On renvoie le nbre d enregistrement traite : 0
		*/
		*pa_Nb_Archiv = 0;
		
		XZST_03EcritureTrace(XZSTC_FONCTION,"OUT  : xzca01_Lecture_Catalogue a echoue");
		return(XDC_NOK);
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1,"xzca01_Lecture_Catalogue : ouverture du fichier de BL %s reussi",pl_NomFile);
		
	}
	/*A
	** Allocation pour la lecture de la premiere ligne du BL.
	*/
	pl_Tab_EltArchiv = (XZCAT_ComposantCAT * )malloc (sizeof(XZCAT_ComposantCAT));
	if(pl_Tab_EltArchiv == NULL)
	{
		xzca_Free_Tab_EltArchiv(pl_Tab_EltArchiv,1);
		XZST_03EcritureTrace(XZSTC_WARNING,"xzca01_Lecture_Catalogue : Probleme d allocation memoire pour le %d eme element du BL",vl_indice+1);
		fclose(pl_FD);
		return(XDC_NOK);
	}	
	/*A
	** Lecture du fichier de BL
	*/
	vl_indice = 0;
	
	/*while ((vl_retcode = fread(&(pl_Tab_EltArchiv[vl_indice]),sizeof(XZCAT_ComposantCAT),1,pl_FD))==1)*/
	while ((fread(&(pl_Tab_EltArchiv[vl_indice]),sizeof(XZCAT_ComposantCAT),1,pl_FD))==1)
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1,"xzca01_Lecture_Catalogue a lu pour la BandeID %ld le nbre de copie %ld",
				pl_Tab_EltArchiv[vl_indice].BandeID,
				pl_Tab_EltArchiv[vl_indice].NumCopie);	
		vl_indice++;
		pl_Tab_EltArchiv = (XZCAT_ComposantCAT * )realloc (pl_Tab_EltArchiv,(((size_t) vl_indice)+1) * sizeof(XZCAT_ComposantCAT));
		if(pl_Tab_EltArchiv == NULL)
		{
			xzca_Free_Tab_EltArchiv(pl_Tab_EltArchiv,vl_indice);
			XZST_03EcritureTrace(XZSTC_WARNING,"xzca01_Lecture_Catalogue : Probleme d allocation memoire pour le %d eme element du catalogue des archives",vl_indice+1);
			fclose(pl_FD);
			return(XZCAC_ERR_LECTURE);
		}
	}
	/*A
	** Si tout s est bien passe : on fait appel a la fonction utilisateur pour chaque element 
	** du tableau constitue a partir du BL.
	*/
	for (vl_indice2=0 ; vl_indice2 < vl_indice ; vl_indice2++)
	{
		if((vl_retcode = (*pa_fct_util) (pl_Tab_EltArchiv[vl_indice2],vl_indice2+1))!=XDC_OK)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"xzca01_Lecture_Catalogue : la fonction utilisateur appelee sur le %d eme element est sortie en erreur",vl_indice2+1);
			fclose(pl_FD);
			xzca_Free_Tab_EltArchiv(pl_Tab_EltArchiv,vl_indice);
			return(XDC_NOK);
		}
	}/* fin du for */
	fclose(pl_FD);
	/*A
	** Desallocation du tableau ayant recu les elements du BL
	*/
	xzca_Free_Tab_EltArchiv(pl_Tab_EltArchiv,vl_indice);
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : xzca01_Lecture_Catalogue ");
	return(XDC_OK);
}
/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	xzca02_Sauvegarder_Archive permet de declencher l ordre de
*	sauvegarde du mois de l annee passes en argument . 
*	Algo : 	Verification de la valeur de l annee et du mois recu recue,
*	constitue un msg RTwks et envoie le message RTWks XDM_CSV_ARC.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int xzca02_Sauvegarder_Archive (XDY_Entier va_Mois, XDY_Entier va_Annee) 
/*
* ARGUMENTS EN ENTREE :
*
*	va_Mois de l annee va_Annee a sauvegarder
*	va_Annee est sur 4 digit et est superieure a 1994 et superieure a la date actuelle
*  - 5 ans et inferieure a la date actuelle -mois.
*
* ARGUMENTS EN SORTIE :
*
*   aucun
*
* CODE RETOUR : 
*
*  XDC_OK si tout s est bien passe
*  XZCAC_ERR_MOIS	: le numero de mois n est pas valable.
*  XZCAC_ERR_ANNEE	: Annee du mois a archiver n est pas ou plus dans la base histo
*  sinon renvoie XDC_NOK.
*
* CONDITION D'UTILISATION
*
*	Il faut etre connecte au RTserveur du CI.	
*	Il faut que l annee passee en argument soit comprise
*
* FONCTION 
*	Fonction permettant d envoyer le message XDM_CSV_ARC.
*	Le message RTwks ne contient pas de grammaire.
* 	
------------------------------------------------------*/
{
	static char *	version = "$Id: xzca.c,v 1.4 2010/12/08 22:34:39 gesconf Exp $	: xzca02_Sauvegarder_Archive" ;
	
	XDY_Datagroup   vl_datagroup;
        T_BOOL		vl_EtatDg  = FALSE;
        int		vl_retcode = XDC_NOK;
        
	XZST_03EcritureTrace(XZSTC_INTERFACE,"IN  : xzca02_Sauvegarder_Archive a recu le mois %d de l annee %d a archiver sur bande",va_Mois,va_Annee);
	
	if ((va_Mois > 12) && (va_Mois < 1))
	{
		XZST_03EcritureTrace ( XZSTC_WARNING, "xzca02_Sauvegarder_Archive a recu un mois incorrect");
		XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT : xzca02_Sauvegarder_Archive : Fin d execution");
		return(XZCAC_ERR_MOIS);
	}
	if (va_Annee<1994)
	{
		XZST_03EcritureTrace ( XZSTC_WARNING, "xzca02_Sauvegarder_Archive a recu une annee incorrecte");
		XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT : xzca02_Sauvegarder_Archive : Fin d execution");
		return(XZCAC_ERR_ANNEE);
	}
	
	/*A
	**  Constitution du nom du datagroup a joindre.
	*/
	
	strcpy(vl_datagroup,XDG_CBDARCHIVE);
	
	/*A
	** Envoi du message XDM_CSV_ARC au DG XDG_CBDARCHIVE
	*/ 

	if (TipcSrvMsgWrite(vl_datagroup,
				XDM_IdentMsg(XDM_CSV_ARC) ,
				XDM_FLG_SRVMSGWRITE,
				T_IPC_FT_INT4,
				va_Mois,
				T_IPC_FT_INT4,
				va_Annee,
				NULL))
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1,"xzca02_Sauvegarder_Archive a reussi a envoyer le msg XDM_CSV_ARC" );	
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_WARNING ,"xzca02_Sauvegarder_Archive : TipcSrvMsgWrite a echoue");	
		XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT : xzca02_Sauvegarder_Archive a echoue : Sortie");
		return(XDC_NOK);	 
	}
	/*A
	** Fin de l interface xzca02_Sauvegarder_Archive
	*/
	XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT : xzca02_Sauvegarder_Archive a reussi : Fin d execution");
	return(XDC_OK);
}
/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	xzca03_Restaurer_Archive permet de declencher l ordre de
*	retauration du mois de l annee passe en argument . 
*	Algo : 	Verification de la valeur de l annee et du mois recu recue,
*	constitue un msg RTwks et envoie le message RTWks XDM_CSV_RES.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int xzca03_Restaurer_Archive (XDY_Entier va_Mois, XDY_Entier va_Annee) 
/*
* ARGUMENTS EN ENTREE :
*
*	va_Mois de l annee va_Annee a restaurer
*	va_Annee est sur 4 digit et est superieure a 1994.
*
* ARGUMENTS EN SORTIE :
*
*   aucun
*
* CODE RETOUR : 
*
*  XDC_OK si tout s est bien passe
*  XZCAC_ERR_MOIS	: le numero de mois n est pas valable.
*  XZCAC_ERR_ANNEE	: Annee du mois a restaurer n est pas valide.
*  sinon renvoie XDC_NOK.
*
* CONDITION D'UTILISATION
*
*	Il faut etre connecte au RTserveur du CI.	
*	Il faut que l annee passee en argument soit comprise
*
* FONCTION 
*	Fonction permettant d envoyer le message XDM_CSV_RES.
*	Le message RTwks ne contient pas de grammaire.
* 	
------------------------------------------------------*/
{
	static char *	version = "$Id: xzca.c,v 1.4 2010/12/08 22:34:39 gesconf Exp $	: xzca03_Restaurer_Archive" ;
	
	XDY_Datagroup   vl_datagroup;
        T_BOOL		vl_EtatDg  = FALSE;
        int		vl_retcode = XDC_NOK;
        
	XZST_03EcritureTrace(XZSTC_INTERFACE,"IN  : xzca03_Restaurer_Archive a recu le mois %d de l annee %d a archiver sur bande",va_Mois,va_Annee);
	
	if ((va_Mois > 12) && (va_Mois < 1))
	{
		XZST_03EcritureTrace ( XZSTC_WARNING, "xzca03_Restaurer_Archive a recu un mois incorrect");
		XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT : xzca02_Sauvegarder_Archive : Fin d execution");
		return(XZCAC_ERR_MOIS);
	}

	if (va_Annee<1994)
	{
		XZST_03EcritureTrace ( XZSTC_WARNING, "xzca03_Restaurer_Archive a recu une annee incorrecte");
		XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT : xzca03_Restaurer_Archive : Fin d execution");
		return(XZCAC_ERR_ANNEE);
	}
	
	/*A
	**  Constitution du nom du datagroup a joindre.
	*/
	
	strcpy(vl_datagroup,XDG_CBDARCHIVE);
	
	/*A
	** Envoi du message XDM_CSV_RES au DG XDG_CBDARCHIVE
	*/ 

	if (TipcSrvMsgWrite(vl_datagroup,XDM_IdentMsg(XDM_CSV_RES) ,
				XDM_FLG_SRVMSGWRITE,
				T_IPC_FT_INT4,va_Mois,
				T_IPC_FT_INT4,va_Annee,
				NULL))
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1,"xzca03_Restaurer_Archive a reussi a envoyer le msg XDM_CSV_RES" );	
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_WARNING ,"xzca03_Restaurer_Archive : TipcSrvMsgWrite a echoue");	
		XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT : xzca03_Restaurer_Archive a echoue : Sortie");
		return(XDC_NOK);	 
	}
	/*A
	** Fin de l interface xzca03_Restaurer_Archive
	*/
	XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT : xzca03_Restaurer_Archive a reussi : Fin d execution");
	return(XDC_OK);
}
/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	xzca_Free_Tab_EltArchiv libere le tab de lecture du BL alloue
*  dynamiquement.
* 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int xzca_Free_Tab_EltArchiv (XZCAT_ComposantCAT *pa_Tab_ComposantARC,int va_Nb_Elt_ComposantARC) 

/*
* ARGUMENTS EN ENTREE :
*	pa_Tab_ComposantARC contient le tableau de structure decrivant un enregistrement 
*  de l archivage a dealloue.
*	va_Nb_Elt_ComposantARC contient le nbre d element constituant le tableau.
* ARGUMENTS EN SORTIE :
*   aucun
* CODE RETOUR : 
*
*  XDC_OK si tout s est bien passe
* CONDITION D'UTILISATION
* FONCTION 
* 	
------------------------------------------------------*/
{
	static char *	version = "$Id: xzca.c,v 1.4 2010/12/08 22:34:39 gesconf Exp $	: xzca_Free_Tab_EltArchiv" ;
	int		vl_indice = 0;
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN  :  xzca_Free_Tab_EltArchiv : Debut d execution");
	free (pa_Tab_ComposantARC);
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT  : xzca_Free_Tab_EltArchiv : Fin d execution");

	return(XDC_OK);
}
/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	xzca_EnvoieNomMachineLocal permet d envoyer a la tache TCSVA le nom de 
*  la machine sur laquelle on doit faire parvenir les message a renvoyer a 
*  l administrateur.
*	Algo : 	Recuperation du nom de la machine locale,
*	constitue un msg RTwks et envoie le message RTWks XDM_CSV_CANAL.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int xzca_EnvoieNomMachineLocal () 
/*
* ARGUMENTS EN ENTREE : aucun
*
* ARGUMENTS EN SORTIE :  aucun
*
* CODE RETOUR : 
*
*  XDC_OK si tout s est bien passe
*  sinon renvoie XDC_NOK.
*
* CONDITION D'UTILISATION
*
*	Il faut etre que la machine locale soit connue par le noyau de SUPERVISON.	
*
* FONCTION 
*	Fonction permettant d envoyer le message XDM_CSV_CANAL.
* 	
------------------------------------------------------*/
{
	static char *	version = "$Id: xzca.c,v 1.4 2010/12/08 22:34:39 gesconf Exp $	: xzca_EnvoieNomMachineLocal" ;
	
	XDY_Datagroup   vl_datagroup;
        T_BOOL		vl_EtatDg  	= FALSE;
        int		vl_retcode 	= XDC_NOK;
        XDY_NomMachine  vl_NomMachine	= "";
        
	XZST_03EcritureTrace(XZSTC_INTERFACE,"IN  : xzca_EnvoieNomMachineLocal : Debut d execution");
	
	
	/*A
	** Recuperation du nom de la machine 
	*/
	if ( (vl_retcode = XZSC_07NomMachine(vl_NomMachine)) != XDC_OK)
	{
		XZST_03EcritureTrace( XZSTC_WARNING, " xzca_EnvoieNomMachineLocal : Recuperation du Nom de la machine impossible .");
		return(XDC_NOK);
	} 
	else
	{
		XZST_03EcritureTrace( XZSTC_DEBUG1, " xzca_EnvoieNomMachineLocal : Recuperation du Nom de la machine : %s.",vl_NomMachine);
	}
	
	
	/*A
	**  Constitution du nom du datagroup a joindre.
	*/
	
	strcpy(vl_datagroup,XDG_CBDARCHIVE);
	
	/*A
	** Envoi du message XDM_CSV_CANAL au DG XDG_CBDARCHIVE 
	*/ 

	if (TipcSrvMsgWrite(vl_datagroup,XDM_IdentMsg(XDM_CSV_CANAL) ,
				XDM_FLG_SRVMSGWRITE,
				T_IPC_FT_STR,vl_NomMachine,
				NULL))
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1,"xzca_EnvoieNomMachineLocal a reussi a envoyer le msg XDM_CSV_CANAL" );	
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_WARNING ,"xzca_EnvoieNomMachineLocal : TipcSrvMsgWrite a echoue");	
		XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT : xzca_EnvoieNomMachineLocal a echoue : Sortie");
		return(XDC_NOK);	 
	}
	/*A
	** Fin de l interface xzca_EnvoieNomMachineLocal
	*/
	XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT : xzca_EnvoieNomMachineLocal a reussi : Fin d execution");
	return(XDC_OK);
}
/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	xzca_Sauvegarder_Hebdo permet de declencher l ordre de
*	sauvegarde hebdomadaire de la base de config (globale) et de la base
*  historique en incrementale. 
*	Algo : 	Constitue un msg RTwks et envoie le message RTWks XDM_CSV_SBD.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int xzca_Sauvegarder_Hebdo () 
/*
* ARGUMENTS EN ENTREE :  aucun
*
* ARGUMENTS EN SORTIE : aucun
*
* CODE RETOUR : 
*
*  XDC_OK si tout s est bien passe
*  sinon renvoie XDC_NOK.
*
* CONDITION D'UTILISATION
*
*	Il faut etre connecte au RTserveur du CI.	
*
* FONCTION 
*	Fonction permettant d envoyer le message XDM_CSV_SBD.
*	Le message RTwks ne contient pas de grammaire.
* 	
------------------------------------------------------*/
{
	static char *	version = "$Id: xzca.c,v 1.4 2010/12/08 22:34:39 gesconf Exp $	: xzca_Sauvegarder_Hebdo" ;
	
	XDY_Datagroup   vl_datagroup;
        T_BOOL		vl_EtatDg  = FALSE;
        int		vl_retcode = XDC_NOK;
        
	XZST_03EcritureTrace(XZSTC_INTERFACE,"IN  : xzca_Sauvegarder_Hebdo : debut d execution");
	
	/*A
	**  Constitution du nom du datagroup a joindre.
	*/
	
	strcpy(vl_datagroup,XDG_CBDARCHIVE);
	
	/*A
	** Envoi du message XDM_CSV_SBD au DG XDG_CBDARCHIVE
	*/ 

	if (TipcSrvMsgWrite(vl_datagroup,
				XDM_IdentMsg(XDM_CSV_SBD) ,
				XDM_FLG_SRVMSGWRITE,
				NULL))
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1,"xzca_Sauvegarder_Hebdo a reussi a envoyer le msg XDM_CSV_SBD" );	
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_WARNING ,"xzca_Sauvegarder_Hebdo : TipcSrvMsgWrite a echoue");	
		XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT : xzca_Sauvegarder_Hebdo a echoue : Sortie");
		return(XDC_NOK);	 
	}
	/*A
	** Fin de l interface xzca_Sauvegarder_Hebdo
	*/
	XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT : xzca_Sauvegarder_Hebdo a reussi : Fin d execution");
	return(XDC_OK);
}
