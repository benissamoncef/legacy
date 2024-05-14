/*E*/
/*Fichier : $Id: xzao.c,v 1.1 2013/02/11 18:10:14 devgfi Exp $      Release : $Revision: 1.1 $        Date : $Date: 2013/02/11 18:10:14 $
------------------------------------------------------------------------------
* ESCOTA  *  PROJET MIGRAZUR
------------------------------------------------------------------------------
* SOUS-SYSTEME  XDMICG
------------------------------------------------------------------------------
* MODULE XZAO * FICHIER xzao.c
------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Objets Statiques : gestion des autoroutes.
*
*   Maintient une table des autoroutes lue en base OPERATIONNELLE (base CFG)
*   et fournit des informations sur ces autoroutes
*   a partir de leur numero ou de leur nom.
*
------------------------------------------------------------------------------
* HISTORIQUE :
*
* JPL		11/02/13 : Creation  1.1
* ABE		17/10/2022 : Ajout xzao29, calcul zone hors reseau, deplace de etdp_don pour appel dans xzad pour taneu DEM-SAE318
----------------------------------------------------------------------------*/

/* Fichiers inclus */

#include "xzao.h"


/* Declaration de variables internes */

static char *version = "$Id: xzao.c,v 1.1 2013/02/11 18:10:14 devgfi Exp $ : xzao.c" ;

static	XZAOT_Autoroute		vm_Autoroute_Nulle = { 0, "" };

static	XZAOT_Autoroute *	tm_Autoroutes = NULL;

static	XDY_Entier		vm_Nb_Autoroutes = 0;


/* Declaration de fonctions internes */


/* Definition de fonctions externes */



/*X---------------------------------------------------------------------------
* SERVICE RENDU :
*
*  Fonction utilisateur interne memorisant les autoroutes
*  lues en base de donnees.
*
------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static int XZAO_Memoriser_Autoroutes ( XZAOT_Autoroute *ta_autoroutes,
                                       XDY_Entier       va_nb_autoroutes )

/*
* PARAMETRES EN ENTREE :
*
*   ta_autoroutes     : table des autoroutes extraites de la base ;
*   va_nb_autoroutes  : nombre d'autoroutes.
*
* PARAMETRES EN SORTIE :
*
*
* CODE RETOUR :
*
*
* CONDITION D'UTILISATION
*
* FONCTION
*
*  Creation de la table des autoroutes.
*
-----------------------------------------------------------------------------------------*/
{
	static char *version = "$Id: xzao.c,v 1.1 2013/02/11 18:10:14 devgfi Exp $ : XZAO_Autoroutes" ;
	int	i;

	tm_Autoroutes = (XZAOT_Autoroute *) calloc ((size_t) va_nb_autoroutes, sizeof (XZAOT_Autoroute));
	if (tm_Autoroutes == NULL)
	{
		XZST_03EcritureTrace (XZSTC_WARNING, "%s: allocation memoire impossible", version);
		return (XDC_NOK);
	}

	for (i = 0;  i < va_nb_autoroutes;  i++)
	{
		tm_Autoroutes[i] = ta_autoroutes[i];
		XZST_03EcritureTrace (XZSTC_INFO, "Autoroute %d <%s>, INV=%d, Escota=%d, Debut/Fin %d/%d, Concession %d/%d",
		                         tm_Autoroutes[i].numero, tm_Autoroutes[i].nom,
		                         tm_Autoroutes[i].inversee, tm_Autoroutes[i].escota,
		                         tm_Autoroutes[i].debut, tm_Autoroutes[i].fin,
		                         tm_Autoroutes[i].debut_concession, tm_Autoroutes[i].fin_concession);
	}
	vm_Nb_Autoroutes = va_nb_autoroutes;

	return (XDC_OK);
}





