/*E*/
/*Fichier : $Id: xzao83.c,v 1.6 2016/07/13 12:43:16 devgfi Exp $      Release : $Revision: 1.6 $        Date : $Date: 2016/07/13 12:43:16 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao83.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module des objets statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	08 Sep 1994	: Creation
* volcic	19 Oct 1994	: Ajout verif coherence (v1.2)
* volcic	25 Oct 1994	: Modif recup param (v1.3)
* JPL		07/12/10 : Migration architecture HP ia64 (DEM 961) : adequation format scanf et type d'entier  1.4
* JPL		14/06/12 : Ajout des champs Nom sortie et Indicateur bifurcation (DEM 1033)  1.5
* JPL		13/07/16 : Concordance types d'arguments pour ia64 : fonction utilisateur : numero ligne XDY_Entier  1.6
------------------------------------------------------*/

/* fichiers inclus */

#include "xzao.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: xzao83.c,v 1.6 2016/07/13 12:43:16 devgfi Exp $ : xzao83" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*    Lit le fichier de configuration des echangeurs et
*    appelle la fonction utilisateur de GECONF
* 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int xzao83_Lire_Config_Echangeur ( 	FILE  *va_Fd,
				        	XDY_FonctionInt va_FonctionUtil,
				        	XDY_Entier   *va_Resultat ) 
				        
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
*       XZAOT_ConfEchang 	contenu de ligne
*       XDY_Entier 		NumLigne
*    La fonction utilisateur retourne XDC_OK ou XDC_NOK . arret code de la lecture
*    et code de retour XDC_NOK
*
* MECANISMES :
*
* UNIX28
------------------------------------------------------*/

