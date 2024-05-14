/*Fichier : $Id: xzao511.c
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao511.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module des objets statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* GGY	16/02/24	: Creation
*
------------------------------------------------------*/

/* fichiers inclus */
#include "xzao.h"

/* definitions de constantes */
/* definitions de types locaux */
/* definition de macro locales */
/* declaration de variables locales */
/* declaration de fonctions internes */
/* definition de fonctions externes */


/*------------------------------------------------------
* SERVICE RENDU : 
*
*    Lit le fichier de configuration des batiments et
*    appelle la fonction utilisateur de GECONF
* 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
extern int xzao511_Lire_Config_Bat ( 	FILE  			*va_Fd,
										XDY_FonctionInt	va_FonctionUtil,
										XDY_Entier   	*va_Resultat ) 
/*
* ARGUMENTS EN ENTREE :
*
* XDY_Fichier		va_Fd
* XDY_Fonction		va_FonctionUtil
*
* ARGUMENTS EN SORTIE :	
*
* XDY_Entier		*va_Resultat:OK=NULL ou ligne de l'erreur du systeme
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
*    Appelle la fonction utilisateur avec les arguments:
*       XZAOT_ConfBat 		contenu de ligne
*       XDY_Entier 		NumLigne
*    La fonction utilisateur retourne XDC_OK ou XDC_NOK . arret code de la lecture
*    et code de retour XDC_NOK
*
* MECANISMES :
*
* UNIX28
------------------------------------------------------*/
{
	XZAOT_ConfBat	vl_Bat;
	char			vl_Lig[285];
	char			vl_String[285];
	char			*vl_Texte;
	char			vl_Texte_out[255];
	XDY_Entier 		vl_num_ligne = 0;
	int  			vl_retcode = 0;
	int				vl_NbCarLu = 0;
	int				vl_Longueur = 0;
	XDY_Entier		vl_Resultat_out = 0;
 
	/* Test sur le file descriptor passe en parametre d'entree */
	if (va_Fd == NULL)
 	{
 		XZST_03EcritureTrace (XZSTC_MESSAGE, "xzao511: fichier non existant");
 		return (XDC_NOK );
 	}
	
	/* Boucle effectuant la lecture ligne par ligne du fichier correspondant a va_Fd */
	while ( ( vl_retcode = XZAO000_LireLigneFich ( va_Fd, vl_Lig, sizeof(vl_Lig), &vl_Resultat_out ) ) != XDC_NOK )
	{
		/* Comptage des lignes lues du fichier va_Fd */
	    vl_num_ligne++; 

		if (vl_Resultat_out != XZAO_EOF )
		{ 
			/* Affectation du parametre de sortie egal au numero de la ligne lue */
			*va_Resultat = vl_num_ligne;
	          
			/* Verification que la ligne n'est pas un commentaire
			** Presence du caractere # */
			vl_retcode = strcspn ( vl_Lig, XZAO_COMMENT );

			if (vl_retcode != 0) 
			{
				/* Extraction des differents champs */
				/* Numero */
				vl_Texte = strtok ( vl_Lig, XZAO_TAB_SEPAR );
				if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_INT, vl_Texte_out ) ) != XDC_OK )
				{  
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao511 : Saisie Numero non valide");
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao511 : retourne %d",XDC_NOK);
					return ( XDC_NOK );
				}
				vl_Bat.Numero = atoi ( vl_Texte_out );

				/* Nom */
				vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
				if ( ( vl_retcode = XZAO000_Verif_Coherence_String ( vl_Texte, XDC_Lon_Nom, vl_Texte_out ) ) != XDC_OK )
				{  
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao511 : Saisie Nom non valide");
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao511 : retourne %d",XDC_NOK);
					return ( XDC_NOK );
				}
				strcpy ( vl_Bat.NomBatiment, vl_Texte_out );

				/* Autoroute */
				vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
				if ( ( vl_retcode = XZAO000_Verif_Coherence_String ( vl_Texte, XDC_Lon_NomAuto, vl_Texte_out ) ) != XDC_OK )
				{  
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao511 : Saisie Autoroute non valide");
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao511 : retourne %d",XDC_NOK);
					return ( XDC_NOK );
				}
				strcpy ( vl_Bat.Autoroute, vl_Texte_out );

				/* PR */
				vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
				if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_LONG, vl_Texte_out ) ) != XDC_OK )
				{  
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao511 : Saisie PR non valide");
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao511 : retourne %d",XDC_NOK);
					return ( XDC_NOK );
				}
				sscanf ( vl_Texte_out, "%d", &vl_Bat.PR );

				/* Sens */
				vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
				if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_UCHAR, vl_Texte_out ) ) != XDC_OK )
				{  
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao511 : Saisie Sens non valide");
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao511 : retourne %d",XDC_NOK);
					return ( XDC_NOK );
				}
				vl_Bat.Sens = atoi ( vl_Texte_out );

				/* Abreviation */
				vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
				if ( ( vl_retcode = XZAO000_Verif_Coherence_String ( vl_Texte, XDC_Lon_Abrev, vl_Texte_out ) ) != XDC_OK )
				{  
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao511 : Saisie Abreviation non valide");
					XZST_03EcritureTrace(XZSTC_WARNING,"xzao511 : retourne %d",XDC_NOK);
					return ( XDC_NOK );
				}
				strcpy ( vl_Bat.Abreviation, vl_Texte_out );

				vl_Bat.Creation = 1;

				/* Verification que la fonction utilisateur passee en parametre est non NULL */
				if (va_FonctionUtil != NULL)
				{
				   /* Appel de la fonction utilisateur passee en parametre */
					if ( (vl_retcode = ( *va_FonctionUtil ) ( vl_Bat, vl_num_ligne ) ) != XDC_OK )
					{    
						XZST_03EcritureTrace(XZSTC_WARNING,"xzao511: appel de FonctionUtilisateur s'est mal deroule");
						return ( XDC_NOK );
					}  
				} 
				else
				{
					/* Fonction utilisateur inexistante => Retour du status XDC_NOK et du numero de ligne */
					XZST_03EcritureTrace(XZSTC_MESSAGE,"xzao511: echec de l'appel de la fonction utilisateur");
					return ( XDC_NOK );
				}
			  }
	    }
		else
		/*  Fin de lecture du fichier va_Fd => Retour du status XDC_OK et du parametre *va_Resultat = NULL */
		{  
			*va_Resultat = NULL;
			return (XDC_OK);
		}
	}
 	/* Fin de la boucle de lecture du fichier va_Fd => Retour du status XDC_OK et du parametre *va_Resultat = NULL */
	*va_Resultat = NULL;
	return XDC_OK;
}
/* Fin de la fonction xzao511_Lire_Config_Bat */