/*X---------------------------------------------------------------------------
* SERVICE RENDU :
*
*  Fonction interne d'interface avec la base de donnees.
*  Lit la liste des autoroutes dans la base operationnelle,
*  et conserve la liste retournee en memoire locale au module.
*
------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static int XZAO_Lire_Autoroutes ( )

/*
*
* PARAMETRES EN ENTREE :
*
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR RENDUE :
*
*  Liste des autoroutes en base operationnelle.
*
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK
*
* CONDITIONS D'UTILISATION :
*
* FONCTION :
*
* Lit en base la liste des autoroutes et la copie en local.
*
* MECANISMES :
*
----------------------------------------------------------------------------*/

{
	int	vl_cr;

	/*B
	** Si la lecture en base a deja ete realisee ou tentee
	** sans succes alors il n'y a rien a faire de plus.
	*/

	if (tm_Autoroutes != NULL)
	{
		return (XDC_OK);
	}


	/*A
	** Invoquer la procedure lisant la table complete d'autoroutes
	*/

	vl_cr = XZAO123_Liste_Complete_Autoroutes (XDC_BASE_CFG, XZAO_Memoriser_Autoroutes);
	if (vl_cr != XDC_OK)
	{
		/*B En cas d'echec de l'operation, marquer la tentative */
		tm_Autoroutes = &vm_Autoroute_Nulle;

		vm_Nb_Autoroutes = 0;
		return (XDC_NOK);
	}

	return (XDC_OK);
}





/*X---------------------------------------------------------------------------
* SERVICE RENDU :
*
*  Retourne le nom d'une autoroute de numero donne.
*
------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAO25_Nom_Autoroute ( XDY_Autoroute va_numero,
                           XDY_NomAuto   va_nom )

/*
*
* PARAMETRES EN ENTREE :
*
*  va_numero           : Numero de l'autoroute.
*
* PARAMETRES EN SORTIE :
*
*  va_nom              : Nom de l'autoroute.
*
* VALEUR RENDUE :
*
*
* CODE RETOUR :
*
*  XDC_OK si l'autoroute de numero indique existe, XDC_NOK sinon.
*
* CONDITIONS D'UTILISATION :
*
* FONCTION :
*
*  Consultation de la table d'autoroutes.
*
* MECANISMES :
*
----------------------------------------------------------------------------*/

{
	static char *version = "$Id: xzao.c,v 1.1 2013/02/11 18:10:14 devgfi Exp $ : XZAO25_Nom_Autoroute" ;

	int		i;


	XZST_03EcritureTrace ( XZSTC_FONCTION, "IN : %s %d", version, va_numero );


	/*A
	** Lire la table d'autoroutes en base si ce n'est pas deja fait
	*/

	if (tm_Autoroutes == NULL)
	{
		XZAO_Lire_Autoroutes ();
	}


	/*A
	** Rechercher l'autoroute de numero indique
	*/

	for ( i = 0;  i < vm_Nb_Autoroutes;  i++ )
	{
		if ( tm_Autoroutes[i].numero == va_numero )
		{
			strcpy (va_nom, tm_Autoroutes[i].nom);
			return (XDC_OK);
		}
	}


	strcpy (va_nom, "");
	XZST_03EcritureTrace (XZSTC_FONCTION, "OUT : %s : Autoroute %d non trouvee.", version, va_numero);
	return (XDC_NOK);
}





/*X---------------------------------------------------------------------------
* SERVICE RENDU :
*
*  Retourne le numero d'une autoroute de nom donne.
*
------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

XDY_Autoroute XZAO26_Numero_Autoroute ( XDY_NomAuto   va_nom )

/*
*
* PARAMETRES EN ENTREE :
*
*  va_nom              : Nom de l'autoroute.
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR RENDUE :
*
*  Numero de l'autoroute recherchee si elle existe, XDC_AUT_INCONNUE sinon.
*
* CODE RETOUR :
*
*
* CONDITIONS D'UTILISATION :
*
* FONCTION :
*
*  Consultation de la table d'autoroutes.
*
* MECANISMES :
*
----------------------------------------------------------------------------*/

