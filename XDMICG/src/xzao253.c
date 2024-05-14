/*E*/
/* Fichier : $Id: xzao253.c,v 1.4 2018/11/05 13:06:15 devgfi Exp $        $Revision: 1.4 $        $Date: 2018/11/05 13:06:15 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao253.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module des objets statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
*  VR	22/12/11	: Creation (DEM/1016)
* JPL	13/07/16 : Concordance types d'arguments pour ia64 : fonction utilisateur : numero ligne XDY_Entier  1.2
* JPL		07/02/18 : Migration Linux 64 bits (DEM 1274) : Concordance constantes/types  1.3
* JPL		05/11/18 : Lecture de l'indicateur 'Supprime' en configuration (DEM 1305)  1.4
------------------------------------------------------*/

/* fichiers inclus */

#include "xzao.h"
#include "xzao253.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "@(#)xzao253.c	1.3 1.3 02/15/11 : xzao253" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*    Lit le fichier de configuration des zones d'influence PRV et
*    appelle la fonction utilisateur de GECONF
* 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int xzao253_Lire_Config_Zon_Regul ( 	FILE  		*pa_Fd,
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
*	contenu de la ligne
*       XDY_Entier 		NumLigne
*    La fonction utilisateur retourne XDC_OK ou XDC_NOK . arret code de la lecture
*    et code de retour XDC_NOK
*
* MECANISMES :
*
* UNIX28
------------------------------------------------------*/

{
   char			vl_Lig[255];
   char			*vl_Texte;
   char			vl_Texte_out[255];
   XDY_Entier 		vl_num_ligne = 0;
   int  		vl_retcode = 0;
   XDY_Entier		vl_Resultat_out = 0;
   XZAOT_ConfZoneReg	vl_zone_reg;
/*   
   XDY_Mot		vl_numero;
   XDY_Mot		vl_autoroute;
   XDY_Sens		vl_sens;
   XDY_PR		vl_PR_debut;
   XDY_PR		vl_PR_fin;
   XDY_Mot		vl_vitesse_amont;
   XDY_Booleen		vl_inhibition_zone;
*/
   /*A
   ** Test sur le file descriptor passe en parametre d'entree 
   */
 
   if (pa_Fd == NULL)
   {
     XZST_03EcritureTrace(XZSTC_MESSAGE,"xzao253: fichier non existant");
     return ( XDC_NOK );
   }
 
   /*A
   ** Boucle effectuant la lecture ligne par ligne du fichier
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
             ** Extraction des differents champs 
             */
    
             vl_Texte = strtok ( vl_Lig, XZAO_TAB_SEPAR );
             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_INT, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao253 : Saisie numero zone non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao253 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             vl_zone_reg.Numero = atoi ( vl_Texte_out ); 
              
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             if ( ( vl_retcode = XZAO000_Verif_Coherence_String ( vl_Texte, 50, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao253 : Saisie nom zone non valide <%s>", vl_Texte);
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao253 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             strcpy(vl_zone_reg.Nom, vl_Texte_out );

              
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             if ( ( vl_retcode = XZAO000_Verif_Coherence_String ( vl_Texte, XDC_Lon_NomAuto, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao253 : Saisie autoroute non valide <%s>", vl_Texte);
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao253 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             strcpy(vl_zone_reg.Autoroute, vl_Texte_out );

             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_INT , vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao253 : Saisie sens zone non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao253 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
			 vl_zone_reg.Sens = atoi ( vl_Texte_out );

             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_LONG, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao253 : Saisie pr debut zone non valide <%s>", vl_Texte);
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao253 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
	         vl_zone_reg.PR_Debut = atoi ( vl_Texte_out );

             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_LONG, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao253 : Saisie PR fin zone non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao253 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             vl_zone_reg.PR_Fin = atoi ( vl_Texte_out );

             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_INT, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao253 : Saisie Vitesse non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao253 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             vl_zone_reg.Vitesse = atoi ( vl_Texte_out );

             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_BIT, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao253 : Saisie Inhibition non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao253 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             vl_zone_reg.Inhibition = atoi ( vl_Texte_out );

			 vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
			 if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_BIT, vl_Texte_out ) ) != XDC_OK )
             {
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao253 : Saisie mode horaire non valide");
				XZST_03EcritureTrace(XZSTC_WARNING,"xzao253 : retourne %d",XDC_NOK);
				return ( XDC_NOK );
             }
			 vl_zone_reg.Mode_Horaire = atoi ( vl_Texte_out );


			vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
			 if ( ( vl_retcode = XZAO000_Verif_Coherence_String ( vl_Texte, XDC_Lon_NomEqt, vl_Texte_out ) ) != XDC_OK )
			 {
				XZST_03EcritureTrace(XZSTC_WARNING,"xzao253 : Saisie Nom Serveur non valide");
				XZST_03EcritureTrace(XZSTC_WARNING,"xzao253 : retourne %d",XDC_NOK);
				return ( XDC_NOK );
			 }
			 strcpy ( vl_zone_reg.NomServeur, vl_Texte_out );


             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );

             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_BIT, vl_Texte_out ) ) != XDC_OK )
             {
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao253 : Saisie Supprime non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao253 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }

             vl_zone_reg.Supprime = atoi ( vl_Texte_out );

             /*A
             ** Verification que la fonction utilisateur passee en parametre est non NULL 
             */
    
             if (pa_FonctionUtil != NULL)
             {
                /*B
                ** Appel de la fonction utilisateur passee en parametre 
                */
         
                if ( (vl_retcode = ( *pa_FonctionUtil ) ( vl_zone_reg
							  ,vl_num_ligne ) ) != XDC_OK )
                {    
                   XZST_03EcritureTrace(XZSTC_WARNING,"xzao253 : appel de FonctionUtilisateur s'est mal deroule");
                   return ( XDC_NOK );
                }  
	     } 
             else
             {
                /*B
                ** Fonction utilisateur inexistante =>
                ** Retour du status XDC_NOK et du numero de ligne 
                */

                XZST_03EcritureTrace(XZSTC_MESSAGE,"xzao253: echec de l'appel de la fonction utilisateur");
                return ( XDC_NOK );
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
   
*pa_Resultat = 0;

return XDC_OK;

}   

/* Fin de la fonction  */

