/*E*/
/*Fichier : $Id: xzdc.c,v 1.4 2010/12/07 12:01:16 pc2dpdy Exp $      Release : $Revision: 1.4 $        Date : $Date: 2010/12/07 12:01:16 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  XDMICG
------------------------------------------------------
* MODULE XZDC_coherence * FICHIER xzdc.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   cf fichier xzdc.h 
*
------------------------------------------------------
* HISTORIQUE :
*
* IT	09 Jan 1995	: Creation
*************
* MERCIER 	15 Jan 1995	: Modification XZDC01 	V 1.3
* test le retour de la fonction utilisateur afin de  s affranchir
* d une variable globale
* JMG	26/06/10	linux DEM/934
------------------------------------------------------*/

/* fichiers inclus */

#include "xzdc.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: xzdc.c,v 1.4 2010/12/07 12:01:16 pc2dpdy Exp $ : xzdc" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */




/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  verifie que les types d'astreintes utilises dans la
*  BDC courante sont bien definis en base de donnees
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int XZDC01_LireAstreintesSAD(Fd, Fct, NbEnrg)
FILE		*Fd;
XDY_FonctionInt	Fct;
int		*NbEnrg;

/*
* ARGUMENTS EN ENTREE :
*   FILE		*Fd;
*   XDY_Fonction	Fct;
*
* ARGUMENTS EN SORTIE :
*   int			*NbEnrg;
*
* CODE RETOUR : 
*   XDC_OK / XDC_NOK 
*
* CONDITION D'UTILISATION
*   a appeler avant d'autoriser une modification des types d'astreintes
*   dans la base de donnees
*
* FONCTION 
*   appel de la fonction utilisateur passee en argument
*   pour chaque type d'astreinte utilise dans la BDC courante
*
------------------------------------------------------*/
{
	XDY_Nom	NomTypeAstreinte="";
	int	NumTypeAstreinte=0;
	int	i=0;
	int	vl_retcode = XDC_NOK;

	XZST_03EcritureTrace(XZSTC_INTERFACE, "IN : XZDC01_LireAstreintesSAD");

	/*A on verifie les arguments */
	*NbEnrg = 0;
	if ( Fd == NULL )
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "le file descriptor passe en argument est nul");
		return(XDC_NOK);
	}
	if ( Fct == NULL )
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "la fonction utilisateur passee en argument est nulle");
		return(XDC_NOK);
	}
	
	/*A on lit le fichier enregistrement par enregistrement */
	while ( fscanf(Fd, "%d %s", &NumTypeAstreinte, NomTypeAstreinte) == 2 )
	{
		/*B on remplace les caracteres de separation par un espace */
		for ( i=0; i<strlen(NomTypeAstreinte); i++)
		{
			if ( NomTypeAstreinte[i] == CARACTERE_DE_SEPARATION )
				NomTypeAstreinte[i] = ' ';
		}
		
		/*B on applique la fonction utilisateur a chaque enregistrement */
 		vl_retcode = ( *Fct ) (NomTypeAstreinte, (XDY_Octet) NumTypeAstreinte);
		if( vl_retcode != XDC_OK)
		{
			XZST_03EcritureTrace(XZSTC_WARNING, "Le fichier des astreintes pose un probleme sur l enregistrement %d",NbEnrg);
			return(XDC_NOK);
		}
		/*B on incremente le nombre d'enregistrements */
		*NbEnrg += 1;
	}

	/*A on traite les erreurs */
	if ( (feof(Fd) == 0 ))
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "le fichier des types d'astreinte est inconsistant");
		return(XDC_NOK);
	}

	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZDC01_LireAstreintesSAD retourne %d", XDC_OK);
	return(XDC_OK);
}



