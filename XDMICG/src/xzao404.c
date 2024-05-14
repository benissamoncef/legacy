/*E*/
/* Fichier : $Id: xzao404.c,v 1.6 2018/11/05 11:36:10 devgfi Exp $        $Revision: 1.6 $        $Date: 2018/11/05 11:36:10 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao404.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module des objets statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer       10 Fev 1997     : Creation (DEM/1395)
* JMG		11/01/12	: type zone DEM/1014
* JPL		13/07/16 : Concordance types d'arguments pour ia64 : fonction utilisateur : numero ligne XDY_Entier  1.3
* JPL		07/02/18 : Migration Linux 64 bits (DEM 1274) : Concordance constantes/types  1.4
* JPL		05/11/18 : Lecture de l'indicateur 'Supprime' en configuration (DEM 1305)  1.6
------------------------------------------------------*/

/* fichiers inclus */

#include "xzao.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: xzao404.c,v 1.6 2018/11/05 11:36:10 devgfi Exp $ : xzao404" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*    Lit le fichier de configuration des Åquipements ZDP (Zone de parcours) et
*    appelle la fonction utilisateur de GECONF
* 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int xzao404_Lire_Config_Eqt_ZDP ( 	FILE  		*pa_Fd,
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
*       xzao404_Lire_Config_Eqt_ZDP 	contenu de ligne
*       XDY_Entier 		NumLigne
*    La fonction utilisateur retourne XDC_OK ou XDC_NOK . arret code de la lecture
*    et code de retour XDC_NOK
*
* MECANISMES :
*
* UNIX28
------------------------------------------------------*/

{
   static char *version = "$Id: xzao404.c,v 1.6 2018/11/05 11:36:10 devgfi Exp $ : xzao404_Lire_Config_Eqt_ZDP" ;
   
   XZAOT_ConfEqtZDP	vl_Eqt;
   char			vl_Lig[255];
   char			*vl_Texte;
   char			vl_Texte_out[255];
   XDY_Entier 		vl_num_ligne = 0;
   int  		vl_retcode = 0;
   XDY_Entier		vl_Resultat_out = 0;
 
   /*A
   ** Test sur le file descriptor passe en parametre d'entree 
   */
 
   if (pa_Fd == NULL)
   {
     XZST_03EcritureTrace(XZSTC_MESSAGE,"xzao404: fichier non existant");
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
             ** Extraction des differents champs de vl_Eqt
             */
    
             vl_Texte = strtok ( vl_Lig, XZAO_TAB_SEPAR );
             
             if ( ( vl_retcode = XZAO000_Verif_Coherence_String ( vl_Texte, XDC_Lon_NomEqt, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao404 : Saisie Nom zone non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao404 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             strcpy ( vl_Eqt.Nom, vl_Texte_out );
              
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             if ( ( vl_retcode = XZAO000_Verif_Coherence_String ( vl_Texte, XDC_Lon_NomAuto, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao404 : Saisie Autoroute non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao404 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             strcpy ( vl_Eqt.Autoroute, vl_Texte_out );
              
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             if ( ( vl_retcode = XZAO000_Verif_Coherence_String ( vl_Texte, XDC_Lon_NomEqt, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao404 : Saisie Maitre non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao404 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             strcpy ( vl_Eqt.NomServeur, vl_Texte_out );
              
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_LONG, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao404 : Saisie PR debut non valide <%s>", vl_Texte);
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao404 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             vl_Eqt.PR_Debut = atoi ( vl_Texte_out );
              
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_UCHAR, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao404 : Saisie Sens non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao404 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             vl_Eqt.Sens = atoi ( vl_Texte_out );
                           
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_INT, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao404 : Saisie code zone  non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao404 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             vl_Eqt.Code = atoi ( vl_Texte_out );
              
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             if ( ( vl_retcode = XZAO000_Verif_Coherence_String ( vl_Texte, XDC_Lon_Nom - 1, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao404 : Saisie libelle non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao404 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             strcpy ( vl_Eqt.Libelle, vl_Texte_out );
              
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             if ( ( vl_retcode = XZAO000_Verif_Coherence_String ( vl_Texte, XDC_Lon_NomSite, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao404 : Saisie Site non valide <%s>", vl_Texte);
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao404 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             strcpy ( vl_Eqt.Site, vl_Texte_out );
              
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_LONG, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao404 : Saisie PR fin non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao404 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             vl_Eqt.PR_Fin = atoi ( vl_Texte_out );
              
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_INT, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao404 : Saisie TDP normal non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao404 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             vl_Eqt.TDP_Normal = atoi ( vl_Texte_out );
              
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_INT, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao404 : Saisie TDP max non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao404 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             vl_Eqt.TDP_Max = atoi ( vl_Texte_out );
              
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_INT, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao404 : Saisie seuil bas non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao404 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             vl_Eqt.Seuil_Bas = atoi ( vl_Texte_out );
              
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_INT, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao404 : Saisie seuil haut non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao404 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             vl_Eqt.Seuil_Haut = atoi ( vl_Texte_out );
              
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_INT, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao404 : Saisie type zone non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao404 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             vl_Eqt.Type_Zone = atoi ( vl_Texte_out );
              
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_BIT, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao404 : Saisie Inhibition non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao404 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             vl_Eqt.Inhibition = atoi ( vl_Texte_out );


             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_BIT, vl_Texte_out ) ) != XDC_OK )
             {
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao404 : Saisie Supprime non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao404 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             vl_Eqt.Supprime = atoi ( vl_Texte_out );


             /*A
             ** Verification que la fonction utilisateur passee en parametre est non NULL 
             */
    
             if (pa_FonctionUtil != NULL)
             {
                /*B
                ** Appel de la fonction utilisateur passee en parametre 
                */
         
                if ( (vl_retcode = ( *pa_FonctionUtil ) ( vl_Eqt, vl_num_ligne ) ) != XDC_OK )
                {    
                   XZST_03EcritureTrace(XZSTC_WARNING,"xzao404 : appel de FonctionUtilisateur s'est mal deroule");
                   return ( XDC_NOK );
                }  
	     } 
             else
             {
                /*B
                ** Fonction utilisateur inexistante =>
                ** Retour du status XDC_NOK et du numero de ligne 
                */

                XZST_03EcritureTrace(XZSTC_MESSAGE,"xzao404: echec de l'appel de la fonction utilisateur");
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

