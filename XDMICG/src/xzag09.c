/*E*/
/*  Fichier : 	$Id: xzag09.c,v 1.6 1996/02/08 18:32:21 gaborit Exp $      Release : $Revision: 1.6 $        Date : $Date: 1996/02/08 18:32:21 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  XDMICG
------------------------------------------------------
* MODULE XZAE * FICHIER xzag09.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Module d'interface des sauvegardes et restauration de bases
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.  21/11/94        : Creation
* C.T.  14/04/95        : Modif type du code retour de "system" (V 1.2-1.4)
* C.T.  12/09/95        : Lecture du compte rendu dans le fichier de trace (V 1.7)
* C.T.  08/02/96        : Ajout un scroll a la fenetre hpterm lancee (V1.6)
------------------------------------------------------*/

/* fichiers inclus */
#include "xzag09.h"

static char *version = "$Id: xzag09.c,v 1.6 1996/02/08 18:32:21 gaborit Exp $      Release : $Revision: 1.6 $        Date : $Date: 1996/02/08 18:32:21 $";

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
* Dechencher la restauration automatique d'une base
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAG09_Restaurer_Base (	XDY_NomSite		va_Site,
				char			*va_Device,
				XZAGT_Volume 		va_Volume,
				char	 		*va_Base,
				char	 		*va_Trace,
				char	 		*va_TraceTempo,
				XZAGT_Bande 		va_FicTape,
				char			*va_Display)
				

/*
* ARGUMENTS EN ENTREE :
*
* XDY_NomSite	va_Site          : CI ou DN ou DM
* char*		va_Device        : nom du device a utiliser
* XZAGY_Volume	va_Volume        : nom de 6 caracteres maximum pour nommer
*                                  la sauvegarde
* char*		va_Base          : SVG ou HIS ou CFG ou toutes les bases
* char*		va_Trace         : fichier de trace pour l'administrateur
* char*		va_TraceTempo    : fichier de trace temporaire de travail
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
* Si le nom du fichier sur la bande n'est pas renseigne, la restauration
* se fait avec le nom attribue par sybase
* Si le nom du fichier sur la bande est renseigne, il doit etre de 15 caracteres
* sinon la restauration ne sera jamais realisee avec ce nom de fichier (erreur)
*
* FONCTION :
*
* Appelle le script XZAG09.ksh
*
* MECANISMES :
*
------------------------------------------------------*/
{
	char	vl_CommandeUnix[2000];
	char	*vl_Chemin;
	char	vl_LigneLue[80];
	char	vl_DerniereLigneLue[80];
	FILE	*vl_File;
	long	vl_Status;
	int	vl_CodeRetour;
	int	vl_NbCaracteres;

	XZST_03EcritureTrace( XZSTC_INTERFACE,"IN : XZAG09_Restaurer_Base : Debut d execution : %s, %s, %s, %s, %s, %s, %s, %s",
				va_Site, va_Device, va_Volume, 
				va_Base, va_Trace, va_TraceTempo, va_FicTape, 
				va_Display);

	/*A test des parametres d'entree */
	if (strlen(va_Volume) != XZAGC_VOLUME - 1)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAG09: parametre incorrect");
		return XDC_ARG_INV;
	}

	/* Recuperer le chemin d'acces au executable */
	vl_Chemin = getenv("exec_migrazur");

	if (vl_Chemin == NULL)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAG09: $exec_migrazur non positionnee");
		return XZAGC_VAR_ENV_INC;
	}

	/*A Positionnemnt de la variable de DISPLAY */
	sprintf(vl_CommandeUnix, "export DISPLAY=%s:0.0; hpterm -C -sb -T \"%s\" &", 
		va_Display, XZAGC_TITRE_REST);

	/*A Creation de la fenetre de lecture de suivi de compte rendu */
	vl_Status = system(vl_CommandeUnix);

	/* attente de la fenetre */
	vl_Status = system("sleep 3");

	/*A Construction de la commande de sauvegarde */
	sprintf(vl_CommandeUnix,
		"cd %s;xzag09.ksh %s %s %s %s %s %s %s > /dev/console",
		vl_Chemin, va_Site, va_Device, va_Volume, 
		va_Base, va_Trace, va_TraceTempo, va_FicTape);

	/*A lancement de la demande de sauvegarde de la base */
	vl_Status = system(vl_CommandeUnix);

	/* impossible de recuperer le code de retour systeme
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
		if (!strncmp(vl_DerniereLigneLue, XZAGC_MES_RST_OK, strlen(XZAGC_MES_RST_OK)))
			vl_CodeRetour = XDC_OK;
		else	vl_CodeRetour = XDC_NOK;
	}
	else	vl_CodeRetour = XDC_NOK;

	if (vl_CodeRetour != XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"XZAG09: impossible de restaurer la base");
		return XZAGC_SVG_KO;
	}

	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAG09_Restaurer_Base retourne %d", vl_Status);

	return XDC_OK;
}
