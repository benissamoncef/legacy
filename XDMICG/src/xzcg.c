/*E*/
/*Fichier : $Id: xzcg.c,v 1.6 2010/12/08 22:45:56 gesconf Exp $      Release : $Revision: 1.6 $        Date : $Date: 2010/12/08 22:45:56 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  GECONF
------------------------------------------------------
* MODULE XZCG * FICHIER xzcg.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*	Conforme au DCG-002/0.1
*
*  Le module suivant contient les fonctions d interface permettant
*  de declencher les callbacks RTwks pour la gestion de configuration.
***********************
*	xzcg01_Livraison permet de declencher la procedure de
*	livraison de la tache TCGCD gerant la configuration.
*	Algo : 	constitue un msg RTwks et
* 	envoie le message RTWks XDM_CGD_LIV.
***********************
*	xzcg02_Lecture_BL : prend un argument une fonction utilisateur
* 	que l on va appeler pour chaque occurence du fichier de Bon de livraison.
***********************
*	xzcg_Free_Tab_ComposantBL libere le tab de lecture du BL alloue
*  	dynamiquement.
***********************
*	xzcg03_Diffusion_Fichiers permet de declencher l ordre de
*	diffusion de l environnemnt de configuration vers l environnement
*	operationnel de la machine specifiee. Cette diffusion est effectuee 
*	d apres le bon de livraison ; apres chaque diffusion, les enregistrements 
*	du bon de livraison concernes sont effaces.
*	Algo : 	constitue un msg RTwks et
*  	envoie le message RTWks XDM_CGD_DIF.
***********************
*	xzcg_uti_TypeFich_Verif permet de verifier le type de fichier recu.
*	Algo : effectue des comparaisons de va_TypeFich avec des constantes
*	predefinie dans le xzcg.h.
*	Cette fonction est utilisee par xzcg03_Diffusion_Fichiers.
***********************
*	xzcg04_Verification_Coherence permet de declencher l ordre de
*	verification de la coherence de l environnement de configuration. 
*	Cette verification est effectuee sur un type de verification. 
*	Algo : 	Verification de la valeur de va_TypeVerif recue,
*	constitue un msg RTwks et envoie le message RTWks XDM_CGD_VRF.
***********************
*	xzcg_uti_TypeVerif_Verif permet de verifier le type de fichier recu.
*	Algo : effectue des comparaisons de va_TypeVerif avec des constantes
*	predefinie dans le xzcg.h. 
***********************
*	xzcg06_DuplicationBD permet de declencher l ordre de
*	duplication de la base : le parametre passe permet de savoir
*	quel type d action on doit effectuer : SAUVEGARDE ou restuaration. 
*	Cette fonction doit etre appelee deux fois : 1 pour effectuer la SAUVEGARDE
*	de CFG sur bande, 2 pour effectuer la restuaration de CFG dans CFT.
*	Entre temps il faut avoir recu un message via XZIA08 pour etre prevenu de la fin
*	de la sauvegarde, et on doit reenclencher la bande pourla restoration .
*	Algo : 	Verification de la valeur de va_TypeAction recue,
*	constitue un msg RTwks et envoie le message RTWks XDM_CGD_DBD.
***********************
***********************
------------------------------------------------------
* HISTORIQUE :
*
* Mercier	12 Oct 1994	: Creation
*************
* Mercier O.   	12 Oct 1994	: Creation de xzcg01_Livraison()
*				  Creation de xzcg03_Diffusion_Fichiers().
*************
* Mercier O.	13 Oct 1994	: Creation de xzcg_uti_TypeFich_Verif()
*				xzcg04_Verification_Coherence()
*				xzcg_uti_TypeVerif_Verif()
*************
* Mercier O.	06 Dec 1994	: Ajout de xzcg02_Lecture_BL 	V 1.2				V 1.2
*				xzcg_Free_Tab_ComposantBL
*	Modification de xzcg03_Diffusion_Fichiers : message sans champs.
*************
* Mercier O. 	17 Fev 1995 	: Correction de l envoie de Msg RtWks	V 1.4
*  On n utilise pas TipcMsgCreate
*
* P.Niepceron   08 Jun 1995	: Ajout du traitement associe a XZCGC_TYPEF_FINF V1.5
* JPL		08/12/10 : Migration architecture HP ia64 (DEM 961) : suppression warnings mineurs  1.6
------------------------------------------------------*/

/* fichiers inclus */

#include "xzcg.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: xzcg.c,v 1.6 2010/12/08 22:45:56 gesconf Exp $ : xzcg" ;

/* declaration de fonctions internes */
int xzcg_uti_TypeFich_Verif (XDY_TypeFich );
int xzcg_uti_TypeVerif_Verif (int);
int xzcg_Free_Tab_ComposantBL (XZCGT_ComposantBL *, int);

/* definition de fonctions externes */
 
