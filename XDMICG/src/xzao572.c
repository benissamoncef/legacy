/*E*/
/* Fichier : $Id: xzao572.c,v 1.3 2018/02/09 15:22:30 devgfi Exp $        Release : $Revision: 1.3 $        Date : $Date: 2018/02/09 15:22:30 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao572.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module des objets statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* VR       18 Jan 2012     : Creation (DEM/1016)
* JPL	13/07/16 : Concordance types d'arguments pour ia64 : fonction utilisateur : numero ligne XDY_Entier  1.2
* JPL	07/02/18 : Migration Linux 64 bits (DEM 1274) : Concordance constantes/types  1.3
------------------------------------------------------*/

/* fichiers inclus */

#include "xzao.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: xzao572.c,v 1.3 2018/02/09 15:22:30 devgfi Exp $ : xzao572" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*    Lit le fichier de configuration des Eqt_PRV XDF_CONF_EQT_PRV et
*    appelle la fonction utilisateur de GECONF
* 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int xzao572_Lire_config_Eqt_PRV ( 	FILE  		*pa_Fd,
					XDY_FonctionInt pa_FonctionUtil,
					XDY_Entier   	*pa_Resultat ) 
				        
/*
* ARGUMENTS EN ENTREE :
*
* FILE			pa_Fd
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
*       xzao572_Lire_config_Eqt_PRV 	contenu de ligne
*    La fonction utilisateur retourne XDC_OK ou XDC_NOK . arret code de la lecture
*    et code de retour XDC_NOK
*
* MECANISMES :
*
* UNIX28
------------------------------------------------------*/

{
   static char *version = "$Id: xzao572.c,v 1.3 2018/02/09 15:22:30 devgfi Exp $ : xzao572_Lire_config_Eqt_PRV" ;
   
   XZAOT_ConfEqtPRV	vl_ConfEqtPRV;
   char			vl_Lig[255];
   char			*vl_Texte;
   char			vl_Texte_out[255];
   XDY_Entier 		vl_num_ligne = 0;
   int  		vl_retcode = 0;
   XDY_Entier		vl_Resultat_out = 0;
	int		vl_init=1;

   /*A
   ** Test sur le file descriptor passe en parametre d'entree 
   */
 
   if (pa_Fd == NULL)
   {
     XZST_03EcritureTrace(XZSTC_MESSAGE,"xzao572: fichier non existant");
     return ( XDC_NOK );
   }
 
   /*A
   ** Boucle effectuant la lecture ligne par ligne du fichier correspondant
   */
    
   while ( ( vl_retcode = XZAO000_LireLigneFich ( pa_Fd, vl_Lig, sizeof(vl_Lig), &vl_Resultat_out ) ) != XDC_NOK )
   {
       /*A
       ** Comptage des lignes lues du fichier pa_Fd 
       */
    
       vl_num_ligne++; 
      	vl_init = 1; 
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
             ** Extraction des differents champs de vl_ConfEqtPRV
             */
	
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             
             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_LONG, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao572 : Saisie Autoroute non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao572 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
              
			 vl_ConfEqtPRV.Autoroute = atoi ( vl_Texte_out );
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             

             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_LONG, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao572 : Saisie Sens non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao572 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             vl_ConfEqtPRV.Sens = atoi ( vl_Texte_out );
               
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR ); 
             
             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_LONG, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao572 : PR");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao572 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             vl_ConfEqtPRV.PR = atoi ( vl_Texte_out );          
                               
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             
             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_BIT, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao572 : Saisie Critique non valide",vl_Texte);
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao572 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             vl_ConfEqtPRV.Critique = atoi ( vl_Texte_out );
			 
			 vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             
             if ( ( vl_retcode = XZAO000_Verif_Coherence_String ( vl_Texte, XZAO_LONG_LONG, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao572 : Saisie zone  non valide",vl_Texte);
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao572 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }

			 vl_ConfEqtPRV.Zone = atoi ( vl_Texte_out );
                         
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR ); 
             
             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_BIT, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao572 : Saisie Inhibition non valide",vl_Texte);
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao572 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             vl_ConfEqtPRV.Inhibition = atoi ( vl_Texte_out );
             
             /*A
             ** Verification que la fonction utilisateur passee en parametre est non NULL 
             */
    
             if (pa_FonctionUtil == NULL) 
             {
                /*B
                ** Fonction utilisateur inexistante =>
                ** Retour du status XDC_NOK et du numero de ligne 
                */

                XZST_03EcritureTrace(XZSTC_MESSAGE,"xzao572: echec de l'appel de la fonction utilisateur");
                return ( XDC_NOK );
             }

                /*B
                ** Appel de la fonction utilisateur passee en parametre 
                */
 
      		          if ( (vl_retcode = ( *pa_FonctionUtil ) ( vl_ConfEqtPRV, vl_num_ligne ) ) != XDC_OK )
       		         {    
       		            XZST_03EcritureTrace(XZSTC_WARNING,"xzao572 : appel de FonctionUtilisateur s'est mal deroule");
       		            return ( XDC_NOK );
       		         }  
			vl_init = 0;
			}
          }
	
      else 
       /* A
       ** Fin de lecture du fichier 
       ** Retour du status XDC_OK et du parametre *pa_Resultat = NULL 
       */
       
       {  
          *pa_Resultat = 0;
          return ( XDC_OK );
       }
		}
 
   /*A
   ** Fin de la boucle de lecture du fichier
   
   ** Retour du status XDC_OK et du parametre *pa_Resultat = NULL 
   */
   
*pa_Resultat = 0;

return XDC_OK;

}   

/* Fin de la fonction  */

