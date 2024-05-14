/*E*/
/* Fichier : $Id: xzao465.c,v 1.3 2018/02/09 15:21:01 devgfi Exp $        Release : $Revision: 1.3 $        Date : $Date: 2018/02/09 15:21:01 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao465.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module des objets statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* VR       08 Juin 2012     : Creation (DEM/1014 PMA)
* JPL	13/07/16 : Concordance types d'arguments pour ia64 : fonction utilisateur : numero ligne XDY_Entier  1.2
* JPL		07/02/18 : Migration Linux 64 bits (DEM 1274) : Concordance constantes/types  1.3
------------------------------------------------------*/

/* fichiers inclus */

#include "xzao.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: xzao465.c,v 1.3 2018/02/09 15:21:01 devgfi Exp $ : xzao465" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*
*    Lit le fichier de configuration des destination temps de parcours et
*    appelle la fonction utilisateur de GECONF
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int xzao465_Lire_Config_Destination_TDP_PMA ( 	FILE  		*pa_Fd,
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
* XDY_Entier		*pa_Resultat:OK=NULL ou ligne de l'erreur du systeme
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
*       xzao465_Lire_Config_Destination_TDP 	contenu de ligne
*       XDY_Entier 		NumLigne
*    La fonction utilisateur retourne XDC_OK ou XDC_NOK . arret code de la lecture
*    et code de retour XDC_NOK
*
* MECANISMES :
*
* UNIX28
------------------------------------------------------*/

{
   static char *version = "$Id: xzao465.c,v 1.3 2018/02/09 15:21:01 devgfi Exp $ : xzao465_Lire_Config_Destination_TDP_PMA" ;

   XZAOT_ConfDestPMA	vl_Dest;
   char			vl_Lig[1000];
   char			*vl_Texte;
   char			vl_Texte_out[1000];
   XDY_Entier 		vl_num_ligne = 0;
   int  		vl_retcode = 0;
   XDY_Entier		vl_Resultat_out = 0;
	int		vl_init=1;

	XZST_03EcritureTrace( XZSTC_FONCTION,"IN : xzao465_Lire_Config_Destination_TDP_PMA");


   /*A
   ** Test sur le file descriptor passe en parametre d'entree
   */

   if (pa_Fd == NULL)
   {
     XZST_03EcritureTrace(XZSTC_MESSAGE,"xzao465: fichier non existant");
     return ( XDC_NOK );
   }

   /*A
   ** Boucle effectuant la lecture ligne par ligne du fichier correspondant a pa_Fd
   */

      	vl_init = 1;
   /*while ( ( vl_retcode = XZAO000_LireLigneFich ( pa_Fd, vl_Lig, sizeof(vl_Lig), &vl_Resultat_out ) ) != XDC_NOK )
   */
   while (fgets(vl_Lig,1000,pa_Fd) != NULL)
   {
       /*A
       ** Comptage des lignes lues du fichier pa_Fd
       */

       XZST_03EcritureTrace( XZSTC_WARNING,"ligne lue %s",vl_Lig);
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
             ** Extraction des differents champs de vl_Dest
             */

             vl_Texte = strtok ( vl_Lig, XZAO_TAB_SEPAR );

             if ( ( vl_retcode = XZAO000_Verif_Coherence_String ( vl_Texte, XDC_Lon_NomAuto, vl_Texte_out ) ) != XDC_OK )
             {
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao465 : Saisie Autoroute non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao465 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }

             strcpy( vl_Dest.Autoroute, vl_Texte_out );


             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );

             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_LONG, vl_Texte_out ) ) != XDC_OK )
             {
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao465 : Saisie PR non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao465 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }

             vl_Dest.PR = atoi( vl_Texte_out );


             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );

             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_LONG, vl_Texte_out ) ) != XDC_OK )
             {
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao465 : Saisie sens non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao465 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }

             vl_Dest.Sens = atoi ( vl_Texte_out );



             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );

             if ( ( vl_retcode = XZAO000_Verif_Coherence_String ( vl_Texte, XDC_Lon_Nom, vl_Texte_out ) ) != XDC_OK )
             {
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao465 : Nom de destination");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao465 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }

             strcpy(vl_Dest.NomDest, vl_Texte_out );


             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );

             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_INT, vl_Texte_out ) ) != XDC_OK )
             {
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao465 : Saisie type destination non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao465 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }

             vl_Dest.TypeDest = atoi ( vl_Texte_out );

             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );

			  if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_LONG, vl_Texte_out ) ) != XDC_OK )
			  {
				 XZST_03EcritureTrace(XZSTC_WARNING,"xzao465 : Saisie Sens destination non valide");
				 XZST_03EcritureTrace(XZSTC_WARNING,"xzao465 : retourne %d",XDC_NOK);
				 return ( XDC_NOK );
			  }

             vl_Dest.SensDest = atoi ( vl_Texte_out );


             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );

             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_BIT, vl_Texte_out ) ) != XDC_OK )
             {
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao465 : Saisie Inhibition (%s) non valide",vl_Texte);
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao465 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }

             vl_Dest.Inhibition = atoi ( vl_Texte_out );

             /*A
             ** Verification que la fonction utilisateur passee en parametre est non NULL
             */

             if (pa_FonctionUtil == NULL)
             {
                /*B
                ** Fonction utilisateur inexistante =>
                ** Retour du status XDC_NOK et du numero de ligne
                */

                XZST_03EcritureTrace(XZSTC_MESSAGE,"xzao465: echec de l'appel de la fonction utilisateur");
                return ( XDC_NOK );
             }

		/*decodage de la serie des zdp entre le PMV et la destination*/
		while ( (vl_Texte=strtok(NULL,XZAO_TAB_SEPAR)) != NULL)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"xzao462 zdp : texte <%s>",
															vl_Texte);
			if ( ( vl_retcode = XZAO000_Verif_Coherence_String ( vl_Texte,
					XDC_Lon_Nom, vl_Texte_out ) ) != XDC_OK )
			{
				 XZST_03EcritureTrace(XZSTC_WARNING,"xzao465 : Saisie desti(%s) non valide",vl_Texte);
				 XZST_03EcritureTrace(XZSTC_WARNING,"xzao465 : retourne %d",XDC_NOK);
				 return ( XDC_NOK );
			}

			strcpy( vl_Dest.ZDP, vl_Texte_out );
			vl_Dest.Init=vl_init;

                /*B
                ** Appel de la fonction utilisateur passee en parametre
                */
			XZST_03EcritureTrace( XZSTC_WARNING,"xzao465_Lire_Config_Destination_TDP_PMA : NomDest = %s\tAutoroute = %s\tPR = %d\tSens = %d\tType: %d\tSensDest:%d\tNom ZDP=%s\tInhibition : %d\tInit:%d\n",
										vl_Dest.NomDest,
										vl_Dest.Autoroute,
										vl_Dest.PR,
										vl_Dest.Sens,
										vl_Dest.TypeDest,
										vl_Dest.SensDest,
										vl_Dest.ZDP,
										vl_Dest.Inhibition,
										vl_Dest.Init);

					if ( (vl_retcode = ( *pa_FonctionUtil ) ( vl_Dest, vl_num_ligne ) ) != XDC_OK )
       		         {
       		            XZST_03EcritureTrace(XZSTC_WARNING,"xzao465 : appel de FonctionUtilisateur s'est mal deroule");
       		            return ( XDC_NOK );
       		         }

			vl_init = 0;
		}
          	}
	}
      else
       /* A
       ** Fin de lecture du fichier pa_Fd
       ** Retour du status XDC_OK et du parametre *pa_Resultat = NULL
       */

       {
          *pa_Resultat = 0;
          return ( XDC_OK );
       }
   }

   /*A
   ** Fin de la boucle de lecture du fichier pa_Fd
   ** Retour du status XDC_OK et du parametre *pa_Resultat = NULL
   */
	XZST_03EcritureTrace( XZSTC_FONCTION,"OUT : xzao465_Lire_Config_Destination_TDP_PMA ");
*pa_Resultat = 0;

return XDC_OK;

}

/* Fin de la fonction  */

