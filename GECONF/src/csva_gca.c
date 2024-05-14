/*E*/
/*Fichier : $Id: csva_gca.c,v 1.1 1995/01/25 11:34:59 mercier Exp $      Release : $Revision: 1.1 $        Date : $Date: 1995/01/25 11:34:59 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  GECONF
------------------------------------------------------
* MODULE GCA * FICHIER csva_gca.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module contenant les fonctions de gestion du catalogue des archives.
* (DCG-002/0.1)
*
------------------------------------------------------
* HISTORIQUE :
*
* mercier	06 Jan 1995	: Creation
------------------------------------------------------*/

/* fichiers inclus */

#include "csva_gca.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: csva_gca.c,v 1.1 1995/01/25 11:34:59 mercier Exp $ : csva_gca" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X*/
/*
------------------------------------------------------
* SERVICE RENDU : 
*  Fonction permettant d ecrire d un enregistrement dans un fichier du 
*  catalogue des archives. Cette enregistrement contient le numero logique
*  de la bande (constituer de 6 digits : 199401 pour le mois de janvier de
*  l annee 1994), avec le nombre de copie.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cgca_EcritCat( XZCAT_ComposantCAT va_CompCAT)

/*
* ARGUMENTS EN ENTREE :
*   va_CompCAT contient le numero de bande equivalent a la concatenation du mois 
*   et de l annee. avec le nbre de copie courante.
*
* ARGUMENTS EN SORTIE :  Aucun
*
* CODE RETOUR : 
*  	XDC_OK si tout s est bien passe sinon
*	XDC_NOK
*
* CONDITION D'UTILISATION
*   Aucune
*
* FONCTION 
*
------------------------------------------------------*/
{
	static char *version = "$Id: csva_gca.c,v 1.1 1995/01/25 11:34:59 mercier Exp $ : cgca_EcritCat" ;
	int            	      	vl_ValRet = XDC_OK;
	FILE 	* 		pl_FD;
	char			pl_NomFile[255];
	XZCAT_ComposantCAT * 	pl_Tab_EltArchiv;
	int			vl_indice  = 0;
	int			vl_indice2 = 0;
	int 			vl_retcode = XDC_OK;
	int			vl_Trouve  = XDC_NOK;
	
	XZST_03EcritureTrace(XZSTC_FONCTION, "IN : cgca_EcritCat () est appelee avec num bande %d ayant %d copie",
				va_CompCAT.BandeID,
				va_CompCAT.NumCopie);
	/*A
	** Ouverture du fichier du catalogue des archives.
	*/
	sprintf(pl_NomFile,"%s/%s",XDC_PATHFICDYN,XDF_CATALOGUE_ARCHIVE);	
	
	if (!(pl_FD=fopen(pl_NomFile,"r+")))
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"cgca_EcritCat : ouverture du fichier de catalogue des archives %s impossible en Read-Write",pl_NomFile);
		fclose(pl_FD);
		
		XZST_03EcritureTrace(XZSTC_FONCTION,"OUT  : cgca_EcritCat a echoue");
		return(XDC_NOK);
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1,"cgca_EcritCat : ouverture du fichier de catalogue des archives %s reussi",pl_NomFile);
		
	}
	/*A
	** Allocation pour la lecture de la premiere ligne du catalogue des archives.
	*/
	pl_Tab_EltArchiv = (XZCAT_ComposantCAT * )malloc (sizeof(XZCAT_ComposantCAT));
	if(pl_Tab_EltArchiv == NULL)
	{
		xzca_Free_Tab_EltArchiv(pl_Tab_EltArchiv,1);
		XZST_03EcritureTrace(XZSTC_WARNING,"cgca_EcritCat : Probleme d allocation memoire pour le %d eme element du catalogue des archives",vl_indice+1);
		fclose(pl_FD);
		return(XDC_NOK);
	}	
	/*A
	** Lecture du fichier de catalogue des archives
	*/
	vl_indice = 0;
	vl_Trouve = XDC_NOK;
	
	while ((vl_retcode = fread(&(pl_Tab_EltArchiv[vl_indice]),sizeof(XZCAT_ComposantCAT),1,pl_FD))==1)
		
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1,"cgca_EcritCat a lu pour la BandeID %ld le nbre de copie %ld",
						pl_Tab_EltArchiv[vl_indice].BandeID,
						pl_Tab_EltArchiv[vl_indice].NumCopie);	
		if (pl_Tab_EltArchiv[vl_indice].BandeID == va_CompCAT.BandeID)
		{
			pl_Tab_EltArchiv[vl_indice].NumCopie++;
			XZST_03EcritureTrace(XZSTC_DEBUG1,"cgca_EcritCat : le nbre de copie pour la bande %d est %d",
						pl_Tab_EltArchiv[vl_indice].BandeID,
						pl_Tab_EltArchiv[vl_indice].NumCopie);
			vl_Trouve = XDC_OK;
		}
		vl_indice++;
		pl_Tab_EltArchiv = (XZCAT_ComposantCAT * )realloc (pl_Tab_EltArchiv,(vl_indice+1) * sizeof(XZCAT_ComposantCAT));
		if(pl_Tab_EltArchiv == NULL)
		{
			xzca_Free_Tab_EltArchiv(pl_Tab_EltArchiv,vl_indice);
			XZST_03EcritureTrace(XZSTC_WARNING,"cgca_EcritCat : Probleme d allocation memoire pour le %d eme element du catalogue des archives",vl_indice+1);
			fclose(pl_FD);
			return(XZCAC_ERR_LECTURE);
		}
	}
		
	if (vl_Trouve == XDC_NOK)
	{
		pl_Tab_EltArchiv[vl_indice].BandeID = va_CompCAT.BandeID;
		pl_Tab_EltArchiv[vl_indice].NumCopie= 1;
		XZST_03EcritureTrace(XZSTC_DEBUG1,"cgca_EcritCat : Premiere sauvegarde de la bande %d",pl_Tab_EltArchiv[vl_indice].BandeID);
	}
	else vl_indice--;
	
	fseek(pl_FD,0L,SEEK_SET);
	
	vl_indice2 = 0;
	
	while (vl_indice2 <= vl_indice)
	{
		vl_retcode = fwrite(&(pl_Tab_EltArchiv[vl_indice2]),sizeof(XZCAT_ComposantCAT),1,pl_FD);
		if(vl_retcode == 1) /*Tant qu 1 ele est ecrit*/
		{
			XZST_03EcritureTrace(XZSTC_DEBUG1,"cgca_EcritCat a ecrit pour la BandeID %d le nbre de copie %d",
							pl_Tab_EltArchiv[vl_indice2].BandeID,
							pl_Tab_EltArchiv[vl_indice2].NumCopie);	
			vl_indice2++;
		}
	}
	
	fflush(pl_FD);
	fclose(pl_FD);
	/*A
	** Desallocation du tableau ayant recu les elements du catalogue des archives
	*/
	xzca_Free_Tab_EltArchiv(pl_Tab_EltArchiv,vl_indice);
			
	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cgca_EcritCat () : Sortie");
	return(XDC_OK);

}  /* Fin cgca_EcritCat */
/*X*/
/*
------------------------------------------------------
* SERVICE RENDU : 
*  Fonction permettant de recuperer le mois et l annee d archivage present
*  dans la base de sauvegarde/restauration.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int  cgca_LectMoisAnneeBD( XDY_Entier *pa_Mois,XDY_Entier *pa_Annee)

/*
* ARGUMENTS EN ENTREE :

* ARGUMENTS EN SORTIE :  Aucun
*
* CODE RETOUR : 
*  	XDC_OK si tout s est bien passe sinon
*	XDC_NOK
*
* CONDITION D'UTILISATION
*   Aucune
*
* FONCTION 
*
------------------------------------------------------*/
{
	static char *version = "$Id: csva_gca.c,v 1.1 1995/01/25 11:34:59 mercier Exp $ : cgca_LectMoisAnneeBD" ;
	
	XZST_03EcritureTrace(XZSTC_FONCTION, "IN : cgca_LectMoisAnneeBD () : Debut d execution ");
	*pa_Mois = CGCAV_MOISBD;
	*pa_Annee= CGCAV_ANNEEBD;
	XZST_03EcritureTrace(XZSTC_INFO,"cgca_LectMoisAnneeBD retourne le contenu de SVG comme le mois %d de l annee %d",
		CGCAV_MOISBD,
		CGCAV_ANNEEBD);
	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cgca_LectMoisAnneeBD () : Fin d execution ");
	return(XDC_OK);
}
/*X*/
/*
------------------------------------------------------
* SERVICE RENDU : 
*  Fonction permettant de sauvegarder le mois et l annee d archivage present
*  dans la base de sauvegarde/restauration.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int  cgca_EcritMoisAnneeBD( XDY_Entier va_Mois, XDY_Entier va_Annee)

/*
* ARGUMENTS EN ENTREE :

* ARGUMENTS EN SORTIE :  Aucun
*
* CODE RETOUR : 
*  	XDC_OK si tout s est bien passe sinon
*	XDC_NOK
*
* CONDITION D'UTILISATION
*   Aucune
*
* FONCTION 
*
------------------------------------------------------*/
{
	static char *version = "$Id: csva_gca.c,v 1.1 1995/01/25 11:34:59 mercier Exp $ : cgca_EcritMoisAnneeBD" ;
	
	XZST_03EcritureTrace(XZSTC_FONCTION, "IN : cgca_EcritMoisAnneeBD () : a recu le mois %d de l annee %d .",
			va_Mois,
			va_Annee);
	CGCAV_MOISBD  = va_Mois;
	CGCAV_ANNEEBD = va_Annee;
	XZST_03EcritureTrace(XZSTC_INFO,"cgca_EcritMoisAnneeBD a positionne le contenu de SVG au mois %d de l annee %d",
		CGCAV_MOISBD,
		CGCAV_ANNEEBD);
	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cgca_EcritMoisAnneeBD () : Fin d execution ");
	return(XDC_OK);
}