{
   static char *version = "$Id: xzao83.c,v 1.6 2016/07/13 12:43:16 devgfi Exp $ : xzao83_Lire_Config_Echangeur" ;
   
   XZAOT_ConfEchang	vl_Echangeur;
   char			vl_Lig[255];
   char			vl_String[255];
   char			*vl_Texte;
   char			vl_Texte_out[255];
   XDY_Entier 		vl_num_ligne = 0;
   int  		vl_retcode = 0;
   int			vl_NbCarLu = 0;
   int			vl_Longueur = 0;
   XDY_Entier		vl_Resultat_out = 0;
 
   /*A
   ** Test sur le file descriptor passe en parametre d'entree 
   */
 
   if (va_Fd == NULL)
   {
     XZST_03EcritureTrace(XZSTC_MESSAGE,"xzao83: fichier non existant");
     return ( XDC_NOK );
   }
 
   /*A
   ** Boucle effectuant la lecture ligne par ligne du fichier correspondant È va_Fd
   */
    
   while ( ( vl_retcode = XZAO000_LireLigneFich ( va_Fd, vl_Lig, sizeof(vl_Lig), &vl_Resultat_out ) ) != XDC_NOK )
   {
       /*A
       ** Comptage des lignes lues du fichier va_Fd 
       */
    
       vl_num_ligne++; 
       
       if (vl_Resultat_out != XZAO_EOF )
       { 
          /*A
          ** Affectation du parametre de sortie egal au numero de la ligne lue
          */
          
          *va_Resultat = vl_num_ligne;
             
          /*A
          ** Verification que la ligne n'est pas un commentaire
          ** Presence du caractere #
          */
          
          vl_retcode = strcspn ( vl_Lig, XZAO_COMMENT );

          if (vl_retcode != 0) 

          {
             /*A
             ** Extraction des differents champs de vl_Echangeur
             */
    
             vl_Texte = strtok ( vl_Lig, XZAO_TAB_SEPAR );
             
             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_BIT, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao83 : Saisie  non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao83 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             vl_Echangeur.Creation = atoi ( vl_Texte_out );
             
             
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             
             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_UCHAR, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao83 : Saisie  non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao83 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             vl_Echangeur.Numero = atoi ( vl_Texte_out );
             
             
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             
             if ( ( vl_retcode = XZAO000_Verif_Coherence_String ( vl_Texte, XDC_Lon_Nom, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao83 : Saisie  non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao83 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             strcpy ( vl_Echangeur.NomEchangeur, vl_Texte_out );
              
             
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             
             if ( ( vl_retcode = XZAO000_Verif_Coherence_String ( vl_Texte, XDC_Lon_Abrev, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao83 : Saisie  non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao83 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             strcpy ( vl_Echangeur.Abreviation, vl_Texte_out );
             
             
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             
             if ( ( vl_retcode = XZAO000_Verif_Coherence_String ( vl_Texte, XDC_Lon_Sortie, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao83 : Saisie  non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao83 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             strcpy ( vl_Echangeur.NomSortie, vl_Texte_out );
              
             
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             
             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_BIT, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao83 : Saisie  non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao83 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             vl_Echangeur.Bifurcation = atoi ( vl_Texte_out );
              
             
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             
             if ( ( vl_retcode = XZAO000_Verif_Coherence_String ( vl_Texte, XDC_Lon_NomAuto, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao83 : Saisie  non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao83 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             strcpy ( vl_Echangeur.Autoroute, vl_Texte_out );
              
             
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             
             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_LONG, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao83 : Saisie  non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao83 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             sscanf ( vl_Texte_out, "%d", &vl_Echangeur.PR );
             
             
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             
             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_LONG, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao83 : Saisie  non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao83 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             sscanf ( vl_Texte_out, "%d", &vl_Echangeur.PRentreeSens1 );
             
             
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             
             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_UCHAR, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao83 : Saisie  non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao83 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             vl_Echangeur.VoiesEntreeSens1 = atoi ( vl_Texte_out );
             
             
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             
             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_BIT, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao83 : Saisie  non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao83 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             vl_Echangeur.PeageEntreeSens1 = atoi ( vl_Texte_out );
             
             
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             
             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_LONG, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao83 : Saisie  non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao83 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             sscanf ( vl_Texte_out, "%d", &vl_Echangeur.PRentreeSens2 );
             
             
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             
             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_UCHAR, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao83 : Saisie  non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao83 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             vl_Echangeur.VoiesEntreeSens2  = atoi ( vl_Texte_out );
             
             
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             
             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_BIT, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao83 : Saisie  non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao83 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             vl_Echangeur.PeageEntreeSens2  = atoi ( vl_Texte_out );
             
             
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             
             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_LONG, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao83 : Saisie  non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao83 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             sscanf ( vl_Texte_out, "%d", &vl_Echangeur.PRsortieSens1 );
             
             
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             
             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_UCHAR, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao83 : Saisie  non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao83 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             vl_Echangeur.VoiesSortieSens1 = atoi ( vl_Texte_out );
             
             
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             
             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_BIT, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao83 : Saisie  non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao83 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             vl_Echangeur.PeageSortieSens1 = atoi ( vl_Texte_out );
             
            
            
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             
             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_LONG, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao83 : Saisie  non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao83 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             sscanf ( vl_Texte_out, "%d", &vl_Echangeur.PRsortieSens2 );
             
             
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             
             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_UCHAR, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao83 : Saisie  non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao83 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             vl_Echangeur.VoiesSortieSens2 = atoi ( vl_Texte_out );
             
             
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             
             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_BIT, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao83 : Saisie  non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao83 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             vl_Echangeur.PeageSortieSens2 = atoi ( vl_Texte_out );
             
             
             
             /*A
             ** Verification que la fonction utilisateur passee en parametre est non NULL 
             */
    
             if (va_FonctionUtil != NULL)
             {
                /*B
                ** Appel de la fonction utilisateur passee en parametre 
                */
         
                if ( (vl_retcode = ( *va_FonctionUtil ) ( vl_Echangeur, vl_num_ligne ) ) != XDC_OK )
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

                XZST_03EcritureTrace(XZSTC_MESSAGE,"xzao83: echec de l'appel de la fonction utilisateur");
                return ( XDC_NOK );
             }
          }
       }
       
       else
       
       /* A
       ** Fin de lecture du fichier va_Fd 
       ** Retour du status XDC_OK et du parametre *va_Resultat = NULL 
       */
       
       {  
          *va_Resultat = NULL;
          return ( XDC_OK );
       }
   }
 
   /*A
   ** Fin de la boucle de lecture du fichier va_Fd 
   ** Retour du status XDC_OK et du parametre *va_Resultat = NULL 
   */
   
*va_Resultat = NULL;

return XDC_OK;

}   
/* Fin de la fonction xzao83_Lire_Config_Echangeur */

