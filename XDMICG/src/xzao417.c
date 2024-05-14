/*E*/
/*Fichier : $Id: xzao417.c,v 1.4 2018/02/09 15:21:57 devgfi Exp $      Release : $Revision: 1.4 $        Date : $Date: 2018/02/09 15:21:57 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao417.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module des objets statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* Guilhou	02/01/98	: creation (dem/1534)
* Guilhou	20/09/00	: modif champs pour traitement recherche pmv dans tdpac 1.2
* JPL		13/07/16 : Concordance types d'arguments pour ia64 : fonction utilisateur : numero ligne XDY_Entier  1.3
* JPL		07/02/18 : Migration Linux 64 bits (DEM 1274) : Concordance constantes/types  1.4
------------------------------------------------------*/

/* fichiers inclus */

#include "xzao.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: xzao417.c,v 1.4 2018/02/09 15:21:57 devgfi Exp $ : xzao417" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*    Lit le fichier de configuration des zones d'influence PMV et
*    appelle la fonction utilisateur de GECONF
* 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int xzao417_Lire_Config_Zon_PMV ( 	FILE  		*pa_Fd,
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
   char			vl_autoroute_pmv[10];
   XDY_PR		vl_pr_pmv;
   XDY_Sens		vl_sens_pmv;
   char			vl_site_pmv[10];
   char			vl_autoroute_zone[10];
   XDY_Sens		vl_sens_zone;
   XDY_PR		vl_debut_zone,vl_fin_zone;
   char			vl_direction_zone[20];
   XDY_Booleen		vl_inhibition_zone;
   XDY_Sens		vl_sens_1_pmv_amont,vl_sens_1_pmv_amont_bis,vl_sens_1_pmv_amont_ter;
   XDY_PR		vl_pr_1_pmv_amont,vl_pr_1_pmv_amont_bis,vl_pr_1_pmv_amont_ter;
   char			vl_autoroute_1_pmv_amont[10];
   char                 vl_autoroute_1_pmv_amont_bis[10];
   char                 vl_autoroute_1_pmv_amont_ter[10];
   int			vl_distance_1_pmv_amont;
   int			vl_distance_1_pmv_amont_bis;
   int			vl_distance_1_pmv_amont_ter;

   /*A
   ** Test sur le file descriptor passe en parametre d'entree 
   */
 
   if (pa_Fd == NULL)
   {
     XZST_03EcritureTrace(XZSTC_MESSAGE,"xzao417: fichier non existant");
     return ( XDC_NOK );
   }
 
   /*A
   ** Boucle effectuant la lecture ligne par ligne du fichier correspondant È pa_Fd
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
             if ( ( vl_retcode = XZAO000_Verif_Coherence_String ( vl_Texte, 11, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao417 : Saisie Autoroute PMV non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao417 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
	     strcpy(vl_autoroute_pmv, vl_Texte_out);
              
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_UCHAR, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao417 : Saisie sens PMV non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao417 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             vl_sens_pmv = atoi ( vl_Texte_out ); 
              
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_LONG, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao417 : Saisie PR PMV non valide <%s>", vl_Texte);
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao417 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             vl_pr_pmv= atoi ( vl_Texte_out );
              
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             if ( ( vl_retcode = XZAO000_Verif_Coherence_String ( vl_Texte, XDC_Lon_Nom - 1, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao417 : Saisie site pmv non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao417 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
	     strcpy(vl_site_pmv,vl_Texte_out);
                           
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             if ( ( vl_retcode = XZAO000_Verif_Coherence_String ( vl_Texte, 11, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao417 : Saisie autoroute zone  non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao417 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
	     strcpy(vl_autoroute_zone, vl_Texte_out);
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_UCHAR , vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao417 : Saisie sens zone non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao417 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
	     vl_sens_zone = atoi ( vl_Texte_out );
              
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_LONG, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao417 : Saisie pr debut zone non valide <%s>", vl_Texte);
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao417 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
	     vl_debut_zone = atoi ( vl_Texte_out );
              
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_LONG, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao417 : Saisie PR fin zone non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao417 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             vl_fin_zone = atoi ( vl_Texte_out );
              
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             if ( ( vl_retcode = XZAO000_Verif_Coherence_String ( vl_Texte, 16, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao417 : Saisie drection non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao417 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
	     strcpy(vl_direction_zone,vl_Texte_out);
              
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_BIT, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao417 : Saisie Inhibition non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao417 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             vl_inhibition_zone = atoi ( vl_Texte_out );
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_LONG, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao417 : Saisie PR 1 PMV AMONT non valide <%s>", vl_Texte);
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao417 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             vl_pr_1_pmv_amont = atoi ( vl_Texte_out );

             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_UCHAR, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao417 : Saisie sens 1 PMV AMONT non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao417 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             vl_sens_1_pmv_amont = atoi ( vl_Texte_out ); 

             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             if ( ( vl_retcode = XZAO000_Verif_Coherence_String ( vl_Texte, 11, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao417 : Saisie autoroute 1 PMV AMONT  non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao417 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
	     strcpy(vl_autoroute_1_pmv_amont, vl_Texte_out);

             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_LONG, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao417 : Saisie distance 1 PMV AMONT non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao417 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             vl_distance_1_pmv_amont = atoi ( vl_Texte_out ); 


             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_LONG, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao417 : Saisie PR 1 PMV AMONT bis non valide <%s>", vl_Texte);
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao417 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             vl_pr_1_pmv_amont_bis = atoi ( vl_Texte_out );

             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_UCHAR, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao417 : Saisie sens 1 PMV AMONT bis non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao417 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             vl_sens_1_pmv_amont_bis = atoi ( vl_Texte_out ); 

             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             if ( ( vl_retcode = XZAO000_Verif_Coherence_String ( vl_Texte, 11, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao417 : Saisie autoroute 1 PMV AMONT bis non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao417 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
	     strcpy(vl_autoroute_1_pmv_amont_bis, vl_Texte_out);

             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_LONG, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao417 : Saisie distance 1 PMV AMONT bis non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao417 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             vl_distance_1_pmv_amont_bis = atoi ( vl_Texte_out ); 

             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_LONG, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao417 : Saisie PR 1 PMV AMONT ter non valide <%s>", vl_Texte);
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao417 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             vl_pr_1_pmv_amont_ter = atoi ( vl_Texte_out );

             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_UCHAR, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao417 : Saisie sens 1 PMV AMONT ter non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao417 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             vl_sens_1_pmv_amont_ter = atoi ( vl_Texte_out ); 

             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             if ( ( vl_retcode = XZAO000_Verif_Coherence_String ( vl_Texte, 11, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao417 : Saisie autoroute 1 PMV AMONT ter non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao417 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
	     strcpy(vl_autoroute_1_pmv_amont_ter, vl_Texte_out);

             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_LONG, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao417 : Saisie distance 1 PMV AMONT ter non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao417 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             vl_distance_1_pmv_amont_ter = atoi ( vl_Texte_out ); 

             /*A
             ** Verification que la fonction utilisateur passee en parametre est non NULL 
             */
    
             if (pa_FonctionUtil != NULL)
             {
                /*B
                ** Appel de la fonction utilisateur passee en parametre 
                */
         
                if ( (vl_retcode = ( *pa_FonctionUtil ) ( vl_autoroute_pmv
							  ,vl_sens_pmv
							  ,vl_pr_pmv
							  ,vl_site_pmv
							  ,vl_autoroute_zone
							  ,vl_sens_zone
							  ,vl_debut_zone
							  ,vl_fin_zone
							  ,vl_direction_zone
							  ,vl_inhibition_zone
							  ,vl_pr_1_pmv_amont
							  ,vl_sens_1_pmv_amont
							  ,vl_autoroute_1_pmv_amont
							  ,vl_distance_1_pmv_amont
							  ,vl_pr_1_pmv_amont_bis
							  ,vl_sens_1_pmv_amont_bis
							  ,vl_autoroute_1_pmv_amont_bis
							  ,vl_distance_1_pmv_amont_bis
							  ,vl_pr_1_pmv_amont_ter
							  ,vl_sens_1_pmv_amont_ter
							  ,vl_autoroute_1_pmv_amont_ter
							  ,vl_distance_1_pmv_amont_ter
							  , vl_num_ligne ) ) != XDC_OK )
                {    
                   XZST_03EcritureTrace(XZSTC_WARNING,"xzao417 : appel de FonctionUtilisateur s'est mal deroule");
                   return ( XDC_NOK );
                }  
	     } 
             else
             {
                /*B
                ** Fonction utilisateur inexistante =>
                ** Retour du status XDC_NOK et du numero de ligne 
                */

                XZST_03EcritureTrace(XZSTC_MESSAGE,"xzao417: echec de l'appel de la fonction utilisateur");
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

