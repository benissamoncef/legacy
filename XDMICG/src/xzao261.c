/*E*/
/* Fichier : $Id: xzao261.c,v 1.1 2018/01/10 19:50:24 devgfi Exp $        Release : $Revision: 1.1 $        Date : $Date: 2018/01/10 19:50:24 $
--------------------------------------------------------------------------------
* ESCOTA * PROJET MIGRAZUR
--------------------------------------------------------------------------------
* SOUS-SYSTEME BASEDD
--------------------------------------------------------------------------------
* MODULE XZAO * FICHIER xzao261.c
--------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module des objets statiques >
*
--------------------------------------------------------------------------------
* HISTORIQUE :
*
* JPL		09/01/17 : Creation  (DEM 1266)  1.1
----------------------------------------------------------------------------- */

/* fichiers inclus */

#include "xzao.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: xzao261.c,v 1.1 2018/01/10 19:50:24 devgfi Exp $ : xzao261";

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X*/
/*------------------------------------------------------------------------------
* SERVICE RENDU :
*
*    Lit le fichier de configuration des points de mesure DIT
*    et appelle la fonction utilisateur de GECONF
*
--------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int xzao261_Lire_Config_Stations_DIT ( 	FILE  		*pa_Fd,
				        XDY_FonctionInt pa_FonctionUtil,
				        XDY_Entier   	*pa_Resultat )

/*
* ARGUMENTS EN ENTREE :
*
* FILE			pa_Fd
* XDY_FonctionInt	pa_FonctionUtil
*
* ARGUMENTS EN SORTIE :
*
* XDY_Entier		*pa_Resultat : OK = NULL ou ligne de l'erreur du systeme
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
*       XZAOT_Conf_Station_DIT	contenu de ligne
*       XDY_Entier 		NumLigne
*    La fonction utilisateur retourne XDC_OK ou XDC_NOK.
*    Si elle retourne XDC_NOK, arret de la lecture et retour XDC_NOK
*
* MECANISMES :
*
------------------------------------------------------*/

{
   static char *version	= "$Id: xzao261.c,v 1.1 2018/01/10 19:50:24 devgfi Exp $ : xzao261_Lire_Config_Zon_Camera";

   XZAOT_Conf_Station_DIT	vl_Pt_Mesure_DIT;
   char			vl_Lig[255];
   char			*vl_Texte;
   char			vl_Texte_out[255];
   XDY_Entier 		vl_num_ligne = 0;
   int  		vl_retcode = 0;
   XDY_Entier		vl_Resultat_out;

   /*A
   ** Test sur le file descriptor passe en parametre d'entree
   */

   if (pa_Fd == NULL)
   {
     XZST_03EcritureTrace(XZSTC_MESSAGE,"xzao261: fichier non existant");
     return ( XDC_NOK );
   }

   /*A
   ** Boucle effectuant la lecture ligne par ligne du fichier correspondant à pa_Fd
   */

   while ( ( vl_retcode = XZAO000_LireLigneFich ( pa_Fd, vl_Lig, sizeof(vl_Lig), &vl_Resultat_out ) ) != XDC_NOK )
   {
       /*A
       ** Comptage des lignes lues du fichier pa_Fd
       */

       vl_num_ligne++;

       if (vl_Resultat_out != XZAO_EOF )
       {
          /*A
          ** Affectation du parametre de sortie egal au numero de la ligne lue
          */

          *pa_Resultat = vl_num_ligne;

          /*A
          ** Verification que la ligne n'est pas un commentaire
          ** Presence du caractere #
          */

          vl_retcode = strcspn ( vl_Lig, XZAO_COMMENT );

          if (vl_retcode != XDC_OK)

          {
             /*A
             ** Extraction des champs du fichier de configuration
             */

             vl_Texte = strtok ( vl_Lig, XZAO_TAB_SEPAR );

             if ( ( vl_retcode = XZAO000_Verif_Coherence_String ( vl_Texte, XDC_Lon_NomEqt, vl_Texte_out ) ) != XDC_OK )
             {
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao261 : Saisie  non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao261 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }

             strcpy ( vl_Pt_Mesure_DIT.Nom_Station_DIT, vl_Texte_out );



             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );

             if ( ( vl_retcode = XZAO000_Verif_Coherence_String ( vl_Texte, XDC_Lon_NomEqt, vl_Texte_out ) ) != XDC_OK )
             {
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao261 : Saisie  non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao261 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }

             strcpy ( vl_Pt_Mesure_DIT.Nom_Station_RADT, vl_Texte_out );



             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );

             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_UCHAR, vl_Texte_out ) ) != XDC_OK )
             {
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao261 : Saisie  non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao261 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }

             vl_Pt_Mesure_DIT.Sens_RADT = atoi ( vl_Texte_out );



             /*A
             ** Verification que la fonction utilisateur passee en parametre est non NULL
             */

             if (pa_FonctionUtil != NULL)
             {
                /*B
                ** Appel de la fonction utilisateur passee en parametre
                */

                if ( (vl_retcode = ( *pa_FonctionUtil ) ( vl_Pt_Mesure_DIT, vl_num_ligne ) ) != XDC_OK )
                {
                   XZST_03EcritureTrace(XZSTC_WARNING,"appel de FonctionUtilisateur s'est mal deroule");
                   return ( XDC_NOK );
                }
	     }
             else
             {
                /*B
                ** Fonction utilisateur inexistante =>
                ** Retour du status XDC_NOK et du numero de ligne
                */

                XZST_03EcritureTrace(XZSTC_MESSAGE,"xzao261: echec de l'appel de la fonction utilisateur");
                return ( XDC_NOK );
             }
          }
       }

       else

       /*A
       ** Fin de lecture du fichier pa_Fd
       ** Retour du status XDC_OK et du parametre *pa_Resultat = NULL
       */

       {
          *pa_Resultat = NULL;
          return ( XDC_OK );
       }
   }

   /*A
   ** Fin de la boucle de lecture du fichier pa_Fd
   ** Retour du status XDC_OK et du parametre *pa_Resultat = NULL
   */

*pa_Resultat = NULL;

return XDC_OK;

}

/* Fin de la fonction xzao261_Lire_Config_Stations_DIT */

