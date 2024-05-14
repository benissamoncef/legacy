/*E*/
/* Fichier : $Id: xzao571.c,v 1.3 2018/02/09 15:22:36 devgfi Exp $        Release : $Revision: 1.3 $        Date : $Date: 2018/02/09 15:22:36 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao571.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module des objets statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* VR       18 Jan 2012     : Creation (DEM/1016)
* JPL		13/07/16 : Concordance types d'arguments pour ia64 : fonction utilisateur : numero ligne XDY_Entier  1.2
* JPL		07/02/18 : Migration Linux 64 bits (DEM 1274) : Concordance constantes/types  1.3
------------------------------------------------------*/

/* fichiers inclus */

#include "xzao.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: xzao571.c,v 1.3 2018/02/09 15:22:36 devgfi Exp $ : xzao571" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*    Lit le fichier de configuration des Zone de Regulation XDF_CONF_EQT_REG_ZON et
*    appelle la fonction utilisateur de GECONF
* 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int xzao571_Lire_Config_Zone_Regul ( 	FILE  		*pa_Fd,
					XDY_FonctionInt pa_FonctionUtil,
					XDY_Entier   	*pa_Resultat ) 
				        
/*
* ARGUMENTS EN ENTREE :
*
* FILE			pa_Fd
* XZAOT_ConfZoneReg	pa_FonctionUtil
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
*       xzao571_Lire_Config_Zone_Regul 	contenu de ligne
*    La fonction utilisateur retourne XDC_OK ou XDC_NOK . arret code de la lecture
*    et code de retour XDC_NOK
*
* MECANISMES :
*
* UNIX28
------------------------------------------------------*/

{
   static char *version = "$Id: xzao571.c,v 1.3 2018/02/09 15:22:36 devgfi Exp $ : xzao571_Lire_Config_Zone_Regul" ;
   
   XZAOT_ConfZoneReg	vl_zone_reg;
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
     XZST_03EcritureTrace(XZSTC_MESSAGE,"xzao571: fichier non existant");
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
             ** Extraction des differents champs 
             */
    
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
	     XZST_03EcritureTrace(XZSTC_WARNING,"xzao571 : vl_Texte = %s vl_Texte_out = %s",vl_Texte,vl_Texte_out);
             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_INT, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao571 : Saisie numero zone non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao571 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             vl_zone_reg.Numero = atoi ( vl_Texte_out ); 
	     XZST_03EcritureTrace(XZSTC_WARNING,"xzao571 : vl_Texte = %s vl_Texte_out = %s",vl_Texte,vl_Texte_out);
              
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             if ( ( vl_retcode = XZAO000_Verif_Coherence_String ( vl_Texte, 50, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao571 : Saisie nom zone non valide <%s>", vl_Texte);
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao571 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             strcpy(vl_zone_reg.Nom, vl_Texte_out );
             XZST_03EcritureTrace(XZSTC_WARNING,"xzao571 : vl_Texte = %s vl_Texte_out = %s",vl_Texte,vl_Texte_out);

             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             if ( ( vl_retcode = XZAO000_Verif_Coherence_String ( vl_Texte, XDC_Lon_NomAuto, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao571 : Saisie autoroute non valide <%s>", vl_Texte);
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao571 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             strcpy(vl_zone_reg.Autoroute, vl_Texte_out );
             XZST_03EcritureTrace(XZSTC_WARNING,"xzao571 : vl_Texte = %s vl_Texte_out = %s",vl_Texte,vl_Texte_out);

             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_INT , vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao571 : Saisie sens zone non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao571 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
			 vl_zone_reg.Sens = atoi ( vl_Texte_out );
             XZST_03EcritureTrace(XZSTC_WARNING,"xzao571 : vl_Texte = %s vl_Texte_out = %s",vl_Texte,vl_Texte_out);

             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_INT, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao571 : Saisie pr debut zone non valide <%s>", vl_Texte);
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao571 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
	         vl_zone_reg.PR_Debut = atoi ( vl_Texte_out );
             XZST_03EcritureTrace(XZSTC_WARNING,"xzao571 : vl_Texte = %s vl_Texte_out = %s",vl_Texte,vl_Texte_out);

             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_INT, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao571 : Saisie PR fin zone non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao571 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             vl_zone_reg.PR_Fin = atoi ( vl_Texte_out );
             XZST_03EcritureTrace(XZSTC_WARNING,"xzao571 : vl_Texte = %s vl_Texte_out = %s",vl_Texte,vl_Texte_out); 
             
	     vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_INT, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao571 : Saisie Vitesse non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao571 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             vl_zone_reg.Vitesse = atoi ( vl_Texte_out );
             XZST_03EcritureTrace(XZSTC_WARNING,"xzao571 : vl_Texte = %s vl_Texte_out = %s",vl_Texte,vl_Texte_out);

             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_BIT, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao571 : Saisie Inhibition non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao571 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             vl_zone_reg.Inhibition = atoi ( vl_Texte_out );
	     XZST_03EcritureTrace(XZSTC_WARNING,"xzao571 : vl_Texte = %s vl_Texte_out = %s",vl_Texte,vl_Texte_out);

			 vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
			 if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_BIT, vl_Texte_out ) ) != XDC_OK )
             {
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao571 : Saisie mode horaire non valide");
				XZST_03EcritureTrace(XZSTC_WARNING,"xzao571 : retourne %d",XDC_NOK);
				return ( XDC_NOK );
             }
			 vl_zone_reg.Mode_Horaire = atoi ( vl_Texte_out );
        XZST_03EcritureTrace(XZSTC_WARNING,"xzao571 : vl_Texte = %s vl_Texte_out = %s",vl_Texte,vl_Texte_out);

             /*A
             ** Verification que la fonction utilisateur passee en parametre est non NULL 
             */
    
             if (pa_FonctionUtil == NULL) 
             {
                /*B
                ** Fonction utilisateur inexistante =>
                ** Retour du status XDC_NOK et du numero de ligne 
                */

                XZST_03EcritureTrace(XZSTC_MESSAGE,"xzao571: echec de l'appel de la fonction utilisateur");
                return ( XDC_NOK );
             }

                /*B
                ** Appel de la fonction utilisateur passee en parametre 
                */
 
      		          if ( (vl_retcode = ( *pa_FonctionUtil ) ( vl_zone_reg, vl_num_ligne ) ) != XDC_OK )
       		         {    
       		            XZST_03EcritureTrace(XZSTC_WARNING,"xzao571 : appel de FonctionUtilisateur s'est mal deroule");
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

