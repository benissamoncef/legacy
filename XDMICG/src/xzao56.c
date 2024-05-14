/*E*/
/*  Fichier : 	$Id: xzao56.c,v 1.10 2016/07/13 12:43:15 devgfi Exp $      Release : $Revision: 1.10 $        Date : $Date: 2016/07/13 12:43:15 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao56.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic       22 Sep 1994     : Creation
* volcic       18 Oct 1994     : Modif XDY_FonctionInt (v1.2)
* volcic       18 Oct 1994     : Modif Recup param (v1.3)
* volcic       18 Oct 1994     : Modif Recup param (v1.4)
* volcic       18 Oct 1994     : Modif Recup param (v1.5)
* volcic       19 Oct 1994     : Modif Recup param (v1.6)
* volcic       25 Oct 1994     : Modif Recup param (v1.7)
* JPL		13/07/16 : Concordance types d'arguments pour ia64 : fonction utilisateur : numero ligne XDY_Entier  1.10
------------------------------------------------------*/

/* fichiers inclus */

#include "xzao.h"

static char *version = "$Id: xzao56.c,v 1.10 2016/07/13 12:43:15 devgfi Exp $ : xzao56" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*    Lit le fichier de configuration des noeuds et
*    appelle la fonction utilisateur de GECONF
* 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int xzao56_Lire_Config_Noeud   ( FILE  		*va_Fd,
				 XDY_FonctionInt va_FonctionUtil,
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
*       XZAOT_ConfNoeud 	contenu de ligne
*       XDY_Entier 		NumLigne
*    La fonction utilisateur retourne XDC_OK ou XDC_NOK . arret code de la lecture
*    et code de retour XDC_NOK
*
* MECANISMES :
*
* UNIX28
------------------------------------------------------*/

{
   static char *version = "$Id: xzao56.c,v 1.10 2016/07/13 12:43:15 devgfi Exp $ : xzao56_Lire_Config_Noeud" ;
   
   XZAOT_ConfNoeud	vl_Noeud;
   char			vl_Lig[256];
   char			vl_String[256];
   char			*vl_Texte;
   char			vl_Texte_out[256];
   XDY_Entier 		vl_num_ligne = 0;
   int  		vl_retcode = 0;
   int  		vl_Inter = 0;
   int			vl_NbCarLu = 0;
   int			vl_Longueur = 0;
   XDY_Entier		vl_Resultat_out = 0;
 
   /*A
   ** Test sur le file descriptor passe en parametre d'entree 
   */
 
   if (va_Fd == NULL)
   {
     XZST_03EcritureTrace(XZSTC_MESSAGE,"xzao56: fichier non existant");
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
          ** Presenece du caractere #
          */
          
          vl_retcode = strcspn ( vl_Lig, XZAO_COMMENT );

          if (vl_retcode != 0) 

          {
             /*A
             ** Extraction des differents champs de vl_District
             */
    
             vl_Texte = strtok ( vl_Lig, XZAO_TAB_SEPAR );
             
             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_BIT, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao56 : Saisie  non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao56 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             vl_Noeud.Creation = atoi ( vl_Texte_out );
             
             
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             
             if ( ( vl_retcode = XZAO000_Verif_Coherence_String ( vl_Texte, XDC_Lon_Nom, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao56 : Saisie  non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao56 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             strcpy ( vl_Noeud.NomPortion1, vl_Texte_out );
             
             
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             
             if ( ( vl_retcode = XZAO000_Verif_Coherence_String ( vl_Texte, XDC_Lon_Nom, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao56 : Saisie  non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao56 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             strcpy ( vl_Noeud.NomPortion2, vl_Texte_out );
             
             
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             
             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_BIT, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao56 : Saisie  non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao56 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             vl_Noeud.ExtremitePortion1 = atoi ( vl_Texte_out );
             
             
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             
             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_BIT, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao56 : Saisie  non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao56 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             vl_Noeud.ExtremitePortion2 = atoi ( vl_Texte_out );
             
             
             /*A
             ** Verification que la fonction utilisateur passee en parametre est non NULL 
             */
    
             if (va_FonctionUtil != NULL)
             {
                /*B
                ** Appel de la fonction utilisateur passee en parametre 
                */
         
                if ( (vl_retcode = ( *va_FonctionUtil ) ( vl_Noeud, vl_num_ligne ) ) != XDC_OK )
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

                XZST_03EcritureTrace(XZSTC_MESSAGE,"xzao56: echec de l'appel de la fonction utilisateur");
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

        

