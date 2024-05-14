/*E*/
/*Fichier : $Id: xzao54.c,v 1.10 2016/07/13 12:43:15 devgfi Exp $      Release : $Revision: 1.10 $        Date : $Date: 2016/07/13 12:43:15 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao54.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module des objets statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	31 Aug 1994	: Creation
* volcic	24 Oct 1994	: Modif recuperation des parametres (v1.9)
* JPL		13/07/16 : Concordance types d'arguments pour ia64 : fonction utilisateur : numero ligne XDY_Entier  1.10
------------------------------------------------------*/

/* fichiers inclus */

#include "xzao.h"

/* definitions constantes */

/* definitions types locaux */
  
/* definition macro locales */

/* declaration variables locales */

static char *version = "$Id: xzao54.c,v 1.10 2016/07/13 12:43:15 devgfi Exp $ : xzao54" ;

/* declaration fonctions internes */

/* definition fonctions externes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*    Lit le fichier configuration des portions et
*    appelle la fonction utilisateur GECONF
* 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int xzao54_Lire_Config_Portion ( FILE  		*va_Fd,
				 XDY_FonctionInt va_FonctionUtil,
				 XDY_Entier   	*va_Resultat) 
				        
/*
* ARGUMENTS EN ENTREE :
*
* FILE *		va_Fd_In
* XDY_Fonction		va_FonctionUtil
*
* ARGUMENTS EN SORTIE :	
*
* XDY_Entier		*va_Resultat:OK=NULL ou ligne l'erreur du systeme
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
*       XZAOT_ConfPortion 	contenu ligne
*       XDY_Entier 		NumLigne
*    La fonction utilisateur retourne XDC_OK ou XDC_NOK . arret code la lecture
*    et code retour XDC_NOK
*
* MECANISMES :
*
* UNIX28
------------------------------------------------------*/

{
   static char *version = "$Id: xzao54.c,v 1.10 2016/07/13 12:43:15 devgfi Exp $ : xzao54_Lire_Config_Portion" ;

   XZAOT_ConfPortion	vl_Portion;
   char			vl_Lig[254];
   char			*vl_Texte;
   char			vl_Texte_out[254];
   XDY_Entier 		vl_num_ligne = 0;
   int  		vl_retcode = 0;
   XDY_Entier		vl_Resultat_out = 0;
   XDY_PR		vl_PR = 0;
   XDY_PR		vl_PRref = 100000;
   char ** 		pl_ptr;
   
   /*A
   ** Test sur le file descriptor passe en parametre d'entree 
   */
 
   if (va_Fd == NULL)
   {
     XZST_03EcritureTrace(XZSTC_MESSAGE,"xzao54: fichier non existant");
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
          ** Affectation du parametre sortie egal au numero la ligne lue
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
             ** Extraction des differents champs vl_Portion
             */
    
             vl_Texte = strtok ( vl_Lig, XZAO_TAB_SEPAR );
             
             if ( ( vl_retcode = XZAO000_Verif_Coherence_String ( vl_Texte, XDC_Lon_Nom , vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"Saisie non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao54 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             strcpy ( vl_Portion.NomPortion, vl_Texte_out );
              
             
             
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             
             if ( ( vl_retcode = XZAO000_Verif_Coherence_String ( vl_Texte, XDC_Lon_NomAuto, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"Saisie non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao54 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             strcpy ( vl_Portion.NomAutoroute, vl_Texte_out );
              
             
             
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             
             if ( ( vl_retcode = XZAO000_Verif_Coherence_String ( vl_Texte, XDC_Lon_NomSite, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"Saisie non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao54 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             strcpy ( vl_Portion.NomDistrict, vl_Texte_out );
              
             
             
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             
             if ( ( vl_retcode = XZAO000_Verif_Coherence_String ( vl_Texte, XDC_Lon_NomSite, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"Saisie non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao54 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             strcpy ( vl_Portion.NomDistrictRecouvrement, vl_Texte_out );
              
             
             
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             
             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_LONG, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"Saisie non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao54 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             vl_Portion.PRdebut = atoi ( vl_Texte_out );
             
             
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             
             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_LONG, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"Saisie non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao54 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             vl_Portion.PRfin = atoi ( vl_Texte_out );
             
             
             
             /*A
             ** Verification que la fonction utilisateur passee en parametre est non NULL 
             */
    
             if (va_FonctionUtil != NULL)
             {
                /*B
                ** Appel la fonction utilisateur passee en parametre 
                */
         
                if ( (vl_retcode = ( *va_FonctionUtil ) ( vl_Portion, vl_num_ligne ) ) != XDC_OK )
                {    
                   XZST_03EcritureTrace(XZSTC_WARNING,"appel FonctionUtilisateur s'est mal deroule");
                   return ( XDC_NOK );
                }  
	     } 
             else
             {
                /*B
                ** Fonction utilisateur inexistante =>
                ** Retour du status XDC_NOK et du numero ligne 
                */

                XZST_03EcritureTrace(XZSTC_MESSAGE,"xzao54: echec l'appel la fonction utilisateur");
                return ( XDC_NOK );
             }
          }
       }
       
       else
       
       /* A
       ** Fin lecture du fichier va_Fd 
       ** Retour du status XDC_OK et du parametre *va_Resultat = NULL 
       */
       
       {  
          *va_Resultat = NULL;
          return ( XDC_OK );
       }
   }
 
   /* Fin la boucle lecture du fichier va_Fd */


   /* Retour du status XDC_OK et du parametre *va_Resultat = NULL */
   
*va_Resultat = NULL;

return XDC_OK;

}     

/* Fin la fonction xzao54_Lire_Config_Portion */