/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	xzcg01_Livraison : permet de declencher la procedure de
*	livraison de la tache TCGCD gerant la configuration.
*	Algo : 	constitue un msg RTwks et
* 	envoie le message RTWks XDM_CGD_LIV.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int xzcg01_Livraison () 
/*
* ARGUMENTS EN ENTREE :
*
*   aucun
*
* ARGUMENTS EN SORTIE :
*
*   aucun
*
* CODE RETOUR : 
*
*  XDC_OK si tout s est bien passe
*  sinon renvoie XDC_NOK.
*
* CONDITION D'UTILISATION
*
*	Il faut etre connecte au RTserveur local.
*	
*
* FONCTION 
*	Fonction permettant d envoyer le message XDM_CGD_LIV.
*	Le message RTwks ne contient pas de grammaire.
* 	
------------------------------------------------------*/
{
	static char *	version = "$Id: xzcg.c,v 1.6 2010/12/08 22:45:56 gesconf Exp $	: xzcg01_Livraison" ;
	
	XDY_Datagroup   vl_datagroup;
	XDY_NomSite 	pl_NomSite;
        T_BOOL		vl_EtatDg = FALSE;
        int		vl_retcode = XDC_NOK;
        
	XZST_03EcritureTrace(XZSTC_INTERFACE,"IN  : xzcg01_Livraison : Debut d execution");
	
	/*A
	**  Constitution du nom du datagroup a joindre.
	*/
	
	strcpy(vl_datagroup,XDG_GESCONFIG);
	
	/*A
	** Envoi du message XDM_CGD_LIV au DG XDG_GESCONFIG = "GESCONFIG"
	*/ 

	if (TipcSrvMsgWrite(vl_datagroup,XDM_IdentMsg(XDM_CGD_LIV) ,XDM_FLG_SRVMSGWRITE,NULL))
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1,"xzcg01_Livraison a reussi a envoyer le msg XDM_CGD_LIV" );	
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_WARNING ,"xzcg01_Livraison : TipcSrvMsgWrite a echoue");	
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : xzcg01_Livraison a echoue : Sortie");
		return(XDC_NOK);	 
	}
	/*A
	** Fin de l interface xzcg01_Livraison
	*/
	XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT : xzcg01_Livraison : Fin d execution");
	return(XDC_OK);
}
 
/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	xzcg02_Lecture_BL : prend un argument une fonction utilisateur
* que l on va appeler pour chaque occurence du fichier de Bon de livraison.
* 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int xzcg02_Lecture_BL (XDY_FonctionInt pa_fct_util, int *pa_Nb_enregistrement) 

/*
* ARGUMENTS EN ENTREE :
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
	static char *	version = "$Id: xzcg.c,v 1.6 2010/12/08 22:45:56 gesconf Exp $	: xzcg02_Lecture_BL" ;
	FILE * 		pl_FD;
	char		pl_NomFile[255];
	XZCGT_ComposantBL * pl_Tab_ComposantBL;
	int		vl_indice  = 0;
	int		vl_indice2 = 0;
	int 		vl_retcode = XDC_OK;
	
	
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : xzcg02_Lecture_BL ");
	/*A
	** Initialise le nb d elet traite a 0 en cas de sortie anormal de l appli
	*/
	*pa_Nb_enregistrement = 0;
	
	/*A
	** Ouverture du fichier de BL des versions logiciel
	*/
	sprintf(pl_NomFile,"%s/%s",XDC_PATHCNFFICDYN,XDF_BON_LIVRAISON);	
	
	if (!(pl_FD=fopen(pl_NomFile,"r+")))
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"xzcg02_Lecture_BL : ouverture du fichier de BL %s impossible en Read-Write",pl_NomFile);
		fclose(pl_FD);
		/*B
		** On renvoie le nbre d enregistrement traite : 0
		*/
		*pa_Nb_enregistrement = 0;
		
		XZST_03EcritureTrace(XZSTC_FONCTION,"OUT  : xzcg02_Lecture_BL a echoue");
		return(XDC_NOK);
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1,"xzcg02_Lecture_BL : ouverture du fichier de BL %s reussi",pl_NomFile);
		
	}
	/*A
	** Allocation pour la lecture de la premiere ligne du BL.
	*/
	pl_Tab_ComposantBL = (XZCGT_ComposantBL * )malloc (sizeof(XZCGT_ComposantBL));
	if(pl_Tab_ComposantBL == NULL)
	{
		xzcg_Free_Tab_ComposantBL(pl_Tab_ComposantBL,1);
		XZST_03EcritureTrace(XZSTC_WARNING,"xzcg02_Lecture_BL : Probleme d allocation memoire pour le %d eme element du BL",vl_indice+1);
		fclose(pl_FD);
		return(XDC_NOK);
	}	
	/*A
	** Lecture du fichier de BL
	*/
	vl_indice = 0;
	
	while (vl_retcode != EOF)
	{
		vl_retcode = fscanf(pl_FD,"%s %s %s %s %lf",	
					pl_Tab_ComposantBL[vl_indice].TypeFic,
					pl_Tab_ComposantBL[vl_indice].NomMachine,
					pl_Tab_ComposantBL[vl_indice].NomFichier,
					pl_Tab_ComposantBL[vl_indice].NumVersion,
					&(pl_Tab_ComposantBL[vl_indice].Horodate));
		if(vl_retcode != EOF)
		{
			XZST_03EcritureTrace(XZSTC_DEBUG1,"xzcg02_Lecture_BL a lu TypeFic = %s\nNomMachine = %s\nNomFichier = %s\nNumVersion = %s\nHorodate = %lf",
							pl_Tab_ComposantBL[vl_indice].TypeFic,
							pl_Tab_ComposantBL[vl_indice].NomMachine,
							pl_Tab_ComposantBL[vl_indice].NomFichier,
							pl_Tab_ComposantBL[vl_indice].NumVersion,
							pl_Tab_ComposantBL[vl_indice].Horodate);	
			vl_indice++;
			pl_Tab_ComposantBL = (XZCGT_ComposantBL * )realloc (pl_Tab_ComposantBL,(((size_t) vl_indice)+1) * sizeof(XZCGT_ComposantBL));
			if(pl_Tab_ComposantBL == NULL)
			{
				xzcg_Free_Tab_ComposantBL(pl_Tab_ComposantBL,vl_indice);
				XZST_03EcritureTrace(XZSTC_WARNING,"xzcg02_Lecture_BL : Probleme d allocation memoire pour le %d eme element du BL",vl_indice+1);
				fclose(pl_FD);
				return(XDC_NOK);
			}
		}
	}
	/*A
	** Si tout s est bien passe : on fait appel a la fonction utilisateur pour chaque element 
	** du tableau constitue a partir du BL.
	*/
	for (vl_indice2=0 ; vl_indice2 < vl_indice ; vl_indice2++)
	{
		if((vl_retcode = (*pa_fct_util) (pl_Tab_ComposantBL[vl_indice2],vl_indice2+1))!=XDC_OK)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"xzcg02_Lecture_BL : la fonction utilisateur appelee sur le %d eme element est sortie en erreur",vl_indice2+1);
			fclose(pl_FD);
			xzcg_Free_Tab_ComposantBL(pl_Tab_ComposantBL,vl_indice);
			return(XDC_NOK);
		}
	}/* fin du for */
	fclose(pl_FD);
	/*A
	** Desallocation du tableau ayant recu les elements du BL
	*/
	xzcg_Free_Tab_ComposantBL(pl_Tab_ComposantBL,vl_indice);
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : xzcg02_Lecture_BL ");
	return(XDC_OK);
}
 
