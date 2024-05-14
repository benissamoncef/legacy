/*E*/
/*  Fichier : 	$Id: xzag08.c,v 1.10 1997/05/15 09:55:27 gaborit Exp $      Release : $Revision: 1.10 $        Date : $Date: 1997/05/15 09:55:27 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  XDMICG
------------------------------------------------------
* MODULE XZAE * FICHIER xzag08.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Module d'interface des sauvegardes et restauration de bases
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.  20/11/94        : Creation
* C.T.  14/04/95        : Modif type du code retour de "system" (V 1.2-1.6)
* C.T.  12/09/95        : Lecture du compte rendu dans le fichier de trace (V 1.7)
* C.T.  12/09/95        : Lecture du compte rendu dans le fichier de trace (V 1.8)
* C.T.  08/02/96        : Ajout un scroll a la fenetre hpterm lancee (V1.9)
* P.N.  08/03/97        : lancement de la svg dans une xterm interactive (V1.10)
------------------------------------------------------*/

/* fichiers inclus */
#include "xzag08.h"

static char *version = "$Id: xzag08.c,v 1.10 1997/05/15 09:55:27 gaborit Exp $      Release : $Revision: 1.10 $        Date : $Date: 1997/05/15 09:55:27 $";

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
* Dechencher la sauvegarde automatique d'une base
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAG08_Sauvegarder_Base (	XDY_NomSite		va_Site,
				char			*va_Device,
				XZAGT_Volume 		va_Volume,
				char 			*va_Mode,
				char	 		*va_Base,
				char	 		*va_Trace,
				XDY_Entier 		va_Capacite,
				XZAGT_Bande 		va_FicTape,
				char			*va_Display)
				

/*
* ARGUMENTS EN ENTREE :
*
* XDY_NomSite	va_Site          : CI ou DN ou DM
* char*		va_Device        : nom du device a utiliser
* XZAGY_Volume	va_Volume        : nom de 6 caracteres maximum pour nommer
*                                  la sauvegarde
* char*		va_Mode          : sauvegarde de la base "globale" ou 
*                                  "incrementale"
* char*		va_Base          : SVG ou HIS ou CFG ou toutes les bases
* char*		va_Trace         : fichier de trace pour l'administrateur
* XDY_Entier	va_Capacite      : capacite de la bande
* XZAGY_Bande	va_FicTape       : fichier sur la bande 
*                                  ( optionnel : 15 caracteres min et 17 max)
* char*		va_Display       : nom machine
*
* ARGUMENTS EN SORTIE :
*
* CODE RETOUR : 
*
*  XDC_OK 
*  XDC_NOK
*
* CONDITION D'UTILISATION :
* La variable d'environnement $exec_migrazur doit etre positionnee 
* Si le nom du fichier sur la bande n'est pas renseigne, la sauvegarde
* se fait avec le nom attribue par sybase
* Si le nom du fichier sur la bande est renseigne, il doit etre de 15 caracteres
* sinon la restauration ne sera jamais realisee avec ce nom de fichier (erreur)
* Dans le cas ou la sauvegarde de toutes les bases du site est demandee :
*  - CFG, EXP, HIS 
*  - master, model, tempdb, sybsystemprocs : bases systeme
*  - CI_RSSD ou DN_RSSD ou DM_RSSD         : base pour la replication
* le nom du fichier est ignore (a implementer)
*
* FONCTION :
*
* Appelle le script XZAG08.ksh
*
* MECANISMES :
*
------------------------------------------------------*/
{
	char	vl_CommandeUnix[2000];
	char	vl_CommandeSVG[2000];
	char	*vl_Chemin;
	char	vl_LigneLue[80];
	char	vl_DerniereLigneLue[80];
	FILE	*vl_File;
	long	vl_Status;
	int	vl_CodeRetour;
	int	vl_Errno;
	int	vl_NbCaracteres;

	XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAG08_Sauvegarde_Base : Debut d execution : %s, %s, %s, %s, %s, %s, %d, %s, %s",
				va_Site, va_Device, va_Volume, va_Mode,
				va_Base, va_Trace, va_Capacite, va_FicTape, 
				va_Display);

	/*A test des parametres d'entree */
	if ((strlen(va_Volume) != XZAGC_VOLUME - 1) ||
		(!strcmp(va_Mode, XZAGC_SVG_GLOB) && 
		 !strcmp(va_Mode, XZAGC_SVG_INCR)))
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAG08: parametre incorrect");
		return XDC_ARG_INV;
	}

	/* Recuperer le chemin d'acces au executable */
	vl_Chemin = getenv("exec_migrazur");

	if (vl_Chemin == NULL)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAG08: $exec_migrazur non positionnee");
		return XZAGC_VAR_ENV_INC;
	}
	/*A Construction de la commande de sauvegarde */
	sprintf(vl_CommandeSVG,
		"%s/xzag08.ksh %s %s %s %s %s %d %s %s",vl_Chemin,
		 va_Site, va_Device, va_Volume, 
		va_Base, va_Trace, va_Capacite, va_Mode, va_FicTape);

	/*A Positionnemnt de la variable de DISPLAY */
	sprintf(vl_CommandeUnix, "export DISPLAY=%s:0.0; xterm -sb -title \"%s\" -e %s", 
		va_Display, XZAGC_TITRE_SVG, vl_CommandeSVG);
		
	/*A Creation de la fenetre d'execution de la svg */
	vl_Status = system(vl_CommandeUnix);

	/* impossible de recuperer le code retour du script avec la supervision
	XZST_03EcritureTrace(	XZSTC_INFO,
				"XZAG08: code retourne par la sauvegarde %ld", 
				vl_Status);
	vl_CodeRetour = ((vl_Status & 0xFF00) >> 8);
	*/

	/*A lecture du compte rendu dans le fichier de trace */
	vl_NbCaracteres = XZAGC_NB_CARACT_A_LIRE;
	vl_File = fopen(va_Trace,"r");
	if (vl_File != NULL) {
		while (fgets(vl_LigneLue, vl_NbCaracteres, vl_File) != NULL)
		{
			strcpy(vl_DerniereLigneLue , vl_LigneLue);
		}
		if (!strncmp(vl_DerniereLigneLue, XZAGC_MES_SVG_OK, strlen(XZAGC_MES_SVG_OK)))
			vl_CodeRetour = XDC_OK;
		else	vl_CodeRetour = XDC_NOK;
	}
	else	vl_CodeRetour = XDC_NOK;


	if (vl_CodeRetour != XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAG08: impossible de sauvegarder la base");
		return XZAGC_SVG_KO;
	}

	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAG08_Sauvegarde_Base retourne %ld", vl_Status);

	return XDC_OK;
}