{
	static char *version = "$Id: xzao.c,v 1.1 2013/02/11 18:10:14 devgfi Exp $ : XZAO26_Numero_Autoroute" ;

	int		i;


	XZST_03EcritureTrace ( XZSTC_FONCTION, "IN : %s %s", version, ((va_nom == NULL) ? "''" : va_nom) );


	/*A
	** Lire la table d'autoroutes en base si ce n'est pas deja fait
	*/

	if (tm_Autoroutes == NULL)
	{
		XZAO_Lire_Autoroutes ();
	}


	/*A
	** Rechercher l'autoroute de nom indique
	*/

	if ( va_nom != NULL )
	{
		for ( i = 0;  i < vm_Nb_Autoroutes;  i++ )
		{
			if ( strcmp (tm_Autoroutes[i].nom, va_nom) == 0 )
			{
				return (tm_Autoroutes[i].numero);
			}
		}
	}


	XZST_03EcritureTrace (XZSTC_FONCTION, "OUT : %s : Autoroute non trouvee.", version);
	return ((XDY_Autoroute) XDC_AUT_INCONNUE);
}





/*X---------------------------------------------------------------------------
* SERVICE RENDU :
*
*  Retourne l'ordre des PRs sur une autoroute de numero donne.
*
------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

XDY_Mot XZAO27_Ordre_PR ( XDY_Autoroute   va_numero )

/*
*
* PARAMETRES EN ENTREE :
*
*  va_numero           : Numero de l'autoroute.
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR RENDUE :
*
*  XDC_PR_NOR ou XDC_PR_INV selon l'ordre des PR sur l'autoroute.
*
* CODE RETOUR :
*
*
* CONDITIONS D'UTILISATION :
*
* FONCTION :
*
*  Consultation de la table d'autoroutes.
*
* MECANISMES :
*
----------------------------------------------------------------------------*/

{
	static char *version = "$Id: xzao.c,v 1.1 2013/02/11 18:10:14 devgfi Exp $ : XZAO27_Ordre_PR" ;

	int		i;


	XZST_03EcritureTrace ( XZSTC_FONCTION, "IN : %s %d", version, va_numero );


	/*A
	** Lire la table d'autoroutes en base si ce n'est pas deja fait
	*/

	if (tm_Autoroutes == NULL)
	{
		XZAO_Lire_Autoroutes ();
	}


	/*A
	** Rechercher l'autoroute de nom indique
	*/

	for ( i = 0;  i < vm_Nb_Autoroutes;  i++ )
	{
		if ( tm_Autoroutes[i].numero == va_numero )
		{
			return (tm_Autoroutes[i].inversee);
		}
	}


	XZST_03EcritureTrace (XZSTC_FONCTION, "OUT : %s : Autoroute %d non trouvee.", version, va_numero);
	return ((XDY_Mot) XDC_PR_NOR);
}





/*X---------------------------------------------------------------------------
* SERVICE RENDU :
*
*  Retourne les donnees d'une autoroute, a partir de son numero ou de son nom
*
------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAO28_Autoroute ( XDY_Autoroute     va_numero,
                       XDY_NomAuto       va_nom,
                       XZAOT_Autoroute  *pa_autoroute )

/*
*
* PARAMETRES EN ENTREE :
*
*  va_numero           : Numero de l'autoroute ;
*  va_nom              : Nom de l'autoroute.
*
* PARAMETRES EN SORTIE :
*
*  pa_autoroute        : Donnees de l'autoroute extraites de la table.
*
* VALEUR RENDUE :
*
*
* CODE RETOUR :
*
*  XDC_OK si l'autoroute est trouvee et retournne, XDC_NOK sinon.
*
* CONDITIONS D'UTILISATION :
*
* FONCTION :
*
*  Recherche dans la table d'autoroutes du numero ou du nom.
*
* MECANISMES :
*
----------------------------------------------------------------------------*/