/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	xzcg_Free_Tab_ComposantBL libere le tab de lecture du BL alloue
*  dynamiquement.
* 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int xzcg_Free_Tab_ComposantBL (XZCGT_ComposantBL *pa_Tab_ComposantBL,int va_Nb_Elt_ComposantBL) 

/*
* ARGUMENTS EN ENTREE :
*	pa_Tab_ComposantBL contient le tableau de structure decrivant un enregistrement du BL
*  a dealloue.
*	va_Nb_Elt_ComposantBL contient le nbre d element constituant le tableau.
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
	static char *	version = "$Id: xzcg.c,v 1.6 2010/12/08 22:45:56 gesconf Exp $	: xzcg_Free_Tab_ComposantBL" ;
	int		vl_indice = 0;
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN  :  xzcg_Free_Tab_ComposantBL : Debut d execution");
	free (pa_Tab_ComposantBL);
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT  : xzcg_Free_Tab_ComposantBL : Fin d execution");

	return(XDC_OK);
}
/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	xzcg03_Diffusion_Fichiers permet de declencher l ordre de
*	diffusion de l environnement de configuration vers l environnement
*	operationnel de la machine specifiee. Cette diffusion est effectuee 
*	d apres le bon de livraison ; apres chaque diffusion, les enregistrements 
*	du bon de livraison concernes sont effaces.
*	Algo : 	constitue un msg RTwks et
*  	envoie le message RTWks XDM_CGD_DIF.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int xzcg03_Diffusion_Fichiers () 
/*
* ARGUMENTS EN ENTREE :
*
*   aucun
*
* ARGUMENTS EN SORTIE :
*
*   aucun
*
* CODE RETOUR : 
*
*  XDC_OK si tout s est bien passe
*   sinon renvoie XDC_NOK.
*
* CONDITION D'UTILISATION
*
*	Il faut etre connecte au RTserveur du CI.
*	
*
* FONCTION 
*	Fonction permettant d envoyer le message XDM_CGD_DIF.
*	Le message RTwks ne contient pas de grammaire.
* 	
------------------------------------------------------*/
{
	static char *	version = "$Id: xzcg.c,v 1.6 2010/12/08 22:45:56 gesconf Exp $	: xzcg03_Diffusion_Fichiers" ;
	
	XDY_Datagroup   vl_datagroup;
        T_BOOL		vl_EtatDg = FALSE;
        int		vl_retcode = XDC_NOK;
        
	XZST_03EcritureTrace(XZSTC_INTERFACE,"IN  : xzcg03_Diffusion_Fichiers : debut d execution");
	
	
	
	/*A
	** Si le type de Fich est connu on continu le traitement :*
	*/
	
	
	/*A
	**  Constitution du nom du datagroup a joindre.
	*/
	
	strcpy(vl_datagroup,XDG_GESCONFIG);
	
	/*A
	** Envoi du message XDM_CGD_DIF au DG XDG_GESCONFIG = "GESCONFIG"
	*/ 

	if (TipcSrvMsgWrite(vl_datagroup,XDM_IdentMsg(XDM_CGD_DIF) ,
				XDM_FLG_SRVMSGWRITE,
				NULL))
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1,"xzcg03_Diffusion_Fichiers a reussi a envoyer le msg XDM_CGD_DIF" );	
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_WARNING ,"xzcg03_Diffusion_Fichiers : TipcSrvMsgWrite a echoue");	
		XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT : xzcg03_Diffusion_Fichiers a echoue : Sortie");
		return(XDC_NOK);	 
	}
	/*A
	** Fin de l interface xzcg03_Diffusion_Fichiers
	*/
	XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT : xzcg03_Diffusion_Fichiers : Fin d execution");
	return(XDC_OK);
}

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	xzcg_uti_TypeFich_Verif permet de verifier le type de fichier recu.
*	Algo : effectue des comparaisons de va_TypeFichier avec des constantes
*	predefinie dans le .h.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int xzcg_uti_TypeFich_Verif (XDY_TypeFich va_TypeFichier) 

/*
* ARGUMENTS EN ENTREE :
*
*   	va_TypeFichier contient le type de fichiers a transferer (XZCGC_TYPEF_****)

*
* ARGUMENTS EN SORTIE :
*
*   aucun
*
* CODE RETOUR : 
*
*  XDC_OK si tout s est bien passe
*  XZCGC_ERR_TYPEF	: si le type de fichier passe est inconnu.
*  sinon renvoie XDC_NOK.
*
* CONDITION D'UTILISATION
*
*	Il faut etre connecte au RTserveur du CI.
*	
*
* FONCTION 
*	Fonction permettant de verifier le type de fichier.
*	Cette fct est utilisee par XZCG03.
* 	
------------------------------------------------------*/
{
	static char *	version = "$Id: xzcg.c,v 1.6 2010/12/08 22:45:56 gesconf Exp $	: xzcg_uti_TypeFich_Verif" ;
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : xzcg_uti_TypeFich_Verif a recu TypeFichier a verifier = %s",va_TypeFichier );
	
	/*A
	**	La chaine est elle vide ? 
	*/
	if (va_TypeFichier == NULL)
	{
		XZST_03EcritureTrace(XZSTC_WARNING ,"xzcg_uti_TypeFich_Verif : le type de fichier n a pas ete passe");	
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : xzcg_uti_TypeFich_Verif retourne une erreur");
		return(XDC_NOK);
	}
	/*A
	**  Comparaison de va_TypeFichier avec les types connus.
	*/
	
	if(!strcmp(va_TypeFichier,XZCGC_TYPEF_SCNT))
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1,"xzcg_uti_TypeFich_Verif a reconnu le type de Fichier %s",XZCGC_TYPEF_SCNT);
		XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : xzcg_uti_TypeFich_Verif : Fin d execution");
		return(XDC_OK);
	}
	
	if(!strcmp(va_TypeFichier,XZCGC_TYPEF_SNAV))
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1,"xzcg_uti_TypeFich_Verif a reconnu le type de Fichier %s",XZCGC_TYPEF_SNAV);
		XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : xzcg_uti_TypeFich_Verif : Fin d execution");
		return(XDC_OK);
	}
	if(!strcmp(va_TypeFichier,XZCGC_TYPEF_LPIC))
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1,"xzcg_uti_TypeFich_Verif a reconnu le type de Fichier %s",XZCGC_TYPEF_LPIC);
		XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : xzcg_uti_TypeFich_Verif : Fin d execution");
		return(XDC_OK);
	}
	
	if(!strcmp(va_TypeFichier,XZCGC_TYPEF_MPMV))
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1,"xzcg_uti_TypeFich_Verif a reconnu le type de Fichier %s",XZCGC_TYPEF_MPMV);
		XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : xzcg_uti_TypeFich_Verif : Fin d execution");
		return(XDC_OK);
	}
	
	if(!strcmp(va_TypeFichier,XZCGC_TYPEF_EQAL))
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1,"xzcg_uti_TypeFich_Verif a reconnu le type de Fichier %s",XZCGC_TYPEF_EQAL);
		XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : xzcg_uti_TypeFich_Verif : Fin d execution");
		return(XDC_OK);
	}
	
	if(!strcmp(va_TypeFichier,XZCGC_TYPEF_SYNP))
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1,"xzcg_uti_TypeFich_Verif a reconnu le type de Fichier %s",XZCGC_TYPEF_SYNP);
		XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : xzcg_uti_TypeFich_Verif : Fin d execution");
		return(XDC_OK);
	}
	
	
	if(!strcmp(va_TypeFichier,XZCGC_TYPEF_RSAD))
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1,"xzcg_uti_TypeFich_Verif a reconnu le type de Fichier %s",XZCGC_TYPEF_RSAD);
		XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : xzcg_uti_TypeFich_Verif : Fin d execution");
		return(XDC_OK);
	}
	
	if(!strcmp(va_TypeFichier,XZCGC_TYPEF_FAID))
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1,"xzcg_uti_TypeFich_Verif a reconnu le type de Fichier %s",XZCGC_TYPEF_FAID);
		XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : xzcg_uti_TypeFich_Verif : Fin d execution");
		return(XDC_OK);
	}

        if(!strcmp(va_TypeFichier,XZCGC_TYPEF_FINF))
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1,"xzcg_uti_TypeFich_Verif a reconnu le type de Fichier %s",XZCGC_TYPEF_FINF);
		XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : xzcg_uti_TypeFich_Verif : Fin d execution");
		return(XDC_OK);
	}


	if(!strcmp(va_TypeFichier,XZCGC_TYPEF_PIGN))
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1,"xzcg_uti_TypeFich_Verif a reconnu le type de Fichier %s",XZCGC_TYPEF_PIGN);
		XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : xzcg_uti_TypeFich_Verif : Fin d execution");
		return(XDC_OK);
	}
	
	
	if(!strcmp(va_TypeFichier,XZCGC_TYPEF_MFBO))
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1,"xzcg_uti_TypeFich_Verif a reconnu le type de Fichier %s",XZCGC_TYPEF_MFBO);
		XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : xzcg_uti_TypeFich_Verif : Fin d execution");
		return(XDC_OK);
	}
	
	if(!strcmp(va_TypeFichier,XZCGC_TYPEF_DORA))
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1,"xzcg_uti_TypeFich_Verif a reconnu le type de Fichier %s",XZCGC_TYPEF_DORA);
		XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : xzcg_uti_TypeFich_Verif : Fin d execution");
		return(XDC_OK);
	}
	if(!strcmp(va_TypeFichier,XZCGC_TYPEF_EQUI))
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1,"xzcg_uti_TypeFich_Verif a reconnu le type de Fichier %s",XZCGC_TYPEF_EQUI);
		XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : xzcg_uti_TypeFich_Verif : Fin d execution");
		return(XDC_OK);
	}

	if(!strcmp(va_TypeFichier,XZCGC_TYPEF_FADM))
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1,"xzcg_uti_TypeFich_Verif a reconnu le type de Fichier %s",XZCGC_TYPEF_FADM);
		XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : xzcg_uti_TypeFich_Verif : Fin d execution");
		return(XDC_OK);
	}
	if(!strcmp(va_TypeFichier,XZCGC_TYPEF_EXEC_BASEDD))
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1,"xzcg_uti_TypeFich_Verif a reconnu le type de Fichier %s",XZCGC_TYPEF_EXEC_BASEDD);
		XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : xzcg_uti_TypeFich_Verif : Fin d execution");
		return(XDC_OK);
	}
	
	if(!strcmp(va_TypeFichier,XZCGC_TYPEF_EXEC_EQUEXT))
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1,"xzcg_uti_TypeFich_Verif a reconnu le type de Fichier %s",XZCGC_TYPEF_EXEC_EQUEXT);
		XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : xzcg_uti_TypeFich_Verif : Fin d execution");
		return(XDC_OK);
	}
	
	if(!strcmp(va_TypeFichier,XZCGC_TYPEF_EXEC_GECONF))
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1,"xzcg_uti_TypeFich_Verif a reconnu le type de Fichier %s",XZCGC_TYPEF_EXEC_GECONF);
		XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : xzcg_uti_TypeFich_Verif : Fin d execution");
		return(XDC_OK);
	}
	
	if(!strcmp(va_TypeFichier,XZCGC_TYPEF_EXEC_GESIHM))
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1,"xzcg_uti_TypeFich_Verif a reconnu le type de Fichier %s",XZCGC_TYPEF_EXEC_GESIHM);
		XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : xzcg_uti_TypeFich_Verif : Fin d execution");
		return(XDC_OK);
	}
	
	if(!strcmp(va_TypeFichier,XZCGC_TYPEF_EXEC_MAINTE))
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1,"xzcg_uti_TypeFich_Verif a reconnu le type de Fichier %s",XZCGC_TYPEF_EXEC_MAINTE);
		XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : xzcg_uti_TypeFich_Verif : Fin d execution");
		return(XDC_OK);
	}
	
	if(!strcmp(va_TypeFichier,XZCGC_TYPEF_EXEC_SAIDEC))
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1,"xzcg_uti_TypeFich_Verif a reconnu le type de Fichier %s",XZCGC_TYPEF_EXEC_SAIDEC);
		XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : xzcg_uti_TypeFich_Verif : Fin d execution");
		return(XDC_OK);
	}
	
	if(!strcmp(va_TypeFichier,XZCGC_TYPEF_EXEC_SIMTST))
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1,"xzcg_uti_TypeFich_Verif a reconnu le type de Fichier %s",XZCGC_TYPEF_EXEC_SIMTST);
		XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : xzcg_uti_TypeFich_Verif : Fin d execution");
		return(XDC_OK);
	}
	
	if(!strcmp(va_TypeFichier,XZCGC_TYPEF_EXEC_SUPERV))
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1,"xzcg_uti_TypeFich_Verif a reconnu le type de Fichier %s",XZCGC_TYPEF_EXEC_SUPERV);
		XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : xzcg_uti_TypeFich_Verif : Fin d execution");
		return(XDC_OK);
	}
	
	if(!strcmp(va_TypeFichier,XZCGC_TYPEF_EXEC_XDMICG))
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1,"xzcg_uti_TypeFich_Verif a reconnu le type de Fichier %s",XZCGC_TYPEF_EXEC_XDMICG);
		XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : xzcg_uti_TypeFich_Verif : Fin d execution");
		return(XDC_OK);
	}
	
	if(!strcmp(va_TypeFichier,XZCGC_TYPEF_DEFT))
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1,"xzcg_uti_TypeFich_Verif a reconnu le type de Fichier %s",XZCGC_TYPEF_DEFT);
		XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : xzcg_uti_TypeFich_Verif : Fin d execution");
		return(XDC_OK);
	}
	
	if(!strcmp(va_TypeFichier,XZCGC_TYPEF_GLOBAL))
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1,"xzcg_uti_TypeFich_Verif a reconnu le type de Fichier %s",XZCGC_TYPEF_GLOBAL);
		XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : xzcg_uti_TypeFich_Verif : Fin d execution");
		return(XDC_OK);
	}
	/*A
	**  Si on n a pas reconnu le type de fichier on returne XZCGC_ERR_TYPEF
	*/
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : xzcg_uti_TypeFich_Verif : Fin d execution");
	return(XZCGC_ERR_TYPEF);
}
/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	xzcg04_Verification_Coherence permet de declencher l ordre de
*	verification de la coherence de l environnement de configuration. 
*	Cette verification est effectuee sur un type de verification. 
*	Algo : 	Verification de la valeur de va_TypeVerif recue,
*	constitue un msg RTwks et envoie le message RTWks XDM_CGD_VRF.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int xzcg04_Verification_Coherence (int va_TypeVerif) 
/*
* ARGUMENTS EN ENTREE :
*
*   	va_TypeVerif contient le type de verification a effectuer (XZCGC_VERIF_****).
*
* ARGUMENTS EN SORTIE :
*
*   aucun
*
* CODE RETOUR : 
*
*  XDC_OK si tout s est bien passe
*  XZCGC_ERR_VERIF	: type de Verification inconnu.
*  sinon renvoie XDC_NOK.
*
* CONDITION D'UTILISATION
*
*	Il faut etre connecte au RTserveur du CI.	
*
* FONCTION 
*	Fonction permettant d envoyer le message XDM_CGD_VRF.
*	Le message RTwks ne contient pas de grammaire.
* 	
------------------------------------------------------*/
{
	static char *	version = "$Id: xzcg.c,v 1.6 2010/12/08 22:45:56 gesconf Exp $	: xzcg04_Verification_Coherence" ;
	
	XDY_Datagroup   vl_datagroup;
        T_BOOL		vl_EtatDg  = FALSE;
        int		vl_retcode = XDC_NOK;
        
	XZST_03EcritureTrace(XZSTC_INTERFACE,"IN  : xzcg04_Verification_Coherence a recu \n\t\tTypeVerif recu = %d",va_TypeVerif);
	
	if((vl_retcode = xzcg_uti_TypeVerif_Verif (va_TypeVerif))== XZCGC_ERR_TYPEF)
	{
		XZST_03EcritureTrace ( XZSTC_WARNING, "xzcg04_Verification_Coherence a recu un type verification inconnu a effectuer !!");
		return(XZCGC_ERR_TYPEF);
	}
	
	/*A
	** Si le type de Fich est connu on continue le traitement :
	*/
	
	/*A
	**  Constitution du nom du datagroup a joindre.
	*/
	
	strcpy(vl_datagroup,XDG_GESCONFIG);
	
	/*A
	** Envoi du message XDM_CGD_VRF au DG XDG_GESCONFIG = "GESCONFIG"
	*/ 

	if (TipcSrvMsgWrite(vl_datagroup,XDM_IdentMsg(XDM_CGD_VRF) ,
				XDM_FLG_SRVMSGWRITE,
				T_IPC_FT_INT4,va_TypeVerif,
				NULL))
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1,"xzcg04_Verification_Coherence a reussi a envoyer le msg XDM_CGD_VRF" );	
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_WARNING ,"xzcg04_Verification_Coherence : TipcSrvMsgWrite a echoue");	
		XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT : xzcg04_Verification_Coherence a echoue : Sortie");
		return(XDC_NOK);	 
	}
	/*A
	** Fin de l interface xzcg04_Verification_Coherence
	*/
	XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT : xzcg04_Verification_Coherence a reussi : Fin d execution");
	return(XDC_OK);
}
/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	xzcg_uti_TypeVerif_Verif permet de verifier le type de fichier recu.
*	Algo : effectue des comparaisons de va_TypeVerif avec des constantes
*	predefinie dans le .h.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int xzcg_uti_TypeVerif_Verif (int va_TypeVerif) 