{
	static char *version = "$Id: xzao.c,v 1.1 2013/02/11 18:10:14 devgfi Exp $ : XZAO28_Autoroute" ;

	int		i;


	XZST_03EcritureTrace ( XZSTC_FONCTION, "IN : %s %d/%s", version, va_numero, ((va_nom == NULL) ? "" : va_nom) );


	/*A
	** Lire la table d'autoroutes en base si ce n'est pas deja fait
	*/

	if (tm_Autoroutes == NULL)
	{
		XZAO_Lire_Autoroutes ();
	}


	/*A
	** Rechercher l'autoroute par son numero s'il est valide
	*/

	if ( va_numero != XDC_AUT_INCONNUE )
	{
		for ( i = 0;  i < vm_Nb_Autoroutes;  i++ )
		{
			if ( tm_Autoroutes[i].numero == va_numero )
			{
				(*pa_autoroute) = tm_Autoroutes[i];
				return (XDC_OK);
			}
		}
	}


	/*A
	** Rechercher l'autoroute par son nom s'il est valide
	*/

	if ( va_nom != NULL )
	{
		for ( i = 0;  i < vm_Nb_Autoroutes;  i++ )
		{
			if ( strcmp (tm_Autoroutes[i].nom, va_nom) == 0 )
			{
				(*pa_autoroute) = tm_Autoroutes[i];
				return (XDC_OK);
			}
		}
	}


	XZST_03EcritureTrace (XZSTC_FONCTION, "OUT : %s : Autoroute %d non trouvee.", version, va_numero);
	memset ((void *) pa_autoroute, '\0', sizeof (XZAOT_Autoroute));
	return (XDC_NOK);
}

/*X*/
/*------------------------------------------------------------------------------
* SERVICE RENDU :
*
* Retourne si la zone est hors réseau Escota
*-------------------------------------------------------------------------------
* SEQUENCE D'APPEL :				
*/

int XZAO29_calcul_zone_hors_reseau (XDY_Autoroute		va_autoroute_dest,
			XDY_PR				va_pr_dest,
			XDY_Sens			va_sens_o,
			XDY_Sens			va_sens_d,
			XDY_PR				*va_pr_limite,
         XDY_Autoroute     *va_autoroute_limite,
         XDY_Sens			   *va_sens_limite)

/*
* ARGUMENTS EN ENTREE :
* XDY_Autoroute   			va_autoroute
* XDY_PR				va_pr
* XDY_Sens				va_sens
*
* ARGUMENTS EN SORTIE :
*   int
*
* CODE RETOUR :
*   -1 erreur, 0 interne réseau, 1 hors réseau
*
* CONDITION D'UTILISATION
*
* FONCTION
------------------------------------------------------------------------------*/
{
   XZST_03EcritureTrace(XZSTC_INFO,"etdp_don XZAO29_calcul_zone_hors_reseau, Autoroute dest = %d, PR = %d, sens_o = %d, sens_d=%d", va_autoroute_dest, va_pr_dest,va_sens_o, va_sens_d);
	/* ORANGE / MONTPELLIER */
	if ( (va_autoroute_dest == 1 && va_pr_dest <= 17900) || va_autoroute_dest == 9 || va_autoroute_dest == 12)//A8 A7 A9
	{
		*va_pr_limite=17900;
      *va_autoroute_limite=1;
      *va_sens_limite=va_sens_o;
		return 1;
	} 
	/* Vintimille */
/*   Vitimille n'est pas "Hors" reseau car on a un equipement terrain (code zone <200) qui donne un tdp et de plus on a les donn�es TDP FCD disponible jusqu'au PR 230000
	else if ( va_autoroute_dest == 1 && va_pr_dest >= 224000)//A8
	{
	  *va_pr_limite=224000;
      *va_autoroute_limite=1;
      *va_sens_limite=va_sens_o;
		return 1;
	} */

   /* MONACO */
   else if  (va_autoroute_dest == 2 || va_autoroute_dest == 19) //
	{
		*va_pr_limite=3000;
      *va_autoroute_limite=2;
      *va_sens_limite=va_sens_o;
		return 1;
	} 
   
   /* Marseille */
	else if (va_autoroute_dest == 3 && va_pr_dest <= 26900) //A50
	{
		*va_pr_limite=2518;
      *va_autoroute_limite=4; //A501
      *va_sens_limite=va_sens_d;
		return 1;
	}
		
	else
		return 0;

return -1;
}