/*
* ARGUMENTS EN ENTREE :
*   	va_TypeVerif contient le type de verif a effectuer (XZCGC_VERIf_****)
* ARGUMENTS EN SORTIE :
*   aucun
* CODE RETOUR : 
*
*  XDC_OK si tout s est bien passe
*  XZCGC_ERR_TYPEF	: si le type de fichier passe est inconnu.
* CONDITION D'UTILISATION
*	Il faut etre connecte au RTserveur du CI.
* FONCTION 
*	Fonction permettant de verifier le type de Verification a effectuer.
*	Cette fct est utilisee par XZCG04.
* 	
------------------------------------------------------*/
{
	static char *	version = "$Id: xzcg.c,v 1.6 2010/12/08 22:45:56 gesconf Exp $	: xzcg_uti_TypeVerif_Verif" ;
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : xzcg_uti_TypeVerif_Verif a recu TypeVerif = %d",va_TypeVerif );
	
	/*A
	**	Type de verification connu? 
	*/
	
	if((va_TypeVerif>=XZCGC_VERIF_SCNT)&&(va_TypeVerif<=XZCGC_VERIF_GLOBAL))
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1,"xzcg_uti_TypeVerif_Verif a reconnu le type de Verification %d",va_TypeVerif);
		XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : xzcg_uti_TypeVerif_Verif : Fin d execution");
		return(XDC_OK);
	}
	/*A
	**  Si on n a pas reconnu le type de verification on returne XZCGC_ERR_VERIF
	*/
	XZST_03EcritureTrace(XZSTC_WARNING,"xzcg_uti_TypeVerif_Verif : le type de la verification n est pas connu : %d",va_TypeVerif);
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : xzcg_uti_TypeFich_Verif : Fin d execution");
	return(XZCGC_ERR_VERIF);
}
/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	xzcg06_DuplicationBD permet de declencher l ordre de
*	duplication de la base : le parametre passe permet de savoir
*	quel type d action on doit effectuer : SAUVEGARDE ou restuaration. 
*	Cette fonction doit etre appelee deux fois : 1 pour effectuer la SAUVEGARDE
*	de CFG sur bande, 2 pour effectuer la restuaration de CFG dans CFT.
*	Entre temps il faut avoir recu un message via XZIA08 pour etre prevenu de la fin
*	de la sauvegarde, et on doit reenclencher la bande pourla restoration .
*	Algo : 	Verification de la valeur de va_TypeAction recue,
*	constitue un msg RTwks et envoie le message RTWks XDM_CGD_DBD.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int xzcg06_DuplicationBD (int va_TypeAction) 
/*
* ARGUMENTS EN ENTREE :
*
*	va_TypeAction : XZCGC_SAUVEGARDE_CFG ou XZCGC_RESTAURATION_DANS_CFT
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
*	Il faut etre connecte au RTserveur du CI.	
*
* FONCTION 
*	Fonction permettant d envoyer le message XDM_CGD_DBD.
* 	
------------------------------------------------------*/
{
	static char *	version = "$Id: xzcg.c,v 1.6 2010/12/08 22:45:56 gesconf Exp $	: xzcg06_DuplicationBD" ;
	
	XDY_Datagroup   vl_datagroup;
        T_BOOL		vl_EtatDg  = FALSE;
        int		vl_retcode = XDC_NOK;
        
	XZST_03EcritureTrace(XZSTC_INTERFACE,"IN  : xzcg06_DuplicationBD a recu \n\t\tTypeAction recu = %d",va_TypeAction);
	
	/*A
	** Verification du type d action demandee
	*/
	if ((va_TypeAction != XZCGC_SAUVEGARDE_CFG) && (va_TypeAction != XZCGC_RESTAURATION_DANS_CFT))
	{
		XZST_03EcritureTrace (XZSTC_WARNING, "xzcg06_DuplicationBD a recu un type d action non connu");
		XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT : xzcg06_DuplicationBD : Fin d execution");
		return(XDC_NOK);
	}
	
	/*A
	**  Constitution du nom du datagroup a joindre.
	*/
	
	strcpy(vl_datagroup,XDG_GESCONFIG);
	
	/*A
	** Envoi du message XDM_CGD_DBD au DG XDG_GESCONFIG = "GESCONFIG"
	*/ 

	if (TipcSrvMsgWrite(vl_datagroup,XDM_IdentMsg(XDM_CGD_DBD) ,
				XDM_FLG_SRVMSGWRITE,
				T_IPC_FT_INT4,va_TypeAction,
				NULL))
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1,"xzcg06_DuplicationBD a reussi a envoyer le msg XDM_CGD_BDB" );	
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_WARNING ,"xzcg06_DuplicationBD : TipcSrvMsgWrite a echoue");	
		XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT : xzcg06_DuplicationBD a echoue : Sortie");
		return(XDC_NOK);	 
	}
	/*A
	** Fin de l interface xzcg06_DuplicationBD
	*/
	XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT : xzcg06_DuplicationBD a reussi : Fin d execution");
	return(XDC_OK);
}
/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	xzcg07_EnvoieNomMachineLocal permet d envoyer a la tache TCGCD le nom de 
*  la machine sur laquelle on doit faire parvenir les message a renvoyer a 
*  l administrateur.
*	Algo : 	Recuperation du nom de la machine locale,
*	constitue un msg RTwks et envoie le message RTWks XDM_CGD_CANAL.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int xzcg07_EnvoieNomMachineLocal () 
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
*	Fonction permettant d envoyer le message XDM_CGD_CANAL.
* 	
------------------------------------------------------*/
{
	static char *	version = "$Id: xzcg.c,v 1.6 2010/12/08 22:45:56 gesconf Exp $	: xzcg07_EnvoieNomMachineLocal" ;
	
	XDY_Datagroup   vl_datagroup;
        T_BOOL		vl_EtatDg  	= FALSE;
        int		vl_retcode 	= XDC_NOK;
        XDY_NomMachine  vl_NomMachine	= "";
        
	XZST_03EcritureTrace(XZSTC_INTERFACE,"IN  : xzcg07_EnvoieNomMachineLocal : Debut d execution");
	
	
	/*A
	** Recuperation du nom de la machine 
	*/
	if ( (vl_retcode = XZSC_07NomMachine(vl_NomMachine)) != XDC_OK)
	{
		XZST_03EcritureTrace( XZSTC_WARNING, " xzcg07_EnvoieNomMachineLocal : Recuperation du Nom de la machine impossible .");
		return(XDC_NOK);
	} 
	else
	{
		XZST_03EcritureTrace( XZSTC_DEBUG1, " xzcg07_EnvoieNomMachineLocal : Recuperation du Nom de la machine : %s.",vl_NomMachine);
	}
	
	/*A
	**  Constitution du nom du datagroup a joindre.
	*/
	
	strcpy(vl_datagroup,XDG_GESCONFIG);
	
	/*A
	** Envoi du message XDM_CGD_CANAL au DG XDG_GESCONFIG = "GESCONFIG"
	*/ 

	if (TipcSrvMsgWrite(vl_datagroup,XDM_IdentMsg(XDM_CGD_CANAL) ,
				XDM_FLG_SRVMSGWRITE,
				T_IPC_FT_STR,vl_NomMachine,
				NULL))
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1,"xzcg07_EnvoieNomMachineLocal a reussi a envoyer le msg XDM_CGD_CANAL" );	
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_WARNING ,"xzcg07_EnvoieNomMachineLocal : TipcSrvMsgWrite a echoue");	
		XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT : xzcg07_EnvoieNomMachineLocal a echoue : Sortie");
		return(XDC_NOK);	 
	}
	/*A
	** Fin de l interface xzcg07_EnvoieNomMachineLocal
	*/
	XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT : xzcg07_EnvoieNomMachineLocal a reussi : Fin d execution");
	return(XDC_OK);
}
/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	xzcg_ActivChecklist permet d envoyer a la tache TCGCD la demande 
*  d activation de la verification checklist. Cette fonction est une fonction de debug
*  et ne doit pas utilisable a partir de l IHM de l administrateur.
*	Algo : 	
*	constitue un msg RTwks et envoie le message RTWks XDM_CGD_CHECK.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int xzcg_ActivChecklist (int va_Activation) 
/*
* ARGUMENTS EN ENTREE : va_Activation
*
*  Peut prendre XDC_OK ou XDC_NOK
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
*	Fonction permettant d envoyer le message XDM_CGD_CHECK.
* 	
------------------------------------------------------*/
{
	static char *	version = "$Id: xzcg.c,v 1.6 2010/12/08 22:45:56 gesconf Exp $	: xzcg_ActivChecklist" ;
	
	XDY_Datagroup   vl_datagroup;
        T_BOOL		vl_EtatDg  	= FALSE;
        int		vl_retcode 	= XDC_NOK;
        
	XZST_03EcritureTrace(XZSTC_INTERFACE,"IN  : xzcg_ActivChecklist : Debut d execution : demande checklist passee a %d",va_Activation);
	
	
	/*A
	**  Constitution du nom du datagroup a joindre.
	*/
	
	strcpy(vl_datagroup,XDG_GESCONFIG);
	
	/*A
	** Envoi du message XDM_CGD_CHECK au DG XDG_GESCONFIG = "GESCONFIG"
	*/ 

	if (TipcSrvMsgWrite(vl_datagroup,XDM_IdentMsg(XDM_CGD_CHECK) ,
				XDM_FLG_SRVMSGWRITE,
				T_IPC_FT_INT4,
				va_Activation,
				NULL))
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1,"xzcg_ActivChecklist a reussi a envoyer le msg XDM_CGD_CHECK" );	
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_WARNING ,"xzcg_ActivChecklist : TipcSrvMsgWrite a echoue");	
		XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT : xzcg_ActivChecklist a echoue : Sortie");
		return(XDC_NOK);	 
	}
	/*A
	** Fin de l interface xzcg_ActivChecklist
	*/
	XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT : xzcg_ActivChecklist a reussi : Fin d execution");
	return(XDC_OK);
}
/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	xzcg_ChgmtBaseCour permet d envoyer a la tache TCGCD la demande 
*  de changement de base.
*	Algo : 	
*	constitue un msg RTwks et envoie le message RTWks XDM_CGD_CHGB.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int xzcg_ChgmtBaseCour (char * va_Base) 
/*
* ARGUMENTS EN ENTREE : va_Activation
*
*  Peut prendre XDC_OK ou XDC_NOK
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
*	Fonction permettant d envoyer le message XDM_CGD_CHGB.
* 	
------------------------------------------------------*/
{
	static char *	version = "$Id: xzcg.c,v 1.6 2010/12/08 22:45:56 gesconf Exp $	: xzcg_ChgmtBaseCour" ;
	
	XDY_Datagroup   vl_datagroup;
        T_BOOL		vl_EtatDg  	= FALSE;
        int		vl_retcode 	= XDC_NOK;
        
	XZST_03EcritureTrace(XZSTC_INTERFACE,"IN  : xzcg_ChgmtBaseCour : Debut d execution : Demande de passer dans la base %s",va_Base);
	
	/*A
	**  Constitution du nom du datagroup a joindre.
	*/
	
	strcpy(vl_datagroup,XDG_GESCONFIG);
	
	/*A
	** Envoi du message XDM_CGD_CHGB au DG XDG_GESCONFIG = "GESCONFIG"
	*/ 

	if (TipcSrvMsgWrite(vl_datagroup,XDM_IdentMsg(XDM_CGD_CHGB) ,
				XDM_FLG_SRVMSGWRITE,
				T_IPC_FT_STR,
				va_Base,
				NULL))
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1,"xzcg_ChgmtBaseCour a reussi a envoyer le msg XDM_CGD_CHGB" );	
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_WARNING ,"xzcg_ChgmtBaseCour : TipcSrvMsgWrite a echoue");	
		XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT : xzcg_ChgmtBaseCour a echoue : Sortie");
		return(XDC_NOK);	 
	}
	/*A
	** Fin de l interface xzcg_ChgmtBaseCour
	*/
	XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT : xzcg_ChgmtBaseCour a reussi : Fin d execution");
	return(XDC_OK);
}
